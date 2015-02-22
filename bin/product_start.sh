#!/bin/sh

./waspp ../log/product.csv ../cfg/product.json server0 &
./waspp ../log/product.csv ../cfg/product.json server1 &

ps -ef | grep waspp | grep product

