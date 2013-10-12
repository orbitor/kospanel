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
typedef struct _kosp_list_t         kosp_list_t;
typedef struct _kosp_list_element_t kosp_list_element_t;

/*
 * typedef int (*kosp_list_sort_callback) (const void *left, const void *right);
 */

typedef bool (*kosp_list_find_callback) (const void *ptr, const void *data);

/*-------------------------------------------------------------------------*/
/* virtual functions */
/*-------------------------------------------------------------------------*/
void kosp_list_destroy(void *vself);

/*-------------------------------------------------------------------------*/
/* type functions */
/*-------------------------------------------------------------------------*/
kosp_list_t *kosp_list_create(bool allow_dups, bool owns_entries);
kosp_base_t *kosp_list_first(kosp_list_t *self);
kosp_base_t *kosp_list_last(kosp_list_t *self);
kosp_base_t *kosp_list_next(kosp_list_t *self, void *ptr);
bool         kosp_list_add(kosp_list_t *self, void *ptr, bool add_front);
kosp_base_t *kosp_list_remove(kosp_list_t *self, void *ptr);
kosp_base_t *kosp_list_find(kosp_list_t *self, void *ptr);

void kosp_list_set_find_callback(kosp_list_t *self,
        kosp_list_find_callback cb_function);
kosp_base_t *kosp_list_find_by_cb(kosp_list_t *self, const void *compare_data);

#endif  /* COM_LUCKYGREENFROG_KOSP_LIST_H_ */

