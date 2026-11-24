options gen2

require dastest/review_gate
require dastest/test_shape
require daslib/fio
require strings
require math
require daslib/strings_boost
require _model_categories
require ../../../utils/common/text_lines.das   // split_trimmed: a table cell's comma list

// The mechanical half of modules/dasLLAMA/tests/REVIEW_FILES.md
// (contract: REVIEW_COMMON.md at the repo root). Run from the repo
// root: bin/daslang modules/dasLLAMA/tests/REVIEW.das - exit 0
// clean, 1 with findings. The root is judged tests-only: run.das,
// the runner, is a script beside the suites it runs.

//! the programs a child process of this folder runs as a program of its own, each for a claim a verb of the test
//! child cannot carry
let private OWN_ROOTS = fixed_array("_child_root.das", "_tuned_fallbacks_root.das", "_exe_smoke_root.das", "_box_runtime_root.das",
    "_audio_lane_whisper_root.das", "_mx4_mint_root.das", "_fake_chat_root.das", "run.das",
    "dastest.das")

//! the test files that build a child's command line on a root they name themselves (`child_cmd_root`): the
//! module-cache test, whose root is a copy it writes
let private ROOT_CMD_CALLERS = fixed_array("test_f32_batch_tile_perms.das")

let private ONE_CHILD = ("a child process of this folder is a verb of the test child (`_child_root.das` dispatching a `_child_<verb>.das` "
    + "module, spawned through `child_verb_cmd`) - a program of its own is licensed only for a claim a verb cannot carry: "
    + "the program's own compile, the process it runs in, a model a stocked test loads, or a server or runner started by "
    + "its command line (ARCHITECTURE_ENGINE_SUPPORT.md#test-child)")

//! the literal first argument of every `call("` in `text`, with the 1-based line of each
def private literal_roots(text, call : string) : array<tuple<root : string; line : int>> {
    var found : array<tuple<root : string; line : int>>
    var inscope calls <- split(text, "{call}(\"")
    calls |> erase(0)
    for (tail in calls) {
        let close = find(tail, "\"")
        if (close > 0) {
            let root = slice(tail, 0, close)
            found |> push((root = root, line = find_line(text, "{call}(\"{root}")))
        }
    }
    return <- found
}

//! the calls that name the daslang binary a test starts
let private BINARY_CALLS = fixed_array("get_das_exe()", "das_exe()", "example_daslang()", "self_daslang()")

//! the spellings that put the modules DASLLAMA_DISABLE_MODULES names after the binary
let private BINARY_FLAGS = fixed_array("daslang_argv(", "disable_module_args(", "disable_module_flags(",
    "child_disable_flags()", "--disable-module")

//! the binary's starts that take no flags, each a file and the text of its line: a program that requires no GPU
//! module - the runner, which hands the knob to its children through their environment, and a gate child
let private BINARY_UNFLAGGED = fixed_array(
    "test_run_suites.das|\"\{scratch\}\{env\}\{das_exe()\} -dasroot \{get_das_root()\} \{cache\}",
    "test_facade_docs.das|[get_das_exe(), \"-dasroot\"")

let private BINARY_FLAGLESS = ("a test of this folder starts the daslang binary without the modules "
    + "DASLLAMA_DISABLE_MODULES leaves out: build an argv with `daslang_argv(<binary>, [...])`, or put "
    + "`\{child_disable_flags()\}` right after the binary in a command string (`_daslang_argv.das`) - a child "
    + "compiling another module set than its parent misses the compile records the run built and reparses "
    + "the engine inside its test's wall; a start BINARY_UNFLAGGED licenses runs a program that requires no GPU "
    + "module, so leaving one out changes nothing it compiles (ARCHITECTURE_ENGINE_SUPPORT.md#test-child)")

//! whether `line` holds one of `spellings`
def private holds_any(line : string; spellings : array<string>) : bool {
    for (s in spellings) {
        return true if (find(line, s) >= 0)
    }
    return false
}

//! whether an entry of BINARY_UNFLAGGED licenses `line` of the file `f`
def private unflagged_licensed(f, line : string) : bool {
    for (entry in BINARY_UNFLAGGED) {
        let bar = find(entry, "|")
        return true if (base_name(f) == slice(entry, 0, bar) && find(line, slice(entry, bar + 1)) >= 0)
    }
    return false
}

//! every line of the folder's `.das` files that names the daslang binary at the head of an argv literal or inside a command
//! string carries the module flags on the same line, but the starts BINARY_UNFLAGGED licenses
def private check_child_binary_flags() {
    var inscope starts <- [for (call in BINARY_CALLS); "[{call}"]
    starts |> push_from([for (call in BINARY_CALLS); "\{{call}\}"])
    var inscope flags <- to_array(BINARY_FLAGS)
    for (f in tracked_files(["modules/dasLLAMA/tests/*.das"], "the binary's starts")) {
        continue if (base_name(f) == "REVIEW.das")
        let text = strip_line_comments(fread(f))
        for (line, i in split(text, "\n"), count(1)) {
            continue if (!holds_any(line, starts))
            if (!holds_any(line, flags) && !unflagged_licensed(f, line)) {
                gate_finding(f, i, BINARY_FLAGLESS)
            }
        }
    }
}

