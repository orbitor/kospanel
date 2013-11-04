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
#include <X11/extensions/Xinerama.h>
#include "kosp_types.h"

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
bool kosp_x11_init(void);
void kosp_x11_shutdown(void);

Display *kosp_x11_display(void);
Window kosp_x11_root_window(void);
int kosp_x11_color_depth(void);
Colormap kosp_x11_colormap(void);
int kosp_x11_screen(void);
Visual *kosp_x11_visual(void);
Time kosp_x11_server_time(void);
void kosp_x11_set_server_time(Time server_time);
void kosp_x11_get_last_x_error(int *error_code, int *request_code);

Window kosp_x11_create_child_window(Window parent,
        int x, int y, unsigned int width, unsigned int height,
        unsigned long background_color);
Window kosp_x11_create_toplevel_window(int x, int y,
        unsigned int width, unsigned int height,
        unsigned long background_color);
void kosp_x11_destroy_window(Window window);

void kosp_x11_map_window(Window window);
void kosp_x11_unmap_window(Window window);

#endif  /* COM_LUCKYGREENFROG_KOSP_X11_H_ */

