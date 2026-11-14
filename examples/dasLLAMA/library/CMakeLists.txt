###########################################################
# dasLLAMA as a library, through both standalone backends. Opt-in (EXCLUDE_FROM_ALL): each one
# compiles the whole engine - the -lib codegen about two minutes, the -ctx translation unit
# ~50 MB, ten minutes and ~6 GB to compile. README.md lists the targets and how to run them.
###########################################################

if(NOT DAS_LLAMA_INCLUDED)
    return()
endif()

set(DASLLAMA_LIB_SRC "${CMAKE_CURRENT_SOURCE_DIR}/dasllama_lib.das")
set(DASLLAMA_CTX_GEN "${CMAKE_CURRENT_BINARY_DIR}/_standalone_ctx_generated")
file(MAKE_DIRECTORY "${DASLLAMA_CTX_GEN}")
include_directories("${DASLLAMA_CTX_GEN}" "${PROJECT_SOURCE_DIR}/include"
    "${PROJECT_SOURCE_DIR}/3rdparty/fmt/include" ${NEED_MODULES_PATH} ${DAS_CONFIG_INCLUDE_DIR}
    ${AUDIO_INCLUDE_DIR} ${CIPIC_HRTF_INCLUDE_DIR} "${PROJECT_SOURCE_DIR}/modules/dasMinfft/minfft")
add_compile_definitions(MINFFT_SINGLE HRTF_SAMPLE_RATE=${DAS_AUDIO_SAMPLE_RATE})

