options gen2

require strings
require daslib/strings_boost
require daslib/strings_convert
require daslib/fio
require dastest/review_gate

// The mechanical half of modules/dasLLAMA/REVIEW.md (contract: REVIEW_COMMON.md at the repo
// root). Run from the repo root: bin/daslang modules/dasLLAMA/REVIEW.das — exit 0 clean,
// 1 with findings.

let private FACADE = "modules/dasLLAMA/dasllama/dasllama.das"
// The TTS surface is a module of its own that dasllama.das re-exports publicly, so its defs
// are facade defs and the coverage floor reads both files.
let private FACADE_FILES = [FACADE, "modules/dasLLAMA/dasllama/dasllama_tts.das"]
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

// leading identifier run of `s` with dots kept ("p.base_pso") - a race arm names its pipeline
// through the field of the pair its shared shell returned
def private path_prefix(s : string) : string {
    var n = 0
    peek_data(s) $(d) {
        while (n < length(d) && (is_ident_byte(int(d[n])) || int(d[n]) == '.')) {
            n++
        }
    }
    return slice(s, 0, n)
}

// trailing identifier run of `s` ("" when s ends with a non-ident byte)
def private ident_suffix(s : string) : string {
    var n = 0
    peek_data(s) $(d) {
        n = length(d)
        while (n > 0 && is_ident_byte(int(d[n - 1]))) {
            n--
        }
    }
    return slice(s, n)
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

// `finalize` is the language's own teardown hook - the compiler calls it at `delete`, so no
// tutorial calls it by name and it carries no floor duty.
var private FLOOR_HOOKS <- {"finalize"}

def private check_floor_of(path : string; var seen : table<string>; tut_texts, rst_texts : array<string>) {
    var line_no = 0
    for (line in split(fread(path), "\n")) {
        line_no++
        continue if (!starts_with(line, "def ") || starts_with(line, "def private ") || starts_with(line, "def operator"))
        let name = ident_prefix(slice(line, 4))
        continue if (empty(name) || key_exists(FLOOR_HOOKS, name) || key_exists(seen, name))
        seen |> insert(name)
        if (!word_in_any(tut_texts, name)) {
            gate_finding(path, line_no,
                "facade def {name} appears in no {TUTORIAL_DIR}/*.das - the facade is the public API, and every facade def is demonstrated in a tutorial")
        }
        if (!word_in_any(rst_texts, name)) {
            gate_finding(path, line_no,
                "facade def {name} appears in no {RST_DIR}/dasLLAMA_*.rst - the tutorial pages narrate every facade def (they are hand-written, not includes)")
        }
    }
}

// The tutorial-coverage floor: every def the facade files themselves define appears in at
// least one tutorial source and one tutorial page. REVIEW.md keeps the judgment half (covered
// means TAUGHT); this gate only proves presence.
def private check_tutorial_floor(tut_texts, rst_texts : array<string>) {
    var seen : table<string>
    for (path in FACADE_FILES) {
        check_floor_of(path, seen, tut_texts, rst_texts)
    }
}

let private AUDIO_CARRIER = "modules/dasLLAMA/dasllama/dasllama_audio_embedder.das"
let private VISION_CARRIER = "modules/dasLLAMA/dasllama/dasllama_vision_embedder.das"

// The licensed family namers per modality - the ARCHITECTURE_MEDIA.md carrier charters (§1.7,
// §1.7b) ledger each with its reason.
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

// The backend-role partition, mechanically: every dasllama_<gpu>_*.das file's suffix is a role
// ARCHITECTURE_GPU.md sec.1.5's role table carries — a new role file adds its role to BOTH.
var private GPU_ROLES_METAL <- {
    "kernels", "common", "decode", "prefill", "gemm", "shapes", "tower", "asr_dec", "lens", "mtp_gemma"
}

var private GPU_ROLES_VULKAN <- {
    "classes", "common", "decode", "prefill", "seams", "dispatch"
}

// the fastmath default, mechanically: a [metal_kernel] declaring fastmath=false
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
                    "backend file with role '{role}' that the partition does not carry - add the role to ARCHITECTURE_GPU.md sec.1.5's role table and this gate's set in the same change")
            }
        }
    }
}

// A serialized exe restores globals as DATA - a function value assigned in a declaration
// initializer arrives NULL, and only a boot-restore [init] heals it (dasllama_config's
// restore_dlim_source_defaults states the law). So every `var g_x = @@fn` declaration must
// have an `if (g_x == null)` restore guard in the SAME file, or the first exe run to reach
// its invoke dies with `invoke null function` while every -jit gate stays green - the vulkan
// decode-mirror claim shipped exactly that way.
// A hand-binding arm (a race/knockout `$(enc)` block spelling kn_buffer/kn_kargs literals)
// must bind at the numbers its target kernel class DECLARES. A mis-numbered arm dispatches,
// reads the wrong buffer, and race_envelope_ok fails CLOSED - the twin just loses and the
// board quietly serves the slower kernel (the mx4 race shipped exactly that way). The target
// class resolves from the arm's kn_tgmem constant, or from kn_pipeline's pso built by
// pipeline_from_source in the same function.
def private rb_scan_classes(text : string; var declared, class_names : table<string>; var class_base : table<string; string>; var kernel_class : table<string; string>) {
    var cur = ""
    for (line in split(text, "\n")) {
        if (line |> starts_with("class ")) {
            var rest = slice(line, length("class "))
            if (rest |> starts_with("template ")) {
                rest = slice(rest, length("template "))
            }
            cur = ident_prefix(rest)
            class_names |> insert(cur)
            let colon = find(rest, " : ")
            if (colon >= 0) {
                class_base[cur] = ident_prefix(slice(rest, colon + 3))
            }
        } elif (line |> starts_with("}")) {
            cur = ""
        } elif (!empty(cur)) {
            let mk = find(line, "[metal_kernel(")
            if (mk >= 0) {
                let nm = find(line, "name", mk)
                if (nm >= 0) {
                    let q1 = find(line, "\"", nm)
                    if (q1 >= 0) {
                        let kn = slice(line, q1 + 1)
                        kernel_class[slice(kn, 0, find(kn, "\""))] = cur
                    }
                }
            }
            let bat = find(line, "@binding = ")
            if (bat >= 0) {
                let n = try_to_int(ident_prefix(slice(line, bat + length("@binding = ")))) ?? -1
                if (n >= 0) {
                    let kind = find(line, "@uniform") >= 0 ? "u" : "b"
                    declared |> insert("{cur}|{kind}|{n}")
                }
            }
        }
    }
}

def private rb_class_declares(declared : table<string>; class_base : table<string; string>; cls, kind : string; n : int) : bool {
    var c = cls
    while (!empty(c)) {
        if (key_exists(declared, "{c}|{kind}|{n}")) {
            return true
        }
        var nxt = ""
        class_base |> get(c) $(v) {
            nxt = v
        }
        c = nxt
    }
    return false
}

