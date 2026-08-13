# Playground UI (web/examples/ui) Code Review Checklist

Run this list on every change under this folder before it ships — including changes to this file.

**What stays in this document:** criteria that can be checked against a diff. Nothing else.
A reader must be able to apply every entry below **without reading the code and without prior
knowledge of the module.** A rule may cite `site/README.md` for the reason behind it; it may
not require that section to be read before the criterion can be applied. If an entry needs
code-reading or prior knowledge, it is not a review criterion — move it to `site/README.md`
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
  citation. Anything that needs a stable reference lives in `site/README.md`.
- **Cite files by name; cite `site/README.md` by section.** Never cite an entry in this file.
- **Name the API a rule is about; never name an example of it.** A rule governing specific
  functions or files must name them or it cannot be checked — that name is the criterion. An
  illustrative aside has no such excuse: nothing keeps it in sync with the code, and a stale
  example is worse than none.
- **One sentence of WHY is allowed where it makes the criterion decidable; anything longer
  belongs in `site/README.md`.** No history, no PR numbers, no direction of travel; planned
  work lives in the follow-up ledgers.

---

**This folder is the deployed playground's UI source — every rule of `site/CODEREVIEW.md`
binds a change here as if the files lived under `site/playground/`.** The deploy vendors
these files into `_site/playground/` (`pages.yml`), so `site/playground/` never shows the
change and the walk from here is the only way that checklist fires.
