options gen2
options persistent_heap   // model-scale allocations — freed per test
// The uniform AsrModel/AsrSession union every backend's model+scratch by value (whisper, qwen3a,
// parakeet, canary, gemma4a) — ~5 KB / ~10 KB each now; tests holding two (q8-gate) or nesting
// load frames (parakeet) need headroom above the default. Bumped for the 6-backend union.
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama_whisper
require dasllama/dasllama_asr
require dasllama/dasllama_audio
require dasllama/dasllama_tower
require dasllama/dasllama_common   // KVDtype — the oracle cells pin the f32 parity rail
require dasllama/dasllama_gguf
require dasllama/dasllama_math   // setup_dasllama_jobque_
require daslib/jobque_boost
require daslib/fio
require daslib/defer
require math
require strings
require _model_tier   // model_available() presence/size-tier gate (see the file's header)
require dasllama/dasllama_env
require daslib/strings_boost   // split_by_chars — the crowned word-level cell's normalizer
require ?das_metal dasllama/dasllama_metal_asr_dec   // set_metal_wdec — the CPU-claim cells pin the GPU cross-KV hook off
require ?das_metal dasllama/dasllama_metal_tower     // set_metal_tower — the mtmd-oracle cells pin the GPU conv hook off
require ?das_metal dasllama/dasllama_metal_prefill   // metal_prefill_shutdown — the crown-pin cell drops the trio's cached flags
require ?vulkan dasllama/dasllama_vulkan_tower
require ?vulkan dasllama/dasllama_vulkan_asr_dec   // set_vulkan_wdec / vulkan_wdec_stats - the decoder twin's seat
require _tower_twin

// Whisper-proper tests. The f16/gelu/mel/id tests are self-contained; the loader and
// transcription tests need a whisper.cpp ggml-tiny.bin (gitignored) and skip without it.
// Oracle constants come from whisper-cli (CPU, greedy: -bs 1 -bo 1 -nf -ng). The Vulkan twin
// needs ggml-tiny.bin and ggml-large-v3-turbo.bin plus jfk.wav, and skips without a Vulkan
// device under DASLLAMA_GPU=1, on a build with das_metal, and when interpreted.

// the ASR corpus (jfk + LibriSpeech clips) lives under the repo's benchmarks/asr, gitignored
def private corpus_dir() : string {
    let env = g_env_test.corpus_dir
    return empty(env) ? "/Users/borisbatkin/Work/daScript/modules/dasLLAMA/benchmarks/asr/corpus/" : env
}

[test]
def test_f32_to_f16(t : T?) {
    t |> run("round-trips every non-nan f16 pattern") @(t : T?) {
        var bad = 0
        for (h in range(65536)) {
            let hu = uint(h)
            if (((hu >> 10u) & 0x1Fu) == 0x1Fu && (hu & 0x3FFu) != 0u) {
                continue
            }
            if (f32_to_f16(f16_to_f32(hu)) != hu) {
                bad++
            }
        }
        t |> success(bad == 0, "all {65536} patterns round-trip ({bad} failed)")
    }
    t |> run("round-to-nearest-even boundaries match numpy float16") @(t : T?) {
        t |> equal(f32_to_f16(0.1), 0x2e66u)
        t |> equal(f32_to_f16(-1.5), 0xbe00u)
        t |> equal(f32_to_f16(65504.0), 0x7bffu)   // f16 max
        t |> equal(f32_to_f16(65519.9), 0x7bffu)   // just under the overflow tie
        t |> equal(f32_to_f16(65520.0), 0x7c00u)   // IEEE overflow to inf
        t |> equal(f32_to_f16(2.9802322387695312e-8), 0x0u)  // exactly 2^-25: tie to even 0
        t |> equal(f32_to_f16(2.9805302e-8), 0x1u)           // a hair above: min subnormal
        t |> equal(f32_to_f16(10.0), 0x4900u)
    }
}

[test]
def test_gelu_tanh_lut(t : T?) {
    t |> run("matches ggml's GGML_GELU_FP16 table semantics") @(t : T?) {
        var g = [1.0, -1.0, 0.5, 3.0, -0.25, 10.0, -10.0, 9.999, -9.999]
        gelu_tanh_lut(g, 9l)
        // expected = f16(gelu_tanh(f16(x))) via numpy float16, clamps applied pre-conversion
        t |> success(g[0] == 0.84130859375, "gelu(1)")
        t |> success(g[1] == -0.1588134765625, "gelu(-1)")
        t |> success(g[2] == 0.345703125, "gelu(0.5)")
        t |> success(g[3] == 2.99609375, "gelu(3)")
        t |> success(g[4] == -0.100341796875, "gelu(-0.25)")
        t |> success(g[5] == 10.0, "gelu(10) passes through")
        t |> success(g[6] == 0.0, "gelu(-10) clamps to 0")
        t |> success(g[7] == 10.0, "gelu(9.999): f16 rounds up, then passthrough value")
        t |> success(g[8] == 0.0, "gelu(-9.999) -> -0.0")
    }
}

[test]
def test_gelu_lut_batch_exhaustive(t : T?) {
    t |> run("gelu_tanh_lut_batch == gelu_tanh_lut for EVERY finite f16 input") @(t : T?) {
        // every f16 pattern as an input row; the batch (table) form must reproduce the
        // computed form bit-for-bit — the table entries ARE the computed middle branch,
        // this proves the branch structure and clamps line up too
        var a : array<float>
        var b : array<float>
        a |> resize(65536)
        for (h in range(65536)) {
            a[h] = f16_to_f32(uint(h))
        }
        var keep : array<float>
        keep |> reserve(65536)
        for (v in a) {
            if (v == v && v < 1e30 && v > -1e30) {   // finite only (nan/inf never reach gelu)
                keep |> push(v)
            }
        }
        let n = length(keep)
        b |> resize(n)
        for (i in range(n)) {
            b[i] = keep[i]
        }
        gelu_tanh_lut(keep, int64(n))
        gelu_tanh_lut_batch(b, int64(n), 1l)   // one row: pure table path, no threading
        var bad = 0
        for (x, y in keep, b) {
            if (x != y) {
                bad++
            }
        }
        t |> success(bad == 0, "all {n} finite f16 patterns bit-equal ({bad} differ)")
    }
    t |> run("gelu_tanh_lut_batch == gelu_tanh_lut on fp32 inputs BETWEEN f16 patterns") @(t : T?) {
        // exact-f16 inputs never exercise f32->f16 rounding — a dense fp32 sweep across
        // (-10.5, 10.5) does, including the values that round UP to the +-10.0 boundary
        // patterns (the bug class: unbuilt table entry read through the v < 10.0 branch)
        let n = 200001
        var a : array<float>
        var b : array<float>
        a |> resize(n)
        b |> resize(n)
        for (i in range(n)) {
            let v = -10.5 + float(i) * 1.05e-4
            a[i] = v
            b[i] = v
        }
        gelu_tanh_lut(a, int64(n))
        gelu_tanh_lut_batch(b, int64(n), 1l)
        var bad = 0
        for (x, y in a, b) {
            if (x != y) {
                bad++
            }
        }
        t |> success(bad == 0, "dense sweep bit-equal ({bad} of {n} differ)")
    }
}

[test]
def test_whisper_mel(t : T?) {
    t |> run("whisper-flavor mel: frame math and silence fill") @(t : T?) {
        // stand-in filterbank (the real one loads from the model file)
        let fb <- build_mel_filterbank(80l, 400l, 16000l)
        let plan <- build_whisper_mel_plan(400l)
        var silence : array<float>
        silence |> resize(16000)          // 1 s of zeros
        let m <- log_mel_whisper(silence, plan, fb, 80l)
        t |> equal(m.n_len, (16000l + 480000l) / 160l)                    // no +1: last frame dropped
        t |> equal(m.n_len_org, 1l + (16000l + 200l - 400l) / 160l)       // real-audio frames
        // all-silence spectrogram: every value is ((-10) + 4)/4 = -1.5 after clamp+scale
        var offv = 0
        for (v in m.data) {
            if (abs(v - -1.5) > 1e-6) {
                offv++
            }
        }
        t |> success(offv == 0, "silence normalizes to exactly -1.5 everywhere ({offv} off)")
    }
    t |> run("window slice zero-fills past n_len") @(t : T?) {
        let fb <- build_mel_filterbank(80l, 400l, 16000l)
        let plan <- build_whisper_mel_plan(400l)
        var silence : array<float>
        silence |> resize(16000)
        let m <- log_mel_whisper(silence, plan, fb, 80l)
        let win <- whisper_mel_window(m, m.n_len - 10l)
        t |> equal(win.n_len, 3000l)
        t |> success(win.data[0] == -1.5, "first 10 frames real")
        t |> success(win.data[11] == 0.0, "zero-fill past n_len")
    }
}

