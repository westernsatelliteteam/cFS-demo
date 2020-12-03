# WEST - cFS Demo 

This flight software (FSW) is intended to function as a proof of concept for cFS as it pertains to WEST. The demo will exercise features within cFS to test its capabilities and famailizarize members with the framework.

## Build and Run

The application is built in Docker to ensure all dependencies are locked. This will require the [Docker Engine](https://docs.docker.com/engine/install/) and [Docker-Compose](https://docs.docker.com/compose/install/)

```
git clone https://github.com/westernsatelliteteam/cFS-demo.git
cd cFS-demo
docker-compose up
```

Should see startup messages, and CFE_ES_Main entering OPERATIONAL state.
NOTE: Currentl build is not operational. Build pipeline will fail when linking to OSAL [WIP].
