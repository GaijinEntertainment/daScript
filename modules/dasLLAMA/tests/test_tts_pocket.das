options gen2
options persistent_heap
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama_spm
require dasllama/dasllama_gguf
require dasllama/dasllama_pocket
require dasllama/dasllama_tts
require dasllama/dasllama_audio_io
require dasllama/dasllama_tts_types
require dasllama/dasllama_tts_blocks
require dasllama/dasllama_math   // setup_dasllama_jobque_
require daslib/jobque_boost
require daslib/defer   // defer() { reset_pocket_q8() } - a lane pin never outlives its cell
require daslib/fio
require daslib/json_boost
require strings
require math
require _tts_oracle
require _tts_parity
require ?das_metal dasllama/dasllama_metal_tower
require _tts_corpus   // the corpus sentences the oracle prepared: the prepare-parity cell's inputs
require _model_tier

// The Pocket TTS family. Model-gated (`pocket-tts-en.gguf` under models_dir(), minted by
// harness/convert_pocket.py; the oracle dumps under `tts_oracle/pocket_english_2026-04/` by
// harness/pocket_oracle.py): the unigram tokenizer id-for-id against the reference package on
// the 200-sentence corpus and on the byte-fallback probes (`_tts_fixtures/pocket_tokens.json`,
// written by the oracle script's --tokens-fixture), the decode round trip, then the parity rail.
// Prepared-prompt transfer needs pocket-tts-en-kq.gguf and pocket-tts-en-stuart-kq.gguf;
// it uses the checked-in jfk_ask_not.wav reference and skips if either model is absent. The
// quiet-floor cell needs pocket-tts-en-kq.gguf alone. The Metal cells skip on a build without the
// das_metal module and need a Metal device that serves the tower (they red where a present device
// declines); the served-lane cells skip unless pocket-tts-en-q8.gguf or pocket-tts-en-kq.gguf is
// stocked, the served frames cell also unless the oracle carries two voices' stage cases; the
// cells that hold the CPU chain pin the tower off for their run.

let GPU_CODEC_BAR = 1e-5   //! the codec seat against the CPU chain on the f32 lane: reads 9e-7 on the exact stamps (M5 Max, -jit)
let GPU_FRAME_BAR = 2e-5   //! the frames seat against the CPU chain on the f32 lane, teacher-forced: reads 2e-6 on the latents (M5 Max, -jit)
let GPU_CODEC_SERVED_BAR = 5e-2   //! the codec seat on the served planes: reads 1.3e-2 on the q8 file, 7.7e-3 on the kq file on the M5 Max (the CPU quantizes its activations, the tower feeds them f32)
let GPU_FRAME_SERVED_BAR = 1e-1   //! the frames seat on the served planes, teacher-forced: reads 1.9e-2 / 3.7e-2 on the q8 file, 3.8e-2 / 7.1e-2 on the kq file (alba / caro_davy) on the M5 Max
let GPU_FRAME_FREE_BAR = 1e-3   //! the frames seat free-running on the f32 lane, its own noise from the same seed, every frame fed its own output: reads 1.5e-4 and 1.1e-4 on the two stage cases on the M5 Max

//! The tower knob off for a cell that holds the CPU chain; the knob it found, for the restore.
def private tower_pin_off() : bool {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let found = metal_tower_knob()
        set_metal_tower(false)
        return found
    } else {
        return false
    }
}

[unused_argument(found)]   //! off-Apple there is no knob
def private tower_restore(found : bool) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        set_metal_tower(found)
    }
}

def private gguf_path() : string {
    return path_join(models_dir(), "pocket-tts-en.gguf")
}

def private tokens_fixture_path() : string {
    return "{get_das_root()}/modules/dasLLAMA/tests/_tts_fixtures/pocket_tokens.json"
}

struct private TokRow {
    id : string
    prepared : string
    frames_after_eos : int
    ids : array<int64>
}

struct private TokProbe {
    text : string
    ids : array<int64>
}

struct private ChunkCase {
    text : string
    chunks : array<string>
}

struct private TokFixture {
    language : string
    revision : string
    rows : array<TokRow>
    chunks : array<ChunkCase>
    probes : array<TokProbe>
}

def private load_tokens_fixture(path : string = tokens_fixture_path()) : TokFixture {
    let text = fread(path)
    if (empty(text)) {
        panic("pocket tokens fixture missing: {path}")
    }
    var fx = TokFixture()
    if (!sscan_json(text, fx)) {
        panic("pocket tokens fixture does not parse: {path}")
    }
    return <- fx
}

def private ids_text(a : array<int64>) : string {
    return build_string() $(var wr) {
        for (i, v in count(), a) {
            write(wr, i == 0 ? "{v}" : " {v}")
        }
    }
}

[test]
def test_pocket_tokenizer(t : T?) {
    let path = gguf_path()
    if (!model_available(t, path)) {
        return
    }
    var inscope tk <- load_tokenizer_gguf(path)
    var unk_id = 0l
    var eos_id = 2l
    var byte_fallback = true
    var inscope reserved : array<int64>
    let f = fopen(path, "rb")
    fmap(f) $(var bytes : array<uint8>#) {
        var inscope m <- parse_gguf_meta(bytes)
        t |> equal(gguf_str(m, bytes, "tokenizer.ggml.model"), "t5", "the GGUF names a unigram SentencePiece model")
        unk_id = gguf_int(m, bytes, "tokenizer.ggml.unknown_token_id")
        eos_id = gguf_int(m, bytes, "tokenizer.ggml.eos_token_id")
        byte_fallback = gguf_int(m, bytes, "tokenizer.ggml.byte_fallback") != 0l
        reserved |> push(unk_id)
        reserved |> push(gguf_int(m, bytes, "tokenizer.ggml.bos_token_id"))
        reserved |> push(eos_id)
        reserved |> push(gguf_int(m, bytes, "tokenizer.ggml.padding_token_id"))
    }
    fclose(f)
    t |> equal(length(tk.vocab), 4000, "the 4000-piece vocabulary")
    t |> success(tk.add_space_prefix, "the dummy-space prefix is on")
    t |> equal(tk.max_token_length, 7, "the longest piece in bytes, its three-byte space marker read as one space")
    var inscope fx <- load_tokens_fixture()
    t |> equal(length(fx.rows), 200, "the 200-sentence corpus, one row each")
    // every corpus sentence, prepared as the package prepares it, tokenizes id for id
    var bad = 0
    for (r in fx.rows) {
        var inscope ours <- unigram_encode(tk, r.prepared, unk_id, byte_fallback, reserved)
        if (!ids_equal(ours, r.ids)) {
            bad++
            if (bad <= 3) {
                to_log(LOG_ERROR, "pocket tokens {r.id}: ours [{ids_text(ours)}] reference [{ids_text(r.ids)}]\n")
            }
        }
    }
    t |> equal(bad, 0, "sentences whose ids differ from the reference, of {length(fx.rows)}")
    // the probes: byte fallback on an emoji and a Cyrillic capital, whitespace runs, tabs and newlines
    for (p in fx.probes) {
        var inscope ours <- unigram_encode(tk, p.text, unk_id, byte_fallback, reserved)
        t |> success(ids_equal(ours, p.ids), "'{p.text}': ours [{ids_text(ours)}] reference [{ids_text(p.ids)}]")
    }
    // decode returns the text the ids came from
    var bad_decode = 0
    for (r in fx.rows) {
        if (spm_decode_ids(tk, r.ids) != r.prepared) {
            bad_decode++
        }
    }
    t |> equal(bad_decode, 0, "sentences whose ids do not decode back to the prepared text")
    for (p in fx.probes) {
        t |> equal(spm_decode_ids(tk, p.ids), p.text, "'{p.text}' decodes back")
    }
    var inscope none <- unigram_encode(tk, "", unk_id, byte_fallback, reserved)
    t |> equal(length(none), 0, "an empty text has no tokens")
    // a control piece typed into the text is its bytes, never the control id (the reference keeps
    // control pieces out of its trie); the unreserved encode is the control that shows the id would win
    var inscope typed <- unigram_encode(tk, "</s>", unk_id, byte_fallback, reserved)
    t |> success(find_index(typed, eos_id) < 0, "'</s>' in text never encodes to the EOS id: [{ids_text(typed)}]")
    t |> equal(spm_decode_ids(tk, typed), "</s>", "and decodes back to the text")
    var inscope no_reserve : array<int64>
    var inscope unguarded <- unigram_encode(tk, "</s>", unk_id, byte_fallback, no_reserve)
    t |> success(find_index(unguarded, eos_id) >= 0, "without the reserved set the score-0 control piece wins: [{ids_text(unguarded)}]")
    // an encode with no unknown piece is refused, not fed -1
    var tripped = false
    try {
        var inscope bad_ids <- unigram_encode(tk, "hello", -1l, byte_fallback, reserved)
    } recover {
        tripped = true
    }
    t |> success(tripped, "unigram_encode panics on an unknown id outside the vocabulary")
}

