#!/bin/sh

./waspp develop 00 &
./waspp develop 01 &

ps -ef | grep waspp | grep develop

