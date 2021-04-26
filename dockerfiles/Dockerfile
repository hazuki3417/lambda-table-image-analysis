FROM ubuntu:20.04
ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y \
    git \
    automake \
    autoconf \
    g++ \
    zip \
    libcurl4-openssl-dev \
    libssl-dev \
    uuid-dev \
    zlib1g-dev \
    libpulse-dev \
    && apt-get clean

# build & install latest cmake
RUN git clone https://github.com/Kitware/CMake.git && \
    cd CMake
    #  && \
    # ./bootstrap && \
    # make && \
    # make install
# RUN
