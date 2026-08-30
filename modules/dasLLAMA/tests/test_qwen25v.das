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
require dasllama/dasllama_common   // set_metal_tensor_crowns - the twin-W leg's crown pin
require _vision_oracle   // the mtmd dump parser, fixture generators, per-token compare
require math

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
        static_if (typeinfo builtin_module_exists(das_metal)) {
            set_metal_tower(false)   // this gate owns the CPU chain - the dumps are CPU-minted
        }
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
        static_if (typeinfo builtin_module_exists(das_metal)) {
            set_metal_tower(true)
        }
        delete st
        delete tw
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
def private shallow_routing_cell(t : T?; mm, name : string; wa : int64) {
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
