options gen2
options indenting = 4
options _dasllama_internal = true

module _prefill_probe_fixture shared public

require dasllama/dasllama_common

var private g_probe_text : array<int64>

//! Serve the probes' windows from `toks` (a real prompt, repeated past its end) instead of the
//! synthetic ids; an empty array returns to the synthetic ones.
def probe_set_text(toks : array<int64>) {
    g_probe_text := toks
}

//! tokens [at, at + n) of the probe's prompt: the text set by `probe_set_text`, else ids that walk
//! 100 .. 5099. The synthetic ids route a MoE onto half its experts; a real prompt spreads them.
def probe_tokens(at, n : int64) : array<int64> {
    let nt = long_length(g_probe_text)
    return <- [for (i in range64(n)); nt > 0l ? g_probe_text[(at + i) % nt] : 100l + (at + i) % 5000l]
}

//! best-of-`reps` wall ms of one `eval_` over the probe prompt's first `npos` tokens, each rep on a fresh session
def measure_one(tr : Model; npos : int64; reps : int) : double {
    var best = 1.0e30lf
    for (_r in range(reps)) {
        var s <- make_run_state(tr.config)
        var toks <- probe_tokens(0l, npos)
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
