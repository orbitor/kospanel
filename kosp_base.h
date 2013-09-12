/*-------------------------------------------------------------------------*/
/* kosp_base.h
 *
 * This module is part of kospanel
 * Copyright (c) 2013, James DeLisle    <jd@luckygreenfrog.com>
 * See LICENSE for details
 */
/*-------------------------------------------------------------------------*/

#ifndef COM_LUCKYGREENFROG_KOSP_BASE_H_
#define COM_LUCKYGREENFROG_KOSP_BASE_H_

typedef struct _kosp_base_t kosp_base;

/*-------------------------------------------------------------------------*/
/* init functions */
/*-------------------------------------------------------------------------*/
void kosp_base_init_default(kosp_base *self);
void kosp_base_init(kosp_base *self, int isa);

/*-------------------------------------------------------------------------*/
/* type functions */
/*-------------------------------------------------------------------------*/
int kosp_isa(void *vself);
void kosp_isa_set(void *vself, int isa);

/*-------------------------------------------------------------------------*/
/* virtual functions */
/*-------------------------------------------------------------------------*/
typedef void (*base_destroy)(void *vself);
void kosp_base_destroy(void *vself);

/*-------------------------------------------------------------------------*/
/* type declaration */
/*-------------------------------------------------------------------------*/
#define KOSP_BASE_MEMBERS_DECLARE \
    base_destroy        destroy; \
    int                 isa;

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
struct _kosp_base_t
{
    KOSP_BASE_MEMBERS_DECLARE
};

#endif  /* COM_LUCKYGREENFROG_KOSP_BASE_H_ */

