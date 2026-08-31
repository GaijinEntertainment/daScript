# REVIEW.md - folder-scoped review rules

Read this before creating any `REVIEW.md`, editing one, or reviewing a diff that changes
one. A `REVIEW.md` is a folder-scoped review checklist: when a PR touches files under its
folder, the review applies every entry (discovery is a folder walk over the changed set -
placing the file IS the registration).

## The contract - one file, shared

The rules every checklist lives under - the whole shared contract - live ONCE, in
`REVIEW_COMMON.md` at the repo root. A checklist does not restate them; its opening is this
block, verbatim except for the module name and the architecture-doc path:

```markdown
# <Module> Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture doc:
`<ARCH-DOC>`.
```

A checklist whose folder has a follow-up ledger appends `` Planned work: `<ledger>`. `` to
the same line. A checklist whose folder's architecture doc has split into companions writes
the plural - `` Architecture docs: `<A>`, `<B>`. `` - listing every doc its rules cite. A checklist with routed companions - a shared concern file, subfolder
checklists, a skill reviewing a construct family - carries their routing lines in or right
after the opening, each keyed by KIND ("a `[test]` file, wherever the diff puts it, answers to
the `tests/` subfolder's checklist"), because the folder walk finds only files named
`REVIEW.md` - a companion is reachable solely through its routing line. An opening that
deviates from this block - or a checklist that restates contract text instead of pointing - is
a self-review finding, fixed like any other.

`<ARCH-DOC>` is the module's own design document - its `ARCHITECTURE.md` when it has one,
otherwise its `CLAUDE.md`; a `README.md` that carries the module's charter and mechanisms
fills the slot too (`site/README.md` is the ruled precedent). Name it concretely; a module
with no rationale home needs one before its rules can cite a reason.

## The executable half - REVIEW.das

The gate contract - what a `REVIEW.das` is, its output protocol, fail-fix, the orphan rule,
and when a checklist rule is deleted in its favor - lives in `REVIEW_COMMON.md`; this section
carries only the authoring craft. A rule is gate material when it is mechanically decidable
from tree state alone: a file exists, a name matches a pattern, list A equals list B, nothing
under X references Y. The support library's API is documented in `dastest/README.md`. A gate
ends by returning `gate_verdict(<name>)` - the support call that prints the accumulated
findings and sets the exit code - so no runner is needed to hand-run one.

A worked example is `utils/REVIEW.das`. (repo-only)

The walkers that execute gates are repo tooling: the per-diff walk (gates before agent
review) is the REVIEW audit row of `skills/internal/make_pr.md`; the tree-wide walk is
`utils/internal/review-md/all.das`, the `extended_checks` "Run REVIEW.das gates" CI step.
(repo-only)

## Writing the entries

- A separable concern gets its own file, routed to from the opening -
  `modules/REVIEW_SHADER_EMITTERS.md` and `modules/dasLLAMA/REVIEW_GPU.md` are the pattern.
- The `placement-auditor` agent checks a placement block's file list against the folder's
  actual contents.
- Test rules add the specifics `REVIEW_COMMON.md`'s "New functionality ships with tests"
  clause can't carry - where tests live, which suite runs for which change, platform gates.
  They may sharpen it; they never weaken it, and never restate it.
- A rule change replaces stale text outright - no dated entries, no "as of" markers.

## Followability - the slimming dimension

A checklist is applied under time pressure by someone holding a diff. Every audit round
judges the file against that reader - not only each rule in isolation (the self-review
rule), but whether the file as a whole can still be applied in one pass. These are the
finding classes; each is reported like any other finding, and the damper below decides
which of them the round edits:

- **Dense multi-clause rule** - one paragraph carrying several independent checks, so the
  reviewer must re-read it to know what to verify. Split it: one rule, one check.
- **Enumeration standing in for a criterion** - a list of named cases or carve-outs where
  one property unites them. State the property; names go stale, the property doesn't.
- **Mechanism prose exceeding the rule** - more words on how the code works than on what to
  check. The mechanism moves to `<ARCH-DOC>`; the rule keeps its one sentence of WHY.
- **Structurally homeless rule** - a rule whose trigger is a change outside this folder, so
  the folder walk can never surface it. Move it to the narrowest folder containing its
  trigger, or to the skill or `CLAUDE.md` that governs that code.
- **Overlapping rules** - two entries whose criteria make the reviewer check the same thing
  twice. Merge into the sharper one.

Size is a symptom, not a criterion: a file where every rule survives these classes is as
long as its module needs. But a reviewer who cannot apply the file to the diff in front of
them has a verdict-blocking finding - cited like any other; its fix is applying the classes
above, never a table of contents.

## Accepting self-review findings - the damper

The standard for rule text is GOOD ENOUGH, not clean: text, like code, can be exercised for
correctness endlessly, so stop when nothing serious remains - the marginal edit costs a
dragon round and buys polish, not verdicts. A round opens a checklist for edit only when it
holds a finding of one of these classes:

- **Verdict-blocking on this diff** - the defect prevented or flipped a verdict while
  auditing the change under review; the auditor cites the code site where rule application
  failed, or the finding is not accepted.
- **In-diff rule defect** - a defect in a rule the diff itself adds or edits.
- **Serious latent** - a factually stale claim (names, lists, APIs no longer matching the
  tree), a contradiction between two rules, or a fused rule hiding a second obligation.
  The test for serious: acting on it changes what a reviewer CHECKS or CONCLUDES; a change
  that only improves the reading is minor.

**Minor findings ride along only in a file already open for one of the classes above** -
never the sole reason to touch a checklist, and otherwise dropped, not ledgered: a latent
defect that matters returns verdict-blocking, with a citation.

**FIX NOW when the repair is forced** - one reasonable edit exists; staleness is the
canonical case. **ASK the user when the repair is a semantic choice** - which side of a
contradiction wins, a rule deleted because a test enforces it, a scope reassignment.

**Auditors tag each self-review finding `blocking | stale | contradiction | structural |
formatting`** - the tag locates the defect; the forced-vs-semantic test on the repair
decides the disposition.

One checklist-edit batch per round, one dragon pass over the batch; a FRESH dragon then
re-reads cold. Serious findings it returns open one more batch on the same terms; the round
exits when a fresh cold read returns none.

A dragon pass reads only the wording the round itself authored; wording a dragon authored and
the round applied verbatim is not part of the batch - re-judging its own prose oscillates. A
document a round has ruled on gets no further dragon pass in later rounds unless a serious
rule change touches it - a ruling stands.

## Reviewing a REVIEW.md diff

Check the change against `REVIEW_COMMON.md`: is every new entry diff-checkable in isolation?
one paragraph? unnumbered? exception-free? example-free? at most one sentence of WHY, and
only where it makes the criterion decidable? Does a moved rationale actually land in the
architecture doc, or did it just get deleted? Those are `REVIEW_COMMON.md`'s most-missed
questions, not its limits - that file is the law. Then apply the self-review rule literally,
and run the followability classes above over the touched rules. The `dragon` agent runs this
audit on any modified rule document; the `placement-auditor` agent audits a placement block
against its folder. The canonical conforming set is `modules/dasLLAMA/REVIEW.md` and its
routed companions.
