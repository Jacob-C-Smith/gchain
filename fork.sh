#!/bin/bash

# print ifconfig info
/sbin/ifconfig eth0 | grep 'inet addr' | cut -d: -f2 | awk '{print $1}'

# start
echo "[docker] Starting gchain node on $(hostname)"

# start gchain
make clean; make
./gchain_node --port 5107 2>&1

# stop
echo "[docker] gchain node stopped"