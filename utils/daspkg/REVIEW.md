# daspkg Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture doc:
`README.md`.

**A change to a `.das` file in this folder without a green unit run is a defect.** The unit
suite is `bin/daslang dastest/dastest.das -- --test utils/daspkg/test_daspkg.das` - fast, no
network, interpreted.

**A diff whose changed lines sit inside a function that runs a git command - in
`commands.das`, `index.das`, or `utils.das` - also runs the integration suite, in the same
change** - a behavior-preserving refactor still counts. The integration suite is `bin/daslang dastest/dastest.das -- --test
utils/daspkg/test_daspkg_git.das`, and it needs network (the `borisbat/daspkg-test-*` fixture
repos).

**A change to `cmd_release`, `cmd_release_wasm`, or a `release_*` helper is verified on macOS,
or the review says it was not.** The release layout differs per platform (`.app` bundle vs flat
directory), and no per-PR CI runs the daspkg test suites.

**A diff that adds a command or a flag also adds its test, its `print_usage` line, and its
`README.md` table row, in the same change.**

**A diff that drops `daspkg` from `DAS_UTILS` or `DAS_UTILS_SHIPPED_EXES` in
`utils/CMakeLists.txt` (repo root) is a defect** - daspkg ships as a prebuilt exe, and dropping
either entry breaks the release bundle.

**A `cmd_release` bundle that ships an exe without a tune sidecar beside it is a defect** - the
tune sidecar is the `<bundle>.tune.json` file of measured kernel choices the exe reads at run
time.

**A diff that lets a release path other than `--quick` inherit a prior sidecar, or lets
`--quick` accept an incomplete one, is a defect** - incomplete means missing any scope key - a
`tune_scopes` entry the exe's deps JSON reports - the same completeness the release build
itself checks.

**A release path that overwrites or deletes a `release_include_if_missing` file, on any
platform, is a defect.**

**A release path that finishes a bundle without writing `.daspkg_release.manifest`, on any
platform, is a defect.**

**A test in `test_daspkg.das` that reaches the network is a defect** - network coverage belongs
in `test_daspkg_git.das`.

**A shell command built from a package, bundle, or app name outside `commands.das`, or without
an `is_safe_pkg_name` check on that name first, is a defect** - `is_safe_pkg_name` is private to
`commands.das`.
