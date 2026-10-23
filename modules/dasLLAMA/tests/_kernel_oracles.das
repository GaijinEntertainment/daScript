// The CPU references the Metal and the Vulkan kernel cells share - the engine's table rope by its pairing, the gated
// activation, dense GEMM rows with their envelope, an expert's FFN rows, the Q8_0 requant block check, the attention cells'
// query rows, the MoE route's padded-CSR prefix, the gemma-4 MoE step - with no device type and no backend require, so a
// build without either GPU module reads it.

options gen2
options _dasllama_internal = true

module _kernel_oracles shared public

require dasllama/dasllama_lint public
require math
require dasllama/dasllama_math   // rope_scaled_tab, rope_scaled_neox_tab, rope_scaled_neox_tab_part
require dasllama/dasllama_gpu_math
require daslib/shader_lingua_franca

//! The engine's table rope over `n` elements of `v` from `base`, heads `hs` wide, on the `tcos` / `tsin` rows: the partial
//! form whenever `rot` names a prefix short of `hs`, else the NEOX or the interleaved pairing as `neox` names - the engine's
//! own rule, a `rot` of 0 naming the whole head as the kernels' args take it. The cos row starts at `tbase` of `tcos`, the
//! sin row at `sbase` of `tsin` (`tbase` where it is negative: one base into two tables, or a packed [cos | sin] row).
def rope_tab_ref(var v : array<float>; base, hs, rot, n : int; neox : bool; tcos, tsin : array<float>; tbase : int = 0; sbase : int = -1) {
    unsafe {
        let cp = addr<float const?>(tcos[tbase])
        let sp = addr<float const?>(tsin[sbase < 0 ? tbase : sbase])
        if (rot > 0 && rot != hs) {
            rope_scaled_neox_tab_part(addr(v[base]), int64(hs), int64(rot), int64(n), cp, sp)
        } elif (neox) {
            rope_scaled_neox_tab(addr(v[base]), int64(hs), int64(n), cp, sp)
        } else {
            rope_scaled_tab(addr(v[base]), int64(hs), int64(n), cp, sp)
        }
    }
}

//! the vision stems' 2-axis position add over `npos` rows of `x`, `dim` wide, on a grid `ncols` wide: row p plus the x-table row
//! p % ncols, then the y-table row p / ncols, the y rows `pos_entries` rows into `tbl` - the engine's (row + ex) + ey order
def pos_add_2d_ref(x, tbl : array<float>; npos, dim, ncols, pos_entries : int) : array<float> {
    var want : array<float>
    want |> resize(npos * dim)
    for (p in range(npos)) {
        for (i in range(dim)) {
            let ex = tbl[(p % ncols) * dim + i]
            let ey = tbl[(pos_entries + p / ncols) * dim + i]
            want[p * dim + i] = (x[p * dim + i] + ex) + ey
        }
    }
    return <- want
}

//! the attention cells' query element `i`: (i * 5) % 11 - 5 eighths
def attn_q_val(i : int) : float => float((i * 5) % 11 - 5) * 0.125

//! `n` query elements, `attn_q_val` of 0 .. n - 1
def attn_q_row(n : int) : array<float> => <- [for (i in range(n)); attn_q_val(i)]

//! `n` gate elements of both signs past the sigmoid's linear range
def attn_gate_row(n : int) : array<float> => <- [for (i in range(n)); 0.09 * float(i % 31) - 1.3]

//! a gated model's q plane: each `hs`-wide head of `q` followed by the same head's gate off `gate`, 2 hs a head
def q_gate_plane(q, gate : array<float>; hs : int) : array<float> {
    var plane : array<float>
    plane |> resize(2 * length(q))
    for (g in range(length(q) / hs)) {
        for (i in range(hs)) {
            plane[g * 2 * hs + i] = q[g * hs + i]
            plane[g * 2 * hs + hs + i] = gate[g * hs + i]
        }
    }
    return <- plane
}

//! the head gate on attention rows `o` in place: each element over one plus exp of minus its gate (`sig_gate`'s CPU form)
def gate_rows(var o : array<float>; gate : array<float>) {
    for (x, g in o, gate) {
        x /= 1.0 + exp(-g)
    }
}

