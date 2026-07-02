# dasLLAMA

daslang-native **CPU** LLM inference (Llama / Qwen2/3 / Phi-3 / Gemma-2/3 transformers). Loads GGUF
(or llama2.c `.bin`), runs the forward pass + KV cache, tokenizes, and decodes —
all in daslang, JIT tier. Verified token-for-token against `llama.cpp` / `llama2.c`.

Run a model: `bin/daslang -jit examples/dasLLAMA/run.das -- <model.gguf>`
Chat: `bin/daslang -jit examples/dasLLAMA/chat.das -- <model.gguf>`

---

## Module layout & usage

dasLLAMA is a pure-daslang module under the `dasllama/` namespace. The core library
lives in `modules/dasLLAMA/dasllama/`; perf benchmarks and verification harnesses sit
beside it under `modules/dasLLAMA/benchmarks/` and `modules/dasLLAMA/harness/`; runnable
demos are in `examples/dasLLAMA/` and tests in `tests/dasLLAMA/`.

```
modules/dasLLAMA/
  .das_module                 # registers the dasllama/ namespace (dynamic binary)
  CMakeLists.txt              # ADD_MODULE_DAS registration (static binary + install)
  API_REWORK.md               # API + backend-seam design record (the plan of record)
  x64_arch.md                 # x64 port docs — seam map; get_x64_going.md = runbook,
  get_x64_going.md            #   tune_for_this_box.md = per-box tuning + measurement discipline
  tune_for_this_box.md
  dasllama/                   # the module — require dasllama/<name>
    dasllama_math.das         #   numeric primitives + matmul/dot kernels (fp32, Q8, Q4) + Q8·Q8 kernel-backend registry
    dasllama_math_default.das #   the portable Q8·Q8 kernel backend (the fallback; platform backends out-rank it)
    dasllama_math_aarch64_neon.das # arm64 SDOT + laneq-repack Q8·Q8 backends ([init]-registered; no-op off-ARM)
    dasllama_par.das          #   maybe_parallel_for threading macro
    dasllama_tune.das         #   per-box kernel loop-hint tuner — [tuned] / [dasllama_grid] (see tune_for_this_box.md)
    dasllama_quant.das        #   Q8_0 / Q4_0 (de)quantization
    dasllama_gguf.das         #   GGUF container parser + tensor transcode
    dasllama_unicode.das      #   Unicode classification + UTF-8 codec
    dasllama_tokenizer.das    #   SentencePiece tokenizer (Llama-2 family, Phi-3, Gemma)
    dasllama_bpe.das          #   byte-level BPE / tiktoken tokenizer (Llama-3 + Qwen2 pre-tokenizers)
    dasllama_common.das       #   engine core — Config / Model / Session, load + forward + generate + sample
    dasllama_arch_llama.das   #   Llama / Llama-2 / Llama-3 / TinyLlama arch (config + chat template)
    dasllama_arch_qwen2.das   #   Qwen2 arch  (per-arch: config setter + [init] registration)
    dasllama_arch_qwen3.das   #   Qwen3 arch (QK-norm)
    dasllama_arch_phi3.das    #   Phi-3 arch
    dasllama_arch_gemma2.das  #   Gemma-2 arch
    dasllama_arch_gemma3.das  #   Gemma-3 arch (SWA pattern + dual rope θ)
    dasllama_arch_gemma4.das  #   Gemma-4 arch (heterogeneous sliding/global geometry + p-RoPE)
    dasllama_transformer.das  #   umbrella — re-exports dasllama_common + registers every arch (require this)
    dasllama_chat.das         #   layer-2 chat engine — Role/Message/ChatSession, template renderer, respond()
  benchmarks/                 # perf harnesses (gen tok/s, prefill TTFT)
    matmul/                   #   matmul kernel micro-bench ledger
  harness/                    # verification / eval test beds, per-box tuners, GGUF inspection tools
tests/dasLLAMA/               # dastest [test] suites (model-gated ones self-skip)
examples/dasLLAMA/            # runnable demos only — run.das (completion + stats), chat.das (REPL)
```

Pull the pieces you need:

```das
require dasllama/dasllama_transformer    // load_model / create_session / eval / generate / sample (+ raw load_gguf / forward)
require dasllama/dasllama_chat           // layer 2: create_chat / add_user / respond (streaming)
require dasllama/dasllama_tokenizer      // SentencePiece
require dasllama/dasllama_bpe            // byte-level BPE (Llama-3 / Qwen2)
require dasllama/dasllama_math           // matmul / rmsnorm / softmax / silu / rope / dot
```

