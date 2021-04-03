#!/bin/bash

if [ "$O" == "build-dev" ]; then
    make prep && make && make install && cd $O/exe/cpu1 && echo "Installed to $O" && ./core-cpu1
else
    make prep && make && make install && echo "Installed to $O"
fi