#!/bin/sh

IP=`/sbin/ifconfig | /bin/grep Bcast | /usr/bin/awk '/inet addr/{print substr($2,6)}'`

sudo ./waspp testing $IP &

sleep 1
ps -ef | grep waspp | grep testing

