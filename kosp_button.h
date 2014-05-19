/*-------------------------------------------------------------------------*/
/* kosp_button.h
 *
 * This module is part of kospanel
 * Copyright (c) 2013, James DeLisle    <jd@luckygreenfrog.com>
 * See LICENSE for details
 */
/*-------------------------------------------------------------------------*/

#ifndef COM_LUCKYGREENFROG_KOSP_BUTTON_H_
#define COM_LUCKYGREENFROG_KOSP_BUTTON_H_

#include "kosp_types.h"
#include "kosp_ui.h"

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
typedef struct _kosp_button kosp_button;

/*-------------------------------------------------------------------------*/
/* create and init functions */
/*-------------------------------------------------------------------------*/
kosp_button *kosp_button_alloc(void);
kosp_button *kosp_button_alloc_init(void *parent, int x,
        int y, unsigned int width, unsigned int height);
void kosp_button_init(kosp_button *self, void *parent, int x,
        int  y, unsigned int width, unsigned int height);
void kosp_button_funcs_init(kosp_button *self);

/*-------------------------------------------------------------------------*/
/* type functions */
/*-------------------------------------------------------------------------*/
void kosp_button_delegate_set(kosp_button *button,
        void *delegate,
        void(*button_clicked_callback)(void *delegate, kosp_button *button));
void kosp_button_delegate_clear(kosp_button *button);

/*-------------------------------------------------------------------------*/
/* virtual function overrides */
/*-------------------------------------------------------------------------*/
void kosp_button_event_button_press(void *vself,
        XButtonPressedEvent *event);
void kosp_button_event_button_released(void *vself,
        XButtonReleasedEvent *event);

/*-------------------------------------------------------------------------*/
/* type definition */
/*-------------------------------------------------------------------------*/
#define KOSP_BUTTON_MEMBERS_DECLARE \
    KOSP_UI_MEMBERS_DECLARE; \
    void *_delegate; \
    void (*button_clicked_callback)(void *delegate, kosp_button *button);

struct _kosp_button
{
    KOSP_BUTTON_MEMBERS_DECLARE
};

#endif  /* COM_LUCKYGREEFROG_KOSP_BUTTON_H_ */

