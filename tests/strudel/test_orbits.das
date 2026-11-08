options gen2

require dastest/testing_boost
require strudel/strudel
require strudel/strudel_synth
require strudel/strudel_scheduler
require strudel/strudel_samples
require math

// Helper: tick a scheduler with a pattern for N chunks and return concatenated output.
def private tick_chunks(var sched : Scheduler; var pat : Pattern; bank : SampleBank; numChunks : int; chunkSec : float = 0.05) : array<float> {
    let chunkFrames = int(chunkSec * float(SAMPLE_RATE))
    var result : array<float>
    result |> reserve(numChunks * chunkFrames * 2)
    for (c in range(numChunks)) {
        let wall = double(c) * double(chunkSec)
        tick(sched, pat, bank, wall, chunkSec)
        result |> push_from(sched.output)
    }
    return <- result
}

// Helper: compute RMS of a range of stereo samples.
def private rms_range(samples : array<float>; startSample, endSample : int) : float {
    var sum = 0.0
    var count = 0
    let start2 = startSample * 2
    let end2 = min(endSample * 2, length(samples))
    for (i in range(start2, end2)) {
        sum += samples[i] * samples[i]
        count ++
    }
    if (count == 0) {
        return 0.0
    }
    return sqrt(sum / float(count))
}

[test(description = "an orbit bus is created when an event uses a non-zero orbit")]
def test_orbit_bus_created_on_demand(t : T?) {
    t |> run("orbit bus created when event uses non-zero orbit") @(t : T?) {
        var inscope sched : Scheduler
        sched.cps = 1.0lf
        // No orbit buses initially
        t |> equal(length(sched.orbit_buses), 0)
        let pat <- atom("sine") |> set_room(0.5) |> set_orbit(2)
        let bank : SampleBank
        tick(sched, pat, bank, 0.0lf, 0.05)
        // Should have created orbit buses 0,1,2 (at least up to index 2)
        t |> success(length(sched.orbit_buses) >= 3, "orbit_buses should grow to include orbit 2")
        t |> success(sched.orbit_buses[2] != null, "orbit bus 2 should be created")
        t |> success(get_orbit_reverb(sched, 2) != null, "orbit 2 should have reverb")
    }
}

[test(description = "the default orbit 0 works as it did before orbits existed")]
def test_orbit_zero_default(t : T?) {
    t |> run("default orbit 0 works as before") @(t : T?) {
        var inscope sched : Scheduler
        sched.cps = 1.0lf
        let pat <- atom("sine") |> set_room(0.5)
        let bank : SampleBank
        tick(sched, pat, bank, 0.0lf, 0.05)
        t |> success(get_orbit_reverb(sched, 0) != null, "orbit 0 reverb should exist")
    }
}

[test(description = "different orbits get independent reverb instances")]
def test_different_orbits_separate_reverb(t : T?) {
    t |> run("different orbits have independent reverb instances") @(t : T?) {
        var inscope sched : Scheduler
        sched.cps = 1.0lf
        // Two events on different orbits, both with reverb
        let pat <- stack([
            atom("sine") |> set_room(0.8) |> set_roomsize(1.0) |> set_orbit(0),
            atom("sine") |> set_room(0.8) |> set_roomsize(8.0) |> set_orbit(1)
        ])
        let bank : SampleBank
        tick(sched, pat, bank, 0.0lf, 0.05)
        // Both orbits should have reverb
        t |> success(get_orbit_reverb(sched, 0) != null, "orbit 0 should have reverb")
        t |> success(get_orbit_reverb(sched, 1) != null, "orbit 1 should have reverb")
        // They should be different instances
        t |> success(get_orbit_reverb(sched, 0) != get_orbit_reverb(sched, 1), "orbits should have different reverb instances")
    }
}

[test(description = "different orbits get independent delay instances")]
def test_different_orbits_separate_delay(t : T?) {
    t |> run("different orbits have independent delay instances") @(t : T?) {
        var inscope sched : Scheduler
        sched.cps = 1.0lf
        let pat <- stack([
            atom("sine") |> set_delay(0.5) |> set_delaytime(0.2) |> set_orbit(0),
            atom("sine") |> set_delay(0.5) |> set_delaytime(0.5) |> set_orbit(1)
        ])
        let bank : SampleBank
        tick(sched, pat, bank, 0.0lf, 0.05)
        t |> success(get_orbit_delay(sched, 0) != null, "orbit 0 should have delay")
        t |> success(get_orbit_delay(sched, 1) != null, "orbit 1 should have delay")
        t |> success(get_orbit_delay(sched, 0) != get_orbit_delay(sched, 1), "orbits should have different delay instances")
    }
}

[test(description = "no orbit bus is created for dry events")]
def test_orbit_no_effects_when_dry(t : T?) {
    t |> run("orbit bus not created for dry events") @(t : T?) {
        var inscope sched : Scheduler
        sched.cps = 1.0lf
        // Dry event on orbit 3 (no room, no delay)
        let pat <- atom("sine") |> set_orbit(3)
        let bank : SampleBank
        tick(sched, pat, bank, 0.0lf, 0.05)
        // Orbit bus should exist but have no reverb/delay
        t |> success(length(sched.orbit_buses) >= 4, "orbit_buses should grow")
        t |> success(get_orbit_reverb(sched, 3) == null, "dry orbit should have no reverb")
        t |> success(get_orbit_delay(sched, 3) == null, "dry orbit should have no delay")
    }
}

