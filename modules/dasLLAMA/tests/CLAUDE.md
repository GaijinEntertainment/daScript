# modules/dasLLAMA/tests — testing discipline

The Metal suites here are wall-time-expensive (model loads dominate; a full pass holds 40GB
GGUFs). The rules below exist because one session spent 5.75 of 6 hours re-running full suites
to verify one-arm fixes. They are enforcement, not advice.

## Run suites ONLY through the runner

```
./bin/daslang -jit modules/dasLLAMA/tests/run.das -- --arm <filter> [--suite decode|prefill|matrix|kernels|image|image-vulkan|coverage|all] [--family llama]
./bin/daslang -jit modules/dasLLAMA/tests/run.das -- --suite model-free        # the per-PR gate, no --arm
```

Never invoke `dastest/dastest.das --test modules/dasLLAMA/tests/...` directly for the metal suites.
`--full` is REFUSED while the Metal build-out is in progress ("please narrow the scope...") —
scope every gate with `--arm` to the arms the change can actually affect (a whole-zoo pass
buys soak time, not coverage; e.g. a driver change gates on `--arm arm,batch --suite decode`).
The runner refuses to run without exactly one of `--arm` / `--full`, tees the COMPLETE output
to a log file (path printed on the DONE line), owns the dastest timeout (1200s), and repeats
only when `--nreps` is passed explicitly (default 1, never best-of-N).

## The iteration loop

1. Fixing/adding one arm → run exactly that arm: `--arm arm12 --suite decode` (~minutes).
   The gate is a cell in the suite, not a scratchpad probe: a probe proves nothing after the
   session that wrote it, and its setup diverges from the suite's silently. If covering a path
   needs a large model, it needs a large model — a slow gate beats an untested change.
