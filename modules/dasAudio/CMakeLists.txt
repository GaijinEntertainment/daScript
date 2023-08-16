
IF ((NOT DAS_AUDIO_INCLUDED) AND ((NOT ${DAS_AUDIO_DISABLED}) OR (NOT DEFINED DAS_AUDIO_DISABLED)))
    SET(DAS_AUDIO_INCLUDED TRUE)
    MESSAGE(STATUS "dasAudio module included.")

	SET(DAS_AUDIO_DIR ${PROJECT_SOURCE_DIR}/modules/dasAudio)
	SET(AUDIO_INCLUDE_DIR ${DAS_AUDIO_DIR}/src ${DAS_AUDIO_DIR}/miniaudio)
	SET(MIT_HRTF_INCLUDE_DIR ${DAS_AUDIO_DIR}/mit-hrtf-lib/include)
	SET(MIT_HRTF_SOURCE_DIR ${DAS_AUDIO_DIR}/mit-hrtf-lib/source)

    LIST(APPEND CMAKE_MODULE_PATH ${DAS_AUDIO_DIR})

	# libDasModuleAudio
	SET(DAS_AUDIO_MODULE_SRC
		${DAS_AUDIO_DIR}/src/dasAudio.h
		${DAS_AUDIO_DIR}/src/dasAudio.cpp
	)

	# min-hrtf
	SET(MIT_HRTF_SRC
		${MIT_HRTF_SOURCE_DIR}/mit_hrtf_lib.c
	)

	ADD_MODULE_LIB(libDasModuleAudio)
	ADD_MODULE_CPP(Audio)
	# ADD_MODULE_NATIVE(AUDIO_boost)
	ADD_LIBRARY(libDasModuleAudio ${DAS_AUDIO_MODULE_SRC} ${DAS_AUDIO_MODULE_PLATFORM_SRC} ${MIT_HRTF_SRC})
	TARGET_LINK_LIBRARIES(libDasModuleAudio ${AUDIO_LIBRARIES})
	# ADD_DEPENDENCIES(libDasModuleAudio)
	TARGET_INCLUDE_DIRECTORIES(libDasModuleAudio PUBLIC ${AUDIO_INCLUDE_DIR} ${MIT_HRTF_INCLUDE_DIR})

	SETUP_CPP11(libDasModuleAudio)

    #ADD_MODULE_DAS(medialib medialib dasbox_AUDIO_utils)
	ADD_MODULE_DAS(audio audio audio_boost)

    install(DIRECTORY ${PROJECT_SOURCE_DIR}/modules/dasAudio/audio
        DESTINATION modules/dasAudio
        FILES_MATCHING
        PATTERN "*.das"
    )

    file(GLOB DAS_AUDIO_EXAMPLES
        ${PROJECT_SOURCE_DIR}/modules/dasAudio/examples/*.*
    )
    install(FILES ${DAS_AUDIO_EXAMPLES}
        DESTINATION examples/audio
    )

	install(FILES ${PROJECT_SOURCE_DIR}/modules/dasAudio/miniaudio/LICENSE DESTINATION . RENAME MINIAUDIO.LICENSE)
	install(FILES ${PROJECT_SOURCE_DIR}/modules/dasAudio/LICENSE.OPENMPT DESTINATION . RENAME OPENMPT.LICENSE)

ENDIF()