[test]
def test_whisper_lang_ids(t : T?) {
    t |> run("g_lang order") @(t : T?) {
        t |> equal(whisper_lang_id("en"), 0)
        t |> equal(whisper_lang_id("zh"), 1)
        t |> equal(whisper_lang_id("yue"), 99)
        t |> equal(whisper_lang_id("xx"), -1)
    }
}

[test]
def test_dec_layer_layout(t : T?) {
    t |> run("decoder layer offsets tile by the stride") @(t : T?) {
        var w = WhisperModel()
        w.n_text_state = 384l
        w.dlayers_off = 0l
        w.dlayer_stride = dec_layer_stride(384l)
        let a = dec_layer_offsets(w, 0l)
        let b = dec_layer_offsets(w, 1l)
        t |> equal(b.ln1_w - a.ln1_w, w.dlayer_stride)
        t |> equal(b.fc2_b - a.fc2_b, w.dlayer_stride)
        // the last tensor of layer 0 ends exactly where layer 1 begins
        t |> equal(a.fc2_b + 384l, b.ln1_w)
    }
}

[test]
def test_whisper_loader(t : T?) {
    t |> run("tiny bin: hparams, special ids, vocab (needs ggml-tiny.bin)") @(t : T?) {
        let path = path_join(whisper_dir(), "ggml-tiny.bin")
        if (!model_available(t, path)) return
        let w <- load_whisper_model(path)
        t |> equal(w.n_vocab, 51865l)
        t |> equal(w.enc.n_mel, 80l)
        t |> equal(w.enc.d_model, 384l)
        t |> equal(w.enc.n_layer, 4l)
        t |> equal(w.enc.n_ctx, 1500l)
        t |> equal(w.n_text_ctx, 448l)
        t |> success(w.enc.gelu_tanh, "whisper tower uses the tanh-LUT gelu")
        // multilingual shift: 99 languages
        t |> equal(w.n_lang, 99l)
        t |> equal(w.tok_eot, 50257)
        t |> equal(w.tok_sot, 50258)
        t |> equal(w.tok_transcribe, 50359)
        t |> equal(w.tok_not, 50363)
        t |> equal(w.tok_beg, 50364)
        t |> equal(w.tok_space, 220)
        t |> equal(w.vocab[400], " And")
        t |> equal(w.vocab[50256], "")
    }
}

[test]
def test_asr_surface(t : T?) {
    t |> run("load_asr_model sniffs the bin + caps report whisper honestly") @(t : T?) {
        let model_path = path_join(whisper_dir(), "ggml-tiny.bin")
        if (!model_available(t, model_path)) return
        var inscope m <- load_asr_model(model_path)   // the sniff route: ggml magic + big vocab -> whisper
        t |> equal(m.whisper.n_vocab, 51865l, "loaded through the sniff")
        let c <- caps(m)
        // honest caps: tiny is a 99-language model (n_vocab 51865) — "yue" (id 99) only exists
        // on the large-v3 family; advertising it here would build a colliding language token
        t |> equal(length(c.languages), 99, "trimmed to the model's n_lang")
        t |> equal(c.languages[0], "en")
        t |> equal(c.languages[98], "su", "list ends before yue")
        t |> success(c.translate, "translate")
        t |> success(c.timestamps == AsrTimestamps.segment, "segment timestamps")
        t |> success(!c.streaming, "no native streaming")
        t |> success(!c.prompt, "no prompt conditioning")
        // "auto" on a multilingual model is accepted; a junk code still panics loudly
        let s <- create_session(m, "auto")
        t |> equal(s.w.lang_id, -1, "auto defers detection")
        delete m   // whole-model delete: image-backed planes are locked views, only the finalizer may release them
    }
}

//! the Vulkan tower knob where the build carries the driver, true elsewhere; `restore_gpu_tower` reads it back
def private vulkan_tower_was_on() : bool {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        return vulkan_tower_on()
    } else {
        return true
    }
}

//! the Vulkan ASR-decoder knob where the build carries the driver, true elsewhere; `restore_gpu_wdec` reads it back
def private vulkan_wdec_was_on() : bool {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        return vulkan_wdec_on()
    } else {
        return true
    }
}

//! the Metal tower knob back to its default (it has no getter), the Vulkan one back to what the cell found
[unused_argument(vulkan_was_on)]
def private restore_gpu_tower(vulkan_was_on : bool) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        set_metal_tower(true)
    }
    static_if (typeinfo builtin_module_exists(vulkan)) {
        set_vulkan_tower(vulkan_was_on)
    }
}

//! the Metal ASR-decoder knob back to its default (it has no getter), the Vulkan one back to what the cell found
[unused_argument(vulkan_was_on)]
def private restore_gpu_wdec(vulkan_was_on : bool) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        set_metal_wdec(true)
    }
    static_if (typeinfo builtin_module_exists(vulkan)) {
        set_vulkan_wdec(vulkan_was_on)
    }
}

[test]
def test_qwen3a_tower(t : T?) {
    t |> run("qwen3a tower: conv2d frontend + windowed encode matches mtmd on jfk") @(t : T?) {
        if (!jit_enabled()) {
            t |> skip("interpreted (dasLLAMA model tests are JIT-only)")
            return
        }
        let path = path_join(models_dir(), "mmproj-Qwen3-ASR-0.6B-bf16.gguf")
        if (!model_available(t, path)) return
        let samples <- read_wav_pcm16_mono(path_join(models_dir(), "jfk.wav"))
        if (empty(samples)) {
            t |> skip("jfk.wav not present")
            return
        }
        // reference-parity cell: pinned to the f32 rail (the q8 rail has its own A/B cell below)
        // and to the CPU conv chain — the documented ~1e-4 reorder bars are CPU claims
        let tower_was_on = vulkan_tower_was_on()
        {
            defer() {
                restore_gpu_tower(tower_was_on)
            }
            set_every_gpu_tower(false)
            set_qwen3a_q8(false)
            var inscope tw <- load_qwen3a_tower(path)
            reset_qwen3a_q8()
            t |> equal(tw.core.d_model, 896l, "d_model")
            t |> equal(tw.core.n_layer, 18l, "layers")
            t |> equal(tw.core.n_head, 14l, "heads")
            t |> equal(tw.n_ch, 480l, "conv channels")
            t |> equal(tw.proj_dim, 1024l, "proj_dim")
            t |> success(tw.core.has_k_bias, "k bias")
            let mel <- log_mel_qwen3a(samples, tw.core.n_mel)
            t |> equal(mel.n_len, 1101l, "effective mel frames (11 s)")
            var enc <- make_encoder_state(tw.core)
            var cs = Qwen3aConvState()
            with_job_que() {
                setup_dasllama_jobque_()
                let win <- qwen3a_mel_window(mel, 0l)
                t |> equal(win.n_len, 800l, "window 0 padded frames")
                qwen3a_encode(tw, enc, cs, win)
            }
            // llama-mtmd-cli MTMD_DEBUG_EMBEDDINGS (CPU, bf16 mmproj) on jfk window 0: token 0
            // opens 0.043467 -0.015482 -0.032934 0.023813; our fp32 re-orders sit ~1e-4 off.
            t |> success(abs(enc.out[0] - 0.043467) < 5e-3, "tok0[0]")
            t |> success(abs(enc.out[1] + 0.015482) < 5e-3, "tok0[1]")
            t |> success(abs(enc.out[2] + 0.032934) < 5e-3, "tok0[2]")
            t |> success(abs(enc.out[3] - 0.023813) < 5e-3, "tok0[3]")
        }
    }
}

