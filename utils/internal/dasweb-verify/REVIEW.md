# dasweb-verify Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture doc:
`README.md`. A `.mjs` file or `expectations.json`, wherever the diff puts it, answers to
`browser/REVIEW.md`.

**A diff that adds or changes a core behavior also adds a dastest test for it in this
directory, in the same change.** `main.das` is thin argv and exit-code glue over tested
pieces, so it needs no test of its own.

**Never put a `[test]` file outside this directory, and never register one in any
`CMakeLists.txt` - keep it here and require its siblings by bare name.**

**Never add a test that touches the filesystem outside a `temp_directory`-rooted path, or
that leaves behind what it creates.**

**A file under this tool that lists, adds, or omits samples is a defect - take the sample list
only from the manifest the playground ships (`web/examples/ui/samples/data.json`, repo
root).** A file carrying per-sample data keyed by manifest name is not such a list.

**A diff that changes either the generated-sample mapping in `verify_core.das` or
`web/stage_playground_imgui_samples.cmake` also updates the other, in the same change.**

**Weakening the fail-closed cases in `test_verify_core.das` is a defect** - a missing,
unparseable, or degenerate manifest or sample entry still raises a named error and returns no
entries, and `main.das` still maps that to a non-zero exit.

**A diff that adds or changes a failure line puts the sample name and the underlying message
in that line.** A failure a reader cannot act on from the log alone is a defect.

**Placement - one file, one line: a diff keeps each file inside its line, and a new file adds
its line here, with its tests, in the same change.**

- `main.das` - launcher: clargs parsing, per-sample reporting, exit-code mapping. No
  manifest parsing, no compilation.
- `verify_core.das` - manifest parsing, generated-sample resolution, and compile execution.
  Zero network.
- `browser/` - the browser leg; its files answer to `browser/REVIEW.md`.
