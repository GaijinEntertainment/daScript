options gen2
options persistent_heap
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama_gemma3v
require dasllama/dasllama_vision_embedder   // vision_exec_fmt / vision_family through the carrier
require dasllama/dasllama_math   // setup_dasllama_jobque_
require daslib/jobque_boost
require daslib/fio
require _model_tier   // models_dir() + model_available() gate
require _vision_oracle   // the mtmd dump parser, fixture generators, per-token compare
require ?das_metal dasllama/dasllama_metal_tower   // the GPU tier-1 attempt; the CPU gates pin the knob off

// Tier-1 gemma3v (SigLIP) tower parity vs llama-mtmd-debug -p encode.
// Reference arm: the f32-widened gemma-3-4b mmproj, CPU, --no-mmproj-offload, -fa off.
// The canvas is FIXED 896² (learned position table), so the fixture axis is content, not
// geometry. Dumps: beside the models in gemma3-vision-oracle/ (mint_gemma3.sh regenerates).

let private TIER1_ABS = 2e-4
let private TIER1_REL = 4e-3   // × token rms - f32 planes vs the f32 oracle: summation order only
let private TIER1_REL_GPU = 4e-2   // the Metal leg: measured 6.9e-3..2.3e-2 x rms on these dumps - the f32 mulmm stages f16 tiles over 27 high-variance LN blocks
let private TIER1_REL_Q8 = 3.2e-1   // the q8 serving lane: measured 0.22-0.25 x rms on these dumps - 27 blocks of q8×q8 GEMMs incl. the padded down (deeper + higher-variance than gemma4v's 16); the caption floor is the product gate

def private gate_encode(t : T?; tw : Gemma3vTower; var st : Gemma3vState; kind : string; logname : string; rel : float = TIER1_REL) : bool {
    var o = OracleEncode()
    let logpath = path_join(path_join(models_dir(), "gemma3-vision-oracle"), logname)
    if (!load_oracle_encode(logpath, o)) {
        t |> skip("oracle dump {logname} not present/parseable - mint via gemma3-vision-oracle/mint_gemma3.sh")
        return false
    }
    let px = int(tw.image_px)
    let planes <- make_planes(kind, px, px)
    var out : array<float>
    var nout = 0l
    with_job_que() {
        setup_dasllama_jobque_()
        nout = gemma3v_encode(tw, st, planes, tw.image_px, tw.image_px, out)
    }
    t |> equal(int(nout), o.npos, "{kind} {px}x{px} token count")
    if (int(nout) != o.npos) {
        return false
    }
    compare_encode(t, "gemma3v tier-1{tw.q8 ? " q8" : ""} {kind}", out, int(tw.proj_dim), o, TIER1_ABS, rel)
    return true
}

[test]
def test_gemma3v_tier1(t : T?) {
    t |> run("tier-1 tower parity vs the mtmd encode dumps (gemma-3-4b)") @(tt : T?) {
        let mm = path_join(models_dir(), "mmproj-gemma-3-4b-it-F16.gguf")
        if (!model_available(tt, mm)) {
            return
        }
        set_gemma3v_q8(false)   // the exact-plane (f32-widened) lane; the q8 serving lane is its own test
        var tw <- load_gemma3v_tower(mm)
        reset_gemma3v_q8()
        static_if (typeinfo builtin_module_exists(das_metal)) {
            set_metal_tower(false)   // this gate owns the CPU chain; the GPU rung is its own test below
        }
        var st = Gemma3vState()
        tt |> success(!tw.q8, "exact-plane lane")
        tt |> equal(int(tw.patch), 14, "patch")
        tt |> equal(int(tw.merge), 4, "pool kernel")
        tt |> equal(int(tw.image_px), 896, "fixed canvas")
        tt |> equal(int(tw.n_embd), 1152, "tower width")
        tt |> equal(int(tw.n_layer), 27, "blocks")
        tt |> equal(int(tw.n_head), 16, "heads")
        tt |> equal(int(tw.n_ff), 4304, "ffn width")
        tt |> equal(int(tw.proj_dim), 2560, "soft-token width")
        tt |> gate_encode(tw, st, "cb", "g3.encode.cb896.log")
        tt |> gate_encode(tw, st, "gray", "g3.encode.gray896.log")
        tt |> gate_encode(tw, st, "red", "g3.encode.red896.log")
        tt |> gate_encode(tw, st, "green", "g3.encode.green896.log")
        tt |> gate_encode(tw, st, "blue", "g3.encode.blue896.log")
        // the fixed-canvas guard panics by name instead of misindexing the position table
        var tripped = false
        try {
            var planes : array<float>
            planes |> reserve(448l * 448l * 3l)
            planes |> resize(448l * 448l * 3l)
            var out : array<float>
            gemma3v_encode(tw, st, planes, 448l, 448l, out)
            delete out
            delete planes
        } recover {
            tripped = true
        }
        tt |> success(tripped, "a non-896 canvas panics at the fixed-canvas gate")
        // refusals by name: a wrong-family mmproj, and a source past the 1 GiB staged-mint line
        let wrong = path_join(models_dir(), "mmproj-gemma-4-E2B-it-bf16.gguf")
        if (stat(wrong).is_valid) {
            var wrong_tripped = false
            try {
                var bad <- load_gemma3v_tower(wrong)
                delete bad
            } recover {
                wrong_tripped = true
            }
            tt |> success(wrong_tripped, "a gemma4v mmproj is refused by projector name")
        }
        let f32twin = path_join(models_dir(), "mmproj-gemma-3-4b-it-F32.gguf")
        if (stat(f32twin).is_valid) {   // the oracle artifact doubles as the over-cap fixture
            var cap_tripped = false
            try {
                var bad2 <- load_gemma3v_tower(f32twin)
                delete bad2
            } recover {
                cap_tripped = true
            }
            tt |> success(cap_tripped, "the 1.6 GB f32 twin trips the staged-mint cap")
        }
        static_if (typeinfo builtin_module_exists(das_metal)) {
            set_metal_tower(true)
        }
        delete st
        delete tw
    }
}

