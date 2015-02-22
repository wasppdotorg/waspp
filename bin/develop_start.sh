#!/bin/sh

./waspp ../log/develop0.csv ../cfg/develop.json server0 &
./waspp ../log/develop1.csv ../cfg/develop.json server1 &

ps -ef | grep waspp | grep develop

