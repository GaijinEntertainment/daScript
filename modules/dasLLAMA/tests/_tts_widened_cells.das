options gen2
options indenting = 4
options _comment_hygiene = true
options _dasllama_internal = true

//! The TTS driver's count of dense dispatches over widened rows (`tts_home_stats(dev).widened`) on a hand-built Pocket
//! frame model no file stocks: a backbone 36 wide and a latent 20 wide, both off the stored members' 32 lattice, so a
//! leaf held as half floats is read back into the slab as float rows. The case bakes the model's frame part, serves it
//! through the registered prompt and frames seats of the home the TTS seats serve on, and holds each count site of the
//! driver to its own number, the model of floats alone the control. Model-free; home-neutral.
module _tts_widened_cells shared public

require dastest/testing_boost public
require dasllama/dasllama_lint public
require dasllama/dasllama_common   // bump_weights_epoch: a model's discharge
require dasllama/dasllama_math   // with_dasllama_jobque_, reserve_resize
require dasllama/dasllama_plane
require dasllama/dasllama_tower   // TowerDev, TowerDevBuild
require dasllama/dasllama_tts_blocks
require dasllama/dasllama_tts_slab   // tts_pk_frames_write
require dasllama/dasllama_pocket
require dasllama/dasllama_tower_tts   // tts_part_bake, tts_home_stats
require ?das_metal dasllama/dasllama_metal_tts   // nolint:STYLE030,LINT019 - the Metal home's speech seats register here
require ?vulkan dasllama/dasllama_vulkan_tts   // nolint:STYLE030,LINT019 - the Vulkan home's speech seats register here
require ?cuda dasllama/dasllama_cuda_tts   // nolint:STYLE030,LINT019 - the CUDA home's speech seats register here
require daslib/jobque_boost
require _compares   // row_fill, all_finite
require _gpu_knobs

//! the leaf of the frame model held as half floats: none, a backbone layer's fused projection, or the frame's input linear
enum WidenedLeaf {
    none
    in_proj
    input_linear
}

let private WD_D = 36l        //! the backbone's width: on the row kernels' 4 lattice, off the stored members' 32
let private WD_LATENT = 20l   //! the latent's width, the same way
let private WD_DH = 64l       //! the one head's width, the attention row's own
let private WD_FFN = 40l
let private WD_HEAD = 32l     //! the flow head's width
let private WD_LAYERS = 2
let private WD_KEYS = 16l     //! the voice caches' rows
let private WD_VOICE = 1l     //! the voice's rows ahead of the text
let private WD_TEXT = 2l      //! the prompt's rows
let private WD_FRAMES = 3l    //! the frames the loop makes: the EOS threshold is out of reach

def private wd_norm(c : int64) : TtsNorm {
    var n = TtsNorm()
    n.w <- [for (_ in range64(c)); 1.0]
    n.b <- [for (_ in range64(c)); 0.0]
    return <- n
}

//! a dense layer of small weights, held as half floats under `halves`
def private wd_linear(nin, nout : int64; seed : int; halves : bool) : TtsLinear {
    var l = TtsLinear(nin = nin, nout = nout)
    l.w <- row_fill(int(nin * nout), 7 + seed, 3 + seed, 0.002, 0.097)
    l.b <- row_fill(int(nout), 5, seed, 0.001, 0.048)
    if (halves) {
        linear_prepare(l, false, false, false, true, true)
    }
    return <- l
}

def private wd_layer(seed : int; halves : bool) : PocketLayer {
    var l = PocketLayer()
    l.norm1 <- wd_norm(WD_D)
    l.in_proj <- wd_linear(WD_D, 3l * WD_DH, seed, halves)
    l.out_proj <- wd_linear(WD_DH, WD_D, seed + 1, false)
    l.norm2 <- wd_norm(WD_D)
    l.ffn1 <- wd_linear(WD_D, WD_FFN, seed + 2, false)
    l.ffn2 <- wd_linear(WD_FFN, WD_D, seed + 3, false)
    return <- l
}

