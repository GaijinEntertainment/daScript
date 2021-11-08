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
    ADD_MODULE_DAS(glsl glsl glsl_opengl)

ENDIF()
