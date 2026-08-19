options gen2
options persistent_heap
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama_gemma4uv
require dasllama/dasllama_math   // setup_dasllama_jobque_
require daslib/jobque_boost
require daslib/fio
require _model_tier   // models_dir() + model_available() gate
require _vision_oracle   // the mtmd dump parser, fixture generators, per-token compare
require ?das_metal dasllama/dasllama_metal_tower   // the GPU tier-1 attempt; the CPU gate pins the knob off

// Tier-1 gemma4uv embedder parity vs llama-mtmd-debug -p encode (CPU, --no-mmproj-offload):
// pre-resized [0,1] float fixtures skip preprocessing entirely, so a diff here isolates the
// embedder math. Oracle per-token lines live beside the models in gemma4-vision-oracle/
// (mint.sh regenerates); both the mmproj and the dumps gate this test with explicit skips.

// the tier-1 gate: encode a fixture, compare per-token mean/v0..v3 against the oracle dump.
// %.6f print rounding is ±5e-7 and the measured accumulation-order noise is ~2e-5 (logged per
// fixture); 2e-4 leaves 10x headroom while staying far under any structural failure
let TIER1_TOL = 2e-4

def private gate_encode(t : T?; emb : Gemma4uvEmbedder; var st : Gemma4uvState;
                        kind : string; w, h : int; logname : string; gpu_ladder : bool = false) : bool {
    var o = OracleEncode()
    let logpath = path_join(path_join(models_dir(), "gemma4-vision-oracle"), logname)
    if (!load_oracle_encode(logpath, o)) {
        t |> skip("oracle dump {logname} not present/parseable - mint via gemma4-vision-oracle/mint.sh")
        return false
    }
    let planes <- make_planes(kind, w, h)
    var out : array<float>
    var npos = 0l
    with_job_que() {
        setup_dasllama_jobque_()
        npos = gemma4uv_encode(emb, st, planes, int64(w), int64(h), out)
    }
    t |> equal(int(npos), o.npos, "{kind} {w}x{h} token count")
    if (int(npos) != o.npos) {
        return false
    }
    let d = int(emb.proj_dim)
    if (gpu_ladder) {
        let maxdiff = compare_encode(t, "gemma4uv tier-1 GPU {kind} {w}x{h}", out, d, o, TIER1_TOL, 4e-3)
        let rung = maxdiff <= TIER1_TOL ? "held the strict CPU 2e-4" : "on the scale-relative rung"
        to_log(LOG_INFO, "gemma4uv tier-1 GPU {kind} {w}x{h}: {rung}\n")
        return true
    }
    compare_encode(t, "gemma4uv tier-1 {kind} {w}x{h}", out, d, o, TIER1_TOL, 0.0)
    return true
}

[test]
def test_gemma4uv_tier1(t : T?) {
    t |> run("tier-1 embedder parity vs the mtmd encode dumps") @(tt : T?) {
        let mm = path_join(models_dir(), "mmproj-gemma-4-12B-it-BF16.gguf")
        if (!model_available(tt, mm)) {
            return
        }
        var emb <- load_gemma4uv_embedder(mm)
        var st = Gemma4uvState()
        static_if (typeinfo builtin_module_exists(das_metal)) {
            set_metal_tower(false)   // this gate owns the CPU chain; the GPU rung is the next test
        }
        tt |> equal(int(emb.patch), 48, "folded superpatch")
        tt |> equal(int(emb.patch_dim), 6912, "im2col row")
        tt |> equal(int(emb.n_embd), 3840, "embedding width")
        tt |> equal(int(emb.proj_dim), 3840, "soft-token width")
        tt |> equal(int(emb.pos_entries), 1120, "pos-table rows per axis")
        tt |> gate_encode(emb, st, "gray", 480, 480, "encode.gray480.log")
        tt |> gate_encode(emb, st, "cb", 336, 336, "encode.cb336.log")
        tt |> gate_encode(emb, st, "red", 336, 336, "encode.red336.log")
        tt |> gate_encode(emb, st, "green", 336, 336, "encode.green336.log")
        tt |> gate_encode(emb, st, "blue", 336, 336, "encode.blue336.log")
        // the non-square orientation / transposed-grid gate: identical cb patches make the
        // per-token values a pure fingerprint of pos_x[col] + pos_y[row]
        tt |> gate_encode(emb, st, "cb", 672, 336, "encode.cb672x336.log")
        // a grid wider than the pos table panics by name instead of reading past the plane
        var tripped = false
        try {
            let w = (emb.pos_entries + 1l) * emb.patch
            var planes : array<float>
            planes |> reserve(w * emb.patch * 3l)
            planes |> resize(w * emb.patch * 3l)
            var wide : array<float>
            gemma4uv_encode(emb, st, planes, w, emb.patch, wide)
            delete wide
            delete planes
        } recover {
            tripped = true
        }
        tt |> success(tripped, "a canvas past pos_entries columns panics at the grid check")
        static_if (typeinfo builtin_module_exists(das_metal)) {
            set_metal_tower(true)
        }
        delete st
        delete emb
    }
}

// The GPU tier-1 gate: the same oracle dumps through the Metal tower.
// The f16-staged GEMMs cannot promise the CPU 2e-4, so the bar is the scale-relative rung
// (TIER1_TOL + 4e-3 * token rms) with the CPU constant untouched; the log states which rung
// each fixture actually held. Engage evidence is the encodes counter, per fixture.
[test]
def test_gemma4uv_tier1_gpu(t : T?) {
    t |> run("tier-1 GPU attempt vs the mtmd encode dumps (scale-relative rung)") @(tt : T?) {
        static_if (typeinfo builtin_module_exists(das_metal)) {
            let mm = path_join(models_dir(), "mmproj-gemma-4-12B-it-BF16.gguf")
            if (!model_available(tt, mm)) {
                return
            }
            var emb <- load_gemma4uv_embedder(mm)
            var st = Gemma4uvState()
            set_metal_tower(true)
            let fixtures = [(kind = "gray", w = 480, h = 480, log = "encode.gray480.log"),
                            (kind = "cb", w = 336, h = 336, log = "encode.cb336.log"),
                            (kind = "red", w = 336, h = 336, log = "encode.red336.log"),
                            (kind = "cb", w = 672, h = 336, log = "encode.cb672x336.log")]
            for (fx in fixtures) {
                let s0 = metal_tower_stats()
                let ran = gate_encode(tt, emb, st, fx.kind, fx.w, fx.h, fx.log, true)
                if (ran) {
                    tt |> equal(metal_tower_stats().encodes - s0.encodes, 1l,
                                "{fx.kind} {fx.w}x{fx.h}: GPU-served (encodes delta)")
                }
            }
            delete st
            delete emb
        }
    }
}
