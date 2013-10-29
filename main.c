
#include <stdlib.h>
#include <stdio.h>

#include "kosp_x11.h"
#include "kosp_ui.h"
#include "kosp_app.h"

int main(int argc, char *argv[])
{
    kosp_ui_t *k1;
    kosp_ui_t *k2;
    kosp_ui_t *k1_2;
    kosp_ui_t *k1_2_1;
    kosp_ui_t *k1_2_2;

    if (false == kosp_x11_init())
    {
        fprintf(stderr, "kospanel: could not open the X display\n");
        return 1;
    }

    kosp_app_init(argc, argv);

    k1 = kosp_ui_create(KPT_UI, NULL, 10, 20, 100, 200, true);
    k2 = kosp_ui_create(KPT_UI, NULL, 100, 50, 80, 22, true);
    k1_2 = kosp_ui_create(KPT_UI, k1, 86, 23, 55, 65, false);
    k1_2_1 = kosp_ui_create(KPT_UI, k1_2, 77, 88, 99, 100, false);
    k1_2_2 = kosp_ui_create(KPT_UI, k1_2, 22, 33, 44, 55, true);

    k1->destroy(k1);
    k2->destroy(k2);

    kosp_app_shutdown();
    kosp_x11_shutdown();

    kosp_app_destroy();

    return 0;
}

