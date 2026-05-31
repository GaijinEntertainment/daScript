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
    @rename = "type" _type : string         //!< "text" | "json_object" | "json_schema".
    @optional @embed json_schema : string   //!< Raw JSON-schema string, when ``_type`` is "json_schema".
}

struct FunctionDef {
    //! A tool's function declaration. ``parameters`` is a raw JSON-schema string.
    name : string                       //!< Function name the model may call.
    @optional description : string      //!< What the function does (helps the model decide when to call it).
    @optional @embed parameters : string //!< Raw JSON-schema string describing the arguments.
}

struct Tool {
    //! A tool the model may call. ``_type`` is "function".
    @rename = "type" _type : string           //!< Tool type ("function").
    @rename = "function" _function : FunctionDef //!< The function declaration.
}

struct ToolCallFunction {
    //! The function a tool call targets.
    name : string       //!< Function name.
    arguments : string  //!< JSON-encoded argument string.
}

struct ToolCall {
    //! A tool call requested by the model. ``_function.arguments`` is a JSON string.
    id : string                                  //!< Unique id for this call; echo it back in the tool result.
    @rename = "type" _type : string              //!< Tool type ("function").
    @rename = "function" _function : ToolCallFunction //!< The requested function + arguments.
}

struct ChatMessage {
    //! A single chat message. ``role`` is "system" | "user" | "assistant" | "tool".
    role : string                       //!< "system" | "user" | "assistant" | "tool".
    content : string                    //!< Message text; may be empty when ``tool_calls`` is set.
    @optional name : string             //!< Optional author name.
    @optional tool_calls : array<ToolCall> //!< Tool calls the assistant is requesting.
    @optional tool_call_id : string     //!< For ``role`` "tool": the id of the call this message answers.
}

struct ChatCompletionRequest {
    //! Request body for ``/chat/completions``. Default-valued optional fields are omitted on send.
    model : string                       //!< Chat model id.
    messages : array<ChatMessage>        //!< The conversation, in order.
    @optional temperature : float        //!< Sampling temperature.
    @optional top_p : float              //!< Nucleus-sampling probability mass.
    @optional n : int                    //!< Number of choices to generate.
    @optional stream : bool              //!< Request a streamed response (set automatically by ``chat_stream``).
    @optional max_tokens : int           //!< Maximum tokens to generate.
    @optional max_completion_tokens : int //!< Newer cap on generated tokens.
    @optional frequency_penalty : float  //!< Penalize tokens by how often they have appeared.
    @optional presence_penalty : float   //!< Penalize tokens that already appeared at all.
    @optional seed : int                 //!< Seed for (best-effort) deterministic sampling.
    @optional stop : array<string>       //!< Stop sequences.
    @optional user : string              //!< Optional end-user id for abuse monitoring.
    @optional @embed response_format : string //!< JSON-mode format (see ``json_object_format``).
    @optional tools : array<Tool>        //!< Tools the model may call.
    @optional tool_choice : string       //!< Tool selection ("auto", "none", or a specific name).
}

struct ChatChoice {
    //! One completion choice.
    index : int             //!< Index of this choice.
    message : ChatMessage   //!< The assistant message.
    finish_reason : string  //!< Why generation stopped ("stop", "length", "tool_calls", ...).
}

struct ChatCompletionResponse {
    //! Response body for a non-streaming ``/chat/completions`` call.
    id : string                          //!< Completion id.
    @rename = "object" _object : string  //!< Object type ("chat.completion").
    created : int64                      //!< Unix creation time.
    model : string                       //!< Model that produced the response.
    choices : array<ChatChoice>          //!< Generated choices.
    usage : Usage                        //!< Token accounting.
}

struct ChatResult {
    //! Outcome of ``chat``: when ``ok`` is true ``response`` is valid, else inspect ``error``.
    ok : bool                          //!< True when ``response`` is valid.
    response : ChatCompletionResponse  //!< Decoded response on success.
    error : OpenAIError                //!< Error detail on failure.
}

// ===== Streaming chat types =====

struct ChatMessageDelta {
    //! Incremental message fields in a streamed chunk (object == "chat.completion.chunk").
    @optional role : string     //!< Role, sent on the first delta.
    @optional content : string  //!< Incremental text content.
}

struct ChunkChoice {
    //! One choice within a streamed chunk.
    index : int                     //!< Index of this choice.
    delta : ChatMessageDelta        //!< Incremental message fields for this frame.
    @optional finish_reason : string //!< Set on the final chunk for this choice.
}

struct ChatCompletionChunk {
    //! One streamed delta frame.
    id : string                          //!< Completion id (stable across chunks).
    @rename = "object" _object : string  //!< Object type ("chat.completion.chunk").
    created : int64                      //!< Unix creation time.
    model : string                       //!< Model producing the stream.
    choices : array<ChunkChoice>         //!< Incremental choices in this frame.
}

struct ChatStreamResult {
    //! Outcome of ``chat_stream``: ``content`` is the full accumulated assistant text.
    ok : bool               //!< True on a successful stream.
    content : string        //!< Full accumulated assistant text.
    finish_reason : string  //!< Why generation stopped.
    error : OpenAIError     //!< Error detail on failure.
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
