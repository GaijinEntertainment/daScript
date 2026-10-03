// Test fixture: a minimal streaming-only mock OpenAI server for
// test_chat_stream_mock.das. Self-contained here (next to the tests) so the
// streaming tests don't depend on the tutorials tree.
//
// The leading underscore keeps dastest from treating this file as a test.
//
// Serves POST /v1/chat/completions only, selecting an SSE script from the
// request body: a fragmented tool-call sequence when "tools" is present, a
// CRLF + comment-line stream when the model id contains "stream-crlf", else a
// plain content stream. Bodies are built from the typed openai_* structs so the
// mock and the client agree on wire shape. Lifecycle mirrors the dasHV server.

options gen2
options rtti   // build canned SSE frames from the typed openai structs via sprint_json

require dashv/dashv_boost public
require daslib/jobque_boost
require daslib/fio
require daslib/strings_boost
require openai/openai_chat public

def private sse_frame(json : string) : string {
    return "data: {json}\n\n"
}

def private content_chunk(text : string) : string {
    let chunk = ChatCompletionChunk(id = "chatcmpl-mock", _object = "chat.completion.chunk",
        created = 0, model = "mock-model",
        choices <- [ChunkChoice(index = 0, delta <- ChatMessageDelta(content = text))])
    return sprint_json(chunk, false)
}

def private finish_chunk(reason : string) : string {
    let fin = ChatCompletionChunk(id = "chatcmpl-mock", _object = "chat.completion.chunk",
        created = 0, model = "mock-model",
        choices <- [ChunkChoice(index = 0, delta <- ChatMessageDelta(), finish_reason = reason)])
    return sprint_json(fin, false)
}

def private tool_call_chunk(td : ToolCallDelta) : string {
    let chunk = ChatCompletionChunk(id = "chatcmpl-mock", _object = "chat.completion.chunk",
        created = 0, model = "mock-model",
        choices <- [ChunkChoice(index = 0, delta <- ChatMessageDelta(tool_calls <- [td]))])
    return sprint_json(chunk, false)
}

def private mock_stream_body() : string {
    let words = ["Hello", " from", " the", " mock", " stream."]
    var frames : array<string>
    frames |> reserve(length(words) + 2)
    for (w in words) {
        frames |> push(sse_frame(content_chunk(w)))
    }
    frames |> push(sse_frame(finish_chunk("stop")))
    frames |> push("data: [DONE]\n\n")
    let body = join(frames, "")
    delete frames
    return body
}

// CRLF line endings + a leading SSE comment line (": ping") — exercises the
// client's trailing-\r stripping and comment-line skipping.
def private mock_stream_body_crlf() : string {
    let words = ["CRLF", " stream", " works."]
    var frames : array<string>
    frames |> reserve(length(words) + 3)
    frames |> push(": ping\r\n\r\n")
    for (w in words) {
        frames |> push("data: {content_chunk(w)}\r\n\r\n")
    }
    frames |> push("data: {finish_chunk("stop")}\r\n\r\n")
    frames |> push("data: [DONE]\r\n\r\n")
    let body = join(frames, "")
    delete frames
    return body
}

// One tool call streamed as fragments: id+name first, then arguments across two frames.
// The leading index=-1 and index=9999999 frames are malformed on purpose —
// chat_stream must drop both (negative + over-cap guards) without crashing or
// polluting the real call at index 0.
def private mock_stream_tool_calls_body() : string {
    var frames <- [
        sse_frame(tool_call_chunk(ToolCallDelta(index = -1, id = "BOGUS", _type = "function",
            _function = ToolCallFunctionDelta(name = "should_be_ignored", arguments = "garbage")))),
        sse_frame(tool_call_chunk(ToolCallDelta(index = 9999999, id = "BOGUS2", _type = "function",
            _function = ToolCallFunctionDelta(name = "also_ignored", arguments = "more garbage")))),
        sse_frame(tool_call_chunk(ToolCallDelta(index = 0, id = "call_mock_1", _type = "function",
            _function = ToolCallFunctionDelta(name = "get_weather")))),
        sse_frame(tool_call_chunk(ToolCallDelta(index = 0,
            _function = ToolCallFunctionDelta(arguments = "\{\"location\":\"")))),
        sse_frame(tool_call_chunk(ToolCallDelta(index = 0,
            _function = ToolCallFunctionDelta(arguments = "Paris\"\}")))),
        sse_frame(finish_chunk("tool_calls")),
        "data: [DONE]\n\n"
    ]
    let body = join(frames, "")
    delete frames
    return body
}

