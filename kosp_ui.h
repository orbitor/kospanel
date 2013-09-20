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
#include "kosp_types.h"
#include "kosp_base.h"
#include "kosp_list.h"

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
enum
{
    KU_CLR_FG_NORMAL,
    KU_CLR_BG_NORMAL,
    KU_CLR_FG_SELECTED,
    KU_CLR_BG_SELECTED,
    KU_CLR_FG_DISABLED,
    KU_CLR_BG_DISABLED,
    KU_CLR_FG_HOVER,
    KU_CLR_BG_HOVER,
    KU_CLR_MAX
};

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
typedef struct _kosp_ui_t kosp_ui;

/*-------------------------------------------------------------------------*/
/* create and init functions */
/*-------------------------------------------------------------------------*/
kosp_ui *kosp_ui_create_default(void);
kosp_ui *kosp_ui_create(int isa, void *parent, int x, int y,
        unsigned int width, unsigned int height);
void kosp_ui_init(kosp_ui *self, int isa, int x, int y,
        unsigned int width, unsigned int height);
void kosp_ui_set(kosp_ui *self, int isa, int x, int y,
        unsigned int width, unsigned int height);
void kosp_ui_funcs_init(kosp_ui *self);

/*-------------------------------------------------------------------------*/
/* type functions */
/*-------------------------------------------------------------------------*/
int kosp_ui_width(void *vself);
int kosp_ui_height(void *vself);
Window kosp_ui_window(void *vself);

void kosp_ui_init_palette_with_data(void *vself,
        unsigned short *color_array,
        int color_array_len);

void kosp_ui_line_draw(void *vself, XSegment segment, int pal_index);

/*-------------------------------------------------------------------------*/
/* virtual functions */
/*-------------------------------------------------------------------------*/
typedef void (*ui_func_init_palette) (void *vself);
typedef void (*ui_func_draw) (void *vself);
typedef void (*ui_func_draw_children) (void *vself);
typedef void (*ui_func_resize) (void *vself, XRectangle new_size);
typedef void (*ui_func_add) (void *vself, void *child, bool add_front);
typedef kosp_base *(*ui_func_remove) (void *vself, void *child);
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
void kosp_ui_init_palette(void *vself);
void kosp_ui_draw(void *vself);
void kosp_ui_draw_children(void *vself);
void kosp_ui_resize(void *vself, XRectangle new_size);
void kosp_ui_add(void *vself, void *child, bool add_front);
kosp_base *kosp_ui_remove(void *vself, void *child);
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

/*-------------------------------------------------------------------------*/
/* type declaration */
/*-------------------------------------------------------------------------*/
#define KOSP_UI_MEMBERS_DECLARE \
    KOSP_BASE_MEMBERS_DECLARE \
    ui_func_init_palette            init_palette; \
    ui_func_draw                    draw; \
    ui_func_draw_children           draw_children; \
    ui_func_resize                  resize; \
    ui_func_add                     add; \
    ui_func_remove                  remove; \
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
    void                           *_parent; \
    kosp_list                      *_child_list; \
    Window                          _window; \
    GC                              _gc; \
    XRectangle                      _posnsize; \
    unsigned long                   _palette[KU_CLR_MAX];

struct _kosp_ui_t
{
    KOSP_UI_MEMBERS_DECLARE
};

#endif  /* COM_LUCKYGREENFROG_KOSP_UI_H_ */

