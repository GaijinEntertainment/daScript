options gen2
options gc
options persistent_heap
require daslib/jobque_boost

struct Scratch { values : array<int> }
[export]
def main {
    var retained <- [for (i in range(64)); i]
    var iteration = 0
    var retained_text = ""
    with_channel(1) $(done) {
        new_thread_loop() @capture(:= retained, = iteration, = retained_text) : bool {
            if (iteration == 0) { set_gc_allocation_budget(1ul, 1ul); retained_text = "remember {iteration}" }
            if (iteration == 3) {
                verify(gc_collection_stats().x > 0ul)
                verify(gc_reclaimed_stats().x > 0ul)
                verify(gc_reclaimed_stats().y > 0ul)
                verify(retained_text == "remember 0")
                var sum = 0
                for (value in retained) { sum += value }
                verify(sum == 2016)
                done |> notify_and_release()
                return false
            }
            var orphan = new Scratch(values <- [for (i in range(4096)); i])
            orphan = null
            for (i in range(128)) { let discarded = "garbage {iteration} {i}"; verify(length(discarded) > 0) }
            iteration++
            return true
        }
        done |> join()
    }
    verify(retained[63] == 63)
    print("THREAD_LOOP_COLLECT_OK\n")
}
