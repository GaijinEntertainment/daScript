options gen2
options persistent_heap
options stack = 524288
options _dasllama_internal = true

require dastest/testing_boost public
require _server_rig                 // models_dir (sibling)
require constraints                 // the constraint fields' reader and the grammar constraint (sibling)
require dasllama/dasllama
require dasllama/dasllama_common    // load_model_: the plain loader, which mints no image
require lr1/lr1
require lr1/gbnf
require daslib/json
require daslib/json_boost
require daslib/fio
require math
require strings

// The server's half of structured output, without a server: the request fields read to a grammar text or
// a refusal (model-free), and the grammar constraint walking a model's own tokens (tinyllama, JIT only;
// skips without the model).

let MODEL_PATH = path_join(models_dir(), "tinyllama-1.1b-chat-v1.0.Q8_0.gguf")
let LARGE_TIER_BYTES = 6ul * 1024ul * 1024ul * 1024ul   //! the model tier's ceiling, as modules/dasLLAMA/tests/_model_tier.das states it

def private spec_of(req : string; tools : array<string>; call_open = ""; call_close = "") : ConstraintSpec {
    var err = ""
    var js = read_json(req, err)
    assert(js != null, "the request text is JSON")
    var spec <- read_constraint_spec(js, tools, call_open, call_close)
    delete_json(js)
    return <- spec
}

def private spec_of(req : string) : ConstraintSpec {
    let no_tools : array<string>
    return <- spec_of(req, no_tools)
}

let SCHEMA_LEGS = "\{\"type\":\"object\",\"properties\":\{\"answer\":\{\"type\":\"integer\"\}\},\"required\":[\"answer\"],\"additionalProperties\":false\}"

[test]
def test_constraint_spec_response_format(t : T?) {
    t |> run("response_format reads to a grammar: json_object, json_schema, and the refusals") @(t : T?) {
        let plain <- spec_of("\{\"messages\":[]\}")
        t |> equal(plain.gbnf, "", "no format asks for no grammar")
        t |> equal(plain.err, "", "... and refuses nothing")
        let text <- spec_of("\{\"response_format\":\{\"type\":\"text\"\}\}")
        t |> equal(text.gbnf, "", "type text asks for no grammar")
        t |> equal(text.err, "", "... and is no refusal")
        let bare <- spec_of("\{\"response_format\":\{\}\}")
        t |> equal(bare.gbnf + bare.err, "", "an empty format object asks for nothing and refuses nothing")
        let obj <- spec_of("\{\"response_format\":\{\"type\":\"json_object\"\}\}")
        t |> equal(obj.kind, "json_object")
        var err = ""
        let og <- gbnf_grammar(obj.gbnf, err)
        t |> equal(err, "", "the json_object grammar builds")
        t |> success(lr_matches(og, "\{\"a\": [1, 2], \"b\": \{\"c\": null\}\}"), "any object passes")
        t |> success(!lr_matches(og, "[1, 2]"), "an array is not an object")
        let sch <- spec_of("\{\"response_format\":\{\"type\":\"json_schema\",\"json_schema\":\{\"strict\":true,\"schema\":{SCHEMA_LEGS}\}\}\}")
        t |> equal(sch.kind, "json_schema")
        t |> equal(sch.err, "")
        let sg <- gbnf_grammar(sch.gbnf, err)
        t |> equal(err, "", "the schema grammar builds")
        t |> success(lr_matches(sg, "\{\"answer\": 4\}"), "the shaped reply passes")
        t |> success(!lr_matches(sg, "\{\"answer\": \"four\"\}"), "a string where the integer is due fails")
        t |> success(!lr_matches(sg, "\{\}"), "the required key cannot be dropped")
        // a schema the compiler refuses: strict refuses the request, non-strict warns and serves unconstrained
        let bad_strict <- spec_of("\{\"response_format\":\{\"type\":\"json_schema\",\"json_schema\":\{\"strict\":true,\"schema\":\{\"type\":\"kaboom\"\}\}\}\}")
        t |> success(find(bad_strict.err, "not served") >= 0, "strict + unsupported schema: {bad_strict.err}")
        let bad_loose <- spec_of("\{\"response_format\":\{\"type\":\"json_schema\",\"json_schema\":\{\"schema\":\{\"type\":\"kaboom\"\}\}\}\}")
        t |> equal(bad_loose.err, "", "non-strict: no refusal")
        t |> equal(bad_loose.gbnf, "", "... and no grammar")
        let no_schema <- spec_of("\{\"response_format\":\{\"type\":\"json_schema\",\"json_schema\":\{\"name\":\"x\"\}\}\}")
        t |> success(find(no_schema.err, "must be an object") >= 0, "json_schema without a schema: {no_schema.err}")
        let unknown <- spec_of("\{\"response_format\":\{\"type\":\"xml\"\}\}")
        t |> success(find(unknown.err, "response_format.type") >= 0, "an unknown type: {unknown.err}")
        let not_obj <- spec_of("\{\"response_format\":\"json\"\}")
        t |> success(find(not_obj.err, "must be an object") >= 0, "a bare string: {not_obj.err}")
    }
}

