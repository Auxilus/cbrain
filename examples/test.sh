#!/bin/bash

cd ..
make uninstall
make clean
make -j15 install
cd examples
gcc stdp_example.c -o stdp_example -lSDL2 -lcbrain
echo
./stdp_example
