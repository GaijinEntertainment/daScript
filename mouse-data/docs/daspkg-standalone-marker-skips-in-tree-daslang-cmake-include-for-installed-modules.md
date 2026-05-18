---
slug: daspkg-standalone-marker-skips-in-tree-daslang-cmake-include-for-installed-modules
title: daslang in-tree build fails because a daspkg-installed module's CMakeLists.txt clobbers PROJECT_VERSION — what's the opt-out?
created: 2026-05-18
last_verified: 2026-05-18
links: []
---

When a `daspkg install <pkg> --global` lands a module under `modules/<pkg>/` and the daslang in-tree build then errors out (typically `No VERSION specified for WRITE_BASIC_CONFIG_VERSION_FILE()` from `CMakeLists.txt:1476` writing `${PROJECT_VERSION}`), the cause is daslang's module sweep INCLUDEing the installed module's CMakeLists.txt and that CMakeLists.txt calling its own `project()` without a VERSION argument — clobbering the parent's `project(DAS VERSION 0.6.2)` PROJECT_VERSION to empty.

Look at daslang `CMakeLists.txt` lines 476-481:

```cmake
FOREACH(_module ${_modules})
    # Skip daspkg-installed modules that build themselves (standalone CMake)
    IF(NOT EXISTS "${PROJECT_SOURCE_DIR}/modules/${_module}/.daspkg_standalone")
        INCLUDE(modules/${_module}/CMakeLists.txt OPTIONAL)
    ENDIF()
ENDFOREACH()
```

**Fix**: drop a `.daspkg_standalone` marker file at `modules/<pkg>/.daspkg_standalone` (any content; presence is the gate). The in-tree build then skips the include. The package builds standalone via its own CMake invocation (`cmake -B _build -S . -DDASLANG_DIR=<daslang-root>`).

This is meant for packages like dasImgui that have their own full CMakeLists with `project(<name> CXX C)`, ImGui sources, etc. — they're not designed to coexist in daslang's project scope.

**Pitfall**: if you'd installed the package into `daslang/modules/<name>/` as a real copy (NOT a junction/symlink) the marker file lives inside that copy. dasImgui's intended dev model has NO `modules/dasImgui` under daslang at all (see `dasimgui-dev-location-d-daspkg-not-modules` if it exists, or memory `feedback_dasimgui_dev_location`).

Verified 2026-05-18 against daslang ced9f5175 + dasImgui 14d85b0.

## Questions
- daslang in-tree build fails because a daspkg-installed module's CMakeLists.txt clobbers PROJECT_VERSION — what's the opt-out?
