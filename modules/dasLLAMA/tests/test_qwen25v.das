options gen2
options persistent_heap
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama_qwen25v
require dasllama/dasllama_math   // setup_dasllama_jobque_
require daslib/jobque_boost
require daslib/fio
require _model_tier   // models_dir() + model_available() gate
require ?das_metal dasllama/dasllama_metal_tower   // the CPU gates pin the tower knob off
require ?das_metal dasllama/dasllama_metal_prefill   // metal_prefill_shutdown - the twin-W leg re-arms crowned PSOs
require ?vulkan dasllama/dasllama_vulkan_tower   // the Vulkan block loop over the halfword twin; the CPU gates pin its knob off too
require ?vulkan dasllama/dasllama_vulkan_common   // vk_moe_init - whether the tier has a device
require dasllama/dasllama_common   // set_metal_tensor_crowns - the twin-W leg's crown pin
require _vision_oracle   // the mtmd dump parser, fixture generators, per-token compare
require math   // nolint:STYLE030,LINT019 — used only inside the GPU static_if arms; the no-GPU lane sees it unused

//! the CPU-lane claims pin every GPU tower driver off (a hook that serves silently stops the cell measuring the CPU chain)
def private gpu_towers(on : bool) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        set_metal_tower(on)
    }
    static_if (typeinfo builtin_module_exists(vulkan)) {
        set_vulkan_tower(on)
    }
}

// Tier-1 qwen25v tower parity vs llama-mtmd-debug -p encode (the Qwen2.5-Omni window ViT).
// Reference arm: the f32-widened mmproj, CPU, -fa off (qwen3vl-vision-oracle/mint_25o.sh).
// Fixtures: pre-resized [0,1] float canvases - no preprocessing, so a diff isolates the tower
// math; cb448 = four full windows, cb112 = the single-window arm, cb616x336 = ragged windows.

let private TIER1_ABS = 2e-4
// x token rms; one GELU-LUT chain (the merger) rides the qwen3v Omni floor
let private TIER1_REL = 1e-2
//! measured up to ~4e-2 rel on this 32-layer mix: 2x headroom, and a structural break
//! (zeroed-block class, >= 0.9) still reds
let private TIER1_REL_GPU = 8e-2

def private gate_encode(t : T?; tw : Qwen25vTower; var st : Qwen25vState; kind : string; w, h : int; logname : string; rel = TIER1_REL) : bool {
    var o = OracleEncode()
    let logpath = path_join(path_join(models_dir(), "qwen3vl-vision-oracle"), logname)
    if (!load_oracle_encode(logpath, o)) {
        t |> skip("oracle dump {logname} not present/parseable - mint via qwen3vl-vision-oracle/mint_25o.sh")
        return false
    }
    let planes <- make_planes(kind, w, h)
    var out : array<float>
    var nout = 0l
    with_job_que() {
        setup_dasllama_jobque_()
        nout = qwen25v_encode(tw, st, planes, int64(w), int64(h), out)
    }
    t |> equal(int(nout), o.npos, "{kind} {w}x{h} token count")
    if (int(nout) != o.npos) {
        return false
    }
    compare_encode(t, "qwen25v tier-1 {kind} {w}x{h}", out, int(tw.proj_dim), o, TIER1_ABS, rel)
    return true
}

