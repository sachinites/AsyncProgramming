gcc -g -c network_utils.c -o network_utils.o
gcc -g -c rtm.c -o rtm.o
gcc -g -c utils.c -o utils.o
gcc -g -c udp_client.c -o udp_client.o
gcc -g -c rt.c -o rt.o
gcc -g rtm.o utils.o rt.o network_utils.o -o exe -lpthread
gcc -g udp_client.o network_utils.o utils.o rt.o -o udp_client.exe -lpthread
