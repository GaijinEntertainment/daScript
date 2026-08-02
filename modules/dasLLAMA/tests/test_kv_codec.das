options gen2
options persistent_heap   // the f16 model A/B holds two Sessions (see test_forward.das)
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)

require dastest/testing_boost public
require dasllama/dasllama_transformer   // umbrella fires each arch [init] registration
require dasllama/dasllama_math   // setup_dasllama_jobque_ + target_chunk_work + fused knob
require dasllama/dasllama_kv_codec   // the codec kernel families under unit test
require dasllama/dasllama_convert    // cvt_f32_to_f16/cvt_f16_to_f32 — the generic f16 rows
require llvm/daslib/f16_cvt
require daslib/jobque_boost
require daslib/fio
require math

// KV-cache codec layout invariants — synthetic Configs only (no model files, never skips):
// the per-session row-byte prefixes built by make_run_state must mirror layout_offsets' shared-
// skipping walk, agree with the definitional kv_cache_off on uniform models, alias shared-KV
// layers to their source slab, and size the blobs exactly. Plus the kv_store_row round-trip.
// The f16 codec cases at the bottom need stories15M (env-gated, skip contract as test_forward).

def private mk_config(layers, heads, kv_heads, hs, ctx : int64) : Config {
    return Config(dim = heads * hs, hidden_dim = 32l, n_layers = layers, n_heads = heads,
        n_kv_heads = kv_heads, vocab_size = 8l, seq_len = ctx, head_size = hs,
        kv_dim = kv_heads * hs, shared_weights = true, rope_freq_base = 10000.0)
}

// independent reference: layout_offsets' prefix walk (elements), shared layers aliasing the source
def private ref_prefix_elems(c : Config) : array<int64> {
    var pref : array<int64>
    var row = 0l
    for (l in range64(c.n_layers)) {
        let src = layer_kv_source(c, l)
        if (src != l) {
            pref |> push(pref[src])
        } else {
            pref |> push(row)
            row += layer_kv_dim(c, l)
        }
    }
    return <- pref
}

def private own_row_elems(c : Config) : int64 {
    var total = 0l
    for (l in range64(c.n_layers)) {
        if (layer_kv_source(c, l) == l) {
            total += layer_kv_dim(c, l)
        }
    }
    return total
}

def private check_layout(t : T?; c : Config; tag : string) {
    var s <- make_run_state(c)
    let ref <- ref_prefix_elems(c)
    for (l in range64(c.n_layers)) {
        t |> equal(s.kv_bprefix_k[l], ref[l] * 4l)   // f32 codec: byte prefix = 4 x element prefix
        t |> equal(s.kv_bprefix_v[l], s.kv_bprefix_k[l])   // same dtype -> same walk
        t |> equal(s.kv_bprefix_k[l], s.kv_bprefix_k[layer_kv_source(c, l)])   // shared-KV aliasing
    }
    let want = c.seq_len * own_row_elems(c) * 4l
    t |> success(long_length(s.key_cache) == want, "{tag}: key cache sized exactly (bytes)")
    t |> success(long_length(s.value_cache) == want, "{tag}: value cache sized exactly (bytes)")
    delete s
}

[test]
def test_kv_layout(t : T?) {
    t |> run("uniform model: prefixes match the definitional kv_cache_off") @(t : T?) {
        let c = mk_config(4l, 4l, 2l, 64l, 8l)
        var s <- make_run_state(c)
        for (l in range64(c.n_layers)) {
            // no shared layers -> the walking Config form and the skipping walk agree
            t |> equal(s.kv_bprefix_k[l] * c.seq_len, kv_cache_off(c, l) * 4l)
        }
        delete s
        check_layout(t, c, "uniform")
    }
    t |> run("gemma4-like ragged layers (SWA class differs in head size AND kv heads)") @(t : T?) {
        var c = mk_config(6l, 4l, 2l, 64l, 8l)
        c.sliding_window = 4l
        c.swa_pattern = 2l        // even layers slide
        c.head_size_swa = 32l
        c.n_kv_heads_swa = 4l
        check_layout(t, c, "ragged")
    }
    t |> run("E-series shared KV: post-boundary layers alias their source slab") @(t : T?) {
        var c = mk_config(6l, 4l, 2l, 64l, 8l)
        c.sliding_window = 4l
        c.swa_pattern = 2l
        c.n_layer_kv_from_start = 4l   // layers 4 (sliding -> src 2) and 5 (global -> src 3) share
        t |> equal(layer_kv_source(c, 4l), 2l)
        t |> equal(layer_kv_source(c, 5l), 3l)
        check_layout(t, c, "shared")
    }
}

