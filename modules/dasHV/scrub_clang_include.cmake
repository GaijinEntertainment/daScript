# Run the command after '--' with clang's resource include dir removed from INCLUDE.
#
# Under the VS ClangCL toolset, INCLUDE carries clang's resource headers
# (.../lib/clang/<ver>/include), whose <stdint.h>/<vadefs.h> use the GNU #include_next.
# OpenSSL builds with plain `cl` (its VC config), which rejects #include_next (C1021).
# Stripping INCLUDE here propagates through the process env to nmake's recursive `cl`
# children, so they fall back to MSVC's stdint.h. (A `nmake CC=clang-cl` override does
# NOT reach those recursive invocations.)

# Command to run = everything after the literal '--'.
set(_cmd "")
set(_sep OFF)
math(EXPR _last "${CMAKE_ARGC}-1")
foreach(_i RANGE 0 ${_last})
    if(_sep)
        list(APPEND _cmd "${CMAKE_ARGV${_i}}")
    elseif("${CMAKE_ARGV${_i}}" STREQUAL "--")
        set(_sep ON)
    endif()
endforeach()
if(NOT _cmd)
    message(FATAL_ERROR "scrub_clang_include.cmake: no command after '--'. "
        "Usage: cmake -P scrub_clang_include.cmake -- <command> [args...]")
endif()

# Drop any INCLUDE entry under a clang resource dir (.../lib/clang/...).
set(_inc "$ENV{INCLUDE}")
set(_keep "")
foreach(_p IN LISTS _inc)
    file(TO_CMAKE_PATH "${_p}" _pn)
    string(TOLOWER "${_pn}" _pl)
    string(FIND "${_pl}" "/lib/clang/" _hit)
    if(_hit EQUAL -1)
        list(APPEND _keep "${_p}")
    endif()
endforeach()
set(ENV{INCLUDE} "${_keep}")

execute_process(COMMAND ${_cmd} RESULT_VARIABLE _rv)
if(_rv)
    message(FATAL_ERROR "scrub_clang_include: command failed (${_rv}): ${_cmd}")
endif()
