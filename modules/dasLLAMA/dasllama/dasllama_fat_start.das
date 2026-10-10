options gen2
options indenting = 4
options _dasllama_internal = true

module dasllama_fat_start shared private

require dasllama/dasllama_lint public
require dasllama/dasllama_common
require dasllama/dasllama_env   // nolint:STYLE030,LINT019 - dasllama_home_dir, used only inside the llvm_tune static_if half
require daslib/fio
require dasllama/dasllama_math   // the runtime knob getters the snapshot records
require dasllama/dasllama_par    // get_total_hw_jobs - the advisory "threads" entry
require daslib/json_boost
require daslib/strings_boost   // nolint:STYLE030,LINT019 - join, used only inside the llvm_tune static_if half
require daslib/module_path   // get_this_module_dir, is_standalone_exe - the engine's own source directory, for the kernel digest
require [tune_framework]
require ?das_metal dasllama/dasllama_metal_prefill  // metal_tensor_race (Apple static_if half)
require ?das_metal dasllama/dasllama_metal_kernels  // metal_tensor_race_decode (Apple static_if half)

//! The runtime section of the app sidecar, and the races that fill it without a tuner: the mint
//! writes it at the end of the kernel half, and a fat exe - which ships no tuner and no policy
//! rail - races its Metal twins once at first start and writes the same section beside itself.
//! Nothing here loads a model or spawns a child; every confirm stays in the harness.

//! The `"runtime"` section as a mint or a first-start race writes it: the CURRENT runtime knobs
//! (defaults unless this process changed them), so the file is complete and hand-editable, plus
//! the Metal twin crowns. TB/L2-budget refinement stays manual (`harness/tune_tb.das` shows the
//! cliff; edit under the knee); `threads` is a receipt of the lanes at mint time, 0 when no job
//! queue exists yet - a fat exe's first start runs before the application opens its queue.
def public dasllama_runtime_snapshot(metal_crowns : string) : JsonValue? {
    let lanes = is_job_que_available() ? get_total_hw_jobs() + 1 : 0
    return JV((q8_token_block = get_q8_token_block(), q8_l2_budget = get_q8_l2_budget(),
        matmul_min_chunk_rows = get_matmul_min_chunk_rows(),
        matmul_min_chunk_rows_gemv = get_matmul_min_chunk_rows_gemv(),
        target_chunk_work = get_target_chunk_work(), gemv_lane_cap = get_gemv_lane_cap(),
        batch_lane_cap = get_batch_lane_cap(), batch_grid_2d = get_batch_grid_2d(),
        dispatch_worker_limit = get_dispatch_worker_limit_(),
        team_rank_gate = get_team_rank_gate(),   // -1 = unset (apply skips negatives; env/que default rules)
        attn_par_threshold = get_attn_par_threshold(),
        requant_par_threshold = get_requant_par_threshold(), norm_par_threshold = get_norm_par_threshold(),
        rope_par_threshold = get_rope_par_threshold(), kv_store_par_threshold = get_kv_store_par_threshold(),
        act_par_threshold = get_act_par_threshold(), q8_chunks_per_job = get_q8_chunks_per_job(),
        q4_chunks_per_job = get_q4_chunks_per_job(), q8_batch_chunks_per_job = get_q8_batch_chunks_per_job(),
        jobque_spin_us = get_jobque_spin_us_(), jobque_join_poll = get_jobque_join_poll(),
        threads = lanes, metal_tensor = metal_crowns,   // total lanes (workers + computing main), the DAS_JOBQUE_THREADS unit
        mtp_depth_nextn = get_mtp_depth_nextn(), mtp_depth_assistant = get_mtp_depth_assistant(),
        metal_tall_floor = metal_tall_floor(), metal_devw_small_panel_mb = metal_devw_small_panel_mb(),
        metal_cvt_min_rows = metal_cvt_min_rows()))
}

