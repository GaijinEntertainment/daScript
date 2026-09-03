# Integration test driver for `daslang -lib`: build the library, then build a C host against the
# generated header with CMake (so every toolchain gets its own flags) and run it. The shared
# scenario always runs; the static one runs where the static runtime archive exists.
#
# Inputs (-D on the cmake -P invocation):
#   DASLANG      - the daslang binary
#   SRC_DAS      - the .das to build as a library
#   HOST_C       - the C host to compile against the generated header
#   C_COMPILER   - the C compiler the nested project uses
#   GENERATOR    - the CMake generator the nested project uses
#   BUILD_TYPE   - the configuration the nested project builds
#   RUNTIME_DLLS - the daslang runtime shared libraries the library import-links
#   STATIC_RT    - the static runtime archive, or empty to skip the static scenario
#   WORKDIR      - a temporary working directory

if(NOT DASLANG OR NOT SRC_DAS OR NOT HOST_C OR NOT WORKDIR)
    message(FATAL_ERROR "run_capi.cmake: missing required -D inputs")
endif()
foreach(_f "${DASLANG}" "${SRC_DAS}" "${HOST_C}")
    if(NOT EXISTS "${_f}")
        message(FATAL_ERROR "not found: ${_f}")
    endif()
endforeach()
if(NOT BUILD_TYPE)
    set(BUILD_TYPE Release)
endif()

file(REMOVE_RECURSE "${WORKDIR}")
file(MAKE_DIRECTORY "${WORKDIR}")

function(run_step desc)
    execute_process(COMMAND ${ARGN} WORKING_DIRECTORY "${WORKDIR}"
        RESULT_VARIABLE _rc OUTPUT_VARIABLE _out ERROR_VARIABLE _err)
    if(_rc)
        message("${_out}${_err}")
        message(FATAL_ERROR "${desc} failed (exit ${_rc})")
    endif()
endfunction()

# Builds and runs the host against `lib_path`, the full path to the produced library. `_kind` is
# SHARED or STATIC; `extra_libs` carries what a static link additionally needs.
function(build_and_run_host tag _kind lib_path extra_libs)
    set(_dir "${WORKDIR}/host_${tag}")
    file(MAKE_DIRECTORY "${_dir}")
    # An IMPORTED target is the portable way to link a library by path: a bare path whose file name
    # is not lib<name>.<ext> gets turned into -l<stem>, which no linker can then find.
    file(WRITE "${_dir}/CMakeLists.txt"
"cmake_minimum_required(VERSION 3.16)
project(das_lib_capi_host C CXX)
find_package(Threads REQUIRED)
add_library(das_fixture ${_kind} IMPORTED)
set_target_properties(das_fixture PROPERTIES IMPORTED_LOCATION \"${lib_path}\" IMPORTED_IMPLIB \"${lib_path}\")
add_executable(host \"${HOST_C}\")
target_include_directories(host PRIVATE \"${WORKDIR}\")
target_link_libraries(host PRIVATE das_fixture ${extra_libs} Threads::Threads \${CMAKE_DL_LIBS})
if(NOT MSVC)
    target_link_libraries(host PRIVATE m)
endif()
# The C++ driver links: the daslang runtime is C++, so a static link needs its standard library.
set_target_properties(host PROPERTIES
    LINKER_LANGUAGE CXX
    RUNTIME_OUTPUT_DIRECTORY \"${WORKDIR}\"
    BUILD_RPATH \"${WORKDIR}\")
")
    set(_cfg_args -S "${_dir}" -B "${_dir}/b" -DCMAKE_BUILD_TYPE=${BUILD_TYPE})
    if(GENERATOR)
        list(APPEND _cfg_args -G "${GENERATOR}")
    endif()
    if(C_COMPILER)
        list(APPEND _cfg_args -DCMAKE_C_COMPILER=${C_COMPILER})
    endif()
    run_step("configuring the ${tag} C host" "${CMAKE_COMMAND}" ${_cfg_args})
    run_step("building the ${tag} C host" "${CMAKE_COMMAND}" --build "${_dir}/b" --config ${BUILD_TYPE})

    # multi-config generators put the exe under a per-config subdirectory
    set(_exe "${WORKDIR}/host")
    foreach(_candidate "${WORKDIR}/host" "${WORKDIR}/host.exe"
                       "${WORKDIR}/${BUILD_TYPE}/host" "${WORKDIR}/${BUILD_TYPE}/host.exe")
        if(EXISTS "${_candidate}")
            set(_exe "${_candidate}")
        endif()
    endforeach()
    if(NOT EXISTS "${_exe}")
        message(FATAL_ERROR "the ${tag} C host was not produced")
    endif()
    execute_process(COMMAND "${_exe}" WORKING_DIRECTORY "${WORKDIR}"
        RESULT_VARIABLE _rc OUTPUT_VARIABLE _out ERROR_VARIABLE _err)
    message("${tag}: ${_out}${_err}")
    if(_rc)
        message(FATAL_ERROR "the C host failed against the ${tag} library (exit ${_rc})")
    endif()
    if(NOT _out MATCHES "ok")
        message(FATAL_ERROR "the C host did not report ok against the ${tag} library")
    endif()
    # <P>_destroy owes the program its [finalize] functions - nothing in the runtime finds them
    if(NOT _out MATCHES "fixture \\[finalize\\] ran")
        message(FATAL_ERROR "the ${tag} library did not run [finalize] on destroy")
    endif()
    file(REMOVE "${_exe}")
endfunction()

# --- the shared library ---
run_step("daslang -lib" "${DASLANG}" -lib "${SRC_DAS}" -output "${WORKDIR}/fixture")
if(NOT EXISTS "${WORKDIR}/fixture.h")
    message(FATAL_ERROR "daslang -lib wrote no C header")
endif()

# the loader looks beside the library, so put the runtime there too
foreach(_dll ${RUNTIME_DLLS})
    if(EXISTS "${_dll}")
        file(COPY "${_dll}" DESTINATION "${WORKDIR}")
    endif()
endforeach()

if(WIN32)
    set(_shared "${WORKDIR}/fixture.lib")      # MSVC links through the import library
    if(NOT EXISTS "${_shared}")
        set(_shared "${WORKDIR}/fixture.dll")
    endif()
elseif(APPLE)
    set(_shared "${WORKDIR}/fixture.dylib")
else()
    set(_shared "${WORKDIR}/fixture.so")
endif()

if(EXISTS "${_shared}")
    build_and_run_host(shared SHARED "${_shared}" "")
else()
    message(STATUS "no shared library was produced (a static daslang build) - shared scenario skipped")
endif()

# --- the static archive ---
if(STATIC_RT AND EXISTS "${STATIC_RT}")
    run_step("daslang -lib --jit-lib-static" "${DASLANG}" -lib "${SRC_DAS}"
        -output "${WORKDIR}/fixture_s" -- --jit-lib-static)
    if(WIN32)
        set(_archive "${WORKDIR}/fixture_s.lib")
    else()
        set(_archive "${WORKDIR}/fixture_s.a")
    endif()
    if(NOT EXISTS "${_archive}")
        message(FATAL_ERROR "--jit-lib-static wrote no archive")
    endif()
    build_and_run_host(static STATIC "${_archive}" "\"${STATIC_RT}\"")
else()
    message(STATUS "static runtime archive unavailable here - static scenario skipped")
endif()
