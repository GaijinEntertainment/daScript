options gen2
options indenting = 4
options persistent_heap
options gc
options no_unused_function_arguments = false
options no_unused_block_arguments = false

require dastest/testing_boost
require strudel/strudel
require strudel/strudel_player
require audio/audio_boost
require daslib/fio

// The playback worker's context is its own: nothing else collects it, and a bed that queries
// four patterns every tick leaves megabytes of garbage a second behind. The number the worker
// publishes is its heap BEFORE the tick's collect, so it reads live set plus that tick's garbage
// - a tick's garbage runs from hundreds of kilobytes to ten megabytes, so the number jitters
// by that much between consecutive ticks even on an idle box, and a bar on its spread measures
// the burstiness of a query, not collection. Two things do measure collection, and each is
// zero for a worker that never collects: a published number LOWER than the one before it,
// which only a collect can produce (test_worker_no_gc.das pins the converse), and a floor -
// the least number over a stretch - that holds where an uncollected heap climbs a window's
// worth a second. Warm-up and the two stretches are measured on the worker's own clock, its
// pattern time, so a slow box warms up before it is measured. Null backend, so it runs headless.

let WARM_UP_SECONDS = 6.0lf
let STRETCH_SECONDS = 3.0lf
let MIN_DROPS = 6                                 // an idle 64-core box reads ~28 per two stretches
let FLOOR_DRIFT_LIMIT = 4ul * 1024ul * 1024ul     // twice the worker's collect window; uncollected grows ~10 MB a stretch
let WALL_CAP_MS = 90000u                          // a worker that never advances is its own failure

def worker_setup() {
    strudel_set_cps(2.0lf)
    strudel_add_track(s("<bd [hh hh] sd [hh hh]> <bd [hh cp?0.5] sd [hh hh]>"), 0.5)
    strudel_add_track(note_pattern("<[a2 a2] [a2 a2] [f2 f2] [g2 g2]> <[a2 a2] [c3 c3] [d3 d3] [e3 e3]>", "square"), 0.3)
    strudel_add_track(note_pattern("<a4 c5 e5 a5> <g5 e5 c5 ~> <[a4 b4] c5 e5 ~>", "sawtooth"), 0.25)
    strudel_add_track(note_pattern("<[a4 c5 e5 a5 c6 a5 e5 c5]> <[e5 a5 c6 e6 c6 a5 e5 a4]>", "triangle"), 0.12)
    strudel_play()
}

def worker_seconds() : double {
    return strudel_get_playback_time()._0
}

// Sleeps until the worker's clock reaches `until`, or the wall cap passes; false on the cap.
def wait_worker_until(until : double; var wall_ms : uint&) : bool {
    while (worker_seconds() < until) {
        return false if (wall_ms >= WALL_CAP_MS)
        sleep(20u)
        wall_ms += 20u
    }
    return true
}

struct Stretch {
    samples : int      // distinct published numbers seen; a stretch with none measured nothing
    lo : uint64
    drops : int
}

// The least published heap over one stretch of worker time, and how many times a tick published
// less than the tick before it; `prev` carries the last number across stretches.
def sample_stretch(until : double; var prev : uint64&; var wall_ms : uint&) : Stretch {
    var st = Stretch(lo = 0xFFFFFFFFFFFFFFFFul)
    while (worker_seconds() < until && wall_ms < WALL_CAP_MS) {
        sleep(20u)
        wall_ms += 20u
        let h = strudel_get_worker_heap_bytes()
        continue if (h == prev)          // the same tick as last time
        if (prev != 0ul && h < prev) { st.drops++ }
        prev = h
        st.lo = min(st.lo, h)
        st.samples++
    }
    return st
}

[test]
def test_worker_heap_stays_flat(t : T?) {
    t |> run("the playback worker collects its own heap: past warm-up the published heap drops between ticks and its floor holds") @(t : T?) {
        sound_set_null_device(true)
        with_audio_system() {
            strudel_init(@@worker_setup)
            var wall_ms = 0u
            let warm = wait_worker_until(WARM_UP_SECONDS, wall_ms)
            t |> success(warm, "the worker reached {WARM_UP_SECONDS} s of pattern time inside {WALL_CAP_MS} ms of wall time")
            if (warm) {
                var prev = 0ul
                let w0 = worker_seconds()
                let early = sample_stretch(w0 + STRETCH_SECONDS, prev, wall_ms)
                let late = sample_stretch(w0 + 2.0lf * STRETCH_SECONDS, prev, wall_ms)
                // a stretch that saw no published number leaves `lo` at its sentinel, which would
                // pass a bare `lo > 0`; the sample counts are what say the worker published at all
                t |> success(early.samples > 0 && late.samples > 0, "the worker publishes its heap: {early.samples} then {late.samples} distinct numbers")
                let drops = early.drops + late.drops
                t |> success(drops >= MIN_DROPS, "the heap dropped between ticks {drops} times over {2.0lf * STRETCH_SECONDS} s of pattern time - only a collect lowers it")
                let drift = int64(late.lo) - int64(early.lo)
                t |> success(drift < int64(FLOOR_DRIFT_LIMIT), "the floor moved {drift / 1024l} KB between the stretches (early {int64(early.lo) / 1024l} KB, late {int64(late.lo) / 1024l} KB)")
            }
            strudel_shutdown()
        }
        sound_set_null_device(false)   // sticky process-wide flag: leave a clean slate
    }
}
