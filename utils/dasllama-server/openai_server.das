options gen2
options stack = 65536   // the forward/prefill chain + generated kernels need more than the 16K default on arm64
options fast_math = true   // ggml-parity FP laxity (non-bit-exact); inference tool, not the test suite

module openai_server

require dasllama/dasllama       // the public facade — the main dasLLAMA surface this tool touches
require llm_scheduler           // continuous-batching scheduler (sibling — bare require, hyphenated dir)
require dashv/dashv_boost public
require daslib/json_boost
require daslib/defer            // defer() { delete js } — free the parsed request JSON on every exit path
require daslib/jobque_boost
require daslib/strings_boost    // join()
require daslib/fio              // sleep(), path_join, base_name, remove, get_env_variable
require strings
require math
require dasllama/dasllama_audio_io   // load_audio_16k_mono: any uploaded audio format -> 16 kHz mono f32

// An OpenAI-compatible HTTP server over the dasLLAMA facade. It reaches only public facade verbs —
// no engine internals — which is the acceptance test for the API: if a full OpenAI surface builds
// without reaching into the engine, the facade is complete.
// Chat and completion requests batch continuously: handlers parse + render + register and return;
// run_server's loop drives llm_scheduler (one eval_batch decode step over every live stream + one
// bounded prefill chunk per tick) and routes SchedEvents back to each request's writer. Streaming
// and buffered responses both ride the deferred writer ops, so N streams generate concurrently.
// The buffered holdouts (/v1/models, /v1/embeddings, /v1/audio/*, /shutdown, /v1/stats) still run
// to completion in-handler and stall generation for their duration.

// ===== Served state (set once at startup; handlers run serially on the tick thread) =====

var g_model = Model()
var g_model_name = ""
var g_asr = AsrModel()
var g_has_asr = false
var g_should_stop = false
var g_req_counter = 0l
var g_sched = Scheduler()       // live from run_server on; handlers submit into it
var g_events : array<SchedEvent>

// One registered request's HTTP side: where this stream id's SchedEvents go. The writer is owned by
// dasHV's active_writers (respond/close_writer/release_writer drop it) — never deleted from here.
struct private StreamWire {
    @do_not_delete writer : HttpResponseWriter?
    streaming : bool
    is_chat : bool                  // chat.completion.chunk vs text_completion framing
    id : string                     // "chatcmpl-N" / "cmpl-N"
    created : int64
    reply_parts : array<string>     // non-streaming (or has_tools): pieces accumulate until the finished event
    utf8_tail : string              // streaming: held-back bytes of a multi-byte char split across tokens
    has_tools : bool                // chat request declared tools on a tool-capable template
    hold : bool                     // streaming: tool-call opener seen — buffer instead of stream
    held_pieces : int               // pieces buffered since hold engaged (drives keepalive chunks)
    call_open : string              // the family's tool-call markers (copied from the chat template)
    call_close : string
}

var g_wires : table<int64; StreamWire>
var g_default_max_new = 256l

//! Move a loaded model into the server (call once, before run_server). ``name`` is the id reported
//! by ``/v1/models`` and echoed back in responses.
def set_model(var m : Model; name : string) {
    g_model <- m
    g_model_name = name
}

//! Default reply token budget for requests that omit ``max_tokens`` (call before run_server;
//! clamped to the context length per request like an explicit value).
def set_default_max_tokens(n : int64) {
    g_default_max_new = n
}

//! Move a loaded ASR model in, enabling ``/v1/audio/transcriptions`` (call once, before run_server).
def set_asr(var a : AsrModel) {
    g_asr <- a
    g_has_asr = true
}

//! Ask the tick loop to exit at the next iteration (also wired to ``POST /shutdown``).
def request_stop() {
    g_should_stop = true
}

// ===== OpenAI wire shapes (response) =====

// Function-calling shapes: `arguments` is JSON TEXT (the OpenAI wire carries it as a string).
// Public (with parse_tool_calls) for the sibling test.
struct OaiToolCallFn {
    name : string
    arguments : string
}

struct OaiToolCall {
    id : string
    @rename = "type" _type : string
    @rename = "function" _function : OaiToolCallFn   // `function` is a das keyword
}

struct private OaiMessage {
    role : string
    content : string
    @optional tool_calls : array<OaiToolCall>
}

struct private OaiChoice {
    index : int
    message : OaiMessage
    finish_reason : string
}

struct private OaiUsage {
    prompt_tokens : int64
    completion_tokens : int64
    total_tokens : int64
}

struct private OaiChatResponse {
    id : string
    @rename = "object" _object : string
    created : int64
    model : string
    choices : array<OaiChoice>
    usage : OaiUsage
}

// Streaming (stream=true) chunk shapes. @optional so an empty delta emits `{}` and an absent
// finish_reason is dropped (clients read absent as null) — matching OpenAI's chunk framing.
struct private OaiStreamToolCall {
    index : int
    id : string
    @rename = "type" _type : string
    @rename = "function" _function : OaiToolCallFn
}

struct private OaiDelta {
    @optional role : string
    @optional content : string
    @optional tool_calls : array<OaiStreamToolCall>
}

struct private OaiStreamChoice {
    index : int
    delta : OaiDelta
    @optional finish_reason : string
}

struct private OaiStreamChunk {
    id : string
    @rename = "object" _object : string
    created : int64
    model : string
    choices : array<OaiStreamChoice>
}

struct private OaiComplStreamChoice {
    index : int
    text : string
    @optional finish_reason : string
}

struct private OaiComplStreamChunk {
    id : string
    @rename = "object" _object : string
    created : int64
    model : string
    choices : array<OaiComplStreamChoice>
}