//! every child process a test of this folder spawns is the one test child, or a licensed program: no root file, no
//! `child_cmd` / `child_cmd_at` naming a root, and no `child_cmd_root` caller outside the licensed sets
[arch(at = "../ARCHITECTURE_ENGINE_SUPPORT.md#test-child")]
def private check_one_test_child() {
    for (f in tracked_files(["modules/dasLLAMA/tests/_*_root.das"], "the test child's roots")) {
        if (!has_value(OWN_ROOTS, base_name(f))) {
            gate_finding(f, ONE_CHILD)
        }
    }
    for (f in tracked_files(["modules/dasLLAMA/tests/*.das"], "the test child's spawns")) {
        continue if (base_name(f) == "REVIEW.das" || base_name(f) == "_child_run.das")   //! the gate's needles, the helpers' own bodies
        let text = fread(f)
        for (call in ["child_cmd", "child_cmd_at"]) {
            for (r in literal_roots(text, call)) {
                if (!has_value(OWN_ROOTS, base_name(r.root))) {
                    gate_finding(f, r.line, ONE_CHILD)
                }
            }
        }
        if (find(text, "child_cmd_root(") >= 0 && !has_value(ROOT_CMD_CALLERS, base_name(f))) {
            gate_finding(f, find_line(text, "child_cmd_root("), ONE_CHILD)
        }
    }
}

let private CATEGORIES_DOC = "modules/dasLLAMA/TEST_CATEGORIES.md"

let private ONE_CATEGORY_ROW = ("a model category is one row of `model_categories` (`_model_categories.das`) and one table row "
    + "under `## Categories` of TEST_CATEGORIES.md holding the category's name, each stem of its model "
    + "files, its topics word for word and each stem of the files it opens beside its own - add, "
    + "change or drop the two in the same change (ARCHITECTURE_ENGINE_SUPPORT.md#test-model-filters)")

//! the categories document and the categories table name the same categories, each with its stems and topics
[arch(at = "../ARCHITECTURE_ENGINE_SUPPORT.md#test-model-filters")]
def private check_test_categories() {
    var inscope rows <- markdown_table_rows(fread(CATEGORIES_DOC), "Categories")
    var known : table<string>
    for (cat in model_categories()) {
        known |> insert(cat.name)
        var row_agrees = false
        for (row in rows) {
            continue if (length(row) < 4 || row[0] != cat.name)
            var inscope stems <- split_trimmed(row[1], ",")
            var inscope opens <- split_trimmed(row[3], ",")
            var inscope opened <- split_trimmed(cat.opens, ",")
            row_agrees = row[2] == cat.topics && length(opens) == length(opened)
            for (stem in split_trimmed(cat.stems, ",")) {
                row_agrees &&= has_value(stems, stem)
            }
            for (stem in opened) {
                row_agrees &&= has_value(opens, stem)
            }
        }
        if (!row_agrees) {
            gate_finding(CATEGORIES_DOC, "{ONE_CATEGORY_ROW} (the category {cat.name})")
        }
    }
    for (row, at in rows, count()) {
        if (at > 0 && !empty(row) && !key_exists(known, row[0])) {
            gate_finding(CATEGORIES_DOC, "{ONE_CATEGORY_ROW} (the row {row[0]})")
        }
    }
}

//! the section every load-once finding names
let private LOAD_ONCE_AT = "ARCHITECTURE_ENGINE_SUPPORT.md#test-load-once"

let private MODEL_ONCE = ("a model file loads once a form in an area's run: its block (`with_cpu_and_gpu_model`, `with_model_once`) "
    + "is called once an area and model file across this folder, its forms the call's list - fold the cells of this call, and the "
    + "forms they need, into the first call (ARCHITECTURE_ENGINE_SUPPORT.md#test-load-once)")

let private MODEL_LITERAL = ("a block (`with_cpu_and_gpu_model`, `with_model_once`) takes its run area and its model file as string "
    + "literals on the call's first line - `t, \"llm\", \"<file>.gguf\", ...`: this gate reads them to hold one call an area and model "
    + "across the folder (ARCHITECTURE_ENGINE_SUPPORT.md#test-load-once)")

let private LOAD_ONCE = ("a model file loads once in an area's run, through the area's one block for it (`with_cpu_and_gpu_model`, "
    + "`with_model_once`): move this cell onto a form of that call, or - where the cell must own its load - declare why in the "
    + "function holding the load, `own_load(\"<why>\")` (ARCHITECTURE_ENGINE_SUPPORT.md#test-load-once)")

let private OWN_LOAD_WHY = ("`own_load` names, as a non-empty string literal, why the function's load cannot be a form of its model's "
    + "block (ARCHITECTURE_ENGINE_SUPPORT.md#test-load-once)")

let private UNLISTED_LOADER = ("a function that passes its parameter to a loader is a loader: `LOADERS` (tests/REVIEW.das) lists it, "
    + "so the load-once check reads the model file at its callers (ARCHITECTURE_ENGINE_SUPPORT.md#test-load-once)")

//! the run areas whose test files the load-once check holds
let private LOAD_ONCE_AREAS = fixed_array("llm", "tts", "translate", "vision", "audio")