// the reference's own text preparation, row by row: the oracle prepared every corpus sentence
// (`prepared`, `frames_after_eos`) and ours must land the same string and the same tail
[test]
def test_prepare_pocket_text(t : T?) {
    let path = gguf_path()
    if (!model_available(t, path)) {
        return
    }
    var inscope m <- load_pocket(path)
    var inscope fx <- load_tokens_fixture()
    var inscope corpus <- load_tts_corpus()
    var inscope by_id <- {for (row in corpus); row.id => row.text}
    var bad = 0
    var checked = 0
    for (r in fx.rows) {
        let text = by_id?[r.id] ?? ""
        continue if (empty(text))
        checked++
        let p = prepare_pocket_text(m, text)
        if (p.prepared != r.prepared || p.frames_after_eos + 2 != r.frames_after_eos) {
            bad++
            if (bad <= 3) {
                to_log(LOG_ERROR, "pocket prepare {r.id}: ours '{p.prepared}' +{p.frames_after_eos + 2} reference '{r.prepared}' +{r.frames_after_eos}\n")
            }
        }
    }
    t |> equal(checked, 200, "every fixture row found its corpus sentence")
    t |> equal(bad, 0, "sentences whose preparation differs from the reference's")
    // the terminal period follows the reference's isalnum: an accented final letter takes one
    t |> equal(prepare_pocket_text(m, "spero che ti piacerò").prepared, "Spero che ti piacerò.", "an accented final letter takes the period")   // nolint:STYLE039 - the accent is the case
    t |> equal(prepare_pocket_text(m, "Hello there,").prepared, "Hello there,", "a trailing mark is left as it is")
    // the word count folds whitespace runs and tabs, as the reference's split() does
    t |> equal(prepare_pocket_text(m, "One two three   four.").frames_after_eos, 3, "four words: the short-text tail")
    t |> equal(prepare_pocket_text(m, "One\ttwo three four five.").frames_after_eos, 1, "five words across a tab: the long-text tail")
    // an empty text is refused, as the reference refuses it; the chunker answers no chunks
    var tripped = false
    try {
        let p = prepare_pocket_text(m, "   ")
    } recover {
        tripped = true
    }
    t |> success(tripped, "prepare_pocket_text panics on a blank text")
    var inscope none <- pocket_chunks(m, "  \n ")
    t |> equal(length(none), 0, "a blank text has no chunks")
}

// ===== the parity rail against harness/pocket_oracle.py =====

def private oracle_dir() : string {
    return path_join(models_dir(), "tts_oracle/pocket_english_2026-04")
}

struct private PocketCase {
    file : string
    id : string
    text : string
    prepared : string
    voice : string
    seed : int
    n_tokens : int
    n_frames : int
    eos_frame : int
    n_samples : int
    voice_frames : int
    stages : bool
}

struct private PocketManifest {
    size : string
    revision : string
    seed_base : int
    temp : float
    cases : array<PocketCase>
}

//! The oracle manifest under `dir_path`; `cases` empty when the directory is not stocked.
def private load_manifest(dir_path : string = oracle_dir()) : PocketManifest {
    var m = PocketManifest()
    load_json_manifest(dir_path, m)
    return <- m
}

let CODEC_BAR = 2.0e-3    // the codec chain: eleven convs and a transformer of f32 GEMMs in a different order
let ENCODER_BAR = 2.0e-3
let FRAME_BAR = 5.0e-3    // one frame teacher-forced: six transformer layers and the head

[test]
def test_pocket_parity(t : T?) {
    let path = gguf_path()
    if (!model_available(t, path)) {
        return
    }
    var inscope man <- load_manifest()
    if (empty(man.cases)) {
        t |> skip("no oracle dumps under {oracle_dir()}")
        return
    }
    with_job_que() {
        setup_dasllama_jobque_()
        defer() {
            reset_pocket_q8()
        }
        set_pocket_q8(false)   // the parity rail holds the f32 lane
        let knob = tower_pin_off()
        defer() {
            tower_restore(knob)
        }
        var inscope m <- load_pocket(path)
        var inscope sc = PocketScratch()
        t |> success(!m.q8, "the reference lane serves f32")
        t |> equal(m.lang, "en", "the English model reports en")
        t |> equal(m.default_voice, "alba")
        t |> equal(m.frame_samples, 1920l, "samples per latent frame")
        var voiced = 0
        for (c in man.cases) {
            continue if (!c.stages)
            var inscope dump <- load_tts_dump(path_join(oracle_dir(), c.file))
            let tag = "{c.id} {c.voice}"
            // the codec decoder over the oracle's own latents, one shot
            var inscope wave : array<float>
            pocket_decode_latents(m, dump["latent"].f, int64(c.n_frames), sc, wave)
            bar_diff(t, "{tag} codec decode", wave, dump["waveform"].f, CODEC_BAR, false)
            // the codec encoder and the voice prompt, once per voice
            if (voiced < 2) {
                var inscope lat : array<float>
                let frames = pocket_encode_latents(m, dump["voice_pcm"].f, sc, lat)
                t |> equal(int(frames), c.voice_frames, "{tag}: the clip's latent frame count")
                bar_diff(t, "{tag} codec encode", lat, dump["voice_latents"].f, ENCODER_BAR, false)
                var inscope vs <- pocket_voice_state(m, dump["voice_pcm"].f, sc)
                t |> equal(int(vs.len), c.voice_frames + 1, "{tag}: the prompt is the BOS row plus the frames")
                for (l in range(length(vs.caches))) {
                    let name = "kv_voice_L{l}"
                    continue if (!key_exists(dump, name))
                    var inscope ours : array<float>
                    let tt = vs.len
                    let hh = vs.caches[l].heads
                    let dh = vs.caches[l].dh
                    let cap = vs.caches[l].cap
                    ours |> resize(2l * tt * hh * dh)
                    for (p in range64(tt)) {
                        for (h in range64(hh)) {
                            for (e in range64(dh)) {
                                ours[(p * hh + h) * dh + e] = vs.caches[l].k[(h * dh + e) * cap + p]
                                ours[tt * hh * dh + (p * hh + h) * dh + e] = vs.caches[l].v[(h * cap + p) * dh + e]
                            }
                        }
                    }
                    bar_diff(t, "{tag} voice kv layer {l}", ours, dump[name].f, FRAME_BAR, false)
                }
                voiced++
            }
        }
    }
}

// every frame's x1 from the head alone, over the oracle's conditioning rows and noise draws
def private head_over_frames(m : PocketModel; var sc : PocketScratch; conds, x0 : array<float>; frames : int64) : array<float> {
    let d = m.backbone.d
    let ld = m.latent_dim
    var inscope out : array<float>
    out |> resize(frames * ld)
    var inscope crow : array<float>
    crow |> resize(d)
    var inscope nrow : array<float>
    nrow |> resize(ld)
    var inscope x1 : array<float>
    for (f in range64(frames)) {
        for (j in range64(d)) {
            crow[j] = conds[f * d + j]
        }
        for (j in range64(ld)) {
            nrow[j] = x0[f * ld + j]
        }
        pocket_head(m, crow, nrow, sc, x1)
        for (j in range64(ld)) {
            out[f * ld + j] = x1[j]
        }
    }
    return <- out
}

// the rows of the codec chain's ping-pong scratch, the window's working set
def private codec_scratch_bytes(sc : PocketScratch) : int64 {
    return (long_capacity(sc.ca) + long_capacity(sc.cb) + long_capacity(sc.cc) + long_capacity(sc.cpad) + long_capacity(sc.cs) + long_capacity(sc.ct)) * 4l
}

let STREAM_BAR = 1.0e-5   // absolute, on a waveform or a latent frame: float noise of the same sums in another batching

//! the codec stream (ARCHITECTURE_POCKET.md sec.2.46): a chunk decoded and a clip encoded a
//! window of frames per step give the whole run's output on the f32 lane, one frame per step
//! included, with the activations scaling with the window
[test]
def test_pocket_codec_stream(t : T?) {
    let path = gguf_path()
    if (!model_available(t, path)) {
        return
    }
    var inscope man <- load_manifest()
    var picked = -1
    for (i, c in iter_range(man.cases), man.cases) {
        if (c.stages && picked < 0) {
            picked = i
        }
    }
    if (picked < 0) {
        t |> skip("no oracle dump with stages under {oracle_dir()}")
        return
    }
    let c & = unsafe(man.cases[picked])
    with_job_que() {
        setup_dasllama_jobque_()
        defer() {
            reset_pocket_q8()
            set_pocket_codec_window(POCKET_CODEC_WINDOW_DEFAULT)
        }
        set_pocket_q8(false)   // the f32 lane: the served GEMMs' result depends on how many rows they batch
        let knob = tower_pin_off()
        defer() {
            tower_restore(knob)
        }
        var inscope m <- load_pocket(path)
        var inscope dump <- load_tts_dump(path_join(oracle_dir(), c.file))
        let frames = int64(c.n_frames)
        set_pocket_codec_window(0l)
        var inscope whole = PocketScratch()
        var inscope ref_wave : array<float>
        pocket_decode_latents(m, dump["latent"].f, frames, whole, ref_wave)
        var inscope ref_lat : array<float>
        let ref_frames = pocket_encode_latents(m, dump["voice_pcm"].f, whole, ref_lat)
        let whole_bytes = codec_scratch_bytes(whole)
        t |> equal(long_length(ref_wave), frames * m.frame_samples, "the whole run decodes frames * frame_samples samples")
        for (w in [1l, POCKET_CODEC_WINDOW_DEFAULT]) {
            set_pocket_codec_window(w)
            var inscope sc = PocketScratch()
            var inscope wave : array<float>
            pocket_decode_latents(m, dump["latent"].f, frames, sc, wave)
            bar_diff(t, "{c.id} decode, window {w}", wave, ref_wave, STREAM_BAR, true)
            var inscope lat : array<float>
            let lframes = pocket_encode_latents(m, dump["voice_pcm"].f, sc, lat)
            t |> equal(lframes, ref_frames, "window {w}: the clip's frame count")
            bar_diff(t, "{c.id} encode, window {w}", lat, ref_lat, STREAM_BAR, true)
            let bytes = codec_scratch_bytes(sc)
            to_log(LOG_INFO, "pocket codec scratch: window {w} {bytes} bytes, whole run {whole_bytes}\n")
            if (frames > 4l * w) {
                t |> success(bytes * 2l < whole_bytes, "window {w}: the codec scratch is under half the whole run's ({bytes} against {whole_bytes})")
            }
            // the release: every carried buffer gone, and the next run grows them back to the same output
            pocket_release_scratch(sc)
            t |> equal(codec_scratch_bytes(sc), 0l, "window {w}: the released carrier holds no codec rows")
            t |> equal(long_capacity(sc.qkv) + long_capacity(sc.h), 0l, "window {w}: the released carrier holds no transformer rows")
            var inscope again : array<float>
            pocket_decode_latents(m, dump["latent"].f, frames, sc, again)
            bar_diff(t, "{c.id} decode after the release, window {w}", again, ref_wave, STREAM_BAR, true)
        }
        // a voice's caches reserve the rows a chunk of the budget can take, not a flat slack
        var inscope vs <- pocket_voice_state(m, dump["voice_pcm"].f, whole)
        let budget = int64(POCKET_MAX_TOKENS) + pocket_max_frames(int64(POCKET_MAX_TOKENS)) + 1l
        t |> equal(vs.caches[0].cap, vs.len + budget, "the voice's caches hold the clip plus one chunk of the budget ({vs.len} + {budget} rows)")
    }
}