// "<Class>_<kernel>" / "<kernel>" / with a "_tgmem"/"_entry"/"_fastmath" tail -> the owning
// class. Kernel names themselves may end in "_msl" (the annotation names the MSL global), so
// the kernel lookup tries the token both with and without that suffix.
def private rb_resolve_class(token : string; class_names : table<string>; kernel_class : table<string; string>) : string {
    var t = token
    for (suffix in ["_msl_tgmem", "_msl_entry", "_msl_fastmath", "_tgmem", "_entry", "_fastmath"]) {
        if (t |> ends_with(suffix)) {
            t = slice(t, 0, length(t) - length(suffix))
            break
        }
    }
    for (cand in [t, "{t}_msl"]) {
        var owner = ""
        kernel_class |> get(cand) $(v) {
            owner = v
        }
        if (!empty(owner)) {
            return owner
        }
    }
    let us = find(t, "_")
    if (us > 0 && key_exists(class_names, slice(t, 0, us))) {
        return slice(t, 0, us)
    }
    return ""
}

// the comma-separated argument at index `i` of a call whose text starts right after its open
// paren, or "" when the call has no such argument on this line
def private rb_call_arg(args : string; i : int) : string {
    var from = 0
    var n = 0
    while (n < i) {
        let c = find(args, ",", from)
        return "" if (c < 0)
        from = c + 1
        n++
    }
    let comma = find(args, ",", from)
    let close = find(args, ")", from)
    let stop = comma >= 0 && (close < 0 || comma < close) ? comma : close
    return strip(stop < 0 ? slice(args, from) : slice(args, from, stop))
}

// the integer between a call's last comma and its closing paren, or -1
def private rb_trailing_int(line : string) : int {
    let close = find(line, ")")
    if (close < 0) {
        return -1
    }
    var last_comma = -1
    var from = 0
    while (true) {
        let c = find(line, ",", from)
        if (c < 0 || c > close) {
            break
        }
        last_comma = c
        from = c + 1
    }
    if (last_comma < 0) {
        return -1
    }
    return try_to_int(strip(slice(line, last_comma + 1, close))) ?? -1
}

// The per-function pipeline map: the calls that bind a local to a class's compiled source -
// `pipeline_from_source` over a literal global, and the shared `race_pso_pair` shell in either
// spelling - plus the local-to-local reads of one. True where the line is such a bind and no
// later check reads it.
def private rb_track_pso(line : string; class_names : table<string>; kernel_class : table<string; string>;
                         var pso_class : table<string; string>) : bool {
    let pf = find(line, "pipeline_from_source(")
    if (pf >= 0) {
        let pvar = rb_bound_var(line)
        let args = slice(line, pf + length("pipeline_from_source("))
        let c1 = find(args, ",")
        let src = strip(slice(args, c1 + 1, find(args, ",", c1 + 1)))
        let cls = rb_resolve_class(src, class_names, kernel_class)
        if (!empty(pvar) && !empty(cls)) {
            pso_class[pvar] = cls
        }
        return true
    }
    // the shared shell hands both pipelines back in one RacePsoPair, so each arm's class comes
    // off the call's stems (race_pso_pair_stamp) or its source triples
    let stamped = find(line, "race_pso_pair_stamp(")
    let plain = find(line, "race_pso_pair(")
    if (stamped >= 0 || plain >= 0) {
        let head = stamped >= 0 ? "race_pso_pair_stamp(" : "race_pso_pair("
        let args = slice(line, (stamped >= 0 ? stamped : plain) + length(head))
        let pvar = rb_bound_var(line)
        return true if (empty(pvar))
        let bcls = rb_resolve_class(rb_call_arg(args, 2), class_names, kernel_class)
        let tcls = rb_resolve_class(rb_call_arg(args, stamped >= 0 ? 3 : 5), class_names, kernel_class)
        if (!empty(bcls)) {
            pso_class["{pvar}.base_pso"] = bcls
        }
        if (!empty(tcls)) {
            pso_class["{pvar}.twin_pso"] = tcls
        }
        return true
    }
    // an arm reads the pair's pipelines into locals before its timed block
    let asg = find(line, " = ")
    if (asg > 0) {
        var lhs = strip(slice(line, 0, asg))
        if (lhs |> starts_with("let ") || lhs |> starts_with("var ")) {
            lhs = slice(lhs, 4)
        }
        let cls = pso_class?[path_prefix(strip(slice(line, asg + 3)))] ?? ""
        if (!empty(cls) && !empty(ident_prefix(lhs))) {
            pso_class[ident_prefix(lhs)] = cls
        }
    }
    return false
}

// the name a `var x = ` / `let x = ` line binds ("" when the line binds nothing)
def private rb_bound_var(line : string) : string {
    var v = strip(line)
    v = slice(v, find(v, "var ") >= 0 ? find(v, "var ") + 4 : 0)
    return ident_prefix(v)
}

def private check_race_bind_numbers {
    var declared : table<string>
    var class_base : table<string; string>
    var kernel_class : table<string; string>
    var files : array<string>
    dir("modules/dasLLAMA/dasllama") $(name) {
        if (name |> ends_with(".das")) {
            files |> push(name)
        }
    }
    var class_names : table<string>
    for (name in files) {
        rb_scan_classes(fread("modules/dasLLAMA/dasllama/{name}"), declared, class_names, class_base, kernel_class)
    }
    for (name in files) {
        let path = "modules/dasLLAMA/dasllama/{name}"
        var pso_class : table<string; string>   // per-function psoVar -> class
        var target = ""
        var line_no = 0
        for (line in split(fread(path), "\n")) {
            line_no++
            if (line |> starts_with("def ")) {
                pso_class |> clear()   // reset, never delete: the table is reused next function
                target = ""
                continue
            }
            continue if (rb_track_pso(line, class_names, kernel_class, pso_class))
            let kp = find(line, "kn_pipeline(enc, ")
            if (kp >= 0) {
                target = pso_class?[path_prefix(slice(line, kp + length("kn_pipeline(enc, ")))] ?? ""
                continue
            }
            let kt = find(line, "kn_tgmem(enc, ")
            if (kt >= 0) {
                let cls = rb_resolve_class(ident_prefix(slice(line, kt + length("kn_tgmem(enc, "))), class_names, kernel_class)
                if (!empty(cls)) {
                    target = cls
                }
                continue
            }
            continue if (empty(target))
            let kb = find(line, "kn_buffer(enc, ")
            if (kb >= 0) {
                let n = rb_trailing_int(line)
                // kind-agnostic on purpose: a hand arm legitimately binds a @uniform slot
                // as a 4-byte buffer; the hazard is a number the class never declared
                if (n >= 0 && !rb_class_declares(declared, class_base, target, "b", n) &&
                        !rb_class_declares(declared, class_base, target, "u", n)) {
                    gate_finding(path, line_no, "hand-bound buffer at binding {n}, which '{target}' does not declare - the arm dispatches, reads the wrong buffer, and the race fails closed (the twin silently loses); bind at the class's declared @binding numbers")
                }
            }
            let kk = find(line, "kn_kargs(enc, ")
            if (kk >= 0) {
                let n = rb_trailing_int(line)
                if (n >= 0 && !rb_class_declares(declared, class_base, target, "u", n)) {
                    gate_finding(path, line_no, "hand-bound kargs at binding {n}, which '{target}' does not declare as a uniform - the arm dispatches with garbage args; bind at the class's declared @binding")
                }
            }
        }
    }
}

