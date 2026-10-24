SET(DAS_CUDA_DETECTED TRUE)

# Pure daslang: the driver API and NVRTC bind as dasbind late externs over the libraries the
# driver and the toolkit install, so there is no SDK at build time. The one C++ source is the
# `cuda` witness module (src/dasCUDA.cpp): compiled in exactly when this package is, so
# `require ?cuda <target>` follows the configure - the das files below are on disk in every
# checkout and would otherwise satisfy a path guard in a build without CUDA. The package is
# 64-bit only (no driver, no [extern] on a 32-bit host) and -DDAS_CUDA_DISABLED=ON drops it;
# the descriptor registers the files on 64-bit hosts only, and a box without the driver
# answers cuda_available() false.
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