[test]
def test_pocket_frames(t : T?) {
    let path = gguf_path()
    if (!model_available(t, path)) {
        return
    }
    var inscope man <- load_manifest()
    if (empty(man.cases)) {
        t |> skip("no oracle dumps under {oracle_dir()}")
        return
    }
    with_job_que() {
        setup_dasllama_jobque_()
        defer() {
            reset_pocket_q8()
        }
        set_pocket_q8(false)
        let knob = tower_pin_off()
        defer() {
            tower_restore(knob)
        }
        var inscope m <- load_pocket(path)
        var inscope sc = PocketScratch()
        var done = 0
        for (c in man.cases) {
            continue if (!c.stages || c.voice != "alba")
            let tag = "{c.id} {c.voice}"
            var inscope k <- forced_case(m, c, man, sc)
            t |> equal(length(k.ids), c.n_tokens, "{tag}: token count")
            // teacher-forced: the oracle's noise and the oracle's frames fed back, so every frame
            // is one backbone step and one head step away from the reference
            let frames = forced_frames(m, k, k.x0, k.x1, sc)
            t |> equal(int(frames), c.n_frames, "{tag}: frame count under teacher forcing")
            var inscope rows <- frame_rows(m, sc, frames)
            // the head alone, fed the oracle's own conditioning and noise: the seam between the two
            var inscope head_x1 <- head_over_frames(m, sc, k.dump["cond"].f, k.x0, frames)
            bar_diff(t, "{tag} flow head on the oracle's conditioning", head_x1, k.x1, FRAME_BAR, false)
            bar_diff(t, "{tag} backbone conditioning (teacher-forced)", rows.conds, k.dump["cond"].f, FRAME_BAR, false)
            bar_diff(t, "{tag} eos logits (teacher-forced)", rows.eos, k.dump["eos_logit"].f, FRAME_BAR, false)
            bar_diff(t, "{tag} latents (teacher-forced)", rows.lat, k.dump["latent"].f, FRAME_BAR, false)
            bar_diff(t, "{tag} waveform (teacher-forced)", sc.wave, k.dump["waveform"].f, CODEC_BAR * 4.0, false)
            to_log(LOG_INFO, "pocket {tag}: {timings_line(k.tm, m.sample_rate)}\n")
            // free-running on the oracle's noise: the frames drift as the errors feed back; logged
            var inscope none : array<float>
            let free = pocket_synthesize(m, k.ids, k.vs, man.temp, k.fae, k.x0, none, k.tm, sc)
            to_log(LOG_INFO, "pocket {tag}: free run {free} frames against the oracle's {c.n_frames}, {timings_line(k.tm, m.sample_rate)}\n")
            done++
            break if (done >= 2)
        }
        t |> success(done > 0, "at least one stage case ran")
    }
}


//! The codec seat on the Metal tower against the CPU chain over the oracle's latents, on the f32
//! lane: the served leg within GPU_CODEC_BAR with the bar's control and the compare's control, one
//! tower encode a call, and the knob-off leg bit-equal to the CPU chain with its decline recorded.
[test]
def test_pocket_codec_metal(t : T?) {
    let path = gguf_path()
    if (!model_available(t, path)) {
        return
    }
    var inscope man <- load_manifest()
    if (empty(man.cases)) {
        t |> skip("no oracle dumps under {oracle_dir()}")
        return
    }
    static_if (typeinfo builtin_module_exists(das_metal)) {
        with_job_que() {
            setup_dasllama_jobque_()
            defer() {
                reset_pocket_q8()
            }
            set_pocket_q8(false)
            let knob_found = metal_tower_knob()
            defer() {
                set_metal_tower(knob_found)
            }
            var inscope m <- load_pocket(path)
            var inscope sc = PocketScratch()
            var done = 0
            for (c in man.cases) {
                continue if (!c.stages)
                var inscope dump <- load_tts_dump(path_join(oracle_dir(), c.file))
                let frames = int64(c.n_frames)
                let tag = "{c.id} {c.voice}"
                var inscope a : array<float>
                return if (!codec_served_leg(t, m, tag, "codec", dump["latent"].f, frames, GPU_CODEC_BAR, sc, a))
                knob_off_leg(t, "{tag} GPU codec", 1l, $ => pocket_gpu_stats("codec")) $ {
                    var inscope off : array<float>
                    pocket_decode_latents(m, dump["latent"].f, frames, sc, off)
                    return rel_rms(off, a) == 0.0
                }
                done++
                break if (done >= 2)
            }
            t |> success(done > 0, "at least one stage case ran")
        }
    } else {
        t |> skip("no Metal module in this build")
    }
}

//! The codec seat on the served planes of the q8 and kq files against each file's own CPU chain
//! over the oracle's latents, within GPU_CODEC_SERVED_BAR with the bar's one-sample control, one
//! hook call served and one encode a call.
[test]
def test_pocket_codec_metal_served(t : T?) {
    var inscope man <- load_manifest()
    if (empty(man.cases)) {
        t |> skip("no oracle dumps under {oracle_dir()}")
        return
    }
    static_if (typeinfo builtin_module_exists(das_metal)) {
        with_job_que() {
            setup_dasllama_jobque_()
            let knob_found = metal_tower_knob()
            defer() {
                set_metal_tower(knob_found)
            }
            reset_pocket_q8()
            var files = 0
            for (path in [q8_gguf_path(), kq_gguf_path()]) {
                continue if (!model_available(t, path))
                files++
                var inscope m <- load_pocket(path)
                var inscope sc = PocketScratch()
                for (c in man.cases) {
                    continue if (!c.stages)
                    var inscope dump <- load_tts_dump(path_join(oracle_dir(), c.file))
                    let frames = int64(c.n_frames)
                    var inscope a : array<float>
                    return if (!codec_served_leg(t, m, "{c.id} {base_name(path)}", "codec served", dump["latent"].f, frames, GPU_CODEC_SERVED_BAR, sc, a))
                    break
                }
            }
            if (files == 0) {
                t |> skip("neither served file is stocked")
            }
        }
    } else {
        t |> skip("no Metal module in this build")
    }
}

//! The codec seat over `latents`: the CPU leg (knob off) into `cpu`, the served leg, x3-scaled latents the control, `gpu_stage_verdict` at `bar`; the knob stays on.
[unused_argument(t, m, tag, stage, latents, frames, bar, sc, cpu)]   // off-Apple the gated body is empty
def private codec_served_leg(t : T?; m : PocketModel; tag, stage : string; latents : array<float>; frames : int64; bar : float; var sc : PocketScratch;
                             var cpu : array<float>) : bool {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        set_metal_tower(false)
        pocket_decode_latents(m, latents, frames, sc, cpu)
        set_metal_tower(true)
        let h0 = pocket_gpu_stats("codec")
        let e0 = metal_tower_stats().encodes
        var inscope b : array<float>
        pocket_decode_latents(m, latents, frames, sc, b)
        let h1 = pocket_gpu_stats("codec")
        let e1 = metal_tower_stats().encodes
        var inscope lat_x <- scaled(latents, 3.0)
        var inscope bp : array<float>
        pocket_decode_latents(m, lat_x, frames, sc, bp)
        return gpu_stage_verdict(t, tag, stage, b, cpu, bp, bar, h1.calls - h0.calls, h1.served - h0.served, e1 - e0, "{metal_tower_declines()}")
    } else {
        return false
    }
}

//! One oracle case set up for teacher forcing: its dump, token ids and voice state, the oracle's noise (`x0`) and
//! frames (`x1`), and what the frame loop reads beside them.
struct private ForcedCase {
    dump : table<string; TtsDumpTensor>
    ids : array<int64>
    vs : PocketVoiceState
    x0 : array<float>
    x1 : array<float>
    temp : float
    fae : int
    seed : uint64
    tm : TtsTimings
}

def private forced_case(m : PocketModel; c : PocketCase; man : PocketManifest; var sc : PocketScratch; odir : string = oracle_dir()) : ForcedCase {
    var inscope k = ForcedCase(temp = man.temp, fae = c.n_frames - c.eos_frame, seed = uint64(c.seed))
    k.dump <- load_tts_dump(path_join(odir, c.file))
    k.ids <- pocket_text_ids(m, c.prepared)
    k.vs <- pocket_voice_state(m, k.dump["voice_pcm"].f, sc)
    k.x0 := k.dump["x0"].f
    k.x1 := k.dump["x1"].f
    return <- k
}

//! The frame loop over `k` from its seed plus `reseed`, fed `captured` noise and `forced` frames; the frame count.
def private forced_frames(m : PocketModel; var k : ForcedCase; captured, forced : array<float>; var sc : PocketScratch; reseed : uint64 = 0ul) : int64 {
    rng_seed(sc.rng, k.seed + reseed)
    return pocket_synthesize(m, k.ids, k.vs, k.temp, k.fae, captured, forced, k.tm, sc)
}

