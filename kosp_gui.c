void
gui_draw_vline (taskbar * tb, int x)
{
    set_foreground (4);
    draw_line (tb, x, 0, x, WINHEIGHT);
    set_foreground (3);
    draw_line (tb, x + 1, 0, x + 1, WINHEIGHT);
}

void
gui_draw_task (taskbar * tb, task * tk)
{
    int len;
    int x = tk->pos_x;
    int taskw = tk->width;

    if (!tk->name)
        return;

    gui_draw_vline (tb, x);

    /*set_foreground (3); *//* it's already 3 from gui_draw_vline() */
    draw_line (tb, x + 1, 0, x + taskw, 0);

    set_foreground (1);
    draw_line (tb, x + 1, WINHEIGHT - 1, x + taskw, WINHEIGHT - 1);

    if (tk->focused)
    {
        x++;
        /*set_foreground (1);*/       /* mid gray */
        fill_rect (tb, x + 3, 3, taskw - 5, WINHEIGHT - 6);
        set_foreground (3);       /* white */
        draw_line (tb, x + 2, WINHEIGHT - 2, x + taskw - 2, WINHEIGHT - 2);
        draw_line (tb, x + taskw - 2, 2, x + taskw - 2, WINHEIGHT - 2);
        set_foreground (0);
        draw_line (tb, x + 1, 2, x + 1, WINHEIGHT - 2);
        set_foreground (4);       /* darkest gray */
        draw_line (tb, x + 2, 2, x + taskw - 2, 2);
        draw_line (tb, x + 2, 2, x + 2, WINHEIGHT - 3);
    } else
    {
        set_foreground (0);       /* mid gray */
        fill_rect (tb, x + 2, 1, taskw - 1, WINHEIGHT - 2);
    }

    {
        register int text_x = x + TEXTPAD + TEXTPAD + ICONWIDTH;

        /* check how many chars can fit */
        len = strlen (tk->name);
        while (XTextWidth (xfs, tk->name, len) >= taskw - (text_x - x) - 2
                 && len > 0)
            len--;

        if (tk->iconified)
        {
            /* draw task's name dark (iconified) */
            set_foreground (3);
            XDrawString (dd, tb->win, fore_gc, text_x, text_y + 1, tk->name,
                             len);
            set_foreground (4);
        } else
        {
            set_foreground (5);
        }

        /* draw task's name here */
        XDrawString (dd, tb->win, fore_gc, text_x, text_y, tk->name, len);
    }

#ifndef HAVE_XPM
    if (!tk->icon)
        return;
#endif

    /* draw the task's icon */
    XSetClipMask (dd, fore_gc, tk->mask);
    XSetClipOrigin (dd, fore_gc, x + TEXTPAD, (WINHEIGHT - ICONHEIGHT) / 2);
    XCopyArea (dd, tk->icon, tb->win, fore_gc, 0, 0, ICONWIDTH, ICONHEIGHT,
                  x + TEXTPAD, (WINHEIGHT - ICONHEIGHT) / 2);
    XSetClipMask (dd, fore_gc, None);
}

void
gui_draw_clock (taskbar * tb)
{
    char *time_str;
    time_t now;
    int width, old_x, x = WINWIDTH - time_width - (TEXTPAD * 4);

    old_x = x;

    width = WINWIDTH - x - 2;

    now = time (0);
    time_str = ctime (&now) + 11;

    gui_draw_vline (tb, x);
    x += TEXTPAD;

    /*set_foreground (3); *//* white *//* it's already 3 from gui_draw_vline() */
    draw_line (tb, x + 1, WINHEIGHT - 2, old_x + width - TEXTPAD,
                  WINHEIGHT - 2);
    draw_line (tb, old_x + width - TEXTPAD, 2, old_x + width - TEXTPAD,
                  WINHEIGHT - 2);

    set_foreground (1);           /* mid gray */
    fill_rect (tb, x + 1, 2, width - (TEXTPAD * 2) - 1, WINHEIGHT - 4);

    set_foreground (4);           /* darkest gray */
    draw_line (tb, x, 2, x + width - (TEXTPAD * 2) - 1, 2);
    draw_line (tb, x, 2, x, WINHEIGHT - 2);

    set_foreground (5);
    XDrawString (dd, tb->win, fore_gc, x + TEXTPAD - 1, text_y, 
                        time_str, 5);
}

void
gui_draw_taskbar (taskbar * tb)
{
    task *tk;
    int x, width, taskw;
    int under = 0;

    set_foreground (5); /* black */
    draw_up_triangle (tb);
    draw_down_triangle (tb);

    width = WINWIDTH - 80 - time_width - (TEXTPAD * 4);
    x = 80;

    if (tb->num_tasks == 0)
        goto clear;

    taskw = width / tb->num_tasks;
    if (taskw > MAX_TASK_WIDTH)
    {
        taskw = MAX_TASK_WIDTH;
        under = 1;
    }

    tk = tb->task_list;
    while (tk)
    {
        tk->pos_x = x;
        tk->width = taskw - 1;
        gui_draw_task (tb, tk);
        x += taskw;
        tk = tk->next;
    }

    if (under)
    {
clear:
        gui_draw_vline (tb, x);
        set_foreground (0);
        fill_rect (tb, x + 2, 0, WINWIDTH, WINHEIGHT);
    }

    gui_draw_clock (tb);

    gui_draw_vline (tb, 8);
    gui_draw_vline (tb, 74);

    draw_grill (tb->win, 2);
    draw_grill (tb->win, WINWIDTH - 6);
}

void
gui_sync (void)
{
    XSync (dd, False);
}

