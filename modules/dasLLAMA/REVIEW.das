options gen2

require math
require strings
require daslib/strings_boost
require daslib/strings_convert
require daslib/fio
require dastest/review_gate

// The mechanical half of modules/dasLLAMA/REVIEW.md (contract: REVIEW_COMMON.md at the repo
// root). Run from the repo root: bin/daslang modules/dasLLAMA/REVIEW.das — exit 0 clean,
// 1 with findings.

let private FACADE = "modules/dasLLAMA/dasllama/dasllama.das"
// dasllama.das defines nothing itself: it re-exports dasllama_core.das (the facade without the
// file decoders), which re-exports the TTS surface and, through the ASR surface, the GigaAM
// family and the shared transducer, so their defs are facade defs and the coverage floor reads
// every one of these files.
let private FACADE_FILES = [FACADE, "modules/dasLLAMA/dasllama/dasllama_core.das", "modules/dasLLAMA/dasllama/dasllama_tts.das",
                            "modules/dasLLAMA/dasllama/dasllama_gigaam.das", "modules/dasLLAMA/dasllama/dasllama_rnnt.das",
                            "modules/dasLLAMA/dasllama/dasllama_translate.das"]
let private TUTORIAL_DIR = "tutorials/dasLLAMA"
let private RST_DIR = "doc/source/reference/tutorials"

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

var private FACADE_DEFS_WITHOUT_TUTORIAL <- {
    "finalize", "tts_finish_styletts2",   //! the runtime or a loader chain calls them, never a user - `ARCHITECTURE_TTS.md#text-to-speech` lists them
    "gpu_resident_mtp_device_home", "gpu_device_prefill_segments",   //! the scheduler's device-mode and segmented-window decisions; a user's program asks the scheduler
    "gpu_device_kv_claim", "gpu_device_kv_victim_claims", "gpu_device_kv_parked_pos",   //! the scheduler's region bookkeeping behind park and adopt
    "gpu_device_kv_checkpoint", "gpu_device_kv_checkpoints",   //! the scheduler's recurrent checkpoints on a parked region
    "gpu_device_kv_export", "gpu_device_kv_import",   //! the scheduler's moves between a region and its paged prefix pool
    "gpu_device_kv_snapshot", "gpu_device_kv_parked_snapshot", "prefix_take_spare",   //! the scheduler's recurrent-state copies off a region into its prefix cache
    "prefix_state_match_len", "prefix_attach_state_split", "prefix_state_touch",   //! the scheduler's lookups of a recurrent checkpoint in its prefix cache
    "prefix_share_pages", "prefix_insert_snapshot",   //! the scheduler's writes of shared pages and device-taken checkpoints into its prefix cache
    "gpu_device_kv_pool_armed",   //! the scheduler reads DASLLAMA_DEVICE_POOL through it; a user sets the variable
    "gpu_device_kv_pool_moves",   //! the region-pool traffic counter a fixture or a bench reads
    "gpu_free_regions", "gpu_resident_prefills",   //! the server's region check before a chat-route embedding and its embedder's device-prefill report
    "embed_session_dtype", "embed_window_rows",   //! the codec and window embed / embed_batch pass the embedding sessions; a user program calls embed / embed_batch
    "stage_gigaam_model", "mint_gigaam_model", "load_gigaam_model",   //! the ASR loader chain and dasllama-convert stage, mint and load GigaAM; a user program calls load_asr_model
    "gigaam_check_arch", "gigaam_check_hparams", "gigaam_image_tag",   //! the GigaAM loader chain's file checks and its prepared-image tag
    "gigaam_transcribe", "gigaam_asr_caps", "gigaam_check_lang", "gigaam_exec_fmt",   //! the ASR surface's GigaAM arm; a user program calls transcribe / caps / create_session
    "gigaam_device_gpu", "gigaam_gpu_serves", "gigaam_gpu_exact_serves", "gigaam_serves_q8",   //! the ASR surface's lane report for a GigaAM model; a user program reads it through the surface
    "set_gigaam_q8", "reset_gigaam_q8",   //! the family's weight-lane pin a fixture sets; a user program pins the ASR lane with set_asr_fp32
    "gigaam_log_mel", "gigaam_conv_out", "gigaam_encode_front", "gigaam_rope_tables",   //! the encoder stages the transcribe arm and the device towers drive
    "gigaam_encode_block", "gigaam_encode", "gigaam_mel_frames", "gigaam_encode_span",   //! the encoder stages the transcribe arm and the device towers drive
    "gigaam_layer_offsets", "gigaam_decode", "gigaam_detokenize", "gigaam_chunks", "gigaam_spans",   //! the encoder's plane offsets and the decode stages the transcribe arm drives
    "register_gigaam_gpu", "register_gigaam_gpu_encode", "register_gigaam_gpu_ctc", "register_rnnt_gpu",   //! the Metal and Vulkan towers and decoders register their device seats through them
    "rnnt_greedy", "rnnt_decode", "rnnt_n_logits", "rnnt_joint_blocks", "rnnt_step_lanes",   //! the shared transducer decode the parakeet and gigaam families drive
    "rnnt_dev_args", "rnnt_dev_cap", "rnnt_dev_faulted", "rnnt_dev_first_input", "rnnt_dev_key",   //! the host half of the device transducer decode the Metal and Vulkan seats call
    "rnnt_dev_land_tokens", "rnnt_dev_layer_args", "rnnt_dev_layout", "rnnt_dev_round_budget",   //! the host half of the device transducer decode the Metal and Vulkan seats call
    "rnnt_dev_rounds", "rnnt_dev_shape_ok", "rnnt_dev_slab", "rnnt_dev_state_words",   //! the host half of the device transducer decode the Metal and Vulkan seats call
    "set_rnnt_dev_fault", "set_rnnt_dev_round_budget",    //! probe seams a fixture sets to force a device fault, a round budget or a lane split
    "set_rnnt_trace",   //! the CPU transducer decode's per-frame trace an engine developer flips while bringing a family up
    "gigaam_gemm_stride", "gigaam_lane_tag", "gigaam_kq_members", "gigaam_lane_pinned",
    "gigaam_fc_record_seats", "gigaam_kq_members_of"   //! the kq lane's seat geometry a test pins, the lane's image tag the bake tool names, the lane's members the load log and the tests read, the pin the carrier's lane policy reads
}

[arch(at = "ARCHITECTURE_TTS.md#text-to-speech")]
def private check_floor_of(path : string; var seen : table<string>; tut_texts, rst_texts : array<string>) {
    var line_no = 0
    for (line in split(fread(path), "\n")) {
        line_no++
        continue if (!starts_with(line, "def ") || starts_with(line, "def private ") || starts_with(line, "def operator"))
        let name = ident_prefix(slice(line, 4))
        continue if (empty(name) || key_exists(FACADE_DEFS_WITHOUT_TUTORIAL, name) || key_exists(seen, name))
        seen |> insert(name)
        if (!word_in_any(tut_texts, name)) {
            gate_finding(path, line_no,
                "facade def {name} appears in no {TUTORIAL_DIR}/*.das - the facade is the public API, and every facade def is demonstrated in a tutorial, or is a def no user program calls (the runtime, a loader chain, the scheduler, a server or a bench does) that REVIEW.das's FACADE_DEFS_WITHOUT_TUTORIAL lists with its reason")
        }
        if (!word_in_any(rst_texts, name)) {
            gate_finding(path, line_no,
                "facade def {name} appears in no {RST_DIR}/dasLLAMA_*.rst - the tutorial pages narrate every facade def (they are hand-written, not includes), or it is a def no user program calls that REVIEW.das's FACADE_DEFS_WITHOUT_TUTORIAL lists with its reason")
        }
    }
}

// The tutorial-coverage floor: every def the facade files themselves define appears in at
// least one tutorial source and one tutorial page. REVIEW_FACADE.md keeps the judgment half
// (covered means TAUGHT); this gate only proves presence.
def private check_tutorial_floor(tut_texts, rst_texts : array<string>) {
    var seen : table<string>
    for (path in FACADE_FILES) {
        check_floor_of(path, seen, tut_texts, rst_texts)
    }
}

let private AUDIO_CARRIER = "modules/dasLLAMA/dasllama/dasllama_audio_embedder.das"
let private VISION_CARRIER = "modules/dasLLAMA/dasllama/dasllama_vision_embedder.das"
let private ASR_CARRIER = "modules/dasLLAMA/dasllama/dasllama_asr.das"

// The licensed family namers per modality - the ARCHITECTURE_MEDIA.md carrier charters (§1.7,
// §1.7b) ledger each with its reason.
var private AUDIO_SEAM_EXEMPT <- {
    "modules/dasLLAMA/dasllama/dasllama_asr.das",
    "modules/dasLLAMA/dasllama/dasllama_metal_tower.das",
    "modules/dasLLAMA/dasllama/dasllama_metal_asr_dec.das",
    "modules/dasLLAMA/dasllama/dasllama_vulkan_tower.das",
    "modules/dasLLAMA/dasllama/dasllama_vulkan_asr_dec.das",
    "modules/dasLLAMA/benchmarks/asr/asr_bench.das",
    "utils/dasllama-convert/main.das"
}

var private ASR_SEAM_EXEMPT <- {
    "modules/dasLLAMA/dasllama/dasllama_audio_embedder.das",
    "modules/dasLLAMA/dasllama/dasllama_metal_tower.das",
    "modules/dasLLAMA/dasllama/dasllama_metal_asr_dec.das",
    "modules/dasLLAMA/dasllama/dasllama_vulkan_tower.das",
    "modules/dasLLAMA/dasllama/dasllama_vulkan_asr_dec.das",
    "modules/dasLLAMA/benchmarks/asr/asr_bench.das",
    "modules/dasLLAMA/harness/pk_parity_probe.das",
    "modules/dasLLAMA/harness/cpu_asr_race.das",
    "utils/dasllama-convert/main.das"
}

//! a carrier kind served by another kind's family file - the two kinds share one family by design
var private ASR_KIND_FILES <- { "dasllama_qwen3omni" => "dasllama_qwen3a" }

var private VISION_SEAM_EXEMPT <- {
    "modules/dasLLAMA/dasllama/dasllama_metal_tower.das",
    "modules/dasLLAMA/dasllama/dasllama_vulkan_tower.das",
    "modules/dasLLAMA/harness/vision_lane_probe.das"   //! the lane probe pins a family's lane and mints its tower in memory - neither reaches the carrier
}