[test]
def test_qwen25v_tier1(t : T?) {
    t |> run("tier-1 window-ViT parity vs the mtmd encode dumps (Qwen2.5-Omni-3B)") @(tt : T?) {
        let mm = path_join(models_dir(), "mmproj-Qwen2.5-Omni-3B-f16.gguf")
        if (!model_available(tt, mm)) {
            return
        }
        var tw <- load_qwen25v_tower(mm)
        gpu_towers(false)   // this gate owns the CPU chain - the dumps are CPU-minted
        var st = Qwen25vState()
        tt |> equal(int(tw.patch), 14, "patch")
        tt |> equal(int(tw.merge), 2, "spatial merge")
        tt |> equal(int(tw.n_embd), 1280, "tower width")
        tt |> equal(int(tw.n_layer), 32, "blocks")
        tt |> equal(int(tw.n_head), 16, "heads")
        tt |> equal(int(tw.n_ff), 3420, "gated hidden (from the tensor dims - the metadata lies)")
        tt |> equal(int(tw.proj_dim), 2048, "soft-token width")
        tt |> equal(int(tw.n_wa_pattern), 8, "full attention every 8th block")
        tt |> equal(int(tw.win_px), 112, "attention window")
        tt |> gate_encode(tw, st, "cb", 112, 112, "encode25.cb112.log")   // 16 tokens: window == canvas
        tt |> gate_encode(tw, st, "gray", 448, 448, "encode25.gray448.log")
        tt |> gate_encode(tw, st, "cb", 448, 448, "encode25.cb448.log")   // four full 4x4-group windows
        tt |> gate_encode(tw, st, "cb", 616, 336, "encode25.cb616x336.log")   // ragged window edges + transposed-grid
        tt |> gate_encode(tw, st, "quad", 448, 448, "encode25.quad448.log")   // WINDOW discriminator: uniform/periodic fixtures hide a broken mask
        var ragged_tripped = false
        try {
            var planes : array<float>
            planes |> reserve(42l * 28l * 3l)
            planes |> resize(42l * 28l * 3l)
            var out : array<float>
            qwen25v_encode(tw, st, planes, 42l, 28l, out)   // 42 is not a multiple of the 28 px merged patch
            delete out
            delete planes
        } recover {
            ragged_tripped = true
        }
        tt |> success(ragged_tripped, "a canvas off the 28 px merged-patch grid panics by name")
        gpu_towers(true)
        delete st
        delete tw
    }
}

let private TWIN_ABS_VK = 2e-4
//! the Vulkan twin's bars, the device chain vs the exact CPU chain (no q8 lane in this family): to eight blocks the route
//! holds 1e-2 x rms on both arms (measured 1.0e-3..4.5e-3); the whole tower holds the Metal rung's order - the f16 GEMM
//! feed and the full layers' f16 K/V shadows re-roll the late blocks' outlier channels (measured 0.02..0.09 x rms at 32
//! blocks; with the window layers on f16 K/V shadows it read 0.16..1.53, which is why the window route reads f32)
let private TWIN_REL_VK_SHALLOW = 1e-2
let private TWIN_REL_VK_DEEP = 0.15

def private encode_maxdiff(a, b : array<float>) : float {
    var m = 0.0
    for (x, y in a, b) {
        m = max(m, abs(x - y))
    }
    return m
}

