options gen2
options persistent_heap
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama_transformer   // umbrella fires each arch [init] registration (qwen35)
require dasllama/dasllama
require dasllama/dasllama_common
require dasllama/dasllama_env
require dasllama/dasllama_image
require dasllama/dasllama_math
require dasllama/dasllama_gpu_tier
require dasllama/dasllama_gpu_resident
require dasllama/dasllama_blocks
require daslib/jobque_boost
require daslib/fio
require daslib/rtti   // this_context().last_exception: the refused load's text
require strings   // find: the refusal names the lane and the decline
require _model_tier
require _compares
require _resident_feed
require _moe_resident   // MOE_LOGIT_BAR_REL: the routed block's bar the MoE lanes' rows hold
require _vulkan_lane

// The vulkan lane's streaming mint on a LIVE tier (the dry twin is test_model_image_vulkan.das): a cold vulkan-armed
// load of the 0.8B-MTP mints the vulkan lane and no other, the trimmed lane under the lever serves the same greedy
// tokens off the same device twin and the same CPU embed rows off the packed emb region, and the trimmed lane maps
// back warm to the same tokens; the K-quant twin's lane mints off its native planes and maps back to the same tokens;
// and a trimmed lane minted under one K/V mirror codec maps under the others and serves each codec's own tokens; the trim
// admits what the whole-model driver serves: under a foreign prefill override both decline with one text, and a trimmed
// lane on disk maps under it as a lane minted again with its planar families.
// Skips when interpreted, without the model, with the image rail off (DASLLAMA_IMAGE=0), without a live armed tier,
// and when the whole-model driver declines the model on this box; the K/V codecs cell also skips where DASLLAMA_VK_KV32=1
// arms an f32 mirror over the codec it asks; the MoE lane's trimmed-rail assert skips under DASLLAMA_RARE_ARENA=0, which
// keeps every lane on the planar planes.

//! the forced-feed form a routed carrier's lanes are held to: the prompt's row and `MINT_STEPS` fed steps' rows on a session
//! of `tr`, the same tokens fed on every lane, so two lanes compare row by row at the routed block's bar with the row one
//! step off the control, where a greedy token can flip on a near-tie expert
def private feed_rows(tr : Model) : array<array<float>> {
    var out : array<array<float>>
    with_dasllama_jobque_() {
        var s <- create_session(tr, KVDtype.f16)
        var prompt <- [for (i in range64(int64(MINT_PROMPT))); 1000l + i * 7l]
        var toks <- [for (i in range64(int64(MINT_STEPS))); 2000l + i * 11l]
        out <- feed_logits(tr, s, prompt, toks) // nolint:PERF030 - fresh; queue runs once
        delete s
        delete prompt
        delete toks
    }
    return <- out
}

def private hold_feed_rows(t : T?; what : string; got, want : array<array<float>>) {
    rows_within_control(t, what, got, want, MOE_LOGIT_BAR_REL, 1l, 1.0, $(r) => r == 0l ? "the prompt's row" : "step {r}")
}

[test]
def test_vulkan_mint_lanes(tst : T?) {
    tst |> run("Qwen3.5-0.8B-MTP: a cold vulkan-armed load mints the vulkan lane alone; the trimmed lane serves the same tokens and embed rows, cold and warm") @(t : T?) {
        with_mint_file(t, "qwen35", MINT_FILE) $(path) {
            lanes_cell(t, path) $(_img) {}
        }
    }
}

let private MOE_FILE = "Qwen1.5-MoE-A2.7B-Chat-Q4_K_M-local.gguf"
let private MOE_HOST_LAYERS = 20l   //! routed layers asked onto the host: their stacks stream, the plan's room arms the unasked pool beside the mirror

