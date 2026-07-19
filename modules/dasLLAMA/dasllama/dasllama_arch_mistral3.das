options gen2
options _comment_hygiene = true

module dasllama_arch_mistral3 shared public

require dasllama/dasllama_common

// Mistral Medium 3.5 (llama.cpp arch "mistral3") — the llama dense shape (NORM rope, SiLU FFN,
// no QK-norm) with YaRN rope (factor 64 over a 4096 original context, beta_fast 4) and the
// v7-tekken chat format ([SYSTEM_PROMPT] system role, unspaced [INST], EOS-closed assistant
// turns). The optional llama4-style attention temperature scaling (attention.temperature_scale)
// is fail-closed in the loader; this model ships it as 0 (disabled).
[unused_argument(c)]
def private configure_mistral3(var c : Config) {
    pass
}

[init]
def register_arch_mistral3() {
    var d = ArchDesc(name = "mistral3", configure = @@configure_mistral3)
    d.blocks = std_blocks()
    d.chat.add_bos = true   // BOS = <s>
    d.chat.system.parts <- [chat_special("[SYSTEM_PROMPT]"), chat_content(), chat_special("[/SYSTEM_PROMPT]")]
    d.chat.user.parts <- [chat_special("[INST]"), chat_content(), chat_special("[/INST]")]
    d.chat.assistant_close.parts <- [chat_special("</s>")]
    d.chat.stop <- ["</s>"]
    register_arch("mistral3", d)
}