[test]
def test_kv_layout_f16(t : T?) {
    t |> run("f16 sessions: byte prefixes halve, mixed K/V dtypes walk independently") @(t : T?) {
        let c = mk_config(4l, 4l, 2l, 64l, 8l)
        let ref <- ref_prefix_elems(c)
        var s16 <- make_run_state(c, KVDtype.f16, KVDtype.f16)
        for (l in range64(c.n_layers)) {
            t |> equal(s16.kv_bprefix_k[l], ref[l] * 2l)   // f16 codec: byte prefix = 2 x element prefix
            t |> equal(s16.kv_bprefix_v[l], s16.kv_bprefix_k[l])
        }
        let want16 = c.seq_len * own_row_elems(c) * 2l
        t |> success(long_length(s16.key_cache) == want16, "f16 key cache is half the f32 bytes")
        t |> success(long_length(s16.value_cache) == want16, "f16 value cache is half the f32 bytes")
        delete s16
        var smix <- make_run_state(c, KVDtype.f16, KVDtype.f32)   // K/V dtypes are independent
        for (l in range64(c.n_layers)) {
            t |> equal(smix.kv_bprefix_k[l], ref[l] * 2l)
            t |> equal(smix.kv_bprefix_v[l], ref[l] * 4l)
        }
        t |> success(long_length(smix.key_cache) * 2l == long_length(smix.value_cache),
            "mixed session: K blob is half the V blob")
        delete smix
    }
}

[test]
def test_kv_layout_q8(t : T?) {
    t |> run("q8_0 sessions: 34-bytes-per-32 prefixes, mixed dtypes walk independently") @(t : T?) {
        let c = mk_config(4l, 4l, 2l, 64l, 8l)
        let ref <- ref_prefix_elems(c)
        var sq <- make_run_state(c, KVDtype.q8_0, KVDtype.q8_0)
        for (l in range64(c.n_layers)) {
            t |> equal(sq.kv_bprefix_k[l], ref[l] / 32l * 34l)   // block codec: 34 bytes per 32 elements
            t |> equal(sq.kv_bprefix_v[l], sq.kv_bprefix_k[l])
        }
        let wantq = c.seq_len * own_row_elems(c) / 32l * 34l
        t |> success(long_length(sq.key_cache) == wantq, "q8_0 key cache sized exactly (34/32 of the element count)")
        t |> success(long_length(sq.value_cache) == wantq, "q8_0 value cache sized exactly")
        delete sq
        var smix <- make_run_state(c, KVDtype.q8_0, KVDtype.f16)   // K/V dtypes stay independent
        for (l in range64(c.n_layers)) {
            t |> equal(smix.kv_bprefix_k[l], ref[l] / 32l * 34l)
            t |> equal(smix.kv_bprefix_v[l], ref[l] * 2l)
        }
        delete smix
    }
}

// block bi's dequantized element k, read back from stored q8_0 row bytes
def private q8_row_elem(p : uint8 const?; bi, k : int64) : float {
    unsafe {
        let bb = bi * 34l
        let dh = uint(p[bb]) | (uint(p[bb + 1l]) << 8u)
        return float(int8(p[bb + 2l + k])) * f16_to_f32(dh)
    }
}

[test]
def test_kv_store_row_q8(t : T?) {
    t |> run("q8_0 kv_store_row: round-trip within d/2, idempotent re-quantize, head-slot placement") @(t : T?) {
        let c = mk_config(3l, 4l, 2l, 32l, 6l)
        let kv_dim = layer_kv_dim(c, 1l)   // 64 = 2 blocks per row
        var s <- make_run_state(c, KVDtype.q8_0, KVDtype.q8_0)
        var src : array<float>
        src |> resize(int(kv_dim))
        for (i in range(length(src))) {
            src[i] = 3.0 * sin(float(i) * 0.37 + 0.11)   // no exact-zero block, spans +/-
        }
        let rowb = kv_row_bytes(KVDtype.q8_0, kv_dim)
        t |> equal(rowb, kv_dim / 32l * 34l)
        unsafe {
            let kbase = kv_base_k(s, 1l, c.seq_len)
            var kcl = addr(s.key_cache[0]) + kbase
            kv_store_row(kcl, s.kv_dtype_k, kv_dim, 3l, 0l, kv_dim, addr<float const?>(src[0]))
            // round-trip: |src - dequant| <= d/2 per element (d = that block's stored scale)
            let row3 = reinterpret<uint8 const?>(kcl + 3l * rowb)
            var ok_rt = true
            var dq : array<float>
            dq |> resize(int(kv_dim))
            for (bi in range64(kv_dim / 32l)) {
                let dh = uint(row3[bi * 34l]) | (uint(row3[bi * 34l + 1l]) << 8u)
                let d = f16_to_f32(dh)
                for (k in range64(32l)) {
                    let v = q8_row_elem(row3, bi, k)
                    dq[bi * 32l + k] = v
                    // d/2 from the round, + up to 127·(f16 rounding of d) — quants round with the
                    // UN-rounded f32 scale but dequant reads the stored f16 one (ggml semantics)
                    ok_rt &&= abs(src[bi * 32l + k] - v) <= d * 0.57 + 1.0e-7
                }
            }
            t |> success(ok_rt, "every element within half a quantization step (+ f16-scale rounding)")
            // idempotence: re-quantizing the dequantized row reproduces the bytes exactly
            kv_store_row(kcl, s.kv_dtype_k, kv_dim, 4l, 0l, kv_dim, addr<float const?>(dq[0]))
            var idem = true
            for (b in range64(rowb)) {
                idem &&= kcl[3l * rowb + b] == kcl[4l * rowb + b]
            }
            t |> success(idem, "quantize(dequantize(row)) is byte-identical")
            // head-slot partial store (sub = head_size = 32): lands at whole-block offset in row 1
            kv_store_row(kcl, s.kv_dtype_k, kv_dim, 1l, 32l, 32l, addr<float const?>(src[0]))
            let row1 = reinterpret<uint8 const?>(kcl + 1l * rowb)
            var ok_sub = true
            for (k in range64(32l)) {
                ok_sub &&= abs(src[k] - q8_row_elem(row1, 1l, k)) <= f16_to_f32(uint(row1[34l]) | (uint(row1[35l]) << 8u)) * 0.57 + 1.0e-7
            }
            t |> success(ok_sub, "head-slot store lands at the right block")
            delete dq
        }
        delete src
        delete s
    }
}

