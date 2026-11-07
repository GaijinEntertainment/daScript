options gen2
options persistent_heap
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama_gemma4uv
require dasllama/dasllama_common   // the tensor-crown pin (the rung is simdgroup-calibrated)
require dasllama/dasllama_math   // setup_dasllama_jobque_
require daslib/jobque_boost
require daslib/fio
require _model_tier   // models_dir() + model_available() gate
require _vision_oracle   // the mtmd dump parser, fixture generators, per-token compare
require ?das_metal dasllama/dasllama_metal_tower   // the GPU tier-1 attempt; the CPU gate pins the knob off
require ?vulkan dasllama/dasllama_vulkan_tower
require ?vulkan dasllama/dasllama_vulkan_common   // vk_drop_model_state: the drop-and-encode leg
require _tower_twin
require ?das_metal dasllama/dasllama_metal_prefill   // metal_prefill_shutdown — the crown-pin cell drops the trio's cached flags

// Tier-1 gemma4uv embedder parity vs llama-mtmd-debug -p encode (CPU, --no-mmproj-offload, dumps
// minted from the f32-widened 12B mmproj twin; the embedder has no attention, so the flash-attn
// setting does not enter): pre-resized [0,1] float fixtures skip preprocessing entirely, so a diff
// here isolates the embedder math. Oracle per-token lines live beside the models in gemma4-vision-oracle/
// (mint.sh regenerates); both the mmproj and the dumps gate this test with explicit skips. On an Apple build the
// GPU cell runs the same dumps through the Metal embedder, on a Vulkan build through the Vulkan one, each off the
// planes with the stem on the device and engage proven per fixture by the encodes counter. The discrete-home cells (the
// device twin, the tier-1 rung on the serving discrete home) seat on the serving discrete home: under the CUDA ask
// (DASLLAMA_CUDA=1 or set_gpu_home("cuda")) on the CUDA tower, skipping without a CUDA device there (`cuda_twin_seat`);
// else on the Vulkan tower, skipping on a das_metal build (its tower hooks own the seats), without a Vulkan device under
// DASLLAMA_GPU=1 and without the tower's tile family (`vulkan_twin_seat`). The twin's control zeroes the patch weights
// and must miss the bar.

// the tier-1 gate: encode a fixture, compare per-token mean/v0..v3 against the oracle dump.
// %.6f print rounding is ±5e-7 and the measured accumulation-order noise is ~2e-5 (logged per
// fixture); 2e-4 leaves 10x headroom while staying far under any structural failure
let TIER1_TOL = 2e-4

[cold_path]
def private gate_encode(t : T?; emb : Gemma4uvEmbedder; var st : Gemma4uvState;
                        kind : string; w, h : int; logname : string; gpu_ladder : bool = false) : bool {
    return oracle_gate(t, path_join(models_dir(), "gemma4-vision-oracle"), logname, "gemma4-vision-oracle/mint.sh", kind, w, h,
            "gemma4uv tier-1{gpu_ladder ? " GPU" : ""} {kind} {w}x{h}", int(emb.proj_dim), TIER1_TOL, gpu_ladder ? 4e-3 : 0.0) $(planes : array<float>; var out : array<float>) : int64 {
        return gemma4uv_encode(emb, st, planes, int64(w), int64(h), out)
    }
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
            // the scale-relative rung is calibrated on the SIMDGROUP kernel set - pin it
            // (a bare setter would not survive the load, which re-applies the box
            // profile's crowns): the envelope-verified mulmm_bf16 tensor crown rounds
            // differently and sits a hair past the rung on some canvases; the crowned
            // lane's quality is the board pregate's claim, not this cell's
            let prev_crowns = get_metal_tensor_crowns()
            pin_metal_tensor_crowns("")
            metal_tower_shutdown()
            metal_prefill_shutdown()   // the crown flags the shared GEMM/trio builders cache
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
            unpin_metal_tensor_crowns()
            set_metal_tensor_crowns(prev_crowns)
            metal_tower_shutdown()
            metal_prefill_shutdown()
            delete st
            delete emb
        }
    }
}

