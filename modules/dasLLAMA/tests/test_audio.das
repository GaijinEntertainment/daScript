options gen2
options persistent_heap   // model-scale allocations (2.5 GB mmproj tower) — freed per test
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama_audio
require dasllama/dasllama_tower
require dasllama/dasllama_math   // setup_dasllama_jobque_
require daslib/jobque_boost
require daslib/fio
require daslib/defer
require math
require _model_tier   // models_dir() + model_available() presence/size-tier gate (see the file's header)
require ?das_metal dasllama/dasllama_metal_tower   // pin the CPU-oracle encodes off-GPU + the blocks parity cell
require ?vulkan dasllama/dasllama_vulkan_tower
require dasllama/dasllama_gemma4a
require dasllama/dasllama_canary
require dasllama/dasllama_qwen3a
require _tower_twin

// Audio tower tests. The mel/gelu/filterbank tests are self-contained; the encoder tests
// need the locally-converted Qwen2-Audio mmproj (2.5 GB, gitignored) and skip without it.
// The oracle constants come from llama-mtmd-debug (CPU backend). The Vulkan twins skip without
// their carriers (the qwen2audio / voxtral / omni-3b f32 mmprojs, the E2B bf16 mmproj, the
// canary f32 encoder, the Qwen3-ASR bf16 mmproj), without jfk.wav, without a Vulkan device under
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
        // the mtmd-debug 'preproc 440' fixture: 5 s sine -> exactly 2 full 3000-frame chunks
        var s : array<float>
        s |> resize(80000)
        for (i in range(80000)) {
            s[i] = sin(2.0 * 3.14159265 * 440.0 * float(i) / 16000.0)
        }
        let chunks <- log_mel_chunks(s, 128l)
        t |> success(length(chunks) == 2, "two chunks (audio + padded silence)")
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
        // the second chunk is all padded silence: every value at the clamp floor
        var uniform = true
        for (v in chunks[1].data) {
            if (v != chunks[1].data[0]) {
                uniform = false
            }
        }
        t |> success(uniform, "silence chunk uniform at the floor")
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
[test]
def test_ultravox_tower(t : T?) {
    t |> run("ultravox mmproj: stack-8 projector shapes + 187 tokens/chunk") @(t : T?) {
        let path = path_join(models_dir(), "mmproj-ultravox-1b-f32.gguf")
        if (!model_available(t, path)) return
        var tw <- load_audio_tower(path)
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
            let tower_was_on = vulkan_tower_knob()
            defer() {
                restore_every_gpu_tower(tower_was_on)
            }
            set_every_gpu_tower(false)
            with_job_que() {
                setup_dasllama_jobque_()
                audio_encode(tw, enc, mel)
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
        var tw <- load_audio_tower(path)
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
            let tower_was_on = vulkan_tower_knob()
            defer() {
                restore_every_gpu_tower(tower_was_on)
            }
            set_every_gpu_tower(false)
            with_job_que() {
                setup_dasllama_jobque_()
                audio_encode(tw, enc, mel)
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

def private zero_q8(var q : array<int8>; off, n : int64) {
    for (i in range64(n)) {
        q[off + i] = int8(0)
    }
}

//! one zeroed block moves a 32-block tower less than its own q8 chain's noise, so the poison zeroes an eighth of the blocks
def private poison_blocks(n_layer : int64) : int64 => max(1l, n_layer / 8l)

//! `zero_block` >= 0 zeroes the q8 GEMM planes of `poison_blocks` blocks from that block on
def private mint_aud(path : string; q8 : bool; zero_block : int64) : AudioTower {
    var inscope stg <- stage_audio_tower(path, q8)
    if (zero_block >= 0l) {
        for (b in range64(zero_block, min(stg.e.n_layer, zero_block + poison_blocks(stg.e.n_layer)))) {
            zero_q8(stg.qblob, stg.e.qlayers_off + b * stg.e.qlayer_stride, stg.e.qlayer_stride)
        }
    }
    var tw = AudioTower()
    mint_audio_tower(stg, tw, q8)
    return <- tw
}

//! `audio_encode` over `mel` as a twin leg: the output copied into `out`, its length returned
def private aud_twin_encode(tw : AudioTower; var enc : EncoderState; mel : MelChunk; var out : array<float>) : int64 {
    audio_encode(tw, enc, mel)
    out := enc.out
    return long_length(out)
}

[cold_path]
def private vulkan_blocks_twin(t : T?; family, file : string; layers_claimed : int64) {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        let path = path_join(models_dir(), file)
        if (!model_available(t, path) || !vulkan_twin_seat(t)) {
            return
        }
        let tag = "{family} synthetic mel"
        let tower_was_on = vulkan_tower_on()
        defer() {
            set_vulkan_tower(tower_was_on)
        }
        var twx <- mint_aud(path, false, -1l)
        var tw <- mint_aud(path, true, -1l)
        t |> success(!twx.q8 && tw.q8, "{tag}: the exact and the q8 towers minted")
        if (layers_claimed > 0l) {
            t |> equal(tw.n_layer, layers_claimed, "{tag}: the tower's depth is the {layers_claimed} blocks the cell names")
        }
        var mel = MelChunk(n_mel = tw.n_mel, n_len = AUDIO_CHUNK_FRAMES)
        mel.data |> resize(tw.n_mel * AUDIO_CHUNK_FRAMES)
        for (i in range(length(mel.data))) {
            mel.data[i] = sin(float(i) * 0.0137) * 0.8 + sin(float(i) * 0.00291) * 0.5
        }
        var encx <- make_encoder_state(twx)
        var enc <- make_encoder_state(tw)
        let spec = TwinSpec(family = family, lbl = tag, n_layer = tw.n_layer, metric = TwinMetric.rel_l2)
        let r <- twin_three_way(t, spec, tw, enc, twx, encx, $(e; var s; var out) => aud_twin_encode(e, s, mel, out)) $(legs) {
            t |> equal(legs.dev.encodes - legs.pre.encodes, 1l, "{tag}: exactly the device leg dispatched")
            t |> equal(legs.dev.blocks - legs.pre.blocks, tw.n_layer, "{tag}: blocks delta = n_layer")
            t |> equal(legs.dev.rows - legs.pre.rows, tw.n_ctx, "{tag}: rows delta = n_ctx")
        }
        if (r.bar > 0.0lf) {
            var twp <- mint_aud(path, true, tw.n_layer / 2l)
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

//! `zero_block` >= 0 zeroes the eleven q8 GEMM planes of `poison_blocks` blocks from that block on
def private mint_g4a(mm : string; q8 : bool; zero_block : int64) : Gemma4aEncoder {
    set_gemma4a_q8(q8)
    defer() {
        reset_gemma4a_q8()
    }
    var inscope stg <- stage_gemma4a_encoder(mm)
    if (zero_block >= 0l) {
        let d = stg.e.d_model
        let ff = stg.e.n_ff
        for (b in range64(zero_block, min(stg.e.n_layer, zero_block + poison_blocks(stg.e.n_layer)))) {
            let lo = stg.layers[b]
            for (w in fixed_array(lo.q, lo.k, lo.v, lo.o, lo.attn_k_rel, lo.conv_pw2)) {
                zero_q8(stg.qblob, w, d * d)
            }
            for (w in fixed_array(lo.ff_up, lo.ff_down, lo.ff_up_1, lo.ff_down_1)) {
                zero_q8(stg.qblob, w, d * ff)
            }
            zero_q8(stg.qblob, lo.conv_pw1, d * 2l * d)
        }
    }
    var enc = Gemma4aEncoder()
    mint_gemma4a_encoder(stg, enc)
    return <- enc
}

//! the first 3 s of a clip: an encode short enough to size the residency's scratch under the full clip's
def private clip_head(samples : array<float>) : array<float> {
    let n = min(long_length(samples), 3l * AUDIO_SAMPLE_RATE)
    return <- [for (i in range64(n)); samples[i]]
}

//! a short encode then the full clip on one residency (the scratch - and gemma4a's rel projection sets, canary's rel quartet -
//! regrown under it), then the clip on a fresh residency after a shutdown, each held to the bar `r` set, then the input poison
//! over the q8 tower `mint_poisoned` returns; `exactly_three`: the three encodes are three dispatches, else at least three (a
//! chunked encode dispatches per chunk)
[cold_path, unused_argument(t, tag, enc, st, samples, r, exactly_three, mint_poisoned, encode)]
def private twin_regrow_legs(t : T?; tag : string; enc : auto(TW); var st : auto(ST); samples : array<float>; r : TwinResult;
                             exactly_three : bool; mint_poisoned : block<() : TW>;
                             encode : block<(e : TW; var s : ST; clip : array<float>; var out : array<float>) : int64>) {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        let head <- clip_head(samples)
        var dev_head : array<float>
        var dev_again : array<float>
        var dev_fresh : array<float>
        set_vulkan_tower(true)
        let e0 = vulkan_tower_stats().encodes
        with_job_que() {
            setup_dasllama_jobque_()
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
        with_job_que() {
            setup_dasllama_jobque_()
            invoke(encode, encp, stp, samples, poisoned)
        }
        tower_twin_poison(t, tag, r.reference, poisoned, r.bar)
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
                var stx = Gemma4aState()
                var st = Gemma4aState()
                var n_exact = 0l
                let spec = TwinSpec(family = "gemma4a", lbl = "gemma4a jfk", n_layer = enc.n_layer, metric = TwinMetric.rel_l2)
                let r <- twin_three_way(t, spec, enc, st, encx, stx, $(e; var s; var out) => gemma4a_encode_all(e, s, samples, out)) $(legs) {
                    n_exact = legs.nexact
                    let n_chunks = legs.dev.encodes - legs.pre.encodes
                    t |> equal(legs.ncpu, legs.nexact, "gemma4a jfk: the q8 chain's token count")
                    t |> equal(legs.ndev, legs.nexact, "gemma4a jfk: the device chain's token count")
                    t |> success(n_chunks >= 1l, "gemma4a jfk: the device served the chunk encodes ({n_chunks})")
                    t |> equal(legs.dev.blocks - legs.pre.blocks, n_chunks * enc.n_layer, "gemma4a jfk: every block of every served encode ran on the driver")
                    front_served(t, "gemma4a jfk", legs.dev.convs - legs.pre.convs, n_chunks, legs.dev.declines - legs.pre.declines)
                }
                if (r.bar > 0.0lf) {
                    var st2 = Gemma4aState()
                    twin_regrow_legs(t, "gemma4a jfk", enc, st2, samples, r, false, $ { return <- mint_g4a(mm, true, enc.n_layer / 2l) }) $(e; var s; clip; var out) => gemma4a_encode_all(e, s, clip, out)
                    delete st2
                }
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

//! `zero_block` >= 0 zeroes the eleven q8 GEMM planes of `poison_blocks` blocks from that block on
def private mint_cn(path : string; q8 : bool; zero_block : int64) : CanaryEncoder {
    var inscope stg <- stage_canary_encoder(path, q8)
    if (zero_block >= 0l) {
        let d = stg.e.d_model
        let ff = stg.e.n_ff
        for (b in range64(zero_block, min(stg.e.n_layer, zero_block + poison_blocks(stg.e.n_layer)))) {
            let lo = stg.layers[b]
            for (w in fixed_array(lo.q_w, lo.k_w, lo.v_w, lo.pos_w, lo.o_w, lo.pw2_w)) {
                zero_q8(stg.qblob, w, d * d)
            }
            for (w in fixed_array(lo.ff1a_w, lo.ff1b_w, lo.ff2a_w, lo.ff2b_w)) {
                zero_q8(stg.qblob, w, d * ff)
            }
            zero_q8(stg.qblob, lo.pw1_w, d * 2l * d)
        }
    }
    var enc = CanaryEncoder()
    mint_canary_encoder(stg, enc)
    return <- enc
}

//! canary's log-mel and encode of `clip` as a twin leg: the output copied into `out`, the frame count returned
def private cn_twin_encode(enc : CanaryEncoder; var st : CanaryState; clip : array<float>; var out : array<float>) : int64 {
    canary_log_mel(enc, clip, st)
    let n = canary_encode(enc, st)
    out := st.out
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
                var stx = CanaryState()
                var st = CanaryState()
                var n_exact = 0l
                let spec = TwinSpec(family = "canary", lbl = "canary jfk", n_layer = enc.n_layer, metric = TwinMetric.rel_l2)
                let r <- twin_three_way(t, spec, enc, st, encx, stx, $(e; var s; var out) => cn_twin_encode(e, s, samples, out)) $(legs) {
                    n_exact = legs.nexact
                    t |> equal(legs.ncpu, legs.nexact, "canary jfk: the q8 chain's frame count")
                    t |> equal(legs.ndev, legs.nexact, "canary jfk: the device chain's frame count")
                    t |> equal(legs.dev.encodes - legs.pre.encodes, 1l, "canary jfk: the device served the encode")
                    t |> equal(legs.dev.blocks - legs.pre.blocks, enc.n_layer, "canary jfk: every block ran on the driver")
                    front_served(t, "canary jfk", legs.dev.convs - legs.pre.convs, 1l, legs.dev.declines - legs.pre.declines)
                }
                if (r.bar > 0.0lf) {
                    var st2 = CanaryState()
                    twin_regrow_legs(t, "canary jfk", enc, st2, samples, r, true, $ { return <- mint_cn(path, true, enc.n_layer / 2l) }) $(e; var s; clip; var out) => cn_twin_encode(e, s, clip, out)
                    delete st2
                }
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
}

//! the qwen3a tower staged and minted in memory under the lane pin, the pin reset after; `zero_block` >= 0 zeroes the
//! q8 GEMM planes of `poison_blocks` blocks from that block on
def private mint_q3a(path : string; q8 : bool; zero_block : int64) : Qwen3aTower {
    set_qwen3a_q8(q8)
    defer() {
        reset_qwen3a_q8()
    }
    var inscope stg <- stage_qwen3a_tower(path)
    if (zero_block >= 0l) {
        let nl = stg.e.core.n_layer
        for (b in range64(zero_block, min(nl, zero_block + poison_blocks(nl)))) {
            zero_q8(stg.core.qblob, stg.e.core.qlayers_off + b * stg.e.core.qlayer_stride, stg.e.core.qlayer_stride)
        }
    }
    var tw = Qwen3aTower()
    mint_qwen3a_tower(stg, tw)
    return <- tw
}

//! a qwen3a twin leg's two states over one tower
struct Q3aLegState {
    enc : EncoderState
    cs : Qwen3aConvState
}

def private q3a_leg_state(tw : Qwen3aTower) : Q3aLegState {
    var s = Q3aLegState()
    s.enc <- make_encoder_state(tw.core)
    return <- s
}

//! `qwen3a_encode` as a twin leg: the device leg (the knob on) reads the device mel's window `dev_win`, the CPU legs the CPU
//! mel's `win`; the output copied into `out`, its length returned
def private q3a_twin_encode(tw : Qwen3aTower; var s : Q3aLegState; win, dev_win : MelChunk; var out : array<float>) : int64 {
    if (vulkan_tower_knob()) {
        qwen3a_encode(tw, s.enc, s.cs, dev_win)
    } else {
        qwen3a_encode(tw, s.enc, s.cs, win)
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
                var sx <- q3a_leg_state(twx)
                var sq <- q3a_leg_state(tw)
                for (off in [0l, QWEN3A_WINDOW]) {
                    let tag = "qwen3a jfk mel+front+blocks at {off}"
                    let win <- qwen3a_mel_window(mel, off)
                    let dev_win <- qwen3a_mel_window(dev_mel, off)
                    let n_chunks = dev_win.n_len / QWEN3A_CHUNK
                    let spec = TwinSpec(family = "qwen3a", lbl = tag, n_layer = tw.core.n_layer, metric = TwinMetric.rel_l2)
                    let r <- twin_three_way(t, spec, tw, sq, twx, sx, $(e; var s; var out) => q3a_twin_encode(e, s, win, dev_win, out)) $(legs) {
                        if (legs.dev.convs == legs.pre.convs) {
                            to_log(LOG_INFO, "{tag}: the driver declined - {vulkan_tower_declines()}\n")
                        }
                        front_served(t, tag, legs.dev.convs - legs.pre.convs, n_chunks, legs.dev.declines - legs.pre.declines)
                        t |> equal(legs.dev.encodes - legs.pre.encodes, 1l, "{tag}: the block loop served the window")
                    }
                    if (off == 0l && r.bar > 0.0lf) {
                        var twp <- mint_q3a(path, true, tw.core.n_layer / 2l)
                        var sp <- q3a_leg_state(twp)
                        twin_poison(t, r, tw.core.n_layer / 2l, twp, sp) $(e; var s; var out) => q3a_twin_encode(e, s, win, dev_win, out)
                        delete sp
                        delete twp
                    }
                }
                delete sq
                delete sx
                delete tw
                delete twx
                vulkan_tower_shutdown()
            }
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

// The GPU tower blocks vs the CPU encoder on the SAME all-ones fixture as the oracle above -
// the projector runs CPU on both legs, so the delta isolates the 32-layer block loop (the
// battery's deep structural check; whisper-large geometry). Counters prove the dispatch.
[test]
def test_encoder_blocks_gpu(tst : T?) {
    tst |> run("qwen2audio tower: GPU blocks vs CPU encoder, 32 layers") @(t : T?) {
        static_if (typeinfo builtin_module_exists(das_metal)) {
            if (!jit_enabled()) {
                t |> skip("interpreted (dasLLAMA model tests are JIT-only)")
                return
            }
            let path = path_join(models_dir(), "qwen2audio-mmproj-f32.gguf")
            if (!model_available(t, path)) {
                return
            }
            var tw <- load_audio_tower(path)
            var mel = MelChunk(n_mel = tw.n_mel, n_len = AUDIO_CHUNK_FRAMES)
            mel.data |> resize(int(tw.n_mel * AUDIO_CHUNK_FRAMES))
            for (i in range(length(mel.data))) {
                // varied, speech-scale mel: the all-ones fixture is degenerate (identical rows
                // make every attention weight a near-tie, so single elements flip O(1))
                mel.data[i] = sin(float(i) * 0.0137) * 0.8 + sin(float(i) * 0.00291) * 0.5
            }
            var enc <- make_encoder_state(tw)
            set_metal_tower(false)
            var cpu_out : array<float>
            let s0 = metal_tower_stats()
            with_job_que() {
                setup_dasllama_jobque_()
                audio_encode(tw, enc, mel)
                cpu_out := enc.out
                set_metal_tower(true)
                audio_encode(tw, enc, mel)
            }
            let s1 = metal_tower_stats()
            t |> equal(s1.encodes - s0.encodes, 1l, "exactly the GPU leg dispatched")
            t |> equal(s1.blocks - s0.blocks, 32l, "blocks delta = n_layer")
            t |> equal(s1.rows - s0.rows, 1500l, "rows delta = n_ctx")
            tower_parity_deep(t, "qwen2audio blocks 32L", cpu_out, enc.out, 32l)
            delete cpu_out
            delete enc
            delete tw
        }
    }
    tst |> run("voxtral tower: GPU blocks vs CPU encoder") @(t : T?) {
        static_if (typeinfo builtin_module_exists(das_metal)) {
            if (!jit_enabled()) {
                t |> skip("interpreted (dasLLAMA model tests are JIT-only)")
                return
            }
            let path = path_join(models_dir(), "voxtral-mini-mmproj-f32.gguf")
            if (!model_available(t, path)) {
                return
            }
            var tw <- load_audio_tower(path)
            var mel = MelChunk(n_mel = tw.n_mel, n_len = AUDIO_CHUNK_FRAMES)
            mel.data |> resize(int(tw.n_mel * AUDIO_CHUNK_FRAMES))
            for (i in range(length(mel.data))) {
                mel.data[i] = sin(float(i) * 0.0137) * 0.8 + sin(float(i) * 0.00291) * 0.5
            }
            var enc <- make_encoder_state(tw)
            set_metal_tower(false)
            var cpu_out : array<float>
            let s0 = metal_tower_stats()
            with_job_que() {
                setup_dasllama_jobque_()
                audio_encode(tw, enc, mel)
                cpu_out := enc.out
                set_metal_tower(true)
                audio_encode(tw, enc, mel)
            }
            let s1 = metal_tower_stats()
            t |> equal(s1.encodes - s0.encodes, 1l, "exactly the GPU leg dispatched")
            t |> equal(s1.blocks - s0.blocks, tw.n_layer, "blocks delta = n_layer")
            tower_parity_deep(t, "voxtral blocks {tw.n_layer}L", cpu_out, enc.out, tw.n_layer)
            delete cpu_out
            delete enc
            delete tw
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
        if (!jit_enabled()) {
            t |> skip("interpreted (dasLLAMA model tests are JIT-only)")
            return
        }
        let path = path_join(models_dir(), "qwen2audio-mmproj-f32.gguf")
        if (!model_available(t, path)) return
        var tw <- load_audio_tower(path)
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
        let tower_was_on = vulkan_tower_knob()
        set_every_gpu_tower(false)
        with_job_que() {
            setup_dasllama_jobque_()
            audio_encode(tw, enc, mel)
        }
        restore_every_gpu_tower(tower_was_on)
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
        if (!jit_enabled()) {
            t |> skip("interpreted (dasLLAMA model tests are JIT-only)")
            return
        }
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
