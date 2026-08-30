# modules/dasLLAMA/tests - testing discipline

The Metal suites here are wall-time-expensive (model loads dominate; a full pass holds 40GB
GGUFs), so the rules below are enforcement, not advice - an ad-hoc full-suite run turns a
one-arm fix into an afternoon.

## Run suites ONLY through the runner

```
./bin/daslang -jit modules/dasLLAMA/tests/run.das -- --arm <filter> [--suite decode|prefill|matrix|kernels|image|image-vulkan|coverage|all] [--family llama]
./bin/daslang -jit modules/dasLLAMA/tests/run.das -- --suite model-free        # the per-PR gate, no --arm
```

`all` (the default) runs the parity suites, the support matrix, the kernel files and
`test_model_image` - it omits `image-vulkan`, `coverage` and `model-free`.

Every suite but `model-free` needs `--arm`. `--full` parses and is then refused, so `--arm` is
the only way in. `--suite model-free` takes neither - it is the whole gate. The runner redirects
the COMPLETE output to a log file, and prints that path on the DONE line. It owns the dastest
timeout. It repeats only when `--nreps` is passed explicitly (default 1, never best-of-N).

## The iteration loop

- Before launching any suite, state what the change can affect. A default-off knob or a
  comment edit does not need a rerun.
- Fixing or adding one arm runs exactly that arm: `--arm arm12 --suite decode`.
- Coverage is a cell in a suite, never a scratchpad probe. A probe proves nothing after the
  session that wrote it, and its setup diverges from the suite's silently. If covering a path
  needs a large model, it needs a large model.
- Batch every pending fix. Never re-run a full suite per fix.
- The pre-commit gate is the `--arm` set covering every arm the batched fixes can affect -
  for example `--arm arm,batch --suite decode` plus the touched suites' arms. A whole-zoo
  pass buys soak time, not coverage.

## Arm filter mechanics

`DASLLAMA_TEST_ARMS` (set by `--arm`) is a comma list of substrings matched against arm names
by `arm_on(t, name)` in `_model_tier.das`. Filtered arms register a LOUD `t |> skip`, so a
filtered run reports SKIPPED, never PASS - partial coverage cannot masquerade as full. A
failed assert still FAILS the test (dastest: `failed` beats `skipped`), so filtering can
never mask a red.

Arm names - decode parity: `arm1-basic arm2-hybrid arm3-step arm4-paged arm5-rewind
arm6-churn arm7-q8kv arm7b-tq4kv arm8-s16 arm9-reload arm10-kq arm11-depth arm12-dim
arm13-conc arm14-poison` (arm14 = the shared-region collision gate: a foreign GPU prefill must
not degrade a later forced-feed decode - Qwen2.5-0.5B, its own `[test]` block),
batch test: `batch` (whole test), `batchB7-partd`, `batchB8-kq`. Prefill parity: `base mm-tail s16
kq cont span span-fused span-mrope span-ds dim qkv` (mm-tail = the GEMV-tail residue peel -
four fixtures, npos % 32 == 1 (the lone row rides the reduction-split GEMV), == 2 (the b4
form at 2 rows), == 5 (two b4 dispatches, 4 rows + 1) and npos == 5 (the pure-tail leg,
no mul_mm at all) - token parity vs the all-CPU control, the tail-off A/B twin, and the
peeled sites/rows counters as the anti-vacuous witness; plus the SmolLM2 alignment-guard
leg - the guard is per mul_mm site on that site's K width (b4 needs % 128, the gemv % 32),
so at npos % 32 == 2 the 576-wide sites must refuse while exactly the hidden-kdim w2 site
(1536) peels per layer, and == 1 rides the gemv everywhere; counters only (greedy token
equality is not a valid instrument on a 135M); span = the non-causal media eval shape,
head + embd span, per codec; span-fused = the image turn's ONE-eval shape - causal head + media rows + causal
tail through the per-query mask, one GPU prefill, with a same-backend fused-vs-splice logits
witness on poison-calibrated per-codec bars; span-mrope = the qwen grid-roped turn - one GPU
prefill via the per-row-table capability, token-exact vs the all-CPU control, plus a
same-backend grid-vs-sequential prefill-logits witness; span-ds = the deepstack turn - one
GPU prefill via the slice-add capability, token parity plus the add-CONTRIBUTION witness:
per backend, the prefill logits with the tails minus the same logits with the tails zeroed;
the GPU's contribution must clear a 0.01 floor and its max difference from the CPU's must
stay under half the GPU's own contribution - greedy tokens tie on the counting fixture, so
a skipped add hides from them).

