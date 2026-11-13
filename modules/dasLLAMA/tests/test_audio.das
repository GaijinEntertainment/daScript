options gen2
options persistent_heap   // model-scale allocations (2.5 GB mmproj tower) — freed per test
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama_audio
require dasllama/dasllama_tower
require dasllama/dasllama_math   // with_dasllama_jobque_
require daslib/jobque_boost
require daslib/fio
require daslib/defer
require math
require _model_tier   // models_dir() + model_available() presence/size-tier gate (see the file's header)
require ?das_metal dasllama/dasllama_metal_tower   // pin the CPU-oracle encodes off-GPU + the blocks parity cell
require ?das_metal dasllama/dasllama_metal_common   // set_metal_take_max_probe_: the scratch declines' seam
require ?das_metal dasllama/dasllama_metal_prefill   // pf_hmm_ready: the half tensor GEMM, the exact lane's device condition
require _rnnt_oracle   // rnnt_seat_*: the build's device transducer seat, its pin and counters
require ?vulkan dasllama/dasllama_vulkan_tower   // the Vulkan home's seams: the storage-range legs' counters, gigaam's standalone mel
require ?vulkan dasllama/dasllama_vulkan_common   // g_gpu: the storage range the blocks-refused legs stand in
require _ctc_oracle   // nolint:STYLE030,LINT019 — ctc_leg / ctc_cpu_leg / ctc_hard_flips, inside the vulkan arm alone
require daslib/linq   // sequence_equal: the twin legs' ids
require daslib/strings_boost   // split: a kq carrier's members by name
require strings   // find: a refusal's words
require dasllama/dasllama_gguf   // GGML_TYPE_*: the retyped carriers' tensor types
require _gguf_writer   // gguf_retyped_copy: the retyped carriers the kq planner refuses
require dasllama/dasllama_kqformat   // kq_fmt_of_slot / kq_qsb: the poisoned kq leg's member planes
require dasllama/dasllama_gemma4a
require dasllama/dasllama_canary
require dasllama/dasllama_qwen3a
require dasllama/dasllama_parakeet
require dasllama/dasllama_asr   // load_asr_model, asr_needs_mmproj: the facade's Parakeet GGUF route
require dasllama/dasllama_gigaam
require dasllama/dasllama_audio_io
require _tower_twin
require ?cuda dasllama/dasllama_cuda_tower_fc   // cuda_gigaam_mel: gigaam's standalone device mel on the CUDA home
require ?cuda dasllama/dasllama_cuda_common   // cud_kernel_census_of: the kq lane's tile census on the CUDA home

// Audio tower tests. The mel/gelu/filterbank tests are self-contained; the encoder tests
// need the locally-converted Qwen2-Audio mmproj (2.5 GB, gitignored) and skip without it.
// The GigaAM cells skip without the gigaam-v3-e2e-rnnt-f16.gguf carrier (the CTC head's Vulkan twin without
// gigaam-v3-ctc-f16.gguf / gigaam-v3-e2e-ctc-f16.gguf), under a family filter
// that leaves out `gigaam`, and per clip without benchmarks/asr/corpus/ru_short.wav or ru_mid.wav; their kq lane legs
// skip without gigaam-v3-e2e-rnnt-q4_k.gguf, the mixed-member leg without gigaam-v3-e2e-rnnt-q4_k-mix-local.gguf;
// the Metal ones on a build without das_metal. The parakeet device-decode twin skips without its
// f32 carrier, per clip without jfk.wav or gb1.wav in benchmarks/asr/corpus/, where no GPU device stands up for the
// serving tower (`serving_tower_device_up`), on the lane the build's chain does not flavor (Metal's q8, a discrete home's
// exact), and where the build or the box has no device transducer seat (`rnnt_seat_skip`).
// The oracle constants come from llama-mtmd-debug (CPU backend). The discrete-home twins run on the home each family's
// seats serve on (`_gpu_knobs`' `CUDA_AUDIO_FAMILIES`) and skip without their carriers (the qwen2audio / voxtral / omni-3b
// f32 mmprojs, the E2B bf16 and E4B BF16 mmprojs, the canary f32 encoder, the parakeet f32 encoder, the ultravox f16
// mmproj, the Qwen3-ASR bf16 mmproj), without jfk.wav, without that home's device under DASLLAMA_GPU=1, on the Vulkan
// home under the CUDA ask (DASLLAMA_CUDA=1 or set_gpu_home("cuda"), where the Vulkan tower's seats stand down -
// `vulkan_twin_seat`), and on a build with das_metal; the qwen3a front also skips when interpreted. The Metal twins
// (`test_fc_metal_twin`: canary and parakeet v3; `test_gemma4a_metal_twin`: E2B and E4B) skip on a build without
// das_metal, with no Metal device standing up for the tower, under a family filter leaving the family out, without
// their carriers (the canary and parakeet v3 f32 encoders, the E2B / E4B mmprojs) and without jfk.wav; the gemma4a
// one also when interpreted. The Parakeet GGUF cells skip without parakeet-tdt-0.6b-v3-Q4_K_M.gguf (the carrier cell's
// first run and the lane-tag controls without parakeet-tdt-0.6b-v3-F32.gguf or the v3 f32 bin), the encode cells without
// jfk.wav, `test_parakeet_kq_discrete_twin` as the discrete-home twins do, `test_parakeet_kq_metal_twin` as the Metal twins do.

[test(description = "gelu_erf reproduces the exact-erf form x times Phi(x) at 1, -1, 0, 3 and 0.5 to within 1e-6 (1e-5 at 3) of the closed-form values")]
def test_gelu_erf(t : T?) {
    t |> run("gelu_erf matches the exact-erf form") @(t : T?) {
        var g = [1.0, -1.0, 0.0, 3.0, 0.5]
        gelu_erf(g, 5l)
        // 0.5·x·(1+erf(x/√2)) = x·Φ(x): Φ(1)=0.8413447, Φ(3)=0.9986501, Φ(0.5)=0.6914625
        t |> success(abs(g[0] - 0.8413447) < 1e-6, "gelu_erf(1)")
        t |> success(abs(g[1] + 0.1586553) < 1e-6, "gelu_erf(-1)")
        t |> success(g[2] == 0.0, "gelu_erf(0)")
        t |> success(abs(g[3] - 2.9959502) < 1e-5, "gelu_erf(3)")
        t |> success(abs(g[4] - 0.3457312) < 1e-6, "gelu_erf(0.5)")
    }
}

[test(description = "The 400-point Hann window is periodic: zero at 0, peak 1 at 200, 0.5 at 100, and h[399] equals h[1]")]
def test_hann_window(t : T?) {
    t |> run("periodic hann-400 endpoints and center") @(t : T?) {
        let h <- build_hann_window(400l)
        t |> success(h[0] == 0.0, "h[0] = 0")
        t |> success(abs(h[200] - 1.0) < 1e-7, "h[200] = 1 (periodic peak)")
        t |> success(abs(h[100] - 0.5) < 1e-6, "h[100] = 0.5")
        t |> success(abs(h[399] - h[1]) < 1e-6, "periodic symmetry h[399] = h[1]")
    }
}

[test(description = "The 128-mel slaney filterbank for a 400-point FFT at 16 kHz is 128 x 201 with no negative weight and nonzero support on every mel row")]
def test_mel_filterbank(t : T?) {
    t |> run("slaney filterbank shape invariants") @(t : T?) {
        let fb <- build_mel_filterbank(128l, 400l, 16000l)
        t |> success(length(fb) == 128 * 201, "128 x 201")
        var neg = 0
        var empty_rows = 0
        for (m in range(128)) {
            var row_sum = 0.0
            for (k in range(201)) {
                let v = fb[m * 201 + k]
                if (v < 0.0) {
                    neg++
                }
                row_sum += v
            }
            if (row_sum <= 0.0) {
                empty_rows++
            }
        }
        t |> success(neg == 0, "non-negative weights")
        t |> success(empty_rows == 0, "every mel row has support")
    }
}

[test(description = "Whisper log-mel puts a 440 Hz sine peak on mel row 18, clamps the spread to 2, fills padded silence uniformly at the floor, and chunks 5 s into one 3000-frame chunk and 35 s into two")]
def test_log_mel_chunks(t : T?) {
    t |> run("whisper mel: 440 Hz sine lands on its mel row, silence clamps") @(t : T?) {
        // 5 s of sine and the 30 s of zeros past it: one full 3000-frame chunk, the partial one dropped
        var s : array<float>
        s |> resize(80000)
        for (i in range(80000)) {
            s[i] = sin(2.0 * 3.14159265 * 440.0 * float(i) / 16000.0)
        }
        let chunks <- log_mel_chunks(s, 128l)
        t |> equal(length(chunks), 1, "a clip under 30 s is one chunk")
        t |> equal(chunks[0].n_mel, 128l, "n_mel")
        t |> equal(chunks[0].n_len, 3000l, "n_len")
        // 440 Hz sits in the slaney linear region: center(m=18) = 19·(m_hi/129)/lin_slope ≈ 444 Hz
        var best_m = 0
        var best_v = -1e9
        for (m in range(128)) {
            let v = chunks[0].data[m * 3000 + 100]
            if (v > best_v) {
                best_v = v
                best_m = m
            }
        }
        t |> equal(best_m, 18, "peak mel row at 440 Hz")
        // the whisper dynamic clamp bounds the spread to exactly 8 log10 units / 4
        var mn = chunks[0].data[0]
        var mx = chunks[0].data[0]
        for (v in chunks[0].data) {
            mn = min(mn, v)
            mx = max(mx, v)
        }
        t |> success(mx - mn <= 2.0 + 1e-6, "max-min <= 8/4 (global clamp)")
        // past the audio the chunk is padded silence: every value at the clamp floor
        var uniform = true
        for (m in range(128)) {
            for (i in range(600, 3000)) {
                if (chunks[0].data[m * 3000 + i] != chunks[0].data[600]) {
                    uniform = false
                }
            }
        }
        t |> success(uniform, "the frames past the audio are uniform at the floor")
        t |> success(chunks[0].data[18 * 3000 + 100] != chunks[0].data[600], "the control: a frame of the sine is off the floor")
        // 35 s of audio: the 30 s of zeros past it complete a second chunk
        s |> resize(560000)
        let two <- log_mel_chunks(s, 128l)
        t |> equal(length(two), 2, "a clip between 30 and 60 s is two chunks")
    }
}

[test(description = "swiglu_swapped_rows applies silu to the second half of each row and multiplies the first half by it, matching hand-computed values within 1e-6")]
def test_swiglu_swapped(t : T?) {
    t |> run("swiglu_swapped: silu on the SECOND half, passthrough first") @(t : T?) {
        // rows of [a b | c d] -> [silu(c)*a, silu(d)*b]; silu(1)=0.7310586, silu(-1)=-0.2689414
        let x = [2.0, 4.0, 1.0, -1.0]
        var o = [0.0, 0.0]
        swiglu_swapped_rows(o, x, 2l, 1l)
        t |> success(abs(o[0] - 2.0 * 0.7310586) < 1e-6, "silu(second)*first [0]")
        t |> success(abs(o[1] + 4.0 * 0.2689414) < 1e-6, "silu(second)*first [1]")
    }
}

// Tower-structure tests for the ultravox / voxtral / omni mmprojs (each skips without its
// local model file). proj_dim comes from the mm tensors, NOT clip.audio.projection_dim —
// the ultravox-1b GGUF metadata says 4096 while mm2 actually projects into the 2048-wide
// decoder. Token counts follow the projector chain's floor math.
//! the tower's CPU image, loaded with every GPU tower off (`#tower-dev-block`): a CPU-chain claim's carrier - the CPU never runs
//! the GPU-flavored image the box's served lane would load
def private load_cpu_audio_tower(path : string) : AudioTower {
    var tw = AudioTower()
    with_gpu_knobs() {
        set_every_gpu_tower(false)
        tw <- load_audio_tower(path, false)
    }
    return <- tw
}

[test(description = "The ultravox f32 mmproj loads as a stack-8 projector with proj_dim 2048 read from mm2 and 187 tokens a chunk, and its CPU encode of a constant mel matches llama-mtmd-debug token 0, mean and max")]
def test_ultravox_tower(t : T?) {
    t |> run("ultravox mmproj: stack-8 projector shapes + 187 tokens/chunk") @(t : T?) {
        let path = path_join(models_dir(), "mmproj-ultravox-1b-f32.gguf")
        if (!model_available(t, path)) return
        var tw <- load_cpu_audio_tower(path)
        t |> success(tw.proj_kind == AudioProjKind.ultravox, "kind")
        t |> equal(tw.stack_factor, 8l, "stack factor")
        t |> equal(tw.mm1_out, 4096l, "mm1 out (pre-swiglu)")
        t |> equal(tw.proj_dim, 2048l, "proj_dim from mm2 (metadata lies: 4096)")
        t |> success(!audio_has_avgpool(tw), "no avg-pool")
        t |> equal(audio_tokens_per_chunk(tw), 187l, "tokens per 30 s chunk (floor 1500/8)")
        if (jit_enabled()) {
            var mel = MelChunk(n_mel = tw.n_mel, n_len = AUDIO_CHUNK_FRAMES)
            mel.data |> resize(int(tw.n_mel * AUDIO_CHUNK_FRAMES))
            for (i in range(length(mel.data))) {
                mel.data[i] = 1.0
            }
            var enc <- make_encoder_state(tw)
            with_gpu_knobs() {
                set_every_gpu_tower(false)
                with_dasllama_jobque_() {
                    audio_encode(tw, enc, mel)
                }
            }
            // llama-mtmd-debug -p encode --audio one -n 3000 (CPU, f32 mmproj): token 0 opens
            // -0.831656 1.679679 0.112337 -0.558047; stats mean=0.002889 max=4.306354.
            t |> success(abs(enc.out[0] + 0.831656) < 5e-3, "tok0[0]")
            t |> success(abs(enc.out[1] - 1.679679) < 5e-3, "tok0[1]")
            t |> success(abs(enc.out[2] - 0.112337) < 5e-3, "tok0[2]")
            t |> success(abs(enc.out[3] + 0.558047) < 5e-3, "tok0[3]")
            var sum = 0.0lf
            var mx = enc.out[0]
            for (v in enc.out) {
                sum += double(v)
                mx = max(mx, v)
            }
            t |> success(abs(float(sum / double(187l * tw.proj_dim)) - 0.002889) < 5e-4, "mean")
            t |> success(abs(mx - 4.306354) < 5e-2, "max")
        }
        delete tw   // image-backed planes are locked views — only the finalizer may release them
    }
}

[test(description = "The voxtral f32 mmproj loads as an avg-pooled stack-4 projector with proj_dim 3072 and 187 tokens a chunk, and its CPU encode of a constant mel matches llama-mtmd-debug token 0, mean and max")]
def test_voxtral_tower(t : T?) {
    t |> run("voxtral mmproj: avg-pool + stack-4 projector shapes + 187 tokens/chunk") @(t : T?) {
        let path = path_join(models_dir(), "voxtral-mini-mmproj-f32.gguf")
        if (!model_available(t, path)) return
        var tw <- load_cpu_audio_tower(path)
        t |> success(tw.proj_kind == AudioProjKind.voxtral, "kind")
        t |> equal(tw.stack_factor, 4l, "stack factor")
        t |> equal(tw.mm1_out, 3072l, "mm1 out")
        t |> equal(tw.proj_dim, 3072l, "proj_dim")
        t |> success(audio_has_avgpool(tw), "avg-pool")
        t |> equal(audio_tokens_per_chunk(tw), 187l, "tokens per 30 s chunk (floor 750/4)")
        if (jit_enabled()) {
            var mel = MelChunk(n_mel = tw.n_mel, n_len = AUDIO_CHUNK_FRAMES)
            mel.data |> resize(int(tw.n_mel * AUDIO_CHUNK_FRAMES))
            for (i in range(length(mel.data))) {
                mel.data[i] = 1.0
            }
            var enc <- make_encoder_state(tw)
            with_gpu_knobs() {
                set_every_gpu_tower(false)
                with_dasllama_jobque_() {
                    audio_encode(tw, enc, mel)
                }
            }
            // llama-mtmd-debug -p encode --audio one -n 3000 (CPU, f32 mmproj): token 0 opens
            // 0.967699 -0.765652 0.050477 -0.571310; stats mean=-0.001517 max=1.459851.
            t |> success(abs(enc.out[0] - 0.967699) < 5e-3, "tok0[0]")
            t |> success(abs(enc.out[1] + 0.765652) < 5e-3, "tok0[1]")
            t |> success(abs(enc.out[2] - 0.050477) < 5e-3, "tok0[2]")
            t |> success(abs(enc.out[3] + 0.571310) < 5e-3, "tok0[3]")
            var sum = 0.0lf
            var mx = enc.out[0]
            for (v in enc.out) {
                sum += double(v)
                mx = max(mx, v)
            }
            t |> success(abs(float(sum / double(187l * tw.proj_dim)) + 0.001517) < 5e-4, "mean")
            t |> success(abs(mx - 1.459851) < 5e-2, "max")
        }
        delete tw   // image-backed planes are locked views — only the finalizer may release them
    }
}

def private zero_plane(var q : array<auto(PT)>; off, n : int64) {
    for (i in range64(n)) {
        q[off + i] = default<PT>
    }
}

//! one zeroed block moves a 32-block tower less than its own q8 chain's noise, so the poison zeroes an eighth of the blocks
def private poison_blocks(n_layer : int64) : int64 => max(1l, n_layer / 8l)

//! `zero_block` >= 0 zeroes `poison_blocks` blocks from it on, on the lane's planes; `dev` the image flavor (`none` the CPU image, `#tower-dev-block`)
def private mint_aud(path : string; q8 : bool; zero_block : int64; dev : TowerDev = TowerDev.none) : AudioTower {
    var inscope stg <- tower_dev_staged(dev) $ { return <- stage_audio_tower(path, q8) }
    if (zero_block >= 0l) {
        for (b in range64(zero_block, min(stg.e.n_layer, zero_block + poison_blocks(stg.e.n_layer)))) {
            if (q8) {
                zero_plane(stg.qblob, stg.e.qlayers_off + b * stg.e.qlayer_stride, stg.e.qlayer_stride)
            } elif (stg.e.gemm_f16) {
                zero_plane(stg.wblob, stg.e.qlayers_off + b * stg.e.qlayer_stride, stg.e.qlayer_stride)
            } else {
                zero_plane(stg.fblob, stg.e.layers_off + b * stg.e.layer_stride, stg.e.layer_stride)
            }
        }
    }
    var tw = AudioTower()
    mint_audio_tower(stg, tw, q8, dev)
    return <- tw
}

def private aud_twin_encode(tw : AudioTower; var enc : EncoderState; mel : MelChunk; var out : array<float>) : int64 {
    audio_encode(tw, enc, mel)
    out := enc.out
    return long_length(out)
}

//! the exact tower's lane on the serving home (`exact_flavor_leg`): `twx`, the CPU image, on the CPU chain against the home's f16 flavor
[cold_path]
def private blocks_exact_leg(t : T?; tag : string; path : string; twx : AudioTower; var encx : EncoderState; mel : MelChunk) {
    let home = serving_tower_dev(TowerFamily.whisper)
    let tail0 = gpu_tower_tail_encodes(TowerFamily.whisper)
    let convs = exact_flavor_leg(t, tag, TowerFamily.whisper, TWIN_EXACT_REL_L2_VK, twx, encx, twx.n_ctx, $(zb) { return <- mint_aud(path, false, zb, home) },
        $(tw) { return <- make_encoder_state(tw) }) $(e; var s; var out) => aud_twin_encode(e, s, mel, out)
    t |> equal(convs, 2l, "{tag} exact lane: the conv stem served on the device, the clean encode and the poisoned one")
    t |> equal(gpu_tower_tail_encodes(TowerFamily.whisper) - tail0, 2l, "{tag} exact lane: the projector tail rode the device behind both encodes' blocks")
}

[cold_path]
def private blocks_twin(t : T?; family, file : string; layers_claimed : int64) {
    static_if (typeinfo builtin_module_exists(vulkan) || typeinfo builtin_module_exists(cuda)) {
        let path = path_join(models_dir(), file)
        if (!model_available(t, path) || !discrete_audio_twin_seat(t, TowerFamily.whisper)) {
            return
        }
        let home = serving_tower_dev(TowerFamily.whisper)
        let tag = "{family} synthetic mel"
        let knobs = gpu_knobs()
        defer() {
            set_gpu_knobs(knobs)
        }
        var twx <- mint_aud(path, false, -1l)
        var tw <- mint_aud(path, true, -1l)
        t |> success(!twx.q8 && tw.q8, "{tag}: the exact and the q8 towers minted")
        if (layers_claimed > 0l) {
            t |> equal(tw.n_layer, layers_claimed, "{tag}: the tower's depth is the {layers_claimed} blocks the cell names")
        }
        let mel <- synth_mel(tw.n_mel)
        var encx <- make_encoder_state(twx)
        var enc <- make_encoder_state(tw)
        let spec = TwinSpec(family = family, tag = tag, n_layer = tw.n_layer, metric = TwinMetric.rel_l2, seat_family = TowerFamily.whisper)
        let tail0 = gpu_tower_tail_encodes(TowerFamily.whisper)
        var twd <- mint_aud(path, true, -1l, home)
        var encd <- make_encoder_state(twd)
        let r <- twin_three_way(t, spec, tw, enc, twd, encd, twx, encx, $(e; var s; var out) => aud_twin_encode(e, s, mel, out)) $(legs) {
            t |> equal(legs.dev.encodes - legs.pre.encodes, 1l, "{tag}: exactly the device leg dispatched")
            t |> equal(legs.dev.blocks - legs.pre.blocks, tw.n_layer, "{tag}: blocks delta = n_layer")
            t |> equal(legs.dev.rows - legs.pre.rows, tw.n_ctx, "{tag}: rows delta = n_ctx")
        }
        delete encd
        delete twd
        t |> equal(gpu_tower_tail_encodes(TowerFamily.whisper) - tail0, 1l, "{tag}: the projector tail rode the device behind the device leg's blocks")
        if (r.bar > 0.0lf) {
            var twp <- mint_aud(path, true, tw.n_layer / 2l, home)
            var encp <- make_encoder_state(twp)
            twin_poison(t, r, tw.n_layer / 2l, twp, encp) $(e; var s; var out) => aud_twin_encode(e, s, mel, out)
            delete encp
            delete twp
        }
        delete enc
        delete tw
        blocks_exact_leg(t, tag, path, twx, encx, mel)
        delete encx
        delete twx
        gpu_tower_shutdown(TowerFamily.whisper)
    } else {
        t |> skip("no discrete GPU home in this build (dasVulkan, dasCUDA)")
    }
}

