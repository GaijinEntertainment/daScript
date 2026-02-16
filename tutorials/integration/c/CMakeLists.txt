#######################################################
# C Integration Tutorials — one executable per tutorial
#######################################################

# Helper macro: create a C integration tutorial target.
# C source is compiled as C but linked with C++ (daScript runtime is C++).
MACRO(DAS_C_INTEGRATION_TUTORIAL target_name)
    add_executable(${target_name} ${ARGN})
    TARGET_COMPILE_DEFINITIONS(${target_name} PRIVATE DAS_MOD_EXPORTS)
    TARGET_LINK_LIBRARIES(${target_name} libDaScript Threads::Threads)
    ADD_DEPENDENCIES(${target_name} libDaScript)
    SETUP_CPP11(${target_name})
    set_target_properties(${target_name} PROPERTIES
        LINKER_LANGUAGE CXX
        CXX_STANDARD 17
    )
ENDMACRO()

#################################
# Tutorial 1 — Hello World
#################################
DAS_C_INTEGRATION_TUTORIAL(integration_c_01
    ${CMAKE_CURRENT_SOURCE_DIR}/tutorials/integration/c/01_hello_world.c
)
