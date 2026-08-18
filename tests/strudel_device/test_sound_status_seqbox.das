options gen2
options indenting = 4
options persistent_heap
options no_unused_function_arguments = false
options no_unused_block_arguments = false

require dastest/testing_boost
require daslib/fio
require daslib/jobque_boost
require audio/audio_boost
require jobque
require math

// Playback status over a SeqBox, on miniaudio's NULL backend so it runs headless. This directory
// is INTERPRETER-ONLY (gated off under --use-aot in tests/.das_test) — see test_device_teardown.das.
//
// What this guards: the box reads sensibly the instant set_status_update returns, it tracks the
// sound to completion, and releasing it needs no ordering with the audio thread — the property
// that replaced the old join().

def private short_tone(seconds : float) : array<float> {
    let n = int(float(MA_SAMPLE_RATE) * seconds)
    return <- [for (x in range(n)); sin(2.0 * PI * 440.0 * float(x) / float(MA_SAMPLE_RATE))]
}

[test]
def test_status_seeded_on_the_calling_thread(t : T?) {
    t |> run("the box reads `starting` before the audio thread has seen the sound") @(t : T?) {
        sound_set_null_device(true)
        with_audio_system() {
            var tone <- short_tone(0.2)
            let sid = play_sound_from_pcm(MA_SAMPLE_RATE, 1, tone)
            var box = seq_box_create()
            set_status_update(sid, box)
            var state = AudioChannelState.stopped
            let got = box |> read() $(s : AudioChannelStatus) {
                state = s.state
            }
            t |> equal(got, true, "the box must not read empty right after registration")
            t |> equal(state, AudioChannelState.starting)
            stop(sid)
            unset_status_update(sid)
            box |> seq_box_release
        }
        sound_set_null_device(false)   // sticky process-wide flag: leave a clean slate
    }
}

[test]
def test_status_reaches_stopped(t : T?) {
    t |> run("a one-shot tone advances to stopped") @(t : T?) {
        sound_set_null_device(true)
        with_audio_system() {
            var tone <- short_tone(0.2)
            let sid = play_sound_from_pcm(MA_SAMPLE_RATE, 1, tone)
            var box = seq_box_create()
            set_status_update(sid, box)
            var reached_stopped = false
            for (_i in range(200)) {
                box |> read() $(s : AudioChannelStatus) {
                    reached_stopped = s.state == AudioChannelState.stopped
                }
                break if (reached_stopped)
                sleep(20u)
            }
            t |> success(reached_stopped, "0.2s tone never reported stopped within a 4s window")
            unset_status_update(sid)
            box |> seq_box_release
        }
        sound_set_null_device(false)   // sticky process-wide flag: leave a clean slate
    }
}

[test]
def test_release_needs_no_ordering(t : T?) {
    t |> run("releasing the box while the sound still plays leaks nothing") @(t : T?) {
        sound_set_null_device(true)
        let before = count_jobque_leaks()
        with_audio_system() {
            for (_i in range(8)) {
                var tone <- short_tone(30.0)
                let sid = play_sound_from_pcm(MA_SAMPLE_RATE, 1, tone)
                var box = seq_box_create()
                set_status_update(sid, box)
                // Drop our reference immediately, with the sound still playing and the audio
                // thread still holding its own. Under the old LockBox this needed unset + join
                // first, or it trapped on "being deleted while being used".
                box |> seq_box_release
            }
            sleep(60u)
        }
        t |> equal(count_jobque_leaks(), before)
        sound_set_null_device(false)   // sticky process-wide flag: leave a clean slate
    }
}
