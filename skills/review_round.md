# The multi-agent review round (repo-only)

Read this before running a deep review of a branch or diff — the pre-PR review round of
`skills/make_pr.md`, or a standalone "review this" request. This is NOT `skills/codereview_md.md`
(authoring the per-folder checklists) and NOT the post-open Copilot loop (`skills/babysit.md`);
this is the round where agents read the change and findings are proven before anyone reads them.

The shape: **ground → derive dimensions → surface → prove → report**. Two principles carry it:
review dimensions are derived from THIS change's risks, never from generic categories; and every
finding is a hypothesis until an adversarial prover fails to disprove it. Surfacing optimizes
recall, proving optimizes precision — neither phase does the other's job.

## Diff range

`git merge-base master HEAD`..`HEAD`, plus the working tree when reviewing uncommitted work.
Always the WHOLE arc — never just the topmost commit of a multi-commit branch.

## Phase 0 — grounding (one agent)

Spawn ONE read-only agent over the full diff range. It produces a map, not findings:

- **Intent** — what the change is for, in one paragraph.
- **Integration surface** — every boundary the change touches: callers of changed functions,
  data formats read or written, caches keyed on changed inputs, module-graph edges added or
  removed, lifecycle/ownership handoffs.
- **Hotspots** — the places most likely to be wrong, each with WHY (a subtle invariant, an
  ordering dependency, a semantics change) and file:line.
- **Blind spots** — what the diff does NOT show: consumers in other directories, generated
  files, configs, platform arms the change can break without touching.
- **The CODEREVIEW.md binding set** — the `skills/make_pr.md` step-0a walk over the changed set.

Grounding is context, not authority: a later agent that finds the code disagreeing with the
grounding believes the code.

## Phase 1 — derive dimensions (orchestrator, no agent)

From the grounding, derive 3–6 surfacing dimensions. Each names a **specific risk this change
introduces**, in the change's own vocabulary — the dimension tells the surfacer WHERE to focus;
the surfacer decides WHAT to find.

- **Self-check, applied to every dimension:** if the name would still make sense pasted onto a
  different change, it is too generic — rewrite it using terms from the grounding.
  Too generic: "correctness review", "integration review". Right shape: "u16→u32 widening
  across the consumer call sites", "cache-key parity between the monolithic and split paths".
- **Not investigation questions either:** a dimension that names the exact code path and the
  expected defect ("X round-trips through Y — check the truncation") has already decided WHAT
  to find; that anchoring is how a surfacer misses the defect NEXT to the predicted one.

## Phase 2 — surfacing (parallel agents)

Spawn in ONE message, all read-only, model `opus`:

- **One surfacer per dimension.** Prompt = the dimension name + the grounding's hotspot notes
  for it (risk patterns, not methodology). Recall over precision: report every suspicion with
  location, concern, evidence, and a proposed severity — the prover will kill the false ones.
- **One general surfacer**, no prescribed dimension: "follow any thread; focus on behavioral
  regressions — semantic contracts, lifecycle guarantees, concurrency — and anything the
  dimension surfacers might miss."
- **One `codereview-md-auditor` instance per discovered CODEREVIEW.md** (the agent fans in,
  the orchestrator fans out — each instance owns exactly one checklist).

Findings come back as: `file:line`, the concern, the evidence (quoted code), severity
(`bug` / `concern` / `suggestion` / `nitpick`) and whether it should block.

## Phase 3 — proving (one agent)

First a mechanical dedupe by the orchestrator: same location + same concern from different
surfacers → keep the strongest-evidence copy. Then spawn ONE read-only prover (model `opus`)
over the survivors. Its stance: findings are hypotheses; the surfacer selected confirming
evidence and anchored on a conclusion; the prover's job is to find the reason each finding is
wrong, by its own reading — not by re-reading the surfacer's quotes. The more confident a
finding sounds, the deeper the investigation.

Gates, in order — failing any gate rejects the finding outright:

1. **Falsification attempt.** Name what would disprove the finding, then go check it ("if the
   guard at Y covers this path, the concern is wrong" → read Y). Disproof succeeds → reject.
   No formulatable disproof → the concern is unfalsifiable → reject.
2. **Observation, not assumption.** Every factual assertion must trace to the prover's own
   fresh read of a specific line — quoted. An assertion that cannot be traced is a guess →
   reject.
3. **Inference chain.** "A exists, B exists, therefore C" — verify the inference step itself,
   not just A and B. An unverified link demotes the finding to `suggestion`, phrased as a
   hypothesis.
4. **Pre-change baseline.** A regression claim needs the OLD behavior verified from the diff's
   removed lines / the pre-change code, in the specific path claimed — a parameter having
   existed is not proof it provided the behavior. No verified baseline → severity caps at
   `concern`, never `bug`, never blocking.

Post-gates on survivors: **scope** (a finding on unchanged code is valid only when this change
made it wrong — otherwise reject); **style** (style findings with no codebase-backed convention
behind them are personal preference — reject); **severity recalibration** per the gate results.

Auditor findings ride through differently: **VIOLATED** verdicts take gate 2 (fresh-read the
cited lines against the quoted rule); **UNPROVEN** verdicts bypass the gates — they are
requests for evidence (a suite run, a platform check, a stated claim), not hypotheses, and
reach the report as their own section; **SELF-REVIEW** findings pass through as checklist
defects.

## Phase 4 — report, then fix

The report, in the conversation (not a file): confirmed findings ranked most-severe first,
each with `file:line`, the evidence, and one line of gate trail (what was checked that failed
to disprove it); then the UNPROVEN evidence-requests; then rejected-count by gate so coverage
is visible. Zero confirmed findings is a reportable result — say what was checked.

Fix policy is the repo's standing one, not this skill's: findings are DISCUSSED before fixing
(fix-vs-workaround is the user's call), accepted fixes land as ONE batch per round, and any
finding that reveals a lint-able pattern is named as a lint candidate in the summary.

## Mechanics

- Every agent is read-only — no Edit/Write; report-only.
- Spawn independent agents in a single message so they run in parallel; the prover waits for
  all surfacers.
- Agent definitions snapshot at session start — a freshly edited `.claude/agents/*.md` is live
  NEXT session, not this one.
- Grounding, dimensions, and finding lists travel in agent prompts and results — keep each
  surfacer's prompt to its own dimension plus the relevant hotspots, not the whole grounding.
