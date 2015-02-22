#!/bin/sh

./waspp ../log/product0.csv ../cfg/product.json server0 &
./waspp ../log/product1.csv ../cfg/product.json server1 &

ps -ef | grep waspp | grep product

