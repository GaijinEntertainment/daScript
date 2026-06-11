# Closing the local-vs-CI coverage gap

Follow-up to the fixed-array rework (PR #3095). Every CI failure during that
PR's babysitting was an **oracle mismatch** — a gate CI enforces that no local
step mirrored — not a wrong change. This plan closes those gaps at four levels.
Process: per-stage plan → implement → review, same as FIXED_ARRAY_REWORK.md.

## Evidence base (from the #3095 session, 2026-06-11)

| Incident | Gap |
|---|---|
| doc.yml failed twice: das2rst positional handmade-doc validation, then the `Uncategorized` grep | Only 2 of doc.yml's 6 gates were mirrored locally; CI stops at the FIRST das2rst panic, hiding the rest |
| doctest `CHECK` on TypeDecl bit-fields: MSVC tolerates, clang/gcc reject — 15 lanes red | No clang-family compile of tests-cpp before push; a syntax-only pass would catch it in seconds |
| `safe_addr(bool[4])` decay regression hid in `modules/dasOpenGL/opengl/*.das` | GLFW-gated das code is compiled by NOTHING local — only extended_checks' sequence smoke |
| extended_checks installs external dasImgui from ITS master → `no member named 'dim'` | ABI breaks vs external repos invisible until CI; resolved pre-merge only via the both-worlds `isArray()` spelling |
| (Not hit, same family) Debug lanes bypass fused interpreter permutations | Release-only local testing misses Debug-divergent paths |

## Stage 0 — language-doc currency (CLAUDE.md + skills)

- Sharpen the "Updating Instructions" rule: syntax/factual corrections are
  **fix-in-place, always** (flag in end-of-turn summary); propose-first stays
  for restructuring, removals, new skill files.
- Fixed-array sweep: purge stale `dim`/`dimExpr` guidance (das_macros.md,
  macro examples, typemacro payload references → `typeMacroExpr`); document
  the structural model where macro authors look (element in `firstType`, one
  size per node in `fixedDim`, quals on the chain head, one-peel rule,
  `make_fixed_array_type`); document the new user-facing semantics
  (`auto(TT)` whole-bind, `auto(TT)[]` peel + const inheritance, one-level
  `-[]`, `safe_addr` element-pointer decay).
- Gen2 currency review: extract every syntax claim from CLAUDE.md's gen2
  section + syntax-adjacent skills; verify each against `ds2_parser.ypp` AND a
  probe-compile with the current binary; fix stale claims in place. Probe
  corpus kept (re-runnable after future grammar changes).

## Stage 1 — process docs (shared, in-repo)

- `skills/preflight.md`: CI-lane ↔ local-mirror table for every lane in
  build.yml / extended_checks.yml / doc.yml — exact local command, or an
  honest "not mirrorable, see skills/wsl_ci_repro.md".
- `skills/make_pr.md`: step-4 trigger gains "removed fields / new enum
  values" (das2rst validates positionally); gate list gains the Uncategorized
  grep, the latex sphinx build, and a "type-system / daslib-generics change →
  sequence smoke + externals sweep" trigger.
- New `skills/abi_break_sweep.md`: externals checklist — grep patterns,
  both-worlds spellings (prefer a predicate that exists with the same meaning
  in both worlds over feature macros — the `isArray()` precedent), junction
  rebuild order, stale-`.shared_module` startup trap, daspkg-index as scope.

## Stage 2 — tools

- `utils/preflight/` (das, clargs): one command, tiered. `--fast` = format +
  lint + clang syntax-only pass on changed C++. `--full` adds interp/AOT/JIT
  suites, tests-cpp, all six doc gates, sequence smoke, CI-only-das compile
  sweep. Parallelize on big machines.
- **Cross-platform is a requirement, not a nicety**: the same tool must run on
  Windows (primary dev box), macOS, and WSL/Linux. Implications: pure `.das` +
  clargs + `fio` path helpers (no shell-specific constructs); syntax pass is
  `clang-cl /Zs` on Windows and `clang -fsyntax-only` elsewhere behind one
  flag; binary/build-dir discovery handles both MSVC multi-config
  (`bin/Release/daslang.exe`) and single-config (`build/daslang`) layouts;
  gates that need missing host tools (sphinx, clang) degrade to an explicit
  SKIP with the install hint, never a silent pass.
- clang syntax-only checker for changed `.cpp`/`.h` (`clang-cl /Zs` with
  project includes on Windows, `clang -fsyntax-only` on mac/Linux) — the
  80/20 for the MSVC-vs-clang diagnostic gap.
- Docs-gate runner mirroring doc.yml steps 1–6 exactly.
- CI-only das surface list (dasOpenGL, sequence, release tooling …) compiled
  via `compile_check` with proper mounts.

## Stage 3 — local build matrix

- clang-cl CMake preset in a gitignored alt build dir, wired into
  `preflight --full` (compile-focused; full builds affordable but rarely needed).
- mingw preset: deferred unless a mingw-specific failure class appears.
- Debug-config build target for the fused-path divergence family.

## Stage 4 — CI

- Nightly cron on daspkg-index building every index package against daslang
  master — external ABI breakage surfaces as a nightly signal instead of
  inside an unrelated PR's extended_checks.
