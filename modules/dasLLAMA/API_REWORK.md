# dasLLAMA API Rework — Plan

**Status:** Phases 1-7 done — core API, arch registry + physical arch/kernel seams (6a/6b), chat layer, and the P7 kernel auto-tuner (grid emission + `[tuned]` reconstitution + TB cliff-guard). Design locked 2026-07-01. All seven T1/T2 model-support waves landed (see [Model-support plan](#model-support-plan--the-t1t2-waves-agreed-2026-07-01)), and the [facade + docs wave](#the-facade--dasllamadasllamadas-landed-2026-07-02) landed 2026-07-02, followed by the [tutorials wave](#the-tutorials-wave--tutorialsdasllama-landed-2026-07-02) the same day. **Next: the performance-ledger pass.**

This is the design record for unifying the dasLLAMA user-facing API and making the
backend extensible. It carries the **why**; the code carries the how. Keep it current
as phases land.

## Goal & priorities

In priority order (Boris, 2026-07-01):

1. **A clean user-facing API** — one way to load a model, tokenize (both directions),
   run, and sample. Today every demo re-implements these.
2. **An extensible backend — two seams:**
   - **arch/block seam** — new model architectures (incl. MoE) slot in without editing
     the core forward pass;
   - **kernel/ISA seam** — new platform kernels (x64 AVX/VNNI) slot in behind the same
     registry the ARM NEON backend already uses.
3. **The tune macro** — per-box kernel tuning driven from a config loaded at compile time
   (see [Tune macro](#tune-macro--for-loop-attribute-reification)).

A **minimal unified chat app stays above the line**: bringing up a new architecture means
talking to it to sanity-check. Nothing fancy — roughly what the current per-model REPLs
do, collapsed into one. Tool use / an OpenAI server are *demonstrations*, below the line.

### The invariant

The engine is validated **token-for-token** against 5 oracles (stories15M, TinyLlama
v0.3/v1.0, Llama-3.2-1B, Llama-3.1-8B) plus the per-arch parity fixtures. **Every phase is
a mergeable PR that keeps that green**, and the dense path stays **bit-identical** until
MoE is a genuinely *new* code path. A refactor that changes a single token is a bug.

## Scope — Tier 1 + Tier 2

What varies across transformer architectures, and how deep we cut:

| Tier | What varies | Status |
|---|---|---|
| **1 — scalar knobs** | RoPE variant/scaling, RMS vs LayerNorm, pre/post-norm, SiLU vs GeLU, QKV bias, softcaps, sliding window, embed scale, head_dim≠dim/heads | **Handled today** — `Config` flags. A new dense llama-like = a few flags. |
| **2 — block swaps** | **MoE** (FFN → router + experts), QK-norm, partial/pruned RoPE, per-layer attention patterns (SWA ratios, per-layer θ, NoPE layers), shared KV cache (Gemma-4: last-N layers reuse earlier KV), per-layer embeddings (PLE), attention sinks, parallel attn+FFN | **This rework.** Not flags — they change block dataflow. See the model-support wave plan below. |
| **3 — deep forks** | MLA / compressed KV (DeepSeek-V3), **hybrid linear attention** (Qwen3.5/3.6 Gated DeltaNet, Mamba), multimodal, bespoke sparse attention (e.g. GLM-5.2 "IndexShare") | **Deferred.** Needs a pluggable KV-cache + attention core, not just blocks. |

## Target surface (3 layers)

### Layer 0 — core
```
def load_model(path : string; mode = QuantMode.fp32) : Model     // arch auto-dispatched from GGUF meta
def create_session(model : Model; max_context = 0) : Session     // 0 = model's trained ctx; Model → many Sessions

// unified tokenizer facade on the model (SPM/BPE chosen internally)
def encode(model; text : string; add_special = true; parse_special = false) : array<int64>
def decode(model; ids : array<int64>) : string
def piece (model; id : int64) : string                           // one token — for streaming

def eval(model; var session; tokens : array<int64>)              // THE primitive; session owns n_past
def sample(var session; params : SamplingParams) : int64         // greedy = SamplingParams(temp=0)
def stats(session) : Stats                                       // ttft, prefill tok/s, gen tok/s
```
`Session` owns its own `n_past`, so callers never pass `start_pos`. Prefill and generation
become the **same call** at different batch sizes: prefill = `eval(prompt)`, step = `eval([tok])`.

### Layer 1 — generation
```
def generate(model; var session; prompt : array<int64>; params : SamplingParams; max_tokens : int) ...
```
Streaming is a **trailing callback block** — `respond(...) $(piece : string) : bool { ... }`,
return `false` to stop. Absorbs the ~60-line `sample()` copy-pasted into every demo today.

### Layer 2 — chat (minimal, above the line)  *(built — Phase 5)*
```
struct Message { role : Role; content : string }                 // Role = system | user | assistant | tool
def create_chat(model; system = ""; max_new = 256) : ChatSession
def add_user(var chat; text : string)
def render_turn(model; chat) : array<int64>                      // the turn's prefill ids, no model run — inspect/test
def respond(model; var chat; params : SamplingParams) $(piece : string) : bool { ... } : string
```
The model is threaded through `respond` (like `eval`/`generate`) rather than held by the ChatSession, so
there is no model-pointer lifetime to manage. `respond` renders the turn, prefills it, streams the reply
through the callback block (return `false` to stop), terminates the turn in the KV cache, appends both
turns to `chat.history`, and returns the full reply text; `stats(chat.session)` reports its timing.
**Context cap:** the session's KV cache is sized to `model.config.seq_len`, so a caller loading a
large-context model (Llama-3/Phi = 131072) must cap `model.config.seq_len` first (as every REPL does) —
the full cache would exceed the 4 GB per-array limit.

## The two backend seams

### Arch/block seam  *(built — Phase 4)*
`configure_arch`'s monolithic `switch` is now an **arch registry** — each arch self-registers
via `[init]` (the same pattern `dasllama_math_aarch64_neon.das` uses for its kernels). As built:
```
struct ArchBlocks { attn_decode; ffn_decode; attn_prefill; ffn_prefill }   // copyable fn-ptrs, ride on the Model
struct ArchDesc   { name; configure; blocks : ArchBlocks; chat : ChatTemplate }   // registry-only
register_arch("llama", { configure_llama, std_blocks(), LLAMA3_CHAT })
```
Blocks are **function pointers, one per (attention|ffn) × (decode|prefill)** — decode is the fused
single-token path, prefill the batched path, so the two forward loops call `t.blocks.attn_decode(t, s,
l, pos)` / `t.blocks.ffn_prefill(t, s, l, npos)` etc. `ArchBlocks` (all fn-ptrs, copyable) is copied
onto the `Model` at load; the full `ArchDesc` (non-copyable — holds a `ChatTemplate`) stays in the
registry. All four current arches are dense llama-likes → they share `std_blocks()` and differ only in
config flags, tokenizer backend, and chat template. **Bit-identical**: the block bodies are the old
per-layer code moved verbatim (proven token-for-token via the oracle suite + an A/B source diff across
all 5 GGUF arches). GeGLU-vs-SwiGLU stays a `ffn_act` flag *inside* the shared dense FFN block.

MoE is the one genuinely new block — it registers a different `ffn_*` here without touching the
forward loops. **Build it general — shared experts + routed top-k, gating pluggable (sigmoid |
softmax)** — the GLM / DeepSeek / Qwen2-MoE shape, *not* Mixtral's plain-routed special case, or
GLM/DeepSeek force a re-plumb. First MoE oracle: a small shared-expert MoE (lean Qwen1.5-MoE-A2.7B),
decided when we get there.

**Chat template — data only in Phase 4.** `ChatTemplate` is an ordered part list per turn
(`text` | `special`-token-spelling | `content` slot) + `add_bos` + stop-token spellings; the four
arches' templates live in the registry, validated for well-formedness by `test_arch_registry.das`.
The **renderer** (parts → token ids, resolving special spellings) lands with the Phase-5 chat engine.
One known wrinkle it must handle: GGUF `general.architecture` is `"llama"` for *both* Llama-3 and
Llama-2/TinyLlama, which use different chat formats — so per-model disambiguation (reading the GGUF
`tokenizer.chat_template`, or detecting special tokens) is a Phase-5 concern. The registry's `"llama"`
template carries the Llama-3 instruct format (the flagship).

### Kernel/ISA seam  *(built — Phase 6b)*
A **named `KernelBackend` registry**, symmetric with the arch registry. `dasllama_math.das` owns the
abstraction (the `MatmulQ8Q8*Fn` typedefs, the `g_mm_q8q8*` active pointers, the `matmul_q8q8*` wrappers,
`repack_q8q8_weight`, and `register_kernel_backend` / `select_kernel_backend` / `pin_kernel_backend` /
`select_matmul_backend_for_load`). Each ISA module self-registers its backend(s) at `[init]`:
```
struct KernelBackend { name; mm; batch; group3; repack; needs_repack; priority }   // copyable
register_kernel_backend(KernelBackend("arm64-laneq", …, needs_repack = true, priority = 20))
```
- `dasllama_math_default.das` — the **portable** backend (`dot_q8q8` + the three portable kernels + the
  parallel_for profiler), registered at priority 0. The fallback everywhere; **required by any q8·q8
  consumer** (the `g_mm_q8q8*` pointers panic-stub until a backend registers).
- `dasllama_math_aarch64_neon.das` — `arm64-sdot` (row-major, priority 10) + `arm64-laneq` (interleaved
  repack, priority 20), gated on `jit_enabled() && get_architecture_name() == "arm64"`.
- `dasllama_math_x64_avx.das` — the SESSION-2 mirror: register `x64-vnni` (SDOT → VNNI `vpdpbusd`) exactly
  the same way. **No edit to `dasllama_math` or the wrappers** — it just adds a file + a `[init]`.
  The x64 bring-up is fully documented in `x64_arch.md` (the seam map), `get_x64_going.md`
  (the runbook), and `tune_for_this_box.md` (per-box tuning + measurement discipline).

**Two selection tiers** (mirroring the hardware): `register_kernel_backend` auto-activates the
highest-priority **no-repack** backend so direct callers (tests/benches, row-major weights) get the best
kernel with no load step; `select_matmul_backend_for_load()` (called by the loader on the Q8 path) picks
the best **overall** backend — including a repack one — and returns whether the loader must repack.
`pin_kernel_backend` forces a named backend for A/B benchmarking. Validated token-identical (100/100 JIT +
AOT) + `test_kernel_backend.das`.

## Tune macro — for-loop-attribute reification

**The tune surface is the kernel loops' attributes**, not just the runtime `TB` knob:
```
for [vectorize, vectorize_width = 8, unroll_count = 2] (j in range64(n)) { ... }
```
These live throughout `dasllama_math.das` (dot / axpy / add / mul / scale / copy / silu /
softmax / rope) and `_aarch64_neon.das`, and the `8` / `2` / `4` / `16` are **literal ints
today**. `vectorize` / `vectorize_width` / `unroll_count` are **not** grammar keywords —
they're generic `for [...]` annotation-args recognized semantically.

The tune macro must **load a box config at compile time and emit the right numbers into
these attributes**. Two routes; the deciding question is phase-ordering (are loop
attributes baked before macros run?):

- **(a)** grammar accepts a const-*expr* in the attribute value (`unroll_count = TUNE.unroll`)
  — a parser/AST change; or
- **(b)** a loop/function macro rewrites the literal annotation-args before codegen — no
  grammar change, using the same AST-rewrite machinery `dasllama_par.das` already uses for
  `maybe_parallel_for`.

This is a **daslang-core capability**, broader than dasLLAMA and broader than `TB`.

**Universal vs per-box:** the token-blocking loop-nest structure and repack concept are
universal (stay in the shared path). `TB`, `unroll_count`, `vectorize_width`, thread count,
and the ISA are the **per-box config** — a box-profile the loader reads. See the memory
roadmap + `dasllama_kernel_opt_findings` for the measurement methodology (interleaved A/B,
the ggml per-op-timing patch) — the make-or-break for any tuner is measurement, not codegen.

## Decisions

- **FP32 load default stays.** Do *not* silently reroute the token-exact tests; demos pass
  `q8` explicitly.
- **Tool use is parked** — a curiosity. v1 (later) = OpenAI-style: parse tool-call text out
  of the generation + inject tool definitions into the system turn. No core-engine change.
- **GLM-5.2 is out of scope** — 744B MoE / ~40B active, 1M ctx via bespoke "IndexShare"
  sparse attention (Tier 3); ~370 GB even at 4-bit. Not a CPU target. Its family (GLM-4.5/4.6)
  informs the general MoE-block shape above.

## Phase order

Each phase is a mergeable PR; all oracles stay green; dense path bit-identical until MoE.

1. **Naming + `eval`** ✅ — `Transformer → Model`, `RunState → Session`, `n_past` into
   `Session`, `eval()` / `create_session()`. Pure refactor, token-identical.
2. **Tokenizer facade** ✅ — `SpmTokenizer` / `BpeTokenizer` (renamed) behind a unified
   `Tokenizer` (`kind` + both backends); `load_tokenizer_auto` picks the backend from the GGUF's
   `tokenizer.ggml.model`. `encode` / `decode` / `piece` on both `Tokenizer` and `Model`;
   `load_model` loads weights **and** tokenizer. Existing raw paths unchanged. *(test_facade.das)*
3. **Sampling** ✅ — `SamplingParams` (defaults = greedy: temp 0, penalty 1.0 → the greedy branch
   calls the existing `argmax`, so bit-identical). `Session` owns the RNG (`rng`) + repetition
   window (`recent`) + top-k scratch. `sample(session, params)` lifts the copy-pasted demo
   `sample()`; streaming `generate(model, session, prompt, params, max, blk)` with a callback block
   (`return false` to stop). `set_seed`. *(test_sampling.das)*
4. **Arch registry + block seam** ✅ — `configure_arch` switch → `[init]` self-registering
   registry (`ArchDesc` = configure + `ArchBlocks` fn-ptrs + `ChatTemplate`); attention/FFN are
   function-pointer blocks (decode + prefill) the forward loops dispatch through; chat-template
   descriptor is a registry field (data only — renderer is Phase 5). Blocks resolved onto the Model at
   load (also for the llama2.c `load_checkpoint` path). No new arch; dense path bit-identical, proven
   token-for-token (oracle suite + A/B source diff on all 5 GGUF arches). *(test_arch_registry.das)*
5. **Minimal unified chat app** ✅ — new `dasllama_chat.das` (layer 2): `Role`/`Message`/`ChatSession`
   + `create_chat`/`add_user`/`respond` (streaming callback block) + `render_turn`. The **renderer**
   turns a `ChatTemplate` part list into token ids by *segment accumulation* — each maximal run of
   text/content between special tokens is encoded as ONE segment, which reproduces every old per-model
   REPL's prefill token-for-token (proven in `test_chat.das` for all 5 families). Special tokens
   resolve by spelling: BPE via `special_id`, SPM via the vocab lookup (added `special_id(SpmTokenizer)`
   + the `Tokenizer`/`Model` facades, plus `bos_id` for `add_bos`; SPM `bos_id`/`eos_id` now loaded from
   GGUF). The `"llama"` collision is resolved by **tokenizer backend** — BPE ⇒ Llama-3 instruct
   (registry default), SPM ⇒ Zephyr/TinyLlama (a local `zephyr_chat_template()`); `Model.arch` carries
   the GGUF architecture for this. `stats()` (ttft + prefill/gen tok/s) added, filled by the instrumented
   streaming `generate()`. Examples collapsed: `chat.das` (one REPL, any model) + `run.das` (one
   completion + stats); the 4 other chat REPLs + 2 runners deleted, their oracles migrated into
   `test_parity.das` (TinyLlama-v0.3, Llama-3.2-1B). Full suite 100/100 JIT + AOT.
6. **Arch files + kernel-backend registry** *(done)* —
   - **Arch files (done):** split the `dasllama_transformer.das` monolith so each architecture lives
     in its own `dasllama_arch_*.das` (config setter + `[init]` registration + chat template). The
     shared engine is `dasllama_common.das`; `dasllama_transformer.das` is now a thin umbrella
     (`require dasllama_common public` + one require per arch, firing each arch's `[init]`), so every
     existing `require dasllama/dasllama_transformer` is unchanged — zero consumer churn. `std_blocks()`
     is the only symbol promoted to public (the arch files' one pull from common); block bodies, forward
     loops, and config are byte-for-byte unchanged, so the suite stays **token-identical** (100/100 JIT
     + AOT). Adding an arch = a new file, never touching the core; MoE brings its own blocks in its own
     file. See [What split into files](#what-split-into-files-done--phase-6).
   - **Kernel-backend registry (done):** the `KernelBackend` registry + `dasllama_math_default.das` split
     — each ISA module self-registers at `[init]`, `select_matmul_backend_for_load()` drives the load-time
     repack handshake, `dasllama_math_x64_avx.das` mirrors NEON with no core edit. See
     [Kernel/ISA seam](#kernelisa-seam--built--phase-6b) + [What split into files](#what-split-into-files-done--phase-6).
7. **Tune macro + loop-attribute reification** — depends on 6.

**AOT note (phase 5):** the `Model`↔`ArchBlocks` cycle (Model holds ArchBlocks by value; ArchBlocks's
fn-ptr fields reference Model) tripped the AOT C++ emitter — it emitted `Model` before `ArchBlocks`
(incomplete-type + `sizeof(Model)` mismatch). That emitter bug is **already fixed upstream** (an AOT PR
pending merge). We also declare `ArchBlocks` + its typedefs *before* `Model` in the source — a harmless
defensive ordering (legitimate on its own; emission is source-order for non-recursed structs) that keeps
the AOT green on this branch before that PR lands, and can stay after.

**Below the line (demonstrations):** an OpenAI-compatible server `/example` (the acceptance
test — if it builds with no reach into internals, the API is right), tool use.

## Model-support plan — the T1/T2 waves *(agreed 2026-07-01)*

Each wave = one engine capability + the smallest popular model that proves it, shipped as a
mergeable PR with a frozen `simple_ids` oracle fixture (`test_parity.das`) + README row. The
local llama.cpp checkout (2026-06-27) implements every target arch — port each one from its
`llm_build_*` in `src/llama-model.cpp` (the definitive spec), never from blog posts. Models
are Q8_0 GGUFs into `~/Work/llama.cpp/models/` unless noted.

| Wave | Engine delta | Oracle model(s) | ~GB |
|---|---|---|---|
| **0** | none — verify + fixtures (incl. backfilling Qwen2.5-1.5B + Phi-3.5), README rows; chat-template *detection* (sniff GGUF `tokenizer.chat_template` → named registry template) | Mistral-7B-Instruct-v0.3 (GGUF arch is `"llama"`), SmolLM2-1.7B-Instruct | 7.7 + 1.8 |
| **1** | QK-norm — Config flag + per-layer `attn_q_norm`/`attn_k_norm` weights, per-head RMSNorm pre-RoPE in the shared attn blocks (a flag like `ffn_act`, not a block swap); `<think>`-stripping in chat history | Qwen3-0.6B (fast iteration), Qwen3-4B-Instruct-2507 (fixture) | 0.7 + 4.3 |
| **2** | per-layer attention patterns — generalize the hardcoded `l % 2` SWA alternation to `sliding_window_pattern` (gemma2 = 2, gemma3 = 6), per-layer RoPE θ (dual rope tables); SmolLM3's NoPE layers = same machinery | gemma-3-1b-it, gemma-3-4b-it (opt: SmolLM3-3B) | 1.1 + 4.5 |
| **3** ✅ | **Gemma 4** — GGUF ground truth revised the plan: the 12B has NO shared KV and NO PLE (`shared_kv_layers = 0`, `embedding_length_per_layer_input = 0` — both are E-series features; the loader now panics honestly on either). What it DOES need, all shipped: heterogeneous per-layer geometry (sliding 16Q/8KV×256 vs global 16Q/1KV×512 — per-layer weight-offset arrays, class-max scratch, per-layer KV-cache packing), explicit bool-array SWA pattern (`swa_mask`), p-RoPE freq factors on global layers only (full rotation, `dimension_count` == head size per class — pruning is a loader panic), weightless V-norm, V-from-K on the no-`attn_v` global layers, unit attention scale, per-layer `layer_output_scale`, final softcap, `suppress_tokens` logit bias, and the new `gemma4` SPM-style-BPE tokenizer (metaspace + newline-only pre-split + byte fallback; 46/46 on the official corpus). Shared-KV + PLE move to a follow-up E-series wave (E2B ~2GB oracle) | gemma-4-12b-it — 40/40 counting + 40/40 window-engaged ~1490-token prompt | 12.7 |
| **4** ✅ | **MoE FFN block** via the ArchBlocks seam (proves Phase 4 — the forward loops were untouched; qwen2moe registers `moe_blocks()`). Shipped: router → top-k over probs-before-selection, pluggable gating (`MoeGate.softmax\|sigmoid` from `{arch}.expert_gating_func`), `norm_topk_prob` + `expert_weights_scale` slots, routed experts + sigmoid-gated **shared expert**; expert-major layout (expert e of layer l = a plain 2D matrix at `we*_off + (l·n_expert+e)·dim·n_ff_exp`) so every existing kernel incl. the arm64 repack applies per expert; 3D `ffn_*_exps` transcode in one contiguous read; honest panics for grouped routing / gating-func 3 / `exp_probs_b` / expert biases. Decode fused; prefill naive per-token (grouped GEMM → ledger). Stretch models not pursued this wave: gemma-4-26B-A4B @ Q4_0, Qwen3-30B-A3B (qwen3moe = QK-norm + `norm_topk_prob`, no shared expert — a thin arch file when wanted) | Qwen1.5-MoE-A2.7B-Chat — 40/40 counting AND 40/40 prose, both prompts token-for-token | 15 |
| **5** ✅ | **gpt-oss** — everything the plan row named plus what the GGUF ground truth added. Shipped: attention sinks (per-head sink logit joins the softmax max + denominator, no V contribution — decode `softmax_sink` + all three prefill cores; flash seeds its online softmax with max = sink, sum = 1); MXFP4 decode (E8M0 half-scale + doubled-e2m1 nibble LUT, one new `gguf_read_tensor_f32` arm → the existing dequant→self-Q8 path covers it); YaRN rope with ZERO engine change — the NTK-by-parts ramp is a per-pair effective position scale, so the loader synthesizes `rope_freqs[j] = 1/(fscale + ramp_j·(1−fscale))` and folds the 1 + 0.1·ln(factor) magnitude into `rope_mscale`; `MoeGate.softmax_weight` (top-k on raw biased logits, softmax over the selected k; knockout sentinel → −FLT_MAX); router + per-expert biases (fblob stacks, expert-major) + attention output-projection bias; `FfnAct.swiglu_oai` (clamped, +1 up branch, scalar + exp4 float4); pre-FFN norm under the `post_attention_norm` name; gpt-4o/o200k pre-tokenizer (llama.cpp's exact case-class approximation: contraction suffixes, upper*/lower+ letter runs, `/` in punct tails, no-BOS default) + Harmony-lite chat template with `<|channel|>` detection sniff | gpt-oss-20b — 40/40 counting + 40/40 window-engaged 449-token prompt (encoded in-test), both FIRST TRY; tokenizer id-for-id vs llama.cpp on counting/contraction/whitespace probes | 12.1 |
| **6** ✅ | *(infra, last)* lifted the linear-allocator 4GB limit — `HeapChunk` `size`/`offset` went `uint64` and `LinearChunkAllocator` dropped every `UINT32_MAX` cap (`allocate`/`free`/`setInitialSize`, the `reset` clamp; virtual `grow` is 64-bit — one override, `DebugInfoAllocator`). Regression tests in `tests-cpp/small/test_heap_64bit.cpp`: an ungated >4GB `setInitialSize` probe plus gated (`DASLANG_HUGE_HEAP_TESTS=1`) >4GB single-chunk and default-context-heap tests. End-to-end on the default heap: Mistral-7B's 7.1GB Q8 blob loads and matches its fixture token-for-token (`harness/parity.das` dropped its `options persistent_heap`), and Llama-3.2-1B at native `seq_len` 131072 allocates two exactly-4GiB single KV arrays and generates. Workaround disposition, re-justified honestly: the tests **keep** `options persistent_heap` — since the explicit-delete discipline landed, it's what makes `delete` really free between fixtures (linear free is a mid-context no-op, so multi-GB weights would accumulate) — and the `seq_len` caps **stay as RAM savers** (native 131072 KV is 8–64GB of fp32); both kinds of comment now state the real reason instead of the vanished cap | none — regression = existing suite + the new tests-cpp cases | — |

Out of scope, and why: Qwen3.5/3.6 (Gated-DeltaNet hybrid linear attention → Tier 3),
Llama-4 (109B+), DeepSeek V3+/GLM-5 (MLA / bespoke sparse → Tier 3), Mixtral (superseded,
too big, plain-routed anyway).

**Chat, long-term (agreed):** stage 1 = the wave-0 template *detection* (string-sniff the
embedded `tokenizer.chat_template` for `[INST]` / `<|im_start|>` / `<start_of_turn>` /
`<|start_header_id|>` … → pick a named registry template — fixes Mistral and the `"llama"`
collision with no template execution). Stage 2 — a Jinja-subset interpreter in daslang (the
llama.cpp "minja" route, executes the embedded template directly) — is deliberately deferred
until the named registry stops scaling; the realistic forcing function is gpt-oss's
channel-based Harmony format at wave 5. Chat remains layer 2 throughout.

## The facade — `dasllama/dasllama.das` *(landed 2026-07-02)*

One require is the public API: `require dasllama/dasllama` re-exports the engine
(`dasllama_transformer public` — which also fires every arch `[init]`) and the chat layer
(`dasllama_chat public`), and defines the **documented, curated surface** — the three layers above
as 14 `//!`-documented stubs (`load_model` / `create_session` / `encode` / `decode` / `piece` /
`eval` / `sample` / `set_seed` / `stats` / `generate` / `create_chat` / `add_user` / `render_turn`
/ `respond`). Everything else stays reachable through the re-export, deliberately undocumented.

- **Naming:** wherever the facade takes the good name, the engine spelling carries a trailing
  underscore (`load_model_`, `eval_`, …) — a same-name stub plus a public re-export would be an
  ambiguous overload at every call site. The raw greedy `generate(t, s, prompt, steps)` keeps its
  name (different arity, no ambiguity — the token-exact oracle path). Public-path consumers
  (examples, `test_parity`/`test_facade`/`test_chat`/`test_sampling`/…) require the facade and use
  the good names; internal/kernel tests and the chat engine's internals use the `_` spellings.
- **Examples are the completeness gate:** `run.das` / `chat.das` require ONLY `dasllama/dasllama`
  from the module — if a demo needs something the facade lacks, the facade grows, not the require
  list. Both verified end-to-end after the switch (TinyLlama completion + chat smoke).
- **Docs:** das2rst registers ONLY the facade module (new stdlib section `sec_ai.rst`,
  `generated/dasllama.rst`; `doc.yml` path filters now include `modules/dasLLAMA/dasllama/**`).
  Engine modules stay undocumented by design — Model's ~40 offset fields are not API. The types the
  facade signatures mention (`Model`, `Session`, `QuantMode`, `SamplingParams`, `Stats`,
  `ChatSession`) get hand-written opaque stanzas emitted by `document_module_dasllama`'s
  `DocsHook.afterEnums` under the exact `:ref:` labels the signature renderer produces, so
  cross-references resolve without documenting internals. The module header
  (`handmade/module-dasllama.rst`) carries the supported-model-family list (and, later, tutorial
  links).
- **🔑 `//!` placement:** the doc extractor (`daslib/rst_comment`) attaches a docstring only when
  the `//!` block is the FIRST thing *inside* the function body — an above-def `//!` is silently
  discarded (this is why no engine docstring ever extracted; the engine's above-def `//!` remain as
  source comments only).
- **Drift detector** (`tests/dasLLAMA/test_facade_docs.das`): every facade def has a body-leading
  `//!` (and no inert above-def `//!` exists); facade stubs ↔ engine `_` spellings stay 1:1 in both
  directions; the examples stay facade-only. Negative-probed: an undocumented extra stub fails it.

## The tutorials wave — `tutorials/dasLLAMA/` *(landed 2026-07-02)*

Six tutorials written strictly against the facade, each a runnable single-file
`main()` (project convention: `tutorials/<area>/`, never `modules/<X>/tutorial/`), with paired
RST pages under `doc/source/reference/tutorials/` and a toctree section, plus tutorial links on
the stdlib module page. The teaching model is SmolLM2-135M-Instruct Q8_0 (~145MB llama-arch
GGUF; models aren't shipped — path via CLI arg or `DASLLAMA_MODEL`):

1. **hello_generate** — load_model / encode / decode / piece / generate / stats.
2. **chat** — create_chat / add_user / respond, multi-turn KV memory, history, render_turn
   (specials are atomic ids, invisible to decode).
3. **sampling** — greedy determinism + the 135M repetition loop, penalty breaking it,
   temp / top-k / set_seed reproducibility.
4. **sessions_and_memory** — KV sizing + the cap-seq_len-BEFORE-create_session rule, session
   independence, manual eval/sample loop, persistent_heap + delete discipline.
5. **performance** — jit_enabled, job-queue requirement + `DAS_JOBQUE_THREADS`, prefill-vs-gen
   physics, fp32/q8/q4 measured table, `_jit_fast_math`.
6. **add_an_arch** — registry walkthrough (arch_names / ArchDesc / std_blocks / chat parts /
   register_arch), no model needed.

Tutorials joined the CMake install + `dry_run_tutorials` compile gate. One durable lesson baked
into 04's structure: a fat `main()` frame plus the forward-pass call chain overflows the default
16KB context stack (das frames are statically sized for all locals) — model-driving mains stay
lean, one function per section.

### Performance ledger (living — address after the model waves)

Standing rule (Boris, 2026-07-01): any performance possibility spotted while doing wave work
gets a note HERE instead of being acted on mid-wave — the model waves optimize for correctness
and coverage; this ledger is the backlog for the perf pass that follows them. Every entry says
what it costs today and what the fix would change.

- **Tied classifier reads the fp32 embedding — the single biggest decode lever on big-vocab
  models.** `shared_weights` models (Gemma family, Qwen small) route the classifier matmul
  through the fp32 `token_embd` in fblob: vocab × dim × 4B of traffic per token — on
  gemma-4-12B (262144 × 3840) that is ~4GB/token, roughly a quarter of its ~5 tok/s decode
  roofline; llama.cpp matmuls the Q8 embedding directly. Same fix also cuts RESIDENT memory
  (the fp32 table costs 4GB vs ~1GB at Q8; today only the per-token embedding-row read needs
  fp32, and that could dequant one row on demand). Changes numerics vs today's fp32 classifier
  → every tied-model parity fixture needs refreezing in the same PR. (Spotted wave 3.)
- **V-from-K layers: fuse the K→V copy with the weightless V-norm.** gemma4 global layers copy
  the K projection into V, then rms_batch it — two passes over npos × kv_dim where one fused
  pass would do. Small (kv_dim = 512 on those layers) but free. (Spotted wave 3.)
- **No llama.cpp A/B on gemma-4-12B yet.** Wave 3 verified tokens, not speed — prefill 62 t/s /
  gen 5 t/s on the M1 are uncalibrated against llama.cpp on the same box. Run the interleaved
  A/B (kernel-opt method) before drawing any conclusions or optimizing. (Spotted wave 3.)
  Same for gpt-oss-20b (wave 5): the chat smoke saw prefill 3 t/s / gen 2 t/s — expected to be
  dominated by the naive per-token MoE prefill + the doubled Q8 expert traffic (both already on
  this ledger), but A/B before touching anything.
- **MoE prefill is naive per-token — expert-bucketed grouped GEMM is the batched fix.** Each
  prefill position routes independently and runs its top-k experts through the single-token
  matmuls (`ffn_moe_prefill`); llama.cpp batches all positions hitting the same expert into one
  GEMM (`mul_mat_id`). Bucket positions by selected expert, run one batched GEMM per touched
  expert, scatter back by weight. Decode is unaffected (one token = no bucketing win).
  (Spotted wave 4.)
- **MoE decode re-quantizes the same activation per expert.** `moe_ffn_core` calls `mm` 2×
  per routed expert + 2× for the shared expert on the SAME `s.xb`, and each Q8 call re-runs
  `quantize_q8_0_into` on it — 5 redundant requants of a dim-wide vector per layer per token
  (only the down-projection inputs differ). Hoist the xb quant once per layer like `mm_qkv`
  does for the fused QKV path. Small per call, ×24 layers ×(2k+2) matmuls. (Spotted wave 4.)
- **MXFP4 experts transcode to Q8 — doubling their bytes.** gpt-oss-20b's expert stacks are
  4.25 bit/weight on disk (~10GB) but run as Q8 (~19GB + 2.4GB scales): 2× the resident memory
  AND 2× the decode weight traffic vs a native MXFP4 kernel (per-block E8M0 scale + 16-entry
  nibble LUT — the int8 side of the SDOT pipeline could eat un-LUTed nibbles with a lane
  table). Q4_0 storage is the existing halfway house (same 4-bit traffic, one lossy requant,
  no new kernel) — worth an A/B before writing MXFP4-native matmuls. (Spotted wave 5.)
- **q4 has no batched prefill kernel — prefill collapses to decode rate.** The q4 path serves
  everything through the scalar fp32-activation `dot_q4`/`matmul_q4` (no q8-style token-blocked
  batch GEMM, no NEON arm, no repack backend), so a q4 prefill runs at generation speed:
  measured on SmolLM2-135M, q8 prefill 1391 t/s vs q4 prefill 70 t/s ≈ its own 69 t/s decode.
  A q4 batch kernel (or the load-time q4→q8 transcode as the cheap fix) closes it.
  (Spotted tutorials wave.)

## What collapsed (done — Phase 5)

- `examples/dasLLAMA/` went from **8 programs to 2**. The 5 chat REPLs (`chat`, `gemma_chat`,
  `llama3_chat`, `phi_chat`, `qwen_chat`) collapsed into one `chat.das` driving the `dasllama_chat`
  engine; the 3 runners (`run`, `gguf_run`, `llama3_run`) collapsed into one `run.das` (completion +
  `stats()`). The deleted runners' token-exact oracles moved into `test_parity.das`.
- The old chat demos hand-rolled a token-by-token `prefill()` that never called the fast
  `forward_prefill`; `respond` prefills the whole rendered turn in one `eval()`, so the collapse also
  sped them up.

## What split into files (done — Phase 6)

The `dasllama` submodule layout, before → after:

| Before | After | Role |
|---|---|---|
| `dasllama_transformer.das` (2024 lines) | `dasllama_common.das` (~1940) | the whole engine: types, loader, kernels, std blocks, forward loops, sampling, registry |
| — | `dasllama_arch_llama.das` | config setter + `[init]` registration + Llama-3 chat template |
| — | `dasllama_arch_qwen2.das` | Qwen2 (QKV bias, NEOX rope) + ChatML template |
| — | `dasllama_arch_phi3.das` | Phi-3 (NEOX rope, fused QKV) + `<|user|>` template |
| — | `dasllama_arch_gemma2.das` | Gemma-2 (GeGLU, softcaps, SWA, 4 norms) + `<start_of_turn>` template |
| `dasllama_transformer.das` (same name) | `dasllama_transformer.das` (11 lines) | thin umbrella — `require dasllama_common public` + one require per arch |

- **The require cycle forces the umbrella.** Arch files `require dasllama_common` (they need
  `ArchDesc`/`Config`/`std_blocks`); common can't require them back (and doesn't — arches self-register
  at `[init]`). daslang forbids require cycles, so a third file requires common (public, re-export) + each
  arch (side-effect require, firing its `[init]`). That third file kept the name `dasllama_transformer.das`
  so no consumer require changed.
- **Only `std_blocks()` went public** — the single symbol the arch files pull from common. Everything else
  they touch (`ArchDesc`, `Config`, `FfnAct`, `ChatTemplate`, `register_arch`, `chat_text/special/content`)
  was already public. `resolve_arch` dispatches through the stored `ArchDesc.configure` fn-ptr, so the
  `configure_*` leaves detach with no dangling refs.
- **Both CMake lists updated** — `modules/dasLLAMA/CMakeLists.txt` (`ADD_MODULE_DAS`) and
  `tests/aot/CMakeLists.txt` (`AOT_DASLLAMA_MODULE_FILES`, leaf-order: common before the arches before the
  umbrella) — plus the `.das_module` `register_native_path` list.
- **Pure refactor** — block bodies / forward loops / config unchanged, so token output is identical
  (100/100 JIT + AOT). Adding a new arch (or MoE, with its own blocks) is now a new file + one require
  line in the umbrella, never an edit to the core.
