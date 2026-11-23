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
require dasllama/dasllama_env   // parity_full_mode: the full suite's flag
require ?das_metal dasllama/dasllama_metal_asr_dec   // set_metal_wdec — the CPU-claim cells pin the GPU cross-KV hook off
require ?das_metal dasllama/dasllama_metal_tower     // set_metal_tower — the mtmd-oracle cells pin the GPU conv hook off
require ?das_metal dasllama/dasllama_metal_prefill   // metal_prefill_shutdown — the crown-pin cell drops the trio's cached flags
require ?vulkan dasllama/dasllama_math_vulkan   // vk_drop_model - the lifetime cell's model drop between two windows
require ?vulkan dasllama/dasllama_vulkan_common   // g_gpu.storage_align: the decoder's alignment decline leg
require ?cuda dasllama/dasllama_cuda_decode   // cud_drop_model - the same drop on the CUDA home
require dasllama/dasllama_audio_io
require dasllama/dasllama_load   // load_model_ - the canary decoder built in memory, off the image rail
require _tower_twin
require _audio_twin   // jfk_twin_why: the discrete twin's reason
require _model_once   // with_model_once_at: a whisper bin's one block in the area; own_load
require _f32_tile_pin   // with_put_back: a form's pin and knobs put back however its load leaves
require _image_memory   // read_page_aligned: a host's own copy of an image
require dasllama/dasllama_image   // with_image_destination: the cold mint's scratch path
require _child_run   // DASLLAMA_AUDIO_LANE loads once at context init: a value a process
require _audio_asr_cells   // session_segs, transcribe_segs, the reused-session and flat-heap legs, segs_text

// Whisper-proper tests. The f16/gelu/mel/id tests are self-contained; the loader and
// transcription tests need a whisper.cpp ggml-tiny.bin (gitignored) and skip without it.
// Oracle constants come from whisper-cli (CPU, greedy: -bs 1 -bo 1 -nf -ng). The discrete-home twin
// needs ggml-tiny.bin and ggml-large-v3-turbo.bin plus jfk.wav, and runs on whisper's home (`_gpu_knobs`'
// `CUDA_AUDIO_FAMILIES`); it skips without that home's device under DASLLAMA_GPU=1, on the Vulkan home under the
// CUDA ask (DASLLAMA_CUDA=1 or set_gpu_home("cuda"), where the Vulkan tower's seats stand down - `vulkan_twin_seat`),
// on a build with das_metal, and when interpreted. The Qwen3-Omni pair is large tier (DASLLAMA_PARITY_FULL=1) and skips
// under a family filter leaving `qwen3` out.
// A whisper bin loads once a form, in its one `with_model_once_at` block (`whisper_form`): the facade's image forms carry
// the loader's, the surface's and the q8 gate's cells, the forms minted in memory the Metal post-norm seat's and the
// discrete twin's. A form's cells run on one home, so a compare of the CPU chain with a device chain is two forms, made
// after the block on what they kept.

let private TINY = "ggml-tiny.bin"
let private TINY_Q8_0 = "ggml-tiny-q8_0.bin"
let private TURBO = "ggml-large-v3-turbo.bin"


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

