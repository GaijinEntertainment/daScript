options gen2
options stack = 524288   // every dasLLAMA program root takes this budget
options _function_length = 0        // per-kernel gates: fixture + builder dispatch + oracle compare, one flat run each
options _cyclomatic_complexity = 0  // same — the arm ladders follow the kernel's own binding/variant axes
options _dasllama_internal = true

require dastest/testing_boost public
require ?das_metal dasllama/dasllama_metal_kernels  // the kq GEMV / KqMv / MoeGemv kernel set (Apple builds)
require ?das_metal dasllama/dasllama_metal_common   // g_dev - the pair walk rides the decode PSOs, so its buffers live on the decode device
require ?das_metal metal/das_metal_boost  // device/pipeline/buffer plumbing + the leak gate
require ?das_metal _metal_kernel_common   // the MetalCell record, check_buf, the device shells, kq plane builders
require dasllama/dasllama_common   // nolint:STYLE030,LINT019 — MTP_MAX_ROWS, the verify's row cap the tiler's third tile reaches (das_metal half)
require _compares   // Bar: the cells' bars
require dasllama/dasllama_kqformat   // KqFmt: the per-format stamp tables' key
require _kernel_oracles   // dense_ref_env, strided_ref: the GEMV references with their envelope
require math

// The K-quant decode GEMV corpus (single-stream KqGemv K4/K5/K5C/K6, the small-batch KqMv
// B2/B4/B8 twins, the expert-indexed MoeGemv stamps over every kq format, the dense q8
// row-per-simdgroup form) vs a CPU reference on synthetic planes:
// random bit patterns are valid quant data, scales come from finite f16 values, and the truth
// side dequants each row through the SHIPPED dequant_k*_plane_superblock (the same math the
// loaders and repack verifiers trust) and dots in double. Float GPU accumulation reassociates,
// so the compare bar rides each output's DOT ENVELOPE (sum |w·x| — the test_metal_gemm
// precedent): |gpu-cpu| <= max(2e-4 * env, 1e-3). y buffers are sentinel-filled and compared
// WHOLE, so a write outside the guarded rows/columns is a red, not silence. Without das_metal in
// the build, or with no Metal device, every check skips.

let private BAR_Q8_APPROX = Bar(rel = 1e-4)   //! the fused W1|W3 outputs: the activation's transcendentals against the double oracle
let private BAR_F16_STORE = Bar(rel = 1e-3, poison_scaled = true)   //! the f16 mirrors: the store rounds at 2^-11 relative

//! the single-stream kq GEMV stamp of a variant: a format's own stamp, the compact k5 twin (`k5c`) or a grid format's f4
//! twin (the `f4` suffix)
def private kq_gemv_stamp(vform) {   //! generic, as the gates are: the Apple half alone instantiates it
    if (vform == "k4") return stamp_of(@@enc_kq_k4_c)
    if (vform == "k5") return stamp_of(@@enc_kq_k5_c)
    if (vform == "k5c") return stamp_of(@@enc_kq_k5c_c)
    if (vform == "iq4xs") return stamp_of(@@enc_kq_iq4xs_c)
    if (vform == "k3") return stamp_of(@@enc_kq_k3_c)
    if (vform == "iq3s") return stamp_of(@@enc_kq_iq3s_c)
    if (vform == "iq3sf4") return stamp_of(@@enc_kq_iq3s_f4_c)
    if (vform == "iq3xxs") return stamp_of(@@enc_kq_iq3xxs_c)
    if (vform == "iq3xxsf4") return stamp_of(@@enc_kq_iq3xxs_f4_c)
    if (vform == "q40") return stamp_of(@@enc_kq_q40_c)
    if (vform == "iq4nl") return stamp_of(@@enc_kq_iq4nl_c)
    if (vform == "k2") return stamp_of(@@enc_kq_k2_c)
    if (vform == "iq2s") return stamp_of(@@enc_kq_iq2s_c)
    if (vform == "iq2xs") return stamp_of(@@enc_kq_iq2xs_c)
    if (vform == "iq2xxs") return stamp_of(@@enc_kq_iq2xxs_c)
    if (vform == "iq2xxsf4") return stamp_of(@@enc_kq_iq2xxs_f4_c)
    return stamp_of(@@enc_kq_k6_c)
}

def private kq_gemv_gate(t : T?; var dev, queue; vform : string; n, d : int) {
    let fmt = vform == "k4" ? 4 : (vform == "k6" ? 6 : (vform == "iq4xs" ? 44 : (vform == "k3" ? 3 : (vform == "iq3s" || vform == "iq3sf4" ? 33 : (vform == "iq3xxs" || vform == "iq3xxsf4" ? 34 : (vform == "q40" ? 40 : (vform == "iq4nl" ? 45 : (vform == "iq2s" ? 23 : (vform == "iq2xs" ? 24 : (vform == "iq2xxs" || vform == "iq2xxsf4" ? 25 : (vform == "k2" ? 2 : 5)))))))))))
    let tag = "kq_gemv_{vform} n={n} d={d}"
    with_metal_cell(dev, queue) $(var mc : MetalCell) {
        var err : string
        let st = kq_gemv_stamp(vform)
        var pso = mc |> keep_stamp(st, err)
        return if (!pso_ok(t, tag, pso, err))
        let nb = n / 256
        let nsb = d * nb
        var inscope kq, ks : array<uint8>
        kq_fill_planes(fmt, nsb, kq, ks)
        var inscope xv : array<float>
        xv |> resize(n)
        for (i in range(n)) {
            xv[i] = 0.11 * float(i % 41) - 2.2
        }
        var inscope ref <- dense_ref_env(1, n, d, xv) $(r; var wrow) {
            kq_row_ref(fmt, r * nb, nb, nsb, kq, ks, wrow)
        }
        let bks = mc |> in_plain(ks)
        let bkq = mc |> in_plain(kq)
        let bx = mc |> in_(xv)
        let by = mc |> out_words(d, -1000.0)
        let bn = mc |> in_plain([uint(n)])
        let bd = mc |> in_plain([uint(d)])
        let doff = kq_split_doff(fmt, nsb)
        let ran = mc |> cell_record(t, tag) $(enc : MetalComputeEncoder?) {
            stamp_run(st, pso) {
                invoke(st.enc, enc, mc.buf[bks], doff, mc.buf[bks], 0ul, mc.buf[bkq], 0ul, mc.buf[bx], 0ul, mc.buf[by], 0ul, mc.buf[bn], mc.buf[bd], int64(d), int64(n))
            }
        }
        return if (!ran)
        check_buf(t, tag, mc.buf[by], ref.want, env_bar(2e-4, ref.env), FLAT_SHAPE, ref.env)
    }
}

// ===== the small-batch KqMv twins (B2/B4/B8 x K4/K5/K6): y[col * ys + row] =====

