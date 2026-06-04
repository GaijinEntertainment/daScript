options gen2
options indenting = 4

// Locks the per-voice vs per-orbit FX classification (the distinction tutorials
// 07/08 disagreed on). Ground truth from the engine signal flow:
//   per-orbit / bus : room/roomsize, delay/delaytime/delayfeedback, chorus
//   per-voice       : lpf/hpf/bpf, phaser, tremolo, compressor, shape, crush,
//                     coarse, djf, gain, pan, speed, fm, vowel
// Per-orbit effects allocate a shared bus instance lazily per orbit; per-voice
// effects only set Event fields and never allocate an orbit bus.

require dastest/testing_boost
require strudel/strudel
require strudel/strudel_synth
require strudel/strudel_scheduler
require strudel/strudel_samples
require math

[test]
def test_per_orbit_effects_allocate_bus(t : T?) {
    t |> run("room/delay/chorus each lazily allocate their orbit bus on send") @(t : T?) {
        // reverb (room) on orbit 0
        var inscope s1 : Scheduler
        s1.cps = 1.0lf
        let pr <- atom("sine") |> room(0.6) |> set_orbit(0)
        let bank : SampleBank
        tick(s1, pr, bank, 0.0lf, 0.05)
        t |> success(get_orbit_reverb(s1, 0) != null, "room should allocate orbit reverb")

        // delay on orbit 1
        var inscope s2 : Scheduler
        s2.cps = 1.0lf
        let pd <- atom("sine") |> delay(0.5) |> set_orbit(1)
        tick(s2, pd, bank, 0.0lf, 0.05)
        t |> success(get_orbit_delay(s2, 1) != null, "delay should allocate orbit delay")

        // chorus on orbit 2
        var inscope s3 : Scheduler
        s3.cps = 1.0lf
        let pc <- atom("sine") |> chorus(0.6) |> set_orbit(2)
        tick(s3, pc, bank, 0.0lf, 0.05)
        t |> success(get_orbit_chorus(s3, 2) != null, "chorus should allocate orbit chorus")
    }
}

[test]
def test_per_voice_effects_do_not_allocate_orbit_bus(t : T?) {
    t |> run("per-voice FX set event fields but allocate no orbit bus") @(t : T?) {
        var inscope sched : Scheduler
        sched.cps = 1.0lf
        // lpf, phaser, crush, shape are per-voice — none should create an orbit bus
        let pat <- atom("sawtooth") |> lpf(800.0) |> phaser(2.0) |> crush(6.0) |> shape(0.4) |> set_orbit(0)
        let bank : SampleBank
        tick(sched, pat, bank, 0.0lf, 0.05)
        t |> success(get_orbit_reverb(sched, 0) == null, "per-voice FX must not allocate reverb")
        t |> success(get_orbit_delay(sched, 0) == null, "per-voice FX must not allocate delay")
        t |> success(get_orbit_chorus(sched, 0) == null, "per-voice FX must not allocate chorus")
    }
}

[test]
def test_per_voice_effects_set_event_fields(t : T?) {
    // per-voice FX live on the Event payload
    let p <- atom("sawtooth") |> lpf(800.0) |> hpf(200.0) |> bpf(1000.0) |> crush(6.0) |> shape(0.4) |> phaser(2.0) |> tremolo(3.0)
    let h <- p(TimeSpan(start = 0.0lf, stop = 1.0lf))
    t |> equal(length(h), 1)
    let e = h[0].value
    t |> success(abs(e.lpf - 800.0) < 0.001)
    t |> success(abs(e.hpf - 200.0) < 0.001)
    t |> success(abs(e.crush - 6.0) < 0.001)
    t |> success(abs(e.shape - 0.4) < 0.001)
}
