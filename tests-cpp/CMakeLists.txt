# tests-cpp — daslang C++ test infrastructure
#
# Layout:
#   small/test_*.cpp                 — globbed into a single doctest exe
#   big/<name>/CMakeLists.txt        — auto-discovered; one or more standalone exes per dir
#   3rdparty/doctest/                — vendored single-header doctest + cmake helper
#
# `enable_testing()` lives in the top-level CMakeLists.txt (it's a CMake hard
# requirement that it be at top scope for `ninja test` to work).

# ── doctest as a header-only INTERFACE library ──────────────────────────────
add_library(doctest INTERFACE)
target_include_directories(doctest INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty)
include(${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/doctest/doctest.cmake)

# ── Small tests: one default executable, glob in test_*.cpp ─────────────────
# CONFIGURE_DEPENDS makes Ninja/Make/MSBuild rescan the dir each build (CMake 3.12+).
# Glob `test_*.cpp` (not `*.cpp`) to skip editor temp files (`.#foo.cpp`, `foo.cpp~`).
file(GLOB SMALL_SOURCES CONFIGURE_DEPENDS small/test_*.cpp)

add_executable(tests-cpp-small doctest_main.cpp ${SMALL_SOURCES})
target_link_libraries(tests-cpp-small PRIVATE
    doctest libDaScript ${SRC_LIBRARIES} ${DAS_MODULES_LIBS})
target_include_directories(tests-cpp-small PRIVATE ${NEED_MODULES_PATH})
SETUP_CPP11(tests-cpp-small)
set_target_properties(tests-cpp-small PROPERTIES FOLDER "tests-cpp")

# Register each TEST_CASE as its own ctest entry.
# Note: this version of doctest.cmake (v2.4.11) uses POST_BUILD discovery only
# (no DISCOVERY_MODE option). For cross-compile targets that can't execute
# the host binary at build time, bump doctest to a release that supports
# `DISCOVERY_MODE PRE_TEST` and add the flag here.
doctest_discover_tests(tests-cpp-small
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    PROPERTIES LABELS "small")

# ── Aggregate run targets (defined BEFORE big-test subdirs so they can register deps) ─
# `ninja test` (or `ctest`) runs everything by default. These two run subsets.
# --build-config $<CONFIG> is required for multi-config generators (MSVC, Xcode);
# single-config generators ignore the flag harmlessly.
add_custom_target(test-small
    COMMAND ${CMAKE_CTEST_COMMAND} --build-config $<CONFIG> -L small --output-on-failure
    DEPENDS tests-cpp-small
    USES_TERMINAL)

add_custom_target(test-big
    COMMAND ${CMAKE_CTEST_COMMAND} --build-config $<CONFIG> -L big --output-on-failure
    USES_TERMINAL)

# ── Big tests: each big/<name>/CMakeLists.txt is auto-picked-up ─────────────
# Glob `big/*/CMakeLists.txt` (not directories) so adding a CMakeLists to a
# pre-existing-but-empty dir triggers a re-glob without manual `cmake` re-run.
file(GLOB BIG_CMAKELISTS CONFIGURE_DEPENDS big/*/CMakeLists.txt)
foreach(_cml ${BIG_CMAKELISTS})
    get_filename_component(_dir ${_cml} DIRECTORY)
    add_subdirectory(${_dir})
endforeach()
