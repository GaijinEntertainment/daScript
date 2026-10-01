options gen2
options _dasllama_internal = true

require dasllama/dasllama_par
require dasllama/dasllama_math
require daslib/jobque_boost

// the team-job panic fixture test_jobque_tripwire.das spawns: one engine-shaped dispatch on the configured queue whose
// chunk panics on a worker, then a marker the exit must keep off the output

[export]
def main() {
    with_dasllama_jobque() {
        maybe_parallel_for(0, 4096, 4) $(rb, re) {
            if (rb >= 0 && re > rb) {
                panic("TEAM_PANIC at chunk {rb}")
            }
        }
    }
    print("TEAM_DONE\n")
}
