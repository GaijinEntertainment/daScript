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

//! Synthesized speech: mono f32 PCM at `sample_rate`.
struct TtsAudio {
    pcm : array<float>
    sample_rate : int
}

//! The source noise a synthesis consumed - captured from the oracle for a parity run, or
//! drawn from the session's own generator. Empty arrays mean "draw".
struct TtsNoise {
    uniform : array<float>   // one initial phase per harmonic
    normal : array<float>    // [samples x harmonics], sample-major
}
