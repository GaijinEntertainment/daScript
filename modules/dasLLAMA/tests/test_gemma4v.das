options gen2
options persistent_heap
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama_gemma4v
require dasllama/dasllama_math   // setup_dasllama_jobque_
require daslib/jobque_boost
require daslib/fio
require math
require _model_tier   // models_dir() + model_available() gate
require _vision_oracle   // the mtmd dump parser, fixture generators, per-token compare

// Tier-1 gemma4v tower parity vs llama-mtmd-debug -p encode on the f32-widened E2B mmproj (CPU,
// --no-mmproj-offload, -fa off): pre-resized [0,1] float fixtures skip preprocessing entirely, so a
// diff here isolates the tower math. The dumps live beside the models in gemma4-vision-oracle/
// (mint_e2b.sh regenerates). The bar is scale-relative: 16 blocks of bf16-weight GEMMs under
// summation-order drift cannot promise gemma4uv's 2e-4; the logged maxdiff is the number.

let private TIER1_ABS = 2e-4
let private TIER1_REL = 4e-3   // × token rms (≈0.75 on E2B) - the rung gemma4uv's GPU leg holds

def private gate_encode(t : T?; tw : Gemma4vTower; var st : Gemma4vState; kind : string; w, h : int; logname : string) : bool {
    var o = OracleEncode()
    let logpath = path_join(path_join(models_dir(), "gemma4-vision-oracle"), logname)
    if (!load_oracle_encode(logpath, o)) {
        t |> skip("oracle dump {logname} not present/parseable - mint via gemma4-vision-oracle/mint_e2b.sh")
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
    compare_encode(t, "gemma4v tier-1 {kind} {w}x{h}", out, int(tw.proj_dim), o, TIER1_ABS, TIER1_REL)
    return true
}

[test]
def test_gemma4v_tier1(t : T?) {
    t |> run("tier-1 tower parity vs the mtmd encode dumps (E2B)") @(tt : T?) {
        let mm = path_join(models_dir(), "mmproj-gemma-4-E2B-it-bf16.gguf")
        if (!model_available(tt, mm)) {
            return
        }
        var tw <- load_gemma4v_tower(mm)
        var st = Gemma4vState()
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
        // the 96x96 checkerboard first: 4 tokens, the node ladder's fixture
        tt |> gate_encode(tw, st, "cb", 96, 96, "e2b.encode.cb96.log")
        tt |> gate_encode(tw, st, "gray", 336, 336, "e2b.encode.gray336.log")
        tt |> gate_encode(tw, st, "cb", 336, 336, "e2b.encode.cb336.log")
        tt |> gate_encode(tw, st, "red", 336, 336, "e2b.encode.red336.log")
        tt |> gate_encode(tw, st, "green", 336, 336, "e2b.encode.green336.log")
        tt |> gate_encode(tw, st, "blue", 336, 336, "e2b.encode.blue336.log")
        tt |> gate_encode(tw, st, "cb", 480, 480, "e2b.encode.cb480.log")
        // the non-square orientation / transposed-grid gate
        tt |> gate_encode(tw, st, "cb", 672, 336, "e2b.encode.cb672x336.log")
        delete st
        delete tw
    }
}

// The clamp knockout: the same tower with every block clamp disarmed must MISS the oracle - the
// sidecar scalars are load-bearing, not decoration (the E2B file's bounds sit where activations
// live). Stage -> zero the active flags -> mint in memory -> encode the cb 336 fixture.
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
        var inscope st <- stage_gemma4v_tower(mm)
        let d = st.t.n_embd
        let hd = d / st.t.n_head
        var armed = 0
        for (l in range64(st.t.n_layer)) {
            let clamps = st.t.layer_f + l * st.t.layer_f_stride + 4l * d + 2l * hd
            for (g in range64(GEMMA4V_GEMMS)) {
                let flag = clamps + g * GEMMA4V_CLAMP_F + 4l
                if (st.blob[flag] != 0.0) {
                    armed++
                }
                st.blob[flag] = 0.0
            }
        }
        tt |> equal(armed, int(st.t.n_layer * GEMMA4V_GEMMS), "every block GEMM carries an active clamp on E2B")
        var tw = Gemma4vTower()
        mint_gemma4v_tower(st, tw)
        var s = Gemma4vState()
        let planes <- make_planes("cb", 336, 336)
        var out : array<float>
        with_job_que() {
            setup_dasllama_jobque_()
            gemma4v_encode(tw, s, planes, 336l, 336l, out)
        }
        var maxdiff = 0.0
        for (p in range(o.npos)) {
            maxdiff = max(maxdiff, abs(out[p * int(tw.proj_dim)] - o.v0[p]))
        }
        to_log(LOG_INFO, "gemma4v clamp knockout cb 336x336: maxdiff {maxdiff} (armed tower holds < 1e-3)\n")
        tt |> success(maxdiff > 1e-3, "disarmed clamps miss the oracle by {maxdiff}")
        delete s
        delete tw
    }
}
