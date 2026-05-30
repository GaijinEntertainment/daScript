options gen2
options rtti  // required so sprint_json/sscan_json honor @optional / @rename
options no_unused_function_arguments = false
options no_unused_block_arguments = false

require dastest/testing_boost public
require openai/openai_completions
require strings

[test]
def test_completion_request_serialization(t : T?) {
    t |> run("completion request includes fields, omits unset optionals") @(t : T?) {
        var req : CompletionRequest
        req.model = "m"
        req.prompt = "hello"
        let js = sprint_json(req, false)
        t |> success(find(js, "\"model\":\"m\"") >= 0, "model present")
        t |> success(find(js, "\"prompt\":\"hello\"") >= 0, "prompt present")
        t |> success(find(js, "\"temperature\"") == -1, "unset temperature omitted")
    }
}

[test]
def test_completion_response_decode(t : T?) {
    t |> run("completion response round-trips through sprint_json/sscan_json") @(t : T?) {
        var resp : CompletionResponse
        resp.id = "cmpl-1"
        resp._object = "text_completion"
        resp.model = "m"
        resp.choices <- [CompletionChoice(text = "Paris", index = 0, finish_reason = "stop")]
        resp.usage = Usage(prompt_tokens = 3, completion_tokens = 1, total_tokens = 4)
        let js = sprint_json(resp, false)
        var back : CompletionResponse
        t |> success(sscan_json(js, back), "decode ok")
        t |> equal(back._object, "text_completion")
        t |> equal(back.choices[0].text, "Paris")
        t |> equal(back.usage.total_tokens, 4)
    }
}

[test]
def test_completion_live_ollama(t : T?) {
    t |> run("live completion against Ollama") @(t : T?) {
        let client = openai_client("http://localhost:11434/v1")
        let text = complete(client, "hf.co/LiquidAI/LFM2.5-1.2B-Instruct-GGUF:Q4_K_M",
            "The capital of France is")
        t |> success(!empty(text), "got completion text")
    }
}