def private check_exe_fn_global_restore {
    dir("modules/dasLLAMA/dasllama") $(name) {
        if (!(name |> ends_with(".das"))) {
            return
        }
        let path = "modules/dasLLAMA/dasllama/{name}"
        let text = fread(path)
        var line_no = 0
        for (line in split(text, "\n")) {
            line_no++
            var rest = ""
            if (line |> starts_with("var private ")) {
                rest = slice(line, length("var private "))
            } elif (line |> starts_with("var ")) {
                rest = slice(line, length("var "))
            } else {
                continue
            }
            let gname = ident_prefix(rest)
            continue if (!(gname |> starts_with("g_")))
            let eq = find(line, "= @@")
            continue if (eq < 0 || (find(line, "//") >= 0 && find(line, "//") < eq))
            if (find(text, "if ({gname} == null)") < 0) {
                gate_finding(path, line_no, "function global `{gname}` has a declaration initializer but no `if ({gname} == null)` boot-restore guard - a serialized exe restores it as NULL and the first invoke dies (add it to the file's restore [init])")
            }
        }
    }
}

// The ONE release, mechanically. A carrier's image backing is either a mapping or a chunk, and
// image_chunk_alloc hands back an image-page-aligned offset INTO a das-heap array<uint8> - so
// munmap on a chunk SUCCEEDS and punches a hole in the process heap, which kills an unrelated
// malloc long afterwards with nothing to point at the caller. Only image_backing_release tells
// the two apart. fmap_close is therefore legal in exactly two places: inside that release, and
// inside a function that opened the mapping it closes.
def private report_stray_closes(path, fn : string; opened : bool; closes : array<int>) {
    return if (fn == "image_backing_release" || opened)
    for (ln in closes) {
        gate_finding(path, ln,
            "fmap_close on a backing this function did not open - a carrier's image_map may be a chunk, and munmap on one succeeds and holes the heap; release it through image_backing_release")
    }
}

def private check_image_backing_release {
    var files : array<string>
    for (folder in ["modules/dasLLAMA/dasllama", "modules/dasLLAMA/tests", "modules/dasLLAMA/benchmarks",
                    "modules/dasLLAMA/performance", "modules/dasLLAMA/harness", "utils/dasllama-server",
                    "utils/dasllama-convert", "tutorials/dasLLAMA"]) {
        collect_das_files(folder, files)
    }
    for (p in files) {
        var fn = ""
        var opened = false
        var closes : array<int>
        var line_no = 0
        for (line in split(strip_line_comments(fread(p)), "\n")) {
            line_no++
            if (line |> starts_with("def ")) {
                report_stray_closes(p, fn, opened, closes)
                var rest = slice(line, 4)
                if (rest |> starts_with("private ")) {
                    rest = slice(rest, length("private "))
                }
                fn = ident_prefix(rest)
                opened = false
                closes |> clear()
            }
            if (contains_word(line, "fmap_open")) {
                opened = true
            }
            if (contains_word(line, "fmap_close")) {
                closes |> push(line_no)
            }
        }
        report_stray_closes(p, fn, opened, closes)
    }
    delete files
}

// ===== the image rail's two mechanical checks (REVIEW_IMAGE.md) =====

let private DASLLAMA_DIR = "modules/dasLLAMA/dasllama"
let private IMAGE_FILE = "modules/dasLLAMA/dasllama/dasllama_image.das"
let private TTS_BLOCKS_FILE = "modules/dasLLAMA/dasllama/dasllama_tts_blocks.das"

// The stamp of the layout closure - every Archive serializer body, build_image, parse_image,
// the TTS block file's *_prepare mints, the layout helpers and the layout constants' declaration
// lines, hashed in file order. A closure change with IMAGE_VERSION unmoved is red; the finding prints the
// value to re-stamp with.
let private IMAGE_LAYOUT_STAMP_VERSION = 39
let private IMAGE_LAYOUT_STAMP_HASH = 0x5c8989fb70eb6c07ul

// The helpers that decide WHERE bytes land: the page pad, the plane and total sizing, the
// writer's append / zero-fill / header patch, the header's scalar stores, and the lattice
// plane's section name. Changing one moves every plane offset (or renames every lattice
// section) while every serializer body reads the same.
var private LAYOUT_HELPERS <- {
    "pad_to_page", "plane_end", "image_total_bytes",
    "w_append", "w_zeros", "w_header", "store_u32", "store_u64", "kq_plane_name"
}

// The constants those helpers place bytes by, and the bands the two streaming writers stage
// through. Their declaration lines are hashed with the bodies, so a value change reds the same
// way an edited body does - the closure is deliberately wider than the byte-moving set, and a
// change that provably moves nothing re-stamps the hash alone.
var private LAYOUT_CONSTANTS <- {
    "IMAGE_PAGE", "IMAGE_HEADER_BYTES", "SECTION_TABLE_SLACK",
    "DWRITE_STAGING_BAND", "METAL_BAND_BLOCKS"
}

def private is_archive_serializer(header : string) : bool {
    return (header |> starts_with("def serialize") || header |> starts_with("def private serialize")) && find(header, "Archive") >= 0
}

// A meta serializer hand-lists a STRUCT's fields, so its count can be pinned; the bulk payload
// forms take an `array` and have no field list of their own.
def private is_meta_serializer(header : string) : bool {
    return is_archive_serializer(header) && find(header, "array<") < 0
}

//! the `*_prepare` mints that place image bytes are the TTS block file's (`conv1d_prepare`, `linear_prepare`); a driver's prepare (`vk_rdec_prepare`, `mirror_prepare`) places none
def private is_layout_function(file : string; header : string) : bool {
    return true if (is_archive_serializer(header))
    var rest = slice(header, 4)
    if (rest |> starts_with("private ")) {
        rest = slice(rest, length("private "))
    }
    let name = ident_prefix(rest)
    return true if (key_exists(LAYOUT_HELPERS, name))
    return name == "build_image" || name == "parse_image" || (find(name, "_prepare") >= 0 && "{DASLLAMA_DIR}/{file}" == TTS_BLOCKS_FILE)
}

def private read_lf(path : string) : string {
    return replace(fread(path), "\r\n", "\n")
}

// the declaration lines of the layout constants a file declares, in file order
def private layout_constant_lines(path : string) : array<tuple<line : int; body : string>> {
    var out : array<tuple<line : int; body : string>>
    var line_no = 0
    for (raw in split(strip_line_comments(read_lf(path)), "\n")) {
        line_no++
        var rest = ""
        if (raw |> starts_with("let private ")) {
            rest = slice(raw, length("let private "))
        } elif (raw |> starts_with("let ")) {
            rest = slice(raw, length("let "))
        } else {
            continue
        }
        if (key_exists(LAYOUT_CONSTANTS, ident_prefix(rest))) {
            out |> push((line = line_no, body = strip(raw)))
        }
    }
    return <- out
}

