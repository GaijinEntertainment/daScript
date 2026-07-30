# dasImgui builds three ways from a single CMakeLists:
#  - included from a daslang superbuild (the ADD_MODULE_* macros exist): link the
#    three modules statically into the host app via the convention path -- how a
#    daScript app that embeds the runtime consumes it, and how the fat-man
#    static-link example exercises every external module.
#  - configured directly for desktop (top-level project, DASLANG_DIR set): the
#    standalone .shared_module build CI, the daspkg index, and -load_module
#    consumers run on.
#  - configured directly for emscripten: static wasm archives for daspkg release
#    wasm and the interpreter-in-wasm playground.
IF(NOT COMMAND ADD_MODULE_CPP)
    cmake_minimum_required(VERSION 3.16)
    project(dasImgui CXX C)
ENDIF()

IF((NOT DAS_IMGUI_INCLUDED) AND ((NOT ${DAS_IMGUI_DISABLED}) OR (NOT DEFINED DAS_IMGUI_DISABLED)))
SET(DAS_IMGUI_INCLUDED TRUE)
MESSAGE(STATUS "dasImgui module included.")

IF(NOT COMMAND ADD_MODULE_CPP)
    # DASLANG_DIR must point to the daslang SDK root (standalone only -- in a
    # superbuild this file IS part of the daslang tree)
    if(NOT DEFINED DASLANG_DIR)
        message(FATAL_ERROR "DASLANG_DIR must be set to the daslang SDK root")
    endif()
    SET(DAS_IMGUI_GLFW_ROOT ${DASLANG_DIR}/modules/dasGlfw)
    SET(DAS_IMGUI_CLIPBOARD_ROOT ${DASLANG_DIR}/modules/dasClipboard)
ELSE()
    # sibling module in the superbuild tree (CMAKE_CURRENT_LIST_DIR, not
    # CMAKE_SOURCE_DIR/modules/...: the web build's top-level source dir is web/)
    SET(DAS_IMGUI_GLFW_ROOT ${CMAKE_CURRENT_LIST_DIR}/../dasGlfw)
    SET(DAS_IMGUI_CLIPBOARD_ROOT ${CMAKE_CURRENT_LIST_DIR}/../dasClipboard)
ENDIF()

IF(NOT EXISTS ${DAS_IMGUI_CLIPBOARD_ROOT}/src/aot_builtin_clipboard.h)
    MESSAGE(FATAL_ERROR "dasImgui requires the dasClipboard module")
ENDIF()

IF(NOT COMMAND ADD_MODULE_CPP AND NOT EMSCRIPTEN)
    ADD_LIBRARY(dasClipboardNative SHARED IMPORTED)
    IF(WIN32)
        FIND_LIBRARY(DAS_IMGUI_CLIPBOARD_IMPORT_LIBRARY
            NAMES dasModuleClipboard
            PATHS
                ${DASLANG_DIR}/lib/Release
                ${DASLANG_DIR}/lib
                ${DASLANG_DIR}/build/modules/dasClipboard
                ${DASLANG_DIR}/build/modules/dasClipboard/Release
                ${DAS_IMGUI_CLIPBOARD_ROOT}
            NO_DEFAULT_PATH)
        IF(NOT DAS_IMGUI_CLIPBOARD_IMPORT_LIBRARY)
            MESSAGE(FATAL_ERROR "dasImgui requires a built dasModuleClipboard import library")
        ENDIF()
        SET_TARGET_PROPERTIES(dasClipboardNative PROPERTIES
            IMPORTED_IMPLIB ${DAS_IMGUI_CLIPBOARD_IMPORT_LIBRARY}
            IMPORTED_LOCATION ${DAS_IMGUI_CLIPBOARD_ROOT}/dasModuleClipboard.shared_module)
    ELSE()
        SET_TARGET_PROPERTIES(dasClipboardNative PROPERTIES
            IMPORTED_LOCATION ${DAS_IMGUI_CLIPBOARD_ROOT}/dasModuleClipboard.shared_module)
    ENDIF()
ENDIF()

SET(DAS_IMGUI_DIR ${CMAKE_CURRENT_LIST_DIR})
include(FetchContent)
SET(DAS_IMGUI_COMMON_DEFS
    IMGUI_DISABLE_OBSOLETE_FUNCTIONS=1
    IMGUI_ENABLE_FREETYPE=1
    IMGUI_USE_WCHAR32=1
)

