/*-------------------------------------------------------------------------*/
/* kosp_types.h
 *
 * This module is part of kospanel
 * Copyright (c) 2013, James DeLisle    <jd@luckygreenfrog.com>
 * See LICENSE for details
 */
/*-------------------------------------------------------------------------*/

#ifndef COM_LUCKYGREENFROG_KOSP_TYPES_H_
#define COM_LUCKYGREENFROG_KOSP_TYPES_H_

#ifndef bool
#define bool int
#endif  /* !bool */

#ifndef true
#define true 1
#endif  /* true */

#ifndef false
#define false 0
#endif  /* false */

typedef enum
{
    KPT_NONE,
    KPT_BASE,
    KPT_LIST,
    KPT_LIST_ELEMENT,
    KPT_UI,
    KPT_PANEL,
    KPT_APP,
    KPT_LABEL,
    KPT_BUTTON,
    KPT_MAX
} KOSP_TYPES;

/* 1 macro. That's it. No more */
#define KP_SELF_CAST(_type) _type *self = (_type *) vself

#endif  /* COM_LUCKYGREENFROG_KOSP_TYPES_H_ */

