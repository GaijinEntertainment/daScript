#######################################################
# C Integration Tutorials — one executable per tutorial
#######################################################

# Helper macro: create a C integration tutorial target.
# C source is compiled as C but linked with C++ (daScript runtime is C++).
MACRO(DAS_C_INTEGRATION_TUTORIAL target_name)
    add_executable(${target_name} ${ARGN})
    TARGET_COMPILE_DEFINITIONS(${target_name} PRIVATE DAS_MOD_EXPORTS)
    TARGET_LINK_LIBRARIES(${target_name} libDaScriptDyn Threads::Threads)
    ADD_DEPENDENCIES(${target_name} libDaScriptDyn)
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
    ${CMAKE_CURRENT_SOURCE_DIR}/integration/c/01_hello_world.c
)

#################################
# Tutorial 2 — Calling daScript Functions
#################################
DAS_C_INTEGRATION_TUTORIAL(integration_c_02
    ${CMAKE_CURRENT_SOURCE_DIR}/integration/c/02_calling_functions.c
)

#################################
# Tutorial 3 — Binding C Types
#################################
DAS_C_INTEGRATION_TUTORIAL(integration_c_03
    ${CMAKE_CURRENT_SOURCE_DIR}/integration/c/03_binding_types.c
)

#################################
# Tutorial 4 — Callbacks and Closures
#################################
DAS_C_INTEGRATION_TUTORIAL(integration_c_04
    ${CMAKE_CURRENT_SOURCE_DIR}/integration/c/04_callbacks.c
)

#################################
# Tutorial 5 — Unaligned ABI & Advanced
#################################
DAS_C_INTEGRATION_TUTORIAL(integration_c_05
    ${CMAKE_CURRENT_SOURCE_DIR}/integration/c/05_unaligned_advanced.c
)

#################################
# Tutorial 6 — Sandbox
#################################
DAS_C_INTEGRATION_TUTORIAL(integration_c_06
    ${CMAKE_CURRENT_SOURCE_DIR}/integration/c/06_sandbox.c
)

#################################
# Tutorial 7 — Context Variables
#################################
DAS_C_INTEGRATION_TUTORIAL(integration_c_07
    ${CMAKE_CURRENT_SOURCE_DIR}/integration/c/07_context_variables.c
)

#################################
# Tutorial 8 — Serialization
#################################
DAS_C_INTEGRATION_TUTORIAL(integration_c_08
    ${CMAKE_CURRENT_SOURCE_DIR}/integration/c/08_serialization.c
)

#################################
# Tutorial 9 — AOT
#################################
# AOT requires a two-stage build:
#   1. Generate C++ from the .das script using daslang's AOT tool
#   2. Compile the generated .cpp into the tutorial executable
SET(INTEGRATION_C_09_AOT_GENERATED_SRC)
add_custom_target(integration_c_09_dasAotStub)
DAS_AOT("integration/cpp/13_aot.das"
        INTEGRATION_C_09_AOT_GENERATED_SRC integration_c_09_dasAotStub daslang)

add_executable(integration_c_09
    ${CMAKE_CURRENT_SOURCE_DIR}/integration/c/09_aot.c
    ${INTEGRATION_C_09_AOT_GENERATED_SRC}
)
TARGET_COMPILE_DEFINITIONS(integration_c_09 PRIVATE DAS_MOD_EXPORTS)
TARGET_LINK_LIBRARIES(integration_c_09 libDaScriptDyn Threads::Threads)
ADD_DEPENDENCIES(integration_c_09 libDaScriptDyn integration_c_09_dasAotStub)
SETUP_CPP11(integration_c_09)
set_target_properties(integration_c_09 PROPERTIES
    LINKER_LANGUAGE CXX
    CXX_STANDARD 17
)

#################################
# Tutorial 10 — Threading
#################################
DAS_C_INTEGRATION_TUTORIAL(integration_c_10
    ${CMAKE_CURRENT_SOURCE_DIR}/integration/c/10_threading.c
)

#################################
# Tutorial 11 — Type Introspection
#################################
DAS_C_INTEGRATION_TUTORIAL(integration_c_11
    ${CMAKE_CURRENT_SOURCE_DIR}/integration/c/11_type_introspection.c
)

#################################
# Tutorial 12 — Mock ECS
#################################
DAS_C_INTEGRATION_TUTORIAL(integration_c_12
    ${CMAKE_CURRENT_SOURCE_DIR}/integration/c/12_ecs.c
)

#################################
# Tutorial 13 — Shared Modules
#################################
DAS_C_INTEGRATION_TUTORIAL(integration_c_13
    ${CMAKE_CURRENT_SOURCE_DIR}/integration/c/13_shared_module.c
)

#################################
# Tutorial 14 — Passing Arrays
#################################
DAS_C_INTEGRATION_TUTORIAL(integration_c_14
    ${CMAKE_CURRENT_SOURCE_DIR}/integration/c/14_passing_arrays.c
)

#################################
# Tutorial 15 - Static StbImage Module
#################################
# This example deliberately links the static compiler and module library so it
# can demonstrate the split initialization sequence and C registration ABI.
if(TARGET libDasModuleStbImage)
    add_executable(integration_c_15
        ${CMAKE_CURRENT_SOURCE_DIR}/integration/c/15_static_stbimage.c
    )
    target_link_libraries(integration_c_15 PRIVATE
        libDasModuleStbImage libDaScript ${SRC_LIBRARIES})
    target_include_directories(integration_c_15 PRIVATE ${NEED_MODULES_PATH})
    SETUP_CPP11(integration_c_15)
    set_target_properties(integration_c_15 PROPERTIES
        LINKER_LANGUAGE CXX
        CXX_STANDARD 17
    )
    if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
        target_compile_options(integration_c_15 PRIVATE -Werror=strict-prototypes)
    endif()
endif()

# NOTE: the install rules for these sources deliberately live in
# tutorials/CMakeLists.txt, NOT here. This file is include()d only when
# DAS_TUTORIAL_DISABLED is off, so keeping them here made "don't build the
# tutorial executables" silently also mean "don't ship the tutorial sources".
