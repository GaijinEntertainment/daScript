options gen2
options _comment_hygiene = true

module dasllama_transformer shared public

// dasLLAMA public front door. Re-exports the engine (dasllama_common) and pulls in every architecture
// so its [init] registration fires. Consumers `require dasllama/dasllama_transformer` get the full API
// plus all registered arches; adding an architecture = a new dasllama_arch_*.das + one require here.
require dasllama/dasllama_common public
require dasllama/dasllama_layout   // [init]-only: registers the disk->compute layout hooks (repacks + GPU gathers) into dasllama_common; unset hooks panic at load
require dasllama/dasllama_moe public   // the MoE router/expert dispatch the block kernels reach through
require dasllama/dasllama_blocks public   // the transformer block kernels + the default block sets
require dasllama/dasllama_batch public   // eval_batch_ + BatchWorkspace — the B-session decode step
require dasllama/dasllama_attn_prefill public   // prefill_attention — the head-threaded prefill attention
require dasllama/dasllama_sampling public   // generate_/sample_/set_seed_/stats_ — re-exported so the umbrella keeps the generation API
require dasllama/dasllama_ple  // [init]-only: registers the gemma-4 PLE pre-step hooks into dasllama_common; it requires the engine back, so the umbrella breaks the cycle
require dasllama/dasllama_gpu_resident  // [init]-only: registers the whole-model GPU residency rail (overrides "vulkan" + the upload hook). Like the metal drivers below it requires the engine back, so the umbrella breaks the cycle; no device module, so it compiles everywhere
require dasllama/dasllama_load public   // the GGUF load walk: load_model_/load_gguf* — re-exported so every consumer of this umbrella keeps the load API
require dasllama/dasllama_arch_llama   // side-effect require: fires the arch's [init] registration
require dasllama/dasllama_arch_qwen2   // side-effect require: fires the arch's [init] registration
require dasllama/dasllama_arch_qwen3   // side-effect require: fires the arch's [init] registration
require dasllama/dasllama_arch_phi3    // side-effect require: fires the arch's [init] registration
require dasllama/dasllama_arch_gemma2  // side-effect require: fires the arch's [init] registration
require dasllama/dasllama_arch_gemma3  // side-effect require: fires the arch's [init] registration
require dasllama/dasllama_arch_gemma4  // side-effect require: fires the arch's [init] registration
require dasllama/dasllama_arch_qwen2moe  // side-effect require: fires the arch's [init] registration
require dasllama/dasllama_arch_qwen3moe  // side-effect require: fires the arch's [init] registration
require dasllama/dasllama_arch_qwen35  // side-effect require: fires the arch's [init] registration
require dasllama/dasllama_arch_gptoss  // side-effect require: fires the arch's [init] registration
require dasllama/dasllama_arch_mistral3  // side-effect require: fires the arch's [init] registration
require dasllama/dasllama_arch_glm4moe  // side-effect require: fires the arch's [init] registration
require dasllama/dasllama_metal_shapes  // [init]-only: registers the PORTABLE metal servable predicate (pure model shape) — every build, so a non-Apple converter can bake the metal flavor
require ?das_metal dasllama/dasllama_metal_prefill  // [init]-only: registers prefill override "metal" (the dasMetal GPU-resident path). STRICT guard: Apple builds only; it lives here (not dasllama_common) because it requires the engine back — the umbrella breaks the cycle
require ?das_metal dasllama/dasllama_metal_decode  // [init]-only: registers decode override "metal" (same contract as the prefill line above)
require ?das_metal dasllama/dasllama_metal_tower  // [init]-only: registers the gemma4uv GPU tower encode (same contract as the prefill line above)
require ?vulkan dasllama/dasllama_math_vulkan  // [init]-only: registers "vulkan" (the GPU tier). STRICT guard: the C++ vulkan module is linked only where dasVulkan is installed. LAST on purpose: the vulkan drivers are the hot-edit modules, and require order is the jit obj-cache layout — a late position keeps the fat module prefix cached across driver edits