let TOOL_WEATHER = "\{\"type\":\"function\",\"function\":\{\"name\":\"get_weather\",\"parameters\":\{\"type\":\"object\",\"properties\":\{\"city\":\{\"type\":\"string\"\}\},\"required\":[\"city\"]\}\}\}"
let TOOL_TIME = "\{\"type\":\"function\",\"function\":\{\"name\":\"get_time\"\}\}"

[test]
def test_constraint_spec_tool_choice(t : T?) {
    t |> run("a forced tool_choice reads to the wire-marked call grammar; its refusals name their cause") @(t : T?) {
        let tools <- [TOOL_WEATHER, TOOL_TIME]
        let forced <- spec_of("\{\"tool_choice\":\{\"type\":\"function\",\"function\":\{\"name\":\"get_weather\"\}\}\}", tools, "<tool_call>", "</tool_call>")
        t |> equal(forced.err, "")
        t |> equal(forced.kind, "tool")
        var err = ""
        let fg <- gbnf_grammar(forced.gbnf, err)
        t |> equal(err, "", "the forced-call grammar builds")
        t |> success(lr_matches(fg, "<tool_call>\n\{\"name\": \"get_weather\", \"arguments\": \{\"city\": \"Oslo\"\}\}\n</tool_call>"), "the marked call passes")
        t |> success(!lr_matches(fg, "\{\"name\": \"get_weather\", \"arguments\": \{\"city\": \"Oslo\"\}\}"), "the bare call lacks the markers")
        t |> success(!lr_matches(fg, "<tool_call>\n\{\"name\": \"get_time\", \"arguments\": \{\}\}\n</tool_call>"), "the other tool is refused under a forced name")
        t |> success(!lr_matches(fg, "<tool_call>\n\{\"name\": \"get_weather\", \"arguments\": \{\}\}\n</tool_call>"), "a required argument cannot be dropped")
        let required <- spec_of("\{\"tool_choice\":\"required\"\}", tools, "<tool_call>", "</tool_call>")
        t |> equal(required.err, "")
        let rg <- gbnf_grammar(required.gbnf, err)
        t |> equal(err, "", "the any-tool grammar builds")
        t |> success(lr_matches(rg, "<tool_call>\n\{\"name\": \"get_time\", \"arguments\": \{\}\}\n</tool_call>"), "a parameterless tool under required")
        t |> success(lr_matches(rg, "<tool_call>\n\{\"name\": \"get_weather\", \"arguments\": \{\"city\": \"Oslo\"\}\}\n</tool_call>"), "the other tool too")
        t |> success(!lr_matches(rg, "<tool_call>\n\{\"name\": \"open_window\", \"arguments\": \{\}\}\n</tool_call>"), "an undeclared tool is refused")
        // auto and none constrain nothing
        let unforced <- spec_of("\{\"tool_choice\":\"auto\"\}", tools, "<tool_call>", "</tool_call>")
        t |> equal(unforced.gbnf, "", "auto asks for no grammar")
        // the refusals
        let no_tools : array<string>
        let orphan <- spec_of("\{\"tool_choice\":\"required\"\}", no_tools, "<tool_call>", "</tool_call>")
        t |> success(find(orphan.err, "declares no tools") >= 0, "no tools: {orphan.err}")
        let unmarked <- spec_of("\{\"tool_choice\":\"required\"\}", tools)
        t |> success(find(unmarked.err, "tool-call format") >= 0, "a family without markers: {unmarked.err}")
        let unknown <- spec_of("\{\"tool_choice\":\{\"type\":\"function\",\"function\":\{\"name\":\"fly\"\}\}\}", tools, "<tool_call>", "</tool_call>")
        t |> success(find(unknown.err, "not among the declared tools") >= 0, "an undeclared name: {unknown.err}")
        let nameless <- spec_of("\{\"tool_choice\":\{\"type\":\"function\"\}\}", tools, "<tool_call>", "</tool_call>")
        t |> success(find(nameless.err, "names a function") >= 0, "an object without a name: {nameless.err}")
        let both <- spec_of("\{\"tool_choice\":\"required\",\"response_format\":\{\"type\":\"json_object\"\}\}", tools, "<tool_call>", "</tool_call>")
        t |> success(find(both.err, "cannot both") >= 0, "a format and a forced call together: {both.err}")
        let odd_tools <- ["\{\"type\":\"function\",\"function\":\{\"name\":\"odd\",\"parameters\":\{\"type\":\"kaboom\"\}\}\}"]
        let odd <- spec_of("\{\"tool_choice\":\"required\"\}", odd_tools, "<tool_call>", "</tool_call>")
        t |> success(find(odd.err, "not served as a schema") >= 0, "a tool whose parameters do not compile: {odd.err}")
        // the request-level predicate: every shape that arms the reader
        var perr = ""
        for (req in ["\{\"response_format\":\{\"type\":\"text\"\}\}", "\{\"tool_choice\":\"required\"\}", "\{\"tool_choice\":\{\"type\":\"function\",\"function\":\{\"name\":\"f\"\}\}\}"]) {
            var js = read_json(req, perr)
            t |> success(constraint_fields_present(js), "a constraint field is present: {req}")
            delete_json(js)
        }
        for (req in ["\{\"messages\":[]\}", "\{\"tool_choice\":\"auto\"\}", "\{\"tool_choice\":\"none\"\}", "\{\"response_format\":null\}"]) {
            var js = read_json(req, perr)
            t |> success(!constraint_fields_present(js), "no constraint field: {req}")
            delete_json(js)
        }
    }
}

