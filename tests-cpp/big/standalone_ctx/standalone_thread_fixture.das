options gen2

require daslib/jobque_boost

//! a standalone context runs a thread: `new_thread` clones this context, the worker pushes five
//! blobs onto a stream and the caller pops them - the sum proves every push crossed
[export]
def threaded_sum() : int {
    var total = 0
    with_job_que() {
        var s = unsafe(stream_create())
        s |> append(1)
        new_thread() <| @capture(= s) {
            for (i in range(5)) {
                let b = [uint8(i * 10)]
                s |> push(b)
            }
            s |> notify_and_release
        }
        var got = 0
        var tries = 0
        while (got < 5 && tries < 50) {
            tries++
            s |> pop_with_timeout(1000) $(view : array<uint8># -const) {
                if (!empty(view)) {
                    total += int(view[0])
                    got++
                }
            }
        }
        s |> release
    }
    return total
}
