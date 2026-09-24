options gen2
options persistent_heap
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama_qwen3v
require dasllama/dasllama_image   // image_path_for - the baked-tag sniff cell locates the q8 .dlim
require dasllama/dasllama_vision_embedder   // load_vision_embedder - the sniff cell routes the baked tag
require dasllama/dasllama_math   // setup_dasllama_jobque_
require daslib/jobque_boost
require daslib/fio
require _model_tier   // models_dir() + model_available() gate
require ?das_metal dasllama/dasllama_metal_tower   // the CPU gates pin the tower knob off
require ?das_metal dasllama/dasllama_metal_prefill   // metal_prefill_shutdown - the f16-route cell re-arms crowned PSOs
require ?vulkan dasllama/dasllama_vulkan_tower
require ?vulkan dasllama/dasllama_vulkan_common
require dasllama/dasllama_common   // set_metal_tensor_crowns - the f16-route cell's crown pin
require _vision_oracle   // the mtmd dump parser, fixture generators, per-token compare
require math

//! the CPU-lane claims pin every GPU tower driver off (a hook that serves silently stops the cell measuring the CPU chain)
def private set_every_gpu_tower(on : bool) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        set_metal_tower(on)
    }
    static_if (typeinfo builtin_module_exists(vulkan)) {
        set_vulkan_tower(on)
    }
}

// Tier-1 qwen3v tower parity vs llama-mtmd-debug -p encode (the Omni-30B SigLIP-geometry ViT).
// Reference arm: the f32-widened mmprojs, CPU, -fa off (qwen3vl-vision-oracle/mint.sh mints the
// Omni encode.* dumps, mint_4b.sh the 4B deepstack encode4b.* dumps).
// Fixtures: pre-resized [0,1] float canvases - no preprocessing, so a diff isolates the tower math.
// The Vulkan twin skips on a build with das_metal - the Metal driver owns the tower hooks there - and
// without a Vulkan device under DASLLAMA_GPU=1.

let private TIER1_ABS = 2e-4
// x token rms. Poison-calibrated: 27 blocks whose GELU rides the f16 LUT quantize the
// summation-order drift between das GEMMs and the reference's to a measured floor of
// 0.6-3.2e-3 (the ff_pad width regroups the down GEMM's f32 accumulation - red448 tops the
// set; the solid-color fixtures sit at low rms, where the floor shows; preproc, the
// pos-table antialias resize, and the rope tables are each independently bit-exact vs their
// references). The live zero-layer poison leg below reds this bar at 100x+.
let private TIER1_REL = 1.5e-2

// the deepstack leg runs FOUR GELU-LUT'd merger chains per wide row (main + three taps) where
// the Omni runs one, and the q-probes read individual slice values - the compounded
// summation-order floor measures 1.12e-2 on the low-rms gray fixture (a skipped-tap poison
// lands at 6.9-9.7 on the same probes, 600x this bar)
let private TIER1_REL_DS = 4e-2

// x token rms - the q8 serving lane vs the f32 oracle: measured 0.42 x rms worst (gray448) -
// the qwen ViT's mid-network outlier channels roughly double gemma3v's 0.22-0.25 q8 floor.
// The zero-layer poison leg proves the bar still discriminates; the caption is the product gate.
// the Metal tower legs: the f32 mulmm stages f16 tiles (the gemma3v precedent, its own
// TIER1_REL_GPU = 4e-2), so the GPU chain's floor runs ~6x the CPU chain's on the low-rms
// uniform fixtures - measured 8.9e-3 worst on the Omni (gray448). The deepstack leg reads
// mid-network tap slices where that noise arrives unwashed by the final post-LN, so - the
// q8-ds precedent one bar up - the uniform gray fixture stays on the CPU set only and the
// GPU-ds legs run the structured fixtures (measured 0.8-1.0e-2). The live poison legs red
// these bars at 10-150x (exact zero-layer 0.87, q8 zero-layer 0.73, zeroed tap 11.4).
let private TIER1_REL_GPU = 4e-2
let private TIER1_REL_GPU_DS = 8e-2

let private TIER1_REL_Q8 = 5.2e-1
// the deepstack q8 leg reads tap slices SAMPLED MID-NETWORK (blocks 5/11/17), where the q8
// outlier noise arrives unwashed by the final post-LN: cb fixtures measure 0.51 x rms; the
// uniform gray fixture measures 7.9 x rms on the q1/q2/q3 slice probes and stays on the EXACT
// lane's set only - the zeroed-slices decoder control in test_vision_chat is the gate that
// proves the served slices still carry their signal
let private TIER1_REL_Q8_DS = 6.5e-1

