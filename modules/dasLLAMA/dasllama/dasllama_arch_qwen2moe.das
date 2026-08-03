options gen2
options _comment_hygiene = true

module dasllama_arch_qwen2moe shared public

require dasllama/dasllama_common
require dasllama/dasllama_blocks   // std_blocks/moe_blocks — the default block sets this arch binds

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
    chatml_chat(d.chat)   // no hermes_tools: the A2.7B-era chat template predates the tool block
    register_arch("qwen2moe", d)
}
