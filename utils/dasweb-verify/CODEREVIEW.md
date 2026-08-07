# dasweb-verify Code Review Checklist

Run this list on every dasweb-verify change before it ships — including changes to this file.
Entries must be checkable against a diff alone; anything needing prior knowledge or another
document belongs in `README.md` with a one-line criterion here.

## Tests

**Every core behavior has a dastest test in this directory.** Exempt: `main.das` argv/exit
glue — its pieces are the tested module.

**Every bug fix lands with the regression test that fails without it, in the same change.**

**`[test]` files live in this directory and require siblings by bare name** — never under the
global `tests/` tree, and never registered in any `CMakeLists.txt`.

**A test that touches the filesystem uses `temp_directory`-rooted paths and deletes what it
creates.** A test writing into the repo tree is a defect.

## Placement — one file, one rule

- `main.das` — the launcher: clargs parsing, per-sample reporting, exit-code mapping. No
  manifest parsing, no compilation.
- `verify_core.das` — manifest parsing and compile execution. Zero network.

**A new file ships with its rule here and its tests, in the same change.**

## Behavior

**The verifier reads the manifest the playground ships (`data.json`), never a hand-maintained
copy.** A second sample list in this directory is a defect.

**The generated-sample mapping in `verify_core.das` mirrors
`web/stage_playground_imgui_samples.cmake`.** A change to either without the other, in the same
change, is a defect.

**Manifest handling fails closed.** A missing, unparseable, or degenerate manifest (or sample
entry) is a named error and a non-zero exit, never a silent skip.

**Every failure line names the sample and carries the underlying message.** A failure a reader
cannot act on from the log alone is a defect.
