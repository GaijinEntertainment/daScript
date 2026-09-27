options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama_transformer   // the umbrella: hc_mix_row / hc_combine_row + the qwen4exp registration
require dasllama/dasllama_arch_qwen4exp   // ngram_gather
require dasllama/dasllama_kqformat   // GGML_TYPE_* + iq4nl_lut
require dasllama/dasllama_math   // rmsnorm - the reference's grouped norm
require math

// Model-free gates for the qwen4exp mechanisms: the hyper-connection mixer and scatter against a
// scalar reference on a synthetic fp32 model, and the n-gram hash gather (the hash, the EOS cut,
// the missing-predecessor rule, the three row decoders) on a hand-built table.

let private DIM = 32l
let private HC = 4l
let private LR = 32l
let private HCD = HC * DIM

def private lcg(var seed : uint&) : float {
    seed = seed * 1664525u + 1013904223u
    return float(seed >> 9u) / float(1u << 23u) - 0.5
}

//! a tiny hyper-connection model in fp32 mode: the mixer's four weights at fixed offsets
def private hc_fixture(var t : Model) {
    let c = Config(dim = DIM, hidden_dim = DIM, n_layers = 1l, n_heads = 1l, n_kv_heads = 1l, head_size = DIM, kv_dim = DIM,
        vocab_size = 16l, seq_len = 8l, hyper_conn = true, hc_count = HC, hc_low_rank = LR)
    t.config = c
    t.quant = QuantMode.fp32
    t.fblob |> resize(HCD + HC * HCD)
    t.wblob |> resize(LR * HCD + HCD * LR)
    var seed = 12345u
    for (i in range64(long_length(t.fblob))) {
        t.fblob[i] = i < HCD ? 1.0 + 0.5 * lcg(seed) : lcg(seed)
    }
    for (i in range64(long_length(t.wblob))) {
        t.wblob[i] = lcg(seed) * 0.2
    }
}

def private sigmoid(x : float) : float => 1.0 / (1.0 + exp(-x))

//! the mixer written out: grouped norm, down, silu(lo / hc), up, sigmoid-gated mean, inject
def private hc_mix_reference(t : Model; res : array<float>; var mixed : array<float>; var inject : array<float>) {
    let c = t.config
    var xn : array<float>
    xn |> resize(HCD)
    unsafe {
        for (ch in range64(HC)) {
            rmsnorm(addr(xn[ch * DIM]), addr < float const? >(res[ch * DIM]), addr < float const? >(t.fblob[ch * DIM]), DIM, c.norm_eps)
        }
    }
    var lo : array<float>
    lo |> resize(LR)
    for (j in range64(LR)) {
        var acc = 0.0
        for (i in range64(HCD)) {
            acc += t.wblob[j * HCD + i] * xn[i]
        }
        let v = acc / float(HC)
        lo[j] = v * sigmoid(v)
    }
    var gate : array<float>
    gate |> resize(HCD)
    for (k in range64(HCD)) {
        var acc = 0.0
        for (j in range64(LR)) {
            acc += t.wblob[LR * HCD + k * LR + j] * lo[j]
        }
        gate[k] = acc
    }
    mixed |> resize(DIM)
    for (i in range64(DIM)) {
        var acc = 0.0
        for (ch in range64(HC)) {
            acc += xn[ch * DIM + i] * sigmoid(gate[ch * DIM + i])
        }
        mixed[i] = acc / float(HC)
    }
    inject |> resize(HC)
    for (ch in range64(HC)) {
        var acc = 0.0
        for (i in range64(HCD)) {
            acc += t.fblob[HCD + ch * HCD + i] * xn[i]
        }
        inject[ch] = acc
    }
    delete xn
    delete lo
    delete gate
}

def private maxdiff(a, b : array<float>; n : int64) : float {
    var m = 0.0
    for (i in range64(n)) {
        m = max(m, abs(a[i] - b[i]))
    }
    return m
}