//! one canvas through the exact CPU chain and the Vulkan block loop over the halfword twin: the counters prove which served,
//! the rows hold to the f16 route's bar, a poisoned element the control that the bar reds
def private twin_encode(tt : T?; tw : Qwen25vTower; var st : Qwen25vState; kind : string; w, h : int) {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        let planes <- make_planes(kind, w, h)
        var cpu : array<float>
        var gpu : array<float>
        var ncpu = 0l
        var ngpu = 0l
        set_vulkan_tower(false)
        let d0 = vulkan_tower_stats()
        with_job_que() {
            setup_dasllama_jobque_()
            ncpu = qwen25v_encode(tw, st, planes, int64(w), int64(h), cpu)
        }
        let d1 = vulkan_tower_stats()
        set_vulkan_tower(true)
        with_job_que() {
            setup_dasllama_jobque_()
            ngpu = qwen25v_encode(tw, st, planes, int64(w), int64(h), gpu)
        }
        let d2 = vulkan_tower_stats()
        if (d2.encodes == d1.encodes) {
            to_log(LOG_INFO, "qwen25v vulkan twin {kind} {w}x{h}: the driver declined - {vulkan_tower_declines()}\n")
        }
        tt |> equal(ncpu, ngpu, "{kind} {w}x{h}: token count on both chains")
        tt |> equal(d1.encodes - d0.encodes, 0l, "{kind} {w}x{h}: the knob-off leg never dispatched")
        tt |> equal(d1.declines - d0.declines, 1l, "{kind} {w}x{h}: the knob-off leg recorded the knob decline")
        tt |> equal(d2.encodes - d1.encodes, 1l, "{kind} {w}x{h}: device-served (encodes delta)")
        tt |> equal(d2.blocks - d1.blocks, tw.n_layer, "{kind} {w}x{h}: every block ran on the driver (blocks delta)")
        tt |> equal(d2.rows - d1.rows, int64(w / 14) * int64(h / 14), "{kind} {w}x{h}: the rows delta is the patch grid")
        var ss = 0.0
        for (a in cpu) {
            ss += a * a
        }
        let rms = sqrt(ss / float(max(length(cpu), 1)))
        let dev_ex = encode_maxdiff(gpu, cpu)
        let rel = tw.n_layer <= 8l ? TWIN_REL_VK_SHALLOW : TWIN_REL_VK_DEEP
        let bar = TWIN_ABS_VK + rel * rms
        // the error's shape: NaNs the maxdiff cannot see, the worst token, the tokens past the bar
        var nans = 0
        var worst_tok = -1
        var worst = 0.0
        var over = 0
        let pd = int(tw.proj_dim)
        for (tok in range(length(cpu) / pd)) {
            var m = 0.0
            for (i in range(pd)) {
                let a = gpu[tok * pd + i]
                if (is_nan(a)) {
                    nans++
                }
                m = max(m, abs(a - cpu[tok * pd + i]))
            }
            if (m > bar) {
                over++
            }
            if (m > worst) {
                worst = m
                worst_tok = tok
            }
        }
        to_log(LOG_INFO, "qwen25v vulkan twin {kind} {w}x{h} ({tw.n_layer} blocks, full every {tw.n_wa_pattern}): rms {rms}; device vs exact {dev_ex} ({dev_ex / rms} x rms); NaNs {nans}, worst token {worst_tok} of {length(cpu) / pd}, {over} tokens past the bar\n")
        tt |> success(dev_ex <= bar, "{kind} {w}x{h}: the device chain holds to the exact chain within {rel} x rms ({dev_ex / rms} x rms)")
        tt |> success(abs((gpu[0] + 2.0 * bar) - cpu[0]) > bar, "{kind} {w}x{h}: a poisoned element reds the bar")
    }
}

//! the routing discrimination on the driver: a one-block tower on quad content, pure-window (wa 99) and all-full (wa 1), each
//! against the CPU chain at 0.1 abs - the bar the Metal precedent holds (one layer's GPU noise measures ~2.6e-2 abs peak,
//! a mixed-quadrant routing defect >= 1.0)
def private shallow_routing_cell_vk(t : T?; mm, name : string; wa : int64) {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        var inscope st <- stage_qwen25v_tower(mm)
        st.t.n_layer = 1l
        st.t.n_wa_pattern = wa
        var tw = Qwen25vTower()
        mint_qwen25v_tower(st, tw)
        let planes <- make_planes("quad", 448, 448)
        var sg = Qwen25vState()
        var og : array<float>
        set_vulkan_tower(true)
        let e0 = vulkan_tower_stats().encodes
        var ng = 0l
        with_job_que() {
            setup_dasllama_jobque_()
            ng = qwen25v_encode(tw, sg, planes, 448l, 448l, og)
        }
        t |> equal(vulkan_tower_stats().encodes - e0, 1l, "shallow {name}: device-served")
        var sc = Qwen25vState()
        var oc : array<float>
        set_vulkan_tower(false)
        var nc = 0l
        with_job_que() {
            setup_dasllama_jobque_()
            nc = qwen25v_encode(tw, sc, planes, 448l, 448l, oc)
        }
        set_vulkan_tower(true)
        t |> equal(int(ng), int(nc), "shallow {name}: token counts agree")
        let md = encode_maxdiff(og, oc)
        t |> success(md < 0.1, "shallow {name}: device == CPU on quad within 0.1 abs (measured {md})")
        delete og
        delete oc
        delete sg
        delete sc
        delete tw
    } else {
        t |> skip("no dasVulkan on this build: {mm} {name} {wa} unread")
    }
}