//! `gate_rows` into a copy, `o` left as it was
def gated_rows(o, gate : array<float>) : array<float> {
    var r := o
    gate_rows(r, gate)
    return <- r
}

//! the rows a routed tile's adaptive tensor op leaves unwritten: a 32-row tile of one to eight live rows runs an 8-row op,
//! of nine to sixteen a 16-row one, so the rows from the op's height to 32 keep the sentinel the compare filled - the
//! expectation over `want` (and its envelope, where one rides) for every twin stamp of both MoE scaffolds
def adapt_pad_rows(cnt, base : array<uint>; ndim : int; sentinel : float; var want : array<float>; var env : array<float>) {
    for (cu, bu in cnt, base) {
        let c = int(cu)
        let rows = (c + 31) / 32 * 32
        var mbase = 0
        while (mbase < rows) {
            let live = min(c - mbase, 32)
            let tr = live <= 8 ? 8 : (live <= 16 ? 16 : 32)
            for (r in range(mbase + tr, mbase + 32)) {
                let row = int(bu) + r
                for (j in range(ndim)) {
                    want[row * ndim + j] = sentinel
                    if (!empty(env)) {
                        env[row * ndim + j] = 0.0
                    }
                }
            }
            mbase += 32
        }
    }
}

//! The padded-CSR prefix of a MoE route: each expert's row count, its rows' base on the 32-row tile lattice, and `pad`, the
//! padded total past the last expert.
def padded_bases(counts : array<int>) : tuple<cnt : array<uint>; base : array<uint>; pad : int> {
    var cnt, base : array<uint>
    cnt |> reserve(length(counts))
    base |> reserve(length(counts))
    var pad = 0
    for (c in counts) {
        cnt |> push(uint(c))
        base |> push(uint(pad))
        pad += (c + 31) / 32 * 32
    }
    return <- (cnt = cnt, base = base, pad = pad)
}

//! the one activation formula (`gk_act_mul`'s shape): tanh-GELU(v) * u, or silu(v) * u - definitional, kept in sync by the arm bars
def act_mul_ref(v, u : float; gelu : bool) : float {
    return (gelu ? (0.5 * v * (1.0 + tanh(0.7978845608028654 * (v + 0.044715 * v * v * v)))) * u
        : (v / (1.0 + exp(-v))) * u)
}

//! the deltanet causal conv over `npos` rows `cd` wide - `dconv` taps a channel at `taps[toff + ch * dconv]`, the rows before row 0
//! from the per-channel history at `hist[hoff + ch * (dconv - 1)]` - through silu, then the `kd`-wide q and k heads (`ds` wide)
//! l2-normed at `eps` and the q heads scaled by `qscale`
def dn_conv_ref(src, hist : array<float>; hoff : int; taps : array<float>; toff, npos, cd, dconv, kd, ds : int; eps, qscale : float) : array<float> {
    let ntap = dconv - 1
    var o : array<float>
    o |> resize(npos * cd)
    for (p in range(npos)) {
        for (ch in range(cd)) {
            var acc = 0.0
            for (k in range(dconv)) {
                let sp = p + k - ntap
                acc += (sp >= 0 ? src[sp * cd + ch] : hist[hoff + ch * ntap + p + k]) * taps[toff + ch * dconv + k]
            }
            o[p * cd + ch] = acc / (1.0 + exp(-acc))
        }
        l2_norm_rows(unsafe(addr(o[p * cd])), int64(2 * kd / ds), int64(ds), eps)
        for (j in range(kd)) {
            o[p * cd + j] *= qscale
        }
    }
    return <- o
}

//! A dense GEMM's reference: `want[r * ndim + j]` is weight row j dotted with row r of `x`, accumulated in double, and
//! `env` the same sum over the products' magnitudes - the scale a relative bar multiplies.
struct DenseRef {
    want : array<float>
    env : array<float>
}

