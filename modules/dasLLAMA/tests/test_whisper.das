options gen2
options persistent_heap   // model-scale allocations — freed per test
// The uniform AsrModel/AsrSession union every backend's model+scratch by value (whisper, qwen3a,
// parakeet, canary, gemma4a) — ~5 KB / ~10 KB each now; tests holding two (q8-gate) or nesting
// load frames (parakeet) need headroom above the default. Bumped for the 6-backend union.
options stack = 131072

require dastest/testing_boost public
require dasllama/dasllama_whisper
require dasllama/dasllama_asr
require dasllama/dasllama_audio
require dasllama/dasllama_gguf
require dasllama/dasllama_math   // setup_dasllama_jobque_
require daslib/jobque_boost
require daslib/fio
require math
require strings
require _model_tier   // model_available() presence/size-tier gate (see the file's header)

// Whisper-proper tests. The f16/gelu/mel/id tests are self-contained; the loader and
// transcription tests need a whisper.cpp ggml-tiny.bin (gitignored) and skip without it.
// Oracle constants come from whisper-cli (CPU, greedy: -bs 1 -bo 1 -nf -ng) — the rig and
// the large-v3-turbo parity runs live in modules/dasLLAMA/whisper_plan.md.
def private whisper_models_dir() : string {
    let env = get_env_variable("DASLLAMA_WHISPER_DIR")
    return empty(env) ? "/Users/borisbatkin/Work/whisper.cpp/models/" : env
}

def private samples_dir() : string {
    let env = get_env_variable("DASLLAMA_MODELS_DIR")
    return empty(env) ? "/Users/borisbatkin/Work/llama.cpp/models/" : env
}

// the ASR corpus (jfk + LibriSpeech clips) lives under the repo's benchmarks/asr, gitignored
def private corpus_dir() : string {
    let env = get_env_variable("DASLLAMA_CORPUS_DIR")
    return empty(env) ? "/Users/borisbatkin/Work/daScript/modules/dasLLAMA/benchmarks/asr/corpus/" : env
}

