#!/bin/sh

i=1
while :
do
    ./udp_client.exe 127.0.0.1 40000 100.1.1.${i} 10.10.10.1 eth0 1
    ./udp_client.exe 127.0.0.1 40001 101.1.1.${i} 10.10.10.1 eth0 1
    ./udp_client.exe 127.0.0.1 40002 102.1.1.${i} 10.10.10.1 eth0 1
    i=$((i+1))
    if [ $i -gt 100 ]
    then
        break
    fi
done