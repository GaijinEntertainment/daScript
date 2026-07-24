IF(NOT DAS_SPIRV_INCLUDED)
    SET(DAS_SPIRV_INCLUDED TRUE)
    MESSAGE(STATUS "dasSpirv module included.")

    ADD_MODULE_DAS(spirv spirv spirv_grammar)
    ADD_MODULE_DAS(spirv spirv spirv_builder)
    ADD_MODULE_DAS(spirv spirv spirv_types)
    ADD_MODULE_DAS(spirv spirv spirv_reflect)
    ADD_MODULE_DAS(spirv spirv spirv_dis)
    ADD_MODULE_DAS(spirv spirv spirv_emit)
    ADD_MODULE_DAS(spirv spirv spirv_builtins)
    ADD_MODULE_DAS(spirv spirv spirv_shader)

    # AOT-able sources (for test_aot / any consumer); empty when disabled.
    SET(DASSPIRV_AOT_FILES
        modules/dasSpirv/spirv/spirv_grammar.das
        modules/dasSpirv/spirv/spirv_builder.das
        modules/dasSpirv/spirv/spirv_types.das
        modules/dasSpirv/spirv/spirv_reflect.das
        modules/dasSpirv/spirv/spirv_dis.das
        modules/dasSpirv/spirv/spirv_builtins.das
)
    # regen inputs for AOT hash freshness (the module's whole .das graph)
    FILE(GLOB DASSPIRV_AOT_DEPENDS CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/modules/dasSpirv/spirv/*.das")
    SET(DASSPIRV_AOT_DEPENDS "${DASSPIRV_AOT_DEPENDS}")

    install(DIRECTORY ${PROJECT_SOURCE_DIR}/modules/dasSpirv/spirv
        DESTINATION ${DAS_INSTALL_MODULESDIR}/dasSpirv
        FILES_MATCHING
        PATTERN "*.das"
        PATTERN "_*" EXCLUDE
    )
ENDIF()
