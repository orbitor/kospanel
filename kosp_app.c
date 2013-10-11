/*-------------------------------------------------------------------------*/
/* kosp_app.c
 *
 * This module is part of kospanel
 * Copyright (c) 2013, James DeLisle    <jd@luckygreenfrog.com>
 * See LICENSE for details
 */
/*-------------------------------------------------------------------------*/

#include <string.h>
#include <X11/Xlib.h>
#include "kosp_base.h"
#include "kosp_list.h"
#include "kosp_x11.h"
#include "kosp_app.h"

/*-------------------------------------------------------------------------*/
/* virtual functions */
/*-------------------------------------------------------------------------*/
static void kosp_app_destroy(void *vself);

/*-------------------------------------------------------------------------*/
/* static functions */
/*-------------------------------------------------------------------------*/
static void kosp_app_process_xevent(XEvent *event);
static kosp_ui_t *kosp_app_find_event_responder(Window xwindow);

static bool kosp_app_ui_event_responders_find(const void *ptr,
        const void *compare_data);

/*-------------------------------------------------------------------------*/
/* typedef for kosp_app */
/*-------------------------------------------------------------------------*/
typedef struct _kosp_app_t kosp_app_t;
struct _kosp_app_t
{
    KOSP_BASE_MEMBERS_DECLARE
    kosp_list_t        *_ui_event_responders;
    bool                _run_state;
    int                 _xfd;
};

/*-------------------------------------------------------------------------*/
/* static instance */
/*-------------------------------------------------------------------------*/
static kosp_app_t kosp_app;

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
bool kosp_app_init(int argc, char *argv[])
{
    memset(&kosp_app, 0, sizeof(kosp_app));
    kosp_base_init((kosp_base_t *) &kosp_app, KPT_APP);
    kosp_app.destroy = kosp_app_destroy;

    kosp_app._run_state = KS_RS_STARTING_UP;

    kosp_app._ui_event_responders = kosp_list_create(false, false);

    if (kosp_app._ui_event_responders)
    {
        kosp_list_set_find_callback(kosp_app._ui_event_responders,
                kosp_app_ui_event_responders_find);
    }
}

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
void kosp_app_shutdown(void)
{
    kosp_app._run_state = KS_RS_SHUTTING_DOWN;
}

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
int kosp_app_exec(void)
{
    kosp_app._run_state = KS_RS_RUNNING;

    kosp_app._xfd = XConnectionNumber(kosp_x11_display());

    while(KS_RS_RUNNING == kosp_app._run_state)
    {
    }

    return 0;
}

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
static void kosp_app_destroy(void *vself)
{
    if (NULL != kosp_app._ui_event_responders)
    {
        kosp_app._ui_event_responders->destroy(kosp_app._ui_event_responders);
        kosp_app._ui_event_responders = NULL;
    }

    kosp_base_destroy(vself);
}

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
int kosp_app_run_state(void)
{
    return kosp_app._run_state;
}

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
void kosp_app_ui_event_responder_add(kosp_ui_t *responder)
{
    if (NULL != kosp_app._ui_event_responders)
    {
    }
}

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
kosp_ui_t *kosp_app_ui_event_responder_remove(kosp_ui_t *responder)
{
    kosp_ui_t *retval = NULL;

    if (NULL != kosp_app._ui_event_responders)
    {
    }

    return retval;
}

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
static void kosp_app_process_xevent(XEvent *event)
{
    kosp_ui_t *event_responder = kosp_app_find_event_responder(
            event->xany.window);

    if (NULL == event_responder)
    {
        return;
    }
}

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
static kosp_ui_t *kosp_app_find_event_responder(Window xwindow)
{
    kosp_ui_t *retval = NULL;

    if (NULL != kosp_app._ui_event_responders)
    {
    }

    return retval;
}

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
static bool kosp_app_ui_event_responders_find(const void *ptr,
        const void *compare_data)
{
    if (kosp_ui_window(ptr) == (Window) *compare_data)
    {
        return true;
    }

    return false;
}

