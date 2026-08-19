options gen2
options _comment_hygiene = true
options _dasllama_internal = true

module dasllama_tools shared public

require dasllama/dasllama_lint public
require daslib/json_boost   // tool defs arrive as OpenAI JSON; non-JSON wire args normalize to JSON text
require daslib/strings_boost
require strings

// The per-ToolMode wire codecs: definition serializers (prompt side) and reply parsers (wire
// side), all pure string+JSON functions — model-free testable. The chat layer renders through
// the serializers; the server parses through the parsers. Formats verified verbatim against the
// GGUF-embedded Jinja templates and llama.cpp's PEG handlers (see tool_formats_plan.md).

//! One parsed tool call: the function name, its arguments normalized to JSON object text, and
//! the wire id where the format carries one (mistral; "" elsewhere).
struct ToolCall {
    name : string = ""
    args : string = ""
    id : string = ""
}

//! A fully classified tool-capable reply: the reasoning span, the content, and the calls.
//! Harmony fills all three in one walk (calls ride the channel structure); the marker formats
//! fill content+calls with reasoning split beforehand by the think matcher.
struct ToolReply {
    reasoning : string = ""
    content : string = ""
    calls : array<ToolCall>
}

// ===== shared JSON helpers =====

// The `function` object of one OpenAI tools[] entry (tolerates a bare function object too).
def private tool_function_of(v : JsonValue?) : JsonValue const? {
    let f = v?["function"]
    return f != null && f.value is _object ? f : v
}

// ===== Harmony (gpt-oss) — TypeScript-namespace defs, channel-structure calls =====

// One parameter's TS type per the canonical template's mapping (unions/oneOf and unknown
// shapes fall to `any`; `nullable` appends `| null`).
def private harmony_ts_type(schema : JsonValue?) : string {
    if (schema == null) {
        return "any"
    }
    let t = schema?["type"] ?? ""
    var base = "any"
    if (t == "string") {
        let en = schema?["enum"]
        if (en != null && en.value is _array) {
            var parts : array<string>
            parts |> reserve(length(en.value as _array))
            for (e in (en.value as _array)) {
                let ev = e ?? ""
                parts |> push("\"{ev}\"")
            }
            base = join(parts, " | ")
            delete parts
        } else {
            base = "string"
        }
    } elif (t == "integer" || t == "number") {
        base = "number"
    } elif (t == "boolean") {
        base = "boolean"
    } elif (t == "array") {
        let it = schema?["items"]
        let itype = it != null ? harmony_ts_type(it) : "any"
        base = (itype == "string" || itype == "number" || itype == "boolean") ? "{itype}[]" : "any[]"
    } elif (t == "object") {
        let props = schema?["properties"]
        if (props != null && props.value is _object) {
            var req : table<string>
            let rv = schema?["required"]
            if (rv != null && rv.value is _array) {
                for (r in (rv.value as _array)) {
                    req |> insert(r ?? "")
                }
            }
            var parts : array<string>
            for (k, pv in keys(props.value as _object), values(props.value as _object)) {
                let opt = key_exists(req, k) ? "" : "?"
                let pt = harmony_ts_type(pv)
                parts |> push("{k}{opt}: {pt}")
            }
            let joined = join(parts, ", ")
            base = "\{{joined}}"
            delete parts
            delete req
        }
    }
    if ((schema?["nullable"] ?? false)) {
        base = "{base} | null"
    }
    return base
}

