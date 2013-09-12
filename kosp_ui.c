/*-------------------------------------------------------------------------*/
/* kosp_ui.h
 *
 * This module is part of kospanel
 * Copyright (c) 2013, James DeLisle    <jd@luckygreenfrog.com>
 * See LICENSE for details
 */
/*-------------------------------------------------------------------------*/

#include <stdlib.h>
#include <string.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include "kosp_ui.h"

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
kosp_ui *kosp_ui_create_default(void)
{
    kosp_ui *kui = (kosp_ui *) malloc(sizeof(kosp_ui));

    if (NULL != kui)
    {
        memset(kui, 0, sizeof(kosp_ui));
    }

    return kui;
}

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
kosp_ui *kosp_ui_create(int isa, int width, int height)
{
    kosp_ui *kui = kosp_ui_create_default();

    if (kui)
    {
        kosp_ui_init(kui, isa, width, height);
    }

    return kui;
}

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
void kosp_ui_init_default(kosp_ui *self)
{
    if (NULL == self)
    {
        return;
    }

    kosp_base_init_default((kosp_base *) self);
    kosp_ui_funcs_init(self);
}

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
void kosp_ui_init(kosp_ui *self, int isa, int width, int height)
{
    if (NULL == self)
    {
        return;
    }

    kosp_base_init((kosp_base *) self, isa);
    kosp_ui_funcs_init(self);
}

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
void kosp_ui_funcs_init(kosp_ui *self)
{
    self->destroy = kosp_ui_destroy;
    self->draw = kosp_ui_draw;
    self->button_press = kosp_ui_event_button_press;
    self->button_release = kosp_ui_event_button_release;
    self->pointer_moved = kosp_ui_event_pointer_moved;
    self->enter_window = kosp_ui_event_enter_window;
    self->leave_window = kosp_ui_event_leave_window;
    self->client_message = kosp_ui_event_client_message;
    self->property_notify = kosp_ui_event_property_notify;
    self->expose = kosp_ui_event_expose;
    self->unmap_notify = kosp_ui_event_unmap_notify;
    self->destroy_notify = kosp_ui_event_destroy_notify;
}

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
int kosp_ui_width(void *vself)
{
    if (vself)
    {
        return ((kosp_ui *) vself)->posnsize.width;
    }

    return -1;
}

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
int kosp_ui_height(void *vself)
{
    if (vself)
    {
        return ((kosp_ui *) vself)->posnsize.height;
    }

    return -1;
}

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
Window kosp_ui_window(void *vself)
{
    if (vself)
    {
        return ((kosp_ui *) vself)->window;
    }

    return None;
}

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
void kosp_ui_init_palette_with_data(void *vself,
        unsigned short *color_array,
        int color_array_len)
{
}

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
void kosp_ui_line_draw(void *vself, XSegment segment, int pal_index)
{
}

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
void kosp_ui_destroy(void *vself)
{
    kosp_base_destroy(vself);
}

void kosp_ui_init_palette(void *vself)
{
}

void kosp_ui_draw(void *vself)
{
}

void kosp_ui_resize(void *vself, XRectangle new_size)
{
}

int kosp_ui_event_button_press(void *vself, XButtonPressedEvent *event)
{
    return 1;
}

int kosp_ui_event_button_release(void *vself, XButtonReleasedEvent *event)
{
    return 1;
}

int kosp_ui_event_pointer_moved(void *vself, XPointerMovedEvent *event)
{
    return 1;
}

int kosp_ui_event_enter_window(void *vself, XEnterWindowEvent *event)
{
    return 1;
}

int kosp_ui_event_leave_window(void *vself, XLeaveWindowEvent *event)
{
    return 1;
}

int kosp_ui_event_client_message(void *vself, XClientMessageEvent *event)
{
    return 1;
}

int kosp_ui_event_property_notify(void *vself, XPropertyEvent *event)
{
    return 1;
}

int kosp_ui_event_expose(void *vself, XExposeEvent *event)
{
    return 1;
}

int kosp_ui_event_unmap_notify(void *vself, XUnmapEvent *event)
{
    return 1;
}

int kosp_ui_event_destroy_notify(void *vself, XDestroyWindowEvent *event)
{
    return 1;
}

