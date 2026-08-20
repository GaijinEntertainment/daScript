options gen2
options persistent_heap
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama_gemma4v
require dasllama/dasllama_vision_embedder   // vision_exec_fmt through the carrier
require dasllama/dasllama_math   // setup_dasllama_jobque_
require daslib/jobque_boost
require daslib/fio
require math
require _model_tier   // models_dir() + model_available() gate
require _vision_oracle   // the mtmd dump parser, fixture generators, per-token compare
require ?das_metal dasllama/dasllama_metal_tower   // the GPU tier-1 attempt; the CPU gates pin the knob off

// Tier-1 gemma4v tower parity vs llama-mtmd-debug -p encode.
// Reference arm: the f32-widened mmproj, CPU, --no-mmproj-offload, -fa off (the only true-f32 arm).
// Fixtures: pre-resized [0,1] float canvases - no preprocessing, so a diff isolates the tower math.
// Dumps: beside the models in gemma4-vision-oracle/ (mint_e2b.sh / mint_e4b.sh regenerate).
// Bar: scale-relative - 16 blocks of bf16-weight GEMMs under summation-order drift cannot promise
// gemma4uv's 2e-4; the logged maxdiff is the number.

let private TIER1_ABS = 2e-4
let private TIER1_REL = 4e-3   // × token rms (≈0.75 on E2B) - the rung gemma4uv's GPU leg holds
let private TIER1_REL_GPU = 8e-3   // the Metal leg: 112 f16-staged GEMM tiles over 16 blocks - measured 8.9e-4..3.7e-3 on these dumps
let private TIER1_REL_Q8 = 1.2e-1   // the q8 serving lane: measured 3.1e-2..6.8e-2 (≈9% of rms) on these dumps - 16 blocks of q8×q8 GEMMs; the caption floor is the product gate
let private KNOCKOUT_FLOOR = 1e-3   // the clamp knockout must miss the oracle by at least this (the armed tower holds 1e-3)

def private gate_encode(t : T?; tw : Gemma4vTower; var st : Gemma4vState; kind : string; w, h : int; logname : string; rel : float = TIER1_REL) : bool {
    var o = OracleEncode()
    let logpath = path_join(path_join(models_dir(), "gemma4-vision-oracle"), logname)
    if (!load_oracle_encode(logpath, o)) {
        t |> skip("oracle dump {logname} not present/parseable - mint via its gemma4-vision-oracle mint script")
        return false
    }
    let planes <- make_planes(kind, w, h)
    var out : array<float>
    var nout = 0l
    with_job_que() {
        setup_dasllama_jobque_()
        nout = gemma4v_encode(tw, st, planes, int64(w), int64(h), out)
    }
    t |> equal(int(nout), o.npos, "{kind} {w}x{h} token count")
    if (int(nout) != o.npos) {
        return false
    }
    compare_encode(t, "gemma4v tier-1{tw.q8 ? " q8" : ""} {kind} {w}x{h}", out, int(tw.proj_dim), o, TIER1_ABS, rel)
    return true
}

