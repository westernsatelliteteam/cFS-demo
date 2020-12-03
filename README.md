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

## Current issues
During the build process, type boolean is not being recognized by `sch` app. `sch` is including `cfe.h` which leads down a rabbit hole of includes. There should be a `#include <stdbool.h>` (compiling with C99) but not sure if a missing include is the root cause.

Steps to reproduce:
1. Clone repo
2. Bring up container with `docker-compose up`

Full output:

```
In file included from /cFS/apps/sch/fsw/src/sch_api.c:26:
/cFS/apps/sch/fsw/public_inc/sch_api.h:85:1: error: unknown type name 'boolean'; did you mean 'bool'?
   85 | boolean SCH_GetProcessingState(void);
      | ^~~~~~~
      | bool
In file included from /cFS/apps/sch/fsw/src/sch_api.c:27:
/cFS/apps/sch/fsw/src/sch_apipriv.h:105:1: error: unknown type name 'boolean'; did you mean 'bool'?
  105 | boolean SCH_GetProcessingState(void);
      | ^~~~~~~
      | bool
/cFS/apps/sch/fsw/src/sch_api.c:82:1: error: unknown type name 'boolean'; did you mean 'bool'?
   82 | boolean SCH_GetProcessingState(void)
      | ^~~~~~~
      | bool
make[7]: *** [apps/sch/CMakeFiles/sch.dir/build.make:63: apps/sch/CMakeFiles/sch.dir/fsw/src/sch_api.c.o] Error 1
make[6]: *** [CMakeFiles/Makefile2:696: apps/sch/CMakeFiles/sch.dir/all] Error 2
make[5]: *** [Makefile:141: all] Error 2
make[4]: *** [CMakeFiles/native_default_cpu1-all.dir/build.make:57: CMakeFiles/native_default_cpu1-all] Error 2
make[3]: *** [CMakeFiles/Makefile2:246: CMakeFiles/native_default_cpu1-all.dir/all] Error 2
make[2]: *** [CMakeFiles/Makefile2:335: CMakeFiles/mission-all.dir/rule] Error 2
make[1]: *** [Makefile:253: mission-all] Error 2
make: *** [Makefile:120: all] Error 2
```