struct private OaiComplChoice {
    index : int
    text : string
    finish_reason : string
}

struct private OaiComplResponse {
    id : string
    @rename = "object" _object : string
    created : int64
    model : string
    choices : array<OaiComplChoice>
    usage : OaiUsage
}

struct private OaiModel {
    id : string
    @rename = "object" _object : string
    created : int64
    owned_by : string
}

struct private OaiModelList {
    @rename = "object" _object : string
    data : array<OaiModel>
}

struct private OaiErrDetail {
    message : string
    @rename = "type" _type : string
}

struct private OaiErr {
    error : OaiErrDetail
}

// /v1/audio/transcriptions response shapes.
struct private OaiTranscription {
    text : string
}

struct private OaiVerboseSegment {
    id : int
    start : float
    end : float
    text : string
}

struct private OaiVerboseTranscription {
    task : string
    language : string
    duration : float
    text : string
    segments : array<OaiVerboseSegment>
}

// /v1/embeddings response shapes.
struct private OaiEmbedding {
    @rename = "object" _object : string
    embedding : array<float>
    index : int
}

struct private OaiEmbUsage {
    prompt_tokens : int64
    total_tokens : int64
}

struct private OaiEmbeddingList {
    @rename = "object" _object : string
    data : array<OaiEmbedding>
    model : string
    usage : OaiEmbUsage
}

// ===== Helpers =====

// Stream ids are the request counter: the scheduler keys on the int64, the wire keys the JSON id.
def private next_id() : int64 {
    g_req_counter ++
    return g_req_counter
}

// `created` is a unix-seconds field no OpenAI client validates; a monotone stand-in is honest and
// avoids a wall-clock dependency in the hot path.
def private created_ts() : int64 {
    return 1751200000l + g_req_counter
}

def private error_body(msg, kind : string) : string {
    return sprint_json(OaiErr(error = OaiErrDetail(message = msg, _type = kind)), false)
}

// Terminal error on a streaming route's writer (chat/completions run through the writer).
def private bad_request(srv : WebSocketServer; var writer : HttpResponseWriter?; msg : string) {
    to_log(LOG_WARNING, "dasllama-server: 400 rejected: {msg}\n")
    respond(srv, writer, int(http_status.BAD_REQUEST), "application/json",
            error_body(msg, "invalid_request_error"))
}

// Bytes of an INCOMPLETE trailing UTF-8 sequence (0 = the string ends on a character boundary).
// A token piece can end mid-character (multi-byte chars span BPE tokens); every SSE delta must
// be valid UTF-8 on its own or strict clients (the OpenAI python lib) abort the stream.
def private utf8_hold_bytes(s : string) : int {
    var hold = 0
    peek_data(s) $(arr) {
        let n = length(arr)
        var back = 0
        while (back < 4 && back < n) {
            let b = uint(arr[n - 1 - back])
            if ((b & 0xC0u) != 0x80u) {         // found a non-continuation byte
                if (b >= 0xC0u) {               // multi-byte lead: expected sequence length
                    let need = b >= 0xF0u ? 4 : (b >= 0xE0u ? 3 : 2)
                    if (back + 1 < need) {
                        hold = back + 1         // sequence is cut short — hold it back
                    }
                }
                break                           // ascii / complete / invalid: nothing to hold
            }
            back++
        }
    }
    return hold
}

// One chat.completion.chunk SSE event. role-only for the opening delta, content for each token,
// empty delta + finish_reason for the terminating chunk.
def private send_chat_chunk(srv : WebSocketServer; var writer : HttpResponseWriter?;
                            id : string; created : int64; role, content, finish : string) {
    var chunk = OaiStreamChunk(id = id, _object = "chat.completion.chunk", created = created, model = g_model_name)
    chunk.choices |> emplace(OaiStreamChoice(index = 0, delta <- OaiDelta(role = role, content = content),
                                             finish_reason = finish))
    sse_event(srv, writer, sprint_json(chunk, false), "")
    delete chunk
}

// The whole call set of a streaming reply as ONE tool_calls delta (index enumerates calls per the
// OpenAI incremental framing); the caller follows with the finish_reason="tool_calls" chunk.
def private send_chat_tool_calls_chunk(srv : WebSocketServer; var writer : HttpResponseWriter?;
                                       id : string; created : int64; calls : array<OaiToolCall>) {
    var chunk = OaiStreamChunk(id = id, _object = "chat.completion.chunk", created = created, model = g_model_name)
    var delta = OaiDelta()
    delta.tool_calls |> reserve(length(calls))
    var idx = 0
    for (c in calls) {
        delta.tool_calls |> push(OaiStreamToolCall(index = idx, id = c.id, _type = c._type,
                                                   _function = c._function))
        idx ++
    }
    chunk.choices |> emplace(OaiStreamChoice(index = 0, delta <- delta))
    sse_event(srv, writer, sprint_json(chunk, false), "")
    delete chunk
}