//! the areas whose model files are `.gguf` alone: a `.bin` their files name is a llama2.c checkpoint, a second read of
//! a 60 MB file
let private GGUF_ONLY_AREAS = fixed_array("llm")

//! the calls that load the model file their arguments name: the engine's loaders, then the folder's helpers that pass a file
//! to one (a listed helper the scanned files do not define is a finding); a call of a `BLOCKS` name is the area's block,
//! read apart
let private ENGINE_LOADERS = fixed_array("load_model_", "load_model", "load_gguf", "load_model_uncached", "load_model_cached",
    "load_cached_q8", "load_resident", "load_resident_host", "load_resident_as", "load_resident_at", "load_resident_by",
    "load_asr_model", "load_whisper_model", "load_parakeet_model", "load_gigaam_model", "load_canary_encoder", "load_vad_model",
    "load_audio_tower", "load_audio_tower_encoder", "load_audio_embedder", "load_gemma4a_encoder", "load_qwen3a_tower",
    "load_gemma3v_tower", "load_gemma4v_tower", "load_gemma4uv_embedder", "load_qwen3v_tower", "load_qwen25v_tower",
    "load_vision_embedder", "load_tts_model", "load_styletts2", "load_pocket", "load_g2p", "load_pos_tagger", "load_ru_stress",
    "load_ru_stress_pack", "load_nmt_model", "load_translator", "load_checkpoint",
    "stage_whisper_model", "stage_parakeet_model", "stage_parakeet_kq_model", "stage_gigaam_model", "stage_canary_encoder",
    "stage_audio_tower", "stage_gemma4a_encoder", "stage_qwen3a_tower", "stage_gemma3v_tower", "stage_gemma4v_tower",
    "stage_gemma4uv_embedder", "stage_qwen3v_tower", "stage_qwen25v_tower", "stage_styletts2", "stage_pocket", "stage_nmt_model")
let private LOADERS = fixed_array("aud_form", "aud_stage", "blob_twin", "canary_form", "canary_stage", "cuda_feed_rig", "cuda_forced_feed", "dense_feeds_block",
    "family_lane_cell", "g2p_pack_form", "g4a_stage", "gemma3v_12b_form", "gemma3v_form", "gemma3v_stage", "gemma4a_e2b_form", "gemma4a_e4b_form", "gemma4a_form", "gemma4uv_form", "gemma4uv_stage",
    "gemma4v_form", "gemma4v_stage", "gemma_format_cell", "gemma_model", "gigaam_form", "gigaam_minted", "greedy_turn", "image_file_transcript", "image_tower_form", "kq_clone_over_roster",
    "kq_native_load", "kquant_cell", "kv_codecs_cell", "lanes_cell", "live_cell", "load_and_drop", "load_capped",
    "load_kq_native", "load_metal_trunk", "load_model_and_drop", "load_outcome", "load_perop", "load_refusal", "load_resident_lane",
    "load_trunk", "metal_lossless_cell", "metal_prefill_cell", "metal_routed_cell",
    "metal_served_cell", "moe_forced_feed", "moe_forced_feed_pinned", "parakeet_form", "parakeet_stage", "parity_row_load", "parity_spec_arms",
    "pk_image_file_rows", "pocket_form", "pocket_minted", "pocket_tts_minted", "ppl_compare", "q3omni_form", "qwen25v_form", "qwen25v_stage",
    "qwen3a_form", "qwen3a_stage", "qwen3v_form", "qwen3v_stage",
    "ru_pack_form", "ru_stress_form", "sampled_metal_cell", "slot_swap_cycle", "speak_line_of",
    "stage_panic", "staged_tokens_at", "staged_tower_form", "staged_twin_tower", "stored_file_pins",
    "stories15m_form", "stream_twin_within", "styletts2_form", "tagger_pack_form", "tail_after_load",
    "translator_form", "translator_stage", "vad_form", "whisper_form", "whisper_stage", "with_cold_lane", "with_gemma_rig", "with_metal_trunk")

//! the file whose loads are the block's own body: no helper of it is listed
let private LOADER_HOMES = fixed_array("_cpu_gpu_model.das")

let private MODELS_LEAD = "\{models_dir()\}/"

//! the calls that are an area's one load of a model file: the chat models' block, and the block of every other model - under
//! the models directory, or under a root the call names
let private BLOCKS = fixed_array("with_cpu_and_gpu_model", "with_model_once", "with_model_once_at")

//! one load of a model file: the file and line, the call, whether the function holding it declares its own load
struct private LoadSite {
    model : string
    file : string
    line : int
    call : string
    own : bool
}

//! one function of a source: its name, its parameters' names, the bytes it spans, its own-load marker
struct private SourceFn {
    name : string
    params : array<string>
    start : int
    stop : int
    own : bool
}

//! the 1-based line of byte `at` of `text`
def private line_at(text : string; at : int) : int {
    var line = 1
    peek_data(text) $(d) {
        for (i in range(min(at, length(d)))) {
            if (int(d[i]) == '\n') {
                line++
            }
        }
    }
    return line
}