//! The `# Tools` developer-turn block for Harmony: the canonical TypeScript `functions`
//! namespace (trailing commas included — the template emits one after the LAST parameter too).
def harmony_tools_namespace(tools_json : array<string>) : string {
    return build_string() $(w) {
        w |> write("## functions\n\nnamespace functions \{\n\n")
        for (tj in tools_json) {
            var perr = ""
            var tv = read_json(tj, perr)
            if (tv == null) {
                continue
            }
            let fv = tool_function_of(tv)
            let name = fv?["name"] ?? ""
            let desc = fv?["description"] ?? ""
            if (!empty(desc)) {
                w |> write("// {desc}\n")
            }
            let params = fv?["parameters"]
            let props = params?["properties"]
            if (props == null || !(props.value is _object) || empty(props.value as _object)) {
                w |> write("type {name} = () => any;\n\n")
            } else {
                w |> write("type {name} = (_: \{\n")
                var req : table<string>
                let rv = params?["required"]
                if (rv != null && rv.value is _array) {
                    for (r in (rv.value as _array)) {
                        req |> insert(r ?? "")
                    }
                }
                for (k, pv in keys(props.value as _object), values(props.value as _object)) {
                    let pdesc = pv?["description"] ?? ""
                    if (!empty(pdesc)) {
                        w |> write("  // {pdesc}\n")
                    }
                    let opt = key_exists(req, k) ? "" : "?"
                    let pt = harmony_ts_type(pv)
                    w |> write("  {k}{opt}: {pt},\n")
                }
                delete req
                w |> write("}) => any;\n\n")
            }
            unsafe {
                delete tv
            }
        }
        w |> write("} // namespace functions")
    }
}

//! The canonical Harmony system body when tools are declared: identity + reasoning effort + the
//! channel declaration + the commentary routing line. No live date line — a cache-stable deviation
//! from the canonical template, noted in tool_formats_plan.md.
let HARMONY_SYSTEM_BODY = ("You are ChatGPT, a large language model trained by OpenAI.\n" +
    "Knowledge cutoff: 2024-06\n\n" +
    "Reasoning: medium\n\n" +
    "# Valid channels: analysis, commentary, final. Channel must be included for every message.\n" +
    "Calls to these tools must go to the commentary channel: 'functions'.")

//! The role-header recipient text of one replayed Harmony CALL turn (before its <|channel|>).
def harmony_call_header(name : string) : string => " to=functions.{name}"

//! The role-header text of one Harmony tool-RESULT turn (between <|start|> and <|channel|>).
def harmony_result_header(name : string) : string => "functions.{name} to=assistant"

// A recipient's function name cut at the first space or "<|" — role-header recipients run to the
// channel marker, and channel-header ones can butt against <|constrain|> with no space between.
def private clip_recipient(s : string) : string {
    var e = length(s)
    let sp = find(s, " ")
    if (sp >= 0 && sp < e) {
        e = sp
    }
    let lt = find(s, "<|")
    if (lt >= 0 && lt < e) {
        e = lt
    }
    return strip(slice(s, 0, e))
}

// One Harmony header at `pos` in the reply's byte view -> kind (reasoning|content|preamble|call|drop,
// name set for calls); advances pos to the body.
def private harmony_header(s : array<uint8> const#; var pos : int&; var name : string&) : string {
    name = ""
    // role-header recipient form: the reply opens with ` to=functions.NAME<|channel|>...`
    if (starts_with(s, pos, " to=")) {
        let ch = find(s, "<|channel|>", pos)
        let msg = find(s, "<|message|>", pos)
        if (msg < 0) {
            return "drop"
        }
        let rcpt = slice(s, pos + length(" to="), ch >= 0 && ch < msg ? ch : msg)
        pos = msg + length("<|message|>")
        if (starts_with(rcpt, "functions.")) {
            name = clip_recipient(slice(rcpt, length("functions."), length(rcpt)))
            return "call"
        }
        return "drop"
    }
    if (!starts_with(s, pos, "<|channel|>")) {
        return ""   // no header: raw text (defensive — classify as content)
    }
    let hstart = pos + length("<|channel|>")
    let msg = find(s, "<|message|>", hstart)
    let nextch = find(s, "<|channel|>", hstart)
    // stray `<|channel|>commentary[ to=assistant]` immediately re-headed — skip it
    if (nextch >= 0 && (msg < 0 || nextch < msg)) {
        pos = nextch
        return harmony_header(s, pos, name)
    }
    if (msg < 0) {
        return "drop"
    }
    let hdr = strip(slice(s, hstart, msg))
    pos = msg + length("<|message|>")
    let sp = find(hdr, " ")
    let cname = sp < 0 ? hdr : slice(hdr, 0, sp)
    let rest = sp < 0 ? "" : strip(slice(hdr, sp + 1, length(hdr)))
    if (starts_with(rest, "to=")) {
        let rcpt = slice(rest, length("to="), length(rest))
        let rsp = find(rcpt, " ")
        let rname = rsp < 0 ? rcpt : slice(rcpt, 0, rsp)
        if (starts_with(rname, "functions.")) {
            name = clip_recipient(slice(rname, length("functions."), length(rname)))
            return "call"
        }
        return cname == "commentary" && rname == "assistant" ? "preamble" : "drop"
    }
    if (cname == "analysis") {
        return "reasoning"
    }
    if (cname == "final") {
        return "content"
    }
    if (cname == "commentary") {
        return "preamble"
    }
    return "drop"
}

