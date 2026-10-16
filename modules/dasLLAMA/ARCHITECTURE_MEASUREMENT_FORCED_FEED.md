# dasLLAMA Architecture - the reference's forced feed

Companion to `ARCHITECTURE_MEASUREMENT.md`; a section is cited by its anchor. This document
carries one section, `ARCHITECTURE_MEASUREMENT_FORCED_FEED.md#reference-forced-feed`: how the
reference build's figures quoted against the gemma forced-feed cells are read, and what they
are. The benchmark rig, the tune gate and the instrumentation rails stay in
`ARCHITECTURE_MEASUREMENT.md`.

### The reference build's forced feed on the E2B Q4_K_M {#reference-forced-feed}

The forced-feed cells (`tests/_gemma_resident.das`'s `gemma_forced_feed`, `prose_feed`) feed a
prompt of 40 prose ids - the paragraph `prose_tokens` repeats, BOS first - then 8 more ids of the
same text one step each, and read the logits row of the prompt's last position and of every
step. The reference figures beside them come from the same 48 ids fed through the reference
build's C API on the M5 Max, in a scratch driver kept outside the tree - its readings are a
probe's, never parity evidence (`REVIEW_GPU_PARITY.md`):

- Build: the `~/Work/llama.cpp` checkout at `98c4764b67` (branch `dasprof-instrumented`, its
  `build/bin` libraries), the driver compiled against it with `clang++ -std=c++17 -O2
  -I<llama.cpp>/include -I<llama.cpp>/ggml/include <driver>.cpp -L<llama.cpp>/build/bin -lllama
  -lggml -lggml-base`.
- Model: `gemma-4-E2B-it-Q4_K_M.gguf` from the default models dir.
- The driver: `ggml_backend_load_all()`, `n_gpu_layers = 0` with the CPU device alone (the
  stock device set and `op_offload` on under `LCPP_ALLDEV=1`), `n_ctx 1024`, `n_batch 512`,
  `n_threads 8`, then either the 40 ids in one `llama_decode` and each later id in its own, or
  (`onebatch`) all 48 in one batch with logits on positions 39 to 47; arguments `MODEL IDS NP OUT
  UBATCH FA` - `NP 40`, `n_ubatch` 512 or 16, `flash_attn_type` -1 (auto) or 0 (off); every row's
  logits written as raw f32.
- The compare: each run's rows against the file's f32 chain (this engine at `QuantMode.fp32`,
  `f32_chain_rows`), the top logit at step 4.

Step 4's top logit, where the f32 chain reads 19.14: 17.92 at `n_ubatch` 512 or 16 with flash
attention auto, 20.09 at 16 with it off, 18.67 and 17.83 in one batch (auto, off), 19.32 and
18.43 on the stock device set (stepped, one batch) - 17.8 to 20.1 by attention form and batching.
The same driver over the file requantized to F32 (`llama-quantize --allow-requantize ... F32`)
reads 19.11 and 19.13, the f32 chain's within 0.03: the band is the quantized activations', not
the weights'. This engine's CPU q8 chain reads 17.85 on the M5.
