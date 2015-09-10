#!/bin/sh

sudo ./waspp testing 00 &
sudo ./waspp testing 01 &

sleep 1
ps -ef | grep waspp | grep testing

