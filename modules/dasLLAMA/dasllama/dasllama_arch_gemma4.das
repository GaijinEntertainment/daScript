options gen2
options _comment_hygiene = true
options _dasllama_internal = true

module dasllama_arch_gemma4 shared public

require dasllama/dasllama_lint public
require dasllama/dasllama_common
require dasllama/dasllama_blocks   // attention_std_decode/prefill — gemma4 binds the standard attention with its own FFN
require dasllama/dasllama_ple  // the PLE + gemma4 MoE kernels this arch binds into ArchBlocks

// Gemma-4 (12B) — Gemma-3's shape (GeGLU, 4 norms/layer, NEOX rope, √dim embed scale, QK-norm,
// per-layer SWA pattern with dual RoPE θ) plus the heterogeneous attention geometry: sliding layers
// run 16 query / 8 KV heads of 256, global layers 16 query / 1 KV head of 512 — with proportional
// p-RoPE factors (rope_freqs) on global layers only, a weightless RMSNorm on V, no attn_v tensor on
// global layers (V reuses the pre-norm K projection), unit attention scale (QK-norm covers it), a
// per-layer output scalar, and a final logit soft-cap. All of those except this config resolve from
// the GGUF (pattern bool-array, per-layer kv-head classes, tensor presence, softcap, suppress ids).
def private configure_gemma4(var c : Config) {
    c.rope_neox = true                  // NEOX rope, full-rotary over each class's head size
    c.ffn_act = FfnAct.gelu             // GeGLU (tanh-approx gelu) instead of SwiGLU
    c.sliding_window = 1024l            // local attention span (GGUF sliding_window overrides)
    c.swa_pattern = 6l                  // fallback pattern; gemma4 GGUFs carry the per-layer bool array
    c.rope_freq_base_swa = 10000.0      // sliding layers' θ (global layers read rope.freq_base = 1M)
    c.pre_post_norm = true              // extra post-attn + post-FFN norms (4 norms/layer)
    c.qk_norm = true                    // per-head RMSNorm on Q/K before RoPE
    c.embed_scale_sqrt_dim = true       // scale token embeddings by sqrt(dim)
    c.attn_scale = 1.0                  // no 1/sqrt(head_size) score scale — QK-norm replaces it
    c.v_norm = true                     // weightless per-head RMSNorm on V after projection
    c.rope_ff_global_only = true        // p-RoPE: rope_freqs factors apply to global layers only
    c.moe_optional = true               // 26B-A4B is MoE; 12B/31B are dense — the loader decides by file
    c.norm_topk_prob = true             // gemma4 renormalizes the selected top-k weights (no GGUF key; build_moe_ffn norm_w=true)
}

[init]
def register_arch_gemma4() {
    var d = ArchDesc(name = "gemma4", configure = @@configure_gemma4)
    d.blocks = gemma4_blocks()   // dense (12B/31B) or parallel dense-shared + routed MoE (26B-A4B) by expert count
    // channel-based turn format (replaces gemma's <start_of_turn>): the assistant opener closes an
    // empty `thought` channel — the template's non-thinking default — so the model answers directly
    d.chat.add_bos = true   // BOS = <bos> (id 2)
    d.chat.system.parts <- [chat_special("<|turn>"), chat_text("system\n"), chat_content(), chat_special("<turn|>"), chat_text("\n")]
    d.chat.user.parts <- [chat_special("<|turn>"), chat_text("user\n"), chat_content(), chat_special("<turn|>"), chat_text("\n")]
    d.chat.assistant_open.parts <- [chat_special("<|turn>"), chat_text("model\n"), chat_special("<|channel>"), chat_text("thought\n"), chat_special("<channel|>")]
    d.chat.think_default = true
    d.chat.think_gate.parts <- [chat_special("<|think|>")]
    d.chat.assistant_open_think.parts <- [chat_special("<|turn>"), chat_text("model\n")]
    // gemma4uv vision: the image soft-token span markers, media-first before the user text
    d.chat.image_pre.parts <- [chat_special("<|image>")]
    d.chat.image_post.parts <- [chat_special("<image|>")]
    // gemma4a audio: same media-first span shape (the E-series mmproj carries both towers)
    d.chat.audio_pre.parts <- [chat_special("<|audio>")]
    d.chat.audio_post.parts <- [chat_special("<audio|>")]
    d.chat.tool_mode = ToolMode.gemma4   // <|tool>declaration DSL defs, <|tool_call> DSL calls
    d.chat.think_mode = ThinkMode.asymmetric
    d.chat.think_open = "<|channel>thought"
    d.chat.think_close = "<channel|>"
    d.chat.stop <- ["<turn|>", "<eos>"]
    // instruct mode closed the thought channel already — a channel marker after the answer is the
    // model rambling past its turn (observed: "…4.<channel|>4"); cut generation there
    d.chat.stop_nothink <- ["<|channel>", "<channel|>"]
    d.lcaps.system_prompt = false   // no system role — the chat layer folds it into the first user turn
    register_arch("gemma4", d)
}

//! gemma4's block set: standard attention + a FFN that dispatches per file — dense (12B/31B) or the
//! parallel dense-shared + routed MoE (26B-A4B) by expert count, so ONE gemma4 registration covers both.
def gemma4_blocks() : ArchBlocks {
    return ArchBlocks(attn_decode = @@attention_std_decode, ffn_decode = @@ffn_gemma4_decode,
                      attn_prefill = @@attention_std_prefill, ffn_prefill = @@ffn_gemma4_prefill,
                      ffn_is_dense = false)
}
