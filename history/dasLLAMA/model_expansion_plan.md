# Model-expansion plan — LLM + audio, post-audio-perf arc

Scoped 2026-07-07 (discussion during PR #3401 babysit). Goal: bring the supported-model
matrix back to "current" on both sides — LLM ("up to date" = the full Gemma-4 family) and
audio (the 2026 crop of audio-embedded and leaderboard ASR models) — every model to the
standing dasLLAMA bar: **token-for-token vs its reference implementation**, README matrix
row + parity fixture per model, one branch per wave.

Landscape facts referenced below were checked 2026-07-07 (Gemma-4 family/licensing per
Google's docs; ASR leaderboard per the Open ASR snapshot). Anything marked **VERIFY** is a
wave-start check, not a settled fact — do not build on it before probing.

## CPU speed reality (measured 2026-07-07, M1 8T, run.das)

Decode is bandwidth-bound: tok/s ≈ effective GEMV rate (~90-95 GB/s measured on M1) ÷
active bytes per token. Measured anchors: **gpt-oss-20b (MoE, ~3.6B active) 30 t/s gen**;
**Llama-3.1-8B dense Q8 11 t/s gen** — both sit exactly on that rail.

Implications for this plan:
- **Qwen3-30B-A3B ≈ 25-30 t/s** (q8) — fully interactive; the A3B shape is the point.
- **Gemma-4-26B-A4B ≈ 20 t/s** — interactive.
- **Gemma-4-31B dense ≈ 3 t/s q8 / ~6-7 q4** — NOT interactive on our boxes; it is a
  coverage/verification row and a prefill workhorse, not a chat target.
- **Gemma-4-E2B/E4B** — small active sets, fastest chat models in the plan; they pair
  with the audio stack for the edge/dictation story.
- Dense interactive ceiling on M1 ≈ 8B q8 (11 t/s) / ~14B q4. Memory-RESIDENT size is
  almost irrelevant to speed; active-streamed bytes are everything.
- K-quants note: at these sizes q4-class weights are also a straight ~2x decode speedup
  over q8, not just a disk saving — one more pull-forward argument.

## Standing rules (inherited, non-negotiable)

- Token-for-token oracle per model; frozen fixture in `test_parity.das` (LLM) or the
  audio suite; README matrix row lands in the same wave as the model.
- New arch features go through the arch registry / std_blocks seams — the core forward
  pass does not grow per-model branches.
- Perf ideas spotted mid-wave go to the API_REWORK.md performance ledger, never chased
  mid-wave.
- Correctness waves run fp32 first; q8 is a follow-up knob with its own gate class.
- Models live in `~/Work/llama.cpp/models` (gitignored), self-conversion recipes recorded
  in the plan of the wave that introduced them.
- **Every LLM section closes with a prefill AND decode A/B vs llama.cpp** (Boris,
  2026-07-07). Correctness token-for-token is necessary but not the finish line: each
  model's section ends with a prefill throughput A/B (das vs `llama-bench`/`simple_ids`
  CPU `-ngl 0`) and a decode/emission A/B (`llama-bench -p 0 -n 128`), on a Parsec-off
  window (announce + wait for go — [[parsec-off-before-profiling]]), the das side re-run
  vs the stored lcpp baseline. **The prefill under test MUST be the batched path, never
  the naive `forward`-per-token loop** — verify q8 engages the batched `forward_prefill`
  (the naive fallback is q4/npos≤0 only; for a new MoE arch confirm the grouped path
  fires). Precedent: Wave Q (prefill 1.59× / decode 1.36× vs lcpp), Wave G2
  (prefill grouped 5.5× → leads lcpp 1.44×).

## Wave G — Gemma-4 family completion (the named target)

We have gemma-4-12B-it verified (arch: heterogeneous sliding/global geometry, p-RoPE,
V-from-K global layers, per-layer output scale, final softcap). The family is five models;
four remain. Order inside the wave = cheapest first, each gated before the next starts.

1. **gemma-4-31B dense — ✅ DONE 2026-07-07 (verify-only, zero code).** Header confirmed
   the same gemma4 arch as the 12B, scaled: 60 layers, embd 5376, 32Q / [16×5,4]KV heads;
   PLE off, shared_kv 0, softcap 30, 5-local:1-global SWA. Ran the existing gemma4 forward
   unchanged → **40/40 token-for-token** vs simple_ids (harness/parity.sh). Fixture
   `test_parity_gemma4_31b` (reuses GEMMA4_12B_GEN — identical counting continuation) +
   README matrix row. Committed on `bbatkin/model-expansion` (`19d7712cd`). Source:
   `ggml-org/gemma-4-31B-it-GGUF` (Q8 + bf16 mmproj), direct GGUF.
2. **gemma-4-26B-A4B MoE — ✅ DONE 2026-07-07. Real MoE slice; ~17 tok/s decode (M1 Max, 4B active).**
   Shipped: config `moe_optional`/`moe_dense_shexp` (arch is MoE only when the file ships experts) +
   5 new fblob regions (router scale, per-expert down scale, `pre/post_ffw_norm_1/2`); loader splits
   the fused `ffn_gate_up_exps` into the existing we1(gate)/we3(up) stacks at load (block-aligned Q8
   slices) + allocates/loads the dense shared expert (`ffn_gate/up/down` → w1/w3/w2 even with
   n_expert>0); forward `gemma4_moe_ffn` (custom router → select → per-expert down scale folded into
   the routed weights → `moe_experts_apply` → sandwich norms → dense-shared-expert sum), reusing the
   extracted `moe_experts_apply` (from `moe_ffn_core`) + `dense_ffn_inplace` (from `ffn_dense_decode`)
   so qwen2moe/gpt-oss/dense are byte-unchanged (full sweep 33/34 confirms). `gemma4_blocks()`
   dispatches dense (12B/31B) vs MoE (26B) by expert count.
   **KEY LESSON — MoE routing is near-tie-brittle.** The router RMS feeds a DISCRETE top-8-of-128
   pick; ggml accumulates it in `ggml_float` (double). With float, dasLLAMA flips experts at router
   near-ties even where the reference is *confident* → real O(1) miscounts. Fix = double-accumulate
   ONLY the router sum-of-squares (2816 adds/layer, non-hot — matches ggml; every GEMM + the residual
   stream stay float; approved by Boris: "double in a hot loop would be a problem, this isn't"). That
   removed the systematic flips (counting 3→1 miss). The survivor is a GENUINE near-tie: verified vs
   llama.cpp's per-step logit margins, the 26B's own top-1/top-2 gap is ~0.5–2.2 at counting steps
   12/26/32/38 — a coin-flip no float engine can pin. So NO 40-token prompt holds token-for-token
   (counting 34/40, days 16, prose/repetition worse — the "long" matches are repetition loops).
   Fixture = the counting prompt capped at its confident 11-token head (`GEMMA4_26B_GEN`); correctness
   proven separately via the margin analysis, not the fixture length. Diagnostic tooling:
   `harness/oracle` `simple_ids_margin`/`simple_ids_multi` (per-step logit margins + multi-prompt).
   ~~PROBED 2026-07-07; a real MoE slice (medium).~~
   Header: gemma4 arch, 30 layers, embd 2816, ff 2112, 16Q / [8×5,2]KV heads, **128 experts
   top-8**, expert_ff 704, softcap 30, SWA 1024, PLE off, shared_kv 0. The gemma4 MoE
   (`src/models/gemma4.cpp` `is_moe_layer` branch, lines ~291-344) is **two parallel branches
   SUMMED** (`cur = shared + routed`):
   - **Shared expert** — dense GeGLU FFN: `ffn_norm` → build_ffn(GELU, PAR) → `ffn_post_norm_1`. Always on.
   - **Routed experts** — `ffn_pre_norm_2` → MoE → `ffn_post_norm_2`. Router is *custom-normalized*:
     `RMS_norm(attn_out) × 1/√n_embd × learned ffn_gate_inp_s`, then `ffn_gate_inp` matmul → logits.
     **Softmax top-8-of-128 WITH renorm** (`norm_w=true`). Experts are **fused gate_up**
     (`ffn_gate_up_exps`, dim n_ff_exp·2) GeGLU + per-expert output scales
     (`ffn_up_exps_s`/`gate_exps_s`/`down_exps_s`).
   **Already in dasLLAMA** (proven by 12B/31B 40/40): attn geometry, dense GeGLU, per-tensor
   `_s` scale machinery. **NEW code** (in `dasllama_arch_gemma4.das`, a MoE-layer branch): the
   router normalization + softmax/renorm top-8, fused-gate_up experts w/ scales, the parallel
   dense-shared-expert sum, and the extra sandwich norms (`ffn_pre_norm_2`,
   `ffn_post_norm_1/2`). Rides the existing routed-expert GEMM seam (qwen2moe/gpt-oss).
   **vs qwen2moe** (the closest): gemma4 renormalizes top-k (qwen2moe doesn't), has a learned
   router input-scale + RMS/√d normalization (qwen2moe's router is a plain matmul), fuses
   gate_up (qwen2moe separate), and its shared expert is a **dense parallel** FFN (qwen2moe's
   is sigmoid-gated). Then 26B parity 40/40 (harness/parity.sh) + `test_parity_gemma4_26b`
   fixture (large-tier) + README row. Source: `ggml-org/gemma-4-26B-A4B-it-GGUF` (Q8 + bf16
   mmproj), on disk 2026-07-07. 4B active = strong CPU target, likely the family's perf sweet spot.
   **PREFILL grouped-batched (✅ 2026-07-07).** The 26B prefill was naive per-token (each position
   looped through `gemma4_moe_ffn`) → **35 t/s, 0.23× lcpp**. New `gemma4_moe_prefill_grouped` +
   `gemma4_router_batch` route every position, bucket (position, slot) by expert (CSR), run one Q8
   batch GEMM chain per touched expert (reusing `moe_gather_rows` + the qwen2moe grouped machinery),
   and batch the dense parallel shared expert over all positions — summed under the shared
   post_ffw_norm. Bit-identical per row to the naive path (same quants/dots, slot-order reduce, router
   SoS in double), gated by the existing `g_moe_grouped_prefill` A/B toggle; no new Session buffers.
   Measured (M1 Max, prefill-512, interleaved A/B): **grouped 217 t/s vs naive 39 = 5.5×**, and **217
   vs lcpp 151.1 = 1.44× — das now LEADS lcpp on 26B prefill.** Correctness held: the counting-prompt
   1492-token prefill still produces `GEMMA4_26B_GEN` token-for-token, grouped==naive==fixture.
3. **gemma-4-E2B / E4B — the edge models. ✅ TEXT DONE 2026-07-07 (2 commits: E4B then
   E2B). 40/40 token-for-token, both flash + classic prefill, window engaged.** The slice
   was 3 features, not the "PLE + KV-sharing" 2 the pre-probe sized — the real GGUF headers
   added a THIRD (E2B's per-layer FFN width). All three landed:
   - **PLE** (both): new fblob regions (fp32 `per_layer_model_proj` + `per_layer_proj_norm`
     + per-layer `post_norm`) + Q8 big weights (the `[vocab × ple·layers]` token table,
     per-layer `inp_gate`/`proj`). Pre-step per token (gather ×√ple; project ×1/√dim →
     RMSNorm → +gathered → ×1/√2) + a per-layer block after the FFN residual, before the
     layer scale: `cur += post_norm(proj·(gelu(inp_gate·cur) ⊙ ple_inp[l]))`. 🔑 the token
     table is `[vocab × (ple·layers)]` — load ALL `ple·layers` per row (the initial bug was
     loading only `ple`).
   - **Cross-layer KV sharing** (both): `n_layer_kv_from_start = layers - shared_kv_layers`
     (15 for E2B, 24 for E4B). It's SEMANTICALLY LOAD-BEARING, not just a memory opt — the
     shared layers' own `attn_k`/`attn_v` are DEAD weights in the file (llama.cpp gates KV
     *use* on `has_kv(il)`, not load). Shared layers project Q only and attend against the
     source layer's cache (sliding → `kv_from_start-2`, global → `kv_from_start-1`). Skipped
     at load (memory win); `kv_row_prefix` aliases the shared slot onto its source so
     `kv_cache_off` is unchanged in the hot path; decode + prefill attention gained a Q-only
     branch. `layer_kv_source(c,l)` mirrors llama.cpp gemma4's `reuse()`.
   - **Per-layer FFN width (E2B ONLY)** — `feed_forward_length` is a per-layer ARRAY on E2B
     (6144 ×15 then 12288 ×20 — the elastic MatFormer sub-model of the E4B; E4B is uniform
     10240). `w1_off`/`w2_off`/`w3_off` became per-layer arrays `w*_offs` filled by
     `fill_dense_ffn_offsets` (kept as separate contiguous regions so the llama2.c checkpoint
     block-copy survives); `ffn_w[l]` + `layer_hidden(t,l)`; `hidden_dim` = max (scratch).
     🔑 `ffn_w` lives on the **Model**, NOT Config — Config must stay copyable (`let c =
     t.config` is everywhere; an array field breaks it).
   - Fixtures both reuse `GEMMA4_12B_GEN` (byte-identical counting continuation). E4B
     large-tier (~8 GB, `DASLLAMA_PARITY_FULL=1`); E2B fast-tier (~5 GB, default loop).
     Existing arches byte-unchanged (all new paths gate on `has_ple` / `kv_src[l]!=l` /
     `ffn_w`, all off elsewhere); parity suite green. The channel-aware chat template + the
     per-class head-size machinery were already in place from G1/G2.
   - **✅ Section-closing prefill + decode A/B vs lcpp** (M1 Max, Parsec off, box profile,
     das best-of-4; lcpp `llama-bench -ngl 0 -t 8`). Prefill rides the batched
     `forward_prefill_body` (batched attn + `ffn_dense_prefill` + `ple_block_prefill`), NOT
     naive. **das is at PARITY on the dense E-series — no lead, no meaningful gap:**
     - **E2B** — prefill das **376 t/s** (pp512) / 405 (pp256) vs lcpp **382** = **0.98×**
       (ahead at pp256); decode das **36.0 t/s** (ctx8) vs lcpp tg128 **36.9** = **0.98×**.
     - **E4B** — prefill das **178.7 t/s** (pp512) vs lcpp **192.9** = **0.93×**; decode das
       **18.7 t/s** (ctx8) vs lcpp tg128 **19.2** = **0.98×**.
     Unlike the MoE waves (Q/G2 LED lcpp via GROUPED prefill — a sparsity lever), these are
     DENSE: prefill has no batching win to exploit, so it's das NEON-SDOT vs lcpp
     Accelerate-BLAS (AMX-backed), and the larger E4B GEMM favors AMX more (hence 0.93× vs
     E2B's 0.98×). Decode is bandwidth-bound → tied both models. Rigs: `benchmarks/
     prefill_perf.das` (batched vs per-token), `benchmarks/decode_prof.das` (two ctx windows).
     **Ledger:** the E4B dense-prefill 0.93× is the AMX-access asymmetry (documented, boxes
     w/o AMX exposure). Potential das-side lever: a gemm-gen tuned Q8 kernel for the E-series
     dims (dim 2560 × ff 10240) — not chased mid-wave.

   Original probe notes (all resolved above):
   PROBED 2026-07-07; slice RE-SIZED DOWN to "PLE + KV-sharing," best case. Read of
   `src/models/gemma4.cpp` vs `gemma3n.cpp` plus E2B/E4B header dumps settled every VERIFY:
   - **PLE — ON & confirmed.** `embedding_length_per_layer_input = 256` on both E2B and
     E4B (the 12B has it 0 → PLE is an E-series-only feature). Tables: `per_layer_tok_embd`
     {256·n_layer, n_vocab}, `per_layer_model_proj`, `per_layer_proj_norm`, plus per-layer
     `per_layer_inp_gate`/`per_layer_proj`/`per_layer_post_norm`. One lookup+project+add
     per layer per token. Loader region + one std_block addition; token-indexed reads suit
     the fmap discipline (tables never need to be resident).
   - **AltUp / LAuReL / MatFormer / activation-sparsity — ALL GONE.** grep count 0 in
     gemma4.cpp; gemma3n.cpp still carries the full bag (7 AltUp + 3 LAuReL tensors/layer).
     No new sub-blocks, no new forward-pass math — the biggest cost items in the pre-probe
     estimate all evaporated. This is the "PLE only" best case.
   - **Cross-layer KV sharing — PRESENT & USED.** `shared_kv_layers` = 20/35 (E2B),
     18/42 (E4B); 12B = 0. Local layers reuse a global layer's KV — real change to the
     per-layer KV-cache indexing; moderate, and a decode-bandwidth win.
   - Geometry: SWA `sliding_window` 512 with per-layer `sliding_window_pattern`;
     `final_logit_softcapping` 30.0; heads 8, kv 1 (E2B) / 2 (E4B); embd 1536 (E2B) /
     2560 (E4B); ctx 131072; blocks 35 (E2B) / 42 (E4B).
   - **Chat template — reasoning model.** gemma-4 emits `<|channel>thought …` blocks
     (harmony/gpt-oss-style channels); the template + stop handling must parse channels,
     not plain turns. Register a gemma-4 template variant; `--jinja` is required in the
     oracle recipe.
   Net slice = PLE loader+block + KV-sharing indexing + channel-aware template. No
   AltUp/LAuReL work. Text path first; audio deferred to Wave A2 — same models, two waves,
   so the text parity fixture exists before tower work starts. Payoff: the E-models are the
   audio carriers AND the fastest chat models in the plan (small active set on the
   bandwidth rail). Models: direct GGUF from `ggml-org/gemma-4-E{2,4}B-it-GGUF` (Q8 text +
   bf16 audio/vision mmproj), downloaded 2026-07-07 — no conversion.

Oracle throughout: llama.cpp `simple_ids` / `harness/parity.sh` (gemma4 support already
proven by the 12B row). Chat template: gemma family template already registered; E-series
**VERIFY** for deltas.

## Wave Q — Qwen3-MoE (best value/cost on the LLM side) — ✅ CORRECTNESS DONE 2026-07-07

**Qwen3-30B-A3B — ✅ DONE (config-only arch, zero new forward code).** VERIFY resolved GREEN
against llama.cpp `build_qwen3moe` (`src/models/qwen3moe.cpp`): standard attention with per-head
Q/K-norm before RoPE (our `c.qk_norm`), `1/√head` scale, no QKV bias; `build_moe_ffn` with softmax
router + `norm_w=true` (renormalized top-k) + `nullptr` shared-expert gate (none). So the whole slice
is `configure_qwen3moe = rope_neox + qk_norm + moe + norm_topk_prob` on `moe_blocks()` (new file
`dasllama_arch_qwen3moe.das`, +transformer require, +`.das_module`/CMake/AOT lists). The loader
already reads `expert_weights_norm`/`expert_shared_feed_forward_length`; the router `moe_select`
already renormalizes; the grouped prefill already handles `nsh==0`. Header (confirmed): 48 layers,
embd 2048, ff 6144, 32Q/4KV heads (head_size 128), **128 experts top-8**, expert_ff 768.
- **Counting 40/40 token-for-token** (`harness/parity.sh`), both grouped + reference prefill paths,
  bit-identical. Fixture `test_parity_qwen3moe_30b` (reuses `QWEN3_PROMPT`/`QWEN3_GEN`, large-tier).
- **NO double-RMS needed** (vs gemma-26B). Margin analysis (`simple_ids_margin`) on a prose probe:
  das tracks the oracle through margins as tight as **0.08 logits** with no systematic router flips —
  the shared float `ffn_norm` already matches ggml's double-accumulated norm closely enough for the
  top-8-of-128 pick. Prose diverges only at the model's OWN first near-tie (oracle top-1/top-2 gap
  **0.04 at step 6**, das picks the oracle's #2 — a coin-flip, not a bug). Better than gemma-26B
  (capped at 11): qwen3moe holds the full 40 on the confident counting continuation.
- **Prefill A/B (M1 Max, PP=512, Parsec off, box profile, interleaved best-of-5):** das grouped
  **206.8 t/s** vs das naive per-position 44.5 = **4.64×** (internal sanity — grouped batching engaged,
  like the 26B's 5.5×); vs lcpp CPU `-ngl 0 -t 8` **130.0 t/s** = **das LEADS lcpp 1.59×**. Robust:
  the pessimistic settled das rep (195) still beats lcpp's optimistic end (136) by 1.43×. Grouped is
  the shipped default (`ffn_moe_prefill` dispatches it for q8); reference path kept as the A/B lever.
- **Decode / emission (M1 Max, box profile):** das **27.5 tok/s** fresh (ctx 8) / 24.8 (ctx 512) vs
  lcpp CPU tg128 `-ngl 0 -t 8` **20.2 t/s** = **das LEADS lcpp 1.36×**. Lands in the plan's predicted
  25-30 t/s band — fully interactive; the A3B (3B-active) shape delivering as expected. So das leads
  lcpp on BOTH prefill (1.59×) and decode (1.36×) for qwen3moe-30B.
- The 235B-A22B header-tolerance row was NOT attempted (doesn't fit; deferred).

Original scope notes (all resolved above): qwen2moe wiring + qwen3's QK-norm, **renormalized** top-k
router, **no shared expert**. Small slice; 3B-active is now the best chat model per CPU-second we ship.
Unlocks the Wave A3 Omni thinker. Oracle: `simple_ids`, standard.

**Prefill = grouped batched, NOT naive `forward`-per-token (Boris, 2026-07-07).** qwen3moe is
qwen2moe-shaped, so its prefill rides the EXISTING `ffn_moe_prefill` → `ffn_moe_prefill_grouped`
path (register `moe_blocks()`; the no-shared-expert case `nsh==0` is already handled). Naive
per-token is allowed only as an INTERMEDIATE correctness step — the shipped prefill must batch.
Verify q8 actually engages the grouped path (`forward_prefill` runs per-token `forward()` for
`q4 || npos≤0`; `g_moe_grouped_prefill` gates the rest). Template: the gemma-4-26B
`gemma4_moe_prefill_grouped` slice (2026-07-07, 5.5× → leads lcpp 1.44×).

**Session closes with A/B profiling vs llama.cpp (Boris, 2026-07-07).** Prefill-512 interleaved
A/B (das grouped vs the stored lcpp baseline; grouped-vs-naive as the internal sanity). Rig =
adapt `harness/tune_confirm_prefill.das` into an interleaved A/B (throwaway). Announce a Parsec-off
window and wait for go before profiling; re-run the das side only vs the stored lcpp number.

## Wave A1 — Canary-Qwen 2.5B (the ASR leaderboard crown)

Tops Open ASR (5.63 % WER English). SALM: FastConformer encoder + **unmodified
Qwen3-1.7B** decoder + a linear projector + LoRA on the LLM.

### Oracle rail — ✅ DONE 2026-07-07 (the wave's designated first sub-task / main risk).

NeMo python greedy transcribe is the reference (NO cli/llama.cpp/onnx-asr canary oracle
exists — llama.cpp has zero canary/SALM/FastConformer support, onnx-asr ships no canary
export; so both parity AND the section-closing perf A/B ride the NeMo SALM engine on the
`benchmarks/asr/` rail, exactly like parakeet/whisper — audio waves are NEVER benched vs
llama.cpp). Rig built + verified end-to-end:
- **Driver:** `harness/canary_qwen_oracle.py` — engine-level (no HTTP), `SALM.from_pretrained`
  + `model.generate` with `GenerationConfig(do_sample=False, num_beams=1)` (pure greedy,
  deterministic — jfk IDs identical across reruns). Emits `IDS`/`TEXT` fixture lines.
- **Frozen fixture:** `harness/canary_qwen_expected.tsv` — jfk + 2 librispeech clips,
  token-for-token greedy ids incl. trailing EOS 151645. jfk → "And so my fellow Americans
  ask not what your country can do for you ask what you can do for your country".
- **Pinned venv** (`~/Work/canary-nemo-oracle/.venv`, gitignored): **python 3.11**, NeMo
  **trunk** `git+https://github.com/NVIDIA/NeMo.git@06312c963` (`nemo_toolkit 3.1.0`; the
  README requires trunk, no released wheel has `speechlm2`), torch 2.12.1, transformers
  5.13.0, **peft 0.19.1** (must add — trunk `[asr]` doesn't pull it; SALM needs it for LoRA).
  Install: `pip install "nemo_toolkit[asr] @ git+https://github.com/NVIDIA/NeMo.git" peft`.
- **🔑 macOS gotcha — brew-python `_decimal` fails (mpdecimal 3→4 ABI drift).** NeMo→lhotse
  does `from _decimal import ROUND_HALF_UP`; brew python 3.10/3.11 were linked against
  `libmpdec.3.dylib` which mpdecimal 4.0.1 no longer ships → `ImportError` at SALM import.
  Fix (benign compat symlink): `ln -sf $(brew --prefix mpdecimal)/lib/libmpdec.4.dylib
  $(brew --prefix mpdecimal)/lib/libmpdec.3.dylib`. (python3.14 / Apple 3.9.6 have working
  `_decimal` but are too-new/too-old for the wheels.)

### Architecture — fully mapped 2026-07-07 (from `config.json` + safetensors header).

Source: `nvidia/canary-qwen-2.5b` ships **HF-native `config.json` + `model.safetensors`
(bf16, 1718 tensors)** — **NOT a `.nemo`** (the plan's "extend the parakeet `.nemo`
converter" premise is wrong for this checkpoint; convert from safetensors directly, or
`SALM.save_to()` a `.nemo` first). The encoder is self-contained in the SALM checkpoint
(`canary-1b-flash` is NOT separately downloaded). All three original **VERIFY**s resolved:
- **Encoder = FastConformer, parakeet-key-identical** (`perception.encoder.*`): d_model
  1024, **32 layers** (parakeet 24), ff 4096, 8 heads, feat_in 128, subsampling `dw_striding`
  **factor 8** (pre_encode.conv.{0,2,3,5,6}/out — same shapes as parakeet: feat 4096=16·256),
  `self_attention_model='rel_pos'`, conv_kernel 9, batch_norm, `att_context_size=[-1,-1]`
  (**full global** attention). **DELTA vs parakeet:** canary has `self_attn.linear_{q,k,v,out}.bias`
  and `feed_forward*.linear{1,2}.bias` (parakeet has NO qkv biases, only `pos_bias_u/v`) —
  the converter + forward must carry them. Drop the whole TDT predictor/joint (canary has none).
- **Projector** = `perception.proj` linear **[2048,1024] + bias** (audio d_model 1024 → LLM
  hidden 2048). `modality_adapter` is `IdentityConnector` (no-op). Mel: `perception.preprocessor.featurizer.fb`
  [1,128,257] + `.window` [400] (n_fft 512, 16 kHz, per_feature norm — parakeet mel flavor).
- **Decoder = plain Qwen3-1.7B we already own** (`llm.base_model.model.model.*`): 28 layers,
  hidden 2048, ff 6144, 16 Q-heads / 8 KV-heads × 128, q_norm/k_norm [128] (qwen3 qk_norm),
  RoPE-neox. `embed_tokens.weight` [151936,2048] shared (no separate `lm_head` — tied).
  **LoRA** on `q_proj` + `v_proj` only (`.base_layer.weight` + `.lora_A`[128,2048] +
  `.lora_B`[out,128]); r=128 α=256 → **merge `W += (α/r)·B·A = W + 2.0·B·A` at conversion**,
  yielding a plain Qwen3-1.7B. prompt_format `qwen`; SALM prompt "Transcribe the following:
  `<|audioplaceholder|>`" wrapped in the Qwen chat template, audio frames splice at the tag.

### daslang model-implementation phase — ✅ DONE 2026-07-08. Token-for-token vs the NeMo SALM oracle.

Shipped the **two-file qwen3a-shaped rail**: a LoRA-merged **Qwen3-1.7B GGUF** decoder + a
"CNRY" **FastConformer encoder bin** loaded via `load_asr_model(decoder, encoder_bin)` (new
`AsrKind.canary`, routed by the mmproj magic sniff), so the audio-splice + greedy-decode rail
carries the decode. Delivered:
- **(1) Converter** `harness/convert_canary_to_ggml.py` (safetensors direct, no NeMo import):
  the decoder half reconstructs a Qwen3-1.7B HF checkpoint with the LoRA **merged** (`W += 2.0·B·A`
  on q/v_proj) and runs `convert_hf_to_gguf.py`; the encoder half writes a self-describing
  named-tensor f32 bin (bf16→f32 exact) of the FastConformer + `perception.proj`.
- **(2) New module `dasllama_canary.das`** — a clean fp32 FastConformer-32 forward (NOT a
  parakeet edit; parakeet's kernels are `private` + q8-tuned, so a self-contained module was the
  safer/cleaner fit). Reuses the shared `mm_blob_b`/`layernorm_batch`/`gemm_f32`/`silu4_batch`
  math. Carries the **qkv + ffn + conv biases** parakeet lacks; the depthwise conv bias folds
  into BN. 🔑 **The load-bearing delta from parakeet was NeMo's `MaskedConvSequential`** — it
  zeroes the time-rows past the propagated valid length (1100→550→275→138) BEFORE each stride-2
  conv; without it the last conv row is wrong and the error compounds ~10% by the last layer.
  With it, **every encoder layer matches the NeMo perception dump at rel 0.0 (OUT ≤1e-4)**.
- **(3) `transcribe_canary`** splices the projector soft tokens at the SALM/Qwen chatml prompt
  (`user\nTranscribe the following: <AUDIO><|im_end|>\n<|im_start|>assistant\n` — the exact ids
  dumped from `formatter.encode_dialog`), greedy-decodes the Qwen3 decoder until the text EOS
  (151645, emitted into the ids per the NeMo `generate` convention).
- **(4) Parity** `test_canary_qwen_oracle` (`test_whisper.das`, large-tier ~6.7 GB two-file,
  `DASLLAMA_PARITY_FULL=1`): **jfk + 2 LibriSpeech clips token-for-token** vs
  `harness/canary_qwen_expected.tsv` (fresh session per clip — reuse across different-length
  clips hits the shared decoder-KV subtlety, out of scope). **(5)** README matrix row.
- **(6) Section-closing perf A/B rides `benchmarks/asr/` vs the NeMo reference engine**, exactly
  like parakeet/whisper — **NOT llama.cpp** (zero canary/SALM support) and NOT onnx (onnx-asr
  ships no canary export). Reference = `harness/`→`benchmarks/asr/canary_qwen_bench.py` (NeMo
  SALM greedy, CPU, timed transcribe excl. load). das transcribe-ms + xRT vs NeMo in
  `benchmarks/asr/results.md` + committed TSVs; das **leads** the NeMo reference ~1.3–1.5× even
  at fp32 (M1 Max 8T). Ledger: the decoder loads fp32 for token-for-token safety — a q8 decoder
  + q8 encoder is the obvious perf follow-up (not chased mid-wave; parity gate is fp32).

## Wave A2 — Gemma-4 E-series audio input — ✅ DONE 2026-07-08 (E2B; encoder token-exact)

**Shipped: `dasllama_gemma4a.das`** — the `gemma4a` Conformer audio encoder + audio mm embedder,
read straight from the bf16 audio mmproj GGUF (no conversion), spliced into the G3 Gemma-4 text
path via `forward_prefill_embd`. New `AsrKind.gemma4a` behind the uniform ASR surface, routed by
the mmproj's `clip.audio.projector_type='gemma4a'`. **The encoder soft tokens match mtmd's audio
embeddings at rel ~0.0027** (float-reduction-order noise vs ggml's DFT + SIMD matmuls — NOT a bug;
a bf16-activation test confirmed f32 is the closest match, so ggml keeps f32 activations too).
- **Architecture (verified against `tools/mtmd/models/gemma4a.cpp`, NOT the pre-probe notes):**
  - Mel: 128-bin **HTK** filterbank (not Slaney), n_fft 512, window 320 (Hann zero-padded to
    512), hop 160, **natural-log magnitude** (not power), floor 0.001, semicausal left-pad
    window/2, NO per-feature norm. Matches mtmd's mel at max-abs 0.0016.
  - Subsample: Conv2D ×2 (k3 s2 p1) → LayerNorm-over-channels (weight-only, eps 1e-6) → ReLU,
    then input projection (flatten order k=freq·32+channel). Verified vs a clean conv1 dump at
    rel 0.0001.
  - **Attention is CHUNKED-LOCAL, not "full self-attn"** — it reduces to a plain causal
    sliding-window of 12 (self + 11 past); the Transformer-XL blocked relative shift maps to
    `RPE_row = 12 − (i−j)`, sinusoidal RPE positions `12−p`; q_scale=(1/√dh)/ln2,
    k_scale=ln(1+e)/ln2, per-dim Q scale, softcap 50. norm_eps hardcoded **1e-6** (ignores the
    1e-5 metadata). No per_dim_k_scale (absent in the file).
  - Conv module: RMSNorm → pw1 → GLU (sigmoid gate on 2nd half) → causal depthwise conv1d k=5
    (ssm_conv, left-pad 4) → RMSNorm → SiLU → pw2. 🔑 **`norm_conv`/`conv_norm` are name-swapped
    in the GGUF** (upstream tensor_mapping.py): the PRE-conv RMSNorm reads the tensor named
    `conv_norm`, the POST-depthwise one `norm_conv`. This was the single load-bearing bug (rel
    0.74 → 0.0027).
  - 🔑 **Per-op activation-quant calibration ranges** (`input_max/min`, `output_max/min`) honored
    on the ffn/attn/conv matmuls (clamp-in before the matmul, clamp-out after) — the flagged A2
    subtlety; loaded as `Clamp` structs, applied in `g4a_mm_clamped`.
  - Output: out proj (+bias) → weightless RMSNorm → mm input proj. All bf16 weights, fp32 forward.
- **Token-for-token (E2B, `test_gemma4a_audio_oracle`):** the no-think greedy transcription
  (`--temp 0 --jinja`, a bare user turn — the reasoning channel is near-tie-brittle and disabled
  both sides) is token-for-token with mtmd-cli through the **48-token confident prefix**, diverging
  ONLY at the Gemma-4 q8 decoder's OWN ~0.6-logit near-tie (llama.cpp's raw logits at that step:
  three tokens within 0.6 — the qwen3moe/gemma-26B situation). Proven audio-INDEPENDENT: feeding
  the EXACT mtmd reference soft tokens through the das q8 decoder diverges at the SAME step. Fixture
  freezes the first 40 confident ids ("The New York Times from July 21st, 1969. This isn't just
  newsprint and ink…"). Fast-tier (E2B decoder 4.97 GB); corpus clip `gemma4a_test2.wav`.
- **Section-closing perf A/B** (`benchmarks/asr/results.md`, M1 Max 8T, no-think both sides): das
  transcribe **6028 ms / xRT 2.89** vs mtmd-cli **1547 ms / xRT 11.3** — **das trails 3.9x**. das
  encode 1888 ms vs mtmd 117 ms (16x — the fp32 **scalar** Conformer vs ggml's bf16-weight SIMD
  GEMMs); long-context decode 21.7 vs 78 tok/s. The audio soft tokens ride the BATCHED
  `forward_prefill_embd` (PLE-aware), not naive per-token.
- **Ledger:** the E2B encode 16x gap is a gemm-gen tuned **Q8 audio-tower kernel** (dims: d_model
  1024 × ff 4096, conv pw 1024↔2048) — the obvious follow-up; and the ~500-position long-context
  decode gap. Neither chased — the A2 gate is fp32 encoder correctness. E4B is the same arch
  (proj_dim 2560) — the loader/encoder are dim-generic; not benched (large-tier).

Original probe notes (superseded — several were wrong; see the verified architecture above):
The E2B/E4B carry native audio. Depends on Wave G step 3 (text path verified). **PROBED
2026-07-07 — both VERIFYs resolved GREEN; oracle proven end-to-end** (E2B transcribed
`tools/mtmd/test-2.mp3` verbatim at `--temp 0 --jinja`; conformer encode 302 ms):
- (a) Encoder = **`gemma4a` conformer** (`clip.audio.projector_type='gemma4a'` on both
  E2B and E4B mmprojs). 12 blocks, audio d_model 1024, FFN 4096, 8 heads, 128 mel bins
  (FFT filterbank), LN eps 1e-5, projects into text embd (1536/2560). Structure
  (`tools/mtmd/models/gemma4a.cpp`): subsample Conv2D ×2 (stride 2) → per block { dual
  half-step macaron FFN (SiLU) · full self-attn w/ sinusoidal RPE (Transformer-XL blocked
  shift) · conv module: pointwise 1024→2048 → GLU → causal depthwise conv1d k=5
  (`ggml_ssm_conv`) → norm → SiLU → pointwise 1024→1024 }. = parakeet's block
  neighborhood — parakeet blocks are the starting point.
- (b) llama.cpp mtmd support = **YES** (runs today; the qwen2audio `llama-mtmd-cli
  --temp 0 --jinja` recipe applies verbatim). Wave does NOT stall; priority holds.
- **Gotcha:** the gemma4a conv ops ship per-op activation-quant calibration ranges
  (`input_max/min`, `output_max/min` tensors) — the encoder must honor them, not just
  weight-dequant.
- Note: llama.cpp also has a second audio flavor `gemma4ua` (encoder-free, raw-waveform
  640-frame, no FFT/filterbank) used by OTHER gemma-4 checkpoints — irrelevant to E2B/E4B
  but on the map for later.
Projector + soft-token splice ride the existing audio-chat rails.

## Wave A3 — Qwen3-Omni-30B-A3B, audio-in/text-out — ✅ DONE 2026-07-08. FULL token-for-token.

Audio understanding + ASR only; Talker (speech-gen) out of scope. The recon-and-gate cleared
GREEN on all three prereqs, and the slice turned out **config-sized — both halves were already
shipped**:
- **Oracle ✅** — llama.cpp mtmd supports Qwen3-Omni audio (PR #19441, in the local checkout;
  `llama-mtmd-cli --temp 0 --jinja` runs it). The audio wrapper is `<|audio_start|>…<|audio_end|>`.
- **Model ✅** — `ggml-org/Qwen3-Omni-30B-A3B-Instruct-GGUF` ships a READABLE **Q8_0 thinker
  (32.5 GB)** + **bf16 audio mmproj (2.2 GB)** — no K-quant blocker, no conversion. On disk 2026-07-08.
- **Thinker ✅** — converts as arch **`qwen3vlmoe`**, NOT plain qwen3moe. But `qwen3vlmoe` =
  qwen3moe (Wave Q) + M-RoPE (whose coordinates all equal the linear position for text+audio, so
  any rope-section layout degenerates to standard rope) + vision-only deepstack (inert without
  vision). So the SHIPPED qwen3moe forward is **bit-identical** — registered as a `qwen3vlmoe`
  alias mirroring the existing qwen3→qwen3vl pattern (config-only, `moe_blocks()` reused).
- **Audio tower = AuT (`qwen3a`)** — the SAME encoder family as the already-shipped **Qwen3-ASR**
  (`dasllama_qwen3a.das`), not whisper-based. The mtmd `qwen3a` preprocessor + graph are
  architecturally fixed (chunk 100, window 800), so the data-driven dasLLAMA tower loaded
  Qwen3-Omni's bigger AuT (d_model 1280, 32 blocks, 20 heads, ff 5120, proj 2048) **verbatim** —
  zero encoder code. proj 2048 = the thinker's n_embd.

Net slice: register the `qwen3vlmoe` alias (+audio markers) in `dasllama_arch_qwen3moe.das`, and add
`AsrKind.qwen3omni` to `dasllama_asr.das` — routed by the `qwen3vlmoe` decoder arch under the same
`qwen3a` mmproj sniff, REUSING the existing `dec`/`q3t`/`qenc`/`qcs` fields (**zero AsrModel/AsrSession
frame growth** — the A2 standing lesson dodged by construction) + a `transcribe_qwen3omni` that splices
the soft tokens into a bare Qwen chatml turn ("Transcribe the audio."), greedy-decoded to plain text
until `<|im_end|>`.
- **Parity: FULL token-for-token** vs `llama-mtmd-cli --temp 0 --jinja` on the same Q8 thinker + bf16
  mmproj — **jfk + 2 LibriSpeech clips, complete id sequences incl. the trailing `<|im_end|>` (151645),
  NO near-tie divergence** (unlike gemma4a/qwen3moe-decoder near-ties — the ASR turn is confident).
  Fixture `test_qwen3omni_audio_oracle` (large-tier, `DASLLAMA_PARITY_FULL=1`) + README matrix row.
- **Section-closing perf A/B** (`benchmarks/asr/results.md`, M1 Max 8T): das transcribe **3625 ms /
  xRT 3.03** (jfk) vs mtmd-cli **1173 ms / xRT 9.4** — **das trails 3.1x** (4.0x on the 33 s jfk3). The
  gap is the fp32 **scalar** qwen3a AuT tower (encode 1665 ms vs mtmd's bf16-SIMD 349 ms ≈ 4.8x); the
  q8 MoE thinker (grouped prefill ~207 t/s + q8 decode) also trails ggml here. Both are ledger items
  (gemm-gen tuned Q8 audio-tower kernel for 1280/5120 + the MoE prefill/decode path); the A3 gate is
  fp32 encoder correctness + parity, not perf.

Original scope notes (superseded — the AuT tower + qwen3moe thinker were both already shipped):
Depends on Wave Q (thinker = qwen3moe). Audio tower is **AuT** — a new encoder family, NOT
whisper-based. Claimed SOTA ASR (beats Voxtral-Small) — if it verifies as SOTA ASR it can enter the
ASR bench scoreboard next to whisper/parakeet/canary (the A/B here is a das-vs-mtmd-cli latency
comparison, not a WER leaderboard run).

## Cross-cutting: K-quant READ support (pull-forward candidate)

We read F32/F16/Q8_0/Q4_0/MXFP4. The 2026 GGUF ecosystem defaults to K-quants
(Q4_K_M/Q5_K/Q6_K) — several waves above otherwise start with "self-convert the HF
original" (already true for Voxtral). Read-side support = dequant K-blocks → our Q8_0 at
load (the parakeet fblob→qblob transcode pattern); NO native K-kernels unless profiling
ever says otherwise. This multiplies every other row (and community usability) more than
any single model. **ASK BORIS: pull forward before Wave G's big downloads, or slot after
Wave Q?**

## The next ENGINE arc — decision, not a wave

Two candidate directions after the waves above; each is flash-decode-sized:

- **Hybrid SSM lane** (Granite-4 micro/tiny, Qwen3-Next, LFM2): new kernel family — SSM
  scan + conv state next to attention in hybrid stacks. CPU-friendly (linear in context),
  clearly where small-model architecture is heading, small CPU-sized carriers exist.
- **MLA lane** (DeepSeek V3/R1, Kimi): only CPU-sized carrier is DeepSeek-V2-Lite
  (16B-A2.4B). The models people want from this family are 671B+ — never fitting our
  boxes; the R1-distills everyone runs locally are llama/qwen archs we already support.

Recommendation: **SSM > MLA** for our CPU story; MLA only if arch-coverage completeness
becomes a goal in itself. **ASK BORIS** when the waves above near completion.

## Quick-win shelf (fill idle moments, no wave ceremony)

- New **Ultravox checkpoints** (v0.6/v0.7): if the projector shape is unchanged, cost ≈
  download + parity run + matrix row.
- **Phi-4 / Phi-4-mini**: phi3-adjacent (**VERIFY**: full-attention + partial rotary
  deltas); likely a small arch tweak + parity.
- **Mistral Small 3.x / Ministral**: llama-arch by reputation (**VERIFY**) — parity-only.
- **Moonshine** (27M/61M streaming ASR): would make live dictation latency absurd;
  ONNX-only reference — needs the A1 python-oracle rail first.
- **GLM-4 dense (9B/32B)**: llama-ish with deltas (**VERIFY** bias/rope details).

## Explicit no-gos (this plan)

- Kimi K2.x, DeepSeek V3/R1 proper, GLM-4.7 flagship, Qwen3-235B parity runs — do not
  fit the boxes; header-tolerance + honest errors only.
- TTS (Voxtral-TTS, CosyVoice3, …) — a different capability arc (speech OUT); not model
  support. Gets its own plan if/when wanted.
- Vision input anywhere — unchanged standing exclusion.
- Talker/speech-generation side of Omni models.

## Sequencing summary (proposed)

G1 (31B verify) → G2 (26B MoE) → Q (qwen3moe 30B-A3B) → G3 (E2B/E4B text) → A1
(Canary-Qwen + python-oracle rail) → A2 (E-series audio) → A3 (Omni audio) → engine-arc
decision. K-quants placement per Boris. Quick-win shelf interleaves freely.
