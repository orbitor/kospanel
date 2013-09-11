/*-------------------------------------------------------------------------*/
/* kosp_task_bar.h
 *
 * This module is part of kospanel
 * Copyright (c) 2013, James DeLisle    <jd@luckygreenfrog.com>
 * See LICENSE for details
 */
/*-------------------------------------------------------------------------*/

#ifndef COM_LUCKYGREENFROG_KOSP_TASK_BAR_H_
#define COM_LUCKYGREENFROG_KOSP_TASK_BAR_H_

typedef struct _kosp_task_bar_t kosp_task_bar;

kosp_task_bar *kosp_task_bar_create(int width, int height);
void kosp_task_bar_destroy(kosp_task_bar *self);


struct _kosp_task_bar_t
{
    KOSP_UI_MEMBERS_DECLARE
    kosp_task_button*   buttons;
    Window              task_bar;
    Window              grip;
    Window              time;
};

#endif  /* COM_LUCKYGREENFROG_KOSP_TASK_BAR_H_ */