//! the frame model: two backbone layers, a pointwise flow head of one block, no z projection, an EOS no logit crosses
def private wd_model(leaf : WidenedLeaf) : PocketModel {
    var m = PocketModel(latent_dim = WD_LATENT, cond_dim = WD_D, eos_threshold = 1e30)
    m.backbone.d = WD_D
    m.backbone.heads = 1l
    m.backbone.kv_heads = 1l
    m.backbone.attn_dim = WD_DH
    m.backbone.dh = WD_DH
    m.backbone.ffn = WD_FFN
    m.backbone.period = 10000.0
    m.backbone.eps = 1e-5
    m.backbone.layers |> reserve(WD_LAYERS)
    for (i in range(WD_LAYERS)) {
        m.backbone.layers |> emplace(wd_layer(10 * i, leaf == WidenedLeaf.in_proj))
    }
    m.input_linear <- wd_linear(WD_LATENT, WD_D, 41, leaf == WidenedLeaf.input_linear)
    m.out_norm <- wd_norm(WD_D)
    m.out_eos <- wd_linear(WD_D, 1l, 43, false)
    m.speaker_proj <- wd_linear(WD_LATENT, WD_D, 45, false)
    m.emb_std.a <- [for (_ in range64(WD_LATENT)); 1.0]
    m.emb_mean.a <- [for (_ in range64(WD_LATENT)); 0.0]
    m.bos_emb.a <- row_fill(int(WD_LATENT), 3, 1, 0.01, 0.48)
    m.head.dim = WD_HEAD
    m.head.flow = PocketFlow.pointwise
    m.head.input_proj <- wd_linear(WD_LATENT, WD_HEAD, 47, false)
    m.head.cond_embed <- wd_linear(WD_D, WD_HEAD, 49, false)
    var blk = PocketResBlock()
    blk.ln <- wd_norm(WD_HEAD)
    blk.adaln <- wd_linear(WD_HEAD, 3l * WD_HEAD, 51, false)
    blk.mlp1 <- wd_linear(WD_HEAD, WD_HEAD, 53, false)
    blk.mlp2 <- wd_linear(WD_HEAD, WD_HEAD, 55, false)
    m.head.blocks |> emplace(blk)
    m.head.final_ln <- wd_norm(WD_HEAD)
    m.head.final_adaln <- wd_linear(WD_HEAD, 2l * WD_HEAD, 57, false)
    m.head.final_linear <- wd_linear(WD_HEAD, WD_LATENT, 59, false)
    return <- m
}

//! a voice of `WD_VOICE` rows in caches of `WD_KEYS`, a cache a backbone layer
def private wd_voice(m : PocketModel) : PocketVoiceState {
    var vs = PocketVoiceState(len = WD_VOICE)
    var inscope k <- row_fill(int(WD_VOICE * WD_DH), 11, 2, 0.01, 0.48)
    var inscope v <- row_fill(int(WD_VOICE * WD_DH), 13, 4, 0.01, 0.48)
    vs.caches |> reserve(length(m.backbone.layers))
    for (_ in m.backbone.layers) {
        var kc = TtsKvCache()
        kv_cache_init(kc, m.backbone.kv_heads, m.backbone.dh, WD_KEYS)
        kv_cache_append(kc, k, v, WD_VOICE, WD_DH)
        vs.caches |> emplace(kc)
    }
    return <- vs
}

//! what one model's prompt call and frames call left: each seat's answer and the widened dispatches the home counted over it
struct WidenedRun {
    prompt_served : bool
    frames : int64
    prompt_widened : int64
    frames_widened : int64
    latents_finite : bool
}

