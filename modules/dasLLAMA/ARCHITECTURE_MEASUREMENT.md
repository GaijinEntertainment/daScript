# dasLLAMA Architecture - the benchmark rig and instrumentation rails

Companion to `ARCHITECTURE.md`; section numbers are that document's. The instruments that time
a kernel away from the served graph - the kernel race's fidelity conditions, the gemv's own
tune seat and the CPU kernel bench's fixture - are `ARCHITECTURE_MEASUREMENT_KERNEL_RACE.md`
sections 2.21, 2.26 and 2.27.

### 2.5 There is ONE benchmark rig, and the records are the baseline {#one-benchmark-rig}

`benchmarks/lcpp_bench.das` is the only thing that produces a recorded measurement. It is a
*mirror* of the upstream `llama-bench` - the same test shapes, rep counts and timing
boundaries, applied to our engine - so `pp` is one batched prefill of `-p` tokens from an empty
cache per rep and `tg` is `-n` single-token forwards with no logit read, each row one untimed
warmup plus `-r` timed reps. The rows' protocol lives in one place, `dasllama/dasllama_bench.das`
(the synthetic ids, the warmups, the timed reps, the warmup logit check, the row statistic), and
two drivers run it: `lcpp_bench` from its loop, and dasllama-server's in-process `/bench` one
step per tick on the model it serves. Only `lcpp_bench`'s rows become records: the server's are
a self-measure the operator reads on the control page, stamped with the device, the KV codec,
the exec tier and the tune state they ran under, and they enter no board, ledger or exchange.
The real `llama-bench` runs only when `--ref <path>` is passed; the upstream columns come from
that run, and they are pinned, not re-measured.

`performance/gen_bench_records.das` sweeps a board by spawning that rig once per cell, and
writes `performance/records/<box>.json`. `gen_site_records.das` merges those into the file the
site renders. A stored row carries its own command, sha, version, tune stamp and exec format, so
a number is self-describing rather than a bare figure in a table. The command, its environment
line and a sidecar's `binary` spell the home directory `~` (`tilde_home`, `daslib/fio`): a
public row names no user and still reproduces on any box. The re-mint rule sanctions one edit
to a stored row - spelling its home directory `~` - since no measurement changes. An archived
tune sidecar's bytes are otherwise untouched: its sha256 is what ties a row to the exe it
shipped with. A sidecar archived as `records/<box>.tune.<sha12>.json` is content-addressed: its
filename carries the hash of its bytes, so a re-stamp of `provenance.engine_sha` re-hashes and
renames the file, and a `records/<box>.json` row's `tune_sha` names that file by that hash.

**Regression checking inverts the same rig:** `gen_bench_records.das --oracle --legs metal`
takes the store's das rows as the work list, re-measures each once, and gates one-sided against
its stored mean (fail past 5%, warn past 3%, gains flagged as suspicious). Upstream never runs,
the store is never written, and a text cell's timed child runs `--frozen` - a prepare pass bakes
and warms its image first (the batch starts wiped), so the timed cell never converts; ASR and
image-chat cells bake what they need mid-cell, like their publishing legs. A second
harness with its own protocol would produce numbers that cannot be compared to any of this;
the server's self-measure shares the protocol and is still not a record, because its session,
its device and its tune state are whatever the operator's box serves.

