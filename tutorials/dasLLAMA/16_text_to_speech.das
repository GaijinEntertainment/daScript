options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)

require dasllama/dasllama
require daslib/jobque_boost
require daslib/defer
require daslib/strings_boost
require strings

// Tutorial dasLLAMA-16: Text to Speech
//
// This tutorial covers:
//   - load_tts_model — one loader for every TTS family, the front-end packs read beside the GGUF
//   - caps() — the voices, the languages and the PCM rate a loaded model serves
//   - tts_normalize / tts_phonemize — the text front end on its own, without synthesizing
//   - set_tts_q8 / reset_tts_q8 / tts_serves_q8 — the two weight lanes, and the pin discipline
//   - synthesize — text in, PCM out; write_wav_pcm16 puts it on disk
//   - synthesize_stream — the same audio delivered sentence by sentence, as each piece lands
//   - TtsTimings and rtf — per-stage wall time, and seconds spent per second of speech
//   - tts_chunks — the sentence splitter one synthesis takes at a time
//
// Two families run through these verbs, and load_tts_model tells them apart from the GGUF's
// own architecture metadata:
//   - KittenTTS — kitten-nano and kitten-mini, 8 voices
//   - Kokoro-82M — 54 voices
// Both are StyleTTS2-lineage models: a text encoder predicts per-phoneme durations, a prosody
// branch predicts pitch and energy, a decoder and an iSTFT generator turn those into samples.
// caps() reports the rate each one emits.
//
// Prerequisites: a TTS GGUF with tts_g2p.bin and tts_postag.bin in the same directory. The
// packs are the text front end — normalization, part-of-speech tagging, grapheme-to-phoneme —
// and load_tts_model reads them from the model's own directory.
//
// Run: daslang.exe -jit tutorials/dasLLAMA/16_text_to_speech.das -- <tts-model.gguf> [out.wav] [--voice <name>] [--f32]

let TEXT = "daslang speaks. The text becomes phonemes, the phonemes become durations, and the durations become a waveform."

let PARAGRAPH = "Dr. Smith read the report on Feb. 3. It ran to 3.5 pages, which is short. Every sentence becomes its own chunk, and a sentence past the cap splits on a word boundary."

def seconds(pcm : array<float>; sample_rate : int) : float {
    return float(length(pcm)) / float(sample_rate)
}