//! The frames one teacher-forced loop left in the scratch: the latents, the conditioning rows and the EOS logits.
def private frame_rows(m : PocketModel; sc : PocketScratch; frames : int64) : FrameRows {
    var r : FrameRows
    r.lat |> resize(frames * m.latent_dim)
    for (i in range64(frames * m.latent_dim)) {
        r.lat[i] = sc.latents[i]
    }
    r.conds |> resize(frames * m.backbone.d)
    for (i in range64(frames * m.backbone.d)) {
        r.conds[i] = sc.conds[i]
    }
    r.eos |> resize(frames)
    for (i in range64(frames)) {
        r.eos[i] = sc.eos_logits[i]
    }
    return <- r
}

def private same_rng(a, b : TtsRng) : bool {
    return a.state == b.state && a.inc == b.inc && a.has_spare == b.has_spare && (!a.has_spare || a.spare == b.spare)
}

typedef FrameRows = tuple<lat : array<float>; conds : array<float>; eos : array<float>>

//! The frames seat teacher-forced on `k` against the CPU leg, `all_rows` adding conds and EOS; the CPU leg's frame count (-1 unserved) and rows; the knob stays on.
[cold_path, unused_argument(t, m, k, tag, bar, all_rows, sc)]   // off-Apple the gated body is empty
def private frames_seat_leg(t : T?; m : PocketModel; var k : ForcedCase; tag : string; bar : float; all_rows : bool;
                            var sc : PocketScratch) : tuple<fc : int64; cpu : FrameRows> {
    var r : tuple<fc : int64; cpu : FrameRows>
    r.fc = -1l
    static_if (typeinfo builtin_module_exists(das_metal)) {
        set_metal_tower(false)
        let fc = forced_frames(m, k, k.x0, k.x1, sc)
        r.cpu <- frame_rows(m, sc, fc)
        let rng_cpu = sc.rng
        set_metal_tower(true)
        let h0 = pocket_gpu_stats("frames")
        let e0 = metal_tower_stats().encodes
        let fg = forced_frames(m, k, k.x0, k.x1, sc)
        let h1 = pocket_gpu_stats("frames")
        let e1 = metal_tower_stats().encodes
        return <- r if (h1.served - h0.served != 1 && !tower_served(t, tag, "{metal_tower_declines()}"))
        t |> equal(h1.calls - h0.calls, 1, "{tag}: one hook call")
        t |> equal(fg, fc, "{tag}: the frame count")
        t |> success(same_rng(sc.rng, rng_cpu), "{tag}: the generator ends where the CPU loop's does")
        let batch = metal_pocket_frame_batch()
        t |> equal(e1 - e0, (fc + batch) / batch + 1l, "{tag}: one tower encode a batch of {batch}, and the codec's")
        var inscope gpu <- frame_rows(m, sc, fg)
        let dl = rel_rms(gpu.lat, r.cpu.lat)
        let dc = rel_rms(gpu.conds, r.cpu.conds)
        let de = rel_rms(gpu.eos, r.cpu.eos)
        let o_gpu = rel_rms(gpu.lat, k.dump["latent"].f)
        let o_cpu = rel_rms(r.cpu.lat, k.dump["latent"].f)
        to_log(LOG_INFO, "pocket {tag}: rel-rms latents {dl} conds {dc} eos {de} against the CPU chain over {fc} frames (bar {bar}); against the f32 oracle the tower {o_gpu}, the CPU chain {o_cpu}\n")
        t |> success(dl <= bar, "{tag}: the latents within {bar}")
        if (all_rows) {
            t |> success(dc <= bar && de <= bar, "{tag}: the conditioning and EOS logits within {bar}")
        }
        var inscope w <- one_wrong(gpu.lat, r.cpu.lat, bar)
        t |> success(rel_rms(w, r.cpu.lat) > bar, "{tag}: one latent moved by twice the bar reds it (the bar's control)")
        var inscope x0x <- scaled(k.x0, 3.0)
        let fx = forced_frames(m, k, x0x, k.x1, sc)
        var inscope gx <- frame_rows(m, sc, min(fx, fc))
        t |> success(rel_rms(gx.lat, r.cpu.lat) > bar, "{tag}: the x3-scaled noise reds the bar (the compare's control)")
        if (all_rows) {
            var inscope x1x <- scaled(k.x1, 3.0)
            let fx1 = forced_frames(m, k, k.x0, x1x, sc)
            var inscope gx1 <- frame_rows(m, sc, min(fx1, fc))
            t |> success(rel_rms(gx1.conds, r.cpu.conds) > bar && rel_rms(gx1.eos, r.cpu.eos) > bar, "{tag}: the x3-scaled forced frames red the conditioning and EOS bars (their control)")
        }
        r.fc = fc
    }
    return <- r
}

[test]
def test_pocket_seat_stats(t : T?) {
    t |> run("a seat name no stage carries panics, so a misspelt key cannot read as zero counters") @(t : T?) {
        let why = panic_text() {
            pocket_gpu_stats("heads")
        }
        t |> success(find(why, "no GPU seat named 'heads'") >= 0, "the refusal names the key: {why}")
    }
    t |> run("the record names its seats in dispatch order, and each name reads its counters") @(t : T?) {
        var inscope seats <- pocket_gpu_seats()
        t |> equal(length(seats), 2, "two seats")
        t |> equal(seats[0], "codec", "the codec first")
        t |> equal(seats[1], "frames", "the frame loop second")
        for (s in seats) {
            let st = pocket_gpu_stats(s)
            t |> success(st.served <= st.calls, "{s}: served never past called")
        }
    }
}

//! The frames seat on the Metal tower against the CPU chain on the f32 lane, teacher-forced on the
//! oracle's noise and frames: the latents, the conditioning rows and the EOS logits within
//! GPU_FRAME_BAR with the bar's one-element control and the x3-scaled noise as the compare's
//! control, the generator left where the CPU loop leaves it, one hook call served, one encode a
//! batch, the knob-off leg bit-equal to the CPU chain with its decline recorded; then the free run
//! served.
[test]
def test_pocket_frames_metal(t : T?) {   // nolint:STYLE038 — one case's legs, in the order the counters read
    let path = gguf_path()
    if (!model_available(t, path)) {
        return
    }
    var inscope man <- load_manifest()
    if (empty(man.cases)) {
        t |> skip("no oracle dumps under {oracle_dir()}")
        return
    }
    static_if (typeinfo builtin_module_exists(das_metal)) {
        with_job_que() {
            setup_dasllama_jobque_()
            defer() {
                reset_pocket_q8()
            }
            set_pocket_q8(false)
            let knob_found = metal_tower_knob()
            defer() {
                set_metal_tower(knob_found)
            }
            var inscope m <- load_pocket(path)
            var inscope sc = PocketScratch()
            var done = 0
            for (c in man.cases) {
                continue if (!c.stages || c.voice != "alba")
                let tag = "{c.id} {c.voice} frames"
                var inscope k <- forced_case(m, c, man, sc)
                var inscope leg <- frames_seat_leg(t, m, k, tag, GPU_FRAME_BAR, true, sc)
                return if (leg.fc < 0l)
                let fc = leg.fc
                knob_off_leg(t, tag, 2l, $ => pocket_gpu_stats("frames")) $ {
                    let fo = forced_frames(m, k, k.x0, k.x1, sc)
                    var inscope off <- frame_rows(m, sc, fo)
                    return fo == fc && rel_rms(off.lat, leg.cpu.lat) == 0.0
                }
                var inscope none : array<float>
                let free_cpu = forced_frames(m, k, none, none, sc)
                var inscope cpu_free <- frame_rows(m, sc, free_cpu)
                let rng_free = sc.rng
                set_metal_tower(true)
                let hf0 = pocket_gpu_stats("frames")
                let free = forced_frames(m, k, none, none, sc)
                let hf1 = pocket_gpu_stats("frames")
                t |> equal(hf1.served - hf0.served, 1, "{tag}: the free run serves on the tower")
                t |> success(speech_stats(sc.wave).rms > 0.01, "{tag}: the free run carries speech")
                t |> equal(free, free_cpu, "{tag}: the free run makes the CPU chain's frame count")
                t |> success(same_rng(sc.rng, rng_free), "{tag}: the free run's generator ends where the CPU loop's does")
                var inscope gpu_free <- frame_rows(m, sc, min(free, free_cpu))
                let df = rel_rms(gpu_free.lat, cpu_free.lat)
                to_log(LOG_INFO, "pocket {tag}: free run {free} frames against the oracle's {c.n_frames}, rel-rms latents {df} against the CPU chain's free run on the same seed (bar {GPU_FRAME_FREE_BAR}), {timings_line(k.tm, m.sample_rate)}\n")
                t |> success(df <= GPU_FRAME_FREE_BAR, "{tag}: the free run's latents within {GPU_FRAME_FREE_BAR} of the CPU chain's on the same noise")
                var inscope wf <- one_wrong(gpu_free.lat, cpu_free.lat, GPU_FRAME_FREE_BAR)
                t |> success(rel_rms(wf, cpu_free.lat) > GPU_FRAME_FREE_BAR, "{tag}: one latent moved by twice the bar reds it (the bar's control)")
                let free_other = forced_frames(m, k, none, none, sc, 1ul)
                var inscope gpu_other <- frame_rows(m, sc, min(free_other, free_cpu))
                t |> success(rel_rms(gpu_other.lat, cpu_free.lat) > GPU_FRAME_FREE_BAR, "{tag}: the next seed's free run reds the bar (the compare's control)")
                let batch_found = metal_pocket_frame_batch()
                set_metal_pocket_frame_batch(3l)
                let eb0 = metal_tower_stats().encodes
                let f3 = forced_frames(m, k, k.x0, k.x1, sc)
                let eb1 = metal_tower_stats().encodes
                var inscope b3 <- frame_rows(m, sc, f3)
                t |> equal(f3, fc, "{tag}: batches of three make the frame count")
                t |> equal(eb1 - eb0, (fc + 3l) / 3l + 1l, "{tag}: one tower encode a batch of three, and the codec's")
                t |> success(rel_rms(b3.lat, leg.cpu.lat) <= GPU_FRAME_BAR, "{tag}: batches of three land within {GPU_FRAME_BAR}")
                set_metal_pocket_frame_batch(0l)
                t |> equal(metal_pocket_frame_batch(), 1l, "{tag}: a batch below one clamps to one")
                set_metal_pocket_frame_batch(batch_found)
                register_pocket_gpu(PocketGpuDriver())   //! the seats taken by an empty record: the tower's registration is displaced
                let ht0 = pocket_gpu_stats("frames")
                let ft = forced_frames(m, k, k.x0, k.x1, sc)
                let ht1 = pocket_gpu_stats("frames")
                t |> equal(ht1.calls - ht0.calls, 0, "{tag}: an empty record reaches no hook")
                t |> equal(ft, fc, "{tag}: and the CPU chain speaks")
                unregister_pocket_gpu()   //! given back: the displaced registration serves again
                let hr0 = pocket_gpu_stats("frames")
                let fr = forced_frames(m, k, k.x0, k.x1, sc)
                let hr1 = pocket_gpu_stats("frames")
                t |> equal(hr1.served - hr0.served, 1, "{tag}: the registration given back serves again")
                t |> equal(fr, fc, "{tag}: at the frame count")
                done++
                break if (done >= 2)
            }
            t |> success(done > 0, "at least one stage case ran")
        }
    } else {
        t |> skip("no Metal module in this build")
    }
}

