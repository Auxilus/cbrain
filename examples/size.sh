#!/bin/bash

cd ../
make uninstall
make clean
make -j15 install
echo
ls -alh libcbrain.so
cd examples
gcc cbrain.c -o cbrain -lSDL2 -lcbrain
strip -S --strip-unneeded --remove-section=.note.gnu.gold-version --remove-section=.coment --remove-section=.note --remove-section=.note.gnu.build-id --remove-section=.note.ABI-tag cbrain
ls -alh cbrain
echo
rm cbrain
cd ..
make uninstall
make clean
