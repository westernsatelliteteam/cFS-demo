FROM ubuntu:20.04

ARG DEBIAN_FRONTEND=noninteractive

ARG LCOV_VERSION=1.14-2

ARG ENABLE_UNIT_TESTS
ENV ENABLE_UNIT_TESTS=${ENABLE_UNIT_TESTS} 

ARG SIMULATION
ENV SIMULATION=${SIMULATION}

ARG BUILDTYPE
ENV BUILDTYPE=${BUILDTYPE}

ARG OMIT_DEPRECATED
ENV OMIT_DEPRECATED=${OMIT_DEPRECATED}

RUN apt-get -qy update                           \
    && apt-get -y install                        \
        ca-certificates  \
        git=1:2.25.1-1ubuntu3                    \
        cmake=3.16.3-1ubuntu1                    \
        make=4.2.1-1.2                           \
        gcc=4:9.3.0-1ubuntu2                     \
        g++=4:9.3.0-1ubuntu2                     \
    && rm -rf /var/lib/apt/lists/*                           

WORKDIR /cfs

ENTRYPOINT [ "./dockerfiles/start.sh" ]