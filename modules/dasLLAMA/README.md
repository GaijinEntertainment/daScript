# dasLLAMA

daslang-native **CPU** LLM inference (Llama-architecture transformers). Loads GGUF
(or llama2.c `.bin`), runs the forward pass + KV cache, tokenizes, and decodes —
all in daslang, JIT tier. Verified token-for-token against `llama.cpp` / `llama2.c`.

Run a model: `bin/daslang -jit examples/dasLLAMA/gguf_run.das -- <model.gguf>`
Chat: `bin/daslang -jit examples/dasLLAMA/chat.das`

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
  dasllama/                   # the module — require dasllama/<name>
    dasllama_math.das         #   numeric primitives + matmul/dot kernels (fp32, Q8, Q8·Q8, Q4)
    dasllama_quant.das        #   Q8_0 / Q4_0 (de)quantization
    dasllama_gguf.das         #   GGUF container parser + tensor transcode
    dasllama_unicode.das      #   Unicode classification + UTF-8 codec
    dasllama_tokenizer.das    #   SentencePiece tokenizer (Llama-2 family)
    dasllama_bpe.das          #   byte-level BPE / tiktoken tokenizer (Llama-3)
    dasllama_transformer.das  #   Config / Transformer / RunState, load + forward + generate
  benchmarks/                 # perf harnesses (gen tok/s, prefill TTFT)
    matmul/                   #   matmul kernel micro-bench ledger
  harness/                    # verification / eval test beds + GGUF inspection tools
tests/dasLLAMA/               # dastest [test] suites (model-gated ones self-skip)
examples/dasLLAMA/            # runnable demos only — run a prompt, chat
```

Pull the pieces you need:

```das
require dasllama/dasllama_transformer    // load_gguf / load_checkpoint, forward, generate
require dasllama/dasllama_tokenizer      // SentencePiece
require dasllama/dasllama_bpe            // Llama-3 BPE
require dasllama/dasllama_math           // matmul / rmsnorm / softmax / silu / rope / dot
```

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

Models are **not** checked into the repo — they live in `~/Work/llama.cpp/models/`
(gitignored). Get them with `hf download <repo> <file> --local-dir ~/Work/llama.cpp/models`.

### Reproduce the verified runs

```sh
# stories15M (toy, correctness baseline)
bin/daslang -jit examples/dasLLAMA/run.das          # vs ~/Work/llama2.c/stories15M.bin

# TinyLlama F16 / Q8_0 GGUF, text-in -> text-out, token-for-token vs llama.cpp
bin/daslang -jit examples/dasLLAMA/gguf_run.das -- ~/Work/llama.cpp/models/tinyllama-1.1b-v0.3-f16.gguf

# Llama-3 (BPE tokenizer + θ=500000 RoPE + llama3 scaling), token-for-token vs llama.cpp
bin/daslang -jit examples/dasLLAMA/llama3_run.das -- ~/Work/llama.cpp/models/Llama-3.2-1B-Instruct-Q8_0.gguf
bin/daslang -jit examples/dasLLAMA/llama3_run.das -- ~/Work/llama.cpp/models/Meta-Llama-3.1-8B-Instruct-Q8_0.gguf

# BPE tokenizer corpus gate (no model needed — uses the in-repo ggml-vocab-llama-bpe fixture)
bin/daslang -jit modules/dasLLAMA/harness/bpe_test.das

# Interactive chat (TinyLlama-1.1B-Chat-v1.0 Q8_0, Zephyr template)
bin/daslang -jit examples/dasLLAMA/chat.das

# Interactive Llama-3 chat (BPE + Llama-3 template + <|eot_id|> stop); defaults to 3.1-8B
bin/daslang -jit examples/dasLLAMA/llama3_chat.das
bin/daslang -jit examples/dasLLAMA/llama3_chat.das -- ~/Work/llama.cpp/models/Llama-3.2-1B-Instruct-Q8_0.gguf
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
| Normalization | RMSNorm — eps hardcoded 1e-5 (correct for Llama-2/3) |
| Positional encoding | RoPE adjacent-pair; **θ + llama3 NTK-by-parts scaling read from GGUF metadata** (θ=10000 default, 500000 for Llama-3) |
| FFN | SwiGLU |
| Tokenizer | **SentencePiece** (Llama-2 family) **and byte-level BPE / tiktoken** (Llama-3, vocab 128256) |
| Model size | files >4GB load (needed the fmap >4GB engine fix) |
| QKV bias | none (not read) |
| Sampling | greedy, temperature, top-k, repetition penalty |
| Performance | KV cache, SIMD + JobQue-threaded matmul, activation-quant Q8·Q8 (ARM SDOT) |

## Known **not** yet supported

So there's no ambiguity about what will fail:

- **`arch != "llama"`** (e.g. Qwen2 `"qwen2"`) — `load_gguf` panics.
- **QKV bias** (Qwen2) — attention has no bias term.
- **Sliding-window attention** (Mistral, long context) — full attention only.
- Non-chat `generate()` stops on **BOS only** — no EOS/`<|eot_id|>` break yet (chat loops handle their own stop tokens).

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
