
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
	SET(CIPIC_HRTF_INCLUDE_DIR ${DAS_AUDIO_DIR}/cipic-hrtf/include)
	SET(CIPIC_HRTF_SOURCE_DIR ${DAS_AUDIO_DIR}/cipic-hrtf/source)

    LIST(APPEND CMAKE_MODULE_PATH ${DAS_AUDIO_DIR})

	# libDasModuleAudio
	SET(DAS_AUDIO_MODULE_SRC
		${DAS_AUDIO_DIR}/src/dasAudio.h
		${DAS_AUDIO_DIR}/src/dasAudio.cpp
	)

	# CIPIC HRTF
	SET(CIPIC_HRTF_SRC
		${CIPIC_HRTF_SOURCE_DIR}/cipic_hrtf_lib.c
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
	ADD_MODULE_LIB(libDasModuleAudio dasModuleAudio ${DAS_AUDIO_MODULE_SRC} ${DAS_AUDIO_MODULE_PLATFORM_SRC} ${CIPIC_HRTF_SRC})
	IF(NOT DEFINED DAS_AUDIO_SAMPLE_RATE)
		SET(DAS_AUDIO_SAMPLE_RATE 48000)
	ENDIF()

	MACRO(SETUP_AUDIO lib)
		TARGET_LINK_LIBRARIES(${lib} PUBLIC ${AUDIO_LIBRARIES})
		TARGET_INCLUDE_DIRECTORIES(${lib} PRIVATE ${AUDIO_INCLUDE_DIR} ${CIPIC_HRTF_INCLUDE_DIR} ${CIPIC_HRTF_SOURCE_DIR})
		TARGET_COMPILE_DEFINITIONS(${lib} PRIVATE HRTF_SAMPLE_RATE=${DAS_AUDIO_SAMPLE_RATE})
		SETUP_CPP11(${lib})
	ENDMACRO()

	SETUP_AUDIO(libDasModuleAudio)
	SETUP_AUDIO(dasModuleAudio)

	# HRTF FFT convolution — compile minfft.c directly (avoids cross-DLL static lib issues on Windows)
	SET(DAS_MINFFT_INCLUDE ${PROJECT_SOURCE_DIR}/modules/dasMinfft/minfft)
	TARGET_INCLUDE_DIRECTORIES(libDasModuleAudio PRIVATE ${DAS_MINFFT_INCLUDE})
	TARGET_COMPILE_DEFINITIONS(libDasModuleAudio PRIVATE MINFFT_SINGLE)
	TARGET_SOURCES(libDasModuleAudio PRIVATE ${DAS_MINFFT_INCLUDE}/minfft.c)
	TARGET_INCLUDE_DIRECTORIES(dasModuleAudio PRIVATE ${DAS_MINFFT_INCLUDE})
	TARGET_COMPILE_DEFINITIONS(dasModuleAudio PRIVATE MINFFT_SINGLE)
	TARGET_SOURCES(dasModuleAudio PRIVATE ${DAS_MINFFT_INCLUDE}/minfft.c)

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
	ADD_MODULE_DAS(strudel strudel strudel_sf2)
	ADD_MODULE_DAS(strudel strudel strudel_sf2_voice)
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

    file(GLOB DAS_AUDIO_MEDIA
        ${PROJECT_SOURCE_DIR}/examples/media/audio/*.*
    )
    install(FILES ${DAS_AUDIO_MEDIA}
        DESTINATION ${DAS_INSTALL_EXAMPLESDIR}/media/audio
    )

    install(DIRECTORY ${PROJECT_SOURCE_DIR}/examples/audio/
        DESTINATION ${DAS_INSTALL_EXAMPLESDIR}/audio
        FILES_MATCHING
        PATTERN "*.das"
    )

    install(DIRECTORY ${PROJECT_SOURCE_DIR}/examples/daStrudel/
        DESTINATION ${DAS_INSTALL_EXAMPLESDIR}/daStrudel
        FILES_MATCHING
        PATTERN "*.das"
        PATTERN "*.md"
    )

    install(DIRECTORY ${PROJECT_SOURCE_DIR}/examples/media/
        DESTINATION ${DAS_INSTALL_EXAMPLESDIR}/media
    )

	install(FILES ${miniaudio_SOURCE_DIR}/LICENSE DESTINATION ${DAS_INSTALL_DOCDIR} RENAME MINIAUDIO.LICENSE)
	install(FILES ${PROJECT_SOURCE_DIR}/modules/dasAudio/LICENSE.OPENMPT DESTINATION ${DAS_INSTALL_DOCDIR} RENAME OPENMPT.LICENSE)

ENDIF()
