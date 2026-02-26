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
4. Spot-check that no repo-internal paths leaked into install files
