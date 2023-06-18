rm -f *o
rm -f *exe
gcc -g -c network_utils.c -o network_utils.o
gcc -g -c stp.c -o stp.o
gcc -g -c utils.c -o utils.o
gcc -g -c udp_client.c -o udp_client.o
gcc -g -c rt.c -o rt.o
gcc -g stp.o utils.o rt.o network_utils.o -o stp.exe -lpthread -lrt
gcc -g udp_client.o network_utils.o utils.o rt.o -o udp_client.exe -lpthread -lrt
