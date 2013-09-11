/*-------------------------------------------------------------------------*/
/* kosp_task_button.h
 *
 * This module is part of kospanel
 * Copyright (c) 2013, James DeLisle    <jd@luckygreenfrog.com>
 * See LICENSE for details
 */
/*-------------------------------------------------------------------------*/

#ifndef COM_LUCKYGREENFROG_KOSP_TASK_BUTTON_H_
#define COM_LUCKYGREENFROG_KOSP_TASK_BUTTON_H_

typedef struct _kosp_task_button_t kosp_task_button;

kosp_task_button *kosp_task_button_create(int width, int height);

typedef struct _kosp_task_button_t
{
    KOSP_UI_MEMBERS_DECLARE
    kosp_task*       task;           /* a single task */
    kosp_task_group* group;          /* a group of tasks */
    Window           button;         /* window of the button */
    Window           icon;           /* window of the icon (from task or group) */
    Window           text;           /* window of the text */
    int              workspace;      /* on which workspace am I? */
    int              monitor;        /* on which monitor am I? */
};

#endif  /* COM_LUCKYGREENFROG_KOSP_TASK_BUTTON_H_ */

