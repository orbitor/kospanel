/*-------------------------------------------------------------------------*/
/* kosp_app.c
 *
 * This module is part of kospanel
 * Copyright (c) 2013, James DeLisle    <jd@luckygreenfrog.com>
 * See LICENSE for details
 */
/*-------------------------------------------------------------------------*/

#include <stdio.h>
#include <string.h>
#include <X11/Xlib.h>
#include "kosp_base.h"
#include "kosp_list.h"
#include "kosp_ui.h"
#include "kosp_x11.h"
#include "kosp_app.h"

/*-------------------------------------------------------------------------*/
/* typedef for kosp_app */
/*-------------------------------------------------------------------------*/
typedef struct _kosp_app_t kosp_app_t;
struct _kosp_app_t
{
    kosp_list_t        *_ui_event_responders;
    bool                _run_state;
    int                 _xfd;
};

typedef struct _kosp_ui_event_responder_t kosp_ui_event_responder_t;
struct _kosp_ui_event_responder_t
{
    kosp_ui_t  *_responder;
    Window      _window;
};

/*-------------------------------------------------------------------------*/
/* static functions */
/*-------------------------------------------------------------------------*/
static void _kosp_app_process_xevent(XEvent *event);
static kosp_ui_event_responder_t *_kosp_app_find_event_responder(Window window);
static bool _kosp_app_ui_event_responder_find_cb(const void *ptr,
        const void *compare_data);

/*-------------------------------------------------------------------------*/
/* static instance */
/*-------------------------------------------------------------------------*/
static kosp_app_t kosp_app;

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
bool kosp_app_init(int argc, char *argv[])
{
    memset(&kosp_app, 0, sizeof(kosp_app));

    kosp_app._run_state = KS_RS_STARTING_UP;

    kosp_app._ui_event_responders = kosp_list_create(false, true);

    if (NULL != kosp_app._ui_event_responders)
    {
        kosp_list_set_find_callback(kosp_app._ui_event_responders,
                _kosp_app_ui_event_responder_find_cb);
    }

    return true;
}

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
void kosp_app_shutdown(void)
{
    kosp_app._run_state = KS_RS_SHUTTING_DOWN;
}

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
void kosp_app_destroy(void)
{
    printf("%s\n", __func__);

    if (NULL != kosp_app._ui_event_responders)
    {
        kosp_list_destroy(kosp_app._ui_event_responders);
        kosp_app._ui_event_responders = NULL;
    }
}

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
int kosp_app_exec(void)
{
    kosp_app._run_state = KS_RS_RUNNING;

    kosp_app._xfd = XConnectionNumber(kosp_x11_display());

    while(KS_RS_RUNNING == kosp_app._run_state)
    {
        do
        {
            XEvent event;

            while(KS_RS_RUNNING == kosp_app._run_state &&
                    XEventsQueued(kosp_x11_display(), QueuedAlready))
            {
                XNextEvent(kosp_x11_display(), &event);
                _kosp_app_process_xevent(&event);
            }
        } while(KS_RS_RUNNING == kosp_app._run_state &&
                XEventsQueued(kosp_x11_display(), QueuedAfterFlush));

        if (KS_RS_RUNNING != kosp_app._run_state)
        {
            break;
        } 
    }

    return 0;
}

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
int kosp_app_run_state(void)
{
    return kosp_app._run_state;
}

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
void kosp_app_ui_event_responder_add(kosp_ui_t *responder, Window window)
{
    if (NULL != kosp_app._ui_event_responders)
    {
        kosp_list_add(kosp_app._ui_event_responders, responder, false);
    }
}

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
kosp_ui_t *kosp_app_ui_event_responder_remove(kosp_ui_t *responder,
        Window window)
{
    kosp_ui_t *retval = NULL;

    if (NULL != kosp_app._ui_event_responders)
    {
        retval = (kosp_ui_t *) kosp_list_remove(
                kosp_app._ui_event_responders, responder);
    }

    return retval;
}

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
static void _kosp_app_process_xevent(XEvent *event)
{
    kosp_ui_event_responder_t *event_responder = 
        _kosp_app_find_event_responder( event->xany.window);

    if (NULL == event_responder)
    {
        return;
    }
}

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
static kosp_ui_event_responder_t *_kosp_app_find_event_responder(Window window)
{
    kosp_ui_event_responder_t *retval = NULL;

    if (NULL != kosp_app._ui_event_responders)
    {
        retval = (kosp_ui_event_responder_t *) kosp_list_find_by_cb(
                kosp_app._ui_event_responders, &window);
    }

    return retval;
}

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
static bool _kosp_app_ui_event_responder_find_cb(const void *ptr,
        const void *compare_data)
{
    if (kosp_ui_window(ptr) == *((const Window *)compare_data))
    {
        return true;
    }

    return false;
}

