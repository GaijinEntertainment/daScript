IF(NOT DAS_OPENGL_INCLUDED)
    SET(DAS_OPENGL_INCLUDED TRUE)
    MESSAGE(STATUS "dasOpenGL module included.")

    ADD_MODULE_DAS(opengl opengl opengl)
    ADD_MODULE_DAS(opengl opengl opengl_boost_internal)
    ADD_MODULE_DAS(opengl opengl opengl_boost)
    ADD_MODULE_DAS(opengl opengl opengl_state)
    ADD_MODULE_DAS(opengl opengl opengl_ttf)
    ADD_MODULE_DAS(opengl opengl opengl_gen)
    ADD_MODULE_DAS(opengl opengl opengl_cache)
    ADD_MODULE_DAS(live opengl opengl_live)
    ADD_MODULE_DAS(glsl glsl glsl_opengl)

    install(DIRECTORY ${PROJECT_SOURCE_DIR}/modules/dasOpenGL/opengl
        DESTINATION ${DAS_INSTALL_MODULESDIR}/dasOpenGL
        FILES_MATCHING
        PATTERN "*.das"
        PATTERN "_aot_generated" EXCLUDE
    )

    install(DIRECTORY ${PROJECT_SOURCE_DIR}/modules/dasOpenGL/glsl
        DESTINATION ${DAS_INSTALL_MODULESDIR}/dasOpenGL
        FILES_MATCHING
        PATTERN "*.das"
        PATTERN "_aot_generated" EXCLUDE
    )

ENDIF()
