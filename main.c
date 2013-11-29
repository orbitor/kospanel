
#include "kosp_x11.h"
#include "kosp_ui.h"
#include "kosp_app.h"
#include "kosp_text_view.h"

#include <stdlib.h>
#include <stdio.h>

static const char *this_is_a_test = "This Is A Test";

int main(int argc, char *argv[])
{
    kosp_ui_t *k1;
    kosp_ui_t *k1_2;
    kosp_ui_t *k1_2_1;

    kosp_text_view *tv1;

    if (false == kosp_x11_init())
    {
        fprintf(stderr, "kospanel: could not open the X display\n");
        return 1;
    }

    kosp_app_init(argc, argv);

    k1 = kosp_ui_alloc_init(KPT_UI, NULL, 0, 0, 600, 200, true);
    k1_2 = kosp_ui_alloc_init(KPT_UI, k1, 200, 0, 400, 200, true);
    k1_2_1 = kosp_ui_alloc_init(KPT_UI, k1_2, 5, 5, 49, 100, true);

    tv1 = kosp_text_view_alloc_init(k1_2, 70, 5, 250, 100, this_is_a_test);
    kosp_text_view_font_load(tv1, "9x15");

    k1->show(k1);

    kosp_app_exec();

    k1->destroy(k1);
    kosp_app_shutdown();
    kosp_x11_shutdown();

    kosp_app_destroy();

    return 0;
}

