FROM ubuntu:20.04

ARG DEBIAN_FRONTEND=noninteractive

RUN apt-get -qy update                              \
    && apt-get -y install --no-install-recommends   \
        ca-certificates \
        git                     \
        cmake               \
        make                         \
        build-essential \
        g++-arm-linux-gnueabihf \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /cfs

ENTRYPOINT [ "./dockerfiles/start.sh" ]