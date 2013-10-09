/*-------------------------------------------------------------------------*/
/* kosp_ui.c
 *
 * This module is part of kospanel
 * Copyright (c) 2013, James DeLisle    <jd@luckygreenfrog.com>
 * See LICENSE for details
 */
/*-------------------------------------------------------------------------*/

#include <string.h>
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

    return true;
}

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
void kosp_x11_shutdown(void)
{
    XCloseDisplay(kosp_x11._display);
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
    unsigned long create_mask = CWOverrideRedirect | CWEventMask;
    Window child = None;

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