// The Vulkan twin: the device block loop against the exact CPU chain (this family has no q8 lane) on
// the shaped fixtures - the single window, four full windows, the ragged edges - and the shallow
// routing cells; quad content stays off the deep numeric set as it does on the Metal rung.
[test, arch(at = "../ARCHITECTURE_GPU_TOWER.md#vk-tower-encode-chains")]
def test_qwen25v_vulkan_twin(t : T?) {
    t |> run("the Vulkan block loop vs the exact CPU chain, and the window routing") @(tt : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            let mm = path_join(models_dir(), "mmproj-Qwen2.5-Omni-3B-f16.gguf")
            if (!model_available(tt, mm)) {
                return
            }
            if (!vk_moe_init()) {
                tt |> skip("no Vulkan device (DASLLAMA_GPU=1 and a device the tier serves)")
                return
            }
            for (nl in [1l, 2l, 8l]) {   //! the shallow twins first, on the tight bar: a drift names its depth before the deep mix re-rolls it
                var inscope stg <- stage_qwen25v_tower(mm)
                stg.t.n_layer = nl
                var tws = Qwen25vTower()
                mint_qwen25v_tower(stg, tws)
                var sts = Qwen25vState()
                twin_encode(tt, tws, sts, "cb", 448, 448)
                twin_encode(tt, tws, sts, "cb", 616, 336)
                delete sts
                delete tws
            }
            var tw <- load_qwen25v_tower(mm)
            var st = Qwen25vState()
            tt |> success(tw.wblob.n > 0l && !tw.blk_bf16, "the halfword twin is baked from an f16 file: the driver's lane")
            twin_encode(tt, tw, st, "cb", 112, 112)
            twin_encode(tt, tw, st, "cb", 448, 448)
            twin_encode(tt, tw, st, "cb", 616, 336)
            delete st
            delete tw
            shallow_routing_cell_vk(tt, mm, "window", 99l)
            shallow_routing_cell_vk(tt, mm, "full", 1l)
            vulkan_tower_shutdown()
        } else {
            feint("dasVulkan is not built on this platform - the twin needs it\n")
        }
    }
}

