options gen2

require dasllama/dasllama   // nolint:STYLE029 — the facade require also fires every arch [init]; Section 1 needs the registry populated
require daslib/strings_boost

// Tutorial dasLLAMA-06: The Architecture Registry
//
// This tutorial covers:
//   - How load_model picks an architecture: GGUF metadata -> the arch registry
//   - The anatomy of an ArchDesc: configure, blocks, chat template
//   - Registering an architecture of your own
//   - What adding a real architecture to the module takes
//
// Prerequisites: tutorials 01-02. No model file (and no -jit) needed — this one
// is about the registry, not inference.
//
// Run: daslang.exe tutorials/dasLLAMA/06_add_an_arch.das

// The configure hook is the Tier-1 dial box: it sets Config flags — rope style, extra
// norms, sliding-window patterns, MoE shape — before the loader reads any tensors.
// (It runs before dim/vocab are known; the loader finalizes dim-dependent fields after.)
def private configure_my_llama(var c : Config) {
    c.rope_neox = true   // NEOX-style rotary embedding, say
}

[export]
def main() {
    // ──────────────────────────────────────────────────────────────────────────
    // Section 1 — The registry
    // ──────────────────────────────────────────────────────────────────────────
    //
    // Requiring dasllama/dasllama registers every supported architecture (each
    // dasllama_arch_*.das self-registers at [init]). load_model reads the GGUF's
    // general.architecture string and looks it up here — an unknown arch panics
    // with this exact list, better than feeding weights through the wrong path.

    let names <- arch_names()
    print("{length(names)} registered architectures: {join(names, ", ")}\n\n")

    // ──────────────────────────────────────────────────────────────────────────
    // Section 2 — Anatomy of an ArchDesc
    // ──────────────────────────────────────────────────────────────────────────
    //
    // An architecture is data, not a fork of the engine:
    //   configure — a function setting Config flags (see configure_my_llama above;
    //               the real qwen3 sets rope_neox + qk_norm and nothing else)
    //   blocks    — the forward-pass kernels. std_blocks() is the standard
    //               attention + FFN pair; MoE (qwen2moe) and gpt-oss swap in their own
    //   chat      — the chat template as data: per-turn part lists + stop tokens
    //
    // Most new model families are just Config flags over std_blocks — that's the
    // Tier-1/Tier-2 design the registry exists for.

    let qwen3_tmpl <- arch_chat("qwen3")
    print("qwen3 chat stop tokens: {qwen3_tmpl.stop}\n\n")

    // ──────────────────────────────────────────────────────────────────────────
    // Section 3 — Register your own
    // ──────────────────────────────────────────────────────────────────────────
    //
    // Suppose a llama-shaped model ships with an architecture string we don't know.
    // Registering a descriptor that maps it onto the existing machinery takes no
    // engine change at all — this is everything:

    let before = has_arch("my_llama")
    var d = ArchDesc(name = "my_llama", configure = @@configure_my_llama)
    d.blocks = std_blocks()
    d.chat.add_bos = true
    d.chat.user.parts <- [chat_special("<|im_start|>"), chat_text("user\n"), chat_content(), chat_special("<|im_end|>"), chat_text("\n")]
    d.chat.assistant_open.parts <- [chat_special("<|im_start|>"), chat_text("assistant\n")]
    d.chat.stop <- ["<|im_end|>"]
    register_arch("my_llama", d)
    let after = has_arch("my_llama")
    print("has_arch(\"my_llama\"): {before} -> {after}\n")
    let names2 <- arch_names()
    print("registry now holds {length(names2)} architectures\n\n")

    // From here, load_model on a GGUF whose general.architecture says "my_llama"
    // would configure, load, and chat through this descriptor.

    // ──────────────────────────────────────────────────────────────────────────
    // Section 4 — Adding a real architecture to the module
    // ──────────────────────────────────────────────────────────────────────────
    //
    // The in-tree recipe (see modules/dasLLAMA/API_REWORK.md's model-support waves
    // for worked examples, from Qwen3's two flags to gpt-oss's MoE-with-sinks):
    //   1. modules/dasLLAMA/dasllama/dasllama_arch_<name>.das — configure + blocks +
    //      chat template, self-registered in an [init] function
    //   2. require it from the dasllama umbrella; add it to the module's CMake AOT
    //      file lists and .das_module descriptor
    //   3. validate token-for-token against llama.cpp on a real GGUF before trusting
    //      any output — greedy continuations must match id-for-id
    //   4. registry-count tests (tests/dasLLAMA/test_arch_registry.das) hardcode the
    //      architecture count — bump them

    print("done\n")
}
