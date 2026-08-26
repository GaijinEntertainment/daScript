# dasLLAMA Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture doc:
`ARCHITECTURE.md`. Planned work: `followup_general.md`, `followup_vulkan.md`; the perf
backlog is `PERF_LEDGER.md` - a performance followup goes there, everything else in the
two followup ledgers.

**A dasLLAMA `[test]` file, wherever the diff puts it, answers to this module's
`tests/REVIEW.md`.**

**A timing rig - a script whose output is a measured wall or rate - wherever the diff puts
it, answers to `benchmarks/REVIEW.md`.**

**A change to what enters `performance/records/` or a provenance manifest, or a change to
WHICH model file (a `.gguf`, a `.dlim`, an mmproj, an image or audio fixture) a recorded row
or manifest pins, answers to `performance/REVIEW.md`.** A test or tool merely opening a
stocked model file by name does not route.

**A change to the sidecar-exchange client or schema - the code that downloads tune winners to
a box and submits that box's winners back - answers to `performance/REVIEW.md`.**

**Every `dasllama/` change applies `tests/REVIEW.md`.**

**A GPU kernel, driver, dispatch-class, or K/V-mirror change applies `REVIEW_GPU.md`.**

**A change to `dasllama/dasllama_audio.das`, `dasllama/dasllama_audio_io.das`,
`dasllama/dasllama_audio_embedder.das`, `dasllama/dasllama_asr.das`,
`dasllama/dasllama_asr_types.das`, `dasllama/dasllama_vad.das`, or an ASR family file - one
`dasllama/dasllama_<family>.das` holding a single speech model family - applies
`REVIEW_AUDIO.md`.**

**A change to `dasllama/dasllama_vision.das`, `dasllama/dasllama_vision_io.das`,
`dasllama/dasllama_vision_embedder.das`, a vision family file - one
`dasllama/dasllama_<family>.das` holding a single vision projector family - or an in-process
path (one that runs inside the program under review, not a spawned child process) that
splices a stream carrying decoded media - pixels or audio samples - into a prompt or
schedules such a stream, applies `REVIEW_VISION.md`.**

**A `dasllama/dasllama_tower.das` change - the shared encoder-tower home - applies
`REVIEW_AUDIO.md` and `REVIEW_VISION.md`;** a family file that only CALLS a shared rail does
not thereby pick up the other modality's checklist.

**A routed file applies BOTH the checklist it routes to and this one; every other file under
`modules/dasLLAMA/` applies this one.**

**Any kernel work bumps `DASLLAMA_VERSION` (`dasllama/dasllama_version.das`) in the same
change, and a bump with no kernel work is the same defect.** Kernel work is whatever changes
the generated kernel source, or the set of compiled pipeline variants (PSOs) built from it -
a kernel body, a variant set, or a `[tune]` / `[tune_perm]` / `[tune_companion]` grid. A
host-side bind or dispatch-argument change (an `@off` binding, a uniform value) is not, and
neither is `[tune_scope]` metadata (`covers=`, `tuner=`, `version_of=`). Equal versions mean
an equal kernel set and an interchangeable sidecar set - the exchange keys validity on
version and box.

**A kernel's shape is compile-time; only its data is runtime.** For a given compiled kernel,
can this value change between dispatches? If yes it is data and belongs in a uniform, a kargs
field, or an `@off` bind offset; if no it is shape and must not reach the kernel as a uniform,
a kargs field, an `@off` bind offset, or a helper parameter.

**A diff that changes a kernel-selection predicate, or a constant whose value was chosen by
timing two candidates against each other, in `dasllama/` rests on timing that ran both
variants interleaved in one process, under one instrument.** A reading taken across two
processes or across two commits says which way the wall moved, not which implementation to
adopt.

**Peak memory wins ties against load cost.** A change to an allocation reached from a load,
bake, or convert path (judge a shared helper at each call site) that trades footprint for speed
ships the measured pair - peak footprint and wall-clock - and an explicit stated decision.

