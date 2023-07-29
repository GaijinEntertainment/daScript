cp ../CMakeXxdImpl.txt .

rm -r -f cmake_temp
mkdir cmake_temp
cd cmake_temp

cmake -DCMAKE_BUILD_TYPE=Release -G Ninja -DCMAKE_TOOLCHAIN_FILE=../emsdk/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake ../ 
