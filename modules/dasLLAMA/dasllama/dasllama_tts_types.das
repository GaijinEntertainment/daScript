options gen2
options _comment_hygiene = true
options _dasllama_internal = true

module dasllama_tts_types shared public

require dasllama/dasllama_lint public

//! The TTS floor: the types every family file and the facade share. Family files require
//! this, never each other.

struct TtsCaps {
    //! What a loaded TTS model can do: its voices (canonical names; aliases resolve in the
    //! family file), the PCM rate it emits, the languages it speaks, whether it clones a voice
    //! from audio (none of the first families do).
    voices : array<string>        //! canonical voice names, in the model's order
    voice_langs : array<string>   //! the language each voice speaks, by index into ``voices``
    sample_rate : int             //! PCM samples per second the model emits
    langs : array<string>         //! the languages it speaks, as codes
    cloning : bool           //! whether a voice can be cloned from audio
}

struct TtsTimings {
    //! Where a synthesis spent its time, in microseconds of wall clock, model loading excluded:
    //! the text front end, then each model stage. ``total_us`` covers front end through waveform.
    front_end_us : int64   //! normalize, tag, phonemize, map into the family's symbols
    bert_us : int64        //! the PL-BERT pass
    text_enc_us : int64    //! the text encoder
    duration_us : int64    //! the duration encoder and the alignment
    prosody_us : int64     //! F0 and energy
    decoder_us : int64     //! the decoder
    source_us : int64      //! the harmonic-plus-noise source
    generator_us : int64   //! the iSTFTNet generator
    total_us : int64       //! front end through waveform
    samples : int64        //! PCM samples produced
}

[cold_path]
def rtf(t : TtsTimings; sample_rate : int) : double {
    //! Real-time factor: generation seconds per second of audio produced.
    return t.samples <= 0l ? 0.0lf : double(t.total_us) / 1e6lf / (double(t.samples) / double(sample_rate))
}

[cold_path]
def timings_line(t : TtsTimings; sample_rate : int) : string {
    //! One line for the log: seconds of audio, wall time, RTF and the per-stage split in ms.
    let ms = @(us : int64) : string => "{double(us) / 1000.0lf}"
    return "{double(t.samples) / double(sample_rate)} s of audio in {ms(t.total_us)} ms (rtf {rtf(t, sample_rate)}): front {ms(t.front_end_us)}, bert {ms(t.bert_us)}, text {ms(t.text_enc_us)}, durations {ms(t.duration_us)}, prosody {ms(t.prosody_us)}, decoder {ms(t.decoder_us)}, source {ms(t.source_us)}, generator {ms(t.generator_us)}"
}

struct TtsAudio {
    //! Synthesized speech: mono f32 PCM at ``sample_rate``, with the time it took.
    pcm : array<float>     //! mono samples in [-1, 1]
    sample_rate : int      //! samples per second
    timings : TtsTimings   //! where the synthesis spent its time
}

struct TtsNoise {
    //! The source noise a synthesis consumed - captured from the oracle for a parity run, or
    //! drawn from the session's own generator into a carrier every synthesis reuses.
    @scratch uniform : array<float>   //! one initial phase per harmonic; drawn per synthesis into a reused carrier
    @scratch normal : array<float>    //! [samples x harmonics], sample-major
    captured : bool                   //! supplied by an oracle dump: a synthesis reads it as is and never redraws
}
