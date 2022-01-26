#include <stdio.h>
#include <time.h>
#include <stdint.h>
#include "rt.h"
#include "network_utils.h"

int
main (int argc, char **argv) {

    if (argc != 7) {
        printf ("Insufficient Argument\n");
        return 0;
    }

    char *dest_ip = argv[1];
    uint32_t dest_port_no = atoi(argv[2]);

    rt_table_entry_t rt_table_entry;

    memcpy (rt_table_entry.dest, argv[3], sizeof(rt_table_entry.dest));
    memcpy (rt_table_entry.oif,   argv[4], sizeof(rt_table_entry.oif));
    memcpy (rt_table_entry.gw,   argv[5], sizeof(rt_table_entry.gw));
    int msg_size = sizeof(uint32_t) + sizeof(rt_table_entry_t);
    uint32_t *msg_to_send = (uint32_t *)calloc (1 , msg_size);
    *msg_to_send = atoi(argv[6]);
    memcpy( (char *)(msg_to_send + 1) , (char *)&rt_table_entry, sizeof(rt_table_entry_t));
    send_udp_msg(dest_ip, dest_port_no, (char *) msg_to_send , msg_size, -1);
    free(msg_to_send);
    return 0;
}
