options gen2
options _comment_hygiene = true
options _dasllama_internal = true

module dasllama_arch_gemma3 shared public

require dasllama/dasllama_lint public
require dasllama/dasllama_common
require dasllama/dasllama_blocks   // std_blocks/moe_blocks — the default block sets this arch binds

// Gemma-3 — Gemma-2's shape (GeGLU, 4 norms/layer, NEOX rope, √dim embed scale) minus the soft-caps,
// plus Qwen3-style per-head QK-norm and the 5-local:1-global attention pattern with dual RoPE:
// sliding layers rope at θ=10k unscaled, global layers at θ=1M (linear-8 position scale on the 4B).
// Window span / linear factor come from GGUF metadata (1B: 512, no scaling; 4B: 1024, factor 8).
def private configure_gemma3(var c : Config) {
    c.rope_neox = true                  // NEOX rope, full-rotary over head_size
    c.ffn_act = FfnAct.gelu             // GeGLU (tanh-approx gelu) instead of SwiGLU
    c.sliding_window = 512l             // local attention span (GGUF sliding_window overrides)
    c.swa_pattern = 6l                  // every 6th layer global, the rest sliding
    c.rope_freq_base_swa = 10000.0      // sliding layers' θ (global layers read rope.freq_base = 1M)
    c.pre_post_norm = true              // extra post-attn + post-FFN norms (4 norms/layer)
    c.qk_norm = true                    // per-head RMSNorm on Q/K before RoPE
    c.embed_scale_sqrt_dim = true       // scale token embeddings by sqrt(dim)
}

[init]
def register_arch_gemma3() {
    var d = ArchDesc(name = "gemma3", configure = @@configure_gemma3)
    d.blocks = std_blocks()
    gemma_chat(d.chat)
    d.lcaps.system_prompt = false   // no system role — the chat layer folds it into the first user turn
    register_arch("gemma3", d)
}
