#!/bin/bash

cd ..
make uninstall
make clean
make -j15 install
cd examples
gcc cbrain.c -o cbrain -lSDL2 -lcbrain
./cbrain