// one METAL_TWIN race row: log it and bank the family when the tensor twin won. Untyped `r` on
// purpose: the two race halves come through different require paths and their element types
// do not unify into one typed parameter
def private absorb_twin(var wins : array<string>; var ran : bool&; r) {
    ran = true
    let note = empty(r.note) ? "" : " ({r.note})"
    to_log(LOG_INFO, "METAL_TWIN {r.family}: base={r.base_ms}ms tensor={r.twin_ms}ms -> {r.winner}{note}\n")
    if (r.winner == "tensor") {
        wins |> push(r.family)
    }
}

//! The Metal-4 tensor twin race: each tensor pso twin against its shipped simdgroup kernel on
//! this box's GPU, prefill and decode halves. `wins` is the families whose twin won - the
//! `"metal_tensor"` crown list - and `ran` says a device raced at all (false = no Metal device,
//! or a build without the Metal module). Synthetic: no model, no child process.
def public dasllama_metal_crown_race() : tuple<wins : array<string>; ran : bool> {
    var wins : array<string>
    var ran = false   // nolint:LINT003,LINT019 — written by the das_metal arm
    static_if (typeinfo builtin_module_exists(das_metal)) {
        for (r in metal_tensor_race()) {
            absorb_twin(wins, ran, r)
        }
        for (r in metal_tensor_race_decode()) {
            absorb_twin(wins, ran, r)
        }
    }
    return <- (wins <- wins, ran = ran)
}

//! Race the runtime section on this box and write it into the app sidecar
//! (`tune_manifest_path()`), the kernels section untouched: the Metal twins under the tune
//! progress display, then the knob snapshot. True when the sidecar was written.
[arch(at = "../ARCHITECTURE_MEASUREMENT_FAT_START.md#fat-first-start")]
def public dasllama_race_runtime_section() : bool {
    static_if (typeinfo module_exists(llvm_tune)) {
        let path = tune_manifest_path()
        to_log(LOG_INFO, "dasLLAMA: first start on this box - minting the runtime section (the Metal twin kernels race where a Metal device exists); it is kept in {path} and the next start reads it\n")
        tune_progress_plan("dasllama_first_start", 1)
        tune_progress_kernel_begin("metal_crowns", 0)
        var race <- dasllama_metal_crown_race()
        let crowns = join(race.wins, ",")
        tune_progress_kernel_end("metal_crowns", crowns, !race.ran ? "skipped" : (empty(crowns) ? "holds" : "beats"))
        tune_progress_finish()
        if (!race.ran) {
            to_log(LOG_INFO, "dasLLAMA: no Metal device - the runtime section records the knob defaults\n")
        }
        var runtime = dasllama_runtime_snapshot(crowns)
        let noKernels : table<string; string>
        let wrote = tune_sidecar_merge(noKernels, runtime)
        if (!wrote) {
            to_log(LOG_WARNING, "dasLLAMA: could not write {path} - the crowns hold for this process only; set DAS_TUNE_MANIFEST to a writable path to keep them\n")
            set_metal_tensor_crowns(crowns)
        }
        delete race
        return wrote
    } else {
        return false
    }
}

// once per process: an unwritable sidecar would otherwise race again on every apply
var private g_fat_start_ran = false

//! The first-start hook a fat exe fires, once per process: a build that is not fat has a policy
//! rail and a tuner for its runtime section and does nothing here.
def public dasllama_fat_first_start() : bool {
    static_if (typeinfo module_exists(llvm_tune)) {
        return false if (!tune_fat_built() || g_fat_start_ran)
        g_fat_start_ran = true
        return dasllama_race_runtime_section()
    } else {
        return false
    }
}

//! Where a fat exe keeps its sidecar: `<dasllama home>/tune/<exe name>.tune.json`, so an installed
//! exe never writes beside itself. `beside` is the default path next to the exe.
def public dasllama_fat_sidecar_path(dasllama_home, beside : string) : string =>
    path_join(path_join(dasllama_home, "tune"), base_name(beside))

