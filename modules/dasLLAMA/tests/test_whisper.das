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
require dasllama/dasllama_tower_image   // tower_dev_halfword: a discrete home's exact encoder is the f16 lane
require dasllama/dasllama_common   // KVDtype — the oracle cells pin the f32 parity rail
require dasllama/dasllama_gguf
require dasllama/dasllama_math   // with_dasllama_jobque_
require dasllama/dasllama_par    // set_single_thread_ / get_disp_count - the parakeet decode step's one-lane leg
require daslib/jobque_boost
require daslib/fio
require daslib/defer
require math
require strings
require _model_tier   // model_available() presence/size-tier gate (see the file's header)
require ?das_metal dasllama/dasllama_metal_asr_dec   // set_metal_wdec — the CPU-claim cells pin the GPU cross-KV hook off
require ?das_metal dasllama/dasllama_metal_tower     // set_metal_tower — the mtmd-oracle cells pin the GPU conv hook off
require ?das_metal dasllama/dasllama_metal_prefill   // metal_prefill_shutdown — the crown-pin cell drops the trio's cached flags
require ?vulkan dasllama/dasllama_math_vulkan   // vk_drop_model - the lifetime cell's model drop between two windows
require ?vulkan dasllama/dasllama_vulkan_common   // g_gpu.storage_align: the decoder's alignment decline leg
require ?cuda dasllama/dasllama_cuda_decode   // cud_drop_model - the same drop on the CUDA home
require dasllama/dasllama_audio_io
require dasllama/dasllama_load   // load_model_ - the canary decoder built in memory, off the image rail
require _tower_twin
require _image_memory   // read_page_aligned: a host's own copy of an image
require dasllama/dasllama_image   // with_image_destination: the cold mint's scratch path
require _child_run   // DASLLAMA_AUDIO_LANE loads once at context init: a value a process

// Whisper-proper tests. The f16/gelu/mel/id tests are self-contained; the loader and
// transcription tests need a whisper.cpp ggml-tiny.bin (gitignored) and skip without it.
// Oracle constants come from whisper-cli (CPU, greedy: -bs 1 -bo 1 -nf -ng). The discrete-home twin
// needs ggml-tiny.bin and ggml-large-v3-turbo.bin plus jfk.wav, and runs on whisper's home (`_gpu_knobs`'
// `CUDA_AUDIO_FAMILIES`); it skips without that home's device under DASLLAMA_GPU=1, on the Vulkan home under the
// CUDA ask (DASLLAMA_CUDA=1 or set_gpu_home("cuda"), where the Vulkan tower's seats stand down - `vulkan_twin_seat`),
// on a build with das_metal, and when interpreted. The E4B facade cell skips when interpreted, without
// gemma-4-E4B-it-Q8_0.gguf (large tier) and mmproj-gemma-4-E4B-it-BF16.gguf, and without jfk.wav; its device legs run
// only where the serving tower's device stands up. The canary oracle cell is large tier (DASLLAMA_PARITY_FULL=1), and its device walk skips where no
// GPU tower serves canary's default lane.

[test(description = "`f32_to_f16` round-trips every non-NaN f16 pattern through `f16_to_f32` and matches numpy float16 on round-to-nearest-even boundaries: the overflow tie, max, and the 2^-25 subnormal tie")]
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

[test(description = "`gelu_tanh_lut` reproduces ggml's GGML_GELU_FP16 table exactly on nine inputs (numpy f16 values), including the passthrough at 10 and the clamp to zero at -10 after f16 rounding")]
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