[test]
def test_gemma4v_tier1(t : T?) {
    t |> run("tier-1 tower parity vs the mtmd encode dumps (E2B)") @(tt : T?) {
        let mm = path_join(models_dir(), "mmproj-gemma-4-E2B-it-bf16.gguf")
        if (!model_available(tt, mm)) {
            return
        }
        set_gemma4v_q8(false)   // the CPU gate owns the exact-plane lane; the q8 serving lane is the next test
        var tw <- load_gemma4v_tower(mm)
        reset_gemma4v_q8()
        static_if (typeinfo builtin_module_exists(das_metal)) {
            set_metal_tower(false)   // this gate owns the CPU chain; the GPU rung is its own test below
        }
        var st = Gemma4vState()
        tt |> success(!tw.q8, "exact-plane lane")
        tt |> equal(int(tw.patch), 16, "patch")
        tt |> equal(int(tw.merge), 3, "pool kernel")
        tt |> equal(int(tw.n_embd), 768, "tower width")
        tt |> equal(int(tw.n_layer), 16, "blocks")
        tt |> equal(int(tw.n_head), 12, "heads")
        tt |> equal(int(tw.n_ff), 3072, "ffn width")
        tt |> equal(int(tw.proj_dim), 1536, "soft-token width")
        tt |> equal(int(tw.pos_entries), 10240, "pos-table rows per axis")
        tt |> success(tw.blk_bf16 && tw.proj_bf16 && !tw.patch_bf16, "plane split as the file has it: bf16 GEMMs, f32 patch conv")
        tt |> success(!tw.has_std, "E2B carries no std_bias/std_scale")
        tt |> gate_encode(tw, st, "cb", 96, 96, "e2b.encode.cb96.log")   // 4 tokens, the smallest grid
        tt |> gate_encode(tw, st, "gray", 336, 336, "e2b.encode.gray336.log")
        tt |> gate_encode(tw, st, "cb", 336, 336, "e2b.encode.cb336.log")
        tt |> gate_encode(tw, st, "red", 336, 336, "e2b.encode.red336.log")
        tt |> gate_encode(tw, st, "green", 336, 336, "e2b.encode.green336.log")
        tt |> gate_encode(tw, st, "blue", 336, 336, "e2b.encode.blue336.log")
        tt |> gate_encode(tw, st, "cb", 480, 480, "e2b.encode.cb480.log")
        tt |> gate_encode(tw, st, "cb", 672, 336, "e2b.encode.cb672x336.log")   // non-square: the transposed-grid gate
        // the two canvas guards panic by name instead of reading past a plane
        var wide_tripped = false
        try {
            let w = (tw.pos_entries + 1l) * tw.patch
            var planes : array<float>
            planes |> reserve(w * tw.patch * 3l)
            planes |> resize(w * tw.patch * 3l)
            var wide : array<float>
            gemma4v_encode(tw, st, planes, w, tw.patch, wide)
            delete wide
            delete planes
        } recover {
            wide_tripped = true
        }
        tt |> success(wide_tripped, "a canvas past pos_entries columns panics at the grid check")
        var ragged_tripped = false
        try {
            var planes : array<float>
            planes |> reserve(64l * 48l * 3l)
            planes |> resize(64l * 48l * 3l)
            var out : array<float>
            gemma4v_encode(tw, st, planes, 64l, 48l, out)   // 64 is not a multiple of the 48 px pooled patch
            delete out
            delete planes
        } recover {
            ragged_tripped = true
        }
        tt |> success(ragged_tripped, "a canvas that is not a multiple of the pooled patch panics at the span check")
        static_if (typeinfo builtin_module_exists(das_metal)) {
            set_metal_tower(true)
        }
        delete st
        delete tw
    }
}

// The GPU tier-1 gate: the same dumps through the Metal tower's block loop (bf16 mul_mm stages
// f16 tiles, so the bar is scale-relative and wider than the CPU gate's).
[test]
def test_gemma4v_tier1_gpu(t : T?) {
    t |> run("tier-1 GPU attempt vs the mtmd encode dumps (scale-relative rung)") @(tt : T?) {
        static_if (typeinfo builtin_module_exists(das_metal)) {
            let mm = path_join(models_dir(), "mmproj-gemma-4-E2B-it-bf16.gguf")
            if (!model_available(tt, mm)) {
                return
            }
            set_metal_tower(false)
            tt |> success(gemma4v_serves_q8(), "with the Metal tower off (and no accelerate tier) the default lane is q8")
            set_metal_tower(true)
            tt |> success(!gemma4v_serves_q8(), "with the Metal tower armed the default lane is the file's planes")
            var tw <- load_gemma4v_tower(mm)
            var st = Gemma4vState()
            tt |> success(!tw.q8, "exact-plane lane (the driver declines q8)")
            let fixtures = [(kind = "cb", w = 96, h = 96, log = "e2b.encode.cb96.log"),
                            (kind = "cb", w = 336, h = 336, log = "e2b.encode.cb336.log"),
                            (kind = "green", w = 336, h = 336, log = "e2b.encode.green336.log"),
                            (kind = "cb", w = 480, h = 480, log = "e2b.encode.cb480.log"),
                            (kind = "cb", w = 672, h = 336, log = "e2b.encode.cb672x336.log")]
            for (fx in fixtures) {
                let s0 = metal_tower_stats()
                let ran = gate_encode(tt, tw, st, fx.kind, fx.w, fx.h, fx.log, TIER1_REL_GPU)
                if (ran) {
                    tt |> equal(metal_tower_stats().encodes - s0.encodes, 1l, "{fx.kind} {fx.w}x{fx.h}: GPU-served (encodes delta)")
                    tt |> equal(metal_tower_stats().blocks - s0.blocks, tw.n_layer, "{fx.kind} {fx.w}x{fx.h}: every block ran on the driver (blocks delta)")
                }
            }
            // the knob-off leg: the same tower, the CPU chain, no dispatch
            set_metal_tower(false)
            let sc0 = metal_tower_stats()
            tt |> gate_encode(tw, st, "cb", 336, 336, "e2b.encode.cb336.log")
            let sc1 = metal_tower_stats()
            tt |> equal(sc1.encodes, sc0.encodes, "knob-off leg never dispatched")
            tt |> equal(sc1.declines - sc0.declines, 1l, "knob-off leg recorded the knob decline")
            set_metal_tower(true)
            delete st
            delete tw
        } else {
            feint("das_metal is not built on this platform - the GPU rung is Apple-only\n")
        }
    }
}

