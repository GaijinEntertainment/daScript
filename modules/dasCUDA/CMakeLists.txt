SET(DAS_CUDA_DETECTED TRUE)

# modules/dasCUDA/ARCHITECTURE.md#cuda-witness-module
IF ((NOT DAS_CUDA_DAS_INCLUDED) AND (NOT DAS_CUDA_DISABLED) AND (CMAKE_SIZEOF_VOID_P EQUAL 8))
    SET(DAS_CUDA_DAS_INCLUDED TRUE)
    MESSAGE(STATUS "dasCUDA module included.")

    ADD_MODULE_DAS_FROM_DESCRIPTOR(cuda cuda)

    SET(DAS_CUDA_DIR ${PROJECT_SOURCE_DIR}/modules/dasCUDA)
    ADD_MODULE_CPP(CUDA)
    ADD_MODULE_LIB(libDasModuleCUDA dasModuleCUDA ${DAS_CUDA_DIR}/src/dasCUDA.cpp)
    SETUP_CPP11(libDasModuleCUDA)
    SETUP_CPP11(dasModuleCUDA)

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
