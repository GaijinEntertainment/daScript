// Tutorial AUDIO-10: Global Controls
//
// This tutorial covers the master controls that affect every playing sound at
// once, rather than a single channel:
//   - set_global_volume — master volume multiplier
//   - set_global_pitch  — master playback-rate multiplier
//   - set_global_pause  — pause/resume the whole mixer
//
// These are the controls you reach for on focus loss (duck the master volume),
// slow-motion effects (global pitch), or a pause menu (global pause) — without
// touching each sound individually.
//
// Run: daslang.exe tutorials/dasAudio/10_global_controls.das

options gen2
options persistent_heap
options indenting = 4

require audio/audio_boost
require daslib/fio
require math

// ──────────────────────────────────────────────────────────────────────────
// Helper — a 1-second looping tone at a given frequency
// ──────────────────────────────────────────────────────────────────────────

def generate_tone(freq : float) : array<float> {
    return <- [for (x in range(MA_SAMPLE_RATE)); sin(2.0 * PI * freq * float(x) / float(MA_SAMPLE_RATE))]
}

[export]
def main() {
    print("AUDIO-10: Global Controls\n\n")
    with_audio_system() {
        // Two looping tones so the GLOBAL effect on every sound is audible.
        var tone_a <- generate_tone(440.0)
        var tone_b <- generate_tone(660.0)
        let sid_a = play_sound_loop_from_pcm(MA_SAMPLE_RATE, 1, tone_a)
        let sid_b = play_sound_loop_from_pcm(MA_SAMPLE_RATE, 1, tone_b)
        assert(sid_a != INVALID_SID && sid_b != INVALID_SID, "failed to start tones")
        print("Two looping tones playing (440 Hz + 660 Hz)\n")

        // ──────────────────────────────────────────────────────────────────
        // Section 1 — Master volume
        // ──────────────────────────────────────────────────────────────────
        //
        // set_global_volume multiplies the final mix. It scales every sound at
        // once and is independent of each sound's own set_volume.

        print("\n=== Section 1: Global volume ===\n")
        set_global_volume(0.3)
        print("  master volume 0.3 (both tones quieter)\n")
        sleep(1000u)
        set_global_volume(1.0)
        print("  master volume 1.0 (restored)\n")
        sleep(1000u)

        // ──────────────────────────────────────────────────────────────────
        // Section 2 — Master pitch
        // ──────────────────────────────────────────────────────────────────
        //
        // set_global_pitch multiplies playback rate for every sound — useful
        // for slow-motion (0.5) or fast-forward (2.0) effects.

        print("\n=== Section 2: Global pitch ===\n")
        set_global_pitch(0.5)
        print("  global pitch 0.5 (an octave down, slow motion)\n")
        sleep(1000u)
        set_global_pitch(1.0)
        print("  global pitch 1.0 (normal)\n")
        sleep(1000u)

        // ──────────────────────────────────────────────────────────────────
        // Section 3 — Master pause / resume
        // ──────────────────────────────────────────────────────────────────
        //
        // set_global_pause(true) freezes the whole mixer; (false) resumes it.
        // Each sound picks up exactly where it left off — ideal for a pause menu.

        print("\n=== Section 3: Global pause ===\n")
        set_global_pause(true)
        print("  paused (silence)\n")
        sleep(1000u)
        set_global_pause(false)
        print("  resumed\n")
        sleep(1000u)

        stop(sid_a)
        stop(sid_b)
        print("\nDone!\n")
    }
}
