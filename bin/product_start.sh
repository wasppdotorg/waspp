#!/bin/sh

sudo ./waspp product 00 &
sudo ./waspp product 01 &

ps -ef | grep waspp | grep product

