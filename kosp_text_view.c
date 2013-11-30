/*-------------------------------------------------------------------------*/
/* kosp_text_view.c
 *
 * This module is part of kospanel
 * Copyright (c) 2013, James DeLisle    <jd@luckygreenfrog.com>
 * See LICENSE for details
 */
/*-------------------------------------------------------------------------*/

#include "kosp_x11.h"
#include "kosp_text_view.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
#define KP_SELF_CAST_LOCAL KP_SELF_CAST(kosp_text_view)

/*-------------------------------------------------------------------------*/
/* create and init functions */
/*-------------------------------------------------------------------------*/
kosp_text_view *kosp_text_view_alloc(void)
{
    kosp_text_view *self = (kosp_text_view *) malloc(sizeof(kosp_text_view));

    if (NULL != self)
    {
        memset(self, 0, sizeof(kosp_text_view));
    }

    return self;
}

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
kosp_text_view *kosp_text_view_alloc_init(void *parent, int x,
        int y, unsigned int width, unsigned int height,
        const char *text)
{
    kosp_text_view *self = kosp_text_view_alloc();

    if (NULL != self)
    {
        kosp_text_view_init(self, parent, x, y, width, height, text);
    }

    return self;
}

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
void kosp_text_view_init(kosp_text_view *self,
        void *parent, int x, int y,
        unsigned int width, unsigned int height,
        const char *text)
{
    kosp_ui_init((kosp_ui_t *) self, 
            KPT_LABEL, parent, x, y, width, height, true);
    kosp_text_view_funcs_init(self);
    self->_text = text;
}

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
void kosp_text_view_funcs_init(kosp_text_view *self)
{
    self->destroy = kosp_text_view_destroy;
    self->draw = kosp_text_view_draw;
    self->button_press_notify = kosp_text_view_event_button_press;
    self->button_release_notify = kosp_text_view_event_button_released;
}

/*-------------------------------------------------------------------------*/
/* type functions */
/*-------------------------------------------------------------------------*/
void kosp_text_view_font_load(kosp_text_view *self, const char *font_name)
{
    kosp_text_view_font_unload(self);

    self->_font_info = XLoadQueryFont(kosp_x11_display(),
            font_name);

    if (self->_font_info)
    {
        XSetFont(kosp_x11_display(),
                self->_gc,
                self->_font_info->fid);
    }
}

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
void kosp_text_view_font_unload(kosp_text_view *self)
{
    if (NULL != self->_font_info)
    {
        XUnloadFont(kosp_x11_display(),
                self->_font_info->fid);
    }
}
/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
void kosp_text_view_alignment_set(kosp_text_view *self, int alignment)
{
    if (KT_ALIGN_NONE < alignment && alignment < KT_ALIGN_MAX)
    {
        self->_alignment = alignment;
    }
}

/*-------------------------------------------------------------------------*/
/* virtual function overrides */
/*-------------------------------------------------------------------------*/
void kosp_text_view_destroy(void *vself)
{
    kosp_text_view_font_unload(vself);
    kosp_ui_destroy(vself);
}

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
void kosp_text_view_draw(void *vself)
{
    KP_SELF_CAST_LOCAL;

    printf("%s\n", __func__);

    /*TODO: don't disregard the alignment */
    int x = 10;
    int y = 20;

    kosp_ui_color_select_into_fg(vself, KU_CLR_BG_HOVER);
    XDrawString(kosp_x11_display(),
            self->_window,
            self->_gc,
            x++,
            y++,
            self->_text,
            strlen(self->_text));

    kosp_ui_color_select_into_fg(vself, KU_CLR_FG_HOVER);
    XDrawString(kosp_x11_display(),
            self->_window,
            self->_gc,
            x,
            y,
            self->_text,
            strlen(self->_text));
}

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
void kosp_text_view_event_button_press(void *vself,
        XButtonPressedEvent *event)
{
    KP_SELF_CAST_LOCAL;
    printf("%s\tforwarding to parent %p\n",
            __func__,
            self->_parent);
    self->_parent->button_press_notify(self->_parent, event);
}

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
void kosp_text_view_event_button_released(void *vself,
        XButtonReleasedEvent *event)
{
    KP_SELF_CAST_LOCAL;
    printf("%s\tforwarding to parent %p\n",
            __func__,
            self->_parent);
    self->_parent->button_release_notify(self->_parent, event);
}