//! Parse a complete Harmony reply into (reasoning, content, calls) in ONE walk — gpt-oss tool
//! calls ride the channel structure, superseding split-then-parse for this mode. Segments end
//! at `<|end|>` (+ optional re-header); the terminal one at end-of-reply (stops never appear).
def harmony_parse(reply : string) : ToolReply {
    var out = ToolReply()
    var rparts : array<string>
    var cparts : array<string>
    peek_data(reply) $(d) {
        var pos = 0
        let n = length(d)
        while (pos < n) {
            var name = ""
            let kind = harmony_header(d, pos, name)
            // segment boundary: <|end|>, or a bare <|start|> re-header — the stream can suppress the
            // <|end|> control token entirely (observed live on gpt-oss-20b). The needle is the SAME
            // "<|start|>" the streaming splitter cuts on, so finish and stream never diverge.
            var bend = find(d, "<|end|>", pos)
            var blen = length("<|end|>")
            let bstart = find(d, "<|start|>", pos)
            if (bstart >= 0 && (bend < 0 || bstart < bend)) {
                bend = bstart
                blen = length("<|start|>")
            }
            let body = slice(d, pos, bend < 0 ? n : bend)
            if (kind == "reasoning") {
                rparts |> push(body)
            } elif (kind == "content" || kind == "" || kind == "preamble") {
                cparts |> push(body)
            } elif (kind == "call") {
                out.calls |> push(ToolCall(name = name, args = strip(body)))
            }
            if (bend < 0) {
                break
            }
            pos = bend + blen
            if (starts_with(d, pos, "<|start|>")) {
                pos += length("<|start|>")
            }
            if (starts_with(d, pos, "assistant")) {
                pos += length("assistant")   // the re-header's role token; the recipient/channel follow
            }
        }
    }
    out.reasoning = strip(join(rparts, ""))
    out.content = strip(join(cparts, ""))
    delete rparts
    delete cparts
    return <- out
}

// ===== Mistral v0.3/Nemo — [TOOL_CALLS] + JSON array =====

//! The `[AVAILABLE_TOOLS]` definitions segment preceding a Mistral user turn: the verbatim tool
//! JSONs joined into the bracketed array (the bracket markers resolve as vocab specials at render).
def mistral_defs_text(tools_json : array<string>) : string {
    let defs = join(tools_json, ", ")
    return "[AVAILABLE_TOOLS][{defs}][/AVAILABLE_TOOLS]"
}