//! the B2 small-batch kq GEMV stamp of a format
def private kq_mvb2_stamp(f) {
    if (f == KqFmt.iq2xxs) return stamp_of(@@enc_kq_mvb2_iq2xxs_c)
    if (f == KqFmt.iq2xs) return stamp_of(@@enc_kq_mvb2_iq2xs_c)
    if (f == KqFmt.iq2s) return stamp_of(@@enc_kq_mvb2_iq2s_c)
    if (f == KqFmt.q40) return stamp_of(@@enc_kq_mvb2_q40_c)
    if (f == KqFmt.iq4nl) return stamp_of(@@enc_kq_mvb2_iq4nl_c)
    if (f == KqFmt.k2) return stamp_of(@@enc_kq_mvb2_k2_c)
    if (f == KqFmt.iq3xxs) return stamp_of(@@enc_kq_mvb2_iq3xxs_c)
    if (f == KqFmt.iq3s) return stamp_of(@@enc_kq_mvb2_iq3s_c)
    if (f == KqFmt.k3) return stamp_of(@@enc_kq_mvb2_k3_c)
    if (f == KqFmt.iq4xs) return stamp_of(@@enc_kq_mvb2_iq4xs_c)
    if (f == KqFmt.k4) return stamp_of(@@enc_kq_mvb2_k4_c)
    if (f == KqFmt.k5) return stamp_of(@@enc_kq_mvb2_k5_c)
    return stamp_of(@@enc_kq_mvb2_k6_c)
}

//! the B4 small-batch kq GEMV stamp of a format
def private kq_mvb4_stamp(f) {
    if (f == KqFmt.iq2xxs) return stamp_of(@@enc_kq_mvb4_iq2xxs_c)
    if (f == KqFmt.iq2xs) return stamp_of(@@enc_kq_mvb4_iq2xs_c)
    if (f == KqFmt.iq2s) return stamp_of(@@enc_kq_mvb4_iq2s_c)
    if (f == KqFmt.q40) return stamp_of(@@enc_kq_mvb4_q40_c)
    if (f == KqFmt.iq4nl) return stamp_of(@@enc_kq_mvb4_iq4nl_c)
    if (f == KqFmt.k2) return stamp_of(@@enc_kq_mvb4_k2_c)
    if (f == KqFmt.iq3xxs) return stamp_of(@@enc_kq_mvb4_iq3xxs_c)
    if (f == KqFmt.iq3s) return stamp_of(@@enc_kq_mvb4_iq3s_c)
    if (f == KqFmt.k3) return stamp_of(@@enc_kq_mvb4_k3_c)
    if (f == KqFmt.iq4xs) return stamp_of(@@enc_kq_mvb4_iq4xs_c)
    if (f == KqFmt.k4) return stamp_of(@@enc_kq_mvb4_k4_c)
    if (f == KqFmt.k5) return stamp_of(@@enc_kq_mvb4_k5_c)
    return stamp_of(@@enc_kq_mvb4_k6_c)
}

//! the B8 small-batch kq GEMV stamp of a format
def private kq_mvb8_stamp(f) {
    if (f == KqFmt.iq2xxs) return stamp_of(@@enc_kq_mvb8_iq2xxs_c)
    if (f == KqFmt.iq2xs) return stamp_of(@@enc_kq_mvb8_iq2xs_c)
    if (f == KqFmt.iq2s) return stamp_of(@@enc_kq_mvb8_iq2s_c)
    if (f == KqFmt.q40) return stamp_of(@@enc_kq_mvb8_q40_c)
    if (f == KqFmt.iq4nl) return stamp_of(@@enc_kq_mvb8_iq4nl_c)
    if (f == KqFmt.k2) return stamp_of(@@enc_kq_mvb8_k2_c)
    if (f == KqFmt.iq3xxs) return stamp_of(@@enc_kq_mvb8_iq3xxs_c)
    if (f == KqFmt.iq3s) return stamp_of(@@enc_kq_mvb8_iq3s_c)
    if (f == KqFmt.k3) return stamp_of(@@enc_kq_mvb8_k3_c)
    if (f == KqFmt.iq4xs) return stamp_of(@@enc_kq_mvb8_iq4xs_c)
    if (f == KqFmt.k4) return stamp_of(@@enc_kq_mvb8_k4_c)
    if (f == KqFmt.k5) return stamp_of(@@enc_kq_mvb8_k5_c)
    return stamp_of(@@enc_kq_mvb8_k6_c)
}

def private kq_mvb_gate(t : T?; var dev, queue; fmt, bwidth, n, d, nr, ys : int; r2 : bool = false) {
    let form = r2 ? "_r2" : ""
    let tag = "kq_mvb{bwidth}_k{fmt}{form} n={n} d={d} nr={nr} ys={ys}"
    with_metal_cell(dev, queue) $(var mc : MetalCell) {
        var err : string
        let f = kq_fmt_of_id(fmt, "kq_mvb_gate")
        let (sr, s2, s4, s8) = (stamp_of(@@enc_kq_mvb2_k4r2_c), kq_mvb2_stamp(f), kq_mvb4_stamp(f), kq_mvb8_stamp(f))
        var pso = r2 ? (mc |> keep_stamp(sr, err)) : (bwidth == 2 ? (mc |> keep_stamp(s2, err)) : (bwidth == 4 ? (mc |> keep_stamp(s4, err)) : (mc |> keep_stamp(s8, err))))
        return if (!pso_ok(t, tag, pso, err))
        let nb = n / 256
        let nsb = d * nb
        var inscope kq, ks : array<uint8>
        kq_fill_planes(fmt, nsb, kq, ks)
        // x is padded to the kernel's column-group width (the driver's mp guarantee); pad rows zero
        let gcols = bwidth == 4 ? (nr + 3) / 4 : 1
        let xpad = bwidth == 2 ? 2 : (bwidth == 4 ? gcols * 4 : 8)
        var inscope xv : array<float>
        xv |> resize(xpad * n)
        for (i in range(xpad * n)) {
            xv[i] = i < nr * n ? 0.09 * float(i % 43) - 1.8 : 0.0
        }
        var inscope dense <- dense_ref_env(nr, n, d, xv) $(r; var wrow) {
            kq_row_ref(fmt, r * nb, nb, nsb, kq, ks, wrow)
        }
        var inscope ref <- strided_ref(dense, d, ys, nr, -1000.0)
        let bks = mc |> in_plain(ks)
        let bkq = mc |> in_plain(kq)
        let bx = mc |> in_(xv)
        let by = mc |> out_words(nr * ys, -1000.0)
        var ka = KqMvArgs(ndim = uint(n), ddim = uint(d), ys = uint(ys), nr = uint(nr))
        let doff = kq_split_doff(fmt, nsb)
        let ran = mc |> cell_record(t, tag) $(enc : MetalComputeEncoder?) {
            let (pks, pkq, px, py) = (mc.buf[bks], mc.buf[bkq], mc.buf[bx], mc.buf[by])
            if (r2) {
                stamp_run(sr, pso) {
                    invoke(sr.enc, enc, pks, doff, pks, 0ul, pkq, 0ul, px, 0ul, py, 0ul, ka, int64(d))
                }
            } elif (bwidth == 2) {
                stamp_run(s2, pso) {
                    invoke(s2.enc, enc, pks, doff, pks, 0ul, pkq, 0ul, px, py, 0ul, ka, int64(d))
                }
            } elif (bwidth == 4) {
                stamp_run(s4, pso) {
                    invoke(s4.enc, enc, pks, doff, pks, 0ul, pkq, 0ul, px, py, 0ul, ka, int64(d), int64(gcols))
                }
            } else {
                stamp_run(s8, pso) {
                    invoke(s8.enc, enc, pks, doff, pks, 0ul, pkq, 0ul, px, py, 0ul, ka, int64(d))
                }
            }
        }
        return if (!ran)
        check_buf(t, tag, mc.buf[by], ref.want, env_bar(2e-4, ref.env), Shape(row_width = ys), ref.env)
    }
}

