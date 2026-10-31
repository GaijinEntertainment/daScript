# modules/dasLLAMA/tests - testing discipline

The Metal suites here are wall-time-expensive (model loads dominate; a full pass holds the
largest GGUFs the box stocks), so the rules below are enforcement, not advice - an ad-hoc
full-suite run turns a one-arm fix into an afternoon.

## Run suites ONLY through the runner

```
./bin/daslang -jit modules/dasLLAMA/tests/run.das -- --arm <filter> [--suite decode|mtp|prefill|matrix|kernels|image|image-vulkan|coverage|all] [--family llama]
./bin/daslang -jit modules/dasLLAMA/tests/run.das -- --suite model-free        # the per-PR gate that needs no models - runs the same on a bare box, no --arm
./bin/daslang -jit modules/dasLLAMA/tests/run.das -- --changed [--base origin/master]   # the per-PR run on a box with models, and after an edit: the files the changed paths reach
./bin/daslang -jit modules/dasLLAMA/tests/run.das -- --suite stocked           # every model-gated file, no --arm; a core-module change reaches every one of them through --changed
./bin/daslang -jit modules/dasLLAMA/tests/run.das -- --suite stocked --exclude test_ple_modes   # the iteration form - drops the PLE file
DASLLAMA_GPU=1 DASLLAMA_CUDA=1 ./bin/daslang -jit modules/dasLLAMA/tests/run.das -- --changed --only cuda,segments,regions,resident_moe,embeddings   # the CUDA home's pass: the run's CUDA files, the segmented window, the regions rig and the resident MoE files
./bin/daslang -jit modules/dasLLAMA/tests/run.das -- --changed --shard 1/3             # one of three boxes: every third file of the same run, from the first
./bin/daslang -jit modules/dasLLAMA/tests/run.das -- --area audio            # one area: audio | vision | tts | translate | llm | infra (comma list)
```

An AREA is what someone working on audio, vision, TTS, the LLM engine or the tooling runs after an
edit: every model-free and stocked file of the area (`area_tests` in `run.das`, a file may sit in
two) plus the `image` suite's arms the area owns (`area_image_arms`; the `gemma` arm - the E2B
image round-trip - belongs to no area, because as an `--arm` substring it would drag every other
`gemma*` arm along; reach it with `--suite image --arm gemma`). `--changed` plans by file from
`git diff --name-only <base>` (`--base`, default `origin/master`; a base git cannot resolve is
refused) plus the module's untracked files (`reach_for_path` in `run.das`): a test file reaches
itself; a `tests/_*.das` fixture the test files whose `require _<stem>` reaches it, through the
fixtures between (a fixture no test names by require falls to its areas); an engine module its
`MODULE_AREAS` areas whole but `llm`, where it reaches its `MODULE_WITNESSES` row (a leaf module's
own carriers and kernel cells) or, with no row there, the CORE set - every model-free `llm` file
and the `LLM_CORE_WITNESSES`, one stocked witness a family and home, the codec rig, the regions
form, the segmented window, the CPU chains; `benchmarks/`'s `.das` the bench rows' files; the
checked-in `models/` weights `audio` whole; any non-`.das` file - `run.das` included -,
`performance/` and `harness/` `infra` whole; any other path under the module every area, `llm` as
the core. Each path prints as a `CHANGED path -> ...` line, the `PLAN` line counts the files and
says when the core changed, and a changed model-suite file prints a hint instead, because its arms
are the caller's to scope. A core change is the per-edit gate only: the whole stocked set
(`--suite stocked`, sharded over the fleet, `../harness/pod_fleet.das`) is owed before the PR, and
the `NEXT` line says so. The fleet's pods share one models directory, so its runs keep the image
rail off (`DASLLAMA_IMAGE=0`, the tool's default environment) and the image suite's arms the plan
owns run on one box with the rail on, through dastest's `--test` with `DASLLAMA_TEST_ARMS` set to
them; the PR cites that run beside the shards. The run ends with `REPORT` (files, failures, cells skipped - a filtered
arm, an absent device, or a missing model - and files excluded) and `NEXT` (what is still owed).

`all` (the default) runs the decode, MTP and prefill parity files, the support matrix, the kernel
files and `test_model_image` - it omits `test_mtp_gemma_drafter.das`, `image-vulkan`, `coverage`,
`model-free` and `stocked`.

Every suite but `model-free` and `stocked` needs `--arm`. `--full` parses and is then refused, so
`--arm` is the only way in. `--suite model-free` and `--suite stocked` take neither - each is a
whole gate; `--exclude <substr,...>` drops the files whose name contains a token and names each
on an `EXCLUDED` line, so a trimmed run cannot read as full; `--only <substr,...>` keeps the files
whose name contains a token and drops the rest, each kept file on an `ONLY` line and the tokens on
the `REPORT` line - the CUDA home's pass is `--changed --only cuda,segments,regions,resident_moe,embeddings` (the CUDA files, the segmented
window, the regions rig and the resident MoE files, whose fixtures read the armed home's witnesses) under
`DASLLAMA_GPU=1 DASLLAMA_CUDA=1`, beside the Vulkan home's whole run, never in its place (the
other files run the Vulkan home's tests on a home that serves the dense slice alone, and red on
its declines). `--shard i/n` splits one run over n
boxes: each box runs the same command with its own `i` and takes every n-th file of the run's
sorted list from the i-th, so the shards partition the run and spread its costly neighbours; the
`SHARD` line counts the shard's files, the `REPORT` line carries `shard i/n`, and the `NEXT` line
names the shards still owed - a run is whole only when every shard's `REPORT` is in hand.
`--no-tune` composes with any suite:
every child runs with `DAS_TUNE_POLICY=reference`, so the `[tune]` families and the `[tuned]`
hints fall to their reference bodies - the portable tier's arm of the gate. That arm also runs
with `DASLLAMA_IMAGE=0`: the reference policy is a different box identity, and a `.dlim` minted
under it would GC-purge the box's tuned images, so the runner refuses `--no-tune` with the
`image` and `image-vulkan` suites, and the image-reading cell outside them
(`test_audio_embedder`'s direct-route cell) skips on the knob and keeps its coverage on the tuned
arm. The runner redirects
the COMPLETE output to a log file, and prints that path on the DONE line. It owns the dastest
timeout - 3600 s per child on the stocked gate and under `--changed`, where the large tier's parity
file alone runs past 20 minutes and the llama resident file's CPU chains run past 1200 s on a cold
JIT cache; 1200 s in arm mode - and repeats a file only when `--nreps` is
passed explicitly (default 1, never best-of-N). Every child runs `-jit -module-cache .cache/daslang/module_cache/dastest.dascache`
(dastest's own module graph) and `--module-cache-dir .cache/daslang/module_cache/dastest_tests/<size>-<mtime>` (the
test program's AST record, one per file, the folder keyed by the daslang binary): a record serves the child's
unchanged module prefix, so an engine test file that parses in ~90 s cold opens in a few seconds warm, and an
engine edit re-parses the edited module and the modules after it alone. The `CACHE` line names the folder; a
run on the runner's own binary removes the folders of earlier binaries, whose records can never serve again,
and a `--das` dry run keeps them.
No preflight tier runs the per-PR runs. `preflight -- --only dasllama-model-free` runs the
model-free suite; the `--changed` run has no preflight lane; `-- --only dasllama-stocked` runs the
whole stocked suite.

## The iteration loop

- Before launching any suite, state what the change can affect. A default-off knob or a
  comment edit does not need a rerun.
- Fixing or adding one arm runs exactly that arm: `--arm arm12 --suite decode`.
- A cell of `test_vulkan_kernels.das` (model-free, so dastest may run it directly) has no arm: a
  kernel edit runs its own cells through dastest's name filter, `--test-names <prefix>`
  (repeatable, a `[test]` function-name prefix) - the whole file (~15 min) runs once before the
  commit, never per edit. The Metal kernel files sit in the `kernels` suite and run through the runner.
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
arm6-churn arm6b-capfit arm6c-capfit-refuse arm7-q8kv arm7b-tq4kv arm8-s16 arm9-reload arm10-kq arm11-depth arm12-dim
arm13-conc arm14-poison arm15-spec-hint` (arm14 = the shared-region collision gate: a foreign GPU
prefill must not degrade a later forced-feed decode - Qwen2.5-0.5B, its own `[test]` block; arm6b = two streams whose K/V passes a 1 MiB idle mirror cap many times
over still take the batched GPU step, token for token with the CPU control, the cap put back as found, and with the device up the room `device_kv_max_rows`
answers is the model's own context; arm6c = the device's largest buffer stood in for at 1 MiB by the probe seam
(`set_metal_mirror_device_max_probe_`): the batched step declines by name, the refusal said once and cleared by the
mirrors' reset, the streams still the CPU control's off the declined step; arm15 =
the single-row driver's greedy chain stands down for a decode sampled at temp > 0 and rides for a
greedy one; arm11 = the 2030-token depth cell, which also holds the single row's attention form by
the kernel census - the fused single-pass stamp once a layer a step on the f16 mirror and its q8_0
twin on the q8_0 mirror, no chunked dispatch on either; the same deep steps with the lever off
(`set_metal_attn_d(false)`) are the control: the chunked pair serves every deep step and the fused
stamp none, the tokens still the CPU's),
batch test: `batch` (whole test), `batchB7-partd`, `batchB8-kq` (the kq batch arm on Llama-3.2-1B
Q4_K_M: B=2, B=3, B=9 and B=12 batched greedy token for token the CPU control, then the half-x leg - the
`kq_rows_half_k4` and `kq_rows_half_k6` crowns pinned with the rail on (`set_metal_kq_half`), B=2 /
B=3 / B=8 token for token the CPU control with k4's two-, four- and eight-column half forms each
counted at least once and the k6 half stamps counted exactly where the rail-off run served k6's f32
small-batch forms, the rail off keeping the f32 forms with no eight-column half dispatch, every leg's
steps the batched driver's, the crowns and the rail put back as found), `batch-ff` (real-text forced
feed, GPU single vs GPU batch at B=2/B=4 on identical tokens, logits tolerance); `batch-mixed` (its own test,
`test_metal_batch_mixed_sites`, family `qwen35`: Qwen3.5-0.8B-Q4_K_M-q8out, a Q8_0 deltanet out plane beside K-quant
qkv planes, at B=3, B=9 and B=12 every one of twelve steps the batched driver's with no batch decline, the streams token
for token the GPU single-row step's, the batched leg first since it grows the recurrent mirror pool to the batch; nine
and twelve rows ride the GEMM tier, whose 32-row M pad the recurrent row planes hold); `batch-per32` (its own test,
`test_metal_batch_per32_sites`, families `llama` / `qwen2`: Llama-3.2-1B Q5_1 and Qwen2.5-0.5B IQ4_NL, the `-local` mints of
`../performance/model_specs.das`, whose dense planes load as q51 / iq4nl32, at B=2, 3, 5, 8, 9 and 12 every one of twelve
steps the batched driver's with no batch decline, each per-32 small-batch dispatch carrying all B rows
(`metal_b32_mvb_stats`) and at least one a per-32 plane a step to eight rows, none past them, the narrow planes on the K-split
stamps to eight rows (`metal_b32_ks_count`), the streams token for token
the GPU single-row step's). The whole test is gated on
`batch` first, and a filter token matches an arm name as a substring OF THE NAME, so `--arm batchB8-kq`
alone skips the whole test (`batchB8-kq` is not a substring of `batch`); `--arm batch` runs every batch
arm, and is the way to reach a sub-arm.

MTP parity (`test_metal_mtp_parity.das`, suite `mtp`): `mtp-ffh-<tag>` = the half-x crowns (k4, k5, k6) pinned
over the counting walk at depth 2: the verify's two-to-eight-row K-quant sites, the MTP head's own q/k/v among
them, read the f16 twin, and the rounds the rail drafts hold up to the uncrowned walk's within a fifth - a head fed
a stale twin drafts garbage and the count collapses while the verify still lands the plain tokens; the plain leg
runs with every half-x crown of the box's profile removed and the rail off, so the two legs differ in the crowns
alone, and on a carrier whose q/k/v/o or dense FFN planes carry a k4 / k5 / k6 format the half-x forms' census
count is asserted above zero (a leg that dispatched none measured the f32 forms twice); the crowns and the rail
put back as found. `mtp-census-<tag>` = the served twin's format census, every site the file carries in its own format
(the planar fixtures' tags; the split-shard served-only `3.8fn` has none). `mtp-warm-<tag>` = the
draft head's prompt warm adds only the head's own projections, norms and rope-and-store against the
same prefill with the warm off, the rope count rising as the control.
`mtp-ctrl-<tag> mtp-ff-<tag> mtp-ffk-<tag> mtp-vff-<tag> mtp-count-<tag>` per fixture tag
`0.8b 4b 9b 27b 35b 35b-iq4xs 3.8-27b 3.8fn` (4b = Qwen3.5-4B-MTP
Q8_0, the mid tier; 35b-iq4xs = Qwen3.6-35B-A3B-MTP-IQ4_XS-pure, large tier, the format census's carrier, its routed
head's eh_proj and expert stacks IQ4_XS; 9b = Qwen3.5-9B-MTP UD-Q5_K_XL, large tier; 3.8-27b = the Qwen3.8-27B
trunk + its split Q8_0 head; 3.8fn = Qwen3.8-Flash-Next UD-IQ4_XS + its split `shared` Q8_0 head,
large tier, the SERVED-ONLY form - planar and blob together do not fit the box, so every arm runs
GPU against GPU on the twin alone, mapped under the rig's identity with no pins and no mint of its
own, and its ff arms hold the plain-MoE bar (2.0: the rows and single GEMV forms round the router's
logits apart and a near-tie flips an expert - prose-a reads 0.89 with zero argmax flips); its
`mtp-dff-3.8fn` arm runs the batch rail's distinct-session step against the single step on the twin,
so the cell's "cpu-ref" row is the GPU single step; `dff_pair` sizes the recurrent-mirror cache to
every twin plus the reference row before the prefills mint their mirrors); the 4b and
9b blocks carry the `qwen35` family tag; every other fixture
loads its served twin through `load_model_cached`, so the file mints a `.dlim` per carrier - ledgered at
`../ARCHITECTURE_IMAGE_LEDGER.md#suite-image-mints`;
ctrl = plain-vs-plain forced feed must be bit-identical; ff = the
verify's row 0 vs the plain GPU step, forced-feed logits tolerance on two prose openers, with the
one-step-off control (row 0 against the PREVIOUS plain step must land outside the bar at every
step); ffk = the
same at depth 2 and 4, every round a k+1-row verify plus the recurrent replay, which re-runs row 0
from the pre-verify recurrent state; vff = the same-slab batch verify's four rows vs four plain
steps, a driver-against-itself compare (on a hyper-connection carrier the rail's `graph` decline instead, its same-slab rows being one session); count = speculative free-run == plain free-run, token-exact, counting prompt, at depth 1, 2
and 4, and by the kernel census the verify's rows on the fused single-pass attention wherever the
kernel carries the model's shape (every head class of 64, 96, 128, 256 or 512 on the f16 mirrors, the
three stamps' counts summed, no chunked dispatch) and on the chunked pair elsewhere, both counts
logged; at depth 1 every drafted round counted one command buffer (`metal_mtp_chained_rounds`) on a table the device
gathers, none on an n-gram model or under the assistant's CPU-oracle drafts (`mtp-count-cpu`) (the controls); `mtp-count8-<tag>` = the same at depth 8, the deepest round - nine verify
rows, a carrier with a rows site - attention, dense or shared-expert FFN, deltanet projection or classifier - on a superblock
plane the box leaves on the small-batch forms counting its nine-row sites on one small-batch dispatch each
(`metal_kq_mvb_wide_count` above zero) and a carrier with none (a Q8_0 one, or one whose formats the box's `kq_rows_<fmt>`
crowns keep on single passes) none;
`mtp-codec-<tag>` = the depth-2 counting walk on q8_0, tq4 and f32 sessions, the verify's rows storing and reading
the session's codec (the codec's fused-attention stamps counted), speculative == plain token for token and drafts
accepted; every arm fails, never skips, on a carrier the box holds that the Metal load or a driver declines;
`mtp-sampled-<tag>` = the sampled accept walk - speculative free-run == plain SAMPLED free-run,
seeded, at depth 1 and 2, temp 0.7 / top-k 1 / penalty 1.1 so every draw is the penalized argmax
and the stream stays deterministic; on a verify tag it rides the assistant round inside the
`mtp-count-<tag>` attach); `mtp-select-<tag>` = the selection landing - a sampled stream (temp 0.7, top-p 0.9, seeded) at
depth 2 through the device's selection decodes the rows-landed stream token for token with the same drafts accepted,
every served step's selection landed (`sel_selections`) and none reaching past it (`sel_fallbacks`)). The same
file carries the BATCH RAIL's parity arms per verify-fixture tag `l1b g12 q30 q38
g26` (Llama-3.2-1B, gemma-4-12B, Qwen3-30B-A3B, Qwen3.8-27B head-less - one recurrent session's
rows the rail serves as one group over its mirror, gemma-4-26B-A4B): `mtp-dff-<tag>` = distinct sessions, GPU batch step vs GPU single step
at B=2/B=4 on identical real-text tokens plus one CPU reference row (the batch rail's logits gate -
the support matrix's batch cell only proves ENGAGE); `mtp-vff-<tag>` takes both tag families;
`mtp-vff1-<tag>` = one row through the batch driver (the encoder alone, no row mixing);
`mtp-vff5-<tag>` = five rows through the batch driver; `mtp-vff9-<tag>` = nine rows, the deepest
verify the round can ask for (MTP_MAX_ROWS); `mtp-vffc-<tag>` = the nine rows again under a 64 MB
idle mirror cap, the drivers shut down first so the arena comes back sized by the rows' own demand -
the rows share one slice, and the step must serve;
`mtp-dff8-<tag>` = distinct sessions at B=5 and B=8. `mtp-count-<tag>` on a verify tag attaches the `mtp-*` assistant sidecar beside a
gemma-4 target first (skips when none) and runs the counting free-run at depth 1, 2, 4; the
diagnostic arms `mtp-count-cpu` (drafts through the CPU oracle), `mtp-count-trace` (drafts vs
truth logged per round) and `mtp-count-pre` (the pre-norm residual as the drafter's h) ride along
with the fixture arm. Bars: 0.5 dense, 2.0 plain MoE, 24.0 gemma-MoE = the support matrix's own
Q4_K_M forced-step bar (the routed experts amplify a float-class difference at a near-tie into
the K/V history: floor 0.003-0.01, isolated spikes to 11, argmax never flips); a K-quant carrier's nine-row verify, and its
two-to-eight-row verify where a rows site's half-x forms serve on the box, hold `MM_TOL` (1.0: both stage x to half) at the
least, each such bar with its control - the same verify with its GEMV stage knocked out outside it; every vff compare counts
each plain step a Metal decode (`metal_decode_stats().decodes`) beside the batch side's served steps, its control's too, and
the batch verify's steps counting none (the decode count's control). Each fixture's
trunk-and-served-twin pair prefills on the GPU twin through the metal prefill override (a planar
27B prefill costs minutes per session), so the fixture shuts down BOTH drivers before its leak
gate.

The `mtp` suite also carries `test_mtp_gemma_drafter.das` (the gemma-4 assistant drafter, also in the stocked suite; each
arm's gates and the served arms' GPU home: the file's entry below), its arms:
- `mtp-gdraft-refuse` - the loader's no-panic refusal contract and the sidecar resolver's refusals (a head path, an absent
  sibling, a split NextN head under the drafter's name - the same name carrying `gemma4-assistant` found); the attach's target
  check on a gemma-4-26B-shaped shell - a trunk of the drafter's own geometry passing, a two-layer trunk and a capture layer
  of another head size or kv head count each refused by its own clause; the facade's refusal text through the assistant
  registry - every registered backend's own early refusal of a planar shell with no resident driver named, and
  `mtp_attach_refusal`'s no-backend text; the Vulkan attach's driver check over a facts record (`vk_drafter_trunk_why`: a
  driver holding every fact passing, then each fact's own refusal in its order); the Vulkan round's bind ladder
  (`assist_bind_rung`: the driver, the codec, the context, a region unpinned and untaken this tick); the Metal attach's
  refusal of a metal-blob shell whose token table the device gathers no row of; and the loader's decline of the Q4_0 head
  `mtp-gemma-4-26B-A4B-it-Q4_0.gguf` (a companion row of `../performance/model_specs.das`), naming the tensor's type and the
  Q8_0 it serves;
- `mtp-gdraft-ple` - the attach refuses the E2B Q8_0 trunk naming its per-layer embeddings and the facade attaches nothing;
  the same trunk with no per-layer embeddings refusing on another clause is the control;
- `mtp-gdraft-load` - the 440 MiB `mtp-gemma-4-26B-A4B-it-Q8_0.gguf` sidecar's whole struct: geometry, per-layer head/kv
  classes, blob and fblob sizes against the tensor sum, 32-byte tensor alignment, the p-RoPE table;
- `mtp-gdraft-cpu` - the CPU oracle's one step over a live gemma-4-26B-A4B session: finiteness and range only, no numeric
  oracle;
- `mtp-gdraft-gpu` (served) - attach, then one device draft step against the CPU oracle at a short anchor and at one position
  past the sliding window: the same token, logits within 0.25 and h_next within 0.05 on Metal, both within 1.0 on the
  discrete homes' Q8_0-fed rows (the oracle's own flip envelope reads 0.38-0.62), both argmax pieces logged; the control is the oracle drafted off another seed token, outside
  both bars. Past the window, Metal holds the oracle run with a window one key wider eight times further off; a discrete home holds the host rows read
  back equal to the device's bit for bit, a poisoned key past the window moving nothing, a poisoned first window key moving
  the draft, and a prompt read on the CPU rails under the prefill pin counting no device prefill - the re-upload counter's
  control;
- `mtp-gdraft-sched` (served) - a gemma-4 target cannot speculate until the facade attaches the sidecar beside it; then a
  scheduler told to speculate keeps `mtp` on and walks the counting prompt token for token with the plain scheduler,
  drafting and accepting as it goes, both streams decoded in the log; the control is the same walk with the drafter detached,
  where a scheduler told to speculate stands down (`mtp` off) and drafts nothing;
- `mtp-gdraft-vdecl` (served) - a tick of two streams on two K/V codecs, whose joint verify the batch rail declines: each
  stream's mirror asserted standing, so the round finds both warm and their draft chains run; no stream carried, no round
  counted, each stream one plain step whose logits are a plain decode step's bit for bit, both argmax pieces logged; the
  control is the same tick on one codec carrying both streams;
- `mtp-gdraft-ring` (served) - the drafter past a sliding capture layer's ring: the 26B loaded under the ring pin on and off
  (`with_gpu_kv_ring`), a counting prompt past the ring prefilled in 512-row calls (a Metal ring grows to the widest window),
  one one-row step and a three-step draft chain at the anchor, then a depth-3 speculating stream; the one-row chain's logits
  and the drafter's first step's logits at the anchor bit for bit across the pins on Vulkan (on Metal, whose AV pass splits a
  ringed key run at the wrap, the trunk's pick alike), the drafts and the streams token for token, the ringed run's accepted
  drafts within a tenth of the flat run's;
- `mtp-gdraft-depth` (served) - the counting walk at depths 3 and 1 token for token the plain scheduler's, every round
  drafting the knob's depth across a 64-row page but the last round the 32-token budget clips (depth 1's one a round the
  control), then four reasoning budgets at depth 3
  whose forced close lands at the budget on the speculating stream;
- `mtp-gdraft-joint` (served) - three streams on three regions at depths 1 and 2, a tick's verify carrying all three, the
  joint verifies counted; the control is one stream at depth 1 counting none;
- `mtp-gdraft-codec` (served) - the depth-2 walk on q8_0 and tq4 sessions, the rounds on the device;
- `mtp-gdraft-iq3` (served) - the UD-IQ3_XXS trunk, whose Q6_K token table the seed embed gathers, at depth 2.

Every served walk logs both streams decoded. On the Metal home every served arm loads its target through
`load_model_cached`, so the file mints a `.dlim` for it - ledgered at `../ARCHITECTURE_IMAGE_LEDGER.md#suite-image-mints`.

Prefill parity: `lastrow-g` (its own test, `test_metal_prefill_lastrow_sandwich`, families `gemma3` / `gemma4` / `gemma4e`:
gemma-3-1b, gemma-4-E2B Q8_0 and, large tier, gemma-4-12B Q8_0 - the last layer's FFN tail narrowed to the final row on the gemma
sandwich, each branch under its post norm, the E-series' per-layer embedding branch over the final row too, against the full panel's tail over 48 counting ids: the logits within the cell's bar, the full tail over the window with its last id changed outside it (the
control), the same argmax, both picks decoded, the narrowed
residual's row 0 off the full one's and `keep_hidden` restoring the full plane bit for bit - `lastrow_ab`, the mm-tail arm's
SmolLM2 legs' form), `base mm-tail s16
kq cont attn-dev mirror mirror-codec mirror-cap segs span span-fused span-mrope span-ds dim qkv moe-gemv moe-base kq-tail kq-scratch bf16-split` (moe-base = the routed
block's two GEMM forms on split-format expert carriers (`test_metal_prefill_moe_base_form`, family `qwen3moe`, large
tier: Qwen3-30B-A3B UD-Q3_K_XL and UD-IQ2_XXS, loaded under the Metal mode so IQ3_S planes read their grid form): a 280-row
then a 20-row window on the tensor twins over the gather panel, then with the panel off - the gate and up sites on the
simdgroup base form - each leg's last-row logits within the carrier's bar of the CPU chain's largest, the base kernel's
census count rising on the base leg alone, the control a window whose last token differs landing outside the bar; segs = the segmented
window: two sessions' chunks as ONE window against the same chunks as two windows on one blob twin - Llama-3.2-1B at
Q8_0, Q4_K_M and IQ4_XS, the three formats the window's GEMM ladder serves apart - on the mirror-fed form and the
panel form: the continuations' last-row logits within the mirror arm's bar, eight greedy steps off each session token
for token and every one of them a device decode (`metal_decode_stats().decodes`), then both openings as one window
from position zero the same way; every segmented window counted by `metal_prefill_segment_windows` and as one device
prefill by `metal_prefill_stats().prefills` (`segments_window` in `_metal_blob_twin.das`, the two-window reference
`two_window_greedy` in `_model_tier.das`); the deltanet form's cell is `test_metal_segments_deltanet` in
`test_metal_batched_row.das`, two Qwen3.5-0.8B sessions' tails as one window and 24 greedy steps each token for
token their single windows' on the same witnesses; kq-tail = the dense
K-quant and per-32 sites' row-batched GEMV peel on a window of eight rows or fewer (`test_metal_prefill_kq_tail`, Llama-3.2-1B
Q4_K_M, IQ4_XS and the Q5_1 mint, Qwen3-4B Q4_K_M, Qwen2.5-0.5B's IQ4_NL mint, the same two-window shape as moe-gemv at 2, 4, 5 and 8 rows - past four rows the split-K tile takes the sites under its knee, its site counter the witness, and at 5 and 8 rows the 16-row
split stamps (`THSk16` / `TSk16`, their census count) take them, the same windows with those stamps held
(`set_metal_prefill_held("split_k16", true)`) dispatching none and the 32-row split stamps serving - and at 34 rows with split-K pinned off, the two-row remainder on the row forms at its x offset; the peel's site
counter the engage witness and `set_metal_prefill_mm_tail(false)` the off-lever); kq-scratch = the dense sites' per-window
scratch panels (`test_metal_prefill_kq_scratch`, Llama-3.2-1B IQ4_XS, Q4_K_M and Q8_0 on a blob twin that carries no stored
panel): a 200-row and a 400-row window with the crowns pinned empty, then pinned to the format's twin crown - the short window
on the twin, the long one on panels past a K-quant twin's floor and on the twin under a Q8_0 one's - each window's last-row
logits within 0.08 of the CPU chain's largest, `metal_prefill_devw_scratch_stats` the engage witness, the same window with
`set_metal_prefill_dev_w(false)` inside the bar with no panel counted, the control a window whose last token differs
landing outside the bar; on the base forms the 200-row window also runs under the dev-W knees (`scratch_knee_cells`): a grid
floor no tile reaches (`set_metal_devw_min_tgs`) leaving every site on its quantized tiles, no panel counted, then the half
tile's knee at 4096 (`set_metal_split_k_tgs_hmm`) - the same window's checks, the half GEMM's split stamp
(`MetalHalfMulMmTHSk`) counted by the census, and split-K pinned off the control: panels taken, no split stamp; bf16-split =
the per-layer embedding's bf16 projection on a short window (`test_metal_prefill_bf16_split`, family `gemma4e`, gemma-4-E2B
Q8_0's blob twin, skipping where the box's profile crowns no bf16 tensor tile): a 24-row window under the q8 / bf16 knee at
1024 rides the bf16 split stamps (`MetalBf16MulMmTHSk` / `TSk`, their census count), split-K pinned off the control
dispatching none; moe-gemv = the short
window's gathered expert route (`test_metal_prefill_moe_gemv_route`, families `qwen2moe` / `qwen3moe` /
`gptoss` - the q8, K-quant and mx4 expert planes, every carrier large-tier; the hybrid holds its parity through
the MTP parity file's served twin and `test_metal_format_census.das`'s blob twin of the pure IQ4_XS file): a 300-row window on the tiles then a 2-, 7-, 16- and 24-row window the
route serves - the 16-row one at the shipped knee, the others at a knee pinned to 24 - the last row's logits
within the carrier's bar of the CPU chain's largest, the route's window counter up by one and the prefill
counter by two on the route and on the tiles leg, the same windows with the knee at 0 (the tiles) logged
beside them and the counter unmoved, the control a window whose last token differs landing outside the bar,
the arms' argmax pieces decoded in the log; a window the device declines skips loudly;
mirror-codec = a q8_0, tq4 and f32 session's two windows (300 and 20 rows, Llama-3.2-1B Q8) feeding the mirror through
the codec's store stamp - both windows counted on `metal_prefill_mirror_panel_served`, the q8_0 / tq4 store stamps counted,
the twelve decode steps after them uploading no row (`metal_mirror_uploaded_rows`) and every one a device decode, greedy
token for token the CPU chain on the same codec; the mirror knob off the control (no window fed, every prompt row uploaded
by the first step, the same tokens); mirror-cap = a slice past the mirror cap serving: under a 64 MiB cap an f32 session of
1100-odd rows (a 128 MiB slice) prefills and steps on the device token for token the CPU chain, its slice resident past
the cap, then at the default cap Llama-3.2-3B Q8_0 at f16 over a counting prompt of 43000-odd rows (its K/V past
4096 MiB, asserted) in 1024-row calls under the `attn_dev` crown - every call a device prefill, six device steps, the
mirror past 4096 MiB and the continuation counting on;
mirror = the mirror-fed window
against the f32 panels on one blob twin under the pair's pinned crown - a 300-row window and a
20-row continuation, the last row's logits within 0.4% of the largest and the census naming each
form's kernels; the decode step after them uploads no row on the mirror-fed form and every row on
the panels form; a paged taker adopts a released donor's mirror for the whole pages a prefix cache
hands it and uploads the copied partial page's rows alone, to the bit of the same run with adoption off, and a reused group id under a
new stamp adopts nothing; attn-dev = the device attention pair
against the staged trio on one blob twin under the pair's pinned crown - a 300-row window on the
32-row stamps, then a 384-row continuation on the 128-row ones, the last row's logits within
0.4% of the largest, the continuation started one token later as the control, the same next
token on both forms, the census counting each stamp once a layer and the softmax pass twice a
layer; mm-tail = the GEMV-tail residue peel -
four fixtures, npos % 32 == 1 (the lone row rides the reduction-split GEMV), == 2 (the b4
form at 2 rows), == 5 (two b4 dispatches, 4 rows + 1) and npos == 4 (the pure-tail leg,
no mul_mm at all - the split-K tile takes a window of 5 to 8) - token parity vs the all-CPU control, the tail-off A/B
twin, and the peeled sites/rows counters as the anti-vacuous witness; plus the SmolLM2 alignment-guard
leg on the whole-K tile (split-K pinned off) - the guard is per mul_mm site on that site's K width (b4 needs % 128,
the gemv % 32), so at npos % 32 == 2 the 576-wide sites must refuse while exactly the hidden-kdim w2 site
(1536) peels per layer, and == 1 rides the gemv everywhere - then the same pair under the split-K knee, where the
one-row remainder peels at every site and the two-row one at none; counters only (greedy token
equality is not a valid instrument on a 135M); span = the non-causal media eval shape,
head + embd span, per codec; span-fused = the image turn's ONE-eval shape - causal head + media rows + causal
tail through the per-query mask, one GPU prefill, with a same-backend fused-vs-splice logits
witness on poison-calibrated per-codec bars (a poison is a value added to the expected result
that must red the bar); span-mrope = the qwen grid-roped turn - one GPU
prefill via the per-row-table capability, token-exact vs the all-CPU control, plus a
same-backend grid-vs-sequential prefill-logits witness; span-ds = the deepstack turn - one
GPU prefill via the slice-add capability, token parity plus the add-CONTRIBUTION witness:
per backend, the prefill logits with the tails minus the same logits with the tails zeroed;
the GPU's contribution must clear a 0.01 floor and its max difference from the CPU's must
stay under half the GPU's own contribution - greedy tokens tie on the counting fixture, so
a skipped add hides from them).

Support matrix: `cells-q8 window cells-s16 mode kq tensor dim8b dim70b` + the
family matrix `fam-qwen3 fam-qwen2 fam-phi3 fam-gemma2 fam-gemma3 fam-gemma4 fam-qwen3moe
fam-gemma4moe fam-gptoss fam-gemma4e fam-qwen35 fam-qwen35moe fam-qwen2moe fam-qwen4exp` (needs-derivation pins +
per-path cells; fam-gemma2 also carries the sliding-window masking parity row, whose census holds
the capped rows past the single-dispatch ceiling on the fused attention's wide stamp and off the chunked pair;
fam-gemma4e's rows carry the per-layer-FFN-width and blob-kq-PLE-gather coverage, the residual step's seam census under the
fuse rail pinned on (`set_metal_decode_fuse`, put back) - every encoded layer stack's seams one dispatch each: the sandwich
seams with a next norm on `metal_pre_add_rms`, the per-layer embedding's residual and the last layer's seam on
`metal_pre_add`, no plain seam - the window's v-norm on the fused q/k pass with no norm pass of its own, the pre-step's
device counters at every decode shape - the planar model under the same overrides finishing, projecting and dispatching none
of them, nor the site's format stamps (the counters' control) - and the greedy chain pinned on taking chained steps where
pinned off takes none, both walks the same tokens; both E2B rows assert parity through their forced-feed cells, not token
equality, because freeform continuations from a 2B sit on near-ties; every fam-gemma4e row ends on the depth cell
(`depth_cell`): three forced steps past a 120-number prompt within the row's bar of the CPU's, the census holding the
full-context layers on the head-of-512 fused stamp, the sliding ones on the head-of-256 stamp, and none on the chunked pair;
the same cell closes every ENGAGE row of a head-96 carrier (`fam-phi3`), its census the narrow fused stamp - the head off the
64 lattice admitted past the ceiling;
fam-gemma3 (gemma-3-1b) and fam-gemma4e (E2B) carry the Metal ring row (`metal_ring_row`): a counting prompt past the window
prefilled in 256-row windows and 24 greedy steps on the blob twin with the ring pinned on, then off, token for token, every
window and step on the device, the ringed layout holding rings that wrap under the prompt and the flat one none, and the
ringed mirror feeding every window the flat one feeds (`metal_prefill_mirror_served`);
`fam-gemma4e-release` (E2B Q8_0) is the release row (`metal_release_row`): three load-serve-delete cycles of the blob twin,
each closed at both drivers' shutdown, every cycle's four steps served on the device, the process footprint once the
device's deferred frees land (`metal_footprint_settled`) at the second and third close within 512 MB of the first and the
live-object count zero after -
a buffer the binding leaves alive adds the 5.3 GB blob a cycle;
fam-gemma4/fam-qwen3moe/fam-gemma4moe/fam-gptoss/fam-qwen35moe/fam-qwen2moe are
DASLLAMA_PARITY_FULL-gated - 7.4/18.5/26.9/12.1/22/15GB; fam-gemma4moe and fam-gptoss are ENGAGE
+ shallow logits TOLERANCE cells only - token parity is not a valid instrument for the 26B, whose double-router
CPU differs from any float implementation by ~2.5 logits/step by construction; fam-gemma4moe runs the 26B files of its
carrier list (Q8_0, Q4_K_M, Q4_0, UD-IQ3_XXS, UD-IQ4_XS and the MXFP4_MOE mint), each opening on the format census (every site the file carries in
its own format, the expert stacks the native-MXFP4 planes exactly on the mint) and asserting the routed GEMV stamps of the
file's expert formats dispatched by the forced step, none of them by the CPU truths on the planar model under the Metal
overrides (their control); its GPU legs are the MoE tail as one step (`MetalMoeCombG4`: counted once per layer for the
prefill window, and once per layer but the last for each encoded forced decode step - a whole multiple of `n_layers - 1`),
the tail on its unfused chain under
`set_metal_moe_g4_unfused` (no step counted, the control), and a q8_0-mirror leg against the CPU chain on q8_0 sessions (the
decode's v-norm arms in the q/k norm's dispatch), every leg's prefill and forced-step rows against the CPU truth one step off
outside their bars (the bars' control); fam-gemma4's 12B and 31B rows hold the forced-feed freeform decode within 8 over the
rows the row names (128 on the 12B, the 29 ahead of the 31B's one-token loop), every one of those GPU rows against the CPU row
one step back outside it (the control); the 31B row, past the 4096-float slab, adds the residual seam census
(`past_slab_seam_cell`): two device steps and a batched step with the fuse rail pinned on, every seam one dispatch on the
1024-lane builders (`metal_residual_lanes`) and none on the 256-lane ones, then pinned off (put back), every seam's split
chain norm passes over the fused run's to the pass and no one-dispatch seam (the control);
fam-qwen35's batch cell asserts the batched step ENGAGES on the deltanet hybrid's blob twin
and compares it against the single-row GPU step; fam-qwen2moe's batch cell compares the blob
twin's batched step against the CPU batched stack on the planar model - the two arms the
batched driver gained, deltanet rows against per-session mirrors and the shared expert's rows
site; fam-qwen35moe keeps the per-row FALLBACK assertion - the MoE hybrid has no batched arm;
both
qwen35 rows open on the format census of their blob twin, every site the file carries in its own format; fam-qwen4exp
is the Flash-Next UD-IQ4_XS served image alone - the planar file and the image do not fit the box together - its Metal
prefill and decode token for token the file's frozen counting fixture, the drivers' counters rising and no decline,
PARITY_FULL-gated). The E2B format carriers have their own rows: `fam-gemma4e-q40` (the Q4_0 file) and `fam-<carrier.tag>`
(built from the carrier list's tag: `fam-gemma4e-e2b-emb-q6k fam-gemma4e-e2b-emb-iq4xs fam-gemma4e-e2b-emb-q50
fam-gemma4e-e2b-emb-q51 fam-gemma4e-e2b-ple-q6k fam-gemma4e-e2b-ple-q50`; an emb carrier's row also asserts the PLE table's
format tag); `--arm fam-gemma4e` selects them all.

Metal format census (`test_metal_format_census.das`, entry below): `census-iq4xs census-tied
census-f16 census-iq3s4 census-iq3s4-moe census-nv4 census-nv4-table census-nv4-hybrid census-moe-nv4` (the nv4 mints:
the Llama-3.2-1B NVFP4 mint with every dense plane nv4, its table mint with the nv4 token table and tied classifier, the
Qwen3.5-0.8B-MTP `--pure` mint with the deltanet planes, the NextN eh_proj, the table and the classifier nv4, and the
Qwen3-30B-A3B `--pure` mint - large tier, family `qwen3moe` - with every routed stack nv4) and the per-32 mints `census-q51 census-iq4nl32
census-iq4nl32-table census-q4032 census-q5032 census-q51-table census-moe-q51 census-moe-q4032
census-moe-q5032`. Vulkan format census (`test_gpu_resident_format_census.das`, entry below):
`vk-census vk-gather vk-perop-ffn vk-per32-iq4nl32 vk-per32-q4032 vk-per32-q5032 vk-per32-q51`;
`vk-per32-moe` is `test_gpu_resident_moe.das`'s per-32 MoE carriers. A filter token matches by
substring, so `--arm census-q51` also runs `census-q51-table`, and `--arm vk-per32` runs every
per-32 arm of both Vulkan files.

The Vulkan gemma resident files (`test_gpu_resident_gemma*.das`, entry below), one arm a carrier (`GemmaFile.arm`): `g2`
(gemma-2-2b), `g3` (the gemma-3-1b cells), `g3b` (gemma-3-4b), `g4k` (the 12B Q4_K_M cells), `g4x` (the 12B Q4_K_M
300-token agreement cell), `g4q8` (the 12B Q8_0 cells), `g4e` (E2B), `g4f` (E4B), `g4m` (the 26B-A4B IQ3_XXS), `g4mk` (the
26B-A4B Q4_K_M), `mx26` (the 26B-A4B MXFP4_MOE mint of the Q4_K_M file), `host26` (the 26B-A4B IQ3_XXS with host experts),
`g31` (the 31B); and the E-series cells' `g4e-ple g4e-kq g4e-kq-q40 g4e-kq-ple-q6k
g4e-kq-ple-q50 g4e-kq-q8 g4e-pre g4e-chain g4e-f16 g4f-codec`; the CUDA gemma files (`test_gpu_resident_cuda_gemma.das`,
`test_gpu_resident_cuda_gemma4.das`) carry the same carriers under a `cuda-` prefix: `cuda-g2 cuda-g3 cuda-g3b cuda-g4k
cuda-g4m cuda-g4mk cuda-g4e cuda-g4f cuda-g4q8 cuda-g31 cuda-g4mx`. By substring `--arm g3` also runs `g3b`, `cuda-g3` and
`cuda-g3b`, `--arm g4m` `g4mk`, `cuda-g4m`, `cuda-g4mk` and `cuda-g4mx`, `--arm g4e` every `g4e-*` arm and `cuda-g4e`,
`--arm g4e-kq` every `g4e-kq-*` one, `--arm g4f` `g4f-codec` and `cuda-g4f`, and every Vulkan token its `cuda-` twin;
`--arm cuda-` selects the CUDA files' arms alone.

The `image` suite (test_model_image - the prepared-image .dlim rail): `mechanics` (synthetic
carrier, model-free - runs with no model stocked; also the layout fingerprint; the Model meta's two site-scale tables -
the per-tensor table and the nv4 stacks' per-expert vectors - read back through `serialize_image_meta`; the save's temp
file - a save the `set_image_write_fault` seam fails publishes nothing and leaves no temp file,
a temp file a killed writer left (its mtime set back) goes at the next save and one written a
moment ago stays; the lane naming -
the file name's lane segment read back, a hash-named legacy image adopted into its lane, a save
reaping only its own lane's legacy other while another lane's OTHER survives, a version-stale
image going in any lane, and the rope-scaling override as a lane suffix and an identity element
that leaves the tag parsing intact, the IQ3_S serve pin's `-lut` / `-grid` on the planar and blob lanes and the dev-W
panel lever's `-devw` on the blob lane alone; a tower's exact lane - the whisper, parakeet, gigaam, canary,
gemma4a, qwen3a and whisper-class tower f32 lanes, the Bergamot students' f32 lane (`bergamot-f32`), the gemma4v, gemma3v,
gemma4uv, qwen3v and qwen25v towers - keying one identity under every whole CPU backend, while the q8 lane names the backend; the per-format
plane table's slot claim - a `kq_sb` slot's planes ride the image under the slot's section names
while a non-sb slot rides no section, its planes being the top-level pair; the split-scale
plane's two arms against hand-built superblocks - k6 and k3 strip-first at `strip_at` 0 / `tail_at` 16,
iq4xs and k2 d-first at `strip_at` 4 / `tail_at` 0 with k2's four-byte tail, each carrying an
added-value poison in one tail byte that must move that byte and nothing in the strip, plus the
k6 sizing leg at 4,000,000 superblocks - and the dev-W bake tables, whose rebuild-not-append
contract, per-format key arithmetic, split-scale cover predicate - a 256-wide 64-row panel
bakes, a 384-wide one and a 32-row one do not - and expert exclusion - an `*_exps.weight`
slice bakes nothing, a shared-expert site and a mis-spelt suffix still bake - are pure taxonomy
over a job list, plus the split-scale transform's idempotence over one carrier) `smol untied metal metal-untied gemma tower whisper voxtral
parakeet gigaam qwen3a canary canary-dec gemma4a gemma4uv gemma4uv-metal gemma4v gemma3v gemma4e
mtower kitten bergamot mx4-stream b32-stream`; `bergamot` (family `bergamot`) is the translate area's arm - ru-en tiny's two lanes, each image baked by the `.bin`'s cold load and loaded back by its own path, three Russian lines translated as the in-memory mint translates them, every side through `nmt_translate` with the device seat pinned off and put back (one entry point, one lane), each side logged; `mx4-stream` (families `gemma4moe` / `gptoss`, large tier: gemma-4-26B-A4B's MXFP4_MOE mint and
gpt-oss-20b mxfp4) is the native-MXFP4 cold mint's streamed fill - a child (`_mx4_mint_root.das`) mints the planar image under
`/usr/bin/time -l` and its peak memory footprint sits under the mxq + mxs bytes the image holds (macOS; elsewhere the leg skips
loudly), then the mapped planes are the eager load's byte for byte; `b32-stream` (family `qwen2`, small tier: Qwen2.5-0.5B's
Q4_0, Q5_0 and IQ4_NL `-local` mints) is the per-32 streamed planar mint - every served plane of the mapped image (the q8 pair,
the fp32 rows, the lattice table, every top-level pair) the eager load's byte for byte, and the image's last-row logits on a
counting prompt the eager load's bit for bit; `kitten` is the TTS area's arm - the StyleTTS2 carrier (kitten-nano) through
`load_styletts2` on both lanes, and the image loaded by its own path with the family data
riding its meta; `gemma4e` is the E2B metal-blob
mint+map arm - the PLE go-live tripwire (`ple_check_table`, which panics when the per-layer
embedding table's plane is short) runs after the blob plane borrows, so a fresh mint and a warm
map must both clear it; `gemma4uv-metal` is the GPU tower
driver's parity/counter/knob gate for the gemma4uv embedder, Apple builds only (`--arm
gemma4uv` selects it too - arm filters match by substring); `mtower` is the whisper-class
tower-blocks gate, Apple builds only - whisper tiny + large-v3-turbo transcript-exact and
qwen3a f32-rail transcript equality, CPU vs GPU, with geometry-derived counter deltas, the
twin-W legs - `wblob`, the halfword copy of a tower's GEMM weights baked beside the f32 blob and
read only where a tensor crown - the pin that compiles a family's half-precision GEMM twins
(`set_metal_tensor_crowns`) - compiled the half GEMMs - on whisper f16 + qwen3a bf16, each
engaging by the route counter, plus whisper's own
twin-knob freeze and whisper's own wblob-ONLY poison that must CHANGE the GPU transcript
(both legs are whisper's alone; qwen3a carries neither; a twin-W route reads its GEMM weights
from `wblob` alone, so zeroing that buffer alone poisons it, while a route that also reads the
f32 plane, `fblob`, is poisoned only with both zeroed), the gemma4a Metal
Conformer cells, E2B and E4B (f32-lane transcript word lists equal CPU vs GPU - punctuation aside, a
comma riding a near-tie token - with the control of the GPU route fed the clip with its second half
silenced reading other words, + the soft tokens' rel-l2 +
counter deltas, the projector tail run on the device - `out_ready` set on the device leg and clear
on the CPU leg - and the two projector widths read off the file: the encoder's 1536 on both, the
embedder's 1536 on E2B and 2560 on E4B; the lane pin/reset discipline mirrors qwen3a's), the canary Metal FastConformer cell (the
same discipline over the rel-pos XL block loop, the mel and the subsample front served on the
device ahead of it - the conv counter rising - and the control a clip with half its windowed
frames silenced, which the per-feature normalization cannot absorb as it would a scaled clip;
decoder = the q8_0 serving artifact), the
parakeet Metal FastConformer cell (the same chain over parakeet's f32 blob, minted in memory;
transcript equality CPU vs GPU + the encoder rows' rel-l2 + counter deltas - the subsample front
counted on the device at every GPU encode - then the front lever off (`set_metal_parakeet_front`:
no front dispatch, the block seat still serving, the rows within the bar) and the two GEMM
routes under the `mulmm_q8` crown - the device twin, its engage counted, and the f32 tiles with
the twin knob off, the counter unmoved - each held to the same rel-l2 bar, then the two attention
forms under the `mulmm_q8,attn_dev` crown - every head at once, its engage counted
(`metal_tower_fc_dev_encodes`) and its transcript the CPU's, and the per-head loop with the lever
off (`set_metal_fc_attn_dev`), the counter unmoved, the two forms' distances from the CPU rail
asserted to differ; the crowns and the three levers are put back as the cell found them), plus
the whisper q8 lane on the tower - a q8 whisper encoder serves on the device off its q8 planes:
the transcript the CPU q8 chain's, one encode and n_layer blocks counted, no `quant_mode` decline,
the halfword twin's counter unmoved, and a q8 tower with one layer's planes zeroed reading another
text through the device chain (the control); the whisper serving default IS q8 unless
`set_asr_fp32` / `set_asr_tower_fp32` asks for f32 (whisper carries no lane policy). Canary,
parakeet and gemma4a do: un-pinned, their lane follows whether the Metal tower would serve, and
their q8 lane declines `quant_mode`. The required-mode panic rides the parakeet q8 cell: that
decline under `MetalMode.required` panics; the arm's DECODER half is the `test_whisper_metal_cross_kv`
cell in `test_model_image.das` - GPU cross-KV on the q8 serving default, transcript-exact
against the CPU chain with window/step counter deltas and the knob and quant_mode declines,
required-mode, step-floor and shutdown-re-arm contract; the voxtral arm re-saves a
5.4 GB image from cold every run by design (it IS the >2 GiB-plane IO coverage); the `metal`
arm mints/maps the blob-only metal flavor (SmolLM2) incl. the CPU-tripwire and a
teacher-forced logits-tolerance parity cell (greedy token equality is NOT a valid bar on a
135M - genuine near-ties flip on ~0.02 gaps under ~0.75 cross-backend noise); `metal-untied`
is its untied twin on TinyLlama-1.1B - the blob plane carrying the classifier AND the
embedding's linear q8 copy (`emb_q8` without `cls_q8`): the layout gate admits the copy, cold
mint and warm map keep the q8 table with no fp32 copy, GPU serve, the same teacher-forced bar
(`--arm metal` and `--arm untied` both select it - substring match). The ASR-family
arms (`parakeet` transcript-exact, `gigaam` transcript-exact over the GGUF's cold bake and the `.dlim` loaded back
through `load_asr_model`, `qwen3a`/`canary`/`gemma4a` element-exact planes - the gemma4a and qwen3a q8-against-f32
transcribe cells comparing word lists, a comma riding a near-tie token between the two rails -,
`canary-dec` = the opt-in fp32 Model rail, token-exact) re-save their images from cold each
run like the voxtral arm. The canary arm carries both lanes: the f32 element-exact cell and
the q8 cell (read-time transcode - qblob/qscales/compact-blob element-exact vs a staged read).

The `image-vulkan` suite (test_model_image_vulkan, arm `vulkan`; every cell skips without the dasVulkan
module) covers the OFFLINE vulkan bake: the runner arms DASLLAMA_GPU + a small VRAM budget so the
probed config carries a vulkan section, the DRY tier collects a role-stamped plan with no device calls (safe on
GPU-less boxes), the flavor image round-trips the plan verbatim, and a cold cached load under
the armed tier mints the vulkan lane ALONE. `test_vulkan_inline_bake`, on the same dry tier, holds
a cold cached load: it mints the vulkan lane and no planar file, the served image carries the plan
and the device twin, and with the trim off the lane keeps the planar families. The minted plan and
device bytes equal the eager dry bake's (the bake over the prepared RAM planes) entry for entry and
byte for byte, and one poisoned reference byte is the control that must miss. Then, with
`set_vulkan_trim` on, the trimmed lane is flagged trimmed, has a different bake tag
(`moe_gpu_bake_tag()`) from the untrimmed lane's, has no q8 planar plane, packs the embedding
table for the CPU embed (`embq`), and is the smaller file. The cell pins the resident route on and
the trim off for its loads, because the trimmed lane's admission reads the route lever; it puts
the route back through `set_gpu_resident_route` and the trim through `restore_vulkan_trim`. Its
live-tier counterpart is `test_vulkan_mint.das` (stocked suite, `-jit`, family `qwen35`). It
skips when interpreted, with the image rail off, without the model, without a live armed tier,
and when the whole-model driver declines the model on the first load. On Qwen3.5-0.8B-MTP the
cold load mints the vulkan lane and no planar lane; the trimmed lane, loaded cold, generates the
same `MINT_STEPS` greedy tokens as the untrimmed lane and reads the same CPU embed row bit for
bit; mapped back warm, it generates the same tokens again. Each generation's token count is
asserted, both streams are decoded in the log, and the whole-model driver is asserted on both
trimmed loads. `test_vulkan_mint_moe_streamed` (family `qwen2moe`, large tier) is the MoE lane: Qwen1.5-MoE Q4_K_M with
twenty routed layers asked onto the host and the prompt stream pinned on, the untrimmed lane's tokens against the trimmed
lane's cold and warm, the trimmed lane without its planar families and the smaller file, the untrimmed lane's host picks
reading the planar planes it kept under `set_rare_arena(0)` (`g_rare_dec_rows` unmoved over its generation) and, under
`set_rare_arena(1)` - `DASLLAMA_RARE_ARENA=2`'s in-process twin - the rail (the counter up, the rail's rows within the MoE bar
of the planar kernels'), the trimmed lane's reading the streamed layers' mirrors through the rare rail where the knob keeps it
on (the counter up over its generations, the assert naming the knob's value; a skip at `=0`), the pins and the stream pin
put back to what the cell found.
`test_vulkan_mint_kv_codecs` holds the lane to the K/V mirror's codecs: the trimmed
lane minted cold under a q8_0 mirror (`set_gpu_kv_dtype`, put back) maps under tq4 and under f16
with no re-mint (the file's size and time as the mint left them - the lane's identity does not
fold the codec), each load armed at the asked codec, and at each codec the lane's greedy tokens
are the staged load's at that codec token for token, both streams decoded in the log; a trimmed
lane panics on a call passed to the CPU rails, so generating is the served witness, and the
a block codec's stream differing from the f16 one is the control that the compare tells them
apart. On Qwen3.5-0.8B-Q4_K_M the loader keeps the K-quant planes in their file format
(`kq_repacked`), the cold load mints the vulkan lane alone with its plan, and the warm map
generates the same tokens and the same CPU embed row. `test_cuda_mint.das` (stocked suite, `-jit` only; the CUDA
home's lane, `DASLLAMA_GPU=1 DASLLAMA_CUDA=1`) runs the mint file's cells on that home, the lane cells under the f16 mirror
pinned (`with_gpu_kv_dtype`, put back): `test_cuda_mint_lanes` - a cold
CUDA-armed load of Qwen3.5-0.8B-MTP mints the `cuda` lane and no other (the bake tag's first word and the file's lane
segment both read the armed home's name, `cuda_lane_named` comparing `image_lane_name(moe_gpu_bake_tag())` and
`dlim_file_lane` against `moe_gpu_backend_name()`), the untrimmed lane keeps the planar families, the trimmed lane
(`set_vulkan_trim`) serves on the whole-model driver without them off the packed emb region as the smaller file, its
greedy tokens the untrimmed lane's token for token and its CPU embed row bit for bit, and the warm map serves the same
tokens again; `test_cuda_mint_kquant` - the Q4_K_M twin's lane mints off its native K-quant planes (`kq_repacked`) with its
plan and maps back warm to the same tokens and embed row; `test_cuda_mint_kv_codecs` - the trimmed lane minted under a
q8_0 mirror maps under tq4 and f16 with no re-mint (the file's size and time as the mint left them), each load armed at
the asked codec and serving the staged load's tokens at that codec; `test_cuda_mint_identity_cap` - the lane identity's
budget word (` b<mb>`) reads the card's total memory within a GiB unpinned and the pinned figure (4096) under
`set_gpu_tier_want_`'s `vram_mb`, skipping where `DASLLAMA_GPU_VRAM_MB` shadows the pin. Every cell skips when interpreted,
without `DASLLAMA_CUDA=1`, without the model, with the image rail off (`DASLLAMA_IMAGE=0`), without the armed home
(`cuda_home_asked`), and where the whole-model driver declines the model on the box. `test_vulkan_mint_trim_follows_the_driver`
holds the trim's admission to the driver's answer: the model the driver serves is admitted
(`resident_would_serve`, an empty `resident_decline_reason`) and its trimmed lane mints onto the
driver; then, under a foreign prefill override the cell registers and selects, a staged load leaves
the driver off (the control), the driver's decline text (`gpu_resident_decline_`) is the chain's
byte for byte, the trim declines the model as this run's decline (`resident_trim_decline_is_run_mode`),
the warm load serves planar from memory and leaves the trimmed lane on disk at its minted size, a
cold trim ask writes the untrimmed lane at the lane's path (the next load under the override
serves it, the driver off); with the override put back to what `active_prefill_override` read
before, the next load mints the trimmed lane over it and the driver serves it.
`test_vulkan_mint_failed_write_leaves_nothing` fails the mint's write through the
`set_image_write_fault` seam (the seam's hit count moves): no lane, no temp file of the mint's own,
the load serves the streamed build from memory with neither lane's file on disk, and with the seam
at 0 the next load mints the lane and the driver serves it. `test_vulkan_mint_trim_refused_unarmed` mints the trimmed
lane, then loads it under the `set_resident_upload_fault` seam - the whole-model upload declining after the trim admitted
the model: the load is refused, its text naming the lane, the seam's decline and the trim; the lane stays on disk at its
minted size, and the same load with the seam off serves on the driver (the control).

The `coverage` suite (test_kernel_coverage, arm `coverage`; arm `coverage-vk` = the vulkan
SERVING census - needs a vulkan device + `DASLLAMA_GPU=1` + `DASLLAMA_MODELS_DIR`, MoE rows
under `DASLLAMA_PARITY_FULL=1` - the Qwen1.5-MoE Q8_0 and its `-local` Q4_K_M mint, the Qwen3-30B
Q4_K_M and UD-IQ2_XXS: the resident MoE block's s stamps (the 32-row column) and e stamps (the
128-row column) on the expert planes those files carry; an e stamp no stocked carrier reaches is a
`VK_CENSUS_NEVER_DISPATCHED` entry in `test_kernel_coverage.das` naming its kernel-unit cell; the
TTS classes are dispatched by `cov_tower_styletts2` and `cov_tower_pocket` on their carriers, their
kernel cells the `test_vulkan_tts_*` files, and a box without the carriers reads them at zero - a
warning, never an entry; the Bergamot translation driver's five classes (`nmt_*_cls`) are `VK_CENSUS_NEVER_DISPATCHED`
entries - the serving census loads no translation model, and its Bergamot row (`cov_tower_bergamot`, ru-en tiny minted in
memory) serves on the Metal seat alone - their kernel cells `test_vkt_nmt` and `test_vkt_nmt_attn`;
the K/V mirror's block codecs ride two carriers under q8_0 and tq4 (`vk_cov_codecs`: Qwen2.5-0.5B
Q8_0 for the four-head slab, gemma-2-2b Q8_0 for the two-head slab) - a prefill and two decode
steps each, reaching the staged stores, the shadows, the codec attention stamps and tq4's
un-rotation; the sink stamps over a codec mirror are `VK_CENSUS_NEVER_DISPATCHED` entries;
every prefill tile family is reached through the qwen3 Q8_0 and Q4_K_M and the 1B llama
requants, machine-local like the other fixtures - the `-local` ones are
minted from the bartowski Q8_0 with `llama-quantize --allow-requantize [--imatrix llama32_1b.imatrix] <q8> <out> <type>` (the IQ2/IQ3 types need the imatrix; the exact recipes are the catalog rows in `performance/model_specs.das`) - each
swept under the coopmat modes its planes have twins in: all five for q8 and q40, the box's mode,
mm and sdot4 for the other kq formats) is the KERNEL COVERAGE census (the census-row obligation is
`../REVIEW_GPU_KERNEL_CLASSES.md`'s): the small-model zoo swept across format/graph/batch/KV axes, then a
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
per-class CPU-oracle units covering the FULL metal kernel census, ~2-3 min) takes one arm
token, `kernels` (`--suite kernels --arm kernels`), which runs every cell. A gate dispatches
through the builder `[metal_dispatch]` generates for its class: `stamp_of(@@<builder>)` gives the
pipeline compiled from that class's kernel source, a seat into the pipeline global the builder
reads, and the builder, so a bind or grid change to the class reaches the gate by construction.
A class whose pipeline global is private to `dasllama_metal_prefill` compiles through
`kernel_of(@@<builder>)` (the pipeline, tgmem, and the builder's `_pso` form, which takes the
pipeline as its second argument) and dispatches through that form. Only a gate that dispatches a
grid or bind the builder cannot express binds by number, the reason at the site. The hand-bound-gate sync
obligation is `REVIEW_KERNEL_CELLS.md`'s. The misc file's vision stem gates run the templates both
homes stamp against `_kernel_oracles.das` and the engine's own walks: `pos_add_gate` (the 2-axis
position add on a non-square grid, bit-exact in the CPU's order), `q3v_stem_gate` (the merge-walk
gather over `q3v_reorder_src` at a width off the 64-thread group) and `patch_im2col_gate` (the
patch columns under the pixel map, the padded stride). The misc file also
carries `test_lens_tgmem_gate` - not a CPU-oracle unit: it spawns two `daslang -compile-only`
child builds (up to 120 s each) proving the lens refuses a `[metal_dispatch]` class with
`@workgroup` members and no `tgmem=`, twin fixture as the must-compile control; its siblings
`test_lens_requires_gate`, `test_lens_params_gate` (a `params=` name no `grid=`, `tg=`,
`requires=` or `@span` reads is refused; one only a `requires=` item reads compiles),
`test_lens_stamp_gate` and `test_lens_call_macro_gates` spawn the same way. The attn file's fused
single-pass cells (`sq_d_gate`) run the part and combine stamps against the CPU attention row at
heads of 128, 64 and 96 (three fixture rows read as one of four heads of 96), on the WIDE stamps at a head of 256 (one head over rows twice as wide, the
combine reading two heads of 128) and 512 (four times as wide, four heads of 128), each on the
float and the block-codec (q8_0, tq4) stamps, with the route table bound past foreign rows whose key
count is 1 - the offset a kernel reading the table from its start would miss the bar on - and
under a sliding window (inside one slice, cutting a slice, wider than every stream), a sink
logit that differs head to head and the logit cap, on the float, quantized and wide stamps. The
gemm file's device pair arms (`attn_qk_mm_gate` with `pair_mt`, `attn_dev_pair_gate`) run both
tile heights at a head of 96 too - QK's reduction width is the head, AV's 64-wide tiles plus the
32-column tail stamp - and the misc file's `embed_kq_gate` the token-table gather of every K-quant format the Metal tier serves (three rows off the engine's own plane-row dequant, bit for bit), `embed_b32_gate` the per-32 gather (q51, iq4nl32, q4032, q5032; three rows against `dequant_b32_plane_block`, bit for bit, at 96 wide and at 2048 wide past a whole other table's blocks - an untied table's offset - and at 96 and 288 wide off the 16-byte grid, bound on the grid below as `enc_embed_table` binds it, the blocks past the bind asserted and both binds asserted on 16 bytes) and `embed_f32_gate` the greedy chain's fp32-table gather (the
row gather at one row under the embed scale) at a 96-wide row and past a lead of foreign floats. The gemv file's
small-batch cells run every superblock format's eight-column form at 9 and 16 rows too - two column groups in one
dispatch over an x of the live rows alone - and every kernel cell of the iq4 family carries the iq3s4 stamps (format 35:
the GEMV, the small-batch forms, the routed GEMV, the base and tensor tiles, the routed tiles and twins, the token
gather). The prefill file's per-32 cells run the small-batch stamps (`b32_mvb_gate`: q51, iq4nl32, q4032 and q5032 at two, four and eight
columns, pad columns and two column groups, y rows past `d`, on the two-row stamps and on the K-split narrow-plane ones, a
64-block row among them) bit for bit against the same form's one-row GEMV a row at a time over x off
the dyadic lattice, the GEMV within a double dot's envelope, and the routed GEMV (`moe_b32_rows_gate`: q51, iq4nl32, q4032, q5032 and mx4's
E8M0 path) over several streams in one dispatch bit for bit against one stream a dispatch; the E-series pre-step's cells
(the bf16 and f32 rows' cells one gate, `router_gemv_gate`, over the two weight packings; `ple_proj_bf16_gate`: the bf16 rows through `enc_ple_proj_bf16_rows` - the word-pair stamps at an even dim and base, eight
columns a simdgroup and the decode column's threadgroup a row at 1536 wide, and the halfword stamps at an odd dim or base,
1537 and 1536 wide on the decode column past whole strides - past a lead of NaN halves with a NaN word after the rows, exact
against the dot in double, the poisoned lead element the control; `f32_rows_gate`: the f32 rows through `enc_f32_rows_gemv` -
the float4 router stamps on the lattice, with and without the bias row, the element-walk stamps off it (dim 66, base 2, dim 515 on an odd base, the one-column
walk past whole strides) - NaN past both row ends, exact against the dot in double, the poisoned lead element the control; `geglu_side_gate`, `ple_finish_gate` - the finish's `XSCALE` stamp (`GkPleFinish`), the finish without the
prescale its control); the side rows' gather rides the token-table family, the misc file's embed gates. The misc file's `cls_select_gate` runs the selection kernel on the template both homes stamp (`GkClsSelect`) over the Vulkan cell's
six rows at a 262144 and a 1000 vocab against `_sel_oracle.das`'s reading. The prefill file's `qk_rope_gate`
carries the window's V arms (V off the pre-norm k rows, the v-norm in place, the V panel bit for bit untouched with neither), and
the misc file's `add_rms_gate` every residual stamp (`GkAddRmsRows`: the plain step, the sandwich seam, the seam with no norm
stored, the f16 twins) through its one-row, rows and window builders and the decode rows' 1024-lane builders, at widths within the
4096-float slab and past it (5376, 8192: the columns past the slab re-read), the sandwich rows held off the plain step's as the
control. The gemm file's `router_f32_gate` holds the prefill router's tile (`MetalF32ExactMm`) at the 35B-A3B shape and at
64 experts against the dot in double on operands off the f16 lattice, the half-staged stamp over the same rows missing
the bar as its control. The prefill file's `dn_groups_gate` carries one checkpoint capture on its second group after that
group's second row: the capture arena holds the state and the history the oracle reaches over those two rows alone, and the
state after all three rows missing the captured state is the control. The
cells that need no kernel: the misc file's `test_metal_served` (a driver's forward answer through `metal_served`: a
decline passes through and leaves the pool's spin window, a served one opens it, a later decline
keeps it), `test_metal_moe_kq_twin_pick` (the K-quant MoE twins' pick: a crowned family at every
shape, an uncrowned box at or under the knee's mean rows an expert and never past it, the knob
answering what it replaced), `test_metal_region_cat_keys` (the concatenated weight regions key on
every piece: two concats over one first piece and two different second pieces each hold their own
bytes, a repeat of either is its cached buffer, two cached regions counted) and
`test_metal_ring_slack_reload` (a model swap returns the mirror rings' slack to its floor and stales
the steps encoded over the old layout; a swap with the slack at its floor moves no epoch), and the prefill file's `test_moe_padded_bound` (the bucket panel's
row bound against the rail's own padded prefix over seeded routings, reached exactly where every
routed expert is distinct, a bound for one row fewer sitting under the rail's pad as the control), `test_pf_bf16_tile_pick`
(the window's pre-step projection takes the bf16 tile at a dim of whole 32-element chunks and layers x ple of whole 64-row
tiles, the position-blocked GEMV off either)
and `test_resident_panel_charge` (the resident panel registry on the
driver's device: an uncharged ask makes its panel and charges nothing, the same key answers again,
another size under it is refused, a charged ask counts its bytes exactly when the budget serves it,
and a shutdown drops the registry). The StyleTTS2
chain's kernel cells (the prefill file): the gathering conv GEMM on a forward conv (40
channels - the k-run tail past the taps, dilation 3 with pads off both ends, 45 rows padded to
64) and a stride-6 transposed one, each direction's own stamp, on both precisions - the f16-staged one on f16-exact operands (every
product exact, so the 1e-3 bar covers the accumulation order alone) and the f32-exact one on
operands off the f16 lattice at 2e-6, where the f16-staged stamp on the same operands must miss
the bar (the stamp's control); the im2col route at 22 channels on a whole chunk and an offset
one; the stats + fused AdaIN pair, Snake and leaky, at one stride with a tail and at two strides
plus a tail; the elementwise rows on every grid tail, the residual join's scale included; the row
gather with its adds off and on; the concat rows with and without the residual columns; the
depthwise transposed pool; both LSTM directions on one gate set against a CPU recurrence (the
fixture centred off tanh's saturation, asserted); the ALBERT attention rows against the CPU
`attention_rows` at 37 and 100 rows; the sigmoid sums; the tanh GELU past its clamp; the harmonic
source trio on either resample law's stamps against the CPU source chain itself - the phase frames
within one ulp of its carry, the mixed signal within 2e-6 - a 40000-frame cumsum against a double
accumulator where the two laws part by whole cycles, and the driver's own noise draw through the
fill kernel (finite, repeatable per seed, moving with it); the STFT on either pad law's stamp and
the inverse STFT with and without the window envelope. Each carries a poisoned input. The census row is one synthesis each
on kitten-nano (the ONNX law's stamps) and kokoro-82m (the torch law's) on the file's planes (`cov_tower_styletts2`). The parakeet
front's cells (the same file): the first conv as the chain the fronts run - the shared im2col at one
channel, the f16-staged GEMM over the [ch][32] tap panel, the bias-and-ReLU rows - on an 11 x 10 image - odd along one axis, even along
the other, so each edge drops its own taps - against the in-test loop, the sums asserted to take
both signs so the ReLU shows; the depthwise conv on the shared template's two Metal stamps
(parakeet's tap-major taps, canary's channel-major ones) against the same loop, then under a length
mask that must move the rows past it; the feature permute (the shared template) bit for bit with
its row pad zero under a sentinel fill; the bias-and-ReLU row pass against max(x + b, 0) over sums
of both signs. The shared templates' other Metal stamps (the same file): the conv module's GLU
against the CPU form with the halves swapped as the control, the subsample LayerNorm + ReLU off
padded rows against a double form with a scaled weight row as the control, gemma4a's subsample
depthwise conv against the CPU loop, gigaam's biased centered depthwise conv (`ga_dw_gate`: five tap-major taps
over 13 rows of 32 against the CPU loop, a poisoned bias reddening it; `ga_dwh_gate`: the same conv over the carrier's f16
taps, `MetalGaDwH`, at 13 rows of 64 and 5 taps and 40 rows of 96 and 31 taps against the CPU loop over the same halfwords,
`fc_dw_bias_rows_f16` - itself held bit for bit to the f32 loop over the widened taps - a poisoned halfword tap reddening it;
the fixture `_kernel_oracles.das`'s `dw_f16_fix`, the Vulkan cell `test_vkt_tower_ga_dwh` the same), the silero VAD's two stamps (`vad_enc_gate`: three
chunks' input gates against `_vad_oracle.das`'s encoder, a poisoned conv tap reddening it; `vad_lstm_gate` at one chunk and
five: the probabilities and the state left in place, a poisoned recurrence weight reddening it; the slab behind a NaN lead,
the Vulkan cell `test_vkt_tower_vad` the same references), gigaam's device mel (`ga_frames_gate`: the frame
window bit for bit the CPU frame loop's product with its row and column pads zero, a poisoned sample reddening it;
`ga_mel_gate`: the clamped-log filterbank against the float sum's log with a frame at the floor and one past the ceiling, a
poisoned filter reddening it), qwen3a's conv-out shuffle bit for bit, and the FastConformers'
rel-position table against the CPU `rel_pos_table` with its zero pad rows and the table of one
frame more as the control. The whisper-class projector tail's
two row kernels (`tw_tail_rows_gate`): the pair pool and the row-split gate at a 70-wide row over
five rows against their host forms, the gate's fixture asserted to tell its halves apart; the vision
tails' row kernels - the grid mean pool (`tw_pool2d_gate`: a 6 x 4 row grid pooled 2 x 2 against the
host mean, a one-axis pool told apart), the standardize (`tw_affine_gate`: in place against
(x * scale - b) * m on the standardize arm and x * scale on the scale-only arm, two rows past the run
kept, the form with no scale told apart), the NEOX table rope over a fused row's k slot
(`rope_tab_gate` over the `RopeTabStamp` enum: the tower's NEOX stamp from position 0 and the pocket
prompt's interleaved stamp from a position base, each against `rope_tab_ref`, the q and v slots
untouched, a poisoned k element) and the gated hidden (`tw_bias_gate_gate`: silu(g + bg) . (u + bu) in
place over g against a double form, the bias rows at offsets, at 210 and at 209 elements - the last
lane's guard). Every seam kind runs at 512 too - two whole trips of the 256-thread stride; the tower
rms stamp is the misc file's `rms_gate` third arm (`RmsStamp.tw`, at 300, 70 and 512, in place too). Canary's mel
normalization (`cn_melnorm`): the log and the per-feature normalization in place against a double
form at 70 features (off the 64-thread group), 11 frames of which 7 are valid - the rest zero - and
two rows past the mel left as they were. The FastConformer
all-heads attention's two stamps (the same file): the half operands (`fc_twin`) bit for bit the RNE
narrow of the host's sum or product over all five panels, their pad rows zero, one poisoned input
a panel; the rel-shift softmax (`fc_pexp`) over two heads at 24 and 300 keys - the weights within
one half step of the oracle's, the stat within 1e-5 of a double sum, pad rows and pad columns
left as they were, a poisoned rel score reddening both. The Pocket chain's cells (the same
file): the row copies with and without the ELU, the layer scale, the rows rope over a row stride
from a column with the tables bound at a position's row, the prompt's rope from the voice tables at a
position base (`pk_rope_tab_gate`: the k span of 23 rows from position 37 against `rope_rows`, the q
and v spans untouched, a poisoned k element reddening the compare), the attention
row against `attention_causal_rows` over a `TtsKvCache` (every key and an 8-key window, an
unseen key's poison staying silent), the rope-and-store kernel's f32 stamp at the frame loop's
binds (no bias, the whole head, the tables and the caches at the position's row) against
`rope_rows` with the caches' sentinel rows, the add-and-norm at a width under the threadgroup and one past it, and the row
GEMV's nine stamps (f32 rows under the bare dot, f32 rows and a q8 blob under the normed and
modulated x with the SiLU out, the gated residual, the SiLU over the slab vector and the frame tail;
the bare q8 dot rides the decode GEMV) against
an fp64 oracle with x, y and the latent row bound at offsets. The census row is one Pocket
codec pass over synthetic latents and one spoken line on the f16 file, then one spoken line on
the kq file so the q8 GEMV stamps count (`cov_tower_pocket`). Shared fixtures - the `MetalCell`
record (the Vulkan `CellPlanes` verbs: `in_` / `in_plain` / `out_` / `out_words` / `io_`, `plane_run` /
`plane_as` / `plane_upload` / `plane_refill`, `cell_record`, `with_metal_cell`), `queue_record`
(`cell_record` over a bare queue), `check_buf` (a
readback and `_compares.check` in one), the device shell `with_metal_gate`, `stamp_of` /
`kernel_of` / `keep_stamp` / `stamp_run` (a builder's pipeline, kept in the cell and seated for
one dispatch, or handed to the builder's `_pso` form), `pso_stamp` / `stamp_tgmem` (a pipeline from ONE kernel stem), `kq_split_doff`
(a split-scale format's d-tail offset), `tensor_pso_or_skip`, `env_bar` /
`lead_poisoned` (the GEMV and GEMM cells' bars with their controls), `check_hx_narrow`, the buffer
helpers of `metal/das_metal_boost`, re-exported, the
kq plane and q8 blob builders, the 64-lane dispatch and the fp64 scalars - live in
`_metal_kernel_common.das`; the StyleTTS2 cells' f16-lattice fills (`f16_at`, `f16_fill`), the source
fixture and CPU chain, the long cumsum's laws and the STFT, inverse STFT and concat forms live in the
backend-free `_tts_kernel_oracles.das`, which the Vulkan TTS cells read too. `_mtl_toy.das`
is the `[metal_dispatch]` multi-kernel (kernel=) fixture; its gate in the misc file
dispatches through the GENERATED builders (kn_ rail), not hand binds.

The control obligation for a new gate is `REVIEW_KERNEL_CELLS.md`'s, for a new or loosened bar
`REVIEW.md`'s. Size a poison as a value ADDED
to the expected result, not as a multiple of the tolerance. A tolerance that scales with the
accumulation length swallows a scaled poison at the kernel's longest dot product, which is
exactly where a bug hides. Every compare against a derived truth gets its own poison. A
dispatch that skips `metal_set_threadgroup_memory_length` for a kernel with `@workgroup` state
reads garbage silently - no error, a plausible wrong number - which is why the lens makes the
omission a compile refusal (`test_lens_tgmem_gate` above).

## The per-PR runs - model-free whole, the stocked files by reach

The split between the `model-free` and `stocked` suites: a file that reaches a machine-local
fixture root (`models_dir()`, `model_available()`, `llama2c_dir()`, `whisper_dir()`, the ASR corpus's `corpus_dir()` /
`asr_clip()`, `asr_oracle_dir()`) is `stocked`,
every other suite-less file is `model-free`, and `test_run_suites.das` reads the files and fails a
misfiled one (the pinned gate `REVIEW.md` names for the split). A `stocked` file declares `options
persistent_heap`, and the same gate fails one that does not: dastest compiles a test program on the
linear heap unless the program says otherwise, and there a `delete` frees nothing until collection - a
cell's deleted carrier stays, and three carriers in one process held a 256 GB box. A `stocked` cell skips honestly
when its file is absent, so on a bare box (CI) the suite is a run of skips; on a stocked box a PR
owes the files its change reaches (`--changed`), and the whole suite only where the engine core
changed, over the fleet - `test_ple_modes` alone is ~10 min, the suite hours. The runner sets
`DASLLAMA_CPU_PREFILL=1` for every child. That is why the CPU-prefill tripwire cannot ride either
suite: the runner disarms the guard that tripwire asserts. The map below is partial. `run.das`'s
two lists together are the census.
`test_vulkan_dec_tail.das` - model-free (a Vulkan device, else skips): the per-op tier's decode
era against a CPU reference - the decode attention block (K-quant and q8 quads, both rope
pairings, a q8 pair carrying the q/k/v projection bias, the hydrate arms, and a k4 quad beside a q8
quad at ONE plane offset - the block keys a layer by its offset under its format, so each serves its
own mirror), the decode FFN tail, and the whole-token decode span with its
device router + top-k against `moe_select_core`, the span with a shared expert in both its arms -
gated (the shared q8 triple beside the routed pair, its gate logit past the router's, one combine)
and ungated (the same at unit gate, a second span record after a reset; the reference without the
shared expert must miss the device row in both) - plus the `vulkan_moe_span` override reached
through its registry, and the shared expert of two activation classes (`test_vulkan_shexp_mixed_forms`: a q8 gate,
a k4 up and a k6 down through `shexp_gpu_gemms` at one row and three, each plane over its own image against the
reference, every row counted by `shexp_gpu_gemm_rows`, the k4 up fed the gate's Q8_0 image missing its row as the
control), the dense FFN of two activation classes (`test_vulkan_dense_ffn_mixed_forms`: a q51 gate on the per-32 batch tile,
a k4 up and a k6 down through `ffn_gpu_gemms` at one row and three against the engine's scalar dots, the same control), and
the routed mx4 form (`test_vulkan_routed_mx4_form`: native-MXFP4 gate, up and down stacks through `matmul_moe_gpu_ffn` on the
decode GEMV arm and the batch arm, plain and under gpt-oss's form - the per-expert gate/up bias rows and the clamped swiglu,
`MoeGpuFfnForm` - against the engine's mx4 scalar dot, the plain reference missing the biased rows as the control), the routed
pair of two activation classes (`test_vulkan_routed_two_forms`: a q8 gate fed Q8_0, a k4 up fed Q8_K off the call's second
image, a k6 down, on the decode GEMV arm and the batch arm, the up stacks fed the gate's image missing the rows as the control),
and the heat pool under gpt-oss's form (`test_vulkan_heat_mx4_form`: mx4 stacks streamed, two experts pinned, a top-k of three
whose two hits run on the async chain under the biased clamped swiglu with their experts named, against the engine's scalar
dots, the plain reference missing the hit rows as the control).
`test_vulkan_tier.das` - model-free (a Vulkan device, else skips): first the tier's shared one-shot command, held before anything
engages the prefill batch tier and after a whole-model drop; then the per-op tier's expert, dense, deltanet and attention
chains against CPU references; among them `test_vulkan_deltanet_two_forms` - a q8 qkv plane fed Q8_0 beside a k4 z plane fed
Q8_K off the call's second image, on the window chain (70 rows) and the decode step (one row), against the CPU chain over each
plane's own image, the z plane fed the qkv's Q8_0 image missing the rows as the control.
`test_vulkan_shared_device.das` - model-free (a Vulkan device, else skips): one device a process
(`../ARCHITECTURE_GPU_VULKAN_RESIDENCY.md#vk-shared-device`) - a worker thread's context arms the tier on the device this
context made, holds a reference while it lives (the holder's count one up) and gives it back at its shutdown, sees the
32 MB buffer this context holds among every other context's bytes (`vk_others_dev_bytes`) and its resident cap
(`carved_budget`) give them up, and lands its upload and read-back round trips; the buffer destroyed, a second worker sees
exactly its bytes gone from the others' count and back in its cap (`destroy_device_buf`'s take-back); then one worker more
than the device's compute queues, so two contexts share a queue, the device's queue counts the family's at most
`VK_DEVICE_QUEUES`, the workers' consecutive takes covering every compute (and transfer) queue - round robin, not one
queue for all - every context's round trips landing and every reference given back; `test_vk_device_queue_ask` holds
`set_device_queue_ask`'s clamp to one and `device_queue_count`'s min of the ask and the family's count;
`test_vk_last_context_out` (in a child where another cell holds the device) holds the last-out release - a worker that
made the device leaves the holder empty and its bytes at zero, and the next worker makes a fresh device and serves on it, and again after a `volkFinalize` (the next context's
`volkInitialize` loads the loader anew);
then the shared-queue cell and the lifecycle cell rerun in a child under the Khronos validation layer
(`VK_INSTANCE_LAYERS`, the loader's layer line its witness; skips where the layer is not installed) - an unlocked
submit or wait on a shared queue (`UNASSIGNED-Threading`) or an object left alive past its parent - a device past its instance
(`VUID-vkDestroy*`) - or, in the lifecycle child, an instance never destroyed (the loader's layer log unloads the layer
fewer times than it inserted it) turns them red; the layer's other errors (a packaged layer older than the driver's pNext structs)
are not the lifecycle's and are not counted.
`test_vulkan_kv_codec_kernels.das` - model-free (a Vulkan device, else skips): the K/V mirror's
block-codec kernels against the CPU cache's own codec (`dasllama_kv_codec.das`).
`test_vkc_q8_store` holds the q8_0 store's bytes to `quantize_q8kv_row` at token rows (two regions,
the layer's base an odd block so its blocks start two bytes off a word) and at window rows (the
`_b` stamp), every byte outside the stored rows left at its fill, one K block's and one V block's
values past the half range so each scale stores as 65504 where the CPU's convert stores an infinity
(the expected bytes clamped the same, one clamped scale a side asserted; either side's clamp
removed reds the cell). `test_vkc_tq4_store` (a K head and a V head past the half range) holds the
tq4 store's bytes to `fwht_signs_row` + `quantize_tq4kv_row` and the q rows rotated in place to
`fwht_signs_row`, bit for bit, at heads of 32, 64, 128 and 512, on a shared-KV layer's dispatch
(q rotates, nothing stores, the mirror left at its fill), at window rows, and at window rows with
no q head (the NextN head's prompt warm). `test_vkc_q8_da_attn` and `test_vkc_tq4_da_attn` run one
arm table (`codec_attn_arms` in `_vk_codec_attn.das`) over each codec's stamps, dispatched as the
driver dispatches the pass (a row a `rowwg` workgroups, its region in its token block), against the CPU
attention over the rows `cvt_q8kv_to_f32` / `cvt_tq4kv_to_f32` read back at 1e-4 relative (reads
6e-8 to 2.5e-6): the four-head and two-head slabs, a short last slab, heads of 32, 64, 128, 256
and 512, the keys in one piece and split with the last piece combining, two rows a dispatch over
two mirror regions, the pass's Q8_0 and Q8_K requant folds (the quants and scales against the CPU
quantizer), the gated query, a 40-key window, the window under a softcap, and the sink stamps
unsplit and split. Every arm's control is the oracle without the codec - under q8_0 the rows
before their quantization, under tq4 the query before its rotation - and an arm that turns a
mechanism on (the gate, the window, the softcap, the sink, a second row) also holds the device row
outside the bar of the oracle with that mechanism off. `test_vkc_shadow` holds both codecs' f16
shadow (`KvShadowT`) to the CPU dequant rounded to half, bit for bit, the rows outside the pass
left at their sentinel, one block's dequant past the half range landing the clamp.
`test_vkc_tq4_unrot` holds the un-rotation to `fwht_unsign_row` bit for bit at heads of 32, 64 and
512 and under the head gate at 128 at the approx bar (the ungated rows the control).
`test_vulkan_tts_conv_kernels.das` - model-free (a Vulkan device, else skips; the half im2col cell
also skips where the device serves no f16 tile - neither cm2 nor the KHR tile at subgroup 32): the TTS tower's
Vulkan sequence classes against the CPU chain - `test_vkt_tts_im2col` feeds the conv im2col
(`TtsIm2col`) through the biased f32 GEMM over the slab-layout weight rows and holds the result to
the CPU `conv1d` end to end, a forward k5 conv at 22 channels (the column rows' pad past k x cin
under poisoned weights, the columns at an offset) and a transposed k4 stride-2 one at 16 (x at an
offset); `test_vkt_tts_im2col16` feeds the half im2col (`TtsIm2col16`) through the f16 tile the
device serves (cm2, else the KHR twin) over the halfword weight rows and the bias pass
(`TowerBiasAct`), the decoder's route, on operands of the f16 lattice - every product exact, so the
bar covers the accumulation alone - the same forward k5 conv, the transposed k4 stride-2 one and a
k3 conv over 32 channels whose 96-wide row sits off the tile's 64 step, the half column plane
carrying the driver's slack rows; `test_vkt_tts_lstm_dir` holds one LSTM direction (`TtsLstmDir`) at hidden 64 and 200 over
gates the CPU `linear_rows` computed, both directions into one shared row set (the backward w_hh
and gates at offsets) against the CPU `bilstm`, and the forward weights walked backward against the
CPU's backward walk, which must move the output off the forward walk; `test_vkt_tts_pool_dw` holds
the depthwise transposed pool (`TtsPoolDw`, k3 stride 2 pad 1 over 22 channels, the output pad
reaching past the input on the last row) against the CPU `conv1d_rows_transposed_depthwise`, the
weight and bias rows at offsets in one slab plane between poisoned slots; every output under a NaN
fill, every compare with its poisoned element.
`test_vulkan_tts_kernels.das` - model-free (a Vulkan device, else skips): the TTS tower's Vulkan
kernel classes against their CPU oracles - `test_vkt_f32_gemm` holds the f32-exact tile GEMM's two
stamps (plain, biased) at shapes off every tile and workgroup multiple against the k-ordered sum,
with the weight pad past K poisoned and a poisoned-element control on each compare; and the PL-BERT
seat's row classes - `test_vkt_row_gather` holds the embedding gather bit-exact against the CPU
word + position + type sum at element bases off zero (controls: the id-0 sum misses the device, two
positions on one word differ, the prefix before the output base keeps its sentinel),
`test_vkt_gelu_tanh` the tanh GELU against the CPU `gelu` at the 1e-5 f32 bar over a run with a
three-element tail and inputs past the tanh clamp (control: the bar tells the f16 LUT form apart),
`test_vkt_leaky` the leaky ReLU bit-exact against `leaky_relu` (a negative element scaled, a
positive one passed) - both maps leaving the elements outside their run untouched - and
`test_vkt_attn` the bidirectional attention against `attention_rows` at the 1e-5 bar at t 37 /
4 x 64, t 130 / 2 x 128 and the full 512-key row / 2 x 64 (control: zeroing the last key moves the
first query row); and the predictor and decoder row classes - `test_vkt_concat` the concat rows
(`TtsConcat`) bit-exact against the CPU concat on two arms, one style row broadcast to every position
(`rs = 0`) and the f0 and noise columns at their strides with a zero pad past them (control: the
columns follow their strides), `test_vkt_sigsum` the duration sigmoid sums (`TtsSigSum`, 50 of 64
columns, the pad columns carrying garbage) against the in-test f32 sum at the approx bar (control:
the pad columns stay out of the sum), `test_vkt_colstats` the column statistics (the four passes
`TtsColPartSum`, `TtsColReduceMean`, `TtsColPartSq`, `TtsColReduceSq` over 128-row blocks) against
double-precision sums at t 45 / 72 channels, t 300 / 20 and t 1000 / 300 (eight blocks, a lane's
second channel),
`test_vkt_adain` the statistics then both fused AdaIN stamps (`TtsAdainLeaky`, `TtsAdainSnake`) against
the CPU `adain_rows_into` followed by `leaky_relu` or `snake_rows`, the style rows at an element base
off zero, plus the leaky stamp in place (bit for bit the out-of-place rows, the input overwritten),
the elements past the run kept, and `test_vkt_add_scale` the residual join (`TtsAddScale`)
bit-exact against (a + b) / sqrt(2) in f32 over 1030 elements, out of place and in place,
the elements past the run kept.
`test_vulkan_tts_source_kernels.das` - model-free (a Vulkan device, else skips): the TTS tower's
Vulkan decoder tail against the CPU chain - `test_vkt_axpy` holds the stage sum's axpy (`TtsAxpy`)
bit-exact against o + 0.25 y in f32 over 1030 elements at offsets, accumulating onto a filled o and
overwriting a NaN-filled one (control: the same dispatch at zero = 0 adds o's old contents), the
elements outside the run kept; `test_vkt_reflect1` the last stage's reflect pad as the driver runs it
(two `TtsPkRows` copies, 37 x 24 rows) bit-exact against the engine's `pad_reflect_left_rows_inplace`,
the sentinel past (t + 1) x c kept; `test_vkt_source` the harmonic source
(`TtsSrcLow*`, `TtsSrcCumsum*`, `TtsSrcNoise`, `TtsSrcSines*`) on either resample law's stamps in the
Metal gate's shape (up 300, three harmonics, 700 frames) and at up 2 x 37 frames (the first taps
read sample 0, the initial phases in) against the CPU `sine_source` chain through `linear_rows` and
`tanh_inplace` - the phase frames within one ulp of its carry, the mixed signal within 2e-6, the
source linear at offsets in a slab plane between poisoned slots, a poisoned f0 frame that must red
both compares - then on the big shape a 40000-frame cumsum against a double accumulator where the
two laws part by whole cycles, and the own noise draw against the CPU hash (finite, repeatable per
seed, moved by seeds 78 and 81, apart from the captured rows' signal); `test_vkt_stft` the STFT on
either pad law's stamp (`TtsStftReflect`, `TtsStftEdge`; the Metal gate's 10 frames with a fourth
bin whose imaginary row is zero, reaching the zero-imaginary phase rule on both signs) and
`test_vkt_istft` the inverse STFT (`TtsIstft`) with and without the window envelope (two zeroed
window taps leaving a quarter of the samples undivided) against their double-precision forms, each
with a poisoned input that must red the compare, the weights at offsets in one slab plane; every
output under a NaN fill, every compare with its poisoned element. The fills, the source fixture and
CPU chain, the long cumsum's laws and the STFT and inverse STFT double forms are the Metal StyleTTS2
gate's own, shared through `_tts_kernel_oracles.das`.
`test_vulkan_tts_pocket_kernels.das` - model-free (a Vulkan device, else skips): the Pocket TTS
family's Vulkan classes against the CPU chain - `test_vkt_pk_rows` holds the codec stream's row
copies (`TtsPkRows`, `TtsPkRowsElu`) over a 7 x 13 window at source and destination strides with
column and row offsets off zero, the plain copy bit-exact and the ELU stamp against the CPU
`elu_rows` at the approx bar (controls: the negative elements move off the plain copy, the
non-negative ones pass bit for bit), every element outside the window left at its sentinel;
`test_vkt_pk_row_scale` the layer scale (`TtsPkRowScale`) in place at t 37 x 72 bit-exact against
`layer_scale_rows`, the scale row at an offset and slack past x (control: the input row is overwritten);
`test_vkt_pk_attn` the causal cached attention row (`TtsPkAttn`) against `attention_causal_rows`
over a `TtsKvCache` of two 64-wide heads, the device's key and value rows written from the cache's
own layouts with three poisoned rows past the appended 45 - a 45-query prompt over every key, a
decode step at position 44 over an 8-key window (the query its own one-row plane) and the
prompt over a 16-key window - at the approx bar (controls: a poisoned key just before the decode
step's window leaves its output bit for bit, the 16-key window moves the prompt's rows);
`test_vkt_pk_attn_grouped` the same row over grouped keys - a 21-query prompt of four 32-wide query
heads over two key-value heads, the caches' rows at the key-value width (`kvs`), every key and an
8-key window, against `attention_causal_rows` (control: the window moves the late rows off the
every-key rows, through `check_exceeds`);
`test_vkt_pk_gemv` the row GEMV's five stamps (`TtsPkGemvDot`, `TtsPkGemvLnSilu`, `TtsPkGemvGate`,
`TtsPkGemvAddSilu`, `TtsPkGemvTail`) at nin 200 (off the lane multiple) and nout 45 (off the
four-row multiple) against `linear_vec` under the CPU chain's norm, modulation, SiLU, gated
residual and frame tail, the weight rows (their stride pad poisoned), bias, norm rows and slab
vector at offsets in one slab plane between poisoned slots, x, the modulation rows, the noise row,
y and the latent row at offsets, in two arms - the weight rows off the four lattice (the float
dot) and on it (the float4 dot) - each arm asserting its rows' alignment (controls: the gated
residual moves its row off its input, the masked rows past nout keep their sentinel); `test_vkt_pk_gemv_fused` the frame loop's fused stamps
(`TtsPkGemvLnQkv`, `TtsPkGemvAddScale`, `TtsPkGemvAdd`, `TtsPkGemvLnGelu`) at a 64-wide backbone of two
heads against the CPU chains - the normed qkv row's q span roped into y and its k span roped and v span
copied into the caches' row at the position (`rope_rows` on `linear_vec` over `layernorm_rows`),
the residual joins with and without the layer scale over a residual row, the normed GELU (controls: the
k and v spans and the caches' other rows keep their sentinel and poison, the residual row moves off its
input), then the qkv stamp's grouped form over the same weight rows - one 32-wide key-value head under the
two query heads, nout = d + 2 dh, the k and v spans at the key-value width into caches of that width
(control: the v row is the narrow span's, outside the bar of the wide form's v span through
`check_exceeds`); `test_vkt_pk_rope` the rows rope (`TtsPkRope`) on the
k span of 23 rows x 384 at column 128, two 64-wide heads at position 37, the tables from
`build_rope_tabs`, against `rope_rows` at the approx bar (the q and v spans bit for bit untouched);
every written-only output under a NaN fill, every compare with its poisoned element.
The four `test_vulkan_tts_*` files and `test_vulkan_tower_kernels.das` share one cell harness:
`_vkd_oracles.das`'s `CellPlanes` declares a cell's planes (`in_`, `in_plain`, `out_` under the NaN
sentinel, `out_words` under a word sentinel of the cell's choosing, `io_` in place, each at an element
base between poisoned slots; a hazard bit a written plane each, or the bits the cell names when its planes
share one), `cls_set` binds a class over them, and `cell_record` records the dispatches - the cell itself
computing each workgroup count - then stages every written plane back, read as floats (`plane_run`) or
typed (`plane_as`); `plane_upload` and `plane_refill` re-lay a plane between two records; `_compares.das`'s
`check_rows` - `check` at a row and head width - holds a compare to its `Bar` (exact, relative with an
absolute floor, relative to the expectation's largest magnitude, or per element against an envelope or a
bar array; a NaN or a length difference is always off), logs the measured max difference with its row,
head and column and the bar there, and runs the poisoned-element control; `check_count` is its count
alone, and `check_exceeds` the control's verdict over it - every element of both sides finite and the
count above 0 - what a control that poisons the kernel's input asserts. The in-test forms
the cells share with the Metal gate, and the CPU re-derivations of engine loops the engine keeps
private (the ALBERT embedding sum, the duration sigmoid sums), live in `_tts_kernel_oracles.das`, which
requires no device module. The kernel cells' other shared references - the engine's table rope by its
pairing (`rope_tab_ref`: the partial form whenever the rotated span is short of the head, else NEOX or
interleaved), the gated activation (`act_mul_ref`, which `_vkd_oracles.das` re-exports), dense GEMM rows
accumulated in double with their envelope (`dense_ref_env`) and the Q8_0 requant block check
(`q8_blocks_off`) - live in the backend-free `_kernel_oracles.das`, and gemma4a's windowed RPE attention
(`g4a_attn_oracle`) beside the softmax row in `_attn_oracle.das`. The CPU kernel cells' K-quant and q8
inputs - the synthetic disk superblocks every element of which a gate knows by formula (`synth_block`), the
hash-patterned ones, the per-row salted plane regions (`build_kq_region`), the Q8_K activation rows
(`build_q8k_acts`) and a row through the engine's Q8_0 or Q8_K quantizer with the dequantized row a GEMV
reads (`q8_roundtrip`, the Vulkan dec-tail references' feeds) - live in the root-free `_kq_fixtures.das`,
which re-exports `_kq_dot.das`; a builder moves there byte for byte, so no cell's input changes.
`_kq_dot.das` also carries the run-time-format kernel calls a cell walks every format through -
`kq_gemv_rt`, `kq_tile_rt` and `kq_grp_row_dot_rt`, each a `kq_fmt_stamp` over the engine's tag family
(`kq_gemv_gen`, `kq_tile_gen`, `grp_row_dot`) - so a gate holds one call where it held a ladder of fourteen; the
grp row dot is a kernel under test there, never a gate's expected value. The one overload it adds is the panic
`grp_row_dot` of the formats with no superblock grp planes.

`test_vulkan_tower_kernels.das` - model-free (a Vulkan device, else skips): the vision and audio towers'
kernel classes against their CPU oracles - the bidirectional flash tiles (h64 and the padded h128
on the cm2 and KHR arms) against `attn_row_oracle` over every key, the causal twin as the control
that the mask switch moves the output while the last row agrees, sentinel slack rows past kvlen as
the tail-mask control, and the poisoned-element control on the bar; and the tower row classes - the
weighted rms, the in-place clamp and its copy into a second plane (the source left as found), the clamp-and-f16 and clamp-and-Q8_0 feeds, the two-axis neox
rope, the post-add with its next pre-norm, and the clamped GEGLU-quick - each against the CPU tower
helper it mirrors (`rms_rows`, `clamp_rows`, `requant_rows_q8_sized`, `rope_neox_2d_rows`,
`add_inplace_rows`) or its closed form, over one command buffer, every written-only output under a
sentinel fill before its dispatch (NaN words; a NaN in both halves for the f16 panels, -128 bytes
for the Q8_0 quants), with a poisoned-element control per bar, and the flag arms - the post-add
classes at `ln_on = 0` (the seam alone, the pre-norm output proven untouched by its sentinel) and
at `ascale = 0.5` (the branch at half weight, the CPU form weighted the same), the plain seam stamp
(`TowerPostAddPlain` at `ascale = 0.5` and at `ln_on = 0`: the branch added at half weight with no
post-norm, told apart from the post-norm stamp), the bias class at `act = BIAS_ACT_NONE` (the bias
alone against `add_bias_rows`) and its relu arm against `max(x + b, 0)`; the vision stems' and tails' row
classes (`test_vkt_tower_vision_rows`: the patch im2col under the [0, 1] -> [-1, 1] map against
`im2col_rgb_patches` with its pad rows zero, the 2-axis position add, the merge-walk stem assemble, the
grid pool against `avg_pool2d_rows`, and the standardize rows on both arms, each with its poisoned
expectation); the biased-block classes (the layernorm, the bias with its tanh
GELU, the seam with its next layernorm, the head restrides to the tile's 128 and the rope on a
fused row's k slot) the same way, and their f16-feed twins (`test_vkt_tower_f16_feeds`: the
layernorm and the seam storing half, the biased restrides (the bias row added as the pad reads),
the half unpad and the bias + activation storing half on its tanh, erf and relu arms, and the seam
storing half at `ln_on = 0` - each bit for bit the f32 stamp's rows on the same input rounded to
half, the seam's against the CPU form, every bar with its poisoned element; the guards the
fixtures reach carry garbage past the live region and sentinel rows past the live rows - the
rel-plane attention's idle lanes, the depthwise convs' rows past the image - and assert them
untouched); the whisper decoder's classes
(`test_vkt_wdec_classes`: the K/V store's three layouts - the f16 plane at a position with the v
bias folded, the CPU chain's scaled transposed keys and its biased values - bit for bit, the
positions past the batch left at the sentinel, then the chunked attention pair over the planes
the store wrote against the CPU decoder form: the causal rows across a chunk boundary, the cross
rows inside the first chunk with the second chunk's partial empty, the scaled-keys and
causal-versus-cross controls), and its fused row passes (`test_vkt_wdec_fused_rq`: the layernorm, the post-add with its next
norm, the bias + tanh GELU and the attention combine, each fused with the Q8_0 requant, byte for byte and scale for scale
against the pass followed by `TowerClampRq` at three shapes - the 256-wide row of the other cells, the 384-wide row of whisper
tiny (the strided loops' partial tail) and turbo's 1280 (the block pass's second trip), the combine at one chunk and at two -
the bias arm and the combine held to their CPU forms at the approx bar as well, the residual rows in place the same and moved
off their input, a poisoned input moving each one's bytes); the row-count rule of the GEMM records (`test_vkt_tile_rows`: a
record on the l column rounds to it, the s and m columns and the batch tile keep the raw count - a CPU helper, no dispatch,
no poisoned element);
the audio fronts' classes - `test_vkt_tower_front_movers` (the k3
s2 p1 im2col over two chunks with its pad and zero tail, the stem's columns through `TtsIm2col16` on
both source layouts and strides, the qwen3a and canary feature shuffles, canary's rel-plane head panels (the
restride stamps at one head, with and without the bias row), the clamp + halfword feed, the
position-plane add (the bias class at `d = nelem`: one bias row the width of the rows, nothing past
it touched), the qwen3a finish as its two bias passes (the conv_out bias row, then the chunk's
position rows as one bias row the width of a chunk) and the subsample LayerNorm + ReLU - the data movers bit for bit as halves or floats, the
rest at the approx bar), `test_vkt_tower_front_mel` (the spectrum on both arms and the log
filterbank in its three forms - gemma4a's max floor, canary's added floor, the whisper
preprocessors' mel-major log10 - against double-precision sums with the quiet frame flooring on
the max arm and reading under it on the add arm, canary's per-feature normalization with its zero
tail, the depthwise conv and the sinusoid rel table with its (0, 1) zero row), `test_vkt_tower_ga_mel`
(gigaam's device mel on the templates the Metal cells stamp - the frame window bit for bit with its zero pads, the
clamped-log filterbank with a frame at the floor and one past the ceiling, each with its poisoned element) and
`test_vkt_tower_cn_attn_rel_plane` (canary's rel-plane attention on both head widths over planes
the CPU built as the f16 GEMM lands them, against `cn_attn_ref` - the full rel-pos loop with the
u/v biases - the scaled keys and the scaled rel table as controls);
the padded attention route end to end (pad, the h128 bidirectional tile, unpad over sixteen 72-wide
heads) against `attention_bidir`; and the window classes - the f32 per-window attention over the
compact rows on sixteen windows (one ragged) against `attention_bidir_windows` with full attention
over the same rows as the leak control, the rms seam and the gated hidden (the shared bias-gate stamp
`TowerBiasGate16`, the qwen25v hidden's) against their CPU forms;
the bias class's erf arm (the whisper-class towers' GELU) against `gelu_erf_batch` at 1e-5
relative - the shader's f32 evaluation of the CPU kernel's erfc fit - with the tanh arm missing that bar as the
told-apart control, and its silu arm (canary's FFN) against `silu` with the tanh arm as its
control; the Conformer classes (the silu rows - the bias class's silu arm with no bias row - the
GLU, the causal depthwise taps, the chunk-12 relative-position attention on both head-width stamps,
the 128-wide one over scaled queries with the oracle at four times the cap as the softcap's
control) and the FastConformer conv module tail (`test_vkt_tower_fastconformer`: the centered
depthwise conv with the folded BatchNorm and silu; `test_vkt_tower_ga_dwh`: gigaam's conv over f16 taps, `TowerGaDwH`,
on the Metal cell's shapes and CPU loop) against the CPU loops' forms, GigaAM's CTC head pick (`test_vkt_ctc_pick`:
`CtcPick` on the Metal cell's cases, `_ctc_oracle.das`'s `ctc_pick_cases` - three widths, a tie every third row picking the
lower class, NaN pad slots past the classes that red a plog if read, an all-NaN row picking class 0 - every record written,
a poisoned bias the control), the Bergamot step's three stamps (`test_vkt_nmt`: `NmtEmbed`, `NmtSsru`, `NmtPick` on the Metal
cells' legs over `_nmt_oracle.das` - the planes sized to the 256-lane grid with their slack under the sentinel, the SSRU's
layer-1 cells moved off their input, the pick at vocabularies of 200, 1000 and 32000 with every case's poisoned leg, the controls
through `check_exceeds` and `nmt_pick_moved`) and its attention pair (`test_vkt_nmt_attn`: `NmtAttnPart` + `NmtAttnComb` at heads
of 32, 48 and 64 over 300 and 150 keys against `wdec_attn_ref`, the keys scaled twice the control), the attention
controls scaling the keys (a uniform shift leaves a softmax alone); and the chat projector tails' row
classes (`test_vkt_tower_tail_rows`: the pair pool against the mean of each row pair, the swapped SwiGLU
against `swiglu_swapped_rows` with the halves-swapped form as the control). Every compare in the file
carries its own poisoned-element control - a value added to the expected element that must red
the bar - and every float compare runs through `check_rows`, which logs its measured max difference;
the attention oracles share `_attn_oracle.das`'s softmax row, `attn_row_scored`, each spelling only
its logit law. The attention and GEMM classes the towers ride are the kernel file's.
The audio towers' Vulkan twins: `test_whisper_vulkan_twin` (`test_whisper.das`; whisper tiny and
large-v3-turbo on jfk, the q8 default load: the same text on the CPU q8 chain and the device chain,
the token flips logged, the counters proving every chunk's blocks and conv stem served, the decoder
driver pinned off for the legs, then the encoder's rows over the first chunk three ways against the
exact model minted in memory at the audio bar, with the input poison - a q8 encoder minted with half
its blocks' GEMM planes zeroed, through the device chain, must EXCEED the bar), and in `test_audio.das`
the three-way twins - the exact f32 tower on the CPU the reference, the q8 tower on the CPU q8
chain and on the device chain, the device's rel_l2 from the exact chain within 1.5x the CPU q8
chain's own plus a 1e-4 floor (`tower_twin_bar` over `tower_parity_stats`), each with the input
poison (a fresh q8 tower with the GEMM planes of an eighth of its blocks - one on a shallow tower -
zeroed from the middle block on, through the device chain, must
EXCEED the bar): `test_encoder_blocks_vulkan` (qwen2audio, voxtral, omni-3b over the synthetic mel
of the Metal blocks cell), `test_gemma4a_vulkan_twin` (the E2B audio encoder over jfk through
`gemma4a_encode_all`) and `test_canary_vulkan_twin` (the canary encoder over jfk's log-mel through
`canary_encode`) and `test_parakeet_vulkan_twin` (parakeet over jfk through `parakeet_encode`), the canary and gemma4a ones also holding the bar on a second, longer encode after a short one on
the same residency (the scratch regrown under it) and on a fresh residency after a shutdown, and
both running their fronts on the device (gemma4a's whole chunk, canary's front) beside the blocks,
so the bar covers the front chains too - the front counter (`convs`) asserted beside the block
counters, and an exact-lane leg (the f32 tower on the device knob) recording the `quant_mode`
decline; `test_qwen3a_vulkan_front` (the Qwen3-ASR bf16 mmproj over jfk): the device mel against
the CPU mel within 1e-3, its counter (`mels`) asserted and no decline counted, then window 0 and
the clip's shorter tail window three ways through `qwen3a_encode` on one residency - the second
window has fewer chunks than the scratch holds - the rows leg fed the device mel, the soft-token
rows at the twin bar, the conv counter proving the front served every chunk and the encode counter
the block loop and the input poison (a q8 tower with zeroed blocks through the device chain must
exceed the bar). Every tower but the whisper twins' is
staged and minted in memory (the qwen3a pair through `stage_qwen3a_tower`); the whisper cells mint
the served model in memory behind the ASR facade (`mint_asr_whisper`), as do their rows compare
and the f32-decoder leg. The three-way twins skip without their carriers, without a Vulkan device under
`DASLLAMA_GPU=1` and on a das_metal build; the jfk-driven cells (gemma4a, canary, parakeet, qwen3a, whisper)
also skip without jfk.wav, and the whisper twin and the qwen3a front when interpreted. Off the f16
GEMM feed (`DASLLAMA_COOPMAT=sdot4`) every front declines `device` by design while the block chains
serve, so each twin's front witness follows the route (`front_served` in `_tower_twin.das` over
`vulkan_tower_front_route`): on the feed the front's counter rose by every dispatch, off it the
counter stayed and the `device` decline was recorded.
`test_vulkan_host_twin.das` - model-free, device-free (a build with neither discrete home skips; the Vulkan home registers the
twin on a Vulkan build, the CUDA home under `DASLLAMA_CUDA=1`): the one-copy rare rail's
kernels and their oracle on every expert format the tier streams, against the planar kernels over a planar disk-order
stack on a Model shell gathered into the device layout by the engine's own gather (`layout_gather_dispatch` under `disk`),
the poisoned expectation the control: the host twin (the discrete templates' own block decode run on the CPU, the shared
`dasllama_gpu_host_twin`'s `moe_gpu_host_twin` / `moe_gpu_host_rows`) on the fifteen superblock formats, the four per-32 ones, mx4 and q8; the row-major GEMVs
(`<fmt>q8_rm_gemv_gen`: q8, the per-32 and MXFP4 planes, the fifteen superblock formats) and their four-token
tiles (`<fmt>q8_rm_tile4_gen`, four activation rows against the planar rows a token). A new expert format lands its cells
here (`HOW_TO_ADD_A_FORMAT.md`, the Vulkan section's one-copy step).
`test_vulkan_moe_cm2.das` - model-free (a cm2 device, else skips): the cm2 expert chain over a
device-side f16 gather, the streamed-group slot hand-off, the streamed split's head on the
per-expert chain (one expert, and two experts through the ring - each its own copy and submit;
the head's routing puts one slot a region, since the by-row combine adds a region's rows onto
their positions at once), and the shared expert's call shape - one region over every position,
the identity slot map at unit weight.
`test_vulkan_kernels.das` - model-free (a Vulkan device, else skips; the two lens cells,
`test_vkd_readonly_stamp` and `test_vkd_lens_readonly_gate`, and the flash ladders' refusal cell
`test_vkd_fa_stamp_refusals` need only the dasVulkan module): the
per-class CPU-oracle units of the Vulkan kernel census (`_vkd_oracles.das` runs the class methods on the CPU as the
oracle; `_vkd_toy.das` is the `[vk_dispatch]` bring-up fixture; `_vk_kq_fixtures.das` holds the
synthetic K-quant expert stacks in the tier's device layout for `test_vulkan_tier` and `test_vulkan_moe_cm2`,
and `../harness/_vk_probe_fixture.das` re-exports it for the Vulkan probes with the word-hash quant
bytes, the packed f16-pair scale word and the device-timestamp scaffold around a recorded command). The per-format tile cells
(`test_vkd_<fmt>_cm2_batch`, one per `kq_sb` format; q8's cm2 tiles ride their own fmt-0 cells
`test_vkd_cm2l_batch` / `test_vkd_cm2m_batch` / `test_vkd_cm2s_batch` / `test_vkd_cm2e_batch`,
and its KHR arm is `test_vkd_q8_khr_batch`, the per-32 plane through the hand-staged KHR tile over the
same two regions, whole and under a k split with the reduce, wherever the device has KHR cooperative
matrices at subgroup 32; `test_vkd_f32_khr_batch` is the same tile over a file's f32 rows (`F32KhrBatch`, each
weight rounded to f16 on the stage) against the f16-rounding oracle over one region, whole and in two k chunks
through the reduce, on the same device condition; `test_vkd_actf16_strided` holds the f16 act over the
strided side rows (`ubase` / `ustride` / `ulen`) against the CPU form, the contiguous read its control; q51's rail rides `test_vkd_q51_cm2_batch` - its l, m and s stamps (the m stamp its e column too)
on a cm2 device and its KHR tile wherever the device has KHR cooperative matrices at subgroup 32,
over the per-32 plane with hand-packed d | m words - and `test_vkd_q51_gemv`,
its Q8_0-activation decode GEMV against the scalar dot's float order at 704 and 1408 (every per-32 GEMV cell also runs the
format's summed twin over one, two and four token rows of two slots each - `per32_sum_rows_cell`: every row its slots' scalar
dots under their weights, a poisoned last row the control, a wider dispatch's rows bit for bit the narrower one's); mx4's rail
rides `test_vkd_mx4_cm2_batch` (the s and e tiles and the KHR tile - the expert-only form has no dense l or m
column - over the doubled-magnitude plane, every nibble code reached), `test_vkd_mx4_gemv` (the Q8_0-fed decode GEMV at 704 and gpt-oss's 2880) and
`test_vkd_mx4_gemv_gu` (the fused gate + up + act + requant twin byte for byte against the split
path and the CPU chain, biased and unbiased, the gate rows' bar with its poison)) run five
arms: the cm2 l/m/s tiles and the
expert schedule's e column (the format's own 128-row e stamp, whose k step is the stamp's - 32 on
iq2xxs, iq2xs, iq2s, iq3xxs and iq3s, 64 on every other format) in mode 4 on an
NV_coopmat2 device and the KHR 128x128 tile wherever the device has KHR coopmat at subgroup
32 - the cell skips only when the device has neither, so a KHR-only card still runs its arm; every
arm runs through the classes file's tile ladders (`cell_arm_set` / `cell_arm_enc` over `cm2_cls_*` and
`khr_cls_*`) in one shared loop (`tile_cell_arms` over a `tile_fixture` - the format's packed planes
handed in, the activation plane and the device buffers built once - with the arms asked for and the
format's oracle passed in), which dispatches two workgroups past its schedule over sentinel map
words (`SCHED_NONE`), the device-written schedules' upper-bound shape, and every arm's rows still
match; each per-format cell is one row of `fmt_tile_rows` (its K, quant words a unit, scale-row form, seeds,
arms, oracle and the sibling codebook's control) through `fmt_tile_cell`, and the q8 fmt-0 cells
(`q8_tile_cell`, `q8_planes` for their planes) run the same loop at one arm each;
`test_vkd_dec_combine_pair` holds the decode span's two combine classes - the routed sum, and the routed
sum with the shared expert's row gated and ungated - to the CPU sum at a width off the workgroup
grid, with the gate's move and a poisoned element as its controls;
`test_vkd_hot_combine` holds the hot expert pool's combine - `MoeCombine` with its slot window at 1 .. k
over slot rows 1 .. k - to the host's row plus the hit rows under their weights, at a width past one
workgroup's threads: the rows of the zero-weight slots are NaN and every output is finite (a product
would carry the NaN), a poisoned element reds the bar, and the same dispatch with the window at 0
loses the host's row; `test_vkd_moe_gather_row` holds the per-expert chain's by-row gather
(`moe_gather_f16_row_cls`) over a nine-row window of 21 bucket rows at widths of 100, 256 and 300 (under
one stride of the 256 threads, whole strides, a tail) to each window row's position row rounded to half,
chunk-local, within 1e-3 of the largest magnitude with a poisoned element, the two slack rows past the
window keeping their NaN-half sentinel and the oracle over the window at 0 missing; `test_vkd_combine_row`
holds its by-row combine (`combine_row_cls`) over a six-row window - one row a position at alternating
slots, a seventh position no row reaches - at a width of 300, the packed weights and the strided layout
(`wstride` 2k, `woff` k, the pick slots poisoned), to the accumulated rows plus each row's weighted down
row within 1e-5 of the largest magnitude with a poisoned element, position 0's row moved off its input,
and the oracle over the window at 0 missing;
`test_vkd_ext_roster` asserts, for every entry of the device-init roster (`vk_ext_roster`: every
Vulkan capability the tier keys a route on, what rides on it), that the entry's presence reads the
same as the arming field it decides, so the roster's log line and the tier's route cannot
disagree; a probe the tier reads and the roster omits is caught by `check_vk_extension_roster`
in `../REVIEW.das`. `test_vkd_upload_batch` opens the staging batch, uploads five pieces at odd
offsets (two overlapping) into one device buffer, closes it and reads the buffer back byte for byte -
the one-submit landing the pool's placement loops bracket (`ARCHITECTURE_GPU_VULKAN_POOL.md#pool-window-prefix`); its
second arm brackets three regions of five eighths of the staging buffer (the second and third each flush what sits before
them mid-bracket) and one region past the buffer (the batch flushed, then the chunked path), every byte read back at its offset.
`test_vkd_direct_decode`
proves a `[spirv_decode]` method called from a kernel body on the plane element (the KHR arm's
staging form: the index travels, the callee chains through the plane) is an ordinary call on
the device, against the same method run on the CPU. `test_vkd_moe_routing` holds the resident MoE
block's routing kernels to CPU oracles: the batched router GEMM at a second layer's offset (at 21
rows, and at 9 - fewer than the tile's half, so every invocation's second row is guarded off),
the per-row top-k against `moe_select_core` (renormalized, and scaled), the device bucket schedule
against its CPU twin at 20 experts and, on the 512-thread stamp, at 512, and the Q8-fed chain's s-only schedule (`SchedArgs.s_only`: every bucket a run of 32-row
tiles, no m piece) - the records and maps word for word over the whole planes (the tile
ladder's two piece lists over a whole m column with an s remainder, partial m columns and
s-column buckets, the ladder spelled out in the oracle rather than called; empty experts; the
four sentinel tails), the slot-to-bucket-row map as a permutation of each bucket's rows (its
atomic cursor fixes no order within a bucket) - and the combine-folded residual step (the shared
row at its gate, ungated, and no shared expert; the in-place residual moved off its input; the
f32 normed row and the f16 twin against one oracle) - every output under a sentinel fill before
its dispatch, every bar with its own poison. `test_vkd_emb_gather_family` holds the token-table gather's
`KqEmb<Fmt>` leaves (every `kq_sb` format) at four rows - three ids off a five-row table past a one-row lead, one
`EMB_ID_HOST` row copying the host's staged row - against the class's own `blk_decode` lanes under the `blk_fold` terms,
and on the formats with a float witness each gathered row dotted with a Q8_K row against `kq_gemv_float_oracle` over the
same table row; `test_vkd_emb_gather_q8_f32` holds the q8 stamp (`EmbGather`) against the CPU dequant and the f32 stamp
(`EmbGatherF32`) against the table's rows, an `EMB_ID_HOST` row on both, each compare with its poisoned row. `test_vkd_gemv_lane_rule` pins the decode GEMV
family's lanes-per-row rule (`gemv_lanes_per_row`) per format class and row length, the grid
formats against the k-lattice ones and q8 at the whole subgroup. `test_vkd_readonly_stamp` reads
the toy kernel's SPIR-V words and holds the lens's derived NonWritable to the binding the kernel
never writes and off the one it writes; `test_vkd_lens_readonly_gate` spawns two `-compile-only`
children (up to 180 s each) proving the lens refuses a `@readonly` binding a kernel writes, the
plain twin as the must-compile control. `test_vkd_f16_gemm` holds the small f16 GEMM class (the
router logits) to the f32 dot of the same f16 values at two padded
shapes and one off every 4-multiple (the reduce's element-guarded tail), the direct store at a
base and the eight-chunk split with the reduce at the same base, the header under the base kept
as the sentinel and every bar with its added-value poison; it skips on a device without the cm2
tile family, the only tile the class rides. `test_vkd_f16_gemm_khr` runs the same fixture's direct
arm through the KHR twin (the E-series projection off cm2) at three shapes, one with rows off the
16-row fragment, wherever the device has KHR cooperative matrices at subgroup 32. the flash tile's lead arm in `test_vkd_fa_cm2` / `test_vkd_fa_khr`, `test_vkd_da_attn_b_row0` and the
`rope_kv_b_row0_arm` of `test_vkd_rope_b_pair` dispatch the flash tile, the batched decode attention and the batched
rope-store at a window row base past zero (`row0`, a segment's): the rows sit behind lead rows of poison in the q,
out and projection planes, the lead rows untouched (the out plane's NaN sentinel, the q plane's poison) and the rows
the oracle's; the three rope-store gates (`rope_kv_arm`, the Metal `rpst_flat_gate` and `rope_store_pocket_gate`)
assert the roped q differs from its input at a rotated index, so the in-place compare has teeth;
`test_vkd_dn_rows_base` holds the deltanet conv and scan at a row base past zero (a segmented window's part: `DnConvArgs.row0`,
`DnScanArgs.row0`) bit for bit the same rows at row 0, the rows ahead of the base keeping their sentinel; the row-0 run
carries one capture after half its rows (`DnScanArgs.cap_n`), its checkpoint state bit for bit a scan over that half alone,
and the whole run's live state missing it (the control);
`test_vkd_qkn_rope_window` holds `qkn_rope_cls`'s window form (three rows past two lead rows, the [k | v] projection rows at
their stride, positions 7 to 9) bit for bit its token rows' form over the same rows, the q rows and both mirrors, the lead q rows
untouched; `test_vkd_dn_tail` holds the window chain's ring hand-off (`DnTail`) bit for bit to the per-channel CPU form at 5, 3, 2 and 1
window rows over 200 channels (the last workgroup partial): the window's last taps rows, and under the taps the leading rows off
the entry image, the words between the two images untouched, the zero-history ring a position-zero call would write as the
control a continuation's ring must differ from; `test_vkd_dn_cap_tail` holds the checkpoint capture's history write
(`dn_cap_tail_cls`) bit for bit to the ring hand-off's CPU form (`dn_tail_oracle`) for a window cut at the capture's row - two
captures in one dispatch, one past the taps and one under them reading its first row off the entry image - the words between
the two rows' images keeping their sentinel; `test_vkd_dn_family` holds the deltanet conv, the
fused step and the two-phase scan to CPU oracles at head sizes 64 and 128 (the step's one-part
and two-part state columns, the scan's four- and eight-lane clusters), and the sigmoid out-gate stamps
(`dn_step_sig_cls`, `dn_scan_p3_sig_cls`, the qwen4exp form) against the same oracles under sigmoid(z) with
the silu oracle as the control each stamp must miss; `test_vkd_hc_seams` holds the hyper-connection
seams to CPU oracles - the grouped rms with the gamma row picked per stream at a site offset (`HcNorm`),
the low-rank row's silu(lo / hc) requant against the CPU quantizer (`HcLoRq`, the scales bit-close, the
dequantized row within one quant step), the gated stream mean (`HcMix`) and the scatter on its four flag
paths - the plain row, the routed sum with the shared row gated, ungated, and with no shared row
(`HcCombine`, the gated and ungated arms asserted apart); `test_vkd_hc_rows` holds the window forms of the
same seams - `HcInit` opening the wide rows as hc copies of every embedded row, `HcNorm` over two wide rows
at once and the last row alone into row 0 (the head mixer's read), `HcMix` over two rows' gated means,
`HcCombine` over rows 1..2 of three, each under its own logits, row 0 untouched - each against the
one-row CPU form a row at a time, the rows outside the dispatch's slice untouched; `test_vkd_ple_side`
holds the n-gram side input's gate and conv (`PleGate`, `PleConv`) to a CPU form at a position every
tap reaches and at one under the taps' reach (the taps before the sequence reading zero), the gate's
ring slot written and the other slots kept, the fixture's stream dots asserted to take both signs, and
the window's panel form - three rows whose taps read the ring before the window and the panel inside it,
then a second position block primed from window rows, and the verify's form - the window's first position
read off the token record (`PLE_POS_TOK`) and three rows committed to the ring's slots of their positions
after the conv (`PleCommit`; 1152 elements, the last workgroup partial), the pos-off-push form asserted to leave the
ring untouched; `test_vkd_dn_scan_narrow`
runs the scan at ds 32 over 64 rows, and `test_vkd_dn_9b_scan` at the 9B geometry - 512 rows,
one row, and the whole `DN_WINDOW` (the prefetch's first-token clamp, the gate arrays' exact
bound); `test_vkd_dn_step_rows` runs the fused step over two rows in two region slots against the
one-row dispatch a row at a time - the row's o row on both forms, its slot's state and both of its
slot's conv-history ring images, bit for bit; `test_vkd_dn_step_rows_sameslot` runs it over three
rows of one slot a dispatch a row (the row in the push, the same-slab verify's form) against the
one-row dispatch run a row at a time on the state and ring the row before it left - every row's o
row, then the slot's state and both ring images after the last row, bit for bit, the state and the
second row's ring image asserted moved off their inputs, and row 1 stepped from the untouched state
as the control that misses. The gemma-family cells: `test_vkd_kq_gemv_k4_gu` (the Q4_K, IQ4_XS, iq3s4 and IQ2_S gate + up GEMVs with
the act and its Q8_0 requant in one dispatch, against the three-kernel path byte for byte and the CPU chain),
`test_vkd_q8_gemv_gu` (the fused q8 gate + up + act + requant, gelu and silu, two depths),
`test_vkd_topk_n` (the N-row command's per-row top-k over four rows against the one-row top-k run a row at a time - the
picks, the weights and the weight blocks bit for bit, every slot's feed blocks its row's - and against the host select,
plain and under the router bias; then the two-deep stamp over 512 experts at ten picks a row, each row's largest logit past
the first 256, the same compares, with the one-deep stamp over the same rows as the control that misses the host select's
picks), `test_vkd_router_gemv_cols` (the router GEMV over three, eight and twelve x rows - twelve in two column tiles,
`router_enc_cols` - against the one-row
dispatch a row at a time, bit for bit, on the bf16 and the f32 stamps and the bf16 halfword stamp at the odd dim 2049, with and without the fused
rms, against a CPU oracle, the widest miss logged with its row and expert, the sentinel past the
last column standing),
`test_vkd_q8_gemv_gu_n` (its N-column stamps - two, four and eight columns, and eleven as an eight-column tile and a
four-column one - against the one-row
kernel run a column at a time, bit for bit, each stamp full and short of its width, over silu and
gelu), `test_vkd_gemv_ncol` (every `KqFmt` member's N-column decode GEMV - q8 and its row-pair stamps, the fifteen kq
leaves, and the per-32 q51 / iq4nl32 / q4032 / q5032 / mx4 leaves - against its one-column
class a column at a time, bit for bit, at one to eight columns and through the tiled
encoder at nine and twelve, the one-column rows against the CPU oracle - the
engine's scalar dots on the per-32 formats - each bar with its poisoned element), `test_vkd_blk32_batch_family` (the
per-32 batch tile `Blk32BatchT` - q51, iq4nl32, q4032, q5032, mx4 - over two regions with
edge tiles and a K tail against the engine's scalar dots, `blk32_gemm_oracle`),
`test_vkd_cls_epi_rows` (the classifier epilogue over four logits rows in one dispatch
against the one-row dispatch a row at a time, bit for bit, and the CPU softcap with every
suppressed id pinned on every row), `test_vkd_cls_argmax` (the classifier tail's device pick over
six rows at two vocab widths against the host's `parallel_argmax`, a tie landing on the lower id,
an all-equal row landing id 0, a row with NaN and -inf lanes landing its widest finite lane, an
all-NaN row landing id 0 and never the empty-slice sentinel, a poisoned row reddening the compare; the served witness is the
regions files' device-mode scheduler cell, which counts the picks the driver landed alone against
the rows it stepped, the sampled fourth request's rows landing logits beside them), `test_vkd_cls_select` (the
classifier tail's selection over six rows at a 262144 and a 1000 vocab against the CPU oracle `select_rows_oracle` of
`_sel_oracle.das` - every id at or above the row's 1024th largest logit with its logit, the ties at the K-th kept, a row whose
ties overflow the landing flagged and landing the cap's worth, NaN and -inf lanes read as the lowest finite, an all-equal and
an all-NaN row, the recent window's logits beside, the denominators within 1e-5 with a NaN read as the worst, a denominator a
tenth of a percent off and an id moved reddening the compares with the first mismatching index logged; the ask words made by
the engine's `sel_ask_words`; its served shape timed at Qwen3.6-35B's vocabulary),
`test_vkd_q8_gemv_ar` (the q8 GEMV whose last workgroup runs the residual step's requant, with
the biased add partner beside the plain step), `test_vkd_q8_gemv_ar_row_twin` (that epilogue
against the row kernel `cls_ar_rq_b` fed the GEMV's own y row, the updated row, the scales and
the quants bit for bit - the sandwich column at gemma-3-1b's and gemma-2's widths and the plain
column; the N-row command's sites take the row kernel where the one-row command's take the
epilogue) and
`test_vkd_q8_gemv_pleact` (the per-layer-embedding act + requant + proj GEMV, two widths, and its columns form over four and three rows against the one-row dispatches bit for bit),
the gpt-oss family's arms - `test_vkd_ar_class` and `test_vkd_ar_rq_fused` add the biased add
partner (the output bias row past the norm row) against the seam and the CPU oracle,
`test_vkd_act_family` runs the unbiased act kernels under the clamped swiglu beside silu and adds
the biased twins (`q8_actrq_b_cls`, `actf16_b_cls`) over a six-expert bias plane, `test_vkd_fa_cm2`
/ `test_vkd_fa_khr` and `test_vkd_da_attn` add the sink arms (the h64 flash stamps whole and under
a 40-key window; the token command's f32 and f16 sink twins unsplit and split, the last piece's
combine seeding the sink), each with the sink-free oracle as the control, and `test_vkd_fa_stamp_refusals` covers
the sink refusals -
`test_vkd_da_attn_rqk` (the decode attention with the Q8_0 and Q8_K requant folded into its store,
unsplit and split - the pass stores the row either way, its last piece combining - on the two-head
slab stamp over the groups it serves, two heads and one, at 64, 256 and 512, every two-head arm but
the 512 Q8_0 one held bit for bit to the four-head stamp (`g4_twin`), and on the four-head slab's own
store at 64; the four scalar-mirror and codec cells - `test_vkd_da_attn`, `test_vkd_da_attn_shapes`,
`test_vkd_da_attn_rqk`, `test_vkd_kv16_readers` - ride `_vk_codec_attn.das`' `codec_attn_arm` at
`QA_BAR`: the scalar mirrors run the same table with no codec control, since the f32 mirror stores
the rows as they are and the fixture's rounding to half stays inside the bar, so the poisoned
expectation and the per-mechanism controls are their controls), `test_vkd_da_attn_bw` (the wide-head
batched attention - heads of 384 and 512, gemma-4's global layers - over offset mirrors, one arm
under a 24-position sliding window, and its f16-mirror twin beside every arm),
`test_vkd_da_attn_b_ring` (the chunked tiles - the 8-row, wide and h128 stamps - over a 192-row ring at positions past it,
against the same keys read flat, the flat read the control) and the flash cells' ring arms (`fa_tile_run` at heads of 128 and 256
under a 40-key window over a 128-row ring), `test_vkd_da_attn_b_h128`'s window and softcap arms (the h128 coopmat tile at a
40-key window, a 0.5 cap and both, the oracle without each mechanism the control), the codec stamps' ring arms (`codec_attn_arms`),
`test_vkd_da_attn_b_gated` (a gated model's chunked attention - q read at the head's [q | gate]
stride, the output under the gate's sigmoid - on the 8-row tile at 128, the wide tile and its f16
twin at 512 and the h128 coopmat tile, each against the gated oracle with the ungated oracle as the
control and a poisoned element; the cell skips where the 8-row or the wide class declines on the
workgroup cap, the f16 wide arm where its own class does, the h128 arm on a device without coopmat),
`test_vkd_act_gelu_past_clamp` (the tanh GELU act on gate values out to +-300 through `actf16_cls`
against the CPU form, every half finite, and the stamp's SPIR-V, kept by `g_vkd_spv_keep`, read back:
every tanh in it reads a clamp's result - a card whose own tanh saturates passes the numeric compare
without the clamp, so the SPIR-V read is the assert that fails), `test_vkd_fa_cm2_h256_softcap` (the gemma-2 softcap tile, the no-cap control in
the same run) and `test_vkd_fa_cm2`'s h512 arm (gemma-4's global heads, the f16 O twin against the
f32 stamp); the KHR twins `test_vkd_fa_khr` and `test_vkd_fa_khr_h256_softcap` run the same fixture
(`fa_tile_run`, `fa_h256_softcap_cell`) through the KHR flash tile wherever the device has KHR
cooperative matrices at subgroup 32, so a coopmat2 card covers both families.
`test_bench_rows.das` - stocked (stories15M, skips without it): the bench rows' contracts on a
real model - the pp rows' capacity refusal (a warmup or a timed rep on a session sized under its
prompt panics before it writes; a session that fits runs the row).
`test_bench_records_schema.das` - model-free: the record store's schema (round-trip, upsert
identity with `workload` in the key, annotations landing only on the rows they select, the
store lister admitting `records/{box}.json` alone) and the record rig's shared seams (the
`-w` workload scope; the stored-row->rig-leg map, `backend`/`flavor` => `metal` | plain cpu |
`accel`, else refused; the tune-stamp gate; the oracle compare's ok/warn/fail bands; the
upstream image-reference parser `parse_mtmd_image` - encode summing, the MTMD_TIMING split,
its refusal arms; the batched reference parser `llama_batched_bench_tg` - the `S_TG` cell of
the row at `npl` alone, 0 on a missing row, a missing table or a cell that is not a number -
and `llama_batched_bench_sibling` reading "" beside a missing reference; the batched
reference row `llama_batched_ref_row` - the leg's arms follow the row's shape on its argv, and
a dead exe names why and reads 0); the annotations' `void_clips` and `held` each reaching only the
run they select, `held` round-tripping the store; the audio outlier rule (`audio_outlier_clips`: a
clip past the multiple of the run's median rate, none on an even run or on two clips; a stored run
rated over the das row's clip lengths, `run_outlier_clips`) and the recorder's void pass
(`void_outlier_clips`: the flagged clip alone re-measured once, voided where it stays off, cleared
where the re-measure lands inside the run, an even run re-measuring nothing); plus the
committed-records sweeps: every batched row's receipt carries its leg's shape (the cpu legs' das
child at `--npl-plen 128 --npl-reps 3`, its reference at `-npp 128`, `--no-op-offload` on the stock
cpu arm alone; every GPU leg - metal and vulkan - its das child on the flat rows' shape and its
reference at `-npp 512`, with neither cpu-only flag), image-chat receipts match their
`backend`/`flavor` stamps and pin the fixture and mmproj, and every das row's `tune_sha`
resolves to its committed generation archive; and the remote-desktop probe's process read: over
an injected listing (`executable_in`) an executable counts by its file name, as a path or bare,
a line under the excepted path does not (the same line with no exception the control), and a
line that only mentions the name does not; live (`process_listed`) a name only the probe's own
command line spells is not listed and the running executable is - that last assert skips where
the executable's name is longer than the 15 characters a Linux process list keeps.
`test_exchange_schema.das` - model-free: the exchange validator, sweeping the ENTIRE in-tree
records/sidecar corpus, so a writer-schema change reds here first; and hostile documents refused
by named path - among them a `held` annotation that is a number or past its length bound, a
ledger-row hold the control.
`test_fat_start.das` - model-free: the runtime section's writer and a fat exe's first-start race
(`dasllama_fat_start`) - the snapshot's crowns and knobs, the race writing the redirected sidecar's
runtime section and the checked apply reading it back (the Metal twins race on a Metal box, ~15 s),
the Vulkan short window's knee race (`test_the_vulkan_knee_race_sets_the_knee`: under an asked GPU tier on a cm2 device the
knob, set to a value no rung offers first, lands on a rung of the race or 0 and the snapshot records it; elsewhere it
stands - a correctness cell, not a verdict on which rung wins), the box-profile apply firing an installed race hook exactly once for a missing section, and the
fat hook declining outside a fat exe; and the per-user sidecar placement - the `~/.dasllama/tune`
path, the one-time adoption of a sidecar beside the exe, the placement hook's wiring (registered
by the engine, run before the apply reads the path, absent without harm), and when a sidecar
location counts as explicitly chosen: a runtime override is, and is the path read; with none, the
location is explicit exactly where `DAS_TUNE_MANIFEST` chose one (the suite runner sets it on a
box with a minted manifest), and that manifest is the path read again once the override clears;
and the per-box runtime file (`test_box_runtime_file`): its path under the home's `tune/` with a
stable key, validity as this box plus these kernels plus a runtime section (other kernels, another
box, a kernels-only file and no file each invalid), the race writing a valid file with no kernels
section that the apply reads, the hook answering "" under an explicit manifest, and two children on
`_box_runtime_root.das` under a temp home (`_child_run.das` spawns them, as the jobque tripwire's
are): one with no manifest reads the box file under that home, raced and written first, and one on
`DAS_TUNE_POLICY=reference` reads "" and writes nothing.
`test_fat_hook_umbrella.das` - model-free: a program requiring the engine umbrella alone (what
the shipped bench requires, never the facade) carries the first-start race hook - the umbrella's
`[init]` set registers it, or a fat bench exe would serve Metal uncrowned forever.
`failed_dasllama_lint_require.das` - model-free, expected-compile-failure: the
facade lint trips DASLLAMA001 (code 50503) on a direct engine require with no escape.
`failed_dasllama_lint_sidedoor.das` - model-free, expected-compile-failure: the lint's tree
guard trips on a path-require resolving into modules/dasLLAMA, name prefix or not.
`test_dasllama_lint_escape.das` - model-free: `options _dasllama_internal = true` admits a
direct engine require (the lint's escape hatch).
`test_dasllama_lint_contracts.das` - model-free: the lint's ALLOWED set (one program on the
facade, scheduler, exchange-schema and bench entries with no escape compiles - one engine-wide
spawn, since each costs the windows nightly runner two minutes; an internal require does not),
`load_audio_16k_mono`'s empty-on-failure contract, `decode_audio_16k_mono`'s frame cap (a
synthetic `sampleRate=1` WAV bomb is refused before decode, an uncapped call still works), and
`gemma4a_probe_proj_dim`'s 0-not-panic contract on `.dlim` / missing / non-GGUF inputs.
`test_mtp.das` - stocked suite, `-jit` only: the NextN loader on the Qwen3.5-0.8B-MTP Q8_0 file
(the extras load; spec off continues the plain file's fixture), the self-speculative greedy decode
token-for-token against plain decode on the counting and prose fixtures - and on the counting
fixture again with the file loaded at `QuantMode.q4_0`, the requant tier the round serves - the
poisoned-verify rollback over the CPU restore and beside it a forced-reject round through the
engine's seam (both sides decoded in the log; the device rollback counter moves exactly when the
device verify counter did - once where the resident driver served the verify, not at all where an
f32 session under an f16 mirror passed both as codec; on the CPU rail the restore and the replayed
step), `test_mtp_depth` - the k-deep round on the CPU rails (`set_mtp_depth`, the pin put back): depths 2 and 4 decode the
counting and prose fixtures token for token the plain stream, depth 4 drafts and accepts more than depth 1 over the same 48
tokens (depth 1 run twice accepting the same count is the control), and the forced reject at depth 3 rolls back to the
accepted prefix, `test_mtp_prefill_seam` - the continuation prefill's seam row on the 0.8B under a fake seam
seat: the head's K row before the window poisoned, a seat that serves leaves it (the seat asked
once for the window's first token at its start), a declined seat on the planar model lets the CPU
seam rewrite it (the control), and a declined seat on the model flagged a blob, a fake prefill
driver claiming the window, leaves it and warns (the blob arm claims the prompt's embed rows through
an embed probe, since the CPU embed of a planar model flagged a blob would read the blob plane) -
`test_mtp_verify_window` - the CPU round's fail-closed exit, the model's device state dropped first
(`moe_gpu_drop_model`: an armed driver whose mirror matches the session's codec serves the verify
itself and opens no window): a fake prefill driver that serves the speculative verify window makes
the round panic naming the driver, the session's verify window (`mtp_verify_window_active(s)`) read
up inside the window and down for the prompt's own prefill and on the session after the round - the 64-trunk-layer Qwen3.6-27B-MTP carrier, and the GLM-4.5-Air arm - the one
non-recurrent MTP model, so the only reach of the depth-1 step's shortcut reject (row 0's logits
and hidden stand, no re-forward): its code fixture runs plain, then again with
`set_mtp_force_reject_every(3)` rejecting every third draft, both token-for-token against plain
decode, the forced run's reject count asserted at a third of its drafts or more (the fixture's own
reject count rides the kernels' summation order and has read zero). The 27B and GLM arms are
large-tier (`DASLLAMA_PARITY_FULL=1`). Under `DASLLAMA_GPU=1` the file reaches the resident driver
only with `DASLLAMA_VK_KV32=1`: its sessions are f32, and without the f32 mirrors every call passes
to the CPU rails as `codec`.
`test_mtp_snapshot.das` - model-free: the speculative round's deltanet rollback sizes its two
snapshot buffers on a bare session carrying a 27B-class recurrent state (151 MB, past the
`max_unreserved_size` guard) and restores the state from them, the n-gram window and the ring
row a rejected draft wrote among it; and why a round stays cold (`mtp_round_cold_why` on a Session shell): each of its four
reasons alone, a round ending on the context's last position drafting; and the checkpoint capture's contract
(`test_dn_capture_contract`, `dn_capture_begin` / `dn_capture_check` on a bare Session): stops inside the window in order open one
unfilled snapshot each at its stop (the control), a stop asked of a session with no recurrent state panics naming it, a stop at
either window edge, before it or out of order panics naming the window, and the check panics naming the unfilled count until
`dn_captured` is set.
`test_mtp_sampled_walk.das` - model-free: the speculative round's sampled accept walk
(sample-and-match) over a bare 8-token session (`bare_session` of `_mtp_walk_fixture.das`, which the accept-walk and
landed-sampler files share with `same_rng`) against plain sampled decode of the same rows -
every draft accepted plus the parked bonus draw, the first miss, RNG state equality (one draw
per emitted token), the restored recent window, and the repetition penalty seeing the accepted
drafts (a walk that forgot the window would accept the repeated draft).
`test_mtp_accept_walks.das` - model-free: the speculative round's two other acceptances
(`set_mtp_accept`, `MtpAccept`) over the same bare session; under the rejection knob a session whose
draft rows recorded no q (the assistant drafter's device picks) walks typical instead of reading past
`mtp_qn`. Typical: a draft accepted where the
sampler's cuts admit it on its row whatever the seed draws (a runner-up inside the nucleus), the first
cut draft replaced by one draw over that row's survivors and never the draft itself, one random draw
a round, each cut rejecting on its own (top-k, min-p, the nucleus over equal leaders), a penalized
greedy sampler judging as the exact walk does with no draw, and the knob routing a sampled stream's
walk (`mtp_walk_by_accept`) through the judge and back (the knob off, the exact walk accepts fewer than the
typical judge's three on some seed of twenty). Rejection sampling: the drafter's draw records q
(the sampler's chain over the head's row, renormalized, id ascending, a point mass for a greedy
sampler; `mtp_q_of` reads it, a q a thousandth off reds the bar), a draft the row gives at least q's mass is always accepted, one the row gives none is always
rejected with the residual draw never landing on it, a draft at half q's mass is accepted on about half
of 400 seeds (at p = q every seed accepts: the control), and the residual over a row that holds q whole falls back to the row's own draw.
`test_walk_cuts_at_a_stop`: with the caller's stop ids set (`Session.spec_stops`), a run of matching drafts ends on the stop
draft - its row the last accepted, the drafts past it cut, no token parked - on the exact walk and on the typical walk, whose
parked draw is dropped; the same runs with no stop ids accept every draft (the control).
`test_constraint.das` - stocked suite, `-jit` only, skips without tinyllama Q8 (its session sizes the logits
row; the cell writes the logits): the sampler under a `TokenConstraint` - a sampled draw with no cut probing
once and refiltering only on a refusal (the admit-all constraint's probe count), the best admitted token when the
constraint refuses the argmax, the whole row walked when it refuses the `CONSTRAINED_PROBE_K` largest (the
candidate list asserted at the vocabulary's size), -1 and `constrain_dead` on both the greedy and the sampled
path when it admits nothing, only admitted survivors under a temperature, one commit per emitted token, the
argmax again with the constraint off; then a scheduler stream under a constraint that admits nothing finishes
`constraint` with no piece emitted.
`test_sampling_landed.das` - model-free: the landed sampler (a device step's selection -
survivors, max, denominator, recent-window logits - in place of the row, `SelLanding`) against
the row sampler with the same seed, the landing built off the cell's own 4096-wide rows by the
selection's oracle (`select_rows_oracle` of `_sel_oracle.das`, the kernel cell's own) behind the cell's own seams
(`install_rdec_select`, the seats and the landing knob put back through `rdec_select_pin` / `get_select_landing`), every
draw logged a side: the nucleus alone, a
top-k within the cap with a nucleus, a min-p alone, penalties over a window spanning landed and
unlanded ids (the denominator corrected), the late landing of the rows where no cut is armed or a
flat row's nucleus reaches past the survivors (the draw then the row's), the three walks'
judges and the rejection drafter's q over landed verify rows, and the chain cut's probability
at the stream's temperature (a probability a percent off reds its bar).
`test_sampling.das` - stocked suite; the greedy oracle and the seeded-determinism cell on
gemma-2-2b (skip without it, `-jit` only), and model-free: the sampler units (cutoffs, penalties,
seeded draws on a bare session) plus the candidate-list fast path against the vocab-wide
reference - three presets over 512 seeded rows (half quantized to plant ties): the survivor set
and probabilities within the bound (a poisoned probability as the bound's control), one RNG
draw on each path, the same token wherever the draw clears a CDF boundary (at least 500 of 512;
a half-mass margin as the floor's control), the k+3 tie set surviving a top-k of k, and a top-k
past the fast cap taking the reference.
`test_chat.das` - stocked suite; the Qwen3.5/3.6 `xml_function` cells (the template sniffed off `<parameter=`;
the first turn and a replayed call turn as the template's own text, the opening's shared head as the tool block, a
call with text alongside, the thinking generation prompt ending inside the think block and the replayed turn
writing its block once, two consecutive user turns through `render_user`), the `preserve_thinking` replay
cell (the think block around the turn's reasoning, ChatML's form), and the chat template renderer per family against pinned token
streams (each cell skips without its carrier; the Qwen2.5 cell also holds `render_turn_marked`'s
opening - the system turn on a first turn, none on a later one, its tokens the pinned ChatML
prefill stream), the tool wires (the Qwen2.5 hermes cell holding the system turn a ChatML template
states for a conversation that opens with none - its text read off the template's escaped string
literal, its tokens ahead of the first user turn), the ToolMode declarations (model-free: gpt-oss harmony, gemma-4's
DSL, llama's whole-reply JSON, the qwen family's hermes, the `[INST]` and v7-tekken template shapes and the mistral3
arch declaring mistral, and a declared mode over a vocab without its control specials demoting to none at create), the
audio marker cells (voxtral's opening
marker, qwen2audio's pair, and the splice at the text offset the message gives - ahead of the text,
inside it, after it - the head and tail carrying exactly the text on their side), the inline-span cell
(`test_chat_inline_span`: a turn marked with `add_user_span_` renders the splice pair's head, the span's
position ids and its tail as one stream at each text offset, a replayed turn keeps its span, and a position
id is no token id and differs by row and by key; `test_chat_span_refusals`: a span of no rows, an image or a
marked audio span on a template with no such marker, and a span ahead of the one before it each panic, a bare
audio span and a span at the last one's offset the controls), and the gemma-4 E2B cells: the
thinking renderer pins (the instruct prefill token for token, the gate + bare opener, the
thinking-off extras on `effective_stop_ids`, the reasoning budget's marks - the channel mark and
`thought` as the tokenizer writes it the open, the channel close the leave and the forced close, both
channel markers the turn's end once the forced close is out - a mid-conversation toggle
staying instruct), `test_chat_gptoss_budget_marks` (gpt-oss-20b, large tier: harmony's marks - the
channel mark, `analysis` as the tokenizer writes it and the message mark the open, the message end
the leave, the end, a start and the `final` channel's header through its message mark the forced
close, the channel mark alone the turn's end once the forced close is out) and
the instruct-mode TEXT turn through `respond` (greedy "2+2": the answer, no channel marker in
the content half, the turn ending on a stop - red when the guard does not end the turn on the
stray `<channel|>` the E2B emits after its answer), the gemma-4 tool wire (`test_chat_gemma4_tool_wire`: the
declaration turn, the call replay leaving the turn open, the result block in that turn followed - thinking off - by
the turn's close and a new model turn, the thought channel re-opened instead with thinking on and the reply split
as one that begins inside it), and `test_gemma4_e_media_body`: one fused media
body (text, the span's own text embeddings as rows, text - one prefill through `media_body_rows`
and `eval_embd_body`) reads the last row's logits within a quarter of their peak of the same
prompt as text / rows / text in three calls, and the same body with its ids cleared - the text
rows under the padding token's per-layer input - reads past the peak (the control). Model-free beside them,
`test_chatml_default_system`: the default system text read off template text and off a string
literal, none where the span holds an expression or the template states no system turn. Beside it,
`test_gemma4_instruct_opener`: a gemma-4 template's non-thinking generation prompt keeps the closed
empty thought where the template writes one (the 12B form) and is the bare model turn where it does
not (the E-series form); the E2B cells' pinned streams carry the bare opener. `test_llama_system_header`: a
Llama-3.1+ template's system header read off the template - the cutoff line, the date the template
fixes (3.1) or none where it has a clock (3.2), nothing on a template with no header (3.0); the
Llama-3 prefill cell pins the date (`set_chat_date`) and holds the header ahead of the system text,
on a conversation with no system message too, and the unpinned stream differing; its Llama-3.2 arm holds the
template's own date where one is set, the `Environment: ipython` line ahead of the cutoff line on a turn that
carries tools and absent without them, and the pinned date over the template's.
`test_think_split.das` - the reply-side reasoning matcher, model-free: every
thinking family's wire shape, whole-string and per-chunk down to 1 byte, and the
instruct-mode stop guard (`nothink_stop_here_`): a channel marker before the reply's first
content piece passes as a leading thought, whitespace is not content, the first marker after
content stops, an unarmed guard never stops.
`test_tool_formats.das` - the per-ToolMode wire codecs (dasllama_tools), model-free: defs
serializers and call parsers for harmony/gemma4/mistral/llama_json against verbatim fixtures, the harmony system
body (`harmony_system_body`: the `Reasoning:` line at each level, the channel declaration, the routing line only with
tools), and the
Qwen3.5/3.6 `xml_function` codec - a replayed call's block, a reply's parse (content before the first block,
an unterminated block, a nameless block, a malformed opener, a key written twice), and the values typed by the
declared parameters (`tool_param_types`: a declared string stays text, a declared number or boolean goes bare).
`test_scheduler.das` - stocked suite; the continuous-batching scheduler (dasllama_scheduler) against
`generate()` references; skips honestly without SmolLM2-135M / the MTP fixture, `-jit` only. Model-free: the idle chunk
on a GPU is the wider of the 2048 floor and the driver's prompt window (`idle_chunk_default`), the CPU's one window. The two deltanet streams on
the GPU tier (Qwen3.5-0.8B Q8_0, `DASLLAMA_GPU=1`, the whole-model driver at its one region or the per-op deltanet
rail) match their solo references with the device's fused deltanet step recorded for every recurrent layer and no call
passed to the CPU rails - two host-cached streams over one region, each taking it from the other, the loser's
device-only rows saved first. The stop-string
cells: `stop_scan`'s hold and release model-free, then a stream cut by a stop string spanning two pieces
(the text before it, `finish_reason` stop) and a held tail flushed at the budget as a piece of token -1.
`test_scheduler_think_budget` holds the reasoning budget's forced close on SmolLM2 with a stand-in two-token
close: a request whose prompt opened the span emits the model's own three tokens, then the close tokens, then
the model's again to its length; the same budget with an open token the model never writes, and with no close
marks, each read the reference token for token; a close token the model writes itself (the reference's second
token) ends the count and forces nothing; a two-token open sequence opens the span after both; a budget of 20 on
12 tokens fires early so the close lands inside the cap; and a re-open marker (the token the first stream wrote
right after its forced close) ends the turn there with `stop`, the marker unemitted.
The SmolLM cells drop the loaded model's GPU state (`moe_gpu_drop_model`) so they serve on
the CPU rails under `DASLLAMA_GPU=1` too: their bit-exact claims hold on one lane, and the
tier's device prefill, resident batch decode and CPU prefill round differently. Its two-stream
deltanet cell needs Qwen3.5-0.8B-Q8_0 and `DASLLAMA_GPU=1` on a box whose tier serves the
deltanet decode step, and skips otherwise. `test_scheduler_media_splice` holds a media span as part of
the prompt the prefix cache matches: a splice at the head, middle and tail equals the all-token stream;
the non-causal flag reaches the kernel; and on a paged scheduler the text ahead of a span attaches and the
hit stops at the span, the media stream donates, the same media asked again attaches past the span and
reads the same stream, a request with no rows is served off the cache, other rows at the same place share
only the text ahead of them (the control), and a rowless request on a cold cache finishes `media_lost` and
donates nothing. Its splice arms hold the span and the text around it at one prefill call (`prefill_evals`),
a body cut at the chunk at a call a chunk past it, and a non-causal span between text at `generate_embd`'s
span form. `test_scheduler_span_cut`: a hit that would end inside a span attaches nothing past the span's
start and the stream reads its own reference. `test_scheduler_span_validation`: a span of no rows, spans out
of order or overlapping, a span off its position ids, rows of the wrong width, inline spans on a device-home
scheduler and a splice beside inline spans each refuse, a well-formed request the control. `test_scheduler_mrope`'s
paged leg asks the grid request twice: the second attaches the span off the cache and its session carries
the span's rope advance (`rope_pos_delta`). `test_scheduler_idle_quantum` holds the prefill
quantum's two sizes by the tokens one tick prefills: a lone stream's first tick takes the whole
prompt under the idle quantum and one chunk with it off (`idle_chunk_tokens = 0`), the two streams
token for token alike under classic prefill, both decoded in the log; a prompt admitted beside a
decoding stream takes one chunk; an unset chunk and idle size are the serving backend's
(`chunk_defaults`), and `chunk_default` reads `CPU_CHUNK_TOKENS` on the CPU and
`DEFAULT_CHUNK_TOKENS` on a GPU. `test_scheduler_chunk_sizes` and `test_scheduler_mtp` pin the idle
quantum off, since a lone stream would otherwise prefill their prompts in one window. `test_scheduler_device_mode_switch` holds the device
mode's two CPU-decidable contracts: only an idle scheduler switches its session kind, and
`submit` refuses a media request in device mode while it takes the same request with the mode off.
`test_scheduler_mtp_resident_picks` runs a greedy speculative stream through the scheduler under
the resident driver on Qwen3.5-0.8B-MTP (skips without it, without the armed tier, or under
`DASLLAMA_VK_KV32=1`, whose f32 mirror passes the f16 sessions as codec): every tick asks a pick
(the scheduler's ask, the device verify landing picks alone) and `sample_` consumes each - the
stream token for token the plain scheduler's on the same rail, and the device verify counter up
by the drafted rounds; a pick left unconsumed would refuse the next round's opening.
`test_scheduler_mtp_device_home` runs the same speculative stream with the scheduler in device mode (the stream's K/V in
the driver's region alone, `gpu_resident_mtp_device_home` asserted first): token for token the host-cached speculating
stream, the device's draft counter (`moe_gpu_resident_drafts`) and verify counter each up by the drafted rounds, no call
passed to the CPU rails; same skips.
`test_scheduler_mtp_stop_close` (Qwen3.5-0.8B-MTP at depth 2, the bit-exact kernels pinned - `pin_spec_kernels`, put back): a
speculating stream finishes on a stop token at cuts 2, 4, 8 and 10 of the counting reply, its tokens before the stop the plain
reply's, its draft count exactly the figure `drain_owed_drafts` owes (each tick's round the depth under the reply's remaining
length), and a stop lands as an accepted draft at least once (`mtp_stop_rows`); turn 2 attaches the prompt, the reply and the
close whole off the finished-turn checkpoint (`n_cached`) and reads the plain scheduler's turn 2 token for token. Its
controls: the same stream with no stop ids finishes on its length with no round on a stop draft, depth 1 drafts exactly one
row a tick, and depth 2 more a tick than depth 1. The non-close cuts (`non_close_stop_cuts`, a fresh scheduler a cut, since
the cuts' turn-2 prompts share openings): the stop is the reply's own token and `close_toks` the template's `<|im_end|>` and
newline; where the stop's row stands (an accepted draft) the close is not evaled, the finished turn leaves no checkpoint and
turn 2 on the prompt, the reply and the close attaches 0 rows; where the stop was sampled it attaches them whole; turn 2
reads the plain scheduler's either way, and a stop's row stands on some cut. `test_scheduler_mtp_length_cap`: depth-3 streams
capped at two tokens - the one round past the prefill's token drafts one row, so an uncapped round accepts rows past the
reply's end - and at seven, each finishing on its length with the plain reply, its draft count exactly the capped figure,
turn 2 attaching the prompt and the reply whole; the depth-1 control drafts one row a tick and the seven-token stream more.
`test_scheduler_capture_stops` (the same carrier, `pin_spec_kernels`): a counting prompt of 120 numbers (502 tokens; a count,
so every next token is the prompt's own and a fresh window and a capture plus a tail agree token for token) carries the
server's four stops - the head at 48, the system opening at 96, the stable history end at 160 and the message end at the
prompt's last token - and runs four legs: the per-token (`recurrent`) and the chunked scan, each with one window holding
every stop and with 96-row windows, so a window ends ON the system opening (the window's end files it, never a capture: no
panic, `prefill_evals` the window count). Every leg leaves the four checkpoints and the finished turn's, each of its kind -
the head and the system opening openings, the stable stop (the request's own, `ckpt_at_tail`), the message end and the
finished turn tails; the window's reply is an uncached scheduler's; and a prompt sharing each interior stop's opening
attaches that checkpoint whole and continues as an uncached one-window prefill does. Per scan, the state and conv history
captured at 96 inside the one window are bit for bit what the window ending there leaves (both scans cut their chunks at 48
and 96 alike), the stop at 48's state and history differing as the control - the token compares alone are blind to a lost
state on a counting tail. `test_scheduler_capture_stops_ngram` (family `qwen4exp`, arm `fam-qwen4exp`, large tier, a
das_metal build): the served Qwen3.8-Flash-Next UD-IQ4_XS image with its split head (`load_model_cached` under
`set_mtp_head_path`, `MetalMode.required` for the load, speculation off, the Metal prefill and decode overrides selected) runs
the same one-window and window-ending-on-96 legs on 60 numbers (240 tokens); the n-gram side input captured at 96 holds the
last tokens bit for bit and the conv history within a thousandth of its largest magnitude (reads 3e-4: the projection GEMM
rounds the two window shapes apart) of the window ending there, the stop at 48's history outside the bar the control; the
recurrent state's distance is logged (0.24 against the control's 2.0).
`test_gpu_home.das` - model-free, device-free (a switch installs seats, nothing arms): the GPU home as a contract
(`../ARCHITECTURE_GPU.md#gpu-backends`) - the home is the environment's spelling (`DASLLAMA_CUDA`) and among the homes the
build registered, the arm registrant (`moe_gpu_backend_name`) and `gpu_want_cuda` follow it, and the flags a home's installer
declares by value read back as that home's (the CUDA home's site feeds on, the Vulkan home's off);
`set_gpu_home` refuses a name no entry registered (`metal`, the empty name) and leaves the home, answers true for the current
one; the switch to the other home this build carries lands and comes back with every flag following, and where the
environment armed the tier at [init] (`DASLLAMA_GPU=1`) the switch is refused and the home stands - the arm the env runs
decides which arm of the cell runs, so the file runs under both envs.
`test_gpu_shared_units.das` - model-free, device-free, loads neither GPU home: the host code both discrete homes share -
`test_stream_pinned_free_follows_the_maker` installs fake allocators for both homes on the expert cache's pinned seat
(`set_stream_pinned_fns`) and holds `stream_pinned_release` to the buffer's maker (`PinnedBuf.home`): a CUDA-made buffer
through the CUDA free alone, a Vulkan-made one through the Vulkan free alone, a refused one (`mapped` null) through neither;
`test_dn_slot_drop_keeps_the_ring_image` walks the recurrent slot's owner record (`dn_slot_bind` / `dn_slot_advance` /
`dn_slot_drop`): the drop forgets the owner and keeps the ring image the device last advanced onto, the image a parked
claim's checkpoint copies after the owner let go.
`test_gpu_tier.das` - model-free: the segments form's media guard (`test_segments_served_by_override`: under a
probe override registered with a segments form and neither the per-row-table nor the deepstack arm, two plain sessions
take the form, a session with `mrope_pos` rows or `ds_active` declines it, both take it again cleared, and no active
override means no form; the override found is put back), and the say-once helper the GPU driver files share (`test_gpu_said_once`: one entry a reason and the call answering that it said it, a repeat recording nothing and answering that it did not, a cleared set saying it again), and the tier's seat forwarders on doubles - among them the NextN
draft seat (`test_rdec_draft_seat`: the token's id (and a host row's -1), the row, the hydrate start and the pick ask forwarded, the
double's pick answered and the logits left alone under the ask, the unset seat declining until the
reinstall, and the seat unset again at the cell's end so the process reads it as it found it; its
second subtest the norms plane's head block - past the trunk's q/k rows and its own q/k slot, never
on the final norm's row) and the same-slab verify seat (`test_rdec_verify_seat`: the rows, their ids (and a call's empty ids),
the position, the pick ask and every landing forwarded, the picks and hidden rows landed under the ask
and the logits left alone); the unset N-row token command and per-layer planes panicking with the module to require
(`test_unset_seats_name_their_module`); the homes' words (`test_home_words`: two children on `_home_words_root.das`, one with
no ask and one under `DASLLAMA_GPU=1 DASLLAMA_CUDA=1`, each printing `rdec_fallback_words` - the per-op rails' words with no
ask, the CPU rails' words under the CUDA ask on a dasCUDA build - and each home's dry-bake arm refusing a config that names
the other home (`vulkan_dry_bake_arm` a `cuda` home, `moe_gpu_dry_bake_arm` a `vulkan` home under the ask), the arm false and
the refusal line naming the foreign home read off the child's log; the CUDA arm refuses the home before it asks for a device, so
the cell holds on a dasCUDA build with no card); and the
overrides' announces (`test_override_announces`: two children on
`_override_announce_root.das` through `_child_run.das`, unset and under `DASLLAMA_METAL_FUSE=0`, each case's lines read between
its `CASE` records - the sliding layers' rings ringed, flat and silent on a model with none, on Metal once a layout; the pinned
host embed only where the ids seat would gather; both device-gather lines; the Metal batch gather, gemma-4 MoE tail and fuse
pins, the fuse line once a state, the knob's at context init; the assistant drafter's sidecar lookup, no resolver registered in
the root, finding none and naming the module to require).
`test_gpu_serving_declines.das` - model-free: the whole-model driver's decline reasons decided
from a Config or a synthetic Model shell, and a trimmed lane's host-layer split read off synthetic
plan entries (`vk_baked_host_layers`: the role-none marker entry's woff, -1 off a lane minted before
the marker; `resident_unserved_features`,
`attn_chain_unserved_features`, `resident_layer_decline`) - every unserved feature and layer
shape is named in the text a user reads, a served one yields "" (the per-op attention chain naming every gemma generation's
attention); the MoE names among them (the
router shapes the routed block's kernels do not serve, the expert and slot counts past their
reach; gpt-oss's router bias, biased stacks, clamped swiglu and softmax-weight gate are served,
a biased stack feeding a K-quant down plane is named by layer, and the attention sinks are named
only while no sink seat is installed or the head size is not 64) and the MoE layer helpers (`layer_is_moe`, `resident_dense_width`: a layer routes only
past the dense lead with all three expert planes, and its dense width is the shared expert's);
plus the KV mirror's binding cap (`resident_binding_ctx`) on a hybrid shell whose layer 0 is
recurrent, its dense twin, the dense twin with every fourth layer sharing the K/V below it (the cap
is one binding over the widest layer's row on all three - a slab holds whole layers, so the layer
count and a sharer move nothing), and a shell with
no attention layer; a block codec's byte cap on a device that binds four times it (an f16 mirror
taking the wider range the control); and the cap a block codec's f16 shadow puts on a region's
context (`resident_shadow_ctx`: one binding of the widest K/V row's halves, the same under q8_0
and tq4, halved by a row twice as wide, none under a scalar codec). `test_resident_ring_mirror` holds the rings' plan arithmetic on a gemma-4-26B shell at 32768 x 4 - the ring rows,
the ring bytes, the ringed mirror past seven times under the flat one, and the 26B UD-IQ3_XXS's flat 32768 x 4 plan (39949
MB) less the flat mirror plus the ringed one under a 32 GB card's plan room (28527 MB), `test_trim_lane_ring_pin` the ring pin outside a trimmed vulkan lane's identity (the identity
the same under either pin, the plan's mirror not, and a trimmed shell's `.dlim` mapped by name under the route off declining by
name with the ring pin among the knobs the identity leaves out - a planar shell mapping with no decline the control), and `test_kv_ring_floor_check` the session's ring floor (`kv_ring_floor_check`: a call at or
past it continues, one under it panics, a start at 0 clears it and the same call then continues).
`test_vulkan_tower_route_before_a_device` holds the audio fronts' route (`vulkan_tower_front_route`)
answering off before a device is up, where the scratch sizing asks it ahead of the arm (skips with
a device up in the process, and without dasVulkan). The Metal serving gates ride the
same synthetic shells: `test_kq_fmt_one_row_gemv` holds the Metal prefill's one-row GEMV predicate (q8 at any width, a served superblock format - the lut-form IQ3_S among them - at whole superblocks and not at a partial one, a per-32 format at whole 32-blocks and not at a partial one, an mx4 plane at none; Q4_0 repacks to superblocks and has it); `test_peel_shape_arms` holds the token command's peel off the NextN-headed, hyper-connection and recurrent shapes one at a time on dense shells, a per-layer-embedding shell fitting it (`rdec_peel_shape_why`), and the device-home tick's segmented-window predicate (`gpu_device_prefill_segments_`) false with no driver armed; `test_moe_metal_expert_formats` sweeps `moe_metal_ok` over the seventeen
expert plane formats the routed block serves and the one tag it declines (mx4), one
mixed-format model (an mx4-tagged down plane declining it, a lut-form IQ3_S one served), the native-MXFP4 stacks under the plain routed gate (served; the repacked planes
and gpt-oss's gate over q8 stacks declining), and the drift guard walking every `KqFmt` member - the kernel roster
(`moe_fmt_metal_served`) and the site-alignment gate (`moe_site_ok`) name ONE set, which is the
only case that isolates the roster, since no format one admits the other refuses;
`test_moe_metal_expert_count` pins the select stamps' ladder at 256 and 512 experts admitted,
513 declined, the count alone deciding it; `test_mm_tile_widths_64_dense_width` and
`test_decode_shape_dense_width` hold `has_dense_ffn`'s guard - a routed model with no dense lead
carries an off-lattice dense width past `mm_tile_widths_64_ok` and past `decode_shape_decline`
on both the decode and the batch needs mask, and the same width on a dense twin declines
both. `test_perop_dense_ffn_mixed_forms` holds the per-op FFN rail's admission (`perop_dense_ffn_decline`) on a shell:
a q51 gate beside a k4 up and one form both ride it, an mx4 up is named by its site and `KqFmt` member. `test_metal_fmt_decline_names_site` holds the Metal plane-format declines' line on shells: a dense plane in a format no Metal kernel reads named by layer, site and member (`kq_load_gpu_unserved`, the token table and the classifier too) while `decode_shape_decline` still answers `kquant_native`, a routed stack's under `graph` (`moe_fmts_unserved`), the line `metal_fmt_decline_line` logs for each, a served shell and a decline no format decided naming nothing; `test_metal_graph_decline_names_site` the same for a sigmoid-gated shared expert's and a deltanet plane's `graph` declines (`shexp_fmts_unserved`, `dn_fmts_unserved`, the window's beta/alpha form `dn_ba_window_fmts_unserved`); and `test_metal_decline_panic_names_site`, on a das_metal build, the required-mode panic of `decline` carrying the same words, a `shape` decline panicking with its reason alone the control. `test_plan_room` is the GPU plan's room arithmetic - the tier cap less headroom, capped by
the OS's room where the OS answers. `test_resident_hot_slots` is the hot expert pool's slot rule
(`resident_hot_slots`): 64 slots where they fit beside the whole mirror, up to 32 out of the mirror's
context where they do not, the mirror's yield never past the room a mirror at its floor leaves and never taking away the
slots that fit beside the mirror (31 fit beside it with the floor's room past the plan: 31), a count asked served from
that room, no pool under four slots or on a plan whose whole mirror is already past its room; its second subtest holds the pool's
share of the plan (`resident_hot_fit`): the chain's scratch is taken off the room before the slots are
counted and charged only with a pool, a first plan's cap holds the count. `test_resident_arm_floor` is the
arming floor (`resident_arm_floor`): the built-in 4096, the env floor, a pin under either as the floor, and a
pin asked whole (`strict`) as the floor whatever its size; its host-layers subtest holds the count a plain MoE's
plan moves to the host (`resident_host_layers_fit`: the fewest first layers whose expert stacks leave for the
rest to fit, -1 where none makes it fit), the pool's floor yielding to a strict pin (`resident_hot_floor_ctx`),
and the binding range over the regions bounding a strict context too (`resident_region_ctx`: four regions on one
binding's 196608 positions hold 49152 each, under the 131072 asked, which the plan declines rather than shortens).
`test_resident_region_ctx` is a mirror region's share of its
side's one binding: the whole of it at one region, a quarter at four, the session's own context
where that is shorter, and one region for a count under one. `test_resident_fit_ctx` is the context
a plan past its room shortens to (`resident_fit_ctx`): the room past the weights, the pool and the
staged rows over the mirror's and a block codec's shadow's bytes a position, seven eighths of it -
the shadow charged whole at the asked context holds fewer positions (the control) - and the same
over the mirror alone under a scalar codec, 0 where the weights pass the room; its second subtest the rings - their bytes off the
room, the rest of the mirror over the context (the rings charged a position at the asked context hold fewer, the control). `test_resident_mirror_codec_ask` is
the mirror codec the plan arms, on shells: a block codec's geometry (`kv_codec_geometry_why`: a
96-wide head holds q8_0 and tq4 names it, a 48-wide head q8_0 names, a scalar codec holds any),
the ask served at the codec the model holds - a tq4 ask on a 96-wide head or a Q/K/V-biased attention at q8_0, a
48-wide head at f16 - and f16 for an f32 ask
(`resident_mirror_dtype_of`, under a pin the cell puts back; skips where `DASLLAMA_VK_KV32=1`
arms f32 over it), the reset lifting the ask, a codec's spelling in any case with an unknown one
reading f16 (`kv_dtype_named`), and its ordinal across the tier's seam with a bad one panicking by
name (`kv_dtype_of`). `test_kv_codec_serving_rule` holds the serving rule itself on shells (`kv_codec_served` /
`kv_codec_serving_why`): a 32-wide head without a bias serves tq4 as asked, a Q/K/V bias steps tq4 down to q8_0 naming
the bias while q8_0 and f16 serve it, a 96-wide head steps tq4 down to q8_0 naming the rotation, a 48-wide head lands
both block codecs at f16. `test_mtp_seat_owner` holds the
speculative round's seat ownership on a Model shell under two fake decode overrides: with the
owner refusing the model, and with seats registered under no owner, `mtp_spec_eval` and
`mtp_spec_round` open the CPU round (its opening refuses the session's parked token, the witness)
and neither reaches a seat; with the owner claiming it, the round reads the verify-rows cap and
the cold seat's reason (the probe seats the cell registers) before it opens. `test_land_pick` holds the pick a round or a driver lands in a logits row's place
(`land_pick`): the next `sample_` returns it once and reads the logits row again after, a second
landing before that `sample_` refuses and leaves the first standing, and the CPU round's opening
refuses a session whose landed pick nobody sampled. `test_mtp_round_panics` holds the round's
fail-closed exits on a NextN Model shell under fake seats: a pick ask beside a sampled walk panics
naming both before any draft, and a rollback seat that declines after its verify seat served panics
naming the override (the verify asked once, the rollback once). `test_mtp_verify_snapshot` holds
`mtp_verify_two`'s snapshot on the same shell: a recurrent model with a serving verify seat and no
rollback seat takes the CPU snapshot before the seat (its conv and state buffers sized to the
shell's history), a non-recurrent one skips it and its reject never reaches a rollback seat, and a
rollback seat that serves takes none - the reject rolling back through it, committing the token
alone and re-seeding the carry from row 0's hidden at the token's row.

`test_gpu_resident_hybrid.das` - stocked suite, `-jit` only; the whole-model resident driver on a
deltanet hybrid under `DASLLAMA_GPU=1`. Each fixture is a row in `../performance/model_specs.das`:
the Q8 carrier, the K-quant twin, the mixed twin, and the NextN-headed MTP twin.

The Q8 carrier is `Qwen3.5-0.8B-Q8_0.gguf`. The resident window chain prefills it - the recurrent
layers through conv + chunked scan on device state, the gated partial-rope attention over the
device K/V mirror - and the resident decode advances the recurrent layers on device. The gate is
the forced-feed logits-tolerance form: the logits sit within the 4% deltanet bar of the all-CPU
chain (the model dropped off the device) after the prefill and at every step. The control is the
one-step-off compare - each step's resident logits against the CPU chain's PREVIOUS step - which
must EXCEED the bar. Counter witnesses hold that the resident tier armed and that the prefill
served on the device. The file pins a 512-row prompt window (`set_pf_window_rows_pin` at init: the
cells read the seam, not the window's width). The cells: one window (a 40-row prompt), two windows
(a prompt one window plus 88 rows long), one row past one window (the chain shortens the preceding window, so the last
one still carries the conv taps) and two tokens (a prompt shorter than the conv taps: the conv
history ring's leading rows are zero). The file skips without the model or the armed tier.

The K-quant twin is `Qwen3.5-0.8B-Q4_K_M.gguf`, minted from the Q8_0 by the recipe its row
carries. It runs the one-window and two-window cells with the deltanet qkv (q6_K), z (q4_K) and
out (q4_K) planes in their file formats on the driver, and asserts the loader kept them off Q8_0. It
holds a 6% bar - 10% where the prefill takes the quant feed. The quant feed is the case where no
coopmat tile serves the planes, so the prefill's activations ride Q8_K. The K-quant chain's
device-vs-CPU noise runs near double the Q8 file's, flat across steps.

The mixed twin is `Qwen3.5-0.8B-Q4_K_M-q8out.gguf`, the same mint with the deltanet out plane at
Q8_0 - the shape the published Unsloth dynamic quants take. Its recurrent layers decide two feeds
separately - the x feed (the rows the qkv and z GEMMs read) from the K-quant qkv/z planes, the o
feed (the rows the out GEMM reads) from the Q8_0 out plane - so on the KHR arm x rides the f16
feed while o falls to the Q8_0 requant. It runs the same one-window and two-window cells with the
same `DASLLAMA_VK_KV32=1` skip, on an 8% bar on the f16 feed (the 10% quant-feed bar off it): its
CPU chain quantizes the activations twice a recurrent layer, so the device and the CPU chain round
on either side of the file's f32 chain, and one step reads 0.91 of a 12.3 max logit apart with the
device's pick the f32 chain's. Both cells therefore carry the same-or-better witness
(`f32_chain_witness`): the file loaded again at `QuantMode.fp32`, its device state dropped, the
same tokens through that f32 chain on the CPU, and the device's summed distance from it held within
1.15 of the CPU q8 chain's own (reads 0.91 and 0.93; the f32 rows one step off 21 and 25, the
control), every chain's picks decoded in the log (`chain_distances`, `_resident_feed.das`, the
three-chain sum the codec cells share). Every K-quant cell - the twin's two and the
mixed twin's two - carries the routing witness: the prefill's Q8_K requant census count, read
through `vk_kernel_coverage_of("cls_q8k_rq_spv")`, below the recurrent-layer count on the f16
feed and at or above it off the feed. The mixed twin is the fixture that discriminates: a
per-layer feed decision would send qkv/z to the sdot4 tile for the out plane's sake, and the
witness reds it there. The witness reads `NO_CENSUS` (`census`, `_resident_feed.das`) and stands
down in a build with no vulkan module.

Every fixture makes its session on the mirror codec the box arms. Under `DASLLAMA_VK_KV32=1` the
Q8 cells run on the f32 mirrors and the K-quant cells skip - their bar is calibrated on the f16
mirror. Under `DASLLAMA_COOPMAT=mm` this file is the KHR arm's end-to-end gate: the K-quant
planes prefill on the KHR kq tile (mode 3), and the 6% bar holds there too.
`DASLLAMA_COOPMAT=sdot4` names the integer dot tile and forces the quant feed, so the 10% bar
applies there.

The NextN-headed twin is `Qwen3.5-0.8B-MTP-Q8_0.gguf`, the file whose head makes the driver land
the speculative carry; every MTP cell loads it through `load_mtp_carrier`, which pins the resident
route and the region count for the load and puts both levers back through their getters.
`test_gpu_resident_hybrid_mtp_carry` holds the carry - `mtp_h`, the post-final-norm hidden, and
`mtp_h_pos1`, its row's position plus one - with speculation on, after the resident prefill and
after each of four resident steps against the CPU chain's: the watermark exact, the hidden within
the 4% deltanet bar, and the one-step-off control past it (`rows_within_control` in
`_resident_feed.das`, the widest miss and the tightest control logged).
`test_gpu_resident_hybrid_mtp_carry_batched` holds the same for two sessions in two regions, both
batched through the N-row command at every step (the step counter witnesses it), each row's carry
against its session alone on the CPU chain. `test_gpu_resident_hybrid_mtp_carry_gate` runs the
same prefill and steps with speculation off: the resident driver lands no carry (the watermark 0
after the prefill and every step) while the same driver with it on lands one, and the spec-off
logits sit within the deltanet bar of the spec-on run's at every row with the one-step-off control
past it. `test_gpu_resident_hybrid_peel` holds the short window's peel on the Q8 carrier and on the NextN-headed twin with speculation off (with it on, a two-token prompt takes the chain, which warms the head's rows): after a 40-row opening, tails of 1, 2, 5 and 8 rows as one prefill call peel through the token commands - the in-order rows command past one row, every recurrent layer stepping the rows in the region's slot - (a peel and a prefill counted, `dn_pos` at the tail's end) and with four fed steps read within the split bar of the same tail stepped a row at a time on a second session, the row one step off the control; a tail one row past the per-row planes takes the chain, no peel counted; every call served; `test_gpu_resident_hybrid_mtp_draft` holds the NextN
draft the driver's head serves: after a 40-token resident prefill with the speculative warm on (so
the head's host rows the draft hydrates hold history), one draft at the prompt's last row through
the seat against the CPU `forward_mtp` on the same token, carry and row - the seat unset on the tier
for the CPU side (`unset_rdec_draft`, put back after) - the logits and the head's hidden within the
4% deltanet bar, the CPU draft one row back (of the token the prompt holds there) past it, the
device pick the argmax of the logits it landed and, tie-aware (`pick_agrees`: the CPU argmax, or a
pick whose CPU logit sits within the bar of the argmax's), the CPU draft's, the head's drafts
counter up by the one device draft alone, and every call served; its second cell runs two
speculative rounds, the first with the seat unset and the second with it back, the round's draft
count and the head's counter saying which side drafted each (both rounds verify through the device
seat). `test_gpu_resident_hybrid_mtp_draft_upload` writes two head rows on the host through the
unset seat (the device's row count stays below them, the host's reaches past), drafts on the device
two rows past the device's (the host's two go up with the draft: the device's count reaches past
the drafted row, the host's stops below it), brings the drafted row down on the next pass (the
host's count past it, the host K row moved) and holds the device pick tie-aware against the CPU
draft's logits over the same rows. `test_gpu_resident_hybrid_mtp_draft_suppressed` drafts once on
the device, then re-arms the driver (`gpu_slot_capture`, `moe_gpu_drop_model`,
`gpu_slot_rearm`) with that pick among the model's suppressed ids and drafts again over the same
prompt: the draft's logit for the id sits at the suppression floor (`SUPPRESSED_LOGIT`), the pick is
another id and the argmax of the logits it landed (the first draft's pick against its own logits the control).
`test_gpu_resident_hybrid_mtp_verify` holds the round's same-slab verify: after the same 40-token
prefill (the host holding the warm's rows below the prompt's last), one device draft at the
prompt's last row (the device's rows reaching past it, the host's stopping below it), a pass
through the unset seat that brings the draft's K row down (the host row moved), then one round
with the draft and the verify on the device (their counters, every call served, no pick asked so
every row's logits land and no pick is published), its two verify rows - the token and the draft -
against two one-row resident steps of the same tokens on a fresh session: each row's logits and
post-norm hidden within twice the deltanet bar of the step's (the verify rows ride the N-row
command's tiles, the step the GEMVs, and the two part by the tiles' rounding on a card), the
argmax the same on both sides, the other row's step as the control past the bar; every row's
device pick the argmax of its landed logits, the region's rows at the
session's position (two past the prompt on an accept, one on a reject), the head's rows staying on
the device (the host's count below the draft's row again) until the next pass brings them down,
and the re-warm's K row at the prompt's last row the draft command's but for a hundredth of its
bytes (a quant on a rounding edge).
`test_gpu_resident_hybrid_mtp_verify_asked` runs six rounds under the caller's pick ask on one
session and six with the logits landing on another from the same prompt - every round's draft,
accept, position and the token `sample_` takes next (the published pick, or the landed row's
argmax) equal, the asked session's logits row not the round's.
`test_gpu_resident_hybrid_mtp_verify_reject` forces the reject
(`set_mtp_force_reject_every(1)`): the reject rolls back on the device (the rollback counter up by
one, the snapshot buffers never sized), the session keeps verify row 0's logits and hidden bit for
bit, the region's rows, its head rows, `dn_pos` and the carry's watermark sit at the token's row,
the next round verifies on the device again with the region's rows at the session's position, and
then the rolled-back deltanet state and conv history (read back to the host) and the next plain
step's logits are bit for bit a fresh session's after one row-at-a-time step of the same token,
with the fresh session stepped through both verify rows as the control that misses.
`test_gpu_resident_hybrid_mtp_verify_reject_asked` forces the reject under the caller's pick ask:
no logits land (the session's row stays the prefill's), verify row 0's device pick is published for
`sample_` (returned once and consumed), the carry re-seeds from row 0's hidden at the token's row,
and the pick equals the one-row step's argmax over the same token on a fresh session.
The head's prompt warm on the window chain: `test_gpu_resident_hybrid_mtp_xb_poison` fills a
session's host residual rows with NaN before the same 40-token resident prefill and holds the
device draft at the prompt's last row bit for bit the clean prefill's (logits, hidden and pick;
the carry bit for bit too, every logit finite, the region's head rows at the prompt's end less one),
the draft one row back as the control; `test_gpu_resident_hybrid_mtp_head_warm` reads the head's
warmed K rows back from the host cache after the resident prefill and holds each within the
8% head-row bar (of the CPU row's max: twice the deltanet bar, since the row is the f16-stored
projection of a hidden row already within the deltanet bar) of the CPU warm's row on the same prompt
after the model drop, the CPU row one down missing by two bars as the control, the widest miss and
the tightest control logged - over the 40-token prompt (rows [0, 39)) and, its second subtest, over
a prompt one window plus 88 rows long (rows [0, 599): the second window's carry row is its row 0 and
the head's rope sits a row behind the trunk's); `test_gpu_resident_hybrid_mtp_rounds`
runs six speculative rounds on
the device (their draft and verify counters, every call served) and six on the CPU from the same
prompt and fed tokens, every round's draft token tie-aware against the CPU round's draft logits
(the head drafted once ahead of each CPU round from the same carry, the carry put back) and, while
the drafts agree, the accept and position equal - or, where the accepts part, the draft a near-tie of the CPU verify row 0's
argmax (`accepts_agree`) - all logged decoded; a near-tie that flips a draft or an accept parts the sessions, and the rounds
after it are logged, not compared - the
region's head rows at the session's position less one with the host's count short of them (the
rounds' rows stay on the device), then a seventh round whose draft the unset seat declines - the
CPU head over the head rows the pass hydrates from the device, the verify on the device - against
the seventh CPU round (the draft and the accept tie-aware), the hydrated host rows within the
head-row bar with the two-bar one-down control, and an eighth declined-draft round against a CPU
round over the head's host rows poisoned with NaN as the control (another draft, or non-finite
rows). The gates around the round: `test_gpu_resident_hybrid_mtp_nonfinite` fills the carry with
NaN before a device draft and holds that the draft panics naming the row it landed non-finite logits
on, the drafts counter unmoved; `test_gpu_resident_hybrid_mtp_pass_arms` shapes a session for each
of the draft and verify seats' pass arms and holds the reason counted (`pass_count` over
`gpu_cpu_passes_`) and the device verify counter unmoved - an f32 session under the f16 mirror
(codec: the prefill, the draft and the verify; skips under `DASLLAMA_VK_KV32=1`), a session the
device never homed, its prompt read on the CPU rails under the server's prefill pin
(`set_resident_prefill_allowed(false)`, the pass counted as pinned_off, no mirror region, the
recurrent state and the carry at the prompt's end) (unhomed: the draft and the verify), and a
round five rows past the region's rows, the rows served on the CPU rails as a continuation under
the prefill pin (pinned_off; the region's rows still at the prompt, the recurrent state and
the carry five past it) (gap - the draft, which has no gap arm, serves) - each round completing
on the CPU rails with no panic, the CPU verify over the recurrent state the CPU rails hold; the
cap arm is not shaped (a prompt past the region's cap);
`test_gpu_resident_hybrid_mtp_rewind_refused` shapes a round off the recurrent state's position
by hand: the draft serves, the verify passes as rewind, and the CPU rails then refuse the rewind
by name with both positions in the text, since the recurrent state is forward-only on every
rail; `test_gpu_resident_hybrid_mtp_head_off`
loads the twin with the draft seat unset so the plan keeps the head off the device (the seat put
back after the load, `rdec_verify_rows` reading 0, the chain warming no head row) and runs one
round: the verify passes as verify_rows, the CPU verify's two-row window passes the resident prefill
as verify_window (`mtp_verify_window_active`), no panic, the driver still armed, and the CPU
verify's row 0 within the deltanet bar of the CPU chain's step of the same token over the rows the
decline brought down, the prompt's logits as the control. `test_gpu_resident_hybrid_mtp_depth8` loads the
twin under the depth-8 knob (`set_mtp_depth`, cleared after): the driver prepares nine verify rows
(`rdec_verify_rows`), past the N-column leaves' eight, and the greedy walk on it (`generate_mtp_greedy`,
32 reply tokens on the counting prompt) emits the plain decode token for token, its drafts and verifies on
the device and none passed as `verify_rows`; loaded at depth 1 the driver prepares two and drafts one token a verify (the
control for the drafts-over-verifies bound). `test_gpu_resident_hybrid_sampled_selection` runs a sampled stream (temp 0.7,
top-p 0.9, seeded) at depth 2 twice over the counting prompt, the rows landed and the selection landed (`sampled_stream` of
`_resident_feed.das`, `set_select_landing` put back): the rows-landed stream asks no selection, the selection serves every
step of the other with no late landing, and the two agree token for token with the same drafts accepted - the streams share
the device's logits and the seed, and `test_sampling_landed.das` holds the landed draw's equivalence to the row draw, so the
exact compare is the right form; a plain sampled stream with the round off first lands a selection a step.
`test_gpu_resident_hybrid_greedy_chain` runs the greedy chain (`../ARCHITECTURE_GPU_VULKAN_LANDING.md#vk-greedy-chain`,
`greedy_chain_cell` in `_resident_feed.das`; the llama file runs it on Llama-3.2-1B Q8_0, and the CUDA llama and qwen35 files
on the CUDA home): 24 steps fed the argmax and 24 fed a
token off it every third step, each with the chain off and pinned on (the armed home's pin - `vk_rdec_chain_mode_set` or
`cud_rdec_chain_mode_set`, put back) - the chained walk's
tokens and every step's logits bit for bit the plain walk's, every step after the first taken on the argmax walk, and on the
other every off-pick step discarded (the recurrent states put back from the snapshot) and every other step taken
(the home's chain stats).

`test_gpu_resident_hybrid_mtp_block_codec` runs the head over a block-codec mirror
(`ARCHITECTURE_GPU_SHARED_ATTN.md#vk-kv-block-codecs`), calling the cells above under a
`set_gpu_kv_dtype` pin it puts back, their sessions on the armed codec (`load_mtp_carrier` asserts
the mirror armed at the codec asked): under q8_0 the draft, the verify, the warmed head rows and the
six rounds, on the f16 bars; under tq4 the verify and the forced reject, which hold the driver to
its own one-row steps bit for bit (a 4-bit head row sits a quant step from the CPU's, past the
head-row bar). The rounds cell compares the hydrated head rows only up to the round whose draft a
near-tie flipped: past it the two sessions hold different tokens.

`test_gpu_resident_hybrid_gated_block_codecs` holds the gated attention under a block-codec mirror
to the CPU chain, on the Q8 carrier through `_kv_codec_cells.das`: the q8_0 mirror in the
forced-feed form at a 6% bar (reads 0.21-0.31 of a 10.7-12.3 max logit), and the tq4 mirror in the
distance form - under tq4 the attention pass runs ungated and the un-rotation applies the head
gate, which only a compare against the CPU chain sees (the hybrid's other tq4 cells hold the
driver to itself): the device's summed distance from the f16 chain within 1.15 of the CPU tq4
chain's own (reads 1.08, the control 9.2), the two tq4 chains within 0.8 of that distance of each
other (reads 0.63, the control 9.3). A gate applied twice reads 9.2 times the CPU chain's distance.

The file's model-free cells: `test_kernel_census_by_name` holds that the census accessor panics on a
kernel name nothing seeded, so a misspelt key cannot read as a zero count; `test_row_bar_nan_reference`
holds the resident rigs' bar (`row_bar`, `_resident_feed.das`: rel of the CPU row's largest |logit|
with NaNs and the suppressed ids' floor left out - `live_maxabs`, `_model_tier.das`) - a NaN in the reference row scales
no bar and reads past it, while a bar scaled by `logits_maxabs` is infinite over the NaN and the same
row holds it (the control); `test_row_bars_red_a_nan` runs the rigs' two row bars -
`rows_within_control` and `tq4_within`, both in `_resident_feed.das` - on synthetic rows through
`probe_fails` (`_compares.das`: a test handle that counts failed asserts instead of failing the cell):
a NaN in the device row or the reference row reds each, the clean rows hold each (the control), a
NaN reference reds with no control row, and the tq4 share has teeth where it sits - a device row at
0.8 of the one-off row's distance holds, one at 0.9 reds.

`test_gpu_resident_qwen2.das` - stocked suite, `-jit` only; the whole-model resident driver on a qwen2
(Qwen2.5-0.5B-Instruct-Q8_0, `DASLLAMA_GPU=1`): the q/k/v projection bias folded into the rope
stage on the device - the hybrid file's forced-feed logits-tolerance form (its K-quant 6% bar,
the one-step-off control) at one window and two windows, with the arm witnesses that the model
carries the bias and the driver armed on it; skips without the model or the armed tier. Its IQ4_NL mint
(`Qwen2.5-0.5B-Instruct-IQ4_NL-local.gguf`, a recipe row in `../performance/model_specs.das`) runs the same two cells with
every dense plane's iq4nl32 tag asserted (`dense_planes_tagged`, `_resident_feed.das`) and every call served. The same
carrier holds the K/V mirror's block codecs (`ARCHITECTURE_GPU_SHARED_ATTN.md#vk-kv-block-codecs`)
through the rig of `_kv_codec_cells.das`, whose loader (`with_codec_model`) pins the codec through
`set_gpu_kv_dtype` for the load, puts back the pin it found (or lifts it where nobody had asked),
asserts the mirror armed at the codec asked, and skips where `DASLLAMA_VK_KV32=1` armed an f32
mirror. `test_gpu_resident_qwen2_q8kv` is the forced-feed form (`cell_codec_forced_feed`) on q8_0
sessions at a 10% bar (both chains quantize each query, each its own, so two roundings part), one window
and two; `test_gpu_resident_qwen2_tq4_steps_down` holds the serving rule on the carrier - a tq4 ask arms the q8_0 mirror
and the forced feed holds the q8_0 bar; `test_gpu_resident_qwen2_tq4kv` is the distance form (`cell_codec_distance`) on
Qwen3-0.6B Q8_0 (`CARRIER_TQ4`: the biased carrier serves no tq4), since no logit bar holds a 4-bit mirror on a 0.5B - three chains over one prompt and eight fed tokens (the
device at tq4, the CPU at tq4, the CPU at f16), every row asserted finite, the sums of the rows'
largest differences held as ratios: the device no further from the f16 chain than 1.15 of the CPU
codec chain's own distance (reads 0.95-1.01), with the f16 rows one step off past that same bound
as its control (reads 1.6-3.7), and the two codec chains within 0.8 of that distance of each other
(reads 0.20-0.61), with the CPU codec rows one step off past that bound as its control (reads
1.5-3.7); `test_gpu_resident_qwen2_codec_rows_up` is the same form with the prompt on the CPU
rails (the resident prefill pinned off, exactly one `pinned_off` pass asserted, the pin put back
as found), so the device's steps read rows the CPU wrote and the driver uploaded as the cache's
own bytes, on q8_0 and tq4; `test_gpu_resident_qwen2_codec_rows` (`cell_codec_stored_rows`) brings
the mirror down after a 40-token resident prefill and eight resident steps and holds layer 0's
stored bytes to the CPU cache's on both codecs - the K rows and the steps' V rows within 2% and 5%
of their bytes (read 0.2% and 1%: a quant on a rounding edge), the prefill's V rows within 0.6 of
the control's share (its GEMM reads f16 rows where the CPU reads Q8_0 blocks; reads 0.39 and 0.12
against 0.97), a row held to the next one the control (asserted past 0.2 of its bytes); the same
cell holds the plan's scratch charge for the codec over the f16 plan's - both shadow planes at the
context as halves and the window's staged float rows, to the byte. The distance form also holds
the device no nearer the f16 chain than half the CPU codec chain's distance (`CODEC_APPLIED_REL`):
a driver that served its f16 rows in the codec's place would read near zero.
`test_gpu_resident_codec_llama_k.das` and `test_gpu_resident_codec_llama_iq.das` - stocked suite,
`-jit` only; the same rig on the other two weight formats a format's parity owes - Llama-3.2-1B
Q4_K_M (a K-quant) and Llama-3.2-1B IQ4_XS (a grid format): the q8_0 mirror in the forced-feed
form at a 10% bar (reads 0.25-0.42 of an 8.4-9.4 max logit) and the tq4 mirror in the distance
form, a 40-token prompt and eight fed tokens each - the device's distance from the f16 chain
within 1.15 of the CPU codec chain's (reads 0.99 and 1.00, the control 4.7 and 5.0), and the two
codec chains within 1.4 of that distance of each other (reads 1.02 and 1.13, the control 4.9 and
5.1): tq4 moves these carriers' logits a point a row, three times the kernels' rounding, so the
two chains part as two independent quantizations do, where the qwen2 carrier's six points a row
keep them at 0.61; the same skips.
`test_gpu_resident_continuation.das` - stocked suite, `-jit` only, `DASLLAMA_GPU=1`; a prompt continued on the whole-model
driver: its second half as a continuation window from the first half's position, and the same half from a checkpoint a fresh
session adopts after the first session stepped past it, each against the whole prompt through one window, the prompt one
token short the control, the window chain's prefill count the witness that the device served every call; Qwen3.5-0.8B-Q4_K_M
(the recurrent state rides the row) at the 5% bar; Qwen3.8-Flash-Next UD-IQ4_XS (the large tier, `DASLLAMA_PARITY_FULL=1`)
holds its n-gram conv ring after a continuation window row for row against the CPU chain's (`rdec_peek_snapshot` reads the
device's), a zeroed ring the control, and its logits at the wide 50% bar, since the pool's fill and the meet's split flip a
near-tie expert run to run. Skips without the file, without the armed tier, and under `DASLLAMA_VK_KV32=1`.

`test_gpu_resident_long_window.das` - stocked suite, `-jit` only, `DASLLAMA_GPU=1`; Qwen3.5-0.8B-Q4_K_M: a 5000-token prompt
against the CPU chain's last row at the 10% bar, the prompt one token short the control, the window chain's prefill count
holding that the device served the prompt, at two prompt windows pinned in process (`set_pf_window_rows_pin`, the pin read
back as the witness): the wide window the driver picks first (`PF_ROWS_WIDE`, 4096), which is DN_WINDOW itself and runs
uncut, and 5120 rows, which the recurrent window's cut at DN_WINDOW bounds - a window past the planes read 30-72% off the
chain. Every other resident cell sizes its prompts against `PF_WINDOW_ROWS` (2048), which `load_resident` pins where no file
pinned a window, since the driver's own pick would run a two-window prompt as one. Skips without the file and without
the armed tier.

`test_parity_dump.das` - stocked suite, `-jit` only, `DASLLAMA_GPU=1`; the parity dump's round trip
(`harness/_parity_dump.das`, the tool `harness/parity_dump.das`) on Qwen3.5-0.8B-MTP: the device arm's
forced-feed run written to a file (the prompt, the feed, the routing tape, every row), read back bit for
bit, the CPU arm run on that file's prompt, feed and tape after the device drop and written, and the two
files compared under the regions files' bar with the one-step-off control - the in-process parity through
the files a model past the box has to use (every row's argmax decoded a side in the log); a logit three bars off reds its row alone, a reference with
another feed is refused. The file is the magic and version words, then the record's byte count and the binary serializer's bytes;
`test_parity_dump_refuses_foreign_files` (model-free) writes a file with another magic word, one with another version word and
one with this build's and reads each refusal by name (`parity_dump_refusal`), a missing file refused as unreadable.
`test_gpu_resident_hc_micro.das` - stocked suite, `-jit` only, `DASLLAMA_GPU=1`; the hyper-connection
chain's k-deep speculative round on a carrier the box holds whole: Lynxpda's micro-qwen4exp (random
weights, hc 4, the PLE 3-gram side input, 8 experts top-2, a NextN head; the trunk and the published head are the
`performance/model_specs.das` row's file and companion, and the `-hcmixer` head twin - the head mixer the loader asks of a
hyper-connection head, copied off the shard's own `output_hc_*` - is minted by `harness/micro_qwen4exp_hcmixer.py`).
Random weights say nothing about text, so the cells read counters and the n-gram conv
ring: at depth 3 (`set_mtp_depth`, the head path and the depth put back through their pins) the hc verify rail serves four
rows on the device with no verify passed to the CPU, the plan picks the wide prompt window with no pin (`PF_ROWS_WIDE`) and
`chunk_defaults` reads its idle chunk as the wider of the GPU floor and that window,
and a round forced to reject at row 0 rolls the ring back - the three rejected rows' slots hold their
pre-round bytes bit for bit (2.27 off without the restore), the committed row's slot its row (a plain step after it moves
neither the verify nor the rollback counter: the no-route control); and a
sampled stream through the selection on the hc chain decodes the rows-landed stream token for token, both streams' ids logged
(a random row's flat nucleus reaches past the survivors, so every landing takes the rows late - asserted, late landings
equal to selections: the plumbing's witness, not the selection's); the chain cut (`set_mtp_chain_min_p`, put back through
`get_mtp_chain_min_p`): at depth 3 four greedy rounds draft three rows each with the cut at 0 and one each at 0.99, since
random weights hold every draft's probability under the floor; and a child under `DASLLAMA_VK_XFERQ=0` (`_resident_load_root.das step`)
loads, prefills and steps the host-experts form on the compute timeline alone - the timelines stand without a
transfer family (`ARCHITECTURE_GPU_VULKAN_LANDING.md#logits-transfer-queue`).
`test_gpu_resident_hc.das` - stocked suite, `-jit` only; the whole-model resident driver on the hyper-connection
carrier Qwen3.8-Flash-Next UD-IQ4_XS (three shards, the large tier, `DASLLAMA_GPU=1`): the wide residual's mixer
seams, the n-gram side input and the deltanet sigmoid out-gate on the device, the routed experts summed on the host
between the segments of the window chain and of the split token command - the hybrid file's forced-feed
logits-tolerance form at the routed block's 20% bar (test_gpu_resident_moe's reading) with the one-step-off
control, a 48-token prompt and six fed steps, the CPU arm routing as the device arm did through the decode pick
tape (`ARCHITECTURE_ENGINE_MECHANISMS.md#moe-pick-tape`: a near-tie of 512 experts flips on kernel-order noise and one
flipped expert of ten moves the row past any bar; the cell asserts the tape aligned select for select, the
prompt's rows first, and logs the CPU's own decode picks off it); the cell asserts the window chain served the
prompt (`gpu_resident_prefills_` up by one) and the driver stayed armed; where the plan armed the hot
expert pool (`gpu_resident_hot_slots`) it asserts every decode pick of every routed layer went through the
pool, experts were uploaded, and the device served some of the picks and the host the rest, so the bar holds
the sum of both shares, and - the prompt past the pool's 32-row window floor - that the tier's expert chain took
every routed layer's window and served some of its slot rows through the expert tiles over the pool's planes, the
host the rest. Skips without the shards
(`DASLLAMA_PARITY_FULL=1` admits them), without the armed tier, when interpreted, and under `DASLLAMA_VK_KV32=1`.
With the split head shard beside the model (`DASLLAMA_MTP_HEAD`, `ARCHITECTURE_GPU_VULKAN_HC.md#hc-draft-head`,
`#hc-verify-rows`) two more cells: the draft cell runs one NextN draft through the resident head on the chain
against the CPU `forward_mtp` on the same token, wide carry and row - the head's one select pinned by the pick
tape, the logits and the wide carry each within their bar with the one-row-back control, and every call served
(`served_every_call`); the verify cell, with the same served assert, runs
one speculative round on the device (the draft, the two-row verify, the accept or the rollback) and holds its
two rows to the split command's own one-row steps on the same picks (`moe_pick_tape_lane`, a lane replay of the
rows-form tape) at the split bars, and to the CPU's one-row steps at the wide bars, the other row as each
row's control. Both also skip where the head shard is not beside the model. `test_gpu_resident_hc_short_window` holds a two-token prompt on the trunk to the window chain (the peel declines the hyper-connection chain); the round cell also holds the N-row command at twelve rows on the trunk - the routed slot planes hold 128 picks, the trunk picks ten a row.
`test_gpu_resident_llama.das` - stocked suite, `-jit` only; the whole-model resident driver on the
llama family (Llama-3.2-1B Q8_0 and its Q4_0 requant `Llama-3.2-1B-Instruct-Q4_0-local.gguf`, whose
recipe row is `../performance/model_specs.das`'s, Llama-3.2-3B Q8_0, Llama-3.1-8B Q4_K_M,
`DASLLAMA_GPU=1`; the Q4_0 file's planes run the kq batch arm off the f16 feed and the cm2 tiles on it; the Q5_1 mint
`Llama-3.2-1B-Instruct-Q5_1-local.gguf`, its recipe row beside the Q4_0's, keeps every dense plane's q51 tag - asserted, the
window's GEMMs on the cm2 column the shape picks on the f16 feed and the per-32 batch tile off it - and runs the peel cell
below too): the
NORM rope, no q/k/v bias, no q/k norm, the tied classifier of the 3.2 files - the qwen2 file's
forced-feed form and bar at one window and two windows per carrier, with the arm witnesses that
the file is a llama with neither bias nor NEOX rope, and the pool's spin-window witness - the
GPU-served window in force while the driver is armed, the CPU window back after the drop; the 1B
also runs the batched bench row (`bench_tg_batched_rep`) with the driver homing two streams: a
row asking more streams than the device homes refuses by name and reads 0, and a row it homes
serves at a rate with every stream parked after it and no call passed to the CPU chain (a
one-stream row is no batch - the step census counts it a row at a time and the row refuses); the 1B
Q8_0 and Q4_K_M also run the short window's peel (`ARCHITECTURE_GPU_VULKAN_WINDOW.md#vk-prefill-peel`):
after a 40-row opening a tail of 1, 2, 5 and 8 rows as one call peels through the token commands
(`moe_gpu_resident_peels` up by one, one prefill counted) and, with four fed steps, reads within the
split bar of the same tail stepped a row at a time on a second session, the row one step off the
control, both sides' argmax rows decoded; a tail one row past the N-row command's rows takes the
chain, no peel counted, on the same bar, as does the eight-row tail with the peel's lever off
(`set_vulkan_prefill_peel`, put back as found) - the peel counter's control; the plan (`resident_plan_pick`) charges the per-row planes at `RDEC_NROW_MAX` rows. The forced-feed helpers it shares with the
other resident files live in `_resident_feed.das`. Skips without the model or the armed tier.
`test_gpu_resident_cuda_llama.das` - stocked suite, `-jit` only; the CUDA home's whole-model driver
(`DASLLAMA_GPU=1 DASLLAMA_CUDA=1`, the dense form: one Q8_0 arena, the f16 mirror, the token step as a
captured graph, the prefill as the window chain over the cm2 tiles - one session's rows, or several sessions' chunks as
the segmented window, which `test_gpu_resident_segments.das` holds on this home too) on Llama-3.2-1B Q8_0 in the llama file's
forced-feed form at 40 tokens (one window) and at a prompt of two windows (the file pins `PF_WINDOW_ROWS` as the
other resident files do, every load through `load_resident` at one region with the resident route pinned), with the
witnesses that the CUDA driver served every position, ran the windows the
prompt takes, replayed the captured graph past the first step and took the attention's unsplit form for every fed step
under the unsplit position and for none past it (the one-window cells' steps under `RD_UNSPLIT_POS`, the two-window
cells' past it; a device that splits no keys has no such form, and the cell then holds the count at zero), a CPU run
with the model off the device moving neither the position nor the window counter and holding no graph (their control);
the Q4_K_M file (the K-quant planes) and the Q5_1 mint (the per-32 planes) the same way at one window and two, the Q5_0
mint (the q5032 planes), the IQ4_XS file and the IQ4_NL mint (the codebook planes, their decode through the home's byte
permute) at one window, and Llama-3.2-3B Q8_0 (head 128: the window's attention on the h128 flash tile) at one window
and two; and the short window's peel through the N-row command on the CUDA driver
(`peel_cell` of `_resident_feed.das`, the Vulkan llama file's and the hybrid file's cell): on the Q8_0, Q4_K_M and Q5_1
carriers, after a 40-row opening, tails of 1, 2, 5 and 8 rows as one prefill call peel (a peel and a prefill counted)
within the split bar of the same tail stepped a row at a time, the row one step off the control; one row past the
per-row planes and the eight-row tail with the lever off take the window chain; and the greedy chain on the CUDA home
(`greedy_chain_cell`, the Vulkan llama file's cell, on the Q8_0 carrier: the chain pinned through `cud_rdec_chain_mode_set`,
its counts `cud_rdec_chain_stats`, each put back). Skips without `DASLLAMA_CUDA=1`, without
the armed tier, without the file, on a card whose room the carrier's plan does not fit (the decline named, as every
`cuda_forced_feed` cell skips), or, on the forced-feed cells, where the prompt window is not `PF_WINDOW_ROWS` rows.
`test_gpu_resident_cuda_codec.das` - stocked suite, `-jit` only: the same
driver under the block-codec mirrors: the codec rig's forced-feed form (`_kv_codec_cells`) under q8_0 on the 1B Q8_0 at
one window and two and on the Q4_K_M at one, at the Vulkan codec carriers' 10% bar, and the distance form under tq4 on
the Q4_K_M at the Vulkan llama codec file's bars; the same skips, and under `DASLLAMA_VK_KV32=1` (the f32 mirror armed
over the asked codec). `test_gpu_resident_cuda_batch.das` - stocked suite,
`-jit` only: the same driver's N-row token command (`ARCHITECTURE_GPU_VULKAN_NROW.md#nrow-token-command` on the CUDA
home) - sessions a mirror region each, six batched steps through one command against each session stepped alone on
the one-row command: every row's logits a step within the split bar of the session alone (the Q8_0 rows read bit for
bit; the K-quant rows round apart, as on Vulkan), every row the compare and its control read asserted finite, the row
one step off the control, both sides' argmax pieces decoded a row, the N-row counter up by every step and unmoved by
the sessions alone, its row counter (`rdec_nrow_rows`) up by every row of every step, every call served - two rows at
two regions on the 1B Q8_0 (the q8 two-column stamp launched) and Q4_K_M, twelve rows at twelve regions on the Q8_0
(one command of twelve rows a step, a tile of eight columns and one of four: the q8 eight- and four-column stamps
launched); the same skips, and where the driver declines the model at the cell's region count.
`test_gpu_resident_cuda_qwen3.das` - stocked suite, `-jit` only: the same driver on Qwen3-0.6B Q8_0, the q/k-normed
carrier, whose norm, rope and K/V store run as the fused pass `CuQknRopeKv` in the token command and the window chain,
in the CUDA llama file's forced-feed form at one window and two (`cuda_forced_feed` of `_cuda_feed.das`, the CUDA
files' one body), and on Qwen3-4B Q8_0 (36 layers, the larger dense qwen3: the mirror slabs and the weight upload a
gigabyte class up) the same way; the same skips. `test_gpu_resident_cuda_phi3.das` - stocked suite, `-jit` only: the same driver on
Phi-3.5-mini Q4_K_M (head 96: the window's attention on the h96 flash tile, the K-quant planes, the fused qkv and gate-up
planes, the long-rope factors) in the same form at the two-window prompt - after the 40-token prompt the K-quant file's
one-row steps at positions 41 and 44 sit 4.5 and 5.96 percent of the max logit off the CPU chain (the Vulkan home 4.1 and
5.8), at the llama bar with the one-step-off control 10 percent off at three steps, so that file holds no one-window arm -
and the Q8_0 file (the q8 decode chain) at one window and two;
the same skips. `test_gpu_resident_cuda_gemma.das` - stocked suite, `-jit` only: the gemma-2 and gemma-3 families on the
same driver in the Vulkan gemma files' forced-feed form (`gemma_forced_feed` of `_gemma_resident.das`: one window of 40
tokens and a two-window prompt, eight fed steps each against the CPU chain) - gemma-2-2b Q8_0 (the attention softcap on
every layer's scores: the window's h256 flash tile under its cap stamp, the token step's attention under its cap, the final
logit softcap; arm `cuda-g2`), gemma-3-1b and gemma-3-4b Q8_0 (the sliding and global layers at head 256, the q/k norms,
the sandwich norms; arms `cuda-g3`, `cuda-g3b`), gemma-3-1b also in the agreement form (`prefill_vs_decode`); the file's
`[init]` pins the MMQ route to `CUDA_MMQ_WINNERS`, and on the CUDA home every compare of the shared forms asserts the CUDA
driver's position counter rising (`cuda_served_since` of `_resident_feed.das`); every cell skips off the JIT, without
`DASLLAMA_CUDA=1`, without `DASLLAMA_GPU=1` on a device the driver serves, or without the file under `models_dir()`.
`test_gpu_resident_cuda_qwen2.das` - stocked suite, `-jit` only: the qwen2 family on the same driver in the CUDA files'
forced-feed form (`cuda_forced_feed`) - Qwen2.5-0.5B Q8_0 (the q/k/v projection bias on both commands, the rope-and-store
kernels' bias arm) at one window and two on the Vulkan qwen2 file's bar (0.06), and its IQ4_NL `-local` mint (the per-32
iq4nl32 planes on the dense sites) at one window on the per-32 carrier's bar (0.08); the file's `[init]` pins the MMQ route
to `CUDA_MMQ_WINNERS`; the same skips.
`test_gpu_resident_cuda_qwen35.das` - stocked suite, `-jit` only (the file's `[init]` pins the MMQ route to `CUDA_MMQ_WINNERS`,
the cells that pin it off as a control restoring that pin): the same driver on the deltanet
hybrids - Qwen3.5-0.8B Q8_0 at one window and two, Qwen3.5-4B Q8_0, the NextN-headed trunk with the head off (a plain
decode is the same chain), the NVFP4 twin (nv4 qkv, z and out planes under their `.scale` factors on the FP4 tensor leaf
under `set_cuda_mmq_pin(CUDA_MMQ_WINNERS)` at one window and two, at the leaf's W4A4 bar, with the MMQ route pinned off -
`CUDA_MMQ_OFF`, the cm2 tiles over f16 activations at the K-quant bar - as the arm's control), the Q4_K_M twin and its
mixed-site mint (K-quant qkv and z planes beside a Q8_0 out plane: the step's requant between sites of two activation
classes) under `CUDA_MMQ_WINNERS`, every pinned cell naming its pin in its label and putting the pin back as it found it
(`with_cuda_mmq_pin` of `_cuda_feed.das`), and the greedy chain on the recurrent carrier (`greedy_chain_cell` on the 0.8B
Q8_0: a miss puts the region's recurrent states back from the chain's snapshot, the off-pick walk's logits the plain walk's
bit for bit), the greedy chain's walks and the peel's legs each asserting the CUDA driver's position counter rising
(`cuda_served_since`); the same skips, and the two NVFP4 leaf cells on a card below the FP4 rung
(compute capability 120, `fp4_rung_on`). `test_gpu_resident_cuda_mtp.das` - stocked suite, `-jit` only: the NextN draft head on the
same driver (`DASLLAMA_GPU=1 DASLLAMA_CUDA=1`), the cells of `_mtp_resident_cells.das` - the bodies `test_gpu_resident_hybrid.das`
runs on the Vulkan home, one body a cell for both discrete homes: the carry the token step, the N-row command and the window
chain land, the draft against the CPU `forward_mtp` (its upload of the host's rows, the suppressed ids, the poisoned residual
rows), the same-slab verify's rows against one-row steps with and without the pick ask, the rejected verify's rollback on the
device, the head's prompt warm over one window and two, six rounds against the CPU round, the depth-8 walk, the gates (a NaN
carry, the pass arms, a rewind, the head kept off) and the q8_0 / tq4 mirrors; the file's `[init]` pins the MMQ route to
`CUDA_MMQ_WINNERS`, every cell asserts the CUDA driver's position counter rising over its prefills (the unhomed pass arm its
control, the counter standing), and the head-embed-gather compare skips on this home (the CUDA driver keeps no such
counter); every cell skips without `DASLLAMA_CUDA=1`
(`cuda_home_asked`) and under the fixture's own skips. `test_gpu_resident_cuda_moe_host.das` - stocked suite, `-jit` only (the
file's `[init]` pins the MMQ route to `CUDA_MMQ_WINNERS`): the
host-experts form on the same driver - Qwen1.5-MoE-A2.7B Q8_0 and its Q4_K_M twin with four routed layers' experts asked
onto the host (`cuda_forced_feed`'s `host_layers`, through `set_gpu_host_layers`, the plan's count asserted exact - `>=` on the
35B alone, whose plan moves more): the split token command as a captured graph a
segment with the host's routed sum between, the cut window chain whose host layers stream through the per-expert chain
(`set_gpu_want_pf_stream(1)`, `set_moe_host_first(8)`, `set_gpu_hot_dma(1)`, `set_gpu_dec_meet(0)` and the pool's slot ask
pinned, put back: the chain's window count `cud_stream_stats` and the meet's device and host expert counts the witnesses,
every meet assert naming `DASLLAMA_GPU_MOE_SPLIT=<value>`, the same cell with the stream pinned off running no streamed
window and moving neither meet counter as the
control), the hot expert pool's hit chains beside the host's sums, each at one window and two at the MoE bar; the pool cells
(`test_gpu_resident_cuda_moe_host_pool`, the Q8_0 carrier under an eight-slot pool ask through `set_gpu_tier_want_`'s `heat` - a
skip naming `DASLLAMA_GPU_HEAT=<v>` where the environment shadows the ask - at a 8192 context, `gpu_device_session_ctx()` and
`gpu_resident_hot_slots()` asserted at the asked values): the steps' misses
landed by DMA off the pinned stream stacks on the copy stream (`cud_stream_dma_fills`, the pool's `dma` count) and the picks
claimed through the decode meet (`meet_steps`), then both knobs off as the control - no copy on the copy stream, the misses on
the host's upload, no met step - and a fourth run of the K-quant carrier with no host layer, where no pool stands, no hit chain
launches, no pick lands and no DMA copies (the pool counters' control); the unstreamed pool cell
(`test_gpu_resident_cuda_moe_host_pool_unstreamed`: the same pool with the prompt
stream pinned off) holds that no stream stack stands - the DMA fill placing nothing over the load, the copy stream copying none -
while the decode's hit chains still serve off the host-filled slots and the fed steps hold the MoE bar; the 35B cell streams its 37 host layers over the pool's prefix with the same witnesses, its pool assert skipping by name where the plan seats no pool, and its rounds cell steps the same tokens plain under `set_mtp_spec(false)` with the draft and verify counters standing (the control); the gpt-oss cells
(`test_gpu_resident_cuda_moe_host_gptoss`, under the FP4 rung and the MMQ pin): two routed layers' experts on the host and
the device claiming first, the biased mx4 stacks through the per-expert chain against the CPU chain at the MoE bar, then the
stream twin (`stream_twin_within`) against the same load with the stream off at the stream twin's bar (`STREAM_TWIN_BAR_REL`, 0.18:
the device's chain over its share of the experts against the host's f32 sums reads 0.4-1.0 of the rows' 7-12 max logits on both
homes of the RTX 5060 Ti, `-jit`) with the chain's biased-act count (`cud_stream_stats().biased`) the form's witness, the Q8_0 and K-quant carriers' twins
holding the count at zero; every cell skips off the JIT, without `DASLLAMA_CUDA=1`, without `DASLLAMA_GPU=1` on a device the
driver serves, without its file under `models_dir()`, without `DASLLAMA_PARITY_FULL=1` (every carrier sits past the large
tier's bytes) and where the prompt window is not `PF_WINDOW_ROWS` rows, the pool cells also where the plan found no room for
the pool beside the mirror on the card, the gpt-oss cells also on a card below the FP4 rung. `test_gpu_resident_cuda_moe.das` - stocked suite, `-jit` only:
the same driver's routed block - Qwen1.5-MoE-A2.7B Q4_K_M (60 experts, 4 routed, a gated shared expert, Q/K/V biases) and
Qwen3-30B-A3B UD-IQ2_XXS (128 experts, 8 routed, no shared expert, the iq2xxs stacks on their own e column), each at one
window and two, every row within the MoE bar of the CPU chain, the Q8_0 carrier the Q4_K_M twin is minted from
(`Qwen1.5-MoE-A2.7B-Chat.Q8_0.gguf`: the Q8_0 expert path - the act + requant, the q8 region GEMV, the q8 e column) at one
window within the same bar under `set_cuda_mmq_pin(CUDA_MMQ_WINNERS)`, and gpt-oss-20b (the native-MXFP4 stacks on the
window's schedule-fed FP4 leaf, the one-row step's gate and up dots with the act and its requant on the fused `Mx4GemvGu`
stamp, every bias, the clamped swiglu, the attention sinks, the 128-key sliding layers' rings)
at one window and two under `set_cuda_mmq_pin(CUDA_MMQ_WINNERS)` within the routed block's bar (a router near-tie
flips under this home's summation order on one step in twelve, the argmax intact, where the Vulkan file's 0.05 holds on
its order), the sink stamps of the K/V codecs (`test_gpu_resident_cuda_gptoss_sink_codecs`, the Vulkan gpt-oss file's twin: the
q8_0 mirror and, under `DASLLAMA_VK_KV32=1`, the f32 mirror in the forced-feed form at an 8192 context; no tq4 arm, since the
carrier's Q/K/V bias steps a tq4 ask down to q8_0), and the ring cell (`test_gpu_resident_cuda_gptoss_ring`, `ring_vs_flat`): 1300 prose ids in 512-row windows
(`set_pf_window_rows_pin`, put back - the prompt wraps the 640-row ring twice; on the CUDA home the driver's window counter
rises by exactly the prompt's windows a load) and eight steps on a ringed load and on a flat
one, the logits bit for bit at the prompt and every step, the ringed load arming its rings and charging less mirror, on the
f16 and q8_0 mirrors under the same MMQ pin; the same skips, and without `DASLLAMA_PARITY_FULL=1` (every carrier sits past
the large tier's bytes), the gpt-oss cells also on a card below the FP4 rung. `test_gpu_resident_cuda_gemma4.das` - stocked
suite, `-jit` only: the dense gemma-4-12B Q4_K_M in the Vulkan 12B file's agreement form (`prefill_vs_decode` at 8 and 300
tokens: the global layers take V off the raw k row at heads of 512 on the wide-head batch attention, the sliding layers scale V
by its own rms), then the gemma-4
routed form on the same driver (the layer's own FFN triple as the dense branch, the routed feed
under its own pre norm, the router over x under its scale row, the picks' weights under the per-expert down scales, the
`cls_ar_comb_g4` combine under the four norms) in the Vulkan 26B files' perplexity form (`ppl_compare`, the same hits slack and
ratio, the device route's own logits scored against the ids one position off landing past the ratio as the control, the CUDA
driver's position counter rising) on gemma-4-26B-A4B UD-IQ3_XXS held whole and on the Q4_K_M file with the rig's context asked whole, which moves the first
routed layers' experts to the host - the split command, the cut window chain and the pool under the form; the 12B Q8_0 in the
Vulkan 12B Q8_0 file's three cells (the forced feed, the perplexity form, the two-window form at its ratio; a 16 GB card arms it
under `DASLLAMA_GPU_MIN_CTX=1024`), the 31B Q4_K_M eight-token agreement cell (a 32 GB card) and the 26B MXFP4_MOE mint's
format census and perplexity cells (the native-MXFP4 stacks on the mx4 stamps); the same skips. The CUDA kernel cells
beside them, `test_cuda_kernels.das` (model-free; a CUDA device with NVRTC, else every cell but the leak gate skips;
the per-format cells - the kq cm2 tiles, the kq and per-32 GEMVs, the token-table gathers, the Q8_K feeds -, the biased
act twins, the deltanet family and the mx4 FP4 leaf over the expert schedule also skip without dasVulkan, and the routing
cell's bucket-schedule arms register a skip of their own there; the mx4 FP4 leaf over the expert schedule also skips on a
card below the FP4 rung): the q8 GEMV and its rate, the MMQ int8 GEMM's leaves against their references - the Q8_0 one and
the superblock ones without a min term (IQ4_XS and IQ4_NL through the codebook, Q4_0's nibbles less eight) per element
against the block-scaled fold, the Q4_K one (its min terms f16-staged and folded sixteen blocks at a time over four
superblocks) at the f16 tiles' bar, the NVFP4 one on the FP4 tensor rung (`test_cud_nv4_mmq`: the k4-tiled nv4 plane
against an FP4 feed in the (k, k + 8) pairing, every element e2m1 x UE4M3 on both sides, the token's row scale and the
plane's factor over the sum) with its feed kernels (`test_cud_fp4_feeds`: a float plane's rows bit for bit the CPU feed
of `_fp4_feed.das`, the gate / up pair through the activation and the fused residual step within a count of strips
that round the other way), and the per-32 ones (Q5_1, Q4_0, Q5_0, IQ4_NL off the 256 lattice) against the
engine's own per-32 dequant dotted with the activation quants, Q5_1 at the f16 tiles' bar for its m - the cm2 l tile
against the f16-feed oracle whole and split in two k chunks through the reduce (`test_cud_q8_cm2l_split`), its rate at
the 1B window's shapes - each rate cell's warm-up held to the oracle first, the GEMV's every row and the tile's
activation rows 0, 255, 256 and 511 over every column, the tile's bit for bit on a fixture its f16 accumulator holds
exactly (sixteen +-1 activations a row over the 2^-7 block scale) - the word copy (a quad-lane run and a three-word tail, the
device block and the out block under a word sentinel), the flash tiles at heads 64, 96, 128, 256 (`test_cud_fa_h256`:
plain and gated, the gated rows under the sigmoid of the q panel's gate half, the ungated oracle over the q halves the
control; `test_cud_fa_h256_softcap`: gemma-2's cap of 50 on the f32 and f16 cap stamps against the capped oracle, the
uncapped one the control) and 512 on the KHR arm (`test_cud_fa_h512`, gemma-4's global layers) with their f16-out twins, through the driver's own ladder (`cud_fa_enc`), against `fa_oracle` of `_attn_oracle.das`
(the Vulkan flash cells' oracle too), whole and under a 40-key window, the deltanet family (`test_cud_dn_family`, at heads
of 64 and 128: the conv, the fused step on its Q8_0, f32 and sigmoid-gated o rows - the state and the ring advanced in place
and asserted moved off their inputs, the sigmoid stamp off the silu oracle as its control -, the two-phase scan with the
silu and sigmoid out-norm (`dn_gate_ref` of `_kernel_oracles.das`, the Vulkan cell's oracle) and its state moved off its
input, and the ring tail at 5 and 2 rows bit for bit the per-channel CPU form), the fused q/k norm + rope + K/V store on the f32 and the f16 mirror stamps against the
oracle the Vulkan cell holds it to (`qkn_rope_rows` of `_kernel_oracles.das`, the rows ahead of the layer's kept at their
fill), the mx4 expert rail (the GEMV at 704 and gpt-oss's 2880 against the scalar dot in the per-32 GEMV cell, the s and e
tiles in the cm2 family cell, and the FP4 tensor leaf fed from the expert schedule - `test_cud_mx4_fp4_mmq_moe`: two buckets of
200 and 300 rows padded to 128-row tiles by the CPU schedule twin, 2880-wide rows on the k tail, a 320-wide plane under the
store's column guard, two sentinel workgroups past the bound, every real row against the e2m1 x UE8M0 reference on both sides
(`mx4_fp4_mmq_ref`); the routing cell's schedule arm also holds the padded m-only schedule word for word; the MXFP4 feed kernels
(`test_cud_mx4_feeds`: `mx4_rows` bit for bit the CPU feed, `mx4_actrows` under the clamped swiglu and its biased twin with each
row's expert's bias rows, the biased feed landing past the compare's allowance off the unbiased CPU feed as the control,
every run's planes refilled under their sentinels); the leaf's width admission
(`test_cud_moe_mmq_fits`: both reduction widths on the 64 lattice serve, a width at 32 past it on either side keeps the cm2
tiles)), the biased act twins against the CPU act over the host-biased rows (`test_cud_act_biased`: the
Q8_0 and Q8_K requant forms and the f16 form, under silu and the clamped swiglu, the second bias layer's rows bound, the
unbiased act the control), the h64 flash and the q8_0 decode attention's sink stamps seeded with a per-head sink logit (the
sink-free oracle the control), the q8_0 decode attention's split form at eight pieces, the last-arriving one combining,
plain and with the sink seeded at the combine, the routed block's routing kernels against the oracles the Vulkan routing
cell reads (`_moe_routing_oracles.das`: the router GEMM at two row counts, the per-row select renormalized, scaled and
under gpt-oss's biased pick, the two-deep select (`topk_rows2_cls`) over 512 experts at ten picks a row on the Vulkan
two-deep cell's planes (`topk_wide_planes`), plain and under the router bias, with the one-deep stamp over the same rows
missing the host select's picks as the control, the bucket schedule at 256 and 512 experts against the CPU twin word for
word, the gather (`moe_gather_f16_cls` as halves, `moe_gather_f32_cls` as floats) into a nine-row window of 21 bucket rows
at widths of 100, 256 and 300 against `moe_gather_oracle`, the rows past the window kept at their sentinel and the gather
over the window at 0 the control, the per-expert chain's by-row pair (`moe_gather_f16_row_cls` over the same window, each
window row its grid entry's position row as halves; `combine_row_cls` over a six-row window one row a position at alternating
slots, each row's weighted down row onto its position's accumulated row, a position no row reaches untouched - the Vulkan
kernel file's two cells), the combine-folded residual step's f32 form under the gated, the ungated and no shared
expert and under the down biases over twelve slots), the compile gate over every
`[cuda_dispatch]` kernel of the home, the shutdown leak gate, the worker-thread bind (`test_cud_worker_thread_binds`: a
`new_thread` worker's own context enters the tier and reads the device's free memory as the main thread does), the short
window's s column (`test_cud_q8_cm2s_split`: the q8 s tile split in two and four k chunks through the reduce against the
f16-feed oracle, the last chunk short; `test_cud_cm2s_short_split_family`: every per-format tile row's s column split along
K at 31 rows against its own oracle; `test_cud_short_window_pick`: `cud_cm2_pick` whole at knee 0, `32x4x256` and
`32x2x768` at knee 256, a full column the wave model's whatever the knee), the E-series pre-step's kernels
(`test_cud_ple_finish`: three layers' slices of 256 over two rows in place against the CPU finish a slice at a time, one
norm over every slice the control; `test_cud_router_gemv_bf16`: the bf16 pair and halfword router GEMV stamps at an even
and an odd dim, three columns at a row and an output base under the output scale against the CPU dot, the sentinel past
the last column kept; `test_cud_f16_gemm`: the small f16 x f16 GEMM on the cm2 tile at five shapes up to the E2B
projection's whole window against the f32 dot, the header under the output base kept; `test_cud_ple_bf16_cvt`: the bf16
rows as f16 bit for bit the widened value's half, saturated, the elements past the count kept), the K-quant and per-32
GEMV leaves and cm2 tiles against the Vulkan cells' oracles, the token-table gathers (the per-32 leaves against the
engine's plane dequant, the exact f32 table's leaf against the rows' own floats), the
Q8_K feeds (the quants a byte within one step, the scales and the in-place residual row through `check_rows`, the
residual row moved off its input), and the K/V mirror's
block codecs - the q8_0 store at token rows and window rows against the CPU cache's bytes, the tq4 store at seven arms
(heads of 32 to 512, a shared-KV layer's rotation alone, the window stamp, the warm with no q head) against the CPU
rotation and the cache's bytes bit for bit, the q8_0 and tq4 shadows against the CPU dequant as halves bit for bit,
the tq4 un-rotation bit for bit and under the head gate, the decode attention's q8_0 and tq4 stamps on both slabs
against the CPU attention over the dequantized rows (the oracles and staged fixtures the Vulkan codec cells share
through `_kv_codec_oracles.das`).
`test_gpu_resident_segments.das` - stocked suite, `-jit` only; the whole-model resident driver's
SEGMENTED window (`ARCHITECTURE_GPU_VULKAN_WINDOW.md#vk-prefill-segments`, `DASLLAMA_GPU=1`) on
Llama-3.2-1B at Q8_0 (the ids form) and Q4_K_M (the x form), two mirror regions: two host-cached
sessions' tails (23 and 31 rows after 40-row openings, and again 300 and 300 - a call of two windows, segment
A's last row landing in the first window and B's rows crossing into the second) as ONE window chain against
the same tails as two one-session windows on the same driver, then eight fed steps each - every row within the resident
rigs' split bar (`SPLIT_BAR_REL`) of the one-session form's with the row one step off as the control,
both argmax streams decoded in the log - then both openings as one window from position zero, each
session's last-row logits within the bar of its solo opening's with the other session's row as the
control, the argmax rows decoded; the witnesses are the segmented-window counter
(`vk_rdec_prefill_segment_windows`) up by one a call for the window that holds both segments (the 300-row tails' second window holds B alone), the driver's prefill count up by a segment, and
every call served; the decline cell runs two sessions' openings under the server's prefill pin
(`set_resident_prefill_allowed(false)`, put back as found) - no segmented window, no prefill counted
(the served cells' counter's control), the engine's one-at-a-time fallback passing each to the CPU
rails as `pinned_off` and landing every row within the bar of the device-prefilled references, the
argmax rows decoded; the decline-arms cell holds the form's other declines, each leaving the
segmented-window counter where it was - a segment's session on another K/V codec than the mirror (the fallback
passing it as `codec`), both regions held by device-home sessions (each host-cached session passed as `busy`),
and a session adding deepstack slices (`ds_active`) the override has no arm for, the other segment running alone
on the driver and that one taking the CPU prefill loop with no pass counted; the ownership cells hold the admission's
dry run - an owner's continuation beside a newcomer binds the owner first and the newcomer takes the region touched
last (one window, the owner's rows the solo form's), three segments over two regions decline whole with no pass
counted and each served alone, and an owner whose region holds fewer rows than its start declines whole with the
fallback's one `gap` pass. The recurrent cells (`recurrent_segments_cells`) run the segments cell, a 2-row tail under the conv
taps beside a 31-row one, and the two 300-row tails on Qwen3.5-0.8B Q8_0, its NextN-headed twin (the carry a row a segment) and,
large tier, Qwen3.6-35B-A3B-MTP IQ4_XS (the routed block over every row of the window), each asserting the driver's segmented
window takes the model. Their captures cell (`captures_cell`): two device-home sessions' tails with checkpoint stops inside them as one segmented
call, session A's region already keeping a checkpoint row at its opening (`gpu_device_kv_checkpoint`), so the two segments'
captures plan different rows of their regions; both parked, each region's rows read back (`gpu_device_kv_checkpoints`), and
every stop's row adopted by a fresh session (`gpu_device_kv_adopt` at the stop, the later stop of a region first) that
continues the tail and takes the fed steps within the split bar of the solo form - a landing that matched the call's
records to a segment by a free row alone took another segment's record here. The E-series cell (`test_gpu_resident_segments_gemma4_e2b`, family `gemma4e`: gemma-4-E2B Q8_0 at the
gemma rig's bar) runs the segments cell and the two 300-row tails, then the host-embedded decline (`host_embedded_cell`): with the
tier's token-table gate unseated (`unregister_embed_gpu_gate`, put back as found) the host embeds every row, the side rows have no
ids to gather off, and two openings ride no segmented window - each its own window, its last row within the bar of the same
opening prefilled alone with the gate back, the openings' segmented window in the segments cell the counter's control. The file never holds more live host-cached sessions than regions: a later
prefill would take a live session's region, which the engine leaves to the scheduler's pin. Under
`DASLLAMA_CUDA=1` the llama cells run the same way on the CUDA home's segmented window, the segmented-window
counter then `cud_rdec_prefill_segment_windows`, the recurrent, E-series and captures cells among them. Skips
without the model or the armed tier, and on a card whose room the plan does not fit at two regions.
`test_batch_decode.das` - stocked suite, `-jit` only: the CPU batched stack against the
per-session sequential forward on llama2.c's stories15M (fp32, Q8 and Q4_0 weights - the Q4_0
tier rides the batched Q4_0 GEMM, bit for bit the GEMV's dot - and the f16 / q8_0 / tq4 KV
codecs, the mid-run shrink, the B == 1 delegation, the step census) and on the deltanet hybrid
Qwen3.5-0.8B Q8 (f32 and f16 KV at 1e-6: the recurrent rows form per session, the gated
partial-rope attention rows, the final deltanet state and conv history, every step counted on
the CPU batched stack by the census - the loaded model's GPU state dropped, as the scheduler
file's SmolLM cells do, so both sides serve on the CPU lane under `DASLLAMA_GPU=1` - and
a decode below the deltanet position panicking - the state is forward-only) and, under the
large tier, on the MoE hybrid Qwen3.6-35B-A3B UD-Q4_K_M - the f32-on-disk beta/alpha arm of the
batched layer's projection prologue, f16 KV at 1e-6; a cell whose bar is exact free-runs the
batched greedy and holds the
tokens, a cell whose bar concedes rounding feeds the reference's tokens and holds the logits, and
every cell carries the cross-row control (row 0 against row 1's reference lands outside the bar);
plus the batched bench row's refusal contract on SmolLM2. Two cells on stories15M run under a fake decode override whose
batched step declines every call (`batch-decline-probe`, its embed batch gate registered beside it, the selection put back as
found and the probe unregistered after): `test_batch_decode_late_embed` - three batched steps over two sessions with the gate answering that the step gathers
its rows and with it answering no, the override asked every step, every row embedded on the host once (late where the gate
took it, `batch_rows_host_embedded`), and the last step's logits bit for bit the same steps with no override; and
`test_batch_decode_verify_declines` - `verify_batch_groups` over two groups that reach the override (the control), then a
group of no rows, a group past the context, groups on two K/V codecs and a flat group beside a paged one, each serving
nothing and never asking the override.
`test_metal_batched_row.das` is the row's Metal twin: with no whole-model driver homing a
stream the row runs host-cached through the Metal batched driver - Llama-3.2-1B Q8, the
E-series gemma-4-E2B Q8, the shared-expert Qwen1.5-MoE-A2.7B Q8 (large-tier), the deltanet
hybrid Qwen3.5-0.8B Q8 (at four streams and at nine - more than the recurrent mirror cache rests
at, which grows to the batch) and, under the scheduler's self-speculative mode, Qwen3.5-0.8B-MTP Q8 -
a rate, every timed step counted a device step by `batch_step_census`; the Llama and E-series carriers also run the
gather cell (`test_metal_batch_gather_*`): three host-cached sessions through six batched steps, every row gathered off its
id (`metal_batch_rows_gathered` up by the rows, `batch_rows_host_embedded` unmoved), every row's logits bit for bit the same
steps' with the gather off (`set_metal_batch_gather(false)`: the host embeds every row and the step pokes it up, no row
gathered - the control); the shared-expert
carrier also runs the one-row same-slab cell (`test_metal_batched_row_shexp_sameslab`; same-slab =
consecutive positions of ONE session as the rows of one batched step, `../ARCHITECTURE_GPU_MTP.md#batch-same-slab`):
twelve rounds of one row through `verify_batch_step` against the single-row step on a second
session of the same prompt, the logits within the plain-MoE bar of 2.0 at every round (reads
1.6e-3 at most on the M1 Max, the one-step-off control 9.7 at least, asserted outside the bar),
every round served by the batched driver - a rows step at one row takes the rows forms, never the
single-row step's shared-expert GEMV over its `bh12s` panel
(`../ARCHITECTURE_GPU_MTP_DECODE.md#metal-layer-enc`); the MTP carrier also runs the depth-2 cell
(`test_metal_batched_row_mtp_depth2`): a greedy walk (`generate_mtp_greedy`) at depth 1 and at
depth 2 over up to 24 reply tokens, the same tokens, the depth-2 walk drafting more than the
depth-1 walk (at depth 2 each draft joins on the host before the next is made) and accepting some; the pick cell
(`test_metal_pick_ask`): a greedy scheduler stream's steps land their device pick in the logits' place
(`metal_picks_landed` up, at most one a token) and emit token for token what `generate()` emits off the host's
argmax, and a stream whose sampler is not a bare argmax lands none (the control);
the MTP carrier also runs the three-stream invariance cell
(`test_metal_batched_row_mtp_joint_invariance`): three speculative streams admitted together emit,
token for token, what each emits alone on a one-stream speculative scheduler, every tick stepping each
stream through its own round (`mtp_joined_max` 0), the ticks' draft and accept counts equal the three
solo streams' sums, the solo rounds accept more than half their drafts; and its staggered twin
(`test_metal_batched_row_mtp_joint_staggered`): the same three streams with the LAST one on a prompt past
200 tokens across three 64-row chunk boundaries, each round's chunk count and attention form its own
stream's, and the paged cell (`test_metal_paged_mtp`): one speculative stream of 150 reply
tokens on a scheduler of 64-row pages - the server's session kind - emits token for token what a
flat session emits, drafts accepted on both sides, the stream's rows asserted to span three pages
so rounds cross page boundaries, and a Metal-served model's `chunk_defaults` read as
`GPU_IDLE_CHUNK_TOKENS` idle and `DEFAULT_CHUNK_TOKENS` chunk; the Llama carrier also
runs the pre-encode cell: four greedy streams through the scheduler with the batched driver's
pre-encoded step off (the reference, its taken count pinned at zero) and on (the taken count at
sixteen or more; the shipped default reads off), token for token per stream, then the rail's
retire under a parked step and the drivers' shutdown against the Metal live-object count; the
deltanet carrier also runs the greedy chain cell (`test_metal_deltanet_greedy_chain`): 24 steps with the single-row
chain off and forced on, fed the greedy pick and then fed off the pick every third step - the chain's hits counted
on both feeds and its misses on the off-pick one, every step a device decode, the tokens and every step's logits
bit for bit the plain run's (a mispredicted chained step that wrote the live recurrent state would part them), the
released-sessions cell (`test_metal_deltanet_released_sessions`): sessions one more than twice the mirror cap
(`dn_mirrors_cap`) each stepped alone and deleted - its state held while it lives and gone from the mirrors and the
stash after (`dn_mirror_holds`) - then as many live sessions stepping in turn, every step a device decode, the stashes
rebuilding the evicted mirrors, each session's tokens its own alone, and the
paged cell (`test_metal_paged_deltanet`): one and two greedy
streams of 40 prompt and 40 reply tokens on a scheduler of 64-row pages - the server's session
kind, each stream's rows past one page - emit token for token what flat sessions emit (one code
path over one set of bytes: the mirror gathers the pool's rows verbatim), each side's Metal
decode steps counted on their own with a one-token reply as the floor's control, both streams
decoded in the log (a recurrent layer owns no K/V rows, so its pool blob is empty and the
mirror's walk reads no base for it); the MTP carrier also runs the prefix checkpoint cells
(`ARCHITECTURE_ENGINE_SERVING.md#prefix-recurrent-checkpoints`): `test_metal_prefix_checkpoint` - three
prompts on one shared opening (asserted past two 64-row pages and off a page boundary) through one speculative scheduler: the first two attach nothing,
the second's prefill leaves the checkpoint at the opening inside its window, the third attaches it, and the
second and third reply token for token as on a scheduler with no cache whose chunk is the opening
(the cached run's own cuts), the two replies differing, then a marked request on a scheduler whose
chunk is three quarters of the opening with the idle quantum off: the first tick prefills the whole
opening: the first tick's window is the chunk alone and leaves no checkpoint, the second carries the opening's stop inside
it and leaves the checkpoint captured at the stop's row, the run ending on two (the opening's and the finished turn's);
`test_metal_prefix_checkpoint_hinted` - a
request marked with its stable opening (`PendingReq.stable_at`) leaves the checkpoint at once, the
second prompt attaches it; a marked first request leaves two checkpoints (its opening and its
finished turn), an unmarked one its prompt's end and its finished turn, and the attached reply is token for
token the uncached reply at the same cut;
`test_metal_prefix_checkpoint_logits` - the verbs direct: a session attached at a checkpoint off a
page boundary lands the donor's logits bit for bit after the same tail (the carry's position
restored, the partial page its own copy), a checkpoint with its recurrent state zeroed lands
logits 5% of the largest or more away, every page back in the pool - the reply to a strongly
prompted tail survives a lost state, which is why this cell holds the logits;
`test_metal_prefix_checkpoint_capture_logits` - a checkpoint captured inside one window: a request marked at a stop past two
pages and off a page boundary, its whole prompt one prefill call (`prefill_evals` 1, one checkpoint standing after that
window), then a fresh session attached at the capture and fed the tail lands within 0.2% of the largest logit of an uncached
session prefilled over the opening and then the tail (the capture stands inside a window of the whole prompt, the reference's
state at the end of a window of the opening alone, so the scans and GEMM tiles round apart: reads 0.055%, the prompt as one
window 0.049% off the reference), the capture with its recurrent state zeroed landing finite logits 5% of the largest or more
away (reads 17.5%);
`test_metal_prefix_checkpoint_next_turn` - a finished turn's checkpoint: the prompt plus its 40
counted tokens attach for the next turn, which continues the count token for token against an
uncached 80-token stream. Stocked
suite; skips off the JIT, without dasMetal, or without the carrier. The row's refusal contract - a timed step that ran its rows one at a time refuses by
name and reads 0 - lives in `test_batch_decode.das` on the SmolLM2 fixture with the rope table
off, where every step is per-row by construction.
`test_gpu_resident_format_census.das` - stocked suite, `-jit` only, `DASLLAMA_GPU=1`; the census rides `_format_census.das`
(`census_cell`, `census_sites_all`), which the Metal file shares. Arm `vk-census` (family `qwen35moe`, large tier) on
Qwen3.6-35B-A3B-MTP-IQ4_XS-pure: every site native, the whole-model driver armed, a window and six fed steps against the
CPU chain at the routed bar with the IQ4_XS tile and GEMV counts rising (the per-op cell also holds every layer's shared
expert on the per-op rail in the file's own format - `perop_shexp_native` - with `shexp_gpu_prefill_layers` up by every layer
and the span or the per-layer chain serving every fed step's, no layer on the shared expert's three dense GEMMs - one
activation form a layer), the untied IQ4_XS table gathered by the window
(`kq_emb_iq4xs_cls_spv`) and by every step where the table's plane sits on the device (`gpu_resident_embed_gathers`:
`vk_rdec_token_gathered` up by the steps; where the plane stayed on the host, no step row gathered) - `load_census` loads
whole where the card holds the experts and again at `HOST_CTX` with the first routed layers' experts on the host after a
memory decline; the verify-rows cell - one round's two verify rows through the split command's segments within the routed
bar of the one-row steps (the other row the control; the two plain steps move no verify counter: the no-route control),
then four rounds' committed stream against the same tokens stepped (`spec_rounds_into` of `_resident_feed.das`); the NextN cell - the routed head
armed (`rdec_verify_rows` at two or more, `gpu_resident_mtp_device_home_`), its prompt rows warmed by the window chain,
one device draft against the CPU head's (`unset_rdec_draft`) within the routed bar with another token's CPU draft as the
control and the pick tie-aware, six rounds drafted and verified on the device (`moe_gpu_resident_drafts` /
`_verifies`), then a device-home session's six rounds against the host-cached session's: the forced-feed form - each
round's verify row 0 within the routed bar of the host's with the host's previous round as the control, through the round
whose drafts part - and the rounds accepting and standing alike while the drafts (the head's picks, which can part on a
near-tie) agree; the per-op cell (the route off, every deltanet layer on the per-op rail's IQ4_XS stacks); the F32 beta/alpha cell
on Qwen3.6-35B-A3B UD-IQ2_XXS. Arm `vk-gather` (families `qwen3`, `llama`): Qwen3-4B-Instruct-2507 Q4_K_M and
Llama-3.2-3B-Instruct Q4_K_M, the tied K-quant table gathered by the window (its `kq_emb_<fmt>_cls_spv` count) and by
every fed step off the classifier's plane, the forced feed against the CPU chain at the split bar (Qwen3-4B's K-quant chain at
`QWEN_KQ_BAR_REL`, its reading in the constant's `//!`). Arm `vk-perop-ffn` (families `llama`, `qwen2`): the per-op FFN rail
(`../ARCHITECTURE_GPU_VULKAN_PEROP.md#perop-dense-ffn`) on Llama-3.2-1B Q8_0, Q4_K_M and the Q5_1 mint and Qwen2.5-0.5B's
IQ4_NL `--pure` mint, loaded with the whole-model route off - with the rail off a fed run counts no device FFN row
(`moe_gpu_dense_ffn_rows`, the counter's control), with it on (`set_gpu_want_dense_ffn`, put back) every layer's triple and
q/o pair is the dense rail's and the classifier the tier's, in the file's format, the window's q/o and FFN GEMMs run on the
device (`moe_gpu_dense_gemm_rows`, five a layer a row at least; the CPU chain with the device state dropped counts none, the GEMM
counter's control), every fed step runs each layer's FFN row on the device, and
the steps hold the CPU chain at the llama file's bar (the qwen2 mint at 8%, its reading in the carrier list's `//!`). Arms `vk-per32-iq4nl32`,
`vk-per32-q4032`, `vk-per32-q5032` (family `qwen2`) and `vk-per32-q51` (family `qwen35`): the per-32 `--pure` mints, every site native
(the census), the table gathered by the window and every step off its `<fmt>_emb_cls_spv` leaf, the forced feed at the
qwen2 file's bar (the iq4nl32 mint at that file's iq4nl32 bar, 8%), and on the Q5_1 hybrid the NextN head armed (`rdec_verify_rows`).
`test_metal_format_census.das` - stocked suite, `-jit` only: every site's tag against the file's own tensor type
(`kq_ggml_type(tag)` against `gguf_tensor_type`; a tied classifier is the token table's plane), one soft check a site -
attention q/k/v/o, routed experts, shared expert, deltanet qkv/gate/out, deltanet beta/alpha, token table, classifier,
NextN eh_proj - on the planar load and on its blob twin, the sites still on a converted copy logged as one list; then a
Metal prefill window over a counting prompt and six Metal decode steps on the twin against the CPU chain over the same
tokens, the CPU chain's next row as each compare's control outside the bar, the prefill and decode counters, and the
token-table gather dispatched by the window and by every step (the host embeds no row). Arm `census-iq4xs` (family
`qwen35moe`, large tier): Qwen3.6-35B-A3B-MTP-IQ4_XS-pure, every big plane IQ4_XS on disk, at the qwen35moe row's bar
(6.0) - the window's beta/alpha planes on the IQ4_XS expert-form GEMV (`metal_moe_gemv_iq4xs`, none on the q8
`MetalDnBa`), the steps' IQ4_XS single-row GEMV count covering every recurrent layer's five deltanet planes and every
layer's shared-expert triple a step, no q8 fused gate+up dispatch, and the window's routers on the f32 tile over their own F32
rows (`metal_f32_exact_mulmm`, a dispatch a routed layer at least). Arm `census-tied` (family `llama`):
Llama-3.2-3B Q4_K_M, the tied Q6_K table read as the classifier's plane and gathered by `MetalEmbedK6`, at the dense
K-quant bar (4.0), no fp32-table row gather. Arm `census-f16` (family `qwen3`): the canary decoder
(`canary-qwen-2.5b-decoder-f16.gguf`, a qwen3 1.7B whose every plane and tied table is F16) - the load keeps
`cls_q8_copy`, the census lists the classifier and the token table each one converted copy (the attention sites with
them, the F16 planes' q8 transcode), the window and six steps within 1.0 of the CPU chain (read 0.21-0.30, the controls
10.4 and up), the token rows gathered off the exact fp32 table (`metal_row_gather`) and never a q8 table. Arm
`census-iq3s4` (family `qwen2`) pins the lut form (`set_iq3s_serve_pin`, put back): Qwen2.5-1.5B IQ3_XS - its 70 IQ3_S
planes loaded as iq3s4, none as the grid, every site native, the window on the iq3s4 GEMM stamps (the last layer's
narrowed site on the GEMV) and the six steps' iq3s4 single-row GEMV count covering every plane a step, at the dense bar
(read 0.67-1.06); `census-iq3s4-moe` (family `qwen35moe`, large tier): Qwen3.6-35B-A3B UD-IQ3_S's routed IQ3_S downs as
iq3s4 stacks, the window's and the steps' routed iq3s4 stamps counted. Arms `census-q51` (family `llama`) and
`census-iq4nl32` (family `qwen2`): Llama-3.2-1B's Q5_1 mint and Qwen2.5-0.5B's IQ4_NL mint (its v planes Q5_1), the dense
planes loaded as q51 / iq4nl32, every site native on both loads, the window's per-32 sites on the per-32 GEMM, small-batch
and GEMV stamps, the six steps' per-32 GEMV count covering every plane a step, at the dense bar (read 0.26-0.36 and
0.57-0.66). Arms `census-iq4nl32-table`, `census-q4032`, `census-q5032` (family `qwen2`) and `census-q51-table` (family `qwen35`): the
`--pure` mints of `../performance/model_specs.das` - Qwen2.5-0.5B IQ4_NL, Q4_0 and Q5_0 and Qwen3.5-0.8B-MTP Q5_1, whose token
table, classifier and (on the hybrid) deltanet planes and NextN eh_proj are per-32 too - the same cell with the window and
every step gathering the table off `MetalEmbedB32<Fmt>`. Arms `census-moe-q51`, `census-moe-q4032` and `census-moe-q5032` (family
`qwen2moe`, large tier): the Qwen1.5-MoE Q5_1, Q4_0 and Q5_0 `--pure` mints, every site native, the routed per-32 stacks on
the routed stamps through the window and every step, at the plain-MoE bar (2.0; read 0.62-1.07 and 0.97-1.74). Its model-free cell holds the classifier predicates
on Model shells (`cls_tied_f32`, `cls_q8_copy`, `emb_kq_plane_off`).
`_resident_regions.das` - the cells the `test_gpu_resident_regions*.das` files run, one model a file (the entries below): the
resident driver's mirror regions and the device-home sessions over them. A carrier loads at two regions through
`set_gpu_resident_regions_`, the rig's context 8192; the three long-context files load four or two regions at the mirror's
whole context and size every host-cached session to it. The instrument is the driver against itself, so no CPU reference
chain runs but in the long-context files' CPU-chain cell. Every batched cell asserts its steps served by the N-row command
(`vk_rdec_token_n_rows` at two or more). Every cell holds `gpu_cpu_passes_()` empty (the pin cell its one `busy`), and every
tolerance compare logs its difference and the one-off row's. Every file is stocked suite, `-jit` only, and skips without its
model or the armed tier, and on a card whose room the plan does not fit at the regions the cell asks (`load_regions`
registers the skip with the driver's own decline text, `moe_gpu_resident_memory_decline`; any other decline is a red). The
cells, by the names the file entries use:
- the handover cell (`cell_region_taken_after_decode`, loaded at ONE region): a session prefills and takes a one-row step -
  its rows on the device alone - a second session prefills and takes the region, then the two step in turn, every row bit
  for bit its session alone and the saved-claim counter (`moe_gpu_resident_stashes`) up;
- the interleaved single steps (`cell_regions_interleaved_single_steps`): a session stepped between another session's single
  steps reads what the session reads alone, the two prompts differing in content and length so a crossed region cannot land
  there;
- the two batched-step cells (`cell_regions_batched_steps`, `cell_regions_batched_steps_split`): a batched step over both
  sessions reads what each session reads alone - bit for bit, unless the file's entry names a bar - the claims unchanged
  through the batched steps (no history came up from the host again); the second cell runs the same batched steps past the
  unsplit position (both prompts longer than `RD_UNSPLIT_POS`, so the N-row command's split attention form serves, its
  arrival counters a row apart);
- the batched-first cell (`cell_regions_batched_first`): a batched step before any one-row step, a one-row step, a batched
  step again - the unsplit twin's availability is the device's, never the one-row record's;
- the widening cell (`cell_regions_batched_widen`): three regions - two rows batched, each alone, then all three batched: a
  record at a row count the command had not recorded after a one-row step, so the N-row sets' own combine form and the
  served-step counter hold;
- the split-forms cell (`cell_regions_batched_steps_split_forms`): the fuse knob pinned off for the load, the rows against
  the split one-row forms (skips under the CUDA ask: the lever is the Vulkan driver's);
- the wide single steps (`cell_regions_wide_single_steps`): both prompts past `RD_WIDE_POS`, so the one-row command's wide
  twin records and serves, bit for bit against the session alone;
- the fuse bisect (`cell_regions_fuse_bisect`): `DASLLAMA_VK_FUSE_BISECT` a bit at a time, one model load a bit - the
  one-row steps within the split bar of the fused run with the one-token-off control, and under the fused q/k norm's bit
  the batched rows bit for bit on the split pair (skips under the CUDA ask: the lever is the Vulkan driver's);
- the groups cell (`cell_regions_batched_groups`): more sessions than the N-column leaves' eight-column tile, or than the
  N-row command steps at once, a region each - every batched step the fewest commands of the command's rows, its GEMVs in
  column tiles, never a row at a time, each row against its session alone;
- the side-rows cell (`cell_regions_batched_side_rows`): a batched row carries its own per-layer-embedding side input,
  gathered on the device off the row's id - no table row gathered on the host (`ple_host_gather_rows`), every batched row
  counted by `vk_rdec_nrow_side_gathered`;
- the embed-gather cell (`cell_regions_batched_embed_gather`): every batched row's token row gathered off its id -
  `vk_rdec_nrow_embed_gathered` up by the rows, `batch_rows_host_embedded` unmoved - and every row's logits bit for bit a
  second load's with the gather pinned off (`set_gpu_resident_embed(0)`, which gathers none, gathers no side row off an id -
  the side-rows cell's counter - and embeds every row on the host), the control; both sides' argmax pieces logged;
- the fold cell (`cell_regions_batched_down_fold`): on the fused forms the server runs, both commands fold the per-32 routed
  down under the routing weights - the summed stamps dispatched and no per-slot down leaf (the census over every per-32
  format) - the split-forms cell's load dispatching no summed stamp (the control);
- the ring floor cell (`cell_ring_floor`, the load ringed): a 1300-row prompt leaves the session's floor at its rows less the
  ring's slack, a call at 40 panics naming it, a 40-row prompt from 0 resets the region's high-water mark and the floor to 0;
  a device-home session's parked claim refuses an adopt of 40 rows and hands all 1300 over with the floor, an export ending
  at 40 and an import of 40 refuse while the whole-row ones move the rows, each carrying the floor;
- the window's fused q/k norm cell (`cell_window_qkn_fused`): a prompt two windows long with the fused pass and with the
  split pair (`set_vulkan_prefill_qkn_fuse`, put back), the census counting the fused pass on the one and the norm pass on
  the other, the prompt's logits and eight fed steps within the split bar, unless the file's entry names a bar, with the row
  one step off the control (skips under the CUDA ask: the lever is the Vulkan driver's);
- the device-home cells (`cell_device_home_sessions`): the single and batched steps again with device-home sessions, which
  allocate no host cache, prefill on the device under the server's prefill pin (`set_resident_prefill_allowed(false)`) and
  come through a hydrate untouched; then the pin - with both regions held by live device-home sessions a host-cached
  outsider passes as `busy`, the owners step on untouched, and a parked region is the next outsider's;
- the scheduler's device mode (`cell_scheduler_device_mode`): against the host-cached scheduler over four requests on two
  streams (`_scheduler_rig.das`, shared with `test_scheduler.das`) - token for token where the two sides run one code path
  (each prompt one window-chain call, each step the batched token command, the prefill counter pinning it), and for the
  second turn, which adopts the rows of the turn it continues and so splits its prefill, the adopted count alone; the
  device-home side's two prompts admitted together ride one segmented window where the model's segmented window serves
  (`gpu_device_prefill_segments`), the counter's rise the tick's witness; the three argmax requests' decode steps land their
  picks on the device (the count held equal to the rows stepped less the second turn's, which samples at top-k 1 under a
  temperature and so lands logits);
- the handoff cell (`cell_scheduler_device_handoff`): two device-home streams of 300 and 1024 rows whose first leaves fewer
  rows than the quantum - where the model's window serves, the first tick carries both and cuts the second's prompt there
  (four calls counted a segment, the segmented-window counter up); where it declines, each tick is one stream's own call
  (three calls - two on a model whose prefill cannot continue, whose second prompt goes whole - no segmented window);
- the prefill-chunks cell (`cell_scheduler_device_prefill_chunks`): a device-home stream prefilling in window-sized
  chunks, and the second turn adopting the rows - and on a recurrent model the state - of the turn it continues;
- the tolerance cells (`cell_prefill_continues_on_the_device` and the parked claim below): one prompt prefilled in one call against two calls, cut at 40 of 57, 512 of 600, 300 of 900 and 40
  of 42 (the cuts placed against the engine's prompt window, `pf_window_rows`, asserted), a prompt one token short the control, then four
  decoded steps over the continued rows, each against the step before it as its control; the chat shape (a prompt, steps, a
  second turn through the window chain against the same tokens stepped one by one); and the parked claim
  (`cell_device_kv_park_adopt`) adopted once, then refused twice (spent by its adoption, and dead after another session took
  its region) - every site that holds the bar carries its own one-token-off control, and every compare logs both sides'
  argmax;
- the recurrent parked claim (`cell_device_kv_park_adopt_recurrent`): a stream parked at 57 rows hands them and its state at
  their end to a session that takes all 57 (`gpu_device_kv_parked_pos_` the witness, the 17 tokens past them one device
  prefill within the bar of the 74-token prompt in one call, the one-token-short control past it) and refuses 40, since the
  state stands at no earlier row;
- the paged cell (`cell_scheduler_paged_resident`): the same four requests on a paged scheduler (64-row pages) and a flat
  one, the paged streams' prompts in the window chain and their steps on the device - their pages up a physical run at a
  time, the rows the device stepped hydrated into the pages before the prefix cache takes them - token for token the flat
  side's for the first three, the second turn attaching the pages (or, on a recurrent model, the recurrent checkpoint) its
  first turn left, every call served;
- the checkpoint cell (`cell_device_checkpoint_adopt`): two requests on one 100-token system prompt, a different 30-token
  user turn each, through a one-stream device-home scheduler and a paged host-cached one, both marking the system prompt's
  end as their stable opening - the device-home stream's region keeps its recurrent state there (a checkpoint row), the
  second request adopts it off the parked claim (100 tokens cached, 160 computed across the run, the same as the
  host-cached run's), and both requests' tokens are the host-cached run's;
- the pool cell (`cell_device_pool`, `../ARCHITECTURE_ENGINE_SERVING.md#device-home-pool`): five requests one after another on
  a two-stream scheduler over three 100-token system prompts - more openings than the two regions hold - the fourth on the
  first's system prompt after two other streams took both regions, the fifth the second conversation's next turn after its
  region went to the fourth: on the paged device-home scheduler the fourth takes its system prompt (100 tokens) and the
  fifth the whole earlier turn (138) up from the host pool, two imports counted (`gpu_device_kv_pool_moves`), and exactly
  the copies the card was about to lose went down - each system prompt as its stream parked (a recurrent model), each
  finished turn as its region went to another stream, the last two turns not at all (6 copies on a recurrent model, 3 on a
  dense one) - every request token for token the host-cached paged scheduler's, whose prefix cache keeps the same openings;
  the flat device-home scheduler, which keeps no pool, is the control - both prefill whole; and on a recurrent model a pool
  capped at four checkpoints keeps the system prompts (openings) over the finished turns (tails), dropping the two oldest
  tails, so the fifth request finds its system prompt alone;
- the speculating pool cell (`cell_device_pool_mtp`): the plain device-home scheduler first as the control (nothing drafted,
  the two openings up), then the host-cached and the device-home pooled speculating schedulers - the same two openings up,
  every request token for token and as many rounds drafted and accepted;
- the head-rows cell (`cell_device_pool_head_rows`): a device-home session's opening parked, exported to a paged carrier and
  imported into a fresh device-home session, which counts the head's rows to the position less one and drafts the logits
  the session that went down drafts; it and the speculating pool cell skip under `DASLLAMA_DEVICE_POOL=0`, without the file,
  on a carrier with no draft head, or where the driver does not run the whole speculative round on the device;
- the guards (`cell_device_home_guards`): a third device-home session over two regions panics with its `busy` counted and
  the owners step on untouched;
- the driver's reports (`cell_driver_reports`): every pass reason reads in words of its own, the plan's K/V scales with the
  region count and a count under one plans one, the tier status counts both mirror sides, a load under a switched-off route
  says `not attempted`;
- the two passes (`cell_passes_with_their_rows`), each bringing its rows down first: a prefill that skips positions
  (`gap`) and one past the region's context (`cap`, on a load capped at 4096 positions);
- the long-context cells, on the file's `LongCtx` shape: the slab cell (`cell_regions_slabbed_mirror`), the CPU-chain cell
  (`cell_long_prompt_against_cpu`), the continuation (`cell_long_continuation`), the streams (`cell_long_streams`) and the
  rows through the host (`cell_long_rows_through_host`); each long-context entry gives its numbers. The long-context cells
  assert their load armed the rings and the mirror under the flat plan's; the slab cell pins the ring off, since its slab
  count is the flat mirror's.

`test_gpu_resident_regions.das` - the qwen2 file, Qwen2.5-0.5B-Instruct Q8_0: the handover cell, the interleaved single
steps, the two batched-step cells, the device-home cells, the scheduler's device mode, the handoff cell, the prefill-chunks
cell, the tolerance cells at the 6% bar of `test_gpu_resident_qwen2.das`, the paged cell, the pool cell, the guards, the driver's reports and the two
passes.
`test_gpu_resident_regions_q8kv.das` - `test_gpu_resident_regions.das`'s carrier with the K/V mirror asked in q8_0 (an `[init]` pins it
through `set_gpu_kv_dtype`; the sessions take the armed codec, `mirror_kv_dtype`): the interleaved single steps, the two
batched-step cells, the device-home cells, the scheduler's device mode, the handoff cell, the prefill-chunks cell, the
tolerance cells, the guards, the driver's reports and the two passes - the staged stores and the codec's store pass at one
row and at N rows, the decode attention's codec stamps, the prefill's attention over the f16 shadow, the rows a pass
brings down as the CPU cache's own bytes.
`test_gpu_resident_regions_tq4kv.das` - the same cells on Qwen3-0.6B Q8_0 (the qwen3 file's carrier: Qwen2.5's Q/K/V
bias serves a tq4 ask at q8_0) with the mirror asked in tq4 (the interleaved single steps,
the two batched-step cells, the device-home cells, the scheduler's device mode, the handoff cell, the prefill-chunks cell,
the tolerance cells, the guards, the driver's reports and the two passes), tq4's un-rotation among what they reach.
Under a tq4 mirror no share of the peak holds a tolerance cell - two window splits of one prompt, and the split q/k
norm against the fused one on the qwen3 file, read as far apart as a bit-for-bit carrier's row one off - so the rig holds each such row to a share of
its distance from the one-token-off row instead (`tq4_within`, the share `TQ4_CTRL_SHARE` in `_resident_feed.das`: above
the widest reading on any card the suite runs on - the 900-token prompt cut at 300 reads widest - and under 1.0, the
one-off row's own distance; the readings per card sit in the constant's `//!`), and every bit-for-bit cell stays bit for
bit.
`test_gpu_resident_regions_qwen3.das` - the qwen3 file, Qwen3-0.6B Q8_0, a q/k-norm carrier: the interleaved single steps,
the two batched-step cells - the N-row command's q/k norm in whichever form the one-row command takes, the fused norm + rope
+ store or the split pair - bit for bit, the batched-first cell, the wide single steps, the fuse bisect, and the window's
fused q/k norm cell.
`test_gpu_resident_regions_phi3.das` - Phi-3.5-mini Q4_K_M: the two batched-step cells on a K-quant carrier, held to the
split bar with the one-token-off control rather than bit for bit.
`test_gpu_resident_regions_llama_k.das` - the llama file, Llama-3.2-1B Q4_K_M: the two batched-step cells on a K-quant
carrier - the N-row command's K-quant GEMV leaves and its split Q8_K sites - held to the split bar with the one-token-off
control rather than bit for bit, since those sites round apart from the one-row command's; and the groups cell twice -
twelve sessions over twelve regions, past the N-column leaves' eight-column tile, every batched step one command whose
GEMVs run a tile of eight columns and one of four, and twenty over twenty, more rows than the N-row command's sixteen, every
batched step two commands of ten (`rdec_nrow_steps` up by the group count a step, never a row at a time; the sessions
alone, stepped through the one-row command first, leave the counter where it was - its control), each row within the split
bar of its session alone.
`test_gpu_resident_regions_hybrid.das` - the hybrid file, Qwen3.5-0.8B Q8_0, its speculating pool cells loading the
carrier's -MTP- twin beside it. Each recurrent layer keeps one device state slot per region, so the two sessions' batched
steps advance their own deltanet states through the N-row command's recurrent form, the gated q - a q projection row that
carries the attention's output gate beside q - running over the rows; before a session's prefill zeroes its region's slot,
it copies back to the host whatever state another session stepped in that slot and had not yet copied back. Its cells: the
handover cell, the interleaved single steps, the batched-step cell - which copies session B's state back after the steps
and holds that it moved - the batched-first cell, the device-home cells, the scheduler's device mode, the handoff cell, the
tolerance cells' continuation form (its window chain starts from the region's live recurrent state, a 2-row tail under the
conv taps among the cuts - the ring's leading rows off the history it continues - the recurrent position asserted at the
prompt's end), the recurrent parked claim, the prefill-chunks cell, the paged cell, the checkpoint cell, the pool cell, the
speculating pool cell (on Qwen3.5-0.8B-MTP Q8_0) and the head-rows cell.
`test_gpu_resident_regions_hybrid_k.das` - Qwen3.5-9B-MTP UD-Q5_K_XL (large tier): the two batched-step cells on the split
bar with the one-token-off control, on a deltanet hybrid whose recurrent projections are K-quant planes, so every recurrent
head takes the rows' Q8_K feed; and the head-rows cell, on the carrier's K-quant head planes.
`test_gpu_resident_regions_hybrid_iq3s.das` - Qwen3.8-27B i1-IQ3_S (large tier): the two batched-step cells on the split
bar with the one-token-off control, on a deltanet hybrid whose FFN and attention q/k/o planes are IQ3_S (the recurrent qkv
and attention v planes Q4_K), so the grid format's N-column GEMV leaf serves beside the k4 leaf at every batched site.
`test_gpu_resident_regions_hybrid_q3kxl.das` - Qwen3.8-27B UD-Q3_K_XL (large tier): the two batched-step cells on the split
bar with the one-token-off control, on the hybrid whose planes mix Q3_K, Q2_K, IQ3_S, IQ3_XXS, IQ2_S, IQ2_XS, IQ2_XXS and
IQ4_NL with IQ4_XS, Q4_K through Q6_K and Q8_0, so one load runs every low-format N-column leaf - the k-lattice and the
grid decodes - under the rows form.
`test_gpu_resident_regions_e2b.das` - the E2B file, gemma-4-E2B Q8_0, the E-series carrier: the interleaved single steps,
the two batched-step cells, the split-forms cell, the side-rows cell, the embed-gather cell, the groups cell at nine rows
(one command, its GEMVs and the per-layer-embedding act in a tile of eight columns and one of one, each row's side row read
from its own row base), the device-home cells, the scheduler's device mode, the handoff cell, and the window's fused q/k
norm cell - a sharer's q heads on the fused pass with no k heads, at the wide bar, since the two norm orders put the
E-series' fed steps apart past the split bar (the readings in the cell's `//!`).
`test_gpu_resident_regions_gemma2.das` - gemma-2-2b Q8_0: the two batched-step cells and the batched-first cell on a carrier
with the attention softcap and the classifier epilogue - the N-row command's epilogue over every row's logits - plus the
stamp witness (`cell_regions_stamp_served`), a step whose census count shows the two-head attention slab served (a group of
two heads); the stamp cell skips under the CUDA ask (the slab pick and the census names are the Vulkan driver's) and where
the census holds no such stamp.
`test_gpu_resident_regions_gemma3.das` - gemma-3-1b Q8_0: the two batched-step cells and the batched-first cell on the
gemma-3 dense base bit for bit, and the ring floor cell.
`test_gpu_resident_regions_gemma4.das` - gemma-4-12B Q8_0, large tier (`DASLLAMA_PARITY_FULL=1`): the two batched-step cells
and the batched-first cell on a carrier with the epilogue's suppressed ids, V-from-K global layers at a head of 512 and a
per-layer output scale - the pins themselves are `test_vkd_cls_epi_rows`' witness, since the batch driver's host tail
re-pins every row.
`test_gpu_resident_regions_qwen2moe.das` - Qwen1.5-MoE-A2.7B Q8_0, large tier: the two batched-step cells on a MoE carrier
with Q8_0 unbiased experts beside a shared expert (the routed block's plain act and the shared expert's Q8_0 requant of the
routed feed) bit for bit.
`test_gpu_resident_regions_qwen3moe.das` - Qwen3-30B-A3B Q4_K_M, large tier: the two batched-step cells on a MoE carrier with
no shared expert, K-quant stacks fed by the rows' Q8_K requant, held to the wide bar with the one-token-off control (the
K-quant projection leaves round apart from the one-row command's, and a MoE's expert planes carry that rounding through
every layer - the bar's reading is `_resident_regions.das`'s).
`test_gpu_resident_regions_qwen35moe.das` - Qwen3.6-35B-A3B-MTP-IQ4_XS-pure, large tier, the routed deltanet hybrid with its
NextN head: the tolerance cells' continuation form at the routed chain's 20% bar (the prefill counter up by every call), the
groups cell over ten sessions on the wide bar, the greedy chain forced on bit for bit the plain walk (`greedy_chain_cell`:
every on-pick step a chain hit), short tails peeled through the token commands (`recurrent_peel_cell`, the peel counter,
speculation off), and the pool cell.
`test_gpu_resident_regions_gptoss.das` - gpt-oss-20b mxfp4, large tier: the two batched-step cells, the batched-first cell
and the widening cell on a MoE carrier - the N-row command's routed block (the router's columns, the per-row top-k, the
expert GEMVs over the rows' slots, the combine per row) on the native-MXFP4 stacks with the biased router, the biased act
and the attention sinks.
`test_gpu_resident_regions_gemma4moe.das` - gemma-4-26B-A4B Q4_K_M, large tier, the gemma-4 MoE form (the routed feed and
the router off their own norms, the parallel shared expert, the folded down scale, the combine over both branches): the
split-forms cell and the widening cell on the wide bar with the one-token-off control, the one-row command's fused forms
pinned off for the load, then the fold cell on the same bar, and the embed-gather cell.
`test_gpu_resident_regions_llama3b_ctx32k.das` - Llama-3.2-3B Q4_K_M at four regions of 32768 positions: the 28 layers' f16
rows at that context run past one binding's range, so the K/V mirror seats them in two or more slab pairs
(`../ARCHITECTURE_GPU_VULKAN_RESIDENCY.md#resident-plan`). The slab cell asserts the slab count, the region count and the
unshortened region context, then runs the interleaved single steps (the N-row counter's control: they move it by nothing)
and the batched steps on the split bar; the long-context cells on the same load: a 12288-token prompt and eight steps
against the CPU chain at the 10% bar with the one-step-off control, a 30000-token prompt continued from 20480 against the
one-call prefill within the split bar, and four device-home streams at 9175 / 15073 / 20971 / 26624 positions (the context
times 28, 46, 64 and 82 hundredths), each prefill bit for bit the host-cached session's and a batched step over all four at
every step within the split bar of each alone. The mirror codec is pinned f16 for the file; it skips on a card whose room
does not hold the 15 GB mirror.
`test_gpu_resident_regions_gemma3_4b_ctx128k.das` - gemma-3-4b Q8_0 at two regions of 131072 - the model's whole context -
on a q8_0 mirror (pinned for the file): seventeen slab pairs under a block codec's byte span (the least-charge cut of 34
uniform layers), the window chain's store and shadow sets per slab, and the five long-context cells - the slab cell's short
prompts bit for bit, the CPU-chain cell at the gemma 16% bar, the continuation 120000 from 65536, two streams at 36700 and
60293 within the split bar (past the wide position the one-row command's wide twin and the N-row command's split pieces sum
the keys in another order), the rows through the host.
`test_gpu_resident_regions_gemma4_e2b_ctx128k.das` - gemma-4-E2B Q8_0 at four regions of 131072 on an f16 mirror: the
fifteen owners in two slab pairs with every sharer on its donor's seat, the five long-context cells, four streams at 36700
to 107479 within 14% of the max logit - past the wide position the E-series' batched rows sit as far from the session alone as
its own device-vs-CPU rows, bit for bit the same on the ringed and the flat mirror (the readings in the file's header).
`test_gpu_resident_gemma*.das` (`_gemma_resident.das` carries the cells; one model a file:
`gemma3_1b`, `gemma3_4b`, `gemma2`, `gemma4_12b_q8`, `gemma4_12b_k`, `gemma4_e2b`, `gemma4_e4b`,
`gemma4_26b`, `gemma4_26b_k`, `gemma4_26b_mx4`, `gemma4_26b_host`, `gemma4_31b` - a process loads one carrier, so no cell inherits another model's device state, and a GPU run
loads ONE model at a time, never a chain) - stocked suite, `-jit` only; the whole-model resident
driver on the gemma dense base (gemma-3-1b-it-Q8_0, `DASLLAMA_GPU=1`): the sandwich norms (the residual steps
norm their add partner first), the sliding-window layers beside the global ones with their own rope
class, the GeGLU FFN and the sqrt(dim) embed scale - the hybrid file's forced-feed logits-tolerance
form at the 16% bar this model's rounding sensitivity sets (its CPU chain's own prefill and decode
kernels land 6-10% of the max logit apart on the same tokens; the resident prefill feeds the GEMMs
f16 rows where the CPU quantizes Q8 blocks), with the one-step-off control (reads 5 to 19), at one
window, eight tokens, 520 tokens (two windows, the window engaged on the last eight positions) and
600 tokens (every sliding layer masking below its window on the prefill tiles and the decode
chunks); plus the agreement cells - the window chain and the token command on the same eight and
forty tokens, held within 6x of the CPU chain's own two paths' gap. Every cell feeds ordinary
prose through the model's own tokenizer (`prose_tokens`), never a synthetic id ramp: on a ramp
gemma's logits saturate the softcap and the CPU chain's own prefill and decode land twenty points
apart at 300 positions, so a bar on them measures nothing. The gemma-4 dense rows (large tier,
`DASLLAMA_PARITY_FULL=1`): gemma-4-12B Q8_0 in the forced-feed form at forty tokens (the weightless
v-norm, the V-from-K global layers at head 512 beside the sliding 256s, the per-layer output scale,
the final softcap and the suppressed ids - the bar's max logit skips the pinned ids) and in the
perplexity form (`ppl_compare`: `n` ids prefilled, `steps` teacher-forced, the resident's
perplexity within 5% of the CPU chain's and its argmax hits within three, the one-position-off
targets the control) at 150 + 150 and at 520 + 80 (two prefill windows, the second eight rows
deep; this cell's ratio is 1.3: over its 80 targets the CPU chain reads 1.97, the resident 2.37
and llama.cpp b10660 2.51, hits 72, 72 and 71, so the resident sits between the two references);
the 12B Q4_K_M in the agreement form at eight and 300 tokens. The perplexity form exists
because the 12B's logits sit within a few points of each other at whole stretches of positions
(llama.cpp's own margins on the same text read 0.6 to 16), so a maxdiff bar at one position
measures the model's flatness, not the driver; the CPU chain's own two paths land 13% to 44% of
the max logit apart there. The E-series rows: gemma-4-E2B Q8_0 (stocked tier) in the forced-feed
form at forty tokens, the perplexity form at 150 + 150 and at 520 + 80 (two windows, the 512
sliding window engaged on the last eight) and the agreement form at eight tokens - the per-layer
embedding branch after every FFN residual (the pre-step's side rows built on the device - the
token command gathers its row off the token id - and riding the token command and the window chain), twenty layers attending over the K/V of the fifteen below them, and the two
dense FFN widths; gemma-4-E4B Q8_0 (large tier) at forty tokens and 150 + 150; gemma-3-4b Q8_0
(the dense base at width 2560 and four kv heads) at forty tokens and 150 + 150; gemma-2-2b Q8_0
(the attention logit softcap on every score - the 8-row prefill tile and the token command take
it, and the cm2 flash tile's softcap stamps at its 256-wide heads) at forty tokens, 150 + 150 and
the eight-token agreement form. The MoE row (large tier): gemma-4-26B-A4B UD-IQ3_XXS - the
routed block in its gemma-4 form (the parallel dense shared expert as the layer's own FFN triple,
the routed feed and the router off their own norms of x, the per-expert down scale folded into
the routing weights, the combine norming both branches and their sum under the layer's output
scale) in the perplexity form at 150 + 150 and at 520 + 80 (two windows) with an argmax slack of
twelve (sixteen on the Q4_K_M file, whose hits move with a kernel recompile) and a perplexity ratio
of 2.0 on both files' 150 + 150 cells and the IQ3_XXS file's 520 + 80 cell, 2.5 on the Q4_K_M
file's 520 + 80 cell. The ratio holds a band, not a point: a kernel recompile moves the reading with
the CPU chain fixed - the kernels are bit-identical at this model's width in the kernel cells, show
no out-of-range access under the validation layer's GPU-assisted mode and read the same with a
barrier before every dispatch, so the drift is the device compiler's rounding per module, not a
race - and a router near-tie flips whole positions between the arms, either arm landing the
farther one, so a forced-feed maxdiff against the CPU chain is no instrument here. Its cells run on
a 32 GB card and skip on a smaller one's memory decline. The IQ3_XXS file's twin
`test_gpu_resident_gemma4_26b_k.das` runs the same two perplexity cells on the model table's
official UD-Q4_K_M file, whose 704-wide down stacks are native Q5_1 and ride the q51 expert rail
(the s and e stamps, the Q8_0-activation decode GEMV) beside the k4 gate and up stacks.
`test_gpu_resident_gemma4_26b_host.das` holds the 26B UD-IQ3_XXS file under an 8000 MB plan room with the rig's context
asked whole: the plan moves the first routed layers' experts to the host (`gpu_resident_host_layers` past zero and short of
every layer), the device keeping the dense branch, the norms, the router and the routed feed
(`../ARCHITECTURE_GPU_VULKAN_MOE.md#vk-gemma4-moe-block`), and the 150 + 150 perplexity cell holds the CPU chain at the same
ratio and slack with every call served (`served_every_call`), and a child load under the same room (`resident_load_log`) logs
the pool sized by the room alone - armed, or declined with the MB left past the plan, never with the gemma-4 form as the reason (its hits carry the down scale folded into their weights, as the host's misses do); the cap binds the device's weight budget at the tier's first
init, so the cell has its own file, and it skips before the load where the cap does not decide the plan -
`DASLLAMA_GPU_VRAM_MB` set to another room, or a device room under 8000 MB. `test_gpu_resident_gemma4_26b_mx4.das` (large
tier) holds the MXFP4_MOE mint of the Q4_K_M file (`../performance/model_specs.das`): the format cell and the 150 + 150
perplexity cell at the same ratio, slack 12. `test_gpu_resident_gemma4_31b.das` holds the largest dense carrier, gemma-4-31B Q4_K_M
(large tier, a 32 GB card), in the 12B Q4_K_M's agreement form at eight tokens - one cell, since the
CPU control of a dense 31B on the reference kernel bodies runs half an hour on an untuned box. The arms and their
substring cases: "Arm filter mechanics" above. The three 26B files open on the
format cell (`gemma_format_cell`): every site the file carries in its own format, the expert stacks the native-MXFP4 planes
exactly on the mint, then a 40-token prompt and two fed steps on the driver, every call served, and on the mint the fed steps'
mx4 fused gate + up and summed down stamps counted (`mx4_gemv_gu_cls_spv`, `mx4_gemv_sum_cls_spv`), the two other files
dispatching neither (the stamps' control). Every feed and perplexity
cell of the rig asserts the device served every call (`served_every_call`); a memory decline skips and logs the decline. Skips
without the model or the armed tier, and on a memory decline (`moe_gpu_resident_memory_decline`:
the plan did not fit the card at the session's context - the 12B Q8_0 file on a 16 GB card arms
under `DASLLAMA_GPU_MIN_CTX=1024`); a feature decline stays a red. Two files carry block-codec
cells, each pinning the mirror's codec around the rig's own cell (`with_gpu_kv_dtype`,
`_resident_feed.das`: the ask put back as found, a skip where the environment arms another codec;
the rig asserts the mirror armed at the codec asked): `test_gpu_resident_gemma3_block_codecs` runs
gemma-3-1b at 1100 tokens - a third prefill window, which starts past the 512-key sliding window,
so the f16 shadow of a sliding layer starts a 64-key step under its window - in the forced-feed
form under q8_0 (reads 0.46-1.37 against bars of 3.3-5.1) and the perplexity form (1100 + 80)
under tq4 (reads 1.022 against the CPU chain's 1.033, 80 of 80 hits; a 4-bit mirror reads one step
5.0 of a 33.7 max logit from the CPU's, so no logit bar holds it with room); a shadow that starts
at the window's first row reads a perplexity of 13844. `test_gpu_resident_gemma4_e2b_short_window` peels short tails of 1, 2 and 5 rows on the E2B through the token commands, the side rows riding them, within the split bar of the same tail stepped a row at a time. `test_gpu_resident_gemma4_e2b_block_codecs`
runs the shared-KV carrier - its upper layers store no K/V row of their own, and under tq4 still
rotate their query - at forty tokens in the forced-feed form under q8_0 and at 150 + 150 in the
perplexity form under tq4 (reads 1.032 against 1.032, 148 of 150 hits each).
`test_gpu_resident_gemma3_ring` and `test_gpu_resident_gemma4_e2b_ring` hold the sliding layers' rings
(`../ARCHITECTURE_GPU_KV_RING.md#kv-sliding-ring`, `ring_vs_flat` in `_resident_feed.das`): 1300 prose ids past the window and
eight fed steps on a ringed load and on a flat one (`set_gpu_kv_ring`, put back) - the logits bit for bit at the prompt and every
step, the ringed load arming rings and charging less mirror, the row a step off the control - on the f16 and q8_0 mirrors, and
tq4 too on the E2B. The E2B's ring test also reads the driver's load log (`ring_load_announces`: two loads in a child,
`_resident_load_root.das`, which `resident_load_log` spawns through `_child_run.das`; `-- <path> <ctx> [step]`, where `step` also
prefills a short prompt and decodes one token through the armed driver) - the ringed load naming its rings, the flat
one under `DASLLAMA_GPU_KV_RING=0` its every-row mirror, the host embed under `DASLLAMA_VK_GPU_EMBED=0` saying so and the
gathering load not. Arm `g4e-ple`
(`test_gpu_resident_gemma4_e2b_ple_tables`): the E2B Q8_0 file and its emb-Q5_0 / emb-Q5_1 / emb-Q4_0 / emb-Q6_K / emb-IQ4_XS
mints - the plan seats the PLE table in its own tag (q8, q5032, q51, q40, k6, iq4xs) and the forced feed's prefill gathers the side
rows off it against the CPU chain, the format's gather leaf counted (`<fmt>_emb_cls_spv` / `kq_emb_<fmt>_cls_spv`) and every
other format's leaf unmoved - on the Q8_0 file every one, the leaves' control.
Arm `g4e-kq` (`test_gpu_resident_gemma4_e2b_ple_planes`): the E2B Q4_K_M (the per-layer gate and projection planes F32 on disk,
read as the file's f32 rows; the feed held against the file's f32 chain with the same-or-better witness, `gemma_forced_feed`'s `exact` form,
at 1.15 on the scalar f32 tile and 1.3 where the window's f32 GEMMs ride the staged KHR tile (its f16 weights and sums),
since its CPU q8 chain reads one step's top logit under the f32 chain, the reference build and the device; the reference's
recipe and the readings: `../ARCHITECTURE_MEASUREMENT_FORCED_FEED.md#reference-forced-feed`), Q4_0 (the
planes Q4_0, arm `g4e-kq-q40`) and the ple-Q6_K / ple-Q5_0 mints (arms `g4e-kq-ple-q6k` / `g4e-kq-ple-q50`) in the forced feed, the
planes' tags asserted against the disk type and the format's decode GEMV counted over the fed steps (`kq_gemv_q40`, `kq_gemv_k6`,
`q5032_gemv`, the one-row and N-column leaves summed); the Q8_0 file (arm `g4e-kq-q8`) dispatches none of the three, their
control. Arm `g4e-pre` (`test_gpu_resident_gemma4_e2b_ple_pre_step`): the token command's side row - gathered, projected and
finished on the device - read back (`vk_rdec_ple_side_row`) within 1e-4 of the CPU pre-step's row, another token's row the
control, the step's row gathered off its id (`vk_rdec_token_gathered`), and a load with the gather off
(`set_gpu_resident_embed(0)`) gathering none - the counter's control. Arm `g4e-f16` (`test_gpu_resident_gemma4_e2b_tied_f16`): the `-embf16-local` mint's tied F16 token table declines the
whole-model driver by name (`tied classifier`), the PLE table tagged q8. Arm `g4f-codec` (`test_gpu_resident_gemma4_e4b_block_codecs`):
the E4B under a q8_0 mirror in the forced-feed form and a tq4 mirror in the perplexity form. Arm `g4e-chain`
(`test_gpu_resident_gemma4_e2b_greedy_chain`): the greedy chain (`greedy_chain_cell`) on the E2B, each chained step's side row
gathered off the pick the device lands, the chained walk bit for bit the plain walk, both walks decoded in the log.
`test_gpu_resident_moe.das` (`_moe_resident.das` carries the rig, shared with
`test_gpu_resident_gptoss.das`) - stocked suite, `-jit` only; the rig reads the armed discrete home's witnesses (the split count,
the unsplit twin's tokens, the kernel census under the home's stamp names), so both files run on the CUDA home under
`DASLLAMA_CUDA=1`, where the per-op heat cells skip by name; the census fixtures (`MoeFixture.census`: the two UD-IQ2_XXS
files and the Q4_K_M host-layer file) open on the format census, every site the file carries in its own format, and
Qwen3-Coder-Next Q4_K_M holds the 512-expert device select beside the host-layer split; arm `vk-per32-moe` (family
`qwen2moe`, large tier) runs the one-window cell on the Qwen1.5-MoE Q5_1, Q4_0 and Q5_0 `--pure` mints - every site native,
the q51 shared expert and routed stacks, the q4032 routed downs, the q5032 shared expert and routed stacks; the whole-model resident driver on a MoE
(Qwen1.5-MoE-A2.7B-Chat-Q4_K_M-local, `DASLLAMA_GPU=1`): the expert stacks in the arena, the window
chain's routed block and the token command's routed block - the hybrid file's forced-feed
logits-tolerance form at the routed chain's 20% bar (the arms part on the router's near-ties from
layer 1 on; a flipped near-tie on the 35B at two windows reads 0.155-0.16 of the step's max logit
under either of two summation orders, the one-step-off controls 0.42 and above; the bar's `//!`
carries the reading) with the one-step-off control, at one window and
two windows, plus the census witnesses: the device bucket schedule and the per-row select ran once
per MoE layer per window, the expert schedule's m pieces dispatched the format's e column on the
three expert planes once per MoE layer per window in cm2 mode - exact where the format keeps its
own e stamp, a floor where its e column is its m stamp (`KQ_CM2E_ALIASES_M`), since that stamp also
serves the window's dense GEMMs (in mm mode the KHR tile serves them too, so its count is a floor of
two dispatches a plane per MoE layer per window; the count sums the column's stamp of every `KqFmt`
member, so a plane the box serves in another form - IQ3_S as `iq3s4` - is counted), the token command's top-k count a whole multiple of the MoE layer count (the command
records once and resubmits), and on a device whose SM count splits the attention keys the
unsplit twin served every step of the one-window cell and none of the two-window one; the second
fixture is the Qwen3.6-35B-A3B UD-IQ2_XXS hybrid, whose recurrent layers take the routed block
after the deltanet head, at the same two lengths; the third is the Qwen3-30B-A3B UD-IQ2_XXS,
the MoE with no shared expert (the residual step with its add partner off, the FFN-norm requant
skipped), at the same two lengths; all five fixtures are large-tier (`DASLLAMA_PARITY_FULL=1`), and the
cells skip without the file or the armed tier; the driver's admission of the fixture is asserted, a decline
is a red that sends the reader to the load log - on a device with neither the cm2 tile family nor KHR cooperative
matrices at subgroup 32, and under `DASLLAMA_COOPMAT=sdot4`, the expert pieces ride the Q8-fed chain's batch tiles
(`../ARCHITECTURE_GPU_VULKAN_MOE.md#vk-moe-q8-chain`), the column witness counting their stamps. Every cell pins the resident
route on, one region, and the context pins (`set_gpu_ctx_max` / `set_gpu_ctx_strict`, at their default for
the fixtures with no asked context) for its load and restores the levers after through their getters, and
every cell asserts the host-layer count `gpu_resident_host_layers` reads - zero where the card holds the
experts whole and the fixture asks none (the load pins the ask at the fixture's, 0 included, so
`DASLLAMA_GPU_HOST_LAYERS` never reaches it), the red naming the environment knobs that override the pins. Three fixtures take the
host-layer form: `test_gpu_resident_moe_host_layers` on the 17 GB Qwen3-30B-A3B Q4_K_M with 32768
positions asked whole, which a 16 GB card does not hold beside every expert, so the plan moves the first
routed layers' experts to the host (the cell asserts not all did; it skips on a card that holds every stack,
and on one too small for the mirror and the dense planes, by `moe_gpu_resident_memory_decline`)
and the split token command and the cut window chain run against the CPU at both lengths, the two-window
cell pinning the window (`assert_pf_window`), and its third cell the same load asking 30 routed layers onto
the host (`MoeFixture.host_layers_ask`, `set_gpu_host_layers`) - past the count the room forces - asserting at
least 30 serve there; `test_gpu_resident_moe_ctx_strict` on the Qwen3.6-35B
UD-IQ2_XXS hybrid with the context asked whole (`set_gpu_ctx_max(131072)` + `set_gpu_ctx_strict`, the
pins read first and put back after the load), asserting the armed region holds exactly that context
and experts yielded to it. The census witnesses count the device's routed blocks (the layers less the
host ones), and the unsplit-twin witness is skipped under host layers (the split command records the
split form alone). `test_gpu_resident_moe_host_layers_short_window` holds a two-token prompt on the host-layers fixture to the window chain (the peel declines routed experts on the host). `test_gpu_resident_moe_block_codecs` runs the twin at one window with the
mirror asked at q8_0 (`with_gpu_kv_dtype`; the rig asserts the mirror armed at the codec
asked): the forced-feed cell on q8_0 sessions at the same 20% bar (reads 0.83-1.17
against bars of 2.1-2.3, the f16 mirror's own reading); the twin's serving-rule cell - its attention carries a Q/K/V
bias, so a tq4 ask arms the q8_0 mirror and a session's codec (`kv_codec_for`) steps down the same way, the driver
armed (it skips under `DASLLAMA_VK_KV32=1`, which arms the f32 mirror over the codec asked); and the tq4 leg on
Qwen3-30B-A3B UD-IQ2_XXS in the codec rig's distance form (`moe_codec_distance` over `codec_distance_over`) - the
device's summed distance from the f16 chain within 1.15 of the CPU tq4 chain's own and the two tq4 chains within 1.4 of
that distance of each other (the router's near-ties part two tq4 chains as far as tq4 parts either from f16).
`test_gpu_resident_moe_host_rows.das` - stocked suite, `-jit` only, `DASLLAMA_GPU=1`, large tier; the host-layers form's K/V
mirror on Qwen3-30B-A3B Q4_K_M at 32768 positions asked whole (the first routed layers' experts on the host, the window chain cut
at them): layer 0's stored rows after a 40-token prompt and the fed steps against the CPU cache's through the codec rig's
stored-rows cell - whether the cut window chain and the split token command write the mirror the decode reads.
`test_gpu_resident_moe_host_pinned.das` - stocked suite, `-jit` only, `DASLLAMA_GPU=1`, large tier; the host-layers form reached
by a pin on three carriers (Qwen1.5-MoE Q8_0 and its Q4_K_M twin, Qwen3-30B UD-IQ2_XXS; `MoeFixture.host_layers_ask` 4): the split
token command and the cut window chain against the CPU chain at one window and two, and the stream twin (`stream_twin_within`):
the streamed windows' rows against the same load with the stream off at the stream twin's bar, the device claiming first.
`test_gpu_resident_moe_formats.das` - stocked suite, `-jit` only, `DASLLAMA_GPU=1`; the host twin's model-level matrix on
the smallest MoE: Qwen1.5-MoE-A2.7B-Chat's routed experts requantized to one format a file (`moe-formats/<file>-exps-<TYPE>.gguf`
under the models dir, a junction to another drive on the reference box; the rest Q8_0; the down stacks' 1408-wide rows take the
type's per-32 fallback where a superblock type is asked - Q4_K's down is Q5_0), four routed layers' experts pinned to the host
under the streamed prompt (`set_gpu_want_pf_stream`), the rare rail reading them off the tier's pinned device-layout planes through
the host twin (`set_rare_arena`): the host-pinned file's two cells a format - the split token command and the cut window chain
against the CPU chain, one window and two, one `[test]` a format (dastest's `--test-names test_moe_formats_<TYPE>` runs one;
`test_moe_formats_NVFP4_35B` runs the two cells on Qwen3.6-35B-A3B's own NVFP4 file, large tier, the nv4 row-major kernels),
the meet's host share forced through `set_moe_host_first` and witnessed by its counter, since a small carrier's device
ring would otherwise take every expert, and the fed steps' host picks witnessed off the device planes too (`g_rare_dec_rows`:
the one-row rail); `test_moe_formats_hot_pool` puts twenty of the Q4_K file's layers on the host, so the plan's room arms the
unasked pool, and holds the pool's slots filled off the layers' pinned mirrors (`g_rare_slot_copies`) with its uploads counted;
`test_moe_formats_expert_cache` streams twenty of the Q4_K file's routed layers under a 3500 MB expert cache (`set_expert_cache_mb`)
so about half of each layer's experts stay resident, holds the token command against the CPU chain with the misses read off the
lane, and runs the same feed twice to hold that the heat profile the drop writes beside the lane (`<lane>.heat`) makes the
second feed read fewer planes; it needs the lane file, so it skips under `DASLLAMA_IMAGE=0`.
`test_moe_formats_knobs_<TYPE>` (Q8_0, Q4_K, IQ4_XS) pins the pool's knobs in process over twenty host layers, four cells a
type: the fill's share first (`set_gpu_hot_share(1.0)` over a pool pinned to `SHARE_CELL_SLOTS` of the carrier's 60 experts
through `g_gpu_want.heat`, so the pool fills and the heat gate has misses to leave; `share_seen` up and `share_fills ==
share_seen` the witness, since every expert of the carrier is a copy the tier can make, the bar the proof; a pool armed at
the full count skips, since it never fills), the DMA fill (`set_gpu_hot_dma`, the misses copied into the pool on the transfer
queue, `gpu_resident_hot_stats().dma` the witness), the fill's no-route leg (the DMA fill and the meet pinned off: `dma` stays
0 over the cell, the host's `uploads` rises - the meet's claims ride DMA whatever the knob says, so it is off too) and the
decode meet (`set_gpu_dec_meet`, a token's picks through the prefill's meet, `meet_steps` the witness - the one-row rail's
counter stays, since the meet replaces it); the pool's counters are the armed driver's, zeroed at its arm inside the feed's
load, so the cells read them absolute; the three pool cells skip under `DASLLAMA_GPU_HEAT=0`, which asks no pool.
`test_moe_formats_rail_arms` runs the Q4_K file with each rail lever off in turn (`set_rare_onepass(0)`, `set_rare_batched(0)`,
`set_rare_arena(0)`) against the same bar, so the A/B arms stay honest.
Run the format cells under `DASLLAMA_IMAGE=0` - a dozen carriers would mint a dozen images beside them,
and the planes in RAM reach the same kernels. A file not minted skips; the mint recipe is `llama-quantize --allow-requantize
--tensor-type ffn_gate_exps=<TYPE> --tensor-type ffn_up_exps=<TYPE> --tensor-type ffn_down_exps=<TYPE> <Q8_0> <out> Q8_0`. The
IQ2 types and IQ3_XXS need an importance matrix: `llama-imatrix -m <Q8_0> -f <a text> -o <file>.imatrix -ngl 99 -c 512 --chunks 64`
(a prebuilt llama.cpp release's, where the box's build lacks the tool; under a minute on the card), then the same recipe with
`--imatrix <file>`.
`test_gpu_resident_gptoss.das` - stocked suite, `-jit` only; the same rig on gpt-oss-20b (the pinned
upstream `gpt-oss-20b-mxfp4.gguf`, large-tier; the sink stamps of the K/V codecs in `test_gpu_resident_gptoss_sink_codecs` -
the q8_0 mirror and, under `DASLLAMA_VK_KV32=1`, the f32 mirror in the forced-feed form at an 8192 context; no tq4 arm, since
the carrier's Q/K/V bias steps a tq4 ask down to q8_0): the native-MXFP4 routed stacks on the mx4 expert
rail, the biased router's softmax over its four picks, the gate / up / down bias rows at the act and
the combine under the clamped swiglu, the per-head attention sinks in the flash tiles and the token
command's pass, the output bias on the residual step and the 128-key sliding layers, at one window
and two; the e-column witness counts the mx4 stamps beside the kq, q8 and q51 ones; the stream twin
(`test_gpu_resident_gptoss_host_stream`, `stream_twin_within` of `_resident_feed.das`): two routed layers' experts pinned
to the host and the device claiming first, the streamed windows' rows against the same load with the stream off at the stream
twin's bar (`STREAM_TWIN_BAR_REL`) with the chain's biased-act count (`vk_xf_biased_acts` on the Vulkan home,
`cud_stream_stats().biased` under the CUDA ask, a skip with neither module) the witness - the chain's clamped
swiglu and the picks' gate and up bias rows. On the per-op tier (the route off; the per-op and heat cells skip under the CUDA
ask, this home having no per-op tier):
`test_gpu_perop_gptoss` places every routed layer's mx4 stacks under the biased clamped swiglu and holds a 40-token window
and six fed steps against the CPU chain; `test_gpu_perop_gptoss_heat` runs `_moe_resident.das`' heat cell
(`moe_perop_heat_cell`: the last twelve routed layers placed, the rest streamed under eight hot slots a layer, 80 fed
steps past the pool's re-pin interval, the pool's hits counted by `heat_totals`) against the CPU chain at the routed
block's bar (`MOE_LOGIT_BAR_REL`, 0.20; the file's other cells hold 0.05) - over 80 steps a router near-tie flips, as
the two-window prompt's first fed step does on the RTX PRO 4500 (0.458 against the 5% bar's 0.341, the argmax intact), so
the resident two-window cell holds the routed block's bar too.
`test_gpu_perop_moe_heat` (`test_gpu_resident_moe.das`) runs the same cell on Qwen3.6-35B-A3B-MTP-IQ4_XS-pure, twenty
layers placed and sixteen slots, and holds every routed layer's shared expert on the device after the routed chain or
the pool's hits every step (`shexp_gpu_after_rows`), none overlapped and none on the three dense GEMMs; its second
run takes the Qwen1.5-MoE Q5_1 `--pure` mint, twelve layers placed and eight q51 slots, under the same asserts.
`test_gpu_perop_kv_codec.das` - stocked suite, `-jit` only; the per-op rails under a block-codec
session (Qwen3-0.6B Q8_0, `DASLLAMA_GPU=1`, the whole-model route pinned off for the load): the
device's attention block keeps an f16 mirror in the model's basis and the session's cache keeps
the codec's bytes (`ARCHITECTURE_GPU_VULKAN_DECODE.md#decode-attention-block`). A tq4 session at a
40-token prompt (the device prefill's rows and eight device steps' rows rotated before their
store) and at 20 tokens (under the device prefill's floor: the block hydrates from tq4 rows the
CPU prefill wrote, taking them out of the rotated basis), and a q8_0 session at 40 tokens: layer
0's K and V cache bytes within 5% of the CPU chain's (read 0 - byte for byte - under tq4, 2e-5
under q8_0) with a row held to the next one the control (reads 0.85-0.88), the device's summed
distance from the CPU chain at f16 no more than the CPU codec chain's own (reads 2.3 against
48.7 where the device prefilled, since it attends the rows before their quantization, and 38.4
against 42.6 over rows the CPU quantized), the f16 rows one step off past the CPU codec chain's
distance (the bound's control), and the census witnesses: the block's attention stamp served the
steps (its count on the CPU chains' steps the control), and the prefill stamp ran exactly where
the prompt reaches the device prefill's 32-token floor; a cell skips where the tier took no
attention layer of the carrier. The cells are `cell_perop_codec` of `_kv_codec_cells.das`.
`test_gpu_perop_kv_codec_k.das` and `test_gpu_perop_kv_codec_iq.das` run the tq4 and q8_0 cells at
40 tokens on the other two weight formats - Qwen3-4B-Instruct Q4_K_M and Qwen2.5-1.5B-Instruct
IQ3_XS - on the same skips and bars (the device's distance reads 0.28 and 0.80 of the CPU codec
chain's on the 4B, 0.79 under q8_0 on the 1.5B); the 1.5B carries a Q/K/V bias, so its tq4 cell is
the serving rule's: the ask serves at q8_0 on both chains and the cell holds the q8_0 bars (the
device reads 0.82 of the CPU codec chain's distance).
`test_gpu_moe_shexp.das` - stocked suite, `-jit` only; the shared expert's prefill on the device
(Qwen1.5-MoE-A2.7B-Chat-Q4_K_M-local, the Q4_K_M mint of the Q8_0 carrier, `DASLLAMA_GPU=1`): the
shexp triple as one region over every position of the routed experts' chain, gated by the tier's
in-process route lever - the same prompt and fed tokens with the route on (the device arm) and off
(the CPU form), the logits within the 12% bar at the prefill and every step (the arms part on the
router's near-ties from layer 1 on, not on the shared expert's rows - the bar's `//!` carries the
reading), the one-step-off control, and the engage witness in two halves - the device prefill's
shexp layer count (`shexp_gpu_prefill_layers`) grows by the model's layers on the device arm and
not at all on the CPU arm, while `span_tokens()` grows by the fed step count on BOTH arms (the
decode span serves every step either way, the shared expert inside it only on the device arm) -
at 64 and 600 tokens; the twin is large-tier, so the cells run under
`DASLLAMA_PARITY_FULL=1`, and skip without the twin, the armed tier, or a shexp mark on every layer.
The whole-model driver is pinned off for the load (`set_gpu_resident_route`): it would take the
twin whole, and its arm is the resident MoE file's.
`test_gpu_model_swap.das` - stocked suite; two models through one process on the armed tier
(Qwen3-0.6B, SmolLM2-135M, `DASLLAMA_GPU=1`): a model reloaded behind the other decodes its own
weights, the pin on the upload rail dropping a still-installed model's device state first; skips
without both models or the armed tier. Its model-free cell drives that pre-load drop through its
own entry with a stale mark installed and checks the MoE layer request survives it (needs only an
installed tier).
`test_program_roots.das` - model-free: every dasllama program root (tutorials, examples,
server tools) declares `options stack = 524288`, and every model-loading root declares its
prefill intent. A root is a file requiring `dasllama/` that exports `main`, or - a library root,
built by `daslang -lib` / `-ctx` - carries `[export_c]` entry points instead; both owe the same
two declarations.
`test_jobque_tripwire.das` - model-free: the engine's first dispatch on a job queue nobody
configured panics naming `setup_dasllama_jobque()` (a bare queue decodes ~200x slower - the
fork-context clone per job, a wake per job, workers parking at once), `DASLLAMA_ALLOW_BARE_JOBQUE=1`
downgrades it to a warning, a configured queue is silent; the bare arms spawn
`_jobque_tripwire_root.das`. `test_team_job_panic_exits_nonzero` spawns `_team_panic_root.das`,
whose chunk panics on a worker: the child exits non-zero, its output carries `JOB EXCEPTION` and
the panic's text, and the program does not run past the dispatch. The child runs with four
settings: `DAS_TUNE_POLICY=fallback` (the root requires the engine's tuned kernels, and the
default policy would mint and re-exec the child at startup, which never ends under a pipe),
`DAS_JOBQUE_AFFINITY=0` (the child inherits the parent's affinity mask, and a worker pinned
outside it never runs the chunk), `DAS_JOBQUE_THREADS=2`, and `DASLLAMA_SINGLE_THREAD=0` (an
inherited single-thread pin runs the chunk inline, with no job exception to report). The
in-process case - a `try/recover` around a fifo `parallel_for`, a team, an indexed and a stage
dispatch whose chunks panic, an ordinary round after them still completing, and no clone leaked
(the process-wide live-object count at shutdown) - is `tests/jobque/test_jobque_team.das` and
`tests/jobque/test_jobque_stages.das` at the repo root. The tripwire reds any `with_job_que()`
block a test opens around the engine without calling `setup_dasllama_jobque_()` first.
`test_rnnt.das` - model-free (area audio): the shared transducer decoder (`rnnt_greedy`) over synthetic nets of tiny widths
built in-test, against a scalar reference written in the file - the zero and blank first inputs, the symbol cap, the shapes
(joint off pred, two layers in both gate orders, a predictor off the float4 lattice), the TDT duration table and the plain
RNN-T, the one-lane step against the team publish, the layer-count refusal and the pool's narrowing; `PLOG_BAR`'s control a
net with one joint row mutated landing outside it. The device-seat cells (`rnnt_seat_skip`: skip on a build or box with no
device transducer seat) decode the same nets on the build's seat against the reference, a clip of no frames, each decline
(the knob, a shape past the staged rows, a device fault, the round budget, a failed command buffer on Metal or a failed
allocation on Vulkan) falling back to the CPU loop with its reason counted, and two sessions sharing one model's device
weights. Its kernel references and fixture are `_rnnt_oracle.das`'s, at two shapes (`rc_shapes`: one off every lattice,
one on them) run by `test_metal_rnnt_kernels` and `test_vkt_rnnt`; the pick's cases include a frame of NaN and one of -inf
token logits, which must name token 0.
`test_gigaam.das` - stocked suite (area audio): GigaAM v3 against the upstream package's per-stage dumps - the
external-reference compare: the f16 lane stage by stage (mel, front, every layer, encoded; each stage bar's control the
clip at 1.1x through the same stages) and its decoded ids, frames and transcript, the CTC probabilities against the
oracle's softmax, the q8 lane by its transcript's words (the clip with its second half silenced the control), the
long-form spans of ru_long, the facade arms, the hparam refusals, the foreign `.dlim` refusal (SmolLM2's image saved under
the test's temp dir), the device decode twin on the build's transducer seat (`rnnt_device_twin`, the serving tower pinned
on, its control the encoder rows at 1.05x) and, on Metal, the CTC head on the device seat against the CPU head (each frame's
log-sum within `CTC_PLOG_BAR`, the picks but at a near-tie frame, the clip at 1.05x through the device chain the control, a
CPU-encoded clip serving off the uploaded rows against the CPU head over those rows), and the CTC head alone against the
oracle (`test_gigaam_ctc_head_oracle`: the ctc and e2e_ctc carriers on both lanes, the oracle's encoder rows through the head
on the CPU and, on the build's device seat where it serves on the box (Metal's, else Vulkan's), counted through `ctc_seat_heads` - the
picks the oracle's argmax but at a near-tie frame, the token
probabilities within `PLOG_BAR`, the ids the oracle's; the control the head's halfwords zeroed in `wblob`, its ids off the
oracle's on every leg); the pick's kernel cell is the prefill
file's `test_metal_ctc_pick_kernel` (`_ctc_oracle.das`'s cases against the CPU row loop: three row widths, a tie, a row of
NaN picking class 0, a poisoned bias the control), the Vulkan one `test_vkt_ctc_pick`. The CTC legs the device cells
share (`ctc_leg`, `ctc_cpu_leg`, `ctc_hard_flips`, `CTC_PLOG_BAR`) live in `_ctc_oracle.das`. The cells skip
without their carrier, the oracle dumps (`asr_oracle/gigaam/` under `models_dir()`) or the corpus clips; the facade, `.dlim` and CTC-head
cells also when interpreted; the side-file arm without the Qwen3-ASR-0.6B mmproj and decoder.
`test_nmt.das` - stocked suite (area translate): the Bergamot students (ru-en and en-ru base-memory, en-ru base, ru-en tiny,
en-de base-memory under `bergamot/` in `models_dir()`, rows of `../performance/model_specs.das` with their vocabularies and dumps
as companions) against marian's own f32 graph over the int8 weights dequantized (`nmt_oracle/<pair>-<tier>/`, minted by
`../benchmarks/nmt/bergamot_oracle.py`): the model-file tokenizer id for id against Python sentencepiece over the fixture lines and
the normalizer fixture (`spm_edge.json` off `corpus/spm_edge.txt`; the control the same reader with the charsmap dropped, spelling
the fixture differently); the exact lane under the oracle's own tokens (the forced feed: every step fed the oracle's previous id,
every tapped stage - the logits among them - within `STAGE_BAR` of the dump's largest magnitude, the poisoned-element control, and
a displaced source token landing the last encoder layer outside the bar, through `check_exceeds`), its cls offset -1, and its free
run at `EXACT_LINES_FLOOR` lines of 30 the oracle's, each line logged both sides decoded, the lines fed the next line's source
falling under the floor (the control); the q8 lane on the two ru/en pairs at `Q8_LINES_FLOOR` lines of 30 the f32 oracle's, the
GEMMs and the classifier on the q8 plane, and the same file minted with its classifier's Q8_0 copy zeroed moving every line and
falling under the floor (the classifier reads the Q8_0 copy; the floor's control); the build's device seat (`test_nmt_device`,
`_nmt_oracle.das`'s `nmt_seat_*`: Metal on a das_metal build, Vulkan on a build without das_metal under `DASLLAMA_GPU=1`) - a
synthetic d 32 student declining `shape`, then on the two ru/en pairs the seat pinned off serving no sentence, then on, each pair
in turn every line served with no decline at `DEVICE_LINES_FLOOR` lines of 30 the CPU q8 chain's, the second pair swapped in with no
shutdown between (the weight-reuse key's model swap), both sides logged; the control each pair's device classifier zeroed, every
line then differing; on the Metal seat a scratch take past a 512-byte stand-in for the device's largest buffer
(`set_metal_take_max_probe_`) declining `device` and translating on the CPU; a source past `NMT_DEVICE_SRC_CAP` tokens
declining `length`; the exact lane declining `quant_mode` and
translating on the CPU. The loader's refusals over synthetic files written to the test's temp directory
(`test_nmt_loader_refusals`, `test_nmt_spm_file_refusals`): a Marian student of one encoder and one decoder layer at d 32, each
fixture differing from a well-formed one (which stages - the control) in one fact - the version word, the blobs past the file's
end, a name past it, no embedded config, each config check, the embedding's width and rows, its item type, a missing or
overlong item, an intgemm8 item short of its multiplier, a split vocabulary, no vocabulary, a BPE vocabulary - each refused by
its own text (`panic_text`); a SentencePiece file cut inside a varint, carrying a group wire type or a field past its message,
each refused, a fixed64 field skipped. `test_nmt_float32_student`: a float32 student widens to its intgemm8 twin's planes bit for
bit, the twin laid [out][in] the control. `test_translate_facade_contracts`: an LLM shell refusing to translate with no direction
and a language to itself, `translator_serves` and `translator_languages` on both kinds, a synthetic student through
`load_translator` on both lanes with a pair name that is not two codes. The facade's LLM translators:
`test_nmt_llm_translator` (MiLMMT-46 Q4_K_M under `milmmt/`, no chat template: no source before `set_translator_langs`, an
unknown code refused, ru -> en reading the expected word on one line, the same prompt with no stop at the line's end running on
past it - the control -, en -> ru, the GGUF's baked `.dlim` loading back through `load_translator` as an LLM and translating;
then the ru-en Bergamot student keeping its one direction and its f32 lane translating),
`test_nmt_chat_translator` (Qwen3-4B-Instruct-2507 Q4_K_M, a chat template: ru -> en on one line with no reasoning) and
`test_nmt_chat_thinking_off` (Qwen3.5-0.8B Q8_0, whose template carries the thinking switch: the translation with no reasoning, its
reply within `CHAT_REPLY_TOKENS`); `test_nmt_step_lanes_and_device_gate` the step's lanes through `step_lanes` and its probe seam,
and `nmt_device_shape_ok`. The step's kernel classes are the kernel files' cells (`test_metal_nmt_kernels`, `test_vkt_nmt`,
`test_vkt_nmt_attn`). The tokenizer cell skips without the model or the dumps, the forward and LLM cells also when interpreted, the
device cell without a seat; the `load_translator` loads bake a `.dlim` beside their model - ledgered at
`../ARCHITECTURE_IMAGE_LEDGER.md#suite-image-mints`.
`test_review_howto.das` - model-free (area infra): the HOW_TO check and the unlocked-queue-call check of `../REVIEW.das`
run as a child over fixture files written under the test's temp dir (its `--howto` and `--queue-calls` rails) - one file a
defect, each red on its own finding, and a clean one green.
`test_audio.das` - stocked suite; model-free cells: the audio front-end units (gelu-erf, hann
window, mel filterbank, log-mel chunking, swapped swiglu); model-gated: the tower structure/oracle
gates (ultravox/voxtral/omni shapes, the mtmd all-ones encode oracles - CPU-claim cells, tower
knob pinned OFF) and the `test_encoder_blocks_gpu` cell, the qwen2audio + voxtral 32-layer
CPU-vs-GPU parity on the depth-scaled bars with counter deltas (the device leg runs the blocks and
the projector tail) - Apple builds, `-jit`; skips honestly without the qwen2audio / voxtral
mmprojs; `test_encoder_tail_gpu`, the projector tail alone, one cell a projector kind (qwen2a on
the Omni-3B f16 mmproj, ultravox on the v0_5 1b f16 one, voxtral on the mini f32 one, each minted
in memory): the device tail against the CPU tail over the SAME device block rows
(`set_metal_tower_tail` off for the CPU-tail leg, put back) within 1e-3 rel-l2 (reads 1.6e-4 to
2.1e-4), the tail counter (`metal_tower_tail_encodes`) up on the device leg alone with the blocks
on the device on both, another mel's rows outside the bar as the control (reads 0.8 to 0.95); `test_log_mel_chunks_gpu`,
the chat towers' chunked mel on the device mel seat - a 128-mel clip's chunk within 1e-3 of the
CPU mel (the seat's GEMMs on the exact f32 tiles: a half tile reads a quiet bin 0.085 off), the mel
counter (`metal_tower_mel_encodes`) up by the device leg alone, an 80-mel call off the seat's
lattice served by the CPU with the counter unmoved, another clip's mel outside the bar; the Vulkan twins
`test_encoder_blocks_vulkan`, `test_gemma4a_vulkan_twin`, `test_canary_vulkan_twin` and
`test_qwen3a_vulkan_front` (the three-way cells described under `test_vulkan_tower_kernels.das`),
`test_parakeet_vulkan_twin` (parakeet-TDT 0.6B v2 over jfk through `parakeet_encode` - the front and the blocks on the
device, the same three-way bar, counters and poison as the canary twin, the q8 and exact models minted in memory
through `stage_parakeet_model`); `test_gigaam_vulkan_twin` (GigaAM v3 e2e-rnnt over `ru_short` and `ru_mid`: the q8 lane's
three-way twin, front lever and poison, then the exact lane off the halfwords on the f16 GEMM against the CPU exact chain -
one encode, its front and every block on the device with no `quant_mode` decline, the ids the CPU exact chain's, the
device rows' rel_l2 from the exact chain within the CPU q8 chain's own, the halfword blocks zeroed from the middle on the
control past it; off the f16 GEMM feed the `device` decline instead - and the lane policy under
`set_vulkan_gigaam_exact_lane`, the tower off its control); `test_gigaam_ctc_vulkan_twin` (the ctc and e2e-ctc
carriers on both lanes over `ru_short`: the CTC head on the Vulkan seat, `vulkan_gigaam_ctc`, against the CPU head over the
encode's rows - an f16 head's CPU leg over the rows rounded to half - counted once on `vulkan_ctc_stats`, off the encode's
held device rows where the chain served the encode (`handoffs`), the picks the CPU's but at a near-tie frame, each log-sum
within `CTC_PLOG_BAR`, the ids and transcript the CPU's, the clip at 1.05x through the device chain the control; then a
CPU-encoded clip serving off uploaded rows with no handoff counted; skips as the twins above do);
`test_gigaam_metal_twin` (Apple builds: GigaAM v3 e2e-rnnt's f16 lane over the
FLEURS `ru_short` and `ru_mid` clips through `gigaam_encode` and its greedy decode - the CPU exact chain with the tower
off, the device chain with the conv front and every block on the driver (encodes, convs and blocks deltas), the other
GEMM route under `set_metal_tower_f16`, the block seat behind the CPU front under `set_metal_gigaam_front(false)`,
each within 1e-2 of the CPU rows' scale with the CPU's transcript, a x3 mel outside the bar as
the control, and the q8 lane's one `quant_mode` decline with no encode and its mel seat's -1; on `ru_short` the seats' side
arms - the mel seat declining on the knob, a clip under one window and a scratch past the device's largest buffer
(`set_metal_take_max_probe_`) with no mel counted, the encode off a CPU mel (`gigaam_encode`) on the device within
the bar, the held encoder rows handing off under their host rows and refusing them rewritten in place, the scratch past
the device's buffer declining the encode with one `memory` decline (the block seat's) and the CPU's transcript - then
the asr_prof buckets: the CPU chain's stage and block buckets with the tower off, `ga.encode.gpu` / `.record` with it
on, the replay's `ga.encode.gpu.restride` / `.attn` under `set_metal_fc_prof_replay` alone, `ga.mel.gpu` off the mel
seat; skips without the carrier or the clips); `test_canary_metal_holds_no_rows` (Apple builds: canary's served Metal
encode leaves no encoder rows held, `metal_tower_held_rows_elems` 0 - a transducer family's encode holds them);
`test_mel_spectrum_magnitude` (model-free: `mel_spectrum_rows`' magnitude and power forms on a synthetic DFT table
against a double DFT, the magnitude rows against the power reference the control); and
`test_encoder_tail_vulkan`, the projector tail alone on the Vulkan chain, one cell a projector kind, three ways
over the SAME device block rows (the f32 tail of the exact tower over those rows the reference, the CPU q8
tail with `set_vulkan_tower_tail` off and the device tail with it on, the lever put back; the device within
`TWIN_SLACK_VK` x the CPU q8 tail's own distance plus the twin floor - the CPU tail quantizes the normed rows to
Q8_0 blocks where the device feeds f16), the tail counter `vulkan_tower_tail_encodes` up on the device leg
alone with the blocks on the device on both, another mel's rows outside the bar as the control, all of which skip without their carriers (the qwen2audio / voxtral / omni-3b f32 mmprojs, the E2B bf16
mmproj, the canary f32 encoder, the parakeet f32 encoder, the ultravox f16 mmproj, the Qwen3-ASR bf16 mmproj), without
jfk.wav, without a Vulkan device under `DASLLAMA_GPU=1`, and on a das_metal build.
`test_whisper.das` - stocked suite; the qwen3a mel cell (model-free: the lanes' mel image against the one-lane
image element for element - the DFT GEMM splits on whole 4-row tiles so every row rounds as one lane's); model-gated: the whisper/parakeet/canary/gemma4a/omni
oracle cells (the parakeet v2 cell also runs the transcription with single-thread mode off and
on: the team leg dispatches, the one-lane leg never does, and its tokens are the team leg's - the
decode step's one-lane form against its team publish, both texts logged - and the session that
read the whole clip reading its first half token for token as a fresh session does, the half clip
over 8 tokens, and two more transcriptions on the session leaving the heap where it stood -
`heap_flat_leg`, which the canary and gemma4a oracle cells run too: a transcription frees what it
allocates; the canary cell, an external-reference compare, on the f32 lane over the pair built in memory (no `.dlim`), each clip on the CPU chain with every tower off - the tower's encode count unmoved - and then served - the tower's
encode counted where it serves - in the forced-feed form: the frozen NeMo ids fed one by one, each our argmax or within
`CN_TIE_BAR` of it, the control the same walk fed each id one step early - the oracle id outside the bar at every step
whose input moved - then the greedy ids the oracle's up to the
step the forced walk's pick leaves the oracle's, both texts logged), the Vulkan twin `test_whisper_vulkan_twin` (whisper tiny and large-v3-turbo; the
cell described under `test_vulkan_tower_kernels.das`, skipping without the ggml files, without
jfk.wav, without a Vulkan device under `DASLLAMA_GPU=1`, on a das_metal build, and when
interpreted), the decoder twin `test_whisper_vulkan_wdec` (tiny and large-v3-turbo on jfk, the q8
default load: the decoder pinned to the CPU chain and then on the Vulkan ASR-decoder driver, the
tower serving the encoder on both legs, the texts token for token with the flips logged, the
counters proving the window's cross-KV and twenty or more decode batches served with no decline,
the knob-off leg's knob decline with zero windows and zero steps counted, then the f32 decoder rail's `quant_mode` decline with the CPU
still transcribing; the same skips as the tower twin, plus the handoff count: every served window took the encoder rows
off the tower's plane device to device), `test_whisper_vulkan_wdec_flush` (tiny: a nine-row first batch through a served
window declines `rows`, the pending cross-KV readback lands, and the CPU chain's logits over it match the knob-off chain's
token and bar, the batch with its last token changed landing outside the bar as the control), `test_whisper_vulkan_stem_flush`
(tiny: the block hooks pinned off through `set_vulkan_audio_blocks`, the stem's device rows land at the `blocks` decline and the
CPU blocks over them transcribe the all-CPU chain's text), `test_whisper_vulkan_wdec_lifetime` (tiny, one session reused the way a
serving worker reuses one: a model drop between two transcriptions - the second serves again and reads the same; the decoder knob
turned off between two - the second reads as a fresh knob-off session; the block hooks pinned off after a served window - no
handoff for the CPU-encoded windows, the text of the CPU-encoder chain), `test_parakeet_q8_0_file` (the parakeet v3 q8_0 bin loads through the same
reader and reads jfk as the f32 bin does; skips without it), `test_whisper_q8_0_file` (a whisper.cpp q8_0 bin of tiny loads - its
Q8_0 tensors read as each block's scale times its quants - and reads jfk as the f16 bin does; skips
without `ggml-tiny-q8_0.bin`), `test_whisper_metal_ln_post`
(tiny, the f32 encoder minted in memory: the blocks-with-post-norm seat declines with the tower
off and serves with it on, one device encode counted, and the device's normed rows are the CPU
post-norm of the device's own block rows within 1e-4 of the largest - reads 3e-7 - with the rows
before the post-norm the control), `test_whisper_metal_wdec_flush`
(tiny, the step's floor lifted so the Metal step serves and the window's cross-KV layouts wait on
the device: the host layouts zeroed, a 33-row first batch hands the window to the CPU chain, the
layouts land first and its logits match the knob-off chain's token and bar, the batch with its
last token changed the control), the ASR knob cells (`set_asr_fp32`, `set_asr_tower_fp32` - the mixed
f32-enc/q8-dec serving mode and its `asr_exec_fmt` stamp; the strict token-identity cell
pins the simdgroup lane, and its tolerance-graded twin pins the crowns ON and asserts WORD
equality - the tensor twins' quality gate), the q8-gate CPU-vs-CPU claims
(CPU-claim cells, wdec knob pinned OFF), and the mel state-reuse determinism cell (gemma4a +
canary, both encoders minted in memory with no `.dlim`: `gemm_f32` accumulates, so a reused state's `st.reim` must start zeroed - bit-equal
mels across calls); its ungated cells are the model-free half.
`test_dasllama_core.das` - model-free: walks the module dependencies in-process; no module
`dasllama_core` depends on is `audio` or `stbimage`, while `dasllama/dasllama` depends on both.
`test_dasllama_lib.das` - model-free: the library example's stream and memory calls refuse
cleanly with no model open.
`test_asr_verbs.das` - model-free: the family-owned ASR facade verbs (`asr_exec_fmt` /
`asr_encode_bucket`) over constructed structs - `asr_encode_bucket` over an `AsrModel` of every
kind answering that family's own bucket - the audio families' lane knobs (qwen3a /
gemma4a / canary / parakeet: the un-pinned default against the predicate the policy itself consults, both
pins, and reset from the EXACT pin), the ASR decoder's K/V dtype knob (`get_asr_kv`: f16 by default, each `set_asr_kv`
value read back, put back as found), and parakeet's SPM detokenizer over a toy vocab.
`test_mtp_gemma_drafter.das` - stocked suite AND the `mtp` suite: the gemma-4 assistant
drafter (`dasllama/dasllama_mtp_gemma`), its arms under "Arm filter mechanics" above. Only `mtp-gdraft-refuse`'s shell cells
run with no model - its gather cell gates on the 440 MiB sidecar, `--family gemma4moe` and a das_metal build, its
type-decline cell on the Q4_0 head and `--family gemma4moe`, its Vulkan driver check on the dasVulkan module;
`mtp-gdraft-ple` gates on the E2B Q8_0 trunk, the sidecar and `--family gemma4e`, the other arms on the files and `--family
gemma4moe` - the target PARITY_FULL-gated on `mtp-gdraft-cpu` and every served arm, the sidecar through `model_available`.
The served arms run on any GPU home - Metal, Vulkan under `DASLLAMA_GPU=1`, CUDA under `DASLLAMA_GPU=1 DASLLAMA_CUDA=1` -
skip on a build with none, and skip where the card holds the trunk only with routed layers' experts on the host (the
round's verify rides the N-row token command, which has no host-experts form on either discrete home - the 26B on a 16 GB
card); `mtp-gdraft-vdecl` runs on Metal alone, since the discrete rounds bind no stream off the mirror's codec.
`test_model_specs.das` - model-free: the model-set table's shape invariants
(`../performance/model_specs.das`: unique file/display keys, official => provenance pinned,
parity-evidence shape - a `prefix_bar` carrier holds at least two ids, so its half-span bar is a
token or more), the fixture compare the gates read (`prefix_match_len` / `fixture_ok` on hand rows:
the strict form reds a flipped last id where the half-span form holds it, a flip inside the first
half reds both, the bar sits exactly at half the span), the derived provenance view's invariants (unique names, sha-or-recipe,
https urls), the quantize-minted rows (`test_quantize_rows`: each names a fetched source row, carries the recipe
`quantize_commands` spells, and reaches the provenance view with its `quantize`; `fetch_models --convert`'s route
`convert_route` - a quantize row to the mint even under a canary name, a canary row to its conversion, any other to the
no-recipe warning, Windows refused - and the mint's `mint_steps` - no source refused, an absent importance matrix built
before the quantize, a present one skipped), the mmproj pairing lookup (`mmproj_companion` - the companion found by name, not
by position; >= 2 official vision rows), the pinned image fixture's provenance
(`bench_image_fixture` rides some spec's companion list), and the board fetch (`board_provenance`:
every official row, the image fixture, every ASR carrier and mmproj and every TTS model, no
off-board row and no on-box parity oracle).
`test_parity.das` - stocked suite; model-gated: the frozen parity gates - token for token, or on a
`prefix_bar` carrier (the gemma-4-26B MoE, `../ARCHITECTURE_MEASUREMENT.md#fixture-prefix-bar`) the
half-span prefix on every arm, the main arm carrying the loosened bar's control on the same live
stream (the fixture with an id inside its first half flipped reds it). ONE
generic loop drives every evidence-carrying spec of the model-set table through its declared
pinned arms (evidence is DATA on `ModelSpec.parity` - ids + arms, regenerated via
`../harness/parity.sh`); hand-written arms remain only for the tied-cls bit-match and the gpt-oss
shared-load double fixture. Every compare logs both decoded streams. Large carriers gate
on `DASLLAMA_PARITY_FULL=1` via `model_available`. `DASLLAMA_PIN_BACKEND` points the compares
at one kernel backend: the generic loop and the gpt-oss arm pin it before their load, restore
it on every exit, and name the served backend in every assert label; a name the box cannot pin
(`kernel_backend_pin_refusal`: unregistered, batch-only or unavailable) skips the cell naming why,
never a silent run on the default backend. The MoE reference and per-expert arms put their levers
back through the getters (`get_moe_grouped_prefill`, `get_moe_fused_expert_gemms`).
`test_parity_pregate.das` - stocked suite; model-gated: the board parity pregate
(`lcpp_bench --parity`, via `parity_check` fed a controlled spec) on small carriers - the real
fixture passes, a flipped id fails, an evidence-less spec is refused, the text-form prompt
encodes through the tokenizer (E2B), the kq-native arm engages and restores the mode
(Qwen3-4B Q4_K_M), and the half-span prefix bar of a `prefix_bar` carrier on Llama-3.2-1B Q4_K_M
as the stand-in (a fixture whose tail half is flipped holds, a flip inside the first half reds it,
and the strict form reds the same flipped tail - the flag is what admits it); plus the timed run's fixture gate (`fixture_gate_check` - the opening
tokens of the same evidence on the served model, once per model before the rows): the table's
fixture passes, an opening that matches nothing refuses, a row with no evidence stands aside;
plus the last layer's FFN slice on the resident Vulkan driver (Llama-3.2-1B Q8_0): a 512-token
prefill sliced against the whole window, logits bit for bit, with split-k pinned off for both
arms - the wave model splits the whole arm's GEMMs on a box with SMs to spare where a sliced
region never splits, and the two k orders round apart.
Requires the bench by relative path (like `test_tok_seed`), so it pays the
bench's engine compile.
`test_run_summary.das` - model-free: `run.das`'s own `log_summary` log scraper (last marker line
wins, end of file closes an unterminated final line, doubled marker yields one line), fed
synthetic log files from a per-process temp dir. Requires `run` by bare same-dir name.
`test_run_suites.das` - model-free: `run.das`'s per-PR split read against the files' text (no
`model-free` file reaches a fixture root, every `stocked` file does, no file sits in both, every
listed file exists), the folder census (every test file sits in a `run.das` suite, the
CPU-prefill tripwire the declared exemption; `run.das` carries no `[init]`), the area tables
(every area file sits in the census, every census file has an area, every arm an area names
exists, every image-suite arm is reachable from an area or listed as unclaimed), the pure
planners behind an area run, the argument contracts, the area map, the reach plan behind
`--changed` (a test file itself, a fixture the tests requiring it and no model-suite file, a leaf
module its witness row, a core module the core set and its other areas whole, a benchmark the
bench rows' files; every witness row and the core set census files, every row's module a file
that reaches llm), the `--exclude` filter's semantics, the `--shard` partition (the shards of one list disjoint
and whole, every refused form), the children's module-cache folder (keyed by the binary, the
other binaries' folders removed on the runner's own binary alone, no folder for a binary that
cannot be stat'd, the child command carrying it), and a dry run of the runner with a no-op
child binary.
Requires `run` by bare same-dir name.
`test_pod_fleet.das` - model-free, pod-free: the pod fleet's planners (`../harness/pod_fleet.das`) -
the `runpodctl pod create` argument list (the models volume and its data center, the GPU, the
disk, the ssh port, the key in the boot's environment, the boot installing sshd, rsync, git and
the Vulkan loader, `--wait` and JSON out), the ssh argument list (the pod's address and port,
never prompting), the tree copy that leaves the caches out, the shard command (`--shard i/n`
appended, `TMPDIR` and the runner's log under the run's folder on the volume, `RUN-EXIT` closing
the log, detached so the ssh call returns), the status read over every shard, and the `~` path.
`test_vk_spv_diff.das` - model-free: the SPIR-V dump differ (`../harness/vk_spv_diff.das`, the
identity gate a kernel fold's "byte-identical" claim rests on) - its four bins over two in-memory
dumps (a changed byte and a changed length both read as moved, name order), the verdict's refusal
of a dump with no stamp in it, and the directory read by stem. Requires the harness by relative
path.
`test_served_bench.das` - model-free: the served-turn instrument (`../harness/served_bench.das`, required
by relative path; it loads no engine module - its wire is `dasOPENAI`'s `chat_stream`, whose
reader cells are `modules/dasOPENAI/tests/test_chat_stream_mock.das`'s: the reasoning delta, the
usage chunk's counts, cache hit and timings, a usage chunk without details reading no cache hit) -
the system prompt (its nonce leads, two nonces part at once, the asked characters cycle the corpus),
the corpus as its `%%` segments over a temp file, the row statistic and the brackets beside a row, and whole turns
against a fake chat server on its own thread that answers by the request's `model` field: a complete
stream (counts, both spans past the server's 30 ms pauses, the server's own TTFT), and the four
refusals - no usage chunk, no reply text, a reply of one token (no decode span to time), HTTP 500 -
plus no server at the address, the follow-up row standing over complete turns and refused by a failed one, and the media cells (an image row and a chat-clip row, the clip an `input_audio` part, each over a file and refused without one) - a file as a data URI and a missing one as none, an image question's wall and prompt tokens off the fake's buffered completion, HTTP 500 saying why, an image row standing over a file and refused without one. The speech rows
run against a fake speech server on a second thread: a transcription's wall holds the server's
300 ms pause, a failed and a blank answer say why, every request of a row uploads a clip the fake
has not heard (a WAV's four reps read `new` four times; a clip that is no WAV goes up as it is, the
fake hears it twice and the row is refused), a row answers the text its reps read and refuses
reps that read different texts or a text other than the one asked, and the loaded row stands while
the fake chat turn outlasts two fast reps and is refused when the reps outlive the turn, when the
turn carries no reply text, and when the loaded text differs from the idle one. The synthesis and
scene cells: a WAV's seconds off its header (a header with no sample and text bytes reading 0), a
synthesis against the fake's one-second WAV (its seconds, a failed and a JSON answer saying why), a
scene round against both fakes (one decode rate, every lane answering inside the turn), a paced
lane sending one request a period with a failed request reading below zero, a scene row
refusing fewer than two samples on either side, the synthesis rows and the scene rows whole (a
failing model or lane refused alone, a lane too slow for the turn refused for its empty round),
the flags refused when they would report no row of a mode they ask for, and the scene's lanes
off the flags.
`test_site_records.das` - model-free: the records-vs-site drift gate - `merge_site_records`
(required by relative path, pays the engine compile) regenerated in memory and byte-compared
against the committed `site/files/dasllama/bench_records.json` and its first-paint projection
`bench_cells.json` (what daslang.io/dasllama.html renders); red means a records commit skipped
`gen_site_records`. Plus the projection contract: the render fields survive, the receipt-only
fields are absent from the text; and the board's filters (`test_board_projection`): an older
release's das row, a retired board's row and its reference, and a reference with no das row to
pair stay off the board, the current row and its reference the control; a held row stays in the
merge and off the board with its reference, its unheld sibling the control; and the stored audio
outlier is voided on the projected row off the das row's clip lengths, unioned with an
annotation's clips, the merge itself untouched.
`test_tok_seed.das` - model-free: `lcpp_bench.das`'s `tok_read_seed` corpus-header walk and its
`flat_row_plen` seam (the flat session's prompt rows beside a tg-real row: the real row's cap
read back, a longer flat prompt kept, the llama-bench sizing with no real row) and its
`rows_ctx_need` seam (the rows' context cap: the batched streams' need - prompt, window, a
slot a stream, the margin, times the self-speculative rows - never under the flat rows' own
need, and the flat rows alone with no batched row) and its `--mtp-ab` seams on a headless model
(`mtp_ab_drafter_attach`: no sidecar beside the model attaches nothing, and a sidecar no backend reads attaches nothing
either; `mtp_ab_drafter_line`: the rounds, joint verifies and per-round draft and verify times, zero rounds reading 0 a
round), required by relative path (`../benchmarks/lcpp_bench.das`), so it pays the bench's full engine compile.
`test_tokenizer.das` - stocked suite; the corpus cells are fixture-gated (the `ggml-vocab-*.gguf`
corpora under the models dir, machine-local): the seven vocab families' `.inp`/`.out` corpora
through `load_tokenizer_auto` -> `encode` / `decode`, ids exact and the decode round-trip
lossless; reports SKIPPED where the vocab is not stocked. One cell is model-free: the BPE
`add_bos` default a GGUF without the key takes (upstream's per-pre table), then the qwen35 vocab
reading `add_bos == false` where that fixture is stocked. The SentencePiece byte-fallback cells:
model-free, a hand-built vocabulary with control tokens ahead of its byte pieces (Mistral v0.3's
layout, where 3 + the byte is a control token) encodes a newline as its `<0x0A>` piece and no
control id, the Llama-2 layout (byte pieces right after eos) reads the same pieces, and a
vocabulary with no byte pieces panics on a byte it cannot spell; model-gated, Mistral-7B-Instruct-v0.3
Q8_0 reads `<0x0A>` at 781 with 13 a control token, and the newline and tab samples encode to
`llama-tokenize --no-bos`'s ids and decode back.
`test_embeddings.das` - stocked suite, `-jit` only; every load in memory (no `.dlim`). The embeddings read per model
(`test_embeddings_qwen3_embedding`, `_past_the_context`, `_chat_model_default_mean`): on Qwen3-Embedding-0.6B Q8_0 the
file's `pooling_type` reads last-token and its `add_eos_token` on, `encode` ends a text with the EOS, and the model card's
example lands the card's cosine matrix within 0.02 under the file's pooling while the mean read misses it (the control); on
tinyllama Q8 (a chat model) the default read is the mean, float for float, no EOS appended; a text past the context embeds its
leading seq_len tokens. `test_embeddings_batch_matches_single`: `embed_batch` gives each text the vector `embed` gives it
alone, in input order, in one window, across windows of 40 rows and under the 256-wide cut. `test_embeddings_hidden_only_window`
(a dasMetal build): an embedding's Metal window on the blob twin runs no classifier and feeds no decode mirror where a chat
window does both, one window an embed, the card's matrix holding and the mean read missing it; the short window's q8 sites
take the split-K form (`metal_prefill_split_k_sites` and the q8 split stamps' census count), `set_metal_prefill_split_k(false)`
the control moving neither; the empty text - its EOS alone, one row - rides one Metal window and lands the planar f32 model's
vector within `EMB_F32_BAR` (cosine 0.9999; the f32 "x" missing it; the reference is f32 because the CPU q8 chain's Q8_0 activations read a
first row about a percent of cosine off it, logged beside), and a batch holding it reads the single's vector
(the batch's other text missing it); with Metal off a planar model's embed rides no Metal window.
`test_embeddings_lone_forwards` (0.6B Q8_0, then the same file loaded as q4_0): `embed_window` runs a one-row text and every
q4_0 text through `embed_forward` alone (`embed_lone_forwards` up by one for the one-row text beside two others, by three
for the q4_0 batch, unmoved for two several-row texts - the control), and each batch matches its singles (another text's
single misses the bar). On an armed Vulkan tier (each device cell pins the whole-model route on and the region
pin, and puts both back): `test_embeddings_on_the_resident_driver` - the sessions take the mirror codec, six texts are six
device prefills, the card's matrix holds (the mean read misses it), the facade's batch is one scratch window of one
device-home session a text matching its singles (another text's single misses the bar), a mean read past the last layer's
32-row tail matches the planar CPU model's within `EMB_MEAN_BAR` (another text's CPU read misses it), no call passes to the
CPU rails, a batch of more texts than the N-row command's rows (`RDEC_NROW_MAX` + 3) still sits in one packed window with
every text a device prefill and matches its singles, and f32 host sessions move neither counter;
`test_embeddings_ring_layers_take_a_window_each` - gemma-3-1b Q8_0 on the whole-model driver: neither discrete home's
packed seat (`rdec_segs_packed`) seats segments back to back over a sliding layer's ring, so a batch takes no packed window
(a window a text, every text a device prefill) and still matches its singles; `test_embeddings_window_rows_follow_the_region` - under a 256-row
region `embed_window_rows` reads the region, a batch past it takes exactly the windows its rows cut into, matches its singles,
and an unserved model reads `EMBED_WINDOW_ROWS`; `test_embeddings_every_region_held` - with every region held by a live
device-home session the batch takes host-cached sessions and no scratch window and reads the free run's vectors;
`test_embeddings_ple_model_takes_no_scratch` (gemma-4 E2B Q8_0) - a per-layer-embedding model with sliding layers is promised
no scratch window, takes none, and its batch matches its singles. Model-free `test_embeddings_session_rules`: `session_keeps_rows_only` per
config field, and two models of one geometry with no fblob read two embed pool keys. `test_embeddings_batch_fresh_sessions`:
a recurrent model (Qwen3.5-0.8B) takes fresh sessions - its batch and singles leave `embed_pool_builds` where they found it
- and matches its singles under the mean, and a pooled model of another
geometry (tinyllama) between two reads rebuilds the pool and back (`embed_pool_builds`; the same model's next call keeps it),
the second read unchanged.
`test_exe_smoke.das` - stocked suite; model-gated (SmolLM2-135M, small tier): the
standalone-exe context gate. Builds `_exe_smoke_root.das` with `-jit -exe` and runs the
artifact - the rail where globals restore as DATA, so a function-typed global with no
boot-restore `[init]` dies on its first invoke while every `-jit` suite stays green - then
builds it again split with LTO (`--jit-split-modules=-1 --jit-lto`; each build's output must
announce its own form and never the other's, so a dropped or a leaked flag cannot pass; on a POSIX
box the arm needs `clang++` on PATH and skips without it) and reads the same `EXE_SMOKE_OK` marker.
~5 min for the two builds.
`test_gen_records_args.das` - model-free: the measurement orchestrator's pure seams - the
pybench args builder's per-tool arms (onnx carries the `--out` recovery file and never a device;
nemo forwards `--device`), the `asr_gpu_pair_tool` roster, the `records_run_verdict` ZERO-cells
refusal, the `argv_receipt` form (every element shell-quoted where it must be, every home
directory spelled `~` outside the quotes), the rig's `--catalog` scope (`official | all`, every
other spelling refused), the cpu legs' batched shape (`npl_cpu_shape_ok`: 1 or more on both
axes, or no batched row; `npl_leg_shape`: a cpu leg reads `--npl-cpu-plen` / `--npl-cpu-reps`,
the metal leg the flat rows' -p / -r, the shipped cpu shape 128 x 3), the two guards on `records/<box>.json` (`--jit` and `--catalog all` each
need an explicit `--store`, each with its store-given control and the board-mint control), the
das child's argv spelling on both arms (released exe vs the `--jit` bench source with
`--for-debug-purposes`), and the cold/warm retry verdict - `cell_rates` folding a cell's das rows
onto the pp and tg axes separately and `keep_cold` keeping the cold cell when the warm re-run is
slower on either, each bar with its control - and the TTS cell's seams: the reference argv on the
torch and mlx arms, its `BENCH` and `VERSIONS` lines read back, the das child's argv, the catalog
lookup, the corpus head and the GPU seats' verdict (`none` / `all` / `partial`) - and the CrispASR
reference's: the summary line's two fields (the realtime factor over the samples where the seconds do not
parse), the runs a process prints, the best rep (`crispasr_best`, none unless every rep timed), the row's
flavor stamp (`asr_ref_flavor`), each tool's CPU exe (`asr_ref_exe`) and the GPU leg's own Metal build
(`asr_gpu_ref_missing`), and `cmake_built_bin`'s three layouts in their order - the branches that
decide what lands on the public board, which no model suite reaches.
`test_sizing_helpers.das` - model-free: the sizing helpers (`reserve_resize` exact capacity,
`grow_resize` geometric reuse and its re-added tail reading zero, `ensure_length` grow-only with
the grown tail zeroed, `overwrite_resize` grow-only no-init) fed directly, including grows past
the `max_unreserved_size` guard that must not panic.
`test_from_template.das` - model-free: the `[from_template]` stamp (`dasllama/dasllama_tune`) - a
placeholder call renames to the annotation's target per stub, and the stub's signature types the
clone so one template stamps both plane overloads.
`test_kv_prefix.das` - stocked suite; beside each attach below, the probe `prefix_match_len_` reads the
count the attach then attaches (0 on a recurrent cache, which attaches at checkpoints); model-free cells on a synthetic Config: the prefix cache's page
accounting, the LRU budget and the token verify, then a recurrent session's checkpoints - one
checkpoint a donation with the page of its last row copied, the `max_states` and byte budgets (tails
dropped before openings, a tail a prefill stops at promoted to an opening, a page two checkpoints of one
conversation share counted once, the checkpoint just inserted never the victim), where a
prefill stops (`prefix_checkpoint_at_`: the caller's stable opening, else the opening an earlier
checkpoint shares, a token short of a prompt an earlier one holds whole, nowhere under a page past
the hit or on a model with no recurrent state), and the side state (the n-gram input's last tokens
and conv history ride the checkpoint; a snapshot of another state size is refused and leaves the
session as it stood), the donation's answer (`test_prefix_insert_reasons`: "" for a kept checkpoint, a touched one and a dense
model's pages; the refusal naming its cause - a token list short of the session's position both positions, a session with no
evaled row, a cache that keeps no checkpoint), a snapshot filed at a stop the cache keeps (`test_prefix_snapshot_stop_touched`:
no second checkpoint, insert, byte charge or page reference, the kept one used now and promoted to an opening), and the device
pool's verbs (`test_prefix_pool_carrier`: a carrier session takes the
whole pages the cache already holds under the longest opening a checkpoint shares - none from an empty cache, the
opening's partial page never - and the rows past them are its own; a state taken off another session enters as a
checkpoint, an opening or a tail as asked, the shared page counted once in the bytes, a state off the carrier's
position not kept; `prefix_state_touch_` answers a kept stop and promotes it to an opening; the split attach
shares every page of the deepest opening into the carrier, copies none, restores the state into a session that
holds no pages and counts a hit, the probe `prefix_state_match_len_` reading its count; on a model with no
recurrent state the page chain's whole pages under the asked count), and the dropped checkpoints' snapshots
(`test_prefix_spare_snapshots`: two of three dropped wait for reuse with their arrays, an empty cache hands an empty one,
the release drops them), and the pages' ring floors (`test_prefix_ring_floor`: a page carries its donor's `kv_ring_floor`, an
attach - and the probe beside it - cuts to the longest run ending at or past every taken page's floor, a copied page past the
whole hits dropped when its rows end under its own floor, the session left at the deepest floor it took; a carrier's shared
pages carry theirs, a floor-0 chain's the control); model-gated (stories15M): attached pages plus a tail eval read the cold
prefill's logits bit for bit, and the page past the whole hits (`test_prefix_tail_copy`): a
donation's partial tail page is an entry beside its whole ones, a prompt that shares rows with it
takes a COPY (the donor's page stays the cache's alone, `n_hit_partial` counts the take) and lands
the cold prefill's logits bit for bit - the tail whole, three of its five rows, and four leading rows
of a whole page - every page back in the pool at the end; its model-free edges
(`test_prefix_tail_edges`): a prompt whose first row past the whole hits the next page does not
share copies nothing, each donation's page past a hit is a candidate (of two donations past one
page a prompt takes the one it shares rows with), and a partial take is an LRU touch - the taken page outlives an older
untouched one when the budget evicts; and the prompt no whole page serves
(`test_prefix_first_page_edges`): of two donations' first pages the one sharing the most leading
rows is copied (a partial first page and a whole one), the same prompt again stops a token short, a
prompt sharing no first row takes no page, the chain whose first page was copied is credited the
hit, and an evicted first page is no candidate; a link to a page the cache dropped attaches
nothing past the whole hits - the
model-gated twin a prompt sharing five rows of the first page alone, bit for bit the cold prefill.
`test_facade.das` - model-free: the facade's own seams over constructed Models; among them
`test_facade_device_kv_rows` - the rows a device holds a session's K/V for (`device_kv_max_rows`):
the installed room seam (`set_gpu_kv_room_hook`, put back) over the model's K/V bytes a row, both
planes of every layer that keeps rows of its own (a `kv_src` layer owns none), f32 halving the f16
count, never past the model's context, and no seam bounding nothing but the context; on a Metal
box the installed seam itself - the device's largest buffer over the row's bytes: the idle mirror cap
(`set_metal_decode_mirror_cap_mb`, put back) bounds no room, so 64 and 128 MiB read the same rows, past the 65536
that 128 MiB of 2048-byte rows would hold.
`test_metal_role_infer.das` - model-free: the `@role` derivation over the `_role_fixtures/` kernels,
each compiled at run time - the grid micro-grammar, a buffer passed whole taking the callee
parameter's direction (a contradicting `@role` refused), a pointer into a tracked buffer taking the
pointee's const, the staged GEMM's device step and the device-pair step and store deriving their
inline-addr operands (a wrong role on either refused), a generic helper resolved by base name, and
a body that delegates to methods. Without das_metal the device-pair cell skips and the other cells
pass after logging that the lens does not load.
`test_driver_knob_getters.das` - model-free: the driver knobs' getters and the decode registry's removal verb -
`mtp_depth_pin` reads whether `set_mtp_depth` pinned a depth and the depth, and `restore_mtp_depth` puts back a pinned and an
unpinned depth; `unregister_decode_override` drops an override and its batched step - the name re-registered single-stream arms no batch twin,
its batched step registered again arming one (the control) - deselecting it where it is active and leaving another selection as
found; on a das_metal build `get_metal_decode_fuse`, `get_metal_prefill_lastrow`, `gemma_trace` and
`gemma_carry_pre_norm` read back every value their setters write and the value found once it is put back, the h reading set with no
drafter attached leaving the decode's carry alone (skips without das_metal).
`test_kernel_backend.das` - model-free: the kernel-backend registry - the portable backend
registered at `[init]` with a row-major default active (`test_portable_backend_always_registered`),
select / pin / for-load pick a registered backend and an unknown name changes nothing
(`test_select_and_pin`), a restored seat keeps the pin it found and leaves none where there was
none (`test_seat_restores_the_pin`), `with_kernel_backend` runs its block under the pin, puts the
seat back, refuses an unknown backend without running the block, and leaves an earlier pin
standing after a block on another backend (`test_with_kernel_backend`), an unavailable backend
skipped by for-load select and refused by pin (`test_backend_availability_predicate`), and the
batch pin overriding the batch slots, surviving re-select and refusing a repack donor on a
row-major active backend (`test_batch_backend_pin`), a batch-only backend neither pinned nor
selected as the whole backend, the active backend and the pin as found, with every donor the build
carries - `metal` on a das_metal build, `vulkan` where dasVulkan builds - registered batch-only and
walked (`test_batch_only_backends_refuse_the_whole_seat`), and the pin refusal
(`test_env_pin_refuses_what_cannot_take`): `kernel_backend_pin_refusal` names why an unregistered
or a batch-only name cannot pin and reads "" for the portable backend, and the pin
`DASLLAMA_PIN_BACKEND` names panics where it cannot take (driven through the `pin_kernel_backend_env`
seam) while the same pin made in process, or a name the environment did not name, warns and keeps
the active backend (the controls). `test_kq_registries` walks every `KqFmt`
through `kq_registries`: a format carries all eight registries or none, each family's tile, gemv
and panel registries open on the reference row, q8 carries none, and no format's tile, tile4,
gemv or panel row is another format's; it skips on a build without dasLLVM, where
`dasllama_math_gen` and its `[tune]` registries are not mounted.
`test_kqformat.das` - model-free: the per-format descriptor row (`kq_desc`: disk bytes equal the
ggml block bytes of the type, ids round-trip, `kq_fmt_of_id` refuses an id no format carries),
`kq_bytes_per_weight` as the stride table's arithmetic on every superblock format, the
`kq_fmt_stamp` call macro binding every member's tag so a tag-overload family resolves at compile
time, the key-radix and lattice-predicate cells, and the load report's row name (`conv_row_name`: the site off the GGUF tensor role, the disk type, a kq plane by its `KqFmt` member and a converted copy by its conversion).
`test_kv_codec.das` - stocked suite; `test_kv_codec_step_down` serves a tq4 ask on Qwen2.5-0.5B (a Q/K/V-biased
attention) at q8_0 in a session and in a pool, q8_0 asked staying q8_0, and Qwen3-0.6B keeping tq4 (both carriers through
`model_available`); its model-free cell `test_kv_codec_side_stamp` holds the
`kv_codec_side` macro (`dasllama/dasllama_kv_dtype`): each codec's arm sees its own cache-pointer
type and, nested once per side, the sixteen K x V pairs each carry the same address.
`test_kquant.das` - model-free: the K-quant and grid plane dequants and dots against in-test
references, including the iq4nl plane dequant on a jobque team lane reading the same values as the
main thread (its codebook read is fork-safe); `test_kq_phi3_longrope_factors` - the model-gated
LongRoPE cell on Phi-3.5 Q4_K_M: the loaded model's `rope_freqs` are the file's
`rope_factors_long` set bit for bit (the file's 131072 context runs past its original 4096) and
not its short set, the rule itself (`rope_factor_tensor`) model-free in `test_rope.das`; and `test_q8_plane_row_f32` - rows quantized with
`quantize_q8_0_into` and repacked at (mr, kgroup, wbias) = (4, 8, 128) and (16, 8, 0), with and
without a row tail, read back through `q8_plane_row_f32` as q times the block's f32 scale bit for
bit (control: the product with the scale rounded to f16 must miss on at least one element). The per-32 cells
(`p32_cells`) hold the dense per-32 arms too (`p32_dense_gate`): `mm_f`, `mm_pre_f` and `mm_b_f` over a Model shell's q51,
iq4nl32, q4032 and q5032 planes against the scalar row dots, bit for bit, each with its poisoned element; the q5032 plane
cell holds the hand-packed Q5_0 blocks to d x (q - 16) exactly (control: the unsigned level must miss).
The grp repack cells at mr=16 also hold the bf16 panel of every format, built into a NaN-filled
buffer: `kq_panel_rows_bf16` puts each weight at its pair-layout slot within half a bf16 step of
the disk row dequant, and where the format's stamp is a bf16 tile (`kq_tileform_of(fmt)` above 0
- an x86 box with AMX bf16) the stamped panel (`kq_panel_bf16`) within a whole step (control on
both: the slots against their pair neighbors' weights must miss). On that stamp the 4-token tile cells register a skip and
return - the stamped tile reads a bf16 panel over 32 tokens, and `test_prefill_cpu_kernels.das`
reaches it through the batch kernel.
`test_softmax.das` - model-free: `softmax`, `parallel_argmax` (the FIRST maximum on ties, the
empty row a no-op) and `hlse`.
`test_compares.das` - model-free: the suites' shared compares (`_compares.das`) over hand-built
rows - a NaN on either side reads as an infinite
`logits_maxdiff` wherever it sits, `maxdiff_at` lands on it, `logits_maxabs` scales no bar over
it, rows of two lengths read infinite, `mismatch_exact` counts every element past the shorter
row's end, and `mismatch_rel`, at a relative bar and at an absolute floor, counts a NaN as off; each
with equal rows and a finite difference as its controls. `check` runs on a probe handle (`probe_fails`):
each bar kind (`rel_of_larger`, `exact`, `rel_of_max`, `envelope` and `per_element`, a float and a
double expectation) passes rows inside it and reds one element past it, a NaN on either side and a
length difference red at any bar, the poisoned-expectation control reds a bar too loose to see it
(at the last element or a chosen one, and an empty compare that has none), `check_count` counts a
NaN on either side, a finite miss and each element of a length difference off and equal rows 0 while
asserting nothing, `check_exceeds` passes a finite miss and reds equal rows (the control) and a NaN or
an infinity on either side, and `check_rows` is `check` over rows and heads; `check_exact` does the same over bytes and words; the host helpers
(`plane_of`, `poisoned_copy`, the generic `copy_span`, `argmax_of`, `opaque`, `jit_only`) and the
`Sweep` tally (lengths, misses, NaN, poison legs inside a loose bar, inert elements) each against
hand-counted results.
`test_deltanet.das` - stocked suite; model-free cells: the deltanet session-state sizing at 27B
geometry through `make_run_state` (S state + widened-conv history past the guard); model-gated:
the chunked-vs-recurrent prefill equivalence probe on Qwen3.5-0.8B, in the forced-feed
logits-tolerance form with the tier's deltanet chain and decode step pinned off (its claim is the
CPU chunk algebra); the YaRN override on the same partial-rotary 0.8B - the divisors over the
rotated span and the magnitude in `rope_mscale`, the decode rope row against the fold spelled out,
the table and the direct rope paths of the attention agreeing under it within the deltanet bar, and
the fold moving the step's logits past the two paths' own noise (the greedy text under YaRN is a
near-tie on a model this small, so llama.cpp's continuation is logged, not asserted).
`test_vision.das` - model-free: the vision preprocessing rail (geometry, letterbox, normalize)
bit-exact against pinned mtmd oracle hashes (dumps + mint scripts in the models dir's
`gemma4-vision-oracle/` and `qwen3vl-vision-oracle/` - the qwen rail letterboxes at align 32),
plus the stbimage decode/dump round-trips in a per-process temp dir.
`test_gemma4uv.das` - stocked suite; the gemma4uv embedder tier-1 parity vs the `-p encode` oracle
dumps (f32-mmproj-minted - the bf16 oracle carries bf16-dot activation noise); gates per-token
mean/v0..v3 at 2e-4 with the measured maxdiff logged; skips honestly without the mmproj or dumps.
On Apple builds the CPU gate pins the tower knob off, and a second test gates the GPU tier-1
encode against the same dumps on a scale-relative bar (2e-4 + 4e-3*token-rms) - exceeding it is a
red, the bar each fixture actually held is logged either way, and engage is proven per fixture by
the encodes counter. On a Vulkan build `test_gemma4uv_tier1_vulkan` runs the same four dumps
through the Vulkan embedder whole off the planes (the embedder minted in memory), the stem's
im2col counted on the device beside the encode, the knobs put back through `with_gpu_knobs`.
`test_gemma4v.das` - stocked suite; the gemma4v ViT tower (E-series) tier-1 parity vs the `-p
encode` dumps minted on the f32-widened mmproj, CPU, `-fa off` (`mint_e2b.sh` / `mint_e4b.sh`):
eight E2B fixtures (96^2 cb through 672x336) on the scale-relative bar 2e-4 + 4e-3*token-rms, the
measured maxdiff logged per fixture; plus the clamp knockout (every block clamp disarmed through
the staging planes must miss the oracle - the sidecar scalars are load-bearing); plus the E4B rung
- the same tower geometry at soft-token width 2560, gated on its mmproj's four-dump seam subset
with one GPU-engage and one q8-lane fixture. Skips honestly without the mmprojs or dumps. Every
CPU-lane claim pins BOTH GPU tower knobs off (`set_every_gpu_tower`). On Apple builds `test_gemma4v_tier1_gpu`
gates the fixtures through the Metal chain - the stem, the blocks and the tail on the device - with the engage
counters, the knob-off decline leg, and the q8 lane on the device: a q8 tower minted under the pin serves off
its q8 planes on the q8 lane's bar, every block counted, no `quant_mode` decline. On a Vulkan build two more cells:
`test_gemma4v_tier1_vulkan` runs four dumps through the Vulkan block loop over the q8 image (minted
in memory under the lane pin, the pin restored) on the q8 lane's bar with the engage counters per
fixture, then the input poison - a q8 tower with block 8's planes zeroed, served by the driver, must
EXCEED the bar on cb336; and `test_gemma4v_vulkan_twin` needs no dump - cb96 and cb336 three ways
(the exact CPU chain, the CPU q8 chain, the device chain) on one- and two-block truncated towers,
then cb96, cb336, green 672x336 and cb768x384 (1152 rows, the aligned tile variant's canvas) on the
whole tower, every tower staged and minted in memory (never `load_gemma4v_tower`, which bakes a
`.dlim` under the pin), the counters proving which served, the device's distance from the exact
chain held within 1.5x the CPU q8 chain's own (the gate's reading: the device chain is the same or
better), the residual rows logged per 64-row tile, the input poison on the one-block cb96 leg (a
fresh q8 tower with its block zeroed through the device chain must EXCEED the bar the clean chains
set), then the exact lane served - the exact tower through the device chain at cb336 on the f32 tile over the
bf16 block rows widened: one encode, every block, the grid's rows, no `quant_mode` decline, the device within the
exact lane's rel_l2 bar (`TWIN_EXACT_REL_L2_VK`) of the exact CPU chain with the moved-expectation control, and a
fresh exact tower with block 8 zeroed through the device chain EXCEEDING it; the E4B cell's q8 leg re-runs its dump
through the driver and runs the same zeroed-block poison against it. All three skip without a
Vulkan device under `DASLLAMA_GPU=1`, and on a build with das_metal, where the Metal driver owns
the tower hooks.
`test_gemma3v.das` - stocked suite; the gemma3 SigLIP tower (gemma-3-4b mmproj) tier-1 parity vs the
`-p encode` dumps minted on the f32-widened f16 mmproj, CPU, `-fa off`
(`gemma3-vision-oracle/mint_gemma3.sh`): the canvas is FIXED 896^2 (learned position table), so
the five fixtures vary content, not geometry; exact lane on the 2e-4 + 4e-3*token-rms bar, the
q8 serving lane on its measured 3.2e-1*rms bar (27 blocks, ffn served at the layout's padded
4352 width so every GEMM quantizes), plus the fixed-canvas panic gate and the carrier
sniff/exec_fmt cells. On Apple builds the CPU gate pins the tower knob off, and a GPU rung
gates two fixtures through the Metal chain (the blocks and, behind them, the projector tail) on its measured 4e-2*rms bar - engage proven
per fixture by the encodes/blocks counters, plus the knob-off decline leg (the 72-wide heads
restride to the attention tiles' 128 on the driver), a flash-off leg (`set_metal_tower_flash(false)`: the padded
slab trio and its unpad serve the attention, on the GPU bar), the q8-lane leg (a PINNED-q8 tower with
the knob ARMED serves off its Q8_0 planes through the prefill ladder's q8 GEMM over the vision q8
slab, on the q8 lane's bar: one encode and every block counted, no `quant_mode` decline), and a third crowned encode on the twin-W route
(`set_metal_tensor_crowns("mulmm_q8")` + `set_metal_tower_f16(true)`, the lane pinned exact so
the twin is baked), witnessed by the `metal_tower_f16_encodes` delta. Skips honestly without
the mmproj or dumps. The CPU-lane claims pin both GPU tower knobs off (`set_every_gpu_tower`); on a
Vulkan build the q8 cell re-runs its two dumps through the Vulkan block loop with the engage
counters and the input poison (a q8 tower with block 13's planes zeroed, served by the driver, must
EXCEED the q8 bar on cb896), and `test_gemma3v_vulkan_twin` is the dump-free instrument - the fixed
canvas three ways (the exact CPU chain, the CPU q8 chain, the device chain) on one- and two-block
truncated towers and the whole tower, every tower staged and minted in memory (never
`load_gemma3v_tower`), the device's distance from the exact chain held within 1.5x the CPU q8
chain's own, the input poison on the one-block leg (a fresh q8 tower with its block zeroed through
the device chain must EXCEED the bar), then the exact lane served on the fixed canvas - the exact tower
through the device chain on the f32 tile over the blob: the counters, no `quant_mode` decline, the device
within `TWIN_EXACT_REL_L2_VK` of the exact CPU chain with the moved-expectation control, and a fresh exact
tower with block 13 zeroed through the device chain EXCEEDING it; one deep canvas only, since the exact chain
at 4096 rows x 27 blocks is the cell's cost. The Vulkan rung and
the twin skip without a Vulkan device under `DASLLAMA_GPU=1`, and on a build with das_metal, where
the Metal driver owns the tower hooks.
`test_gemma3v_tail_gpu` holds the device tail alone, dump-free: the soft tokens with the tail on the
device against the CPU tail over the same device block rows (`set_metal_tower_tail` off) within 1e-3
rel-l2 (reads 1.3e-4), the tail counter up by the device leg alone, another canvas outside the bar.
`test_qwen3v.das` - stocked suite; the qwen3v tower tier-1 parity vs the `-p encode` dumps minted on
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
the Metal tower nor the accelerate float-batch tier serves - `qwen3v_gpu_exact_serves()` is the
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
GATED fixture, its own GPU-lane zero-layer poison, and the q8 lane on the device - the deepstack
tower minted q8 under the pin serves off its q8 planes at cb448 on the q8-ds bar, every block
counted, no `quant_mode` decline); an f16-W ROUTE cell (crowns pinned
via set_metal_tensor_crowns + prefill re-init so the half twins compile, the 4B fixtures on
the same GPU bars, engage proven by the metal_tower_f16_encodes delta, plus the knob-off
leg whose counter must not move, and the Omni-30B bf16 half - the same crowned route on the
bf16-sourced tower, one fixture, its own f16-encodes delta, which is what proves the twin
bake covers bf16 files and not just f16 ones); and a model-free lane-knob cell. The CPU-lane
claims pin both GPU tower knobs off (`set_every_gpu_tower`). On a Vulkan build `test_qwen3v_vulkan_twin`
(Qwen3-VL 4B, the deepstack carrier) is the dump-free instrument: the exact CPU chain, the CPU
q8 chain and the Vulkan chain over the q8 image (the stem, the blocks, every tap's merger and the
tail on the device, the projected plane's slices read back) on one-, two- and six-block truncated towers (the six-block one reaches the first tap) and
the whole tower, three canvases, every tower staged and minted in memory (never
`load_qwen3v_tower`), the device's distance from the exact chain held within 1.5x the CPU q8
chain's own, the input poison on the one-block cb96 leg (a fresh q8 tower with its block zeroed
through the device chain must EXCEED the bar), then the exact lane served at cb448 - the exact tower
through the device chain on the f32 tile over the blob, the taps' mergers on the device too: the counters,
no `quant_mode` decline, the device within `TWIN_EXACT_REL_L2_VK` of the exact CPU chain with the
moved-expectation control, and a fresh exact tower with block 13 zeroed through the device chain
EXCEEDING it. Skips without the mmproj, without a Vulkan device under `DASLLAMA_GPU=1`, and on
a build with das_metal, where the Metal driver owns the tower hooks. The vision and audio families'
Vulkan twins share one instrument, `_tower_twin.das`: the seat guard (the tier's want and the device,
`vulkan_tower_arms`) and the jfk cells' seat, the three-way encode with the family's counters and
the bar by metric (rel_l2 wherever a CPU q8 chain is the control - the vision canvases and the
audio towers alike, since a deep tower's worst element is one tail element - qwen25v's maxdiff
against the exact chain), the input
poison, the audio towers' exact-lane decline and the vision towers' exact lane served, the vision canvas
and dump-poison legs, and the staged tower's truncate-and-zero (every plane that spans the block: a q8
tower's qblob, an exact tower's blob and halfword plane) and in-memory mint. The GPU knobs it pins (both towers, the ASR decoder) are
captured and put back through `_gpu_knobs.das`, the record the TTS rail shares.
The model-gated cells skip honestly without the mmprojs or dumps (the metal cell counts its
gated fixtures and skips when the dumps are absent).
`test_qwen25v.das` - stocked suite; the qwen25v tower (Qwen2.5-Omni's window-attention ViT,
projector `qwen2.5o`) tier-1 parity vs the `-p encode` dumps minted on the f32-widened dual-tower
mmproj, CPU (`qwen3vl-vision-oracle/mint_25o.sh`): five fixtures, four of them shaped - cb112 =
the single-window arm, cb448 = four full windows, cb616x336 = ragged window edges, and quad448 =
the WINDOW DISCRIMINATOR (four exact-value quadrants; uniform/periodic fixtures make every window
statistically identical, so an all-full-attention poison hides under them - quad reds it at 10.7
vs the 2e-4 + 1e-2*rms bar) - plus the merged-patch-grid panic gate. No q8 lane in this tower, so
no q8-lane cells. On Apple builds the CPU gate pins the tower knob off, and a GPU rung gates four
fixtures through the Metal window-ViT driver on the 8e-2*rms bar (the 32-layer mix amplifies the
f16-operand GEMM noise and re-rolls on low-bit kernel changes; quad448 is OFF this rung's numeric
set for exactly that reason - window-distinct content realizes the amplification at 0.18-0.4*rms),
with engage proven per fixture by the encodes/blocks counters, a twin-route counter witness
(crowns pinned), the knob-off decline leg, a zeroed-block GPU-lane poison through BOTH staging
planes, and the SHALLOW routing cells - 1-layer truncated towers (pure-window and all-full) on
quad content, GPU vs the CPU chain at 0.1 abs, the chaos-free window discrimination for the DRIVER
(the kernel's own block-diagonal strictness is the kernels-suite `tower_win` gate). Skips honestly
without the mmproj or dumps. The CPU-lane claims pin both GPU tower knobs off (`set_every_gpu_tower`). On a
Vulkan build `test_qwen25v_vulkan_twin` holds the Vulkan block loop over the baked halfword twin
(this family has no q8 lane, so the exact CPU chain is the reference) to 1e-2*rms on one-, two- and
eight-block truncated towers and to 0.15*rms on the whole tower (the f16 GEMM feed's re-roll at
depth, the Metal rung's order; the window layers attend in f32 so their noise does not compound)
on the single-window, four-window and ragged-edge fixtures with the engage counters and the
knob-off decline, every tower staged and minted in memory (never `load_qwen25v_tower`), the input
poison on the one-block cb448 leg (a fresh tower with its block zeroed through BOTH staging planes,
served by the driver, must EXCEED the bar), then the shallow routing cells on the driver - the
one-block pure-window and all-full towers on quad content against the CPU chain at 0.1 abs, the
measured maxdiff logged, each with its own poison (the device over the zeroed one-block tower
against the clean CPU chain must EXCEED the bar); quad content stays off the deep numeric set as on
the Metal rung. Skips without the mmproj, without a Vulkan device under `DASLLAMA_GPU=1`, and on a
build with das_metal, where the Metal driver owns the tower hooks.
`_vision_oracle.das` is the shared dump parser / fixture generator / per-token compare /
over-bar scorer (the must-EXCEED half of a poison leg) / dump gate (`oracle_gate`: the dump, the
encode, the token count, the compare) all vision tier-1 tests use (the `quad` generator and the
q1/q2/q3 quarter-offset probe fields live here).
`test_audio_embedder.das` - stocked suite; model-free cells: the `AudioEmbedder` carrier's own
arms - the no-audio refusals and the probe's 0-not-panic contract; model-gated: the gemma4a arm on
the E2B mmproj (and the E4B mmproj's probe reading the decoder's 2560 where the encoder's width differs), carrying the padding-contract cell (a 320-sample clip encodes to exactly 1 soft
token) and the direct-image route on the lane-named `.dlim` of the lane the box serves, mapped only where its verdict reads
`CURRENT` (an image another box baked under the same name skips loudly), and the same route over an image the cell mints for
this box into its scratch directory, `CURRENT` and mapped (the control); the
whisper-class tower arm (`test_audio_embedder_tower_arm`: the ultravox v0_5 1b and the
Qwen2.5-Omni-3B f16 mmprojs - the probed width, the projector kind, a 5 s clip one chunk of 187 and
750 rows, finite and not all zero, a reused state reading the same rows bit for bit, another clip
the control, a 35 s clip two chunks, two more encodes leaving the heap where it stood; and the span rule off the projector, ultravox bare, qwen2.5o
not, a missing file false); the pre-encoded rows seam on a plain chat (`add_user_audio_rows`: a second's clip lands
25 rows, two clips append, the turn answers and consumes them, a short row block and a queued
image panic, and the same turn with thinking off under the chat sampler preset answers - gated
on the E2B Q4_K_M decoder + its bf16 mmproj, loaded staged, no `.dlim`), and
the no-audio-arm refusal (SmolLM2-135M: a family with no audio markers panics, and the same rows
passed as a bare span queue).
`test_vision_embedder.das` - stocked suite; model-free cells: the `VisionEmbedder` carrier's own
arms over constructed carriers - the text-only (none) shape, the loader's refusals by name
(missing file, audio-only mmproj), and the `vision_exec_fmt` lane stamp (the qwen3v q8 flag
reaching it; qwen25v exact-only); model-gated: the gemma4uv arm on the 12B mmproj - the sniffed
family tag, the 48 px align, the 3840 projection width.
`test_qwen4exp.das` - model-free: the hyper-connection carrier's CPU forms on a synthetic Model shell -
`test_hc_mixer` (the mixer seams against the scalar form), `test_hc_mixer_batched` (`hc_mix_b` and
`block_resid_b` split over positions bit for bit the inline leg, the split leg asserted to dispatch,
rows at the ends and the middle against the scalar mixer at 1e-5 / 1e-4 with the next row's reference
as the control on both bars), and the n-gram side input's gather, window and row decoders
(`test_ngram_gather`, `test_ngram_window`, `test_ngram_row_decoders`).
`test_ple_check.das` - model-free: the gemma4 router input's row identity (`gemma4_router` over
npos rows against the npos = 1 decode call, bit for bit, on a synthetic scale row) and the PLE go-live tripwire (`ple_check_table`) on synthetic
Model shells - short plane trips per format arm (the per-32 q51 / q5032 arms one byte short included), full plane passes, non-PLE exempt; and the per-layer planes' tags off the file's types (`test_ple_plane_formats`,
`detect_ple_formats` over a synthetic meta: an all-F32 kind marked f32, a Q6_K kind tagged k6, a kind mixing F32 with another
type panicking on the first layer that differs from layer 0, naming that tensor and both types, gate and projection alike).
`test_ple_modes.das` - stocked suite; model-gated (E2B Q8_0 + Q4_K_M, small tier): the PLE
token table's pinned-plane rail across serving modes - fp32 keeps the Q8_0 table on a
dedicated q8 plane (offset 0, plane == table exactly, gather rows BIT-match the file dequant,
wblob provably too small to carry the expansion) and a K-quant table on its native kq plane;
plus a greedy chat turn per serving mode on the E2B Q8_0 carrier - a q8 control, an fp32 turn,
and a `q4_0` turn, the `q4_0` one proving q4 serving has a per-layer-embedding rail; `test_ple_per32_tables` loads the
E2B emb-Q5_0 / emb-Q5_1 / emb-Q4_0 mints - the PLE table tagged q5032 / q51 / q40, four rows of `ple_gather_row` bit for bit
the file dequant (the neighbor row the control), a greedy turn on the CPU chain.
`test_tts_textnorm.das` - model-free: the TTS text normalizer - the number, ordinal, year and
decimal readers, the reference normalizer's own examples, the fixed upstream defects as
failing-first cases, and the 200-sentence corpus (`_tts_fixtures/g2p_corpus.json`, loaded by
`_tts_corpus.das`) against the reference normalizer's output - or the fixture's hand-corrected
`expected` form where that output is not what a person says - idempotence included.
`test_tts_postag.das` - stocked suite; model-free cells: the tagger's word class and shape
features (the trainer's own hyphen rule, the d/h/q suffixes), the tokenizer's comma and colon
infix rules, and the missing-pack panic; model-gated (`tts_postag.bin` in the models dir): the TTS
tokenizer token-for-token against the reference tokenizer on the corpus, the tagger's agreement
with the reference tagger (overall and on the heteronym words), the packed file's header floors,
and the load budget.
`test_tts_g2p.das` - stocked suite; model-free cells: the missing-pack panic, the pack-version
refusal (a synthetic version 1 header in a per-process temp dir; the stocked pack is its positive
control) and the American-to-British rewrite table over pinned strings, idempotence included;
model-gated (`tts_g2p.bin` + `tts_postag.bin`): the grapheme-to-phoneme rail phoneme-identical
with the reference front end on the corpus (fed the same normalized text) except the sentences its
heteronym rules and the lexicon additions of `../harness/g2p_local_additions.json` read past it
(named in the cell), both rails' remaining mismatch counts pinned exactly at 2, the heteronym gate
(both annotated readings present, 32 of 38 against the reference's 24; a verb tag out-ranks the
collocation table), the function-word, splitter, inflection and number arms (a glued number past
2^31 reads its digits, never "zero"), the fallback chain (the lexicon's own plain reading before
CMUdict, on both rails), stress helpers, the load budget and the pack's size budget; then the
BRITISH half against
`_tts_fixtures/g2p_corpus_gb.json` (loaded by `_tts_corpus_gb.das`, minted by
`../harness/mint_tts_g2p_gb_fixture.py` from the reference's own `british=True` front end with its
espeak `en-gb` fallback): phoneme-identical on the 157 rows the reference answered from its
lexicon alone (an `oov` row is not a lexicon-parity row - our own fallback answers from the
lexicon, or from American sources rewritten), the inventory sweep - no American-only symbol
and nothing outside `gb_gold`'s inventory survives in 200 sentences, with the same sweep over
the American output as the must-EXCEED control - the inflection rules the flag switches (the
-ing decline on a schwa- or length-mark-final stem among them), the rewrite table's
pack-derived rhotic rows and its keep-the-r fallback for a vowel with no row, and the fallback
rewrite on the fixture's probe words with the reference's espeak readings logged beside ours, and the
British lexicon tier itself - the bath-trap split, the LOT vowel and the non-rhotic vowels
answered from the British tier rather than the rewrite, with the American reading of the same
line beside it. Then the AMERICAN-ONLY pack (`tts_g2p_en_us.bin`, skips when absent): phoneme-identical
with the full pack on the 200 sentences, British refused by name on it (the full pack reading
British is the control), and the directory rule of `g2p_pack_path` - the full pack wins where
both sit, the American twin serves alone, an empty directory panics naming it.
`test_tts_kitten.das` - stocked suite; the symbol-map and token-rule cells run everywhere
(the front end's inventory into espeak-style IPA against the reference rewrite over the corpus,
the reference driver's re-spacing and wrapping), as does `test_spectrum_diff_reds_a_nan` (the
parity rail's source-spectrum compare on a synthetic two-bin spectrum through `probe_fails`:
a NaN in our magnitude, our phase or the reference magnitude reds it,
the clean rows the control), the model-gated cells (`kitten-<size>.gguf` +
`tts_oracle/kitten_<size>/` under the models dir, both from `performance/build_tts_data.das`)
run the parity rail of `_tts_parity.das` per size and a facade smoke cell that speaks one
sentence and checks the PCM is finite, non-silent, of speech length, and carries its timings;
`test_kitten_synthesis_metal` (nano) is the synthesis across the tower knob through
`tts_gpu_synthesis` (`_tts_parity.das`; kokoro's twin is `test_kokoro_synthesis_metal`): on the
served lane every chunk served at every seat but the generator's, one tower encode per seat per
chunk, the knob-off chunks reaching every seat (the generator seat's included), serving nothing and
declining by name at each, the generator seat never reached while the decode seat serves, the
tower leg audible, the sample counts within a twentieth
(the q8 CPU chain quantizes its activations, so its durations can round a frame apart); on the
reference lane, one captured noise stream on both legs, the sample counts equal - the durations
token for token. The sample-wise figures are logged, not gated: an uncaptured synthesis draws its
noise on the tower (a hashed stream) and on the CPU (its PCG32 stream), and the source phase sits
at a hundred thousand radians in f32, so an F0 a millionth apart moves whole ulps of it. The strict
compares are the parity rail's stage cells, each stage's seat fed the CPU chain's inputs, on the
f32 lane: PL-BERT within 5e-5 (reads 3e-6; the f16-staged route read 2e-3, which the bars sit
under), the text encoder within 1e-5, the duration encoder, the raw durations and prosody within
2e-5 (each reads under 2e-6), the rounded frame counts equal, the decode seat - the decoder through
the source, the generator and the inverse STFT on the oracle's noise - within 5e-2 (reads 3e-3 to
7e-3 on kitten-nano, under 2e-2 on kokoro) at the same sample count; and the decode seat again on
the served q8 planes within 1e-1 (reads 2e-2 to 6e-2; the CPU chain quantizes its activations, the
tower reads the weights dequantized at f32 through their f16 block scales) - the only numeric gate
on the q8 slab writes. Each cell carries a scaled or
reversed input as the compare's control. The seam check carries the generator seat alone on the
oracle's own inputs within `GPU_SEAM_BAR` (5e-3; 1.2e-3 at most), the knob-off leg bit-equal to
the CPU chain with its decline recorded. The models for these cells mint in memory from the GGUF's
staging on the lane each cell names, no `.dlim` baked. The GPU cells read the serving driver
through the rail's helpers (the `_gpu_knobs.das` record - `gpu_knobs`, `set_gpu_knobs`,
`with_gpu_knobs`, shared with the tower twins - and `set_serving_gpu_tower`, `gpu_encodes`, `gpu_declines`): the Metal
tower on an Apple build, the Vulkan TTS driver elsewhere. Where no GPU device serves, every cell
skips loudly, and a driver that registers no seat for a cell's stage skips that cell; a device
whose seat declines a stage is a red.
The kitten image rail is the `image` suite's `kitten` arm (the TTS area's), not a cell here.
`test_tts_kokoro.das` - stocked suite; model-free cells: the symbol map over a synthetic phoneme
string, the out-of-vocabulary drop, the style-row clamps, and the pack-name language rule
(`<language><f|m>_` for all nine codes, and every other shape reading "" whatever letter it opens
with - the letter alone would make "emma" Spanish); model-gated (`kokoro-82m.gguf` +
`tts_oracle/kokoro/`): the vocabulary and style-row rule, the parity rail of `_tts_parity.das`
against the PyTorch reference, a facade smoke cell through the front end's own inventory (the 28
English packs, two declared languages, 8 of them British), the British voice cell - one line
phonemized in both dialects, every British symbol proven to be in the model's own vocabulary by
the token count, `bf_emma` speaking, and the sample count of that synthesis held against the model
driven straight from each dialect's string, which is what proves the VOICE's dialect reached the
synthesis - and the voice refusals (a pack whose language the front end lacks names that language;
a voice the model has never heard of refuses first, with no language to name); the GPU cells of
`_tts_parity.das` - the seam check, the per-stage cells with the q8 decode cell, and the served
synthesis across the tower knob on both lanes - as the kitten entry describes them, the seat-absent
skip included; and, model-free, the seat-name refusal of `styletts2_gpu_stats`.
`test_tts_pocket.das` - stocked suite (`pocket-tts-en.gguf` + `tts_oracle/pocket_english_2026-04/`
under the models dir, minted by `../harness/convert_pocket.py` and `../harness/pocket_oracle.py`): the
unigram tokenizer id for id against the package on the 200-sentence corpus and the byte-fallback
probes (`_tts_fixtures/pocket_tokens.json`, the oracle script's `--tokens-fixture --no-replace` -
the file predates the replacement keys, so its fixture carries the package's rules with the table
off), the preparation rules on the fixture's short texts (the terminal mark after a closer, a
trailing dash, a sentence end before a comma), the decode
round trip; the parity rail - the codec decoder over the oracle's latents (one shot), the codec
encoder and the voice prompt's key-value rows per layer, then teacher-forced frames (the oracle's
noise and its own frames fed back): the head alone on the oracle's conditioning, the backbone
conditioning and EOS logit per frame, the latents and the waveform, plus a free run logged; the
facade cells - caps (cloning, no speed, one language, the 19-voice roster), the reference's chunk lists over
multi-sentence texts, one sentence spoken with the family's own timing stages, a cloned voice
joining the roster and speaking, and the refusals (an unknown voice, a speed, a phoneme request,
a clip at another rate); the q8 lane (the served default: the GEMMs minted q8, every codec
conv f32, teacher-forced frames logged against the f32 oracle at a rel-l2 figure, the free run's
frame count and speech - the rig is the lane's quality gate); the GPU cells (the Metal tower on an
Apple build, the Vulkan TTS driver elsewhere, through the rail's helpers) - the codec seat
against the CPU chain over the oracle's latents on the f32 lane within `GPU_CODEC_BAR` (1e-5;
reads 9e-7 on the exact stamps on the M5 Max, 9e-4 on the f16-staged route, which is why the seat runs exact)
with the bar's one-sample control and the x3-scaled latents as the compare's control, one tower
encode a call, the knob-off leg bit-equal to the CPU chain with its decline recorded; the codec seat
on the served planes of the q8 and kq files against each file's own CPU chain within
`GPU_CODEC_SERVED_BAR` (2e-1; reads 1.3e-2 and 7.7e-3 on the M5 Max, 9.2e-2 and 1.5e-1 on the pod's
x64 CPU chain - the CPU's Q8_0 activation blocks, the tower at 3e-6 of the CPU f32 chain on either
box) with the x3-scaled latents as the compare's control; the frames
seat against the CPU chain teacher-forced on the oracle's noise and frames, on the f32 lane: the
latents, conditioning rows and EOS logits within `GPU_FRAME_BAR` (2e-5; reads 2e-6 on the M5 Max) with the
bar's one-element control and the x3-scaled noise as the compare's control, the generator left
where the CPU loop leaves it (the last batch's draws past the frames made rewound), one hook
call, one encode a batch of frames, the knob-off leg bit-equal with its decline, then the free
run - its own noise, every frame fed its own output - against the CPU chain's on the same seed
within `GPU_FRAME_FREE_BAR` (5e-3; reads 1.5e-4 and 1.1e-4 on the M5 Max, 7e-5 and 1.8e-3 on the
pod) with the generator check, batches of three (the encode count within one of the batch count -
the batch the EOS frame lands in may split the tail - the latents within the bar, a batch below
one clamping to one), and the
seats taken by an empty record and given back (`register_pocket_gpu` / `unregister_pocket_gpu`,
the hook unreached then serving again); the prompt seat's model-free rules (`test_pocket_prompt_rules`: the cache admission's four refusals on a
bare voice state, and the residency record spent by one take and missing on another slot build, row
count, fill or embedding rows); the prompt seat (`test_pocket_prompt_gpu`: every layer's K and V rows of one case's text, the
seat against the CPU chain, within `GPU_PROMPT_BAR` on the f32 lane and `GPU_PROMPT_SERVED_BAR` on
the q8 and kq files' served planes, each with the bar's one-element control, the text rotated by one
token as the compare's control, one hook call and one encode served, the knob-off leg bit-equal with
its decline); the frames seat after the LLM tier's model drop in the same
process (`test_pocket_frames_after_model_drop`: one served leg on the q8 file, `moe_gpu_drop_model`,
the same leg again serving and reading the same frame count - the slabs, the scratch and the voice
slot rebuilt behind the drop); the served frames cell takes both oracle voices in turn
on each file, so the second voice's slot displaces the first's, within `GPU_FRAME_SERVED_BAR` (1e-1; reads 1.9e-2 to 7.1e-2)
with the x3-scaled noise as the compare's control; the seat record's refusal of a
name no seat carries and its seat names in order (`test_pocket_seat_stats`, model-free); the
long chunk's codec seat declining by shape; and the served synthesis across the knob, every
chunk's prompt, codec and frame loop served, the encodes
past one a chunk, the knob-off chunks declining at every seat - where no GPU device serves all
three skip loudly, a present device that declines is a red; the parity, stream and frames cells
pin the tower off, since the CPU chain is what they hold; the published Q8_0 file
(`pocket-tts-en-q8.gguf`) against the f16 file's load-time quants - every backbone GEMM arrived
as Q8_0 and no codec conv did, the same lane within a few percent; the quiet floor
(`pocket-tts-en-kq.gguf` alone): the served lane's differenced quiet-window floor within 6 dB of
the f32 lane's on one two-sentence text, with a hissed copy of the reference as the control
that lands outside the bar; and the five other
languages (`pocket-tts-{de,es,it,pt,fr}-q8.gguf`, oracle dirs `tts_oracle/pocket_<config>/`
minted over `_tts_fixtures/pocket_sentences.json`, token fixtures `pocket_tokens_<config>.json`):
the language code and the one default voice, the tokenizer on the language's own sentences
and the probes, the teacher-forced frames of every oracle case, one sentence through the
facade. Two more carriers gate their own cells: the small form `pocket-tts-en-kq.gguf` (the
K-quant file: its Q4_K tensors arrive as kq planes on the unpinned lane, the head as Q8_0; the
teacher-forced latents of the kq lane held to the q8 lane of the same file at the q8 bar with a
poisoned-expectation control, the f16 twin's distance logged, the exact lane speaking) and the
one-voice `pocket-tts-en-stuart-kq.gguf` and its 18m twin `pocket-tts-en-18m-stuart-kq.gguf`
(no codec encoder: `caps()` reports one voice and `cloning = false`, the stored voice speaks
from its latent frames, `tts_register_voice` refuses by name; the 18m twin speaks the same
samples as `pocket-tts-en-18m-kq.gguf` on `stuart_bell`, whose tensors it carries unchanged). The prepared-prompt transfer cell uses both KQ files and the checked-in
`modules/dasLLAMA/models/jfk_ask_not.wav`: encode a one-second reference, serialize it,
destroy the source model, register in the encoder-free model (the record with its `latent_space`
moved is refused first), and assert finite audible speech plus lazy conditioning-state creation;
its last leg registers the same 2026-04 prompt in `pocket-tts-en-18m-q8.gguf`, a 2026-10 file of
the same codec under another config name, and hears it speak. It skips if either KQ model is
absent, the last leg without the 18m file. The 2026-10
small English models (`test_pocket_small_models`: the f32 references `pocket-tts-en-18m.gguf`,
`pocket-tts-en-7m.gguf`, `pocket-tts-en-2m-f32.gguf`, the oracle dirs
`tts_oracle/pocket_english_{18m,7m,2m}_2026-10/`, the fixture
`_tts_fixtures/pocket_tokens_english_2026-10.json`; each model skips where its reference or
oracle is absent): the shape the file declares (key-value heads, the queries' width, 64-wide
heads, the head's sampling rule with no timestep constant, the 64-wide `z_proj` bottleneck the
head and the EOS logit read, the text table multiplied out, the 128-wide decoder behind the
100M encoder) and the 4.0.1 driver rules (the EOS threshold, six guarded frames, the 5 ms
fade-in, the prompt trim, four replacements); the parity rail on the f32 lane - the prompt
trim against the oracle's trimmed clip within `TRIM_BAR`, the encoder and the voice state's
rows, the codec over the oracle's latents against its one-shot decode, the teacher-forced
frames (the head alone, the conditioning, the EOS logits, the latents) and the faded waveform
against the oracle's, every one within the 100M rail's own bars (reads float noise: codec
decode 2e-6 to 4e-6 of the peak, latents 5e-5 on the M5 Max); the published file
(`pocket-tts-en-{18m-q8,7m-q8,2m}.gguf`) - its backbone GEMMs' format, the tokenizer and the
preparation probes on the fixture, the teacher-forced latents at the q8 bar, the chunk lists
through the facade, and one sentence spoken with the GPU seats armed - each seat the file's row
of `SMALL` says admits it served once - every seat on every file: the 2m's 112-wide residual rides the
prompt seat through rows expanded to the GEMM's padded input and its 12-channel codec stages through
the im2col route - none declining by shape; on the references the frames and
prompt seats run teacher-forced against the CPU chain through `frames_seat_leg` and
`prompt_seat_leg` (within `GPU_FRAME_BAR` and `GPU_PROMPT_BAR`; read 2.3e-6 and 1.1e-6 on the M5
Max - grouped keys, the bottleneck row, the time-free tail) and the codec seat over the oracle's
latents within `GPU_CODEC_BAR` through `codec_served_leg` (reads 1.3e-6 on the M5 Max), the
32-wide heads on the attention row; and the reward-tuned twins (`test_pocket_small_refl`:
`pocket-tts-en-{18m-refl-q8,7m-refl-q8,2m-refl}.gguf`, one sentence each through the facade).
`test_tts_facade.das` - stocked suite; model-free cells: the sentence chunker (the reference
driver's boundary rule, the cap counted in codepoints, the hard split of a whitespace-free run,
the appended comma as Kitten's driver rule and the bare text Kokoro's sends), the normalizer the
facade exposes (`tts_normalize`: abbreviation, decimal, money, clock, date, ordinal, percentage,
unit, empty), the voice refusal a pack whose name carries NO language draws (hand-built model -
the sentence must not render an empty code), the WAV container, the codec's malformed-lead and
astral arms, kitten's dropped-symbol rule, the `rtf` guard, and the chunk cap knob on the
model's own chunker (`tts_set_chunk_chars` on a hand-built model: sixty short words one chunk at
the default, ten or more pieces each under a 30-codepoint cap, the default restored; a cap under
one refused by name), the release keeping a carrier whose noise was captured and emptying a drawn
one, and the harmonic source's noise stream on a hand-built decoder (a captured stream drawn in
the reference's order - phases, then normals row by row - against the phases from
`styletts2_draw_noise` plus the source's own per-window draw, sample-identical; another seed
moving most samples; the window knob read back from the carry's stride at the default, the whole
run and five frames, each speaking the same samples; a negative window refused by name); model-gated
(`kitten-nano.gguf` through `model_available` + the front-end
packs): the streaming form's chunks concatenate to the buffered synthesis sample for sample, one
synthesis at one lane and the other at the box's lanes, the phonemizer on the corpus rail, the
model-keyed chunker's Kitten arm (the driver's comma - the arm no model-free cell reaches), the
language form of `tts_phonemize` - the declared language reads as the bare form does, an
undeclared one panics at the call site - and the idle release with the cap: the three-sentence
text makes three chunks at the default cap, a synthesis leaves the generator rows and the
initial phases in the carrier and no per-sample noise, the source's window-zero control speaks
the same samples as the default window (the window set to `SOURCE_WINDOW_FRAMES` before the
first synthesis and left there), `tts_release_scratch` empties the rows, the waveform and the
phases and the next synthesis is sample-identical, and a 24-codepoint cap speaks the same text in
more than three chunks, audible (peak over 0.05, a silent buffer the control) and 0.7 to 1.5 the
uncapped length (half the stream the control).
`test_tts_blocks.das` - model-free: the block home's two layouts against each other - every
rows form (token-major [T][C]) held to its channel-major twin at the dot-envelope bar (a
tolerance times the sum of |w|*|x| feeding each output, with a zeroed-tap poison leg that must
EXCEED it, and on every compare `_compares`' poisoned-expectation control - one expected element
moved just past the bar, which must red), the q8 lane at its own 2% bar, the padded input width on both lanes, the
multi-chunk stacked-tap loop, `adain_rows_into` against `adain_rows`, `layernorm_rows_into`
bit for bit against `layernorm_rows` in place on a copy with the source untouched (control: a
moved source element moves its output row), the norms and the
duration expansion; plus the split-invariance gate (every rows kernel bit-equal on both axes
that move the row split - batch lane cap 1 against 0, and jobque worker limit 1 against the
box's job count, the axis that reaches the AdaIN column stats and the bare `lanes_for_work`
sites the cap never touches - each axis carrying a witness arm that its narrow leg shapes a
rows GEMM to fewer lanes than its wide one) and the must-panic cells for the rows conv's shape
refusals; then the continuous-audio family's kernels - the causal cached attention (`TtsKvCache`,
`attention_causal_rows`) against its double-precision reference at a prompt, a decode step, a
key window and a one-key window, bit-equal on both split axes, the cache's overflow panic; the
grouped-query form (two and four query heads a cache head) bit-equal to the plain form over the
cache with every key-value head repeated, within the attention bar of the double reference, and
the three refusals (a width off the head lattice, fewer query heads than cache heads, a count
the cache's heads do not divide); `compact_rows` (every kept value at its channel-count slot, a
wider target refused), `fade_in` (the ramp's values, the clamp past the wave's end, the no-op at
one or zero samples) and `end_on_pause` against its double form (the cut at the last loud frame,
the fade to zero, the pause of zeros; a clip loud to its end keeps every frame, one under a frame
comes back as it is) -
`elu_rows`, `layer_scale_rows` and `pad_edge_left_rows` against their scalar forms; and the
harmonic source's window forms: each resampler's window form (output columns from the source
columns its taps read, both at strides off the whole row, on both arithmetics and at the source's
two scales plus two more) bit-equal to the whole row, the source itself in 1-, 5- and 16-frame
windows bit-equal to the whole run on both arithmetics over a synthetic F0 with unvoiced frames
and a drawn noise stream, its mix through `linear_rows` in windows bit-equal to the whole
run's on both split axes, and a window past the carry's reset refused.
`_tts_parity.das` - the rail both families run: token ids against the reference driver on every
oracle case, identical durations on every case, and on the bring-up set every stage through the
decoder output within 1e-4 of the oracle's peak, the sine source within `SOURCE_BAR` (5e-3, absolute:
the phase past 1e5 radians turns a ulp of an increment into radians; the pod's x64 chain reads
2.5e-3 on kitten's ONNX law, the M5 Max under 1e-4) fed the oracle's F0,
the source spectrum's magnitude within 1e-4 of its peak and its phase within 1e-2 rad where the
magnitude carries signal, and the generator (its stage-0 internals included) within 1e-4 fed the
oracle's spectrum and decoder output - its conv_post seam within `SEAM_CONV_POST_BAR` (2.5e-4 of the
peak) and the waveform within `SEAM_WAVE_BAR` (1e-3, absolute), the fp32 batch tile rounding apart from
the per-token dot, each with the control of the generator fed `gen_in` rotated one row landing outside
the bar; the end-to-end waveform difference is logged, not gated (the F0 phase drift the file header
explains). The whole rail runs under `with_reference_f32_tile` (`_f32_tile_pin.das`): every fp32 batch
GEMM on `f32_batch_tile`'s declared fallback through the engine's f32 tile pin (`set_f32_tile_pin`), a
seat of its own ahead of the float-batch override, so a sidecar crowning another permutation moves no bar
and the rail holds under `DASLLAMA_ACCEL=1` - only the f32 tile is pinned, every other kernel keeps its
stamp, and the pin is put back however the block leaves, a panic included.
`test_f32_batch_tile_perms.das` - model-free: every permutation of `f32_batch_tile`'s grid
(`_f32_tile_pin.das`'s `[dasllama_grid]` clones) over the 4x4 lattice of the inverse STFT's GEMM
kitten mini ends on (n 11, d 20, 45480 frames) and of a 301-wide dot (d 20, 2048 tokens: past every
permutation's lanes with a scalar remainder, asserted) under a NaN sentinel, against a double oracle within
its own order's bound, `2 * eps_f32 * sqrt(depth) * sum |w x|` per output - depth the roundings a
product passes through, `n / L + log2 L + n % L` over the permutation's `L` lanes, `n` when the loop
is sequential or shorter than `L` - its largest difference logged with token and row; the control
moves one weight so its output shifts by twice that output's bound, which must land past it. Then the
pin itself (`test_f32_tile_pin_seat`): the plain and the offset `matmul_batch` forms under the pin bit for
bit the reference permutation's lattice plus `dot` over the rest, the pin counting both, with a float-batch
override armed - the accelerate tier under `DASLLAMA_ACCEL=1`, else a counting stand-in that is offered
no GEMM while pinned, and unpinned is offered the offset form and never the plain one (the control); the
pin alone arms no override seat (the lane announce reads that seat); a panic inside the block reaches the
caller with the pin put back.
`test_tower_helpers.das` - model-free: the shared encoder-tower helpers in `dasllama/dasllama_tower`
(clamp, row norms, f16-table GEGLU-quick, im2col, two-axis rope, avg-pool, `attention_bidir`),
each against an in-test reference; `rel_pos_table` against an fp64 sin/cos reference, its
relative-position-0 row exactly (0, 1) pairs, the table of one frame more as the control that
must miss the bar.
`test_matmul_batch.das` - model-free: the batched GEMMs against a plain double oracle under a NaN
sentinel - the fp32 tile walk within `F32_TILE_REL` of the oracle at every tail shape with an
`all_finite` assert and a poisoned expectation, the threaded walk bit for bit the one-lane walk (its
control a moved element), the row nest under `set_f32_batch_tile_min_n` bit for bit the per-token
`matmul` (the tile at the shipped knob rounding apart is the control), the bf16 plane through the
same walk at every tail shape and the GEMV shaper, its oracle over the bf16-rounded activations
where the box stamps `bf16w_tile_gen` (every walk pinned to the shipped knobs through
`with_shipped_knobs`), and the Q8 path bit for bit.
`test_tower_asr_kernels.das` - model-free: the public tower / audio / TTS kernels with no CPU
unit of their own, each against an in-test fp64 reference over a procedural seeded-LCG fixture
and each bar carrying its own must-EXCEED poison arm - the padded-width GEMM wrappers
(`mm_blob_b` on an owned blob and on a borrowed plane, `mm_bf16_b` on bf16-exact operands,
`mm_plane_b`'s per-tensor routing, and the claim that a zero-padded served width is
bit-identical to the unpadded GEMM), `layernorm` and both `layernorm_batch` overloads at a dim
off every lane width, `add_bias_rows` and `add_inplace_rows` against the exact scalar loop at
lengths off the float4 block, `gelu_erf_batch` against an fp64 erf series and against its own
whole-image form, `rope_neox_tab_rows` with a per-row-table-index leg, `attention_bidir_windows`
against an oracle that attends strictly inside each ragged window with per-window content so a
leak is material, `interpolate_grid_bilinear_aa` down and up at a non-zero plane offset, the
audio front end's `build_fft_plan` / `fft_pow2_run` / `build_dft_twiddles` against an fp64
O(n^2) DFT of the same forward convention (plus the plan's non-power-of-2 refusal), the TTS
`resize_linear` / `resize_linear_torch` against an fp64 reference of each interpolation
convention, and the STFT pair `magnitude_phase` (with the zero-imaginary branch planted at both
zero signs - a bare `atan2` reads -pi where torch.angle reads +pi) and `istft_envelope_divide`
(four overlap-add geometries plus the zero-window leg that must leave the wave untouched through
the negligible-envelope guard).
`test_math_elementwise.das` - model-free: the `[tuned]` elementwise kernels (`dot`, `dot_bf16`,
`axpy`, `add_inplace`, `add_scale_inplace`, `mul_inplace`, `scale_inplace`, `copy_floats`) over
a 27-length sweep that reaches every width-8/unroll-2 tail (1..9, 15/16/17, 23/24/25, 31/32/33,
63/64/65, 100, 127/128/129, 1023/1024/1025). Inputs come from a seeded LCG tainted through a
`[sideeffects]` seam so no fixture folds at compile time, and every reference is accumulated in
`double` in-test. The two dots run twice: once on a dyadic lattice chosen so the whole fp32
accumulation is provably exact (products are multiples of 3/64 bounded well inside 2^24, so any
vectorized or reassociated reduction still bit-matches), and once on unit-range inputs against
a length-only bar `1e-7*n^2 + 1e-6` - the textbook sequential-FMA bound at |a|,|b| <= 1 - each
length shipping its must-EXCEED control, the expected result offset by an added 0.5. `dot_bf16`
builds its operands AS bf16 and widens them for the reference, so the compare never measures
input rounding. The six in-place kernels are exact-equality gates: their operand lattices make
the fp64 reference land on a representable float and differ from the input at every element by
construction (`copy_floats` writes over a 1e9 sentinel, `add_scale_inplace` uses positive
operands with a negative scale so the result cannot be the input, `mul_inplace`'s multiplier is
a multiple of 0.375 so it can never be one), and every cell asserts that count. A closing cell
pins a zero count as a no-op on all eight. `test_f32_to_f16_forms` holds the row convert
`cvt_f32_to_f16` (the resident decode's three host f16 copies) to the language's `float16()`
cast bit for bit over the tail sweep, the halfway ties, the subnormal band, the largest half,
both zeros and a value under the smallest subnormal, and holds its clamp past the half range
where the cast overflows, with the un-clamped cast as the control.
`test_math_activations.das` - model-free: the activation, norm and fp32 GEMM kernels against
closed forms and in-test fp64 references over the same tail sweep. `softmax_sink` is gated per
element plus the closed form its name carries - the row's sum plus the sink mass is one - at a
negligible sink (-1000, where the -80 clamp engages) and a dominant one, and by shift
invariance (adding 5 to the scores and to the sink leaves the distribution alone).
`softcap`/`softcap4` are gated as `cap*tanh(x/cap)` at caps 30 and 50 over inputs out to 200,
with the band assert `|out| <= cap`; `sigmoid_gate`/`sigmoid_gate4`, `swiglu_oai`/`swiglu_oai4`
and `silu_mul4` (writing over a sentinel destination) each get the scalar and vec4 forms against
the same fp64 reference, so the twins are held to an oracle rather than to each other.
`swiglu_oai` additionally carries the trained-clamp gate: a gate of 20 must read bit-identically
to a gate of 7, an up value of +-20 to +-7, with a gate pair inside the limit as the must-differ
control. `silu4_batch` is gated for split invariance - bit-identical to `silu4` over the whole
buffer inline and under a `with_job_que` scope, with the dispatch counter (`get_disp_count`) as
the anti-vacuous witness that the threaded leg really split - plus the two fallback shapes (row
length off the float4 grouping, single row) which must not dispatch. `l2_norm_rows` is gated
per element and by the unit-norm property over eight (rows, row_size) pairs, plus the eps floor
on a zero row and a 1e-9 row. `gemm_f32` and `gemm_f32_jo` run twelve shapes covering both
remainder strips and the pure 4x16 tile, each against an fp64 GEMM that starts from the
pre-initialized C (so the accumulate contract is part of the claim) and against each other
bit-for-bit. Every tolerance bar in the file ships its control in the same cell: the expected
value offset by an added 0.01, which must land outside the bar.
`test_q8q8_family.das` - model-free: the q8q8 kernel family end to end. Six widths with tails
(64, 96, 512, 1024, 1056, 3072) across six `[test]` functions (twelve cells), each judged by an in-test fp64 dequant
reference (int8 products summed exactly, both block scales applied in double) whose bar is the
per-block envelope times the block count, and each bar carrying a poison leg - 0.25 ADDED to
one expected element - that must EXCEED it. The stamped family repacks through
`repack_q8q8_grp` at the layout/wbias/kgroup companions and walks `q8q8_tile_gen` at the
tokstep companion's stride with a `q8q8_gemv_gen` token tail, bit-exact against per-token
GEMVs. The dispatch wrappers run on every backend `kernel_backend_names()` lists - repack
backends through their own `repack_q8q8_weight`, never on row-major data - with
`matmul_q8q8_batch` bit-exact against ntok x `matmul_q8q8`. The wscale_f16 arm covers
`dot_q8q8_f16s`, the `_s16` rows and groupN kernels, the stamped s16 tile/GEMV twins and the
`matmul_q8q8` / `_batch` / `_groupn` s16 overloads, against their f32 twins over f16-exact
scales - bit-exact on the portable backend, within the fp64 bar on a generated one whose s16
and f32 stamps fold differently. Where the `x64-gen` stamp is a bf16 tile (`q8q8_tileform_gen()`
above 0 - an x86 box with AMX bf16, nowhere else) the two batch compares, f32 and s16, hold
the bf16 envelope instead (`held_bf16`: every element within 1e-2 of the image's largest
magnitude - a NaN counts as outside - that magnitude added to one expected element landing
outside it), since the tile rounds every weight and activation to bf16 before the dot; on that
stamp the two stamped-tile cells assert their GEMV and the tile4 companion (the perm's 4-token
vector tile over the int8 planes, bit-exact against the per-token GEMV on every stamp, held to
the fp64 bar with its poison) and then register a skip and return, the stamped tile reading a
bf16 panel where the cell holds int8 planes. On an int8 amx stamp the two cells run the tile
itself, and call the family's witness first: it is the tile unit's grant, which a cell that
selects no backend would otherwise never ask for. The f32 batch cell runs at d=64 and again at d=48
(on a grp16 bf16 stamp: a two-group tile and a one-group tail), its batch output NaN-filled
before the run; the wrapper cells also run at ntok 4 and 9 (the decode step's shapes), where a
bf16 stamp's batch never reaches the panel and is bit-exact against ntok GEMVs.
`matmul_q8q8_group3` (f32 and s16) runs against three
independent GEMVs on unequal regions 32/40/44 (the row tail); `matmul_q8` / `dot_q8` cover the fp32-activation rail;
the mx4 cell drives `matmul_mx4q8_batch` and `matmul_mx4q8_batch_groupn` against ntok
independent `matmul_mx4q8` GEMVs and the `dot_mx4q8_scalar` leaf, pinned portable and swept
over the repack backends that carry those slots. Nothing here self-skips off-JIT - every cell
holds on the reference bodies.
`test_rope_apply.das` - model-free: the CPU rope APPLICATION leaves (`dasllama_math`'s
`rope_scaled`, `rope_scaled_neox`, the `_tab` and `_part` twins and `rope_apply`) against an
in-test fp64 angle reference over the same theta schedule, on head sizes 40/64/96/256 with
tails, positions 0/1/7/37/512 (4096 on the factored partial forms), two thetas, fscale and mscale
arms, the `rope_freqs` divisor on the full and the partial (`_part`, table and direct) forms, and
`yarn_rope_freqs`'s band against the NTK-by-parts formula spelled out in fp64 (full frequency
under the band, the factor above it, the ramp between, a shifted-band poison past the bar)
- the bar is one rotation's f32 accumulation plus the f32 angle's own rounding, each compare a
`_compares` `check` that logs its largest difference by head and dim, with an added-value poison
that must exceed the bar. The `_tab` forms also ride a tight twin bar against the
un-tabled forms (loose only by the cross-compilation-unit cos/sin ulp drift), the `_part` forms
are bit-exact against a full apply over the gathered rotated prefix with the un-rotated dims
proven to pass through, and `rope_apply` is bit-exact against the leaf its `neox` flag names,
including the `use_ff = false` p-RoPE arm. `build_rope_tabs` over no positions leaves both tables
empty, a table that held rows cut to it. The Apple-only Metal rope tests use these kernels as
their oracle; this file gates them on every platform.
`test_prefill_cpu_kernels.das` - model-free: the prefill and KV CPU kernels, on q4_K / q6_K /
q4_0 synthetic disk planes built in-file. `matmul_kq_batch` and
`matmul_kq_batch_groupn` - the per-position and per-expert GEMV routes a tier with no kq batch
slot runs, bit-matched against per-(token,row) and per-(region,token) disk dots, with no skip on
any tier; where a kq-carrying backend can be pinned (restored on exit) the native batched
kernels additionally ride bit-for-bit against `matmul_kq_active` and the rows-core GEMVs, and
against the disk dots within the f32 fold order (1e-5 of the image's largest magnitude, that
magnitude added to one expected element landing outside it) -
or, where the format's stamp is a bf16 tile (`kq_tileform_of(fmt)` above 0, an x86 box with AMX
bf16) and the batch is at least the tile's 32 tokens, the batch against the disk dots and
against `matmul_kq_active` within the bf16 envelope (1e-2 of the image's largest magnitude, a
NaN outside it, that magnitude added to one expected element landing outside it); a shorter
batch on that stamp never reaches the panel - its walk is the tile4 companion and the rows core -
and is bit-exact. The batch cells run at ntok 1/4/5/9/13, at n=1024 d=64 ntok=6, and at
n=512 d=48 ntok=67 - on a grp16 bf16 stamp the shape that reaches the tile, its token tail and
its group tail; the region walk runs 1/4/6 and 3/35/67 (n=512 d=48), the long runs under the
envelope on a bf16 stamp and exact off it. Every batch output is NaN-filled before its run.
`test_xbf16_build` holds the bf16 activation plane, built over a stale plane of other values:
every element within half a bf16 step of q times its superblock scale (control: the truncated
bf16 must miss), the plane on a 64-byte boundary; and two threads building at once, each over
its own activation row for 400 rounds, each reading its own plane back (the two planes at
distinct addresses, the two rows differing). The threads are `new_thread` contexts, the shape
two concurrent inferences take - a team lane owns no globals and builds no plane.
`cvt_q8kv_to_f32` / `kv_row_to_f32` (all four overloads, at a non-zero `kv_head_off` base)
against `quantize_q8kv_row`'s stored blocks plus a half-step round-trip assert.
`requant_rows_q8` vs `quantize_q8_0` per row and `requant_rows_q8k_bs` vs an in-test Q8_K
reference, on both `par` arms. `repack_kq_weight`'s round trip: the pure grp repack read back
through `kq_grp_row_dot` on every tier, then the dispatch seam - byte-identical to that layout
where the tier carries kq slots, the registered identity where it does not. Every compare above
ships an added-value poison control. `test_walk_panels` holds the batch walk's panel scratch on
the generated backend, pinned for the cell and put back through `kernel_backend_pin`, and skips
where that backend carries no kq batch kernel: two `new_thread` contexts walk k6 at once (n=512
d=48 ntok=67, 200 rounds each), each over its own planes and on its calling thread, and read the
main thread's walk of the same inputs bit for bit, the two outputs differing; then one context
walks across the job queue (n=512 d=128, 200 rounds), the batch walk and the region walk over
two token runs, each bit for bit the same walk on its calling thread alone, every round
dispatched (the dispatch counter), the dispatched-walk assert skipped on a box of one dispatch
slot. Both log their mismatch counts and the first element off by round, token and row; neither
carries a poison - the control is the kernel with every slot on one panel, which reads over a
million elements off. `test_bf16_token_block` holds the bf16 walk's block off the knob (1, 128
and 500 read 512, 1000 reads 992, 1024 reads 1024, each a whole number of tiles) and under an L2
budget that cuts the int8 walk's block to 215 at n=9728, where the bf16 block stays 512; it
restores both knobs from their getters.
`test_attn_span.das` - stocked suite; the non-causal image span (`eval_embd_` with `non_causal =
true`): mask direction by perturbation (causal row 0 blind to the last row, span row 0 sees it;
the same direction pair again on the q4_0 requant of the fixture - the embedding prefill on the
q4_0 tier), classic/flash agreement, and the flag-reset bit-exactness; plus the FUSED mid-turn span
(`eval_embd_span_`): splice equivalence via a whole-cache decode-logits witness (classic
bit-exact, flash tolerance) and the per-query mask direction inside one eval; plus the deepstack
wide-row rail (stamped `n_deepstack`): zero-tail wide == narrow bit-exact, nonzero tails move the
logits, slice 0 vs slice 2 add at different depths, no stale plane after the quantum; stories15M
fixture (test_flash's), skips without it.
`test_cpu_prefill_tripwire.das` - the CPU-prefill guard: an undeclared prefill trips - span,
causal, MROPE-quantum, and DEEPSTACK-quantum alike (the metal rail serves all four, so a
CPU-served one is a silent fallback);
same stories15M fixture, deliberately never calls `allow_cpu_prefill()` (which is why it
cannot live in test_attn_span - that file arms it in `[init]`). Metal-capable builds only;
plain dastest only.
`test_vision_chat.das` - stocked suite; the image chat turn end to end, one pair per vision
carrier plus the showcase (the 12B pairs and the Omni pair skip by name on a box whose RAM is
short of their served planes - `box_ram_gb`, the container's limit where one caps the process -
since a 12B Q4_K_M serves 45 GB at f32 on the CPU chain): the 12B gemma4uv pair (the cats fixture,
so `DASLLAMA_PARITY_FULL=1`),
the E2B gemma4v pair (E2B Q8 decoder + bf16 mmproj - small tier, runs without the flag), the
E2B instruct-mode cell (the Q4_K_M decoder + the same mmproj, thinking off, the image on the
first turn under the chat sampler preset at seed 1234: the turn answers with the cats in the
content half and the history holds no thought - red without the channel-marker guard), the
gemma-3-4b gemma3v pair (small tier), the gemma-3-12b pair (the same SigLIP tower at projection
3840 - large tier, `DASLLAMA_PARITY_FULL=1`), and the Qwen3-Omni qwen3v pair (large tier - the
mrope leg: grid reaches the chat, the session's rope delta reflects the grid advance), the
Qwen3-VL 4B deepstack pair (small tier - wide 10240-float rows through the chat, the caption, and
the zeroed-slices decoder control: tails zeroed on the same rows must move the prefill logits,
measured 10.4 - a caption alone cannot see a decoder that ignores the slices), the rows-seam cell
(same 4B pair - pre-encoded wide rows through `add_user_image_rows_` on a plain chat reproduce the
embedder walk token-for-token; the seam lcpp_bench prices, where a hand-splice can feed deepstack
decoders a narrow scrambled span), the Qwen2.5-Omni-3B qwen25v pair (small tier - the window ViT +
the qwen2vl NON-interleaved MROPE decoder; the vocab spells the span markers
`<|vision_bos|>`/`<|vision_eos|>`, resolved by the chat layer's vocab-driven fallback) plus the
`test_omni_showcase` cell in the same file (one Omni session: an image turn, then a text turn
whose answer needs the image turn across the mrope position delta; the chat AudioTower serves the
whisper-class families only): the prompt stream shape around the splice (marker ids, media-first,
span length from the geometry) and the greedy caption, logged in full. NOT token-parity with
llama-mtmd-cli - the oracle renders its jinja template in thinking mode while dasLLAMA's gemma-4
arm defaults to instruct. On Apple builds the turn also carries the tower legs: the default
caption's image encode must show an encodes delta (GPU-served), then a knob-off chat repeats the
turn on the CPU embedder and must clear the same caption bar - the caption names the cats and is a
description, not a fragment - with zero tower dispatches and the knob decline counted.

## Model loads - never the image rail

The loader obligation is `REVIEW.md`'s. The mechanism: the `.dlim` image rail stamps every
mint with the box identity (backend pin, wscale, tune manifest), and GC-purges sibling
flavors. A suite child's identity under a lane pin or `--no-tune` differs from the serving rig's. So a suite on the
rail both re-mints multi-GB images the rig cannot use and purges the flavors the rig depends
on. A cell that loads under a lane pin - a `set_<family>_q8`-class knob, or a
`set_metal_tensor_crowns` / `pin_metal_tensor_crowns` pin - is the sharpest case: a disk bake
under the pinned lane purges the serving lane's `.dlim` beside the model, and the next
direct-image load in another suite panics on the wrong identity. Image-rail coverage (mint, map,
GC, flavors) lives in the image suites alone (`test_model_image`, `test_model_image_vulkan`).

The suite cells that mint a model image by design are ledgered at
`../ARCHITECTURE_IMAGE_LEDGER.md#suite-image-mints`.

## Metal fixtures - driver knobs and the two-model pattern

The establish-and-restore obligation is `REVIEW_LANE_PINS.md`'s. The mechanism: the hooks are on by
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
`coverage`, plus the image `smol`, `untied`, `metal` and `metal-untied` arms and the `image-vulkan` `vulkan` arm),
`qwen2`, `qwen3`, `phi3`,
`gemma2`, `gemma3`, `gemma4`, `qwen3moe`, `gemma4moe`, `gptoss`, `qwen35`, `qwen35moe`, `qwen2moe`, `qwen4exp` (the support-matrix family cells; `qwen35` also tags `test_vulkan_mint.das` and `test_metal_batched_row.das`), `gemma`,
`ultravox`, `whisper`, `voxtral`, `parakeet`, `gigaam`, `qwen3a`, `canary`, `gemma4a` (image suite arms),
`gemma3v`, `qwen25v`, `qwen3v` (the coverage census tower rows), `kitten` (the image suite's
kitten arm), `kokoro`, `pocket` (the coverage census TTS rows), `vad` (the coverage census silero VAD row), `bergamot` (the image suite's
bergamot arm and the coverage census Bergamot row, ru-en tiny on the Metal seat),
`gemma4e` (support-matrix rows under `fam-gemma4e` - E4B PARITY_FULL-gated; E2B Q8_0, Q4_K_M,
Q4_0 and the format carriers of `../performance/model_specs.das` (the token and PLE tables Q6_K and
IQ4_XS, the per-layer gate and projection planes Q6_K and Q5_0) small-tier always-on - what the rows assert is the
support matrix's `fam-gemma4e` clause; plus the coverage-census E4B row; `gptoss`/`qwen2moe`/`qwen3moe` carry census rows too - all PARITY_FULL-gated).
When profiling one family across formats, gate each round with
`--arm <arms> --family <fam>` instead of the whole zoo. The family tag on a new
model-loading block is `REVIEW.md`'s obligation.

## Model tiers

`model_available(t, path)` is the size gate; the tier rule is `REVIEW.md`'s. Set
`DASLLAMA_PARITY_FULL=1` explicitly with an `--arm` run when a change genuinely needs the
large tier. A run reporting SKIPPED for those arms is correct, not a failure. `REVIEW.md` bans a
batched-vs-sequential parity cell on a carrier above the tier: streaming a 70B's 40GB on the CPU
while the GPU has the same bytes wired OOM-kills a 64GB box, so small-model pins carry that
coverage instead (e.g. `set_metal_batch_addrms_unfused`). The one ledgered cell above the tier is
`test_batch_decode.das`'s Qwen3.6-35B-A3B cell: no stocked carrier under the tier takes the
f32-on-disk beta/alpha arm of the CPU deltanet batch, and the cell gates on `DASLLAMA_PARITY_FULL=1`.

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
with the GPU side actually CORRECT (it matches the current CPU chain). Before declaring a fam-row red a
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
  (the hyphenated directory is unreachable by path require). `test_openai_server_audio.das`'s
  `test_openai_server_audio_tower` serves an ultravox mmproj on a stock Llama-3.2-1B through the
  audio arm alone: no vision arm on stats, and a clip adding exactly its 187 rows to the text-only
  prompt count - no marker token - with the clip after the text and ahead of it, each reply logged.
- `utils/dasllama-server/test_constraints.das` - requires the server's `constraints` module by bare
  same-dir name: the chat request's `response_format` / `tool_choice` fields read to a grammar text
  or a refusal (model-free), and the grammar constraint walking tinyllama's own token pieces along a
  schema's sentence, the stop token admitted at its end alone (JIT, skips without the model).
- `utils/dasllama-server/test_worker_dispatch.das` - requires the server (`openai_server`) by
  bare same-dir name, like the server suites beside it.
- `utils/dasllama-server/test_server_flags.das` - requires the server's program root (`main`) and
  `openai_server` by bare same-dir name; its ctx-clamp cell doubles the engine's K/V room seam
  (`dasllama_gpu_tier`) under the internal escape, so a blob-only Model shell's `seq_len` is cut to
  the rows the seam answers while a planar shell's stands; its slot-codec cell holds the K/V codec
  a slot runs on (`slot_kv_dtype`, which the live-load clamp reads): the server default for a slot
  that asked for none, its own for one that asked; its speculating-slot cell (`test_slot_mtp_device_home`, Qwen3.5-0.8B-MTP
  under `DASLLAMA_GPU=1` and the JIT, skips without them) builds a slot over the loaded model, holds that
  `slot_wants_device_kv` homes its stream on the device and the model card says so, then steps a greedy stream and holds the
  device's draft and verify counters up by the slot's drafted rounds; its budget cell holds `slot_prefix_states` over a synthetic
  one-recurrent-layer Config (704 state floats, a 35584-byte checkpoint): 1024 for a budget of 0 or under, 256 under 256
  checkpoints' bytes, the whole checkpoints between, 1024 past 1024.
- `utils/dasllama-server/test_exchange_client.das` - requires `dasllama/dasllama_exchange` by
  registered name (nothing pins it to that directory); it stays beside the server suites
  because its fixed test port is coordinated with theirs (see its `TEST_PORT` note).
- `modules/dasLLVM/tests/test_grid.das`, `test_tune.das`, `test_tuned.das` - the tune
  framework's own tests; they require `dasllama/dasllama_tune` (the tuner contract) and
  live with dasLLVM under `modules/dasLLVM/REVIEW.md`.
- `modules/dasLLAMA/benchmarks/matmul/test_matmul_par.das` - the bench self-check, requiring
  `matmul_variants` by bare same-dir name.
- `utils/dasllama-convert/test_convert.das` - requires the converter's `convert_sniff` module by bare
  same-dir name (the hyphenated directory is unreachable by path require): the carrier sniff over
  the GigaAM carriers, a decoder and a Qwen3-ASR mmproj, gated through `_model_tier.das`'s
  `model_available` (path-required), and the absent-file cell; run `-jit`
  (`run_tests_dasllama_convert`).

The ladder tests (`utils/internal/dasllama-ladder/test_*.das`) answer to that folder's own
`REVIEW.md` - not dasLLAMA tests, not ledger entries. That holds even though two of them,
`test_ladder_store.das` and `test_ladder_server.das`, reach `ladder_store.das`, which
requires `dasllama/dasllama_exchange_schema` publicly.
