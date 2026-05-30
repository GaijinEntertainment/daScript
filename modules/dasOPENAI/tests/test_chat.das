options gen2
options rtti  // required so sprint_json/sscan_json honor @optional / @rename
options no_unused_function_arguments = false
options no_unused_block_arguments = false

require dastest/testing_boost public
require openai/openai_chat
require strings
require daslib/fio

// ===== Unit: serialization (no backend needed) =====

[test]
def test_chat_request_serialization(t : T?) {
    t |> run("request includes set fields and omits unset optionals") @(t : T?) {
        var req : ChatCompletionRequest
        req.model = "m1"
        req.messages <- [ChatMessage(role = "user", content = "hi")]
        let js = sprint_json(req, false)
        t |> success(find(js, "\"model\":\"m1\"") >= 0, "model present")
        t |> success(find(js, "\"role\":\"user\"") >= 0, "message role present")
        t |> success(find(js, "\"content\":\"hi\"") >= 0, "message content present")
        t |> success(find(js, "\"temperature\"") == -1, "unset temperature omitted")
        t |> success(find(js, "\"stop\"") == -1, "unset stop array omitted")

        req.temperature = 0.7
        req.max_tokens = 32
        let js2 = sprint_json(req, false)
        t |> success(find(js2, "\"temperature\"") >= 0, "set temperature included")
        t |> success(find(js2, "\"max_tokens\"") >= 0, "set max_tokens included")
    }
}

[test]
def test_chat_response_decode(t : T?) {
    t |> run("chat.completion round-trips through sprint_json/sscan_json") @(t : T?) {
        var resp : ChatCompletionResponse
        resp.id = "chatcmpl-123"
        resp._object = "chat.completion"
        resp.created = 1700000000
        resp.model = "test-model"
        resp.choices <- [ChatChoice(index = 0,
            message = ChatMessage(role = "assistant", content = "Hello there"),
            finish_reason = "stop")]
        resp.usage = Usage(prompt_tokens = 9, completion_tokens = 3, total_tokens = 12)

        let js = sprint_json(resp, false)
        // @rename = "object" must surface as the JSON key "object"
        t |> success(find(js, "\"object\":\"chat.completion\"") >= 0, "renamed object key emitted")

        var back : ChatCompletionResponse
        t |> success(sscan_json(js, back), "sscan_json succeeds")
        t |> equal(back.id, "chatcmpl-123")
        t |> equal(back._object, "chat.completion")
        t |> equal(back.model, "test-model")
        t |> equal(length(back.choices), 1)
        t |> equal(back.choices[0].message.role, "assistant")
        t |> equal(back.choices[0].message.content, "Hello there")
        t |> equal(back.choices[0].finish_reason, "stop")
        t |> equal(back.usage.total_tokens, 12)
    }
}

// ===== Live integration (requires a running backend) =====

[test]
def test_chat_live_ollama(t : T?) {
    t |> run("live chat against Ollama") @(t : T?) {
        let client = openai_client("http://localhost:11434/v1")
        var req : ChatCompletionRequest
        req.model = "hf.co/LiquidAI/LFM2.5-1.2B-Instruct-GGUF:Q4_K_M"
        req.messages <- [ChatMessage(role = "user", content = "Reply with exactly the word: pong")]
        req.max_tokens = 16
        let res = chat(client, req)
        t |> success(res.ok, "chat ok (kind={res.error.kind} status={res.error.status} {res.error.message})")
        if (res.ok) {
            t |> success(!empty(res.response.choices), "response has choices")
            t |> success(res.response.usage.total_tokens > 0, "usage reported")
        }
    }
}

[test]
def test_chat_live_openrouter(t : T?) {
    t |> run("live chat against OpenRouter (validates Bearer auth)") @(t : T?) {
        if (!has_env_variable("OPENROUTER_API_KEY")) {
            t |> success(true, "OPENROUTER_API_KEY not set - skipped")
            return
        }
        let client = openai_client("https://openrouter.ai/api/v1", get_env_variable("OPENROUTER_API_KEY"))
        var req : ChatCompletionRequest
        req.model = "liquid/lfm-2.5-1.2b-instruct:free"
        req.messages <- [ChatMessage(role = "user", content = "Reply with exactly the word: pong")]
        req.max_tokens = 16
        let res = chat(client, req)
        if (!res.ok && res.error.status == 429) {
            t |> success(true, "OpenRouter free-tier rate-limited (429) - skipped")
            return
        }
        t |> success(res.ok, "chat ok (kind={res.error.kind} status={res.error.status} {res.error.message})")
        if (res.ok) {
            t |> success(!empty(res.response.choices), "response has choices")
        }
    }
}
