# dasOPENAI depends on dasHV (dashv/dashv_boost); only register it when dasHV is enabled,
# otherwise consumers hit confusing `require dashv/...` failures.
IF ((NOT DAS_OPENAI_INCLUDED) AND ((NOT ${DAS_HV_DISABLED}) OR (NOT DEFINED DAS_HV_DISABLED)))
    SET(DAS_OPENAI_INCLUDED TRUE)
    MESSAGE(STATUS "dasOPENAI module included.")

    # Pure-das module (HTTP transport via dasHV, JSON via daslib/json_boost).
    # One module per section; openai_common holds the shared client + error + transport.
    # require openai/openai_<section> -> modules/dasOPENAI/openai/openai_<section>.das
    ADD_MODULE_DAS(openai openai openai_common)
    ADD_MODULE_DAS(openai openai openai_chat)
    ADD_MODULE_DAS(openai openai openai_embeddings)
    ADD_MODULE_DAS(openai openai openai_models)
    ADD_MODULE_DAS(openai openai openai_completions)
    ADD_MODULE_DAS(openai openai openai_audio)
    ADD_MODULE_DAS(openai openai openai_moderations)
    ADD_MODULE_DAS(openai openai openai_images)
    ADD_MODULE_DAS(openai openai openai_vision)

    install(DIRECTORY ${PROJECT_SOURCE_DIR}/modules/dasOPENAI/openai
        DESTINATION ${DAS_INSTALL_MODULESDIR}/dasOPENAI
        FILES_MATCHING
        PATTERN "*.das"
    )
    install(FILES ${PROJECT_SOURCE_DIR}/modules/dasOPENAI/.das_module
        DESTINATION ${DAS_INSTALL_MODULESDIR}/dasOPENAI
    )
ENDIF()
