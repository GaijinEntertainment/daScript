# dasLLAMA performance Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
docs: `../ARCHITECTURE.md`, `../ARCHITECTURE_ENGINE.md`, `../ARCHITECTURE_MEASUREMENT.md`.
Planned work: `../followup_general.md`.

**Never add a second validator for exchange submissions (record stores and tune sidecars) -
validate through `../dasllama/dasllama_exchange_schema.das` instead.**

**A diff that narrows any `REVIEW.das` check - the files it walks, the names it does not flag -
ledgers the excluded scope in `../ARCHITECTURE_ENGINE.md` or `../ARCHITECTURE_MEASUREMENT.md`
in the same change.**

**A diff that weakens any `REVIEW.das` check in any other way - the conditions it fires on - is
a defect.**

**A diff that answers a `REVIEW.das` red on the conditions a `records/` row, an archived
sidecar, a `defaults/` profile, or `last_known_good_sidecar.json` was measured under - box
noise, a remote-desktop session, the release it was minted at - by hand-editing that artifact
is a defect: re-mint it on a quiet, session-free box instead.**

**A diff that writes a commit stamp anywhere under this folder naming a commit the branch
under review cannot reach is a defect - re-mint, or re-stamp to a reachable commit whose
`modules/dasLLAMA/` tree is byte-identical to the tree that was measured, with the PR body
naming the re-stamp.** A commit stamp is any field in a checked-in JSON under this folder that
names a daslang commit - a `das` row's `sha`, a sidecar's `provenance.engine_sha`, a ruler
record's `meta.das_sha`. A stamp that resolves to no commit at all counts as unreachable.

**A diff that re-stamps an archived sidecar (`records/<box>.tune.<sha12>.json`) re-hashes and
renames the file and repoints every `records/<box>.json` row whose `tune_sha` named the old
name, in the same change.** The archive is content-addressed; a row left on the old name
points at a file that no longer exists.

**A diff that writes a reference-engine row - a run row whose `engine` is not `das` - to
`records/<box>.json` whose `sha` names anything but the standing ref pin (`DEFAULT_REF_SHA`,
`../benchmarks/setup_lcpp_ref.das`) is a defect - re-mint.**

**A diff that writes a reference-engine row carrying no `sha` to `records/<box>.json` names,
in that row's provenance, the checkout that built the binary it timed; a python leg names
`../benchmarks/asr/requirements-*.txt` instead.**

**A diff that writes a `records/<box>.json` row, sidecar archive, or `defaults/` profile under
this folder whose version pin is missing, or differs from `DASLLAMA_RELEASE`
(`../dasllama/dasllama_version.das`), is a defect - re-mint.** The pin is a records row's
`dasllama_version`, and `provenance.dasllama_version` in a sidecar archive or a `defaults/`
profile. For a sidecar with an `engine_sha`, read the value at that commit; a `defaults/`
profile compares against the branch under review.

**A diff that writes a row to `records/<box>.json` mints that row from a board cell.** A board
cell is one `gen_bench_records.das` spawns, or a manual `../benchmarks/lcpp_bench.das` cell
its `../PROFILE.md` section documents. A timing taken any other way settles its own decision
in the report where it was taken.

**Only the reference cells of `gen_bench_records.das` - the cells that time, over a board
workload, a program this repository does not build - write a reference-engine row into
`records/<box>.json`.**

**A file under `records/mtp/` is written only by `../harness/mtp_ruler.das`, never by hand; a
diff that adds one names the ruler command line in the PR body.** The ruler record is the
speculative round's cell (`../ARCHITECTURE_MEASUREMENT.md` sec.2.45); its shape is the ruler's,
and the board walkers (`list_record_stores`) do not read it.

**A diff that adds or changes a `records/mtp/` file names its engines in `meta.das_sha`, and in
`meta.lcpp_version` when a reference arm ran (`-` when none did).**

**A diff that adds or changes a `records/mtp/` file carrying a reference-engine row with no
`das` row from the same run is a defect - re-mint the pair.**

**A diff that adds or changes a `records/mtp/` file whose `meta.settle` is missing, or below
the ruler's `--settle` default (the ruler's `--help` states it), names the reason in the PR body
and labels the file's rows `direction-grade` everywhere the change cites them - PR body, ledger
rows, architecture or profile docs.**

**A diff that writes a `das` row - a run row whose `engine` is `das` - to `records/<box>.json`
times that row with the released `lcpp_bench` exe.** That exe is
`../benchmarks/lcpp_bench.das` built by `daspkg release`.

