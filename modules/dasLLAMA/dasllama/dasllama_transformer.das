options gen2
options _comment_hygiene = true

module dasllama_transformer shared public

// dasLLAMA public front door. Re-exports the engine (dasllama_common) and pulls in every architecture
// so its [init] registration fires. Consumers `require dasllama/dasllama_transformer` get the full API
// plus all registered arches; adding an architecture = a new dasllama_arch_*.das + one require here.
require dasllama/dasllama_common public
require dasllama/dasllama_arch_llama   // nolint:STYLE030 — side-effect require: fires the arch's [init] registration
require dasllama/dasllama_arch_qwen2   // nolint:STYLE030 — side-effect require: fires the arch's [init] registration
require dasllama/dasllama_arch_qwen3   // nolint:STYLE030 — side-effect require: fires the arch's [init] registration
require dasllama/dasllama_arch_phi3    // nolint:STYLE030 — side-effect require: fires the arch's [init] registration
require dasllama/dasllama_arch_gemma2  // nolint:STYLE030 — side-effect require: fires the arch's [init] registration
require dasllama/dasllama_arch_gemma3  // nolint:STYLE030 — side-effect require: fires the arch's [init] registration
require dasllama/dasllama_arch_gemma4  // nolint:STYLE030 — side-effect require: fires the arch's [init] registration
require dasllama/dasllama_arch_qwen2moe  // nolint:STYLE030 — side-effect require: fires the arch's [init] registration
require dasllama/dasllama_arch_qwen3moe  // nolint:STYLE030 — side-effect require: fires the arch's [init] registration
require dasllama/dasllama_arch_qwen35  // nolint:STYLE030 — side-effect require: fires the arch's [init] registration
require dasllama/dasllama_arch_gptoss  // nolint:STYLE030 — side-effect require: fires the arch's [init] registration
require dasllama/dasllama_arch_mistral3  // nolint:STYLE030 — side-effect require: fires the arch's [init] registration
require dasllama/dasllama_arch_glm4moe  // nolint:STYLE030 — side-effect require: fires the arch's [init] registration
require ?das_metal dasllama/dasllama_metal_prefill  // nolint:STYLE030 — [init]-only: registers prefill override "metal" (the dasMetal GPU-resident path). STRICT guard: Apple builds only; it lives here (not dasllama_common) because it requires the engine back — the umbrella breaks the cycle
require ?das_metal dasllama/dasllama_metal_llama  // nolint:STYLE030 — [init]-only: registers decode override "metal" (same contract as the prefill line above)
