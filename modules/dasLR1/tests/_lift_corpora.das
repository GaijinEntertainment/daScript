options gen2
options indenting = 4

require daslib/fio
require daslib/json
require math
require daslib/json_boost
require daslib/module_path
require daslib/strings_boost
require strings

// Lifts the two corpora beside this file from a llama.cpp checkout - `_gbnf_corpus.json` from
// tests/test-grammar-integration.cpp (every test_grammar and test_schema call: the grammar or schema text, the
// strings it accepts, the strings it refuses; a grammar naming a token reference `<[...]>` is left out, since
// the acceptor sees bytes) and `_schema_corpus.json` from tests/test-json-schema-to-grammar.cpp (every
// test({...}) case: its verdict, the schema, the grammar text the reference emits). Run it as
//   daslang modules/dasLR1/tests/_lift_corpora.das -- <llama.cpp root>
// and commit the two files it rewrites; a corpus that differs from this program's output is hand-edited.

struct Cursor {
    text : array<uint8>
    pos : int
}

def private at(c : Cursor; off : int = 0) : int => c.pos + off < length(c.text) ? int(c.text[c.pos + off]) : -1

def private starts(c : Cursor; s : string) : bool {
    var ok = true
    peek_data(s) $(d) {
        for (i in range(length(d))) {
            if (at(c, i) != int(d[i])) {
                ok = false
                break
            }
        }
    }
    return ok
}

// whitespace and comments
def private skip_blank(var c : Cursor) {
    while (true) {
        let ch = at(c)
        if (ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r') {
            c.pos++
        } elif (ch == '/' && at(c, 1) == '/') {
            while (at(c) >= 0 && at(c) != '\n') {
                c.pos++
            }
        } elif (ch == '/' && at(c, 1) == '*') {
            c.pos += 2
            while (at(c) >= 0 && !(at(c) == '*' && at(c, 1) == '/')) {
                c.pos++
            }
            c.pos += 2
        } else {
            break
        }
    }
}

def private is_ident_start(ch : int) : bool => is_alpha(ch) || ch == '_'

def private read_ident(var c : Cursor) : string {
    let start = c.pos
    while (is_ident_start(at(c)) || is_number(at(c))) {
        c.pos++
    }
    let out <- [for (i in range(start, c.pos)); c.text[i]]
    return string(out)
}

// one C++ string literal at the cursor: a raw string R"delim(...)delim", or a quoted one with escapes
def private read_literal(var c : Cursor) : string {
    var out : array<uint8>
    if (at(c) == 'R' && at(c, 1) == '"') {
        c.pos += 2
        var delim : array<uint8>
        while (at(c) >= 0 && at(c) != '(') {
            delim |> push(uint8(at(c)))
            c.pos++
        }
        c.pos++
        let close = ")" + string(delim) + "\""
        while (at(c) >= 0 && !starts(c, close)) {
            out |> push(uint8(at(c)))
            c.pos++
        }
        c.pos += length(close)
        return string(out)
    }
    if (at(c) != '"') {
        let ctx <- [for (i in range(40)); uint8(max(at(c, i), 32))]
        panic("a string literal expected at {c.pos}: {string(ctx)}")
    }
    c.pos++
    while (at(c) >= 0 && at(c) != '"') {
        if (at(c) == '\\') {
            let e = at(c, 1)
            c.pos += 2
            if (e == 'n') {
                out |> push(uint8('\n'))
            } elif (e == 't') {
                out |> push(uint8('\t'))
            } elif (e == 'r') {
                out |> push(uint8('\r'))
            } elif (e == '0') {
                out |> push(0u8)
            } else {
                out |> push(uint8(e))   // \" \\ \' and the rest spell themselves
            }
        } else {
            out |> push(uint8(at(c)))
            c.pos++
        }
    }
    c.pos++
    return string(out)
}

// adjacent literals concatenate, as in C++
def private read_string(var c : Cursor) : string {
    return build_string() $(var w) {
        w |> write(read_literal(c))
        while (true) {
            skip_blank(c)
            if (at(c) == '"' || (at(c) == 'R' && at(c, 1) == '"')) {
                w |> write(read_literal(c))
            } else {
                break
            }
        }
    }
}

def private expect_ch(var c : Cursor; ch : int) {
    skip_blank(c)
    if (at(c) != ch) {
        panic("expected {to_char(ch)} at {c.pos}, got {to_char(at(c))}")
    }
    c.pos++
}

