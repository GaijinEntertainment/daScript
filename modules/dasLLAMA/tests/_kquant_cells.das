options gen2
options _dasllama_internal = true

module _kquant_cells shared public

//! the K-quant placement cells on a kq-native load and the plane and file dequants; no entry loads a model

require dastest/testing_boost public
require dasllama/dasllama_transformer   // umbrella fires each arch [init] registration
require dasllama/dasllama_gguf   // parse_gguf_meta, gguf_read_tensor_f32: the file dequant
require dasllama/dasllama_common   // dequant_kq_plane_sb, mm_b_kq, site_scale, cls_kq
require dasllama/dasllama_core   // create_session - the session mm_b_kq takes
require daslib/fio
require math
require _model_tier   // rel_l2, mirror_kv_dtype
require _compares   // check, the bars and shapes

// The K-quant load's placement cells on a model loaded kq-native under the portable backend's pin (`kq_native_load` in
// `test_kquant.das`: `set_kquant_native(true)` around a q8 `load_gguf`, inside `with_kernel_backend("portable")` so no
// kq-capable backend repacks the planes and they stay disk-order - the placement oracle), each against the file's own
// dequant: the NVFP4 planes and their per-tensor scales (Qwen3.5-0.8B-NVFP4-MTP), Phi-3.5's fused attn_qkv slices and its
// LongRoPE factors (Phi-3.5-mini Q4_K_M), and the E-series per-layer token table (gemma-4-E2B Q4_K_M). Beside them the plane
// and file dequants `test_kquant.das`'s own cells read.

//! dequant `n` elements from a Model kq plane starting at plane-local element offset `eloff`
def dequant_plane_prefix(tr : Model; fmt : KqFmt; eloff, n : int64; var dst : array<float>) {
    for (blk in range64(n / 256l)) {
        let sb = (eloff + blk * 256l) / 256l
        dequant_kq_plane_sb(fmt, tr.kq[int(fmt)].q, tr.kq[int(fmt)].s, sb, false, dst, blk * 256l)
    }
}

//! dequant `n` elements of `name` from element `first` into fp32; reports the on-disk type via `gt`
def read_prefix(path, name : string; n : int64; var gt : int&; first : int64 = 0l) : array<float> {
    var dst : array<float>
    dst |> reserve(n)
    dst |> resize(n)
    let f = fopen(path, "rb")
    if (f == null) {
        panic("cannot open {path}")
    }
    fmap(f) $(var bytes : array<uint8>#) {
        var m <- parse_gguf_meta(bytes)
        gt = gguf_tensor_type(m, name)
        gguf_read_tensor_f32(m, bytes, name, dst, 0l, n, first)
        delete m
    }
    fclose(f)
    return <- dst
}

//! the per-tensor scale reaches the CPU matmul's epilogue: eight rows of blk.0.ffn_down through `mm_b_kq` against the file
//! dequant's dot times the site scale, x on the int lattice so the Q8 activation step is exact
def private check_nv4_site_scale_epilogue(t : T?; tr : Model; fdq : array<float>) {
    let cols = 3584l
    var x <- [for (k in range64(cols)); float((k * 37l) % 255l - 127l)]
    var y : array<float>
    y |> resize(8l)
    var s <- create_session(tr, mirror_kv_dtype())
    forward_prefill_alloc(tr, s, 1l, 0l)
    mm_b_kq(s, y, tr, KqFmt.nv4, tr.w2_offs[0], x, cols, 8l, 1l)
    delete s
    let sc = site_scale(tr, KqFmt.nv4, tr.w2_offs[0])
    var want : array<float>
    want |> resize(8l)
    for (r in range64(8l)) {
        var acc = 0.0lf
        for (k in range64(cols)) {
            acc += double(fdq[r * cols + k]) * double(x[k])
        }
        want[r] = float(acc * double(sc))
    }
    check(t, "nv4 ffn_down rows through mm_b_kq against the file dequant's dot under the site scale", y, want, BAR_APPROX, FLAT_SHAPE)
    delete want
    delete y
    delete x
}

//! On Qwen3.5-0.8B-NVFP4-MTP `tr`, loaded kq-native under the portable pin: the load keeps the nv4 planes disk-order, tags
//! blk.0.ffn_down nv4, lands every NVFP4 plane's `.scale` tensor on the site table at the file's value, places the down plane
//! at its cursor offset bit for bit the file dequant, and the site scale reaches `mm_b_kq`'s epilogue
def kq_nv4_load_layout_cell(t : T?; tr : Model) {
    let path = path_join(models_dir(), "Qwen3.5-0.8B-NVFP4-MTP.gguf")
    t |> success(!tr.kq_repacked, "portable-pinned load keeps the nv4 planes disk-order")
    t |> success(tr.w2_fmt[0] == KqFmt.nv4, "blk.0.ffn_down tagged nv4")
    t |> equal(length(tr.site_scale), 108, "every NVFP4 plane's `.scale` tensor lands on the site table")
    t |> success(abs(site_scale(tr, KqFmt.nv4, tr.w2_offs[0]) - 1.0299681e-4) < 1.0e-9, "blk.0.ffn_down's site scale is the file's")
    let n = 8l * 3584l   // eight rows of the down plane
    var have : array<float>
    have |> resize(n)
    dequant_plane_prefix(tr, tr.w2_fmt[0], tr.w2_offs[0], n, have)
    var gt = 0
    var fdq <- read_prefix(path, "blk.0.ffn_down.weight", n, gt)
    check(t, "nv4 ffn_down plane against the file dequant", have, fdq, BAR_EXACT, FLAT_SHAPE)
    check_nv4_site_scale_epilogue(t, tr, fdq)
    delete fdq
    delete have
}

