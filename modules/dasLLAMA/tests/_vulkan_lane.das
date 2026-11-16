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
require daslib/rtti   // this_context().last_exception: a refused call's text
require strings   // find: the refusal names the model's form
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

//! the text a forward on `tr` through an f32 session panics with, "" where it served: the armed mirror is f16, so the driver
//! passes the call to the CPU rails
def private cpu_call_refusal(tr : Model) : string {
    var why = ""
    try {
        var ids : array<int64>
        var row : array<float>
        mint_witness(tr, ids, row, KVDtype.f32)
        delete ids
        delete row
    } recover {
        why = this_context().last_exception
    }
    return why
}

//! whether `tr` holds a planar weight plane on the host
def private holds_planes(tr : Model) : bool {
    let f = footprint(tr)
    return (f.weight_fp32_bytes + f.q8_bytes + f.q8_scale_bytes + f.q4_bytes + f.q4_scale_bytes + f.mx4_bytes + f.mx4_scale_bytes
        + f.kq_bytes + f.kq_scale_bytes) > 0l
}

//! `m` dropped and re-armed as a slot switch does it - its marks captured, the device emptied, the marks put back: the
//! re-arm's answer
def private rearm_dropped(var m : Model) : string {
    var slot = gpu_model_marks_init()
    gpu_slot_capture(slot)
    moe_gpu_drop_model()
    moe_gpu_model_marks_restore(slot)
    return gpu_slot_rearm(g_gpu_want, m)
}

//! the live-load cell both homes run over `path`: the lane's tokens as the reference, then - the image save off, so there is no
//! lane to write - the gguf streamed onto the device (`gguf_live_loads` up by one, no planar plane and no image on the host, no
//! lane on disk) serving the lane's tokens and embed row, re-armed off the gguf to the same tokens, a call the driver passes to
//! the CPU refused by name; and under the host-planes ask the same load keeping its planes, counting no live load, the same tokens.
//! Every load here places one transcode of the gguf's planes on the device - baked into the lane, gathered live, or uploaded
//! off the host's planes - so the driver's logits are bit-identical and the greedy tokens compare exactly
[cold_path]
def live_cell(t : T?; path : string) {
    with_cold_lane(t, path, "the model") $(var lane; img_v) {
        var a <- witness_steps(t, lane, "the lane")
        let lane_gathers = gpu_resident_embed_gathers()
        delete lane
        moe_gpu_drop_model()
        remove(img_v)
        let save_set = image_save_lever_set()
        let save_was = image_save_enabled()
        let planes_set = host_planes_lever_set()
        let planes_was = host_planes()
        set_image_save(false)
        set_host_planes(false)
        let live0 = gguf_live_loads()
        var live <- load_cached_q8(path)
        t |> equal(gguf_live_loads(), live0 + 1l, "with no lane to write the gguf streams onto the device")
        t |> success(moe_gpu_resident_active(), "the live load is served by the whole-model driver")
        t |> success(live.planes_trimmed && live.image_map == null && !holds_planes(live), "the live load holds no planar plane and no image on the host")
        t |> equal(gpu_resident_embed_gathers(), lane_gathers, "the live load's token rows gather on the device where the lane's do ({lane_gathers})")
        t |> success(!stat(img_v).is_valid, "the live load wrote no lane")
        var b <- witness_steps(t, live, "the live load")
        log_gen_texts(live, "lane / live load", a.ids, b.ids)
        t |> equal(mismatch_exact(a.ids, b.ids), 0, "the live load's greedy tokens are the lane's, token for token (both streams decoded above)")
        t |> equal(mismatch_exact(a.row, b.row), 0, "its CPU embed row off the packed emb region is the lane's, bit for bit")
        t |> equal(rearm_dropped(live), "gpu:resident", "the dropped live load re-arms by streaming its gguf again")
        var r <- witness_steps(t, live, "the re-armed live load")
        log_gen_texts(live, "live load / re-armed live load", b.ids, r.ids)
        t |> equal(mismatch_exact(a.ids, r.ids), 0, "... and serves the same tokens (both streams decoded above)")
        let why = cpu_call_refusal(live)
        t |> success(find(why, "planes-trimmed") >= 0, "a call the driver passes to the CPU is refused by name: '{why}'")
        delete live
        moe_gpu_drop_model()
        set_host_planes(true)
        var kept <- load_cached_q8(path)
        t |> equal(gguf_live_loads(), live0 + 1l, "under the host-planes ask the load streams nothing live")
        t |> success(!kept.planes_trimmed && holds_planes(kept), "... and keeps its planes on the host")
        var c <- witness_steps(t, kept, "the load with its planes kept")
        log_gen_texts(kept, "live load / planes kept", b.ids, c.ids)
        t |> equal(mismatch_exact(b.ids, c.ids), 0, "... serving the live load's tokens (both streams decoded above)")
        delete kept
        moe_gpu_drop_model()
        restore_image_save(save_set, save_was)
        restore_host_planes(planes_set, planes_was)
        delete a
        delete b
        delete c
        delete r
    }
}

