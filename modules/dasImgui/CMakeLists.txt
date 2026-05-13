cmake_minimum_required(VERSION 3.16)
project(dasImgui CXX C)

# DASLANG_DIR must point to the daslang SDK root
if(NOT DEFINED DASLANG_DIR)
    message(FATAL_ERROR "DASLANG_DIR must be set to the daslang SDK root")
endif()

SET(DAS_IMGUI_DIR ${CMAKE_CURRENT_LIST_DIR})

# imgui via FetchContent
include(FetchContent)
SET(IMGUI_VERSION "1.90.6-docking")
FetchContent_Declare(imgui_src
    URL https://github.com/ocornut/imgui/archive/refs/tags/v${IMGUI_VERSION}.tar.gz
    URL_HASH SHA256=fc7f81d009ef718917aee0ac3ea1c74c8a5cfc8016049ad153b4d91d302b8aef
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

# GLFW (from main repo or sibling module)
if(EXISTS ${DASLANG_DIR}/modules/dasGlfw/glfw)
    SET(GLFW_INCLUDE_DIR ${DASLANG_DIR}/modules/dasGlfw/glfw/$<CONFIG>/include)
else()
    message(FATAL_ERROR "GLFW not found at ${DASLANG_DIR}/modules/dasGlfw/glfw")
endif()

# imgui source files
SET(DAS_IMGUI_SRC
    ${imgui_INCLUDE_DIR}/imgui.cpp
    ${imgui_INCLUDE_DIR}/imgui_demo.cpp
    ${imgui_INCLUDE_DIR}/imgui_draw.cpp
    ${imgui_INCLUDE_DIR}/imgui_tables.cpp
    ${imgui_INCLUDE_DIR}/imgui_widgets.cpp
)

# binding source files
SET(DAS_IMGUI_BIND_SRC
    ${DAS_IMGUI_DIR}/src/dasIMGUI.cpp
    ${DAS_IMGUI_DIR}/src/dasIMGUI.main.cpp
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
)

# imgui app source files
SET(DAS_IMGUI_APP_SRC
    ${DAS_IMGUI_DIR}/src/module_imgui_app.cpp
    ${imgui_INCLUDE_DIR}/backends/imgui_impl_glfw.cpp
    ${imgui_INCLUDE_DIR}/backends/imgui_impl_opengl3.cpp
    ${DAS_IMGUI_DIR}/gl3w/GL/gl3w.cpp
)

IF(WIN32)
    SET(IMGUI_PUBLIC_ATTR "__declspec\(dllexport\)")
ELSE()
    SET(IMGUI_PUBLIC_ATTR "__attribute__\(\(visibility\(\"default\"\)\)\)")
ENDIF()

# dasModuleImgui.shared_module — core imgui bindings
add_library(dasModuleImgui SHARED ${DAS_IMGUI_BIND_SRC} ${DAS_IMGUI_SRC})
target_compile_features(dasModuleImgui PRIVATE cxx_std_17)
target_compile_definitions(dasModuleImgui PRIVATE
    DAS_MOD_EXPORTS DAS_ENABLE_DLL=1
    IMGUI_DISABLE_OBSOLETE_FUNCTIONS=1
    IMGUI_API=${IMGUI_PUBLIC_ATTR}
    $<$<CONFIG:Release>:DAS_NO_ASSERTIONS>
    $<$<CONFIG:MinSizeRel>:DAS_NO_ASSERTIONS>
    $<$<CONFIG:RelWithDebInfo>:DAS_NO_ASSERTIONS>
)
target_include_directories(dasModuleImgui PRIVATE
    ${DASLANG_DIR}/include
    ${imgui_INCLUDE_DIR}
    ${DAS_IMGUI_DIR}/src
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
        IMGUI_DISABLE_OBSOLETE_FUNCTIONS=1
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