// the ids whose pieces spell `text`, longest piece first - the tokenizer's own cut would prepend a space
def private ids_spelling(pieces : table<string; int64>; text : string) : array<int64> {
    var ids : array<int64>
    var at = 0
    let n = length(text)
    peek_data(text) $(d) {
        while (at < n) {
            var best = 0
            var best_id = -1l
            for (plen in range(1, min(16, n - at) + 1)) {
                let piece = slice(d, at, at + plen)
                if (pieces |> key_exists(piece)) {
                    best = plen
                    best_id = pieces?[piece] ?? -1l
                }
            }
            assert(best > 0, "every byte of the text is some token's piece")
            ids |> push(best_id)
            at += best
        }
    }
    return <- ids
}

[test]
def test_grammar_constraint_walk(t : T?) {
    t |> run("the grammar constraint admits a model's tokens along a sentence and the stop token at its end alone (tinyllama Q8)") @(t : T?) {
        if (!jit_enabled()) {
            t |> skip("interpreted (the piece table decodes the vocabulary)")
            return
        }
        let st = stat(MODEL_PATH)
        if (!st.is_valid) {
            t |> skip("{base_name(MODEL_PATH)} not present (set DASLLAMA_MODELS_DIR)")
            return
        }
        if (st.size > LARGE_TIER_BYTES) {
            t |> skip("{base_name(MODEL_PATH)} is above the model tier")
            return
        }
        with_dasllama_jobque() {
            grammar_walk_cell(t)
        }
    }
}

