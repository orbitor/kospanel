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
kosp_ui *kosp_ui_create(int isa, void *parent, int x, int y,
        unsigned int width, unsigned int height)
{
    kosp_ui *kui = kosp_ui_create_default();

    if (NULL != kui)
    {
        kosp_ui_init(kui, isa, x, y, width, height);
    }

    return kui;
}

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
void kosp_ui_init(kosp_ui *self, int isa, int x, int y, 
        unsigned int width, unsigned int height)
{
    kosp_base_init((kosp_base *) self, isa);
    kosp_ui_funcs_init(self);
    kosp_ui_set(self, isa, x, y, width, height);

    self->_child_list = kosp_list_create(false, true);
}

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
void kosp_ui_set(kosp_ui *self, int isa, int x, int y,
        unsigned int width, unsigned int height)
{
    kosp_isa_set(self, isa);
    self->_posnsize.width = width;
    self->_posnsize.height = height;
}

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
void kosp_ui_funcs_init(kosp_ui *self)
{
    self->destroy = kosp_ui_destroy;
    self->init_palette = kosp_ui_init_palette;
    self->draw = kosp_ui_draw;
    self->resize = kosp_ui_resize;
    self->add = kosp_ui_add;
    self->remove = kosp_ui_remove;
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
        return ((kosp_ui *) vself)->_posnsize.width;
    }

    return -1;
}

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
int kosp_ui_height(void *vself)
{
    if (vself)
    {
        return ((kosp_ui *) vself)->_posnsize.height;
    }

    return -1;
}

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
Window kosp_ui_window(void *vself)
{
    if (vself)
    {
        return ((kosp_ui *) vself)->_window;
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
/* virtual functions */
/*-------------------------------------------------------------------------*/
void kosp_ui_destroy(void *vself)
{
    kosp_list *cl = NULL;

    if (NULL == vself)
    {
        return;
    }

    cl = ((kosp_ui *) vself)->_child_list;

    if (NULL != cl)
    {
        cl->destroy(cl);
    }

    kosp_base_destroy(vself);
}

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
void kosp_ui_init_palette(void *vself)
{
}

void kosp_ui_draw(void *vself)
{
}

void kosp_ui_resize(void *vself, XRectangle new_size)
{
}

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
void kosp_ui_add(void *vself, void *child, bool add_front)
{
    if (NULL == vself || NULL == child)
    {
        return;
    }

    kosp_list_add(((kosp_ui *) vself)->_child_list, child, add_front);
}

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
kosp_base *kosp_ui_remove(void *vself, void *child)
{
    kosp_base *retval = NULL;

    if (NULL == vself || NULL == child)
    {
        return NULL;
    }

    retval = kosp_list_remove(((kosp_ui *) vself)->_child_list, child);
    return retval;
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

