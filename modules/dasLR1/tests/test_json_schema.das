options gen2
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost public
require lr1/lr1
require lr1/gbnf
require lr1/json_schema
require daslib/fio
require daslib/module_path
require daslib/json
require daslib/json_boost
require daslib/strings_boost
require strings

// The schema compiler against two lifted corpora: `_schema_corpus.json` pairs a schema with the grammar text
// the reference converter emits (the compiler's output must match it line for line), and the schema cases
// of `_gbnf_corpus.json` pair a schema with the documents it must accept and refuse (the engine's verdict
// on the compiled grammar).

struct ConversionCase {
    status : string
    name : string
    schema : string
    grammar : string
}

struct AcceptanceCase {
    kind : string
    name : string
    text : string
    passing : array<string>
    failing : array<string>
}

def load_json_file(name : string) : JsonValue? {
    var err = ""
    return read_json(fread(path_join(get_this_module_dir(), name)), err)
}

// the reference's expected texts carry their source indentation: compare line by line, each stripped
def private trimmed(s : string) : string {
    let lines <- [for (ln in split(s, "\n")); strip(ln); where !empty(strip(ln))]
    return join(lines, "\n")
}

[test]
def test_schema_conversion_corpus(t : T?) {
    t |> run("every schema of the conversion corpus compiles to the reference converter's grammar text") @(tt : T?) {
        var js = load_json_file("_schema_corpus.json")
        tt |> success(js != null, "the corpus loaded")
        return if (js == null)
        var cases <- from_JV(js, type<array<ConversionCase>>)
        delete_json(js)
        tt |> success(length(cases) >= 70, "{length(cases)} cases")
        for (c in cases) {
            var err = ""
            let text = json_schema_to_gbnf(c.schema, err)
            if (c.status == "FAILURE") {
                tt |> success(!empty(err), "{c.name}: the schema is refused ({err})")
                continue
            }
            tt |> equal(err, "", "{c.name}: compiles")
            continue if (!empty(err))
            if (trimmed(text) != trimmed(c.grammar)) {
                tt |> failure("{c.name}: grammar text differs\n--- ours\n{text}\n--- reference\n{c.grammar}")
            }
            // and the text the reference emitted builds on the engine
            var gerr = ""
            let g <- gbnf_grammar(text, gerr)
            tt |> equal(gerr, "", "{c.name}: the grammar builds")
        }
    }
}

[test]
def test_schema_acceptance_corpus(t : T?) {
    t |> run("every schema case of the acceptance corpus admits its documents and refuses the others") @(tt : T?) {
        var js = load_json_file("_gbnf_corpus.json")
        tt |> success(js != null, "the corpus loaded")
        return if (js == null)
        var cases <- from_JV(js, type<array<AcceptanceCase>>)
        delete_json(js)
        var schemas = 0
        for (c in cases) {
            continue if (c.kind != "schema")
            schemas++
            var err = ""
            let g <- json_schema_grammar(c.text, err)
            tt |> equal(err, "", "{c.name}: the schema compiles and builds")
            continue if (!empty(err))
            for (s in c.passing) {
                tt |> success(lr_matches(g, s), "{c.name}: accepts {escape(s)}")
            }
            for (s in c.failing) {
                tt |> success(!lr_matches(g, s), "{c.name}: refuses {escape(s)}")
            }
        }
        tt |> equal(schemas, 33, "every schema case ran")
    }
}

[test]
def test_schema_tool_call_shape(t : T?) {
    t |> run("a tool call's schema: the name an enum, the arguments an object with required keys - the viable prefix forbids a dropped key") @(tt : T?) {
        let schema = "\{\"type\":\"object\",\"properties\":\{\"name\":\{\"enum\":[\"control\",\"room_devices\"]\},\"arguments\":\{\"type\":\"object\",\"properties\":\{\"point\":\{\"type\":\"string\"\},\"action\":\{\"type\":\"string\"\}\},\"required\":[\"point\",\"action\"],\"additionalProperties\":false\}\},\"required\":[\"name\",\"arguments\"],\"additionalProperties\":false\}"
        var err = ""
        let g <- json_schema_grammar(schema, err)
        tt |> equal(err, "", "compiles and builds")
        return if (!empty(err))
        tt |> equal(g.conflicts, 0, "a schema grammar is deterministic: one stack all the way")
        tt |> success(lr_matches(g, "\{\"name\": \"control\", \"arguments\": \{\"point\": \"light.2_0_176\", \"action\": \"set_switch\"\}\}"), "the well-formed call")
        tt |> success(!lr_matches(g, "\{\"name\": \"control\", \{\"point\": \"light.2_0_176\", \"action\": \"set_switch\"\}\}"), "the call with the arguments key dropped")
        tt |> success(!lr_matches(g, "\{\"name\": \"open_window\", \"arguments\": \{\"point\": \"x\", \"action\": \"y\"\}\}"), "a tool not in the list")
        tt |> success(!lr_matches(g, "\{\"name\": \"control\", \"arguments\": \{\"point\": \"x\"\}\}"), "a required argument missing")
        // byte by byte: after `"name": "control", ` the only continuation is the arguments key
        var st <- lr_start(g)
        tt |> success(lr_step_bytes(g, st, "\{\"name\": \"control\", "), "up to the second key")
        let nb : ByteSet = lr_next_bytes(g, st)
        tt |> success(nb['"'] && !nb['{'] && !nb['}'], "only a key may follow")
        tt |> success(lr_step_bytes(g, st, "\"a"), "the key begins")
        tt |> success(!lr_step(g, st, uint8('c')), "... and cannot turn into another word")
        tt |> success(lr_step_bytes(g, st, "rguments\": \{\"point\": \"p\", \"action\": \"a\"\}\}") && lr_at_end(g, st), "completes")
    }
}

[test]
def test_schema_refusals(t : T?) {
    t |> run("what the compiler refuses, by name") @(tt : T?) {
        var err = ""
        json_schema_to_gbnf("\{\"type\": \"kaboom\"\}", err)
        tt |> success(find(err, "unrecognized schema") >= 0, "an unknown type: {err}")
        err = ""
        json_schema_to_gbnf("\{\"$ref\": \"https://example.com/x.json\"\}", err)
        tt |> success(find(err, "unsupported $ref") >= 0, "a remote reference: {err}")
        err = ""
        json_schema_to_gbnf("\{\"type\": \"string\", \"pattern\": \"abc\"\}", err)
        tt |> success(find(err, "pattern must start") >= 0, "an unanchored pattern: {err}")
        err = ""
        json_schema_to_gbnf("not json", err)
        tt |> success(find(err, "json schema:") >= 0, "malformed schema text: {err}")
    }
}
