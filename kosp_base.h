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

/*-------------------------------------------------------------------------*/
/* typedef */
/*-------------------------------------------------------------------------*/
typedef struct _kosp_base_t kosp_base_t;

/*-------------------------------------------------------------------------*/
/* init functions */
/*-------------------------------------------------------------------------*/
void kosp_base_init(kosp_base_t *self, int isa);
void kosp_base_funcs_init(kosp_base_t *self);

/*-------------------------------------------------------------------------*/
/* type functions */
/*-------------------------------------------------------------------------*/
int kosp_isa(void *vself);
void kosp_isa_set(void *vself, int isa);

/*-------------------------------------------------------------------------*/
/* virtual functions */
/*-------------------------------------------------------------------------*/
void kosp_base_destroy(void *vself);

/*-------------------------------------------------------------------------*/
/* type declaration */
/*-------------------------------------------------------------------------*/
#define KOSP_BASE_MEMBERS_DECLARE \
    void (*destroy) (void *vself); \
    int isa;

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
struct _kosp_base_t
{
    KOSP_BASE_MEMBERS_DECLARE
};

#endif  /* COM_LUCKYGREENFROG_KOSP_BASE_H_ */