def private carried_families(carrier, kind_enum : string) : array<string> {
    var fams : array<string>
    var in_kind = false
    for (line in split(fread(carrier), "\n")) {
        if (starts_with(line, "enum {kind_enum}") || starts_with(line, "enum private {kind_enum}")) {
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

//! every .das under `folder`; `prune_private` also skips `.`/`_`-prefixed names and build trees
def private collect_das_files(folder : string; var out : array<string>; prune_private : bool = false) {
    dir(folder) $(fname) {
        return if (fname == "." || fname == ".." || (prune_private && (fname |> starts_with(".") || fname |> starts_with("_"))))
        let p = "{folder}/{fname}"
        let st = stat(p)
        if (st.is_valid && st.is_dir) {
            return if (prune_private && fexist("{p}/CMakeCache.txt"))
            collect_das_files(p, out, prune_private)
        } elif (ends_with(fname, ".das")) {
            out |> push(p)
        }
    }
}

def private check_family_seams(carrier, kind_enum : string; exempt : table<string>; route_hint : string) {
    var inscope kind_files : table<string; string>
    check_family_seams(carrier, kind_enum, exempt, kind_files, "ARCHITECTURE_MEDIA.md's carrier charter", route_hint)
}

def private check_family_seams(carrier, kind_enum : string; exempt : table<string>; kind_files : table<string; string>;
                               charter, route_hint : string) {
    var licensed <- [for (k in keys(exempt)); k]
    licensed |> sort()
    let licensed_text = join(licensed, ", ")
    var inscope fams <- carried_families(carrier, kind_enum)
    for (fam in fams) {
        let owner = kind_files?[fam] ?? fam
        if (owner != fam) {
            if (!has_value(fams, owner)) {
                gate_finding(carrier, "{kind_enum} arm {fam} is declared to share {owner}'s family file, but {owner} is no {kind_enum} arm - a shared file is another kind's own ({charter})")
            }
            continue
        }
        let fam_path = "modules/dasLLAMA/dasllama/{fam}.das"
        if (!fexist(fam_path)) {
            gate_finding(carrier, "{kind_enum} arm {fam} has no {fam_path} - the carrier names a family the module does not carry, and its seams would go unpoliced ({charter})")
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
                    gate_finding(p, "names {n}, which the {fam} family owns - outside the family's own file a family type is named only in the carrier, in the licensed namers {charter} ledgers ({licensed_text}) and under tests/; route through the carrier ({route_hint})")
                }
            }
        }
    }
}

// The backend-role partition, mechanically: every dasllama_<gpu>_*.das file's suffix is a role
// ARCHITECTURE_GPU.md#gpu-backends's role table carries — a new role file adds its role to BOTH.
var private GPU_ROLES_METAL <- {
    "kernels", "common", "decode", "prefill", "gemm", "shapes", "tower", "asr_dec", "nmt", "lens", "mtp_gemma"
}

var private GPU_ROLES_VULKAN <- {
    "tts",
    "classes", "common", "decode", "prefill", "seams", "dispatch", "tower", "asr_dec", "nmt", "mtp_gemma"
}

var private GPU_ROLES_CUDA <- {
    "classes", "common", "dispatch", "decode", "stream", "mtp_gemma"
}

// the fastmath default, mechanically: a [metal_kernel] declaring fastmath=false reds unless
// its kernel name is listed here against the test that reds under fast math (the failing test
// ships with the kernel per the checklist; adding a row is a gate edit the master's weakening
// rule reviews). The StyleTTS2 harmonic-source kernels earned theirs: under fast math
// `st2_source_gate` in tests/test_metal_prefill_kernels.das reds on both resample laws (the
// divisions approximate, the two-float cumsum's compensation folds away).
var private FASTMATH_OFF_EXEMPT <- {
    "metal_st2_src_low_torch_msl" => "st2_source_gate",
    "metal_st2_src_low_onnx_msl" => "st2_source_gate",
    "metal_st2_src_cumsum_torch_msl" => "st2_source_gate",
    "metal_st2_src_cumsum_onnx_msl" => "st2_source_gate",
    "metal_st2_src_sines_torch_msl" => "st2_source_gate",
    "metal_st2_src_sines_onnx_msl" => "st2_source_gate",
    //! the per-32 GEMV and its small-batch form (an unnamed kernel keys by its method): under fast math the two sum a
    //! column's dot apart, and `b32_mvb_gate` in tests/test_metal_prefill_kernels.das reds its bit-for-bit compare
    "metal_kq_gemv_q51_msl" => "b32_mvb_gate",
    "metal_kq_gemv_iq4nl32_msl" => "b32_mvb_gate",
    "metal_kq_gemv_q51_ks_msl" => "b32_mvb_gate",
    "metal_kq_gemv_iq4nl32_ks_msl" => "b32_mvb_gate",
    "metal_kq_gemv_q4032_msl" => "b32_mvb_gate",
    "metal_kq_gemv_q4032_ks_msl" => "b32_mvb_gate",
    "metal_kq_gemv_q5032_msl" => "b32_mvb_gate",
    "metal_kq_gemv_q5032_ks_msl" => "b32_mvb_gate",
    "metal_b32_mvb" => "b32_mvb_gate",
    //! the classifier tail's selection: under fast math a NaN lane passes the ordered compare that floors it, and
    //! `cls_select_gate` in tests/test_metal_misc_kernels.das reds the recent window's landed NaN against the floor
    "metal_cls_select_msl" => "cls_select_gate"
}

//! The `name="..."` a [metal_kernel] line declares, or "" where the line carries none.
def private metal_kernel_name(line : string) : string {
    let at = find(line, "name")
    return "" if (at < 0)
    let q0 = find(line, "\"", at)
    return "" if (q0 < 0)
    let q1 = find(line, "\"", q0 + 1)
    return "" if (q1 < 0)
    return slice(line, q0 + 1, q1)
}

[arch(at = "ARCHITECTURE_ENGINE.md#engine-core")]
def private check_fastmath_default {
    var files : array<string>
    collect_das_files("modules/dasLLAMA/dasllama", files)
    for (p in files) {
        var line_no = 0
        var inscope lines <- split(fread(p), "\n")
        for (line in lines) {
            line_no++
            continue if ((find(line, "fastmath=false") < 0 && find(line, "fastmath = false") < 0) || (strip(line) |> starts_with("//")))
            var key = metal_kernel_name(line)
            if (empty(key) && line_no < length(lines)) {   //! an unnamed kernel: the method the next line declares
                let next = strip(lines[line_no])
                key = next |> starts_with("def ") ? strip(slice(next, 4, find(next, " ", 4) < 0 ? length(next) : find(next, " ", 4))) : ""
            }
            if (!key_exists(FASTMATH_OFF_EXEMPT, key)) {
                gate_finding(p, line_no,
                    ("kernel declares fastmath=false without an exemption row naming the test that"
                    + " fails under fastmath - fastmath is the default; the opt-out ships its"
                    + " demonstrated breakage, and its row ships its line in"
                    + " ARCHITECTURE_ENGINE.md#engine-core"))
            }
        }
    }
    delete files
}

//! every code line of the `.das` files under `folders` (comments stripped) carrying one of `tokens`, each a finding with
//! `msg` - but a line `allowed` admits, and a file `exempt` names
def private gate_banned_tokens(folders : array<string>; exempt : string; tokens : array<string>; msg : string; allowed : block<(line : string) : bool>) {
    var files : array<string>
    for (folder in folders) {
        collect_das_files(folder, files)
    }
    gate_banned_tokens_in(files, exempt, tokens, msg, allowed)
    delete files
}

//! `gate_banned_tokens` over a named file list - the folder walk's body, and a fixture rail's
def private gate_banned_tokens_in(files : array<string>; exempt : string; tokens : array<string>; msg : string; allowed : block<(line : string) : bool>) {
    for (p in files) {
        continue if (base_name(p) == exempt)
        var line_no = 0
        for (line in split(strip_line_comments(fread(p)), "\n")) {
            line_no++
            for (tok in tokens) {
                if (find(line, tok) >= 0 && !invoke(allowed, line)) {
                    gate_finding(p, line_no, msg)
                    break
                }
            }
        }
    }
}

//! the K/V mirror's per-slab arrays - the slab pairs, the window store and shadow sets - are indexed by a layer's seat
//! (`mir_slab`) at every dispatch, sync, readback and hydrate; the bare index lives in dasllama_vulkan_common.das's
//! accessors alone (ARCHITECTURE_GPU_VULKAN_RESIDENCY.md#resident-plan)
def private check_mirror_slab_access {
    gate_banned_tokens(["modules/dasLLAMA/dasllama"], "dasllama_vulkan_common.das",
        ["k_slabs[", "v_slabs[", "pf_kv_store_sets[", "pf_kv_shadow_sets["],
        "indexes a K/V mirror slab array by something other than a layer's seat - take the layer's slab through rd_slab(L), or index the per-slab set by L.mir_slab (ARCHITECTURE_GPU_VULKAN_RESIDENCY.md#resident-plan)") $(line) {
        return find(line, ".mir_slab]") >= 0
    }
}

//! one top-level function of a scanned file: its attribute text, parameter names and line span (0-based, inclusive)
struct private PcFn {
    attrs : string
    params : array<string>
    first : int
    last : int
}

//! the parameter names a `def` line declares
def private def_params(sig : string) : array<string> {
    var names : array<string>
    let open = find(sig, "(")
    return <- names if (open < 0)
    let close = find(sig, ")", open)
    let inner = slice(sig, open + 1, close >= 0 ? close : length(sig))
    for (piece in split(inner, ";")) {
        let colon = find(piece, ":")
        continue if (colon < 0)
        for (n in split(slice(piece, 0, colon), ",")) {
            let w = ident_suffix(strip(n))
            names |> push(w) if (!empty(w))
        }
    }
    return <- names
}

//! the top-level functions of `lines`: a function runs from its `def` to the line before the next column-0 line
def private pc_functions(lines : array<string>) : array<PcFn> {
    var fns : array<PcFn>
    var attrs : array<string>
    for (line, i in lines, count()) {
        continue if (empty(line) || is_white_space(first_character(line)) || starts_with(line, "}"))
        if (!empty(fns) && fns[length(fns) - 1].last < 0) {
            fns[length(fns) - 1].last = i - 1
        }
        if (starts_with(line, "[")) {
            attrs |> push(line)
        } elif (starts_with(line, "def ")) {
            fns |> emplace(PcFn(attrs = join(attrs, " "), params <- def_params(line), first = i, last = -1))
            attrs |> clear()
        } else {
            attrs |> clear()
        }
    }
    if (!empty(fns) && fns[length(fns) - 1].last < 0) {
        fns[length(fns) - 1].last = length(lines) - 1
    }
    return <- fns
}

//! the text of a call's first argument, from just past its open paren to the first top-level comma
def private first_arg(s : string; open : int) : string {
    var depth = 0
    var n = open + 1
    peek_data(s) $(d) {
        while (n < length(d)) {
            let c = int(d[n])
            if (c == '(' || c == '[') {
                depth++
            } elif (c == ')' || c == ']') {
                break if (depth == 0)
                depth--
            } elif (c == ',' && depth == 0) {
                break
            }
            n++
        }
    }
    return strip(slice(s, open + 1, n))
}

//! a buffer path with its trailing index groups cut (`s.devs[i]` -> `s.devs`)
def private cut_index(path : string) : string {
    var n = 0
    peek_data(path) $(d) {
        n = length(d)
        while (n > 0 && int(d[n - 1]) == ']') {
            var depth = 0
            var i = n - 1
            while (i >= 0) {
                let c = int(d[i])
                if (c == ']') {
                    depth++
                } elif (c == '[') {
                    depth--
                    break if (depth == 0)
                }
                i--
            }
            break if (i < 0)
            n = i
            while (n > 0 && is_white_space(int(d[n - 1]))) {
                n--
            }
        }
    }
    return slice(path, 0, n)
}

let private PC_ALLOCATORS = ["_device_buf(", "_dev_buf(", "_call_buf(", "dev_from(", "dev_upload("]

def private has_allocator(s : string) : bool {
    for (a in PC_ALLOCATORS) {
        return true if (find(s, a) >= 0)
    }
    return false
}

//! The right-hand sides that set buffer field `f` in `line`: a field assignment (`.f = `, `.f[i] = `), a named
//! initializer (`(f = `, `, f = `), or a push into it (`.f |> push(`).
def private field_rhs(line, f : string) : array<string> {
    var out : array<string>
    peek_data(line) $(d) {
        for (lead in [".", "(", ", "]) {
            let key = "{lead}{f}"
            var at = find(line, key)
            while (at >= 0) {
                var p = at + length(key)
                if (p < length(d) && !is_ident_byte(int(d[p]))) {
                    if (int(d[p]) == '[') {
                        p = find(line, "]", p) + 1
                    }
                    if (p > 0 && find(line, " = ", p) == p) {
                        out |> push(first_arg(line, p + 2))
                    } elif (p > 0 && lead == "." && find(line, " |> push(", p) == p) {
                        out |> push(first_arg(line, p + 8))
                    }
                }
                at = find(line, key, at + 1)
            }
        }
    }
    return <- out
}

//! Every allocation that can land in buffer field `f`, followed through field aliases (`dev = s.devs[slot]`,
//! `k_slabs |> push(mir.a)`): file:line plus whether it carries the per-call property.
def private pc_field_allocs(f : string; files : array<string>; texts : array<array<string>>; var seen : table<string>;
                            var out : array<tuple<at : string; call : bool>>) {
    return if (key_exists(seen, f))
    seen |> insert(f)
    for (path, lines in files, texts) {
        for (line, li in lines, count()) {
            continue if (find(line, f) < 0)
            for (rhs in field_rhs(line, f)) {
                if (has_allocator(rhs)) {
                    out |> push((at = "{path}:{li + 1}", call = find(rhs, "_call_buf(") >= 0))
                } else {
                    let alias = path_prefix(strip(rhs))
                    if (find(alias, ".") >= 0) {
                        pc_field_allocs(ident_suffix(cut_index(alias)), files, texts, seen, out)
                    }
                }
            }
        }
    }
}

//! ARCHITECTURE_GPU_VULKAN_ROOM.md#vk-per-call-buffers: a host upload (`upload_region_at`, `upload_region`) in a
//! function that is neither `[cold_path]` (a load-time fill) nor the one that allocated the buffer is a per-call
//! fill, and every allocation that can land in its target buffer carries the per-call property (`*_call_buf`)
def private check_per_call_buffers {
    var inscope files : array<string>
    collect_das_files("modules/dasLLAMA/dasllama", files)
    var inscope vk : array<string>
    for (p in files) {
        vk |> push(p) if (starts_with(base_name(p), "dasllama_vulkan_"))
    }
    var inscope texts : array<array<string>>
    texts |> reserve(length(vk))
    for (p in vk) {
        texts |> emplace(split(strip_line_comments(fread(p)), "\n"))
    }
    for (fi in range(length(vk))) {
        var inscope fns <- pc_functions(texts[fi])
        for (fn in fns) {
            continue if (find(fn.attrs, "cold_path") >= 0)
            for (li in range(fn.first, fn.last + 1)) {
                let line = texts[fi][li]
                for (call in ["upload_region_at(", "upload_region("]) {
                    let at = upload_call_at(line, call)
                    if (at >= 0) {
                        pc_check_site(vk, texts, fi, li, fn, first_arg(line, at + length(call) - 1))
                    }
                }
            }
        }
    }
}

//! where `line` calls `call` (a whole identifier, not the `def` that declares it); -1 where it does not
def private upload_call_at(line, call : string) : int {
    let at = find(line, call)
    return -1 if (at < 0 || find(line, "def {call}") == 0 || find(line, "def private {call}") == 0)
    var bare = true
    if (at > 0) {
        peek_data(line) $(d) {
            bare = !is_ident_byte(int(d[at - 1]))
        }
    }
    return bare ? at : -1
}

def private pc_local_rhs(lines : array<string>; fn : PcFn; name : string) : string {
    for (li in range(fn.first, fn.last + 1)) {
        let line = strip(lines[li])
        for (decl in ["let {name} = ", "var {name} = ", "let {name} & = ", "var {name} & = "]) {
            return slice(line, length(decl)) if (starts_with(line, decl))
        }
    }
    return ""
}

def private pc_check_site(vk : array<string>; texts : array<array<string>>; fi, li : int; fn : PcFn; target : string) {
    var path = cut_index(target)
    if (find(path, ".") < 0) {
        return if (find_index(fn.params, path) >= 0)
        let rhs = pc_local_rhs(texts[fi], fn, path)
        return if (has_allocator(rhs))
        path = cut_index(path_prefix(rhs))
        if (find(path, ".") < 0) {
            gate_finding(vk[fi], li + 1, "a host upload into `{target}`, a buffer this gate cannot follow to its allocation - name it through the field or local it was allocated into (ARCHITECTURE_GPU_VULKAN_ROOM.md#vk-per-call-buffers)")
            return
        }
    }
    let f = ident_suffix(path)
    for (bl in range(fn.first, fn.last + 1)) {
        for (rhs in field_rhs(texts[fi][bl], f)) {
            return if (has_allocator(rhs))
        }
    }
    var inscope seen : table<string>
    var inscope allocs : array<tuple<at : string; call : bool>>
    pc_field_allocs(f, vk, texts, seen, allocs)
    if (empty(allocs)) {
        gate_finding(vk[fi], li + 1, "a per-call upload into `{target}`, whose field `{f}` this gate finds no allocation for - allocate it into a field through a `*_call_buf` allocator (ARCHITECTURE_GPU_VULKAN_ROOM.md#vk-per-call-buffers)")
        return
    }
    for (a in allocs) {
        if (!a.call) {
            gate_finding(vk[fi], li + 1, "a per-call upload into `{target}`, a buffer allocated at {a.at} without the per-call property - allocate every buffer the host refills after a device read through make_call_buf / try_call_buf / soft_call_buf or the driver's *_call_buf, or mark this function [cold_path] when it is a load-time fill (ARCHITECTURE_GPU_VULKAN_ROOM.md#vk-per-call-buffers)")
        }
    }
}

//! the top-level ` + ` summands of an index expression (parentheses and brackets keep their sums whole)
def private index_summands(idx : string) : array<string> {
    var out : array<string>
    peek_data(idx) $(d) {
        var depth = 0
        var from = 0
        for (i in range(length(d))) {
            let ch = int(d[i])
            if (ch == '(' || ch == '[') {
                depth++
            } elif (ch == ')' || ch == ']') {
                depth--
            } elif (ch == '+' && depth == 0 && i > 0 && int(d[i - 1]) == ' ' && i + 1 < length(d) && int(d[i + 1]) == ' ') {
                out |> push(strip(slice(d, from, i)))
                from = i + 1
            }
        }
        out |> push(strip(slice(d, from, length(d))))
    }
    return <- out
}

//! The index texts `line` READS `buf` at: every `buf[...]` that is not the target of an assignment.
def private indexed_reads(line, buf : string) : array<string> {
    var out : array<string>
    peek_data(line) $(d) {
        var at = find(d, "{buf}[")
        while (at >= 0) {
            let open = at + length(buf)
            var close = open + 1
            var depth = 1
            while (close < length(d) && depth > 0) {
                depth += int(d[close]) == '[' ? 1 : int(d[close]) == ']' ? -1 : 0
                close++
            }
            var after = close
            while (after < length(d) && int(d[after]) == ' ') {
                after++
            }
            let eq = after < length(d) && int(d[after]) == '=' && !(after + 1 < length(d) && int(d[after + 1]) == '=')
            let op_eq = after + 1 < length(d) && int(d[after + 1]) == '=' && find("+-*/|&^", slice(d, after, after + 1)) >= 0
            let whole = at == 0 || !is_ident_byte(int(d[at - 1]))
            if (whole && depth == 0 && !eq && !op_eq) {
                out |> push(slice(d, open + 1, close - 1))
            }
            at = find(d, "{buf}[", close)
        }
    }
    return <- out
}

//! How `term` reads once its loops are unrolled: 1 = a constant built from a loop variable (its every name a loop
//! variable or an integer cast), 0 = a constant with no name at all, -1 = a run-time value.
def private unrolled_kind(term : string; loop_vars : array<string>) : int {
    var kind = 0
    peek_data(term) $(d) {
        var i = 0
        while (i < length(d) && kind >= 0) {
            if (!is_alpha(int(d[i])) && int(d[i]) != '_') {
                i++
                continue
            }
            let from = i
            while (i < length(d) && is_ident_byte(int(d[i]))) {
                i++
            }
            let lit_suffix = from > 0 && is_number(int(d[from - 1]))
            let name = slice(d, from, i)
            if (lit_suffix || name == "uint" || name == "int") {
                continue
            }
            kind = find_index(loop_vars, name) >= 0 ? 1 : -1
        }
    }
    return kind
}

//! true when `idx` sums a constant of a fully unrolled loop's variable with a run-time base
def private sums_loop_var_with_base(idx : string; loop_vars : array<string>) : bool {
    var inscope terms <- index_summands(idx)
    var looped = false
    var based = false
    for (term in terms) {
        let kind = unrolled_kind(term, loop_vars)
        looped ||= kind > 0
        based ||= kind < 0
    }
    return looped && based
}

//! ARCHITECTURE_GPU_PREFILL_WINDOW.md#kernel-load-addressing, per Metal kernel file: a fully unrolled loop reads a
//! bound buffer through a pointer taken outside it, never through a sum of a run-time base and the loop variable.
def private check_load_addressing_in(path : string) {
    var inscope lines <- split(fread(path), "\n")
    var inscope bufs : table<string>
    for (line in lines) {
        let colon = find(line, " : array<")
        if (find(line, "@ssbo") >= 0 && colon >= 0) {
            bufs |> insert(ident_suffix(slice(line, 0, colon)))
        }
    }
    var inscope loop_indent : array<int>
    var inscope loop_vars : array<string>
    var line_no = 0
    for (line in lines) {
        line_no++
        let body = strip(line)
        continue if (empty(body) || starts_with(body, "//"))
        let indent = find(line, body)
        while (!empty(loop_indent) && loop_indent[length(loop_indent) - 1] >= indent) {
            loop_indent |> pop()
            loop_vars |> pop()
        }
        let header = find(body, "for [unroll_full] (")
        if (header >= 0) {
            let var_at = header + length("for [unroll_full] (")
            loop_indent |> push(indent)
            loop_vars |> push(ident_prefix(slice(body, var_at)))
            continue
        }
        continue if (empty(loop_vars))
        for (buf in keys(bufs)) {
            var inscope reads <- indexed_reads(body, buf)
            for (idx in reads) {
                if (sums_loop_var_with_base(idx, loop_vars)) {
                    gate_finding(path, line_no,
                        "{buf}[{idx}] in a fully unrolled loop adds the loop variable to a run-time base a load - a 32-bit index sum does not fold into one address; take the address once outside the loop (`let p = unsafe(addr({buf}[<base>]))`) and read `unsafe(p[<offset>])` (ARCHITECTURE_GPU_PREFILL_WINDOW.md#kernel-load-addressing)")
                }
            }
        }
    }
}

def private check_metal_kernel_load_addressing {
    dir("modules/dasLLAMA/dasllama") $(name) {
        if (starts_with(name, "dasllama_metal_") && ends_with(name, ".das")) {
            check_load_addressing_in("modules/dasLLAMA/dasllama/{name}")
        }
    }
}

//! every Metal GEMM form a format family stamps, every member stamps
def private check_kq_form_matrix {
    var inscope forms_by_fmt, family_forms : table<string; table<string>>
    var inscope first_stamp : table<string; string>
    var inscope first_stamp_line : table<string; int>
    var alltext = ""
    dir("modules/dasLLAMA/dasllama") $(name) {
        return if (!(starts_with(name, "dasllama_metal_") && ends_with(name, ".das")))
        let path = "modules/dasLLAMA/dasllama/{name}"
        let text = fread(path)
        alltext += text
        var line_no = 0
        for (line in split(text, "\n")) {
            line_no++
            let k = find(line, "stamp = \"")
            continue if (k < 0)
            let rest = slice(line, k + length("stamp = \""))
            let q = find(rest, "\"")
            continue if (q < 0)
            var inscope parts <- split(slice(rest, 0, q), ":")
            continue if (length(parts) != 3)
            let key = "{parts[0]}|{parts[1]}"
            forms_by_fmt[key] |> insert(parts[2])
            family_forms[parts[0]] |> insert(parts[2])
            if (!key_exists(first_stamp, key)) {
                first_stamp[key] = path
                first_stamp_line[key] = line_no
            }
        }
    }
    var inscope stamped <- [for (k in keys(forms_by_fmt)); k]
    for (key in stamped) {
        var inscope kp <- split(key, "|")
        let family = kp[0]
        let fmt = kp[1]
        let path = first_stamp?[key] ?? ""
        let line = first_stamp_line?[key] ?? 0
        for (form in keys(family_forms[family])) {
            if (!key_exists(forms_by_fmt[key], form)) {
                gate_finding(path, line,
                    "format {fmt} stamps no {family}:{form} while another format of the family does - a form one format has and another lacks sends the two through different kernel sequences on one machine; stamp it, or drop the form from every member")
            }
        }
        let race = family == "kq_mm" ? "\"kq_mulmm_{fmt}\"" : "\"moe_mulmm_{fmt}\""
        if (find(alltext, race) < 0) {
            gate_finding(path, line, "format {fmt} stamps {family} twins and races none - no {race} verdict picks between its twin and its base; register the race in metal_tensor_race")
        }
        if (family == "kq_mm" && find(alltext, "\"enc_kq_dq_{fmt}\"") < 0 && find(alltext, "enc_dequant_{fmt}h(") < 0) {
            gate_finding(path, line, "format {fmt} stamps dense twins and has no dev-W dequant pass - every dense format takes the dev-W arm or none does")
        }
    }
}

def private check_gpu_role_partition {
    dir("modules/dasLLAMA/dasllama") $(name) {
        return if (!ends_with(name, ".das"))
        for (backend in ["metal", "vulkan", "cuda"]) {
            let prefix = "dasllama_{backend}_"
            continue if (!starts_with(name, prefix))
            var role = ""
            peek_data(name) $(d) {
                role = slice(d, length(prefix), length(d) - length(".das"))
            }
            var known = key_exists(GPU_ROLES_CUDA, role)
            if (backend == "metal") {
                known = key_exists(GPU_ROLES_METAL, role)
            } elif (backend == "vulkan") {
                known = key_exists(GPU_ROLES_VULKAN, role)
            }
            if (!known) {
                gate_finding("modules/dasLLAMA/dasllama/{name}",
                    "backend file with role '{role}' that the partition does not carry - add the role to ARCHITECTURE_GPU.md#gpu-backends's role table and this gate's set in the same change")
            }
        }
    }
}

//! a `class [template] X [: Y]` line's parts; an empty name where `line` declares no class
def private class_header(line : string) : tuple<name : string; base : string; has_base : bool; is_template : bool> {
    return (name = "", base = "", has_base = false, is_template = false) if (!(line |> starts_with("class ")))
    var rest = slice(line, length("class "))
    let is_tpl = rest |> starts_with("template ")
    if (is_tpl) {
        rest = slice(rest, length("template "))
    }
    let colon = find(rest, " : ")
    return (name = ident_prefix(rest), base = colon >= 0 ? ident_prefix(slice(rest, colon + 3)) : "", has_base = colon >= 0, is_template = is_tpl)
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
            let hdr = class_header(line)
            cur = hdr.name
            class_names |> insert(cur)
            if (hdr.has_base) {
                class_base[cur] = hdr.base
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
// `pipeline_from_source` or its dumping shell `pso_from_source` over a literal global, and the
// shared `race_pso_pair` shell in either spelling - plus the local-to-local reads of one. True
// where the line is such a bind and no later check reads it.
def private rb_track_pso(line : string; class_names : table<string>; kernel_class : table<string; string>;
                         var pso_class : table<string; string>) : bool {
    let pfs = find(line, "pipeline_from_source(")
    let pf = pfs >= 0 ? pfs : find(line, "pso_from_source(")
    if (pf >= 0) {
        let pvar = rb_bound_var(line)
        let args = slice(line, find(line, "(", pf) + 1)
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
    var class_base, kernel_class : table<string; string>
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
        var inscope pso_class : table<string; string>   // per-function psoVar -> class
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
        //! the annotation line itself, not a mention in a comment: `[init, boot_restore]` or `[boot_restore, init]` at column 0
        var generated = false
        var restore_line = 0
        var line_no = 0
        for (line in split(strip_line_comments(text), "\n")) {
            line_no++
            if ((line |> starts_with("[")) && annotation_has(line, "init") && annotation_has(line, "boot_restore")) {
                generated = true
                restore_line = line_no
            }
        }
        line_no = 0
        for (line in split(text, "\n")) {
            line_no++
            var rest = ""
            if (line |> starts_with("var private ")) {
                rest = slice(line, length("var private "))
            } elif (line |> starts_with("var ")) {
                rest = slice(line, length("var "))
            } elif (line |> starts_with("let private ") || line |> starts_with("let ")) {
                //! a `let` global's initializer holding function values - a table or struct of `@@fn` - restores as data too, and no [init] can refill a `let`
                let at = find(line, "@@")
                if (at >= 0 && !(find(line, "//") >= 0 && find(line, "//") < at)) {
                    gate_finding(path, line_no, "`let` global carries a function value (`@@`) in its declaration initializer - a serialized exe restores it as NULL and the first invoke dies; declare it `var` and fill it in an [init]")
                }
                continue
            } else {
                continue
            }
            let gname = ident_prefix(rest)
            continue if (!(gname |> starts_with("g_")))
            let eq = find(line, "= @@")
            continue if (eq < 0 || (find(line, "//") >= 0 && find(line, "//") < eq))
            if (!generated && find(text, "if ({gname} == null)") < 0) {
                gate_finding(path, line_no, "function global `{gname}` has a declaration initializer but no `if ({gname} == null)` boot-restore guard - a serialized exe restores it as NULL and the first invoke dies (add it to the file's restore [init], or annotate that [init] `boot_restore`, which writes every guard)")
            } elif (generated && line_no > restore_line) {
                gate_finding(path, line_no, "function global `{gname}` is declared below the `[init, boot_restore]` function (line {restore_line}), which the annotation walks as the parser reaches it - the global gets no guard; move the restore function below every `= @@fn` global")
            }
        }
    }
}

//! the tier's text with comments stripped, "" where the tree carries no tier or its tier has no home switch (a fixture's cut-down copy)
def private gpu_tier_text : string {
    let path = "{DASLLAMA_DIR}/dasllama_gpu_tier.das"
    return "" if (!fexist(path))
    let text = strip_line_comments(fread(path))
    return find(text, "def set_gpu_home_(") < 0 ? "" : text
}

//! the GPU home switch resets the tier's seats: the `= @@fn` globals through `[seat_reset]`'s walk, the flags a home's entry
//! declares by value through `gpu_home_reset_flags` - every `g_*_installed` global but the arm's own `g_moe_gpu_installed`
//! (the switch runs before any arm) and every global a public `install_*` seat, `set_moe_gpu_streams` or
//! `set_rdec_chain_charged` assigns `on`, `true` or `false` must appear in that function's body, or the other home inherits
//! the first one's declaration; the `set_moe_gpu_*_route` levers are the process's, not a home's (ARCHITECTURE_GPU.md#gpu-backends)
def private check_gpu_home_reset_flags {
    let path = "{DASLLAMA_DIR}/dasllama_gpu_tier.das"
    let text = gpu_tier_text()
    return if (empty(text))
    var owed : table<string; int>   //! flag global -> the line that declares or assigns it
    var reset_names : table<string>   //! the globals `gpu_home_reset_flags` assigns
    var in_setter = false
    var in_reset = false
    var seen_reset = false
    var line_no = 0
    for (line in split(text, "\n")) {
        line_no++
        if (line |> starts_with("def ")) {
            in_setter = (line |> starts_with("def public install_")) || (line |> starts_with("def public set_moe_gpu_streams(")) || (line |> starts_with("def public set_rdec_chain_charged("))
            in_reset = line |> starts_with("def private gpu_home_reset_flags")
            seen_reset = seen_reset || in_reset
            continue
        }
        if (in_reset) {
            let body = strip(line)
            if (find(body, " = ") > 0) {
                reset_names |> insert(ident_prefix(body))
            }
            continue
        }
        var rest = ""
        if (line |> starts_with("var private ")) {
            rest = slice(line, length("var private "))
        } elif (line |> starts_with("var ")) {
            rest = slice(line, length("var "))
        }
        if (!empty(rest)) {
            let gname = ident_prefix(rest)
            if ((gname |> starts_with("g_")) && (gname |> ends_with("_installed")) && gname != "g_moe_gpu_installed" && !key_exists(owed, gname)) {
                owed[gname] = line_no
            }
            continue
        }
        if (in_setter) {
            let body = strip(line)
            let eq = find(body, " = ")
            continue if (eq < 0)
            let gname = ident_prefix(body)
            let rhs = strip(slice(body, eq + 3))
            if ((gname |> starts_with("g_")) && gname != "g_moe_gpu_installed" && (rhs == "on" || rhs == "true" || rhs == "false") && !key_exists(owed, gname)) {
                owed[gname] = line_no
            }
        }
    }
    if (!seen_reset) {
        gate_finding(path, 1, "`gpu_home_reset_flags` is missing: the GPU home switch cannot reset the flags a home's entry declares")
        return
    }
    for (gname, at in keys(owed), values(owed)) {
        if (!key_exists(reset_names, gname)) {
            gate_finding(path, at, "`{gname}` is a flag a GPU home's entry declares, and `gpu_home_reset_flags` does not reset it - the other home would inherit this one's declaration after `set_gpu_home`")
        }
    }
}

//! the tier's globals declared `= @@fn` - the seats `[seat_reset]` walks, `var` and `var private` alike
def private fn_seat_globals(text : string) : table<string> {
    var seats : table<string>
    for (line in split(text, "\n")) {
        var rest = ""
        if (line |> starts_with("var private ")) {
            rest = slice(line, length("var private "))
        } elif (line |> starts_with("var ")) {
            rest = slice(line, length("var "))
        }
        if (!empty(rest) && find(rest, "= @@") > 0) {
            seats |> insert(ident_prefix(rest))
        }
    }
    return <- seats
}

//! the setters of the tier's function seats: every function of `dasllama_gpu_tier` whose body assigns one of `fn_seat_globals`
def private gpu_tier_seat_setters(text : string) : table<string> {
    var inscope seats <- fn_seat_globals(text)
    var setters : table<string>
    var fn = ""
    for (line in split(text, "\n")) {
        if (line |> starts_with("def ")) {
            fn = def_name_of(line)
            continue
        }
        continue if (empty(fn))
        let body = strip(line)
        continue if (find(body, " = ") <= 0)
        if (key_exists(seats, ident_prefix(body))) {
            setters |> insert(fn)
        }
    }
    return <- setters
}

//! `name` among the names of a function's annotation lines (`[init]`, `[init, arch(...)]`, `[boot_restore, init]`, several lines joined)
def private annotation_has(ann, name : string) : bool {
    for (group in split(ann, "]")) {
        let open = find(group, "[")
        continue if (open < 0)
        for (part in split(slice(group, open + 1), ",")) {
            return true if (strip(part) == name)
        }
    }
    return false
}

//! the home switch resets every seat and refills them from the installers alone (`register_gpu_home`, `register_gpu_home_shared`):
//! a seat an `[init]` fills directly is empty after `set_gpu_home`, and the tier reads its declared default (a provider that answers
//! nothing, a hook that declines) with no line said
def private check_gpu_home_seat_registrants {
    let tier_text = gpu_tier_text()
    return if (empty(tier_text))
    var inscope setters <- gpu_tier_seat_setters(tier_text)
    dir(DASLLAMA_DIR) $(name) {
        return if (!ends_with(name, ".das") || name == "dasllama_gpu_tier.das")
        let path = "{DASLLAMA_DIR}/{name}"
        var ann = ""
        var fn = ""
        var fn_init = false
        var line_no = 0
        for (line in split(strip_line_comments(fread(path)), "\n")) {
            line_no++
            if (line |> starts_with("[")) {
                ann += line
                continue
            }
            if (line |> starts_with("def ")) {
                fn = def_name_of(line)
                fn_init = annotation_has(ann, "init")
                ann = ""
                continue
            }
            if (line |> starts_with("}")) {
                fn_init = false
                continue
            }
            continue if (!fn_init)
            for (setter in keys(setters)) {
                if (find(line, "{setter}(") >= 0) {
                    gate_finding(path, line_no, "`{setter}` fills a GPU home seat from `[init]` (`{fn}`): `set_gpu_home` resets every seat and refills them from the installers alone - register the call with `register_gpu_home` under its home, or `register_gpu_home_shared` where every home holds it alike, and run the installer from the `[init]` (ARCHITECTURE_GPU.md#gpu-backends)")
                }
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
        var inscope closes : array<int>
        var line_no = 0
        for (line in split(strip_line_comments(fread(p)), "\n")) {
            line_no++
            if (line |> starts_with("def ")) {
                report_stray_closes(p, fn, opened, closes)
                fn = def_name_of(line)
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
let private IMAGE_LAYOUT_STAMP_VERSION = 56
let private IMAGE_LAYOUT_STAMP_HASH = 0x9f51a1dde3ef8ae1ul

// The helpers that decide WHERE bytes land: the page pad, the plane and total sizing, the
// writer's append / zero-fill / header patch, the header's scalar stores, the lattice
// plane's section name, and the TTS blob's aligned append, its per-weight walk slot and the
// conv's stacked-row gather the q8 mint quantizes. Changing one moves every plane offset (or
// renames every lattice section) while every serializer body reads the same.
var private LAYOUT_HELPERS <- {
    "pad_to_page", "w_page", "plane_end", "image_total_bytes",
    "w_append", "w_zeros", "w_header", "store_at", "kq_plane_name",
    "blob_push", "weight_slot", "conv1d_weight_row",
    "vk_mint_plane", "vk_mint_walk", "vk_mint_sink_append",
    "gg_stream_plane"
}

// The constants those helpers place bytes by, and the bands the two streaming writers stage
// through. Their declaration lines are hashed with the bodies, so a value change reds the same
// way an edited body does - the closure is deliberately wider than the byte-moving set, and a
// change that provably moves nothing re-stamps the hash alone.
var private LAYOUT_CONSTANTS <- {
    "IMAGE_PAGE", "IMAGE_HEADER_BYTES", "SECTION_TABLE_SLACK",
    "DWRITE_STAGING_BAND", "METAL_BAND_BLOCKS", "BLOB_ALIGN_FLOATS"
}

def private is_archive_serializer(header : string) : bool =>
    (header |> starts_with("def serialize") || header |> starts_with("def private serialize")) && find(header, "Archive") >= 0

// A meta serializer hand-lists a STRUCT's fields, so its count can be pinned; the bulk payload
// forms take an `array` and have no field list of their own.
def private is_meta_serializer(header : string) : bool =>
    is_archive_serializer(header) && find(header, "array<") < 0

//! the `*_prepare` mints that place image bytes are the TTS block file's (`conv1d_prepare`, `linear_prepare`); a driver's prepare (`vk_rdec_prepare`, `mirror_prepare`) places none
def private is_layout_function(file : string; header : string) : bool {
    return true if (is_archive_serializer(header))
    let name = def_name_of(header)
    return true if (key_exists(LAYOUT_HELPERS, name))
    return name == "build_image" || name == "parse_image" || (find(name, "_prepare") >= 0 && "{DASLLAMA_DIR}/{file}" == TTS_BLOCKS_FILE)
}

def private read_lf(path : string) : string => replace(fread(path), "\r\n", "\n")

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
            "the image layout closure changed (every Archive serializer, build_image, parse_image, every *_prepare mint of dasllama_tts_blocks.das, the layout helpers pad_to_page/w_page/plane_end/image_total_bytes/w_append/w_zeros/w_header/store_at/kq_plane_name/blob_push/weight_slot/conv1d_weight_row, and the layout constants IMAGE_PAGE/IMAGE_HEADER_BYTES/SECTION_TABLE_SLACK/DWRITE_STAGING_BAND/METAL_BAND_BLOCKS/BLOB_ALIGN_FLOATS): re-stamp IMAGE_LAYOUT_STAMP_HASH = {h}ul in modules/dasLLAMA/REVIEW.das; bump IMAGE_VERSION (still {version}) and IMAGE_LAYOUT_STAMP_VERSION with it only if the bytes moved, because a stale image at an unchanged path stays structurally valid and serves different bytes")
    } else {
        gate_finding("modules/dasLLAMA/REVIEW.das",
            "the layout stamp is behind IMAGE_VERSION {version} - re-stamp: IMAGE_LAYOUT_STAMP_HASH = {h}ul, IMAGE_LAYOUT_STAMP_VERSION = {version}")
    }
}

// ===== the Vulkan checklist's mechanical checks (REVIEW_GPU_VULKAN.md) =====

let private VK_CLASSES = "modules/dasLLAMA/dasllama/dasllama_vulkan_classes.das"
let private DISCRETE_KERNELS = "modules/dasLLAMA/dasllama/dasllama_gpu_kernels_discrete.das"   //! the bodies the discrete homes stamp: the GEMV family and its grid literals
let private KQ_FORMAT = "modules/dasLLAMA/dasllama/dasllama_kqformat.das"
let private VK_COMMON = "modules/dasLLAMA/dasllama/dasllama_vulkan_common.das"
let private GPU_TIER = "modules/dasLLAMA/dasllama/dasllama_gpu_tier.das"   //! the constants both discrete homes' drivers read
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
    for (line in class_lines(DISCRETE_KERNELS, "class template KqCm2BatchT")) {
        let s = strip(line)
        continue if (!(s |> starts_with("def ")) || find(s, "khr_stage16(") < 0)
        found = true
        if (!(s |> starts_with("def abstract khr_stage16"))) {
            gate_finding(DISCRETE_KERNELS, "khr_stage16 on KqCm2BatchT has a body - it stays abstract, so a format template that forgets its override fails to compile instead of decoding garbage on every KHR card")
        }
    }
    if (!found) {
        gate_finding(DISCRETE_KERNELS, "no `def khr_stage16` inside `class template KqCm2BatchT` - the KHR stage's abstract rule has nothing to read; keep the spelling or move this check with it")
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
    var inscope common_lines <- split(strip_line_comments(fread(GPU_TIER)), "\n")
    let constant = literal_after(common_lines, "let public RDEC_AR_MAX_DIM = ")
    var inscope ar_lines <- class_lines(DISCRETE_KERNELS, "class template ArBase")
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
        gate_finding(GPU_TIER, "no `let public RDEC_AR_MAX_DIM = N` line - the add+rms slab triple has no constant to compare against")
    }
    if (slab < 0) {
        gate_finding(DISCRETE_KERNELS, "no `@workgroup row : float[N]` inside `class template ArBase` - the add+rms slab triple has no slab to compare")
    }
    if (gate < 0) {
        gate_finding(BLOCKS_FILE, "no `c.dim > N` inside attn_dec_shape_ok - the add+rms slab triple has no servability cap to compare")
    }
    return if (constant < 0 || slab < 0 || gate < 0)
    if (constant != slab || constant != gate) {
        gate_finding(VK_CLASSES, "the add+rms row slab triple disagrees: RDEC_AR_MAX_DIM {constant} (dasllama_gpu_tier.das), ArBase.row[{slab}], attn_dec_shape_ok's c.dim cap {gate} (dasllama_blocks.das) - a cap past the slab writes past its end on the device; change the three together")
    }
}

var private CM2_KHR_EXEMPT : table<string>   //! every format on the cm2 template owns a KHR arm today; a per-32 format the feed never admits in mm mode would join here
var private CM2_NO_EXEMPT : table<string>

//! every `class template <Fmt>Cm2T` on the cm2 base - directly, or through another format's template (`Iq4nlCm2T : Q40Cm2T`)
def private cm2_format_templates(exempt : table<string>) : array<string> {
    var out : array<string>
    for (line in split(strip_line_comments(fread(DISCRETE_KERNELS)), "\n")) {
        continue if (!(line |> starts_with("class template ")))
        let name = ident_prefix(slice(line, length("class template ")))
        let base = cm2_template_base(line)
        if (name |> ends_with("Cm2T") && (base == "KqCm2BatchT" || base |> ends_with("Cm2T")) && !key_exists(exempt, name)) {
            out |> push(name)
        }
    }
    return <- out
}

//! the base a `class [template] X : Y {` line names, "" when the line names none
def private cm2_template_base(line : string) : string {
    let colon = find(line, " : ")
    return colon < 0 ? "" : ident_prefix(slice(line, colon + 3))
}

//! a cm2 format template's class lines, then its bases' up to the cm2 base: an override inherited from a parent template counts
def private cm2_template_chain_lines(name : string) : array<string> {
    var out : array<string>
    var cur = name
    while (!empty(cur) && cur != "KqCm2BatchT") {
        out |> push_from(class_lines(DISCRETE_KERNELS, "class template {cur}"))
        var next = ""
        for (line in split(strip_line_comments(fread(DISCRETE_KERNELS)), "\n")) {
            if (line |> starts_with("class template {cur} ")) {
                next = cm2_template_base(line)
                break
            }
        }
        cur = next
    }
    return <- out
}

//! one class ladder trio every format template on the cm2 base joins
struct private LadderTrio {
    suffix : string     //! the stamp's class suffix: `<Fmt>` + suffix
    tail : string       //! the dispatch name's tail: `kq_batch_<fmt>_` + tail; a per-32 format (q8, q51, mx4, iq4nl32, q4032, q5032) spells `<fmt>_batch_` + tail
    ladder : string     //! a ladder trio of this tile's own beside the engine's `cm2_cls_*` (the kernel cells' `khr_cls_`), "" where there is none
    why : string        //! what a missing arm does at run time
}

def private cm2_dispatch_name(fmt, tail : string) : string =>
    fmt == "q8" || fmt == "q51" || fmt == "mx4" || fmt == "iq4nl32" || fmt == "q4032" || fmt == "q5032" ? "{fmt}_batch_{tail}" : "kq_batch_{fmt}_{tail}"

//! the words of a `let [private] NAME = "..."` line in `path`, or none with a finding: a roster the gate reads where the code reads it
def private let_words(path, name : string) : array<string> {
    for (line in split(strip_line_comments(fread(path)), "\n")) {
        var from = -1
        for (marker in fixed_array("let {name} = \"", "let private {name} = \"")) {
            if (line |> starts_with(marker)) {
                from = length(marker)
            }
        }
        continue if (from < 0)
        return <- split(slice(line, from, find(line, "\"", from)), " ")
    }
    gate_finding(path, "no `let {name} = \"...\"` line - the rule reading it has no roster; keep the spelling or move this check with it")
    var none : array<string>
    return <- none
}

//! the words of `let KQ_CM2E_ALIASES_M = "..."` in the kqformat module: the formats whose e column the m stamp serves
def private kqformat_cm2e_aliases : array<string> => let_words(KQ_FORMAT, "KQ_CM2E_ALIASES_M")

//! the engine's one cm2 class ladder, `cm2_cls_*`: stamped by `kq_tile_stamp` over `tile_<verb>()` with the `CM2_TILE_NONE` skip
//! list - a `<fmt>/<tail>` pair on that list whose tail is this trio's and whose format has a template on the cm2 base hides the
//! template behind the skip, and the format panics at its first dispatch of the tile
def private check_cm2_live_ladder(trio : LadderTrio; templates : array<string>) {
    var inscope ladders <- function_bodies(VK_CLASSES) $(header) {
        return header |> starts_with("def cm2_cls_ensure") || header |> starts_with("def cm2_cls_set") || header |> starts_with("def cm2_cls_enc")
    }
    if (length(ladders) != 3) {
        gate_finding(VK_CLASSES, "found {length(ladders)} of the three `cm2_cls_ensure` / `cm2_cls_set` / `cm2_cls_enc` ladders - keep the spelling or move this check with it")
        return
    }
    for (f in ladders) {
        let ladder = ident_prefix(slice(f.body, length("def ")))
        let placeholder = "tile_{slice(ladder, length("cm2_cls_"))}("
        if (find(f.body, "kq_tile_stamp(fmt, CM2_TILE_NONE, ") < 0 || find(f.body, placeholder) < 0) {
            gate_finding(VK_CLASSES, f.line, "{ladder} is not stamped by `kq_tile_stamp(fmt, CM2_TILE_NONE, ...)` over `{placeholder}` - the stamp walks every KqFmt member, so a format without the class fails the compile instead of {trio.why}")
        }
    }
    var inscope skips <- let_words(VK_CLASSES, "CM2_TILE_NONE")
    for (word in skips) {
        let slash = find(word, "/")
        continue if (slash < 0 || slice(word, slash + 1) != trio.tail)
        let fmt = slice(word, 0, slash)
        for (tpl in templates) {
            if (to_lower(slice(tpl, 0, length(tpl) - length("Cm2T"))) == fmt) {
                gate_finding(VK_CLASSES, "CM2_TILE_NONE skips {word} while `class template {tpl}` sits on the cm2 base - a skipped (format, tile) pair panics at its first dispatch, so {trio.why}")
            }
        }
    }
}

//! a stamp's k step off its class lines: the cm2 template's BK unless the stamp overrides it
def private stamp_bk(lines : array<string>; template_bk : int) : int {
    let over = literal_after(lines, "override BK = ")
    return over < 0 ? template_bk : over
}

//! the cm2 template's own BK (`@template_constant BK : uint = N`), -1 when the line is not there
def private cm2_template_bk : int {
    var inscope base_lines <- class_lines(DISCRETE_KERNELS, "class template KqCm2BatchT")
    return literal_after(base_lines, "@template_constant BK : uint = ")
}

def private check_cm2_ladder_set(trio : LadderTrio; exempt : table<string>) {
    var inscope templates <- cm2_format_templates(exempt)
    if (empty(templates)) {
        gate_finding(DISCRETE_KERNELS, "no `class template <Fmt>Cm2T : KqCm2BatchT` found - the {trio.ladder}* ladder rule has no templates to walk; keep the spelling or move this check with it")
        return
    }
    let classes = strip_line_comments(fread(VK_CLASSES))
    check_cm2_live_ladder(trio, templates)
    var inscope ladders <- function_bodies(VK_CLASSES) $(header) {
        return !empty(trio.ladder) && (header |> starts_with("def {trio.ladder}ensure") || header |> starts_with("def {trio.ladder}set") || header |> starts_with("def {trio.ladder}enc"))
    }
    if (!empty(trio.ladder) && length(ladders) != 3) {
        gate_finding(VK_CLASSES, "found {length(ladders)} of the three `{trio.ladder}ensure` / `{trio.ladder}set` / `{trio.ladder}enc` ladders - keep the spelling or move this check with it")
    }
    for (f in ladders) {
        let ladder = ident_prefix(slice(f.body, length("def ")))
        let placeholder = "tile_{slice(ladder, length(trio.ladder))}_{trio.tail}("
        let stamp_at = find(f.body, "kq_tile_stamp(")
        if (stamp_at < 0 || find(f.body, placeholder) < 0) {
            gate_finding(VK_CLASSES, f.line, "{ladder} is not stamped by `kq_tile_stamp` over `{placeholder}` - the stamp walks every KqFmt member, so a format without the class fails the compile instead of {trio.why}")
            continue
        }
        //! a skip list takes a member off the ladder, and the innermost verify then panics at the format's first dispatch - the gate reads it so a cm2 template never hides behind one
        let call = slice(f.body, stamp_at, find(f.body, ")", stamp_at))
        let q0 = find(call, "\"")
        continue if (q0 < 0)
        for (word in split(slice(call, q0 + 1, find(call, "\"", q0 + 1)), " ")) {
            continue if (empty(word))
            for (tpl in templates) {
                if (to_lower(slice(tpl, 0, length(tpl) - length("Cm2T"))) == word) {
                    gate_finding(VK_CLASSES, f.line, "{ladder} skips {word} in its `kq_tile_stamp` list while `class template {tpl}` sits on the cm2 base - a skipped format panics at its first dispatch, so {trio.why}")
                }
            }
        }
    }
    let e_trio = trio.suffix == "Cm2EBatch"
    var inscope aliases <- kqformat_cm2e_aliases()
    let base_bk = cm2_template_bk()
    for (tpl in templates) {
        let pfx = slice(tpl, 0, length(tpl) - length("Cm2T"))
        let fmt = to_lower(pfx)
        let dispatch = cm2_dispatch_name(fmt, trio.tail)
        if (e_trio) {
            //! the e column steps k as the s column does (32 on the grid-codebook formats), so the s stamp's BK against the m stamp's says whether an e stamp would differ from the m stamp
            var inscope slines <- class_lines(VK_CLASSES, "class {pfx}Cm2SBatch")
            var inscope mlines <- class_lines(VK_CLASSES, "class {pfx}Cm2MBatch")
            let has_m = !empty(mlines)
            let m_bk = stamp_bk(mlines, base_bk)
            let e_is_m = has_m && !empty(slines) && stamp_bk(slines, base_bk) == m_bk
            let aliased = find_index(aliases, fmt) >= 0
            if (aliased && !has_m) {
                gate_finding(VK_CLASSES, "{tpl}: KQ_CM2E_ALIASES_M names {fmt} but the file has no `class {pfx}Cm2MBatch` for its e column to alias")
            } elif (aliased && !e_is_m) {
                gate_finding(VK_CLASSES, "{tpl}: KQ_CM2E_ALIASES_M names {fmt} but its expert column steps k by {stamp_bk(slines, base_bk)} (its s stamp's BK), not the m stamp's {m_bk} - the format leaves the roster and keeps its own e stamp")
            } elif (!aliased && e_is_m) {
                gate_finding(VK_CLASSES, "{tpl}: its expert column steps k by {m_bk} (its s stamp's BK), so its e stamp is the m stamp byte for byte - {fmt} joins KQ_CM2E_ALIASES_M in dasllama_kqformat.das and ships no e stamp")
            }
            if (aliased) {
                if (find(classes, "class {pfx}{trio.suffix} : {tpl}") >= 0) {
                    gate_finding(VK_CLASSES, "{tpl} keeps a `class {pfx}{trio.suffix}` its ladder never picks - {fmt} aliases the m stamp (KQ_CM2E_ALIASES_M); delete the stamp or take the format off the roster")
                }
                continue
            }
        }
        if (find(classes, "class {pfx}{trio.suffix} : {tpl}") < 0) {
            gate_finding(VK_CLASSES, "{tpl} has no `class {pfx}{trio.suffix} : {tpl}` - a format on the cm2 template ships its {trio.suffix} stamp in the same change, or {trio.why}")
        }
        if (find(classes, "name = \"{dispatch}\"") < 0) {
            gate_finding(VK_CLASSES, "{tpl} has no `[vk_dispatch(name = \"{dispatch}\"` stamp - its {trio.suffix} class has no dispatch")
        }
    }
}

def private check_cm2_ladder_sets {
    check_cm2_ladder_set(LadderTrio(suffix = "KhrBatch", tail = "khr_cls", ladder = "khr_cls_",
        why = "the f16 feed admits the format on a KHR-mode card, where the KHR ladders panic; a per-32 format the feed never admits there joins CM2_KHR_EXEMPT in this file and the licensed set ARCHITECTURE_GPU_VULKAN.md names, in the same change"), CM2_KHR_EXEMPT)
    check_cm2_ladder_set(LadderTrio(suffix = "Cm2EBatch", tail = "cm2e_cls", ladder = "",
        why = "the expert schedule takes the format's planes, and the e arm panics on the first resident MoE window"), CM2_NO_EXEMPT)
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
            out |> push(VkStamp(dispatch_name = pending, line = line_no,
                cls = ident_prefix(slice(line, length("class "))),
                base = cm2_template_base(line)))
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
    var inscope tlines <- cm2_template_chain_lines(st.base)
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
    if (st.dispatch_name |> ends_with("_cm2m_cls") && bn != d.m_rows) {
        let head = slice(st.dispatch_name, 0, length(st.dispatch_name) - length("_cm2m_cls"))
        let fmt = head |> starts_with("kq_batch_") ? slice(head, length("kq_batch_")) : slice(head, 0, find(head, "_batch"))
        if (find_index(kqformat_cm2e_aliases(), fmt) >= 0) {
            gate_finding(path, st.line, "{st.dispatch_name}: BN {bn} on the m stamp of {fmt}, whose expert column IS this stamp (KQ_CM2E_ALIASES_M) - the expert schedule's m pieces are SCHED_M_ROWS ({d.m_rows}) rows wide; keep BN or take the format off the roster and give it an e stamp")
        }
    }
}

//! a grid format's GEMV leaf template carries its codebook's words and first word as literal template constants
//! (`override GRID_WORDS` / `override GRID_OFF`): each held to the `let KQ_GRID_<FMT>_WORDS` line and the
//! `let KQ_GRID_<FMT> = KQ_GRID_<PREV> + KQ_GRID_<PREV>_WORDS` chain the host fill (`kq_grid_dev`) reads
def private check_kq_gemv_grid_literals {
    var inscope lines <- split(strip_line_comments(fread(DISCRETE_KERNELS)), "\n")
    var words, offs : table<string; int>
    var templates : array<string>
    for (line in lines) {
        if (line |> starts_with("let KQ_GRID_")) {
            let name = ident_prefix(slice(line, length("let ")))
            continue if (name == "KQ_GRID_WORDS" || name == "KQ_GRID_BYTES")
            let rhs = strip(slice(line, find(line, "=") + 1))
            if (name |> ends_with("_WORDS")) {
                words[slice(name, length("KQ_GRID_"), length(name) - length("_WORDS"))] = literal_after([line], "= ")
            } elif (is_number(first_character(rhs))) {
                offs[slice(name, length("KQ_GRID_"))] = literal_after([line], "= ")
            } else {
                let prev = slice(ident_prefix(rhs), length("KQ_GRID_"))
                offs[slice(name, length("KQ_GRID_"))] = (offs?[prev] ?? -1) + (words?[prev] ?? -1)
            }
        } elif (line |> starts_with("class template ") && find(line, "GemvT : KqGemvLeafT") >= 0) {
            templates |> push(ident_prefix(slice(line, length("class template "))))
        }
    }
    if (empty(templates)) {
        gate_finding(DISCRETE_KERNELS, "no `class template <Fmt>GemvT : KqGemvLeafT` found - the grid literal rule has no templates to walk; keep the spelling or move this check with it")
        return
    }
    for (tpl in templates) {
        var inscope body <- class_lines(DISCRETE_KERNELS, "class template {tpl}")
        continue if (ident_after(body, "override GRID = ") != "true")
        let fmt = to_upper(slice(tpl, 0, length(tpl) - length("GemvT")))
        let want_words = words?[fmt] ?? -1
        let want_off = offs?[fmt] ?? -1
        let got_words = literal_after(body, "override GRID_WORDS = ")
        let got_off = literal_after(body, "override GRID_OFF = ")
        if (want_words < 0 || want_off < 0) {
            gate_finding(DISCRETE_KERNELS, "{tpl}: no `let KQ_GRID_{fmt}_WORDS` / `let KQ_GRID_{fmt}` for its grid - the host fill and the stamp read the codebook from different rosters")
        } elif (got_words != want_words || got_off != want_off) {
            gate_finding(DISCRETE_KERNELS, "{tpl}: `override GRID_WORDS = {got_words}u` / `override GRID_OFF = {got_off}u` against KQ_GRID_{fmt}_WORDS = {want_words} at word {want_off} - a template constant is a literal, so the stamp's copy moves with the `let` chain in the same change")
        }
    }
}

def private check_cm2_stamp_tiles {
    var inscope templates <- cm2_format_templates(CM2_NO_EXEMPT)
    var inscope base_lines <- class_lines(DISCRETE_KERNELS, "class template KqCm2BatchT")
    let bn0 = literal_after(base_lines, "@template_constant BN : uint = ")
    let bk0 = literal_after(base_lines, "@template_constant BK : uint = ")
    let unr0 = literal_after(base_lines, "@template_constant UNR : uint = ")
    var inscope sched_lines <- split(strip_line_comments(fread(DISCRETE_KERNELS)), "\n")   //! the schedule's widths sit beside the schedule template both discrete homes stamp
    let s_rows = literal_after(sched_lines, "let SCHED_S_ROWS = ")
    let m_rows = literal_after(sched_lines, "let SCHED_M_ROWS = ")
    if (bn0 < 0 || bk0 < 0 || unr0 < 0) {
        gate_finding(DISCRETE_KERNELS, "no `@template_constant BN : uint = N` / `BK : uint = N` / `UNR : uint = N` inside `class template KqCm2BatchT` - the stamp tile rule has no defaults to read; keep the spelling or move this check with it")
    }
    if (s_rows < 0 || m_rows < 0) {
        gate_finding(DISCRETE_KERNELS, "no `let SCHED_S_ROWS = N` / `let SCHED_M_ROWS = N` line - the stamp tile rule has no schedule widths to compare; keep the spelling or move this check with it")
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

//! every identifier of `text` that follows `prefix` (the prefix itself excluded); `quoted` keeps
//! only those a closing quote ends - a whole-string literal, never a name inside prose
def private ident_tokens_after(text, prefix : string; quoted : bool) : array<string> {
    var out : array<string>
    peek_data(text) $(d) {
        var from = 0
        while (true) {
            let at = find(d, prefix, from)
            break if (at < 0)
            let start = at + length(prefix)
            var n = start
            while (n < length(d) && is_ident_byte(int(d[n]))) {
                n++
            }
            if (n > start && (!quoted || (n < length(d) && int(d[n]) == '"'))) {
                out |> push(slice(d, start, n))
            }
            from = n
        }
    }
    return <- out
}

// every whole-string `"VK_*"` literal of `text` - an extension read by name, never a name inside prose
def private vk_ext_literals(text : string) : array<string> {
    var out <- [for (name in ident_tokens_after(text, "\"", true)); name; where name |> starts_with("VK_")]
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

//! the device copies and clears the CUDA home issues; each names its stream as the last argument
let private CUDA_STREAM_OPS <- ["cuda_memset_d8(", "cuda_memcpy_htod(", "cuda_memcpy_dtoh(", "cuda_memcpy_dtod("]

//! the argument text of the first `call` (its text ending in `(`) on `line`, "" when the line holds no such call
def private call_args_of(line : string; call : string) : string {
    let at = find(line, call)
    return "" if (at < 0)
    let open = at + length(call) - 1
    var out = ""
    peek_data(line) $(d) {
        let close = paren_close(d, open)
        if (close >= 0) {
            out = slice(d, open + 1, close)
        }
    }
    return out
}

//! the offsets of the commas of `d[from, to)` outside every bracket: the argument separators of a call's argument text
def private arg_commas(d; from, to : int) : array<int> {
    var depth = 0
    var cuts : array<int>
    for (i in range(from, to)) {
        let c = int(d[i])
        if (c == '(' || c == '[' || c == '{') {
            depth++
        } elif (c == ')' || c == ']' || c == '}') {
            depth--
        } elif (c == ',' && depth == 0) {
            cuts |> push(i)
        }
    }
    return <- cuts
}

//! the last top-level comma-separated argument of `args`, stripped
def private last_arg_of(args : string) : string {
    var cut = -1
    peek_data(args) $(d) {
        var inscope cuts <- arg_commas(d, 0, length(d))
        cut = empty(cuts) ? -1 : cuts[length(cuts) - 1]
    }
    return strip(cut < 0 ? args : slice(args, cut + 1))
}

def private check_cuda_stream_discipline {
    var files : array<string>
    collect_das_files("modules/dasLLAMA/dasllama", files)
    for (p in files) {
        let name = base_name(p)
        continue if (!(name |> starts_with("dasllama_cuda_")) && name != "dasllama_math_cuda.das")
        var line_no = 0
        for (line in split(strip_line_comments(fread(p)), "\n")) {
            line_no++
            for (op in CUDA_STREAM_OPS) {
                let args = call_args_of(line, op)
                continue if (empty(args))
                let last = last_arg_of(args)
                if (!(last == "s" || last == "stream" || last |> ends_with(".stream"))) {
                    gate_finding(p, line_no, "{op}...) names no stream - the home's stream is non-blocking and a stream-less copy or clear runs on the legacy stream, unordered against every kernel (ARCHITECTURE_GPU.md#gpu-backends); pass g_cud.stream (or the launch's `s`) as the last argument")
                }
            }
        }
    }
    delete files
}


// The whole-model driver's seats are one set the two discrete homes both fill (ARCHITECTURE_GPU_ASYMMETRIES.md#gpu-asymmetries,
// the seats entry): a seat the Vulkan registration installs and the CUDA registration does not is a gap the entry or a
// ledger row rules, named here with its ruling - a new Vulkan seat lands on the CUDA home in the same change, or lands a row.
let private SEAT_PREFIXES <- [ "install_rdec_", "install_moe_gpu_resident", "set_moe_gpu_", "set_dlim_", "register_" ]
var private CUDA_SEATS_RULED <- {
    "install_rdec_dn_snap_ring" => "a hyper-connection seat (the n-gram conv ring) - the CUDA home declines the chain by name",
    "set_moe_gpu_attn_dec_hooks" => "a per-op tier hook - the Vulkan family's",
    "set_moe_gpu_ffn_bias_hook" => "a per-op tier hook - the Vulkan family's",
    "set_moe_gpu_ffn_tail_hooks" => "a per-op tier hook - the Vulkan family's",
    "set_moe_gpu_heat_hooks" => "a per-op tier hook - the Vulkan family's",
    "set_moe_gpu_qkv_hook" => "a per-op tier hook - the Vulkan family's",
    "set_moe_gpu_span_dec_hook" => "a per-op tier hook - the Vulkan family's",
    "set_moe_gpu_host_bytes_fn" => "the mint census's host-bytes line - the CUDA home keeps no host-visible count",
    "set_moe_gpu_unmap_notify" => "the mapping's unmap notice - the CUDA home imports no mapping"
}

//! the seat names a registration file calls: an identifier under one of `SEAT_PREFIXES` followed by `(`
def private seats_called(path : string) : table<string> {
    var seats : table<string>
    for (line in split(strip_line_comments(fread(path)), "\n")) {
        for (prefix in SEAT_PREFIXES) {
            var at = find(line, prefix)
            while (at >= 0) {
                let name = ident_prefix(slice(line, at))
                if (slice(line, at + length(name)) |> starts_with("(")) {
                    seats |> insert(name)
                }
                at = find(line, prefix, at + 1)
            }
        }
    }
    return <- seats
}

def private check_discrete_seat_parity {
    let vk_path = "modules/dasLLAMA/dasllama/dasllama_math_vulkan.das"
    let cud_path = "modules/dasLLAMA/dasllama/dasllama_math_cuda.das"
    return if (!fexist(vk_path) || !fexist(cud_path))   // a fixture tree without the two entries has no parity to hold
    var vk <- seats_called(vk_path)
    var cud <- seats_called(cud_path)
    for (seat in keys(vk)) {
        continue if (key_exists(cud, seat) || key_exists(CUDA_SEATS_RULED, seat))
        gate_finding(cud_path, "the Vulkan registration installs `{seat}` and the CUDA registration does not - install it on the CUDA home, or rule it in this gate's CUDA_SEATS_RULED where it serves a mechanism the CUDA home does not have (a per-op tier, the hyper-connection chain, a host-visible plane count, a mapped import - the property ARCHITECTURE_GPU_ASYMMETRIES.md#gpu-asymmetries' seats entry names), and name it in that entry; a seat the CUDA home could fill is a finding, never a ruling")
    }
    for (seat in keys(CUDA_SEATS_RULED)) {
        if (key_exists(cud, seat)) {
            gate_finding(cud_path, "`{seat}` is ruled Vulkan-only in this gate's CUDA_SEATS_RULED and the CUDA registration installs it - drop the ruling here and in ARCHITECTURE_GPU_ASYMMETRIES.md#gpu-asymmetries' seats entry")
        } elif (!key_exists(vk, seat)) {
            gate_finding(vk_path, "`{seat}` is ruled Vulkan-only in this gate's CUDA_SEATS_RULED and the Vulkan registration no longer installs it - drop the ruling here and in ARCHITECTURE_GPU_ASYMMETRIES.md#gpu-asymmetries' seats entry")
        }
    }
    delete vk
    delete cud
}

// A Vulkan pipeline is built only by the `[vk_dispatch]`-generated `ensure_*`: a hand build bypasses the
// hazard rail and the dispatch census, so its races and its coverage are invisible. The two llama.cpp
// shader ports under performance/ are reference measurements of another engine's kernels, outside this walk.
def private check_no_hand_pipelines {
    gate_banned_tokens(["modules/dasLLAMA/dasllama", "modules/dasLLAMA/harness", "modules/dasLLAMA/tests"], "", ["vkCreateComputePipelines("],
        "hand-built Vulkan pipeline - a pipeline is created only by a [vk_dispatch]-generated ensure_*, which puts it on the hazard rail and in the dispatch census") $(_line) {
        return false
    }
}

//! every engine, harness and test context submits on the process's one shared device, whose queues other contexts' threads
//! also submit on: a submit or a wait takes the queue's lock (ARCHITECTURE_GPU_VULKAN_RESIDENCY.md#vk-shared-device)
def private check_locked_queue_calls(files : array<string>) {
    gate_banned_tokens_in(files, "", ["run_cmd_sync(", "vkQueueSubmit(", "vkQueueWaitIdle(", "vkDeviceWaitIdle("],
        "submits or waits on a Vulkan queue without the shared device's queue lock - another context's thread can be on the same queue, and an unlocked vkQueueSubmit/vkQueueWaitIdle beside it is a validation-layer threading error and an intermittent DEVICE_LOST; record through cmd_one_shot or submit through submit_wait/timeline_submit, which call vk_queue_submit_locked and vk_queue_wait_idle_locked (ARCHITECTURE_GPU_VULKAN_RESIDENCY.md#vk-shared-device)") $(_line) {
        return false
    }
}

def private check_locked_queue_calls {
    var files : array<string>
    for (folder in ["modules/dasLLAMA/dasllama", "modules/dasLLAMA/harness", "modules/dasLLAMA/tests"]) {
        collect_das_files(folder, files)
    }
    check_locked_queue_calls(files)
    delete files
}

let private RESIDENT_FILE = "modules/dasLLAMA/dasllama/dasllama_gpu_resident.das"

//! the resident verify seat in call order: the gated entry, then the body it calls. Every decline in either - a `return`
//! whose value is not `true` - sits above the call that moves the session's recurrent state to the device (`rdec_dn_own_all`),
//! since a decline below it hands the CPU verify a session whose state the device holds
//! (ARCHITECTURE_GPU_VULKAN_MTP.md#resident-verify-rollback)
let private VERIFY_SEAT_BODIES <- ["vulkan_resident_verify_gated", "vulkan_resident_verify_go"]

//! a line's `return` value where the line is a return statement (the `if` tail of a postfix form dropped), "" elsewhere
def private return_value_of(line : string) : string {
    let s = strip(line)
    return "" if (!(s |> starts_with("return ")))
    let expr = slice(s, length("return "))
    let tail = find(expr, " if ")
    return strip(tail >= 0 ? slice(expr, 0, tail) : expr)
}

def private check_verify_decline_before_state_move {
    var inscope fns <- function_bodies(RESIDENT_FILE) $(header) {   // the bodies come comment-stripped: a `rdec_dn_own_all` or a `return` in a comment counts for nothing
        for (name in VERIFY_SEAT_BODIES) {
            return true if (header |> starts_with("def private {name}("))
        }
        return false
    }
    return if (empty(fns))   //! a tree without the seat bodies (the walker fixtures, a build without the Vulkan seats) has no decline to order
    var moved = false
    for (name in VERIFY_SEAT_BODIES) {
        var seen = false
        for (f in fns) {
            continue if (!(f.body |> starts_with("def private {name}(")))
            seen = true
            var line_no = f.line - 1
            for (line in split(f.body, "\n")) {
                line_no++
                if (find(line, "rdec_dn_own_all(") >= 0) {
                    moved = true
                    continue
                }
                let ret = return_value_of(line)
                if (moved && !empty(ret) && ret != "true") {
                    gate_finding(RESIDENT_FILE, line_no, "the resident verify seat declines (`return {ret}`) after rdec_dn_own_all moved the session's recurrent state to the device - the CPU verify then reads a session whose state the device holds; move the check above the call (ARCHITECTURE_GPU_VULKAN_MTP.md#resident-verify-rollback)")
                }
            }
        }
        if (!seen) {
            gate_finding(RESIDENT_FILE, "no `def private {name}(` - the check that every verify-seat decline sits above the recurrent state's move to the device (ARCHITECTURE_GPU_VULKAN_MTP.md#resident-verify-rollback) has no seat body to walk; keep the spelling or move this check with it")
        }
    }
    if (!moved) {
        gate_finding(RESIDENT_FILE, "no `rdec_dn_own_all(` call inside the resident verify seat - the check that every verify-seat decline sits above the recurrent state's move to the device (ARCHITECTURE_GPU_VULKAN_MTP.md#resident-verify-rollback) has no state move to order against; keep the spelling or move this check with it")
    }
}

//! the capacity field a line assigns a non-zero value through a record (`r.cap_rows = cap`), "" elsewhere
def private cap_assign_field(line : string) : string {
    let s = strip(line)
    let eq = find(s, " = ")
    return "" if (eq < 0)
    let lhs = slice(s, 0, eq)
    let dot = rfind(lhs, ".")
    return "" if (dot < 0 || find(lhs, " ") >= 0 || find(lhs, "[") >= 0)
    let field = slice(lhs, dot + 1)
    let is_cap = (field == "cap" || field |> starts_with("cap_") || field |> ends_with("_cap") || find(field, "_cap_") >= 0)
    return "" if (!is_cap || field |> ends_with("_dev"))
    let rhs = strip(slice(s, eq + 3))
    return (rhs == "0l" || rhs == "0") ? "" : field
}

def private check_vk_scratch_cap_order {
    var inscope files : array<string>
    collect_das_files(ENGINE_DIR, files, true)
    for (path in files) {
        continue if (!(base_name(path) |> starts_with("dasllama_vulkan")))
        var inscope fns <- function_bodies(path) $(_header) => true
        for (f in fns) {
            var cap_field = ""
            var line_no = f.line - 1
            for (line in split(f.body, "\n")) {
                line_no++
                if (empty(cap_field)) {
                    cap_field = cap_assign_field(line)
                } elif (find(line, "vk_range_ok(") >= 0) {
                    gate_finding(path, line_no, "`{cap_field}` is assigned above this `vk_range_ok` check - a scratch builder that returns on the range check after setting its capacity leaves the capacity claiming buffers its release destroyed, and the next call within it skips the rebuild and dispatches into freed memory; assign the capacity after the range check (ARCHITECTURE_GPU_TOWER_VULKAN.md#vk-tower-encode-chains)")
                    break
                }
            }
        }
    }
}

// A `[test]` file that requires the engine lives in tests/ - unless it requires a file of its
// own folder by bare same-dir name (a bench self-check beside its subject), which tests/REVIEW.md's
// out-of-folder ledger owns instead.
def private check_test_file_placement {
    var inscope files : array<string>
    dir("modules/dasLLAMA") $(name) {
        return if (name == "." || name == ".." || name == "tests" || name |> starts_with(".") || name |> starts_with("_"))
        let full = "modules/dasLLAMA/{name}"
        let st = stat(full)
        return if (!st.is_valid)
        if (st.is_dir) {
            collect_das_files(full, files, true)
        } elif (name |> ends_with(".das")) {
            files |> push(full)
        }
    }
    files |> sort()
    for (path in files) {
        continue if (find(strip_line_comments(read_lf(path)), "[test]") < 0)
        var requires_engine = false
        var requires_sibling = false
        let folder = dir_name(path)
        for (modpath in das_requires(path)) {
            if (modpath |> starts_with("dasllama/")) {
                requires_engine = true
            } elif (find(modpath, "/") < 0 && fexist("{folder}/{modpath}.das")) {
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
    return if (!fexist(UNICODE_FILE))   //! a fixture tree without the tokenizer has no tables to stamp
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

let private GEMM_GEN_FILES = [
    "modules/dasLLAMA/dasllama/dasllama_gemm_gen.das",
    "modules/dasLLAMA/dasllama/dasllama_gemm_schema.das"
]
let private GEMM_GEN_STAMP_HASH = 0xed575c7c8876ebdeul

[arch(at = "ARCHITECTURE_ENGINE_FORMATS.md#cpu-kernel-tiers")]
def private check_gemm_gen_stamp {
    var h = 0xcbf29ce484222325ul
    for (path in GEMM_GEN_FILES) {
        return if (!fexist(path))   //! a fixture tree without the generated tier has no generator to stamp
        h = fnv1a("{base_name(path)}\n", h)
        for (line in split(strip_line_comments(read_lf(path)), "\n")) {
            let code = strip(line)
            continue if (empty(code))   //! a comment line leaves an empty one behind, which is no code
            h = fnv1a("{code}\n", h)
        }
    }
    if (h != GEMM_GEN_STAMP_HASH) {
        gate_finding(GEMM_GEN_FILES[0],
            "the generated GEMM tier's generator sources changed (dasllama_gemm_gen.das, dasllama_gemm_schema.das - ARCHITECTURE_ENGINE_FORMATS.md#cpu-kernel-tiers): a generator's text is outside the JIT cache key, so `.cache/daslang` serves the kernels the old text emitted - bump LLVM_JIT_CODEGEN_VERSION in modules/dasLLVM/daslib/llvm_jit_plan.das unless the emitted code is the same for every perm (the bump moves LLVM_JIT_EMITTER_HASH, which tests-cpp/small/test_jit_emitter_pin.cpp prints), then re-stamp GEMM_GEN_STAMP_HASH = {h}ul in modules/dasLLAMA/REVIEW.das")
    }
}

// One device and queue per GPU family, from its _common file's init. The exceptions the rule
// names: dasllama_metal_gemm.das owns its device (ARCHITECTURE_GPU.md#gpu-backends), and the three
// tuner race entries - the tensor crowns, the decode crowns, the split-K knees - build theirs because the
// tuner and a fat exe's first start race them before any driver inits.
let private DEVICE_CREATION_FILES <- {"dasllama_metal_common.das", "dasllama_vulkan_common.das", "dasllama_cuda_common.das", "dasllama_metal_gemm.das"}
let private DEVICE_CREATION_FUNCTIONS <- {"metal_tensor_race", "metal_tensor_race_decode", "metal_split_knee_race"}
let private DEVICE_CREATION_CALLS <- ["metal_create_system_default_device(", "metal_new_command_queue(", "create_device_storage_8_16_int_dot",
    "cuda_create_context(", "cuda_stream_create("]

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
            gate_finding(path, f.line, "`{fname}` creates a GPU device or queue of its own - a GPU family shares the one device and queue its _common file's init made; a tuner race entry that must run before the driver inits is named in REVIEW.das's DEVICE_CREATION_FUNCTIONS with its reason (ARCHITECTURE_GPU.md#gpu-backends)")
        }
    }
}


//! every file under `folder` ending in `suffix` (a rule document or a source), build trees and `.`/`_` names skipped
def private collect_files_ending(folder, suffix : string; var out : array<string>) {
    dir(folder) $(fname) {
        return if (fname == "." || fname == ".." || fname |> starts_with(".") || fname |> starts_with("_"))
        let p = "{folder}/{fname}"
        let st = stat(p)
        if (st.is_valid && st.is_dir) {
            return if (fexist("{p}/CMakeCache.txt"))
            collect_files_ending(p, suffix, out)
        } elif (ends_with(fname, suffix)) {
            out |> push(p)
        }
    }
}

//! the `.md` name the word before `at` on `line` spells (backticks and brackets stripped), "" when none
def private md_name_before(line : string; at : int) : string {
    var tok = strip(slice(line, 0, at))
    let sp = rfind(tok, " ")
    tok = sp >= 0 ? slice(tok, sp + 1) : tok
    tok = tok |> replace("`", "") |> replace("(", "") |> replace("'s", "")
    return find(tok, ".md") >= 0 ? tok : ""
}

//! a `sec.N` pointer at one of this module's documents names a number no heading carries (`ARCHITECTURE_COMMON.md`); a
//! ledger's own numbered sections, a HOW_TO's steps and another folder's still-numbered document stay citable by number
def private check_arch_cites_by_name {
    var docs : array<string>
    collect_files_ending("modules/dasLLAMA", ".md", docs)
    var sources : array<string>
    collect_das_files("modules/dasLLAMA", sources, true)
    for (path in docs) {
        let base = base_name(path)
        continue if (!(base |> starts_with("ARCHITECTURE")) && !(base |> starts_with("REVIEW")))
        var line_no = 0
        for (line in split(read_lf(path), "\n")) {
            line_no++
            peek_data(line) $(d) {
                var from = 0
                while (true) {
                    let at = find(line, "sec.", from)
                    break if (at < 0)
                    from = at + 4
                    continue if (at + 4 >= length(d) || !is_number(int(d[at + 4])))
                    let named = md_name_before(line, at)
                    continue if (find(named, "/") >= 0 || named |> starts_with("followup_") || named |> starts_with("BRINGUP") || named |> starts_with("HOW_TO"))
                    gate_finding(path, line_no, "cites an architecture section by number - the set is cited by `<doc>.md#<anchor>` (ARCHITECTURE_COMMON.md); name the heading's anchor")
                    break
                }
                //! the bare form: `ARCHITECTURE_X.md` 2.2b - a document name, a space and a section number with no prefix
                var from2 = 0
                while (true) {
                    let tick = find(line, ".md` ", from2)
                    break if (tick < 0)
                    from2 = tick + 5
                    continue if (tick + 5 >= length(d) || !is_number(int(d[tick + 5])) || find(md_name_before(line, tick + 4), "ARCHITECTURE") < 0)
                    gate_finding(path, line_no, "cites an architecture section by number - the set is cited by `<doc>.md#<anchor>` (ARCHITECTURE_COMMON.md); name the heading's anchor")
                    break
                }
            }
        }
    }
    for (path in sources) {
        var line_no = 0
        for (line in split(read_lf(path), "\n")) {
            line_no++
            let arch = find(line, "ARCHITECTURE")
            continue if (arch < 0)
            let at = find(line, "sec.", arch)
            continue if (at < 0 || at - arch > 60)
            var numbered = false
            peek_data(line) $(d) {
                numbered = at + 4 < length(d) && is_number(int(d[at + 4]))
            }
            continue if (!numbered)
            gate_finding(path, line_no, "cites an architecture section by number - the set is cited by `<doc>.md#<anchor>` (ARCHITECTURE_COMMON.md); name the heading's anchor")
        }
    }
}

let private ENGINE_DIR = "modules/dasLLAMA/dasllama"
let private TOWER_FILE = "modules/dasLLAMA/dasllama/dasllama_tower.das"
let private TTS_FAMILY_FILES <- ["dasllama_kitten", "dasllama_kokoro", "dasllama_pocket", "dasllama_styletts2"]

//! the module names a file's `require` lines name, the path form folded to its last segment
def private required_modules(path : string) : array<string> {
    var out : array<string>
    for (line in split(strip_line_comments(read_lf(path)), "\n")) {
        continue if (!(line |> starts_with("require ")))
        var rest = strip(slice(line, 8))
        let sp = find(rest, " ")
        rest = sp >= 0 ? slice(rest, 0, sp) : rest
        let slash = rfind(rest, "/")
        out |> push(slash >= 0 ? slice(rest, slash + 1) : rest)
    }
    return <- out
}

let private TESTS_DIR = "modules/dasLLAMA/tests"

//! every file dastest runs - one holding a `[test]` function - declares `options persistent_heap`: a dastest
//! program compiles on the linear heap without it, where a delete frees nothing until the context ends, so a
//! cell's deleted carriers stay resident across the file
def private check_test_heap_model {
    var files : array<string>
    collect_das_files(TESTS_DIR, files)
    for (path in files) {
        var declared = false
        var holds_test = false
        for (line in split(fread(path), "\n")) {
            if (starts_with(line, "options persistent_heap")) {
                declared = true
            } elif (starts_with(line, "[test]") || starts_with(line, "[test,") || starts_with(line, "[test(")) {
                holds_test = true
            }
        }
        continue if (!holds_test)
        if (!declared) {
            gate_finding(path, 1, "a dastest root with no `options persistent_heap` - the linear heap frees nothing a cell deletes until the context ends, so every model a cell loads stays resident across the file; add the line beside `options gen2`")
        }
    }
}

[arch(at = "ARCHITECTURE_MEDIA.md#tower-audio-charters")]
def private check_require_bans {
    if (fexist(TOWER_FILE)) {
        var banned <- { "dasllama_audio", "dasllama_vision" }
        for (fam in carried_families(AUDIO_CARRIER, "AudioKind")) {
            banned |> insert(fam)
        }
        for (fam in carried_families(VISION_CARRIER, "VisionKind")) {
            banned |> insert(fam)
        }
        for (fam in TTS_FAMILY_FILES) {
            banned |> insert(fam)
        }
        for (m in required_modules(TOWER_FILE)) {
            if (key_exists(banned, m)) {
                gate_finding(TOWER_FILE, "requires `{m}` - the tower helper file requires no carrier and no family file (ARCHITECTURE_MEDIA.md#tower-audio-charters); the shared shape lands in dasllama_asr_types.das, the family-specific helper in the family's file")
            }
        }
    }
    var files : array<string>
    collect_das_files(ENGINE_DIR, files, true)
    for (path in files) {
        let base = base_name(path)
        for (m in required_modules(path)) {
            if (m == "audio" && base != "dasllama_audio_io.das") {
                gate_finding(path, "requires `audio` - only dasllama_audio_io.das decodes audio containers (ARCHITECTURE_MEDIA.md#tower-audio-charters); decode through that file")
            } elif (m == "stbimage" && base != "dasllama_vision_io.das") {
                gate_finding(path, "requires `stbimage` - only dasllama_vision_io.das decodes images (ARCHITECTURE_MEDIA.md#tower-audio-charters); decode through that file")
            }
        }
    }
}

let private SHARED_KERNEL_FILES <- ["modules/dasLLAMA/dasllama/dasllama_gpu_kernels_discrete.das",
    "modules/dasLLAMA/dasllama/dasllama_gpu_kernels_common.das"]
let private CUDA_CLASSES_FILE = "modules/dasLLAMA/dasllama/dasllama_cuda_classes.das"
let private METAL_KERNELS_FILE = "modules/dasLLAMA/dasllama/dasllama_metal_kernels.das"

//! the discrete templates one home stamps - the row of
//! followup_general.md naming the stamps that wait on their families
var private DISCRETE_ONE_HOME_STAMPS : table<string; void>   //! empty: every discrete template is stamped by both homes; a one-home stamp lands here with its followup_general.md row

//! `class [template] X : Y` lines of `path`: X's base, whether X is a
//! template, and the home or shared file X lives in
def private scan_class_decls(path, home : string; var base : table<string; string>;
                             var templates, owner : table<string; string>) {
    for (line in split(read_lf(path), "\n")) {
        let hdr = class_header(line)
        continue if (empty(hdr.name))
        owner[hdr.name] = home
        if (hdr.is_template) {
            templates[hdr.name] = home
        }
        if (hdr.has_base) {
            base[hdr.name] = hdr.base
        }
    }
}

[arch(at = "ARCHITECTURE_GPU.md#gpu-backends")]
def private check_shared_template_homes {
    var base, templates, owner : table<string; string>
    for (path in SHARED_KERNEL_FILES) {
        scan_class_decls(path, base_name(path), base, templates, owner)
    }
    dir(ENGINE_DIR) $(name) {
        return if (!ends_with(name, ".das"))
        for (home in ["metal", "vulkan", "cuda"]) {
            if (name |> starts_with("dasllama_{home}_")) {
                scan_class_decls("{ENGINE_DIR}/{name}", home, base, templates, owner)
            }
        }
    }
    //! "<shared template>|<home>" for every shared template a home's class
    //! derives, through any chain of intermediates
    var stamped, seen : table<string>
    for (name, home in keys(owner), values(owner)) {
        continue if (home |> ends_with(".das"))
        var c = base?[name] ?? ""
        clear(seen)
        while (!empty(c) && !key_exists(seen, c)) {
            seen |> insert(c)
            if ((owner?[c] ?? "") |> ends_with(".das")) {
                stamped |> insert("{c}|{home}")
            }
            c = base?[c] ?? ""
        }
    }
    let discrete = base_name(SHARED_KERNEL_FILES[0])
    for (t, file in keys(templates), values(templates)) {
        continue if (!(file |> ends_with(".das")))
        let path = "{ENGINE_DIR}/{file}"
        if (file == discrete) {
            let vk = key_exists(stamped, "{t}|vulkan")
            let cu = key_exists(stamped, "{t}|cuda")
            if (key_exists(DISCRETE_ONE_HOME_STAMPS, t)) {
                if (vk && cu) {
                    gate_finding(path, ("`{t}` is stamped by both discrete homes now " +
                        "(ARCHITECTURE_GPU_SHARED_KERNELS.md#gpu-shared-kernels) - drop it from " +
                        "DISCRETE_ONE_HOME_STAMPS in REVIEW.das and from followup_general.md's row " +
                        "of stamps that wait on their families"))
                }
            } elif (!vk || !cu) {
                let which = vk ? "only the Vulkan home" : (cu ? "only the CUDA home" : "no home")
                gate_finding(path, ("`{t}` is a discrete template that {which} stamps - the discrete " +
                    "file holds the bodies both discrete homes stamp (ARCHITECTURE_GPU.md#gpu-backends); " +
                    "a body one home runs lives in that home's classes file, and a stamp the other " +
                    "home's family brings later is a followup_general.md row plus a " +
                    "DISCRETE_ONE_HOME_STAMPS name in REVIEW.das"))
            }
        } elif (!key_exists(stamped, "{t}|metal") || !key_exists(stamped, "{t}|vulkan")) {
            gate_finding(path, ("`{t}` is a common template that only one of the Metal and Vulkan " +
                "homes stamps - the common file holds the kernels both stamp " +
                "(ARCHITECTURE_GPU_SHARED_KERNELS.md#gpu-shared-kernels); a body one home runs " +
                "lives in that home's kernel file"))
        }
    }
}

[arch(at = "ARCHITECTURE_GPU_SHARED_KERNELS.md#gpu-shared-kernels")]
def private check_shared_file_spelling {
    for (path in [SHARED_KERNEL_FILES[0], SHARED_KERNEL_FILES[1], CUDA_CLASSES_FILE]) {
        var markers = false
        for (m in das_requires(path)) {
            continue if (!(m |> starts_with("spirv/")))
            if (m == "spirv/spirv_markers") {
                markers = true
            } else {
                gate_finding(path, ("requires `{m}` - a shared kernel file and the CUDA home's classes " +
                    "file read the marker annotations from the emitter-free `spirv/spirv_markers`, " +
                    "never the emitter behind `spirv_shader` (ARCHITECTURE_GPU.md#gpu-backends)"))
            }
        }
        if (!markers) {
            gate_finding(path, ("does not require `spirv/spirv_markers` - the marker annotations' " +
                "home (ARCHITECTURE_GPU.md#gpu-backends)"))
        }
    }
    var files : array<string>
    collect_das_files(ENGINE_DIR, files, true)
    for (path in files) {
        let base = base_name(path)
        continue if (base == base_name(SHARED_KERNEL_FILES[0]) || base == base_name(METAL_KERNELS_FILE))
        var line_no = 0
        for (line in split(read_lf(path), "\n")) {
            line_no++
            continue if (!(line |> starts_with("def gk_subgroup_")) &&
                !(line |> starts_with("def public gk_subgroup_")) &&
                !(line |> starts_with("def private gk_subgroup_")))
            gate_finding(path, line_no, ("defines a `gk_subgroup_*` lane primitive - the discrete file " +
                "carries the one copy in SPIR-V's spelling, which both discrete emitters lower, and a " +
                "home defines its own only where its spelling differs, Metal's `simd_*` " +
                "(ARCHITECTURE_GPU_SHARED_KERNELS.md#gpu-shared-kernels)"))
        }
    }
}

[arch(at = "ARCHITECTURE_GPU.md#gpu-backends")]
def private check_shared_file_cites {
    for (path in SHARED_KERNEL_FILES) {
        var line_no = 0
        for (line in split(read_lf(path), "\n")) {
            line_no++
            continue if (find(line, "arch(at") < 0)
            var from = find(line, "ARCHITECTURE_")
            while (from >= 0) {
                let end = find(line, ".md", from)
                break if (end < 0)
                let doc = slice(line, from, end)
                from = find(line, "ARCHITECTURE_", end)
                for (home in ["VULKAN", "VK", "METAL", "CUDA"]) {
                    continue if (find(doc, home) < 0)
                    gate_finding(path, line_no, ("cites `{doc}.md`, a home's document - a shared kernel file " +
                        "cites ARCHITECTURE_GPU.md or an ARCHITECTURE_GPU_SHARED_*.md companion, since its " +
                        "mechanism is neither home's (ARCHITECTURE_GPU.md#gpu-backends); move the section " +
                        "into a shared companion, or cite the shared section that carries the mechanism"))
                    break
                }
            }
        }
    }
}

[arch(at = "ARCHITECTURE_ENGINE.md#instrumentation-and-support")]
def private check_harness_dashv {
    var files : array<string>
    collect_das_files("modules/dasLLAMA/harness", files, true)
    for (path in files) {
        var speaks_http = false
        for (m in required_modules(path)) {
            speaks_http = speaks_http || m |> starts_with("dashv")
        }
        continue if (!speaks_http)
        if (find(read_lf(path), "dasllama.io") >= 0) {
            gate_finding(path, "requires dashv and names dasllama.io - a harness tool reaches the sidecar exchange through dasllama/dasllama_exchange, never with its own HTTP call (ARCHITECTURE_ENGINE.md#instrumentation-and-support)")
        }
    }
}

//! one `class` a kernel file declares: its base, the constants it sets or declares, and the attribute and `//!` lines above it
struct private StampDecl {
    base : string
    line : int
    consts : table<string; string>
    doc : string
    is_template : bool
    dispatched : bool
}

//! the value text of an `override NAME = v` / `@template_constant NAME : T = v` line, its comment dropped
def private const_value(s : string) : string {
    let eq = find(s, "=")
    let cm = find(s, "//")
    return strip(slice(s, eq + 1, cm >= 0 ? cm : length(s)))
}

//! the constant a class body line sets or declares, "" on any other line
def private const_name(s : string) : string {
    if (s |> starts_with("override ")) {
        return ident_prefix(slice(s, length("override ")))
    } elif (s |> starts_with("@template_constant ")) {
        return ident_prefix(slice(s, length("@template_constant ")))
    }
    return ""
}

def private stamp_decl_at(lines : array<string>; i : int; var out : table<string; StampDecl>) {
    let tmpl = lines[i] |> starts_with("class template ")
    let rest = slice(lines[i], tmpl ? length("class template ") : length("class "))
    let colon = find(rest, ":")
    var decl <- StampDecl(line = i + 1, is_template = tmpl, base = colon >= 0 ? ident_prefix(strip(slice(rest, colon + 1))) : "")
    var j = i + 1
    while (j < length(lines) && !(lines[j] |> starts_with("}"))) {
        let s = strip(lines[j])
        let cn = const_name(s)
        if (!empty(cn) && find(s, "=") >= 0) {
            decl.consts[cn] = const_value(s)
        }
        j++
    }
    var k = i - 1
    while (k >= 0 && (lines[k] |> starts_with("[") || lines[k] |> starts_with("    "))) {
        decl.dispatched ||= find(lines[k], "_dispatch(") >= 0
        k--
    }
    while (k >= 0 && lines[k] |> starts_with("//!")) {
        decl.doc = "{lines[k]} {decl.doc}"
        k--
    }
    out |> emplace(ident_prefix(rest), decl)
}

def private stamp_decls(path : string; var out : table<string; StampDecl>) {
    var inscope lines <- split(read_lf(path), "\n")
    for (i in range(length(lines))) {
        if (lines[i] |> starts_with("class ")) {
            stamp_decl_at(lines, i, out)
        }
    }
}

//! the base `n` names in `local`, else in `shipped`; "" past the chain's root
def private stamp_base(n : string; local, shipped : table<string; StampDecl>) : string {
    var b = ""
    let found = get(local, n) $(d) {
        b = d.base
    }
    if (!found) {
        get(shipped, n) $(d) {
            b = d.base
        }
    }
    return b
}

//! `name`'s class chain, itself first, through `local` then `shipped`
def private stamp_chain(name : string; local, shipped : table<string; StampDecl>) : array<string> {
    var out : array<string>
    var n = name
    while (!empty(n) && length(out) < 64) {
        out |> push(n)
        n = stamp_base(n, local, shipped)
    }
    return <- out
}

//! the constants `name` compiles with: every class of its chain's settings, the nearest one winning
def private stamp_consts(name : string; local, shipped : table<string; StampDecl>) : table<string; string> {
    var out : table<string; string>
    var inscope chain <- stamp_chain(name, local, shipped)
    for (i in range(length(chain))) {
        let n = chain[length(chain) - 1 - i]
        let found = get(local, n) $(d) {
            for (k, v in keys(d.consts), values(d.consts)) {
                out[k] = v
            }
        }
        if (!found) {
            get(shipped, n) $(d) {
                for (k, v in keys(d.consts), values(d.consts)) {
                    out[k] = v
                }
            }
        }
    }
    return <- out
}

//! true when `shipped` holds `n` as a dispatched class
def private shipped_stamp(n : string; shipped : table<string; StampDecl>) : bool {
    var hit = false
    get(shipped, n) $(d) {
        hit = d.dispatched
    }
    return hit
}

//! the first backticked name of a race stamp's `//!` lines that a shipped dispatched class carries
def private raced_stamp(doc : string; shipped : table<string; StampDecl>) : string {
    var inscope parts <- split(doc, "`")
    for (i in range(length(parts))) {
        continue if (i % 2 == 0)
        let n = ident_prefix(parts[i])
        return n if (!empty(n) && shipped_stamp(n, shipped))
    }
    return ""
}

def private consts_apart(a, b : table<string; string>) : array<string> {
    var out <- [for (k, v in keys(a), values(a)); k; where(b?[k] ?? "") != v]
    for (k in keys(b)) {
        if (!key_exists(a, k)) {
            out |> push(k)
        }
    }
    out |> sort()
    return <- out
}

def private check_bench_stamps_in(path : string; shipped : table<string; StampDecl>) {
    var inscope local : table<string; StampDecl>
    stamp_decls(path, local)
    for (n, d in keys(local), values(local)) {
        continue if (d.is_template || !d.dispatched)
        let raced = raced_stamp(d.doc, shipped)
        continue if (empty(raced))
        var inscope chain <- stamp_chain(n, local, shipped)
        let tmpl = stamp_base(raced, shipped, shipped)
        if (find_index(chain, tmpl) < 0) {
            gate_finding(path, d.line, "race stamp `{n}` names `{raced}` but does not stamp its template `{tmpl}` - a race stamp is the shipped stamp's template with the raced constant moved, or the race times a kernel that never ships (ARCHITECTURE_MEASUREMENT_KERNEL_RACE.md#kernel-race-fidelity)")
            continue
        }
        var inscope mine <- stamp_consts(n, local, shipped)
        var inscope theirs <- stamp_consts(raced, shipped, shipped)
        var inscope apart <- consts_apart(mine, theirs)
        if (length(apart) > 1) {
            gate_finding(path, d.line, "race stamp `{n}` names `{raced}` and differs from it in {length(apart)} constants ({join(apart, ", ")}) - a race stamp moves the one constant the race picks and keeps every other the shipped stamp's, or the race times a kernel that never ships (ARCHITECTURE_MEASUREMENT_KERNEL_RACE.md#kernel-race-fidelity)")
        }
    }
}

//! a race stamp - a dispatched class under benchmarks/, harness/ or performance/ whose `//!` names a shipped stamp - is that stamp
//! with one constant moved
[arch(at = "ARCHITECTURE_MEASUREMENT_KERNEL_RACE.md#kernel-race-fidelity")]
def private check_bench_stamps {
    var inscope shipped : table<string; StampDecl>
    var inscope engine : array<string>
    collect_das_files(DASLLAMA_DIR, engine)
    for (p in engine) {
        stamp_decls(p, shipped)
    }
    var inscope files : array<string>
    for (folder in ["modules/dasLLAMA/benchmarks", "modules/dasLLAMA/harness", "modules/dasLLAMA/performance"]) {
        collect_das_files(folder, files, true)
    }
    for (p in files) {
        check_bench_stamps_in(p, shipped)
    }
}

//! the index of the `)` closing the `(` at `open`, -1 when the line ends first
def private paren_close(d; open : int) : int {
    var depth = 0
    for (i in range(open, length(d))) {
        let ch = int(d[i])
        if (ch == '(' || ch == '[') {
            depth++
        } elif (ch == ')' || ch == ']') {
            depth--
            return i if (depth == 0)
        }
    }
    return -1
}

//! true when `inner` holds a ` - ` outside every bracket: a difference
def private top_level_minus(inner : string) : bool {
    var depth = 0
    var found = false
    peek_data(inner) $(d) {
        for (i in range(length(d))) {
            let ch = int(d[i])
            if (ch == '(' || ch == '[') {
                depth++
            } elif (ch == ')' || ch == ']') {
                depth--
            } elif (ch == '-' && depth == 0 && i > 0 && int(d[i - 1]) == ' ' && i + 1 < length(d) && int(d[i + 1]) == ' ') {
                found = true
            }
        }
    }
    return found
}

//! true when `line` keeps a running largest difference as `max(m, abs(a - b))`
def private running_abs_diff_max(line : string) : bool {
    var hit = false
    peek_data(line) $(d) {
        var at = find(d, "max(")
        while (at >= 0 && !hit) {
            let from = at + length("max(")
            let name = ident_prefix(slice(d, from))
            let tail = from + length(name)
            if ((at == 0 || !is_ident_byte(int(d[at - 1]))) && !empty(name) && slice(d, tail, tail + 6) == ", abs(") {
                let open = tail + 5
                let close = paren_close(d, open)
                hit = close > open && top_level_minus(slice(d, open + 1, close))
            }
            at = find(d, "max(", from)
        }
    }
    return hit
}

//! a running largest difference over two float rows in a test reads a NaN as infinite: `max` drops a NaN a later finite element follows
[arch(at = "ARCHITECTURE_ENGINE.md#test-folder-gates")]
def private check_running_max_nan {
    var inscope files : array<string>
    collect_das_files(TESTS_DIR, files)
    for (path in files) {
        var line_no = 0
        for (line in split(strip_line_comments(read_lf(path)), "\n")) {
            line_no++
            if (running_abs_diff_max(line)) {
                gate_finding(path, line_no, "a running largest difference `max(m, abs(a - b))` - `max` drops a NaN that a later finite element follows, so an output holding a NaN passes the bar; take `nan_wide_diff(a, b)` (_compares.das, a NaN on either side infinite) or route the compare through `logits_maxdiff` / `check` (ARCHITECTURE_ENGINE.md#test-folder-gates)")
            }
        }
    }
}

let private COVERAGE_FILE = "modules/dasLLAMA/tests/test_kernel_coverage.das"
let private PINNED_DOC = "modules/dasLLAMA/tests/REVIEW_PINNED_GATES.md"
let private CENSUS_TABLES = ["let private CENSUS_NEVER_DISPATCHED <- [", "let private VK_CENSUS_NEVER_DISPATCHED <- ["]

//! the backticked names of the pinned set's entries, and the files whose entry pins them whole (an entry naming the file and no cell)
def private pinned_set(var names : table<string>; var whole_files : table<string>) {
    for (line in split(read_lf(PINNED_DOC), "\n")) {
        var inscope parts <- split(line, "`")
        for (i in range(length(parts))) {
            if (i % 2 == 1) {
                names |> insert(parts[i])
            }
        }
        if (line |> starts_with("- `") && length(parts) >= 3 && parts[1] |> ends_with(".das") && parts[2] |> starts_with(" must")) {
            whole_files |> insert(parts[1])
        }
    }
}

//! the evidence names a census table's comment lines cite: the kernel-unit cells (`test_vk*`, `test_metal_*`) and the gates (`*_gate`)
def private census_evidence(var out : table<string; int>) {
    let text = read_lf(COVERAGE_FILE)
    var inscope lines <- split(text, "\n")
    for (marker in CENSUS_TABLES) {
        let from = find_line(text, marker)
        continue if (from <= 0)
        var i = from
        while (i < length(lines) && find(lines[i - 1], "\"]") < 0) {
            let s = strip(lines[i - 1])
            if (s |> starts_with("//")) {
                for (tok in census_evidence_tokens(s)) {
                    if (!key_exists(out, tok)) {
                        out[tok] = i
                    }
                }
            }
            i++
        }
    }
}

def private census_evidence_tokens(s : string) : array<string> {
    var out : array<string>
    peek_data(s) $(d) {
        var i = 0
        while (i < length(d)) {
            if (!is_ident_byte(int(d[i])) || (i > 0 && is_ident_byte(int(d[i - 1])))) {
                i++
                continue
            }
            let tok = ident_prefix(slice(d, i))
            i += max(length(tok), 1)
            continue if (i < length(d) && int(d[i]) == '.')
            if (tok |> starts_with("test_vk") || tok |> starts_with("test_metal_") || tok |> ends_with("_gate")) {
                out |> push(tok)
            }
        }
    }
    return <- out
}

//! every file under tests/ by its stem, then every `def` there by its name, each to the file that holds it
def private tests_homes : table<string; string> {
    var out : table<string; string>
    var inscope files : array<string>
    collect_das_files(TESTS_DIR, files)
    for (p in files) {
        out[slice(base_name(p), 0, length(base_name(p)) - length(".das"))] = p
    }
    for (p in files) {
        for (line in split(read_lf(p), "\n")) {
            if (line |> starts_with("def ")) {
                let n = def_name_of(line)
                if (!key_exists(out, n)) {
                    out[n] = p
                }
            }
        }
    }
    return <- out
}

//! true when a cell the pinned set names in `home` calls `name` - a gate helper a pinned cell runs
def private called_by_pinned_cell(name, home : string; pinned : table<string>) : bool {
    var inscope fns <- function_bodies(home) $(header) {
        return header |> starts_with("def test_") && key_exists(pinned, def_name_of(header))
    }
    for (f in fns) {
        return true if (find(f.body, "{name}(") >= 0)
    }
    return false
}

//! every kernel-unit cell a census table names as a blind spot's evidence is in the pinned set
[arch(at = "ARCHITECTURE_ENGINE.md#test-folder-gates")]
def private check_census_evidence_pinned {
    return if (!fexist(COVERAGE_FILE) || !fexist(PINNED_DOC))
    var inscope pinned, whole_files : table<string>
    pinned_set(pinned, whole_files)
    var inscope evidence : table<string; int>
    census_evidence(evidence)
    var inscope homes <- tests_homes()
    for (name, line in keys(evidence), values(evidence)) {
        continue if (key_exists(pinned, name))
        let home = homes?[name] ?? ""
        if (empty(home)) {
            gate_finding(COVERAGE_FILE, line, "a census table cites `{name}` as evidence and no file under tests/ defines it - name the cell that dispatches the kernel (ARCHITECTURE_ENGINE.md#test-folder-gates)")
        } elif (!key_exists(whole_files, base_name(home)) && !called_by_pinned_cell(name, home, pinned)) {
            gate_finding(COVERAGE_FILE, line, "a census table cites `{name}` ({base_name(home)}) as a blind spot's evidence, and the pinned set does not hold it - a cell a checked-in table names as its evidence is pinned, by a named-cell entry or its file's entry, in tests/REVIEW_PINNED_GATES.md (ARCHITECTURE_ENGINE.md#test-folder-gates)")
        }
    }
}

let private LOAD_FILE = "modules/dasLLAMA/dasllama/dasllama_load.das"

//! the top-level argument count of the call whose `(` sits at `open`
def private call_arg_count(line : string; open : int) : int {
    var n = 1
    peek_data(line) $(d) {
        let close = paren_close(d, open)
        var inscope cuts <- arg_commas(d, open + 1, close < 0 ? length(d) : close)
        n += length(cuts)
    }
    return n
}

//! every `load_big` call names the plane's format - a call with none converts the plane to one form whatever its disk type
[arch(at = "ARCHITECTURE_ENGINE_FORMATS.md#the-load-and-image-rail")]
def private check_load_big_format {
    return if (!fexist(LOAD_FILE))
    var line_no = 0
    for (line in split(strip_line_comments(read_lf(LOAD_FILE)), "\n")) {
        line_no++
        let at = find(line, "load_big(")
        continue if (at < 0 || find(line, "def ") >= 0)
        if (call_arg_count(line, at + length("load_big")) < 9) {
            gate_finding(LOAD_FILE, line_no, "a `load_big` call with no format converts the plane to the q8 rail whatever its disk type, and no report names the conversion - pass the plane's tag (`kq_tag` / `kq_fmt_of`), or the `KqFmt` member the conversion serves (ARCHITECTURE_ENGINE_FORMATS.md#the-load-and-image-rail)")
        }
    }
}

//! every `RdecPass` value has its own sentence in `rdec_pass_words`, the reason a status page prints beside the count
[arch(at = "ARCHITECTURE_GPU_ASYMMETRIES.md#gpu-asymmetries")]
def private check_rdec_pass_words {
    return if (!fexist(RESIDENT_FILE))
    let text = read_lf(RESIDENT_FILE)
    let enum_line = find_line(text, "enum private RdecPass \{")
    var inscope words <- function_bodies(RESIDENT_FILE) $(header) {
        return header |> starts_with("def private rdec_pass_words(")
    }
    if (enum_line <= 0 || empty(words)) {
        gate_finding(RESIDENT_FILE, "no `enum private RdecPass` or no `def private rdec_pass_words(` - the check that every pass reason has a reader-facing sentence (ARCHITECTURE_GPU_ASYMMETRIES.md#gpu-asymmetries) has nothing to walk; keep the spellings or move this check with them")
        return
    }
    var inscope lines <- split(text, "\n")
    var i = enum_line
    while (i < length(lines) && !(strip(lines[i]) |> starts_with("}"))) {
        let member = ident_prefix(strip(lines[i]))
        i++
        continue if (empty(member))
        if (find(words[0].body, "RdecPass.{member})") < 0) {
            gate_finding(RESIDENT_FILE, i, "`RdecPass.{member}` has no arm in `rdec_pass_words` - a status page prints the value's machine name where it has no sentence; give it one in plain words that name no part of the engine (ARCHITECTURE_GPU_ASYMMETRIES.md#gpu-asymmetries)")
        }
    }
}

let private TESTS_DOC = "modules/dasLLAMA/tests/CLAUDE.md"

//! every whitespace-separated word of every backtick span in `text`
def private backticked_words(text : string) : table<string> {
    var out : table<string>
    var inscope parts <- split(replace(text, "\n", " "), "`")
    for (i in range(length(parts))) {
        continue if (i % 2 == 0)
        for (w in split(parts[i], " ")) {
            if (!empty(w)) {
                out |> insert(w)
            }
        }
    }
    return <- out
}

//! the arm a literal `arm_on(t, "...")` names on `line`: the whole token, or the prefix ahead of an interpolation (`mtp-ff-{tag}`)
def private arm_literals(line : string) : array<tuple<tok : string; prefix : bool>> {
    var out : array<tuple<tok : string; prefix : bool>>
    peek_data(line) $(d) {
        var at = find(d, "arm_on(")
        while (at >= 0) {
            let from = at + length("arm_on(")
            at = find(d, "arm_on(", from)
            let q = find(d, ", \"", from)
            continue if (q < 0 || (at >= 0 && q > at))
            let handle = slice(d, from, q)
            continue if (find(handle, " ") >= 0 || find(handle, "\\") >= 0)
            let end = find(d, "\"", q + 3)
            continue if (end < 0)
            let tok = slice(d, q + 3, end)
            let brace = find(tok, "\{")
            out |> push((tok = brace >= 0 ? slice(tok, 0, brace) : tok, prefix = brace >= 0))
        }
    }
    return <- out
}

//! every literal arm token a tests file filters on appears backticked in tests/CLAUDE.md
[arch(at = "ARCHITECTURE_ENGINE.md#test-folder-gates")]
def private check_arm_tokens_documented(doc_words : table<string>) {
    dir(TESTS_DIR) $(name) {
        return if (!(name |> ends_with(".das")))
        let path = "{TESTS_DIR}/{name}"
        var line_no = 0
        for (line in split(strip_line_comments(read_lf(path)), "\n")) {
            line_no++
            for (arm in arm_literals(line)) {
                var known = !arm.prefix && key_exists(doc_words, arm.tok)
                if (arm.prefix) {
                    known = empty(arm.tok)
                    for (w in keys(doc_words)) {
                        known ||= w |> starts_with(arm.tok)
                    }
                }
                if (!known) {
                    let shown = arm.prefix ? "{arm.tok}<...>" : arm.tok
                    gate_finding(path, line_no, "arm `{shown}` appears in no backtick span of tests/CLAUDE.md - an `--arm` filter token a reader cannot find is an arm nobody runs; name it under \"Arm filter mechanics\" (ARCHITECTURE_ENGINE.md#test-folder-gates)")
                }
            }
        }
    }
}

//! the folders a `test_*` / `cell_*` name in tests/CLAUDE.md resolves in: the module, and every folder the document names by a path
def private doc_name_roots(doc_words : table<string>) : array<string> {
    var roots <- ["modules/dasLLAMA"]
    for (w in keys(doc_words)) {
        continue if (find(w, "/") <= 0 || w |> starts_with(".") || !(w |> ends_with(".das")))
        let d = dir_name(w)
        if (!empty(d) && fexist(d) && find_index(roots, d) < 0) {
            roots |> push(d)
        }
    }
    return <- roots
}

//! every `.das` file name and every `def` under `roots`
def private names_under(roots : array<string>; var files, defs : table<string>) {
    for (r in roots) {
        var inscope paths : array<string>
        collect_das_files(r, paths)
        for (p in paths) {
            files |> insert(base_name(p))
            for (line in split(read_lf(p), "\n")) {
                if (line |> starts_with("def ")) {
                    defs |> insert(def_name_of(line))
                }
            }
        }
    }
}

//! the bare name a backticked word spells: punctuation trimmed, a path cut to its last segment
def private doc_word_name(w : string) : string {
    var n = length(w)
    peek_data(w) $(d) {
        while (n > 0 && find("(),;:.", slice(d, n - 1, n)) >= 0 && !(slice(d, 0, n) |> ends_with(".das"))) {
            n--
        }
    }
    return base_name(slice(w, 0, n))
}

//! every backticked `test_*` / `cell_*` name in tests/CLAUDE.md resolves to a file or a `def`
[arch(at = "ARCHITECTURE_ENGINE.md#test-folder-gates")]
def private check_doc_names_resolve(doc_words : table<string>) {
    var inscope roots <- doc_name_roots(doc_words)
    var inscope files, defs : table<string>
    names_under(roots, files, defs)
    let text = read_lf(TESTS_DOC)
    for (w in keys(doc_words)) {
        let name = doc_word_name(w)
        continue if (!(name |> starts_with("test_") || name |> starts_with("cell_")) || find(w, "*") >= 0 || find(w, "<") >= 0 || find(w, "\{") >= 0)
        let ok = name |> ends_with(".das") ? key_exists(files, name) : (key_exists(defs, name) || key_exists(files, "{name}.das"))
        if (!ok) {
            gate_finding(TESTS_DOC, find_line(text, w), "`{w}` names no file and no `def` under the module or a folder the document names by path - a stale or misspelt cell name sends the reader to nothing; rename it to the cell it means or drop it (ARCHITECTURE_ENGINE.md#test-folder-gates)")
        }
    }
}

//! tests/CLAUDE.md is the test discipline, not a ledger: it cites no `followup_*.md` item or row
[arch(at = "ARCHITECTURE_ENGINE.md#test-folder-gates")]
def private check_doc_cites_no_ledger {
    var line_no = 0
    for (line in split(read_lf(TESTS_DOC), "\n")) {
        line_no++
        if (find(line, "followup_") >= 0) {
            gate_finding(TESTS_DOC, line_no, "cites a `followup_*.md` ledger - the test discipline states what the cells hold, and a ledger item moves or closes under it; state the fact, or drop the clause (ARCHITECTURE_ENGINE.md#test-folder-gates)")
        }
    }
}

//! a cell's name with its articles and counts dropped, lowercased, singular - the form an entry is searched for
def private cell_name_key(s : string) : string {
    let k = to_lower(strip(s))
    var from = 0
    var moved = true
    while (moved) {
        moved = false
        for (w in ["the ", "and ", "plus ", "two ", "three ", "four ", "five "]) {
            if (find(k, w, from) == from) {
                from += length(w)
                moved = true
            }
        }
    }
    let to = k |> ends_with("s") ? length(k) - 1 : length(k)
    return slice(k, from, max(from, to))
}

//! the phrase naming the text ahead of position `at` in `flat` - back to the last `- `, `, `, `: `, `; ` or `(`
def private phrase_before(flat : string; at : int) : string {
    let head = strip(slice(flat, 0, at))
    var cut = -1
    for (sep in ["- ", ", ", ": ", "; ", "("]) {
        cut = max(cut, rfind(head, sep))
    }
    return cell_name_key(cut >= 0 ? slice(head, cut + 1) : head)
}

//! the regions lead's bullets: every `cell_*` a bullet names, with the bullet's head and the cell's own phrase
def private regions_cell_names(lines : array<string>; var out : table<string; array<string>>) {
    var in_lead = false
    var inscope bullet : array<string>
    for (line in lines) {
        if (line |> starts_with("`_resident_regions.das`")) {
            in_lead = true
            continue
        }
        continue if (!in_lead)
        let in_bullet = !empty(bullet) && bullet[0] |> starts_with("- ")
        if (line |> starts_with("- ") || (in_bullet && (line |> starts_with("`") || empty(line)))) {
            regions_bullet_names(join(bullet, " "), out)
            bullet |> clear()
            bullet |> push(line)
            break if (!(line |> starts_with("- ")))
        } else {
            bullet |> push(strip(line))
        }
    }
    regions_bullet_names(join(bullet, " "), out)
}

def private regions_bullet_names(bullet : string; var out : table<string; array<string>>) {
    return if (!(bullet |> starts_with("- ")))
    var head_end = length(bullet)
    for (sep in [" (", ":", ","]) {
        let at = find(bullet, sep)
        head_end = at >= 0 ? min(head_end, at) : head_end
    }
    let head = cell_name_key(slice(bullet, 2, head_end))
    peek_data(bullet) $(d) {
        var at = find(d, "`cell_")
        while (at >= 0) {
            let cell = ident_prefix(slice(d, at + 1))
            let open = rfind(slice(d, 0, at), "(")
            out[cell] |> push(head)
            if (open > 0) {
                out[cell] |> push(phrase_before(bullet, open))
            }
            at = find(d, "`cell_", at + 1)
        }
    }
}

//! each `test_gpu_resident_regions*.das` entry's text: the line naming the file and its continuation lines
def private regions_entries(lines : array<string>; var out : table<string; string>) {
    var cur = ""
    var inscope parts : array<string>
    for (i in range(length(lines) + 1)) {
        let line = i < length(lines) ? lines[i] : ""
        let tick = line |> starts_with("`") ? find(line, "`", 1) : -1
        let opens = tick > 0 && slice(line, tick, tick + 4) == "` - "
        if (!empty(cur) && !opens && !empty(line) && !(line |> starts_with("- ")) && !(line |> starts_with("#"))) {
            parts |> push(line)
            continue
        }
        if (!empty(cur)) {
            out[cur] = join(parts, " ")
        }
        parts |> clear()
        cur = opens && line |> starts_with("`test_gpu_resident_regions") ? slice(line, 1, tick) : ""
        if (!empty(cur)) {
            parts |> push(line)
        }
    }
}

//! the `cell_*` functions a file's code calls
def private called_cells(path : string) : table<string> {
    var out : table<string>
    for (line in split(strip_line_comments(read_lf(path)), "\n")) {
        peek_data(line) $(d) {
            var at = find(d, "cell_")
            while (at >= 0) {
                let cell = ident_prefix(slice(d, at))
                if ((at == 0 || !is_ident_byte(int(d[at - 1]))) && find(d, "{cell}(") >= 0) {
                    out |> insert(cell)
                }
                at = find(d, "cell_", at + 1)
            }
        }
    }
    return <- out
}

//! every `cell_*` a `test_gpu_resident_regions*.das` file calls is named in that file's tests/CLAUDE.md entry
[arch(at = "ARCHITECTURE_ENGINE.md#test-folder-gates")]
def private check_regions_entries {
    var inscope lines <- split(read_lf(TESTS_DOC), "\n")
    var inscope names : table<string; array<string>>
    regions_cell_names(lines, names)
    var inscope entries : table<string; string>
    regions_entries(lines, entries)
    dir(TESTS_DIR) $(fname) {
        return if (!(fname |> starts_with("test_gpu_resident_regions") && fname |> ends_with(".das")))
        let path = "{TESTS_DIR}/{fname}"
        let entry = entries?[fname] ?? ""
        if (empty(entry)) {
            gate_finding(TESTS_DOC, "`{fname}` has no entry - every regions file has one naming the cells it runs (ARCHITECTURE_ENGINE.md#test-folder-gates)")
            return
        }
        let lowered = to_lower(entry)
        var inscope called <- called_cells(path)
        for (cell in keys(called)) {
            var named = find(entry, cell) >= 0
            get(names, cell) $(ns) {
                for (n in ns) {
                    named ||= !empty(n) && find(lowered, n) >= 0
                }
            }
            if (!named) {
                let lead = key_exists(names, cell) ? "by the name the `_resident_regions.das` list gives it" : "and the `_resident_regions.das` list names it nowhere"
                gate_finding(TESTS_DOC, find_line(read_lf(TESTS_DOC), "`{fname}`"), "`{fname}` calls `{cell}`, and its entry does not name it {lead} - an entry lists every cell its file runs (ARCHITECTURE_ENGINE.md#test-folder-gates)")
            }
        }
    }
}

def private check_tests_doc {
    return if (!fexist(TESTS_DOC))
    var inscope doc_words <- backticked_words(read_lf(TESTS_DOC))
    check_arm_tokens_documented(doc_words)
    check_doc_names_resolve(doc_words)
    check_doc_cites_no_ledger()
    check_regions_entries()
}

// ===== the HOW_TO documents' shape (REVIEW_DOCS.md) =====

let private HOWTO_SECTION = "ARCHITECTURE_DOCS.md#howto-documents"
//! the past-tense markers, lower-cased whole words outside backtick spans - each reads as a record of what happened, never as an instruction
let private HOWTO_PAST_WORDS <- ["was", "were", "had", "previously", "formerly", "originally"]
let private HOWTO_PAST_PHRASES <- ["used to ", "turned out", "back then"]

def private outside_code_spans(line : string) : string {
    let parts <- split(line, "`")
    return join([for (p, i in parts, count()); p; where i % 2 == 0], " ")
}

def private digit_at(d; i : int) : bool => i >= 0 && i < length(d) && is_number(int(d[i]))

//! a `20YY-MM` date anywhere on the line - MM a month, 01 to 12 - digits on neither side
def private has_date(line : string) : bool {
    var hit = false
    peek_data(line) $(d) {
        for (i in range(length(d) - 6)) {
            if (int(d[i]) == '2' && int(d[i + 1]) == '0' && digit_at(d, i + 2) && digit_at(d, i + 3) && int(d[i + 4]) == '-'
                    && digit_at(d, i + 5) && digit_at(d, i + 6) && !digit_at(d, i - 1) && !digit_at(d, i + 7)) {
                let month = (int(d[i + 5]) - '0') * 10 + int(d[i + 6]) - '0'
                if (month >= 1 && month <= 12) {
                    hit = true
                    break
                }
            }
        }
    }
    return hit
}

//! a `#NNN` PR or issue number - a hash and three digits or more
def private has_pr_number(line : string) : bool {
    var hit = false
    peek_data(line) $(d) {
        for (i in range(length(d) - 3)) {
            if (int(d[i]) == '#' && digit_at(d, i + 1) && digit_at(d, i + 2) && digit_at(d, i + 3)) {
                hit = true
                break
            }
        }
    }
    return hit
}

def private past_marker(line : string) : string {
    let prose = to_lower(outside_code_spans(line))
    for (w in HOWTO_PAST_WORDS) {
        return w if (contains_word(prose, w))
    }
    for (p in HOWTO_PAST_PHRASES) {
        return strip(p) if (find(prose, p) >= 0)
    }
    return ""
}

def private cites_architecture_anchor(line : string) : bool {
    var from = 0
    while (true) {
        let at = find(line, "ARCHITECTURE", from)
        return false if (at < 0)
        let md = find(line, ".md#", at)
        let sp = find(line, " ", at)
        return true if (md >= 0 && (sp < 0 || md < sp))
        from = at + 12
    }
    return false
}

def private report_uncited_step(path : string; step_line : int) {
    gate_finding(path, step_line, "a numbered step that cites no `ARCHITECTURE_*.md#<anchor>` - a HOW_TO step names the architecture section that owns it ({HOWTO_SECTION})")
}

[arch(at = "ARCHITECTURE_DOCS.md#howto-documents")]
def private check_howto_doc(path : string) {
    var inscope lines <- split(read_lf(path), "\n")
    var step_line = 0
    var step_cited = true
    var line_no = 0
    for (line in lines) {
        line_no++
        if (starts_with(line, "## ")) {
            if (!step_cited) {
                report_uncited_step(path, step_line)
            }
            var numbered = false
            peek_data(line) $(d) {
                numbered = digit_at(d, 3)
            }
            step_line = line_no
            step_cited = !numbered
        }
        step_cited ||= cites_architecture_anchor(line)
        let marker = past_marker(line)
        if (!empty(marker)) {
            gate_finding(path, line_no, "past-tense marker '{marker}' - a HOW_TO is a procedure in the imperative, and what happened lives in git ({HOWTO_SECTION})")
        }
        if (has_date(line)) {
            gate_finding(path, line_no, "a date - a HOW_TO is timeless, and what happened when lives in git ({HOWTO_SECTION})")
        }
        if (has_pr_number(line)) {
            gate_finding(path, line_no, "a PR or issue number - a HOW_TO cites architecture anchors, never the change that made a step ({HOWTO_SECTION})")
        }
    }
    if (!step_cited) {
        report_uncited_step(path, step_line)
    }
}

def private check_howto_docs {
    var docs : array<string>
    collect_files_ending("modules/dasLLAMA", ".md", docs)
    for (path in docs) {
        if (base_name(path) |> starts_with("HOW_TO_")) {
            check_howto_doc(path)
        }
    }
}

//! the arguments after `flag` on the command line: `-- --howto <doc>...` reads only the named documents as HOW_TO
//! documents, `-- --queue-calls <file>...` scans only the named sources for unlocked queue calls - the checks' fixture rails
def private fixture_args(flag : string) : array<string> {
    var docs : array<string>
    var on = false
    for (a in get_command_line_arguments()) {
        if (a == flag) {
            on = true
        } elif (on) {
            docs |> push(a)
        }
    }
    return <- docs
}

[export]
def main() : int {
    let fixtures <- fixture_args("--howto")
    if (!empty(fixtures)) {
        for (doc in fixtures) {
            check_howto_doc(doc)
        }
        return gate_verdict("dasllama --howto")
    }
    let queue_fixtures <- fixture_args("--queue-calls")
    if (!empty(queue_fixtures)) {
        check_locked_queue_calls(queue_fixtures)
        return gate_verdict("dasllama --queue-calls")
    }
    if (!fexist("modules/dasLLAMA/REVIEW.das") || !fexist(FACADE)) {
        to_log(LOG_ERROR, "modules/dasLLAMA/REVIEW.das: run from the repo root\n")
        return 2
    }
    check_meta_tripwires()
    check_image_layout_stamp()
    check_exe_fn_global_restore()
    check_gpu_home_reset_flags()
    check_gpu_home_seat_registrants()
    check_race_bind_numbers()
    check_gpu_role_partition()
    check_fastmath_default()
    check_mirror_slab_access()
    check_per_call_buffers()
    check_metal_kernel_load_addressing()
    check_image_backing_release()
    check_khr_stage16_abstract()
    check_ar_max_dim_triple()
    check_cm2_ladder_sets()
    check_cm2_stamp_tiles()
    check_kq_gemv_grid_literals()
    check_kq_form_matrix()
    check_vk_extension_roster()
    check_cuda_stream_discipline()
    check_discrete_seat_parity()
    check_no_hand_pipelines()
    check_locked_queue_calls()
    check_verify_decline_before_state_move()
    check_vk_scratch_cap_order()
    check_test_file_placement()
    check_unicode_tables_stamp()
    check_gemm_gen_stamp()
    check_device_creation_sites()
    check_arch_cites_by_name()
    check_require_bans()
    check_shared_template_homes()
    check_shared_file_spelling()
    check_shared_file_cites()
    check_test_heap_model()
    check_harness_dashv()
    check_bench_stamps()
    check_running_max_nan()
    check_census_evidence_pinned()
    check_load_big_format()
    check_rdec_pass_words()
    check_tests_doc()
    check_howto_docs()
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
    check_family_seams(ASR_CARRIER, "AsrKind", ASR_SEAM_EXEMPT, ASR_KIND_FILES,
        "ARCHITECTURE_MEDIA.md#tower-audio-charters", "dasllama_asr.das's public functions")
    return gate_verdict("dasllama")
}
