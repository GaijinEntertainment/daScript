options gen2
options indenting = 4
options persistent_heap
options no_unused_function_arguments = false
options no_unused_block_arguments = false

require dastest/testing_boost
require daslib/defer
require strudel/strudel
require strudel/strudel_player
require audio/audio_boost
require jobque

// Exercises the AUDIO DEVICE teardown — with_audio_system + strudel main-thread
// playback (strudel_create_channel + strudel_tick) + strudel_shutdown — on
// miniaudio's NULL backend, so it runs headless / in CI (no real hardware).
//
// This whole directory is INTERPRETER-ONLY (gated off under --use-aot in
// tests/.das_test): driving the real device + its callback thread under AOT's
// tighter timing surfaces the pre-existing main-thread drain in
// audio_system_finalize (audio_boost.das:1284) racing the audio thread on the
// non-thread-safe mixer context (verySafeContext=false) -> heap corruption. No
// device test is AOT'd anywhere. The single-thread drain path itself is verified
// in the browser (site/tests/playground/audio.spec.js).
//
// What this guards: that the create/tick/shutdown cycle leaves NO net
// JobStatus/Channel/LockBox/Stream behind. On a threaded build (here, native +
// null backend) strudel_shutdown takes the original join path — the audio thread
// processes the deferred releases — so this is the regression check that the
// teardown ref-counting still balances.

[test]
def test_device_teardown_no_leak(t : T?) {
    t |> run("strudel device create/tick/shutdown leaves no JobQue leak") @(t : T?) {
        sound_set_null_device(true)
        // reset the global null-backend flag on exit, so a later audio test in this process isn't forced onto it
        defer() {
            sound_set_null_device(false)
        }
        let before = count_jobque_leaks()
        with_audio_system() {
            strudel_create_channel()
            strudel_set_cps(4.0lf)
            let pat <- note("c4 e4 g4 c5", "sine") |> attack(0.01) |> decay(0.05) |> sustain(0.4) |> release(0.1)
            strudel_add_track(pat)
            for (_i in range(20)) {
                strudel_tick()
            }
            strudel_shutdown()
        }
        let after = count_jobque_leaks()
        t |> equal(after, before)
    }
}

// Run the full cycle twice — re-creating the audio system after a clean teardown must
// work (the wasm playground re-runs the same program; this is the native mirror).
[test]
def test_device_teardown_rerun(t : T?) {
    t |> run("strudel device cycle is repeatable, still no JobQue leak") @(t : T?) {
        sound_set_null_device(true)
        // reset the global null-backend flag on exit, so a later audio test in this process isn't forced onto it
        defer() {
            sound_set_null_device(false)
        }
        let before = count_jobque_leaks()
        for (_cycle in range(2)) {
            with_audio_system() {
                strudel_create_channel()
                strudel_set_cps(4.0lf)
                let pat <- note("c4 e4 g4", "sine") |> release(0.1)
                strudel_add_track(pat)
                for (_i in range(10)) {
                    strudel_tick()
                }
                strudel_shutdown()
            }
        }
        let after = count_jobque_leaks()
        t |> equal(after, before)
    }
}
