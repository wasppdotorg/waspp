#!/bin/sh

./waspp ../log/testing.csv ../cfg/testing.json server0 &
./waspp ../log/testing.csv ../cfg/testing.json server1 &

ps -ef | grep waspp | grep testing

