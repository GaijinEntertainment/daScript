### dasLLAMA AOT host: the daslang command-line binary with the engine's CPU modules compiled
### to C++ ahead of time and linked in - the portable tier without the JIT.
###
###   cmake --build build --target dasllama_aot
###   bin/dasllama_aot -use-aot examples/dasLLAMA/run.das -- <model.gguf>
###
### Opt-in (EXCLUDE_FROM_ALL): ~90 AOT TUs. The GPU tiers, the LLVM IR generators and the
### exchange server are not AOT'd - they interpret as scaffolding (the Vulkan classes alone
### emit 40 MB of C++), and a call from AOT'd code into them is inlined C++ regardless.

IF(DAS_LLAMA_INCLUDED AND NOT ${DAS_AOT_EXAMPLES_DISABLED} AND NOT (WIN32 AND CMAKE_SIZEOF_VOID_P EQUAL 4))
    FILE(GLOB DASLLAMA_AOT_ENGINE_FILES RELATIVE ${PROJECT_SOURCE_DIR} CONFIGURE_DEPENDS "modules/dasLLAMA/dasllama/*.das")
    list(FILTER DASLLAMA_AOT_ENGINE_FILES EXCLUDE REGEX "dasllama_(vulkan|metal|math_vulkan|gemm_gen|gemm_register|exchange)")

    # the daslib modules the CPU path calls at run time that libDaScriptAot does not carry
    SET(DASLLAMA_AOT_DASLIB_FILES
        daslib/aarch64_neon.das
        daslib/x64_avx.das
        daslib/f16_cvt.das
        daslib/jobque_boost.das
        daslib/jobque_profile.das
        daslib/fio.das
        daslib/array_boost.das
        daslib/math_bits.das
        daslib/sha_256.das
        daslib/archive.das
        daslib/clargs.das
        daslib/env_registry.das
        daslib/tty.das
        daslib/toml.das
        daslib/safe_addr.das
    )

    # an edit to any engine module changes its dependents' semantic hashes: every engine TU
    # regenerates on every engine edit (the daslib glob is a global dependency already). The
    # generator reads each C++ module's aotRequire out of the loaded shared module, so those
    # are inputs too - a module without the override AOT-disables its whole require closure
    FILE(GLOB DASLLAMA_AOT_ALL_ENGINE_FILES CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/modules/dasLLAMA/dasllama/*.das")
    SET(DAS_AOT_EXTRA_DEPENDS ${DASLLAMA_AOT_ALL_ENGINE_FILES} ${DAS_DYN_MODULES_LIBS})
    add_custom_target(dasllama_aot_engine)
    SET(DASLLAMA_AOT_ENGINE_GENERATED_SRC)
    DAS_AOT_LIB("${DASLLAMA_AOT_ENGINE_FILES}" DASLLAMA_AOT_ENGINE_GENERATED_SRC dasllama_aot_engine daslang)
    add_custom_target(dasllama_aot_daslib)
    SET(DASLLAMA_AOT_DASLIB_GENERATED_SRC)
    DAS_AOT_LIB("${DASLLAMA_AOT_DASLIB_FILES}" DASLLAMA_AOT_DASLIB_GENERATED_SRC dasllama_aot_daslib daslang)
    unset(DAS_AOT_EXTRA_DEPENDS)
    SOURCE_GROUP_FILES("aot generated" DASLLAMA_AOT_ENGINE_GENERATED_SRC)
    SOURCE_GROUP_FILES("aot generated" DASLLAMA_AOT_DASLIB_GENERATED_SRC)

    add_executable(dasllama_aot EXCLUDE_FROM_ALL ${DAS_DASCRIPT_MAIN_SRC}
        ${DASLLAMA_AOT_ENGINE_GENERATED_SRC}
        ${DASLLAMA_AOT_DASLIB_GENERATED_SRC}
    )
    target_sources(dasllama_aot PRIVATE ${PROJECT_SOURCE_DIR}/src/misc/alloc_tracker_overrides.cpp)
    TARGET_LINK_LIBRARIES(dasllama_aot libDaScriptAot ${SRC_LIBRARIES} ${DAS_MODULES_LIBS})
    ADD_DEPENDENCIES(dasllama_aot libDaScriptAot dasllama_aot_engine dasllama_aot_daslib)
    # the engine's AOT TUs include the C++ headers of every module in their require closure
    # (dasAudio's miniaudio, dasMinfft, dasVulkan's volk) - test_aot's dirs and defines plus volk
    target_include_directories(dasllama_aot PRIVATE ${NEED_MODULES_PATH} ${AUDIO_INCLUDE_DIR} ${CIPIC_HRTF_INCLUDE_DIR}
        ${PROJECT_SOURCE_DIR}/modules/dasMinfft/minfft
        ${PROJECT_SOURCE_DIR}/modules/dasVulkan/vendor/volk
        ${PROJECT_SOURCE_DIR}/modules/dasVulkan/vendor/Vulkan-Headers/include)
    target_compile_definitions(dasllama_aot PRIVATE MINFFT_SINGLE HRTF_SAMPLE_RATE=${DAS_AUDIO_SAMPLE_RATE})
    SETUP_CPP11(dasllama_aot)
ENDIF()
