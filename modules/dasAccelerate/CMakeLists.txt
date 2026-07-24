# das_accelerate — Apple Accelerate BLAS binding (AMX on M1-M3, SME on M4+). C++ shim only;
# all dispatch/scratch logic lives das-side in dasLLAMA's dasllama_math_accelerate driver.
IF ((NOT DAS_ACCELERATE_INCLUDED) AND ((NOT ${DAS_ACCELERATE_DISABLED}) OR (NOT DEFINED DAS_ACCELERATE_DISABLED)) AND APPLE)
    SET(DAS_ACCELERATE_INCLUDED TRUE)
    MESSAGE(STATUS "dasAccelerate module included.")

    SET(DAS_ACCELERATE_DIR ${PROJECT_SOURCE_DIR}/modules/dasAccelerate)

    SET(DAS_ACCELERATE_MODULE_SRC
        ${DAS_ACCELERATE_DIR}/src/dasAccelerate.cpp
    )

    ADD_MODULE_CPP(DasAccelerate)
    ADD_MODULE_LIB(libDasModuleAccelerate dasModuleAccelerate ${DAS_ACCELERATE_MODULE_SRC})
    MACRO(SETUP_ACCELERATE_LIB lib)
        TARGET_LINK_LIBRARIES(${lib} PUBLIC
            "-framework Accelerate"
        )
        SETUP_CPP11(${lib})
    ENDMACRO()
    SETUP_ACCELERATE_LIB(libDasModuleAccelerate)
    SETUP_ACCELERATE_LIB(dasModuleAccelerate)
ENDIF()
