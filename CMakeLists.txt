cmake_minimum_required (VERSION 3.20)
project (DAS)

INCLUDE(./CMakeCommon.txt)

IF(MSVC)
add_compile_options($<$<CXX_COMPILER_ID:MSVC>:/MP>)
ENDIF()

IF(DEFINED DAS_CONFIG_INCLUDE_DIR)
    INCLUDE_DIRECTORIES(${DAS_CONFIG_INCLUDE_DIR})
ENDIF()

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(EXECUTABLE_OUTPUT_PATH  ${PROJECT_SOURCE_DIR}/bin/)

SET(LINUX_UUID FALSE)

FIND_XXD()
FIND_FLEX_AND_BISON()

SETUP_COMPILER()

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

MACRO(DAS_AOT input genList mainTarget dasAotTool)
    get_filename_component(input_src ${input} ABSOLUTE)
    get_filename_component(input_dir ${input_src} DIRECTORY)
    get_filename_component(input_name ${input} NAME)
    set(out_dir ${input_dir}/_aot_generated)
    set(out_src ${out_dir}/${input_name}.cpp)
    file(MAKE_DIRECTORY ${out_dir})
	  ADD_CUSTOM_COMMAND(
		    DEPENDS ${input_src}
        DEPENDS ${dasAotTool}
        OUTPUT  ${out_src}
        COMMENT "AOT precompiling ${input_src} -> ${out_src}"
        COMMAND ${dasAotTool} -aot ${input_src} ${out_src}
    )
    list(APPEND ${genList} ${out_src})
    set(custom_name ${mainTarget}_${input_name}_aot)
    ADD_CUSTOM_TARGET(${custom_name} DEPENDS ${out_src})
    SET_TARGET_PROPERTIES(${custom_name} PROPERTIES FOLDER _${mainTarget}_aot)
    ADD_DEPENDENCIES(${mainTarget} ${custom_name})
ENDMACRO()

MACRO(DAS_AOT_D input genList mainTarget dasAotTool)
    get_filename_component(input_src ${input} ABSOLUTE)
    get_filename_component(input_dir ${input_src} DIRECTORY)
    get_filename_component(input_name ${input} NAME)
    set(out_dir ${input_dir}/_aot_generated)
    set(out_src ${out_dir}/${input_name}.cpp)
    file(MAKE_DIRECTORY ${out_dir})
	ADD_CUSTOM_COMMAND(
		DEPENDS ${input_src}
        DEPENDS ${dasAotTool}
        OUTPUT  ${out_src}
        COMMENT "AOT precompiling ${input_src} -> ${out_src}"
        COMMAND ${dasAotTool} -aot ${input_src} ${out_src}
    )
    list(APPEND ${genList} ${out_src})
ENDMACRO()

SET(AOT_BATCH_SIZE 10)

MACRO(DAS_AOT_DIR inFiles genList mainTarget dasAotTool)
    set(file_index 0)
    set(batch_index 0)
    set(batch_list)
    FOREACH(inF ${inFiles})
        DAS_AOT_D(${inF} batch_list ${mainTarget} ${dasAotTool})
        math(EXPR file_index "${file_index} + 1")
        if(file_index EQUAL ${AOT_BATCH_SIZE})
            set(custom_name ${mainTarget}_${batch_index}_aot)
            ADD_CUSTOM_TARGET(${custom_name} ALL DEPENDS ${batch_list})
            ADD_DEPENDENCIES(${mainTarget} ${custom_name})
            list(APPEND ${genList} ${batch_list})
            set(file_index 0)
            math(EXPR batch_index "${batch_index} + 1")
            set(batch_list)
        endif()
    ENDFOREACH()
    if(NOT(file_index EQUAL "0"))
        set(custom_name ${mainTarget}_${batch_index}_aot)
        ADD_CUSTOM_TARGET(${custom_name} ALL DEPENDS ${batch_list})
        ADD_DEPENDENCIES(${mainTarget} ${custom_name})
        list(APPEND ${genList} ${batch_list})
    endif()
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
        ADD_DEPENDENCIES(${tgt} daScript)
        ADD_CUSTOM_COMMAND(
            TARGET ${tgt}
            WORKING_DIRECTORY ${src_dir}
            VERBATIM
            COMMAND ${CLANG_BIN_EXE} -Xclang -ast-dump=json -x c++ -c ${header_from} -I${inc_dirs} > ${header_from}.json 2> ${header_from}.json.log || cmd /c exit /b 0
            COMMENT "Generating JSON AST for ${header_from} to ${header_from}.json\ninclude directoryes are ${inc_dirs}\n"
        )
        ADD_CUSTOM_COMMAND(
            TARGET ${tgt}
            WORKING_DIRECTORY ${src_dir}
            VERBATIM
            COMMAND daScript ${generate_das} -args ${header_from}.json ${prefix} "${extras}"
            COMMENT "Generating C++ bindings from ${header_from}.json\n"
        )
    ENDMACRO()

    MACRO(DAS_OUTPUT_AST tgt header_from src_dir inc_dirs)
        MESSAGE(STATUS "Generating C++ bindings target ${tgt} from ${header_from}")
        ADD_CUSTOM_TARGET(${tgt})
        ADD_DEPENDENCIES(${tgt} daScript)
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
        ADD_DEPENDENCIES(${tgt} daScript)
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

