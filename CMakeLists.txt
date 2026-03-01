cmake_minimum_required (VERSION 3.16)
project (DAS VERSION 0.6.0)
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
option(DAS_IMGUI_DISABLED "Disable dasIMGUI (IMGUI, IMNODES, IMGUI-NODE-EDITOR gui libraries)" ON)
option(DAS_MINFFT_DISABLED "Disable dasMinfft (Minimal FFT library)" ON)
option(DAS_AUDIO_DISABLED "Disable dasAudio (Miniaudio sound library)" ON)
option(DAS_OPENAI_DISABLED "Disable dasOpenAI (OpenAI site API)" ON)
option(DAS_STDDLG_DISABLED "Disable dasStdDlg (File new,open,save etc dialogs)" OFF)
option(DAS_STBIMAGE_DISABLED "Disable dasStbImage (StbImage bindings, image loading and saving)" OFF)
option(DAS_STBTRUETYPE_DISABLED "Disable dasStbTrueType (StbTrueType bindings, ttf rasterization)" OFF)
option(DAS_PUGIXML_DISABLED "Disable dasPUGIXML (xml parsing library)" ON)
option(DAS_SQLITE_DISABLED "Disable dasSQLITE (sqlite3 library)" ON)
option(DAS_TELEGRAM_DISABLED "Disable dasTelegram (Telegram API bindings)" ON)
option(DAS_TOOLS_DISABLED "Disable dasTools" OFF)
option(DAS_AOT_EXAMPLES_DISABLED "Disable dasAOT examples" OFF)
option(DAS_TUTORIAL_DISABLED "Disable dasTutorial" OFF)
option(DAS_TESTS_DISABLED "Disable dasTests" OFF)
option(DAS_BUILD_DOCUMENTATION "Build HTML documentation via Sphinx during install" OFF)
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

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

MACRO(DAS_AOT_EXT input_files genList mainTarget dasAotTool dasAotToolArg)
    set(all_depends ${dasAotTool})
    set(all_outputs "")
    set(command_args "")
    list(LENGTH input_files num_files)

    message(STATUS "DAS_AOT_EXT called on ${mainTarget} with files number: ${num_files}")

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
FILE(GLOB _modules RELATIVE "${PROJECT_SOURCE_DIR}/modules" "modules/*")

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
    INCLUDE(modules/${_module}/CMakeLists.txt OPTIONAL)
ENDFOREACH()

# Clean stale .shared_module files. When a module is disabled, its .shared_module
# from a previous build may remain and cause crashes at startup (the dynamic module
# loader tries to load them even though the module code is no longer linked).
FILE(GLOB_RECURSE _all_shared_modules "${PROJECT_SOURCE_DIR}/modules/*.shared_module")
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
src/ast/ast_aot_cpp.cpp
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
src/ast/ast_inscope_pod.cpp
src/ast/ast_unused.cpp
src/ast/ast_annotations.cpp
src/ast/ast_export.cpp
src/ast/ast_parse.cpp
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
src/builtin/module_builtin_das.cpp
src/builtin/module_builtin_math.cpp
src/builtin/module_builtin_raster.cpp
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
src/builtin/builtin.das.inc
src/builtin/builtin.das
src/builtin/fio.das.inc
src/builtin/fio.das
src/builtin/rtti.das.inc
src/builtin/rtti.das
src/builtin/ast.das.inc
src/builtin/ast.das
src/builtin/network.das.inc
src/builtin/network.das
src/builtin/debugger.das.inc
src/builtin/debugger.das
)
list(SORT BUILTIN_SRC)
SOURCE_GROUP_FILES("module builtin" BUILTIN_SRC)
CMAKE_TEXT_XXD(libDaScript src/builtin/builtin.das)
CMAKE_TEXT_XXD(libDaScript src/builtin/fio.das)
CMAKE_TEXT_XXD(libDaScript src/builtin/rtti.das)
CMAKE_TEXT_XXD(libDaScript src/builtin/ast.das)
CMAKE_TEXT_XXD(libDaScript src/builtin/network.das)
CMAKE_TEXT_XXD(libDaScript src/builtin/debugger.das)