# FreeType is the Dear ImGui-supported path for color-layered glyphs. Pin and
# build it with a minimal, deterministic dependency set: Segoe UI Emoji's
# COLR/CPAL outlines need none of the optional compressed-font/image libraries.
SET(FT_DISABLE_ZLIB ON CACHE BOOL "Use FreeType's internal zlib" FORCE)
SET(FT_DISABLE_BZIP2 ON CACHE BOOL "Disable FreeType bzip2 support" FORCE)
SET(FT_DISABLE_PNG ON CACHE BOOL "Disable FreeType PNG bitmap support" FORCE)
SET(FT_DISABLE_HARFBUZZ ON CACHE BOOL "Disable FreeType HarfBuzz auto-hinting" FORCE)
SET(FT_DISABLE_BROTLI ON CACHE BOOL "Disable FreeType WOFF2 support" FORCE)
SET(FT_ENABLE_ERROR_STRINGS OFF CACHE BOOL "Disable FreeType error strings" FORCE)
FetchContent_Declare(freetype_src
    URL
        https://sourceforge.net/projects/freetype/files/freetype2/2.14.3/freetype-2.14.3.tar.xz/download
        https://download.savannah.gnu.org/releases/freetype/freetype-2.14.3.tar.xz
    URL_HASH SHA256=36bc4f1cc413335368ee656c42afca65c5a3987e8768cc28cf11ba775e785a5f
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_MakeAvailable(freetype_src)
SET_TARGET_PROPERTIES(freetype PROPERTIES POSITION_INDEPENDENT_CODE ON)

# MD4C is the semantic Markdown parser used by the view-independent model in
# markdown/. Pull only its parser/entity C translation units instead of importing the
# upstream CMake project (which also builds HTML/CLI targets and changes global
# compiler flags). Keeping it inside dasModuleImgui avoids another runtime DLL.
FetchContent_Declare(md4c_src
    URL https://github.com/mity/md4c/archive/refs/tags/release-0.5.3.tar.gz
    URL_HASH SHA256=353c346f376b87c954a13f3415ede2d51264cc61dc5abcd38ff1d2aa0d059b9e
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
    # Populate without adding MD4C's own CMake project. Its top-level build
    # changes compiler flags and creates HTML/CLI targets we do not use.
    SOURCE_SUBDIR __dasimgui_source_only
)
FetchContent_MakeAvailable(md4c_src)
SET(MD4C_INCLUDE_DIR ${md4c_src_SOURCE_DIR}/src)

# imgui via FetchContent
SET(IMGUI_VERSION "1.92.6-docking")
FetchContent_Declare(imgui_src
    URL https://github.com/ocornut/imgui/archive/refs/tags/v${IMGUI_VERSION}.tar.gz
    URL_HASH SHA256=5e84cdaa6a6041586a0d11a3071b749734a0439d66fdbdad37ae5b27e37d396c
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
    SOURCE_DIR ${DAS_IMGUI_DIR}/imgui
)
FetchContent_MakeAvailable(imgui_src)

# Patch: add data() method to ImVector (needed by daScript's ast_handle.h ManagedVectorAnnotation)
FILE(READ ${DAS_IMGUI_DIR}/imgui/imgui.h IMGUI_H_CONTENT)
IF(NOT IMGUI_H_CONTENT MATCHES "inline T\\*           data\\(\\)")
    STRING(REPLACE
        "inline T*           end()                               { return Data + Size; }"
        "inline T*           data()                              { return Data; }\n    inline const T*     data() const                        { return Data; }\n    inline T*           end()                               { return Data + Size; }"
        IMGUI_H_CONTENT "${IMGUI_H_CONTENT}")
    FILE(WRITE ${DAS_IMGUI_DIR}/imgui/imgui.h "${IMGUI_H_CONTENT}")
    MESSAGE(STATUS "Patched imgui.h: added data() method to ImVector")
ENDIF()

SET(imgui_INCLUDE_DIR ${DAS_IMGUI_DIR}/imgui)

# GLFW headers. Superbuild: dasGlfw's CMakeLists (included earlier -- the modules
# glob is alphabetical) already set GLFW_INCLUDE_DIR for the ExternalProject it
# drives. Desktop standalone: the built GLFW 3.4 include from the sibling dasGlfw
# module. Web standalone: the vendored 3.4 headers in dasGlfw/web_include (no GLFW
# source build; emcc's -sUSE_GLFW=3 supplies the symbols at the final app link).
IF(COMMAND ADD_MODULE_CPP)
    IF(NOT DEFINED GLFW_INCLUDE_DIR)
        MESSAGE(FATAL_ERROR "dasImgui superbuild: dasGlfw must be included first (GLFW_INCLUDE_DIR unset)")
    ENDIF()
ELSEIF(EMSCRIPTEN)
    SET(GLFW_INCLUDE_DIR ${DAS_IMGUI_GLFW_ROOT}/web_include)
ELSEIF(EXISTS ${DAS_IMGUI_GLFW_ROOT}/glfw)
    SET(GLFW_INCLUDE_DIR ${DAS_IMGUI_GLFW_ROOT}/glfw/$<CONFIG>/include)
ELSE()
    message(FATAL_ERROR "GLFW not found at ${DAS_IMGUI_GLFW_ROOT}/glfw")
ENDIF()

# imgui source files
SET(DAS_IMGUI_SRC
    ${md4c_src_SOURCE_DIR}/src/md4c.c
    ${md4c_src_SOURCE_DIR}/src/entity.c
    ${imgui_INCLUDE_DIR}/imgui.cpp
    ${imgui_INCLUDE_DIR}/imgui_demo.cpp
    ${imgui_INCLUDE_DIR}/imgui_draw.cpp
    ${imgui_INCLUDE_DIR}/imgui_tables.cpp
    ${imgui_INCLUDE_DIR}/imgui_widgets.cpp
    ${imgui_INCLUDE_DIR}/misc/freetype/imgui_freetype.cpp
)

# binding source files
SET(DAS_IMGUI_BIND_SRC
    ${DAS_IMGUI_DIR}/src/dasIMGUI.cpp
    ${DAS_IMGUI_DIR}/src/dasIMGUI.main.cpp
    ${DAS_IMGUI_DIR}/src/dasIMGUI.markdown.cpp
    ${DAS_IMGUI_DIR}/src/dasIMGUI.func_1.cpp
    ${DAS_IMGUI_DIR}/src/dasIMGUI.func_2.cpp
    ${DAS_IMGUI_DIR}/src/dasIMGUI.func_3.cpp
    ${DAS_IMGUI_DIR}/src/dasIMGUI.func_4.cpp
    ${DAS_IMGUI_DIR}/src/dasIMGUI.func_5.cpp
    ${DAS_IMGUI_DIR}/src/dasIMGUI.func_6.cpp
    ${DAS_IMGUI_DIR}/src/dasIMGUI.func_7.cpp
    ${DAS_IMGUI_DIR}/src/dasIMGUI.func_8.cpp
    ${DAS_IMGUI_DIR}/src/dasIMGUI.func_9.cpp
    ${DAS_IMGUI_DIR}/src/dasIMGUI.func_10.cpp
    ${DAS_IMGUI_DIR}/src/dasIMGUI.func_11.cpp
    ${DAS_IMGUI_DIR}/src/dasIMGUI.func_12.cpp
    ${DAS_IMGUI_DIR}/src/dasIMGUI.func_13.cpp
    ${DAS_IMGUI_DIR}/src/dasIMGUI.func_14.cpp
    ${DAS_IMGUI_DIR}/src/dasIMGUI.func_15.cpp
    ${DAS_IMGUI_DIR}/src/dasIMGUI.func_16.cpp
    ${DAS_IMGUI_DIR}/src/dasIMGUI.func_17.cpp
    ${DAS_IMGUI_DIR}/src/dasIMGUI.func_18.cpp
    ${DAS_IMGUI_DIR}/src/dasIMGUI.func_19.cpp
    ${DAS_IMGUI_DIR}/src/dasIMGUI.func_20.cpp
    ${DAS_IMGUI_DIR}/src/dasIMGUI.func_21.cpp
    ${DAS_IMGUI_DIR}/src/dasIMGUI.func_22.cpp
    ${DAS_IMGUI_DIR}/src/dasIMGUI.func_23.cpp
    ${DAS_IMGUI_DIR}/src/dasIMGUI.func_24.cpp
    ${DAS_IMGUI_DIR}/src/dasIMGUI.func_25.cpp
    ${DAS_IMGUI_DIR}/src/dasIMGUI.func_26.cpp
    ${DAS_IMGUI_DIR}/src/dasIMGUI.func_27.cpp
    ${DAS_IMGUI_DIR}/src/dasIMGUI.func_28.cpp
    ${DAS_IMGUI_DIR}/src/dasIMGUI.func_29.cpp
    ${DAS_IMGUI_DIR}/src/dasIMGUI.func_30.cpp
    ${DAS_IMGUI_DIR}/src/dasIMGUI.func_31.cpp
    ${DAS_IMGUI_DIR}/src/dasIMGUI.func_32.cpp
    ${DAS_IMGUI_DIR}/src/dasIMGUI.func_33.cpp
    ${DAS_IMGUI_DIR}/src/dasIMGUI.func_34.cpp
)

# imgui app source files. gl3w is desktop-only: its loader source pulls desktop GL
# headers that are fatal under emscripten, where GL comes from -sFULL_ES3 at the app
# link instead (the pure-das opengl_imgui_driver does the GL).
SET(DAS_IMGUI_APP_SRC
    ${DAS_IMGUI_DIR}/src/module_imgui_app.cpp
    ${imgui_INCLUDE_DIR}/backends/imgui_impl_glfw.cpp
)
IF(NOT EMSCRIPTEN)
    LIST(APPEND DAS_IMGUI_APP_SRC ${DAS_IMGUI_DIR}/gl3w/GL/gl3w.cpp)
ENDIF()

IF(COMMAND ADD_MODULE_CPP)
    # ===== daslang superbuild: static-embed =====
    # Static libs link into the app via ${DAS_MODULES_LIBS}; ADD_MODULE_CPP emits
    # NEED/DECLARE/PULL(Module_*) so the app needs no manual registration, and each
    # das source sections register boost paths so `require imgui/<name>` resolves
    # in-tree. File stem == module name in each section. `text` contains the
    # view-independent layout foundation; `markdown` contains its ImGui-facing
    # consumers without flattening those subsystems into widgets/.
    ADD_MODULE_CPP(dasIMGUI)
    ADD_MODULE_CPP(imgui_app)
    ADD_MODULE_CPP(imgui_app_headless)
    FILE(GLOB _imgui_widget_files "${DAS_IMGUI_DIR}/widgets/*.das")
    FOREACH(_f ${_imgui_widget_files})
        GET_FILENAME_COMPONENT(_n "${_f}" NAME_WE)
        ADD_MODULE_DAS(imgui widgets ${_n})
    ENDFOREACH()
    FILE(GLOB _imgui_text_files "${DAS_IMGUI_DIR}/text/*.das")
    FOREACH(_f ${_imgui_text_files})
        GET_FILENAME_COMPONENT(_n "${_f}" NAME_WE)
        ADD_MODULE_DAS(imgui text ${_n})
    ENDFOREACH()
    FILE(GLOB _imgui_markdown_files "${DAS_IMGUI_DIR}/markdown/*.das")
    FOREACH(_f ${_imgui_markdown_files})
        GET_FILENAME_COMPONENT(_n "${_f}" NAME_WE)
        ADD_MODULE_DAS(imgui markdown ${_n})
    ENDFOREACH()
    FILE(GLOB _imgui_terminal_files "${DAS_IMGUI_DIR}/terminal/*.das")
    FOREACH(_f ${_imgui_terminal_files})
        GET_FILENAME_COMPONENT(_n "${_f}" NAME_WE)
        ADD_MODULE_DAS(imgui terminal ${_n})
    ENDFOREACH()

    # Static halves only — no ADD_MODULE_LIB shared twins. In a superbuild the DLL
    # flavor comes from this module's own standalone build (daspkg); the twins would
    # need the whole dllexport/dllimport matrix (IMGUI_API across the pair, glfw
    # import libs) for nothing an embedded host ever loads.
    ADD_DAS_STATIC_MODULE_LIB(libDasModuleImgui ${DAS_IMGUI_BIND_SRC} ${DAS_IMGUI_SRC})
    ADD_DAS_STATIC_MODULE_LIB(libImguiApp ${DAS_IMGUI_APP_SRC})
    ADD_DAS_STATIC_MODULE_LIB(libImguiAppHeadless ${DAS_IMGUI_DIR}/src/module_imgui_app_headless.cpp)
    SET(_IMGUI_SUPER_TARGETS libDasModuleImgui libImguiApp libImguiAppHeadless)

    MACRO(SETUP_IMGUI imgui_mod_lib)
        TARGET_INCLUDE_DIRECTORIES(${imgui_mod_lib} PRIVATE
            ${imgui_INCLUDE_DIR}
            ${MD4C_INCLUDE_DIR}
            ${DAS_IMGUI_DIR}/src
            ${DAS_IMGUI_DIR}/gl3w
            ${GLFW_INCLUDE_DIR}
            ${DAS_IMGUI_GLFW_ROOT}/src
            ${DAS_IMGUI_CLIPBOARD_ROOT}/src
        )
        # Dear ImGui compile-time options are part of its public C++ ABI.
        # Publish them so superbuild consumers such as ImPlot use the same
        # ImWchar width and feature configuration as libDasModuleImgui.
        TARGET_COMPILE_DEFINITIONS(${imgui_mod_lib}
            PUBLIC ${DAS_IMGUI_COMMON_DEFS}
            PRIVATE
            IMGUI_IMPL_OPENGL_LOADER_GL3W
            # dasGlfw's binding TU owns typeFactory<GLFWwindow> in a superbuild link
            DAS_IMGUI_APP_EXTERN_GLFW_TYPE_FACTORY=1)
        SETUP_CPP11(${imgui_mod_lib})
    ENDMACRO()
    FOREACH(_t ${_IMGUI_SUPER_TARGETS})
        SETUP_IMGUI(${_t})
    ENDFOREACH()
    # the imgui core symbols live in libDasModuleImgui; the app backends use them
    TARGET_LINK_LIBRARIES(libImguiApp PUBLIC libDasModuleImgui)
    TARGET_LINK_LIBRARIES(libImguiAppHeadless PUBLIC libDasModuleImgui)
    TARGET_LINK_LIBRARIES(libDasModuleImgui PRIVATE freetype)
    TARGET_LINK_LIBRARIES(libDasModuleImgui PRIVATE libDasModuleClipboard)
    IF(NOT EMSCRIPTEN)
        FIND_PACKAGE(OpenGL REQUIRED)
        TARGET_LINK_LIBRARIES(libImguiApp PUBLIC ${OPENGL_LIBRARIES})
    ENDIF()

ELSEIF(EMSCRIPTEN)
    # ===== Web (emscripten) =====
    # dasImgui is an EXTERNAL module: built standalone here, not inside daslang's web
    # CMake, so the ADD_DAS_STATIC_MODULE_LIB helper is unavailable. Build the same
    # sources as plain STATIC archives -- the generated dasIMGUI.cpp's REGISTER_MODULE
    # provides register_Module_dasIMGUI, which a cross-compiled app's ensure_module
    # links. `daspkg release wasm` drives this through the .das_package
    # release_wasm_build hook and stages the liblib*.a into the wasm lib dir.
    #
    # gl3w is excluded: its desktop GL/glx.h is fatal under emscripten, and GL comes
    # from -sFULL_ES3 at the app link -- the pure-das opengl_imgui_driver does the GL.
    # Two ABI flavors, selected by cache vars (both default ON = the historical behavior):
    #   memory64 + pthread  -> the cross-compiled examples-card apps (daspkg release wasm,
    #                          which links the threaded daslang runtime). Archives in _wasm.
    #   wasm32 + single-thread -> linked into daslang_static (the interpreter-in-wasm
    #                          playground, a wasm32 single-thread monolith). Archives in _wasm32.
    # The flavors must not share an output dir (different ABI, same archive names), so the
    # output dir is suffixed by flavor. ABI knobs (memory64, atomics/shared memory) must match
    # whatever links the archive or the final wasm link fails.
    OPTION(DAS_IMGUI_WASM_MEMORY64 "Build the wasm archives for memory64 (wasm64)" ON)
    OPTION(DAS_IMGUI_WASM_PTHREADS "Build the wasm archives threaded (-pthread)" ON)
    MESSAGE(STATUS "dasImgui: emscripten/web build (3 module archives + FreeType; memory64=${DAS_IMGUI_WASM_MEMORY64} pthreads=${DAS_IMGUI_WASM_PTHREADS})")

    SET(IMGUI_WASM_FLAGS
        -fno-rtti
        -msimd128 -msse2 -mnontrapping-fptoint
        -fwasm-exceptions -sWASM_LEGACY_EXCEPTIONS=0)
    TARGET_COMPILE_OPTIONS(freetype PRIVATE
        -fwasm-exceptions -sWASM_LEGACY_EXCEPTIONS=0)
    IF(DAS_IMGUI_WASM_MEMORY64)
        LIST(APPEND IMGUI_WASM_FLAGS -sMEMORY64=1)
        TARGET_COMPILE_OPTIONS(freetype PRIVATE -sMEMORY64=1)
    ENDIF()
    IF(DAS_IMGUI_WASM_PTHREADS)
        LIST(APPEND IMGUI_WASM_FLAGS -pthread)
        TARGET_COMPILE_OPTIONS(freetype PRIVATE -pthread)
    ENDIF()
    # Output dir per ABI flavor (different ABI, same archive names — must not collide):
    #   _wasm     = memory64 (+pthread) : the cross-compiled examples-card apps
    #   _wasm32   = wasm32 single-thread : (unused now the interpreter is threaded)
    #   _wasm32mt = wasm32 + pthread     : linked into the threaded daslang_static interpreter
    IF(DAS_IMGUI_WASM_MEMORY64)
        SET(IMGUI_WASM_OUTDIR "${DAS_IMGUI_DIR}/_wasm")
    ELSEIF(DAS_IMGUI_WASM_PTHREADS)
        SET(IMGUI_WASM_OUTDIR "${DAS_IMGUI_DIR}/_wasm32mt")
    ELSE()
        SET(IMGUI_WASM_OUTDIR "${DAS_IMGUI_DIR}/_wasm32")
    ENDIF()
    SET(IMGUI_WASM_DEFS
        ${DAS_IMGUI_COMMON_DEFS}
        DAS_NO_ASSERTIONS DAS_ENABLE_EXCEPTIONS _EMSCRIPTEN_VER)
    SET(IMGUI_WASM_INC
        ${DASLANG_DIR}/include
        ${imgui_INCLUDE_DIR}
        ${MD4C_INCLUDE_DIR}
        ${DAS_IMGUI_DIR}/src
        ${DAS_IMGUI_CLIPBOARD_ROOT}/src
        ${DAS_IMGUI_DIR}/gl3w
        ${GLFW_INCLUDE_DIR}
        ${DASLANG_DIR}/modules/dasGlfw/src)

    # Stage archives at <module>/_wasm[32], named liblib<CppModule>.a to match daspkg's
    # wasm_archive_name() convention (the name the in-tree web build emits), so
    # release_wasm_archive() declarations resolve at the link step.
    MACRO(ADD_IMGUI_WASM_ARCHIVE tgt out)
        target_compile_features(${tgt} PRIVATE cxx_std_17)
        target_compile_options(${tgt} PRIVATE ${IMGUI_WASM_FLAGS})
        target_compile_definitions(${tgt} PRIVATE ${IMGUI_WASM_DEFS})
        target_include_directories(${tgt} PRIVATE ${IMGUI_WASM_INC})
        set_target_properties(${tgt} PROPERTIES
            PREFIX "" OUTPUT_NAME "${out}" SUFFIX ".a"
            ARCHIVE_OUTPUT_DIRECTORY "${IMGUI_WASM_OUTDIR}")
    ENDMACRO()

    add_library(dasModuleImgui STATIC ${DAS_IMGUI_BIND_SRC} ${DAS_IMGUI_SRC})
    ADD_IMGUI_WASM_ARCHIVE(dasModuleImgui liblibDasModuleImgui)
    target_link_libraries(dasModuleImgui PRIVATE freetype)

    add_library(imguiApp STATIC ${DAS_IMGUI_APP_SRC})
    ADD_IMGUI_WASM_ARCHIVE(imguiApp liblibImguiApp)

    add_library(imguiAppHeadless STATIC ${DAS_IMGUI_DIR}/src/module_imgui_app_headless.cpp)
    ADD_IMGUI_WASM_ARCHIVE(imguiAppHeadless liblibImguiAppHeadless)
ELSE() # ===== Desktop =====

IF(WIN32)
    SET(IMGUI_PUBLIC_ATTR "__declspec\(dllexport\)")
ELSE()
    SET(IMGUI_PUBLIC_ATTR "__attribute__\(\(visibility\(\"default\"\)\)\)")
ENDIF()

# dasModuleImgui.shared_module — core imgui bindings
add_library(dasModuleImgui SHARED ${DAS_IMGUI_BIND_SRC} ${DAS_IMGUI_SRC})
target_link_libraries(dasModuleImgui PRIVATE freetype dasClipboardNative)
target_compile_features(dasModuleImgui PRIVATE cxx_std_17)
target_compile_definitions(dasModuleImgui PRIVATE
    DAS_MOD_EXPORTS DAS_ENABLE_DLL=1 DAS_CLIPBOARD_IMPORTS=1
    ${DAS_IMGUI_COMMON_DEFS}
    IMGUI_API=${IMGUI_PUBLIC_ATTR}
    $<$<CONFIG:Release>:DAS_NO_ASSERTIONS>
    $<$<CONFIG:MinSizeRel>:DAS_NO_ASSERTIONS>
    $<$<CONFIG:RelWithDebInfo>:DAS_NO_ASSERTIONS>
)
target_include_directories(dasModuleImgui PRIVATE
    ${DASLANG_DIR}/include
    ${imgui_INCLUDE_DIR}
    ${MD4C_INCLUDE_DIR}
    ${DAS_IMGUI_DIR}/src
    ${DAS_IMGUI_CLIPBOARD_ROOT}/src
)
if(WIN32)
    if(EXISTS "${DASLANG_DIR}/lib/Release/libDaScriptDyn.lib")
        target_link_libraries(dasModuleImgui PRIVATE
            ${DASLANG_DIR}/lib/Release/libDaScriptDyn.lib
            ${DASLANG_DIR}/lib/Release/libDaScriptDyn_runtime.lib
        )
    else()
        target_link_libraries(dasModuleImgui PRIVATE
            ${DASLANG_DIR}/lib/libDaScriptDyn.lib
            ${DASLANG_DIR}/lib/libDaScriptDyn_runtime.lib
        )
    endif()
elseif(APPLE)
    target_link_libraries(dasModuleImgui PRIVATE
        ${DASLANG_DIR}/lib/liblibDaScriptDyn.dylib
        ${DASLANG_DIR}/lib/liblibDaScriptDyn_runtime.dylib
    )
else()
    target_link_libraries(dasModuleImgui PRIVATE
        ${DASLANG_DIR}/lib/liblibDaScriptDyn.so
        ${DASLANG_DIR}/lib/liblibDaScriptDyn_runtime.so
    )
endif()

MACRO(SETUP_OPTIONS target)
    if(MSVC)
        target_compile_options(${target} PRIVATE /wd4005 /bigobj
            $<$<CONFIG:Release>:/GR->
            $<$<CONFIG:MinSizeRel>:/GR->
            $<$<CONFIG:RelWithDebInfo>:/GR->)
    else()
        # runtime libs are built with -fno-rtti for optimized configs (see CMakeCommon.txt); match ABI
        target_compile_options(${target} PRIVATE
            $<$<CONFIG:Release>:-fno-rtti>
            $<$<CONFIG:MinSizeRel>:-fno-rtti>
            $<$<CONFIG:RelWithDebInfo>:-fno-rtti>)
    endif()
ENDMACRO()

SETUP_OPTIONS(dasModuleImgui)

set_target_properties(dasModuleImgui PROPERTIES
    SUFFIX ".shared_module"
    PREFIX ""
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${DAS_IMGUI_DIR}"
    LIBRARY_OUTPUT_DIRECTORY "${DAS_IMGUI_DIR}"
    ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${DAS_IMGUI_DIR}"
    ARCHIVE_OUTPUT_DIRECTORY "${DAS_IMGUI_DIR}"
)

# imguiApp.shared_module — GLFW + OpenGL3 backend
FIND_PACKAGE(OpenGL)
IF(OPENGL_FOUND)
    add_library(imguiApp SHARED ${DAS_IMGUI_APP_SRC})
    target_compile_features(imguiApp PRIVATE cxx_std_17)
    target_compile_definitions(imguiApp PRIVATE
        DAS_MOD_EXPORTS DAS_ENABLE_DLL=1
        ${DAS_IMGUI_COMMON_DEFS}
        IMGUI_API=${IMGUI_PUBLIC_ATTR}
        IMGUI_IMPL_API=${IMGUI_PUBLIC_ATTR}
        IMGUI_IMPL_OPENGL_LOADER_GL3W
        $<$<CONFIG:Release>:DAS_NO_ASSERTIONS>
        $<$<CONFIG:MinSizeRel>:DAS_NO_ASSERTIONS>
        $<$<CONFIG:RelWithDebInfo>:DAS_NO_ASSERTIONS>
    )
    target_include_directories(imguiApp PRIVATE
        ${DASLANG_DIR}/include
        ${imgui_INCLUDE_DIR}
        ${GLFW_INCLUDE_DIR}
        ${DAS_IMGUI_DIR}/gl3w
        ${DAS_IMGUI_DIR}/src
        ${DASLANG_DIR}/modules/dasGlfw/src
    )
    if(WIN32)
        if(EXISTS "${DASLANG_DIR}/lib/Release/libDaScriptDyn.lib")
            target_link_libraries(imguiApp PRIVATE
                ${DASLANG_DIR}/lib/$<CONFIG>/libDaScriptDyn.lib
                ${DASLANG_DIR}/lib/$<CONFIG>/libDaScriptDyn_runtime.lib
                ${DASLANG_DIR}/modules/dasGlfw/glfw_dyn/$<CONFIG>/lib/glfw3dll.lib
                ${OPENGL_LIBRARIES}
            )
        else()
            target_link_libraries(imguiApp PRIVATE
                ${DASLANG_DIR}/lib/libDaScriptDyn.lib
                ${DASLANG_DIR}/lib/libDaScriptDyn_runtime.lib
                ${DASLANG_DIR}/modules/dasGlfw/glfw_dyn/$<CONFIG>/lib/glfw3dll.lib
                ${OPENGL_LIBRARIES}
            )
        endif()
    elseif(APPLE)
        FIND_LIBRARY(COCOA_LIBRARY Cocoa REQUIRED)
        FIND_LIBRARY(COREFOUNDATION_LIBRARY CoreFoundation REQUIRED)
        target_link_libraries(imguiApp PRIVATE
            ${DASLANG_DIR}/lib/liblibDaScriptDyn.dylib
            ${DASLANG_DIR}/lib/liblibDaScriptDyn_runtime.dylib
            ${DASLANG_DIR}/modules/dasGlfw/glfw_dyn/$<CONFIG>/lib/libglfw.dylib
            ${OPENGL_LIBRARIES}
            ${COCOA_LIBRARY}
            ${COREFOUNDATION_LIBRARY}
        )
    else()
        target_link_libraries(imguiApp PRIVATE
            ${DASLANG_DIR}/lib/liblibDaScriptDyn.so
            ${DASLANG_DIR}/lib/liblibDaScriptDyn_runtime.so
            ${DASLANG_DIR}/modules/dasGlfw/glfw_dyn/$<CONFIG>/lib/libglfw.so
            ${OPENGL_LIBRARIES}
        )
        target_link_libraries(imguiApp PRIVATE X11)
    endif()
    SETUP_OPTIONS(imguiApp)
    set_target_properties(imguiApp PROPERTIES
        SUFFIX ".shared_module"
        PREFIX ""
        RUNTIME_OUTPUT_DIRECTORY_RELEASE "${DAS_IMGUI_DIR}"
        LIBRARY_OUTPUT_DIRECTORY "${DAS_IMGUI_DIR}"
    )
    # imguiApp needs imgui symbols
    target_link_libraries(imguiApp PRIVATE dasModuleImgui)
ELSE()
    MESSAGE(STATUS "OpenGL not found — skipping imguiApp module")
ENDIF()

# imguiAppHeadless.shared_module — display-less backend for CI / Playwright
# runs. Companion to imguiApp; no GLFW, no OpenGL, no gl3w. Used by
# imgui/imgui_harness when its --headless arm is active.
add_library(imguiAppHeadless SHARED ${DAS_IMGUI_DIR}/src/module_imgui_app_headless.cpp)
target_compile_features(imguiAppHeadless PRIVATE cxx_std_17)
target_compile_definitions(imguiAppHeadless PRIVATE
    DAS_MOD_EXPORTS DAS_ENABLE_DLL=1
    ${DAS_IMGUI_COMMON_DEFS}
    IMGUI_API=${IMGUI_PUBLIC_ATTR}
    $<$<CONFIG:Release>:DAS_NO_ASSERTIONS>
    $<$<CONFIG:MinSizeRel>:DAS_NO_ASSERTIONS>
    $<$<CONFIG:RelWithDebInfo>:DAS_NO_ASSERTIONS>
)
target_include_directories(imguiAppHeadless PRIVATE
    ${DASLANG_DIR}/include
    ${imgui_INCLUDE_DIR}
    ${DAS_IMGUI_DIR}/src
)
if(WIN32)
    if(EXISTS "${DASLANG_DIR}/lib/Release/libDaScriptDyn.lib")
        target_link_libraries(imguiAppHeadless PRIVATE
            ${DASLANG_DIR}/lib/$<CONFIG>/libDaScriptDyn.lib
            ${DASLANG_DIR}/lib/$<CONFIG>/libDaScriptDyn_runtime.lib
        )
    else()
        target_link_libraries(imguiAppHeadless PRIVATE
            ${DASLANG_DIR}/lib/libDaScriptDyn.lib
            ${DASLANG_DIR}/lib/libDaScriptDyn_runtime.lib
        )
    endif()
elseif(APPLE)
    target_link_libraries(imguiAppHeadless PRIVATE
        ${DASLANG_DIR}/lib/liblibDaScriptDyn.dylib
        ${DASLANG_DIR}/lib/liblibDaScriptDyn_runtime.dylib
    )
else()
    target_link_libraries(imguiAppHeadless PRIVATE
        ${DASLANG_DIR}/lib/liblibDaScriptDyn.so
        ${DASLANG_DIR}/lib/liblibDaScriptDyn_runtime.so
    )
endif()
SETUP_OPTIONS(imguiAppHeadless)
set_target_properties(imguiAppHeadless PROPERTIES
    SUFFIX ".shared_module"
    PREFIX ""
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${DAS_IMGUI_DIR}"
    LIBRARY_OUTPUT_DIRECTORY "${DAS_IMGUI_DIR}"
)
target_link_libraries(imguiAppHeadless PRIVATE dasModuleImgui)

ENDIF() # superbuild / emscripten / desktop

ENDIF() # DAS_IMGUI_INCLUDED guard