[test]
def test_kv_runs_flat(t : T?) {
    t |> run("flat sessions: every window is exactly one identity run; kv_phys_row is identity") @(t : T?) {
        let c = mk_config(2l, 4l, 2l, 64l, 16l)
        var s <- make_run_state(c)
        t |> equal(1, length(s.kv_runs))   // make_run_state sizes the flat scratch to one entry
        // whole-window, offset-window, single-row — all one run with row0 == j0
        var nrun = kv_runs(s, 0l, 16l, s.kv_runs)
        t |> equal(1l, nrun)
        t |> equal(0l, s.kv_runs[0].j0)
        t |> equal(16l, s.kv_runs[0].j1)
        t |> equal(0l, s.kv_runs[0].row0)
        nrun = kv_runs(s, 5l, 12l, s.kv_runs)   // sliding-window shape
        t |> equal(1l, nrun)
        t |> equal(5l, s.kv_runs[0].j0)
        t |> equal(12l, s.kv_runs[0].j1)
        t |> equal(5l, s.kv_runs[0].row0)       // flat: physical row == absolute position
        nrun = kv_runs(s, 7l, 8l, s.kv_runs)    // one row
        t |> equal(1l, nrun)
        t |> equal(7l, s.kv_runs[0].row0)
        t |> equal(0l, kv_runs(s, 9l, 9l, s.kv_runs))   // empty window: no runs
        t |> equal(0l, kv_runs(s, 9l, 3l, s.kv_runs))   // inverted window: no runs
        for (pos in range64(16l)) {
            t |> equal(pos, kv_phys_row(s, pos))
        }
        delete s
    }
}

[test]
def test_kv_store_row_f16(t : T?) {
    t |> run("f16 kv_store_row: RNE convert on store, clamp to ±65504, exact read-back") @(t : T?) {
        let c = mk_config(3l, 4l, 2l, 16l, 4l)
        let kv_dim = layer_kv_dim(c, 1l)
        var s <- make_run_state(c, KVDtype.f16, KVDtype.f16)
        var src : array<float>
        src |> resize(int(kv_dim))
        for (i in range(length(src))) {
            src[i] = float(i) * 0.5 - 3.0   // exactly representable in f16
        }
        src[0] = 0.1                        // not representable: pins the RNE rounding
        src[1] = 1.0e6                      // past f16 max: pins the +clamp
        src[2] = -1.0e6                     // pins the -clamp
        unsafe {
            let kbase = kv_base_k(s, 1l, c.seq_len)
            kv_store_row(addr(s.key_cache[0]) + kbase, s.kv_dtype_k, kv_dim, 3l, 0l, kv_dim,
                addr<float const?>(src[0]))
            let kcp = reinterpret<uint16 const?>(addr(s.key_cache[0]) + kbase)
            var ok = true
            for (i in range64(kv_dim)) {
                let want = f32_to_f16(clamp(src[i], -65504.0, 65504.0))
                ok &&= uint(kcp[3l * kv_dim + i]) == want
            }
            t |> success(ok, "every stored half == RNE(clamp(src))")
            t |> equal(f16_to_f32(uint(kcp[3l * kv_dim + 1l])), 65504.0)    // clamped, finite
            t |> equal(f16_to_f32(uint(kcp[3l * kv_dim + 2l])), -65504.0)
        }
        delete src
        delete s
    }
}

// ===== kv_codec kernel units — byte-level, model-free =====
// Exactness split: integer-accumulating dots (q8q8kv/q8tq4kv) and per-element kernels
// (axpy/cvt/quantize) are bit-exact against a scalar reference on ANY tuned perm; the
// float-reduction dots (dot_q8kv/dot_f16) tolerance-compare (a vectorized perm reorders adds).

def private blk_scale(b : array<uint8>; off : int64) : float => f16_to_f32(uint(b[off]) | (uint(b[off + 1l]) << 8u))

def private fill_sin(var a : array<float>; n : int64; amp, w, ph : float) {
    a |> resize(n)
    for (i in range(length(a))) {
        a[i] = amp * sin(float(i) * w + ph)
    }
}

// deterministic full-range int8 quants + fixed per-32 scales — a hand-built Q8 query
def private fill_q8_query(var q : array<int8>; var qs : array<float>; n : int64; step : int) {
    q |> resize(n)
    for (i in range(length(q))) {
        q[i] = int8(((i * step) % 255) - 127)
    }
    qs |> resize(n / 32l)
    for (i in range(length(qs))) {
        qs[i] = 0.5 / float(i + 1)
    }
}