//! The frames seat on the served planes of the q8 and kq files against each file's own CPU chain,
//! teacher-forced on the oracle's noise and frames: the latents within GPU_FRAME_SERVED_BAR with
//! the bar's one-element control, the frame count equal, one hook call served, one encode a batch;
//! the distance to the f32 oracle logged beside the CPU chain's.
[test]
def test_pocket_frames_metal_served(t : T?) {
    var inscope man <- load_manifest()
    if (empty(man.cases)) {
        t |> skip("no oracle dumps under {oracle_dir()}")
        return
    }
    static_if (typeinfo builtin_module_exists(das_metal)) {
        with_job_que() {
            setup_dasllama_jobque_()
            let knob_found = metal_tower_knob()
            defer() {
                set_metal_tower(knob_found)
            }
            reset_pocket_q8()
            var files = 0
            for (path in [q8_gguf_path(), kq_gguf_path()]) {
                continue if (!model_available(t, path))
                files++
                var inscope m <- load_pocket(path)
                var inscope sc = PocketScratch()
                var voices_seen : table<string; bool>
                for (c in man.cases) {
                    continue if (!c.stages || key_exists(voices_seen, c.voice))   //! one case a voice: the second voice's slot displaces the first's within the process
                    voices_seen[c.voice] = true
                    var inscope k <- forced_case(m, c, man, sc)
                    var inscope leg <- frames_seat_leg(t, m, k, "{c.id} {c.voice} {base_name(path)} frames served", GPU_FRAME_SERVED_BAR, false, sc)
                    return if (leg.fc < 0l)
                }
                if (length(voices_seen) < 2) {
                    t |> skip("{base_name(path)}: the oracle carries one voice's stage cases, so no second voice took the seat")
                }
            }
            if (files == 0) {
                t |> skip("neither served file is stocked")
            }
        }
    } else {
        t |> skip("no Metal module in this build")
    }
}

//! The served synthesis across the tower knob on the served lane: every chunk's codec and frame
//! loop on the tower, one codec encode a chunk, the knob-off leg declining both seats per chunk,
//! both legs speaking.
[test]
def test_pocket_synthesis_metal(t : T?) {
    let path = gguf_path()
    if (!model_available(t, path)) {
        return
    }
    static_if (typeinfo builtin_module_exists(das_metal)) {
        with_job_que() {
            setup_dasllama_jobque_()
            reset_pocket_q8()   //! the served lane is the unpinned default, whatever an earlier cell left
            var inscope m <- load_tts_model(path)
            let text = "The tower serves the codec, and the words still land. Then it slept."
            served_lane_counters(t, m, text, "alba", "pocket served lane", "") $(a, b : TtsAudio; chunks : int; encodes : int64) {
                t |> success(encodes > int64(chunks), "the tower encoded the frame batches and one codec pass a chunk ({encodes} encodes over {chunks} chunk(s))")
                t |> success(speech_stats(a.pcm).finite && speech_stats(b.pcm).finite, "both legs finite")
                t |> success(length(a.pcm) > 24000 && length(b.pcm) > 24000, "both legs speak ({length(a.pcm)} and {length(b.pcm)} samples)")
                to_log(LOG_INFO, "pocket served lane: CPU {timings_line(a.timings, a.sample_rate)}; tower {timings_line(b.timings, b.sample_rate)}\n")
            }
        }
    } else {
        t |> skip("no Metal module in this build")
    }
}

// the q8 lane against the f32 oracle: a frame's latent is a flow sample, and a quantized
// conditioning row moves a few of its 32 elements by whole tenths, so the per-element figures are
// logged and the lane's quality gate is the rig (harness/tts_rig.py --q8 against --f32)
let Q8_RMS_BAR = 0.5   // a quantized conditioning row moves a flow sample by tenths on a few elements

def private q8_report(t : T?; tag : string; ours, ref : array<float>) {
    if (length(ours) != length(ref)) {
        t |> failure("{tag}: {length(ours)} elements against the oracle's {length(ref)}")
        return
    }
    let rel = rel_rms(ours, ref)
    to_log(LOG_INFO, "pocket {tag}: rms relative {rel}, max |diff| {max_abs_diff(ours, ref)}\n")
    t |> success(rel < Q8_RMS_BAR, "{tag}: rms relative {rel} is in the same picture as the oracle")
    // the control: the expectation shifted by its own rms lands outside the bar
    var ssr = 0.0lf
    for (b in ref) {
        ssr += double(b * b)
    }
    let shift = float(sqrt(ssr / double(max(length(ref), 1))))
    var inscope poisoned := ref
    for (v in poisoned) {
        v += shift
    }
    t |> success(rel_rms(ours, poisoned) >= Q8_RMS_BAR, "{tag}: the rms bar discriminates a poisoned expectation")
}

[test]
def test_pocket_q8_lane(t : T?) {
    let path = gguf_path()
    if (!model_available(t, path)) {
        return
    }
    var inscope man <- load_manifest()
    if (empty(man.cases)) {
        t |> skip("no oracle dumps under {oracle_dir()}")
        return
    }
    with_job_que() {
        setup_dasllama_jobque_()
        defer() {
            reset_pocket_q8()
        }
        set_pocket_q8(true)   // the lane this cell measures, pinned - the sibling cells pin the other
        var inscope m <- load_pocket(path)
        t |> success(m.q8, "the pinned q8 lane serves")
        t |> success(pocket_serves_q8(), "the pin is what the policy answers")
        t |> success(m.backbone.layers[0].in_proj.q8 && !m.mimi.dec_in.q8 && !m.mimi.enc_in.q8 && !m.mimi.dec_out.q8,
            "the backbone GEMMs serve q8; every codec conv serves f32")
        var inscope sc = PocketScratch()
        var done = 0
        for (c in man.cases) {
            continue if (!c.stages || c.voice != "alba")
            let tag = "{c.id} {c.voice} q8"
            var inscope k <- forced_case(m, c, man, sc)
            let frames = forced_frames(m, k, k.x0, k.x1, sc)
            t |> equal(int(frames), c.n_frames, "{tag}: frame count under teacher forcing")
            var inscope rows <- frame_rows(m, sc, frames)
            q8_report(t, "{tag} latents (teacher-forced)", rows.lat, k.dump["latent"].f)
            var inscope none : array<float>
            let free = pocket_synthesize(m, k.ids, k.vs, man.temp, k.fae, k.x0, none, k.tm, sc)
            t |> success(abs(int(free) - c.n_frames) <= 4, "{tag}: the free run's {free} frames sit by the oracle's {c.n_frames}")
            t |> success(speech_stats(sc.wave).rms > 0.01, "{tag}: the free run carries speech")
            to_log(LOG_INFO, "pocket {tag}: free run {free} frames against the oracle's {c.n_frames}, {timings_line(k.tm, m.sample_rate)}\n")
            done++
            break if (done >= 2)
        }
        t |> success(done > 0, "at least one stage case ran")
    }
}

def private q8_gguf_path() : string {
    return path_join(models_dir(), "pocket-tts-en-q8.gguf")
}

// the frames of one teacher-forced case on a model, for a lane-to-lane compare
def private forced_latents(m : PocketModel; c : PocketCase; man : PocketManifest; var sc : PocketScratch; odir : string = oracle_dir()) : array<float> {
    var inscope k <- forced_case(m, c, man, sc, odir)
    let frames = forced_frames(m, k, k.x0, k.x1, sc)
    var inscope rows <- frame_rows(m, sc, frames)
    return <- rows.lat
}

