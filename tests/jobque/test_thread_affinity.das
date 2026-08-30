options gen2
require dastest/testing_boost
require jobque
require math

[test]
def test_current_thread_affinity(t : T?) {
    t |> run("processor index is -1 or a valid index") @@(t : T?) {
        let cpu = get_current_processor()
        t |> success(cpu >= -1, "index {cpu}")
    }
    t |> run("a hard pin keeps the thread on that processor") @@(t : T?) {
        let cpu = get_current_processor()
        if (cpu < 0 || get_platform_name() != "windows") {
            t |> skip("hard pins are verified where a new thread does not inherit the calling thread's mask")
            return
        }
        set_current_thread_affinity(cpu, true)
        var acc = 0
        for (i in range(100000)) {
            acc += i & 7
        }
        feint("{acc}")
        t |> equal(get_current_processor(), cpu, "still on {cpu} after a pinned loop")
    }
    t |> run("the hint mode does not throw") @@(t : T?) {
        set_current_thread_affinity(max(get_current_processor(), 0), false)
        t |> success(true)
    }
}