SET(DAS_MODULES_RESOLVE_INC ${PROJECT_SOURCE_DIR}/include/modules/external_resolve.inc)
SET(DAS_MODULES_NEED_INC ${PROJECT_SOURCE_DIR}/include/modules/external_need.inc)
FILE(WRITE ${DAS_MODULES_RESOLVE_INC}.temp "")
FILE(WRITE ${DAS_MODULES_NEED_INC}.temp "")

SET(DAS_MODULES_LIBS)
SET(DAS_ALL_EXAMPLES)
FILE(GLOB _modules RELATIVE "${PROJECT_SOURCE_DIR}/modules" "modules/*")
FOREACH(_module ${_modules})
    MACRO(ADD_MODULE_LIB lib)
        MESSAGE("REGISTER DAS MODULE ${lib}")
        LIST(APPEND DAS_MODULES_LIBS ${lib})
    ENDMACRO()

    MACRO(ADD_EXAMPLE tgt)
        MESSAGE("EXAMPLE ${tgt}")
        LIST(APPEND DAS_ALL_EXAMPLES ${tgt})
    ENDMACRO()

    FUNCTION(ADD_MODULE_CPP cpp)
        FILE(APPEND ${DAS_MODULES_NEED_INC}.temp "NEED_MODULE(Module_${cpp});\n")
    ENDFUNCTION()

    FUNCTION(ADD_MODULE_NATIVE native)
        FILE(APPEND ${DAS_MODULES_RESOLVE_INC}.temp "NATIVE_MODULE(\"daslib\",\"daslib\", ${_module}, ${native});\n")
    ENDFUNCTION()

    FUNCTION(ADD_MODULE_DAS category subfolder native)
        FILE(APPEND ${DAS_MODULES_RESOLVE_INC}.temp "NATIVE_MODULE(${category}, ${subfolder}, ${_module}, ${native});\n")
    ENDFUNCTION()


    INCLUDE(modules/${_module}/CMakeLists.txt OPTIONAL)
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

ADD_CUSTOM_TARGET(need_and_resolve ALL DEPENDS ${DAS_MODULES_NEED_INC} ${DAS_MODULES_RESOLVE_INC})

FLEX(src/parser/ds_lexer.lpp)
BISON(src/parser/ds_parser.ypp)


SET(PARSER_GENERATED_SRC
src/parser/ds_parser.hpp
src/parser/ds_parser.cpp
src/parser/ds_parser.output
src/parser/ds_lexer.cpp
src/parser/lex.yy.h
src/parser/parser_state.h
)

SET(PARSER_SRC
src/parser/ds_parser.ypp
src/parser/ds_lexer.lpp
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
)
list(SORT VECMATH_SRC)
SOURCE_GROUP_FILES("vecmath" VECMATH_SRC)

