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

typedef struct _kosp_list_t kosp_list;
typedef struct _kosp_list_element_t kosp_list_element;

/*
 * typedef bool (*list_find_callback) (const void *ptr);
 * typedef int (*list_sort_callback) (const void *left, const void *right);
 */

#define KOSP_LIST_MEMBERS_DECLARE \
    KOSP_BASE_MEMBERS_DECLARE \
    kosp_list_element  *_first; \
    kosp_list_element  *_last; \
    kosp_list_element  *_cache; \
    bool                _allow_dups;

#define KOSP_LIST_ELEMENT_MEMBERS_DECLARE \
    KOSP_BASE_MEMBERS_DECLARE \
    kosp_list_element  *_next; \
    kosp_list_element  *_prev; \
    kosp_base          *_ptr;

kosp_list *kosp_list_create(bool allow_dups);
kosp_base *kosp_list_first(kosp_list *self);
kosp_base *kosp_list_last(kosp_list *self);
kosp_base *kosp_list_next(kosp_list *self, void *ptr);
bool       kosp_list_add(kosp_list *self, void *ptr, bool add_front);
kosp_base *kosp_list_remove(kosp_list *self, void *ptr);

struct _kosp_list_t
{
    KOSP_LIST_MEMBERS_DECLARE
};

struct _kosp_list_element_t
{
    KOSP_LIST_ELEMENT_MEMBERS_DECLARE
};

#endif  /* COM_LUCKYGREENFROG_KOSP_LIST_H_ */