//! the model file names `args` holds as string literals: a `.gguf` name, its `{models_dir()}/` lead cut; a literal holding
//! any other interpolation names no file
def private gguf_names(args : string) : array<string> {
    var out : array<string>
    for (seg in quoted_segments(args)) {
        let name = seg |> starts_with(MODELS_LEAD) ? slice(seg, length(MODELS_LEAD)) : seg
        if ((name |> ends_with(".gguf") || name |> ends_with(".bin")) && find(name, "\{") < 0) {
            out |> push(name)
        }
    }
    return <- out
}

//! the identifiers of `args`, in order
def private idents_of(args : string) : array<string> {
    var out : array<string>
    peek_data(args) $(d) {
        var start = -1
        for (i in range(length(d) + 1)) {
            let b = i < length(d) ? int(d[i]) : ' '
            if (is_ident_byte(b)) {
                start = start < 0 ? i : start
            } elif (start >= 0) {
                if (!is_number(int(d[start]))) {
                    out |> push(slice(d, start, i))
                }
                start = -1
            }
        }
    }
    return <- out
}

//! the byte of `text` at `at`, or a space past either end
def private byte_at(text : string; at : int) : int {
    var b = ' '
    peek_data(text) $(d) {
        if (at >= 0 && at < length(d)) {
            b = int(d[at])
        }
    }
    return b
}

//! the text between the paren that opens at `open` and the one that closes it (`bracket_body`)
def private call_args(text : string; open : int) : string => bracket_body(text, open, '(', ')')

//! the names the last `let` / `var` line of `lines` before line `before` (0: every line) binding `id` holds; where it holds
//! none, `depth` hops more through the identifiers of its right-hand side, bound in `lines` before it
def private bound_names(lines : array<string>; id : string; before : int; depth : int = 1) : array<string> {
    var bound = ""
    var at = 0
    for (ln, i in lines, count(1)) {
        break if (before > 0 && i >= before)
        var l = strip(ln)
        l = l |> starts_with("let private ") ? "let {slice(l, 12)}" : l
        continue if (!(l |> starts_with("let ") || l |> starts_with("var ")))
        let rest = strip(slice(l, 4))
        let inscope_rest = rest |> starts_with("inscope ") ? slice(rest, 8) : rest
        continue if (!(inscope_rest |> starts_with(id)) || length(inscope_rest) == length(id) || is_ident_byte(byte_at(inscope_rest, length(id))))
        bound = inscope_rest
        at = i
    }
    var out <- gguf_names(bound)
    return <- out if (!empty(out) || depth <= 0 || empty(bound))
    for (next in idents_of(slice(bound, length(id)))) {
        delete out
        out <- bound_names(lines, next, at, depth - 1)
        return <- out if (!empty(out))
    }
    return <- out
}

//! the `let` lines of `text` outside every brace: a file's top level, judged by depth, not by column
def private top_level_lets(text : string) : array<string> {
    var out : array<string>
    peek_data(text) $(d) {
        var depth = 0
        var quote = 0
        var start = 0
        for (i in range(length(d) + 1)) {
            let b = i < length(d) ? int(d[i]) : '\n'
            if (b == '\n') {
                if (depth == 0 && quote == 0 && slice(d, start, i) |> starts_with("let ")) {
                    out |> push(slice(d, start, i))
                }
                start = i + 1
            } elif (quote != 0) {
                quote = (b == quote && int(d[i - 1]) != '\\') ? 0 : quote
            } elif (b == '"' || b == '\'') {
                quote = b
            } elif (b == '{') {
                depth++
            } elif (b == '}') {
                depth--
            }
        }
    }
    return <- out
}

//! the top-level `let` names of `.gguf` literals across `files`: an identifier a call passes resolves here last
def private global_names(files : array<string>) : table<string; array<string>> {
    var out : table<string; array<string>>
    for (f in files) {
        for (ln in top_level_lets(strip_line_comments(fread(f)))) {
            let l = ln |> starts_with("let private ") ? slice(ln, 12) : slice(ln, 4)
            var inscope ids <- idents_of(l)
            continue if (empty(ids))
            var names <- gguf_names(l)
            if (!empty(names)) {
                out[ids[0]] <- names
            }
        }
    }
    return <- out
}

//! the functions of `text`, a `def` at column 0 each, with their parameters and their `own_load` marker; an `own_load` whose
//! reason is not a non-empty literal is a finding against `file`
def private source_fns(file, text : string) : array<SourceFn> {
    var out : array<SourceFn>
    var inscope heads <- split("\n{text}", "\ndef ")   //! a `def` on the first line opens a function too
    out |> reserve(length(heads))
    var at = length(heads[0])
    for (i in range(1, length(heads))) {
        let head = heads[i]
        let body = head |> starts_with("private ") ? slice(head, 8) : head
        let paren = find(body, "(")
        let name_end = paren >= 0 ? paren : find(body, " ")
        var f = SourceFn(name = slice(body, 0, max(name_end, 0)), start = at, stop = at + length(head) + 5)
        if (paren >= 0 && paren < find(body, "\n")) {
            for (group in split(call_args(body, paren), ";")) {
                let colon = find(group, ":")
                for (p in split(colon >= 0 ? slice(group, 0, colon) : group, ",")) {
                    let n = strip(p) |> starts_with("var ") ? strip(slice(strip(p), 4)) : strip(p)
                    if (!empty(n) && n != "t" && n != "tt") {
                        f.params |> push(n)
                    }
                }
            }
        }
        own_load_marker(f, file, text, head)
        out |> emplace(f)
        at += length(head) + 5
    }
    return <- out
}

