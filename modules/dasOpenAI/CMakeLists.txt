IF(NOT DAS_OPENAI_INCLUDED)
    SET(DAS_OPENAI_INCLUDED TRUE)
    MESSAGE(STATUS "dasOpenAI module included.")
    ADD_MODULE_DAS(openai openai openai)
    install(DIRECTORY ${PROJECT_SOURCE_DIR}/modules/dasOpenAI/openai
        DESTINATION modules/dasOpenAI
        FILES_MATCHING
        PATTERN "*.das"
    )
    file(GLOB DAS_OPENAI_EXAMPLES
        ${PROJECT_SOURCE_DIR}/modules/dasOpenAI/examples/*.das
    )

    install(FILES ${DAS_OPENAI_EXAMPLES}
        DESTINATION examples/openai
    )
ENDIF()
