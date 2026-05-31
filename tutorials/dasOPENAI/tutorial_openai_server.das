// Shared mock OpenAI-compatible server for dasOPENAI tutorials
//
// Provides a self-contained HTTP server on localhost that speaks the OpenAI
// REST surface (chat, embeddings, models, audio, completions) with canned
// responses, so the tutorials run end-to-end without a live LLM backend.
//
// Responses are built from the same typed openai_* structs the client decodes
// into (via sprint_json), so the mock and the real API agree on wire shape.
//
// Endpoints (all under /v1):
//   POST /chat/completions      — plain reply; or tool_calls / JSON-mode / SSE
//                                 stream depending on the request body
//   POST /embeddings            — a small canned embedding vector
//   GET  /models, /models/:id   — canned model list / single model
//   POST /audio/speech          — canned audio bytes
//   POST /audio/transcriptions  — canned transcription text
//   POST /completions           — canned legacy completion
//
// Usage:
//   require tutorial_openai_server
//
//   [export]
//   def main() {
//       with_openai_server(18190) $(base_url) {
//           let client = openai_client(base_url)
//           ...
//       }
//   }
//
// Lifecycle (thread + tick loop + shutdown) mirrors tutorials/dasHV/tutorial_server.das.

options gen2
options rtti   // build canned responses from the typed openai structs via sprint_json

require dashv/dashv_boost public
require daslib/jobque_boost
require daslib/fio
require daslib/json_boost
require daslib/strings_boost
require openai/openai_chat public
require openai/openai_embeddings public
require openai/openai_models public
require openai/openai_completions public

// ──────────────────────────────────────────────────────────────────────────
// Canned response builders
// ──────────────────────────────────────────────────────────────────────────

// ModelList is private in openai_models, so wrap the list locally.
struct private MockModelList {
    @rename = "object" _object : string
    data : array<Model>
}

def private sse_frame(json : string) : string {
    return "data: {json}\n\n"
}

def private mock_chat_json(content : string) : string {
    let r = ChatCompletionResponse(id = "chatcmpl-mock", _object = "chat.completion",
        created = 0, model = "mock-model",
        choices <- [ChatChoice(index = 0,
            message <- ChatMessage(role = "assistant", content = content),
            finish_reason = "stop")],
        usage = Usage(prompt_tokens = 9, completion_tokens = 7, total_tokens = 16))
    return sprint_json(r, false)
}

def private mock_tool_call_json() : string {
    let r = ChatCompletionResponse(id = "chatcmpl-mock", _object = "chat.completion",
        created = 0, model = "mock-model",
        choices <- [ChatChoice(index = 0,
            message <- ChatMessage(role = "assistant", content = "",
                tool_calls <- [ToolCall(id = "call_mock_1", _type = "function",
                    _function = ToolCallFunction(name = "get_weather",
                        arguments = "\{\"location\":\"Paris\"\}"))]),
            finish_reason = "tool_calls")],
        usage = Usage(prompt_tokens = 12, completion_tokens = 8, total_tokens = 20))
    return sprint_json(r, false)
}

def private mock_stream_body() : string {
    let words = ["Hello", " from", " the", " mock", " stream."]
    var frames : array<string>
    frames |> reserve(length(words) + 2)
    for (w in words) {
        let chunk = ChatCompletionChunk(id = "chatcmpl-mock", _object = "chat.completion.chunk",
            created = 0, model = "mock-model",
            choices <- [ChunkChoice(index = 0, delta = ChatMessageDelta(content = w))])
        frames |> push(sse_frame(sprint_json(chunk, false)))
    }
    let fin = ChatCompletionChunk(id = "chatcmpl-mock", _object = "chat.completion.chunk",
        created = 0, model = "mock-model",
        choices <- [ChunkChoice(index = 0, delta = ChatMessageDelta(), finish_reason = "stop")])
    frames |> push(sse_frame(sprint_json(fin, false)))
    frames |> push("data: [DONE]\n\n")
    let body = join(frames, "")
    delete frames
    return body
}

def private mock_embedding_json() : string {
    let r = EmbeddingResponse(_object = "list",
        data <- [EmbeddingData(_object = "embedding", index = 0,
            embedding <- [0.10, 0.20, 0.30, 0.40])],
        model = "mock-embed", usage = EmbeddingUsage(prompt_tokens = 4, total_tokens = 4))
    return sprint_json(r, false)
}

def private mock_models_json() : string {
    let ml = MockModelList(_object = "list",
        data <- [Model(id = "mock-model", _object = "model", owned_by = "mock"),
                 Model(id = "mock-embed", _object = "model", owned_by = "mock")])
    return sprint_json(ml, false)
}

def private mock_model_json(id : string) : string {
    let m = Model(id = id, _object = "model", owned_by = "mock")
    return sprint_json(m, false)
}

def private mock_completion_json() : string {
    let r = CompletionResponse(id = "cmpl-mock", _object = "text_completion",
        created = 0, model = "mock-model",
        choices <- [CompletionChoice(text = " a mock completion.", index = 0, finish_reason = "stop")],
        usage = Usage(prompt_tokens = 5, completion_tokens = 4, total_tokens = 9))
    return sprint_json(r, false)
}

// ──────────────────────────────────────────────────────────────────────────
// Server
// ──────────────────────────────────────────────────────────────────────────

class MockOpenAIServer : HvWebServer {
    def override onInit {
        // Chat: branch on the request body the client sent.
        POST("/v1/chat/completions") <| @(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
            let body = string(req.body)
            if (find(body, "\"stream\":true") >= 0) {
                set_header(resp, "Content-Type", "text/event-stream")
                return resp |> TEXT_PLAIN(mock_stream_body())
            }
            if (find(body, "\"tools\":") >= 0) {
                return resp |> JSON(mock_tool_call_json())
            }
            if (find(body, "\"response_format\":") >= 0) {
                return resp |> JSON(mock_chat_json("\{\"answer\":42\}"))
            }
            return resp |> JSON(mock_chat_json("Hello! This is a mock reply."))
        }
        POST("/v1/embeddings") <| @(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
            return resp |> JSON(mock_embedding_json())
        }
        GET("/v1/models") <| @(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
            return resp |> JSON(mock_models_json())
        }
        GET("/v1/models/:id") <| @(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
            return resp |> JSON(mock_model_json(get_param(req, "id")))
        }
        POST("/v1/audio/speech") <| @(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
            // Canned "audio" bytes — the client returns the body as array<uint8>.
            return resp |> TEXT_PLAIN("RIFFmock....WAVEmock-audio-bytes")
        }
        POST("/v1/audio/transcriptions") <| @(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
            return resp |> JSON("\{\"text\":\"a mock transcription of the audio\"\}")
        }
        POST("/v1/completions") <| @(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
            return resp |> JSON(mock_completion_json())
        }
    }
}

// ──────────────────────────────────────────────────────────────────────────
// Lifecycle helper — runs the server on a worker thread, invokes blk with the
// /v1 base URL, then shuts the thread down. Mirrors the dasHV tutorial server.
// ──────────────────────────────────────────────────────────────────────────

def with_openai_server(port : int; blk : block<(base_url : string) : void>) {
    with_job_status(1) $(started) {
        with_job_status(1) $(finished) {
            with_atomic32() $(stop_flag) {
                new_thread() @() {
                    var server = new MockOpenAIServer()
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
