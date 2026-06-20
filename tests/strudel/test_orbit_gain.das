options gen2
options indenting = 4

require dastest/testing_boost
require strudel/strudel
require strudel/strudel_scheduler
require strudel/strudel_samples
require math

// Render numChunks chunks and return the RMS of the LAST chunk's output buffer.
def private tick_chunks_rms(var sched : Scheduler; var pat : Pattern; bank : SampleBank; numChunks : int; startWall, chunkSec : float) : float {
    var lastRms = 0.0
    for (c in range(numChunks)) {
        let wall = double(startWall) + double(c) * double(chunkSec)
        tick(sched, pat, bank, wall, chunkSec)
        var sum = 0.0
        for (s in sched.output) {
            sum += s * s
        }
        lastRms = sqrt(sum / float(max(length(sched.output), 1)))
    }
    return lastRms
}

[test]
def test_orbit_gain_ramp(t : T?) {
    t |> run("orbit gain ramps linearly to target, then settles") @(t : T?) {
        var inscope sched : Scheduler
        t |> numericEqual(orbit_gain(sched, 3), 1.0)   // unset orbit reads unity
        fade_orbit(sched, 3, 0.0, 0.5)                 // 1.0 -> 0.0 over 0.5s
        t |> numericEqual(orbit_gain(sched, 3), 1.0)   // fade armed, not advanced yet
        advance_orbit_gains(sched, 0.25)
        t |> numericEqual(orbit_gain(sched, 3), 0.5)   // halfway
        advance_orbit_gains(sched, 0.25)
        t |> numericEqual(orbit_gain(sched, 3), 0.0)   // arrived
        advance_orbit_gains(sched, 0.25)
        t |> numericEqual(orbit_gain(sched, 3), 0.0)   // settled, stays put
        t |> numericEqual(orbit_gain(sched, 1), 1.0)   // unrelated orbit unaffected
    }
    t |> run("fade up past unity amplifies; overshoot clamps to target") @(t : T?) {
        var inscope sched : Scheduler
        fade_orbit(sched, 2, 3.0, 1.0)                 // 1.0 -> 3.0 over 1s
        advance_orbit_gains(sched, 0.5)
        t |> numericEqual(orbit_gain(sched, 2), 2.0)
        advance_orbit_gains(sched, 1.0)                          // would overshoot -> clamp
        t |> numericEqual(orbit_gain(sched, 2), 3.0)
    }
    t |> run("time=0 snaps the level instantly") @(t : T?) {
        var inscope sched : Scheduler
        set_orbit_gain(sched, 0, 0.25)
        t |> numericEqual(orbit_gain(sched, 0), 0.25)
    }
}

[test]
def test_orbit_gain_attenuates_output(t : T?) {
    t |> run("snapping an orbit's gain to 0 silences its routed voices") @(t : T?) {
        var inscope sched : Scheduler
        sched.cps = 0.5lf
        let pat <- atom("sine") |> set_gain(1.0) |> set_sustain(1.0) |> set_release(0.5) |> set_orbit(1)
        let bank : SampleBank
        let loud = tick_chunks_rms(sched, pat, bank, 4, 0.0, 0.05)
        t |> success(loud > 0.001, "orbit at unity should be audible")
        set_orbit_gain(sched, 1, 0.0)
        let silent = tick_chunks_rms(sched, pat, bank, 4, 0.2, 0.05)
        t |> success(silent < loud * 0.05, "muted orbit should drop to near silence")
    }
    t |> run("a partial level proportionally lowers a sustained voice") @(t : T?) {
        var inscope sched : Scheduler
        sched.cps = 0.5lf
        let pat <- atom("sine") |> set_gain(1.0) |> set_sustain(1.0) |> set_orbit(2)
        let bank : SampleBank
        let full = tick_chunks_rms(sched, pat, bank, 2, 0.0, 0.05)
        t |> success(full > 0.001, "unity orbit should be audible")
        set_orbit_gain(sched, 2, 0.25)
        let quarter = tick_chunks_rms(sched, pat, bank, 2, 0.1, 0.05)
        t |> success(quarter > 0.0, "0.25 level should still be audible")
        t |> success(quarter < full * 0.5, "0.25 level should be markedly quieter than unity")
    }
}
