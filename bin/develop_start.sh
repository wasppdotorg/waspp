#!/bin/sh

./waspp ../log/develop.csv ../cfg/develop.json server0 &
./waspp ../log/develop.csv ../cfg/develop.json server1 &

ps -ef | grep waspp | grep develop