//! Make `home_path`'s directory, and copy the sidecar at `beside` there when `home_path` has none.
def public dasllama_adopt_sidecar(beside, home_path : string) {
    mkdir_rec(dir_name(home_path))
    if (!stat(home_path).is_valid && stat(beside).is_valid) {
        var err = ""
        if (copy_file(beside, home_path, false, err)) {
            to_log(LOG_INFO, "dasLLAMA: the tune sidecar {beside} is copied to {home_path}, which every start reads from now on\n")
        }
    }
}

//! Point a fat exe's sidecar at the per-user directory, unless its location was chosen already
//! (`DAS_TUNE_MANIFEST`, or an earlier call); a sidecar an earlier version wrote beside the exe is
//! copied there once.
[arch(at = "../ARCHITECTURE_MEASUREMENT_FAT_START.md#fat-first-start")]
def public dasllama_place_fat_sidecar {
    static_if (typeinfo module_exists(llvm_tune)) {
        return if (!tune_fat_built() || tune_manifest_explicit())
        let beside = tune_manifest_path()
        let home_path = dasllama_fat_sidecar_path(dasllama_home_dir(), beside)
        dasllama_adopt_sidecar(beside, home_path)
        set_tune_manifest_runtime_path(home_path)
    }
}

// fnv-1a over a string's bytes, as a hex word: the kernel digest and the box key are names, never compared as numbers
def private fnv_hex(s : string) : string {
    var h = 0xcbf29ce484222325ul
    peek_data(s) $(d) {
        for (b in d) {
            h = (h ^ uint64(b)) * 0x100000001b3ul
        }
    }
    return "{h}"
}

// the files of `dir` whose names `keep` admits, sorted, each as name:size:mtime
def private digest_files(dir : string; keep : block<(fname : string) : bool>; var w : StringBuilderWriter) : int {
    var names : array<string>
    fio::dir(dir) $(fname) {
        if (fname |> ends_with(".das") && invoke(keep, fname)) {
            names |> push(fname)
        }
    }
    sort(names)
    for (n in names) {
        var fs : FStat
        if (stat(path_join(dir, n), fs)) {
            write(w, "{n}:{fs.size}:{fs.mtime};")
        }
    }
    return length(names)
}

//! The digest of the sources this process compiles its Metal kernels from: the name, size and mtime of every
//! `dasllama_metal*.das`, the shared kernel templates and the spliced math helpers beside this file, and the MSL emitter
//! under `modules/dasMetal/metal/`; "" where the engine's sources are not on disk.
def public dasllama_kernel_sources_digest() : string {
    let dir = get_this_module_dir()
    var seen = 0
    let acc = build_string() $(w) {
        seen = digest_files(dir, $(fname) => fname |> starts_with("dasllama_metal") || fname == "dasllama_gpu_kernels_common.das"
            || fname == "dasllama_gpu_math.das", w)
        digest_files(path_join(path_join(path_join(dir, ".."), ".."), path_join("dasMetal", "metal")), $(_fname) => true, w)
    }
    return seen > 0 ? fnv_hex(acc) : ""
}

//! Where every run on this box keeps the runtime section it raced: `<dasllama home>/tune/box-<box key>.tune.json`, the
//! key this box's identity folded as the sidecar staleness rule folds it.
def public dasllama_box_runtime_path_from(dasllama_home : string) : string {
    var key : string
    static_if (typeinfo module_exists(llvm_tune)) {
        key = fnv_hex(box_match_key(tune_box_identity()))
    } else {
        key = "nobox"
    }
    return path_join(path_join(dasllama_home, "tune"), "box-{key}.tune.json")
}

def public dasllama_box_runtime_path() : string => dasllama_box_runtime_path_from(dasllama_home_dir())