//! `dense_ref_env` over `m` rows of `x`, each `kdim` wide, against `ndim` weight rows: `row(j, w)` writes weight row j into `w`
def dense_ref_env(m, kdim, ndim : int; x : array<float>; row : block<(j : int; var w : array<float>) : void>) : DenseRef {
    var out : DenseRef
    out.want |> reserve(m * ndim)   //! the large tower shapes pass the unreserved-resize guard
    out.want |> resize(m * ndim)
    out.env |> reserve(m * ndim)
    out.env |> resize(m * ndim)
    var w : array<float>
    w |> reserve(kdim)
    w |> resize(kdim)
    for (j in range(ndim)) {
        invoke(row, j, w)
        for (r in range(m)) {
            var acc = 0.0lf
            var ev = 0.0lf
            for (c in range(kdim)) {
                let p = double(w[c]) * double(x[r * kdim + c])
                acc += p
                ev += abs(p)
            }
            out.want[r * ndim + j] = float(acc)
            out.env[r * ndim + j] = float(ev)
        }
    }
    delete w
    return <- out
}

//! `dense_ref_env` over a dense row-major weight plane: weight row j is `w[j * kdim ..]`
def dense_ref_rows(w, x : array<float>; m, kdim, ndim : int) : DenseRef {
    return <- dense_ref_env(m, kdim, ndim, x) $(j; var wrow) {
        for (c in range(kdim)) {
            wrow[c] = w[j * kdim + c]
        }
    }
}

//! `ref`'s rows of `ndim` laid `ys` apart in `nout` rows, the gap past each row and the rows past `ref`'s holding
//! `sentinel` with a zero envelope - the reference of an output written at a row stride wider than its row
def strided_ref(ref : DenseRef; ndim, ys, nout : int; sentinel : float) : DenseRef {
    var out : DenseRef
    out.want |> reserve(nout * ys)
    out.want |> resize(nout * ys)
    out.env |> reserve(nout * ys)
    out.env |> resize(nout * ys)
    for (i in range(nout * ys)) {
        out.want[i] = sentinel
        out.env[i] = 0.0
    }
    for (r in range(length(ref.want) / ndim)) {
        for (j in range(ndim)) {
            out.want[r * ys + j] = ref.want[r * ndim + j]
            out.env[r * ys + j] = ref.env[r * ndim + j]
        }
    }
    return <- out
}

//! `dense_ref_env` over `ndim` rows of the plane `w`, `kdim` wide, from row `r0`
def private plane_rows_ref(w, x : array<float>; m, kdim, ndim, r0 : int) : DenseRef {
    return <- dense_ref_env(m, kdim, ndim, x) $(j; var wrow) {
        for (c in range(kdim)) {
            wrow[c] = w[(r0 + j) * kdim + c]
        }
    }
}

//! One expert's FFN over `m` dequantized feed rows `x`, `n` wide: the gate and up rows of `w1` / `w3` from `r1` / `r3` (`nfe` of
//! them), silu(gate) x up rounded in place by `round_h` row by row - the device's requant or half store - then the `n` down rows
//! of `w2` from `r2`, `nfe` wide, every dot in double. The `m * n` outputs.
def ffn_expert_ref(w1, w3, w2, x : array<float>; m, n, nfe, r1, r3, r2 : int; round_h : block<(var h : array<float>) : void>) : array<float> {
    var g <- plane_rows_ref(w1, x, m, n, nfe, r1)
    var u <- plane_rows_ref(w3, x, m, n, nfe, r3)
    var h : array<float>
    h |> reserve(m * nfe)
    for (r in range(m)) {
        var inscope hr <- [for (i in range(nfe)); silu_f32(g.want[r * nfe + i]) * u.want[r * nfe + i]]
        invoke(round_h, hr)
        h |> push_from(hr)
    }
    var y <- plane_rows_ref(w2, h, m, nfe, n, r2)
    var out <- y.want
    delete g
    delete u
    delete h
    delete y
    return <- out
}

