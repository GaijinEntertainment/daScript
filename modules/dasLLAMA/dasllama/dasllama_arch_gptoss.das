options gen2
options _comment_hygiene = true

module dasllama_arch_gptoss shared public

require dasllama/dasllama_common

// gpt-oss (OpenAI, llama.cpp arch "gpt-oss" / openai-moe) — a MoE with attention extras: every FFN
// is routed top-k (32 experts, top-4 on the 20B, softmax over the SELECTED weights, no shared
// expert), clamped SwiGLU (+1 up branch), per-head attention-sink logits, alternating sliding
// (even) / global (odd) layers, biases on QKV + the output projection + the router + every expert,
// and YaRN rope (factor 32 over a 4096 original context). The biases/sinks are tensor-detected by
// the loader; the pre-FFN norm ships under the post_attention_norm name.
def private configure_gptoss(var c : Config) {
    c.attn_qkv_bias = true
    c.rope_neox = true
    c.moe = true
    c.moe_gate = MoeGate.softmax_weight  // llama.cpp hardcodes SOFTMAX_WEIGHT for this arch (no GGUF key)
    c.ffn_act = FfnAct.swiglu_oai
    c.sliding_window = 128l              // opt-in + the 20B's span (GGUF sliding_window overrides)
    c.swa_pattern = 2l                   // default pattern: even layers slide, odd are global
    c.post_attn_is_ffn_norm = true
}

[init]
def register_arch_gptoss() {
    var d = ArchDesc(name = "gpt-oss", configure = @@configure_gptoss)
    d.blocks = moe_blocks()
    // Harmony-lite (the llama.cpp OPENAI_MOE template): the model itself opens `<|channel|>analysis`
    // etc. after the bare assistant header; replies end at <|return|> (or <|call|> for tool calls)
    d.chat.system.parts <- [chat_special("<|start|>"), chat_text("system"), chat_special("<|message|>"), chat_content(), chat_special("<|end|>")]
    d.chat.user.parts <- [chat_special("<|start|>"), chat_text("user"), chat_special("<|message|>"), chat_content(), chat_special("<|end|>")]
    d.chat.assistant_open.parts <- [chat_special("<|start|>"), chat_text("assistant")]
    d.chat.stop <- ["<|return|>", "<|call|>"]
    register_arch("gpt-oss", d)
}
