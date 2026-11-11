// The CPU references the Metal and the Vulkan kernel cells share - the engine's table rope by its pairing, the gated
// activation, dense GEMM rows with their envelope, an expert's FFN rows, the Q8_0 requant block check, the attention cells'
// query rows, the MoE route's padded-CSR prefix, the gemma-4 MoE step, the f16-tap depthwise conv, the audio fronts' im2cols, shuffles, spectrum, mel and frame forms, the audio blocks' GLU, depthwise convs, pair pool and rel table, and the rail role the discrete homes' cell rigs seat a plane at - with no device type and no backend require, so a
// build without either GPU module reads it.

options gen2
options _dasllama_internal = true

module _kernel_oracles shared public

require dasllama/dasllama_lint public
require math
require dasllama/dasllama_math   // rope_scaled_tab, rope_scaled_neox_tab, rope_scaled_neox_tab_part, l2_norm_rows
require dasllama/dasllama_gpu_math
require dasllama/dasllama_arch_qwen35   // dn_delta_head: the sequential delta rule the scan and step oracles walk
require daslib/shader_lingua_franca
require daslib/f16_cvt   // f32_to_f16: the depthwise taps' halfwords
require dasllama/dasllama_plane   // PlaneF / PlaneU16: the CPU depthwise loops' planes
require dasllama/dasllama_tower   // fc_dw_bias_rows / fc_dw_bias_rows_f16: the CPU depthwise loops
require dasllama/dasllama_gpu_rail   // TowerBuf: the cell rigs' plane seats
require _compares   // erf_series: the erf GELU's double form

//! the rail role a cell rig seats its plane `p` at: a cell's planes ride the roles in the order the cell laid them
def cell_role(p : int) : TowerBuf => unsafe(reinterpret<TowerBuf>(p))

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
            to_log(LOG_INFO, "q8_blocks_off: block {blk} of {nblk} scales {os[blk]} against the reference {d_ref}; its first quants {oq[blk * 32]} {oq[blk * 32 + 1]} {oq[blk * 32 + 2]} {oq[blk * 32 + 3]} {oq[blk * 32 + 4]} {oq[blk * 32 + 5]} {oq[blk * 32 + 6]} {oq[blk * 32 + 7]}\n")
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

//! the k3 s2 p1 conv2d's im2col (`GkIm2col2d`) over [chunk][ih][iw][cin]: row (chunk, oy, ox), column (ic x 3 + kh) x 3 + kw, zero off the image and past cin x 9
def im2col2d_ref(src : array<float>; iw, ih, cin, kdp, rows : int) : array<float> {
    let ow = (iw - 1) / 2 + 1
    let oh = (ih - 1) / 2 + 1
    var col : array<float>
    col |> resize(rows * kdp)
    for (gid in range(rows * kdp)) {
        let r = gid / kdp
        let k = gid % kdp
        var v = 0.0
        if (k < cin * 9) {
            let rr = r % (oh * ow)
            let iy = (rr / ow) * 2 + (k % 9) / 3 - 1
            let ix = (rr % ow) * 2 + k % 3 - 1
            if (iy >= 0 && iy < ih && ix >= 0 && ix < iw) {
                v = src[(r / (oh * ow)) * ih * iw * cin + (iy * iw + ix) * cin + k / 9]
            }
        }
        col[gid] = v
    }
    return <- col
}

//! a 1-D conv's im2col (`GkIm2col1d`): row i column j = tap j / cin of channel j % cin at i stride + tap - pad_l, zero off x; x [tin][cin] under `row_major`, else [cin][tin]
def im2col1d_ref(x : array<float>; cin, tin, tout, k, stride, pad_l, kdim : int; row_major : bool) : array<float> {
    var col : array<float>
    col |> resize(tout * kdim)
    for (gid in range(tout * kdim)) {
        let i = gid / kdim
        let j = gid % kdim
        let sp = i * stride + j / cin - pad_l
        var v = 0.0
        if (j < k * cin && sp >= 0 && sp < tin) {
            v = row_major ? x[sp * cin + j % cin] : x[(j % cin) * tin + sp]
        }
        col[gid] = v
    }
    return <- col
}