[test]
def test_pocket_q8_file(t : T?) {
    //! The published form: a file whose served GEMM weights are Q8_0 blocks in the kernels'
    //! layout, read straight into the int8 planes, against the f16 file quantized at load.
    let path = gguf_path()
    let q8path = q8_gguf_path()
    if (!model_available(t, path) || !model_available(t, q8path)) {
        return
    }
    var inscope man <- load_manifest()
    if (empty(man.cases)) {
        t |> skip("no oracle dumps under {oracle_dir()}")
        return
    }
    with_job_que() {
        setup_dasllama_jobque_()
        var inscope sc = PocketScratch()
        defer() {
            reset_pocket_q8()
        }
        set_pocket_q8(true)
        var inscope mq <- load_pocket(q8path)
        var inscope mf <- load_pocket(path)
        t |> success(mq.q8 && mf.q8, "both loads serve the q8 lane")
        var from_file = 0
        for (l in mq.backbone.layers) {
            from_file += (l.in_proj.q8 && l.out_proj.q8 && l.ffn1.q8 && l.ffn2.q8) ? 1 : 0
        }
        t |> equal(from_file, length(mq.backbone.layers), "every backbone layer's four GEMMs came in as Q8_0")
        t |> success(!mq.mimi.dec_in.q8 && !mq.mimi.enc_out.q8 && !mq.mimi.dec_res[0].conv1.q8 && !mf.mimi.dec_in.q8,
            "the codec's convs serve f32 on the q8 lane, whichever form the file stores them in")
        for (c in man.cases) {
            continue if (!c.stages || c.voice != "alba")
            var inscope a <- forced_latents(mq, c, man, sc)
            var inscope b <- forced_latents(mf, c, man, sc)
            let rel = rel_rms(a, b)
            to_log(LOG_INFO, "pocket {c.id} q8 file vs q8 lane: rms relative {rel}\n")
            // the file's block scales are f16 (the Q8_0 format), the load-time quantizer keeps
            // them f32, and the flow head amplifies that half-a-thousandth per block into
            // percents of a frame; the published file's own rig row is its quality gate
            t |> success(rel < Q8_FILE_BAR, "{c.id}: the file's blocks and the load-time quants are the same lane (rms relative {rel})")
            var inscope poisoned := b
            for (v in poisoned) {
                v += 2.0
            }
            t |> success(rel_rms(a, poisoned) >= Q8_FILE_BAR, "{c.id}: the bar discriminates a poisoned expectation")
            break
        }
        // the same file on the f32 lane: every Q8_0 tensor dequantized and re-laid, against the f16
        // file's f32 lane - the published file's other lane, the one a --tts-lane f32 boot serves
        set_pocket_q8(false)
        var inscope mq32 <- load_pocket(q8path)
        var inscope mf32 <- load_pocket(path)
        t |> success(!mq32.q8 && !mf32.q8, "both loads serve the f32 lane")
        t |> success(!mq32.backbone.layers[0].in_proj.q8 && !mq32.mimi.dec_in.q8, "the published file's Q8_0 tensors dequantized into f32 planes")
        for (c in man.cases) {
            continue if (!c.stages || c.voice != "alba")
            var inscope a <- forced_latents(mq32, c, man, sc)
            var inscope b <- forced_latents(mf32, c, man, sc)
            let rel = rel_rms(a, b)
            to_log(LOG_INFO, "pocket {c.id} q8 file on the f32 lane vs the f16 file: rms relative {rel}\n")
            t |> success(rel < Q8_FILE_BAR, "{c.id}: the dequantized file serves the f16 file's frames (rms relative {rel})")
            break
        }
    }
}

let Q8_FILE_BAR = 5.0e-2
let KQ_EXACT_BAR = 5.0e-2   //! the kq lane against the exact lane of the same file (measured 3.4e-2): the head's Q8_0 route and the activation quants
let KQ_HEAD_BAR = 2.0e-2    //! the flow head on its Q8_0 route against the f32 oracle's frames (measured 9.8e-3), the oracle's own conditioning and noise in

def private kq_gguf_path() : string {
    return path_join(models_dir(), "pocket-tts-en-kq.gguf")
}

[test]
def test_pocket_kq_file(t : T?) {
    //! The small form (`convert_pocket.py --kq`): the backbone's and the codec transformers'
    //! matrices and the text embedding as Q4_K, the flow head as Q8_0. Unpinned, the K-quant
    //! tensors serve as their own planes (the kq lane); pinned q8 or f32 they dequantize into
    //! that lane. The three lanes of one file against each other and against the f16 reference.
    let path = gguf_path()
    let kqpath = kq_gguf_path()
    if (!model_available(t, path) || !model_available(t, kqpath)) {
        return
    }
    var inscope man <- load_manifest()
    if (empty(man.cases)) {
        t |> skip("no oracle dumps under {oracle_dir()}")
        return
    }
    with_job_que() {
        setup_dasllama_jobque_()
        var inscope sc = PocketScratch()
        defer() {
            reset_pocket_q8()
        }
        reset_pocket_q8()
        t |> success(pocket_serves_native(), "unpinned, a K-quant tensor serves as its own planes")
        var inscope mk <- load_pocket(kqpath)
        var kq_layers = 0
        for (l in mk.backbone.layers) {
            kq_layers += (l.in_proj.kq_fmt == 4 && l.out_proj.kq_fmt == 4 && l.ffn1.kq_fmt == 4 && l.ffn2.kq_fmt == 4) ? 1 : 0
        }
        t |> equal(kq_layers, length(mk.backbone.layers), "every backbone layer's four GEMMs came in as Q4_K planes")
        t |> success(mk.mimi.dec_tf.layers[0].ffn1.kq_fmt == 4 && mk.mimi.enc_tf.layers[0].in_proj.kq_fmt == 4, "the codec transformers' matrices too")
        t |> success(mk.head.blocks[0].mlp1.q8 && mk.head.final_linear.q8 && mk.head.cond_embed.q8 && mk.head.input_proj.q8, "the flow head's matrices came in as Q8_0 blocks, its 32-wide input projection included")
        t |> success(!mk.speaker_proj.q8 && mk.speaker_proj.kq_fmt == 0, "the speaker projection stays f32")
        t |> equal(long_length(mk.text_emb.a), 4001l * mk.backbone.d, "the Q4_K embedding table dequantized into its f32 form")
        // the same file on the q8 lane: every Q4_K tensor dequantized and requantized to Q8_0 (a
        // re-encode of the same values), so the two lanes differ by that re-encode and their
        // activation quants; the head is the same Q8_0 blocks on both
        set_pocket_q8(true)
        var inscope mq <- load_pocket(kqpath)
        t |> success(mq.backbone.layers[0].in_proj.q8 && mq.backbone.layers[0].in_proj.kq_fmt == 0, "pinned q8, the K-quant tensor is Q8_0 blocks")
        // the exact lane over the K-quant file: dequantized f32 planes, the head dequantized too -
        // the one load whose head is not the Q8_0 route, so it is what pins that route
        set_pocket_q8(false)
        var inscope m32 <- load_pocket(kqpath)
        t |> success(!m32.q8 && m32.backbone.layers[0].in_proj.kq_fmt == 0 && !m32.backbone.layers[0].in_proj.q8, "pinned f32, the K-quant tensor is an f32 plane")
        t |> success(!m32.head.blocks[0].mlp1.q8, "pinned f32, the head is f32 too")
        // the f16 file on its f32 lane: the reference the whole small form is measured against
        var inscope mf <- load_pocket(path)
        t |> success(!mf.q8 && !mf.backbone.layers[0].in_proj.q8, "the f16 reference serves its f32 lane")
        reset_pocket_q8()
        var compared = 0
        for (c in man.cases) {
            continue if (!c.stages || c.voice != "alba")
            compared++
            kq_lanes_case(t, c, man, mk, mq, m32, mf, sc)
            break
        }
        t |> success(compared > 0, "the oracle manifest carries a stages case in alba - the lanes were compared")
        kq_clone_over_roster(t, kqpath)
    }
}

// the three lanes of the small file on one oracle case: kq against q8 at the q8 bar (with the
// poisoned control), kq against the exact lane (the head's Q8_0 route), the f16 reference logged;
// then the stored roster against the reference encoder (the f16 file on its f32 lane)
[cold_path]
def private kq_lanes_case(t : T?; c : PocketCase; man : PocketManifest; var mk, mq, m32, mf : PocketModel; var sc : PocketScratch) {
    var inscope a <- forced_latents(mk, c, man, sc)
    var inscope b <- forced_latents(mq, c, man, sc)
    let rel = rel_rms(a, b)
    to_log(LOG_INFO, "pocket {c.id} kq lane vs the q8 lane of the same file: rms relative {rel}\n")
    t |> success(rel < Q8_FILE_BAR, "{c.id}: the kq lane and the q8 lane of one file agree (rms relative {rel})")
    var inscope poisoned := b
    for (v in poisoned) {
        v += 2.0
    }
    t |> success(rel_rms(a, poisoned) >= Q8_FILE_BAR, "{c.id}: the bar discriminates a poisoned expectation")
    var inscope e <- forced_latents(m32, c, man, sc)
    let rel32 = rel_rms(a, e)
    to_log(LOG_INFO, "pocket {c.id} kq lane vs the exact lane of the same file: rms relative {rel32} (the head's Q8_0 route and the activation quants)\n")
    t |> success(rel32 < KQ_EXACT_BAR, "{c.id}: the kq lane and the exact lane of one file agree (rms relative {rel32})")
    var inscope f <- forced_latents(mf, c, man, sc)
    to_log(LOG_INFO, "pocket {c.id} kq lane vs the f16 reference: rms relative {rel_rms(a, f)} (the format's loss; the rig is its gate)\n")
    var inscope dump <- load_tts_dump(path_join(oracle_dir(), c.file))
    // the head alone on its Q8_0 route - Q8_0 blocks and a Q8_0-quantized activation vector per
    // GEMV - fed the oracle's own conditioning and noise, against the oracle's frames
    var inscope x0 := dump["x0"].f
    var inscope hk <- head_over_frames(mk, sc, dump["cond"].f, x0, int64(c.n_frames))
    let rel_head = rel_rms(hk, dump["x1"].f)
    to_log(LOG_INFO, "pocket {c.id} the flow head on Q8_0 blocks vs the oracle: rms relative {rel_head}\n")
    t |> success(rel_head < KQ_HEAD_BAR, "{c.id}: the head's Q8_0 route holds the oracle's frames (rms relative {rel_head})")
    var inscope head_poisoned := dump["x1"].f
    for (v in head_poisoned) {
        v += 2.0
    }
    t |> success(rel_rms(hk, head_poisoned) >= KQ_HEAD_BAR, "{c.id}: the head bar discriminates a poisoned expectation")
    var inscope lat : array<float>
    let frames = pocket_encode_latents(mf, dump["voice_pcm"].f, sc, lat)   // the f16 file's encoder, f32 weights: the parity rail's own; the kq file's carries Q4_K and Q8_0
    var inscope stored : array<float>
    mk.voice_latents |> get("alba") $(s) {
        stored := s
    }
    t |> equal(long_length(stored), frames * mk.latent_dim, "alba's stored frames are the clip's frame count")
    bar_diff(t, "{c.id} stored roster vs the reference encoder", stored, lat, ENCODER_BAR, false)
}

