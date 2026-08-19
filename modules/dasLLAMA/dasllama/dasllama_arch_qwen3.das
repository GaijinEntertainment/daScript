options gen2
options _comment_hygiene = true
options _dasllama_internal = true

module dasllama_arch_qwen3 shared public

require dasllama/dasllama_lint public
require dasllama/dasllama_common
require dasllama/dasllama_blocks   // std_blocks/moe_blocks — the default block sets this arch binds

// Qwen3 — per-head RMSNorm on Q/K before RoPE (QK-norm) + NEOX rope; no QKV bias (unlike Qwen2).
def private configure_qwen3(var c : Config) {
    c.rope_neox = true
    c.qk_norm = true
}

[init]
def register_arch_qwen3() {
    var d = ArchDesc(name = "qwen3", configure = @@configure_qwen3)
    d.blocks = std_blocks()
    chatml_chat(d.chat)
    hermes_tools(d.chat)
    // Qwen3-ASR's decoder converts as `qwen3vl` — M-RoPE + deepstack; for text/audio input every
    // M-RoPE coord equals linear position, so it's bit-identical to plain qwen3 forward here.
    // (Clone the template BEFORE register_arch moves `d`.)
    var dv = ArchDesc(name = "qwen3vl", configure = @@configure_qwen3)
    dv.blocks = std_blocks()
    dv.chat := d.chat
    // audio markers on `dv` ONLY: qwen3-family audio models convert as vl; base-arch audio
    // is the qwen2 family's shape (qwen2 sets its markers before the clone)
    dv.chat.audio_pre.parts <- [chat_special("<|audio_start|>")]
    dv.chat.audio_post.parts <- [chat_special("<|audio_end|>")]
    register_arch("qwen3", d)
    register_arch("qwen3vl", dv)
}
