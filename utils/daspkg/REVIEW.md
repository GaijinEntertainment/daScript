# daspkg Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture doc:
`README.md`.

**A change to a `.das` file in this folder without a green unit run is a defect.** The unit
suite is `bin/daslang dastest/dastest.das -- --test utils/daspkg/test_daspkg.das` - fast, no
network, interpreted.

**A diff whose changed lines sit inside a function that runs a git command - in
`commands.das`, `index.das`, or `utils.das` - also runs the integration suite, in the same
change.** The integration suite is `bin/daslang dastest/dastest.das -- --test
utils/daspkg/test_daspkg_git.das`, and it needs network (the `borisbat/daspkg-test-*` fixture
repos).

**A change to `cmd_release`, `cmd_release_wasm`, or a `release_*` helper states in the review
whether the release was run on macOS.** The release layout differs per platform (`.app` bundle
vs flat directory).

**A diff that adds a command or a flag also adds its `print_usage` line and its `README.md`
table row, in the same change.**

**A `cmd_release` bundle that ships an exe without a tune sidecar beside it is a defect** - the
tune sidecar is the `<bundle>.tune.json` file of measured kernel choices the exe reads at run
time.

**A diff that lets a release path other than `--quick` reuse a sidecar from an earlier run is a
defect.**

**A diff that lets `--quick` accept an incomplete sidecar is a defect** - incomplete means
missing a scope key, that is, an entry of the `tune_scopes` list in the deps JSON that
`daslang -exe --list-shared-modules` writes.

**A release path that overwrites or deletes a `release_include_if_missing` file is a defect** -
one the package's `release()` declares that way: a starter file deployed once, then owned by
the user.

**A diff that removes `daspkg` from `DAS_UTILS_SHIPPED_EXES` in `utils/CMakeLists.txt` is a
defect.** The removal drops the prebuilt daspkg exe from the release bundle, and the parent's
gate cannot see a removed entry - it reads this sentence to know the decision stands.

**A `cmd_release` bundle that finishes without writing `.daspkg_release.manifest` is a
defect.** `release wasm` is out: it wipes its output directory first, so it has no stale file to
account for.

**A test in `test_daspkg.das` that reaches the network is a defect** - network coverage belongs
in `test_daspkg_git.das`.

**A shell command built from any string a `.das_package` supplies - a package, bundle, app or
module name - outside `commands.das`, or without an `is_safe_pkg_name` check on that string
first, is a defect** - `is_safe_pkg_name` is private to `commands.das`. The emcc flags of
`release_emcc_arg` are the carve-out: free-form shell text by design, appended verbatim.
