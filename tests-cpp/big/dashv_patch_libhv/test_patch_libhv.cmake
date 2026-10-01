# Drives modules/dasHV/patch_libhv.cmake over pristine_hlog.c: a fresh run patches and saves the
# pristine copy, a re-run rebuilds from that copy, a drifted anchor and a missing source dir fail.
#
# Usage: cmake -DPATCH_SCRIPT=<patch_libhv.cmake> -DFIXTURE=<pristine_hlog.c> -DWORK_DIR=<dir> -P test_patch_libhv.cmake

function(stage case_dir fixture_text)
    file(REMOVE_RECURSE "${case_dir}")
    file(WRITE "${case_dir}/base/hlog.c" "${fixture_text}")
    file(GLOB_RECURSE excerpts RELATIVE "${CMAKE_CURRENT_LIST_DIR}/pristine" "${CMAKE_CURRENT_LIST_DIR}/pristine/*.txt")
    foreach(excerpt IN LISTS excerpts)
        string(REGEX REPLACE "\\.txt$" "" source_name "${excerpt}")
        file(READ "${CMAKE_CURRENT_LIST_DIR}/pristine/${excerpt}" contents)
        file(WRITE "${case_dir}/${source_name}" "${contents}")
    endforeach()
endfunction()

function(run_patch src_dir out_rc out_log)
    if(src_dir STREQUAL "")
        execute_process(COMMAND ${CMAKE_COMMAND} -P ${PATCH_SCRIPT}
            RESULT_VARIABLE rc OUTPUT_VARIABLE out ERROR_VARIABLE err)
    else()
        execute_process(COMMAND ${CMAKE_COMMAND} -DLIBHV_SRC_DIR=${src_dir} -P ${PATCH_SCRIPT}
            RESULT_VARIABLE rc OUTPUT_VARIABLE out ERROR_VARIABLE err)
    endif()
    set(${out_rc} "${rc}" PARENT_SCOPE)
    set(${out_log} "${out}${err}" PARENT_SCOPE)
endfunction()

function(fail message)
    message(FATAL_ERROR "test_patch_libhv: ${message}")
endfunction()

file(READ "${FIXTURE}" pristine)

set(fresh "${WORK_DIR}/fresh")
stage("${fresh}" "${pristine}")
run_patch("${fresh}" rc log)
if(NOT rc EQUAL 0)
    fail("a fresh run fails: ${log}")
endif()
file(READ "${fresh}/base/hlog.c" patched)
file(READ "${fresh}/base/hlog.c.das_pristine" saved)
if(NOT saved STREQUAL pristine)
    fail("the saved copy is not the file the run found")
endif()
string(FIND "${patched}" "hmutex_init(&logger->mutex_);\n    logger_set_file(logger, DEFAULT_LOG_FILE);" at_init)
if(at_init EQUAL -1)
    fail("logger_init does not initialize the mutex before logger_set_file")
endif()
string(FIND "${patched}" "fclose(logger->fp_);" at_close)
if(at_close EQUAL -1)
    fail("logger_set_file does not close the open log file")
endif()

run_patch("${fresh}" rc log)
file(READ "${fresh}/base/hlog.c" rerun)
if(NOT rc EQUAL 0 OR NOT rerun STREQUAL patched)
    fail("a re-run fails or changes the patched file: ${log}")
endif()

file(WRITE "${fresh}/base/hlog.c" "a file an older patch left behind\n")
run_patch("${fresh}" rc log)
file(READ "${fresh}/base/hlog.c" rebuilt)
if(NOT rc EQUAL 0 OR NOT rebuilt STREQUAL patched)
    fail("a re-run does not rebuild from the saved pristine copy: ${log}")
endif()

set(drift "${WORK_DIR}/drift")
string(REPLACE "hmutex_init(&logger->mutex_);" "hmutex_init(&logger->lock_);" drifted "${pristine}")
stage("${drift}" "${drifted}")
run_patch("${drift}" rc log)
string(FIND "${log}" "no longer carries the pristine anchor" at_msg)
if(rc EQUAL 0 OR at_msg EQUAL -1)
    fail("a drifted anchor builds, or fails without naming it: ${log}")
endif()

run_patch("" rc log)
string(FIND "${log}" "-DLIBHV_SRC_DIR" at_usage)
if(rc EQUAL 0 OR at_usage EQUAL -1)
    fail("a run with no source dir succeeds, or fails without its usage: ${log}")
endif()


file(READ "${fresh}/http/server/HttpServer.cpp" shutdown)
string(FIND "${shutdown}" "loop->queueInLoop([loop]() { loop->stop(); });" queued_stop)
if(queued_stop EQUAL -1)
    fail("HTTP server shutdown must dispatch stop on the loop owner")
endif()

file(REMOVE_RECURSE "${WORK_DIR}")
message(STATUS "test_patch_libhv: all cases passed")