//! `n` GEMM weights at `off` zeroed on the staging's lane: the q8 plane, the f16 lane's halfwords, else the f32 blob
def private zero_g4a_gemm(var stg : Gemma4aStaging; off, n : int64) {
    if (stg.e.q8) {
        zero_plane(stg.qblob, off, n)
    } elif (stg.e.gemm_f16) {
        zero_plane(stg.wblob, off, n)
    } else {
        zero_plane(stg.blob, off, n)
    }
}

//! `zero_block` >= 0 zeroes the eleven GEMM planes of `poison_blocks` blocks from it on, on the lane's planes; `dev` the image flavor
def private mint_g4a(mm : string; q8 : bool; zero_block : int64; dev : TowerDev = TowerDev.none) : Gemma4aEncoder {
    set_gemma4a_q8(q8)
    defer() {
        reset_gemma4a_q8()
    }
    var inscope stg <- tower_dev_staged(dev) $ { return <- stage_gemma4a_encoder(mm) }
    if (zero_block >= 0l) {
        let d = stg.e.d_model
        let ff = stg.e.n_ff
        for (b in range64(zero_block, min(stg.e.n_layer, zero_block + poison_blocks(stg.e.n_layer)))) {
            let lo = stg.layers[b]
            for (w in fixed_array(lo.q, lo.k, lo.v, lo.o, lo.attn_k_rel, lo.conv_pw2)) {
                zero_g4a_gemm(stg, w, d * d)
            }
            for (w in fixed_array(lo.ff_up, lo.ff_down, lo.ff_up_1, lo.ff_down_1)) {
                zero_g4a_gemm(stg, w, d * ff)
            }
            zero_g4a_gemm(stg, lo.conv_pw1, d * 2l * d)
        }
    }
    var enc = Gemma4aEncoder()
    mint_gemma4a_encoder(stg, enc, "", dev)
    return <- enc
}

//! the first 3 s of a clip: an encode short enough to size the residency's scratch under the full clip's
def private clip_head(samples : array<float>) : array<float> {
    let n = min(long_length(samples), 3l * AUDIO_SAMPLE_RATE)
    return <- [for (i in range64(n)); samples[i]]
}

//! the regrow legs at `r`'s bar on the twin family's home (short encode, clip, clip after a shutdown, poison); `exactly_three`: one dispatch an encode
[cold_path, unused_argument(t, tag, enc, st, samples, r, exactly_three, mint_poisoned, encode)]
def private twin_regrow_legs(t : T?; tag : string; enc : auto(TW); var st : auto(ST); samples : array<float>; r : TwinResult;
                             exactly_three : bool; mint_poisoned : block<() : TW>;
                             encode : block<(e : TW; var s : ST; clip : array<float>; var out : array<float>) : int64>) {
    static_if (typeinfo builtin_module_exists(vulkan) || typeinfo builtin_module_exists(cuda)) {
        let fam = r.spec.seat_family
        let head <- clip_head(samples)
        var dev_head, dev_again, dev_fresh : array<float>
        set_serving_gpu_tower(true, fam)
        let e0 = gpu_tower_encodes(fam)
        with_dasllama_jobque_() {
            invoke(encode, enc, st, head, dev_head)
            invoke(encode, enc, st, samples, dev_again)
            gpu_tower_shutdown(fam)
            invoke(encode, enc, st, samples, dev_fresh)
        }
        let n_enc = gpu_tower_encodes(fam) - e0
        if (exactly_three) {
            t |> equal(n_enc, 3l, "{tag}: the short, the regrown and the fresh encodes ran on the device")
        } else {
            t |> success(n_enc >= 3l, "{tag}: the short, the regrown and the fresh encodes ran on the device")
        }
        tower_twin_held(t, "{tag} regrown scratch", fam, r.reference, dev_again, r.bar)
        tower_twin_held(t, "{tag} after shutdown", fam, r.reference, dev_fresh, r.bar)
        var encp <- invoke(mint_poisoned)
        var stp = ST()
        var poisoned : array<float>
        let p0 = gpu_tower_encodes(fam)
        with_dasllama_jobque_() {
            invoke(encode, encp, stp, samples, poisoned)
        }
        t |> success(gpu_tower_encodes(fam) > p0, "{tag} poison: device-served (encodes delta)")
        tower_twin_poisoned(t, tag, fam, r.reference, poisoned, r.bar)
        delete poisoned
        delete stp
        delete encp
        delete dev_head
        delete dev_again
        delete dev_fresh
    }
}

//! one gemma4a encoder `tag` on jfk three ways on the q8 lane with its regrow and poison legs, then its exact lane on the home's exact image
[cold_path, unused_argument(tst, tag, mm_file)]
def private g4a_discrete_twin(tst : T?; tag, mm_file : string; want_proj : int64) {
    tst |> run("gemma4a {tag}: the discrete home's Conformer chain vs the CPU q8 chain on jfk, then its exact lane, the exact chain the reference") @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan) || typeinfo builtin_module_exists(cuda)) {
            let mm = path_join(models_dir(), mm_file)
            let jt = "gemma4a {tag} jfk"
            jfk_twin_cell(t, mm, false, TowerFamily.gemma4a) $(samples) {
                let home = serving_tower_dev(TowerFamily.gemma4a)
                var encx <- mint_g4a(mm, false, -1l)
                var enc <- mint_g4a(mm, true, -1l)
                t |> success(!encx.q8 && enc.q8, "the exact and the q8 encoders minted")
                t |> equal(gemma4a_mid_dim(enc), 1536l, "{jt}: the encoder's own width")
                t |> equal(enc.proj_dim, want_proj, "{jt}: the embedder projects to the decoder's width")
                var encd <- mint_g4a(mm, true, -1l, home)
                var stx = Gemma4aState()
                var st = Gemma4aState()
                var std = Gemma4aState()
                var n_exact = 0l
                let spec = TwinSpec(family = "gemma4a", tag = jt, n_layer = enc.n_layer, metric = TwinMetric.rel_l2, seat_family = TowerFamily.gemma4a)
                let r <- twin_three_way(t, spec, enc, st, encd, std, encx, stx, $(e; var s; var out) => gemma4a_encode_all(e, s, samples, out)) $(legs) {
                    n_exact = legs.n_exact
                    let n_chunks = legs.dev.encodes - legs.pre.encodes
                    t |> equal(legs.n_cpu, legs.n_exact, "{jt}: the q8 chain's token count")
                    t |> equal(legs.n_dev, legs.n_exact, "{jt}: the device chain's token count")
                    t |> success(n_chunks >= 1l, "{jt}: the device served the chunk encodes ({n_chunks})")
                    t |> equal(legs.dev.blocks - legs.pre.blocks, n_chunks * enc.n_layer, "{jt}: every block of every served encode ran on the driver")
                    front_served(t, jt, legs.dev.convs - legs.pre.convs, n_chunks, legs.dev.declines - legs.cpu.declines)
                }
                if (r.bar > 0.0lf) {
                    var st2 = Gemma4aState()
                    twin_regrow_legs(t, jt, encd, st2, samples, r, false, $ { return <- mint_g4a(mm, true, enc.n_layer / 2l, home) }) $(e; var s; clip; var out) => gemma4a_encode_all(e, s, clip, out)
                    delete st2
                }
                delete std
                delete encd
                let xconvs = exact_flavor_leg(t, jt, TowerFamily.gemma4a, encx, stx, n_exact, $(zb) { return <- mint_g4a(mm, false, zb, home) }) $(e; var s; var out) => gemma4a_encode_all(e, s, samples, out)
                t |> success(xconvs >= 2l, "{jt} exact lane: the chunk front served on both device encodes ({xconvs} dispatches)")
                delete stx
                delete st
                delete enc
                delete encx
                gpu_tower_shutdown(TowerFamily.gemma4a)
            }
        } else {
            t |> skip("no discrete GPU home in this build (dasVulkan, dasCUDA)")
        }
    }
}

//! E2B's audio embedder is square on the encoder's 1536; E4B's widens it to the decoder's 2560 - the projector tail at two widths
[test(description = "The gemma4a E2B and E4B encoders on the discrete home stay within the three-way bar the CPU q8 chain sets against the exact chain on jfk, across regrown and fresh residencies, and a zeroed-blocks poison exceeds it"), arch(at = "../ARCHITECTURE_GPU_TOWER_VULKAN.md#vk-tower-encode-chains")]
def test_gemma4a_discrete_twin(tst : T?) {
    g4a_discrete_twin(tst, "E2B", "mmproj-gemma-4-E2B-it-bf16.gguf", 1536l)
    g4a_discrete_twin(tst, "E4B", "mmproj-gemma-4-E4B-it-BF16.gguf", 2560l)
}

//! the GEMM planes of one FastConformer block zeroed in its GEMM plane (the q8 quants, or the halfwords) - eleven, or ten where
//! the family has no pos projection (`pos_w` -1): the poison of the device twins' control
def private poison_fc_block(var plane : array<auto(PT)>; lo : CanaryLayerOffs; d, ff : int64) {
    for (w in fixed_array(lo.q_w, lo.k_w, lo.v_w, lo.pos_w, lo.o_w, lo.pw2_w)) {
        if (w >= 0l) {
            zero_plane(plane, w, d * d)
        }
    }
    for (w in fixed_array(lo.ff1a_w, lo.ff1b_w, lo.ff2a_w, lo.ff2b_w)) {
        zero_plane(plane, w, d * ff)
    }
    zero_plane(plane, lo.pw1_w, d * 2l * d)
}

//! `zero_block` >= 0: the GEMM planes of `poison_blocks` blocks from that block on zeroed in `plane`, each block's offsets
//! `layer` maps onto canary's record
def private poison_fc_blocks(var plane : array<auto(PT)>; n_layer, zero_block, d, ff : int64; layer : block<(b : int64) : CanaryLayerOffs>) {
    return if (zero_block < 0l)
    for (b in range64(zero_block, min(n_layer, zero_block + poison_blocks(n_layer)))) {
        poison_fc_block(plane, invoke(layer, b), d, ff)
    }
}

//! one encode of `run` over `st`, the encoder's `width`-wide rows of `st.x` it answered for into `out`; its frame count
def private x_rows_encode(var st : auto(ST); width : int64; var out : array<float>; run : block<(var s : ST) : int64>) : int64 {
    let n = invoke(run, st)
    delete out
    out <- copy_span(st.x, 0l, n * width)
    return n
}

//! a Vulkan-flavored model's encode under a storage range stood in at `msr_at`: the front stands down and the CPU chain refuses (`#tower-dev-block`)
[unused_argument(t, tag, fam, msr_at, run)]   // without dasVulkan the gated body is empty
def private blocks_refused_leg(t : T?; tag : string; fam : TowerFamily; msr_at : int64; run : block<() : void>) {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        return if (tower_family_cuda(fam))
        let msr = g_gpu.msr
        let s0 = vulkan_tower_stats()
        let k0 <- vulkan_tower_declines()
        g_gpu.msr = msr_at
        var refused = ""
        try {
            invoke(run)
        } recover {
            refused = "refused"
        }
        g_gpu.msr = msr
        let s1 = vulkan_tower_stats()
        let k1 <- vulkan_tower_declines()
        t |> equal(s1.convs - s0.convs, 0l, "{tag}, blocks past the range: no front dispatch counted")
        t |> equal(s1.mels - s0.mels, 0l, "{tag}, blocks past the range: no device mel counted")
        t |> equal(s1.encodes - s0.encodes, 0l, "{tag}, blocks past the range: no encode served")
        t |> equal((k1?["memory"] ?? 0l) - (k0?["memory"] ?? 0l), 0l, "{tag}, blocks past the range: the front's buffers fit (no memory decline)")
        t |> equal(refused, "refused", "{tag}, blocks past the range: the Vulkan-flavored model refuses the CPU chain")
    }
}

//! the synthetic speech-scale mel every twin reads: the all-ones fixture is degenerate (identical rows make every attention weight
//! a near-tie, so single elements flip O(1)); `other` is the second clip, the control's rows outside the bar
def private synth_mel_fill(var mel : MelChunk; other : bool) {
    mel.data |> resize(mel.n_mel * AUDIO_CHUNK_FRAMES)
    for (i in range(length(mel.data))) {
        mel.data[i] = other ? sin(float(i) * 0.0091) * 0.8 + sin(float(i) * 0.00417) * 0.5 : sin(float(i) * 0.0137) * 0.8 + sin(float(i) * 0.00291) * 0.5
    }
}

def private synth_mel(n_mel : int64) : MelChunk {
    var mel = MelChunk(n_mel = n_mel, n_len = AUDIO_CHUNK_FRAMES)
    synth_mel_fill(mel, false)
    return <- mel
}

//! `zero_block` >= 0 zeroes the eleven GEMM planes of `poison_blocks` blocks from it on, in the lane's GEMM plane; `dev` the image flavor
def private mint_cn(path : string; q8 : bool; zero_block : int64; dev : TowerDev = TowerDev.none) : CanaryEncoder {
    var inscope stg <- tower_dev_staged(dev) $ { return <- stage_canary_encoder(path, q8) }
    if (q8) {
        poison_fc_blocks(stg.qblob, stg.e.n_layer, zero_block, stg.e.d_model, stg.e.n_ff) $(b) => stg.layers[b]
    } elif (stg.e.gemm_f16) {
        poison_fc_blocks(stg.wblob, stg.e.n_layer, zero_block, stg.e.d_model, stg.e.n_ff) $(b) => stg.layers[b]
    } else {
        poison_fc_blocks(stg.blob, stg.e.n_layer, zero_block, stg.e.d_model, stg.e.n_ff) $(b) => stg.layers[b]
    }
    var enc = CanaryEncoder()
    mint_canary_encoder(stg, enc, dev)
    return <- enc
}

[cold_path]
def private cn_twin_encode(enc : CanaryEncoder; var st : CanaryState; clip : array<float>; var out : array<float>) : int64 {
    canary_log_mel(enc, clip, st)
    let n = canary_encode(enc, st)
    delete out
    out <- copy_span(st.out, 0l, n * enc.proj_dim)   // the encode's rows: the state's plane keeps a longer clip's past them
    return n
}

//! a FastConformer's exact lane on family `fam`'s home (`exact_flavor_leg` over `mint`'s halfword flavor), the front on both device legs
[cold_path]
def private fc_exact_lane_leg(t : T?; tag : string; fam : TowerFamily; tw_exact : auto(TW); var st_exact : auto(ST); mint : block<(zb : int64) : TW>;
                              encode : block<(e : TW; var s : ST; var out : array<float>) : int64>) {
    let convs = exact_flavor_leg(t, tag, fam, tw_exact, st_exact, -1l, mint, encode)
    front_served(t, "{tag} exact lane", convs, 2l, 0l)
}

[test(description = "The canary encoder on the discrete home serves its front and every block on the device within the three-way bar on jfk, holds it across regrown and fresh residencies and on the exact f16 lane, and a poison exceeds it"), arch(at = "../ARCHITECTURE_GPU_TOWER_VULKAN.md#vk-tower-encode-chains")]
def test_canary_discrete_twin(tst : T?) {
    tst |> run("canary-qwen 2.5B: the discrete home's FastConformerblocks vs the CPU q8 chain on jfk, the exact chain the reference") @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan) || typeinfo builtin_module_exists(cuda)) {
            let path = path_join(models_dir(), "canary-qwen-2.5b-encoder-f32.bin")
            jfk_twin_cell(t, path, false, TowerFamily.canary) $(samples) {
                let home = serving_tower_dev(TowerFamily.canary)
                var encx <- mint_cn(path, false, -1l)
                var enc <- mint_cn(path, true, -1l)
                t |> success(!encx.q8 && enc.q8, "the exact and the q8 encoders minted")
                var encd <- mint_cn(path, true, -1l, home)
                var stx = CanaryState()
                var st = CanaryState()
                var std = CanaryState()
                let spec = TwinSpec(family = "canary", tag = "canary jfk", n_layer = enc.n_layer, metric = TwinMetric.rel_l2, seat_family = TowerFamily.canary)
                let r <- twin_three_way(t, spec, enc, st, encd, std, encx, stx, $(e; var s; var out) => cn_twin_encode(e, s, samples, out)) $(legs) {
                    t |> equal(legs.n_cpu, legs.n_exact, "canary jfk: the q8 chain's frame count")
                    t |> equal(legs.n_dev, legs.n_exact, "canary jfk: the device chain's frame count")
                    t |> equal(legs.dev.encodes - legs.pre.encodes, 1l, "canary jfk: the device served the encode")
                    t |> equal(legs.dev.blocks - legs.pre.blocks, enc.n_layer, "canary jfk: every block ran on the driver")
                    front_served(t, "canary jfk", legs.dev.convs - legs.pre.convs, 1l, legs.dev.declines - legs.cpu.declines)
                }
                if (r.bar > 0.0lf) {
                    var st2 = CanaryState()
                    twin_regrow_legs(t, "canary jfk", encd, st2, samples, r, true, $ { return <- mint_cn(path, true, enc.n_layer / 2l, home) }) $(e; var s; clip; var out) => cn_twin_encode(e, s, clip, out)
                    delete st2
                }
                delete std
                delete encd
                delete enc
                fc_exact_lane_leg(t, "canary jfk", TowerFamily.canary, encx, stx, $(zb) { return <- mint_cn(path, false, zb, home) }) $(e; var s; var out) => cn_twin_encode(e, s, samples, out)
                delete stx
                delete st
                delete encx
                gpu_tower_shutdown(TowerFamily.canary)
            }
        } else {
            t |> skip("no discrete GPU home in this build (dasVulkan, dasCUDA)")
        }
    }
}

//! parakeet staged and minted in memory on the lane asked - `kq` a quantized GGUF's own planes, else `q8` or the exact lane; `zero_block` >= 0 zeroes the GEMM planes of the blocks from it on
def private mint_pk(path : string; q8 : bool; zero_block : int64; dev : TowerDev = TowerDev.none; kq : bool = false) : ParakeetModel {
    var inscope stg <- tower_dev_staged(dev) $ {
        return <- stage_parakeet_kq_model(path) if (kq)
        return <- stage_parakeet_model(path, q8)
    }
    if (kq) {
        if (zero_block >= 0l) {
            poison_kq_seats(stg.gemm_seats, zero_block * PK_LAYER_SEATS, stg.e.n_layer * PK_LAYER_SEATS, stg.kq, stg.qblob)
        }
    } elif (q8) {
        poison_fc_blocks(stg.qblob, stg.e.n_layer, zero_block, stg.e.n_state, 4l * stg.e.n_state) $(b) => pk_fc_layer(pk_layer_offsets(stg.e, b))
    } elif (stg.e.gemm_f16) {
        poison_fc_blocks(stg.wblob, stg.e.n_layer, zero_block, stg.e.n_state, 4l * stg.e.n_state) $(b) => pk_fc_layer(pk_layer_offsets(stg.e, b))
    } else {
        poison_fc_blocks(stg.blob, stg.e.n_layer, zero_block, stg.e.n_state, 4l * stg.e.n_state) $(b) => pk_fc_layer(pk_layer_offsets(stg.e, b))
    }
    var m = ParakeetModel()
    mint_parakeet_model(stg, m, q8, dev)
    return <- m
}

[cold_path]
def private pk_twin_encode(m : ParakeetModel; var st : ParakeetState; clip : array<float>; var out : array<float>) : int64 {
    return x_rows_encode(st, m.n_state, out) $(var s) {
        parakeet_log_mel(clip, m, s)
        return parakeet_encode(m, s)
    }
}

let private SHORT_CLIP_SAMPLES = fixed_array(320l, 480l, 640l, 800l, 1600l)   //! 20 to 100 ms: a few mel frames each, from under four frames to past it
let private SHORT_CLIP_BAR = 5e-2lf   //! the device rows' rel_l2 from the CPU q8 chain's over a clip of a few frames - two q8 chains, the device's GEMMs on the f16 feed

//! the next sample of a seeded white noise in [-0.3, 0.3)
def private short_clip_noise(var seed : uint&) : float {
    seed = seed * 1664525u + 1013904223u
    return (float(seed >> 8u) / 16777216.0 - 0.5) * 0.6
}