//! `f.own` set where `head`, its function's text, declares its own load; a declaration with no literal reason is a finding
def private own_load_marker(var f : SourceFn; file, text, head : string) {
    var from = 0
    while (true) {
        let at = find(head, "own_load(", from)
        break if (at < 0)
        from = at + 1
        continue if (is_ident_byte(byte_at(head, at - 1)) || at == 0 || (head |> starts_with("private ") && at == 8))
        var inscope why <- quoted_segments(call_args(head, at + 8))
        if (empty(why) || empty(strip(why[0])) || !(strip(call_args(head, at + 8)) |> starts_with("\""))) {
            gate_finding(file, line_at(text, f.start + 4 + at), OWN_LOAD_WHY)
        } else {
            f.own = true
        }
    }
}

//! the function of `fns` holding byte `at`
def private fn_at(fns : array<SourceFn>; at : int) : int {
    for (f, i in fns, count()) {
        return i if (at >= f.start && at < f.stop)
    }
    return -1
}

//! every call of a loader of `loaders` in `file` whose model file the gate reads: a literal in the call's arguments, else an
//! identifier bound to one in the holding function, the file or `globals`. A call that passes the holding function's own
//! parameter makes that function a loader - a finding where `loaders` does not list it
def private load_sites(file : string; loaders : array<string>; globals : table<string; array<string>>;
                       var sites : array<LoadSite>) {
    let text = strip_line_comments(fread(file))
    var inscope top <- top_level_lets(text)
    var inscope fns <- source_fns(file, text)
    for (f in fns) {
        g_defined |> insert(f.name)
    }
    for (call in loaders) {
        var from = 0
        while (true) {
            let at = find(text, "{call}(", from)
            break if (at < 0)
            from = at + 1
            continue if (is_ident_byte(byte_at(text, at - 1)) || byte_at(text, at - 1) == '.')
            let line = line_at(text, at)
            let fi = fn_at(fns, at)
            let args = call_args(text, at + length(call))
            var inscope names <- site_names(args, top, line, fi >= 0 ? fns[fi].start : 0, text, globals)
            if (empty(names) && !(fi >= 0 && passed_param(file, line, fns[fi], call, args, loaders))) {
                g_unread++
                to_log(LOG_INFO, "UNREAD {file}:{line}: `{call}` names its model file through a value the load-once check cannot read\n")
            }
            sites |> reserve(length(sites) + length(names))
            for (n in names) {
                sites |> push(LoadSite(model = n, file = file, line = line, call = call, own = fi >= 0 && fns[fi].own))
                to_log(LOG_INFO, "SITE {file}:{line}: `{call}` loads {n}{fi >= 0 && fns[fi].own ? " (its own load)" : ""}\n")
            }
        }
    }
}

//! the arguments of a call's argument text `args`, split at its top-level commas, string literals read whole
def private top_args(args : string) : array<string> {
    var out : array<string>
    peek_data(args) $(d) {
        var depth = 0
        var quote = false
        var start = 0
        for (i in range(length(d))) {
            let b = int(d[i])
            if (quote) {
                quote = b != '"' || int(d[i - 1]) == '\\'
            } elif (b == '"') {
                quote = true
            } elif (b == '(' || b == '[') {
                depth++
            } elif (b == ')' || b == ']') {
                depth--
            } elif (b == ',' && depth == 0) {
                out |> push(slice(d, start, i))
                start = i + 1
            }
        }
        out |> push(slice(d, start, length(d)))
    }
    return <- out
}

//! whether a call's argument `a` can name no model file: the test's `T?`, a number, a flag, a string literal that names none
def private no_file_arg(a : string) : bool {
    return true if (a == "t" || a == "tt" || a == "ct" || a == "true" || a == "false" || is_number(byte_at(a, 0)))
    return a |> starts_with("\"") && empty(gguf_names(a))
}

//! the model files of the first argument in `args` that names some (`no_file_arg` passed over; a loader takes its model file
//! ahead of a drafter or a companion): its literals, else the first of its identifiers bound to some in the function holding the
//! call (from `fn_start`, before `line`), in `top` (the file's top-level `let` lines) or in `globals`; an argument that calls a function and names none ends the search
def private site_names(args : string; top : array<string>; line, fn_start : int; text : string;
                       globals : table<string; array<string>>) : array<string> {
    var inscope fn_lines <- split(slice(text, fn_start, length(text)), "\n")
    let fn_line = line_at(text, fn_start)
    var out : array<string>
    for (raw in top_args(args)) {
        let arg = strip(raw)
        continue if (no_file_arg(arg))
        delete out
        out <- gguf_names(arg)
        return <- out if (!empty(out))
        for (id in idents_of(arg)) {
            delete out
            out <- bound_names(fn_lines, id, line - fn_line + 1)
            return <- out if (!empty(out))
            delete out
            out <- bound_names(top, id, 0)
            return <- out if (!empty(out))
            if (key_exists(globals, id)) {
                out := globals?[id] ?? array<string>()
                return <- out
            }
        }
        return <- out if (find(arg, "(") >= 0)   //! a value a call computes: the file it names is the run's
    }
    return <- out
}

