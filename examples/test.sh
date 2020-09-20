#!/bin/bash

cd ..
make uninstall
make clean
make -j15 install
cd examples
gcc tem_test.c -o tem_test -lSDL2 -lcbrain
echo
./tem_test
