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
require daslib/jobque_boost
require daslib/fio
require _model_tier
require _compares
require _resident_feed
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
// arms an f32 mirror over the codec it asks.

let private MINT_FILE = "Qwen3.5-0.8B-MTP-Q8_0.gguf"
let private KQ_FILE = "Qwen3.5-0.8B-Q4_K_M.gguf"
let private MINT_PROMPT = 16
let private MINT_STEPS = 12

//! `MINT_STEPS` greedy tokens over a counting prompt, and the CPU embed row of one token: what the two lanes must agree on
def private mint_witness(tr : Model; var ids : array<int64>; var row : array<float>; kv : KVDtype = KVDtype.f16) {
    with_dasllama_jobque_() {
        var s <- create_session(tr, kv)
        var prompt <- [for (i in range64(int64(MINT_PROMPT))); 1000l + i * 7l]
        delete ids
        ids <- generate(tr, s, prompt, MINT_PROMPT + MINT_STEPS)
        row |> resize(tr.config.dim)
        embed_row(tr, 1234l, unsafe(addr(row[0])))
        delete s
        delete prompt
    }
}

typedef private MintRun = tuple<ids : array<int64>; row : array<float>>

//! `mint_witness` over `tr` with its token count asserted: `what` generated the asked steps past the prompt
def private witness_steps(t : T?; tr : Model; what : string) : MintRun {
    var w : MintRun
    mint_witness(tr, w.ids, w.row)
    t |> equal(long_length(w.ids), int64(MINT_PROMPT + MINT_STEPS), "{what} generated the {MINT_STEPS} asked steps past the {MINT_PROMPT}-token prompt")
    return <- w
}

//! the box facts every cell here skips on; "" = the cell runs
def private mint_skip(t : T?; path : string) : bool {
    return true if (!jit_only(t))
    if (!g_env_engine.image) {
        t |> skip("the image rail is off (DASLLAMA_IMAGE=0) - the lane mint has nothing to write")
        return true
    }
    if (!model_available(t, path)) {
        return true
    }
    if (empty(moe_gpu_bake_tag())) {
        t |> skip("no armed vulkan tier (DASLLAMA_GPU=1 on a box with a device)")
        return true
    }
    return false
}

//! the lane cells' frame: the family gate and `file`'s skips, then `body` over the model's path under the pinned lane levers, put back after
def private with_mint_file(t : T?; file : string; body : block<(path : string) : void>) {
    return if (!family_on(t, "qwen35"))
    let path = path_join(models_dir(), file)
    return if (mint_skip(t, path))
    let levers = pin_lane_levers()
    invoke(body, path)
    restore_lane_levers(levers)
}

//! a cold vulkan-armed load of `path` with both lanes' images removed first, `body` over it and the vulkan image's path (the body
//! drops its models), the image removed after; a skip naming `what` where the whole-model driver did not take the model
def private with_cold_lane(t : T?; path, what : string; body : block<(var cold : Model; img_v : string) : void>) {
    let img_v = image_path_for(path, moe_gpu_bake_tag())
    let img_p = image_path_for(path, "", "q8")
    remove(img_v)
    remove(img_p)
    var cold <- load_cached_q8(path)
    if (moe_gpu_resident_active()) {
        t |> success(stat(img_v).is_valid && !stat(img_p).is_valid, "the cold load minted {what}'s vulkan lane and no planar lane")
        invoke(body, cold, img_v)
    } else {
        delete cold
        moe_gpu_drop_model()
        t |> skip("the whole-model driver did not take {what} on this box (the decline line above)")
    }
    remove(img_v)
}

