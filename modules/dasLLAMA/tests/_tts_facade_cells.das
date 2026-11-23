options gen2
options _dasllama_internal = true

module _tts_facade_cells shared public

//! the TTS facade's cells on a kitten-nano its caller loaded; no entry loads a model

require dastest/testing_boost public
require dasllama/dasllama_tts
require dasllama/dasllama_tts_types
require dasllama/dasllama_styletts2   // the source window, the empty driver that holds the device seat off
require dasllama/dasllama_math   // the batch lane cap
require daslib/defer
require strings
require _model_tier   // tts_data_missing
require _tts_corpus   // the 200-sentence front-end fixture: the phonemes arm E was scored against

// The facade's cells on a loaded kitten-nano (its block's image form, `load_tts_model` with the front-end packs beside
// the file): the facade's phonemes (`tts_phonemize`) against the corpus fixture's own inventory, the model's own chunker,
// the streaming form's chunks concatenating to the buffered form's audio with summed timings, the two syntheses run at
// different worker counts, and the idle release (`tts_release_scratch`): the carrier's generator rows, waveform and noise
// gone, the next synthesis sample-identical, and a lowered cap speaking the same sentences in more pieces. `path` is the
// model's file: every cell skips without the phoneme pack beside it.

// corpus rows the daslang front end reproduces phoneme for phoneme, so a facade wrapper that
// reached a family's rewrite instead of the front end's own inventory reds here
let private PINNED_CORPUS_ROWS <- {"lj000", "lj002", "lj003"}

//! `tts_phonemize` on `m` reproduces the pinned corpus rows' misaki phonemes exactly, bare and with en-us; the model declares
//! en-us alone, and an undeclared language panics
def private test_facade_phonemizer(t : T?; m : TtsModel; path : string) {
    unsafe {
        t |> run("the facade phonemizes into the front end's own inventory, on the corpus rail") @capture(& m)(t : T ?) {
            return if (tts_data_missing(t, path, false))
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
        t |> run("the language form answers for a language the model declares and panics for one it does not") @capture(& m)(t : T ?) {
            return if (tts_data_missing(t, path, false))
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
// cell can reach: the boolean form is pinned in test_tts_facade.das, Kokoro's bare text in test_tts_kokoro.das.
//! a loaded Kitten model's own chunker appends the driver's comma to unpunctuated chunks and keeps the text's own punctuation
def private test_chunker_model_rule(t : T?; m : TtsModel; path : string) {
    unsafe {
        t |> run("a Kitten model's chunks gain the driver's comma") @capture(& m)(t : T ?) {
            return if (tts_data_missing(t, path, false))
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

//! Kitten streaming synthesis at one lane emits one chunk a sentence whose concatenation equals the buffered synthesis at the
//! box's lane count sample for sample, and the summed timings count every sample
[arch(at = "../ARCHITECTURE_TTS.md#tts-two-layouts")]
def private test_streaming(t : T?; var m : TtsModel; path : string) {
    unsafe {
        t |> run("chunks concatenate to the buffered synthesis, at a different worker count") @capture(& m)(t : T ?) {
            return if (tts_data_missing(t, path, false))
            {   // a bare block: the defers are the cell's cleanup, not the lambda's finalizer
                let cap0 = get_batch_lane_cap()
                defer() {
                    set_batch_lane_cap(cap0)
                }
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
}

//! on kitten-nano CPU, the source window and `tts_release_scratch` leave the next synthesis sample-identical while the release
//! frees the carrier, and a 24-codepoint cap yields more chunks of audible speech of about the same length; the cap is put
//! back as the cell found it
def private test_release_and_cap_on_a_model(t : T?; var m : TtsModel; path : string) {
    unsafe {
        t |> run("the release empties the carrier and the next synthesis speaks the same samples; a cap makes more chunks") @capture(& m)(t : T ?) {
            return if (tts_data_missing(t, path, false))
            {   // a bare block: the defers are the cell's cleanup, not the lambda's finalizer
                // the source window is process state: the cell establishes the default it claims against and puts back what it found
                let win0 = styletts2_source_window()
                defer() {
                    set_styletts2_source_window(win0)
                }
                set_styletts2_source_window(SOURCE_WINDOW_FRAMES)
                // the carrier claims below are the CPU generator's: the empty driver holds the device seat off, the defer puts it back
                register_styletts2_gpu(St2GpuDriver())
                defer() {
                    unregister_styletts2_gpu()
                }
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
                let chars0 = m.chunk_chars
                defer() {
                    tts_set_chunk_chars(m, chars0)
                }
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
}

//! the facade's cells on `m`, kitten-nano's image form loaded from `path`; the release cell, which empties the carrier, last
[cold_path]
def tts_facade_kitten_cells(t : T?; var m : TtsModel; path : string) {
    test_facade_phonemizer(t, m, path)
    test_chunker_model_rule(t, m, path)
    test_streaming(t, m, path)
    test_release_and_cap_on_a_model(t, m, path)
}
