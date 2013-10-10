/*-------------------------------------------------------------------------*/
/* kosp_list.h
 *
 * This module is part of kospanel
 * Copyright (c) 2013, James DeLisle    <jd@luckygreenfrog.com>
 * See LICENSE for details
 */
/*-------------------------------------------------------------------------*/

#ifndef COM_LUCKYGREENFROG_KOSP_LIST_H_
#define COM_LUCKYGREENFROG_KOSP_LIST_H_

#include "kosp_types.h"
#include "kosp_base.h"

/*-------------------------------------------------------------------------*/
/* typedef */
/*-------------------------------------------------------------------------*/
typedef struct _kosp_list_t kosp_list_t;
typedef struct _kosp_list_element_t kosp_list_element_t;

/*
 * typedef bool (*list_find_callback) (const void *ptr);
 * typedef int (*list_sort_callback) (const void *left, const void *right);
 */

/*-------------------------------------------------------------------------*/
/* type functions */
/*-------------------------------------------------------------------------*/
kosp_list_t *kosp_list_create(bool allow_dups, bool owns_entries);
kosp_base_t *kosp_list_first(kosp_list_t*self);
kosp_base_t *kosp_list_last(kosp_list_t*self);
kosp_base_t *kosp_list_next(kosp_list_t*self, void *ptr);
bool         kosp_list_add(kosp_list_t*self, void *ptr, bool add_front);
kosp_base_t *kosp_list_remove(kosp_list_t*self, void *ptr);

/*-------------------------------------------------------------------------*/
/* virtual functions */
/*-------------------------------------------------------------------------*/
void       kosp_list_destroy(void *vself);

/*-------------------------------------------------------------------------*/
/* type declarations */
/*-------------------------------------------------------------------------*/
#define KOSP_LIST_MEMBERS_DECLARE \
    KOSP_BASE_MEMBERS_DECLARE \
    kosp_list_element_t *_first; \
    kosp_list_element_t *_last; \
    kosp_list_element_t *_cache; \
    bool                 _allow_dups; \
    bool                 _owns_entries;

#define KOSP_LIST_ELEMENT_MEMBERS_DECLARE \
    KOSP_BASE_MEMBERS_DECLARE \
    kosp_list_element_t *_next; \
    kosp_list_element_t *_prev; \
    kosp_base_t         *_ptr;

struct _kosp_list_t
{
    KOSP_LIST_MEMBERS_DECLARE
};

struct _kosp_list_element_t
{
    KOSP_LIST_ELEMENT_MEMBERS_DECLARE
};

#endif  /* COM_LUCKYGREENFROG_KOSP_LIST_H_ */