taskbar *
gui_create_taskbar (void)
{
    taskbar *tb;
    Window win;
    XClassHint wm_class;
    MWMHints mwm;
    XSizeHints size_hints;
    XWMHints wmhints;
    XSetWindowAttributes att;

    att.background_pixel = palette[0];
    att.event_mask = ButtonPressMask | ExposureMask;

    win = XCreateWindow (
                                  /* display */ dd,
                                  /* parent  */ root_win,
                                  /* x       */ 0,
                                  /* y       */ scr_height - WINHEIGHT,
                                  /* width   */ WINWIDTH,
                                  /* height  */ WINHEIGHT,
                                  /* border  */ 0,
                                  /* depth   */ CopyFromParent,
                                  /* class   */ InputOutput,
                                  /* visual  */ CopyFromParent,
                                  /*value mask*/ CWBackPixel | CWEventMask,
                                  /* attribs */ &att);

    /* set name and class */
    wm_class.res_name = "fspanel";

    /* this is the magic sauce that makes this work with Openbox
     * properly: stick and on top
     */
    wm_class.res_class = "DockApp";
    XSetClassHint (dd, win, &wm_class);

    /* don't let any windows cover fspanel */
    if (wm_use_ewmh)
    {
#if 0
        XChangeProperty (dd, win,
                         atom__NET_WM_STATE, XA_ATOM, 32,
                         PropModeReplace,
                         (unsigned char *) &atom__NET_WM_STATE_ABOVE, 1);
#endif

        Atom ret_type = 0;
        int format = 0;
        unsigned long num_items = 0;
        unsigned long bytes_after = 0;
        unsigned char *data;

        int retval =
        XChangeProperty (dd, win,
                         atom__NET_WM_STATE,
                         XA_ATOM,
                         32,
                         PropModeReplace,
                         (unsigned char *) &atom__NET_WM_STATE_STICKY,
                         1);

        printf("%s\tSTATE_STICKY: %d\n",
                __func__,
                retval);

        retval = XGetWindowProperty(dd, win,
                atom__NET_WM_STATE,
                0,
                1,
                False,
                AnyPropertyType,
                &ret_type,
                &format,
                &num_items,
                &bytes_after,
                &data);

        XFree(data);

        XChangeProperty (dd, win,
                         atom__NET_WM_WINDOW_TYPE,
                         XA_ATOM,
                         32,
                         PropModeReplace,
                         (unsigned char *) &atom__NET_WM_WINDOW_TYPE_DOCK,
                         1);
    }
    else
    {
        set_prop (win, atom__WIN_LAYER, 10);    /* WIN_LAYER_ABOVE_DOCK */
        set_prop (win, atom__WIN_STATE,
                  WIN_STATE_STICKY | WIN_STATE_FIXED_POSITION);
    }

    set_prop (win, atom__WIN_HINTS,
                WIN_HINTS_SKIP_FOCUS |
                WIN_HINTS_SKIP_WINLIST |
                WIN_HINTS_SKIP_TASKBAR |
                WIN_HINTS_DO_NOT_COVER); 

    /* borderless motif hint */
    memset (&mwm, 0, sizeof (mwm));
    mwm.flags = MWM_HINTS_DECORATIONS;
    XChangeProperty (dd, win, atom__MOTIF_WM_HINTS,
                            atom__MOTIF_WM_HINTS, 32, PropModeReplace,
                            (unsigned char *) &mwm, sizeof (MWMHints) / 4);

    /* make sure the WM obeys our window position */
    size_hints.flags = PPosition;
    /*XSetWMNormalHints (dd, win, &size_hints);*/
    XChangeProperty (dd, win, XA_WM_NORMAL_HINTS,
                            XA_WM_SIZE_HINTS, 32, PropModeReplace,
                            (unsigned char *) &size_hints, sizeof (XSizeHints) / 4);

    /* make our window unfocusable */
    wmhints.flags = InputHint;
    wmhints.input = 0;
    /*XSetWMHints (dd, win, &wmhints);*/
    XChangeProperty (dd, win, XA_WM_HINTS,
        XA_WM_HINTS, 32, PropModeReplace,
        (unsigned char *) &wmhints, sizeof (XWMHints) / 4);

    strut_init(win);

    XMapWindow (dd, win);

    tb = calloc (1, sizeof (taskbar));
    tb->win = win;

    return tb;
}

void
gui_init (void)
{
    XGCValues gcv;
    XColor xcl;
    int i, j;
    char *fontname;

    i = j = 0;
    do
    {
        xcl.red = cols[i];
        i++;
        xcl.green = cols[i];
        i++;
        xcl.blue = cols[i];
        i++;
        XAllocColor (dd, DefaultColormap (dd, scr_screen), &xcl);
        palette[j] = xcl.pixel;
        j++;
    }
    while (j < PALETTE_COUNT);

    fontname = FONT_NAME;
    do
    {
        xfs = XLoadQueryFont (dd, fontname);
        fontname = "fixed";
    }
    while (!xfs);

    /*time_width = XTextWidth (xfs, "88:88", 5); */
#define time_width (35)
    text_y = xfs->ascent + ((WINHEIGHT - xfs->ascent) / 2);

    gcv.font = xfs->fid;
    gcv.graphics_exposures = False;
    fore_gc = XCreateGC (dd, root_win, GCFont | GCGraphicsExposures, &gcv);

#ifdef HAVE_XPM
    XpmCreatePixmapFromData (dd, root_win, icon_xpm, &generic_icon,
                                     &generic_mask, NULL);
#else
    generic_icon = 0;
#endif
}

void
move_taskbar (taskbar * tb)
{
    int x, y;

    x = y = 0;

    if (tb->hidden)
        x = WINWIDTH - TEXTPAD;

    if (!tb->at_top)
        y = scr_height - WINHEIGHT;

    XMoveWindow (dd, tb->win, x, y);
}


