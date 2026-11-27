# dasLLAMA tests - PR Runs Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture doc:
`../ARCHITECTURE.md`. Planned work: `../followup_general.md`.

A PR's kind is the first line of its body's Validation section: `Kind: final PR of the <arc> arc.`
makes a final PR; any other first line, or a body with no Validation section, makes an intermediate
PR. A final PR's change is the union of its arc's PRs' diffs, its own included. A final PR's body
names its arc's PRs and the commit the arc's first PR branched from. Its `--changed` run takes
`--base` at that commit, and the run plans a superset of the change. A scope report is the RUN and
NOT RUN lists the `scope-auditor` agent returns for an intermediate PR's diff. A change reaches a
test file or a cell as `REVIEW.md` (beside this file) defines it.

**A PR whose change reaches a test file or a cell runs `run.das -- --suite model-free`.**

**A PR quotes each `run.das` run it cites by its whole `REPORT` line, the count of files not run
included, and a cited run passes no `--rerun`.** A file the run reports from an earlier pass counts
as run; `--rerun` runs every file and records no pass.

**A final PR runs `run.das -- --changed --base <the commit its body names>` on a box holding the
stocked artifacts of every file its `PLAN` line counts, never a whole-directory run instead.**

**An intermediate PR's body carries its scope report: each RUN entry's command with that run's
`REPORT` line - for a command other than `run.das`, the output line that states its verdict (a
compare's match, the rig's WER and UTMOS) - and each NOT RUN entry with its reason.**

**On a final PR, a test file the change reaches that the `--changed` run does not plan, or whose
reached cells it skips, runs on its own where those cells do not skip, and the PR body cites that
run.**

**A final PR whose change reaches a cell that skips on, or picks its tolerance or its code path by,
a hardware or build capability one native box has and another lacks - a device, a module the build
may omit, a kernel the instruction sets of the box's CPU do not carry - runs that cell on a box that
has it, and names that box in the PR body.** A reached cell run without the capability did not run
what the capability selects.

**A final PR whose change reaches a cell that skips on, or picks its tolerance or its code path by,
a capability no native box has and a wasm build does - a kernel only the wasm64 release seats - runs
that cell in `run.das -- --suite wasm`, and cites that run's `REPORT` line in the PR body.** A
native run skips such a cell, or runs its other path, on every box.

**A PR whose change reaches a cell that loads a model file a category holds runs `run.das --
--category <name>` for that category on a box holding its files, and cites that run's `REPORT` line
and the box in the PR body; a final PR names each category it did not run, with the reason.** No
other run opens a category's files, the full suite's included.

**A final PR whose change reaches a cell that runs only in the full suite (`parity_full_mode`) runs
that cell under `DASLLAMA_PARITY_FULL=1` - in a `run.das -- --changed` run whose `PLAN` holds the
cell's file, or in a `run.das --suite` or `--area` run listing the file, with `--arm` naming the
cell where that run takes `--arm` - and names the box in the PR body.** A run without
`DASLLAMA_PARITY_FULL=1` skips every such cell and passes.

**The `--changed` run a final PR cites carries no `--exclude` and no `--only`.** A trimmed run never
ran the coverage it dropped; a trimmed run is an intermediate PR's form.

**Every `--changed --only` run a final PR cites runs under `DASLLAMA_GPU=1 DASLLAMA_CUDA=1` beside
the whole `--changed` run, never in its place, and a final PR citing any such run cites among them
one whose tokens match every file of `run.das`'s `CUDA_WITNESSES`.**

**A final PR whose change reaches a test file holding a cell that runs on CUDA under
`DASLLAMA_CUDA=1` cites a run under `DASLLAMA_GPU=1 DASLLAMA_CUDA=1` whose `ONLY` lines name that
file.**

**A final PR cites a `--suite stocked` run with no `--exclude` or `--only`.** The `--changed` run
holds one stocked test a family and backend, the stocked suite every one.
