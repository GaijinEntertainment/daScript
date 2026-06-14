IF(NOT DAS_SPIRV_INCLUDED)
    SET(DAS_SPIRV_INCLUDED TRUE)
    MESSAGE(STATUS "dasSpirv module included.")

    ADD_MODULE_DAS(spirv spirv spirv_grammar)
    ADD_MODULE_DAS(spirv spirv spirv_builder)
    ADD_MODULE_DAS(spirv spirv spirv_types)
    ADD_MODULE_DAS(spirv spirv spirv_dis)
    ADD_MODULE_DAS(spirv spirv spirv_emit)
    ADD_MODULE_DAS(spirv spirv spirv_builtins)
    ADD_MODULE_DAS(spirv spirv spirv_shader)

    install(DIRECTORY ${PROJECT_SOURCE_DIR}/modules/dasSpirv/spirv
        DESTINATION ${DAS_INSTALL_MODULESDIR}/dasSpirv
        FILES_MATCHING
        PATTERN "*.das"
        PATTERN "_*" EXCLUDE
    )
ENDIF()