def private gate_encode(t : T?; tw : Qwen3vTower; var st : Qwen3vState; kind : string; w, h : int; logname : string;
                        rel : float = TIER1_REL) : bool {
    var o = OracleEncode()
    let logpath = path_join(path_join(models_dir(), "qwen3vl-vision-oracle"), logname)
    if (!load_oracle_encode(logpath, o)) {
        t |> skip("oracle dump {logname} not present/parseable - mint via qwen3vl-vision-oracle/mint.sh (encode.*) / mint_4b.sh (encode4b.*)")
        return false
    }
    let planes <- make_planes(kind, w, h)
    var out : array<float>
    var nout = 0l
    with_job_que() {
        setup_dasllama_jobque_()
        nout = qwen3v_encode(tw, st, planes, int64(w), int64(h), out)
    }
    t |> equal(int(nout), o.npos, "{kind} {w}x{h} token count")
    if (int(nout) != o.npos) {
        return false
    }
    compare_encode(t, "qwen3v tier-1 {kind} {w}x{h}", out, int(qwen3v_row_width(tw)), o, TIER1_ABS, rel)
    return true
}

[test]
def test_qwen3v_tier1(t : T?) {
    t |> run("tier-1 tower parity vs the mtmd encode dumps (Omni-30B)") @(tt : T?) {
        let mm = path_join(models_dir(), "mmproj-Qwen3-Omni-30B-A3B-Instruct-bf16.gguf")
        if (!model_available(tt, mm)) {
            return
        }
        set_qwen3v_q8(false)   // the exact-plane lane; the q8 serving lane is its own test
        var tw <- load_qwen3v_tower(mm)
        reset_qwen3v_q8()
        set_every_gpu_tower(false)
        var st = Qwen3vState()
        tt |> success(!tw.q8, "exact-plane lane")
        tt |> equal(int(tw.patch), 16, "patch")
        tt |> equal(int(tw.merge), 2, "spatial merge")
        tt |> equal(int(tw.n_embd), 1152, "tower width")
        tt |> equal(int(tw.n_layer), 27, "blocks")
        tt |> equal(int(tw.n_head), 16, "heads")
        tt |> equal(int(tw.n_ff), 4304, "ffn width")
        tt |> equal(int(tw.proj_dim), 2048, "soft-token width")
        tt |> equal(int(tw.pos_side), 48, "pos-table side")
        tt |> success(tw.blk_bf16 && tw.mm_bf16, "the file carries bf16 GEMMs (the exact lane serves the blocks f32)")
        tt |> gate_encode(tw, st, "cb", 96, 96, "encode.cb96.log")   // 9 tokens: the pos-table DOWNSCALE arm
        tt |> gate_encode(tw, st, "gray", 448, 448, "encode.gray448.log")
        tt |> gate_encode(tw, st, "cb", 448, 448, "encode.cb448.log")
        tt |> gate_encode(tw, st, "red", 448, 448, "encode.red448.log")
        tt |> gate_encode(tw, st, "green", 448, 448, "encode.green448.log")
        tt |> gate_encode(tw, st, "blue", 448, 448, "encode.blue448.log")
        tt |> gate_encode(tw, st, "cb", 640, 320, "encode.cb640x320.log")   // non-square: the merge-reorder + transposed-grid gate
        var ragged_tripped = false
        try {
            var planes : array<float>
            planes |> reserve(48l * 32l * 3l)
            planes |> resize(48l * 32l * 3l)
            var out : array<float>
            qwen3v_encode(tw, st, planes, 48l, 32l, out)   // 48 is not a multiple of the 32 px merged patch
            delete out
            delete planes
        } recover {
            ragged_tripped = true
        }
        tt |> success(ragged_tripped, "a canvas off the 32 px merged-patch grid panics by name")
        set_every_gpu_tower(true)
        // negative control for the exact bar: layer 13's f32 layer_w region zeroed through the
        // staging planes must red the same fixture on the same bar (the loosened-bar rule)
        var o = OracleEncode()
        if (load_oracle_encode(path_join(path_join(models_dir(), "qwen3vl-vision-oracle"), "encode.gray448.log"), o)) {
            set_qwen3v_q8(false)
            var inscope pst <- stage_qwen3v_tower(mm)
            reset_qwen3v_q8()
            let base = pst.t.layer_w + 13l * pst.t.layer_w_stride
            for (i in range64(pst.t.layer_w_stride)) {
                pst.blob[base + i] = 0.0
            }
            for (i in range64(long_length(pst.wblob) > 0l ? pst.t.layer_w_stride : 0l)) {
                pst.wblob[base + i] = uint16(0)   // the baked twin serves the GPU route - a blob-only poison is laundered
            }
            var ptw = Qwen3vTower()
            mint_qwen3v_tower(pst, ptw)
            let planes <- make_planes("gray", 448, 448)
            var pout : array<float>
            with_job_que() {
                setup_dasllama_jobque_()
                qwen3v_encode(ptw, st, planes, 448l, 448l, pout)
            }
            let excess = encode_excess(pout, int(qwen3v_row_width(ptw)), o, TIER1_ABS, TIER1_REL)
            to_log(LOG_INFO, "qwen3v exact poison leg: zero layer 13 -> excess {excess} over the bar\n")
            tt |> success(excess > 0.0, "a zeroed mid-layer f32 region reds the exact bar (excess {excess})")
            delete pout
            delete ptw
        }
        delete st
        delete tw
    }
}

