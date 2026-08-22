CPU large-language-model inference in pure daslang: load a GGUF model, tokenize, run the
transformer, sample — or hold a full chat — validated token-for-token against llama.cpp on every
supported family. Run with ``-jit``; ``examples/dasLLAMA/run.das`` and ``chat.das`` show the
canonical program shape.

Supported model families (GGUF — fp32 / f16 / q8_0 / q4_0 / mxfp4 weights read directly;
K-quant files such as Q4_K_M / Q5_K_M / Q6_K run on native K-quant kernels):

* **Llama** — Llama-2 / TinyLlama, Llama-3.1 / 3.2, Mistral-7B-Instruct, SmolLM2, plus llama2.c ``.bin`` checkpoints
* **Qwen** — Qwen2.5, Qwen3 (QK-norm), Qwen3.5 / Qwen3.6 (hybrid Gated-DeltaNet attention, incl. the 35B-A3B MoE); MoE: Qwen1.5-MoE (routed + sigmoid-gated shared expert), Qwen3-30B-A3B (routed-only, renormalized top-k); vision + omni: Qwen3-Omni-30B and dense Qwen3-VL (M-RoPE image positions, deepstack), Qwen2.5-Omni / VL (window-attention ViT) — images and audio in chat
* **Phi** — Phi-3.5-mini
* **Gemma** — Gemma-2, Gemma-3 (per-layer sliding-window patterns, vision via the SigLIP mmproj), Gemma-4 (12B / 31B dense, the 26B-A4B MoE, and the E2B / E4B edge series with per-layer embeddings + cross-layer KV sharing; vision and audio in chat via the family mmprojs)
* **gpt-oss** — gpt-oss-20b (attention sinks, native MXFP4 experts, YaRN long context, Harmony chat format)

The architecture is picked from GGUF metadata at load — the same program runs any of these.

Hands-on tutorials (:ref:`overview <tutorials_dasllama>`):
:ref:`the problem statement <tutorial_dasLLAMA_problem_statement>`,
:ref:`hello, generation <tutorial_dasLLAMA_hello_generate>`,
:ref:`chat and templates <tutorial_dasLLAMA_chat>`,
:ref:`sampling <tutorial_dasLLAMA_sampling>`,
:ref:`sessions and memory <tutorial_dasLLAMA_sessions_and_memory>`,
:ref:`performance <tutorial_dasLLAMA_performance>`,
:ref:`the architecture registry <tutorial_dasLLAMA_add_an_arch>`.