//! True when the per-box runtime file at `path` is this box's, carries a runtime section, and was raced over the kernel
//! sources this process compiles (`digest`); anything else is another box's or another build's state.
def public dasllama_box_runtime_valid(path, digest : string) : bool {
    let text = fread(path)
    return false if (empty(text))
    var err = ""
    var doc = read_json(text, err)
    return false if (doc == null)
    let box : string = doc?["provenance"]?["box"] ?? ""
    let kl : string = doc?["kernel_library"] ?? ""
    let rt = doc?["runtime"]
    var same_box : bool
    static_if (typeinfo module_exists(llvm_tune)) {
        same_box = !empty(box) && box_match_key(box) == box_match_key(tune_box_identity())
    } else {
        same_box = false
    }
    let ok = same_box && kl == digest && rt != null && rt.value is _object
    delete_json(doc)
    return ok
}

//! Race the Metal twin crowns on this box and write the runtime section, the box identity and `digest` to `path` - the
//! per-box file every run on the box reads from then on. True when the file was written.
def public dasllama_box_runtime_race(path, digest : string) : bool {
    static_if (typeinfo module_exists(llvm_tune)) {
        let t0 = get_clock()
        to_log(LOG_INFO, "dasLLAMA: first run on this box with these kernels - racing the Metal twin crowns, kept in {path} for every run on it\n")
        tune_progress_plan("dasllama_box_start", 1)
        tune_progress_kernel_begin("metal_crowns", 0)
        var race <- dasllama_metal_crown_race()
        let crowns = join(race.wins, ",")
        tune_progress_kernel_end("metal_crowns", crowns, !race.ran ? "skipped" : (empty(crowns) ? "holds" : "beats"))
        tune_progress_finish()
        //! the crowns alone: the knobs a process holds at its first apply are its own, not the box's, so none of them ride here
        var doc = JV((kernel_library = digest, provenance = JV((box = tune_box_identity(),
            platform = get_platform_name(), arch = get_architecture_name())), runtime = JV((metal_tensor = crowns))))
        mkdir_rec(dir_name(path))
        let tmp = "{path}.{get_time_usec(0l)}.tmp"
        var wrote = fwrite(tmp, write_json(doc))
        if (wrote) {
            let moved = rename_result(tmp, path)   //! one rename: a reader never sees a half-written file
            wrote = moved is value && moved as value
            if (!wrote) {
                remove(tmp)
            }
        }
        delete_json(doc)
        let secs = get_clock() - t0
        if (wrote) {
            to_log(LOG_WARNING, "dasLLAMA: metal crowns raced for this box ({secs} s) -> {path}\n")
        } else {
            to_log(LOG_WARNING, "dasLLAMA: could not write {path} - the crowns hold for this process only\n")
            set_metal_tensor_crowns(crowns)
        }
        delete race
        return wrote
    } else {
        return false
    }
}

// once per process: an unwritable per-box file would otherwise race again on every apply
var private g_box_race_ran = false

//! The per-box runtime file a run that keeps no sidecar of its own reads: "" where this process keeps its manifest - a
//! fat exe (its own per-user sidecar), an explicit `DAS_TUNE_MANIFEST`, a standalone exe with its sidecar beside it, a
//! run on the reference or fallback kernel policy (the portable tier's arm, crownless by its own contract) - else the
//! file's path, raced and written first when none valid for these kernels exists.
[arch(at = "../ARCHITECTURE_MEASUREMENT_FAT_START.md#box-runtime-file")]
def public dasllama_box_runtime_file() : string {
    static_if (typeinfo module_exists(llvm_tune)) {
        return "" if (tune_fat_built() || tune_manifest_explicit() || is_standalone_exe())
        let policy = tune_policy_env()
        return "" if (policy == "reference" || policy == "fallback")
        let path = dasllama_box_runtime_path()
        let digest = dasllama_kernel_sources_digest()
        return path if (dasllama_box_runtime_valid(path, digest))
        return "" if (g_box_race_ran)
        g_box_race_ran = true
        return dasllama_box_runtime_race(path, digest) ? path : ""
    } else {
        return ""
    }
}

[init]
def private register_fat_start {
    set_runtime_race_hook(@@dasllama_fat_first_start)
    set_sidecar_place_hook(@@dasllama_place_fat_sidecar)
    set_box_runtime_hook(@@dasllama_box_runtime_file)
}
