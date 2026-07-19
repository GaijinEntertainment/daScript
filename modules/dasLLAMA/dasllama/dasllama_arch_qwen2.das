options gen2
options _comment_hygiene = true

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
    // ChatML template detection maps every <|im_start|> GGUF here, thinking fine-tunes (Qwen3,
    // Qwen3.6) included — the suppress renders only when the vocab carries the think specials
    d.chat.think_suppress.parts <- [chat_special("<think>"), chat_text("\n\n"), chat_special("</think>"), chat_text("\n\n")]
    d.chat.stop <- ["<|im_end|>", "<|endoftext|>"]
    hermes_tools(d.chat)
    // audio-capable qwen2 family (Qwen2-Audio, Qwen2.5-Omni) wraps the soft-token span in
    // audio markers; renders only on audio turns, so text-only qwen2 vocabs never resolve these
    d.chat.audio_pre.parts <- [chat_special("<|audio_bos|>")]
    d.chat.audio_post.parts <- [chat_special("<|audio_eos|>")]
    // Qwen2.5-Omni's decoder converts as `qwen2vl` — M-RoPE with rope.dimension_sections; for
    // text/audio input every M-RoPE coord equals linear position, so it's bit-identical to plain
    // qwen2 forward (vision is out of scope). (Clone the template BEFORE register_arch moves `d`.)
    var dv = ArchDesc(name = "qwen2vl", configure = @@configure_qwen2)
    dv.blocks = std_blocks()
    dv.chat := d.chat
    register_arch("qwen2", d)
    register_arch("qwen2vl", dv)
}
