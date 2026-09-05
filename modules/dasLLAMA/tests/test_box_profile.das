options gen2
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama_common
require dasllama/dasllama_math  // the knob setters/getters (incl. set/get_q8_token_block) + the backend registry (engine-level, not re-exported)
require daslib/fio
require dasllama/dasllama_env
require dasllama/dasllama_tune   // box_profile_verdict_at
require llvm/daslib/llvm_tune    // tune_box_identity

// The tune sidecar's "runtime" section: apply_box_profile_runtime(path) sets the runtime knobs
// and logs each applied entry. The explicit-path form keeps the test independent of the app's
// real sidecar (<app>.tune.json / DAS_TUNE_MANIFEST); knobs are restored after each case.

def private restore_defaults() {
    set_q8_token_block(128l)
    set_q8_l2_budget(4l * 1024l * 1024l)
    set_target_chunk_work(1_000_000l)
    set_gemv_lane_cap(0)
    set_batch_lane_cap(0)
    set_attn_par_threshold(100_000l)
    set_requant_par_threshold(256_000l)
    set_norm_par_threshold(256_000l)
    set_rope_par_threshold(100_000l)
    set_kv_store_par_threshold(256_000l)
    set_act_par_threshold(100_000l)
    set_q8_chunks_per_job(2)
    set_q4_chunks_per_job(4)
    set_q8_batch_chunks_per_job(4)
    set_jobque_spin_us(30000l)
    set_jobque_join_poll(50l)
    set_team_rank_gate(-1)
    clear_mtp_depth()
    set_mtp_depth_nextn(1l)
    set_mtp_depth_assistant(1l)
    set_mtp_round_kind(MtpRoundKind.nextn)
}

[test]
def test_runtime_section_applies(t : T?) {
    var terr = ""
    let dir = temp_directory(terr)
    t |> success(terr == "", "temp_directory resolves")
    let path = path_join(dir, "dasllama_test_box_profile.json")
    let js = ("\{\"dot\":\"vec8_u2\",\"runtime\":\{\"q8_token_block\":256,\"q8_l2_budget\":8388608,"
        + "\"target_chunk_work\":500000,\"gemv_lane_cap\":12,\"batch_lane_cap\":24,"
        + "\"attn_par_threshold\":50000,\"requant_par_threshold\":128000,"
        + "\"norm_par_threshold\":128000,\"rope_par_threshold\":50000,\"kv_store_par_threshold\":128000,"
        + "\"act_par_threshold\":50000,\"q8_chunks_per_job\":3,\"q4_chunks_per_job\":5,"
        + "\"q8_batch_chunks_per_job\":2,\"jobque_spin_us\":1000,\"jobque_join_poll\":10,"
        + "\"team_rank_gate\":1\}\}")
    t |> success(fwrite(path, js), "profile written")
    apply_box_profile_runtime(path)
    t |> equal(get_q8_token_block(), 256l, "q8_token_block applied")
    t |> equal(get_q8_l2_budget(), 8388608l, "q8_l2_budget applied")
    t |> equal(get_target_chunk_work(), 500000l, "target_chunk_work applied")
    t |> equal(get_gemv_lane_cap(), 12, "gemv_lane_cap applied")
    t |> equal(get_batch_lane_cap(), 24, "batch_lane_cap applied")
    t |> equal(get_attn_par_threshold(), 50000l, "attn_par_threshold applied")
    t |> equal(get_requant_par_threshold(), 128000l, "requant_par_threshold applied")
    t |> equal(get_norm_par_threshold(), 128000l, "norm_par_threshold applied")
    t |> equal(get_rope_par_threshold(), 50000l, "rope_par_threshold applied")
    t |> equal(get_kv_store_par_threshold(), 128000l, "kv_store_par_threshold applied")
    t |> equal(get_act_par_threshold(), 50000l, "act_par_threshold applied")
    t |> equal(get_q8_chunks_per_job(), 3, "q8_chunks_per_job applied")
    t |> equal(get_q4_chunks_per_job(), 5, "q4_chunks_per_job applied")
    t |> equal(get_q8_batch_chunks_per_job(), 2, "q8_batch_chunks_per_job applied")
    t |> equal(get_jobque_spin_us(), 1000l, "jobque_spin_us stored")
    t |> equal(get_jobque_join_poll(), 10l, "jobque_join_poll stored")
    t |> equal(get_team_rank_gate(), 1, "team_rank_gate stored")
    remove(path)
    restore_defaults()
}

