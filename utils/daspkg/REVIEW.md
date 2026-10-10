# daspkg Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture doc:
`README.md`.

**A change to a `.das` file in this folder without a green unit run is a defect.** The unit
suite is `bin/daslang dastest/dastest.das -- --test utils/daspkg --exclude test_daspkg_git` -
no network, interpreted.

**A diff whose changed lines sit inside `run_cmd` (`utils.das`), or inside a function in any
`.das` in this folder that passes a `git` command line to `run_cmd` or calls a function whose
name starts with `git_`, also runs the integration suite, in the same change.** The integration
suite is `bin/daslang dastest/dastest.das -- --test utils/daspkg/test_daspkg_git.das`, and it
needs network (the `borisbat/daspkg-test-*` fixture repos).

**A diff whose changed lines in a release function write a file into the bundle, choose a
file's name or location inside it, or build the command that compiles or links an exe, runs
that release and states in the review which platform it ran on.** A release function is
`cmd_release`, `cmd_release_wasm`, or a function in `commands.das` that either one calls,
directly or through other `commands.das` functions. The native release layout differs per
platform (`.app` bundle vs flat directory), and the wasm release links with the emsdk installed
on that platform.

**A diff that adds a command also adds its `print_usage` line and its row in the `README.md`
Commands table, in the same change.**

**A diff that adds a `daspkg` command-line flag gives its `DaspkgArgs` field a `@clarg_doc`
annotation and adds a row for the flag to the `README.md` Options table, in the same change** -
the help text renders the annotation, so a field without one is a blank help line.

**A diff that changes a flag's `@clarg_doc` text updates that flag's row in the `README.md`
Options table to say the same thing, in the same change.**

**A diff that adds a `.das_package` manifest function - a function `daslib/daspkg.das` (repo
root) exports for a manifest body to call - also adds it to the `README.md` `.das_package`
manifest section, in the same change.**

**A diff that lets a `cmd_release` bundle built without `--fat` finish without a
`<stem>.tune.json` sidecar beside every exe it ships - the main exe, and each companion the
package's `release()` declares with `release_program` - is a defect** - the sidecar holds the
measured kernel choices an exe reads at run time, under that exe's own file name.

**A diff that lets a `--fat` release finish while any scope's `fat_unprofiled` list is non-empty
in any deps JSON the release wrote - the file `daslang -exe --list-shared-modules` writes beside
a built exe - or while one of those files cannot be read, is a defect** - `release_fat_gate`
refuses; a fat exe never tunes.

**A diff that lets a release path other than `--quick` reuse a sidecar from an earlier run is a
defect.**

**A diff that lets `--quick` accept an incomplete sidecar is a defect** - incomplete means
missing a scope key, that is, an entry of the `tune_scopes` list in any deps JSON the release
wrote.

**A diff that lets a release finish while a `release_program` companion's deps JSON declares a
tune scope the main program's deps JSON does not is a defect** - the release refuses that
companion.

**A diff that lets a release path overwrite or delete a `release_include_if_missing` file is a
defect** - one the package's `release()` declares that way: a starter file deployed once, then
owned by the user.

**A diff that lets a `cmd_release` bundle finish without writing `.daspkg_release.manifest` is
a defect.**

**A test outside `test_daspkg_git.das` that reaches the network is a defect - put it in
`test_daspkg_git.das`.**

**A diff that builds a shell command or filesystem path from a name a `.das_package` or the
command line supplied that becomes one file name, one directory name, or one bare word of a
shell command - never a directory the command line names as an input or output root - outside
`commands.das`, or before `is_safe_pkg_name` accepts the whole name, is a defect.**
`is_safe_pkg_name` is private to `commands.das`, and a string carrying a space, a quote, a
separator or `..` splits the command or reads outside the directory the path was built for.

**A diff that builds a shell command or filesystem path from a `/`-separated path a
`.das_package` supplies is a defect when it does so outside `commands.das`, or before
`is_safe_pkg_name` accepts every segment; only a `release_program` script path and a
`release_include_from` source path may also carry `..` segments, which reach another tree.**
