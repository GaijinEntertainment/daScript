options gen2
options persistent_heap
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama_qwen3v
require dasllama/dasllama_math   // setup_dasllama_jobque_
require daslib/jobque_boost
require daslib/fio
require _model_tier   // models_dir() + model_available() gate
require ?das_metal dasllama/dasllama_metal_tower   // the CPU gates pin the tower knob off
require _vision_oracle   // the mtmd dump parser, fixture generators, per-token compare

// Tier-1 qwen3v tower parity vs llama-mtmd-debug -p encode (the Omni-30B SigLIP-geometry ViT).
// Reference arm: the f32-widened mmprojs, CPU, -fa off (qwen3vl-vision-oracle/mint.sh mints the
// Omni encode.* dumps, mint_4b.sh the 4B deepstack encode4b.* dumps).
// Fixtures: pre-resized [0,1] float canvases - no preprocessing, so a diff isolates the tower math.

let private TIER1_ABS = 2e-4
// x token rms. Poison-calibrated: 27 blocks whose GELU rides ggml's f16 LUT quantize the
// summation-order drift between das GEMMs and the reference's to a measured 0.6-2.7e-3 floor
// (the solid-color fixtures sit at low rms, where the floor shows; preproc, the pos-table
// antialias resize, and the rope tables are each independently bit-exact vs their references).
// A swapped rope-ladder poison lands at 0.71-0.85 maxdiff - 300x this bar.
let private TIER1_REL = 1e-2

// the deepstack leg runs FOUR GELU-LUT'd merger chains per wide row (main + three taps) where
// the Omni runs one, and the q-probes read individual slice values - the compounded
// summation-order floor measures 1.12e-2 on the low-rms gray fixture (a skipped-tap poison
// lands at 6.9-9.7 on the same probes, 600x this bar)
let private TIER1_REL_DS = 4e-2

// x token rms - the q8 serving lane vs the f32 oracle: measured 0.42 x rms worst (gray448) -
// the qwen ViT's mid-network outlier channels roughly double gemma3v's 0.22-0.25 q8 floor.
// The zero-layer poison leg proves the bar still discriminates; the caption is the product gate.
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
        static_if (typeinfo builtin_module_exists(das_metal)) {
            set_metal_tower(false)   // this gate owns the CPU chain - the dumps are CPU-minted
        }
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
        tt |> success(tw.blk_bf16 && tw.mm_bf16, "block and merger GEMMs bf16 as the file has them")
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
        delete st
        delete tw
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
        static_if (typeinfo builtin_module_exists(das_metal)) {
            set_metal_tower(false)   // this gate owns the CPU chain - the dumps are CPU-minted
        }
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
        static_if (typeinfo builtin_module_exists(das_metal)) {
            set_metal_tower(false)   // this gate owns the CPU chain - the dumps are CPU-minted
        }
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
        delete st
        delete tw
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
        static_if (typeinfo builtin_module_exists(das_metal)) {
            set_metal_tower(false)   // this gate owns the CPU chain - the dumps are CPU-minted
        }
        var st = Qwen3vState()
        tt |> success(tw.q8, "q8 lane loaded")
        tt |> gate_encode(tw, st, "cb", 448, 448, "encode4b.cb448.log", TIER1_REL_Q8_DS)
        tt |> gate_encode(tw, st, "cb", 96, 96, "encode4b.cb96.log", TIER1_REL_Q8_DS)
        delete st
        delete tw
    }
}

// the lane policy itself - model-free: default q8 (no accelerate tier in a test child), the
// pin flips it either way, reset returns to the policy
[test]
def test_qwen3v_lane_knob(t : T?) {
    t |> run("the q8 lane pin and policy default") @(tt : T?) {
        tt |> success(qwen3v_serves_q8(), "the CPU default serves q8")
        set_qwen3v_q8(false)
        tt |> success(!qwen3v_serves_q8(), "pinned exact")
        set_qwen3v_q8(true)
        tt |> success(qwen3v_serves_q8(), "pinned q8")
        reset_qwen3v_q8()
        tt |> success(qwen3v_serves_q8(), "reset returns to the policy default")
    }
}