[test]
def test_qwen25v_tier1_gpu(t : T?) {
    t |> run("tier-1 GPU attempt vs the mtmd encode dumps (scale-relative rung)") @(tt : T?) {
        static_if (typeinfo builtin_module_exists(das_metal)) {
            let mm = path_join(models_dir(), "mmproj-Qwen2.5-Omni-3B-f16.gguf")
            if (!model_available(tt, mm)) {
                return
            }
            set_metal_tower(true)
            var tw <- load_qwen25v_tower(mm)
            var st = Qwen25vState()
            let fixtures = [(kind = "cb", w = 112, h = 112, log = "encode25.cb112.log"),
                            (kind = "gray", w = 448, h = 448, log = "encode25.gray448.log"),
                            (kind = "cb", w = 448, h = 448, log = "encode25.cb448.log"),
                            (kind = "cb", w = 616, h = 336, log = "encode25.cb616x336.log")]
            for (fx in fixtures) {
                let s0 = metal_tower_stats()
                let ran = gate_encode(tt, tw, st, fx.kind, fx.w, fx.h, fx.log, TIER1_REL_GPU)
                if (ran) {
                    tt |> equal(metal_tower_stats().encodes - s0.encodes, 1l, "{fx.kind} {fx.w}x{fx.h}: GPU-served (encodes delta)")
                    tt |> equal(metal_tower_stats().blocks - s0.blocks, tw.n_layer, "{fx.kind} {fx.w}x{fx.h}: every block ran on the driver (blocks delta)")
                }
            }
            set_metal_tensor_crowns("mulmm_q8")
            metal_prefill_shutdown()
            set_metal_tower_f16(true)   // the twin route's own knob - established, never inherited
            let tw0 = metal_tower_f16_encodes()
            if (gate_encode(tt, tw, st, "cb", 448, 448, "encode25.cb448.log", TIER1_REL_GPU)) {
                tt |> success(metal_tower_f16_encodes() - tw0 >= 1l, "the f16-sourced tower rode the twin route")
            }
            set_metal_tower_f16(true)
            set_metal_tensor_crowns("")
            metal_prefill_shutdown()
            set_metal_tower(false)
            let sc0 = metal_tower_stats()
            tt |> gate_encode(tw, st, "cb", 448, 448, "encode25.cb448.log")
            let sc1 = metal_tower_stats()
            tt |> equal(sc1.encodes, sc0.encodes, "knob-off leg never dispatched")
            tt |> equal(sc1.declines - sc0.declines, 1l, "knob-off leg recorded the knob decline")
            set_metal_tower(true)
            delete st
            delete tw
            var inscope stp <- stage_qwen25v_tower(mm)
            let base = stp.t.layer_w + 16l * stp.t.layer_w_stride
            for (i in range64(stp.t.layer_w_stride)) {
                stp.blob[base + i] = 0.0
            }
            for (i in range64(long_length(stp.wblob) > 0l ? stp.t.layer_w_stride : 0l)) {
                stp.wblob[base + i] = uint16(0)
            }
            var twp = Qwen25vTower()
            mint_qwen25v_tower(stp, twp)
            var stq = Qwen25vState()
            var o = OracleEncode()
            if (load_oracle_encode(path_join(path_join(models_dir(), "qwen3vl-vision-oracle"), "encode25.cb448.log"), o)) {
                let planes <- make_planes("cb", 448, 448)
                var out : array<float>
                with_job_que() {
                    setup_dasllama_jobque_()
                    qwen25v_encode(twp, stq, planes, 448l, 448l, out)
                }
                let ex = encode_excess(out, int(twp.proj_dim), o, TIER1_ABS, TIER1_REL_GPU)
                tt |> success(ex > 0.0, "zeroed-block poison EXCEEDS the GPU bar (excess {ex})")
                delete out
            }
            delete stq
            delete twp
            shallow_routing_cell(tt, mm, "window", 99l)   //! (0+1) % 99 != 0 -> the window arm
            shallow_routing_cell(tt, mm, "full", 1l)      //! (0+1) % 1 == 0 -> the slab arm
        } else {
            tt |> skip("no das_metal on this build - the GPU rung has no device to serve")
        }
    }
}

//! one layer of GPU noise measures ~2.6e-2 abs peak here (f16-operand GEMMs + the K=5120 merger
//! on |~70| values) and a routing defect mixes quadrants at >= 1.0, so the bar is 0.1 abs
def private shallow_routing_cell(t : T?; mm, name : string; wa : int64) {   // nolint:LINT012,LINT019 — the whole body is the das_metal static_if arm; the no-metal lane sees every argument unused
    static_if (typeinfo builtin_module_exists(das_metal)) {
        var inscope st <- stage_qwen25v_tower(mm)
        st.t.n_layer = 1l
        st.t.n_wa_pattern = wa
        var tw = Qwen25vTower()
        mint_qwen25v_tower(st, tw)
        let planes <- make_planes("quad", 448, 448)
        var sg = Qwen25vState()
        var og : array<float>
        set_metal_tower(true)
        let e0 = metal_tower_stats().encodes
        var ng = 0l
        with_job_que() {
            setup_dasllama_jobque_()
            ng = qwen25v_encode(tw, sg, planes, 448l, 448l, og)
        }
        t |> equal(metal_tower_stats().encodes - e0, 1l, "shallow {name}: GPU-served")
        var sc = Qwen25vState()
        var oc : array<float>
        set_metal_tower(false)
        var nc = 0l
        with_job_que() {
            setup_dasllama_jobque_()
            nc = qwen25v_encode(tw, sc, planes, 448l, 448l, oc)
        }
        set_metal_tower(true)
        t |> equal(int(ng), int(nc), "shallow {name}: token counts agree")
        var md = 0.0
        for (a, b in og, oc) {
            let av = abs(a - b)
            if (av > md) {
                md = av
            }
        }
        t |> success(md < 0.1, "shallow {name}: GPU == CPU on quad within 0.1 abs (measured {md})")
        delete og
        delete oc
        delete sg
        delete sc
        delete tw
    }
}
