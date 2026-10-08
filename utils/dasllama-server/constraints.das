options gen2
options stack = 524288

module constraints shared public

require dasllama/dasllama
require lr1/lr1
require lr1/gbnf
require lr1/json_schema
require daslib/json
require daslib/json_boost
require daslib/strings_boost
require strings

// The grammar behind a constrained chat request: `response_format` (a JSON object, or a JSON schema) and a
// forced `tool_choice` (one function, or any of the declared ones) each compile to a byte grammar the
// engine's sampler consults token by token. The sampler's contract is the engine's `TokenConstraint`, asked
// "may this token come next" and told "this token was emitted"; the one here owns the grammar's stack set
// and reads the token's bytes off the slot's piece table.

//! Per served slot, every token's bytes: decoded once, on the first constrained request the slot serves.
var private g_token_pieces : array<array<string>>

//! Compiled grammars by their GBNF text: a schema the clients repeat compiles once. Clients choose the
//! keys, so the table is emptied once it holds `GRAMMAR_CACHE_MAX` grammars.
var private g_grammar_cache : table<string; Grammar>
let GRAMMAR_CACHE_MAX = 64

//! Build the piece table of `slot`'s model if it is not built yet.
def token_pieces(m : Model; slot : int) {
    if (slot >= length(g_token_pieces)) {
        g_token_pieces |> resize(slot + 1)
    }
    if (empty(g_token_pieces[slot])) {
        let vocab = int(m.config.vocab_size)
        g_token_pieces[slot] |> resize(vocab)
        for (id in range(vocab)) {
            g_token_pieces[slot][id] = piece(m, int64(id))   // the stream's own text for the token
        }
    }
}

//! The bytes of token `id` on `slot` ("" past the table).
def token_piece_of(slot : int; id : int64) : string {
    if (slot >= length(g_token_pieces) || id < 0l || id >= long_length(g_token_pieces[slot])) {
        return ""
    }
    return g_token_pieces[slot][int(id)]
}

//! Forget every slot's piece table (the server shut down).
def reset_token_pieces() {
    delete g_token_pieces
    delete g_grammar_cache
}

//! Drop `slot`'s piece table as the server drops the slot: the slots above it shift down one, and so do their tables.
def erase_token_pieces(slot : int) {
    if (slot < length(g_token_pieces)) {
        delete g_token_pieces[slot]
        g_token_pieces |> erase(slot)
    }
}

// the grammar of `gbnf`, from the cache or built now; `err` names the failure
def private grammar_for(gbnf : string; var err : string&) : Grammar {
    if (g_grammar_cache |> key_exists(gbnf)) {
        var g : Grammar
        g := g_grammar_cache[gbnf]
        return <- g
    }
    var built <- gbnf_grammar(gbnf, err)
    if (empty(err)) {
        if (length(g_grammar_cache) >= GRAMMAR_CACHE_MAX) {
            delete g_grammar_cache
        }
        var copy : Grammar
        copy := built
        g_grammar_cache[gbnf] <- copy
    }
    return <- built
}

//! The constraint over a grammar for one slot's model: a stop token (`ends`) is admitted exactly where the
//! grammar's sentence is complete, every other token where its bytes extend a sentence.
class GrammarConstraint : TokenConstraint {
    g : Grammar
    st : LrStack
    ends : table<int64>
    slot : int

    def override admits(tok : int64) : bool {
        if (ends |> key_exists(tok)) {
            return lr_at_end(g, st)
        }
        let piece = token_piece_of(slot, tok)
        if (empty(piece)) {
            return false
        }
        var probe : LrStack
        probe.stacks := st.stacks
        return lr_step_bytes(g, probe, piece)
    }

    def override commit(tok : int64) : void {
        if (!(ends |> key_exists(tok))) {
            lr_step_bytes(g, st, token_piece_of(slot, tok))
        }
    }
}

//! A grammar constraint for `slot`'s model, its piece table built first (outside the sampler's hot path).
def grammar_constraint(m : Model; slot : int; var g : Grammar; var ends : table<int64>) : TokenConstraint? {
    token_pieces(m, slot)
    var c = new GrammarConstraint(slot = slot)
    c.g <- g
    c.ends <- ends
    c.st <- lr_start(c.g)
    return c
}