**A new GEMM/GEMV call site takes the fastest serving lane that exists for its weights; the
f32 fallback is for correctness rails only.** A new call to an f32 matmul (`matmul_batch`,
`mm_blob_b`, per-head `gemm_f32`, or an f32 GPU mm) outside a parity or oracle rail, where a
faster-format twin already serves the same weights and shape, is a defect. Weights with no
faster twin (unquantized planes) are out of scope; a site that must stay f32 for another
reason is ledgered on the site's `ARCHITECTURE.md` sec.1 charter line, not commented into
compliance.

**Platform-specific code in an engine file (`dasllama/`) lands only in that platform's backend
file.** A platform-neutral engine file carrying it is a defect.

**A new engine concern that is not `Model`/`Session`/`Config` state gets its own file, not
more of `dasllama/dasllama_common.das`.**

**No ad-hoc profiling.** A NEW clock read paired with a print or log of the elapsed interval is
a defect in an engine file (`dasllama/`) outside a cold one-shot load, bake, map, or
tokenizer-build progress log - instrumentation goes through the sanctioned rails, listed with
their reasons in `ARCHITECTURE.md` sec.2.10.

**A clock value that changes what the program DOES - control flow, eviction, a generated
name; not a reported wall or a best-of reduction over reported walls - is marked
`// clock: control`, in an engine file (`dasllama/`)** - unmarked it reads as free-hand
timing to the ad-hoc-profiling sweep, which runs over the same tree.

