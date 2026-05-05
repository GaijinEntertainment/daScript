cmake_minimum_required (VERSION 3.16)
project (DAS VERSION 0.6.2)
# enable_testing() must live at the top-level scope of the top-level
# CMakeLists.txt for `ninja test` / ctest to work from the build root.
# The actual add_test() calls live under tests-cpp/.
enable_testing()
include(GNUInstallDirs)

# for now we gonna run with exceptions enabled
#add_compile_definitions(DAS_ENABLE_EXCEPTIONS=1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
#set(CMAKE_CXX_EXTENSIONS OFF)

option(DAS_FLEX_BISON_DISABLED "Disable FLEX/BISON stage" OFF)
option(DAS_CLANG_BIND_DISABLED "Disable dasClangBind (libclang bindings, C/C++ parsing)" ON)
option(DAS_LLVM_DISABLED "Disable dasLLVM (llvm bindings)" ON)
option(DAS_HV_DISABLED "Disable dasHV (websokets,http server and client)" ON)
option(DAS_GLFW_DISABLED "Disable dasGLFW (GLFW window for graphics apps)" OFF)
option(DAS_AUDIO_DISABLED "Disable dasAudio (Miniaudio sound library)" OFF)
option(DAS_STDDLG_DISABLED "Disable dasStdDlg (File new,open,save etc dialogs)" OFF)
option(DAS_STBIMAGE_DISABLED "Disable dasStbImage (StbImage bindings, image loading and saving)" OFF)

option(DAS_PUGIXML_DISABLED "Disable dasPUGIXML (xml parsing library)" ON)
option(DAS_SQLITE_DISABLED "Disable dasSQLITE (sqlite3 library)" ON)
option(DAS_TOOLS_DISABLED "Disable dasTools" OFF)
option(DAS_TREE_SITTER_DISABLED "Disable tree-sitter-daslang grammar (no dependencies)" OFF)
option(DAS_AOT_EXAMPLES_DISABLED "Disable dasAOT examples" OFF)
option(DAS_TUTORIAL_DISABLED "Disable dasTutorial" OFF)
option(DAS_TESTS_DISABLED "Disable dasTests" OFF)
option(DAS_BUILD_DOCUMENTATION "Build HTML documentation via Sphinx during install" OFF)
option(DAS_ENABLE_PIC "Build static libs as position-independent code (UNIX, required to link into shared objects)" ON)
# RelWithDebInfo is permanently our heap-leak tracking / audit build.
# DAS_TRACK_ALLOC=1 (and the global new/delete override) are auto-enabled below
# via generator expressions; tracker source files are #if DAS_TRACK_ALLOC-gated
# so they compile to empty TUs in Release/Debug.
#
# Note on RelWithDebInfo inlining: CMakeCommon's SETUP_COMPILER hard-resets
# CMAKE_CXX_FLAGS_RELWITHDEBINFO without any explicit /Ob level. Because the
# flag string retains /O2 (added by MSBuild's RelWithDebInfo defaults), /O2
# implies /Ob2 per the MSVC docs, so codegen still matches Release inlining.
# Do not reintroduce an /Ob1 -> /Ob2 string(REPLACE) here -- CMakeCommon would
# overwrite it, making the replace dead code.
set(DAS_INSTALL_BINDIR bin CACHE STRING "Directory where to install binaries")
set(DAS_INSTALL_LIBDIR lib CACHE STRING "Directory where to install binaries")
set(DAS_INSTALL_DOCDIR . CACHE STRING "Directory where to install documentation")
set(DAS_INSTALL_DASLIBDIR daslib CACHE STRING "Directory where to install daslib")
set(DAS_INSTALL_EXAMPLESDIR examples CACHE STRING "Directory where to install examples")
set(DAS_INSTALL_MODULESDIR modules CACHE STRING "Directory where to install modules")
set(DAS_INSTALL_TUTORIALSDIR tutorials CACHE STRING "Directory where to install tutorial sources")
set(DAS_INSTALL_DASTESTDIR dastest CACHE STRING "Directory where to install dastest")

IF(WIN32)
    IF(DAS_LLVM_DISABLED)
        # FOR NOW DISABLE EXCEPTIONS AND USE SETJMP/LONGJUMP, IF JIT IS ENABLED
        add_compile_definitions(DAS_ENABLE_EXCEPTIONS=1)
    ENDIF()
ENDIF()

INCLUDE(./CMakeCommon.txt)

IF(DEFINED DAS_CONFIG_INCLUDE_DIR)
    INCLUDE_DIRECTORIES(${DAS_CONFIG_INCLUDE_DIR})
ENDIF()

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(EXECUTABLE_OUTPUT_PATH  ${PROJECT_SOURCE_DIR}/bin/)

SET(LINUX_UUID FALSE)

IF(NOT DAS_FLEX_BISON_DISABLED)