file(GLOB DASLLAMA_LIB_ENGINE CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/modules/dasLLAMA/dasllama/*.das")
set(DASLLAMA_LIB_DEPS daslang "${DASLLAMA_LIB_SRC}" ${DASLLAMA_LIB_ENGINE}
    "${PROJECT_SOURCE_DIR}/daslib/c_api_header.das")

### -ctx: a standalone context - one header plus one translation unit
add_custom_command(
    OUTPUT "${DASLLAMA_CTX_GEN}/dasllama_lib.das.cpp" "${DASLLAMA_CTX_GEN}/dasllama_lib.das.h"
    # both emissions drop dasVulkan; this one drops dasLLVM and dasCUDA too, which have no
    # AOT form - ARCHITECTURE.md#library-two-backends
    COMMAND $<TARGET_FILE:daslang> --disable-module dasLLVM --disable-module dasVulkan
        --disable-module dasCUDA
        "${PROJECT_SOURCE_DIR}/utils/aot/main.das" -- -ctx "${DASLLAMA_LIB_SRC}" "${DASLLAMA_CTX_GEN}/"
    DEPENDS ${DASLLAMA_LIB_DEPS} "${PROJECT_SOURCE_DIR}/utils/aot/main.das"
        "${PROJECT_SOURCE_DIR}/daslib/aot_standalone.das" "${PROJECT_SOURCE_DIR}/daslib/aot_cpp.das"
    WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
    COMMENT "dasLLAMA standalone context (C++): dasllama_lib.das"
    VERBATIM)

add_library(dasllama_lib_ctx OBJECT EXCLUDE_FROM_ALL "${DASLLAMA_CTX_GEN}/dasllama_lib.das.cpp")
target_link_libraries(dasllama_lib_ctx PRIVATE libDaScript ${SRC_LIBRARIES} libDasModuleAudio)
SETUP_CPP11(dasllama_lib_ctx)
set_target_properties(dasllama_lib_ctx PROPERTIES FOLDER "examples/dasLLAMA")

foreach(_host cpp c)
    set(_t dasllama_host_${_host}_ctx)
    add_executable(${_t} EXCLUDE_FROM_ALL
        "${CMAKE_CURRENT_SOURCE_DIR}/main.${_host}" $<TARGET_OBJECTS:dasllama_lib_ctx>)
    target_link_libraries(${_t} PRIVATE libDaScript ${SRC_LIBRARIES} libDasModuleAudio)
    SETUP_CPP11(${_t})
    set_target_properties(${_t} PROPERTIES FOLDER "examples/dasLLAMA")
    if(MSVC)
        # the emitted engine runs on the host's own stack, and outgrows the 1 MB a Windows exe gets
        target_link_options(${_t} PRIVATE "/STACK:16777216")
    endif()
endforeach()
target_compile_definitions(dasllama_host_c_ctx PRIVATE DASLLAMA_LIB_STANDALONE_CTX)
set_target_properties(dasllama_host_c_ctx PROPERTIES
    C_STANDARD 11 C_STANDARD_REQUIRED ON LINKER_LANGUAGE CXX)

### -lib: the same source jitted into a native library with a C ABI
if(NOT DAS_LLVM_DISABLED)
    # the CPU class a build for any PC of this architecture starts from, and the class profile
    # a forced build stamps its kernels from
    if(CMAKE_SYSTEM_PROCESSOR MATCHES "^(x86_64|AMD64|amd64)$")
        set(DASLLAMA_LIB_FLOOR "x86-avx2")
    else()
        set(DASLLAMA_LIB_FLOOR "arm-neon")
    endif()
    set(DASLLAMA_LIB_PROFILE
        "${PROJECT_SOURCE_DIR}/modules/dasLLAMA/performance/defaults/${DASLLAMA_LIB_FLOOR}.tune-defaults.json")
    # a fat build stamps a clone from every class profile, so each is an input of it
    file(GLOB DASLLAMA_LIB_PROFILES CONFIGURE_DEPENDS
        "${PROJECT_SOURCE_DIR}/modules/dasLLAMA/performance/defaults/*.tune-defaults.json")

    # One -lib build and its C host: `mode` is host, fat or forced, `linkage` shared or static
    # (README.md, "Three ways to pick the kernels"). Every variant is named dasllama_lib, because
    # the artifact's name is the prefix of its C entry points, so each sits in a folder of its
    # own with the host that links it.
    function(dasllama_lib_variant mode linkage)
        set(_tag "${mode}_${linkage}")
        set(_dir "${CMAKE_CURRENT_SOURCE_DIR}/_out/${_tag}")
        set(_rel "examples/dasLLAMA/library/_out/${_tag}/dasllama_lib")
        set(_libt dasllama_lib_${_tag})
        set(_hostt dasllama_host_c_${_tag})
        set(_tail "")
        set(_bind "")
        set(_env "")
        if(_tag STREQUAL "host_shared")
            # README.md and main.das build this variant as dasllama_lib_jit, with its daslang bindings
            set(_dir "${CMAKE_CURRENT_SOURCE_DIR}/_out")
            set(_rel "examples/dasLLAMA/library/_out/dasllama_lib")
            set(_libt dasllama_lib_jit)
            set(_hostt dasllama_host_c_jit)
            set(_bind "${CMAKE_CURRENT_SOURCE_DIR}/dasllama_lib_c.das")
            set(_tail -- --jit-lib-bindings "${_bind}")
        endif()
        set(_profiles "")
        if(mode STREQUAL "fat")
            set(_env "DAS_JIT_BASELINE=${DASLLAMA_LIB_FLOOR}" "DAS_TUNE_MODE=fat")
            set(_profiles ${DASLLAMA_LIB_PROFILES})
        elseif(mode STREQUAL "forced")
            set(_env "DAS_JIT_BASELINE=${DASLLAMA_LIB_FLOOR}" "DAS_TUNE_FORCE=${DASLLAMA_LIB_PROFILE}")
            set(_profiles "${DASLLAMA_LIB_PROFILE}")
        endif()
        if(linkage STREQUAL "static")
            set(_lib "${_dir}/dasllama_lib${CMAKE_STATIC_LIBRARY_SUFFIX}")
            set(_tail -- --jit-static)
        else()
            set(_lib "${_dir}/dasllama_lib${CMAKE_SHARED_LIBRARY_SUFFIX}")
        endif()
        add_custom_command(
            OUTPUT "${_lib}" "${_dir}/dasllama_lib.h" ${_bind}
            COMMAND ${CMAKE_COMMAND} -E make_directory "${_dir}"
            COMMAND ${CMAKE_COMMAND} -E env ${_env} $<TARGET_FILE:daslang> --disable-module dasVulkan
                -lib "${DASLLAMA_LIB_SRC}" -output "${_rel}" ${_tail}
            DEPENDS ${DASLLAMA_LIB_DEPS} ${_profiles}
                "${PROJECT_SOURCE_DIR}/modules/dasLLVM/daslib/jit_standalone.das"
                "${PROJECT_SOURCE_DIR}/modules/dasLLVM/daslib/llvm_jit_run.das"
            WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
            COMMENT "dasLLAMA C-ABI library (JIT, ${mode} kernels, ${linkage}): dasllama_lib.das"
            VERBATIM)
        add_custom_target(${_libt} DEPENDS "${_lib}" ${_bind})
        set_target_properties(${_libt} PROPERTIES FOLDER "examples/dasLLAMA")

        add_executable(${_hostt} EXCLUDE_FROM_ALL "${CMAKE_CURRENT_SOURCE_DIR}/main.c")
        add_dependencies(${_hostt} ${_libt})
        target_include_directories(${_hostt} PRIVATE "${_dir}")
        set_target_properties(${_hostt} PROPERTIES
            C_STANDARD 11 C_STANDARD_REQUIRED ON FOLDER "examples/dasLLAMA")
        if(linkage STREQUAL "static")
            # the archive holds the engine alone: the daslang library is the host's to link - the
            # whole one, since the engine registers a module that lives in its compiler half
            target_link_libraries(${_hostt} PRIVATE "${_lib}" libDaScript ${SRC_LIBRARIES})
            set_target_properties(${_hostt} PROPERTIES LINKER_LANGUAGE CXX)
        else()
            # lld-link writes an import library beside the dll, which is what a C host links there
            set(_link_input "${_lib}")
            if(WIN32)
                set(_link_input "${_dir}/dasllama_lib${CMAKE_IMPORT_LIBRARY_SUFFIX}")
            endif()
            # a link OPTION: the artifact has no `lib` prefix, so CMake would rewrite it to -L plus -l
            target_link_options(${_hostt} PRIVATE "${_link_input}")
            set_target_properties(${_hostt} PROPERTIES BUILD_RPATH "${_dir};${PROJECT_SOURCE_DIR}/lib")
        endif()
        if(_tag STREQUAL "host_shared")
            if(WIN32)
                # no rpath on windows: the dll has to sit beside the exe that loads it
                add_custom_command(TARGET ${_hostt} POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${_lib}" $<TARGET_FILE_DIR:${_hostt}>)
            endif()
        else()
            if(WIN32 AND linkage STREQUAL "shared")
                # no rpath on windows: the two daslang dlls the library imports sit beside it too
                add_custom_command(TARGET ${_hostt} POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy_if_different
                        $<TARGET_FILE:libDaScriptDyn_runtime> $<TARGET_FILE:libDaScriptDyn> "${_dir}")
            endif()
            set_target_properties(${_hostt} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${_dir}")
            foreach(_cfg ${CMAKE_CONFIGURATION_TYPES})
                string(TOUPPER "${_cfg}" _cfg_up)
                set_target_properties(${_hostt} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_${_cfg_up} "${_dir}")
            endforeach()
        endif()
    endfunction()

    foreach(_mode host fat forced)
        foreach(_linkage shared static)
            dasllama_lib_variant(${_mode} ${_linkage})
        endforeach()
    endforeach()
endif()
