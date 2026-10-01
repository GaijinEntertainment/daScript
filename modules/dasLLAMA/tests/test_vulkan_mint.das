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

// The vulkan lane's streaming mint on a LIVE tier (the dry twin is test_model_image_vulkan.das): a cold vulkan-armed
// load of the 0.8B-MTP mints the vulkan lane and no other, the trimmed lane under the lever serves the same greedy
// tokens off the same device twin and the same CPU embed rows off the packed emb region, and the trimmed lane maps
// back warm to the same tokens; the K-quant twin's lane mints off its native planes and maps back to the same tokens.
// Skips when interpreted, without the model, with the image rail off (DASLLAMA_IMAGE=0), without a live armed tier,
// and when the whole-model driver declines the model on this box.

let private MINT_FILE = "Qwen3.5-0.8B-MTP-Q8_0.gguf"
let private KQ_FILE = "Qwen3.5-0.8B-Q4_K_M.gguf"
let private MINT_PROMPT = 16
let private MINT_STEPS = 12

def private load_cached_q8(path : string) : Model {
    var m = Model()
    with_dasllama_jobque_() {
        m <- load_model_cached(path, QuantMode.q8)   // nolint:PERF030,LINT019 -- the move must sit inside with_job_que; the target is fresh-empty, so nothing drops
    }
    return <- m
}

//! `MINT_STEPS` greedy tokens over a counting prompt, and the CPU embed row of one token: what the two lanes must agree on
[cold_path]
def private mint_witness(tr : Model; var ids : array<int64>; var row : array<float>) {
    with_dasllama_jobque_() {
        var s <- create_session(tr, KVDtype.f16)
        var prompt <- [for (i in range64(int64(MINT_PROMPT))); 1000l + i * 7l]
        delete ids
        ids <- generate(tr, s, prompt, MINT_PROMPT + MINT_STEPS)
        row |> resize(tr.config.dim)
        embed_row(tr, 1234l, unsafe(addr(row[0])))
        delete s
        delete prompt
    }
}

//! the levers a lane cell pins for its loads and puts back as it found them
struct private LaneLevers {
    route : bool
    trim_set : bool
    trim : bool
}

def private pin_lane_levers() : LaneLevers {
    let l = LaneLevers(route = gpu_resident_route_lever(), trim_set = vulkan_trim_lever_set(), trim = vulkan_trim())
    set_gpu_resident_route(true)
    set_vulkan_trim(false)
    return l
}

