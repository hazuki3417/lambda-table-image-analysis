# Install dependent packages
FROM hazuki3417/cpp-build-cmake:latest as dependent-packages

# Install dependent packages
RUN apt-get install -y \
    libtool \
    libpng-dev \
    libjpeg-dev \
    libtiff-dev \
    zlib1g-dev \
    libarmadillo-dev


FROM hazuki3417/cpp-build-cmake:latest as aws-lambda-cpp-runtime

COPY --from=dependent-packages /usr /usr

ARG SUBMODULE_NAME=aws-lambda-cpp-runtime
ARG SUBMODULE_PATH=/tmp/${SUBMODULE_NAME}

COPY ./submodules/${SUBMODULE_NAME} ${SUBMODULE_PATH}

WORKDIR ${SUBMODULE_PATH}

# build & install aws lambda cpp runtime
RUN mkdir build && cd build && \
    cmake \
    -DCMAKE_BUILD_TYPE=Release \
    -DBUILD_SHARED_LIBS=OFF \
    -DCMAKE_INSTALL_PREFIX=/usr \
    .. && \
    make && \
    make install



FROM hazuki3417/cpp-build-cmake:latest as aws-sdk-cpp

COPY --from=aws-lambda-cpp-runtime /usr /usr

ARG SUBMODULE_NAME=aws-sdk-cpp
ARG SUBMODULE_PATH=/tmp/${SUBMODULE_NAME}

COPY ./submodules/${SUBMODULE_NAME} ${SUBMODULE_PATH}

WORKDIR ${SUBMODULE_PATH}

# build & install aws sdk cpp
RUN mkdir build && cd build && \
    cmake \ 
    -DBUILD_ONLY="core" \
    -DCMAKE_BUILD_TYPE=Release \
    -DBUILD_SHARED_LIBS=OFF \
    -DCMAKE_INSTALL_PREFIX=/usr \
    -DCUSTOM_MEMORY_MANAGEMENT=OFF \
    -DENABLE_UNITY_BUILD=ON \
    .. && \
    make && \
    make install



FROM hazuki3417/cpp-build-cmake:latest as boost

COPY --from=aws-sdk-cpp /usr /usr

RUN apt-get install -y \
    libboost-filesystem-dev \
    libboost-iostreams-dev \
    libboost-serialization-dev \
    libboost-system-dev





FROM hazuki3417/cpp-build-cmake:latest as googletest

COPY --from=boost /usr /usr

RUN apt-get install -y \
    libgtest-dev


FROM hazuki3417/cpp-build-cmake:latest as leptonica

COPY --from=googletest /usr /usr

RUN apt-get install -y \
    libleptonica-dev



FROM hazuki3417/cpp-build-cmake:latest as opencv

COPY --from=leptonica /usr /usr

RUN apt-get install -y \
    libopencv-dev

FROM hazuki3417/cpp-build-cmake:latest as tesseract

COPY --from=opencv /usr /usr


RUN apt-get install -y \
    libtesseract-dev

# FROM hazuki3417/cpp-build-cmake:latest

# COPY --from=tesseract /usr /usr

# WORKDIR /tmp/workspace

# COPY ./docker-build-entrypoint.sh /entrypoint.sh

# RUN chmod +x /entrypoint.sh

# ENTRYPOINT [ "/entrypoint.sh" ]