// the k4 pair walk (enc_kq_mvb_k4_pairs): nr columns as two-row tiles plus an odd single pass, on the
// production decode PSOs; ys > d so a misplaced pair lands in the guard band
def private kq_k4_pairs_gate(t : T?; var queue; n, d, nr, ys : int) {
    let tag = "kq_mvb_k4_pairs n={n} d={d} nr={nr} ys={ys}"
    with_metal_cell(g_dev, queue) $(var mc : MetalCell) {
        let nb = n / 256
        let nsb = d * nb
        var inscope kq, ks : array<uint8>
        kq_fill_planes(4, nsb, kq, ks)
        var inscope xv : array<float>
        xv |> resize(nr * n)
        for (i in range(nr * n)) {
            xv[i] = 0.09 * float(i % 43) - 1.8
        }
        var inscope dense <- dense_ref_env(nr, n, d, xv) $(r; var wrow) {
            kq_row_ref(4, r * nb, nb, nsb, kq, ks, wrow)
        }
        var inscope ref <- strided_ref(dense, d, ys, nr, -1000.0)
        let bks = mc |> in_plain(ks)
        let bkq = mc |> in_plain(kq)
        let bx = mc |> in_(xv)
        let by = mc |> out_words(nr * ys, -1000.0)
        let bn = mc |> in_plain([uint(n)])
        let bd = mc |> in_plain([uint(d)])
        let ran = mc |> cell_record(t, tag) $(enc : MetalComputeEncoder?) {
            enc_kq_mvb_k4_pairs(enc, mc.buf[bkq], 0ul, mc.buf[bks], 0ul, 0ul, mc.buf[bx], mc.buf[by], 0ul, mc.buf[bn], mc.buf[bd], int64(d), int64(n), int64(ys), int64(nr))
        }
        return if (!ran)
        check_buf(t, tag, mc.buf[by], ref.want, env_bar(2e-4, ref.env), Shape(row_width = ys), ref.env)
    }
}

// a format's half-x route (enc_kq_rows_half: k4's wide forms, every other format's small-batch half stamps):
// nr live rows of an f16 x twin against the CPU reference over the same f16-lattice x, on the production decode
// PSOs; the twin holds eight rows, ys > d so a misplaced column lands in the guard band
def private kq_half_gate(t : T?; var queue; fmt, n, d, nr, ys : int) {
    let f = kq_fmt_of_id(fmt, "kq_half_gate")
    let tag = "kq_rows_half_{f} n={n} d={d} nr={nr} ys={ys}"
    with_metal_cell(g_dev, queue) $(var mc : MetalCell) {
        let nb = n / 256
        let nsb = d * nb
        var inscope kq, ks : array<uint8>
        kq_fill_planes(fmt, nsb, kq, ks)
        var inscope xv : array<float>
        xv |> resize(8 * n)
        for (i in range(8 * n)) {
            xv[i] = i < nr * n ? float(float16(0.09 * float(i % 43) - 1.8)) : 0.0
        }
        var inscope xh <- [for (v in xv); float16(v)]
        var inscope dense <- dense_ref_env(nr, n, d, xv) $(r; var wrow) {
            kq_row_ref(fmt, r * nb, nb, nsb, kq, ks, wrow)
        }
        var inscope ref <- strided_ref(dense, d, ys, nr, -1000.0)
        let bks = mc |> in_plain(ks)
        let bkq = mc |> in_plain(kq)
        let bxh = mc |> in_plain(xh)
        let by = mc |> out_words(nr * ys, -1000.0)
        let doff = kq_split_doff(fmt, nsb)
        let ran = mc |> cell_record(t, tag) $(enc : MetalComputeEncoder?) {
            enc_kq_rows_half(enc, f, mc.buf[bkq], 0ul, mc.buf[bks], 0ul, doff, mc.buf[bxh], mc.buf[by], 0ul, int64(d), int64(n), int64(ys), int64(nr))
        }
        return if (!ran)
        check_buf(t, tag, mc.buf[by], ref.want, env_bar(2e-4, ref.env), Shape(row_width = ys), ref.env)
    }
}

//! every K-quant format with a half-x route, by id (the race table's thirteen)
let private KQ_HALF_FORMATS <- [2, 3, 4, 5, 6, 23, 24, 25, 33, 34, 40, 44, 45]

[test]
def test_metal_kq_half_forms(t : T?) {
    t |> run("every format's half-x route matches the CPU reference at every row count the dispatcher serves") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(das_metal)) {
            with_metal_decode_gate(t) $(var queue : MetalCommandQueue?) {
                for (fmt in KQ_HALF_FORMATS) {
                    kq_half_gate(t, queue, fmt, 768, 17, 2, 22)   // the two-column form; d=17 leaves a tail row pair
                    kq_half_gate(t, queue, fmt, 768, 17, 3, 22)   // the four-column form with one pad column
                    kq_half_gate(t, queue, fmt, 768, 33, 4, 40)
                    kq_half_gate(t, queue, fmt, 512, 17, 5, 22)   // the eight-row form with three pad columns
                    kq_half_gate(t, queue, fmt, 512, 33, 8, 40)
                }
            }
        } else {
            t |> skip("das_metal is not built on this platform")
        }
    }
}

[test]
def test_metal_kq_k4_pair_walk(t : T?) {
    t |> run("the k4 pair walk matches the CPU reference at 3, 4 and 5 columns") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(das_metal)) {
            with_metal_decode_gate(t) $(var queue : MetalCommandQueue?) {
                kq_k4_pairs_gate(t, queue, 768, 17, 3, 22)   // one pair + an odd tail; d=17 leaves a tail row pair
                kq_k4_pairs_gate(t, queue, 768, 32, 4, 40)   // two pairs, no tail
                kq_k4_pairs_gate(t, queue, 512, 33, 5, 40)   // two pairs + an odd tail
            }
        } else {
            t |> skip("das_metal is not built on this platform")
        }
    }
}

// ===== the expert-indexed MoeGemv stamps (MetalMoeGemvK4 / K5 / K6 / K3 / Iq4xs / Iq3s / Iq3xxs / Iq2s / Iq2xs / Iq2xxs) =====

//! the expert-indexed kq GEMV stamp of a format
def private moe_gemv_stamp(f) {
    if (f == KqFmt.k4) return stamp_of(@@enc_moe_gemv_k4_c)
    if (f == KqFmt.k5) return stamp_of(@@enc_moe_gemv_k5_c)
    if (f == KqFmt.k6) return stamp_of(@@enc_moe_gemv_k6_c)
    if (f == KqFmt.k3) return stamp_of(@@enc_moe_gemv_k3_c)
    if (f == KqFmt.iq3s) return stamp_of(@@enc_moe_gemv_iq3s_c)
    if (f == KqFmt.iq3xxs) return stamp_of(@@enc_moe_gemv_iq3xxs_c)
    if (f == KqFmt.iq2s) return stamp_of(@@enc_moe_gemv_iq2s_c)
    if (f == KqFmt.iq2xs) return stamp_of(@@enc_moe_gemv_iq2xs_c)
    if (f == KqFmt.iq2xxs) return stamp_of(@@enc_moe_gemv_iq2xxs_c)
    if (f == KqFmt.iq4xs) return stamp_of(@@enc_moe_gemv_iq4xs_c)
    if (f != KqFmt.q40) {
        panic("moe_gemv_stamp: no expert GEMV for {f}")
    }
    return stamp_of(@@enc_moe_gemv_q40_c)
}

