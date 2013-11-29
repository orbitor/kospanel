/*-------------------------------------------------------------------------*/
/* kosp_text_view.h
 *
 * This module is part of kospanel
 * Copyright (c) 2013, James DeLisle    <jd@luckygreenfrog.com>
 * See LICENSE for details
 */
/*-------------------------------------------------------------------------*/

#ifndef COM_LUCKYGREENFROG_KOSP_TEXT_VIEW_H_
#define COM_LUCKYGREENFROG_KOSP_TEXT_VIEW_H_

/*-------------------------------------------------------------------------*/
/* text alignment enum */
/*-------------------------------------------------------------------------*/
enum
{
    KT_ALIGN_NONE,
    KT_ALIGN_LEFT,
    KT_ALIGN_CENTER,
    KT_ALIGN_RIGHT,
    KT_ALIGN_TOP,
    KT_ALIGN_BOTTOM,
    KT_ALIGN_LEFT_TOP,
    KT_ALIGN_CENTER_TOP,
    KT_ALIGN_RIGHT_TOP,
    KT_ALIGN_LEFT_CENTER,
    KT_ALIGN_RIGHT_CENTER,
    KT_ALIGN_LEFT_BOTTOM,
    KT_ALIGN_CENTER_BOTTOM,
    KT_ALIGN_RIGHT_BOTTOM,
    KT_ALIGN_MAX
};

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
typedef struct _kosp_text_view kosp_text_view;

/*-------------------------------------------------------------------------*/
/* create and init functions */
/*-------------------------------------------------------------------------*/
kosp_text_view *kosp_text_view_alloc(void);
kosp_text_view *kosp_text_view_alloc_init(void *parent, int x,
        int y, unsigned int width, unsigned int height,
        const char *text);
void kosp_text_view_init(kosp_text_view *self,
        void *parent, int x, int y,
        unsigned int width, unsigned int height,
        const char *text);
void kosp_text_view_funcs_init(kosp_text_view *self);

/*-------------------------------------------------------------------------*/
/* type functions */
/*-------------------------------------------------------------------------*/
void kosp_text_view_font_load(kosp_text_view *self, const char *font_name);
void kosp_text_view_font_unload(kosp_text_view *self);
void kosp_text_view_alignment_set(kosp_text_view *self, int alignment);

/*-------------------------------------------------------------------------*/
/* virtual function overrides */
/*-------------------------------------------------------------------------*/
void kosp_text_view_destroy(void *vself);
void kosp_text_view_draw(void *vself);

#endif  /* COM_LUCKYGREENFROG_KOSP_TEXT_VIEW_H_ */

