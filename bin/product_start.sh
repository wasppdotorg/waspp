#!/bin/sh

IP=`/sbin/ifconfig eth0 | /usr/bin/awk '/inet addr/{print substr($2, 6)}'`

sudo ./waspp product $IP &

sleep 1
ps -ef | grep waspp | grep product

