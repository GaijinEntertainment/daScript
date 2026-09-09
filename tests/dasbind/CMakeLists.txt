# The probe library tests/dasbind binds through [extern]: C functions whose argument shapes
# reach past the callee's registers. Built only on a host that builds and dlopens a shared
# library at test time - the mobile and wasm targets that also run the extern path share an
# ABI with a desktop lane that does - and placed in <dasroot>/lib, the last directory dasbind's
# library search tries, so the test names it by bare file name everywhere.
if(NOT CMAKE_SIZEOF_VOID_P EQUAL 8 OR EMSCRIPTEN OR ANDROID OR IOS)
    return()
endif()

add_library(dasbind_probe SHARED probe/dasbind_probe.cpp)
set(_dasbind_probe_dir "${PROJECT_SOURCE_DIR}/lib")
set_target_properties(dasbind_probe PROPERTIES
    PREFIX ""
    LIBRARY_OUTPUT_DIRECTORY "${_dasbind_probe_dir}"
    RUNTIME_OUTPUT_DIRECTORY "${_dasbind_probe_dir}"
    LIBRARY_OUTPUT_DIRECTORY_DEBUG "${_dasbind_probe_dir}"
    LIBRARY_OUTPUT_DIRECTORY_MINSIZEREL "${_dasbind_probe_dir}"
    LIBRARY_OUTPUT_DIRECTORY_RELEASE "${_dasbind_probe_dir}"
    LIBRARY_OUTPUT_DIRECTORY_RELWITHDEBINFO "${_dasbind_probe_dir}"
    RUNTIME_OUTPUT_DIRECTORY_DEBUG "${_dasbind_probe_dir}"
    RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL "${_dasbind_probe_dir}"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${_dasbind_probe_dir}"
    RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${_dasbind_probe_dir}"
)
