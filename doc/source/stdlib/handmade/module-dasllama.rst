CPU large-language-model inference in pure daslang: load a GGUF model, tokenize, run the
transformer, sample — or hold a full chat — validated token-for-token against llama.cpp on every
supported family. Run with ``-jit``; ``examples/dasLLAMA/run.das`` and ``chat.das`` show the
canonical program shape.

Supported model families (GGUF, fp32 / q8 / q4 weights):

* **Llama** — Llama-2 / TinyLlama, Llama-3.1 / 3.2, Mistral-7B-Instruct, SmolLM2, plus llama2.c ``.bin`` checkpoints
* **Qwen** — Qwen2.5, Qwen3 (QK-norm), Qwen1.5-MoE (shared + routed experts)
* **Phi** — Phi-3.5-mini
* **Gemma** — Gemma-2, Gemma-3 (per-layer sliding-window patterns), Gemma-4-12B (p-RoPE, heterogeneous attention geometry)
* **gpt-oss** — gpt-oss-20b (attention sinks, MXFP4 weights, YaRN long context, Harmony chat format)

The architecture is picked from GGUF metadata at load — the same program runs any of these.

Hands-on tutorials (:ref:`overview <tutorials_dasllama>`):
:ref:`hello, generation <tutorial_dasLLAMA_hello_generate>`,
:ref:`chat and templates <tutorial_dasLLAMA_chat>`,
:ref:`sampling <tutorial_dasLLAMA_sampling>`,
:ref:`sessions and memory <tutorial_dasLLAMA_sessions_and_memory>`,
:ref:`performance <tutorial_dasLLAMA_performance>`,
:ref:`the architecture registry <tutorial_dasLLAMA_add_an_arch>`.