// a clone over a roster name: alba re-registered from a clip keeps the roster at 19 and speaks
def private kq_clone_over_roster(t : T?; kqpath : string) {
    var inscope mkf <- load_tts_model(kqpath)
    var inscope clip : array<float>
    clip |> resize(24000 * 3)
    var r = TtsRng()
    rng_seed(r, 7ul)
    for (i in range(length(clip))) {
        clip[i] = 0.3 * sin(float(i) * 0.05) * (0.5 + 0.5 * rng_normal(r))
    }
    tts_register_voice(mkf, "alba", clip, 24000)
    var inscope ck <- caps(mkf)
    t |> equal(length(ck.voices), 19, "a clone under a roster name replaces the voice, the roster count stays")
    var inscope spoken <- synthesize(mkf, "The quick brown fox jumps over the lazy dog.", "alba")
    let st = speech_stats(spoken.pcm)
    t |> success(st.finite && st.rms > 0.01, "the replaced roster voice speaks (rms {st.rms})")
}

// ===== the other five languages: one Q8_0 file each, its own tokenizer, one default voice =====

struct private PocketLang {
    code : string        // what caps reports
    config : string      // the package's config name: the oracle dir and the tokens fixture carry it
    voice : string
}

let private LANGS <- [PocketLang(code = "de", config = "german", voice = "juergen"), PocketLang(code = "es", config = "spanish", voice = "lola"),
                      PocketLang(code = "it", config = "italian", voice = "giovanni"), PocketLang(code = "pt", config = "portuguese", voice = "rafael"),
                      PocketLang(code = "fr", config = "french_24l", voice = "estelle")]

// one language's cells: the tokenizer on its own sentences and the probes, the teacher-forced
// frames of every stage case on the published file, one sentence through the facade
[cold_path]
def private language_cells(t : T?; L : PocketLang) {
    let path = path_join(models_dir(), "pocket-tts-{L.code}-q8.gguf")
    let odir = path_join(models_dir(), "tts_oracle/pocket_{L.config}")
    if (!model_available(t, path)) {
        return
    }
    var inscope man <- load_manifest(odir)
    if (empty(man.cases)) {
        t |> skip("{L.config}: no oracle dumps under {odir}")
        return
    }
    var inscope fx <- load_tokens_fixture("{get_das_root()}/modules/dasLLAMA/tests/_tts_fixtures/pocket_tokens_{L.config}.json")
    var inscope m <- load_pocket(path)
    t |> equal(m.lang, L.code, "{L.config}: the language code")
    t |> equal(m.default_voice, L.voice, "{L.config}: the default voice")
    t |> equal(length(m.voice_names), 1, "{L.config}: one bundled voice")
    var bad = 0
    for (r in fx.rows) {
        var inscope ours <- pocket_text_ids(m, r.prepared)
        if (!ids_equal(ours, r.ids)) {
            bad++
            to_log(LOG_ERROR, "pocket {L.config} tokens {r.id} '{r.prepared}': ours [{ids_text(ours)}] '{spm_decode_ids(m.tok, ours)}' reference [{ids_text(r.ids)}]\n")
        }
    }
    for (p in fx.probes) {
        var inscope ours <- pocket_text_ids(m, p.text)
        if (!ids_equal(ours, p.ids)) {
            bad++
            to_log(LOG_ERROR, "pocket {L.config} probe '{p.text}': ours [{ids_text(ours)}] '{spm_decode_ids(m.tok, ours)}' reference [{ids_text(p.ids)}]\n")
        }
    }
    t |> equal(bad, 0, "{L.config}: sentences and probes whose ids differ from the reference")
    var inscope sc = PocketScratch()
    var cases = 0
    for (c in man.cases) {
        continue if (!c.stages)
        var inscope lat <- forced_latents(m, c, man, sc, odir)
        var inscope dump <- load_tts_dump(path_join(odir, c.file))
        // on the q8 lane an EOS logit near the threshold moves the frame count by one, and
        // teacher forcing keeps the frames comparable up to the shorter of the two runs
        let ours = long_length(lat) / m.latent_dim
        let theirs = long_length(dump["latent"].f) / m.latent_dim
        t |> success(abs(ours - theirs) <= 2l, "{L.config} {c.id}: {ours} frames against the oracle's {theirs}")
        let n = min(long_length(lat), long_length(dump["latent"].f))
        var inscope a : array<float>
        var inscope b : array<float>
        a |> resize(n)
        b |> resize(n)
        for (i in range64(n)) {
            a[i] = lat[i]
            b[i] = dump["latent"].f[i]
        }
        q8_report(t, "{L.config} {c.id} latents (teacher-forced)", a, b)
        t |> success(speech_stats(sc.wave).rms > 0.01, "{L.config} {c.id}: the decoded frames carry speech")
        cases++
    }
    t |> success(cases > 0, "{L.config}: {cases} oracle cases ran")
    var inscope tm <- load_tts_model(path)
    var inscope c2 <- caps(tm)
    t |> equal(c2.langs[0], L.code, "{L.config}: the facade reports the language")
    var inscope a <- synthesize(tm, fx.rows[0].prepared, L.voice)
    let st = speech_stats(a.pcm)
    t |> success(st.finite && st.rms > 0.01, "{L.config}: '{fx.rows[0].prepared}' speaks ({length(a.pcm)} samples, rms {st.rms})")
    to_log(LOG_INFO, "pocket {L.config}: {timings_line(a.timings, a.sample_rate)}\n")
}

[test]
def test_pocket_languages(t : T?) {
    with_job_que() {
        setup_dasllama_jobque_()
        for (L in LANGS) {
            language_cells(t, L)
        }
    }
}

def private speech_stats(pcm : array<float>) : tuple<finite : bool; rms : float> {
    var finite = true
    var ssq = 0.0lf
    for (v in pcm) {
        finite = finite && abs(v) < 1e6   // a NaN compares false
        ssq += double(v * v)
    }
    return (finite = finite, rms = float(sqrt(ssq / double(max(length(pcm), 1)))))
}

[test]
def test_pocket_facade(t : T?) {
    let path = gguf_path()
    if (!model_available(t, path)) {
        return
    }
    with_job_que() {
        setup_dasllama_jobque_()
        var inscope m <- load_tts_model(path)
        var inscope c <- caps(m)
        t |> success(c.cloning, "a Pocket model clones")
        t |> success(!c.speed, "a Pocket model has no speed to honour")
        t |> equal(length(c.langs), 1)
        t |> equal(c.langs[0], "en")
        t |> equal(length(c.voices), 19, "the shipped English roster")
        t |> equal(c.voice_langs[0], "en")
        t |> equal(c.sample_rate, 24000)
        t |> equal(tts_voice_lang(m, "alba"), "en")
        // the chunker: the reference's own chunk lists over multi-sentence texts
        var inscope fx <- load_tokens_fixture()
        for (cc in fx.chunks) {
            var inscope ours <- tts_chunks(m, cc.text)
            var same = length(ours) == length(cc.chunks)
            if (same) {
                for (a, b in ours, cc.chunks) {
                    same = same && a == b
                }
            }
            if (!same) {
                to_log(LOG_ERROR, "pocket chunks differ for '{slice(cc.text, 0, 60)}...': ours {ours} reference {cc.chunks}\n")
            }
            t |> success(same, "chunks of a {length(cc.chunks)}-chunk text match the reference")
        }
        // one sentence spoken
        var inscope a <- synthesize(m, "The quick brown fox jumps over the lazy dog.", "alba")
        let st = speech_stats(a.pcm)
        t |> success(st.finite, "the PCM is finite")
        t |> success(st.rms > 0.01, "the PCM carries speech (rms {st.rms})")
        t |> success(length(a.pcm) >= 24000 && length(a.pcm) <= 24000 * 8, "speech length {length(a.pcm)} samples")
        t |> equal(a.timings.samples, long_length(a.pcm))
        t |> success(a.timings.backbone_us > 0l && a.timings.codec_us > 0l && a.timings.bert_us == 0l, "the family's own stages carry the time")
        to_log(LOG_INFO, "pocket facade: {timings_line(a.timings, a.sample_rate)}\n")
        // a cloned voice: the checked-in reference clip under a new name speaks and is listed
        var inscope clip <- load_audio_mono("{get_das_root()}/modules/dasLLAMA/models/jfk_ask_not.wav", 24000)
        t |> success(!empty(clip), "the checked-in reference clip is the clone's source")
        tts_register_voice(m, "cloned", clip, 24000)
        var inscope c2 <- caps(m)
        t |> equal(length(c2.voices), 20, "the cloned voice joined the roster")
        var inscope b <- synthesize(m, "Hello from a cloned voice.", "cloned")
        t |> success(speech_stats(b.pcm).rms > 0.01, "the cloned voice speaks")
        pocket_refusals(t, m, clip)
    }
}

// an unknown voice, a speed, a phoneme request and a clip at the wrong rate are each refused by name
def private pocket_refusals(t : T?; var m : TtsModel; clip : array<float>) {
    var why = panic_text() {
        var inscope none <- synthesize(m, "x", "nobody")
    }
    t |> success(why |> find("unknown voice") >= 0, "an unknown voice is refused: {why}")
    why = panic_text() {
        var inscope none <- synthesize(m, "x", "alba", 1.5)
    }
    t |> success(why |> find("speed") >= 0, "a speed is refused: {why}")
    why = panic_text() {
        let ps = tts_phonemize(m, "x")
    }
    t |> success(why |> find("phonemes") >= 0, "phonemizing is refused: {why}")
    why = panic_text() {
        tts_register_voice(m, "wrong", clip, 16000)
    }
    t |> success(why |> find("16000") >= 0, "a clip at another rate is refused: {why}")
    var inscope long_clip : array<float>
    long_clip |> resize((POCKET_MAX_VOICE_SECONDS + 1l) * 24000l)
    why = panic_text() {
        tts_register_voice(m, "endless", long_clip, 24000)
    }
    t |> success(why |> find("{POCKET_MAX_VOICE_SECONDS} s") >= 0, "a clip past the cap is refused by the cap: {why}")
    var inscope c3 <- caps(m)
    t |> equal(find_index(c3.voices, "endless"), -1, "and left no half voice in the roster")
}

