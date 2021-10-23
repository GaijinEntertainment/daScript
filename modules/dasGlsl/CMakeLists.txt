IF(NOT DAS_GLSL_INCLUDED)
    SET(DAS_GLSL_INCLUDED TRUE)
    MESSAGE(STATUS "dasGlsl module included.")

	ADD_MODULE_DAS(glsl glsl glsl_common)
	ADD_MODULE_DAS(glsl glsl glsl_internal)
ENDIF()