[test]
def test_kv_codec_signs(t : T?) {
    t |> run("tq4_fill_signs: ±1 values, smaller head sizes are a prefix") @(t : T?) {
        var signs : array<float>
        tq4_fill_signs(signs, 64l)
        var ok_pm = true
        for (x in signs) {
            ok_pm &&= (x == 1.0 || x == -1.0)
        }
        t |> success(ok_pm, "signs are ±1")
        var pre : array<float>
        tq4_fill_signs(pre, 32l)
        var ok_prefix = true
        for (i in range(32)) {
            ok_prefix &&= pre[i] == signs[i]
        }
        t |> success(ok_prefix, "head_size 32 signs are a prefix of the 64 vector")
        delete signs
        delete pre
    }
}

[test]
def test_kv_codec_fwht(t : T?) {
    t |> run("fwht: unsign inverts sign-rotate, the transform preserves dots") @(t : T?) {
        let hs = 64l
        var signs : array<float>
        tq4_fill_signs(signs, hs)
        var x : array<float>
        var y : array<float>
        fill_sin(x, hs, 1.0, 0.7, 0.3)
        fill_sin(y, hs, 1.0, 0.41, 1.57)
        var xr := x
        var yr := y
        unsafe {
            var d0 = 0.0
            for (i in range64(hs)) {
                d0 += x[i] * y[i]
            }
            fwht_signs_row(addr(xr[0]), addr<float const?>(signs[0]), hs)
            fwht_signs_row(addr(yr[0]), addr<float const?>(signs[0]), hs)
            var d1 = 0.0
            for (i in range64(hs)) {
                d1 += xr[i] * yr[i]
            }
            t |> success(abs(d1 - d0) <= 1.0e-4 * max(abs(d0), 1.0), "rotated dot == original dot (orthonormal)")
            fwht_unsign_row(addr(xr[0]), addr<float const?>(signs[0]), hs)
            var ok_inv = true
            for (i in range64(hs)) {
                ok_inv &&= abs(xr[i] - x[i]) <= 1.0e-5
            }
            t |> success(ok_inv, "unsign_row(signs_row(v)) == v")
        }
        delete signs
        delete x
        delete y
        delete xr
        delete yr
    }
}

[test]
def test_kv_codec_tq4_roundtrip(t : T?) {
    t |> run("tq4 quantize/dequant: round-trip within one step, idempotent re-quantize") @(t : T?) {
        let n = 64l
        var src : array<float>
        fill_sin(src, n, 1.75, 0.53, 0.07)
        var blk : array<uint8>
        blk |> resize(int(n / 32l * 18l))
        var dq : array<float>
        dq |> resize(int(n))
        unsafe {
            quantize_tq4kv_row(addr(blk[0]), addr<float const?>(src[0]), n)
            cvt_tq4kv_to_f32(addr(dq[0]), addr<uint8 const?>(blk[0]), n)
            var ok_rt = true
            for (bi in range64(n / 32l)) {
                let d = blk_scale(blk, bi * 18l)
                for (k in range64(32l)) {
                    // half a truncation step in general, but q4_0 saturates the POSITIVE extreme
                    // (min(15, ...) caps x = +8d at quant 15 -> dequant 7d): up to one full step
                    ok_rt &&= abs(src[bi * 32l + k] - dq[bi * 32l + k]) <= abs(d) * 1.08 + 1.0e-7
                }
            }
            t |> success(ok_rt, "every element within one quantization step (q4_0 saturation)")
            var blk2 : array<uint8>
            blk2 |> resize(int(n / 32l * 18l))
            quantize_tq4kv_row(addr(blk2[0]), addr<float const?>(dq[0]), n)
            var idem = true
            for (b in range64(long_length(blk))) {
                idem &&= blk[b] == blk2[b]
            }
            t |> success(idem, "quantize(dequantize(row)) is byte-identical")
            delete blk2
        }
        delete src
        delete blk
        delete dq
    }
}

[test]
def test_kv_codec_q8_dots(t : T?) {
    t |> run("q8_0 dots agree with the scalar block reference") @(t : T?) {
        let n = 64l
        var src : array<float>
        fill_sin(src, n, 2.5, 0.61, 0.23)
        var a : array<float>
        fill_sin(a, n, 1.0, 0.13, 1.97)
        var q : array<int8>
        var qs : array<float>
        fill_q8_query(q, qs, n, 37)
        var row : array<uint8>
        row |> resize(int(n / 32l * 34l))
        unsafe {
            quantize_q8kv_row(addr(row[0]), addr<float const?>(src[0]), n)
            let rp = addr<uint8 const?>(row[0])
            // dot_q8kv: float inner reduction — a vectorized perm reorders adds, so tolerance
            var want = 0.0
            for (bi in range64(n / 32l)) {
                var facc = 0.0
                for (k in range64(32l)) {
                    facc += a[bi * 32l + k] * float(int8(row[bi * 34l + 2l + k]))
                }
                want += facc * blk_scale(row, bi * 34l)
            }
            let got = dot_q8kv(addr<float const?>(a[0]), rp, n)
            t |> success(abs(got - want) <= 1.0e-3 * max(abs(want), 1.0), "dot_q8kv matches the block reference")
            // dot_q8q8kv: integer inner sum commutes — exact on any perm
            var want2 = 0.0
            for (bi in range64(n / 32l)) {
                var iacc = 0
                for (k in range64(32l)) {
                    iacc += int(q[bi * 32l + k]) * int(int8(row[bi * 34l + 2l + k]))
                }
                want2 += float(iacc) * qs[bi] * blk_scale(row, bi * 34l)
            }
            let got2 = dot_q8q8kv(addr<int8 const?>(q[0]), addr<float const?>(qs[0]), rp, n)
            t |> success(got2 == want2, "dot_q8q8kv bit-exact vs the integer block reference")
        }
        delete src
        delete a
        delete q
        delete qs
        delete row
    }
}