// The Metal tower arm: the SAME fixtures and bars, tower pinned ON — the GPU chain's parity
// gate, with engage evidence (encode/block counter deltas), never "the model ran".
[test]
def test_qwen3v_tier1_metal(t : T?) {   // nolint:STYLE038 — one flat gate ladder + its poison leg, honest shape
    t |> run("tier-1 on the Metal tower (Omni-30B narrow + 4B deepstack)") @(tt : T?) {
        static_if (typeinfo builtin_module_exists(das_metal)) {
            let mm = path_join(models_dir(), "mmproj-Qwen3-Omni-30B-A3B-Instruct-bf16.gguf")
            let mm4 = path_join(models_dir(), "mmproj-Qwen3VL-4B-Instruct-F16.gguf")
            if (!model_available(tt, mm) || !model_available(tt, mm4)) {
                return
            }
            set_qwen3v_q8(false)   // the Metal tower serves the f32 exact planes
            var tw <- load_qwen3v_tower(mm)
            var tw4 <- load_qwen3v_tower(mm4)
            reset_qwen3v_q8()
            set_metal_tower(true)
            var st = Qwen3vState()
            let s0 = metal_tower_stats()
            var omni_gated = 0l
            var ds_gated = 0l
            if (tt |> gate_encode(tw, st, "gray", 448, 448, "encode.gray448.log", TIER1_REL_GPU)) {
                omni_gated++
            }
            if (tt |> gate_encode(tw, st, "cb", 448, 448, "encode.cb448.log", TIER1_REL_GPU)) {
                omni_gated++
            }
            if (tt |> gate_encode(tw, st, "cb", 640, 320, "encode.cb640x320.log", TIER1_REL_GPU)) {
                omni_gated++
            }
            var st4 = Qwen3vState()
            if (tt |> gate_encode(tw4, st4, "cb", 448, 448, "encode4b.cb448.log", TIER1_REL_GPU_DS)) {
                ds_gated++
            }
            if (tt |> gate_encode(tw4, st4, "cb", 640, 320, "encode4b.cb640x320.log", TIER1_REL_GPU_DS)) {
                ds_gated++
            }
            if (tt |> gate_encode(tw4, st4, "cb", 96, 96, "encode4b.cb96.log", TIER1_REL_GPU_DS)) {
                ds_gated++
            }
            let s1 = metal_tower_stats()
            to_log(LOG_INFO, "qwen3v metal tier-1: +{s1.encodes - s0.encodes} encodes, +{s1.blocks - s0.blocks} blocks, +{s1.declines - s0.declines} declines ({omni_gated}+{ds_gated} fixtures gated)\n")
            if (omni_gated + ds_gated == 0l) {
                tt |> skip("no oracle dumps present - the GPU parity fixtures all skipped")
            } else {
                tt |> success(s1.encodes - s0.encodes >= omni_gated + ds_gated, "every gated encode served on the Metal tower")
                tt |> equal(s1.blocks - s0.blocks, omni_gated * 27l + ds_gated * 24l, "the block counters advanced per gated fixture")
            }
            // negative control for the GPU bars: the same zero-layer-13 poison, GPU-served,
            // must red TIER1_REL_GPU (the loosened-bar rule wants the control ON THIS lane)
            var po = OracleEncode()
            if (load_oracle_encode(path_join(path_join(models_dir(), "qwen3vl-vision-oracle"), "encode.gray448.log"), po)) {
                set_qwen3v_q8(false)
                var inscope pst <- stage_qwen3v_tower(mm)
                reset_qwen3v_q8()
                let base = pst.t.layer_w + 13l * pst.t.layer_w_stride
                for (i in range64(pst.t.layer_w_stride)) {
                    pst.blob[base + i] = 0.0
                }
                for (i in range64(long_length(pst.wblob) > 0l ? pst.t.layer_w_stride : 0l)) {
                    pst.wblob[base + i] = uint16(0)
                }
                var ptw = Qwen3vTower()
                mint_qwen3v_tower(pst, ptw)
                let planes <- make_planes("gray", 448, 448)
                var pout : array<float>
                with_job_que() {
                    setup_dasllama_jobque_()
                    qwen3v_encode(ptw, st, planes, 448l, 448l, pout)
                }
                let excess = encode_excess(pout, int(qwen3v_row_width(ptw)), po, TIER1_ABS, TIER1_REL_GPU)
                to_log(LOG_INFO, "qwen3v GPU-lane poison leg: zero layer 13 -> excess {excess} over the GPU bar\n")
                tt |> success(excess > 0.0, "a zeroed mid-layer reds the GPU bar on the GPU-served lane (excess {excess})")
                delete pout
                delete ptw
            }
            delete st
            delete st4
            delete tw
            delete tw4
        } else {
            tt |> skip("no das_metal on this build - the Metal tower arm has no device to serve")
        }
    }
}

