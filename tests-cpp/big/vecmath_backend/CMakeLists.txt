# Lives under big/ because each arm is a standalone exe (the scalar-forced TU's
# vec4f is a different type than the one libDaScript was compiled with, so
# neither arm can link libDaScript or join the small glob exe), but labelled
# "small" so the per-PR CI lane runs both backends. Both arms pin precise math: the rows
# assert IEEE NaN, tie and out-of-range answers, which a fast-math build is free to fold.

add_executable(test_vecmath_scalar test_vecmath_backend.cpp)
target_compile_definitions(test_vecmath_scalar PRIVATE _TARGET_SIMD_SCALAR=1 EXPECT_SCALAR=1)
target_compile_options(test_vecmath_scalar PRIVATE ${DAS_PRECISE_MATH_FLAG})
target_include_directories(test_vecmath_scalar PRIVATE ${PROJECT_SOURCE_DIR}/include)
SETUP_CPP11(test_vecmath_scalar)
set_target_properties(test_vecmath_scalar PROPERTIES FOLDER "tests-cpp/big")

add_test(NAME vecmath_scalar COMMAND test_vecmath_scalar
         WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})
set_tests_properties(vecmath_scalar PROPERTIES LABELS "small")
add_dependencies(test-small test_vecmath_scalar)

# The native arm only exists where a native SIMD backend can be selected: under
# DAS_VECMATH_SCALAR (or on a host with no SIMD ISA) its EXPECT_NATIVE tripwire
# would fail the build by design.
if(NOT DAS_VECMATH_SCALAR)
    add_executable(test_vecmath_native test_vecmath_backend.cpp)
    target_compile_definitions(test_vecmath_native PRIVATE EXPECT_NATIVE=1)
    target_compile_options(test_vecmath_native PRIVATE ${DAS_PRECISE_MATH_FLAG})
    target_include_directories(test_vecmath_native PRIVATE ${PROJECT_SOURCE_DIR}/include)
    SETUP_CPP11(test_vecmath_native)
    set_target_properties(test_vecmath_native PROPERTIES FOLDER "tests-cpp/big")

    add_test(NAME vecmath_native COMMAND test_vecmath_native
             WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})
    set_tests_properties(vecmath_native PROPERTIES LABELS "small")
    add_dependencies(test-small test_vecmath_native)
endif()

# Under emscripten (the web/ build) the native arm is the wasm SIMD128 backend and both arms
# become node-runnable .js + .wasm pairs under web/output/tests. The web scope's directory-wide
# link options embed daslib and disable main (-sINVOKE_RUN=0) for the interpreter binary; a
# backend test wants neither, so the pair keeps only the exception-model flags every object in
# that build carries. The wasm_build.yml lane builds and runs both under node.
if(EMSCRIPTEN)
    target_compile_definitions(test_vecmath_native PRIVATE EXPECT_WASM=1)
    foreach(_arm test_vecmath_scalar test_vecmath_native)
        set_target_properties(${_arm} PROPERTIES
            LINK_OPTIONS "-fwasm-exceptions;-sWASM_LEGACY_EXCEPTIONS=0"
            RUNTIME_OUTPUT_DIRECTORY ${DAS_WEB_OUTPUT_DIR}/tests)
    endforeach()
endif()
