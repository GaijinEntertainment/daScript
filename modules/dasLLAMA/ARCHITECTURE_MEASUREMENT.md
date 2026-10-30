# dasLLAMA Architecture - the benchmark rig and instrumentation rails

Companion to `ARCHITECTURE.md`; a section is cited by its anchor. The instruments that time a kernel
away from the served graph and the timing-race vocabulary are
`ARCHITECTURE_MEASUREMENT_KERNEL_RACE.md#timing-race-vocabulary`; the Vulkan GEMM probe's arms,
shapes and alternates are `ARCHITECTURE_MEASUREMENT_VK_GEMM_PROBE.md#vk-gemm-probe`; a fat exe's
first-start race and where it keeps its sidecar are
`ARCHITECTURE_MEASUREMENT_FAT_START.md#fat-first-start`; the client clock around a live server's
turns is `ARCHITECTURE_MEASUREMENT_SERVED.md#served-turn-instrument`; the reference build's forced
feed beside the gemma feed cells is `ARCHITECTURE_MEASUREMENT_FORCED_FEED.md#reference-forced-feed`;
the ASR board's GPU row pairs (`ARCHITECTURE_MEASUREMENT_AUDIO.md#asr-gpu-pairs`), the MLX ASR
reference (`ARCHITECTURE_MEASUREMENT_AUDIO.md#mlx-asr-reference`), the TTS reference instrument
(`ARCHITECTURE_MEASUREMENT_AUDIO.md#the-tts-reference-instrument`), its CUDA provider and g2p-venv
runs (`ARCHITECTURE_MEASUREMENT_AUDIO.md#tts-ref-g2p-venv`) and the ASR board's CPU references
(`ARCHITECTURE_MEASUREMENT_AUDIO.md#asr-cpu-references`) are the audio companion's; a board row's
release boundary is `ARCHITECTURE_MEASUREMENT_RELEASE.md#board-rows-release-boundary`; the llama.cpp
rows a GPU home's records read against are `ARCHITECTURE_MEASUREMENT_REFERENCE_ROWS.md#vulkan-reference-rows`
and `ARCHITECTURE_MEASUREMENT_REFERENCE_ROWS.md#cuda-reference-rows`.

### There is ONE benchmark rig, and the records are the baseline {#one-benchmark-rig}

