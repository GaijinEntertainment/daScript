#!/bin/sh -e

time {

    rm -r -f cmake_temp
    mkdir cmake_temp
    cd cmake_temp

    CC=clang CXX=clang++ cmake -DCMAKE_OSX_ARCHITECTURES="x86_64;arm64" \
        -DDAS_GLFW_DISABLED:BOOL=TRUE           \
        -DDAS_IMGUI_DISABLED:BOOL=TRUE          \
        -DDAS_BGFX_DISABLED:BOOL=TRUE           \
        -DDAS_XBYAK_DISABLED:BOOL=TRUE          \
        -DDAS_CLANG_BIND_DISABLED:BOOL=TRUE     \
        -DDAS_MINFFT_DISABLED:BOOL=TRUE         \
        -DDAS_SOUND_DISABLED:BOOL=TRUE          \
        -DDAS_STDDLG_DISABLED:BOOL=TRUE         \
        -DDAS_STBIMAGE_DISABLED:BOOL=TRUE       \
        -DDAS_STBTRUETYPE_DISABLED:BOOL=TRUE    \
        -DCMAKE_INSTALL_PREFIX=. -DCMAKE_BUILD_TYPE=DEBUG -DCMAKE_OSX_DEPLOYMENT_TARGET=10.12 -G Xcode ../

}