//! Parse a Mistral reply: free content up to `[TOOL_CALLS]`, then a JSON array of
//! `\{"name","arguments"[,"id"]}` objects (id genuinely optional and order-tolerant — we do
//! not copy llama.cpp's fixed-field-order trap). A bare object is tolerated too.
def mistral_parse(reply : string) : ToolReply {
    var out = ToolReply()
    let m = find(reply, "[TOOL_CALLS]")
    if (m < 0) {
        // the SPM stream suppresses control-token pieces, so the marker may never reach the
        // decoded reply — accept a bare call array/object (requires name+arguments to qualify)
        let b = find(reply, "[")
        let b2 = find(reply, "\{")
        let start = b >= 0 && (b2 < 0 || b < b2) ? b : b2
        if (start >= 0) {
            var perr = ""
            var js = read_json(slice(reply, start, length(reply)), perr)
            if (js != null) {
                if (js.value is _array) {
                    for (cv in (js.value as _array)) {
                        let nm = cv?["name"] ?? ""
                        if (!empty(nm) && cv?["arguments"] != null) {
                            out.calls |> push(mistral_call_of(cv))
                        }
                    }
                } elif (js.value is _object) {
                    let nm = js?["name"] ?? ""
                    if (!empty(nm) && js?["arguments"] != null) {
                        out.calls |> push(mistral_call_of(js))
                    }
                }
                unsafe {
                    delete js
                }
            }
            if (!empty(out.calls)) {
                out.content = strip(slice(reply, 0, start))
                return <- out
            }
        }
        out.content = reply
        return <- out
    }
    out.content = strip(slice(reply, 0, m))
    let body = strip(slice(reply, m + length("[TOOL_CALLS]"), length(reply)))
    var perr = ""
    var js = read_json(body, perr)
    if (js == null) {
        return <- out
    }
    if (js.value is _array) {
        out.calls |> reserve(length(js.value as _array))
        for (cv in (js.value as _array)) {
            out.calls |> push(mistral_call_of(cv))
        }
    } elif (js.value is _object) {
        out.calls |> push(mistral_call_of(js))
    }
    unsafe {
        delete js
    }
    return <- out
}

def private mistral_call_of(cv : JsonValue?) : ToolCall {
    var c = ToolCall(name = cv?["name"] ?? "", id = cv?["id"] ?? "")
    let av = cv?["arguments"]
    if (av != null && av.value is _string) {   // non-spec but seen: arguments emitted as a string pass through verbatim
        let s = av ?? ""
        c.args = empty(s) ? "\{}" : s
    } elif (av != null && !(av.value is _null)) {
        c.args = write_json_compact(av)
    } else {
        c.args = "\{}"
    }
    return c
}

//! Extract one call from a verbatim `\{"name":...,"arguments":...}` JSON string (the server's
//! replay shape) — name + args normalized to compact JSON object text.
def tool_call_of_json(cj : string) : ToolCall {
    var perr = ""
    var js = read_json(cj, perr)
    if (js == null) {
        return ToolCall()
    }
    let c = mistral_call_of(js)
    unsafe {
        delete js
    }
    return c
}

//! The canonical `content|tojson` rule for result turns: a result that IS valid JSON
//! (object/array/number/bool) passes through normalized; anything else renders as a JSON
//! string literal. harmony + llama_json result turns use this.
def json_value_or_quoted(s : string) : string {
    var perr = ""
    var js = read_json(s, perr)
    if (js != null && !(js.value is _string)) {
        let out = write_json_compact(js)
        unsafe {
            delete js
        }
        return out
    }
    if (js != null) {
        unsafe {
            delete js
        }
    }
    return json_quoted(s)
}

//! `s` as a JSON string literal (quotes + escapes) — harmony/llama result turns carry plain
//! string results JSON-quoted on the wire.
def json_quoted(s : string) : string {
    var jv = JV(s)
    let out = write_json_compact(jv)
    unsafe {
        delete jv
    }
    return out
}

//! Render one replayed Mistral call turn's text: `[TOOL_CALLS][{...}]` with the id kept when
//! the call carries one (the Nemo template requires ids in history; v0.3 output has none).
def mistral_calls_text(calls : array<ToolCall>) : string {
    return build_string() $(w) {
        w |> write("[TOOL_CALLS][")
        var first = true
        for (c in calls) {
            if (!first) {
                w |> write(", ")
            }
            first = false
            let a = empty(c.args) ? "\{}" : c.args
            w |> write("\{\"name\": \"{c.name}\", \"arguments\": {a}")
            if (!empty(c.id)) {
                w |> write(", \"id\": \"{c.id}\"")
            }
            w |> write("}")
        }
        w |> write("]")
    }
}

// ===== Llama-3.x — the whole reply is one JSON object =====

