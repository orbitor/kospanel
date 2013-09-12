/*-------------------------------------------------------------------------*/
/* kosp_base.h
 *
 * This module is part of kospanel
 * Copyright (c) 2013, James DeLisle    <jd@luckygreenfrog.com>
 * See LICENSE for details
 */
/*-------------------------------------------------------------------------*/

#include <stdlib.h>
#include "kosp_types.h"
#include "kosp_base.h"

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
void kosp_base_init_default(kosp_base *self)
{
    if (self)
    {
        self->destroy = kosp_base_destroy;
        kosp_isa_set(self, KPT_BASE);
    }
}

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
void kosp_base_init(kosp_base *self, int isa)
{
    if (self)
    {
        kosp_base_init_default(self);
        kosp_isa_set(self, isa);
    }
}

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
void kosp_base_destroy(void *vself)
{
    if (NULL != vself)
    {
        free(vself);
        vself = NULL;
    }
}

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
inline int kosp_isa(void *vself)
{
    return ((kosp_base *) vself)->isa;
}

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
void kosp_isa_set(void *vself, int isa)
{
    if (NULL != vself)
    {
        ((kosp_base *) vself)->isa = isa;
    }
}