// The clamp knockout: the same tower with every block clamp disarmed must MISS the oracle - the
// sidecar scalars are load-bearing, not decoration (the E2B file's bounds sit where activations
// live).
[test]
def test_gemma4v_clamps_bind(t : T?) {
    t |> run("disarming the block clamps moves tier-1 past the bar") @(tt : T?) {
        let mm = path_join(models_dir(), "mmproj-gemma-4-E2B-it-bf16.gguf")
        if (!model_available(tt, mm)) {
            return
        }
        var o = OracleEncode()
        let logpath = path_join(path_join(models_dir(), "gemma4-vision-oracle"), "e2b.encode.cb336.log")
        if (!load_oracle_encode(logpath, o)) {
            tt |> skip("oracle dump e2b.encode.cb336.log not present/parseable - mint via gemma4-vision-oracle/mint_e2b.sh")
            return
        }
        set_gemma4v_q8(false)   // the exact-plane lane - the knockout reads against the strict bar's context
        var inscope stg <- stage_gemma4v_tower(mm)
        reset_gemma4v_q8()
        let d = stg.t.n_embd
        let hd = d / stg.t.n_head
        var armed = 0
        for (l in range64(stg.t.n_layer)) {
            let clamps = stg.t.layer_f + l * stg.t.layer_f_stride + 4l * d + 2l * hd
            for (g in range64(GEMMA4V_GEMMS)) {
                let flag = clamps + g * GEMMA4V_CLAMP_F + 4l
                if (stg.blob[flag] != 0.0) {
                    armed++
                }
                stg.blob[flag] = 0.0
            }
        }
        tt |> equal(armed, int(stg.t.n_layer * GEMMA4V_GEMMS), "every block GEMM carries an active clamp on E2B")
        var tw = Gemma4vTower()
        mint_gemma4v_tower(stg, tw)
        static_if (typeinfo builtin_module_exists(das_metal)) {
            set_metal_tower(false)   // the knockout reads the CPU chain
        }
        var st = Gemma4vState()
        let planes <- make_planes("cb", 336, 336)
        var out : array<float>
        with_job_que() {
            setup_dasllama_jobque_()
            gemma4v_encode(tw, st, planes, 336l, 336l, out)
        }
        var maxdiff = 0.0
        for (p in range(o.npos)) {
            maxdiff = max(maxdiff, abs(out[p * int(tw.proj_dim)] - o.v0[p]))
        }
        to_log(LOG_INFO, "gemma4v clamp knockout cb 336x336: maxdiff {maxdiff} (floor {KNOCKOUT_FLOOR})\n")
        tt |> success(maxdiff > KNOCKOUT_FLOOR, "disarmed clamps miss the oracle by {maxdiff}")
        delete st
        delete tw
        // the same knockout on the q8 lane: the q8 GEMM route's inactive-clamp arm runs and misses too
        set_gemma4v_q8(true)
        var inscope stgq <- stage_gemma4v_tower(mm)
        reset_gemma4v_q8()
        for (l in range64(stgq.t.n_layer)) {
            let clamps = stgq.t.layer_f + l * stgq.t.layer_f_stride + 4l * d + 2l * hd
            for (g in range64(GEMMA4V_GEMMS)) {
                stgq.blob[clamps + g * GEMMA4V_CLAMP_F + 4l] = 0.0
            }
        }
        var twq = Gemma4vTower()
        mint_gemma4v_tower(stgq, twq)
        var stq = Gemma4vState()
        var outq : array<float>
        with_job_que() {
            setup_dasllama_jobque_()
            gemma4v_encode(twq, stq, planes, 336l, 336l, outq)
        }
        var maxdiffq = 0.0
        for (p in range(o.npos)) {
            maxdiffq = max(maxdiffq, abs(outq[p * int(twq.proj_dim)] - o.v0[p]))
        }
        tt |> success(twq.q8 && maxdiffq > KNOCKOUT_FLOOR, "q8 lane: disarmed clamps miss the oracle by {maxdiffq}")
        static_if (typeinfo builtin_module_exists(das_metal)) {
            set_metal_tower(true)
        }
        delete stq
        delete twq
    }
}

