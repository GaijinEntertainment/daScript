options gen2

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
require dasllama/dasllama_arch_gptoss  // nolint:STYLE030 — side-effect require: fires the arch's [init] registration
