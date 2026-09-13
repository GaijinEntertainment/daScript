###########################################################
# Three daslang libraries with a C ABI, bound back into daslang.
#
# Each .das becomes a shared library through `daslang -lib`, and the same run writes the
# daslang `[extern]` bindings for it - the das twin of the generated C header. main.das
# requires those, so the example hand-writes no declaration and parses no C header.
#
# The bindings sit beside their source because a das `require` resolves relative to the
# requiring file; they are generated, and .gitignore covers them.
###########################################################

if(DAS_LLVM_DISABLED)
    return()
endif()

# The libraries live beside the source, the one place main.das also rebuilds them into, and
# the -output path stays relative to the tree root so the bindings carry a relative library
# path rather than this machine's absolute one.
set(C_API_LIB_OUT "examples/c_api_library/_out")

function(das_c_api_library stem select_flag jit_flag)
    set(_bind "${CMAKE_CURRENT_SOURCE_DIR}/${stem}_c.das")
    # one -lib run writes both, and main.das rebuilds neither: declare the library too, or a
    # deleted artifact leaves the example bound to nothing
    set(_lib "${CMAKE_CURRENT_SOURCE_DIR}/_out/${stem}${CMAKE_SHARED_LIBRARY_SUFFIX}")
    add_custom_command(
        OUTPUT "${_bind}" "${_lib}"
        COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_CURRENT_SOURCE_DIR}/_out"
        COMMAND $<TARGET_FILE:daslang> -lib "${CMAKE_CURRENT_SOURCE_DIR}/${stem}.das"
            -output "${C_API_LIB_OUT}/${stem}" ${select_flag}
            -- ${jit_flag} --jit-lib-bindings "${_bind}"
        DEPENDS daslang
            "${CMAKE_CURRENT_SOURCE_DIR}/${stem}.das"
            "${PROJECT_SOURCE_DIR}/daslib/c_api_header.das"
            "${PROJECT_SOURCE_DIR}/modules/dasLLVM/daslib/jit_standalone.das"
            "${PROJECT_SOURCE_DIR}/modules/dasLLVM/daslib/llvm_jit_run.das"
        WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
        COMMENT "daslang -lib bindings: ${stem}"
        VERBATIM
    )
    set(C_API_LIB_BINDINGS ${C_API_LIB_BINDINGS} "${_bind}" PARENT_SCOPE)
endfunction()

das_c_api_library(shapes    "" "")
das_c_api_library(units     -lib-export-all "")
das_c_api_library(greetings "" --jit-lib-export-marked)

# ALL, not just a test-small dependency: CI runs `ctest -L small` without building that target
add_custom_target(c_api_library_bindings ALL DEPENDS ${C_API_LIB_BINDINGS})
set_target_properties(c_api_library_bindings PROPERTIES FOLDER "examples")

# The libraries and their bindings both come from the commands above; main.das builds nothing,
# so the example is only about binding three libraries at once and calling them.
add_test(NAME c_api_library COMMAND $<TARGET_FILE:daslang> examples/c_api_library/main.das
         WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})
set_tests_properties(c_api_library PROPERTIES LABELS "small")
add_dependencies(test-small c_api_library_bindings)
