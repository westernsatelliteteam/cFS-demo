#!/bin/bash

make prep
make
make install

if [ "$O" == "build-dev" ]; then
    cd $O/exe/cpu1
    sudo ./core-cpu1
fi