if(NOT DAS_MESHOPTIMIZER_INCLUDED AND NOT DAS_MESHOPTIMIZER_DISABLED)
    set(DAS_MESHOPTIMIZER_INCLUDED TRUE)
    set(DAS_MESHOPTIMIZER_DIR ${PROJECT_SOURCE_DIR}/modules/dasMeshOptimizer)
    set(MESHOPTIMIZER_SOURCE_DIR "${CMAKE_CURRENT_BINARY_DIR}/meshoptimizer/src/meshoptimizer")
    set(MESHOPTIMIZER_SOURCES "${MESHOPTIMIZER_SOURCE_DIR}/src/meshoptimizer.h")
    foreach(source IN ITEMS
        allocator
        clusterizer
        indexanalyzer
        indexcodec
        indexgenerator
        meshletcodec
        meshletutils
        opacitymap
        overdrawoptimizer
        partition
        quantization
        rasterizer
        simplifier
        spatialorder
        stripifier
        tangentspace
        vcacheoptimizer
        vertexcodec
        vertexfilter
        vfetchoptimizer
)
        list(APPEND MESHOPTIMIZER_SOURCES "${MESHOPTIMIZER_SOURCE_DIR}/src/${source}.cpp")
    endforeach()

    include(ExternalProject)
    ExternalProject_Add(meshoptimizer_external
        URL https://github.com/zeux/meshoptimizer/archive/9d9890c73011d75920af614485296d1e03e95448.tar.gz
        URL_HASH SHA256=f002af938e3ba99a6ae50c0036dbbb17f3e433ce8b071d8597590d2d5649bda2
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
        PREFIX "${CMAKE_CURRENT_BINARY_DIR}/meshoptimizer"
        SOURCE_DIR "${MESHOPTIMIZER_SOURCE_DIR}"
        CONFIGURE_COMMAND ""
        BUILD_COMMAND ""
        INSTALL_COMMAND ""
        BUILD_BYPRODUCTS ${MESHOPTIMIZER_SOURCES}
    )
    set_source_files_properties(${MESHOPTIMIZER_SOURCES} PROPERTIES GENERATED TRUE)

    ADD_MODULE_CPP(MeshOptimizer)
    ADD_MODULE_PUB_LIB(libDasModuleMeshOptimizer dasModuleMeshOptimizer
        ${DAS_MESHOPTIMIZER_DIR}/src/dasMeshOptimizer.cpp
        ${DAS_MESHOPTIMIZER_DIR}/src/dasMeshOptimizer.h ${MESHOPTIMIZER_SOURCES})
    foreach(target IN ITEMS libDasModuleMeshOptimizer dasModuleMeshOptimizer)
        target_include_directories(${target} SYSTEM PRIVATE "${MESHOPTIMIZER_SOURCE_DIR}/src")
        add_dependencies(${target} meshoptimizer_external)
        SETUP_CPP11(${target})
    endforeach()
    install(FILES ${DAS_MESHOPTIMIZER_DIR}/MESHOPTIMIZER.LICENSE DESTINATION ${DAS_INSTALL_DOCDIR})
endif()