//! whether `f` passes a parameter of its own to `call` - a finding where `loaders` does not list `f`
def private passed_param(file : string; line : int; f : SourceFn; call, args : string; loaders : array<string>) : bool {
    var inscope ids <- idents_of(args)
    for (p in f.params) {
        continue if (!has_value(ids, p))
        if (!has_value(loaders, f.name) && !has_value(LOADER_HOMES, base_name(file))) {
            gate_finding(file, line, "{UNLISTED_LOADER} - `{f.name}` passes `{p}` to `{call}`")
        }
        return true
    }
    return false
}

var private g_unread = 0   //! the load sites whose model file the check could not read
var private g_defined : table<string>   //! the functions the scanned files define: a listed helper is one of them

//! the test files the runner `run_path`'s `area_tests` lists for `area`, as paths beside it; a finding where it lists none
def private area_files(run_path, area : string) : array<string> {
    let run = fread(run_path)
    let dir = dir_name(run_path)
    let body_at = find(run, "def area_tests(")
    let at = find(run, "area == \"{area}\")", body_at)
    var out : array<string>
    if (body_at >= 0 && at >= 0) {
        var stop = find(run, "\} elif (area ==", at)
        stop = stop < 0 ? find(run, "var none", at) : stop
        for (seg in quoted_segments(slice(run, at, stop))) {
            if (seg |> starts_with("test_") && seg |> ends_with(".das") && fexist("{dir}/{seg}")) {
                out |> push("{dir}/{seg}")
            }
        }
    }
    if (empty(out)) {
        gate_finding(run_path, "the load-once check read no test file of the area {area} off `area_tests`: it held nothing ({LOAD_ONCE_AT})")
    }
    return <- out
}

let private TESTS = "modules/dasLLAMA/tests"

//! the first line of each block call (`BLOCKS`) in `f`, its area and file where both are literals; a call that names them
//! otherwise is a finding
def private block_calls(f : string) : array<tuple<key : string; line : int; call : string>> {
    var out : array<tuple<key : string; line : int; call : string>>
    let text = fread(f)
    for (call in BLOCKS) {
        var inscope calls <- split(text, "{call}(t, ")
        calls |> erase(0)
        for (tail in calls) {
            let eol = find(tail, "\n")
            let head = eol >= 0 ? slice(tail, 0, eol) : tail
            let at = find_line(text, "{call}(t, {head}")
            var inscope lits <- split(head, "\"")   //! `"area", "file", rest`: empty, the area, the comma, the file, the rest
            if (!(head |> starts_with("\"")) || length(lits) < 5 || strip(lits[2]) != ",") {
                gate_finding(f, at, MODEL_LITERAL)
                continue
            }
            out |> push((key = "{lits[1]}, {lits[3]}", line = at, call = call))
        }
    }
    return <- out
}

//! the load-once check over `files` for `area`: the area's blocks (`blocks`, keyed `area, file`) each the one load of its model
//! file; every other load site of a model a block loads, and every load site of a model loaded at two sites and by no block,
//! is a finding unless its function declares its own load, a load a call of `loaders` names
def private load_once_area(area : string; files : array<string>; blocks : table<string; string>; globals : table<string; array<string>>;
                           loaders : array<string>) {
    var inscope sites : array<LoadSite>
    for (f in files) {
        load_sites(f, loaders, globals, sites)
    }
    if (has_value(GGUF_ONLY_AREAS, area)) {
        sites |> erase_if() $(s) => s.model |> ends_with(".bin")
    }
    var unowned : table<string; array<string>>
    for (s in sites) {
        if (!s.own && !key_exists(blocks, "{area}, {s.model}")) {
            unowned[s.model] |> push("{base_name(s.file)}:{s.line}")
        }
    }
    for (s in sites) {
        continue if (s.own)
        let owner = blocks?["{area}, {s.model}"] ?? ""
        let others = length(unowned?[s.model] ?? array<string>())
        continue if (empty(owner) && others < 2)
        let where_ = empty(owner) ? "loaded at {others} sites of the {area} area and by no block: {join(unowned?[s.model] ?? array<string>(), ", ")}" : "its block is {owner}"
        gate_finding(s.file, s.line, "`{s.call}` loads {s.model}, {where_} - {LOAD_ONCE}")
    }
    delete unowned
}

//! the text between the first `open` bracket at or past byte `at` of `text` and the one that
//! closes it, string literals read whole; "" where none closes
def private bracket_body(text : string; at : int; open, close : int) : string {
    var out = ""
    peek_data(text) $(d) {
        var depth = 0
        var quote = false
        var start = 0
        for (i in range(at, length(d))) {
            let b = int(d[i])
            if (quote) {
                quote = b != '"' || int(d[i - 1]) == '\\'
            } elif (b == '"') {
                quote = true
            } elif (b == open) {
                start = depth == 0 ? i + 1 : start
                depth++
            } elif (b == close && depth > 0) {
                depth--
                if (depth == 0) {
                    out = slice(d, start, i)
                    break
                }
            }
        }
    }
    return out
}