//! the MoE lane under the trim: twenty routed layers' expert stacks streamed to the host's pinned mirrors, so the lane drops every
//! planar family and the host reads the mirrors through the rare rail; on the untrimmed lane the rail's arena arm pinned off
//! reads the planar planes (none through the rail) and pinned on - `set_rare_arena(1)`, the in-process twin of
//! DASLLAMA_RARE_ARENA=2 - reads the same picks off the mirrors through the rail, its rows within the routed block's bar of the
//! planar kernels'; the trimmed lane's rail follows the knob's default policy (the pin put back as found)
[test]
def test_vulkan_mint_moe_streamed(tst : T?) {
    tst |> run("Qwen1.5-MoE Q4_K_M with twenty routed layers streamed: the trimmed lane drops the planar families and serves the untrimmed lane's tokens, cold and warm, off the mirrors; the untrimmed lane's picks through the rail under the arena pin alone") @(t : T?) {
        return if (!family_on(t, "qwen2moe"))
        let path = path_join(models_dir(), MOE_FILE)
        return if (mint_skip(t, path))
        let levers = pin_lane_levers()
        let hl = gpu_host_layers_pin_()
        let pf0 = gpu_want_pf_stream_pin()
        let arena0 = rare_arena_pin()
        set_gpu_host_layers_(MOE_HOST_LAYERS)
        set_gpu_want_pf_stream(1)
        with_cold_lane(t, path, "the MoE") $(var plain; img_v) {
            t |> success(!plain.planes_trimmed && long_length(plain.vkplan) > 0l, "the served image is the vulkan lane, the planar families kept with the trim off")
            t |> equal(gpu_resident_host_layers(), MOE_HOST_LAYERS, "the first {MOE_HOST_LAYERS} routed layers' experts serve on the host")
            let plain_bytes = stat(img_v).size
            set_rare_arena(0)   // the arena arm pinned off: the host picks read the planar planes the untrimmed lane kept
            let r0 = g_rare_dec_rows
            var a <- witness_steps(t, plain, "the untrimmed lane")
            var a_rows <- feed_rows(plain)
            t |> equal(g_rare_dec_rows, r0, "set_rare_arena(0): the untrimmed lane's host picks read the planar planes the lane kept, none through the rail")
            set_rare_arena(1)   // the arm pinned on (DASLLAMA_RARE_ARENA=2's twin): the same picks read off the pinned mirrors through the rail
            let r_on = g_rare_dec_rows
            var arena_rows <- feed_rows(plain)
            t |> success(g_rare_dec_rows > r_on, "set_rare_arena(1): the untrimmed lane's host picks read {g_rare_dec_rows - r_on} pairs off the mirrors through the rail, the planar planes loaded beside them")
            hold_feed_rows(t, "the untrimmed lane's rows through the rail against the planar kernels'", arena_rows, a_rows)
            set_rare_arena(arena0)
            delete arena_rows
            delete plain
            moe_gpu_drop_model()
            set_vulkan_trim(true)
            remove(img_v)
            var trimmed <- load_cached_q8(path)
            t |> success(moe_gpu_resident_active(), "the trimmed lane is served by the whole-model driver")
            t |> equal(gpu_resident_host_layers(), MOE_HOST_LAYERS, "... with the same routed layers on the host")
            t |> success(trimmed.image_map != null && trimmed.planes_trimmed && long_length(trimmed.qblob) == 0l && long_length(trimmed.embq) > 0l,
                "the trimmed lane serves without the planar families, the emb region packed")
            t |> success(stat(img_v).size < plain_bytes, "the trimmed lane is the smaller file ({stat(img_v).size} against {plain_bytes} bytes)")
            let r1 = g_rare_dec_rows
            var b <- witness_steps(t, trimmed, "the trimmed lane")
            var b_rows <- feed_rows(trimmed)
            if (rare_arena_serves(trimmed)) {
                t |> success(g_rare_dec_rows > r1, "the trimmed lane's host picks read {g_rare_dec_rows - r1} pairs off the mirrors through the rail (DASLLAMA_RARE_ARENA={g_env_engine.rare_arena}, the pin {rare_arena_pin()})")
            } else {
                t |> skip("DASLLAMA_RARE_ARENA={g_env_engine.rare_arena} (the pin {rare_arena_pin()}) keeps the trimmed lane off the rail: the rail's assert needs the knob's default")
            }
            log_gen_texts(trimmed, "untrimmed lane / trimmed lane", a.ids, b.ids)
            hold_feed_rows(t, "the trimmed lane's rows against the untrimmed lane's", b_rows, a_rows)
            t |> equal(mismatch_exact(a.row, b.row), 0, "the CPU embed row off the packed emb region is the planar plane's, bit for bit")
            delete trimmed
            moe_gpu_drop_model()
            var warm <- load_cached_q8(path)
            t |> success(moe_gpu_resident_active() && warm.image_map != null && warm.planes_trimmed, "the trimmed lane maps back warm onto the driver")
            var c <- witness_steps(t, warm, "the warm lane")
            var c_rows <- feed_rows(warm)
            log_gen_texts(warm, "untrimmed lane / warm trimmed lane", a.ids, c.ids)
            hold_feed_rows(t, "the warm trimmed lane's rows against the untrimmed lane's", c_rows, a_rows)
            delete warm
            moe_gpu_drop_model()
            delete a
            delete b
            delete c
            delete a_rows
            delete b_rows
            delete c_rows
        }
        set_gpu_want_pf_stream(pf0)
        set_gpu_host_layers_(hl)
        restore_lane_levers(levers)
    }
}

