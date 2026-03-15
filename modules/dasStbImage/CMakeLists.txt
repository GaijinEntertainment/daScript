
IF ((NOT DAS_STBIMAGE_INCLUDED) AND (NOT ${DAS_STBIMAGE_DISABLED}))
    SET(DAS_STBIMAGE_INCLUDED TRUE)
    MESSAGE(STATUS "dasStbImage module included.")

	SET(DAS_STBIMAGE_DIR ${PROJECT_SOURCE_DIR}/modules/dasStbImage)

    LIST(APPEND CMAKE_MODULE_PATH ${DAS_STBIMAGE_DIR})

	# libDasModuleStbImage
	SET(DAS_STBIMAGE_MODULE_SRC
		${DAS_STBIMAGE_DIR}/src/dasStbImage.h
		${DAS_STBIMAGE_DIR}/src/dasStbImage.cpp
		${DAS_STBIMAGE_DIR}/src/aot_builtin_raster.h
		${DAS_STBIMAGE_DIR}/src/module_builtin_raster.cpp
	)

	ADD_MODULE_CPP(StbImage)
	ADD_MODULE_CPP(Raster)
	ADD_MODULE_DAS(stbimage stbimage stbimage_boost)
	# Disable UBSan for dasStbImage.cpp — stb_image_resize2.h uses intentional negative
	# pointer arithmetic in sRGB table lookups (fp32_to_srgb8_tab4 - 912) that is safe
	# but flagged as UB by UndefinedBehaviorSanitizer.
	IF(DAS_USE_SANITIZER STREQUAL "undefined" OR DAS_USE_SANITIZER STREQUAL "ubsan")
		SET_SOURCE_FILES_PROPERTIES(${DAS_STBIMAGE_DIR}/src/dasStbImage.cpp
			PROPERTIES COMPILE_FLAGS "-fno-sanitize=undefined")
	ENDIF()

	ADD_MODULE_PUB_LIB(libDasModuleStbImage dasModuleStbImage ${DAS_STBIMAGE_MODULE_SRC} ${DAS_STBIMAGE_MODULE_PLATFORM_SRC})
	TARGET_LINK_LIBRARIES(libDasModuleStbImage PUBLIC ${STBIMAGE_LIBRARIES})
    TARGET_COMPILE_OPTIONS(libDasModuleStbImage PRIVATE -fvisibility=default)
	TARGET_LINK_LIBRARIES(libDasModuleStbImage ${STBIMAGE_LIBRARIES})
	# ADD_DEPENDENCIES(libDasModuleStbImage)
	TARGET_INCLUDE_DIRECTORIES(libDasModuleStbImage PUBLIC ${STBIMAGE_INCLUDE_DIR})

	SETUP_CPP11(libDasModuleStbImage)

    install(DIRECTORY ${PROJECT_SOURCE_DIR}/modules/dasStbImage/stbimage
        DESTINATION ${DAS_INSTALL_MODULESDIR}/dasStbImage
        FILES_MATCHING
        PATTERN "*.das"
        PATTERN "_aot_generated" EXCLUDE
    )

ENDIF()
