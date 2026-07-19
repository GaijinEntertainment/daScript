options gen2
options _comment_hygiene = true

module dasllama_arch_llama shared public

require dasllama/dasllama_common

// Llama / Llama-2 / Llama-3 / TinyLlama. Every Config default already encodes this family, so the
// config setter is empty. The chat template is the Llama-3 instruct DEFAULT: GGUF general.architecture
// is "llama" for BOTH Llama-3 and Llama-2/TinyLlama (different formats), and the chat engine
// disambiguates by tokenizer backend — BPE ⇒ Llama-3, SPM ⇒ TinyLlama/Zephyr.
[unused_argument(c)]
def private configure_llama(var c : Config) {
    pass
}

[init]
def register_arch_llama() {
    var d = ArchDesc(name = "llama", configure = @@configure_llama)
    d.blocks = std_blocks()
    d.chat.add_bos = true   // BOS = <|begin_of_text|>
    d.chat.system.parts <- [chat_special("<|start_header_id|>"), chat_text("system"), chat_special("<|end_header_id|>"),
                            chat_text("\n\n"), chat_content(), chat_special("<|eot_id|>")]
    d.chat.user.parts <- [chat_special("<|start_header_id|>"), chat_text("user"), chat_special("<|end_header_id|>"),
                          chat_text("\n\n"), chat_content(), chat_special("<|eot_id|>")]
    d.chat.assistant_open.parts <- [chat_special("<|start_header_id|>"), chat_text("assistant"),
                                    chat_special("<|end_header_id|>"), chat_text("\n\n")]
    d.chat.stop <- ["<|eot_id|>", "<|end_of_text|>"]
    register_arch("llama", d)
}
