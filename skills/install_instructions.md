# Maintaining Install AI Instructions

When updating or creating AI instruction files (`CLAUDE.md` + skills) for the installed SDK, follow this guide. These files ship with the install and help AI assistants write daslang code and use the SDK correctly.

## File layout

Source files live under `install/` in the repo; CMake installs them to the SDK root:

| Source (repo) | Installed to | Purpose |
|---|---|---|
| `install/CLAUDE.md` | `CLAUDE.md` (root) | Main AI instructions for SDK users |
| `install/skills/das_formatting.md` | `skills/das_formatting.md` | Code formatter usage |
| `install/skills/cpp_integration.md` | `skills/cpp_integration.md` | C++ embedding patterns |
| `install/skills/daslib_modules.md` | `skills/daslib_modules.md` | Standard library module reference |
| `install/skills/das_macros.md` | `skills/das_macros.md` | Macro & AST programming |
| `install/skills/daspkg.md` | `skills/daspkg.md` | Package manager usage |
| `install/skills/dynamic_modules.md` | `skills/dynamic_modules.md` | `.das_module` descriptors |
| `install/skills/daslang_live.md` | `skills/daslang_live.md` | Live-reload host |
| `install/skills/clargs_usage.md` | `skills/clargs_usage.md` | Command-line argument parsing for user tools |
| `install/skills/json.md` | `skills/json.md` | JSON read/write via `sprint_json`/`sscan_json` and `JV`/`from_JV` |
| `install/skills/xml.md` | `skills/xml.md` | XML via `dasPUGIXML`/`PUGIXML_boost` |
| `install/skills/filesystem.md` | `skills/filesystem.md` | Filename & filesystem ops via `fio` / `daslib/fio` (path helpers, `stat`, `dir_rec`, RAII `fopen`, `_result` variants) |
| `install/skills/writing_tests.md` | `skills/writing_tests.md` | dastest framework usage for SDK consumers |
| `install/skills/memory_leak_detection.md` | `skills/memory_leak_detection.md` | Heap / gc_node / smart_ptr / handle leak detection — runtime CLI flags |
| `install/skills/jobque_debugging.md` | `skills/jobque_debugging.md` | Channel/LockBox/JobStatus/Stream/Feature leak debugging with `--track-job-status` |
| `install/skills/find_dupes.md` | `skills/find_dupes.md` | Duplicate-function detection — corpus build, MCP tools (`export_corpus`, `find_duplicates`), CLI modes |
| `utils/mcp/` (whole dir) | `utils/mcp/` | MCP server for AI assistants (gated on dasHV) |
| `utils/find_dupes/` (whole dir) | `utils/find_dupes/` | Duplicate-function detector (canonicalizer, clusterer, MinHash, pattern filter) |

## What belongs in install instructions

**Include (SDK-user-facing):**

- daslang language reference (gen2 syntax, all rules)
- Running scripts (`bin/daslang path/to/script.das`)
- AOT generation (`bin/daslang -aot input.das output.cpp`)
- Debugging tips (exit codes, crash diagnosis)
- Memory management, move semantics, smart pointers
- AST macros, structure macros, qmacro/quote (advanced but language-level)
- Error handling (`try/recover`, `panic`, `assert`)
- Standard library modules (daslib)
- C++ integration (embedding, binding types/functions/enums)
- Code formatting tool usage
- Install directory layout (bin/, lib/, daslib/, include/, examples/)
- MCP server usage (starting, configuring, available tools)
- Keywords reference

**Exclude (repo-dev-only):**

- Build system details (CMake, MSVC, generate_*.bat)
- GitHub operations, PR workflow
- Test framework internals (dastest conventions, writing tests for the repo)
- Documentation generation (RST, Sphinx, das2rst)
- Benchmark framework
- Repo-internal source paths (src/, tests/, doc/reflections/)
- "Updating instructions with new knowledge" meta-notes
- CI/CD configuration

## Path conventions

- Use **cross-platform paths** without OS-specific extensions: `bin/daslang` not `bin/daslang.exe`
- Use **relative paths from install root**: `bin/daslang`, `daslib/linq.das`, `examples/hello_world.das`
- Never reference repo-internal paths like `bin/Release/daslang.exe`