//! clips a few mel frames long through `twd` (the GPU-flavored tower) on family `fam`'s home against `tw` (its CPU image) at `SHORT_CLIP_BAR`, `encode` the family's encode of a clip
[cold_path, unused_argument(t, tag, fam, tw, st, twd, std)]   // without a discrete home the gated body is empty
def private short_clip_cell(t : T?; tag : string; fam : TowerFamily; tw : auto(TW); var st : auto(ST); twd : TW; var std : ST;
                            encode : block<(e : TW; var s : ST; clip : array<float>; var out : array<float>) : int64>) {
    static_if (typeinfo builtin_module_exists(vulkan) || typeinfo builtin_module_exists(cuda)) {
        var last_cpu : array<float>
        for (n in SHORT_CLIP_SAMPLES) {
            var inscope clip <- [for (i in range64(n)); 0.3 * sin(float(i) * 0.05) + 0.1 * sin(float(i) * 0.31)]
            var cpu, dev : array<float>
            var n_cpu = 0l
            var n_dev = 0l
            var s0 = GpuTowerCounters()
            var s1 = GpuTowerCounters()
            var s2 = GpuTowerCounters()
            with_gpu_knobs() {
                with_dasllama_jobque_() {
                    set_serving_gpu_tower(true, fam)
                    s0 = gpu_tower_counters(fam)
                    n_cpu = invoke(encode, tw, st, clip, cpu)
                    s1 = gpu_tower_counters(fam)
                    n_dev = invoke(encode, twd, std, clip, dev)
                    s2 = gpu_tower_counters(fam)
                }
            }
            let ctag = "{tag} {n}-sample clip"
            t |> equal(s1.encodes - s0.encodes, 0l, "{ctag}: the CPU image's leg never dispatched")
            t |> equal(s2.encodes - s1.encodes, 1l, "{ctag}: the device served the encode")
            t |> equal(s2.declines - s1.declines, 0l, "{ctag}: the device leg declined nothing")
            t |> equal(n_dev, n_cpu, "{ctag}: the device's row count is the CPU's")
            var got = TowerParityStats()
            if (n_cpu > 0l && tower_parity_stats(t, ctag, cpu, dev, got)) {
                to_log(LOG_INFO, "test_audio: {ctag}: device vs CPU q8 rel_l2 {got.rel_l2} over {n_cpu} rows\n")
                t |> success(got.rel_l2 <= SHORT_CLIP_BAR, "{ctag}: the device rows within {SHORT_CLIP_BAR} of the CPU's ({got.rel_l2})")
            }
            last_cpu := cpu
            delete cpu
            delete dev
        }
        var seed = 12345u
        var inscope noise <- [for (_i in range64(SHORT_CLIP_SAMPLES[length(SHORT_CLIP_SAMPLES) - 1])); short_clip_noise(seed)]
        var other : array<float>
        with_gpu_knobs() {
            with_dasllama_jobque_() {
                set_serving_gpu_tower(true, fam)
                invoke(encode, twd, std, noise, other)
            }
        }
        var ctl = TowerParityStats()
        if (tower_parity_stats(t, "{tag} control", last_cpu, other, ctl)) {
            t |> success(is_finite(ctl.rel_l2) && ctl.rel_l2 > SHORT_CLIP_BAR, "{tag}: the device's rows of a noise clip land outside the bar of the longest clip's CPU rows ({ctl.rel_l2}) - the control")
        }
        delete other
        delete last_cpu
    }
}

[test(description = "Clips of 320 to 1600 samples through the gemma4a, canary and parakeet GPU-flavored images serve as one device encode with no decline, the CPU image row count and rows within SHORT_CLIP_BAR, a noise clip outside it"), arch(at = "../ARCHITECTURE_GPU_TOWER_VULKAN.md#vk-tower-encode-chains")]
def test_vulkan_short_clips(tst : T?) {
    tst |> run("gemma4a E2B: clips of 320 to 1600 samples serve on the discrete home's image as the CPU image reads them") @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan) || typeinfo builtin_module_exists(cuda)) {
            let mm = path_join(models_dir(), "mmproj-gemma-4-E2B-it-bf16.gguf")
            return if (!model_available(t, mm) || !discrete_audio_twin_seat(t, TowerFamily.gemma4a))
            var enc <- mint_g4a(mm, true, -1l)
            var encd <- mint_g4a(mm, true, -1l, serving_tower_dev(TowerFamily.gemma4a))
            var st = Gemma4aState()
            var std = Gemma4aState()
            short_clip_cell(t, "gemma4a", TowerFamily.gemma4a, enc, st, encd, std) $(e; var s; clip; var out) => gemma4a_encode_all(e, s, clip, out)
            delete std
            delete st
            delete encd
            delete enc
            gpu_tower_shutdown(TowerFamily.gemma4a)
        } else {
            t |> skip("no discrete GPU home in this build (dasVulkan, dasCUDA)")
        }
    }
    tst |> run("canary: clips of 320 to 1600 samples serve on the discrete home's image as the CPU image reads them") @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan) || typeinfo builtin_module_exists(cuda)) {
            let path = path_join(models_dir(), "canary-qwen-2.5b-encoder-f32.bin")
            return if (!model_available(t, path) || !discrete_audio_twin_seat(t, TowerFamily.canary))
            var enc <- mint_cn(path, true, -1l)
            var encd <- mint_cn(path, true, -1l, serving_tower_dev(TowerFamily.canary))
            var st = CanaryState()
            var std = CanaryState()
            short_clip_cell(t, "canary", TowerFamily.canary, enc, st, encd, std) $(e; var s; clip; var out) => cn_twin_encode(e, s, clip, out)
            delete std
            delete st
            delete encd
            delete enc
            gpu_tower_shutdown(TowerFamily.canary)
        } else {
            t |> skip("no discrete GPU home in this build (dasVulkan, dasCUDA)")
        }
    }
    tst |> run("parakeet-TDT 0.6B v2: clips of 320 to 1600 samples serve on the discrete home's image as the CPU image reads them") @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan) || typeinfo builtin_module_exists(cuda)) {
            let path = path_join(whisper_dir(), "ggml-parakeet-tdt-0.6b-v2-f32.bin")
            return if (!model_available(t, path) || !discrete_audio_twin_seat(t, TowerFamily.parakeet))
            var m <- mint_pk(path, true, -1l)
            var md <- mint_pk(path, true, -1l, serving_tower_dev(TowerFamily.parakeet))
            var st = ParakeetState()
            var std = ParakeetState()
            short_clip_cell(t, "parakeet", TowerFamily.parakeet, m, st, md, std) $(e; var s; clip; var out) => pk_twin_encode(e, s, clip, out)
            delete std
            delete st
            delete md
            delete m
            gpu_tower_shutdown(TowerFamily.parakeet)
        } else {
            t |> skip("no discrete GPU home in this build (dasVulkan, dasCUDA)")
        }
    }
}

[test(description = "Parakeet-TDT v2 on the discrete home serves its front and every block on the device within the three-way bar on jfk, refuses the CPU chain past the storage range, and a poison and the exact lane hold their bars"), arch(at = "../ARCHITECTURE_GPU_TOWER_VULKAN.md#vk-tower-encode-chains")]
def test_parakeet_discrete_twin(tst : T?) {
    tst |> run("parakeet-TDT 0.6B v2: the discrete home's FastConformerfront and blocks vs the CPU q8 chain on jfk, the exact chain the reference") @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan) || typeinfo builtin_module_exists(cuda)) {
            let path = path_join(whisper_dir(), "ggml-parakeet-tdt-0.6b-v2-f32.bin")
            jfk_twin_cell(t, path, false, TowerFamily.parakeet) $(samples) {
                let home = serving_tower_dev(TowerFamily.parakeet)
                var mx <- mint_pk(path, false, -1l)
                var m <- mint_pk(path, true, -1l)
                var md <- mint_pk(path, true, -1l, home)
                t |> success(!mx.q8 && m.q8 && md.dev_backend == home, "the exact and the q8 models minted, the q8 lane's {home} image beside")
                var stx = ParakeetState()
                var st = ParakeetState()
                var std = ParakeetState()
                let spec = TwinSpec(family = "parakeet", tag = "parakeet jfk", n_layer = m.n_layer, metric = TwinMetric.rel_l2, seat_family = TowerFamily.parakeet)
                let r <- twin_three_way(t, spec, m, st, md, std, mx, stx, $(e; var s; var out) => pk_twin_encode(e, s, samples, out)) $(legs) {
                    t |> equal(legs.n_cpu, legs.n_exact, "parakeet jfk: the q8 chain's frame count")
                    t |> equal(legs.n_dev, legs.n_exact, "parakeet jfk: the device chain's frame count")
                    t |> equal(legs.dev.encodes - legs.pre.encodes, 1l, "parakeet jfk: the device served the encode")
                    t |> equal(legs.dev.blocks - legs.pre.blocks, m.n_layer, "parakeet jfk: every block ran on the driver")
                    front_served(t, "parakeet jfk", legs.dev.convs - legs.pre.convs, 1l, legs.dev.declines - legs.cpu.declines)
                }
                if (r.bar > 0.0lf) {
                    var st_rf = ParakeetState()
                    var out_rf : array<float>
                    blocks_refused_leg(t, "parakeet jfk", TowerFamily.parakeet, 256l << 20l) $ {
                        pk_twin_encode(md, st_rf, samples, out_rf)
                    }
                    delete st_rf
                    var mp <- mint_pk(path, true, m.n_layer / 2l, home)
                    var stp = ParakeetState()
                    twin_poison(t, r, m.n_layer / 2l, mp, stp) $(e; var s; var out) => pk_twin_encode(e, s, samples, out)
                    delete stp
                    delete mp
                }
                delete std
                delete md
                fc_exact_lane_leg(t, "parakeet jfk", TowerFamily.parakeet, mx, stx, $(zb) { return <- mint_pk(path, false, zb, home) }) $(e; var s; var out) => pk_twin_encode(e, s, samples, out)
                delete stx
                delete st
                delete m
                delete mx
                gpu_tower_shutdown(TowerFamily.parakeet)
            }
        } else {
            t |> skip("no discrete GPU home in this build (dasVulkan, dasCUDA)")
        }
    }
}

//! a Parakeet-TDT v3 carrier of the GGUF set by its quant name (`F32`, `Q4_K_M`)
def private pk_gguf(quant : string) : string => path_join(models_dir(), "parakeet-tdt-0.6b-v3-{quant}.gguf")

//! a held kq-lane staging's seats zeroed from seat `first` up to seat `past` (the seats behind it kept): the quant bytes of every
//! K-quant and Q8_0 seat between them, the planes' ends where `past` is the seat count
def private poison_kq_seats(seats : array<int64>; first, past : int64; var kq : array<uint8>; var qblob : array<int8>) {
    let q8 = int64(int(KqFmt.q8))
    var kq_lo = long_length(kq)
    var kq_hi = long_length(kq)
    var q8_lo = long_length(qblob)
    var q8_hi = long_length(qblob)
    for (i in range64(first, long_length(seats) / TOWER_SEAT_WORDS)) {
        let at = seats[i * TOWER_SEAT_WORDS + 1l]
        if (seats[i * TOWER_SEAT_WORDS] == q8) {
            q8_lo = min(q8_lo, at)
            q8_hi = i >= past ? min(q8_hi, at) : q8_hi
        } else {
            kq_lo = min(kq_lo, at)
            kq_hi = i >= past ? min(kq_hi, at) : kq_hi
        }
    }
    for (i in range64(kq_lo, kq_hi)) {
        kq[i] = 0u8
    }
    for (i in range64(q8_lo, q8_hi)) {
        qblob[i] = int8(0)
    }
}

//! a quantized Parakeet GGUF staged on the kq lane and minted in memory; `zero_block` >= 0 poisons the blocks from it on
def private mint_pk_kq(path : string; zero_block : int64; dev : TowerDev = TowerDev.none) : ParakeetModel
    => <- mint_pk(path, false, zero_block, dev, true)

//! the encoder rows of `samples` through `m` on the CPU chain
def private pk_cpu_rows(m : ParakeetModel; samples : array<float>) : array<float> {
    var out : array<float>
    var st = ParakeetState()
    with_gpu_knobs() {
        with_dasllama_jobque_() {
            set_serving_gpu_tower(false, TowerFamily.parakeet)
            pk_twin_encode(m, st, samples, out)
        }
    }
    delete st
    return <- out
}

[test(description = "The F32 Parakeet GGUF stages the f32 ggml bin's planes and encodes jfk to the same rows, and the Q4_K_M file's kq lane, named k4, holds its CPU rows within 2.5x the q8 lane's distance from the dequantized chain with a zeroed half outside it"), arch(at = "../ARCHITECTURE_MEDIA_ASR.md#parakeet-gguf")]
def test_parakeet_gguf_carrier(tst : T?) {
    tst |> run("parakeet-TDT 0.6B v3: the F32 GGUF against the f32 ggml bin") @(t : T?) {
        let bin = path_join(whisper_dir(), "ggml-parakeet-tdt-0.6b-v3-f32.bin")
        let gguf = pk_gguf("F32")
        return if (!model_available(t, bin) || !model_available(t, gguf))
        let samples <- load_audio_16k_mono(asr_clip("jfk.wav"))
        if (empty(samples)) {
            t |> skip("jfk.wav not present")
            return
        }
        var mb <- mint_pk(bin, false, -1l)
        var mg <- mint_pk(gguf, false, -1l)
        t |> equal(mg.n_layer, mb.n_layer, "the block count")
        t |> equal(mg.n_vocab, mb.n_vocab, "the vocabulary without the blank")
        t |> equal(mg.blob.n, mb.blob.n, "the f32 blob's length")
        var inscope rb <- pk_cpu_rows(mb, samples)
        var inscope rg <- pk_cpu_rows(mg, samples)
        var got = TowerParityStats()
        if (tower_parity_stats(t, "parakeet gguf vs bin", rb, rg, got)) {
            to_log(LOG_INFO, "test_audio: parakeet F32 gguf vs f32 bin on jfk: rel_l2 {got.rel_l2}\n")
            t |> success(got.rel_l2 <= PK_GGUF_BIN_BAR, "the GGUF's encoder rows are the bin's ({got.rel_l2})")
        }
        var inscope loud <- [for (v in samples); v * 0.5]
        var inscope rl <- pk_cpu_rows(mb, loud)
        var ctl = TowerParityStats()
        if (long_length(rl) == long_length(rb) && tower_parity_stats(t, "parakeet gguf vs bin control", rl, rg, ctl)) {
            t |> success(is_finite(ctl.rel_l2) && ctl.rel_l2 > PK_GGUF_BIN_BAR, "the bin's rows of the clip at half volume land outside the bar ({ctl.rel_l2}) - the control")
        }
        delete mg
        delete mb
    }
    tst |> run("parakeet-TDT 0.6B v3 Q4_K_M: the kq lane on the CPU against the q8 lane, the dequantized f32 chain the reference") @(t : T?) {
        let path = pk_gguf("Q4_K_M")
        return if (!model_available(t, path))
        let samples <- load_audio_16k_mono(asr_clip("jfk.wav"))
        if (empty(samples)) {
            t |> skip("jfk.wav not present")
            return
        }
        var mx <- mint_pk(path, false, -1l)
        var mq <- mint_pk(path, true, -1l)
        var mk <- mint_pk_kq(path, -1l)
        t |> success(mk.kq_lane && !mk.q8 && !mx.kq_lane && mq.q8, "the kq lane's model beside the exact and the q8 lanes'")
        t |> equal(parakeet_exec_fmt(mk), "k4", "the lane's name: its one K-quant member's disk type")
        t |> success(mk.kq.n > 0l && mk.qblob.n > 0l, "K-quant seats beside Q8_0 ones: the attention outputs and the pointwise convs")
        var inscope ex <- pk_cpu_rows(mx, samples)
        var inscope q8 <- pk_cpu_rows(mq, samples)
        var inscope kq <- pk_cpu_rows(mk, samples)
        var q8_ex = TowerParityStats()
        var kq_ex = TowerParityStats()
        if (tower_parity_stats(t, "parakeet q8 vs exact", ex, q8, q8_ex) && tower_parity_stats(t, "parakeet kq vs exact", ex, kq, kq_ex)) {
            let bar = PK_KQ_CPU_SLACK * q8_ex.rel_l2 + TWIN_REL_L2_FLOOR
            to_log(LOG_INFO, "test_audio: parakeet Q4_K_M on jfk: rel_l2 from the dequantized chain - kq {kq_ex.rel_l2}, q8 {q8_ex.rel_l2}, the bar {bar}\n")
            t |> success(kq_ex.rel_l2 <= bar, "the kq lane's rows within {PK_KQ_CPU_SLACK}x the q8 lane's rel_l2 from the dequantized chain ({kq_ex.rel_l2} vs {q8_ex.rel_l2})")
            var mz <- mint_pk_kq(path, mk.n_layer / 2l)
            var inscope bad <- pk_cpu_rows(mz, samples)
            var bad_ex = TowerParityStats()
            if (tower_parity_stats(t, "parakeet poisoned kq vs exact", ex, bad, bad_ex)) {
                t |> success(is_finite(bad_ex.rel_l2) && bad_ex.rel_l2 > bar, "zeroed seats from the middle block on EXCEED the bar ({bad_ex.rel_l2} vs {bar})")
            }
            delete mz
        }
        delete mk
        delete mq
        delete mx
    }
}

let private PK_GGUF_BIN_BAR = 1e-4lf   //! the two carriers' rows apart: one model's f32 tensors, the GGUF's filterbank and window rebuilt from its header
let private PK_KQ_CPU_SLACK = 2.5lf    //! the CPU kq lane's rel_l2 over the q8 lane's, which it sits at about twice: a K-quant seat's Q8_K feed holds one scale a 256 elements where Q8_0's holds one a 32

//! the kq planner's refusal of the Q4_K_M carrier with `retype` applied, staged on the kq lane - "" where it staged
[cold_path]
def private pk_retyped_refusal(name : string; retype : table<string; int>) : string {
    return gguf_retyped_refusal(pk_gguf("Q4_K_M"), name, retype) $(p) {
        var inscope st <- stage_parakeet_kq_model(p)
    }
}

[test(description = "Over retyped copies of the Q4_K_M Parakeet carrier the kq planner refuses a Q5_1 seat, q in Q8_0 beside K-quant k and v, and one block's position linear in Q8_0, each by name, stages an unchanged copy, refuses a ggml bin, and the load tag is the kq lane's for a quantized GGUF asked for q8 un-pinned alone"), arch(at = "../ARCHITECTURE_MEDIA_ASR.md#parakeet-kq-seats")]
def test_parakeet_kq_refusals(tst : T?) {
    let path = pk_gguf("Q4_K_M")
    return if (!model_available(tst, path))
    tst |> run("the kq planner refuses what its lane cannot seat") @(t : T?) {
        let b0 = "enc.blocks.0."
        let seat = pk_retyped_refusal("pk_seat", { "{b0}ff1.linear1.weight" => GGML_TYPE_Q5_1 })
        t |> success(find(seat, "has no seat for") >= 0 && find(seat, "{b0}ff1.linear1.weight") >= 0, "a Q5_1 GEMM refused at its seat naming the tensor: '{seat}'")
        let qkv = pk_retyped_refusal("pk_qkv", { "{b0}attn.linear_q.weight" => GGML_TYPE_Q8_0 })
        t |> success(find(qkv, "attention q, k and v mix Q8_0 with a K-quant format") >= 0, "q in Q8_0 beside K-quant k and v refused: '{qkv}'")
        let pos = pk_retyped_refusal("pk_pos", { "enc.blocks.1.attn.linear_pos.weight" => GGML_TYPE_Q8_0 })
        t |> success(find(pos, "position linears mix Q8_0 with a K-quant format") >= 0 && find(pos, "block 1") >= 0, "one block's position linear in Q8_0 refused naming the block: '{pos}'")
        let nothing : table<string; int>
        t |> equal(pk_retyped_refusal("pk_clean", nothing), "", "the copy with nothing retyped stages (the control)")
    }
    tst |> run("the kq lane is a quantized GGUF's alone") @(t : T?) {
        reset_parakeet_q8()
        t |> equal(parakeet_load_tag(path, true), PARAKEET_KQ_TAG, "the Q4_K_M GGUF asked for q8, un-pinned: the kq lane")
        t |> equal(parakeet_load_tag(path, false), "parakeet-f32", "asked for the exact lane: the f32 lane")
        set_parakeet_q8(true)
        t |> equal(parakeet_load_tag(path, true), "parakeet-q8", "pinned q8: the q8 lane, the carrier dequantized")
        reset_parakeet_q8()
        let f32 = pk_gguf("F32")
        if (model_available(t, f32)) {
            t |> equal(parakeet_load_tag(f32, true), "parakeet-q8", "the F32 GGUF asked for q8: the q8 lane (the control)")
        }
        let bin = path_join(whisper_dir(), "ggml-parakeet-tdt-0.6b-v3-f32.bin")
        if (model_available(t, bin)) {
            let why = panic_text() $ {
                var inscope st <- stage_parakeet_kq_model(bin)
            }
            t |> success(find(why, "is a ggml bin") >= 0, "the kq lane refuses a ggml bin: '{why}'")
        }
    }
}

//! jfk-style clip `samples` transcribed through `m` on the CPU chain and the CPU decode
def private pk_cpu_text(m : ParakeetModel; samples : array<float>) : string {
    var text = ""
    var st = ParakeetState()
    with_gpu_knobs() {
        with_dasllama_jobque_() {
            set_serving_gpu_tower(false, TowerFamily.parakeet)
            parakeet_log_mel(samples, m, st)
            let tt = parakeet_encode(m, st)
            parakeet_tdt_decode(m, st, tt)
            text = parakeet_detokenize(m, st.toks)
        }
    }
    delete st
    return text
}

//! the loader's refusal of the Q4_K_M carrier with byte `v` written at offset `at` of header key `key`'s value - "" where it staged
[cold_path]
def private pk_header_refusal(name, key : string; at, v : int) : string {
    let dst = path_join(test_temp_dir(), "{name}.gguf")
    let nothing : table<string; int>
    gguf_retyped_copy(pk_gguf("Q4_K_M"), dst, nothing, [(key = key, at = at, v = v)])
    let why = panic_text() $ {
        var inscope st <- stage_parakeet_kq_model(dst)
    }
    remove(dst)
    return why
}

