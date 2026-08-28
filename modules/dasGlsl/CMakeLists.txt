IF(NOT DAS_GLSL_INCLUDED)
    SET(DAS_GLSL_INCLUDED TRUE)
    MESSAGE(STATUS "dasGlsl module included.")

    ADD_MODULE_DAS_FROM_DESCRIPTOR(glsl glsl)
    ADD_MODULE_DAS_FROM_DESCRIPTOR(geometry glsl)

    # runtime-callable module code the AOT suite must carry stubs for (glsl_* stays out:
    # emission-side macro code, nothing calls it at runtime)
    SET(DASGLSL_AOT_FILES
        modules/dasGlsl/glsl/geom_gen.das
)

    install(DIRECTORY ${PROJECT_SOURCE_DIR}/modules/dasGlsl/glsl
        DESTINATION ${DAS_INSTALL_MODULESDIR}/dasGlsl
        FILES_MATCHING
        PATTERN "*.das"
        PATTERN "_aot_generated" EXCLUDE
    )
    install(FILES ${PROJECT_SOURCE_DIR}/modules/dasGlsl/.das_module
        DESTINATION ${DAS_INSTALL_MODULESDIR}/dasGlsl
    )
ENDIF()