[test, arch(at="../ARCHITECTURE_IMAGE.md#image-tower-twin-plane")]
def test_qwen3v_tier1_metal_f16(t : T?) {
    t |> run("tier-1 on the Metal tower's twin-W route (crowned twins, 4B f16 + Omni bf16)") @(tt : T?) {
        static_if (typeinfo builtin_module_exists(das_metal)) {
            let mm4 = path_join(models_dir(), "mmproj-Qwen3VL-4B-Instruct-F16.gguf")
            if (!model_available(tt, mm4)) {
                return
            }
            set_metal_tensor_crowns("mulmm_q8")
            metal_prefill_shutdown()
            set_qwen3v_q8(false)
            var tw4 <- load_qwen3v_tower(mm4)
            reset_qwen3v_q8()
            set_metal_tower(true)
            set_metal_tower_f16(true)
            var st4 = Qwen3vState()
            let f0 = metal_tower_f16_encodes()
            var gated = 0l
            if (tt |> gate_encode(tw4, st4, "cb", 448, 448, "encode4b.cb448.log", TIER1_REL_GPU_DS)) {
                gated++
            }
            if (tt |> gate_encode(tw4, st4, "cb", 96, 96, "encode4b.cb96.log", TIER1_REL_GPU_DS)) {
                gated++
            }
            let f1 = metal_tower_f16_encodes()
            if (gated == 0l) {
                tt |> skip("no oracle dumps present - the f16-route parity fixtures all skipped")
            } else {
                tt |> success(f1 - f0 >= gated, "every gated encode rode the f16-W route ({f1 - f0} of {gated})")
                set_metal_tower_f16(false)
                tt |> gate_encode(tw4, st4, "cb", 448, 448, "encode4b.cb448.log", TIER1_REL_GPU_DS)
                tt |> equal(metal_tower_f16_encodes() - f1, 0l, "the knob-off leg keeps the f32 GEMM route")
                set_metal_tower_f16(true)
            }
            let mm = path_join(models_dir(), "mmproj-Qwen3-Omni-30B-A3B-Instruct-bf16.gguf")
            if (model_available(tt, mm)) {
                set_qwen3v_q8(false)
                var tw <- load_qwen3v_tower(mm)
                reset_qwen3v_q8()
                var st = Qwen3vState()
                let b0 = metal_tower_f16_encodes()
                if (tt |> gate_encode(tw, st, "cb", 448, 448, "encode.cb448.log", TIER1_REL_GPU)) {
                    tt |> success(metal_tower_f16_encodes() - b0 >= 1l, "the bf16-sourced tower rode the twin route")
                }
                delete st
                delete tw
            }
            set_metal_tower_f16(true)
            set_metal_tensor_crowns("")
            metal_prefill_shutdown()
            delete st4
            delete tw4
        } else {
            tt |> skip("no das_metal on this build - the f16-W route has no device to serve")
        }
    }
}