//! `f4` runs the iq3s gathered site's f4-slab twin (the dense twin's crown gates it in serving; the gate compiles it unconditionally)
def private moe_kq_gemv_gate(t : T?; var dev, queue; fmt, n, d, net, k, nst : int; perslot : bool; f4 : bool = false) {
    let tag = "moe_gemv_k{fmt}{f4 ? " f4" : ""} n={n} d={d} net={net} k={k} nst={nst} perslot={perslot}"
    with_metal_cell(dev, queue) $(var mc : MetalCell) {
        var err : string
        let st = f4 ? stamp_of(@@enc_moe_gemv_iq3s_f4_c) : moe_gemv_stamp(kq_fmt_of_id(fmt, "moe_kq_gemv_gate"))
        var pso = mc |> keep_stamp(st, err)
        return if (!pso_ok(t, tag, pso, err))
        let nb = n / 256
        let nsb = net * d * nb              // the whole expert stack's superblocks
        var inscope kq, ks : array<uint8>
        kq_fill_planes(fmt, nsb, kq, ks)
        // selection rows: [k idx u32][k w f32] per stream — only the idx section is read here
        var inscope sel : array<uint>
        sel |> resize(nst * 2 * k)
        for (s in range(nst)) {
            for (j in range(k)) {
                sel[s * 2 * k + j] = uint((s + j * 3 + net - 1) % net)
            }
        }
        let xrows = perslot ? nst * k : nst
        var inscope xv : array<float>
        xv |> resize(xrows * n)
        for (i in range(xrows * n)) {
            xv[i] = 0.07 * float(i % 37) - 1.2
        }
        var inscope want, env : array<float>
        want |> reserve(nst * k * d)
        env |> reserve(nst * k * d)
        for (s in range(nst)) {
            for (j in range(k)) {
                let e = int(sel[s * 2 * k + j])
                let x0 = (perslot ? s * k + j : s) * n
                var inscope xs <- [for (c in range(n)); xv[x0 + c]]
                var inscope ref <- dense_ref_env(1, n, d, xs) $(r; var wrow) {
                    kq_row_ref(fmt, (e * d + r) * nb, nb, nsb, kq, ks, wrow)
                }
                want |> push_from(ref.want)
                env |> push_from(ref.env)
            }
        }
        let bks = mc |> in_plain(ks)
        let bkq = mc |> in_plain(kq)
        let bx = mc |> in_(xv)
        let by = mc |> out_words(nst * k * d, -1000.0)
        let bsel = mc |> in_plain(sel)
        var ka = MoeGemvArgs(ndim = uint(n), ddim = uint(d), eplane = uint(d * nb),
            xs = perslot ? uint(n) : 0u, ss = uint(2 * k),
            bxs = uint((perslot ? k : 1) * n), bys = uint(k * d), hasb = 0u)
        let doff = kq_split_doff(fmt, nsb)
        let ran = mc |> cell_record(t, tag) $(enc : MetalComputeEncoder?) {
            stamp_run(st, pso) {
                invoke(st.enc, enc, mc.buf[bks], doff, mc.buf[bks], 0ul, mc.buf[bkq], 0ul, mc.buf[bx], mc.buf[by], 0ul, ka, mc.buf[bsel], int64(d), int64(k), int64(nst))
            }
        }
        return if (!ran)
        check_buf(t, tag, mc.buf[by], want, env_bar(2e-4, env), Shape(row_width = d), env)
    }
}

// ===== the fused QKV rope-store pair (MetalQ8GemvQkvRsF16/F32) =====

// One dispatch = q8 GEMV over all three QKV segments + rope + segment stores: q rows to qo
// (f32), k rows roped into the mirror (f16 clamped / f32), v rows straight. Segments bind ONE
// blob at per-segment byte offsets — the production @off scheme. The exact-arithmetic fixture
// makes qo (and the f32 twin's k/v) BIT-exact; the f16 twin's k/v differ only by store rounding.
def private qkv_rs_gate(t : T?; var dev, queue; f16 : bool; neox, has_bias : bool; ndim, qd, kvd, hs : int) {
    let tag = "qkv_rs{f16 ? 16 : 32}{neox ? " neox" : ""}{has_bias ? " bias" : ""} n={ndim} qd={qd} kvd={kvd}"
    with_metal_cell(dev, queue) $(var mc : MetalCell) {
        var err : string
        let st = f16 ? stamp_of(@@enc_qkvrs16_c) : stamp_of(@@enc_qkvrs32_c)
        var pso = mc |> keep_stamp(st, err)
        return if (!pso_ok(t, tag, pso, err))
        let ddim = qd + 2 * kvd
        let half = hs / 2
        let nb = ndim / 32
        var inscope blob : array<uint8>
        var inscope wdeq : array<float>
        q8_blob_fill(ddim, nb, blob, wdeq)   // one stacked blob: q rows, then k, then v (segment offsets)
        var inscope xf : array<float>
        q8_x_fill(ndim, xf)
        var inscope tcos, tsin : array<float>
        tcos |> resize(half)
        tsin |> resize(half)
        for (j in range(half)) {
            tcos[j] = float((j * 3) % 5 - 2) * 0.5
            tsin[j] = float((j * 7 + 1) % 5 - 2) * 0.25
        }
        var inscope bias : array<float>
        bias |> resize(ddim)
        for (i in range(ddim)) {
            bias[i] = float((i * 13) % 9 - 4) * 0.25
        }
        var inscope qwant, kwant, vwant : array<float>
        qwant |> resize(qd)
        kwant |> resize(kvd)
        vwant |> resize(kvd)
        for (pr in range(ddim / 2)) {
            var row = pr * 2
            var row2 = row + 1
            if (neox && row < qd + kvd) {
                let lp = row < qd ? pr : pr - qd / 2
                let seg = row < qd ? 0 : qd
                row = seg + (lp / half) * hs + lp % half
                row2 = row + half
            }
            var a0 = 0.0lf
            var a1 = 0.0lf
            for (c in range(ndim)) {
                a0 += double(wdeq[row * ndim + c]) * double(xf[c])
                a1 += double(wdeq[row2 * ndim + c]) * double(xf[c])
            }
            if (has_bias) {
                a0 += double(bias[row])
                a1 += double(bias[row2])
            }
            if (row < qd) {
                let j = neox ? row % hs : (row % hs) / 2
                qwant[row] = float(a0 * double(tcos[j]) - a1 * double(tsin[j]))
                qwant[row2] = float(a0 * double(tsin[j]) + a1 * double(tcos[j]))
            } elif (row < qd + kvd) {
                let ik = row - qd
                let j = neox ? ik % hs : (ik % hs) / 2
                kwant[ik] = float(a0 * double(tcos[j]) - a1 * double(tsin[j]))
                kwant[row2 - qd] = float(a0 * double(tsin[j]) + a1 * double(tcos[j]))
            } else {
                let iv = row - qd - kvd
                vwant[iv] = float(a0)
                vwant[iv + 1] = float(a1)
            }
        }
        var ka = Q8QkvRsArgs(ndim = uint(ndim), qd = uint(qd), kvd = uint(kvd),
            hs = uint(hs), neox = neox ? 1u : 0u, has_bias = has_bias ? 1u : 0u)
        let koffb = uint64(qd * nb * 34)         // k segment's byte offset into the stacked blob
        let voffb = uint64((qd + kvd) * nb * 34)
        let bw = mc |> in_plain(blob)
        let bx = mc |> in_(xf)
        let bqo = mc |> out_words(qd, -1000.0)
        let bkd = f16 ? (mc |> out_words(kvd, float16(-1000.0))) : (mc |> out_words(kvd, -1000.0))
        let bvd = f16 ? (mc |> out_words(kvd, float16(-1000.0))) : (mc |> out_words(kvd, -1000.0))
        let bcos = mc |> in_(tcos)
        let bsin = mc |> in_(tsin)
        let bbias = mc |> in_(bias)
        let ran = mc |> cell_record(t, tag) $(enc : MetalComputeEncoder?) {
            let w = mc.buf[bw]   // one blob: the q, k and v segments' byte and half-scale views at their segment offsets
            stamp_run(st, pso) {
                invoke(st.enc, enc, w, 0ul, w, mc.buf[bx], mc.buf[bqo], mc.buf[bkd], 0ul, mc.buf[bvd], 0ul, mc.buf[bcos], mc.buf[bsin], mc.buf[bbias],
                    w, koffb, w, w, voffb, w, ka, int64(ddim / 2))
            }
        }
        return if (!ran)
        let heads = Shape(row_width = hs)
        check_buf(t, "{tag} q", mc.buf[bqo], qwant, BAR_EXACT, heads)
        if (f16) {
            check_buf_h(t, "{tag} k", mc.buf[bkd], kwant, BAR_F16_STORE, heads)
            check_buf_h(t, "{tag} v", mc.buf[bvd], vwant, BAR_F16_STORE, heads)
        } else {
            check_buf(t, "{tag} k", mc.buf[bkd], kwant, BAR_EXACT, heads)
            check_buf(t, "{tag} v", mc.buf[bvd], vwant, BAR_EXACT, heads)
        }
    }
}