// the file shape a demo ships: one voice stored as its latent frames, the codec encoder left out
// (`convert_pocket.py --voices stuart_bell --no-cloning --kq`) - it speaks its roster from the
// stored frames, says it cannot clone, and refuses a clip by name
[test]
def test_pocket_no_encoder(t : T?) {
    let path = path_join(models_dir(), "pocket-tts-en-stuart-kq.gguf")
    if (!model_available(t, path)) {
        return
    }
    with_job_que() {
        setup_dasllama_jobque_()
        t |> success(!tts_needs_packs(path), "a Pocket file needs no packs")
        var inscope m <- load_tts_model(path)
        var inscope c <- caps(m)
        t |> success(!c.cloning, "a file without the encoder does not clone")
        t |> success(!c.speed, "and has no speed to honour")
        t |> equal(length(c.voices), 1, "one voice in the roster")
        t |> equal(c.voices[0], "stuart_bell")
        t |> equal(length(c.langs), 1)
        t |> equal(c.langs[0], "en")
        var inscope a <- synthesize(m, "Whose woods these are I think I know.", "stuart_bell")
        let st = speech_stats(a.pcm)
        t |> success(st.finite && st.rms > 0.01, "the stored voice speaks (rms {st.rms})")
        t |> success(length(a.pcm) >= 24000 && length(a.pcm) <= 24000 * 8, "speech length {length(a.pcm)} samples")
        var inscope clip : array<float>
        clip |> resize(24000)
        let why = panic_text() {
            tts_register_voice(m, "x", clip, 24000)
        }
        t |> success(why |> find("no codec encoder") >= 0, "a clip is refused by the missing encoder: {why}")
        var inscope c2 <- caps(m)
        t |> equal(length(c2.voices), 1, "and the roster is unchanged")
    }
}

//! The first difference drops the low end the leading silence and the breath sit in, so the lowest twentieth of the 100 ms window RMS reads the hiss in the pauses, not the speech above it.
def private quiet_floor_db(pcm : array<float>; rate : int) : float {
    let win = rate / 10
    let n = (length(pcm) - 1) / win
    var rms : array<float>
    rms |> reserve(n)
    for (i in range(n)) {
        var ssq = 0.0lf
        for (j in range(win)) {
            let v = pcm[i * win + j + 1] - pcm[i * win + j]
            ssq += double(v * v)
        }
        rms |> push(float(sqrt(ssq / double(win))))
    }
    rms |> sort()
    let k = max(1, n / 20)
    var acc = 0.0
    for (i in range(k)) {
        acc += rms[i]
    }
    return 20.0 * log10(max(acc / float(k), 1e-9))
}

[test]
def test_pocket_quiet_floor(t : T?) {
    //! A per-frame parity bar reads the loud part of a frame, so the quiet floor is the only lane compare that catches a hiss in the silences.
    let path = kq_gguf_path()
    if (!model_available(t, path)) {
        return
    }
    with_job_que() {
        setup_dasllama_jobque_()
        defer() {
            reset_pocket_q8()
        }
        let text = "The quick brown fox jumps over the lazy dog. Then it slept, and all was still."
        set_pocket_q8(true)   // the lane this cell measures, pinned
        var inscope served <- load_tts_model(path)
        t |> success(served.pocket.q8, "the pinned q8 lane serves")
        var inscope a <- synthesize(served, text, "alba")
        delete served
        set_pocket_q8(false)
        var inscope reference <- load_tts_model(path)
        t |> success(!reference.pocket.q8, "the pinned f32 lane serves")
        var inscope b <- synthesize(reference, text, "alba")
        t |> success(length(a.pcm) >= 2 * a.sample_rate && length(b.pcm) >= 2 * b.sample_rate,
            "both renders carry seconds of audio ({length(a.pcm)} and {length(b.pcm)} samples) - the floor reads their quietest twentieth")
        let fa = quiet_floor_db(a.pcm, a.sample_rate)
        let fb = quiet_floor_db(b.pcm, b.sample_rate)
        to_log(LOG_INFO, "pocket quiet floor: served {fa} dBFS, f32 lane {fb} dBFS\n")
        t |> success(fa <= fb + 6.0, "the served lane's quiet floor ({fa} dBFS) sits within 6 dB of the f32 lane's ({fb} dBFS)")
        // the control that lands outside the bar: the reference with a hiss 60 dB down mixed in
        var hissed := b.pcm
        var seed = 0x9e3779b9u
        for (i in range(length(hissed))) {
            seed = seed * 1664525u + 1013904223u
            hissed[i] += (float(seed >> 8u) / 16777216.0 - 0.5) * 0.002
        }
        let fh = quiet_floor_db(hissed, b.sample_rate)
        t |> success(fh > fb + 6.0, "a reference with a -60 dBFS hiss reads {fh} dBFS - past the bar the served lane must stay under")
    }
}

// a text the chunker cannot split - no sentence mark, no comma - is one chunk past the token
// budget: the voice's caches grow to hold its text and every frame the cap allows, and the
// synthesis runs to EOS instead of panicking on the cache
[test]
def test_pocket_long_chunk(t : T?) {
    let path = gguf_path()
    if (!model_available(t, path)) {
        return
    }
    with_job_que() {
        setup_dasllama_jobque_()
        var inscope m <- load_tts_model(path)
        let text = build_string() $(w) {
            for (i in range(230)) {
                w |> write(i == 0 ? "word" : " word")
            }
        }
        var inscope chunks <- tts_chunks(m, text)
        t |> equal(length(chunks), 1, "an unsplittable text is one chunk")
        var inscope ids <- pocket_text_ids(m.pocket, chunks[0])
        t |> success(length(ids) > POCKET_MAX_TOKENS, "{length(ids)} tokens: past the budget the caches were sized for")
        static_if (typeinfo builtin_module_exists(das_metal)) {
            let knob_found = metal_tower_knob()
            defer() {
                set_metal_tower(knob_found)
            }
            set_metal_tower(true)
            let d0 = metal_tower_declines()
            var inscope a <- synthesize(m, text, "alba")
            t |> success(speech_stats(a.pcm).finite && length(a.pcm) > 24000, "the long chunk speaks ({length(a.pcm)} samples)")
            let d1 = metal_tower_declines()
            t |> success((d1?["shape"] ?? 0l) - (d0?["shape"] ?? 0l) >= 1l, "the chunk past the codec seat's frame cap declines by shape, and the CPU codec speaks it")
        } else {
            var inscope a <- synthesize(m, text, "alba")
            t |> success(speech_stats(a.pcm).finite && length(a.pcm) > 24000, "the long chunk speaks ({length(a.pcm)} samples)")
        }
        var grown = 0l
        var first = 0l
        var need = 0l
        m.pocket.voice_states |> get("alba") $(vs) {
            grown = vs.caches[0].cap
            first = vs.len + int64(POCKET_MAX_TOKENS) + pocket_max_frames(int64(POCKET_MAX_TOKENS)) + 1l
            need = vs.len + long_length(ids) + pocket_max_frames(long_length(ids)) + 1l
        }
        t |> success(grown > first && grown >= need, "the voice's caches grew past the budget's rows to the chunk's ({grown} positions, sized {first}, the chunk needs {need})")
        // the next short sentence on the same voice still starts from the voice alone
        var inscope b <- synthesize(m, "And a short one after it.", "alba")
        t |> success(speech_stats(b.pcm).rms > 0.01, "a short sentence after the long one speaks")
    }
}

[test]
def test_pocket_prepared_prompt_transfer(t : T?) {
    let sender_path = path_join(models_dir(), "pocket-tts-en-kq.gguf")
    let receiver_path = path_join(models_dir(), "pocket-tts-en-stuart-kq.gguf")
    if (!model_available(t, sender_path) || !model_available(t, receiver_path)) { return }
    with_job_que() {
        setup_dasllama_jobque_()
        var serialized = ""
        {
            var inscope sender <- load_tts_model(sender_path)
            var inscope clip <- load_audio_mono("{get_das_root()}/modules/dasLLAMA/models/jfk_ask_not.wav", 24000)
            t |> success(length(clip) >= 24000, "reference decoded")
            resize(clip, 24000)
            var inscope prompt <- tts_prepare_voice_prompt(sender, clip, 24000)
            t |> success(tts_voice_prompt_valid(sender, prompt))
            t |> success(!empty(prompt.latents), "encoder produced frames")
            serialized = sprint_json(prompt, false)
        }
        var inscope receiver <- load_tts_model(receiver_path)
        t |> success(!receiver.pocket.cloning, "receiving file has no encoder")
        var inscope restored = TtsVoicePrompt()
        t |> success(sscan_json(serialized, restored))
        t |> success(tts_register_voice_prompt(receiver, "prepared", restored))
        t |> success(!key_exists(receiver.pocket.voice_states, "prepared"), "conditioning is lazy")
        var inscope audio <- synthesize(receiver, "Hello from a prepared voice.", "prepared")
        let stats = speech_stats(audio.pcm)
        t |> success(stats.finite && stats.rms > .001, "restored voice produces finite audible speech")
        t |> success(length(audio.pcm) >= 2400, "nonempty speech")
        t |> success(key_exists(receiver.pocket.voice_states, "prepared"), "synthesis built conditioning")
    }
}
