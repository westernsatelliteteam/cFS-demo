# WEST - cFS Demo 

This flight software (FSW) is intended to function as a proof of concept for cFS as it pertains to WEST. The demo will exercise features within cFS to test its performance and capabilities.

## Setup

Ensure the following software are installed: Make, CMake, GCC, and Git.  To setup the cFS BUNDLE directly from the latest set of interoperable repositories:

    git clone https://github.com/westernsatelliteteam/cFS-demo.git
    cd cFS-demo
    git submodule init
    git submodule update

## Build and Run

To prep, compile, and run on the host (from cFS directory above) as a normal user (best effort message queue depth and task priorities):

    make SIMULATION=native prep
    make
    make install
    cd build/exe/cpu1/
    ./core-cpu1

Should see startup messages, and CFE_ES_Main entering OPERATIONAL state.  Note the code must be executed from the build/exe/cpu1 directory to find the startup script and shared objects.

Note: The steps above are for a debug, permissive mode build and includes deprecated elements.  For a release build, recommendation is `make BUILDTYPE=release OMIT_DEPRECATED=true prep`.  Unit tests can be added with `ENABLE_UNIT_TESTS=true`, run with `make test`, and coverage reported with `make lcov`.

## Send commands, receive telemetry

The cFS-GroundSystem tool can be used to send commands and receive telemetry.  For details on using and setting up the Ground System, see the [Guide-GroundSystem](https://github.com/nasa/cFS-GroundSystem/blob/main/Guide-GroundSystem.md).  Note it depends on PyQt5 and PyZMQ:

1. Install PyQt5 and PyZMQ on your system.  Some systems may also require installing libcanberra-gtk-module.
       
2. Compile cmdUtil and start the ground system executable

       cd tools/cFS-GroundSystem/Subsystems/cmdUtil
       make
       cd ../..
       python3 GroundSystem.py

3. Select "Start Command System"
4. Select "Enable Tlm"
5. Enter IP address of system executing cFS, 127.0.0.1 if running locally

Should see telemetry, can send noops and see command counters increment.
