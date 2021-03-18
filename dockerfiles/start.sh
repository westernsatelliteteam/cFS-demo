#!/bin/bash

if [ "$O" == "build-dev" ]; then
    make prep && make && make install && cd $O/exe/cpu1 && sudo ./core-cpu1
else
    make prep && make && make install
fi