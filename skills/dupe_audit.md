# Dupe audit - one idea, one implementation

Read this before auditing a diff or a file set for duplicated or parameterizable code, and
before claiming that a helper you are about to write is new. The procedure is the same for a
diff and for a whole folder; only the scope differs.

## The rule

**One idea has one implementation.** A change that adds a function whose job an existing
function already does is a defect. Two or more functions whose bodies differ only on an axis -
a type, a constant, a shape, a format tag, a called helper - are one parameterized
implementation written N times; the set is a concern, and the report names the fold - the
single parameterized form the set collapses into.

The structural sweep (detect-dupe) is one tool in the audit's kit. It orders the reading; it
never replaces it. A function it does not pair can still be a duplicate, and a pair it reports
can be noise. The audit is the reading of every function in scope.

## Two tiers

- **DUPLICATE** - a defect. An existing function in `daslib/`, a module's `daslib/`, `utils/`,
  or the same change already does the job. The fix is reuse; when the existing function cannot
  serve, the change says why, and then the existing function is what needs fixing.
- **TEMPLATABLE** - a concern. Two or more siblings, in scope or across scope and the tree,
  differ on one or two axes. The fold is discussed before it is applied; the report carries
  what a decision needs: the members, the axis, the mechanism, the caller count.

## What counts

- Same job under a different name: `is_ws`, `is_space`, `iswhitespace`.
- Same body modulo one axis: a constant (64 vs 128), a type (f16 vs f32), a format (q4 vs
  q8), a shape (single vs batch), a called helper (the helper is the axis).
- Same control flow with the divergent lines in the same places - the twin pattern.
- A copy-pasted block of six or more lines inside two functions - a LOCAL finding; the fold
  is a helper, not a template.
- A third copy added where two already existed: the new copy is the finding, the old pair is
  its context.

## What does not count

- Structural-only similarity: visitor methods, dispatch lists, test wrappers, emitter shells.
  The sweep filters these by default; a reading that pairs them is wrong for the same reason.
- Siblings the folder's `ARCHITECTURE*.md` rules apart. Report them as SEPARATE BY RULING with
  the section named. When the reading disagrees with the ruling, say so in one line - the
  ruling can be wrong - but the set is not a finding against the change.
- A measured performance fork: a sibling that exists because the merged form was measured
  slower, with the measurement in the tree (a ledger row, a `[tune_perm]` grid, a benchmark).
  "For performance" without a measurement is not a reason; the set is TEMPLATABLE.
- Two functions that share a shape and differ in what they compute. Different arithmetic on
  the same skeleton is two ideas.

## The procedure

1. **Read the folder's `ARCHITECTURE*.md` and `REVIEW*.md` first.** They name the folding
   mechanisms the folder sanctions and the separations it has ruled. A fold the checklist
   already demands is reported in the checklist's words.
2. **Run the structural sweep** over the scope against the corpus (commands below). Walk exact
   clusters first, then fuzzy matches from the highest similarity down. Every pair is a
   candidate, nothing more.
3. **Read every function in scope, in full.** Build an inventory as you go: one line per
   function - `file:line name - job in ten words - axis-like parameters`. The inventory is
   what finds the duplicates the sweep cannot: the same job with a different skeleton.
4. **For each candidate, fresh-read both bodies.** Name the difference in words. Name the
   axis. Check the architecture doc for a ruling. Count the callers of each member (`grep_usage`
   or a repo-wide grep) - a fold that touches thirty call sites is still a fold, and the number
   is what the decision needs.
5. **A DUPLICATE claim names the existing function** with file:line and states what the two
   bodies differ on - "nothing", or the one line. "An existing helper probably does this" is
   not a finding.
6. **A claim of absence carries its evidence.** "No existing helper" comes with the search
   command, run from the repo root over the whole tree (generated directories excluded), and
   what it returned.

## Folding mechanisms

