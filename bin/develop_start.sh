#!/bin/sh

sudo ./waspp develop 00 &
sudo ./waspp develop 01 &

ps -ef | grep waspp | grep develop

