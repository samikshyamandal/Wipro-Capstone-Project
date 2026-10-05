#!/bin/bash

echo "Compiling Kernel Module..."
make

echo "Compiling Dashboard..."
g++ dashboard.cpp -o dashboard -pthread

echo "Loading Module..."
sudo insmod conveyor_driver.ko

MAJOR=$(awk '$2=="conveyor_motor" {print $1}' /proc/devices)
echo "Device registered with Major Number: $MAJOR"

sudo rm -f /dev/conveyor_motor
sudo mknod /dev/conveyor_motor c $MAJOR 0
sudo chmod 666 /dev/conveyor_motor

echo "Starting Application..."
./dashboard
