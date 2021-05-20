# WEST - cFS Demo 
[![Docker](https://img.shields.io/badge/Docker-v20.10.5-blue)](https://www.docker.com/)
[![Python](https://img.shields.io/badge/Python-3-brightgreen)](https://www.python.org/)

This flight software (FSW) is intended to function as a proof of concept for cFS as it pertains to WEST. The demo will exercise features within cFS to test its capabilities and famailizarize members with the framework. The ground station is implemented in [Yamcs](https://yamcs.org/) and there are future plans to utilize [OpenMCT](https://nasa.github.io/openmct/).

## Requirements

The application is built in Docker to ensure all dependencies are locked and the bundle is cross-platform. This will require the [Docker Engine](https://docs.docker.com/engine/install/) and [Docker-Compose](https://docs.docker.com/compose/install/). These usually come installed with Docker Desktop.

## Quick Start

We will run cFS locally and use the ground station to showcase uplink and downlink communications. First clone the repository and start the required systems.
```
git clone https://github.com/westernsatelliteteam/cFS-demo.git
cd cFS-demo
cp sample.env .env # defaults will work
docker-compose up dev yamcs
```
Open your browser to [localhost:8090](http://localhost:8090) and you should see the Yamcs ground station. TODO: add yamcs instructions.

Before Yamcs can receive packets, you need to enable telemetry downlink in the cFS TO application. The TO app extends the cFS network over a [UDP port](https://searchnetworking.techtarget.com/definition/UDP-User-Datagram-Protocol). We need to tell the TO app which IP to send telemetry packets to (Yamcs). Docker makes this really easy since Yamcs and cFS are on the same Docker network and can easily reference each other as simply 'yamcs' or 'cfs'. Unfortunately, the TO app has some limitations and can't resolve that hostname into an IP like most tools can. A quick workaround is to use `python3 scripts/get_ip/get_ip.py`, which will print both IPs to the terminal.

Copy the IP for Yamcs and navigate to [the ground station](http://localhost:8090). Click on the active instance and select "Send a command" under "Commanding" on the left panel. Select cFS -> TO_LAB -> TO_LAB_OUTPUT_ENABLE. Paste the previous copied IP and press send. You should see "EVS Port1 66/1/TO_LAB_APP 3: TO telemetry output enabled for IP xxx.xxx.xxx.xxx" in the cFS terminal.

The cFS application should be downlinking telemetry packets. Click on the home tab and you should see a non-zero packet rate for enabled telemetry downlinks. Unfortunately, the spacecraft doesn't know what time it is (I guess it's hard to get a watch in space!) since it has nothing to base its time on. If you click on "Packets" under "Telemetry" in the Yamcs ground station and change the time filter to "No limit" you'll see packets coming through...from 1970?? This is the [Linux Epoch](https://en.wikipedia.org/wiki/Unix_time) and is defined as the default for cFS ([CFE_MISSION_TIME_EPOCH](sample_defs/sample_mission_cfg.h)). So we need to tell the spacecraft what time it is! Use `python3 -m pip install -r scripts/time_sync/requirements.txt` and `python3 scripts/time_sync/time_sync.py` to send a `CFE_TIME_SYNC_TIME` command with the current time. The delay on packets should be a second or less now.

All downlinked CCSDS packets are decoded on the server and all telemetry mapping is defined in TODO: instructions for telemetry mapping. The rate at which packets are downlinked is defined in [SCH's table](apps/sch_lab/fsw/tables/sch_lab_table.c). Note: the current fastest downlink rate is 1Hz.

## Build and Run Locally

```
git clone https://github.com/westernsatelliteteam/cFS-demo.git
cd cFS-demo
cp sample.env .env # defaults will work
docker-compose up dev
```

Should see startup messages, and CFE_ES_Main entering OPERATIONAL state. Use `docker-compose down` to bring down containers.

## Cross Compile for RPI4

```
git clone https://github.com/westernsatelliteteam/cFS-demo.git
cd cFS-demo
docker-compose up rpi4

scp -r build-rpi4/exe/cpu1 <target_user>@<target_ip>:<target_path>
ssh <target_user>@<target_ip>
cd <target_path>
sudo ./core-cpu1
```

Should see startup messages, and CFE_ES_Main entering OPERATIONAL state. Use `docker-compose down` to bring down containers.

## Start Ground Station

```
docker-compose up dev gnd
```

Application should be available at [localhost:8090](http://localhost:8090). TODO: instructions for modifying udp host and ip.