// every top-level function of the file whose header `pick` accepts: its header line number and
// its text through the closing brace, comments stripped
def private function_bodies(path : string; pick : block<(header : string) : bool>) : array<tuple<line : int; body : string>> {
    var out : array<tuple<line : int; body : string>>
    var inscope lines : array<string>
    var header_line = 0
    var line_no = 0
    for (raw in split(strip_line_comments(read_lf(path)), "\n")) {
        line_no++
        if (raw |> starts_with("def ")) {
            if (header_line > 0) {
                out |> push((line = header_line, body = join(lines, "\n")))
                lines |> clear()
                header_line = 0
            }
            if (pick |> invoke(raw)) {
                // a `def ... => expr` one-liner has no closing brace - end its window on the
                // header, or it swallows every line up to the next def
                if (find(raw, "=>") >= 0) {
                    out |> push((line = line_no, body = raw))
                } else {
                    header_line = line_no
                    lines |> push(raw)
                }
            }
        } elif (header_line > 0) {
            lines |> push(raw)
            if (raw |> starts_with("}")) {
                out |> push((line = header_line, body = join(lines, "\n")))
                lines |> clear()
                header_line = 0
            }
        }
    }
    if (header_line > 0) {
        out |> push((line = header_line, body = join(lines, "\n")))
    }
    return <- out
}

def private fnv1a(text : string; seed : uint64) : uint64 {
    var h = seed
    peek_data(text) $(d) {
        for (b in d) {
            h ^= uint64(b)
            h *= 0x100000001b3ul
        }
    }
    return h
}

// A field write into the archive, in every spelling the serializers use: the pipe form
// (`arch |> serialize`, which covers `serialize_raw`) and the call form a nested or bulk
// serializer takes (`serialize(arch, ...)`, `serialize_pod_array(arch, ...)`).
def private writes_meta_field(line : string) : bool {
    return true if (find(line, "arch |> serialize") >= 0)
    let at = find(line, "(arch,")
    return false if (at < 0)
    return ident_suffix(slice(line, 0, at)) |> starts_with("serialize")
}

[arch(at = "ARCHITECTURE_IMAGE.md#image-meta-tripwire")]
def private check_meta_tripwires {
    dir(DASLLAMA_DIR) $(name) {
        return if (!ends_with(name, ".das"))
        let path = "{DASLLAMA_DIR}/{name}"
        var inscope fns <- function_bodies(path) $(header) {
            return is_meta_serializer(header)
        }
        for (f in fns) {
            var calls = 0
            var guarded = false
            for (line in split(f.body, "\n")) {
                if (writes_meta_field(line)) {
                    calls++
                }
                if (find(line, "verify(count_meta_fields(") >= 0) {
                    guarded = true
                }
            }
            if (calls >= 2 && !guarded) {
                gate_finding(path, f.line,
                    "hand-listed Archive serializer with {calls} field writes and no verify(count_meta_fields(x) == K) - a field added to the struct and forgotten here reads back zero on every load; pin the count")
            }
        }
    }
}

[arch(at = "ARCHITECTURE_IMAGE.md#image-layout-stamp")]
def private check_image_layout_stamp {
    var version = -1
    for (line in split(read_lf(IMAGE_FILE), "\n")) {
        if (line |> starts_with("let IMAGE_VERSION = ")) {
            version = try_to_int(ident_prefix(slice(line, length("let IMAGE_VERSION = ")))) ?? -1
        }
    }
    if (version < 0) {
        gate_finding(IMAGE_FILE, "no `let IMAGE_VERSION = N` line - the layout stamp gate reads the version from it")
        return
    }
    var inscope files : array<string>
    dir(DASLLAMA_DIR) $(name) {
        if (ends_with(name, ".das")) {
            files |> push(name)
        }
    }
    files |> sort()
    var h = 0xcbf29ce484222325ul
    for (name in files) {
        let p = "{DASLLAMA_DIR}/{name}"
        var inscope consts <- layout_constant_lines(p)
        for (c in consts) {
            h = fnv1a("{name}\n{c.body}\n", h)
        }
        var inscope fns <- function_bodies(p) $(header) {
            return is_layout_function(name, header)
        }
        for (f in fns) {
            h = fnv1a("{name}\n{f.body}\n", h)
        }
    }
    return if (h == IMAGE_LAYOUT_STAMP_HASH && version == IMAGE_LAYOUT_STAMP_VERSION)
    if (version == IMAGE_LAYOUT_STAMP_VERSION) {
        gate_finding(IMAGE_FILE,
            "the image layout closure changed (every Archive serializer, build_image, parse_image, every *_prepare mint of dasllama_tts_blocks.das, the layout helpers pad_to_page/plane_end/image_total_bytes/w_append/w_zeros/w_header/store_u32/store_u64/kq_plane_name, and the layout constants IMAGE_PAGE/IMAGE_HEADER_BYTES/SECTION_TABLE_SLACK/DWRITE_STAGING_BAND/METAL_BAND_BLOCKS): re-stamp IMAGE_LAYOUT_STAMP_HASH = {h}ul in modules/dasLLAMA/REVIEW.das; bump IMAGE_VERSION (still {version}) and IMAGE_LAYOUT_STAMP_VERSION with it only if the bytes moved, because a stale image at an unchanged path stays structurally valid and serves different bytes")
    } else {
        gate_finding("modules/dasLLAMA/REVIEW.das",
            "the layout stamp is behind IMAGE_VERSION {version} - re-stamp: IMAGE_LAYOUT_STAMP_HASH = {h}ul, IMAGE_LAYOUT_STAMP_VERSION = {version}")
    }
}

// ===== the Vulkan checklist's mechanical checks (REVIEW_GPU_VULKAN.md) =====

let private VK_CLASSES = "modules/dasLLAMA/dasllama/dasllama_vulkan_classes.das"
let private VK_PREFILL = "modules/dasLLAMA/dasllama/dasllama_vulkan_prefill.das"
let private VK_COMMON = "modules/dasLLAMA/dasllama/dasllama_vulkan_common.das"
let private BLOCKS_FILE = "modules/dasLLAMA/dasllama/dasllama_blocks.das"

// a das integer literal's value at the head of `s` (digit separators and the l/u suffix dropped), -1 when none
def private int_literal(s : string) : int {
    let tok = replace(ident_prefix(strip(s)), "_", "")
    var n = length(tok)
    peek_data(tok) $(d) {
        while (n > 0 && (int(d[n - 1]) == 'l' || int(d[n - 1]) == 'u')) {
            n--
        }
    }
    return try_to_int(slice(tok, 0, n)) ?? -1
}

// the lines of the class whose header starts with `header`, in order (the body ends at the first
// line starting with `}`, the file's class convention); empty when the class is not in the file
def private class_lines(path, header : string) : array<string> {
    var out : array<string>
    var in_cls = false
    for (line in split(strip_line_comments(fread(path)), "\n")) {
        if (line |> starts_with(header)) {
            in_cls = true
            continue
        }
        if (in_cls && line |> starts_with("}")) {
            break
        }
        if (in_cls) {
            out |> push(line)
        }
    }
    return <- out
}

