options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)

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
    t |> run("all eighteen arches self-register at [init]") @(t : T?) {
        let expected <- ["llama", "mistral3", "qwen2", "qwen2vl", "qwen3", "qwen3vl", "phi3", "gemma2", "gemma3", "gemma4", "qwen2moe", "qwen3moe", "qwen3vlmoe", "qwen35", "qwen35moe", "qwen3next", "glm4moe", "gpt-oss"]
        for (name in expected) {
            t |> success(has_arch(name), "arch '{name}' registered")
        }
        t |> equal(length(arch_names()), length(expected))
        t |> success(!has_arch("bogus_arch"), "unknown arch not registered")
    }
}

[test]
def test_llm_caps(t : T?) {
    t |> run("per-arch LlmCaps: gemma has no system role, everyone else does") @(t : T?) {
        for (name in ["gemma2", "gemma3", "gemma4"]) {
            t |> success(!arch_lcaps(name).system_prompt, "'{name}' system_prompt false")
        }
        for (name in ["llama", "mistral3", "qwen2", "qwen2vl", "qwen3", "qwen3vl", "phi3", "qwen2moe", "qwen3moe", "qwen3vlmoe", "qwen35", "qwen35moe", "qwen3next", "glm4moe", "gpt-oss"]) {
            t |> success(arch_lcaps(name).system_prompt, "'{name}' system_prompt true")
        }
    }
}

[test]
def test_audio_wrap_parts(t : T?) {
    t |> run("audio span markers: qwen2 family wraps, llama splices bare") @(t : T?) {
        for (name in ["qwen2", "qwen2vl"]) {
            let ct <- arch_chat(name)
            t |> equal(length(ct.audio_pre.parts), 1, "'{name}' audio_pre")
            t |> equal(ct.audio_pre.parts[0].s, "<|audio_bos|>")
            t |> equal(length(ct.audio_post.parts), 1, "'{name}' audio_post")
            t |> equal(ct.audio_post.parts[0].s, "<|audio_eos|>")
        }
        let q3 <- arch_chat("qwen3vl")
        t |> equal(q3.audio_pre.parts[0].s, "<|audio_start|>", "qwen3vl audio_pre")
        t |> equal(q3.audio_post.parts[0].s, "<|audio_end|>", "qwen3vl audio_post")
        let lt <- arch_chat("llama")
        t |> success(empty(lt.audio_pre.parts) && empty(lt.audio_post.parts), "llama-3: bare embeddings")
    }
}

[test]
def test_arch_chat_templates(t : T?) {
    t |> run("each arch has a well-formed chat template") @(t : T?) {
        for (name in ["llama", "qwen2", "qwen2vl", "qwen3", "qwen3vl", "phi3", "gemma2", "gemma3", "gemma4", "qwen2moe", "qwen3moe", "qwen3vlmoe", "qwen35", "qwen35moe", "qwen3next", "glm4moe", "gpt-oss"]) {
            let ct <- arch_chat(name)
            t |> success(length(ct.user.parts) > 0, "'{name}' user turn non-empty")
            t |> success(has_content(ct.user), "'{name}' user turn has a content slot")
            t |> success(length(ct.assistant_open.parts) > 0, "'{name}' assistant_open non-empty")
            t |> success(!empty(ct.stop), "'{name}' has stop tokens")
        }
        // mistral3 (v7-tekken): the reply opens bare right after [/INST] — assistant_open is
        // deliberately empty, so it sits outside the loop's non-empty assertion
        let mt <- arch_chat("mistral3")
        t |> success(has_content(mt.user), "'mistral3' user turn has a content slot")
        t |> success(empty(mt.assistant_open.parts), "'mistral3' reply opens bare after [/INST]")
        t |> success(!empty(mt.stop), "'mistral3' has stop tokens")
        // glm4moe extras: the conversation-start prelude turn + the non-thinking prefill
        let gt <- arch_chat("glm4moe")
        t |> success(!empty(gt.prelude.parts), "'glm4moe' [gMASK]<sop> prelude present")
        t |> success(!empty(gt.think_suppress.parts), "'glm4moe' think_suppress wired")
    }
}
