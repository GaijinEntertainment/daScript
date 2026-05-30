options gen2
options indenting = 4
options rtti  // honor @optional / @rename (consumer roots must also set it)

//! OpenAI-compatible API — chat completions: non-streaming ``chat``, streaming ``chat_stream``,
//! tools / function-calling, and structured outputs (JSON mode).

module openai_chat shared public

require openai/openai_common public
require daslib/strings_boost

// ===== Chat types =====

struct ResponseFormat {
    //! Chat ``response_format``: ``_type`` is "text" | "json_object" | "json_schema".
    @rename = "type" _type : string
    @optional @embed json_schema : string
}

struct FunctionDef {
    //! A tool's function declaration. ``parameters`` is a raw JSON-schema string.
    name : string
    @optional description : string
    @optional @embed parameters : string
}

struct Tool {
    //! A tool the model may call. ``_type`` is "function".
    @rename = "type" _type : string
    @rename = "function" _function : FunctionDef
}

struct ToolCallFunction {
    name : string
    arguments : string
}

struct ToolCall {
    //! A tool call requested by the model. ``_function.arguments`` is a JSON string.
    id : string
    @rename = "type" _type : string
    @rename = "function" _function : ToolCallFunction
}

struct ChatMessage {
    //! A single chat message. ``role`` is "system" | "user" | "assistant" | "tool".
    role : string
    content : string
    @optional name : string
    @optional tool_calls : array<ToolCall>
    @optional tool_call_id : string
}

struct ChatCompletionRequest {
    //! Request body for ``/chat/completions``. Default-valued optional fields are omitted on send.
    model : string
    messages : array<ChatMessage>
    @optional temperature : float
    @optional top_p : float
    @optional n : int
    @optional stream : bool
    @optional max_tokens : int
    @optional max_completion_tokens : int
    @optional frequency_penalty : float
    @optional presence_penalty : float
    @optional seed : int
    @optional stop : array<string>
    @optional user : string
    @optional @embed response_format : string
    @optional tools : array<Tool>
    @optional tool_choice : string
}

struct ChatChoice {
    index : int
    message : ChatMessage
    finish_reason : string
}

struct ChatCompletionResponse {
    //! Response body for a non-streaming ``/chat/completions`` call.
    id : string
    @rename = "object" _object : string
    created : int64
    model : string
    choices : array<ChatChoice>
    usage : Usage
}

struct ChatResult {
    //! Outcome of ``chat``: when ``ok`` is true ``response`` is valid, else inspect ``error``.
    ok : bool
    response : ChatCompletionResponse
    error : OpenAIError
}

// ===== Streaming chat types =====

struct ChatMessageDelta {
    //! Incremental message fields in a streamed chunk (object == "chat.completion.chunk").
    @optional role : string
    @optional content : string
}

struct ChunkChoice {
    index : int
    delta : ChatMessageDelta
    @optional finish_reason : string
}

struct ChatCompletionChunk {
    //! One streamed delta frame.
    id : string
    @rename = "object" _object : string
    created : int64
    model : string
    choices : array<ChunkChoice>
}

struct ChatStreamResult {
    //! Outcome of ``chat_stream``: ``content`` is the full accumulated assistant text.
    ok : bool
    content : string
    finish_reason : string
    error : OpenAIError
}

// ===== Chat =====

def chat(client : OpenAIClient; req : ChatCompletionRequest) : ChatResult {
    //! Sends a non-streaming ``/chat/completions`` request and decodes the response.
    var response : ChatCompletionResponse
    let err = post_and_decode(client, "/chat/completions", sprint_json(req, false), response)
    return ChatResult(ok = empty(err.kind), response <- response, error = err)
}

def json_object_format() : string {
    //! Returns the ``response_format`` value for JSON mode (assign to ``req.response_format``).
    return sprint_json(ResponseFormat(_type = "json_object"), false)
}

def chat_text(client : OpenAIClient; model : string; prompt : string) : string {
    //! Convenience: one user turn, returns the assistant's text ("" on error).
    let req = ChatCompletionRequest(model = model, messages <- [ChatMessage(role = "user", content = prompt)])
    let res = chat(client, req)
    if (!res.ok || empty(res.response.choices)) return ""
    return res.response.choices[0].message.content
}

// ===== Streaming chat =====

def chat_stream(client : OpenAIClient; var req : ChatCompletionRequest;
                on_delta : block<(delta : string) : void>) : ChatStreamResult {
    //! Streams a ``/chat/completions`` response, invoking ``on_delta`` with each text
    //! increment as it arrives. Returns the full accumulated content plus status.
    //!
    //! Accumulates text deltas (``choices[0].delta.content``) only — streamed tool/function
    //! calls are not reassembled. For tool calls, use the non-streaming ``chat``.
    req.stream = true
    let body_json = sprint_json(req, false)
    let url = "{client.base_url}/chat/completions"

    var result : ChatStreamResult
    var buffer = ""
    var parts : array<string>   // accumulate deltas, join once at the end (avoid O(n^2) string growth)
    var finish = ""

    with_http_request() $(var hreq : HttpRequest?#) {
        hreq.method = http_method.POST
        hreq.url := url
        hreq.body := body_json
        set_content_type(hreq, "application/json")
        configure_request(hreq, client)
        request_cb(hreq,
            $(chunk : string) {
                buffer += chunk
                var nl = find(buffer, "\n")
                while (nl >= 0) {
                    var line = slice(buffer, 0, nl)
                    buffer = slice(buffer, nl + 1, length(buffer))
                    // tolerate CRLF line endings: drop a trailing '\r' so payload + [DONE] parse cleanly
                    if (ends_with(line, "\r")) {
                        line = slice(line, 0, length(line) - 1)
                    }
                    if (starts_with(line, "data: ")) {
                        let payload = slice(line, 6, length(line))
                        if (payload != "[DONE]") {
                            var frame : ChatCompletionChunk
                            if (sscan_json(payload, frame) && !empty(frame.choices)) {
                                let d = frame.choices[0].delta.content
                                if (!empty(d)) {
                                    parts |> push(d)
                                    invoke(on_delta, d)
                                }
                                if (!empty(frame.choices[0].finish_reason)) {
                                    finish = frame.choices[0].finish_reason
                                }
                            }
                        }
                    }
                    nl = find(buffer, "\n")
                }
            },
            $(var resp : HttpResponse?) {
                if (resp == null) {
                    result.error = OpenAIError(kind = "transport", message = "no response from {url}")
                } else {
                    let code = int(resp.status_code)
                    if (code >= 200 && code < 300) {
                        result.ok = true
                    } else {
                        result.error = parse_error_body(code, "{resp.body}")
                    }
                }
            })
    }
    result.content = join(parts, "")
    delete parts
    result.finish_reason = finish
    return result
}