//! Extract Hermes-style tool calls from a finished reply: every well-formed open…close span becomes
//! one call; returns the content BEFORE the first opener (everything when nothing parses, so no
//! reply text is silently dropped). Unparseable call JSON logs and is skipped; an unterminated
//! span logs and ends the scan.
def parse_tool_calls(full : string; open_mark, close_mark, id_base : string;
                     var calls : array<OaiToolCall>) : string {
    var content_end = -1
    var pos = 0
    while (true) {
        let o = find(full, open_mark, pos)
        if (o < 0) {
            break
        }
        if (content_end < 0) {
            content_end = o
        }
        let bstart = o + length(open_mark)
        let cl = find(full, close_mark, bstart)
        if (cl < 0) {
            to_log(LOG_WARNING, "dasllama-server: unterminated {open_mark} span (token budget cut?) — scan stopped\n")
            break
        }
        let body = strip(slice(full, bstart, cl))
        var perr = ""
        var cjs = read_json(body, perr)
        if (cjs == null) {
            to_log(LOG_WARNING, "dasllama-server: unparseable tool-call JSON ({perr}): {body}\n")
        } else {
            let cname = cjs?["name"] ?? ""
            let av = cjs?["arguments"]
            var args = "\{}"
            if (av != null && av.value is _string) {   // non-spec but seen: arguments emitted as a string
                let s = av ?? ""
                args = empty(s) ? "\{}" : s
            } elif (av != null && !(av.value is _null)) {
                args = write_json_compact(av)   // the OpenAI wire carries arguments as compact JSON text
            }
            if (!empty(cname)) {
                calls |> push(OaiToolCall(id = "{id_base}_{length(calls)}", _type = "function",
                                          _function = OaiToolCallFn(name = cname, arguments = args)))
            } else {
                to_log(LOG_WARNING, "dasllama-server: tool call without a name — skipped: {body}\n")
            }
            unsafe { delete cjs }
        }
        pos = cl + length(close_mark)
    }
    return (content_end < 0 || empty(calls)) ? full : strip(slice(full, 0, content_end))
}

def private send_compl_chunk(srv : WebSocketServer; var writer : HttpResponseWriter?;
                             id : string; created : int64; text, finish : string) {
    var chunk = OaiComplStreamChunk(id = id, _object = "text_completion", created = created, model = g_model_name)
    chunk.choices |> push(OaiComplStreamChoice(index = 0, text = text, finish_reason = finish))
    sse_event(srv, writer, sprint_json(chunk, false), "")
    delete chunk
}

// Clamp a requested reply budget to [1, model context].
def private clamp_max(requested : int64) : int64 {
    return clamp(requested, 1l, g_model.config.seq_len)
}

// Error on a buffered (resp-based) route — /v1/models and the audio endpoints use the response path.
def private resp_error(var resp : HttpResponse?; status : http_status; msg : string) : http_status {
    return resp |> JSON(error_body(msg, "invalid_request_error"), status)
}

def private tmpdir() : string {
    var err = ""
    let d = temp_directory(err)   // fio: platform temp dir (Windows GetTempPath, POSIX $TMPDIR/tmp)
    return empty(d) ? "/tmp" : d
}

// ===== Unhandled-request visibility =====

// Top-level fields each JSON route reads; anything else in a request logs a warning instead of
// silently vanishing — the diagnostic for "agent X immediately fails against this server".
let CHAT_FIELDS : table<string> <- { "model", "messages", "stream", "temperature", "top_k", "top_p", "min_p",
                                     "presence_penalty", "frequency_penalty", "repeat_penalty", "seed",
                                     "max_tokens", "max_completion_tokens", "tools", "tool_choice",
                                     "enable_thinking", "chat_template_kwargs" }
let COMPL_FIELDS : table<string> <- { "model", "prompt", "stream", "temperature", "top_k", "top_p", "min_p",
                                     "presence_penalty", "frequency_penalty", "repeat_penalty", "seed",
                                     "max_tokens" }
let EMB_FIELDS : table<string> <- { "model", "input" }

// Sampling knobs shared by both completion routes — OpenAI names, plus the llama.cpp-style
// extensions (top_k / min_p / repeat_penalty). Absent fields keep the greedy defaults.
def private parse_sampling(js : JsonValue?) : SamplingParams {
    var params = SamplingParams()
    params.temp = js?["temperature"] ?? 0.0
    params.top_k = js?["top_k"] ?? 0l
    params.top_p = js?["top_p"] ?? 1.0
    params.min_p = js?["min_p"] ?? 0.0
    params.penalty = js?["repeat_penalty"] ?? 1.0
    params.presence_penalty = js?["presence_penalty"] ?? 0.0
    params.frequency_penalty = js?["frequency_penalty"] ?? 0.0
    return params
}

def private warn_ignored_fields(js : JsonValue?; known : table<string>; route : string) {
    if (js == null || !(js.value is _object)) {
        return
    }
    for (k in keys(js.value as _object)) {
        if (!key_exists(known, k)) {
            to_log(LOG_WARNING, "{route}: ignoring unsupported request field '{k}'\n")
        }
    }
}

// A message's text content: a plain string, or the OpenAI content-parts array (agents send
// [\{type:"text", text:"..."}] even for pure text) — text parts concatenate; other part kinds
// (image_url, ...) log and are skipped.
def private message_text(mv : JsonValue?; route : string) : string {
    let cv = mv?["content"]
    if (cv == null || cv.value is _null) {
        return ""
    }
    if (cv.value is _string) {
        return cv ?? ""
    }
    if (cv.value is _array) {
        var parts : array<string>
        for (pv in (cv.value as _array)) {
            let ptype = pv?["type"] ?? ""
            if (ptype == "text") {
                parts |> push(pv?["text"] ?? "")
            } else {
                to_log(LOG_WARNING, "{route}: skipping non-text content part '{ptype}'\n")
            }
        }
        let s = join(parts, "")
        delete parts
        return s
    }
    to_log(LOG_WARNING, "{route}: message content is neither a string nor a parts array — parsed as empty\n")
    return ""
}

// Unknown endpoint: log the probe (method, path, body head) and 404 with an OpenAI error body.
def private handle_unknown(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
    let body = string(req.body)
    let head = length(body) > 400 ? slice(body, 0, 400) : body
    if (empty(head)) {
        to_log(LOG_WARNING, "unhandled request: {req.method} {req.path}\n")
    } else {
        to_log(LOG_WARNING, "unhandled request: {req.method} {req.path} body: {head}\n")
    }
    return resp_error(resp, http_status.NOT_FOUND, "unknown endpoint: {req.path}")
}

