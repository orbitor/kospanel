/*-------------------------------------------------------------------------*/
/* kosp_button.c
 *
 * This module is part of kospanel
 * Copyright (c) 2013, James DeLisle    <jd@luckygreenfrog.com>
 * See LICENSE for details
 */
/*-------------------------------------------------------------------------*/

#include "kosp_button.h"

#include <stdlib.h>
#include <string.h>

/*-------------------------------------------------------------------------*/
/* create and init functions */
/*-------------------------------------------------------------------------*/
kosp_button *kosp_button_alloc(void)
{
    kosp_button *self = (kosp_button *) malloc(sizeof(kosp_button));

    if (NULL != self)
    {
        memset(self, 0, sizeof(kosp_button));
    }

    return self;
}

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
kosp_button *kosp_button_alloc_init(void *parent, int x,
        int y, unsigned int width, unsigned int height)
{
    kosp_button *self = kosp_button_alloc();

    if (NULL != self)
    {
        kosp_button_init(self, parent, x, y, width, height);
    }

    return self;
}

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
void kosp_button_init(kosp_button *self, void *parent, int x,
        int  y, unsigned int width, unsigned int height)
{
    kosp_ui_init((kosp_ui_t *) self,
            KPT_BUTTON,
            parent, x, y, width, height, true);
    kosp_button_funcs_init(self);
}

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
void kosp_button_funcs_init(kosp_button *self)
{
    self->button_press_notify = kosp_button_event_button_press;
    self->button_release_notify = kosp_button_event_button_released;
}

/*-------------------------------------------------------------------------*/
/* type functions */
/*-------------------------------------------------------------------------*/
void kosp_button_delegate_set(kosp_button *button,
        void *delegate,
        void(*button_clicked_callback)(void *delegate, kosp_button *button))
{
    button->_delegate = delegate;
    button->button_clicked_callback = button_clicked_callback;
}

/*-------------------------------------------------------------------------*/
/* virtual function overrides */
/*-------------------------------------------------------------------------*/
void kosp_button_event_button_press(void *vself,
        XButtonPressedEvent *event)
{
    kosp_button *self = (kosp_button *) vself;

    if (self->button_clicked_callback)
    {
        self->button_clicked_callback(self->_delegate, self);
    }
}

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
void kosp_button_event_button_released(void *vself,
        XButtonReleasedEvent *event)
{
    kosp_button *self = (kosp_button *) vself;

    if (self->button_clicked_callback)
    {
        self->button_clicked_callback(self->_delegate, self);
    }
}

