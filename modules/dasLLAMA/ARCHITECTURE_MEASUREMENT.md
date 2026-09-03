# dasLLAMA Architecture - the benchmark rig and instrumentation rails

Companion to `ARCHITECTURE.md`; section numbers are that document's.

### 2.5 There is ONE benchmark rig, and the records are the baseline {#one-benchmark-rig}

`benchmarks/lcpp_bench.das` is the only thing that measures performance. It is a *mirror* of
the upstream `llama-bench` - the same test shapes, rep counts and timing boundaries, applied to
our engine - so `pp` is one batched prefill of `-p` tokens from an empty cache per rep and `tg`
is `-n` single-token forwards with no logit read, each row one untimed warmup plus `-r` timed
reps. The real `llama-bench` runs only when `--ref <path>` is passed; that is how the upstream
columns were produced, and they are pinned, not re-measured.

`performance/gen_bench_records.das` sweeps a board by spawning that rig once per cell, and
writes `performance/records/<box>.json`. `gen_site_records.das` merges those into the file the
site renders. A stored row carries its own command, sha, version, tune stamp and exec format, so
a number is self-describing rather than a bare figure in a table. The command, its environment
line and a sidecar's `binary` spell the home directory `~` (`tilde_home`, `daslib/fio`): a
public row names no user and still reproduces on any box. The re-mint rule sanctions one edit
to a stored row - spelling its home directory `~` - since no measurement changes; an archived
tune sidecar is never edited, its sha256 being what ties a row to the exe it shipped with.

**Regression checking inverts the same rig:** `gen_bench_records.das --oracle --legs metal`
takes the store's das rows as the work list, re-measures each once, and gates one-sided against
its stored mean (fail past 5%, warn past 3%, gains flagged as suspicious). Upstream never runs,
the store is never written, and a text cell's timed child runs `--frozen` - a prepare pass bakes
and warms its image first (the batch starts wiped), so the timed cell never converts; ASR and
image-chat cells bake what they need mid-cell, like their publishing legs. A second
harness would produce numbers that cannot be compared to any of this.

**The tune stamp gates the comparison.** A manifest older than the binary fails every cell, and
an untuned invocation re-execs into a full retune rather than measuring - so re-mint the box
manifest and check its winners against the stored rows' `tune` stamps before trusting a delta.

**The Vulkan GEMM probe attributes prefill GEMM cost on three axes.**
`harness/vk_gemm_probe.das` times one shape at a time: the serving GEMM against its alternates
on the dense role shapes (gate/up, down, q/wo, k/v - the mm_a kernel against the cm2 l and m
tiles, the sdot4 kq tile against the k4 and k6 cm2 tiles); one decode callback against
spellings of itself with the rest of the tile held fixed (the `cm2x` and `k6x` bisect arms);
and our tile against the upstream coopmat2 GEMM blob, served in place of a probe class's body
through `DASLLAMA_VK_SPV_OVERRIDE` (the `ref` arm). The `cm2:<fmt>` arm generalizes the first
axis to any kq superblock format: it drives the prefill's own (format, column) ladders for the
l and m columns with the kq batch tile as the control row, over random block bytes at that
format's device block size, and it runs the four-wide decode's two arms (the twin served, then
stripped through `vkd_pipes_rebuild`) interleaved in one process, two rounds each, so a format's
`DECVEC` verdict comes from one instrument. A new arm joins one of the three.

**A measured number proves its kernel provenance through `tune_gate()`
(`performance/profile_common.das`), one arm per world it can run in.** Three worlds, because
`tune_status()` populates in exactly one of them: a standalone exe checks the sidecar the
release shipped beside it; a `DAS_TUNE_MANIFEST` run checks that file; a plain script checks
that every `[tune]` row stamps a manifest winner. An invocation no arm covers refuses - or
worse, measures on fallback kernels - which is why every measuring entry point calls the gate
before its first timed rep. Two rig shapes fall outside "measuring entry point" by the
property itself, ledgered here: a kernel A/B lab dispatches its variants through its own arms
(never the `[tune]` selection), and `lcpp_bench.das`'s `--tok` cell dispatches no kernels at
all - neither can measure a fallback silently. A kernel A/B lab is also outside the
in-process reference check: `harness/vk_gemm_probe.das` dispatches the shipped, suite-gated
kernels on timing fixtures, compares no arm's output, and marks every row `timing-only`; its
rows never enter a record store, and a decision it seeds is confirmed by the e2e board rows.

