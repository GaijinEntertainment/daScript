options gen2
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost public
require lr1/lr1
require lr1/gbnf
require daslib/fio
require daslib/module_path
require daslib/json
require daslib/json_boost
require strings

// The GBNF reader over the lifted acceptance corpus (`_gbnf_corpus.json`: each case a grammar text, the
// strings it must accept and the strings it must refuse) and over the sample grammar files under `_gbnf/`.
// A schema case in the corpus belongs to the schema compiler's test, not this one.

//! one acceptance case of the corpus
struct CorpusCase {
    kind : string
    name : string
    text : string
    passing : array<string>
    failing : array<string>
}

def load_corpus() : array<CorpusCase> {
    var out : array<CorpusCase>
    let path = path_join(get_this_module_dir(), "_gbnf_corpus.json")
    var err = ""
    var js = read_json(fread(path), err)
    if (js != null) {
        out <- from_JV(js, type<array<CorpusCase>>)
        delete_json(js)
    }
    return <- out
}

[test]
def test_gbnf_corpus(t : T?) {
    t |> run("the lifted grammar corpus: every byte-level case builds, accepts its sentences, refuses its non-sentences") @(tt : T?) {
        var cases <- load_corpus()
        tt |> success(length(cases) > 40, "the corpus loaded ({length(cases)} cases)")
        var grammars = 0
        for (c in cases) {
            continue if (c.kind != "grammar")
            grammars++
            var err = ""
            var g <- gbnf_grammar(c.text, err)
            tt |> equal(err, "", "{c.name}: the grammar builds")
            continue if (!empty(err))
            for (s in c.passing) {
                tt |> success(lr_matches(g, s), "{c.name}: accepts {escape(s)}")
            }
            for (s in c.failing) {
                tt |> success(!lr_matches(g, s), "{c.name}: refuses {escape(s)}")
            }
        }
        tt |> equal(grammars, 12, "every byte-level grammar case ran")
    }
}

[test]
def test_gbnf_sample_grammars(t : T?) {
    t |> run("the sample grammar files build, and json.gbnf judges JSON text") @(tt : T?) {
        let dir = path_join(get_this_module_dir(), "_gbnf")
        var names <- ["arithmetic", "c", "chess", "english", "japanese", "json", "json_arr", "list"]
        for (n in names) {
            var err = ""
            let g <- gbnf_grammar(fread(path_join(dir, "{n}.gbnf")), err)
            tt |> equal(err, "", "{n}.gbnf builds: {err}")
        }
        var err = ""
        var json_g <- gbnf_grammar(fread(path_join(dir, "json.gbnf")), err)
        return if (!empty(err))
        tt |> success(lr_matches(json_g, "\{\"a\": [1, 2.5e3, true, null, \"x\\u00e9\\n\"], \"b\": \{\}\}\n"), "a JSON document")
        tt |> success(lr_matches(json_g, "\{\"a\": []\}\n"), "an empty array as a member")
        tt |> success(!lr_matches(json_g, "[]\n"), "the root is an object, so a bare array is not a document")
        tt |> success(!lr_matches(json_g, "\{\"a\": \}"), "a missing value")
        tt |> success(!lr_matches(json_g, "[1,]"), "a trailing comma")
        tt |> success(!lr_matches(json_g, "\{\"a\": 01\}"), "a leading zero")
        // the viable prefix: inside a string, a raw control character is refused where an escape is not
        var st <- lr_start(json_g)
        tt |> success(lr_step_bytes(json_g, st, "\{\"key\": \"val"), "mid-string")
        tt |> success(!lr_step(json_g, st, 0x01u8), "a control byte inside a string is refused")
        tt |> success(lr_step_bytes(json_g, st, "\\n\""), "an escape and the close quote")
        let nb : ByteSet = lr_next_bytes(json_g, st)
        tt |> success(nb[','] && nb['}'] && !nb['"'] && !nb['a'], "after a member: a comma or the close")
        // UTF-8 above ASCII passes through the negated class as whole characters
        tt |> success(lr_matches(json_g, "\{\"k\": \"héllo ✓\"\}\n"), "non-ASCII text inside a string")   // nolint:STYLE039 - the non-ASCII bytes are the point
        var st2 <- lr_start(json_g)
        tt |> success(lr_step_bytes(json_g, st2, "\{\"h"), "a key opened")
        tt |> success(lr_step(json_g, st2, 0xE2u8), "the lead byte of a 3-byte character")
        tt |> success(!lr_step(json_g, st2, uint8('a')), "an ASCII byte where a continuation byte is due")
        tt |> success(lr_step(json_g, st2, 0x9Cu8) && lr_step(json_g, st2, 0x93u8), "the two continuation bytes")
        tt |> success(lr_step_bytes(json_g, st2, "\": 1\}\n") && lr_at_end(json_g, st2), "closed and complete")
    }
}

