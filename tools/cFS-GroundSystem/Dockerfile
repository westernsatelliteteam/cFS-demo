FROM ubuntu:20.04 AS builder

ARG DEBIAN_FRONTEND=noninteractive

RUN apt-get -qy update                           \
    && apt-get -y install                        \
        ca-certificates=20201027ubuntu0.20.04.1  \
        make=4.2.1-1.2                           \
        gcc=4:9.3.0-1ubuntu2                     \
        g++=4:9.3.0-1ubuntu2                     \
    && rm -rf /var/lib/apt/lists/*                           

COPY . .

RUN cd Subsystems/cmdUtil && make && cd ../..

ENTRYPOINT [ "./cmd.sh" ] 