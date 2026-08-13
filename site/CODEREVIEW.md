# site (daslang.io) Code Review Checklist

Run this list on every site change before it ships — including changes to this file.

**What stays in this document:** criteria that can be checked against a diff. Nothing else.
A reader must be able to apply every entry below **without reading the code and without prior
knowledge of the module.** A rule may cite `README.md` for the reason behind it; it may
not require that section to be read before the criterion can be applied. If an entry needs
code-reading or prior knowledge, it is not a review criterion — move it to `README.md`
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
- **No numbers.** These are criteria, not a spec, and numbering invites citation. Anything
  that needs a stable reference lives in `README.md`, which is numbered for that purpose.
- **Cite files by name; cite `README.md` by section.** Never cite an entry in this file.
- **Name the API a rule is about; never name an example of it.** A rule governing specific
  functions or files must name them or it cannot be checked — that name is the criterion. An
  illustrative aside has no such excuse: nothing keeps it in sync with the code, and a stale
  example is worse than none.
- **One sentence of WHY is allowed where it makes the criterion decidable; anything longer
  belongs in `README.md`.** No history, no PR numbers, no direction of travel; planned
  work lives in the follow-up ledgers.

---

**Every shell command shown on a page runs verbatim and produces the result the page shows.**
A command, flag, or output line invented for illustration is a defect; if the implementation
does not exist yet, the page does not show the command.

**Every code sample shown on a page compiles and runs with the current toolchain.** daslang
samples are gen2 and compile with the current binary; no pseudo-code presented as code.

**Every number shown is either rendered from live data or copied from a checked-in
measurement record; anything else is a placeholder and carries an HTML comment naming it as
one.** A placeholder that could be mistaken for a fact is a defect.

**The `dl-*` measurement-table language's source of truth is `files/dasllama-table.css`**
(the file dasllama.io loads). This page does not link it — it carries an inline mirror — so a
`dl-*` change updates the source AND this page's inline copy together; changing only one is a defect.

**News entries state real, shipped events.** An entry in `_news/*.md` for something not yet
true at publish time is a defect.
