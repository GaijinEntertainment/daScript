options gen2
options persistent_heap
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama_tts
require dasllama/dasllama_tts_types
require dasllama/dasllama_audio_io
require dasllama/dasllama_kitten   // kitten_token_ids: the symbol table's drop rule, model-free
require dasllama/dasllama_styletts2   // the voices table: a hand-built model whose pack names no language; the source's stream
require dasllama/dasllama_tts_blocks   // TtsRng, TtsLinear, SineSourceCfg: the source's noise stream held to its own layout
require dasllama/dasllama_unicode   // the chunker counts codepoints, so the cells do too
require dasllama/dasllama_math   // setup_dasllama_jobque_ + the batch lane cap
require daslib/jobque_boost
require daslib/defer
require daslib/rtti   // this_context().last_exception: the voice refusals are read as text
require daslib/fio
require strings
require daslib/strings_boost
require math
require _model_tier
require _tts_corpus   // the 200-sentence front-end fixture: the phonemes arm E was scored against

def private cpt_count(s : string) : int {
    var inscope c <- utf8_to_cpts(s)
    return length(c)
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

// The TTS facade above the families. Model-free: the sentence chunker (the reference driver's
// boundary rule - abbreviations, decimals, a.m./p.m. - the 400-CODEPOINT cap, the hard split of
// a whitespace-free run, the appended punctuation), the normalizer the facade exposes
// (`tts_normalize`), the WAV writer's container (header fields, sample round trip), the timings'
// own rtf guard, the kitten symbol table's drop rule, the chunk cap knob on the model's own
// chunker (`tts_set_chunk_chars`, its floor refused by name), the release keeping a captured
// carrier, and the harmonic source's noise stream on a hand-built decoder: the drawn stream
// speaks the captured stream's samples, another seed another stream, the window knob reaching
// the carry (the whole run, five frames) with the same samples, a negative window refused.
// Model-gated (`kitten-nano.gguf` + the front-end packs under models_dir()): the facade's
// phonemes (`tts_phonemize`) against the corpus fixture's own inventory, the streaming
// form's chunks concatenate to the buffered form's audio with summed timings, the two
// syntheses run at different worker counts, and the idle release (`tts_release_scratch`): the
// carrier's generator rows, waveform and noise gone, the next synthesis sample-identical, and a
// lowered cap speaking the same sentences in more pieces.

[test]
def test_chunker(t : T?) {
    t |> run("sentences split on terminal punctuation followed by space") @(t : T?) {
        var inscope c <- tts_chunks("First sentence. Second one! Third? Fourth")
        t |> equal(length(c), 4)
        t |> equal(c[0], "First sentence.")
        t |> equal(c[1], "Second one!")
        t |> equal(c[2], "Third?")
        t |> equal(c[3], "Fourth,", "a chunk left without punctuation gains a comma")
    }
}

[test]
def test_chunker_family_rule(t : T?) {
    t |> run("the appended comma is Kitten's driver rule; off, the text goes through as it is") @(t : T?) {
        var inscope bare <- tts_chunks("hello how are you", TTS_CHUNK_CHARS, false)
        t |> equal(length(bare), 1)
        t |> equal(bare[0], "hello how are you", "Kokoro's pipeline adds no mark - its voices breathe one out loud")
        var inscope kept <- tts_chunks("First one. Second", TTS_CHUNK_CHARS, false)
        t |> equal(length(kept), 2)
        t |> equal(kept[0], "First one.", "punctuation the text carries stays")
        t |> equal(kept[1], "Second")
        var words = ""
        for (i in range(40)) {
            words = i == 0 ? "word" : "{words} word"
        }
        var inscope pieces <- tts_chunks(words, 50, false)
        t |> success(length(pieces) >= 3, "{length(pieces)} pieces")
        for (p in pieces) {
            t |> success(!ends_with(p, ","), "a word-split piece stays bare too: '{p}'")
        }
    }
}

[test]
def test_chunker_rules(t : T?) {
    t |> run("abbreviations, decimals and clock times do not end a sentence") @(t : T?) {
        var inscope c <- tts_chunks("Dr. Smith paid 3.50 dollars at 9 a.m. today. Then he left.")
        t |> equal(length(c), 2)
        t |> equal(c[0], "Dr. Smith paid 3.50 dollars at 9 a.m. today.")
        t |> equal(c[1], "Then he left.")
        var inscope d <- tts_chunks("We met at 9 p.m. Later we ate.")
        t |> equal(length(d), 2, "a capital after p.m. ends the sentence")
        var inscope e <- tts_chunks("In Jan. we left. Fig. 3 shows it.")
        t |> equal(length(e), 2)
    }
    t |> run("a long sentence splits on words under the cap") @(t : T?) {
        var words = ""
        for (i in range(120)) {
            words = i == 0 ? "word" : "{words} word"
        }
        var inscope c <- tts_chunks(words, 100)
        t |> success(length(c) >= 6, "{length(c)} chunks")
        for (ch in c) {
            t |> success(length(ch) <= 101, "chunk of {length(ch)} characters (cap 100 plus the comma)")
        }
    }
    t |> run("empty and whitespace text yield no chunks") @(t : T?) {
        t |> equal(length(tts_chunks("")), 0)
        t |> equal(length(tts_chunks("   ")), 0)
    }
    t |> run("the cap counts codepoints, not bytes") @(t : T?) {
        // thirty em dashes: 59 codepoints under the cap of 100, 119 bytes over it
        var inscope dashes : array<string>
        dashes |> reserve(30)
        for (_i in range(30)) {
            dashes |> push("—")   // nolint:STYLE039
        }
        let em = join(dashes, " ")
        t |> equal(cpt_count(em), 59)
        t |> equal(length(em), 119, "bytes, which is what a byte cap would have split on")
        var inscope c <- tts_chunks(em, 100)
        t |> equal(length(c), 1, "one chunk: 59 codepoints fit")
        var inscope quotes : array<string>
        quotes |> reserve(30)
        for (_i in range(30)) {
            quotes |> push("“ok”")   // nolint:STYLE039
        }
        let curly = join(quotes, " ")
        t |> equal(cpt_count(curly), 149)
        t |> success(length(curly) > 149, "{length(curly)} bytes")
        var inscope q <- tts_chunks(curly, 200)
        t |> equal(length(q), 1, "one chunk: 149 codepoints fit under 200")
    }
    t |> run("a whitespace-free run hard-splits at the cap") @(t : T?) {
        let bare = build_string() $(var w) {
            for (i in range(513)) {
                w |> write(cpt_to_utf8('a' + i % 26))
            }
        }
        t |> equal(cpt_count(bare), 513)
        var inscope c <- tts_chunks(bare, 100)
        t |> equal(length(c), 6, "five full pieces and a thirteen-character tail")
        let rejoined = build_string() $(var w) {
            for (ch in c) {
                t |> success(cpt_count(ch) <= 101, "chunk of {cpt_count(ch)} codepoints (cap 100 plus the appended comma)")
                t |> success(ends_with(ch, ","), "each piece gains the punctuation the split left off")
                w |> write(slice(ch, 0, length(ch) - 1))
            }
        }
        t |> equal(rejoined, bare, "the pieces are the run, in order, nothing dropped")
        var inscope d <- tts_chunks(bare, 513)
        t |> equal(length(d), 1, "a run exactly at the cap does not split")
    }
}

// A pack whose name carries no language of the pipeline's shape: the refusal has no code to name,
// and must say so in words rather than render a blank. Hand-built - every stocked pack is shaped.
[test]
def test_voice_refusal_without_a_language(t : T?) {
    t |> run("a voice whose pack names no language refuses in words, not in an empty code") @(t : T?) {
        var inscope m = TtsModel()
        m.kind = TtsKind.kokoro
        var inscope pack : array<float>
        m.model.voices["emma"] <- pack   // the model HAS the voice; what it does not have is a language for it
        let why = panic_text_of() {
            feint("{tts_voice_lang(m, "emma")}")
        }
        t |> success(find(why, "unknown voice 'emma'") >= 0, "the refusal names the voice: {why}")
        t |> success(find(why, "speaks a language the front end does not phonemize") >= 0,
            "and says what it cannot do with it: {why}")
        t |> success(find(why, "speaks , which") < 0, "no empty language code rendered into the sentence: {why}")
    }
}

[test]
def test_facade_normalizer(t : T?) {
    t |> run("the facade reaches the normalizer every synthesis runs first") @(t : T?) {
        t |> equal(tts_normalize("Dr. Chen read 3.5 pages and paid $12."),
            "Doctor Chen read three point five pages and paid twelve dollars.",
            "an abbreviation, a decimal and an amount of money")
        t |> equal(tts_normalize("Prof. Lee owes $1.50."), "Professor Lee owes one dollar and fifty cents.")
        t |> equal(tts_normalize("It started at 9 a.m. on Feb. 3, 2021."),
            "It started at nine a.m. on February third, twenty twenty-one.", "a clock time and a date")
        t |> equal(tts_normalize("The 1st of May, 75% off, 3 km away."),
            "The first of May, seventy-five percent off, three kilometers away.",
            "an ordinal, a percentage and a unit")
        t |> equal(tts_normalize(""), "", "empty text stays empty")
    }
}

[test]
def test_kitten_symbol_drop(t : T?) {
    t |> run("an unknown phoneme takes its separator space with it") @(t : T?) {
        // 'a' and 'b' are in the table, '\x01' is in no table: the ids around it must read as
        // if it had never been there
        var inscope known <- kitten_token_ids("ab ab")
        var inscope withOov <- kitten_token_ids("a{cpt_to_utf8(1)}b ab")
        t |> equal(length(withOov), length(known), "the dropped symbol leaves no doubled space")
        var same = true
        for (x, y in known, withOov) {
            same = same && x == y
        }
        t |> success(same, "id for id with the known string")
    }
}

[test]
def test_timings_guard(t : T?) {
    t |> run("rtf answers zero rather than dividing by no audio") @(t : T?) {
        var timings = TtsTimings()
        timings.total_us = 1000l
        t |> equal(rtf(timings, 24000), 0.0lf, "no samples yet")
        timings.samples = -1l
        t |> equal(rtf(timings, 24000), 0.0lf, "a negative count is no audio either")
        timings.samples = 24000l
        t |> equal(rtf(timings, 24000), 0.001lf, "one second of audio in a millisecond")
    }
}

[test]
def test_wav_writer(t : T?) {
    t |> run("the 16-bit container round-trips") @(t : T?) {
        var inscope pcm <- [for (i in range(2400)); 0.5 * sin(float(i) * 0.1)]
        var inscope bytes <- wav_pcm16_bytes(pcm, 24000)
        t |> equal(length(bytes), 44 + 2400 * 2)
        t |> equal(int(bytes[0]), 'R')
        t |> equal(int(bytes[8]), 'W')
        t |> equal(int(bytes[22]) | (int(bytes[23]) << 8), 1, "mono")
        t |> equal(int(bytes[24]) | (int(bytes[25]) << 8) | (int(bytes[26]) << 16), 24000, "sample rate")
        var terr = ""
        let path = path_join(temp_directory(terr), "dasllama_tts_facade_test.wav")
        t |> success(write_wav_pcm16(path, pcm, 24000), "write")
        var inscope back <- read_wav_pcm16_mono(path)
        t |> equal(length(back), 2400)
        var worst = 0.0
        for (a, b in pcm, back) {
            worst = max(worst, abs(a - b))
        }
        t |> success(worst < 1.0 / 32000.0, "round trip within one quantization step ({worst})")
        var inscope raw <- pcm16_bytes(pcm)
        t |> equal(length(raw), 4800)
        let removed = remove_result(path)
        t |> success(removed is value, "temp file removed")
    }
}

// corpus rows the daslang front end reproduces phoneme for phoneme, so a facade wrapper that
// reached a family's rewrite instead of the front end's own inventory reds here
let private PINNED_CORPUS_ROWS <- {"lj000", "lj002", "lj003"}

[test]
def test_facade_phonemizer(t : T?) {
    t |> run("the facade phonemizes into the front end's own inventory, on the corpus rail") @(t : T?) {
        let gguf = path_join(models_dir(), "kitten-nano.gguf")
        if (!stat(gguf).is_valid || !stat(path_join(models_dir(), "tts_g2p.bin")).is_valid) {
            t |> skip("kitten-nano.gguf or the front-end packs not present")
            return
        }
        with_job_que() {
            setup_dasllama_jobque_()
            var inscope m <- load_tts_model(gguf)
            var inscope rows <- load_tts_corpus()
            var seen = 0
            for (r in rows) {
                continue if (!key_exists(PINNED_CORPUS_ROWS, r.id))
                seen++
                t |> equal(strip(tts_phonemize(m, r.norm)), r.ps_misaki,
                    "{r.id}: the 45-symbol US inventory the corpus pins, never a family's own symbols")
            }
            t |> equal(seen, length(PINNED_CORPUS_ROWS), "every pinned row is in the fixture")
        }
    }
    t |> run("the language form answers for a language the model declares and panics for one it does not") @(t : T?) {
        let gguf = path_join(models_dir(), "kitten-nano.gguf")
        if (!stat(gguf).is_valid || !stat(path_join(models_dir(), "tts_g2p.bin")).is_valid) {
            t |> skip("kitten-nano.gguf or the front-end packs not present")
            return
        }
        with_job_que() {
            setup_dasllama_jobque_()
            var inscope m <- load_tts_model(gguf)
            var inscope c <- caps(m)
            var inscope rows <- load_tts_corpus()
            // Kitten speaks American English alone, so its front end declares one language and
            // refuses the other rather than answering in the wrong dialect
            t |> success(length(c.langs) == 1 && c.langs[0] == "en-us", "{length(c.langs)} language(s), first '{c.langs[0]}'")
            var seen = 0
            for (r in rows) {
                continue if (!key_exists(PINNED_CORPUS_ROWS, r.id))
                seen++
                t |> equal(strip(tts_phonemize(m, r.norm, "en-us")), r.ps_misaki, "{r.id}: the declared language reads the same as the bare form")
            }
            t |> equal(seen, length(PINNED_CORPUS_ROWS), "every pinned row is in the fixture")
            var refused = false
            try {
                t |> failure("phonemized {tts_phonemize(m, "the water tower", "en-gb")} for a language this front end does not declare")
            } recover {
                refused = true
            }
            t |> success(refused, "an undeclared language panics at the call site")
        }
    }
}

// The model-keyed chunker picks the family's driver rule, and Kitten's is the arm no model-free
// cell can reach: the boolean form is pinned above, Kokoro's bare text in test_tts_kokoro.das.
[test]
def test_chunker_model_rule(t : T?) {
    t |> run("a Kitten model's chunks gain the driver's comma") @(t : T?) {
        let gguf = path_join(models_dir(), "kitten-nano.gguf")
        if (!stat(gguf).is_valid || !stat(path_join(models_dir(), "tts_g2p.bin")).is_valid) {
            t |> skip("kitten-nano.gguf or the front-end packs not present")
            return
        }
        with_job_que() {
            setup_dasllama_jobque_()
            var inscope m <- load_tts_model(gguf)
            var inscope bare <- tts_chunks(m, "hello how are you")
            t |> equal(length(bare), 1)
            t |> equal(bare[0], "hello how are you,", "KittenTTS's driver appends the prosodic close")
            var inscope kept <- tts_chunks(m, "First one. Second")
            t |> equal(length(kept), 2)
            t |> equal(kept[0], "First one.", "punctuation the text carries stays as it is")
            t |> equal(kept[1], "Second,")
        }
    }
}

[test, arch(at = "../ARCHITECTURE_TTS.md#tts-two-layouts")]
def test_streaming(t : T?) {
    t |> run("chunks concatenate to the buffered synthesis, at a different worker count") @(t : T?) {
        let gguf = path_join(models_dir(), "kitten-nano.gguf")
        if (!stat(gguf).is_valid || !stat(path_join(models_dir(), "tts_g2p.bin")).is_valid) {
            t |> skip("kitten-nano.gguf or the front-end packs not present")
            return
        }
        with_job_que() {
            setup_dasllama_jobque_()
            let cap0 = get_batch_lane_cap()
            defer() {
                set_batch_lane_cap(cap0)
            }
            var inscope m <- load_tts_model(gguf)
            let text = "The first sentence is short. The second one is a little longer than the first! Is the third a question?"
            var chunks = 0
            var inscope streamed : array<float>
            // one lane: every rows kernel runs its blocks in one sweep, in one order
            set_batch_lane_cap(1)
            synthesize_stream(m, text, "Bella", 1.0) $(chunk) {
                chunks++
                streamed |> push_from(chunk.pcm)
            }
            t |> equal(chunks, 3, "three sentences, three chunks")
            // the box's own lane count: the same blocks split across workers
            set_batch_lane_cap(0)
            var inscope whole <- synthesize(m, text, "Bella")
            t |> equal(length(whole.pcm), length(streamed), "the buffered form is the concatenation")
            var differing = 0
            for (a, b in whole.pcm, streamed) {
                differing += a == b ? 0 : 1
            }
            t |> equal(differing, 0, "{differing} of {length(whole.pcm)} samples move with the worker count")
            t |> success(whole.timings.samples == long_length(whole.pcm), "the summed timings count every sample")
        }
    }
}

[test]
def test_chunk_cap_knob(t : T?) {
    t |> run("a lowered cap splits the same text into shorter pieces, on the model's own chunker") @(t : T?) {
        var inscope m = TtsModel()
        m.kind = TtsKind.kitten
        var words = ""
        for (i in range(60)) {
            words = i == 0 ? "word" : "{words} word"
        }
        var inscope wide <- tts_chunks(m, words)
        t |> equal(length(wide), 1, "sixty short words sit under the default cap")
        tts_set_chunk_chars(m, 30)
        var inscope narrow <- tts_chunks(m, words)
        t |> success(length(narrow) >= 10, "{length(narrow)} pieces at a 30-codepoint cap")
        for (p in narrow) {
            t |> success(cpt_count(p) <= 30, "every piece is under the cap: '{p}'")
        }
        tts_set_chunk_chars(m, TTS_CHUNK_CHARS)
        var inscope back <- tts_chunks(m, words)
        t |> equal(length(back), 1, "the default cap restored")
    }
    t |> run("a cap under one codepoint is refused by name") @(t : T?) {
        var inscope m = TtsModel()
        m.kind = TtsKind.kokoro
        let why = panic_text_of() {
            tts_set_chunk_chars(m, 0)
        }
        t |> success(find(why, "chunk cap") >= 0, "the refusal names the knob: {why}")
    }
    t |> run("the release keeps a carrier whose noise was captured, and empties one whose noise was drawn") @(t : T?) {
        var inscope m = TtsModel()
        m.kind = TtsKind.kitten
        m.scratch.gen_a |> reserve(16)
        m.scratch.gen_a |> resize(16)
        m.scratch.noise.captured = true
        tts_release_scratch(m)
        t |> success(long_capacity(m.scratch.gen_a) >= 16l, "the parity rail's captured noise keeps its carrier")
        m.scratch.noise.captured = false
        tts_release_scratch(m)
        t |> equal(long_capacity(m.scratch.gen_a), 0l, "a drawn carrier goes")
    }
}

// the source's noise is one stream the reference draws in one order - the harmonics' initial
// phases, then the normals row by row - split between styletts2_draw_noise and the source
def private captured_stream(cfg : SineSourceCfg; n : int64; seed : uint64) : TtsNoise {
    var r = TtsRng()
    rng_seed(r, seed)
    var inscope noise = TtsNoise(captured = true)
    noise.uniform |> resize(cfg.n_harm)
    for (h in range64(cfg.n_harm)) {
        noise.uniform[h] = rng_uniform(r)
    }
    noise.normal |> resize(n * cfg.n_harm)
    for (i in range64(n * cfg.n_harm)) {
        noise.normal[i] = rng_normal(r)
    }
    return <- noise
}

def private source_from(d : St2Decoder; cfg : SineSourceCfg; f0 : array<float>; frames : int64; noise : TtsNoise; seed : uint64;
                        var har : array<float>; var sc : St2Scratch) {
    styletts2_source(d, cfg, f0, frames, noise, seed, har, sc)
}

def private samples_off(a, b : array<float>) : int {
    var diffs = length(a) == length(b) ? 0 : 1
    for (x, y in a, b) {
        diffs += x == y ? 0 : 1
    }
    return diffs
}

[test]
def test_source_stream_contract(t : T?) {
    t |> run("the drawn noise is the captured stream's layout: phases first, then the normals in row order") @(t : T?) {
        {   // a bare block: the defer is a per-call cleanup, not the lambda's finalizer
        let win0 = styletts2_source_window()
        defer() {
            set_styletts2_source_window(win0)
        }
        set_styletts2_source_window(SOURCE_WINDOW_FRAMES)
        let cfg = SineSourceCfg()
        let frames = 100l
        let n = frames * cfg.upsample
        var r = TtsRng()
        rng_seed(r, 5ul)
        var inscope d = St2Decoder()
        d.source_linear = TtsLinear(nin = cfg.n_harm, nout = 1l)
        d.source_linear.w |> resize(cfg.n_harm)
        for (h in range64(cfg.n_harm)) {
            d.source_linear.w[h] = rng_uniform(r) - 0.5
        }
        linear_prepare(d.source_linear)
        var inscope f0 : array<float>
        f0 |> resize(frames)
        for (i in range64(frames)) {
            f0[i] = i % 13l == 0l ? 0.0 : 120.0 + 200.0 * rng_uniform(r)
        }
        let seed = 77ul
        var inscope captured <- captured_stream(cfg, n, seed)
        var inscope sc_c = St2Scratch()
        var inscope har_c : array<float>
        source_from(d, cfg, f0, frames, captured, 0ul, har_c, sc_c)
        var inscope drawn = TtsNoise()
        styletts2_draw_noise(cfg, seed, drawn)
        t |> equal(long_length(drawn.normal), 0l, "the phases' draw leaves the normals to the source")
        var inscope sc_d = St2Scratch()
        var inscope har_d : array<float>
        source_from(d, cfg, f0, frames, drawn, seed, har_d, sc_d)
        t |> equal(long_length(har_d), n, "{n} samples")
        t |> equal(samples_off(har_c, har_d), 0, "the drawn stream speaks the captured stream's samples")
        t |> equal(sc_d.so_carry.stride, SOURCE_WINDOW_FRAMES + 2l, "the default window plus its two halo frames is what the carry held")
        // the control: another seed is another stream
        var inscope other = TtsNoise()
        styletts2_draw_noise(cfg, seed + 1ul, other)
        var inscope sc_o = St2Scratch()
        var inscope har_o : array<float>
        source_from(d, cfg, f0, frames, other, seed + 1ul, har_o, sc_o)
        t |> success(int64(samples_off(har_c, har_o)) > n / 2l, "another seed moves most samples")
        // the knob reaches the run: the whole run in one window, then five frames a window, each the same samples
        set_styletts2_source_window(0l)
        var inscope har_w : array<float>
        source_from(d, cfg, f0, frames, drawn, seed, har_w, sc_d)
        t |> equal(sc_d.so_carry.stride, frames + 2l, "window 0 ran the whole run in one window")
        t |> equal(samples_off(har_c, har_w), 0, "and speaks the same samples")
        set_styletts2_source_window(5l)
        source_from(d, cfg, f0, frames, drawn, seed, har_w, sc_d)
        t |> equal(sc_d.so_carry.stride, 7l, "a five-frame window ran five frames a step")
        t |> equal(samples_off(har_c, har_w), 0, "and speaks the same samples")
        }
    }
    t |> run("a negative window is refused by name") @(t : T?) {
        let why = panic_text_of() {
            set_styletts2_source_window(-1l)
        }
        t |> success(find(why, "set_styletts2_source_window") >= 0, "the refusal names the knob: {why}")
        t |> equal(styletts2_source_window(), SOURCE_WINDOW_FRAMES, "and the window stays where it was")
    }
}

[test]
def test_release_and_cap_on_a_model(t : T?) {
    t |> run("the release empties the carrier and the next synthesis speaks the same samples; a cap makes more chunks") @(t : T?) {
        let gguf = path_join(models_dir(), "kitten-nano.gguf")
        return if (!model_available(t, gguf))
        if (!stat(path_join(models_dir(), "tts_g2p.bin")).is_valid) {
            t |> skip("the front-end packs not present")
            return
        }
        with_job_que() {
            setup_dasllama_jobque_()
            // the source window is process state: the cell establishes the default it claims against and puts back what it found
            let win0 = styletts2_source_window()
            defer() {
                set_styletts2_source_window(win0)
            }
            set_styletts2_source_window(SOURCE_WINDOW_FRAMES)
            var inscope m <- load_tts_model(gguf)
            let text = "The first sentence is short. The second one is a little longer than the first! Is the third a question?"
            var inscope default_chunks <- tts_chunks(m, tts_normalize(text))
            t |> equal(length(default_chunks), 3, "three sentences under the default cap")
            var inscope first <- synthesize(m, text, "Bella")
            t |> success(long_capacity(m.scratch.gen_a) > 0l, "a synthesis leaves the generator's rows in the carrier")
            t |> success(long_capacity(m.scratch.noise.uniform) > 0l, "and the initial phases it drew")
            t |> equal(long_capacity(m.scratch.noise.normal), 0l, "the per-sample noise is drawn a window at a time, never held")
            // the source's window is a memory knob, not a numeric one: the whole run in one window speaks the same samples
            set_styletts2_source_window(0l)
            var inscope whole <- synthesize(m, text, "Bella")
            t |> equal(length(whole.pcm), length(first.pcm), "the whole-run source speaks the same length")
            var window_diff = 0
            for (a, b in first.pcm, whole.pcm) {
                window_diff += a == b ? 0 : 1
            }
            t |> equal(window_diff, 0, "{window_diff} of {length(first.pcm)} samples move with the source window")
            set_styletts2_source_window(SOURCE_WINDOW_FRAMES)
            tts_release_scratch(m)
            t |> equal(long_capacity(m.scratch.gen_a), 0l, "the release gives the generator's rows back")
            t |> equal(long_capacity(m.scratch.wave), 0l, "and the waveform")
            t |> equal(long_capacity(m.scratch.noise.uniform), 0l, "and the phases - a synthesis redraws them from the seed")
            var inscope again <- synthesize(m, text, "Bella")
            t |> equal(length(again.pcm), length(first.pcm), "the next synthesis speaks the same length")
            var differing = 0
            for (a, b in first.pcm, again.pcm) {
                differing += a == b ? 0 : 1
            }
            t |> equal(differing, 0, "{differing} of {length(first.pcm)} samples move across the release")
            // a memory-tight cap: the same sentences in more pieces, each its own synthesis
            tts_set_chunk_chars(m, 24)
            var chunks = 0
            var inscope capped : array<float>
            synthesize_stream(m, text, "Bella", 1.0) $(chunk) {
                chunks++
                capped |> push_from(chunk.pcm)
            }
            t |> success(chunks > 3, "{chunks} chunks at a 24-codepoint cap, more than the three sentences")
            let peak = logits_maxabs(capped)
            t |> success(peak > 0.05 && peak <= 1.0, "the capped synthesis is audible speech (peak {peak})")
            var inscope silent : array<float>
            silent |> resize(length(capped))
            t |> success(!(logits_maxabs(silent) > 0.05), "the peak floor's control: a silent buffer of the same length lands under it")
            let ratio = float(length(capped)) / float(length(first.pcm))
            t |> success(ratio > 0.7 && ratio < 1.5, "and about as long as the uncapped one ({ratio})")
            let half = ratio * 0.5
            t |> success(!(half > 0.7 && half < 1.5), "the length band's control: half the capped stream lands outside it ({half})")
        }
    }
}
