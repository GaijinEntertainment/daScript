
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
    ADD_MODULE_LIB(libDasModuleLiveHost dasModuleLiveHost ${DAS_LIVE_HOST_MODULE_SRC})
    SETUP_CPP11(libDasModuleLiveHost)
    SETUP_CPP11(dasModuleLiveHost)

ENDIF()
