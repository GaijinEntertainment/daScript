# Maintaining Install AI Instructions

When updating or creating AI instruction files (`CLAUDE.md` + skills) for the installed SDK, follow this guide. These files ship with the install and help AI assistants write daslang code and use the SDK correctly.

## File layout

`install/CLAUDE.md` is the SDK-facing main instructions file (audience-curated counterpart to top-level `CLAUDE.md`). The skills tree is **not** mirrored under `install/skills/` — there is a single source of truth at `skills/`, and `install/skills.list` is a manifest naming which skills get copied to the SDK at install time.

| Source (repo) | Installed to | Purpose |
|---|---|---|
| `install/CLAUDE.md` | `CLAUDE.md` (root) | Main AI instructions for SDK users |
| `install/skills.list` | (not installed) | Manifest naming which `skills/*.md` to ship |
| `skills/<name>.md` | `skills/<name>.md` | Each skill listed in `install/skills.list` |
| `utils/mcp/` (whole dir) | `utils/mcp/` | MCP server for AI assistants (gated on dasHV) |
| `utils/detect-dupe/` (whole dir) | `utils/detect-dupe/` | Duplicate-function detector |

The current ship list lives in [install/skills.list](../install/skills.list) — alphabetical, `#` comments allowed.

## Adding or removing a shipped skill

1. **Adding:** put the skill at `skills/<name>.md` (or use an existing one). Add a single line `<name>.md` to `install/skills.list` in alphabetical order. Add a row to the `install/CLAUDE.md` skill table with the SDK-facing description.
2. **Removing:** delete the line from `install/skills.list` and the row from `install/CLAUDE.md`. Decide whether to keep the skill at `skills/<name>.md` (repo-only) or delete it entirely.

The CMake install rule (in root `CMakeLists.txt`) reads the manifest with `file(STRINGS ...)` and `install(FILES ...)` for each named basename. Missing files trigger `FATAL_ERROR` at configure time, so a typo in `install/skills.list` fails loudly.

## What belongs in install instructions

**Include (SDK-user-facing):**

- daslang language reference (gen2 syntax, all rules)
- Running scripts (`bin/daslang path/to/script.das`)
- AOT generation, JIT execution, project files
- Debugging tips (exit codes, crash diagnosis)
- Memory management, move semantics, smart pointers
- AST macros, structure macros, qmacro/quote (advanced but language-level)
- Error handling (`try/recover`, `panic`, `assert`)
- Standard library modules (daslib)
- C++ integration (embedding, binding types/functions/enums)
- Code formatting tool usage
- Install directory layout (bin/, lib/, daslib/, include/, examples/)
- MCP server usage (starting, configuring, available tools)

**Exclude (repo-dev-only):**

- Build system details (CMake, MSVC, generate_*.bat)
- GitHub operations, PR workflow
- Repo-internal test infrastructure (registering test directories in CMake, AOT test build wiring)
- Documentation generation (RST, Sphinx, das2rst)
- Benchmark framework, perf/style lint rule authoring
- Repo-internal source paths (`src/`, `tests/`, `doc/reflections/`)
- "Updating instructions with new knowledge" meta-notes
- CI/CD configuration

## Path conventions in shipped content

- Use **cross-platform paths** without OS-specific extensions: `bin/daslang` not `bin/daslang.exe`
- Use **relative paths from install root**: `bin/daslang`, `daslib/linq.das`, `examples/hello_world.das`
- Never reference repo-internal paths like `bin/Release/daslang.exe`, `src/builtin/...`, `tests/...`

## Keeping content in sync

Since the skills tree is single-sourced, most updates only touch one file. The exception is the two CLAUDE.md heads:

1. **Language knowledge changes** (gen2 syntax, gotchas, new keywords) — update BOTH `CLAUDE.md` AND `install/CLAUDE.md`. The two heads carry near-identical syntax content.
2. **Repo-dev-only changes** (build system, CI, test conventions) — update top-level `CLAUDE.md` only.
3. **Skill table edits** — update BOTH if the skill ships, top-level only if it doesn't.
4. **Skill content edits** — update `skills/<name>.md` only. If the skill ships, the same content reaches SDK users automatically via the manifest.

## CMake install rules

Install rules live in the root `CMakeLists.txt` near line 1471. The shape:

```cmake
install(FILES ${PROJECT_SOURCE_DIR}/install/CLAUDE.md DESTINATION ${DAS_INSTALL_DOCDIR})

# Skills via manifest
file(STRINGS ${PROJECT_SOURCE_DIR}/install/skills.list SHIPPED_SKILLS_RAW)
set(SHIPPED_SKILLS "")
foreach(LINE IN LISTS SHIPPED_SKILLS_RAW)
    string(STRIP "${LINE}" LINE)
    if(LINE AND NOT LINE MATCHES "^#")
        set(SKILL_PATH "${PROJECT_SOURCE_DIR}/skills/${LINE}")
        if(NOT EXISTS "${SKILL_PATH}")
            message(FATAL_ERROR "install/skills.list references missing file: skills/${LINE}")
        endif()
        list(APPEND SHIPPED_SKILLS "${SKILL_PATH}")
    endif()
endforeach()
install(FILES ${SHIPPED_SKILLS} DESTINATION ${DAS_INSTALL_DOCDIR}/skills)
```

`DAS_INSTALL_DOCDIR` is the install root (`.`), so `CLAUDE.md` lands at the top level and `skills/` as a subdirectory.

## Verification

After modifying install instructions, install to a scratch prefix and verify the layout. Replace `<prefix>` with whatever directory you prefer (e.g. `~/daslang-test`, `/tmp/daslang`).

1. Rebuild: `cmake --build build --config Release --target daslang`
2. Install: `cmake --install build --config Release --prefix <prefix>`
3. Verify `<prefix>/CLAUDE.md` exists and reads as the SDK-facing version (not the top-level repo one).
4. Verify `<prefix>/skills/` contains exactly the basenames listed in `install/skills.list` and no others. Cross-check: `ls <prefix>/skills/ | sort` should match `grep -v '^#' install/skills.list | grep -v '^$' | sort`.
5. Spot-check a couple of shipped skills for repo-internal-path leakage (`bin/Release`, `src/builtin/`, `tests/...`).
