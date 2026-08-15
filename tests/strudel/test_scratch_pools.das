options gen2
options indenting = 4
options persistent_heap = true
options very_safe_context

// scope: the audio opt-in half of the scratch flag
// marks: scheduler pools/buffers + each orbit bus's send/return buffers, set at construction
// seams: mark_pools_scratch / mark_orbit_scratch run directly (the player's instances are
//        private); fade_orbit is the public path through ensure_orbit
// proof: one eager-math arm on a marked field (only the final generation lives)

require dastest/testing_boost public
require strudel/strudel_scheduler

var g_sched : Scheduler
var g_orbit_sched : Scheduler

[test]
def test_scheduler_pools_marked(t : T?) {
    t |> run("mark_pools_scratch marks every pool and buffer") @(t : T?) {
        mark_pools_scratch(g_sched)
        t |> success(is_scratch(g_sched.voices), "voices")
        t |> success(is_scratch(g_sched.osc_voices), "osc_voices")
        t |> success(is_scratch(g_sched.output), "output")
        t |> success(is_scratch(g_sched.sf2_voices), "sf2_voices")
        t |> success(is_scratch(g_sched.sf2_meta), "sf2_meta")
        t |> success(is_scratch(g_sched.orbit_buses), "orbit_buses")
        t |> success(is_scratch(g_sched.voice_fx_buf), "voice_fx_buf")
        t |> success(is_scratch(g_sched.hrtf_mono_buf), "hrtf_mono_buf")
        t |> success(is_scratch(g_sched.hrtf_stereo_buf), "hrtf_stereo_buf")
        t |> success(is_scratch(g_sched.hrtf_stereo_buf_r), "hrtf_stereo_buf_r")
    }
    t |> run("a marked pool grows eagerly under very_safe") @(t : T?) {
        let before = heap_bytes_allocated()
        for (c in fixed_array(16l, 32l, 64l, 128l, 256l, 512l, 1024l, 2048l, 4096l)) {
            g_sched.output |> reserve(c)
        }
        let grew = heap_bytes_allocated() - before
        t |> equal(grew, uint64(long_capacity(g_sched.output)) * 4ul)   // only the final generation lives
    }
}

[test]
def test_orbit_bus_marked(t : T?) {
    t |> run("mark_orbit_scratch marks the six send/return buffers") @(t : T?) {
        var bus : OrbitBus   // no inscope: nothing here allocates, and finalizing the raw FX pointers needs unsafe
        mark_orbit_scratch(bus)
        t |> success(is_scratch(bus.reverb_send_buf), "reverb_send_buf")
        t |> success(is_scratch(bus.reverb_out_buf), "reverb_out_buf")
        t |> success(is_scratch(bus.delay_send_buf), "delay_send_buf")
        t |> success(is_scratch(bus.delay_out_buf), "delay_out_buf")
        t |> success(is_scratch(bus.chorus_send_buf), "chorus_send_buf")
        t |> success(is_scratch(bus.chorus_out_buf), "chorus_out_buf")
    }
    t |> run("a bus built through ensure_orbit arrives marked") @(t : T?) {
        fade_orbit(g_orbit_sched, 0, 1.0, 0.0)   // public path: creates orbit 0 on demand
        t |> success(g_orbit_sched.orbit_buses[0] != null, "bus exists")
        t |> success(is_scratch(g_orbit_sched.orbit_buses[0].reverb_send_buf), "reverb_send_buf")
        t |> success(is_scratch(g_orbit_sched.orbit_buses[0].chorus_out_buf), "chorus_out_buf")
    }
}