**A binary-stale sidecar still serves its `runtime` section; a foreign one serves nothing.**
The staleness rule kills measured kernel WINNERS - a rebuild can change the bodies they were
raced on - but the `runtime` knobs (lane caps, jobque shape, the `metal_tensor` crowns that turn
the tensor mul_mm twins on) are properties of the box, not of the binary. The engine's no-path
`apply_box_profile_runtime()` therefore takes the checked route: `stale_binary` applies the
runtime section and says so; `foreign_box` and `unreadable` apply nothing, because those knobs
are the minting box's state; `absent` is an unminted box. Without the crowns the twins run their
base forms - a prefill reads well under half its board cell - so `metal_decode_init`
warns when a profile was asked for, declined, and no crowns are set, and `lcpp_bench` stamps a
cell that passed `tune_gate()` on `DASLLAMA_ALLOW_UNTUNED=1` with an `untuned:` flavor prefix.

**The retune re-exec bites scaffolding, and the pin for it is checked in.** Any bare `daslang`
run that requires the engine - a probe, a one-off script, a REPL experiment - re-execs into a
full retune when no manifest is armed. `performance/last_known_good_sidecar.json` exists for
exactly that: a frozen copy of a complete, noise-gated mint, tracked in git (the `*.tune.json`
ignore rule deliberately does not match it). Point `DAS_TUNE_MANIFEST` at it and the framework
never retunes; on a different box the identity mismatch just serves fallbacks, and a copy minted
before the current `DASLLAMA_RELEASE` serves fallbacks on any box - the compile says which with
one `WARNING DAS_TUNE_MANIFEST` line per scope. That is the whole
contract - it suppresses the re-exec, it does not tune the box, and a number measured under it
is not a benchmark. Benches and the rig keep minting their own; refresh the copy when a
re-mint moves the crowns or `DASLLAMA_RELEASE` bumps.

### 2.20 The ASR board's GPU row pairs {#asr-gpu-pairs}

The das Metal ASR leg is OPT-IN per catalog row: `AsrModelSpec.metal_served`
(`performance/profile_common.das`) declares that the Metal driver serves that family end to
end - tower and decoder both. An unflagged family keeps the CPU by design, and asking for its
GPU leg trips the anti-sandbag: the `--ngl` arms assert that the tower engage counters moved,
so a family whose tower silently falls back reds its row instead of publishing a CPU wall
under a GPU heading.

Three reference tools carry a GPU arm the board pairs against a das Metal row, each with its
own spelling: the whisper reference exe takes `-ngl`, the media-chat reference exe takes
`-ngl 99`, and the NeMo bench script takes `--device mps`. The remaining two reference legs
have no pair - the parakeet exe measures slower on the GPU, and the ONNX export is CPU-only -
so their das rows stand alone in the CPU category.

The media-chat reference exe is built as the bench exe's sibling in one reference worktree:
`benchmarks/setup_lcpp_ref.das` builds both targets, because a bench-only build leaves the
image and audio-chat cells with no binary and the board quietly mints das-only rows. That
sibling needs the timing patch beside it (`benchmarks/asr/patches/`) - the record parser reads
its per-rep timing lines, and an unpatched sibling mints "no rep parsed" failures. The apply
is guarded on the patched marker already being in the tree, and runs three-way so it rides
pin drift. On Apple boxes `performance/setup_asr_rig.das` builds a second, Metal-ON copy of
the same patched checkout, because `-ngl` on a Metal-OFF build is inert; `mtmd_bin_metal()`
returns "" when it is absent and the GPU reference leg skips loudly.

### 2.10 Sanctioned instrumentation rails

