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
require _rnnt_oracle   // rnnt_seat_*: the build's device transducer seat, its pin and counters
require ?vulkan dasllama/dasllama_vulkan_tower
require ?vulkan dasllama/dasllama_vulkan_common   // g_gpu: the storage range the blocks-refused legs stand in
require ?vulkan dasllama/dasllama_vulkan_asr_dec   // vulkan_ctc_stats: the CTC head's device seat
require _ctc_oracle   // nolint:STYLE030,LINT019 — ctc_leg / ctc_cpu_leg / ctc_hard_flips, inside the vulkan arm alone
require daslib/linq   // sequence_equal: the twin legs' ids
require daslib/strings_boost   // split: a kq carrier's members by name
require dasllama/dasllama_kqformat   // kq_fmt_of_slot / kq_qsb: the poisoned kq leg's member planes
require dasllama/dasllama_gemma4a
require dasllama/dasllama_canary
require dasllama/dasllama_qwen3a
require dasllama/dasllama_parakeet
require dasllama/dasllama_gigaam
require dasllama/dasllama_audio_io
require _tower_twin

// Audio tower tests. The mel/gelu/filterbank tests are self-contained; the encoder tests
// need the locally-converted Qwen2-Audio mmproj (2.5 GB, gitignored) and skip without it.
// The GigaAM cells skip without the gigaam-v3-e2e-rnnt-f16.gguf carrier (the CTC head's Vulkan twin without
// gigaam-v3-ctc-f16.gguf / gigaam-v3-e2e-ctc-f16.gguf), under a family filter
// that leaves out `gigaam`, and per clip without benchmarks/asr/corpus/ru_short.wav or ru_mid.wav; their kq lane legs
// skip without gigaam-v3-e2e-rnnt-q4_k.gguf, the mixed-member leg without gigaam-v3-e2e-rnnt-q4_k-mix-local.gguf;
// the Metal ones on a build without das_metal. The parakeet device-decode twin skips without its
// f32 carrier, per clip without jfk.wav or gb1.wav in benchmarks/asr/corpus/, where no GPU device stands up for the
// serving tower (`serving_tower_device_up`), on the lane the build's chain does not flavor (Metal's q8, Vulkan's exact),
// and where the build or the box has no device transducer seat (`rnnt_seat_skip`).
// The oracle constants come from llama-mtmd-debug (CPU backend). The Vulkan twins skip without
// their carriers (the qwen2audio / voxtral / omni-3b f32 mmprojs, the E2B bf16 mmproj, the
// canary f32 encoder, the parakeet f32 encoder, the ultravox f16 mmproj, the Qwen3-ASR bf16 mmproj), without jfk.wav,
// without a Vulkan device under
// DASLLAMA_GPU=1, and on a build with das_metal; the qwen3a front also skips when interpreted.

[test]
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

[test]
def test_hann_window(t : T?) {
    t |> run("periodic hann-400 endpoints and center") @(t : T?) {
        let h <- build_hann_window(400l)
        t |> success(h[0] == 0.0, "h[0] = 0")
        t |> success(abs(h[200] - 1.0) < 1e-7, "h[200] = 1 (periodic peak)")
        t |> success(abs(h[100] - 0.5) < 1e-6, "h[100] = 0.5")
        t |> success(abs(h[399] - h[1]) < 1e-6, "periodic symmetry h[399] = h[1]")
    }
}

[test]
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

[test]
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

[test]
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
        tw <- load_audio_tower(path)
    }
    return <- tw
}

[test]
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

[test]
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

//! `zero_block` >= 0 zeroes the q8 GEMM planes of `poison_blocks` blocks from that block on; `dev` the image flavor (`none` the
//! CPU image, a GPU flavor staged row-major for its driver's bake - `#tower-dev-block`)
def private mint_aud(path : string; q8 : bool; zero_block : int64; dev : TowerDev = TowerDev.none) : AudioTower {
    var inscope stg <- tower_dev_staged(dev) $ { return <- stage_audio_tower(path, q8) }
    if (zero_block >= 0l) {
        for (b in range64(zero_block, min(stg.e.n_layer, zero_block + poison_blocks(stg.e.n_layer)))) {
            zero_plane(stg.qblob, stg.e.qlayers_off + b * stg.e.qlayer_stride, stg.e.qlayer_stride)
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

[cold_path]
def private vulkan_blocks_twin(t : T?; family, file : string; layers_claimed : int64) {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        let path = path_join(models_dir(), file)
        if (!model_available(t, path) || !vulkan_audio_twin_seat(t)) {
            return
        }
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
        let spec = TwinSpec(family = family, tag = tag, n_layer = tw.n_layer, metric = TwinMetric.rel_l2)
        let tail0 = vulkan_tower_tail_encodes()
        var twd <- mint_aud(path, true, -1l, TowerDev.vulkan)
        var encd <- make_encoder_state(twd)
        let r <- twin_three_way(t, spec, tw, enc, twd, encd, twx, encx, $(e; var s; var out) => aud_twin_encode(e, s, mel, out)) $(legs) {
            t |> equal(legs.dev.encodes - legs.pre.encodes, 1l, "{tag}: exactly the device leg dispatched")
            t |> equal(legs.dev.blocks - legs.pre.blocks, tw.n_layer, "{tag}: blocks delta = n_layer")
            t |> equal(legs.dev.rows - legs.pre.rows, tw.n_ctx, "{tag}: rows delta = n_ctx")
        }
        delete encd
        delete twd
        t |> equal(vulkan_tower_tail_encodes() - tail0, 1l, "{tag}: the projector tail rode the device behind the device leg's blocks")
        if (r.bar > 0.0lf) {
            var twp <- mint_aud(path, true, tw.n_layer / 2l, TowerDev.vulkan)
            var encp <- make_encoder_state(twp)
            twin_poison(t, r, tw.n_layer / 2l, twp, encp) $(e; var s; var out) => aud_twin_encode(e, s, mel, out)
            delete encp
            delete twp
        }
        var encx2 <- make_encoder_state(twx)
        exact_lane_declines(t, tag, long_length(r.reference), false) $() : int64 {
            audio_encode(twx, encx2, mel)
            return long_length(encx2.out)
        }
        delete encx2
        delete enc
        delete encx
        delete tw
        delete twx
        vulkan_tower_shutdown()
    } else {
        t |> skip("dasVulkan not present")
    }
}

//! `zero_block` >= 0 zeroes the eleven q8 GEMM planes of `poison_blocks` blocks from that block on; `dev` the image flavor
//! (`none` the CPU image, a GPU flavor staged row-major for its driver's bake - `#tower-dev-block`)
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
                zero_plane(stg.qblob, w, d * d)
            }
            for (w in fixed_array(lo.ff_up, lo.ff_down, lo.ff_up_1, lo.ff_down_1)) {
                zero_plane(stg.qblob, w, d * ff)
            }
            zero_plane(stg.qblob, lo.conv_pw1, d * 2l * d)
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

//! the regrow legs at `r`'s bar - a short encode then the clip on one residency, the clip after a shutdown, then the poison; `exactly_three`: one dispatch an encode, else a chunked encode's several
[cold_path, unused_argument(t, tag, enc, st, samples, r, exactly_three, mint_poisoned, encode)]
def private twin_regrow_legs(t : T?; tag : string; enc : auto(TW); var st : auto(ST); samples : array<float>; r : TwinResult;
                             exactly_three : bool; mint_poisoned : block<() : TW>;
                             encode : block<(e : TW; var s : ST; clip : array<float>; var out : array<float>) : int64>) {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        let head <- clip_head(samples)
        var dev_head, dev_again, dev_fresh : array<float>
        set_vulkan_tower(true)
        let e0 = vulkan_tower_stats().encodes
        with_dasllama_jobque_() {
            invoke(encode, enc, st, head, dev_head)
            invoke(encode, enc, st, samples, dev_again)
            vulkan_tower_shutdown()
            invoke(encode, enc, st, samples, dev_fresh)
        }
        let n_enc = vulkan_tower_stats().encodes - e0
        if (exactly_three) {
            t |> equal(n_enc, 3l, "{tag}: the short, the regrown and the fresh encodes ran on the device")
        } else {
            t |> success(n_enc >= 3l, "{tag}: the short, the regrown and the fresh encodes ran on the device")
        }
        tower_twin_held(t, "{tag} regrown scratch", r.reference, dev_again, r.bar)
        tower_twin_held(t, "{tag} after shutdown", r.reference, dev_fresh, r.bar)
        var encp <- invoke(mint_poisoned)
        var stp = ST()
        var poisoned : array<float>
        let p0 = vulkan_tower_stats().encodes
        with_dasllama_jobque_() {
            invoke(encode, encp, stp, samples, poisoned)
        }
        t |> success(vulkan_tower_stats().encodes > p0, "{tag} poison: device-served (encodes delta)")
        tower_twin_poisoned(t, tag, r.reference, poisoned, r.bar)
        delete poisoned
        delete stp
        delete encp
        delete dev_head
        delete dev_again
        delete dev_fresh
    }
}