// The cm2 template's KHR stage stays abstract: a default body lets a format template that forgot
// its override compile, and that format decodes garbage on every KHR card while the cm2 arms stay green.
def private check_khr_stage16_abstract {
    var found = false
    for (line in class_lines(VK_CLASSES, "class template KqCm2BatchT")) {
        let s = strip(line)
        continue if (!(s |> starts_with("def ")) || find(s, "khr_stage16(") < 0)
        found = true
        if (!(s |> starts_with("def abstract khr_stage16"))) {
            gate_finding(VK_CLASSES, "khr_stage16 on KqCm2BatchT has a body - it stays abstract, so a format template that forgets its override fails to compile instead of decoding garbage on every KHR card")
        }
    }
    if (!found) {
        gate_finding(VK_CLASSES, "no `def khr_stage16` inside `class template KqCm2BatchT` - the KHR stage's abstract rule has nothing to read; keep the spelling or move this check with it")
    }
}

// the value the first line of `lines` carrying `marker` declares right after it, -1 when no line does
def private literal_after(lines : array<string>; marker : string) : int {
    for (line in lines) {
        let at = find(line, marker)
        if (at >= 0) {
            return int_literal(slice(line, at + length(marker)))
        }
    }
    return -1
}

// The add+rms row slab triple agrees: the constant, ArBase's @workgroup row slab, and the dense-width
// cap of the Vulkan servability gate. A cap raised without the slab writes past the slab's end on the device.
def private check_ar_max_dim_triple {
    var inscope common_lines <- split(strip_line_comments(fread(VK_COMMON)), "\n")
    let constant = literal_after(common_lines, "let AR_MAX_DIM = ")
    var inscope ar_lines <- class_lines(VK_CLASSES, "class ArBase")
    let slab = literal_after(ar_lines, "@workgroup row : float[")
    var gate = -1
    var inscope fns <- function_bodies(BLOCKS_FILE) $(header) {
        return header |> starts_with("def private attn_dec_shape_ok") || header |> starts_with("def attn_dec_shape_ok")
    }
    for (f in fns) {
        var inscope body_lines <- split(f.body, "\n")
        gate = literal_after(body_lines, "c.dim > ")
    }
    if (constant < 0) {
        gate_finding(VK_COMMON, "no `let AR_MAX_DIM = N` line - the add+rms slab triple has no constant to compare against")
    }
    if (slab < 0) {
        gate_finding(VK_CLASSES, "no `@workgroup row : float[N]` inside `class ArBase` - the add+rms slab triple has no slab to compare")
    }
    if (gate < 0) {
        gate_finding(BLOCKS_FILE, "no `c.dim > N` inside attn_dec_shape_ok - the add+rms slab triple has no servability cap to compare")
    }
    return if (constant < 0 || slab < 0 || gate < 0)
    if (constant != slab || constant != gate) {
        gate_finding(VK_CLASSES, "the add+rms row slab triple disagrees: AR_MAX_DIM {constant} (dasllama_vulkan_common.das), ArBase.row[{slab}], attn_dec_shape_ok's c.dim cap {gate} (dasllama_blocks.das) - a cap past the slab writes past its end on the device; change the three together")
    }
}

var private CM2_KHR_EXEMPT : table<string>   //! every format on the cm2 template owns a KHR arm today; a per-32 format the feed never admits in mm mode would join here
var private CM2_NO_EXEMPT : table<string>

def private cm2_format_templates(exempt : table<string>) : array<string> {
    var out : array<string>
    for (line in split(strip_line_comments(fread(VK_CLASSES)), "\n")) {
        continue if (!(line |> starts_with("class template ")))
        let name = ident_prefix(slice(line, length("class template ")))
        if (name |> ends_with("Cm2T") && find(line, ": KqCm2BatchT") >= 0 && !key_exists(exempt, name)) {
            out |> push(name)
        }
    }
    return <- out
}

//! one class ladder trio every format template on the cm2 base joins
struct private LadderTrio {
    suffix : string     //! the stamp's class suffix: `<Fmt>` + suffix
    tail : string       //! the dispatch name's tail: `kq_batch_<fmt>_` + tail; a per-32 format (q8, q51) spells `<fmt>_batch_` + tail
    ladder : string     //! the ladder functions' prefix: ladder + `ensure` / `set` / `enc`
    why : string        //! what a missing arm does at run time
}

def private cm2_dispatch_name(fmt, tail : string) : string {
    return fmt == "q8" || fmt == "q51" || fmt == "mx4" ? "{fmt}_batch_{tail}" : "kq_batch_{fmt}_{tail}"
}

def private check_cm2_ladder_set(trio : LadderTrio; exempt : table<string>) {
    var inscope templates <- cm2_format_templates(exempt)
    if (empty(templates)) {
        gate_finding(VK_CLASSES, "no `class template <Fmt>Cm2T : KqCm2BatchT` found - the {trio.ladder}* ladder rule has no templates to walk; keep the spelling or move this check with it")
        return
    }
    let classes = strip_line_comments(fread(VK_CLASSES))
    var inscope ladders <- function_bodies(VK_PREFILL) $(header) {
        return header |> starts_with("def {trio.ladder}ensure") || header |> starts_with("def {trio.ladder}set") || header |> starts_with("def {trio.ladder}enc")
    }
    if (length(ladders) != 3) {
        gate_finding(VK_PREFILL, "found {length(ladders)} of the three `{trio.ladder}ensure` / `{trio.ladder}set` / `{trio.ladder}enc` ladders - keep the spelling or move this check with it")
    }
    for (tpl in templates) {
        let pfx = slice(tpl, 0, length(tpl) - length("Cm2T"))
        let dispatch = cm2_dispatch_name(to_lower(pfx), trio.tail)
        if (find(classes, "class {pfx}{trio.suffix} : {tpl}") < 0) {
            gate_finding(VK_CLASSES, "{tpl} has no `class {pfx}{trio.suffix} : {tpl}` - a format on the cm2 template ships its {trio.suffix} stamp in the same change, or {trio.why}")
        }
        if (find(classes, "name = \"{dispatch}\"") < 0) {
            gate_finding(VK_CLASSES, "{tpl} has no `[vk_dispatch(name = \"{dispatch}\"` stamp - its {trio.suffix} class has no dispatch")
        }
        for (f in ladders) {
            let ladder = ident_prefix(slice(f.body, length("def ")))
            let arm = "{slice(ladder, length(trio.ladder))}_{dispatch}("
            if (find(f.body, arm) < 0) {
                gate_finding(VK_PREFILL, f.line, "{ladder} has no arm for {tpl} ({arm}) - {trio.why}")
            }
        }
    }
}

