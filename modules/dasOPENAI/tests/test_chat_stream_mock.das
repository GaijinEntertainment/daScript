options gen2
options rtti  // sprint_json/sscan_json honor @optional / @rename
options persistent_heap
options gc

require dastest/testing_boost public
require _openai_stream_mock
require openai/openai_chat

// Offline end-to-end streaming tests: stand up the local streaming mock server
// (_openai_stream_mock.das) on a worker thread and run real chat_stream calls
// against it. The mock writes the SSE bytes, so it reproduces each wire
// condition deterministically — plain content, CRLF endings + comment lines,
// and fragmented tool-call frames.

[test]
def test_stream_plain(t : T?) {
    t |> run("plain content stream accumulates in order") @(t : T?) {
        with_stream_server() $(base_url) {
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
        with_stream_server() $(base_url) {
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
        with_stream_server() $(base_url) {
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

[test]
def test_stream_usage(t : T?) {
    t |> run("stream_options.include_usage: the usage chunk's counts, cache hit and timings; reasoning deltas fire too; the request's template kwargs reach the wire") @(t : T?) {
        with_stream_server() $(base_url) {
            let client = openai_client(base_url)
            var req = ChatCompletionRequest(model = "mock-stream-usage",
                messages <- [ChatMessage(role = "user", content = "hi")],
                stream_options = StreamOptions(include_usage = true),
                chat_template_kwargs = "\{\"enable_thinking\":false}")
            var deltas = 0
            let res = chat_stream(client, req) $(delta : string) {
                deltas++
            }
            t |> success(res.ok, "stream ok")
            t |> equal(res.reasoning, "hm", "the reasoning delta accumulates apart from the content")
            t |> equal(res.content, "ok usage-asked thinking-off", "stream_options and the template kwargs reached the wire")
            t |> equal(deltas, 3, "the reasoning delta and both content deltas fired; the role-only opening and the usage chunk did not")
            t |> equal(res.usage.prompt_tokens, 100)
            t |> equal(res.usage.completion_tokens, 3)
            t |> equal(res.usage.total_tokens, 103)
            t |> equal(res.usage.prompt_tokens_details.cached_tokens, 40)
            t |> equal(res.timings.ttft_ms, 12.5)
            t |> equal(res.timings.gen_ms, 20.0, "a whole-number clock reads too")
        }
    }
    t |> run("a usage chunk without details reads no cache hit; a stream with no usage chunk reads no counts") @(t : T?) {
        with_stream_server() $(base_url) {
            let client = openai_client(base_url)
            var req = ChatCompletionRequest(model = "mock-stream-usage-nodetails",
                messages <- [ChatMessage(role = "user", content = "hi")],
                stream_options = StreamOptions(include_usage = true))
            let res = chat_stream(client, req) $(delta : string) {}
            t |> success(res.ok, "stream ok")
            t |> equal(res.usage.prompt_tokens, 100)
            t |> equal(res.usage.prompt_tokens_details.cached_tokens, -1, "no details: the server reported none")
            var plain = ChatCompletionRequest(model = "mock-stream-usage-nousage",
                messages <- [ChatMessage(role = "user", content = "hi")])
            let none = chat_stream(client, plain) $(delta : string) {}
            t |> success(none.ok, "stream ok")
            t |> equal(none.content, "ok usage-unasked thinking-on", "a request with neither field sends neither")
            t |> equal(none.usage.prompt_tokens, 0, "no usage chunk: no counts")
            t |> equal(none.timings.ttft_ms, 0.0, "no timings: a server that keeps no clock of its own")
        }
    }
}
