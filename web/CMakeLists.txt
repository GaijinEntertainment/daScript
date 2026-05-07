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

add_compile_options(-fwasm-exceptions)
add_link_options(-fwasm-exceptions)

add_compile_definitions(DAS_ENABLE_EXCEPTIONS)
add_compile_definitions(_EMSCRIPTEN_VER)

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
add_link_options(--embed-file ${CMAKE_CURRENT_SOURCE_DIR}/../daslib@daslib)

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

# Copy web UI assets to output at build time.
add_custom_target(copy_web_ui ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/ui
        ${DAS_WEB_OUTPUT_DIR}
    COMMENT "Copying web UI to ${DAS_WEB_OUTPUT_DIR}"
)