[test]
def test_qwen3v_tier1_deepstack(t : T?) {
    t |> run("tier-1 deepstack tower parity vs the mtmd encode dumps (Qwen3-VL 4B)") @(tt : T?) {
        let mm = path_join(models_dir(), "mmproj-Qwen3VL-4B-Instruct-F16.gguf")
        if (!model_available(tt, mm)) {
            return
        }
        set_qwen3v_q8(false)   // the exact-plane lane; the q8 serving lane is its own test
        var tw <- load_qwen3v_tower(mm)
        reset_qwen3v_q8()
        set_every_gpu_tower(false)
        var st = Qwen3vState()
        tt |> success(!tw.q8, "exact-plane lane")
        tt |> equal(int(tw.n_embd), 1024, "tower width")
        tt |> equal(int(tw.n_layer), 24, "blocks")
        tt |> equal(int(tw.n_ff), 4096, "ffn width")
        tt |> equal(int(tw.proj_dim), 2560, "soft-token width (main slice)")
        tt |> equal(int(tw.n_deepstack), 3, "deepstack taps")
        tt |> equal(int(tw.ds_l0), 5, "tap 0 block")
        tt |> equal(int(tw.ds_l1), 11, "tap 1 block")
        tt |> equal(int(tw.ds_l2), 17, "tap 2 block")
        tt |> equal(int(qwen3v_row_width(tw)), 10240, "wide row = (1+3) x 2560")
        tt |> gate_encode(tw, st, "gray", 448, 448, "encode4b.gray448.log", TIER1_REL_DS)
        tt |> gate_encode(tw, st, "cb", 448, 448, "encode4b.cb448.log", TIER1_REL_DS)
        tt |> gate_encode(tw, st, "cb", 640, 320, "encode4b.cb640x320.log", TIER1_REL_DS)   // non-square: merge-reorder + transposed-grid
        tt |> gate_encode(tw, st, "cb", 96, 96, "encode4b.cb96.log", TIER1_REL_DS)   // 9 tokens: the pos-table DOWNSCALE arm
        set_every_gpu_tower(true)
        delete st
        delete tw
    }
}

// The q8 serving lane (the CPU default): the same dumps on their own scale-relative bars.
[test]
def test_qwen3v_tier1_q8(t : T?) {
    t |> run("tier-1 on the q8 block-GEMM lane (the serving default, Omni-30B)") @(tt : T?) {
        let mm = path_join(models_dir(), "mmproj-Qwen3-Omni-30B-A3B-Instruct-bf16.gguf")
        if (!model_available(tt, mm)) {
            return
        }
        set_qwen3v_q8(true)   // a CPU-lane claim pins the covering knob, never relies on the policy
        var tw <- load_qwen3v_tower(mm)
        reset_qwen3v_q8()
        set_every_gpu_tower(false)
        var st = Qwen3vState()
        tt |> success(tw.q8, "q8 lane loaded")
        tt |> equal(int(tw.ff_pad), 4352, "ffn served at the layout's padded width")
        tt |> gate_encode(tw, st, "gray", 448, 448, "encode.gray448.log", TIER1_REL_Q8)
        tt |> gate_encode(tw, st, "cb", 448, 448, "encode.cb448.log", TIER1_REL_Q8)
        tt |> gate_encode(tw, st, "cb", 96, 96, "encode.cb96.log", TIER1_REL_Q8)
        // negative control: layer 13's qblob region zeroed through the staging planes (the
        // gemma4v knockout shape) must red the same fixture on the same bar
        var o = OracleEncode()
        if (load_oracle_encode(path_join(path_join(models_dir(), "qwen3vl-vision-oracle"), "encode.gray448.log"), o)) {
            set_qwen3v_q8(true)
            var inscope pst <- stage_qwen3v_tower(mm)
            reset_qwen3v_q8()
            let base = pst.t.layer_w + 13l * pst.t.layer_w_stride
            for (i in range64(pst.t.layer_w_stride)) {
                pst.qblob[base + i] = int8(0)
            }
            var ptw = Qwen3vTower()
            mint_qwen3v_tower(pst, ptw)
            let planes <- make_planes("gray", 448, 448)
            var pout : array<float>
            with_job_que() {
                setup_dasllama_jobque_()
                qwen3v_encode(ptw, st, planes, 448l, 448l, pout)
            }
            let excess = encode_excess(pout, int(qwen3v_row_width(ptw)), o, TIER1_ABS, TIER1_REL_Q8)
            to_log(LOG_INFO, "qwen3v q8 poison leg: zero layer 13 -> excess {excess} over the bar\n")
            tt |> success(excess > 0.0, "a zeroed mid-layer qblob region reds the bar (excess {excess})")
            delete pout
            delete ptw
        }
        // the q8-lane .dlim's baked tag routes through the carrier sniff (the qwen3v-q8 arm)
        let img = image_path_for(mm, "qwen3v-q8")
        if (stat(img).is_valid) {
            var inscope e <- load_vision_embedder(img)
            tt |> equal(vision_family(e), "qwen3v", "the baked q8 tag sniffs to the qwen3v arm")
            tt |> equal(vision_exec_fmt(e), "q8", "the carrier stamps the q8 lane")
        }
        set_every_gpu_tower(true)
        delete st
        delete tw
    }
}

