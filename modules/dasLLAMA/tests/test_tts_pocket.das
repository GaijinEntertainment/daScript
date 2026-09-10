options gen2
options persistent_heap
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama_spm
require dasllama/dasllama_gguf
require dasllama/dasllama_pocket
require dasllama/dasllama_tts
require dasllama/dasllama_tts_types
require dasllama/dasllama_tts_blocks
require dasllama/dasllama_math   // setup_dasllama_jobque_
require daslib/jobque_boost
require daslib/defer   // defer() { reset_pocket_q8() } - a lane pin never outlives its cell
require daslib/fio
require daslib/json_boost
require daslib/rtti   // this_context().last_exception: the refusals are read as text
require strings
require math
require _tts_oracle
require _tts_corpus   // the corpus sentences the oracle prepared: the prepare-parity cell's inputs
require _model_tier

// The Pocket TTS family. Model-gated (`pocket-tts-en.gguf` under models_dir(), minted by
// harness/convert_pocket.py; the oracle dumps under `tts_oracle/pocket_english_2026-04/` by
// harness/pocket_oracle.py): the unigram tokenizer id-for-id against the reference package on
// the 200-sentence corpus and on the byte-fallback probes (`_tts_fixtures/pocket_tokens.json`,
// written by the oracle script's --tokens-fixture), the decode round trip, then the parity rail.

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

