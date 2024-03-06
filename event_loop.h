/*
 * =====================================================================================
 *
 *       Filename:  event_loop.h
 *
 *    Description: This file declares the structures defining the event loop 
 *
 *        Version:  1.0
 *        Created:  12/18/2021 03:37:42 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  ABHISHEK SAGAR (), sachinites@gmail.com
 *   Organization:  Cisco Systems
 *
 * =====================================================================================
 */

#ifndef __EV_LOOP__
#define __EV_LOOP__

typedef struct task_ task_t;
typedef struct event_loop_ event_loop_t;

typedef enum EL_RES_ {

    EL_CONTINUE,
    EL_FINISH

} EL_RES_T;

typedef enum TASK_PRIORITY_ {

    TASK_PRIORITY_HIGH,
    TASK_PRIORITY_MEDIUM,
    TASK_PRIORITY_LOW,
    TASK_PRIORITY_MAX

} TASK_PRIORITY_T;

typedef EL_RES_T (*event_cbk)(void *);

struct task_ {

    event_cbk cbk;
    void *arg;
    struct task_ *left, *right;
    TASK_PRIORITY_T priority;
};

typedef enum {

    EV_LOOP_IDLE,
    EV_LOOP_BUSY
} EV_LOOP_STATE;

struct event_loop_{

    /* head to the start of the task array */
    struct task_ *task_array_head[TASK_PRIORITY_MAX];
    /* Mutex to enforce Mutual exclusion enqueue/Deque
     * Operation in task array. Also used to update event loop
     * attributes in mutual exclusive way
     */
    pthread_mutex_t ev_loop_mutex;
    /* State of event loop */
    EV_LOOP_STATE ev_loop_state;
    /* CV to suspened event loop thread */
    pthread_cond_t ev_loop_cv;
    /*  Event loop thread */
    pthread_t *thread;
    /* Current task which event loop thread is executing.
     * NULL if event loop is resting in peace*/
    struct task_ *current_task;
};

void
event_loop_init(event_loop_t *el);

void
event_loop_run(event_loop_t *el);

task_t *
task_create_new_job(event_loop_t *el, event_cbk cbk, void *arg, TASK_PRIORITY_T priority);

void
task_cancel_job(event_loop_t *el, task_t *task);

#endif /* __EV_LOOP__ */
