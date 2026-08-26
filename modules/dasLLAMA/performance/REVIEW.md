# dasLLAMA performance Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
doc: `../ARCHITECTURE.md`. Planned work: `../followup_general.md`.

**`../dasllama/dasllama_exchange_schema.das` is the single validator for exchange submissions -
record stores and tune sidecars.** A second validator is a defect. The engine-free half (no
`dasllama/` require beyond the lint macro module) is `REVIEW.das`'s to enforce; weakening
that gate is a defect.

**A row or sidecar entering `records/` was minted on a quiet box: a row's
`hardware.remote_desktop` is `off`, and a sidecar's `provenance.noise` is `ok`.** A `parsec`
row, or a sidecar minted under noise, is a defect - re-mint on a box with no remote-desktop
session.

**A `das` row's `sha` and a sidecar's `provenance.engine_sha` entering `records/` name commits
reachable from the branch under review.** A stamp naming no resolvable commit is a defect -
re-mint.

**A reference-engine row's `sha` entering `records/` matches the standing ref pin
(`DEFAULT_REF_SHA`, `../benchmarks/setup_lcpp_ref.das`).** A row naming another reference
build is a defect - re-mint.

**A sidecar entering `records/` carries a `provenance.dasllama_version` equal to
`DASLLAMA_VERSION` (`../dasllama/dasllama_version.das`) at the commit its
`provenance.engine_sha` names.** A version mismatch is a defect - re-mint.

**A diff that adds a row to `records/<box>.json` mints that row from a board cell - one
`gen_bench_records.das` spawns, or a manual `../benchmarks/lcpp_bench.das` cell its
`../PROFILE.md` section documents.** A timing taken any other way - a lab's A/B arm, a
reading compared across two processes or two commits, a wall measured from outside the
benchmark process - settles its own decision in its own report and never reaches a record
file. The cell runs the released `lcpp_bench` exe (`daspkg release`), never the `-jit`
script.

**A field added to what `write_bench_records` (`profile_common.das`) writes is added to
`../dasllama/dasllama_exchange_schema.das`'s run validation in the same change** - the
validator ignores run keys it does not know, so an unvalidated field ships silently.

**`../dasllama/dasllama_exchange.das` is the single exchange client - every HTTP call to
the sidecar exchange (lookup, download, submit) goes through it.** A second HTTP path is a
defect; the mechanical half (no second `dashv` requirer under the module) is `REVIEW.das`'s
to enforce.

**Weakening the exchange download gate (content sha, schema, `DASLLAMA_VERSION`), the
submission strip, or the submit rails that keep exchange-sourced and foreign-box sidecars
from going back up is a defect** - `utils/dasllama-server/test_exchange_client.das` enforces
the download gate, the strip, and the rails.

**Every submission goes through `exchange_strip_private`.** A submission path around it is a
defect even where the strip itself is intact.

**A lookup failure never kills a boot.** A boot path that fails when the exchange lookup
fails is a defect.

**`model_specs.das`'s `model_specs()` (text) and `profile_common.das`'s `asr_catalog()`
(audio) are the model set; a third FUNCTION in `.das` under this folder listing model files,
quants, board membership (which models the site results board shows), provenance, or parity
fixtures is a defect** - a new list is written as a view over those two: it recomputes from
them on every call and stores no `url`/`bytes`/`sha256` of its own.

**A diff that makes a recorded row or manifest under this folder pin a model file keeps that
file's provenance on its own row in `model_specs()` (`model_specs.das`) or `asr_catalog()`
(`profile_common.das`).** The row carries the `url` + `bytes` + `sha256` itself, or a
`recipe` a reader can run. One named accessor call may stand in for the row: a function in
`model_specs.das` whose own body carries those three fields. A second hop - an accessor
forwarding to another accessor, or an unnamed table lookup - does not count.

**A companion artifact (an mmproj, an image fixture) rides the `companions` of the row that
pins its carrier.** A companion several rows consume is referenced from the other rows by
name - uniqueness itself is `../tests/test_model_specs.das`'s to enforce.

**`fetch_models.das --fetch` downloads only.** A convert, a bench, or a tune-state write
reached from `--fetch` is a defect - a conversion recipe runs under `--convert`, a timing in
a board cell (`gen_bench_records.das` or a `../benchmarks/lcpp_bench.das` cell), a tune
sidecar under a `--tune` run.

**A change to a model row's provenance that alters which bytes verify - `bytes`, `sha256`,
`recipe`, a new row or a new `companions` entry in `model_specs.das` or
`profile_common.das` - or a change to `fetch_models.das` other than its comments, records
its settling evidence in the PR description: a `fetch_models.das --` run ending
`0 pending, 0 failed` on a box that HOLDS the pins.**

**A url-only re-pin - a row's `url` changed with its `bytes` and `sha256` unchanged -
records its settling evidence in the PR description: a fetch through the new url into a
scratch dir, or a documented resolve of the pinned revision's size and content sha against
the row's canonical values** - the verify never reads the url of a file already on disk.

**A refreshed `last_known_good_sidecar.json` is one complete mint from the box its provenance
names, at the current `dasllama_version` - never a hand-edited copy.** `REVIEW.das` (beside
this file) machine-checks the age-independent half (`noise` `ok`, `validation` `ok`, every
`race` winner equal to its `kernels` value); weakening that gate is a defect.