//! The Q8_0 blocks a kernel requantized, counted against the activated reference row: `scales` off 2e-3 of the block's
//! amax / 127, `values` whose dequantized element sits more than a quant step (plus 2e-3 of itself) from the reference,
//! `poison_inside` blocks whose first expectation, pushed three quant steps, still sits inside. A NaN is off.
struct Q8BlocksOff {
    scales : int
    values : int
    poison_inside : int
    worst : float   //! the largest dequantized distance from the reference, in that block's quant steps
    worst_at : int = -1
}

//! `Q8BlocksOff` over `nblk` blocks of 32: quants `oq`, scales `os`, the activated reference row `a`
def q8_blocks_off(oq : array<int8>; os : array<float>; a : array<float>; nblk : int) : Q8BlocksOff {
    var off = Q8BlocksOff()
    for (blk in range(nblk)) {
        var m = 0.0
        for (e in range(32)) {
            m = max(m, abs(a[blk * 32 + e]))
        }
        let d_ref = m / 127.0
        if (!(abs(os[blk] - d_ref) <= 2e-3 * max(d_ref, 1e-6))) {
            off.scales++
        }
        for (e in range(32)) {
            let w = a[blk * 32 + e]
            let deq = float(oq[blk * 32 + e]) * os[blk]
            if (!(abs(deq - w) <= d_ref + 2e-3 * abs(w))) {
                off.values++
            }
            let steps = abs(deq - w) / max(d_ref, 1e-30)
            if (steps > off.worst) {
                off.worst = steps
                off.worst_at = blk * 32 + e
            }
            if (e == 0 && abs(deq - (w + 3.0 * d_ref + 1.0)) <= d_ref + 2e-3 * abs(w)) {
                off.poison_inside++
            }
        }
    }
    return off
}

//! The gemma-4 MoE step's rows (`GkMoeCombG4`) over `npos` rows `n` wide: row p's routed sum - its `k` slots at `sidx[p k + j]`
//! of `rows` under `sw[p k + j]` in slot order, or under `presummed` row p of `rows` - and its dense row each under their post
//! norm, the two summed under the post-FFN norm, x = (x + that) . ascale, then x under the next norm; `wn` carries the norm rows
//! at 0 (the next), n (the dense post), 2n (the routed post), 3n (the post-FFN). Returns the updated residual rows and the
//! normed ones.
def g4_comb_ref(n, k, npos : int; sidx : array<uint>; sw, rows, dense, xs, wn : array<float>; eps, ascale : float;
                presummed : bool) : tuple<x : array<float>; y : array<float>> {
    var wx, wy, r, c : array<float>
    wx |> resize(npos * n)
    wy |> resize(npos * n)
    r |> resize(n)
    c |> resize(n)
    for (p in range(npos)) {
        var sr = 0.0
        var sd = 0.0
        for (i in range(n)) {
            var v = 0.0
            if (presummed) {
                v = rows[p * n + i]
            } else {
                for (j in range(k)) {
                    v += sw[p * k + j] * rows[int(sidx[p * k + j]) * n + i]
                }
            }
            r[i] = v
            sr += v * v
            sd += dense[p * n + i] * dense[p * n + i]
        }
        let inv_r = 1.0 / sqrt(sr / float(n) + eps)
        let inv_d = 1.0 / sqrt(sd / float(n) + eps)
        var sc = 0.0
        for (i in range(n)) {
            c[i] = wn[2 * n + i] * (r[i] * inv_r) + wn[n + i] * (dense[p * n + i] * inv_d)
            sc += c[i] * c[i]
        }
        let inv_c = 1.0 / sqrt(sc / float(n) + eps)
        var ss = 0.0
        for (i in range(n)) {
            let xv = (xs[p * n + i] + wn[3 * n + i] * (c[i] * inv_c)) * ascale
            wx[p * n + i] = xv
            ss += xv * xv
        }
        let inv = 1.0 / sqrt(ss / float(n) + eps)
        for (i in range(n)) {
            wy[p * n + i] = wn[i] * (wx[p * n + i] * inv)
        }
    }
    delete r
    delete c
    return <- (x <- wx, y <- wy)
}