[test]
def test_kv_codec_q8_axpy(t : T?) {
    t |> run("axpy_q8kv: one mad per element — bit-exact on any perm") @(t : T?) {
        let n = 64l
        var src : array<float>
        fill_sin(src, n, 2.5, 0.61, 0.23)
        var row : array<uint8>
        row |> resize(int(n / 32l * 34l))
        var dst : array<float>
        var dref : array<float>
        dst |> resize(int(n))
        dref |> resize(int(n))
        for (i in range64(n)) {
            dst[i] = float(i) * 0.125 - 2.0
            dref[i] = dst[i]
        }
        unsafe {
            quantize_q8kv_row(addr(row[0]), addr<float const?>(src[0]), n)
            for (bi in range64(n / 32l)) {
                let c = 0.6 * blk_scale(row, bi * 34l)
                for (k in range64(32l)) {
                    dref[bi * 32l + k] = mad(c, float(int8(row[bi * 34l + 2l + k])), dref[bi * 32l + k])
                }
            }
            axpy_q8kv(addr(dst[0]), addr<uint8 const?>(row[0]), 0.6, n)
        }
        var okx = true
        for (i in range64(n)) {
            okx &&= dst[i] == dref[i]
        }
        t |> success(okx, "axpy_q8kv bit-exact per element")
        delete src
        delete row
        delete dst
        delete dref
    }
}

[test]
def test_kv_codec_tq4_dot(t : T?) {
    t |> run("dot_q8tq4kv: integer inner sum — bit-exact vs the block reference") @(t : T?) {
        let n = 64l
        var src : array<float>
        fill_sin(src, n, 1.4, 0.83, 0.51)
        var q : array<int8>
        var qs : array<float>
        fill_q8_query(q, qs, n, 53)
        var blk : array<uint8>
        blk |> resize(int(n / 32l * 18l))
        unsafe {
            quantize_tq4kv_row(addr(blk[0]), addr<float const?>(src[0]), n)
            var want = 0.0
            for (bi in range64(n / 32l)) {
                var iacc = 0
                for (k in range64(16l)) {
                    let by = int(blk[bi * 18l + 2l + k])
                    iacc += int(q[bi * 32l + 2l * k]) * ((by & 15) - 8)
                    iacc += int(q[bi * 32l + 2l * k + 1l]) * ((by >> 4) - 8)
                }
                want += float(iacc) * qs[bi] * blk_scale(blk, bi * 18l)
            }
            let got = dot_q8tq4kv(addr<int8 const?>(q[0]), addr<float const?>(qs[0]), addr<uint8 const?>(blk[0]), n)
            t |> success(got == want, "dot_q8tq4kv bit-exact vs the integer block reference")
        }
        delete src
        delete q
        delete qs
        delete blk
    }
}

[test]
def test_kv_codec_tq4_axpy(t : T?) {
    t |> run("axpy_tq4kv: one mad per nibble element — bit-exact on any perm") @(t : T?) {
        let n = 64l
        var src : array<float>
        fill_sin(src, n, 1.4, 0.83, 0.51)
        var blk : array<uint8>
        blk |> resize(int(n / 32l * 18l))
        var dst : array<float>
        var dref : array<float>
        dst |> resize(int(n))
        dref |> resize(int(n))
        for (i in range64(n)) {
            dst[i] = 1.0 - float(i) * 0.0625
            dref[i] = dst[i]
        }
        unsafe {
            quantize_tq4kv_row(addr(blk[0]), addr<float const?>(src[0]), n)
            for (bi in range64(n / 32l)) {
                let c = -0.8 * blk_scale(blk, bi * 18l)
                for (k in range64(16l)) {
                    let by = int(blk[bi * 18l + 2l + k])
                    dref[bi * 32l + 2l * k] = mad(c, float((by & 15) - 8), dref[bi * 32l + 2l * k])
                    dref[bi * 32l + 2l * k + 1l] = mad(c, float((by >> 4) - 8), dref[bi * 32l + 2l * k + 1l])
                }
            }
            axpy_tq4kv(addr(dst[0]), addr<uint8 const?>(blk[0]), -0.8, n)
        }
        var okx = true
        for (i in range64(n)) {
            okx &&= dst[i] == dref[i]
        }
        t |> success(okx, "axpy_tq4kv bit-exact per element")
        delete src
        delete blk
        delete dst
        delete dref
    }
}

