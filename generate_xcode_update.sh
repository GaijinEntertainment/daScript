#!/bin/sh -e

time {
    cd cmake_temp
    CC=clang CXX=clang++ cmake -DCMAKE_OSX_ARCHITECTURES="x86_64" -DCMAKE_INSTALL_PREFIX=. -DCMAKE_BUILD_TYPE=DEBUG -DCMAKE_OSX_DEPLOYMENT_TARGET=10.12 -G Xcode ../
}
