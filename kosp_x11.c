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
typedef struct _kosp_x11_t kosp_x11_t;
struct _kosp_x11_t
{
    Display *_display;
    Visual *_visual;
    XineramaScreenInfo *_xinerama_info;
    Window _root_window;
    Colormap _colormap;
    Time _server_time;
    int _screen;
    int _color_depth;
    int _error_code;
    int _request_code;
};

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

#if 0
    {
        int nscreens = ScreenCount(kosp_x11._display);
        int width = DisplayWidth(kosp_x11._display, kosp_x11._screen);
        int height = DisplayHeight(kosp_x11._display, kosp_x11._screen);
        printf("nscreens %d\twidth %d\theight %d\n",
                nscreens, width, height);
    }
#endif

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

            printf("%s\txinerama_info %p\tsize %d\n",
                    __func__,
                    kosp_x11._xinerama_info,
                    sizeof(XineramaScreenInfo) * nscr);

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
    printf("%s\n", __func__);

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
Time kosp_x11_server_time(void)
{
    return kosp_x11._server_time;
}

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
void kosp_x11_set_server_time(Time server_time)
{
    kosp_x11._server_time = server_time;
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
Window kosp_x11_create_child_window(Window parent,
        int x, int y,
        unsigned int width, unsigned int height,
        unsigned long background_color)
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
            x,
            y,
            width,
            height,
            0,
            kosp_x11._color_depth,
            InputOutput,
            kosp_x11._visual,
            create_mask,
            &attrib);

    XSelectInput(kosp_x11._display,
            child,
            EnterWindowMask |
            LeaveWindowMask |
            ButtonReleaseMask |
            ButtonPressMask |
            ButtonMotionMask |
            ExposureMask |
            StructureNotifyMask);

    XSetWindowBackground(kosp_x11._display,
            child,
            background_color);

    return child;
}

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
Window kosp_x11_create_toplevel_window(int x, int y,
        unsigned int width, unsigned int height,
        unsigned long background_color)
{
    XSetWindowAttributes attrib;
    XSizeHints size_hints;

    /*
    unsigned long create_mask = CWColormap | CWOverrideRedirect | CWEventMask;
    */
    unsigned long create_mask = CWColormap | CWEventMask;
    Window window = None;

    attrib.colormap = kosp_x11._colormap;
    attrib.override_redirect = True;
    attrib.event_mask = EnterWindowMask |
        LeaveWindowMask |
        ButtonReleaseMask |
        ButtonPressMask |
        ButtonMotionMask |
        ExposureMask;

    window = XCreateWindow(kosp_x11._display,
            kosp_x11._root_window,
            x,
            y,
            width,
            height,
            0,
            kosp_x11._color_depth,
            InputOutput,
            kosp_x11._visual,
            create_mask,
            &attrib);

    XSelectInput(kosp_x11._display,
            window,
            EnterWindowMask |
            LeaveWindowMask |
            ButtonReleaseMask |
            ButtonPressMask |
            ButtonMotionMask |
            ExposureMask |
            StructureNotifyMask);

    XSetWindowBackground(kosp_x11._display,
            window,
            background_color);

    size_hints.flags = PSize | PMinSize;
    size_hints.width = width;
    size_hints.height = height;
    size_hints.min_width = width;
    size_hints.min_height = height;

    XSetWMProperties(kosp_x11._display,
            window,
            NULL,
            NULL,
            NULL,
            0,
            &size_hints,
            NULL,
            NULL);

    return window;
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
void kosp_x11_map_window(Window window)
{
    if (None != window)
    {
        printf("%s\tmapping %d\n",
                __func__,
                (int) window);

        XMapWindow(kosp_x11._display, window);
    }
}

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
void kosp_x11_unmap_window(Window window)
{
    if (None != window)
    {
        XUnmapWindow(kosp_x11._display, window);
    }
}

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
GC kosp_x11_create_default_gc(Window window)
{
    GC gc = XCreateGC(kosp_x11._display,
            window,
            0,
            NULL);

    return gc;
}

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
static void kosp_x11_error_handler(Display *display, XErrorEvent *xev)
{
    kosp_x11._error_code = xev->error_code;
    kosp_x11._request_code = xev->request_code;
}
