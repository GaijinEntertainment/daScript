options gen2
options _comment_hygiene = true
options _dasllama_internal = true

module dasllama_tts_types shared public

require dasllama/dasllama_lint public

//! The TTS floor: the types every family file and the facade share. Family files require
//! this, never each other.

//! What a loaded TTS model can do: its voices (canonical names; aliases resolve in the
//! family file), the PCM rate it emits, the languages it speaks, whether it clones a voice
//! from audio (none of the first families do).
struct TtsCaps {
    voices : array<string>
    sample_rate : int
    langs : array<string>
    cloning : bool
}

//! Where a synthesis spent its time, in microseconds of wall clock, model loading excluded:
//! the text front end, then each model stage. `total_us` covers front end through waveform.
struct TtsTimings {
    front_end_us : int64
    bert_us : int64
    text_enc_us : int64
    duration_us : int64
    prosody_us : int64
    decoder_us : int64
    source_us : int64
    generator_us : int64
    total_us : int64
    samples : int64
}

//! Real-time factor: generation seconds per second of audio produced.
def rtf(t : TtsTimings; sample_rate : int) : double {
    return t.samples <= 0l ? 0.0lf : double(t.total_us) / 1e6lf / (double(t.samples) / double(sample_rate))
}

//! One line for the log: seconds of audio, wall time, RTF and the per-stage split in ms.
def timings_line(t : TtsTimings; sample_rate : int) : string {
    let ms = @(us : int64) : string => "{double(us) / 1000.0lf}"
    return "{double(t.samples) / double(sample_rate)} s of audio in {ms(t.total_us)} ms (rtf {rtf(t, sample_rate)}): front {ms(t.front_end_us)}, bert {ms(t.bert_us)}, text {ms(t.text_enc_us)}, durations {ms(t.duration_us)}, prosody {ms(t.prosody_us)}, decoder {ms(t.decoder_us)}, source {ms(t.source_us)}, generator {ms(t.generator_us)}"
}

//! Synthesized speech: mono f32 PCM at `sample_rate`, with the time it took.
struct TtsAudio {
    pcm : array<float>
    sample_rate : int
    timings : TtsTimings
}

//! The source noise a synthesis consumed - captured from the oracle for a parity run, or
//! drawn from the session's own generator into a carrier every synthesis reuses.
struct TtsNoise {
    @scratch uniform : array<float>   // one initial phase per harmonic; drawn per synthesis into a reused carrier
    @scratch normal : array<float>    // [samples x harmonics], sample-major
    captured : bool                   // supplied by an oracle dump: a synthesis reads it as is and never redraws
}
