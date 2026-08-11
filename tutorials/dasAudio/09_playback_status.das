// Tutorial AUDIO-09: Playback Status
//
// This tutorial covers:
//   - Registering a status SeqBox with set_status_update
//   - Reading the AudioChannelStatus snapshot (state, position, queue depth)
//   - Watching a sound advance through its AudioChannelState lifecycle
//   - Releasing the box with seq_box_release, in any order you like
//
// The audio thread publishes a snapshot into a SeqBox once per mix; the main
// thread reads it on demand with `box |> read()`. Neither side ever waits for the
// other. This is how you detect when a one-shot sound has finished, or how full a
// streaming queue is.
//
// Run: daslang.exe tutorials/dasAudio/09_playback_status.das

options gen2
options persistent_heap
options indenting = 4

require audio/audio_boost
require daslib/jobque_boost
require daslib/fio
require math

// ──────────────────────────────────────────────────────────────────────────
// Helper — a short (non-looping) tone so the sound finishes on its own
// ──────────────────────────────────────────────────────────────────────────

def generate_short_tone(seconds : float) : array<float> {
    let n = int(float(MA_SAMPLE_RATE) * seconds)
    return <- [for (x in range(n)); sin(2.0 * PI * 440.0 * float(x) / float(MA_SAMPLE_RATE))]
}

[export]
def main() {
    print("AUDIO-09: Playback Status\n\n")
    with_audio_system() {
        var tone <- generate_short_tone(0.3)
        let sid = play_sound_from_pcm(MA_SAMPLE_RATE, 1, tone)
        assert(sid != INVALID_SID, "failed to start sound")
        print("Playing a 0.3s tone, SID = {sid}\n")

        // ──────────────────────────────────────────────────────────────────
        // Register a status box. set_status_update seeds it with state
        // `starting`; the audio thread then publishes into it as the sound plays.
        // ──────────────────────────────────────────────────────────────────
        var status_box = seq_box_create()
        set_status_update(sid, status_box)

        // Poll until the sound reports `stopped` (or we time out after ~2s),
        // printing each state transition we observe.
        var last_state = AudioChannelState.starting
        var saw_read = false
        var reached_stopped = false
        for (_i in range(100)) {
            status_box |> read() $(s : AudioChannelStatus) {
                saw_read = true
                if (s.state != last_state) {
                    print("  state: {s.state}  (position={int(s.playback_position)} frames, queue={s.stream_que_length})\n")
                    last_state = s.state
                }
                if (s.state == AudioChannelState.stopped) {
                    reached_stopped = true
                }
            }
            break if (reached_stopped)
            sleep(20u)
        }

        assert(saw_read, "status box never produced a reading")
        print("  final observed state: {last_state}\n")
        // The whole point of the status box is detecting completion — so confirm
        // the one-shot tone actually ran through to `stopped` (0.3s tone, ~2s window).
        assert(reached_stopped, "sound never reached the stopped state within the poll window")

        // ──────────────────────────────────────────────────────────────────
        // Release the box. No ordering to get right and nothing to wait for:
        // the audio thread also holds a reference, and whichever side drops the
        // last one deletes it.
        // ──────────────────────────────────────────────────────────────────
        unset_status_update(sid)
        status_box |> seq_box_release

        print("\nDone!\n")
    }
}
