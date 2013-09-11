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
#include "kosp_rect.h"
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
        kosp_ui_init(isa, width, height);
    }
}

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
void kosp_ui_init_default(kosp_ui *self)
{
}

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
void kosp_ui_init(int isa, int width, int height)
{
}

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
void kosp_ui_destroy(kosp_ui *self)
{
    if (NULL != self)
    {
        free(self);
        self = NULL;
    }
}

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
void kosp_ui_init_funcs(kosp_ui *self)
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

void kosp_ui_draw(kosp_ui *self)
{
}

void kosp_ui_resize(kosp_ui *self, kosp_rect *size)
{
}

int kosp_ui_event_button_press(kosp_ui *self, XButtonPressedEvent *event)
{
    return 1;
}

int kosp_ui_event_button_release(kosp_ui *self, XButtonReleasedEvent *event)
{
    return 1;
}

int kosp_ui_event_pointer_moved(kosp_ui *self, XPointerMovedEvent *event)
{
    return 1;
}

int kosp_ui_event_enter_window(kosp_ui *self, XEnterWindowEvent *event)
{
    return 1;
}

int kosp_ui_event_leave_window(kosp_ui *self, XLeaveWindowEvent *event)
{
    return 1;
}

int kosp_ui_event_client_message(kosp_ui *self, XClientMessageEvent *event)
{
    return 1;
}

int kosp_ui_event_property_notify(kosp_ui *self, XPropertyEvent *event)
{
    return 1;
}

int kosp_ui_event_expose(kosp_ui *self, XExposeEvent *event)
{
    return 1;
}

int kosp_ui_event_unmap_notify(kosp_ui *self, XUnmapEvent *event)
{
    return 1;
}

int kosp_ui_event_destroy_notify(kosp_ui *self, XDestroyWindowEvent *event)
{
    return 1;
}