// A thinking model's stream under stream_options.include_usage: a role-only opening delta, a reasoning
// delta, two content deltas, then the usage chunk (no choices) with the server's own timings; a request
// whose model id carries "nodetails" gets a usage chunk without prompt_tokens_details, one carrying
// "nousage" none at all. The request's stream_options and chat_template_kwargs are echoed as content
// so a test can see what reached the wire.
def private mock_stream_usage_body(body : string; details, usage : bool) : string {
    let opening = ChatCompletionChunk(id = "chatcmpl-mock", _object = "chat.completion.chunk", created = 0, model = "mock-model",
        choices <- [ChunkChoice(index = 0, delta <- ChatMessageDelta(role = "assistant"))])
    let thought = ChatCompletionChunk(id = "chatcmpl-mock", _object = "chat.completion.chunk", created = 0, model = "mock-model",
        choices <- [ChunkChoice(index = 0, delta <- ChatMessageDelta(reasoning_content = "hm"))])
    let wire = "{find(body, "\"include_usage\":true") >= 0 ? "usage-asked" : "usage-unasked"} {find(body, "\"enable_thinking\":false") >= 0 ? "thinking-off" : "thinking-on"}"
    var frames <- [sse_frame(sprint_json(opening, false)), sse_frame(sprint_json(thought, false)),
        sse_frame(content_chunk("ok ")), sse_frame(content_chunk(wire)), sse_frame(finish_chunk("stop"))]
    if (usage) {
        let det = details ? ",\"prompt_tokens_details\":\{\"cached_tokens\":40}" : ""
        frames |> push(sse_frame("\{\"id\":\"chatcmpl-mock\",\"object\":\"chat.completion.chunk\",\"created\":0,\"model\":\"mock-model\",\"choices\":[],\"usage\":\{\"prompt_tokens\":100,\"completion_tokens\":3,\"total_tokens\":103{det}},\"timings\":\{\"ttft_ms\":12.5,\"gen_ms\":20}}"))
    }
    frames |> push("data: [DONE]\n\n")
    let out = join(frames, "")
    delete frames
    return out
}

class MockStreamServer : HvWebServer {
    def override onInit {
        // a STREAM route: the client reads a chunked event stream, never a buffered body
        STREAM("/v1/chat/completions") <| @(var req : HttpRequest?; var writer : HttpResponseWriter?) {
            let body = string(req.body)
            var script = ""
            if (find(body, "\"tools\":") >= 0) {
                script = mock_stream_tool_calls_body()
            } elif (find(body, "stream-crlf") >= 0) {
                script = mock_stream_body_crlf()
            } elif (find(body, "stream-usage") >= 0) {
                script = mock_stream_usage_body(body, find(body, "nodetails") < 0, find(body, "nousage") < 0)
            } else {
                script = mock_stream_body()
            }
            set_header(server, writer, "Content-Type", "text/event-stream")
            write_chunked(server, writer, script)   // the first chunk ends the headers as a chunked transfer
            close_writer(server, writer)
        }
    }
}

//! the mock on its own thread and an ephemeral port (a fixed port is refused on a box where it is taken, and the
//! client then times out instead of failing)
def with_stream_server(blk : block<(base_url : string) : void>) {
    with_job_status(1) $(started) {
        with_job_status(1) $(finished) {
            with_atomic32() $(bound) {
            with_atomic32() $(stop_flag) {
                new_thread() @() {
                    var server = new MockStreamServer()
                    server->init(0)
                    var port = -1
                    if (server->start() == 0) {
                        port = server->bound_port()
                    }
                    bound |> set(port)
                    started |> notify_and_release
                    while (stop_flag |> get == 0) {
                        server->tick()
                        sleep(10u)
                    }
                    server->stop()
                    server->cleanup()
                    unsafe {
                        delete server
                    }
                    finished |> notify_and_release
                }
                started |> join
                let port = bound |> get
                invoke(blk, "http://127.0.0.1:{port}/v1")
                stop_flag |> set(1)
                finished |> join
            }
            }
        }
    }
}
