void
set_foreground (int index)
{
    XSetForeground (dd, fore_gc, palette[index]);
}

void
draw_line (taskbar *tb, int x, int y, int a, int b)
{
    XDrawLine (dd, tb->win, fore_gc, x, y, a, b);
}

void
fill_rect (taskbar *tb, int x, int y, int a, int b)
{
    XFillRectangle (dd, tb->win, fore_gc, x, y, a, b);
}

void
scale_icon (task *tk)
{
    int xx, yy, x, y;
    unsigned int w, h, d, bw;
    Pixmap pix, mk = None;
    XGCValues gcv;
    GC mgc = None;

    Window unused;
    XGetGeometry (dd, tk->icon, &unused, &x, &y, &w, &h, &bw, &d);
    pix = XCreatePixmap (dd, tk->win, ICONWIDTH, ICONHEIGHT, scr_depth);

    if (tk->mask != None)
    {
        mk = XCreatePixmap (dd, tk->win, ICONWIDTH, ICONHEIGHT, 1);
        gcv.subwindow_mode = IncludeInferiors;
        gcv.graphics_exposures = False;
        mgc = XCreateGC (dd, mk, GCGraphicsExposures | GCSubwindowMode, &gcv);
    }

    set_foreground (3);

    /* this is my simple & dirty scaling routine */
    for (y = ICONHEIGHT - 1; y >= 0; y--)
    {
        yy = (y * h) / ICONHEIGHT;
        for (x = ICONWIDTH - 1; x >= 0; x--)
        {
            xx = (x * w) / ICONWIDTH;
            if (d != scr_depth)
                XCopyPlane (dd, tk->icon, pix, fore_gc, xx, yy, 1, 1, x, y, 1);
            else
                XCopyArea (dd, tk->icon, pix, fore_gc, xx, yy, 1, 1, x, y);
            if (mk != None)
                XCopyArea (dd, tk->mask, mk, mgc, xx, yy, 1, 1, x, y);
        }
    }

    if (mk != None)
    {
        XFreeGC (dd, mgc);
        tk->mask = mk;
    }

    tk->icon = pix;
}

void
draw_dot (Window win, int x, int y)
{
    set_foreground (4);
    XDrawPoint (dd, win, fore_gc, x, y);
    set_foreground (3);
    XDrawPoint (dd, win, fore_gc, x + 1, y + 1);
}

void
draw_grill (Window win, int x)
{
    int y = 0;
    while (y < WINHEIGHT - 4)
    {
        y += 3;
        draw_dot (win, x + 3, y);
        draw_dot (win, x, y);
    }
}

void
draw_up_triangle (taskbar *tb)
{
    fill_rect (tb, left_arrow_x + 2, (WINHEIGHT - 4) / 2 + 1, 3, 3);
    draw_line (tb, left_arrow_x, (WINHEIGHT - 4) / 2 + 3, left_arrow_x + 3, (WINHEIGHT - 4) / 2);
    draw_line (tb, left_arrow_x + 3, (WINHEIGHT - 4) / 2, left_arrow_x + 6, (WINHEIGHT - 4) / 2 + 3);
    draw_line (tb, left_arrow_x + 1, (WINHEIGHT - 4) / 2 + 3, left_arrow_x + 5, (WINHEIGHT - 4) / 2 + 3);
}

void
draw_down_triangle (taskbar *tb)
{
    draw_line (tb, right_arrow_x, (WINHEIGHT - 4) / 2, right_arrow_x + 3, (WINHEIGHT - 4) / 2 + 3);
    draw_line (tb, right_arrow_x, (WINHEIGHT - 4) / 2, right_arrow_x + 4, (WINHEIGHT - 4) / 2 + 2);
    draw_line (tb, right_arrow_x, (WINHEIGHT - 4) / 2, right_arrow_x + 5, (WINHEIGHT - 4) / 2 + 1);
    draw_line (tb, right_arrow_x, (WINHEIGHT - 4) / 2, right_arrow_x + 6, (WINHEIGHT - 4) / 2);
}


