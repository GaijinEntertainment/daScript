options gen2
options persistent_heap
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama_tts
require dasllama/dasllama_tts_types
require dasllama/dasllama_audio_io
require dasllama/dasllama_math   // setup_dasllama_jobque_
require daslib/jobque_boost
require daslib/fio
require math
require _model_tier

// The TTS facade above the families. Model-free: the sentence chunker (the reference driver's
// boundary rule - abbreviations, decimals, a.m./p.m. - the 400-character cap, the appended
// punctuation) and the WAV writer's container (header fields, sample round trip). Model-gated
// (`kitten-nano.gguf` + the front-end packs under models_dir()): the streaming form's chunks
// concatenate to the buffered form's audio with summed timings.

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

[test]
def test_streaming(t : T?) {
    t |> run("chunks concatenate to the buffered synthesis") @(t : T?) {
        let gguf = path_join(models_dir(), "kitten-nano.gguf")
        if (!stat(gguf).is_valid || !stat(path_join(models_dir(), "tts_g2p.bin")).is_valid) {
            t |> skip("kitten-nano.gguf or the front-end packs not present")
            return
        }
        with_job_que() {
            setup_dasllama_jobque_()
            var inscope m <- load_tts_model(gguf)
            let text = "The first sentence is short. The second one is a little longer than the first! Is the third a question?"
            var chunks = 0
            var inscope streamed : array<float>
            synthesize_stream(m, text, "Bella", 1.0) $(chunk) {
                chunks++
                streamed |> push_from(chunk.pcm)
            }
            t |> equal(chunks, 3, "three sentences, three chunks")
            var inscope whole <- synthesize(m, text, "Bella")
            t |> equal(length(whole.pcm), length(streamed), "the buffered form is the concatenation")
            var same = true
            for (a, b in whole.pcm, streamed) {
                same = same && a == b
            }
            t |> success(same, "sample for sample")
            t |> success(whole.timings.samples == long_length(whole.pcm), "the summed timings count every sample")
        }
    }
}
