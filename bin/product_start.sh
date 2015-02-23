#!/bin/sh

./waspp product 00 &
./waspp product 01 &

ps -ef | grep waspp | grep product

