cmake_minimum_required (VERSION 3.16)
project (DAS)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(DAS_FLEX_BISON_DISABLED "Disable FLEX/BISON stage" ON)
option(DAS_CLANG_BIND_DISABLED "Disable dasClangBind (libclang bindings, C/C++ parsing)" ON)
option(DAS_LLVM_DISABLED "Disable dasLLVM (llvm bindings)" ON)
option(DAS_HV_DISABLED "Disable dasHV (websokets,http server and client)" ON)
option(DAS_GLFW_DISABLED "Disable dasGLFW (GLFW window for graphics apps)" ON)
option(DAS_AUDIO_DISABLED "Disable dasAudio (Miniaudio sound library)" ON)
option(DAS_STDDLG_DISABLED "Disable dasStdDlg (File new,open,save etc dialogs)" ON)
option(DAS_STBIMAGE_DISABLED "Disable dasStbImage (StbImage bindings, image loading and saving)" ON)

option(DAS_PUGIXML_DISABLED "Disable dasPUGIXML (xml parsing library)" ON)
option(DAS_SQLITE_DISABLED "Disable dasSQLITE (sqlite3 library)" ON)

add_compile_options(-msimd128)
add_compile_options(-msse2)
add_compile_options(-mnontrapping-fptoint)

# Modern wasm EH (try_table/exnref). DAS_ENABLE_EXCEPTIONS routes daslang
# error paths through C++ throw instead of setjmp/longjmp.
add_compile_options(-fwasm-exceptions -sWASM_LEGACY_EXCEPTIONS=0)
add_link_options(-fwasm-exceptions -sWASM_LEGACY_EXCEPTIONS=0)
add_compile_definitions(DAS_ENABLE_EXCEPTIONS _EMSCRIPTEN_VER)

add_link_options(
    # Default STACK_SIZE is 64KB, not enough for daslang.
    -sSTACK_SIZE=4MB
    -sEXPORTED_RUNTIME_METHODS=callMain,FS
    # INVOKE_RUN needed to disable running daslang on initialization
    # (it will print help message and exit).
    -sINVOKE_RUN=0
    # NODEFS required for mounting the host filesystem in Node.js tests.
    -sFORCE_FILESYSTEM=1
    -lnodefs.js
    # Allow heap to grow beyond the default 16MB cap.
    -sALLOW_MEMORY_GROWTH=1
)
# embed daslang standard library into wasm binary.
# SHELL: keeps the --embed-file + path pair atomic — without it, CMake folds
# repeated --embed-file flags into one token (drops the second), so the second
# directory becomes a positional arg to em++ and the embed silently fails.
add_link_options("SHELL:--embed-file ${CMAKE_CURRENT_SOURCE_DIR}/../daslib@daslib")
# Exclude modules whose native dependencies are not linked in the WASM build.
# spoof + its only consumer linked_list both require peg/peg (dasPEG module),
# which is not linked here. Embedding them gives users a confusing
# "module peg/peg not found" compile error; better to fail at the require
# itself with "module not found".
add_link_options("SHELL:--exclude-file */daslib/spoof.das")
add_link_options("SHELL:--exclude-file */daslib/linked_list.das")
# embed dastest runner so the playground's Test button can invoke
# /dastest/dastest.das against user-authored [test] functions.
add_link_options("SHELL:--embed-file ${CMAKE_CURRENT_SOURCE_DIR}/../dastest@dastest")

IF(DEFINED DAS_CONFIG_INCLUDE_DIR)
    INCLUDE_DIRECTORIES(${DAS_CONFIG_INCLUDE_DIR})
ENDIF()

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(DAS_WEB_OUTPUT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/output
    CACHE PATH "Directory where daslang.wasm and web UI assets are written")

add_subdirectory(.. ${CMAKE_BINARY_DIR}/daslang_src EXCLUDE_FROM_ALL)
# All we need in web is daslang_static.wasm.
set_target_properties(daslang_static PROPERTIES
    EXCLUDE_FROM_ALL FALSE
    RUNTIME_OUTPUT_DIRECTORY ${DAS_WEB_OUTPUT_DIR})

# Pin the wasm32 runtime archive into web/output/lib so it doesn't collide
# with the host build's lib/ (root CMakeLists.txt:317 resets
# CMAKE_ARCHIVE_OUTPUT_DIRECTORY inside the subdirectory scope, so we override
# per-target). dasLLVM (link_wasm) auto-locates the archives from this path.
set_target_properties(libDaScript_runtime PROPERTIES
    EXCLUDE_FROM_ALL FALSE
    ARCHIVE_OUTPUT_DIRECTORY ${DAS_WEB_OUTPUT_DIR}/lib)

# Copy web UI assets to output at build time.
add_custom_target(copy_web_ui ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/examples/ui
        ${DAS_WEB_OUTPUT_DIR}
    COMMENT "Copying web UI to ${DAS_WEB_OUTPUT_DIR}"
)

