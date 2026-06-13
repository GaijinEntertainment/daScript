// Tutorial AUDIO-09: Playback Status
//
// This tutorial covers:
//   - Registering a status LockBox with set_status_update
//   - Reading the AudioChannelStatus snapshot (state, position, queue depth)
//   - Watching a sound advance through its AudioChannelState lifecycle
//   - Releasing the status box cleanly (unset_status_update + lock_box_remove)
//
// The audio thread publishes a snapshot into a LockBox once per mix; the main
// thread reads it on demand with `box |> get()`. This is how you detect when a
// one-shot sound has finished, or how full a streaming queue is.
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
        // `starting`; the audio thread then updates it as the sound plays.
        // ──────────────────────────────────────────────────────────────────
        var status_box <- lock_box_create()
        set_status_update(sid, status_box)

        // Poll until the sound reports `stopped` (or we time out after ~2s),
        // printing each state transition we observe.
        var last_state = AudioChannelState.starting
        var saw_read = false
        var reached_stopped = false
        for (_i in range(100)) {
            status_box |> get() $(s : AudioChannelStatus#) {
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

        // ──────────────────────────────────────────────────────────────────
        // Release the status box. Order matters: stop the updates, clear the
        // stored value, join, then remove — otherwise the LockBox leaks.
        // ──────────────────────────────────────────────────────────────────
        unset_status_update(sid)
        clear_status(status_box)
        status_box |> join()
        unsafe(lock_box_remove(status_box))

        print("\nDone!\n")
    }
}