[test]
def test_vulkan_mint_lanes(tst : T?) {
    tst |> run("Qwen3.5-0.8B-MTP: a cold vulkan-armed load mints the vulkan lane alone; the trimmed lane serves the same tokens and embed rows, cold and warm") @(t : T?) {
        with_mint_file(t, MINT_FILE) $(path) {
            with_cold_lane(t, path, "the model") $(var plain; img_v) {
                t |> success(plain.image_map != null && long_length(plain.vkplan) > 0l && long_length(plain.qblob) > 0l, "the served image is the vulkan lane, the planar families kept with the trim off")
                let plain_bytes = stat(img_v).size
                var a <- witness_steps(t, plain, "the untrimmed lane")
                delete plain
                moe_gpu_drop_model()
                set_vulkan_trim(true)
                remove(img_v)
                var trimmed <- load_cached_q8(path)
                t |> success(moe_gpu_resident_active(), "the trimmed lane is served by the whole-model driver")
                t |> success(trimmed.image_map != null && trimmed.planes_trimmed && long_length(trimmed.qblob) == 0l && long_length(trimmed.embq) > 0l,
                    "the trimmed lane serves without the planar families, the emb region packed")
                t |> success(stat(img_v).size < plain_bytes, "the trimmed lane is the smaller file ({stat(img_v).size} against {plain_bytes} bytes)")
                var b <- witness_steps(t, trimmed, "the trimmed lane")
                log_gen_texts(trimmed, "untrimmed lane / trimmed lane", a.ids, b.ids)
                t |> equal(mismatch_exact(a.ids, b.ids), 0, "the trimmed lane's greedy tokens are the untrimmed lane's, token for token (both streams decoded above)")
                t |> equal(mismatch_exact(a.row, b.row), 0, "the CPU embed row off the packed emb region is the planar plane's, bit for bit")
                delete trimmed
                moe_gpu_drop_model()
                var warm <- load_cached_q8(path)
                t |> success(moe_gpu_resident_active(), "the warm trimmed lane is served by the whole-model driver")
                t |> success(warm.image_map != null && warm.planes_trimmed, "the trimmed lane maps back warm")
                var c <- witness_steps(t, warm, "the warm lane")
                log_gen_texts(warm, "untrimmed lane / warm trimmed lane", a.ids, c.ids)
                t |> equal(mismatch_exact(a.ids, c.ids), 0, "... and serves the same tokens (both streams decoded above)")
                delete warm
                moe_gpu_drop_model()
                delete a
                delete b
                delete c
            }
        }
    }
}

[test]
def test_vulkan_mint_kquant(tst : T?) {
    tst |> run("Qwen3.5-0.8B-Q4_K_M: the K-quant twin's lane mints off its native planes and maps back warm to the same tokens") @(t : T?) {
        with_mint_file(t, KQ_FILE) $(path) {
            with_cold_lane(t, path, "the K-quant twin") $(var cold; _img_v) {
                t |> success(cold.kq_repacked, "the loader kept the K-quant planes native and froze their interleave (the job view copies it)")
                t |> success(long_length(cold.vkplan) > 0l, "the K-quant model's cold lane carries the plan")
                var a <- witness_steps(t, cold, "the cold lane")
                delete cold
                moe_gpu_drop_model()
                var warm <- load_cached_q8(path)
                t |> success(moe_gpu_resident_active() && warm.image_map != null, "the lane maps back warm onto the whole-model driver")
                var b <- witness_steps(t, warm, "the warm lane")
                log_gen_texts(warm, "cold mint / warm map", a.ids, b.ids)
                t |> equal(mismatch_exact(a.ids, b.ids), 0, "the mapped twin serves the mint's tokens, token for token (both streams decoded above)")
                t |> equal(mismatch_exact(a.row, b.row), 0, "the CPU embed row is the same on both loads, bit for bit")
                delete warm
                moe_gpu_drop_model()
                delete a
                delete b
            }
        }
    }
}

let private TRIM_PROBE = "trim-probe"

//! the prefill override the trim cell selects to keep the whole-model driver off a load: it passes every call to the CPU layer loop
[unused_argument(tr, s, npos, start_pos)]
def private pass_prefill(tr : Model; var s : Session; npos : int64; start_pos : int64) : bool => false

