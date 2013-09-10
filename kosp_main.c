void
handle_error (Display * d, XErrorEvent * ev)
{
}

int
main (int argc, char *argv[])
{
    taskbar *tb;
    XEvent ev;
    fd_set fd;
    struct timeval tv;
    int xfd;
    time_t now;
    struct tm *lt;
    void *prop;

    dd = XOpenDisplay (NULL);
    if (!dd)
        return 0;
    scr_screen = DefaultScreen (dd);
    scr_depth = DefaultDepth (dd, scr_screen);
    scr_height = DisplayHeight (dd, scr_screen);
    scr_width = DisplayWidth (dd, scr_screen);
    root_win = RootWindow (dd, scr_screen);

    /* helps us catch windows closing/opening */
    XSelectInput (dd, root_win, PropertyChangeMask);

    XSetErrorHandler ((XErrorHandler) handle_error);

    XInternAtoms (dd, atom_names, ATOM_COUNT, False, atoms);

    /* check if the WM supports EWMH
    Note that this is not reliable. When switching to a EWMH-unaware WM, it
    will not delete this property. Also, we can't react to changes in this
    without a restart. */
    prop = get_prop_data (root_win, atom__NET_SUPPORTED, XA_ATOM, NULL);
    if (prop)
    {
        wm_use_ewmh = 1;
        XFree (prop);
    }

    gui_init ();
    tb = gui_create_taskbar ();
    xfd = ConnectionNumber (dd);
    gui_sync ();

    while (1)
    {
        now = time (0);
        lt = gmtime (&now);
        tv.tv_usec = 0;
        tv.tv_sec = 60 - lt->tm_sec;
        FD_ZERO (&fd);
        FD_SET (xfd, &fd);
        if (select (xfd + 1, &fd, 0, 0, &tv) == 0)
            gui_draw_clock (tb);

        while (XPending (dd))
        {
            XNextEvent (dd, &ev);
            switch (ev.type)
            {
            case ButtonPress:
                if (ev.xbutton.button == 1)
                    handle_press (tb, ev.xbutton.x, ev.xbutton.y);
                break;
            case DestroyNotify:
                del_task (tb, ev.xdestroywindow.window);
                /* fall through */
            case Expose:
                gui_draw_taskbar (tb);
                break;
            case PropertyNotify:
                handle_propertynotify (tb, ev.xproperty.window,
                                              ev.xproperty.atom);
                break;
            case FocusIn:
                handle_focusin (tb, ev.xfocus.window);
                break;
            /*default:
                   printf ("unknown evt type: %d\n", ev.type);*/
            }
        }
    }

    /*XCloseDisplay (dd);

    return 0;*/
}


