// Heartbeat — Periodic Callback Injection
//
// Demonstrates `daslib/heartbeat` — a compile-time macro that
// automatically injects `heartbeat()` calls into every function
// body and every loop (for/while).  This lets you run a callback
// periodically even inside long-running or infinite loops.
//
// Use cases:
//   - Cooperative multitasking (yield to scheduler)
//   - Watchdog / timeout detection (break infinite loops)
//   - Progress reporting during heavy computation
//   - Frame budget enforcement in game loops
//
// How it works:
//   1. `require daslib/heartbeat` activates a macro pass that
//      adds `heartbeat()` calls to function entries and loop bodies
//   2. `set_heartbeat(callback)` registers a lambda to be called
//      on each injected heartbeat point
//   3. The callback fires on every function call and every loop
//      iteration — use a counter to throttle expensive work
//
// Note: lambda captures are by-copy for value types, so shared
// counters and flags must be module-level variables.
//
// Run: daslang.exe examples/debugapi/heartbeat.das

options gen2

require daslib/heartbeat

// Module-level state shared between heartbeat callbacks and code.
// Lambda captures copy value types, so locals can't be shared
// with the callback — use module-level vars instead.
var hb_count : int = 0
var hb_should_stop : bool = false

// ============================================================
// Example 1: Watchdog for infinite loops
// ============================================================
// The heartbeat fires on every loop iteration.  After a
// threshold, the callback sets a flag that the loop checks.

def demo_infinite_loop_watchdog() {
    print("=== heartbeat: watchdog for infinite loops ===\n")

    hb_count = 0
    hb_should_stop = false
    let max_iterations = 50

    set_heartbeat() @ {
        hb_count++
        if (hb_count >= max_iterations) {
            hb_should_stop = true
        }
    }

    // Without the heartbeat watchdog, this runs forever.
    // The heartbeat macro injects `heartbeat()` at the top of
    // the loop body, so our callback fires every iteration.
    var sum = 0
    while (!hb_should_stop) {
        sum += 1
    }
    print("  watchdog stopped loop after {hb_count} heartbeats, sum={sum}\n")
}

// ============================================================
// Example 2: Progress reporting during computation
// ============================================================
// Throttle the heartbeat callback to report every N beats.

def heavy_computation() : int {
    var result = 0
    for (i in range(1000)) {
        result += i * i
    }
    return result
}

def demo_progress_reporting() {
    print("\n=== heartbeat: progress reporting ===\n")

    hb_count = 0

    set_heartbeat() @ {
        hb_count++
        if (hb_count % 200 == 0) {
            print("  ... {hb_count} heartbeats so far\n")
        }
    }

    let result = heavy_computation()
    print("  computation result = {result}\n")
    print("  total heartbeats   = {hb_count}\n")
}

// ============================================================
// Example 3: Nested function calls
// ============================================================
// Heartbeat fires at the start of EVERY function body,
// not just loops.

def inner() {
    // heartbeat() is injected here automatically
    pass
}

def middle() {
    // heartbeat() is injected here automatically
    inner()
    inner()
}

def outer() {
    // heartbeat() is injected here automatically
    middle()
}

def demo_nested_calls() {
    print("\n=== heartbeat: nested function calls ===\n")

    hb_count = 0
    set_heartbeat() @ {
        hb_count++
    }

    outer()
    // outer(1) + middle(1) + inner(1) + inner(1) = 4
    // (plus heartbeats from other function entries)
    print("  heartbeats from outer->middle->inner: {hb_count}\n")
}

// ============================================================
// Main
// ============================================================

[export]
def main() {
    demo_infinite_loop_watchdog()
    demo_progress_reporting()
    demo_nested_calls()

    // Clear the heartbeat by setting a no-op callback
    set_heartbeat() @ {
        pass
    }
}
