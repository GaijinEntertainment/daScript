# dasLLAMA performance Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first — its contract binds this checklist.** Architecture
doc: `../ARCHITECTURE.md`.

**`exchange_schema.das` is the single validator for exchange submissions — record stores and
tune sidecars — and stays engine-free.** A second validator, or a dasLLAMA/dasLLVM require added
to it, is a defect; a change to what the writers emit keeps `tests/test_exchange_schema.das`
green.

**`exchange_client.das` is the single exchange client — every HTTP call to the sidecar exchange
(lookup, download, submit) goes through it.** Everything downloaded passes the client-side gate
(content sha, schema, `DASLLAMA_VERSION`) first; every submission goes through
`exchange_strip_private`; exchange-sourced and foreign-box sidecars are never submitted; a
lookup failure never kills a boot. A second HTTP path, or a submission around the strip, is a
defect.

**`fetch_models.das` is the model-provenance manifest** — an entry is the HF repo + revision
pin, canonical bytes + sha256, or a conversion recipe. A model referenced anywhere without its
entry is a defect. `--fetch` downloads only: a convert, a bench, or a tune-state write added to
it is a defect.

**A change to an entry, or to the verify or fetch logic, in `fetch_models.das` records its gate
run: `fetch_models.das --` ends `0 failed`.** A comment-only change does not.

**A new measuring entry point calls `tune_gate()` (`profile_common.das`) before its first timed
rep**, or it measures fallback kernels silently. Kernel A/B labs dispatch through their own
arms and the `--tok` cell dispatches none — neither is a measuring entry point in this sense.
