options gen2
options _dasllama_internal = true

module _kquant_cells shared public

//! the K-quant placement cells on a kq-native load and the plane and file dequants; no entry loads a model

require dastest/testing_boost public
require dasllama/dasllama_transformer   // umbrella fires each arch [init] registration
require dasllama/dasllama_gguf   // parse_gguf_meta, gguf_read_tensor_f32: the file dequant
require dasllama/dasllama_common   // dequant_kq_plane_sb, mm_b_kq, site_scale, cls_kq
require dasllama/dasllama_core   // create_session - the session mm_b_kq takes
require dasllama/dasllama_convert   // quantize_q8_0_into: a per-32 site's activation row
require dasllama/dasllama_gemm_schema   // kq_dev_is_plane: a 1-bit grid member's plane form
require daslib/fio
require math
require _model_tier   // rel_l2, mirror_kv_dtype
require _compares   // check, the bars and shapes
require _format_census   // census_cell: a low-bit carrier's sites in their own formats

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

//! An activation whose Q8_K image is EXACT (integers in [-127, 127], a +-127 peak each 256-block): a GEMV over it adds no quantization error
def exact_q8k_row(var x : array<float>; n, seed : int64) {
    for (i in range64(n)) {
        x[i] = float(((i * 37l + 11l + seed * 13l) % 255l) - 127l)
    }
}

//! the first `rows` rows of `w` (`n` wide) dotted with `x` in f64: the served rows' oracle
def rows_dot_f64(w, x : array<float>; n, rows : int64) : array<double> {
    var out : array<double>
    out |> reserve(rows)
    for (r in range64(rows)) {
        var acc = 0.0lf
        for (i in range64(n)) {
            acc += double(w[r * n + i]) * double(x[i])
        }
        out |> push(acc)
    }
    return <- out
}

//! Largest |y[r] - double-dot(w row r, x)| over `rows`, relative to the dot's own magnitude; a NaN reads as infinite.
def rows_dot_dev(w, x, y : array<float>; n, rows : int64) : float {
    var inscope want <- rows_dot_f64(w, x, n, rows)
    var worst = 0.0
    for (r in range64(rows)) {
        let dev = float(abs(double(y[r]) - want[r]) / (abs(want[r]) + 1.0lf))
        worst = max(worst, is_nan(dev) ? INFINITE_DIFF : dev)
    }
    return worst
}

//! On a carrier `tr` at `path` whose dense planes are member `fmt`'s disk type: the loader tags every layer's seven dense planes `fmt`
def kq_dense_tags_cell(t : T?; tr : Model; path : string; fmt : KqFmt) {
    kq_mixed_tags_cell(t, tr, path, fmt, 7 * int(tr.config.n_layers))
}

//! On a carrier `tr` at `path` of mixed dense disk types: every layer's seven dense planes tagged their type's member, `want` of them `fmt`
def kq_mixed_tags_cell(t : T?; tr : Model; path : string; fmt : KqFmt; want : int) {
    let c = tr.config
    var bad = ""
    var got = 0
    let fd = fopen(path, "rb")
    t |> success(fd != null, "{path} opens")
    return if (fd == null)
    fmap(fd) $(var bytes : array<uint8>#) {
        var m <- parse_gguf_meta(bytes)
        for (l in range64(c.n_layers)) {
            for (kind, f in ["attn_q", "attn_k", "attn_v", "attn_output", "ffn_gate", "ffn_down", "ffn_up"],
                    [tr.wq_fmt[l], tr.wk_fmt[l], tr.wv_fmt[l], tr.wo_fmt[l], tr.w1_fmt[l], tr.w2_fmt[l], tr.w3_fmt[l]]) {
                let disk = gguf_tensor_type(m, "blk.{l}.{kind}.weight")
                if (kq_ggml_type(f) != disk) {
                    bad = "blk.{l}.{kind} is {f} over disk type {disk}"
                }
                got += f == fmt ? 1 : 0
            }
        }
        delete m
    }
    fclose(fd)
    t |> equal(bad, "", "every layer's seven dense planes are tagged their disk type's member")
    t |> equal(got, want, "{want} dense planes are {ggml_type_name(kq_ggml_type(fmt))} on disk and tagged {fmt}")
}

