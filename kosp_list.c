/*-------------------------------------------------------------------------*/
/* kosp_list.c
 *
 * This module is part of kospanel
 * Copyright (c) 2013, James DeLisle    <jd@luckygreenfrog.com>
 * See LICENSE for details
 */
/*-------------------------------------------------------------------------*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "kosp_list.h"

/*-------------------------------------------------------------------------*/
/* type declarations */
/*-------------------------------------------------------------------------*/
#define KOSP_LIST_MEMBERS_DECLARE \
    KOSP_BASE_MEMBERS_DECLARE \
    kosp_list_element_t    *_first; \
    kosp_list_element_t    *_last; \
    kosp_list_element_t    *_cache; \
    kosp_list_find_callback _find_callback; \
    int                     _nelements; \
    bool                    _allow_dups; \
    bool                    _owns_entries;

#define KOSP_LIST_ELEMENT_MEMBERS_DECLARE \
    KOSP_BASE_MEMBERS_DECLARE \
    kosp_list_element_t    *_next; \
    kosp_list_element_t    *_prev; \
    kosp_base_t            *_ptr;

struct _kosp_list_t
{
    KOSP_LIST_MEMBERS_DECLARE
};

struct _kosp_list_element_t
{
    KOSP_LIST_ELEMENT_MEMBERS_DECLARE
};

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
static kosp_list_element_t *_kosp_list_element_create(void *ptr,
        bool owns_ptr);
static kosp_list_element_t *_kosp_list_element_find_by_ptr(
        kosp_list_t *self, void *ptr);