//! the kq lane's name over the Q4_K_M carrier with `retype` applied, staged
[cold_path]
def private pk_retyped_lane_name(name : string; retype : table<string; int>) : string {
    let dst = path_join(test_temp_dir(), "{name}.gguf")
    gguf_retyped_copy(pk_gguf("Q4_K_M"), dst, retype)
    var inscope st <- stage_parakeet_kq_model(dst)
    remove(dst)
    return parakeet_exec_fmt(st.e)
}

[test(description = "The Parakeet GGUF loader refuses a foreign architecture, linear biases, input scaling, an FF off four times the width, a token count off the predictor's and a front end off 16 kHz by name; the F32 GGUF transcribes jfk as the f32 bin does and the Q4_K_M kq lane holds jfk's word floor; a Q6_K seat names the lane kq, a BF16 GEMM stages, the Q8_0 file names it q8"), arch(at = "../ARCHITECTURE_MEDIA_ASR.md#parakeet-gguf")]
def test_parakeet_gguf_loads(tst : T?) {
    let path = pk_gguf("Q4_K_M")
    return if (!model_available(tst, path))
    tst |> run("the header's refusals") @(t : T?) {
        let enc = "stt.parakeet.encoder"
        t |> success(find(pk_header_refusal("pk_bias", "{enc}.use_bias", 0, 1), "{enc}.use_bias") >= 0, "linear biases refused naming the key")
        t |> success(find(pk_header_refusal("pk_xs", "{enc}.xscaling", 0, 1), "{enc}.xscaling") >= 0, "input scaling refused naming the key")
        t |> success(find(pk_header_refusal("pk_dff", "{enc}.d_model", 0, 1), "an FF four times the model width") >= 0, "an FF off four times the width refused")
        t |> success(find(pk_header_refusal("pk_tok", "stt.parakeet.predictor.vocab", 0, 7), "the blank is the last of them") >= 0, "a token count off the predictor's refused")
        t |> success(find(pk_header_refusal("pk_rate", "stt.frontend.sample_rate", 0, 129), "Hz front end") >= 0, "a front end off 16 kHz refused")
        t |> success(find(pk_header_refusal("pk_hop", "stt.frontend.hop_length", 0, 161), "Hz front end") >= 0, "a hop off 160 refused")
        t |> equal(pk_header_refusal("pk_same", "stt.frontend.hop_length", 0, 160), "", "the header rewritten with its own byte stages (the control)")
        let ga = path_join(models_dir(), "gigaam-v3-e2e-rnnt-f16.gguf")
        if (model_available(t, ga)) {
            let why = panic_text() $ {
                var inscope st <- stage_parakeet_model(ga, false)
            }
            t |> success(find(why, "parakeet") >= 0 && !empty(why), "a GigaAM GGUF refused by the Parakeet reader: '{why}'")
        }
    }
    tst |> run("the lane's name follows its seats") @(t : T?) {
        let b0 = "enc.blocks.0."
        t |> equal(pk_retyped_lane_name("pk_mix", { "{b0}ff1.linear1.weight" => GGML_TYPE_Q6_K }), "kq", "a Q6_K seat beside the Q4_K ones: the members mix")
        t |> equal(pk_retyped_lane_name("pk_bf16", { "{b0}ff1.linear1.weight" => GGML_TYPE_BF16 }), "k4", "a BF16 GEMM takes a Q8_0 seat: the lane keeps its one member's name")
        let nothing : table<string; int>
        t |> equal(pk_retyped_lane_name("pk_plain", nothing), "k4", "the copy with nothing retyped (the control)")
        let q8 = pk_gguf("Q8_0")
        if (model_available(t, q8)) {
            var inscope st <- stage_parakeet_kq_model(q8)
            t |> equal(parakeet_exec_fmt(st.e), "q8", "the Q8_0 file: every seat Q8_0")
            t |> equal(parakeet_load_tag(pk_gguf("F16"), true) == PARAKEET_KQ_TAG, false, "an F16 GGUF is no kq carrier") if (model_available(t, pk_gguf("F16")))
        }
    }
    tst |> run("the decoder half: transcripts on jfk") @(t : T?) {
        let samples <- load_audio_16k_mono(asr_clip("jfk.wav"))
        if (empty(samples)) {
            t |> skip("jfk.wav not present")
            return
        }
        var mk <- mint_pk_kq(path, -1l)
        let kq_text = pk_cpu_text(mk, samples)
        to_log(LOG_INFO, "test_audio: parakeet Q4_K_M kq jfk: {kq_text}\n")
        jfk_words_floor(t, "parakeet Q4_K_M kq lane", kq_text)
        delete mk
        let bin = path_join(whisper_dir(), "ggml-parakeet-tdt-0.6b-v3-f32.bin")
        let f32 = pk_gguf("F32")
        if (model_available(t, bin) && model_available(t, f32)) {
            var mb <- mint_pk(bin, false, -1l)
            var mg <- mint_pk(f32, false, -1l)
            let bin_text = pk_cpu_text(mb, samples)
            t |> equal(pk_cpu_text(mg, samples), bin_text, "the F32 GGUF's transcript is the f32 bin's: the predictor, the joint and the durations read alike")
            jfk_words_floor(t, "parakeet f32 bin", bin_text)
            delete mg
            delete mb
        }
    }
}

[test(description = "The ASR facade loads the Q4_K_M Parakeet GGUF alone on the kq lane's CPU image, names the lane k4, holds jfk's word floor, and loads the lane's image back by path to the same transcript"), arch(at = "../ARCHITECTURE_MEDIA_ASR.md#parakeet-kq-seats")]
def test_parakeet_gguf_facade(tst : T?) {
    let path = pk_gguf("Q4_K_M")
    return if (!model_available(tst, path))
    tst |> run("the facade loads the GGUF alone, and its kq image back by path") @(t : T?) {
        let samples <- load_audio_16k_mono(asr_clip("jfk.wav"))
        if (empty(samples)) {
            t |> skip("jfk.wav not present")
            return
        }
        t |> success(!asr_needs_mmproj(path), "a Parakeet GGUF is a one-file carrier")
        with_gpu_knobs() {
            set_serving_gpu_tower(false, TowerFamily.parakeet)
            reset_parakeet_q8()
            var inscope am <- load_asr_model(path)
            t |> success(am.kind == AsrKind.parakeet && am.pk.kq_lane && am.pk.dev_backend == TowerDev.none, "the GGUF routes to parakeet on the kq lane's CPU image")
            t |> equal(asr_exec_fmt(am), "k4", "the facade names the lane by its member")
            let text = pk_cpu_text(am.pk, samples)
            jfk_words_floor(t, "parakeet Q4_K_M through the facade", text)
            let img = "{path}.{PARAKEET_KQ_TAG}.dlim"
            var fs : FStat
            if (stat(img, fs) && fs.is_reg) {
                var inscope im <- load_asr_model(img)
                t |> success(im.kind == AsrKind.parakeet && im.pk.kq_lane, "the image routes by its tag to the kq lane")
                t |> equal(pk_cpu_text(im.pk, samples), text, "the image by path transcribes as the load that minted it")
            } else {
                t |> skip("no prepared image beside the carrier (the image rail is off)")
            }
        }
    }
}

[test(description = "The Q4_K_M Parakeet GGUF on the discrete home serves its front and every block from the kq lane's device planes within the three-way bar on jfk, a k4 tile counted, and zeroed seats exceed the bar"), arch(at = "../ARCHITECTURE_MEDIA_ASR.md#parakeet-kq-seats")]
def test_parakeet_kq_discrete_twin(tst : T?) {
    tst |> run("parakeet-TDT 0.6B v3 Q4_K_M: the discrete home's chain on the kq lane vs the CPU kq chain on jfk, the dequantized chain the reference") @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan) || typeinfo builtin_module_exists(cuda)) {
            let path = pk_gguf("Q4_K_M")
            jfk_twin_cell(t, path, false, TowerFamily.parakeet) $(samples) {
                let home = serving_tower_dev(TowerFamily.parakeet)
                var mx <- mint_pk(path, false, -1l)
                var m <- mint_pk_kq(path, -1l)
                var md <- mint_pk_kq(path, -1l, home)
                t |> success(m.kq_lane && md.kq_lane && md.dev_backend == home, "the kq lane's CPU image, its {home} image beside")
                t |> equal(parakeet_exec_fmt(md), "k4", "the {home} image's lane is named by its one K-quant member's disk type")
                t |> equal(md.kq.n + md.ks.n + md.qblob.n, 0l, "the {home} image carries the device planes alone")
                var stx = ParakeetState()
                var st = ParakeetState()
                var std = ParakeetState()
                let k4_0 = kq_tile_count("k4", TowerFamily.parakeet)
                let spec = TwinSpec(family = "parakeet", tag = "parakeet kq jfk", n_layer = m.n_layer, metric = TwinMetric.rel_l2, seat_family = TowerFamily.parakeet)
                let r <- twin_three_way(t, spec, m, st, md, std, mx, stx, $(e; var s; var out) => pk_twin_encode(e, s, samples, out)) $(legs) {
                    t |> equal(legs.n_dev, legs.n_exact, "parakeet kq jfk: the device chain's frame count")
                    t |> equal(legs.dev.encodes - legs.pre.encodes, 1l, "parakeet kq jfk: the device served the encode")
                    t |> equal(legs.dev.blocks - legs.pre.blocks, m.n_layer, "parakeet kq jfk: every block ran on the driver")
                    front_served(t, "parakeet kq jfk", legs.dev.convs - legs.pre.convs, 1l, legs.dev.declines - legs.cpu.declines)
                }
                t |> success(kq_tile_count("k4", TowerFamily.parakeet) > k4_0, "parakeet kq jfk: a k4 tile served the K-quant seats (the census counter rose)")
                if (r.bar > 0.0lf) {
                    var mp <- mint_pk_kq(path, m.n_layer / 2l, home)
                    var stp = ParakeetState()
                    twin_poison(t, r, m.n_layer / 2l, mp, stp) $(e; var s; var out) => pk_twin_encode(e, s, samples, out)
                    delete stp
                    delete mp
                }
                delete std
                delete st
                delete stx
                delete md
                delete m
                delete mx
                gpu_tower_shutdown(TowerFamily.parakeet)
            }
        } else {
            t |> skip("no discrete GPU home in this build (dasVulkan, dasCUDA)")
        }
    }
}

[test(description = "The Q4_K_M Parakeet GGUF on the Metal FastConformer chain serves its front and every block from the kq lane's slabs within the three-way bar on jfk with no quant_mode decline, a K-quant mul_mm stamp counted, and zeroed seats exceed the bar"), arch(at = "../ARCHITECTURE_MEDIA_ASR.md#parakeet-kq-seats")]
def test_parakeet_kq_metal_twin(tst : T?) {
    tst |> run("parakeet-TDT 0.6B v3 Q4_K_M: the Metal chain on the kq lane vs the CPU kq chain on jfk, the dequantized chain the reference") @(t : T?) {
        static_if (typeinfo builtin_module_exists(das_metal)) {
            let path = pk_gguf("Q4_K_M")
            return if (!family_on(t, "parakeet"))
            metal_jfk_twin_cell(t, path, false, TowerFamily.parakeet) $(samples) {
                var mx <- mint_pk(path, false, -1l)
                var m <- mint_pk_kq(path, -1l)
                var md <- mint_pk_kq(path, -1l, TowerDev.metal)
                t |> success(m.kq_lane && md.kq_lane && md.dev_backend == TowerDev.metal, "the kq lane's CPU image, its Metal image beside")
                t |> equal(parakeet_exec_fmt(md), "k4", "the Metal image's lane is named by its one K-quant member's disk type")
                var stx = ParakeetState()
                var st = ParakeetState()
                var std = ParakeetState()
                let mm0 = kq_mm_count()
                metal_q8_twin(t, "parakeet kq metal jfk", TowerFamily.parakeet, m, st, md, std, mx, stx, $ { return <- mint_pk_kq(path, m.n_layer / 2l, TowerDev.metal) }) $(e; var s; var out) => pk_twin_encode(e, s, samples, out)
                t |> success(kq_mm_count() > mm0, "parakeet kq metal jfk: a K-quant mul_mm stamp served the K-quant seats (the census counter rose)")
                delete std
                delete st
                delete stx
                delete md
                delete m
                delete mx
                metal_tower_shutdown()
            }
        } else {
            t |> skip("no Metal build")
        }
    }
}

//! `mel_spectrum_rows` on a synthetic DFT table: the ln of each filter's sum over the spectrum `magnitude` asks for, in double
def private spectrum_ref(xw, tw, filt : array<float>; n_len, n_fft, n_bins, n_mel : int64; magnitude : bool) : array<float> {
    var out : array<float>
    out |> reserve(n_len * n_mel)
    for (i in range64(n_len)) {
        for (jm in range64(n_mel)) {
            var sum = 0.0lf
            for (k in range64(n_bins)) {
                var re = 0.0lf
                var im = 0.0lf
                for (n in range64(n_fft)) {
                    re += double(xw[i * n_fft + n]) * double(tw[n * n_bins * 2l + 2l * k])
                    im += double(xw[i * n_fft + n]) * double(tw[n * n_bins * 2l + 2l * k + 1l])
                }
                let p = re * re + im * im
                sum += double(filt[jm * n_bins + k]) * (magnitude ? sqrt(p) : p)
            }
            out |> push(float(log(max(sum, 1e-10lf))))
        }
    }
    return <- out
}

[test(description = "mel_spectrum_rows sums the magnitude or the power of the spectrum into the filters to within 1e-5 of a double-precision DFT reference, and the magnitude form must mismatch the power reference")]
def test_mel_spectrum_magnitude(t : T?) {
    t |> run("the magnitude form sums |X| into the filters, the power form |X|^2, each against a double DFT") @(t : T?) {
        let n_len = 5l
        let n_fft = 16l
        let n_bins = 9l
        let n_mel = 4l
        var inscope xw <- [for (i in range64(n_len * n_fft)); sin(0.37 * float(i)) * 3.0]
        var inscope tw <- [for (i in range64(n_fft * n_bins * 2l)); cos(0.11 * float(i * i % 97))]
        var inscope filt <- [for (i in range64(n_mel * n_bins)); 0.1 + 0.05 * float(i % 7)]
        var inscope reim, mel_mag, mel_pow : array<float>
        let fp = unsafe(addr<float const?>(filt[0]))
        let tp = unsafe(addr<float const?>(tw[0]))
        mel_spectrum_rows(xw, tp, fp, n_len, n_fft, n_bins, n_mel, MelForm(magnitude = true, log = MelLog.ln_max, floor = 1e-10lf), reim, mel_mag)
        mel_spectrum_rows(xw, tp, fp, n_len, n_fft, n_bins, n_mel, MelForm(log = MelLog.ln_max, floor = 1e-10lf), reim, mel_pow)
        var inscope want_mag <- spectrum_ref(xw, tw, filt, n_len, n_fft, n_bins, n_mel, true)
        var inscope want_pow <- spectrum_ref(xw, tw, filt, n_len, n_fft, n_bins, n_mel, false)
        let bar = Bar(rel = 1e-5, abs_floor = 1e-5)
        check_rows(t, "the magnitude form", mel_mag, want_mag, bar, int(n_mel))
        check_rows(t, "the power form", mel_pow, want_pow, bar, int(n_mel))
        check_exceeds(t, "the magnitude form against the power reference (MUST mismatch)", mel_mag, want_pow, bar)
    }
}

//! max |device - CPU| over the CPU rows' largest magnitude: the device GEMMs stage W and X through halves, about 1e-3 of the rows' scale over sixteen blocks, so 1e-2 is that floor with headroom and a poisoned clip (x3, the log-mel moved by ln 9) reads at the rows' own scale.
let private GA_TWIN_BAR = 1e-2

//! the device log-mel against the CPU's, each element within 1e-5 of its magnitude floored at 1: the device frames are the
//! CPU's bit for bit and both sides sum in float, so the log reads within an ulp of the clip's loudest value
let private GA_MEL_BAR = 1e-5

struct private GaLeg {
    n : int64
    x : array<float>
    ids : array<int>
    text : string
}

//! one GigaAM log-mel, encode and greedy decode of `samples` through the span encode on the current tower knobs (the device mel
//! where the hook serves); `scale` scales the samples ahead of it
def private ga_leg(m : GigaamModel; samples : array<float>; scale : float = 1.0) : GaLeg {
    var st = GigaamState()
    var r = GaLeg()
    var inscope clip := samples
    for (v in clip) {
        v *= scale
    }
    r.n = gigaam_encode_span(clip, 0l, long_length(clip), m, st)
    r.x <- copy_span(st.x, 0l, r.n * m.d_model)
    gigaam_decode(m, st, r.n)
    r.ids <- [for (k in st.toks); k.id]
    r.text = gigaam_detokenize(m, st.toks)
    delete st
    return <- r
}

//! the device rows' bar: within ``GA_TWIN_BAR`` of the CPU rows' largest magnitude, the poisoned expected element that
//! magnitude off
def private ga_bar() : Bar => Bar(kind = BarKind.rel_of_max, rel = GA_TWIN_BAR, poison = 1.0, poison_of_max = true)

//! each element's bar: ``GA_TWIN_BAR`` of its own frame's largest magnitude in `want`, frames `d` wide
def private ga_frame_bars(want : array<float>; d : int64) : array<float> {
    var bars : array<float>
    bars |> reserve(length(want))
    for (f in range64(long_length(want) / d)) {
        var peak = 0.0
        for (i in range64(f * d, (f + 1l) * d)) {
            peak = max(peak, abs(want[i]))
        }
        for (_ in range64(d)) {
            bars |> push(GA_TWIN_BAR * peak)
        }
    }
    return <- bars
}

//! the device rows against the CPU's at the twin bar twice: over the rows' largest magnitude, and frame by frame over each frame's
//! own, so a quiet frame's error cannot hide under a loud frame's scale
def private ga_check_rows(t : T?; tag : string; got, want : array<float>; d : int64) {
    check_rows(t, tag, got, want, ga_bar(), int(d))
    var inscope bars <- ga_frame_bars(want, d)
    check(t, "{tag}, per frame", got, want, Bar(kind = BarKind.per_element, poison = 1.0, poison_of_max = true), Shape(row_width = int(d)), bars)
}

//! the device log-mel alone (`metal_gigaam_mel`, the encode chain's mel dispatches) against `gigaam_log_mel` at ``GA_MEL_BAR``,
//! the frame count and the counter beside it, the bar's poisoned element the control
[unused_argument(t, m, samples, clip)]   // off-Apple the gated body is empty
def private ga_mel_twin_cell(t : T?; m : GigaamModel; samples : array<float>; clip : string) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        var st = GigaamState()
        gigaam_log_mel(samples, m, st)
        var inscope dev : array<float>
        let mel0 = metal_tower_ga_mels()
        let n = metal_gigaam_mel(m, samples, dev)
        t |> equal(n, st.n_len, "{clip}: the device mel's frame count")
        t |> equal(metal_tower_ga_mels() - mel0, 1l, "{clip}: the mel seat ran on the device")
        check_rows(t, "gigaam {clip} metal: the device log-mel tracks the CPU's", dev, st.mel, Bar(rel = GA_MEL_BAR, poison_scaled = true), int(m.n_mel))
        var inscope loud <- [for (v in samples); v * 1.1]
        var inscope dev_loud : array<float>
        metal_gigaam_mel(m, loud, dev_loud)
        check_exceeds(t, "gigaam {clip} metal: the control - the clip at 1.1x on the device side off the CPU's log-mel", dev_loud, st.mel,
            Bar(rel = GA_MEL_BAR))
        delete st
    }
}

//! one clip's legs: the CPU chain over the CPU image `mc`, the device chain over the Metal-flavored `m` (its halfword planes on
//! the half tensor GEMM, the one route the flavor pick takes them on), a poisoned clip; the encoder's frame count. The cell pins the tower and its f16 twin route at its head and puts every knob back as
//! it found them.
[cold_path, unused_argument(t, m, mc, samples, clip)]   // off-Apple the gated body is empty
def private ga_clip_cell(t : T?; m, mc : GigaamModel; samples : array<float>; clip : string) : int64 {
    var frames = 0l
    with_gpu_knobs() {
        frames = ga_clip_legs(t, m, mc, samples, clip)
    }
    return frames
}

[unused_argument(t, m, mc, samples, clip)]   // off-Apple the gated body is empty
def private ga_clip_legs(t : T?; m, mc : GigaamModel; samples : array<float>; clip : string) : int64 {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let f16_found = get_metal_tower_f16()
        set_metal_tower_f16(true)
        set_metal_tower(false)
        let c0 = metal_tower_stats()
        var inscope cpu <- ga_leg(mc, samples)
        let c1 = metal_tower_stats()
        t |> equal(c1.encodes - c0.encodes, 0l, "{clip}: the tower-off leg never dispatched")
        t |> success(c1.declines - c0.declines >= 1l, "{clip}: the tower-off leg counted the knob decline")
        set_metal_tower(true)
        ga_mel_twin_cell(t, m, samples, clip)
        let g0 = metal_tower_stats()
        let f0 = metal_tower_f16_encodes()
        let mel0 = metal_tower_ga_mels()
        let kq0 = kq_mm_count()
        var inscope dev <- ga_leg(m, samples)
        let g1 = metal_tower_stats()
        let twin = metal_tower_f16_encodes() - f0
        t |> equal(kq_mm_count() - kq0, 0l, "{clip}: the exact lane dispatches no K-quant mul_mm stamp (the kq lane counter's control)")
        t |> equal(g1.encodes - g0.encodes, 1l, "{clip}: the device served the encode")
        t |> equal(metal_tower_ga_mels() - mel0, 1l, "{clip}: the log-mel ran on the device, inside the encode's command buffer")
        t |> equal(g1.convs - g0.convs, 1l, "{clip}: the conv front ran on the device")
        t |> equal(g1.blocks - g0.blocks, m.n_layer, "{clip}: every block ran on the driver")
        t |> equal(dev.n, cpu.n, "{clip}: the frame count")
        ga_check_rows(t, "gigaam {clip} metal ({twin > 0l ? "twin" : "f32 tiles"}): the device rows track the CPU's", dev.x, cpu.x, m.d_model)
        t |> equal(dev.text, cpu.text, "{clip}: the transcript")
        to_log(LOG_INFO, "test_audio: gigaam {clip} cpu: {cpu.text}\n")
        to_log(LOG_INFO, "test_audio: gigaam {clip} gpu: {dev.text}\n")
        var inscope bad <- ga_leg(m, samples, 3.0)
        check_exceeds(t, "gigaam {clip} metal: a poisoned clip reds the bar", bad.x, cpu.x, ga_bar())
        set_metal_tower_f16(f16_found)
        return cpu.n
    } else {
        return 0l
    }
}

