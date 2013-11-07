/*-------------------------------------------------------------------------*/
/* kosp_app.c
 *
 * This module is part of kospanel
 * Copyright (c) 2013, James DeLisle    <jd@luckygreenfrog.com>
 * See LICENSE for details
 */
/*-------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
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
typedef struct _kosp_ui_event_responder_t kosp_ui_event_responder_t;
struct _kosp_ui_event_responder_t
{
    kosp_ui_t  *_responder;
    Window      _window;
};

typedef struct _kosp_app_t kosp_app_t;
struct _kosp_app_t
{
    kosp_list_t                *_ui_event_responders;
    kosp_ui_event_responder_t  *_ui_event_responder_cache;
    bool                        _run_state;
    int                         _xfd;
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
    if (NULL == responder ||
            None == window)
    {
        return;
    }

    kosp_ui_event_responder_t *uie = (kosp_ui_event_responder_t *) malloc(
            sizeof(kosp_ui_event_responder_t));

    if (NULL != uie)
    {
        uie->_responder = responder;
        uie->_window = window;
        kosp_list_add(kosp_app._ui_event_responders, uie, false);
    }
}

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
kosp_ui_t *kosp_app_ui_event_responder_remove(kosp_ui_t *responder,
        Window window)
{
    kosp_ui_event_responder_t *uie = NULL;

    if (NULL == responder || None == window)
    {
        return NULL;
    }

    uie = _kosp_app_find_event_responder(window);

    if (NULL != uie)
    {
        uie = (kosp_ui_event_responder_t *) kosp_list_remove(
                kosp_app._ui_event_responders, uie);
    }

    return uie->_responder;
}

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
static void _kosp_app_process_xevent(XEvent *event)
{
    kosp_ui_event_responder_t *event_responder = 
        _kosp_app_find_event_responder(event->xany.window);

    if (NULL == event_responder)
    {
        printf("%s\tno event responder for window %d\n",
                __func__,
                (int) event->xany.window);
        return;
    }

    printf("%s\tevent responder found for window %d\n",
            __func__,
            (int) event->xany.window);

    switch(event->type)
    {
    case Expose:
        event_responder->_responder->draw(
                event_responder->_responder);
        break;

    case ConfigureNotify:
        event_responder->_responder->configure_notify(
                event_responder->_responder,
                (XConfigureEvent *) &event->xconfigure);
        break;

    case MotionNotify:
        break;

    case EnterNotify:
        event_responder->_responder->enter_window(
                event_responder->_responder,
                (XEnterWindowEvent *) &event->xcrossing);
        break;

    case LeaveNotify:
        event_responder->_responder->leave_window(
                event_responder->_responder,
                (XLeaveWindowEvent *) &event->xcrossing);
        break;

    case ButtonPress:
        event_responder->_responder->button_press(
                event_responder->_responder,
                (XButtonPressedEvent *) &event->xbutton);
        break;

    case ButtonRelease:
        break;

    default:
        break;
    }
}

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
static kosp_ui_event_responder_t *_kosp_app_find_event_responder(Window window)
{
    kosp_ui_event_responder_t *uie = NULL;

    if (NULL != kosp_app._ui_event_responder_cache &&
            kosp_app._ui_event_responder_cache->_window == window)
    {
        printf("%s\tevent responder is cache\n",
                __func__);

        uie = kosp_app._ui_event_responder_cache;
    }
    else
    {
        uie = (kosp_ui_event_responder_t *) kosp_list_find_by_cb(
                kosp_app._ui_event_responders, &window);

        if (NULL != uie)
        {
            printf("%s\tupdating event responder cache\n",
                    __func__);

            kosp_app._ui_event_responder_cache = uie;
        }
    }

    return uie;
}

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
static bool _kosp_app_ui_event_responder_find_cb(const void *ptr,
        const void *compare_data)
{
    kosp_ui_event_responder_t *uie = (kosp_ui_event_responder_t *) ptr;

    if (kosp_ui_window(uie->_responder) == *((const Window *)compare_data))
    {
        return true;
    }

    return false;
}

