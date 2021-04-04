# WEST - cFS Demo 
[![Docker](https://img.shields.io/badge/Docker-v20.10.5-blue)](https://www.docker.com/)

This flight software (FSW) is intended to function as a proof of concept for cFS as it pertains to WEST. The demo will exercise features within cFS to test its capabilities and famailizarize members with the framework.

## Requirements

The application is built in Docker to ensure all dependencies are locked. This will require the [Docker Engine](https://docs.docker.com/engine/install/) and [Docker-Compose](https://docs.docker.com/compose/install/). These usually come installed with Docker Desktop.

## Quick Start

We will run cFS locally and use the ground station to showcase uplink and downlink communications. First clone the repository and start the required systems.
```
git clone https://github.com/westernsatelliteteam/cFS-demo.git
cd cFS-demo
docker-compose up dev gnd
```
Now open your browser to [localhost:5000](http://localhost:5000) and you should see a barebones ground station. Each application has its own section, starting with the five cFE core applications: Executive Services (ES), Event Services (EVS), Software Bus (SB) Services, Table (TBL)Services, and Time (TIME) Services. There is also the Command Ingest (CI), Telemetry Output (TO), Scheduler (SCH), and Sample App.

Before the ground station can receive any data, we must uplink a command to commence communication. Under TO, enter 'gnd' into the text field and press the 'Enable Tlm' button. 'gnd' is the internal name that Docker uses to reference the ground station. This gets converted to an equivalent IP and uplinked to the satellite, you should see this in the output in the bottom right (any SB messages from TO will show up here). You can play around with a couple of the NO-OP (No Operation) command buttons and you'll see data show up in the output as well. A quick note that the ip can be switched from 'gnd' to any ip to allow the ground station to communicate with any device on the network (still testing), instead of just the docker network.

All cFE applications and Sample App downlink telemetry by default. You can open each app's telemetry page with their respective 'Open Telemetry" buttons. This will open a new tab for the app's telemetry. Try pressing a NO-OP command for a particular application and watch the command counter increment on the telemetry page.

All downlinked CCSDS packets are decoded on [the server](ground-station/server.js) and all telemetry mapping is defined in [telemetry_defs.js](ground-station/public/js/telemetry_defs.js). The rate at which packets are downlinked is defined in [SCH's table](apps/sch_lab/fsw/tables/sch_lab_table.c).

## Build and Run Locally

```
git clone https://github.com/westernsatelliteteam/cFS-demo.git
cd cFS-demo
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

Application should be available at [localhost:5000](http://localhost:5000)