//! the string literals of each row of the table literal `name` (`let [private] <name> <- [...]`)
//! of `text`
def private table_rows(text, name : string) : array<array<string>> {
    var rows : array<array<string>>
    let lines = "\n{text}"   // a table on the first line starts a line too
    for (head in fixed_array("\nlet private {name} <- [", "\nlet {name} <- [")) {
        let at = find(lines, head)
        continue if (at < 0)
        let body = bracket_body(lines, at + length(head) - 1, '[', ']')
        rows <- [for (row in top_args(body)); quoted_segments(row)]
        break
    }
    return <- rows
}

//! each `<name>[<n>]` of `body`, a literal row of a table
def private row_refs(body : string) : array<tuple<name : string; row : int>> {
    var out : array<tuple<name : string; row : int>>
    var inscope parts <- split(body, "[")
    for (i in range(1, length(parts))) {
        var index = 0
        var digits = 0
        peek_data(parts[i]) $(d) {
            for (b in d) {
                break if (!is_number(int(b)))
                index = index * 10 + int(b) - '0'
                digits++
            }
        }
        continue if (digits == 0 || byte_at(parts[i], digits) != ']')
        let prev = parts[i - 1]
        var start = length(prev)
        while (start > 0 && is_ident_byte(byte_at(prev, start - 1))) {
            start--
        }
        if (start < length(prev)) {
            out |> push((name = slice(prev, start), row = index))
        }
    }
    return <- out
}

let private BLOCK_ROW = ("a load-once block's cells take the row of its own file: a row of a table "
    + "literal the block passes holds a string that is the block's file stem or one of its `-` "
    + "tokens (the language code, the file stem)")

//! every block call (`BLOCKS`) of `files` whose body takes a row of a table literal of its file (`<name>[<n>]`) passes a
//! row naming the block's file - its stem, or a `-` token of it, one of the row's strings
def private check_block_rows(files : array<string>) {
    for (f in files) {
        let text = strip_line_comments(fread(f))
        for (call in BLOCKS) {
            var from = 0
            while (true) {
                let at = find(text, "{call}(t, ", from)
                break if (at < 0)
                from = at + 1
                continue if (is_ident_byte(byte_at(text, at - 1)))
                var inscope lits <- quoted_segments(call_args(text, at + length(call)))
                continue if (length(lits) < 2)
                let file = lits[1]
                let dot = rfind(file, ".")
                let stem = dot > 0 ? slice(file, 0, dot) : file
                var inscope tokens <- split(stem, "-")
                for (ref in row_refs(bracket_body(text, at, '{', '}'))) {
                    var inscope rows <- table_rows(text, ref.name)
                    continue if (ref.row >= length(rows))
                    var named = false
                    for (v in rows[ref.row]) {
                        named = v == stem || has_value(tokens, v)
                        break if (named)
                    }
                    if (!named) {
                        let row = "`{ref.name}[{ref.row}]` ({join(rows[ref.row], ", ")})"
                        let msg = "{BLOCK_ROW} - the block of {file} passes {row} ({LOAD_ONCE_AT})"
                        gate_finding(f, line_at(text, at), msg)
                    }
                }
            }
        }
    }
}

//! one block call (`BLOCKS`) an area and model file across the folder's test files (a second call naming the same area and
//! file is a second full load of a form; every call names its area and file by literals on the call's first line), and in
//! each area of LOAD_ONCE_AREAS no other load of a model file a call loads, nor two loads of one no call loads
[arch(at = "../ARCHITECTURE_ENGINE_SUPPORT.md#test-load-once")]
def private check_models_load_once() {
    var inscope files <- tracked_files(["modules/dasLLAMA/tests/test_*.das"], "the model blocks")
    var inscope fixtures <- [for (f in tracked_files(["modules/dasLLAMA/tests/_*.das"], "the fixture modules")); f;
        where !(base_name(f) |> starts_with("_child_")) && !(base_name(f) |> ends_with("_root.das"))]
    var inscope blocks <- load_once_blocks(files)
    var inscope globals <- global_names(fixtures)
    var inscope none : array<LoadSite>
    var inscope listed <- [for (l in LOADERS); l]
    var inscope loaders <- all_loaders(listed)
    for (f in fixtures) {
        load_sites(f, loaders, globals, none)
    }
    for (area in LOAD_ONCE_AREAS) {
        var inscope area_set <- area_files("{TESTS}/run.das", area)
        load_once_area(area, area_set, blocks, globals, loaders)
    }
    check_block_rows(files)
    stale_loaders(listed)
    if (g_unread > 0) {
        to_log(LOG_WARNING, "REVIEW.das load-once: {g_unread} load site(s) name their model file through a value the check cannot read - DAS_LOG_LEVEL=info prints them\n")
    }
}

//! the blocks of `files` keyed `area, file` to their `file:line`, in the files' order; a second block of one key is a finding
def private load_once_blocks(files : array<string>) : table<string; string> {
    var blocks : table<string; string>
    for (f in files) {
        for (b in block_calls(f)) {
            if (!key_exists(blocks, b.key)) {
                blocks |> insert(b.key, "{base_name(f)}:{b.line}")
            } else {
                gate_finding(f, b.line, "{MODEL_ONCE} - the first call is {blocks?[b.key] ?? ""}")
            }
        }
    }
    return <- blocks
}

