options gen2
options indenting = 4
options _dasllama_internal = true

module _prefill_probe_fixture shared public

require dasllama/dasllama_common

//! best-of-`reps` wall ms of one `eval_` over `npos` synthetic tokens, each rep on a fresh session
def measure_one(tr : Model; npos : int64; reps : int) : double {
    var best = 1.0e30lf
    for (_r in range(reps)) {
        var s <- make_run_state(tr.config)
        var toks : array<int64>
        toks |> reserve(npos)
        for (i in range64(npos)) {
            toks |> push(100l + i % 5000l)
        }
        let t0 = ref_time_ticks()
        eval_(tr, s, toks)
        let ms = double(get_time_usec(t0)) / 1000.0lf
        if (ms < best) {
            best = ms
        }
        delete toks
        delete s
    }
    return best
}
