
#include <stdlib.h>
#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

#define bool int

#include "kosp_rect.h"
#include "kosp_ui.h"
#include "kosp_task.h"
#include "kosp_task_button.h"
#include "kosp_task_bar.h"

kosp_task_bar *kosp_task_bar_create(int width, int height)
{
    kosp_task_bar *ktb = (kosp_task_bar *) malloc(sizeof(kosp_task_bar));
    ktb->destroy = kosp_task_bar_destroy;
}

void kosp_task_bar_destroy(kosp_task_bar *self)
{
    if (self != NULL)
    {
        free(self);
        self = NULL;
    }
}

int main(int argc, char *argv[])
{
    kosp_task_bar *ktb = kosp_task_bar_create(10, 10);
    kosp_ui *kui = kosp_ui_create(7, 100, 200);

    printf("%s\tktb 0x%08x\n",
            __func__,
            (unsigned int) ktb);

    printf("%s\tktb 0x%08x\n",
            __func__,
            (unsigned int) kui);


    ktb->destroy(ktb);
    kui->destroy(kui);

    return 0;
}

