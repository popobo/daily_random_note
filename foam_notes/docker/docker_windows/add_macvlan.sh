#!/bin/bash

ip link set eno1 promisc on
ip link add link eno1 macvlan0-host type macvlan mode bridge
ip link set dev macvlan0-host up
ip route add 192.168.40.20 dev macvlan0-host
