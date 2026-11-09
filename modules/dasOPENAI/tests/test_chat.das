options gen2
options rtti  // required so sprint_json/sscan_json honor @optional / @rename
options no_unused_function_arguments = false
options no_unused_block_arguments = false

require dastest/testing_boost public
require _openai_live
require openai/openai_chat
require strings
require daslib/fio

//! The chat endpoint: wire shapes, streamed chunks, tool calls and
//! JSON mode, offline and live against Ollama or OpenRouter.

def private weather_tool() : Tool {
    var tool : Tool
    tool._type = "function"
    tool._function.name = "get_weather"
    tool._function.description = "Get the current weather for a location"
    tool._function.parameters = write_json(%json~
        {"type":"object","properties":{"location":{"type":"string"}},"required":["location"]}
    %%)
    return tool
}

[test(description = "A chat request serializes the model and messages, omits temperature and stop until they are set, and emits them once set.")]
def test_chat_request_serialization(t : T?) {
    t |> run("request includes set fields and omits unset optionals") @(t : T?) {
        var req = ChatCompletionRequest(
            model = "m1",
            messages <- [ChatMessage(role = "user", content = "hi")])
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

[test(description = "A chat.completion response round-trips through sprint_json and sscan_json with every field intact, and its _object field is written under the JSON key object.")]
def test_chat_response_decode(t : T?) {
    t |> run("chat.completion round-trips through sprint_json/sscan_json") @(t : T?) {
        let resp = ChatCompletionResponse(
            id = "chatcmpl-123",
            _object = "chat.completion",
            created = 1700000000,
            model = "test-model",
            choices <- [ChatChoice(index = 0,
                message = ChatMessage(role = "assistant", content = "Hello there"),
                finish_reason = "stop")],
            usage = Usage(prompt_tokens = 9, completion_tokens = 3, total_tokens = 12))

        let js = sprint_json(resp, false)
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

[test(description = "A live chat against Ollama returns choices and reports token usage; skipped when no Ollama server answers.", timeout = 1200)]
def test_chat_live_ollama(t : T?) {
    t |> run("live chat against Ollama") @(t : T?) {
        return if (!live_server(t, "http://localhost:11434/v1"))
        let client = openai_client("http://localhost:11434/v1")
        let req = ChatCompletionRequest(
            model = "hf.co/LiquidAI/LFM2.5-1.2B-Instruct-GGUF:Q4_K_M",
            messages <- [ChatMessage(role = "user", content = "Reply with exactly the word: pong")],
            max_tokens = 16)
        let res = chat(client, req)
        t |> success(res.ok, "chat ok (kind={res.error.kind} status={res.error.status} {res.error.message})")
        if (res.ok) {
            t |> success(!empty(res.response.choices), "response has choices")
            t |> success(res.response.usage.total_tokens > 0, "usage reported")
        }
    }
}

[test(description = "A live chat against OpenRouter authenticates with the Bearer key and returns choices; skipped without OPENROUTER_API_KEY, passed on a 429 rate limit.", timeout = 1200)]
def test_chat_live_openrouter(t : T?) {
    t |> run("live chat against OpenRouter (validates Bearer auth)") @(t : T?) {
        return if (!live_key(t, "OPENROUTER_API_KEY"))
        let client = openai_client("https://openrouter.ai/api/v1", get_env_variable("OPENROUTER_API_KEY"))
        let req = ChatCompletionRequest(
            model = "liquid/lfm-2.5-1.2b-instruct:free",
            messages <- [ChatMessage(role = "user", content = "Reply with exactly the word: pong")],
            max_tokens = 16)
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

[test(description = "A chat.completion.chunk round-trips through sprint_json and sscan_json with its delta role and content intact, under the JSON key object.")]
def test_stream_chunk_decode(t : T?) {
    t |> run("chat.completion.chunk round-trips through sprint_json/sscan_json") @(t : T?) {
        let frame = ChatCompletionChunk(
            id = "chunk-1",
            _object = "chat.completion.chunk",
            choices <- [ChunkChoice(index = 0,
                delta <- ChatMessageDelta(role = "assistant", content = "Hel"),
                finish_reason = "")])
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

[test(description = "A live chat_stream against Ollama delivers deltas whose concatenation is the accumulated content; skipped when no Ollama server answers.", timeout = 1200)]
def test_stream_live_ollama(t : T?) {
    t |> run("live streaming against Ollama") @(t : T?) {
        return if (!live_server(t, "http://localhost:11434/v1"))
        let client = openai_client("http://localhost:11434/v1")
        var req = ChatCompletionRequest(
            model = "hf.co/LiquidAI/LFM2.5-1.2B-Instruct-GGUF:Q4_K_M",
            messages <- [ChatMessage(role = "user", content = "Say: streaming works")],
            max_tokens = 32)
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

[test(description = "A live chat_stream against OpenRouter delivers deltas and non-empty content; skipped without OPENROUTER_API_KEY, passed on a 429 rate limit.", timeout = 1200)]
def test_stream_live_openrouter(t : T?) {
    t |> run("live streaming against OpenRouter") @(t : T?) {
        return if (!live_key(t, "OPENROUTER_API_KEY"))
        let client = openai_client("https://openrouter.ai/api/v1", get_env_variable("OPENROUTER_API_KEY"))
        var req = ChatCompletionRequest(
            model = "liquid/lfm-2.5-1.2b-instruct:free",
            messages <- [ChatMessage(role = "user", content = "Say: streaming works")],
            max_tokens = 32)
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

[test(description = "A tool attached to a chat request serializes its function name, tool_choice and the parameters schema embedded as raw JSON.")]
def test_tool_serialization(t : T?) {
    t |> run("tool serializes into request with function + parameters") @(t : T?) {
        var req = ChatCompletionRequest(
            model = "m",
            messages <- [ChatMessage(role = "user", content = "weather?")])
        req.tools |> push(weather_tool())
        req.tool_choice = "auto"
        let js = sprint_json(req, false)
        t |> success(find(js, "\"name\":\"get_weather\"") >= 0, "function name present")
        t |> success(find(js, "\"tool_choice\":\"auto\"") >= 0, "tool_choice present")
        // parameters is embedded raw JSON (write_json is pretty-printed, so match key fragments)
        t |> success(find(js, "\"parameters\":") >= 0, "parameters key present")
        t |> success(find(js, "location") >= 0 && find(js, "required") >= 0, "parameters schema embedded")
    }
}

[test(description = "An assistant message's tool_calls round-trip through sprint_json and sscan_json with the call id and function name intact.")]
def test_tool_call_response_decode(t : T?) {
    t |> run("assistant tool_calls round-trip through sprint_json/sscan_json") @(t : T?) {
        let args = write_json(%json~ {"location":"Paris"} %%)
        var resp = ChatCompletionResponse(id = "x", _object = "chat.completion")
        var msg : ChatMessage
        msg.role = "assistant"
        msg.tool_calls |> push(ToolCall(id = "call_1", _type = "function",
            _function = ToolCallFunction(name = "get_weather", arguments = args)))
        resp.choices |> emplace(ChatChoice(index = 0, message <- msg, finish_reason = "tool_calls"))

        let js = sprint_json(resp, false)
        var back : ChatCompletionResponse
        t |> success(sscan_json(js, back), "decode ok")
        t |> success(!empty(back.choices[0].message.tool_calls), "tool_calls decoded")
        t |> equal(back.choices[0].message.tool_calls[0]._function.name, "get_weather")
        t |> equal(back.choices[0].message.tool_calls[0].id, "call_1")
    }
}

[test(description = "A live model on OpenRouter answers a weather question by calling the offered get_weather tool; skipped without OPENROUTER_API_KEY, passed on a 429 rate limit.", timeout = 1200)]
def test_tools_live_openrouter(t : T?) {
    t |> run("live tool calling against OpenRouter") @(t : T?) {
        return if (!live_key(t, "OPENROUTER_API_KEY"))
        let client = openai_client("https://openrouter.ai/api/v1", get_env_variable("OPENROUTER_API_KEY"))
        var req = ChatCompletionRequest(
            model = "deepseek/deepseek-v4-flash:free",
            messages <- [ChatMessage(role = "user", content = "What is the weather in Paris? Use the get_weather tool.")])
        req.tools |> push(weather_tool())
        req.tool_choice = "auto"
        let res = chat(client, req)
        if (!res.ok && res.error.status == 429) {
            t |> success(true, "OpenRouter free-tier rate-limited (429) - skipped")
            return
        }
        t |> success(res.ok, "chat ok (kind={res.error.kind} status={res.error.status} {res.error.message})")
        if (res.ok && !empty(res.response.choices)) {
            t |> success(!empty(res.response.choices[0].message.tool_calls), "model returned tool_calls")
            if (!empty(res.response.choices[0].message.tool_calls)) {
                t |> equal(res.response.choices[0].message.tool_calls[0]._function.name, "get_weather")
            }
        }
    }
}

[test(description = "JSON mode sets response_format to json_object on a chat request, and a request without it omits the field.")]
def test_response_format_serialization(t : T?) {
    t |> run("json mode sets response_format, plain request omits it") @(t : T?) {
        var req = ChatCompletionRequest(
            model = "m",
            messages <- [ChatMessage(role = "user", content = "hi")])
        t |> success(find(sprint_json(req, false), "response_format") == -1, "omitted when unset")
        req.response_format = json_object_format()
        let js2 = sprint_json(req, false)
        t |> success(find(js2, "response_format") >= 0 && find(js2, "json_object") >= 0,
            "json_object response_format embedded")
    }
}

[test(description = "A live JSON-mode chat against Ollama returns content that parses as JSON; skipped when no Ollama server answers.", timeout = 1200)]
def test_structured_live_ollama(t : T?) {
    t |> run("live JSON mode against Ollama") @(t : T?) {
        return if (!live_server(t, "http://localhost:11434/v1"))
        let client = openai_client("http://localhost:11434/v1")
        let req = ChatCompletionRequest(
            model = "hf.co/LiquidAI/LFM2.5-1.2B-Instruct-GGUF:Q4_K_M",
            messages <- [ChatMessage(role = "user",
                content = "Return a JSON object with keys name and age for a person named Alice aged 30.")],
            response_format = json_object_format(),
            max_tokens = 128)
        let res = chat(client, req)
        t |> success(res.ok, "chat ok (kind={res.error.kind} {res.error.message})")
        if (res.ok && !empty(res.response.choices)) {
            let content = res.response.choices[0].message.content
            var err : string
            let parsed = read_json(content, err)
            t |> success(parsed != null, "response content is valid JSON ({content})")
        }
    }
}