[test]
def test_hc_mixer(t : T?) {
    t |> run("hc_mix_row: the sigmoid-gated stream mean and the scatter logits match the scalar reference") @(t : T?) {
        var m = Model()
        hc_fixture(m)
        var s <- make_run_state(m.config)
        t |> equal(long_length(s.hc_res), HCD)
        var seed = 777u
        for (i in range64(HCD)) {
            s.hc_res[i] = lcg(seed) * 4.0
        }
        var want_mixed : array<float>
        var want_inject : array<float>
        hc_mix_reference(m, s.hc_res, want_mixed, want_inject)
        hc_mix_row(m, s, s.hc_res, 0l, 0l, 0l, LR * HCD, HCD)
        let dm = maxdiff(s.xb, want_mixed, DIM)
        let di = maxdiff(s.hc_inject, want_inject, HC)
        t |> success(dm < 1.0e-5, "mixed row within 1e-5 of the reference (maxdiff {dm})")
        t |> success(di < 1.0e-4, "inject logits within 1e-4 of the reference (maxdiff {di})")
        var nz = 0
        for (i in range64(DIM)) {
            if (abs(s.xb[i]) > 1.0e-3) {
                nz++
            }
        }
        t |> success(int64(nz) > DIM / 2l, "the mixed row is not degenerate ({nz} of {DIM} live)")
        // the head mixer (no inject) leaves the scatter logits alone
        s.hc_inject[0] = 123.0
        hc_mix_row(m, s, s.hc_res, 0l, 0l, 0l, LR * HCD, -1l)
        t |> equal(s.hc_inject[0], 123.0)
        delete want_mixed
        delete want_inject
        delete s
        delete m
    }
    t |> run("hc_combine_row: res_c += out * 2 sigmoid(inject_c / hc); a zero logit is a plain residual add") @(t : T?) {
        var m = Model()
        hc_fixture(m)
        var s <- make_run_state(m.config)
        var out : array<float>
        out |> resize(DIM)
        for (i in range64(DIM)) {
            out[i] = float(i) * 0.25
            for (ch in range64(HC)) {
                s.hc_res[ch * DIM + i] = float(ch)
            }
        }
        var inj <- [0.0, 4.0, -4.0, 40.0]
        unsafe {
            hc_combine_row(m, s.hc_res, 0l, addr < float const? >(out[0]), addr < float const? >(inj[0]))
        }
        var bad = 0
        for (i in range64(DIM)) {
            for (ch in range64(HC)) {
                let w = 2.0 * sigmoid(inj[ch] / float(HC))
                if (abs(s.hc_res[ch * DIM + i] - (float(ch) + out[i] * w)) > 1.0e-5) {
                    bad++
                }
            }
        }
        t |> success(bad == 0, "every stream took its own scatter weight ({bad} mismatches)")
        t |> success(abs(s.hc_res[5l] - (0.0 + out[5])) < 1.0e-6, "stream 0 (zero logit) is the plain residual add")
        delete out
        delete inj
        delete s
        delete m
    }
}

// ===== the n-gram gather =====

let private HD = 32l   // the hash table's row width (one 32-block per row)

//! a table of `rows` F32 rows, row r = r + 0.001 i; hash constants: multipliers 3/5/7,
//! heads_per_ngram 2 over ngram 3 (bigram heads 0-1, trigram heads 2-3), head h owns rows [50h, 50h + 50)
def private ngram_fixture(var t : Model; eos : int64) {
    let c = Config(dim = DIM, hidden_dim = DIM, n_layers = 1l, n_heads = 1l, n_kv_heads = 1l, head_size = DIM, kv_dim = DIM,
        vocab_size = 16l, seq_len = 8l, hyper_conn = true, hc_count = HC, hc_low_rank = LR,
        ple_layer = 0l, ple_ngram_size = 3l, ple_heads_per_ngram = 2l, ple_head_dim = HD, ple_conv_kernel = 4l, ple_eos_token = eos)
    t.config = c
    t.quant = QuantMode.fp32
    t.ngram_type = GGML_TYPE_F32
    t.ngram_rows = 200l
    t.ngram_row_bytes = HD * 4l
    t.ngram_tab |> resize(t.ngram_rows * t.ngram_row_bytes)
    unsafe {
        var fp = addr<float?>(t.ngram_tab[0])
        for (r in range64(t.ngram_rows)) {
            for (i in range64(HD)) {
                fp[r * HD + i] = float(r) + 0.001 * float(i)
            }
        }
    }
    t.ple_mult <- [3ul, 5ul, 7ul]
    t.ple_head_off <- [0l, 50l, 100l, 150l]
    t.ple_head_vocab <- [50l, 50l, 50l, 50l]
}

