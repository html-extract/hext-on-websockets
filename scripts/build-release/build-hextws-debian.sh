#!/usr/bin/env bash

{
  set -xeuo pipefail

  apt update
  apt upgrade -y
  apt install -y \
    git \
    cmake \
    build-essential \
    libboost1.81-dev \
    libboost-regex1.81-dev \
    libboost-program-options1.81.0 \
    libboost-program-options1.81-dev \
    libgumbo-dev \
    googletest \
    libgtest-dev \
    libssl-dev \
    rapidjson-dev \
    libseccomp-dev \
    libboost-regex1.81.0 \
    libgumbo1 \
    libssl3 \
    libseccomp2

  HEXTD=$(mktemp -d)
  git clone "https://github.com/html-extract/hext.git" "$HEXTD"

  CMAKE_MAKE_FLAGS="-j16"

  LIBHEXTD="$HEXTD/libhext"
  cd "$LIBHEXTD/test/build"
  cmake ..
  make $CMAKE_MAKE_FLAGS
  ./libhext-test

  HEXTPREFIXD=$(mktemp -d)

  cd "$LIBHEXTD/build"
  cmake -DCMAKE_INSTALL_PREFIX="$HEXTPREFIXD" -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=Off ..
  make $CMAKE_MAKE_FLAGS
  make install

  HEXTWSD=$(mktemp -d)
  git clone "https://github.com/html-extract/hext-on-websockets.git" "$HEXTWSD"

  cd "$HEXTWSD/build"
  cmake -DCMAKE_INSTALL_PREFIX="$HEXTPREFIXD" -DCMAKE_BUILD_TYPE=Release -DHEXTWS_SECCOMP=On ..
  make $CMAKE_MAKE_FLAGS
  ldd ./hextws
  make install

  cmake -DCMAKE_INSTALL_PREFIX="$HEXTPREFIXD" -DCMAKE_BUILD_TYPE=Release -DHEXTWS_SECCOMP=On -DHEXTWS_SECCOMP_TEST=On ..
  make $CMAKE_MAKE_FLAGS
  cp ./hextws "$HEXTPREFIXD/bin/hextws-seccomp-test"

  ls -lh "$HEXTPREFIXD/bin"
} 1>&2

tar -c -z -C "$HEXTPREFIXD" bin
