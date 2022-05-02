
rd /Q /S cmake_temp
mkdir cmake_temp
cd cmake_temp

set CMAKE_GENERATOR=Visual Studio 17 2022

cmake -G "%CMAKE_GENERATOR%" -DCMAKE_INSTALL_PREFIX=. ..\