[test]
def test_team_rank_gate_tristate(t : T?) {
    restore_defaults()
    var terr = ""
    let dir = temp_directory(terr)
    let path = path_join(dir, "dasllama_test_box_profile_gate.json")

    // 0 = force off is a real value (apply_i64_knob fires on v >= 0)
    t |> success(fwrite(path, "\{\"runtime\":\{\"team_rank_gate\":0\}\}"), "gate-off profile written")
    apply_box_profile_runtime(path)
    t |> equal(get_team_rank_gate(), 0, "profile 0 stored (force off)")

    // negative profile value = unset (skipped, same as absent) — the tuner writes -1 on unset boxes
    set_team_rank_gate(-1)
    t |> success(fwrite(path, "\{\"runtime\":\{\"team_rank_gate\":-1\}\}"), "unset profile written")
    apply_box_profile_runtime(path)
    t |> equal(get_team_rank_gate(), -1, "negative profile value skipped (stays unset)")

    // setup_dasllama_jobque_ lands the stored knob on the live que — unless the env override is
    // present (the sweep rigs' A/B rail; also how the gate-forced suite run reaches this test)
    if (is_none(g_env_core.jobque_team_rank_gate)) {
        with_job_que() {
            set_team_rank_gate(1)
            setup_dasllama_jobque_()
            t |> success(get_jobque_team_rank_gate(), "profile 1 lands on the que")
            set_team_rank_gate(0)
            setup_dasllama_jobque_()
            t |> success(!get_jobque_team_rank_gate(), "profile 0 lands on the que")
            set_jobque_team_rank_gate(true)   // unset must leave whatever the que already has
            set_team_rank_gate(-1)
            setup_dasllama_jobque_()
            t |> success(get_jobque_team_rank_gate(), "unset leaves the que state alone")
        }
    }
    remove(path)
    restore_defaults()
}

[test]
def test_partial_and_missing_are_no_ops(t : T?) {
    restore_defaults()
    var terr = ""
    let dir = temp_directory(terr)
    let path = path_join(dir, "dasllama_test_box_profile_partial.json")

    // only one runtime key set — everything else keeps its default
    t |> success(fwrite(path, "\{\"runtime\":\{\"q8_token_block\":512\}\}"), "partial profile written")
    apply_box_profile_runtime(path)
    t |> equal(get_q8_token_block(), 512l, "the one present key applied")
    t |> equal(get_target_chunk_work(), 1_000_000l, "absent keys keep defaults")
    restore_defaults()

    // kernel-perms-only profile (no "runtime" section) — a no-op for the runtime knobs
    t |> success(fwrite(path, "\{\"dot\":\"vec8_u2\"\}"), "flat profile written")
    apply_box_profile_runtime(path)
    t |> equal(get_q8_token_block(), 128l, "no runtime section = no-op")

    // missing file — silent no-op
    remove(path)
    apply_box_profile_runtime(path)
    t |> equal(get_q8_token_block(), 128l, "missing file = no-op")

    // invalid JSON — warns, applies nothing
    t |> success(fwrite(path, "not json"), "garbage written")
    apply_box_profile_runtime(path)
    t |> equal(get_q8_token_block(), 128l, "invalid JSON = no-op")

    // "threads" is a logged receipt — and must NOT call get_total_hw_jobs(), which throws when no
    // job queue exists (model loads may legally run outside with_job_que)
    t |> success(fwrite(path, "\{\"runtime\":\{\"threads\":8\}\}"), "threads-advisory profile written")
    apply_box_profile_runtime(path)
    t |> equal(get_q8_token_block(), 128l, "threads key applies no knob")
    remove(path)
}

[test]
def test_jobque_pool_knobs_apply(t : T?) {
    //! these knobs declare the NEXT que, never a live one - the [init] tier heuristic seeds
    //! them and a profile overrides
    let saved_pool = get_jobque_threads()
    let saved_decode = get_phase_decode_workers()
    var terr = ""
    let dir = temp_directory(terr)
    let path = path_join(dir, "dasllama_test_box_profile_pool.json")
    t |> success(fwrite(path, "\{\"runtime\":\{\"jobque_pool\":14,\"phase_decode_workers\":9\}\}"), "pool profile written")
    apply_box_profile_runtime(path)
    t |> equal(get_jobque_threads(), 14, "jobque_pool applied")
    t |> equal(get_phase_decode_workers(), 9, "phase_decode_workers applied")
    set_jobque_threads(saved_pool)
    set_phase_decode_workers(saved_decode)
    remove(path)
}