// The GPU rung: the same dumps through the Metal tower's block loop (the f32 mulmm stages f16
// tiles, and the 72-wide heads restride to 128 around the attention trio).
[test]
def test_gemma3v_tier1_gpu(t : T?) {
    t |> run("tier-1 GPU attempt vs the mtmd encode dumps (scale-relative rung)") @(tt : T?) {
        static_if (typeinfo builtin_module_exists(das_metal)) {
            let mm = path_join(models_dir(), "mmproj-gemma-3-4b-it-F16.gguf")
            if (!model_available(tt, mm)) {
                return
            }
            var tw <- load_gemma3v_tower(mm)   // metal armed: the policy default is the exact lane
            var st = Gemma3vState()
            tt |> success(!tw.q8, "exact-plane lane (the driver declines q8)")
            let fixtures = [(kind = "cb", log = "g3.encode.cb896.log"),
                            (kind = "green", log = "g3.encode.green896.log")]
            for (fx in fixtures) {
                let s0 = metal_tower_stats()
                let ran = gate_encode(tt, tw, st, fx.kind, fx.log, TIER1_REL_GPU)
                if (ran) {
                    tt |> equal(metal_tower_stats().encodes - s0.encodes, 1l, "{fx.kind}: GPU-served (encodes delta)")
                    tt |> equal(metal_tower_stats().blocks - s0.blocks, tw.n_layer, "{fx.kind}: every block ran on the driver (blocks delta)")
                }
            }
            // the knob-off leg: the same tower, the CPU chain, no dispatch
            set_metal_tower(false)
            let sc0 = metal_tower_stats()
            tt |> gate_encode(tw, st, "cb", "g3.encode.cb896.log")
            let sc1 = metal_tower_stats()
            tt |> equal(sc1.encodes, sc0.encodes, "knob-off leg never dispatched")
            tt |> equal(sc1.declines - sc0.declines, 1l, "knob-off leg recorded the knob decline")
            set_metal_tower(true)
            delete st
            delete tw
            // the q8-decline leg: a q8 tower with the knob ARMED must never dispatch (its planes
            // are Q8_0 - a driver serve would read them as f32 garbage)
            set_gemma3v_q8(true)
            var twq <- load_gemma3v_tower(mm)
            reset_gemma3v_q8()
            var stq = Gemma3vState()
            let sq0 = metal_tower_stats()
            tt |> gate_encode(twq, stq, "cb", "g3.encode.cb896.log", TIER1_REL_Q8)
            let sq1 = metal_tower_stats()
            tt |> equal(sq1.encodes, sq0.encodes, "q8 tower never dispatched")
            tt |> success(sq1.declines - sq0.declines >= 1l, "q8 tower recorded the quant_mode decline")
            delete stq
            delete twq
        } else {
            feint("das_metal is not built on this platform - the GPU rung is Apple-only\n")
        }
    }
}

// The q8 serving lane (the CPU default): the same dumps on its own scale-relative bar.
[test]
def test_gemma3v_tier1_q8(t : T?) {
    t |> run("tier-1 on the q8 block-GEMM lane (the serving default)") @(tt : T?) {
        let mm = path_join(models_dir(), "mmproj-gemma-3-4b-it-F16.gguf")
        if (!model_available(tt, mm)) {
            return
        }
        set_gemma3v_q8(true)
        static_if (typeinfo builtin_module_exists(das_metal)) {
            set_metal_tower(false)   // a CPU-lane claim pins the covering knob, never relies on the runtime decline
        }
        var tw <- load_gemma3v_tower(mm)
        var inscope via_carrier <- load_vision_embedder(mm)
        tt |> equal(vision_family(via_carrier), "gemma3v", "the carrier sniffs the family")
        tt |> equal(vision_exec_fmt(via_carrier), "q8", "the carrier stamps the q8 lane")
        set_gemma3v_q8(false)
        var inscope via_carrier_exact <- load_vision_embedder(mm)
        tt |> equal(vision_exec_fmt(via_carrier_exact), "f32", "the carrier stamps the exact lane")
        reset_gemma3v_q8()
        var st = Gemma3vState()
        tt |> success(tw.q8, "q8 lane loaded")
        tt |> gate_encode(tw, st, "cb", "g3.encode.cb896.log", TIER1_REL_Q8)
        tt |> gate_encode(tw, st, "green", "g3.encode.green896.log", TIER1_REL_Q8)
        delete st
        delete tw
    }
}