//! the head of `samples` whose encoder frame count is the largest multiple of 64 the clip holds - the rotary chain's attention
//! with no pad rows; empty where the clip holds under 64 frames
def private ga_lattice_clip(m : GigaamModel; samples : array<float>) : array<float> {
    let frames = $(n : int64) => gigaam_conv_out(gigaam_conv_out(n >= m.n_fft ? (n - m.n_fft) / m.hop + 1l : 0l, m.subs_kernel), m.subs_kernel)
    var n = long_length(samples)
    let target = invoke(frames, n) / 64l * 64l
    while (target > 0l && invoke(frames, n) > target) {
        n -= m.hop
    }
    return <- copy_span(samples, 0l, target > 0l ? n : 0l)
}

//! the q8 lane on the Metal chain, off the q8 slab, against the CPU q8 chain with the exact chain `mx` the reference: the device
//! rows' rel_l2 from the exact chain within ``TWIN_SLACK_VK`` x the CPU q8 chain's own plus ``TWIN_REL_L2_FLOOR``, the token ids
//! the CPU q8 chain's, the encode, its log-mel and every block on the device with no ``quant_mode`` decline; the control a q8
//! model with blocks from the middle on zeroed, through the device chain, past the bar
[cold_path, unused_argument(t, path, mx, samples, clip)]   // off-Apple the gated body is empty
def private ga_q8_cell(t : T?; path : string; mx : GigaamModel; samples : array<float>; clip : string) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        var mq <- mint_ga(path, true, -1l)
        var mqd <- mint_ga(path, true, -1l, TowerDev.metal)
        var mp <- mint_ga(path, true, mq.n_layer / 2l, TowerDev.metal)
        let tag = "gigaam {clip} q8 lane"
        with_gpu_knobs() {
            set_metal_tower(false)
            var inscope ex <- ga_leg(mx, samples)
            var inscope cpu <- ga_leg(mq, samples)
            set_metal_tower(true)
            let q0 = metal_tower_stats()
            let mel0 = metal_tower_ga_mels()
            let qd0 = metal_tower_declines()?["quant_mode"] ?? 0l
            var inscope dev <- ga_leg(mqd, samples)
            let q1 = metal_tower_stats()
            t |> equal(q1.encodes - q0.encodes, 1l, "{tag}: the device served the encode")
            t |> equal(q1.blocks - q0.blocks, mq.n_layer, "{tag}: every block ran on the driver")
            t |> equal(metal_tower_ga_mels() - mel0, 1l, "{tag}: the log-mel ran on the device")
            t |> equal((metal_tower_declines()?["quant_mode"] ?? 0l) - qd0, 0l, "{tag}: no quant_mode decline")
            t |> equal(dev.n, cpu.n, "{tag}: the frame count")
            t |> equal(dev.ids, cpu.ids, "{tag}: the token ids are the CPU q8 chain's")
            to_log(LOG_INFO, "test_audio: {tag} cpu: {cpu.text}\n")
            to_log(LOG_INFO, "test_audio: {tag} gpu: {dev.text}\n")
            var q8_ex = TowerParityStats()
            var dev_ex = TowerParityStats()
            if (tower_parity_stats(t, "{tag} cpu q8 vs exact", ex.x, cpu.x, q8_ex) && tower_parity_stats(t, "{tag} metal vs exact", ex.x, dev.x, dev_ex)) {
                let bar = double(TWIN_SLACK_VK) * q8_ex.rel_l2 + TWIN_REL_L2_FLOOR
                to_log(LOG_INFO, "test_audio: {tag}: rel_l2 from the exact chain - cpu q8 {q8_ex.rel_l2}, metal {dev_ex.rel_l2}, the bar {bar}\n")
                t |> success(dev_ex.rel_l2 <= bar, "{tag}: the device rows within {TWIN_SLACK_VK}x the CPU q8 chain's rel_l2 from the exact chain ({dev_ex.rel_l2} vs {q8_ex.rel_l2})")
                var inscope bad <- ga_leg(mp, samples)
                var bad_ex = TowerParityStats()
                if (tower_parity_stats(t, "{tag} poisoned metal vs exact", ex.x, bad.x, bad_ex)) {
                    t |> success(is_finite(bad_ex.rel_l2) && bad_ex.rel_l2 > bar, "{tag}: zeroed blocks through the device chain EXCEED the bar ({bad_ex.rel_l2} vs {bar})")
                }
            }
        }
        delete mp
        delete mqd
        delete mq
    }
}

//! the device's dense K-quant mul_mm dispatches, every stamp and form: on the Q4_K carrier the kq lane's K-quant seats alone move them
def private kq_mm_count() : int64 {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        var n = 0l
        var inscope cov <- metal_kernel_coverage()
        for (k, v in keys(cov), values(cov)) {
            if (k |> ends_with("_metal_kq_mulmm_t") || k |> ends_with("_metal_kq_mulmm_db") || k |> ends_with("_metal_kq_mulmm_k6_t") || k |> ends_with("_metal_kq_mulmm_iq4xs_t")
                    || k |> ends_with("_metal_kq_mulmm") || k |> ends_with("_metal_kq_mulmm_sk")) {
                n += v
            }
        }
        return n
    } else {
        return 0l
    }
}

//! the kq lane on the Metal chain against the CPU kq chain, the exact chain `mx` the reference: no `kq_lane` or `quant_mode` decline, and a K-quant mul_mm stamp counted - "a kernel counter ... that no other plane of that file moves" (REVIEW_KQ_FORMATS.md)
[cold_path, unused_argument(t, kq_path, mx, samples, clip, fmt), arch(at = "../ARCHITECTURE_GPU_TOWER_FC.md#tower-fc-kq")]   // off-Apple the gated body is empty
def private ga_kq_cell(t : T?; kq_path : string; mx : GigaamModel; samples : array<float>; clip, fmt : string) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        var mk <- mint_ga(kq_path, false, -1l)
        var mkd <- mint_ga(kq_path, false, -1l, TowerDev.metal)
        let tag = "gigaam {clip} kq lane {gigaam_kq_members(mk)}"
        t |> success(mk.kq_lane, "{tag}: the carrier serves as its own planes")
        t |> equal(gigaam_exec_fmt(mk), fmt, "{tag}: the lane's name - its one K-quant member's disk type, or `kq` where the members mix")
        with_gpu_knobs() {
            set_metal_tower(false)
            var inscope ex <- ga_leg(mx, samples)
            var inscope cpu <- ga_leg(mk, samples)
            set_metal_tower(true)
            let q0 = metal_tower_stats()
            let mel0 = metal_tower_ga_mels()
            let qd0 = metal_tower_declines()?["quant_mode"] ?? 0l
            let mm0 = kq_mm_count()
            var inscope dev <- ga_leg(mkd, samples)
            let q1 = metal_tower_stats()
            t |> equal(q1.encodes - q0.encodes, 1l, "{tag}: the device served the encode")
            t |> equal(q1.blocks - q0.blocks, mk.n_layer, "{tag}: every block ran on the driver")
            t |> equal(metal_tower_ga_mels() - mel0, 1l, "{tag}: the log-mel ran on the device")
            t |> equal((metal_tower_declines()?["quant_mode"] ?? 0l) - qd0, 0l, "{tag}: no quant_mode decline")
            t |> success(kq_mm_count() > mm0, "{tag}: a K-quant mul_mm stamp served the K-quant seats (the census counter rose)")
            t |> equal(dev.n, cpu.n, "{tag}: the frame count")
            t |> equal(dev.ids, cpu.ids, "{tag}: the token ids are the CPU kq chain's")
            to_log(LOG_INFO, "test_audio: {tag} cpu: {cpu.text}\n")
            to_log(LOG_INFO, "test_audio: {tag} gpu: {dev.text}\n")
            var kq_ex = TowerParityStats()
            var dev_ex = TowerParityStats()
            if (tower_parity_stats(t, "{tag} cpu kq vs exact", ex.x, cpu.x, kq_ex) && tower_parity_stats(t, "{tag} metal vs exact", ex.x, dev.x, dev_ex)) {
                let bar = double(TWIN_SLACK_VK) * kq_ex.rel_l2 + TWIN_REL_L2_FLOOR
                to_log(LOG_INFO, "test_audio: {tag}: rel_l2 from the exact chain - cpu kq {kq_ex.rel_l2}, metal {dev_ex.rel_l2}, the bar {bar}\n")
                t |> success(dev_ex.rel_l2 <= bar, "{tag}: the device rows within {TWIN_SLACK_VK}x the CPU kq chain's rel_l2 from the exact chain ({dev_ex.rel_l2} vs {kq_ex.rel_l2})")
                ga_kq_poison_leg(t, tag, kq_path, mk.n_layer, TowerDev.metal, ex.x, samples, bar)
            }
        }
        delete mkd
        delete mk
    }
}

//! the kq lane's poisoned control: the carrier at `kq_path` minted `dev`-flavored with its block GEMMs zeroed from the middle
//! block on, through the device chain, lands outside `bar` of the exact chain's rows `exact`
[cold_path]
def private ga_kq_poison_leg(t : T?; tag, kq_path : string; n_layer : int64; dev : TowerDev; exact : array<float>; samples : array<float>; bar : double) {
    var inscope mz <- mint_ga(kq_path, false, n_layer / 2l, dev)
    t |> success(mz.kq_lane && mz.dev_backend == dev, "{tag}: the poisoned carrier is a {dev}-flavored kq lane image")
    var inscope bad <- ga_leg(mz, samples)
    var bad_ex = TowerParityStats()
    if (tower_parity_stats(t, "{tag} poisoned {dev} vs exact", exact, bad.x, bad_ex)) {
        t |> success(is_finite(bad_ex.rel_l2) && bad_ex.rel_l2 > bar, "{tag}: the control - its GEMMs zeroed from block {n_layer / 2l} on, the device rows land outside the bar ({bad_ex.rel_l2} vs {bar})")
    }
}

//! the GigaAM device seats' side arms on one clip: the mel seat's declines (the knob, a clip under one window, a scratch past the largest buffer), the encode off an uploaded mel, the held rows' key, and a scratch the tower cannot take refusing the CPU chain; `m` Metal-flavored, `mc` its CPU image, the knobs put back
[cold_path, unused_argument(t, m, mc, samples)]   // off-Apple the gated body is empty
def private ga_seat_arms_cell(t : T?; m, mc : GigaamModel; samples : array<float>) {
    with_gpu_knobs() {
        ga_seat_arms_legs(t, m, mc, samples)
    }
}

[unused_argument(t, m, mc, samples)]   // off-Apple the gated body is empty
def private ga_seat_arms_legs(t : T?; m, mc : GigaamModel; samples : array<float>) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        set_metal_tower(false)
        var inscope cpu <- ga_leg(mc, samples)
        set_metal_tower(true)
        var inscope dev : array<float>
        let mel0 = metal_tower_ga_mels()
        set_metal_tower(false)
        t |> equal(metal_gigaam_mel(m, samples, dev), -1l, "mel seat: the knob off declines")
        set_metal_tower(true)
        var inscope tiny : array<float>
        tiny |> resize(int(m.n_fft) - 1)
        t |> equal(metal_gigaam_mel(m, tiny, dev), -1l, "mel seat: a clip under one window declines")
        let take_was = set_metal_take_max_probe_(512ul)
        t |> equal(metal_gigaam_mel(m, samples, dev), -1l, "mel seat: a scratch past the device's largest buffer declines")
        set_metal_take_max_probe_(take_was)
        t |> equal(metal_tower_ga_mels() - mel0, 0l, "mel seat: no declined call ran a mel")
        var st = GigaamState()
        gigaam_log_mel(samples, m, st)
        let e0 = metal_tower_stats()
        let n = gigaam_encode(m, st)
        let e1 = metal_tower_stats()
        t |> equal(e1.encodes - e0.encodes, 1l, "uploaded mel: the encode served on the device")
        t |> equal(e1.convs - e0.convs, 1l, "uploaded mel: the conv front ran on the device")
        t |> equal(metal_tower_ga_mels() - mel0, 0l, "uploaded mel: no mel ran on the device")
        t |> equal(n, cpu.n, "uploaded mel: the frame count")
        var inscope rows <- copy_span(st.x, 0l, n * m.d_model)
        ga_check_rows(t, "gigaam metal, uploaded mel: the device rows track the CPU's", rows, cpu.x, m.d_model)
        let elems = n * m.d_model
        t |> success(st.enc_gen != 0ul, "the served encode recorded its generation ({st.enc_gen})")
        t |> success(metal_tower_enc_rows(st.enc_gen, elems) != null, "the served encode's rows are held under the generation it recorded")
        t |> equal(metal_tower_held_rows_elems(), elems, "the hold's element count")
        t |> success(metal_tower_enc_rows(0ul, elems) == null, "generation 0 - no device encode - hands off nothing")
        t |> success(metal_tower_enc_rows(st.enc_gen, elems + 1l) == null, "another element count hands off nothing")
        var st2 = GigaamState()
        gigaam_log_mel(samples, m, st2)
        gigaam_encode(m, st2)
        t |> success(st2.enc_gen != st.enc_gen, "a second session's encode records its own generation")
        t |> success(metal_tower_enc_rows(st.enc_gen, elems) == null, "after a second session's encode the first session's generation hands off nothing")
        t |> success(metal_tower_enc_rows(st2.enc_gen, elems) != null, "the second session's generation hands its rows off")
        delete st2
        delete st
        let s0 = metal_tower_stats()
        set_metal_take_max_probe_(512ul)
        var refused = false
        try {
            var inscope mem <- ga_leg(m, samples)
        } recover {
            refused = true
        }
        set_metal_take_max_probe_(take_was)
        t |> equal(metal_tower_stats().encodes - s0.encodes, 0l, "a scratch the tower cannot take: no encode on the device")
        t |> success(refused, "... the Metal-flavored model refuses the CPU chain")
    }
}

//! the asr_prof buckets a GigaAM transcribe fills: the CPU chain's stage and block buckets with the tower off, the device
//! chain's command-buffer time with it on, and the rotary chain's replayed restrides and slab trio under the replay alone; the
//! replay pinned off at the head, every knob and the profiler put back as found
[cold_path, unused_argument(t, m, mc, samples)]   // off-Apple the gated body is empty
def private ga_prof_cell(t : T?; m, mc : GigaamModel; samples : array<float>) {
    with_gpu_knobs() {
        ga_prof_legs(t, m, mc, samples)
    }
}

[unused_argument(t, m, mc, samples)]   // off-Apple the gated body is empty
def private ga_prof_legs(t : T?; m, mc : GigaamModel; samples : array<float>) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let replay_found = set_metal_fc_prof_replay(false)
        let prof_found = asr_prof_on()
        set_asr_prof(true)
        asr_prof_reset()
        set_metal_tower(false)
        var inscope cpu <- ga_leg(mc, samples)
        set_metal_tower(true)
        for (b in ["ga.mel", "ga.front", "ga.blocks", GIGAAM_ENCODE_BUCKET, "ga.ffn", "ga.attn_heads", "ga.conv_module"]) {
            t |> success(asr_prof_ms(b) > 0.0lf, "tower off: the CPU chain fills {b} ({asr_prof_ms(b)} ms)")
        }
        t |> equal(asr_prof_ms("ga.encode.gpu"), 0.0lf, "tower off: no device time")
        asr_prof_reset()
        var inscope dev <- ga_leg(m, samples)
        for (b in ["ga.encode.gpu", "ga.encode.record", GIGAAM_ENCODE_BUCKET]) {
            t |> success(asr_prof_ms(b) > 0.0lf, "tower on: the device chain fills {b} ({asr_prof_ms(b)} ms)")
        }
        t |> equal(asr_prof_ms("ga.encode.gpu.attn"), 0.0lf, "replay off: no replayed slab trio")
        set_metal_fc_prof_replay(true)
        asr_prof_reset()
        var inscope rep <- ga_leg(m, samples)
        set_metal_fc_prof_replay(replay_found)
        for (b in ["ga.encode.gpu.restride", "ga.encode.gpu.attn"]) {
            t |> success(asr_prof_ms(b) > 0.0lf, "replay on: the replay fills {b} ({asr_prof_ms(b)} ms)")
        }
        asr_prof_reset()
        var inscope mel : array<float>
        metal_gigaam_mel(m, samples, mel)
        t |> success(asr_prof_ms("ga.mel.gpu") > 0.0lf, "the mel seat fills ga.mel.gpu")
        set_asr_prof(prof_found)
        t |> equal(dev.text, cpu.text, "the profiled device pass's transcript")
        t |> equal(rep.text, cpu.text, "the replayed pass's transcript")
    }
}

[test(description = "GigaAM v3 e2e-rnnt on Metal runs its conv front and every block on the device within 1e-2 of the CPU exact rows with the CPU transcript on the ru clips, a scaled mel outside the bar, and its q8 and kq lanes hold"), arch(at = "../ARCHITECTURE_GPU_TOWER_FC.md#tower-fc-rotary")]
def test_gigaam_metal_twin(tst : T?) {
    tst |> run("GigaAM v3 e2e-rnnt: the Metal rotary FastConformer chain vs the CPU exact chain on the FLEURS ru clips, the q8 lane and the Q4_K carrier's kq lane beside it") @(t : T?) {
        static_if (typeinfo builtin_module_exists(das_metal)) {
            let path = path_join(models_dir(), "gigaam-v3-e2e-rnnt-f16.gguf")
            return if (!family_on(t, "gigaam") || !model_available(t, path))
            let kq_path = path_join(models_dir(), "gigaam-v3-e2e-rnnt-q4_k.gguf")
            with_dasllama_jobque_() {
                var inscope m <- mint_ga(path, false, -1l, TowerDev.metal)
                var inscope mc <- mint_ga(path, false, -1l)
                for (clip in ["ru_short", "ru_mid"]) {
                    let wav = asr_clip("{clip}.wav")
                    if (!stat(wav).is_valid) {
                        t |> skip("{clip}.wav not present")
                        continue
                    }
                    var inscope samples <- load_audio_16k_mono(wav)
                    ga_clip_cell(t, m, mc, samples, clip)
                    ga_q8_cell(t, path, mc, samples, clip)
                    if (model_available(t, kq_path)) {
                        // the CPU kq leg in disk order on the portable batch GEMM, as the Metal image holds its planes: the ids compare
                        let pinned = with_kernel_backend("portable") {
                            ga_kq_cell(t, kq_path, mc, samples, clip, "k4")
                        }
                        t |> success(pinned, "{clip}: the portable backend pinned for the kq lane cell")
                    }
                    let mix_path = path_join(models_dir(), "gigaam-v3-e2e-rnnt-q4_k-mix-local.gguf")
                    if (clip == "ru_short" && model_available(t, mix_path)) {
                        // the mixed-member carrier: Q4_K attention, Q6_K FF, Q8_0 conv pointwise - two K-quant members on one lane
                        let pinned = with_kernel_backend("portable") {
                            ga_kq_cell(t, mix_path, mc, samples, clip, "kq")
                        }
                        t |> success(pinned, "{clip}: the portable backend pinned for the mixed kq lane cell")
                    }
                    if (clip == "ru_short") {
                        ga_seat_arms_cell(t, m, mc, samples)
                        ga_prof_cell(t, m, mc, samples)
                    }
                    if (clip == "ru_mid") {
                        var inscope head <- ga_lattice_clip(m, samples)
                        if (empty(head)) {
                            t |> skip("ru_mid holds under 64 encoder frames - no pad-free clip")
                            continue
                        }
                        let frames = ga_clip_cell(t, m, mc, head, "ru_mid lattice")
                        t |> success(frames >= 64l && frames % 64l == 0l, "ru_mid lattice: the encoder frames are a whole multiple of 64 ({frames})")
                    }
                }
                with_gpu_knobs() {
                    set_metal_tower(true)
                    let ga_q8 = gigaam_serves_q8()
                    t |> equal(ga_q8, !gigaam_gpu_exact_serves(), "the tower on: the un-pinned lane follows the driver's faster lane")
                    t |> equal(!ga_q8, get_metal_tower_f16() && pf_hmm_ready(), "the tower on: the un-pinned lane is the f16 planes where the toolchain compiled the half tensor GEMM, the q8 slab on a box without it")
                    set_metal_tower(false)
                    t |> success(gigaam_serves_q8(), "the tower off: the un-pinned lane is the q8 planes (the control)")
                }
                metal_tower_shutdown()
            }
        } else {
            t |> skip("no Metal build - the GigaAM rotary FastConformer driver has no device to serve")
        }
    }
}

