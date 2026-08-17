# daspkg Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first — its contract binds this checklist.** Architecture doc:
`README.md`.

**Run the unit suite on every change:** `bin/daslang dastest/dastest.das -- --test
utils/daspkg/test_daspkg.das` — fast, no network, interpreted. A daspkg change without a green
unit run is a defect.

**The integration suite — `bin/daslang dastest/dastest.das -- --test
utils/daspkg/test_daspkg_git.das` — runs on any edit to `commands.das`, `index.das`, or
`utils.das` whose hunks reach a function that runs a git command** — a behavior-preserving
refactor still counts. Needs network (the `borisbat/daspkg-test-*` fixture repos).

**A change to `cmd_release`, `cmd_release_wasm`, or a `release_*` helper is verified on macOS,
or the review says it was not.** The release layout forks per platform (`.app` bundle vs flat
directory), and no per-PR CI runs these suites.

**A new command or flag lands with its test cell, its `print_usage` line, and its README table
row in the same change.**

**daspkg ships as a prebuilt exe: it stays in `DAS_UTILS_SHIPPED_EXES` (`utils/CMakeLists.txt`)
and `main.das` stays `daslang -exe`-compilable** — a change that breaks the exe build breaks the
release bundle.

**A release always mints the tune sidecar.** A bundle that ships an exe without a sidecar
beside it is a defect.

**`--quick` is the only path that inherits a prior sidecar, and it refuses an incomplete one**
— incomplete meaning missing any scope key the exe's deps JSON reports, the same completeness
the release build itself checks.

**`release_include_if_missing` files are user-owned after initialization.** A release path that
overwrites or deletes one, on any platform, is a defect.

**`.daspkg_release.manifest` is written on every platform.**

**Unit cells touch only local fixtures.** A `test_daspkg.das` cell that reaches the network is
a defect — network coverage belongs in `test_daspkg_git.das`.

**A package, bundle, or app name that reaches a shell command is validated by
`is_safe_pkg_name` first, and only `commands.das` may build such a command** (the validator is
private to it); an interpolation site without the check, or outside `commands.das`, is a defect.
