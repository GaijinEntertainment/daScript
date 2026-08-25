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

let private AUDIO_CARRIER = "modules/dasLLAMA/dasllama/dasllama_audio_embedder.das"
let private VISION_CARRIER = "modules/dasLLAMA/dasllama/dasllama_vision_embedder.das"

// The licensed family namers per modality - ARCHITECTURE.md §1 ledgers each with its reason.
var private AUDIO_SEAM_EXEMPT <- {
    "modules/dasLLAMA/dasllama/dasllama_asr.das",
    "modules/dasLLAMA/dasllama/dasllama_metal_tower.das",
    "modules/dasLLAMA/dasllama/dasllama_metal_asr_dec.das",
    "modules/dasLLAMA/benchmarks/asr/asr_bench.das",
    "utils/dasllama-convert/main.das"
}

var private VISION_SEAM_EXEMPT <- {
    "modules/dasLLAMA/dasllama/dasllama_metal_tower.das"
}

def private carried_families(carrier, kind_enum : string) : array<string> {
    var fams : array<string>
    var in_kind = false
    for (line in split(fread(carrier), "\n")) {
        if (starts_with(line, "enum {kind_enum}")) {
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

def private family_owned_names(fam : string; fam_path : string; var out : array<string>) {
    let prefix = slice(fam, length("dasllama_"))
    for (line in split(fread(fam_path), "\n")) {
        if (starts_with(line, "struct ") && !starts_with(line, "struct private ")) {
            out |> push(ident_prefix(slice(line, 7)))
        } elif (starts_with(line, "typedef ") && !starts_with(line, "typedef private ")) {
            out |> push(ident_prefix(slice(line, 8)))
        } elif (starts_with(line, "def ") && !starts_with(line, "def private ") && !starts_with(line, "def operator")) {
            let name = ident_prefix(slice(line, 4))
            // not a prefix test - the family word sits mid-identifier (stage_gemma4uv_embedder, set_gemma4v_q8)
            if (find(name, prefix) >= 0) {
                out |> push(name)
            }
        }
    }
}

def private collect_das_files(folder : string; var out : array<string>) {
    dir(folder) $(fname) {
        return if (fname == "." || fname == "..")
        let p = "{folder}/{fname}"
        let st = stat(p)
        if (st.is_valid && st.is_dir) {
            collect_das_files(p, out)
        } elif (ends_with(fname, ".das")) {
            out |> push(p)
        }
    }
}

def private check_family_seams(carrier, kind_enum : string; exempt : table<string>; route_hint : string) {
    for (fam in carried_families(carrier, kind_enum)) {
        let fam_path = "modules/dasLLAMA/dasllama/{fam}.das"
        if (!fexist(fam_path)) {
            gate_finding(carrier, "{kind_enum} arm {fam} has no {fam_path} - the carrier names a family the module does not carry, and its seams would go unpoliced")
            continue
        }
        var inscope names : array<string>
        family_owned_names(fam, fam_path, names)
        var inscope files : array<string>
        for (folder in ["modules/dasLLAMA/dasllama", "modules/dasLLAMA/benchmarks", "modules/dasLLAMA/performance",
                        "modules/dasLLAMA/harness", "utils/dasllama-server", "utils/dasllama-convert", "tutorials/dasLLAMA"]) {
            collect_das_files(folder, files)
        }
        for (p in files) {
            continue if (p == fam_path || p == carrier || key_exists(exempt, p))
            let text = strip_line_comments(fread(p))
            for (n in names) {
                if (contains_word(text, n)) {
                    gate_finding(p, "names {n}, which the {fam} family owns - the carrier is the one seam type; route through it ({route_hint})")
                }
            }
        }
    }
}

// REVIEW_GPU.md's backend-role partition, mechanically: every dasllama_<gpu>_*.das file's
// suffix is a role the checklist's list carries — a new role file adds its role to BOTH.
var private GPU_ROLES_METAL <- {
    "kernels", "common", "decode", "prefill", "gemm", "shapes", "tower", "asr_dec", "lens"
}

var private GPU_ROLES_VULKAN <- {
    "classes", "common", "decode", "prefill", "seams", "dispatch"
}

// REVIEW_GPU.md's fastmath default, mechanically: a [metal_kernel] declaring fastmath=false
// reds unless its file is listed here (the failing test ships with the kernel per the
// checklist; adding to this set is a gate edit the master's weakening rule reviews).
// Empty by design — no engine kernel has earned the opt-out.
var private FASTMATH_OFF_EXEMPT : table<string>

def private check_fastmath_default {
    var files : array<string>
    collect_das_files("modules/dasLLAMA/dasllama", files)
    for (p in files) {
        var line_no = 0
        for (line in split(fread(p), "\n")) {
            line_no++
            continue if (find(line, "fastmath=false") < 0 && find(line, "fastmath = false") < 0)
            if (!key_exists(FASTMATH_OFF_EXEMPT, p)) {
                gate_finding(p, line_no,
                    "kernel declares fastmath=false without an exemption naming the test that fails under fastmath - fastmath is the default; the opt-out ships its demonstrated breakage")
            }
        }
    }
    delete files
}

def private check_gpu_role_partition {
    dir("modules/dasLLAMA/dasllama") $(name) {
        return if (!ends_with(name, ".das"))
        for (backend in ["metal", "vulkan"]) {
            let prefix = "dasllama_{backend}_"
            continue if (!starts_with(name, prefix))
            var role = ""
            peek_data(name) $(d) {
                role = slice(d, length(prefix), length(d) - length(".das"))
            }
            let known = backend == "metal" ? key_exists(GPU_ROLES_METAL, role) : key_exists(GPU_ROLES_VULKAN, role)
            if (!known) {
                gate_finding("modules/dasLLAMA/dasllama/{name}",
                    "backend file with role '{role}' that REVIEW_GPU.md's partition does not carry - add the role to the checklist's list and to this gate's set in the same change")
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
    check_gpu_role_partition()
    check_fastmath_default()
    var inscope tut_texts : array<string>
    read_das_stripped(TUTORIAL_DIR, tut_texts)
    var inscope rst_texts : array<string>
    read_rst(RST_DIR, "dasLLAMA_", rst_texts)
    check_tutorial_floor(tut_texts, rst_texts)
    gate_descriptor_census("modules/dasLLAMA/.das_module", ["modules/dasLLAMA/dasllama"])
    check_family_seams(AUDIO_CARRIER, "AudioKind", AUDIO_SEAM_EXEMPT,
        "load_audio_embedder / encode_audio / audio_probe_proj_dim")
    check_family_seams(VISION_CARRIER, "VisionKind", VISION_SEAM_EXEMPT,
        "load_vision_embedder / encode_image / vision_proj_dim")
    return gate_verdict("dasllama")
}
