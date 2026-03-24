# Build daslang utilities as standalone executables using -exe mode.
# Each utility with a main.das is compiled via daslang -exe and the
# resulting binary is placed into ${PROJECT_SOURCE_DIR}/bin/.

set(DAS_UTILS
    aot
    dascov
    daspkg
    mcp
)

if(NOT DAS_SQLITE_DISABLED)
    # list(APPEND DAS_UTILS benchctl)
endif()

# Collect daslib and LLVM daslib sources as dependencies.
file(GLOB DASLIB_SOURCES ${PROJECT_SOURCE_DIR}/daslib/*.das)
file(GLOB DASLLVM_DASLIB_SOURCES ${PROJECT_SOURCE_DIR}/modules/dasLLVM/daslib/*.das)

# daslang -exe appends `.exe` to the output path
foreach(util ${DAS_UTILS})
    set(UTIL_MAIN ${PROJECT_SOURCE_DIR}/utils/${util}/main.das)
    set(UTIL_OUT  ${PROJECT_SOURCE_DIR}/bin/${util}.exe)

    add_custom_command(
        OUTPUT  ${UTIL_OUT}
        COMMAND daslang -exe -output ${PROJECT_SOURCE_DIR}/bin/${util} ${UTIL_MAIN}
        DEPENDS ${UTIL_MAIN} daslang ${DASLIB_SOURCES} ${DASLLVM_DASLIB_SOURCES}
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
        COMMENT "Building ${util} executable (daslang -exe)"
    )

    add_custom_target(${util}_exe DEPENDS ${UTIL_OUT})
endforeach()

add_custom_target(all_utils_exe)
foreach(util ${DAS_UTILS})
    add_dependencies(all_utils_exe ${util}_exe)
endforeach()