The clean surface is `load_model → create_session → encode → eval → sample / generate
(streaming) → decode / piece`, with chat one layer up (`create_chat → add_user →
respond(model, chat, params) $(piece) { ... }`); design record in `API_REWORK.md`.

---

## Supported models — what runs *for sure*

Legend: ✅ **verified token-for-token** vs the reference · 🚧 in progress · ❌ not supported yet

| Model | Format(s) tested | Arch | Tokenizer | Status | Verified against |
|---|---|---|---|---|---|
| **stories15M** (llama2.c toy) | F32, self-Q8, self-Q4 | Llama-2 | SPM (`.bin`) | ✅ | `llama2.c ./run` greedy, token-for-token (Q8 too; Q4 coherent) |
| **TinyLlama-1.1B-Chat-v0.3** | F16 GGUF | Llama-2 | SPM (GGUF) | ✅ | `llama.cpp`, 40/40 token-for-token (weak model — ChatML) |
| **TinyLlama-1.1B-Chat-v1.0** | Q8_0 GGUF | Llama-2 | SPM (GGUF) | ✅ | `llama.cpp`, 69/69 token-for-token vs fp32; good chat (Zephyr) |
| **Llama-3.2-1B-Instruct** | Q8_0 GGUF | Llama-3 | BPE/tiktoken | ✅ | `llama.cpp` (instrumented `simple_ids`, CPU greedy), 40/40 token-for-token |
| **Llama-3.1-8B-Instruct** | Q8_0 GGUF | Llama-3 | BPE/tiktoken | ✅ | `llama.cpp` (instrumented `simple_ids`, CPU greedy), 40/40 token-for-token (8.5GB, needs the fmap >4GB fix) |
| **SmolLM2-1.7B-Instruct** | Q8_0 GGUF | Llama (arch `llama`) | BPE | ✅ | `llama.cpp` `simple_ids`, 40/40 token-for-token; frozen fixture in `test_parity.das` |
| **Mistral-7B-Instruct-v0.3** | Q8_0 GGUF | Llama (arch `llama`, no SWA) | SPM (GGUF) | ✅ | `llama.cpp` `simple_ids`, 40/40 token-for-token; frozen fixture; chat via the detected `[INST]` template |
| **Qwen2.5-0.5B / 1.5B-Instruct** | Q8_0 GGUF | Qwen2 (QKV bias, NEOX rope, eps 1e-6) | BPE (qwen2 pre) | ✅ | `llama.cpp` `simple_ids` / `harness/parity.sh`: 1.5B 40/40 (frozen fixture); 0.5B matches to ~0.02 logits, flips only genuine near-ties (tiny model) |
| **Qwen3-0.6B / 4B-Instruct-2507** | Q8_0 GGUF | Qwen3 (QK-norm: per-head Q/K RMSNorm pre-RoPE; NEOX rope, no QKV bias) | BPE (qwen2 pre) | ✅ | `llama.cpp` `simple_ids` / `harness/parity.sh`: both 40/40 token-for-token (frozen fixtures in `test_parity.das`) |
| **Phi-3.5-mini-instruct** | Q8_0 GGUF | Phi3 (fused QKV + gate_up, NEOX rope, LongRoPE) | SPM | ✅ | `llama.cpp` `simple_ids` / `harness/parity.sh`: 40/40 (frozen fixture); prose matches to ~0.06 logits, flips only genuine near-ties |
| **Gemma-2-2B-it** | Q8_0 GGUF | Gemma2 (GeGLU, dual softcap, sliding window, sandwich norms, embed ×√dim) | SPM (GGUF) | ✅ | `llama.cpp` `simple_ids`, token-for-token; frozen fixture in `tests/dasLLAMA/test_parity.das`; SWA exercised on a 4k+ context |
| **Gemma-3-1B / 4B-it** | Q8_0 GGUF | Gemma3 (Gemma-2 shape minus softcaps + QK-norm; 5:1 sliding:global layer pattern, dual RoPE θ 10k/1M, linear-8 position scale on the 4B's global layers) | SPM (GGUF) | ✅ | `llama.cpp` `simple_ids` / `harness/parity.sh`: both 40/40 token-for-token (frozen fixtures); sliding mask exercised on a ~900-token prompt (40/40) |
| **Gemma-4-12B-it** | Q8_0 GGUF | Gemma4 (heterogeneous geometry: sliding layers 16Q/8KV heads of 256 vs global 16Q/1KV of 512, per-layer bool SWA pattern; p-RoPE freq factors on global layers only; weightless V-norm; V-from-K on global layers; unit attention scale; per-layer output scale; final softcap; suppressed-token logit bias) | SPM-style BPE (`gemma4`) | ✅ | `llama.cpp` `simple_ids` / `harness/parity.sh`: 40/40 token-for-token on the short counting prompt AND on a ~1490-token window-engaged prompt — the latter is the frozen fixture, encoded in-test through the gemma4 tokenizer (the short prompt's continuation drifts into the channel format on a near-tie, so it stays out of the suite); tokenizer 46/46 vs the `ggml-vocab-gemma-4` corpus |

Models are **not** checked into the repo — they live in `~/Work/llama.cpp/models/`
(gitignored). Get them with `hf download <repo> <file> --local-dir ~/Work/llama.cpp/models`.

### Reproduce the verified runs

```sh
# Greedy completion + prefill/gen tok/s over any supported GGUF (arch + tokenizer auto-detected)
bin/daslang -jit examples/dasLLAMA/run.das -- ~/Work/llama.cpp/models/tinyllama-1.1b-v0.3-f16.gguf
bin/daslang -jit examples/dasLLAMA/run.das -- ~/Work/llama.cpp/models/Llama-3.2-1B-Instruct-Q8_0.gguf

# Interactive chat — ONE REPL for every model; the arch registry picks the chat template + stop tokens
bin/daslang -jit examples/dasLLAMA/chat.das -- ~/Work/llama.cpp/models/tinyllama-1.1b-chat-v1.0.Q8_0.gguf
bin/daslang -jit examples/dasLLAMA/chat.das -- ~/Work/llama.cpp/models/gemma-2-2b-it-Q8_0.gguf

# Token-for-token gates: test_forward (stories15M vs llama2.c) + test_parity (frozen llama.cpp
# simple_ids fixtures: TinyLlama-v0.3, Llama-3.2-1B, gemma-2-2b) — model-gated cases self-skip
bin/daslang -jit dastest/dastest.das -- --test tests/dasLLAMA/

# BPE tokenizer corpus gate (no model needed — uses the in-repo ggml-vocab-llama-bpe fixture)
bin/daslang -jit modules/dasLLAMA/harness/bpe_test.das

# Ad-hoc parity vs llama.cpp for a new model / prompt (wires both sides)
modules/dasLLAMA/harness/parity.sh <model.gguf> [N] [quant] [prompt]
```

---

## Engine capabilities

What a model needs to "just work" today:

| Feature | Supported |
|---|---|
| GGUF weight types (read directly) | **F32, F16, Q8_0, Q4_0** |
| On-the-fly self-quantization | Q8, Q4 (from an F16/F32 model) |
| Architecture | `llama`, `qwen2`, `qwen3`, `phi3`, `gemma2`, `gemma3`, `gemma4` — a self-registering arch registry (`dasllama_arch_*.das`, `[init]`); the loader dispatches on GGUF `general.architecture`, splits Phi3's fused attn_qkv / gate_up at load, and panics with the registered list on an unknown arch |
| Attention | MHA **and** GQA (grouped-query); sliding-window with a per-layer pattern (Gemma-2 alternating, Gemma-3 5 local : 1 global, Gemma-4 explicit per-layer bool array); heterogeneous per-layer geometry (Gemma-4: sliding vs global layers differ in head size AND kv-head count, incl. V-from-K layers with no attn_v tensor); configurable attention-score scale (Gemma-4: 1.0); attention + final-logit soft-capping; suppressed-token logit bias |
| Normalization | RMSNorm — eps from GGUF metadata (1e-5 Llama/Phi3, 1e-6 Qwen2/Qwen3/Gemma); Gemma's pre+post sandwich norms; per-head Q/K norms pre-RoPE (QK-norm — Qwen3, Gemma-3/4); weightless per-head V-norm (Gemma-4) |
| Positional encoding | RoPE — **NORM** (Llama, adjacent-pair) and **NEOX** (Qwen2 / Phi3 / Gemma, pairs offset by head_size/2); per-pair freq scaling + θ from metadata (llama3 NTK-by-parts; Phi3 LongRoPE short factors + attn_factor mscale); θ=10000 default, 500000 Llama-3, 1e6 Qwen2.5; per-layer dual θ + linear position scaling (Gemma-3: sliding layers 10k unscaled, global layers 1M ÷8 on the 4B); p-RoPE — proportional freq factors on global layers only, with per-class head sizes (Gemma-4) |
| FFN | SwiGLU **and** GeGLU (incl. Phi3's fused gate_up, split at load) |
| Tokenizer | **SentencePiece** (Llama-2 family, Phi-3, Gemma), **byte-level BPE / tiktoken** (Llama-3 + Qwen2 pre-tokenizers, exact llama.cpp split), **and SPM-style BPE** (Gemma-4: metaspace escape, newline-only pre-split, raw-UTF-8 ranked merges, `<0xXX>` byte fallback — validated against the `ggml-vocab-gemma-4` corpus) |
| Model size | files >4GB load (needed the fmap >4GB engine fix) |
| QKV bias | **Qwen2** — learned bias on the Q/K/V projections |
| Sampling | greedy, temperature, top-k, repetition penalty (`SamplingParams`; greedy = temp 0, bit-identical to argmax) |
| Chat | per-arch data-driven templates in the registry + one segment-accumulation renderer (`dasllama_chat`) — reproduces the reference prefills token-for-token (`test_chat.das`); the template is auto-detected by sniffing the GGUF's embedded `tokenizer.chat_template` (never executed), falling back to the arch heuristic; Qwen3-style `<think>` blocks are stripped from chat history (`strip_think`); Gemma-4 uses its channel-based turn format (`<|turn>` / `<turn|>`, non-thinking opener closes an empty `thought` channel) |
| Performance | KV cache, SIMD + JobQue-threaded matmul, activation-quant Q8·Q8 behind a pluggable kernel-backend registry (ARM SDOT/laneq today — `x64_arch.md` for the x64 mirror), flash-attention batched prefill, per-box kernel tuning (`tune_for_this_box.md`) |

## Known **not** yet supported

So there's no ambiguity about what will fail:

- **MoE architectures** (Mixtral / Qwen-MoE / GLM family) — the arch registry's block seam is
  built for them, but no MoE FFN block exists yet (`API_REWORK.md` carries the plan; Tier-3
  archs — MLA, Mamba/hybrid, multimodal — are explicitly deferred there).
- **GGUF weight types beyond F32 / F16 / Q8_0 / Q4_0** — no K-quants / IQ / BF16.
- `encode(..., parse_special)` is reserved and currently a no-op — the chat renderer injects
  special-token *ids* directly (so templates work); parsing special tokens out of free text
  doesn't.
- The legacy greedy `generate(t, s, prompt, steps)` (kept for the token-exact oracle path)
  stops on **BOS only**; the streaming `generate(..., params, max_tokens, blk)` and chat
  `respond` stop via the callback + the arch's registered stop tokens.

---

## How to verify a new model (oracle method)

Correctness is proven by reproducing a reference **token-for-token** (greedy / `temp 0`),
keeping the tokenizer out of the loop so only the loader + forward pass are under test:

1. **Prompt token IDs** from the reference: `llama-tokenize -m <model> -p "<prompt>" --ids`.
2. **Expected output IDs** from the instrumented reference oracle `harness/oracle/simple_ids.cpp`
   (a copy of llama.cpp `examples/simple` that prints greedy token ids), or `run_ids` for the
   llama2.c toy. Greedy via `llama_sampler_init_greedy`.
3. Feed the same prompt IDs through dasLLAMA's forward pass (`harness/parity.das`) and diff the
   generated IDs — `harness/parity.sh <model> [N] [quant] [prompt]` wires both sides automatically.
   Once verified, freeze the prompt + continuation as a fixture in `tests/dasLLAMA/test_parity.das`
   so it becomes a self-skipping regression gate.
   Note: on tiny models a near-tie argmax can flip from sub-ULP numerical differences vs llama.cpp's
   libm — diagnose by the top-2 logit margin (a ~0.02 gap is a tie, not a bug), and prefer a larger
   model for a clean token-for-token gate.
4. For the tokenizer itself: the `ggml-vocab-*.gguf.inp`/`.out` fixtures in
   `~/Work/llama.cpp/models/` are ready-made round-trip test vectors.

Perf is compared with `llama-bench -m <model> -ngl 0 -t 8` (CPU, generation `tg`).