//! The llama-3 custom-functions instruction block prefixed to the first user turn: the canonical
//! prompt text + the verbatim tool JSONs. The chat layer appends the user's message after it.
def llama_json_defs_text(tools_json : array<string>) : string {
    let defs = join(tools_json, "\n\n")
    return ("Given the following functions, please respond with a JSON for a function call " +
        "with its proper arguments that best answers the given prompt.\n\n" +
        "Respond in the format \{\"name\": function name, \"parameters\": dictionary of argument name and its value}." +
        "Do not use variables.\n\n{defs}")
}

//! One replayed llama-3 call turn's body — the whole turn is this one JSON object.
def llama_json_call_text(name : string; args : string) : string {
    let a = empty(args) ? "\{}" : args
    return "\{\"name\": \"{name}\", \"parameters\": {a}}"
}

//! Parse a llama-3 reply: content up to the first `\{`; the remainder must parse as
//! `\{["type":"function",]"name":...,"parameters":...}` to be a call (structural acceptance).
//! A reply with no parseable call object is pure content.
def llama_json_parse(reply : string) : ToolReply {
    var out = ToolReply()
    let b = find(reply, "\{")
    if (b < 0) {
        out.content = reply
        return <- out
    }
    let tail = slice(reply, b, length(reply))
    var perr = ""
    var js = read_json(tail, perr)
    if (js == null || !(js.value is _object)) {
        out.content = reply
        return <- out
    }
    let fv = tool_function_of(js)
    let name = fv?["name"] ?? ""
    let pv = fv?["parameters"]
    if (empty(name) || pv == null) {
        out.content = reply
        unsafe {
            delete js
        }
        return <- out
    }
    out.content = strip(slice(reply, 0, b))
    out.calls |> push(ToolCall(name = name, args = write_json_compact(pv)))
    unsafe {
        delete js
    }
    return <- out
}

// ===== hermes — marker-framed JSON calls (<tool_call> ... </tool_call>) =====

//! Parse a marker-framed (hermes-style) reply: each well-formed ``open_mark``..``close_mark`` span
//! parses as one JSON call object (nameless spans skip; an unterminated span ends the scan); content
//! is the text before the first opener — the whole reply when nothing parses or ``open_mark`` is empty.
def marker_parse(reply : string; open_mark, close_mark : string) : ToolReply {
    var out = ToolReply()
    if (empty(open_mark)) {
        out.content = reply
        return <- out
    }
    var content_end = -1
    peek_data(reply) $(d) {
        var pos = 0
        while (true) {
            let o = find(d, open_mark, pos)
            if (o < 0) {
                break
            }
            if (content_end < 0) {
                content_end = o
            }
            let bstart = o + length(open_mark)
            let cl = find(d, close_mark, bstart)
            if (cl < 0) {
                break   // unterminated span (token budget cut mid-call) — scan stops
            }
            let c = tool_call_of_json(strip(slice(d, bstart, cl)))
            if (!empty(c.name)) {
                out.calls |> push(c)
            }
            pos = cl + length(close_mark)
        }
    }
    out.content = (content_end < 0 || empty(out.calls)) ? reply : strip(slice(reply, 0, content_end))
    return <- out
}

// ===== gemma-4 — the bracketed DSL (unquoted keys, <|"|> string-quote token, dictsorted) =====

let private G4Q = "<|\"|>"   // the family's string-quote TOKEN — DSL strings need no escaping

