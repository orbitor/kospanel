/*-------------------------------------------------------------------------*/
/* kosp_rect.h
 *
 * This module is part of kospanel
 * Copyright (c) 2013, James DeLisle    <jd@luckygreenfrog.com>
 * See LICENSE for details
 */
/*-------------------------------------------------------------------------*/

#ifndef COM_LUCKYGREENFROG_KOSP_RECT_H_
#define COM_LUCKYGREENFROG_KOSP_RECT_H_

typedef struct _kosp_rect_t kosp_rect;

struct _kosp_rect_t
{
    int x;
    int y;
    int width;
    int height;
};

#endif  /* COM_LUCKYGREENFROG_KOSP_RECT_H_ */