//! On a carrier `tr` at `path` whose every dense plane is member `fmt`'s disk type, loaded kq-native (`portable` its form's pin to
//! the portable backend): the tags and the served rows
def kq_dense_served_cell(t : T?; tr : Model; path : string; fmt : KqFmt; portable : bool) {
    kq_dense_tags_cell(t, tr, path, fmt)
    kq_last_q_served_cell(t, tr, path, fmt, portable)
}

//! the served rows' bar: on an exact Q8_K row only the kernel's f32 fold parts it from the f64 oracle - the ternary planes' integer
//! block dots exact (tq2 and tq1 read 0), the 1-bit grid planes' sub-block scales and delta folded at a 2^-24 rounding an add
//! (6.8e-6 at most over the 64 rows on rizen); the poisoned expectation, half its magnitude off, lands outside
let private SERVED_ROWS_BAR = Bar(rel = 1.0e-5, poison_scaled = true)
let private SERVED_ROWS = 64l   //! the attn_q rows the served cell reads off the file

//! On a kq-native carrier whose last attn_q is `fmt`'s disk type: its first `SERVED_ROWS` rows through `mm_f` dot the file's on an
//! exact Q8_K row within the fold's rounding; on the form pinned to the portable backend (`portable`) the plane stays disk-order
//! and dequantizes to the file's bit for bit
def kq_last_q_served_cell(t : T?; tr : Model; path : string; fmt : KqFmt; portable : bool) {
    let c = tr.config
    let l = c.n_layers - 1l
    let dim = c.dim
    let rows = SERVED_ROWS
    var gt = 0
    var inscope w <- read_prefix(path, "blk.{l}.attn_q.weight", rows * dim, gt)
    if (portable) {
        t |> success(!tr.kq_repacked, "the portable-pinned load keeps blk.{l}.attn_q's {fmt} plane disk-order")
        var inscope have <- nan_plane(int(rows * dim))
        dequant_plane_prefix(tr, tr.wq_fmt[l], tr.wq_offs[l], rows * dim, have)
        check(t, "blk.{l}.attn_q's {fmt} plane against the file dequant", have, w, BAR_EXACT, FLAT_SHAPE)
    }
    var inscope x : array<float>
    x |> resize(dim)
    exact_q8k_row(x, dim, 0l)
    var inscope xq : array<int8>
    xq |> resize(dim)
    var inscope xs : array<float>
    xs |> resize(dim / 32l + 8l)
    var inscope xbs : array<int>
    xbs |> resize(dim / 16l + 8l)
    let qd = layer_qd(c, l)
    var inscope y <- nan_plane(int(qd))
    mm_f(y, tr, tr.wq_fmt[l], tr.wq_offs[l], x, xq, xs, xbs, dim, qd)
    var inscope want <- rows_dot_f64(w, x, dim, rows)
    var inscope head <- [for (r in range64(rows)); y[r]]
    check(t, "blk.{l}.attn_q's first {rows} rows through the served matmul (repacked {tr.kq_repacked}) against the file rows' f64 dot",
        head, want, SERVED_ROWS_BAR, FLAT_SHAPE)
}

