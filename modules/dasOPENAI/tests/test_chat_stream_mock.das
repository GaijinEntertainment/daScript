options gen2
options rtti  // sprint_json/sscan_json honor @optional / @rename
options persistent_heap
options gc

require dastest/testing_boost public
require _openai_stream_mock
require openai/openai_chat
require strings

// Offline end-to-end streaming tests: stand up the local streaming mock server
// (_openai_stream_mock.das) on a worker thread and run real chat_stream calls
// against it. The mock writes the SSE bytes, so it reproduces each wire
// condition deterministically — plain content, CRLF endings + comment lines,
// and fragmented tool-call frames.

[test]
def test_stream_plain(t : T?) {
    t |> run("plain content stream accumulates in order") @(t : T?) {
        with_stream_server(18231) $(base_url) {
            let client = openai_client(base_url)
            var req = ChatCompletionRequest(model = "mock-model",
                messages <- [ChatMessage(role = "user", content = "hi")])
            var acc = ""
            var deltas = 0
            let res = chat_stream(client, req) $(delta : string) {
                deltas++
                acc += delta
            }
            t |> success(res.ok, "stream ok")
            t |> equal(res.content, "Hello from the mock stream.")
            t |> equal(acc, res.content)
            t |> success(deltas == 5, "five content deltas, got {deltas}")
            t |> equal(res.finish_reason, "stop")
            t |> equal(length(res.tool_calls), 0)
        }
    }
}

[test]
def test_stream_crlf(t : T?) {
    t |> run("CRLF endings and comment lines parse cleanly") @(t : T?) {
        with_stream_server(18232) $(base_url) {
            let client = openai_client(base_url)
            var req = ChatCompletionRequest(model = "mock-stream-crlf",
                messages <- [ChatMessage(role = "user", content = "hi")])
            let res = chat_stream(client, req) $(delta : string) {}
            t |> success(res.ok, "stream ok")
            t |> equal(res.content, "CRLF stream works.")
            t |> equal(res.finish_reason, "stop")
        }
    }
}

[test]
def test_stream_tool_calls(t : T?) {
    t |> run("streamed tool-call fragments reassemble by index") @(t : T?) {
        with_stream_server(18233) $(base_url) {
            let client = openai_client(base_url)
            var req = ChatCompletionRequest(model = "mock-model",
                messages <- [ChatMessage(role = "user", content = "weather in Paris?")],
                tools <- [Tool(_type = "function", _function = FunctionDef(name = "get_weather"))])
            var textDeltas = 0
            let res = chat_stream(client, req) $(delta : string) {
                textDeltas++
            }
            t |> success(res.ok, "stream ok")
            t |> equal(res.finish_reason, "tool_calls")
            t |> equal(textDeltas, 0)  // a tool-call stream carries no text content
            t |> equal(length(res.tool_calls), 1)
            if (length(res.tool_calls) == 1) {
                t |> equal(res.tool_calls[0].id, "call_mock_1")
                t |> equal(res.tool_calls[0]._type, "function")
                t |> equal(res.tool_calls[0]._function.name, "get_weather")
                t |> equal(res.tool_calls[0]._function.arguments, "\{\"location\":\"Paris\"\}")
            }
        }
    }
}
