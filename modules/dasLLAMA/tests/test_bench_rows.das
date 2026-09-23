options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama_common
require dasllama/dasllama_load
require dasllama/dasllama_bench
require dasllama/dasllama_math   // setup_dasllama_jobque_
require dasllama/dasllama_env
require daslib/jobque_boost
require daslib/fio

// The bench rows' contracts on a real model (stories15M, the llama2.c checkpoint): what the
// released exe and the server's /bench run per row, held on the smallest model that runs them.

def private llama2c_dir() : string {
    let env = g_env_test.llama2c_dir
    return empty(env) ? "/Users/borisbatkin/Work/llama2.c" : env
}
let MODEL_PATH = path_join(llama2c_dir(), "stories15M.bin")

// a pp row on a session sized to `seq_len`: under the prompt it must refuse before it writes,
// at or over it the row runs
def private pp_row_panics(var tr : Model; seq_len, plen : int64; warm : bool) : bool {
    tr.config.seq_len = seq_len
    var s <- make_run_state(tr.config)
    var tripped = false
    try {
        if (warm) {
            bench_pp_warm(tr, s, plen)
        } else {
            bench_pp_rep(tr, s, plen, 1l)
        }
    } recover {
        tripped = true
    }
    delete s
    return tripped
}

[test]
def test_pp_rows_capacity(t : T?) {
    t |> run("the pp rows refuse a session sized under their prompt (needs stories15M)") @(t : T?) {
        if (!stat(MODEL_PATH).is_valid) {
            t |> skip("stories15M.bin not present at {MODEL_PATH} (set DASLLAMA_LLAMA2C_DIR)")
            return
        }
        var tr <- load_checkpoint(MODEL_PATH)
        with_job_que() {
            setup_dasllama_jobque_()
            t |> success(!pp_row_panics(tr, 16l, 8l, true), "a session that fits runs the warmup")
            t |> success(pp_row_panics(tr, 4l, 8l, true), "the warmup prefill checks the cap before it writes")
            t |> success(pp_row_panics(tr, 4l, 8l, false), "a timed rep checks the cap before it writes")
        }
    }
}