**A diff that writes a `llama.cpp` row - a run row whose `engine` is `llama.cpp` - to
`records/<box>.json` times that row with the reference exe the ref pin builds.**

**A field added to what `write_bench_records` (`profile_common.das`) writes is added to
`../dasllama/dasllama_exchange_schema.das`'s run validation in the same change** - the
validator ignores run keys it does not know, so an unvalidated field ships silently.

**A fallback that recovers reference rows from a file on disk names the tool that writes
that file, and reads it for no other tool** - a row recovered across tools lands in the
second tool's record carrying the wrong engine, and looks real.

**Outside `model_specs()` (text, in `model_specs.das`) and `asr_catalog()` (audio, in
`profile_common.das`), a `.das` function under this folder that lists model files, quants,
board membership, provenance, or parity fixtures is a defect - write it as a view over those
two functions.** Board membership is which models the site results board shows.

**A diff that adds or changes a view over `model_specs()` or `asr_catalog()` recomputes from
them on every call; a view storing a `url`, `bytes`, or `sha256` of its own is a defect.**

**A view over `model_specs()` or `asr_catalog()` selects its rows by one field whose value on
the row states membership; a view that matches a field against a list of literal values - file
names, name prefixes, recipe constants - is a defect.** A literal list is a second catalog that
drifts from the first.

**A diff that makes a records row, a sidecar archive or a `defaults/` profile under this
folder pin a model file keeps that file's provenance on its own row.** The row is a row of
`model_specs()` (`model_specs.das`) or of `asr_catalog()` (`profile_common.das`). The row
carries the `url` + `bytes` + `sha256` itself, or a `recipe` a reader can run. One named
accessor call may stand in for the row: a function in `model_specs.das` whose own body carries
those three fields. A second hop does not count - an accessor forwarding to another accessor,
or an unnamed table lookup.

**A diff that adds a companion artifact - a file fetched or verified with a model and consumed
beside it: a projector, a draft head, an assistant sidecar, an image fixture - puts it in the
`companions` of the row that pins its carrier.**

**A diff that adds a row whose test cell consumes a companion pinned on another row - a fixture,
an oracle, a twin file that cell reads beside it - names that companion in the new row's
`companions`, in the same change; a diff that adds a companion an existing row's cell consumes
names it from that row too.** A box that fetches the row alone gets its companions with it, and
the cell runs instead of skipping.

**A diff that changes what any `serve_*` function in `model_specs.das` returns - a `serve_*`
field on a row, the function's body, or a `companions` entry with a `url` on a row a `serve_*`
view reads - lands `utils/dasllama-server/test_model_catalog.das` (repo root) green in the same
change** - the serving catalog is a view over these rows, so its gates red on a row this folder
ships.

**A convert, a bench, or a tune-state write reached from `fetch_models.das --fetch` is a
defect - `--fetch` downloads only.** Each has its own home: a conversion recipe runs
under `--convert`, a timing runs in a board cell (`gen_bench_records.das` or a
`../benchmarks/lcpp_bench.das` cell), and a tune sidecar is written under a `--tune` run.

**A diff that adds a row or a `companions` entry, or changes a `bytes`, `sha256` or `recipe`
value, in `model_specs.das` or `profile_common.das` records its settling evidence in the PR
description: one `fetch_models.das -- -o <name>` run per changed row, on a box holding that
row's model file, ending `0 pending, 0 failed` with the row reported `ok`, plus one unscoped
`fetch_models.das --` run in which no row the diff touched is `pending`.** A box stocks only
some of the rows, so an unscoped run's `pending` count is the box's.

**A diff that adds a `recipe` row carrying no `sha256`, or changes such a row's `recipe`,
records, in the PR description, the conversion command as run and the produced file's
identity - its byte size, or a `file_identity` hash.** `fetch_models` reports such a row `ok`
on presence alone, so its run cannot tell a re-mint from the stale file the old recipe made.

**A diff that changes `fetch_models.das` beyond its comments records its settling evidence in
the PR description: one unscoped `fetch_models.das --` run ending `0 failed`.**

**A `url` a diff adds or re-points on a `model_specs.das` or `profile_common.das` row, or on
one of that row's `companions` entries, records its settling evidence in the PR description: a
fetch through that url into a scratch dir, or the size and content sha the url's host reports
for that file, pasted with the command that asked, matching the row's `bytes` and `sha256`** -
`fetch_models` checks a file already on disk by size and hash, never through its url.

**A diff that refreshes `last_known_good_sidecar.json` replaces it with one complete mint from
the box its provenance names, at the current `DASLLAMA_RELEASE` - never a hand-edited copy.**
