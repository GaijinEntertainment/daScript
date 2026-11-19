# dasLLAMA testing by PR kind (repo-only)

Read this before planning the test runs of a dasLLAMA pull request. It says how much a PR
tests; `modules/dasLLAMA/tests/CLAUDE.md` says how the runner works, and
`skills/internal/make_pr.md` the gates every PR in the repo passes.

## Two kinds of PR

- **Intermediate** - the default: a PR inside an arc, or one outside any arc. It tests
  what its change can realistically break, and nothing else.
- **Final** - the PR that closes an arc. It runs everything, on the change of the arc's PRs
  together.

The PR body's Validation section opens with the kind: `Kind: intermediate PR of the <arc>
arc.` (`Kind: intermediate PR, no arc.` outside one) or `Kind: final PR of the <arc> arc.`
A final PR's body also names the arc's PRs and the commit the first one branched from.

The repo-wide gates do not depend on the kind: `make-pr` and the one full preflight run on
every PR.

## Intermediate PR

Runs:

1. `./bin/daslang -jit modules/dasLLAMA/tests/run.das -- --suite model-free` - it needs no
   models and runs the same on any box.
2. The runs the scope report names (next section), and no others.

Not owed: the whole stocked suite, a fleet run, the large tier (`DASLLAMA_PARITY_FULL=1`)
beyond a cell the scope names, a GPU home the change does not touch, a re-tune of the box,
and any measurement (see Profiling).

The body's Validation carries the scope report: each RUN entry's command with that run's
`REPORT` line, and each NOT RUN entry with its reason.

### The scope report

One `scope-auditor` agent (`.claude/agents/scope-auditor.md`) decides the scope before any
model-loading run starts. Give it the PR's range (`origin/master..HEAD`), the boxes at hand, and
the runs already made on the branch. It runs nothing and returns three lists: RUN (the exact
command, what it covers, the reason), NOT RUN (the area or suite, the reason it cannot
realistically break) and RISK (what the NOT RUN list would catch if a reason is wrong).

The list is short by design. An intermediate PR buys iteration speed, and the arc's final PR
runs everything: a run is on the RUN list only when the diff changes what that run observes
and no run already made covers it, and a doubt is a NOT RUN entry with its risk named, never
a run added to be safe. A run made earlier on the branch stands unless a later commit changes
what it observes.

The agent answers by SUBJECT, not by reach (what `run.das --changed` would plan from the
changed paths): it names what each changed function is for, lists the test files and suite
arms that exercise that purpose, and applies these rules.

1. A change is tested where its subject is served. Code that only a TTS load reaches is
   tested by the TTS files, whatever module holds it.
2. A change to a shared format or reader (the image layout, the GGUF reader, a quant
   transcode) runs that format's own cells - for the image, the `image` suite's
   `mechanics` arm - and one test file for each branch of the reader or writer the diff's
   hunks edit (an image lane - planar, metal, vulkan, cuda, a tower flavor - or a format's
   case); a branch the hunks do not edit runs nothing.
3. A layer that reaches the change only through a layer whose tests run is covered by
   those tests: a server or tool test is in scope only when the diff changes that server
   or tool, or a fixture it reads - and then every reader of the fixture runs, the `.das`
   tests and the browser specs (`utils/dasllama-server/tests/*.spec.js`) alike.
4. A refactor that claims no behavior change is proved by comparing outputs before and
   after on the files it touches - bytes of a written file, samples of a synthesis, token
   ids of a walk - and that compare replaces the suite runs of the refactored paths. Rule
   5 still runs the test files the refactor edits.
5. Each cell the diff adds or edits runs where it does not skip, selected by the arm that
   reaches it; a cell with no arm runs its file through the runner (`--area <a> --only
   <file stem>`), and only a model-free file's cell may run alone, as `./bin/daslang -jit
   dastest/dastest.das -- --test <file> --test-names <prefix>`; an unchanged cell of a
   changed file runs only when another rule puts it in scope.
6. A GPU home runs only when the diff changes that home's driver or kernels, or the bytes
   of the planes it uploads; a header or container change that leaves every plane's bytes
   alone does not.
7. A quality run (`modules/dasLLAMA/harness/tts_rig.py` for speech) is in scope only
   where the diff changes what a flag-free run synthesizes: each model file it mints or
   re-mints, on the lane it mints; each weight lane whose kernels or mint it changes, on
   one model of each family that takes the lane; and, where it does neither, one model
   the change reaches at its unpinned default. A re-mint the diff forces without changing
   a weight - an image version bump, a header field - is proved by the image suite's byte
   compares, not the rig; and where the output must not move, REVIEW_TTS's PCM-hash
   compare stands in for the rig.

`run.das -- --changed` plans by reach, so a core-module change plans every area. On an
intermediate PR narrow it with `--changed --only <substr,...>` (the runner names each kept
file on an `ONLY` line), or run the RUN list's areas alone with `--area <a,b>` in place of
`--changed`; a RUN entry in a model suite runs as `--suite <name> --arm <arms>`.

A reason in the NOT RUN list that turns out wrong - a later red traced to a skipped area -
is fixed in the PR that finds it, and the brief's rule that let it through is sharpened
here.

## Profiling

An intermediate PR owes no number. It writes no board row, no site record, and no card
for the site.

- Measured anyway? The figures go in the PR body or a dated `PERF_LEDGER.md` entry, under
  `modules/dasLLAMA/REVIEW_MEASUREMENT.md`'s naming rules - never a board row, a site
  record or a card.
- Made a cell change (REVIEW_MEASUREMENT's term) and measured nothing? The body says so in
  one line and names the `modules/dasLLAMA/followup_general.md` row under which the release
  re-mint re-measures the cell.
- Every other duty `modules/dasLLAMA/REVIEW_MEASUREMENT.md` puts on an intermediate PR
  still applies, the say-so lines included: a changed Metal kernel library,
  `profile_common.das` or a module `lcpp_bench.das` requires says no fat-exe run was made; a
  faster-route claim names its re-mint row; a newly reached route adds its spawned cell.

Quality rows - word error rate, UTMOS - are not profiling: the brief's quality-run rule
decides them, and the full sweep over every model and lane the arc reaches is the final
PR's.

## Final PR

The run rules of `modules/dasLLAMA/tests/REVIEW_RUNS.md` bind this PR in full: the
model-free suite, `run.das -- --changed --base <the commit the body names>` with no
`--exclude` or `--only`, the whole stocked suite over the fleet, the large tier, every
capability box and GPU home the arc's change reaches. Beyond them it runs:

- Every model suite - decode, mtp, prefill, matrix, kernels, image, image-vulkan, coverage -
  holding a file the arc changed or a cell the arc's change reaches, as `--suite <name>
  --arm <arms the change reaches>`; the `--changed` run's `CHANGED` lines name each changed
  suite file, and `suite_files` in `run.das` maps a file to its suite.
- The tool suites the arc reaches:
  `./bin/daslang -jit dastest/dastest.das -- --test utils/dasllama-server`, and the same
  with `utils/dasllama-convert`.
- Every figure the arc owes under `modules/dasLLAMA/REVIEW_MEASUREMENT.md`, on tuned
  kernels, from every board box of the author's.
- The speech rig's full sweep, where the arc changed what a model says
  (`modules/dasLLAMA/REVIEW_TTS.md`).