[test]
def test_vulkan_mint_trim_follows_the_driver(tst : T?) {
    tst |> run("Qwen3.5-0.8B-MTP: the trim admits what the whole-model driver serves - a foreign prefill override declines both with one text, the trimmed lane on disk stays for the next run and this run serves planar from memory, and a lane minted under the override is no lane at all") @(t : T?) {
        with_mint_file(t, MINT_FILE) $(path) {
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
                t |> success(!minted.planes_trimmed && long_length(minted.qblob) > 0l && !stat(img_v).is_valid,
                    "a trim ask under the override writes no lane: the streamed build serves planar from memory and nothing lands under the trimmed identity")
                delete minted
                moe_gpu_drop_model()
                select_prefill_override(override_found)
                var again <- load_cached_q8(path)
                t |> success(again.planes_trimmed && moe_gpu_resident_active(), "with the override lifted the next load mints the trimmed lane and the driver serves it")
                delete again
                moe_gpu_drop_model()
            }
        }
    }
}

//! the greedy tokens a staged (no image) load of `path` serves on the device at mirror codec `kv`: what a lane serves at that codec
[cold_path]
def private staged_tokens_at(t : T?; path : string; kv : KVDtype) : array<int64> {
    var m <- load_resident_at(path, kv)
    var ids : array<int64>
    if (moe_gpu_resident_active()) {   // no driver, no tokens: the caller skips
        t |> equal(mirror_kv_dtype(), kv, "the staged load arms the mirror at the asked codec")
        var row : array<float>
        mint_witness(m, ids, row, kv)
        delete row
    }
    delete m
    moe_gpu_drop_model()
    return <- ids
}

[test]
def test_vulkan_mint_kv_codecs(tst : T?) {
    tst |> run("Qwen3.5-0.8B-MTP: the trimmed lane minted under a q8_0 mirror maps under tq4 and f16 with no re-mint, and serves at each codec the tokens the staged load serves there") @(t : T?) {
        with_mint_file(t, MINT_FILE) $(path) {
            let codecs = fixed_array(KVDtype.q8_0, KVDtype.tq4, KVDtype.f16)   // the mint's codec first
            with_gpu_kv_dtype(t, codecs[0]) {
                var want <- [for (kv in codecs); staged_tokens_at(t, path, kv)]
                if (empty(want[0])) {
                    t |> skip("the whole-model driver did not take the model on this box (the decline line above)")
                } else {
                    set_vulkan_trim(true)
                    let img_t = image_path_for(path, moe_gpu_bake_tag())
                    remove(img_t)
                    var minted_size = 0ul
                    var minted_at = 0l
                    for (kv, ids_want, i in codecs, want, count()) {
                        set_gpu_kv_dtype(kv)
                        var m <- load_cached_q8(path)
                        t |> success(moe_gpu_resident_active() && mirror_kv_dtype() == kv, "{kv}: the lane is served by the whole-model driver at the asked mirror codec")
                        t |> success(m.image_map != null && m.planes_trimmed, "{kv}: the served image is the trimmed lane")
                        let st = stat(img_t)
                        if (i == 0) {
                            minted_size = st.size
                            minted_at = int64(st.mtime)
                            t |> success(st.is_valid, "the cold load under {kv} minted the trimmed lane")
                        } else {
                            t |> success(st.size == minted_size && int64(st.mtime) == minted_at, "{kv}: the lane minted under {codecs[0]} mapped as it was - no re-mint ({st.size} bytes)")
                        }
                        var ids : array<int64>
                        var row : array<float>
                        mint_witness(m, ids, row, kv)   // a trimmed lane panics on a call passed to the CPU rails: generating is the served witness
                        log_gen_texts(m, "{kv}: the staged load / the trimmed lane", ids_want, ids)
                        t |> equal(mismatch_exact(ids, ids_want), 0, "{kv}: the trimmed lane's greedy tokens are the staged load's at that codec, token for token")
                        delete ids
                        delete row
                        delete m
                        moe_gpu_drop_model()
                    }
                    t |> success(mismatch_exact(want[0], want[2]) > 0 || mismatch_exact(want[1], want[2]) > 0, "control: a block codec moves the greedy tokens off the f16 mirror's on this prompt, so the per-codec compares tell the codecs apart")
                    remove(img_t)
                }
                delete want
            }
        }
    }
}
