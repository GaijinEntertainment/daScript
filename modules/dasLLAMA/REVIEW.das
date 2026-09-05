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
            let pf = find(line, "pipeline_from_source(")
            if (pf >= 0) {
                var v = strip(line)
                v = slice(v, find(v, "var ") >= 0 ? find(v, "var ") + 4 : 0)
                let pvar = ident_prefix(v)
                let args = slice(line, pf + length("pipeline_from_source("))
                let c1 = find(args, ",")
                let src = strip(slice(args, c1 + 1, find(args, ",", c1 + 1)))
                let cls = rb_resolve_class(src, class_names, kernel_class)
                if (!empty(pvar) && !empty(cls)) {
                    pso_class[pvar] = cls
                }
                continue
            }
            let kp = find(line, "kn_pipeline(enc, ")
            if (kp >= 0) {
                target = pso_class?[ident_prefix(slice(line, kp + length("kn_pipeline(enc, ")))] ?? ""
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

// The stamp of the layout closure - every Archive serializer body, build_image, parse_image,
// every *_prepare mint, the layout helpers and the layout constants' declaration lines, hashed
// in file order. A closure change with IMAGE_VERSION unmoved is red; the finding prints the
// value to re-stamp with.
let private IMAGE_LAYOUT_STAMP_VERSION = 33
let private IMAGE_LAYOUT_STAMP_HASH = 0xec0727cff52428eeul

// The helpers that decide WHERE bytes land: the page pad, the plane and total sizing, the
// writer's append / zero-fill / header patch, and the header's scalar stores. Changing one
// moves every plane offset while every serializer body reads the same.
var private LAYOUT_HELPERS <- {
    "pad_to_page", "plane_end", "image_total_bytes",
    "w_append", "w_zeros", "w_header", "store_u32", "store_u64"
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

def private is_layout_function(header : string) : bool {
    return true if (is_archive_serializer(header))
    var rest = slice(header, 4)
    if (rest |> starts_with("private ")) {
        rest = slice(rest, length("private "))
    }
    let name = ident_prefix(rest)
    return true if (key_exists(LAYOUT_HELPERS, name))
    return name == "build_image" || name == "parse_image" || find(name, "_prepare") >= 0
}

// the declaration lines of the layout constants a file declares, in file order
def private layout_constant_lines(path : string) : array<tuple<line : int; body : string>> {
    var out : array<tuple<line : int; body : string>>
    var line_no = 0
    for (raw in split(strip_line_comments(fread(path)), "\n")) {
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
    for (raw in split(strip_line_comments(fread(path)), "\n")) {
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
    for (line in split(fread(IMAGE_FILE), "\n")) {
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
            return is_layout_function(header)
        }
        for (f in fns) {
            h = fnv1a("{name}\n{f.body}\n", h)
        }
    }
    return if (h == IMAGE_LAYOUT_STAMP_HASH && version == IMAGE_LAYOUT_STAMP_VERSION)
    if (version == IMAGE_LAYOUT_STAMP_VERSION) {
        gate_finding(IMAGE_FILE,
            "the image layout closure changed (every Archive serializer, build_image, parse_image, every *_prepare mint, the layout helpers pad_to_page/plane_end/image_total_bytes/w_append/w_zeros/w_header/store_u32/store_u64, and the layout constants IMAGE_PAGE/IMAGE_HEADER_BYTES/SECTION_TABLE_SLACK/DWRITE_STAGING_BAND/METAL_BAND_BLOCKS): re-stamp IMAGE_LAYOUT_STAMP_HASH = {h}ul in modules/dasLLAMA/REVIEW.das; bump IMAGE_VERSION (still {version}) and IMAGE_LAYOUT_STAMP_VERSION with it only if the bytes moved, because a stale image at an unchanged path stays structurally valid and serves different bytes")
    } else {
        gate_finding("modules/dasLLAMA/REVIEW.das",
            "the layout stamp is behind IMAGE_VERSION {version} - re-stamp: IMAGE_LAYOUT_STAMP_HASH = {h}ul, IMAGE_LAYOUT_STAMP_VERSION = {version}")
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
