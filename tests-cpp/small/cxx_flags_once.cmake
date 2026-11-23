include(${CMAKE_CURRENT_LIST_DIR}/../../cmake/das_cxx_flags_once.cmake)

macro(EXPECT_CXX_FLAGS _expected _case)
    if(NOT "${CMAKE_CXX_FLAGS}" STREQUAL "${_expected}")
        message(FATAL_ERROR "${_case}: CMAKE_CXX_FLAGS is '${CMAKE_CXX_FLAGS}', expected '${_expected}'")
    endif()
endmacro()

set(CMAKE_CXX_FLAGS "/DWIN32 /EHsc")
DAS_CXX_FLAGS_ONCE("/std:c++17")
EXPECT_CXX_FLAGS("/DWIN32 /EHsc /std:c++17" "a flag that is not there is appended")
DAS_CXX_FLAGS_ONCE("/std:c++17")
DAS_CXX_FLAGS_ONCE("/std:c++17")
EXPECT_CXX_FLAGS("/DWIN32 /EHsc /std:c++17" "a flag that is there is not appended again")

set(CMAKE_CXX_FLAGS "/std:c++17")
DAS_CXX_FLAGS_ONCE("/std:c++17")
EXPECT_CXX_FLAGS("/std:c++17" "the only flag counts as there")

set(CMAKE_CXX_FLAGS "-std=c++17x")
DAS_CXX_FLAGS_ONCE("-std=c++17")
EXPECT_CXX_FLAGS("-std=c++17x -std=c++17" "a longer flag with the same start is another flag")

set(CMAKE_CXX_FLAGS "x-std=c++17")
DAS_CXX_FLAGS_ONCE("-std=c++17")
EXPECT_CXX_FLAGS("x-std=c++17 -std=c++17" "a longer flag with the same end is another flag")

set(CMAKE_CXX_FLAGS "-O2")
DAS_CXX_FLAGS_ONCE("-m64 -std=c++17")
DAS_CXX_FLAGS_ONCE("-m64 -std=c++17")
EXPECT_CXX_FLAGS("-O2 -m64 -std=c++17" "a group of flags is appended once")
