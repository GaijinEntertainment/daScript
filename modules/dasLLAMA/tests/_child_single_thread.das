options gen2
options _dasllama_internal = true

module _child_single_thread shared

require dasllama/dasllama_core  // the flag is read back through its own getter here
require dasllama/dasllama_par   // maybe_parallel_for + the dispatch counters the arms move
require daslib/jobque_boost

// The test child's `single-thread` verb, which test_par_indexed.das spawns to reach what no in-process cell can: DASLLAMA_SINGLE_THREAD
// is read once at context init, so only a child started with it set or unset exercises that path. It
// dispatches three rounds on a CONFIGURED queue, each of which would thread but for the flag, and
// prints what the context made of them - so the announce's once-per-context guard is observable too.

def public child_single_thread() {
    with_dasllama_jobque() {
        for (_round in range(3)) {
            maybe_parallel_for(0, 4096, 4) $(rb, re) {
                if (rb > re) {
                    print("never: {rb} {re}\n")
                }
            }
        }
        print("SINGLE={get_single_thread()} inline={get_disp_inline()} disp={get_disp_count()}\n")
    }
    print("ROOT_DONE\n")
}
