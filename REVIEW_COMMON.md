# REVIEW.md — the common contract

Every folder-scoped `REVIEW.md` is a binding per-change review checklist for the files under
its folder: run it on every such change before it ships, including a change to the checklist
itself. This file is the part all checklists share; a checklist's own text carries only its
folder's criteria, and its opening block points here. An external repo that adopts `REVIEW.md`
vendors a copy of this file at its root.

**What stays in a checklist: rules that can be checked against a diff. Nothing else.**
A reader must be able to apply every rule **without reading the code and without prior
knowledge of the folder.** A term of art is defined in place, in a dozen words, or the rule is
rewritten around plain words. A rule may cite the folder's own architecture doc by section for
the reason behind it; it may not require that section to be read before the rule can be
applied, and it may not quote or cite any other rule document — coupling between rule
documents is done by ROUTING ("a diff touching X applies that checklist too", "a `[tune]`
change is reviewed with `skills/tune.md`"), never by restating.

**Every checklist reviews itself: a rule a reviewer cannot apply as written is a defect of the
checklist.** Mark it like any other finding — a checklist defect blocks nothing, but its fix (a
rewrite or a move, never silent tolerance) lands in the same batch as the round's other fixes.

**New functionality ships with tests — same PR, no follow-up promises.** A new or changed
reachable branch ships a test that fails without it; a diff that adds a branch no test
distinguishes is a defect. The audit procedure — including the negative control that settles
"would it fail?" — is `skills/tdd_audit.md`.

**A rule that a test or lint enforces is deleted.** Automation replaces the rule; the
checklist keeps at most "weakening that check is a defect." A rule that COULD be automated is
a lint candidate — say so in the review round.

**A rule is absolute; sanctioned violations live in the architecture doc.** No "except",
"exempt", "carve-out", or "the one sanctioned" — move the boundary inside the trigger, fix
the code so the exception has no reason to exist, or ledger the case in the architecture doc.
A ledgered case firing the absolute rule is expected: the author answers "yes, ledgered," and
that handshake is the system working.

**A rule lives in the checklist of the narrowest folder that contains its trigger.**
Subfolder-scoped rules move to that subfolder's own `REVIEW.md` — creating it is never an
objection; checklists are cheap, thousands can sit in a repository and few fire per PR. The
parent routes strays by KIND ("a `[test]` file, wherever the diff puts it, answers to
`tests/REVIEW.md`"). A rule whose trigger lies entirely outside the folder can never fire and
is deleted — better absent than dead.

**Form, and it is a hard limit:**

- **One rule is one short paragraph — bold criterion first, blank line between rules.** A
  rule that needs more than that is describing how to write code, not how to review it.
  Split it or move it. Procedure — a files-to-touch how-to — is not a rule.
- **No sections.** A section header is a bucket asking to be filled: a grouping either
  decouples into the flat list or is a separable concern that earns its own routed file.
- **Rules are unnumbered and unnicknamed** — numbering invites citation. Anything that needs
  a stable reference lives in the checklist's architecture doc.
- **Cite files by name; cite the folder's own architecture doc by section.**
  No file cites a rule in a checklist by number, position, or nickname — quote the rule's
  words instead. A quote states the reason the code is shaped this way and survives any
  rewording; a position points at whatever sits there today.
- **Name the API a rule is about; never name an example of it.** A rule governing specific
  functions or files must name them or it cannot be checked — that name is the criterion. An
  illustrative aside has no such excuse: nothing keeps it in sync, and a stale example is
  worse than none.
- **A ban names its replacement.** "Never X" is half a rule; "— do Y instead" completes it
  for a few words.
- **One sentence of WHY is allowed where it makes the criterion decidable; anything longer
  belongs in the architecture doc.** No history, no PR numbers, no direction of travel;
  planned work lives in the folder's follow-up ledger (a checklist that keeps one elsewhere
  names it in its opening).
- **The placement block, when a checklist carries one, closes the file**: one bold criterion
  ("Placement — one file, one line: a diff keeps each file inside its line, and a new file
  adds its line here, with its tests, in the same change.") over `` `file` `` — role lines,
  optionally a bare never-phrase ("Zero network"). A clause with its own defect-verdict or
  same-change obligation is a rule, and it lives in the flat list above.

**Adding a rule starts with reading the whole checklist.** A rule appended by an author who
has not just read the file is how checklists rot — duplication, drift, and homeless placement
all start there. And when the rule you just wrote is the longest paragraph in the file, stop:
it is almost certainly several rules, an exception in disguise, or an essay.
