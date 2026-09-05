# preflight

Run CI's gates locally before pushing. The CI-lane <-> gate mapping, the tiers and the
manual commands this tool automates live in
[skills/internal/preflight.md](https://github.com/GaijinEntertainment/daScript/blob/master/skills/internal/preflight.md);
the budget the tiers serve (a full run fits 20 minutes on the M5 box) is
`plans/ci_preflight_budget.md`.

```bash
# fast tier, serial, a red stops the run: untracked, format --verify, lint changed .das (three
# rails), hash-refs, review-md, md-ascii, ast-verify changed .das (batch mode, parallel, 300s/file),
# clang frontend pass on changed C++, dasgen freshness, ci-das, ci-matrix, compile-sweep (every
# program root under utils/, examples/, tutorials/ and the modules' examples and utils, in parallel)
daslang utils/internal/preflight/main.das

# --full adds the lanes, every one at once so the wall is the longest: docs (the seven doc.yml
# gates), tests-cpp, tests-interp, tests-jit, tests-aot (builds test_aot first), utils-tests
daslang utils/internal/preflight/main.das -- --full
daslang utils/internal/preflight/main.das -- --full --serial     # the lanes one after another, for diagnosis

# module gates never run from a tier - name them when the module is the work
daslang utils/internal/preflight/main.das -- --only imgui
daslang utils/internal/preflight/main.das -- --only dasllama-model-free

# subset / introspection: --list-gates prints tier, reach and description
daslang utils/internal/preflight/main.das -- --list-gates
daslang utils/internal/preflight/main.das -- --only docs,ci-das
daslang utils/internal/preflight/main.das -- --skip tests-aot --full

# pre-push lint gate: the changed-set lint (rails and cost: skills/internal/preflight.md, the lint row)
daslang utils/internal/preflight/main.das -- --only lint
```

A gate with a reach set (`dasgen`, `docs`, `utils-tests`, `ci-matrix`, the module gates) skips
with the reason when nothing under its paths or the core (`src/`, `include/`, `daslib/`,
`dastest/`, `CMakeLists.txt`, `cmake/`) changed against `--base` (default `origin/master`);
`--only` runs a gate whatever changed. Each verdict line carries its breakdown indented beneath
it, and the run closes with a time-by-gate table.

A complete `--full` run requires a Release host and fails immediately when
given `bin/Debug/daslang`. Debug remains available for focused `--only` or
`--skip` diagnosis, but running the entire ~13k-file matrix under it is never
an acceptable pre-push substitute. On Windows, if the Release runtime DLL is
locked by a `utils/mcp/main.das` process, stop that worktree's MCP host and
retry the Release build; its watcher restarts it. Do not fall back to Debug.

Machine-specific defaults live in a read-only `preflight.toml`; preflight
never rewrites it. The default path is `%LOCALAPPDATA%/daslang/preflight.toml`
on Windows, `~/Library/Application Support/daslang/preflight.toml` on macOS,
and `$XDG_CONFIG_HOME/daslang/preflight.toml` (falling back to
`~/.config/daslang/preflight.toml`) on Linux. `DAS_PREFLIGHT_CONFIG` selects a
different file. Command-line flags override the file.

```toml
[jit]
threads = 48
max_file_time = 60
```

Without a machine setting, JIT isolation uses half the logical CPUs, clamped
to 4..32 workers. `--jit-jobs=0` requests that automatic rule explicitly.

Cross-platform (Windows / macOS / Linux+WSL): subprocesses go through
`popen_argv` (no shell) - except the sequence gate, which by design runs CI's
own smoke scripts under pwsh/bash. The C++ pass uses `clang-cl /Zs` on
Windows (preferring the VS-bundled clang - the same binary CI's ClangCL
toolset uses) and `clang -fsyntax-only` elsewhere; both are frontend-only
(parse + semantic analysis + template instantiation, no codegen), which keeps
even the full ~200-TU header-change sweep at ~15-30 s. A gate whose host tool
or module is missing reports `SKIP` with an install/rebuild hint instead of
passing silently. Exit code is non-zero when any gate fails.

The full interpreter and AOT sweeps pass `--max-file-time` to `dastest`. The
parallel cold JIT sweep defaults to 60 seconds because healthy files can cross
30 seconds under worker contention. Any completed test file above its ceiling
fails preflight even when its assertions pass; the suite-wide timeout remains
the separate deadlock guard.

`ci_only_das.txt` lists the in-repo das surface that no default local build
compiles (dasOpenGL today); see the header comment there before adding
entries - surfaces that pull external daspkg packages belong to the
`sequence` gate, not the compile sweep. The compile sweep's own exclusions,
each with its reason, are `SWEEP_EXCLUDED` in `config.das`.