// ===== the fused W1|W3 + activation family =====

def private silu_gelu_ref(g, u : double; gelu : bool) : float {
    if (!gelu) {
        return float((g / (1.0lf + exp(-g))) * u)
    }
    let ga = 0.7978845608028654lf * (g + 0.044715lf * g * g * g)
    return float(0.5lf * g * (2.0lf - 2.0lf / (1.0lf + exp(2.0lf * ga))) * u)
}

//! the fused W1|W3 reference over `nrows` rows of `xf`: gate rows [0, hidden) and up rows [hidden, 2 * hidden) of `wdeq`
//! dotted in double, act(g) * u at [row * hidden + r]
def private w13_want(wdeq, xf : array<float>; nrows, ndim, hidden : int; gelu : bool) : array<float> {
    var want : array<float>
    want |> resize(nrows * hidden)
    for (b in range(nrows)) {
        for (r in range(hidden)) {
            var g = 0.0lf
            var u = 0.0lf
            for (c in range(ndim)) {
                g += double(wdeq[r * ndim + c]) * double(xf[b * ndim + c])
                u += double(wdeq[(hidden + r) * ndim + c]) * double(xf[b * ndim + c])
            }
            want[b * hidden + r] = silu_gelu_ref(g, u, gelu)
        }
    }
    return <- want
}

// MetalQ8GemvW13Sw: one threadgroup dots gate row (w1) AND up row (w3), stores act(g)·u.
//! the row-per-simdgroup form takes 4 elements per threadgroup, on the ceil grid behind a row guard; else the split form
def private w13sw_gate(t : T?; var dev, queue; gelu : bool; ndim, hidden : int; row_per_sg : bool = false) {
    let tag = "w13sw{gelu ? " gelu" : ""}{row_per_sg ? " sg" : ""} n={ndim} h={hidden}"
    with_metal_cell(dev, queue) $(var mc : MetalCell) {
        var err : string
        let st = row_per_sg ? stamp_of(@@enc_w13sw_sg_c) : stamp_of(@@enc_w13sw_c)
        var pso = mc |> keep_stamp(st, err)
        return if (!pso_ok(t, tag, pso, err))
        let nb = ndim / 32
        var inscope blob1 : array<uint8>
        var inscope wdeq1 : array<float>
        q8_blob_fill(2 * hidden, nb, blob1, wdeq1)   // one generator, two slices: w1 rows then w3 rows
        var inscope blob3 <- [for (i in range(hidden * nb * 34)); blob1[hidden * nb * 34 + i]]
        var inscope xf : array<float>
        q8_x_fill(ndim, xf)
        var inscope want <- w13_want(wdeq1, xf, 1, ndim, hidden, gelu)
        let bw1 = mc |> in_plain(blob1)
        let bw3 = mc |> in_plain(blob3)
        let bx = mc |> in_(xf)
        // the output carries a sentinel tail past hidden: the !SPLIT stamp's row guard (r >= hidden)
        // is otherwise unobservable, because its ceil grid hands the last threadgroup up to three
        // rows past hidden and a guard that let them through would write exactly here
        let tailn = 8
        let sentinel = -1000.0
        let bh = mc |> out_words(hidden + tailn, sentinel)
        let bn = mc |> in_plain([uint(ndim)])
        let bhid = mc |> in_plain([uint(hidden)])
        let bact = mc |> in_plain([gelu ? 1u : 0u])
        let ran = mc |> cell_record(t, tag) $(enc : MetalComputeEncoder?) {
            stamp_run(st, pso) {
                invoke(st.enc, enc, mc.buf[bw1], 0ul, mc.buf[bw1], mc.buf[bx], mc.buf[bh], mc.buf[bn], mc.buf[bhid], mc.buf[bw3], 0ul, mc.buf[bw3], mc.buf[bact], int64(hidden))
            }
        }
        return if (!ran)
        check_buf(t, tag, mc.buf[bh], want, lead_poisoned(BAR_Q8_APPROX, want), FLAT_SHAPE)
        var inscope got <- mc |> plane_run(bh)
        var touched = 0
        for (i in range(tailn)) {
            touched += got[hidden + i] == sentinel ? 0 : 1
        }
        t |> equal(touched, 0, "{tag}: the row guard kept the sentinel tail past hidden")
    }
}