//! qwen3a's conv-out feed (`GkQ3aShuffle`): feat[chunk tok + ow][h + 16 c] = y3[chunk][(h tok + ow) nch + c]
def q3a_shuffle_ref(y3 : array<float>; nch, tok, npos : int) : array<float> {
    let feat_w = 16 * nch
    var feat : array<float>
    feat |> resize(npos * feat_w)
    for (gid in range(npos * feat_w)) {
        let row = gid / feat_w
        let j = gid % feat_w
        feat[gid] = y3[(row / tok) * (16 * tok * nch) + ((j % 16) * tok + row % tok) * nch + j / 16]
    }
    return <- feat
}

//! the subsample stack's feature rows (`GkFeatShuffle`): the [t][w][ch] image as [t][ch][w] rows, the rows past t up to `rows_pad` zero
def feat_shuffle_ref(src : array<float>; t, w, ch, rows_pad : int) : array<float> {
    var dst : array<float>
    dst |> resize(rows_pad * w * ch)
    for (gid in range(rows_pad * w * ch)) {
        let tt = gid / (w * ch)
        let rem = gid % (w * ch)
        dst[gid] = tt < t ? src[(tt * w + rem % w) * ch + rem / w] : 0.0
    }
    return <- dst
}

//! a row's LayerNorm (weight only, biased variance) then ReLU (`GkLnReluRows`), the statistics in double
def ln_relu_rows_ref(x, wn : array<float>; rows, d : int; eps : float) : array<float> {
    var y : array<float>
    y |> resize(rows * d)
    for (r in range(rows)) {
        var mean = 0.0lf
        for (c in range(d)) {
            mean += double(x[r * d + c])
        }
        mean /= double(d)
        var vv = 0.0lf
        for (c in range(d)) {
            let dv = double(x[r * d + c]) - mean
            vv += dv * dv
        }
        let inv = 1.0lf / sqrt(vv / double(d) + double(eps))
        for (c in range(d)) {
            y[r * d + c] = max(float((double(x[r * d + c]) - mean) * inv) * wn[c], 0.0)
        }
    }
    return <- y
}

//! the windowed frames' spectrum (`TowerSpecDftT`) in double: bin b sums the taps against twiddle pairs [k][2 b], [k][2 b + 1]; magnitude under `mag`, else power
def spec_dft_ref(xw, tw : array<float>; n_fft, frames : int; mag : bool) : array<float> {
    let nb = n_fft / 2 + 1
    var pw : array<float>
    pw |> resize(frames * nb)
    for (gid in range(frames * nb)) {
        let f = gid / nb
        let b = gid % nb
        var re = 0.0lf
        var im = 0.0lf
        for (k in range(n_fft)) {
            re += double(xw[f * n_fft + k]) * double(tw[k * 2 * nb + 2 * b])
            im += double(xw[f * n_fft + k]) * double(tw[k * 2 * nb + 2 * b + 1])
        }
        pw[gid] = float(mag ? sqrt(re * re + im * im) : re * re + im * im)
    }
    return <- pw
}

//! the log filterbank (`GkMelLog`) under law `form`: log(floored pw[f] . fil[foff + j nbins]) x scale, either row order; `float_order` sums as the CPU gigaam clamp form does
def mel_log_ref(pw, fil : array<float>; nbins, nmel, frames, foff : int; form : uint; lo, hi, scale : float;
                frame_major, float_order : bool) : array<float> {
    var mel : array<float>
    mel |> resize(frames * nmel)
    for (f in range(frames)) {
        for (j in range(nmel)) {
            var sum = 0.0lf
            var sumf = 0.0
            for (k in range(nbins)) {
                sum += double(pw[f * nbins + k]) * double(fil[foff + j * nbins + k])
                sumf += pw[f * nbins + k] * fil[foff + j * nbins + k]
            }
            if (float_order) {
                sum = double(sumf)
            }
            let v = form == 2u ? clamp(sum, double(lo), double(hi)) : (form == 1u ? sum + double(lo) : max(sum, double(lo)))
            mel[frame_major ? f * nmel + j : j * frames + f] = float(log(v) * double(scale))
        }
    }
    return <- mel
}

