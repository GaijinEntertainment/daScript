options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama_common
require dasllama/dasllama_ple
require ?das_metal dasllama/dasllama_metal_shapes   // ffn_second_hidden — the width-servability unit

// The PLE go-live tripwire (ple_check_table) and the MatFormer width classifier
// (ffn_second_hidden): both decided from a synthetic Model, no weights on disk.

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
    t |> run("a short PLE table plane panics at go-live, per format arm; a full one passes") @(t : T?) {
        var mq <- ple_shell(KqFmt.q8)
        let need = mq.config.vocab_size * mq.config.n_layers * mq.config.n_embd_per_layer
        t |> success(trips(mq), "q8 fmt with an empty qblob trips")
        mq.qblob |> resize(need)
        t |> success(!trips(mq), "q8 fmt with the table resident passes")
        delete mq
        // every kq arm reads its own plane pair: a stride or plane mix-up must trip here, not
        // mid-gather (no k4/k6/q40 PLE carrier is stocked, so this unit is their only cover)
        for (fmt, qsb in [KqFmt.k4, KqFmt.k5, KqFmt.k6, KqFmt.q40], [K4_QSB, K5_QSB, K6_QSB, Q40_QSB]) {
            var mk <- ple_shell(fmt)
            t |> success(trips(mk), "{fmt} fmt with an empty plane trips")
            let sbytes = (need / 256l) * qsb
            if (fmt == KqFmt.k4) {
                mk.k4q |> resize(sbytes)
            } elif (fmt == KqFmt.k5) {
                mk.k5q |> resize(sbytes)
            } elif (fmt == KqFmt.k6) {
                mk.k6q |> resize(sbytes)
            } else {
                mk.q40q |> resize(sbytes)
            }
            t |> success(!trips(mk), "{fmt} fmt with the table resident passes")
            delete mk
        }
        // the exemption is a real branch: a non-PLE model keeps whatever offset it has, and the
        // check must not read the planes at all (a nonzero offset would trip if it did)
        var mn <- ple_shell(KqFmt.q8)
        mn.config.n_embd_per_layer = 0l
        mn.ple_emb_off = 4096l
        t |> success(!trips(mn), "a model with no per-layer embeddings is exempt")
        delete mn
    }
}

[test]
def test_ffn_second_hidden(t : T?) {
    t |> run("the MatFormer width classifier: uniform 0, two widths the second, three -1") @(t : T?) {
        static_if (typeinfo builtin_module_exists(das_metal)) {
            var m = Model()
            m.config.n_layers = 4l
            m.config.hidden_dim = 6144l
            t |> equal(ffn_second_hidden(m), 0l)   // empty ffn_w: uniform by config
            m.ffn_w <- [6144l, 6144l, 6144l, 6144l]
            t |> equal(ffn_second_hidden(m), 0l)
            m.ffn_w <- [6144l, 6144l, 12288l, 12288l]   // the E2B shape
            t |> equal(ffn_second_hidden(m), 12288l)
            m.ffn_w <- [12288l, 6144l, 6144l, 6144l]    // the odd width first
            t |> equal(ffn_second_hidden(m), 6144l)
            m.ffn_w <- [6144l, 6144l, 12288l, 8192l]    // a third width has no served form
            t |> equal(ffn_second_hidden(m), -1l)
            // the strict half of the decline: a batch-shaped caller refuses hetero widths
            // outright (no per-layer uniform there), where the single-stream caller admits them
            m.ffn_w <- [6144l, 6144l, 12288l, 12288l]
            m.config.dim = 2048l
            m.quant = QuantMode.q8
            t |> success(decode_shape_decline(m, DECODE_NEEDS_OK, true, true, false) != MetalDecodeDecline.none,
                "hetero widths decline when the caller cannot bind per layer")
        } else {
            feint("das_metal is not built on this platform - the width classifier is Metal-side\n")
        }
    }
}
