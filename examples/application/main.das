options gen2
options indenting = 4
options gc
options persistent_heap

// The canonical daslang application shape. Copy this file to start any
// long-running program — game, tool, server, simulation — with or without UI.
//
// Three exported lifecycle functions + one standalone driver:
//
//   init()      one-time setup
//   update()    one tick of work
//   shutdown()  teardown
//   main()      init -> loop(update + maybe_collect_gc) -> shutdown
//
// The same file runs two ways:
//
//   STANDALONE: daslang examples/application/main.das
//       main() drives the loop; maybe_collect_gc() is the per-iteration GC
//       boundary a standalone loop must provide (needs `options gc` +
//       `options persistent_heap` above; without a collect point a
//       long-running loop grows the heap without bound — interp and JIT alike).
//   LIVE:       daslang-live examples/application/main.das
//       the host calls init/update/shutdown itself (and collects each frame —
//       maybe_collect_gc self-disables). Edit this file while it runs: the
//       host recompiles and hot-swaps it; `@live` globals survive the reload,
//       everything else takes fresh source defaults.

require daslib/clargs
require daslib/fio
require live/live_gc
require live/live_vars
require live_host

// @live: preserved across live reloads (auto-serialized by live_vars).
// Watch it keep counting while you edit MESSAGE and save.
var @live g_ticks = 0

let MESSAGE = "tick"

[CommandLineArgs]
struct Args {
    @clarg_doc = "Standalone only: stop after this many ticks (0 = run until interrupted)"
    frames : int = 300

    @clarg_doc = "Standalone only: per-tick pacing in ms (0 = spin; a real app paces via its work, vsync, or a poll timeout)"
    tick_ms : int = 0
}

[export]
def init() {
    print("application init\n")
}

[export]
def update() {
    g_ticks ++
    if (g_ticks % 60 == 0) {
        print("{MESSAGE} {g_ticks}\n")
    }
}

[export]
def shutdown() {
    print("application shutdown after {g_ticks} tick(s)\n")
}

[export]
def main() {
    var parse_result <- parse_args(type<Args>)
    if (parse_result |> is_err) {
        print("command-line args: {parse_result |> unwrap_err}\n")
        return
    }
    let args <- parse_result |> move_unwrap
    init()
    while (args.frames == 0 || g_ticks < args.frames) {
        update()
        maybe_collect_gc()
        if (args.tick_ms > 0) {
            sleep(uint(args.tick_ms))
        }
    }
    shutdown()
}