SET(AST_SRC
src/ast/ast.cpp
src/ast/ast_visitor.cpp
src/ast/ast_generate.cpp
src/ast/ast_simulate.cpp
src/ast/ast_typedecl.cpp
src/ast/ast_match.cpp
src/ast/ast_module.cpp
src/ast/ast_print.cpp
src/ast/ast_aot_cpp.cpp
src/ast/ast_infer_type.cpp
src/ast/ast_lint.cpp
src/ast/ast_allocate_stack.cpp
src/ast/ast_const_folding.cpp
src/ast/ast_block_folding.cpp
src/ast/ast_unused.cpp
src/ast/ast_annotations.cpp
src/ast/ast_export.cpp
src/ast/ast_parse.cpp
src/ast/ast_debug_info_helper.cpp
src/ast/ast_handle.cpp
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
src/builtin/module_builtin_string.cpp
src/builtin/module_builtin_rtti.h
src/builtin/module_builtin_rtti.cpp
src/builtin/module_builtin_ast.cpp
src/builtin/module_builtin_ast.h
src/builtin/ast_gen.inc
src/builtin/module_builtin_fio.cpp
src/builtin/module_builtin_dasbind.cpp
src/builtin/module_builtin_network.cpp
src/builtin/module_builtin_debugger.cpp
src/builtin/module_builtin_jobque.cpp
src/builtin/debugapi_gen.inc
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
XXD(src/builtin/builtin.das)
XXD(src/builtin/fio.das)
XXD(src/builtin/rtti.das)
XXD(src/builtin/ast.das)
XXD(src/builtin/network.das)
XXD(src/builtin/debugger.das)

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
include/daScript/misc/fnv.h
include/daScript/misc/safebox.h
include/daScript/misc/smart_ptr.h
include/daScript/misc/free_list.h
include/daScript/misc/sysos.h
include/daScript/misc/callable.h
include/daScript/misc/debug_break.h
include/daScript/misc/instance_debugger.h
include/daScript/misc/job_que.h
src/misc/sysos.cpp
src/misc/string_writer.cpp
src/misc/memory_model.cpp
src/misc/job_que.cpp
src/misc/job_que.cpp
src/misc/daScriptC.cpp
src/misc/smmalloc_integration.cpp
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
include/daScript/misc/network.h
src/misc/network.cpp
src/simulate/hash.cpp
src/simulate/debug_info.cpp
src/simulate/runtime_string.cpp
src/simulate/runtime_array.cpp
src/simulate/runtime_table.cpp
src/simulate/runtime_range.cpp
src/simulate/runtime_profile.cpp
src/simulate/simulate.cpp
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
include/daScript/simulate/fs_file_info.h
src/simulate/fs_file_info.cpp
${DAS_MODULES_RESOLVE_INC}
)
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

SET(DAS_SMMALLOC_DIR ${CMAKE_SOURCE_DIR}/3rdparty/smmalloc)

SET(SMMALLOC_SRC
${DAS_SMMALLOC_DIR}/smmalloc.cpp
${DAS_SMMALLOC_DIR}/smmalloc.h
${DAS_SMMALLOC_DIR}/smmalloc_generic.cpp
${DAS_SMMALLOC_DIR}/smmalloc_tls.cpp
)

include_directories(include)
ADD_LIBRARY(libDaScript ${VECMATH_SRC} ${AST_SRC} ${BUILTIN_SRC} ${MISC_SRC} ${SIMULATE_SRC}
    ${SIMULATE_FUSION_SRC} ${TEST_SRC} ${MAIN_SRC} ${PARSER_SRC} ${PARSER_GENERATED_SRC}
	${DAGOR_NOISE_SRC} ${FLAT_HASH_MAP_SRC} ${DAS_LIB_SRC} ${SMMALLOC_SRC})
target_include_directories(libDaScript PUBLIC ${DAS_SMMALLOC_DIR})
IF(LINUX_UUID)
target_link_libraries(libDaScript uuid)
ENDIF()
SETUP_CPP11(libDaScript)
#target_precompile_headers(libDaScript PUBLIC include/daScript/misc/platform.h)

add_custom_target(dasAotStub)
SET(AOT_GENERATED_SRC)
DAS_AOT("daslib/functional.das" AOT_GENERATED_SRC dasAotStub daScript)
DAS_AOT("daslib/json.das" AOT_GENERATED_SRC dasAotStub daScript)
DAS_AOT("daslib/json_boost.das" AOT_GENERATED_SRC dasAotStub daScript)
DAS_AOT("daslib/regex.das" AOT_GENERATED_SRC dasAotStub daScript)
DAS_AOT("daslib/regex_boost.das" AOT_GENERATED_SRC dasAotStub daScript)
DAS_AOT("daslib/strings_boost.das" AOT_GENERATED_SRC dasAotStub daScript)
DAS_AOT("daslib/random.das" AOT_GENERATED_SRC dasAotStub daScript)
DAS_AOT("daslib/math_boost.das" AOT_GENERATED_SRC dasAotStub daScript)
SOURCE_GROUP_FILES("aot stub" AOT_GENERATED_SRC)
#UNITIZE_BUILD("daslib" AOT_GENERATED_SRC)

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
SETUP_CPP11(libUriParser)
target_compile_definitions(libUriParser PUBLIC URIPARSER_BUILD_CHAR)
target_compile_definitions(libUriParser PUBLIC URI_STATIC_BUILD)
target_include_directories(libUriParser PUBLIC
  ${CMAKE_SOURCE_DIR}/3rdparty/uriparser/include
)