[test(description = "A canary encode served on the Metal tower counts one device encode and leaves zero encoder rows held on the device, since no transducer decodes them")]
def test_canary_metal_holds_no_rows(tst : T?) {
    tst |> run("canary's served Metal encode leaves no encoder rows on the device: no transducer decodes them") @(t : T?) {
        static_if (typeinfo builtin_module_exists(das_metal)) {
            let path = path_join(models_dir(), "canary-qwen-2.5b-encoder-f32.bin")
            return if (!family_on(t, "canary") || !model_available(t, path))
            var inscope samples <- load_audio_16k_mono(asr_clip("jfk.wav"))
            if (empty(samples)) {
                t |> skip("jfk.wav not present")
                return
            }
            with_dasllama_jobque_() {
                var inscope enc <- mint_cn(path, false, -1l, TowerDev.metal)
                var st = CanaryState()
                let e0 = metal_tower_stats()
                canary_log_mel(enc, samples, st)
                canary_encode(enc, st)
                t |> equal(metal_tower_stats().encodes - e0.encodes, 1l, "the encode served on the device")
                t |> equal(metal_tower_held_rows_elems(), 0l, "no rows held past it")
                delete st
                metal_tower_shutdown()
            }
        } else {
            t |> skip("no Metal build")
        }
    }
}

//! family `fam`'s q8 lane three ways on the Metal home (`twin_three_way`), every stage on the device, then the poisoned image `mint_poisoned` mints
[cold_path, unused_argument(t, tag, fam, tw, st, twd, std, twx, stx, mint_poisoned, encode)]
def private metal_q8_twin(t : T?; tag : string; fam : TowerFamily; tw : auto(TW); var st : auto(ST); twd : TW; var std : ST; twx : TW; var stx : ST;
                          mint_poisoned : block<() : TW>; encode : block<(e : TW; var s : ST; var out : array<float>) : int64>) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let n_layer = twin_facts(tw).n_layer
        let dq0 = gpu_tower_quant_declines(fam)
        let f0 = metal_tower_f16_encodes()
        let spec = TwinSpec(family = "{fam}", tag = tag, n_layer = n_layer, metric = TwinMetric.rel_l2, seat_family = fam)
        let r <- twin_three_way(t, spec, tw, st, twd, std, twx, stx, encode) $(legs) {
            t |> equal(legs.n_cpu, legs.n_exact, "{tag}: the q8 chain's frame count")
            t |> equal(legs.n_dev, legs.n_exact, "{tag}: the device chain's frame count")
            let n_enc = legs.dev.encodes - legs.pre.encodes
            t |> success(n_enc >= 1l, "{tag}: the device served the encodes ({n_enc})")
            t |> equal(legs.dev.blocks - legs.pre.blocks, n_enc * n_layer, "{tag}: every block of every encode ran on the driver")
            front_served(t, tag, legs.dev.convs - legs.pre.convs, n_enc, legs.dev.declines - legs.cpu.declines)
        }
        t |> equal(gpu_tower_quant_declines(fam) - dq0, 0l, "{tag}: the q8 lane declined nothing (`quant_mode`)")
        t |> equal(metal_tower_f16_encodes() - f0, 0l, "{tag}: the q8 lane read the q8 slab, not the halfword plane")
        if (r.bar > 0.0lf) {
            var twp <- invoke(mint_poisoned)
            var stp = ST()
            twin_poison(t, r, n_layer / 2l, twp, stp, encode)
            delete stp
            delete twp
        }
    }
}

//! the Metal home's FastConformer rel-form twins on jfk: the q8 lane three ways with its poison, then the exact lane (`fc_exact_lane_leg`)
[test(description = "Canary and parakeet v3 on the Metal FastConformer chain serve every block on the q8 slab within the three-way bar on jfk with no quant_mode decline, a poison exceeds it, and the exact f16 lane holds its bar"), arch(at = "../ARCHITECTURE_GPU_TOWER.md#tower-encode-chains")]
def test_fc_metal_twin(tst : T?) {
    tst |> run("canary-qwen 2.5B: the Metal FastConformer chain's q8 and f16 lanes vs the CPU chains on jfk, the exact chain the reference") @(t : T?) {
        static_if (typeinfo builtin_module_exists(das_metal)) {
            let path = path_join(models_dir(), "canary-qwen-2.5b-encoder-f32.bin")
            return if (!family_on(t, "canary"))
            metal_jfk_twin_cell(t, path, false, TowerFamily.canary) $(samples) {
                var encx <- mint_cn(path, false, -1l)
                var enc <- mint_cn(path, true, -1l)
                var encd <- mint_cn(path, true, -1l, TowerDev.metal)
                t |> success(!encx.q8 && enc.q8 && encd.q8 && encd.dev_backend == TowerDev.metal, "the exact and the q8 encoders minted, the q8 lane's Metal image beside")
                var stx = CanaryState()
                var st = CanaryState()
                var std = CanaryState()
                metal_q8_twin(t, "canary metal jfk", TowerFamily.canary, enc, st, encd, std, encx, stx, $ { return <- mint_cn(path, true, enc.n_layer / 2l, TowerDev.metal) }) $(e; var s; var out) => cn_twin_encode(e, s, samples, out)
                delete std
                delete encd
                delete enc
                fc_exact_lane_leg(t, "canary metal jfk", TowerFamily.canary, encx, stx, $(zb) { return <- mint_cn(path, false, zb, TowerDev.metal) }) $(e; var s; var out) => cn_twin_encode(e, s, samples, out)
                delete st
                delete stx
                delete encx
                metal_tower_shutdown()
            }
        } else {
            t |> skip("no Metal build")
        }
    }
    tst |> run("parakeet-TDT 0.6B v3: the Metal FastConformer chain's q8 and f16 lanes vs the CPU chains on jfk, the exact chain the reference") @(t : T?) {
        static_if (typeinfo builtin_module_exists(das_metal)) {
            let path = path_join(whisper_dir(), "ggml-parakeet-tdt-0.6b-v3-f32.bin")
            return if (!family_on(t, "parakeet"))
            metal_jfk_twin_cell(t, path, false, TowerFamily.parakeet) $(samples) {
                var mx <- mint_pk(path, false, -1l)
                var m <- mint_pk(path, true, -1l)
                var md <- mint_pk(path, true, -1l, TowerDev.metal)
                t |> success(!mx.q8 && m.q8 && md.q8 && md.dev_backend == TowerDev.metal, "the exact and the q8 models minted, the q8 lane's Metal image beside")
                var stx = ParakeetState()
                var st = ParakeetState()
                var std = ParakeetState()
                metal_q8_twin(t, "parakeet metal jfk", TowerFamily.parakeet, m, st, md, std, mx, stx, $ { return <- mint_pk(path, true, m.n_layer / 2l, TowerDev.metal) }) $(e; var s; var out) => pk_twin_encode(e, s, samples, out)
                delete std
                delete md
                delete m
                fc_exact_lane_leg(t, "parakeet metal jfk", TowerFamily.parakeet, mx, stx, $(zb) { return <- mint_pk(path, false, zb, TowerDev.metal) }) $(e; var s; var out) => pk_twin_encode(e, s, samples, out)
                delete st
                delete stx
                delete mx
                metal_tower_shutdown()
            }
        } else {
            t |> skip("no Metal build")
        }
    }
}

//! the Metal home's gemma4a twins on jfk, E2B and E4B: the q8 lane three ways with its poison, then the exact lane (`exact_flavor_leg`)
[test(description = "The gemma4a E2B and E4B encoders on the Metal Conformer chain serve every block on the q8 slab within the three-way bar on jfk, a poison exceeds it, and the exact f16 lane holds its bar"), arch(at = "../ARCHITECTURE_GPU_TOWER.md#tower-encode-chains")]
def test_gemma4a_metal_twin(tst : T?) {
    for (tag, mm_file in fixed_array("E2B", "E4B"), fixed_array("mmproj-gemma-4-E2B-it-bf16.gguf", "mmproj-gemma-4-E4B-it-BF16.gguf")) {
        let mm = path_join(models_dir(), mm_file)
        tst |> run("gemma4a {tag}: the Metal Conformer chain's q8 and f16 lanes vs the CPU chains on jfk ({base_name(mm)}), the exact chain the reference") @(t : T?) {
            static_if (typeinfo builtin_module_exists(das_metal)) {
                return if (!family_on(t, "gemma4a"))
                let jt = "gemma4a {tag} metal jfk"
                metal_jfk_twin_cell(t, mm, true, TowerFamily.gemma4a) $(samples) {
                    var encx <- mint_g4a(mm, false, -1l)
                    var enc <- mint_g4a(mm, true, -1l)
                    var encd <- mint_g4a(mm, true, -1l, TowerDev.metal)
                    t |> success(!encx.q8 && enc.q8 && encd.q8 && encd.dev_backend == TowerDev.metal, "{jt}: the exact and the q8 encoders minted, the q8 lane's Metal image beside")
                    var stx = Gemma4aState()
                    var st = Gemma4aState()
                    var std = Gemma4aState()
                    metal_q8_twin(t, jt, TowerFamily.gemma4a, enc, st, encd, std, encx, stx, $ { return <- mint_g4a(mm, true, enc.n_layer / 2l, TowerDev.metal) }) $(e; var s; var out) => gemma4a_encode_all(e, s, samples, out)
                    delete std
                    delete encd
                    delete enc
                    exact_flavor_leg(t, jt, TowerFamily.gemma4a, encx, stx, -1l, $(zb) { return <- mint_g4a(mm, false, zb, TowerDev.metal) }) $(e; var s; var out) => gemma4a_encode_all(e, s, samples, out)
                    delete st
                    delete stx
                    delete encx
                    metal_tower_shutdown()
                }
            } else {
                t |> skip("no Metal build")
            }
        }
    }
}

//! the GigaAM model staged and minted in memory on the lane asked; `zero_block` >= 0 zeroes the ten GEMM planes of
//! `poison_blocks` blocks from that block on - the q8 quants, or on the exact lane the halfwords
def private mint_ga(path : string; q8 : bool; zero_block : int64; dev : TowerDev = TowerDev.none) : GigaamModel {
    var inscope stg <- tower_dev_staged(dev) $ { return <- stage_gigaam_model(path, q8, zero_block >= 0l) }
    if (stg.e.kq_lane) {
        if (zero_block >= 0l) {
            poison_kq_seats(stg.gemm_seats, zero_block * FC_BLOCK_GEMMS, long_length(stg.gemm_seats) / TOWER_SEAT_WORDS, stg.kq, stg.qblob)
        }
    } elif (q8) {
        poison_fc_blocks(stg.qblob, stg.e.n_layer, zero_block, stg.e.d_model, stg.e.n_ff) $(b) => gigaam_layer_offsets(stg.e, b)
    } else {
        poison_fc_blocks(stg.wblob, stg.e.n_layer, zero_block, stg.e.d_model, stg.e.n_ff) $(b) => gigaam_layer_offsets(stg.e, b)
    }
    var m = GigaamModel()
    mint_gigaam_model(stg, m, q8, dev)
    return <- m
}

//! `mint_ga` in the flavor the lane serves on this build (`#tower-dev-block`): the GPU driver's where its chain serves the lane,
//! else the CPU image
def private mint_ga_served(path : string; q8 : bool; zero_block : int64) : GigaamModel {
    var inscope probe <- stage_gigaam_model(path, q8, false)
    let dev = gigaam_dev_flavor(probe.e)
    return <- mint_ga(path, q8, zero_block, dev)
}

//! the device's declines that keep a lane off it on this box: `device` (no form for the lane's GEMMs), or `flavor` (the lane
//! loaded its CPU image)
def private off_device_declines() : int64
    => gpu_tower_declines_of("device", TowerFamily.gigaam) + gpu_tower_declines_of("flavor", TowerFamily.gigaam)

//! gigaam's device log-mel alone on its home (the encode chain's mel record) into `mel`: its frame count
[unused_argument(m, samples, mel)]
def private gpu_gigaam_mel(m : GigaamModel; samples : array<float>; @exact_size var mel : array<float>) : int64 {
    static_if (typeinfo builtin_module_exists(cuda)) {
        return cuda_gigaam_mel(m, samples, mel) if (tower_family_cuda(TowerFamily.gigaam))
    }
    static_if (typeinfo builtin_module_exists(vulkan)) {
        return vulkan_gigaam_mel(m, samples, mel)
    } else {
        return 0l
    }
}

def private ga_twin_encode(m : GigaamModel; var st : GigaamState; clip : array<float>; var out : array<float>) : int64 {
    return x_rows_encode(st, m.d_model, out) $(var s) {
        return gigaam_encode_span(clip, 0l, long_length(clip), m, s)
    }
}

//! gigaam's device log-mel alone on its home against `gigaam_log_mel` at `GA_MEL_BAR`, frame count and counter; a poisoned element the control
[unused_argument(t, m, samples, clip)]   // without a discrete home the gated body is empty
def private ga_discrete_mel_twin_cell(t : T?; m : GigaamModel; samples : array<float>; clip : string) {
    static_if (typeinfo builtin_module_exists(vulkan) || typeinfo builtin_module_exists(cuda)) {
        var st = GigaamState()
        with_dasllama_jobque_() {
            gigaam_log_mel(samples, m, st)
        }
        var inscope dev : array<float>
        let s0 = gpu_tower_counters(TowerFamily.gigaam)
        let n = gpu_gigaam_mel(m, samples, dev)
        t |> equal(n, st.n_len, "{clip}: the device mel's frame count")
        t |> equal(gpu_tower_counters(TowerFamily.gigaam).mels - s0.mels, 1l, "{clip}: the mel seat ran on the device")
        check_rows(t, "gigaam {clip} {serving_tower_dev(TowerFamily.gigaam)}: the device log-mel tracks the CPU's", dev, st.mel, Bar(rel = GA_MEL_BAR, poison_scaled = true), int(m.n_mel))
        var inscope loud <- [for (v in samples); v * 1.1]
        var inscope dev_loud : array<float>
        gpu_gigaam_mel(m, loud, dev_loud)
        check_exceeds(t, "gigaam {clip} {serving_tower_dev(TowerFamily.gigaam)}: the control - the clip at 1.1x on the device side off the CPU's log-mel", dev_loud, st.mel,
            Bar(rel = GA_MEL_BAR))
        delete st
    }
}

//! one clip's device legs on gigaam's home: the three-way twin over the q8 GPU image `md`, both transcripts, the range refusal, the poison
[cold_path, unused_argument(t, path, mx, m, md, samples, clip)]   // without a discrete home the gated body is empty
def private ga_discrete_clip_cell(t : T?; path : string; mx, m, md : GigaamModel; samples : array<float>; clip : string) {
    static_if (typeinfo builtin_module_exists(vulkan) || typeinfo builtin_module_exists(cuda)) {
        var stx = GigaamState()
        var st = GigaamState()
        var std = GigaamState()
        let tag = "gigaam {clip}"
        let spec = TwinSpec(family = "gigaam", tag = tag, n_layer = m.n_layer, metric = TwinMetric.rel_l2, seat_family = TowerFamily.gigaam)
        let r <- twin_three_way(t, spec, m, st, md, std, mx, stx, $(e; var s; var out) => ga_twin_encode(e, s, samples, out)) $(legs) {
            t |> equal(legs.n_cpu, legs.n_exact, "{tag}: the q8 chain's frame count")
            t |> equal(legs.n_dev, legs.n_exact, "{tag}: the device chain's frame count")
            t |> equal(legs.dev.encodes - legs.pre.encodes, 1l, "{tag}: the device served the encode")
            t |> equal(legs.dev.blocks - legs.pre.blocks, m.n_layer, "{tag}: every block ran on the driver")
            front_served(t, tag, legs.dev.convs - legs.pre.convs, 1l, legs.dev.declines - legs.cpu.declines)
            t |> equal(legs.dev.mels - legs.pre.mels, legs.dev.convs - legs.pre.convs, "{tag}: the log-mel ran on the device wherever the front did")
        }
        ga_discrete_mel_twin_cell(t, md, samples, clip)
        delete stx
        delete st
        delete std
        with_gpu_knobs() {
            with_dasllama_jobque_() {
                set_serving_gpu_tower(false, TowerFamily.gigaam)
                var inscope cpu <- ga_leg(m, samples)
                set_serving_gpu_tower(true, TowerFamily.gigaam)
                let k4_0 = kq_tile_count("k4")
                var inscope dev <- ga_leg(md, samples)
                t |> equal(kq_tile_count("k4") - k4_0, 0l, "{tag}: the q8 lane dispatches no k4 tile (the kq lane counter's control)")
                to_log(LOG_INFO, "test_audio: gigaam {clip} {md.dev_backend} vs cpu q8: rel_l2 {rel_l2(dev.x, cpu.x)}\n")
                to_log(LOG_INFO, "test_audio: gigaam {clip} cpu q8: {cpu.text}\n")
                to_log(LOG_INFO, "test_audio: gigaam {clip} {md.dev_backend}: {dev.text}\n")
                t |> equal(dev.text, cpu.text, "{tag}: the device chain's transcript is the CPU q8 chain's")
                blocks_refused_leg(t, tag, TowerFamily.gigaam, 64l << 20l) $ {
                    var inscope refused <- ga_leg(md, samples)
                }
            }
        }
        if (r.bar > 0.0lf) {
            var mp <- mint_ga(path, true, m.n_layer / 2l, md.dev_backend)
            var stp = GigaamState()
            twin_poison(t, r, m.n_layer / 2l, mp, stp) $(e2; var s; var out) => ga_twin_encode(e2, s, samples, out)
            delete stp
            delete mp
        }
    }
}

//! GigaAM's exact lane on its discrete home's chain (the f16 GEMM over the halfwords) against the CPU exact chain: every stage on the device with no `quant_mode` decline, the CPU's frames and ids, the rows nearer the exact chain than the CPU q8 chain's, the zeroed-blocks control past that bar; off the f16 feed the lane loads its CPU image (`mxd` the exact lane's flavor on this box)
[cold_path, unused_argument(t, path, mx, mxd, mq, samples, clip)]   // without a discrete home the gated body is empty
def private ga_discrete_exact_cell(t : T?; path : string; mx, mxd, mq : GigaamModel; samples : array<float>; clip : string) {
    static_if (typeinfo builtin_module_exists(vulkan) || typeinfo builtin_module_exists(cuda)) {
        let tag = "gigaam {clip} exact lane"
        t |> success(!mx.q8 && mx.gemm_f16, "{tag}: the block GEMMs serve off the halfword plane")
        with_gpu_knobs() {
            with_dasllama_jobque_() {
                set_serving_gpu_tower(false, TowerFamily.gigaam)
                var inscope ex <- ga_leg(mx, samples)
                var inscope q8 <- ga_leg(mq, samples)
                set_serving_gpu_tower(true, TowerFamily.gigaam)
                let s0 = gpu_tower_counters(TowerFamily.gigaam)
                let dq0 = gpu_tower_quant_declines(TowerFamily.gigaam)
                let dd0 = off_device_declines()
                var inscope dev <- ga_leg(mxd, samples)
                let s1 = gpu_tower_counters(TowerFamily.gigaam)
                t |> equal(gpu_tower_quant_declines(TowerFamily.gigaam) - dq0, 0l, "{tag}: no quant_mode decline")
                t |> equal(dev.n, ex.n, "{tag}: the frame count")
                if (!gpu_tower_front_route(TowerFamily.gigaam)) {
                    t |> equal(s1.encodes - s0.encodes, 0l, "{tag}: off the f16 GEMM feed no encode on the device")
                    t |> success(off_device_declines() > dd0, "{tag}: off the f16 GEMM feed the chain declined the lane")
                    return
                }
                device_leg_served(t, tag, s0, s1, mx.n_layer, -1l)
                t |> equal(s1.convs - s0.convs, 1l, "{tag}: the conv front ran on the device")
                t |> equal(dev.ids, ex.ids, "{tag}: the token ids are the CPU exact chain's")
                to_log(LOG_INFO, "test_audio: {tag} cpu exact: {ex.text}\n")
                to_log(LOG_INFO, "test_audio: {tag} {mxd.dev_backend}   : {dev.text}\n")
                var q8_ex = TowerParityStats()
                var dev_ex = TowerParityStats()
                if (tower_parity_stats(t, "{tag} cpu q8 vs exact", ex.x, q8.x, q8_ex) && tower_parity_stats(t, "{tag} device vs exact", ex.x, dev.x, dev_ex)) {
                    let bar = q8_ex.rel_l2
                    to_log(LOG_INFO, "test_audio: {tag}: rel_l2 from the exact chain - {mxd.dev_backend} exact lane {dev_ex.rel_l2}, the bar (cpu q8) {bar}\n")
                    t |> success(dev_ex.rel_l2 <= bar, "{tag}: the device rows sit nearer the exact chain than the CPU q8 chain's ({dev_ex.rel_l2} vs {bar})")
                    var mp <- mint_ga(path, false, mx.n_layer / 2l, serving_tower_dev(TowerFamily.gigaam))
                    let p0 = gpu_tower_encodes(TowerFamily.gigaam)
                    var inscope bad <- ga_leg(mp, samples)
                    t |> equal(gpu_tower_encodes(TowerFamily.gigaam) - p0, 1l, "{tag} poison: device-served (encodes delta)")
                    var bad_ex = TowerParityStats()
                    if (tower_parity_stats(t, "{tag} poisoned device vs exact", ex.x, bad.x, bad_ex)) {
                        t |> success(is_finite(bad_ex.rel_l2) && bad_ex.rel_l2 > bar, "{tag}: zeroed halfword blocks through the device chain EXCEED the bar ({bad_ex.rel_l2} vs {bar})")
                    }
                    delete mp
                }
            }
        }
    }
}

