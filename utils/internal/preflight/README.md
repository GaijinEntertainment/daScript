# preflight

Run CI's gates locally before pushing. The CI-lane <-> gate mapping, the tiers and the
manual commands this tool automates live in
[skills/internal/preflight.md](https://github.com/GaijinEntertainment/daScript/blob/master/skills/internal/preflight.md).
The budget the tiers serve: a full run fits 20 minutes on the M5 box.

```bash
# fast tier, serial, a red stops the run: untracked, format --verify, lint changed .das and .md (three
# rails), hash-refs, review-md, md-ascii, ast-verify changed .das (batch mode, parallel, 600s/file),
# clang frontend pass on changed C++, dasgen freshness, ci-das, ci-matrix, compile-sweep (every
# program root under utils/, examples/, tutorials/ and the modules' examples and utils, in parallel)
daslang utils/internal/preflight/main.das

# --full adds the lanes, all at once: docs (the seven doc.yml gates), tests-cpp, tests-interp,
# tests-jit, watchdog-interp / watchdog-jit (tests/watchdog, which no sweep of tests/ runs),
# tests-aot and utils-tests. What takes turns among them is "Build turns" below, and when the
# docs lane starts is "The docs lane beside the fast tier". utils-tests builds
# run_utils_tests_products and runs the suites of
# run_utils_tests itself, every suite at once, from the list cmake writes to
# <build>/run_utils_tests.txt - so a suite works only in folders no other suite names.
# The wall is the longest lane
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
threads = 12
max_file_time = 60
```

Without a machine setting, JIT isolation uses half the logical CPUs, clamped
to 4..16 workers. `--jit-jobs=0` requests that automatic rule explicitly.

Cross-platform (Windows / macOS / Linux+WSL): subprocesses go through
`popen_argv` (no shell) - except the sequence gate, which by design runs CI's
own smoke scripts under pwsh/bash. The C++ pass uses `clang-cl /Zs` on
Windows (preferring the VS-bundled clang - the same binary CI's ClangCL
toolset uses) and `clang -fsyntax-only` elsewhere; both are frontend-only
(parse + semantic analysis + template instantiation, no codegen), which keeps
even the full ~200-TU header-change sweep at ~15-30 s. A gate whose host tool
or module is missing reports `SKIP` with an install/rebuild hint instead of
passing silently. Exit code is non-zero when any gate fails.

The interpreter, JIT and AOT sweeps run isolated, twenty files a worker
process. The interpreter and AOT sweeps always take the automatic worker
count: the wall of a sequential sweep is the sum of a few long files, and a
batch of twenty keeps the CPU near the sequential run's.

## Build turns {#build-turn}

The lanes of one `--full` run share one build lock: a folder named once per run in the temp
dir and passed to each lane child as `--build-lock`. A lane starts its turn by creating the
folder and ends it by removing the folder. A preflight started without `--build-lock` takes
no turns. A lane that waits past the wait limit fails the step it was waiting for. Every
`cmake --build` in the shared build dir runs inside a turn, because two builds in one dir
collide on MSBuild's tlog and object files. A step that takes every core also takes a turn -
off Windows that is sphinx - so that step and the builds wait for each other. A build re-runs
cmake part way when a file came or went under a `CONFIGURE_DEPENDS` glob after the build's
own check. Under MSBuild, whose projects then regenerate all at once, that build fails, and
configures that race can leave the generated module lists written twice. So when a failed
build re-ran cmake, `run_build` configures once more and builds a second time, while it still
has its turn.

## The docs lane beside the fast tier {#docs-early}

On Windows the docs lane of a `--full` run starts with the fast tier, not after it. It reads
nothing the fast tier checks, and its sphinx step is one process that takes no build turn, so
its time is hidden behind the fast tier's. A red fast tier stops it. Off Windows sphinx fills
every core, so the docs lane starts with the other lanes. A run with `--only` or `--serial`
starts nothing early.

The full interpreter and AOT sweeps pass `--max-file-time` to `dastest`. The
parallel cold JIT sweep defaults to 60 seconds because healthy files can cross
30 seconds under worker contention. Any completed test file above its ceiling
fails preflight even when its assertions pass. Every sweep also passes
`--test-timeout 60`: a worker that never finishes is killed with everything it
started - at 1200 seconds for a batch of twenty, under the suite's 1800 - and the
files it had not reported run again one by one. The suite-wide timeout remains
the guard for a dastest that itself stops.

`ci_only_das.txt` lists the in-repo das surface that no default local build
compiles (dasOpenGL today); see the header comment there before adding
entries - surfaces that pull external daspkg packages belong to the
`sequence` gate, not the compile sweep. The compile sweep's own exclusions,
each with its reason, are `SWEEP_EXCLUDED` in `config.das`; the trees whose roots need a
native module the box may not have built (dasVulkan, off by default on Apple) are
`SWEEP_NEEDS_MODULE` there, swept only while the module's shared artifact exists. A root that
requires `dasllama/` is left out too: each pays the engine compile, a quarter of a minute,
so CI and the module's own gates compile those.
