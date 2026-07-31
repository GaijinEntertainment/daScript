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
kq cont dim qkv`. Support matrix: `cells-q8 window cells-s16 mode kq dim8b dim70b` + the
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
`kernels` suite (test_metal_prefill_kernels — model-less kernel units, ~80s) has no arms;
remember it exists — kernel uniform/binding changes MUST update its hand-bound dispatches.
The `image` suite (test_model_image — the prepared-image .dlim rail): `mechanics` (synthetic
carrier, model-free, runs in CI) `smol metal tower whisper voxtral parakeet qwen3a canary
canary-dec gemma4a`; the voxtral arm re-saves a
5.4 GB image from cold every run by design (it IS the >2 GiB-plane IO coverage); the `metal`
arm mints/maps the blob-only metal flavor (SmolLM2) incl. the CPU-tripwire and a
teacher-forced logits-tolerance parity cell (greedy token equality is NOT a valid bar on a
135M — genuine near-ties flip on ~0.02 gaps under ~0.75 cross-backend noise). The ASR-family
arms (`parakeet` transcript-exact, `qwen3a`/`canary`/`gemma4a` element-exact planes,
`canary-dec` = the opt-in fp32 Model rail, token-exact) re-save their images from cold each
run like the voxtral arm.
The `image-vulkan` suite (test_model_image_vulkan, arm `vulkan`) covers the OFFLINE vulkan
bake: the runner arms DASLLAMA_GPU + a small VRAM budget so the probed config carries a
vulkan section, the DRY tier collects a role-stamped plan with no device calls (safe on
GPU-less boxes), and the flavor image round-trips the plan verbatim.
The `coverage` suite (test_kernel_coverage, arm `coverage`) is the KERNEL COVERAGE census
(CODEREVIEW rule 17): the small-model zoo swept across format/graph/batch/KV axes, then a
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

## Model loads — never the image rail (CODEREVIEW rule 20)

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
`gemma4e` (the coverage-census E4B row).
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
THE EYEBALL RAIL (CODEREVIEW rule 18): every token-for-token generate cell logs both decoded
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
