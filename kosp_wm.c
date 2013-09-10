char *atom_names[] = {
    "KWM_WIN_ICON",
    "_MOTIF_WM_HINTS",
    "_WIN_WORKSPACE",
    "_WIN_HINTS",
    "_WIN_LAYER",
    "_NET_CLIENT_LIST",
    "_WIN_CLIENT_LIST",
    "_WIN_WORKSPACE_COUNT",
    "_WIN_STATE",
    "WM_STATE",
    "_NET_NUMBER_OF_DESKTOPS",
    "_NET_CURRENT_DESKTOP",
    "_NET_WM_STATE",
    "_NET_WM_STATE_ABOVE",
    "_NET_SUPPORTED",
    "_NET_WM_WINDOW_TYPE",
    "_NET_WM_WINDOW_TYPE_DOCK",
    "_NET_WM_DESKTOP",
    "_NET_WM_NAME",
    "_NET_WM_STRUT",
    "_NET_WM_STRUT_PARTIAL",
    "_NET_WM_STATE_STICKY",
    "_NET_WM_WINDOW_TYPE_DESKTOP",
    "UTF8_STRING",
};

#define ATOM_COUNT (sizeof (atom_names) / sizeof (atom_names[0]))

Atom atoms[ATOM_COUNT];

#define atom_KWM_WIN_ICON atoms[0]
#define atom__MOTIF_WM_HINTS atoms[1]
#define atom__WIN_WORKSPACE atoms[2]
#define atom__WIN_HINTS atoms[3]
#define atom__WIN_LAYER atoms[4]
#define atom__NET_CLIENT_LIST atoms[5]
#define atom__WIN_CLIENT_LIST atoms[6]
#define atom__WIN_WORKSPACE_COUNT atoms[7]
#define atom__WIN_STATE atoms[8]
#define atom_WM_STATE atoms[9]
#define atom__NET_NUMBER_OF_DESKTOPS atoms[10]
#define atom__NET_CURRENT_DESKTOP atoms[11]
#define atom__NET_WM_STATE atoms[12]
#define atom__NET_WM_STATE_ABOVE atoms[13]
#define atom__NET_SUPPORTED atoms[14]
#define atom__NET_WM_WINDOW_TYPE atoms[15]
#define atom__NET_WM_WINDOW_TYPE_DOCK atoms[16]
#define atom__NET_WM_DESKTOP atoms[17]
#define atom__NET_WM_NAME atoms[18]
#define atom__NET_WM_STRUT atoms[19]
#define atom__NET_WM_STRUT_PARTIAL atoms[20]
#define atom__NET_WM_STATE_STICKY atoms[21]
#define atom__NET_WM_WINDOW_TYPE_DESKTOP atoms[22]
#define atom_UTF8_STRING atoms[23]


void *
get_prop_data (Window win, Atom prop, Atom type, int *items)
{
    Atom type_ret;
    int format_ret;
    unsigned long items_ret;
    unsigned long after_ret;
    unsigned char *prop_data;

    prop_data = 0;

    XGetWindowProperty (dd, win, prop, 0, 0x7fffffff, False,
                              type, &type_ret, &format_ret, &items_ret,
                              &after_ret, &prop_data);
    if (items)
        *items = items_ret;

    return prop_data;
}

void
get_task_hinticon (task *tk)
{
    XWMHints *hin;

    tk->icon = None;
    tk->mask = None;

    hin = (XWMHints *) get_prop_data (tk->win, XA_WM_HINTS, XA_WM_HINTS, 0);
    if (hin)
    {
        if ((hin->flags & IconPixmapHint))
        {
            if ((hin->flags & IconMaskHint))
            {
                tk->mask = hin->icon_mask;
            }

            tk->icon = hin->icon_pixmap;
            tk->icon_copied = 1;
            scale_icon (tk);
        }
        XFree (hin);
    }

    if (tk->icon == None)
    {
        tk->icon = generic_icon;
        tk->mask = generic_mask;
    }
}

void
get_task_kdeicon (task *tk)
{
    unsigned long *data;

    data = get_prop_data (tk->win, atom_KWM_WIN_ICON, atom_KWM_WIN_ICON, 0);
    if (data)
    {
        tk->icon = data[0];
        tk->mask = data[1];
        XFree (data);
    }
}

/* returns whether the window is visible on the desktop */
int
is_visible_on_desktop (Window win, int desk)
{
    int client_desk = -1;
    unsigned long *data;

    if (wm_use_ewmh)
        data = get_prop_data (win, atom__NET_WM_DESKTOP, XA_CARDINAL, 0);
    else
        data = get_prop_data (win, atom__WIN_WORKSPACE, XA_CARDINAL, 0);

    if (data)
    {
        client_desk = *data;
        XFree (data);
    }

    /* If the client_desk is -1, it is visible on all desktops */
    return (client_desk == -1) || (client_desk == desk);
}

/* index of the currently displayed desktop */
int
get_current_desktop ()
{
    int desk = 0;
    unsigned long *data;

    if (wm_use_ewmh)
        data = get_prop_data (root_win, atom__NET_CURRENT_DESKTOP, XA_CARDINAL, 0);
    else
        data = get_prop_data (root_win, atom__WIN_WORKSPACE, XA_CARDINAL, 0);

    if (data)
    {
        desk = *data;
        XFree (data);
    }
    return desk;
}

