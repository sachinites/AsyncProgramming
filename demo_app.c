#include <stdio.h>
#include <stdlib.h>
#include "event_loop.h"

event_loop_t el;

int
main(int argc, char **argv) {

    event_loop_init(&el);
    event_loop_run(&el);

    scanf("\n");
}