[test]
def test_parakeet_oracle(t : T?) {
    t |> run("parakeet-TDT greedy on jfk.wav is token-for-token with parakeet-cli") @(t : T?) {
        if (!jit_enabled()) {
            t |> skip("interpreted (dasLLAMA model tests are JIT-only)")
            return
        }
        let path = path_join(whisper_dir(), "ggml-parakeet-tdt-0.6b-v2-f32.bin")
        if (!model_available(t, path)) return
        let samples <- read_wav_pcm16_mono(path_join(models_dir(), "jfk.wav"))
        if (empty(samples)) {
            t |> skip("jfk.wav not present")
            return
        }
        // through the uniform ASR surface: the sniff routes a small-vocab ggml bin to parakeet
        reset_parakeet_q8()
        var inscope m <- load_asr_model(path)
        let c <- caps(m)
        t |> equal(length(c.languages), 1, "English-only")
        t |> success(c.timestamps == AsrTimestamps.segment, "segment timestamps")
        t |> equal(m.pk.n_vocab, 1024l, "vocab")
        t |> equal(m.pk.n_layer, 24l, "layers")
        t |> equal(m.pk.durations[4], 4, "tdt durations")
        with_job_que() {
            setup_dasllama_jobque_()
            var inscope s <- create_session(m)
            var inscope segs : array<TranscribeSegment>
            transcribe(m, s, samples) $(seg) {
                segs |> push_clone(seg)
            }
            // parakeet-cli -ng -ps (CPU, f32 self-convert): 33 tokens, these exact ends
            t |> equal(length(segs), 1)
            to_log(LOG_INFO, "parakeet-v2 jfk eyeball: '{segs[0].text}'\n")
            t |> equal(length(segs[0].tokens), 33, "token count")
            t |> equal(segs[0].tokens[0], 123, "tok[0] '_And' (leading SentencePiece space marker)")
            t |> equal(segs[0].tokens[1], 96, "tok[1] '_so' (leading SentencePiece space marker)")
            t |> equal(segs[0].tokens[31], 353, "tok[31] 'ry'")
            t |> equal(segs[0].tokens[32], 841, "tok[32] '.'")
            t |> success(segs[0].text == "And so, my fellow Americans, ask not what your country can do for you, ask what you can do for your country.", "text")
            t |> equal(s.detected_lang, "en")

            t |> equal(m.pk.q8, !parakeet_gpu_serves(), "default load follows the lane policy - q8 unless the Metal driver serves")
            var inscope mf = AsrModel(kind = AsrKind.parakeet)
            var inscope stg <- stage_parakeet_model(path, !m.pk.q8)
            mint_parakeet_model(stg, mf.pk, !m.pk.q8)
            t |> equal(mf.pk.q8, !m.pk.q8, "the mint takes the other lane")
            var inscope sf <- create_session(mf)
            var inscope fsegs : array<TranscribeSegment>
            transcribe(mf, sf, samples) $(seg) {
                fsegs |> push_clone(seg)
            }
            t |> equal(length(fsegs), 1)
            to_log(LOG_INFO, "parakeet-v2 jfk other-lane eyeball: '{fsegs[0].text}'\n")
            t |> equal(length(fsegs[0].tokens), length(segs[0].tokens), "q8/fp32 token count")
            for (a, b in fsegs[0].tokens, segs[0].tokens) {
                t |> equal(a, b, "q8/fp32 token")
            }
        }
    }
}

[test]
def test_parakeet_v3_oracle(t : T?) {
    t |> run("parakeet-TDT v3 (25-lang) greedy on jfk.wav is token-for-token with parakeet-cli") @(t : T?) {
        if (!jit_enabled()) {
            t |> skip("interpreted (dasLLAMA model tests are JIT-only)")
            return
        }
        let path = path_join(whisper_dir(), "ggml-parakeet-tdt-0.6b-v3-f32.bin")
        if (!model_available(t, path)) return
        let samples <- read_wav_pcm16_mono(path_join(models_dir(), "jfk.wav"))
        if (empty(samples)) {
            t |> skip("jfk.wav not present")
            return
        }
        // the 8192-piece SPM vocab routes to parakeet AND flips the multilingual caps
        reset_parakeet_q8()
        var inscope m <- load_asr_model(path)
        let c <- caps(m)
        t |> equal(length(c.languages), 25, "25 languages")
        t |> equal(m.pk.n_vocab, 8192l, "vocab")
        t |> equal(m.pk.q8, !parakeet_gpu_serves(), "default load follows the lane policy - q8 unless the Metal driver serves")
        with_job_que() {
            setup_dasllama_jobque_()
            var inscope s <- create_session(m, "de")   // any of the 25; parakeet has no conditioning
            var inscope segs : array<TranscribeSegment>
            transcribe(m, s, samples) $(seg) {
                segs |> push_clone(seg)
            }
            //! parakeet-cli -ng -ps (CPU, f32 self-convert) gives these 38 tokens; either lane stays id-exact on jfk
            t |> equal(length(segs), 1)
            to_log(LOG_INFO, "parakeet-v3 jfk eyeball: '{segs[0].text}'\n")
            t |> equal(length(segs[0].tokens), 38, "token count")
            t |> equal(segs[0].tokens[0], 1976, "tok[0] '_And' (leading SentencePiece space marker)")
            t |> equal(segs[0].tokens[1], 547, "tok[1] '_so' (leading SentencePiece space marker)")
            t |> equal(segs[0].tokens[36], 7893, "tok[36] 'y'")
            t |> equal(segs[0].tokens[37], 7883, "tok[37] '.'")
            t |> success(segs[0].text == "And so, my fellow Americans, ask not what your country can do for you, ask what you can do for your country.", "text")
            t |> equal(s.detected_lang, "", "v3 does not report a language")
        }
    }
}

[test]
def test_whisper_transcribe_oracle(t : T?) {
    t |> run("tiny greedy on jfk.wav is token-for-token with whisper-cli") @(t : T?) {
        if (!jit_enabled()) {
            t |> skip("interpreted (dasLLAMA model tests are JIT-only)")
            return
        }
        let model_path = path_join(whisper_dir(), "ggml-tiny.bin")
        if (!model_available(t, model_path)) return
        let samples <- read_wav_pcm16_mono(path_join(models_dir(), "jfk.wav"))
        if (empty(samples)) {
            t |> skip("jfk.wav not present")
            return
        }
        var w <- load_whisper_model(model_path)
        with_job_que() {
            setup_dasllama_jobque_()
            var inscope s <- create_session(w)
            var inscope segs : array<TranscribeSegment>
            transcribe(w, s, samples) $(seg) {
                segs |> push_clone(seg)
            }
            // whisper-cli -bs 1 -bo 1 -nf -ng: one segment [0, 10500ms], these exact tokens
            // (incl. the leading <|0.00|> and closing <|10.50|>)
            let want <- [50364, 400, 370, 452, 7177, 6280, 1029, 406, 437, 428, 1941, 393,
                         360, 337, 291, 11, 1029, 437, 291, 393, 360, 337, 428, 1941, 13, 50889]
            t |> equal(length(segs), 1)
            to_log(LOG_INFO, "whisper-tiny jfk eyeball: '{segs[0].text}'\n")
            t |> equal(segs[0].t0, 0l)
            t |> equal(segs[0].t1, 1050l)
            t |> equal(length(segs[0].tokens), length(want))
            var mismatches = 0
            for (a, b in segs[0].tokens, want) {
                if (a != b) {
                    mismatches++
                }
            }
            t |> success(mismatches == 0, "token-for-token ({mismatches} mismatches)")
            t |> success(segs[0].text == " And so my fellow Americans ask not what your country can do for you, ask what you can do for your country.", "text")

            // the push-chunks rail lands on the same result: feed in 2 chunks, flush
            var inscope s2 <- create_session(w)
            let half = length(samples) / 2
            var part : array<float>
            part |> reserve(length(samples))
            for (i in range(half)) {
                part |> push(samples[i])
            }
            s2 |> feed(part)
            part |> clear()
            for (i in range(half, length(samples))) {
                part |> push(samples[i])
            }
            s2 |> feed(part)
            var flushed = ""
            flush(w, s2) $(seg) {
                flushed = "{flushed}{seg.text}"
            }
            t |> success(flushed == segs[0].text, "feed+flush == one-shot")

            // every emitted segment carries a real confidence (greedy plogs are <= 0, and a
            // clean read of jfk stays well above the no-speech −1.0 bar)
            t |> success(segs[0].avg_logprob < 0.0 && segs[0].avg_logprob > -1.0, "avg_logprob in (-1, 0)")

            // lang="auto" detects English on the same clip and lands on the identical text
            var inscope s3 <- create_session(w, "auto")
            t |> equal(s3.lang_id, -1, "auto starts unresolved")
            var auto_text = ""
            transcribe(w, s3, samples) $(seg) {
                auto_text = "{auto_text}{seg.text}"
            }
            t |> equal(s3.lang_id, 0, "detected en")
            t |> success(auto_text == segs[0].text, "auto == en text")
        }
    }
}

//! the tokens two transcriptions disagree on, segment by segment (the shorter's count where the lengths differ), and how many were compared
def private segs_token_flips(a_segs, b_segs : array<TranscribeSegment>) : tuple<flips : int; total : int> {
    var flips = 0
    var total = 0
    for (a, b in a_segs, b_segs) {
        for (x, y in a.tokens, b.tokens) {
            total++
            if (x != y) {
                flips++
            }
        }
    }
    return (flips = flips, total = total)
}

def private assert_segs_token_identical(tt : T?; a_segs, b_segs : array<TranscribeSegment>; tag : string) {
    tt |> equal(length(a_segs), length(b_segs), "{tag}: segment count")
    for (a, b in a_segs, b_segs) {
        tt |> equal(length(a.tokens), length(b.tokens), "{tag}: token count per segment")
        tt |> success(a.text == b.text, "{tag}: text")
    }
    let f = segs_token_flips(a_segs, b_segs)
    tt |> success(f.flips == 0, "{tag}: token-identical ({f.flips}/{f.total} flips)")
}

//! a served whisper model minted in memory from a staged read (never the `.dlim` rail); `zero_from` >= 0 zeroes the q8 encoder's GEMM planes of every block from that one on
def private mint_whisper(path : string; enc_q8, dec_q8 : bool; zero_from : int64) : WhisperModel {
    var inscope stg <- stage_whisper_model(path, enc_q8, dec_q8)
    if (zero_from >= 0l) {
        let qoff = stg.e.enc.qlayers_off
        let qstride = stg.e.enc.qlayer_stride
        for (b in range64(zero_from, stg.e.enc.n_layer)) {
            for (i in range64(qstride)) {
                stg.enc.qblob[qoff + b * qstride + i] = int8(0)
            }
        }
    }
    var w = WhisperModel()
    mint_whisper_model(stg, w, enc_q8, dec_q8)
    return <- w
}

