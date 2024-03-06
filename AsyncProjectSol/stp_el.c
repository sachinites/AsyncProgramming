#include <stdint.h>
#include <stdlib.h>
#include <pthread.h>
#include <memory.h>
#include <time.h>
#include <stdio.h>
#include "utils.h"
#include "network_utils.h"
#include "../timerlib.h"
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

static EL_RES_T
el_stp_update_routing_table_cbk(void *arg) {

    el_rt_table_update_data_t *el_rt_table_update_data =
        (el_rt_table_update_data_t *)arg;

    stp_update_routing_table(el_rt_table_update_data->rt_table,
                                              el_rt_table_update_data->cmd_code,
                                              el_rt_table_update_data->rt_entry);

    free( el_rt_table_update_data->rt_entry);
    free( el_rt_table_update_data);
    return EL_FINISH;
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
                                                                (void *)el_rt_table_update_data, TASK_PRIORITY_MEDIUM);
    return task;
}

typedef struct rt_table_print_cntxt_ {

    int i;
    rt_table_entry_t *rt_table_entry;

} rt_table_print_cntxt_t;


static EL_RES_T
rt_display_rt_table_preemption_conext_save_cbk(void *arg) {

    time_t curr_time = time(NULL);
    unsigned int uptime_in_seconds = 0;
    rt_table_print_cntxt_t *cntxt = (rt_table_print_cntxt_t *)arg;

    rt_table_entry_t *rt_table_entry = cntxt->rt_table_entry;
    int i = cntxt->i;

    for( ; rt_table_entry;
        rt_table_entry = rt_table_entry->next){

        uptime_in_seconds = (unsigned int)difftime(
            curr_time, rt_table_entry->last_updated_time);

        printf("%d. %-18s %-4d %-18s %-18s", i,
            rt_table_entry->dest,
            rt_table_entry->mask,
            rt_table_entry->gw,
            rt_table_entry->oif);
        
        printf("Last updated : %s  ", hrs_min_sec_format(uptime_in_seconds)),
        printf ("Exp time : %lu\n",
        rt_table_entry->exp_timer ? \
            timer_get_time_remaining_in_mill_sec (rt_table_entry->exp_timer) : 0);
        i++;

        /* Save the context*/
        if (i % 10 == 0 && rt_table_entry->next) {

            cntxt->rt_table_entry = rt_table_entry->next;
            cntxt->i = i++;
            return EL_CONTINUE;
        }
    }

    free(cntxt);
    return EL_FINISH;
}


void
rt_display_rt_table_preemption_conext_save (rt_table_t *rt){

     rt_table_entry_t *rt_table_entry = rt->head;

     if (!rt_table_entry) return;

    printf ("# count = %u\n", rt->count);

    rt_table_print_cntxt_t *cntxt = 
        (rt_table_print_cntxt_t *)calloc (1, sizeof (rt_table_print_cntxt_t));

    cntxt->i = 1;
    cntxt->rt_table_entry = rt_table_entry;

     task_create_new_job (&el, 
                                         rt_display_rt_table_preemption_conext_save_cbk, 
                                         (void *) cntxt, TASK_PRIORITY_HIGH);
}


static EL_RES_T
rt_entry_serlialize_and_send_task_cbk (void *arg) {

    rt_table_entry_t *rt_entry = (rt_table_entry_t *)arg;

    /* look up RT entry in RT table*/
    rt_table_entry_t *actual_rt_entry = rt_look_up_rt_table_entry (
                                                            rt_entry->rt_table,
                                                            rt_entry->dest,
                                                            rt_entry->mask);

    if (!actual_rt_entry) {

        printf ("Serialize Task : RT Entry do not exist\n");
        free(rt_entry);
        return EL_FINISH;
    }

    int udp_sockfd = socket (AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    
    if (udp_sockfd < 0) {
        printf ("Error : Failed to create UDP socket\n");
        free(rt_entry);
        return EL_FINISH;
    }

    int msg_size = sizeof(uint32_t) + sizeof(rt_table_entry_t);
    uint32_t *msg_to_send = (uint32_t *)calloc (1 , msg_size);
    *msg_to_send = ROUTE_CREATE;
    memcpy( (char *)(msg_to_send + 1) , (char *)actual_rt_entry, sizeof(rt_table_entry_t));

    send_udp_msg("127.0.0.1",
			 50000,
			 (char *)msg_to_send, msg_size,
			 udp_sockfd);

    close (udp_sockfd);
    free(msg_to_send);
    free(rt_entry);
    return EL_FINISH;
}

void
el_stp_serialize_and_send_rt_entry (rt_table_t *rt_table, rt_table_entry_t *rt_entry_tmplate) {

    /* This new rt_entry serves the purpose of context-save structure for this task*/
    rt_table_entry_t *rt_entry = (rt_table_entry_t *)calloc (1, sizeof (rt_table_entry_t));
    strncpy (rt_entry->dest, rt_entry_tmplate->dest, 16);
    rt_entry->mask = 32;
    rt_entry->rt_table = rt_table;

    task_create_new_job (&el, 
                                        rt_entry_serlialize_and_send_task_cbk,
                                        (void *)rt_entry, TASK_PRIORITY_MEDIUM);
}