[export]
def main {  // nolint:STYLE038 - one linear tutorial walkthrough
    let cmd <- get_command_line_arguments()
    var model_path = ""
    var out_path = "speech.wav"
    var voice = ""
    var want_f32 = false
    var i = 0
    while (i < length(cmd)) {
        let a = cmd[i]
        if (a == "--f32") {
            want_f32 = true
        } elif (a == "--voice" && i + 1 < length(cmd)) {
            voice = cmd[i + 1]
            i++
        } elif (ends_with(a, ".gguf")) {
            model_path = a
        } elif (ends_with(a, ".wav")) {
            out_path = a
        }
        i++
    }
    if (empty(model_path)) {
        print("usage: daslang.exe -jit tutorials/dasLLAMA/16_text_to_speech.das -- <tts-model.gguf> [out.wav] [--voice <name>] [--f32]\n")
        return
    }

    // ──────────────────────────────────────────────────────────────────────────
    // Section 1 — the two weight lanes, pinned before the load
    // ──────────────────────────────────────────────────────────────────────────
    //
    // A TTS model serves the GEMMs of its decoder and generator from one of two prepared
    // images beside the GGUF. The q8 lane holds those weights as Q8_0 quants and is what a
    // load serves by default; the f32 lane holds the file's own planes and is the reference
    // the parity tests compare against. tts_serves_q8() answers which lane the NEXT load
    // takes, set_tts_q8 pins it, and reset_tts_q8 returns to the default.
    //
    // The pin is process-wide state, so it must not outlive the load it was set for — a
    // stale pin silently changes the lane of the next model this process loads. defer()
    // resets it on every path out of main, a panic included.
    defer() {
        reset_tts_q8()
    }
    if (want_f32) {
        set_tts_q8(false)
    }
    print("weight lane: {tts_serves_q8() ? "q8" : "f32"}\n")

    // ──────────────────────────────────────────────────────────────────────────
    // Section 2 — load a model, then ask what it can say
    // ──────────────────────────────────────────────────────────────────────────
    //
    // One loader for every family. caps() is the honesty contract: the voices it accepts by
    // name, the languages it speaks, the rate its PCM comes out at, and whether it can clone
    // a voice from a recording. Ask a voice caps() does not carry and the call panics — the
    // request is never quietly swapped for something the model does have.
    let t_load = ref_time_ticks()
    var m <- load_tts_model(model_path)
    print("loaded in {double(get_time_usec(t_load)) / 1.0e6lf} s\n")

    let c <- caps(m)
    print("voices ({length(c.voices)}): {join(c.voices, ", ")}\n")
    print("languages: {join(c.langs, ", ")}\n")
    print("rate: {c.sample_rate} Hz; clones a voice: {c.cloning}\n")
    let vname = empty(voice) ? c.voices[0] : voice

    // ──────────────────────────────────────────────────────────────────────────
    // Section 3 — what the model receives
    // ──────────────────────────────────────────────────────────────────────────
    //
    // The two packs beside the GGUF are the text front end, and they run before any model
    // weight is touched. tts_normalize is the first pass — it rewrites numbers, money, dates,
    // times and abbreviations into the words a reader says. tts_phonemize is the last one: the
    // tagger of tts_postag.bin labels every token, and the grapheme-to-phoneme pass of
    // tts_g2p.bin turns the labelled tokens into the 45 US phoneme symbols. The tag is what
    // settles a heteronym — "read" the past tense and "read" the present tense are the same
    // letters and two different sounds. synthesize runs both passes for you; run them alone and
    // you see what the model is asked to say.
    let sentence = "Dr. Chen read 3.5 pages and paid $12."
    let spoken = tts_normalize(sentence)
    print("\ntext:       {sentence}\n")
    print("normalized: {spoken}\n")
    print("phonemes:   {tts_phonemize(m, spoken)}\n")

    // ──────────────────────────────────────────────────────────────────────────
    // Section 4 — the sentence chunker
    // ──────────────────────────────────────────────────────────────────────────
    //
    // One synthesis takes one sentence-sized piece. tts_chunks is the splitter synthesize
    // runs over the normalized text: sentences first, then words when a sentence runs past
    // TTS_CHUNK_CHARS codepoints, and every chunk ends in punctuation so the model hears a
    // prosodic close. A period inside "Dr." or "3.5" is not a sentence end.
    let pieces <- tts_chunks(PARAGRAPH)
    print("\ntts_chunks: {length(pieces)} chunk(s), cap {TTS_CHUNK_CHARS} codepoints\n")
    for (p in pieces) {
        print("  {p}\n")
    }

    with_job_que() {
        setup_dasllama_jobque()

        // ──────────────────────────────────────────────────────────────────────
        // Section 5 — one call: text in, a WAV out
        // ──────────────────────────────────────────────────────────────────────
        //
        // synthesize normalizes the whole text once, chunks it, speaks each chunk and
        // concatenates the result. TtsAudio carries mono f32 PCM, the rate, and TtsTimings —
        // where the wall time went, model loading excluded. write_wav_pcm16 writes that PCM
        // as a 16-bit RIFF/WAVE file, the container every player opens.
        let audio <- synthesize(m, TEXT, vname)
        print("\nsynthesize: {seconds(audio.pcm, audio.sample_rate)} s of speech, rtf {rtf(audio.timings, audio.sample_rate)}\n")
        print("  {timings_line(audio.timings, audio.sample_rate)}\n")
        if (write_wav_pcm16(out_path, audio.pcm, audio.sample_rate)) {
            print("  wrote {out_path}\n")
        } else {
            print("  could not write {out_path}\n")
        }

        // ──────────────────────────────────────────────────────────────────────
        // Section 6 — the same audio, one chunk at a time
        // ──────────────────────────────────────────────────────────────────────
        //
        // synthesize_stream hands each chunk to the block as soon as it exists, in order, so
        // a player can start on sentence one while sentence two is still being made. Every
        // chunk draws its own source noise from the chunk index, so both forms walk the same
        // chunk list and produce the same samples — the stream is the audio arriving earlier,
        // not different audio.
        var n = 0
        var streamed = 0
        synthesize_stream(m, TEXT, vname, 1.0) $(chunk) {
            n++
            streamed += length(chunk.pcm)
            print("\nchunk {n}: {seconds(chunk.pcm, chunk.sample_rate)} s\n")
            print("  {timings_line(chunk.timings, chunk.sample_rate)}\n")
        }
        print("\n{n} chunk(s), {streamed} samples; synthesize returned {length(audio.pcm)}\n")

        // ──────────────────────────────────────────────────────────────────────
        // Section 7 — speed and voice
        // ──────────────────────────────────────────────────────────────────────
        //
        // speed scales the predicted durations, so the same words come out in less time
        // without changing pitch. The voice is a name from caps().voices; each one is a
        // style vector the model was trained with, and switching it costs nothing. A
        // KittenTTS voice also carries its own speed prior, so the same speed number on two
        // voices is not the same tempo — hold the voice fixed to read the speed knob.
        let quick <- synthesize(m, TEXT, vname, 1.25)
        print("\n{vname} at speed 1.0:  {seconds(audio.pcm, audio.sample_rate)} s\n")
        print("{vname} at speed 1.25: {seconds(quick.pcm, quick.sample_rate)} s\n")
        let alt = length(c.voices) > 1 ? c.voices[1] : c.voices[0]
        let other <- synthesize(m, TEXT, alt)
        print("{alt} at speed 1.0:  {seconds(other.pcm, other.sample_rate)} s\n")
    }
}
