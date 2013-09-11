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

typedef struct _kosp_ui_t kosp_ui;

kosp_ui *kosp_ui_create_default(void);
kosp_ui *kosp_ui_create(int isa, int width, int height);
void kosp_ui_init_default(kosp_ui *self);
void kosp_ui_init(int isa, int width, int height);
void kosp_ui_init_funcs(kosp_ui *self);

void kosp_ui_destroy(kosp_ui *self);
void kosp_ui_draw(kosp_ui *self);
void kosp_ui_resize(kosp_ui *self, kosp_rect *size);

int kosp_ui_event_button_press(kosp_ui *self, XButtonPressedEvent *event);
int kosp_ui_event_button_release(kosp_ui *self, XButtonReleasedEvent *event);
int kosp_ui_event_pointer_moved(kosp_ui *self, XPointerMovedEvent *event);
int kosp_ui_event_enter_window(kosp_ui *self, XEnterWindowEvent *event);
int kosp_ui_event_leave_window(kosp_ui *self, XLeaveWindowEvent *event);
int kosp_ui_event_client_message(kosp_ui *self, XClientMessageEvent *event);
int kosp_ui_event_property_notify(kosp_ui *self, XPropertyEvent *event);
int kosp_ui_event_expose(kosp_ui *self, XExposeEvent *event);
int kosp_ui_event_unmap_notify(kosp_ui *self, XUnmapEvent *event);
int kosp_ui_event_destroy_notify(kosp_ui *self, XDestroyWindowEvent *event);

inline int kosp_ui_isa(kosp_ui *self);
inline int kosp_ui_width(kosp_ui *self);
inline int kosp_ui_height(kosp_ui *self);
inline Window kosp_ui_window(kosp_ui *self);

typedef void (*ui_func_destroy) (kosp_ui *self);
typedef void (*ui_func_draw) (kosp_ui *self);
typedef int  (*ui_func_event_button_press) (kosp_ui *self, XButtonPressedEvent *event);
typedef int  (*ui_func_event_button_release) (kosp_ui *self, XButtonReleasedEvent *event);
typedef int  (*ui_func_event_pointer_moved) (kosp_ui *self, XPointerMovedEvent *event);
typedef int  (*ui_func_event_enter_window) (kosp_ui *self, XEnterWindowEvent *event);
typedef int  (*ui_func_event_leave_window) (kosp_ui *self, XLeaveWindowEvent *event);
typedef int  (*ui_func_event_client_message) (kosp_ui *self, XClientMessageEvent *event);
typedef int  (*ui_func_event_property_notify) (kosp_ui *self, XPropertyEvent *event);
typedef int  (*ui_func_event_expose) (kosp_ui *self, XExposeEvent *event);
typedef int  (*ui_func_event_unmap_notify) (kosp_ui *self, XUnmapEvent *event);
typedef int  (*ui_func_event_destroy_notify) (kosp_ui *self, XDestroyWindowEvent *event);

#define KOSP_UI_MEMBERS_DECLARE \
    ui_func_destroy                 destroy; \
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
    int                             isa; \
    Window                          xwin; \
    kosp_rect                       posnsize;

struct _kosp_ui_t
{
    KOSP_UI_MEMBERS_DECLARE
};
    

#endif  /* COM_LUCKYGREENFROG_KOSP_UI_H_ */

