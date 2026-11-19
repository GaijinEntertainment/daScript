options gen2
options _dasllama_internal = true

module _child_jobque_tripwire shared

require dasllama/dasllama_par
require daslib/jobque_boost

// the bare-queue fixture test_jobque_tripwire.das spawns: one engine-shaped dispatch on a queue
// nobody configured, then a marker the tripwire must keep off the output when it panics

def public child_jobque_tripwire() {
    with_job_que() {
        maybe_parallel_for(0, 4096, 4) $(rb, re) {
            if (rb > re) {
                print("never: {rb} {re}\n")
            }
        }
    }
    print("TRIPWIRE_DONE\n")
}
