// Tutorial DASTRUDEL-12: Samples
//
// This tutorial covers:
//   - Loading audio files into named sample banks with strudel_load_sound
//   - Playing samples with s("bd sd hh cp") — mini-notation drum patterns
//   - Sample variations: s("bd:0 bd:1") picks indexed variants
//   - Pitch-shifting samples with speed() (2.0 = one octave up)
//   - Decoding a single file with load_audio_file
//
// Run: daslang.exe tutorials/daStrudel/daStrudel_12_samples.das

options gen2
options indenting = 4
options persistent_heap

require strudel/strudel public
require strudel/strudel_player
require strudel/strudel_samples
require audio/audio_boost
require daslib/fio

let MEDIA = "{get_das_root()}/examples/media"

def play(var pat : Pattern; seconds : float = 6.0; cps : double = 0.5lf; setup : block = $ {}) {
    with_audio_system() {
        strudel_create_channel()
        strudel_set_cps(cps)
        invoke(setup)
        strudel_add_track(pat)
        let t0 = ref_time_ticks()
        while (float(get_time_usec(t0)) / 1000000.0 < seconds) {
            strudel_tick()
            sleep(5u)
        }
        strudel_shutdown()
    }
}

def load_drum_bank() {
    // Each "bd" / "sd" / "hh" / "cp" is a folder of audio files. Files are
    // sorted alphabetically and become variations indexed :0, :1, :2, ...
    strudel_load_sound("{MEDIA}/drums/bd", "bd")
    strudel_load_sound("{MEDIA}/drums/sd", "sd")
    strudel_load_sound("{MEDIA}/drums/hh", "hh")
    strudel_load_sound("{MEDIA}/drums/cp", "cp")
}

// ──────────────────────────────────────────────────────────────────────────
// Section 1 — Drum patterns from a sample bank
// ──────────────────────────────────────────────────────────────────────────
//
// A sample bank is just a directory of folders named after the sounds.
// strudel_load_sound("path/to/bd", "bd") loads every audio file in the
// folder; they become variations accessible as bd:0, bd:1, ... in the
// mini-notation. s("bd sd hh cp") is a classic 4-step drum pattern.

def example_drum_pattern() {
    print("=== Section 1: Drum pattern (bd sd hh cp) ===\n")
    let pat <- s("bd sd hh cp") |> gain(0.7)
    play(pat, 6.0, 0.5lf) {
        load_drum_bank()
    }
}

// ──────────────────────────────────────────────────────────────────────────
// Section 2 — Sample variations with :N
// ──────────────────────────────────────────────────────────────────────────
//
// When a sound folder has multiple files, the :N suffix picks a specific
// variation. s("bd:0") uses the first file, s("bd:1") the second, and so on.
// Omitting the suffix (just "bd") defaults to :0.

def example_variations() {
    print("\n=== Section 2: Variations (bd:0, bd:1) ===\n")
    // Alternate two kick variations per cycle.
    let pat <- s("bd:0 bd:1 bd:0 bd:1") |> gain(0.7)
    play(pat, 4.0, 0.5lf) {
        load_drum_bank()
    }
}

// ──────────────────────────────────────────────────────────────────────────
// Section 3 — Pitch-shifting with speed()
// ──────────────────────────────────────────────────────────────────────────
//
// speed() scales the playback rate of a sample. speed(2.0) is one octave up
// and twice as fast (tape-style pitch shift). This only affects sample
// playback — oscillators (sine, saw, ...) ignore speed().

def example_speed_shift() {
    print("\n=== Section 3: Pitch shift via speed() ===\n")
    // Alternate normal / pitched-up kicks each step.
    let pat <- s("bd bd bd bd") |> speed(1.0) |> gain(0.6)
    play(pat, 3.0, 0.5lf) {
        load_drum_bank()
    }
    let pitched <- s("bd bd bd bd") |> speed(2.0) |> gain(0.6)
    play(pitched, 3.0, 0.5lf) {
        load_drum_bank()
    }
}

// ──────────────────────────────────────────────────────────────────────────
// Section 4 — load_audio_file: decode a single file
// ──────────────────────────────────────────────────────────────────────────
//
// load_audio_file decodes one WAV/MP3/FLAC/OGG into a Sample struct so you
// can inspect channels, sample rate, and frame count without scheduling
// playback. Empty Sample (length=0) indicates a decode failure.

def example_decode() {
    print("\n=== Section 4: load_audio_file inspection ===\n")
    let sample <- load_audio_file("{MEDIA}/drums/bd/BT0A0A7.wav")
    if (empty(sample.data)) {
        print("  (file not found — skipping)\n")
        return
    }
    print("  channels:    {sample.channels}\n")
    print("  sample_rate: {sample.sample_rate} Hz\n")
    print("  frames:      {length(sample.data) / sample.channels}\n")
    print("  duration:    {float(length(sample.data)) / float(sample.channels * sample.sample_rate):.3f} s\n")
}

[export]
def main() {
    print("DASTRUDEL-12: Samples\n\n")
    example_drum_pattern()
    example_variations()
    example_speed_shift()
    example_decode()
    print("\nDone!\n")
}