//! The front convs' CPU form on an image odd along one axis and even along the other, so each edge drops its own taps:
//! the first conv off one channel (FIRST) with its bias and ReLU (RELU), or the depthwise conv with its bias, the rows
//! past `valid` masked off. Bar: nine f16-exact products summed in f32 against the same order.
def dw_s2_front_ref(first, relu : bool; iw, ch, oh, ow, valid : int; xv, wv, bv : array<float>) : array<float> {
    var want : array<float>
    want |> resize(oh * ow * ch)
    for (oy in range(oh)) {
        for (ox in range(ow)) {
            for (c in range(ch)) {
                var acc = relu ? 0.0 : bv[c]
                for (kh in range(3)) {
                    for (kw in range(3)) {
                        let iy = oy * 2 - 1 + kh
                        let ix = ox * 2 - 1 + kw
                        if (iy >= 0 && iy < valid && ix >= 0 && ix < iw) {
                            acc += wv[(kh * 3 + kw) * ch + c] * xv[first ? iy * iw + ix : (iy * iw + ix) * ch + c]
                        }
                    }
                }
                want[(oy * ow + ox) * ch + c] = relu ? max(acc + bv[c], 0.0) : acc
            }
        }
    }
    return <- want
}

//! the GEMM oracles' region-record walk: each record's `cnt` rows from `row0` at every output column, y[row * d + c] = `dot`(row, c, wblk0)
def rec_walk(recs : array<uint>; nrec, d : int; var y : array<float>; dot : block<(row, c, wblk0 : int) : float>) {
    for (rr in range(nrec)) {
        let wblk0 = int(recs[rr * 4])
        let row0 = int(recs[rr * 4 + 1])
        for (r in range(int(recs[rr * 4 + 2]))) {
            for (c in range(d)) {
                y[(row0 + r) * d + c] = invoke(dot, row0 + r, c, wblk0)
            }
        }
    }
}

def byte_u8(a : array<uint>; i : int) : int => int(a[i / 4] >> uint((i % 4) * 8)) & 0xFF

def byte_s8(a : array<uint>; i : int) : int {
    let b = byte_u8(a, i)
    return b >= 128 ? b - 256 : b
}

def half_at(a : array<uint>; hi : int) : float {
    let w = unpackHalf2x16(a[hi / 2])
    return hi % 2 == 0 ? w.x : w.y
}

//! summed per (row, col) in block order; the device tiles reassociate, so a compare takes the approx bar
def q8f16_gemm_oracle(wq : array<uint>; wsw : array<uint>; xf : array<uint>;
                      recs : array<uint>; nrec, n, d : int; var y : array<float>) {
    let nbb = n / 32
    rec_walk(recs, nrec, d, y) $(row, c, wblk0 : int) : float {
        var acc = 0.0
        for (b in range(nbb)) {
            let wb = wblk0 + c * nbb + b
            let wsf = wb % 2 == 0 ? unpackHalf2x16(wsw[wb / 2]).x : unpackHalf2x16(wsw[wb / 2]).y
            var bacc = 0.0
            for (j in range(32)) {
                bacc += float(byte_s8(wq, wb * 32 + j)) * half_at(xf, row * n + b * 32 + j)
            }
            acc += bacc * wsf
        }
        return acc
    }
}

//! exact int dots a 32-block folded in block order; the device folds reassociate, so a compare takes the approx bar
def q8_gemm_oracle(wq : array<uint>; wsw : array<uint>; xq : array<uint>; xs : array<float>;
                   recs : array<uint>; nrec, n, d : int; var y : array<float>) {
    let nbb = n / 32
    rec_walk(recs, nrec, d, y) $(row, c, wblk0 : int) : float {
        var acc = 0.0
        for (b in range(nbb)) {
            let wb = wblk0 + c * nbb + b
            let xb = row * nbb + b
            var idot = 0
            for (j in range(8)) {
                idot += sdot4(wq[wb * 8 + j], xq[xb * 8 + j])
            }
            let wsf = wb % 2 == 0 ? unpackHalf2x16(wsw[wb / 2]).x : unpackHalf2x16(wsw[wb / 2]).y
            acc += float(idot) * wsf * xs[xb]
        }
        return acc
    }
}