| The axis | The fold |
|---|---|
| a type | a generic `def f(a)` / `auto(TT)` with contracts; `static_if typeinfo(...)` for the one line that differs |
| a constant or shape the caller knows | a parameter, or a struct of constants passed once |
| a constant a GPU kernel must bake in | a `@template_constant` on a `class template` stamp, `static_if` on it in the body |
| a body divergence on one compile-time choice | a `class template` with `def abstract` / `def override`, spliced flat at emission |
| a helper chosen at run time | a lambda or function-pointer parameter, or a `variant`; in a kernel never a pointer - stamp it |
| a syntactic shape - the siblings differ in what they name, not in what they do | a reification macro (`qmacro`, `apply_template` from `daslib/templates_boost`) or an annotation macro that generates the set |
| performance permutations of one reference body | a `[tune_perm]` grid over the reference `def` (`skills/tune.md`) |
| N values | a table or array literal walked by one loop |

Name the mechanism in one line; the merged signature is the author's to write.

## Reporting

Per finding, in this order:

```
DUPLICATE | TEMPLATABLE | LOCAL | SEPARATE BY RULING
MEMBERS:  file:line name           (one per line)
AXIS:     what varies, in words
EXISTING: file:line name           (DUPLICATE only - the function that already does the job)
FOLD:     the mechanism, one line
CALLERS:  N sites for each member
RULING:   ARCHITECTURE_X.md section   (SEPARATE BY RULING only)
```

Then the summary line: `N functions read: D duplicates, T templatable sets (M members),
L local blocks, R ruled separate`. Report DUPLICATE and TEMPLATABLE in full; list the rest by
count with one line each. A clean audit names the inventory it built and the corpus it swept;
"nothing duplicated" without either is not an audit.

## The structural sweep - commands

The engine is `utils/detect-dupe/main.das`; the corpus is a JSON of every function's token
stream with names and literals normalized. Build it once per audit over the bodies of code the scope is compared against,
then query the scope against it. Excluding tests from the corpus keeps fixture shapes out of
the pairs; excluding the scope's own files from a directory corpus is automatic in the
`--against` form.

```sh
# corpus: the stdlib, every module's daslib, the in-tree tools
bin/daslang utils/detect-dupe/main.das -- -p daslib -p utils -p modules --export-functions corpus.json

# the scope against it - per-candidate exact and fuzzy matches
bin/daslang utils/detect-dupe/main.das -- --import-functions corpus.json --against <file-or-dir> --json report.json

# a diff's files, from git
git diff --name-only <base>..<head> | grep '\.das$' | \
    bin/daslang utils/detect-dupe/main.das -- --import-functions corpus.json --against-from-stdin --json report.json
```

`report.json` carries `candidates[]`, each with `exact_matches` (similarity 1.0) and
`fuzzy_matches` sorted by similarity. A file that fails to compile is reported `FAIL` and
still scanned from the AST the failed compile left; a `missing prerequisite` module is skipped
loudly. Pattern-filtered boilerplate is dropped by default; `--keep all` shows it when a
candidate count reads zero.

The MCP tools `export_corpus` / `detect_duplicates` wrap the same two steps.

## Where this runs in the daslang repo (repo-only)

- The per-PR audit is the `dupe-auditor` agent (`.claude/agents/dupe-auditor.md`): one
  instance over the whole diff, launched in `skills/internal/make_pr.md` step 0a beside the
  `tdd-auditor`, and in the surfacing phase of `skills/internal/review_round.md`. Its DUPLICATE
  verdicts are defects; its TEMPLATABLE verdicts are concerns discussed before any fix.
- The standalone sweep is the `dupe-sweeper` agent (`.claude/agents/dupe-sweeper.md`): the
  orchestrator hands it a file set; a set past roughly six thousand lines is sharded - one
  instance per shard reading in full and returning its inventory, then one merge instance over
  the inventories and the sweep report for the cross-shard sets.
- The sweep's flags, modes, and pattern filter: `skills/internal/detect_dupe.md` and
  `skills/internal/detect_dupe_reference.md`. The Claude-judged triage (`find_dupe`) is not part
  of this audit - the auditor is the judge.
- A flag or behavior change in `utils/detect-dupe/main.das` updates the command block above,
  `skills/internal/detect_dupe.md`, and `skills/internal/detect_dupe_reference.md` in the same
  change.
