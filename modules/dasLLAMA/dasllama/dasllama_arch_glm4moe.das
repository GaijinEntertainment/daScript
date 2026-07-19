options gen2
options _comment_hygiene = true

module dasllama_arch_glm4moe shared public

require dasllama/dasllama_common

// GLM-4.5 / GLM-4.5-Air (llama.cpp arch "glm4moe") — leading dense layer(s) then routed MoE with
// sigmoid gating + the DeepSeek-style exp_probs_b selection bias and an UNGATED shared expert
// (all loader-detected). Attention: QKV bias, NEOX partial rotary (rope.dimension_count 64 of
// head 128), pre-FFN norm named post_attention_norm. The trailing MTP/NextN draft block
// (nextn_predict_layers = 1) rides the existing n_layer_nextn machinery. Chat: [gMASK]<sop>
// prelude, role-marker turns, thinking on by default (the model opens with <think> itself).
def private configure_glm4moe(var c : Config) {
    c.moe = true
    c.moe_gate = MoeGate.sigmoid   // llama.cpp defaults glm4moe to SIGMOID when the key is absent (Air ships key = 2)
    c.attn_qkv_bias = true
    c.rope_neox = true
    c.partial_rope = true
    c.post_attn_is_ffn_norm = true
}

[init]
def register_arch_glm4moe() {
    var d = ArchDesc(name = "glm4moe", configure = @@configure_glm4moe)
    d.blocks = moe_blocks()
    d.chat.prelude.parts <- [chat_special("[gMASK]"), chat_special("<sop>")]
    d.chat.system.parts <- [chat_special("<|system|>"), chat_text("\n"), chat_content()]
    d.chat.user.parts <- [chat_special("<|user|>"), chat_text("\n"), chat_content()]
    d.chat.assistant_open.parts <- [chat_special("<|assistant|>")]
    // the official Jinja also appends /nothink to the last user turn; the empty-block prefill
    // alone closes the think channel, so das renders just that half (same as the qwen3 family)
    d.chat.think_suppress.parts <- [chat_text("\n"), chat_special("<think>"), chat_special("</think>")]
    d.chat.stop <- ["<|user|>", "<|observation|>", "<|endoftext|>"]
    register_arch("glm4moe", d)
}
