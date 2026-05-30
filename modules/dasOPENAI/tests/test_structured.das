options gen2
options rtti  // required so sprint_json/sscan_json honor @optional / @rename
options no_unused_function_arguments = false
options no_unused_block_arguments = false

require dastest/testing_boost public
require openai/openai_chat
require strings

[test]
def test_response_format_serialization(t : T?) {
    t |> run("json mode sets response_format, plain request omits it") @(t : T?) {
        var req : ChatCompletionRequest
        req.model = "m"
        req.messages <- [ChatMessage(role = "user", content = "hi")]
        t |> success(find(sprint_json(req, false), "response_format") == -1, "omitted when unset")
        req.response_format = json_object_format()
        let js2 = sprint_json(req, false)
        t |> success(find(js2, "response_format") >= 0 && find(js2, "json_object") >= 0,
            "json_object response_format embedded")
    }
}

[test]
def test_structured_live_ollama(t : T?) {
    t |> run("live JSON mode against Ollama") @(t : T?) {
        let client = openai_client("http://localhost:11434/v1")
        var req : ChatCompletionRequest
        req.model = "hf.co/LiquidAI/LFM2.5-1.2B-Instruct-GGUF:Q4_K_M"
        req.messages <- [ChatMessage(role = "user",
            content = "Return a JSON object with keys name and age for a person named Alice aged 30.")]
        req.response_format = json_object_format()
        req.max_tokens = 128
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
