/*---------------------------------------------------------------------------*/
/* kosp_task.h
 *
 * This module is part of kospanel
 * Copyright (c) 2013, James DeLisle    <jd@luckygreenfrog.com>
 * See LICENSE for details
 */
/*---------------------------------------------------------------------------*/

#ifndef KOSP_TASK_H_
#define KOSP_TASK_H_

typedef struct _kosp_task_t kosp_task;

struct _kosp_task_t
{
    char*       client_title;
    Window      client;
    Pixmap      icon;
    Pixmap      mask;
    Pixmap      thumb_nail;
    bool        focused;
    bool        iconified;
    bool        icon_copied;
};

typedef struct _kosp_task_group_t kosp_task_group;

struct _kosp_task_group_t
{
    kosp_task*      tasks;
    Pixmap          icon;
    Pixmap          mask;
};

#endif  /* KOSP_TASK_H_ */