// ===== Handlers =====

def private handle_models(var resp : HttpResponse?) : http_status {
    var list = OaiModelList(_object = "list")
    list.data |> push(OaiModel(id = g_model_name, _object = "model", created = created_ts(), owned_by = "dasllama"))
    if (g_has_asr) {
        list.data |> push(OaiModel(id = "{g_model_name}-asr", _object = "model", created = created_ts(), owned_by = "dasllama"))
    }
    return resp |> JSON(sprint_json(list, false), http_status.OK)
}

// /v1/embeddings: mean-pooled, L2-normalized sentence embeddings over the facade `embed` verb.
// `input` is a string or an array of strings; usage is counted via the facade `encode` (cheap
// relative to the forward). Decoder-only-as-embedder — RAG-grade vectors, not a dedicated model.
def private handle_embeddings(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
    var perr = ""
    var js = read_json(string(req.body), perr)
    if (js == null) {
        return resp_error(resp, http_status.BAD_REQUEST, "invalid JSON: {perr}")
    }
    defer() { unsafe { delete js } }
    warn_ignored_fields(js, EMB_FIELDS, "/v1/embeddings")
    let iv = js?["input"]
    if (iv == null) {
        return resp_error(resp, http_status.BAD_REQUEST, "'input' is required (a string or array of strings)")
    }
    var inputs : array<string>
    if (iv.value is _array) {
        inputs <- from_JV(iv, type<array<string>>)
    } else {
        let single = iv ?? ""
        if (empty(single)) {
            return resp_error(resp, http_status.BAD_REQUEST, "'input' must be a non-empty string or array of strings")
        }
        inputs |> push(single)
    }
    if (empty(inputs)) {
        return resp_error(resp, http_status.BAD_REQUEST, "'input' is empty")
    }

    var out = OaiEmbeddingList(_object = "list", model = g_model_name)
    out.data |> reserve(length(inputs))
    var prompt_tokens = 0l
    var idx = 0
    for (text in inputs) {
        let toks <- encode(g_model, text, true, false)
        prompt_tokens += int64(length(toks))
        var vec = embed(g_model, text)
        out.data |> emplace(OaiEmbedding(_object = "embedding", embedding <- vec, index = idx))
        idx ++
    }
    out.usage = OaiEmbUsage(prompt_tokens = prompt_tokens, total_tokens = prompt_tokens)
    return resp |> JSON(sprint_json(out, false), http_status.OK)
}

// Validate the rendered prompt, submit it to the scheduler, and register the wire that routes its
// SchedEvents back to `writer`. Owns the mandatory 400 (prompt does not fit the context — submit
// would panic) and the 503 (queue full). Moves prompt/stop_ids/close_toks into the request.
def private register_request(srv : WebSocketServer; var writer : HttpResponseWriter?;
                             var prompt, stop_ids, close_toks : array<int64>;
                             params : SamplingParams; max_new : int64;
                             streaming, is_chat : bool; prefix : string;
                             tools_declared : bool = false; call_open : string = ""; call_close : string = "";
                             seed : int = 0) {
    let np = int64(length(prompt))
    if (np + 1l > g_model.config.seq_len) {
        bad_request(srv, writer, "the rendered prompt is {np} tokens; the model context is {g_model.config.seq_len} — shorten the conversation or raise --ctx")
        return
    }
    let rid = next_id()
    let cid = "{prefix}-{rid}"
    let created = created_ts()
    var preq <- PendingReq(id = rid, params = params, max_new = max_new, seed = seed)
    preq.prompt <- prompt
    preq.stop_ids <- stop_ids
    preq.close_toks <- close_toks
    if (!submit(g_model, g_sched, preq)) {
        delete preq
        to_log(LOG_WARNING, "dasllama-server: 503 queue full ({g_sched.max_queue}) — rejected {prefix} request ({np} tok)\n")
        respond(srv, writer, int(http_status.SERVICE_UNAVAILABLE), "application/json",
                error_body("the server is at capacity ({g_sched.max_queue} requests queued) — retry shortly", "server_error"))
        return
    }
    to_log(LOG_INFO, "[req {rid}] {prefix}: prompt={np} tok, stream={streaming}, max_new={max_new}\n")
    var wire <- StreamWire(writer = writer, streaming = streaming, is_chat = is_chat,
                           id = cid, created = created,
                           has_tools = tools_declared && !empty(call_open),
                           call_open = call_open, call_close = call_close)
    g_wires[rid] <- wire
    if (streaming && is_chat) {
        send_chat_chunk(srv, writer, cid, created, "assistant", "", "")   // opening role delta
    }
}

