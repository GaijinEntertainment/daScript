
IF ((NOT DAS_STBTRUETYPE_INCLUDED) AND ((NOT ${DAS_STBTRUETYPE_DISABLED}) OR (NOT DEFINED DAS_STBTRUETYPE_DISABLED)))
    SET(DAS_STBTRUETYPE_INCLUDED TRUE)
    MESSAGE(STATUS "dasStbTrueType module included.")

	SET(DAS_STBTRUETYPE_DIR ${PROJECT_SOURCE_DIR}/modules/dasStbTrueType)

    LIST(APPEND CMAKE_MODULE_PATH ${DAS_STBTRUETYPE_DIR})

	# libDasModuleStbTrueType
	SET(DAS_STBTRUETYPE_MODULE_SRC
		${DAS_STBTRUETYPE_DIR}/src/dasStbTrueType.h
		${DAS_STBTRUETYPE_DIR}/src/dasStbTrueType.cpp
	)

	ADD_MODULE_LIB(libDasModuleStbTrueType)
	ADD_MODULE_CPP(StbTrueType)
	# ADD_MODULE_NATIVE(stbimage_boost)
	ADD_LIBRARY(libDasModuleStbTrueType ${DAS_STBTRUETYPE_MODULE_SRC} ${DAS_STBTRUETYPE_MODULE_PLATFORM_SRC})
	TARGET_LINK_LIBRARIES(libDasModuleStbTrueType ${STBTRUETYPE_LIBRARIES})
	# ADD_DEPENDENCIES(libDasModuleStbTrueType)
	TARGET_INCLUDE_DIRECTORIES(libDasModuleStbTrueType PUBLIC ${STBTRUETYPE_INCLUDE_DIR})
	SETUP_CPP11(libDasModuleStbTrueType)

	ADD_MODULE_DAS(ttf ttf stb_ttf_common)

    install(DIRECTORY ${PROJECT_SOURCE_DIR}/modules/dasStbTrueType/ttf
        DESTINATION modules/dasStbTrueType
        FILES_MATCHING
        PATTERN "*.das"
    )

    install(FILES ${PROJECT_SOURCE_DIR}/modules/dasBGFX/bgfx/bgfx/examples/runtime/font/droidsansmono.ttf
        DESTINATION examples/media
    )


ENDIF()
