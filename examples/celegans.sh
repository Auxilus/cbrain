#!/bin/bash

cd ..
make -j15
cp libcbrain.so examples
cd examples
gcc cbrain.c -o cbrain -lSDL2 -Wl,-rpath=. -L. -lcbrain
echo
