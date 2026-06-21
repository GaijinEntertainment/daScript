options gen2
options indenting = 4

require dastest/testing_boost
require strudel/strudel
require strudel/strudel_scheduler
require strudel/strudel_samples
require strudel/strudel_player
require math

// One-shots = a finite phrase fired on an event, then "stop and die": stop emitting new notes
// after the window (the `once`/`playFor` gate), and retire the track once every voice has finished
// ringing (the release tail is preserved). Quantize snaps the start to the global beat grid.

def private pcm_rms(buf : array<float>) : float {
    if (empty(buf)) {
        return 0.0
    }
    var sum = 0.0
    for (s in buf) {
        sum += s * s
    }
    return sqrt(sum / float(length(buf)))
}

[test]
def test_play_for_gate(t : T?) {
    t |> run("once() keeps the phrase's onsets only within its first cycle, then silence") @(t : T?) {
        let pat <- once(fast(atom("sine"), 4))            // 4 onsets per cycle, gated to cycle 0
        var c0 <- pat(TimeSpan(start = 0.0lf, stop = 1.0lf))
        t |> success(length(c0) == 4, "the whole phrase plays once ({length(c0)} onsets)")
        delete c0
        var c1 <- pat(TimeSpan(start = 1.0lf, stop = 2.0lf))
        t |> success(length(c1) == 0, "no onsets after the window (stop)")
        delete c1
        var cneg <- pat(TimeSpan(start = -1.0lf, stop = 0.0lf))
        t |> success(length(cneg) == 0, "nothing before it starts")
        delete cneg
    }
    t |> run("playFor(n) spans n cycles, then stops") @(t : T?) {
        let pat <- playFor(atom("sine"), 2)
        var c0 <- pat(TimeSpan(start = 0.0lf, stop = 1.0lf))
        var c1 <- pat(TimeSpan(start = 1.0lf, stop = 2.0lf))
        var c2 <- pat(TimeSpan(start = 2.0lf, stop = 3.0lf))
        t |> success(length(c0) == 1 && length(c1) == 1, "plays across both cycles")
        t |> success(length(c2) == 0, "silent on the third cycle")
        delete c0
        delete c1
        delete c2
    }
}

[test]
def test_one_shot_render_stops(t : T?) {
    t |> run("once(): rings out during its cycle, then stays silent (stop + tail)") @(t : T?) {
        var inscope sched : Scheduler
        sched.cps = 2.0lf   // 1 cycle = 0.5 s
        let pat <- once(atom("sine") |> set_gain(1.0) |> set_sustain(0.15) |> set_release(0.15))
        let bank : SampleBank
        // render continuously (voices advance per tick, so don't jump the clock): the phrase sounds
        // early, then must stay silent — its release decays and `once` blocks any re-trigger.
        var headPeak = 0.0
        var tailPeak = 0.0
        let nChunks = 48        // 2.4 s
        let chunkSec = 0.05
        for (c in range(nChunks)) {
            tick(sched, pat, bank, double(c) * double(chunkSec), chunkSec)
            let r = pcm_rms(sched.output)
            if (c < 8) {                          // first 0.4 s — the one-shot is sounding
                headPeak = max(headPeak, r)
            } elif (c >= nChunks - 8) {            // last 0.4 s — long past the window
                tailPeak = max(tailPeak, r)
            }
        }
        t |> success(headPeak > 0.001, "audible during its one cycle")
        t |> success(tailPeak < 0.0001, "silent in the tail — release decayed and nothing re-triggers (stop)")
    }
}

// Player-level lifecycle + quantize share the strudel globals (g_wall_time, g_tracks), so they
// run as ordered `run` blocks in one [test] with a single teardown — no cross-test contamination.
[test]
def test_one_shot_player(t : T?) {
    strudel_set_cps(2.0lf)   // cycle = 0.5 s
    t |> run("strudel_one_shot fires, then auto-retires once silent (die)") @(t : T?) {
        let before = strudel_active_tracks()
        strudel_one_shot(atom("sine") |> set_gain(1.0) |> set_sustain(0.1) |> set_release(0.1))
        t |> success(strudel_active_tracks() == before + 1, "track is live while it plays")
        let t_end = g_wall_time + 2.0lf
        while (g_wall_time < t_end) {
            strudel_tick_offline()
        }
        t |> success(strudel_active_tracks() == before, "track is gone after it finishes — no leak (die)")
    }
    t |> run("quantized one-shot's first sound lands on the beat grid, not at fire time") @(t : T?) {
        let q = 0.5                         // snap to half-cycle beats (every 0.25 s) — float, matches the param
        let qd = double(q)
        // advance to an off-beat phase so the wait is observable
        let base = g_wall_time
        while (g_wall_time < base + 0.13lf) {
            strudel_tick_offline()
        }
        let fireWall = g_wall_time
        let cyc = fireWall * 2.0lf
        let nextBeatCyc = ceil((cyc + 1.0e-6lf) / qd) * qd
        let expectedWall = nextBeatCyc / 2.0lf
        strudel_one_shot(atom("sine") |> set_gain(1.0) |> set_sustain(0.3) |> set_release(0.1), 1.0, q)
        var onsetWall = -1.0lf
        let limit = g_wall_time + 1.0lf
        while (g_wall_time < limit) {
            strudel_tick_offline()
            if (onsetWall < 0.0lf && pcm_rms(g_master_pcm) > 0.001) {
                onsetWall = g_wall_time
            }
        }
        t |> success(onsetWall >= 0.0lf, "the quantized one-shot eventually sounds")
        t |> success(onsetWall > fireWall + 0.01lf, "it waited rather than firing immediately")
        t |> success(abs(onsetWall - expectedWall) < 0.09lf, "first sound lands on the next beat (~{expectedWall}s, got {onsetWall}s)")
    }
    strudel_shutdown()
}
