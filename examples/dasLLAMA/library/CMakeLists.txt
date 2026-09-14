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
    # both emissions drop dasVulkan, this one dasLLVM too - ARCHITECTURE.md sec. 3.8
    COMMAND $<TARGET_FILE:daslang> --disable-module dasLLVM --disable-module dasVulkan
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
endforeach()
target_compile_definitions(dasllama_host_c_ctx PRIVATE DASLLAMA_LIB_STANDALONE_CTX)
set_target_properties(dasllama_host_c_ctx PROPERTIES
    C_STANDARD 11 C_STANDARD_REQUIRED ON LINKER_LANGUAGE CXX)

### -lib: the same source jitted into a native library with a C ABI
if(NOT DAS_LLVM_DISABLED)
    set(_out "${CMAKE_CURRENT_SOURCE_DIR}/_out")
    set(_lib "${_out}/dasllama_lib${CMAKE_SHARED_LIBRARY_SUFFIX}")
    set(_bind "${CMAKE_CURRENT_SOURCE_DIR}/dasllama_lib_c.das")
    add_custom_command(
        OUTPUT "${_lib}" "${_out}/dasllama_lib.h" "${_bind}"
        COMMAND ${CMAKE_COMMAND} -E make_directory "${_out}"
        COMMAND $<TARGET_FILE:daslang> --disable-module dasVulkan
            -lib "${DASLLAMA_LIB_SRC}" -output "examples/dasLLAMA/library/_out/dasllama_lib"
            -- --jit-lib-bindings "${_bind}"
        DEPENDS ${DASLLAMA_LIB_DEPS}
            "${PROJECT_SOURCE_DIR}/modules/dasLLVM/daslib/jit_standalone.das"
            "${PROJECT_SOURCE_DIR}/modules/dasLLVM/daslib/llvm_jit_run.das"
        WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
        COMMENT "dasLLAMA C-ABI library (JIT): dasllama_lib.das"
        VERBATIM)
    add_custom_target(dasllama_lib_jit DEPENDS "${_lib}" "${_bind}")
    set_target_properties(dasllama_lib_jit PROPERTIES FOLDER "examples/dasLLAMA")

    # lld-link writes an import library beside the dll, which is what a C host links there
    set(_link_input "${_lib}")
    if(WIN32)
        set(_link_input "${_out}/dasllama_lib${CMAKE_IMPORT_LIBRARY_SUFFIX}")
    endif()

    add_executable(dasllama_host_c_jit EXCLUDE_FROM_ALL "${CMAKE_CURRENT_SOURCE_DIR}/main.c")
    add_dependencies(dasllama_host_c_jit dasllama_lib_jit)
    target_include_directories(dasllama_host_c_jit PRIVATE "${_out}")
    # a link OPTION: the artifact has no `lib` prefix, so CMake would rewrite it to -L plus -l
    target_link_options(dasllama_host_c_jit PRIVATE "${_link_input}")
    set_target_properties(dasllama_host_c_jit PROPERTIES
        C_STANDARD 11 C_STANDARD_REQUIRED ON
        BUILD_RPATH "${_out};${PROJECT_SOURCE_DIR}/lib"
        FOLDER "examples/dasLLAMA")
    if(WIN32)
        # no rpath on windows: the dll has to sit beside the exe that loads it
        add_custom_command(TARGET dasllama_host_c_jit POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different "${_lib}" $<TARGET_FILE_DIR:dasllama_host_c_jit>)
    endif()
endif()
