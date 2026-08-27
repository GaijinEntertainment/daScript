# dasLLAMA Architecture - the benchmark rig and instrumentation rails

Companion to `ARCHITECTURE.md`; section numbers are that document's.

### 2.5 There is ONE benchmark rig, and the records are the baseline

`benchmarks/lcpp_bench.das` is the only thing that measures performance. It is a *mirror* of
the upstream `llama-bench` - the same test shapes, rep counts and timing boundaries, applied to
our engine - so `pp` is one batched prefill of `-p` tokens from an empty cache per rep and `tg`
is `-n` single-token forwards with no logit read, each row one untimed warmup plus `-r` timed
reps. The real `llama-bench` runs only when `--ref <path>` is passed; that is how the upstream
columns were produced, and they are pinned, not re-measured.

`performance/gen_bench_records.das` sweeps a board by spawning that rig once per cell, and
writes `performance/records/<box>.json`. `gen_site_records.das` merges those into the file the
site renders. A stored row carries its own command, sha, version, tune stamp and exec format, so
a number is self-describing rather than a bare figure in a table.

**Regression checking inverts the same rig:** `gen_bench_records.das --oracle --legs metal`
takes the store's das rows as the work list, re-measures each once, and gates one-sided against
its stored mean (fail past 5%, warn past 3%, gains flagged as suspicious). Upstream never runs,
the store is never written, and a text cell's timed child runs `--frozen` - a prepare pass bakes
and warms its image first (the batch starts wiped), so the timed cell never converts; ASR and
image-chat cells bake what they need mid-cell, like their publishing legs. A second
harness would produce numbers that cannot be compared to any of this.

**The tune stamp gates the comparison.** A manifest older than the binary fails every cell, and
an untuned invocation re-execs into a full retune rather than measuring - so re-mint the box
manifest and check its winners against the stored rows' `tune` stamps before trusting a delta.

**A measured number proves its kernel provenance through `tune_gate()`
(`performance/profile_common.das`), one arm per world it can run in.** Three worlds, because
`tune_status()` populates in exactly one of them: a standalone exe checks the sidecar the
release shipped beside it; a `DAS_TUNE_MANIFEST` run checks that file; a plain script checks
that every `[tune]` row stamps a manifest winner. An invocation no arm covers refuses - or
worse, measures on fallback kernels - which is why every measuring entry point calls the gate
before its first timed rep. Two rig shapes fall outside "measuring entry point" by the
property itself, ledgered here: a kernel A/B lab dispatches its variants through its own arms
(never the `[tune]` selection), and `lcpp_bench.das`'s `--tok` cell dispatches no kernels at
all - neither can measure a fallback silently.

**The retune re-exec bites scaffolding, and the pin for it is checked in.** Any bare `daslang`
run that requires the engine - a probe, a one-off script, a REPL experiment - re-execs into a
full retune when no manifest is armed. `performance/last_known_good_sidecar.json` exists for
exactly that: a frozen copy of a complete, noise-gated mint, tracked in git (the `*.tune.json`
ignore rule deliberately does not match it). Point `DAS_TUNE_MANIFEST` at it and the framework
never retunes; on a different box the identity mismatch just serves fallbacks, and a copy minted
before the current `DASLLAMA_VERSION` serves fallbacks on any box - the compile says which with
one `WARNING DAS_TUNE_MANIFEST` line per scope. That is the whole
contract - it suppresses the re-exec, it does not tune the box, and a number measured under it
is not a benchmark. Benches and the rig keep minting their own; refresh the copy when a
re-mint moves the crowns or `DASLLAMA_VERSION` bumps.

### 2.10 Sanctioned instrumentation rails

Engine timing goes through the rails that aggregate and tag it: the `jobque_profile` markers
(`profile_tag` / `profile_marker` and the `trace_*` wrappers in `dasllama_math.das`), the
`prof_add` / `forward_profile_*` decode buckets (`dasllama_common.das`), and the `asr_prof_add`
encode buckets (`dasllama_audio.das`). A new clock read paired with a print of the elapsed
interval bypasses the aggregation - it measures one call site once, is not filterable, and rots
where a rail entry would keep serving. Where a timed line IS the deliverable - `benchmarks/`,
`performance/`, `harness/`, and cold one-shot load/mint progress logs (image bake/map, load
stages, tokenizer build) - the rails do not apply. A clock whose value feeds logic is control
flow, not instrumentation; it is marked `// clock: control` so the sweep and any future lint
leave it alone.