// Minimal RIFF/WAVE reader for the 16-bit PCM mono test fixture (jfk.wav).
def private read_wav_pcm16_mono(path : string) : array<float> {
    var out : array<float>
    let f = fopen(path, "rb")
    if (f == null) {
        return <- out
    }
    fmap(f) $(var bytes : array<uint8>#) {
        let n = long_length(bytes)
        var o = 12l                       // "RIFF" size "WAVE"
        while (o + 8l <= n) {
            let id = rd_u32(bytes, o)
            let sz = int64(rd_u32(bytes, o + 4l))
            if (id == 0x61746164u) {      // "data"
                let cnt = min(sz, n - o - 8l) / 2l
                out |> resize(int(cnt))
                for (i in range64(cnt)) {
                    var v = int(rd_u16(bytes, o + 8l + i * 2l))
                    if (v >= 32768) {
                        v -= 65536
                    }
                    out[i] = float(v) / 32768.0
                }
                break
            }
            o += 8l + sz + (sz & 1l)
        }
    }
    fclose(f)
    return <- out
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
        let path = path_join(whisper_models_dir(), "ggml-tiny.bin")
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
        let model_path = path_join(whisper_models_dir(), "ggml-tiny.bin")
        if (!model_available(t, model_path)) return
        var m <- load_asr_model(model_path)   // the sniff route: ggml magic + big vocab -> whisper
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

[test]
def test_qwen3a_tower(t : T?) {
    t |> run("qwen3a tower: conv2d frontend + windowed encode matches mtmd on jfk") @(t : T?) {
        if (!jit_enabled()) {
            t |> skip("interpreted (dasLLAMA model tests are JIT-only)")
            return
        }
        let path = path_join(samples_dir(), "mmproj-Qwen3-ASR-0.6B-bf16.gguf")
        if (!model_available(t, path)) return
        let samples <- read_wav_pcm16_mono(path_join(samples_dir(), "jfk.wav"))
        if (length(samples) == 0) {
            t |> skip("jfk.wav not present")
            return
        }
        var tw <- load_qwen3a_tower(path)
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
        delete tw.core.fblob
        delete tw.cblob
    }
}

[test]
def test_parakeet_oracle(t : T?) {
    t |> run("parakeet-TDT greedy on jfk.wav is token-for-token with parakeet-cli") @(t : T?) {
        if (!jit_enabled()) {
            t |> skip("interpreted (dasLLAMA model tests are JIT-only)")
            return
        }
        let path = path_join(whisper_models_dir(), "ggml-parakeet-tdt-0.6b-v2-f32.bin")
        if (!model_available(t, path)) return
        let samples <- read_wav_pcm16_mono(path_join(samples_dir(), "jfk.wav"))
        if (length(samples) == 0) {
            t |> skip("jfk.wav not present")
            return
        }
        // through the uniform ASR surface: the sniff routes a small-vocab ggml bin to parakeet
        var m <- load_asr_model(path)
        let c <- caps(m)
        t |> equal(length(c.languages), 1, "English-only")
        t |> success(c.timestamps == AsrTimestamps.segment, "segment timestamps")
        t |> equal(m.pk.n_vocab, 1024l, "vocab")
        t |> equal(m.pk.n_layer, 24l, "layers")
        t |> equal(m.pk.durations[4], 4, "tdt durations")
        with_job_que() {
            setup_dasllama_jobque_()
            var s <- create_session(m)
            var segs : array<TranscribeSegment>
            transcribe(m, s, samples) $(seg) {
                segs |> push_clone(seg)
            }
            // parakeet-cli -ng -ps (CPU, f32 self-convert): 33 tokens, these exact ends
            t |> equal(length(segs), 1)
            t |> equal(length(segs[0].tokens), 33, "token count")
            t |> equal(segs[0].tokens[0], 123, "tok[0] '▁And'")
            t |> equal(segs[0].tokens[1], 96, "tok[1] '▁so'")
            t |> equal(segs[0].tokens[31], 353, "tok[31] 'ry'")
            t |> equal(segs[0].tokens[32], 841, "tok[32] '.'")
            t |> success(segs[0].text == "And so, my fellow Americans, ask not what your country can do for you, ask what you can do for your country.", "text")
            t |> equal(s.detected_lang, "en")

            // the default load above ran the Q8 fast path — pin that set_asr_fp32 gives the
            // bit-exact fp32 route AND that both routes stay token-identical (jfk; gb1 was
            // verified 786/786 at the q8 landing)
            set_asr_fp32(true)
            var mf <- load_asr_model(path)
            set_asr_fp32(false)
            t |> success(!mf.pk.q8, "knob forces fp32")
            t |> success(m.pk.q8, "default load is q8")
            var sf <- create_session(mf)
            var fsegs : array<TranscribeSegment>
            transcribe(mf, sf, samples) $(seg) {
                fsegs |> push_clone(seg)
            }
            t |> equal(length(fsegs), 1)
            t |> equal(length(fsegs[0].tokens), length(segs[0].tokens), "q8/fp32 token count")
            for (a, b in fsegs[0].tokens, segs[0].tokens) {
                t |> equal(a, b, "q8/fp32 token")
            }
            delete mf.pk.blob
        }
        delete m.pk.blob
    }
}

[test]
def test_parakeet_v3_oracle(t : T?) {
    t |> run("parakeet-TDT v3 (25-lang) greedy on jfk.wav is token-for-token with parakeet-cli") @(t : T?) {
        if (!jit_enabled()) {
            t |> skip("interpreted (dasLLAMA model tests are JIT-only)")
            return
        }
        let path = path_join(whisper_models_dir(), "ggml-parakeet-tdt-0.6b-v3-f32.bin")
        if (!model_available(t, path)) return
        let samples <- read_wav_pcm16_mono(path_join(samples_dir(), "jfk.wav"))
        if (length(samples) == 0) {
            t |> skip("jfk.wav not present")
            return
        }
        // the 8192-piece SPM vocab routes to parakeet AND flips the multilingual caps
        var m <- load_asr_model(path)
        let c <- caps(m)
        t |> equal(length(c.languages), 25, "25 languages")
        t |> equal(m.pk.n_vocab, 8192l, "vocab")
        t |> success(m.pk.q8, "default load is q8")
        with_job_que() {
            setup_dasllama_jobque_()
            var s <- create_session(m, "de")   // any of the 25; parakeet has no conditioning
            var segs : array<TranscribeSegment>
            transcribe(m, s, samples) $(seg) {
                segs |> push_clone(seg)
            }
            // parakeet-cli -ng -ps (CPU, f32 self-convert): 38 tokens; the q8 default stays
            // id-exact on jfk (durations too — gb1's 16/655 duration-only flips are the known
            // q8 divergence, ids/text exact there as well)
            t |> equal(length(segs), 1)
            t |> equal(length(segs[0].tokens), 38, "token count")
            t |> equal(segs[0].tokens[0], 1976, "tok[0] '▁And'")
            t |> equal(segs[0].tokens[1], 547, "tok[1] '▁so'")
            t |> equal(segs[0].tokens[36], 7893, "tok[36] 'y'")
            t |> equal(segs[0].tokens[37], 7883, "tok[37] '.'")
            t |> success(segs[0].text == "And so, my fellow Americans, ask not what your country can do for you, ask what you can do for your country.", "text")
            t |> equal(s.detected_lang, "", "v3 does not report a language")
        }
        delete m.pk.blob
    }
}

[test]
def test_whisper_transcribe_oracle(t : T?) {
    t |> run("tiny greedy on jfk.wav is token-for-token with whisper-cli") @(t : T?) {
        if (!jit_enabled()) {
            t |> skip("interpreted (dasLLAMA model tests are JIT-only)")
            return
        }
        let model_path = path_join(whisper_models_dir(), "ggml-tiny.bin")
        if (!model_available(t, model_path)) return
        let samples <- read_wav_pcm16_mono(path_join(samples_dir(), "jfk.wav"))
        if (length(samples) == 0) {
            t |> skip("jfk.wav not present")
            return
        }
        var w <- load_whisper_model(model_path)
        with_job_que() {
            setup_dasllama_jobque_()
            var s <- create_session(w)
            var segs : array<TranscribeSegment>
            transcribe(w, s, samples) $(seg) {
                segs |> push_clone(seg)
            }
            // whisper-cli -bs 1 -bo 1 -nf -ng: one segment [0, 10500ms], these exact tokens
            // (incl. the leading <|0.00|> and closing <|10.50|>)
            let want <- [50364, 400, 370, 452, 7177, 6280, 1029, 406, 437, 428, 1941, 393,
                         360, 337, 291, 11, 1029, 437, 291, 393, 360, 337, 428, 1941, 13, 50889]
            t |> equal(length(segs), 1)
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
            var s2 <- create_session(w)
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
            var s3 <- create_session(w, "auto")
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

[test]
def test_whisper_q8_gate(t : T?) {
    t |> run("tiny q8 tower (the load_asr_model default) stays token-identical to fp32 on jfk") @(t : T?) {
        if (!jit_enabled()) {
            t |> skip("interpreted (dasLLAMA model tests are JIT-only)")
            return
        }
        let model_path = path_join(whisper_models_dir(), "ggml-tiny.bin")
        if (!model_available(t, model_path)) return
        let samples <- read_wav_pcm16_mono(path_join(samples_dir(), "jfk.wav"))
        if (length(samples) == 0) {
            t |> skip("jfk.wav not present")
            return
        }
        var m <- load_asr_model(model_path)     // the asr-surface default: q8 tower
        set_asr_fp32(true)
        var mf <- load_asr_model(model_path)    // knob-pinned fp32 (the oracle route)
        set_asr_fp32(false)
        t |> success(m.whisper.enc.q8, "default load is q8")
        t |> success(!mf.whisper.enc.q8, "knob forces fp32")
        with_job_que() {
            setup_dasllama_jobque_()
            var s <- create_session(m)
            var segs : array<TranscribeSegment>
            transcribe(m, s, samples) $(seg) {
                segs |> push_clone(seg)
            }
            var sf <- create_session(mf)
            var fsegs : array<TranscribeSegment>
            transcribe(mf, sf, samples) $(seg) {
                fsegs |> push_clone(seg)
            }
            t |> equal(length(segs), length(fsegs), "segment count")
            var mismatches = 0
            var total = 0
            for (a, b in segs, fsegs) {
                t |> equal(length(a.tokens), length(b.tokens), "token count per segment")
                for (x, y in a.tokens, b.tokens) {
                    total++
                    if (x != y) {
                        mismatches++
                    }
                }
                t |> success(a.text == b.text, "q8/fp32 text")
            }
            t |> success(mismatches == 0, "q8/fp32 token-identical ({mismatches}/{total} flips)")
        }
        // whole-model deletes: image-backed planes are locked views, only the finalizer may release them
        delete m
        delete mf
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
    var s <- create_session(m, "en")
    var segs : array<TranscribeSegment>
    transcribe(m, s, samples) $(seg) {
        segs |> push_clone(seg)
    }
    t |> equal(length(segs), 1, "{base_name(wav)}: one segment")
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
            t |> skip("canary-qwen is large-tier — set DASLLAMA_PARITY_FULL=1 to run")
            return
        }
        let dec = path_join(samples_dir(), "canary-qwen-2.5b-decoder-f16.gguf")
        let enc = path_join(samples_dir(), "canary-qwen-2.5b-encoder-f32.bin")
        if (!model_available(t, dec) || !model_available(t, enc)) {
            return
        }
        // the two-file route: decoder GGUF (LoRA-merged Qwen3-1.7B) + "CNRY" FastConformer bin
        var m <- load_asr_model(dec, enc)
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
        delete m.dec.wblob
        delete m.cenc.blob
    }
}

// Gemma-4 E2B audio: greedy no-think transcription is token-for-token with llama-mtmd-cli through
// the confident prefix. The gemma4a Conformer encoder soft tokens match mtmd's audio embeddings at
// rel ~0.0027 (float-reduction-order noise); the full transcription diverges only at the Gemma-4
// q8 decoder's OWN ~0.6-logit near-tie (llama.cpp's raw logits at that step: three tokens within
// 0.6), the qwen3moe/gemma-26B situation. So the fixture asserts the confident head, not the tail.
// Oracle: llama-mtmd-cli -m E2B-Q8 --mmproj bf16 --audio clip --temp 0 --jinja (no-think turn).
[test]
def test_gemma4a_audio_oracle(t : T?) {
    t |> run("Gemma-4 E2B audio greedy is token-for-token with llama-mtmd-cli (confident prefix)") @(t : T?) {
        if (!jit_enabled()) {
            t |> skip("interpreted (dasLLAMA model tests are JIT-only)")
            return
        }
        let dec = path_join(samples_dir(), "gemma-4-E2B-it-Q8_0.gguf")
        let mmproj = path_join(samples_dir(), "mmproj-gemma-4-E2B-it-bf16.gguf")
        if (!model_available(t, dec) || !model_available(t, mmproj)) {
            return
        }
        var m <- load_asr_model(dec, mmproj)
        t |> equal(m.g4a.n_layer, 12l, "gemma4a Conformer, 12 blocks")
        t |> equal(m.g4a.d_model, 1024l, "audio d_model 1024")
        t |> equal(m.g4a.n_head, 8l, "8 heads")
        t |> equal(m.g4a.proj_dim, m.dec.config.dim, "projects into the Gemma-4 embedding width")
        let wav = path_join(corpus_dir(), "gemma4a_test2.wav")
        let samples <- read_wav_pcm16_mono(wav)
        if (empty(samples)) {
            delete m.dec.wblob
            delete m.g4a.blob
            t |> skip("gemma4a_test2.wav not present")
            return
        }
        with_job_que() {
            setup_dasllama_jobque_()
            var s <- create_session(m, "en")
            var segs : array<TranscribeSegment>
            transcribe(m, s, samples) $(seg) {
                segs |> push_clone(seg)
            }
            t |> equal(length(segs), 1, "one segment")
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
        delete m.dec.wblob
        delete m.g4a.blob
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
    var s <- create_session(m, "auto")
    var segs : array<TranscribeSegment>
    transcribe(m, s, samples) $(seg) {
        segs |> push_clone(seg)
    }
    t |> equal(length(segs), 1, "{base_name(wav)}: one segment")
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
            t |> skip("Qwen3-Omni-30B is large-tier — set DASLLAMA_PARITY_FULL=1 to run")
            return
        }
        let dec = path_join(samples_dir(), "Qwen3-Omni-30B-A3B-Instruct-Q8_0.gguf")
        let mmproj = path_join(samples_dir(), "mmproj-Qwen3-Omni-30B-A3B-Instruct-bf16.gguf")
        if (!model_available(t, dec) || !model_available(t, mmproj)) {
            return
        }
        var m <- load_asr_model(dec, mmproj)
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
        delete m.dec.wblob
        delete m.q3t.core.fblob
        delete m.q3t.cblob
    }
}
