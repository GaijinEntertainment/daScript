
rd /Q /S cmake_temp
mkdir cmake_temp
cd cmake_temp

set CMAKE_GENERATOR=Visual Studio 16 2019

cmake -G "%CMAKE_GENERATOR%" -DCMAKE_INSTALL_PREFIX=. ..\
