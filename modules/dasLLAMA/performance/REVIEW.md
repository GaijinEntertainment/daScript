# dasLLAMA performance Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first — its contract binds this checklist.** Architecture
doc: `../ARCHITECTURE.md`. Planned work: `../followup_general.md`.

A tune-sidecar emission change lives in the dasLLVM tune rail and answers to
`modules/dasLLVM/REVIEW.md`.

**`exchange_schema.das` is the single validator for exchange submissions — record stores and
tune sidecars — and stays engine-free.** A second validator, or a dasLLAMA/dasLLVM require
added to it, is a defect.

**A change to the fields `write_bench_records` (`profile_common.das`) writes keeps
`modules/dasLLAMA/tests/test_exchange_schema.das` AND
`modules/dasLLAMA/tests/test_bench_records_schema.das` green in the same change.**

**`exchange_client.das` is the single exchange client — every HTTP call to the sidecar exchange
(lookup, download, submit) goes through it.** A second HTTP path is a defect.

**Weakening the exchange download gate or the submission strip
(`utils/dasllama-server/test_exchange_client.das` enforces both) is a defect** — and a
submission path around `exchange_strip_private` is one even where the strip itself is intact.

**A lookup failure never kills a boot.** A boot path that fails when the exchange lookup
fails is a defect.

**`fetch_models.das` is the model-provenance manifest; a model referenced anywhere without
its entry is a defect.** A downloaded entry carries the source URL, canonical bytes, and
sha256; a converted entry carries its conversion recipe.

**`fetch_models.das --fetch` downloads only.** A convert, a bench, or a tune-state write
added to it is a defect.

**A change to `fetch_models.das` other than to its comments records its gate run:
`fetch_models.das --` ends `0 failed`.**

**An entry point whose timed reps dispatch `[tune]`-selected kernels calls `tune_gate()`
(`profile_common.das`) before its first timed rep**, or it measures fallback kernels silently.

**A refreshed `last_known_good_sidecar.json` is one complete mint from the box its provenance
names** — `mode` `paranoid`, `noise` `ok`, the current `dasllama_version`, and every `race`
entry's `winner` equal to that family's `kernels` value; a partial or hand-edited copy is a
defect.