// Parse + validate + render + register, then return — no generation in the handler. The prompt is
// rendered through the S1 render seam (tokens only, no KV), so a queued request holds no cache
// memory; the session exists only once the scheduler admits the stream.
def private handle_chat(srv : WebSocketServer; var req : HttpRequest?; var writer : HttpResponseWriter?) {
    var perr = ""
    var js = read_json(string(req.body), perr)
    if (js == null) {
        bad_request(srv, writer, "invalid JSON: {perr}")
        return
    }
    defer() { unsafe { delete js } }
    warn_ignored_fields(js, CHAT_FIELDS, "/v1/chat/completions")
    let msgs_v = js?["messages"]
    if (msgs_v == null || !(msgs_v.value is _array)) {
        bad_request(srv, writer, "'messages' must be an array")
        return
    }
    let streaming = js?["stream"] ?? false
    let params = parse_sampling(js)
    let seed = js?["seed"] ?? 0
    var req_max = js?["max_tokens"] ?? 0l
    if (req_max <= 0l) {
        req_max = js?["max_completion_tokens"] ?? g_default_max_new   // the newer OpenAI spelling
    }
    let max_new = clamp_max(req_max)

    // tools: each tools[] entry re-serialized verbatim into the system turn's tool block.
    // tool_choice: "none" drops them; the object form (forced function) is not honored.
    var tools : array<string>
    defer() { delete tools }
    let tools_v = js?["tools"]
    if (tools_v != null && !(tools_v.value is _null)) {
        if (!(tools_v.value is _array)) {
            bad_request(srv, writer, "'tools' must be an array of tool definitions")
            return
        }
        tools |> reserve(length(tools_v.value as _array))
        for (tv in (tools_v.value as _array)) {
            tools |> push(write_json_compact(tv))   // the template's one-tool-per-line tojson shape
        }
    }
    let tcv = js?["tool_choice"]
    if (tcv != null && !(tcv.value is _null)) {   // ?[] yields the null SENTINEL for a missing key
        if (tcv.value is _string) {
            let choice = tcv ?? ""
            if (choice == "none") {
                tools |> clear()
            } elif (choice != "auto" && choice != "required") {
                to_log(LOG_WARNING, "/v1/chat/completions: unknown tool_choice '{choice}' — treating as auto\n")
            }
        } else {
            to_log(LOG_WARNING, "/v1/chat/completions: forced tool_choice is not honored — treating as auto\n")
        }
    }

    // OpenAI is stateless — the client resends the whole transcript. Fold system turns, replay
    // prior user/assistant/tool turns through the renderer, then render the trailing turn + the
    // generation prompt.
    var system = ""
    for (mv in (msgs_v.value as _array)) {
        if ((mv?["role"] ?? "") == "system") {
            let sc = message_text(mv, "/v1/chat/completions")
            system = empty(system) ? sc : "{system}\n{sc}"
        }
    }
    var chat <- create_chat_renderer(g_model, system, max_new)
    defer() { delete chat }
    // top-level enable_thinking, or the llama.cpp-server spelling chat_template_kwargs.enable_thinking;
    // other kwargs keys warn like any unsupported field instead of vanishing
    let ctk = js?["chat_template_kwargs"]
    if (ctk != null && ctk.value is _object) {
        for (k in keys(ctk.value as _object)) {
            if (k != "enable_thinking") {
                to_log(LOG_WARNING, "/v1/chat/completions: ignoring unsupported chat_template_kwargs key '{k}'\n")
            }
        }
    } elif (ctk != null && !(ctk.value is _null)) {
        to_log(LOG_WARNING, "/v1/chat/completions: 'chat_template_kwargs' must be an object — ignoring\n")
    }
    var think = js?["enable_thinking"] ?? true
    think = think && (ctk?["enable_thinking"] ?? true)
    if (!think) {
        set_thinking(chat, false)
    }
    let has_tools = !empty(tools)
    if (has_tools) {
        if (empty(chat.tmpl.tool_call_open)) {
            bad_request(srv, writer, "'tools' is not supported: the {g_model_name} chat template declares no tool-call format")
            return
        }
        set_tools(chat, tools)   // moved in; `tools` is empty from here
    }
    var prompt : array<int64>
    defer() { delete prompt }
    var pending = false
    var tool_results : array<string>
    defer() { delete tool_results }
    for (mv in (msgs_v.value as _array)) {
        let role = mv?["role"] ?? ""
        let content = message_text(mv, "/v1/chat/completions")
        if (role != "tool" && !empty(tool_results)) {
            add_tool_results(chat, tool_results)   // flush the consecutive tool-result run
            tool_results |> clear()
            pending = true
        }
        if (role == "user") {
            add_user(chat, content)
            pending = true
        } elif (role == "assistant") {
            // rebuild any tool calls this turn made as the {"name","arguments"} objects the
            // template renders (the wire carries arguments as JSON text — spliced verbatim)
            var calls : array<string>
            let tc = mv?["tool_calls"]
            if (tc != null && tc.value is _array) {
                calls |> reserve(length(tc.value as _array))
                for (tcall in (tc.value as _array)) {
                    let fv = tcall?["function"]
                    var nv = JV(fv?["name"] ?? "")   // JSON-escape the name (quotes/backslashes)
                    let cname = write_json_compact(nv)
                    unsafe {
                        delete nv
                    }
                    let av = fv?["arguments"]
                    var argstr = "\{}"
                    if (av != null && av.value is _string) {
                        let s = av ?? ""
                        if (!empty(s)) {
                            argstr = s
                        }
                    } elif (av != null && !(av.value is _null)) {
                        argstr = write_json_compact(av)   // tolerated: arguments sent as an object
                    }
                    calls |> push("\{\"name\": {cname}, \"arguments\": {argstr}}")
                }
            }
            if (pending) {
                if (!empty(calls)) {
                    render_assistant_calls(g_model, chat, content, calls, prompt)
                } else {
                    render_assistant(g_model, chat, content, prompt)
                }
                pending = false
            }
            delete calls
        } elif (role == "tool") {
            tool_results |> push(content)
        } elif (role != "system") {
            to_log(LOG_WARNING, "/v1/chat/completions: dropping message with unsupported role '{role}'\n")
        }
    }
    if (!empty(tool_results)) {
        add_tool_results(chat, tool_results)   // trailing tool results: generation continues from them
        pending = true
    }
    if (!pending) {
        bad_request(srv, writer, "the last message must be a user or tool message")
        return
    }
    var turn <- render_turn(g_model, chat)
    prompt |> push_from(turn)
    delete turn
    var close <- render_close(g_model, chat)
    defer() { delete close }   // register_request moves it on success; the 400/503 paths leave it here
    register_request(srv, writer, prompt, chat.stop_ids, close, params, max_new, streaming, true, "chatcmpl",
                     has_tools, chat.tmpl.tool_call_open, chat.tmpl.tool_call_close, seed)
}

