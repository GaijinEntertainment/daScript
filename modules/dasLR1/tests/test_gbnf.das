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
        t |> success(find(err, "token") >= 0, "a token reference: {err}")
        err = ""
        gbnf_grammar("root ::= root", err)
        t |> success(find(err, "derives no terminal") >= 0, "a rule that never bottoms out: {err}")
    }
}
