// The discrete lane cells' rig - test_vulkan_mint and test_cuda_mint (the live tiers) and test_model_image_vulkan (the dry
// tier): the q8 cached load inside the job queue, the route and trim levers a lane cell pins for its loads and puts back as it
// found them, the lane witness (greedy tokens and a CPU embed row), the cells' frame and the cold mint.

options gen2
options _dasllama_internal = true

module _vulkan_lane shared public

require dasllama/dasllama_lint public
require dastest/testing_boost public
require dasllama/dasllama_common
require dasllama/dasllama   // create_session, generate, moe_gpu_drop_model
require dasllama/dasllama_math   // with_dasllama_jobque_
require dasllama/dasllama_image   // load_model_cached, image_path_for
require dasllama/dasllama_gpu_tier
require dasllama/dasllama_gpu_resident   // moe_gpu_resident_active
require dasllama/dasllama_env   // g_env_engine.image: the rail's switch
require dasllama/dasllama_kv_dtype
require daslib/fio   // stat, remove
require _model_tier   // models_dir, model_available, family_on, mirror_kv_dtype
require _compares   // jit_only
require _resident_feed   // load_resident_at

let MINT_FILE = "Qwen3.5-0.8B-MTP-Q8_0.gguf"
let KQ_FILE = "Qwen3.5-0.8B-Q4_K_M.gguf"
let MINT_PROMPT = 16
let MINT_STEPS = 12