//! Phi-3.5 Q4_K_M `tr`, loaded kq-native under the portable pin: its fused attn_qkv splits into k5 q and k slices whose plane
//! dequants match the file dequant within 1e-3 relative L2, the k slice at its dim x qd source offset. Fused-projection (Phi3)
//! gate-lift oracle: attn_qkv / ffn_up split at load with dim-multiple src_offs into the kq planes - a slice landing off by
//! one superblock shows up as noise vs the file dequant
def kq_fused_qkv_slices_cell(t : T?; tr : Model) {
    unsafe {
        t |> run("phi3 fused attn_qkv K-quant slices land at their plane offsets (Phi-3.5 Q4_K_M)") @capture(& tr)(t : T ?) {
            let path = path_join(models_dir(), "Phi-3.5-mini-instruct-Q4_K_M.gguf")
            t |> success(tr.kquant_native, "load detected native K-quant tensors")
            t |> success(tr.wq_fmt[0] == KqFmt.k5 && tr.wk_fmt[0] == KqFmt.k5 && tr.wv_fmt[0] == KqFmt.k5,
                "fused attn_qkv slices share the Q5_K tag")
            t |> success(tr.w1_fmt[0] == KqFmt.k4 && tr.w3_fmt[0] == KqFmt.k4, "fused ffn_up gate/up slices tagged k4")
            t |> success(tr.wcls_fmt == KqFmt.k6 && !cls_kq(tr), "untied Q6_K classifier tagged (no tied-cls path)")
            let dim = tr.config.dim
            let qd = layer_qd(tr.config, 0l)
            let n = 262144l
            var have : array<float>
            have |> resize(n)
            var gt = 0
            // q slice (src_off 0): the trivial placement
            dequant_plane_prefix(tr, tr.wq_fmt[0], tr.wq_offs[0], n, have)
            var fq <- read_prefix(path, "blk.0.attn_qkv.weight", dim * qd + n, gt)
            var want : array<float>
            want |> resize(n)
            for (i in range64(n)) {
                want[i] = fq[i]
            }
            t |> success(rel_l2(have, want) < 1.0e-3, "q slice matches the file dequant (f16-pair rounding only)")
            // k slice (src_off = dim*qd): the fused-split placement this arm exists for
            dequant_plane_prefix(tr, tr.wk_fmt[0], tr.wk_offs[0], n, have)
            for (i in range64(n)) {
                want[i] = fq[dim * qd + i]
            }
            t |> success(rel_l2(have, want) < 1.0e-3, "k slice at src_off dim*qd matches the file dequant")
            delete fq
            delete want
            delete have
        }
    }
}

//! Phi-3.5 Q4_K_M `tr`, loaded kq-native under the portable pin, whose context runs past the original 4096: it carries the
//! file's 48 `rope_factors_long` values bit for bit as `rope_freqs`, and they differ from the short set
def kq_phi3_longrope_factors_cell(t : T?; tr : Model) {
    unsafe {
        t |> run("phi3 LongRoPE: the long set serves a context past the original 4096") @capture(& tr)(t : T ?) {
            let path = path_join(models_dir(), "Phi-3.5-mini-instruct-Q4_K_M.gguf")
            var gt = 0
            var long_f <- read_prefix(path, "rope_factors_long.weight", 48l, gt)
            var short_f <- read_prefix(path, "rope_factors_short.weight", 48l, gt)
            t |> success(tr.config.seq_len > 4096l,
                         "the file's context ({tr.config.seq_len}) runs past the original 4096")
            t |> equal(48l, long_length(tr.rope_freqs),
                       "one factor per rotation pair of the 96-wide head")
            check(t, "the model's factors are the file's long set",
                  tr.rope_freqs, long_f, BAR_EXACT, FLAT_SHAPE)
            var apart = 0
            for (a, b in tr.rope_freqs, short_f) {
                apart += a != b ? 1 : 0
            }
            t |> success(apart > 0, "... which the short set is not ({apart} of 48 pairs differ)")
            delete short_f
            delete long_f
        }
    }
}

//! gemma-4-E2B Q4_K_M `tr`, loaded kq-native under the portable pin: its per-layer token table loads on the k5 plane, not a
//! silent q8 fallback (which also passes parity, so the tag is pinned here), and its first 16 rows dequant within 1e-3
//! relative L2 of the file dequant at its offset - the E-series PLE token table (family c), a gather-only tensor on native kq
//! planes
def kq_ple_table_cell(t : T?; tr : Model) {
    unsafe {
        t |> run("E-series PLE token table takes the kq plane path (gemma-4-E2B Q4_K_M)") @capture(& tr)(t : T ?) {
            let path = path_join(models_dir(), "gemma-4-E2B-it-Q4_K_M.gguf")
            t |> success(tr.kquant_native, "load detected native K-quant tensors")
            t |> success(tr.ple_emb_fmt == KqFmt.k5, "per_layer_token_embd tagged k5")
            t |> success(tr.emb_fmt == KqFmt.k4 && cls_kq(tr), "tied k4 classifier takes the kq plane path")
            // whole-row placement: rows 0..R of the table vs the file dequant (f16-pair rounding only)
            let all = tr.config.n_layers * tr.config.n_embd_per_layer
            let n = all * 16l
            var have : array<float>
            have |> resize(n)
            dequant_plane_prefix(tr, tr.ple_emb_fmt, tr.ple_emb_off, n, have)
            var gt = 0
            var fdq <- read_prefix(path, "per_layer_token_embd.weight", n, gt)
            t |> success(rel_l2(have, fdq) < 1.0e-3, "PLE table plane rows match the file dequant")
            delete fdq
            delete have
        }
    }
}
