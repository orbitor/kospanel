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

bool kosp_app_init(int argc, char *argv[]);
void kosp_app_shutdown(void);

int kosp_app_exec(void);

#endif  /* COM_LUCKYGREENFROG_KOSP_APP_H_ */