SET(MISC_SRC
include/daScript/misc/enums.h
include/daScript/misc/hal.h
include/daScript/misc/fpe.h
include/daScript/misc/copy_bytes.h
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
include/daScript/misc/job_que.h
include/daScript/misc/uric.h
src/misc/sysos.cpp
src/misc/string_writer.cpp
src/misc/memory_model.cpp
src/misc/job_que.cpp
src/misc/free_list.cpp
src/misc/daScriptC.cpp
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
src/hal/project_specific.cpp
src/hal/project_specific_file_info.cpp
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
include/daScript/simulate/debug_print.h
include/daScript/simulate/for_each.h
include/daScript/simulate/bind_enum.h
include/daScript/simulate/bin_serializer.h
src/simulate/bin_serializer.cpp
include/daScript/simulate/aot.h
include/daScript/simulate/aot_library.h
include/daScript/simulate/aot_builtin.h
include/daScript/simulate/aot_builtin_math.h
include/daScript/simulate/aot_builtin_raster.h
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

SET(FLAT_HASH_MAP_SRC
include/ska/flat_hash_map.hpp
)
SOURCE_GROUP_FILES("flat_hash_map" FLAT_HASH_MAP_SRC)

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

MACRO(SETUP_LIBDASCRIPT library shared_lib)
    IF(${shared_lib})
        SETUP_DAS_SHARED_LIBRARY(${library} ${PARSER_GENERATED_SRC}
                ${PARSER_SRC} ${VECMATH_SRC} ${AST_SRC} ${SIMULATE_SRC} ${BUILTIN_SRC}
                ${MISC_SRC} ${SIMULATE_FUSION_SRC} ${MAIN_SRC}
                ${DAGOR_NOISE_SRC} ${FLAT_HASH_MAP_SRC} ${FAST_FLOAT_SRC} ${DASCRIPT_FMT_SRC})
        TARGET_COMPILE_DEFINITIONS(${library} PRIVATE DAS_EXPORTS=1 DAS_MOD_EXPORTS=1)
        TARGET_COMPILE_DEFINITIONS(${library} PUBLIC DAS_ENABLE_DLL=1)
        # Set DAS_EXPORTS to export all MOD symbols from libDaScript
    ELSE()
        ADD_LIBRARY(${library} STATIC ${PARSER_GENERATED_SRC}
                ${PARSER_SRC} ${VECMATH_SRC} ${AST_SRC} ${SIMULATE_SRC} ${BUILTIN_SRC}
                ${MISC_SRC} ${SIMULATE_FUSION_SRC} ${MAIN_SRC}
                ${DAGOR_NOISE_SRC} ${FLAT_HASH_MAP_SRC} ${FAST_FLOAT_SRC} ${DASCRIPT_FMT_SRC})
    ENDIF()
    TARGET_COMPILE_DEFINITIONS(${library} PUBLIC DAS_ENABLE_DYN_INCLUDES=1)
    ADD_DEPENDENCIES(${library} need_and_resolve)
    ADD_TARGET_PROJECT_XXD_DEPENDS(${library} libDaScript)
    target_include_directories(${library} PRIVATE
            ${DAS_SMMALLOC_DIR}
            ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/uriparser/include
            ${CMAKE_CURRENT_SOURCE_DIR}/include
    )
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
    SETUP_CPP11(${library})
    target_compile_features(${library} PUBLIC cxx_std_17)
    #target_precompile_headers(${library} PUBLIC include/daScript/misc/platform.h)

    # Static modules generated include.
    target_include_directories(${library} PUBLIC
        $<BUILD_INTERFACE:${NEED_MODULES_PATH}>
        $<INSTALL_INTERFACE:include>
    )
ENDMACRO()

SETUP_LIBDASCRIPT(libDaScript OFF)
SETUP_LIBDASCRIPT(libDaScriptDyn ON)

if(NOT ${DAS_AOT_EXAMPLES_DISABLED})

    if(${DAS_TOOLS_DISABLED})
        # error, we need tools to generate aot
        message(FATAL_ERROR "DAS_AOT_EXAMPLES_DISABLED requires DAS_TOOLS_DISABLED to be OFF")
    endif()

    set(AotDaslibList
        daslib/ast_boost.das
        daslib/functional.das
        daslib/json_boost.das
        daslib/json.das
        daslib/math_boost.das
        daslib/random.das
        daslib/regex_boost.das
        daslib/regex.das
        daslib/strings_boost.das
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
endif()

if (NOT ${DAS_TUTORIAL_DISABLED})
    include(tutorials/integration/c/CMakeLists.txt)
    include(tutorials/integration/cpp/CMakeLists.txt)
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

    add_executable(das-fmt ${PROJECT_SOURCE_DIR}/utils/dasFormatter/main.cpp)
    TARGET_LINK_LIBRARIES(das-fmt libDaScript ${SRC_LIBRARIES} ${DAS_MODULES_LIBS})
    target_include_directories(das-fmt PRIVATE ${NEED_MODULES_PATH})

    SETUP_CPP11(das-fmt)
    SETUP_LTO(das-fmt)
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

    # Thread-safety test (issue #2027) — static link
    add_executable(test_concurrent_init ${PROJECT_SOURCE_DIR}/tests/threading/test_concurrent_init.cpp)
    TARGET_LINK_LIBRARIES(test_concurrent_init libDaScript ${SRC_LIBRARIES} ${DAS_MODULES_LIBS})
    target_include_directories(test_concurrent_init PRIVATE ${NEED_MODULES_PATH})
    SETUP_CPP11(test_concurrent_init)

    # Thread-safety test (issue #2027) — dynamic link (DLL)
    add_executable(test_concurrent_init_dyn ${PROJECT_SOURCE_DIR}/tests/threading/test_concurrent_init.cpp)
    TARGET_LINK_LIBRARIES(test_concurrent_init_dyn libDaScriptDyn ${SRC_LIBRARIES})
    ADD_DEPENDENCIES(test_concurrent_init_dyn libDaScriptDyn ${DAS_DYN_MODULES_LIBS})
    target_include_directories(test_concurrent_init_dyn PRIVATE ${NEED_MODULES_PATH})
    SETUP_CPP11(test_concurrent_init_dyn)

endif()

# This list should be significantly reduced, most of the files, except aot related should be private.
SET(DAS_RELEASE_AST_INCLUDE
    include/daScript/ast/aot_templates.h
    include/daScript/ast/ast_aot_cpp.h
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
    include/daScript/ast/ast_typedecl.h
    include/daScript/ast/ast_typefactory_bind.h
    include/daScript/ast/ast_typefactory.h
    include/daScript/ast/ast_visitor.h
    include/daScript/ast/compilation_errors.h
    include/daScript/ast/dyn_modules.h
)

SET(DAS_RELEASE_C_API
    include/daScript/daScriptC.h
)

SET(DAS_RELEASE_INCLUDE
    include/daScript/daScript.h
    include/daScript/daScriptModule.h
    include/daScript/das_config.h
    include/daScript/das_project_specific.h
)

SET(DAS_RELEASE_MISC_INCLUDE
    include/daScript/misc/anyhash.h
    include/daScript/misc/arraytype.h
    include/daScript/misc/callable.h
    include/daScript/misc/copy_bytes.h
    include/daScript/misc/debug_break.h
    include/daScript/misc/enums.h
    include/daScript/misc/fpe.h
    include/daScript/misc/free_list.h
    include/daScript/misc/hal.h
    include/daScript/misc/hash.h
    include/daScript/misc/instance_debugger.h
    include/daScript/misc/job_que.h
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
    include/daScript/simulate/aot_builtin_raster.h
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
    ${FLAT_HASH_MAP_SRC}
    DESTINATION include/ska
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
    libDaScript libDaScriptDyn libUriParser libUriParserDyn
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
install(DIRECTORY ${PROJECT_SOURCE_DIR}/install/skills DESTINATION ${DAS_INSTALL_DOCDIR})
install(FILES ${PROJECT_SOURCE_DIR}/LICENSE DESTINATION ${DAS_INSTALL_DOCDIR})
install(FILES ${PROJECT_SOURCE_DIR}/3rdparty/uriparser/COPYING DESTINATION ${DAS_INSTALL_DOCDIR} RENAME URIPARSER.LICENSE)
install(FILES ${PROJECT_SOURCE_DIR}/include/dag_noise/LICENSE DESTINATION ${DAS_INSTALL_DOCDIR} RENAME DAG_NOISE.LICENSE)
install(FILES ${PROJECT_SOURCE_DIR}/include/ska/LICENSE_1_0.txt DESTINATION ${DAS_INSTALL_DOCDIR} RENAME SKA.LICENSE)
install(FILES ${PROJECT_SOURCE_DIR}/include/vecmath/LICENSE DESTINATION ${DAS_INSTALL_DOCDIR} RENAME VEC_MATH.LICENSE)

# Install utility scripts
install(FILES ${PROJECT_SOURCE_DIR}/utils/aot/main.das DESTINATION utils/aot)
install(FILES ${PROJECT_SOURCE_DIR}/utils/dasCodeFormatter/main.das DESTINATION utils/dasCodeFormatter)

# Install core example .das files
install(FILES
    ${PROJECT_SOURCE_DIR}/examples/hello_world.das
    DESTINATION ${DAS_INSTALL_EXAMPLESDIR}
)

file(GLOB DAS_DEBUGAPI_EXAMPLES
    ${PROJECT_SOURCE_DIR}/examples/debugapi/*.das
)
install(FILES ${DAS_DEBUGAPI_EXAMPLES}
    DESTINATION ${DAS_INSTALL_EXAMPLESDIR}/debugapi
)

file(GLOB DAS_GRAPHICS_EXAMPLES
    ${PROJECT_SOURCE_DIR}/examples/graphics/*.das
)
install(FILES ${DAS_GRAPHICS_EXAMPLES}
    DESTINATION ${DAS_INSTALL_EXAMPLESDIR}/graphics
)

file(GLOB DAS_UNCATEGORIZED_EXAMPLES
    ${PROJECT_SOURCE_DIR}/examples/uncategorized/*.das
)
install(FILES ${DAS_UNCATEGORIZED_EXAMPLES}
    DESTINATION ${DAS_INSTALL_EXAMPLESDIR}/uncategorized
)

# Install language tutorial .das files
file(GLOB LANGUAGE_TUTORIAL_SOURCES
    ${PROJECT_SOURCE_DIR}/tutorials/language/*.das
)
install(FILES ${LANGUAGE_TUTORIAL_SOURCES}
    DESTINATION ${DAS_INSTALL_TUTORIALSDIR}/language
)

# Install macro tutorial .das files
file(GLOB MACRO_TUTORIAL_SOURCES
    ${PROJECT_SOURCE_DIR}/tutorials/macros/*.das
)
install(FILES ${MACRO_TUTORIAL_SOURCES}
    DESTINATION ${DAS_INSTALL_TUTORIALSDIR}/macros
)

# Install dasHV tutorial .das files
file(GLOB DASHV_TUTORIAL_SOURCES
    ${PROJECT_SOURCE_DIR}/tutorials/dasHV/*.das
)
install(FILES ${DASHV_TUTORIAL_SOURCES}
    DESTINATION ${DAS_INSTALL_TUTORIALSDIR}/dasHV
)

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
