#ifndef __STP_EL__
#define __STP_EL__

#include "../event_loop.h"
#include "rt.h"

/* Data Structure to pack all arguments as one single argument */

typedef struct el_rt_table_update_data_ {

    rt_table_t *rt_table;
    int cmd_code;
    rt_table_entry_t *rt_entry;
}  el_rt_table_update_data_t;

void
stp_init_el(event_loop_t *el);

/* Now create an API to update routing table using Event Loop */
task_t *
el_stp_update_routing_table(rt_table_t *rt, int cmd_code, rt_table_entry_t *rt_entry);

#endif