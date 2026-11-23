# Drives modules/dasGlfw/patch_glfw.cmake: a fresh run applies the patch, a re-run leaves the patched
# source as it is, a source the patch was not made for and a missing argument fail.
#
# Usage: cmake -DPATCH_SCRIPT=<patch_glfw.cmake> -DGIT_EXECUTABLE=<git> -DWORK_DIR=<dir> -P test_patch_glfw.cmake

function(stage case_dir text)
    file(REMOVE_RECURSE "${case_dir}")
    file(WRITE "${case_dir}/kept.txt" "${text}")
endfunction()

function(run_patch src_dir out_rc out_log)
    set(args -DGLFW_PATCH=${patch_file} -DGIT_EXECUTABLE=${GIT_EXECUTABLE})
    if(NOT src_dir STREQUAL "")
        list(APPEND args -DGLFW_SRC_DIR=${src_dir})
    endif()
    execute_process(COMMAND ${CMAKE_COMMAND} ${args} -P ${PATCH_SCRIPT}
        RESULT_VARIABLE rc OUTPUT_VARIABLE out ERROR_VARIABLE err)
    set(${out_rc} "${rc}" PARENT_SCOPE)
    set(${out_log} "${out}${err}" PARENT_SCOPE)
endfunction()

function(fail message)
    message(FATAL_ERROR "test_patch_glfw: ${message}")
endfunction()

file(REMOVE_RECURSE "${WORK_DIR}")
set(pristine "one\ntwo\nthree\n")
set(patched "one\nTWO\nthree\n")
set(drifted "one\nzwei\nthree\n")
set(patch_file "${WORK_DIR}/fixture.patch")
string(JOIN "\n" patch_text
    "diff --git a/kept.txt b/kept.txt"
    "--- a/kept.txt"
    "+++ b/kept.txt"
    "@@ -1,3 +1,3 @@"
    " one"
    "-two"
    "+TWO"
    " three"
    "diff --git a/added.txt b/added.txt"
    "new file mode 100644"
    "--- /dev/null"
    "+++ b/added.txt"
    "@@ -0,0 +1 @@"
    "+added"
    "")
file(WRITE "${patch_file}" "${patch_text}")

set(fresh "${WORK_DIR}/fresh")
stage("${fresh}" "${pristine}")
run_patch("${fresh}" rc log)
if(NOT rc EQUAL 0)
    fail("a fresh run fails: ${log}")
endif()
file(READ "${fresh}/kept.txt" kept)
if(NOT kept STREQUAL patched OR NOT EXISTS "${fresh}/added.txt")
    fail("a fresh run exits 0 and leaves the source unpatched: ${log}")
endif()

run_patch("${fresh}" rc log)
file(READ "${fresh}/kept.txt" kept)
file(READ "${fresh}/added.txt" added)
string(FIND "${log}" "is already applied" at_msg)
if(NOT rc EQUAL 0 OR NOT kept STREQUAL patched OR NOT added STREQUAL "added\n" OR at_msg EQUAL -1)
    fail("a re-run fails, changes the patched source, or does not report the patch as already applied: ${log}")
endif()

set(drift "${WORK_DIR}/drift")
stage("${drift}" "${drifted}")
run_patch("${drift}" rc log)
file(READ "${drift}/kept.txt" kept)
string(FIND "${log}" "neither applies" at_msg)
if(rc EQUAL 0 OR at_msg EQUAL -1 OR NOT kept STREQUAL drifted)
    fail("a run over a source the patch was not made for succeeds, changes it, or fails without saying so: ${log}")
endif()

run_patch("" rc log)
string(FIND "${log}" "pass -DGLFW_SRC_DIR" at_msg)
if(rc EQUAL 0 OR at_msg EQUAL -1)
    fail("a run with no source dir succeeds, or fails without naming the argument: ${log}")
endif()