[test]
def test_gbnf_any_character(t : T?) {
    t |> run("`.` is any character but a newline, one whole UTF-8 character at a time") @(t : T?) {
        var err = ""
        let g <- gbnf_grammar("root ::= \"<\" . . \">\"", err)
        t |> equal(err, "", "builds")
        t |> success(lr_matches(g, "<ab>"), "two ASCII characters")
        t |> success(lr_matches(g, "<é✓>"), "two multi-byte characters")   // nolint:STYLE039 - the non-ASCII bytes are the point
        t |> success(!lr_matches(g, "<a>"), "one character is not two")
        t |> success(!lr_matches(g, "<abc>"), "three are not two")
        t |> success(lr_matches(g, "<a\n>"), "a newline is a character too, as in the reference")
    }
}

[test]
def test_gbnf_forms(t : T?) {
    t |> run("the reader's forms the corpora only build: escapes, a continued rule, a repeated multi-byte literal, the next rule's head") @(t : T?) {
        var err = ""
        let esc <- gbnf_grammar("root ::= \"\\t\" \"\\r\" [\\]\\-\\^] \"\\u00e9\" \"\\U0001F600\"", err)
        t |> equal(err, "", "every escape form reads")
        t |> success(lr_matches(esc, "\t\r]é😀") && lr_matches(esc, "\t\r-é😀") && lr_matches(esc, "\t\r^é😀"), "... and spells the bytes it names")   // nolint:STYLE039 - the non-ASCII bytes are the point
        t |> success(!lr_matches(esc, "\t\rxé😀"), "a byte outside the class")   // nolint:STYLE039 - the non-ASCII bytes are the point
        let cont <- gbnf_grammar("root ::= \"a\"\n  | \"b\"\nnext ::= \"c\"\nroot ::= next", err)
        t |> success(find(err, "defined twice") >= 0, "a rule defined twice is refused: {err}")
        err = ""
        let cont2 <- gbnf_grammar("root ::= \"a\"\n  | \"b\" next\nnext ::= \"c\"", err)
        t |> equal(err, "", "a `|` on the next line continues the rule; `next ::=` starts another")
        t |> success(lr_matches(cont2, "a") && lr_matches(cont2, "bc") && !lr_matches(cont2, "b") && !lr_matches(cont2, "ac"), "... with the alternatives split where the rule heads are")
        let rep <- gbnf_grammar("root ::= \"ab\"? \"c\"", err)
        t |> equal(err, "", "a repeated multi-byte literal reads")
        t |> success(lr_matches(rep, "abc") && lr_matches(rep, "c") && !lr_matches(rep, "ac") && !lr_matches(rep, "bc"), "... as a group, never its last byte alone")
    }
}

[test]
def test_gbnf_reader_errors(t : T?) {
    t |> run("the reader names what is wrong") @(t : T?) {
        var err = ""
        gbnf_grammar("root ::= foo", err)
        t |> success(find(err, "never defined") >= 0, "an undefined rule: {err}")
        err = ""
        gbnf_grammar("start ::= \"x\"", err)
        t |> success(find(err, "no root") >= 0, "no root: {err}")
        err = ""
        gbnf_grammar("root ::= \"x", err)
        t |> success(find(err, "unterminated") >= 0, "an unterminated literal: {err}")
        err = ""
        gbnf_grammar("root ::= <[10]> \"x\"", err)
        t |> success(find(err, "token") >= 0, "a token reference by id: {err}")
        err = ""
        gbnf_grammar("root ::= <bos> \"x\"", err)
        t |> success(find(err, "token") >= 0, "a token reference by name: {err}")
        err = ""
        gbnf_grammar("root ::= !<eos> \"x\"", err)
        t |> success(find(err, "token") >= 0, "a negated token reference: {err}")
        err = ""
        gbnf_grammar("root ::= \"x\"\{1,{GBNF_REPEAT_CAP + 1}\}", err)
        t |> success(find(err, "past the cap") >= 0, "a repetition past the cap: {err}")
        err = ""
        gbnf_grammar("root ::= \"x\"\{{GBNF_REPEAT_CAP + 1}\}", err)
        t |> success(find(err, "past the cap") >= 0, "an exact count past the cap: {err}")
        err = ""
        let capped <- gbnf_grammar("root ::= \"x\"\{0,{GBNF_REPEAT_CAP}\}", err)
        t |> equal(err, "", "the cap itself builds")
        t |> success(lr_matches(capped, "xxx") && !lr_matches(capped, "y"), "... and accepts its language")
        err = ""
        gbnf_grammar("root ::= root", err)
        t |> success(find(err, "derives no terminal") >= 0, "a rule that never bottoms out: {err}")
        // every other refusal names its site
        for (bad in ["root ::= \"\\q\"", "root ::= \"\\xZZ\"", "root ::= [abc", "root ::= \"a\"\{x\}", "root ::= \"a\"\{1,2", "root ::= (\"a\"", "root ::= \"a\" %", "::= \"a\"", "root \"a\""]) {
            err = ""
            gbnf_grammar(bad, err)
            t |> success(!empty(err), "refused: {escape(bad)} - {err}")
        }
        err = ""
        gbnf_grammar("root ::= \"\xC3\"", err)
        t |> success(!empty(err), "malformed UTF-8 in a literal: {err}")
    }
}
