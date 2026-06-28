cmake_minimum_required (VERSION 3.16)
project (DAS)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(DAS_FLEX_BISON_DISABLED "Disable FLEX/BISON stage" ON)
option(DAS_CLANG_BIND_DISABLED "Disable dasClangBind (libclang bindings, C/C++ parsing)" ON)
option(DAS_LLVM_DISABLED "Disable dasLLVM (llvm bindings)" ON)
option(DAS_HV_DISABLED "Disable dasHV (websokets,http server and client)" ON)
# dasGLFW + dasOpenGL are ENABLED on web: dasGLFW binds glfw* at link-time (emcc
# -sUSE_GLFW=3 supplies the symbols; the module's CMakeLists takes its EMSCRIPTEN
# branch — no GLFW source build), and dasOpenGL's gl* late-externs resolve via
# emscripten_webgl_get_proc_address (gate (b), module_builtin_dasbind.cpp). This is
# the WebGL2/GL-on-the-web rail of the harmonization arc (HARMONIZATION_PLAN §11).
option(DAS_GLFW_DISABLED "Disable dasGLFW (GLFW window for graphics apps)" OFF)
# dasAudio ENABLED on web: miniaudio's emscripten backend uses the Web Audio API
# (ScriptProcessorNode), whose audio callback runs on the main thread — so the
# whole strudel/audio stack runs single-threaded (no -pthread / SharedArrayBuffer).
# strudel's main-thread playback mode (strudel_tick) is non-blocking; the device
# callback and strudel_tick are the same thread. Statically linked into
# daslang_static; the audio/strudel .das halves are embedded below.
option(DAS_AUDIO_DISABLED "Disable dasAudio (Miniaudio sound library)" OFF)
option(DAS_STDDLG_DISABLED "Disable dasStdDlg (File new,open,save etc dialogs)" ON)
# dasStbImage ENABLED on web: opengl_boost requires `stbimage` (and opengl_ttf
# requires stbimage/stbimage_ttf) for texture/font loading. It vendors pure stb
# headers (no system deps) so it cross-compiles to wasm cleanly.
option(DAS_STBIMAGE_DISABLED "Disable dasStbImage (StbImage bindings, image loading and saving)" OFF)

option(DAS_PUGIXML_DISABLED "Disable dasPUGIXML (xml parsing library)" ON)
option(DAS_SQLITE_DISABLED "Disable dasSQLITE (sqlite3 library)" ON)

add_compile_options(-msimd128)
add_compile_options(-msse2)
add_compile_options(-mnontrapping-fptoint)

# wasm64 (memory64): 8-byte pointers. With a 64-bit host, host-ptr-size ==
# target-ptr-size, so the JIT/AOT cross-compile bakes correct layouts (no
# pointer-width mismatch). Build the runtime archive with this ON and point
# DAS_WEB_OUTPUT_DIR at a separate dir (e.g. web/output64) so it does not
# collide with the wasm32 archive — they are ABI-incompatible. dasLLVM
# (find_runtime_lib / link_wasm) auto-selects web/output64 + -sMEMORY64=1 for a
# wasm64-unknown-emscripten target.
option(DAS_WASM_MEMORY64 "Build wasm output for memory64 (8-byte pointers)" OFF)
if(DAS_WASM_MEMORY64)
    add_compile_options(-sMEMORY64=1)
    add_link_options(-sMEMORY64=1)
endif()

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

