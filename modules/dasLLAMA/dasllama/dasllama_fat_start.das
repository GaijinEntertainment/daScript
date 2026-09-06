options gen2
options indenting = 4
options _dasllama_internal = true

module dasllama_fat_start shared private

require dasllama/dasllama_lint public
require dasllama/dasllama_common
require dasllama/dasllama_math   // the runtime knob getters the snapshot records
require dasllama/dasllama_par    // get_total_hw_jobs - the advisory "threads" entry
require daslib/json_boost
require daslib/strings_boost
require ?llvm llvm/daslib/llvm_tune
require ?das_metal dasllama/dasllama_metal_prefill  // metal_tensor_race (Apple static_if half)
require ?das_metal dasllama/dasllama_metal_kernels  // metal_tensor_race_decode (Apple static_if half)

//! The runtime section of the app sidecar, and the races that fill it without a tuner: the mint
//! writes it at the end of the kernel half, and a fat exe - which ships no tuner and no policy
//! rail - races its Metal twins once at first start and writes the same section beside itself.
//! Nothing here loads a model or spawns a child; every confirm stays in the harness.

//! The `"runtime"` section as a mint or a first-start race writes it: the CURRENT runtime knobs
//! (defaults unless this process changed them), so the file is complete and hand-editable, plus
//! the Metal twin crowns. TB/L2-budget refinement stays manual (`harness/tune_tb.das` shows the
//! cliff; edit under the knee); `threads` is advisory - `DAS_JOBQUE_THREADS` changes it.
def public dasllama_runtime_snapshot(metal_crowns : string) : JsonValue? {
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
        jobque_spin_us = get_jobque_spin_us(), jobque_join_poll = get_jobque_join_poll(),
        threads = get_total_hw_jobs() + 1, metal_tensor = metal_crowns,   // total lanes (workers + computing main), the DAS_JOBQUE_THREADS unit
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
[arch(at = "../ARCHITECTURE_MEASUREMENT.md#fat-first-start")]
def public dasllama_race_runtime_section() : bool {
    static_if (typeinfo module_exists(llvm_tune)) {
        let path = tune_manifest_path()
        to_log(LOG_INFO, "dasLLAMA: first start on this box - racing the Metal twin kernels; the winners are kept in {path} and the next start reads them\n")
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

[init]
def private register_fat_start {
    set_runtime_race_hook(@@dasllama_fat_first_start)
}