def private check_cm2_ladder_sets {
    check_cm2_ladder_set(LadderTrio(suffix = "KhrBatch", tail = "khr_cls", ladder = "khr_cls_",
        why = "the f16 feed admits the format on a KHR-mode card, where the KHR ladders panic; a per-32 format the feed never admits there joins CM2_KHR_EXEMPT in this file and the licensed set ARCHITECTURE_GPU_VULKAN.md names, in the same change"), CM2_KHR_EXEMPT)
    check_cm2_ladder_set(LadderTrio(suffix = "Cm2EBatch", tail = "cm2e_cls", ladder = "cm2e_cls_",
        why = "the expert schedule takes the format's planes, and the e ladders panic on the first resident MoE window"), CM2_NO_EXEMPT)
}

struct private VkStamp {
    dispatch_name : string
    cls : string
    base : string
    line : int
    body : string
}

//! every `[vk_dispatch]`-stamped class of `path`, in file order (a body ends at the first line starting with `}`)
def private vk_stamps(path : string) : array<VkStamp> {
    var out : array<VkStamp>
    var pending = ""
    var line_no = 0
    var cur = -1
    for (line in split(strip_line_comments(fread(path)), "\n")) {
        line_no++
        if (cur >= 0) {
            if (line |> starts_with("}")) {
                cur = -1
            } else {
                out[cur].body += "{line}\n"
            }
            continue
        }
        if (line |> starts_with("[vk_dispatch(name = \"")) {
            let from = length("[vk_dispatch(name = \"")
            pending = slice(line, from, find(line, "\"", from))
        } elif (!empty(pending) && line |> starts_with("class ")) {
            let colon = find(line, " : ")
            out |> push(VkStamp(dispatch_name = pending, line = line_no,
                cls = ident_prefix(slice(line, length("class "))),
                base = colon < 0 ? "" : ident_prefix(slice(line, colon + 3))))
            cur = length(out) - 1
            pending = ""
        } elif (!(line |> starts_with("["))) {
            pending = ""
        }
    }
    return <- out
}

// the identifier the first line of `lines` carrying `marker` declares right after it, "" when no line does
def private ident_after(lines : array<string>; marker : string) : string {
    for (line in lines) {
        let at = find(line, marker)
        if (at >= 0) {
            return ident_prefix(strip(slice(line, at + length(marker))))
        }
    }
    return ""
}

//! the template's constants a stamp inherits where it overrides none, and the two schedule widths
struct private StampDefaults {
    bn, bk, unr, blkw, s_rows, m_rows : int
}

//! one stamp against the tile rule: its unrolled block one superblock, a scale-cache format's BLKW that block, its four tile typedefs at its BK and BN, an s or e stamp's BN the schedule's width
def private check_cm2_stamp_tile(path : string; st : VkStamp; d : StampDefaults) {
    var inscope body <- split(st.body, "\n")
    let bn_over = literal_after(body, "override BN = ")
    let bk_over = literal_after(body, "override BK = ")
    let unr_over = literal_after(body, "override UNR = ")
    let bn = bn_over < 0 ? d.bn : bn_over
    let bk = bk_over < 0 ? d.bk : bk_over
    let unr = unr_over < 0 ? d.unr : unr_over
    if (bk * unr != d.bk * d.unr) {
        gate_finding(path, st.line, "{st.dispatch_name}: UNR {unr} at BK {bk} - a stamp's unrolled block is one superblock ({d.bk * d.unr} elements: the template's BK x UNR), so a 32-deep stamp carries `override UNR = {d.bk * d.unr / 32}u` and a 64-deep one none")
    }
    var inscope tlines <- class_lines(VK_CLASSES, "class template {st.base}")
    if (ident_after(tlines, "override SCACHE = ") == "true") {
        let blkw_over = literal_after(tlines, "override BLKW = ")
        let blkw = blkw_over < 0 ? d.blkw : blkw_over
        if (blkw != bk * unr) {
            gate_finding(path, st.line, "{st.dispatch_name}: a scale-cache stamp at BLKW {blkw} with an unrolled block of {bk * unr} - the cache refills once per superblock (`sc_step` keys on BLKW), so a SCACHE format's BLKW is its BK x UNR")
        }
    }
    let want <- fixed_array(
        (name = "AT", tile = "coopmatWgA_f16_128x{bk}", why = "a stamp's A tile is 128 rows by its k step"),
        (name = "BT", tile = "coopmatWgB_f16_{bk}x{bn}", why = "a stamp's B tile is its k step by its token column"),
        (name = "ACC", tile = "coopmatWgAcc_f16_128x{bn}", why = "a stamp's accumulator is 128 rows by its token column"),
        (name = "ACCW", tile = "coopmatWgAcc_f32_128x{bn}", why = "a stamp's f32 accumulator is 128 rows by its token column"))
    for (w in want) {
        let got = ident_after(body, "typedef {w.name} = ")
        if (empty(got)) {
            gate_finding(path, st.line, "{st.dispatch_name}: no `typedef {w.name}` at BK {bk} BN {bn} - {w.why} ({w.tile}); a stamp declares all four tile types")
        } elif (got != w.tile) {
            gate_finding(path, st.line, "{st.dispatch_name}: `typedef {w.name} = {got}` at BK {bk} BN {bn} - {w.why} ({w.tile}); change the constants and the types together")
        }
    }
    if (st.dispatch_name |> ends_with("_cm2e_cls") && bn != d.m_rows) {
        gate_finding(path, st.line, "{st.dispatch_name}: BN {bn} on an e stamp - the expert schedule's m pieces are SCHED_M_ROWS ({d.m_rows}) rows wide, the column the stamp serves")
    }
    if (st.dispatch_name |> ends_with("_cm2s_cls") && bn != d.s_rows) {
        gate_finding(path, st.line, "{st.dispatch_name}: BN {bn} on an s stamp - the expert schedule's s pieces are SCHED_S_ROWS ({d.s_rows}) rows wide, the column the stamp serves")
    }
}

