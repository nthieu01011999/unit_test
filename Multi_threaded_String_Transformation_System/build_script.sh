#!/bin/bash
clear
make clean && make -j4 && make run
./main
