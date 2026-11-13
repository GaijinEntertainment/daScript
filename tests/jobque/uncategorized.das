options gen2

require dastest/testing_boost public
require jobque
require daslib/jobque_boost
require math
require daslib/fio
require daslib/clargs
require daslib/strings_boost
require daslib/runtime_memory

struct PinResult {
    pinned_on : int
    seen : int
}

[test(description = "The current processor index is -1 or valid, a negative pin is ignored, and a hard pin moves a dedicated thread")]
def test_current_thread_affinity(t : T?) {
    t |> run("processor index is -1 or a valid index") @@(t : T?) {
        let cpu = get_current_processor()
        t |> success(cpu >= -1, "index {cpu}")
    }
    t |> run("a negative index is ignored") @@(t : T?) {
        set_current_thread_affinity(-1, true)
        t |> success(get_current_processor() >= -1, "still running after a negative pin")
    }
    t |> run("a hard pin moves a dedicated thread to another processor") @@(t : T?) {
        if (get_current_processor() < 0) {
            t |> skip("{get_platform_name()}: no processor index to re-read")
            return
        }
        with_job_que() {
            let processors = min(get_total_hw_threads(), 64)
            if (processors < 2) {
                t |> skip("one processor: nowhere to move")
                return
            }
            with_channel(1) $(ch) {
                new_thread() @() {
                    let target = (get_current_processor() + 1) % processors
                    set_current_thread_affinity(target, true)
                    var acc = 0
                    for (i in range(100000)) {
                        acc += i & 7
                    }
                    feint("{acc}")
                    let seen = get_current_processor()
                    set_current_thread_affinity(target, false)
                    ch |> push_clone(PinResult(pinned_on = target, seen = seen))
                    ch |> notify_and_release
                }
                var pinned_on = -1
                var seen = -2
                ch |> for_each_clone() $(r : PinResult#) {
                    pinned_on = r.pinned_on
                    seen = r.seen
                }
                t |> equal(seen, pinned_on, "on {pinned_on} after a pinned loop")
            }
        }
    }
}

[test(description = "A thread loop collects its garbage at a native boundary, proven by a child run printing THREAD_LOOP_COLLECT_OK", timeout = 1200)]
def thread_steps_collect_at_a_native_boundary(t : T?) {
    var output : string
    let result = run_and_capture([get_host_binary(), "-dasroot", get_das_root(),
            "{get_das_root()}/tests/jobque/_fixture_thread_loop_collect.das"], output, 30.0)
    t |> equal(result, 0, output)
    t |> success(contains(output, "THREAD_LOOP_COLLECT_OK"), output)
}

[test(description = "context_memory counts an owned 1 MB allocation while it lives and releases it after delete")]
def test_context_memory_tracks_owned_allocations(t : T?) {
    let before = context_memory()
    var data : array<uint8>; resize(data, 1048576)
    let during = context_memory()
    t |> success(during.heap_used >= before.heap_used + 1048576ul)
    t |> success(during.heap_reserved >= during.heap_used)
    t |> success(during.strings_reserved >= during.strings_used)
    t |> success(during.stack > 0ul)
    delete data
    t |> success(context_memory().heap_used < during.heap_used)
    if (get_running_platform_name() == "emscripten") {
        let allocator = native_allocator_stats(); let extent = native_allocator_extent()
        t |> success(allocator.x > 0ul && extent.y > extent.x)
        t |> success(allocator.x + allocator.y <= extent.y - extent.x)
        t |> success(during.native_stack > 0ul)
    } else {
        t |> equal(native_allocator_stats(), urange64(0ul, 0ul))
    }
}

