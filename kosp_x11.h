/*-------------------------------------------------------------------------*/
/* kosp_ui.h
 *
 * This module is part of kospanel
 * Copyright (c) 2013, James DeLisle    <jd@luckygreenfrog.com>
 * See LICENSE for details
 */
/*-------------------------------------------------------------------------*/

#ifndef COM_LUCKYGREENFROG_KOSP_X11_H_
#define COM_LUCKYGREENFROG_KOSP_X11_H_

#include <X11/Xlib.h>
#include "kosp_types.h"

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
typedef struct _kosp_x11_t kosp_x11_t;
struct _kosp_x11_t
{
    Display *_display;
    Visual *_visual;
    Window _root_window;
    int _screen;
    int _color_depth;
    int _error_code;
    int _request_code;
};

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
bool kosp_x11_init(void);
void kosp_x11_shutdown(void);

Display *kosp_x11_display(void);
Window kosp_x11_root_window(void);
int kosp_x11_color_depth(void);
int kosp_x11_screen(void);
Visual *kosp_x11_visual(void);
void kosp_x11_get_last_x_error(int *error_code, int *request_code);

Window kosp_x11_create_child_window(Window parent);
Window kosp_x11_create_toplevel_window(void);
void kosp_x11_destroy_window(Window window);

#endif  /* COM_LUCKYGREENFROG_KOSP_X11_H_ */

