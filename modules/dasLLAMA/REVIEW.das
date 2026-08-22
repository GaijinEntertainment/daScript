options gen2

require strings
require daslib/strings_boost
require daslib/fio
require dastest/review_gate

// The mechanical half of modules/dasLLAMA/REVIEW.md (contract: REVIEW_COMMON.md at the repo
// root). Run from the repo root: bin/daslang modules/dasLLAMA/REVIEW.das — exit 0 clean,
// 1 with findings.

let private FACADE = "modules/dasLLAMA/dasllama/dasllama.das"
let private TUTORIAL_DIR = "tutorials/dasLLAMA"
let private RST_DIR = "doc/source/reference/tutorials"

def private is_ident_byte(b : int) : bool {
    return is_alnum(b) || b == '_'
}

// leading identifier run of `s` ("" when s starts with a non-ident byte)
def private ident_prefix(s : string) : string {
    var n = 0
    peek_data(s) $(d) {
        while (n < length(d) && is_ident_byte(int(d[n]))) {
            n++
        }
    }
    return slice(s, 0, n)
}

// whole-word occurrence: `name` present with no identifier byte on either side
def private contains_word(text : string; name : string) : bool {
    var found = false
    peek_data(text) $(d) {
        let nl = length(name)
        var from = 0
        while (!found) {
            let at = find(d, name, from)
            if (at < 0) {
                break
            }
            let okl = at == 0 || !is_ident_byte(int(d[at - 1]))
            let okr = at + nl >= length(d) || !is_ident_byte(int(d[at + nl]))
            found = okl && okr
            from = at + 1
        }
    }
    return found
}

def private word_in_any(texts : array<string>; name : string) : bool {
    for (t in texts) {
        if (contains_word(t, name)) {
            return true
        }
    }
    return false
}

// `text` with every // comment cut to end-of-line (string-literal-aware, so a "//" inside a
// quoted URL survives): a facade def mentioned ONLY in a comment must not satisfy the floor.
def private strip_line_comments(text : string) : string {
    return build_string() $(var b : StringBuilderWriter) {
        for (line in split(text, "\n")) {
            var cut = -1
            peek_data(line) $(d) {
                var in_str = false
                var i = 0
                while (i < length(d)) {
                    let c = int(d[i])
                    if (in_str) {
                        if (c == '\\') {
                            i++
                        } elif (c == '"') {
                            in_str = false
                        }
                    } elif (c == '"') {
                        in_str = true
                    } elif (c == '/' && i + 1 < length(d) && int(d[i + 1]) == '/') {
                        cut = i
                        break
                    }
                    i++
                }
            }
            b |> write(cut >= 0 ? slice(line, 0, cut) : line)
            b |> write("\n")
        }
    }
}

def private read_matching(folder : string; prefix : string; suffix : string; strip_comments : bool; var out : array<string>) {
    dir(folder) $(name) {
        if (name |> starts_with(prefix) && name |> ends_with(suffix)) {
            let text = fread("{folder}/{name}")
            out |> push(strip_comments ? strip_line_comments(text) : text)
        }
    }
}

// The tutorial-coverage floor: every def the facade file itself defines appears in at least
// one tutorial source and one tutorial page. REVIEW.md keeps the judgment half (covered
// means TAUGHT); this gate only proves presence.
def private check_tutorial_floor(tut_texts, rst_texts : array<string>) {
    var seen : table<string>
    let lines <- split(fread(FACADE), "\n")
    var line_no = 0
    for (line in lines) {
        line_no++
        continue if (!starts_with(line, "def "))
        let name = ident_prefix(slice(line, 4))
        continue if (empty(name) || key_exists(seen, name))
        seen |> insert(name)
        if (!word_in_any(tut_texts, name)) {
            gate_finding(FACADE, line_no,
                "facade def {name} appears in no {TUTORIAL_DIR}/*.das - the facade is the public API, and every facade def is demonstrated in a tutorial")
        }
        if (!word_in_any(rst_texts, name)) {
            gate_finding(FACADE, line_no,
                "facade def {name} appears in no {RST_DIR}/dasLLAMA_*.rst - the tutorial pages narrate every facade def (they are hand-written, not includes)")
        }
    }
}

[export]
def main() : int {
    if (!fexist("modules/dasLLAMA/REVIEW.das") || !fexist(FACADE)) {
        to_log(LOG_ERROR, "modules/dasLLAMA/REVIEW.das: run from the repo root\n")
        return 2
    }
    var inscope tut_texts : array<string>
    read_matching(TUTORIAL_DIR, "", ".das", true, tut_texts)
    var inscope rst_texts : array<string>
    read_matching(RST_DIR, "dasLLAMA_", ".rst", false, rst_texts)
    check_tutorial_floor(tut_texts, rst_texts)
    return gate_verdict("dasllama")
}
