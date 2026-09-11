options gen2
options gc
options persistent_heap

require daslib/jobque_boost

// A thread lambda's capture outlives a collect the thread runs: the capture block's header names
// the capture's type, so the root walk reaches the captured array where a headerless block
// would have the walk read a null type.

[export]
def main {
    var data <- [for (i in range(64)); i]
    with_channel(1) $(ch) {
        new_thread() <| @capture(:= data) {
            unsafe(heap_collect(false, true))
            let churn <- [for (i in range(4096)); i]
            var sum = 0
            for (v in data) {
                sum += v
            }
            print("THREAD_COLLECT_SUM {sum}\n")
            ch |> notify_and_release()
        }
        ch |> join()
    }
    print("THREAD_COLLECT_DONE\n")
}
