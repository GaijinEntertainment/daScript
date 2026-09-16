IF(NOT DAS_GLTF_INCLUDED)
    SET(DAS_GLTF_INCLUDED TRUE)
    MESSAGE(STATUS "dasGLTF module included.")

    ADD_MODULE_DAS_FROM_DESCRIPTOR(gltf gltf)

    install(DIRECTORY ${PROJECT_SOURCE_DIR}/modules/dasGLTF/gltf
        DESTINATION ${DAS_INSTALL_MODULESDIR}/dasGLTF
        FILES_MATCHING
        PATTERN "*.das"
    )
    install(FILES ${PROJECT_SOURCE_DIR}/modules/dasGLTF/.das_module
        DESTINATION ${DAS_INSTALL_MODULESDIR}/dasGLTF
    )
    # the asset converter README.md documents - a shipped tree with no way to make
    # a .das_gltf cannot use the processed-asset rail
    install(FILES ${PROJECT_SOURCE_DIR}/modules/dasGLTF/tools/convert.das
        DESTINATION ${DAS_INSTALL_MODULESDIR}/dasGLTF/tools
    )

ENDIF()
