#!/bin/sh

IP=`/sbin/ifconfig eth0 | /usr/bin/awk '/inet addr/{print substr($2, 6)}'`

sudo ./waspp testing $IP &

sleep 1
ps -ef | grep waspp | grep testing

