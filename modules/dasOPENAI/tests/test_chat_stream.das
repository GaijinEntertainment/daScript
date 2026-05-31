options gen2
options rtti  // required so sprint_json/sscan_json honor @optional / @rename
options no_unused_function_arguments = false
options no_unused_block_arguments = false

require dastest/testing_boost public
require openai/openai_chat
require strings
require daslib/fio

// ===== Unit: chunk serialization (no backend needed) =====

[test]
def test_stream_chunk_decode(t : T?) {
    t |> run("chat.completion.chunk round-trips through sprint_json/sscan_json") @(t : T?) {
        var frame : ChatCompletionChunk
        frame.id = "chunk-1"
        frame._object = "chat.completion.chunk"
        frame.choices <- [ChunkChoice(index = 0,
            delta <- ChatMessageDelta(role = "assistant", content = "Hel"),
            finish_reason = "")]
        let js = sprint_json(frame, false)
        t |> success(find(js, "\"object\":\"chat.completion.chunk\"") >= 0, "renamed object key emitted")

        var back : ChatCompletionChunk
        t |> success(sscan_json(js, back), "decode ok")
        t |> equal(back._object, "chat.completion.chunk")
        t |> equal(length(back.choices), 1)
        t |> equal(back.choices[0].delta.role, "assistant")
        t |> equal(back.choices[0].delta.content, "Hel")
    }
}

// ===== Live integration (requires a running backend) =====

[test]
def test_stream_live_ollama(t : T?) {
    t |> run("live streaming against Ollama") @(t : T?) {
        let client = openai_client("http://localhost:11434/v1")
        var req : ChatCompletionRequest
        req.model = "hf.co/LiquidAI/LFM2.5-1.2B-Instruct-GGUF:Q4_K_M"
        req.messages <- [ChatMessage(role = "user", content = "Say: streaming works")]
        req.max_tokens = 32
        var deltas = 0
        var acc = ""
        let res = chat_stream(client, req) $(delta : string) {
            deltas++
            acc += delta
        }
        t |> success(res.ok, "stream ok (kind={res.error.kind} status={res.error.status} {res.error.message})")
        if (res.ok) {
            t |> success(deltas > 0, "received {deltas} delta(s)")
            t |> success(!empty(res.content), "accumulated content non-empty")
            t |> equal(res.content, acc)
        }
    }
}

[test]
def test_stream_live_openrouter(t : T?) {
    t |> run("live streaming against OpenRouter") @(t : T?) {
        if (!has_env_variable("OPENROUTER_API_KEY")) {
            t |> success(true, "OPENROUTER_API_KEY not set - skipped")
            return
        }
        let client = openai_client("https://openrouter.ai/api/v1", get_env_variable("OPENROUTER_API_KEY"))
        var req : ChatCompletionRequest
        req.model = "liquid/lfm-2.5-1.2b-instruct:free"
        req.messages <- [ChatMessage(role = "user", content = "Say: streaming works")]
        req.max_tokens = 32
        var deltas = 0
        let res = chat_stream(client, req) $(delta : string) {
            deltas++
        }
        if (!res.ok && res.error.status == 429) {
            t |> success(true, "OpenRouter free-tier rate-limited (429) - skipped")
            return
        }
        t |> success(res.ok, "stream ok (kind={res.error.kind} status={res.error.status} {res.error.message})")
        if (res.ok) {
            t |> success(deltas > 0, "received {deltas} delta(s)")
            t |> success(!empty(res.content), "content non-empty")
        }
    }
}