//! the embedder with the patch weights zeroed in every plane the chain reads them from: the device twin's input poison
def private mint_patch_zeroed(mm : string) : Gemma4uvEmbedder {
    var inscope stgp <- stage_gemma4uv_embedder(mm)
    let n = stgp.e.n_embd * stgp.e.patch_dim
    for (i in range64(long_length(stgp.blob) >= stgp.e.patch_w + n ? n : 0l)) {
        stgp.blob[stgp.e.patch_w + i] = 0.0
    }
    for (i in range64(long_length(stgp.wblob) >= stgp.e.patch_w + n ? n : 0l)) {
        stgp.wblob[stgp.e.patch_w + i] = uint16(0)
    }
    var embp = Gemma4uvEmbedder()
    mint_gemma4uv_embedder(stgp, embp)
    return <- embp
}

[test, arch(at = "../ARCHITECTURE_GPU_TOWER_VULKAN_VISION.md#vk-vision-chains")]
def test_gemma4uv_device_twin(t : T?) {
    t |> run("the device embedder vs the CPU chain, dump-free, and the patch-zeroed control") @(tt : T?) {
        static_if (typeinfo builtin_module_exists(vulkan) || typeinfo builtin_module_exists(cuda)) {
            let mm = path_join(models_dir(), "mmproj-gemma-4-12B-it-BF16.gguf")
            if (!model_available(tt, mm) || !discrete_twin_seat(tt)) {
                return
            }
            var inscope stg <- stage_gemma4uv_embedder(mm)
            var emb = Gemma4uvEmbedder()
            mint_gemma4uv_embedder(stg, emb)
            var st = Gemma4uvState()
            var inscope embp <- mint_patch_zeroed(mm)
            var stp = Gemma4uvState()
            let home = serving_tower_dev(TowerFamily.vision)
            for (fx in [(w = 336, h = 336), (w = 672, h = 336)]) {
                let tag = "gemma4uv {home} cb {fx.w}x{fx.h}"
                var inscope planes <- make_planes("cb", fx.w, fx.h)
                var cpu, dev, poisoned : array<float>
                var n_cpu = 0l
                var n_dev = 0l
                let pre = gpu_tower_counters(TowerFamily.vision)
                var s0 = pre
                var s1 = pre
                var s2 = pre
                with_gpu_knobs() {
                    set_serving_gpu_tower(false, TowerFamily.vision)
                    with_dasllama_jobque_() {
                        n_cpu = gemma4uv_encode(emb, st, planes, int64(fx.w), int64(fx.h), cpu)
                    }
                    s0 = gpu_tower_counters(TowerFamily.vision)
                    set_serving_gpu_tower(true, TowerFamily.vision)
                    with_dasllama_jobque_() {
                        n_dev = gemma4uv_encode(emb, st, planes, int64(fx.w), int64(fx.h), dev)
                    }
                    s1 = gpu_tower_counters(TowerFamily.vision)
                    with_dasllama_jobque_() {
                        gemma4uv_encode(embp, stp, planes, int64(fx.w), int64(fx.h), poisoned)
                    }
                    s2 = gpu_tower_counters(TowerFamily.vision)
                }
                tt |> equal(s0.encodes - pre.encodes, 0l, "{tag}: the knob-off leg never dispatched (encodes delta)")
                tt |> equal(s0.convs - pre.convs, 0l, "{tag}: the knob-off leg never ran the stem on the device (convs delta)")
                tt |> equal(n_cpu, n_dev, "{tag}: token count on both chains")
                tt |> equal(s1.encodes - s0.encodes, 1l, "{tag}: device-served (encodes delta)")
                tt |> equal(s1.convs - s0.convs, 1l, "{tag}: the stem ran on the device (convs delta)")
                tt |> equal(s2.encodes - s1.encodes, 1l, "{tag} patch-zeroed: device-served (encodes delta)")
                tower_twin_held(tt, tag, TowerFamily.vision, cpu, dev, TWIN_EXACT_REL_L2_VK)
                tower_twin_poisoned(tt, "{tag} patch-zeroed", TowerFamily.vision, cpu, poisoned, TWIN_EXACT_REL_L2_VK)
                delete cpu
                delete dev
                delete poisoned
            }
            gpu_tower_shutdown(TowerFamily.vision)
            delete stp
            delete st
            delete emb
        } else {
            tt |> skip("no discrete GPU home (dasVulkan or dasCUDA) in this build")
        }
    }
}