[test]
def test_kv_codec_f16(t : T?) {
    t |> run("f16 dot/axpy + convert's f16 row pair") @(t : T?) {
        let n = 48l
        var a : array<float>
        fill_sin(a, n, 1.0, 0.9, 0.1)
        var s : array<float>
        fill_sin(s, n, 2.0, 0.77, 1.57)
        s[3] = 1.0e6   // pins the store clamp
        var h : array<uint16>
        h |> resize(int(n))
        unsafe {
            cvt_f32_to_f16(addr(h[0]), addr<float const?>(s[0]), n)
            t |> equal(f16_to_f32(uint(h[3])), 65504.0)   // clamped, finite — inf never enters
            var back : array<float>
            back |> resize(int(n))
            cvt_f16_to_f32(addr(back[0]), addr<uint16 const?>(h[0]), n)
            var ok = true
            for (i in range64(n)) {
                ok &&= back[i] == f16_to_f32(f32_to_f16(clamp(s[i], -65504.0, 65504.0)))
            }
            t |> success(ok, "cvt round-trip == scalar f16 semantics per element")
            // dot_f16: float reduction — tolerance vs the scalar mad chain
            var want = 0.0
            for (i in range64(n)) {
                want = mad(a[i], back[i], want)
            }
            let got = dot_f16(addr<float const?>(a[0]), addr<uint16 const?>(h[0]), n)
            t |> success(abs(got - want) <= 1.0e-3 * max(abs(want), 1.0), "dot_f16 matches the reference")
            // axpy_f16: one mad per element — exact on any perm
            var dst : array<float>
            var dref : array<float>
            dst |> resize(int(n))
            dref |> resize(int(n))
            for (i in range64(n)) {
                dst[i] = float(i) * 0.25
                dref[i] = mad(0.75, back[i], dst[i])
            }
            axpy_f16(addr(dst[0]), addr<uint16 const?>(h[0]), 0.75, n)
            var okx = true
            for (i in range64(n)) {
                okx &&= dst[i] == dref[i]
            }
            t |> success(okx, "axpy_f16 bit-exact per element")
            delete back
            delete dst
            delete dref
        }
        delete a
        delete s
        delete h
    }
}

def private llama2c_dir() : string {
    let env = get_env_variable("DASLLAMA_LLAMA2C_DIR")
    return empty(env) ? "/Users/borisbatkin/Work/llama2.c" : env
}
let MODEL_PATH = path_join(llama2c_dir(), "stories15M.bin")
let N_STEPS = 24

def private argmax_of(s : Session; vocab : int64) : int64 {
    var best = 0l
    for (i in range64(1l, vocab)) {
        if (s.logits[i] > s.logits[best]) {
            best = i
        }
    }
    return best
}

def private greedy_ids(tr : Model; var s : Session; var ids : array<int64>; var logits : array<float>) {
    ids |> reserve(N_STEPS)
    var token = 1l   // BOS
    var pos = 0l
    for (_i in range(N_STEPS)) {
        forward(tr, s, token, pos)
        pos++
        token = argmax_of(s, tr.config.vocab_size)
        ids |> push(token)
    }
    logits := s.logits
}

[test]
def test_kv_f16_decode(t : T?) {
    t |> run("f16 KV: greedy trajectory matches f32, fused==std bit-exact under f16 (needs stories15M.bin)") @(t : T?) {
        if (!jit_enabled()) {
            t |> skip("interpreted (dasLLAMA model tests are JIT-only)")
            return
        }
        if (!stat(MODEL_PATH).is_valid) {
            t |> skip("stories15M.bin not present")
            return
        }
        var tr <- load_checkpoint(MODEL_PATH)
        let c = tr.config
        quantize_weights(tr)
        let prev_thresh = get_target_chunk_work()
        with_job_que() {
            setup_dasllama_jobque_()
            set_target_chunk_work(1l)
            var ids32 : array<int64>
            var ids16 : array<int64>
            var ids16f : array<int64>
            var lg32 : array<float>
            var lg16 : array<float>
            var lg16f : array<float>
            set_fused_decode(false)
            var s32 <- make_run_state(c)
            greedy_ids(tr, s32, ids32, lg32)
            delete s32
            var s16 <- make_run_state(c, KVDtype.f16, KVDtype.f16)
            greedy_ids(tr, s16, ids16, lg16)
            delete s16
            set_fused_decode(true)
            var s16f <- make_run_state(c, KVDtype.f16, KVDtype.f16)
            greedy_ids(tr, s16f, ids16f, lg16f)
            delete s16f
            // f16 vs f32: same greedy trajectory (the real gate) + a structural logit backstop —
            // on a Q8 model a ~1e-3 attention diff can cross int8 requant boundaries and amplify
            // (~0.3 observed at step 24), so the bound only catches gross corruption (O(10))
            var id_bad = 0
            for (i in range(N_STEPS)) {
                if (ids32[i] != ids16[i]) {
                    id_bad++
                }
            }
            t |> equal(0, id_bad)
            var worst = 0.0
            for (i in range64(c.vocab_size)) {
                worst = max(worst, abs(lg32[i] - lg16[i]))
            }
            t |> success(worst < 1.0, "f16-vs-f32 final logits within the structural bound (worst |diff| = {worst})")
            // fused vs std, BOTH under f16: identical rounding both sides -> bit-exact
            var f_bad = 0
            for (i in range(N_STEPS)) {
                if (ids16[i] != ids16f[i]) {
                    f_bad++
                }
            }
            t |> equal(0, f_bad)
            var lg_bad = 0
            for (i in range64(c.vocab_size)) {
                if (lg16[i] != lg16f[i]) {
                    lg_bad++
                }
            }
            t |> equal(0, lg_bad)
            delete ids32
            delete ids16
            delete ids16f
            delete lg32
            delete lg16
            delete lg16f
        }
        set_target_chunk_work(prev_thresh)
        delete tr
    }
}