# WebGL2 / GLFW link profile for the GL-on-the-web rail (HARMONIZATION_PLAN §11).
#   -sUSE_GLFW=3                 emcc's JS reimplementation of the GLFW3 API (canvas/DOM/WebGL);
#                               supplies the glfw* symbols dasGLFW binds at link time.
#   -sFULL_ES3 / MAX_WEBGL=2    GLES3/WebGL2 emulation — the WebGL2 portable floor (no compute/SSBO).
#   -sGL_ENABLE_GET_PROC_ADDRESS emscripten_webgl_get_proc_address (gate (b)) — how dasOpenGL's
#                               gl* late-externs resolve on wasm. Default-on, set explicitly.
# Only daslang_static (Model A interpreter-in-wasm) links in this scope; the standalone
# cross-compiled examples are linked by the host daslang and are unaffected.
add_link_options(
    -sUSE_GLFW=3
    -sFULL_ES3=1
    -sMAX_WEBGL_VERSION=2
    -sGL_ENABLE_GET_PROC_ADDRESS=1
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

# Embed the GL module .das source trees so the interpreter-in-wasm can `require`
# them at runtime (the native fs resolver looks for
# getDasRoot()/modules/<dir>/<subdir>/<name>.das — fs_file_info.cpp). The native
# C++ bindings (libDasModuleGlfw, dasOpenGL) are linked in; these are the matching
# .das halves. Mounted at /modules/<dir>/<subdir> to mirror the desktop layout.
#   glfw/glfw_boost, glfw/glfw_live           -> modules/dasGlfw/dasglfw
#   opengl/*, live/opengl_live                -> modules/dasOpenGL/opengl
#   glsl/glsl_opengl                          -> modules/dasOpenGL/glsl
#   glsl/glsl_common, glsl/glsl_internal      -> modules/dasGlsl/glsl
add_link_options("SHELL:--embed-file ${CMAKE_CURRENT_SOURCE_DIR}/../modules/dasGlfw/dasglfw@modules/dasGlfw/dasglfw")
add_link_options("SHELL:--embed-file ${CMAKE_CURRENT_SOURCE_DIR}/../modules/dasOpenGL/opengl@modules/dasOpenGL/opengl")
add_link_options("SHELL:--embed-file ${CMAKE_CURRENT_SOURCE_DIR}/../modules/dasOpenGL/glsl@modules/dasOpenGL/glsl")
add_link_options("SHELL:--embed-file ${CMAKE_CURRENT_SOURCE_DIR}/../modules/dasGlsl/glsl@modules/dasGlsl/glsl")
#   stbimage, stbimage/stbimage_boost, stbimage/stbimage_ttf -> modules/dasStbImage/stbimage
add_link_options("SHELL:--embed-file ${CMAKE_CURRENT_SOURCE_DIR}/../modules/dasStbImage/stbimage@modules/dasStbImage/stbimage")
# Embed the HUD font so cache_font("{get_das_root()}/modules/dasStbImage/fonts/
# droidsansmono.ttf") resolves in the interpreter. opengl_ttf asserts a non-empty
# font bitmap, so a missing .ttf kills the script in init() — that is why the
# arcanoid/pac-man playground samples (and any TTF-using example) showed a black
# canvas. Mounted at /modules/dasStbImage/fonts: get_das_root() is "." on wasm and
# the MEMFS cwd is "/", so the loader's "./modules/..." resolves to the embed.
add_link_options("SHELL:--embed-file ${CMAKE_CURRENT_SOURCE_DIR}/../modules/dasStbImage/fonts@modules/dasStbImage/fonts")

# Embed the dasAudio .das source trees (the matching halves of the statically
# linked libDasModuleAudio). The native fs resolver looks for
# getDasRoot()/modules/dasAudio/<subdir>/<name>.das (fs_file_info.cpp,
# NATIVE_MODULE), so mount them at /modules/dasAudio/<subdir> to mirror desktop.
#   audio/audio_boost, audio/audio_wav        -> modules/dasAudio/audio
#   strudel/*                                 -> modules/dasAudio/strudel
add_link_options("SHELL:--embed-file ${CMAKE_CURRENT_SOURCE_DIR}/../modules/dasAudio/audio@modules/dasAudio/audio")
add_link_options("SHELL:--embed-file ${CMAKE_CURRENT_SOURCE_DIR}/../modules/dasAudio/strudel@modules/dasAudio/strudel")

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

# Copy web UI assets to output at build time. The OpenGL deferred-shading sample
# fetches mesh + PBR textures from tutorials/_assets/ at runtime (see its
# .das.assets.json manifest + main.js preloadSampleAssets); stage them at the
# repo-relative path the manifest names so the playground can fetch them.
add_custom_target(copy_web_ui ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/examples/ui
        ${DAS_WEB_OUTPUT_DIR}
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/../tutorials/_assets
        ${DAS_WEB_OUTPUT_DIR}/tutorials/_assets
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
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/../tutorials/_assets
        ${SITE_PLAYGROUND_DIR}/tutorials/_assets
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
find_program(DAS_WASMTIME wasmtime)
set(_repo ${CMAKE_CURRENT_SOURCE_DIR}/..)

# DAS_HOST_DASLANG_OVERRIDE: reuse a pre-built host daslang (with dasLLVM + the
# glfw/opengl shared modules) instead of building one here via ExternalProject.
# CI (pages.yml) builds ONE games-capable host and points BOTH the game release
# (daspkg release wasm) and this compute cross-compile at it — so the two don't
# clobber bin/daslang with different configs, and the host is built once. Leave
# unset locally to auto-build a tooling host (LLVM on, glfw off — compute only).
if(DAS_HOST_DASLANG_OVERRIDE)
    set(DAS_HOST_DASLANG ${DAS_HOST_DASLANG_OVERRIDE})
    add_custom_target(host_daslang)   # no-op so `DEPENDS host_daslang` resolves to the prebuilt binary
else()
    find_program(DAS_HOST_CC  NAMES clang gcc   REQUIRED)
    find_program(DAS_HOST_CXX NAMES clang++ g++ REQUIRED)
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
                         # time. This auto-built sub-build is tooling-only (drives
                         # .das→wasm codegen via dasLLVM — no graphics needed).
                         -DDAS_GLFW_DISABLED:BOOL=ON
        BUILD_COMMAND    ${CMAKE_COMMAND} --build <BINARY_DIR> --target daslang --parallel)
    set(DAS_HOST_DASLANG ${_repo}/bin/daslang)
endif()

# Cross-compile web playground example scripts to wasm via the host daslang.
set(_web_ex_src ${CMAKE_CURRENT_SOURCE_DIR}/examples/ui/samples/examples)
set(_web_ex_out ${DAS_WEB_OUTPUT_DIR}/samples/examples)

add_custom_target(all_wasm)
set(_wasm_run_cmds)
# Compute playground samples cross-compile to wasm64 ONLY. wasm32 cross-compile is
# unsupported: the 64-bit host bakes 8-byte-pointer layouts the wasm32 runtime reads
# as 4 — these samples only ever "worked" at wasm32 by dodging that bug class. So the
# whole cross-compile is gated on DAS_WASM_MEMORY64 (the web/build64 config). The
# default wasm32 web build ships only daslang_static, which interprets these same .das
# sources on-target (no host-baked layouts, nothing to dodge).
#
# Each name maps to web/examples/ui/samples/examples/<name>.das. Names match
# the bench `name` slug emitted by dasProfile (and the playground `?example=`
# URL param), with hyphens replaced by underscores (`spectral-norm` →
# `spectral_norm`) so they're valid CMake target stems.
if(DAS_WASM_MEMORY64)
    foreach(ex hello loop func table_sort f2s tree
            dict sha256 exp f2i fib_loop fib_recursive mandelbrot
            nbodies particles primes queen spectral_norm)
        add_custom_command(OUTPUT ${_web_ex_out}/${ex}.wasm
            COMMAND ${CMAKE_COMMAND} -E make_directory ${_web_ex_out}
            COMMAND ${DAS_HOST_DASLANG} -exe -output ${_web_ex_out}/${ex}
                    ${_web_ex_src}/${ex}.das --
                    --jit-target=wasm64-unknown-emscripten
                    --jit-runtime-lib=$<TARGET_FILE:libDaScript_runtime>
            DEPENDS ${_web_ex_src}/${ex}.das host_daslang libDaScript_runtime
            WORKING_DIRECTORY ${_repo}
            COMMENT "Cross-compile ${ex} → wasm64" VERBATIM)
        add_custom_target(${ex}_web_wasm DEPENDS ${_web_ex_out}/${ex}.wasm)
        add_dependencies(all_wasm ${ex}_web_wasm)
        if(DAS_WASMTIME)
            list(APPEND _wasm_run_cmds
                COMMAND ${CMAKE_COMMAND} -E echo "=== ${ex}.wasm ==="
                COMMAND ${DAS_WASMTIME} -W exceptions=y -W memory64=y ${_web_ex_out}/${ex}.wasm)
        endif()
    endforeach()
endif()

# run_wasm_examples + stage_site_playground_wasm are defined UNCONDITIONALLY so a
# non-memory64 web build (the wasm_cross CI lane invokes `ninja run_wasm_examples`)
# doesn't hit "unknown target". When DAS_WASM_MEMORY64 is OFF the foreach above
# registered nothing — `_wasm_run_cmds` is empty and all_wasm has no deps — so both
# targets are no-ops (wasm32 cross-compile is unsupported; the real wasm64 compute
# smoke runs in the memory64 build, driven by pages.yml / daspkg release wasm).
if(DAS_WASMTIME)
    add_custom_target(run_wasm_examples
        ${_wasm_run_cmds}
        DEPENDS all_wasm
        COMMENT "Run cross-compiled wasm64 examples under wasmtime")
endif()

# Stage example .wasm into site/playground for local-dev preview (gitignored).
# make_directory _web_ex_out first so copy_directory is a clean no-op when the
# foreach produced nothing (non-memory64 build).
add_custom_target(stage_site_playground_wasm
    COMMAND ${CMAKE_COMMAND} -E make_directory ${_web_ex_out}
    COMMAND ${CMAKE_COMMAND} -E make_directory ${_repo}/site/playground/samples/examples
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${_web_ex_out} ${_repo}/site/playground/samples/examples
    DEPENDS all_wasm
    COMMENT "Staging wasm64 example .wasm into site/playground/")
