# dasOPENAI depends on dasHV (dashv/dashv_boost); only register it when dasHV is enabled,
# otherwise consumers hit confusing `require dashv/...` failures.
IF ((NOT DAS_OPENAI_INCLUDED) AND ((NOT ${DAS_HV_DISABLED}) OR (NOT DEFINED DAS_HV_DISABLED)))
    SET(DAS_OPENAI_INCLUDED TRUE)
    MESSAGE(STATUS "dasOPENAI module included.")

    # Pure-das module (HTTP transport via dasHV, JSON via daslib/json_boost).
    # One module per section; openai_common holds the shared client + error + transport.
    # require openai/openai_<section> -> modules/dasOPENAI/openai/openai_<section>.das
    ADD_MODULE_DAS_FROM_DESCRIPTOR(openai openai)

    install(DIRECTORY ${PROJECT_SOURCE_DIR}/modules/dasOPENAI/openai
        DESTINATION ${DAS_INSTALL_MODULESDIR}/dasOPENAI
        FILES_MATCHING
        PATTERN "*.das"
    )
    install(FILES ${PROJECT_SOURCE_DIR}/modules/dasOPENAI/.das_module
        DESTINATION ${DAS_INSTALL_MODULESDIR}/dasOPENAI
    )

    # The client's own tests: the wire shapes, and the stream reader against an in-process mock. A cell that
    # talks to a server this repo does not start (Ollama, a speech server, OpenRouter) skips without it.
    if(NOT DAS_TOOLS_DISABLED)
        das_lane_dastest_target(run_tests_dasopenai COMMENT "Testing dasOPENAI" FILES modules/dasOPENAI/tests)
    endif()
ENDIF()
