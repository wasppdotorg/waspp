#!/bin/sh

IP=`/sbin/ifconfig  | grep Bcast | /usr/bin/awk '/inet addr/{print substr($2, 6)}'`

sudo ./waspp develop $IP &

sleep 1
ps -ef | grep waspp | grep develop

