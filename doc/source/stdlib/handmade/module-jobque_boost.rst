The JOBQUE_BOOST module provides high-level job queue abstractions built on
the low-level ``jobque`` primitives. It includes ``with_job``, ``with_job_status``,
and channel-based patterns for simplified concurrent programming.

All functions and symbols are in "jobque_boost" module, use require to get access to it. ::

    require daslib/jobque_boost

Example: ::

    require daslib/jobque_boost

        [export]
        def main() {
            with_job_status(1) <| $(status) {
                new_thread <| @() {
                    print("from thread\n")
                    status |> notify_and_release()
                }
                status |> join()
                print("thread done\n")
            }
        }
        // output:
        // from thread
        // thread done
