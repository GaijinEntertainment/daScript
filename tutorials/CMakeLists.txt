# Install language tutorial .das files
file(GLOB LANGUAGE_TUTORIAL_SOURCES
    ${PROJECT_SOURCE_DIR}/tutorials/language/*.das
)
install(FILES ${LANGUAGE_TUTORIAL_SOURCES}
    DESTINATION ${DAS_INSTALL_TUTORIALSDIR}/language
)

# Install macro tutorial .das files
file(GLOB MACRO_TUTORIAL_SOURCES
    ${PROJECT_SOURCE_DIR}/tutorials/macros/*.das
)
install(FILES ${MACRO_TUTORIAL_SOURCES}
    DESTINATION ${DAS_INSTALL_TUTORIALSDIR}/macros
)

# Install dasHV tutorial .das files
file(GLOB DASHV_TUTORIAL_SOURCES
    ${PROJECT_SOURCE_DIR}/tutorials/dasHV/*.das
)
install(FILES ${DASHV_TUTORIAL_SOURCES}
    DESTINATION ${DAS_INSTALL_TUTORIALSDIR}/dasHV
)

# Install dasPUGIXML tutorial files (.das + data files)
file(GLOB PUGIXML_TUTORIAL_SOURCES
    ${PROJECT_SOURCE_DIR}/tutorials/dasPUGIXML/*.das
    ${PROJECT_SOURCE_DIR}/tutorials/dasPUGIXML/*.xml
)
file(GLOB PUGIXML_TUTORIAL_DAS_SOURCES
    ${PROJECT_SOURCE_DIR}/tutorials/dasPUGIXML/*.das
)

install(FILES ${PUGIXML_TUTORIAL_SOURCES}
    DESTINATION ${DAS_INSTALL_TUTORIALSDIR}/dasPUGIXML
)

# Install dasStbImage tutorial .das files
file(GLOB STBIMAGE_TUTORIAL_SOURCES
    ${PROJECT_SOURCE_DIR}/tutorials/dasStbImage/*.das
)
install(FILES ${STBIMAGE_TUTORIAL_SOURCES}
    DESTINATION ${DAS_INSTALL_TUTORIALSDIR}/dasStbImage
)

# Install dasAudio tutorial .das files
file(GLOB AUDIO_TUTORIAL_SOURCES
    ${PROJECT_SOURCE_DIR}/tutorials/dasAudio/*.das
)
install(FILES ${AUDIO_TUTORIAL_SOURCES}
    DESTINATION ${DAS_INSTALL_TUTORIALSDIR}/dasAudio
)

# Install dasSQLITE tutorial .das files
file(GLOB SQL_TUTORIAL_SOURCES
    ${PROJECT_SOURCE_DIR}/tutorials/sql/*.das
)
install(FILES ${SQL_TUTORIAL_SOURCES}
    DESTINATION ${DAS_INSTALL_TUTORIALSDIR}/sql
)

# Install dasPEG tutorial .das files
file(GLOB DASPEG_TUTORIAL_SOURCES
    ${PROJECT_SOURCE_DIR}/tutorials/dasPEG/*.das
)
install(FILES ${DASPEG_TUTORIAL_SOURCES}
    DESTINATION ${DAS_INSTALL_TUTORIALSDIR}/dasPEG
)

# Install daStrudel tutorial .das files
file(GLOB DASTRUDEL_TUTORIAL_SOURCES
    ${PROJECT_SOURCE_DIR}/tutorials/daStrudel/*.das
)
install(FILES ${DASTRUDEL_TUTORIAL_SOURCES}
    DESTINATION ${DAS_INSTALL_TUTORIALSDIR}/daStrudel
)

# Install jsonrpc tutorial .das files
file(GLOB JSONRPC_TUTORIAL_SOURCES
    ${PROJECT_SOURCE_DIR}/tutorials/jsonrpc/*.das
)
install(FILES ${JSONRPC_TUTORIAL_SOURCES}
    DESTINATION ${DAS_INSTALL_TUTORIALSDIR}/jsonrpc
)

if (NOT ${DAS_TUTORIAL_DISABLED})
    include(integration/c/CMakeLists.txt)
    include(integration/cpp/CMakeLists.txt)
endif()

SET(ALL_TUTORIAL_SOURCES
    ${LANGUAGE_TUTORIAL_SOURCES}
    ${MACRO_TUTORIAL_SOURCES}
    ${DASPEG_TUTORIAL_SOURCES}
    ${DASTRUDEL_TUTORIAL_SOURCES}
    ${JSONRPC_TUTORIAL_SOURCES}
)

# Modules tutorials.
IF ((NOT ${DAS_PUGIXML_DISABLED}) OR (NOT DEFINED DAS_PUGIXML_DISABLED))
    list(APPEND ALL_TUTORIAL_SOURCES ${PUGIXML_TUTORIAL_DAS_SOURCES})
ENDIF()
IF ((NOT ${DAS_SQLITE_DISABLED}) OR (NOT DEFINED DAS_SQLITE_DISABLED))
    list(APPEND ALL_TUTORIAL_SOURCES ${SQL_TUTORIAL_SOURCES})
ENDIF()
IF ((NOT ${DAS_SQLITE_DISABLED}) OR (NOT DEFINED DAS_STBIMAGE_DISABLED))
    list(APPEND ALL_TUTORIAL_SOURCES ${STBIMAGE_TUTORIAL_SOURCES})
ENDIF()
IF ((NOT ${DAS_AUDIO_DISABLED}) OR (NOT DEFINED DAS_AUDIO_DISABLED))
    list(APPEND ALL_TUTORIAL_SOURCES ${AUDIO_TUTORIAL_SOURCES})
ENDIF()

add_custom_target(dry_run_tutorials)

foreach(tutor ${ALL_TUTORIAL_SOURCES})
    get_filename_component(input_name ${tutor} NAME)
    add_custom_target(${input_name}_dry
        COMMAND daslang -dry-run ${tutor}
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
        COMMENT "Testing tutorial ${tutor}"
    )
    add_dependencies(dry_run_tutorials ${input_name}_dry)
endforeach()