// a sidecar minted before the hybrid split spells "uncapped" as gemv_lane_cap 0; on a slow-tier
// box that value would hand the GEMV every lane, so the apply REFUSES it and the seeded
// fast-tier cap stands. Any other value still applies.
[test]
def test_gemv_lane_cap_zero_refused_on_hybrid(t : T?) {
    if (!is_slow_tier_compute()) {
        t |> skip("homogeneous box - a 0 cap is the honest uncapped value here, nothing to refuse")
        return
    }
    restore_defaults()
    var terr = ""
    let dir = temp_directory(terr)
    let path = path_join(dir, "dasllama_test_box_profile_gemv0.json")
    set_gemv_lane_cap(6)
    t |> success(fwrite(path, "\{\"runtime\":\{\"gemv_lane_cap\":0\}\}"), "uncapped profile written")
    apply_box_profile_runtime(path)
    t |> equal(get_gemv_lane_cap(), 6, "gemv_lane_cap 0 refused on a hybrid box - the seeded cap stands")
    t |> success(fwrite(path, "\{\"runtime\":\{\"gemv_lane_cap\":9\}\}"), "capped profile written")
    apply_box_profile_runtime(path)
    t |> equal(get_gemv_lane_cap(), 9, "a nonzero cap from the same profile shape still applies")
    remove(path)
    restore_defaults()
}

[test, arch(at="../ARCHITECTURE_RUNTIME.md#hybrid-pool-policy")]
def test_slow_tier_kind_contract(t : T?) {
    if (is_slow_tier_compute()) {
        t |> success(get_total_perf_cores() > 0, "compute slow tier implies known perf-core topology")
        t |> success(get_total_perf_cores() < get_total_hw_cores(), "compute slow tier implies a genuine second tier")
    } else {
        t |> success(true, "homogeneous/Efficiency/unknown topology: extension stays off")
    }
}

[test]
def test_metal_prefill_knobs_apply(t : T?) {
    var terr = ""
    let dir = temp_directory(terr)
    let path = path_join(dir, "dasllama_test_box_profile_metal.json")
    t |> success(fwrite(path, ("\{\"runtime\":\{\"metal_tall_floor\":96,"
        + "\"metal_devw_small_panel_mb\":24,\"metal_cvt_min_rows\":320\}\}")), "metal-knob profile written")
    apply_box_profile_runtime(path)
    t |> equal(metal_tall_floor(), 96l, "metal_tall_floor applied")
    t |> equal(metal_devw_small_panel_mb(), 24l, "metal_devw_small_panel_mb applied")
    t |> equal(metal_cvt_min_rows(), 320l, "metal_cvt_min_rows applied")

    // out-of-range values are refused with a warning — the stored value stands
    t |> success(fwrite(path, ("\{\"runtime\":\{\"metal_tall_floor\":0,"
        + "\"metal_devw_small_panel_mb\":0,\"metal_cvt_min_rows\":0\}\}")), "zero-knob profile written")
    apply_box_profile_runtime(path)
    t |> equal(metal_tall_floor(), 96l, "tall_floor 0 refused (the measured-bad unfloored config)")
    t |> equal(metal_devw_small_panel_mb(), 24l, "devw panel 0 refused")
    t |> equal(metal_cvt_min_rows(), 320l, "cvt_min_rows 0 refused (a numerics form flip from JSON)")

    set_metal_tall_floor(64l)
    set_metal_devw_small_panel_mb(32l)
    set_metal_cvt_min_rows(256l)
    remove(path)
}

[test]
def test_backend_pin_applies(t : T?) {
    var terr = ""
    let dir = temp_directory(terr)
    let path = path_join(dir, "dasllama_test_box_profile_pin.json")
    let before = active_kernel_backend()

    // unknown backend name: pin is rejected (warn + keep) — active backend unchanged
    t |> success(fwrite(path, "\{\"runtime\":\{\"backend\":\"no-such-backend\"\}\}"), "bad-pin profile written")
    apply_box_profile_runtime(path)
    t |> equal(active_kernel_backend(), before, "unknown pin keeps the active backend")

    // the portable backend always registers, so pinning it must take everywhere
    t |> success(fwrite(path, "\{\"runtime\":\{\"backend\":\"portable\"\}\}"), "portable-pin profile written")
    apply_box_profile_runtime(path)
    t |> equal(active_kernel_backend(), "portable", "known pin applies")
    clear_kernel_backend_pin()
    select_kernel_backend(before)   // restore for any later test in this process
    remove(path)
}

// The checked route (what the no-path apply takes): a sidecar older than the binary keeps its
// runtime section - the knobs are this box's state and only the kernel winners died with the
// rebuild - while another box's sidecar applies nothing.
[test]
def test_stale_binary_sidecar_keeps_runtime(t : T?) {
    var terr = ""
    let dir = temp_directory(terr)
    t |> success(terr == "", "temp_directory resolves")
    let path = path_join(dir, "dasllama_test_box_profile_stale.json")
    let js = "\{\"kernels\":\{\},\"runtime\":\{\"q8_token_block\":320\},\"provenance\":\{\"box\":\"{tune_box_identity()}\"\}\}"
    t |> success(fwrite(path, js), "profile written")
    var err : string
    t |> success(set_mtime(path, mktime(2020, 1, 1, 0, 0, 0), err), "mtime set into the past ({err})")
    t |> equal(box_profile_verdict_at(path), "stale_binary", "reads as binary-stale")
    apply_box_profile_runtime_checked(path)
    t |> equal(get_q8_token_block(), 320l, "runtime knob applied off a binary-stale sidecar")
    t |> success(box_profile_runtime_applied(), "the applied flag is up")
    remove(path)
    restore_defaults()
}