def private grammar_walk_cell(t : T?) {
    var m <- load_model_(MODEL_PATH, QuantMode.q8)   // the plain loader: a test never mints an image beside the model
    let vocab = int(m.config.vocab_size)
    token_pieces(m, 0)
    // the piece table: a mid-sentence SentencePiece token keeps its leading space, the first token the same
    let cat <- encode(m, "the cat", false)
    t |> success(length(cat) >= 2, "two tokens")
    if (length(cat) >= 2) {
        t |> equal(token_piece_of(0, cat[1]), " cat", "the second token's piece carries its space")
        t |> equal(token_piece_of(0, cat[0]), " the", "... and so does the sentence-initial one")
    }
    var pieces : table<string; int64>
    for (id in range(vocab)) {
        let p = token_piece_of(0, int64(id))
        if (!empty(p) && !(pieces |> key_exists(p))) {
            pieces[p] = int64(id)
        }
    }
    to_log(LOG_INFO, "test_constraints: {length(pieces)} distinct pieces over {vocab} tokens\n")
    var chat = create_chat(m, "", 8l)
    let stops <- effective_stop_ids(chat)
    t |> success(!empty(stops), "the chat's stop ids")
    let eos = stops[0]
    let spec <- spec_of("\{\"response_format\":\{\"type\":\"json_schema\",\"json_schema\":\{\"strict\":true,\"schema\":{SCHEMA_LEGS}\}\}\}")
    var err = ""
    var c = constraint_of(spec, m, 0, stops, err)
    t |> equal(err, "")
    t |> success(c != null, "the constraint builds")
    return if (c == null)
    t |> success(!c->admits(eos), "the stop token is refused on an empty reply")
    t |> equal(token_piece_of(0, 1l), "<s>", "the BOS token's piece is its marker text")
    t |> success(!c->admits(1l), "... which no JSON document opens with")
    t |> success(!c->admits(pieces["]"]), "a byte outside the grammar is refused")
    t |> success(c->admits(pieces["\{"]), "the opening brace is admitted")
    let ids <- ids_spelling(pieces, "\{\"answer\": 42\}")
    for (id in ids) {
        t |> success(c->admits(id), "admits {escape(token_piece_of(0, id))}")
        c->commit(id)
    }
    t |> success(c->admits(eos), "the stop token is admitted once the object is complete")
    t |> success(!c->admits(pieces["\{"]), "... and nothing else is")
    // a fresh constraint on the same grammar starts over (the cache hands out a copy, never the walked one)
    var c2 = constraint_of(spec, m, 0, stops, err)
    t |> success(c2 != null && !c2->admits(eos), "a second constraint starts at the beginning")
    // a schema that compiles but whose grammar cannot build: a required self-reference derives no sentence
    let bottomless <- spec_of("\{\"response_format\":\{\"type\":\"json_schema\",\"json_schema\":\{\"strict\":true,\"schema\":\{\"$ref\":\"#/$defs/a\",\"$defs\":\{\"a\":\{\"type\":\"object\",\"properties\":\{\"x\":\{\"$ref\":\"#/$defs/a\"\}\},\"required\":[\"x\"],\"additionalProperties\":false\}\}\}\}\}\}")
    t |> equal(bottomless.err, "", "the schema itself compiles")
    var berr = ""
    var none = constraint_of(bottomless, m, 0, stops, berr)
    t |> success(none == null && find(berr, "derives no terminal string") >= 0, "the grammar does not build, by name: {berr}")
    unsafe {
        delete c
        delete c2
    }
    reset_token_pieces()
    delete chat
    delete m
}