//! the row each head landed on, read back off the gathered F32 values
def private gathered_rows(dst : array<float>) : array<int64> {
    return <- [for (h in range64(4l)); int64(dst[h * HD] + 0.5)]
}

[test]
def test_ngram_gather(t : T?) {
    t |> run("the hash: bigram heads XOR two scaled tokens, trigram heads three; row = mixed % vocab + offset") @(t : T?) {
        var m = Model()
        ngram_fixture(m, 7l)
        var s <- make_run_state(m.config)
        var dst : array<float>
        dst |> resize(4l * HD)
        var toks <- [4l, 9l, 2l]
        unsafe {
            ngram_gather(m, s, toks, 2l, addr(dst[0]))
        }
        // ctx = [2, 9, 4]: bigram 2*3 ^ 9*5 = 43; trigram 43 ^ 4*7 = 43 ^ 28 = 55
        var rows <- gathered_rows(dst)
        t |> equal(rows[0], 43l)
        t |> equal(rows[1], 93l)
        t |> equal(rows[2], 105l)   // (43 ^ 28) % 50 = 5, + 100
        t |> equal(rows[3], 155l)
        t |> success(abs(dst[HD - 1l] - (43.0 + 0.031)) < 1.0e-4, "the whole row came along (last element {dst[HD - 1l]})")
        delete rows
        delete toks
        delete dst
        delete s
        delete m
    }
    t |> run("an EOS predecessor reads as EOS and cuts everything older; the token's own EOS cuts nothing") @(t : T?) {
        var m = Model()
        ngram_fixture(m, 7l)
        var s <- make_run_state(m.config)
        var dst : array<float>
        dst |> resize(4l * HD)
        var toks <- [4l, 7l, 9l, 2l]
        unsafe {
            ngram_gather(m, s, toks, 3l, addr(dst[0]))
        }
        // ctx = [2, 9, eos]: the eos two back replaces 4; bigram 6 ^ 45 = 43; trigram 43 ^ 49 = 26
        var rows <- gathered_rows(dst)
        t |> equal(rows[0], 43l)
        t |> equal(rows[2], 126l)
        delete rows
        var toks2 <- [4l, 9l, 7l]
        unsafe {
            ngram_gather(m, s, toks2, 2l, addr(dst[0]))
        }
        // ctx = [eos, 9, 4]: the token's own eos keeps its predecessors; bigram 21 ^ 45 = 56 -> row 6 / 56; trigram 56 ^ 28 = 36
        var rows2 <- gathered_rows(dst)
        t |> equal(rows2[0], 6l)
        t |> equal(rows2[1], 56l)
        t |> equal(rows2[2], 136l)
        delete rows2
        delete toks
        delete toks2
        delete dst
        delete s
        delete m
    }
}

