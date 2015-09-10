#!/bin/sh

sudo ./waspp develop 00 &
sudo ./waspp develop 01 &

sleep 1
ps -ef | grep waspp | grep develop