//! GigaAM's lane policy on its discrete home: tower on, the lane knob picks q8 or exact; tower off, q8 whatever the knob; the knob restored
[unused_argument(t)]   // without a discrete home the gated body is empty
def private ga_discrete_lane_policy_cell(t : T?) {
    static_if (typeinfo builtin_module_exists(vulkan) || typeinfo builtin_module_exists(cuda)) {
        with_gpu_knobs() {
            reset_gigaam_q8()   // the cell reads the un-pinned policy's lane
            set_serving_gpu_tower(true, TowerFamily.gigaam)
            let was = set_gpu_gigaam_exact_lane(false)
            t |> success(gigaam_gpu_serves() && gigaam_serves_q8(), "the lane knob off: the un-pinned lane is the q8 image")
            set_gpu_gigaam_exact_lane(true)
            t |> success(!gigaam_serves_q8(), "the lane knob on: the un-pinned lane is the exact planes")
            set_serving_gpu_tower(false, TowerFamily.gigaam)
            t |> success(gigaam_serves_q8(), "the tower off: the un-pinned lane is the q8 image whatever the knob (the control)")
            set_gpu_gigaam_exact_lane(was)
        }
    }
}

//! the device's k4 cm2 and KHR tile dispatches: the kq lane's K-quant seats run on one of them, by the device's mode and the column pick
//! the dispatches of the K-quant member `fmt`'s batch tiles (`kq_batch_<fmt>_*`) the census counted
[unused_argument(fmt, fam)]
def private kq_tile_count(fmt : string; fam : TowerFamily = TowerFamily.gigaam) : int64 {
    static_if (typeinfo builtin_module_exists(cuda)) {
        if (tower_family_cuda(fam)) {
            return (max(cud_kernel_census_of("kq_batch_{fmt}_cm2l_cls_cuda"), 0l) + max(cud_kernel_census_of("kq_batch_{fmt}_cm2m_cls_cuda"), 0l)
                + max(cud_kernel_census_of("kq_batch_{fmt}_cm2s_cls_cuda"), 0l))
        }
    }
    static_if (typeinfo builtin_module_exists(vulkan)) {
        return (vk_kernel_coverage_of("kq_batch_{fmt}_cm2l_cls_spv") + vk_kernel_coverage_of("kq_batch_{fmt}_cm2m_cls_spv")
            + vk_kernel_coverage_of("kq_batch_{fmt}_cm2s_cls_spv") + vk_kernel_coverage_of("kq_batch_{fmt}_khr_cls_spv"))
    } else {
        return 0l
    }
}

//! the carrier's K-quant members, Q8_0 aside, by the names their tiles take
def private kq_members_of(m : GigaamModel) : array<string> {
    let names <- split(gigaam_kq_members(m), "+")
    return <- [for (name in names); name; where name != "q8"]
}

//! GigaAM's kq lane on its discrete home's chain against the CPU kq chain: the lane named `fmt`, no `kq_lane` or `quant_mode`
//! decline, every K-quant member's tiles counted - "a kernel counter ... that no other plane of that file moves"
//! (REVIEW_KQ_FORMATS.md); off the f16 GEMM feed the chain declines `device`
[cold_path, unused_argument(t, kq_path, mx, mk, mkd, samples, clip, fmt), arch(at = "../ARCHITECTURE_GPU_TOWER_VULKAN_FC.md#vk-tower-fc-kq")]   // without a discrete home the gated body is empty
def private ga_discrete_kq_cell(t : T?; kq_path : string; mx, mk, mkd : GigaamModel; samples : array<float>; clip, fmt : string) {
    static_if (typeinfo builtin_module_exists(vulkan) || typeinfo builtin_module_exists(cuda)) {
        let tag = "gigaam {clip} kq lane {gigaam_kq_members(mk)}"
        t |> success(mk.kq_lane, "{tag}: the carrier serves as its own planes")
        t |> equal(gigaam_exec_fmt(mk), fmt, "{tag}: the lane's name - its one K-quant member's disk type, or `kq` where the members mix")
        with_gpu_knobs() {
            with_dasllama_jobque_() {
                let members <- kq_members_of(mk)
                set_serving_gpu_tower(false, TowerFamily.gigaam)
                var inscope ex <- ga_leg(mx, samples)
                var inscope cpu <- ga_leg(mk, samples)
                set_serving_gpu_tower(true, TowerFamily.gigaam)
                let s0 = gpu_tower_counters(TowerFamily.gigaam)
                let dk0 = gpu_tower_declines_of("kq_lane", TowerFamily.gigaam)
                let dq0 = gpu_tower_quant_declines(TowerFamily.gigaam)
                let dd0 = off_device_declines()
                let tiles0 <- [for (n in members); kq_tile_count(n)]
                var inscope dev <- ga_leg(mkd, samples)
                let s1 = gpu_tower_counters(TowerFamily.gigaam)
                t |> equal(gpu_tower_declines_of("kq_lane", TowerFamily.gigaam) - dk0, 0l, "{tag}: no kq_lane decline")
                t |> equal(gpu_tower_quant_declines(TowerFamily.gigaam) - dq0, 0l, "{tag}: no quant_mode decline")
                t |> equal(dev.n, cpu.n, "{tag}: the frame count")
                if (!gpu_tower_front_route(TowerFamily.gigaam)) {
                    t |> equal(s1.encodes - s0.encodes, 0l, "{tag}: off the f16 GEMM feed no encode on the device")
                    t |> success(off_device_declines() > dd0, "{tag}: off the f16 GEMM feed the chain declined the lane")
                    return
                }
                device_leg_served(t, tag, s0, s1, mk.n_layer, -1l)
                t |> equal(s1.convs - s0.convs, 1l, "{tag}: the conv front ran on the device")
                for (n, c0 in members, tiles0) {
                    t |> success(kq_tile_count(n) > c0, "{tag}: the {n} tiles served the {n} seats (the census counter rose)")
                }
                t |> equal(dev.ids, cpu.ids, "{tag}: the token ids are the CPU kq chain's")
                to_log(LOG_INFO, "test_audio: {tag} cpu kq : {cpu.text}\n")
                to_log(LOG_INFO, "test_audio: {tag} {mkd.dev_backend} : {dev.text}\n")
                let bar = tower_twin_bar(t, tag, TowerFamily.gigaam, ex.x, cpu.x, dev.x, mk.n_layer, double(TWIN_SLACK_VK), TWIN_REL_L2_FLOOR)
                if (bar > 0.0lf) {
                    ga_kq_poison_leg(t, tag, kq_path, mk.n_layer, serving_tower_dev(TowerFamily.gigaam), ex.x, samples, bar)
                }
            }
        }
    }
}

//! the kq lane over the mixed-member carrier (Q4_K attention, Q6_K FF, Q8_0 conv pointwise), its home's image beside its CPU one
[cold_path, unused_argument(t, mx, samples, clip)]   // without a discrete home the gated body is empty
def private ga_discrete_kq_mix_cell(t : T?; mx : GigaamModel; samples : array<float>; clip : string) {
    static_if (typeinfo builtin_module_exists(vulkan) || typeinfo builtin_module_exists(cuda)) {
        let mix_path = path_join(models_dir(), "gigaam-v3-e2e-rnnt-q4_k-mix-local.gguf")
        return if (!model_available(t, mix_path))
        var inscope mm <- mint_ga(mix_path, false, -1l)
        var inscope mmd <- mint_ga_served(mix_path, false, -1l)
        t |> equal(gigaam_kq_members(mm), "k4+k6+q8", "the mixed carrier's members: Q4_K attention, Q6_K FF, Q8_0 conv pointwise")
        ga_discrete_kq_cell(t, mix_path, mx, mm, mmd, samples, clip, "kq")
    }
}

[test(description = "GigaAM v3 e2e-rnnt on the discrete home holds the three-way bar on its q8 lane, its exact f16 lane and its Q4_K and mixed kq lanes over the ru clips, with the CPU chain ids, every member tile counted and poisons outside"), arch(at = "../ARCHITECTURE_GPU_TOWER_VULKAN_FC.md#vk-tower-fc-rotary")]
def test_gigaam_discrete_twin(tst : T?) {
    tst |> run("GigaAM v3 e2e-rnnt: the discrete home's rotary FastConformer front and blocks on every lane - the q8 lane vs the CPU q8 chain, the exact lane off the halfwords vs the CPU exact chain, the Q4_K carrier's kq lane vs the CPU kq chain - on the FLEURS ru clips") @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan) || typeinfo builtin_module_exists(cuda)) {
            let path = path_join(models_dir(), "gigaam-v3-e2e-rnnt-f16.gguf")
            return if (!family_on(t, "gigaam") || !model_available(t, path) || !discrete_audio_twin_seat(t, TowerFamily.gigaam))
            let home = serving_tower_dev(TowerFamily.gigaam)
            var mx <- mint_ga(path, false, -1l)
            var m <- mint_ga(path, true, -1l)
            var md <- mint_ga(path, true, -1l, home)
            var mxd <- mint_ga_served(path, false, -1l)
            t |> success(!mx.q8 && m.q8 && md.dev_backend == home, "the exact and the q8 models minted, the q8 lane's {home} image beside")
            let kq_path = path_join(models_dir(), "gigaam-v3-e2e-rnnt-q4_k.gguf")
            var mk = GigaamModel()
            var mkd = GigaamModel()
            let has_kq = model_available(t, kq_path)
            if (has_kq) {
                delete mk
                delete mkd
                mk <- mint_ga(kq_path, false, -1l)
                mkd <- mint_ga_served(kq_path, false, -1l)
                t |> success(mk.kq_lane, "the Q4_K carrier takes the kq lane")
            }
            for (clip in ["ru_short", "ru_mid"]) {
                let wav = asr_clip("{clip}.wav")
                if (!stat(wav).is_valid) {
                    t |> skip("{clip}.wav not present")
                    continue
                }
                var inscope samples <- load_audio_16k_mono(wav)
                ga_discrete_clip_cell(t, path, mx, m, md, samples, clip)
                ga_discrete_exact_cell(t, path, mx, mxd, m, samples, clip)
                if (has_kq) {
                    ga_discrete_kq_cell(t, kq_path, mx, mk, mkd, samples, clip, "k4")
                }
                if (clip == "ru_short") {
                    ga_discrete_kq_mix_cell(t, mx, samples, clip)
                }
            }
            ga_discrete_lane_policy_cell(t)
            delete mkd
            delete mk
            delete mxd
            delete md
            delete m
            delete mx
            gpu_tower_shutdown(TowerFamily.gigaam)
        } else {
            t |> skip("no discrete GPU home in this build (dasVulkan, dasCUDA)")
        }
    }
}

//! GigaAM's CTC head on the family's discrete seat (`vulkan_gigaam_ctc`, `cuda_gigaam_ctc`) against the CPU head over one encode: each pick the CPU's but at a near-tie, -log sum exp within `CTC_PLOG_BAR`, the ids and transcript the CPU's, the device leg counted once and the CPU leg (`mc`) never, the 1.05x control outside the bar; then the head off the first clip's uploaded rows
[cold_path, unused_argument(t, m, mc, samples, tag)]   // without a discrete home the gated body is empty
def private ga_discrete_ctc_cell(t : T?; m, mc : GigaamModel; samples : array<float>; tag : string) {
    static_if (typeinfo builtin_module_exists(vulkan) || typeinfo builtin_module_exists(cuda)) {
        with_gpu_knobs() {
            with_dasllama_jobque_() {
                set_serving_gpu_tower(true, TowerFamily.gigaam)
                let bar = Bar(rel = CTC_PLOG_BAR, abs_floor = CTC_PLOG_BAR, poison_scaled = true)
                let n_in = min(long_length(samples), GIGAAM_MAX_CLIP_S * AUDIO_SAMPLE_RATE)
                var st = GigaamState()
                let n = gigaam_encode_span(samples, 0l, n_in, m, st)
                let held = st.enc_gen != 0ul
                to_log(LOG_INFO, "test_audio: {tag}: the encode {held ? "served on the device, its rows held" : "ran on the CPU"}\n")
                set_serving_gpu_tower(false, TowerFamily.gigaam)
                var sc = GigaamState()
                sc.x := st.x
                var inscope cpu <- ctc_cpu_leg(mc, sc, n)
                delete sc
                set_serving_gpu_tower(true, TowerFamily.gigaam)
                let s0 = gpu_ctc_stats()
                var inscope dev <- ctc_leg(m, st, n)
                let s1 = gpu_ctc_stats()
                to_log(LOG_INFO, "test_audio: {tag} cpu   : {cpu.text}\n")
                to_log(LOG_INFO, "test_audio: {tag} {m.dev_backend}: {dev.text}\n")
                t |> equal(cpu.served, 0l, "{tag}: the tower knob off - the head stays on the CPU")
                t |> equal(dev.served, 1l, "{tag}: the head served on the device")
                t |> equal(s1.handoffs - s0.handoffs, held ? 1l : 0l, "{tag}: the head read the encode's own device rows where the chain held them")
                t |> equal(ctc_hard_flips(cpu, dev, m.n_classes), 0, "{tag}: the picks are the CPU's but at a near-tie frame")
                check(t, "test_audio: {tag} the device head's per-frame log-sums", dev.plogs, cpu.plogs, bar, FLAT_SHAPE)
                t |> success(sequence_equal(dev.ids, cpu.ids), "{tag}: the token ids are the CPU head's")
                t |> equal(dev.text, cpu.text, "{tag}: the transcript")
                var inscope loud <- [for (v in samples); v * 1.05]
                var sl = GigaamState()
                let nl = gigaam_encode_span(loud, 0l, min(long_length(loud), GIGAAM_MAX_CLIP_S * AUDIO_SAMPLE_RATE), m, sl)
                var inscope moved <- ctc_leg(m, sl, nl)
                t |> equal(moved.served, 1l, "{tag}: the control's head served on the device")
                check_exceeds(t, "test_audio: {tag} the control - the clip at 1.05x through the device chain", moved.plogs, cpu.plogs, bar)
                t |> success(sl.enc_gen != st.enc_gen, "{tag}: the control's encode holds the device rows now")
                delete sl
                let u0 = gpu_ctc_stats()
                var inscope up <- ctc_leg(m, st, n)
                t |> equal(up.served, 1l, "{tag}: off rows the device no longer holds the head serves off the uploaded rows")
                t |> equal(gpu_ctc_stats().handoffs - u0.handoffs, 0l, "{tag}: no held rows to read")
                t |> equal(ctc_hard_flips(cpu, up, m.n_classes), 0, "{tag}: the uploaded rows' picks are the CPU head's but at a near-tie frame")
                check(t, "test_audio: {tag} the head off the uploaded rows' log-sums", up.plogs, cpu.plogs, bar, FLAT_SHAPE)
                t |> equal(up.text, cpu.text, "{tag}: the uploaded rows' transcript")
                delete st
            }
        }
    }
}

[test(description = "The GigaAM CTC head on the discrete seat, off held device rows and off uploaded rows, gives the CPU head picks, ids and transcript with log-sums within CTC_PLOG_BAR, and a 1.05x clip lands outside the bar"), arch(at = "../ARCHITECTURE_MEDIA_ASR.md#gigaam-planes")]
def test_gigaam_ctc_discrete_twin(tst : T?) {
    for (kind, q8 in ["e2e-ctc", "ctc", "e2e-ctc", "ctc"], [false, false, true, true]) {
        tst |> run("GigaAM v3 {kind} {q8 ? "q8" : "f16"}: the CTC head on the discrete device seat is the CPU head's, off the encode's held rows and off uploaded rows") @(t : T?) {
            static_if (typeinfo builtin_module_exists(vulkan) || typeinfo builtin_module_exists(cuda)) {
                let path = path_join(models_dir(), "gigaam-v3-{kind}-f16.gguf")
                return if (!family_on(t, "gigaam") || !model_available(t, path) || !discrete_audio_twin_seat(t, TowerFamily.gigaam))
                let wav = asr_clip("ru_short.wav")
                if (!stat(wav).is_valid) {
                    t |> skip("ru_short.wav not present")
                    return
                }
                var m <- mint_ga_served(path, q8, -1l)
                var mc <- mint_ga(path, q8, -1l)
                t |> success(m.ctc_f16 && m.dw_f16, "the head and the depthwise taps serve off the carrier's halfwords")
                var inscope samples <- load_audio_16k_mono(wav)
                ga_discrete_ctc_cell(t, m, mc, samples, "gigaam {kind} {q8 ? "q8" : "f16"} ru_short")
                delete mc
                delete m
                gpu_tower_shutdown(TowerFamily.gigaam)
            } else {
                t |> skip("no discrete GPU home in this build (dasVulkan, dasCUDA)")
            }
        }
    }
}

[test(description = "The qwen2audio, voxtral, omni-3b and ultravox towers on the discrete home run every block and the projector tail on the device within the three-way bar on a synthetic mel, a poison exceeds it, and the exact lane holds"), arch(at = "../ARCHITECTURE_GPU_TOWER_VULKAN.md#vk-tower-encode-chains")]
def test_encoder_blocks_vulkan(tst : T?) {
    tst |> run("qwen2audio tower: the discrete home's blocksvs the CPU q8 chain on the synthetic mel, 32 layers") @(t : T?) {
        blocks_twin(t, "qwen2audio", "qwen2audio-mmproj-f32.gguf", 32l)
    }
    tst |> run("voxtral tower: the discrete home's blocksvs the CPU q8 chain on the synthetic mel") @(t : T?) {
        blocks_twin(t, "voxtral", "voxtral-mini-mmproj-f32.gguf", 0l)
    }
    tst |> run("omni-3b audio tower: the discrete home's blocksvs the CPU q8 chain on the synthetic mel") @(t : T?) {
        blocks_twin(t, "omni-3b", "mmproj-omni-3b-f32.gguf", 0l)
    }
    tst |> run("ultravox audio tower: the discrete home's blocksand the gated-MLP tail vs the CPU q8 chain on the synthetic mel") @(t : T?) {
        blocks_twin(t, "ultravox", "mmproj-ultravox-v0_5-llama-3_2-1b-f16.gguf", 0l)
    }
}

//! qwen3a staged and minted in memory under the lane pin, reset after; `zero_block` >= 0 zeroes `poison_blocks` blocks from it on; `dev` the flavor
def private mint_q3a(path : string; q8 : bool; zero_block : int64; dev : TowerDev = TowerDev.none) : Qwen3aTower {
    set_qwen3a_q8(q8)
    defer() {
        reset_qwen3a_q8()
    }
    var inscope stg <- tower_dev_staged(dev) $ { return <- stage_qwen3a_tower(path) }
    if (zero_block >= 0l) {
        let nl = stg.e.core.n_layer
        for (b in range64(zero_block, min(nl, zero_block + poison_blocks(nl)))) {
            if (q8) {
                zero_plane(stg.core.qblob, stg.e.core.qlayers_off + b * stg.e.core.qlayer_stride, stg.e.core.qlayer_stride)
            } elif (stg.e.core.gemm_f16) {
                zero_plane(stg.core.wblob, stg.e.core.qlayers_off + b * stg.e.core.qlayer_stride, stg.e.core.qlayer_stride)
            } else {
                zero_plane(stg.core.fblob, stg.e.core.layers_off + b * stg.e.core.layer_stride, stg.e.core.layer_stride)
            }
        }
    }
    var tw = Qwen3aTower()
    mint_qwen3a_tower(stg, tw, dev)
    return <- tw
}

//! a qwen3a twin leg's two states over one tower
struct Q3aLegState {
    enc : EncoderState
    conv : Qwen3aConvState
}

def private q3a_leg_state(tw : Qwen3aTower) : Q3aLegState {
    var s = Q3aLegState()
    s.enc <- make_encoder_state(tw.core)
    return <- s
}

//! the device leg (the knob on) encodes the device mel's `dev_win`, the CPU legs the CPU mel's `win`
[cold_path]
def private q3a_twin_encode(tw : Qwen3aTower; var s : Q3aLegState; win, dev_win : MelChunk; var out : array<float>) : int64 {
    if (serving_gpu_tower(TowerFamily.qwen3a)) {
        qwen3a_encode(tw, s.enc, s.conv, dev_win)
    } else {
        qwen3a_encode(tw, s.enc, s.conv, win)
    }
    out := s.enc.out
    return long_length(out)
}

//! qwen3a's exact lane on the serving home over window 0 (`exact_flavor_leg`): the CPU image's chain against the home's f16 flavor on the device
[cold_path]
def private q3a_exact_leg(t : T?; path : string; twx : Qwen3aTower; var sx : Q3aLegState; mel, dev_mel : WhisperMel) {
    let home = serving_tower_dev(TowerFamily.qwen3a)
    let tag = "qwen3a jfk mel+front+blocks at 0"
    let win <- qwen3a_mel_window(mel, 0l)
    let dev_win <- qwen3a_mel_window(dev_mel, 0l)
    let n_chunks = dev_win.n_len / QWEN3A_CHUNK
    let convs = exact_flavor_leg(t, tag, TowerFamily.qwen3a, TWIN_EXACT_REL_L2_VK, twx, sx, n_chunks * QWEN3A_TOK_PER_CHUNK,
        $(zb) { return <- mint_q3a(path, false, zb, home) }, $(tw) { return <- q3a_leg_state(tw) }) $(e; var s; var out) => q3a_twin_encode(e, s, win, dev_win, out)
    t |> equal(convs, 2l * n_chunks, "{tag} exact lane: the front served every chunk of both device encodes")
}

