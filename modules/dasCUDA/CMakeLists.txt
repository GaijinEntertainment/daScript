SET(DAS_CUDA_DETECTED TRUE)

# Pure daslang: the driver API and NVRTC bind as dasbind late externs over the libraries the
# driver and the toolkit install, so there is no C++ module and no SDK at build time. The
# descriptor registers the files on 64-bit hosts only; a box without the driver answers
# cuda_available() false.
IF (NOT DAS_CUDA_DAS_INCLUDED)
    SET(DAS_CUDA_DAS_INCLUDED TRUE)

    ADD_MODULE_DAS_FROM_DESCRIPTOR(cuda cuda)

    # the driver binding and its boost are late externs, which the AOT emitter cannot spell (both carry options no_aot)
    SET(DASCUDA_AOT_FILES)
    FILE(GLOB DASCUDA_AOT_DEPENDS CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/modules/dasCUDA/cuda/*.das")
    SET(DASCUDA_AOT_DEPENDS "${DASCUDA_AOT_DEPENDS}")

    install(FILES ${PROJECT_SOURCE_DIR}/modules/dasCUDA/.das_module
        DESTINATION ${DAS_INSTALL_MODULESDIR}/dasCUDA
    )
    install(DIRECTORY ${PROJECT_SOURCE_DIR}/modules/dasCUDA/cuda
        DESTINATION ${DAS_INSTALL_MODULESDIR}/dasCUDA
        FILES_MATCHING
        PATTERN "*.das"
        PATTERN "_*" EXCLUDE
    )
ENDIF()
