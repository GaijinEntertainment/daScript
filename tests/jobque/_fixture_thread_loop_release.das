options gen2
options gc
options persistent_heap
require daslib/jobque_boost
require daslib/clargs
[export]
def main {
    let args <- get_cli_arguments()
    let nested = !empty(args) && args[0] == "nested"
    with_channel(1) $(done) {
        new_thread_loop() @() : bool {
            if (nested) {
                var inner <- @ { verify(done != null) }
                invoke(inner)
                unsafe { delete inner }
                done |> notify_and_release()
            }
            return false
        }
        done |> join()
    }
}