[test]
def test_ngram_window(t : T?) {
    t |> run("a missing predecessor reads as EOS: the session window feeds the batch's first rows, -1 slots cut") @(t : T?) {
        var m = Model()
        ngram_fixture(m, 7l)
        var s <- make_run_state(m.config)
        t |> equal(long_length(s.ple_prev), 2l)
        t |> equal(s.ple_prev[0], -1l)
        var dst : array<float>
        dst |> resize(4l * HD)
        var toks <- [8l]
        unsafe {
            ngram_gather(m, s, toks, 0l, addr(dst[0]))
        }
        // an empty window: ctx = [8, eos, eos]: bigram 24 ^ 35 = 59 -> 9; trigram 59 ^ 49 = 10
        var rows <- gathered_rows(dst)
        t |> equal(rows[0], 9l)
        t |> equal(rows[2], 110l)
        delete rows
        s.ple_prev[1] = 5l   // the newest window slot is the immediate predecessor; the older one is still missing
        unsafe {
            ngram_gather(m, s, toks, 0l, addr(dst[0]))
        }
        // ctx = [8, 5, eos]: bigram 24 ^ 25 = 1; trigram 1 ^ 49 = 48
        var rows2 <- gathered_rows(dst)
        t |> equal(rows2[0], 1l)
        t |> equal(rows2[2], 148l)
        delete rows2
        delete toks
        delete dst
        delete s
        delete m
    }
}

[test]
def test_ngram_row_decoders(t : T?) {
    t |> run("the Q8_0 and IQ4_NL row decoders: f16 scale x int8 / x the nibble codebook") @(t : T?) {
        var m = Model()
        ngram_fixture(m, 0l)
        // one head over two rows (mixed % 2): multipliers 1, heads_per_ngram 1 over ngram 2
        m.config.ple_ngram_size = 2l
        m.config.ple_heads_per_ngram = 1l
        m.ple_mult <- [1ul, 1ul]
        m.ple_head_off <- [0l]
        m.ple_head_vocab <- [2l]
        var s <- make_run_state(m.config)
        var dst : array<float>
        dst |> resize(HD)
        // Q8_0: d = 1.0 (0x3C00), row 0 = 0..31, row 1 = -1..-32
        m.ngram_type = GGML_TYPE_Q8_0
        m.ngram_rows = 2l
        m.ngram_row_bytes = 34l
        m.ngram_tab |> resize(68)
        for (r in range64(2l)) {
            m.ngram_tab[r * 34l] = 0x00u8
            m.ngram_tab[r * 34l + 1l] = 0x3Cu8
            for (k in range64(32l)) {
                m.ngram_tab[r * 34l + 2l + k] = uint8(r == 0l ? k : 256l - (k + 1l))
            }
        }
        var toks <- [3l]   // ctx = [3, eos = 0]: mixed = 3 -> row 1
        unsafe {
            ngram_gather(m, s, toks, 0l, addr(dst[0]))
        }
        t |> equal(dst[0], -1.0)
        t |> equal(dst[31], -32.0)
        var toks0 <- [2l]   // mixed = 2 -> row 0
        unsafe {
            ngram_gather(m, s, toks0, 0l, addr(dst[0]))
        }
        t |> equal(dst[5], 5.0)
        // IQ4_NL: d = 1.0, nibble j in the low half, 15 - j in the high half of byte j
        m.ngram_type = GGML_TYPE_IQ4_NL
        m.ngram_row_bytes = 18l
        m.ngram_tab |> resize(36)
        for (r in range64(2l)) {
            m.ngram_tab[r * 18l] = 0x00u8
            m.ngram_tab[r * 18l + 1l] = 0x3Cu8
            for (k in range64(16l)) {
                m.ngram_tab[r * 18l + 2l + k] = uint8(k | ((15l - k) << 4l))
            }
        }
        unsafe {
            ngram_gather(m, s, toks0, 0l, addr(dst[0]))
        }
        let lut = iq4nl_lut()
        var bad = 0
        for (k in range64(16l)) {
            if (dst[k] != float(lut[k]) || dst[k + 16l] != float(lut[15l - k])) {
                bad++
            }
        }
        t |> success(bad == 0, "every nibble decoded through the codebook ({bad} mismatches)")
        delete toks
        delete toks0
        delete dst
        delete s
        delete m
    }
}
