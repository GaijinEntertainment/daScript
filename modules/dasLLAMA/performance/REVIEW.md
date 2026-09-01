# dasLLAMA performance Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
doc: `../ARCHITECTURE.md`. Planned work: `../followup_general.md`.

**Never add a second validator for exchange submissions (record stores and tune sidecars) -
validate through `../dasllama/dasllama_exchange_schema.das` instead.** The engine-free half (no
`dasllama/` require beyond the lint macro module) is `REVIEW.das`'s to enforce; weakening
that gate is a defect.

**Weakening `REVIEW.das`'s record-hygiene checks - `hardware.remote_desktop` `off` on every
stored run, `provenance.noise` `ok` on every checked-in JSON under this folder that carries a
`provenance` block, no `engine_sha` in a `defaults/` profile - is a defect; the fix for a red
is a re-mint on a quiet, session-free box, never an edit.**

**A diff that writes a commit stamp anywhere under this folder naming a commit the branch
under review cannot reach is a defect - re-mint, or re-stamp to a reachable commit whose
`modules/dasLLAMA/` tree is byte-identical to the tree that was measured, with the PR body
naming the re-stamp.** The commit stamps are a `das` row's `sha` and a `provenance.engine_sha`
in any checked-in JSON under this folder. A stamp that resolves to no commit at all counts as
unreachable.

**A diff that re-stamps an archived sidecar (`records/<box>.tune.<sha12>.json`) re-hashes and
renames the file and repoints every `records/<box>.json` row whose `tune_sha` named the old
name, in the same change.** The archive is content-addressed; a row left on the old name
points at a file that no longer exists.

**A diff that writes a reference-engine row to `records/` whose `sha` names anything but the
standing ref pin (`DEFAULT_REF_SHA`, `../benchmarks/setup_lcpp_ref.das`) is a defect -
re-mint.** A reference row that carries no `sha` (the cli and python reference tools) is
pinned by its builder instead - the cli tools by that same ref pin's checkout, the python
legs by `../benchmarks/asr/requirements-*.txt`.

**A diff that moves a reference-tool pin - the ref pin's checkout, or a python leg's
`requirements-*.txt` - re-mints every row the moved pin's tool minted, or withdraws them.**

**A diff that writes a provenance manifest under this folder whose `provenance.dasllama_version`
differs from the `DASLLAMA_RELEASE` string (`../dasllama/dasllama_version.das`; the JSON key
stays `dasllama_version` across constant renames) is a defect - re-mint.** For a sidecar
with an `engine_sha`, read the value at that commit; a `defaults/` profile compares against
the branch under review.

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

**A fallback that recovers reference rows from a file on disk names the tool that writes
that file, and reads it for no other tool** - a row recovered across tools lands in the
second tool's record carrying the wrong engine, and looks real.

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

**A diff that adds or changes a `serve_*` field on a row of `model_specs()` lands
`utils/dasllama-server/test_model_catalog.das` (repo root) green in the same change** - the
serving catalog is a view over these rows, and its gates (pinned urls, unique ids, one
default) red on a row this folder ships.

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
the box its provenance names, at the current `DASLLAMA_RELEASE` - never a hand-edited copy.**
`REVIEW.das` (beside this file) machine-checks the age-independent half (`noise` `ok`,
`validation` `ok`, every `race` winner equal to its `kernels` value); weakening that gate is a
defect.

**A diff that changes the exchange consent notice wording updates every checked-in copy in
the same change, and a diff that adds a copy names it here in the same change:
`EXCHANGE_CONSENT_NOTICE` (`../dasllama/dasllama_exchange.das`), `CONSENT_TITLE` /
`CONSENT_TEXT` (`utils/watchdog/watchdog.py`, repo root), and the captured
`utils/dasllama-server/tests/fixtures/exchange.json` (repo root).** The console prompt and the
control page render the served constant, so they are not copies.