Engine timing goes through the rails that aggregate and tag it: the `jobque_profile` markers
(`profile_tag` / `profile_marker` and the `trace_*` wrappers in `dasllama_math.das`), the
`prof_add` / `forward_profile_*` decode buckets (`dasllama_common.das`), the `asr_prof_add`
encode buckets (`dasllama_audio.das`), and the Vulkan tier's `vk_prof()`-gated ledgers
(`DASLLAMA_GPU_PROF=1`; per-dispatch and per-submit sums the tier prints at its own cadence -
a report leg, never the served path). A new clock read paired with a print of the elapsed
interval bypasses the aggregation - it measures one call site once, is not filterable, and rots
where a rail entry would keep serving. Where a timed line IS the deliverable - `benchmarks/`,
`performance/`, `harness/`, and cold one-shot load/mint progress logs (image bake/map, load
stages, tokenizer build) - the rails do not apply. A clock whose value feeds logic is control
flow, not instrumentation; it is marked `// clock: control` so the sweep and any future lint
leave it alone.

The override-announce rule (REVIEW.md) draws its boundary here: a knob or setter whose purpose
is timing still counts as an override when it moves computed numerics - two GEMM forms of the
same math differ in float terms - while one that changes only WHEN work happens does not, and
a CLI flag is never an override (it is the run's own command line, visible where the run is
launched).


### Re-stamping inside the content-addressed archive

A sidecar archived as `records/<box>.tune.<sha12>.json` is content-addressed: its filename
carries the hash of its bytes. Re-stamping such a file's `provenance.engine_sha` to a reachable
commit (the remedy `performance/REVIEW.md` allows when the measured `modules/dasLLAMA/` tree is
byte-identical) therefore re-hashes and renames the file, and every `records/<box>.json` row
whose `tune_sha` named the old file is repointed in the same change - a row left on the old
name points at a file that no longer exists.

### 2.21 An isolated kernel race is only as good as the graph it imitates {#kernel-race-fidelity}

A kernel A/B race times two spellings of one compute on a synthetic fixture. Three conditions
decide whether its winner is the winner the served graph would pick, and a race missing any of
them crowns confidently and wrongly:

- **Overlap.** The served graph's consecutive GEMVs write DIFFERENT output buffers, so the
  scheduler overlaps them. A race chaining every dispatch through one output buffer serializes
  on the write-after-read hazard; on the iq2xxs f4-slab twin that one difference read +9% for
  an arm the served graph rejects at -8.6%.
- **A warmed clock.** An Apple GPU's clock governor ramps under load, and `race_pair_ms` runs
  base-then-twin per round, so the first side pays the ramp the second rides. About 150 ms of
  GPU work before the first timed round removes the bias; back-to-back dispatches inside each
  timed encoder hold the clock there. A sparse invocation without the burn reads idle-clock
  times and can flip a verdict outright.
- **A real site shape.** `REVIEW_GPU.md` binds this one.

A race arm owns a transient command queue for its timed pairs and releases it before returning -
the one exception to the family's shared device and queue (`ARCHITECTURE_GPU.md` sec.1.5) - so
the tune-time race never queues behind served work.

Even a race meeting all three can be structurally blind. `kq_gemv_iq2xxs_f4` is the standing
case: every isolated regime crowns the twin and the served decode graph loses 8.6% with it,
because the effect is mixed-stream occupancy - it exists only when the GEMV runs beside the
rest of the token step. That crown is therefore NOT auto-raced. It is minted on the real shapes
by `harness/tune_kernels.das`'s serving confirm: two temporary manifests differing only in
`runtime.metal_tensor` membership, each served to
`benchmarks/lcpp_bench.das --for-debug-purposes --ngl 99 -p 32 -n 128 -r 3` under
`DAS_TUNE_MANIFEST`, the tg128 line the verdict. Both arms carry an EMPTY `kernels` section, so
they stamp identical fallbacks and the tg delta isolates the crown under test. The margin is
`CONFIRM_TG_MARGIN` = 1.005, best-of-3: the crown's serving win where it wins is +0.6% (m5) and
its serving loss where it loses is -8.6% (m4), so the margin only has to clear run noise, and a
spuriously minted crown costs a re-mint rather than a board row. No IQ2_XXS vehicle on the box
means no crown - the base kernel is the safe side - and the run says so with a provisioning
hint.