**The tune stamp gates the comparison.** A manifest older than the binary fails every cell, and
an untuned invocation stamps the shipped class profile (a box the profile does not cover
re-execs into the residue race - a tuner run over the families the profile leaves uncovered)
rather than measuring. A delta taken against stored rows whose `tune` stamps name other winners
compares two tune states, not two engines.

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
`DECVEC` verdict comes from one instrument; every arm of the sweep dispatches over two alternating
output planes under a fresh hazard each, the form the served graph overlaps (the RTX 5080 rows the
ledger carries read the one-plane form, which serialized a chain on the plane's write-after-write
hazard). The `khrx` arm is the second axis for the KHR kq
tile. It runs eight arms: a resync copy of the shipped k4 tile with no lever moved
(`khrpx_ship`), five copies each with one lever moved back - the weight stage as a constant fill
or as the four-wide callback on the plane element, f32 accumulators, a 16-row strip tiling, and
the reference exe's 128-thread geometry - and the shipped class and the sdot4 tile as controls.
The resync copy is the row a lever's arm is read against, and its bit-exact reading against the
shipped class is what says the copies still track the shipped body. The sweep runs three whole
windows and one partial window of 300 tokens, the row that takes the edge store; the copies
stage and store whole tiles, so on the partial window only the two controls run.
`khrprof:<arm>` submits one arm alone for a GPU profiler. The `mmqx` arm is the first axis for
the integer tile: the sdot4 k4 tile against register-block prototypes over the same planes. Both
sweeps time the served graph's shape first: sixteen dispatches per submit over two alternating
outputs with a fresh hazard each, the arms interleaved round by round, an arm's figure its best
round. The comparison follows the timing: each compared arm's last output is read back and
measured against the sweep's reference - the shipped class in `khrx`, the sdot4 kq tile in
`mmqx`. An arm that stages constants, or reads its own activation fixture, is timing-only and is
never read back. A khrx copy passes within a 2e-2 relative difference of the shipped class, and
reads bit-exact where the lever leaves the arithmetic alone. The shipped class itself is read
against the k4 CPU oracle on six corners of the output, at the kernel cell's bar:
`|gpu - cpu| <= 2e-2 |cpu| + 4e-3 max|y|`. The probe's exit code is non-zero on a compared arm
over its bound, a CPU-oracle miss, an unknown `khrprof` arm, or a run that produced no result
row. The `moe:<fmt>` and `moesk:<fmt>` arms are the first axis over the expert schedule: the
format's cm2 s and e stamps over the Qwen3-30B-A3B window's routed buckets (512 tokens, 8 routed
of 128 experts, 4096 bucket rows) at the gate/up shape (d 768, K 2048) and the down shape (d
2048, K 768), dispatched to the bound the device schedule sizes with its sentinel tail, both
decode arms interleaved as `cm2:<fmt>` runs them. `moe:` gives every expert 32 rows - the
reference harness's uniform profile - and `moesk:` takes the 30B window's own skew (69 experts
route, nine holding the large buckets, eight at 96 rows, four at 48, the rest at 18). Each
profile runs two schedule forms and their alternates in one process: the whole buckets at the s
column, and the ladder, where a bucket past 32 rows takes whole 128-row columns on the e stamp
with the last one partial and a remainder of at most 32 rows past them takes the s column; the
ladder again on the dense 64-deep m stamp, and - on the formats that carry one - the whole
buckets on the s stamp's other-depth twin, the probe's own stamp of the format template at the
k step the engine does not run for that format (64 where the engine runs 32, 32 where it runs
64). `cm2:<fmt>` runs the same twin beside the s stamp on a 64-row window (`gate@64`),
the dense chain's s use, and the 35B's shared expert shapes (`shexp`: 512 rows over K 2048;
`shdown`: 2048 over 512), where the m column takes 16 and 64 workgroups on a 512-token window.
`cm2d:<fmt>` runs the same sweep over the 4096-wide dense hybrid's window shapes (the
Qwen3.5-9B: the FFN pair 12288 over K 4096, its down 4096 over K 12288, the fused qkv 8192, a z
or out plane 4096 and an attention k or v plane 1024, at 512 tokens); its alternate is the
reference exe's own rate on that model's window under its concurrent logger
(`GGML_VK_PERF_LOGGER=1 GGML_VK_PERF_LOGGER_CONCURRENT=1 llama-bench -p 512 -n 0`: the q5_K
pair 124 TFLOP/s and its down 105, q6_K 112 and 94, on the RTX 5080). `cm2w:<fmt>` is the wave
sweep: the l stamp over one 256-token column (K 4096) at half-wave steps of the device's SM count
from one wave to four, so the cost of a second wave says how many of the stamp's workgroups an SM
runs at once; its alternate is the one-wave row (the RTX 5080 runs one: k5 at 84 workgroups
0.170 ms, 126 0.333, 168 0.379, 252 0.562, 336 0.746 - a partial wave costs a whole one).
`dec` times the decode head's two small kernels in isolation, each as a chain of dispatches
over its own buffers under one hazard set (the serial form the token command runs them in): the
f16 beta/alpha GEMV at the 9B's, 0.8B's and 27B's shapes (64 rows over 4096, 32 over 1024, 64
over 5120) and the fused deltanet step at 32 and 16 heads of 128; its alternates are the
reference exe's decode logger rows on the same models (its two f32 m=32 GEMVs ~5 us each,
GATED_DELTA_NET 4.1 us + SSM_CONV 5.2 at one token, RTX 5080), and a whole-token profile role
that reads past the isolated figure names the chain around the kernel, not the kernel (the RTX
5080 reads the GEMV at 4.6 / 3.0 / 4.7 us and the step at 8.8 where the token profile bills the
roles 17 and 25). The decode GEMV probe (`harness/vk_gemv_probe.das <n> <d>`) streams a ring of
plane copies past the L2 for the DRAM rate; its `single` arm dispatches one plane copy under the
same hazard chain, so the row reads the chained dispatch's floor past its bytes (4.5 us on every
format at 0.6 MB planes, RTX 5080) and the served plane L2-warm - the two figures a token's GEMV
role sits between.
The reference row is `test-backend-ops perf MUL_MAT_ID` at `n_mats=128,n_used=8,m=768,n=512,k=2048`.
Two arms read the window chain's own overheads at those shapes rather than a tile: `ts:<fmt>`
dispatches two m stamps back to back into two planes - plain, with the profile's bottom-of-pipe
timestamp between them, with a barrier between them (the second writes the first's plane), and
with the split-k reduce between them on that plane - so a driver that drains the queue at a
timestamp reads the stamped pair at the barrier row (the Linux 580 driver does: 103 us plain
against 210 stamped and 209 with a barrier on the RTX 5080; the Windows 616 driver reads the
plain figure on all but the barrier row), and a pipeline change that costs the device a
reconfiguration reads the reduce row past the barrier row by more than the reduce (neither
driver does). `cold:<fmt>` runs the l, m and s stamps warm (one copy of the planes, L2-resident
across dispatches) and cold (a ring of 96 copies, past a 64 MB L2), the m stamp with its
schedule words in host memory (the prefill's meta) and over the engine's plane forms (the weight
planes at the far end of a 3 GB slab; the feed and output planes sized to an 8192-row window),
and the three stamps taking turns over the ring; the warm row is the alternate every other row
ranks against (the RTX 5080 reads the 64-workgroup m stamp at 26-27 us on every row). The arm's
flush row is the split-k reduce copying a 128 MB plane between two dispatches of one stamp, so
the next dispatch finds its code, descriptors and planes out of every cache - the weight a
window chain streams between two uses of one stamp; the flush writes the feed's region, so the
stamp after it waits on the flush and the next flush waits on the stamp.

**A measured number proves its kernel provenance through `tune_gate()`
(`performance/profile_common.das`), one arm per world it can run in.** Four worlds, because
`tune_status()` populates in two of them: a standalone exe checks the sidecar the release
shipped beside it; a fat exe (sec.2.42a) checks that every kernel's most capable active class
row is a profile stamp; a `DAS_TUNE_MANIFEST` run checks that file; a plain script checks
that every `[tune]` row stamps a manifest winner. An invocation no arm covers refuses - or
worse, measures on fallback kernels - which is why every measuring entry point calls the gate
before its first timed rep. Two rig shapes fall outside "measuring entry point" by the
property itself, ledgered here: a kernel A/B lab dispatches its variants through its own arms
(never the `[tune]` selection), and `lcpp_bench.das`'s `--tok` cell dispatches no kernels at
all - neither can measure a fallback silently. A kernel A/B lab is also outside the
in-process reference check: it dispatches shipped kernels and prototypes on timing fixtures,
compares an arm's output against its own reference arm where the outputs are comparable and
marks the rest `timing-only`; its rows never enter a record store, and a decision it seeds is
confirmed by the e2e board rows.

**A binary-stale sidecar still serves its `runtime` section; a foreign one serves nothing.**
The staleness rule discards measured kernel WINNERS - a rebuild can change the bodies they were
raced on - but the `runtime` knobs (lane caps, jobque shape, the `metal_tensor` crowns that turn
the tensor mul_mm twins on) are properties of the box, not of the binary. The engine's no-path
`apply_box_profile_runtime()` therefore takes the checked route: `stale_binary` applies the
runtime section and says so; `foreign_box` and `unreadable` apply nothing, because those knobs
are the minting box's state; `absent` is an unminted box. Without the crowns the twins run their
base forms - a prefill reads well under half its board cell - so `metal_decode_init`
warns when a profile was asked for, declined, and no crowns are set, and `lcpp_bench` stamps a
cell that passed `tune_gate()` on `DASLLAMA_ALLOW_UNTUNED=1` with an `untuned:` flavor prefix.

**The retune re-exec fires on scaffolding runs on a box its class profile does not cover, and
the pin that suppresses it is checked in.** A bare `daslang` run that requires the engine - a
probe, a one-off script, a REPL experiment - stamps the shipped class profile at compile time
and runs; on a box the profile does not cover it re-execs into the residue race when no manifest
is armed, and on either box it serves no runtime section (no Metal crowns - no profile ships
those). `performance/last_known_good_sidecar.json` exists for exactly that: a frozen copy of a
complete, noise-gated mint, tracked in git (the `*.tune.json` ignore rule deliberately does not
match it). Point `DAS_TUNE_MANIFEST` at it and the framework never retunes; on a different box
the identity mismatch just serves fallbacks, and a copy minted before the current
`DASLLAMA_RELEASE` serves fallbacks on any box - the compile says which with one
`WARNING DAS_TUNE_MANIFEST` line per scope. That is the whole contract - it suppresses the
re-exec, it does not tune the box, and a number measured under it is not a benchmark. Benches
and the rig keep minting their own.

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
stages, tokenizer build) - the rails do not apply. A timing that is part of an API's answer -
the facade's `TtsTimings`, the per-stage walls a synthesis returns to its caller and the
server logs per request - is a deliverable of the same kind, not instrumentation. A clock whose
value feeds logic is control flow, not instrumentation; it is marked `// clock: control` so the
sweep and any future lint leave it alone.

The override-announce rule (REVIEW.md) draws its boundary here: a knob or setter whose purpose
is timing still counts as an override when it moves computed numerics - two GEMM forms of the
same math differ in float terms - while one that changes only WHEN work happens does not, and
a CLI flag is never an override (it is the run's own command line, visible where the run is
launched).

### 2.20 The ASR board's GPU row pairs {#asr-gpu-pairs}

The das Metal ASR leg is OPT-IN per catalog row: `AsrModelSpec.metal_served`
(`performance/profile_common.das`) declares that the Metal driver serves that family end to
end - tower and decoder both. An unflagged family keeps the CPU by design, and asking for its
GPU leg reds the row: the `--ngl` arms assert that the tower engage counters moved, so a family
whose tower silently falls back fails instead of publishing a CPU wall under a GPU heading.

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
is guarded on the patched marker already being in the tree, and runs three-way, so it still
applies after the reference pin moves. On Apple boxes `performance/setup_asr_rig.das` builds a
second, Metal-ON copy of the same patched checkout, because `-ngl` on a Metal-OFF build is
inert; `mtmd_bin_metal()` returns "" when it is absent and the GPU reference leg skips loudly.

### 2.40 A `[tuned]` kernel's perm is decided at its own compile {#tuned-perm-precedence}

`dasllama_tune.das` picks one perm per `[tuned]` kernel, first match wins: the `reference` tune
policy (the kill switch - the template's own loops, no hints stamped), a `perm=` pin on the
annotation, this box's sidecar `"kernels"` entry, this box's class entry in the shipped defaults
profile (`performance/defaults`), the annotation's `fallback` `;`-chain, then `DEFAULT_PERM`.
`tune_kernel_pick` (llvm_tune) reads the sidecar and the profile in that order and hands back the
FILE its answer came from, so a verbose compile names which of the two stamped each kernel. A box
the shipped profile covers therefore compiles tuned kernels without racing anything, and a box it
does not cover falls to the fallback chain - never to another box's winners. With no tune
framework in the build the first three steps do not exist - no policy env, no sidecar, no
profile - so the ladder is the `perm=` pin, then the `fallback` chain's first UNCONDITIONAL
entry (a `suffix:requires` seat cannot be judged with no feature probe), then `DEFAULT_PERM`,
and the compile reports nothing.

### 2.41 The mint's own wall rides in the sidecar's provenance {#mint-wall-provenance}

A successful mint stamps three provenance keys into the sidecar before the archive copy is
written: `mint_gen_ms` (the generator half), `mint_kernels_ms` (the loop-hint half) and
`mint_total_ms` (the wrapper's own wall). The order is what makes the timestamped copy in the
box's tune-history directory carry them too, so the box's longitudinal record answers what
shipping a profile cost without re-running the mint - a console line that scrolled past is not
that record.

### 2.42a A fat exe races its runtime section at first start {#fat-first-start}

A fat exe (`DAS_TUNE_MODE=fat`, `modules/dasLLVM/ARCHITECTURE_TARGET_FEATURES.md` sec.11) ships
its kernels baked per CPU class and carries no tuner and no policy rail, so nothing would ever
mint the sidecar's `"runtime"` section - the Metal twin crowns among its knobs, a 2-4x
kernel-form gain of a tensor twin over its simdgroup kernel per twin-race row on the M5 Max
(`harness/tune_kernels.das`, the metal_crowns family) - and a shipped Mac exe would run
uncrowned forever. The section needs no rebuild, so the exe mints it itself:
`dasllama_fat_start` registers `dasllama_fat_first_start` with the box-profile apply
(`set_runtime_race_hook`) from its `[init]`, and the engine umbrella (`dasllama_transformer`)
requires the module so every engine program carries the registration - the shipped bench
requires the umbrella, never the facade; `apply_box_profile_runtime_checked` fires the hook when
the sidecar is absent, another box's, or carries no runtime section, then reads the file the
hook wrote. The hook answers false outside a fat exe (`tune_fat_built()`); inside one it runs
`dasllama_race_runtime_section`: the Metal twin races (`dasllama_metal_crown_race` - both halves,
synthetic, no model) under the tune progress display, then `dasllama_runtime_snapshot` - the same
writer the mint's kernel half ends with - merged into the app sidecar beside the exe with the
kernels section untouched. The next start reads it and races nothing. A box without a Metal
device records the knob defaults, so the file still documents the box. An unwritable location
keeps the crowns for the process and says so; `DAS_TUNE_MANIFEST` moves the file.

What a first start never does: load a model, spawn a child, or race a kernel. The tuner's
confirms - the generator half's end-to-end prefill A/B, the kernel half's serving and MTP depth
confirms - each spawn a daslang child on a harness script and a vehicle model, and they are the
harness's alone; under `harness/dasllama_tuner.das` on the M5 Max the confirms take 147 s of the
metal_crowns family's 161 s, the twin race itself 14 s.

### 2.45 The speculative round's cell is a ruler record {#ruler-records}

**`performance/records/mtp/mtp_<box>_<model>[_variant].json` is a ruler record: one file per
box and model, written only by `harness/mtp_ruler.das`.** The board (`records/<box>.json`) has
no speculative column, because a speculative rate is not one engine's number: acceptance is a
property of the text and of the drafter both engines share, so the honest cell is the two
engines on the identical rendered prompt in one run. The ruler measures our released exe FIRST
from a parent that has loaded nothing (a parent that had just run the engine in-process read
the exe's speculative arm a fifth low), then the reference server at the ref pin, every arm
settled, and writes `meta` (date, box, `das_sha`, `das_exe`, `lcpp_server`, `lcpp_version`, the
model and head with their shas, the corpus, `ngen`, `reps`, `depths`) plus one row per engine,
depth and prompt. The shape is the ruler's, not the board's - `list_record_stores` and the
records gate read `records/` one level deep and never see the folder - and `mtp_ruler --render
<record>` prints the table. Every third-party wall in the file is the other half of a pair
taken in that run.