def private check_cm2_stamp_tiles {
    var inscope templates <- cm2_format_templates(CM2_NO_EXEMPT)
    var inscope base_lines <- class_lines(VK_CLASSES, "class template KqCm2BatchT")
    let bn0 = literal_after(base_lines, "@template_constant BN : uint = ")
    let bk0 = literal_after(base_lines, "@template_constant BK : uint = ")
    let unr0 = literal_after(base_lines, "@template_constant UNR : uint = ")
    var inscope all_lines <- split(strip_line_comments(fread(VK_CLASSES)), "\n")
    let s_rows = literal_after(all_lines, "let SCHED_S_ROWS = ")
    let m_rows = literal_after(all_lines, "let SCHED_M_ROWS = ")
    if (bn0 < 0 || bk0 < 0 || unr0 < 0) {
        gate_finding(VK_CLASSES, "no `@template_constant BN : uint = N` / `BK : uint = N` / `UNR : uint = N` inside `class template KqCm2BatchT` - the stamp tile rule has no defaults to read; keep the spelling or move this check with it")
    }
    if (s_rows < 0 || m_rows < 0) {
        gate_finding(VK_CLASSES, "no `let SCHED_S_ROWS = N` / `let SCHED_M_ROWS = N` line - the stamp tile rule has no schedule widths to compare; keep the spelling or move this check with it")
    }
    return if (bn0 < 0 || bk0 < 0 || unr0 < 0 || s_rows < 0 || m_rows < 0)
    let blkw0 = literal_after(base_lines, "@template_constant BLKW : uint = ")
    let defaults = StampDefaults(bn = bn0, bk = bk0, unr = unr0, blkw = blkw0, s_rows = s_rows, m_rows = m_rows)
    var stamped = 0
    //! every file that stamps a `<Fmt>Cm2T` template: the engine's classes, the probe's other-depth twins, the toy fixture (a walk fixture holds only the first)
    for (path in fixed_array(VK_CLASSES, "modules/dasLLAMA/harness/vk_gemm_probe.das", "modules/dasLLAMA/tests/_vkd_toy.das")) {
        continue if (!fexist(path))
        var inscope stamps <- vk_stamps(path)
        for (st in stamps) {
            continue if (find_index(templates, st.base) < 0)
            stamped++
            check_cm2_stamp_tile(path, st, defaults)
        }
    }
    if (stamped == 0) {
        gate_finding(VK_CLASSES, "no `[vk_dispatch]` stamp of a `<Fmt>Cm2T` template found - the stamp tile rule has nothing to read; keep the spelling or move this check with it")
    }
}

let private VK_BOOST = "modules/dasVulkan/daslib/vulkan_boost.das"

def private vk_boost_probes : array<string> {
    var out : array<string>
    for (line in split(strip_line_comments(fread(VK_BOOST)), "\n")) {
        continue if (!(line |> starts_with("def public ")))
        let name = ident_prefix(slice(line, length("def public ")))
        if (name |> ends_with("_supported")) {
            out |> push(name)
        }
    }
    return <- out
}

// every whole-string `"VK_*"` literal of `text` - an extension read by name, never a name inside prose
def private vk_ext_literals(text : string) : array<string> {
    var out : array<string>
    peek_data(text) $(d) {
        var from = 0
        while (true) {
            let at = find(d, "\"VK_", from)
            break if (at < 0)
            var n = at + 1
            while (n < length(d) && is_ident_byte(int(d[n]))) {
                n++
            }
            if (n < length(d) && int(d[n]) == '"') {
                out |> push(slice(d, at + 1, n))
            }
            from = n
        }
    }
    return <- out
}

def private check_vk_extension_roster {
    if (!fexist(VK_BOOST)) {
        gate_finding(VK_COMMON, "no {VK_BOOST} - the extension roster rule has no probe list to read; keep the path or move this check with it")
        return
    }
    var inscope probes <- vk_boost_probes()
    var inscope rosters <- function_bodies(VK_COMMON) $(header) {
        return header |> starts_with("def public vk_ext_roster") || header |> starts_with("def vk_ext_roster")
    }
    if (length(rosters) != 1) {
        gate_finding(VK_COMMON, "no `def vk_ext_roster` in dasllama_vulkan_common.das - the extension roster rule has nothing to read; keep the spelling or move this check with it")
        return
    }
    let roster = rosters[0].body
    var files : array<string>
    collect_das_files("modules/dasLLAMA/dasllama", files)
    for (p in files) {
        var line_no = 0
        for (line in split(strip_line_comments(fread(p)), "\n")) {
            line_no++
            var inscope names <- vk_ext_literals(line)
            for (name in names) {
                if (find(roster, name) < 0) {
                    gate_finding(p, line_no, "the tier reads {name} here, and vk_ext_roster (dasllama_vulkan_common.das) does not name it - add the capability with what the tier does with it and what serves without it, so the device-init log says which route it decided")
                }
            }
            for (probe in probes) {
                if (find(line, "{probe}(") >= 0 && find(roster, "{probe}(") < 0) {
                    gate_finding(p, line_no, "the tier calls {probe} here, and vk_ext_roster (dasllama_vulkan_common.das) does not - add the capability it probes to the roster with what the tier does with it and what serves without it")
                }
            }
        }
    }
    delete files
}

// A Vulkan pipeline is built only by the `[vk_dispatch]`-generated `ensure_*`: a hand build bypasses the
// hazard rail and the dispatch census, so its races and its coverage are invisible. The two llama.cpp
// shader ports under performance/ are reference measurements of another engine's kernels, outside this walk.
def private check_no_hand_pipelines {
    var files : array<string>
    for (folder in ["modules/dasLLAMA/dasllama", "modules/dasLLAMA/harness", "modules/dasLLAMA/tests"]) {
        collect_das_files(folder, files)
    }
    for (p in files) {
        var line_no = 0
        for (line in split(strip_line_comments(fread(p)), "\n")) {
            line_no++
            if (find(line, "vkCreateComputePipelines(") >= 0) {
                gate_finding(p, line_no, "hand-built Vulkan pipeline - a pipeline is created only by a [vk_dispatch]-generated ensure_*, which puts it on the hazard rail and in the dispatch census")
            }
        }
    }
    delete files
}

// A `[test]` file that requires the engine lives in tests/ - unless it requires a file of its
// own folder by bare same-dir name (a bench self-check beside its subject), which tests/REVIEW.md's
// out-of-folder ledger owns instead.
def private walk_das(folder : string; var out : array<string>) {
    dir(folder) $(name) {
        return if (name == "." || name == ".." || name |> starts_with(".") || name |> starts_with("_"))
        let full = "{folder}/{name}"
        let st = stat(full)
        return if (!st.is_valid)
        if (st.is_dir) {
            return if (fexist("{full}/CMakeCache.txt"))
            walk_das(full, out)
        } elif (name |> ends_with(".das")) {
            out |> push(full)
        }
    }
}

def private check_test_file_placement {
    var inscope files : array<string>
    dir("modules/dasLLAMA") $(name) {
        return if (name == "." || name == ".." || name == "tests" || name |> starts_with(".") || name |> starts_with("_"))
        let full = "modules/dasLLAMA/{name}"
        let st = stat(full)
        return if (!st.is_valid)
        if (st.is_dir) {
            walk_das(full, files)
        } elif (name |> ends_with(".das")) {
            files |> push(full)
        }
    }
    files |> sort()
    for (path in files) {
        let text = strip_line_comments(read_lf(path))
        continue if (find(text, "[test]") < 0)
        var requires_engine = false
        var requires_sibling = false
        let folder = dir_name(path)
        for (line in split(text, "\n")) {
            continue if (!(line |> starts_with("require ")))
            var rest = slice(line, length("require "))
            if (rest |> starts_with("?")) {
                rest = slice(rest, 1)
            }
            let modname = ident_prefix(rest)
            if (rest |> starts_with("dasllama/")) {
                requires_engine = true
            } elif (!empty(modname) && find(rest, "/") < 0 && fexist("{folder}/{modname}.das")) {
                requires_sibling = true
            }
        }
        if (requires_engine && !requires_sibling) {
            gate_finding(path, "a [test] file requiring the engine sits outside modules/dasLLAMA/tests/ and requires no file of its own folder by bare name - move it into tests/ (tests/REVIEW.md's out-of-folder ledger owns the ones that must stay beside a same-folder subject)")
        }
    }
}

