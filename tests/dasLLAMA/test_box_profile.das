options gen2

require dastest/testing_boost public
require dasllama/dasllama_common
require dasllama/dasllama_math  // the knob setters/getters + the backend registry (engine-level, not re-exported)
require dasllama/dasllama_math_aarch64_neon  // set/get_q8_token_block
require daslib/fio

// box_profile.json "runtime" section: apply_box_profile_runtime(path) sets the runtime knobs and
// logs each applied entry. The explicit-path form keeps the test independent of the box's real
// profile (DASLLAMA_BOX_PROFILE / <das_root>/box_profile.json); knobs are restored after each case.

def private restore_defaults() {
    set_q8_token_block(128l)
    set_q8_l2_budget(4l * 1024l * 1024l)
    set_matmul_par_threshold(2_000_000l)
    set_attn_par_threshold(100_000l)
    set_decode_attn_par_threshold(262_144l)
    set_requant_par_threshold(256_000l)
    set_norm_par_threshold(256_000l)
    set_rope_par_threshold(100_000l)
    set_kv_store_par_threshold(256_000l)
    set_act_par_threshold(100_000l)
    set_q8_chunks_per_job(2)
    set_q4_chunks_per_job(4)
}

[test]
def test_runtime_section_applies(t : T?) {
    var terr = ""
    let dir = temp_directory(terr)
    t |> success(terr == "", "temp_directory resolves")
    let path = path_join(dir, "dasllama_test_box_profile.json")
    let js = ("\{\"dot\":\"vec8_u2\",\"runtime\":\{\"q8_token_block\":256,\"q8_l2_budget\":8388608,"
        + "\"matmul_par_threshold\":1000000,\"attn_par_threshold\":50000,\"decode_attn_par_threshold\":2000000,"
        + "\"requant_par_threshold\":128000,"
        + "\"norm_par_threshold\":128000,\"rope_par_threshold\":50000,\"kv_store_par_threshold\":128000,"
        + "\"act_par_threshold\":50000,\"q8_chunks_per_job\":3,\"q4_chunks_per_job\":5\}\}")
    t |> success(fwrite(path, js), "profile written")
    apply_box_profile_runtime(path)
    t |> equal(get_q8_token_block(), 256l, "q8_token_block applied")
    t |> equal(get_q8_l2_budget(), 8388608l, "q8_l2_budget applied")
    t |> equal(get_matmul_par_threshold(), 1000000l, "matmul_par_threshold applied")
    t |> equal(get_attn_par_threshold(), 50000l, "attn_par_threshold applied")
    t |> equal(get_decode_attn_par_threshold(), 2000000l, "decode_attn_par_threshold applied")
    t |> equal(get_requant_par_threshold(), 128000l, "requant_par_threshold applied")
    t |> equal(get_norm_par_threshold(), 128000l, "norm_par_threshold applied")
    t |> equal(get_rope_par_threshold(), 50000l, "rope_par_threshold applied")
    t |> equal(get_kv_store_par_threshold(), 128000l, "kv_store_par_threshold applied")
    t |> equal(get_act_par_threshold(), 50000l, "act_par_threshold applied")
    t |> equal(get_q8_chunks_per_job(), 3, "q8_chunks_per_job applied")
    t |> equal(get_q4_chunks_per_job(), 5, "q4_chunks_per_job applied")
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
    t |> equal(get_matmul_par_threshold(), 2_000_000l, "absent keys keep defaults")
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
    // job queue exists (load_model may legally run outside with_job_que)
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