/*-------------------------------------------------------------------------*/
/* virtual functions */
/*-------------------------------------------------------------------------*/
void kosp_list_destroy(void *vself)
{
    kosp_list_t *self = (kosp_list_t *) vself;
    kosp_list_element_t *element;
    kosp_list_element_t *next;
    kosp_base_t *ptr;

    if (NULL == self)
    {
        return;
    }

    printf("%s\tdestroying %p\tsize %ld\n",
            __func__,
            self,
            sizeof(kosp_list_t));

    element = self->_first;

    while(element)
    {
        next = element->_next;
        ptr = kosp_list_remove(self, element->_ptr);

        if (self->_owns_entries)
        {
            ptr->destroy(ptr);
        }

        element = next;
    }

    kosp_base_destroy(vself);
}

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
kosp_list_t *kosp_list_create(bool allow_dups, bool owns_entries)
{
    kosp_list_t *kl = (kosp_list_t *) malloc(sizeof(kosp_list_t));

    if (NULL != kl)
    {
        memset(kl, 0, sizeof(kosp_list_t));
        kosp_base_init((kosp_base_t *) kl, KPT_LIST);
        kl->destroy = kosp_list_destroy;
        kl->_allow_dups = allow_dups;
        kl->_owns_entries = owns_entries;

        printf("%s\tcreating %p\tsize %ld\n",
                __func__,
                kl,
                sizeof(*kl));
    }

    return kl;
}

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
kosp_base_t *kosp_list_first(kosp_list_t *self)
{
    if (NULL != self->_first)
    {
        self->_cache = self->_first;
        return self->_first->_ptr;
    }

    return NULL;
}

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
kosp_base_t *kosp_list_last(kosp_list_t *self)
{
    if (NULL != self->_last)
    {
        return self->_last->_ptr;
    }

    return NULL;
}

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
kosp_base_t *kosp_list_next(kosp_list_t *self, void *ptr)
{
    kosp_base_t *retval = NULL;

    if (NULL == self->_first || NULL == ptr)
    {
        return NULL;
    }

    if (NULL != self->_cache && self->_cache->_ptr == ptr)
    {
        if (NULL != self->_cache->_next)
        {
            retval = self->_cache->_next->_ptr;
            self->_cache = self->_cache->_next;
        }
    }
    else
    {
        kosp_list_element_t *kle = self->_first;

        while(kle)
        {
            if (kle->_ptr == ptr)
            {
                if (NULL != kle->_next)
                {
                    retval = kle->_next->_ptr;
                    self->_cache = kle->_next;
                }

                break;
            }

            kle = kle->_next;
        }
    }

    return retval;
}

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
bool kosp_list_add(kosp_list_t *self, void *ptr, bool add_front)
{
    kosp_list_element_t *kle = NULL;

    if (NULL == ptr)
    {
        return false;
    }

    if (false == self->_allow_dups)
    {
        kosp_list_element_t *dup = _kosp_list_element_find_by_ptr(self, ptr);

        if (NULL != dup)
        {
            return false;
        }
    }

    kle = _kosp_list_element_create(ptr, self->_owns_entries);

    if (NULL == kle)
    {
        return false;
    }

    if (NULL == self->_first)
    {
        self->_first = kle;
        self->_last = kle;
    }
    else
    {
        if (true == add_front)
        {
            kle->_next = self->_first;
            self->_first->_prev = kle;
            self->_first = kle;
        }
        else
        {
            kle->_prev = self->_last;
            self->_last->_next = kle;
            self->_last = kle;
        }
    }

    self->_nelements++;

    return true;
}

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
kosp_base_t *kosp_list_remove(kosp_list_t *self, void *ptr)
{
    kosp_list_element_t *kle = _kosp_list_element_find_by_ptr(self, ptr);
    kosp_base_t *retptr = NULL;

    if (kle)
    {
        if (self->_first == kle)
        {
            self->_first = self->_first->_next;
        }

        if (self->_last == kle)
        {
            self->_last = self->_last->_prev;
        }

        if (NULL != kle->_prev)
        {
            kle->_prev->_next = kle->_next;
        }

        if (NULL != kle->_next)
        {
            kle->_next->_prev = kle->_prev;
        }

        self->_cache = self->_first;

        retptr = kle->_ptr;

        printf("%s\tdestroying %p\tsize %ld\n",
                __func__,
                kle,
                sizeof(*kle));

        kle->destroy(kle);

        self->_nelements--;
    }

    return retptr;
}

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
kosp_base_t *kosp_list_find(kosp_list_t *self, void *ptr)
{
    kosp_base_t *retval = NULL;

    kosp_base_t *compare = kosp_list_first(self);

    while(compare)
    {
        if (compare == (kosp_base_t *) ptr)
        {
            retval = compare;
            break;
        }

        compare = kosp_list_next(self, compare);
    }

    return retval;
}

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
void kosp_list_set_find_callback(kosp_list_t *self,
        kosp_list_find_callback cb_function)
{
    self->_find_callback = cb_function;
}

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
kosp_base_t *kosp_list_find_by_cb(kosp_list_t *self, const void *compare_data)
{
    kosp_base_t *retval = NULL;

    if (NULL == self->_find_callback)
    {
        return retval;
    }

    kosp_list_element_t *kle = self->_first;

    while(kle)
    {
        if (true == self->_find_callback(kle->_ptr, compare_data))
        {
            retval = kle->_ptr;
            break;
        }

        kle = kle->_next;
    }

    return retval;
}

/*-------------------------------------------------------------------------*/
/* static functions */
/*-------------------------------------------------------------------------*/
static kosp_list_element_t *_kosp_list_element_create(void *ptr,
        bool owns_ptr)
{
    kosp_list_element_t *kle = 
        (kosp_list_element_t *) malloc(sizeof(kosp_list_element_t));

    if (kle)
    {
        memset(kle, 0, sizeof(kosp_list_element_t));
        kosp_base_init((kosp_base_t *) kle, KPT_LIST_ELEMENT);
        kle->_ptr = ptr;

        printf("%s\tcreating %p\tsize %ld\n",
                __func__,
                kle,
                sizeof(*kle));
    }

    return kle;
}

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
static kosp_list_element_t *_kosp_list_element_find_by_ptr(
        kosp_list_t *self, void *ptr)
{
    kosp_list_element_t *kle = self->_first;

    while(kle)
    {
        if (kle->_ptr == ptr)
        {
            return kle;
        }

        kle = kle->_next;
    }

    return NULL;
}

