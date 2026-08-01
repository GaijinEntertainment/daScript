options gen2
options persistent_heap
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)

require dasllama/dasllama_common
require dasllama/dasllama_math
require daslib/jobque_boost
require daslib/fio

def bench(tr : Model; toks : array<int64>; dt : KVDtype) {
    var s <- make_run_state(tr.config, dt, dt)
    let t0 = ref_time_ticks()
    eval_(tr, s, toks)
    let us = get_time_usec(t0)
    print("{dt}: 2048-tok prefill in {us / 1000}ms -> {1000000.0 * 2048.0 / float(us)} tok/s\n")
    delete s
}

[export]
def main {
    let env = get_env_variable("DASLLAMA_MODELS_DIR")
    let mdir = empty(env) ? "D:/Work/llama.cpp/models/" : env
    var tr <- load_gguf(path_join(mdir, "Qwen3-4B-Instruct-2507-Q4_K_M.gguf"), QuantMode.q8)
    tr.config.seq_len = 4096l
    var toks : array<int64>
    toks |> reserve(2048)
    for (i in range64(2048l)) {
        toks |> push(100l + i % 5000l)
    }
    with_job_que() {
        setup_dasllama_jobque_()
        bench(tr, toks, KVDtype.f16)
        bench(tr, toks, KVDtype.q8_0)
        bench(tr, toks, KVDtype.tq4)
    }
    delete toks
    delete tr
}