[test]
def test_vulkan_mint_kquant(tst : T?) {
    tst |> run("Qwen3.5-0.8B-Q4_K_M: the K-quant twin's lane mints off its native planes and maps back warm to the same tokens") @(t : T?) {
        with_mint_file(t, "qwen35", KQ_FILE) $(path) {
            kquant_cell(t, path) $(_img) {}
        }
    }
}

let private TRIM_PROBE = "trim-probe"

//! the prefill override the trim cell selects to keep the whole-model driver off a load: it passes every call to the CPU layer loop
[unused_argument(tr, s, npos, start_pos)]
def private pass_prefill(tr : Model; var s : Session; npos : int64; start_pos : int64) : bool => false

[test]
def test_vulkan_mint_trim_follows_the_driver(tst : T?) {
    tst |> run("Qwen3.5-0.8B-MTP: the trim admits what the whole-model driver serves - a foreign prefill override declines both with one text, the trimmed lane on disk stays for the next run and this run serves planar from memory, a cold trim ask under the override writes the untrimmed lane the next load under it serves, and the load after the override lifts mints the trimmed lane over it") @(t : T?) {
        with_mint_file(t, "qwen35", MINT_FILE) $(path) {
            with_cold_lane(t, path, "the model") $(var plain; img_v) {
                t |> success(resident_would_serve(plain) && empty(resident_decline_reason(plain)), "the trim admits the model the driver serves")
                delete plain
                moe_gpu_drop_model()
                set_vulkan_trim(true)
                remove(img_v)
                var trimmed <- load_cached_q8(path)
                t |> success(trimmed.planes_trimmed && moe_gpu_resident_active(), "the trimmed lane mints, and the driver serves it")
                delete trimmed
                moe_gpu_drop_model()
                let lane_size = stat(img_v).size
                let override_found = active_prefill_override()
                register_prefill_override(TRIM_PROBE, @@pass_prefill)
                select_prefill_override(TRIM_PROBE)
                var staged <- load_model_(path, QuantMode.q8)
                let why = resident_decline_reason(staged)
                t |> success(!moe_gpu_resident_active(), "control: the foreign override keeps the driver off the model")
                t |> equal(why, "the '{TRIM_PROBE}' prefill override is active")
                t |> equal(gpu_resident_decline_(), why, "the driver's decline is the chain's text, byte for byte")
                t |> success(!resident_would_serve(staged) && resident_trim_decline_is_run_mode(resident_trim_decline(staged)), "the trim declines what the driver declines, as this run's decline")
                delete staged
                moe_gpu_drop_model()
                //! the override is the run's, not the model's: the trimmed lane stays on disk byte for byte, and this run's
                //! load serves planar from memory - no untrimmed lane is written under the trimmed identity
                var warm <- load_cached_q8(path)
                t |> success(!warm.planes_trimmed && long_length(warm.qblob) > 0l, "under the override the load serves planar from memory")
                t |> success(stat(img_v).is_valid && stat(img_v).size == lane_size, "the trimmed lane the run cannot serve stays on disk, its size as the mint left it")
                delete warm
                moe_gpu_drop_model()
                remove(img_v)
                var minted <- load_cached_q8(path)
                t |> success(!minted.planes_trimmed && long_length(minted.qblob) > 0l && stat(img_v).is_valid,
                    "a trim ask under the override with no trimmed lane on disk writes the untrimmed lane at the path: the streamed build serves planar from memory")
                delete minted
                moe_gpu_drop_model()
                var fallback <- load_cached_q8(path)
                t |> success(!fallback.planes_trimmed && long_length(fallback.qblob) > 0l && !moe_gpu_resident_active(), "the next load under the override serves the untrimmed lane")
                delete fallback
                moe_gpu_drop_model()
                t |> success(!stat(image_path_for(path, "", "q8")).is_valid, "control: the held mint cached no planar image either - the next load stays on the streaming arm, the one that trims")
                select_prefill_override(override_found)
                var again <- load_cached_q8(path)
                t |> success(again.planes_trimmed && moe_gpu_resident_active(), "with the override lifted the next load mints the trimmed lane over the untrimmed one and the driver serves it")
                delete again
                moe_gpu_drop_model()
            }
        }
    }
}