# On macOS, search Homebrew for keg-only versions of Bison and Flex. Xcode does
# not provide new enough versions for us to use.
if (CMAKE_HOST_SYSTEM_NAME MATCHES "Darwin")
    execute_process(
        COMMAND brew --prefix bison
        RESULT_VARIABLE BREW_BISON
        OUTPUT_VARIABLE BREW_BISON_PREFIX
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    if (BREW_BISON EQUAL 0 AND EXISTS "${BREW_BISON_PREFIX}")
        message(STATUS "Found Bison keg installed by Homebrew at ${BREW_BISON_PREFIX}")
        set(BISON_EXECUTABLE "${BREW_BISON_PREFIX}/bin/bison")
    endif()

    execute_process(
        COMMAND brew --prefix flex
        RESULT_VARIABLE BREW_FLEX
        OUTPUT_VARIABLE BREW_FLEX_PREFIX
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    if (BREW_FLEX EQUAL 0 AND EXISTS "${BREW_FLEX_PREFIX}")
        message(STATUS "Found Flex keg installed by Homebrew at ${BREW_FLEX_PREFIX}")
        set(FLEX_EXECUTABLE "${BREW_FLEX_PREFIX}/bin/flex")
    endif()
endif()

    FIND_FLEX_AND_BISON()
ENDIF()

SETUP_COMPILER()

IF(UNIX AND DAS_ENABLE_PIC)
    # On windows PIC is noop.
    set(CMAKE_POSITION_INDEPENDENT_CODE ON)
ENDIF()

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

FILE(GLOB DAS_AOT_DASLIB_DEPENDS CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/daslib/*.das")

MACRO(DAS_AOT_EXT input_files genList mainTarget dasAotTool dasAotToolArg)
    set(all_depends ${dasAotTool} ${PROJECT_SOURCE_DIR}/utils/aot/main.das ${DAS_AOT_DASLIB_DEPENDS})
    set(all_outputs "")
    set(command_args "")
    list(LENGTH input_files num_files)

    foreach(input ${input_files})
        get_filename_component(input_src ${input} ABSOLUTE)
        get_filename_component(input_dir ${input_src} DIRECTORY)
        get_filename_component(input_name ${input} NAME)
        if(${dasAotToolArg} STREQUAL "-ctx")
            set(out_dir ${input_dir}/_standalone_ctx_generated)
            set(out_arg "${input_dir}/_standalone_ctx_generated/")
            set(out_src "${input_dir}/_standalone_ctx_generated/${input_name}.cpp")
        else()
            set(out_dir ${input_dir}/_aot_generated)
            set(out_arg "${out_dir}/${mainTarget}_${input_name}.cpp")
            set(out_src "${out_dir}/${mainTarget}_${input_name}.cpp")
        endif()

        # Add to dependencies
        list(APPEND all_depends ${input_src})

        # Add to outputs
        list(APPEND all_outputs ${out_src})
        list(APPEND ${genList} ${out_src})

        # Build command arguments: tool_arg input1 output1 tool_arg input2 output2 ...
        list(APPEND command_args ${dasAotToolArg} ${input_src} ${out_arg})

        file(MAKE_DIRECTORY ${out_dir})
        set_source_files_properties(${out_src} PROPERTIES GENERATED TRUE)
    endforeach()

    ADD_CUSTOM_COMMAND(
        OUTPUT  ${all_outputs}
        DEPENDS ${all_depends}
        COMMENT "AOT compiling files..."
        COMMAND ${dasAotTool} ${PROJECT_SOURCE_DIR}/utils/aot/main.das -- ${command_args} ${CROSS_PLATFORM}
    )
    set(custom_name ${mainTarget}_genaot)
    ADD_CUSTOM_TARGET(${custom_name} DEPENDS ${all_outputs})
    ADD_DEPENDENCIES(${mainTarget} ${custom_name})
ENDMACRO()

MACRO(DAS_AOT input genList mainTarget dasAotTool)
    DAS_AOT_EXT("${input}" ${genList} ${mainTarget} ${dasAotTool} -aot)
ENDMACRO()

MACRO(DAS_AOT_STANDALONE input genList mainTarget dasAotTool dasAotToolArg)
    get_filename_component(input_src ${input} ABSOLUTE)
    get_filename_component(input_dir ${input_src} DIRECTORY)
    get_filename_component(input_name ${input} NAME)
    set(out_dir ${input_dir}/_standalone_ctx_generated)
    set(out_inc ${out_dir}/${input_name}.h)
    set(out_src ${out_dir}/${input_name}.cpp)
    get_filename_component(ctx_name ${input} NAME_WE)
    file(MAKE_DIRECTORY ${out_dir})
    ADD_CUSTOM_COMMAND(
            DEPENDS ${input_src} ${dasAotTool}
            OUTPUT  ${out_src}
            COMMENT "AOT precompiling ${input_src} -> ${out_src}"
            COMMAND ${dasAotTool} ${dasAotToolArg} ${input_src} ${out_dir} -cross-platform -standalone-context ${ctx_name} -standalone-class Standalone
    )
    list(APPEND ${genList} ${out_src})
    set(custom_name ${mainTarget}_${input_name}_standalone)
    ADD_CUSTOM_TARGET(${custom_name} DEPENDS ${out_src})
    set_source_files_properties(${out_src} PROPERTIES GENERATED TRUE)
    SET_TARGET_PROPERTIES(${custom_name} PROPERTIES FOLDER _${mainTarget}_aot)
    ADD_DEPENDENCIES(${mainTarget} ${custom_name})
ENDMACRO()

MACRO(DAS_AOT_CTX input genList mainTarget dasAotTool)
    DAS_AOT_EXT("${input}" ${genList} ${mainTarget} ${dasAotTool} -ctx)
ENDMACRO()

MACRO(DAS_AOT_LIB input genList mainTarget dasAotTool)
    DAS_AOT_EXT("${input}" ${genList} ${mainTarget} ${dasAotTool} -aotlib)
ENDMACRO()

SET(UNITZE_BUILD_BATCH_SIZE 10)

MACRO(UNITIZE_BUILD input_dir genList)
    set(unitList)
    set(files ${${genList}})
    set(out_dir "${PROJECT_SOURCE_DIR}/${input_dir}/_aot_generated")
    set_source_files_properties(${files} PROPERTIES HEADER_FILE_ONLY true)
    set(unit_build_file "")
    set(file_index 0)
    set(batch_index 0)
    foreach(source_file ${files} )
        if(file_index EQUAL "0")
            set(unit_build_file "${out_dir}/unity_build_${batch_index}.cpp")
            FILE(WRITE ${unit_build_file} "// unity build batch ${batch_index}\n")
            list(APPEND unitList ${unit_build_file})
        endif()
        get_filename_component(input_name ${source_file} NAME)
        FILE(APPEND ${unit_build_file} "#include \"${input_name}\"\n")
        math(EXPR file_index "${file_index} + 1")
        if(file_index EQUAL ${UNITZE_BUILD_BATCH_SIZE})
            set(file_index 0)
            math(EXPR batch_index "${batch_index} + 1")
        endif()
    endforeach()
    list(APPEND ${genList} ${unitList})
ENDMACRO()

FIND_PROGRAM(CLANG_BIN_EXE clang)
IF(CLANG_BIN_EXE)
    MACRO(DAS_CPP_BIND_AST tgt generate_das header_from prefix src_dir inc_dirs extras)
        MESSAGE(STATUS "Generating C++ bindings target ${tgt} from ${header_from}")
        ADD_CUSTOM_TARGET(${tgt})
        ADD_DEPENDENCIES(${tgt} daslang)
        ADD_CUSTOM_COMMAND(
            TARGET ${tgt}
            WORKING_DIRECTORY ${src_dir}
            VERBATIM
            COMMAND ${CLANG_BIN_EXE} -Xclang -ast-dump=json -x c++ -c ${header_from} -I${inc_dirs} > ${header_from}.json 2> ${header_from}.json.log || cmd /c exit /b 0
            COMMENT "Generating JSON AST for ${header_from} to ${header_from}.json\ninclude directories are ${inc_dirs}\n"
        )
        ADD_CUSTOM_COMMAND(
            TARGET ${tgt}
            WORKING_DIRECTORY ${src_dir}
            VERBATIM
            COMMAND daslang ${generate_das} -args ${header_from}.json ${prefix} "${extras}"
            COMMENT "Generating C++ bindings from ${header_from}.json\n"
        )
    ENDMACRO()

    MACRO(DAS_OUTPUT_AST tgt header_from src_dir inc_dirs)
        MESSAGE(STATUS "Generating C++ bindings target ${tgt} from ${header_from}")
        ADD_CUSTOM_TARGET(${tgt})
        ADD_DEPENDENCIES(${tgt} daslang)
        ADD_CUSTOM_COMMAND(
            TARGET ${tgt}
            WORKING_DIRECTORY ${src_dir}
            VERBATIM
            COMMAND ${CLANG_BIN_EXE} -Xclang -ast-dump -x c++ -c ${header_from} -I${inc_dirs} > ${header_from}.ast 2> ${header_from}.ast.log || cmd /c exit /b 0
            COMMENT "Generating JSON AST for ${header_from} to ${header_from}.json\ninclude directoryes are ${inc_dirs}\n"
        )
    ENDMACRO()

    MACRO(DAS_OUTPUT_JSON tgt header_from src_dir inc_dirs)
        MESSAGE(STATUS "Generating C++ bindings target ${tgt} from ${header_from}")
        ADD_CUSTOM_TARGET(${tgt})
        ADD_DEPENDENCIES(${tgt} daslang)
        ADD_CUSTOM_COMMAND(
            TARGET ${tgt}
            WORKING_DIRECTORY ${src_dir}
            VERBATIM
            COMMAND ${CLANG_BIN_EXE} -Xclang -ast-dump=json -x c++ -c ${header_from} -I${inc_dirs} > ${header_from}.ast 2> ${header_from}.ast.log || cmd /c exit /b 0
            COMMENT "Generating JSON AST for ${header_from} to ${header_from}.json\ninclude directoryes are ${inc_dirs}\n"
        )
    ENDMACRO()


ELSE()
    MACRO(DAS_CPP_BIND_AST tgt generate_das header_from prefix src_dir inc_dirs)
        MESSAGE(STATUS "Skipping C++ bindings from ${header_from}")
    ENDMACRO()

    MACRO(DAS_OUTPUT_AST tgt header_from src_dir inc_dirs)
        MESSAGE(STATUS "Skipping ast dump from from ${header_from}")
    ENDMACRO()

ENDIF()

# All shared libraries should pass through this macro.
# It sets required properties for shared libraries.
MACRO(SETUP_DAS_SHARED_LIBRARY lib)
    ADD_LIBRARY(${lib} SHARED ${ARGN})
    SETUP_LIB_DEFINITIONS(${lib})
    if(WIN32)
        # We export a lot of structures, some of them has not exported internals.
        # Instead of marking everything as exported let's disable
        # warning and refactor our public API in future.
        TARGET_COMPILE_OPTIONS(${lib} PUBLIC /wd4251 /wd4661 /wd4275)
    else()
        SET_TARGET_PROPERTIES(${lib} PROPERTIES POSITION_INDEPENDENT_CODE ON)
    endif()
ENDMACRO()

MACRO(ADD_DAS_STATIC_LIBRARY lib)
    ADD_LIBRARY(${lib} STATIC ${ARGN})
    SETUP_LIB_DEFINITIONS(${lib})
ENDMACRO()


SET(NEED_MODULES_PATH "${CMAKE_CURRENT_BINARY_DIR}/include")

SET(DAS_MODULES_RESOLVE_INC ${NEED_MODULES_PATH}/modules/external_resolve.inc)
SET(DAS_MODULES_NEED_INC ${NEED_MODULES_PATH}/modules/external_need.inc)
SET(DAS_MODULES_DECLARE_INC ${NEED_MODULES_PATH}/modules/external_declare.inc)
SET(DAS_MODULES_PULL_INC ${NEED_MODULES_PATH}/modules/external_pull.inc)
FILE(WRITE ${DAS_MODULES_RESOLVE_INC}.temp "")
FILE(WRITE ${DAS_MODULES_NEED_INC}.temp "")
FILE(WRITE ${DAS_MODULES_DECLARE_INC}.temp "")
FILE(WRITE ${DAS_MODULES_PULL_INC}.temp "")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${EXECUTABLE_OUTPUT_PATH})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)

# All dynamic modules libraries will be added here (files like dasModuleAudio.shared_module).
SET(DAS_DYN_MODULES_LIBS)
# All static modules libraries will be added here (files like libDasModuleAudio.a).
SET(DAS_MODULES_LIBS)

SET(DAS_ALL_EXAMPLES)
SET(DAS_EXAMPLES_TO_RUN)
FILE(GLOB _modules RELATIVE "${PROJECT_SOURCE_DIR}/modules" CONFIGURE_DEPENDS "modules/*")

# Every dynamic module should pass through this macro.
MACRO(ADD_DAS_SHARED_MODULE_LIB module_name)
    MESSAGE("REGISTER DAS SHARED MODULE ${module_name}")
    SETUP_DAS_SHARED_LIBRARY(${module_name} ${ARGN})

    # extract from modules/dasLLVM -> dasLLVM
    SET(SHARED_OUTPUT_PATH "${CMAKE_CURRENT_LIST_DIR}/")
    get_filename_component(FOLDER_NAME "${CMAKE_CURRENT_LIST_DIR}" NAME)
    SET_TARGET_PROPERTIES(${module_name} PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY "${SHARED_OUTPUT_PATH}"
        RUNTIME_OUTPUT_DIRECTORY "${SHARED_OUTPUT_PATH}"
        RUNTIME_OUTPUT_DIRECTORY_DEBUG "${SHARED_OUTPUT_PATH}"
        RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL "${SHARED_OUTPUT_PATH}"
        RUNTIME_OUTPUT_DIRECTORY_RELEASE "${SHARED_OUTPUT_PATH}"
        RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${SHARED_OUTPUT_PATH}"
        SUFFIX ".shared_module"
        PREFIX ""
    )
    # Debug builds use _debug suffix so Debug and Release .shared_module can coexist
    SET_PROPERTY(TARGET ${module_name} PROPERTY OUTPUT_NAME_DEBUG "${module_name}_debug")
    # Ignore tutorial, profile, test.
    if(NOT "${FOLDER_NAME}" STREQUAL "tutorial" AND
       NOT "${FOLDER_NAME}" STREQUAL "profile" AND
       NOT "${FOLDER_NAME}" STREQUAL "test" AND
       NOT ${module_name} STREQUAL "dasModuleUnitTest"
       )
        install(TARGETS ${module_name}
            ARCHIVE DESTINATION ${DAS_INSTALL_MODULESDIR}/${FOLDER_NAME}
            RUNTIME DESTINATION ${DAS_INSTALL_MODULESDIR}/${FOLDER_NAME} OPTIONAL
            LIBRARY DESTINATION ${DAS_INSTALL_MODULESDIR}/${FOLDER_NAME} OPTIONAL
        )
        # Compiled dynamic module is useless without config.
        # Build only static lib or create .das_module file if no config exists.
        install(FILES "${CMAKE_CURRENT_LIST_DIR}/.das_module"
            DESTINATION ${DAS_INSTALL_MODULESDIR}/${FOLDER_NAME}
        )
    ENDIF()
    TARGET_COMPILE_DEFINITIONS(${module_name} PRIVATE DAS_MOD_EXPORTS DAS_ENABLE_DLL=1)
    TARGET_LINK_LIBRARIES(${module_name} PUBLIC libDaScriptDyn)
    ADD_DEPENDENCIES(${module_name} libDaScriptDyn)
    # Each shared module needs its own copy of the global new/delete overrides:
    # Windows resolves operator new per-module at link time, and a module that
    # allocates via the tracker but frees through the default CRT (or vice versa)
    # looks like a leak. The TU is #if DAS_TRACK_ALLOC-gated so it compiles empty
    # outside RelWithDebInfo. DAS_TRACK_ALLOC propagates from libDaScriptDyn_runtime.
    target_sources(${module_name} PRIVATE ${PROJECT_SOURCE_DIR}/src/misc/alloc_tracker_overrides.cpp)
    LIST(APPEND DAS_DYN_MODULES_LIBS ${module_name})
ENDMACRO()

# Every static module should pass through this macro.
MACRO(ADD_DAS_STATIC_MODULE_LIB lib)
    MESSAGE("REGISTER DAS STATIC MODULE ${lib}")
    ADD_DAS_STATIC_LIBRARY(${lib} ${ARGN})
    TARGET_LINK_LIBRARIES(${lib} PUBLIC libDaScript)
    # Compiled dynamic module is useless without config.
    # Build only static lib or create .das_module file if no config exists.
    get_filename_component(FOLDER_NAME "${CMAKE_CURRENT_LIST_DIR}" NAME)
    if(NOT "${FOLDER_NAME}" STREQUAL "tutorial" AND
       NOT "${FOLDER_NAME}" STREQUAL "profile" AND
       NOT "${FOLDER_NAME}" STREQUAL "test")
        install(TARGETS
            ${lib}
            EXPORT DASTargets
            RUNTIME DESTINATION ${DAS_INSTALL_BINDIR}
            LIBRARY DESTINATION ${DAS_INSTALL_LIBDIR}
            ARCHIVE DESTINATION ${DAS_INSTALL_LIBDIR}
            INCLUDES DESTINATION include
        )
    ENDIF()

    LIST(APPEND DAS_MODULES_LIBS ${lib})
ENDMACRO()

# If you need both static and dynamic call this.
# module_name target should end on `_mod` and first part should
# be exactly `module_name`. Like `llvm_mod` for llvm module.
MACRO(ADD_MODULE_LIB lib module_name)
    ADD_DAS_STATIC_MODULE_LIB(${lib} ${ARGN})
    ADD_DAS_SHARED_MODULE_LIB(${module_name} ${ARGN})
ENDMACRO()
MACRO(ADD_MODULE_PUB_LIB lib module_name)
    ADD_MODULE_LIB(${lib} ${module_name} ${ARGN})
    SET_TARGET_PROPERTIES(${module_name} PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
ENDMACRO()

MACRO(ADD_EXAMPLE tgt)
    MESSAGE("EXAMPLE ${tgt}")
    LIST(APPEND DAS_ALL_EXAMPLES ${tgt})
ENDMACRO()

# Register a .das file to be run as a headless example check.
# Modules call this from their CMakeLists.txt for examples that
# can run without GUI, audio, or user interaction.
MACRO(ADD_EXAMPLE_RUN das_file)
    SET(_exe TRUE)
    IF("${ARGN}" STREQUAL "FALSE")
        SET(_exe FALSE)
    ENDIF()
    MESSAGE("EXAMPLE_RUN ${das_file} (exe=${_exe})")
    LIST(APPEND DAS_EXAMPLES_TO_RUN "${das_file}|${_exe}")
ENDMACRO()

ADD_EXAMPLE_RUN("${PROJECT_SOURCE_DIR}/examples/dasbind/dasbind_example.das")

FUNCTION(ADD_MODULE_CPP cpp)
    FILE(APPEND ${DAS_MODULES_NEED_INC}.temp "NEED_MODULE(Module_${cpp});\n")
    FILE(APPEND ${DAS_MODULES_DECLARE_INC}.temp "DECLARE_MODULE(Module_${cpp});\n")
    FILE(APPEND ${DAS_MODULES_PULL_INC}.temp "PULL_MODULE(Module_${cpp});\n")
ENDFUNCTION()

FUNCTION(ADD_MODULE_NATIVE native)
    FILE(APPEND ${DAS_MODULES_RESOLVE_INC}.temp "NATIVE_MODULE(\"daslib\",\"daslib\", ${_module}, ${native});\n")
ENDFUNCTION()

FUNCTION(ADD_MODULE_DAS category subfolder native)
    FILE(APPEND ${DAS_MODULES_RESOLVE_INC}.temp "NATIVE_MODULE(${category}, ${subfolder}, ${_module}, ${native});\n")
ENDFUNCTION()


FOREACH(_module ${_modules})
    # Skip daspkg-installed modules that build themselves (standalone CMake)
    IF(NOT EXISTS "${PROJECT_SOURCE_DIR}/modules/${_module}/.daspkg_standalone")
        INCLUDE(modules/${_module}/CMakeLists.txt OPTIONAL)
    ENDIF()
ENDFOREACH()

# Clean stale .shared_module files. When a module is disabled, its .shared_module
# from a previous build may remain and cause crashes at startup (the dynamic module
# loader tries to load them even though the module code is no longer linked).
FILE(GLOB_RECURSE _all_shared_modules CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/modules/*.shared_module")
FOREACH(_sm ${_all_shared_modules})
    GET_FILENAME_COMPONENT(_sm_name "${_sm}" NAME_WE)
    IF(NOT TARGET ${_sm_name})
        MESSAGE(STATUS "Removing stale ${_sm}")
        FILE(REMOVE "${_sm}")
    ENDIF()
ENDFOREACH()

FOREACH(_example ${DAS_ALL_EXAMPLES})
    MESSAGE("adding to ${_example} ${DAS_MODULES_LIBS}")
    TARGET_LINK_LIBRARIES(${_example} ${DAS_MODULES_LIBS})
    ADD_DEPENDENCIES(${_example} ${DAS_MODULES_LIBS})
ENDFOREACH()

# run_examples — runs all .das files registered via ADD_EXAMPLE_RUN.
# Each example is executed with `daslang <file>` and must exit cleanly.
#
# We generate a CMake -P script and run that from ADD_CUSTOM_TARGET so
# that program stdout is captured by execute_process() instead of being
# piped through MSBuild, which misinterprets lines containing "error:"
# as build errors.
IF(DAS_EXAMPLES_TO_RUN)
    SET(_script "${CMAKE_BINARY_DIR}/run_examples.cmake")
    FILE(WRITE ${_script} "# Auto-generated by CMakeLists.txt — do not edit\n")
    FOREACH(_entry ${DAS_EXAMPLES_TO_RUN})
        STRING(REPLACE "|" ";" _pair "${_entry}")
        LIST(GET _pair 0 _das)
        LIST(GET _pair 1 _do_exe)
        STRING(MD5 _path_hash ${_das})
        GET_FILENAME_COMPONENT(_example_name ${_das} NAME)
        SET(_out_path "${_path_hash}_${_example_name}")

        FILE(APPEND ${_script}
"message(STATUS \"Running ${_das}\")
execute_process(
    COMMAND \"\${DASLANG}\" \"${_das}\"
    WORKING_DIRECTORY \"\${WORKDIR}\"
    RESULT_VARIABLE _rc OUTPUT_VARIABLE _out ERROR_VARIABLE _err)
if(_rc)
    message(\"\${_out}\${_err}\")
    message(FATAL_ERROR \"${_das} failed (exit \${_rc})\")
endif()
")
        IF(_do_exe)
            FILE(APPEND ${_script}
"message(STATUS \"Compiling to \${EXEDIR}/${_out_path}\")
execute_process(
    COMMAND \"\${DASLANG}\" -exe \"${_das}\" -output \"\${EXEDIR}/${_out_path}\"
    WORKING_DIRECTORY \"\${WORKDIR}\"
    RESULT_VARIABLE _rc OUTPUT_VARIABLE _out ERROR_VARIABLE _err)
if(_rc)
    message(\"\${_out}\${_err}\")
    message(FATAL_ERROR \"Compiling ${_das} failed (exit \${_rc})\")
endif()
execute_process(
    COMMAND \"\${EXEDIR}/${_out_path}.exe\"
    WORKING_DIRECTORY \"\${WORKDIR}\"
    RESULT_VARIABLE _rc OUTPUT_VARIABLE _out ERROR_VARIABLE _err)
if(_rc)
    message(\"\${_out}\${_err}\")
    message(FATAL_ERROR \"Running compiled ${_out_path}.exe failed (exit \${_rc})\")
endif()
")
        ENDIF()
    ENDFOREACH()
    ADD_CUSTOM_TARGET(run_examples
        COMMAND ${CMAKE_COMMAND}
            -DDASLANG=$<TARGET_FILE:daslang>
            -DEXEDIR=$<TARGET_FILE_DIR:daslang>
            -DWORKDIR=${PROJECT_SOURCE_DIR}
            -P ${_script}
        DEPENDS daslang
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
        COMMENT "Running headless module examples"
    )
ENDIF()

ADD_CUSTOM_COMMAND(
    DEPENDS ${DAS_MODULES_NEED_INC}.temp
    OUTPUT ${DAS_MODULES_NEED_INC}
    VERBATIM
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${DAS_MODULES_NEED_INC}.temp  ${DAS_MODULES_NEED_INC}
)

ADD_CUSTOM_COMMAND(
    DEPENDS ${DAS_MODULES_RESOLVE_INC}.temp
    OUTPUT ${DAS_MODULES_RESOLVE_INC}
    VERBATIM
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${DAS_MODULES_RESOLVE_INC}.temp  ${DAS_MODULES_RESOLVE_INC}
)

ADD_CUSTOM_COMMAND(
    DEPENDS ${DAS_MODULES_DECLARE_INC}.temp
    OUTPUT ${DAS_MODULES_DECLARE_INC}
    VERBATIM
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${DAS_MODULES_DECLARE_INC}.temp  ${DAS_MODULES_DECLARE_INC}
)

ADD_CUSTOM_COMMAND(
    DEPENDS ${DAS_MODULES_PULL_INC}.temp
    OUTPUT ${DAS_MODULES_PULL_INC}
    VERBATIM
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${DAS_MODULES_PULL_INC}.temp  ${DAS_MODULES_PULL_INC}
)

ADD_CUSTOM_TARGET(need_and_resolve ALL DEPENDS ${DAS_MODULES_NEED_INC} ${DAS_MODULES_RESOLVE_INC} ${DAS_MODULES_DECLARE_INC} ${DAS_MODULES_PULL_INC})

# libUriParser

SET(URIPARSER_SRCS
3rdparty/uriparser/src/UriCommon.c
3rdparty/uriparser/src/UriCompare.c
3rdparty/uriparser/src/UriEscape.c
3rdparty/uriparser/src/UriFile.c
3rdparty/uriparser/src/UriIp4.c
3rdparty/uriparser/src/UriIp4Base.c
3rdparty/uriparser/src/UriNormalize.c
3rdparty/uriparser/src/UriNormalizeBase.c
3rdparty/uriparser/src/UriParse.c
3rdparty/uriparser/src/UriParseBase.c
3rdparty/uriparser/src/UriQuery.c
3rdparty/uriparser/src/UriRecompose.c
3rdparty/uriparser/src/UriResolve.c
3rdparty/uriparser/src/UriShorten.c
3rdparty/uriparser/src/UriMemory.c
)

ADD_LIBRARY(libUriParser STATIC ${URIPARSER_SRCS})
ADD_LIBRARY(libUriParserDyn OBJECT ${URIPARSER_SRCS})
SETUP_CPP11(libUriParser)
SET_TARGET_PROPERTIES(libUriParserDyn PROPERTIES POSITION_INDEPENDENT_CODE ON)
target_compile_definitions(libUriParser PUBLIC URIPARSER_BUILD_CHAR)
target_compile_definitions(libUriParser PUBLIC URI_STATIC_BUILD)
target_include_directories(libUriParser PUBLIC
  $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/3rdparty/uriparser/include>
  $<INSTALL_INTERFACE:include>
)
target_compile_definitions(libUriParserDyn PUBLIC URIPARSER_BUILD_CHAR)
target_compile_definitions(libUriParserDyn PUBLIC URI_STATIC_BUILD)
target_include_directories(libUriParserDyn PUBLIC
  $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/3rdparty/uriparser/include>
  $<INSTALL_INTERFACE:include>
)
# libDaScript

FLEX(src/parser/ds_lexer.lpp)
FLEX(src/parser/ds2_lexer.lpp)
BISON(src/parser/ds_parser.ypp)
BISON(src/parser/ds2_parser.ypp)
FLEX(utils/dasFsormatter/ds_lexer.lpp)
BISON(utils/dasFormatter/ds_parser.ypp)

SET(PARSER_GENERATED_SRC
# 1st parser
src/parser/ds_parser.hpp
src/parser/ds_parser.cpp
src/parser/ds_lexer.cpp
src/parser/lex.yy.h
# 2nd parser
src/parser/ds2_parser.hpp
src/parser/ds2_parser.cpp
src/parser/ds2_lexer.cpp
src/parser/lex2.yy.h
)

SET(PARSER_SRC
src/parser/ds_parser.ypp
src/parser/ds_lexer.lpp
src/parser/ds2_parser.ypp
src/parser/ds2_lexer.lpp
src/parser/parser_state.h
src/parser/parser_impl.cpp
src/parser/parser_impl.h
)
list(SORT PARSER_SRC)
SOURCE_GROUP_FILES("parser" PARSER_SRC)
SOURCE_GROUP_FILES("parser/generated" PARSER_GENERATED_SRC)

IF(MSVC)
    # Don't use precompiled headers with flex/bison generated files
    FOREACH(file ${PARSER_GENERATED_SRC})
        SET_SOURCE_FILES_PROPERTIES(${file} PROPERTIES COMPILE_FLAGS "/Y-" )
    ENDFOREACH()
ENDIF()

SET(VECMATH_SRC
include/vecmath/dag_vecMath.h
include/vecmath/dag_vecMathDecl.h
include/vecmath/dag_vecMath_common.h
include/vecmath/dag_vecMath_const.h
include/vecmath/dag_vecMath_neon.h
include/vecmath/dag_vecMath_pc_sse.h
include/vecmath/dag_vecMath_trig.h
)
list(SORT VECMATH_SRC)
SOURCE_GROUP_FILES("vecmath" VECMATH_SRC)

SET(AST_SRC
src/ast/ast.cpp
src/ast/ast_interop.cpp
src/ast/ast_tls.cpp
src/ast/ast_visitor.cpp
src/ast/ast_generate.cpp
src/ast/ast_simulate.cpp
src/ast/ast_typedecl.cpp
src/ast/ast_match.cpp
src/ast/ast_module.cpp
src/ast/ast_print.cpp
src/ast/ast_infer_type.cpp
src/ast/ast_infer_type_report.cpp
src/ast/ast_infer_type_function.cpp
src/ast/ast_infer_type_helper.cpp
src/ast/ast_infer_type_op.cpp
src/ast/ast_infer_type_make.cpp
src/ast/ast_lint.cpp
src/ast/ast_allocate_stack.cpp
src/ast/ast_derive_alias.cpp
src/ast/ast_const_folding.cpp
src/ast/ast_block_folding.cpp
src/ast/ast_gc_collect.cpp
src/ast/ast_inscope_pod.cpp
src/ast/ast_unused.cpp
src/ast/ast_annotations.cpp
src/ast/ast_export.cpp
src/ast/ast_parse.cpp
src/ast/ast_validate.cpp
src/ast/ast_debug_info_helper.cpp
src/ast/ast_handle.cpp
src/ast/dyn_modules.cpp
include/daScript/ast/compilation_errors.h
include/daScript/ast/ast_typedecl.h
include/daScript/ast/ast_typefactory.h
include/daScript/ast/ast.h
include/daScript/ast/ast_expressions.h
include/daScript/ast/ast_visitor.h
include/daScript/ast/ast_generate.h
include/daScript/ast/ast_match.h
include/daScript/ast/ast_interop.h
include/daScript/ast/ast_handle.h
include/daScript/ast/ast_policy_types.h
)
list(SORT AST_SRC)
SOURCE_GROUP_FILES("ast" AST_SRC)

SET(BUILTIN_SRC
include/daScript/builtin/ast_gen.inc
include/daScript/builtin/debugapi_gen.inc

src/builtin/module_builtin.h
src/builtin/module_builtin.cpp
src/builtin/module_builtin_misc_types.cpp
src/builtin/module_builtin_runtime.cpp
src/builtin/module_builtin_runtime_sort.cpp
src/builtin/module_builtin_runtime_lockcheck.cpp
src/builtin/module_builtin_vector.cpp
src/builtin/module_builtin_vector_ctor.cpp
src/builtin/module_builtin_array.cpp
src/builtin/module_builtin_math.cpp
src/builtin/module_builtin_string.cpp
src/builtin/module_builtin_rtti.h
src/builtin/module_builtin_rtti.cpp
src/builtin/module_builtin_ast.cpp
src/builtin/module_builtin_ast_serialize.cpp
src/builtin/module_builtin_ast_flags.cpp
src/builtin/module_builtin_ast_annotations.cpp
src/builtin/module_builtin_ast_annotations_1.cpp
src/builtin/module_builtin_ast_annotations_2.cpp
src/builtin/module_builtin_ast_annotations_3.cpp
src/builtin/module_builtin_ast_adapters.cpp
src/builtin/module_builtin_ast.h
src/builtin/module_builtin_uriparser.h
src/builtin/module_builtin_uriparser.cpp
src/builtin/module_jit.cpp
src/builtin/module_builtin_fio.cpp
src/builtin/module_builtin_dasbind.cpp
src/builtin/module_builtin_network.cpp
src/builtin/module_builtin_debugger.cpp
src/builtin/module_builtin_jobque.cpp
src/builtin/module_file_access.cpp
)
list(SORT BUILTIN_SRC)
SOURCE_GROUP_FILES("module builtin" BUILTIN_SRC)

SET(MISC_SRC
include/daScript/misc/enums.h
include/daScript/misc/hal.h
include/daScript/misc/fpe.h
include/daScript/misc/platform.h
include/daScript/misc/vectypes.h
include/daScript/misc/arraytype.h
include/daScript/misc/rangetype.h
include/daScript/misc/string_writer.h
include/daScript/misc/type_name.h
include/daScript/misc/memory_model.h
include/daScript/misc/wyhash.h
include/daScript/misc/anyhash.h
include/daScript/misc/safebox.h
include/daScript/misc/smart_ptr.h
include/daScript/misc/free_list.h
include/daScript/misc/sysos.h
include/daScript/misc/callable.h
include/daScript/misc/debug_break.h
include/daScript/misc/instance_debugger.h
include/daScript/misc/gc_node.h
include/daScript/misc/job_que.h
include/daScript/misc/handle_registry.h
include/daScript/misc/uric.h
include/daScript/misc/lexer_alloc_track.h
src/misc/gc_node.cpp
src/misc/globals.cpp
src/misc/sysos.cpp
src/misc/string_writer.cpp
src/misc/memory_model.cpp
src/misc/job_que.cpp
src/misc/handle_registry.cpp
src/misc/free_list.cpp
src/misc/alloc_tracker.cpp
src/misc/alloc_tracker_fast_stack.cpp
src/misc/lexer_alloc_track.cpp
src/misc/uric.cpp
src/misc/format.cpp
)
list(SORT MISC_SRC)
SOURCE_GROUP_FILES("misc" MISC_SRC)

SET(SIMULATE_FUSION_SRC
src/simulate/simulate_fusion.cpp
src/simulate/simulate_fusion_op1.cpp
src/simulate/simulate_fusion_op1_return.cpp
src/simulate/simulate_fusion_ptrfdr.cpp
src/simulate/simulate_fusion_op2.cpp
src/simulate/simulate_fusion_op2_set.cpp
src/simulate/simulate_fusion_op2_bool.cpp
src/simulate/simulate_fusion_op2_bin.cpp
src/simulate/simulate_fusion_op2_vec.cpp
src/simulate/simulate_fusion_op2_set_vec.cpp
src/simulate/simulate_fusion_op2_bool_vec.cpp
src/simulate/simulate_fusion_op2_bin_vec.cpp
src/simulate/simulate_fusion_op2_scalar_vec.cpp
src/simulate/simulate_fusion_at.cpp
src/simulate/simulate_fusion_at_array.cpp
src/simulate/simulate_fusion_tableindex.cpp
src/simulate/simulate_fusion_misc_copy.cpp
src/simulate/simulate_fusion_call1.cpp
src/simulate/simulate_fusion_call2.cpp
src/simulate/simulate_fusion_if.cpp
include/daScript/simulate/simulate_fusion.h
include/daScript/simulate/simulate_fusion_op1.h
include/daScript/simulate/simulate_fusion_op1_impl.h
include/daScript/simulate/simulate_fusion_op1_perm.h
include/daScript/simulate/simulate_fusion_op1_set_impl.h
include/daScript/simulate/simulate_fusion_op1_set_perm.h
include/daScript/simulate/simulate_fusion_op1_reg.h
include/daScript/simulate/simulate_fusion_op2.h
include/daScript/simulate/simulate_fusion_op2_impl.h
include/daScript/simulate/simulate_fusion_op2_perm.h
include/daScript/simulate/simulate_fusion_op2_set_impl.h
include/daScript/simulate/simulate_fusion_op2_set_perm.h
include/daScript/simulate/simulate_fusion_op2_vec_settings.h
)
list(SORT SIMULATE_FUSION_SRC)
SOURCE_GROUP_FILES("fusion" SIMULATE_FUSION_SRC)

SET(SIMULATE_SRC
src/hal/performance_time.cpp
include/daScript/misc/performance_time.h
src/hal/debug_break.cpp
src/hal/crash_handler.cpp
src/hal/project_specific.cpp
src/hal/project_specific_file_info.cpp
src/hal/project_specific_crash_handler.cpp
include/daScript/misc/crash_handler.h
include/daScript/misc/network.h
src/misc/network.cpp
src/simulate/hash.cpp
src/simulate/debug_info.cpp
src/simulate/runtime_string.cpp
src/simulate/runtime_array.cpp
src/simulate/runtime_table.cpp
src/simulate/runtime_profile.cpp
src/simulate/standalone_ctx_utils.cpp
src/simulate/simulate.cpp
src/simulate/simulate_exceptions.cpp
src/simulate/simulate_gc.cpp
src/simulate/simulate_tracking.cpp
src/simulate/simulate_visit.cpp
src/simulate/simulate_print.cpp
src/simulate/simulate_fn_hash.cpp
src/simulate/simulate_instrument.cpp
include/daScript/simulate/cast.h
include/daScript/simulate/hash.h
include/daScript/simulate/heap.h
src/simulate/heap.cpp
include/daScript/simulate/debug_info.h
include/daScript/simulate/interop.h
include/daScript/simulate/runtime_string.h
include/daScript/simulate/runtime_string_delete.h
include/daScript/simulate/runtime_array.h
include/daScript/simulate/runtime_table.h
include/daScript/simulate/runtime_table_nodes.h
include/daScript/simulate/runtime_range.h
include/daScript/simulate/runtime_profile.h
include/daScript/simulate/runtime_matrices.h
include/daScript/simulate/simulate.h
include/daScript/simulate/simulate_nodes.h
include/daScript/simulate/simulate_visit.h
include/daScript/simulate/simulate_visit_op.h
include/daScript/simulate/simulate_visit_op_undef.h
include/daScript/simulate/sim_policy.h
src/simulate/data_walker.cpp
include/daScript/simulate/data_walker.h
src/simulate/debug_print.cpp
src/simulate/json_print.cpp
src/simulate/json_scan.cpp
include/daScript/simulate/debug_print.h
include/daScript/simulate/for_each.h
include/daScript/simulate/bind_enum.h
include/daScript/simulate/bin_serializer.h
src/simulate/bin_serializer.cpp
include/daScript/simulate/aot.h
include/daScript/simulate/aot_library.h
include/daScript/simulate/aot_builtin.h
include/daScript/simulate/aot_builtin_math.h
include/daScript/simulate/aot_builtin_matrix.h
include/daScript/simulate/aot_builtin_time.h
include/daScript/simulate/aot_builtin_string.h
include/daScript/simulate/aot_builtin_fio.h
include/daScript/simulate/aot_builtin_rtti.h
include/daScript/simulate/aot_builtin_ast.h
include/daScript/simulate/aot_builtin_debugger.h
include/daScript/simulate/aot_builtin_jobque.h
include/daScript/simulate/aot_builtin_dasbind.h
include/daScript/simulate/aot_builtin_uriparser.h
include/daScript/simulate/aot_builtin_jit.h
include/daScript/simulate/fs_file_info.h
src/simulate/fs_file_info.cpp
)


# das formatter
SET(DASCRIPT_FMT_SRC
        ${PROJECT_SOURCE_DIR}/utils/dasFormatter/fmt.cpp
        ${PROJECT_SOURCE_DIR}/utils/dasFormatter/formatter.cpp
        ${PROJECT_SOURCE_DIR}/utils/dasFormatter/helpers.cpp
        ${PROJECT_SOURCE_DIR}/utils/dasFormatter/ds_parser.cpp
        CACHE INTERNAL "DAS_DASCRIPT_FMT_SRC"
)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  set_source_files_properties(src/builtin/module_builtin_ast_serialize.cpp PROPERTIES
    COMPILE_FLAGS "-fexceptions"
  )
elseif(MSVC)
  set_source_files_properties(src/builtin/module_builtin_ast_serialize.cpp PROPERTIES
    COMPILE_FLAGS "/EHsc"
  )
endif()

list(SORT SIMULATE_SRC)
SOURCE_GROUP_FILES("simulate" SIMULATE_SRC)

SET(DAGOR_NOISE_SRC
include/dag_noise/dag_uint_noise.h
)
SOURCE_GROUP_FILES("dag_noise" DAGOR_NOISE_SRC)

SET(DAS_HASH_MAP_SRC
include/das_hash_map/das_hash_map.h
)
SOURCE_GROUP_FILES("das_hash_map" DAS_HASH_MAP_SRC)

SET(FAST_FLOAT_SRC
include/fast_float/fast_float.h
)

SET(MAIN_SRC
include/daScript/daScript.h
include/daScript/daScriptC.h
include/daScript/daScriptModule.h
include/daScript/das_config.h
include/daScript/das_project_specific.h
)
SOURCE_GROUP_FILES("main" MAIN_SRC)

file(GLOB DAS_LIB_SRC
"daslib/*.das"
)
list(SORT DAS_LIB_SRC)
SOURCE_GROUP_FILES("daslib" DSA_LIB_SRC)
list(SORT DAS_LIB_SRC)

include_directories(include)
include_directories(3rdparty/fmt/include)

MACRO(SETUP_LIBDASCRIPT library shared_lib full_setup sources exports_def)
    IF(${shared_lib})
        SETUP_DAS_SHARED_LIBRARY(${library} ${${sources}})
        TARGET_COMPILE_DEFINITIONS(${library} PRIVATE ${exports_def}=1 DAS_MOD_EXPORTS=1)
        TARGET_COMPILE_DEFINITIONS(${library} PUBLIC DAS_ENABLE_DLL=1)
    ELSE()
        ADD_DAS_STATIC_LIBRARY(${library} ${${sources}})
    ENDIF()
    TARGET_COMPILE_DEFINITIONS(${library} PUBLIC DAS_ENABLE_DYN_INCLUDES=1)
    ADD_DEPENDENCIES(${library} need_and_resolve)
    ADD_TARGET_PROJECT_XXD_DEPENDS(${library} libDaScript)
    target_include_directories(${library} PRIVATE
            ${DAS_SMMALLOC_DIR}
            ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/uriparser/include
            ${CMAKE_CURRENT_SOURCE_DIR}/include
    )
    IF(${full_setup})
        IF(${shared_lib})
            target_link_libraries(${library} libUriParserDyn)
        ELSE()
            target_link_libraries(${library} libUriParser)
        ENDIF()
        #target_link_libraries(${library} fmt::fmt)
        IF(LINUX_UUID)
            target_link_libraries(${library} uuid)
        ENDIF()
        IF(UNIX AND NOT APPLE)
            TARGET_LINK_LIBRARIES(${library} ${CMAKE_DL_LIBS})
        ENDIF()
        IF(HAIKU)
            TARGET_LINK_LIBRARIES(${library} network uuid)
        ENDIF()
        IF(WIN32)
            target_link_libraries(${library} dbghelp)
        ENDIF()
    ENDIF()
    SETUP_CPP11(${library})
    target_compile_features(${library} PUBLIC cxx_std_17)
    #target_precompile_headers(${library} PUBLIC include/daScript/misc/platform.h)

    # Static modules generated include.
    target_include_directories(${library} PUBLIC
        $<BUILD_INTERFACE:${NEED_MODULES_PATH}>
        $<INSTALL_INTERFACE:include>
    )
ENDMACRO()

SET(LIBDASCRIPT_RUNTIME_SRC
    ${PARSER_GENERATED_SRC}
    ${PARSER_SRC} ${VECMATH_SRC} ${AST_SRC} ${SIMULATE_SRC} ${BUILTIN_SRC}
    ${MISC_SRC} ${MAIN_SRC}
    ${DAGOR_NOISE_SRC} ${DAS_HASH_MAP_SRC} ${FAST_FLOAT_SRC} ${DASCRIPT_FMT_SRC}
)

SET(LIBDASCRIPT_COMPILER_SRC
    ${SIMULATE_FUSION_SRC}
    src/misc/daScriptC.cpp
    src/builtin/modules.cpp
)

# Eventually libDaScript_runtime will contain only minimal subset of sources
# required to run standalone executable.
#
# Note: Now it contains everything except fusion.
SETUP_LIBDASCRIPT(libDaScript_runtime OFF ON LIBDASCRIPT_RUNTIME_SRC DAS_EXPORTS)
SETUP_LIBDASCRIPT(libDaScriptDyn_runtime ON ON LIBDASCRIPT_RUNTIME_SRC DAS_EXPORTS)

SETUP_LIBDASCRIPT(libDaScript OFF OFF LIBDASCRIPT_COMPILER_SRC DAS_CC_EXPORTS)
SETUP_LIBDASCRIPT(libDaScriptDyn ON OFF LIBDASCRIPT_COMPILER_SRC DAS_CC_EXPORTS)

# Heap leak tracking: permanently enabled in RelWithDebInfo only.
# DAS_FREE_LIST=0 forces the reuse cache off so every aligned allocation reaches
# the tracker and free_list.cpp's global operator new/delete overrides stay inert
# (no double-override).
target_compile_definitions(libDaScript_runtime    PUBLIC
    $<$<CONFIG:RelWithDebInfo>:DAS_TRACK_ALLOC=1>
    $<$<CONFIG:RelWithDebInfo>:DAS_FREE_LIST=0>)
target_compile_definitions(libDaScriptDyn_runtime PUBLIC
    $<$<CONFIG:RelWithDebInfo>:DAS_TRACK_ALLOC=1>
    $<$<CONFIG:RelWithDebInfo>:DAS_FREE_LIST=0>)
# Global new/delete override: covers STL/raw new/smart_ptr refcount blocks.
# On Windows each module resolves operator new per-CRT, so the override TU must
# be compiled into every binary that wants its own code covered. Uses std::malloc
# internally for cross-CRT compatibility; orphan frees (cross-module) are counted
# at track_free_hook for diagnostics. The TU is #if DAS_TRACK_ALLOC-gated, so it
# compiles empty in Release/Debug.
target_sources(libDaScript_runtime    PRIVATE src/misc/alloc_tracker_overrides.cpp)
target_sources(libDaScriptDyn_runtime PRIVATE src/misc/alloc_tracker_overrides.cpp)

target_link_libraries(libDaScript libDaScript_runtime)
target_link_libraries(libDaScriptDyn libDaScriptDyn_runtime)

if(NOT ${DAS_AOT_EXAMPLES_DISABLED})

    if(${DAS_TOOLS_DISABLED})
        # error, we need tools to generate aot
        message(FATAL_ERROR "DAS_AOT_EXAMPLES_DISABLED requires DAS_TOOLS_DISABLED to be OFF")
    endif()

    set(AotDaslibList
        daslib/ast_boost.das
        daslib/ast_cursor.das
        daslib/functional.das
        daslib/json_boost.das
        daslib/json.das
        daslib/math_boost.das
        daslib/random.das
        daslib/regex_boost.das
        daslib/regex.das
        daslib/strings_boost.das
        daslib/strings_convert.das
        daslib/templates_boost.das
        daslib/utf8_utils.das
    )

    add_custom_target(dasAotStub)
    DAS_AOT_LIB("${AotDaslibList}" AOT_GENERATED_SRC dasAotStub daslang)

    SOURCE_GROUP_FILES("aot stub" AOT_GENERATED_SRC)

    SET(AOT_STUB_SRC
            src/misc/aot_stub.cpp
    )

    #UNITIZE_BUILD("daslib" AOT_GENERATED_SRC)

    ADD_DAS_STATIC_LIBRARY(libDaScriptAot ${AOT_GENERATED_SRC} ${AOT_STUB_SRC})
    ADD_DEPENDENCIES(libDaScriptAot dasAotStub libDaScript)
    TARGET_LINK_LIBRARIES(libDaScriptAot PUBLIC libDaScript)
    SETUP_CPP11(libDaScriptAot)
endif()

SET(DAS_DASCRIPT_MAIN_SRC
    ${PROJECT_SOURCE_DIR}/utils/daScript/main.cpp
    ${DAS_MODULES_NEED_INC}
    ${DAS_MODULES_DECLARE_INC}
    ${DAS_MODULES_PULL_INC}
    CACHE INTERNAL "DAS_DASCRIPT_MAIN_SRC"
)

# Tests
if (NOT ${DAS_TESTS_DISABLED})
    include(examples/pathTracer/CMakeLists.txt)
    include(examples/crash/CMakeLists.txt)
    if(NOT ${DAS_AOT_EXAMPLES_DISABLED} AND NOT (WIN32 AND CMAKE_SIZEOF_VOID_P EQUAL 4))
        include(tests/aot/CMakeLists.txt)
    endif()
endif()


if (NOT ${DAS_TOOLS_DISABLED})
# Stand alone command line compiler

    SOURCE_GROUP_FILES("main" DAS_DASCRIPT_MAIN_SRC)

    SET(SRC_LIBRARIES Threads::Threads)

    MACRO(SETUP_COMPILER_BINARY target_name library_name modules dyn_modules)
        add_executable(${target_name} ${DAS_DASCRIPT_MAIN_SRC})
        TARGET_LINK_LIBRARIES(${target_name} ${library_name} ${SRC_LIBRARIES} ${${modules}})
        ADD_DEPENDENCIES(${target_name} ${library_name} ${${dyn_modules}})
        # Static modules generated include.
        target_include_directories(${target_name} PRIVATE ${NEED_MODULES_PATH})
        SETUP_CPP11(${target_name})
        SETUP_LTO(${target_name})
    ENDMACRO()

    SETUP_COMPILER_BINARY(daslang_static libDaScript DAS_MODULES_LIBS "")

    # Dynamic/DLL compiler binary — the default shipped binary.
    SETUP_COMPILER_BINARY(daslang libDaScriptDyn "" DAS_DYN_MODULES_LIBS)
    # Each binary needs its own copy of the override TU to cover its own code
    # (Windows resolves operator new per-CRT). TU is #if DAS_TRACK_ALLOC-gated.
    target_sources(daslang        PRIVATE src/misc/alloc_tracker_overrides.cpp)
    target_sources(daslang_static PRIVATE src/misc/alloc_tracker_overrides.cpp)
    # relative RPATH from install location
    if(APPLE)
        SET_TARGET_PROPERTIES(daslang PROPERTIES
            INSTALL_RPATH "@loader_path/../lib"
            CMAKE_MACOSX_RPATH ON
        )
    elseif(UNIX)
        SET_TARGET_PROPERTIES(daslang PROPERTIES
            INSTALL_RPATH "\$ORIGIN/../lib"
        )
    endif()
    # Export daslang target, so it can be found
    # as a target (for example in aot).
    INSTALL(
        TARGETS daslang
        EXPORT DASTargets
        RUNTIME DESTINATION ${DAS_INSTALL_BINDIR}
    )

    # daslang-live: live-reloading application host
    SET(DAS_LIVE_MAIN_SRC
        ${PROJECT_SOURCE_DIR}/utils/daslang-live/main.cpp
        ${DAS_MODULES_NEED_INC}
    )
    add_executable(daslang-live ${DAS_LIVE_MAIN_SRC})
    target_sources(daslang-live PRIVATE src/misc/alloc_tracker_overrides.cpp)
    TARGET_LINK_LIBRARIES(daslang-live libDaScriptDyn ${SRC_LIBRARIES})
    ADD_DEPENDENCIES(daslang-live libDaScriptDyn ${DAS_DYN_MODULES_LIBS})
    target_include_directories(daslang-live PRIVATE
        ${NEED_MODULES_PATH})
    SETUP_CPP11(daslang-live)
    SETUP_LTO(daslang-live)
    if(APPLE)
        SET_TARGET_PROPERTIES(daslang-live PROPERTIES
            INSTALL_RPATH "@loader_path/../lib"
            CMAKE_MACOSX_RPATH ON
        )
    elseif(UNIX)
        SET_TARGET_PROPERTIES(daslang-live PROPERTIES
            INSTALL_RPATH "\$ORIGIN/../lib"
        )
    endif()
    INSTALL(
        TARGETS daslang-live
        RUNTIME DESTINATION ${DAS_INSTALL_BINDIR}
    )

    file(GLOB DASLIB_SOURCES
        ${PROJECT_SOURCE_DIR}/daslib/*.das
    )
    install(FILES ${DASLIB_SOURCES}
        DESTINATION ${DAS_INSTALL_DASLIBDIR}
    )

    file(GLOB DASTEST_SOURCES
        ${PROJECT_SOURCE_DIR}/dastest/*.das
    )
    install(FILES ${DASTEST_SOURCES}
        DESTINATION ${DAS_INSTALL_DASTESTDIR}
    )
    install(FILES
        ${PROJECT_SOURCE_DIR}/dastest/LICENSE
        ${PROJECT_SOURCE_DIR}/dastest/README.md
        DESTINATION ${DAS_INSTALL_DASTESTDIR}
    )
    install(DIRECTORY ${PROJECT_SOURCE_DIR}/dastest/tests/
        DESTINATION ${DAS_INSTALL_DASTESTDIR}/tests
        FILES_MATCHING PATTERN "*.das"
    )

    add_executable(das-fmt ${PROJECT_SOURCE_DIR}/utils/dasFormatter/main.cpp)
    TARGET_LINK_LIBRARIES(das-fmt libDaScript ${SRC_LIBRARIES} ${DAS_MODULES_LIBS})
    target_include_directories(das-fmt PRIVATE ${NEED_MODULES_PATH})

    SETUP_CPP11(das-fmt)
    SETUP_LTO(das-fmt)
    install(TARGETS das-fmt RUNTIME DESTINATION ${DAS_INSTALL_BINDIR})
get_target_property(DAS_FMT_INCLUDES libDaScriptDyn INCLUDE_DIRECTORIES)
get_target_property(DAS_FMT_DEFS das-fmt COMPILE_DEFINITIONS)
get_target_property(DAS_FMT_LIBS das-fmt LINK_LIBRARIES)

    #IF(APPLE)
    #  add_executable(daslangOsx MACOSX_BUNDLE ${DAS_DASCRIPT_MAIN_SRC})
    #  TARGET_LINK_LIBRARIES(daslangOsx libDaScript libDaScriptTest Threads::Threads ${DAS_MODULES_LIBS})
    #  ADD_DEPENDENCIES(daslangOsx libDaScript libDaScriptTest  need_and_resolve ${DAS_MODULES_LIBS})
    #  SETUP_CPP11(daslangOsx)
    #  SETUP_LTO(daslangOsx)
    #  set_target_properties(daslangOsx PROPERTIES MACOSX_BUNDLE_BUNDLE_NAME "daslang")
    #  set_target_properties(daslangOsx PROPERTIES
    #      MACOSX_BUNDLE_SHORT_VERSION_STRING "0.6"
    #      MACOSX_BUNDLE_LONG_VERSION_STRING  "daslang 0.6"
    #      MACOSX_BUNDLE_INFO_PLIST ${CMAKE_SOURCE_DIR}/utils/daScript/MacOSXBundleInfo.plist.in)
    #ENDIF()

    # C++ test infrastructure (doctest under tests-cpp/, plus big-test executables).
    # Owns ALL C++-test-related CMake. enable_testing() lives at the top of this file.
    add_subdirectory(tests-cpp)

    # Build daslang utilities as standalone executables (-exe mode)
    add_subdirectory(utils)

endif()

# This list should be significantly reduced, most of the files, except aot related should be private.
SET(DAS_RELEASE_AST_INCLUDE
    include/daScript/ast/aot_templates.h
    include/daScript/ast/ast_expressions.h
    include/daScript/ast/ast_generate.h
    include/daScript/ast/ast.h
    include/daScript/ast/ast_handle.h
    include/daScript/ast/ast_infer_type.h
    include/daScript/ast/ast_interop.h
    include/daScript/ast/ast_match.h
    include/daScript/ast/ast_policy_types.h
    include/daScript/ast/ast_serialize_macro.h
    include/daScript/ast/ast_serializer.h
    include/daScript/ast/ast_simulate.h
    include/daScript/ast/ast_typedecl.h
    include/daScript/ast/ast_typefactory_bind.h
    include/daScript/ast/ast_typefactory.h
    include/daScript/ast/ast_visitor.h
    include/daScript/ast/ast_aot_cpp.h
    include/daScript/ast/compilation_errors.h
    include/daScript/ast/dyn_modules.h
)

SET(DAS_RELEASE_C_API
    include/daScript/daScriptC.h
)

SET(DAS_RELEASE_INCLUDE
    include/daScript/daScript.h
    include/daScript/daScriptBind.h
    include/daScript/daScriptModule.h
    include/daScript/das_common.h
    include/daScript/das_config.h
    include/daScript/das_project_specific.h
)

SET(DAS_RELEASE_MISC_INCLUDE
    include/daScript/misc/anyhash.h
    include/daScript/misc/arraytype.h
    include/daScript/misc/callable.h
    include/daScript/misc/crash_handler.h
    include/daScript/misc/debug_break.h
    include/daScript/misc/enums.h
    include/daScript/misc/fpe.h
    include/daScript/misc/free_list.h
    include/daScript/misc/gc_node.h
    include/daScript/misc/hal.h
    include/daScript/misc/hash.h
    include/daScript/misc/instance_debugger.h
    include/daScript/misc/job_que.h
    include/daScript/misc/lexer_alloc_track.h
    include/daScript/misc/handle_registry.h
    include/daScript/misc/macro.h
    include/daScript/misc/memory_model.h
    include/daScript/misc/network.h
    include/daScript/misc/performance_time.h
    include/daScript/misc/platform.h
    include/daScript/misc/rangetype.h
    include/daScript/misc/runtime_table_utils.h
    include/daScript/misc/safebox.h
    include/daScript/misc/smart_ptr.h
    include/daScript/misc/string_writer.h
    include/daScript/misc/sysos.h
    include/daScript/misc/type_name.h
    include/daScript/misc/uric.h
    include/daScript/misc/vectypes.h
    include/daScript/misc/wyhash.h
)

SET(DAS_SIMULATE_INCLUDES
    include/daScript/simulate/aot_builtin_ast.h
    include/daScript/simulate/aot_builtin_dasbind.h
    include/daScript/simulate/aot_builtin_debugger.h
    include/daScript/simulate/aot_builtin_fio.h
    include/daScript/simulate/aot_builtin.h
    include/daScript/simulate/aot_builtin_jit.h
    include/daScript/simulate/aot_builtin_jobque.h
    include/daScript/simulate/aot_builtin_math.h
    include/daScript/simulate/aot_builtin_matrix.h
    include/daScript/simulate/aot_builtin_network.h
    include/daScript/simulate/aot_builtin_rtti.h
    include/daScript/simulate/aot_builtin_string.h
    include/daScript/simulate/aot_builtin_time.h
    include/daScript/simulate/aot_builtin_uriparser.h
    include/daScript/simulate/aot.h
    include/daScript/simulate/aot_library.h
    include/daScript/simulate/bind_enum.h
    include/daScript/simulate/bin_serializer.h
    include/daScript/simulate/cast.h
    include/daScript/simulate/data_walker.h
    include/daScript/simulate/debug_info.h
    include/daScript/simulate/debug_print.h
    include/daScript/simulate/for_each.h
    include/daScript/simulate/fs_file_info.h
    include/daScript/simulate/hash.h
    include/daScript/simulate/heap.h
    include/daScript/simulate/interop.h
    include/daScript/simulate/jit_abi.h
    include/daScript/simulate/runtime_array.h
    include/daScript/simulate/runtime_matrices.h
    include/daScript/simulate/runtime_profile.h
    include/daScript/simulate/runtime_range.h
    include/daScript/simulate/runtime_string_delete.h
    include/daScript/simulate/runtime_string.h
    include/daScript/simulate/runtime_table.h
    include/daScript/simulate/runtime_table_nodes.h
    include/daScript/simulate/sim_policy.h
    include/daScript/simulate/simulate_fusion.h
    include/daScript/simulate/simulate_fusion_op1.h
    include/daScript/simulate/simulate_fusion_op1_impl.h
    include/daScript/simulate/simulate_fusion_op1_perm.h
    include/daScript/simulate/simulate_fusion_op1_reg.h
    include/daScript/simulate/simulate_fusion_op1_set_impl.h
    include/daScript/simulate/simulate_fusion_op1_set_perm.h
    include/daScript/simulate/simulate_fusion_op2.h
    include/daScript/simulate/simulate_fusion_op2_impl.h
    include/daScript/simulate/simulate_fusion_op2_perm.h
    include/daScript/simulate/simulate_fusion_op2_set_impl.h
    include/daScript/simulate/simulate_fusion_op2_set_perm.h
    include/daScript/simulate/simulate_fusion_op2_vec_settings.h
    include/daScript/simulate/simulate.h
    include/daScript/simulate/simulate_nodes.h
    include/daScript/simulate/simulate_visit.h
    include/daScript/simulate/simulate_visit_op.h
    include/daScript/simulate/simulate_visit_op_undef.h
    include/daScript/simulate/standalone_ctx_utils.h
)

install(FILES
    ${DAS_RELEASE_AST_INCLUDE}
    DESTINATION include/daScript/ast
)

install(FILES
    ${DAS_RELEASE_C_API}
    DESTINATION include/daScript
)

install(FILES
    ${DAS_RELEASE_INCLUDE}
    DESTINATION include/daScript
)

install(FILES
    ${DAS_RELEASE_MISC_INCLUDE}
    DESTINATION include/daScript/misc
)

install(FILES
    ${DAS_SIMULATE_INCLUDES}
    DESTINATION include/daScript/simulate
)

install(FILES
    ${DAS_HASH_MAP_SRC}
    DESTINATION include/das_hash_map
)

install(FILES
    ${VECMATH_SRC}
    DESTINATION include/vecmath
)

# Install dag_noise header (referenced by aot_builtin_math.h).
install(FILES include/dag_noise/dag_uint_noise.h
    DESTINATION include/dag_noise
)

# Install internal builtin headers (needed by integration tutorials like class adapters).
file(GLOB DAS_BUILTIN_HEADERS ${PROJECT_SOURCE_DIR}/src/builtin/*.h)
install(FILES ${DAS_BUILTIN_HEADERS}
    DESTINATION include/daScript/builtin
)

# Install all modules and main library.
install(TARGETS
    libDaScript libDaScript_runtime libDaScriptDyn libDaScriptDyn_runtime libUriParser libUriParserDyn
    EXPORT DASTargets
    RUNTIME DESTINATION ${DAS_INSTALL_BINDIR}
    LIBRARY DESTINATION ${DAS_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${DAS_INSTALL_LIBDIR}
    INCLUDES DESTINATION include
)

set(DAS_CONFIG_INSTALL_DIR "${DAS_INSTALL_LIBDIR}/cmake/DAS")

install(EXPORT DASTargets
    FILE DASTargets.cmake
    NAMESPACE DAS::
    DESTINATION ${DAS_CONFIG_INSTALL_DIR}
)

include(CMakePackageConfigHelpers)

# Perform package configuration.
configure_package_config_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/cmake/DASConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/cmake/DASConfig.cmake
  INSTALL_DESTINATION "${DAS_CONFIG_INSTALL_DIR}"
)

write_basic_package_version_file(
  ${CMAKE_CURRENT_BINARY_DIR}/cmake/DASConfigVersion.cmake
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY AnyNewerVersion
)


install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/cmake/DASConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/cmake/DASConfigVersion.cmake
    DESTINATION ${DAS_CONFIG_INSTALL_DIR}
)

install(FILES ${PROJECT_SOURCE_DIR}/install/README.md DESTINATION ${DAS_INSTALL_DOCDIR})
install(FILES ${PROJECT_SOURCE_DIR}/install/CLAUDE.md DESTINATION ${DAS_INSTALL_DOCDIR})
install(FILES ${PROJECT_SOURCE_DIR}/install/.gitignore DESTINATION ${DAS_INSTALL_DOCDIR})
install(DIRECTORY ${PROJECT_SOURCE_DIR}/install/skills DESTINATION ${DAS_INSTALL_DOCDIR})
install(FILES ${PROJECT_SOURCE_DIR}/LICENSE DESTINATION ${DAS_INSTALL_DOCDIR})
install(FILES ${PROJECT_SOURCE_DIR}/3rdparty/uriparser/COPYING DESTINATION ${DAS_INSTALL_DOCDIR} RENAME URIPARSER.LICENSE)
install(FILES ${PROJECT_SOURCE_DIR}/include/dag_noise/LICENSE DESTINATION ${DAS_INSTALL_DOCDIR} RENAME DAG_NOISE.LICENSE)
install(FILES ${PROJECT_SOURCE_DIR}/include/vecmath/LICENSE DESTINATION ${DAS_INSTALL_DOCDIR} RENAME VEC_MATH.LICENSE)

# Install utility scripts
install(FILES ${PROJECT_SOURCE_DIR}/utils/aot/main.das DESTINATION utils/aot)

# Install MCP server (stdio transport, no dasHV dependency)
install(FILES
    ${PROJECT_SOURCE_DIR}/utils/mcp/main.das
    ${PROJECT_SOURCE_DIR}/utils/mcp/protocol.das
    ${PROJECT_SOURCE_DIR}/utils/mcp/README.md
    DESTINATION utils/mcp
)
file(GLOB DAS_MCP_TOOLS ${PROJECT_SOURCE_DIR}/utils/mcp/tools/*.das)
install(FILES ${DAS_MCP_TOOLS} DESTINATION utils/mcp/tools)
install(FILES
    ${PROJECT_SOURCE_DIR}/utils/mcp/test_tools.das
    DESTINATION utils/mcp
)
install(DIRECTORY ${PROJECT_SOURCE_DIR}/utils/mcp/tests/
    DESTINATION utils/mcp/tests
    FILES_MATCHING PATTERN "*.das"
)

# Install dascov (code coverage)
install(FILES ${PROJECT_SOURCE_DIR}/utils/dascov/main.das DESTINATION utils/dascov)
install(FILES ${PROJECT_SOURCE_DIR}/utils/dascov/README.md DESTINATION utils/dascov)
file(GLOB DAS_DASCOV_TEST_FILES ${PROJECT_SOURCE_DIR}/utils/dascov/tests/*.das)
install(FILES ${DAS_DASCOV_TEST_FILES} DESTINATION utils/dascov/tests)

# Install benchctl (benchmark analysis)
file(GLOB DAS_BENCHCTL_FILES ${PROJECT_SOURCE_DIR}/utils/benchctl/*.das)
install(FILES ${DAS_BENCHCTL_FILES} DESTINATION utils/benchctl)
install(FILES ${PROJECT_SOURCE_DIR}/utils/benchctl/README.md DESTINATION utils/benchctl)

# Install lint (unified linter)
install(FILES ${PROJECT_SOURCE_DIR}/utils/lint/main.das DESTINATION utils/lint)

# Install hygiene (comment/doc hygiene checker)
file(GLOB DAS_HYGIENE_FILES ${PROJECT_SOURCE_DIR}/utils/hygiene/*.das)
install(FILES ${DAS_HYGIENE_FILES} DESTINATION utils/hygiene)

# Install detect-dupe (cross-file duplicate-function detector)
file(GLOB DAS_DETECT_DUPE_FILES ${PROJECT_SOURCE_DIR}/utils/detect-dupe/*.das)
install(FILES ${DAS_DETECT_DUPE_FILES} DESTINATION utils/detect-dupe)
install(FILES
    ${PROJECT_SOURCE_DIR}/utils/detect-dupe/README.md
    DESTINATION utils/detect-dupe
)
install(DIRECTORY ${PROJECT_SOURCE_DIR}/utils/detect-dupe/fixture/
    DESTINATION utils/detect-dupe/fixture
    FILES_MATCHING PATTERN "*.das"
)

# Install find-dupe (AI judge for detect-dupe clusters via das-claude).
# Not built into all_utils_exe — it depends on the das-claude daspkg
# package fetched at runtime, which isn't available at build time.
file(GLOB DAS_FIND_DUPE_FILES ${PROJECT_SOURCE_DIR}/utils/find-dupe/*.das)
install(FILES ${DAS_FIND_DUPE_FILES} DESTINATION utils/find-dupe)
install(FILES
    ${PROJECT_SOURCE_DIR}/utils/find-dupe/README.md
    ${PROJECT_SOURCE_DIR}/utils/find-dupe/.das_package
    DESTINATION utils/find-dupe
)
install(DIRECTORY ${PROJECT_SOURCE_DIR}/utils/find-dupe/tests/
    DESTINATION utils/find-dupe/tests
    FILES_MATCHING PATTERN "*.das"
)

# Install daspkg (package manager)
file(GLOB DAS_DASPKG_FILES ${PROJECT_SOURCE_DIR}/utils/daspkg/*.das)
install(FILES ${DAS_DASPKG_FILES} DESTINATION utils/daspkg)
install(FILES
    ${PROJECT_SOURCE_DIR}/utils/daspkg/README.md
    DESTINATION utils/daspkg
)
install(DIRECTORY ${PROJECT_SOURCE_DIR}/utils/daspkg/fixtures/
    DESTINATION utils/daspkg/fixtures
    FILES_MATCHING PATTERN "*.das_package"
)

# Install daspkg examples
install(DIRECTORY ${PROJECT_SOURCE_DIR}/examples/daspkg/
    DESTINATION examples/daspkg
)

# Install daslive examples
install(DIRECTORY ${PROJECT_SOURCE_DIR}/examples/daslive/
    DESTINATION ${DAS_INSTALL_EXAMPLESDIR}/daslive
    FILES_MATCHING
        PATTERN "*.das"
        PATTERN ".das_package"
    PATTERN "modules" EXCLUDE
    PATTERN ".jitted_scripts" EXCLUDE
    PATTERN "_aot_generated" EXCLUDE
    PATTERN "jitted_scripts" EXCLUDE
)

# Install game examples with their full directory structure.
install(DIRECTORY ${PROJECT_SOURCE_DIR}/examples/games/
    DESTINATION ${DAS_INSTALL_EXAMPLESDIR}/games
    PATTERN ".daspkg_cache" EXCLUDE
    PATTERN ".daspkg_tmp" EXCLUDE
    PATTERN ".daspkg.log" EXCLUDE
    PATTERN ".jitted_scripts" EXCLUDE
    PATTERN "jitted_scripts" EXCLUDE
    PATTERN "_aot_generated" EXCLUDE
)

# Install core example files
install(FILES
    ${PROJECT_SOURCE_DIR}/examples/hello_world.das
    ${PROJECT_SOURCE_DIR}/examples/README.md
    DESTINATION ${DAS_INSTALL_EXAMPLESDIR}
)

# Install dasbind example
install(FILES
    ${PROJECT_SOURCE_DIR}/examples/dasbind/dasbind_example.das
    DESTINATION ${DAS_INSTALL_EXAMPLESDIR}/dasbind
)

# Install crash example
install(FILES
    ${PROJECT_SOURCE_DIR}/examples/crash/main.das
    ${PROJECT_SOURCE_DIR}/examples/crash/stack_overflow.das
    ${PROJECT_SOURCE_DIR}/examples/crash/main.cpp
    ${PROJECT_SOURCE_DIR}/examples/crash/CMakeLists.txt
    DESTINATION ${DAS_INSTALL_EXAMPLESDIR}/crash
)

# Install minfft example (install rule is in modules/dasMinfft/CMakeLists.txt)

file(GLOB DAS_DEBUGAPI_EXAMPLES
    ${PROJECT_SOURCE_DIR}/examples/debugapi/*.das
)
install(FILES ${DAS_DEBUGAPI_EXAMPLES}
    DESTINATION ${DAS_INSTALL_EXAMPLESDIR}/debugapi
)

file(GLOB DAS_GRAPHICS_EXAMPLES
    ${PROJECT_SOURCE_DIR}/examples/graphics/*.das
    ${PROJECT_SOURCE_DIR}/examples/graphics/.das_package
    ${PROJECT_SOURCE_DIR}/examples/graphics/README.md
)
install(FILES ${DAS_GRAPHICS_EXAMPLES}
    DESTINATION ${DAS_INSTALL_EXAMPLESDIR}/graphics
)

# Install node-editor example
file(GLOB DAS_NODE_EDITOR_EXAMPLES
    ${PROJECT_SOURCE_DIR}/examples/node-editor/*.das
    ${PROJECT_SOURCE_DIR}/examples/node-editor/.das_package
    ${PROJECT_SOURCE_DIR}/examples/node-editor/README.md
)
install(FILES ${DAS_NODE_EDITOR_EXAMPLES}
    DESTINATION ${DAS_INSTALL_EXAMPLESDIR}/node-editor
)

file(GLOB DAS_UNCATEGORIZED_EXAMPLES
    ${PROJECT_SOURCE_DIR}/examples/uncategorized/*.das
)
install(FILES ${DAS_UNCATEGORIZED_EXAMPLES}
    DESTINATION ${DAS_INSTALL_EXAMPLESDIR}/uncategorized
)

# Install telegram example
install(FILES
    ${PROJECT_SOURCE_DIR}/examples/telegram/.das_package
    ${PROJECT_SOURCE_DIR}/examples/telegram/README.md
    ${PROJECT_SOURCE_DIR}/examples/telegram/echo_bot.das
    DESTINATION ${DAS_INSTALL_EXAMPLESDIR}/telegram
)

# Install claude example
install(FILES
    ${PROJECT_SOURCE_DIR}/examples/claude/.das_package
    ${PROJECT_SOURCE_DIR}/examples/claude/.gitignore
    ${PROJECT_SOURCE_DIR}/examples/claude/daslang_helper_bot.das
    ${PROJECT_SOURCE_DIR}/examples/claude/sandbox.das_project
    DESTINATION ${DAS_INSTALL_EXAMPLESDIR}/claude
)
install(FILES
    ${PROJECT_SOURCE_DIR}/examples/claude/examples/daslang_helper/system_prompt.md
    DESTINATION ${DAS_INSTALL_EXAMPLESDIR}/claude/examples/daslang_helper
)

file(GLOB DAS_HV_EXAMPLES
    ${PROJECT_SOURCE_DIR}/examples/hv/*.das
    ${PROJECT_SOURCE_DIR}/examples/hv/*.html
    ${PROJECT_SOURCE_DIR}/examples/hv/*.md
)
install(FILES ${DAS_HV_EXAMPLES}
    DESTINATION ${DAS_INSTALL_EXAMPLESDIR}/hv
)

# Install opengl examples
file(GLOB DAS_OPENGL_EXAMPLES
    ${PROJECT_SOURCE_DIR}/examples/opengl/*.das
)
install(FILES ${DAS_OPENGL_EXAMPLES}
    DESTINATION ${DAS_INSTALL_EXAMPLESDIR}/opengl
)
install(FILES
    ${PROJECT_SOURCE_DIR}/examples/opengl/image.png
    # This mesh was taken from BGFX `modules/dasBGFX/bgfx/bgfx/examples/assets/meshes`
    ${PROJECT_SOURCE_DIR}/examples/opengl/orb.obj
    # This font was taken from BGFX `modules/dasBGFX/bgfx/bgfx/examples/runtime/font`
    ${PROJECT_SOURCE_DIR}/examples/opengl/droidsansmono.ttf
    DESTINATION ${DAS_INSTALL_EXAMPLESDIR}/opengl/media
)

add_subdirectory(tutorials)

# ── Documentation build (optional) ──────────────────────────────────────────
if(DAS_BUILD_DOCUMENTATION)
    find_package(Python3 COMPONENTS Interpreter)
    if(Python3_FOUND)
        execute_process(
            COMMAND ${Python3_EXECUTABLE} -c "import sphinx; print(sphinx.__version__)"
            RESULT_VARIABLE SPHINX_CHECK_RESULT
            OUTPUT_VARIABLE SPHINX_VERSION
            ERROR_QUIET OUTPUT_STRIP_TRAILING_WHITESPACE
        )
    endif()
    if(Python3_FOUND AND SPHINX_CHECK_RESULT EQUAL 0)
        message(STATUS "Sphinx ${SPHINX_VERSION} found — documentation build enabled")
        set(DAS_DOC_SOURCE_DIR  "${PROJECT_SOURCE_DIR}/doc/source")
        set(DAS_DOC_BUILD_DIR   "${CMAKE_CURRENT_BINARY_DIR}/doc_html")
        set(DAS_DOC_DOCTREE_DIR "${CMAKE_CURRENT_BINARY_DIR}/doc_doctrees")

        # Step 1: regenerate stdlib RST from daslib source comments (requires daslang)
        add_custom_command(
            OUTPUT  ${CMAKE_CURRENT_BINARY_DIR}/das2rst.stamp
            COMMAND $<TARGET_FILE:daslang> "${PROJECT_SOURCE_DIR}/doc/reflections/das2rst.das"
            COMMAND ${CMAKE_COMMAND} -E touch ${CMAKE_CURRENT_BINARY_DIR}/das2rst.stamp
            DEPENDS daslang
            WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
            COMMENT "Generating stdlib RST documentation (das2rst)"
        )

        # Step 2: build HTML with Sphinx
        add_custom_command(
            OUTPUT  ${CMAKE_CURRENT_BINARY_DIR}/sphinx.stamp
            COMMAND ${Python3_EXECUTABLE} -m sphinx
                    -b html
                    -d "${DAS_DOC_DOCTREE_DIR}"
                    "${DAS_DOC_SOURCE_DIR}"
                    "${DAS_DOC_BUILD_DIR}"
            COMMAND ${CMAKE_COMMAND} -E touch ${CMAKE_CURRENT_BINARY_DIR}/sphinx.stamp
            DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/das2rst.stamp
            WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/doc
            COMMENT "Building HTML documentation (Sphinx)"
        )

        add_custom_target(das_documentation ALL
            DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/sphinx.stamp
        )

        install(DIRECTORY ${DAS_DOC_BUILD_DIR}/
            DESTINATION ${DAS_INSTALL_DOCDIR}/doc
            PATTERN ".buildinfo" EXCLUDE
            PATTERN ".doctrees" EXCLUDE
        )
    else()
        message(WARNING "DAS_BUILD_DOCUMENTATION is ON but Sphinx was not found. "
                        "Install Python 3 and run: pip install sphinx sphinx-rtd-theme")
    endif()
endif()

# macOS: ad-hoc code sign all installed binaries and shared libraries.
# This prevents Gatekeeper from quarantining downloaded SDK artifacts
# ("cannot be opened because the developer cannot be verified").
if(APPLE)
    install(CODE [[
        message(STATUS "Ad-hoc code signing installed binaries...")
        file(GLOB_RECURSE _sign_targets
            "${CMAKE_INSTALL_PREFIX}/*.dylib"
            "${CMAKE_INSTALL_PREFIX}/*.shared_module"
            "${CMAKE_INSTALL_PREFIX}/bin/*"
        )
        foreach(_f IN LISTS _sign_targets)
            if(IS_SYMLINK "${_f}")
                continue()
            endif()
            execute_process(
                COMMAND codesign --force --sign - "${_f}"
                RESULT_VARIABLE _sign_result
                ERROR_VARIABLE  _sign_err
                OUTPUT_QUIET
            )
            if(_sign_result EQUAL 0)
                message(STATUS "  Signed: ${_f}")
            else()
                message(WARNING "  Failed to sign ${_f}: ${_sign_err}")
            endif()
        endforeach()
    ]])
endif()

# ============================================================================
# tree-sitter-daslang grammar
# ============================================================================

if(NOT ${DAS_TREE_SITTER_DISABLED})
    add_library(tree_sitter_daslang SHARED
        tree-sitter-daslang/src/parser.c
        tree-sitter-daslang/src/scanner.c
    )
    target_include_directories(tree_sitter_daslang PRIVATE
        tree-sitter-daslang/src
    )
    set_target_properties(tree_sitter_daslang PROPERTIES
        C_STANDARD 11
        PREFIX ""
        OUTPUT_NAME "daslang"
        LIBRARY_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/tree-sitter-daslang"
        RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/tree-sitter-daslang"
        RUNTIME_OUTPUT_DIRECTORY_DEBUG "${PROJECT_SOURCE_DIR}/tree-sitter-daslang"
        RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL "${PROJECT_SOURCE_DIR}/tree-sitter-daslang"
        RUNTIME_OUTPUT_DIRECTORY_RELEASE "${PROJECT_SOURCE_DIR}/tree-sitter-daslang"
        RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${PROJECT_SOURCE_DIR}/tree-sitter-daslang"
    )
    if(MSVC)
        target_compile_options(tree_sitter_daslang PRIVATE /W3)
    else()
        target_compile_options(tree_sitter_daslang PRIVATE -Wall -Wextra -Wno-unused-parameter)
    endif()
    # Copy platform-specific sgconfig.yml to project root for ast-grep
    if(WIN32)
        set(SGCONFIG_TEMPLATE "${PROJECT_SOURCE_DIR}/sgconfig.yml.windows")
    elseif(APPLE)
        set(SGCONFIG_TEMPLATE "${PROJECT_SOURCE_DIR}/sgconfig.yml.osx")
    else()
        set(SGCONFIG_TEMPLATE "${PROJECT_SOURCE_DIR}/sgconfig.yml.linux")
    endif()
    add_custom_command(TARGET tree_sitter_daslang POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${SGCONFIG_TEMPLATE}"
            "${PROJECT_SOURCE_DIR}/sgconfig.yml"
        COMMENT "Copying sgconfig.yml for ast-grep (from ${SGCONFIG_TEMPLATE})"
    )
    install(TARGETS tree_sitter_daslang
        RUNTIME DESTINATION ${DAS_INSTALL_BINDIR}
        LIBRARY DESTINATION ${DAS_INSTALL_LIBDIR}
    )
    # Also install DLL into tree-sitter-daslang/ for ast-grep (sgconfig.yml references it there)
    install(TARGETS tree_sitter_daslang
        RUNTIME DESTINATION tree-sitter-daslang
        LIBRARY DESTINATION tree-sitter-daslang
    )
    install(FILES
        tree-sitter-daslang/src/node-types.json
        tree-sitter-daslang/grammar.js
        tree-sitter-daslang/package.json
        DESTINATION tree-sitter-daslang
    )
    install(FILES
        tree-sitter-daslang/queries/highlights.scm
        DESTINATION tree-sitter-daslang/queries
    )
    # Install platform-specific sgconfig.yml for ast-grep (used by grep_usage and outline MCP tools)
    install(FILES "${SGCONFIG_TEMPLATE}" DESTINATION . RENAME sgconfig.yml)
    install(FILES
        tree-sitter-daslang/outline_rules.yml
        DESTINATION tree-sitter-daslang
    )
endif()