The per-format isolated rig is `benchmarks/matmul/bench_metal_kq_race.das`: synthetic planes,
no model, no tuner, every arm gated against a CPU plane-dequant oracle before it is timed, and
`--burn-ms` (default 150) spent on GPU work before each cell's first timed round. Its cells
chain every dispatch through ONE shared output buffer on purpose - the serialized regime is
the instrument's probe shape, imitating the reference tool it is compared against - and its
numbers reach the engine only through a human porting decision, never a minted crown.

### 2.26 The gemv takes its own tune seat {#gemv-seat}

A kq family's manifest entry is its tile-best row, and the gemv gets a SECOND entry when a different
row serves the streamed decode better. Only same-mr rows can differ, because the layout companion
pins the plane's interleave; of those the two best by tile time race, the winner takes the gemv only
by the margin over the tile winner's own gemv, and the incumbent keeps a tie. Every family's perm grid
therefore carries a 256-wide `mr = 16` alternate beside its 512-wide tile crown. The seat is decided
at the engine's decode shape - a DRAM-bound plane streamed by every lane through the engine's own
splitter - because the engine's row length moves the answer (k3 on Granite Rapids: the 256 seat wins
at n=2048 and loses at 14336 - `benchmarks/matmul/kq_kernel_bench.das`, tune mode, seats pinned, d=32768). The seat fixture is a 512-row build at the ffn width tiled 320 times,
past the largest L3 a socket lends a slice of, and the seat takes the MEDIAN of seven rounds: a round
that finds the plane in L3 must not crown it. In normal mode `llvm_tune` stamps a companion from its
own manifest entry when one exists and is a perm this box can run, else from the tile's.

### 2.27 The CPU kernel bench's fixture conditions {#cpu-kernel-bench-fixture}

`benchmarks/matmul/kq_kernel_bench.das` times raw kernels on synthetic planes, and three fixture
properties decide whether its numbers mean anything. Every plane of one format lives in ONE arena at
fixed offsets, staggered so no two starts share their low 12 address bits: the heap places separate
arrays at run-dependent relative addresses, and planes that alias in the L1/L2 set logic make a run's
time depend on where the heap put them. Scale planes are filled with a byte that is a normal number in
every scale form, never random bytes, because denormal math runs orders of magnitude slower. Each row
is warmed before it is timed - three unmeasured rounds solo, six dispatches per row on the team arm -
because a core ramps over several rounds and one warm call is not enough. The q8 row exists in two
flavors: f32 group scales (the engine's own quantization) and `q8s16` over binary16 scales - the
wscale_f16 rail a GGUF q8_0 tensor runs, and the like-for-like row against the reference's q8_0.
Provenance for every figure in this section: `benchmarks/matmul/kq_kernel_bench.das` under
`DAS_TUNE_MODE=tune`, one thread, its default `--fmt` / `-n` / `-d` shape.

### 2.28 The speculative round's cell is a ruler record {#ruler-records}

**`performance/records/mtp/mtp_<box>_<model>[_variant].json` is a ruler record: one file per box
and model, written only by `harness/mtp_ruler.das`.** The board (`records/<box>.json`) has no
speculative column, because a speculative rate is not one engine's number: acceptance is a property
of the text and of the drafter both engines share, so the honest cell is the two engines on the
identical rendered prompt in one run. The ruler measures our released exe FIRST from a parent that
has loaded nothing (a parent that had just run the engine in-process read the exe's speculative arm
a fifth low), then the reference server at the ref pin, every arm settled, and writes `meta` (date,
box, `das_sha`, `das_exe`, `lcpp_server`, `lcpp_version`, the model and head with their shas, the
corpus, `ngen`, `reps`, `depths`) plus one row per engine, depth and prompt. The shape is the ruler's,
not the board's - `list_record_stores` and the records gate read `records/` one level deep and never
see the folder - and `mtp_ruler --render <record>` prints the table. A third-party wall lives here
only as the other half of a pair taken in the same run.
