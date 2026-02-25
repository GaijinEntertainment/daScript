IF(NOT DAS_OPENAI_INCLUDED AND ((NOT ${DAS_OPENAI_DISABLED}) OR (NOT DEFINED DAS_OPENAI_DISABLED)))
    SET(DAS_OPENAI_INCLUDED TRUE)
    MESSAGE(STATUS "dasOpenAI module included.")
    ADD_MODULE_DAS(openai openai openai)
    install(DIRECTORY ${PROJECT_SOURCE_DIR}/modules/dasOpenAI/openai
        DESTINATION ${DAS_INSTALL_MODULESDIR}/dasOpenAI
        FILES_MATCHING
        PATTERN "*.das"
    )
    file(GLOB DAS_OPENAI_EXAMPLES
        ${PROJECT_SOURCE_DIR}/modules/dasOpenAI/examples/*.das
    )

    install(FILES ${DAS_OPENAI_EXAMPLES}
        DESTINATION ${DAS_INSTALL_EXAMPLESDIR}/openai
    )
ENDIF()
