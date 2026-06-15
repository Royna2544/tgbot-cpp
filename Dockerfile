FROM debian:latest
LABEL org.opencontainers.image.authors="Oleg Morozenkov <m@oleg.rocks>"

RUN apt-get -qq update && \
    apt-get -qq install -y g++ make binutils cmake libssl-dev zlib1g-dev nlohmann-json3-dev && \
    rm -rf /var/lib/apt/lists/*

WORKDIR /usr/src/tgbot-cpp
COPY include include
COPY src src
COPY third_party third_party
COPY CMakeLists.txt ./

RUN cmake . && \
    make -j$(nproc) && \
    make install && \
    rm -rf /usr/src/tgbot-cpp/*
