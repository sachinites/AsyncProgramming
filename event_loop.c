#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <assert.h>
#include <unistd.h>
#include "event_loop.h"

bool static el_debug = true;

static task_t *
event_loop_get_next_task_to_run(event_loop_t *el){

	task_t *task;
	if (!el->task_array_head) return NULL;
    task = el->task_array_head;
	el->task_array_head = task->right;
	if (el->task_array_head) {
		el->task_array_head->left = NULL;
	}
	task->left = NULL;
	task->right = NULL;
	return task;
}

static void
event_loop_add_task_in_task_array(
			event_loop_t *el,
			task_t *new_task) {

	task_t *task, *prev_task;
	
	prev_task = NULL;
	
	task = el->task_array_head;
	while (task) {
		prev_task = task;
		task = task->right;
	}
	if (prev_task) {
		prev_task->right = new_task;
		new_task->left = prev_task;
	}
	else {
		el->task_array_head = new_task;
	}
}

static bool
task_is_present_in_task_array(task_t *task) {

	return !(task->left == NULL && task->right == NULL);
}


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

    task_t *task;
    event_loop_t *el = (event_loop_t *)arg;

    while(1) {
        /* Lock the event Loop Mutex */
        pthread_mutex_lock(&el->ev_loop_mutex);

        while ( (task = event_loop_get_next_task_to_run(el) ) == NULL) {

        /* Event Loop thread do not have any task to fire
           , suspended the event loop thread*/
           if (el_debug) {
               printf ("Task Array Empty, EL thread is suspending\n");
           }
           pthread_cond_wait(&el->ev_loop_cv, &el->ev_loop_mutex);
           /* We are here when event loop thread recvs signal. On receiving the
           signal, go back, fetch the new task from task array
           again and decide if we want to block again or fire the task*/
        }

        /* We are done with the task array, unlock it now, object of interest
        is already detached from task array*/
        pthread_mutex_unlock(&el->ev_loop_mutex);

        if (el_debug) {
            printf("EL Thread woken up, Firing the task\n");
        }

        /* Fire the task now */
        task->cbk(task->arg);
        /* Free the task, we are done with this task*/
        free(task);
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