//! the rows are `whisper_encode`'s `xb` over jfk's first 30 s window three ways - the exact f32 encoder minted in memory, `w`'s q8 encoder on the CPU and on the device - then the input poison: a q8 encoder with every block's GEMM planes from the middle one on zeroed, through the device chain, must EXCEED the bar
[cold_path]
def private whisper_rows_twin(tt : T?; name, model_path : string; w : WhisperModel; samples : array<float>) {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        let tag = "{name} jfk encoder rows"
        var padded : array<float>
        var mel = WhisperMel()
        log_mel_whisper(samples, w.mel, w.filters, w.enc.n_mel, padded, mel)
        var win = MelChunk(n_mel = w.enc.n_mel, n_len = AUDIO_CHUNK_FRAMES)
        win.data |> resize(w.enc.n_mel * AUDIO_CHUNK_FRAMES)
        whisper_mel_window(mel, 0l, win)
        set_vulkan_tower(false)
        var rows_exact : array<float>
        {
            var inscope wx <- mint_whisper(model_path, false, false, -1l)
            tt |> success(!wx.enc.q8, "{tag}: the exact encoder minted for the compare")
            var esx <- make_encoder_state(wx.enc)
            whisper_encode(wx.enc, esx, win)
            rows_exact := esx.xb
            delete esx
        }
        var es <- make_encoder_state(w.enc)
        whisper_encode(w.enc, es, win)
        var rows_cpu := es.xb
        set_vulkan_tower(true)
        let r0 = vulkan_tower_stats()
        whisper_encode(w.enc, es, win)
        tt |> equal(vulkan_tower_stats().encodes - r0.encodes, 1l, "{tag}: the device encode served")
        let bar = tower_twin_bar(tt, tag, rows_exact, rows_cpu, es.xb, w.enc.n_layer, double(TWIN_SLACK_VK), TWIN_REL_L2_FLOOR)
        var inscope wz <- mint_whisper(model_path, true, true, w.enc.n_layer / 2l)
        tt |> success(wz.enc.q8, "{tag}: the poisoned encoder is on the q8 lane")
        var esz <- make_encoder_state(wz.enc)
        let z0 = vulkan_tower_stats()
        whisper_encode(wz.enc, esz, win)
        tt |> equal(vulkan_tower_stats().encodes - z0.encodes, 1l, "{tag}: the poisoned device encode served")
        if (bar > 0.0lf) {
            tower_twin_poison(tt, tag, rows_exact, esz.xb, bar)
        }
        delete esz
        delete rows_cpu
        delete rows_exact
        delete es
        delete win
        delete mel
        delete padded
    }
}

[test, arch(at = "../ARCHITECTURE_GPU_TOWER_VULKAN.md#vk-tower-encode-chains")]
def test_whisper_vulkan_twin(t : T?) {
    for (name in ["ggml-tiny.bin", "ggml-large-v3-turbo.bin"]) {
        t |> run("{name}: the Vulkan block loop vs the CPU q8 chain on jfk, token for token") @(tt : T?) {
            static_if (typeinfo builtin_module_exists(vulkan)) {
                if (!jit_enabled()) {
                    tt |> skip("interpreted (dasLLAMA model tests are JIT-only)")
                    return
                }
                let model_path = path_join(whisper_dir(), name)
                if (!model_available(tt, model_path)) {
                    return
                }
                let samples <- read_wav_pcm16_mono(path_join(models_dir(), "jfk.wav"))
                if (empty(samples)) {
                    tt |> skip("jfk.wav not present")
                    return
                }
                if (!vulkan_twin_seat(tt)) {
                    return
                }
                let tower_was_on = vulkan_tower_on()
                let wdec_was_on = vulkan_wdec_on()
                defer() {
                    set_vulkan_tower(tower_was_on)
                    set_vulkan_wdec(wdec_was_on)
                }
                set_vulkan_wdec(false)   // the decoder rail stays on the CPU so only the encoder varies between the legs
                set_asr_fp32(false)
                set_asr_tower_fp32(false)
                var inscope m <- load_asr_model(model_path)
                tt |> success(m.whisper.enc.q8, "default load is q8")
                with_job_que() {
                    setup_dasllama_jobque_()
                    set_vulkan_tower(false)
                    let d0 = vulkan_tower_stats()
                    var inscope s <- create_session(m)
                    var inscope cpu_segs : array<TranscribeSegment>
                    transcribe(m, s, samples) $(seg) {
                        cpu_segs |> push_clone(seg)
                    }
                    let d1 = vulkan_tower_stats()
                    set_vulkan_tower(true)
                    var inscope sg <- create_session(m)
                    var inscope dev_segs : array<TranscribeSegment>
                    transcribe(m, sg, samples) $(seg) {
                        dev_segs |> push_clone(seg)
                    }
                    let d2 = vulkan_tower_stats()
                    if (d2.encodes == d1.encodes) {
                        to_log(LOG_INFO, "{name} vulkan twin: the driver declined - {vulkan_tower_declines()}\n")
                    }
                    let n_enc = d2.encodes - d1.encodes
                    tt |> equal(d1.encodes - d0.encodes, 0l, "the knob-off leg never dispatched")
                    tt |> success(d1.declines - d0.declines >= 1l, "the knob-off leg recorded the knob decline")
                    tt |> success(n_enc >= 1l, "the device served the chunk encodes ({n_enc})")
                    tt |> equal(d2.blocks - d1.blocks, n_enc * m.whisper.enc.n_layer, "every block of every served encode ran on the driver")
                    tt |> equal(d2.rows - d1.rows, n_enc * m.whisper.enc.n_ctx, "the rows delta is the chunks' positions")
                    front_served(tt, "{name} jfk conv stem", d2.convs - d1.convs, n_enc, d2.declines - d1.declines)
                    let cpu_text = segs_text(cpu_segs)
                    let dev_text = segs_text(dev_segs)
                    let f = segs_token_flips(cpu_segs, dev_segs)
                    to_log(LOG_INFO, "{name} cpu q8 jfk: '{cpu_text}'\n")
                    to_log(LOG_INFO, "{name} vulkan jfk: '{dev_text}' ({f.flips}/{f.total} token flips against the CPU q8 chain)\n")
                    tt |> equal(length(cpu_segs), length(dev_segs), "{name} jfk: segment count")
                    tt |> success(cpu_text == dev_text, "{name} jfk cpu q8 / vulkan: the same text (two q8 chains, the device's summation order its own: a near-tie token may flip, the words do not)")
                    whisper_rows_twin(tt, name, model_path, m.whisper, samples)
                }
                delete m
                vulkan_tower_shutdown()
            } else {
                tt |> skip("dasVulkan not present")
            }
        }
    }
}

