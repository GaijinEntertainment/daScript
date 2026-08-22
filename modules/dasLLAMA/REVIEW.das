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

// .das sources arrive comment-stripped: a name mentioned only in a comment must not count
def private read_das_stripped(folder : string; var out : array<string>) {
    dir(folder) $(name) {
        if (name |> ends_with(".das")) {
            out |> push(strip_line_comments(fread("{folder}/{name}")))
        }
    }
}

def private read_rst(folder : string; prefix : string; var out : array<string>) {
    dir(folder) $(name) {
        if (name |> starts_with(prefix) && name |> ends_with(".rst")) {
            out |> push(fread("{folder}/{name}"))
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

let private CARRIER = "modules/dasLLAMA/dasllama/dasllama_audio_embedder.das"

// Files licensed to name a carrier-carried audio family outside its own file: the carrier and
// the ASR facade (their union fields and one-line arms), the metal family hooks, the per-family
// bake tool, and tests.
var private SEAM_EXEMPT <- {
    "modules/dasLLAMA/dasllama/dasllama_audio_embedder.das",
    "modules/dasLLAMA/dasllama/dasllama_asr.das",
    "modules/dasLLAMA/dasllama/dasllama_metal_tower.das",
    "modules/dasLLAMA/dasllama/dasllama_metal_asr_dec.das",
    "utils/dasllama-convert/main.das"
}

// the carried set is the AudioKind enum itself: every arm but `none` is a family
def private carried_families() : array<string> {
    var fams : array<string>
    var in_kind = false
    for (line in split(fread(CARRIER), "\n")) {
        if (starts_with(line, "enum AudioKind")) {
            in_kind = true
        } elif (in_kind && starts_with(line, "}")) {
            break
        } elif (in_kind) {
            let name = ident_prefix(slice(line, 4))
            if (!empty(name) && name != "none") {
                fams |> push("dasllama_{name}")
            }
        }
    }
    return <- fams
}

// what the family owns: its public structs, and every def carrying the family's name prefix
// (load_gemma4a_encoder, gemma4a_probe_proj_dim, gemma4a_encode_all, ...)
def private family_owned_names(fam : string; fam_path : string; var out : array<string>) {
    let prefix = slice(fam, length("dasllama_"))
    for (line in split(fread(fam_path), "\n")) {
        if (starts_with(line, "struct ") && !starts_with(line, "struct private ")) {
            out |> push(ident_prefix(slice(line, 7)))
        } elif (starts_with(line, "def ") && !starts_with(line, "def private ") && !starts_with(line, "def operator")) {
            let name = ident_prefix(slice(line, 4))
            if (starts_with(name, "{prefix}_") || starts_with(name, "load_{prefix}")) {
                out |> push(name)
            }
        }
    }
}

def private check_audio_family_seams() {
    for (fam in carried_families()) {
        let fam_path = "modules/dasLLAMA/dasllama/{fam}.das"
        continue if (!fexist(fam_path))
        var inscope names : array<string>
        family_owned_names(fam, fam_path, names)
        for (folder in ["modules/dasLLAMA/dasllama", "utils/dasllama-server", "utils/dasllama-convert", "tutorials/dasLLAMA"]) {
            dir(folder) $(fname) {
                return if (!ends_with(fname, ".das"))
                let p = "{folder}/{fname}"
                return if (p == fam_path || key_exists(SEAM_EXEMPT, p))
                let text = strip_line_comments(fread(p))
                for (n in names) {
                    if (contains_word(text, n)) {
                        gate_finding(p, "names {n}, which the {fam} family owns - the AudioEmbedder carrier is the one seam type; route through it (load_audio_embedder / encode_audio / audio_probe_proj_dim)")
                    }
                }
            }
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
    read_das_stripped(TUTORIAL_DIR, tut_texts)
    var inscope rst_texts : array<string>
    read_rst(RST_DIR, "dasLLAMA_", rst_texts)
    check_tutorial_floor(tut_texts, rst_texts)
    check_audio_family_seams()
    return gate_verdict("dasllama")
}
