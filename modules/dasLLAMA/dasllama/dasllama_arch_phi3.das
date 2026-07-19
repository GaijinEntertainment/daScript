options gen2
options _comment_hygiene = true

module dasllama_arch_phi3 shared public

require dasllama/dasllama_common

// Phi-3 — NEOX rope + LongRoPE, fused QKV + gate_up (split at load); <|user|> / <|assistant|> template.
def private configure_phi3(var c : Config) {
    c.rope_neox = true       // Phi3: NEOX rope + LongRoPE (freq factors + mscale, read in the loader)
    c.fused_qkv = true       // ...with fused attn_qkv + ffn_up (gate_up) tensors, split at load
}

[init]
def register_arch_phi3() {
    var d = ArchDesc(name = "phi3", configure = @@configure_phi3)
    d.blocks = std_blocks()
    d.chat.system.parts <- [chat_special("<|system|>"), chat_text("\n"), chat_content(), chat_special("<|end|>"), chat_text("\n")]
    d.chat.user.parts <- [chat_special("<|user|>"), chat_text("\n"), chat_content(), chat_special("<|end|>"), chat_text("\n")]
    d.chat.assistant_open.parts <- [chat_special("<|assistant|>"), chat_text("\n")]
    d.chat.stop <- ["<|end|>", "<|endoftext|>"]
    register_arch("phi3", d)
}