//! the trimmed-lane cell both homes run over `path`: a staged load with its planes on the host says whether the model's token
//! rows gather on the device; the gguf streamed live (the image save off) is the reference - the same gather, its tokens;
//! then, the trim on, the lane minted cold with no planar family and served by the whole-model driver to the reference's
//! tokens, the token rows gathered where the staged load's are, mapped warm and re-armed the same. The live load and the lane
//! place one transcode of the gguf's planes through one gather, so the driver's logits are bit-identical and the greedy
//! tokens compare exactly; the staged load uploads its repacked planes, and its tokens are logged, not held
[cold_path]
def family_lane_cell(t : T?; path : string) {
    var staged <- load_resident_host(path, 1l)
    if (!moe_gpu_resident_active()) {
        delete staged
        moe_gpu_drop_model()
        t |> skip("the whole-model driver did not take the model on this box (the decline line above)")
        return
    }
    let gathers = gpu_resident_embed_gathers()
    var p <- witness_steps(t, staged, "the staged load")
    delete staged
    moe_gpu_drop_model()
    set_vulkan_trim(true)
    let img_t = image_path_for(path, moe_gpu_bake_tag())
    remove(img_t)
    let save_set = image_save_lever_set()
    let save_was = image_save_enabled()
    set_image_save(false)
    let live0 = gguf_live_loads()
    var live <- load_cached_q8(path)
    restore_image_save(save_set, save_was)
    t |> success(gguf_live_loads() == live0 + 1l && moe_gpu_resident_active(), "with no lane to write the gguf streams onto the device and the whole-model driver serves it")
    t |> equal(gpu_resident_embed_gathers(), gathers, "live: the token rows gather on the device exactly where the staged load's do ({gathers})")
    var a <- witness_steps(t, live, "the live load")
    log_gen_texts(live, "staged load / live load", p.ids, a.ids)
    delete live
    moe_gpu_drop_model()
    for (leg in ["cold", "warm"]) {
        var lane <- load_cached_q8(path)
        t |> success(moe_gpu_resident_active(), "{leg}: the trimmed lane is served by the whole-model driver (the load log names any decline)")
        t |> success(lane.image_map != null && lane.planes_trimmed && !holds_planes(lane) && stat(img_t).is_valid, "{leg}: the lane maps with no planar family on the host")
        t |> equal(gpu_resident_embed_gathers(), gathers, "{leg}: the token rows gather on the device exactly where the staged load's do ({gathers})")
        var b <- witness_steps(t, lane, "the {leg} trimmed lane")
        log_gen_texts(lane, "live load / {leg} trimmed lane", a.ids, b.ids)
        t |> equal(mismatch_exact(a.ids, b.ids), 0, "{leg}: the trimmed lane's greedy tokens are the live load's, token for token (both streams decoded above)")
        delete b
        if (leg == "warm") {
            t |> equal(rearm_dropped(lane), "gpu:resident", "the dropped lane re-arms onto the whole-model driver")
            t |> equal(gpu_resident_embed_gathers(), gathers, "re-armed: the token rows gather on the device where the staged load's do")
            var r <- witness_steps(t, lane, "the re-armed lane")
            log_gen_texts(lane, "live load / re-armed lane", a.ids, r.ids)
            t |> equal(mismatch_exact(a.ids, r.ids), 0, "re-armed: the same tokens (both streams decoded above)")
            delete r
        }
        delete lane
        moe_gpu_drop_model()
    }
    remove(img_t)
    delete a
    delete p
}

//! the lanes cell both homes run over a cold load of `path` (`with_cold_lane`): `lane_check` over the lane's path first (the CUDA
//! home names the lane; the Vulkan home checks nothing), the served image the home's lane with the planar families kept, the
//! untrimmed lane's witness, the trimmed lane cold - served by the whole-model driver without the planar families off the packed
//! emb region, the smaller file, the untrimmed lane's tokens token for token and its CPU embed row bit for bit - and warm, the
//! same tokens again
[cold_path]
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
        let read_set = lane_read_lever_set()
        let read_was = lane_read()
        set_lane_read(true)
        let reads0 = vk_slice_file_reads()
        var warm <- load_cached_q8(path)
        let reads1 = vk_slice_file_reads()
        t |> success(moe_gpu_resident_active(), "the warm trimmed lane is served by the whole-model driver")
        t |> success(warm.image_map != null && warm.planes_trimmed, "the trimmed lane maps back warm")
        t |> success(reads1 > reads0, "the warm load's planes are read off the lane file ({reads1 - reads0} reads)")
        var c <- witness_steps(t, warm, "the warm lane")
        log_gen_texts(warm, "untrimmed lane / warm trimmed lane", a.ids, c.ids)
        t |> equal(mismatch_exact(a.ids, c.ids), 0, "... and serves the same tokens (both streams decoded above)")
        let reads2 = vk_slice_file_reads()
        t |> equal(rearm_dropped(warm), "gpu:resident", "the dropped lane re-arms onto the whole-model driver")
        t |> equal(vk_slice_file_reads() - reads2, reads1 - reads0, "the re-arm reads the planes the load read, off the lane file again")
        var r <- witness_steps(t, warm, "the re-armed lane")
        log_gen_texts(warm, "warm lane / re-armed lane", c.ids, r.ids)
        t |> equal(mismatch_exact(c.ids, r.ids), 0, "... and serves the same tokens (both streams decoded above)")
        delete r
        delete warm
        moe_gpu_drop_model()
        set_lane_read(false)
        let reads3 = vk_slice_file_reads()
        var mapped <- load_cached_q8(path)
        t |> equal(vk_slice_file_reads(), reads3, "with the lane read off the planes are copied out of the mapping: no file read")
        var d <- witness_steps(t, mapped, "the warm lane copied out of the mapping")
        log_gen_texts(mapped, "warm lane read / warm lane copied", c.ids, d.ids)
        t |> equal(mismatch_exact(c.ids, d.ids), 0, "... and serves the tokens the read lane serves (both streams decoded above)")
        delete mapped
        moe_gpu_drop_model()
        restore_lane_read(read_set, read_was)
        delete d
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
