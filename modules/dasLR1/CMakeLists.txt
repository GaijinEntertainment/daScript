IF(NOT DAS_LR1_INCLUDED)
    SET(DAS_LR1_INCLUDED TRUE)
    MESSAGE(STATUS "dasLR1 module included.")

    ADD_MODULE_DAS_FROM_DESCRIPTOR(lr1 lr1)

    install(DIRECTORY ${PROJECT_SOURCE_DIR}/modules/dasLR1/lr1
        DESTINATION ${DAS_INSTALL_MODULESDIR}/dasLR1
        FILES_MATCHING
        PATTERN "*.das"
    )
    install(FILES ${PROJECT_SOURCE_DIR}/modules/dasLR1/.das_module
        DESTINATION ${DAS_INSTALL_MODULESDIR}/dasLR1
    )

    das_lane_dastest_target(run_tests_daslr1 COMMENT "Testing dasLR1"
        FILES
            modules/dasLR1/tests/test_lr1.das
            modules/dasLR1/tests/test_gbnf.das
            modules/dasLR1/tests/test_json_schema.das
    )

ENDIF()
