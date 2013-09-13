/*-------------------------------------------------------------------------*/
/* kosp_list.h
 *
 * This module is part of kospanel
 * Copyright (c) 2013, James DeLisle    <jd@luckygreenfrog.com>
 * See LICENSE for details
 */
/*-------------------------------------------------------------------------*/

#include <string.h>
#include <stdlib.h>
#include "kosp_types.h"
#include "kosp_list.h"

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
static kosp_list_element *_kosp_list_element_find_by_ptr(
        kosp_list *self, void *ptr);
static kosp_list_element *_kosp_list_element_create(void *ptr);

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
kosp_list *kosp_list_create(void)
{
    kosp_list *kl = (kosp_list *) malloc(sizeof(kosp_list));

    if (NULL != kl)
    {
        memset(kl, 0, sizeof(kosp_list));
        kosp_base_init((kosp_base *) kl, KPT_LIST);
    }

    return kl;
}

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
kosp_base *kosp_list_first(kosp_list *self)
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
kosp_base *kosp_list_last(kosp_list *self)
{
    if (NULL != self->_last)
    {
        return self->_last->_ptr;
    }

    return NULL;
}

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
kosp_base *kosp_list_next(kosp_list *self, void *ptr)
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
        kosp_list_element *kle = self->_first;

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
bool kosp_list_add(kosp_list *self, void *ptr, bool add_front)
{
    kosp_list_element *kle = NULL;

    if (NULL == ptr)
    {
        return false;
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
kosp_base *kosp_list_remove(kosp_list *self, void *ptr)
{
    kosp_list_element *kle = _kosp_list_element_find_by_ptr(self, ptr);
    kosp_base *retptr = NULL;

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
/*-------------------------------------------------------------------------*/
static kosp_list_element *_kosp_list_element_find_by_ptr(
        kosp_list *self, void *ptr)
{
    kosp_list_element *kle = self->_first;

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

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
static kosp_list_element *_kosp_list_element_create(void *ptr)
{
    kosp_list_element *kle = 
        (kosp_list_element *) malloc(sizeof(kosp_list_element));

    if (kle)
    {
        memset(kle, 0, sizeof(kosp_list_element));
        kosp_base_init((kosp_base *) kle, KPT_LIST_ELEMENT);
    }

    return kle;
}

