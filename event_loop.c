#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <assert.h>
#include <unistd.h>
#include "event_loop.h"

void
event_loop_init(event_loop_t *el){

    el->task_array_head = NULL;
    pthread_mutex_init ( &el->ev_loop_mutex, NULL);
    el->ev_loop_state = EV_LOOP_IDLE;
    pthread_cond_init(&el->ev_loop_cv, NULL);
    el->current_task = NULL;
}

static void *
event_loop_thread(void *arg) {

    while(1) {
        sleep(2);
        printf("%s() called\n", __FUNCTION__);
    }
}

void
event_loop_run(event_loop_t *el) {

    pthread_attr_t attr;

    assert(el->thread == NULL);

    el->thread = (pthread_t *)calloc(1, sizeof(pthread_t));

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    pthread_create(el->thread, &attr,
                    event_loop_thread, (void *)el);

}