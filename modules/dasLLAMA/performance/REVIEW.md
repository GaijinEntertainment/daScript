# dasLLAMA performance Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first — its contract binds this checklist.** Architecture
doc: `../ARCHITECTURE.md`. Planned work: `../followup_general.md`.
A tune-sidecar emission change lives in the dasLLVM tune rail and is reviewed with
`skills/llvm_tune.md`; its exchange gate is the same
`modules/dasLLAMA/tests/test_exchange_schema.das`.

**`exchange_schema.das` is the single validator for exchange submissions — record stores and
tune sidecars — and stays engine-free.** A second validator, or a dasLLAMA/dasLLVM require added
to it, is a defect; a change to what `write_bench_records` (`profile_common.das`) emits keeps
`modules/dasLLAMA/tests/test_exchange_schema.das` green.

**`exchange_client.das` is the single exchange client — every HTTP call to the sidecar exchange
(lookup, download, submit) goes through it.** A second HTTP path is a defect.

**Everything downloaded from the exchange passes the client-side gate — content sha, schema,
`DASLLAMA_VERSION` — before anything reads it.**

**Every submission goes through `exchange_strip_private`; exchange-sourced and foreign-box
sidecars are never submitted; a lookup failure never kills a boot.** A submission around the
strip is a defect.

**`fetch_models.das` is the model-provenance manifest** — an entry is the HF repo + revision
pin, canonical bytes + sha256, or a conversion recipe. A model referenced anywhere without its
entry is a defect. `--fetch` downloads only: a convert, a bench, or a tune-state write added to
it is a defect.

**A change to an entry, or to the verify or fetch logic, in `fetch_models.das` records its gate
run: `fetch_models.das --` ends `0 failed`.** A comment-only change does not.

**An entry point whose timed reps dispatch `[tune]`-selected kernels calls `tune_gate()`
(`profile_common.das`) before its first timed rep**, or it measures fallback kernels silently.

**A refreshed `last_known_good_sidecar.json` is one complete mint from the box its provenance
names** — `mode` `paranoid`, `noise` `ok`, the current `dasllama_version`, a `race` entry for
every raced family in `kernels`; a partial or hand-edited copy is a defect.