2. Batch every pending fix. Do NOT re-run a full suite per fix.
3. The pre-commit gate is the `--arm` set covering every arm the batched fixes can affect
   (e.g. `--arm arm,batch --suite decode` + the touched suites' arms). `--full` is refused
   while the Metal build-out is in progress.
4. Before launching ANY suite, state what the change can affect; a default-off knob or a
   comment edit does not need a rerun.

## Arm filter mechanics

`DASLLAMA_TEST_ARMS` (set by `--arm`) is a comma list of substrings matched against arm names
by `arm_on(t, name)` in `_model_tier.das`. Filtered arms register a LOUD `t |> skip`, so a
filtered run reports SKIPPED, never PASS — partial coverage cannot masquerade as full. A
failed assert still FAILS the test (dastest: `failed` beats `skipped`), so filtering can
never mask a red.

Arm names — decode parity: `arm1-basic arm2-hybrid arm3-step arm4-paged arm5-rewind
arm6-churn arm7-q8kv arm7b-tq4kv arm8-s16 arm9-reload arm10-kq arm11-depth arm12-dim
arm13-conc arm14-poison` (arm14 = the shared-region collision gate: a foreign GPU prefill must
not degrade a later forced-feed decode — Qwen2.5-0.5B, its own `[test]` block),
batch test: `batch` (whole test), `batchB7-partd`, `batchB8-kq`. Prefill parity: `base s16
kq cont span dim qkv` (span = the non-causal media eval shape, head + embd span, per codec).
Support matrix: `cells-q8 window cells-s16 mode kq dim8b dim70b` + the
family matrix `fam-qwen3 fam-qwen2 fam-phi3 fam-gemma2 fam-gemma3 fam-gemma4 fam-qwen3moe
fam-gemma4moe fam-gptoss fam-gemma4e fam-qwen35 fam-qwen35moe fam-qwen2moe` (needs-derivation pins +
per-path cells; fam-gemma2 also carries the sliding-window masking parity row;
fam-gemma4/fam-qwen3moe/fam-gemma4moe/fam-gptoss/fam-qwen35moe/fam-qwen2moe are
DASLLAMA_PARITY_FULL-gated — 7.4/18.5/26.9/12.1/22/15GB; fam-gemma4moe and fam-gptoss are ENGAGE
+ shallow logits TOLERANCE cells only — token parity is not a valid instrument for the 26B, whose double-router
CPU differs from any float implementation by ~2.5 logits/step by construction;
fam-qwen35/fam-qwen35moe are deltanet hybrids whose batch cell asserts the per-row FALLBACK
shape — metal batch steps 0, both rows served on the single-decode path; fam-qwen2moe's
batch cell asserts the `graph` DECLINE on the planar model — shexp has no batch arm, and a
blob twin's CPU batch fallback would trip the blob-only panic). The
`kernels` suite (7 files: test_metal_{prefill,decode,rope,gemv,misc,attn,gemm}_kernels —
model-less per-class CPU-oracle units covering the FULL metal kernel census, ~2-3 min) has
no arms; remember it exists — kernel uniform/binding changes MUST update its hand-bound
dispatches. Shared fixtures (buf helpers, eyeball-dump compares, kq plane + q8 blob
builders) live in `_metal_kernel_common.das`; the prefill file predates it and keeps only its
tag-less mismatch compares local (same arity would collide — the buf_* twins are retired).
`_mtl_toy.das` is the `[metal_dispatch]` multi-kernel (kernel=) fixture — its gate in
the misc file dispatches through the GENERATED builders (kn_ rail), not hand binds.
A new gate gets a NEGATIVE CONTROL before its first commit (poison the oracle → red
with dumps → revert); size an additive poison to beat rel·env at the longest dot, and give
every derived-truth compare its own poison. A kernel with `@workgroup` state needs
`metal_set_threadgroup_memory_length` in the gate exactly as in its production encoder —
missing tgmem reads garbage silently.
The `image` suite (test_model_image — the prepared-image .dlim rail): `mechanics` (synthetic
carrier, model-free, runs in CI) `smol metal tower whisper voxtral parakeet qwen3a canary
canary-dec gemma4a gemma4uv gemma4uv-metal gemma4v gemma3v gemma4e mtower`; `gemma4e` is the E2B metal-blob
mint+map arm — the PLE go-live tripwire (`ple_check_table`, which panics when the per-layer
embedding table's plane is short) runs after the blob plane borrows, so a fresh mint and a warm
map must both clear it; `gemma4uv-metal` is the GPU tower
driver's parity/counter/knob gate for the gemma4uv embedder, Apple builds only (`--arm
gemma4uv` selects it too — arm filters match by substring); `mtower` is the whisper-class
tower-blocks gate, Apple builds only — whisper tiny + large-v3-turbo transcript-exact and
qwen3a f32-rail transcript equality, CPU vs GPU, with geometry-derived counter deltas, plus
the tower q8-decline (the serving default never dispatches the TOWER), required-mode panic, and
Conformer-absence (parakeet) cells; the arm's DECODER half is `test_whisper_metal_cross_kv` —
GPU cross-KV on the q8 serving default, transcript-exact against the CPU chain with
window/step counter deltas and the knob and quant_mode declines, required-mode, step-floor and shutdown-re-arm
contract; the voxtral arm re-saves a
5.4 GB image from cold every run by design (it IS the >2 GiB-plane IO coverage); the `metal`
arm mints/maps the blob-only metal flavor (SmolLM2) incl. the CPU-tripwire and a
teacher-forced logits-tolerance parity cell (greedy token equality is NOT a valid bar on a
135M — genuine near-ties flip on ~0.02 gaps under ~0.75 cross-backend noise). The ASR-family
arms (`parakeet` transcript-exact, `qwen3a`/`canary`/`gemma4a` element-exact planes,
`canary-dec` = the opt-in fp32 Model rail, token-exact) re-save their images from cold each
run like the voxtral arm. The canary arm carries both lanes: the f32 element-exact cell and
the q8 cell (read-time transcode — qblob/qscales/compact-blob element-exact vs a staged read).
The `image-vulkan` suite (test_model_image_vulkan, arm `vulkan`) covers the OFFLINE vulkan
bake: the runner arms DASLLAMA_GPU + a small VRAM budget so the probed config carries a
vulkan section, the DRY tier collects a role-stamped plan with no device calls (safe on
GPU-less boxes), and the flavor image round-trips the plan verbatim.
The `coverage` suite (test_kernel_coverage, arm `coverage`; arm `coverage-vk` = the vulkan
SERVING census — needs a vulkan device + `DASLLAMA_GPU=1` + `DASLLAMA_MODELS_DIR`, MoE rows
under `DASLLAMA_PARITY_FULL=1`) is the KERNEL COVERAGE census
(REVIEW: "A new GPU kernel ships with a small model in the kernel coverage suite"): the small-model zoo swept across format/graph/batch/KV axes, then a
report of per-kernel dispatch counts with LOUD WARNINGS for compiled-but-never-dispatched
kernels — never an auto-dead verdict. A zero means "nothing THIS zoo runs dispatched it",
never "unreachable": the deletion gate is a reachability AUDIT of the kernel's dispatch
predicate (can any servable model / quant / shape satisfy the guard — models and quants not
stocked locally count, and so does any family registered later); the census only proves local
coverage and points the audit. Run it BEFORE deleting any kernel; a NEW kernel's
small-model run joins it. Small-tier warnings for kernels whose carriers sit above the tier
(MoE/mx4/suppress) are expected — their census rows serve only under `DASLLAMA_PARITY_FULL=1`;
the served-count floor is asserted only on family-unfiltered runs. The vulkan half here is
the device-free rail unit; the serving vulkan census runs on the PC box.

## Model-free / no-arm tests

A model-free file — one with at least one cell that runs, not skips, with no model present and
with CPU prefill declared (`DASLLAMA_CPU_PREFILL=1`, which the runner sets for every child) —
runs under plain dastest (still `-jit`) or as a set through `run.das -- --suite model-free`,
the per-PR gate. The `model-free` list in `run.das` is the census of those files; this note is
the per-file map. A file in a model suite (every suite but `model-free`) is listed in its
suite's arm list in `run.das` instead.
Current note: `failed_dasllama_lint_require.das` — model-free, expected-compile-failure: the
facade lint trips DASLLAMA001 (code 50503) on a direct engine require with no escape.
`failed_dasllama_lint_sidedoor.das` — model-free, expected-compile-failure: the lint's tree
guard trips on a path-require resolving into modules/dasLLAMA, name prefix or not.
`test_dasllama_lint_escape.das` — model-free: `options _dasllama_internal = true` admits a
direct engine require (the lint's escape hatch).
`test_dasllama_lint_contracts.das` — model-free: the lint's ALLOWED set (a facade-only program
with no escape compiles; an internal require does not) via spawned compiles, and
`load_audio_16k_mono`'s empty-on-failure contract.
`test_think_split.das` — the reply-side reasoning matcher, model-free: every
thinking family's wire shape, whole-string and per-chunk down to 1 byte.
`test_tool_formats.das` — the per-ToolMode wire codecs (dasllama_tools), model-free: defs
serializers and call parsers for harmony/gemma4/mistral/llama_json against verbatim fixtures.
`test_scheduler.das` — the continuous-batching scheduler (dasllama_scheduler) against
`generate()` references; skips honestly without SmolLM2-135M / the MTP fixture, `-jit` only.
`test_program_roots.das` — model-free: every dasllama program root (tutorials, examples,
server tools) declares `options stack = 524288`, and every model-loading root declares its
prefill intent.
`test_audio.das` — model-free: the audio front-end units (gelu-erf, hann window, mel
filterbank, log-mel chunking, swapped swiglu); model-gated: the tower structure/oracle gates
(ultravox/voxtral/omni shapes, the mtmd all-ones encode oracles — CPU-claim cells, tower knob
pinned OFF per the fixtures section) and `test_encoder_blocks_gpu`, the qwen2audio +
voxtral 32-layer CPU-vs-GPU blocks parity on the depth-scaled bars with counter deltas —
Apple builds, `-jit`; skips honestly without the qwen2audio / voxtral mmprojs.
`test_whisper.das` — suite-less, model-gated: the whisper/parakeet/canary/gemma4a/omni oracle
cells, the ASR knob cells (`set_asr_fp32`, `set_asr_tower_fp32` — the mixed f32-enc/q8-dec
serving mode and its `asr_exec_fmt` stamp), and the q8-gate CPU-vs-CPU claims (CPU-claim
cells, wdec knob pinned OFF per the fixtures section). Runs under plain dastest.
`test_asr_verbs.das` — model-free: the family-owned ASR facade verbs (`asr_exec_fmt` /
`asr_encode_bucket`) over constructed structs and parakeet's SPM detokenizer over a toy vocab.
`test_model_specs.das` — model-free: the model-set table's shape invariants
(`../performance/model_specs.das`: unique file/display keys, official ⇒ provenance pinned,
parity-evidence shape) and the derived provenance view's invariants (unique names, sha-or-recipe,
https urls).
`test_parity.das` — suite-less, model-gated: the frozen token-for-token parity gates. ONE
generic loop drives every evidence-carrying spec of the model-set table through its declared
pinned arms (evidence is DATA on `ModelSpec.parity` — ids + arms, regenerated via
`harness/parity.sh`); hand-written arms remain only for the tied-cls bit-match and the gpt-oss
shared-load double fixture. Every compare logs decoded-text eyeball lines. Large carriers gate
on `DASLLAMA_PARITY_FULL=1` via `model_available`.
`test_parity_pregate.das` — suite-less, model-gated: the board parity pregate
(`lcpp_bench --parity`, via `parity_check` fed a controlled spec) on small carriers — the real
fixture passes, a flipped id fails, an evidence-less spec is refused, the text-form prompt
encodes through the tokenizer (E2B), the kq-native arm engages and restores the mode
(Qwen3-4B Q4_K_M). Requires the bench by relative path (like `test_tok_seed`), so it pays the
bench's engine compile.
`test_run_summary.das` — model-free: `run.das`'s own `log_summary` log scraper (last marker line
wins, end of file closes an unterminated final line, doubled marker yields one line), fed
synthetic log files from a per-process temp dir. Requires `run` by bare same-dir name.
`test_tok_seed.das` — model-free: `lcpp_bench.das`'s `tok_read_seed` corpus-header walk, required
by relative path (`../benchmarks/lcpp_bench.das`), so it pays the bench's full engine compile.
`test_sizing_helpers.das` — model-free: the sizing helpers (`reserve_resize` exact capacity,
`grow_resize` geometric reuse, `overwrite_resize` grow-only no-init) fed directly, including
grows past the `max_unreserved_size` guard that must not panic.
`test_deltanet.das` — model-free: the deltanet session-state sizing at 27B geometry through
`make_run_state` (S state + widened-conv history past the guard); model-gated: the
chunked-vs-recurrent prefill equivalence probe on Qwen3.5-0.8B.
`test_vision.das` — model-free: the vision preprocessing rail (geometry, letterbox, normalize)
bit-exact against pinned mtmd oracle hashes (dumps + mint.sh in the models dir's
`gemma4-vision-oracle/`), plus the stbimage decode/dump round-trips in a per-process temp dir.
`test_gemma4uv.das` — the gemma4uv embedder tier-1 parity vs the `-p encode` oracle dumps
(f32-mmproj-minted — the bf16 oracle carries ggml's bf16-dot activation noise); gates per-token
mean/v0..v3 at 2e-4 with the measured maxdiff logged; skips honestly without the mmproj or dumps.
On Apple builds the CPU gate pins the tower knob off, and a second test gates the GPU tier-1
encode against the same dumps on a scale-relative bar (2e-4 + 4e-3·token-rms) — exceeding it
is a red, the bar each fixture actually held is logged either way, and engage is proven per
fixture by the encodes counter.
`test_gemma4v.das` — the gemma4v ViT tower (E-series) tier-1 parity vs the `-p encode` dumps
minted on the f32-widened mmproj, CPU, `-fa off` (`mint_e2b.sh` / `mint_e4b.sh`): eight E2B
fixtures (96² cb through 672×336) on the scale-relative bar 2e-4 + 4e-3·token-rms, the measured
maxdiff logged per fixture; plus the clamp knockout (every block clamp disarmed through the
staging planes must miss the oracle — the sidecar scalars are load-bearing); plus the E4B rung —
the same tower geometry at soft-token width 2560, gated on its mmproj's four-dump seam subset
with one GPU-engage and one q8-lane fixture. Skips honestly without the mmprojs or dumps.
`test_gemma3v.das` — the gemma3 SigLIP tower (gemma-3-4b mmproj) tier-1 parity vs the
`-p encode` dumps minted on the f32-widened f16 mmproj, CPU, `-fa off`
(`gemma3-vision-oracle/mint_gemma3.sh`): the canvas is FIXED 896² (learned position table), so
the five fixtures vary content, not geometry; exact lane on the 2e-4 + 4e-3·token-rms bar, the
q8 serving lane on its measured 3.2e-1·rms bar (27 blocks, ffn served at the layout's padded
4352 width so every GEMM quantizes), plus the fixed-canvas panic gate and the carrier
sniff/exec_fmt cells. On Apple builds the CPU gate pins the tower knob off, and a GPU rung
gates two fixtures through the Metal block loop on its measured 4e-2·rms bar — engage proven
per fixture by the encodes/blocks counters, plus the knob-off decline leg (the 72-wide heads
restride to the attention tiles' 128 on the driver). Skips honestly without the mmproj or dumps.
`_vision_oracle.das` is the shared dump parser / fixture generator /
per-token compare all vision tier-1 tests use.
`test_vision_embedder.das` — model-free: the `VisionEmbedder` carrier's own arms — the sniffed
family tag, the none-carrier refusals, and the `.dlim` route — over constructed carriers.
`test_ple_check.das` — model-free: the PLE go-live tripwire (`ple_check_table`) on synthetic
Model shells — short plane trips per format arm, full plane passes, non-PLE exempt.
`test_ple_modes.das` — suite-less, model-gated (E2B Q8_0 + Q4_K_M, small tier): the PLE
token table's pinned-plane rail across serving modes — fp32 keeps the Q8_0 table on a
dedicated q8 plane (offset 0, plane == table exactly, gather rows BIT-match the file dequant,
wblob provably too small to carry the expansion) and a K-quant table on its native kq plane;
plus a greedy chat turn per serving mode on the E2B Q8_0 carrier — a q8 control, an fp32 turn,
and a `q4_0` turn, the `q4_0` one proving q4 serving has a per-layer-embedding rail.
`test_tower_helpers.das` — model-free: the shared encoder-tower helpers in `dasllama/dasllama_tower`
(clamp, row norms, f16-table GEGLU-quick, im2col, two-axis rope, avg-pool, `attention_bidir`),
each against an in-test reference.
`test_attn_span.das` — the non-causal image span (`eval_embd_ non_causal`): mask direction by
perturbation (causal row 0 blind to the last row, span row 0 sees it), classic/blocked/flash
agreement, and the flag-reset bit-exactness; stories15M fixture (test_flash's), skips without it.
`test_cpu_prefill_tripwire.das` — the CPU-prefill guard: an undeclared prefill trips, span
and causal alike (the metal rail serves spans, so a CPU-served one is a silent fallback);
same stories15M fixture, deliberately never calls `allow_cpu_prefill()` (which is why it
cannot live in test_attn_span — that file arms it in `[init]`, and why it stays out of the
runner's `model-free` suite — the runner sets `DASLLAMA_CPU_PREFILL=1`). Metal-capable builds
only; plain dastest only.
`test_vision_chat.das` — the image chat turn end to end, three families / four legs: the 12B
gemma4uv pair (the cats fixture, so `DASLLAMA_PARITY_FULL=1`), the E2B gemma4v pair (E2B Q8
decoder + bf16 mmproj — small tier, runs without the flag), the gemma-3-4b gemma3v pair (small
tier), and the gemma-3-12b pair (the same SigLIP tower at projection 3840 — large tier,
`DASLLAMA_PARITY_FULL=1`): the prompt stream shape around the splice
(marker ids, media-first, span length from the geometry) and the greedy caption, logged in
full. NOT token-parity with
llama-mtmd-cli — the oracle renders its jinja template in thinking mode while dasLLAMA's gemma-4
arm defaults to instruct, and freeform token-parity cells are banned (see below). On Apple
builds the turn also carries the tower legs: the default caption's image encode must show an
encodes delta (GPU-served), then a knob-off chat repeats the turn on the CPU embedder and
must clear the same caption bar — the caption names the cats and is a description, not a
fragment — with zero tower dispatches and the knob decline counted.

## Model loads — never the image rail (REVIEW: "A suite loads decoders with `load_model_`, never the image rail")

Suites load models with `load_model_` (the direct gguf load) — never `load_model` /
`load_model_cached` (the `.dlim` image rail). The rail stamps every mint with the box
identity (backend pin, wscale, tune manifest) and GC-purges sibling flavors; a suite child's
pinned identity differs from the serving rig's, so a suite on the rail both re-mints multi-GB
images the rig cannot use and purges the flavors the rig depends on. Image-rail coverage
(mint, map, GC, flavors) lives in the image suites alone (`test_model_image`,
`test_model_image_vulkan`).

## Metal fixtures — driver knobs and the two-model pattern

(REVIEW: "A cell whose claim is a CPU-served or f32-decoder leg pins the covering driver knob
OFF for that leg and restores it after.") The
hooks are on by default: they flip a q8 leg to the GPU silently, and an f32 leg records a
quant_mode decline that panics under required mode — either way the cell stops measuring what
its name says.

The Metal drivers serve ONLY blob-form models (`convert_model_to_metal_blob` /
metal-flavor images), and CPU inference on a blob model PANICS. Every CPU-vs-GPU arm
therefore runs a PLANAR model for CPU stages and its `blob_twin(t, path, seq_cap)` for
override-selected stages — sessions are geometry-bound, so one session spans both models
(CPU prefill on the planar model, GPU decode on the twin, etc.). Decline-reason cells keep
the planar model: capability reasons (`feature`, `graph`, `shape`, ...) out-rank the
`planar` decline in every gate, and the planar CPU fallback serves quietly. The prefill
npos POLICY window is planar-only now (a blob model serves any npos — no CPU fallback
exists); the legacy quantized-X prefill rail is dead (`set_metal_prefill_mulmm_legacy`
forces a `planar` capability decline — the required-mode panic cell uses it).

## Family filter (profiling cadence)

`--family <tokens>` (env `DASLLAMA_TEST_FAMILY`, comma list) composes with `--arm`: only
model blocks tagged with a listed family run — `family_on(t, name)` in
`_model_tier.das`, EXACT token match, loud `t |> skip` like the arm filter. Model-free blocks
(the `kernels` suite, the image `mechanics` arm) carry no tag and always run. Family tokens
today: `llama` (all four metal suites + the image smol arm), `qwen2`, `qwen3`, `phi3`,
`gemma2`, `gemma3`, `gemma4`, `qwen3moe`, `gemma4moe`, `gptoss`, `qwen35`, `qwen35moe`, `qwen2moe` (the support-matrix family cells), `gemma`,
`ultravox`, `whisper`, `voxtral`, `parakeet`, `qwen3a`, `canary`, `gemma4a` (image suite arms),
`gemma4e` (support-matrix rows under `fam-gemma4e` — E4B PARITY_FULL-gated; E2B Q8_0 and
Q4_K_M small-tier always-on, carrying the per-layer-FFN-width and blob-kq-PLE-gather coverage.
Both E2B rows assert parity through their forced-feed cells, not token equality, because
freeform continuations from a 2B sit on near-ties; plus the coverage-census E4B row; `gptoss`/`qwen2moe`/`qwen3moe` carry census rows too — all PARITY_FULL-gated).
When profiling one family across formats, gate each round with
`--arm <arms> --family <fam>` instead of the whole zoo. Tag every NEW model-loading block
with its family or it silently joins every family's gate.

## Model tiers

`model_available(t, path)` gates by size: models over 6GiB (the 8B, the 70B) run only under
`DASLLAMA_PARITY_FULL=1` (set that env explicitly with an `--arm` run when a change genuinely
needs the large tier — `--full`, which used to set it, is refused). A run reporting SKIPPED
for those arms is correct, not a failure. The 70B is further restricted: no CPU-control
batch parity against it (streaming 40GB on the CPU while the GPU has the same bytes wired
OOM-kills a 64GB box) — 70B batch coverage is ENGAGE-only in the support matrix; the
batched code paths get their parity on small models via pins (e.g.
`set_metal_batch_addrms_unfused`).

## Log discipline

Always capture COMPLETE logs (the runner does this); grep afterwards, never at capture time —
a capture-time filter once hid the exact proof line a verification run existed to produce.
When a fixture claims a size/depth property ("2030 tokens", "crosses 2048"), assert the
actual number in the test; a resize cap is not evidence.
THE EYEBALL RAIL (REVIEW: "Every test that compares generated tokens, ids, or logits logs the decoded text for both sides"): every token-for-token generate cell logs both decoded
streams (`log_gen_texts` in `_model_tier.das`), and every logits-tolerance cell logs a decoded
text form (forced stream + the GPU's greedy would-be picks, or both next-token pieces) — read
the text before trusting a red or a suspicious green; a near-tie synonym flip and real garbage
look identical as id diffs and completely different as text.

## Stale truth caches (`<model>.ref.<key>.tsv`)

`cached_ids`/`cached_vals` pin a CPU trajectory into `<gguf>.ref.<key>.tsv` beside the model.
FREEFORM-prompt caches sit on sub-noise near-ties, so any numerics-adjacent master merge can
legitimately move the CURRENT CPU trajectory off the cached one — the parity assert then fails
with the GPU side actually CORRECT (it matches today's CPU). Before declaring a fam-row red a
regression: (1) stash + clean-tree rerun (same red ⇒ not your diff), (2) `mv` the cell's `.ref`
tsv aside and rerun — a fresh-truth green means stale cache, keep the refreshed tsv. Counting
caches are tie-proof by construction and should NOT move; a counting-cache mismatch is a real
red.

Freeform TOKEN-parity cells are contract-breaking and banned: new freeform coverage uses the
FORCED-FEED logits-tolerance form (the k4 freeform cell, cached stream `gen_free_n128`), never
token equality. For that form the cache is a FEED, not a truth: both sides force through the
same tokens, so a stale feed stays a valid instrument and the stale-cache red class does not
exist for it. Counting cells stay token-exact.

## Standalone schema tests (model-free)

`test_bench_records_schema.das` (the record store: round-trip, upsert identity, annotations)
and `test_exchange_schema.das` (the exchange validator: sweeps the ENTIRE in-tree
records/sidecar corpus, so a writer-schema change reds here first) run directly under dastest
with `-jit` — no runner, no arms, no models.

## Out-of-folder test files (the checklist's placement ledger)

Every `[test]` file requiring a `dasllama/*` module outside this folder, each with its reason:
- `utils/dasllama-server/test_openai_server*.das` — require the server by bare same-dir name
  (the hyphenated directory is unreachable by path require).
- `utils/dasllama-server/test_exchange_client.das` — requires `dasllama/dasllama_exchange` by
  registered name (nothing pins it to that directory); it stays beside the server suites
  because its fixed test port is coordinated with theirs (see its `TEST_PORT` note).
- `modules/dasLLVM/tests/test_grid.das`, `test_tune.das`, `test_tuned.das` — the tune
  framework's own tests; they require `dasllama/dasllama_tune` (the tuner contract) and
  live with dasLLVM under `modules/dasLLVM/REVIEW.md`.
- `modules/dasLLAMA/benchmarks/matmul/test_matmul_par.das` — the bench self-check, requiring
  `matmul_variants` by bare same-dir name.

The ladder tests (`utils/internal/dasllama-ladder/test_*.das`) require no engine module and answer to
that folder's own `REVIEW.md` — not dasLLAMA tests, not ledger entries.