let private TWIN_SLACK_VK = 1.5   //! 1.5x: the q8 chain's activation quants plus f16 weight scales, the tiles' summation order, the closed-form GELU against the f16 LUT

//! a tower minted in memory from the staged planes under the lane pin (pinned, staged, the pin restored): `nl` blocks (0 = the
//! file's count); `zero_block` >= 0 zeroes that block's q8 GEMM planes - the input poison of the device-side bar (q8 lane).
//! Never `load_qwen3v_tower`, which bakes a `.dlim` under the pin.
def private mint_tower(mm : string; q8 : bool; nl : int64 = 0l; zero_block : int64 = -1l) : Qwen3vTower {
    set_qwen3v_q8(q8)
    var inscope stg <- stage_qwen3v_tower(mm)
    reset_qwen3v_q8()
    if (nl > 0l) {
        stg.t.n_layer = nl
    }
    if (zero_block >= 0l) {
        let base = stg.t.layer_w + zero_block * stg.t.layer_w_stride
        for (i in range64(stg.t.layer_w_stride)) {
            stg.qblob[base + i] = int8(0)
        }
    }
    var tw = Qwen3vTower()
    mint_qwen3v_tower(stg, tw)
    return <- tw
}

//! one canvas three ways - the exact chain, the CPU q8 chain, the device chain - with the counters and the bar; `zero_block` >= 0
//! adds the input poison: the device chain over a fresh q8 tower with that block zeroed must EXCEED the bar the clean chains set
def private twin_encode(tt : T?; tw : Qwen3vTower; var st : Qwen3vState; tw_exact : Qwen3vTower; var st_exact : Qwen3vState;
                        kind : string; w, h : int; mm : string; zero_block : int64 = -1l) {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        let planes <- make_planes(kind, w, h)
        var cpu : array<float>
        var gpu : array<float>
        var exact : array<float>
        var ncpu = 0l
        var ngpu = 0l
        set_vulkan_tower(false)
        with_job_que() {
            setup_dasllama_jobque_()
            qwen3v_encode(tw_exact, st_exact, planes, int64(w), int64(h), exact)
        }
        let d0 = vulkan_tower_stats()
        with_job_que() {
            setup_dasllama_jobque_()
            ncpu = qwen3v_encode(tw, st, planes, int64(w), int64(h), cpu)
        }
        let d1 = vulkan_tower_stats()
        set_vulkan_tower(true)
        with_job_que() {
            setup_dasllama_jobque_()
            ngpu = qwen3v_encode(tw, st, planes, int64(w), int64(h), gpu)
        }
        let d2 = vulkan_tower_stats()
        if (d2.encodes == d1.encodes) {
            to_log(LOG_INFO, "qwen3v vulkan twin {kind} {w}x{h}: the driver declined - {vulkan_tower_declines()}\n")
        }
        tt |> equal(ncpu, ngpu, "{kind} {w}x{h}: token count on both chains")
        tt |> equal(d1.encodes - d0.encodes, 0l, "{kind} {w}x{h}: the knob-off leg never dispatched")
        tt |> equal(d1.declines - d0.declines, 1l, "{kind} {w}x{h}: the knob-off leg recorded the knob decline")
        tt |> equal(d2.encodes - d1.encodes, 1l, "{kind} {w}x{h}: device-served (encodes delta)")
        tt |> equal(d2.blocks - d1.blocks, tw.n_layer, "{kind} {w}x{h}: every block ran on the driver (blocks delta)")
        tt |> equal(d2.rows - d1.rows, int64(w / 16) * int64(h / 16), "{kind} {w}x{h}: the rows delta is the patch grid")
        var ss = 0.0
        for (a in exact) {
            ss += a * a
        }
        let rms = sqrt(ss / float(max(length(exact), 1)))
        let q8_ex = logits_maxdiff(cpu, exact)
        let dev_ex = logits_maxdiff(gpu, exact)
        let dev_q8 = logits_maxdiff(gpu, cpu)
        let bar = TWIN_SLACK_VK * q8_ex + TIER1_ABS
        to_log(LOG_INFO, "qwen3v vulkan twin {kind} {w}x{h} ({tw.n_layer} blocks, {tw.n_deepstack} taps): rms {rms}; vs exact: cpu q8 {q8_ex} ({q8_ex / rms} x rms), device {dev_ex} ({dev_ex / rms} x rms); device vs cpu q8 {dev_q8} ({dev_q8 / rms} x rms)\n")
        tt |> success(dev_ex <= bar, "{kind} {w}x{h}: the device chain sits within {TWIN_SLACK_VK}x the CPU q8 chain's distance from the exact chain ({dev_ex} vs {q8_ex})")
        if (zero_block >= 0l) {
            var twp <- mint_tower(mm, true, tw.n_layer, zero_block)
            var stp = Qwen3vState()
            var poisoned : array<float>
            let p0 = vulkan_tower_stats()
            with_job_que() {
                setup_dasllama_jobque_()
                qwen3v_encode(twp, stp, planes, int64(w), int64(h), poisoned)
            }
            tt |> equal(vulkan_tower_stats().encodes - p0.encodes, 1l, "{kind} {w}x{h} poison: device-served (encodes delta)")
            let poison_ex = logits_maxdiff(poisoned, exact)
            to_log(LOG_INFO, "qwen3v vulkan twin {kind} {w}x{h} ({tw.n_layer} blocks) poison: block {zero_block} zeroed on the device lands {poison_ex} from the exact chain ({poison_ex / rms} x rms), bar {bar}\n")
            tt |> success(poison_ex > bar, "{kind} {w}x{h}: a zeroed block through the device chain EXCEEDS the bar ({poison_ex} vs {bar})")
            delete stp
            delete twp
        }
    }
}

