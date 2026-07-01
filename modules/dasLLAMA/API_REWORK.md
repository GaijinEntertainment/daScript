# dasLLAMA API Rework — Plan

**Status:** in progress — Phases 1-5 done, Phase 6 (kernel-backend registry) next. Design locked 2026-07-01.

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
6. **Arch files + kernel-backend registry** *(current)* —
   - **Arch files (done):** split the `dasllama_transformer.das` monolith so each architecture lives
     in its own `dasllama_arch_*.das` (config setter + `[init]` registration + chat template). The
     shared engine is `dasllama_common.das`; `dasllama_transformer.das` is now a thin umbrella
     (`require dasllama_common public` + one require per arch, firing each arch's `[init]`), so every
     existing `require dasllama/dasllama_transformer` is unchanged — zero consumer churn. `std_blocks()`
     is the only symbol promoted to public (the arch files' one pull from common); block bodies, forward
     loops, and config are byte-for-byte unchanged, so the suite stays **token-identical** (100/100 JIT
     + AOT). Adding an arch = a new file, never touching the core; MoE brings its own blocks in its own
     file. See [What split into files](#what-split-into-files-done--phase-6).
   - **Kernel-backend registry (next):** formalize the ISA seam so `dasllama_math_x64_avx.das` can mirror
     the NEON self-registration (SDOT → VNNI `vpdpbusd`).
7. **Tune macro + loop-attribute reification** — depends on 6.

**AOT note (phase 5):** the `Model`↔`ArchBlocks` cycle (Model holds ArchBlocks by value; ArchBlocks's
fn-ptr fields reference Model) tripped the AOT C++ emitter — it emitted `Model` before `ArchBlocks`
(incomplete-type + `sizeof(Model)` mismatch). That emitter bug is **already fixed upstream** (an AOT PR
pending merge). We also declare `ArchBlocks` + its typedefs *before* `Model` in the source — a harmless
defensive ordering (legitimate on its own; emission is source-order for non-recursed structs) that keeps
the AOT green on this branch before that PR lands, and can stay after.

**Below the line (demonstrations):** MoE block (proves Phase 4), collapse the 3 run-demos
→ 1 + `stats()`, an OpenAI-compatible server `/example` (the acceptance test — if it builds
with no reach into internals, the API is right), tool use.

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