[test, arch(at = "../ARCHITECTURE_GPU_TOWER_VULKAN.md#vk-asr-decoder")]
def test_whisper_vulkan_wdec(t : T?) {
    for (name in ["ggml-tiny.bin", "ggml-large-v3-turbo.bin"]) {
        t |> run("{name}: the Vulkan decoder vs the CPU q8 decoder on jfk, token for token") @(tt : T?) {
            static_if (typeinfo builtin_module_exists(vulkan)) {
                if (!jit_enabled()) {
                    tt |> skip("interpreted (dasLLAMA model tests are JIT-only)")
                    return
                }
                let model_path = path_join(whisper_dir(), name)
                if (!model_available(tt, model_path)) {
                    return
                }
                let samples <- read_wav_pcm16_mono(path_join(models_dir(), "jfk.wav"))
                if (empty(samples)) {
                    tt |> skip("jfk.wav not present")
                    return
                }
                if (!vulkan_twin_seat(tt)) {
                    return
                }
                let tower_was_on = vulkan_tower_on()
                let wdec_was_on = vulkan_wdec_on()
                defer() {
                    set_vulkan_tower(tower_was_on)
                    set_vulkan_wdec(wdec_was_on)
                }
                set_vulkan_tower(true)   // the tower serves the encoder on both legs, so only the decoder varies
                set_asr_fp32(false)
                set_asr_tower_fp32(false)
                var inscope m <- load_asr_model(model_path)
                tt |> success(m.whisper.dq8, "default load is the q8 decoder")
                var cpu_segs : array<TranscribeSegment>
                var dev_segs : array<TranscribeSegment>
                with_job_que() {
                    setup_dasllama_jobque_()
                    set_vulkan_wdec(false)
                    let d0 = vulkan_wdec_stats()
                    var inscope s <- create_session(m)
                    transcribe(m, s, samples) $(seg) {
                        cpu_segs |> push_clone(seg)
                    }
                    let d1 = vulkan_wdec_stats()
                    tt |> equal(d1.windows - d0.windows, 0l, "the knob-off leg never served a window")
                    tt |> equal(d1.steps - d0.steps, 0l, "the knob-off leg never served a decode step")
                    tt |> success(key_exists(vulkan_wdec_declines(), "knob"), "the knob-off leg recorded the knob decline")
                    set_vulkan_wdec(true)
                    var inscope sg <- create_session(m)
                    transcribe(m, sg, samples) $(seg) {
                        dev_segs |> push_clone(seg)
                    }
                    let d2 = vulkan_wdec_stats()
                    if (d2.windows == d1.windows) {
                        to_log(LOG_INFO, "{name} vulkan wdec: the driver declined - {vulkan_wdec_declines()}\n")
                    }
                    tt |> success(d2.windows - d1.windows >= 1l, "the device served the window's cross-KV ({d2.windows - d1.windows})")
                    tt |> success(d2.steps - d1.steps >= 20l, "the decode batches served on the device ({d2.steps - d1.steps})")
                    tt |> success(d2.tokens - d1.tokens >= d2.steps - d1.steps, "the token rows cover the steps")
                    tt |> equal(d2.declines, d1.declines, "the device leg recorded no decline")
                    tt |> equal(d2.handoffs - d1.handoffs, d2.windows - d1.windows, "every served window took the encoder rows off the tower's plane, device to device")
                    tt |> equal(d1.handoffs, d0.handoffs, "and the knob-off leg took none")
                }
                let cpu_text = segs_text(cpu_segs)
                let dev_text = segs_text(dev_segs)
                let f = segs_token_flips(cpu_segs, dev_segs)
                to_log(LOG_INFO, "{name} cpu q8 decoder jfk: '{cpu_text}'\n")
                to_log(LOG_INFO, "{name} vulkan decoder jfk: '{dev_text}' ({f.flips}/{f.total} token flips against the CPU q8 decoder)\n")
                assert_segs_token_identical(tt, cpu_segs, dev_segs, "{name} jfk cpu q8 / vulkan decoder")
                delete cpu_segs
                delete dev_segs
                delete m
                wdec_f32_declines(tt, name, model_path, samples)
                vulkan_wdec_shutdown()
                vulkan_tower_shutdown()
            } else {
                tt |> skip("dasVulkan not present")
            }
        }
    }
}

//! the stem's rows stay on the device for the block chain; with the block hooks pinned off they land in `s.x` at the decline and the
//! CPU block loop transcribes over them: the same text as the all-CPU chain, the stem counter up, the block counter still
[test]
def test_whisper_vulkan_stem_flush(t : T?) {
    t |> run("ggml-tiny.bin: the stem's pending readback lands at the block hooks' decline; the CPU blocks over it match the all-CPU chain") @(tt : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!jit_enabled()) {
                tt |> skip("interpreted (dasLLAMA model tests are JIT-only)")
                return
            }
            let model_path = path_join(whisper_dir(), "ggml-tiny.bin")
            if (!model_available(tt, model_path)) {
                return
            }
            let samples <- read_wav_pcm16_mono(path_join(models_dir(), "jfk.wav"))
            if (empty(samples)) {
                tt |> skip("jfk.wav not present")
                return
            }
            if (!vulkan_twin_seat(tt)) {
                return
            }
            let tower_was_on = vulkan_tower_on()
            let wdec_was_on = vulkan_wdec_on()
            defer() {
                set_vulkan_tower(tower_was_on)
                set_vulkan_tower_blocks(true)
                set_vulkan_wdec(wdec_was_on)
            }
            set_vulkan_wdec(false)
            set_asr_fp32(false)
            set_asr_tower_fp32(false)
            var inscope m <- load_asr_model(model_path)
            var cpu_segs : array<TranscribeSegment>
            var stem_segs : array<TranscribeSegment>
            with_job_que() {
                setup_dasllama_jobque_()
                set_vulkan_tower(false)
                var inscope sc <- create_session(m)
                transcribe(m, sc, samples) $(seg) {
                    cpu_segs |> push_clone(seg)
                }
                set_vulkan_tower(true)
                set_vulkan_tower_blocks(false)
                let s0 = vulkan_tower_stats()
                let knob0 = vulkan_tower_declines()?["knob"] ?? 0l
                var inscope sg <- create_session(m)
                transcribe(m, sg, samples) $(seg) {
                    stem_segs |> push_clone(seg)
                }
                let s1 = vulkan_tower_stats()
                tt |> success(s1.convs > s0.convs, "the stem served on the device ({s1.convs - s0.convs} dispatches)")
                tt |> equal(s1.encodes, s0.encodes, "the block hooks served nothing")
                tt |> success((vulkan_tower_declines()?["knob"] ?? 0l) > knob0, "and recorded the knob decline the stem's rows landed at")
            }
            to_log(LOG_INFO, "ggml-tiny.bin stem flush: all-CPU '{segs_text(cpu_segs)}' / device stem + CPU blocks '{segs_text(stem_segs)}'\n")
            assert_segs_token_identical(tt, cpu_segs, stem_segs, "ggml-tiny.bin jfk all-CPU / device stem + CPU blocks")
            delete cpu_segs
            delete stem_segs
            delete m
            vulkan_tower_shutdown()
        } else {
            tt |> skip("dasVulkan not present")
        }
    }
}

//! the last row's logits of the CPU chain over the flushed memory against the knob-off chain's: the same token, within a bar on the
//! logit scale (the device's cross-KV planes are the f16 feed's GEMM, the knob-off chain's the q8 dot), the bar with its poison
def private wdec_flush_compare(tt : T?; cpu, dev : array<float>) {
    tt |> equal(long_length(dev), long_length(cpu), "both chains landed the last row's logits")
    var maxc = 0.0
    var md = 0.0
    var argc = 0
    var argd = 0
    for (i in range(length(cpu))) {
        maxc = max(maxc, abs(cpu[i]))
        md = max(md, abs(dev[i] - cpu[i]))
        if (cpu[i] > cpu[argc]) {
            argc = i
        }
        if (dev[i] > dev[argd]) {
            argd = i
        }
    }
    let bar = 0.05 * maxc
    to_log(LOG_INFO, "ggml-tiny.bin wdec flush: the CPU chain over the flushed memory vs the knob-off chain - max diff {md} of {maxc} (bar {bar}), argmax {argd} vs {argc}\n")
    tt |> equal(argd, argc, "the CPU chain over the flushed memory picks the knob-off chain's token")
    tt |> success(md <= bar, "and its logits sit within the bar ({md} vs {bar})")
    tt |> success(md + 3.0 > bar, "the bar's poison: a logit moved by 3 must exceed it")
}

//! the served window's CPU-layout memory (kx, vx) lands only when the CPU chain needs it: a first batch past the step's row cap
//! declines `rows`, the pending readback flushes, and the CPU decode over it reads what the knob-off chain computes itself
[test]
def test_whisper_vulkan_wdec_flush(t : T?) {
    t |> run("ggml-tiny.bin: the pending cross-KV readback lands at the step's rows decline; the CPU chain over it matches the knob-off chain") @(tt : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!jit_enabled()) {
                tt |> skip("interpreted (dasLLAMA model tests are JIT-only)")
                return
            }
            let model_path = path_join(whisper_dir(), "ggml-tiny.bin")
            if (!model_available(tt, model_path)) {
                return
            }
            let samples <- read_wav_pcm16_mono(path_join(models_dir(), "jfk.wav"))
            if (empty(samples)) {
                tt |> skip("jfk.wav not present")
                return
            }
            if (!vulkan_twin_seat(tt)) {
                return
            }
            let tower_was_on = vulkan_tower_on()
            let wdec_was_on = vulkan_wdec_on()
            defer() {
                set_vulkan_tower(tower_was_on)
                set_vulkan_wdec(wdec_was_on)
            }
            set_vulkan_tower(true)
            set_asr_fp32(false)
            set_asr_tower_fp32(false)
            var inscope m <- load_asr_model(model_path)
            let batch <- [for (i in range(9)); m.whisper.tok_sot + i]   //! nine rows: one past the step's cap, the first batch of a window
            var cpu_logits : array<float>
            var dev_logits : array<float>
            with_job_que() {
                setup_dasllama_jobque_()
                set_vulkan_wdec(false)
                var inscope sc <- create_session(m)
                transcribe(m, sc, samples) $(_seg) {}
                sc.w.ds.n_past = 0l
                whisper_decode_batch(m.whisper, sc.w.ds, batch)
                cpu_logits := sc.w.ds.logits
                set_vulkan_wdec(true)
                let d0 = vulkan_wdec_stats()
                var inscope sg <- create_session(m)
                transcribe(m, sg, samples) $(_seg) {}
                let d1 = vulkan_wdec_stats()
                tt |> success(d1.windows - d0.windows >= 1l, "the device served the window's cross-KV ({d1.windows - d0.windows})")
                let rows0 = vulkan_wdec_declines()?["rows"] ?? 0l
                sg.w.ds.n_past = 0l
                sg.w.ds.gpu_kv_gen = 0l
                whisper_decode_batch(m.whisper, sg.w.ds, batch)
                let rows1 = vulkan_wdec_declines()?["rows"] ?? 0l
                tt |> equal(rows1 - rows0, 1l, "a first batch past the row cap declined `rows`")
                tt |> equal(vulkan_wdec_stats().steps, d1.steps, "and the device served no step: the CPU chain decoded it over the flushed memory")
                dev_logits := sg.w.ds.logits
            }
            wdec_flush_compare(tt, cpu_logits, dev_logits)
            delete cpu_logits
            delete dev_logits
            delete m
            vulkan_wdec_shutdown()
            vulkan_tower_shutdown()
        } else {
            tt |> skip("dasVulkan not present")
        }
    }
}

