FROM ubuntu:bionic

RUN apt-get update \
  && apt-get install -y --no-install-recommends \
  build-essential \
  clang-8 \
  git \
  libjpeg-dev \
  libpng-dev \
  libz-dev \
  llvm-8-dev \
  python3-pip \
  && rm -rf /var/lib/apt/lists/* \
  && python3 -m pip --no-cache install cmake

WORKDIR /usr/local/src/heterohalide
COPY Halide Halide
COPY new new
RUN cmake -S Halide -B build \
  && cmake --build build --parallel $(nproc) --target Halide
RUN new/run_all.sh
