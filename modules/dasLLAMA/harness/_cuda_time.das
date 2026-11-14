options gen2
options indenting = 4
options _dasllama_internal = true

// The CUDA probes' clocks with no Vulkan module behind them: a chain of launches timed on the host wall (and between a
// CUDA event pair), its best round, and the profile rail's per-kernel report.

module _cuda_time shared public

require math
require dasllama/dasllama_cuda_common

//! `dispatches` launches closed by a sync, in microseconds a launch on the host wall; `launch` takes its index within the round
def cuda_time_wall(dispatches : int; launch : block<(i : int) : void>) : double {
    let t0 = ref_time_ticks()
    for (i in range(dispatches)) {
        invoke(launch, i)
    }
    cud_sync()
    return double(get_time_usec(t0)) / double(dispatches)
}

//! one round of a CUDA arm in microseconds a launch: `dispatches` launches closed by a sync on the host wall, then as many
//! between a CUDA event pair; `launch` takes its index within the round
def cuda_time_chain(dispatches : int; launch : block<(i : int) : void>) : tuple<wall : double; event : double> {
    let wall = cuda_time_wall(dispatches, launch)
    let ms = cud_time_ms() {
        for (i in range(dispatches)) {
            invoke(launch, i)
        }
    }
    return (wall = wall, event = double(ms) * 1000.0lf / double(dispatches))
}

//! `cuda_time_wall` over `reps` dispatches, the best of five chains after a warm one
def cuda_time_best(reps : int; launch : block<(i : int) : void>) : double {
    var best = 1.0e30lf
    for (round in range(6)) {
        let us = cuda_time_wall(reps, launch)
        if (round > 0) {
            best = min(best, us)
        }
    }
    return best
}

//! the profile rail's sums started over, unread
def cuda_times_drop {
    var rows <- cud_kernel_times_take()
    delete rows
}

//! the profile rail's kernels since its last take, a `TIMES <what>:` line each - its wall and launches a `per`th of the
//! window (a tick's, a command's, a round's) and its share of the sum, widest first, those under `floor` percent of the
//! sum dropped - then the sum's line, `tail` after its milliseconds
def cuda_times_report(what : string; per : int64; floor : double; tail : string) {
    var rows <- cud_kernel_times_take()
    var total = 0.0lf
    for (r in rows) {
        total += r.ms
    }
    for (r in rows) {
        continue if (r.ms <= 0.0lf || r.ms * 100.0lf < total * floor)
        print("TIMES {what}: {r.ms / double(per):.3f} ms {r.ms * 100.0lf / total:.1f}% in {r.n / per} x {r.name}\n")
    }
    print("TIMES {what}: {total / double(per):.2f} ms {tail}\n")
    delete rows
}
