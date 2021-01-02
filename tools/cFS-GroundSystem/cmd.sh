#!/bin/bash
# This script is run within the docker container and manages the ground station application
# Attach to the ground station docker container using docker attach <container name>
# NOTE: You can get the container name with docker ps
# You will now have an interactive shell that can interface the cmdUtil app
# Commands:
#   t, test == Send a test NOOP command to event services TODO: update the packet
#   *       == Sends a command according to the cmdUtil documentation (see below for available flags)
#   e, exit == exit the application
# 
# Available flags:
#   --pktid : The Packet ID for the command being sent
#   --cmdcode : The command code for the command being sent
#   --endian : BE: Dont swap words, LE: Swap words on packet
#   --half : Add a 16 bit parameter to the packet ( hex or dec )
#   --long : Add a 32 bit parameter to the packet ( hex or dec )
#   --string : Add a fixed length string to the packet
#       The string parameter has the form "NNN:StringData"
#       Where NNN is the length of the parameter in the
#       command, and StringData is the string. So you could put
#       --string="32:ES_APP" and it will build the string
#       parameter and pad the rest of the 32 bytes with 0's.
#   --byte : Add an 8 bit parameter to the packet ( hex or dec )
#       This one is not implemented yet, because I will have to
#       pad the other 8 bits..

run_wrapper=true

cleanup () {
    kill -s SIGTERM $!
    exit 0
}

trap cleanup SIGINT SIGTERM

sleep 1
echo "Starting Ground Station wrapper"

while read command < /dev/stdin; do
    case $command in
        "e" | "exit")
            stop
            run=false
            ;;
        "t" | "test")
            ./Subsystems/cmdUtil/cmdUtil --host=cfs --port=1234 --pktid=0x1803 --cmdcode=3 --half=100 --string="16:ES_APP"
            ;;
        *)
            # ./cmdUtil --host=cfs --port=1234 $command
            ./Subsystems/cmdUtil/cmdUtil --host=cfs --port=1234 $command
            ;;
    esac
done

echo "Ending Ground Station wrapper"