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

# Install dasMinfft tutorial .das files
file(GLOB MINFFT_TUTORIAL_SOURCES
    ${PROJECT_SOURCE_DIR}/tutorials/dasMinfft/*.das
)
install(FILES ${MINFFT_TUTORIAL_SOURCES}
    DESTINATION ${DAS_INSTALL_TUTORIALSDIR}/dasMinfft
)

# Install dasSQLITE tutorial .das files
file(GLOB SQL_TUTORIAL_SOURCES
    ${PROJECT_SOURCE_DIR}/tutorials/sql/*.das
)
install(FILES ${SQL_TUTORIAL_SOURCES}
    DESTINATION ${DAS_INSTALL_TUTORIALSDIR}/sql
)

# the schema_from fixture DBs tutorials 39/42 read at compile time
file(GLOB SQL_TUTORIAL_DATA
    ${PROJECT_SOURCE_DIR}/tutorials/sql/data/*.db
)
install(FILES ${SQL_TUTORIAL_DATA}
    DESTINATION ${DAS_INSTALL_TUTORIALSDIR}/sql/data
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

# Install dasOPENAI tutorial .das files
file(GLOB OPENAI_TUTORIAL_SOURCES
    ${PROJECT_SOURCE_DIR}/tutorials/dasOPENAI/*.das
)
install(FILES ${OPENAI_TUTORIAL_SOURCES}
    DESTINATION ${DAS_INSTALL_TUTORIALSDIR}/dasOPENAI
)

# Install dasLLAMA tutorial .das files
file(GLOB DASLLAMA_TUTORIAL_SOURCES
    ${PROJECT_SOURCE_DIR}/tutorials/dasLLAMA/*.das
)
install(FILES ${DASLLAMA_TUTORIAL_SOURCES}
    DESTINATION ${DAS_INSTALL_TUTORIALSDIR}/dasLLAMA
)

# Install OpenGL tutorial .das files (nested one-dir-per-rung, so preserve structure)
install(DIRECTORY ${PROJECT_SOURCE_DIR}/tutorials/opengl/
    DESTINATION ${DAS_INSTALL_TUTORIALSDIR}/opengl
    FILES_MATCHING PATTERN "*.das"
)

# Install the shared assets the OpenGL rungs load at runtime (referenced as
# "tutorials/_assets/..."): brick/cat textures, the cat mesh, an HDRI, provenance.
# Exclude dev helper scripts (e.g. cat/gltf_to_obj.py) — not runtime assets.
install(DIRECTORY ${PROJECT_SOURCE_DIR}/tutorials/_assets/
    DESTINATION ${DAS_INSTALL_TUTORIALSDIR}/_assets
    PATTERN "*.py" EXCLUDE
)

# Install the C / C++ integration tutorial sources + their standalone recipes.
# These are UNCONDITIONAL on purpose: DAS_TUTORIAL_DISABLED means "don't build the
# tutorial executables", not "don't ship the tutorial sources". The rules used to
# live inside the two include()d files below, which coupled the two.
file(GLOB C_INTEGRATION_TUTORIAL_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/integration/c/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/integration/c/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/integration/c/*.das
    ${CMAKE_CURRENT_SOURCE_DIR}/integration/c/*.inc
    ${CMAKE_CURRENT_SOURCE_DIR}/integration/c/*.das_project
)
install(FILES ${C_INTEGRATION_TUTORIAL_SOURCES}
    DESTINATION ${DAS_INSTALL_TUTORIALSDIR}/integration/c
)
install(FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/integration/c/CMakeLists.standalone.cmake
    DESTINATION ${DAS_INSTALL_TUTORIALSDIR}/integration/c
    RENAME CMakeLists.txt
)

file(GLOB CPP_INTEGRATION_TUTORIAL_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/integration/cpp/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/integration/cpp/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/integration/cpp/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/integration/cpp/*.das
    ${CMAKE_CURRENT_SOURCE_DIR}/integration/cpp/*.inc
    ${CMAKE_CURRENT_SOURCE_DIR}/integration/cpp/*.das_project
)
install(FILES ${CPP_INTEGRATION_TUTORIAL_SOURCES}
    DESTINATION ${DAS_INSTALL_TUTORIALSDIR}/integration/cpp
)
install(FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/integration/cpp/CMakeLists.standalone.cmake
    DESTINATION ${DAS_INSTALL_TUTORIALSDIR}/integration/cpp
    RENAME CMakeLists.txt
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
IF ((NOT ${DAS_MINFFT_DISABLED}) OR (NOT DEFINED DAS_MINFFT_DISABLED))
    list(APPEND ALL_TUTORIAL_SOURCES ${MINFFT_TUTORIAL_SOURCES})
ENDIF()
# dasOPENAI tutorials require dasHV (HTTP transport); only dry-run them when HV is enabled.
IF ((NOT ${DAS_HV_DISABLED}) OR (NOT DEFINED DAS_HV_DISABLED))
    list(APPEND ALL_TUTORIAL_SOURCES ${OPENAI_TUTORIAL_SOURCES})
ENDIF()
# dasLLAMA tutorials compile without a model file, so the dry run always applies.
list(APPEND ALL_TUTORIAL_SOURCES ${DASLLAMA_TUTORIAL_SOURCES})

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
