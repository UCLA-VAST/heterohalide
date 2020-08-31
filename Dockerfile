FROM ubuntu:bionic
WORKDIR /usr/local/src/heterohalide
COPY Halide Halide
COPY new new

RUN apt-get update \
  && apt-get install -y --no-install-recommends \
  build-essential \
  clang-8 \
  libz-dev \
  llvm-8-dev \
  python3-pip \
  && rm -rf /var/lib/apt/lists/* \
  && python3 -m pip --no-cache install cmake
RUN cmake -S Halide -B build \
  && cmake --build build --parallel $(nproc) --target Halide
RUN new/blur/run_blur.sh