[test]
def test_kv_f16_prefill_vs_decode(t : T?) {
    t |> run("f16 KV: prefill == per-token decode, logits bit-exact (needs stories15M.bin)") @(t : T?) {
        if (!jit_enabled()) {
            t |> skip("interpreted (dasLLAMA model tests are JIT-only)")
            return
        }
        if (!stat(MODEL_PATH).is_valid) {
            t |> skip("stories15M.bin not present")
            return
        }
        var tr <- load_checkpoint(MODEL_PATH)
        let c = tr.config
        quantize_weights(tr)
        let prev_thresh = get_target_chunk_work()
        let prev_mode = get_prefill_attn_mode()
        with_job_que() {
            setup_dasllama_jobque_()
            set_target_chunk_work(1l)
            set_prefill_attn_mode(AttnPrefillMode.blocked)   // blocked/classic are the bit-exact modes (flash reorders)
            var prompt : array<int64>
            for (i in range(16)) {
                prompt |> push(int64(i % 50 + 2))
            }
            let n = long_length(prompt)
            var sp <- make_run_state(c, KVDtype.f16, KVDtype.f16)
            forward_batch(tr, sp, prompt, n, 0l)
            var sd <- make_run_state(c, KVDtype.f16, KVDtype.f16)
            var pos = 0l
            for (tok in prompt) {
                forward(tr, sd, tok, pos)
                pos++
            }
            var lg_bad = 0
            for (i in range64(c.vocab_size)) {
                if (sp.logits[i] != sd.logits[i]) {
                    lg_bad++
                }
            }
            t |> equal(0, lg_bad)
            var kv_bad = 0
            for (i in range(length(sd.key_cache))) {
                if (sp.key_cache[i] != sd.key_cache[i]) {
                    kv_bad++
                }
            }
            for (i in range(length(sd.value_cache))) {
                if (sp.value_cache[i] != sd.value_cache[i]) {
                    kv_bad++
                }
            }
            t |> equal(0, kv_bad)
            delete sp
            delete sd
            delete prompt
        }
        set_prefill_attn_mode(prev_mode)
        set_target_chunk_work(prev_thresh)
        delete tr
    }
}

// q8_0 model tests run a GGUF carrier (stories15M's head_size 48 is rejected by the create-time
// 32-block check — by design); SmolLM2-135M is the small head_size-64 carrier, same rail as
// test_fused_decode. Override the model root with DASLLAMA_MODELS_DIR.
def private models_dir() : string {
    let env = get_env_variable("DASLLAMA_MODELS_DIR")
    return empty(env) ? "/Users/borisbatkin/Work/llama.cpp/models/" : env
}
let GGUF_PATH = path_join(models_dir(), "SmolLM2-135M-Instruct-Q8_0.gguf")

def private gguf_missing(t : T?) : bool {
    if (!jit_enabled()) {
        t |> skip("interpreted (dasLLAMA model tests are JIT-only)")
        return true
    }
    if (!stat(GGUF_PATH).is_valid) {
        t |> skip("{GGUF_PATH} not present")
        return true
    }
    return false
}

// teacher-forced per-step logits under one KV config: the SAME deterministic token stream into a
// fresh session, all N_STEPS logit rows out — trajectory-compounding-free comparisons
def private forced_logits(tr : Model; kdt, vdt : KVDtype; var out : array<float>) {
    let c = tr.config
    out |> resize(int64(N_STEPS) * c.vocab_size)
    var s <- make_run_state(c, kdt, vdt)
    var pos = 0l
    for (st in range(N_STEPS)) {
        forward(tr, s, int64(st * 37 % 100 + 2), pos)
        pos++
        let base = int64(st) * c.vocab_size
        for (i in range64(c.vocab_size)) {
            out[base + i] = s.logits[i]
        }
    }
    delete s
}

def private worst_diff(a : array<float>; b : array<float>) : float {
    var w = 0.0
    for (va, vb in a, b) {
        w = max(w, abs(va - vb))
    }
    return w
}

