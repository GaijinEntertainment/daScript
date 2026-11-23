options gen2
options persistent_heap
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama_tts
require dasllama/dasllama_tts_types
require dasllama/dasllama_audio_io
require dasllama/dasllama_wav
require dasllama/dasllama_kitten   // kitten_token_ids: the symbol table's drop rule, model-free
require dasllama/dasllama_styletts2   // the voices table: a hand-built model whose pack names no language; the source's stream
require dasllama/dasllama_tts_blocks   // TtsRng, TtsLinear, SineSourceCfg: the source's noise stream held to its own layout
require dasllama/dasllama_unicode   // the chunker counts codepoints, so the cells do too
require dasllama/dasllama_math   // with_dasllama_jobque_ + the batch lane cap
require daslib/jobque_boost
require daslib/defer
require daslib/fio
require strings
require daslib/strings_boost
require math
require _model_tier
require _tts_corpus   // the 200-sentence front-end fixture: the phonemes arm E was scored against
require _tts_oracle   // captured_noise: the source's noise stream in the reference's draw order

// The TTS facade above the families. Model-free: the sentence chunker (the reference driver's
// boundary rule - abbreviations, decimals, a.m./p.m. - the 400-CODEPOINT cap, the hard split of
// a whitespace-free run, the appended punctuation), the normalizer the facade exposes
// (`tts_normalize`), the WAV writer's container (header fields, sample round trip), the timings'
// own rtf guard, the kitten symbol table's drop rule, the chunk cap knob on the model's own
// chunker (`tts_set_chunk_chars`, its floor refused by name), the release keeping a captured
// carrier, and the harmonic source's noise stream on a hand-built decoder: the drawn stream
// speaks the captured stream's samples, another seed another stream, the window knob reaching
// the carry (the whole run, five frames) with the same samples, a negative window refused.
// The facade's cells on a loaded model are `_tts_facade_cells.das`'s, run on kitten-nano's block
// in test_tts_kitten.das.

[test(description = "`tts_chunks` splits text at terminal punctuation followed by a space into exact sentence chunks, and a final chunk without punctuation gains a comma")]
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

[test(description = "With the appended-comma rule off, `tts_chunks` passes text through unpunctuated, keeps the text's own punctuation, and leaves word-split pieces bare")]
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

[test(description = "`tts_chunks` does not end a sentence at abbreviations, decimals or a.m./p.m. before lower case, splits long sentences on words under a codepoint (not byte) cap, hard-splits a whitespace-free run losslessly, and yields nothing for blank text")]
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
[test(description = "`tts_voice_lang` on a voice whose pack names no language panics naming the voice and saying its language is not phonemized, never rendering an empty language code")]
def test_voice_refusal_without_a_language(t : T?) {
    t |> run("a voice whose pack names no language refuses in words, not in an empty code") @(t : T?) {
        var inscope m = TtsModel()
        m.kind = TtsKind.kokoro
        var inscope pack : array<float>
        m.model.voices["emma"] <- pack   // the model HAS the voice; what it does not have is a language for it
        let why = panic_text() {
            feint("{tts_voice_lang(m, "emma")}")
        }
        t |> success(find(why, "unknown voice 'emma'") >= 0, "the refusal names the voice: {why}")
        t |> success(find(why, "speaks a language the front end does not phonemize") >= 0,
            "and says what it cannot do with it: {why}")
        t |> success(find(why, "speaks , which") < 0, "no empty language code rendered into the sentence: {why}")
    }
}

[test(description = "`tts_normalize` expands abbreviations, decimals, money, clock times, dates, ordinals, percentages and units to exact spoken English, and keeps empty text empty")]
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

[test(description = "`kitten_token_ids` drops an unknown phoneme so the ids equal those of the same string without it, id for id, with no doubled separator")]
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

