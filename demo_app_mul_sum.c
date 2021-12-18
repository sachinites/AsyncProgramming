#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "event_loop.h"

event_loop_t el;
int arr[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

int
sum(int arr[], int n) {

    int i, sum = 0;
    for (i = 0; i < n; i++) {
        sum+=arr[i];
    }
    return sum;
}

int
mul(int arr[], int n) {

    int i, mul = 1;
    for (i = 0; i < n; i++) {
        mul *= arr[i];
    }
    return mul;
}

typedef struct arg_obj_ {

    int *arr;
    int n;
} arg_obj_t;


void
sum_wrapper(void *arg) {

    arg_obj_t *arg_obj = (arg_obj_t *)arg;
    printf ("sum = %d\n", sum(arg_obj->arr, arg_obj->n));
}

void
mul_wrapper(void *arg) {

    arg_obj_t *arg_obj = (arg_obj_t *)arg;
    printf ("mul = %d\n", mul(arg_obj->arr, arg_obj->n));
}


int
main(int argc, char **argv) {

    event_loop_init(&el);
    event_loop_run(&el);
    sleep(1);

    arg_obj_t *arg_obj1 = (arg_obj_t *)calloc(1, sizeof(arg_obj_t));
    arg_obj1->arr = arr;
    arg_obj1->n = sizeof(arr)/sizeof(arr[0]);
    task_t *task_sum = task_create_new_job(&el, sum_wrapper, (void *)arg_obj1);


    arg_obj_t *arg_obj2 = (arg_obj_t *)calloc(1, sizeof(arg_obj_t));
    arg_obj2->arr = arr;
    arg_obj2->n = sizeof(arr)/sizeof(arr[0]);
    task_t *task_mul = task_create_new_job(&el, mul_wrapper, (void *)arg_obj2);


    printf ("End of main\n");
    scanf("\n");
}