def private segs_text(segs : array<TranscribeSegment>) : string {
    return build_string() $(var w : StringBuilderWriter) {
        for (seg in segs) {
            w |> write(seg.text)
        }
    }
}

//! the f32 decoder, minted in memory, never serves on the Vulkan decoder: it records the `quant_mode` decline and the CPU still transcribes jfk
[cold_path]
def private wdec_f32_declines(tt : T?; name, model_path : string; samples : array<float>) {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        let tag = "{name} jfk f32 decoder"
        var inscope mf <- mint_whisper(model_path, false, false, -1l)
        tt |> success(!mf.dq8, "{tag}: the mint carries the f32 decoder")
        with_job_que() {
            setup_dasllama_jobque_()
            let f0 = vulkan_wdec_stats()
            var inscope sf <- create_session(mf)
            var n_segs = 0
            transcribe(mf, sf, samples) $(_seg) {
                n_segs++
            }
            let f1 = vulkan_wdec_stats()
            tt |> equal(f1.windows, f0.windows, "{tag}: never serves")
            tt |> success(f1.declines - f0.declines >= 1l, "{tag}: records a decline")
            tt |> success(key_exists(vulkan_wdec_declines(), "quant_mode"), "{tag}: the decline reason is quant_mode")
            tt |> success(n_segs > 0, "{tag}: the f32 rail still transcribes on the CPU")
        }
    }
}

[test]
def test_whisper_q8_gate(t : T?) {
    t |> run("tiny q8 tower (the load_asr_model default) stays token-identical to fp32 on jfk") @(t : T?) {
        if (!jit_enabled()) {
            t |> skip("interpreted (dasLLAMA model tests are JIT-only)")
            return
        }
        let model_path = path_join(whisper_dir(), "ggml-tiny.bin")
        if (!model_available(t, model_path)) return
        let samples <- read_wav_pcm16_mono(path_join(models_dir(), "jfk.wav"))
        if (empty(samples)) {
            t |> skip("jfk.wav not present")
            return
        }
        let tower_was_on = vulkan_tower_was_on()
        let wdec_was_on = vulkan_wdec_was_on()
        {
            defer() {
                restore_gpu_tower(tower_was_on)
                restore_gpu_wdec(wdec_was_on)
            }
            set_every_gpu_tower(false)
            set_every_gpu_wdec(false)
            var inscope m <- load_asr_model(model_path)     // the asr-surface default: q8 tower
            set_asr_fp32(true)
            var inscope mf <- load_asr_model(model_path)    // knob-pinned fp32 (the oracle route)
            set_asr_fp32(false)
            t |> success(m.whisper.enc.q8, "default load is q8")
            t |> success(!mf.whisper.enc.q8, "knob forces fp32")
            with_job_que() {
                setup_dasllama_jobque_()
                var inscope s <- create_session(m)
                var inscope segs : array<TranscribeSegment>
                transcribe(m, s, samples) $(seg) {
                    segs |> push_clone(seg)
                }
                var inscope sf <- create_session(mf)
                var inscope fsegs : array<TranscribeSegment>
                transcribe(mf, sf, samples) $(seg) {
                    fsegs |> push_clone(seg)
                }
                to_log(LOG_INFO, "whisper-tiny q8 jfk eyeball: '{segs[0].text}'\n")
                to_log(LOG_INFO, "whisper-tiny f32 jfk eyeball: '{fsegs[0].text}'\n")
                assert_segs_token_identical(t, segs, fsegs, "q8/fp32")
            }
            // whole-model deletes: image-backed planes are locked views, only the finalizer may release them
            delete m
            delete mf
        }
    }
}

[test]
def test_whisper_tower_fp32_knob(t : T?) {
    t |> run("tiny mixed f32-enc/q8-dec (set_asr_tower_fp32, the GPU serving mode) stays token-identical on jfk") @(t : T?) {
        if (!jit_enabled()) {
            t |> skip("interpreted (dasLLAMA model tests are JIT-only)")
            return
        }
        let model_path = path_join(whisper_dir(), "ggml-tiny.bin")
        if (!model_available(t, model_path)) return
        let samples <- read_wav_pcm16_mono(path_join(models_dir(), "jfk.wav"))
        if (empty(samples)) {
            t |> skip("jfk.wav not present")
            return
        }
        // token-identity between two GPU lanes is pinned to the SIMDGROUP kernel set: the
        // envelope-verified tensor crowns (attn_qkmm here) round differently and flip
        // whisper-tiny tokens; the crowned lane's transcription quality is the board
        // pregate's claim, not this cell's. Pinned BEFORE the loads - the baked-image
        // flavor and the PSO builds both key on the crown set.
        let prev_crowns = get_metal_tensor_crowns()
        pin_metal_tensor_crowns("")
        static_if (typeinfo builtin_module_exists(das_metal)) {
            metal_tower_shutdown()      // earlier cells built PSOs and latched crown flags -
            metal_wdec_shutdown()       // every driver the encode rides rebuilds under the pin
            metal_prefill_shutdown()    // the attention trio's crown flags cache at prefill init
        }
        set_asr_tower_fp32(true)
        var inscope mm <- load_asr_model(model_path)
        set_asr_tower_fp32(false)
        t |> success(!mm.whisper.enc.q8, "tower knob forces the fp32 encoder")
        t |> success(mm.whisper.dq8, "tower knob leaves the decoder q8")
        t |> equal(asr_exec_fmt(mm), "f32/q8", "exec fmt stamps the halves")
        var inscope m <- load_asr_model(model_path)   // the q8/q8 default = the transcript truth
        with_job_que() {
            setup_dasllama_jobque_()
            var inscope s <- create_session(m)
            var inscope segs : array<TranscribeSegment>
            transcribe(m, s, samples) $(seg) {
                segs |> push_clone(seg)
            }
            var inscope sm <- create_session(mm)
            var inscope msegs : array<TranscribeSegment>
            transcribe(mm, sm, samples) $(seg) {
                msegs |> push_clone(seg)
            }
            t |> equal(length(segs), length(msegs), "segment count")
            t |> success(!empty(segs), "q8-default produced segments")
            t |> success(!empty(msegs), "f32/q8 produced segments")
            if (!empty(segs) && !empty(msegs)) {   // an empty run reds above instead of crashing here
                to_log(LOG_INFO, "whisper-tiny q8-default jfk eyeball: '{segs[0].text}'\n")
                to_log(LOG_INFO, "whisper-tiny f32/q8 jfk eyeball: '{msegs[0].text}'\n")
            }
            var mismatches = 0
            var total = 0
            for (a, b in segs, msegs) {
                t |> equal(length(a.tokens), length(b.tokens), "token count per segment")
                for (x, y in a.tokens, b.tokens) {
                    total++
                    if (x != y) {
                        mismatches++
                    }
                }
                t |> success(a.text == b.text, "q8-default/mixed text")
            }
            t |> success(mismatches == 0, "q8-default/mixed token-identical ({mismatches}/{total} flips)")
        }
        unpin_metal_tensor_crowns()
        set_metal_tensor_crowns(prev_crowns)
        static_if (typeinfo builtin_module_exists(das_metal)) {
            metal_tower_shutdown()
            metal_wdec_shutdown()
            metal_prefill_shutdown()
        }
        delete m
        delete mm
    }
}

// lowercase word list, punctuation dropped - the grade the crowned lane answers to
def private word_forms(text : string) : array<string> {
    let lowered = to_lower(text)
    let norm = build_string() $(w) {
        peek_data(lowered) $(d) {
            for (b in d) {
                let c = int(b)
                w |> write(is_alnum(c) ? to_char(c) : " ")
            }
        }
    }
    return <- [for (w in split_by_chars(norm, " ")); w; where !empty(w)]
}

def private word_flips(a, b : array<string>) : int {
    var flips = 0
    for (x, y in a, b) {
        if (x != y) {
            flips++
        }
    }
    return flips
}

