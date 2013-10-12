/*-------------------------------------------------------------------------*/
/* kosp_app.h
 *
 * This module is part of kospanel
 * Copyright (c) 2013, James DeLisle    <jd@luckygreenfrog.com>
 * See LICENSE for details
 */
/*-------------------------------------------------------------------------*/

#ifndef COM_LUCKYGREENFROG_KOSP_APP_H_
#define COM_LUCKYGREENFROG_KOSP_APP_H_

#include "kosp_types.h"

/* forward declaration */
struct kosp_ui_t;

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
enum
{
    KA_RS_NONE,
    KS_RS_STARTING_UP,
    KS_RS_RUNNING,
    KS_RS_SHUTTING_DOWN,
    KS_RS_HALTED,
    KS_RS_MAX
};

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
bool kosp_app_init(int argc, char *argv[]);
void kosp_app_shutdown(void);
void kosp_app_destroy(void);

int kosp_app_exec(void);
int kosp_app_run_state(void);

void kosp_app_ui_event_responder_add(kosp_ui_t *responder, Window window);
kosp_ui_t *kosp_app_ui_event_responder_remove(kosp_ui_t *responder,
        Window window);

#endif  /* COM_LUCKYGREENFROG_KOSP_APP_H_ */

