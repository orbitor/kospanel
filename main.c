
#include <stdlib.h>
#include <stdio.h>

#include "kosp_x11.h"
#include "kosp_ui.h"

int main(int argc, char *argv[])
{
    kosp_x11_init();

    kosp_ui *kui = kosp_ui_create(KPT_UI, NULL, 10, 20, 100, 200);

    printf("%s\tkui 0x%08x\n",
            __func__,
            (unsigned int) kui);

    kui->destroy(kui);

    kosp_x11_shutdown();

    return 0;
}

