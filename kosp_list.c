/*-------------------------------------------------------------------------*/
/* kosp_list.c
 *
 * This module is part of kospanel
 * Copyright (c) 2013, James DeLisle    <jd@luckygreenfrog.com>
 * See LICENSE for details
 */
/*-------------------------------------------------------------------------*/

#include <string.h>
#include <stdlib.h>
#include "kosp_list.h"

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
static kosp_list_element_t *_kosp_list_element_create(void *ptr);
static kosp_list_element_t *_kosp_list_element_find_by_ptr(
        kosp_list_t *self, void *ptr);

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
    if (NULL == self->_first || NULL == ptr)
    {
        return NULL;
    }

    if (NULL != self->_cache)
    {
        if (NULL != self->_cache->_next)
        {
            if (ptr == self->_cache->_next->_ptr)
            {
                self->_cache = self->_cache->_next;
                return self->_cache->_ptr;
            }
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
                    self->_cache = kle->_next;
                    return self->_cache->_ptr;
                }
                else
                {
                    break;
                }
            }

            kle = kle->_next;
        }
    }

    return NULL;
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

    kle = _kosp_list_element_create(ptr);

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
        else if (self->_last == kle)
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
        kle->destroy(kle);
    }

    return retptr;
}

/*-------------------------------------------------------------------------*/
/* virtual functions */
/*-------------------------------------------------------------------------*/
void kosp_list_destroy(void *vself)
{
    kosp_list_t *self = (kosp_list_t *) vself;
    kosp_list_element_t *element;
    kosp_list_element_t *next;
    kosp_base_t *ptr = NULL;

    if (NULL == self)
    {
        return;
    }

    element = self->_first;

    while(element)
    {
        next = element->_next;

        if (self->_owns_entries)
        {
            ptr = element->_ptr;
        }

        element->destroy(element);

        if (NULL != ptr)
        {
            ptr->destroy(ptr);
        }

        element = next;
    }

    kosp_base_destroy(self);
}

/*-------------------------------------------------------------------------*/
/* static functions */
/*-------------------------------------------------------------------------*/
static kosp_list_element_t *_kosp_list_element_create(void *ptr)
{
    kosp_list_element_t *kle = 
        (kosp_list_element_t *) malloc(sizeof(kosp_list_element_t));

    if (kle)
    {
        memset(kle, 0, sizeof(kosp_list_element_t));
        kosp_base_init((kosp_base_t *) kle, KPT_LIST_ELEMENT);
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

