###########################################################
# C++ Integration Tutorials — one executable per tutorial
###########################################################

# Helper macro: create a C++ integration tutorial target.
MACRO(DAS_CPP_INTEGRATION_TUTORIAL target_name)
    add_executable(${target_name} ${ARGN})
    TARGET_COMPILE_DEFINITIONS(${target_name} PRIVATE DAS_MOD_EXPORTS)
    TARGET_LINK_LIBRARIES(${target_name} libDaScript Threads::Threads)
    ADD_DEPENDENCIES(${target_name} libDaScript)
    SETUP_CPP11(${target_name})
ENDMACRO()

#################################
# Tutorial 1 — Hello World
#################################
DAS_CPP_INTEGRATION_TUTORIAL(integration_cpp_01
    ${CMAKE_CURRENT_SOURCE_DIR}/tutorials/integration/cpp/01_hello_world.cpp
)

#################################
# Tutorial 2 — Calling Functions
#################################
DAS_CPP_INTEGRATION_TUTORIAL(integration_cpp_02
    ${CMAKE_CURRENT_SOURCE_DIR}/tutorials/integration/cpp/02_calling_functions.cpp
)

#################################
# Tutorial 3 — Binding Functions
#################################
DAS_CPP_INTEGRATION_TUTORIAL(integration_cpp_03
    ${CMAKE_CURRENT_SOURCE_DIR}/tutorials/integration/cpp/03_binding_functions.cpp
)

#################################
# Tutorial 4 — Binding Types
#################################
DAS_CPP_INTEGRATION_TUTORIAL(integration_cpp_04
    ${CMAKE_CURRENT_SOURCE_DIR}/tutorials/integration/cpp/04_binding_types.cpp
)

#################################
# Tutorial 5 — Binding Enums
#################################
DAS_CPP_INTEGRATION_TUTORIAL(integration_cpp_05
    ${CMAKE_CURRENT_SOURCE_DIR}/tutorials/integration/cpp/05_binding_enums.cpp
)

#################################
# Tutorial 6 — Low-Level Interop
#################################
DAS_CPP_INTEGRATION_TUTORIAL(integration_cpp_06
    ${CMAKE_CURRENT_SOURCE_DIR}/tutorials/integration/cpp/06_interop.cpp
)

#################################
# Tutorial 7 — Callbacks
#################################
DAS_CPP_INTEGRATION_TUTORIAL(integration_cpp_07
    ${CMAKE_CURRENT_SOURCE_DIR}/tutorials/integration/cpp/07_callbacks.cpp
)

#################################
# Tutorial 8 — Binding Methods
#################################
DAS_CPP_INTEGRATION_TUTORIAL(integration_cpp_08
    ${CMAKE_CURRENT_SOURCE_DIR}/tutorials/integration/cpp/08_methods.cpp
)

#################################
# Tutorial 9 — Operators & Properties
#################################
DAS_CPP_INTEGRATION_TUTORIAL(integration_cpp_09
    ${CMAKE_CURRENT_SOURCE_DIR}/tutorials/integration/cpp/09_operators_and_properties.cpp
)

# Install C++ integration tutorial source files
file(GLOB CPP_INTEGRATION_TUTORIAL_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/tutorials/integration/cpp/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/tutorials/integration/cpp/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/tutorials/integration/cpp/*.das
    ${CMAKE_CURRENT_SOURCE_DIR}/tutorials/integration/cpp/*.das.inc
)
install(FILES ${CPP_INTEGRATION_TUTORIAL_SOURCES}
    DESTINATION ${DAS_INSTALL_TUTORIALSDIR}/integration/cpp
)
