#!/bin/bash
# This script is run within the docker container and manages the cFS application
# Attach to the cFS docker container using docker attach <container name>
# NOTE: You can get the container name with docker ps
# You will now have an interactive shell that can hot reload any new code changes
# Commands:
#   c, compile  == compile code and launch cFS
#   stop        == stop cFS process
#   s, start    == start cFS process (code must be compiled)
#   rs, restart == restart cFS process (code must be compiled)
#   e, exit     == exit application

TLD=/cFS
DEFAULT_ENTRY=/cFS/build/exe/cpu1
ENTRY=$DEFAULT_ENTRY
FILENAME=./core-cpu1
run_wrapper=true
cfs_running=false

cleanup () {
    kill -s SIGTERM $!
    exit 0
}

trap cleanup SIGINT SIGTERM

start() {
    echo "Starting..."
    cd $ENTRY
    ls .
    $FILENAME & script_pid=$!
}

stop() {
    if [ -n "$script_pid" ]; then
        echo "Stopping..."
        kill $script_pid
    fi
}

restart() {
    echo "Restarting..."
    stop
    start
}

compile() {
    stop
    echo "Recompiling"
    rm -rf $TLD/temp $TLD/build
    cp -r $TLD/source $TLD/temp
    cd $TLD/temp && make prep && make && make install
    cp -r $TLD/temp/build $TLD/build
    start
}

sleep 1
echo "Starting cFS wrapper"
compile

while read command < /dev/stdin || run_wrapper; do
    case $command in
        "rs" | "restart")
            restart
            ;;
        "s" | "start")
            start
            ;;
        "stop")
            stop
            ;;
        "c" | "compile")
            compile
            ;;
        "e" | "exit")
            stop
            run=false
            ;;
        *)
            eval $command
            ;;
    esac
done

echo "Ending cFS wrapper"