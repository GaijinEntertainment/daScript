# dasLLAMA Hot-Path Annotation Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
doc: `ARCHITECTURE_RUNTIME.md`. Planned work: `followup_general.md`.

**Routed from `REVIEW.md`: a diff that checklist routes here applies this list together with
`REVIEW.md`'s.**

**Every new kernel or loop the runtime re-enters per token, per frame, or per prefill
quantum - one batch of prompt tokens the prefill path processes in a single pass - is reached
by an annotated region entry: `[hot_path]`, any of the `[no_alloc]` / `[no_env]` / `[no_io]`
contracts, or `[cold_path]` on its only reaching entry.** The region entry is the OUTERMOST such
function - interior means every caller is itself re-entered that way, so a function reached only
through a registered function value is an entry (`ARCHITECTURE_RUNTIME.md` sec.2.11).

**A diff that renames a function carrying `[hot_path]`, `[cold_path]` or a `[no_alloc]` /
`[no_env]` / `[no_io]` contract moves that annotation to the new name in the same change** - it
is no new entry.

**A `[hot_path]` or a `[no_alloc]` / `[no_env]` / `[no_io]` contract on a function below the
region entry is a defect - move it to the entry; an interior function carries only a
`[cold_path]` on a rarely-taken branch.**

**Never put `[hot_path]` or a `[no_alloc]` / `[no_env]` / `[no_io]` contract on a loop reached
only from a load, stage, bake, or convert path - it is no region entry; it carries `[cold_path]`
or nothing.**

**A driver that calls a model serving entry - `forward`, `eval`, `eval_batch`, or a `forward_*`
entry taking a `Session` - and is reached only by a measurement - a benchmark row, a rig's loop -
carries `[cold_path]`.**
