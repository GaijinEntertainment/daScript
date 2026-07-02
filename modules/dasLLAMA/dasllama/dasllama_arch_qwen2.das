options gen2

module dasllama_arch_qwen2 shared public

require dasllama/dasllama_common

// Qwen2 — learned QKV bias + NEOX rope; ChatML template (<|im_start|> / <|im_end|>).
def private configure_qwen2(var c : Config) {
    c.attn_qkv_bias = true   // Qwen2 adds a learned bias on the Q/K/V projections
    c.rope_neox = true       // ...and uses NEOX-style rope
}

[init]
def register_arch_qwen2() {
    var d = ArchDesc(name = "qwen2", configure = @@configure_qwen2)
    d.blocks = std_blocks()
    d.chat.system.parts <- [chat_special("<|im_start|>"), chat_text("system\n"), chat_content(), chat_special("<|im_end|>"), chat_text("\n")]
    d.chat.user.parts <- [chat_special("<|im_start|>"), chat_text("user\n"), chat_content(), chat_special("<|im_end|>"), chat_text("\n")]
    d.chat.assistant_open.parts <- [chat_special("<|im_start|>"), chat_text("assistant\n")]
    d.chat.stop <- ["<|im_end|>", "<|endoftext|>"]
    register_arch("qwen2", d)
}