//! a native site's rows through `mm_f` (one activation row: Q8_0 on a per-32 plane, the Q8_K form on a superblock plane) against the
//! double dot of the file's dequantized rows with the activation's dequant, at sampled rows; the bar is f32 reassociation, the control a
//! row's neighbour
def native_site_rows(t : T?; tr : Model; path, site, name : string; fmt : KqFmt; woff, n, d : int64) {
    t |> success(kq_block32(fmt) || kq_dev_is_plane(fmt), "{site}: the plane serves its own form ({fmt})")
    var x, y, xs : array<float>
    var xq : array<int8>
    var xbs : array<int>
    x |> resize(n)
    for (i in range64(n)) {
        x[i] = float((i * 37l) % 101l - 50l) * 0.013
    }
    xq |> resize(n)
    xs |> resize(n / 32l)
    xbs |> resize(n / 16l)
    y |> resize(d)
    mm_f(y, tr, fmt, woff, x, xq, xs, xbs, n, d)
    let unit = kq_elems(fmt)
    if (!kq_sb(fmt)) {
        quantize_q8_0_into(x, n, xq, xs, 0l, 0l)
    }
    for (r in [0l, 7l, d / 2l + 3l, d - 1l]) {
        var gt = 0
        var fdq <- read_prefix(path, name, n, gt, r * n)
        var ref = 0.0lf
        var mag = 0.0lf
        for (k in range64(n)) {
            let p = double(fdq[k]) * double(float(xq[k]) * xs[k / unit])
            ref += p
            mag += abs(p)
        }
        let bar = 1.0e-5lf * mag + 1.0e-6lf
        t |> success(abs(double(y[r]) - ref) <= bar, "{site} row {r}: {y[r]} within {bar} of the file rows' dot {ref}")
        if (r + 1l < d) {
            t |> success(abs(double(y[r + 1l]) - ref) > bar || abs(ref) < bar, "{site} row {r}: the next row's output lands outside the bar (the control)")
        }
        delete fdq
    }
    delete x
    delete y
    delete xs
    delete xq
    delete xbs
}

//! On a low-bit dense carrier `tr` at `path` loaded kq-native, its planes member `f`: every site native, layer 0's attention q and layer `down_l`'s FFN down rows (1-bit mints: layers 0-1 down Q2_K)
def kq_dense_sites_cell(t : T?; tr : Model; path : string; f : KqFmt; down_l : int64 = 0l) {
    census_cell(t, tr, path, "cpu load", ["attention q/k/v/o"])
    let c = tr.config
    t |> equal(fmt_at(tr.wq_fmt, 0l), f, "attention q loads as {f}")
    t |> equal(fmt_at(tr.w2_fmt, down_l), f, "layer {down_l}'s FFN down loads as {f}")
    native_site_rows(t, tr, path, "attention q", "blk.0.attn_q.weight", fmt_at(tr.wq_fmt, 0l), tr.wq_offs[0], c.dim, c.n_heads * c.head_size)
    native_site_rows(t, tr, path, "FFN down", "blk.{down_l}.ffn_down.weight", fmt_at(tr.w2_fmt, down_l), tr.w2_offs[down_l], c.hidden_dim, c.dim)
}

//! On a low-bit expert carrier `tr` at `path` loaded kq-native, its routed stacks member `f`: layer `l`'s expert 0 gate and up rows, and its down under `with_down` (the 1-bit carriers' downs are IQ4_NL)
def kq_expert_sites_cell(t : T?; tr : Model; path : string; f : KqFmt; l : int64; with_down : bool = true) {
    census_cell(t, tr, path, "cpu load", ["routed experts"])
    t |> equal(fmt_at(tr.we1_fmt, l), f, "the gate stack loads as {f}")
    t |> equal(fmt_at(tr.we3_fmt, l), f, "the up stack loads as {f}")
    native_site_rows(t, tr, path, "expert 0 gate", "blk.{l}.ffn_gate_exps.weight", fmt_at(tr.we1_fmt, l), tr.we1_offs[l], tr.config.dim, tr.config.n_ff_exp)
    native_site_rows(t, tr, path, "expert 0 up", "blk.{l}.ffn_up_exps.weight", fmt_at(tr.we3_fmt, l), tr.we3_offs[l], tr.config.dim, tr.config.n_ff_exp)
    if (with_down) {
        t |> equal(fmt_at(tr.we2_fmt, l), f, "the down stack's 1408-wide rows load as {f}")
        native_site_rows(t, tr, path, "expert 0 down", "blk.{l}.ffn_down_exps.weight", fmt_at(tr.we2_fmt, l), tr.we2_offs[l], tr.config.n_ff_exp, tr.config.dim)
    }
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
