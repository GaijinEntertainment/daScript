# dasLLAMA performance Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
docs: `../ARCHITECTURE.md`, `../ARCHITECTURE_ENGINE.md`, `../ARCHITECTURE_MEASUREMENT.md`.
Planned work: `../followup_general.md`.

**Never add a second validator for exchange submissions (record stores and tune sidecars) -
validate through `../dasllama/dasllama_exchange_schema.das` instead.** The engine-free half (no
`dasllama/` require beyond the lint macro module) is `REVIEW.das`'s to enforce; weakening
that gate is a defect.

**Weakening any check in `REVIEW.das` - the conditions it fires on - is a defect; the fix for a
red is a re-mint on a quiet, session-free box, never an edit.**

**Narrowing the scope of any `REVIEW.das` check - the files it walks, the names it does not
flag - is a defect unless `../ARCHITECTURE_ENGINE.md` or `../ARCHITECTURE_MEASUREMENT.md`
ledgers the excluded scope in the same change.** What each check enforces is
read from the gate itself; each check's finding text states its own rule. The
single-exchange-client check walks the engine (`../dasllama/`); a measurement harness talking
HTTP to a reference server is the ledgered exclusion.

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

**A diff that writes a reference-engine row to `records/` whose `sha` names anything but the
standing ref pin (`DEFAULT_REF_SHA`, `../benchmarks/setup_lcpp_ref.das`) is a defect -
re-mint.** A reference row that carries no `sha` is pinned by the builder its record names,
and that builder is the ref pin's checkout - the cli tools and the reference server by the
checkout the record's provenance spells, the python legs by
`../benchmarks/asr/requirements-*.txt`.

**A diff that writes a records row, sidecar archive, or `defaults/` profile under this
folder whose `provenance.dasllama_version` differs from, or is absent where,
`DASLLAMA_RELEASE` (`../dasllama/dasllama_version.das`) is a defect - re-mint.** For a
sidecar with an `engine_sha`, read the value at that commit; a `defaults/` profile compares
against the branch under review. A ruler record pins its engines through `meta.das_sha`, and
through `meta.lcpp_version` when a reference arm ran (`-` when none did).

**A diff that writes a row to `records/<box>.json` mints that row from a board cell.** A board
cell is one `gen_bench_records.das` spawns, or a manual `../benchmarks/lcpp_bench.das` cell
its `../PROFILE.md` section documents. A timing taken any other way stays out of `records/`
and settles its own decision in the report where it was taken.

**A file under `records/mtp/` is written only by `../harness/mtp_ruler.das`, never by hand; a
diff that adds one names the ruler command line in the PR body.** The ruler record is the
speculative round's cell (`../ARCHITECTURE_MEASUREMENT.md` sec.2.28); its shape is the ruler's,
and the board walkers (`list_record_stores`) do not read it.

**A `records/mtp/` file that carries a reference-engine row with no our-engine row from the same
run is a defect - re-mint the pair.** A file with our-engine rows alone conforms.

**A `records/mtp/` file whose `meta.settle` is below the ruler's default names the reason in the
PR body, and a diff that adds one names its rows `direction-grade` wherever it cites them.**

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
board membership, provenance, or parity fixtures is a defect - write it as a view over those
two functions.** Board membership is which models the site results board shows.

**A view over `model_specs()` or `asr_catalog()` recomputes from them on every call, selects
rows by one field whose value on the row states membership, and stores no
`url`/`bytes`/`sha256` of its own; a view that matches a field against a list of literal
values - file names, name prefixes, recipe constants - is a defect.** A literal list is a
second catalog that drifts from the first.

**A diff that makes a recorded row or manifest under this folder pin a model file keeps that
file's provenance on its own row.** The row is a row of `model_specs()` (`model_specs.das`) or
of `asr_catalog()` (`profile_common.das`). The row carries the `url` + `bytes` + `sha256`
itself, or a `recipe` a reader can run. One named accessor call may stand in for the row: a
function in `model_specs.das` whose own body carries those three fields. A second hop does not
count - an accessor forwarding to another accessor, or an unnamed table lookup.

**A diff that adds a companion artifact - a file fetched or verified with a model and consumed
beside it: a projector, a draft head, an assistant sidecar, an image fixture - puts it in the
`companions` of the row that pins its carrier, and names it from every other row that consumes
it.** Uniqueness itself is `../tests/test_model_specs.das`'s to enforce.

**A diff that changes what any `serve_*` function in `model_specs.das` returns - a `serve_*`
field on a row, the function's body, or a `companions` entry with a `url` on a row a `serve_*`
view reads - lands `utils/dasllama-server/test_model_catalog.das` (repo root) green in the same
change** - the serving catalog is a view over these rows, so its gates red on a row this folder
ships.

**A convert, a bench, or a tune-state write reached from `fetch_models.das --fetch` is a
defect - `--fetch` downloads only.** Each has its own home: a conversion recipe runs under
`--convert`, a timing runs in a board cell (`gen_bench_records.das` or a
`../benchmarks/lcpp_bench.das` cell), and a tune sidecar is written under a `--tune` run.

**A diff that adds a row or a `companions` entry, or changes a `bytes`, `sha256` or `recipe`
value, in `model_specs.das` or `profile_common.das` records its settling evidence in the PR
description: one `fetch_models.das -- -o <name>` run per changed row, on a box holding that
row's model file, ending `0 pending, 0 failed` with the row reported `ok`, plus one unscoped
`fetch_models.das --` run in which no row the diff touched is `pending`.** A box stocks only
some of the rows, so an unscoped run's `pending` count is the box's.

**A diff that changes the `recipe` of a row carrying no `sha256` records, in the PR
description, the conversion command as run and the produced file's identity - its byte size,
or a `file_identity` hash.** `fetch_models` reports such a row `ok` on presence alone, so its
run cannot tell a re-mint from the stale file the old recipe made.

**A diff that changes `fetch_models.das` beyond its comments records its settling evidence in
the PR description: one unscoped `fetch_models.das --` run ending `0 failed`.**

**A url-only re-pin - a row's `url` changed with its `bytes` and `sha256` unchanged -
records its settling evidence in the PR description: a fetch through the new url into a
scratch dir, or a documented resolve of the pinned revision's size and content sha against
the row's canonical values** - the verify never reads the url of a file already on disk.

**A diff that refreshes `last_known_good_sidecar.json` replaces it with one complete mint from
the box its provenance names, at the current `DASLLAMA_RELEASE` - never a hand-edited copy.**
