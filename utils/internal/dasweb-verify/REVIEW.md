# dasweb-verify Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture doc:
`README.md`. A `.mjs` file or `expectations.json`, wherever the diff puts it, answers to
`browser/REVIEW.md`.

**Never put a `[test]` file for this tool outside this directory - keep it here.**

**A `[test]` file here requires its siblings by bare name (`require verify_core`), never by a
path.**

**A diff that adds a `[test]` file here also adds it to the `run_tests_dasweb_verify` target in
`utils/CMakeLists.txt` (repo root), in the same change; a diff that names a test file from here
in any other `CMakeLists.txt` is a defect.**

**A test here writes files only under a directory it made with `create_temp_directory`, and
removes that directory before it returns.**

**A diff that makes `main.das` or `verify_core.das` choose which samples get checked - by a
sample list of its own, or by a hardcoded name added to or removed from the manifest's list - is
a defect; take the set only from the manifest the playground ships
(`web/examples/ui/samples/data.json`, repo root).**

**Weakening a `test_verify_core.das` case that feeds `load_manifest` a missing, unparseable, or
empty manifest, or a sample that lists no files, is a defect: dropping the case, dropping its
check that the error names the file or sample at fault, or dropping its check that no entries
come back.**

**A diff that makes `main.das` exit 0 after `load_manifest` sets an error is a defect.**

**A diff to `main.das` or `verify_core.das` after which the line `main.das` logs for a failed
sample lacks the sample's manifest `name`, or lacks the message of the step that failed, is a
defect.** The CI log is all a reader gets.

**Weakening `REVIEW.das` (beside this file) is a defect: dropping a check, narrowing what a
check compares, or rewriting a finding text so it no longer names what failed.**

**Placement - one file, one line: a diff keeps each file inside its line, and a new file adds
its line here, with its tests, in the same change.**

- `main.das` - launcher: clargs parsing, per-sample reporting, exit-code mapping. No
  manifest parsing, no compilation.
- `verify_core.das` - manifest parsing, generated-sample resolution, and compile execution.
  Zero network.
- `REVIEW.das` - the gate pairing `GENERATED_SAMPLES` with the stage script's reads and writes.
- `browser/` - the browser leg.