def private handle_completion(srv : WebSocketServer; var req : HttpRequest?; var writer : HttpResponseWriter?) {
    var perr = ""
    var js = read_json(string(req.body), perr)
    if (js == null) {
        bad_request(srv, writer, "invalid JSON: {perr}")
        return
    }
    defer() { unsafe { delete js } }
    warn_ignored_fields(js, COMPL_FIELDS, "/v1/completions")
    let prompt = js?["prompt"] ?? ""
    if (empty(prompt)) {
        bad_request(srv, writer, "'prompt' must be a non-empty string")
        return
    }
    let streaming = js?["stream"] ?? false
    let params = parse_sampling(js)
    let seed = js?["seed"] ?? 0
    let max_new = clamp_max(js?["max_tokens"] ?? g_default_max_new)
    var toks <- encode(g_model, prompt, true, false)
    defer() { delete toks }
    // no stop ids and no close: raw completions run to the budget, matching bare generate()
    var none_stop : array<int64>
    var none_close : array<int64>
    register_request(srv, writer, toks, none_stop, none_close, params, max_new, streaming, false, "cmpl",
                     [seed = seed])
}

// Evict streams whose client vanished: is_writer_connected reads false once the connection is
// gone (the async writer ops themselves never report a dead peer). The scheduler appends the
// "evicted" finished event; flush_events below releases the writer and drops the wire.
def private reap_disconnected(srv : WebSocketServer) {
    for (id, w in keys(g_wires), values(g_wires)) {
        if (!is_writer_connected(srv, w.writer)) {
            evict(g_sched, id, g_events)
        }
    }
}

// Route drained SchedEvents to their writers: streaming wires emit SSE chunks, buffered wires
// accumulate pieces and respond once at finish. "evicted" releases the writer with no bytes —
// the client is gone (reap_disconnected produced it). Finished wires unregister.
def private flush_events(srv : WebSocketServer) {
    for (e in g_events) {
        // interior pointer into g_wires — nothing inserts into the table between here and the erase
        var w = unsafe(g_wires?[e.stream])
        if (w == null) {
            continue
        }
        if (e.kind == SchedEventKind.piece) {
            if (!w.streaming || w.has_tools) {
                w.reply_parts |> push(e.text)   // buffered reply, and/or the finish-time tool-call parse
            }
            if (w.streaming && w.hold) {
                // a long tool-call body buffers for minutes at decode speed; with no bytes on the
                // wire the client's idle timeout kills the stream (pi: "terminated" + retry loop).
                // An empty delta {} is protocol-legal liveness every ~16 tokens.
                w.held_pieces ++
                if (w.held_pieces % 16 == 0) {
                    send_chat_chunk(srv, w.writer, w.id, w.created, "", "", "")
                }
            } elif (w.streaming) {
                // re-join any held-back partial character, then hold the new incomplete tail
                // so every delta on the wire is valid UTF-8 by itself
                let whole = "{w.utf8_tail}{e.text}"
                if (w.has_tools) {
                    // the ChatML tool-call opener is one vocab token, so per-piece detection is
                    // exact; from the opener on the reply is tool-call body — buffer, don't stream
                    let o = find(whole, w.call_open)
                    if (o >= 0) {
                        w.hold = true
                        w.utf8_tail = ""
                        to_log(LOG_INFO, "stream {w.id}: tool-call opener — buffering the call body (keepalives on)\n")
                        let head = slice(whole, 0, o)   // the opener starts on a char boundary (ascii)
                        if (!empty(head)) {
                            send_chat_chunk(srv, w.writer, w.id, w.created, "", head, "")
                        }
                        continue
                    }
                }
                let hold = utf8_hold_bytes(whole)
                let cut = length(whole) - hold
                let emit_now = hold > 0 ? slice(whole, 0, cut) : whole
                w.utf8_tail = hold > 0 ? slice(whole, cut) : ""
                if (!empty(emit_now)) {
                    if (w.is_chat) {
                        send_chat_chunk(srv, w.writer, w.id, w.created, "", emit_now, "")
                    } else {
                        send_compl_chunk(srv, w.writer, w.id, w.created, emit_now, "")
                    }
                }
            }
            continue
        }
        if (e.finish == "evicted") {
            release_writer(srv, w.writer)
        } elif (w.streaming) {
            if (!empty(w.utf8_tail)) {
                // generation ended mid-character (max-tokens cut): the held bytes can never
                // complete, and emitting them would put invalid UTF-8 on the wire — drop loudly
                to_log(LOG_WARNING, "dasllama-server: stream {w.id} ended mid-UTF-8-sequence; dropped {length(w.utf8_tail)} byte(s)\n")
            }
            var finish = e.finish
            if (w.hold) {
                // the buffered tail is tool-call body: parse it into a tool_calls delta (the
                // pre-opener content already streamed); nothing parseable => release it as text
                let full = join(w.reply_parts, "")
                var calls : array<OaiToolCall>
                parse_tool_calls(full, w.call_open, w.call_close, "call_{e.stream}", calls)
                if (!empty(calls)) {
                    send_chat_tool_calls_chunk(srv, w.writer, w.id, w.created, calls)
                    finish = "tool_calls"
                } else {
                    send_chat_chunk(srv, w.writer, w.id, w.created, "", slice(full, find(full, w.call_open)), "")
                }
                delete calls
            }
            if (w.is_chat) {
                send_chat_chunk(srv, w.writer, w.id, w.created, "", "", finish)
            } else {
                send_compl_chunk(srv, w.writer, w.id, w.created, "", finish)
            }
            sse_event(srv, w.writer, "[DONE]", "")
            close_writer(srv, w.writer)
        } elif (w.is_chat) {
            let full = join(w.reply_parts, "")
            var content = full
            var finish = e.finish
            var calls : array<OaiToolCall>
            if (w.has_tools && find(full, w.call_open) >= 0) {
                content = parse_tool_calls(full, w.call_open, w.call_close, "call_{e.stream}", calls)
                if (!empty(calls)) {
                    finish = "tool_calls"
                }
            }
            var out = OaiChatResponse(id = w.id, _object = "chat.completion", created = w.created, model = g_model_name)
            out.choices |> emplace(OaiChoice(index = 0,
                                             message <- OaiMessage(role = "assistant", content = content,
                                                                   tool_calls <- calls),
                                             finish_reason = finish))
            out.usage = OaiUsage(prompt_tokens = e.n_prompt, completion_tokens = e.n_gen,
                                 total_tokens = e.n_prompt + e.n_gen)
            respond(srv, w.writer, int(http_status.OK), "application/json", sprint_json(out, false))
            delete out
        } else {
            var out = OaiComplResponse(id = w.id, _object = "text_completion", created = w.created, model = g_model_name)
            out.choices |> push(OaiComplChoice(index = 0, text = join(w.reply_parts, ""), finish_reason = e.finish))
            out.usage = OaiUsage(prompt_tokens = e.n_prompt, completion_tokens = e.n_gen,
                                 total_tokens = e.n_prompt + e.n_gen)
            respond(srv, w.writer, int(http_status.OK), "application/json", sprint_json(out, false))
            delete out
        }
        let tps = e.gen_us > 0l ? int64(double(e.n_gen) * 1.0e6lf / double(e.gen_us)) : 0l
        to_log(LOG_INFO, "stream {w.id}: finish={e.finish} prompt={e.n_prompt} cached={e.n_cached} gen={e.n_gen} ttft={e.ttft_us / 1000l}ms tps={tps}\n")
        delete g_wires[e.stream]   // finalize the wire in place (erase is a raw remove)
        g_wires |> erase(e.stream)
    }
    g_events |> clear()
}