int
is_hidden (Window win)
{
    unsigned long *data;
    int ret = 0;

    data = get_prop_data (win, atom__WIN_HINTS, XA_CARDINAL, 0);
    if (data)
    {
        if ((*data) & WIN_HINTS_SKIP_TASKBAR)
            ret = 1;
        XFree (data);
    }

    return ret;
}

int
is_iconified (Window win)
{
    unsigned long *data;
    int ret = 0;

    data = get_prop_data (win, atom_WM_STATE, atom_WM_STATE, 0);
    if (data)
    {
        if (data[0] == IconicState)
            ret = 1;
        XFree (data);
    }
    return ret;
}

/* window name
The returned pointer must be freed using XFree() after use.

TODO: The encoding for WM_NAME can be STRING or COMPOUND_TEXT. In any case
this encoding should be normalized before returning from this function. */
char*
get_window_name (Window win)
{
    char* res = NULL;
    /* try EWMH's _NET_WM_NAME first */
    res = get_prop_data (win, atom__NET_WM_NAME, atom_UTF8_STRING, 0);
    if (!res)
        /* fallback to WM_NAME */
        res = get_prop_data (win, XA_WM_NAME, XA_STRING, 0);
    return res;
}

void
switch_desk (taskbar * tb, int rel)
{
    XClientMessageEvent xev;
    unsigned long *data, max_desks;
    int to_desktop;

    if (wm_use_ewmh)
        data = get_prop_data (root_win, atom__NET_NUMBER_OF_DESKTOPS, XA_CARDINAL, 0);
    else
        data = get_prop_data (root_win, atom__WIN_WORKSPACE_COUNT, XA_CARDINAL, 0);
    if (!data)
        /* number of workspaces not available */
        return;

    max_desks = *data;
    XFree (data);

    /* who doesn't want this to wrap? */
    if (tb->my_desktop == 0)
    {
        if (rel < 0)
        {
            to_desktop = max_desks - 1;
        }
        else
        {
            to_desktop = rel;
        }
    }
    else if (tb->my_desktop >= max_desks - 1)
    {
        if (rel > 0)
        {
            to_desktop = 0;
        }
        else
        {
            to_desktop = max_desks - 2;
        }
    }
    else
    {
        to_desktop = tb->my_desktop + rel;
    }


    if (max_desks <= to_desktop)
        return;

    xev.type = ClientMessage;
    xev.window = root_win;
    if (wm_use_ewmh)
        xev.message_type = atom__NET_CURRENT_DESKTOP;
    else
        xev.message_type = atom__WIN_WORKSPACE;
    xev.format = 32;
    xev.data.l[0] = to_desktop;
    xev.data.l[1] = 0;  /* explicitly not setting the time-stamp */
    XSendEvent(dd, root_win, False,
            SubstructureNotifyMask | SubstructureRedirectMask,
            (XEvent *) &xev);
}

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
void strut_init(Window WindowId)
{
    unsigned int strut[12];
    Bool xin_exists;
    Bool xin_active;
    int xin_num_screens = 0;
    int event_base;
    int error_base;
    XineramaScreenInfo *xin_screens;

    /* first, the simple one */
    strut[0] = 0;               /* left */
    strut[1] = 0;               /* right */
    strut[2] = 0;               /* top */
    strut[3] = WINHEIGHT;       /* bottom */

    XChangeProperty(dd,
            WindowId,
            atom__NET_WM_STRUT,
            XA_CARDINAL,
            32,
            PropModeReplace,
            (const unsigned char *) &strut[0],
            4);

    /* the partial, if Xinerama is supported */
    xin_exists = XineramaQueryExtension(dd,
            &event_base,
            &error_base);

    if (xin_exists == False)
    {
        return;
    }

    xin_active = XineramaIsActive(dd);

    if (xin_active == False)
    {
        return;
    }

    xin_screens = XineramaQueryScreens(dd,
            &xin_num_screens);

    if (xin_screens == NULL ||
            xin_num_screens < 2)
    {
        return;
    }

    strut[0] = 0;               /* left */
    strut[1] = 0;               /* right */
    strut[2] = 0;               /* top */
    strut[3] = WINHEIGHT;       /* bottom */
    strut[4] = 0;               /* left_start_y */
    strut[5] = 0;               /* left_end_y */
    strut[6] = 0;               /* right_start_y */
    strut[7] = 0;               /* right_end_y */
    strut[8] = 0;               /* top_start_x */
    strut[9] = 0;               /* top_end_x */
#if 0
    strut[10] = xin_screens[0].x_org;   /* bottom_start_x */
    strut[11] = xin_screens[0].x_org + xin_screens[0].width;    /* bottom_end_x */
#endif
    strut[10] = 0;   /* bottom_start_x */
    strut[11] = 0;    /* bottom_end_x */

    XFree(xin_screens);

    XChangeProperty(dd,
            WindowId,
            atom__NET_WM_STRUT_PARTIAL,
            XA_CARDINAL,
            32,
            PropModeReplace,
            (const unsigned char *) &strut[0],
            12);
}

void
set_prop (Window win, Atom at, long val)
{
    XChangeProperty (dd, win, at, XA_CARDINAL, 32,
                          PropModeReplace, (unsigned char *) &val, 1);
}


