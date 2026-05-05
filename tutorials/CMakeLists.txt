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

if (NOT ${DAS_TUTORIAL_DISABLED})
    include(integration/c/CMakeLists.txt)
    include(integration/cpp/CMakeLists.txt)
endif()
