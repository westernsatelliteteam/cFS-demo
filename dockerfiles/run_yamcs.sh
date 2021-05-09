#!/bin/bash

YAMCS_WEST_FILE='/yamcs/env.yamcs.west.yaml'
COPY_LOCATION='/yamcs/src/main/yamcs/etc/yamcs.west.yaml'

WARNING_MESSAGE=' \
######################### WARNING ########################## \
# DO NOT MODIFY THIS FILE AS IT IS AUTO GENERATED          # \
# INSTEAD MODIFY THE SAME NAMED FILE (PREFIXED WITH env.)  # \
# IN THE YAMCS FOLDER                                      # \
######################### WARNING ########################## \
'

echo "Inserting env => cFS IP: $CFS_IP, TO port: $TO_PORT, CI port: $CI_PORT"

# copy file to target location
cp $YAMCS_WEST_FILE $COPY_LOCATION

# copy warning message
sed -i "1s/^/$WARNING_MESSAGE\n\n/" $COPY_LOCATION

# copy environment variables
# TODO: make this a for loop on each line of the .env file
sed -i "s/\$CFS_IP/$CFS_IP/g" $COPY_LOCATION
sed -i "s/\$TO_PORT/$TO_PORT/g" $COPY_LOCATION
sed -i "s/\$CI_PORT/$CI_PORT/g" $COPY_LOCATION

mvn yamcs:run