//! the embedder whole on the serving device off the planes holds the CPU bar on the mtmd dumps; engage evidence: the encodes counter, per fixture
[test]
def test_gemma4uv_tier1_vulkan(t : T?) {
    t |> run("tier-1 encode on the serving discrete home's tower ({serving_tower_dev(TowerFamily.vision)}) vs the mtmd encode dumps") @(tt : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            let mm = path_join(models_dir(), "mmproj-gemma-4-12B-it-BF16.gguf")
            if (!model_available(tt, mm) || !discrete_twin_seat(tt)) {
                return
            }
            var inscope stg <- stage_gemma4uv_embedder(mm)
            var emb = Gemma4uvEmbedder()
            mint_gemma4uv_embedder(stg, emb)   //! in memory: the loader would bake the image beside the model
            var st = Gemma4uvState()
            with_gpu_knobs() {
                set_serving_gpu_tower(true, TowerFamily.vision)
                let fixtures = [(kind = "gray", w = 480, h = 480, log = "encode.gray480.log"),
                                (kind = "cb", w = 336, h = 336, log = "encode.cb336.log"),
                                (kind = "red", w = 336, h = 336, log = "encode.red336.log"),
                                (kind = "cb", w = 672, h = 336, log = "encode.cb672x336.log")]
                for (fx in fixtures) {
                    let s0 = gpu_tower_counters(TowerFamily.vision)
                    let ran = gate_encode(tt, emb, st, fx.kind, fx.w, fx.h, fx.log, false)
                    if (ran) {
                        tt |> equal(gpu_tower_counters(TowerFamily.vision).encodes - s0.encodes, 1l, "{fx.kind} {fx.w}x{fx.h}: device-served (encodes delta)")
                        tt |> equal(gpu_tower_counters(TowerFamily.vision).convs - s0.convs, 1l, "{fx.kind} {fx.w}x{fx.h}: the stem ran on the device (convs delta)")
                    }
                }
                var o = OracleEncode()
                if (oracle_dump(tt, "gemma4-vision-oracle", "encode.cb336.log", "gemma4-vision-oracle/mint.sh", o)) {
                    var inscope embp <- mint_patch_zeroed(mm)
                    var stp = Gemma4uvState()
                    dump_poison_leg(tt, "gemma4uv", "cb", 336, 336, o, 0.0, 0l, int(embp.proj_dim), TIER1_TOL, embp, stp) $(t; var s; planes; var out) => gemma4uv_encode(t, s, planes, 336l, 336l, out)
                    delete stp
                }
                vk_drop_model_state()   //! the drop forgets the vision marks: the next encode regrows its buffers and uploads the stem off a fresh key
                let sd = gpu_tower_counters(TowerFamily.vision)
                if (gate_encode(tt, emb, st, "cb", 336, 336, "encode.cb336.log", false)) {
                    tt |> equal(gpu_tower_counters(TowerFamily.vision).encodes - sd.encodes, 1l, "after the model drop: device-served again (encodes delta)")
                }
            }
            gpu_tower_shutdown(TowerFamily.vision)
            delete st
            delete emb
        } else {
            tt |> skip("dasVulkan not present")
        }
    }
}
