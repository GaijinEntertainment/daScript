options gen2

module dasllama_arch_qwen3 shared public

require dasllama/dasllama_common

// Qwen3 — per-head RMSNorm on Q/K before RoPE (QK-norm) + NEOX rope; no QKV bias (unlike Qwen2).
def private configure_qwen3(var c : Config) {
    c.rope_neox = true
    c.qk_norm = true
}

[init]
def register_arch_qwen3() {
    var d = ArchDesc(name = "qwen3", configure = @@configure_qwen3)
    d.blocks = std_blocks()
    d.chat.system.parts <- [chat_special("<|im_start|>"), chat_text("system\n"), chat_content(), chat_special("<|im_end|>"), chat_text("\n")]
    d.chat.user.parts <- [chat_special("<|im_start|>"), chat_text("user\n"), chat_content(), chat_special("<|im_end|>"), chat_text("\n")]
    d.chat.assistant_open.parts <- [chat_special("<|im_start|>"), chat_text("assistant\n")]
    d.chat.stop <- ["<|im_end|>", "<|endoftext|>"]
    register_arch("qwen3", d)
}
