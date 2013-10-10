/*-------------------------------------------------------------------------*/
/* kosp_ui.c
 *
 * This module is part of kospanel
 * Copyright (c) 2013, James DeLisle    <jd@luckygreenfrog.com>
 * See LICENSE for details
 */
/*-------------------------------------------------------------------------*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <X11/Xutil.h>
#include "kosp_x11.h"

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
static kosp_x11_t kosp_x11;

static void kosp_x11_error_handler(Display *display, XErrorEvent *xev);

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
bool kosp_x11_init(void)
{
    memset(&kosp_x11, 0, sizeof(kosp_x11));

    if ((kosp_x11._display = XOpenDisplay(NULL)) == NULL)
    {
        return false;
    }

    XSetErrorHandler((XErrorHandler) kosp_x11_error_handler);

    kosp_x11._root_window = DefaultRootWindow(kosp_x11._display);
    kosp_x11._screen = DefaultScreen(kosp_x11._display);
    kosp_x11._color_depth = DefaultDepth(kosp_x11._display,
            kosp_x11._screen);
    kosp_x11._visual = DefaultVisual(kosp_x11._display,
            kosp_x11._screen);
    kosp_x11._colormap = DefaultColormap(kosp_x11._display,
            kosp_x11._screen);

    {
        int nscreens = ScreenCount(kosp_x11._display);
        int width = DisplayWidth(kosp_x11._display, kosp_x11._screen);
        int height = DisplayHeight(kosp_x11._display, kosp_x11._screen);
        printf("nscreens %d\twidth %d\theight %d\n",
                nscreens, width, height);
    }

    /* make sure we have a reasonable color depth */
    if (kosp_x11._color_depth < 8)
    {
        /* Try to get a 'better' visual */
        XVisualInfo vinfo;
        XVisualInfo *vinfo_retval;
        int nitems;
        int best_depth_index = -1;

        vinfo.screen = kosp_x11._screen;
        vinfo.class = TrueColor;

        vinfo_retval = XGetVisualInfo(kosp_x11._display,
                VisualScreenMask | VisualClassMask,
                &vinfo,
                &nitems);

        if (NULL != vinfo_retval)
        {
            int found_max_depth = 1;
            int i;

            for (i = 0; i < nitems; i++)
            {
                if (vinfo_retval[i].depth < found_max_depth)
                {
                    continue;
                }

                found_max_depth = vinfo_retval[i].depth;
                best_depth_index = i;
            }

            if (found_max_depth < kosp_x11._color_depth)
            {
                best_depth_index = -1;
            }
        }

        if (-1 != best_depth_index)
        {
            kosp_x11._color_depth = vinfo_retval[best_depth_index].depth;
            kosp_x11._visual = vinfo_retval[best_depth_index].visual;
            kosp_x11._colormap = XCreateColormap(kosp_x11._display,
                    kosp_x11._root_window,
                    kosp_x11._visual,
                    AllocNone);
        }

        if (NULL != vinfo_retval)
        {
            XFree(vinfo_retval);
        }
    }

    /* check for Xinerama support */
    if (XineramaIsActive(kosp_x11._display))
    {
        int nscr;
        XineramaScreenInfo *xsi_ret = XineramaQueryScreens(kosp_x11._display,
                &nscr);

        if (NULL != xsi_ret)
        {
            kosp_x11._xinerama_info = (XineramaScreenInfo *)
                malloc(sizeof(XineramaScreenInfo) * nscr);

            if (NULL != kosp_x11._xinerama_info)
            {
                memcpy(kosp_x11._xinerama_info,
                        xsi_ret,
                        sizeof(XineramaScreenInfo) * nscr);
            }

            XFree(xsi_ret);
        }
    }

    return true;
}

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
void kosp_x11_shutdown(void)
{
    XCloseDisplay(kosp_x11._display);

    if (NULL != kosp_x11._xinerama_info)
    {
        free(kosp_x11._xinerama_info);
    }

    memset(&kosp_x11, 0, sizeof(kosp_x11));
}

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
Display *kosp_x11_display(void)
{
    return kosp_x11._display;
}

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
Window kosp_x11_root_window(void)
{
    return kosp_x11._root_window;
}

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
int kosp_x11_color_depth(void)
{
    return kosp_x11._color_depth;
}

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
Colormap kosp_x11_colormap(void)
{
    return kosp_x11._colormap;
}

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
int kosp_x11_screen(void)
{
    return kosp_x11._screen;
}

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
Visual *kosp_x11_visual(void)
{
    return kosp_x11._visual;
}

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
void kosp_x11_get_last_x_error(int *error_code, int *request_code)
{
    *error_code = kosp_x11._error_code;
    *request_code = kosp_x11._request_code;
}

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
Window kosp_x11_create_child_window(Window parent)
{
    XSetWindowAttributes attrib;
    unsigned long create_mask = CWEventMask;
    Window child = None;

    attrib.event_mask = ButtonPressMask |
        ButtonReleaseMask |
        ButtonMotionMask |
        EnterWindowMask |
        LeaveWindowMask |
        ExposureMask;

    child = XCreateWindow(kosp_x11._display,
            parent,
            0,
            0,
            1,
            1,
            0,
            kosp_x11._color_depth,
            InputOutput,
            kosp_x11._visual,
            create_mask,
            &attrib);

    return child;
}

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
Window kosp_x11_create_toplevel_window(void)
{
    XSetWindowAttributes attrib;
    unsigned long create_mask = CWColormap | CWOverrideRedirect | CWEventMask;
    Window child = None;

    attrib.colormap = kosp_x11._colormap;
    attrib.override_redirect = True;
    attrib.event_mask = EnterWindowMask | LeaveWindowMask;

    child = XCreateWindow(kosp_x11._display,
            kosp_x11._root_window,
            0,
            0,
            1,
            1,
            0,
            kosp_x11._color_depth,
            InputOutput,
            kosp_x11._visual,
            create_mask,
            &attrib);

    return child;
}

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
void kosp_x11_destroy_window(Window window)
{
    if (None != window)
    {
        XDestroyWindow(kosp_x11._display, window);
    }
}

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
static void kosp_x11_error_handler(Display *display, XErrorEvent *xev)
{
    kosp_x11._error_code = xev->error_code;
    kosp_x11._request_code = xev->request_code;
}
