cmake_minimum_required(VERSION 3.12)
# modules/dasGlfw/ARCHITECTURE.md#ime-patch
#
# Usage: cmake -DGLFW_SRC_DIR=<GLFW source dir> -DGLFW_PATCH=<patch file> -DGIT_EXECUTABLE=<git> -P patch_glfw.cmake

foreach(arg GLFW_SRC_DIR GLFW_PATCH GIT_EXECUTABLE)
    if(NOT ${arg})
        message(FATAL_ERROR "patch_glfw.cmake: pass -D${arg}=<value>")
    endif()
endforeach()

set(ENV{GIT_DIR} "${GLFW_SRC_DIR}.no-repository")

function(git_apply out_rc out_log)
    execute_process(COMMAND "${GIT_EXECUTABLE}" apply --whitespace=nowarn ${ARGN} "${GLFW_PATCH}"
        WORKING_DIRECTORY "${GLFW_SRC_DIR}"
        RESULT_VARIABLE rc OUTPUT_VARIABLE out ERROR_VARIABLE err)
    set(${out_rc} "${rc}" PARENT_SCOPE)
    set(${out_log} "${out}${err}" PARENT_SCOPE)
endfunction()

git_apply(rc log --reverse --check)
if(rc EQUAL 0)
    message(STATUS "patch_glfw.cmake: ${GLFW_PATCH} is already applied to ${GLFW_SRC_DIR}")
    return()
endif()

git_apply(rc log)
if(NOT rc EQUAL 0)
    get_filename_component(prefix_dir "${GLFW_SRC_DIR}/../.." ABSOLUTE)
    message(FATAL_ERROR "patch_glfw.cmake: ${GLFW_PATCH} neither applies to ${GLFW_SRC_DIR} nor is already applied there - "
        "the source is not the release the patch was made for; delete ${prefix_dir} so the release is extracted again\n${log}")
endif()