[test, arch(at = "../ARCHITECTURE_GPU_TOWER_VULKAN.md#vk-tower-encode-chains")]
def test_gemma4a_vulkan_twin(tst : T?) {
    tst |> run("gemma4a E2B: the Vulkan Conformer blocks vs the CPU q8 chain on jfk, the exact chain the reference") @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            let mm = path_join(models_dir(), "mmproj-gemma-4-E2B-it-bf16.gguf")
            jfk_twin_cell(t, mm, false) $(samples) {
                var encx <- mint_g4a(mm, false, -1l)
                var enc <- mint_g4a(mm, true, -1l)
                t |> success(!encx.q8 && enc.q8, "the exact and the q8 encoders minted")
                var encd <- mint_g4a(mm, true, -1l, TowerDev.vulkan)
                var stx = Gemma4aState()
                var st = Gemma4aState()
                var std = Gemma4aState()
                var n_exact = 0l
                let spec = TwinSpec(family = "gemma4a", tag = "gemma4a jfk", n_layer = enc.n_layer, metric = TwinMetric.rel_l2)
                let r <- twin_three_way(t, spec, enc, st, encd, std, encx, stx, $(e; var s; var out) => gemma4a_encode_all(e, s, samples, out)) $(legs) {
                    n_exact = legs.n_exact
                    let n_chunks = legs.dev.encodes - legs.pre.encodes
                    t |> equal(legs.n_cpu, legs.n_exact, "gemma4a jfk: the q8 chain's token count")
                    t |> equal(legs.n_dev, legs.n_exact, "gemma4a jfk: the device chain's token count")
                    t |> success(n_chunks >= 1l, "gemma4a jfk: the device served the chunk encodes ({n_chunks})")
                    t |> equal(legs.dev.blocks - legs.pre.blocks, n_chunks * enc.n_layer, "gemma4a jfk: every block of every served encode ran on the driver")
                    front_served(t, "gemma4a jfk", legs.dev.convs - legs.pre.convs, n_chunks, legs.dev.declines - legs.cpu.declines)
                }
                if (r.bar > 0.0lf) {
                    var st2 = Gemma4aState()
                    twin_regrow_legs(t, "gemma4a jfk", encd, st2, samples, r, false, $ { return <- mint_g4a(mm, true, enc.n_layer / 2l, TowerDev.vulkan) }) $(e; var s; clip; var out) => gemma4a_encode_all(e, s, clip, out)
                    delete st2
                }
                delete std
                delete encd
                var stx2 = Gemma4aState()
                var exact2 : array<float>
                exact_lane_declines(t, "gemma4a jfk", n_exact, false) $() : int64 {
                    return gemma4a_encode_all(encx, stx2, samples, exact2)
                }
                delete exact2
                delete stx2
                delete stx
                delete st
                delete enc
                delete encx
                vulkan_tower_shutdown()
            }
        } else {
            t |> skip("dasVulkan not present")
        }
    }
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