[test(description = "The table form `gelu_tanh_lut_batch` equals the computed `gelu_tanh_lut` bit for bit on every finite f16 input and on a dense 200001-point f32 sweep of (-10.5, 10.5) that crosses the +-10 boundary roundings")]
def test_gelu_lut_batch_exhaustive(t : T?) {
    t |> run("gelu_tanh_lut_batch == gelu_tanh_lut for EVERY finite f16 input") @(t : T?) {
        // every f16 pattern as an input row; the batch (table) form must reproduce the
        // computed form bit-for-bit — the table entries ARE the computed middle branch,
        // this proves the branch structure and clamps line up too
        var a, b : array<float>
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
        var a, b : array<float>
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

[test(description = "The whisper log-mel of one second of silence has the expected frame counts and normalizes to exactly -1.5 everywhere, and its 3000-frame window keeps real frames then zero-fills past n_len")]
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

[test(description = "`whisper_lang_id` follows whisper's language order: en is 0, zh is 1, yue is 99, and an unknown code is -1")]
def test_whisper_lang_ids(t : T?) {
    t |> run("g_lang order") @(t : T?) {
        t |> equal(whisper_lang_id("en"), 0)
        t |> equal(whisper_lang_id("zh"), 1)
        t |> equal(whisper_lang_id("yue"), 99)
        t |> equal(whisper_lang_id("xx"), -1)
    }
}

[test(description = "The decoder layer offsets at width 384 advance by exactly `dec_layer_stride` from layer to layer, and layer 0's last tensor ends where layer 1's first begins")]
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

[test(description = "Loading ggml-tiny.bin yields whisper tiny's hyperparameters (51865 vocab, 384 wide, 4 layers, 99 languages), its special token ids, the tanh-LUT GELU flag and known vocab strings")]
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

[test(description = "`load_asr_model` sniffs ggml-tiny.bin as whisper and its caps report exactly the 99 languages ending before yue, translate and segment timestamps, no streaming or prompt; an auto session defers language detection")]
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

//! `samples` transcribed in English through the ASR image file `dlim`, mapped for the call alone; inside a job queue
[cold_path]
def private image_file_transcript(dlim : string; samples : array<float>) : string {
    var inscope m <- load_asr_model(dlim)
    var inscope s <- create_session(m, "en")
    return strip(transcribe(m, s, samples))
}

[test(description = "A Whisper image its host holds in memory loads through `load_asr_model` from the bytes as whisper and transcribes jfk as the image's file does, at jfk's word floor and with the file gone; with its planes zeroed the load transcribes otherwise")]
def test_whisper_from_memory(t : T?) {
    t |> run("whisper tiny: the CPU image's bytes, borrowed in place") @(t : T?) {
        let model_path = path_join(whisper_dir(), "ggml-tiny.bin")
        return if (!jit_model_available(t, model_path))
        let samples <- load_audio_16k_mono(asr_clip("jfk.wav"))
        if (empty(samples)) {
            t |> skip("jfk.wav not present")
            return
        }
        with_gpu_knobs() {
            set_every_gpu_tower(false)   // the CPU image: the one an image in memory serves
            set_every_gpu_wdec(false)
            with_dasllama_jobque_() {
                let dlim = path_join(test_temp_dir(), "whisper_memory.dlim")
                with_image_destination(dlim) {   // the cold mint lands in scratch, not beside the carrier
                    var inscope cold <- load_asr_model(model_path)
                }
                let from_file = image_file_transcript(dlim, samples)
                jfk_words_floor(t, "whisper image file", from_file)
                var raw : array<uint8>
                var size = 0ul
                let base = read_page_aligned(dlim, raw, size)
                remove(dlim)
                t |> success(base != null, "the image reads into memory")
                if (base != null) {
                    {
                        var inscope m <- load_asr_model(base, size)
                        t |> success(m.kind == AsrKind.whisper, "the bytes route to whisper on the image's baked lane")
                        var inscope s <- create_session(m, "en")
                        let from_memory = strip(transcribe(m, s, samples))
                        to_log(LOG_INFO, "whisper from memory file: {from_file}\n")
                        to_log(LOG_INFO, "whisper from memory bytes: {from_memory}\n")
                        t |> equal(from_memory, from_file, "and transcribe as the file does, with the file gone")
                    }
                    zero_planes(base, size)
                    var inscope poisoned <- load_asr_model(base, size)
                    var inscope ps <- create_session(poisoned, "en")
                    let other = strip(transcribe(poisoned, ps, samples))
                    to_log(LOG_INFO, "whisper from memory zeroed: {other}\n")
                    t |> success(other != from_file, "its planes zeroed, the load transcribes otherwise (the control)")
                }
                delete raw
            }
        }
    }
}

[test(description = "The qwen3a audio tower on the CPU f32 rail loads its shape and encodes jfk window 0 to a first token within 5e-3 of llama-mtmd's embeddings on its first four values")]
def test_qwen3a_tower(t : T?) {
    t |> run("qwen3a tower: conv2d frontend + windowed encode matches mtmd on jfk") @(t : T?) {
        let path = path_join(models_dir(), "mmproj-Qwen3-ASR-0.6B-bf16.gguf")
        return if (!jit_model_available(t, path))
        let samples <- load_audio_16k_mono(asr_clip("jfk.wav"))
        if (empty(samples)) {
            t |> skip("jfk.wav not present")
            return
        }
        // reference-parity cell: pinned to the f32 rail (the q8 rail has its own A/B cell below)
        // and to the CPU conv chain — the documented ~1e-4 reorder bars are CPU claims
        with_gpu_knobs() {
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
            with_dasllama_jobque_() {
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

[test(description = "The qwen3a log-mel of a 10-second two-tone signal computed across the job lanes equals the single-lane image element for element")]
def test_qwen3a_mel_lanes(t : T?) {
    t |> run("qwen3a mel: the lanes' image is the one-lane image, element for element") @(t : T?) {
        var samples : array<float>
        samples |> resize(16000 * 10)
        for (i in range(16000 * 10)) {
            samples[i] = 0.3 * sin(float(i) * 0.01) + 0.1 * sin(float(i) * 0.37)
        }
        with_dasllama_jobque_() {
            set_single_thread_(true)
            var inscope one <- log_mel_qwen3a(samples, 128l)
            set_single_thread_(false)
            var inscope lanes <- log_mel_qwen3a(samples, 128l)
            t |> equal(one.n_len, lanes.n_len, "the two images span the same frames")
            t |> success(!empty(one.data), "the image has elements")
            var apart = 0
            for (p, q in one.data, lanes.data) {
                if (p != q) {
                    apart++
                }
            }
            t |> equal(apart, 0, "elements apart between the one-lane and the lanes' mel")
        }
    }
}

[test(description = "Parakeet-TDT v2 greedy on jfk.wav emits parakeet-cli's 33 tokens and text; a reused session and a flat heap hold, the other weight lane and the one-lane decode step give the same token ids")]
def test_parakeet_oracle(t : T?) {
    t |> run("parakeet-TDT greedy on jfk.wav is token-for-token with parakeet-cli") @(t : T?) {
        let path = path_join(whisper_dir(), "ggml-parakeet-tdt-0.6b-v2-f32.bin")
        return if (!jit_model_available(t, path))
        let samples <- load_audio_16k_mono(asr_clip("jfk.wav"))
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
        with_dasllama_jobque_() {
            var inscope s <- create_session(m)
            var inscope segs <- session_segs(m, s, samples)
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

            reused_session_leg(t, m, s, samples)
            heap_flat_leg(t, m, s, samples, "parakeet")

            t |> success(m.pk.q8, "default load follows the lane policy - q8 on every home")
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
            // the decode step's one-lane form against its team publish: the same rows, columns and order either way
            let was_single = get_single_thread_()
            set_single_thread_(false)
            let d0 = get_disp_count()
            var inscope s0 <- create_session(m)
            var inscope tsegs <- session_segs(m, s0, samples)
            let d1 = get_disp_count()
            set_single_thread_(true)
            var inscope s1 <- create_session(m)
            var inscope isegs <- session_segs(m, s1, samples)
            let d2 = get_disp_count()
            set_single_thread_(was_single)
            t |> success(d1 > d0, "the team transcription dispatched ({d1 - d0} times)")
            t |> equal(d2, d1, "the one-lane transcription never dispatched")
            t |> success(length(tsegs) == 1 && length(isegs) == 1, "one segment a leg")
            to_log(LOG_INFO, "parakeet-v2 jfk team eyeball: '{tsegs[0].text}'\nparakeet-v2 jfk one-lane eyeball: '{isegs[0].text}'\n")
            t |> equal(length(isegs[0].tokens), length(tsegs[0].tokens), "one-lane token count")
            for (a, b in isegs[0].tokens, tsegs[0].tokens) {
                t |> equal(a, b, "one-lane token")
            }
        }
    }
}

[test(description = "Parakeet-TDT v3 loads as a 25-language 8192-vocab model on the q8 lane and greedy on jfk.wav emits parakeet-cli's 38 tokens and exact text, reporting no detected language")]
def test_parakeet_v3_oracle(t : T?) {
    t |> run("parakeet-TDT v3 (25-lang) greedy on jfk.wav is token-for-token with parakeet-cli") @(t : T?) {
        let path = path_join(whisper_dir(), "ggml-parakeet-tdt-0.6b-v3-f32.bin")
        return if (!jit_model_available(t, path))
        let samples <- load_audio_16k_mono(asr_clip("jfk.wav"))
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
        t |> success(m.pk.q8, "default load follows the lane policy - q8 on every home")
        with_dasllama_jobque_() {
            var inscope s <- create_session(m, "de")   // any of the 25; parakeet has no conditioning
            var inscope segs <- session_segs(m, s, samples)
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

[test(description = "Whisper tiny greedy on jfk.wav emits whisper-cli's 26 token ids, one 0-10.5 s segment and its exact text; two-chunk feed+flush and an auto session that detects English land on the same text, avg_logprob in (-1, 0)")]
def test_whisper_transcribe_oracle(t : T?) {
    t |> run("tiny greedy on jfk.wav is token-for-token with whisper-cli") @(t : T?) {
        let model_path = path_join(whisper_dir(), "ggml-tiny.bin")
        return if (!jit_model_available(t, model_path))
        let samples <- load_audio_16k_mono(asr_clip("jfk.wav"))
        if (empty(samples)) {
            t |> skip("jfk.wav not present")
            return
        }
        var w <- load_whisper_model(model_path)
        with_dasllama_jobque_() {
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

//! a whisper model minted in memory from a staged read in the flavor `dev` (`#tower-dev-block`); `zero_from` >= 0 zeroes the encoder's blocks from it on
def private mint_whisper(path : string; enc_q8, dec_q8 : bool; zero_from : int64; dev : TowerDev = TowerDev.none) : WhisperModel {
    var inscope stg <- tower_dev_staged(dev) $ { return <- stage_whisper_model(path, enc_q8, dec_q8) }
    if (zero_from >= 0l) {
        let on_qlayers = enc_q8 || stg.e.enc.gemm_f16
        let base = on_qlayers ? stg.e.enc.qlayers_off : stg.e.enc.layers_off
        let stride = on_qlayers ? stg.e.enc.qlayer_stride : stg.e.enc.layer_stride
        for (b in range64(zero_from, stg.e.enc.n_layer)) {
            for (i in range64(stride)) {
                if (enc_q8) {
                    stg.enc.qblob[base + b * stride + i] = int8(0)
                } elif (stg.e.enc.gemm_f16) {
                    stg.enc.wblob[base + b * stride + i] = uint16(0)
                } else {
                    stg.enc.fblob[base + b * stride + i] = 0.0
                }
            }
        }
    }
    var w = WhisperModel()
    mint_whisper_model(stg, w, enc_q8, dec_q8, dev)
    return <- w
}

//! the served whisper model behind the ASR facade, minted in memory on the q8 lanes the discrete twin pins in the flavor `dev` - the
//! facade loader would bake the serving lane's `.dlim` beside the file
def private mint_asr_whisper(path : string; dev : TowerDev = TowerDev.none) : AsrModel {
    var m = AsrModel(kind = AsrKind.whisper)
    m.whisper <- mint_whisper(path, true, true, -1l, dev)
    return <- m
}

def private whisper_rows_encode(enc : AudioTower; var es : EncoderState; win : MelChunk; var out : array<float>) : int64 {
    whisper_encode(enc, es, win)
    out := es.xb
    return long_length(out)
}

//! the rows are `whisper_encode`'s `xb` over jfk's first 30 s window three ways - the exact f32 encoder minted in memory, `w`'s q8
//! encoder on the CPU and `wd`'s (the home's flavored image) on the device - then the input poison: a home-flavored q8 encoder
//! with every block's GEMM planes from the middle one on zeroed, through the device chain, must EXCEED the bar
[cold_path]
def private whisper_rows_twin(tt : T?; name, model_path : string; w, wd : WhisperModel; samples : array<float>) {
    static_if (typeinfo builtin_module_exists(vulkan) || typeinfo builtin_module_exists(cuda)) {
        let tag = "{name} jfk encoder rows"
        var padded : array<float>
        var mel = WhisperMel()
        log_mel_whisper(samples, w.mel, w.filters, w.enc.n_mel, padded, mel)
        var win = MelChunk(n_mel = w.enc.n_mel, n_len = AUDIO_CHUNK_FRAMES)
        win.data |> resize(w.enc.n_mel * AUDIO_CHUNK_FRAMES)
        whisper_mel_window(mel, 0l, win)
        var es <- make_encoder_state(w.enc)
        var esd <- make_encoder_state(wd.enc)
        let spec = TwinSpec(family = "whisper", tag = tag, n_layer = w.enc.n_layer, metric = TwinMetric.rel_l2, seat_family = TowerFamily.whisper)
        var wx <- mint_whisper(model_path, false, false, -1l)
        tt |> success(!wx.enc.q8, "{tag}: the exact encoder minted for the compare")
        var esx <- make_encoder_state(wx.enc)
        var r <- twin_three_way(tt, spec, w.enc, es, wd.enc, esd, wx.enc, esx, $(e; var s; var out) => whisper_rows_encode(e, s, win, out)) $(legs) {
            tt |> equal(legs.dev.encodes - legs.cpu.encodes, 1l, "{tag}: the device encode served")
        }
        delete esx
        delete wx
        var inscope wz <- mint_whisper(model_path, true, true, w.enc.n_layer / 2l, serving_tower_dev(TowerFamily.whisper))
        tt |> success(wz.enc.q8, "{tag}: the poisoned encoder is on the q8 lane")
        var esz <- make_encoder_state(wz.enc)
        twin_poison(tt, r, w.enc.n_layer / 2l, wz.enc, esz) $(e; var s; var out) => whisper_rows_encode(e, s, win, out)
        delete esz
        delete r
        delete esd
        delete es
        delete win
        delete mel
        delete padded
    }
}

//! the exact encoder's lane on the device: the f32 CPU image vs the home's f16 image at `TWIN_F16_WHISPER_REL_L2`, the poison, then jfk transcribed on each
[cold_path]
def private whisper_exact_lane_twin(tt : T?; name, model_path : string; samples : array<float>) {
    static_if (typeinfo builtin_module_exists(vulkan) || typeinfo builtin_module_exists(cuda)) {
        let tag = "{name} jfk exact encoder"
        let home = serving_tower_dev(TowerFamily.whisper)
        var inscope mx = AsrModel(kind = AsrKind.whisper)
        mx.whisper <- mint_whisper(model_path, false, true, -1l)
        var inscope mxd = AsrModel(kind = AsrKind.whisper)
        mxd.whisper <- mint_whisper(model_path, false, true, -1l, home)
        tt |> success(!mxd.whisper.enc.q8 && mxd.whisper.enc.gemm_f16 && mxd.whisper.dq8 && mxd.whisper.enc.dev_backend == home,
            "{tag}: the f16 encoder's {home}-flavored image, its decoder q8")
        tt |> equal(asr_device_lane(mxd), "{home}", "{tag}: the facade names the home the exact lane serves on")
        var padded : array<float>
        var mel = WhisperMel()
        log_mel_whisper(samples, mx.whisper.mel, mx.whisper.filters, mx.whisper.enc.n_mel, padded, mel)
        var win = MelChunk(n_mel = mx.whisper.enc.n_mel, n_len = AUDIO_CHUNK_FRAMES)
        win.data |> resize(mx.whisper.enc.n_mel * AUDIO_CHUNK_FRAMES)
        whisper_mel_window(mel, 0l, win)
        {
            var inscope es <- make_encoder_state(mx.whisper.enc)
            var inscope esd <- make_encoder_state(mxd.whisper.enc)
            var inscope wz <- mint_whisper(model_path, false, true, mx.whisper.enc.n_layer / 2l, home)
            var inscope esz <- make_encoder_state(wz.enc)
            exact_lane_served_at(tt, "{tag} rows", TowerFamily.whisper, TWIN_F16_WHISPER_REL_L2, mx.whisper.enc, es, mxd.whisper.enc, esd, mx.whisper.enc.n_ctx, wz.enc, esz,
                $(e; var s; var out) => whisper_rows_encode(e, s, win, out))
        }
        with_dasllama_jobque_() {
            with_gpu_knobs() {
                set_serving_gpu_tower(false, TowerFamily.whisper)
                set_gpu_wdec(false)
                var inscope cpu_segs <- transcribe_segs(mx, samples)
                set_serving_gpu_tower(true, TowerFamily.whisper)
                set_gpu_wdec(true)
                let d0 = gpu_tower_counters(TowerFamily.whisper)
                let w0 = gpu_wdec_stats()
                var inscope dev_segs <- transcribe_segs(mxd, samples)
                let d1 = gpu_tower_counters(TowerFamily.whisper)
                let w1 = gpu_wdec_stats()
                let cpu_text = segs_text(cpu_segs)
                let dev_text = segs_text(dev_segs)
                to_log(LOG_INFO, "{name} cpu exact-encoder jfk: '{cpu_text}'\n")
                to_log(LOG_INFO, "{name} {home} exact-encoder jfk: '{dev_text}'\n")
                tt |> success(d1.encodes - d0.encodes >= 1l, "{tag}: the device served the chunk encodes ({d1.encodes - d0.encodes})")
                tt |> equal(d1.blocks - d0.blocks, (d1.encodes - d0.encodes) * mxd.whisper.enc.n_layer, "{tag}: every block of every served encode ran on the driver")
                tt |> success(w1.windows - w0.windows >= 1l, "{tag}: the decoder served the window's cross-KV off the exact encoder's rows")
                tt |> equal(w1.handoffs - w0.handoffs, w1.windows - w0.windows, "{tag}: every served window took the encoder rows device to device")
                tt |> success(cpu_text == dev_text, "{tag}: the CPU exact lane and the device's exact lane read the same text")
            }
        }
        delete win
        delete mel
        delete padded
    }
}

//! a storage-offset alignment past `RAIL_META_ALIGN` declines the cross-KV window `align` and the Vulkan image refuses the CPU; restored, it serves
[cold_path]
def private wdec_align_declines(tt : T?; name : string; md : WhisperModel) {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        return if (tower_family_cuda(TowerFamily.whisper))
        let tag = "{name} cross-KV on a device asking a storage-offset alignment past {RAIL_META_ALIGN} bytes"
        var inscope enc_out : array<float>
        enc_out |> resize(md.enc.n_ctx * md.n_text_state)
        var inscope ds <- make_decoder_state(md)
        let align = g_gpu.storage_align
        let w0 = gpu_wdec_stats()
        let align0 = gpu_wdec_declines()?["align"] ?? 0l
        let device0 = gpu_wdec_declines()?["device"] ?? 0l
        var refused = false
        g_gpu.storage_align = RAIL_META_ALIGN * 2l
        try {
            whisper_cross_kv(md, ds, enc_out)
        } recover {
            refused = true
        }
        g_gpu.storage_align = align
        let w1 = gpu_wdec_stats()
        tt |> equal(w1.windows, w0.windows, "{tag}: no window served")
        tt |> equal((gpu_wdec_declines()?["align"] ?? 0l) - align0, 1l, "{tag}: one align decline")
        tt |> equal((gpu_wdec_declines()?["device"] ?? 0l) - device0, 0l, "{tag}: no device decline - the alignment is its own reason")
        tt |> success(refused, "{tag}: the Vulkan image refuses the CPU chain")
        whisper_cross_kv(md, ds, enc_out)
        tt |> equal(gpu_wdec_stats().windows - w1.windows, 1l, "the control: the alignment put back, the same window serves")
    }
}

//! the f32 decoder, minted in memory, never serves on whisper's discrete decoder: it records the `quant_mode` decline and the CPU transcribes jfk
[cold_path]
def private wdec_f32_declines(tt : T?; name, model_path : string; samples : array<float>) {
    static_if (typeinfo builtin_module_exists(vulkan) || typeinfo builtin_module_exists(cuda)) {
        let tag = "{name} jfk f32 decoder"
        var inscope mf <- mint_whisper(model_path, false, false, -1l)
        tt |> success(!mf.dq8, "{tag}: the mint carries the f32 decoder")
        with_dasllama_jobque_() {
            let f0 = gpu_wdec_stats()
            var inscope sf <- create_session(mf)
            var n_segs = 0
            transcribe(mf, sf, samples) $(_seg) {
                n_segs++
            }
            let f1 = gpu_wdec_stats()
            tt |> equal(f1.windows, f0.windows, "{tag}: never serves")
            tt |> success(f1.declines - f0.declines >= 1l, "{tag}: records a decline")
            tt |> success(key_exists(gpu_wdec_declines(), "quant_mode"), "{tag}: the decline reason is quant_mode")
            tt |> success(n_segs > 0, "{tag}: the f32 rail still transcribes on the CPU")
        }
    }
}

//! The whisper lane whole on each side (`#tower-dev-block`): the CPU image on the CPU, the home's flavored image on the device, counters proving each, texts equal
[test(description = "On the discrete GPU home, whisper tiny and turbo's q8 lane read jfk with the CPU q8 lane's text while counters prove each side ran where claimed; encoder rows hold the twin bar with a poison control, the f32 decoder and bad alignment decline"), arch(at = "../ARCHITECTURE_GPU_TOWER_VULKAN.md#vk-tower-encode-chains")]
def test_whisper_discrete_twin(t : T?) {
    for (name in ["ggml-tiny.bin", "ggml-large-v3-turbo.bin"]) {
        t |> run("{name}: the discrete home's q8 lane vs the CPU q8 lane on jfk") @(tt : T?) {
            static_if (typeinfo builtin_module_exists(vulkan) || typeinfo builtin_module_exists(cuda)) {
                let model_path = path_join(whisper_dir(), name)
                jfk_twin_cell(tt, model_path, true, TowerFamily.whisper) $(samples) {
                    set_asr_fp32(false)
                    set_asr_tower_fp32(false)
                    let home = serving_tower_dev(TowerFamily.whisper)
                    var inscope m <- mint_asr_whisper(model_path)
                    var inscope md <- mint_asr_whisper(model_path, home)
                    tt |> success(m.whisper.enc.q8 && m.whisper.dq8 && md.whisper.enc.dev_backend == home, "the q8 lane's CPU image and its {home}-flavored image")
                    {
                        var inscope dsd <- make_decoder_state(md.whisper)
                        var inscope dsc <- make_decoder_state(m.whisper)
                        tt |> success(empty(dsd.kcache) && empty(dsd.kx) && empty(dsd.h) && empty(dsd.xqi), "the {home} image's decoder state carries no CPU chain buffers")
                        tt |> success(!empty(dsc.kcache) && !empty(dsc.kx) && !empty(dsc.h), "the CPU image's carries them")
                    }
                    with_dasllama_jobque_() {
                        set_serving_gpu_tower(false, TowerFamily.whisper)
                        set_gpu_wdec(false)
                        let d0 = gpu_tower_counters(TowerFamily.whisper)
                        let w0 = gpu_wdec_stats()
                        var inscope cpu_segs <- transcribe_segs(m, samples)
                        let d1 = gpu_tower_counters(TowerFamily.whisper)
                        let w1 = gpu_wdec_stats()
                        set_serving_gpu_tower(true, TowerFamily.whisper)
                        set_gpu_wdec(true)
                        var inscope dev_segs <- transcribe_segs(md, samples)
                        let d2 = gpu_tower_counters(TowerFamily.whisper)
                        let w2 = gpu_wdec_stats()
                        if (d2.encodes == d1.encodes || w2.windows == w1.windows) {
                            to_log(LOG_INFO, "{name} {home} twin: a driver declined - tower {gpu_tower_declines(TowerFamily.whisper)}, decoder {gpu_wdec_declines()}\n")
                        }
                        let n_enc = d2.encodes - d1.encodes
                        tt |> equal(d1.encodes - d0.encodes, 0l, "the CPU leg's encoder never dispatched")
                        tt |> success(d1.declines - d0.declines >= 1l, "the CPU leg recorded the tower knob's decline")
                        tt |> equal(w1.windows - w0.windows, 0l, "the CPU leg's decoder never served a window")
                        tt |> equal(w1.steps - w0.steps, 0l, "the CPU leg's decoder never served a decode step")
                        tt |> success(key_exists(gpu_wdec_declines(), "knob"), "the CPU leg recorded the decoder knob's decline")
                        tt |> success(n_enc >= 1l, "the device served the chunk encodes ({n_enc})")
                        tt |> equal(d2.blocks - d1.blocks, n_enc * md.whisper.enc.n_layer, "every block of every served encode ran on the driver")
                        tt |> equal(d2.rows - d1.rows, n_enc * md.whisper.enc.n_ctx, "the rows delta is the chunks' positions")
                        front_served(tt, "{name} jfk conv stem", d2.convs - d1.convs, n_enc, d2.declines - d1.declines)
                        tt |> success(w2.windows - w1.windows >= 1l, "the device served the window's cross-KV ({w2.windows - w1.windows})")
                        tt |> success(w2.steps - w1.steps >= 20l, "the decode batches served on the device ({w2.steps - w1.steps})")
                        tt |> success(w2.tokens - w1.tokens >= w2.steps - w1.steps, "the token rows cover the steps")
                        tt |> equal(w2.declines, w1.declines, "the device leg's decoder recorded no decline")
                        tt |> equal(w2.handoffs - w1.handoffs, w2.windows - w1.windows, "every served window took the encoder rows off the tower's plane, device to device")
                        tt |> equal(w1.handoffs, w0.handoffs, "and the CPU leg took none")
                        let cpu_text = segs_text(cpu_segs)
                        let dev_text = segs_text(dev_segs)
                        let f = segs_token_flips(cpu_segs, dev_segs)
                        to_log(LOG_INFO, "{name} cpu q8 jfk: '{cpu_text}'\n")
                        to_log(LOG_INFO, "{name} {home} jfk: '{dev_text}' ({f.flips}/{f.total} token flips against the CPU q8 lane)\n")
                        tt |> equal(length(cpu_segs), length(dev_segs), "{name} jfk: segment count")
                        tt |> success(cpu_text == dev_text, "{name} jfk cpu q8 / {home}: the same text (two q8 chains, the device's summation order its own: a near-tie token may flip, the words do not)")
                        whisper_rows_twin(tt, name, model_path, m.whisper, md.whisper, samples)
                        wdec_align_declines(tt, name, md.whisper)
                    }
                    delete md
                    delete m
                    wdec_f32_declines(tt, name, model_path, samples)
                    whisper_exact_lane_twin(tt, name, model_path, samples)
                    gpu_tower_shutdown(TowerFamily.whisper)
                }
            } else {
                tt |> skip("no discrete GPU home in this build (dasVulkan, dasCUDA)")
            }
        }
    }
}

//! A whisper.cpp q8_0 bin loads: its Q8_0 tensors read as their blocks' scale times the quants, and it transcribes what the
//! f16 bin of the same model does.
[test(description = "A whisper.cpp ggml-tiny-q8_0.bin loads with its Q8_0 tensors read as scale times quants and transcribes jfk to exactly the text the f16 tiny bin produces")]
def test_whisper_q8_0_file(t : T?) {
    t |> run("ggml-tiny-q8_0.bin: a q8_0 bin loads and reads jfk as the f16 bin does") @(tt : T?) {
        let f16_path = path_join(whisper_dir(), "ggml-tiny.bin")
        let q8_path = path_join(whisper_dir(), "ggml-tiny-q8_0.bin")
        return if (!jit_model_available(tt, f16_path) || !jit_model_available(tt, q8_path))
        let samples <- load_audio_16k_mono(asr_clip("jfk.wav"))
        if (empty(samples)) {
            tt |> skip("jfk.wav not present")
            return
        }
        set_asr_fp32(false)
        set_asr_tower_fp32(false)
        var inscope m16 <- mint_asr_whisper(f16_path)
        var inscope m8 <- mint_asr_whisper(q8_path)
        var from_f16 = ""
        var from_q8 = ""
        with_dasllama_jobque_() {
            var inscope s16 <- create_session(m16)
            from_f16 = transcribe(m16, s16, samples)
            var inscope s8 <- create_session(m8)
            from_q8 = transcribe(m8, s8, samples)
        }
        to_log(LOG_INFO, "whisper tiny f16 bin: '{from_f16}'\n")
        to_log(LOG_INFO, "whisper tiny q8_0 bin: '{from_q8}'\n")
        tt |> success(!empty(from_q8), "the q8_0 bin transcribes")
        tt |> equal(from_q8, from_f16, "and reads what the f16 bin reads")
    }
}

//! one parakeet bin staged and minted in memory on the lane `q8`, reading `samples`
def private parakeet_text(path : string; q8 : bool; samples : array<float>) : string {
    var inscope m = AsrModel(kind = AsrKind.parakeet)
    var inscope stg <- stage_parakeet_model(path, q8)
    mint_parakeet_model(stg, m.pk, q8)
    var text = ""
    with_dasllama_jobque_() {
        var inscope s <- create_session(m, "en")
        text = transcribe(m, s, samples)
    }
    return text
}

//! A parakeet q8_0 bin loads through the reader the whisper bins share, and reads what the f32 bin of the same model does.
[test(description = "A parakeet-TDT v3 q8_0 bin loads through the reader the whisper bins share and transcribes jfk to exactly the text the f32 bin of the same model produces")]
def test_parakeet_q8_0_file(t : T?) {
    t |> run("ggml-parakeet-tdt-0.6b-v3-q8_0.bin: a q8_0 bin loads and reads jfk as the f32 bin does") @(tt : T?) {
        let f32_path = path_join(whisper_dir(), "ggml-parakeet-tdt-0.6b-v3-f32.bin")
        let q8_path = path_join(whisper_dir(), "ggml-parakeet-tdt-0.6b-v3-q8_0.bin")
        return if (!jit_model_available(tt, f32_path) || !jit_model_available(tt, q8_path))
        let samples <- load_audio_16k_mono(asr_clip("jfk.wav"))
        if (empty(samples)) {
            tt |> skip("jfk.wav not present")
            return
        }
        let from_f32 = parakeet_text(f32_path, false, samples)
        let from_q8 = parakeet_text(q8_path, false, samples)
        to_log(LOG_INFO, "parakeet v3 f32 bin: '{from_f32}'\n")
        to_log(LOG_INFO, "parakeet v3 q8_0 bin: '{from_q8}'\n")
        tt |> success(!empty(from_q8), "the q8_0 bin transcribes")
        tt |> equal(from_q8, from_f32, "and reads what the f32 bin reads")
    }
}

//! The Metal tower fills the whisper blocks-with-post-norm seat: the post-norm runs behind the blocks on the device and the
//! normed rows land in `xb`, where the CPU chain's post-norm would put them. The CPU leg runs the exact lane's CPU image, the
//! device legs its Metal-flavored image (`#tower-dev-block`).
[test(description = "The Metal tower's blocks-with-post-norm seat on whisper tiny serves one device encode whose normed rows are the CPU post-norm of its own block rows within 1e-4 relative, while the un-normed rows miss by over 0.1")]
def test_whisper_metal_ln_post(t : T?) {
    t |> run("ggml-tiny.bin: the encoder's post-norm on the Metal tower lands the CPU chain's normed rows") @(tt : T?) {
        static_if (typeinfo builtin_module_exists(das_metal)) {
            let model_path = path_join(whisper_dir(), "ggml-tiny.bin")
            return if (!jit_model_available(tt, model_path))
            let samples <- load_audio_16k_mono(asr_clip("jfk.wav"))
            if (empty(samples)) {
                tt |> skip("jfk.wav not present")
                return
            }
            var inscope w <- mint_whisper(model_path, false, false, -1l)
            var inscope wd <- mint_whisper(model_path, false, true, -1l, TowerDev.metal)
            with_dasllama_jobque_() {
                var padded : array<float>
                var mel = WhisperMel()
                log_mel_whisper(samples, w.mel, w.filters, w.enc.n_mel, padded, mel)
                var win = MelChunk(n_mel = w.enc.n_mel, n_len = AUDIO_CHUNK_FRAMES)
                win.data |> resize(w.enc.n_mel * AUDIO_CHUNK_FRAMES)
                whisper_mel_window(mel, 0l, win)
                var es <- make_encoder_state(w.enc)
                set_metal_tower(false)
                tt |> success(!audio_encode_blocks_ln_post(w.enc, es, win), "tower off: the seat declines and the CPU owes the post-norm")
                var raw := es.x   // the blocks' rows before the post-norm: the control's reference
                whisper_encode(w.enc, es, win)
                var cpu := es.xb
                set_metal_tower(true)
                var esd <- make_encoder_state(wd.enc)
                let s0 = metal_tower_stats()
                tt |> success(audio_encode_blocks_ln_post(wd.enc, esd, win), "tower on: the seat served the blocks and the post-norm")
                tt |> equal(metal_tower_stats().encodes - s0.encodes, 1l, "one device encode")
                // a NaN on either side reads infinite in the difference, and in the scale where it is the reference's: the
                // ratio is then infinite or NaN, outside the bar
                let off = logits_maxdiff(esd.xb, cpu) / logits_maxabs(cpu)
                let unnormed = logits_maxdiff(esd.xb, raw) / logits_maxabs(raw)
                var normed := esd.xb
                // the post-norm is the unit: the blocks seat alone gives the device's own block rows, the CPU norms THOSE, and the
                // device's normed rows must be that image - the block chain's device-vs-CPU distance stays out of the bar
                audio_encode_blocks(wd.enc, esd, win)
                var want : array<float>
                want |> resize(length(esd.x))
                layernorm_batch(want, esd.x, w.enc.fblob, w.enc.lnp_w_off, w.enc.lnp_b_off, w.enc.d_model, w.enc.n_ctx, w.enc.eps)
                let ln_off = logits_maxdiff(normed, want) / logits_maxabs(want)
                to_log(LOG_INFO, "whisper tiny ln_post: device norm vs the CPU norm of the device's block rows {ln_off}; vs the CPU chain {off}; vs the un-normed rows {unnormed}\n")
                tt |> success(ln_off < 1e-4, "the device's post-norm is the CPU post-norm of the same rows ({ln_off})")
                tt |> success(unnormed > 0.1, "the control: against the rows before the post-norm the compare misses ({unnormed})")
                delete want
                delete normed
                delete raw
                delete cpu
                delete esd
                delete es
                delete win
                delete mel
                delete padded
            }
            metal_tower_shutdown()
        } else {
            tt |> skip("dasMetal not present")
        }
    }
}

//! a lifetime cell's seat: tiny on jfk, both drivers on, one session reused across transcriptions the way a serving worker reuses one
def private wdec_lifetime_cell(tt : T?; body : block<(m : AsrModel; var s : AsrSession; samples : array<float>) : void>) {
    static_if (typeinfo builtin_module_exists(vulkan) || typeinfo builtin_module_exists(cuda)) {
        let model_path = path_join(whisper_dir(), "ggml-tiny.bin")
        jfk_twin_cell(tt, model_path, true, TowerFamily.whisper) $(samples) {
            set_serving_gpu_tower(true, TowerFamily.whisper)
            set_gpu_wdec(true)
            set_asr_fp32(false)
            set_asr_tower_fp32(false)
            var inscope m <- mint_asr_whisper(model_path, serving_tower_dev(TowerFamily.whisper))
            with_dasllama_jobque_() {
                var inscope s <- create_session(m)
                invoke(body, m, s, samples)
            }
            delete m
            gpu_tower_shutdown(TowerFamily.whisper)
        }
    } else {
        tt |> skip("no discrete GPU home in this build (dasVulkan, dasCUDA)")
    }
}

[test(description = "After a whole-model device drop between two transcriptions on one session, whisper tiny's GPU decoder serves the cross-KV window again and reads jfk token-identical to the first transcription")]
def test_whisper_vulkan_wdec_lifetime(t : T?) {
    t |> run("ggml-tiny.bin: a model drop between two transcriptions of one session - the second serves again and reads the same") @(tt : T?) {
        wdec_lifetime_cell(tt) $(m; var s; samples) {
            var inscope first <- session_segs(m, s, samples)
            let d0 = gpu_wdec_stats()
            var dropped = false   // the server's LLM slot switch on whisper's home: every device object swept, the decoder's with them
            static_if (typeinfo builtin_module_exists(cuda)) {
                if (tower_family_cuda(TowerFamily.whisper)) {
                    cud_drop_model()
                    dropped = true
                }
            }
            static_if (typeinfo builtin_module_exists(vulkan)) {
                if (!dropped) {
                    vk_drop_model()
                }
            }
            var inscope again <- session_segs(m, s, samples)
            let d1 = gpu_wdec_stats()
            tt |> success(d1.windows - d0.windows >= 1l, "the device served the window again after the drop ({d1.windows - d0.windows})")
            assert_segs_token_identical(tt, first, again, "ggml-tiny.bin jfk before / after the drop")
        }
    }
}

//! a fresh session over `m` (an `AsrModel` or a `WhisperModel`) transcribing `samples`, its segments collected; `lang` names the
//! session's language where the cell sets one
def private transcribe_segs(m; samples : array<float>; lang : string = "") : array<TranscribeSegment> {
    if (empty(lang)) {
        var inscope s <- create_session(m)
        return <- session_segs(m, s, samples)
    }
    var inscope s <- create_session(m, lang)
    return <- session_segs(m, s, samples)
}

//! `s` transcribing `samples` - a cell reuses one session across transcriptions the way a serving worker does
def private session_segs(m; var s; samples : array<float>) : array<TranscribeSegment> {
    var segs : array<TranscribeSegment>
    transcribe(m, s, samples) $(seg) {
        segs |> push_clone(seg)
    }
    return <- segs
}

//! A serving worker transcribes clip after clip on one session: the heap after the fourth transcription of a clip is the
//! heap after the second (within 256 KB), so a transcription frees what it allocates.
def private heap_flat_leg(t : T?; m; var s; samples : array<float>; tag : string) {
    for (_i in range(2)) {
        var inscope warm <- session_segs(m, s, samples)
    }
    let h0 = heap_bytes_allocated()
    for (_i in range(2)) {
        var inscope again <- session_segs(m, s, samples)
    }
    let h1 = heap_bytes_allocated()
    t |> success(h1 <= h0 + 262144ul, "{tag}: two more transcriptions leave the heap where it stood ({h0} -> {h1} bytes)")
}

//! a serving worker's session sees clips of every length: a session that read the whole clip reads its first half as a fresh one does
def private reused_session_leg(t : T?; m; var s; samples : array<float>) {
    var inscope half <- [for (i in range(length(samples) / 2)); samples[i]]
    var inscope rsegs <- session_segs(m, s, half)
    var inscope sh <- create_session(m)
    var inscope hsegs <- session_segs(m, sh, half)
    t |> success(length(rsegs) == 1 && length(hsegs) == 1, "one segment a leg")
    to_log(LOG_INFO, "half clip, reused session: '{rsegs[0].text}'\nhalf clip, fresh session: '{hsegs[0].text}'\n")
    t |> success(length(hsegs[0].tokens) > 8, "the half clip is a transcript of its own ({length(hsegs[0].tokens)} tokens)")
    t |> equal(length(rsegs[0].tokens), length(hsegs[0].tokens), "reused-session token count")
    for (a, b in rsegs[0].tokens, hsegs[0].tokens) {
        t |> equal(a, b, "reused-session token")
    }
}

def private segs_text(segs : array<TranscribeSegment>) : string {
    return build_string() $(var w : StringBuilderWriter) {
        for (seg in segs) {
            w |> write(seg.text)
        }
    }
}

[test(description = "With GPU towers off, whisper tiny's default q8 load and the knob-pinned fp32 load transcribe jfk token-identical, same segment count and text")]
def test_whisper_q8_gate(t : T?) {
    t |> run("tiny q8 tower (the load_asr_model default) stays token-identical to fp32 on jfk") @(t : T?) {
        let model_path = path_join(whisper_dir(), "ggml-tiny.bin")
        return if (!jit_model_available(t, model_path))
        let samples <- load_audio_16k_mono(asr_clip("jfk.wav"))
        if (empty(samples)) {
            t |> skip("jfk.wav not present")
            return
        }
        with_gpu_knobs() {
            set_every_gpu_tower(false)
            set_every_gpu_wdec(false)
            var inscope m <- load_asr_model(model_path)     // the asr-surface default: q8 tower
            set_asr_fp32(true)
            var inscope mf <- load_asr_model(model_path)    // knob-pinned fp32 (the oracle route)
            set_asr_fp32(false)
            t |> success(m.whisper.enc.q8, "default load is q8")
            t |> success(!mf.whisper.enc.q8, "knob forces fp32")
            with_dasllama_jobque_() {
                var inscope segs <- transcribe_segs(m, samples)
                var inscope fsegs <- transcribe_segs(mf, samples)
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

[test(description = "`set_asr_tower_fp32` loads whisper tiny with the exact encoder (the f16 lane on a GPU home's flavor) beside the q8 decoder, stamps the exec format, and reads jfk token-identical to the q8 default with Metal crowns pinned off")]
def test_whisper_tower_fp32_knob(t : T?) {
    t |> run("tiny mixed f32-enc/q8-dec (set_asr_tower_fp32, the GPU serving mode) stays token-identical on jfk") @(t : T?) {
        let model_path = path_join(whisper_dir(), "ggml-tiny.bin")
        return if (!jit_model_available(t, model_path))
        let samples <- load_audio_16k_mono(asr_clip("jfk.wav"))
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
        let knobs_was = gpu_knobs()
        set_serving_gpu_tower(true, TowerFamily.whisper)   // the loads' flavor pick reads the tower knob
        set_asr_fp32(false)   // a leaked whole-model pin would put the decoder on f32 too
        set_asr_tower_fp32(true)
        var inscope mm <- load_asr_model(model_path)
        set_asr_tower_fp32(false)
        t |> success(!mm.whisper.enc.q8, "tower knob forces the exact encoder")
        t |> success(mm.whisper.dq8, "tower knob leaves the decoder q8")
        let f16 = tower_dev_halfword(mm.whisper.enc.dev_backend)   // a GPU home's flavor serves the exact encoder as its f16 lane
        t |> equal(mm.whisper.enc.gemm_f16, f16, "the exact encoder is the f16 lane exactly on a GPU home's flavor ({mm.whisper.enc.dev_backend})")
        t |> equal(asr_exec_fmt(mm), f16 ? "f16/q8" : "f32/q8", "exec fmt stamps the halves")
        var inscope m <- load_asr_model(model_path)   // the q8/q8 default = the transcript truth
        with_dasllama_jobque_() {
            var inscope segs <- transcribe_segs(m, samples)
            var inscope msegs <- transcribe_segs(mm, samples)
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
        set_gpu_knobs(knobs_was)
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

//! `_audio_lane_whisper_root.das` over tiny and `clip` under DASLLAMA_AUDIO_LANE=`v`, on the run's GPU home, at the info floor
//! the load's lane line prints at
def private whisper_lane_child(t : T?; v, model_path, clip : string) : string {
    var out : string
    let rc = run_child(child_cmd("_audio_lane_whisper_root.das", [(name = "DASLLAMA_AUDIO_LANE", value = v), (name = "DAS_LOG_LEVEL", value = "info")],
        "\"{model_path}\" \"{clip}\"", true), out)
    t |> equal(rc, 0, "the whisper lane root ran (DASLLAMA_AUDIO_LANE={v}):\n{out}")
    t |> success(find(out, "ROOT_DONE") >= 0, "the whisper lane root reached its end:\n{out}")
    return out
}

//! the rest of the line after `key` in `out` ("" where no line carries it)
def private line_after(out, key : string) : string {
    let at = find(out, key)
    return "" if (at < 0)
    let from = at + length(key)
    let eol = find(out, "\n", from)
    return strip(slice(out, from, eol < 0 ? length(out) : eol))
}

[test(description = "In a child process, DASLLAMA_AUDIO_LANE=16 loads tiny's exact encoder (f16 where the home is halfword) and =8 the q8 one, each lane line naming the knob, and both read the same jfk sentence past the words floor")]
def test_whisper_audio_lane_env(t : T?) {
    t |> run("DASLLAMA_AUDIO_LANE=16 takes tiny's facade load onto the exact encoder (the home's f16 lane), =8 onto q8, one jfk sentence on both") @(t : T?) {
        let model_path = path_join(whisper_dir(), "ggml-tiny.bin")
        return if (!jit_model_available(t, model_path))
        let clip = asr_clip("jfk.wav")
        if (!stat(clip).is_valid) {
            t |> skip("jfk.wav not present")
            return
        }
        let exact = whisper_lane_child(t, "16", model_path, clip)
        let q8 = whisper_lane_child(t, "8", model_path, clip)
        let lane16 = line_after(exact, "WLANE ")
        let lane8 = line_after(q8, "WLANE ")
        t |> success(find(lane16, "q8=false ") == 0, "=16 loads the exact encoder: {lane16}")
        let half = find(lane16, "halfword=true") >= 0
        t |> success(find(lane16, half ? "f16=true" : "f16=false") >= 0, "... on the f16 lane exactly where the home's flavor is halfword: {lane16}")
        t |> success(find(lane16, "dec_q8=true") >= 0, "... beside the q8 decoder: {lane16}")
        t |> success(find(lane8, "q8=true f16=false") == 0, "=8 loads the q8 encoder: {lane8}")
        let named16 = "GEMM lane {half ? "f16" : "f32"} planes - asked by DASLLAMA_AUDIO_LANE=16"
        t |> success(find(exact, named16) >= 0, "the load's lane line names the knob: {named16}\n{exact}")
        t |> success(find(q8, "whisper encoder: GEMM lane q8 - asked by DASLLAMA_AUDIO_LANE=8") >= 0, "the =8 load's lane line names the knob:\n{q8}")
        let text16 = line_after(exact, "TEXT ")
        let text8 = line_after(q8, "TEXT ")
        to_log(LOG_INFO, "whisper-tiny jfk under DASLLAMA_AUDIO_LANE=16 ({lane16}): '{text16}'\n")
        to_log(LOG_INFO, "whisper-tiny jfk under DASLLAMA_AUDIO_LANE=8 ({lane8}): '{text8}'\n")
        t |> success(!empty(text16), "=16 transcribed")
        t |> equal(text16, text8, "the same sentence on both lanes")
        jfk_words_floor(t, "=16", text16)
    }
}

[test(description = "With the Metal tensor crowns pinned on, whisper tiny's q8 and f32/q8 lanes both encode on the tower and decode on the crowned wdec, and read jfk word for word identical")]
def test_whisper_crowned_word_level(t : T?) {
    t |> run("the CROWNED lanes agree at word level on jfk (the crowned-pipeline quality gate)") @(t : T?) {
        static_if (typeinfo builtin_module_exists(das_metal)) {
            let model_path = path_join(whisper_dir(), "ggml-tiny.bin")
            return if (!jit_model_available(t, model_path))
            let samples <- load_audio_16k_mono(asr_clip("jfk.wav"))
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
            let tower_was = metal_tower_knob()
            let wdec_was = metal_wdec_knob()
            let small_was = metal_wdec_small_cpu()
            set_metal_tower(true)
            set_metal_wdec(true)
            set_metal_wdec_step(true)
            set_metal_wdec_small_cpu(false)   // the crowned step on the device: tiny's default bake runs it on the CPU
            let prev_crowns = get_metal_tensor_crowns()
            pin_metal_tensor_crowns("attn_qkmm,mulmm_bf16,mulmm_q8,gemmb_q8,gemmb_sk_q8,gemm64b_q8,kq_mulmm_k4,kq_mulmm_k5,kq_mulmm_k6")
            metal_tower_shutdown()
            metal_wdec_shutdown()
            metal_prefill_shutdown()
            set_asr_fp32(false)
            set_asr_tower_fp32(true)
            var inscope mm <- load_asr_model(model_path)
            set_asr_tower_fp32(false)
            var inscope m <- load_asr_model(model_path)
            let s0 = metal_tower_stats()
            let w0 = metal_wdec_stats()
            with_dasllama_jobque_() {
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
                // transcripts and pass vacuously. Both ENCODERS serve on the tower - the q8
                // default off its q8 planes, the mixed lane off its f32 blob - and both
                // DECODERS ride the crowned wdec
                t |> equal(metal_tower_stats().encodes - s0.encodes, 2l,
                           "both lanes' encoders GPU-served (tower encode delta)")
                t |> success(metal_wdec_stats().steps - w0.steps > 0l,
                             "the crowned decoder served (wdec step delta)")
                let wa <- transcript_words(text_default)
                let wb <- transcript_words(text_mixed)
                t |> success(!empty(wa) && !empty(wb), "both crowned lanes produced words")
                t |> equal(length(wa), length(wb), "word count agrees across crowned lanes")
                let flips = mismatch_exact(wa, wb)
                t |> equal(flips, 0, "crowned lanes word-identical ({flips}/{length(wa)} word flips)")
            }
            unpin_metal_tensor_crowns()
            set_metal_tensor_crowns(prev_crowns)
            metal_tower_shutdown()
            metal_wdec_shutdown()
            metal_prefill_shutdown()
            set_metal_tower(tower_was)
            set_metal_wdec(wdec_was)
            set_metal_wdec_step(true)
            set_metal_wdec_small_cpu(small_was)
            delete m
            delete mm
        } else {
            t |> skip("das_metal not built - the crowned lanes cannot serve here")
        }
    }
}

// Transcribe one corpus clip through the uniform ASR surface (fresh session) and gate its ids
// against the frozen NeMo SALM greedy reference. Skips if the clip is absent.
//! the widest gap a forced walk may read between its argmax logit and the oracle token's: a greedy near-tie the two
//! implementations' rounding flips (`PERF_LEDGER.md`, the Canary-Qwen oracle row)
let CN_TIE_BAR = 0.25

//! the oracle's ids fed to the decoder after the prompt, `lead` ids ahead of the step that reads them: at each step our argmax
//! and the gap from its logit down to the oracle token's (0 where they are one id)
struct private CnForced {
    picks : array<int>
    gap : array<float>
}

def private canary_forced(m : AsrModel; samples : array<float>; expected : array<int>; lead : int) : CnForced {
    var r : CnForced
    r.picks |> reserve(length(expected))
    r.gap |> reserve(length(expected))
    var inscope s <- create_session(m, "en")
    canary_prefill(m.dec, m.cenc, s.cn, samples)
    var inscope one <- [0l]
    for (i in range(length(expected))) {
        let best = int(argmax_of(s.cn.qctx.logits))
        r.picks |> push(best)
        r.gap |> push(s.cn.qctx.logits[best] - s.cn.qctx.logits[expected[i]])
        if (i + lead + 1 < length(expected)) {
            one[0] = int64(expected[i + lead])
            eval_(m.dec, s.cn.qctx, one)
        }
    }
    return <- r
}

//! One clip against the frozen oracle on one encoder route, an external-reference compare in the forced-feed form: every
//! oracle token is our argmax or within `CN_TIE_BAR` of it; the control feeds the oracle's ids one step early, every step
//! whose input moved reading the oracle token outside the bar; then the greedy ids are the oracle's up to the step where
//! the forced walk's pick leaves the oracle's - all of them where it never does
def private check_canary_route(t : T?; m : AsrModel; wav, route : string; samples : array<float>; expected : array<int>) {
    let name = "{base_name(wav)} {route}"
    var inscope fw <- canary_forced(m, samples, expected, 0)
    var worst = 0.0
    var worst_at = -1
    var agree = length(expected)
    for (i in range(length(expected))) {
        if (!(fw.gap[i] <= worst)) {   //! a NaN gap is the widest, so a NaN logit reds the bar
            worst = fw.gap[i]
            worst_at = i
        }
        if (fw.picks[i] != expected[i] && agree == length(expected)) {
            agree = i
        }
    }
    var wide_text = ""
    if (worst_at >= 0) {
        var inscope ours_at <- [int64(fw.picks[worst_at])]
        var inscope oracle_at <- [int64(expected[worst_at])]
        wide_text = ", our '{eyeball_text(m.dec, ours_at)}' against the oracle's '{eyeball_text(m.dec, oracle_at)}'"
    }
    t |> success(worst <= CN_TIE_BAR, "{name}: every oracle token is our argmax or within {CN_TIE_BAR} of it (widest {worst} at step {worst_at}{wide_text})")
    var inscope ctl <- canary_forced(m, samples, expected, 1)
    var ctl_in = 0
    var ctl_least = 1e30
    for (i in range(1, length(expected) - 1)) {
        ctl_in += !(ctl.gap[i] > CN_TIE_BAR) ? 1 : 0   //! a NaN gap counts inside, so a NaN logit reds the control too
        ctl_least = min(ctl_least, ctl.gap[i])
    }
    let ctl_steps = length(expected) - 2
    t |> success(ctl_steps > 0 && ctl_in == 0, "{name} control: fed one id early, the oracle token sits outside {CN_TIE_BAR} at every one of {ctl_steps} steps ({ctl_in} inside, nearest {ctl_least})")
    var inscope s <- create_session(m, "en")
    var inscope segs <- session_segs(m, s, samples)
    t |> equal(length(segs), 1, "{name}: one segment")
    var inscope ours <- [for (id in segs[0].tokens); int64(id)]
    var inscope oracle <- [for (id in expected); int64(id)]
    to_log(LOG_INFO, "canary {name}: the forced walk leaves the oracle's pick at step {agree} of {length(expected)} (widest gap {worst} at step {worst_at}{wide_text}), the early-fed control's nearest gap {ctl_least} over {ctl_steps} steps\n")
    to_log(LOG_INFO, "canary {name} oracle text: {eyeball_text(m.dec, oracle)}\n")
    to_log(LOG_INFO, "canary {name} greedy text: {eyeball_text(m.dec, ours)}\n")
    //! up to `agree` the oracle's ids are the forced walk's own picks, and greedy decode computes the walk's logits bit for bit:
    //! `canary_transcribe` runs the same `canary_prefill` and then one-token `eval_` steps on the same session kind
    t |> success(length(segs[0].tokens) >= agree, "{name}: the greedy ids reach step {agree}, where the forced walk's pick leaves the oracle's")
    for (i in range(min(agree, length(segs[0].tokens)))) {
        t |> equal(expected[i], segs[0].tokens[i], "{name}: token id {i}")
    }
    if (agree == length(expected)) {
        t |> equal(length(expected), length(segs[0].tokens), "{name}: token count")
    }
    t |> equal("en", s.detected_lang, "{name}: english")
}

//! the clip's frozen ids on one route of `m`'s encoder: on the device (`device`, canary's tower knob on, every forced walk and the
//! greedy decode counted as a device encode) or on the CPU chain (every tower off, no device encode - the count's control)
def private check_canary_clip(t : T?; m : AsrModel; wav, route : string; device : bool; expected : array<int>) {
    let samples <- load_audio_16k_mono(wav)
    if (empty(samples)) {
        t |> skip("{base_name(wav)} not present")
        return
    }
    with_gpu_knobs() {
        if (device) {
            set_serving_gpu_tower(true, TowerFamily.canary)
        } else {
            set_every_gpu_tower(false)
        }
        let e0 = gpu_tower_encodes(TowerFamily.canary)
        check_canary_route(t, m, wav, route, samples, expected)
        let encodes = gpu_tower_encodes(TowerFamily.canary) - e0
        if (device) {
            t |> success(encodes >= 3l, "{base_name(wav)} {route}: the tower encoded both forced walks and the greedy decode on the device ({encodes} encodes)")
        } else {
            t |> equal(encodes, 0l, "{base_name(wav)} {route}: the tower encoded nothing with every tower off (the encode count's control)")
        }
    }
}

//! the Canary encoder minted in memory on the `q8` lane in flavor `dev` (`none` the CPU image) - the facade loader would bake
//! a `.dlim` beside the file
def private mint_canary_enc(enc : string; q8 : bool; dev : TowerDev) : CanaryEncoder {
    var inscope stg <- tower_dev_staged(dev) $ { return <- stage_canary_encoder(enc, q8) }
    var e = CanaryEncoder()
    mint_canary_encoder(stg, e, dev)
    return <- e
}

//! the Canary pair behind the ASR facade, built in memory with the encoder's CPU image on its f32 lane
def private mint_asr_canary(dec, enc : string) : AsrModel {
    var d <- load_model_(dec, QuantMode.q8)
    var e <- mint_canary_enc(enc, false, TowerDev.none)
    return <- canary_asr_model(d, e, dec)
}

//! the oracle's three clips and their frozen NeMo SALM greedy ids (`harness/canary_qwen_expected.tsv`; trailing 151645 = text EOS)
def private canary_oracle_clips() : array<tuple<wav : string; ids : array<int>>> {
    return <- [
        (wav = asr_clip("jfk.wav"), ids = [3036, 773, 847, 12357, 8877, 2548, 537, 1128, 697, 3146, 646, 653, 369, 498, 2548,
                                           1128, 498, 646, 653, 369, 697, 3146, 151645]),
        (wav = asr_clip("librispeech/09_1089-134686-0008.wav"), ids = [785, 26915, 304, 892, 806, 83909, 413, 55707, 3304, 5086,
                                                                         572, 264, 9255, 11, 78908, 6540, 315, 5561, 151645]),
        (wav = asr_clip("librispeech/05_1089-134686-0004.wav"), ids = [2833, 5779, 29160, 451, 11896, 374, 8580, 389, 498, 13,
                                                                         7684, 3729, 9972, 151645])
    ]
}

[test(description = "Canary-Qwen 2.5B on three clips, on the CPU chain and the GPU tower's default lane, keeps every NeMo SALM oracle token within 0.25 logit of its argmax (one-step-early feed misses) and greedy ids match the oracle through the forced walk's agreement")]
def test_canary_qwen_oracle(t : T?) {
    t |> run("Canary-Qwen 2.5B greedy is token-for-token with the NeMo SALM oracle") @(t : T?) {
        return if (!jit_only(t))
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
        let prior_kv = get_asr_kv()
        set_asr_kv(KVDtype.f32)   // the frozen NeMo ids are the f32 parity rail
        var inscope m <- mint_asr_canary(dec, enc)
        let c <- caps(m)
        t |> equal(length(c.languages), 1, "English-only")
        t |> success(!c.translate && c.timestamps == AsrTimestamps.none && !c.streaming, "no translate / timestamps / streaming")
        t |> equal(m.cenc.n_layer, 32l, "FastConformer-32")
        t |> equal(m.cenc.proj_dim, 2048l, "projects to the Qwen3 hidden width")
        var inscope clips <- canary_oracle_clips()
        with_dasllama_jobque_() {
            for (clip in clips) {
                check_canary_clip(t, m, clip.wav, "cpu chain", false, clip.ids)
            }
            var inscope jfk <- load_audio_16k_mono(asr_clip("jfk.wav"))
            if (!empty(jfk)) {
                var inscope hs <- create_session(m, "en")
                heap_flat_leg(t, m, hs, jfk, "canary")
            }
            // the device walk: the encoder minted on the lane the home's tower serves canary by default, in that tower's flavor
            with_gpu_knobs() {
                set_serving_gpu_tower(true, TowerFamily.canary)
                reset_canary_enc_q8()
                let q8 = canary_enc_serves_q8()
                let dev = canary_dev_flavor(q8)
                if (dev == TowerDev.none) {
                    t |> skip("no GPU tower on this box serves canary's default {q8 ? "q8" : "exact"} lane - the device walk needs one")
                } else {
                    delete m.cenc
                    m.cenc <- mint_canary_enc(enc, q8, dev)
                    for (clip in clips) {
                        check_canary_clip(t, m, clip.wav, "{dev} {q8 ? "q8" : "exact"} lane", true, clip.ids)
                    }
                }
            }
        }
        set_asr_kv(prior_kv)
    }
}

// Gemma-4 E2B audio: greedy no-think transcription is token-for-token with llama-mtmd-cli through
// the confident prefix. The gemma4a Conformer encoder soft tokens match mtmd's audio embeddings at
// rel ~0.0027 (float-reduction-order noise); the full transcription diverges only at the Gemma-4
// q8 decoder's OWN ~0.6-logit near-tie (the upstream raw logits at that step: three tokens within
// 0.6), the qwen3moe/gemma-26B situation. So the fixture asserts the confident head, not the tail.
// Oracle: llama-mtmd-cli -m E2B-Q8 --mmproj bf16 --audio clip --temp 0 --jinja (no-think turn).
[test(description = "Gemma-4 E2B audio on the f32 KV rail transcribes gemma4a_test2.wav with the first 40 token ids of llama-mtmd-cli's greedy output, the confident prefix before its own near-tie, with a flat heap across reuse")]
def test_gemma4a_audio_oracle(t : T?) {
    t |> run("Gemma-4 E2B audio greedy is token-for-token with llama-mtmd-cli (confident prefix)") @(t : T?) {
        return if (!jit_only(t))
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
        let wav = asr_clip("gemma4a_test2.wav")
        let samples <- load_audio_16k_mono(wav)
        if (empty(samples)) {
            t |> skip("gemma4a_test2.wav not present")
            return
        }
        set_asr_kv(KVDtype.f32)   // the frozen mtmd-prefix ids are the f32 parity rail; below every skip
        with_dasllama_jobque_() {
            var inscope s <- create_session(m, "en")
            var inscope segs <- session_segs(m, s, samples)
            heap_flat_leg(t, m, s, samples, "gemma4a")
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

//! Gemma-4 E4B audio through the facade: the embedder widens 1536 to 2560, soft tokens off the serving GPU tower, the transcript jfk's words
[test(description = "Gemma-4 E4B audio through the ASR facade widens the encoder's 1536 to the decoder's 2560 and transcribes jfk past the words floor, with soft tokens off the serving GPU tower where its device stands up")]
def test_gemma4a_e4b_audio_facade(t : T?) {
    t |> run("Gemma-4 E4B audio through the facade: the widening embedder's soft tokens transcribe jfk") @(t : T?) {
        return if (!jit_only(t))
        let dec = path_join(models_dir(), "gemma-4-E4B-it-Q8_0.gguf")
        let mmproj = path_join(models_dir(), "mmproj-gemma-4-E4B-it-BF16.gguf")
        if (!model_available(t, dec) || !model_available(t, mmproj)) {
            return
        }
        let samples <- load_audio_16k_mono(asr_clip("jfk.wav"))
        if (empty(samples)) {
            t |> skip("jfk.wav not present")
            return
        }
        with_gpu_knobs() {
            set_serving_gpu_tower(true, TowerFamily.gemma4a)   // the tower knob the load's flavor pick and the encode read
            var inscope m <- load_asr_model(dec, mmproj)
            t |> equal(gemma4a_mid_dim(m.g4a), 1536l, "the encoder's own width")
            t |> equal(m.g4a.proj_dim, 2560l, "the embedder widens to the decoder's width")
            let device_up = serving_tower_device_up(TowerFamily.gemma4a)
            let e0 = gpu_tower_encodes(TowerFamily.gemma4a)
            var text = ""
            with_dasllama_jobque_() {
                var inscope s <- create_session(m, "en")
                var inscope segs <- session_segs(m, s, samples)
                for (sg in segs) {
                    text += sg.text
                }
                if (device_up) {
                    t |> success(gpu_tower_encodes(TowerFamily.gemma4a) > e0, "the soft tokens came off the {serving_tower_dev(TowerFamily.gemma4a)} tower")
                    knob_off_unrouted(t, "gemma4a E4B jfk", TowerFamily.gemma4a) {
                        var inscope so <- create_session(m, "en")
                        var inscope off_segs <- session_segs(m, so, samples)
                        to_log(LOG_INFO, "gemma4a E4B jfk with the tower knob off: {length(off_segs)} segments\n")
                    }
                }
            }
            jfk_words_floor(t, "gemma4a E4B jfk", text)
        }
    }
}

// Transcribe one corpus clip through the uniform ASR surface (fresh session) and gate its ids
// against the frozen llama-mtmd-cli greedy reference. Skips if the clip is absent.
def private check_q3omni_clip(t : T?; m : AsrModel; wav : string; expected : array<int>) {
    let samples <- load_audio_16k_mono(wav)
    if (empty(samples)) {
        t |> skip("{base_name(wav)} not present")
        return
    }
    var inscope segs <- transcribe_segs(m, samples, "auto")
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
[test(description = "Qwen3-Omni-30B audio-in, with the shared qwen3a AuT tower feeding the MoE thinker, transcribes jfk and two librispeech clips to llama-mtmd-cli's full greedy id sequences, trailing im_end included")]
def test_qwen3omni_audio_oracle(t : T?) {
    t |> run("Qwen3-Omni-30B greedy audio transcription is token-for-token with llama-mtmd-cli") @(t : T?) {
        return if (!jit_only(t))
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
        with_dasllama_jobque_() {
            // frozen llama-mtmd-cli --temp 0 --jinja greedy ids (trailing 151645 = <|im_end|> EOG)
            check_q3omni_clip(t, m, asr_clip("jfk.wav"),
                [3036, 773, 11, 847, 12357, 8877, 11, 2548, 537, 1128, 697, 3146, 646, 653, 369,
                 498, 11, 2548, 1128, 498, 646, 653, 369, 697, 3146, 13, 151645])
            check_q3omni_clip(t, m, asr_clip("librispeech/09_1089-134686-0008.wav"),
                [785, 26915, 304, 892, 806, 83909, 269, 55707, 3304, 5086, 572, 264, 9255, 11,
                 78908, 6540, 315, 5561, 13, 151645])
            check_q3omni_clip(t, m, asr_clip("librispeech/05_1089-134686-0004.wav"),
                [2833, 5779, 11, 7722, 451, 613, 645, 374, 8580, 389, 498, 13, 7684, 3729, 11,
                 9972, 13, 7684, 3729, 13, 151645])
        }
        set_asr_kv(KVDtype.f16)   // back to the serving default
    }
}

[test(description = "The gemma4a and canary log-mel of jfk computed twice on one reused state are bit-identical, a NaN counting as a difference")]
def test_mel_state_reuse_deterministic(t : T?) {
    t |> run("gemma4a/canary log-mel is bit-identical across calls on one reused state") @(t : T?) {
        return if (!jit_only(t))
        let wav = asr_clip("jfk.wav")
        let samples <- load_audio_16k_mono(wav)
        if (empty(samples)) {
            t |> skip("jfk.wav not present in the asr corpus")
            return
        }
        let g4a_path = path_join(models_dir(), "mmproj-gemma-4-E2B-it-bf16.gguf")
        if (model_available(t, g4a_path)) {
            var inscope g4a_stg <- stage_gemma4a_encoder(g4a_path)   //! minted in memory: the loader would bake a `.dlim` beside the file
            var inscope enc = Gemma4aEncoder()
            mint_gemma4a_encoder(g4a_stg, enc)
            var st = Gemma4aState()
            gemma4a_log_mel(enc, samples, 0l, long_length(samples), st)
            var mel1 := st.mel
            gemma4a_log_mel(enc, samples, 0l, long_length(samples), st)
            var md = 0.0
            for (a, b in mel1, st.mel) {
                md = max(md, nan_wide_diff(a, b))
            }
            t |> success(md == 0.0, "gemma4a mel maxdiff across state reuse is 0 (got {md})")
            delete mel1
            delete st
        }
        let cn_path = path_join(models_dir(), "canary-qwen-2.5b-encoder-f32.bin")
        if (model_available(t, cn_path)) {
            var inscope cn_stg <- stage_canary_encoder(cn_path, false)   //! minted in memory, the f32 lane: no `.dlim` beside the file
            var inscope cenc = CanaryEncoder()
            mint_canary_encoder(cn_stg, cenc)
            var cst = CanaryState()
            canary_log_mel(cenc, samples, cst)
            var cmel1 := cst.mel
            canary_log_mel(cenc, samples, cst)
            var cmd = 0.0
            for (a, b in cmel1, cst.mel) {
                cmd = max(cmd, nan_wide_diff(a, b))
            }
            t |> success(cmd == 0.0, "canary mel maxdiff across state reuse is 0 (got {cmd})")
            delete cmel1
            delete cst
        }
    }
}
