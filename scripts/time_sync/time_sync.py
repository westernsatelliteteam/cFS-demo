from yamcs.client import YamcsClient
from datetime import datetime

INSTANCE_NAME = 'west'
PROCESSOR_NAME = 'realtime'
YAMCS_SERVER = 'localhost:8090'
TIME_SYNC = 'cFS/TIME/TIME_SET_TIME'
SEND_BUFFER_TIME_MS = 21 # add roughly how long Yamcs takes to send

client = YamcsClient(YAMCS_SERVER)

# most is copied from the Yamcs Python API docs
processor = client.get_processor(instance=INSTANCE_NAME, processor=PROCESSOR_NAME)
date = client.get_time(instance=INSTANCE_NAME)

timestamp = date.timestamp() + round(SEND_BUFFER_TIME_MS/1000)

# format the command payload, takes a "seconds" and "microseconds" parameter
# this is just a quick way to get the integer and fractional parts of timestamp
time = { 'seconds': int(timestamp), 'microseconds': round((timestamp-int(timestamp))*1000) }

processor.issue_command(TIME_SYNC, time)
print("Set onboard time to " + str(timestamp) + "ms (" + str(date) + ") past epoch.")