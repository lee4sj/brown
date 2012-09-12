#!/bin/bash
if [ $# -eq 1 ]; then
  ip tuntap del dev $1 mode tun
else
   echo "usage: tundown <devicename>"
fi
