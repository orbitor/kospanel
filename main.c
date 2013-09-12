
#include <stdlib.h>
#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

//#define bool int

#include "kosp_types.h"
#include "kosp_ui.h"

int main(int argc, char *argv[])
{
    kosp_ui *kui = kosp_ui_create(KPT_UI, 100, 200);

    printf("%s\tkui 0x%08x\n",
            __func__,
            (unsigned int) kui);


    kui->destroy(kui);

    return 0;
}

