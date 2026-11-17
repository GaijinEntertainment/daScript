# dasLLAMA tests - the runs a PR cites

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture doc:
`../ARCHITECTURE.md`. Planned work: `../followup_general.md`. The parent checklist is `REVIEW.md`
(beside this file); the runner's tables are `run.das`.

A PR's kind is the line its body's Validation section opens with: `Kind: final PR of the <arc> arc.`
makes it a final PR; a PR opening `Kind: intermediate PR of the <arc> arc.`, or carrying neither
line, is an intermediate PR. A final PR's change is the union of the diffs of its arc's PRs, its own
included; its body names those PRs and the commit the arc's first PR branched from. Its `--changed`
run takes `--base` at that commit, and the run plans a superset of the change. A scope report is the
RUN and NOT RUN lists a `targeted-reviewer` agent returns for an intermediate PR's diff.

**Every PR runs `run.das -- --suite model-free`.**

**A final PR runs `run.das -- --changed --base <the commit its body names>` on a box holding the
stocked artifacts of every file its `PLAN` line counts, never a whole-directory run instead.**

**An intermediate PR's body carries its scope report: each RUN entry's command with that run's
`REPORT` line - for a command other than `run.das`, the output line that states its verdict (a
compare's match, the rig's WER and UTMOS) - and each NOT RUN entry with its reason.**

**On a final PR, a test file the change reaches that the `--changed` run does not plan, or whose
reached cells it skips, runs on its own where those cells do not skip, and the PR body cites that
run.**

**A final PR whose change reaches a cell that skips on, or picks its tolerance or its code path by,
a hardware or build capability the box may lack - a device, a module the build may omit, a kernel
the instruction sets of the box's CPU do not carry - runs that cell on a box that has it, and names
that box in the PR body.** A reached cell run without the capability did not run what the capability
selects.

**A final PR that adds a cell loading a model above the large tier (`MODEL_TIER_CEILING_BYTES`,
`../dasllama/dasllama_gguf.das`), or whose change reaches such a cell, runs that cell under
`DASLLAMA_PARITY_FULL=1` on a box with the model stocked - in a `run.das -- --changed` run whose
`PLAN` holds the cell's file, or in a `run.das --suite` run listing the file, with `--arm` naming
the cell where that suite takes `--arm` - and names the box in the PR body.** A run without
`DASLLAMA_PARITY_FULL=1` skips every such cell and passes.

**The `--changed` run a final PR cites carries no `--exclude` and no `--only`.** A trimmed run never
ran the coverage it dropped; a trimmed run is an intermediate PR's form.

**Every `--changed --only` run a final PR cites runs under `DASLLAMA_GPU=1 DASLLAMA_CUDA=1` beside
the whole `--changed` run, never in its place, and a final PR citing any such run cites among them
one whose tokens match every file of `run.das`'s `CUDA_WITNESSES`.**

**A final PR whose change reaches a test file holding a cell that runs on CUDA under
`DASLLAMA_CUDA=1` cites a run under `DASLLAMA_GPU=1 DASLLAMA_CUDA=1` whose `ONLY` lines name that
file.**

**A final PR cites every shard of a sharded run (`--shard i/n`) with its `REPORT` line and its box,
and a `--suite stocked` run with no `--exclude` or `--only`.** A shard is a fraction of the run's
files; the `--changed` run holds one stocked test a family and backend, the stocked suite every one.