//! the Vulkan encode of a Vulkan-flavored model (`run`) with the device's storage range stood in at `msr_at` bytes - past the
//! front's buffers, under the blocks' q8 plane: the front stands down before it runs (no front, mel or encode counted, no memory
//! decline) and the family refuses the CPU chain - a Vulkan-flavored image never runs on the CPU (`#tower-dev-block`)
[unused_argument(t, tag, msr_at, run)]   // without dasVulkan the gated body is empty
def private blocks_refused_leg(t : T?; tag : string; msr_at : int64; run : block<() : void>) {
    static_if (typeinfo builtin_module_exists(vulkan)) {
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

//! `zero_block` >= 0 zeroes the eleven q8 GEMM planes of `poison_blocks` blocks from that block on; `dev` the image flavor
//! (`none` the CPU image, a GPU flavor staged row-major for its driver's bake - `#tower-dev-block`)
def private mint_cn(path : string; q8 : bool; zero_block : int64; dev : TowerDev = TowerDev.none) : CanaryEncoder {
    var inscope stg <- tower_dev_staged(dev) $ { return <- stage_canary_encoder(path, q8) }
    poison_fc_blocks(stg.qblob, stg.e.n_layer, zero_block, stg.e.d_model, stg.e.n_ff) $(b) => stg.layers[b]
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

[test, arch(at = "../ARCHITECTURE_GPU_TOWER_VULKAN.md#vk-tower-encode-chains")]
def test_canary_vulkan_twin(tst : T?) {
    tst |> run("canary-qwen 2.5B: the Vulkan FastConformer blocks vs the CPU q8 chain on jfk, the exact chain the reference") @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            let path = path_join(models_dir(), "canary-qwen-2.5b-encoder-f32.bin")
            jfk_twin_cell(t, path, false) $(samples) {
                var encx <- mint_cn(path, false, -1l)
                var enc <- mint_cn(path, true, -1l)
                t |> success(!encx.q8 && enc.q8, "the exact and the q8 encoders minted")
                var encd <- mint_cn(path, true, -1l, TowerDev.vulkan)
                var stx = CanaryState()
                var st = CanaryState()
                var std = CanaryState()
                var n_exact = 0l
                let spec = TwinSpec(family = "canary", tag = "canary jfk", n_layer = enc.n_layer, metric = TwinMetric.rel_l2)
                let r <- twin_three_way(t, spec, enc, st, encd, std, encx, stx, $(e; var s; var out) => cn_twin_encode(e, s, samples, out)) $(legs) {
                    n_exact = legs.n_exact
                    t |> equal(legs.n_cpu, legs.n_exact, "canary jfk: the q8 chain's frame count")
                    t |> equal(legs.n_dev, legs.n_exact, "canary jfk: the device chain's frame count")
                    t |> equal(legs.dev.encodes - legs.pre.encodes, 1l, "canary jfk: the device served the encode")
                    t |> equal(legs.dev.blocks - legs.pre.blocks, enc.n_layer, "canary jfk: every block ran on the driver")
                    front_served(t, "canary jfk", legs.dev.convs - legs.pre.convs, 1l, legs.dev.declines - legs.cpu.declines)
                }
                if (r.bar > 0.0lf) {
                    var st2 = CanaryState()
                    twin_regrow_legs(t, "canary jfk", encd, st2, samples, r, true, $ { return <- mint_cn(path, true, enc.n_layer / 2l, TowerDev.vulkan) }) $(e; var s; clip; var out) => cn_twin_encode(e, s, clip, out)
                    delete st2
                }
                delete std
                delete encd
                var stx2 = CanaryState()
                exact_lane_declines(t, "canary jfk", n_exact, false) $() : int64 {
                    canary_log_mel(encx, samples, stx2)
                    return canary_encode(encx, stx2)
                }
                delete stx2
                delete stx
                delete st
                delete enc
                delete encx
                vulkan_tower_shutdown()
            }
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

//! the parakeet model staged and minted in memory on the lane asked; `zero_block` >= 0 zeroes the eleven q8 GEMM planes of
//! `poison_blocks` blocks from that block on
def private mint_pk(path : string; q8 : bool; zero_block : int64; dev : TowerDev = TowerDev.none) : ParakeetModel {
    var inscope stg <- tower_dev_staged(dev) $ { return <- stage_parakeet_model(path, q8) }
    poison_fc_blocks(stg.qblob, stg.e.n_layer, zero_block, stg.e.n_state, 4l * stg.e.n_state) $(b) => pk_fc_layer(pk_layer_offsets(stg.e, b))
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

//! a Vulkan-flavored tower's encode of clips a few mel frames long against its CPU image's: one device encode a clip with no decline, the CPU's row count, the rows within `SHORT_CLIP_BAR`; the control the device's rows of a noise clip outside the bar of the longest clip's CPU rows (a short clip's rows sit near the normalized mel's fixed point)
[cold_path, unused_argument(t, tag, tw, st, twd, std)]   // without dasVulkan the gated body is empty
def private short_clip_cell(t : T?; tag : string; tw : auto(TW); var st : auto(ST); twd : TW; var std : ST;
                            encode : block<(e : TW; var s : ST; clip : array<float>; var out : array<float>) : int64>) {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        var last_cpu : array<float>
        for (n in SHORT_CLIP_SAMPLES) {
            var inscope clip <- [for (i in range64(n)); 0.3 * sin(float(i) * 0.05) + 0.1 * sin(float(i) * 0.31)]
            var cpu, dev : array<float>
            var n_cpu = 0l
            var n_dev = 0l
            var s0 = VtStats()
            var s1 = VtStats()
            var s2 = VtStats()
            with_gpu_knobs() {
                with_dasllama_jobque_() {
                    set_vulkan_tower(true)
                    s0 = vulkan_tower_stats()
                    n_cpu = invoke(encode, tw, st, clip, cpu)
                    s1 = vulkan_tower_stats()
                    n_dev = invoke(encode, twd, std, clip, dev)
                    s2 = vulkan_tower_stats()
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
                set_vulkan_tower(true)
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

[test, arch(at = "../ARCHITECTURE_GPU_TOWER_VULKAN.md#vk-tower-encode-chains")]
def test_vulkan_short_clips(tst : T?) {
    tst |> run("gemma4a E2B: clips of 320 to 1600 samples serve on the Vulkan image as the CPU image reads them") @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            let mm = path_join(models_dir(), "mmproj-gemma-4-E2B-it-bf16.gguf")
            return if (!model_available(t, mm) || !vulkan_audio_twin_seat(t))
            var enc <- mint_g4a(mm, true, -1l)
            var encd <- mint_g4a(mm, true, -1l, TowerDev.vulkan)
            var st = Gemma4aState()
            var std = Gemma4aState()
            short_clip_cell(t, "gemma4a", enc, st, encd, std) $(e; var s; clip; var out) => gemma4a_encode_all(e, s, clip, out)
            delete std
            delete st
            delete encd
            delete enc
            vulkan_tower_shutdown()
        } else {
            t |> skip("dasVulkan not present")
        }
    }
    tst |> run("canary: clips of 320 to 1600 samples serve on the Vulkan image as the CPU image reads them") @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            let path = path_join(models_dir(), "canary-qwen-2.5b-encoder-f32.bin")
            return if (!model_available(t, path) || !vulkan_audio_twin_seat(t))
            var enc <- mint_cn(path, true, -1l)
            var encd <- mint_cn(path, true, -1l, TowerDev.vulkan)
            var st = CanaryState()
            var std = CanaryState()
            short_clip_cell(t, "canary", enc, st, encd, std) $(e; var s; clip; var out) => cn_twin_encode(e, s, clip, out)
            delete std
            delete st
            delete encd
            delete enc
            vulkan_tower_shutdown()
        } else {
            t |> skip("dasVulkan not present")
        }
    }
    tst |> run("parakeet-TDT 0.6B v2: clips of 320 to 1600 samples serve on the Vulkan image as the CPU image reads them") @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            let path = path_join(whisper_dir(), "ggml-parakeet-tdt-0.6b-v2-f32.bin")
            return if (!model_available(t, path) || !vulkan_audio_twin_seat(t))
            var m <- mint_pk(path, true, -1l)
            var md <- mint_pk(path, true, -1l, TowerDev.vulkan)
            var st = ParakeetState()
            var std = ParakeetState()
            short_clip_cell(t, "parakeet", m, st, md, std) $(e; var s; clip; var out) => pk_twin_encode(e, s, clip, out)
            delete std
            delete st
            delete md
            delete m
            vulkan_tower_shutdown()
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

[test, arch(at = "../ARCHITECTURE_GPU_TOWER_VULKAN.md#vk-tower-encode-chains")]
def test_parakeet_vulkan_twin(tst : T?) {
    tst |> run("parakeet-TDT 0.6B v2: the Vulkan FastConformer front and blocks vs the CPU q8 chain on jfk, the exact chain the reference") @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            let path = path_join(whisper_dir(), "ggml-parakeet-tdt-0.6b-v2-f32.bin")
            jfk_twin_cell(t, path, false) $(samples) {
                var mx <- mint_pk(path, false, -1l)
                var m <- mint_pk(path, true, -1l)
                var md <- mint_pk(path, true, -1l, TowerDev.vulkan)
                t |> success(!mx.q8 && m.q8 && md.dev_backend == TowerDev.vulkan, "the exact and the q8 models minted, the q8 lane's Vulkan image beside")
                var stx = ParakeetState()
                var st = ParakeetState()
                var std = ParakeetState()
                var n_exact = 0l
                let spec = TwinSpec(family = "parakeet", tag = "parakeet jfk", n_layer = m.n_layer, metric = TwinMetric.rel_l2)
                let r <- twin_three_way(t, spec, m, st, md, std, mx, stx, $(e; var s; var out) => pk_twin_encode(e, s, samples, out)) $(legs) {
                    n_exact = legs.n_exact
                    t |> equal(legs.n_cpu, legs.n_exact, "parakeet jfk: the q8 chain's frame count")
                    t |> equal(legs.n_dev, legs.n_exact, "parakeet jfk: the device chain's frame count")
                    t |> equal(legs.dev.encodes - legs.pre.encodes, 1l, "parakeet jfk: the device served the encode")
                    t |> equal(legs.dev.blocks - legs.pre.blocks, m.n_layer, "parakeet jfk: every block ran on the driver")
                    front_served(t, "parakeet jfk", legs.dev.convs - legs.pre.convs, 1l, legs.dev.declines - legs.cpu.declines)
                }
                if (r.bar > 0.0lf) {
                    var st_rf = ParakeetState()
                    var out_rf : array<float>
                    blocks_refused_leg(t, "parakeet jfk", 256l << 20l) $ {
                        pk_twin_encode(md, st_rf, samples, out_rf)
                    }
                    delete st_rf
                    var mp <- mint_pk(path, true, m.n_layer / 2l, TowerDev.vulkan)
                    var stp = ParakeetState()
                    twin_poison(t, r, m.n_layer / 2l, mp, stp) $(e; var s; var out) => pk_twin_encode(e, s, samples, out)
                    delete stp
                    delete mp
                }
                var stx2 = ParakeetState()
                exact_lane_declines(t, "parakeet jfk", n_exact, false) $() : int64 {
                    parakeet_log_mel(samples, mx, stx2)
                    return parakeet_encode(mx, stx2)
                }
                delete stx2
                delete stx
                delete st
                delete std
                delete md
                delete m
                delete mx
                vulkan_tower_shutdown()
            }
        } else {
            t |> skip("dasVulkan not present")
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

[test]
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

[test, arch(at = "../ARCHITECTURE_GPU_TOWER_FC.md#tower-fc-rotary")]
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
                    t |> equal(gigaam_serves_q8(), !gigaam_gpu_exact_serves(), "the tower on: the un-pinned lane follows the driver's faster lane")
                    t |> success(!gigaam_serves_q8(), "the tower on: the un-pinned lane is the f16 planes")
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

[test]
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

//! the GigaAM model staged and minted in memory on the lane asked; `zero_block` >= 0 zeroes the ten GEMM planes of
//! `poison_blocks` blocks from that block on - the q8 quants, or on the exact lane the halfwords
def private mint_ga(path : string; q8 : bool; zero_block : int64; dev : TowerDev = TowerDev.none) : GigaamModel {
    var inscope stg <- tower_dev_staged(dev) $ { return <- stage_gigaam_model(path, q8, zero_block >= 0l) }
    if (stg.e.kq_lane) {
        poison_ga_kq_blocks(stg, zero_block)
    } elif (q8) {
        poison_fc_blocks(stg.qblob, stg.e.n_layer, zero_block, stg.e.d_model, stg.e.n_ff) $(b) => gigaam_layer_offsets(stg.e, b)
    } else {
        poison_fc_blocks(stg.wblob, stg.e.n_layer, zero_block, stg.e.d_model, stg.e.n_ff) $(b) => gigaam_layer_offsets(stg.e, b)
    }
    var m = GigaamModel()
    mint_gigaam_model(stg, m, q8, dev)
    return <- m
}

//! a held kq-lane staging's block GEMMs zeroed from block `zero_block` on (none where it is negative): every K-quant seat's quant
//! bytes and every Q8_0 seat's quants, found through the plan's jobs
def private poison_ga_kq_blocks(var stg : GigaamStaging; zero_block : int64) {
    return if (zero_block < 0l)
    for (j in stg.plan.jobs) {
        continue if (!(j.plane == GigaamPlane.kq || (j.plane == GigaamPlane.q8 && j.fmt >= 0)))
        let layer = to_int64(slice(j.name, length("encoder.layers."), find(j.name, ".", length("encoder.layers."))), false)
        continue if (layer < zero_block)
        if (j.plane == GigaamPlane.kq) {
            let f = kq_fmt_of_slot(j.fmt)
            for (i in range64((j.n / kq_elems(f)) * kq_qsb(f))) {
                stg.kq[j.off + i] = 0u8
            }
        } else {
            for (i in range64(j.n)) {
                stg.qblob[j.off + i] = int8(0)
            }
        }
    }
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
def private off_device_declines() : int64 {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        return (vulkan_tower_declines()?["device"] ?? 0l) + (vulkan_tower_declines()?["flavor"] ?? 0l)
    } else {
        return 0l
    }
}

def private ga_twin_encode(m : GigaamModel; var st : GigaamState; clip : array<float>; var out : array<float>) : int64 {
    return x_rows_encode(st, m.d_model, out) $(var s) {
        return gigaam_encode_span(clip, 0l, long_length(clip), m, s)
    }
}

//! the Vulkan device log-mel alone (`vulkan_gigaam_mel`, the encode chain's mel record) against `gigaam_log_mel` at
//! ``GA_MEL_BAR``, the frame count and the counter beside it, the bar's poisoned element the control
[unused_argument(t, m, samples, clip)]   // without dasVulkan the gated body is empty
def private ga_vk_mel_twin_cell(t : T?; m : GigaamModel; samples : array<float>; clip : string) {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        var st = GigaamState()
        with_dasllama_jobque_() {
            gigaam_log_mel(samples, m, st)
        }
        var inscope dev : array<float>
        let s0 = vulkan_tower_stats()
        let n = vulkan_gigaam_mel(m, samples, dev)
        t |> equal(n, st.n_len, "{clip}: the Vulkan device mel's frame count")
        t |> equal(vulkan_tower_stats().mels - s0.mels, 1l, "{clip}: the Vulkan mel seat ran on the device")
        check_rows(t, "gigaam {clip} vulkan: the device log-mel tracks the CPU's", dev, st.mel, Bar(rel = GA_MEL_BAR, poison_scaled = true), int(m.n_mel))
        var inscope loud <- [for (v in samples); v * 1.1]
        var inscope dev_loud : array<float>
        vulkan_gigaam_mel(m, loud, dev_loud)
        check_exceeds(t, "gigaam {clip} vulkan: the control - the clip at 1.1x on the device side off the CPU's log-mel", dev_loud, st.mel,
            Bar(rel = GA_MEL_BAR))
        delete st
    }
}

//! one clip's Vulkan legs: the three-way twin (exact, CPU q8, device over the q8 lane's Vulkan image `md`), the CPU q8 and device
//! transcripts, the device's range refusal, the poison.
[cold_path, unused_argument(t, path, mx, m, md, samples, clip)]   // without dasVulkan the gated body is empty
def private ga_vk_clip_cell(t : T?; path : string; mx, m, md : GigaamModel; samples : array<float>; clip : string) {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        var stx = GigaamState()
        var st = GigaamState()
        var std = GigaamState()
        let tag = "gigaam {clip}"
        let spec = TwinSpec(family = "gigaam", tag = tag, n_layer = m.n_layer, metric = TwinMetric.rel_l2)
        let r <- twin_three_way(t, spec, m, st, md, std, mx, stx, $(e; var s; var out) => ga_twin_encode(e, s, samples, out)) $(legs) {
            t |> equal(legs.n_cpu, legs.n_exact, "{tag}: the q8 chain's frame count")
            t |> equal(legs.n_dev, legs.n_exact, "{tag}: the device chain's frame count")
            t |> equal(legs.dev.encodes - legs.pre.encodes, 1l, "{tag}: the device served the encode")
            t |> equal(legs.dev.blocks - legs.pre.blocks, m.n_layer, "{tag}: every block ran on the driver")
            front_served(t, tag, legs.dev.convs - legs.pre.convs, 1l, legs.dev.declines - legs.cpu.declines)
            t |> equal(legs.dev.mels - legs.pre.mels, legs.dev.convs - legs.pre.convs, "{tag}: the log-mel ran on the device wherever the front did")
        }
        ga_vk_mel_twin_cell(t, md, samples, clip)
        delete stx
        delete st
        delete std
        with_gpu_knobs() {
            with_dasllama_jobque_() {
                set_vulkan_tower(false)
                var inscope cpu <- ga_leg(m, samples)
                set_vulkan_tower(true)
                let k4_0 = kq_tile_count("k4")
                var inscope dev <- ga_leg(md, samples)
                t |> equal(kq_tile_count("k4") - k4_0, 0l, "{tag}: the q8 lane dispatches no k4 tile (the kq lane counter's control)")
                to_log(LOG_INFO, "test_audio: gigaam {clip} vulkan vs cpu q8: rel_l2 {rel_l2(dev.x, cpu.x)}\n")
                to_log(LOG_INFO, "test_audio: gigaam {clip} cpu q8: {cpu.text}\n")
                to_log(LOG_INFO, "test_audio: gigaam {clip} vulkan: {dev.text}\n")
                t |> equal(dev.text, cpu.text, "{tag}: the device chain's transcript is the CPU q8 chain's")
                blocks_refused_leg(t, tag, 64l << 20l) $ {
                    var inscope refused <- ga_leg(md, samples)
                }
            }
        }
        if (r.bar > 0.0lf) {
            var mp <- mint_ga(path, true, m.n_layer / 2l, TowerDev.vulkan)
            var stp = GigaamState()
            twin_poison(t, r, m.n_layer / 2l, mp, stp) $(e2; var s; var out) => ga_twin_encode(e2, s, samples, out)
            delete stp
            delete mp
        }
    }
}

//! GigaAM's exact lane on the Vulkan chain (the f16 GEMM over the halfwords) against the CPU exact chain: every stage on the device with no `quant_mode` decline, the CPU's frames and ids, the rows nearer the exact chain than the CPU q8 chain's, the zeroed-blocks control past that bar; off the f16 feed the lane loads its CPU image (`mxd` the exact lane's flavor on this box)
[cold_path, unused_argument(t, path, mx, mxd, mq, samples, clip)]   // without dasVulkan the gated body is empty
def private ga_vk_exact_cell(t : T?; path : string; mx, mxd, mq : GigaamModel; samples : array<float>; clip : string) {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        let tag = "gigaam {clip} exact lane"
        t |> success(!mx.q8 && mx.gemm_f16, "{tag}: the block GEMMs serve off the halfword plane")
        with_gpu_knobs() {
            with_dasllama_jobque_() {
                set_vulkan_tower(false)
                var inscope ex <- ga_leg(mx, samples)
                var inscope q8 <- ga_leg(mq, samples)
                set_vulkan_tower(true)
                let s0 = vulkan_tower_stats()
                let dq0 = vulkan_tower_declines()?["quant_mode"] ?? 0l
                let dd0 = off_device_declines()
                var inscope dev <- ga_leg(mxd, samples)
                let s1 = vulkan_tower_stats()
                t |> equal((vulkan_tower_declines()?["quant_mode"] ?? 0l) - dq0, 0l, "{tag}: no quant_mode decline")
                t |> equal(dev.n, ex.n, "{tag}: the frame count")
                if (!vulkan_tower_front_route()) {
                    t |> equal(s1.encodes - s0.encodes, 0l, "{tag}: off the f16 GEMM feed no encode on the device")
                    t |> success(off_device_declines() > dd0, "{tag}: off the f16 GEMM feed the chain declined the lane")
                    return
                }
                device_leg_served(t, tag, s0, s1, mx.n_layer, -1l)
                t |> equal(s1.convs - s0.convs, 1l, "{tag}: the conv front ran on the device")
                t |> equal(dev.ids, ex.ids, "{tag}: the token ids are the CPU exact chain's")
                to_log(LOG_INFO, "test_audio: {tag} cpu exact: {ex.text}\n")
                to_log(LOG_INFO, "test_audio: {tag} vulkan   : {dev.text}\n")
                var q8_ex = TowerParityStats()
                var dev_ex = TowerParityStats()
                if (tower_parity_stats(t, "{tag} cpu q8 vs exact", ex.x, q8.x, q8_ex) && tower_parity_stats(t, "{tag} vulkan vs exact", ex.x, dev.x, dev_ex)) {
                    let bar = q8_ex.rel_l2
                    to_log(LOG_INFO, "test_audio: {tag}: rel_l2 from the exact chain - vulkan exact lane {dev_ex.rel_l2}, the bar (cpu q8) {bar}\n")
                    t |> success(dev_ex.rel_l2 <= bar, "{tag}: the device rows sit nearer the exact chain than the CPU q8 chain's ({dev_ex.rel_l2} vs {bar})")
                    var mp <- mint_ga(path, false, mx.n_layer / 2l, TowerDev.vulkan)
                    let p0 = vulkan_tower_stats()
                    var inscope bad <- ga_leg(mp, samples)
                    t |> equal(vulkan_tower_stats().encodes - p0.encodes, 1l, "{tag} poison: device-served (encodes delta)")
                    var bad_ex = TowerParityStats()
                    if (tower_parity_stats(t, "{tag} poisoned vulkan vs exact", ex.x, bad.x, bad_ex)) {
                        t |> success(is_finite(bad_ex.rel_l2) && bad_ex.rel_l2 > bar, "{tag}: zeroed halfword blocks through the device chain EXCEED the bar ({bad_ex.rel_l2} vs {bar})")
                    }
                    delete mp
                }
            }
        }
    }
}

//! the Vulkan build's GigaAM lane policy: with the tower on, the lane knob off keeps the q8 image as the un-pinned lane and on
//! takes the exact planes; with the tower off the q8 image whatever the knob (the control); the knob put back as found
[unused_argument(t)]   // without dasVulkan the gated body is empty
def private ga_vk_lane_policy_cell(t : T?) {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        with_gpu_knobs() {
            set_vulkan_tower(true)
            let was = set_vulkan_gigaam_exact_lane(false)
            t |> success(gigaam_gpu_serves() && gigaam_serves_q8(), "the lane knob off: the un-pinned lane is the q8 image")
            set_vulkan_gigaam_exact_lane(true)
            t |> success(!gigaam_serves_q8(), "the lane knob on: the un-pinned lane is the exact planes")
            set_vulkan_tower(false)
            t |> success(gigaam_serves_q8(), "the tower off: the un-pinned lane is the q8 image whatever the knob (the control)")
            set_vulkan_gigaam_exact_lane(was)
        }
    }
}

//! the device's k4 cm2 and KHR tile dispatches: the kq lane's K-quant seats run on one of them, by the device's mode and the column pick
//! the dispatches of the K-quant member `fmt`'s batch tiles (`kq_batch_<fmt>_*`) the census counted
[unused_argument(fmt)]
def private kq_tile_count(fmt : string) : int64 {
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

//! GigaAM's kq lane on the Vulkan chain against the CPU kq chain: the lane named `fmt`, no `kq_lane` or `quant_mode` decline,
//! every K-quant member's tiles counted - "a kernel counter ... that no other plane of that file moves" (REVIEW_KQ_FORMATS.md);
//! off the f16 GEMM feed the chain declines `device`
[cold_path, unused_argument(t, kq_path, mx, mk, mkd, samples, clip, fmt), arch(at = "../ARCHITECTURE_GPU_TOWER_VULKAN_FC.md#vk-tower-fc-kq")]   // without dasVulkan the gated body is empty
def private ga_vk_kq_cell(t : T?; kq_path : string; mx, mk, mkd : GigaamModel; samples : array<float>; clip, fmt : string) {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        let tag = "gigaam {clip} kq lane {gigaam_kq_members(mk)}"
        t |> success(mk.kq_lane, "{tag}: the carrier serves as its own planes")
        t |> equal(gigaam_exec_fmt(mk), fmt, "{tag}: the lane's name - its one K-quant member's disk type, or `kq` where the members mix")
        with_gpu_knobs() {
            with_dasllama_jobque_() {
                let members <- kq_members_of(mk)
                set_vulkan_tower(false)
                var inscope ex <- ga_leg(mx, samples)
                var inscope cpu <- ga_leg(mk, samples)
                set_vulkan_tower(true)
                let s0 = vulkan_tower_stats()
                let dk0 = vulkan_tower_declines()?["kq_lane"] ?? 0l
                let dq0 = vulkan_tower_declines()?["quant_mode"] ?? 0l
                let dd0 = off_device_declines()
                let tiles0 <- [for (n in members); kq_tile_count(n)]
                var inscope dev <- ga_leg(mkd, samples)
                let s1 = vulkan_tower_stats()
                t |> equal((vulkan_tower_declines()?["kq_lane"] ?? 0l) - dk0, 0l, "{tag}: no kq_lane decline")
                t |> equal((vulkan_tower_declines()?["quant_mode"] ?? 0l) - dq0, 0l, "{tag}: no quant_mode decline")
                t |> equal(dev.n, cpu.n, "{tag}: the frame count")
                if (!vulkan_tower_front_route()) {
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
                to_log(LOG_INFO, "test_audio: {tag} vulkan : {dev.text}\n")
                let bar = tower_twin_bar(t, tag, ex.x, cpu.x, dev.x, mk.n_layer, double(TWIN_SLACK_VK), TWIN_REL_L2_FLOOR)
                if (bar > 0.0lf) {
                    ga_kq_poison_leg(t, tag, kq_path, mk.n_layer, TowerDev.vulkan, ex.x, samples, bar)
                }
            }
        }
    }
}

//! the kq lane over the mixed-member carrier (Q4_K attention, Q6_K FF, Q8_0 conv pointwise), its Vulkan image beside its CPU one
[cold_path, unused_argument(t, mx, samples, clip)]   // without dasVulkan the gated body is empty
def private ga_vk_kq_mix_cell(t : T?; mx : GigaamModel; samples : array<float>; clip : string) {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        let mix_path = path_join(models_dir(), "gigaam-v3-e2e-rnnt-q4_k-mix-local.gguf")
        return if (!model_available(t, mix_path))
        var inscope mm <- mint_ga(mix_path, false, -1l)
        var inscope mmd <- mint_ga_served(mix_path, false, -1l)
        t |> equal(gigaam_kq_members(mm), "k4+k6+q8", "the mixed carrier's members: Q4_K attention, Q6_K FF, Q8_0 conv pointwise")
        ga_vk_kq_cell(t, mix_path, mx, mm, mmd, samples, clip, "kq")
    }
}

[test, arch(at = "../ARCHITECTURE_GPU_TOWER_VULKAN_FC.md#vk-tower-fc-rotary")]
def test_gigaam_vulkan_twin(tst : T?) {
    tst |> run("GigaAM v3 e2e-rnnt: the Vulkan rotary FastConformer front and blocks on every lane - the q8 lane vs the CPU q8 chain, the exact lane off the halfwords vs the CPU exact chain, the Q4_K carrier's kq lane vs the CPU kq chain - on the FLEURS ru clips") @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            let path = path_join(models_dir(), "gigaam-v3-e2e-rnnt-f16.gguf")
            return if (!family_on(t, "gigaam") || !model_available(t, path) || !vulkan_audio_twin_seat(t))
            var mx <- mint_ga(path, false, -1l)
            var m <- mint_ga(path, true, -1l)
            var md <- mint_ga(path, true, -1l, TowerDev.vulkan)
            var mxd <- mint_ga_served(path, false, -1l)
            t |> success(!mx.q8 && m.q8 && md.dev_backend == TowerDev.vulkan, "the exact and the q8 models minted, the q8 lane's Vulkan image beside")
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
                ga_vk_clip_cell(t, path, mx, m, md, samples, clip)
                ga_vk_exact_cell(t, path, mx, mxd, m, samples, clip)
                if (has_kq) {
                    ga_vk_kq_cell(t, kq_path, mx, mk, mkd, samples, clip, "k4")
                }
                if (clip == "ru_short") {
                    ga_vk_kq_mix_cell(t, mx, samples, clip)
                }
            }
            ga_vk_lane_policy_cell(t)
            delete mkd
            delete mk
            delete mxd
            delete md
            delete m
            delete mx
            vulkan_tower_shutdown()
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

//! GigaAM's CTC head on the Vulkan seat (`vulkan_gigaam_ctc`) against the CPU head over one encode: each pick the CPU's but at a near-tie, -log sum exp within `CTC_PLOG_BAR`, the ids and transcript the CPU's, the device leg counted once and the CPU leg (`mc`) never, the 1.05x control outside the bar; then the head off the first clip's uploaded rows
[cold_path, unused_argument(t, m, mc, samples, tag)]   // without dasVulkan the gated body is empty
def private ga_vk_ctc_cell(t : T?; m, mc : GigaamModel; samples : array<float>; tag : string) {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        with_gpu_knobs() {
            with_dasllama_jobque_() {
                set_vulkan_tower(true)
                let bar = Bar(rel = CTC_PLOG_BAR, abs_floor = CTC_PLOG_BAR, poison_scaled = true)
                let n_in = min(long_length(samples), GIGAAM_MAX_CLIP_S * AUDIO_SAMPLE_RATE)
                var st = GigaamState()
                let n = gigaam_encode_span(samples, 0l, n_in, m, st)
                let held = st.enc_gen != 0ul
                to_log(LOG_INFO, "test_audio: {tag}: the encode {held ? "served on the device, its rows held" : "ran on the CPU"}\n")
                set_vulkan_tower(false)
                var sc = GigaamState()
                sc.x := st.x
                var inscope cpu <- ctc_cpu_leg(mc, sc, n)
                delete sc
                set_vulkan_tower(true)
                let s0 = vulkan_ctc_stats()
                var inscope dev <- ctc_leg(m, st, n)
                let s1 = vulkan_ctc_stats()
                to_log(LOG_INFO, "test_audio: {tag} cpu   : {cpu.text}\n")
                to_log(LOG_INFO, "test_audio: {tag} vulkan: {dev.text}\n")
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
                let u0 = vulkan_ctc_stats()
                var inscope up <- ctc_leg(m, st, n)
                t |> equal(up.served, 1l, "{tag}: off rows the device no longer holds the head serves off the uploaded rows")
                t |> equal(vulkan_ctc_stats().handoffs - u0.handoffs, 0l, "{tag}: no held rows to read")
                t |> equal(ctc_hard_flips(cpu, up, m.n_classes), 0, "{tag}: the uploaded rows' picks are the CPU head's but at a near-tie frame")
                check(t, "test_audio: {tag} the head off the uploaded rows' log-sums", up.plogs, cpu.plogs, bar, FLAT_SHAPE)
                t |> equal(up.text, cpu.text, "{tag}: the uploaded rows' transcript")
                delete st
            }
        }
    }
}

[test, arch(at = "../ARCHITECTURE_MEDIA_ASR.md#gigaam-planes")]
def test_gigaam_ctc_vulkan_twin(tst : T?) {
    for (kind, q8 in ["e2e-ctc", "ctc", "e2e-ctc", "ctc"], [false, false, true, true]) {
        tst |> run("GigaAM v3 {kind} {q8 ? "q8" : "f16"}: the CTC head on the Vulkan device seat is the CPU head's, off the encode's held rows and off uploaded rows") @(t : T?) {
            static_if (typeinfo builtin_module_exists(vulkan)) {
                let path = path_join(models_dir(), "gigaam-v3-{kind}-f16.gguf")
                return if (!family_on(t, "gigaam") || !model_available(t, path) || !vulkan_audio_twin_seat(t))
                let wav = asr_clip("ru_short.wav")
                if (!stat(wav).is_valid) {
                    t |> skip("ru_short.wav not present")
                    return
                }
                var m <- mint_ga_served(path, q8, -1l)
                var mc <- mint_ga(path, q8, -1l)
                t |> success(m.ctc_f16 && m.dw_f16, "the head and the depthwise taps serve off the carrier's halfwords")
                var inscope samples <- load_audio_16k_mono(wav)
                ga_vk_ctc_cell(t, m, mc, samples, "gigaam {kind} {q8 ? "q8" : "f16"} ru_short")
                delete mc
                delete m
                vulkan_ctc_shutdown()
                vulkan_tower_shutdown()
            } else {
                t |> skip("dasVulkan not present")
            }
        }
    }
}

[test, arch(at = "../ARCHITECTURE_GPU_TOWER_VULKAN.md#vk-tower-encode-chains")]
def test_encoder_blocks_vulkan(tst : T?) {
    tst |> run("qwen2audio tower: Vulkan blocks vs the CPU q8 chain on the synthetic mel, 32 layers") @(t : T?) {
        vulkan_blocks_twin(t, "qwen2audio", "qwen2audio-mmproj-f32.gguf", 32l)
    }
    tst |> run("voxtral tower: Vulkan blocks vs the CPU q8 chain on the synthetic mel") @(t : T?) {
        vulkan_blocks_twin(t, "voxtral", "voxtral-mini-mmproj-f32.gguf", 0l)
    }
    tst |> run("omni-3b audio tower: Vulkan blocks vs the CPU q8 chain on the synthetic mel") @(t : T?) {
        vulkan_blocks_twin(t, "omni-3b", "mmproj-omni-3b-f32.gguf", 0l)
    }
    tst |> run("ultravox audio tower: Vulkan blocks and the gated-MLP tail vs the CPU q8 chain on the synthetic mel") @(t : T?) {
        vulkan_blocks_twin(t, "ultravox", "mmproj-ultravox-v0_5-llama-3_2-1b-f16.gguf", 0l)
    }
}

//! the qwen3a tower staged and minted in memory under the lane pin, the pin reset after; `zero_block` >= 0 zeroes the
//! q8 GEMM planes of `poison_blocks` blocks from that block on; `dev` the image flavor (`none` the CPU image, a GPU flavor staged
//! row-major for its driver's bake - `#tower-dev-block`)
def private mint_q3a(path : string; q8 : bool; zero_block : int64; dev : TowerDev = TowerDev.none) : Qwen3aTower {
    set_qwen3a_q8(q8)
    defer() {
        reset_qwen3a_q8()
    }
    var inscope stg <- tower_dev_staged(dev) $ { return <- stage_qwen3a_tower(path) }
    if (zero_block >= 0l) {
        let nl = stg.e.core.n_layer
        for (b in range64(zero_block, min(nl, zero_block + poison_blocks(nl)))) {
            zero_plane(stg.core.qblob, stg.e.core.qlayers_off + b * stg.e.core.qlayer_stride, stg.e.core.qlayer_stride)
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
    if (gpu_knobs().vulkan_tower) {
        qwen3a_encode(tw, s.enc, s.conv, dev_win)
    } else {
        qwen3a_encode(tw, s.enc, s.conv, win)
    }
    out := s.enc.out
    return long_length(out)
}

[test]
def test_qwen3a_vulkan_front(tst : T?) {
    tst |> run("qwen3a: the Vulkan mel + conv front + blocks vs the CPU q8 chain on jfk windows 0 and the tail") @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            let path = path_join(models_dir(), "mmproj-Qwen3-ASR-0.6B-bf16.gguf")
            jfk_twin_cell(t, path, true) $(samples) {
                var twx <- mint_q3a(path, false, -1l)
                var tw <- mint_q3a(path, true, -1l)
                t |> success(!twx.core.q8 && tw.core.q8 && tw.cq8, "qwen3a jfk: the exact and the q8 towers minted, the cblob trio on the q8 lane")
                set_vulkan_tower(false)
                let m0 = vulkan_tower_stats()
                let mel <- log_mel_qwen3a(samples, tw.core.n_mel)
                let m1 = vulkan_tower_stats()
                t |> equal(m1.mels - m0.mels, 0l, "qwen3a jfk mel: the knob-off mel ran on the CPU (mels delta)")
                set_vulkan_tower(true)
                let dev_mel <- log_mel_qwen3a(samples, tw.core.n_mel)
                let m2 = vulkan_tower_stats()
                t |> equal(m2.mels - m1.mels, 1l, "qwen3a jfk mel: the device mel served (mels delta)")
                t |> equal(m2.declines, m1.declines, "qwen3a jfk mel: the device mel served with no decline")
                t |> equal(dev_mel.n_len, mel.n_len, "qwen3a jfk mel: the device mel has the CPU mel's frames")
                let mel_maxd = logits_maxdiff(dev_mel.data, mel.data)
                t |> success(mel_maxd < 1e-3, "qwen3a jfk mel: the device log-mel matches the CPU's within 1e-3 (max |diff| {mel_maxd})")
                // a fresh resident under a storage range its scratch runs past: the range decline must leave no capacity behind,
                // so with the range put back the next mel builds its scratch rather than dispatching into the released buffers
                vulkan_tower_shutdown()
                let msr = g_gpu.msr
                let k0 = vulkan_tower_declines()?["memory"] ?? 0l
                g_gpu.msr = 4096l
                let cpu_mel <- log_mel_qwen3a(samples, tw.core.n_mel)
                g_gpu.msr = msr
                t |> equal((vulkan_tower_declines()?["memory"] ?? 0l) - k0, 1l, "qwen3a jfk mel: the scratch past the range declined memory")
                check_exact(t, "qwen3a jfk mel: the declined mel is the CPU's", cpu_mel.data, mel.data)
                let m3 = vulkan_tower_stats()
                let re_mel <- log_mel_qwen3a(samples, tw.core.n_mel)
                t |> equal(vulkan_tower_stats().mels - m3.mels, 1l, "qwen3a jfk mel: the range put back, the device mel served on a rebuilt scratch")
                check_exact(t, "qwen3a jfk mel: the rebuilt scratch's rows are the first device mel's", re_mel.data, dev_mel.data)
                var twd <- mint_q3a(path, true, -1l, TowerDev.vulkan)
                var sx <- q3a_leg_state(twx)
                var sq <- q3a_leg_state(tw)
                var sd <- q3a_leg_state(twd)
                for (off in [0l, QWEN3A_WINDOW]) {
                    let tag = "qwen3a jfk mel+front+blocks at {off}"
                    let win <- qwen3a_mel_window(mel, off)
                    let dev_win <- qwen3a_mel_window(dev_mel, off)
                    let n_chunks = dev_win.n_len / QWEN3A_CHUNK
                    let spec = TwinSpec(family = "qwen3a", tag = tag, n_layer = tw.core.n_layer, metric = TwinMetric.rel_l2)
                    let r <- twin_three_way(t, spec, tw, sq, twd, sd, twx, sx, $(e; var s; var out) => q3a_twin_encode(e, s, win, dev_win, out)) $(legs) {
                        if (legs.dev.convs == legs.pre.convs) {
                            to_log(LOG_INFO, "{tag}: the driver declined - {vulkan_tower_declines()}\n")
                        }
                        front_served(t, tag, legs.dev.convs - legs.pre.convs, n_chunks, legs.dev.declines - legs.cpu.declines)
                        t |> equal(legs.dev.encodes - legs.pre.encodes, 1l, "{tag}: the block loop served the window")
                    }
                    if (off == 0l && r.bar > 0.0lf) {
                        var twp <- mint_q3a(path, true, tw.core.n_layer / 2l, TowerDev.vulkan)
                        var sp <- q3a_leg_state(twp)
                        twin_poison(t, r, tw.core.n_layer / 2l, twp, sp) $(e; var s; var out) => q3a_twin_encode(e, s, win, dev_win, out)
                        delete sp
                        delete twp
                    }
                }
                delete sd
                delete sq
                delete sx
                delete twd
                delete tw
                delete twx
                vulkan_tower_shutdown()
            }
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

//! the Metal chain against the CPU encoder on one synthetic mel, one cell a projector kind (the CPU image on the CPU, the Metal-flavored one on the device, the tail behind the blocks): one encode, every block, the tail once, nothing on the CPU leg, the tokens within `tower_parity_deep`'s bar, another mel's outside it; `layers` > 0 pins the depth
[cold_path, unused_argument(file, kind, layers)]   // off-Apple the cell skips
def private metal_tower_chain_cell(t : T?; file : string; kind : AudioProjKind; layers : int64) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let path = path_join(models_dir(), file)
        return if (!jit_model_available(t, path))
        let tower_was = metal_tower_knob()
        set_metal_tower(true)
        var twc <- mint_aud(path, false, -1l)
        var tw <- mint_aud(path, false, -1l, TowerDev.metal)
        t |> success(tw.proj_kind == kind, "{file}: the projector kind the cell names")
        if (layers > 0l) {
            t |> equal(tw.n_layer, layers, "{file}: the tower's depth is the {layers} blocks the cell names")
        }
        var inscope mel <- synth_mel(tw.n_mel)
        var encc <- make_encoder_state(twc)
        var enc <- make_encoder_state(tw)
        with_dasllama_jobque_() {
            let s0 = metal_tower_stats()
            let t0 = metal_tower_tail_encodes()
            audio_encode(twc, encc, mel)
            t |> equal(metal_tower_stats().encodes - s0.encodes, 0l, "{file}: the CPU image's leg never dispatched")
            audio_encode(tw, enc, mel)
            let s1 = metal_tower_stats()
            t |> equal(s1.encodes - s0.encodes, 1l, "{file}: the Metal image's leg dispatched once")
            t |> equal(s1.blocks - s0.blocks, tw.n_layer, "{file}: blocks delta = n_layer")
            t |> equal(s1.rows - s0.rows, tw.n_ctx, "{file}: rows delta = n_ctx")
            t |> equal(metal_tower_tail_encodes() - t0, 1l, "{file}: the projector tail rode the device behind the blocks")
            tower_parity_deep(t, "{file} {tw.n_layer}L", encc.out, enc.out, tw.n_layer)
            synth_mel_fill(mel, true)
            audio_encode(tw, enc, mel)
            var ctl = TowerParityStats()
            if (tower_parity_stats(t, "{file} control", encc.out, enc.out, ctl)) {
                let bar = tower_deep_bar(tw.n_layer)
                t |> success(is_finite(ctl.rel_l2) && ctl.rel_l2 > bar, "{file}: another mel's tokens land outside the bar (rel_l2 {ctl.rel_l2}) - the control")
            }
        }
        set_metal_tower(tower_was)
        delete enc
        delete encc
        delete tw
        delete twc
    } else {
        t |> skip("das_metal not present")
    }
}

[test]
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
[test]
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

[test]
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

[test]
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
[test]
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

//! one parakeet clip's encoder rows decoded on the CPU loop and on the device seat (`rnnt_device_twin`): the CPU's tuples, the plog within `PK_DEVICE_PLOG_BAR`, the exact lane projected on the device (off the encode's device rows on Metal), the q8 lane off the CPU's projection; `m` the lane's GPU-flavored model, `mc` its CPU image
[cold_path]
def private pk_device_decode_clip(t : T?; m, mc : ParakeetModel; samples : array<float>; tag : string) {
    return if (rnnt_seat_skip(t, tag))
    with_gpu_knobs() {
        set_serving_gpu_tower(true)
        var st = ParakeetState()
        var stc = ParakeetState()
        parakeet_log_mel(samples, m, st)
        let tt = parakeet_encode(m, st)
        var inscope tw <- rnnt_device_twin(t, "test_audio: {tag}", PK_DEVICE_PLOG_BAR, m.q8 ? 0l : 1l, !m.q8 && rnnt_seat_name() == "metal" ? 1l : 0l,
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

[test]
def test_parakeet_device_decode_twin(tst : T?) {
    with_dasllama_jobque_() {
        for (v in ["v2", "v3"]) {
            let path = path_join(whisper_dir(), "ggml-parakeet-tdt-0.6b-{v}-f32.bin")
            continue if (!model_available(tst, path))
            for (q8 in [false, true]) {
                tst |> run("parakeet {v} {q8 ? "q8" : "f32"}: the TDT decode on the device seat is the CPU decoder's") @(t : T?) {
                    if (!serving_tower_device_up()) {
                        t |> skip("parakeet {v}: no GPU device stands up for the serving tower on this build or box")
                        return
                    }
                    let served_q8 = serving_tower_dev() == TowerDev.vulkan   // the lane the build's FastConformer chain flavors: Vulkan's q8, Metal's exact
                    if (q8 != served_q8) {
                        t |> skip("parakeet {v} {q8 ? "q8" : "f32"}: the {serving_tower_dev()} chain flavors the other lane - this one runs whole on the CPU")
                        return
                    }
                    var dev = TowerDev.none
                    with_gpu_knobs() {
                        set_serving_gpu_tower(true)
                        dev = parakeet_dev_flavor(q8)
                    }
                    t |> equal(dev, serving_tower_dev(), "parakeet {v}: the serving tower flavors the lane its chain serves")
                    return if (dev == TowerDev.none)
                    var inscope m <- mint_pk(path, q8, -1l, dev)
                    var inscope mc <- mint_pk(path, q8, -1l)
                    for (clip in ["jfk", "gb1"]) {
                        let wav = asr_clip("{clip}.wav")
                        if (!stat(wav).is_valid) {
                            t |> skip("{clip}.wav not present")
                            continue
                        }
                        var inscope samples <- load_audio_16k_mono(wav)
                        pk_device_decode_clip(t, m, mc, samples, "parakeet {v} {q8 ? "q8" : "f32"} {clip}")
                    }
                }
            }
        }
    }
}
