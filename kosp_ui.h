/*-------------------------------------------------------------------------*/
/* kosp_ui.h
 *
 * This module is part of kospanel
 * Copyright (c) 2013, James DeLisle    <jd@luckygreenfrog.com>
 * See LICENSE for details
 */
/*-------------------------------------------------------------------------*/

#ifndef COM_LUCKYGREENFROG_KOSP_UI_H_
#define COM_LUCKYGREENFROG_KOSP_UI_H_

#include "kosp_types.h"
#include "kosp_base.h"
#include "kosp_list.h"

#include <X11/Xlib.h>
#include <X11/Xutil.h>

/*-------------------------------------------------------------------------*/
/* color indicies */
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
/* keep track of state so we know how to draw ourselves */
/*-------------------------------------------------------------------------*/
enum
{
    KU_STATE_NONE,
    KU_STATE_NORMAL,
    KU_STATE_SELECTED,
    KU_STATE_DISABLED,
    KU_STATE_HOVER,
    KU_STATE_MAX
};

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
typedef struct _kosp_ui_t kosp_ui_t;

/*-------------------------------------------------------------------------*/
/* create and init functions */
/*-------------------------------------------------------------------------*/
kosp_ui_t *kosp_ui_alloc(void);
kosp_ui_t *kosp_ui_alloc_init(int isa, kosp_ui_t *parent, int x, int y,
        unsigned int width, unsigned int height,
        bool isa_responder);
void kosp_ui_init(kosp_ui_t *self, int isa, kosp_ui_t *parent, int x, int y,
        unsigned int width, unsigned int height,
        bool isa_responder);
void kosp_ui_set(kosp_ui_t *self, int isa, int x, int y,
        unsigned int width, unsigned int height,
        bool isa_responder);
void kosp_ui_funcs_init(kosp_ui_t *self);

/*-------------------------------------------------------------------------*/
/* type functions */
/*-------------------------------------------------------------------------*/
bool kosp_ui_isa_responder(void *vself);
void kosp_ui_isa_responder_set(void *vself, bool isa_responder);
int  kosp_ui_state(void *vself);
void kosp_ui_state_set(void *vself, int state);
bool kosp_ui_is_visible(void *vself);
void kosp_ui_is_visible_set(void *vself, bool is_visible);

int kosp_ui_width(const void *vself);
int kosp_ui_height(const void *vself);
Window kosp_ui_window(const void *vself);

void kosp_ui_init_palette_with_data(void *vself,
        unsigned short *color_array,
        int color_array_len);

void kosp_ui_line_draw(void *vself, XSegment segment, int pal_index);
void kosp_ui_color_set(void *vself, int index, unsigned long color);
unsigned long kosp_ui_color_get(void *vself, int index);
void kosp_ui_color_select_into_bg(void *vself, int index);
void kosp_ui_color_select_into_fg(void *vself, int index);
void kosp_ui_smudge(void *vself);

/*-------------------------------------------------------------------------*/
/* virtual functions */
/*-------------------------------------------------------------------------*/
void kosp_ui_destroy(void *vself);
void kosp_ui_init_palette(void *vself);
void kosp_ui_draw(void *vself);
void kosp_ui_draw_children(void *vself);
void kosp_ui_resize(void *vself, XRectangle new_size);
void kosp_ui_add(void *vself, void *child, bool add_front);
kosp_base_t *kosp_ui_remove(void *vself, void *child);
void kosp_ui_show(void *vself);
void kosp_ui_hide(void *vself);
void kosp_ui_event_button_press(void *vself, XButtonPressedEvent *event);
void kosp_ui_event_button_release(void *vself, XButtonReleasedEvent *event);
void kosp_ui_event_pointer_moved(void *vself, XPointerMovedEvent *event);
void kosp_ui_event_enter_window(void *vself, XEnterWindowEvent *event);
void kosp_ui_event_leave_window(void *vself, XLeaveWindowEvent *event);
void kosp_ui_event_client_message(void *vself, XClientMessageEvent *event);
void kosp_ui_event_property_notify(void *vself, XPropertyEvent *event);
void kosp_ui_event_configure_notify(void *vself, XConfigureEvent *event);
void kosp_ui_event_expose(void *vself, XExposeEvent *event);
void kosp_ui_event_unmap_notify(void *vself, XUnmapEvent *event);
void kosp_ui_event_destroy_notify(void *vself, XDestroyWindowEvent *event);

/*-------------------------------------------------------------------------*/
/* type declaration */
/*-------------------------------------------------------------------------*/
#define KOSP_UI_MEMBERS_DECLARE \
    KOSP_BASE_MEMBERS_DECLARE \
    void (*init_palette) (void *vself); \
    void (*draw) (void *vself); \
    void (*draw_children) (void *vself); \
    void (*resize) (void *vself, XRectangle new_size); \
    void (*add) (void *vself, void *child, bool add_front); \
    kosp_base_t *(*remove) (void *vself, void *child); \
    void (*show) (void *vself); \
    void (*hide) (void *vself); \
    void (*button_press_notify) (void *vself, XButtonPressedEvent *event); \
    void (*button_release_notify) (void *vself, XButtonReleasedEvent *event); \
    void (*pointer_moved_notify) (void *vself, XPointerMovedEvent *event); \
    void (*enter_window_notify) (void *vself, XEnterWindowEvent *event); \
    void (*leave_window_notify) (void *vself, XLeaveWindowEvent *event); \
    void (*client_message_notify) (void *vself, XClientMessageEvent *event); \
    void (*property_notify) (void *vself, XPropertyEvent *event); \
    void (*configure_notify) (void *vself, XConfigureEvent *event); \
    void (*expose_notify) (void *vself, XExposeEvent *event); \
    void (*unmap_notify) (void *vself, XUnmapEvent *event); \
    void (*destroy_notify) (void *vself, XDestroyWindowEvent *event); \
    unsigned long   _palette[KU_CLR_MAX]; \
    kosp_ui_t      *_parent; \
    kosp_list_t    *_child_list; \
    XRectangle      _posnsize; \
    Window          _window; \
    GC              _gc; \
    int             _state; \
    bool            _isa_responder; \
    bool            _is_visible;

struct _kosp_ui_t
{
    KOSP_UI_MEMBERS_DECLARE
};

#endif  /* COM_LUCKYGREENFROG_KOSP_UI_H_ */

