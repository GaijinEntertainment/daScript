# dasLLAMA performance Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first — its contract binds this checklist.** Architecture
doc: `../ARCHITECTURE.md`. Planned work: `../followup_general.md`.

A tune-sidecar emission change lives in the dasLLVM tune rail and answers to
`modules/dasLLVM/REVIEW.md`.

**`../dasllama/dasllama_exchange_schema.das` is the single validator for exchange submissions —
record stores and tune sidecars — and stays engine-free: no require of a module that compiles
engine code in (a compile-time macro module is not engine code).** A second validator, or an
engine require added to it, is a defect.

**A field added to what `write_bench_records` (`profile_common.das`) writes is added to
`../dasllama/dasllama_exchange_schema.das`'s run validation in the same change** — the validator ignores run keys
it does not know, so an unvalidated field ships silently.

**Weakening `modules/dasLLAMA/tests/test_exchange_schema.das` or
`modules/dasLLAMA/tests/test_bench_records_schema.das` — both round-trip the real
`write_bench_records` output — is a defect.**

**`../dasllama/dasllama_exchange.das` is the single exchange client — every HTTP call to the sidecar exchange
(lookup, download, submit) goes through it.** A second HTTP path is a defect.

**Weakening the exchange download gate (content sha, schema, `DASLLAMA_VERSION`), the
submission strip, or the submit rails that keep exchange-sourced and foreign-box sidecars
from going back up is a defect** — `utils/dasllama-server/test_exchange_client.das` enforces
all three.

**Every submission goes through `exchange_strip_private`.** A submission path around it is a
defect even where the strip itself is intact.

**A lookup failure never kills a boot.** A boot path that fails when the exchange lookup
fails is a defect.

**`model_specs.das`'s `model_specs()` (text) and `profile_common.das`'s `asr_catalog()`
(audio) are the model set; a third list of model files, quants, board membership,
provenance, or parity fixtures is a defect** — a new list is written as a view over those two
(`llm_catalog`, `official_catalog`, `models_provenance` are such views).

**A model file this repo fetches, converts, or publishes a public benchmark-board number for
carries its provenance on its own row in `model_specs()` (`model_specs.das`) or
`asr_catalog()` (`profile_common.das`) — `url` + `bytes` + `sha256`, or a `recipe` — and a
companion artifact (an mmproj, an image fixture) rides its owning row's `companions`.**
Weakening `../tests/test_model_specs.das`'s provenance invariants is a defect.

**`fetch_models.das --fetch` downloads only.** A convert, a bench, or a tune-state write
added to it is a defect.

**A change to a provenance field on a model row (`url`, `bytes`, `sha256`, `recipe`, a
`companions` entry in `model_specs.das` or `profile_common.das`), or to `fetch_models.das`
other than its comments, records its gate run in the PR description: `fetch_models.das --`
ends `0 failed`.**

**An entry point whose timed reps dispatch `[tune]`-selected kernels calls `tune_gate()`
(`profile_common.das`) before its first timed rep**, or it measures fallback kernels silently.

**A refreshed `last_known_good_sidecar.json` is one complete mint from the box its provenance
names** — `noise` `ok`, `validation` `ok`, the current `dasllama_version`, and every `race`
entry's `winner` equal to that family's `kernels` value; a partial or hand-edited copy is a
defect.
