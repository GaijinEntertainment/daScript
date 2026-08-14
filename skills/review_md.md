# REVIEW.md — folder-scoped review rules

Read this before creating any `REVIEW.md`, editing one, or reviewing a diff that changes
one. A `REVIEW.md` is a folder-scoped review checklist: when a PR touches files under its
folder, the review applies every entry (`skills/make_pr.md` step 0a discovers these after the
master rebase — placing the file IS the registration).

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
`` Planned work: `<ledger>`. `` to the same line. An opening that deviates from this block —
or a checklist that restates contract text instead of pointing — is a self-review finding,
fixed like any other. An external repo that adopts `REVIEW.md` vendors a copy of
`REVIEW_COMMON.md` at its root, so the pointer resolves there too.

`<ARCH-DOC>` is the module's own design document — `ARCHITECTURE.md` (dasLLAMA),
`MASTERPLAN.md` (dasMetal), or the module's `CLAUDE.md` when no separate design doc exists.
Name it concretely; a module whose rules keep wanting a rationale home probably needs to
grow one.

## Why "no numbers" is load-bearing

A numbered entry gets cited — from other modules, from commit messages, from other rule
files — and the citation outlives the numbering. (dasMetal once cited "dasLLAMA REVIEW
#21"; dasLLAMA's entries are unnumbered, so the citation pointed at nothing.) Cross-module
references restate the criterion in place and cite the other module's architecture doc by
section for the reason.

## Writing the entries

- Lead each entry with the **bold testable claim**, tail with the consequence ("… is a
  defect") where it sharpens the check.
- Group under a few `##` sections when the list grows (dasLLAMA uses Tests / Placement /
  memory model / Documentation / Implementation) — sections are organization, not structure
  to fill in. A new module starts with a handful of flat entries.
- A per-file placement list ("one file, one rule") is the strongest section a big module can
  carry: each line names a file and what it holds, and any code written elsewhere is a
  defect. Keep each line to the file's own contents.
- A module's `## Tests` section adds specifics the constitutional branch-test clause can't
  carry — where tests live, which suite runs for which change, platform gates. It may
  sharpen the clause (a function-granular form, a bug-fix-regression form); it never
  weakens it, and it never restates it.
- The rule for rule-changes is the same as everywhere else in this repo: replace stale text
  outright, no dated entries, no "as of" markers.

## Followability — the slimming dimension

A checklist is applied under time pressure by someone holding a diff. Every audit round
judges the file against that reader — not only each rule in isolation (the self-review
rule), but whether the file as a whole can still be applied in one pass. These are the
finding classes; each is reported like any other finding, and its fix lands in the same
batch:

- **Dense multi-clause rule** — one paragraph carrying several independent checks, so the
  reviewer must re-read it to know what to verify. Split it: one rule, one check.
- **Enumeration standing in for a criterion** — a list of named cases or carve-outs where
  one property unites them. State the property; names go stale, the property doesn't.
- **Mechanism prose exceeding the rule** — more words on how the code works than on what to
  check. The mechanism moves to `<ARCH-DOC>`; the rule keeps its one sentence of WHY.
- **Structurally homeless rule** — a rule whose trigger is a change *outside* this folder.
  Step 0a discovers a checklist only through changed files under its own directory, so a
  diff elsewhere can never surface the rule — it will never fire. Move it to a checklist
  the trigger's folder walk reaches, or to the skill/CLAUDE.md that governs that code.
- **Overlapping rules** — two entries whose criteria make the reviewer check the same thing
  twice. Merge into the sharper one.

Size is a symptom, not a criterion: a file where every rule survives these classes is as
long as its module needs. But "a reviewer reports they can no longer follow the file" is
itself a finding, and its fix is applying the classes above — never a table of contents.

## Reviewing a REVIEW.md diff

Check the change against `REVIEW_COMMON.md`: is every new entry diff-checkable in
isolation? one paragraph? unnumbered? example-free? at most one sentence of WHY, and only
where it makes the criterion decidable? Does a moved rationale actually land in the
architecture doc, or did it just get deleted? Then apply the self-review rule literally
— an entry a reviewer cannot apply as written is itself a finding — and run the
followability classes above over the touched sections. The canonical conforming file is
`modules/dasLLAMA/REVIEW.md`.