// MetalGemvW13SwB2/B4: the fixed-B batch twins (silu only; binding 1 is SKIPPED by the class —
// scale views sit at 2/9). ndim = 320 walks one full 256-elem X panel plus the strided tail.
def private w13sw_b_gate(t : T?; var dev, queue; bwidth, ndim, hidden : int) {
    let tag = "w13sw_b{bwidth} n={ndim} h={hidden}"
    with_metal_cell(dev, queue) $(var mc : MetalCell) {
        var err : string
        let st = bwidth == 2 ? stamp_of(@@enc_w13sw_b2_c) : stamp_of(@@enc_w13sw_b4_c)
        var pso = mc |> keep_stamp(st, err)
        return if (!pso_ok(t, tag, pso, err))
        let nb = ndim / 32
        var inscope blob1 : array<uint8>
        var inscope wdeq1 : array<float>
        q8_blob_fill(2 * hidden, nb, blob1, wdeq1)
        var inscope blob3 <- [for (i in range(hidden * nb * 34)); blob1[hidden * nb * 34 + i]]
        var inscope xf : array<float>
        q8_x_fill(bwidth * ndim, xf)
        var inscope want <- w13_want(wdeq1, xf, bwidth, ndim, hidden, false)
        let bw1 = mc |> in_plain(blob1)
        let bw3 = mc |> in_plain(blob3)
        let bx = mc |> in_(xf)
        let by = mc |> out_words(bwidth * hidden, -1000.0)
        let bn = mc |> in_plain([uint(ndim)])
        let bhid = mc |> in_plain([uint(hidden)])
        let ran = mc |> cell_record(t, tag) $(enc : MetalComputeEncoder?) {
            stamp_run(st, pso) {
                invoke(st.enc, enc, mc.buf[bw1], 0ul, mc.buf[bw1], mc.buf[bx], 0ul, mc.buf[by], 0ul, mc.buf[bn], mc.buf[bhid], mc.buf[bw3], 0ul, mc.buf[bw3], int64(hidden))
            }
        }
        return if (!ran)
        check_buf(t, tag, mc.buf[by], want, lead_poisoned(BAR_Q8_APPROX, want), Shape(row_width = hidden))
    }
}

// ===== the fixed-B batched q8 GEMV twins =====

// MetalGemvB2/B4 (x-staged panel form): dense [B x ndim] X, y at ROW stride ys — the exact
// fixture makes every output BIT-exact, and ys > ddim gap columns stay sentinel.
def private gemv_b_gate(t : T?; var dev, queue; bwidth, ndim, ddim, ys : int) {
    let tag = "gemv_b{bwidth} n={ndim} d={ddim} ys={ys}"
    with_metal_cell(dev, queue) $(var mc : MetalCell) {
        var err : string
        let st = bwidth == 2 ? stamp_of(@@enc_gemv_b2_c) : stamp_of(@@enc_gemv_b4_c)
        var pso = mc |> keep_stamp(st, err)
        return if (!pso_ok(t, tag, pso, err))
        var inscope blob : array<uint8>
        var inscope wdeq : array<float>
        q8_blob_fill(ddim, ndim / 32, blob, wdeq)
        var inscope xf : array<float>
        q8_x_fill(bwidth * ndim, xf)
        var inscope ref <- strided_ref(dense_ref_rows(wdeq, xf, bwidth, ndim, ddim), ddim, ys, bwidth, -1000.0)
        let bw = mc |> in_plain(blob)
        let bx = mc |> in_(xf)
        let by = mc |> out_words(bwidth * ys, -1000.0)
        let bn = mc |> in_plain([uint(ndim)])
        let bd = mc |> in_plain([uint(ddim)])
        let bys = mc |> in_plain([uint(ys)])
        let ran = mc |> cell_record(t, tag) $(enc : MetalComputeEncoder?) {
            stamp_run(st, pso) {
                invoke(st.enc, enc, mc.buf[bw], 0ul, mc.buf[bw], mc.buf[bx], 0ul, mc.buf[by], 0ul, mc.buf[bn], mc.buf[bd], mc.buf[bys], int64(ddim))
            }
        }
        return if (!ran)
        check_buf(t, tag, mc.buf[by], ref.want, BAR_EXACT, Shape(row_width = ys))
    }
}

// MetalQ8MvB2/B4 (streamed skinny form, 64-thread tg; b2 walks 256-quant stripes, b4 128):
// X rows at float4 stride xs4; BOTH forms gate stores on nrows — pad columns compute and
// discard (b4's x2/x3 clamp; the x1 phantom read stays in the padded x rows), so rows past
// nrows keep the sentinel and the compare asserts it.
def private q8_mv_b_gate(t : T?; var dev, queue; bwidth, ndim, ddim, nrows, xs4, ys : int) {
    let tag = "q8_mv_b{bwidth} n={ndim} d={ddim} nr={nrows} xs4={xs4} ys={ys}"
    with_metal_cell(dev, queue) $(var mc : MetalCell) {
        var err : string
        let st = bwidth == 2 ? stamp_of(@@enc_mv_b2_c) : stamp_of(@@enc_mv_b4_c)
        var pso = mc |> keep_stamp(st, err)
        return if (!pso_ok(t, tag, pso, err))
        var inscope blob : array<uint8>
        var inscope wdeq : array<float>
        q8_blob_fill(ddim, ndim / 32, blob, wdeq)
        var inscope xf : array<float>
        q8_x_fill(max(nrows, 2) * xs4 * 4, xf)   // the unclamped x1 phantom read needs a row-1 backing
        let nwr = bwidth == 2 ? 2 : 4   // the form's column count: rows past nrows must stay sentinel
        var inscope want : array<float>
        want |> resize(nwr * ys)
        for (i in range(nwr * ys)) {
            want[i] = -1000.0
        }
        for (b in range(nrows)) {
            for (r in range(ddim)) {
                var acc = 0.0lf
                for (c in range(ndim)) {
                    acc += double(wdeq[r * ndim + c]) * double(xf[b * xs4 * 4 + c])
                }
                want[b * ys + r] = float(acc)
            }
        }
        var ka = Q8MvArgs(ndim = uint(ndim), ddim = uint(ddim), nrows = uint(nrows),
            xs4 = uint(xs4), ys = uint(ys))
        let bw = mc |> in_plain(blob)
        let bx = mc |> in_(xf)
        let by = mc |> out_words(nwr * ys, -1000.0)
        let ran = mc |> cell_record(t, tag) $(enc : MetalComputeEncoder?) {
            stamp_run(st, pso) {
                invoke(st.enc, enc, mc.buf[bw], 0ul, mc.buf[bw], mc.buf[bx], 0ul, mc.buf[by], 0ul, ka, int64(ddim))
            }
        }
        return if (!ran)
        check_buf(t, tag, mc.buf[by], want, BAR_EXACT, Shape(row_width = ys))
    }
}