## Keeping content in sync

The install CLAUDE.md derives from the repo CLAUDE.md but is NOT a copy. When updating:

1. **Language knowledge changes** (gen2 syntax, gotchas, new keywords) — update BOTH repo `CLAUDE.md` AND `install/CLAUDE.md`
2. **Repo-dev-only changes** (build system, CI, test conventions) — update repo `CLAUDE.md` only
3. **New daslib module docs** — update BOTH `skills/daslib_modules.md` AND `install/skills/daslib_modules.md`
4. **New C++ integration patterns** — update BOTH `skills/cpp_integration.md` AND `install/skills/cpp_integration.md`; omit repo-internal codebase notes from the install version
5. **Formatter changes** — update BOTH `skills/das_formatting.md` AND `install/skills/das_formatting.md`; use `bin/daslang` paths in the install version
6. **MCP tool changes** (new tools, API changes) — update BOTH repo `CLAUDE.md` MCP section AND `install/CLAUDE.md` MCP section; use `bin/daslang` paths in install version. The MCP source files (`utils/mcp/`) are installed directly by CMake (gated on `NOT DAS_HV_DISABLED`)
7. **Library / module skills** (`json.md`, `xml.md`, `daslib_modules.md`, etc.) — update BOTH repo and install versions. In the install version: drop `tests/` references (tests aren't shipped), drop repo-only doc paths (`doc/source/...`), drop CMake-flag / `.vscode/settings.json` build-config sections (install users get a pre-built binary — they cannot flip `DAS_*_DISABLED`), drop the "no `print` in tests/daslib" gotcha (a repo-development convention, not user-facing), drop `skills/tutorials.md` cross-references (not in install). Keep all `daslib/*.das`, `tutorials/`, `examples/`, `utils/mcp/`, `utils/daspkg/`, `modules/<X>/daslib/*.das` references — those all ship

## CMake install rules

Install rules are in the root `CMakeLists.txt`. The CLAUDE.md and skills are installed alongside README.md:

```cmake
# AI instructions
install(FILES ${PROJECT_SOURCE_DIR}/install/CLAUDE.md DESTINATION ${DAS_INSTALL_DOCDIR})
install(DIRECTORY ${PROJECT_SOURCE_DIR}/install/skills DESTINATION ${DAS_INSTALL_DOCDIR})
```

`DAS_INSTALL_DOCDIR` is the install root (`.`), so CLAUDE.md ends up at the top level and skills/ as a subdirectory.

## Verification

After modifying install instructions:

1. Rebuild: `cmake --build build --config Release --target daslang`
2. Install: `cmake --install build --config Release --prefix D:/daslang`
3. Verify files exist:
   - `D:/daslang/CLAUDE.md`
   - `D:/daslang/skills/das_formatting.md`
   - `D:/daslang/skills/cpp_integration.md`
   - `D:/daslang/skills/daslib_modules.md`
   - `D:/daslang/skills/das_macros.md`
   - `D:/daslang/skills/daspkg.md`
   - `D:/daslang/skills/dynamic_modules.md`
   - `D:/daslang/skills/daslang_live.md`
   - `D:/daslang/skills/clargs_usage.md`
   - `D:/daslang/skills/json.md`
   - `D:/daslang/skills/xml.md`
   - `D:/daslang/skills/filesystem.md`
   - `D:/daslang/skills/writing_tests.md`
   - `D:/daslang/skills/memory_leak_detection.md`
   - `D:/daslang/skills/jobque_debugging.md`
   - `D:/daslang/skills/find_dupes.md`
   - `D:/daslang/utils/mcp/main.das` (only if built with `DAS_HV_DISABLED=OFF`)
   - `D:/daslang/utils/mcp/tools/common.das` (only if built with `DAS_HV_DISABLED=OFF`)
   - `D:/daslang/utils/find_dupes/main.das`
   - `D:/daslang/utils/find_dupes/patterns.das`
   - `D:/daslang/utils/find_dupes/fixture/synth.das`
4. Spot-check that no repo-internal paths leaked into install files