// One DSL value at `pos` in the block body's byte view -> JSON text (recursive: string | dict |
// array | raw number/bool/null). The view's length is the bound gemma4_parse cut at the closer.
def private gemma4_dsl_value(s : array<uint8> const#; var pos : int&) : string {
    let n = length(s)
    if (starts_with(s, pos, G4Q)) {
        let e = find(s, G4Q, pos + length(G4Q))
        let body = slice(s, pos + length(G4Q), e < 0 ? n : e)
        pos = e < 0 ? n : e + length(G4Q)
        var jv = JV(body)
        let out = write_json_compact(jv)   // JSON-escaped "..."
        unsafe {
            delete jv
        }
        return out
    }
    if (starts_with(s, pos, "\{")) {
        pos ++
        var parts : array<string>
        while (pos < n && !starts_with(s, pos, "}")) {
            let c = find(s, ":", pos)
            let eb = find(s, "}", pos)
            if (c < 0 || (eb >= 0 && eb < c)) {
                pos = eb < 0 ? n : eb   // no key separator before the dict closes — resync at the closer
                break
            }
            let key = strip(slice(s, pos, c))
            pos = c + 1
            let v = gemma4_dsl_value(s, pos)
            var kv = JV(key)
            let kq = write_json_compact(kv)
            unsafe {
                delete kv
            }
            parts |> push("{kq}:{v}")
            if (pos < n && starts_with(s, pos, ",")) {
                pos ++
            }
        }
        if (pos < n) {
            pos ++   // '}'
        }
        let joined = join(parts, ",")
        delete parts
        return "\{{joined}}"
    }
    if (starts_with(s, pos, "[")) {
        pos ++
        var parts : array<string>
        while (pos < n && !starts_with(s, pos, "]")) {
            parts |> push(gemma4_dsl_value(s, pos))
            if (pos < n && starts_with(s, pos, ",")) {
                pos ++
            }
        }
        if (pos < n) {
            pos ++   // ']'
        }
        let joined = join(parts, ",")
        delete parts
        return "[{joined}]"
    }
    // raw token: number / true / false / null — up to the enclosing delimiter
    var e = pos
    while (e < n) {
        let ch = int(s[e])
        if (ch == ',' || ch == '}' || ch == ']') {
            break
        }
        e ++
    }
    let tok = strip(slice(s, pos, e))
    pos = e
    return tok
}

//! Parse the CONTENT half of a gemma-4 reply (reasoning already split by the asymmetric think
//! matcher): text outside `<|tool_call>call:NAME\{...}<tool_call|>` blocks is content, each
//! block one call with its DSL dict converted to JSON args. Adjacent blocks = parallel calls.
def gemma4_parse(reply : string) : ToolReply {
    var out = ToolReply()
    var cparts : array<string>
    peek_data(reply) $(d) {
        var pos = 0
        let n = length(d)
        while (pos < n) {
            let b = find(d, "<|tool_call>call:", pos)
            if (b < 0) {
                cparts |> push(slice(d, pos, n))
                break
            }
            cparts |> push(slice(d, pos, b))
            let p = b + length("<|tool_call>call:")
            // every scan inside the block is bounded by ITS closer — a malformed body can never grab
            // a later block's brace or run the walk past the closer
            let close = find(d, "<tool_call|>", p)
            let bend = close < 0 ? n : close
            let brace = find(d, "\{", p)
            if (brace < 0 || brace >= bend) {
                pos = close < 0 ? n : close + length("<tool_call|>")   // no args dict in this block — drop it
                continue
            }
            let name = strip(slice(d, p, brace))
            let body = slice(d, brace, bend)
            var bp = 0
            var args = ""
            peek_data(body) $(bd) {
                args = gemma4_dsl_value(bd, bp)
            }
            out.calls |> push(ToolCall(name = name, args = args))
            pos = close < 0 ? n : close + length("<tool_call|>")
        }
    }
    out.content = strip(join(cparts, ""))
    delete cparts
    return <- out
}

// JSON value -> gemma-4 DSL text (dict keys bare + ALPHABETICAL, strings in quote tokens).
def private gemma4_json_dsl(v : JsonValue?) : string {
    if (v == null || v.value is _null) {
        return "null"
    }
    if (v.value is _string) {
        let sv = v ?? ""
        return "{G4Q}{sv}{G4Q}"
    }
    if (v.value is _bool) {
        return "{v ?? false}"
    }
    if (v.value is _number || v.value is _longint) {   // integer numbers parse as the _longint extension
        return write_json_compact(v)
    }
    if (v.value is _array) {
        var parts <- [for (e in (v.value as _array)); gemma4_json_dsl(e)]
        let joined = join(parts, ",")
        delete parts
        return "[{joined}]"
    }
    var ks <- [for (k in keys(v.value as _object)); k]
    sort(ks)
    var parts <- [for (k in ks); "{k}:{gemma4_json_dsl(v?[k])}"]
    let joined = join(parts, ",")
    delete ks
    delete parts
    return "\{{joined}}"
}

