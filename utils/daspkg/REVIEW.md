# daspkg Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first — its contract binds this checklist.** Architecture doc:
`README.md`.

## Tests

**Run the unit suite on every change:**

```text
bin/daslang dastest/dastest.das -- --test utils/daspkg/test_daspkg.das
```

Fast, no network, runs interpreted. A daspkg change without a green unit run is a defect.

**Run the integration suite on any edit to `commands.das`, `index.das`, or `utils.das` — a
behavior-preserving refactor still counts:**

```text
bin/daslang dastest/dastest.das -- --test utils/daspkg/test_daspkg_git.das
```

Needs network (the `borisbat/daspkg-test-*` fixture repos). Exception: a change confined to
functions that run no git command (checkable in the diff's own hunks) stays the unit suite's.

**A change to `cmd_release`, `cmd_release_wasm`, or a `release_*` helper is verified on macOS,
or the review says it was not.** The release layout forks per platform (`.app` bundle vs flat
directory), and no per-PR CI runs these suites.

**A new command or flag lands with its test cell, its `print_usage` line, and its README table
row in the same change.**

## Behavior

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

**A package, bundle, or app name that reaches a shell command is validated by `is_safe_pkg_name`
first** — a new interpolation site in `commands.das` without that check is a defect. The
validator is `def private` to `commands.das`, so no validator is reachable from any other file
here: a new shell command interpolating a package, bundle, or app name outside `commands.das` is
a defect.
