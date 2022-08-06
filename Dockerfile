FROM ubuntu:focal

RUN apt-get clean && \
    apt-get update && \
    apt-get install -y software-properties-common --no-install-recommends

# Contains gcc10
RUN add-apt-repository ppa:ubuntu-toolchain-r/test && \
    apt-get update && \
    apt-get install -y \
    wget \
    git \
    pkg-config \
    libssl-dev \
    libsasl2-dev \
    libcurl4-openssl-dev \
    gcc-10 \
    g++-10 \
    build-essential \
    cmake \
    lcov \
    gdb \
    --no-install-recommends

# Copying the repository into docker image
ENV PROJECT_ROOT=/usr/local/workspace
WORKDIR ${PROJECT_ROOT}
RUN mkdir -p ${PROJECT_ROOT}
COPY . .

## ENVIRONMENT
ENV CXX_COMPILER=g++-10 \
    C_COMPILER=gcc-10 \
    MAKE=make \
    CMAKE=cmake 

RUN PARALLEL_BUILD_PROC=${PARALLEL_BUILD_PROC:-$(nproc --ignore=$(($(nproc)*1/4)))}

ENV BUILD_DIR=${PROJECT_ROOT}/build

## JSON
### I think this is not necessary only json header file should be copied
ENV THIRD_PARTY_DIR=${PROJECT_ROOT}/3rdparty
WORKDIR ${THIRD_PARTY_DIR}
ENV JSON_VERSION=3.9.1
ENV JSON_DIR=${THIRD_PARTY_DIR}/json-${JSON_VERSION}
ENV JSON_BUILD_DIR=${BUILD_DIR}/json-${JSON_VERSION}
ENV JSON_INSTALL_PATH=${JSON_BUILD_DIR}/install/

ADD https://github.com/nlohmann/json/archive/v${JSON_VERSION}.tar.gz ${THIRD_PARTY_DIR}
RUN tar -xzf v${JSON_VERSION}.tar.gz && \
    mkdir -p ${JSON_BUILD_DIR} &&\
    cd ${JSON_BUILD_DIR} && \
    ${CMAKE} -DCMAKE_INSTALL_PREFIX:PATH=${JSON_INSTALL_PATH} -DJSON_BuildTests=OFF -DCMAKE_CXX_COMPILER=${CXX_COMPILER} -DCMAKE_BUILD_TYPE=Release  ${JSON_DIR} && \
    ${MAKE} -j${PARALLEL_BUILD_PROC:-1} install

## PISTACHE
ENV PISTACHE_COMMIT=2a8ea402fcba505a6a90ac3aff2743bf1ec59d94
ENV PISTACHE_DIR=${THIRD_PARTY_DIR}/pistache
ENV PISTACHE_BUILD_DIR=${BUILD_DIR}/pistache/release
WORKDIR ${PISTACHE_DIR}
RUN git clone  https://github.com/oktal/pistache.git . && \
    git reset --hard ${PISTACHE_COMMIT} && \
    mkdir -p ${PISTACHE_BUILD_DIR} && \
    cd ${PISTACHE_BUILD_DIR} && \
    ${CMAKE} -DCMAKE_INSTALL_PREFIX=${PISTACHE_BUILD_DIR} -DCMAKE_CXX_COMPILER=${CXX_COMPILER} -DCMAKE_BUILD_TYPE=Release ${PISTACHE_DIR} && \
    ${MAKE} -j${PARALLEL_BUILD_PROC:-1} install

## RCS SERVER
WORKDIR ${BUILD_DIR}
ENV LIB_PATH=${PISTACHE_BUILD_DIR};${JSON_INSTALL_PATH};
RUN mkdir -p ${BUILD_DIR} &&\
    mkdir -p ${BUILD_DIR}/results &&\
    cd ${BUILD_DIR}
RUN ${CMAKE} ${PROJECT_ROOT} \
    -DCMAKE_BUILD_TYPE=Debug  \
    -DCMAKE_CXX_COMPILER=${CXX_COMPILER} \
    -DCMAKE_C_COMPILER=${C_COMPILER} \
    -DCMAKE_PREFIX_PATH=${LIB_PATH} && \
    ${MAKE} -j${PARALLEL_BUILD_PROC:-1}

EXPOSE 9080
CMD ["tests/server/TestServer", "src/RestServer"]