def private same_ids(a, b : array<int64>) : bool {
    return false if (length(a) != length(b))
    for (x, y in a, b) {
        return false if (x != y)
    }
    return true
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
        if (!same_ids(ours, r.ids)) {
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
        t |> success(same_ids(ours, p.ids), "'{p.text}': ours [{ids_text(ours)}] reference [{ids_text(p.ids)}]")
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

def private load_manifest() : PocketManifest {
    var m = PocketManifest()
    let text = fread(path_join(oracle_dir(), "manifest.json"))
    return <- m if (empty(text))
    if (!sscan_json(text, m)) {
        panic("pocket oracle manifest does not parse")
    }
    return <- m
}

// the largest |ours - oracle| relative to the oracle's peak, logged on green and red alike
def private rel_diff(t : T?; tag : string; ours, ref : array<float>; bar : float) {
    if (length(ours) != length(ref)) {
        t |> failure("{tag}: {length(ours)} elements against the oracle's {length(ref)}")
        return
    }
    var peak = 0.0
    for (v in ref) {
        peak = max(peak, abs(v))
    }
    let d = max_abs_diff(ours, ref)
    to_log(LOG_INFO, "pocket {tag}: maxdiff {d} over {length(ref)} (oracle peak {peak})\n")
    t |> success(d <= bar * peak, "{tag}: maxdiff {d} <= {bar * peak}")
    // the control: one expected element moved by twice the bar must land outside it
    if (!empty(ref)) {
        var inscope poisoned := ref
        poisoned[0] += 2.0 * bar * peak
        t |> success(max_abs_diff(ours, poisoned) > bar * peak, "{tag}: the bar discriminates a poisoned expectation")
    }
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
            rel_diff(t, "{tag} codec decode", wave, dump["waveform"].f, CODEC_BAR)
            // the codec encoder and the voice prompt, once per voice
            if (voiced < 2) {
                var inscope lat : array<float>
                let frames = pocket_encode_latents(m, dump["voice_pcm"].f, sc, lat)
                t |> equal(int(frames), c.voice_frames, "{tag}: the clip's latent frame count")
                rel_diff(t, "{tag} codec encode", lat, dump["voice_latents"].f, ENCODER_BAR)
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
                    rel_diff(t, "{tag} voice kv layer {l}", ours, dump[name].f, FRAME_BAR)
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
        var inscope m <- load_pocket(path)
        var inscope sc = PocketScratch()
        var done = 0
        for (c in man.cases) {
            continue if (!c.stages || c.voice != "alba")
            var inscope dump <- load_tts_dump(path_join(oracle_dir(), c.file))
            let tag = "{c.id} {c.voice}"
            var inscope ids <- pocket_text_ids(m, c.prepared)
            t |> equal(length(ids), c.n_tokens, "{tag}: token count")
            var inscope vs <- pocket_voice_state(m, dump["voice_pcm"].f, sc)
            var tm = TtsTimings()
            var inscope x0 := dump["x0"].f
            var inscope x1 := dump["x1"].f
            // teacher-forced: the oracle's noise and the oracle's frames fed back, so every frame
            // is one backbone step and one head step away from the reference
            rng_seed(sc.rng, uint64(c.seed))
            let frames = pocket_synthesize(m, ids, vs, man.temp, c.n_frames - c.eos_frame, x0, x1, tm, sc)
            t |> equal(int(frames), c.n_frames, "{tag}: frame count under teacher forcing")
            var inscope lat : array<float>
            lat |> resize(frames * m.latent_dim)
            for (i in range64(frames * m.latent_dim)) {
                lat[i] = sc.latents[i]
            }
            var inscope conds : array<float>
            conds |> resize(frames * m.backbone.d)
            for (i in range64(frames * m.backbone.d)) {
                conds[i] = sc.conds[i]
            }
            var inscope eos : array<float>
            eos |> resize(frames)
            for (i in range64(frames)) {
                eos[i] = sc.eos_logits[i]
            }
            // the head alone, fed the oracle's own conditioning and noise: the seam between the two
            var inscope head_x1 <- head_over_frames(m, sc, dump["cond"].f, x0, frames)
            rel_diff(t, "{tag} flow head on the oracle's conditioning", head_x1, x1, FRAME_BAR)
            rel_diff(t, "{tag} backbone conditioning (teacher-forced)", conds, dump["cond"].f, FRAME_BAR)
            rel_diff(t, "{tag} eos logits (teacher-forced)", eos, dump["eos_logit"].f, FRAME_BAR)
            rel_diff(t, "{tag} latents (teacher-forced)", lat, dump["latent"].f, FRAME_BAR)
            rel_diff(t, "{tag} waveform (teacher-forced)", sc.wave, dump["waveform"].f, CODEC_BAR * 4.0)
            to_log(LOG_INFO, "pocket {tag}: {timings_line(tm, m.sample_rate)}\n")
            // free-running on the oracle's noise: the frames drift as the errors feed back; logged
            var inscope none : array<float>
            let free = pocket_synthesize(m, ids, vs, man.temp, c.n_frames - c.eos_frame, x0, none, tm, sc)
            to_log(LOG_INFO, "pocket {tag}: free run {free} frames against the oracle's {c.n_frames}, {timings_line(tm, m.sample_rate)}\n")
            done++
            break if (done >= 2)
        }
        t |> success(done > 0, "at least one stage case ran")
    }
}

// the q8 lane against the f32 oracle: a frame's latent is a flow sample, and a quantized
// conditioning row moves a few of its 32 elements by whole tenths, so the per-element figures are
// logged and the lane's quality gate is the rig (harness/tts_rig.py --q8 against --f32)
def private rms_relative(ours, ref : array<float>) : double {
    var ssd = 0.0lf
    var ssr = 0.0lf
    for (a, b in ours, ref) {
        ssd += double((a - b) * (a - b))
        ssr += double(b * b)
    }
    return sqrt(ssd / max(ssr, 1e-30lf))
}

let Q8_RMS_BAR = 0.5lf   // a quantized conditioning row moves a flow sample by tenths on a few elements

def private q8_report(t : T?; tag : string; ours, ref : array<float>) {
    if (length(ours) != length(ref)) {
        t |> failure("{tag}: {length(ours)} elements against the oracle's {length(ref)}")
        return
    }
    let rel = rms_relative(ours, ref)
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
    t |> success(rms_relative(ours, poisoned) >= Q8_RMS_BAR, "{tag}: the rms bar discriminates a poisoned expectation")
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
        t |> success(m.backbone.layers[0].in_proj.q8 && m.mimi.dec_in.q8 && !m.mimi.enc_in.q8 && !m.mimi.dec_out.q8,
            "the GEMMs on 32-wide inputs minted q8; the one-channel convs stayed f32")
        var inscope sc = PocketScratch()
        var done = 0
        for (c in man.cases) {
            continue if (!c.stages || c.voice != "alba")
            var inscope dump <- load_tts_dump(path_join(oracle_dir(), c.file))
            let tag = "{c.id} {c.voice} q8"
            var inscope ids <- pocket_text_ids(m, c.prepared)
            var inscope vs <- pocket_voice_state(m, dump["voice_pcm"].f, sc)
            var tm = TtsTimings()
            var inscope x0 := dump["x0"].f
            var inscope x1 := dump["x1"].f
            rng_seed(sc.rng, uint64(c.seed))
            let frames = pocket_synthesize(m, ids, vs, man.temp, c.n_frames - c.eos_frame, x0, x1, tm, sc)
            t |> equal(int(frames), c.n_frames, "{tag}: frame count under teacher forcing")
            var inscope lat : array<float>
            lat |> resize(frames * m.latent_dim)
            for (i in range64(frames * m.latent_dim)) {
                lat[i] = sc.latents[i]
            }
            q8_report(t, "{tag} latents (teacher-forced)", lat, dump["latent"].f)
            var inscope none : array<float>
            let free = pocket_synthesize(m, ids, vs, man.temp, c.n_frames - c.eos_frame, x0, none, tm, sc)
            t |> success(abs(int(free) - c.n_frames) <= 4, "{tag}: the free run's {free} frames sit by the oracle's {c.n_frames}")
            t |> success(speech_stats(sc.wave).rms > 0.01, "{tag}: the free run carries speech")
            to_log(LOG_INFO, "pocket {tag}: free run {free} frames against the oracle's {c.n_frames}, {timings_line(tm, m.sample_rate)}\n")
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
    var inscope dump <- load_tts_dump(path_join(odir, c.file))
    var inscope ids <- pocket_text_ids(m, c.prepared)
    var inscope vs <- pocket_voice_state(m, dump["voice_pcm"].f, sc)
    var tm = TtsTimings()
    var inscope x0 := dump["x0"].f
    var inscope x1 := dump["x1"].f
    rng_seed(sc.rng, uint64(c.seed))
    let frames = pocket_synthesize(m, ids, vs, man.temp, c.n_frames - c.eos_frame, x0, x1, tm, sc)
    var inscope lat : array<float>
    lat |> resize(frames * m.latent_dim)
    for (i in range64(frames * m.latent_dim)) {
        lat[i] = sc.latents[i]
    }
    return <- lat
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
        t |> success(mq.mimi.dec_in.q8 && mq.mimi.enc_out.q8 && mq.mimi.dec_res[0].conv1.q8 && !mq.mimi.enc_in.q8 && !mq.mimi.dec_out.q8,
            "the codec's 32-wide convs came in as Q8_0 slabs, the one-channel ends as f16")
        t |> equal(mq.mimi.dec_in.q8_k, mq.mimi.dec_in.k * mq.mimi.dec_in.cin, "the slab's row is kernel times input channels")
        for (c in man.cases) {
            continue if (!c.stages || c.voice != "alba")
            var inscope a <- forced_latents(mq, c, man, sc)
            var inscope b <- forced_latents(mf, c, man, sc)
            var ssd = 0.0lf
            var ssr = 0.0lf
            for (x, y in a, b) {
                ssd += double((x - y) * (x - y))
                ssr += double(y * y)
            }
            let rel = sqrt(ssd / max(ssr, 1e-30lf))
            to_log(LOG_INFO, "pocket {c.id} q8 file vs q8 lane: rms relative {rel}\n")
            // the file's block scales are f16 (the Q8_0 format), the load-time quantizer keeps
            // them f32, and the flow head amplifies that half-a-thousandth per block into
            // percents of a frame; the published file's own rig row is its quality gate
            t |> success(rel < Q8_FILE_BAR, "{c.id}: the file's blocks and the load-time quants are the same lane (rms relative {rel})")
            var inscope poisoned := b
            for (v in poisoned) {
                v += 2.0
            }
            t |> success(rms_relative(a, poisoned) >= Q8_FILE_BAR, "{c.id}: the bar discriminates a poisoned expectation")
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
            let rel = rms_relative(a, b)
            to_log(LOG_INFO, "pocket {c.id} q8 file on the f32 lane vs the f16 file: rms relative {rel}\n")
            t |> success(rel < Q8_FILE_BAR, "{c.id}: the dequantized file serves the f16 file's frames (rms relative {rel})")
            break
        }
    }
}

let Q8_FILE_BAR = 5.0e-2lf
let KQ_EXACT_BAR = 5.0e-2lf   //! the kq lane against the exact lane of the same file (measured 3.4e-2): the head's Q8_0 route and the activation quants
let KQ_HEAD_BAR = 2.0e-2lf    //! the flow head on its Q8_0 route against the f32 oracle's frames (measured 9.8e-3), the oracle's own conditioning and noise in

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
def private kq_lanes_case(t : T?; c : PocketCase; man : PocketManifest; var mk, mq, m32, mf : PocketModel; var sc : PocketScratch) {
    var inscope a <- forced_latents(mk, c, man, sc)
    var inscope b <- forced_latents(mq, c, man, sc)
    let rel = rms_relative(a, b)
    to_log(LOG_INFO, "pocket {c.id} kq lane vs the q8 lane of the same file: rms relative {rel}\n")
    t |> success(rel < Q8_FILE_BAR, "{c.id}: the kq lane and the q8 lane of one file agree (rms relative {rel})")
    var inscope poisoned := b
    for (v in poisoned) {
        v += 2.0
    }
    t |> success(rms_relative(a, poisoned) >= Q8_FILE_BAR, "{c.id}: the bar discriminates a poisoned expectation")
    var inscope e <- forced_latents(m32, c, man, sc)
    let rel32 = rms_relative(a, e)
    to_log(LOG_INFO, "pocket {c.id} kq lane vs the exact lane of the same file: rms relative {rel32} (the head's Q8_0 route and the activation quants)\n")
    t |> success(rel32 < KQ_EXACT_BAR, "{c.id}: the kq lane and the exact lane of one file agree (rms relative {rel32})")
    var inscope f <- forced_latents(mf, c, man, sc)
    to_log(LOG_INFO, "pocket {c.id} kq lane vs the f16 reference: rms relative {rms_relative(a, f)} (the format's loss; the rig is its gate)\n")
    var inscope dump <- load_tts_dump(path_join(oracle_dir(), c.file))
    // the head alone on its Q8_0 route - Q8_0 blocks and a Q8_0-quantized activation vector per
    // GEMV - fed the oracle's own conditioning and noise, against the oracle's frames
    var inscope x0 := dump["x0"].f
    var inscope hk <- head_over_frames(mk, sc, dump["cond"].f, x0, int64(c.n_frames))
    let rel_head = rms_relative(hk, dump["x1"].f)
    to_log(LOG_INFO, "pocket {c.id} the flow head on Q8_0 blocks vs the oracle: rms relative {rel_head}\n")
    t |> success(rel_head < KQ_HEAD_BAR, "{c.id}: the head's Q8_0 route holds the oracle's frames (rms relative {rel_head})")
    var inscope head_poisoned := dump["x1"].f
    for (v in head_poisoned) {
        v += 2.0
    }
    t |> success(rms_relative(hk, head_poisoned) >= KQ_HEAD_BAR, "{c.id}: the head bar discriminates a poisoned expectation")
    var inscope lat : array<float>
    let frames = pocket_encode_latents(mf, dump["voice_pcm"].f, sc, lat)   // the f16 file's encoder, f32 weights: the parity rail's own; the kq file's carries Q4_K and Q8_0
    var inscope stored : array<float>
    mk.voice_latents |> get("alba") $(s) {
        stored := s
    }
    t |> equal(long_length(stored), frames * mk.latent_dim, "alba's stored frames are the clip's frame count")
    rel_diff(t, "{c.id} stored roster vs the reference encoder", stored, lat, ENCODER_BAR)
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

def private load_lang_manifest(dir_path : string) : PocketManifest {
    var m = PocketManifest()
    let text = fread(path_join(dir_path, "manifest.json"))
    return <- m if (empty(text))
    if (!sscan_json(text, m)) {
        panic("pocket oracle manifest does not parse: {dir_path}")
    }
    return <- m
}

// one language's cells: the tokenizer on its own sentences and the probes, the teacher-forced
// frames of every stage case on the published file, one sentence through the facade
def private language_cells(t : T?; L : PocketLang) {
    let path = path_join(models_dir(), "pocket-tts-{L.code}-q8.gguf")
    let odir = path_join(models_dir(), "tts_oracle/pocket_{L.config}")
    if (!model_available(t, path)) {
        return
    }
    var inscope man <- load_lang_manifest(odir)
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
        if (!same_ids(ours, r.ids)) {
            bad++
            to_log(LOG_ERROR, "pocket {L.config} tokens {r.id} '{r.prepared}': ours [{ids_text(ours)}] '{spm_decode_ids(m.tok, ours)}' reference [{ids_text(r.ids)}]\n")
        }
    }
    for (p in fx.probes) {
        var inscope ours <- pocket_text_ids(m, p.text)
        if (!same_ids(ours, p.ids)) {
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
        // a cloned voice: the roster's own clip under a new name speaks and is listed
        var inscope clip : array<float>
        m.pocket.voices |> get("caro_davy") $(pcm) {
            clip := pcm
        }
        t |> success(!empty(clip), "the parity file stores its roster as clips (the older form) - caro_davy's is the clone's source")
        tts_register_voice(m, "cloned", clip, 24000)
        var inscope c2 <- caps(m)
        t |> equal(length(c2.voices), 20, "the cloned voice joined the roster")
        var inscope b <- synthesize(m, "Hello from a cloned voice.", "cloned")
        t |> success(speech_stats(b.pcm).rms > 0.01, "the cloned voice speaks")
        pocket_refusals(t, m, clip)
    }
}

def private panic_text_of(blk : block<() : void>) : string {
    var why = ""
    try {
        invoke(blk)
    } recover {
        why = this_context().last_exception |> rtrim
    }
    return why
}

// an unknown voice, a speed, a phoneme request and a clip at the wrong rate are each refused by name
def private pocket_refusals(t : T?; var m : TtsModel; clip : array<float>) {
    var why = panic_text_of() {
        var inscope none <- synthesize(m, "x", "nobody")
    }
    t |> success(why |> find("unknown voice") >= 0, "an unknown voice is refused: {why}")
    why = panic_text_of() {
        var inscope none <- synthesize(m, "x", "alba", 1.5)
    }
    t |> success(why |> find("speed") >= 0, "a speed is refused: {why}")
    why = panic_text_of() {
        let ps = tts_phonemize(m, "x")
    }
    t |> success(why |> find("phonemes") >= 0, "phonemizing is refused: {why}")
    why = panic_text_of() {
        tts_register_voice(m, "wrong", clip, 16000)
    }
    t |> success(why |> find("16000") >= 0, "a clip at another rate is refused: {why}")
    var inscope long_clip : array<float>
    long_clip |> resize((POCKET_MAX_VOICE_SECONDS + 1l) * 24000l)
    why = panic_text_of() {
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
        let why = panic_text_of() {
            tts_register_voice(m, "x", clip, 24000)
        }
        t |> success(why |> find("no codec encoder") >= 0, "a clip is refused by the missing encoder: {why}")
        var inscope c2 <- caps(m)
        t |> equal(length(c2.voices), 1, "and the roster is unchanged")
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
        t |> success(length(ids) >= 194, "{length(ids)} tokens: past the 1024-row slack with its frame cap")
        var inscope a <- synthesize(m, text, "alba")
        t |> success(speech_stats(a.pcm).finite && length(a.pcm) > 24000, "the long chunk speaks ({length(a.pcm)} samples)")
        var grown = 0l
        m.pocket.voice_states |> get("alba") $(vs) {
            grown = vs.caches[0].cap
        }
        t |> success(grown > 63l + 1024l + 1024l, "the voice's caches grew past the slack ({grown} positions)")
        // the next short sentence on the same voice still starts from the voice alone
        var inscope b <- synthesize(m, "And a short one after it.", "alba")
        t |> success(speech_stats(b.pcm).rms > 0.01, "a short sentence after the long one speaks")
    }
}