// a `tool_choice` that forces a call: the object form, or "required"
def private forces_tool(js : JsonValue?) : bool {
    let tcv = js?["tool_choice"]
    if (tcv != null && tcv.value is _object) {
        return true
    }
    return tcv != null && tcv.value is _string && (tcv ?? "") == "required"
}

//! Whether the request carries a constraint field at all - a `response_format` of any shape, or a
//! `tool_choice` that forces a call - so its reader runs and refuses a malformed one.
def constraint_fields_present(js : JsonValue?) : bool {
    let rf = js?["response_format"]
    return (rf != null && !(rf.value is _null)) || forces_tool(js)
}

//! The verdict of reading a request's constraint fields.
struct ConstraintSpec {
    gbnf : string       //!< the grammar text, "" when the request asks for none
    err : string        //!< a client-facing refusal, "" when the fields read clean
    kind : string       //!< "json_object" | "json_schema" | "tool" | ""
}

// the composite schema a forced tool call fills: the function's name as a constant, its parameters
// as the arguments object
def private tool_call_schema(tool : JsonValue?) : string {
    let fn = tool?["function"]
    let name = fn?["name"] ?? ""
    let params = fn?["parameters"]
    let params_text = params != null && !(params.value is _null) ? write_json_compact(params) : "\{\"type\":\"object\"\}"
    return "\{\"type\":\"object\",\"properties\":\{\"name\":\{\"const\":{write_json_compact(JV(name))}\},\"arguments\":{params_text}\},\"required\":[\"name\",\"arguments\"],\"additionalProperties\":false\}"
}

// the schema's grammar with its root renamed, under a root that wraps it in the wire markers
def private wrap_tool_grammar(schema_gbnf : string; call_open, call_close : string) : string {
    var lines <- split(schema_gbnf, "\n")
    for (ln in lines) {
        if (ln |> starts_with("root ::= ")) {
            ln = "tool-call ::= {slice(ln, length("root ::= "))}"
        }
    }
    let body = join(lines, "\n")
    return "{body}\nroot ::= {gbnf_literal(call_open)} space tool-call space {gbnf_literal(call_close)} space\n"
}

// the Qwen3.6 call shape as a grammar over the forced tools: `<tool_call>` + one `<function=NAME>` block whose
// `<parameter=K>` blocks name that tool's declared parameters, each value any text without a `<`
def private xml_function_tool_grammar(tools : array<string>; forced_name : string; required : bool) : string {
    var rules, fns : array<string>
    var idx = 0
    for (tv in tools) {
        var perr = ""
        var tool = read_json(tv, perr)
        continue if (tool == null)
        let name = tool?["function"]?["name"] ?? ""
        if (!empty(name) && (required || name == forced_name)) {
            var inscope params : array<string>
            let props = tool?["function"]?["parameters"]?["properties"]
            let close_lit = gbnf_literal("\n</parameter>\n")
            if (props != null && props.value is _object) {
                params |> reserve(length(props.value as _object))
                for (k in keys(props.value as _object)) {
                    let open_lit = gbnf_literal("<parameter={k}>\n")
                    params |> push("{open_lit} value {close_lit}")
                }
            }
            let plist = empty(params) ? "" : " ({join(params, " | ")})*"
            let fn_open = gbnf_literal("<function={name}>\n")
            let fn_close = gbnf_literal("</function>")
            rules |> push("fn-{idx} ::= {fn_open}{plist} {fn_close}")
            fns |> push("fn-{idx}")
            idx ++
        }
        delete_json(tool)
    }
    if (empty(fns)) {
        return ""
    }
    let body = join(rules, "\n")
    delete rules
    let alts = join(fns, " | ")
    delete fns
    let call_open = gbnf_literal("<tool_call>\n")
    let call_close = gbnf_literal("\n</tool_call>")
    return "{body}\nfn ::= {alts}\nvalue ::= [^<]*\nspace ::= [ \\t\\n]*\nroot ::= {call_open} fn {call_close} space\n"
}