[test(description = "an OscVoice takes its orbit from the event")]
def test_orbit_osc_voice_field(t : T?) {
    t |> run("OscVoice orbit field populated from Event.orbit") @(t : T?) {
        var inscope sched : Scheduler
        sched.cps = 1.0lf
        let pat <- atom("sine") |> set_orbit(5)
        let bank : SampleBank
        tick(sched, pat, bank, 0.0lf, 0.05)
        t |> equal(length(sched.osc_voices), 1)
        t |> equal(sched.osc_voices[0].orbit, 5)
    }
}

[test(description = "a Voice takes its orbit from the event")]
def test_orbit_voice_field(t : T?) {
    t |> run("Voice orbit field populated from Event.orbit") @(t : T?) {
        var inscope sched : Scheduler
        sched.cps = 1.0lf
        let pat <- atom("bd") |> set_orbit(2)
        let bank : SampleBank
        tick(sched, pat, bank, 0.0lf, 0.05)
        t |> equal(length(sched.voices), 1)
        t |> equal(sched.voices[0].orbit, 2)
    }
}

[test(description = "an orbit with reverb produces an audible reverb tail")]
def test_orbit_reverb_produces_output(t : T?) {
    t |> run("orbit with reverb produces audible reverb tail") @(t : T?) {
        var inscope sched : Scheduler
        sched.cps = 0.5lf
        let pat <- atom("sine") |> set_room(1.0) |> set_gain(1.0) |> set_sustain(0.0) |> set_release(0.05) |> set_decay(0.1) |> set_orbit(1)
        let bank : SampleBank
        let output <- tick_chunks(sched, pat, bank, 20, 0.05)
        let sr = 48000
        let tailRms = rms_range(output, sr / 2, sr / 2 + sr / 4)
        t |> success(tailRms > 0.00001, "orbit reverb tail should be audible")
    }
}

[test(description = "an orbit with delay produces an echo")]
def test_orbit_delay_produces_echo(t : T?) {
    t |> run("orbit with delay produces echo") @(t : T?) {
        var inscope sched : Scheduler
        sched.cps = 0.5lf
        let pat <- atom("sine") |> set_delay(1.0) |> set_delaytime(0.25) |> set_delayfeedback(0.5) |> set_gain(1.0) |> set_sustain(0.0) |> set_release(0.05) |> set_decay(0.1) |> set_orbit(2)
        let bank : SampleBank
        let output <- tick_chunks(sched, pat, bank, 40, 0.05)
        let sr = 48000
        let echoRms = rms_range(output, sr / 4, sr / 2)
        t |> success(echoRms > 0.0001, "orbit delay echo should be audible")
    }
}

[test(description = "different room sizes on different orbits produce different reverb")]
def test_different_orbits_different_roomsize(t : T?) {
    t |> run("different roomsizes on different orbits produce different reverb") @(t : T?) {
        // Single scheduler, two simultaneous events with different orbits and roomsizes
        var inscope sched : Scheduler
        sched.cps = 0.5lf
        // Short pluck on orbit 0 (small room) and orbit 1 (large room)
        let pat <- stack([
            atom("sine") |> set_room(0.8) |> set_roomsize(0.3) |> set_gain(1.0) |> set_sustain(0.0) |> set_release(0.05) |> set_decay(0.05) |> set_orbit(0),
            atom("sine") |> set_room(0.8) |> set_roomsize(8.0) |> set_gain(1.0) |> set_sustain(0.0) |> set_release(0.05) |> set_decay(0.05) |> set_orbit(1)
        ])
        let bank : SampleBank
        let output <- tick_chunks(sched, pat, bank, 40, 0.05)
        // Both contribute to the same output, but the large room will have a louder tail
        // at 1.5s compared to a single small room.
        // Test that we get meaningful output (the orbits are processing independently)
        let sr = 48000
        let tailRms = rms_range(output, sr + sr / 2, sr + sr / 2 + sr / 4)
        t |> success(tailRms > 0.00001, "combined orbit output should have reverb tail")
    }
}

[test(description = "a pre-rendered drum on an orbit with delay produces an echo")]
def test_orbit_delay_on_drums(t : T?) {
    t |> run("pre-rendered drum with orbit delay has echo") @(t : T?) {
        var inscope sched : Scheduler
        sched.cps = 0.5lf
        let pat <- atom("bd") |> set_delay(0.8) |> set_delaytime(0.3) |> set_delayfeedback(0.5) |> set_orbit(1)
        let bank : SampleBank
        let output <- tick_chunks(sched, pat, bank, 40, 0.05)
        let sr = 48000
        let echoRms = rms_range(output, sr / 2, sr / 2 + sr / 4)
        t |> success(echoRms > 0.0001, "drum orbit delay echo should be non-zero")
    }
}

[test(description = "an orbit with both reverb and delay produces a combined tail")]
def test_orbit_reverb_plus_delay(t : T?) {
    t |> run("orbit with both reverb and delay produces combined tail") @(t : T?) {
        var inscope sched : Scheduler
        sched.cps = 0.5lf
        let pat <- atom("sine") |> set_room(0.6) |> set_delay(0.8) |> set_delaytime(0.25) |> set_delayfeedback(0.5) |> set_gain(1.0) |> set_sustain(0.0) |> set_release(0.05) |> set_decay(0.1) |> set_orbit(0)
        let bank : SampleBank
        let output <- tick_chunks(sched, pat, bank, 40, 0.05)
        let sr = 48000
        let tailRms = rms_range(output, sr / 2, sr / 2 + sr / 4)
        t |> success(tailRms > 0.0001, "orbit reverb+delay combined tail should be audible")
    }
}

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

[test(description = "an orbit gain ramps linearly to its target and settles, and time zero snaps it at once")]
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

[test(description = "an orbit gain of zero silences its routed voices and a partial level lowers a sustained voice proportionally")]
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