# Stage everything site/playground/ needs for local-dev preview:
#   - vendored UI sources (main.js/css, jquery)        from web/examples/ui/src/
#   - sample bundles (data.json, hello.das, ...)       from web/examples/ui/samples/
#   - WASM artifact (daslang_static.{js,wasm})         from daslang_static target
# Files land alongside the committed site/playground/index.html + forge-skin.css
# + playground-*.js. The destination dir is .gitignore'd for these artifacts —
# see site/.gitignore. After `ninja`, `cd site && python3 -m http.server 8000`
# serves /playground/ with no extra cp.
set(SITE_PLAYGROUND_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../site/playground)
set(SITE_FILES_WASM_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../site/files/wasm)
add_custom_target(stage_site_playground ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/examples/ui/src
        ${SITE_PLAYGROUND_DIR}
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/examples/ui/samples
        ${SITE_PLAYGROUND_DIR}/samples
    COMMAND ${CMAKE_COMMAND} -E copy
        ${DAS_WEB_OUTPUT_DIR}/daslang_static.js
        ${DAS_WEB_OUTPUT_DIR}/daslang_static.wasm
        ${SITE_PLAYGROUND_DIR}/
    COMMAND ${CMAKE_COMMAND} -E make_directory ${SITE_FILES_WASM_DIR}
    COMMAND ${CMAKE_COMMAND} -E copy
        ${DAS_WEB_OUTPUT_DIR}/daslang_static.js
        ${DAS_WEB_OUTPUT_DIR}/daslang_static.wasm
        ${SITE_FILES_WASM_DIR}/
    DEPENDS daslang_static
    COMMENT "Staging site/playground/ and site/files/wasm/ for local-dev preview"
)

# Host daslang sub-build (outer is emsdk), drives .das→.wasm cross-compile.
include(ExternalProject)
find_program(DAS_HOST_CC  NAMES clang gcc   REQUIRED)
find_program(DAS_HOST_CXX NAMES clang++ g++ REQUIRED)
find_program(DAS_WASMTIME wasmtime)
set(_host_compiler_args)
if(NOT CMAKE_HOST_WIN32)
    set(_host_compiler_args
        -DCMAKE_C_COMPILER:FILEPATH=${DAS_HOST_CC}
        -DCMAKE_CXX_COMPILER:FILEPATH=${DAS_HOST_CXX})
endif()
ExternalProject_Add(host_daslang
    EXCLUDE_FROM_ALL TRUE
    SOURCE_DIR       ${CMAKE_CURRENT_SOURCE_DIR}/..
    INSTALL_COMMAND  ""
    BUILD_BYPRODUCTS ${CMAKE_CURRENT_SOURCE_DIR}/../bin/daslang
    STEP_TARGETS     build
    CMAKE_CACHE_ARGS -DCMAKE_TOOLCHAIN_FILE:STRING=
                     ${_host_compiler_args}
                     -DCMAKE_BUILD_TYPE:STRING=Release
                     -DDAS_LLVM_DISABLED:BOOL=OFF
                     -DDAS_PUGIXML_DISABLED:BOOL=OFF
                     # dasGlfw pulls find_package(OpenGL REQUIRED) at configure
                     # time. The pages.yml runner doesn't apt-install OpenGL
                     # dev libs, and this sub-build is tooling-only (drives
                     # .das→wasm codegen via dasLLVM — no graphics needed).
                     -DDAS_GLFW_DISABLED:BOOL=ON
    BUILD_COMMAND    ${CMAKE_COMMAND} --build <BINARY_DIR> --target daslang --parallel)

# Cross-compile web playground example scripts to wasm32 via host daslang.
set(_repo ${CMAKE_CURRENT_SOURCE_DIR}/..)
set(DAS_HOST_DASLANG ${_repo}/bin/daslang)
set(_web_ex_src ${CMAKE_CURRENT_SOURCE_DIR}/examples/ui/samples/examples)
set(_web_ex_out ${DAS_WEB_OUTPUT_DIR}/samples/examples)

add_custom_target(all_wasm)
set(_wasm_run_cmds)
# Each name maps to web/examples/ui/samples/examples/<name>.das. Names match
# the bench `name` slug emitted by dasProfile (and the playground `?example=`
# URL param), with hyphens replaced by underscores (`spectral-norm` →
# `spectral_norm`) so they're valid CMake target stems.
foreach(ex hello loop func
        dict sha256 exp f2i f2s fib_loop fib_recursive mandelbrot
        nbodies particles primes queen spectral_norm table_sort tree)
    add_custom_command(OUTPUT ${_web_ex_out}/${ex}.wasm
        COMMAND ${CMAKE_COMMAND} -E make_directory ${_web_ex_out}
        COMMAND ${DAS_HOST_DASLANG} -exe -output ${_web_ex_out}/${ex}
                ${_web_ex_src}/${ex}.das --
                --jit-target=wasm32-unknown-emscripten
                --jit-runtime-lib=$<TARGET_FILE:libDaScript_runtime>
        DEPENDS ${_web_ex_src}/${ex}.das host_daslang libDaScript_runtime
        WORKING_DIRECTORY ${_repo}
        COMMENT "Cross-compile ${ex} → wasm32" VERBATIM)
    add_custom_target(${ex}_web_wasm DEPENDS ${_web_ex_out}/${ex}.wasm)
    add_dependencies(all_wasm ${ex}_web_wasm)
    if(DAS_WASMTIME)
        list(APPEND _wasm_run_cmds
            COMMAND ${CMAKE_COMMAND} -E echo "=== ${ex}.wasm ==="
            COMMAND ${DAS_WASMTIME} -W exceptions=y ${_web_ex_out}/${ex}.wasm)
    endif()
endforeach()

if(DAS_WASMTIME)
    add_custom_target(run_wasm_examples
        ${_wasm_run_cmds}
        DEPENDS all_wasm
        COMMENT "Run cross-compiled .wasm examples under wasmtime")
endif()

# Stage example .wasm into site/playground for local-dev preview (gitignored).
add_custom_target(stage_site_playground_wasm
    COMMAND ${CMAKE_COMMAND} -E make_directory ${_repo}/site/playground/samples/examples
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${_web_ex_out} ${_repo}/site/playground/samples/examples
    DEPENDS all_wasm
    COMMENT "Staging JIT example .wasm into site/playground/")