[test, arch(at = "../ARCHITECTURE_GPU_TOWER.md#vk-tower-encode-chains")]
def test_qwen3v_vulkan_twin(t : T?) {
    t |> run("the Vulkan block loop vs the CPU q8 chain (Qwen3-VL 4B, deepstack), and the exact-lane decline") @(tt : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            static_if (typeinfo builtin_module_exists(das_metal)) {
                tt |> skip("das_metal on this build owns the tower hooks - the Vulkan twin has no seat")
            } else {
                let mm = path_join(models_dir(), "mmproj-Qwen3VL-4B-Instruct-F16.gguf")
                if (!model_available(tt, mm)) {
                    return
                }
                if (!vk_moe_init()) {
                    tt |> skip("no Vulkan device (DASLLAMA_GPU=1 and a device the tier serves)")
                    return
                }
                var tw_exact <- mint_tower(mm, false)
                var st_exact = Qwen3vState()
                tt |> success(!tw_exact.q8 && tw_exact.n_deepstack == 3l, "exact-plane lane minted, three deepstack taps")
                for (nl in [1l, 2l, 6l]) {
                    var twxs <- mint_tower(mm, false, nl)
                    var twq <- mint_tower(mm, true, nl)
                    var stq = Qwen3vState()
                    var stxs = Qwen3vState()
                    tt |> success(twq.q8 && !twxs.q8 && twq.n_layer == nl && twxs.n_layer == nl, "the {nl}-block towers, q8 and exact")
                    twin_encode(tt, twq, stq, twxs, stxs, "cb", 96, 96, mm, nl == 1l ? 0l : -1l)   // the poison on the one-block tower, where the bar is tightest
                    twin_encode(tt, twq, stq, twxs, stxs, "cb", 448, 448, mm)
                    delete stq
                    delete stxs
                    delete twq
                    delete twxs
                }
                var tw <- mint_tower(mm, true)
                var st = Qwen3vState()
                tt |> success(tw.q8, "q8 lane minted")
                twin_encode(tt, tw, st, tw_exact, st_exact, "cb", 96, 96, mm)
                twin_encode(tt, tw, st, tw_exact, st_exact, "cb", 448, 448, mm)
                twin_encode(tt, tw, st, tw_exact, st_exact, "cb", 640, 320, mm)   // non-square: the merge-reorder and the transposed grid; no gray canvas - the q8 lane is no instrument there (its own cells keep gray on the exact set)
                delete st
                delete tw
                let planes <- make_planes("cb", 96, 96)
                var outx : array<float>
                set_vulkan_tower(true)
                let x0 = vulkan_tower_stats()
                let dq0 = vulkan_tower_declines()?["quant_mode"] ?? 0l
                with_job_que() {
                    setup_dasllama_jobque_()
                    qwen3v_encode(tw_exact, st_exact, planes, 96l, 96l, outx)
                }
                let x1 = vulkan_tower_stats()
                tt |> equal(x1.encodes - x0.encodes, 0l, "the exact-lane tower never dispatched")
                tt |> equal((vulkan_tower_declines()?["quant_mode"] ?? 0l) - dq0, 1l, "the exact-lane tower reached the blocks seat and the driver declined quant_mode")
                tt |> success(long_length(outx) == 9l * qwen3v_row_width(tw_exact), "the CPU chain served the exact-lane encode")
                delete st_exact
                delete tw_exact
                vulkan_tower_shutdown()
            }
        } else {
            tt |> skip("dasVulkan not present")
        }
    }
}

