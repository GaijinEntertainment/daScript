# dasLLAMA performance Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
doc: `../ARCHITECTURE.md`. Planned work: `../followup_general.md`.

**Never add a second validator for exchange submissions (record stores and tune sidecars) -
validate through `../dasllama/dasllama_exchange_schema.das` instead.** The engine-free half (no
`dasllama/` require beyond the lint macro module) is `REVIEW.das`'s to enforce; weakening
that gate is a defect.

**A diff that writes a row to `records/` (adds one or re-mints one in place) whose
`hardware.remote_desktop` is anything but `off` is a defect - re-mint on a box with no
remote-desktop session.**

**A diff that writes a sidecar to `records/` whose `provenance.noise` is not `ok` is a
defect - re-mint on a quiet box.**

**A diff that writes a commit stamp to `records/` naming a commit the branch under review
cannot reach is a defect - re-mint, or re-stamp to a reachable commit whose
`modules/dasLLAMA/` tree is byte-identical to the tree that was measured, with the PR body
naming the re-stamp.** The commit stamps are a `das` row's `sha` and a sidecar's
`provenance.engine_sha`. A stamp that resolves to no commit at all counts as unreachable.

**A diff that writes a reference-engine row to `records/` whose `sha` is not the standing ref pin
(`DEFAULT_REF_SHA`, `../benchmarks/setup_lcpp_ref.das`) is a defect - re-mint.**

**A diff that writes a sidecar to `records/` whose `provenance.dasllama_version` differs from
`DASLLAMA_RELEASE` (`../dasllama/dasllama_version.das`) is a defect - re-mint.** Read
`DASLLAMA_RELEASE` at the commit the sidecar's `provenance.engine_sha` names.

**A diff that writes a row to `records/<box>.json` mints that row from a board cell.** A board
cell is one `gen_bench_records.das` spawns, or a manual `../benchmarks/lcpp_bench.das` cell
its `../PROFILE.md` section documents. A timing taken any other way stays out of `records/`
and settles its own decision in the report where it was taken.

**A diff that writes a `das` row to `records/<box>.json` times that row with the released
`lcpp_bench` exe.** That exe is `../benchmarks/lcpp_bench.das` built by `daspkg release`.

**A diff that writes a reference-engine row to `records/<box>.json` times that row with the
reference exe the ref pin builds.**

**A field added to what `write_bench_records` (`profile_common.das`) writes is added to
`../dasllama/dasllama_exchange_schema.das`'s run validation in the same change** - the
validator ignores run keys it does not know, so an unvalidated field ships silently.

**Never add a second HTTP path to the sidecar exchange - every HTTP call (lookup, download,
submit) goes through `../dasllama/dasllama_exchange.das`.** The mechanical half (no second
`dashv` requirer under the module) is `REVIEW.das`'s to enforce.

**Weakening the exchange download gate, the submission strip, or the submit rails is a
defect.** The download gate checks content sha, schema, and `DASLLAMA_RELEASE`. The submit
rails stop a sidecar that came from the exchange, or was minted on another box, from being
submitted. `utils/dasllama-server/test_exchange_client.das` enforces the download gate, the
strip, and the rails.

**A diff that adds a submission path around `exchange_strip_private` is a defect, even where
the strip itself is intact.**

**A tune-boot path (`exchange_scope_resolver` / `exchange_boot_submit_check`,
`../dasllama/dasllama_exchange.das`) that fails when `exchange_lookup` fails is a defect -
it falls through to the local sidecar and the baked winners.**

**Outside `model_specs()` (text, in `model_specs.das`) and `asr_catalog()` (audio, in
`profile_common.das`), a `.das` function under this folder that lists model files, quants,
board membership, provenance, or parity fixtures is a defect.** Board membership is which
models the site results board shows. Write a new list as a view over those two functions: it
recomputes from them on every call, and it stores no `url`/`bytes`/`sha256` of its own.

**A diff that makes a recorded row or manifest under this folder pin a model file keeps that
file's provenance on its own row.** The row is a row of `model_specs()` (`model_specs.das`) or
of `asr_catalog()` (`profile_common.das`). The row carries the `url` + `bytes` + `sha256`
itself, or a `recipe` a reader can run. One named accessor call may stand in for the row: a
function in `model_specs.das` whose own body carries those three fields. A second hop does not
count - an accessor forwarding to another accessor, or an unnamed table lookup.

**A diff that adds a companion artifact (an mmproj, an image fixture) puts it in the
`companions` of the row that pins its carrier, and names it from every other row that consumes
it.** Uniqueness itself is `../tests/test_model_specs.das`'s to enforce.

**A convert, a bench, or a tune-state write reached from `fetch_models.das --fetch` is a
defect - `--fetch` downloads only.** Each has its own home: a conversion recipe runs under
`--convert`, a timing runs in a board cell (`gen_bench_records.das` or a
`../benchmarks/lcpp_bench.das` cell), and a tune sidecar is written under a `--tune` run.

**A change to a model row's provenance that alters which bytes verify - `bytes`, `sha256`,
`recipe`, a new row or a new `companions` entry in `model_specs.das` or
`profile_common.das` - or a change to `fetch_models.das` other than its comments, records
its settling evidence in the PR description: a `fetch_models.das --` run ending
`0 pending, 0 failed` on a box that already has the pinned files on disk.**

**A url-only re-pin - a row's `url` changed with its `bytes` and `sha256` unchanged -
records its settling evidence in the PR description: a fetch through the new url into a
scratch dir, or a documented resolve of the pinned revision's size and content sha against
the row's canonical values** - the verify never reads the url of a file already on disk.

**A diff that refreshes `last_known_good_sidecar.json` replaces it with one complete mint from
the box its provenance names, at the current `dasllama_version` - never a hand-edited copy.**
`REVIEW.das` (beside this file) machine-checks the age-independent half (`noise` `ok`,
`validation` `ok`, every `race` winner equal to its `kernels` value); weakening that gate is a
defect.

**A diff that changes the exchange consent notice wording updates every checked-in copy in
the same change, and a diff that adds a copy names it here in the same change:
`EXCHANGE_CONSENT_NOTICE` (`../dasllama/dasllama_exchange.das`), `CONSENT_TITLE` /
`CONSENT_TEXT` (`utils/watchdog/watchdog.py`, repo root), and the captured
`utils/dasllama-server/tests/fixtures/exchange.json` (repo root).** The console prompt and the
control page render the served constant, so they are not copies.

**A diff that adds an outbound exchange request reachable from a tune-boot path
(`exchange_scope_resolver` / `exchange_boot_submit_check`) without routing it through
`exchange_may_contact` (`../dasllama/dasllama_exchange.das`) is a defect.**

**A diff that adds a tune-boot-path (`exchange_scope_resolver` /
`exchange_boot_submit_check`) consent question with no terminal to ask on also emits that
question as a `@sidecar` event, in the same change** - the watchdog dialog and the control
page are the answer surfaces a supervised boot has. (The stdin-prompt ban itself is
`modules/dasLLAMA/REVIEW.md`'s, which every file under the module applies.)