var owned_global_memory : int[128]
[test(description = "An idle fork context accounts its memory apart from its owner")]
def test_idle_fork_memory_is_separate_from_owner(t : T?) {
    owned_global_memory[0] = 42
    t |> equal(context_idle_forks().count, 0ul)
    set_jobque_threads(2)
    var results : array<int>; resize(results, 8)
    var destination = unsafe(addr(results[0]))
    with_job_que() {
        set_jobque_fork_pool(true, true)
        parallel_for(0, 8, 2) $(first, last, done) {
            var payload : array<uint8>; resize(payload, 4096)
            for (i in range(first, last)){unsafe(destination[i]) = i + length(payload)}
            delete payload
            done |> notify_and_release
        }
    }
    let idle = context_idle_forks()
    t |> success(idle.count > 0ul)
    t |> success(idle.memory.stack >= idle.count * 16384ul)
    t |> equal(idle.memory.globals, idle.count * context_memory().globals)
    t |> success(idle.memory.globals > 0ul)
    t |> equal(owned_global_memory[0], 42)
    t |> success(idle.memory.heap_reserved >= idle.memory.heap_used)
    t |> success(idle.memory.strings_reserved >= idle.memory.strings_used)
    t |> equal(idle.memory.native_stack, 0ul, "fork contexts are not native threads")
    for (v, i in results, count()){t |> equal(v, i + 4096)}
    delete results; set_jobque_threads(0)
}

//! the panic fixture's exit code and output, in `mode`
def run_panic_fixture(mode : string; var output : string&) : int {
    var args <- [get_host_binary(), "-dasroot", get_das_root(), "-no-module-cache",
        "{get_das_root()}/tests/jobque/_fixture_panic_parked_job.das"]
    if (!empty(mode)) {
        args |> push("--")
        args |> push(mode)
    }
    return run_and_capture(args, output, 60.0)
}

[test(description = "A panic in main with a job or a detached thread still parked on a channel exits with the panic code -2 instead of hanging the teardown, and a panic with nothing running tears down as before with the same code, proven by child runs", timeout = 1200)]
def panic_with_a_parked_job_exits(t : T?) {
    let panicCode = get_running_platform_name() == "windows" ? -2 : 254
    for (mode in ["", "thread"]) {
        t |> run(empty(mode) ? "a parked que job" : "a parked detached thread") @(t : T?) {
            var output : string
            let result = run_panic_fixture(mode, output)
            t |> equal(result, panicCode, output)
            t |> success(contains(output, "EXCEPTION: boom"), output)
            t |> success(contains(output, "exiting without teardown"), output)
        }
    }
    t |> run("nothing running: the regular teardown, no early exit") @(t : T?) {
        var output : string
        let result = run_panic_fixture("clean", output)
        t |> equal(result, panicCode, output)
        t |> success(contains(output, "EXCEPTION: boom"), output)
        t |> success(!contains(output, "exiting without teardown"), output)
    }
}

struct ArgCount {
    count : int
}

def argument_count() : int => length(get_command_line_arguments())

[test(description = "with_argv replaces the command line for the calling thread only: a job started inside the block still reads the process arguments, and nested blocks restore the outer one")]
def test_with_argv_is_per_thread(t : T?) {
    let process_count = argument_count()
    let outer <- [for (i in range(process_count + 2)); "outer{i}"]
    let inner <- [for (i in range(process_count + 5)); "inner{i}"]
    var seen_by_job = -1
    with_argv(outer) {
        t |> equal(process_count + 2, argument_count(), "the block's thread reads its own")
        with_argv(inner) {
            t |> equal(process_count + 5, argument_count(), "a nested block reads the inner ones")
        }
        t |> equal(process_count + 2, argument_count(), "the nested block restored the outer")
        with_job_que() {
            with_channel(1) $(ch) {
                with_wait_group(1) $(wg) {
                    new_job() @() {
                        ch |> push_clone(ArgCount(count = argument_count()))
                        ch |> notify_and_release
                        wg |> done
                    }
                }
                ch |> for_each_clone() $(val : ArgCount#) {
                    seen_by_job = val.count
                }
            }
        }
    }
    t |> equal(process_count, seen_by_job, "a job started inside the block reads the process ones")
    t |> equal(process_count, argument_count(), "leaving the block restores the process arguments")
}