# MODULE - uri parser

SET(MODULE_URIPARSE_MAIN_SRC
include/modules/uriparser/module_uriparser.h
src/modules/uriparser/module_uriparser.cpp
)

ADD_LIBRARY(libDasModuleUriparser ${MODULE_URIPARSE_MAIN_SRC})
SETUP_CPP11(libDasModuleUriparser)
target_include_directories(libDasModuleUriparser PUBLIC
  ${CMAKE_SOURCE_DIR}/3rdparty/uriparser/include
  include/modules/uriparser
)
ADD_DEPENDENCIES(libDasModuleUriparser libDaScript libUriParser)

if (NOT ${DAS_BUILD_TOOLS} MATCHES NO)
# Stand alone command line compiler

  SET(DASCRIPT_MAIN_SRC
  utils/daScript/main.cpp
  ${DAS_MODULES_NEED_INC}
  )
  SOURCE_GROUP_FILES("main" DASCRIPT_MAIN_SRC)

  add_executable(daScript ${DASCRIPT_MAIN_SRC})
  TARGET_LINK_LIBRARIES(daScript libDaScript libDasModuleUriparser libUriParser libDaScriptProfile libDaScriptTest Threads::Threads ${DAS_MODULES_LIBS})
  ADD_DEPENDENCIES(daScript libDaScript libDasModuleUriparser libUriParser libDaScriptProfile libDaScriptTest  need_and_resolve ${DAS_MODULES_LIBS})
  SETUP_CPP11(daScript)
  SETUP_LTO(daScript)

  #IF(APPLE)
  #  add_executable(daScriptOsx MACOSX_BUNDLE ${DASCRIPT_MAIN_SRC})
  #  TARGET_LINK_LIBRARIES(daScriptOsx libDaScript libDasModuleUriparser libUriParser libDaScriptProfile libDaScriptTest Threads::Threads ${DAS_MODULES_LIBS})
  #  ADD_DEPENDENCIES(daScriptOsx libDaScript libDasModuleUriparser libUriParser libDaScriptProfile libDaScriptTest  need_and_resolve ${DAS_MODULES_LIBS})
  #  SETUP_CPP11(daScriptOsx)
  #  SETUP_LTO(daScriptOsx)
  #  set_target_properties(daScriptOsx PROPERTIES MACOSX_BUNDLE_BUNDLE_NAME "daScript")
  #  set_target_properties(daScriptOsx PROPERTIES
  #      MACOSX_BUNDLE_SHORT_VERSION_STRING "0.3"
  #      MACOSX_BUNDLE_LONG_VERSION_STRING  "daScript 0.3"
  #      MACOSX_BUNDLE_INFO_PLIST ${CMAKE_SOURCE_DIR}/utils/daScript/MacOSXBundleInfo.plist.in)
  #ENDIF()

endif()

# Test module

SET(TEST_MAIN_LIB_SRC
examples/test/test_handles.cpp
examples/test/test_enum.cpp
examples/test/module_unitTest.h
examples/test/unitTest.h
examples/test/unit_test.das.inc
examples/test/unit_test.das
)
SOURCE_GROUP_FILES("source" TEST_MAIN_LIB_SRC)

XXD(examples/test/unit_test.das)

ADD_LIBRARY(libDaScriptTest ${TEST_MAIN_LIB_SRC})
SETUP_CPP11(libDaScriptTest)
ADD_DEPENDENCIES(libDaScriptTest libDaScript)

# Test
if (NOT ${DAS_BUILD_TEST} MATCHES NO)
  add_subdirectory(examples/test)
endif()

# Profile module

SET(PROFILE_MAIN_LIB_SRC
examples/profile/test_profile.cpp
examples/profile/test_profile.h
)
SOURCE_GROUP_FILES("source" PROFILE_MAIN_LIB_SRC)

ADD_LIBRARY(libDaScriptProfile ${PROFILE_MAIN_LIB_SRC})
SETUP_CPP11(libDaScriptProfile)
ADD_DEPENDENCIES(libDaScriptProfile libDaScript)

if (NOT ${DAS_BUILD_PROFILE} MATCHES NO)
  add_subdirectory(examples/profile)
endif()

if (NOT ${DAS_BUILD_TUTORIAL} MATCHES NO)
  add_subdirectory(examples/tutorial)
endif()
