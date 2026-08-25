IF ((NOT DAS_TREE_SITTER_INCLUDED) AND (NOT DAS_TREE_SITTER_DISABLED))
    SET(DAS_TREE_SITTER_INCLUDED TRUE)
    MESSAGE(STATUS "dasTreeSitter module included.")

    SET(DAS_TREE_SITTER_DIR ${CMAKE_CURRENT_LIST_DIR})
    SET(DAS_TREE_SITTER_RUNTIME_DIR ${DAS_TREE_SITTER_DIR}/3rdparty/tree-sitter)

    FUNCTION(DAS_TREE_SITTER_LANGUAGE_OBJECT target source_dir)
        ADD_LIBRARY(${target} OBJECT ${ARGN})
        TARGET_INCLUDE_DIRECTORIES(${target} PRIVATE ${source_dir})
        SET_TARGET_PROPERTIES(${target} PROPERTIES
            C_STANDARD 11
            C_STANDARD_REQUIRED ON
            POSITION_INDEPENDENT_CODE ON)
        IF(MSVC)
            TARGET_COMPILE_OPTIONS(${target} PRIVATE /W0)
        ELSE()
            TARGET_COMPILE_OPTIONS(${target} PRIVATE -w)
        ENDIF()
    ENDFUNCTION()

    # Keep each generated parser paired with the parser.h it was generated
    # against. The bundled languages currently span Tree-sitter ABI 14 and 15.
    DAS_TREE_SITTER_LANGUAGE_OBJECT(tree_sitter_daslang_objects
        ${PROJECT_SOURCE_DIR}/tree-sitter-daslang/src
        ${PROJECT_SOURCE_DIR}/tree-sitter-daslang/src/parser.c
        ${PROJECT_SOURCE_DIR}/tree-sitter-daslang/src/scanner.c)
    DAS_TREE_SITTER_LANGUAGE_OBJECT(tree_sitter_c_objects
        ${DAS_TREE_SITTER_DIR}/languages/c
        ${DAS_TREE_SITTER_DIR}/languages/c/parser.c)
    DAS_TREE_SITTER_LANGUAGE_OBJECT(tree_sitter_cpp_objects
        ${DAS_TREE_SITTER_DIR}/languages/cpp
        ${DAS_TREE_SITTER_DIR}/languages/cpp/parser.c
        ${DAS_TREE_SITTER_DIR}/languages/cpp/scanner.c)
    DAS_TREE_SITTER_LANGUAGE_OBJECT(tree_sitter_markdown_objects
        ${DAS_TREE_SITTER_DIR}/languages/markdown
        ${DAS_TREE_SITTER_DIR}/languages/markdown/parser.c
        ${DAS_TREE_SITTER_DIR}/languages/markdown/scanner.c)
    DAS_TREE_SITTER_LANGUAGE_OBJECT(tree_sitter_markdown_inline_objects
        ${DAS_TREE_SITTER_DIR}/languages/markdown_inline
        ${DAS_TREE_SITTER_DIR}/languages/markdown_inline/parser.c
        ${DAS_TREE_SITTER_DIR}/languages/markdown_inline/scanner.c)

    ADD_LIBRARY(dasTreeSitterRuntime STATIC
        ${DAS_TREE_SITTER_RUNTIME_DIR}/lib/src/lib.c
        $<TARGET_OBJECTS:tree_sitter_daslang_objects>
        $<TARGET_OBJECTS:tree_sitter_c_objects>
        $<TARGET_OBJECTS:tree_sitter_cpp_objects>
        $<TARGET_OBJECTS:tree_sitter_markdown_objects>
        $<TARGET_OBJECTS:tree_sitter_markdown_inline_objects>)
    TARGET_INCLUDE_DIRECTORIES(dasTreeSitterRuntime PRIVATE
        ${DAS_TREE_SITTER_RUNTIME_DIR}/lib/include
        ${DAS_TREE_SITTER_RUNTIME_DIR}/lib/src)
    TARGET_COMPILE_DEFINITIONS(dasTreeSitterRuntime PRIVATE
        _POSIX_C_SOURCE=200112L _DEFAULT_SOURCE _BSD_SOURCE _DARWIN_C_SOURCE)
    SET_TARGET_PROPERTIES(dasTreeSitterRuntime PROPERTIES
        C_STANDARD 11
        C_STANDARD_REQUIRED ON
        POSITION_INDEPENDENT_CODE ON)
    IF(MSVC)
        TARGET_COMPILE_OPTIONS(dasTreeSitterRuntime PRIVATE /W0)
    ELSE()
        TARGET_COMPILE_OPTIONS(dasTreeSitterRuntime PRIVATE -w)
    ENDIF()

    FILE(READ ${PROJECT_SOURCE_DIR}/tree-sitter-daslang/queries/highlights.scm
        DAS_TS_DASLANG_QUERY)
    FILE(READ ${DAS_TREE_SITTER_DIR}/languages/c/highlights.scm DAS_TS_C_QUERY)
    FILE(READ ${DAS_TREE_SITTER_DIR}/languages/cpp/highlights.scm DAS_TS_CPP_ONLY_QUERY)
    FILE(READ ${DAS_TREE_SITTER_DIR}/languages/markdown/highlights.scm
        DAS_TS_MARKDOWN_QUERY)
    FILE(READ ${DAS_TREE_SITTER_DIR}/languages/markdown_inline/highlights.scm
        DAS_TS_MARKDOWN_INLINE_QUERY)
    # tree-sitter-cpp intentionally publishes only its C++ overrides. Its
    # complete highlight query is the C query followed by those overrides.
    SET(DAS_TS_CPP_QUERY "${DAS_TS_C_QUERY}\n${DAS_TS_CPP_ONLY_QUERY}")
    CONFIGURE_FILE(${DAS_TREE_SITTER_DIR}/src/dasTreeSitterQueries.h.in
        ${CMAKE_CURRENT_BINARY_DIR}/dasTreeSitterQueries.h @ONLY)

    SET(DAS_TREE_SITTER_MODULE_SRC
        ${DAS_TREE_SITTER_DIR}/src/aot_builtin_tree_sitter.h
        ${DAS_TREE_SITTER_DIR}/src/dasTreeSitter.cpp)

    ADD_MODULE_CPP(TreeSitter)
    ADD_MODULE_DAS_FROM_DESCRIPTOR(tree_sitter daslib)
    ADD_MODULE_LIB(libDasModuleTreeSitter dasModuleTreeSitter
        ${DAS_TREE_SITTER_MODULE_SRC})

    FOREACH(target libDasModuleTreeSitter dasModuleTreeSitter)
        TARGET_INCLUDE_DIRECTORIES(${target} PRIVATE
            ${DAS_TREE_SITTER_RUNTIME_DIR}/lib/include
            ${CMAKE_CURRENT_BINARY_DIR}
            ${DAS_TREE_SITTER_DIR}/src)
        TARGET_LINK_LIBRARIES(${target} PRIVATE dasTreeSitterRuntime)
        SETUP_CPP11(${target})
    ENDFOREACH()

    INSTALL(DIRECTORY ${DAS_TREE_SITTER_DIR}/daslib
        DESTINATION ${DAS_INSTALL_MODULESDIR}/dasTreeSitter
        FILES_MATCHING PATTERN "*.das")
    INSTALL(FILES ${DAS_TREE_SITTER_DIR}/src/aot_builtin_tree_sitter.h
        DESTINATION ${DAS_INSTALL_MODULESDIR}/dasTreeSitter/src)
    INSTALL(TARGETS dasTreeSitterRuntime
        EXPORT DASTargets
        ARCHIVE DESTINATION ${DAS_INSTALL_LIBDIR})
    # markdown_inline ships the identical MIT text, so one install covers both grammars
    INSTALL(FILES ${DAS_TREE_SITTER_DIR}/3rdparty/tree-sitter/LICENSE
        DESTINATION ${DAS_INSTALL_DOCDIR} RENAME TREE_SITTER.LICENSE)
    INSTALL(FILES ${DAS_TREE_SITTER_DIR}/3rdparty/tree-sitter/lib/src/unicode/LICENSE
        DESTINATION ${DAS_INSTALL_DOCDIR} RENAME TREE_SITTER_ICU.LICENSE)
    INSTALL(FILES ${DAS_TREE_SITTER_DIR}/languages/c/LICENSE
        DESTINATION ${DAS_INSTALL_DOCDIR} RENAME TREE_SITTER_C.LICENSE)
    INSTALL(FILES ${DAS_TREE_SITTER_DIR}/languages/cpp/LICENSE
        DESTINATION ${DAS_INSTALL_DOCDIR} RENAME TREE_SITTER_CPP.LICENSE)
    INSTALL(FILES ${DAS_TREE_SITTER_DIR}/languages/markdown/LICENSE
        DESTINATION ${DAS_INSTALL_DOCDIR} RENAME TREE_SITTER_MARKDOWN.LICENSE)
ENDIF()
