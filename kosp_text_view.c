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
static const char *defaultXftFontName = "sans-12:bold";
static XftFont *_default_xft_font = NULL;

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
#if 0
    kosp_text_view_font_unload(self);

    self->_font_info = XLoadQueryFont(kosp_x11_display(),
            font_name);

    if (self->_font_info)
    {
        XSetFont(kosp_x11_display(),
                self->_gc,
                self->_font_info->fid);
    }
#endif

    if (NULL == _default_xft_font)
    {
        _default_xft_font = XftFontOpenName(kosp_x11_display(),
                kosp_x11_screen(),
                defaultXftFontName);
    }

    self->_xft_font = _default_xft_font;
    self->_xft_draw = XftDrawCreate(kosp_x11_display(),
            self->_window,
            kosp_x11_visual(),
            kosp_x11_colormap());
}

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
void kosp_text_view_font_unload(kosp_text_view *self)
{
#if 0
    if (NULL != self->_font_info)
    {
        XFreeFont(kosp_x11_display(),
                self->_font_info);

        self->_font_info = NULL;
    }
#endif
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

#if 0
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
#endif

    if (NULL != self->_xft_font)
    {
        XftColor clr;
        clr.pixel = self->_palette[1];
        XftDrawStringUtf8(self->_xft_draw,
                &clr,
                self->_xft_font,
                10,
                20,
                (const unsigned char *) self->_text,
                strlen(self->_text));
    }
    else
    {
        printf("===> %s\tself->_xft_font is NULL\n", __func__);
    }
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