[test]
def test_whisper_crowned_word_level(t : T?) {
    t |> run("the CROWNED lanes agree at word level on jfk (the crowned-pipeline quality gate)") @(t : T?) {
        static_if (typeinfo builtin_module_exists(das_metal)) {
            if (!jit_enabled()) {
                t |> skip("interpreted (dasLLAMA model tests are JIT-only)")
                return
            }
            let model_path = path_join(whisper_dir(), "ggml-tiny.bin")
            if (!model_available(t, model_path)) return
            let samples <- read_wav_pcm16_mono(path_join(models_dir(), "jfk.wav"))
            if (empty(samples)) {
                t |> skip("jfk.wav not present")
                return
            }
            // the tolerance-graded twin of the strict cell above: crowns pinned ON, the
            // pipeline serves whatever crowned variants the box readies, and the bar is
            // WORD equality - rounding drift that flips a token or a comma passes, a
            // garbled kernel on the mixed lane's route does not. Per-VARIANT dispatch
            // witnesses (kcov row asserts) are followup 60's stronger form - tiny's hs=64
            // attention may not take the tensor QK route at all
            set_metal_tower(true)
            set_metal_wdec(true)
            set_metal_wdec_step(true)
            set_metal_wdec_step_min_d(0l)   // tiny sits under the step's serving floor - the census's same seat
            let prev_crowns = get_metal_tensor_crowns()
            pin_metal_tensor_crowns("attn_qkmm,mulmm_bf16,mulmm_q8,gemmb_q8,gemmb_sk_q8,gemm64b_q8,kq_mulmm_k4,kq_mulmm_k5,kq_mulmm_k6")
            metal_tower_shutdown()
            metal_wdec_shutdown()
            metal_prefill_shutdown()
            set_asr_tower_fp32(true)
            var inscope mm <- load_asr_model(model_path)
            set_asr_tower_fp32(false)
            var inscope m <- load_asr_model(model_path)
            let s0 = metal_tower_stats()
            let w0 = metal_wdec_stats()
            with_job_que() {
                setup_dasllama_jobque_()
                var inscope s <- create_session(m)
                var text_default = ""
                transcribe(m, s, samples) $(seg) {
                    text_default = "{text_default}{seg.text} "
                }
                var inscope sm <- create_session(mm)
                var text_mixed = ""
                transcribe(mm, sm, samples) $(seg) {
                    text_mixed = "{text_mixed}{seg.text} "
                }
                to_log(LOG_INFO, "whisper-tiny crowned q8 jfk eyeball: '{text_default}'\n")
                to_log(LOG_INFO, "whisper-tiny crowned f32/q8 jfk eyeball: '{text_mixed}'\n")
                // engage witness - without it a declining driver would compare two CPU
                // transcripts and pass vacuously. The q8-default ENCODER declines the tower
                // by design (quant_mode), so the tower delta is the mixed lane's f32 encode
                // alone; both DECODERS ride the crowned wdec
                t |> equal(metal_tower_stats().encodes - s0.encodes, 1l,
                           "the mixed lane's f32 encoder GPU-served (tower encode delta)")
                t |> success(metal_wdec_stats().steps - w0.steps > 0l,
                             "the crowned decoder served (wdec step delta)")
                let wa <- word_forms(text_default)
                let wb <- word_forms(text_mixed)
                t |> success(!empty(wa) && !empty(wb), "both crowned lanes produced words")
                t |> equal(length(wa), length(wb), "word count agrees across crowned lanes")
                let flips = word_flips(wa, wb)
                t |> equal(flips, 0, "crowned lanes word-identical ({flips}/{length(wa)} word flips)")
            }
            unpin_metal_tensor_crowns()
            set_metal_tensor_crowns(prev_crowns)
            metal_tower_shutdown()
            metal_wdec_shutdown()
            metal_prefill_shutdown()
            set_metal_tower(true)   // restore: the defaults later cells' GPU-serving claims see
            set_metal_wdec(true)
            set_metal_wdec_step(true)
            set_metal_wdec_step_min_d(1024l)   // the measured serving floor (the declaration default)
            delete m
            delete mm
        } else {
            t |> skip("das_metal not built - the crowned lanes cannot serve here")
        }
    }
}

// Transcribe one corpus clip through the uniform ASR surface (fresh session) and gate its ids
// against the frozen NeMo SALM greedy reference. Skips if the clip is absent.
def private check_canary_clip(t : T?; m : AsrModel; wav : string; expected : array<int>) {
    let samples <- read_wav_pcm16_mono(wav)
    if (empty(samples)) {
        t |> skip("{base_name(wav)} not present")
        return
    }
    var inscope s <- create_session(m, "en")
    var inscope segs : array<TranscribeSegment>
    transcribe(m, s, samples) $(seg) {
        segs |> push_clone(seg)
    }
    t |> equal(length(segs), 1, "{base_name(wav)}: one segment")
    to_log(LOG_INFO, "canary {base_name(wav)} eyeball: '{segs[0].text}'\n")
    t |> equal(length(segs[0].tokens), length(expected), "{base_name(wav)}: token count")
    for (a, b in segs[0].tokens, expected) {
        t |> equal(a, b, "{base_name(wav)}: token id")
    }
    t |> equal(s.detected_lang, "en", "{base_name(wav)}: english")
}

[test]
def test_canary_qwen_oracle(t : T?) {
    t |> run("Canary-Qwen 2.5B greedy is token-for-token with the NeMo SALM oracle") @(t : T?) {
        if (!jit_enabled()) {
            t |> skip("interpreted (dasLLAMA model tests are JIT-only)")
            return
        }
        if (!parity_full_mode()) {   // ~6.7 GB two-file (decoder GGUF + f32 encoder bin) — large-tier
            t |> skip("canary-qwen is large-tier - set DASLLAMA_PARITY_FULL=1 to run")
            return
        }
        let dec = path_join(models_dir(), "canary-qwen-2.5b-decoder-f16.gguf")
        let enc = path_join(models_dir(), "canary-qwen-2.5b-encoder-f32.bin")
        if (!model_available(t, dec) || !model_available(t, enc)) {
            return
        }
        // the two-file route: decoder GGUF (LoRA-merged Qwen3-1.7B) + "CNRY" FastConformer bin
        set_asr_kv(KVDtype.f32)   // the frozen NeMo ids are the f32 parity rail
        var inscope m <- load_asr_model(dec, enc)
        let c <- caps(m)
        t |> equal(length(c.languages), 1, "English-only")
        t |> success(!c.translate && c.timestamps == AsrTimestamps.none && !c.streaming, "no translate / timestamps / streaming")
        t |> equal(m.cenc.n_layer, 32l, "FastConformer-32")
        t |> equal(m.cenc.proj_dim, 2048l, "projects to the Qwen3 hidden width")
        with_job_que() {
            setup_dasllama_jobque_()
            // Frozen NeMo SALM greedy ids (harness/canary_qwen_expected.tsv); trailing 151645 = text EOS
            check_canary_clip(t, m, path_join(corpus_dir(), "jfk.wav"),
                [3036, 773, 847, 12357, 8877, 2548, 537, 1128, 697, 3146, 646, 653, 369, 498, 2548,
                 1128, 498, 646, 653, 369, 697, 3146, 151645])
            check_canary_clip(t, m, path_join(corpus_dir(), "librispeech/09_1089-134686-0008.wav"),
                [785, 26915, 304, 892, 806, 83909, 413, 55707, 3304, 5086, 572, 264, 9255, 11, 78908,
                 6540, 315, 5561, 151645])
            check_canary_clip(t, m, path_join(corpus_dir(), "librispeech/05_1089-134686-0004.wav"),
                [2833, 5779, 29160, 451, 11896, 374, 8580, 389, 498, 13, 7684, 3729, 9972, 151645])
        }
        set_asr_kv(KVDtype.f16)   // back to the serving default
    }
}

