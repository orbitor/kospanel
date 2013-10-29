/*-------------------------------------------------------------------------*/
/* kosp_ui.h
 *
 * This module is part of kospanel
 * Copyright (c) 2013, James DeLisle    <jd@luckygreenfrog.com>
 * See LICENSE for details
 */
/*-------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "kosp_x11.h"
#include "kosp_ui.h"
#include "kosp_app.h"

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
static unsigned short _kosp_ui_default_palette[] =
{
    0x0000, 0x0000, 0x0000,     /* KU_CLR_FG_NORMAL */
    0xbefb, 0xbefb, 0xbefb,     /* KU_CLR_BG_NORMAL */
    0xffff, 0xffff, 0xffff,     /* KU_CLR_FG_SELECTED */
    0x8617, 0x8617, 0x8617,     /* KU_CLR_BG_SELECTED */
    0xd75c, 0xd75c, 0xd75c,     /* KU_CLR_FG_DISABLED */
    0x8617, 0x8617, 0x8617,     /* KU_CLR_BG_DISALBED */
    0xffff, 0xffff, 0xffff,     /* KU_CLR_FG_HOVER */
    0x0000, 0x9999, 0xcccc,     /* KU_CLR_BG_HOVER */
};

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
kosp_ui_t *kosp_ui_create_default(void)
{
    kosp_ui_t *kui = (kosp_ui_t *) malloc(sizeof(kosp_ui_t));

    if (NULL != kui)
    {
        memset(kui, 0, sizeof(kosp_ui_t));
    }

    return kui;
}

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
kosp_ui_t *kosp_ui_create(int isa, void *parent, int x, int y,
        unsigned int width, unsigned int height,
        bool isa_responder)
{
    Window xparent = None;
    kosp_ui_t *kui = kosp_ui_create_default();

    if (NULL != kui)
    {
        kosp_ui_init(kui, isa, x, y, width, height, isa_responder);

        if (NULL != parent)
        {
            xparent = ((kosp_ui_t *) parent)->_window;
            kosp_ui_add(parent, kui, false);
        }

        if (None == xparent)
        {
            kui->_window = kosp_x11_create_toplevel_window();
        }
        else
        {
            kui->_window = kosp_x11_create_child_window(xparent);
        }

        if (None != kui->_window && true == kui->_isa_responder)
        {
            kosp_app_ui_event_responder_add(kui, kui->_window);
        }

        printf("%s\tcreating %p\tsize %d\n",
                __func__,
                kui,
                sizeof(*kui));
    }

    return kui;
}

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
void kosp_ui_init(kosp_ui_t *self, int isa, int x, int y, 
        unsigned int width, unsigned int height,
        bool isa_responder)
{
    kosp_base_init((kosp_base_t *) self, isa);
    kosp_ui_funcs_init(self);
    kosp_ui_set(self, isa, x, y, width, height, isa_responder);

    self->_window = None;
    self->_child_list = kosp_list_create(false, true);
}

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
void kosp_ui_set(kosp_ui_t *self, int isa, int x, int y,
        unsigned int width, unsigned int height,
        bool isa_responder)
{
    kosp_isa_set(self, isa);
    self->_posnsize.x = x;
    self->_posnsize.y = y;
    self->_posnsize.width = width;
    self->_posnsize.height = height;
    self->_isa_responder = isa_responder;
}

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
void kosp_ui_funcs_init(kosp_ui_t *self)
{
    self->destroy = kosp_ui_destroy;
    self->init_palette = kosp_ui_init_palette;
    self->draw = kosp_ui_draw;
    self->draw_children = kosp_ui_draw_children;
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
bool kosp_ui_isa_responder(void *vself)
{
    if (NULL != vself)
    {
        return ((kosp_ui_t *) vself)->_isa_responder;
    }

    return false;
}

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
void kosp_ui_isa_responder_set(void *vself, bool isa_responder)
{
    if (NULL != vself)
    {
        kosp_ui_t *self = (kosp_ui_t *) vself;

        if (self->_isa_responder == isa_responder)
        {
            return;
        }

        self->_isa_responder = isa_responder;

        if (self->_isa_responder)
        {
            kosp_app_ui_event_responder_add(self, self->_window);
        }
        else
        {
            kosp_app_ui_event_responder_remove(self, self->_window);
        }
    }
}

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
int kosp_ui_width(const void *vself)
{
    if (vself)
    {
        return ((const kosp_ui_t *) vself)->_posnsize.width;
    }

    return -1;
}

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
int kosp_ui_height(const void *vself)
{
    if (vself)
    {
        return ((const kosp_ui_t *) vself)->_posnsize.height;
    }

    return -1;
}

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
Window kosp_ui_window(const void *vself)
{
    if (vself)
    {
        return ((const kosp_ui_t *) vself)->_window;
    }

    return None;
}

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
void kosp_ui_init_palette_with_data(void *vself,
        unsigned short *color_array,
        int color_array_len)
{
    unsigned short *clr = NULL;
    int len = 0;
    int i;
    int j = KU_CLR_FG_NORMAL;
    XColor xclr;

    kosp_ui_t *self = (kosp_ui_t *) vself;

    if (NULL == self)
    {
        return;
    }

    if (NULL == color_array || 0 == color_array_len)
    {
        clr = _kosp_ui_default_palette;
        len = KU_CLR_MAX;
    }
    else
    {
        clr = color_array;
        len = color_array_len;

        if (len > KU_CLR_MAX)
        {
            len = KU_CLR_MAX;
        }
    }

    for (i = 0; i < len; i++)
    {
        xclr.red = *clr++;
        xclr.green = *clr++;
        xclr.blue = *clr++;
        XAllocColor(kosp_x11_display(),
                DefaultColormap(kosp_x11_display(), kosp_x11_screen()),
                &xclr);
        self->_palette[j] = xclr.pixel;
        j++;
    }
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
    kosp_ui_t *kui = NULL;

    if (NULL == vself)
    {
        return;
    }

    kui = (kosp_ui_t *) vself;

    kosp_app_ui_event_responder_remove(kui, kui->_window);

    if (NULL != kui->_child_list)
    {
        kosp_list_destroy(kui->_child_list);
        kui->_child_list = NULL;
    }

    if (None != kui->_window)
    {
        kosp_x11_destroy_window(kui->_window);
    }

    printf("%s\tdestroying %p\tsize %d\n",
            __func__,
            kui,
            sizeof(*kui));

    kosp_base_destroy(vself);
}

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
void kosp_ui_init_palette(void *vself)
{
    kosp_ui_init_palette_with_data(vself, NULL, 0);
}

void kosp_ui_draw(void *vself)
{
}

void kosp_ui_draw_children(void *vself)
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

    kosp_list_add(((kosp_ui_t *) vself)->_child_list, child, add_front);
}

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
kosp_base_t *kosp_ui_remove(void *vself, void *child)
{
    kosp_base_t *retval = NULL;

    if (NULL == vself || NULL == child)
    {
        return NULL;
    }

    retval = kosp_list_remove(((kosp_ui_t *) vself)->_child_list, child);
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

