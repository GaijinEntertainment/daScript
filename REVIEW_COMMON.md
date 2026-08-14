# REVIEW.md — the common contract

Every folder-scoped `REVIEW.md` is a binding per-change review checklist for the files under
its folder: run it on every such change before it ships, including a change to the checklist
itself. This file is the part all checklists share; a checklist's own text carries only its
folder's criteria, and its opening block points here. An external repo that adopts `REVIEW.md`
vendors a copy of this file at its root.

**What stays in a checklist: rules that can be checked against a diff. Nothing else.**
A reader must be able to apply every rule **without reading the code and without prior
knowledge of the folder.** A rule may cite the architecture doc for the reason behind it; it
may not require that section to be read before the rule can be applied. If a rule needs
code-reading or prior knowledge, it is not diff-checkable — move it to the architecture
doc and leave a one-line rule in the checklist.

**Every checklist reviews itself: a rule a reviewer cannot apply as written is a defect of the
checklist.** Mark it like any other finding — a checklist defect blocks nothing, but its fix (a
rewrite or a move, never silent tolerance) lands in the same batch as the round's other fixes.

**New functionality ships with tests — same PR, no follow-up promises.** A new or changed
reachable branch ships a test that fails without it; a diff that adds a branch no test
distinguishes is a defect. The audit procedure — including the negative control that settles
"would it fail?" — is `skills/tdd_audit.md`.

**Form, and it is a hard limit:**

- **One rule is one short paragraph.** A rule that needs more than that is describing how to
  write code, not how to review it. Split it or move it.
- **Rules are unnumbered.** No ordinal labels and no section numbers — numbering invites
  citation. Anything that needs a stable reference lives in the checklist's architecture doc,
  which each checklist names in its opening.
- **Cite files by name; cite the architecture doc by section.**
  No file cites a rule in a checklist by number, position, or nickname — quote the rule's
  words instead. A quote states the reason the code is shaped this way and survives any
  rewording; a position points at whatever sits there today.
- **Name the API a rule is about; never name an example of it.** A rule governing specific
  functions or files must name them or it cannot be checked — that name is the criterion. An
  illustrative aside ("the way the X family does it") has no such excuse: nothing keeps it in
  sync with the code, and a stale example is worse than none.
- **One sentence of WHY is allowed where it makes the criterion decidable; anything longer
  belongs in the architecture doc.** No history, no PR numbers, no direction of travel;
  planned work lives in the folder's follow-up ledger (a checklist that keeps one elsewhere
  names it in its opening).
