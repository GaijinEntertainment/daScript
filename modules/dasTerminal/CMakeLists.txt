IF ((NOT DAS_TERMINAL_INCLUDED) AND (NOT DAS_TERMINAL_DISABLED))
    SET(DAS_TERMINAL_INCLUDED TRUE)
    MESSAGE(STATUS "dasTerminal module included.")

    SET(DAS_TERMINAL_DIR ${CMAKE_CURRENT_LIST_DIR})
    SET(DAS_TERMINAL_MODULE_SRC
        ${DAS_TERMINAL_DIR}/src/pty.h
        ${DAS_TERMINAL_DIR}/src/pty.cpp
        ${DAS_TERMINAL_DIR}/src/aot_builtin_terminal.h
        ${DAS_TERMINAL_DIR}/src/dasTerminal.cpp)

    ADD_MODULE_CPP(Terminal)
    ADD_MODULE_DAS(terminal daslib terminal)
    ADD_EXAMPLE_RUN(modules/dasTerminal/tests/snapshot_smoke.das)
    ADD_EXAMPLE_RUN(modules/dasTerminal/tests/terminal_semantics.das)
    ADD_EXAMPLE_RUN(modules/dasTerminal/tests/app_compat.das)
    ADD_MODULE_LIB(libDasModuleTerminal dasModuleTerminal ${DAS_TERMINAL_MODULE_SRC})

    FOREACH(target libDasModuleTerminal dasModuleTerminal)
        TARGET_INCLUDE_DIRECTORIES(${target} PRIVATE ${DAS_TERMINAL_DIR}/src)
        SETUP_CPP11(${target})
    ENDFOREACH()

    INSTALL(DIRECTORY ${DAS_TERMINAL_DIR}/daslib
        DESTINATION ${DAS_INSTALL_MODULESDIR}/dasTerminal
        FILES_MATCHING PATTERN "*.das")
    INSTALL(FILES
        ${DAS_TERMINAL_DIR}/src/pty.h
        ${DAS_TERMINAL_DIR}/src/aot_builtin_terminal.h
        DESTINATION ${DAS_INSTALL_MODULESDIR}/dasTerminal/src)
ENDIF()