//! the engine's loaders and the helpers `listed`
def private all_loaders(listed : array<string>) : array<string> {
    var out <- [for (l in ENGINE_LOADERS); l]
    out |> push_from(listed)
    return <- out
}

//! a finding for each helper of `listed` that no scanned file defines
def private stale_loaders(listed : array<string>) {
    for (l in listed) {
        if (!key_exists(g_defined, l)) {
            gate_finding("{TESTS}/REVIEW.das", "LOADERS lists `{l}`, which no file the load-once check reads defines: drop it from the list ({LOAD_ONCE_AT})")
        }
    }
}

//! the load-once check over the named files alone, as the areas `areas` - an area's files the ones the runner `area_run`
//! lists for it where named, else the named files: its fixture rail. Its helper loaders are `listed` where it names some, each
//! one a named file must define, else LOADERS
def private check_load_once_files(files, listed, areas : array<string>; area_run : string) {
    var inscope blocks <- load_once_blocks(files)
    var inscope globals <- global_names(files)
    var inscope loaders <- all_loaders(empty(listed) ? [for (l in LOADERS); l] : listed)
    for (area in areas) {
        var inscope area_set <- empty(area_run) ? [for (f in files); f] : area_files(area_run, area)
        load_once_area(area, area_set, blocks, globals, loaders)
    }
    check_block_rows(files)
    if (!empty(listed)) {
        stale_loaders(listed)
    }
}

let private CELL_PINS = fixed_array("pin_kernel_backend", "pin_kernel_backend_env", "clear_kernel_backend_pin",
    "restore_kernel_backend", "with_kernel_backend", "select_kernel_backend", "select_batch_backend", "set_kquant_native")

let private CELL_PIN = ("a fixture cell module (`tests/_*_cells.das`) never pins, clears or re-pins the kernel backend or the K-quant "
    + "native knob: the backend and the knob a cell runs on are properties of its form's load - give the form "
    + "`cpu_cells_form(backend = \"<name>\", kq_native = KqNative.<on|off>)` and call the cell on it "
    + "(ARCHITECTURE_ENGINE_SUPPORT.md#test-load-once)")

//! no call of a CELL_PINS name in `files`, comments aside
def private check_cell_pins_in(files : array<string>) {
    for (f in files) {
        let text = strip_line_comments(fread(f))
        for (call in CELL_PINS) {
            var from = 0
            while (true) {
                let at = find(text, "{call}(", from)
                break if (at < 0)
                from = at + 1
                continue if (is_ident_byte(byte_at(text, at - 1)) || byte_at(text, at - 1) == '.')
                gate_finding(f, line_at(text, at), "{CELL_PIN} - `{call}`")
            }
        }
    }
}

//! the fixture cell modules hold no backend or K-quant knob pin: their forms do
[arch(at = "../ARCHITECTURE_ENGINE_SUPPORT.md#test-load-once")]
def private check_cell_pins() {
    var inscope files <- tracked_files(["modules/dasLLAMA/tests/_*_cells.das"], "the fixture cell modules")
    check_cell_pins_in(files)
}

//! a test file that calls a block (`BLOCKS`): its one `[test]` runs the block's cells
def private holds_block(path : string) : bool {
    let text = fread(path)
    for (call in BLOCKS) {
        return true if (find(text, "{call}(t, ") >= 0)
    }
    return false
}

//! the folder's file shape, the three-tests floor lifted from a file that holds a block
//! (ARCHITECTURE_ENGINE_SUPPORT.md#test-load-once)
def private check_folder_shape(root : string) {
    check_test_shape_floorless(root, 100, true) $(path : string) => holds_block(path)
}

//! the gate; `-- --load-once <file>... --loaders <name>... --area-run <run.das> --area <name>...` runs
//! the load-once check over the named files alone as the named areas, `llm` where none is named (`check_load_once_files`),
//! `-- --floor <folder>` the file-shape check over that folder, and `-- --cell-pins <file>...` the cell-pin check - its
//! fixture rails
[export]
def main() : int {
    let fixtures <- gate_flag_args("--load-once")
    if (!empty(fixtures)) {
        let area_run <- gate_flag_args("--area-run")
        var inscope areas <- gate_flag_args("--area")
        if (empty(areas)) {
            areas |> push("llm")
        }
        let run_at = empty(area_run) ? "" : area_run[0]
        check_load_once_files(fixtures, gate_flag_args("--loaders"), areas, run_at)
        return gate_verdict("modules/dasLLAMA/tests --load-once")
    }
    let floors <- gate_flag_args("--floor")
    if (!empty(floors)) {
        check_folder_shape(floors[0])
        return gate_verdict("modules/dasLLAMA/tests --floor")
    }
    let pinned <- gate_flag_args("--cell-pins")
    if (!empty(pinned)) {
        check_cell_pins_in(pinned)
        return gate_verdict("modules/dasLLAMA/tests --cell-pins")
    }
    check_folder_shape(TESTS)
    check_one_test_child()
    check_child_binary_flags()
    check_test_categories()
    check_models_load_once()
    check_cell_pins()
    return gate_verdict("modules/dasLLAMA/tests")
}
