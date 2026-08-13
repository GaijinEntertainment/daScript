# preflight

Run CI's gates locally before pushing. The CI-lane ↔ gate mapping and the
manual commands this tool automates live in
[skills/preflight.md](https://github.com/GaijinEntertainment/daScript/blob/master/skills/preflight.md).

```bash
# fast tier (seconds): format --verify, lint changed .das, clang frontend pass
# on changed C++ (full src+tests-cpp sweep when a header changed)
daslang utils/preflight/main.das

# full tier: adds the untracked-files gate (working tree carries none — commit,
# delete, or ignore each), dasgen freshness, CI-only-das compile sweep, the
# six doc gates, ctest -L small, interpreter/JIT/AOT suites, sequence smoke
daslang utils/preflight/main.das -- --full

# subset / introspection
daslang utils/preflight/main.das -- --list-gates
daslang utils/preflight/main.das -- --only docs,ci-das
daslang utils/preflight/main.das -- --skip tests-aot --full
```

A complete `--full` run requires a Release host and fails immediately when
given `bin/Debug/daslang`. Debug remains available for focused `--only` or
`--skip` diagnosis, but running the entire ~12k-file matrix under it is never
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
`popen_argv` (no shell) — except the sequence gate, which by design runs CI's
own smoke scripts under pwsh/bash. The C++ pass uses `clang-cl /Zs` on
Windows (preferring the VS-bundled clang — the same binary CI's ClangCL
toolset uses) and `clang -fsyntax-only` elsewhere; both are frontend-only
(parse + semantic analysis + template instantiation, no codegen), which keeps
even the full ~160-TU header-change sweep at ~15-30 s. A gate whose host tool
or module is missing reports `SKIP` with an install/rebuild hint instead of
passing silently. Exit code is non-zero when any gate fails.

The full interpreter and AOT sweeps pass `--max-file-time 30` to `dastest`.
The parallel cold JIT sweep defaults to 60 seconds because healthy files can
cross 30 seconds under worker contention. Any completed test file above its
ceiling fails preflight even when its assertions pass; the suite-wide timeout
remains the separate deadlock guard.

`ci_only_das.txt` lists the in-repo das surface that no default local build
compiles (dasOpenGL today); see the header comment there before adding
entries — surfaces that pull external daspkg packages belong to the
`sequence` gate, not the compile sweep.
