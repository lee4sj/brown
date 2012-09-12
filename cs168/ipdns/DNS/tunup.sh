#!/bin/bash
if [ $# -eq 4 ]; then
   ip tuntap add dev $1 mode tun user $2
   ip link set $1 up
   ip link set $1 mtu 68
   ip addr add $3 peer $4 dev $1
   ifconfig $1
else
   echo "usage: tunup <devicename> <user> <clientip> <serverip>"
fi