//! canary's per-feature normalization (`GkMelNorm`): each column's mean and unbiased deviation over `valid` frames in double, the frames past them zero
def mel_norm_ref(mel_in : array<float>; nmel, nframes, valid : int) : array<float> {
    var out : array<float>
    out |> resize(nframes * nmel)
    for (j in range(nmel)) {
        var sum = 0.0lf
        for (i in range(valid)) {
            sum += double(mel_in[i * nmel + j])
        }
        let mean = sum / double(valid)
        var sq = 0.0lf
        for (i in range(valid)) {
            let dv = double(mel_in[i * nmel + j]) - mean
            sq += dv * dv
        }
        let denom = sqrt(sq / double(valid - 1)) + 1e-5lf
        for (i in range(nframes)) {
            out[i * nmel + j] = i < valid ? float((double(mel_in[i * nmel + j]) - mean) / denom) : 0.0
        }
    }
    return <- out
}

//! gigaam's windowed frames (`GkFrameWindow`): row i column j = win[woff + j] x samp[i hop + j] for the live frames, the rows past them up to `rows` zero
def frame_window_ref(samp, win : array<float>; woff, n_fft, hop, frames, rows : int) : array<float> {
    var xw : array<float>
    xw |> resize(rows * n_fft)
    for (i in range(rows)) {
        for (j in range(n_fft)) {
            xw[i * n_fft + j] = i < frames ? win[woff + j] * samp[i * hop + j] : 0.0
        }
    }
    return <- xw
}

//! `[ch][k]` channel-major taps as the tap-major `[k][ch]` layout (`dw_s2_front_ref`'s, parakeet's)
def taps_tap_major(wcm : array<float>; ch, k : int) : array<float> {
    var wv : array<float>
    wv |> resize(k * ch)
    for (c in range(ch)) {
        for (kk in range(k)) {
            wv[kk * ch + c] = wcm[c * k + kk]
        }
    }
    return <- wv
}

//! the Conformer conv module's GLU (`GkGatedRows`, the sigmoid gate): row r's value half times the sigmoid of its gate half
def glu_sig_ref(pw_rows : array<float>; rows, d : int) : array<float> {
    var g : array<float>
    g |> resize(rows * d)
    for (r in range(rows)) {
        for (c in range(d)) {
            g[r * d + c] = pw_rows[r * 2 * d + c] * sigmoid_f32(pw_rows[r * 2 * d + d + c])
        }
    }
    return <- g
}

//! the Conformer's causal k5 depthwise conv (`GkDwConvRows`): channel-major taps at `woff`, the rows before the first zero
def dw_conv5_causal_ref(x, wn : array<float>; rows, d, woff : int) : array<float> {
    var y : array<float>
    y |> resize(rows * d)
    for (r in range(rows)) {
        for (c in range(d)) {
            var acc = 0.0
            for (kk in range(5)) {
                let src = r - 4 + kk
                if (src >= 0) {
                    acc += wn[woff + c * 5 + kk] * x[src * d + c]
                }
            }
            y[r * d + c] = acc
        }
    }
    return <- y
}

//! The FastConformer conv module tail's centered depthwise conv over channel-major taps at `taps_off`: `bn` the folded
//! BatchNorm (rows at `bnw_off` / `bnb_off`) and silu, `ga` gigaam's form - the bias row at `bnb_off` first, no epilogue
struct FcDwRef {
    bn : array<float>
    ga : array<float>
}

def fc_dw_ref(x, wn : array<float>; rows, d, k, taps_off, bnw_off, bnb_off : int) : FcDwRef {
    var r : FcDwRef
    r.bn |> resize(rows * d)
    r.ga |> resize(rows * d)
    let kpad = (k - 1) / 2
    for (row in range(rows)) {
        for (c in range(d)) {
            var acc = 0.0
            for (kk in range(k)) {
                let src = row + kk - kpad
                if (src >= 0 && src < rows) {
                    acc += wn[taps_off + c * k + kk] * x[src * d + c]
                }
            }
            let bv = acc * wn[bnw_off + c] + wn[bnb_off + c]
            r.bn[row * d + c] = bv * sigmoid_f32(bv)
            r.ga[row * d + c] = wn[bnb_off + c] + acc
        }
    }
    return <- r
}

//! the chat tails' pair pool (`GkPoolRows`): output row i is the mean of the input row pair 2 i, 2 i + 1
def pool_pairs_ref(x : array<float>; width, total : int) : array<float>
    => <- [for (i in range(total)); 0.5 * (x[(i / width) * 2 * width + i % width] + x[(i / width) * 2 * width + i % width + width])]