// `{ "a", "b", }` - a brace list of strings
def private read_list(var c : Cursor) : array<string> {
    var out : array<string>
    expect_ch(c, '{')
    while (true) {
        skip_blank(c)
        if (at(c) == '}') {
            c.pos++
            break
        }
        if (at(c) == '"' || (at(c) == 'R' && at(c, 1) == '"')) {
            out |> push(read_string(c))
        } else {
            // an expression (a token case's `token(10) + "..."`): skipped whole; its grammar is left out anyway
            var depth = 0
            while (at(c) >= 0 && !((at(c) == ',' || at(c) == '}') && depth == 0)) {
                depth += at(c) == '(' ? 1 : (at(c) == ')' ? -1 : 0)
                c.pos++
            }
            out |> push("")
        }
        skip_blank(c)
        if (at(c) == ',') {
            c.pos++
        }
    }
    return <- out
}

def private load(path : string) : Cursor {
    var c : Cursor
    let text = fread(path)
    if (empty(text)) {
        panic("cannot read {path}")
    }
    peek_data(text) $(d) {
        c.text |> resize(length(d))
        for (i in range(length(d))) {
            c.text[i] = d[i]
        }
    }
    return <- c
}

// every test_grammar(...) and test_schema(...) call of the integration test
def private lift_acceptance(root : string) : JsonValue? {
    var c <- load(path_join(root, "tests/test-grammar-integration.cpp"))
    var cases : array<JsonValue?>
    while (at(c) >= 0) {
        skip_blank(c)
        if (!is_ident_start(at(c))) {
            c.pos++
            continue
        }
        let name = read_ident(c)
        if (name != "test_grammar" && name != "test_schema") {
            continue
        }
        skip_blank(c)
        if (at(c) != '(') {
            continue
        }
        c.pos++
        skip_blank(c)
        if (at(c) != '"' && at(c) != 'R') {
            continue   // the helper's own definition
        }
        let desc = read_string(c)
        expect_ch(c, ',')
        skip_blank(c)
        let body = read_string(c)
        expect_ch(c, ',')
        var passing <- read_list(c)
        expect_ch(c, ',')
        var failing <- read_list(c)
        expect_ch(c, ')')
        if (name == "test_grammar" && find(body, "<[") >= 0) {
            continue   // a token reference: outside the byte acceptor's dialect
        }
        cases |> push(JV((kind = name == "test_grammar" ? "grammar" : "schema", name = desc, text = body,
                           passing = passing, failing = failing)))
    }
    return JV(cases)
}

// every test({ STATUS, "name", schema, grammar }) case of the conversion test
def private lift_conversion(root : string) : JsonValue? {
    var c <- load(path_join(root, "tests/test-json-schema-to-grammar.cpp"))
    var cases : array<JsonValue?>
    while (at(c) >= 0) {
        skip_blank(c)
        if (!is_ident_start(at(c))) {
            c.pos++
            continue
        }
        let name = read_ident(c)
        if (name != "test") {
            continue
        }
        skip_blank(c)
        if (at(c) != '(') {
            continue
        }
        c.pos++
        skip_blank(c)
        if (at(c) != '{') {
            continue   // the test function's own definition
        }
        c.pos++
        skip_blank(c)
        let status = read_ident(c)
        expect_ch(c, ',')
        skip_blank(c)
        let desc = read_string(c)
        expect_ch(c, ',')
        skip_blank(c)
        let schema = read_string(c)
        expect_ch(c, ',')
        skip_blank(c)
        let grammar = read_string(c)
        expect_ch(c, '}')
        expect_ch(c, ')')
        cases |> push(JV((status = status, name = desc, schema = schema, grammar = grammar)))
    }
    return JV(cases)
}

def private save(name : string; var js : JsonValue?) {
    let path = path_join(get_this_module_dir(), name)
    fopen(path, "wb") $(f) {
        if (f == null) {
            panic("cannot write {path}")
        }
        f |> fwrite(write_json(js))
        f |> fwrite("\n")
    }
    delete_json(js)
}

[export]
def main() {
    let argv <- get_command_line_arguments()
    var root = ""
    for (i in range(length(argv) - 1)) {
        if (argv[i] == "--") {
            root = argv[i + 1]
        }
    }
    if (empty(root)) {
        print("usage: daslang _lift_corpora.das -- <llama.cpp root>\n")
        return
    }
    var acc = lift_acceptance(root)
    print("_gbnf_corpus.json: {length(acc.value as _array)} cases\n")
    save("_gbnf_corpus.json", acc)
    var conv = lift_conversion(root)
    print("_schema_corpus.json: {length(conv.value as _array)} cases\n")
    save("_schema_corpus.json", conv)
}
