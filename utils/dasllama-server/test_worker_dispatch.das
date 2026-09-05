options gen2
options stack = 524288
options _dasllama_internal = true

require dastest/testing_boost public
require openai_server
require dasllama/dasllama
require dasllama/dasllama_par
require daslib/jobque_boost

struct WorkerCheck {
    passed : bool
}

[test]
def test_media_worker_dispatch(t : T?) {
    with_job_que() {
        setup_dasllama_jobque()
        set_jobque_team_mode(false)
        with_channel(1) $(var channel) {
            new_thread() <| @capture(= channel) {
                var passed = false
                try {
                    configure_media_worker_dispatch(false)
                    let before = get_disp_count()
                    maybe_parallel_for(0, 4096, 4) $(rb, re) {
                        if (rb > re) { panic("Invalid dispatch bounds") }
                    }
                    passed = get_jobque_fork_pool() && get_disp_count() > before && !get_jobque_team_mode()
                } recover {
                    passed = false
                }
                channel |> push_clone(WorkerCheck(passed = passed))
                channel |> notify_and_release()
            }
            channel |> for_each_clone() $(result : WorkerCheck#) {
                t |> success(result.passed, "worker enables its fork pool, dispatches, and preserves the shared queue policy")
            }
        }
    }
}
