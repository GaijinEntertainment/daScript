#!/bin/sh -e

rm -r -f cmake_temp
mkdir cmake_temp
cd cmake_temp

#-DDAS_CLANG_BIND_DISABLED=FALSE
cmake .. -DCMAKE_BUILD_TYPE=RELEASE -DDAS_LLVM_DISABLED=FALSE -DDAS_GLFW_DISABLED=TRUE -G Ninja
