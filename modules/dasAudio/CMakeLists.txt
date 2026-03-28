
IF ((NOT DAS_AUDIO_INCLUDED) AND ((NOT ${DAS_AUDIO_DISABLED}) OR (NOT DEFINED DAS_AUDIO_DISABLED)))
    SET(DAS_AUDIO_INCLUDED TRUE)
    MESSAGE(STATUS "dasAudio module included.")

	INCLUDE(FetchContent)
	FetchContent_Declare(miniaudio
		URL https://github.com/mackron/miniaudio/archive/refs/tags/0.11.25.tar.gz
		DOWNLOAD_EXTRACT_TIMESTAMP TRUE
	)
	FetchContent_Populate(miniaudio)

	SET(DAS_AUDIO_DIR ${PROJECT_SOURCE_DIR}/modules/dasAudio)
	SET(AUDIO_INCLUDE_DIR ${DAS_AUDIO_DIR}/src ${miniaudio_SOURCE_DIR})
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

	IF(APPLE)
		SET(AUDIO_LIBRARIES
			"-framework CoreFoundation"
			"-framework CoreAudio"
			"-framework AudioToolbox"
		)
	ENDIF()

	ADD_MODULE_CPP(Audio)
	# ADD_MODULE_NATIVE(AUDIO_boost)
	ADD_MODULE_LIB(libDasModuleAudio dasModuleAudio ${DAS_AUDIO_MODULE_SRC} ${DAS_AUDIO_MODULE_PLATFORM_SRC} ${MIT_HRTF_SRC})
	MACRO(SETUP_AUDIO lib)
		TARGET_LINK_LIBRARIES(${lib} PUBLIC ${AUDIO_LIBRARIES})
		TARGET_INCLUDE_DIRECTORIES(${lib} PRIVATE ${AUDIO_INCLUDE_DIR} ${MIT_HRTF_INCLUDE_DIR})
		SETUP_CPP11(${lib})
	ENDMACRO()

	SETUP_AUDIO(libDasModuleAudio)
	SETUP_AUDIO(dasModuleAudio)

    #ADD_MODULE_DAS(medialib medialib dasbox_AUDIO_utils)
	ADD_MODULE_DAS(audio audio audio_boost)
	ADD_MODULE_DAS(audio audio audio_wav)
	ADD_MODULE_DAS(strudel strudel strudel)
	ADD_MODULE_DAS(strudel strudel strudel_event)
	ADD_MODULE_DAS(strudel strudel strudel_time)
	ADD_MODULE_DAS(strudel strudel strudel_pattern)
	ADD_MODULE_DAS(strudel strudel strudel_synth)
	ADD_MODULE_DAS(strudel strudel strudel_scheduler)
	ADD_MODULE_DAS(strudel strudel strudel_mini)
	ADD_MODULE_DAS(strudel strudel strudel_samples)
	ADD_MODULE_DAS(strudel strudel strudel_midi)
	ADD_MODULE_DAS(strudel strudel strudel_midi_player)
	ADD_MODULE_DAS(strudel strudel strudel_player)
	ADD_MODULE_DAS(strudel strudel strudel_live)

    install(DIRECTORY ${PROJECT_SOURCE_DIR}/modules/dasAudio/audio
        DESTINATION ${DAS_INSTALL_MODULESDIR}/dasAudio
        FILES_MATCHING
        PATTERN "*.das"
    )

    install(DIRECTORY ${PROJECT_SOURCE_DIR}/modules/dasAudio/strudel
        DESTINATION ${DAS_INSTALL_MODULESDIR}/dasAudio
        FILES_MATCHING
        PATTERN "*.das"
    )

    file(GLOB DAS_AUDIO_EXAMPLES
        ${PROJECT_SOURCE_DIR}/modules/dasAudio/examples/*.*
    )
    install(FILES ${DAS_AUDIO_EXAMPLES}
        DESTINATION ${DAS_INSTALL_EXAMPLESDIR}/audio
    )

    install(DIRECTORY ${PROJECT_SOURCE_DIR}/examples/daStrudel/
        DESTINATION ${DAS_INSTALL_EXAMPLESDIR}/daStrudel
        FILES_MATCHING
        PATTERN "*.das"
    )

    install(DIRECTORY ${PROJECT_SOURCE_DIR}/examples/media/
        DESTINATION ${DAS_INSTALL_EXAMPLESDIR}/media
    )

	install(FILES ${miniaudio_SOURCE_DIR}/LICENSE DESTINATION ${DAS_INSTALL_DOCDIR} RENAME MINIAUDIO.LICENSE)
	install(FILES ${PROJECT_SOURCE_DIR}/modules/dasAudio/LICENSE.OPENMPT DESTINATION ${DAS_INSTALL_DOCDIR} RENAME OPENMPT.LICENSE)

ENDIF()
