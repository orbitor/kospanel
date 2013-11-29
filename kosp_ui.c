/*-------------------------------------------------------------------------*/
/* kosp_ui.c
 *
 * This module is part of kospanel
 * Copyright (c) 2013, James DeLisle    <jd@luckygreenfrog.com>
 * See LICENSE for details
 */
/*-------------------------------------------------------------------------*/

#include "kosp_x11.h"
#include "kosp_ui.h"
#include "kosp_app.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
static unsigned short _kosp_ui_default_palette[] =
{
    0x0000, 0xffff, 0x0000,     /* KU_CLR_FG_NORMAL */
    0x0000, 0x0000, 0xffff,     /* KU_CLR_BG_NORMAL */
    0xffff, 0xffff, 0xffff,     /* KU_CLR_FG_SELECTED */
    0x8617, 0x8617, 0x8617,     /* KU_CLR_BG_SELECTED */
    0xd75c, 0xd75c, 0xd75c,     /* KU_CLR_FG_DISABLED */
    0x8617, 0x8617, 0x8617,     /* KU_CLR_BG_DISALBED */
    0xffff, 0xffff, 0xffff,     /* KU_CLR_FG_HOVER */
    0x0000, 0x9999, 0xcccc,     /* KU_CLR_BG_HOVER */
};

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
kosp_ui_t *kosp_ui_alloc(void)
{
    kosp_ui_t *self = (kosp_ui_t *) malloc(sizeof(kosp_ui_t));

    if (NULL != self)
    {
        memset(self, 0, sizeof(kosp_ui_t));
    }

    return self;
}

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
kosp_ui_t *kosp_ui_alloc_init(int isa, void *parent, int x, int y,
        unsigned int width, unsigned int height,
        bool isa_responder)
{
    kosp_ui_t *self = kosp_ui_alloc();

    if (NULL != self)
    {
        kosp_ui_init(self, isa, parent, x, y, width, height, isa_responder);
    }

    return self;
}

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
void kosp_ui_init(kosp_ui_t *self, int isa, void *parent, int x, int y, 
        unsigned int width, unsigned int height,
        bool isa_responder)
{
    Window xparent = None;

    kosp_base_init((kosp_base_t *) self, isa);
    kosp_ui_funcs_init(self);
    kosp_ui_set(self, isa, x, y, width, height, isa_responder);
    kosp_ui_init_palette(self);

    self->_window = None;
    self->_child_list = kosp_list_create(false, true);

    if (NULL != parent)
    {
        xparent = ((kosp_ui_t *) parent)->_window;
        kosp_ui_add(parent, self, false);
    }

    if (None == xparent)
    {
        self->_window = kosp_x11_create_toplevel_window(x, y,
                width, height, self->_palette[KU_CLR_BG_NORMAL]);
    }
    else
    {
        self->_window = kosp_x11_create_child_window(xparent,
                x, y, width, height,
                self->_palette[KU_CLR_BG_NORMAL]);
    }

    self->_gc = kosp_x11_create_default_gc(self->_window);

    if (None != self->_window && true == self->_isa_responder)
    {
        kosp_app_ui_event_responder_add(self, self->_window);
    }
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
    self->show = kosp_ui_show;
    self->hide = kosp_ui_hide;
    self->button_press = kosp_ui_event_button_press;
    self->button_release = kosp_ui_event_button_release;
    self->pointer_moved = kosp_ui_event_pointer_moved;
    self->enter_window = kosp_ui_event_enter_window;
    self->leave_window = kosp_ui_event_leave_window;
    self->client_message = kosp_ui_event_client_message;
    self->property_notify = kosp_ui_event_property_notify;
    self->configure_notify = kosp_ui_event_configure_notify;
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
bool kosp_ui_is_enabled(void *vself)
{
    return ((kosp_ui_t *) vself)->_is_enabled;
}

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
void kosp_ui_is_enabled_set(void *vself, bool is_enabled)
{
    kosp_ui_t *self = (kosp_ui_t *) vself;

    /*TODO: if we are being disabled, then we should also disable
     * all of our sub-windows.
     */
    self->_is_enabled = is_enabled;
    kosp_ui_smudge(self);
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
        printf("%s\tindex %d\tcolor %lu\n",
                __func__,
                j,
                xclr.pixel);
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
/*-------------------------------------------------------------------------*/
unsigned long kosp_ui_color_get(void *vself, int index)
{
    return ((kosp_ui_t *) vself)->_palette[index];
}

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
void kosp_ui_color_set(void *vself, int index, unsigned long color)
{
    if (index < KU_CLR_MAX)
    {
        ((kosp_ui_t *) vself)->_palette[index] = color;
    }
}

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
void kosp_ui_color_select_into_bg(void *vself, int index)
{
    XSetWindowBackground(kosp_x11_display(),
            kosp_ui_window(vself),
            kosp_ui_color_get(vself, index));
}

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
void kosp_ui_color_select_into_fg(void *vself, int index)
{
    XSetForeground(kosp_x11_display(),
            ((kosp_ui_t *) vself)->_gc,
            kosp_ui_color_get(vself, index));
}

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
void kosp_ui_background_color_set(void *vself, unsigned long color)
{
    XSetWindowBackground(kosp_x11_display(),
            ((kosp_ui_t *) vself)->_window,
            color);
    kosp_ui_smudge(vself);
}

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
void kosp_ui_smudge(void *vself)
{
    if (NULL != vself)
    {
        XClearWindow(kosp_x11_display(),
                ((kosp_ui_t *) vself)->_window);
    }
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

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
void kosp_ui_draw(void *vself)
{
    kosp_ui_t *self = (kosp_ui_t *) vself;
    int retval;

    printf("%s\tvself %p\n", __func__, vself);

    kosp_ui_color_select_into_fg(self, KU_CLR_FG_NORMAL);

    retval = XSetLineAttributes(kosp_x11_display(),
            self->_gc,
            2,
            LineSolid,
            CapButt,
            JoinMiter);
    /*
    retval = XDrawRectangle(kosp_x11_display(),
            self->_window,
            self->_gc,
            0,
            0,
            self->_posnsize.width,
            self->_posnsize.height);
    */
    /*
    retval = XSetForeground(kosp_x11_display(),
            self->_gc,
            self->_palette[KU_CLR_FG_DISABLED]);
    */
    retval = XDrawLine(kosp_x11_display(),
            self->_window,
            self->_gc,
            0,
            0,
            self->_posnsize.width,
            self->_posnsize.height);
    retval = XDrawLine(kosp_x11_display(),
            self->_window,
            self->_gc,
            self->_posnsize.width,
            0,
            0,
            self->_posnsize.height);
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

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
void kosp_ui_show(void *vself)
{
    kosp_ui_t *self = (kosp_ui_t *) vself;
    kosp_ui_t *child = NULL;

    if (NULL == vself)
    {
        return;
    }

    /* first map all of the children, if present */
    child = (kosp_ui_t *) kosp_list_first(self->_child_list);

    while(child)
    {
        child->show(child);
        child = (kosp_ui_t *) kosp_list_next(self->_child_list, child);
    }

    printf("%s\tself %p\n",
            __func__,
            self);
    
    kosp_x11_map_window(self->_window);
}

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
void kosp_ui_hide(void *vself)
{
    if (NULL != vself)
    {
        kosp_x11_unmap_window(((kosp_ui_t *) vself)->_window);
    }
}

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
int kosp_ui_event_button_press(void *vself, XButtonPressedEvent *event)
{
    printf("%s\tvself %p\n", __func__, vself);
    return 1;
}

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
int kosp_ui_event_button_release(void *vself, XButtonReleasedEvent *event)
{
    printf("%s\tvself %p\n", __func__, vself);
    return 1;
}

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
int kosp_ui_event_pointer_moved(void *vself, XPointerMovedEvent *event)
{
    printf("%s\tvself %p\n", __func__, vself);
    return 1;
}

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
int kosp_ui_event_enter_window(void *vself, XEnterWindowEvent *event)
{
    printf("%s\tvself %p\n", __func__, vself);
    kosp_ui_background_color_set(vself,
            ((kosp_ui_t *) vself)->_palette[KU_CLR_BG_HOVER]);
    kosp_ui_smudge(vself);
    ((kosp_ui_t *) vself)->draw(vself);
    return 1;
}

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
int kosp_ui_event_leave_window(void *vself, XLeaveWindowEvent *event)
{
    printf("%s\tvself %p\n", __func__, vself);
    kosp_ui_background_color_set(vself,
            ((kosp_ui_t *) vself)->_palette[KU_CLR_BG_NORMAL]);
    kosp_ui_smudge(vself);
    ((kosp_ui_t *) vself)->draw(vself);
    return 1;
}

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
int kosp_ui_event_client_message(void *vself, XClientMessageEvent *event)
{
    printf("%s\tvself %p\n", __func__, vself);
    return 1;
}

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
int kosp_ui_event_property_notify(void *vself, XPropertyEvent *event)
{
    printf("%s\tvself %p\n", __func__, vself);
    return 1;
}

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
int kosp_ui_event_configure_notify(void *vself, XConfigureEvent *event)
{
    printf("%s\tvself %p\n", __func__, vself);
    return 1;
}

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
int kosp_ui_event_expose(void *vself, XExposeEvent *event)
{
    printf("%s\tvself %p\n", __func__, vself);
    ((kosp_ui_t *) vself)->draw(vself);
    return 1;
}

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
int kosp_ui_event_unmap_notify(void *vself, XUnmapEvent *event)
{
    printf("%s\tvself %p\n", __func__, vself);
    return 1;
}

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
int kosp_ui_event_destroy_notify(void *vself, XDestroyWindowEvent *event)
{
    printf("%s\tvself %p\n", __func__, vself);
    return 1;
}