[test]
def test_foreign_sidecar_applies_nothing(t : T?) {
    var terr = ""
    let dir = temp_directory(terr)
    t |> success(terr == "", "temp_directory resolves")
    let path = path_join(dir, "dasllama_test_box_profile_foreign.json")
    let js = "\{\"kernels\":\{\},\"runtime\":\{\"q8_token_block\":320\},\"provenance\":\{\"box\":\"dasllama-test-foreign-box\"\}\}"
    t |> success(fwrite(path, js), "profile written")
    t |> equal(box_profile_verdict_at(path), "foreign_box", "reads as another box's")
    apply_box_profile_runtime_checked(path)
    t |> equal(get_q8_token_block(), 128l, "runtime knob NOT applied off another box's sidecar")
    t |> equal(box_profile_runtime_declined(), "foreign_box", "the decline names its reason")
    remove(path)
    restore_defaults()
}

[test]
def test_kernels_only_sidecar_declines_as_no_runtime(t : T?) {
    var terr = ""
    let dir = temp_directory(terr)
    t |> success(terr == "", "temp_directory resolves")
    let path = path_join(dir, "dasllama_test_box_profile_kernels_only.json")
    let js = "\{\"kernels\":\{\"dot\":\"vec8_u2\"\},\"provenance\":\{\"box\":\"{tune_box_identity()}\"\}\}"
    t |> success(fwrite(path, js), "profile written")
    apply_box_profile_runtime_checked(path)
    t |> equal(box_profile_runtime_declined(), "no_runtime_section", "a fallback-stamp sidecar declines by name")
    remove(path)
    restore_defaults()
}

[test]
def test_apply_status_describes_the_last_checked_apply(t : T?) {
    var terr = ""
    let dir = temp_directory(terr)
    t |> success(terr == "", "temp_directory resolves")
    let foreign = path_join(dir, "dasllama_test_box_profile_last_foreign.json")
    let own = path_join(dir, "dasllama_test_box_profile_last_own.json")
    t |> success(fwrite(foreign, "\{\"kernels\":\{\},\"runtime\":\{\"q8_token_block\":320\},\"provenance\":\{\"box\":\"dasllama-test-foreign-box\"\}\}"), "foreign profile written")
    t |> success(fwrite(own, "\{\"kernels\":\{\},\"runtime\":\{\"q8_token_block\":320\},\"provenance\":\{\"box\":\"{tune_box_identity()}\"\}\}"), "own profile written")
    apply_box_profile_runtime_checked(foreign)
    t |> equal(box_profile_runtime_declined(), "foreign_box", "the first apply declines")
    apply_box_profile_runtime_checked(own)
    t |> success(box_profile_runtime_applied(), "the second apply applies")
    t |> equal(box_profile_runtime_declined(), "", "a later apply clears the earlier decline reason")
    apply_box_profile_runtime_checked(foreign)
    t |> success(!box_profile_runtime_applied(), "a later decline lowers the applied flag")
    t |> equal(box_profile_runtime_declined(), "foreign_box", "and names its reason")
    remove(foreign)
    remove(own)
    restore_defaults()
}

// The speculative depth: the box knobs decide per round kind until an explicit set wins.
[test]
def test_mtp_depth_knobs_apply_per_round_kind(t : T?) {
    var terr = ""
    let dir = temp_directory(terr)
    t |> success(terr == "", "temp_directory resolves")
    let path = path_join(dir, "dasllama_test_box_profile_mtp.json")
    let js = "\{\"runtime\":\{\"mtp_depth_nextn\":1,\"mtp_depth_assistant\":2\}\}"
    t |> success(fwrite(path, js), "profile written")
    apply_box_profile_runtime(path)
    t |> equal(get_mtp_depth_assistant(), 2l, "assistant knob applied")
    t |> equal(get_mtp_depth_nextn(), 1l, "nextn knob applied")
    set_mtp_round_kind(MtpRoundKind.nextn)
    t |> equal(get_mtp_depth(), 1l, "a NextN round drafts the nextn depth")
    set_mtp_round_kind(MtpRoundKind.assistant)
    t |> equal(get_mtp_depth(), 2l, "an assistant round drafts the assistant depth")
    set_mtp_depth(3l)
    t |> equal(get_mtp_depth(), 3l, "an explicit depth wins over the knobs")
    clear_mtp_depth()
    t |> equal(get_mtp_depth(), 2l, "clearing the explicit depth hands the knobs back")
    remove(path)
    restore_defaults()
}
