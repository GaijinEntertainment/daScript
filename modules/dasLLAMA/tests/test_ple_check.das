options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama_common

// The PLE go-live tripwire (ple_check_table): a carrier whose table plane is short — a stale
// pre-rail image, a mis-routed load — must fail AT LOAD, per format arm. Model-free: synthetic
// Model shells, no weights.

def private ple_shell(fmt : KqFmt) : Model {
    var m = Model()
    m.config.vocab_size = 4l
    m.config.n_layers = 2l
    m.config.n_embd_per_layer = 256l
    m.ple_emb_fmt = fmt
    return <- m
}

def private trips(m : Model) : bool {
    var tripped = false
    try {
        ple_check_table(m, "synthetic")
    } recover {
        tripped = true
    }
    return tripped
}

[test]
def test_ple_check_table(t : T?) {
    t |> run("a short PLE table plane panics at go-live, a full one passes, non-PLE is exempt") @(t : T?) {
        let need = 4l * 2l * 256l   // vocab * layers * ple
        var mq <- ple_shell(KqFmt.q8)
        t |> success(trips(mq), "q8 fmt with an empty qblob trips")
        mq.qblob |> resize(need)
        t |> success(!trips(mq), "q8 fmt with the table resident passes")
        delete mq
        var mk <- ple_shell(KqFmt.k5)
        t |> success(trips(mk), "k5 fmt with an empty k5 plane trips")
        mk.k5q |> resize((need / 256l) * K5_QSB)
        t |> success(!trips(mk), "k5 fmt with the table resident passes")
        delete mk
        var mn <- ple_shell(KqFmt.q8)
        mn.config.n_embd_per_layer = 0l   // not an E-series model: the check is a no-op
        t |> success(!trips(mn), "a non-PLE model is exempt")
        delete mn
    }
}
