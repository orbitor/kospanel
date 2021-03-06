/*-------------------------------------------------------------------------*/
/* kosp_base.h
 *
 * This module is part of kospanel
 * Copyright (c) 2013, James DeLisle    <jd@luckygreenfrog.com>
 * See LICENSE for details
 */
/*-------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include "kosp_types.h"
#include "kosp_base.h"

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
void kosp_base_init(kosp_base_t *self, int isa)
{
    if (self)
    {
        kosp_base_funcs_init(self);
        kosp_isa_set(self, isa);
    }
}

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
void kosp_base_funcs_init(kosp_base_t *self)
{
    self->destroy = kosp_base_destroy;
}

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
void kosp_base_destroy(void *vself)
{
    if (NULL != vself)
    {
        printf("%s\tdestroying %p\tsize %d\n",
                __func__,
                vself,
                sizeof(kosp_base_t));

        free(vself);
        vself = NULL;
    }
}

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
int kosp_isa(void *vself)
{
    return ((kosp_base_t *) vself)->isa;
}

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
void kosp_isa_set(void *vself, int isa)
{
    if (NULL != vself)
    {
        ((kosp_base_t *) vself)->isa = isa;
    }
}