`benchmarks/lcpp_bench.das` is the only thing that produces a recorded measurement. It is a
*mirror* of the upstream `llama-bench` - the same test shapes, rep counts and timing
boundaries, applied to our engine - so `pp` is one batched prefill of `-p` tokens from an empty
cache per rep and `tg` is `-n` single-token forwards with no logit read, each row one untimed
warmup plus `-r` timed reps; its `tg128@N` row (`--npl N`, its streams' prompt length `--npl-plen`
and its rep count `--npl-reps`, defaulting to the flat rows' -p - 512 when the pp row is off - and -r) mirrors the reference's batched bench
at that parallel count - N streams served together through the scheduler, device-home where a
whole-model driver homes that many and host-cached otherwise (the Metal batched driver and the CPU
batched stack take host-cached rows), the rate their served tokens summed over the step wall. The
row stands only where every timed step was ONE batched dispatch: a step that ran its rows one at
a time, or that a device-served model ran on the CPU batched stack, refuses the row by name -
`dasllama_batch.das`'s step census counts the three ways a step is served - so a per-row fallback
never posts a number under the batched label, whatever its rate. The batched row's clock covers
the scheduler step whole - the sampling, the detokenization and the event list inside it - where
the reference exe times its decode call alone, so a ratio taken between the two reads conservative
for ours. The row's prefills and first tokens run before the clock starts, as the reference times
PP apart, and each of its streams decodes to `plen + npl + ngen` positions - the context a driver
sizes before it opens the row's session, times `depth + 1` for the self-speculative arm, whose
streams emit up to that many tokens a tick and end by length inside the timed steps on a plain
cap. The row's reference is `llama-batched-bench` beside the
`--ref` binary (`setup_lcpp_ref.das` builds both), `--npl-reps` runs of its `-npl N -npp <--npl-plen>`
row folded to one `tg128@N` test on the reference run. The row's rate is the tokens the timed steps emitted over
their wall, which is `npl x ngen` for plain decode; under `--npl-mtp` the streams tick through the
self-speculative round (every stream's verify rows in one joint pass), the accepted drafts count,
and the reference row stays plain decode - the comparison reads what a served speculative stream
delivers against the reference's best. The rows' protocol lives in one place,
`dasllama/dasllama_bench.das` (the synthetic ids, the warmups, the timed reps, the warmup logit
check, the row statistic; the streams' KV codec is a parameter, f16 on the board), and
three drivers run it: `lcpp_bench` from its loop, dasllama-server's in-process `/bench` one
step per tick on the model it serves, and `harness/batch_decline_census.das`, which runs one row
per carrier under a named codec to read whether the step served or declined, never its rate.
Only `lcpp_bench`'s rows become records: the server's are
a self-measure the operator reads on the control page, stamped with the device, the KV codec,
the exec tier and the tune state they ran under, and they enter no board, ledger or exchange.
The real `llama-bench` runs only when `--ref <path>` is passed; the upstream columns come from
that run, and they are pinned, not re-measured.

A run's flat rows and its tg-real rows share one session context, and the flat rows never
narrow it. `flat_row_plen` sizes the flat session at `plen` prompt rows - `plen + ngen`
positions, as `llama-bench` does - except beside a tg-real row, whose own prompts already
capped the context at the row's size: there it reads that cap back as `row_ctx - ngen` prompt
rows. A flat session sized under the real row's prompts leaves the speculative round's context
gate reading a context every prompt exceeds, so the round stays cold and the run's speculative
arm drafts nothing.

`performance/gen_bench_records.das` sweeps a board by spawning that rig once per cell, and
writes `performance/records/<box>.json`. `gen_site_records.das` merges those into the two files
the site renders: the full merge, and its projection minus the receipt fields (`SiteModel`) that
the page paints first, reading the full store only when a receipt opens. A stored row carries
its own command, sha, version, tune stamp and exec format, so a number is self-describing
rather than a bare figure in a table. The command, its environment
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

**A measured number proves its kernel provenance through `tune_gate()`
(`performance/profile_common.das`), one arm per world it can run in.** Four worlds, because
`tune_status()` populates in two of them: a standalone exe checks the sidecar the release
shipped beside it; a fat exe (`ARCHITECTURE_MEASUREMENT_FAT_START.md#fat-first-start`) checks that every kernel's most capable active class
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
`WARNING DAS_TUNE_MANIFEST` line per scope. It suppresses the re-exec and tunes nothing: a number measured under it is not a benchmark, and the benches and the rig keep minting their own.

**A warm re-run that reads slower than the cold one is heat, not map warming.**
`gen_bench_records.das` re-runs a cell whose cv exceeds 3% once, warm, and compares the two runs
per board axis - pp against pp, tg against tg, because one summed rate IS a pp number and would
let pp carry a tg regression. The warm cell is kept only when it reads at least as fast on BOTH
axes; slower on either keeps the COLD cell and logs `HEAT`. That rate verdict is taken BEFORE the
variance break, because a heat-soaked re-run lands low with a clean cv and a cv-first break would
keep it; map warming can only make a re-run faster, so a slower axis says the box was still shedding the previous cell's heat and `--das-settle` is short for that tier.

**The two decode rulers beside the GEMV probe read against rows of their own run.** Every figure
of ours in this paragraph is the pod's (Linux, RTX PRO 4500, `daslang -jit`, the vulkan tier in
its cm2 mode, kv f16), each probe at its defaults - the attention ruler at 4 rows, 640 positions,
head 64, 32 heads and 4 heads a kv head, the copy ruler at its four sizes.
`harness/vk_attn_probe.das [rows] [cnt] [hs] [heads] [kv_mul]` times the token command's attention
pass a layer on the device clock across the key-split ladder - a held-output chain: sixteen
dispatches into one output buffer, each behind the last, so a figure is a dependent chain's cost
a layer, never one dispatch's latency; a row's alternates are the other
splits of the same run, and the reference-engine row is llama.cpp's `FLASH_ATTN_EXT` line from
`GGML_VK_PERF_LOGGER=1 llama-batched-bench -m <gguf> -c 4096 -b 2048 -ub 512 -npp 512 -ntg 128 -npl 4 -ngl 99 -fa on`
(b10660 on the RTX PRO 4500 at Llama-3.2-1B's shape, four rows: 15.7 us a layer, its split-k
reduce inside). The ruler reproduces the token command's stamps for the shipped pass and is the
only clock that separates a change's fixed cost from its per-key cost; the token profile
(`DASLLAMA_GPU_PROF=1`, itself ~5% of the rate) stays the number a kernel is judged by, since the
ruler's static fixture has read a retired pass three times off its profile. `harness/vk_dma_probe.das`
times one device-to-host copy at four sizes on the compute queue by the device clock and on the
transfer queue by the host clock in one run: each queue's row is the other's alternate, and the pod's 4.3 GB/s against 19-27 is the whole verdict.

### The host-experts reference row {#host-experts-reference}

A MoE whose expert stacks do not fit the card beside its other planes is measured against
llama.cpp with the same split: `llama-bench -m <shard 1> -ngl 99 -ncmoe <n_layers> -t <threads>
-p 512 -n 128 -r 3` keeps every layer's routed experts on the CPU and the rest of the model on
the device, the shape the resident driver's split token command serves. The row is external
(`REVIEW_MEASUREMENT.md`): it names the build tag, the backend build (Vulkan or CUDA) and the
driver version beside the figures, and `-ngl 99` alone is not the reference - it streams the
experts across the link every batch and measures the link. Where the card holds some routed layers'
experts and the plan moves the rest to the host, the reference keeps the same count there: `-ncmoe
<the plan's host layers>` - the count off our load's host-layers line - with `-fa 1` beside it on the
CUDA build (`ARCHITECTURE_MEASUREMENT_REFERENCE_ROWS.md#cuda-reference-rows`, that build's arm), so the row
reads `llama-bench -m <gguf> -ngl 99 -fa 1 -ncmoe <n> -t <threads> -p 512 -n 128 -r 3`; a row under
another count compares two splits, not two engines.

### The committed records gate {#committed-records-gate}

`gen_bench_records.das` writes `performance/records/<box>.json`, and `RETIRED_BOXES`
(`dasllama/dasllama_exchange_schema.das`) is the one list of boxes whose boards are history - the
records and the ladder keep their rows, the site projection reads the list to keep them off the
cards. `performance/REVIEW.das` checks the records' tree-decidable provenance:
- `check_records_corpus` - every store parses, every run's `hardware.remote_desktop` is `off` or
  absent, every archived sidecar's `provenance.noise` is `ok` or absent. Licenses no names.
- `check_das_row` - a `das` row is not past `DASLLAMA_RELEASE`, and below it only on a box
  `RETIRED_BOXES` lists - the licensed set, boxes no release re-mints; a current-release row
  carries `hardware.remote_desktop` and a `sha` HEAD reaches whose `dasllama_version.das` reads
  the same release.
- `check_row_archive` - a current-release row's `tune_sha` names an archive that exists, pins
  the current release, and stamps an `engine_sha` HEAD reaches. Licenses no names.
- `check_defaults_profiles` - a `defaults/` profile is a quiet mint at the current release with
  a `kernels` section and no `engine_sha`. Licenses no names.
- `check_ruler_records` - a `records/mtp/` file names no home directory. Licenses no names.
- `main` - `dasllama_version.das` declares a readable `DASLLAMA_RELEASE`. Licenses no names.

### Sanctioned instrumentation rails {#sanctioned-instrumentation-rails}

Engine timing goes through the rails that aggregate and tag it: the `jobque_profile` markers
(`profile_tag` / `profile_marker` and the `trace_*` wrappers in `dasllama_math.das`), the
`prof_add` / `forward_profile_*` decode buckets (`dasllama_common.das`), the `asr_prof_add`
encode buckets (`dasllama_audio.das`), and the Vulkan tier's `vk_prof()`-gated ledgers
(`DASLLAMA_GPU_PROF=1`; per-dispatch and per-submit sums the tier prints at its own cadence -
a report leg, never the served path) with the resident prefill's host-side report the same knob gates in `dasllama_gpu_resident.das`. A new clock read paired with a print of the elapsed
interval bypasses the aggregation - it measures one call site once, is not filterable, and rots
where a rail entry would keep serving. Where a timed line IS the deliverable - `benchmarks/`,
`performance/`, `harness/`, and cold one-shot load/mint progress logs (image bake/map, load
stages, tokenizer build) - the rails do not apply, and a timing that is part of an API's answer -
the facade's `TtsTimings`, the per-stage walls a synthesis returns to its caller and the server
logs per request - is a deliverable of the same kind, not instrumentation. A clock whose value
feeds logic is control flow, not instrumentation; it is marked `// clock: control` so the sweep and any future lint leave it alone.

The override-announce rule (REVIEW.md) draws its boundary here: a knob or setter whose purpose
is timing still counts as an override when it moves computed numerics - two GEMM forms of the
same math differ in float terms - while one that changes only WHEN work happens does not, and
a CLI flag is never an override (it is the run's own command line, visible where the run is
launched).

### The audio outlier rule {#audio-outlier-rule}

An audio run's clips are rated in milliseconds per audio second, and a clip past
`AUDIO_OUTLIER_RATE_MULT` (4) times the run's median rate is an outlier of that run, not a speed
of its engine: a cold start, or a reference decoding at length on one clip. The rule is one
function, `audio_outlier_clips` (`dasllama/dasllama_exchange_schema.das`, engine-free), and three
readers apply it to the same stored shape. The recorder (`gen_bench_records.das`) applies it as a
run lands: a flagged clip is re-measured once, and one still flagged lands in the run's
`void_clips` with a warning. The site projection (`gen_site_records.das`) applies it to every
stored audio run as `bench_cells.json` is written, so a row recorded before the rule is voided
without a re-measure, and the pages read only the field - a voided clip fronts no speedup, no bar
and never the hero. The ladder store applies it to the audio cells it keeps per run, and a cell
rides to the page with its `voided` verdict. A reference row records no audio length, so every
reader borrows a clip's length from any run of the same model that recorded it. Under three rated
clips nothing is voided, and a run's own `void_clips` - the annotations' spelling - unions with the
rule's.

### A routed MoE's parity fixture holds as a half-span prefix {#fixture-prefix-bar}

The timed run's fixture gate holds every carrier as a matching prefix of half the checked span; the `--parity` pregate holds token for token, except on a carrier whose evidence sets `prefix_bar` (`ParityEvidence`, `performance/model_specs.das`) - the gemma-4-26B MoE, Q8_0 and Q4_K_M - which it holds to the same half-span prefix over the whole fixture. The 26B routes a discrete top-8 of 128 experts a layer and its forward amplifies one ulp of rounding layer over layer, so any float engine's logits move by whole points when only the accumulation order changes - the reference engine's between thread counts or micro-batch sizes, ours between JIT routes - and a token past the opening is a coin flip, not a forward bug. `fixture_ok` is the one bar both gates read.

### A `[tuned]` kernel's perm is decided at its own compile {#tuned-perm-precedence}

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

### The mint's own wall rides in the sidecar's provenance {#mint-wall-provenance}

A successful mint stamps three provenance keys into the sidecar before the archive copy is
written: `mint_gen_ms` (the generator half), `mint_kernels_ms` (the loop-hint half) and
`mint_total_ms` (the wrapper's own wall). The order is what makes the timestamped copy in the
box's tune-history directory carry them too, so the box's longitudinal record answers what
shipping a profile cost without re-running the mint - a console line that scrolled past is not
that record.

### The speculative round's cell is a ruler record {#ruler-records}

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

### The mlx-lm window recipe {#mlx-window-recipe}

`harness/mlx_window_profile.py` is the third-party twin of the window probe: under mlx-lm it
builds a prompt cache of `--prefix` tokens in 512-token chunks, times a `--window`-token prefill
as served (`mx.eval` of the logits and the cache state, the cache restored between reps, the
median of `--reps` after a warmup), then runs the same window once more with every module class's
call forced to evaluate for a per-class exclusive breakdown - the forced pass is slower than the
served one by the sync a call and the lost overlap, so its shares are the attribution and the served
figure the cost. Every reading it yields is `external`; the ledger pairs it with the window probe's
`--kprof` report on the same box, model geometry and window.
