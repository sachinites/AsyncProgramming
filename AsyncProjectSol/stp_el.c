#include <stdint.h>
#include <stdlib.h>
#include <pthread.h>
#include <memory.h>
#include <time.h>

#include "stp_el.h"

/* Take Event Loop global variable */
event_loop_t el;

/* Import external function */
extern int
stp_update_routing_table(rt_table_t *rt_table, uint32_t cmd_code, rt_table_entry_t *rt_entry) ;

void
stp_init_el(event_loop_t *el) {

    event_loop_init(el);
    event_loop_run(el);
}

static void
el_stp_update_routing_table_cbk(void *arg) {

    el_rt_table_update_data_t *el_rt_table_update_data =
        (el_rt_table_update_data_t *)arg;

    stp_update_routing_table(el_rt_table_update_data->rt_table,
                                              el_rt_table_update_data->cmd_code,
                                              el_rt_table_update_data->rt_entry);

    free( el_rt_table_update_data->rt_entry);
    free( el_rt_table_update_data);
}

task_t *
el_stp_update_routing_table(rt_table_t *rt_table, int cmd_code, rt_table_entry_t *rt_entry) {

    el_rt_table_update_data_t *el_rt_table_update_data = 
        (el_rt_table_update_data_t *) calloc (1, sizeof (el_rt_table_update_data_t));

    el_rt_table_update_data->rt_table = rt_table;
    el_rt_table_update_data->cmd_code = cmd_code;

    el_rt_table_update_data->rt_entry = (rt_table_entry_t *)calloc (1, sizeof(rt_table_entry_t));

    memcpy((char *)el_rt_table_update_data->rt_entry,
                    rt_entry, sizeof(*rt_entry));

    task_t *task = task_create_new_job (&el,
                                                                el_stp_update_routing_table_cbk,
                                                                (void *)el_rt_table_update_data);
    return task;
}