**Every new kernel or loop the runtime re-enters per token, per frame, or per prefill
quantum is COVERED by an annotated region entry** - `[hot_path]`, any of the `[no_alloc]` /
`[no_env]` / `[no_io]` contracts, or `[cold_path]` on its only reaching entry. Covered means
an annotated entry reaches it: the contracts arm down the call graph, so an interior
function carries nothing of its own. A region entry is the outermost such function (a kernel
`*_encode` / `*_decode`, a step driver, the CPU decoder's `forward_*` entries); a loop
reached only from a load, stage, bake, or convert path is not one.

**A new function that no annotated entry reaches but the runtime re-enters per token, per
frame, or per prefill quantum - a step driver, or a backend entry called from dispatch or
harness paths - carries its annotation itself; a rename is not new** (annotations follow
the name in the same change).

**A change to code or data of `encode`/`bpe_encode` or anything they reach in
`dasllama/dasllama_spm.das` / `dasllama/dasllama_bpe.das` / `dasllama/dasllama_pretok.das`
ships before/after `--tok` rows (`benchmarks/lcpp_bench.das`) for the affected backend** - the
instrument is the scaling ratio across the size ladder, and superlinear is a defect.

**A change to code or data in `dasllama/dasllama_tokenizer.das`, `dasllama/dasllama_spm.das`,
`dasllama/dasllama_bpe.das`, or `dasllama/dasllama_pretok.das`, or to the special-token or
template strings any of them look up, records a `tests/test_tokenizer.das` run with its cases
EXECUTED, not skipped.**

**An override announces itself where it changes the outcome.** An override is an environment
knob or an exported runtime setter that moves a gate, policy, or threshold off its default
and thereby changes what the run writes, reads, mints, or computes - including a knob or
setter whose purpose is timing when it moves computed numerics, since two GEMM forms of
the same math differ in float terms. A knob or setter that changes only WHEN work happens
is not an override, and a CLI flag is never one. A run that engages one prints a line naming
the override by the spelling a user would set - the environment variable name, the sidecar
key, or the setter's function name. Per-site repeats are fine. Adding one, or giving one a
new effect, without the announce is a defect.

**A self-measured served-turn time - tok/s, a turn wall - entering `PERF_LEDGER.md` comes
from the released `lcpp_bench` exe - `benchmarks/lcpp_bench.das` built by `daspkg release`,
spawned by `performance/gen_bench_records.das` or run by hand where the cell's `PROFILE.md`
section says so - never from the `-jit` script** (a `--for-debug-purposes` row is a debug
instrument). A tutorial's printed wall-clock is teaching output, feeding no board.

**A subtraction of two measured walls written into `PERF_LEDGER.md` carries both raw walls
in the entry.**

**A diff that adds an entry to `PERF_LEDGER.md` carrying a reading no board cell produced
names the instrument that produced it.** The entry also tags the reading `direction-grade`
when it compares across two processes or two commits, and `out-of-process` when the wall was
measured from outside the benchmark process; a reading that is neither carries no tag.

**A diff that adds an entry to `PERF_LEDGER.md` never records a lab's A/B selection timing** -
that timing settles its adoption decision in the lab's own report and in the PR that lands
the kernel, and the ledger learns the winner only through a re-measured cell.

**A new servable capability gets its cell in the same change**: a board row spawned by
`performance/gen_bench_records.das`, or a manual `benchmarks/lcpp_bench.das` cell with its own
`PROFILE.md` section. A servable capability is a modality, a family, or a serving path - a
lane a user's turn can be served through, a q8 or f32 serving lane and a GPU tower included;
a new family or serving path landing inside an existing cell re-mints that cell's row on at
least one box instead.

**A timing figure describing a served turn as a whole - tok/s, latency, a whole-turn model
or engine comparison - is a defect wherever it is written down with no cell behind it: a
checked-in doc, a ledger, a code comment, or a PR description.** The cell states its quant
mode and stamps box and engine provenance, so a number can never silently describe a format
nobody serves or a kernel set nobody ships. A rig-internal measurement margin - a crown
delta, a noise floor, tuner timing - is settled by the sidecar or manifest that carries the
value; written into a source comment or a doc it is a timing figure like any other.

**A figure measuring one engine stage inside a served turn - a stage wall, a stage share, a
stage speedup, or a cross-engine comparison of one stage; never a board cell's `pp`/`tg`
rate and never the whole wall of a `benchmarks/lcpp_bench.das` `-p`/`-n` cell - names the
harness and flags that produced it, wherever it is written down: a checked-in doc, a
ledger, a code comment, or a PR description.** The naming rides the
figure's own sentence, a table heading that covers the table's rows, or a section-level
provenance line that covers the paragraphs under it.

**Runtime serves weights out of a mapped `.dlim`.** A live carrier's planes point into
`parse_image`'s mapping, and going live does no real work - repacking, quantizing, folding,
permuting belong to the mint. A transform on the go-live path is a defect.

**A missing `.dlim` is minted first, and the model is served from what was minted.**

**There is one way to mint, and one way to load.** A weight carrier becomes live through
`build_image` and `parse_image` in `dasllama/dasllama_image.das`. Reading weights into a live carrier,
or releasing an image backing, anywhere else is a defect - and a second mint path, per family,
per format, or per backend, is a defect even where its output is identical.

**A decoder mint - the mint of an LLM decoder model, not a tower or embedder carrier - never
holds the whole model.** It sizes the image before the first byte goes out and writes each
plane as it is produced. Keeping the source model resident to write from is a defect, and a
mint that is slower in exchange for a lower peak is correct.

**A staged carrier mint (`cache_via_image_staged`) either refuses a source file at or past
1 GiB, naming that file in the refusal, or streams it the way a decoder mint does** - the
staged form holds source and image at once, and that doubled peak is what the line caps.

**A `.dlim` is box- and config-specific, not a portable format.** `image_identity` names the
box profile, the knobs, and the flavor a file was baked for, and a mismatch declines loudly. A
path that reinterprets a mismatched image, or widens an identity so that more files match, is a
defect.

**An image save reaps only its own lane** - an identity's (quant, tag) pair. A save drops AT
MOST that lane's dead siblings plus BROKEN/version-stale images in any lane, nothing else.

**Only a process that can recompute an image's identity may judge it dead.** Reaping an image
whose identity the code cannot recompute - another flavor's, another family's - is a defect.

**A plane split that follows the source FILE rather than a runtime knob takes ONE image tag**,
with the meta flags describing the layout - a per-tensor type split is not a second flavor.

**An image carries only what its flavor uses.** A plane the target platform or config never
reads is not written - the mint decides, not the load. Carrying another flavor's planes is a
defect.

**A flavor takes its image file through `image_path_for` and its tag through
`register_image_family_tag`.** A path or tag formed any other way is a defect.

**A change to user-facing API updates every place it is shown.** User-facing means anything a
consumer outside this repo can depend on - what it calls, types, requires, or parses
(facade functions, CLI flags, environment knobs, file formats, defaults, what the installed
SDK lets a program `require`) - plus the in-repo rig and tool surface: any output another tool
parses, a console-only diagnostic not being one. A tutorial source, `.rst` page, docstring,
help string, `README.md`, or checked-in document still showing the old call, flag, or default
is a defect of the change, not of the docs.

**Weakening `dasllama_lint` (`dasllama/dasllama_lint.das`) - the compile-time check that a
consumer requires only this module's public entry modules, matched by the resolved file's
path under `modules/dasLLAMA/` - is a defect:** a module added to its allowed set, the path
match dropped or narrowed, or an error text that no longer names the facade to require
instead. The allowed set is the table in the lint.

**`options _dasllama_internal` belongs only in a file whose job is to reach engine
internals: an engine file under `dasllama/`, a test, harness, benchmark, or rig this module
owns, or a consumer `ARCHITECTURE.md` sec.1 names as ruled.** Any other file carrying it is a
defect - a symbol the facade lacks is added to `dasllama/dasllama.das`, not escaped around.
A `require ... public` that re-exports an engine module OUT of an escaped file, beyond what
that consumer's ruled charter (`ARCHITECTURE.md` sec.1) grants, is the same defect wearing a
different line.

**Weakening `REVIEW.das` (beside this file) is a defect:** dropping a check, adding a name to
a check's licensed set - the names that check does not flag - or a finding text that no
longer names what failed. What the gate enforces is read from the gate itself; each check's
finding text states its own rule.

**A new `REVIEW.das` check ships its `ARCHITECTURE.md` sec.1 line in the same change: a line
naming the check and the names it licenses - the names that check does not flag - and saying
so when it licenses none.**

**An upstream mechanism is described in our own terms, not attributed** - no
"lifted/ported verbatim from" and no upstream symbol, header, or constant names in a `.md`
file or a `.das` comment; state what the code does and why its shape wins. Provenance is
not attribution: a path naming where checked-in data is regenerated FROM, an env-knob row
in `ENVIRONMENT.md` whose value locates the reference binary, and a command line or flag
list in `METHODOLOGY.md`, `PROFILE.md`, or `BRINGUP.md`, all name the binary outright;
every other `.md` line and `.das` comment writes "the reference exe" or "upstream". Legal
attribution lives in `THIRD_PARTY_NOTICES.md` and the `LICENSE.*` files, so prose never
carries it.

**A def of `dasllama/dasllama.das` - and a new OVERLOAD of one - is TAUGHT: demonstrated in
runnable code in a `tutorials/dasLLAMA/*.das` source and narrated on a
`doc/source/reference/tutorials/dasLLAMA_*.rst` page.** The gate matches def NAMES only, so
an overload passes on a sibling's tutorial - the reviewer confirms a tutorial calls the NEW
signature, and a mention that only names it (a comment, a passing reference) does not count.

**A NEW `[EnvConfig]` area struct is rendered by `env_markdown()` in the same change.** A
struct the renderer never emits is absent from `ENVIRONMENT.md` and invisible to every test;
a struct the renderer emits but the registry does not is caught by
`tests/test_env_registry.das`.

**`dasllama/dasllama_unicode.das`'s RANGES/WS tables are generated - retranscoded from
`$LCPP/src/unicode-data.cpp` (the reference checkout); hand-editing them is a defect.**

**A diff that adds a file under `dasllama/`, adds a file beside one that has its own sec.1
charter line, moves code between files, or changes what a file owns lands the sec.1 edit
that keeps the charters true, in the same change** (a module-root doc file - a ledger, a
plan, `LAWS.md` - has no charter line and lands free).

**A per-file inventory restated in this checklist is a defect of the checklist** -
`ARCHITECTURE.md` sec.1 owns the per-file list; a rule naming what KIND of code lands in
which file is the checklist's own.

**A tensor format conversion lands in `dasllama/dasllama_convert.das`.**

**A disk-order -> compute-order transform lands per scope: kernel-layout in
`dasllama/dasllama_repack.das`, load-scope in `dasllama/dasllama_layout.das`.**

**When placement rules disagree on one function, `ARCHITECTURE.md` sec.1's charter decides.**

**A CPU KV-cache store, read, score dot, or V-accumulate lands in `dasllama/dasllama_kv_codec.das`,
its format family kept whole.** GPU twins land in their backend kernel file.

**A pre-tokenizer split lands in `dasllama/dasllama_pretok.das`; a merge algorithm in its backend file
(`dasllama/dasllama_spm.das` / `dasllama/dasllama_bpe.das`).**

**A kernel body lands in its owner's backend file: a GPU kernel where its PSO is compiled
and released, a CPU-tier kernel in that tier's `dasllama/dasllama_math_<tier>.das`** - never
in `dasllama/dasllama_math.das` or a lens/dispatch macro file.

**A family quirk lands in the family file; a piece two families need moves UP into the
concern's shared file (its own file when none exists)** - never sideways into a sibling.

**A family gaining an arm for a media kind adds that kind's span markers - the template text
that opens and closes the media rows - to that family's chat template, never a second
renderer.** A family whose template or vocab lacks them has no arm for that media kind -
`create_chat_` panics at create, not at render.

**No signature in `dasllama/dasllama_tower.das` - the shared encoder-tower home - takes a
type `dasllama/dasllama_audio.das`, `dasllama/dasllama_vision.das`, or a family file
declares, and the file requires none of them.** A doc comment naming the family a helper was
built for is fine; the code stays family-blind.

**A `dasllama/dasllama_tower.das` helper with one calling family lands in that family's
file** - a single-caller helper sanctioned as tower-worthy is ledgered on `ARCHITECTURE.md`
sec.1's tower charter line, not argued in review.

**On the lane that serves the file's own planes, a weight plane's element type follows its
SOURCE tensors, per weight region - the set of source tensors a carrier stores in one plane
(a block stack, a merger/projector).**

**A weight region - the set of source tensors a carrier stores in one plane - whose source
tensors disagree on element type is refused in a message naming the offending tensor and
both element types.**

**A lane that PERSISTS a converted form of the file's planes - a form an image could carry -
is a separate flavor under its own image identity, and the load that picks it prints which
lane it picked.** A conversion made and dropped inside one forward pass persists nothing and
is not such a lane.

**A harness that prints output for another tool to compare fails loudly when it has nothing to
print.** A run that ends without its comparison lines - wrong flags, failed load - exits
non-zero.

**Tool wire text - building or parsing - is produced only in `dasllama/dasllama_tools.das`.**

**No engine file (`dasllama/`) other than `dasllama/dasllama_audio_io.das` requires `audio` (the
miniaudio decode module).**

**No engine file (`dasllama/`) other than `dasllama/dasllama_vision_io.das` requires `stbimage`.**
Benchmarks, harnesses, and tests decode their own fixtures.

**Engine, HTTP, or writer logic never lands in `dasllama/dasllama_scheduler.das`** - engine logic in
engine files, HTTP in the server, writer logic in the writer's own file.

**An `[init]`-only side-effect require in an engine file (`dasllama/`) lives in
`dasllama/dasllama_transformer.das`** - arch registrations, GPU tiers, every module requiring
the engine back; it sits in `dasllama/dasllama_common.das` only if engine code needs it. A
program root (test, harness, benchmark, tool) requires the registration module it needs
directly.

**An architecture file (`dasllama/dasllama_arch_*.das`) is declarative registration only.** An
architecture that changes a forward loop, or tests a family name on a shared path, is a defect.