def private restore_lane_levers(l : LaneLevers) {
    set_gpu_resident_route(l.route)
    restore_vulkan_trim(l.trim_set, l.trim)
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

[test]
def test_vulkan_mint_lanes(tst : T?) {
    tst |> run("Qwen3.5-0.8B-MTP: a cold vulkan-armed load mints the vulkan lane alone; the trimmed lane serves the same tokens and embed rows, cold and warm") @(t : T?) {
        if (!family_on(t, "qwen35")) return
        let path = path_join(models_dir(), MINT_FILE)
        if (mint_skip(t, path)) return
        let levers = pin_lane_levers()
        let vk_tag = moe_gpu_bake_tag()
        let img_v = image_path_for(path, vk_tag)
        let img_p = image_path_for(path, "", "q8")
        remove(img_v)
        remove(img_p)
        var plain <- load_cached_q8(path)
        if (!moe_gpu_resident_active()) {
            delete plain
            moe_gpu_drop_model()
            remove(img_v)
            restore_lane_levers(levers)
            t |> skip("the whole-model driver did not take the model on this box (the decline line above)")
            return
        }
        t |> success(stat(img_v).is_valid && !stat(img_p).is_valid, "the cold load minted the vulkan lane and no planar lane")
        t |> success(plain.image_map != null && long_length(plain.vkplan) > 0l && long_length(plain.qblob) > 0l, "the served image is the vulkan lane, the planar families kept with the trim off")
        let plain_bytes = stat(img_v).size
        var ids_a : array<int64>
        var row_a : array<float>
        mint_witness(plain, ids_a, row_a)
        t |> equal(long_length(ids_a), int64(MINT_PROMPT + MINT_STEPS), "the untrimmed lane generated the {MINT_STEPS} asked steps past the {MINT_PROMPT}-token prompt")
        delete plain
        moe_gpu_drop_model()
        set_vulkan_trim(true)
        remove(img_v)
        var trimmed <- load_cached_q8(path)
        t |> success(moe_gpu_resident_active(), "the trimmed lane is served by the whole-model driver")
        t |> success(trimmed.image_map != null && trimmed.planes_trimmed && long_length(trimmed.qblob) == 0l && long_length(trimmed.embq) > 0l,
            "the trimmed lane serves without the planar families, the emb region packed")
        t |> success(stat(img_v).size < plain_bytes, "the trimmed lane is the smaller file ({stat(img_v).size} against {plain_bytes} bytes)")
        var ids_b : array<int64>
        var row_b : array<float>
        mint_witness(trimmed, ids_b, row_b)
        t |> equal(long_length(ids_b), int64(MINT_PROMPT + MINT_STEPS), "the trimmed lane generated the {MINT_STEPS} asked steps")
        log_gen_texts(trimmed, "untrimmed lane / trimmed lane", ids_a, ids_b)
        t |> equal(mismatch_exact(ids_a, ids_b), 0, "the trimmed lane's greedy tokens are the untrimmed lane's, token for token (both streams decoded above)")
        t |> equal(mismatch_exact(row_a, row_b), 0, "the CPU embed row off the packed emb region is the planar plane's, bit for bit")
        delete trimmed
        moe_gpu_drop_model()
        var warm <- load_cached_q8(path)
        t |> success(moe_gpu_resident_active(), "the warm trimmed lane is served by the whole-model driver")
        t |> success(warm.image_map != null && warm.planes_trimmed, "the trimmed lane maps back warm")
        var ids_c : array<int64>
        var row_c : array<float>
        mint_witness(warm, ids_c, row_c)
        t |> equal(long_length(ids_c), int64(MINT_PROMPT + MINT_STEPS), "the warm lane generated the {MINT_STEPS} asked steps")
        log_gen_texts(warm, "untrimmed lane / warm trimmed lane", ids_a, ids_c)
        t |> equal(mismatch_exact(ids_a, ids_c), 0, "... and serves the same tokens (both streams decoded above)")
        delete warm
        moe_gpu_drop_model()
        remove(img_v)
        restore_lane_levers(levers)
        delete ids_a
        delete ids_b
        delete ids_c
        delete row_a
        delete row_b
        delete row_c
    }
}

[test]
def test_vulkan_mint_kquant(tst : T?) {
    tst |> run("Qwen3.5-0.8B-Q4_K_M: the K-quant twin's lane mints off its native planes and maps back warm to the same tokens") @(t : T?) {
        if (!family_on(t, "qwen35")) return
        let path = path_join(models_dir(), KQ_FILE)
        if (mint_skip(t, path)) return
        let levers = pin_lane_levers()
        let vk_tag = moe_gpu_bake_tag()
        let img_v = image_path_for(path, vk_tag)
        let img_p = image_path_for(path, "", "q8")
        remove(img_v)
        remove(img_p)
        var cold <- load_cached_q8(path)
        if (!moe_gpu_resident_active()) {
            delete cold
            moe_gpu_drop_model()
            remove(img_v)
            restore_lane_levers(levers)
            t |> skip("the whole-model driver did not take the K-quant twin on this box (the decline line above)")
            return
        }
        t |> success(cold.kq_repacked, "the loader kept the K-quant planes native and froze their interleave (the job view copies it)")
        t |> success(stat(img_v).is_valid && !stat(img_p).is_valid && long_length(cold.vkplan) > 0l, "the cold load minted the K-quant model's vulkan lane alone, the plan aboard")
        var ids_a : array<int64>
        var row_a : array<float>
        mint_witness(cold, ids_a, row_a)
        t |> equal(long_length(ids_a), int64(MINT_PROMPT + MINT_STEPS), "the cold lane generated the {MINT_STEPS} asked steps")
        delete cold
        moe_gpu_drop_model()
        var warm <- load_cached_q8(path)
        t |> success(moe_gpu_resident_active() && warm.image_map != null, "the lane maps back warm onto the whole-model driver")
        var ids_b : array<int64>
        var row_b : array<float>
        mint_witness(warm, ids_b, row_b)
        t |> equal(long_length(ids_b), int64(MINT_PROMPT + MINT_STEPS), "the warm lane generated the {MINT_STEPS} asked steps")
        log_gen_texts(warm, "cold mint / warm map", ids_a, ids_b)
        t |> equal(mismatch_exact(ids_a, ids_b), 0, "the mapped twin serves the mint's tokens, token for token (both streams decoded above)")
        t |> equal(mismatch_exact(row_a, row_b), 0, "the CPU embed row is the same on both loads, bit for bit")
        delete warm
        moe_gpu_drop_model()
        remove(img_v)
        restore_lane_levers(levers)
        delete ids_a
        delete ids_b
        delete row_a
        delete row_b
    }
}
