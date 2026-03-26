# Version Update Skill

How to update the daslang version number across the codebase.

## Canonical version sources (MUST update)

| File | What to change | Example |
|---|---|---|
| `include/daScript/misc/platform.h` | `DAS_VERSION_MAJOR`, `DAS_VERSION_MINOR`, `DAS_VERSION_PATCH` macros | `#define DAS_VERSION_PATCH 1` |
| `CMakeLists.txt` (line 2) | `project (DAS VERSION X.Y.Z)` | `project (DAS VERSION 0.6.1)` |
| `doc/source/conf.py` | `version` (short X.Y) and `release` (full X.Y.Z) | `version = u'0.6'`, `release = u'0.6.1'` |

## Documentation (MUST update)

| File | What to change |
|---|---|
| `doc/source/reference/index.rst` | Title: `Daslang X.Y Reference Manual` |
| `doc/source/stdlib/index.rst` | Title: `Daslang Standard Library X.Y` |
| `doc/source/stdlib/generated/builtin.rst` | Example output of `get_das_version()` |
| `install/README.md` | `- **Version:** X.Y.Z` line |

## Derived (auto-computed, no manual change needed)

These are computed from the macros in `platform.h` — no manual update required:

- `DAS_VERSION` composite macro: `MAJOR*10000 + MINOR*100 + PATCH`
- `DAS_BUILD_ID`: `DAS_VERSION * 100 + sizeof(void*) [+ 10 if assertions]`
- `get_das_version()` runtime function in `src/builtin/module_builtin_runtime.cpp` — formats from the macros
- CMake `PROJECT_VERSION` — derived from the `project()` call

## Short version (X.Y) vs full version (X.Y.Z)

- `conf.py` `version` and RST titles use **short** form (e.g. `0.6`) — only update on minor/major bumps
- Everything else uses **full** form (e.g. `0.6.1`)

## Checklist

1. Edit `platform.h` macros
2. Edit `CMakeLists.txt` project version
3. Edit `doc/source/conf.py` release (and version if minor/major changed)
4. Edit `doc/source/reference/index.rst` title (if minor/major changed)
5. Edit `doc/source/stdlib/index.rst` title (if minor/major changed)
6. Edit `doc/source/stdlib/generated/builtin.rst` example version string
7. Edit `install/README.md` version line
8. Build and verify `get_das_version()` returns the new version
