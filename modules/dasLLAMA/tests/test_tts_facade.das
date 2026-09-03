options gen2
options persistent_heap
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama_tts
require dasllama/dasllama_tts_types
require dasllama/dasllama_audio_io
require dasllama/dasllama_kitten   // kitten_token_ids: the symbol table's drop rule, model-free
require dasllama/dasllama_unicode   // the chunker counts codepoints, so the cells do too
require dasllama/dasllama_math   // setup_dasllama_jobque_ + the batch lane cap
require daslib/jobque_boost
require daslib/defer
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

// The TTS facade above the families. Model-free: the sentence chunker (the reference driver's
// boundary rule - abbreviations, decimals, a.m./p.m. - the 400-CODEPOINT cap, the hard split of
// a whitespace-free run, the appended punctuation), the normalizer the facade exposes
// (`tts_normalize`), the WAV writer's container (header fields, sample round trip), the timings'
// own rtf guard, and the kitten symbol table's drop rule.
// Model-gated (`kitten-nano.gguf` + the front-end packs under models_dir()): the facade's
// phonemes (`tts_phonemize`) against the corpus fixture's own inventory, and the streaming
// form's chunks concatenate to the buffered form's audio with summed timings, the two
// syntheses run at different worker counts.

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
