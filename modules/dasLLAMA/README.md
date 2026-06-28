# dasLLAMA

daslang-native **CPU** LLM inference (Llama-architecture transformers). Loads GGUF
(or llama2.c `.bin`), runs the forward pass + KV cache, tokenizes, and decodes —
all in daslang, JIT tier. Verified token-for-token against `llama.cpp` / `llama2.c`.

Run a model: `bin/daslang -jit modules/dasLLAMA/dasllama_gguf_run.das -- <model.gguf>`
Chat: `bin/daslang -jit modules/dasLLAMA/dasllama_chat.das`

---

## Supported models — what runs *for sure*

Legend: ✅ **verified token-for-token** vs the reference · 🚧 in progress · ❌ not supported yet

| Model | Format(s) tested | Arch | Tokenizer | Status | Verified against |
|---|---|---|---|---|---|
| **stories15M** (llama2.c toy) | F32, self-Q8, self-Q4 | Llama-2 | SPM (`.bin`) | ✅ | `llama2.c ./run` greedy, token-for-token (Q8 too; Q4 coherent) |
| **TinyLlama-1.1B-Chat-v0.3** | F16 GGUF | Llama-2 | SPM (GGUF) | ✅ | `llama.cpp`, 40/40 token-for-token (weak model — ChatML) |
| **TinyLlama-1.1B-Chat-v1.0** | Q8_0 GGUF | Llama-2 | SPM (GGUF) | ✅ | `llama.cpp`, 69/69 token-for-token vs fp32; good chat (Zephyr) |
| **Llama-3.2-1B-Instruct** | Q8_0 GGUF | Llama-3 | BPE/tiktoken | 🚧 | dev/iterate oracle for the BPE tokenizer arc |
| **Llama-3.1-8B-Instruct** | Q8_0 GGUF | Llama-3 | BPE/tiktoken | 🚧 | target (needs BPE tokenizer + rope_theta/scaling from metadata) |

Models are **not** checked into the repo — they live in `~/Work/llama.cpp/models/`
(gitignored). Get them with `hf download <repo> <file> --local-dir ~/Work/llama.cpp/models`.

### Reproduce the verified runs

```sh
# stories15M (toy, correctness baseline)
bin/daslang -jit modules/dasLLAMA/dasllama_run.das          # vs ~/Work/llama2.c/stories15M.bin

# TinyLlama F16 / Q8_0 GGUF, text-in -> text-out, token-for-token vs llama.cpp
bin/daslang -jit modules/dasLLAMA/dasllama_gguf_run.das -- ~/Work/llama.cpp/models/tinyllama-1.1b-v0.3-f16.gguf

# Interactive chat (TinyLlama-1.1B-Chat-v1.0 Q8_0, Zephyr template)
bin/daslang -jit modules/dasLLAMA/dasllama_chat.das
```

---

## Engine capabilities

What a model needs to "just work" today:

| Feature | Supported |
|---|---|
| GGUF weight types (read directly) | **F32, F16, Q8_0, Q4_0** |
| On-the-fly self-quantization | Q8, Q4 (from an F16/F32 model) |
| Architecture | `arch == "llama"` only |
| Attention | MHA **and** GQA (grouped-query) |
| Normalization | RMSNorm — eps **hardcoded 1e-5** |
| Positional encoding | RoPE, adjacent-pair convention — θ **hardcoded 10000** |
| FFN | SwiGLU |
| Tokenizer | **SentencePiece** (llama2.c `.bin` + GGUF, `▁`→space) |
| QKV bias | none (not read) |
| Sampling | greedy, temperature, top-k, repetition penalty |
| Performance | KV cache, SIMD + JobQue-threaded matmul, activation-quant Q8·Q8 (ARM SDOT) |

## Known **not** yet supported

So there's no ambiguity about what will fail:

- **`arch != "llama"`** (e.g. Qwen2 `"qwen2"`) — `load_gguf` panics.
- **BPE / tiktoken tokenizer** (Llama-3 vocab 128256, Qwen BPE) — *in progress*; only SentencePiece works today.
- **`rope_theta` / `rms_eps` from metadata** — both hardcoded (10000 / 1e-5). Llama-3 needs θ=500000.
- **llama3 RoPE frequency scaling** — not implemented.
- **QKV bias** (Qwen2) — attention has no bias term.
- **Sliding-window attention** (Mistral, long context) — full attention only.
- Non-chat `generate()` stops on **BOS only** — no EOS/`<|eot_id|>` break yet.

---

## How to verify a new model (oracle method)

Correctness is proven by reproducing a reference **token-for-token** (greedy / `temp 0`),
keeping the tokenizer out of the loop so only the loader + forward pass are under test:

1. **Prompt token IDs** from the reference: `llama-tokenize -m <model> -p "<prompt>" --ids`.
2. **Expected output IDs** from an instrumented reference — `scratchpad/simple_ids.cpp`
   (a copy of llama.cpp `examples/simple` that prints each token id), or `run_ids` for the
   llama2.c toy. Greedy via `llama_sampler_init_greedy`.
3. Feed the same prompt IDs through dasLLAMA's forward pass and diff the generated IDs.
4. For the tokenizer itself: the `ggml-vocab-*.gguf.inp`/`.out` fixtures in
   `~/Work/llama.cpp/models/` are ready-made round-trip test vectors.

Perf is compared with `llama-bench -m <model> -ngl 0 -t 8` (CPU, generation `tg`).
