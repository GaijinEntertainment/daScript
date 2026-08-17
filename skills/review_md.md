# REVIEW.md — folder-scoped review rules

Read this before creating any `REVIEW.md`, editing one, or reviewing a diff that changes
one. A `REVIEW.md` is a folder-scoped review checklist: when a PR touches files under its
folder, the review applies every entry (discovery is a folder walk over the changed set —
placing the file IS the registration).

## The contract — one file, shared

The rules every checklist lives under — what a checklist may contain (diff-checkable
criteria only), the self-review rule, the branch-test rule, and the Form hard limits —
live ONCE, in `REVIEW_COMMON.md` at the repo root. A checklist does not restate them;
its opening is this block, verbatim except for the name and the two facts it declares:

```markdown
# <Module> Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first — its contract binds this checklist.** Architecture doc:
`<ARCH-DOC>`.
```

A checklist that keeps its follow-up ledger somewhere non-obvious appends
`` Planned work: `<ledger>`. `` to the same line. A checklist with routed companions — a
shared concern file, subfolder checklists, a skill reviewing a construct family — carries
their routing lines in or right after the opening, each keyed by KIND ("a `[test]` file,
wherever the diff puts it, answers to the `tests/` subfolder's checklist"), because the
folder walk finds only files named `REVIEW.md` — a companion is reachable solely through its
routing line. An opening that deviates from this block —
or a checklist that restates contract text instead of pointing — is a self-review finding,
fixed like any other. An external repo that adopts `REVIEW.md` vendors a copy of
`REVIEW_COMMON.md` at its root, so the pointer resolves there too.

`<ARCH-DOC>` is the module's own design document — `ARCHITECTURE.md` (dasLLAMA),
`MASTERPLAN.md` (dasMetal), or the module's `CLAUDE.md` when no separate design doc exists.
Name it concretely; a module whose rules keep wanting a rationale home probably needs to
grow one.

## Why "no numbers" is load-bearing

A numbered entry gets cited — from other modules, from commit messages, from other rule
files — and the citation outlives the numbering. A cross-module reference restates the
criterion in place and cites nothing; coupling between rule documents is routing, never
citation.

## Writing the entries

- Lead each entry with the **bold testable claim**, tail with the consequence ("… is a
  defect") where it sharpens the check.
- The list is flat — no section headers, blank line between entries. A grouping that keeps
  wanting a header is a separable concern: give it its own file and route to it from the
  opening (`modules/REVIEW_SHADER_EMITTERS.md` and `modules/dasLLAMA/REVIEW_GPU.md` are the
  pattern) — or its rules are subfolder-scoped and move to that subfolder's own `REVIEW.md`.
- The placement block, when the module carries one, closes the file in the fixed form
  `REVIEW_COMMON.md` states: one bold criterion over `` `file` `` — role lines with at most
  a bare never-phrase. A smuggled rule moves to the flat list; the `placement-auditor` agent
  checks the map against the territory.
- Test rules add the specifics the constitutional branch-test clause can't carry — where
  tests live, which suite runs for which change, platform gates. They may sharpen the
  clause; they never weaken it, and never restate it.
- The rule for rule-changes is the same as everywhere else in this repo: replace stale text
  outright, no dated entries, no "as of" markers.

## Followability — the slimming dimension

A checklist is applied under time pressure by someone holding a diff. Every audit round
judges the file against that reader — not only each rule in isolation (the self-review
rule), but whether the file as a whole can still be applied in one pass. These are the
finding classes; each is reported like any other finding, and the damper below decides
which of them the round edits:

- **Dense multi-clause rule** — one paragraph carrying several independent checks, so the
  reviewer must re-read it to know what to verify. Split it: one rule, one check.
- **Enumeration standing in for a criterion** — a list of named cases or carve-outs where
  one property unites them. State the property; names go stale, the property doesn't.
- **Mechanism prose exceeding the rule** — more words on how the code works than on what to
  check. The mechanism moves to `<ARCH-DOC>`; the rule keeps its one sentence of WHY.
- **Structurally homeless rule** — a rule whose trigger is a change *outside* this folder.
  Checklist discovery walks the parent directories of the changed files, so a diff
  elsewhere can never surface the rule — it will never fire. Move it to the checklist
  of the narrowest folder containing its trigger (creating that subfolder's `REVIEW.md` is
  never an objection), or to the skill/CLAUDE.md that governs that code.
- **Overlapping rules** — two entries whose criteria make the reviewer check the same thing
  twice. Merge into the sharper one.

Size is a symptom, not a criterion: a file where every rule survives these classes is as
long as its module needs. But a reviewer who cannot apply the file to the diff in front of
them has a verdict-blocking finding — cited like any other; its fix is applying the classes
above, never a table of contents.

## Accepting self-review findings — the damper

The standard for rule text is GOOD ENOUGH, not clean: text, like code, can be exercised for
correctness endlessly, so stop when nothing serious remains — the marginal edit costs a
dragon round and buys polish, not verdicts. A round opens a checklist for edit only when it
holds a finding of one of these classes:

1. **Verdict-blocking on this diff** — the defect prevented or flipped a verdict while
   auditing the change under review; the auditor cites the code site where rule application
   failed, or the finding is not accepted.
2. **A defect in a rule the diff itself adds or edits.**
3. **Serious latent** — a factually stale claim (names, lists, APIs no longer matching the
   tree), a contradiction between two rules, or a fused rule hiding a second obligation.
   The test for serious: acting on it changes what a reviewer CHECKS or CONCLUDES; a change
   that only improves the reading is minor.

Minor findings ride along only in a file already open for the reasons above — never the
sole reason to touch a checklist — and are otherwise dropped, not ledgered: a latent defect
that matters returns as class 1 with a citation. Disposition: FIX NOW when the repair is
forced (one reasonable edit — staleness is the canonical case); ASK the user when the
repair is a semantic choice (which side of a contradiction wins, a rule deleted because a
test enforces it, a scope reassignment). Auditors tag each self-review finding
`blocking | stale | contradiction | structural | formatting`; the tag locates the defect,
and the forced-vs-semantic test on its REPAIR decides the disposition. One checklist-edit batch per round, one dragon pass over the batch; a FRESH
dragon then re-reads cold. Serious findings it returns open one more batch on the same
terms; the round exits when a fresh cold read returns none. Wording a dragon itself authored
and the round applied verbatim is exempt from the next pass — never run dragon on dragon;
re-judging its own prose is oscillation by construction. A document a round has ruled on is
not re-dragoned in later rounds unless a serious rule change touches it — a ruling stands.

## Reviewing a REVIEW.md diff

Check the change against `REVIEW_COMMON.md`: is every new entry diff-checkable in
isolation? one paragraph? unnumbered? exception-free? example-free? at most one sentence of
WHY, and only where it makes the criterion decidable? Does a moved rationale actually land
in the architecture doc, or did it just get deleted? Then apply the self-review rule
literally — an entry a reviewer cannot apply as written is itself a finding — and run the
followability classes above over the touched rules. The `dragon` agent runs this audit on
any modified rule document; the `placement-auditor` agent audits a placement block against
its folder. The canonical conforming set is `modules/dasLLAMA/REVIEW.md` and its routed
companions.
