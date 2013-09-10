void
handle_press (taskbar * tb, int x, int y)
{
    task *tk;

    if (y > 3 && y < WINHEIGHT - 3)
    {
        /* the right arrow is pointing down, so shouldn't
         * we be going 'down' in desktop order?
         */
        if (x >= right_arrow_x && x < right_arrow_x + 9)
        {
            switch_desk (tb, -1);
            return;
        }

        if (x >= left_arrow_x && x < left_arrow_x + 9)
        {
            switch_desk (tb, +1);
            return;
        }
    }

    /* clicked left grill */
    if (x < 6)
    {
        if (tb->hidden)
            tb->hidden = 0;
        else
            tb->at_top = !tb->at_top;
        move_taskbar (tb);
        return;
    }

    /* clicked right grill */
    if (x + TEXTPAD > WINWIDTH)
    {
        tb->hidden = !tb->hidden;
        move_taskbar (tb);
        return;
    }

    tk = tb->task_list;
    while (tk)
    {
        if (x > tk->pos_x && x < tk->pos_x + tk->width)
        {
            if (tk->iconified)
            {
                tk->iconified = 0;
                tk->focused = 1;
                XMapWindow (dd, tk->win);
            } else
            {
                if (tk->focused)
                {
                    tk->iconified = 1;
                    tk->focused = 0;
                    XIconifyWindow (dd, tk->win, scr_screen);
                } else
                {
                    tk->focused = 1;
                    XRaiseWindow (dd, tk->win);
                    XSetInputFocus (dd, tk->win, RevertToNone, CurrentTime);
                }
            }
            gui_sync ();
            gui_draw_task (tb, tk);
        } else
        {
            if (tk->focused)
            {
                tk->focused = 0;
                gui_draw_task (tb, tk);
            }
        }

        tk = tk->next;
    }
}

void
handle_focusin (taskbar * tb, Window win)
{
    task *tk;

    tk = tb->task_list;
    while (tk)
    {
        if (tk->focused)
        {
            if (tk->win != win)
            {
                tk->focused = 0;
                gui_draw_task (tb, tk);
            }
        } else
        {
            if (tk->win == win)
            {
                tk->focused = 1;
                gui_draw_task (tb, tk);
            }
        }
        tk = tk->next;
    }
}

void
handle_propertynotify (taskbar * tb, Window win, Atom at)
{
    task *tk;

    if (win == root_win)
    {
        if (wm_use_ewmh)
        {
            if (at == atom__NET_CLIENT_LIST ||
                at == atom__NET_CURRENT_DESKTOP)
            {
                taskbar_read_clientlist (tb);
                gui_draw_taskbar (tb);
            }
        }
        else
        {
            if (at == atom__WIN_CLIENT_LIST ||
                at == atom__WIN_WORKSPACE)
            {
                taskbar_read_clientlist (tb);
                gui_draw_taskbar (tb);
            }
        }
        return;
    }

    tk = find_task (tb, win);
    if (!tk)
        return;

    if (at == XA_WM_NAME || at == atom__NET_WM_NAME)
    {
        /* window's title changed */
        if (tk->name)
            XFree (tk->name);
        tk->name = get_window_name (tk->win);
        gui_draw_task (tb, tk);
    } else if (at == atom_WM_STATE)
    {
        /* iconified state changed? */
        if (is_iconified (tk->win) != tk->iconified)
        {
            tk->iconified = !tk->iconified;
            gui_draw_task (tb, tk);
        }
    } else if (at == XA_WM_HINTS)
    {
        /* some windows set their WM_HINTS icon after mapping */
        if (tk->icon == generic_icon)
        {
            get_task_hinticon (tk);
            gui_draw_task (tb, tk);
        }
    }
}