[test]
def test_metal_kq_gemv_kernels(t : T?) {
    t |> run("kq GEMV / KqMv / MoeGemv-kq kernels == the plane-dequant CPU reference") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(das_metal)) {
            with_metal_gate(t) $(var dev : MetalDevice?; var queue : MetalCommandQueue?) {
                // single-stream: all four variants; d = 11 exercises the row-guard tails
                kq_gemv_gate(t, dev, queue, "k4", 1280, 11)
                kq_gemv_gate(t, dev, queue, "k5", 1280, 11)
                kq_gemv_gate(t, dev, queue, "k5c", 1280, 11)
                kq_gemv_gate(t, dev, queue, "k6", 1280, 11)
                kq_gemv_gate(t, dev, queue, "k4", 512, 30)
                kq_gemv_gate(t, dev, queue, "k6", 512, 30)
                kq_gemv_gate(t, dev, queue, "iq4xs", 1280, 11)
                kq_gemv_gate(t, dev, queue, "iq4xs", 512, 30)
                kq_gemv_gate(t, dev, queue, "k3", 1280, 11)
                kq_gemv_gate(t, dev, queue, "k3", 512, 30)
                kq_gemv_gate(t, dev, queue, "iq3s", 1280, 11)
                kq_gemv_gate(t, dev, queue, "iq3s", 512, 30)
                kq_gemv_gate(t, dev, queue, "iq3xxs", 1280, 11)
                kq_gemv_gate(t, dev, queue, "iq3xxs", 512, 30)
                kq_gemv_gate(t, dev, queue, "iq3sf4", 1280, 11)
                kq_gemv_gate(t, dev, queue, "iq3sf4", 512, 30)
                kq_gemv_gate(t, dev, queue, "iq3xxsf4", 1280, 11)
                kq_gemv_gate(t, dev, queue, "iq3xxsf4", 512, 30)
                kq_gemv_gate(t, dev, queue, "iq4nl", 1280, 11)
                kq_gemv_gate(t, dev, queue, "iq4nl", 512, 30)
                kq_gemv_gate(t, dev, queue, "q40", 1280, 11)
                kq_gemv_gate(t, dev, queue, "q40", 512, 30)
                kq_gemv_gate(t, dev, queue, "k2", 1280, 11)
                kq_gemv_gate(t, dev, queue, "k2", 512, 30)
                kq_gemv_gate(t, dev, queue, "iq2s", 1280, 11)
                kq_gemv_gate(t, dev, queue, "iq2s", 512, 30)
                kq_gemv_gate(t, dev, queue, "iq2xs", 1280, 11)
                kq_gemv_gate(t, dev, queue, "iq2xs", 512, 30)
                kq_gemv_gate(t, dev, queue, "iq2xxs", 1280, 11)
                kq_gemv_gate(t, dev, queue, "iq2xxs", 512, 30)
                kq_gemv_gate(t, dev, queue, "iq2xxsf4", 1280, 11)
                kq_gemv_gate(t, dev, queue, "iq2xxsf4", 512, 30)
                // the small-batch twins: nr = 2 / 3 (b4 col guard) / 6 (b8 pad), ys > d
                for (fmt in [4, 5, 6, 44, 3, 33, 34, 45, 40, 2, 23, 24, 25]) {
                    kq_mvb_gate(t, dev, queue, fmt, 2, 768, 17, 2, 22)
                    if (fmt == 4) {   // the two-row tile: d=17 leaves a tail pair with one live row, d=32 none
                        kq_mvb_gate(t, dev, queue, fmt, 2, 768, 17, 2, 22, true)
                        kq_mvb_gate(t, dev, queue, fmt, 2, 768, 32, 2, 40, true)
                    }
                    kq_mvb_gate(t, dev, queue, fmt, 4, 768, 17, 3, 22)
                    kq_mvb_gate(t, dev, queue, fmt, 4, 768, 17, 6, 22)   // two column groups: the reduce's colbase term
                    kq_mvb_gate(t, dev, queue, fmt, 8, 768, 17, 6, 22)
                }
                // expert-indexed: shared-x (gate/up) and per-slot (down) forms
                for (fmt in [4, 5, 6, 3, 33, 34, 23, 24, 25, 44, 40]) {
                    moe_kq_gemv_gate(t, dev, queue, fmt, 512, 9, 5, 3, 2, false)
                    moe_kq_gemv_gate(t, dev, queue, fmt, 512, 9, 5, 3, 2, true)
                }
                // the q40 expert row at the gemma-4-26B-A4B decode geometry: n = dim 2816 (the
                // gate/up site's reduction), d = 7 rows of the 704-wide stack
                moe_kq_gemv_gate(t, dev, queue, 40, 2816, 7, 5, 3, 2, false)
                // the iq3s gathered f4-slab twin: the cell shapes above (a row tail past the 8-row tile), then the
                // Qwen3.6-35B expert geometry - gate/up n = dim 2048 over d = 512, down n = 512 over d = 2048
                moe_kq_gemv_gate(t, dev, queue, 33, 512, 9, 5, 3, 2, false, true)
                moe_kq_gemv_gate(t, dev, queue, 33, 512, 9, 5, 3, 2, true, true)
                moe_kq_gemv_gate(t, dev, queue, 33, 2048, 512, 4, 2, 2, false, true)
                moe_kq_gemv_gate(t, dev, queue, 33, 512, 2048, 4, 2, 1, true, true)
                // the fused QKV rope-store pair: norm/neox x bias x f16/f32 mirror
                qkv_rs_gate(t, dev, queue, true, false, true, 64, 16, 8, 8)
                qkv_rs_gate(t, dev, queue, true, true, false, 64, 16, 8, 8)
                qkv_rs_gate(t, dev, queue, false, true, true, 64, 16, 8, 8)
                qkv_rs_gate(t, dev, queue, false, false, false, 64, 16, 8, 8)
                // production-depth reduction: nb=128 blocks puts work on all 4 simdgroups AND
                // wraps the 32-block round - a gate-shape ndim leaves sgs 1-3 idle and the
                // wrap unexecuted (both were mutation-deletable with the suite green)
                qkv_rs_gate(t, dev, queue, true, true, true, 4096, 16, 8, 8)
                // the fused W1|W3 + activation family (h=18: the row guard / min-clamp tails)
                w13sw_gate(t, dev, queue, false, 320, 18)
                w13sw_gate(t, dev, queue, true, 64, 8)
                w13sw_gate(t, dev, queue, false, 2048, 6)   // nb=64: the multi-round wrap arm
                w13sw_gate(t, dev, queue, false, 320, 18, row_per_sg = true)   // h=18 leaves a partial threadgroup behind the row guard
                w13sw_gate(t, dev, queue, true, 64, 8, row_per_sg = true)
                w13sw_gate(t, dev, queue, false, 896, 12, row_per_sg = true)   // nb=28: three full 8-block rounds plus a partial one per simdgroup
                w13sw_gate(t, dev, queue, false, 320, 21, row_per_sg = true)   // h%4 == 1: the row guard's widest slack - three rows past hidden in the last threadgroup
                w13sw_b_gate(t, dev, queue, 2, 320, 18)
                w13sw_b_gate(t, dev, queue, 4, 320, 18)
                // the fixed-B q8 GEMV twins: panel+tail ndim, min-clamp d, ys gaps, padded xs4,
                // and the b4 nrows=3 store gate / pad-column clamp
                gemv_b_gate(t, dev, queue, 2, 320, 18, 18)
                gemv_b_gate(t, dev, queue, 2, 256, 8, 24)
                gemv_b_gate(t, dev, queue, 4, 320, 18, 18)
                gemv_b_gate(t, dev, queue, 4, 256, 8, 24)
                gemv_b_gate(t, dev, queue, 4, 2112, 18, 18)   // K off the 256 stripe (gemma-4-26B's dense hidden): the tail loop, not the mv forms' contract
                gemv_b_gate(t, dev, queue, 2, 2112, 8, 24)
                q8_mv_b_gate(t, dev, queue, 2, 512, 11, 2, 128, 11)
                q8_mv_b_gate(t, dev, queue, 2, 256, 8, 2, 68, 20)
                q8_mv_b_gate(t, dev, queue, 4, 512, 11, 3, 128, 11)
                q8_mv_b_gate(t, dev, queue, 4, 256, 8, 4, 64, 8)
                // the store-gate controls: nrows=1 on b2 (row 1 must stay sentinel — the
                // ungated kernel phantom-wrote it) and nrows=2 on b4 (the GEMV tail's r==2 shape)
                q8_mv_b_gate(t, dev, queue, 2, 512, 11, 1, 128, 11)
                q8_mv_b_gate(t, dev, queue, 4, 512, 11, 2, 128, 11)
            }
        } else {
            t |> skip("das_metal is not built on this platform; kq GEMV kernel checks skipped")
        }
    }
}

