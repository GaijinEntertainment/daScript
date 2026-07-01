# dasLLAMA API Rework — Plan

**Status:** in progress — Phase 1. Design locked 2026-07-01.

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
| **2 — block swaps** | **MoE** (FFN → router + experts), QK-norm, partial RoPE, parallel attn+FFN, attention sinks | **This rework.** Not flags — they change block dataflow. |
| **3 — deep forks** | MLA / compressed KV (DeepSeek-V3), Mamba/hybrid, multimodal, bespoke sparse attention (e.g. GLM-5.2 "IndexShare") | **Deferred.** Needs a pluggable KV-cache + attention core, not just blocks. |

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

### Layer 2 — chat (minimal, above the line)
```
struct Message { role : Role; content : string }                 // Role = system | user | assistant | tool
def create_chat(model; system = "") : ChatSession
def add_user(var chat; text : string)
def respond(var chat; params : SamplingParams) ...               // template → encode → eval → sample → stop; appends turn
```

## The two backend seams

### Arch/block seam
`configure_arch`'s monolithic `switch` becomes an **arch registry**. Each arch
self-registers via `[init]` — the same pattern `dasllama_math_aarch64_neon.das` already
uses to register its kernels:
```
register_arch("llama",    { configure, attn = std,     ffn = dense_swiglu, tok = spm, chat = llama3 })
register_arch("qwen3moe", { configure, attn = qk_norm, ffn = moe,          tok = bpe, chat = chatml })
```
The per-layer loop calls **swappable blocks**:
```
x += attention(block.attn, model, session, x, layer)   // std | qk_norm | ...
x += ffn      (block.ffn,  model, x, layer)            // dense_swiglu | dense_geglu | MoE(router + experts)
```
MoE is the one genuinely new block. **Build it general — shared experts + routed top-k,
gating pluggable (sigmoid | softmax)** — the GLM / DeepSeek / Qwen2-MoE shape, *not*
Mixtral's plain-routed special case, or GLM/DeepSeek force a re-plumb. First MoE oracle: a
small shared-expert MoE (lean Qwen1.5-MoE-A2.7B), decided when we get there.

### Kernel/ISA seam
The scaffolding half-exists: `dasllama_math.das` holds the portable/default kernels;
`dasllama_math_aarch64_neon.das` registers NEON kernels at `[init]` via function-pointer
swap, gated on `jit_enabled() && get_architecture_name() == "arm64"`. Finishing the seam =
moving the last shared branches behind the registry so `dasllama_math_x64_avx.das` can
mirror it (SDOT → VNNI `vpdpbusd`). This is what the x64 handoff consumes.

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
3. **Sampling** — `SamplingParams` + sampling `generate` + callback. Greedy (temp=0)
   bit-identical to today's `argmax`. *(current)*
4. **Arch registry + block seam** — relocate the existing 4 arches; prove all 5 models
   still token-exact. No new arch yet. Chat-template descriptor becomes a registry field.
5. **Minimal unified chat app** — collapse the 5 REPLs → 1 engine + per-arch descriptors.
6. **Kernel-backend registry** — formalize so x64 can mirror the NEON self-registration.
7. **Tune macro + loop-attribute reification** — depends on 6.

**Below the line (demonstrations):** MoE block (proves Phase 4), collapse the 3 run-demos
→ 1 + `stats()`, an OpenAI-compatible server `/example` (the acceptance test — if it builds
with no reach into internals, the API is right), tool use.

## What collapses (current state)

- `examples/dasLLAMA/` has 8 programs. The **5 chat REPLs** (`chat`, `gemma_chat`,
  `llama3_chat`, `phi_chat`, `qwen_chat`) are the *same program* — identical `sample()` +
  `prefill()` + REPL loop; the only per-model variation is (a) tokenizer, (b) a
  chat-template descriptor, (c) a stop-token set. The **3 runners** (`run`, `gguf_run`,
  `llama3_run`) are one parameterized completion + oracle check.
- The chat demos hand-roll a token-by-token `prefill()` that never calls the fast
  `forward_prefill` — collapsing to one engine speeds them all up for free.