// Gemma-4 E2B audio: greedy no-think transcription is token-for-token with llama-mtmd-cli through
// the confident prefix. The gemma4a Conformer encoder soft tokens match mtmd's audio embeddings at
// rel ~0.0027 (float-reduction-order noise); the full transcription diverges only at the Gemma-4
// q8 decoder's OWN ~0.6-logit near-tie (the upstream raw logits at that step: three tokens within
// 0.6), the qwen3moe/gemma-26B situation. So the fixture asserts the confident head, not the tail.
// Oracle: llama-mtmd-cli -m E2B-Q8 --mmproj bf16 --audio clip --temp 0 --jinja (no-think turn).
[test]
def test_gemma4a_audio_oracle(t : T?) {
    t |> run("Gemma-4 E2B audio greedy is token-for-token with llama-mtmd-cli (confident prefix)") @(t : T?) {
        if (!jit_enabled()) {
            t |> skip("interpreted (dasLLAMA model tests are JIT-only)")
            return
        }
        let dec = path_join(models_dir(), "gemma-4-E2B-it-Q8_0.gguf")
        let mmproj = path_join(models_dir(), "mmproj-gemma-4-E2B-it-bf16.gguf")
        if (!model_available(t, dec) || !model_available(t, mmproj)) {
            return
        }
        var inscope m <- load_asr_model(dec, mmproj)
        t |> equal(m.g4a.n_layer, 12l, "gemma4a Conformer, 12 blocks")
        t |> equal(m.g4a.d_model, 1024l, "audio d_model 1024")
        t |> equal(m.g4a.n_head, 8l, "8 heads")
        t |> equal(m.g4a.proj_dim, m.dec.config.dim, "projects into the Gemma-4 embedding width")
        let wav = path_join(corpus_dir(), "gemma4a_test2.wav")
        let samples <- read_wav_pcm16_mono(wav)
        if (empty(samples)) {
            t |> skip("gemma4a_test2.wav not present")
            return
        }
        set_asr_kv(KVDtype.f32)   // the frozen mtmd-prefix ids are the f32 parity rail; below every skip
        with_job_que() {
            setup_dasllama_jobque_()
            var inscope s <- create_session(m, "en")
            var inscope segs : array<TranscribeSegment>
            transcribe(m, s, samples) $(seg) {
                segs |> push_clone(seg)
            }
            t |> equal(length(segs), 1, "one segment")
            to_log(LOG_INFO, "gemma4a eyeball: '{segs[0].text}'\n")
            // frozen llama-mtmd-cli no-think greedy ids (first 40 tokens — the confident prefix
            // before the model's own ~0.6-logit near-tie at step 48). "The New York Times from
            // July 21st, 1969. This isn't just newsprint and ink. This is the moment when..."
            let expected = [818, 1799, 3773, 11922, 699, 5505, 236743, 236778, 236770, 540,
                            236764, 236743, 236770, 236819, 236825, 236819, 236761, 1174, 5889, 236789,
                            236745, 1164, 4668, 1995, 532, 19764, 236761, 1174, 563, 506,
                            3479, 1056, 27069, 236789, 236751, 24625, 9156, 5452, 3645, 236772]
            t |> success(long_length(segs[0].tokens) >= long_length(expected),
                "emitted at least {long_length(expected)} tokens ({long_length(segs[0].tokens)})")
            var mismatches = 0
            for (i in range(min(length(expected), length(segs[0].tokens)))) {
                if (segs[0].tokens[i] != expected[i]) {
                    mismatches++
                }
            }
            t |> success(mismatches == 0, "confident prefix token-for-token ({mismatches} mismatches)")
            t |> equal(s.detected_lang, "en", "english")
        }
        set_asr_kv(KVDtype.f16)   // back to the serving default
    }
}

// Transcribe one corpus clip through the uniform ASR surface (fresh session) and gate its ids
// against the frozen llama-mtmd-cli greedy reference. Skips if the clip is absent.
def private check_q3omni_clip(t : T?; m : AsrModel; wav : string; expected : array<int>) {
    let samples <- read_wav_pcm16_mono(wav)
    if (empty(samples)) {
        t |> skip("{base_name(wav)} not present")
        return
    }
    var inscope s <- create_session(m, "auto")
    var inscope segs : array<TranscribeSegment>
    transcribe(m, s, samples) $(seg) {
        segs |> push_clone(seg)
    }
    t |> equal(length(segs), 1, "{base_name(wav)}: one segment")
    to_log(LOG_INFO, "qwen3omni {base_name(wav)} eyeball: '{segs[0].text}'\n")
    t |> equal(length(segs[0].tokens), length(expected), "{base_name(wav)}: token count")
    for (a, b in segs[0].tokens, expected) {
        t |> equal(a, b, "{base_name(wav)}: token id")
    }
}

// Qwen3-Omni-30B audio-in/text-out: the SHARED qwen3a AuT tower (proj_dim 2048) + the qwen3vlmoe
// MoE thinker (= qwen3moe + M-RoPE-degenerate-to-1D + inert deepstack for audio). Greedy transcription
// (--temp 0 --jinja, bare user turn on the non-thinking Instruct thinker) is token-for-token with
// llama-mtmd-cli on the same Q8 thinker + bf16 mmproj — the FULL id sequence incl. the trailing
// <|im_end|> (151645), with NO near-tie divergence (unlike gemma4a/qwen3moe-decoder near-ties).
// Oracle: llama-mtmd-cli -m Q8 --mmproj bf16 --audio clip -p "Transcribe the audio." --temp 0 --jinja.
[test]
def test_qwen3omni_audio_oracle(t : T?) {
    t |> run("Qwen3-Omni-30B greedy audio transcription is token-for-token with llama-mtmd-cli") @(t : T?) {
        if (!jit_enabled()) {
            t |> skip("interpreted (dasLLAMA model tests are JIT-only)")
            return
        }
        if (!parity_full_mode()) {   // ~34 GB (Q8 thinker + bf16 mmproj) — large-tier
            t |> skip("Qwen3-Omni-30B is large-tier - set DASLLAMA_PARITY_FULL=1 to run")
            return
        }
        let dec = path_join(models_dir(), "Qwen3-Omni-30B-A3B-Instruct-Q8_0.gguf")
        let mmproj = path_join(models_dir(), "mmproj-Qwen3-Omni-30B-A3B-Instruct-bf16.gguf")
        if (!model_available(t, dec) || !model_available(t, mmproj)) {
            return
        }
        set_asr_kv(KVDtype.f32)   // the frozen mtmd ids are the f32 parity rail
        var inscope m <- load_asr_model(dec, mmproj)
        t |> equal(m.dec.arch, "qwen3vlmoe", "the Qwen3-Omni MoE thinker")
        t |> equal(m.q3t.core.n_layer, 32l, "AuT tower, 32 blocks")
        t |> equal(m.q3t.core.d_model, 1280l, "audio d_model 1280")
        t |> equal(m.q3t.proj_dim, m.dec.config.dim, "projects into the thinker embedding width (2048)")
        let c <- caps(m)
        t |> success(empty(c.languages) && !c.translate && c.timestamps == AsrTimestamps.none && !c.streaming,
            "detects language internally, no translate / timestamps / streaming")
        with_job_que() {
            setup_dasllama_jobque_()
            // frozen llama-mtmd-cli --temp 0 --jinja greedy ids (trailing 151645 = <|im_end|> EOG)
            check_q3omni_clip(t, m, path_join(corpus_dir(), "jfk.wav"),
                [3036, 773, 11, 847, 12357, 8877, 11, 2548, 537, 1128, 697, 3146, 646, 653, 369,
                 498, 11, 2548, 1128, 498, 646, 653, 369, 697, 3146, 13, 151645])
            check_q3omni_clip(t, m, path_join(corpus_dir(), "librispeech/09_1089-134686-0008.wav"),
                [785, 26915, 304, 892, 806, 83909, 269, 55707, 3304, 5086, 572, 264, 9255, 11,
                 78908, 6540, 315, 5561, 13, 151645])
            check_q3omni_clip(t, m, path_join(corpus_dir(), "librispeech/05_1089-134686-0004.wav"),
                [2833, 5779, 11, 7722, 451, 613, 645, 374, 8580, 389, 498, 13, 7684, 3729, 11,
                 9972, 13, 7684, 3729, 13, 151645])
        }
        set_asr_kv(KVDtype.f16)   // back to the serving default
    }
}

[test]
def test_mel_state_reuse_deterministic(t : T?) {
    t |> run("gemma4a/canary log-mel is bit-identical across calls on one reused state") @(t : T?) {
        if (!jit_enabled()) {
            t |> skip("interpreted (dasLLAMA model tests are JIT-only)")
            return
        }
        let wav = path_join(corpus_dir(), "jfk.wav")
        let samples <- read_wav_pcm16_mono(wav)
        if (empty(samples)) {
            t |> skip("jfk.wav not present in the asr corpus")
            return
        }
        let g4a_path = path_join(models_dir(), "mmproj-gemma-4-E2B-it-bf16.gguf")
        if (model_available(t, g4a_path)) {
            var inscope enc <- load_gemma4a_encoder(g4a_path)
            var st = Gemma4aState()
            gemma4a_log_mel(enc, samples, 0l, long_length(samples), st)
            var mel1 := st.mel
            gemma4a_log_mel(enc, samples, 0l, long_length(samples), st)
            var md = 0.0
            for (a, b in mel1, st.mel) {
                md = max(md, abs(a - b))
            }
            t |> success(md == 0.0, "gemma4a mel maxdiff across state reuse is 0 (got {md})")
            delete mel1
            delete st
        }
        let cn_path = path_join(models_dir(), "canary-qwen-2.5b-encoder-f32.bin")
        if (model_available(t, cn_path)) {
            var inscope cenc <- load_canary_encoder(cn_path)
            var cst = CanaryState()
            canary_log_mel(cenc, samples, cst)
            var cmel1 := cst.mel
            canary_log_mel(cenc, samples, cst)
            var cmd = 0.0
            for (a, b in cmel1, cst.mel) {
                cmd = max(cmd, abs(a - b))
            }
            t |> success(cmd == 0.0, "canary mel maxdiff across state reuse is 0 (got {cmd})")
            delete cmel1
            delete cst
        }
    }
}
