
IF ((NOT DAS_MINFFT_INCLUDED) AND ((NOT ${DAS_MINFFT_DISABLED}) OR (NOT DEFINED DAS_MINFFT_DISABLED)))
    SET(DAS_MINFFT_INCLUDED TRUE)
    MESSAGE(STATUS "dasMinfft module included.")

    SET(DAS_MINFFT_DIR ${PROJECT_SOURCE_DIR}/modules/dasMinfft)

    SET(DAS_MINFFT_MODULE_SRC
        ${DAS_MINFFT_DIR}/src/dasMinfft.cpp
        ${DAS_MINFFT_DIR}/src/dasMinfft.h
        ${DAS_MINFFT_DIR}/minfft/minfft.c
        ${DAS_MINFFT_DIR}/minfft/minfft.h
    )

    ADD_MODULE_CPP(Minfft)
    ADD_MODULE_LIB(libDasModuleMinfft dasModuleMinfft ${DAS_MINFFT_MODULE_SRC})
    MACRO(SETUP_MINFFT lib)
        TARGET_INCLUDE_DIRECTORIES(${lib} PRIVATE ${DAS_MINFFT_DIR}/minfft ${DAS_MINFFT_DIR}/src)
        TARGET_COMPILE_DEFINITIONS(${lib} PRIVATE MINFFT_SINGLE)
        SETUP_CPP11(${lib})
    ENDMACRO()

    SETUP_MINFFT(libDasModuleMinfft)
    SETUP_MINFFT(dasModuleMinfft)

    # Public include directory for consumers (e.g. dasAudio)
    SET(DAS_MINFFT_INCLUDE_DIR ${DAS_MINFFT_DIR}/minfft CACHE INTERNAL "dasMinfft include directory")

ENDIF()
