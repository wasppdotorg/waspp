#!/bin/sh

./waspp ../log/testing0.csv ../cfg/testing.json server0 &
./waspp ../log/testing1.csv ../cfg/testing.json server1 &

ps -ef | grep waspp | grep testing

