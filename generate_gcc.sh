#!/bin/sh -e

time {

    rm -r -f cmake_temp
    mkdir cmake_temp
    cd cmake_temp

    CC=gcc-10 CXX=g++-10 cmake -DCMAKE_OSX_ARCHITECTURES="x86_64" -DCMAKE_INSTALL_PREFIX=. -DCMAKE_BUILD_TYPE=DEBUG -DCMAKE_OSX_DEPLOYMENT_TARGET=10.12 -G "Unix Makefiles" ../
}