// /v1/stats: the scheduler's counters as JSON (buffered — cheap enough to run in-handler).
struct private OaiStatsResponse {
    active : int64
    queued : int64
    peak_active : int64
    admitted : int64
    finished : int64
    evicted : int64
    decode_steps : int64
    gen_tokens : int64
    avg_batch : float
    cached_tokens : int64
    prefix_pages : int64
    max_streams : int64
    chunk_tokens : int64
    mtp_drafted : int64
    mtp_accepted : int64
}

def private handle_stats(var resp : HttpResponse?) : http_status {
    let out = OaiStatsResponse(
        active = n_active(g_sched), queued = n_queued(g_sched), peak_active = g_sched.peak_active,
        admitted = g_sched.n_admitted, finished = g_sched.n_finished, evicted = g_sched.n_evicted,
        decode_steps = g_sched.decode_steps, gen_tokens = g_sched.gen_tokens,
        avg_batch = float(g_sched.sum_batch) / float(max(g_sched.decode_steps, 1l)),
        cached_tokens = g_sched.cached_tokens, prefix_pages = prefix_held_groups(g_sched.prefix),
        max_streams = g_sched.max_streams, chunk_tokens = g_sched.chunk_tokens,
        mtp_drafted = g_sched.mtp_drafted, mtp_accepted = g_sched.mtp_accepted)
    return resp |> JSON(sprint_json(out, false), http_status.OK)
}

// /v1/audio/transcriptions (translate=false) and /v1/audio/translations (translate=true, to English).
// Multipart upload: save the 'file' part to disk (binary-safe), decode to 16 kHz mono f32, transcribe.
def private handle_transcription(var req : HttpRequest?; var resp : HttpResponse?; translate : bool) : http_status {
    if (!g_has_asr) {
        return resp_error(resp, http_status.NOT_FOUND, "no ASR model loaded — start the server with --asr")
    }
    var fname = ""
    each_form_field(req) $(name, _content, filename) {
        if (name == "file" && !empty(filename)) {
            fname = filename
        }
    }
    if (empty(fname)) {
        return resp_error(resp, http_status.BAD_REQUEST, "multipart form must include a 'file' audio upload")
    }
    let temppath = path_join(tmpdir(), "dasllama_upload_{base_name(fname)}")
    save_form_file(req, "file", temppath)
    let samples <- load_audio_16k_mono(temppath)
    remove(temppath)
    if (empty(samples)) {
        return resp_error(resp, http_status.BAD_REQUEST, "could not decode the uploaded audio")
    }
    let fmt = get_form_data(req, "response_format")
    var lang = get_form_data(req, "language")
    if (empty(lang)) {
        lang = "en"
    }
    var session <- create_session(g_asr, lang)
    session.translate = translate
    if (fmt == "verbose_json") {
        var vt = OaiVerboseTranscription(task = translate ? "translate" : "transcribe",
                                         language = lang, duration = float(length(samples)) / 16000.0)
        var full : array<string>
        var sid = 0
        transcribe(g_asr, session, samples) $(seg) {
            vt.segments |> push(OaiVerboseSegment(id = sid, start = float(seg.t0) / 100.0,
                                                  end = float(seg.t1) / 100.0, text = seg.text))
            full |> push(seg.text)
            sid ++
        }
        vt.text = join(full, "")
        vt.language = session.detected_lang
        return resp |> JSON(sprint_json(vt, false), http_status.OK)
    }
    let text = transcribe(g_asr, session, samples)
    if (fmt == "text") {
        return resp |> TEXT_PLAIN(text)
    }
    return resp |> JSON(sprint_json(OaiTranscription(text = text), false), http_status.OK)
}