// The standardize guard (std_bias / std_scale): absent on E2B, so the arm is reached through
// staging - a neutral pair (bias 0, scale 1) must leave the encode bit-identical, a biased pair
// must move it. The sign convention (x - bias) * scale is what the second half pins.
[test]
def test_gemma4v_std_guard(t : T?) {
    t |> run("synthesized std_bias/std_scale: neutral is identity, a bias moves the tokens") @(tt : T?) {
        let mm = path_join(models_dir(), "mmproj-gemma-4-E2B-it-bf16.gguf")
        if (!model_available(tt, mm)) {
            return
        }
        static_if (typeinfo builtin_module_exists(das_metal)) {
            set_metal_tower(false)   // the CPU chain carries the standardize step (the driver serves blocks only)
        }
        set_gemma4v_q8(false)
        let planes <- make_planes("cb", 96, 96)
        var plain : array<float>
        var neutral : array<float>
        var biased : array<float>
        with_job_que() {
            setup_dasllama_jobque_()
            var inscope stg0 <- stage_gemma4v_tower(mm)
            tt |> success(!stg0.t.has_std, "E2B carries no std pair - the arm is synthesized below")
            var tw0 = Gemma4vTower()
            mint_gemma4v_tower(stg0, tw0)
            var s0 = Gemma4vState()
            gemma4v_encode(tw0, s0, planes, 96l, 96l, plain)
            delete s0
            delete tw0
            for (arm in range(2)) {
                var inscope stg <- stage_gemma4v_tower(mm)
                let d = stg.t.n_embd
                stg.t.has_std = true
                stg.t.std_bias = long_length(stg.blob)
                stg.t.std_scale = stg.t.std_bias + d
                stg.blob |> reserve(stg.t.std_scale + d)
                stg.blob |> resize(stg.t.std_scale + d)
                for (i in range64(d)) {
                    stg.blob[stg.t.std_bias + i] = arm == 0 ? 0.0 : 0.25
                    stg.blob[stg.t.std_scale + i] = arm == 0 ? 1.0 : -1.0
                }
                var tw = Gemma4vTower()
                mint_gemma4v_tower(stg, tw)
                tt |> success(tw.has_std, "arm {arm}: the served tower carries the std pair")
                var s = Gemma4vState()
                if (arm == 0) {
                    gemma4v_encode(tw, s, planes, 96l, 96l, neutral)
                } else {
                    gemma4v_encode(tw, s, planes, 96l, 96l, biased)
                }
                delete s
                delete tw
            }
        }
        reset_gemma4v_q8()
        var neutral_diff = 0.0
        var biased_diff = 0.0
        for (p0, pn, pb in plain, neutral, biased) {
            neutral_diff = max(neutral_diff, abs(pn - p0))
            biased_diff = max(biased_diff, abs(pb - p0))
        }
        tt |> success(neutral_diff == 0.0, "bias 0 / scale 1 leaves the encode bit-identical (diff {neutral_diff})")
        tt |> success(biased_diff > 1e-2, "bias 0.25 / scale -1 moves the tokens (diff {biased_diff})")
        static_if (typeinfo builtin_module_exists(das_metal)) {
            set_metal_tower(true)
        }
    }
}