// ===== the verify's q8 row tilers (enc_gemv_rows / enc_gemv_w13sw_rows over the decode PSOs) =====

// the rows a tiler writes at nrows live rows: the B2 form covers two, every B4 tile four
def private tiler_rows_written(nrows : int) : int => nrows <= 2 ? 2 : ((nrows + 3) / 4) * 4

//! the compare covers the whole padded panel, so a live row the tiler skipped keeps its sentinel and reads as a mismatch
def private gemv_rows_gate(t : T?; var queue; nrows, ndim, ddim, ys : int) {
    let tag = "gemv_rows nrows={nrows} n={ndim} d={ddim} ys={ys}"
    with_metal_cell(g_dev, queue) $(var mc : MetalCell) {
        let mp = ((nrows + 3) / 4) * 4
        let wrote = tiler_rows_written(nrows)
        var inscope blob : array<uint8>
        var inscope wdeq : array<float>
        q8_blob_fill(ddim, ndim / 32, blob, wdeq)
        var inscope xf : array<float>
        q8_x_fill(nrows * ndim, xf)
        xf |> resize(mp * ndim)
        var inscope ref <- strided_ref(dense_ref_rows(wdeq, xf, wrote, ndim, ddim), ddim, ys, mp, -1000.0)
        let bw = mc |> in_plain(blob)
        let bx = mc |> in_(xf)
        let by = mc |> out_words(mp * ys, -1000.0)
        let bn = mc |> in_plain([uint(ndim)])
        let bd = mc |> in_plain([uint(ddim)])
        let bys = mc |> in_plain([uint(ys)])
        let ran = mc |> cell_record(t, tag) $(enc : MetalComputeEncoder?) {
            enc_gemv_rows(enc, int64(nrows), mc.buf[bw], 0ul, mc.buf[bx], mc.buf[by], mc.buf[bn], mc.buf[bd], mc.buf[bys], int64(ndim), int64(ddim), int64(ys))
        }
        return if (!ran)
        check_buf(t, tag, mc.buf[by], ref.want, lead_poisoned(BAR_EXACT, ref.want), Shape(row_width = ys))
    }
}

//! the row-per-simdgroup dense q8 GEMV over the decode PSO: every row against the CPU dot, the rows past the last full threadgroup included
def private gemv_sg_gate(t : T?; var queue; ndim, ddim : int) {
    let tag = "gemv_sg n={ndim} d={ddim}"
    with_metal_cell(g_dev, queue) $(var mc : MetalCell) {
        var inscope blob : array<uint8>
        var inscope wdeq : array<float>
        q8_blob_fill(ddim, ndim / 32, blob, wdeq)
        var inscope xf : array<float>
        q8_x_fill(ndim, xf)
        var inscope ref <- dense_ref_rows(wdeq, xf, 1, ndim, ddim)
        let bw = mc |> in_plain(blob)
        let bx = mc |> in_(xf)
        let by = mc |> out_words(ddim, -1000.0)
        let bn = mc |> in_plain([uint(ndim)])
        let bd = mc |> in_plain([uint(ddim)])
        let ran = mc |> cell_record(t, tag) $(enc : MetalComputeEncoder?) {
            enc_gemv_sg(enc, mc.buf[bw], 0ul, mc.buf[bx], mc.buf[by], mc.buf[bn], mc.buf[bd], int64(ddim))
        }
        return if (!ran)
        check_buf(t, tag, mc.buf[by], ref.want, lead_poisoned(BAR_EXACT, ref.want), FLAT_SHAPE)
    }
}

// the fused W1|W3 + swiglu tiler on one blob holding the w1 rows then the w3 rows
def private w13sw_rows_gate(t : T?; var queue; nrows, ndim, hidden : int) {
    let tag = "w13sw_rows nrows={nrows} n={ndim} h={hidden}"
    with_metal_cell(g_dev, queue) $(var mc : MetalCell) {
        let mp = ((nrows + 3) / 4) * 4
        let wrote = tiler_rows_written(nrows)
        let nb = ndim / 32
        var inscope blob : array<uint8>
        var inscope wdeq : array<float>
        q8_blob_fill(2 * hidden, nb, blob, wdeq)
        let w3off = uint64(hidden * nb * 34)
        var inscope xf : array<float>
        q8_x_fill(nrows * ndim, xf)
        xf |> resize(mp * ndim)
        var inscope want <- w13_want(wdeq, xf, wrote, ndim, hidden, false)
        want |> resize(mp * hidden)
        for (i in range(wrote * hidden, mp * hidden)) {
            want[i] = -1000.0
        }
        let bw = mc |> in_plain(blob)
        let bx = mc |> in_(xf)
        let by = mc |> out_words(mp * hidden, -1000.0)
        let bn = mc |> in_plain([uint(ndim)])
        let bhid = mc |> in_plain([uint(hidden)])
        let ran = mc |> cell_record(t, tag) $(enc : MetalComputeEncoder?) {
            enc_gemv_w13sw_rows(enc, int64(nrows), mc.buf[bw], 0ul, w3off, mc.buf[bx], mc.buf[by], mc.buf[bn], mc.buf[bhid], int64(ndim), int64(hidden))
        }
        return if (!ran)
        check_buf(t, tag, mc.buf[by], want, lead_poisoned(BAR_Q8_APPROX, want), Shape(row_width = hidden))
    }
}

[test]
def test_metal_gemv_rows_tiles(t : T?) {
    t |> run("the verify's q8 row tilers write every live row at 2, 5 and 9 rows") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(das_metal)) {
            with_metal_decode_gate(t) $(var queue : MetalCommandQueue?) {
                gemv_rows_gate(t, queue, 2, 256, 8, 24)
                gemv_rows_gate(t, queue, 5, 256, 8, 24)
                gemv_rows_gate(t, queue, int(MTP_MAX_ROWS), 256, 8, 24)
                w13sw_rows_gate(t, queue, 5, 320, 18)
                w13sw_rows_gate(t, queue, 9, 320, 18)
            }
        } else {
            t |> skip("das_metal is not built on this platform")
        }
    }
}

[test]
def test_metal_gemv_sg(t : T?) {
    t |> run("the row-per-simdgroup q8 GEMV matches the CPU dot on the tail, the strided walk and a 0.5B site") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(das_metal)) {
            with_metal_decode_gate(t) $(var queue : MetalCommandQueue?) {
                gemv_sg_gate(t, queue, 320, 17)      // nb4=80: the tail walk alone; d=17 = two threadgroups, the second one row deep
                gemv_sg_gate(t, queue, 2048, 40)     // nb4=512: four 128-strides of the batched walk, then the tail
                gemv_sg_gate(t, queue, 896, 1152)    // the Qwen2.5-0.5B fused-QKV width
            }
        } else {
            t |> skip("das_metal is not built on this platform")
        }
    }
}
