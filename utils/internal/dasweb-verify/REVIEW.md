# dasweb-verify Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture doc:
`README.md`. A `.mjs` file or `expectations.json`, wherever the diff puts it, answers to
`browser/REVIEW.md`.

**Every core behavior has a dastest test in this directory** - `main.das` stays thin argv/exit
glue over tested pieces, so it needs none of its own.

**`[test]` files live in this directory and require siblings by bare name** - never under the
global `tests/` tree, and never registered in any `CMakeLists.txt`.

**A test that touches the filesystem uses `temp_directory`-rooted paths and deletes what it
creates.** A test writing into the repo tree is a defect.

**Which samples exist comes only from the manifest the playground ships
(`web/examples/ui/samples/data.json`, repo root), never a hand-maintained copy.** A file
under this tool that lists, adds, or omits samples is a defect; a file carrying per-sample
data keyed by manifest name is not one.

**The generated-sample mapping in `verify_core.das` mirrors
`web/stage_playground_imgui_samples.cmake`:** a change to either lands with the other in the
same change.

**Weakening the fail-closed cases in `test_verify_core.das` is a defect** - a missing,
unparseable, or degenerate manifest or sample entry stays a named error returning no
entries, and `main.das` still maps that to a non-zero exit.

**Every failure line names the sample and carries the underlying message.** A failure a reader
cannot act on from the log alone is a defect.

**Placement - one file, one line: a diff keeps each file inside its line, and a new file adds
its line here, with its tests, in the same change.**

- `main.das` - launcher: clargs parsing, per-sample reporting, exit-code mapping. No
  manifest parsing, no compilation.
- `verify_core.das` - manifest parsing, generated-sample resolution, and compile execution.
  Zero network.
- `browser/` - the browser leg; its files answer to `browser/REVIEW.md`.
