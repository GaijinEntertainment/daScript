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

class MockStreamServer : HvWebServer {
    def override onInit {
        POST("/v1/chat/completions") <| @(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
            let body = string(req.body)
            set_header(resp, "Content-Type", "text/event-stream")
            if (find(body, "\"tools\":") >= 0) {
                return resp |> TEXT_PLAIN(mock_stream_tool_calls_body())
            }
            if (find(body, "stream-crlf") >= 0) {
                return resp |> TEXT_PLAIN(mock_stream_body_crlf())
            }
            return resp |> TEXT_PLAIN(mock_stream_body())
        }
    }
}

def with_stream_server(port : int; blk : block<(base_url : string) : void>) {
    with_job_status(1) $(started) {
        with_job_status(1) $(finished) {
            with_atomic32() $(stop_flag) {
                new_thread() @() {
                    var server = new MockStreamServer()
                    server->init(port)
                    server->start()
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
                let base_url = "http://127.0.0.1:{port}/v1"
                invoke(blk, base_url)
                stop_flag |> set(1)
                finished |> join
            }
        }
    }
}
