#!/bin/sh

./waspp testing 00 &
./waspp testing 01 &

ps -ef | grep waspp | grep testing

