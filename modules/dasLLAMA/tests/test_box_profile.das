options gen2

require dastest/testing_boost public
require dasllama/dasllama_common
require dasllama/dasllama_math  // the knob setters/getters (incl. set/get_q8_token_block) + the backend registry (engine-level, not re-exported)
require daslib/fio

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
    if (!has_env_variable("DAS_JOBQUE_TEAM_RANK_GATE")) {
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

    // "threads" is advisory (logged) — and must NOT call get_total_hw_jobs(), which throws when no
    // job queue exists (model loads may legally run outside with_job_que)
    t |> success(fwrite(path, "\{\"runtime\":\{\"threads\":8\}\}"), "threads-advisory profile written")
    apply_box_profile_runtime(path)
    t |> equal(get_q8_token_block(), 128l, "threads key applies no knob")
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
