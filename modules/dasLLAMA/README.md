# dasLLAMA

dasLLAMA is a **CPU LLM + speech stack written entirely in daslang** — no C++ kernels, no
hand-written assembly. The daslang JIT compiles the whole engine, and a per-box auto-tuner
picks the kernel forms for *your* machine, so the same source ships tuned code on an M1 and
a Threadripper alike. It loads stock GGUF files (llama.cpp's format — single-file or
multi-shard splits, F32/F16/Q8_0/Q4_0/Q5_0/MXFP4 and native Q4_K/Q5_K/Q6_K planes), runs text
LLMs, audio-input "omni" chat models, speech-to-text, and voice-activity detection, and it
is fast: consistently ahead of llama.cpp's CPU inference on prompt processing (up to ~1.7×),
trading blows on generation, and up to ~6× ahead on audio/omni workloads — live scoreboard +
methodology at [daslang.io/dasllama.html](https://daslang.io/dasllama.html). Every supported
model is **verified token-for-token** against its reference implementation before it lands
in the support table.

## Quick start

**Serve it over HTTP** — `utils/dasllama-server/` is a drop-in OpenAI-compatible server
(chat/completions with streaming + tool calling, embeddings, audio transcription); point
opencode, Open WebUI, or the `openai` SDK at `http://127.0.0.1:8080/v1`:

```sh
bin/daslang -jit utils/dasllama-server/main.das -- --model <model.gguf> [--port 8080] \
    [--asr <asr-model>] [--streams 4] [--ctx 4096]
```

Flag table, TOML config, and the scheduler/paged-KV details live in
[utils/dasllama-server/README.md](../../utils/dasllama-server/README.md).

Or drive it from the command line:

```sh
# greedy/sampled completion + tok/s stats
bin/daslang -jit examples/dasLLAMA/run.das -- <model.gguf>

# interactive chat REPL — the arch registry picks the template + stop tokens
bin/daslang -jit examples/dasLLAMA/chat.das -- <model.gguf>

# audio chat (omni models: decoder GGUF + audio mmproj)
bin/daslang -jit examples/dasLLAMA/audio.das -- <decoder.gguf> <mmproj.gguf> <audio-file> [prompt]

# speech-to-text: whisper / parakeet / canary ggml bins, or qwen3-asr GGUF pairs
bin/daslang -jit examples/dasLLAMA/transcribe.das -- <ggml-model.bin | decoder.gguf mmproj.gguf> <audio-file>

# live dictation from the microphone
bin/daslang -jit examples/dasLLAMA/dictate.das -- <asr-model.bin>
```

## Supported model families

- **Text LLMs** (GGUF, arch auto-detected): Llama-2 / Llama-3 (incl. TinyLlama, SmolLM2,
  Mistral-7B), Qwen2.5, Qwen3, Qwen-MoE (Qwen1.5-MoE, Qwen3-30B-A3B), the Qwen3.5 / Qwen3.6
  hybrid-DeltaNet family — dense 0.8B–27B and MoE 35B-A3B / 122B-A10B, including the `-MTP-`
  files' NextN head for **self-speculative decode** (~1.4× decode on the 27B, output-invariant) —
  Qwen3-Coder-Next (80B-A3B), Phi-3.5, Gemma-2, Gemma-3, Gemma-4 (dense 12B/31B, 26B-A4B MoE,
  and the E2B/E4B edge models with per-layer embeddings + cross-layer KV sharing), gpt-oss-20b.
- **Audio-in chat (omni)**: Qwen2-Audio, Qwen2.5-Omni, Qwen3-Omni-30B-A3B, Ultravox v0.5,
  Voxtral-Mini-3B, Gemma-4 E-series audio.
- **Speech-to-text**: the whole Whisper family (tiny → large-v3-turbo, stock whisper.cpp bins),
  Parakeet-TDT 0.6b v2/v3, Qwen3-ASR 0.6B/1.7B, Canary-Qwen 2.5B.
- **Voice activity detection**: Silero-VAD v6 (weights checked in — works with zero setup).

The full per-model table — exact files tested, arch notes, and the token-for-token oracle
evidence — is in [Supported models](#supported-models--what-runs-for-sure) below.

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
    dasllama_math_aarch64_neon.das # arm64 SDOT row-major Q8·Q8 backend + the laneq dot leaves the gen tier composes ([init]-registered; no-op off-ARM)
    dasllama_math_metal.das   #   Metal GPU prefill-GEMM backend (batch-only donor; decode GEMV stays CPU)
    dasllama_metal_prefill.das #  Metal full-GPU-resident prefill kernel set (requant, rmsnorm, rope, attention)
    dasllama_math_gen.das     #   the generated GEMM tier — registers "arm64-gen"/"x64-gen" (load-select repack backends; traversals read the stamped layout)
    dasllama_gemm_schema.das  #   tune_perm grid + layout schema shared by the generator and the runtime
    dasllama_gemm_gen.das     #   the GEMM tile generator (emits per-perm kernels under llvm_tune)
    dasllama_gemm_register.das #  [tune] family registration — manifest > per-ISA fallback chain
    dasllama_par.das          #   maybe_parallel_for threading macro
    dasllama_tune.das         #   per-box kernel loop-hint tuner — [tuned] / [dasllama_grid] (see tune_for_this_box.md)
    dasllama_quant.das        #   Q8_0 / Q4_0 (de)quantization
    dasllama_gguf.das         #   GGUF container parser + tensor transcode
    dasllama_unicode.das      #   Unicode classification + UTF-8 codec
    dasllama_tokenizer.das    #   SentencePiece tokenizer (Llama-2 family, Phi-3, Gemma)
    dasllama_bpe.das          #   byte-level BPE / tiktoken tokenizer (Llama-3 + Qwen2 pre-tokenizers)
    dasllama_common.das       #   engine core — Config / Model / Session, load + forward + generate + sample (incl. MTP/NextN self-spec decode)
    dasllama_prefix.das       #   vLLM-style page-granular prefix cache over the paged KV pool
    dasllama_audio_io.das     #   miniaudio decode (wav/mp3/flac/ogg -> 16 kHz mono f32) + mic capture
    dasllama_audio.das        #   audio tower — whisper mel frontends + encoder core + qwen2a projector (soft-token splice)
    dasllama_whisper.das      #   whisper-proper ASR — ggml-bin loader, cross-attn decoder, greedy driver, transcribe API
    dasllama_parakeet.das     #   Parakeet-TDT ASR — FastConformer + LSTM predictor + duration-skip transducer
    dasllama_canary.das       #   Canary-Qwen ASR — FastConformer-32 encoder + projector over a stock Qwen3 decoder
    dasllama_qwen3a.das       #   Qwen3-ASR audio encoder — conv2d chunk frontend, windowed transformer, MLP projector
    dasllama_gemma4a.das      #   Gemma-4 E-series audio encoder — gemma4a Conformer (macaron FFN, chunked-local RPE attn, conv module, per-op activation clamps) + mm embedder
    dasllama_asr.das          #   uniform ASR surface — load_asr_model (format-sniffed), caps(), family-free verbs
    dasllama_vad.das          #   Silero-VAD (voice activity) — STFT+conv+LSTM forward, batch + streaming segmenter
    dasllama_arch_llama.das   #   Llama / Llama-2 / Llama-3 / TinyLlama arch (config + chat template)
    dasllama_arch_qwen2.das   #   Qwen2 arch  (per-arch: config setter + [init] registration)
    dasllama_arch_qwen3.das   #   Qwen3 arch (QK-norm)
    dasllama_arch_qwen35.das  #   Qwen3.5/3.6 + Qwen3-Next hybrid arches (Gated-DeltaNet + gated attention; qwen35 / qwen35moe / qwen3next)
    dasllama_arch_phi3.das    #   Phi-3 arch
    dasllama_arch_gemma2.das  #   Gemma-2 arch
    dasllama_arch_gemma3.das  #   Gemma-3 arch (SWA pattern + dual rope θ)
    dasllama_arch_gemma4.das  #   Gemma-4 arch (heterogeneous sliding/global geometry + p-RoPE)
    dasllama_arch_qwen2moe.das #  Qwen2-MoE arch (routed top-k + shared expert)
    dasllama_arch_qwen3moe.das #  Qwen3-MoE arch (QK-norm attn + renormalized top-k, no shared expert)
    dasllama_arch_gptoss.das  #   gpt-oss arch (MoE + sinks + YaRN + swiglu_oai + biases)
    dasllama_transformer.das  #   umbrella — re-exports dasllama_common + registers every arch
    dasllama_chat.das         #   layer-2 chat engine — Role/Message/ChatSession, template renderer, respond()
    dasllama.das              #   THE public facade — documented API over engine + chat (require this)
  benchmarks/                 # perf harnesses (gen tok/s, prefill TTFT)
    matmul/                   #   matmul kernel micro-bench ledger
  harness/                    # verification / eval test beds, per-box tuners, GGUF inspection tools
tests/dasLLAMA/               # dastest [test] suites (model-gated ones self-skip)
examples/dasLLAMA/            # runnable demos only — run.das (completion + stats), chat.das (REPL), audio.das (audio chat), transcribe.das (speech-to-text), dictate.das (live mic -> text)
tutorials/dasLLAMA/           # the guided series — 01 generate ... 06 add an arch, 07 speech-to-text, 08 audio chat
```

One require is the whole public API (docs: the `dasllama` page in the stdlib reference):

```das
require dasllama/dasllama                // load_model / create_session / encode / eval / sample /
                                         // generate (streaming) / decode / piece + chat: create_chat /
                                         // add_user / respond — and it re-exports the engine below
```

Engine internals remain reachable for tools and kernel work:

```das
require dasllama/dasllama_transformer    // engine spellings (load_model_, eval_, ...) + raw load_gguf / forward
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
| **Qwen1.5-MoE-A2.7B-Chat** | Q8_0 GGUF | Qwen2-MoE (routed experts: softmax router → top-4 of 60, un-renormalized weights; sigmoid-gated shared expert; QKV bias, NEOX rope) | BPE (qwen2 pre) | ✅ | `llama.cpp` `simple_ids` / `harness/parity.sh`: 40/40 token-for-token on the counting AND prose prompts (frozen fixture in `test_parity.das`) |
| **Qwen3-30B-A3B** | Q8_0 GGUF | Qwen3-MoE (arch `qwen3moe`: Qwen3 attention — per-head QK-norm pre-RoPE, NEOX rope, no QKV bias — with routed experts: softmax router → **renormalized** top-8 of 128, and **no shared expert**; vs qwen2moe's un-renormalized top-4 + sigmoid shared expert. 48 layers, embd 2048, 32Q/4KV heads, expert_ff 768) | BPE (qwen2 pre) | ✅ | `llama.cpp` `simple_ids` / `harness/parity.sh`: 40/40 token-for-token on the counting continuation (frozen fixture in `test_parity.das`, both grouped + reference prefill paths, bit-identical). High-entropy prompts diverge at the model's OWN near-ties — a prose probe flips at the first coin-flip (oracle top-1/top-2 gap 0.04 at step 6, das picks the oracle's #2), **not** a forward bug: das tracks the oracle through margins as tight as 0.08 logits, no systematic router flips (the shared float `ffn_norm` already matches ggml's double-accumulated norm closely enough for the top-8-of-128 pick) |
| **Qwen3.5-0.8B** | Q8_0 GGUF | Qwen3.5 (arch `qwen35` hybrid: 18 Gated-DeltaNet recurrent layers + 6 gated full-attention layers — 2×-wide Q with per-head sigmoid out-gate, partial NEOX rope 64/256, QK-norm) | BPE (qwen2 pre) | ✅ | `llama.cpp` `simple_ids` / `harness/parity.sh`: token-for-token on the counting continuation, both the per-token and chunked-DeltaNet prefill paths (frozen fixture in `test_parity.das`) |
| **Qwen3.6-35B-A3B** | UD-Q4_K_M GGUF (native K-quant planes) | Qwen3.5-MoE (arch `qwen35moe`: the qwen35 hybrid — 30 DeltaNet + 10 gated-attention layers, 16Q/2KV with 32 V-heads over 16 K-heads (modulo GQA-repeat) — plus routed experts: softmax top-8-of-256 renormalized + sigmoid-gated shared expert; F32 ssm_beta/alpha on the fp32 matmul rail; K-quant expert stacks) | BPE (qwen2 pre) | ✅ | `llama.cpp` `simple_ids` / `harness/parity.sh`: token-for-token on the counting continuation across all three prefill paths — grouped MoE, reference, and chunked-DeltaNet (frozen large-tier fixture in `test_parity.das`) |
| **Qwen3-Coder-Next (80B-A3B)** | Q4_K_M GGUF (native K-quant planes) | Qwen3-Next (arch `qwen3next`: the parent of the qwen35 hybrid — 36 DeltaNet + 12 gated-attention layers, softmax top-10-of-512 + sigmoid-gated shared expert. The FILE layout differs from qwen35: fused Q4_K `ssm_ba` and HF grouped v-head order; the loader splits ba and permutes every v-indexed tensor to the kernels' tiled order at load) | BPE (qwen2 pre) | ✅ | `llama.cpp` `simple_ids` / `harness/parity.sh`: token-for-token on the counting continuation, per-token and chunked-DeltaNet prefill (frozen huge-tier fixture in `test_parity.das`) |
| **Phi-3.5-mini-instruct** | Q8_0 GGUF | Phi3 (fused QKV + gate_up, NEOX rope, LongRoPE) | SPM | ✅ | `llama.cpp` `simple_ids` / `harness/parity.sh`: 40/40 (frozen fixture); prose matches to ~0.06 logits, flips only genuine near-ties |
| **Gemma-2-2B-it** | Q8_0 GGUF | Gemma2 (GeGLU, dual softcap, sliding window, sandwich norms, embed ×√dim) | SPM (GGUF) | ✅ | `llama.cpp` `simple_ids`, token-for-token; frozen fixture in `tests/dasLLAMA/test_parity.das`; SWA exercised on a 4k+ context |
| **Gemma-3-1B / 4B-it** | Q8_0 GGUF | Gemma3 (Gemma-2 shape minus softcaps + QK-norm; 5:1 sliding:global layer pattern, dual RoPE θ 10k/1M, linear-8 position scale on the 4B's global layers) | SPM (GGUF) | ✅ | `llama.cpp` `simple_ids` / `harness/parity.sh`: both 40/40 token-for-token (frozen fixtures); sliding mask exercised on a ~900-token prompt (40/40) |
| **Gemma-4-12B-it** | Q8_0 GGUF | Gemma4 (heterogeneous geometry: sliding layers 16Q/8KV heads of 256 vs global 16Q/1KV of 512, per-layer bool SWA pattern; p-RoPE freq factors on global layers only; weightless V-norm; V-from-K on global layers; unit attention scale; per-layer output scale; final softcap; suppressed-token logit bias) | SPM-style BPE (`gemma4`) | ✅ | `llama.cpp` `simple_ids` / `harness/parity.sh`: 40/40 token-for-token on the short counting prompt AND on a ~1490-token window-engaged prompt — the latter is the frozen fixture, encoded in-test through the gemma4 tokenizer (the short prompt's continuation drifts into the channel format on a near-tie, so it stays out of the suite); tokenizer 46/46 vs the `ggml-vocab-gemma-4` corpus |
| **Gemma-4-31B-it** | Q8_0 GGUF | Gemma4 — the same heterogeneous arch as the 12B, scaled (60 layers, embd 5376, 32Q / [16×5,4]KV heads; PLE off, no shared-KV, final softcap 30, 5-local:1-global SWA) | SPM-style BPE (`gemma4`) | ✅ | `llama.cpp` `simple_ids` / `harness/parity.sh`: 40/40 token-for-token on the ~1490-token window-engaged counting prompt — runs the existing gemma4 forward unchanged; the frozen fixture reuses the 12B continuation (byte-identical, both count) |
| **Gemma-4-26B-A4B-it** | Q8_0 / Q4_K_M GGUF (the Q4_K_M mix bumps layer-3+ `ffn_down`/`ffn_down_exps` to Q5_0 — loaded via the exact Q5_0→Q8 transcode; the Q4_K/Q6_K rest ride native kq planes) | Gemma4 **MoE** (30 layers, embd 2816, 16Q / [8×5,2]KV heads; 128 experts top-8, expert_ff 704, softcap 30, SWA 1024, PLE off). Every layer SUMS two parallel branches: a **dense shared expert** (`ffn_norm` → GeGLU → `post_ffw_norm_1`, always on) and **routed experts** (`pre_ffw_norm_2` → softmax top-8-of-128 renormalized → `post_ffw_norm_2`), then a shared `post_ffw_norm` on the sum. Custom-normalized router — weightless RMS(attn_out) × 1/√dim × learned `ffn_gate_inp_s` (double-accumulated to match ggml's `ggml_float`, since it feeds a discrete selection); fused `ffn_gate_up_exps` split into the separate gate/up stacks at load; a per-expert down-output scale folded into the routed weights | SPM-style BPE (`gemma4`) | ✅ | `llama.cpp` `simple_ids` / per-step logit margins: token-for-token on the confident head of the counting continuation (frozen 11-token fixture in `test_parity.das`). Past that the model's OWN routing is near-tied (oracle top-1/top-2 gap ~0.5–2.2 at several counting steps) — the discrete top-8-of-128 routing lets a float residual land on the other side of the coin-flip; verified **not** a forward bug (matches the oracle at every confident step; the double router RMS removes the systematic flips). The Q4_K_M file holds a 15-token confident head the same way (its own per-file oracle; frozen fixture in `test_parity.das`) |
| **Gemma-4-E4B-it** | Q8_0 GGUF | Gemma4 **edge** member (42 layers, embd 2560, 8Q/2KV heads; 5-local:1-global SWA, window 512, softcap 30). Two features beyond the dense 12B/31B: **PLE (per-layer embeddings)** — a per-(token, layer) side input (gather the token's Q8 PLE row, project the scaled embedding through `per_layer_model_proj`, RMSNorm + combine) mixed into each layer's output by `gelu(inp_gate·cur) ⊙ ple_inp[l] → proj → post_norm`, before the layer scale; and **cross-layer KV sharing** — the last 18 layers carry no `attn_k`/`attn_v`, projecting Q only and attending against an earlier layer's cached K/V (sliding → `kv_from_start-2`, global → `kv_from_start-1`), mirroring llama.cpp gemma4's `has_kv()`/`reuse()` (their own K/V weights are dead in the file, skipped at load) | SPM-style BPE (`gemma4`) | ✅ | `llama.cpp` `simple_ids` / `harness/parity.sh`: 40/40 token-for-token on the ~1490-token window-engaged counting prompt (the 512 window is genuinely engaged, kstart > 0) — dense, so it holds the full 40; the frozen fixture reuses the 12B continuation (byte-identical, same counting momentum + vocab) |
| **Gemma-4-E2B-it** | Q8_0 GGUF | Gemma4 edge model (35 layers, embd 1536, 8Q/1KV heads) — PLE + cross-layer KV sharing exactly as the E4B, plus **per-layer FFN width**: `feed_forward_length` is a per-layer array (6144 for layers 0-14, 12288 for 15-34 — the elastic MatFormer sub-model of the E4B), so the dense FFN weight offsets vary per layer (`ffn_w` / `w1_offs`·`w2_offs`·`w3_offs`; `hidden_dim` is the max for scratch) | SPM-style BPE (`gemma4`) | ✅ | `llama.cpp` `simple_ids` / `harness/parity.sh`: 40/40 token-for-token on the window-engaged 1..320 counting prompt; reuses the 12B continuation. Fast-tier (~5 GB) — runs in the default test loop |
| **Qwen2-Audio-7B-Instruct** (audio input) | self-converted F16→Q8_0 + f32 mmproj (ggml-org publishes no pre-quant — convert with the in-tree `convert_hf_to_gguf.py`, decoder + `--mmproj`) | Qwen2 decoder untouched + whisper-large-v3 encoder tower (fp32: mel→conv×2→32 blocks→avg-pool→linear projector; soft tokens spliced at `<|audio_bos|>…<|audio_eos|>` via `forward_prefill_embd`) | BPE (qwen2 pre) | ✅ | `llama-mtmd-cli --temp 0`: token-for-token on jfk.wav AND tools/mtmd/test-2.mp3; encoder gated vs `llama-mtmd-debug` all-ones mel (test_audio.das); mel gated vs the 440 Hz preproc dump (3.1e-05) — rig + findings in `qwen2_audio_plan.md` |
| **Ultravox v0.5** (audio input): llama-3.2-1b + llama-3.1-8b | STOCK Llama-3 Q8_0 GGUFs (decoders untouched) + ggml-org f16 mmproj (oracle runs use a local f16→f32 re-encode so mtmd computes in fp32; our loader reads either) | whisper-large-v3 tower + `ultravox` projector: post-LN → stack-8 frame concat (floor rows, ragged tail dropped) → RMS-pre → mm1 → swiglu_swapped → RMS-mid → mm2, no biases; 187 soft tokens/chunk spliced INLINE in the llama-3 template (no audio delimiter tokens) | BPE/tiktoken | ✅ | `llama-mtmd-cli --temp 0`: 1b token-for-token on jfk.wav AND tools/mtmd/test-2.mp3; 8b token-for-token on jfk.wav; tower shapes gated in `test_audio.das` |
| **Qwen2.5-Omni 3B / 7B** (audio input) | ggml-org Q8_0 GGUFs + f16 mmproj (same local f32 re-encode for the oracle; the combined mmproj's `v.*` vision tensors are skipped by the name-driven loader) | decoder is arch `qwen2vl` — M-RoPE whose coordinates all equal the linear position for text+audio, so the plain qwen2 forward is bit-identical (registered as a qwen2 alias; vision out of scope); audio side = the QWEN2A tower + projector VERBATIM (750 tokens/chunk at `<|audio_bos|>…<|audio_eos|>`) | BPE (qwen2 pre) | ✅ | `llama-mtmd-cli --temp 0`: 3B token-for-token on jfk.wav AND test-2.mp3; 7B token-for-token on jfk.wav |
| **Voxtral-Mini-3B** (audio input) | self-converted decoder Q8_0 + mmproj f32 (ggml-org publishes only K-quants; `convert_hf_to_gguf.py` needs `mistral-common` + a local missing-`return` fix, see `audio_models_plan.md`) | llama-arch decoder (head_dim 128, rope θ 1e8) + whisper tower + `voxtral` projector: avg-pool → post-LN → stack-4 → mm1 → gelu_erf → mm2, no biases; 187 tokens/chunk after `[BEGIN_AUDIO]` in the `[INST]…[/INST]` v7-tekken template; NEW `tekken` pre-tokenizer (generic-wregex alternation semantics — no contractions, single digits, '/' punct tail) | BPE (`tekken` pre) | ✅ | `llama-mtmd-cli --temp 0`: transcription prompt on jfk.wav byte-identical to natural stop; tokenizer id-for-id vs `llama-tokenize` (incl. 你A / contraction / digit probes); decoder 40/40 text-only (`harness/parity.sh`); encoder ≤5e-4 vs mtmd on real input. The two open-caption prompts each flip ONE step sitting at a 0.079 / 0.195-logit near-tie (p≈0.5 coin-flips; every matched step clears gaps up to 12 logits) — same knife-edge class as whisper tiny/base, not chased |
| **Qwen3-ASR 0.6B / 1.7B** (speech-to-text) | ggml-org Q8_0 decoder + bf16 mmproj GGUFs (read directly — bf16→f32 is exact) | decoder is arch `qwen3vl` (M-RoPE degenerates for text+audio, deepstack inactive without vision — registered as a qwen3 alias) + NEW `qwen3a` encoder: 100-frame mel chunks through 3 stride-2 conv2d blocks (13×16×480), features to d_model, per-chunk position reset, transformer over 800-frame windows (13 tokens/chunk, k-biased attention on the shared tower block loop), GELU MLP projector; LLM-style transcription — the model emits `language {L}<asr_text>{text}`, parsed into `detected_lang` + one segment (no timestamps); context biasing via `AsrSession.context` | BPE (qwen2 pre) | ✅ | `llama-mtmd-cli --temp 0`: 0.6B transcript byte-identical on jfk.wav AND tools/mtmd/test-2.mp3 (88-token read of the NYT clip); 1.7B byte-identical on jfk.wav; encoder gated vs `MTMD_DEBUG_EMBEDDINGS` per-window token-0 (≤1e-4) in `test_whisper.das` |
| **Parakeet-TDT 0.6b-v2** (speech-to-text) | self-converted `.nemo` → ggml bin (whisper.cpp's in-tree `convert-parakeet-to-ggml.py`, `--use-f32`) | NEW family end to end: frame-major mel (preemph 0.97, zero-center-pad, natural log, per-feature Bessel normalization) → conv2d subsampling (regular+depthwise ×2, 8× time) → 24 FastConformer blocks (macaron half-FFNs ×0.5, rel-pos attention with u/v biases + interleaved sin/cos table + scalar rel-shift gather, GLU→depthwise-k9→inference-BN→SiLU conv module, per-layer post-norm) → 2-layer LSTM predictor ([i,f,o,g] packed gates) → TDT joint (relu, full-row log-softmax) + duration-skip greedy | SPM (▁ pieces from the bin) | ✅ | `parakeet-cli -ng -ps` (CPU, f32): **jfk.wav 33/33 and gb1 (3-min Columbia address) 786/786 token-for-token** — ids, frames, duration indices AND values; the duration argmax must run over fp32 softmax values, not raw logits (the oracle's underflow-to-−inf tie-break, see `parakeet_plan.md`); suite gate in `test_whisper.das` |
| **Parakeet-TDT 0.6b-v3** (speech-to-text, 25 European languages) | same converter as v2 (`--use-f32`) | the v2 pipeline verbatim — only the SPM vocab grows to 8192 pieces (joint row 8198); the loader reads every dim from the header, the ASR sniff routes any sub-32768 ggml-bin vocab to parakeet, and `caps()` flips to the 25-language list on the 8192-piece vocab | SPM (▁ pieces from the bin) | ✅ | `parakeet-cli -ng -ps` (CPU, f32): **jfk.wav 38/38 and gb1 655/655 token-for-token** — ids, frames, duration indices AND values; q8 default stays id/text-exact on both (jfk durations too; gb1 flips 16/655 duration picks only — ≤4-frame transient timestamp drift, always re-syncing); suite gate in `test_whisper.das` |
| **Canary-Qwen 2.5B** (speech-to-text, tops Open ASR) | self-converted from HF-native safetensors (`harness/convert_canary_to_ggml.py`) → **two files**: a LoRA-merged Qwen3-1.7B decoder GGUF (fold `W += 2.0·B·A` on q/v_proj at conversion) + a "CNRY" FastConformer encoder bin (all-f32; bf16→f32 exact) | SALM = **FastConformer-32 encoder + linear projector + unmodified Qwen3-1.7B decoder** (new module `dasllama_canary.das`). Encoder = parakeet's conformer neighborhood PLUS the qkv/ffn/conv biases parakeet lacks (32 layers, d_model 1024, 8 heads, dw-striding ×8 through NeMo's **MaskedConvSequential** length mask — zero the time-rows past the propagated valid length before each stride-2 conv — rel-pos u/v attention, GLU→depthwise-k9→BN→SiLU conv module with the depthwise bias folded into BN), then `perception.proj` (1024→2048); no TDT/predictor/joint. Soft tokens splice at `<\|audioplaceholder\|>` in the SALM/Qwen chatml prompt (`user\nTranscribe the following: <AUDIO><\|im_end\|>\n…assistant`), greedy decode until the text EOS (151645, emitted into the ids). New `AsrKind.canary` behind the uniform ASR surface — `load_asr_model(decoder, encoder_bin)`, routed by the "CNRY" mmproj magic | BPE (qwen2 pre) | ✅ | **NeMo SALM greedy** (`do_sample=False`, engine-level python — NO cli/llama.cpp/onnx-asr canary support exists): **jfk + 2 LibriSpeech clips token-for-token** incl. trailing EOS vs `harness/canary_qwen_expected.tsv`; encoder+projector gated bit-exact (≤1e-4, every layer 0.0 rel) vs the NeMo perception dump on jfk; large-tier fixture `test_canary_qwen_oracle` in `test_whisper.das` (`DASLLAMA_PARITY_FULL=1`). Perf A/B (fp32) in `benchmarks/asr/results.md` — das leads the NeMo reference ~1.3–1.5x |
| **Whisper — full family** (speech-to-text): tiny/base/base.en/small/medium/large-v3/large-v3-turbo | stock whisper.cpp `ggml-*.bin` (f16, read directly — no conversion) | Whisper encoder (the qwen2a tower core, tanh-fp16-LUT gelu, ln_post tail) + NEW text decoder w/ cross-attention (learned abs pos, tied logits, ≤448 ctx) + `whisper_full`'s greedy driver (timestamp rules, no-speech gate, long-form seek) | byte-level vocab from the bin | ✅ | `whisper-cli` CPU greedy (`-bs 1 -bo 1 -nf -ng`), jfk.wav w/ timestamps: **tiny, base.en, small, medium, large-v3, large-v3-turbo all token-for-token** (turbo also on 33 s long-form, 3 windows exact); base flips ONE step where the force-timestamp rule sits at a 2e-4-logit margin (oracle's own p=0.495 — a coin flip; base is exact in `-nt` mode) — rig + findings in `whisper_plan.md`; suite gate in `test_whisper.das` |
| **Silero-VAD v6** (voice activity detection, 16 kHz) | checked-in `models/silero_vad.bin` (~1.2 MB, MIT © Silero Team — converted from the official `silero_vad_16k_op15.onnx` initializers, NOT the safetensors, which ship non-reparameterized weights) | STFT-as-conv (258-basis, reflect-pad 640, 4 hops → 129-bin magnitude) → 4× conv1d encoder (129→128→64→64→128, k3, strides 1/2/2/1, ReLU) → single LSTM cell (hidden 128, i,f,g,o) → conv1d 128→1 + sigmoid; per 512-sample chunk carrying a 64-sample context + LSTM h/c | — | ✅ | onnxruntime `silero_vad_16k_op15.onnx` (proven bit-exact to silero's `OnnxWrapper`): per-frame prob **≤4.6e-6** (jfk 344 frames, gb1 6211); `find_speech_timestamps` == silero `get_speech_timestamps` **sample-for-sample** (jfk 4/4, gb1 73/73 segments); streaming `VadIter` == silero `VADIterator` (jfk 9/9, gb1 146/146 events); suite gate in `test_vad.das` (self-contained LCG + jfk segments) |
| **Gemma-4 E2B / E4B** (audio input) | ggml-org Q8_0 decoder + **bf16 audio mmproj** GGUFs (read directly — bf16→f32 exact, no conversion) | Gemma-4 E-series text decoder (unchanged — the G3 PLE + cross-layer-KV path) + NEW `gemma4a` **Conformer** audio encoder (`dasllama_gemma4a.das`): 128-mel HTK filterbank → Conv2D ×2 subsample (LN-over-channels + ReLU) → input proj → 12 blocks { half-step macaron FFN (SiLU) · causal sliding-window-12 self-attn with sinusoidal RPE (`matrix_bd[12−d]`) + softcap 50 + per-dim Q scale · conv module: pw1 → GLU → causal depthwise conv1d k=5 → RMSNorm → SiLU → pw2 } → out proj → weightless RMSNorm → mm input proj. 🔑 the FFN/attn/conv matmuls honor **per-op activation-quant calibration ranges** (clamp-in/clamp-out); 🔑 the conv `norm_conv`/`conv_norm` tensors are name-swapped in the GGUF. Soft tokens splice at `<\|audio>…<audio\|>` in the Gemma-4 turn template via `forward_prefill_embd`; new `AsrKind.gemma4a`, routed by the mmproj's `clip.audio.projector_type='gemma4a'` | SPM (gemma) | ✅ | `llama-mtmd-cli --temp 0 --jinja` (no-think): E2B encoder soft tokens match mtmd's audio embeddings at rel ~0.0027 (float-order noise); greedy transcription token-for-token through the 48-token confident prefix, diverging only at the Gemma-4 q8 decoder's OWN ~0.6-logit near-tie (llama.cpp's raw logits there: 3 tokens within 0.6). Fixture `test_gemma4a_audio_oracle` in `test_whisper.das`; perf A/B in `benchmarks/asr/results.md` |
| **Qwen3-Omni-30B-A3B-Instruct** (audio input) | ggml-org Q8_0 thinker + **bf16 audio mmproj** GGUFs (read directly — bf16→f32 exact, no conversion; ~34 GB) | Audio-in/text-out (Talker/speech-generation out of scope). Thinker = the SHIPPED `qwen3moe` forward, registered as arch `qwen3vlmoe` (= qwen3moe + M-RoPE whose coordinates all equal the linear position for text+audio so any section layout degenerates to standard rope, + deepstack inactive without vision — bit-identical to the plain qwen3moe forward: 48 layers, embd 2048, 32Q/4KV heads, 128 experts top-8, expert_ff 768). Audio side = the SHARED `qwen3a` AuT tower VERBATIM (data-driven from the mmproj: d_model 1280, 32 blocks, 20 heads, ff 5120, proj 2048 — same conv2d-chunk frontend + 800-frame windowing as Qwen3-ASR, just bigger dims). Greedy transcription turn: soft tokens splice at `<\|audio_start\|>…<\|audio_end\|>` in a bare Qwen chatml user turn (non-thinking Instruct), decoded until `<\|im_end\|>`; new `AsrKind.qwen3omni`, routed by the `qwen3vlmoe` decoder arch under the same `qwen3a` mmproj sniff (reuses the decoder + tower fields — zero frame growth) | BPE (qwen2 pre) | ✅ | `llama-mtmd-cli --temp 0 --jinja` (same Q8 thinker + bf16 mmproj): **jfk + 2 LibriSpeech clips FULL token-for-token** incl. the trailing `<\|im_end\|>` (151645), with NO near-tie divergence. Large-tier fixture `test_qwen3omni_audio_oracle` in `test_whisper.das` (`DASLLAMA_PARITY_FULL=1`); perf A/B in `benchmarks/asr/results.md` |
| **gpt-oss-20b** | MXFP4 GGUF → self-Q8 | gpt-oss (MoE: 32 experts top-4, softmax over the SELECTED weights, no shared expert; router + per-expert + QKV + output-projection biases; clamped SwiGLU `swiglu_oai`; per-head attention sinks; alternating 128-token SWA; YaRN rope factor 32 over a 4096 original context) | BPE (`gpt-4o` pre) | ✅ | `llama.cpp` `simple_ids` / `harness/parity.sh`: 40/40 token-for-token on the counting prompt AND on a 449-token window-engaged prompt encoded in-test through the gpt-4o pre-tokenizer (both frozen fixtures in `test_parity.das`); tokenizer verified id-for-id vs llama.cpp on counting / mixed-case-contraction / whitespace probes |

Models are **not** checked into the repo — they live in `~/Work/llama.cpp/models/`
(gitignored). Get them with `hf download <repo> <file> --local-dir ~/Work/llama.cpp/models`.
The one exception is the tiny Silero-VAD weights (`models/silero_vad.bin`, ~1.2 MB, MIT), which
are checked in so the VAD runs — and its test gates — with zero setup.

### Reproduce the verified runs

```sh
# Greedy completion + prefill/gen tok/s over any supported GGUF (arch + tokenizer auto-detected)
bin/daslang -jit examples/dasLLAMA/run.das -- ~/Work/llama.cpp/models/tinyllama-1.1b-v0.3-f16.gguf
bin/daslang -jit examples/dasLLAMA/run.das -- ~/Work/llama.cpp/models/Llama-3.2-1B-Instruct-Q8_0.gguf

# Interactive chat — ONE REPL for every model; the arch registry picks the chat template + stop tokens
bin/daslang -jit examples/dasLLAMA/chat.das -- ~/Work/llama.cpp/models/tinyllama-1.1b-chat-v1.0.Q8_0.gguf
bin/daslang -jit examples/dasLLAMA/chat.das -- ~/Work/llama.cpp/models/gemma-2-2b-it-Q8_0.gguf

# Audio chat: whisper-encoder tower + soft-token splice into the decoder. The mmproj's
# projector type picks the prompt shape — the same demo runs Qwen2-Audio, Qwen2.5-Omni,
# Ultravox (stock Llama-3 decoders), and Voxtral pairs:
bin/daslang -jit examples/dasLLAMA/audio.das -- ~/Work/llama.cpp/models/qwen2audio-7b-q8_0.gguf \
    ~/Work/llama.cpp/models/qwen2audio-mmproj-f32.gguf ~/Work/llama.cpp/models/jfk.wav "What is being said in this audio?"
bin/daslang -jit examples/dasLLAMA/audio.das -- ~/Work/llama.cpp/models/Llama-3.2-1B-Instruct-Q8_0.gguf \
    ~/Work/llama.cpp/models/mmproj-ultravox-1b-f32.gguf ~/Work/llama.cpp/models/jfk.wav

# Speech-to-text (Whisper): any audio file -> timestamped segments, stock whisper.cpp models
bin/daslang -jit examples/dasLLAMA/transcribe.das -- ~/Work/whisper.cpp/models/ggml-large-v3-turbo.bin \
    ~/Work/whisper.cpp/samples/jfk.wav

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
| GGUF weight types (read directly) | **F32, F16, Q8_0, Q4_0, MXFP4, and the K-quant planes Q4_K / Q5_K / Q6_K** — K-quant tensors keep their native format and run on dedicated kq kernels (a Q4_K_M file mixes formats per tensor); MXFP4 expert stacks stay native too (mx4·q8 kernels); bf16 audio-tower mmprojs read exactly |
| On-the-fly self-quantization | Q8, Q4 (from an F16/F32 model) |
| Architecture | `llama`, `qwen2`, `qwen3`, `qwen35`, `phi3`, `gemma2`, `gemma3`, `gemma4`, `qwen2moe`, `qwen3moe`, `qwen35moe`, `gpt-oss` — a self-registering arch registry (`dasllama_arch_*.das`, `[init]`); the loader dispatches on GGUF `general.architecture`, splits Phi3's fused attn_qkv / gate_up at load, and panics with the registered list on an unknown arch |
| Attention | MHA **and** GQA (grouped-query); sliding-window with a per-layer pattern (Gemma-2 alternating, Gemma-3 5 local : 1 global, Gemma-4 explicit per-layer bool array); heterogeneous per-layer geometry (Gemma-4: sliding vs global layers differ in head size AND kv-head count, incl. V-from-K layers with no attn_v tensor); **cross-layer KV sharing** (Gemma-4 E-series: later layers carry no K/V of their own — Q only, attending against an earlier layer's cached K/V); configurable attention-score scale (Gemma-4: 1.0); per-head attention-sink logits in the softmax denominator (gpt-oss, all three prefill cores + decode); attention + final-logit soft-capping; suppressed-token logit bias |
| Normalization | RMSNorm — eps from GGUF metadata (1e-5 Llama/Phi3, 1e-6 Qwen2/Qwen3/Gemma); Gemma's pre+post sandwich norms; per-head Q/K norms pre-RoPE (QK-norm — Qwen3, Gemma-3/4); weightless per-head V-norm (Gemma-4) |
| Positional encoding | RoPE — **NORM** (Llama, adjacent-pair) and **NEOX** (Qwen2 / Phi3 / Gemma, pairs offset by head_size/2); per-pair freq scaling + θ from metadata (llama3 NTK-by-parts; Phi3 LongRoPE short factors + attn_factor mscale); θ=10000 default, 500000 Llama-3, 1e6 Qwen2.5; per-layer dual θ + linear position scaling (Gemma-3: sliding layers 10k unscaled, global layers 1M ÷8 on the 4B); p-RoPE — proportional freq factors on global layers only, with per-class head sizes (Gemma-4); YaRN (gpt-oss: factor 32 / original context 4096) — the NTK-by-parts ramp folds into the same per-pair factor machinery at load, magnitude correction into mscale |
| FFN | SwiGLU, GeGLU **and** clamped `swiglu_oai` (gpt-oss; incl. Phi3's fused gate_up, split at load); **MoE** — routed top-k experts over a softmax / sigmoid / softmax-after-top-k router (un-renormalized or renormalized weights, optional weight scale, optional router + per-expert biases) + optional sigmoid-gated shared expert, expert-major Q8 so the dense kernels apply per expert (`moe_blocks()`; decode fused, prefill naive per-token — grouped GEMM is a ledger item); **Gemma-4 MoE** (`gemma4_blocks()`) — a distinct shape: a **dense parallel shared expert** summed with the routed branch, each with its own pre/post sandwich norms; a **custom-normalized router** (weightless RMS(attn_out) × 1/√dim × learned per-channel scale); fused `ffn_gate_up_exps` split into the gate/up stacks at load; a per-expert down-output scale folded into the routed weights |
| Tokenizer | **SentencePiece** (Llama-2 family, Phi-3, Gemma), **byte-level BPE / tiktoken** (Llama-3 + Qwen2 + gpt-4o/o200k pre-tokenizers, exact llama.cpp split incl. its GPT4O case-class approximation), **and SPM-style BPE** (Gemma-4: metaspace escape, newline-only pre-split, raw-UTF-8 ranked merges, `<0xXX>` byte fallback — validated against the `ggml-vocab-gemma-4` corpus) |
| Model size | files >4GB load (needed the fmap >4GB engine fix); single >4GB in-memory arrays (a 7.1GB Q8 weight blob, a 4GiB KV cache at native seq_len) work on the default context heap — no `options persistent_heap` needed (needed the 64-bit linear-chunk engine fix) |
| Attention biases | **Qwen2** — Q/K/V projection biases; **gpt-oss** — Q/K/V + output-projection biases |
| Sampling | greedy, temperature, top-k, top-p, min-p, repetition/presence/frequency penalties (`SamplingParams`; greedy = temp 0, bit-identical to argmax) |
| Chat | per-arch data-driven templates in the registry + one segment-accumulation renderer (`dasllama_chat`) — reproduces the reference prefills token-for-token (`test_chat.das`); the template is auto-detected by sniffing the GGUF's embedded `tokenizer.chat_template` (never executed), falling back to the arch heuristic; Qwen3-style `<think>` blocks are stripped from chat history (`strip_think`); Gemma-4 uses its channel-based turn format (`<|turn>` / `<turn|>`, non-thinking opener closes an empty `thought` channel); gpt-oss uses the Harmony-lite template (`<|start|>role<|message|>...<|end|>`, replies end at `<|return|>` / `<|call|>`, channel markers render in decoded text as in llama.cpp) |
| Self-speculative decode (MTP/NextN) | qwen35-family `-MTP-` GGUFs load natively (`nextn_predict_layers == 1`): the in-file NextN draft block — one trunk-shaped attention layer at index `n_layers` plus `nextn.eh_proj/enorm/hnorm/shared_head_norm` — drafts one token ahead and a 2-row batched trunk forward verifies it, so accepted steps stream the weights once for two tokens. **Output-invariant vs plain greedy** (gated by `tests/dasLLAMA/test_mtp.das` + the scheduler arm in `test_llm_scheduler.das`); 1.39× decode on Qwen3.6-27B Q8 flat-loop, ~2× through the server (zen2, list continuation). Engine gate `set_mtp_spec` + per-step `mtp_spec_eval`: prefills maintain the head's KV incrementally (chunked prefill included, seam rows via the `mtp_h` handoff), flat AND paged sessions (the pool carries the draft-block slab), every KV codec; `dasllama-server --mtp` spec-ticks greedy streams. Greedy-only v1; the DeltaNet recurrent state snapshots and rolls back around rejected drafts; `mtp_only` sidecar files and per-head `nextn.embed_tokens`/`shared_head_head` panic honestly |
| Performance | KV cache, SIMD + JobQue-threaded matmul, activation-quant Q8·Q8 behind a pluggable kernel-backend registry (ARM SDOT/laneq + the generated x64 AVX2 maddubs/vpdpbusd tier — `x64_arch.md`), flash-attention batched prefill, per-box kernel tuning (`tune_for_this_box.md`) |

## Optimization guidelines

Non-negotiable rules for anything on a hot path (per-token, per-frame, per-layer — anything
that runs per unit of work rather than per load):

1. **Dynamic memory allocation on a hot path does not exist.** No `resize`/`push`/`reserve`/
   `new`/array literals per call — every buffer lives in a passed 'decoder'-style state
   struct (`RunState`, `ParakeetState`, `EncoderState`, ...) owned by the session, grown
   reserve-exact when a bigger shape arrives, never shrunk, allocated zero times in steady
   state. No module-global scratch — state is passed explicitly. Corollary: reused scratch
   is not zero-initialized like a fresh array — zero explicitly wherever the code relied on
   fresh-allocation zeros, and reset carried state (LSTM h/c) per utterance.
2. **If it's not local, the pointer is brought local.** Inner loops never chase struct
   fields, blob offsets, or array handles — hoist `addr(state.arr[0])` / `addr(m.blob[off])`
   into locals before the loop and index through those (`var p` for writable, the const
   model gives read-only otherwise).
3. **Optimize before threading.** A kernel reaches its best single-lane form first —
   right algorithm, then loop form — and only then gets a `maybe_parallel_for`. Threading
   a bad kernel just burns more cores on the same waste.
4. **Loop forms are decided in isolated benchmarks, not by eye.** For a hot loop, build
   the `[tune]`-marked scalar template AND a hand-float4 version, bench them head-to-head
   standalone (the tune rig / a dedicated microbench, NOT end-to-end transcribe) — commit
   the winner. "To know for sure."
5. **Every `with_job_que()` that runs engine kernels calls `setup_dasllama_jobque()` first**
   (engine spelling `setup_dasllama_jobque_`). Bare `with_job_que` leaves the queue on the
   ~10us/job fifo fork/join — measured 2-7x on the parakeet jfk encoder stages, 2.2x
   end-to-end — and any benchmark taken that way is measuring the wrong dispatch mode.
   Same trap for isolated Q8 GEMM benches: a model load selects the repack backend tier
   (`select_matmul_backend_for_load`), so a bench that skips the load step times the slower
   hand backend unless it selects the tier itself (3x on M1: arm64-gen 900 vs sdot 300 GMAC/s).
6. **Work units, not structural units.** When a stage's natural unit count is near the lane
   count (per-head, per-expert, ...), flatten to finer units — a handful of units on N lanes
   is a built-in makespan floor (whisper tiny: 6 head-units on 8 lanes = 25% idle; turbo 20
   on 16 = ragged second wave). If each unit needs scratch, don't size it per unit —
   dispatch with `maybe_parallel_for_indexed` and index per-slot scratch sized
   `get_dispatch_slot_bound()` (tower attention is the reference consumer).

## Known **not** yet supported

So there's no ambiguity about what will fail:

- **MoE beyond the qwen2moe / gpt-oss / gemma4 shapes** — grouped expert routing (DeepSeek-V3
  `expert_group_count`) and the expert-selection bias (`exp_probs_b`) panic honestly at load
  (Tier-3 archs — MLA, Mamba/hybrid, multimodal — remain explicitly deferred in
  `API_REWORK.md`).
- **GGUF weight types beyond F32 / F16 / Q8_0 / Q4_0 / MXFP4 / Q4_K / Q5_K / Q6_K** — no IQ
  quants; BF16 is read only for the audio-tower mmprojs, not as an LLM weight format.
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

Perf is compared with `llama-bench -ngl 0 -t 16 --cpu-mask 0x55555555 --cpu-strict 1` on the
x86 boxes (16 physical cores, pinned on BOTH sides — das pins via the jobque's hard affinity
mask; unpinned SMT placement halves lockstep prefill). The per-box methodology lives in
`performance/profile_common.das`; `performance/results_llm.md` is the scoreboard.
