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
    install(FILES ${PROJECT_SOURCE_DIR}/modules/dasPEG/.das_module
        DESTINATION ${DAS_INSTALL_MODULESDIR}/dasPEG
    )

    ADD_EXAMPLE_RUN(tutorials/dasPEG/01_hello_parser.das)
    ADD_EXAMPLE_RUN(tutorials/dasPEG/02_calculator.das)
    ADD_EXAMPLE_RUN(tutorials/dasPEG/03_csv_parser.das)
    ADD_EXAMPLE_RUN(tutorials/dasPEG/04_email_validator.das)
    ADD_EXAMPLE_RUN(tutorials/dasPEG/05_json_parser.das)
    ADD_EXAMPLE_RUN(tutorials/dasPEG/06_debugging.das)
    ADD_EXAMPLE_RUN(tutorials/dasPEG/07_basic.das)

ENDIF()
