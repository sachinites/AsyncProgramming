#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <assert.h>
#include <unistd.h>
#include "event_loop.h"

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