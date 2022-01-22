#include <stdio.h>
#include <time.h>
#include "rt.h"
#include "network_utils.h"

int
main (int argc, char **argv) {

    if (argc != 6) {
        printf ("Insufficient Argument\n");
        return 0;
    }

    char *dest_ip = argv[1];
    uint32_t dest_port_no = atoi(argv[2]);

    rt_table_entry_t rt_table_entry;

    memcpy (rt_table_entry.dest, argv[3], sizeof(rt_table_entry.dest));
    memcpy (rt_table_entry.oif,   argv[4], sizeof(rt_table_entry.oif));
    memcpy (rt_table_entry.gw,   argv[5], sizeof(rt_table_entry.gw));
    
    send_udp_msg(dest_ip, dest_port_no, (char *)&rt_table_entry, sizeof(rt_table_entry), -1);
    return 0;
}