// ===== Server =====

class OpenAiServer : HvWebServer {
    def override onInit {
        allow_cors()
        GET("/v1/models") <| @(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
            return handle_models(resp)
        }
        STREAM("/v1/chat/completions") <| @(var req : HttpRequest?; var writer : HttpResponseWriter?) {
            handle_chat(server, req, writer)
        }
        STREAM("/v1/completions") <| @(var req : HttpRequest?; var writer : HttpResponseWriter?) {
            handle_completion(server, req, writer)
        }
        POST("/v1/audio/transcriptions") <| @(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
            return handle_transcription(req, resp, false)
        }
        POST("/v1/audio/translations") <| @(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
            return handle_transcription(req, resp, true)
        }
        POST("/v1/embeddings") <| @(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
            return handle_embeddings(req, resp)
        }
        GET("/v1/stats") <| @(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
            return handle_stats(resp)
        }
        POST("/shutdown") <| @(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
            g_should_stop = true
            return resp |> JSON("\{\"ok\":true}", http_status.OK)
        }
        ANY("*") <| @(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
            return handle_unknown(req, resp)
        }
    }
}

//! Run the server on the calling thread until ``request_stop`` (or ``POST /shutdown``). Sets up the
//! job queue, then drives the serve loop: pump HTTP (handlers parse + render + submit), evict
//! streams whose client disconnected, one scheduler step (continuous batching over every live
//! stream), flush events to the writers.
//! ``max_streams`` concurrent generation streams, ``max_queue`` waiting requests (503 beyond),
//! ``chunk_tokens`` the prefill quantum (decode stalls at most this many tokens per tick).
//! ``page_rows`` > 0 (the default) serves PAGED: KV memory tracks actual context and a prefix
//! cache reuses completed streams' pages, so repeated prompts (system prompts, multi-turn chats)
//! skip their prefill; ``prefix_groups`` caps the cache (0 = auto: one full context per stream
//! slot, < 0 = unbounded). ``page_rows`` = 0 restores flat preallocated sessions (each the full
//! ``--ctx`` slab). ``kv_dtype`` picks the KV-cache codec for every stream (pool-wide when paged;
//! see ``KVDtype`` — ``tq4`` needs a power-of-two head_size). Call after ``set_model``.
def run_server(port : int; max_streams : int64 = 4l; max_queue : int64 = 32l; chunk_tokens : int64 = 64l;
               page_rows : int64 = 64l; prefix_groups : int64 = 0l; kv_dtype : KVDtype = KVDtype.f16;
               mtp : bool = false) {
    g_should_stop = false   // reset so a restart in the same context after /shutdown doesn't exit at once
    with_job_que() {
        setup_dasllama_jobque()
        var pfx = prefix_groups
        if (page_rows > 0l && pfx == 0l) {   // auto: retain up to one full context per stream slot
            pfx = max_streams * ((g_model.config.seq_len + page_rows - 1l) / page_rows)
        }
        if (pfx < 0l) {
            pfx = 0l   // explicit unbounded
        }
        var spec = mtp
        if (spec && g_model.config.n_layer_nextn <= 0l) {
            to_log(LOG_WARNING, "dasllama-server: --mtp requested but the model has no NextN head — self-spec stays off\n")
            spec = false
        }
        let prev_spec = get_mtp_spec()
        set_mtp_spec(spec)   // engine gate: prefills maintain the draft head's KV (chunked included); restored at exit
        g_sched <- create_scheduler(g_model, max_streams, max_queue, chunk_tokens, page_rows, pfx, kv_dtype, spec)
        var server = new OpenAiServer()
        server->init(port)
        let rc = server->start()   // 0 on success; non-zero = bind/listen failed (e.g. port in use)
        if (rc != 0) {
            to_log(LOG_ERROR, "dasllama-server: could not start on port {port} (start rc {rc}; is the port already in use?)\n")
        } else {
            let pfx_desc = pfx > 0l ? "{pfx}" : "unbounded"
            let kv_mode = page_rows > 0l ? "{kv_dtype} paged P={page_rows}, prefix cache {pfx_desc} pages" : "{kv_dtype} flat"
            let spec_desc = spec ? ", mtp: on" : ""
            to_log(LOG_INFO, "dasllama-server listening on http://127.0.0.1:{port}/v1  (model: {g_model_name}, streams: {max_streams}, prefill chunk: {chunk_tokens}, kv: {kv_mode}{spec_desc})\n")
            while (!g_should_stop) {
                server->tick()   // drains queued handlers: requests parse + render + submit here
                reap_disconnected(server.server)
                let t_step = ref_time_ticks()
                let busy = scheduler_step(g_model, g_sched, g_events)
                let step_ms = int64(get_time_usec(t_step)) / 1000l
                if (step_ms > 3000l) {
                    // the "why does it pause" catcher: one decode step / prefill chunk should be
                    // well under a second — anything slower means contention or a stall
                    to_log(LOG_WARNING, "dasllama-server: slow scheduler step: {step_ms}ms (active={n_active(g_sched)}, queued={n_queued(g_sched)})\n")
                }
                flush_events(server.server)
                if (!busy) {
                    sleep(2u)
                }
            }
            server->stop()
        }
        server->cleanup()
        unsafe {
            delete server
        }
        unsafe {
            delete g_sched   // live sessions + workspace go with it (writers already died with the server)
        }
        delete g_events
        delete g_wires
        set_mtp_spec(prev_spec)   // a /shutdown + restart in the same context starts from a clean gate
    }
}
