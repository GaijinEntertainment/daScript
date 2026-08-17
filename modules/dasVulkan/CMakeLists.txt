# dasVulkan builds two ways from a single CMakeLists:
#  - included from a daslang superbuild (the ADD_MODULE_* macros exist): link
#    the module statically into the host app via the convention path -- how a
#    daScript app that embeds the runtime (ModuleGroup + FsFileAccess, no DLL
#    host) consumes it.
#  - configured directly (this is the top-level project, DASLANG_DIR set): the
#    standalone shared-module (.shared_module) build CI, the daspkg index, and
#    -load_module consumers run on.
IF(NOT COMMAND ADD_MODULE_CPP)
    cmake_minimum_required(VERSION 3.16)
    project(dasVulkan CXX C)
ENDIF()

IF((NOT DAS_VULKAN_INCLUDED) AND ((NOT ${DAS_VULKAN_DISABLED}) OR (NOT DEFINED DAS_VULKAN_DISABLED)))
    SET(DAS_VULKAN_INCLUDED TRUE)
    MESSAGE(STATUS "dasVulkan module included.")

    # Optional GLSL -> SPIR-V helper (DAS_VULKAN_COMPILE_SHADER) for apps that
    # author shaders in GLSL. Inert unless called, so the module build stays
    # free of any Vulkan SDK dependency. Available in both build modes.
    include(${CMAKE_CURRENT_LIST_DIR}/cmake/DasVulkanCompileShader.cmake)

    IF(COMMAND ADD_MODULE_CPP)
        # CMAKE_CURRENT_LIST_DIR, not ${CMAKE_SOURCE_DIR}/modules/dasVulkan: in the daslang
        # web build the top-level source dir is web/, so the CMAKE_SOURCE_DIR form would
        # resolve to web/modules/dasVulkan (same trap dasGlfw documents).
        SET(DAS_VULKAN_DIR ${CMAKE_CURRENT_LIST_DIR})
        SET(VULKAN_HEADERS_INCLUDE ${DAS_VULKAN_DIR}/vendor/Vulkan-Headers/include)
        SET(VOLK_DIR ${DAS_VULKAN_DIR}/vendor/volk)

        # Generated binding sources (produced by generator/generate.das). The
        # chunk count varies with the bound-surface size, so glob -- delete
        # src/dasVULKAN.gen_*.cpp before regenerating, then re-run CMake
        # configure so this glob re-evaluates.
        FILE(GLOB DAS_VULKAN_GEN_SRC "${DAS_VULKAN_DIR}/src/dasVULKAN.gen_*.cpp")
        SET(DAS_VULKAN_MODULE_SRC
            ${DAS_VULKAN_DIR}/src/dasVULKAN.main.cpp
            ${DAS_VULKAN_GEN_SRC}
            ${VOLK_DIR}/volk.c
        )
        IF(APPLE)
            # Objective-C++ TU bridging NSWindow -> CAMetalLayer for the MoltenVK
            # surface; isolated so Cocoa/QuartzCore stay out of the plain-C++ TUs.
            LIST(APPEND DAS_VULKAN_MODULE_SRC ${DAS_VULKAN_DIR}/src/dasVULKAN.metal.mm)
        ENDIF()

        # Static lib links into the app via ${DAS_MODULES_LIBS}; ADD_MODULE_CPP
        # emits NEED/DECLARE/PULL(Module_dasVULKAN) so the app needs no manual
        # registration, and each ADD_MODULE_DAS registers a boost path so
        # `require vulkan/<name>` resolves in-tree (getDasRoot()/modules/
        # dasVulkan/daslib/<name>.das).  KEEP IN SYNC with the
        # register_native_path list in .das_module (the DLL/daspkg flow) —
        # every new daslib file has these two registration points.
        ADD_MODULE_CPP(dasVULKAN)
        ADD_MODULE_DAS(vulkan daslib vulkan_runtime)
        ADD_MODULE_DAS(vulkan daslib vulkan_ctors)
        ADD_MODULE_DAS(vulkan daslib vulkan_handles)
        ADD_MODULE_DAS(vulkan daslib vulkan_structs)
        ADD_MODULE_DAS(vulkan daslib vulkan_commands)
        ADD_MODULE_DAS(vulkan daslib vulkan_cmds)
        ADD_MODULE_DAS(vulkan daslib vulkan_boost)
        ADD_MODULE_DAS(vulkan daslib vulkan_window)
        ADD_MODULE_DAS(vulkan daslib vulkan_reflect)
        ADD_MODULE_DAS(vulkan daslib vulkan_lint_data)
        ADD_MODULE_DAS(vulkan daslib vulkan_lint)
        ADD_MODULE_DAS(vulkan daslib spirv_vulkan_shader)
        ADD_MODULE_DAS(vulkan daslib vulkan_assets)
        ADD_MODULE_DAS(vulkan daslib vulkan_live)
        ADD_MODULE_DAS(vulkan daslib vulkan_imgui_driver)
        ADD_MODULE_DAS(vulkan daslib vulkan_imgui_app)
        # static half only — no ADD_MODULE_LIB shared twin: in a superbuild the DLL
        # flavor comes from this module's own standalone build (daspkg); the twin is
        # dead weight an embedded host never loads (and the imgui-family externals
        # can't even link theirs without a dllexport matrix)
        ADD_DAS_STATIC_MODULE_LIB(libDasModuleVulkan ${DAS_VULKAN_MODULE_SRC})

        MACRO(SETUP_VULKAN vulkan_mod_lib)
            TARGET_INCLUDE_DIRECTORIES(${vulkan_mod_lib} PRIVATE
                ${VULKAN_HEADERS_INCLUDE}
                ${VOLK_DIR}
                ${DAS_VULKAN_DIR}/src
            )
            TARGET_COMPILE_DEFINITIONS(${vulkan_mod_lib} PRIVATE VK_NO_PROTOTYPES)
            IF(WIN32)
                TARGET_COMPILE_DEFINITIONS(${vulkan_mod_lib} PRIVATE VK_USE_PLATFORM_WIN32_KHR)
            ELSEIF(APPLE)
                TARGET_COMPILE_DEFINITIONS(${vulkan_mod_lib} PRIVATE VK_USE_PLATFORM_METAL_EXT)
            ELSE()
                TARGET_COMPILE_DEFINITIONS(${vulkan_mod_lib} PRIVATE VK_USE_PLATFORM_XLIB_KHR VK_USE_PLATFORM_WAYLAND_KHR)
            ENDIF()
            IF(MSVC)
                TARGET_COMPILE_OPTIONS(${vulkan_mod_lib} PRIVATE /bigobj /wd4005)
            ENDIF()
            SETUP_CPP11(${vulkan_mod_lib})
        ENDMACRO()
        SETUP_VULKAN(libDasModuleVulkan)
        IF(APPLE)
            TARGET_LINK_LIBRARIES(libDasModuleVulkan PUBLIC "-framework Cocoa" "-framework QuartzCore")
        ENDIF()

        # Install the boost daslib so daspkg / DLL consumers resolve it from the
        # install tree; the static in-tree build resolves it via ADD_MODULE_DAS.
        FILE(GLOB DAS_VULKAN_DASLIB ${DAS_VULKAN_DIR}/daslib/*.das)
        INSTALL(FILES ${DAS_VULKAN_DASLIB}
            DESTINATION ${DAS_INSTALL_MODULESDIR}/dasVulkan/daslib
        )
    ELSE()
        # DASLANG_DIR must point to the daslang SDK root
        if(NOT DEFINED DASLANG_DIR)
            message(FATAL_ERROR "DASLANG_DIR must be set to the daslang SDK root")
        endif()

        SET(DAS_VULKAN_DIR ${CMAKE_CURRENT_LIST_DIR})
        SET(VULKAN_HEADERS_INCLUDE ${DAS_VULKAN_DIR}/vendor/Vulkan-Headers/include)
        SET(VOLK_DIR ${DAS_VULKAN_DIR}/vendor/volk)

        # generated binding sources (produced by generator/generate.das). The chunk count
        # varies with the bound-surface size, so glob — delete src/dasVULKAN.gen_*.cpp
        # before regenerating, then re-run CMake configure so this glob re-evaluates.
        file(GLOB DAS_VULKAN_GEN_SRC "${DAS_VULKAN_DIR}/src/dasVULKAN.gen_*.cpp")
        SET(DAS_VULKAN_SRC
            ${DAS_VULKAN_DIR}/src/dasVULKAN.main.cpp
            ${DAS_VULKAN_GEN_SRC}
            ${VOLK_DIR}/volk.c
        )
        if(APPLE)
            # Objective-C++ TU: bridges the NSWindow to a CAMetalLayer for the MoltenVK
            # surface. Isolated here so Cocoa/QuartzCore stay out of the plain-C++ TUs.
            list(APPEND DAS_VULKAN_SRC ${DAS_VULKAN_DIR}/src/dasVULKAN.metal.mm)
        endif()

        add_library(dasModuleVulkan SHARED ${DAS_VULKAN_SRC})
        target_compile_features(dasModuleVulkan PRIVATE cxx_std_17)
        target_compile_definitions(dasModuleVulkan PRIVATE
            DAS_MOD_EXPORTS DAS_ENABLE_DLL=1
            VK_NO_PROTOTYPES
            $<$<CONFIG:Release>:DAS_NO_ASSERTIONS>
            $<$<CONFIG:MinSizeRel>:DAS_NO_ASSERTIONS>
            $<$<CONFIG:RelWithDebInfo>:DAS_NO_ASSERTIONS>
        )
        if(WIN32)
            target_compile_definitions(dasModuleVulkan PRIVATE VK_USE_PLATFORM_WIN32_KHR)
        elseif(APPLE)
            target_compile_definitions(dasModuleVulkan PRIVATE VK_USE_PLATFORM_METAL_EXT)
        else()
            target_compile_definitions(dasModuleVulkan PRIVATE VK_USE_PLATFORM_XLIB_KHR VK_USE_PLATFORM_WAYLAND_KHR)
        endif()
        target_include_directories(dasModuleVulkan PRIVATE
            ${DASLANG_DIR}/include
            ${VULKAN_HEADERS_INCLUDE}
            ${VOLK_DIR}
            ${DAS_VULKAN_DIR}/src
        )
        if(WIN32)
            if(EXISTS "${DASLANG_DIR}/lib/Release/libDaScriptDyn.lib")
                target_link_libraries(dasModuleVulkan PRIVATE
                    ${DASLANG_DIR}/lib/Release/libDaScriptDyn.lib
                    ${DASLANG_DIR}/lib/Release/libDaScriptDyn_runtime.lib
                )
            else()
                target_link_libraries(dasModuleVulkan PRIVATE
                    ${DASLANG_DIR}/lib/libDaScriptDyn.lib
                    ${DASLANG_DIR}/lib/libDaScriptDyn_runtime.lib
                )
            endif()
        elseif(APPLE)
            target_link_libraries(dasModuleVulkan PRIVATE
                ${DASLANG_DIR}/lib/liblibDaScriptDyn.dylib
                ${DASLANG_DIR}/lib/liblibDaScriptDyn_runtime.dylib
                "-framework Cocoa"
                "-framework QuartzCore"
            )
        else()
            target_link_libraries(dasModuleVulkan PRIVATE
                ${DASLANG_DIR}/lib/liblibDaScriptDyn.so
                ${DASLANG_DIR}/lib/liblibDaScriptDyn_runtime.so
            )
        endif()
        if(MSVC)
            target_compile_options(dasModuleVulkan PRIVATE /wd4005 /bigobj)
        else()
            # daslang runtime libs are built -fno-rtti for optimized configs; match the ABI
            # or the .shared_module mislinks.
            target_compile_options(dasModuleVulkan PRIVATE
                $<$<CONFIG:Release>:-fno-rtti>
                $<$<CONFIG:MinSizeRel>:-fno-rtti>
                $<$<CONFIG:RelWithDebInfo>:-fno-rtti>)
        endif()
        set_target_properties(dasModuleVulkan PROPERTIES
            SUFFIX ".shared_module"
            PREFIX ""
            RUNTIME_OUTPUT_DIRECTORY_RELEASE "${DAS_VULKAN_DIR}"
            LIBRARY_OUTPUT_DIRECTORY "${DAS_VULKAN_DIR}"
        )
    ENDIF()
ENDIF()
