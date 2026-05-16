
IF ((NOT DAS_LIVE_HOST_INCLUDED) AND ((NOT ${DAS_LIVE_HOST_DISABLED}) OR (NOT DEFINED DAS_LIVE_HOST_DISABLED)))
    SET(DAS_LIVE_HOST_INCLUDED TRUE)
    MESSAGE(STATUS "dasLiveHost module included.")

    SET(DAS_LIVE_HOST_DIR ${PROJECT_SOURCE_DIR}/modules/dasLiveHost)

    SET(DAS_LIVE_HOST_MODULE_SRC
        ${DAS_LIVE_HOST_DIR}/src/dasLiveHost.h
        ${DAS_LIVE_HOST_DIR}/src/dasLiveHost.cpp
    )
    SOURCE_GROUP_FILES("source" DAS_LIVE_HOST_MODULE_SRC)

    ADD_MODULE_CPP(LiveHost)
    ADD_MODULE_DAS(live live live_commands)
    ADD_MODULE_DAS(live live live_api_builtins)
    ADD_MODULE_DAS(live live live_api)
    ADD_MODULE_DAS(live live live_api_stdio)
    ADD_MODULE_DAS(live live live_watch)
    ADD_MODULE_DAS(live live decs_live)
    ADD_MODULE_LIB(libDasModuleLiveHost dasModuleLiveHost ${DAS_LIVE_HOST_MODULE_SRC})
    SETUP_CPP11(libDasModuleLiveHost)
    SETUP_CPP11(dasModuleLiveHost)

    install(DIRECTORY ${DAS_LIVE_HOST_DIR}/live
        DESTINATION ${DAS_INSTALL_MODULESDIR}/dasLiveHost
        FILES_MATCHING PATTERN "*.das"
        PATTERN "_aot_generated" EXCLUDE
        PATTERN "jitted_scripts" EXCLUDE
    )

ENDIF()
