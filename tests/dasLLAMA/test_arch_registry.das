options gen2

require dastest/testing_boost public
require dasllama/dasllama_transformer   // nolint:STYLE029 — umbrella fires each arch [init] registration; requiring dasllama_common directly would drop them

// The arch registry must self-populate at [init]: every supported architecture registered with its
// block kernels + a well-formed chat template — a user turn carrying a content slot, a generation
// prompt (assistant_open), and stop tokens. The template is data only in this phase; the chat engine
// that renders it lands in a later phase. No model file needed — the registry is [init]-populated.

def private has_content(turn : ChatTurn) : bool {
    for (p in turn.parts) {
        if (p.kind == ChatPartKind.content) {
            return true
        }
    }
    return false
}

[test]
def test_arch_registry_populated(t : T?) {
    t |> run("all six arches self-register at [init]") @(t : T?) {
        let expected <- ["llama", "qwen2", "qwen3", "phi3", "gemma2", "gemma3"]
        for (name in expected) {
            t |> success(has_arch(name), "arch '{name}' registered")
        }
        t |> equal(length(arch_names()), length(expected))
        t |> success(!has_arch("bogus_arch"), "unknown arch not registered")
    }
}

[test]
def test_arch_chat_templates(t : T?) {
    t |> run("each arch has a well-formed chat template") @(t : T?) {
        for (name in ["llama", "qwen2", "qwen3", "phi3", "gemma2", "gemma3"]) {
            let ct <- arch_chat(name)
            t |> success(length(ct.user.parts) > 0, "'{name}' user turn non-empty")
            t |> success(has_content(ct.user), "'{name}' user turn has a content slot")
            t |> success(length(ct.assistant_open.parts) > 0, "'{name}' assistant_open non-empty")
            t |> success(!empty(ct.stop), "'{name}' has stop tokens")
        }
    }
}
