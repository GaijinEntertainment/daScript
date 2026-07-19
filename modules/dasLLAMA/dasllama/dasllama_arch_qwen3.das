options gen2
options _comment_hygiene = true

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
    d.chat.think_suppress.parts <- [chat_special("<think>"), chat_text("\n\n"), chat_special("</think>"), chat_text("\n\n")]
    d.chat.stop <- ["<|im_end|>", "<|endoftext|>"]
    hermes_tools(d.chat)
    // Qwen3-ASR's decoder converts as `qwen3vl` — M-RoPE + deepstack; for text/audio input every
    // M-RoPE coord equals linear position, so it's bit-identical to plain qwen3 forward here.
    // (Clone the template BEFORE register_arch moves `d`.)
    var dv = ArchDesc(name = "qwen3vl", configure = @@configure_qwen3)
    dv.blocks = std_blocks()
    dv.chat := d.chat
    dv.chat.audio_pre.parts <- [chat_special("<|audio_start|>")]
    dv.chat.audio_post.parts <- [chat_special("<|audio_end|>")]
    register_arch("qwen3", d)
    register_arch("qwen3vl", dv)
}