// The RANGES/WS tables are transcoded from the reference checkout's unicode data; a hand edit
// drifts from it silently. The stamp changes only through a regeneration.
let private UNICODE_FILE = "modules/dasLLAMA/dasllama/dasllama_unicode.das"
let private UNICODE_TABLES_HASH = 0x59f112fa8e7f0d9dul

def private check_unicode_tables_stamp {
    let text = read_lf(UNICODE_FILE)
    let from = find(text, "let private RANGES <- [")
    let ws = find(text, "let private WS <- [")
    if (from < 0 || ws < 0) {
        gate_finding(UNICODE_FILE, "the RANGES / WS table literals are not where the stamp gate reads them (`let private RANGES <- [` / `let private WS <- [`)")
        return
    }
    let ws_end = find(text, "\n]", ws)
    if (ws_end < 0) {
        gate_finding(UNICODE_FILE, "the WS table literal has no closing `]` line")
        return
    }
    let h = fnv1a(slice(text, from, ws_end + 2), 0xcbf29ce484222325ul)
    if (h != UNICODE_TABLES_HASH) {
        gate_finding(UNICODE_FILE, "the RANGES / WS tables changed - a hand edit is a defect: regenerate them by retranscoding the reference checkout's unicode data, then re-stamp UNICODE_TABLES_HASH = {h}ul in modules/dasLLAMA/REVIEW.das")
    }
}

// One device and queue per GPU family, from its _common file's init. The exceptions the rule
// names: dasllama_metal_gemm.das owns its device (ARCHITECTURE_GPU.md sec.1.5), and the two
// tuner race entries build theirs because the tuner runs before the driver inits.
let private DEVICE_CREATION_FILES <- {"dasllama_metal_common.das", "dasllama_vulkan_common.das", "dasllama_metal_gemm.das"}
let private DEVICE_CREATION_FUNCTIONS <- {"metal_tensor_race", "metal_tensor_race_decode"}
let private DEVICE_CREATION_CALLS <- ["metal_create_system_default_device(", "metal_new_command_queue("]

def private def_name_of(body : string) : string {
    let nl = find(body, "\n")
    var header = nl < 0 ? body : slice(body, 0, nl)
    header = slice(header, length("def "))
    if (header |> starts_with("private ")) {
        header = slice(header, length("private "))
    } elif (header |> starts_with("public ")) {
        header = slice(header, length("public "))
    }
    return ident_prefix(header)
}

def private check_device_creation_sites {
    dir(DASLLAMA_DIR) $(name) {
        return if (!ends_with(name, ".das") || DEVICE_CREATION_FILES |> key_exists(name))
        let path = "{DASLLAMA_DIR}/{name}"
        var inscope fns <- function_bodies(path) $(_header) {
            return true
        }
        for (f in fns) {
            var creates = false
            for (call in DEVICE_CREATION_CALLS) {
                creates = creates || find(f.body, call) >= 0
            }
            continue if (!creates)
            let fname = def_name_of(f.body)
            continue if (DEVICE_CREATION_FUNCTIONS |> key_exists(fname))
            gate_finding(path, f.line, "`{fname}` creates a GPU device or queue of its own - a GPU family shares the one device and queue its _common file's init made; a tuner race entry that must run before the driver inits is named in REVIEW.das's DEVICE_CREATION_FUNCTIONS with its reason")
        }
    }
}

// The PLE token-table gather has one format list in three places: the compiled pipelines, the
// PSO pick, and the encode ladder. A format in two of them declines or gathers under the wrong
// layout silently.
let private PREFILL_FILE = "modules/dasLLAMA/dasllama/dasllama_metal_prefill.das"

def private kqfmt_tokens(text : string; var out : table<string>) {
    return if (empty(text))
    peek_data(text) $(d) {
        var from = 0
        while (true) {
            let at = find(d, "KqFmt.", from)
            break if (at < 0)
            var end = at + length("KqFmt.")
            while (end < length(d) && is_ident_byte(int(d[end]))) {
                end++
            }
            if (end > at + length("KqFmt.")) {
                out |> insert(slice(d, at + length("KqFmt."), end))
            }
            from = end
        }
    }
}

def private set_diff_text(a, b : table<string>) : string {
    var missing <- [for (k in keys(a)); k; where !(b |> key_exists(k))]
    missing |> sort()
    return join(missing, ", ")
}

def private check_ple_gather_sites {
    let text = strip_line_comments(read_lf(PREFILL_FILE))
    var psos : table<string>
    for (line in split(text, "\n")) {
        continue if (!(line |> starts_with("var private g_pf_pso_ple_gather")))
        let rest = slice(line, length("var private g_pf_pso_ple_gather"))
        let suffix = ident_prefix(rest)
        psos |> insert(empty(suffix) ? "q8" : slice(suffix, 1))
    }
    var picks : table<string>
    var encodes : table<string>
    var inscope fns <- function_bodies(PREFILL_FILE) $(header) {
        return find(header, "ple_gather_pso_of(") >= 0 || find(header, "pf_enc_ple_gather_fmt(") >= 0
    }
    for (f in fns) {
        if (find(f.body, "ple_gather_pso_of(") >= 0) {
            kqfmt_tokens(f.body, picks)
        } else {
            kqfmt_tokens(f.body, encodes)
        }
    }
    if (empty(psos) || empty(picks) || empty(encodes)) {
        gate_finding(PREFILL_FILE, "the PLE gather's three sites are not where the gate reads them (g_pf_pso_ple_gather* globals, ple_gather_pso_of, pf_enc_ple_gather_fmt)")
        return
    }
    let a = set_diff_text(psos, picks)
    let b = set_diff_text(picks, psos)
    let c = set_diff_text(picks, encodes)
    let d = set_diff_text(encodes, picks)
    if (!empty(a) || !empty(b) || !empty(c) || !empty(d)) {
        gate_finding(PREFILL_FILE, "the PLE gather's format list differs across its three sites - pipelines not in the pick: [{a}]; pick not in the pipelines: [{b}]; pick not in the encode ladder: [{c}]; encode ladder not in the pick: [{d}] - a format lands in all three in one change")
    }
}

[export]
def main() : int {
    if (!fexist("modules/dasLLAMA/REVIEW.das") || !fexist(FACADE)) {
        to_log(LOG_ERROR, "modules/dasLLAMA/REVIEW.das: run from the repo root\n")
        return 2
    }
    check_meta_tripwires()
    check_image_layout_stamp()
    check_exe_fn_global_restore()
    check_race_bind_numbers()
    check_gpu_role_partition()
    check_fastmath_default()
    check_image_backing_release()
    check_khr_stage16_abstract()
    check_ar_max_dim_triple()
    check_cm2_ladder_sets()
    check_cm2_stamp_tiles()
    check_vk_extension_roster()
    check_no_hand_pipelines()
    check_test_file_placement()
    check_unicode_tables_stamp()
    check_device_creation_sites()
    check_ple_gather_sites()
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
