options gen2
options _comment_hygiene = true

module dasllama_tools shared public

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
def private harmony_ts_type(schema : JsonValue?) : string { // nolint:STYLE037 — one arm per JSON-schema shape, mirroring the template's type ladder
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
def harmony_tools_namespace(tools_json : array<string>) : string { // nolint:STYLE037 — flat per-tool emission into one builder
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

// One Harmony header at `pos` -> kind (reasoning|content|preamble|call|drop, name set for calls); advances pos to the body.
def private harmony_header(s : string; var pos : int&; var name : string&) : string { // nolint:STYLE037 — one arm per header shape, per the PEG rules
    name = ""
    // role-header recipient form: the reply opens with ` to=functions.NAME<|channel|>...`
    if (starts_with(slice(s, pos, length(s)), " to=")) {
        let ch = find(s, "<|channel|>", pos)
        let msg = find(s, "<|message|>", pos)
        if (msg < 0) {
            return "drop"
        }
        let rcpt = slice(s, pos + length(" to="), ch >= 0 && ch < msg ? ch : msg)
        pos = msg + length("<|message|>")
        if (starts_with(rcpt, "functions.")) {
            name = strip(slice(rcpt, length("functions."), length(rcpt)))
            return "call"
        }
        return "drop"
    }
    if (!starts_with(slice(s, pos, length(s)), "<|channel|>")) {
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
            name = slice(rname, length("functions."), length(rname))
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
def harmony_parse(reply : string) : ToolReply { // nolint:STYLE037 — the segment loop routes each kind once
    var out = ToolReply()
    var rparts : array<string>
    var cparts : array<string>
    var pos = 0
    let n = length(reply)
    while (pos < n) {
        var name = ""
        let kind = harmony_header(reply, pos, name)
        let bend = find(reply, "<|end|>", pos)
        let body = slice(reply, pos, bend < 0 ? n : bend)
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
        pos = bend + length("<|end|>")
        if (starts_with(slice(reply, pos, n), "<|start|>assistant")) {
            pos += length("<|start|>assistant")
        }
    }
    out.reasoning = strip(join(rparts, ""))
    out.content = strip(join(cparts, ""))
    delete rparts
    delete cparts
    return <- out
}

// ===== Mistral v0.3/Nemo — [TOOL_CALLS] + JSON array =====

//! Parse a Mistral reply: free content up to `[TOOL_CALLS]`, then a JSON array of
//! `\{"name","arguments"[,"id"]}` objects (id genuinely optional and order-tolerant — we do
//! not copy llama.cpp's fixed-field-order trap). A bare object is tolerated too.
def mistral_parse(reply : string) : ToolReply {
    var out = ToolReply()
    let m = find(reply, "[TOOL_CALLS]")
    if (m < 0) {
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
    c.args = av != null ? write_json_compact(av) : "\{}"
    return c
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
