
/********************************************************
 ** F***ing Small Panel 0.7 Copyright (c) 2000-2001 By **
 ** Peter Zelezny <zed@linuxpower.org>                 **
 ** See file COPYING for license details.              **
 ********************************************************/

#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <X11/extensions/Xinerama.h>

#ifdef HAVE_XPM
#include <X11/xpm.h>
#include "icon.xpm"
#endif

#include <stdio.h>
#include "kospanel.h"

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
void strut_init(Window WindowId);

/*-------------------------------------------------------------------------*/
/* you can edit these */
#define MAX_TASK_WIDTH 140
#define ICONWIDTH 16
#define ICONHEIGHT 16
#define WINHEIGHT 24
#define WINWIDTH (scr_width)
#define FONT_NAME "-*-lucida*-m*-r-*-*-12-*-*"

/* don't edit these */
#define TEXTPAD 6
#define left_arrow_x 25
#define right_arrow_x 50

Display *dd;
Window root_win;
/* WM supports EWMH
This flag is set if the window manager supports the EWMH protocol for e.g.
switching workspaces. The fallback if this is not supported is to use the
Gnome variant. This is determined by looking for the presence of the
_NET_SUPPORTED property of the root window. Note that this is only used
for communication with the WM, whether each client supports this protocol
is up to the individual client. */
int wm_use_ewmh;
Pixmap generic_icon;
Pixmap generic_mask;
GC fore_gc;
XFontStruct *xfs;
int scr_screen;
int scr_depth;
int scr_width;
int scr_height;
int text_y;
/*int time_width;*/

unsigned short cols[] = {
    0xd75c, 0xd75c, 0xd75c,       /* 0. light gray */
    0xbefb, 0xbaea, 0xbefb,       /* 1. mid gray */
    0xaefb, 0xaaea, 0xaefb,       /* 2. dark gray */
    0xefbe, 0xefbe, 0xefbe,       /* 3. white */
    0x8617, 0x8207, 0x8617,       /* 4. darkest gray */
    0x0000, 0x0000, 0x0000        /* 5. black */
};

#define PALETTE_COUNT (sizeof (cols) / sizeof (cols[0]) / 3)

unsigned long palette[PALETTE_COUNT];

void
add_task (taskbar * tb, Window win, int focus)
{
    task *tk, *list;

    /* is this window on a different desktop or hidden? */
    if (!is_visible_on_desktop (win, tb->my_desktop) || is_hidden (win))
        return;

    tk = calloc (1, sizeof (task));
    tk->win = win;
    tk->focused = focus;
    tk->name = get_window_name (win);
    tk->iconified = is_iconified (win);

    get_task_kdeicon (tk);
    if (tk->icon == None)
        get_task_hinticon (tk);

    XSelectInput (dd, win, PropertyChangeMask | FocusChangeMask |
                      StructureNotifyMask);

    /* now append it to our linked list */
    tb->num_tasks++;

    list = tb->task_list;
    if (!list)
    {
        tb->task_list = tk;
        return;
    }
    while (1)
    {
        if (!list->next)
        {
            list->next = tk;
            return;
        }
        list = list->next;
    }
}

task *
find_task (taskbar * tb, Window win)
{
    task *list = tb->task_list;
    while (list)
    {
        if (list->win == win)
            return list;
        list = list->next;
    }
    return 0;
}

void
del_task (taskbar * tb, Window win)
{
    task *next, *prev = 0, *list = tb->task_list;

    while (list)
    {
        next = list->next;
        if (list->win == win)
        {
            /* unlink and free this task */
            tb->num_tasks--;
            if (list->icon_copied)
            {
                XFreePixmap (dd, list->icon);
                if (list->mask != None)
                    XFreePixmap (dd, list->mask);
            }
            if (list->name)
                XFree (list->name);
            free (list);
            if (prev == 0)
                tb->task_list = next;
            else
                prev->next = next;
            return;
        }
        prev = list;
        list = next;
    }
}

void
taskbar_read_clientlist (taskbar * tb)
{
    Window *win, focus_win;
    int num, i, rev, desk, new_desk = 0;
    task *list, *next;

    desk = get_current_desktop ();
    if (desk != tb->my_desktop)
    {
        new_desk = 1;
        tb->my_desktop = desk;
    }

    XGetInputFocus (dd, &focus_win, &rev);

    /* try unified window spec first */
    win = get_prop_data (root_win, atom__NET_CLIENT_LIST, XA_WINDOW, &num);
    if (!win)
    {
        /* failed, let's try gnome */
        win = get_prop_data (root_win, atom__WIN_CLIENT_LIST, XA_CARDINAL, &num);
        if (!win)
            return;
    }

    /* remove windows that aren't in the _WIN_CLIENT_LIST anymore */
    list = tb->task_list;
    while (list)
    {
        list->focused = (focus_win == list->win);
        next = list->next;

        if (!new_desk)
            for (i = num - 1; i >= 0; i--)
                if (list->win == win[i])
                    goto dontdel;
        del_task (tb, list->win);
dontdel:

        list = next;
    }

    /* add any new windows */
    for (i = 0; i < num; i++)
    {
        if (!find_task (tb, win[i]))
            add_task (tb, win[i], (win[i] == focus_win));
    }

    XFree (win);
}

