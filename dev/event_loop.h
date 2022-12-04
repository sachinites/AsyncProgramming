#ifndef __EV_LOOP__
#define __EV_LOOP__

typedef void (*event_callback)(void*);

struct task_ 
{
    event_callback ev_cbk;
    void* arg;
    struct task_ *left, *right;
};

typedef enum
{
    EV_LOOP_IDLE,
    EV_LOOP_BUSY
} EV_LOOP_STATE;

struct event_loop_
{
    struct task* task_arr_head;
    pthread_mutex_t ev_loop_mutex;
    EV_LOOP_STATE ev_loop_state;
    pthread_cond_t ev_loop_cv;
    pthread_t* thread;
    struct task_* curr_task;
};

void
event_loop_init(event_loop_* el);

void
event_loop_run(event_loop_* el);

#endif /* __EV_LOOP__ */