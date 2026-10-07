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
        tt |> equal(length(cases), 73, "every case of the reference's conversion test")
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

// the compiled grammar's verdict on `text`
def private admits(schema : string; text : string) : bool {
    var err = ""
    let g <- json_schema_grammar(schema, err)
    assert(empty(err))
    return lr_matches(g, text)
}

[test]
def test_schema_composed_keywords(t : T?) {
    t |> run("oneOf, anyOf, allOf, $ref and prefixItems: the engine admits exactly the composed documents") @(t : T?) {
        let one_of = "\{\"oneOf\":[\{\"type\":\"integer\"\},\{\"type\":\"boolean\"\}]\}"
        t |> success(admits(one_of, "42") && admits(one_of, "true"), "oneOf: either alternative")
        t |> success(!admits(one_of, "\"x\"") && !admits(one_of, "null"), "oneOf: nothing else")
        let any_of = "\{\"anyOf\":[\{\"type\":\"string\"\},\{\"type\":\"null\"\}]\}"
        t |> success(admits(any_of, "\"x\"") && admits(any_of, "null"), "anyOf: either alternative")
        t |> success(!admits(any_of, "1"), "anyOf: nothing else")
        let all_of = "\{\"allOf\":[\{\"type\":\"object\",\"properties\":\{\"a\":\{\"type\":\"integer\"\}\},\"required\":[\"a\"]\},\{\"properties\":\{\"b\":\{\"type\":\"boolean\"\}\},\"required\":[\"b\"]\}]\}"
        t |> success(admits(all_of, "\{\"a\": 1, \"b\": true\}"), "allOf: both parts' keys")
        t |> success(!admits(all_of, "\{\"a\": 1\}") && !admits(all_of, "\{\"b\": true\}"), "allOf: neither part alone")
        let ref = "\{\"$ref\":\"#/$defs/pt\",\"$defs\":\{\"pt\":\{\"type\":\"object\",\"properties\":\{\"x\":\{\"type\":\"number\"\}\},\"required\":[\"x\"]\}\}\}"
        t |> success(admits(ref, "\{\"x\": 1.5\}"), "$ref: the definition's shape")
        t |> success(!admits(ref, "\{\"y\": 1\}") && !admits(ref, "1.5"), "$ref: nothing else")
        let pair = "\{\"type\":\"array\",\"prefixItems\":[\{\"type\":\"integer\"\},\{\"type\":\"string\"\}]\}"
        t |> success(admits(pair, "[1, \"a\"]"), "prefixItems: the typed positions")
        t |> success(!admits(pair, "[\"a\", 1]") && !admits(pair, "[1]"), "prefixItems: the wrong order, a short tuple")
    }
}

[test]
def test_schema_parser_and_formats(t : T?) {
    t |> run("the schema parser's escapes, the uuid and date formats, a recursive $ref, and the pattern forms") @(t : T?) {
        // string escapes land as their bytes in the emitted literal
        let esc = "\{\"const\":\"a\\tb\\bc\\fd\\/e\\u00e9\\ud83d\\ude00\"\}"
        var err = ""
        let eg <- json_schema_grammar(esc, err)
        t |> equal(err, "", "the escapes parse")
        t |> success(lr_matches(eg, "\"a\\tb\\bc\\fd/eé😀\""), "... and the constant spells them as JSON")   // nolint:STYLE039 - the non-ASCII bytes are the point
        t |> success(admits("\{\"type\":\"string\",\"format\":\"uuid\"\}", "\"123e4567-e89b-12d3-a456-426614174000\""), "uuid: a well-formed one")
        t |> success(!admits("\{\"type\":\"string\",\"format\":\"uuid\"\}", "\"123e4567-e89b-12d3-a456\""), "uuid: a short one")
        t |> success(admits("\{\"type\":\"string\",\"format\":\"date-time\"\}", "\"2026-10-07T12:34:56Z\""), "date-time")
        t |> success(!admits("\{\"type\":\"string\",\"format\":\"date\"\}", "\"2026-13-07\""), "date: month 13")
        // a recursive definition builds once and refers to itself
        let tree = "\{\"$ref\":\"#/$defs/node\",\"$defs\":\{\"node\":\{\"type\":\"object\",\"properties\":\{\"kids\":\{\"type\":\"array\",\"items\":\{\"$ref\":\"#/$defs/node\"\}\}\},\"required\":[\"kids\"],\"additionalProperties\":false\}\}\}"
        t |> success(admits(tree, "\{\"kids\": [\{\"kids\": []\}, \{\"kids\": []\}]\}"), "a recursive schema's nested document")
        t |> success(!admits(tree, "\{\"kids\": [1]\}"), "... and a leaf of the wrong shape")
        // pattern forms: a non-capturing group, an open-ended count
        let pat = "\{\"type\":\"string\",\"pattern\":\"^(?:ab)\{2,\}c?$\"\}"
        t |> success(admits(pat, "\"ababab\"") && admits(pat, "\"ababc\""), "a non-capturing group repeated at least twice")
        t |> success(!admits(pat, "\"ab\"") && !admits(pat, "\"abac\""), "... one copy, or a broken copy")
        for (bad in ["^[a$", "^a\{1$", "^a\{1,2,3\}$", "^*a$", "^(?=a)$"]) {
            err = ""
            json_schema_to_gbnf("\{\"type\":\"string\",\"pattern\":\"{bad}\"\}", err)
            t |> success(!empty(err), "pattern refused: {bad} - {err}")
        }
        for (bad in ["\{\"type\":\"string\",\"const\":\"unterminated\}", "\{\"a\":\"\\q\"\}", "\{\"a\":\"\\u12\"\}", "\{\"a\" 1\}", "\{\} trailing", "[1]", "\{\"$ref\":\"#/$defs/missing\"\}"]) {
            err = ""
            json_schema_to_gbnf(bad, err)
            t |> success(!empty(err), "schema text refused: {escape(bad)} - {err}")
        }
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
        err = ""
        json_schema_to_gbnf("\{\"type\": 123\}", err)
        tt |> success(find(err, "unrecognized schema") >= 0, "a type that is no name: {err}")
        err = ""
        json_schema_to_gbnf("\{\"type\": \"array\", \"items\": \{\"type\": \"integer\"\}, \"maxItems\": 1000000000\}", err)
        tt |> success(find(err, "past the cap") >= 0, "an item count past the cap: {err}")
        err = ""
        json_schema_to_gbnf("\{\"type\": \"string\", \"minLength\": {GBNF_REPEAT_CAP + 1}\}", err)
        tt |> success(find(err, "past the cap") >= 0, "a string length past the cap: {err}")
        err = ""
        json_schema_to_gbnf("\{\"type\": \"string\", \"pattern\": \"^a\{1,{GBNF_REPEAT_CAP + 1}\}$\"\}", err)
        tt |> success(find(err, "past the cap") >= 0, "a pattern count past the cap: {err}")
        err = ""
        json_schema_to_gbnf("\{\"type\": \"array\", \"items\": \{\"type\": \"integer\"\}, \"maxItems\": {GBNF_REPEAT_CAP}\}", err)
        tt |> equal(err, "", "the cap itself compiles")
    }
}