//! One replayed gemma-4 call block: `<|tool_call>call:NAME\{dictsorted-DSL}<tool_call|>` —
//! marker spellings resolve as vocab specials at render, the DSL body is text.
def gemma4_call_dsl(name : string; args_json : string) : string {
    var perr = ""
    var js = read_json(empty(args_json) ? "\{}" : args_json, perr)
    let body = js == null ? "\{}" : gemma4_json_dsl(js)
    if (js != null) {
        unsafe {
            delete js
        }
    }
    return "call:{name}{body}"
}

//! One tool-result block body: `response:NAME\{...}` — a JSON-object result renders dictsorted,
//! anything else wraps as `\{value:<|"|>text<|"|>}` (the template's non-mapping rule).
def gemma4_response_dsl(name : string; result : string) : string {
    var perr = ""
    var js = read_json(result, perr)
    if (js != null && js.value is _object) {
        let body = gemma4_json_dsl(js)
        unsafe {
            delete js
        }
        return "response:{name}{body}"
    }
    if (js != null) {
        unsafe {
            delete js
        }
    }
    return "response:{name}\{value:{G4Q}{result}{G4Q}}"
}

//! One tool declaration for the system turn: `declaration:NAME\{description:...,parameters:
//! \{properties:\{...},required:[...],type:<|"|>OBJECT<|"|>}}` — dictsorted properties, types
//! UPPERCASED, the fixed per-property key order of the canonical template.
def gemma4_tool_declaration(tool_json : string) : string {
    var perr = ""
    var tv = read_json(tool_json, perr)
    if (tv == null) {
        return ""
    }
    let fv = tool_function_of(tv)
    let name = fv?["name"] ?? ""
    let desc = fv?["description"] ?? ""
    let out = build_string() $(w) {
        w |> write("declaration:{name}\{")
        if (!empty(desc)) {
            w |> write("description:{G4Q}{desc}{G4Q},")
        }
        w |> write("parameters:\{")
        let params = fv?["parameters"]
        let props = params?["properties"]
        if (props != null && props.value is _object) {
            w |> write("properties:\{")
            var ks <- [for (k in keys(props.value as _object)); k]
            sort(ks)
            var first = true
            for (k in ks) {
                if (!first) {
                    w |> write(",")
                }
                first = false
                w |> write("{k}:\{")
                gemma4_property_body(w, props?[k])
                w |> write("}")
            }
            delete ks
            w |> write("},")
        }
        let rv = params?["required"]
        if (rv != null && rv.value is _array && !empty(rv.value as _array)) {
            var rparts : array<string>
            for (r in (rv.value as _array)) {
                let rs = r ?? ""
                rparts |> push("{G4Q}{rs}{G4Q}")
            }
            let joined = join(rparts, ",")
            w |> write("required:[{joined}],")
            delete rparts
        }
        w |> write("type:{G4Q}OBJECT{G4Q}}}")
    }
    unsafe {
        delete tv
    }
    return out
}

// One property's body in the declaration DSL — the template's fixed key order:
// description -> enum -> nullable -> type (UPPERCASED) LAST.
def private gemma4_property_body(var w : StringBuilderWriter; pv : JsonValue?) {
    let pdesc = pv?["description"] ?? ""
    if (!empty(pdesc)) {
        w |> write("description:{G4Q}{pdesc}{G4Q},")
    }
    let en = pv?["enum"]
    if (en != null && en.value is _array) {
        var parts : array<string>
        parts |> reserve(length(en.value as _array))
        for (e in (en.value as _array)) {
            let ev = e ?? ""
            parts |> push("{G4Q}{ev}{G4Q}")
        }
        let joined = join(parts, ",")
        w |> write("enum:[{joined}],")
        delete parts
    }
    if ((pv?["nullable"] ?? false)) {
        w |> write("nullable:true,")
    }
    let pt = pv?["type"] ?? "string"
    let ptu = to_upper(pt)
    w |> write("type:{G4Q}{ptu}{G4Q}")
}
