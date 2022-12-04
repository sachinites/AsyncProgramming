
#include <iostream>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include "event_loop.h"

using namespace std;

void
event_loop_init(event_loop_* el)
{
    el->task_arr_head = nullptr;
    pthread_mutex_init(&el->ev_loop_mutex, nullptr);
    el->ev_loop_state = EV_LOOP_IDLE;
    pthread_cond_init(&el->ev_loop_cv, nullptr);
    el->curr_task = nullptr;
}

static void*
event_loop_thread(void* arg)
{
    while (1)
    {
        sleep(2);
        printf("%s() called\n", __FUNCTION__);
    }
}

void
event_loop_run(event_loop_* el)
{
    pthread_attr_t attr;
    assert(el->thread == nullptr);

    el->thread = new pthread_t;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    pthread_create(el->thread, &attr, event_loop_thread, (void*)el);
}