//! the SwiGLU with its halves the other way from the swapped gate's, silu(first) . second: the swapped gate's control
def swiglu_unswapped_ref(x : array<float>; width, total : int) : array<float>
    => <- [for (i in range(total)); silu_f32(x[(i / width) * 2 * width + i % width]) * x[(i / width) * 2 * width + i % width + width]]

//! the Transformer-XL rel table (`GkRelSinCos`): row w at rel tt - 1 - w, [sin | cos] pairs at the inverse-10000 frequencies
def rel_sincos_ref(tt, d : int) : array<float> {
    let ww = 2 * tt - 1
    var rel_want : array<float>
    rel_want |> resize(ww * d)
    for (w in range(ww)) {
        let rel = double(tt - 1 - w)
        for (k in range(d / 2)) {
            let freq = exp(-(double(k * 2) * log(10000.0lf) / double(d)))
            let theta = float(freq * rel)
            rel_want[w * d + 2 * k] = sin(theta)
            rel_want[w * d + 2 * k + 1] = cos(theta)
        }
    }
    return <- rel_want
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

//! `n` elements of `src` from `soff`, plus the bias row `bh` from `boff` when the cell has one (an empty `bh`: no bias)
def biased_row(src : array<float>; soff : int; bh : array<float>; boff, n : int) : array<float> {
    if (empty(bh)) {
        return <- [for (i in range(n)); src[soff + i]]
    }
    return <- [for (i in range(n)); src[soff + i] + bh[boff + i]]
}

//! the rope oracle's cos and sin rows: the two halves of the `hs`-wide cos | sin row at `at`
def cossin_rows(csh : array<float>; at, hs : int) : tuple<c : array<float>; s : array<float>> {
    var c <- [for (j in range(hs / 2)); csh[at + j]]
    var s <- [for (j in range(hs / 2)); csh[at + hs / 2 + j]]
    return <- (c = c, s = s)
}

//! the fused qk-norm + rope + KV-store oracle at one position (both homes' `QknRopeKvT` cells): each head row biased when `bh` is set, rmsnormed on `wnh` (q heads on its first `hs`, k heads on the next), table-roped on the cos | sin row `csh`; k lands at mirror row `krow`, v copies its raw row plus bias, both through `round`
def qkn_rope_rows(q0h, kvh, bh, wnh, csh : array<float>; bpad, nh, nkvh, hs, krow, mirn : int; eps : float;
                  round : block<(x : float) : float>) : tuple<q : array<float>; mk : array<float>; mv : array<float>> {
    let qd = nh * hs
    let kvd = nkvh * hs
    var cs <- cossin_rows(csh, 0, hs)
    var q, mk, mv : array<float>
    q |> resize(qd)
    mk |> resize(mirn)
    mv |> resize(mirn)
    for (wid in range(nh + nkvh)) {
        let is_k = wid >= nh
        let hh = is_k ? wid - nh : wid
        let sbase = is_k ? qd + hh * hs : hh * hs
        var inscope row <- is_k ? biased_row(kvh, sbase, bh, bpad + sbase, hs) : biased_row(q0h, sbase, bh, bpad + sbase, hs)
        var inscope nrow : array<float>
        nrow |> resize(hs)
        unsafe {
            rmsnorm(addr<float?>(nrow[0]), addr<float const?>(row[0]), addr<float const?>(wnh[is_k ? hs : 0]), int64(hs), eps)
        }
        rope_tab_ref(nrow, 0, hs, hs, hs, true, cs.c, cs.s)
        for (i in range(hs)) {
            if (is_k) {
                mk[krow + hh * hs + i] = invoke(round, nrow[i])
            } else {
                q[hh * hs + i] = nrow[i]
            }
        }
        if (is_k) {
            var inscope vrow <- biased_row(kvh, qd + kvd + hh * hs, bh, bpad + qd + kvd + hh * hs, hs)
            for (i in range(hs)) {
                mv[krow + hh * hs + i] = invoke(round, vrow[i])
            }
            delete vrow
        }
        delete row
        delete nrow
    }
    delete cs
    return <- (q = q, mk = mk, mv = mv)
}

//! The f16-tap depthwise cells' case (gigaam's conv, `GkDwConvRows` FORM 3 tap-major): `np` rows of `d` channels, `nk` taps
//! [nk][d] as f16 halfwords, the bias row f32 behind a lead the stamps never read.
struct DwF16Fix {
    np : int
    d : int
    nk : int
    boff : int
    x : array<float>
    taps : array<uint16>
    wn : array<float>
}

def dw_f16_fix(np, d, nk : int) : DwF16Fix {
    var fx = DwF16Fix(np = np, d = d, nk = nk, boff = 5)
    fx.x |> resize(np * d)
    for (i in range(np * d)) {
        fx.x[i] = 0.09 * float((i * 5) % 43) - 1.7
    }
    fx.taps |> resize(nk * d)
    for (i in range(nk * d)) {
        fx.taps[i] = uint16(f32_to_f16(0.0371 * float((i * 7) % 31) - 0.8))
    }
    fx.wn |> resize(fx.boff + d)
    for (i in range(fx.boff)) {
        fx.wn[i] = 1.0e30
    }
    for (c in range(d)) {
        fx.wn[fx.boff + c] = 0.013 * float((c * 3) % 17) - 0.07
    }
    return <- fx
}

//! the CPU loop over the halfword taps (`fc_dw_bias_rows_f16`) - the rows the device stamps are held to; `f32` runs the f32 loop
//! (`fc_dw_bias_rows`) over the taps widened into an f32 plane instead, the form the halfwords must match to the bit
def dw_f16_cpu(fx : DwF16Fix; f32 : bool) : array<float> {
    var o : array<float>
    o |> resize(fx.np * fx.d)
    let n = int64(fx.nk * fx.d)
    unsafe {
        let bias = PlaneF(p = addr<float const?>(fx.wn[0]), n = long_length(fx.wn))
        if (f32) {
            var inscope wide : array<float>
            wide |> resize(n + int64(fx.d))
            for (i in range64(n)) {
                wide[i] = f16_to_f32(uint(fx.taps[i]))
            }
            for (c in range(fx.d)) {
                wide[n + int64(c)] = fx.wn[fx.boff + c]
            }
            fc_dw_bias_rows(o, fx.x, PlaneF(p = addr<float const?>(wide[0]), n = long_length(wide)), 0l, n, int64(fx.d), int64(fx.np), int64(fx.nk))
        } else {
            let taps = PlaneU16(p = addr<uint16 const?>(fx.taps[0]), n = n)
            var inscope wide : array<float>
            fc_dw_bias_rows_f16(o, fx.x, taps, 0l, bias, int64(fx.boff), int64(fx.d), int64(fx.np), int64(fx.nk), wide)
        }
    }
    return <- o
}

//! the sequential delta rule the dn scan cells hold the chunked scan to, every value head's `npos` steps over `convh` (q | k | v rows `cd` wide) from `sth`, beta / gate / decay / dt at their `smh` offsets: the raw o rows [npos x nvh x ds], then the final states [nvh x ds x ds]
def dn_delta_ref(convh, sth, smh : array<float>; npos, nkh, nvh, ds, beta_off, g_off, a_off, dt_off : int) : array<float> {
    let kd = nkh * ds
    let di = nvh * ds
    let cd = 2 * kd + di
    var ref : array<float>
    ref |> reserve(npos * di + nvh * ds * ds)   // the whole-window arm's rows pass the unreserved-growth guard
    ref |> resize(npos * di + nvh * ds * ds)
    var st_h, dvn : array<float>
    st_h |> resize(ds * ds)
    dvn |> resize(ds)
    for (hh in range(nvh)) {
        let kh = hh % nkh
        for (e in range(ds * ds)) {
            st_h[e] = sth[hh * ds * ds + e]
        }
        for (tt in range(npos)) {
            let crow = tt * cd
            let gt = smh[a_off + hh] * softplus(smh[g_off + tt * nvh + hh] + smh[dt_off + hh])
            unsafe {
                dn_delta_head(addr(st_h[0]), addr(convh[crow + kh * ds]), addr(convh[crow + kd + kh * ds]), addr(convh[crow + 2 * kd + hh * ds]),
                    addr(ref[tt * di + hh * ds]), addr(dvn[0]), gt, sigmoid_f32(smh[beta_off + tt * nvh + hh]), int64(ds))
            }
        }
        for (e in range(ds * ds)) {
            ref[npos * di + hh * ds * ds + e] = st_h[e]
        }
    }
    delete st_h
    delete dvn
    return <- ref
}

//! the scan's gated out-norm over `dn_delta_ref`'s rows: each head's raw o row (`npos` rows of `nvh` heads `ds` wide) rms-normed under
//! the norm row at `smh[wnorm_off]`, times silu(z) - or sigmoid(z) under `sig`, the qwen4exp stamp's - of its `zh` row; the final
//! states past the rows as they were
def dn_gate_ref(o_raw, zh, smh : array<float>; npos, nvh, ds, wnorm_off : int; eps : float; sig : bool) : array<float> {
    let di = nvh * ds
    var o := o_raw
    for (tt in range(npos)) {
        for (hh in range(nvh)) {
            let at = tt * di + hh * ds
            var sum = 0.0
            for (j in range(ds)) {
                sum += o_raw[at + j] * o_raw[at + j]
            }
            let inv = 1.0 / sqrt(sum / float(ds) + eps)
            for (j in range(ds)) {
                let zv = zh[at + j]
                let normed = o_raw[at + j] * inv * smh[wnorm_off + j]
                o[at + j] = normed * (sig ? sigmoid_f32(zv) : silu_f32(zv))
            }
        }
    }
    return <- o
}

//! the window's ring hand-off on the CPU, per channel: new ring row k is `src` row `row0 + k - old_rows`, or for k under `old_rows` the entry image's (`old_off`) row `k + taps - old_rows`; the new image at `out_off`
def dn_tail_oracle(src, smalls : array<float>; cd, taps, row0, old_rows, old_off, out_off : int) : array<float> {
    var want := smalls
    for (ch in range(cd)) {
        for (k in range(taps)) {
            want[out_off + ch * taps + k] = (k >= old_rows ? src[(row0 + k - old_rows) * cd + ch]
                : smalls[old_off + ch * taps + k + taps - old_rows])
        }
    }
    return <- want
}

//! the fused deltanet step's oracle over one row in slot 0: the o row under the silu out-gate and under the sigmoid one, the
//! state and the smalls (the ring advanced into the other image) as the step leaves them
struct DnStepRef {
    out : array<float>
    out_sig : array<float>
    state : array<float>
    smalls : array<float>
}

//! the fused deltanet step on the CPU over `projh` ([q | k | v | z] at `cd`): ring conv + silu, q / k l2 norms and q scale, the chunk-length-1 delta rule over `sth`, the gated out-norm; reads ring image `par`, advances the other; beta / g at `beta_off` / `g_off` (row 0), a, dt, taps, out-norm weights at their `smh` offsets
def dn_step_ref(projh, sth, smh : array<float>; par, nvh, nkh, cd, ds, dconv, beta_off, g_off, a_off, dt_off, taps_off, wnorm_off, hist_off : int; eps, qscale : float) : DnStepRef {
    let kd = nkh * ds
    let taps = dconv - 1
    var r : DnStepRef
    r.smalls := smh
    let rrb = hist_off + par * cd * taps
    let wrb = hist_off + (1 - par) * cd * taps
    for (ch in range(cd)) {
        for (k in range(taps - 1)) {
            r.smalls[wrb + ch * taps + k] = smh[rrb + ch * taps + k + 1]
        }
        r.smalls[wrb + ch * taps + taps - 1] = projh[ch]
    }
    r.state := sth
    r.out |> resize(nvh * ds)
    r.out_sig |> resize(nvh * ds)
    var inscope dq, dk, dv, dvn, dout : array<float>
    dq |> resize(ds)
    dk |> resize(ds)
    dv |> resize(ds)
    dvn |> resize(ds)
    dout |> resize(ds)
    for (hh in range(nvh)) {
        let kh = hh % nkh
        for (sl in range(3)) {
            for (j in range(ds)) {
                let ch = (sl == 0 ? kh * ds : (sl == 1 ? kd + kh * ds : 2 * kd + hh * ds)) + j
                var acc = projh[ch] * smh[taps_off + ch * dconv + taps]
                for (k in range(taps)) {
                    acc += smh[rrb + ch * taps + k] * smh[taps_off + ch * dconv + k]
                }
                let sv = silu_f32(acc)
                if (sl == 0) {
                    dq[j] = sv
                } elif (sl == 1) {
                    dk[j] = sv
                } else {
                    dv[j] = sv
                }
            }
        }
        l2_norm_rows(unsafe(addr(dq[0])), 1l, int64(ds), eps)
        l2_norm_rows(unsafe(addr(dk[0])), 1l, int64(ds), eps)
        let graw = smh[g_off + hh] + smh[dt_off + hh]
        let eg = exp(smh[a_off + hh] * softplus(graw))
        let bt = sigmoid_f32(smh[beta_off + hh])
        var qk = 0.0
        for (j in range(ds)) {
            dq[j] *= qscale
            qk += dq[j] * dk[j]
        }
        let sbase = hh * ds * ds
        for (j in range(ds)) {
            var ks = 0.0
            var qs = 0.0
            for (i in range(ds)) {
                ks += dk[i] * sth[sbase + i * ds + j]
                qs += dq[i] * sth[sbase + i * ds + j]
            }
            dvn[j] = bt * (dv[j] - eg * ks)
            dout[j] = eg * qs + qk * dvn[j]
        }
        for (e in range(ds * ds)) {
            r.state[sbase + e] = sth[sbase + e] * eg + dk[e / ds] * dvn[e % ds]
        }
        var sum = 0.0
        for (j in range(ds)) {
            sum += dout[j] * dout[j]
        }
        let inv = 1.0 / sqrt(sum / float(ds) + eps)
        for (j in range(ds)) {
            let zv = projh[cd + hh * ds + j]
            r.out[hh * ds + j] = dout[j] * inv * smh[wnorm_off + j] * silu_f32(zv)
            r.out_sig[hh * ds + j] = dout[j] * inv * smh[wnorm_off + j] * sigmoid_f32(zv)
        }
    }
    return <- r
}

//! LayerNorm of `rows` rows of `x`, `dim` wide, in double: a row less its mean, over sqrt(variance + eps), times the weight
//! row at `woff` of `w`, plus the bias row at `boff` of `b`, into `want`; `env` takes each element's |normed| + |bias|.
def layernorm_ref(x : array<auto(XT)>; rows, dim : int64; w, b : array<float>; woff, boff : int64; eps : double;
                  var want : array<double>; var env : array<double>) {
    want |> resize(rows * dim)
    env |> resize(rows * dim)
    for (p in range64(rows)) {
        let off = p * dim
        var mean = 0.0lf
        for (i in range64(dim)) {
            mean += double(x[off + i])
        }
        mean /= double(dim)
        var vr = 0.0lf
        for (i in range64(dim)) {
            let dv = double(x[off + i]) - mean
            vr += dv * dv
        }
        vr /= double(dim)
        let scale = 1.0lf / sqrt(vr + eps)
        for (i in range64(dim)) {
            let nrm = (double(x[off + i]) - mean) * scale * double(w[woff + i])
            let bias = double(b[boff + i])
            want[off + i] = nrm + bias
            env[off + i] = abs(nrm) + abs(bias)
        }
    }
}

//! Bidirectional multi-head attention over `t` rows of `hidden`, `heads` heads, in double: every query over every key, the
//! logits scaled by 1 / sqrt(head width), a softmax row, the value rows under it.
def attention_bidir_ref(q, k, v : array<double>; t, hidden, heads : int64) : array<double> {
    let dh = hidden / heads
    var ctx : array<double>
    ctx |> resize(t * hidden)
    var inscope p : array<double>
    p |> resize(t)
    for (hd in range64(heads)) {
        let off = hd * dh
        for (i in range64(t)) {
            var mx = -1e300lf
            for (j in range64(t)) {
                var s = 0.0lf
                for (e in range64(dh)) {
                    s += q[i * hidden + off + e] * k[j * hidden + off + e]
                }
                p[j] = s / sqrt(double(dh))
                mx = max(mx, p[j])
            }
            var sum = 0.0lf
            for (j in range64(t)) {
                p[j] = exp(p[j] - mx)
                sum += p[j]
            }
            for (e in range64(dh)) {
                var acc = 0.0lf
                for (j in range64(t)) {
                    acc += p[j] * v[j * hidden + off + e]
                }
                ctx[i * hidden + off + e] = acc / sum
            }
        }
    }
    return <- ctx
}

//! GELU of `x` in double: the erf form where `erf`, else the tanh approximation.
def gelu_form_ref(x : double; erf : bool) : double {
    return 0.5lf * x * (1.0lf + erf_series(x / sqrt(2.0lf))) if (erf)
    return 0.5lf * x * (1.0lf + tanh(sqrt(2.0lf / 3.141592653589793lf) * (x + 0.044715lf * x * x * x)))
}
