options gen2
options _comment_hygiene = true

module dasllama_arch_qwen2moe shared public

require dasllama/dasllama_common

// Qwen1.5/Qwen2-MoE — Qwen2 attention (QKV bias + NEOX rope) with the routed-experts FFN:
// softmax router -> top-k with un-renormalized weights, plus a sigmoid-gated shared expert.
// The expert geometry (60/4/1408/5632 on the A2.7B) comes from GGUF metadata at load.
def private configure_qwen2moe(var c : Config) {
    c.attn_qkv_bias = true
    c.rope_neox = true
    c.moe = true
}

[init]
def register_arch_qwen2moe() {
    var d = ArchDesc(name = "qwen2moe", configure = @@configure_qwen2moe)
    d.blocks = moe_blocks()
    d.chat.system.parts <- [chat_special("<|im_start|>"), chat_text("system\n"), chat_content(), chat_special("<|im_end|>"), chat_text("\n")]
    d.chat.user.parts <- [chat_special("<|im_start|>"), chat_text("user\n"), chat_content(), chat_special("<|im_end|>"), chat_text("\n")]
    d.chat.assistant_open.parts <- [chat_special("<|im_start|>"), chat_text("assistant\n")]
    d.chat.think_suppress.parts <- [chat_special("<think>"), chat_text("\n\n"), chat_special("</think>"), chat_text("\n\n")]
    d.chat.stop <- ["<|im_end|>", "<|endoftext|>"]
    register_arch("qwen2moe", d)
}
