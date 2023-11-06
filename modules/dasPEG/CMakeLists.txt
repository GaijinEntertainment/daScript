IF(NOT DAS_PEG_INCLUDED)
    SET(DAS_PEG_INCLUDED TRUE)
    MESSAGE(STATUS "dasPEG module included.")

	ADD_MODULE_DAS(peg peg peg)
    ADD_MODULE_DAS(peg peg meta_ast)
	ADD_MODULE_DAS(peg peg parse_macro)
	ADD_MODULE_DAS(peg peg parser_generator)

    install(DIRECTORY ${PROJECT_SOURCE_DIR}/modules/dasPEG/peg
        DESTINATION ${DAS_INSTALL_MODULESDIR}/dasPEG
        FILES_MATCHING
        PATTERN "*.das"
    )
ENDIF()
