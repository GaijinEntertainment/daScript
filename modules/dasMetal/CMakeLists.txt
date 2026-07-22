SET(DAS_METAL_DETECTED TRUE)

# The metal/ daslang files (MSL emitter + [metal_kernel] + boost) are pure das and register on
# ALL platforms — the emitter/text tests (tests/msl) run everywhere. Only the Obj-C++ shim over
# Metal.framework is Apple-gated below.
IF (NOT DAS_METAL_DAS_INCLUDED)
    SET(DAS_METAL_DAS_INCLUDED TRUE)

    ADD_MODULE_DAS(metal metal msl_types)
    ADD_MODULE_DAS(metal metal metal_builtins)
    ADD_MODULE_DAS(metal metal msl_emit)
    ADD_MODULE_DAS(metal metal msl_shader)
    ADD_MODULE_DAS(metal metal das_metal_boost)

    # AOT-able sources (for test_aot / any consumer); empty when disabled.
    SET(DASMETAL_AOT_FILES
        modules/dasMetal/metal/metal_builtins.das
)
    # regen inputs for AOT hash freshness (the module's whole .das graph)
    FILE(GLOB DASMETAL_AOT_DEPENDS CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/modules/dasMetal/metal/*.das")
    SET(DASMETAL_AOT_DEPENDS "${DASMETAL_AOT_DEPENDS}")

    install(DIRECTORY ${PROJECT_SOURCE_DIR}/modules/dasMetal/metal
        DESTINATION ${DAS_INSTALL_MODULESDIR}/dasMetal
        FILES_MATCHING
        PATTERN "*.das"
        PATTERN "_*" EXCLUDE
    )
ENDIF()

# C++ binding is Apple-only (system Metal.framework, zero third-party code).
IF ((NOT DAS_METAL_INCLUDED) AND ((NOT ${DAS_METAL_DISABLED}) OR (NOT DEFINED DAS_METAL_DISABLED)) AND APPLE)
    SET(DAS_METAL_INCLUDED TRUE)
    MESSAGE(STATUS "dasMetal module included.")

    SET(DAS_METAL_DIR ${PROJECT_SOURCE_DIR}/modules/dasMetal)

    SET(DAS_METAL_MODULE_SRC
        ${DAS_METAL_DIR}/src/dasMetal.h
        ${DAS_METAL_DIR}/src/dasMetal.mm
    )
    SET_SOURCE_FILES_PROPERTIES(${DAS_METAL_DIR}/src/dasMetal.mm PROPERTIES COMPILE_OPTIONS "-fobjc-arc")

    ADD_MODULE_CPP(DasMetal)
    ADD_MODULE_LIB(libDasModuleMetal dasModuleMetal ${DAS_METAL_MODULE_SRC})
    MACRO(SETUP_METAL_LIB lib)
        TARGET_LINK_LIBRARIES(${lib} PUBLIC
            "-framework Metal"
            "-framework Foundation"
        )
        # dasClipboard enables OBJCXX before this module is included. Once CMake
        # recognizes .mm as OBJCXX, CMAKE_CXX_FLAGS_* no longer supplies the
        # runtime's optimized-build -fno-rtti ABI flag to dasMetal.mm.
        TARGET_COMPILE_OPTIONS(${lib} PRIVATE
            "$<$<AND:$<COMPILE_LANGUAGE:OBJCXX>,$<OR:$<CONFIG:Release>,$<CONFIG:MinSizeRel>,$<CONFIG:RelWithDebInfo>>>:-fno-rtti>")
        SETUP_CPP11(${lib})
    ENDMACRO()
    SETUP_METAL_LIB(libDasModuleMetal)
    SETUP_METAL_LIB(dasModuleMetal)
ENDIF()
