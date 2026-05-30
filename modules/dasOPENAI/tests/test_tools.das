options gen2
options rtti  // required so sprint_json/sscan_json honor @optional / @rename
options no_unused_function_arguments = false
options no_unused_block_arguments = false

require dastest/testing_boost public
require openai/openai_chat
require strings
require daslib/fio

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

[test]
def test_tool_serialization(t : T?) {
    t |> run("tool serializes into request with function + parameters") @(t : T?) {
        var req : ChatCompletionRequest
        req.model = "m"
        req.messages <- [ChatMessage(role = "user", content = "weather?")]
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

[test]
def test_tool_call_response_decode(t : T?) {
    t |> run("assistant tool_calls round-trip through sprint_json/sscan_json") @(t : T?) {
        let args = write_json(%json~ {"location":"Paris"} %%)
        var resp : ChatCompletionResponse
        resp.id = "x"
        resp._object = "chat.completion"
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

[test]
def test_tools_live_openrouter(t : T?) {
    t |> run("live tool calling against OpenRouter") @(t : T?) {
        if (!has_env_variable("OPENROUTER_API_KEY")) {
            t |> success(true, "OPENROUTER_API_KEY not set - skipped")
            return
        }
        let client = openai_client("https://openrouter.ai/api/v1", get_env_variable("OPENROUTER_API_KEY"))
        var req : ChatCompletionRequest
        req.model = "deepseek/deepseek-v4-flash:free"
        req.messages <- [ChatMessage(role = "user", content = "What is the weather in Paris? Use the get_weather tool.")]
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
