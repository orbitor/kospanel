/*-------------------------------------------------------------------------*/
/* kosp_app.c
 *
 * This module is part of kospanel
 * Copyright (c) 2013, James DeLisle    <jd@luckygreenfrog.com>
 * See LICENSE for details
 */
/*-------------------------------------------------------------------------*/

#include "kosp_base.h"
#include "kosp_app.h"

static void kosp_app_destroy(void *vself);

typedef struct _kosp_app_t kosp_app_t;
struct _kosp_app_t
{
    KOSP_BASE_MEMBERS_DECLARE
};

static kosp_app_t kosp_app;

bool kosp_app_init(int argc, char *argv[])
{
}

void kosp_app_shutdown(void)
{
}

int kosp_app_exec(void)
{
}

static void kosp_app_destroy(void *vself)
{
    kosp_base_destory(vself);
}

#endif  /* COM_LUCKYGREENFROG_KOSP_APP_H_ */

