# The JIT half of the standalone sweep, run by the standalone_sweep_jit target:
#   cmake -DDASLANG=<exe> -DROOT=<tree> -DOUT=<dir> "-DFILES=a.das|b.das" -P sweep_jit.cmake
# Emission goes through utils/internal/jit/main.das, which takes many files per process - the
# ~52-file dasLLVM load costs ~4.5s and is paid once per chunk, not once per library. Each library
# is then loaded back by a generated host. A library that never emitted is tallied as refused; one
# that emitted and cannot run sets the exit code.

foreach(_var DASLANG ROOT OUT FILES)
    if(NOT DEFINED ${_var})
        message(FATAL_ERROR "sweep_jit.cmake: -D${_var} is required")
    endif()
endforeach()
file(MAKE_DIRECTORY ${OUT})
string(REPLACE "|" ";" _files "${FILES}")
list(LENGTH _files _total)

# the emitters are .das, so daslang's mtime says nothing about them
file(GLOB _emitter_srcs ${ROOT}/daslib/*.das ${ROOT}/modules/dasLLVM/daslib/*.das
    ${ROOT}/utils/internal/jit/main.das)
set(_newest_input "${DASLANG}")
foreach(_src IN LISTS _emitter_srcs)
    if(${_src} IS_NEWER_THAN ${_newest_input})
        set(_newest_input "${_src}")
    endif()
endforeach()

set(_emit "")
set(_kept 0)
foreach(_f IN LISTS _files)
    get_filename_component(_stem ${_f} NAME_WE)
    set(_so "${OUT}/${_stem}.so")
    if(EXISTS ${_so} AND EXISTS "${OUT}/${_stem}_c.das"
            AND NOT ${_f} IS_NEWER_THAN ${_so} AND NOT ${_newest_input} IS_NEWER_THAN ${_so})
        math(EXPR _kept "${_kept} + 1")
    else()
        list(APPEND _emit ${_f})
    endif()
endforeach()
list(LENGTH _emit _n_emit)
message(STATUS "standalone sweep (JIT): ${_n_emit} libraries to emit, ${_kept} reused")

# 32 per process: a file that leaves the JIT's global state broken takes only its chunk with it
set(_chunk "")
set(_done 0)
foreach(_f IN LISTS _emit)
    list(APPEND _chunk ${_f})
    math(EXPR _done "${_done} + 1")
    list(LENGTH _chunk _n_chunk)
    if(_n_chunk EQUAL 32 OR _done EQUAL _n_emit)
        execute_process(
            COMMAND ${DASLANG} ${ROOT}/utils/internal/jit/main.das -- ${_chunk}
                    --jit-lib --lib-export-all --lib-output-dir ${OUT} --jit-lib-bindings auto
            WORKING_DIRECTORY ${ROOT})
        set(_chunk "")
    endif()
endforeach()

set(_ok 0)
set(_refused "")
set(_broken "")
foreach(_f IN LISTS _files)
    get_filename_component(_stem ${_f} NAME_WE)
    set(_bind "${OUT}/${_stem}_c.das")
    set(_p "")
    if(EXISTS ${_bind})
        # the entry points carry the emitter's prefix, which escapes a keyword stem (enum -> enum_)
        file(STRINGS ${_bind} _decl REGEX "name=\"[A-Za-z_][A-Za-z0-9_]*_create\"" LIMIT_COUNT 1)
        string(REGEX MATCH "name=\"([A-Za-z_][A-Za-z0-9_]*)_create\"" _unused "${_decl}")
        set(_p "${CMAKE_MATCH_1}")
    endif()
    if(_p STREQUAL "")
        list(APPEND _refused ${_f})
        continue()
    endif()

    # the host binds nothing by hand: it requires the generated bindings beside it, and
    # shutdown_runtime balances what create brought up, or daslang's atexit check trips
    set(_host "${OUT}/${_stem}_host.das")
    file(WRITE ${_host}
"options gen2

require ${_stem}_c

[export]
def main() {
    var ctx = ${_p}_create()
    if (ctx == null) {
        panic(\"${_p}: {${_p}_last_error(null)}\")
    }
    ${_p}_destroy(ctx)
    ${_p}_shutdown_runtime()
}
")
    execute_process(COMMAND ${DASLANG} ${_host} WORKING_DIRECTORY ${ROOT}
        RESULT_VARIABLE _rc OUTPUT_VARIABLE _log ERROR_VARIABLE _log)
    if(_rc EQUAL 0)
        math(EXPR _ok "${_ok} + 1")
    else()
        list(APPEND _broken ${_f})
        message(STATUS "    FAILED to load or run (${_rc})\n${_log}")
    endif()
endforeach()

list(LENGTH _refused _n_refused)
list(LENGTH _broken _n_broken)
message(STATUS "standalone sweep (JIT): ${_total} files, ${_ok} ran, ${_n_refused} refused, ${_n_broken} broken")
foreach(_f IN LISTS _refused _broken)
    message(STATUS "  not run: ${_f}")
endforeach()
if(_n_broken GREATER 0)
    message(FATAL_ERROR "standalone sweep (JIT): ${_n_broken} libraries built but did not run")
endif()
