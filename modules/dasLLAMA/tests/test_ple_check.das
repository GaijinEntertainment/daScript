options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options _dasllama_internal = true
options persistent_heap   // a dastest root frees what it deletes only on this heap; the linear heap holds every carrier a cell deleted until the context ends

require dastest/testing_boost public
require dasllama/dasllama_common
require dasllama/dasllama_ple
require dasllama/dasllama_gguf   // GGUFMeta / GGUFTensorInfo - the per-layer planes' disk types for the format detection
require dasllama/dasllama_load   // detect_ple_formats - the per-layer planes' tags off the file's types
require ?das_metal dasllama/dasllama_metal_shapes   // ffn_second_hidden — the width-servability unit
require math
require strings

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
            mk.kq[int(fmt)].q |> resize(sbytes)
            t |> success(!trips(mk), "{fmt} fmt with the table resident passes")
            delete mk
        }
        // the per-32 arms read their own top-level plane pair at the 32-block stride
        for (fmt in [KqFmt.q51, KqFmt.q5032]) {
            var mb <- ple_shell(fmt)
            t |> success(trips(mb), "{fmt} fmt with an empty plane trips")
            let qbytes = (need / KQ_B32_ELEMS) * kq_qsb(fmt)
            if (fmt == KqFmt.q51) {
                mb.q51q |> resize(qbytes - 1l)
            } else {
                mb.q5032q |> resize(qbytes - 1l)
            }
            t |> success(trips(mb), "{fmt} fmt one byte short of the table trips")
            if (fmt == KqFmt.q51) {
                mb.q51q |> resize(qbytes)
            } else {
                mb.q5032q |> resize(qbytes)
            }
            t |> success(!trips(mb), "{fmt} fmt with the table resident passes")
            delete mb
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
def test_gemma4_router_rows(t : T?) {
    t |> run("a prefill row of the gemma4 router input is bit-identical to the decode row (npos = 1)") @(t : T?) {
        let dim = 256l
        let npos = 5l
        var m = Model()
        m.config.dim = dim
        m.config.norm_eps = 1.0e-6
        m.moe_router_scale_off = 32l
        m.fblob |> resize(m.moe_router_scale_off + 2l * dim)   // two layers of scale rows
        for (i in range64(2l * dim)) {
            m.fblob[m.moe_router_scale_off + i] = 0.5 + 0.01 * float(i % 37l)
        }
        var src : array<float>
        src |> resize(npos * dim)
        for (i in range64(npos * dim)) {
            src[i] = sin(float(i) * 0.37) * (1.0 + float(i / dim))
        }
        var batch : array<float>
        batch |> resize(npos * dim)
        gemma4_router(m, batch, src, 1l, dim, npos)
        var row, one : array<float>
        row |> resize(dim)
        one |> resize(dim)
        var bad = 0
        for (p in range64(npos)) {
            for (i in range64(dim)) {
                row[i] = src[p * dim + i]
            }
            gemma4_router(m, one, row, 1l, dim, 1l)
            for (i in range64(dim)) {
                if (one[i] != batch[p * dim + i]) {
                    bad++
                }
            }
        }
        t |> equal(0, bad, "every element of every row matches the single-row call")
        t |> success(batch[1] != 0.0 && batch[dim + 1l] != batch[1], "the rows carry distinct non-zero values (the compare is not vacuous)")
        delete m
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

//! a two-layer E-series shell's per-layer gate and projection planes on disk: `gate` and `proj` the ggml types of layer 0 and 1
def private ple_planes_meta(gate, proj : array<int>) : GGUFMeta {
    return <- GGUFMeta(tensors <- [
        GGUFTensorInfo(name = "blk.0.inp_gate.weight", ggml_type = gate[0], n_elem = 256l * 256l),
        GGUFTensorInfo(name = "blk.0.proj.weight", ggml_type = proj[0], n_elem = 256l * 256l),
        GGUFTensorInfo(name = "blk.1.inp_gate.weight", ggml_type = gate[1], n_elem = 256l * 256l),
        GGUFTensorInfo(name = "blk.1.proj.weight", ggml_type = proj[1], n_elem = 256l * 256l)])
}

[test]
def test_ple_plane_formats(t : T?) {
    t |> run("a per-layer plane kind whose every layer ships F32 keeps the f32 rows, and a kind mixing F32 with another type declines the load") @(t : T?) {
        var shell <- ple_shell(KqFmt.q8)
        shell.config.dim = 256l
        var f32 <- ple_planes_meta([GGML_TYPE_F32, GGML_TYPE_F32], [GGML_TYPE_F32, GGML_TYPE_F32])
        t |> success(!detect_ple_formats(shell, f32), "an all-F32 file tags every plane q8 (the f32 rows' slot)")
        t |> success(shell.ple_gate_f32 && shell.ple_proj_f32, "and marks both kinds f32")
        var kq <- ple_planes_meta([GGML_TYPE_Q6_K, GGML_TYPE_Q6_K], [GGML_TYPE_F32, GGML_TYPE_F32])
        t |> success(detect_ple_formats(shell, kq), "a Q6_K gate stack tags its own format")
        t |> success(!shell.ple_gate_f32 && shell.ple_proj_f32, "the Q6_K kind is not f32, the F32 one is")
        t |> equal(shell.ple_gate_fmt[1], KqFmt.k6, "the gate's tag is the file's")
        var mixed <- ple_planes_meta([GGML_TYPE_F32, GGML_TYPE_Q8_0], [GGML_TYPE_F32, GGML_TYPE_F32])
        let why = panic_text() ${
            detect_ple_formats(shell, mixed)
        }
        t |> success(find(why, "inp_gate planes mix F32 with other types - tensor 'blk.1.inp_gate.weight' is Q8_0, 'blk.0.inp_gate.weight' is F32") >= 0,
            "a gate stack of one F32 and one Q8_0 layer declines naming the differing tensor and both types: {why}")
        var mixed_proj <- ple_planes_meta([GGML_TYPE_Q8_0, GGML_TYPE_Q8_0], [GGML_TYPE_Q6_K, GGML_TYPE_F32])
        let why_proj = panic_text() ${
            detect_ple_formats(shell, mixed_proj)
        }
        t |> success(find(why_proj, "proj planes mix F32 with other types - tensor 'blk.1.proj.weight' is F32, 'blk.0.proj.weight' is Q6_K") >= 0,
            "and a projection stack the same, whichever side layer 0 takes: {why_proj}")
        delete mixed_proj
        delete mixed
        delete kq
        delete f32
        delete shell
    }
}
