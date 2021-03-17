# WEST - cFS Demo 

This flight software (FSW) is intended to function as a proof of concept for cFS as it pertains to WEST. The demo will exercise features within cFS to test its capabilities and famailizarize members with the framework.

## Requirements

The application is built in Docker to ensure all dependencies are locked. This will require the [Docker Engine](https://docs.docker.com/engine/install/) and [Docker-Compose](https://docs.docker.com/compose/install/). These usually come installed with Docker Desktop.

## Build and Run Locally

```
git clone https://github.com/westernsatelliteteam/cFS-demo.git
cd cFS-demo
docker-compose run dev
```

Should see startup messages, and CFE_ES_Main entering OPERATIONAL state. Use `docker-compose down` to bring down containers.

## Cross Compile for RPI4

```
git clone https://github.com/westernsatelliteteam/cFS-demo.git
cd cFS-demo
docker-compose run rpi4
scp -r build-rpi4/exe/cpu1 <target_user>@<target_ip>:<target_path>
ssh <target_user>@<target_ip>
cd <target_path>
sudo ./core-cpu1
```

Should see startup messages, and CFE_ES_Main entering OPERATIONAL state. Use `docker-compose down` to bring down containers.