// Tiny's hyperparameters, special ids and vocab, on the `image_f32` form.
[cold_path]
def private test_whisper_loader(t : T?; w : WhisperModel) {
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

// The facade's sniff and caps, on the `image` form.
[cold_path]
def private test_asr_surface(t : T?; m : AsrModel) {
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
        own_load("reloads the model: the cold mint lands in scratch, loads back as a file, then twice from the bytes in memory")
        let model_path = path_join(whisper_dir(), "ggml-tiny.bin")
        return if (!jit_model_available(t, model_path))
        let samples <- asr_samples(t, "jfk.wav")
        return if (empty(samples))
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

// Tiny's greedy transcription of jfk against whisper-cli, on the `image_f32` form.
[cold_path]
def private test_whisper_transcribe_oracle(t : T?; w : WhisperModel) {
    return if (!jit_only(t))
    let samples <- asr_samples(t, "jfk.wav")
    return if (empty(samples))
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

//! a whisper model minted in memory from `stg`, a staged read in the flavor `dev` (`#tower-dev-block`); `zero_from` >= 0
//! zeroes the encoder's blocks from it on
def private mint_whisper(var stg : WhisperStaging; enc_q8, dec_q8 : bool; zero_from : int64; dev : TowerDev) : WhisperModel {
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

//! a whisper bin's form read off its name (`tower_form_of`): the lanes `q8` (both halves q8), `f32` (both exact) and
//! `mixed` (the exact encoder beside the q8 decoder)
def private whisper_form_of(form : string) : TowerForm => tower_form_of(form, ["q8", "f32", "mixed"], "whisper")

//! the staged read a whisper bin's form is minted from (`tower_stage_key_of`)
def private whisper_stage_key(form : string) : string => tower_stage_key_of(whisper_form_of(form))

//! a whisper bin's staged read the key `key` names: its lanes, in its image flavor
def private whisper_stage(path, key : string) : WhisperStaging {
    let f = whisper_form_of(key)
    return <- tower_dev_staged(tower_form_dev_of(f, TowerFamily.whisper)) $ { return <- stage_whisper_model(path, f.lane == "q8", f.lane != "f32") }
}

//! One form of a whisper bin as the ASR facade holds it, its block's form loader (`whisper_form_of`). The image forms are
//! the facade's own load: `image` as the run's knobs route it, `image_q8` with every GPU tower and decoder knob off (the
//! q8 CPU image), `image_f32` under the facade's fp32 pin - the pin and the knobs put back however the load leaves, a panic
//! included. Any other form is minted in memory (`mint_whisper`) from `stg`, its own copy of the bin's staging: a
//! `_z<block>` form with its encoder's blocks zeroed from that block on, an `@dev` form in the image flavor of whisper's
//! serving home.
def private whisper_form(path, form : string; var stg : WhisperStaging) : AsrModel {
    let f = whisper_form_of(form)
    var inscope m = AsrModel(kind = AsrKind.whisper)
    if (!f.image) {
        m.whisper <- mint_whisper(stg, f.lane == "q8", f.lane != "f32", f.zero_block, tower_form_dev_of(f, TowerFamily.whisper))
        return <- m
    }
    let knobs = gpu_knobs()
    if (!empty(f.lane)) {
        set_every_gpu_tower(false)
        set_every_gpu_wdec(false)
    }
    set_asr_fp32(f.lane == "f32")
    set_asr_tower_fp32(false)
    with_put_back($ {
        set_asr_fp32(false)
        set_gpu_knobs(knobs)
    }) {
        delete m
        m <- load_asr_model(path)
    }
    return <- m
}

//! The form is the lanes and image flavor it names: on `m`, a whisper bin loaded as the form `form` of `file`, each half's
//! lane flag and, on a minted form, the encoder's image flavor. The `image` form's lanes are the run's policy: not read.
def private whisper_form_is(t : T?; file, form : string; m : AsrModel) {
    let f = whisper_form_of(form)
    t |> success(m.kind == AsrKind.whisper, "{file}: the {form} form is a whisper model")
    return if (f.image && empty(f.lane))
    //! an image form pins no encoder lane short of f32: its q8 lane is whisper_tower_serves_q8's unpinned answer
    let enc_q8 = f.image && f.lane == "q8" ? whisper_tower_serves_q8() : f.lane == "q8"
    t |> success(m.whisper.enc.q8 == enc_q8 && m.whisper.dq8 == (f.lane != "f32"), "{file}: the {form} form serves the lanes it names")
    if (!f.image) {
        t |> equal(m.whisper.enc.dev_backend, tower_form_dev_of(f, TowerFamily.whisper), "{file}: the {form} form is the image flavor it names")
    }
}

def private whisper_rows_encode(enc : AudioTower; var es : EncoderState; win : MelChunk; var out : array<float>) : int64 {
    whisper_encode(enc, es, win)
    out := es.xb
    return long_length(out)
}

//! jfk's first 30 s window as `w`'s front end reads it
def private jfk_window(w : WhisperModel; samples : array<float>) : MelChunk {
    var inscope padded : array<float>
    var inscope mel = WhisperMel()
    log_mel_whisper(samples, w.mel, w.filters, w.enc.n_mel, padded, mel)
    var win = MelChunk(n_mel = w.enc.n_mel, n_len = AUDIO_CHUNK_FRAMES)
    win.data |> resize(w.enc.n_mel * AUDIO_CHUNK_FRAMES)
    whisper_mel_window(mel, 0l, win)
    return <- win
}

//! jfk's transcription on one lane as two forms keep it: the CPU image's on the CPU, the home's flavored image's on the device
struct private LanePair {
    cpu_segs : array<TranscribeSegment>
    dev_segs : array<TranscribeSegment>
    legs : int
}

//! A whisper bin's discrete twin as the forms of its file keep it, for the compares after the block
//! (`test_whisper_discrete_twin`): `form` the form whose legs run and `half` the block its poison forms zero from; the q8
//! lane's images and transcriptions, the encoder rows three ways with their poison (`TwinRows`), and the exact encoder's
//! lane - its rows (`ExactLaneKept`) and its transcriptions.
struct private WhisperTwin {
    name : string
    form : string
    half : int64
    cpu_q8 : bool            //! the `q8` form holds both halves q8
    dev_backend : TowerDev   //! the `q8@dev` form's image flavor
    q8 : LanePair
    rows : TwinRows = TwinRows()
    exact : ExactLaneKept
    mixed : LanePair
}

//! the twin's encoder rows named, and its exact lane's bar set, from the tower at hand
def private twin_named(var k : WhisperTwin; enc : AudioTower) {
    k.rows.spec = TwinSpec(family = "whisper", tag = "{k.name} jfk encoder rows", n_layer = enc.n_layer, metric = TwinMetric.rel_l2, seat_family = TowerFamily.whisper)
    k.exact.tag = "{k.name} jfk exact encoder rows"
    k.exact.fam = TowerFamily.whisper
    k.exact.bar = TWIN_F16_WHISPER_REL_L2
    k.exact.npos = enc.n_ctx
}

// The q8 lane's CPU legs, on the `q8` form: jfk transcribed with the tower and the decoder knobs off, the counters proving
// no driver served, then the encoder rows' CPU q8 leg.
[cold_path]
def private twin_q8_cpu_leg(tt : T?; m : AsrModel; samples : array<float>; var k : WhisperTwin) {
    k.cpu_q8 = m.whisper.enc.q8 && m.whisper.dq8
    {
        var inscope dsc <- make_decoder_state(m.whisper)
        tt |> success(!empty(dsc.kcache) && !empty(dsc.kx) && !empty(dsc.h), "the CPU image's carries them")
    }
    with_dasllama_jobque_() {
        with_gpu_knobs() {
            set_serving_gpu_tower(false, TowerFamily.whisper)
            set_gpu_wdec(false)
            let d0 = gpu_tower_counters(TowerFamily.whisper)
            let w0 = gpu_wdec_stats()
            k.q8.cpu_segs <- transcribe_segs(m, samples)
            let d1 = gpu_tower_counters(TowerFamily.whisper)
            let w1 = gpu_wdec_stats()
            tt |> equal(d1.encodes - d0.encodes, 0l, "the CPU leg's encoder never dispatched")
            tt |> success(d1.declines - d0.declines >= 1l, "the CPU leg recorded the tower knob's decline")
            tt |> equal(w1.windows - w0.windows, 0l, "the CPU leg's decoder never served a window")
            tt |> equal(w1.steps - w0.steps, 0l, "the CPU leg's decoder never served a decode step")
            tt |> success(key_exists(gpu_wdec_declines(), "knob"), "the CPU leg recorded the decoder knob's decline")
            tt |> equal(w1.handoffs, w0.handoffs, "and the CPU leg took none")
        }
    }
    k.q8.legs++
    var inscope win <- jfk_window(m.whisper, samples)
    var inscope es <- make_encoder_state(m.whisper.enc)
    twin_named(k, m.whisper.enc)
    twin_rows_leg(k.rows, TwinLeg.cpu, m.whisper.enc, es) $(e; var s; var out) => whisper_rows_encode(e, s, win, out)
}

// The q8 lane's device legs, on the `q8@dev` form: jfk transcribed with both drivers on, the counters proving each
// served, the encoder rows' device leg, and the decoder's alignment decline.
[cold_path]
def private twin_q8_dev_leg(tt : T?; md : AsrModel; samples : array<float>; var k : WhisperTwin) {
    let name = k.name
    let home = serving_tower_dev(TowerFamily.whisper)
    k.dev_backend = md.whisper.enc.dev_backend
    {
        var inscope dsd <- make_decoder_state(md.whisper)
        tt |> success(empty(dsd.kcache) && empty(dsd.kx) && empty(dsd.h) && empty(dsd.xqi), "the {home} image's decoder state carries no CPU chain buffers")
    }
    with_dasllama_jobque_() {
        with_gpu_knobs() {
            set_serving_gpu_tower(true, TowerFamily.whisper)
            set_gpu_wdec(true)
            let d1 = gpu_tower_counters(TowerFamily.whisper)
            let w1 = gpu_wdec_stats()
            k.q8.dev_segs <- transcribe_segs(md, samples)
            let d2 = gpu_tower_counters(TowerFamily.whisper)
            let w2 = gpu_wdec_stats()
            if (d2.encodes == d1.encodes || w2.windows == w1.windows) {
                to_log(LOG_INFO, "{name} {home} twin: a driver declined - tower {gpu_tower_declines(TowerFamily.whisper)}, decoder {gpu_wdec_declines()}\n")
            }
            let n_enc = d2.encodes - d1.encodes
            tt |> success(n_enc >= 1l, "the device served the chunk encodes ({n_enc})")
            tt |> equal(d2.blocks - d1.blocks, n_enc * md.whisper.enc.n_layer, "every block of every served encode ran on the driver")
            tt |> equal(d2.rows - d1.rows, n_enc * md.whisper.enc.n_ctx, "the rows delta is the chunks' positions")
            front_served(tt, "{name} jfk conv stem", d2.convs - d1.convs, n_enc, d2.declines - d1.declines)
            tt |> success(w2.windows - w1.windows >= 1l, "the device served the window's cross-KV ({w2.windows - w1.windows})")
            tt |> success(w2.steps - w1.steps >= 20l, "the decode batches served on the device ({w2.steps - w1.steps})")
            tt |> success(w2.tokens - w1.tokens >= w2.steps - w1.steps, "the token rows cover the steps")
            tt |> equal(w2.declines, w1.declines, "the device leg's decoder recorded no decline")
            tt |> equal(w2.handoffs - w1.handoffs, w2.windows - w1.windows, "every served window took the encoder rows off the tower's plane, device to device")
            k.q8.legs++
            var inscope win <- jfk_window(md.whisper, samples)
            var inscope esd <- make_encoder_state(md.whisper.enc)
            twin_named(k, md.whisper.enc)
            twin_rows_leg(k.rows, TwinLeg.dev, md.whisper.enc, esd) $(e; var s; var out) => whisper_rows_encode(e, s, win, out)
            wdec_align_declines(tt, name, md.whisper)
        }
    }
}

// The exact encoder's rows for the twin's bar and the f32 decoder's decline, on the `f32` form.
[cold_path]
def private twin_f32_leg(tt : T?; mx : AsrModel; samples : array<float>; var k : WhisperTwin) {
    twin_named(k, mx.whisper.enc)
    let tag = k.rows.spec.tag
    tt |> success(!mx.whisper.enc.q8, "{tag}: the exact encoder minted for the compare")
    var inscope win <- jfk_window(mx.whisper, samples)
    var inscope esx <- make_encoder_state(mx.whisper.enc)
    twin_rows_leg(k.rows, TwinLeg.exact, mx.whisper.enc, esx) $(e; var s; var out) => whisper_rows_encode(e, s, win, out)
    with_gpu_knobs() {
        set_serving_gpu_tower(true, TowerFamily.whisper)
        set_gpu_wdec(true)
        wdec_f32_declines(tt, k.name, mx.whisper, samples)
    }
}

// The input poison of the encoder rows' bar, on the `q8_z<half>@dev` form: the home-flavored q8 encoder with every
// block's GEMM planes from the middle one on zeroed, through the device chain.
[cold_path]
def private twin_q8_poison_leg(tt : T?; wz : AsrModel; samples : array<float>; var k : WhisperTwin) {
    twin_named(k, wz.whisper.enc)
    let tag = k.rows.spec.tag
    tt |> success(wz.whisper.enc.q8, "{tag}: the poisoned encoder is on the q8 lane")
    tt |> equal(k.half, wz.whisper.enc.n_layer / 2l, "{tag}: the poison form zeroes from the middle block on")
    var inscope win <- jfk_window(wz.whisper, samples)
    var inscope esz <- make_encoder_state(wz.whisper.enc)
    twin_rows_poison_leg(tt, k.rows, k.half, wz.whisper.enc, esz) $(e; var s; var out) => whisper_rows_encode(e, s, win, out)
}

// The exact encoder's lane on the CPU, on the `mixed` form: its rows and jfk's transcription with every driver off.
[cold_path]
def private twin_mixed_cpu_leg(mx : AsrModel; samples : array<float>; var k : WhisperTwin) {
    twin_named(k, mx.whisper.enc)
    var inscope win <- jfk_window(mx.whisper, samples)
    var inscope es <- make_encoder_state(mx.whisper.enc)
    exact_lane_cpu_leg(k.exact, mx.whisper.enc, es) $(e; var s; var out) => whisper_rows_encode(e, s, win, out)
    with_dasllama_jobque_() {
        with_gpu_knobs() {
            set_serving_gpu_tower(false, TowerFamily.whisper)
            set_gpu_wdec(false)
            k.mixed.cpu_segs <- transcribe_segs(mx, samples)
        }
    }
    k.mixed.legs++
}

// The exact encoder's lane on the device, on the `mixed@dev` form: the home's f16 image, its rows, and jfk transcribed
// with both drivers on.
[cold_path]
def private twin_mixed_dev_leg(tt : T?; mxd : AsrModel; samples : array<float>; var k : WhisperTwin) {
    twin_named(k, mxd.whisper.enc)
    let tag = "{k.name} jfk exact encoder"
    let home = serving_tower_dev(TowerFamily.whisper)
    tt |> success(!mxd.whisper.enc.q8 && mxd.whisper.enc.gemm_f16 && mxd.whisper.dq8 && mxd.whisper.enc.dev_backend == home,
        "{tag}: the f16 encoder's {home}-flavored image, its decoder q8")
    tt |> equal(asr_device_lane(mxd), "{home}", "{tag}: the facade names the home the exact lane serves on")
    var inscope win <- jfk_window(mxd.whisper, samples)
    var inscope esd <- make_encoder_state(mxd.whisper.enc)
    exact_lane_dev_leg(k.exact, mxd.whisper.enc, esd) $(e; var s; var out) => whisper_rows_encode(e, s, win, out)
    with_dasllama_jobque_() {
        with_gpu_knobs() {
            set_serving_gpu_tower(true, TowerFamily.whisper)
            set_gpu_wdec(true)
            let d0 = gpu_tower_counters(TowerFamily.whisper)
            let w0 = gpu_wdec_stats()
            k.mixed.dev_segs <- transcribe_segs(mxd, samples)
            let d1 = gpu_tower_counters(TowerFamily.whisper)
            let w1 = gpu_wdec_stats()
            tt |> success(d1.encodes - d0.encodes >= 1l, "{tag}: the device served the chunk encodes ({d1.encodes - d0.encodes})")
            tt |> equal(d1.blocks - d0.blocks, (d1.encodes - d0.encodes) * mxd.whisper.enc.n_layer, "{tag}: every block of every served encode ran on the driver")
            tt |> success(w1.windows - w0.windows >= 1l, "{tag}: the decoder served the window's cross-KV off the exact encoder's rows")
            tt |> equal(w1.handoffs - w0.handoffs, w1.windows - w0.windows, "{tag}: every served window took the encoder rows device to device")
        }
    }
    k.mixed.legs++
}

// The exact lane's input poison, on the `mixed_z<half>@dev` form.
[cold_path]
def private twin_mixed_poison_leg(tt : T?; wz : AsrModel; samples : array<float>; var k : WhisperTwin) {
    twin_named(k, wz.whisper.enc)
    tt |> equal(k.half, wz.whisper.enc.n_layer / 2l, "{k.exact.tag}: the poison form zeroes from the middle block on")
    var inscope win <- jfk_window(wz.whisper, samples)
    var inscope esz <- make_encoder_state(wz.whisper.enc)
    exact_lane_poison_leg(tt, k.exact, wz.whisper.enc, esz) $(e; var s; var out) => whisper_rows_encode(e, s, win, out)
}

// A form's legs of a whisper bin's discrete twin, each on the form's one home.
[cold_path]
def private whisper_twin_leg(tt : T?; m : AsrModel; var k : WhisperTwin) {
    let samples <- asr_samples(tt, "jfk.wav")
    return if (empty(samples))
    let f = whisper_form_of(k.form)
    if (f.lane == "f32") {
        twin_f32_leg(tt, m, samples, k)
    } elif (f.lane == "q8" && f.zero_block >= 0l) {
        twin_q8_poison_leg(tt, m, samples, k)
    } elif (f.lane == "q8" && f.dev) {
        twin_q8_dev_leg(tt, m, samples, k)
    } elif (f.lane == "q8") {
        twin_q8_cpu_leg(tt, m, samples, k)
    } elif (f.zero_block >= 0l) {
        twin_mixed_poison_leg(tt, m, samples, k)
    } elif (f.dev) {
        twin_mixed_dev_leg(tt, m, samples, k)
    } else {
        twin_mixed_cpu_leg(m, samples, k)
    }
}

//! one lane's two transcriptions of jfk, logged as `cpu_name` and `dev_name`
def private lane_texts(k : LanePair; cpu_name, dev_name : string) : tuple<cpu : string; dev : string> {
    let cpu_text = segs_text(k.cpu_segs)
    let dev_text = segs_text(k.dev_segs)
    let f = segs_token_flips(k.cpu_segs, k.dev_segs)
    to_log(LOG_INFO, "{cpu_name} jfk: '{cpu_text}'\n")
    to_log(LOG_INFO, "{dev_name} jfk: '{dev_text}' ({f.flips}/{f.total} token flips against the CPU lane)\n")
    return (cpu = cpu_text, dev = dev_text)
}

// The whisper lane whole on each side (`#tower-dev-block`), after the bin's block: the CPU image on the CPU, the home's
// flavored image on the device, the texts equal; the encoder rows within the twin's bar with the poison past it; the exact
// encoder's lane within its bar, its poison past it, its texts equal.
[cold_path, arch(at = "../ARCHITECTURE_GPU_TOWER_VULKAN.md#vk-tower-encode-chains")]
def private test_whisper_discrete_twin(tt : T?; k : WhisperTwin) {
    let name = k.name
    let home = serving_tower_dev(TowerFamily.whisper)
    tt |> success(k.cpu_q8 && k.dev_backend == home, "the q8 lane's CPU image and its {home}-flavored image")
    tt |> equal(k.q8.legs, 2, "{name}: the q8 lane kept its CPU and its device transcription")
    if (k.q8.legs == 2) {
        let q8 = lane_texts(k.q8, "{name} cpu q8", "{name} {home}")
        tt |> equal(length(k.q8.cpu_segs), length(k.q8.dev_segs), "{name} jfk: segment count")
        tt |> success(q8.cpu == q8.dev, "{name} jfk cpu q8 / {home}: the same text (two q8 chains, the device's summation order its own: a near-tie token may flip, the words do not)")
    }
    let tag = k.rows.spec.tag
    twin_rows_held(tt, k.rows) $(legs) {
        tt |> equal(legs.dev.encodes - legs.cpu.encodes, 1l, "{tag}: the device encode served")
    }
    let kept = !empty(k.exact.cpu) && !empty(k.exact.dev) && !empty(k.exact.poisoned)
    tt |> success(kept, "{k.exact.tag}: the exact lane kept its clean legs and its poison leg")
    if (kept) {
        exact_lane_held(tt, k.exact)
        exact_lane_poison_held(tt, k.exact)
    }
    tt |> equal(k.mixed.legs, 2, "{name}: the exact encoder's lane kept its CPU and its device transcription")
    if (k.mixed.legs == 2) {
        let mixed = lane_texts(k.mixed, "{name} cpu exact-encoder", "{name} {home} exact-encoder")
        tt |> success(mixed.cpu == mixed.dev, "{name} jfk exact encoder: the CPU exact lane and the device's exact lane read the same text")
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

//! the f32 decoder `mf` never serves on whisper's discrete decoder: it records the `quant_mode` decline and the CPU transcribes jfk
[cold_path]
def private wdec_f32_declines(tt : T?; name : string; mf : WhisperModel; samples : array<float>) {
    let tag = "{name} jfk f32 decoder"
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

//! what the tiny bin's forms keep for the compares after its block: the form whose cells run, the q8 mint's jfk text (the
//! q8_0 bin's reference), the q8 gate's two transcriptions, the post-norm seat's rows
struct private TinyKept {
    form : string
    f16_text : string
    gate_q8 : array<TranscribeSegment>
    gate_f32 : array<TranscribeSegment>
    ln : LnPostKept
}

//! what the post-norm seat's two forms keep: the CPU chain's rows before and after the post-norm with the norm's weight
//! and bias, and the device's normed rows beside its own block rows
struct private LnPostKept {
    d_model : int64
    n_ctx : int64
    eps : float
    lnp : array<float>   //! the post-norm's weight, then its bias
    raw : array<float>
    cpu : array<float>
    normed : array<float>
    blocks : array<float>
}

// The f16 bin's side of the q8_0 file's compare, on tiny's `q8` form: jfk's text off the q8 mint.
[cold_path]
def private q8_bin_text_leg(tt : T?; m16 : AsrModel; var k : TinyKept) {
    return if (!jit_only(tt))
    let samples <- asr_samples(tt, "jfk.wav")
    return if (empty(samples))
    with_dasllama_jobque_() {
        var inscope s16 <- create_session(m16)
        k.f16_text = transcribe(m16, s16, samples)
    }
}

// A whisper.cpp q8_0 bin loads: its Q8_0 tensors read as their blocks' scale times the quants, and it transcribes what the
// f16 bin of the same model does - on the q8_0 bin's `q8` form, against the text tiny's `q8` form kept.
[cold_path]
def private test_whisper_q8_0_file(tt : T?; m8 : AsrModel; k : TinyKept) {
    return if (!jit_only(tt))
    let samples <- asr_samples(tt, "jfk.wav")
    return if (empty(samples))
    if (empty(k.f16_text)) {
        tt |> skip("{TINY}'s q8 form kept no text - its block did not run")
        return
    }
    let from_f16 = k.f16_text
    var from_q8 = ""
    with_dasllama_jobque_() {
        var inscope s8 <- create_session(m8)
        from_q8 = transcribe(m8, s8, samples)
    }
    to_log(LOG_INFO, "whisper tiny f16 bin: '{from_f16}'\n")
    to_log(LOG_INFO, "whisper tiny q8_0 bin: '{from_q8}'\n")
    tt |> success(!empty(from_q8), "the q8_0 bin transcribes")
    tt |> equal(from_q8, from_f16, "and reads what the f16 bin reads")
}

// The Metal tower's blocks-with-post-norm seat, its CPU leg on the `f32` form: the seat declines with the tower off, and
// the CPU chain's rows before and after the post-norm are kept with the norm's weight and bias.
[cold_path, unused_argument(tt, m, k)]
def private ln_post_cpu_leg(tt : T?; m : AsrModel; var k : LnPostKept) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        return if (!jit_only(tt))
        let samples <- asr_samples(tt, "jfk.wav")
        return if (empty(samples))
        with_dasllama_jobque_() {
            with_gpu_knobs() {
                var inscope win <- jfk_window(m.whisper, samples)
                var inscope es <- make_encoder_state(m.whisper.enc)
                set_metal_tower(false)
                tt |> success(!audio_encode_blocks_ln_post(m.whisper.enc, es, win), "tower off: the seat declines and the CPU owes the post-norm")
                k.raw := es.x   // the blocks' rows before the post-norm: the control's reference
                whisper_encode(m.whisper.enc, es, win)
                k.cpu := es.xb
                k.d_model = m.whisper.enc.d_model
                k.n_ctx = m.whisper.enc.n_ctx
                k.eps = m.whisper.enc.eps
                delete k.lnp
                k.lnp <- [for (i in range64(2l * k.d_model)); m.whisper.enc.fblob[(i < k.d_model ? m.whisper.enc.lnp_w_off : m.whisper.enc.lnp_b_off - k.d_model) + i]]
            }
        }
    }
}

// The seat's device leg, on the `mixed@dev` form - the exact encoder's Metal-flavored image (`#tower-dev-block`): one
// device encode serves the blocks and the post-norm, then the blocks seat alone gives the device's own block rows.
[cold_path, unused_argument(tt, md, k)]
def private ln_post_dev_leg(tt : T?; md : AsrModel; var k : LnPostKept) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        return if (!jit_only(tt))
        let samples <- asr_samples(tt, "jfk.wav")
        return if (empty(samples))
        with_dasllama_jobque_() {
            with_gpu_knobs() {
                var inscope win <- jfk_window(md.whisper, samples)
                set_metal_tower(true)
                var inscope esd <- make_encoder_state(md.whisper.enc)
                let s0 = metal_tower_stats()
                tt |> success(audio_encode_blocks_ln_post(md.whisper.enc, esd, win), "tower on: the seat served the blocks and the post-norm")
                tt |> equal(metal_tower_stats().encodes - s0.encodes, 1l, "one device encode")
                k.normed := esd.xb
                audio_encode_blocks(md.whisper.enc, esd, win)
                k.blocks := esd.x
            }
        }
        metal_tower_shutdown()
    }
}

// The Metal tower fills the whisper blocks-with-post-norm seat: the post-norm runs behind the blocks on the device and the
// normed rows land in `xb`, where the CPU chain's post-norm would put them - after the block, on the rows the two forms kept.
[cold_path]
def private test_whisper_metal_ln_post(tt : T?; k : LnPostKept) {
    return if (!jit_only(tt))
    if (!asr_clip_present("jfk.wav")) {
        tt |> skip("jfk.wav not present")
        return
    }
    let kept = !empty(k.cpu) && !empty(k.raw) && !empty(k.normed) && !empty(k.blocks)
    tt |> success(kept, "the post-norm seat kept its CPU and its device rows")
    return if (!kept)
    // a NaN on either side reads infinite in the difference, and in the scale where it is the reference's: the
    // ratio is then infinite or NaN, outside the bar
    let off = logits_maxdiff(k.normed, k.cpu) / logits_maxabs(k.cpu)
    let unnormed = logits_maxdiff(k.normed, k.raw) / logits_maxabs(k.raw)
    // the post-norm is the unit: the blocks seat alone gives the device's own block rows, the CPU norms THOSE, and the
    // device's normed rows must be that image - the block chain's device-vs-CPU distance stays out of the bar
    var inscope want : array<float>
    want |> resize(length(k.blocks))
    layernorm_batch(want, k.blocks, k.lnp, 0l, k.d_model, k.d_model, k.n_ctx, k.eps)
    let ln_off = logits_maxdiff(k.normed, want) / logits_maxabs(want)
    to_log(LOG_INFO, "whisper tiny ln_post: device norm vs the CPU norm of the device's block rows {ln_off}; vs the CPU chain {off}; vs the un-normed rows {unnormed}\n")
    tt |> success(ln_off < 1e-4, "the device's post-norm is the CPU post-norm of the same rows ({ln_off})")
    tt |> success(unnormed > 0.1, "the control: against the rows before the post-norm the compare misses ({unnormed})")
}

//! a lifetime cell's seat: tiny on jfk, both drivers on, one session reused across transcriptions the way a serving worker reuses one
def private wdec_lifetime_cell(tt : T?; body : block<(m : AsrModel; var s : AsrSession; samples : array<float>) : void>) {
    static_if (typeinfo builtin_module_exists(vulkan) || typeinfo builtin_module_exists(cuda)) {
        own_load("drops the model before its last step: the device model is swept between two transcriptions of one session")
        let model_path = path_join(whisper_dir(), "ggml-tiny.bin")
        jfk_twin_cell(tt, model_path, true, TowerFamily.whisper) $(samples) {
            set_serving_gpu_tower(true, TowerFamily.whisper)
            set_gpu_wdec(true)
            var inscope stg <- whisper_stage(model_path, whisper_stage_key("q8@dev"))
            var inscope m <- whisper_form(model_path, "q8@dev", stg)
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

// A leg of the q8 gate, on the facade's `image_q8` and `image_f32` forms: jfk transcribed with every GPU tower and
// decoder off.
[cold_path]
def private q8_gate_leg(t : T?; m : AsrModel; var k : TinyKept) {
    return if (!jit_only(t))
    let samples <- asr_samples(t, "jfk.wav")
    return if (empty(samples))
    with_gpu_knobs() {
        set_every_gpu_tower(false)
        set_every_gpu_wdec(false)
        with_dasllama_jobque_() {
            if (k.form == "image_q8") {
                t |> equal(m.whisper.enc.q8, whisper_tower_serves_q8(), "the default load takes the lane whisper_tower_serves_q8 picks unpinned")
                k.gate_q8 <- transcribe_segs(m, samples)
            } else {
                t |> success(!m.whisper.enc.q8, "knob forces fp32")
                k.gate_f32 <- transcribe_segs(m, samples)
            }
        }
    }
}

// Tiny's default q8 load against the knob-pinned fp32 load on jfk, after the block: token-identical.
[cold_path]
def private test_whisper_q8_gate(t : T?; k : TinyKept) {
    return if (!jit_only(t))
    if (!asr_clip_present("jfk.wav")) {
        t |> skip("jfk.wav not present")
        return
    }
    let kept = !empty(k.gate_q8) && !empty(k.gate_f32)
    t |> success(kept, "the q8 gate kept both lanes' segments")
    return if (!kept)
    to_log(LOG_INFO, "whisper-tiny q8 jfk eyeball: '{k.gate_q8[0].text}'\n")
    to_log(LOG_INFO, "whisper-tiny f32 jfk eyeball: '{k.gate_f32[0].text}'\n")
    assert_segs_token_identical(t, k.gate_q8, k.gate_f32, "q8/fp32")
}


[test(description = "`set_asr_tower_fp32` loads whisper tiny with the exact encoder (the f16 lane on a GPU home's flavor) beside the q8 decoder, stamps the exec format, and reads jfk token-identical to the q8 default with Metal crowns pinned off")]
def test_whisper_tower_fp32_knob(t : T?) {
    t |> run("tiny mixed f32-enc/q8-dec (set_asr_tower_fp32, the GPU serving mode) stays token-identical on jfk") @(t : T?) {
        own_load("shuts down the model: the Metal drivers are shut down and rebuilt under a crown pin held across both lanes' loads and transcriptions")
        let model_path = path_join(whisper_dir(), "ggml-tiny.bin")
        return if (!jit_model_available(t, model_path))
        let samples <- asr_samples(t, "jfk.wav")
        return if (empty(samples))
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
            own_load("shuts down the model: the Metal drivers are shut down and rebuilt under the crown pin held across both lanes' loads and transcriptions")
            let model_path = path_join(whisper_dir(), "ggml-tiny.bin")
            return if (!jit_model_available(t, model_path))
            let samples <- asr_samples(t, "jfk.wav")
            return if (empty(samples))
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
// On the mmproj's `image_asr` form: the ASR facade's two-file load of the mmproj with its Q8 thinker.
[cold_path]
def private test_qwen3omni_audio_oracle(t : T?; m : AsrModel) {
    let prior_kv = get_asr_kv()
    set_asr_kv(KVDtype.f32)   // the frozen mtmd ids are the f32 parity rail
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
    set_asr_kv(prior_kv)
}

//! the Qwen3-Omni-30B mmproj's form loader: `image_asr`, the ASR facade's two-file load of the mmproj with its Q8 thinker
//! under the models directory, which sniffs the pair's route
def private q3omni_form(path, form : string) : AsrModel {
    if (form != "image_asr") {
        panic("the Qwen3-Omni tests: '{form}' names no form - the pair loads through the facade alone")
    }
    let planes = host_planes_pin()
    set_host_planes(true)   //! its cell rides the f32 K/V CPU rail: a load onto a discrete home keeps no plane on the host
    var inscope m = AsrModel()
    with_put_back($ {
        restore_host_planes(planes)
    }) {
        delete m
        m <- load_asr_model(path_join(models_dir(), "Qwen3-Omni-30B-A3B-Instruct-Q8_0.gguf"), path)
    }
    return <- m
}

// The Qwen3-Omni-30B pair: the mmproj's block, whose one form is the facade's load with the thinker - the full suite's alone.
[test(description = "Qwen3-Omni-30B audio-in, with the shared qwen3a AuT tower feeding the MoE thinker, transcribes jfk and two librispeech clips to llama-mtmd-cli's full greedy id sequences, trailing im_end included")]
def test_qwen3omni_pair(t : T?) {
    let cell = "Qwen3-Omni-30B greedy audio transcription is token-for-token with llama-mtmd-cli"
    return if (!family_on(t, "qwen3"))
    if (!parity_full_mode()) {   // ~34 GB (Q8 thinker + bf16 mmproj) - large-tier
        skip_cell(t, "{cell} - Qwen3-Omni-30B is large-tier, set DASLLAMA_PARITY_FULL=1 to run")
        return
    }
    return if (!asr_decoder_available(t, "Qwen3-Omni-30B-A3B-Instruct-Q8_0.gguf"))
    with_model_once(t, "audio", "mmproj-Qwen3-Omni-30B-A3B-Instruct-bf16.gguf", ["image_asr"], @@q3omni_form) $(var m; form) {
        t |> success(m.kind == AsrKind.qwen3omni, "mmproj-Qwen3-Omni-30B-A3B-Instruct-bf16.gguf: the {form} form is the facade's Qwen3-Omni pair")
        tower_cell(t, cell, m, @@test_qwen3omni_audio_oracle)
    }
}

// Whisper tiny, one block, and the q8_0 bin of the same model after it.
[test(description = "ggml-tiny.bin loads once a form. The facade's load sniffs it as whisper and its caps report exactly the 99 languages ending before yue, translate and segment timestamps, no streaming or prompt, an auto session deferring language detection. The exact load yields whisper tiny's hyperparameters (51865 vocab, 384 wide, 4 layers, 99 languages), its special token ids, the tanh-LUT GELU flag and known vocab strings, and greedy on jfk.wav emits whisper-cli's 26 token ids, one 0-10.5 s segment and its exact text, two-chunk feed+flush and an auto session that detects English landing on the same text, avg_logprob in (-1, 0). With GPU towers off the default q8 load and the knob-pinned fp32 load transcribe jfk token-identical, same segment count and text. The Metal tower's blocks-with-post-norm seat serves one device encode whose normed rows are the CPU post-norm of its own block rows within 1e-4 relative, while the un-normed rows miss by over 0.1. On the discrete GPU home the q8 lane reads jfk with the CPU q8 lane's text while counters prove each side ran where claimed, encoder rows hold the twin bar with a poison control, the f32 decoder and a bad alignment decline, and the exact encoder's lane is served within its bar. A whisper.cpp ggml-tiny-q8_0.bin loads with its Q8_0 tensors read as scale times quants and transcribes jfk to exactly the text the f16 tiny bin produces")]
def test_whisper_tiny(t : T?) {
    var inscope kept = TinyKept()
    var inscope twin = WhisperTwin(name = TINY, half = 2l)
    let twin_why = jfk_twin_why(TowerFamily.whisper)
    let twin_on = empty(twin_why)
    let metal_on = typeinfo builtin_module_exists(das_metal)
    var inscope forms <- ["image", "image_f32", "image_q8", "q8"]
    add_served_forms(t, forms, TINY, ["f32", "mixed@dev"], either_why(metal_build_why(), twin_why))
    add_served_forms(t, forms, TINY, ["mixed_z2@dev", "q8@dev", "q8_z2@dev", "mixed"], twin_why)
    let ran = with_model_once_at(t, "audio", "ggml-tiny.bin", forms, whisper_dir(), @@whisper_stage_key, @@whisper_stage, @@whisper_form) $(var m; form) {
        whisper_form_is(t, TINY, form, m)
        kept.form = form
        twin.form = form
        if (form == "image") {
            tower_cell(t, "load_asr_model sniffs the bin + caps report whisper honestly", m, @@test_asr_surface)
        } elif (form == "image_f32") {
            tower_cell(t, "tiny bin: hparams, special ids, vocab (needs ggml-tiny.bin)", m.whisper, @@test_whisper_loader)
            tower_cell(t, "tiny greedy on jfk.wav is token-for-token with whisper-cli", m.whisper, @@test_whisper_transcribe_oracle)
            tower_cell(t, "tiny on jfk under the facade's fp32 pin, the q8 gate's exact leg", m, kept, @@q8_gate_leg)
        } elif (form == "image_q8") {
            tower_cell(t, "tiny on jfk on the facade's default load, the q8 gate's q8 leg", m, kept, @@q8_gate_leg)
        } else {
            if (form == "q8") {
                tower_cell(t, "{TINY}: jfk off the q8 mint, the q8_0 bin's reference text", m, kept, @@q8_bin_text_leg)
            }
            if (metal_on && form == "f32") {
                tower_cell(t, "{TINY}: the post-norm seat's CPU leg", m, kept.ln, @@ln_post_cpu_leg)
            } elif (metal_on && form == "mixed@dev") {
                tower_cell(t, "{TINY}: the post-norm seat's device leg", m, kept.ln, @@ln_post_dev_leg)
            }
            if (twin_on) {
                tower_cell(t, "{TINY}: the discrete twin, the {form} form's legs", m, twin, @@whisper_twin_leg)
            }
        }
    }
    if (ran) {
        kept_cell(t, "tiny q8 tower (the load_asr_model default) stays token-identical to fp32 on jfk", kept, @@test_whisper_q8_gate)
        if (metal_on) {
            kept_cell(t, "{TINY}: the encoder's post-norm on the Metal tower lands the CPU chain's normed rows", kept.ln, @@test_whisper_metal_ln_post)
        } else {
            skip_cell(t, "{TINY}: the encoder's post-norm on the Metal tower - dasMetal not present")
        }
    }
    if (ran && twin_on) {
        kept_cell(t, "{TINY}: the discrete home's q8 lane vs the CPU q8 lane on jfk", twin, @@test_whisper_discrete_twin)
        gpu_tower_shutdown(TowerFamily.whisper)
    }
    with_model_once_at(t, "audio", "ggml-tiny-q8_0.bin", ["q8"], whisper_dir(), @@whisper_stage_key, @@whisper_stage, @@whisper_form) $(var m8; form8) {
        whisper_form_is(t, TINY_Q8_0, form8, m8)
        tower_cell(t, "{TINY_Q8_0}: a q8_0 bin loads and reads jfk as the f16 bin does", m8, kept, @@test_whisper_q8_0_file)
    }
}

// Whisper large-v3-turbo, one block: the discrete twin's forms alone.
[test(description = "ggml-large-v3-turbo.bin loads once a form: on the discrete GPU home turbo's q8 lane reads jfk with the CPU q8 lane's text while counters prove each side ran where claimed; encoder rows hold the twin bar with a poison control, the f32 decoder and a bad alignment decline, and the exact encoder's lane is served on the device within its bar with a poison control, both exact lanes reading the same text")]
def test_whisper_turbo(t : T?) {
    var inscope twin = WhisperTwin(name = TURBO, half = 16l)
    var inscope forms : array<string>
    add_served_forms(t, forms, TURBO, ["q8", "f32", "mixed", "q8@dev", "q8_z16@dev", "mixed@dev", "mixed_z16@dev"], jfk_twin_why(TowerFamily.whisper))
    let ran = with_model_once_at(t, "audio", "ggml-large-v3-turbo.bin", forms, whisper_dir(), @@whisper_stage_key, @@whisper_stage, @@whisper_form) $(var m; form) {
        whisper_form_is(t, TURBO, form, m)
        twin.form = form
        tower_cell(t, "{TURBO}: the discrete twin, the {form} form's legs", m, twin, @@whisper_twin_leg)
    }
    if (ran && !empty(forms)) {
        kept_cell(t, "{TURBO}: the discrete home's q8 lane vs the CPU q8 lane on jfk", twin, @@test_whisper_discrete_twin)
        gpu_tower_shutdown(TowerFamily.whisper)
    }
}