//! `MINT_STEPS` greedy tokens over a counting prompt, and the CPU embed row of one token: what the two lanes must agree on
def mint_witness(tr : Model; var ids : array<int64>; var row : array<float>; kv : KVDtype = KVDtype.f16) {
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

typedef MintRun = tuple<ids : array<int64>; row : array<float>>

//! `mint_witness` over `tr` with its token count asserted: `what` generated the asked steps past the prompt
[cold_path]
def witness_steps(t : T?; tr : Model; what : string) : MintRun {
    var w : MintRun
    mint_witness(tr, w.ids, w.row)
    t |> equal(long_length(w.ids), int64(MINT_PROMPT + MINT_STEPS), "{what} generated the {MINT_STEPS} asked steps past the {MINT_PROMPT}-token prompt")
    return <- w
}

//! the box facts every lane cell skips on; "" = the cell runs
def mint_skip(t : T?; path : string) : bool {
    return true if (!jit_only(t))
    if (!g_env_engine.image) {
        t |> skip("the image rail is off (DASLLAMA_IMAGE=0) - the lane mint has nothing to write")
        return true
    }
    if (!model_available(t, path)) {
        return true
    }
    if (empty(moe_gpu_bake_tag())) {
        t |> skip("no armed discrete tier (DASLLAMA_GPU=1 on a box with a device)")
        return true
    }
    return false
}

//! the lane cells' frame: the `family` gate and `file`'s skips, then `body` over the model's path under the pinned lane levers, put back after
def with_mint_file(t : T?; family, file : string; body : block<(path : string) : void>) {
    return if (!family_on(t, family))
    let path = path_join(models_dir(), file)
    return if (mint_skip(t, path))
    let levers = pin_lane_levers()
    invoke(body, path)
    restore_lane_levers(levers)
}

//! a cold load on the armed home of `path` with both lanes' images removed first, `body` over it and the home's lane path (the
//! body drops its models), the image removed after; a skip naming `what` where the whole-model driver did not take the model
def with_cold_lane(t : T?; path, what : string; body : block<(var cold : Model; img_v : string) : void>) {
    let img_v = image_path_for(path, moe_gpu_bake_tag())
    let img_p = image_path_for(path, "", "q8")
    remove(img_v)
    remove(img_p)
    var cold <- load_cached_q8(path)
    if (moe_gpu_resident_active()) {
        t |> success(stat(img_v).is_valid && !stat(img_p).is_valid, "the cold load minted {what}'s {image_lane_name(moe_gpu_bake_tag())} lane and no planar lane")
        invoke(body, cold, img_v)
    } else {
        delete cold
        moe_gpu_drop_model()
        t |> skip("the whole-model driver did not take {what} on this box (the decline line above)")
    }
    remove(img_v)
}

//! the greedy tokens a staged (non-image) load serves under the `kv` mirror codec; empty where the driver did not take the model
[cold_path]
def staged_tokens_at(t : T?; path : string; kv : KVDtype) : array<int64> {
    var m <- load_resident_at(path, kv)
    var ids : array<int64>
    if (moe_gpu_resident_active()) {
        t |> equal(mirror_kv_dtype(), kv, "the staged load arms the mirror at the asked codec")
        var row : array<float>
        mint_witness(m, ids, row, kv)
        delete row
    }
    delete m
    moe_gpu_drop_model()
    return <- ids
}

//! `path` loaded through the image cache in q8 inside the job queue
def load_cached_q8(path : string) : Model {
    var m = Model()
    with_dasllama_jobque_() {
        m <- load_model_cached(path, QuantMode.q8)   // nolint:PERF030,LINT019 -- the move must sit inside with_job_que; the target is fresh-empty, so nothing drops
    }
    return <- m
}

//! the levers a lane cell pins for its loads, as it found them
struct LaneLevers {
    route : bool
    trim_set : bool
    trim : bool
}

//! the levers captured, then the resident route pinned on (the trimmed lane's admission reads it) and the trim off
def pin_lane_levers() : LaneLevers {
    let l = LaneLevers(route = gpu_resident_route_lever(), trim_set = vulkan_trim_lever_set(), trim = vulkan_trim())
    set_gpu_resident_route(true)
    set_vulkan_trim(false)
    return l
}

//! the route and the trim put back as `pin_lane_levers` found them
def restore_lane_levers(l : LaneLevers) {
    set_gpu_resident_route(l.route)
    restore_vulkan_trim(l.trim_set, l.trim)
}

//! the lanes cell both homes run over a cold load of `path` (`with_cold_lane`): `lane_check` over the lane's path first (the CUDA
//! home names the lane; the Vulkan home checks nothing), the served image the home's lane with the planar families kept, the
//! untrimmed lane's witness, the trimmed lane cold - served by the whole-model driver without the planar families off the packed
//! emb region, the smaller file, the untrimmed lane's tokens token for token and its CPU embed row bit for bit - and warm, the
//! same tokens again
def lanes_cell(t : T?; path : string; lane_check : block<(img : string) : void>) {
    with_cold_lane(t, path, "the model") $(var plain; img_v) {
        invoke(lane_check, img_v)
        t |> success(plain.image_map != null && long_length(plain.vkplan) > 0l && long_length(plain.qblob) > 0l, "the served image is the {image_lane_name(moe_gpu_bake_tag())} lane, the planar families kept with the trim off")
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

//! the K-quant twin's cell both homes run over a cold load of `path`: `lane_check` over the lane's path, the planes kept native
//! (`kq_repacked`) with the plan, the cold lane's witness, and the warm map onto the whole-model driver serving the mint's tokens
//! and embed row
def kquant_cell(t : T?; path : string; lane_check : block<(img : string) : void>) {
    with_cold_lane(t, path, "the K-quant twin") $(var cold; img_v) {
        invoke(lane_check, img_v)
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

//! the K/V codecs cell both homes run over `path`: the trimmed lane minted under a q8_0 mirror maps under tq4 and f16 with no
//! re-mint (the file's size and time as the mint left them; `lane_check` over the minted lane's path), each load armed at the
//! asked codec and serving the staged load's tokens there; the control: a block codec moves the tokens off the f16 mirror's
def kv_codecs_cell(t : T?; path : string; lane_check : block<(img : string) : void>) {
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
                    invoke(lane_check, img_t)
                } else {
                    t |> success(st.size == minted_size && int64(st.mtime) == minted_at, "{kv}: the lane minted under {codecs[0]} mapped as it was - no re-mint ({st.size} bytes)")
                }
                var inscope ids : array<int64>
                var inscope row : array<float>
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
