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

struct StreamOptions {
    //! ``stream_options`` of a streamed request.
    include_usage : bool   //!< Ask the server to end the stream on a chunk with no choices carrying ``usage``.
}

struct ChatCompletionRequest {
    //! Request body for ``/chat/completions``. Default-valued optional fields are omitted on send.
    @optional model : string             //!< Chat model id; empty = the server's own default model (local servers).
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
    @optional stream_options : StreamOptions  //!< ``include_usage``: a streamed reply ends on a usage chunk (``ChatStreamResult.usage``).
    @optional @embed chat_template_kwargs : string //!< Raw JSON for the server's chat template, e.g. ``{"enable_thinking":false}`` (llama.cpp-family servers).
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

struct ToolCallFunctionDelta {
    //! Incremental function fields in a streamed tool-call delta. ``arguments`` accretes across frames.
    @optional name : string       //!< Function name (sent once, on the first frame for this call).
    @optional arguments : string  //!< Partial JSON argument text; concatenate across frames in order.
}

struct ToolCallDelta {
    //! One streamed tool-call fragment. Fragments with the same ``index`` reassemble into one ``ToolCall``.
    index : int                                            //!< Which tool call this fragment belongs to.
    @optional id : string                                  //!< Call id (sent once, on the first frame).
    @optional @rename = "type" _type : string             //!< Tool type ("function"), sent once.
    @optional @rename = "function" _function : ToolCallFunctionDelta //!< Incremental function name / arguments.
}

struct ChatMessageDelta {
    //! Incremental message fields in a streamed chunk (object == "chat.completion.chunk").
    @optional role : string                    //!< Role, sent on the first delta.
    @optional content : string                 //!< Incremental text content.
    @optional reasoning_content : string       //!< Incremental reasoning text (a thinking model's channel).
    @optional tool_calls : array<ToolCallDelta> //!< Incremental tool-call fragments (reassembled by ``chat_stream``).
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
    choices : array<ChunkChoice>         //!< Incremental choices in this frame; empty on the usage chunk.
    @optional usage : Usage              //!< Token accounting, on the chunk ``stream_options.include_usage`` asks for.
    @optional timings : StreamTimings    //!< The server's own clock for the turn, when it reports one (llama.cpp-family servers).
}

struct StreamTimings {
    //! A server's own timing of a streamed turn, in milliseconds.
    @optional ttft_ms : float   //!< Admission to the first token.
    @optional gen_ms : float    //!< The first token to the finish.
}

struct ChatStreamResult {
    //! Outcome of ``chat_stream``: ``content`` is the full accumulated assistant text.
    ok : bool                       //!< True on a successful stream.
    content : string                //!< Full accumulated assistant text.
    reasoning : string              //!< Full accumulated reasoning text (empty from a model with no thinking channel).
    finish_reason : string          //!< Why generation stopped ("stop", "tool_calls", ...).
    tool_calls : array<ToolCall>    //!< Reassembled tool calls (empty unless the model streamed any).
    usage : Usage                   //!< The usage chunk's counts (``prompt_tokens`` 0 when no chunk carried them).
    timings : StreamTimings         //!< The server's own clock from that chunk, zeros when it reports none.
    error : OpenAIError             //!< Error detail on failure.
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

// Upper bound on a streamed tool-call index — guards against a malformed/hostile
// server forcing unbounded allocation by sending a huge index. Real responses
// carry a handful of tool calls; this is far above any legitimate count.
let private MAX_STREAM_TOOL_CALLS = 256

def chat_stream(client : OpenAIClient; var req : ChatCompletionRequest; // nolint:STYLE038 — one SSE stream lifecycle: request setup, per-chunk line framing, completion
                on_delta : block<(delta : string) : void>) : ChatStreamResult {
    //! Streams a ``/chat/completions`` response, invoking ``on_delta`` with each text
    //! increment as it arrives. Returns the full accumulated content plus status.
    //!
    //! Text deltas (``choices[0].delta.content``) are accumulated into ``result.content`` and reasoning
    //! deltas (``reasoning_content``) into ``result.reasoning``, ``on_delta`` firing for either as it arrives;
    //! streamed tool-call fragments are reassembled by ``index`` into ``result.tool_calls``; a chunk
    //! carrying ``usage`` (``stream_options.include_usage``) fills ``result.usage`` and ``result.timings``.
    //! Tolerates CRLF line endings and SSE comment lines.
    req.stream = true
    let body_json = sprint_json(req, false)
    let url = "{client.base_url}/chat/completions"

    var result = ChatStreamResult(usage = Usage(prompt_tokens_details = PromptTokensDetails(cached_tokens = -1)))
    var buffer = ""
    var parts : array<string>   // accumulate deltas, join once at the end (avoid O(n^2) string growth)
    var thoughts : array<string>
    var tools : array<ToolCall> // reassembled per-index from streamed tool-call fragments
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
                // one peek_data per chunk: every line is framed inside the view, so no
                // per-line string is materialized and the buffer is compacted once
                var consumed = 0
                peek_data(buffer) $(bytes) {
                    var nl = find(bytes, '\n', consumed)
                    while (nl >= 0) {
                        // tolerate CRLF line endings: drop a trailing '\r' so payload + [DONE] parse cleanly
                        var line_end = nl
                        if (line_end > consumed && int(bytes[line_end - 1]) == '\r') {
                            line_end --
                        }
                        if (starts_with(bytes, consumed, "data: ")) {
                            let payload = slice(bytes, consumed + 6, line_end)
                            if (payload != "[DONE]") {
                                // -1 survives a usage chunk with no details: the server reported no cache hit
                                var frame = ChatCompletionChunk(usage = Usage(prompt_tokens_details = PromptTokensDetails(cached_tokens = -1)))
                                let parsed = sscan_json(payload, frame)
                                if (parsed && frame.usage.prompt_tokens > 0) {
                                    result.usage = frame.usage
                                    result.timings = frame.timings
                                }
                                if (parsed && !empty(frame.choices)) {
                                    let delta = frame.choices[0].delta.content
                                    if (!empty(delta)) {
                                        parts |> push(delta)
                                        invoke(on_delta, delta)
                                    }
                                    let thought = frame.choices[0].delta.reasoning_content
                                    if (!empty(thought)) {
                                        thoughts |> push(thought)
                                        invoke(on_delta, thought)
                                    }
                                    for (tcd in frame.choices[0].delta.tool_calls) {
                                        let idx = tcd.index
                                        // ignore out-of-range indices: negative would underflow the array,
                                        // an absurdly large one would force unbounded allocation
                                        continue if (idx < 0 || idx >= MAX_STREAM_TOOL_CALLS)
                                        while (length(tools) <= idx) {
                                            tools |> emplace(ToolCall())
                                        }
                                        if (!empty(tcd.id)) { tools[idx].id = tcd.id }
                                        if (!empty(tcd._type)) { tools[idx]._type = tcd._type }
                                        if (!empty(tcd._function.name)) { tools[idx]._function.name = tcd._function.name }
                                        tools[idx]._function.arguments += tcd._function.arguments
                                    }
                                    if (!empty(frame.choices[0].finish_reason)) {
                                        finish = frame.choices[0].finish_reason
                                    }
                                }
                            }
                        }
                        consumed = nl + 1
                        nl = find(bytes, '\n', consumed)
                    }
                }
                if (consumed > 0) {
                    buffer = slice(buffer, consumed)
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
    result.reasoning = join(thoughts, "")
    delete parts
    delete thoughts
    result.tool_calls <- tools
    result.finish_reason = finish
    return <- result
}