//! the model of `leaf` baked for `dev`, its prompt of `WD_TEXT` rows and its `WD_FRAMES` frames through the registered seats, then discharged
def private wd_run(leaf : WidenedLeaf; dev : TowerDev) : WidenedRun {
    var inscope m <- wd_model(leaf)
    var inscope b : TowerDevBuild
    var inscope slots : TtsPkFramesSlab
    tts_part_bake(b, dev, "pk.frames", slots, $(var w : TtsSlabWriter; var s : TtsPkFramesSlab) {
        tts_pk_frames_write(w, m, s)
    }, false, true)
    m.dev = PlaneU8(p = unsafe(addr(b.blob[0])), n = long_length(b.blob))
    m.devplan = PlaneI64(p = unsafe(addr(b.plan[0])), n = long_length(b.plan))
    var inscope vs <- wd_voice(m)
    var inscope sc : PocketScratch
    sc.rows <- row_fill(int(WD_TEXT * WD_D), 17, 5, 0.01, 0.48)
    sc.eos_logits |> reserve_resize(WD_FRAMES)
    var inscope noise <- row_fill(int(WD_FRAMES * WD_LATENT), 19, 7, 0.01, 0.48)
    var inscope forced : array<float>
    let drv = pocket_gpu_driver()
    var r = WidenedRun()
    let before = tts_home_stats(dev).widened
    r.prompt_served = invoke(drv.prompt, m, vs, WD_TEXT, sc)
    let prompted = tts_home_stats(dev).widened
    r.frames = invoke(drv.frames, m, vs, WD_TEXT, 1.0, 0, noise, forced, WD_FRAMES, sc).frames
    r.prompt_widened = prompted - before
    r.frames_widened = tts_home_stats(dev).widened - prompted
    r.latents_finite = long_length(sc.latents) == WD_FRAMES * WD_LATENT && all_finite(sc.latents)
    bump_weights_epoch()   // the model's discharge: the home lets go of the slab over a block this function frees
    m.dev = PlaneU8()
    m.devplan = PlaneI64()
    return r
}

def private wd_served(t : T?; name : string; r : WidenedRun) {
    t |> success(r.prompt_served, "{name}: the prompt seat served the model")
    t |> equal(r.frames, WD_FRAMES, "{name}: the frames seat made every frame")
    t |> success(r.latents_finite, "{name}: every frame's latent row came back finite")
}

//! The home's count of dense dispatches over widened rows on the hand-built frame model, a count site a leaf: the rows
//! GEMM of the prompt's layers and the frame loop's fused projection over a backbone projection of halves, the row GEMV
//! over an input linear of halves, and no count over the model of floats. Skips where no GPU device serves the TTS seats.
def tts_widened_counts_case(t : T?) {
    with_dasllama_jobque_() {
        with_gpu_knobs() {
            set_serving_gpu_tower(true, TowerFamily.tts)
            let dev = serving_tower_dev(TowerFamily.tts)
            if (dev == TowerDev.none || !serving_tower_device_up(TowerFamily.tts)) {
                t |> skip("no GPU device serves the TTS seats on this box")
                return
            }
            let drv = pocket_gpu_driver()
            t |> success(drv.prompt != null && drv.frames != null, "the home registers the Pocket prompt and frames seats")
            return if (drv.prompt == null || drv.frames == null)
            let floats = wd_run(WidenedLeaf.none, dev)
            let proj = wd_run(WidenedLeaf.in_proj, dev)
            let lin = wd_run(WidenedLeaf.input_linear, dev)
            wd_served(t, "floats", floats)
            wd_served(t, "a projection of halves", proj)
            wd_served(t, "an input linear of halves", lin)
            t |> equal(floats.prompt_widened, 0l, "floats: the prompt's layers count no widened dispatch (the control)")
            t |> equal(floats.frames_widened, 0l, "floats: the frame loop counts no widened dispatch (the control)")
            t |> equal(proj.prompt_widened, int64(WD_LAYERS),
                "a projection of halves off the 32 lattice: the prompt's rows GEMM over it counts once a layer")
            t |> equal(proj.frames_widened, int64(WD_LAYERS) * WD_FRAMES,
                "a projection of halves off the 32 lattice: the frame loop's fused projection counts once a layer a frame")
            t |> equal(lin.prompt_widened, 0l, "an input linear of halves: the prompt's layers read none of it")
            t |> equal(lin.frames_widened, WD_FRAMES,
                "an input linear of halves off the 32 lattice: the frame's row GEMV over it counts once a frame")
        }
    }
}