//! `path`'s cached load, "" where it served, else the text it panicked with
def private load_refusal(path : string) : string {
    var why = ""
    try {
        var m <- load_cached_q8(path)
        delete m
    } recover {
        why = this_context().last_exception
    }
    return why
}

[test]
def test_vulkan_mint_trim_refused_unarmed(tst : T?) {
    tst |> run("Qwen3.5-0.8B-MTP: a trimmed lane the whole-model driver does not arm for after the trim admitted it is refused at its load, naming the lane and the decline - never at a CPU forward; the same load with the driver armed serves") @(t : T?) {
        with_mint_file(t, "qwen35", MINT_FILE) $(path) {
            with_cold_lane(t, path, "the model") $(var plain; img_v) {
                delete plain
                moe_gpu_drop_model()
                set_vulkan_trim(true)
                remove(img_v)
                var trimmed <- load_cached_q8(path)
                t |> success(trimmed.planes_trimmed && moe_gpu_resident_active(), "the trimmed lane mints, and the driver serves it")
                delete trimmed
                moe_gpu_drop_model()
                let lane_size = stat(img_v).size
                let fault_was = resident_upload_fault()
                set_resident_upload_fault(true)
                let why = load_refusal(path)
                set_resident_upload_fault(fault_was)
                moe_gpu_drop_model()
                t |> success(find(why, img_v) >= 0 && find(why, RESIDENT_UPLOAD_FAULT) >= 0 && find(why, "planes-trimmed") >= 0,
                    "the load refuses the unarmed trimmed lane by name, with the driver's decline: {why}")
                t |> success(stat(img_v).is_valid && stat(img_v).size == lane_size, "the refused lane stays on disk as the mint left it")
                t |> equal(load_refusal(path), "", "control: the same load with the driver armed serves the lane")
                t |> success(moe_gpu_resident_active(), "... on the whole-model driver")
                moe_gpu_drop_model()
            }
        }
    }
}

[test]
def test_vulkan_mint_failed_write_leaves_nothing(tst : T?) {
    tst |> run("Qwen3.5-0.8B-MTP: a mint whose write fails writes no lane, leaves no temp file and still serves; the next load mints") @(t : T?) {
        with_mint_file(t, "qwen35", MINT_FILE) $(path) {
            with_cold_lane(t, path, "the model") $(var plain; img_v) {
                delete plain
                moe_gpu_drop_model()
                remove(img_v)
                let fault_found = get_image_write_fault()
                let hits_found = image_write_fault_hits()
                let tmps_found = image_tmp_count(img_v)
                set_image_write_fault(1ul << 20ul)
                var held <- load_cached_q8(path)
                set_image_write_fault(0ul)
                t |> success(image_write_fault_hits() != hits_found, "the seam failed a write of this load's mint")
                t |> success(!stat(img_v).is_valid, "the mint whose write failed published no lane")
                t |> equal(image_tmp_count(img_v), tmps_found, "and left no temp file of its own beside it")
                t |> success(held.image_map != null && !stat(image_path_for(path, "", "q8")).is_valid,
                    "the load serves the streamed build from memory - an image is in hand and neither lane's file is on disk")
                delete held
                moe_gpu_drop_model()
                remove(image_path_for(path, "", "q8"))
                var again <- load_cached_q8(path)
                t |> success(stat(img_v).is_valid && moe_gpu_resident_active(), "control: with the seam off the next load mints the lane and the driver serves it")
                delete again
                moe_gpu_drop_model()
                set_image_write_fault(fault_found)
            }
        }
    }
}

[test]
def test_vulkan_mint_kv_codecs(tst : T?) {
    tst |> run("Qwen3.5-0.8B-MTP: the trimmed lane minted under a q8_0 mirror maps under tq4 and f16 with no re-mint, and serves at each codec the tokens the staged load serves there") @(t : T?) {
        with_mint_file(t, "qwen35", MINT_FILE) $(path) {
            kv_codecs_cell(t, path) $(_img) {}
        }
    }
}
