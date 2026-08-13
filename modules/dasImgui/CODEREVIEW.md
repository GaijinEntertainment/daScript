# dasImgui Code Review Checklist

Run this list on every dasImgui change before it ships — including changes to this file.

**What stays in this document:** criteria that can be checked against a diff. Nothing else.
A reader must be able to apply every entry below **without reading the code and without prior
knowledge of the module.** A rule may cite `modules/dasImgui/CLAUDE.md` for the reason behind
it; it may not require that section to be read before the criterion can be applied. If an
entry needs code-reading or prior knowledge, it is not a review criterion — move it to
`modules/dasImgui/CLAUDE.md` and leave a one-line criterion here.

**This file reviews itself: a rule a reviewer cannot apply as written is a defect of this
file.** Mark it like any other finding — a checklist defect blocks nothing, but its fix (a
rewrite or a move, never silent tolerance) lands in the same batch as the round's other fixes.

**New functionality ships with tests — same PR, no follow-up promises.** A new or changed
reachable branch ships a test that fails without it; a diff that adds a branch no test
distinguishes is a defect. The audit procedure — including the negative control that settles
"would it fail?" — is `skills/tdd_audit.md`.

**Form, and it is a hard limit:**

- **One rule is one short paragraph.** An entry that needs more than that is describing how to
  write code, not how to review it. Split it or move it.
- **No numbers.** These are criteria, not a spec, and numbering invites citation. Anything that
  needs a stable reference lives in `modules/dasImgui/CLAUDE.md`.
- **Cite files by name; cite `modules/dasImgui/CLAUDE.md` by section.** Never cite an entry in
  this file.
- **Name the API a rule is about; never name an example of it.** A rule governing specific
  functions or files must name them or it cannot be checked — that name is the criterion. An
  illustrative aside has no such excuse: nothing keeps it in sync with the code, and a stale
  example is worse than none.
- **One sentence of WHY is allowed where it makes the criterion decidable; anything longer
  belongs in `modules/dasImgui/CLAUDE.md`.** No history, no PR numbers, no direction of
  travel; planned work lives in the follow-up ledgers.

---

## Tests

**Tests go under `modules/dasImgui/tests`.** No tests go under `tests/dasImgui` — that folder
must not exist.

**Before a PR: run the suite** —
`daslang dastest/dastest.das -- --test modules/dasImgui/tests --headless --isolated-mode --isolated-mode-threads 4 --exclude glfw_synth --exclude key_hud --exclude embedded_terminal`
(also available as `preflight --only imgui`). `nightly_imgui.yml` is the suite's only CI lane,
so the local run is the pre-push gate.

**On local Windows runs, additionally pass the high-POST excludes** from
`modules/dasImgui/CLAUDE.md` § Tests:
`inputs_drag inputs_numeric inputs_slider indexed_dynamic inputs_color inputs_choice inputs_text`.

**Tests are kept multiplatform** — no hardcoded platform-specific paths, separators, or
OS-only assumptions; locations resolve via `get_das_root()` / module-relative helpers, never
absolute or machine-local paths.

**`modules/dasImgui/tests/test_grammar_canary.das` is governed by
`tree-sitter-daslang/CODEREVIEW.md`** — a grammar change touching it applies that list too.