//! Read `response_format` and `tool_choice` off a chat request: the grammar text they ask for, or why the
//! request is refused. `tools` are the declared tool definitions as compact JSON; `call_open`/`call_close`
//! the model's tool-call markers ("" = the family carries no marker format); `xml_calls` = the Qwen3.6 call shape.
def read_constraint_spec(js : JsonValue?; tools : array<string>; call_open, call_close : string; xml_calls : bool = false) : ConstraintSpec {   // nolint:STYLE037,STYLE038 - one arm per request field shape, each a refusal or a grammar
    var spec : ConstraintSpec
    let rf = js?["response_format"]
    if (rf != null && rf.value is _object) {
        let rtype = rf?["type"] ?? ""
        if (rtype == "json_object") {
            spec.kind = "json_object"
            var err = ""
            spec.gbnf = json_schema_to_gbnf("\{\"type\":\"object\"\}", err)
        } elif (rtype == "json_schema") {
            let holder = rf?["json_schema"]
            let schema = holder?["schema"]
            if (schema == null || !(schema.value is _object)) {
                spec.err = "'response_format.json_schema.schema' must be an object"
                return <- spec
            }
            var err = ""
            let gbnf = json_schema_to_gbnf(write_json_compact(schema), err)
            if (!empty(err)) {
                if (holder?["strict"] ?? false) {
                    spec.err = "'response_format.json_schema' is not served: {err}"
                    return <- spec
                }
                to_log(LOG_WARNING, "/v1/chat/completions: response_format schema not enforced ({err}) - the reply is unconstrained\n")
            } else {
                spec.kind = "json_schema"
                spec.gbnf = gbnf
            }
        } elif (rtype != "text" && rtype != "") {
            spec.err = "'response_format.type' must be text, json_object or json_schema"
            return <- spec
        }
    } elif (rf != null && !(rf.value is _null)) {
        spec.err = "'response_format' must be an object"
        return <- spec
    }
    let tcv = js?["tool_choice"]
    var forced_name = ""
    if (tcv != null && tcv.value is _object) {
        forced_name = tcv?["function"]?["name"] ?? ""
        if (empty(forced_name)) {
            spec.err = "'tool_choice' as an object names a function: \{\"type\":\"function\",\"function\":\{\"name\":...\}\}"
            return <- spec
        }
    }
    let required = empty(forced_name) && forces_tool(js)
    if (!empty(forced_name) || required) {
        if (empty(tools)) {
            spec.err = "'tool_choice' forces a tool call but the request declares no tools"
            return <- spec
        }
        if (!empty(spec.gbnf)) {
            spec.err = "'response_format' and a forced 'tool_choice' cannot both constrain one reply"
            return <- spec
        }
        if (empty(call_open)) {
            spec.err = "a forced 'tool_choice' is not served for this model's tool-call format"
            return <- spec
        }
        if (xml_calls) {
            let g = xml_function_tool_grammar(tools, forced_name, required)
            if (empty(g)) {
                spec.err = "'tool_choice' names '{forced_name}', which is not among the declared tools"
                return <- spec
            }
            spec.kind = "tool"
            spec.gbnf = g
            return <- spec
        }
        var alts : array<string>
        for (tv in tools) {
            var perr = ""
            var tool = read_json(tv, perr)
            if (tool != null) {
                let name = tool?["function"]?["name"] ?? ""
                if (required || name == forced_name) {
                    alts |> push(tool_call_schema(tool))
                }
                delete_json(tool)
            }
        }
        if (empty(alts)) {
            spec.err = "'tool_choice' names '{forced_name}', which is not among the declared tools"
            return <- spec
        }
        let schema = length(alts) == 1 ? alts[0] : "\{\"anyOf\":[{join(alts, ",")}]\}"
        var err = ""
        let gbnf = json_schema_to_gbnf(schema, err)
        if (!empty(err)) {
            spec.err = "the forced tool's parameters are not served as a schema: {err}"
            return <- spec
        }
        spec.kind = "tool"
        spec.gbnf = wrap_tool_grammar(gbnf, call_open, call_close)
    }
    return <- spec
}

//! The constraint a spec compiles to for `slot`'s model, or null (and `err`) when its grammar does not build.
def constraint_of(spec : ConstraintSpec; m : Model; slot : int; stop_ids : array<int64>; var err : string&) : TokenConstraint? {
    if (empty(spec.gbnf)) {
        return null
    }
    var g <- grammar_for(spec.gbnf, err)
    if (!empty(err)) {
        return null
    }
    var ends : table<int64>
    for (id in stop_ids) {
        ends |> insert(id)
    }
    return grammar_constraint(m, slot, g, ends)
}