[test]
def test_kv_q8_decode(t : T?) {
    t |> run("q8_0 KV: teacher-forced logits track f16, fused==std bit-exact under q8 (needs SmolLM2-135M)") @(t : T?) {
        if (gguf_missing(t)) {
            return
        }
        let prev_backend = active_kernel_backend()
        let prev_thresh = get_target_chunk_work()
        with_job_que() {
            setup_dasllama_jobque_()
            set_target_chunk_work(1l)
            pin_kernel_backend("portable")   // the rows core every host shares — the fused gate needs one
            var tr <- load_gguf(GGUF_PATH, QuantMode.q8)
            tr.config.seq_len = min(tr.config.seq_len, 128l)
            var lg16 : array<float>
            var lg8 : array<float>
            var lg8m : array<float>
            var lg8f : array<float>
            set_fused_decode(false)
            forced_logits(tr, KVDtype.f16, KVDtype.f16, lg16)
            forced_logits(tr, KVDtype.q8_0, KVDtype.q8_0, lg8)
            forced_logits(tr, KVDtype.q8_0, KVDtype.f16, lg8m)   // K quantized, V half
            set_fused_decode(true)
            forced_logits(tr, KVDtype.q8_0, KVDtype.q8_0, lg8f)
            // q8 vs f16, teacher-forced: per-step logits stay within the structural bound (greedy
            // trajectories are NOT compared — on a 135M model a near-tie argmax flips under f16
            // noise too, and one flip compounds the whole stream). Measured here: q8-vs-f32 noise
            // is the same order as f16-vs-f32 (~0.4-1.8 worst per step).
            let w8 = worst_diff(lg16, lg8)
            t |> success(w8 < 4.0, "q8/q8-vs-f16 teacher-forced logits within the structural bound (worst |diff| = {w8})")
            let w8m = worst_diff(lg16, lg8m)
            t |> success(w8m < 4.0, "q8-K/f16-V-vs-f16 within the structural bound (worst |diff| = {w8m})")
            // fused vs std, BOTH under q8: identical kernels + identical query quants -> bit-exact
            var lg_bad = 0
            for (i in range(length(lg8))) {
                if (lg8[i] != lg8f[i]) {
                    lg_bad++
                }
            }
            t |> equal(0, lg_bad)
            delete lg16
            delete lg8
            delete lg8m
            delete lg8f
            delete tr
            clear_kernel_backend_pin()
        }
        set_target_chunk_work(prev_thresh)
        select_kernel_backend(prev_backend)
    }
}

[test]
def test_kv_q8_prefill_vs_decode(t : T?) {
    t |> run("q8_0 KV: blocked prefill == per-token decode, logits + cache bit-exact (needs SmolLM2-135M)") @(t : T?) {
        if (gguf_missing(t)) {
            return
        }
        let prev_thresh = get_target_chunk_work()
        let prev_mode = get_prefill_attn_mode()
        with_job_que() {
            setup_dasllama_jobque_()
            set_target_chunk_work(1l)
            set_prefill_attn_mode(AttnPrefillMode.blocked)   // the bit-exact prefill mode (flash reorders)
            var tr <- load_gguf(GGUF_PATH, QuantMode.q8)
            tr.config.seq_len = min(tr.config.seq_len, 64l)
            let c = tr.config
            var prompt : array<int64>
            for (i in range(16)) {
                prompt |> push(int64(i % 50 + 2))
            }
            let n = long_length(prompt)
            var sp <- make_run_state(c, KVDtype.q8_0, KVDtype.q8_0)
            forward_batch(tr, sp, prompt, n, 0l)
            var sd <- make_run_state(c, KVDtype.q8_0, KVDtype.q8_0)
            var pos = 0l
            for (tok in prompt) {
                forward(tr, sd, tok, pos)
                pos++
            }
            // blocked prefill quantizes its queries and runs the SAME int8×int8 score dot as
            // decode — the prefill==decode contract stays bit-exact under the block codec
            var lg_bad = 0
            for (i in range64(c.vocab_size)) {
                if (sp.logits[i] != sd.logits[i]) {
                    lg_bad++
                }
            }
            t |> equal(0, lg_bad)
            var kv_bad = 0
            for (i in range(length(sd.key_cache))) {
                if (sp.key_cache[i] != sd.key_cache[i]) {
                    kv_bad++
                }
            }
            for (i in range(length(sd.value_cache))) {
                if (sp.value_cache[i] != sd.value_cache[i]) {
                    kv_bad++
                }
            }
            t |> equal(0, kv_bad)
            delete sp
            delete sd
            delete prompt
            delete tr
        }
        set_prefill_attn_mode(prev_mode)
        set_target_chunk_work(prev_thresh)
    }
}

[test]
def test_kv_store_row_roundtrip(t : T?) {
    t |> run("f32 kv_store_row: whole row + partial row land at the layer/row/sub address") @(t : T?) {
        let c = mk_config(3l, 4l, 2l, 16l, 4l)
        let kv_dim = layer_kv_dim(c, 1l)
        var s <- make_run_state(c)
        var src : array<float>
        src |> resize(int(kv_dim))
        for (i in range(length(src))) {
            src[i] = float(i) * 0.5 - 3.0
        }
        unsafe {
            let kbase = kv_base_k(s, 1l, c.seq_len)
            kv_store_row(addr(s.key_cache[0]) + kbase, s.kv_dtype_k, kv_dim, 3l, 0l, kv_dim,
                addr<float const?>(src[0]))   // whole row at row 3
            kv_store_row(addr(s.key_cache[0]) + kbase, s.kv_dtype_k, kv_dim, 1l, 16l, 16l,
                addr<float const?>(src[0]))   // one head slot at row 1, sub 16
            let kcp = reinterpret<float const?>(addr(s.key_cache[0]) + kbase)
            var ok_row = true
            for (i in range64(kv_dim)) {
                ok_row &&= kcp[3l * kv_dim + i] == src[i]
            }
            t |> success(ok_row, "whole-row store reads back exactly")
            var ok_sub = true
            for (i in range64(16l)) {
                ok_sub &&= kcp[1l * kv_dim + 16l + i] == src[i]
            }
            t |> success(ok_sub, "partial-row (head slot) store reads back exactly")
        }
        delete src
        delete s
    }
}
