options gen2
options _comment_hygiene = true

module dasllama_arch_gemma2 shared public

require dasllama/dasllama_common

// Gemma-2 — GeGLU, dual logit soft-cap, sliding-window on even layers, 4 norms/layer, NEOX rope.
// embed_scale (√dim), head_size (key_length) and the (1+w) norm weights are resolved in the loader
// (dim isn't known at configure time, and Gemma's GGUF pre-bakes the +1).
def private configure_gemma2(var c : Config) {
    c.rope_neox = true                  // NEOX rope, full-rotary over head_size
    c.ffn_act = FfnAct.gelu             // GeGLU (tanh-approx gelu) instead of SwiGLU
    c.attn_logit_softcap = 50.0         // soft-cap attention scores before softmax
    c.final_logit_softcap = 30.0        // soft-cap the classifier logits
    c.sliding_window = 4096l            // local attention on even layers (swa_pattern default 2)
    c.pre_post_norm = true              // extra post-attn + post-FFN norms (4 norms/layer)
    c.embed_scale_sqrt_dim = true       // scale token embeddings by sqrt(dim)
}

[init]
def register_arch_gemma2() {
    var d = ArchDesc(name = "gemma2", configure = @@configure_gemma2)
    d.blocks = std_blocks()
    d.chat.add_bos = true   // BOS = <bos>; Gemma has no system role — fold it into the first user turn
    d.chat.user.parts <- [chat_special("<start_of_turn>"), chat_text("user\n"), chat_content(), chat_special("<end_of_turn>"), chat_text("\n")]
    d.chat.assistant_open.parts <- [chat_special("<start_of_turn>"), chat_text("model\n")]
    d.chat.stop <- ["<end_of_turn>", "<eos>"]
    d.lcaps.system_prompt = false   // no system role — the chat layer folds it into the first user turn
    register_arch("gemma2", d)
}
