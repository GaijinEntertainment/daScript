cmake_minimum_required(VERSION 3.16)
project(example_sort_bench CXX)

# Standalone — does not link against the daslang runtime, only includes
# src/builtin/das_qsort_r.h (header-only, no external deps). Build it on
# its own: `cmake -S examples/sort -B build/example_sort_bench -DCMAKE_BUILD_TYPE=Release`
# then `cmake --build build/example_sort_bench -j`.

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "" FORCE)
endif()

add_executable(example_sort_bench bench_sort_family.cpp)
target_include_directories(example_sort_bench PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../include)

add_executable(example_byte_swap_bench bench_byte_swap.cpp)

if(MSVC)
    target_compile_options(example_sort_bench PRIVATE /O2 /W4)
    target_compile_options(example_byte_swap_bench PRIVATE /O2 /W4)
else()
    target_compile_options(example_sort_bench PRIVATE -O3 -Wall -Wextra)
    target_compile_options(example_byte_swap_bench PRIVATE -O3 -Wall -Wextra)
endif()

# Optional libstdc++ build of the sort bench, gated on availability of a
# system GCC. On macOS the system clang uses libc++; libstdc++ ships only
# with Homebrew GCC. This produces a second binary so we can see how our
# das_qsort_r.h compares against libstdc++'s Musser introsort in addition
# to libc++'s pdqsort+block-quicksort.
find_program(EXAMPLE_SORT_BENCH_GXX
    NAMES g++-15 g++-14 g++-13 g++-12 g++-11
    DOC "GCC C++ compiler for the libstdc++ variant of the sort bench")
if(EXAMPLE_SORT_BENCH_GXX)
    set(_libstdcxx_out ${CMAKE_CURRENT_BINARY_DIR}/example_sort_bench_libstdcxx)
    add_custom_command(
        OUTPUT ${_libstdcxx_out}
        COMMAND ${EXAMPLE_SORT_BENCH_GXX}
                -std=c++17 -O3 -Wall -Wextra
                -I ${CMAKE_CURRENT_SOURCE_DIR}/../../include
                ${CMAKE_CURRENT_SOURCE_DIR}/bench_sort_family.cpp
                -o ${_libstdcxx_out}
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/bench_sort_family.cpp
                ${CMAKE_CURRENT_SOURCE_DIR}/../../include/daScript/simulate/das_qsort_r.h
        COMMENT "Building example_sort_bench_libstdcxx with ${EXAMPLE_SORT_BENCH_GXX}"
        VERBATIM)
    add_custom_target(example_sort_bench_libstdcxx ALL DEPENDS ${_libstdcxx_out})
endif()