Support matrix: `cells-q8 window cells-s16 mode kq tensor dim8b dim70b` + the
family matrix `fam-qwen3 fam-qwen2 fam-phi3 fam-gemma2 fam-gemma3 fam-gemma4 fam-qwen3moe
fam-gemma4moe fam-gptoss fam-gemma4e fam-qwen35 fam-qwen35moe fam-qwen2moe` (needs-derivation pins +
per-path cells; fam-gemma2 also carries the sliding-window masking parity row;
fam-gemma4/fam-qwen3moe/fam-gemma4moe/fam-gptoss/fam-qwen35moe/fam-qwen2moe are
DASLLAMA_PARITY_FULL-gated - 7.4/18.5/26.9/12.1/22/15GB; fam-gemma4moe and fam-gptoss are ENGAGE
+ shallow logits TOLERANCE cells only - token parity is not a valid instrument for the 26B, whose double-router
CPU differs from any float implementation by ~2.5 logits/step by construction;
fam-qwen35/fam-qwen35moe are deltanet hybrids whose batch cell asserts the per-row FALLBACK
shape - metal batch steps 0, both rows served on the single-decode path; fam-qwen2moe's
batch cell asserts the `graph` DECLINE on the planar model - shexp has no batch arm, and a
blob twin's CPU batch fallback would trip the blob-only panic).

The `image` suite (test_model_image - the prepared-image .dlim rail): `mechanics` (synthetic
carrier, model-free - runs with no model stocked; also the layout fingerprint and the dev-W
bake tables, whose rebuild-not-append contract and per-format key arithmetic are pure taxonomy
over a job list) `smol metal gemma tower whisper voxtral
parakeet qwen3a canary canary-dec gemma4a gemma4uv gemma4uv-metal gemma4v gemma3v gemma4e
mtower`; `gemma4e` is the E2B metal-blob
mint+map arm - the PLE go-live tripwire (`ple_check_table`, which panics when the per-layer
embedding table's plane is short) runs after the blob plane borrows, so a fresh mint and a warm
map must both clear it; `gemma4uv-metal` is the GPU tower
driver's parity/counter/knob gate for the gemma4uv embedder, Apple builds only (`--arm
gemma4uv` selects it too - arm filters match by substring); `mtower` is the whisper-class
tower-blocks gate, Apple builds only - whisper tiny + large-v3-turbo transcript-exact and
qwen3a f32-rail transcript equality, CPU vs GPU, with geometry-derived counter deltas, the
twin-W legs - `wblob`, the halfword copy of a tower's GEMM weights baked beside the f32 blob and
read only where a tensor crown compiled the half GEMMs (`../ARCHITECTURE_IMAGE.md` sec 2.1i)
- on whisper f16 + qwen3a bf16, each engaging by the route counter, plus whisper's own
twin-knob freeze and whisper's own wblob-ONLY poison that must CHANGE the GPU transcript
(both legs are whisper's alone; qwen3a carries neither), the gemma4a Metal
Conformer cell (f32-lane transcript equality CPU vs GPU + encode rel-rms + counter deltas -
the lane pin/reset discipline mirrors qwen3a's), the canary Metal FastConformer cell (the
same discipline over the rel-pos XL block loop; decoder = the q8_0 serving artifact), plus
the tower q8-decline - a q8 whisper encoder never dispatches and records the `quant_mode`
decline, and the whisper serving default IS q8 unless `set_asr_fp32` / `set_asr_tower_fp32`
asks for f32 (whisper carries no lane policy). Canary and gemma4a do: un-pinned, their lane
follows whether the Metal tower would serve (`../ARCHITECTURE_MEDIA.md` sec 2.15).
Then the required-mode panic and Conformer-absence (parakeet) cells; the arm's DECODER half is the `test_whisper_metal_cross_kv`
cell in `test_model_image.das` - GPU cross-KV on the q8 serving default, transcript-exact
against the CPU chain with window/step counter deltas and the knob and quant_mode declines,
required-mode, step-floor and shutdown-re-arm contract; the voxtral arm re-saves a
5.4 GB image from cold every run by design (it IS the >2 GiB-plane IO coverage); the `metal`
arm mints/maps the blob-only metal flavor (SmolLM2) incl. the CPU-tripwire and a
teacher-forced logits-tolerance parity cell (greedy token equality is NOT a valid bar on a
135M - genuine near-ties flip on ~0.02 gaps under ~0.75 cross-backend noise). The ASR-family
arms (`parakeet` transcript-exact, `qwen3a`/`canary`/`gemma4a` element-exact planes,
`canary-dec` = the opt-in fp32 Model rail, token-exact) re-save their images from cold each
run like the voxtral arm. The canary arm carries both lanes: the f32 element-exact cell and
the q8 cell (read-time transcode - qblob/qscales/compact-blob element-exact vs a staged read).

The `image-vulkan` suite (test_model_image_vulkan, arm `vulkan`) covers the OFFLINE vulkan
bake: the runner arms DASLLAMA_GPU + a small VRAM budget so the probed config carries a
vulkan section, the DRY tier collects a role-stamped plan with no device calls (safe on
GPU-less boxes), and the flavor image round-trips the plan verbatim.

The `coverage` suite (test_kernel_coverage, arm `coverage`; arm `coverage-vk` = the vulkan
SERVING census - needs a vulkan device + `DASLLAMA_GPU=1` + `DASLLAMA_MODELS_DIR`, MoE rows
under `DASLLAMA_PARITY_FULL=1`) is the KERNEL COVERAGE census (the census-row obligation is
`REVIEW.md`'s): the small-model zoo swept across format/graph/batch/KV axes, then a
report of per-kernel dispatch counts with LOUD WARNINGS for compiled-but-never-dispatched
kernels - never an auto-dead verdict. A zero means "nothing THIS zoo runs dispatched it",
never "unreachable". A kernel's dispatch predicate can be satisfiable by a servable model,
quant, or shape nobody stocks locally, and by any family registered later. So before a kernel
is deleted, the census only points the reachability audit of that predicate; it never settles
it. Small-tier warnings for kernels whose carriers sit above the tier
(MoE/mx4/suppress) are expected - their census rows serve only under `DASLLAMA_PARITY_FULL=1`;
the served-count floor is asserted only on family-unfiltered runs. The vulkan half here is
the device-free rail unit; the serving vulkan census runs on the PC box.

## Metal kernel gates

The `kernels` suite (test_metal_{prefill,decode,rope,gemv,misc,attn,gemm}_kernels - model-less
per-class CPU-oracle units covering the FULL metal kernel census, ~2-3 min) has no arms;
remember it exists (the hand-bound-gate sync obligation is `REVIEW.md`'s). Shared fixtures
(buf helpers, the mismatch compares that dump both sides, kq plane + q8 blob builders) live
in `_metal_kernel_common.das`. `test_metal_prefill_kernels.das` keeps its tag-less mismatch
compares local - a same-arity twin would collide with the shared tagged one. `_mtl_toy.das`
is the `[metal_dispatch]` multi-kernel (kernel=) fixture; its gate in the misc file
dispatches through the GENERATED builders (kn_ rail), not hand binds.

The control obligation for a new gate or bar is `REVIEW.md`'s. Size a poison as a value ADDED
to the expected result, not as a multiple of the tolerance. A tolerance that scales with the
accumulation length swallows a scaled poison at the kernel's longest dot product, which is
exactly where a bug hides. Every compare against a derived truth gets its own poison. A gate
that skips `metal_set_threadgroup_memory_length` for a kernel with `@workgroup` state reads
garbage silently - no error, a plausible wrong number.

## Model-free / no-arm tests

Which files belong to the `model-free` suite is `REVIEW.md`'s to say. The runner sets
`DASLLAMA_CPU_PREFILL=1` for every child. That is why the CPU-prefill tripwire cannot ride
the `model-free` suite: the runner disarms the guard that tripwire asserts. The map below is
partial. `run.das`'s `model-free` list is the census.
`test_vulkan_dec_tail.das` - model-free (a Vulkan device, else skips): the per-op tier's decode
era against a CPU reference - the decode attention block (K-quant and q8 quads, both rope
pairings, the hydrate arms), the decode FFN tail, and the whole-token decode span with its
device router + top-k against `moe_select_core`, plus the `vulkan_moe_span` override reached
through its registry.
`test_vulkan_moe_cm2.das` - model-free (a cm2 device, else skips): the cm2 expert chain over a
device-side f16 gather, the streamed-group slot hand-off, and the streamed split's async head.
`test_bench_records_schema.das` - model-free: the record store's schema (round-trip, upsert
identity with `workload` in the key, annotations landing only on the rows they select, the
store lister admitting `records/{box}.json` alone) and the record rig's shared seams (the
`-w` workload scope; the stored-row->rig-leg map, `backend`/`flavor` => `metal` | plain cpu |
`accel`, else refused; the tune-stamp gate; the oracle compare's ok/warn/fail bands; the
upstream image-reference parser `parse_mtmd_image` - encode summing, the MTMD_TIMING split,
its refusal arms); plus the committed-records sweeps: image-chat receipts match their
`backend`/`flavor` stamps and pin the fixture and mmproj, and every das row's `tune_sha`
resolves to its committed generation archive.
`test_exchange_schema.das` - model-free: the exchange validator, sweeping the ENTIRE in-tree
records/sidecar corpus, so a writer-schema change reds here first.
`failed_dasllama_lint_require.das` - model-free, expected-compile-failure: the
facade lint trips DASLLAMA001 (code 50503) on a direct engine require with no escape.
`failed_dasllama_lint_sidedoor.das` - model-free, expected-compile-failure: the lint's tree
guard trips on a path-require resolving into modules/dasLLAMA, name prefix or not.
`test_dasllama_lint_escape.das` - model-free: `options _dasllama_internal = true` admits a
direct engine require (the lint's escape hatch).
`test_dasllama_lint_contracts.das` - model-free: the lint's ALLOWED set (a facade-only program
with no escape compiles; an internal require does not) via spawned compiles,
`load_audio_16k_mono`'s empty-on-failure contract, `decode_audio_16k_mono`'s frame cap (a
synthetic `sampleRate=1` WAV bomb is refused before decode, an uncapped call still works), and
`gemma4a_probe_proj_dim`'s 0-not-panic contract on `.dlim` / missing / non-GGUF inputs.
`test_think_split.das` - the reply-side reasoning matcher, model-free: every
thinking family's wire shape, whole-string and per-chunk down to 1 byte.
`test_tool_formats.das` - the per-ToolMode wire codecs (dasllama_tools), model-free: defs
serializers and call parsers for harmony/gemma4/mistral/llama_json against verbatim fixtures.
`test_scheduler.das` - the continuous-batching scheduler (dasllama_scheduler) against
`generate()` references; skips honestly without SmolLM2-135M / the MTP fixture, `-jit` only.
`test_program_roots.das` - model-free: every dasllama program root (tutorials, examples,
server tools) declares `options stack = 524288`, and every model-loading root declares its
prefill intent.
`test_audio.das` - model-free: the audio front-end units (gelu-erf, hann window, mel
filterbank, log-mel chunking, swapped swiglu); model-gated: the tower structure/oracle gates
(ultravox/voxtral/omni shapes, the mtmd all-ones encode oracles - CPU-claim cells, tower knob
pinned OFF) and the `test_encoder_blocks_gpu` cell, the qwen2audio + voxtral 32-layer
CPU-vs-GPU blocks parity on the depth-scaled bars with counter deltas - Apple builds, `-jit`;
skips honestly without the qwen2audio / voxtral mmprojs.
`test_whisper.das` - model-free suite; model-gated: the whisper/parakeet/canary/gemma4a/omni
oracle cells, the ASR knob cells (`set_asr_fp32`, `set_asr_tower_fp32` - the mixed
f32-enc/q8-dec serving mode and its `asr_exec_fmt` stamp), the q8-gate CPU-vs-CPU claims
(CPU-claim cells, wdec knob pinned OFF), and the mel state-reuse determinism cell (gemma4a +
canary: `gemm_f32` accumulates, so a reused state's `st.reim` must start zeroed - bit-equal
mels across calls); its ungated cells are the model-free half.
`test_asr_verbs.das` - model-free: the family-owned ASR facade verbs (`asr_exec_fmt` /
`asr_encode_bucket`) over constructed structs, the audio families' lane knobs (qwen3a /
gemma4a / canary: the un-pinned default against the predicate the policy itself consults, both
pins, and reset from the EXACT pin), and parakeet's SPM detokenizer over a toy vocab.
`test_model_specs.das` - model-free: the model-set table's shape invariants
(`../performance/model_specs.das`: unique file/display keys, official => provenance pinned,
parity-evidence shape), the derived provenance view's invariants (unique names, sha-or-recipe,
https urls), the mmproj pairing lookup (`mmproj_companion` - the companion found by name, not
by position; >= 2 official vision rows), and the pinned image fixture's provenance
(`bench_image_fixture` rides some spec's companion list).
`test_parity.das` - model-free suite; model-gated: the frozen token-for-token parity gates. ONE
generic loop drives every evidence-carrying spec of the model-set table through its declared
pinned arms (evidence is DATA on `ModelSpec.parity` - ids + arms, regenerated via
`../harness/parity.sh`); hand-written arms remain only for the tied-cls bit-match and the gpt-oss
shared-load double fixture. Every compare logs both decoded streams. Large carriers gate
on `DASLLAMA_PARITY_FULL=1` via `model_available`.
`test_parity_pregate.das` - model-free suite; model-gated: the board parity pregate
(`lcpp_bench --parity`, via `parity_check` fed a controlled spec) on small carriers - the real
fixture passes, a flipped id fails, an evidence-less spec is refused, the text-form prompt
encodes through the tokenizer (E2B), the kq-native arm engages and restores the mode
(Qwen3-4B Q4_K_M). Requires the bench by relative path (like `test_tok_seed`), so it pays the
bench's engine compile.
`test_run_summary.das` - model-free: `run.das`'s own `log_summary` log scraper (last marker line
wins, end of file closes an unterminated final line, doubled marker yields one line), fed
synthetic log files from a per-process temp dir. Requires `run` by bare same-dir name.
`test_site_records.das` - model-free: the records-vs-site drift gate - `merge_site_records`
(required by relative path, pays the engine compile) regenerated in memory and byte-compared
against the committed `site/files/dasllama/bench_records.json` (what daslang.io/dasllama.html
renders); red means a records commit skipped `gen_site_records`.
`test_tok_seed.das` - model-free: `lcpp_bench.das`'s `tok_read_seed` corpus-header walk, required
by relative path (`../benchmarks/lcpp_bench.das`), so it pays the bench's full engine compile.
`test_sizing_helpers.das` - model-free: the sizing helpers (`reserve_resize` exact capacity,
`grow_resize` geometric reuse, `overwrite_resize` grow-only no-init) fed directly, including
grows past the `max_unreserved_size` guard that must not panic.
`test_deltanet.das` - model-free: the deltanet session-state sizing at 27B geometry through
`make_run_state` (S state + widened-conv history past the guard); model-gated: the
chunked-vs-recurrent prefill equivalence probe on Qwen3.5-0.8B.
`test_vision.das` - model-free: the vision preprocessing rail (geometry, letterbox, normalize)
bit-exact against pinned mtmd oracle hashes (dumps + mint scripts in the models dir's
`gemma4-vision-oracle/` and `qwen3vl-vision-oracle/` - the qwen rail letterboxes at align 32),
plus the stbimage decode/dump round-trips in a per-process temp dir.
`test_gemma4uv.das` - the gemma4uv embedder tier-1 parity vs the `-p encode` oracle dumps
(f32-mmproj-minted - the bf16 oracle carries bf16-dot activation noise); gates per-token
mean/v0..v3 at 2e-4 with the measured maxdiff logged; skips honestly without the mmproj or dumps.
On Apple builds the CPU gate pins the tower knob off, and a second test gates the GPU tier-1
encode against the same dumps on a scale-relative bar (2e-4 + 4e-3*token-rms) - exceeding it
is a red, the bar each fixture actually held is logged either way, and engage is proven per
fixture by the encodes counter.
`test_gemma4v.das` - the gemma4v ViT tower (E-series) tier-1 parity vs the `-p encode` dumps
minted on the f32-widened mmproj, CPU, `-fa off` (`mint_e2b.sh` / `mint_e4b.sh`): eight E2B
fixtures (96^2 cb through 672x336) on the scale-relative bar 2e-4 + 4e-3*token-rms, the measured
maxdiff logged per fixture; plus the clamp knockout (every block clamp disarmed through the
staging planes must miss the oracle - the sidecar scalars are load-bearing); plus the E4B
rung - the same tower geometry at soft-token width 2560, gated on its mmproj's four-dump seam
subset with one GPU-engage and one q8-lane fixture. Skips honestly without the mmprojs or dumps.
`test_gemma3v.das` - the gemma3 SigLIP tower (gemma-3-4b mmproj) tier-1 parity vs the
`-p encode` dumps minted on the f32-widened f16 mmproj, CPU, `-fa off`
(`gemma3-vision-oracle/mint_gemma3.sh`): the canvas is FIXED 896^2 (learned position table), so
the five fixtures vary content, not geometry; exact lane on the 2e-4 + 4e-3*token-rms bar, the
q8 serving lane on its measured 3.2e-1*rms bar (27 blocks, ffn served at the layout's padded
4352 width so every GEMM quantizes), plus the fixed-canvas panic gate and the carrier
sniff/exec_fmt cells. On Apple builds the CPU gate pins the tower knob off, and a GPU rung
gates two fixtures through the Metal block loop on its measured 4e-2*rms bar - engage proven
per fixture by the encodes/blocks counters, plus the knob-off decline leg (the 72-wide heads
restride to the attention tiles' 128 on the driver), the q8-decline leg (a PINNED-q8 tower with
the knob ARMED must never dispatch and must record the `quant_mode` decline - its Q8_0 planes
would read as f32 garbage), and a third crowned encode on the twin-W route
(`set_metal_tensor_crowns("mulmm_q8")` + `set_metal_tower_f16(true)`, the lane pinned exact so
the twin is baked), witnessed by the `metal_tower_f16_encodes` delta. Skips honestly without
the mmproj or dumps.
`test_qwen3v.das` - the qwen3v tower tier-1 parity vs the `-p encode` dumps minted on
f32-widened mmprojs, CPU (`qwen3vl-vision-oracle/mint.sh` + `mint_4b.sh`): the Omni leg
(`qwen3vl_merger` no deepstack) on seven fixtures (cb96 = the pos-table downscale arm,
cb640x320 = the merge-reorder/transposed-grid gate) at 2e-4 + 1.5e-2*token-rms (the ff_pad
width regroups the down GEMM's f32 accumulation; the live exact-lane zero-layer poison reds
the bar at 0.87), plus the
merged-patch-grid panic gate; and the Qwen3-VL 4B DEEPSTACK leg (taps 5/11/17, wide
10240-float rows) on four fixtures at 2e-4 + 4e-2*rms - the 4B dumps carry q1/q2/q3 quarter-offset probe
fields - the compare applies them when the dump has them - hitting each concatenated
slice's first element (a skipped-tap poison lands at 6.9-9.7 on them, 600x; mean+v0..v3
alone are BLIND to a zeroed slice). The q8 serving lane (the CPU policy default when neither
the Metal tower nor the accelerate float-batch tier serves - `qwen3v_gpu_would_serve()` is the
driver clause) gets its own cells, each bar carrying its own
must-EXCEED poison leg - a block's qblob region zeroed through the staging planes, scored
by `encode_excess`: the Omni leg on gray448 + cb448 + cb96 at its measured 5.2e-1*rms bar,
poisoned at a mid-stack block; the deepstack leg on cb448 + cb96 at 6.5e-1, poisoned at
the TAP-1 block so the zero hits both the main path and a served slice - gray's tap-slice
probes measure 7.9*rms because the taps sample mid-network residuals, so gray stays on the
exact lane's set, and the proof that the served slices still carry signal is the
zeroed-slices decoder control in `test_vision_chat.das`; a METAL tower cell (both towers,
tower pinned ON, its own GPU bars 4e-2 / 8e-2*rms per the gemma3v f16-tile precedent, gray
off the GPU-ds set like the q8-ds cell, engage proven by encode/block counter deltas per
GATED fixture, and its own GPU-lane zero-layer poison); an f16-W ROUTE cell (crowns pinned
via set_metal_tensor_crowns + prefill re-init so the half twins compile, the 4B fixtures on
the same GPU bars, engage proven by the metal_tower_f16_encodes delta, plus the knob-off
leg whose counter must not move, and the Omni-30B bf16 half - the same crowned route on the
bf16-sourced tower, one fixture, its own f16-encodes delta, which is what proves the twin
bake covers bf16 files and not just f16 ones); and a model-free lane-knob cell.
The model-gated cells skip honestly without the mmprojs or dumps (the metal cell counts its
gated fixtures and skips when the dumps are absent).
`test_qwen25v.das` - the qwen25v tower (Qwen2.5-Omni's window-attention ViT, projector
`qwen2.5o`) tier-1 parity vs the `-p encode` dumps minted on the f32-widened dual-tower
mmproj, CPU (`qwen3vl-vision-oracle/mint_25o.sh`): five fixtures, four of them shaped - cb112 =
the single-window arm, cb448 = four full windows, cb616x336 = ragged window edges, and
quad448 = the WINDOW DISCRIMINATOR (four exact-value quadrants; uniform/periodic fixtures
make every window
statistically identical, so an all-full-attention poison hides under them - quad reds it at
10.7 vs the 2e-4 + 1e-2*rms bar) - plus the merged-patch-grid panic gate. No q8 lane in
this tower, so no q8-lane cells. On Apple builds the CPU gate pins the tower knob off, and a
GPU rung gates four fixtures through the Metal window-ViT driver on the 8e-2*rms bar (the
32-layer mix amplifies the f16-operand GEMM noise and re-rolls on low-bit kernel changes;
quad448 is OFF this rung's numeric set for exactly that reason - window-distinct content
realizes the amplification at 0.18-0.4*rms), with engage proven per fixture by the
encodes/blocks counters, a twin-route counter witness (crowns pinned), the knob-off decline
leg, a zeroed-block GPU-lane poison through BOTH staging planes, and the SHALLOW routing
cells - 1-layer truncated towers (pure-window and all-full) on quad content, GPU vs the CPU
chain at 0.1 abs, the chaos-free window discrimination for the DRIVER (the kernel's own
block-diagonal strictness is the kernels-suite `tower_win` gate). Skips honestly without the
mmproj or dumps.
`_vision_oracle.das` is the shared dump parser / fixture generator / per-token compare /
over-bar scorer (the must-EXCEED half of a poison leg) all vision tier-1 tests use (the
`quad` generator and the q1/q2/q3 quarter-offset probe fields live here).
`test_audio_embedder.das` - model-free: the `AudioEmbedder` carrier's own arms - the no-audio
refusals and the probe's 0-not-panic contract; model-gated: the gemma4a arm on the E2B mmproj,
carrying the padding-contract cell (a 320-sample clip encodes to exactly 1 soft token).
`test_vision_embedder.das` - model-free: the `VisionEmbedder` carrier's own arms over
constructed carriers - the text-only (none) shape, the loader's refusals by name (missing
file, audio-only mmproj), and the `vision_exec_fmt` lane stamp (the qwen3v q8 flag reaching
it; qwen25v exact-only); model-gated: the gemma4uv arm on the 12B mmproj - the sniffed
family tag, the 48 px align, the 3840 projection width.
`test_ple_check.das` - model-free: the PLE go-live tripwire (`ple_check_table`) on synthetic
Model shells - short plane trips per format arm, full plane passes, non-PLE exempt.
`test_ple_modes.das` - model-free suite; model-gated (E2B Q8_0 + Q4_K_M, small tier): the PLE
token table's pinned-plane rail across serving modes - fp32 keeps the Q8_0 table on a
dedicated q8 plane (offset 0, plane == table exactly, gather rows BIT-match the file dequant,
wblob provably too small to carry the expansion) and a K-quant table on its native kq plane;
plus a greedy chat turn per serving mode on the E2B Q8_0 carrier - a q8 control, an fp32 turn,
and a `q4_0` turn, the `q4_0` one proving q4 serving has a per-layer-embedding rail.
`test_tower_helpers.das` - model-free: the shared encoder-tower helpers in `dasllama/dasllama_tower`
(clamp, row norms, f16-table GEGLU-quick, im2col, two-axis rope, avg-pool, `attention_bidir`),
each against an in-test reference.
`test_attn_span.das` - the non-causal image span (`eval_embd_` with `non_causal = true`): mask direction by
perturbation (causal row 0 blind to the last row, span row 0 sees it), classic/blocked/flash
agreement, and the flag-reset bit-exactness; plus the FUSED mid-turn span (`eval_embd_span_`):
splice equivalence via a whole-cache decode-logits witness (classic/blocked bit-exact, flash
tolerance) and the per-query mask direction inside one eval; plus the deepstack wide-row rail
(stamped `n_deepstack`): zero-tail wide == narrow bit-exact, nonzero tails move the logits,
slice 0 vs slice 2 add at different depths, no stale plane after the quantum; stories15M
fixture (test_flash's), skips without it.
`test_cpu_prefill_tripwire.das` - the CPU-prefill guard: an undeclared prefill trips - span,
causal, MROPE-quantum, and DEEPSTACK-quantum alike (the metal rail serves all four, so a
CPU-served one is a silent fallback);
same stories15M fixture, deliberately never calls `allow_cpu_prefill()` (which is why it
cannot live in test_attn_span - that file arms it in `[init]`). Metal-capable builds only;
plain dastest only.
`test_vision_chat.das` - the image chat turn end to end, one pair per vision carrier plus the
showcase: the 12B gemma4uv pair (the cats fixture, so `DASLLAMA_PARITY_FULL=1`), the E2B
gemma4v pair (E2B Q8 decoder + bf16 mmproj - small tier, runs without the flag), the
gemma-3-4b gemma3v pair (small tier), the gemma-3-12b pair (the same SigLIP tower at
projection 3840 - large tier, `DASLLAMA_PARITY_FULL=1`), and the Qwen3-Omni qwen3v pair
(large tier - the mrope leg: grid reaches the chat, the session's rope delta reflects the
grid advance), the Qwen3-VL 4B deepstack pair (small tier - wide 10240-float rows through
the chat, the caption, and the zeroed-slices decoder control: tails zeroed on the same rows
must move the prefill logits, measured 10.4 - a caption alone cannot see a decoder that
ignores the slices), the rows-seam cell (same 4B pair - pre-encoded wide rows through
`add_user_image_rows_` on a plain chat reproduce the embedder walk token-for-token; the seam
lcpp_bench prices, where a hand-splice can feed deepstack decoders a narrow scrambled span),
the Qwen2.5-Omni-3B qwen25v pair (small tier - the window ViT + the
qwen2vl NON-interleaved MROPE decoder; the vocab spells the span markers
`<|vision_bos|>`/`<|vision_eos|>`, resolved by the chat layer's vocab-driven fallback)
plus the `test_omni_showcase` cell in the same file (one Omni session: an image turn, then a text turn
whose answer needs the image turn across the mrope position delta; the chat AudioTower serves the whisper-class families only):
the prompt stream shape around the splice
(marker ids, media-first, span length from the geometry) and the greedy caption, logged in
full. NOT token-parity with
llama-mtmd-cli - the oracle renders its jinja template in thinking mode while dasLLAMA's gemma-4
arm defaults to instruct. On Apple
builds the turn also carries the tower legs: the default caption's image encode must show an
encodes delta (GPU-served), then a knob-off chat repeats the turn on the CPU embedder and
must clear the same caption bar - the caption names the cats and is a description, not a
fragment - with zero tower dispatches and the knob decline counted.

## Model loads - never the image rail

The loader obligation is `REVIEW.md`'s. The mechanism: the `.dlim` image rail stamps every
mint with the box identity (backend pin, wscale, tune manifest), and GC-purges sibling
flavors. A suite child's pinned identity differs from the serving rig's. So a suite on the
rail both re-mints multi-GB images the rig cannot use and purges the flavors the rig depends
on. Image-rail coverage (mint, map, GC, flavors) lives in the image suites alone
(`test_model_image`, `test_model_image_vulkan`).

## Metal fixtures - driver knobs and the two-model pattern

The establish-and-restore obligation is `REVIEW.md`'s. The mechanism: the hooks are on by
default. They flip a q8 leg to the GPU silently, and an f32 leg records a quant_mode decline
that panics under required mode. Either way the cell stops measuring what its name says. The
family serving-lane pins (`set_<family>_q8`) are the same trap in the other direction: an
unpinned lane cell measures whichever lane the box's policy picked.

The Metal drivers serve ONLY blob-form models (`convert_model_to_metal_blob` / metal-flavor
images), and CPU inference on a blob model PANICS. That is why a CPU-vs-GPU arm needs the
planar model plus its `blob_twin(t, path, seq_cap)`. Decline-reason cells keep the planar
model: capability reasons (`feature`, `graph`, `shape`, ...) out-rank the `planar` decline in
every gate, and the planar CPU fallback serves quietly. The prefill npos POLICY window is
planar-only - a blob model serves any npos, because no CPU fallback exists.
`set_metal_prefill_mulmm_legacy` forces a `planar` capability decline; the required-mode
panic cell uses it.

## Family filter (profiling cadence)

`--family <tokens>` (env `DASLLAMA_TEST_FAMILY`, comma list) composes with `--arm`: only
model blocks tagged with a listed family run - `family_on(t, name)` in
`_model_tier.das`, EXACT token match, loud `t |> skip` like the arm filter. Model-free blocks
carry no tag and always run. Family tokens: `llama` (`--suite decode`, `prefill`, `matrix` and
`coverage`, plus the image `smol` and `metal` arms and the `image-vulkan` `vulkan` arm),
`qwen2`, `qwen3`, `phi3`,
`gemma2`, `gemma3`, `gemma4`, `qwen3moe`, `gemma4moe`, `gptoss`, `qwen35`, `qwen35moe`, `qwen2moe` (the support-matrix family cells), `gemma`,
`ultravox`, `whisper`, `voxtral`, `parakeet`, `qwen3a`, `canary`, `gemma4a` (image suite arms),
`gemma4e` (support-matrix rows under `fam-gemma4e` - E4B PARITY_FULL-gated; E2B Q8_0 and
Q4_K_M small-tier always-on, carrying the per-layer-FFN-width and blob-kq-PLE-gather coverage.
Both E2B rows assert parity through their forced-feed cells, not token equality, because
freeform continuations from a 2B sit on near-ties; plus the coverage-census E4B row; `gptoss`/`qwen2moe`/`qwen3moe` carry census rows too - all PARITY_FULL-gated).
When profiling one family across formats, gate each round with
`--arm <arms> --family <fam>` instead of the whole zoo. The family tag on a new
model-loading block is `REVIEW.md`'s obligation.

## Model tiers

`model_available(t, path)` is the size gate; the tier rule is `REVIEW.md`'s. Set
`DASLLAMA_PARITY_FULL=1` explicitly with an `--arm` run when a change genuinely needs the
large tier. A run reporting SKIPPED for those arms is correct, not a failure. The 70B's
no-CPU-control-batch-parity restriction is `REVIEW.md`'s too. The reason: streaming 40GB on
the CPU while the GPU has the same bytes wired OOM-kills a 64GB box. The small-model pins
that carry that coverage instead are e.g. `set_metal_batch_addrms_unfused`.

## Log discipline

Always capture COMPLETE logs (the runner does this); grep afterwards, never at capture time - a
capture-time filter can hide the exact proof line the run exists to produce, and the silent
capture reads as success.
The size/depth-claim assert ("2030 tokens", "crosses 2048") is `REVIEW.md`'s obligation.
Logging both decoded streams is `REVIEW.md`'s too. The mechanism: a near-tie synonym flip and
real garbage look identical as id diffs, and completely different as text. So read the text
before trusting a red or a suspicious green. A token-for-token generate cell has
`log_gen_texts` (`_model_tier.das`) for it. A logits-tolerance cell logs a decoded text form
instead - the forced stream plus the GPU's greedy would-be picks, or both next-token pieces.

## Stale truth caches (`<model>.ref.<key>.tsv`)

`cached_ids`/`cached_vals` pin a CPU trajectory into `<gguf>.ref.<key>.tsv` beside the model.
FREEFORM-prompt caches sit on sub-noise near-ties, so any numerics-adjacent master merge can
legitimately move the CURRENT CPU trajectory off the cached one - the parity assert then fails
with the GPU side actually CORRECT (it matches today's CPU). Before declaring a fam-row red a
regression: (1) stash + clean-tree rerun (same red => not your diff), (2) `mv` the cell's `.ref`
tsv aside and rerun - a fresh-truth green means stale cache, keep the refreshed tsv. Counting
caches are tie-proof by construction and should NOT move; a counting-cache mismatch is a real
red.

The freeform-token-parity ban is `REVIEW.md`'s. The form that replaces it is the k4 freeform
cell's, cached stream `gen_free_n128`. For that form the cache is a FEED, not a truth: both
sides force through the same tokens. So a stale feed stays a valid instrument, and the
stale-cache red class does not exist for it.

## Out-of-folder test files (the checklist's placement ledger)

Every `[test]` file requiring a `dasllama/*` module outside this folder, each with its reason:
- `utils/dasllama-server/test_openai_server*.das` - require the server by bare same-dir name
  (the hyphenated directory is unreachable by path require).
- `utils/dasllama-server/test_exchange_client.das` - requires `dasllama/dasllama_exchange` by
  registered name (nothing pins it to that directory); it stays beside the server suites
  because its fixed test port is coordinated with theirs (see its `TEST_PORT` note).
- `modules/dasLLVM/tests/test_grid.das`, `test_tune.das`, `test_tuned.das` - the tune
  framework's own tests; they require `dasllama/dasllama_tune` (the tuner contract) and
  live with dasLLVM under `modules/dasLLVM/REVIEW.md`.
- `modules/dasLLAMA/benchmarks/matmul/test_matmul_par.das` - the bench self-check, requiring
  `matmul_variants` by bare same-dir name.

The ladder tests (`utils/internal/dasllama-ladder/test_*.das`) answer to that folder's own
`REVIEW.md` - not dasLLAMA tests, not ledger entries. That holds even though two of them,
`test_ladder_store.das` and `test_ladder_server.das`, reach `ladder_store.das`, which
requires `dasllama/dasllama_exchange_schema` publicly.
