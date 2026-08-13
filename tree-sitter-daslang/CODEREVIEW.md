# tree-sitter-daslang Code Review Checklist

Run this list on every tree-sitter-daslang change before it ships — including changes to this
file.

**What stays in this document:** criteria that can be checked against a diff. Nothing else.
A reader must be able to apply every entry below **without reading the code and without prior
knowledge of the module.** A rule may cite `skills/mcp_tools.md` for the reason behind it; it may
not require that section to be read before the criterion can be applied. If an entry needs
code-reading or prior knowledge, it is not a review criterion — move it to `skills/mcp_tools.md`
and leave a one-line criterion here.

**This file reviews itself: a rule a reviewer cannot apply as written is a defect of this
file.** Mark it like any other finding — a checklist defect blocks nothing, but its fix (a
rewrite or a move, never silent tolerance) lands in the same batch as the round's other fixes.

**New functionality ships with tests — same PR, no follow-up promises.** A new or changed
reachable branch ships a test that fails without it; a diff that adds a branch no test
distinguishes is a defect. The audit procedure — including the negative control that settles
"would it fail?" — is `skills/tdd_audit.md`.

**Form, and it is a hard limit:**

- **One rule is one short paragraph.** An entry that needs more than that is describing how
  to write code, not how to review it. Split it or move it.
- **Rules are unnumbered.** No ordinal labels and no section numbers — numbering invites
  citation. Anything that needs a stable reference lives in `skills/mcp_tools.md`.
- **Cite files by name; cite `skills/mcp_tools.md` by section.** Never cite an entry in this
  file.
- **Name the API a rule is about; never name an example of it.** A rule governing specific
  functions or files must name them or it cannot be checked — that name is the criterion. An
  illustrative aside ("the way the X family does it") has no such excuse: nothing keeps it
  in sync with the code, and a stale example is worse than none.
- **One sentence of WHY is allowed where it makes the criterion decidable; anything longer
  belongs in `skills/mcp_tools.md`.** No history, no PR numbers, no direction of travel; planned
  work lives in the follow-up ledgers.

---

## Grammar canary — drift contract

**Any change to `grammar.js` regenerates `parser.c`, rebuilds all three consumers** (the
tree_sitter_daslang shared module/DLL, `daslang`, `daslang-live`) **and shows a green
`modules/dasImgui/tests/test_grammar_canary.das` — in the same change.** "It still parses" is not
the gate; the canary is.

**New syntax (`src/parser/ds2_parser.ypp` or `grammar.js`) lands with a new canary section in
`modules/dasImgui/tests/test_grammar_canary.das` in the same PR** — the canary only protects
syntax it pins.

**A red canary names the section that broke. Never ship around it by deleting or loosening
sections.**