[test(description = "`st2_group` files a StyleTTS2 tensor under its recipe group by name - the published files' own tensor names, the staged names an LSTM, a style projection and a voice pack go by - each group's recipe format is the one the architecture states, and every group the recipe lists is reached")]
def test_recipe_groups(t : T?) {
    let cases <- [
        (name = "bert_encoder.weight", group = "bert", fmt = St2Fmt.q8),
        (name = "bert.encoder.albert_layer_groups.0.albert_layers.0.ffn.weight", group = "bert", fmt = St2Fmt.q8),
        (name = "predictor.N_proj.weight", group = "predictor", fmt = St2Fmt.q8),
        (name = "predictor.F0.0.conv1.weight", group = "predictor", fmt = St2Fmt.q8),
        (name = "predictor.N.0.norm1.fc.weight", group = "fc", fmt = St2Fmt.q8),
        (name = "decoder.decode.0.norm2.fc.weight", group = "fc", fmt = St2Fmt.q8),
        (name = ".fc", group = "fc", fmt = St2Fmt.q8),
        (name = "lstm", group = "lstm", fmt = St2Fmt.q8),
        (name = "text_encoder.cnn.0.0.weight", group = "text_encoder", fmt = St2Fmt.k4),
        (name = "decoder.N_conv.weight", group = "decoder", fmt = St2Fmt.k4),
        (name = "decoder.F0_conv.weight", group = "decoder", fmt = St2Fmt.k4),
        (name = "decoder.asr_res.0.weight", group = "decoder", fmt = St2Fmt.k4),
        (name = "decoder.decode.0.conv2.weight", group = "decoder", fmt = St2Fmt.k4),
        (name = "decoder.generator.resblocks.0.convs1.0.weight", group = "generator", fmt = St2Fmt.k4),
        (name = "decoder.generator.m_source.l_linear.weight", group = "generator", fmt = St2Fmt.k4),
        (name = "decoder.generator.ups.0.weight", group = "ups", fmt = St2Fmt.k6),
        (name = "decoder.generator.noise_convs.0.weight", group = "noise", fmt = St2Fmt.f32),
        (name = "decoder.generator.conv_post.weight", group = "post", fmt = St2Fmt.f32),
        (name = "voice.af_sky", group = "voice", fmt = St2Fmt.q8)
    ]
    var inscope reached : table<string>
    for (c in cases) {
        t |> equal(st2_group(c.name), c.group, "{c.name}")
        t |> equal(st2_recipe_default(c.group), c.fmt, "the {c.group} group's recipe format")
        reached |> insert(c.group)
    }
    for (group in ST2_GROUPS) {
        t |> success(key_exists(reached, group), "the cases reach the {group} group")
    }
    t |> success(st2_group("predictor.N_proj.weight") != st2_group("decoder.N_conv.weight"), "a predictor tensor is not the decoder's (the prefix test's control)")
}

[test(description = "A `.dlim` that is no readable image is asked about without a panic: `tts_pack_names` names the English packs for it, as for an image with no GGUF beside it, and `tts_needs_packs` says it reads packs")]
def test_unreadable_image_asked_first(t : T?) {
    let junk = path_join(test_temp_dir(), "unreadable.dlim")
    fopen(junk, "wb") $(f) {
        f |> fwrite("not an image: text standing where a prepared image's header would be")
    }
    defer() {
        remove(junk)
    }
    var inscope names <- tts_pack_names(junk)
    t |> success(!empty(names), "the pack question answers for an unreadable image ({length(names)} packs named)")
    t |> success(tts_needs_packs(junk), "and it is no Pocket image, so it reads packs")
}

[test(description = "A recipe ask serves Q8_0 beside its K-quant groups whatever the q8 pin says: with the f32 lane pinned `set_styletts2_recipe` turns `styletts2_serves_q8` on, and `reset_styletts2_recipe` hands the lane back to the pin")]
def test_recipe_ask_over_f32_pin(t : T?) {
    defer() {
        reset_styletts2_recipe()
        reset_styletts2_q8()
    }
    reset_styletts2_recipe()
    set_styletts2_q8(false)
    t |> success(!styletts2_serves_q8() && !styletts2_serves_recipe(), "the f32 pin alone serves neither")
    set_styletts2_recipe("")
    t |> success(styletts2_serves_recipe() && styletts2_serves_q8(), "the recipe ask serves the recipe lane and its Q8_0 groups over the f32 pin")
    reset_styletts2_recipe()
    t |> success(!styletts2_serves_q8(), "the ask dropped, the f32 pin stands (the control)")
}

[test(description = "`rtf` returns 0 for zero or negative sample counts instead of dividing, and 0.001 for one second of audio made in one millisecond")]
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

[test(description = "`wav_pcm16_bytes` writes a 44-byte RIFF header with mono and the sample rate in place, and a written WAV loads back within one 16-bit quantization step per sample")]
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
        var inscope back <- load_audio_mono(path, 24000)
        t |> equal(length(back), 2400)
        var worst = 0.0
        for (a, b in pcm, back) {
            worst = max(worst, nan_wide_diff(a, b))
        }
        t |> success(worst < 1.0 / 32000.0, "round trip within one quantization step ({worst})")
        var inscope raw <- pcm16_bytes(pcm)
        t |> equal(length(raw), 4800)
        let removed = remove_result(path)
        t |> success(removed is value, "temp file removed")
    }
}

[test(description = "`tts_set_chunk_chars` lowers the model chunker's codepoint cap and restores it, a cap under one is refused by name, and `tts_release_scratch` keeps a captured-noise carrier but empties a drawn one")]
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
        let why = panic_text() {
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

[test(description = "On a hand-built decoder the drawn source noise speaks the captured stream's samples bit for bit at the default, whole-run and five-frame windows, another seed moves most samples, and a negative window is refused by name")]
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
        var inscope captured <- captured_noise(cfg.n_harm, n, seed)
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
        let why = panic_text() {
            set_styletts2_source_window(-1l)
        }
        t |> success(find(why, "set_styletts2_source_window") >= 0, "the refusal names the knob: {why}")
        t |> equal(styletts2_source_window(), SOURCE_WINDOW_FRAMES, "and the window stays where it was")
    }
}
