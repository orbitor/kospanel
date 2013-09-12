/*-------------------------------------------------------------------------*/
/* kosp_ui.h
 *
 * This module is part of kospanel
 * Copyright (c) 2013, James DeLisle    <jd@luckygreenfrog.com>
 * See LICENSE for details
 */
/*-------------------------------------------------------------------------*/

#ifndef COM_LUCKYGREENFROG_KOSP_UI_H_
#define COM_LUKCYGREENFROG_KOSP_UI_H_

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include "kosp_base.h"
#include "kosp_rect.h"

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
typedef struct _kosp_ui_t kosp_ui;

kosp_ui *kosp_ui_create_default(void);
kosp_ui *kosp_ui_create(int isa, int width, int height);
void kosp_ui_init_default(kosp_ui *self);
void kosp_ui_init(kosp_ui *self, int isa, int width, int height);
void kosp_ui_init_funcs(kosp_ui *self);

/* class functions */
inline int kosp_ui_width(kosp_ui *self);
inline int kosp_ui_height(kosp_ui *self);
inline Window kosp_ui_window(kosp_ui *self);

/* virtual functions */
typedef void (*ui_func_draw) (void *vself);
typedef int  (*ui_func_event_button_press) (void *vself, XButtonPressedEvent *event);
typedef int  (*ui_func_event_button_release) (void *vself, XButtonReleasedEvent *event);
typedef int  (*ui_func_event_pointer_moved) (void *vself, XPointerMovedEvent *event);
typedef int  (*ui_func_event_enter_window) (void *vself, XEnterWindowEvent *event);
typedef int  (*ui_func_event_leave_window) (void *vself, XLeaveWindowEvent *event);
typedef int  (*ui_func_event_client_message) (void *vself, XClientMessageEvent *event);
typedef int  (*ui_func_event_property_notify) (void *vself, XPropertyEvent *event);
typedef int  (*ui_func_event_expose) (void *vself, XExposeEvent *event);
typedef int  (*ui_func_event_unmap_notify) (void *vself, XUnmapEvent *event);
typedef int  (*ui_func_event_destroy_notify) (void *vself, XDestroyWindowEvent *event);

void kosp_ui_destroy(void *vself);
void kosp_ui_draw(void *vself);
void kosp_ui_resize(void *vself, kosp_rect *size);
int kosp_ui_event_button_press(void *vself, XButtonPressedEvent *event);
int kosp_ui_event_button_release(void *vself, XButtonReleasedEvent *event);
int kosp_ui_event_pointer_moved(void *vself, XPointerMovedEvent *event);
int kosp_ui_event_enter_window(void *vself, XEnterWindowEvent *event);
int kosp_ui_event_leave_window(void *vself, XLeaveWindowEvent *event);
int kosp_ui_event_client_message(void *vself, XClientMessageEvent *event);
int kosp_ui_event_property_notify(void *vself, XPropertyEvent *event);
int kosp_ui_event_expose(void *vself, XExposeEvent *event);
int kosp_ui_event_unmap_notify(void *vself, XUnmapEvent *event);
int kosp_ui_event_destroy_notify(void *vself, XDestroyWindowEvent *event);

#define KOSP_UI_MEMBERS_DECLARE \
    KOSP_BASE_MEMBERS_DECLARE \
    ui_func_draw                    draw; \
    ui_func_event_button_press      button_press; \
    ui_func_event_button_release    button_release; \
    ui_func_event_pointer_moved     pointer_moved; \
    ui_func_event_enter_window      enter_window; \
    ui_func_event_leave_window      leave_window; \
    ui_func_event_client_message    client_message; \
    ui_func_event_property_notify   property_notify; \
    ui_func_event_expose            expose; \
    ui_func_event_unmap_notify      unmap_notify; \
    ui_func_event_destroy_notify    destroy_notify; \
    Window                          xwin; \
    kosp_rect                       posnsize;

struct _kosp_ui_t
{
    KOSP_UI_MEMBERS_DECLARE
};
    

#endif  /* COM_LUCKYGREENFROG_KOSP_UI_H_ */