[test]
def test_qwen3v_tier1_q8_deepstack(t : T?) {
    t |> run("tier-1 deepstack on the q8 block-GEMM lane (Qwen3-VL 4B)") @(tt : T?) {
        let mm = path_join(models_dir(), "mmproj-Qwen3VL-4B-Instruct-F16.gguf")
        if (!model_available(tt, mm)) {
            return
        }
        set_qwen3v_q8(true)   // a CPU-lane claim pins the covering knob, never relies on the policy
        var tw <- load_qwen3v_tower(mm)
        reset_qwen3v_q8()
        set_every_gpu_tower(false)
        var st = Qwen3vState()
        tt |> success(tw.q8, "q8 lane loaded")
        tt |> gate_encode(tw, st, "cb", 448, 448, "encode4b.cb448.log", TIER1_REL_Q8_DS)
        tt |> gate_encode(tw, st, "cb", 96, 96, "encode4b.cb96.log", TIER1_REL_Q8_DS)
        // negative control for THIS bar: the same zero-layer knockout must red it (tap 1's
        // block, so the poison hits both the main path and a served slice)
        var o = OracleEncode()
        if (load_oracle_encode(path_join(path_join(models_dir(), "qwen3vl-vision-oracle"), "encode4b.cb448.log"), o)) {
            set_qwen3v_q8(true)
            var inscope pst <- stage_qwen3v_tower(mm)
            reset_qwen3v_q8()
            let base = pst.t.layer_w + pst.t.ds_l1 * pst.t.layer_w_stride
            for (i in range64(pst.t.layer_w_stride)) {
                pst.qblob[base + i] = int8(0)
            }
            var ptw = Qwen3vTower()
            mint_qwen3v_tower(pst, ptw)
            let planes <- make_planes("cb", 448, 448)
            var pout : array<float>
            with_job_que() {
                setup_dasllama_jobque_()
                qwen3v_encode(ptw, st, planes, 448l, 448l, pout)
            }
            let excess = encode_excess(pout, int(qwen3v_row_width(ptw)), o, TIER1_ABS, TIER1_REL_Q8_DS)
            to_log(LOG_INFO, "qwen3v deepstack q8 poison leg: zero the tap-1 block -> excess {excess} over the bar\n")
            tt |> success(excess > 0.0, "a zeroed tap block reds the deepstack bar (excess {excess})")
            delete pout
            delete ptw
        }
        set_every_gpu_tower(true)
        delete st
        delete tw
    }
}

// the lane policy itself - model-free: the default follows the box's accelerate tier (q8 when
// the tier is off), the pin flips it either way, reset returns to the policy
[test]
def test_qwen3v_lane_knob(t : T?) {
    t |> run("the q8 lane pin and policy default") @(tt : T?) {
        tt |> equal(qwen3v_serves_q8(), !(qwen3v_gpu_would_serve() || float_batch_override_active()), "the policy default follows the Metal tower and the accelerate tier")
        set_qwen3v_q8(true)
        tt |> success(qwen3v_serves_q8(), "pinned q8")
        set_qwen3v_q8(false)
        tt |> success(!qwen3v_serves_q8(), "pinned exact")
        reset_qwen3v_q8()   // from the EXACT pin, so a no-op reset cannot pass as the (q8) policy default
        tt |> equal(qwen3v_serves_q8(), !(qwen3v_gpu_would_serve() || float_batch_override_active()), "reset returns to the policy default")
    }
}