// The q8 serving lane (the CPU default): the same dumps on its own scale-relative bar. The
// exact-plane gate above is the parity rail; this one says what serving costs in fidelity.
[test]
def test_gemma4v_tier1_q8(t : T?) {
    t |> run("tier-1 on the q8 block-GEMM lane (the serving default)") @(tt : T?) {
        let mm = path_join(models_dir(), "mmproj-gemma-4-E2B-it-bf16.gguf")
        if (!model_available(tt, mm)) {
            return
        }
        set_gemma4v_q8(true)   // pinned: on Apple builds the Metal tower's presence flips the default to the exact lane
        var tw <- load_gemma4v_tower(mm)
        var inscope via_carrier <- load_vision_embedder(mm)
        tt |> equal(vision_exec_fmt(via_carrier), "q8", "the carrier stamps the q8 lane")
        set_gemma4v_q8(false)
        var inscope via_carrier_exact <- load_vision_embedder(mm)
        tt |> equal(vision_exec_fmt(via_carrier_exact), "f32", "the carrier stamps the exact lane")
        reset_gemma4v_q8()
        var st = Gemma4vState()
        tt |> success(tw.q8, "q8 lane loaded")
        tt |> success(length(tw.qblob) == tw.n_layer * tw.layer_w_stride, "qblob holds exactly the block GEMM set")
        tt |> gate_encode(tw, st, "cb", 96, 96, "e2b.encode.cb96.log", TIER1_REL_Q8)
        tt |> gate_encode(tw, st, "cb", 336, 336, "e2b.encode.cb336.log", TIER1_REL_Q8)
        tt |> gate_encode(tw, st, "green", 336, 336, "e2b.encode.green336.log", TIER1_REL_Q8)
        tt |> gate_encode(tw, st, "cb", 672, 336, "e2b.encode.cb672x336.log", TIER1_REL_Q8)
        delete st
        delete tw
    }
}

// The E4B rung: the larger E-series vehicle shares the tower geometry block for block - only the
// soft-token width differs (2560 vs 1536) - so the dump set is the seam subset (smallest grid,
// one product size, the orientation gate; mint_e4b.sh), with one GPU and one q8-lane fixture.
[test]
def test_gemma4v_tier1_e4b(t : T?) {
    t |> run("tier-1 tower parity vs the mtmd encode dumps (E4B)") @(tt : T?) {
        let mm = path_join(models_dir(), "mmproj-gemma-4-E4B-it-BF16.gguf")
        if (!model_available(tt, mm)) {
            return
        }
        set_gemma4v_q8(false)
        var tw <- load_gemma4v_tower(mm)
        reset_gemma4v_q8()
        static_if (typeinfo builtin_module_exists(das_metal)) {
            set_metal_tower(false)   // the CPU gate first; the GPU rung re-arms below
        }
        var st = Gemma4vState()
        tt |> success(!tw.q8, "exact-plane lane")
        tt |> equal(int(tw.proj_dim), 2560, "soft-token width - the one dimension E4B changes")
        tt |> equal(int(tw.n_embd), 768, "tower width matches E2B's")
        tt |> equal(int(tw.n_layer), 16, "block count matches E2B's")
        tt |> success(tw.blk_bf16 && tw.proj_bf16 && !tw.patch_bf16, "plane split as the file has it: bf16 GEMMs, f32 patch conv")
        tt |> success(!tw.has_std, "E4B carries no std_bias/std_scale")
        tt |> gate_encode(tw, st, "cb", 96, 96, "e4b.encode.cb96.log")
        tt |> gate_encode(tw, st, "cb", 336, 336, "e4b.encode.cb336.log")
        tt |> gate_encode(tw, st, "green", 336, 336, "e4b.encode.green336.log")
        tt |> gate_encode(tw, st, "cb", 672, 336, "e4b.encode.cb672x336.log")
        static_if (typeinfo builtin_module_exists(das_metal)) {
            set_metal_tower(true)
            let s0 = metal_tower_stats()
            let ran = gate_encode(tt, tw, st, "cb", 336, 336, "e4b.encode.cb336.log", TIER1_REL_GPU)
            if (ran) {
                tt |> equal(metal_tower_stats().encodes - s0.encodes, 1l, "GPU-served (encodes delta)")
                tt |> equal(metal_tower_stats().blocks - s0.blocks, tw.n_layer, "every block ran on the driver (blocks delta)")
            }
        }
        delete st
        delete tw
        set_gemma4v_q8(true)   // the serving lane on its own bar (the Metal driver declines q8)
        var twq <- load_gemma4v_tower(mm)
        reset_gemma4v_q8()
        var stq = Gemma4vState()
        tt |> success(twq.q8, "q8 lane loaded")
        tt |> gate_encode(twq, stq, "cb", 336, 336, "e4b.encode.cb336.log", TIER1_REL_Q8)
        delete stq
        delete twq
    }
}
