IF(NOT DAS_TELEGRAM_INCLUDED)
    SET(DAS_TELEGRAM_INCLUDED TRUE)
    MESSAGE(STATUS "dasTELEGRAM module included.")
    ADD_MODULE_DAS(telegram telegram tbotapi)
    ADD_MODULE_DAS(telegram telegram tbotlog)
    ADD_MODULE_DAS(telegram telegram tbot)

    install(DIRECTORY ${PROJECT_SOURCE_DIR}/modules/dasTelegram/telegram
        DESTINATION modules/dasTelegram
        FILES_MATCHING
        PATTERN "*.das"
    )

    file(GLOB DAS_OPENGL_EXAMPLES
        ${PROJECT_SOURCE_DIR}/modules/dasTelegram/examples/*.das
    )
    install(FILES ${DAS_OPENGL_EXAMPLES}
        DESTINATION examples/telegram
    )

ENDIF()
