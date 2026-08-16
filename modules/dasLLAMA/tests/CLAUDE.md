# modules/dasLLAMA/tests — testing discipline

The Metal suites here are wall-time-expensive (model loads dominate; a full pass holds 40GB
GGUFs). The rules below exist because one session spent 5.75 of 6 hours re-running full suites
to verify one-arm fixes. They are enforcement, not advice.

## Run suites ONLY through the runner

```
./bin/daslang -jit modules/dasLLAMA/tests/run.das -- --arm <filter> [--suite decode|prefill|matrix|all] [--family llama]
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
kq cont span dim qkv` (span = the non-causal media eval shape, head + embd span, per codec). Support matrix: `cells-q8 window cells-s16 mode kq dim8b dim70b` + the
family matrix `fam-qwen3 fam-qwen2 fam-phi3 fam-gemma2 fam-gemma3 fam-gemma4 fam-qwen3moe
fam-gemma4moe fam-gptoss fam-qwen35 fam-qwen35moe fam-qwen2moe` (needs-derivation pins +
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
canary-dec gemma4a gemma4uv gemma4uv-metal mtower`; the gemma4uv-metal arm is the GPU tower
driver's parity/counter/knob gate and `mtower` the whisper-class tower-blocks gate — whisper
tiny + large-v3-turbo transcript-exact and qwen3a f32-rail transcript equality, CPU vs GPU,
with geometry-derived counter deltas, plus the q8-decline (the serving default never
dispatches), required-mode panic, and Conformer-absence (parakeet) cells (all Apple builds;
`--arm gemma4uv` selects gemma4uv-metal too by substring); the voxtral arm re-saves a
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

Suite-less files run under plain dastest (still `-jit`) — no arm, no family tag, no runner.
New suite-less files register on this note (REVIEW: "A suite-less file's `CLAUDE.md` entry is
accurate in the same change"); suite members register in their suite's arm list via `run.das`
instead.
Current note: `test_think_split.das` — the reply-side reasoning matcher, model-free: every
thinking family's wire shape, whole-string and per-chunk down to 1 byte.
`test_tool_formats.das` — the per-ToolMode wire codecs (dasllama_tools), model-free: defs
serializers and call parsers for harmony/gemma4/mistral/llama_json against verbatim fixtures.
`test_scheduler.das` — the continuous-batching scheduler (dasllama_scheduler) against
`generate()` references; skips honestly without SmolLM2-135M / the MTP fixture, `-jit` only.
`test_program_roots.das` — model-free: every dasllama program root (tutorials, examples,
server tools) declares `options stack = 524288`, and every model-loading root declares its
prefill intent.
`test_audio.das` — the audio tower structure/oracle gates (ultravox/voxtral/omni shapes, the
mtmd all-ones encode oracles — those pin the tower knob OFF; the hook would silently flip them
to GPU) plus `test_encoder_blocks_gpu`: qwen2audio + voxtral 32-layer CPU-vs-GPU blocks parity
on the depth-scaled bars, counter deltas asserted.
`test_asr_verbs.das` — model-free: the family-owned ASR facade verbs (`asr_exec_fmt` /
`asr_encode_bucket`) over constructed structs, parakeet's SPM detokenizer over a toy vocab, and
the `fetch_models.das` provenance-manifest invariants.
`test_run_summary.das` — model-free: `run.das`'s own `log_summary` log scraper (last marker line
wins, end of file closes an unterminated final line, doubled marker yields one line), fed
synthetic log files from a per-process temp dir. Requires `run` by bare same-dir name.
`test_tok_seed.das` — model-free: `lcpp_bench.das`'s `tok_read_seed` corpus-header walk, required
by relative path (`../benchmarks/lcpp_bench.das`), so it pays the bench's full engine compile.
`test_sizing_helpers.das` — model-free: the sizing helpers (`reserve_resize` exact capacity,
`grow_resize` geometric reuse, `overwrite_resize` grow-only no-init) fed directly, including
grows past the `max_unreserved_size` guard that must not panic.
`test_vision.das` — model-free: the vision preprocessing rail (geometry, letterbox, normalize)
bit-exact against pinned mtmd oracle hashes (dumps + mint.sh in the models dir's
`gemma4-vision-oracle/`), plus the stbimage decode/dump round-trips in a per-process temp dir.
`test_gemma4uv.das` — the gemma4uv embedder tier-1 parity vs the `-p encode` oracle dumps
(f32-mmproj-minted — the bf16 oracle carries ggml's bf16-dot activation noise); gates per-token
mean/v0..v3 at 2e-4 with the measured maxdiff logged; skips honestly without the mmproj or dumps.
On Apple builds the CPU gate pins the tower knob off, and a second test runs the GPU tier-1
ATTEMPT: same dumps, scale-relative rung (2e-4 + 4e-3·token-rms), rung reached logged, engage
proven per fixture by the encodes counter.
`test_attn_span.das` — the non-causal image span (`eval_embd_ non_causal`): mask direction by
perturbation (causal row 0 blind to the last row, span row 0 sees it), classic/blocked/flash
agreement, and the flag-reset bit-exactness; stories15M fixture (test_flash's), skips without it.
`test_cpu_prefill_tripwire.das` — the CPU-prefill guard vs the span: the non-causal span is
EXEMPT (CPU is its only correct arm) while an undeclared causal prefill still panics; same
stories15M fixture, deliberately never calls `allow_cpu_prefill()` (which is why it cannot
live in test_attn_span — that file arms it in `[init]`). Metal-capable builds only; no suite arm.
`test_vision_chat.das` — the image chat turn end to end (12B + mmproj + the cats fixture, so
`DASLLAMA_PARITY_FULL=1`): the prompt stream shape around the splice (marker ids, media-first,
span length from the geometry) and the greedy caption, logged in full. NOT token-parity with
llama-mtmd-cli — the oracle renders its jinja template in thinking mode while dasLLAMA's gemma-4
arm defaults to instruct, and freeform token-parity cells are banned (see below). On Apple
builds the turn also carries the tower legs: the default caption's image encode must show an
encodes delta (GPU-served), then a knob-off chat repeats the turn on the CPU embedder — same
caption floor, zero dispatches, the knob decline counted.

## Model loads — never the image rail (REVIEW: "A suite loads decoders with `load_model_`, never the image rail")

Suites load models with `load_model_` (the direct gguf load) — never `load_model` /
`load_model_cached` (the `.dlim` image rail). The rail stamps every mint with the box
identity (backend pin, wscale, tune manifest) and GC-purges sibling flavors; a suite child's
pinned identity differs from the serving rig's, so a suite on the rail both re-mints multi-GB
images the rig cannot use and purges the flavors the rig depends on. Image-rail coverage
(mint, map, GC, flavors) lives in the image suites alone (`test_model_image`,
`test_model_image_vulkan`).

## Blob-only Metal fixtures (the two-model pattern)

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
`gemma4e` (the coverage-census E4B row; `gptoss`/`qwen2moe`/`qwen3moe` carry census rows too — all PARITY_FULL-gated).
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
THE EYEBALL RAIL (REVIEW: "Every test that compares logits also logs decoded text"): every token-for-token generate cell logs both decoded
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
- `utils/dasllama-server/test_exchange_client.das` — requires its subject by relative path,
  but coordinates its fixed test port with the serving-leg suites' ports in that directory
  (see its `TEST_PORT` note).
- `modules/dasLLAMA/benchmarks/matmul/test_matmul_par.das` — the bench self-check, requiring
  `matmul_variants` by bare same-dir name.

The ladder tests (`utils/dasllama-ladder/test_*.das`) require no engine module and answer to
that folder's own `REVIEW.md` — not dasLLAMA tests, not ledger entries.