//! the Vulkan storage-range seam over the qwen3a mel: a range its scratch runs past declines `memory` and keeps no capacity, so the next mel rebuilds
[unused_argument(t, samples, n_mel, mel, dev_mel)]   // without dasVulkan the gated body is empty
def private q3a_mel_range_legs(t : T?; samples : array<float>; n_mel : int64; mel, dev_mel : WhisperMel) {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        return if (tower_family_cuda(TowerFamily.qwen3a))
        vulkan_tower_shutdown()
        let msr = g_gpu.msr
        let k0 = vulkan_tower_declines()?["memory"] ?? 0l
        g_gpu.msr = 4096l
        let cpu_mel <- log_mel_qwen3a(samples, n_mel)
        g_gpu.msr = msr
        t |> equal((vulkan_tower_declines()?["memory"] ?? 0l) - k0, 1l, "qwen3a jfk mel: the scratch past the range declined memory")
        check_exact(t, "qwen3a jfk mel: the declined mel is the CPU's", cpu_mel.data, mel.data)
        let m3 = vulkan_tower_stats()
        let re_mel <- log_mel_qwen3a(samples, n_mel)
        t |> equal(vulkan_tower_stats().mels - m3.mels, 1l, "qwen3a jfk mel: the range put back, the device mel served on a rebuilt scratch")
        check_exact(t, "qwen3a jfk mel: the rebuilt scratch's rows are the first device mel's", re_mel.data, dev_mel.data)
    }
}

[test(description = "Qwen3-ASR on the discrete home computes the jfk log-mel within 1e-3 of the CPU mel, then serves front, blocks and projector on windows 0 and the tail within the three-way bar, a poison and the exact lane holding theirs")]
def test_qwen3a_vulkan_front(tst : T?) {
    tst |> run("qwen3a: the discrete home's mel + conv front + blocks vs the CPU q8 chain on jfk windows 0 and the tail") @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan) || typeinfo builtin_module_exists(cuda)) {
            let path = path_join(models_dir(), "mmproj-Qwen3-ASR-0.6B-bf16.gguf")
            jfk_twin_cell(t, path, true, TowerFamily.qwen3a) $(samples) {
                let home = serving_tower_dev(TowerFamily.qwen3a)
                var twx <- mint_q3a(path, false, -1l)
                var tw <- mint_q3a(path, true, -1l)
                t |> success(!twx.core.q8 && tw.core.q8 && tw.cq8, "qwen3a jfk: the exact and the q8 towers minted, the cblob trio on the q8 lane")
                set_serving_gpu_tower(false, TowerFamily.qwen3a)
                let m0 = gpu_tower_counters(TowerFamily.qwen3a)
                let mel <- log_mel_qwen3a(samples, tw.core.n_mel)
                let m1 = gpu_tower_counters(TowerFamily.qwen3a)
                t |> equal(m1.mels - m0.mels, 0l, "qwen3a jfk mel: the knob-off mel ran on the CPU (mels delta)")
                set_serving_gpu_tower(true, TowerFamily.qwen3a)
                let dev_mel <- log_mel_qwen3a(samples, tw.core.n_mel)
                let m2 = gpu_tower_counters(TowerFamily.qwen3a)
                t |> equal(m2.mels - m1.mels, 1l, "qwen3a jfk mel: the device mel served (mels delta)")
                t |> equal(m2.declines, m1.declines, "qwen3a jfk mel: the device mel served with no decline")
                t |> equal(dev_mel.n_len, mel.n_len, "qwen3a jfk mel: the device mel has the CPU mel's frames")
                let mel_maxd = logits_maxdiff(dev_mel.data, mel.data)
                t |> success(mel_maxd < 1e-3, "qwen3a jfk mel: the device log-mel matches the CPU's within 1e-3 (max |diff| {mel_maxd})")
                q3a_mel_range_legs(t, samples, tw.core.n_mel, mel, dev_mel)
                var twd <- mint_q3a(path, true, -1l, home)
                var sx <- q3a_leg_state(twx)
                var sq <- q3a_leg_state(tw)
                var sd <- q3a_leg_state(twd)
                for (off in [0l, QWEN3A_WINDOW]) {
                    let tag = "qwen3a jfk mel+front+blocks at {off}"
                    let win <- qwen3a_mel_window(mel, off)
                    let dev_win <- qwen3a_mel_window(dev_mel, off)
                    let n_chunks = dev_win.n_len / QWEN3A_CHUNK
                    let spec = TwinSpec(family = "qwen3a", tag = tag, n_layer = tw.core.n_layer, metric = TwinMetric.rel_l2, seat_family = TowerFamily.qwen3a)
                    let tail0 = gpu_tower_tail_encodes(TowerFamily.qwen3a)
                    let r <- twin_three_way(t, spec, tw, sq, twd, sd, twx, sx, $(e; var s; var out) => q3a_twin_encode(e, s, win, dev_win, out)) $(legs) {
                        if (legs.dev.convs == legs.pre.convs) {
                            to_log(LOG_INFO, "{tag}: the driver declined - {gpu_tower_declines(TowerFamily.qwen3a)}\n")
                        }
                        front_served(t, tag, legs.dev.convs - legs.pre.convs, n_chunks, legs.dev.declines - legs.cpu.declines)
                        t |> equal(legs.dev.encodes - legs.pre.encodes, 1l, "{tag}: the block loop served the window")
                    }
                    t |> equal(gpu_tower_tail_encodes(TowerFamily.qwen3a) - tail0, 1l,
                        "{tag}: the projector ran on the device behind the device leg's blocks and on neither knob-off leg (tail encodes delta)")
                    if (off == 0l && r.bar > 0.0lf) {
                        var twp <- mint_q3a(path, true, tw.core.n_layer / 2l, home)
                        var sp <- q3a_leg_state(twp)
                        twin_poison(t, r, tw.core.n_layer / 2l, twp, sp) $(e; var s; var out) => q3a_twin_encode(e, s, win, dev_win, out)
                        delete sp
                        delete twp
                    }
                }
                delete sd
                delete sq
                delete twd
                delete tw
                q3a_exact_leg(t, path, twx, sx, mel, dev_mel)
                delete sx
                delete twx
                gpu_tower_shutdown(TowerFamily.qwen3a)
            }
        } else {
            t |> skip("no discrete GPU home in this build (dasVulkan, dasCUDA)")
        }
    }
}

//! the Metal home's chat tower on one synthetic mel, one cell a projector kind: the q8 lane three ways (`twin_three_way`: the
//! exact CPU image the reference, the CPU q8 image, the q8 lane's Metal image with the projector tail behind its blocks) with
//! its poison, then the exact lane on its f16 Metal image (`blocks_exact_leg`); `layers` > 0 pins the depth
[cold_path, unused_argument(file, kind, layers)]   // off-Apple the cell skips
def private metal_tower_chain_cell(t : T?; file : string; kind : AudioProjKind; layers : int64) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let path = path_join(models_dir(), file)
        return if (!jit_model_available(t, path) || !metal_twin_seat(t, TowerFamily.whisper))
        let tag = "{file} metal synthetic mel"
        with_gpu_knobs() {
            set_metal_tower(true)
            var twx <- mint_aud(path, false, -1l)
            var tw <- mint_aud(path, true, -1l)
            var twd <- mint_aud(path, true, -1l, TowerDev.metal)
            t |> success(tw.proj_kind == kind, "{tag}: the projector kind the cell names")
            if (layers > 0l) {
                t |> equal(tw.n_layer, layers, "{tag}: the tower's depth is the {layers} blocks the cell names")
            }
            t |> success(!twx.q8 && tw.q8 && twd.q8 && twd.dev_backend == TowerDev.metal, "{tag}: the exact and the q8 towers minted, the q8 lane's Metal image beside")
            let mel <- synth_mel(tw.n_mel)
            var encx <- make_encoder_state(twx)
            var enc <- make_encoder_state(tw)
            var encd <- make_encoder_state(twd)
            let spec = TwinSpec(family = file, tag = tag, n_layer = tw.n_layer, metric = TwinMetric.rel_l2, seat_family = TowerFamily.whisper)
            let tail0 = metal_tower_tail_encodes()
            let f0 = metal_tower_f16_encodes()
            let r <- twin_three_way(t, spec, tw, enc, twd, encd, twx, encx, $(e; var s; var out) => aud_twin_encode(e, s, mel, out)) $(legs) {
                t |> equal(legs.dev.encodes - legs.pre.encodes, 1l, "{tag}: exactly the device leg dispatched")
                t |> equal(legs.dev.blocks - legs.pre.blocks, tw.n_layer, "{tag}: blocks delta = n_layer")
                t |> equal(legs.dev.rows - legs.pre.rows, tw.n_ctx, "{tag}: rows delta = n_ctx")
            }
            t |> equal(metal_tower_tail_encodes() - tail0, 1l, "{tag}: the q8 projector tail rode the device behind the device leg's blocks")
            t |> equal(metal_tower_f16_encodes() - f0, 0l, "{tag}: the q8 lane read the q8 slab, not the halfword plane")
            delete encd
            delete twd
            if (r.bar > 0.0lf) {
                var twp <- mint_aud(path, true, tw.n_layer / 2l, TowerDev.metal)
                var encp <- make_encoder_state(twp)
                twin_poison(t, r, tw.n_layer / 2l, twp, encp) $(e; var s; var out) => aud_twin_encode(e, s, mel, out)
                delete encp
                delete twp
            }
            delete enc
            delete tw
            blocks_exact_leg(t, tag, path, twx, encx, mel)
            delete encx
            delete twx
            metal_tower_shutdown()
        }
    } else {
        t |> skip("das_metal not present")
    }
}

[test(description = "The qwen2audio, voxtral, omni-3b and ultravox towers on Metal run every block and the projector tail on the q8 slab within the three-way bar on a synthetic mel, a poison exceeds it, and the f16 exact lane holds")]
def test_encoder_blocks_gpu(tst : T?) {
    tst |> run("qwen2audio tower: the Metal chain vs the CPU encoder, 32 layers") @(t : T?) {
        metal_tower_chain_cell(t, "qwen2audio-mmproj-f32.gguf", AudioProjKind.qwen2a, 32l)
    }
    tst |> run("voxtral tower: the Metal chain vs the CPU encoder - pair pool, post-norm, stacked GELU MLP") @(t : T?) {
        metal_tower_chain_cell(t, "voxtral-mini-mmproj-f32.gguf", AudioProjKind.voxtral, 0l)
    }
    tst |> run("omni-3b tower: the Metal chain vs the CPU encoder - pair pool, post-norm, one biased linear") @(t : T?) {
        metal_tower_chain_cell(t, "mmproj-Qwen2.5-Omni-3B-f16.gguf", AudioProjKind.qwen2a, 0l)
    }
    tst |> run("ultravox tower: the Metal chain vs the CPU encoder - post-norm, stacked rms, gated MLP") @(t : T?) {
        metal_tower_chain_cell(t, "mmproj-ultravox-v0_5-llama-3_2-1b-f16.gguf", AudioProjKind.ultravox, 0l)
    }
}

//! The chat towers' chunked mel on the device seat against the CPU spectrum: a 128-mel clip's chunk within 1e-3 of the CPU
//! mel, the mel counter up by the device leg alone, an 80-mel call (off the seat's lattice) served by the CPU with the
//! counter unmoved, and a second clip's mel outside the bar (the control).
[test(description = "The chunked 128-mel log-mel on the Metal mel seat lands within 1e-3 of the CPU mel with one device mel counted, an 80-mel call falls to the CPU, and another clip mel lands outside the bar")]
def test_log_mel_chunks_gpu(t : T?) {
    t |> run("the chunked whisper mel rides the device mel seat") @(t : T?) {
        static_if (typeinfo builtin_module_exists(das_metal)) {
            return if (!jit_only(t))
            var inscope samples <- [for (i in range(16000 * 5)); 0.3 * sin(float(i) * 0.05) + 0.1 * sin(float(i) * 0.31)]
            var inscope other <- [for (i in range(16000 * 5)); 0.3 * sin(float(i) * 0.11)]
            let was = metal_tower_knob()
            with_dasllama_jobque_() {
                set_metal_tower(false)
                let m0 = metal_tower_mel_encodes()
                var inscope cpu <- log_mel_chunks(samples, 128l)
                t |> equal(metal_tower_mel_encodes() - m0, 0l, "tower off: the CPU spectrum")
                set_metal_tower(true)
                var inscope dev <- log_mel_chunks(samples, 128l)
                t |> equal(metal_tower_mel_encodes() - m0, 1l, "tower on: one device mel a clip")
                var inscope dev2 <- log_mel_chunks(other, 128l)
                var inscope m80 <- log_mel_chunks(samples, 80l)
                t |> equal(metal_tower_mel_encodes() - m0, 2l, "an 80-mel call is off the seat's lattice: the CPU serves it")
                t |> success(length(cpu) == 1 && length(dev) == 1 && length(m80) == 1, "one chunk each")
                if (length(cpu) == 1 && length(dev) == 1 && length(dev2) == 1) {
                    let d = logits_maxdiff(cpu[0].data, dev[0].data)
                    let ctl = logits_maxdiff(cpu[0].data, dev2[0].data)
                    t |> success(d <= 1e-3, "device mel within 1e-3 of the CPU mel (max diff {d})")
                    t |> success(ctl > 1e-3, "another clip's mel lands outside the bar (max diff {ctl}) - the control")
                }
            }
            set_metal_tower(was)
        }
    }
}

[test(description = "The Qwen2.5-Omni 3B mmproj loads its audio side as a qwen2a tower (proj_dim 2048, 32 layers, 750 tokens a chunk) with the vision tensors skipped")]
def test_omni_tower(t : T?) {
    t |> run("qwen2.5o mmproj: audio side is qwen2a verbatim, vision tensors skipped") @(t : T?) {
        let path = path_join(models_dir(), "mmproj-omni-3b-f32.gguf")
        if (!model_available(t, path)) return
        var tw <- load_audio_tower(path)   // loading at all proves the v.* tensors are skipped
        t |> success(tw.proj_kind == AudioProjKind.qwen2a, "kind aliases qwen2a")
        t |> equal(tw.proj_dim, 2048l, "proj_dim (3B decoder)")
        t |> equal(tw.n_layer, 32l, "layers")
        t |> equal(audio_tokens_per_chunk(tw), 750l, "tokens per 30 s chunk")
        delete tw   // image-backed planes are locked views — only the finalizer may release them
    }
}

[test(description = "The qwen2audio f32 tower has the pinned shape, and its CPU encode of an all-ones mel matches the llama-mtmd-debug token 0 values within 5e-3 and the mean and max of the output")]
def test_encoder_oracle(t : T?) {
    t |> run("all-ones mel through the tower matches mtmd (needs qwen2audio mmproj)") @(t : T?) {
        let path = path_join(models_dir(), "qwen2audio-mmproj-f32.gguf")
        return if (!jit_model_available(t, path))
        var tw <- load_cpu_audio_tower(path)
        t |> equal(tw.n_mel, 128l, "n_mel")
        t |> equal(tw.d_model, 1280l, "d_model")
        t |> equal(tw.n_head, 20l, "heads")
        t |> equal(tw.n_layer, 32l, "layers")
        t |> equal(tw.n_ff, 5120l, "ffn")
        t |> equal(tw.n_ctx, 1500l, "positions")
        t |> equal(tw.proj_dim, 4096l, "projection")
        var mel = MelChunk(n_mel = tw.n_mel, n_len = AUDIO_CHUNK_FRAMES)
        mel.data |> resize(int(tw.n_mel * AUDIO_CHUNK_FRAMES))
        for (i in range(length(mel.data))) {
            mel.data[i] = 1.0
        }
        var enc <- make_encoder_state(tw)
        with_gpu_knobs() {
            set_every_gpu_tower(false)
            with_dasllama_jobque_() {
                audio_encode(tw, enc, mel)
            }
        }
        // llama-mtmd-debug -p encode --audio one -n 3000 (CPU): token 0 opens
        // 0.251974 0.311756 0.556828 0.005232 -0.838854; stats mean=0.009261 max=7.527778.
        // fp32 CPU re-orders put us ~1e-3 off their flash-attn encoder; gate at 5e-3.
        t |> success(abs(enc.out[0] - 0.251974) < 5e-3, "tok0[0]")
        t |> success(abs(enc.out[1] - 0.311756) < 5e-3, "tok0[1]")
        t |> success(abs(enc.out[2] - 0.556828) < 5e-3, "tok0[2]")
        t |> success(abs(enc.out[4] + 0.838854) < 5e-3, "tok0[4]")
        var sum = 0.0lf
        var mx = enc.out[0]
        for (v in enc.out) {
            sum += double(v)
            mx = max(mx, v)
        }
        let mean = sum / double(750l * tw.proj_dim)
        t |> success(abs(float(mean) - 0.009261) < 5e-4, "mean")
        t |> success(abs(mx - 7.527778) < 5e-2, "max")
        delete tw   // image-backed planes are locked views — only the finalizer may release them
    }
}

// Model-free: encoder-state sizing on a whole-clip tower — tt = the clip's frames, so at
// minutes scale the tt- and 4·tt·d-sized buffers cross the 64 MB max_unreserved_size guard
// and make_encoder_state must reserve each exact size (a bare resize panics here).
[test(description = "make_encoder_state for a 16384-frame whole-clip tower reserves x, hp and h at their exact sizes past the max_unreserved_size guard without panicking")]
def test_encoder_state_sizing(t : T?) {
    t |> run("whole-clip encoder state clears max_unreserved_size") @(t : T?) {
        return if (!jit_only(t))
        let tw = AudioTower(n_mel = 128l, d_model = 1088l, n_head = 8l, n_layer = 1l,
            n_ff = 1024l, n_ctx = 16384l, proj_dim = 0l,
            conv1_w_off = 0l, conv1_b_off = 0l, conv2_w_off = 0l, conv2_b_off = 0l,
            pos_off = 0l, layers_off = 0l, layer_stride = 0l, lnp_w_off = 0l, lnp_b_off = 0l,
            proj_w_off = 0l, proj_b_off = 0l)
        var s <- make_encoder_state(tw)
        t |> equal(long_length(s.x), tw.n_ctx * tw.d_model)
        t |> equal(long_length(s.hp), 4l * tw.n_ctx * tw.d_model)
        t |> equal(long_length(s.h), tw.n_ctx * tw.n_ff)
        delete s
    }
}

let private PK_DEVICE_PLOG_BAR = 1e-4   //!< the device decode's plog off the CPU's: float log-sums over a recurrence of several hundred tokens, 2.3e-5 measured on gb1

//! one parakeet clip decoded on the CPU loop and on the device seat (`rnnt_device_twin`) at `PK_DEVICE_PLOG_BAR`: `m` the lane's
//! GPU-flavored model, `mc` the CPU image whose projection the device runs (the exact image where `m` keeps the joint's half f32)
[cold_path]
def private pk_device_decode_clip(t : T?; m, mc : ParakeetModel; samples : array<float>; tag : string) {
    return if (rnnt_seat_skip(t, tag))
    with_gpu_knobs() {
        set_serving_gpu_tower(true, TowerFamily.parakeet)
        var st = ParakeetState()
        var stc = ParakeetState()
        parakeet_log_mel(samples, m, st)
        let tt = parakeet_encode(m, st)
        let projected = m.q8 && !m.jenc_exact ? 0l : 1l
        var inscope tw <- rnnt_device_twin(t, "test_audio: {tag}", PK_DEVICE_PLOG_BAR, projected, projected,
                st.x, $(var toks) {
            stc.x := st.x
            parakeet_tdt_decode(mc, stc, tt)
            toks := stc.toks
        }) $(var toks) {
            parakeet_tdt_decode(m, st, tt)
            toks := st.toks
        }
        to_log(LOG_INFO, "test_audio: {tag} cpu   : {parakeet_detokenize(m, tw.cpu)}\n")
        to_log(LOG_INFO, "test_audio: {tag} device: {parakeet_detokenize(m, tw.dev)}\n")
        delete stc
        delete st
    }
}

[test(description = "The parakeet v2 and v3 TDT decode on the device seat, both lanes, matches the CPU decoder tokens with log-probs within PK_DEVICE_PLOG_BAR on jfk and gb1, and encoder rows scaled 1.05x move them past it")]
def test_parakeet_device_decode_twin(tst : T?) {
    with_dasllama_jobque_() {
        for (v in ["v2", "v3"]) {
            let path = path_join(whisper_dir(), "ggml-parakeet-tdt-0.6b-{v}-f32.bin")
            continue if (!model_available(tst, path))
            for (q8 in [false, true]) {
                tst |> run("parakeet {v} {q8 ? "q8" : "exact"}: the TDT decode on the device seat is the CPU decoder's") @(t : T?) {
                    if (!serving_tower_device_up(TowerFamily.parakeet)) {
                        t |> skip("parakeet {v}: no GPU device stands up for the serving tower on this build or box")
                        return
                    }
                    let home = serving_tower_dev(TowerFamily.parakeet)
                    var dev = TowerDev.none
                    with_gpu_knobs() {
                        set_serving_gpu_tower(true, TowerFamily.parakeet)
                        dev = parakeet_dev_flavor(q8)
                    }
                    t |> equal(dev, home, "parakeet {v}: the serving tower flavors the lane its chain serves")
                    return if (dev == TowerDev.none)
                    var inscope m <- mint_pk(path, q8, -1l, dev)
                    var inscope mc <- mint_pk(path, q8 && !m.jenc_exact, -1l)
                    for (clip in ["jfk", "gb1"]) {
                        let wav = asr_clip("{clip}.wav")
                        if (!stat(wav).is_valid) {
                            t |> skip("{clip}.wav not present")
                            continue
                        }
                        var inscope samples <- load_audio_16k_mono(wav)
                        pk_device_decode_clip(t, m, mc, samples, "parakeet {v} {q8 ? "q8" : "exact"} {clip}")
                    }
                    gpu_tower_shutdown(TowerFamily.parakeet)
                }
            }
        }
    }
}
