options gen2
options stack = 524288   // every dasLLAMA program root takes this budget
options _function_length = 0        // per-kernel gates: fixture + dispatch + oracle compare, one flat run each
options _cyclomatic_complexity = 0  // same — the arm ladders follow the kernel's own binding/variant axes
options _dasllama_internal = true

require dastest/testing_boost public
require daslib/math_bits            // float_bits_to_uint / uint_bits_to_float — the bf16 halfword pun
require ?das_metal dasllama/dasllama_metal_kernels  // the kq mul_mm trio (Apple builds)
require ?das_metal dasllama/dasllama_metal_prefill  // the mul_mm twins (bf16 + Metal-4 tensor forms)
require ?das_metal dasllama/dasllama_metal_common   // the pipeline globals the generated builders read
require ?das_metal metal/das_metal_boost  // device/pipeline/buffer plumbing + the leak gate
require ?das_metal _metal_kernel_common   // the MetalCell record, check_buf, the device shell, kq plane builders
require dasllama/dasllama_convert         // devw_dequant_*_core — the image mint's dev-W bake mirrors
require dasllama/dasllama_gemm_schema     // kq_qsb — the ONE per-format quant-plane stride source
require dasllama/dasllama_kqformat        // KqFmt: the per-format stamp tables' key
require _compares   // check_exact: the double-buffered arms' bit identity
require _kernel_oracles   // padded_bases: the MoE route's padded prefix; dense_ref_env / dense_ref_rows / strided_ref
require math
require strings

// The production GEMM corpus vs CPU references on synthetic planes: the prefill K-quant
// mul_mm trio (K4/K5/K6) and every Metal-4 tensor twin (kq T trio, Q8MulMmT, Bf16MulMm/T),
// plus the batched-decode q8 family — MetalQ8GemmB[T] (M-pad-32, y at token-row stride ys),
// the split-K pair MetalQ8GemmBSk[T] + MetalSkReduce run as the production two-dispatch
// chain, and the 64-wide N tile MetalQ8Gemm64B[T]. K-quant truth dequants rows through the
// shipped dequant_k*_plane_superblock and dots in double under the envelope bar (float GPU
// accumulation reassociates); bf16 truth dequants the halfword pun exactly; the q8 fixtures
// use pow2 scales + small-int values so every staged f16 value is exact. Prefill shapes ride
// the mul_mm grid contract: m % 32, ndim % 64, kdim % 256 (kq) or % 32, y token-major
// [m x ndim]. A tensor twin skips only when the CAPABILITY probe says the toolchain has no
// Metal-4 tensor support (metal4_tensor_available); with the toolchain proven, a PSO that
// fails to compile is a RED, and the simdgroup bases always assert. Without das_metal in the
// build, or with no Metal device, every check skips.

//! the simdgroup kq mul_mm stamp of a format
def private kq_mm_stamp(f) {   //! generic, as the gates are: the Apple half alone instantiates it
    if (f == KqFmt.iq2xxs) return stamp_of(@@enc_kq_mm_iq2xxs_c)
    if (f == KqFmt.iq2xs) return stamp_of(@@enc_kq_mm_iq2xs_c)
    if (f == KqFmt.iq2s) return stamp_of(@@enc_kq_mm_iq2s_c)
    if (f == KqFmt.q40) return stamp_of(@@enc_kq_mm_q40_c)
    if (f == KqFmt.iq4nl) return stamp_of(@@enc_kq_mm_iq4nl_c)
    if (f == KqFmt.k2) return stamp_of(@@enc_kq_mm_k2_c)
    if (f == KqFmt.iq3xxs) return stamp_of(@@enc_kq_mm_iq3xxs_c)
    if (f == KqFmt.iq3s) return stamp_of(@@enc_kq_mm_iq3s_c)
    if (f == KqFmt.k3) return stamp_of(@@enc_kq_mm_k3_c)
    if (f == KqFmt.iq4xs) return stamp_of(@@enc_kq_mm_iq4xs_c)
    if (f == KqFmt.k4) return stamp_of(@@enc_kq_mm_k4_c)
    if (f == KqFmt.k5) return stamp_of(@@enc_kq_mm_k5_c)
    return stamp_of(@@enc_kq_mm_k6_c)
}

//! the 128-row f16-X Metal-4 tensor kq mul_mm kernel of a format (the tall stamps)
def private kq_mm_th128_kernel(f) {
    if (f == KqFmt.iq3xxs) return kernel_of(@@enc_kq_mm_iq3xxs_th128_c)
    if (f == KqFmt.iq3s) return kernel_of(@@enc_kq_mm_iq3s_th128_c)
    if (f == KqFmt.k4) return kernel_of(@@enc_kq_mm_k4_th128_c)
    if (f == KqFmt.k5) return kernel_of(@@enc_kq_mm_k5_th128_c)
    return kernel_of(@@enc_kq_mm_k6_th128_c)
}

//! the 32-row Metal-4 tensor kq mul_mm kernel of a format, f16 X (`halfx`) or f32 X
def private kq_mm_tensor_kernel(f; halfx : bool) {
    if (f == KqFmt.iq2xxs) return halfx ? kernel_of(@@enc_kq_mm_iq2xxs_th_c) : kernel_of(@@enc_kq_mm_iq2xxs_t_c)
    if (f == KqFmt.iq2xs) return halfx ? kernel_of(@@enc_kq_mm_iq2xs_th_c) : kernel_of(@@enc_kq_mm_iq2xs_t_c)
    if (f == KqFmt.iq2s) return halfx ? kernel_of(@@enc_kq_mm_iq2s_th_c) : kernel_of(@@enc_kq_mm_iq2s_t_c)
    if (f == KqFmt.q40) return halfx ? kernel_of(@@enc_kq_mm_q40_th_c) : kernel_of(@@enc_kq_mm_q40_t_c)
    if (f == KqFmt.iq4nl) return halfx ? kernel_of(@@enc_kq_mm_iq4nl_th_c) : kernel_of(@@enc_kq_mm_iq4nl_t_c)
    if (f == KqFmt.k2) return halfx ? kernel_of(@@enc_kq_mm_k2_th_c) : kernel_of(@@enc_kq_mm_k2_t_c)
    if (f == KqFmt.iq3xxs) return halfx ? kernel_of(@@enc_kq_mm_iq3xxs_th_c) : kernel_of(@@enc_kq_mm_iq3xxs_t_c)
    if (f == KqFmt.iq3s) return halfx ? kernel_of(@@enc_kq_mm_iq3s_th_c) : kernel_of(@@enc_kq_mm_iq3s_t_c)
    if (f == KqFmt.k3) return halfx ? kernel_of(@@enc_kq_mm_k3_th_c) : kernel_of(@@enc_kq_mm_k3_t_c)
    if (f == KqFmt.iq4xs) return halfx ? kernel_of(@@enc_kq_mm_iq4xs_th_c) : kernel_of(@@enc_kq_mm_iq4xs_t_c)
    if (f == KqFmt.k4) return halfx ? kernel_of(@@enc_kq_mm_k4_th_c) : kernel_of(@@enc_kq_mm_k4_t_c)
    if (f == KqFmt.k5) return halfx ? kernel_of(@@enc_kq_mm_k5_th_c) : kernel_of(@@enc_kq_mm_k5_t_c)
    return halfx ? kernel_of(@@enc_kq_mm_k6_th_c) : kernel_of(@@enc_kq_mm_k6_t_c)
}

def private kq_mulmm_gate(t : T?; var dev, queue; fmt : int; tensor : bool; m, kdim, ndim : int; halfx : bool = false; tall : bool = false) {
    let tag = "kq_mulmm_k{fmt}{tensor ? "t" : ""}{tall ? " tall" : ""}{halfx ? " halfx" : ""} m={m} k={kdim} n={ndim}"
    with_metal_cell(dev, queue) $(var mc : MetalCell) {
        var err : string
        let f = kq_fmt_of_id(fmt, "kq_mulmm_gate")
        let sb = kq_mm_stamp(f)
        let kt = tall ? kq_mm_th128_kernel(f) : kq_mm_tensor_kernel(f, halfx)
        var pso = tensor ? (mc |> keep_stamp(kt, err)) : (mc |> keep_stamp(sb, err))
        return if (!(tensor ? tensor_pso_or_skip(t, tag, pso, dev, err) : pso_ok(t, tag, pso, err)))
        let nb = kdim / 256
        let nsb = ndim * nb
        var inscope kq, ks : array<uint8>
        kq_fill_planes(fmt, nsb, kq, ks)
        var inscope xf : array<float>
        xf |> resize(m * kdim)
        for (i in range(m * kdim)) {
            // halfx: round-trip through f16 so the oracle dot below stays the arm's exact reference
            xf[i] = halfx ? float(float16(0.07 * float(i % 39) - 1.3)) : 0.07 * float(i % 39) - 1.3
        }
        var inscope xh : array<float16>
        if (halfx) {
            xh |> resize(m * kdim)
            for (i in range(m * kdim)) {
                xh[i] = float16(xf[i])
            }
        }
        var inscope ref <- dense_ref_env(m, kdim, ndim, xf) $(j; var wrow) {
            kq_row_ref(fmt, j * nb, nb, nsb, kq, ks, wrow)
        }
        let bks = mc |> in_plain(ks)
        let bkq = mc |> in_plain(kq)
        let bx = halfx ? (mc |> in_plain(xh)) : (mc |> in_(xf))
        let by = mc |> out_words(m * ndim, -1000.0)
        let bk = mc |> in_plain([uint(kdim)])
        let bn = mc |> in_plain([uint(ndim)])
        // a tall m off the 128 floor mirrors the encoder's pair: the 128 stamp covers r0 rows and
        // the 32-row TH covers the rest at its x/y byte offsets - the nonzero-offset arm's binding
        let r0 = tall ? m / 128 * 128 : m
        let kr = kq_mm_tensor_kernel(f, true)
        var rem_pso = (tall && r0 != m) ? (mc |> keep_stamp(kr, err)) : null
        if (tall && r0 != m) {
            t |> success(rem_pso != null, "{tag}: remainder pipeline: {err}")
        }
        let doff = kq_split_doff(fmt, nsb)
        let ran = mc |> cell_record(t, tag) $(enc : MetalComputeEncoder?) {
            if (!tensor) {
                stamp_run(sb, pso) {
                    invoke(sb.enc, enc, mc.buf[bks], doff, mc.buf[bks], 0ul, mc.buf[bkq], 0ul, mc.buf[bx], mc.buf[by], 0ul, mc.buf[bk], mc.buf[bn], int64(m), int64(ndim))
                }
                return
            }
            invoke(kt.enc, enc, pso, mc.buf[bks], doff, mc.buf[bks], 0ul, mc.buf[bkq], 0ul, mc.buf[bx], 0ul, mc.buf[by], 0ul, mc.buf[bk], mc.buf[bn], int64(r0), int64(ndim))
            if (rem_pso != null) {
                invoke(kr.enc, enc, rem_pso, mc.buf[bks], doff, mc.buf[bks], 0ul, mc.buf[bkq], 0ul, mc.buf[bx], uint64(r0 * kdim * 2), mc.buf[by], uint64(r0 * ndim * 4), mc.buf[bk], mc.buf[bn], int64(m - r0), int64(ndim))
            }
        }
        return if (!ran)
        check_buf(t, tag, mc.buf[by], ref.want, env_bar(2e-4, ref.env), Shape(row_width = ndim), ref.env)
    }
}

// the Metal-4 tensor twin of the (already direct-covered) MetalQ8MulMm — same bindings; the
// q8u form dequant-stages W into @workgroup, so the gate binds its tgmem like production
def private q8_mulmm_t_gate(t : T?; var dev, queue; m, kdim, ndim : int; halfx : bool = false; mt : int = 32) {
    let hx = halfx ? " halfx" : ""
    let tag = "q8_mulmm_t{hx} mt{mt} m={m} k={kdim} n={ndim}"
    with_metal_cell(dev, queue) $(var mc : MetalCell) {
        var err : string
        let kt = (mt == 128 ? (halfx ? kernel_of(@@enc_gemm_mm_th128_c) : kernel_of(@@enc_gemm_mm_t128_c))
            : (halfx ? kernel_of(@@enc_gemm_mm_th_c) : kernel_of(@@enc_gemm_mm_t_c)))
        var pso = mc |> keep_stamp(kt, err)
        return if (!tensor_pso_or_skip(t, tag, pso, dev, err))
        let nb = kdim / 32
        var inscope blob : array<uint8>
        var inscope wdeq : array<float>
        q8_blob_fill(ndim, nb, blob, wdeq)
        var inscope xf : array<float>
        q8_x_fill(m * kdim, xf)
        var inscope xh : array<float16>
        if (halfx) {
            xh |> resize(m * kdim)
            for (i in range(m * kdim)) {
                xh[i] = float16(xf[i])   // the fixture is f16-exact, so the oracle dot is unchanged
            }
        }
        var inscope ref <- dense_ref_rows(wdeq, xf, m, kdim, ndim)
        let bw = mc |> in_plain(blob)
        let bx = halfx ? (mc |> in_plain(xh)) : (mc |> in_(xf))
        let by = mc |> out_words(m * ndim, -1000.0)
        let bk = mc |> in_plain([uint(kdim)])
        let bn = mc |> in_plain([uint(ndim)])
        let ran = mc |> cell_record(t, tag) $(enc : MetalComputeEncoder?) {
            invoke(kt.enc, enc, pso, mc.buf[bw], 0ul, mc.buf[bw], mc.buf[bx], 0ul, mc.buf[by], 0ul, mc.buf[bk], mc.buf[bn], int64(m), int64(ndim))
        }
        return if (!ran)
        check_buf(t, tag, mc.buf[by], ref.want, env_bar(2e-3, ref.env), Shape(row_width = ndim), ref.env)
    }
}

//! the poison-panel control's quiet count: the elements of plane `p` outside max(`rel` x env, 1e-3) of `want` - no eyeball
//! dump, since this red is the expectation, not a failure
def private poison_panel_over(mc; p : int; want, env : array<float>; rel : float) : int {
    var inscope got <- mc |> plane_run(p)
    var badp = 0
    for (g, w, e in got, want, env) {
        badp += abs(g - w) <= max(rel * e, 1e-3) ? 0 : 1
    }
    return badp
}

// the dev-W pair: the q8 blob dequantized to a device f16 panel (one thread per 34B block),
// then the all-device half x half mul_mm reading it - the production >= 256-row prefill
// form. Both dispatches chain in ONE encoder (hazard tracking orders the panel write/read).
def private q8_devw_gate(t : T?; var dev, queue; m, kdim, ndim : int; mt : int = 32) {
    let tag = "q8_devw mt{mt} m={m} k={kdim} n={ndim}"
    with_metal_cell(dev, queue) $(var mc : MetalCell) {
        var err : string
        let kd = kernel_of(@@enc_dequant_q8h)
        let km = mt == 128 ? kernel_of(@@enc_gemm_hmm_th128) : kernel_of(@@enc_gemm_hmm_th)
        var dq_pso = mc |> keep_stamp(kd, err)
        var mm_pso = mc |> keep_stamp(km, err)
        // the tensor GEMM alone may skip; the dequant is a base stamp and reds
        return if (!(mm_pso == null ? tensor_pso_or_skip(t, tag, mm_pso, dev, err) : pso_ok(t, tag, dq_pso, err)))
        let nb = kdim / 32
        let nblk = ndim * nb
        var inscope blob : array<uint8>
        var inscope wdeq : array<float>
        q8_blob_fill(ndim, nb, blob, wdeq)
        var inscope xf : array<float>
        q8_x_fill(m * kdim, xf)
        var inscope xh : array<float16>
        xh |> resize(m * kdim)
        for (i in range(m * kdim)) {
            xh[i] = float16(xf[i])   // the fixture is f16-exact, so the oracle dot is unchanged
        }
        var inscope ref <- dense_ref_rows(wdeq, xf, m, kdim, ndim)
        let bw = mc |> in_plain(blob)
        let bx = mc |> in_plain(xh)
        let bwh = mc |> out_words(ndim * kdim, float16(-100.0))   // poison fill: an unwritten panel block reds the compare
        let by = mc |> out_words(m * ndim, -1000.0)
        let bk = mc |> in_plain([uint(kdim)])
        let bn = mc |> in_plain([uint(ndim)])
        var dka = CvtArgs(total = uint(nblk))
        let ran = mc |> cell_record(t, tag) $(enc : MetalComputeEncoder?) {
            invoke(kd.enc, enc, dq_pso, mc.buf[bw], 0ul, mc.buf[bw], mc.buf[bwh], dka, int64(nblk))
            invoke(km.enc, enc, mm_pso, mc.buf[bwh], 0ul, mc.buf[bx], 0ul, mc.buf[by], 0ul, mc.buf[bk], mc.buf[bn], int64(m), int64(ndim))
        }
        if (ran) {
            check_buf(t, tag, mc.buf[by], ref.want, env_bar(2e-3, ref.env), Shape(row_width = ndim), ref.env)
        }
        // the N-TILED chain (the production over-knee form): two half-N tiles, each dequant at
        // the blob column offset + GEMM into y's column slice - same offset math as pf_devw_panel
        if (ndim % 128 == 0 && mt == 32) {
            let td = ndim / 2
            let nbr = kdim / 32
            let by_t = mc |> out_words(m * ndim, -500.0)
            let bwh_t = mc |> out_words(td * kdim, float16(-100.0))
            let bwh_t2 = mc |> out_words(td * kdim, float16(-100.0))
            var tka = CvtArgs(total = uint(td * nbr))
            let rant = mc |> cell_record(t, "{tag} tiled") $(enc : MetalComputeEncoder?) {
                for (ti in range(2)) {
                    let tbase = ti * td
                    let bwh_ti = ti == 0 ? bwh_t : bwh_t2
                    invoke(kd.enc, enc, dq_pso, mc.buf[bw], uint64(tbase * nbr * 34), mc.buf[bw], mc.buf[bwh_ti], tka, int64(td * nbr))
                    invoke(km.enc, enc, mm_pso, mc.buf[bwh_ti], 0ul, mc.buf[bx], 0ul, mc.buf[by_t], uint64(tbase * 4), mc.buf[bk], mc.buf[bn], int64(m), int64(td))
                }
            }
            if (rant) {
                check_buf(t, "{tag} tiled", mc.buf[by_t], ref.want, env_bar(2e-3, ref.env), Shape(row_width = ndim), ref.env)
            }
        }
        // negative control: the GEMM alone over a poison-only panel (dequant skipped) must MISS
        // the oracle - proves the chained compare above actually rode the dequant output
        let bwh_p = mc |> out_words(ndim * kdim, float16(-100.0))
        let by_p = mc |> out_words(m * ndim, -1000.0)
        let ranp = mc |> cell_record(t, "{tag} control") $(enc : MetalComputeEncoder?) {
            invoke(km.enc, enc, mm_pso, mc.buf[bwh_p], 0ul, mc.buf[bx], 0ul, mc.buf[by_p], 0ul, mc.buf[bk], mc.buf[bn], int64(m), int64(ndim))
        }
        if (ranp) {
            let badp = poison_panel_over(mc, by_p, ref.want, ref.env, 2e-3)
            t |> success(badp > 0, "{tag}: poison-panel control reds the bar ({badp} over)")
        }
    }
}

//! the dev-W dequant kernel of a format the CvtArgs pass serves (k4, k5, k6)
def private kq_dqh_kernel(f) {   //! generic, as the gates are: the Apple half alone instantiates it
    if (f == KqFmt.k4) return kernel_of(@@enc_dequant_k4h)
    if (f == KqFmt.k5) return kernel_of(@@enc_dequant_k5h)
    if (f != KqFmt.k6) {
        panic("kq_dqh_kernel: no CvtArgs dequant kernel for {f}")
    }
    return kernel_of(@@enc_dequant_k6h)
}

//! the dev-W dequant kernel of a split-scale format
def private kq_dq_kernel(f) {   //! generic, as the gates are: the Apple half alone instantiates it
    if (f == KqFmt.iq4xs) return kernel_of(@@enc_kq_dq_iq4xs)
    if (f == KqFmt.iq4nl) return kernel_of(@@enc_kq_dq_iq4nl)
    if (f == KqFmt.q40) return kernel_of(@@enc_kq_dq_q40)
    if (f == KqFmt.k3) return kernel_of(@@enc_kq_dq_k3)
    if (f == KqFmt.iq3s) return kernel_of(@@enc_kq_dq_iq3s)
    if (f == KqFmt.iq3xxs) return kernel_of(@@enc_kq_dq_iq3xxs)
    if (f == KqFmt.k2) return kernel_of(@@enc_kq_dq_k2)
    if (f == KqFmt.iq2s) return kernel_of(@@enc_kq_dq_iq2s)
    if (f == KqFmt.iq2xs) return kernel_of(@@enc_kq_dq_iq2xs)
    if (f != KqFmt.iq2xxs) {
        panic("kq_dq_kernel: no dev-W dequant kernel for {f}")
    }
    return kernel_of(@@enc_kq_dq_iq2xxs)
}

//! a format's dev-W dequant pipeline: the split-scale kernel, or the CvtArgs pass on k4 / k5 / k6
def private kq_dq_pso(var mc; fmt : int; var err : string&) : MetalComputePipeline? {
    let f = kq_fmt_of_id(fmt, "kq_dq_pso")
    return kq_dq_split(fmt) ? (mc |> keep_stamp(kq_dq_kernel(f), err)) : (mc |> keep_stamp(kq_dqh_kernel(f), err))
}

def private kq_dq_split(fmt : int) : bool => fmt != 4 && fmt != 5 && fmt != 6

//! one dequant dispatch of W rows [tbase, tbase+td) into bwh through the format's builder - the plane offsets stride per
//! superblock as pf_enc_kq_dq's do (16B strips, the d tail past the strips, kq_qsb quant bytes)
def private kq_dq_dispatch(enc; fmt : int; dq_pso;
                           bks, bkq, bwh, bk, bn; nsb, nb, tbase, td, kdim : int) {
    let f = kq_fmt_of_id(fmt, "kq_dq_dispatch")
    let compact = fmt == 4 || fmt == 5 || fmt == 45 || fmt == 40
    let so = uint64(tbase * nb * 16)
    let d0 = compact ? so : uint64(nsb * 16 + tbase * nb * (fmt == 2 ? 4 : 2))
    let qo = uint64(tbase) * uint64(nb) * uint64(kq_qsb(fmt))
    if (kq_dq_split(fmt)) {
        invoke(kq_dq_kernel(f).enc, enc, dq_pso, bks, d0, bks, so, bkq, qo, bwh, 0ul, bwh, 0ul, bk, bn, bwh, int64(kdim), int64(td))
    } else {
        var ka = CvtArgs(total = uint(td * (kdim / 32)))
        invoke(kq_dqh_kernel(f).enc, enc, dq_pso, bks, d0, bks, so, bkq, qo, bwh, ka, int64(td * (kdim / 32)))
    }
}

// the kq dev-W chains: the format's dequant into a device f16 panel, then the SAME all-device
// GEMM as q8's - per-format truth via kq_row_ref, plus the shared poison-panel negative
// control (dequant skipped must miss the oracle)
def private kq_devw_gate(t : T?; var dev, queue; fmt : int; m, kdim, ndim : int) {
    let tag = "kq_devw_k{fmt} m={m} k={kdim} n={ndim}"
    with_metal_cell(dev, queue) $(var mc : MetalCell) {
        var err : string
        var dq_pso = kq_dq_pso(mc, fmt, err)
        let km = kernel_of(@@enc_gemm_hmm_th)
        var mm_pso = mc |> keep_stamp(km, err)
        return if (!(mm_pso == null ? tensor_pso_or_skip(t, tag, mm_pso, dev, err) : pso_ok(t, tag, dq_pso, err)))
        let nb = kdim / 256
        let nsb = ndim * nb
        var inscope kq, ks : array<uint8>
        kq_fill_planes(fmt, nsb, kq, ks)
        var inscope xf : array<float>
        xf |> resize(m * kdim)
        for (i in range(m * kdim)) {
            xf[i] = float(float16(0.07 * float(i % 39) - 1.3))   // f16 round-trip: the oracle dot stays exact
        }
        var inscope xh : array<float16>
        xh |> resize(m * kdim)
        for (i in range(m * kdim)) {
            xh[i] = float16(xf[i])
        }
        var inscope ref <- dense_ref_env(m, kdim, ndim, xf) $(j; var wrow) {
            kq_row_ref(fmt, j * nb, nb, nsb, kq, ks, wrow)
        }
        let bks = mc |> in_plain(ks)
        let bkq = mc |> in_plain(kq)
        let bx = mc |> in_plain(xh)
        let bwh = mc |> out_words(ndim * kdim, float16(-100.0))   // poison fill: an unwritten panel block reds the compare
        let by = mc |> out_words(m * ndim, -1000.0)
        let bk = mc |> in_plain([uint(kdim)])
        let bn = mc |> in_plain([uint(ndim)])
        let ran = mc |> cell_record(t, tag) $(enc : MetalComputeEncoder?) {
            kq_dq_dispatch(enc, fmt, dq_pso, mc.buf[bks], mc.buf[bkq], mc.buf[bwh], mc.buf[bk], mc.buf[bn], nsb, nb, 0, ndim, kdim)
            invoke(km.enc, enc, mm_pso, mc.buf[bwh], 0ul, mc.buf[bx], 0ul, mc.buf[by], 0ul, mc.buf[bk], mc.buf[bn], int64(m), int64(ndim))
        }
        if (ran) {
            check_buf(t, tag, mc.buf[by], ref.want, env_bar(2e-4, ref.env), Shape(row_width = ndim), ref.env)
        }
        // the N-TILED chain: per-format tile plane offsets - same math as pf_devw_panel_kq
        if (ndim % 128 == 0) {
            let td = ndim / 2
            let by_t = mc |> out_words(m * ndim, -500.0)
            let bwh_t = mc |> out_words(td * kdim, float16(-100.0))
            let bwh_t2 = mc |> out_words(td * kdim, float16(-100.0))
            let rant = mc |> cell_record(t, "{tag} tiled") $(enc : MetalComputeEncoder?) {
                for (ti in range(2)) {
                    let tbase = ti * td
                    let bwh_ti = ti == 0 ? bwh_t : bwh_t2
                    kq_dq_dispatch(enc, fmt, dq_pso, mc.buf[bks], mc.buf[bkq], mc.buf[bwh_ti], mc.buf[bk], mc.buf[bn], nsb, nb, tbase, td, kdim)
                    invoke(km.enc, enc, mm_pso, mc.buf[bwh_ti], 0ul, mc.buf[bx], 0ul, mc.buf[by_t], uint64(tbase * 4), mc.buf[bk], mc.buf[bn], int64(m), int64(td))
                }
            }
            if (rant) {
                check_buf(t, "{tag} tiled", mc.buf[by_t], ref.want, env_bar(2e-4, ref.env), Shape(row_width = ndim), ref.env)
            }
        }
        // negative control: the GEMM alone over a poison-only panel must MISS the oracle
        let bwh_p = mc |> out_words(ndim * kdim, float16(-100.0))
        let by_p = mc |> out_words(m * ndim, -1000.0)
        let ranp = mc |> cell_record(t, "{tag} control") $(enc : MetalComputeEncoder?) {
            invoke(km.enc, enc, mm_pso, mc.buf[bwh_p], 0ul, mc.buf[bx], 0ul, mc.buf[by_p], 0ul, mc.buf[bk], mc.buf[bn], int64(m), int64(ndim))
        }
        if (ranp) {
            let badp = poison_panel_over(mc, by_p, ref.want, ref.env, 2e-4)
            t |> success(badp > 0, "{tag}: poison-panel control reds the bar ({badp} over)")
        }
    }
}

//! the double-buffered arm's output against the single-buffered one's, word for word
def private dbuf_identity(t : T?; mc; tag : string; by_a, by_b, n : int) {
    var inscope single <- mc |> plane_as(by_a, type<uint>, n)
    var inscope dbuf <- mc |> plane_as(by_b, type<uint>, n)
    check_exact(t, "{tag} dbuf == single", dbuf, single)
}

def private q8_dbuf_gate(t : T?; var dev, queue; m, kdim, ndim : int; mt : int = 128) {
    let tag = "q8_dbuf mt{mt} m={m} k={kdim} n={ndim}"
    with_metal_cell(dev, queue) $(var mc : MetalCell) {
        var err : string
        let ka = mt == 128 ? kernel_of(@@enc_gemm_mm_th128_c) : kernel_of(@@enc_gemm_mm_th_c)
        let kb = mt == 128 ? kernel_of(@@enc_gemm_mm_thdb128_c) : kernel_of(@@enc_gemm_mm_thdb_c)
        var pso_a = mc |> keep_stamp(ka, err)
        var pso_b = mc |> keep_stamp(kb, err)
        return if ((pso_a == null || pso_b == null) && !tensor_pso_or_skip(t, tag, null, dev, err))
        let nb = kdim / 32
        var inscope blob : array<uint8>
        var inscope wdeq : array<float>
        q8_blob_fill(ndim, nb, blob, wdeq)
        var inscope xf : array<float>
        q8_x_fill(m * kdim, xf)
        var inscope xh : array<float16>
        xh |> resize(m * kdim)
        for (i in range(m * kdim)) {
            xh[i] = float16(xf[i])
        }
        let bw = mc |> in_plain(blob)
        let bx = mc |> in_plain(xh)
        let by_a = mc |> out_words(m * ndim, -1000.0)
        let by_b = mc |> out_words(m * ndim, -500.0)
        let bk = mc |> in_plain([uint(kdim)])
        let bn = mc |> in_plain([uint(ndim)])
        let ran = mc |> cell_record(t, tag) $(enc : MetalComputeEncoder?) {
            invoke(ka.enc, enc, pso_a, mc.buf[bw], 0ul, mc.buf[bw], mc.buf[bx], 0ul, mc.buf[by_a], 0ul, mc.buf[bk], mc.buf[bn], int64(m), int64(ndim))
            invoke(kb.enc, enc, pso_b, mc.buf[bw], 0ul, mc.buf[bw], mc.buf[bx], 0ul, mc.buf[by_b], 0ul, mc.buf[bk], mc.buf[bn], int64(m), int64(ndim))
        }
        return if (!ran)
        dbuf_identity(t, mc, tag, by_a, by_b, m * ndim)
        var inscope ref <- dense_ref_rows(wdeq, xf, m, kdim, ndim)
        check_buf(t, tag, mc.buf[by_a], ref.want, env_bar(2e-3, ref.env), Shape(row_width = ndim), ref.env)
    }
}

def private kq_dbuf_gate(t : T?; var dev, queue; fmt : int; m, kdim, ndim : int) {
    let tag = "kq_dbuf_k{fmt} m={m} k={kdim} n={ndim}"
    with_metal_cell(dev, queue) $(var mc : MetalCell) {
        var err : string
        let ka = kq_mm_th128_kernel(kq_fmt_of_id(fmt, "kq_dbuf_gate"))
        let kb = fmt == 4 ? kernel_of(@@enc_kq_mm_k4_thdb128_c) : (fmt == 5 ? kernel_of(@@enc_kq_mm_k5_thdb128_c) : kernel_of(@@enc_kq_mm_k6_thdb128_c))
        var pso_a = mc |> keep_stamp(ka, err)
        var pso_b = mc |> keep_stamp(kb, err)
        return if ((pso_a == null || pso_b == null) && !tensor_pso_or_skip(t, tag, null, dev, err))
        let nb = kdim / 256
        let nsb = ndim * nb
        var inscope kq, ks : array<uint8>
        kq_fill_planes(fmt, nsb, kq, ks)
        var inscope xf : array<float>
        xf |> resize(m * kdim)
        for (i in range(m * kdim)) {
            xf[i] = float(float16(0.05 * float(i % 41) - 0.9))   // f16 round-trip: the oracle dot stays exact
        }
        var inscope xh : array<float16>
        xh |> resize(m * kdim)
        for (i in range(m * kdim)) {
            xh[i] = float16(xf[i])
        }
        // the single-buffered arm carries the CPU oracle: bit-identity alone would pass with both
        // arms wrong, so one of the two is scored against the dequantized rows
        var inscope ref <- dense_ref_env(m, kdim, ndim, xf) $(j; var wrow) {
            kq_row_ref(fmt, j * nb, nb, nsb, kq, ks, wrow)
        }
        let bks = mc |> in_plain(ks)
        let bkq = mc |> in_plain(kq)
        let bx = mc |> in_plain(xh)
        let by_a = mc |> out_words(m * ndim, -1000.0)
        let by_b = mc |> out_words(m * ndim, -500.0)
        let bk = mc |> in_plain([uint(kdim)])
        let bn = mc |> in_plain([uint(ndim)])
        let doff = fmt == 6 ? uint64(nsb * 16) : 0ul   //! k6 keeps its f16 d scales in a tail past the 16B blocks
        let ran = mc |> cell_record(t, tag) $(enc : MetalComputeEncoder?) {
            invoke(ka.enc, enc, pso_a, mc.buf[bks], doff, mc.buf[bks], 0ul, mc.buf[bkq], 0ul, mc.buf[bx], 0ul, mc.buf[by_a], 0ul, mc.buf[bk], mc.buf[bn], int64(m), int64(ndim))
            invoke(kb.enc, enc, pso_b, mc.buf[bks], doff, mc.buf[bks], 0ul, mc.buf[bkq], 0ul, mc.buf[bx], 0ul, mc.buf[by_b], 0ul, mc.buf[bk], mc.buf[bn], int64(m), int64(ndim))
        }
        return if (!ran)
        dbuf_identity(t, mc, tag, by_a, by_b, m * ndim)
        check_buf(t, tag, mc.buf[by_a], ref.want, env_bar(2e-4, ref.env), Shape(row_width = ndim), ref.env)
    }
}

[arch(at="../ARCHITECTURE_IMAGE.md#image-devw-plane")]
def private devw_bake_mirror_gate(t : T?; var dev, queue; fmt : int; ndim, kdim : int) {
    let tag = "devw_bake_mirror fmt{fmt} n={ndim} k={kdim}"
    with_metal_cell(dev, queue) $(var mc : MetalCell) {
        var err : string
        let kd = kernel_of(@@enc_dequant_q8h)
        var dq_pso = fmt == 0 ? (mc |> keep_stamp(kd, err)) : kq_dq_pso(mc, fmt, err)
        // the split-scale dequants are tensor-toolchain stamps; q8 and the k4/k5/k6 forms are base stamps and red
        return if (!(fmt != 0 && fmt != 4 && fmt != 5 && fmt != 6 ? tensor_pso_or_skip(t, tag, dq_pso, dev, err) : pso_ok(t, tag, dq_pso, err)))
        let nblk = ndim * (kdim / 32)
        var inscope mir : array<float16>
        mir |> resize(ndim * kdim)
        let bwh = mc |> out_words(ndim * kdim, float16(-100.0))
        var dka = CvtArgs(total = uint(nblk))
        var ran = false
        if (fmt == 0) {
            var inscope blob : array<uint8>
            var inscope wdeq : array<float>
            q8_blob_fill(ndim, kdim / 32, blob, wdeq)
            //! the mirror eats the transcode pair (int8 quants + f32 scales) - the blob's f16 scales
            //! widen to f32 and re-narrow exactly, so the pair IS the blob's content
            var inscope q : array<int8>
            var inscope sf : array<float>
            q |> resize(nblk * 32)
            sf |> resize(nblk)
            unsafe {
                for (b in range(nblk)) {
                    sf[b] = float(addr<float16 const?>(blob[b * 34])[0])
                    for (c in range(32)) {
                        q[b * 32 + c] = int8(blob[b * 34 + 2 + c])
                    }
                }
                devw_dequant_q8_core(addr(q[0]), addr(sf[0]), int64(nblk), addr(mir[0]))
            }
            let bw = mc |> in_plain(blob)
            ran = mc |> cell_record(t, tag) $(enc : MetalComputeEncoder?) {
                invoke(kd.enc, enc, dq_pso, mc.buf[bw], 0ul, mc.buf[bw], mc.buf[bwh], dka, int64(nblk))
            }
        } else {
            let nsb = ndim * (kdim / 256)
            var inscope kq, ks : array<uint8>
            kq_fill_planes(fmt, nsb, kq, ks)
            unsafe {
                if (fmt == 6) {
                    devw_dequant_k6_core(addr(kq[0]), addr(ks[0]), 16l, addr(ks[nsb * 16]), 2l, int64(nsb), addr(mir[0]))
                } elif (fmt == 4 || fmt == 5) {
                    devw_dequant_k45_core(addr(kq[0]), addr(ks[0]), 16l, int64(nsb), fmt == 5, addr(mir[0]))
                } else {
                    var kf : KqFmt
                    t |> success(kq_fmt_of_stream_code(fmt == 2 ? K2_STREAM_TAG : fmt, kf), "{tag}: format id {fmt} names a KqFmt")
                    t |> success(devw_dequant_kq_mirror(kf, kq, ks, int64(ndim), int64(kdim), mir), "{tag}: the mirror serves the shape")
                }
            }
            let bks = mc |> in_plain(ks)
            let bkq = mc |> in_plain(kq)
            let bk = mc |> in_plain([uint(kdim)])
            let bn = mc |> in_plain([uint(ndim)])
            ran = mc |> cell_record(t, tag) $(enc : MetalComputeEncoder?) {
                kq_dq_dispatch(enc, fmt, dq_pso, mc.buf[bks], mc.buf[bkq], mc.buf[bwh], mc.buf[bk], mc.buf[bn], nsb, kdim / 256, 0, ndim, kdim)
            }
        }
        return if (!ran)
        // the halfword compare stays bitwise (a float16 == would read two NaN payloads as different and -0 as +0); its
        // control flips the FIRST halfword's low bit
        var bad = 0
        var first = -1
        unsafe {
            let p = reinterpret<uint16 const?>(metal_buffer_contents(mc.buf[bwh]))
            let mp = addr<uint16 const?>(mir[0])
            for (i in range(ndim * kdim)) {
                if (p[i] != mp[i]) {
                    bad++
                    if (first < 0) {
                        first = i
                    }
                    if (bad <= 4) {
                        to_log(LOG_ERROR, "{tag}: [{i}] gpu={uint(p[i])} cpu={uint(mp[i])}\n")
                    }
                }
            }
        }
        if (bad != 0) {
            to_log(LOG_ERROR, "{tag}: {bad} halfwords differ, first at {first}\n")
        }
        t |> equal(bad, 0)
        var bad_p = 0
        unsafe {
            let p = reinterpret<uint16 const?>(metal_buffer_contents(mc.buf[bwh]))
            var mp = addr<uint16?>(mir[0])
            mp[0] = uint16(uint(mp[0]) ^ 1u)
            for (i in range(ndim * kdim)) {
                if (p[i] != mp[i]) {
                    bad_p++
                }
            }
        }
        t |> success(bad_p != 0, "{tag}: flipped-halfword control must miss the GPU panel")
    }
}

// the native-bf16 A-stage pair: W is bf16 halfwords (top 16 bits of f32), row-major [ndim x kdim]
// The dense f32-weight tower stamp. exact=true builds pow2/small-int fixtures every staged
// value survives f32->f16 exactly (the compare then measures tiling/indexing alone, 2e-3);
// exact=false lets the f16 stage round on BOTH operands (double-RNE model ~9.8e-4*env, bar
// 4e-3). m < mp exercises the pad rows: X pad rows are zero, so their outputs pin at 0.
def private f32_mulmm_gate(t : T?; var dev, queue; exact : bool; m, mp, kdim, ndim : int) {
    let tag = "f32_mulmm{exact ? " exact" : ""} m={m}/{mp} k={kdim} n={ndim}"
    with_metal_cell(dev, queue) $(var mc : MetalCell) {
        var err : string
        let st = stamp_of(@@enc_f32_mm)
        var pso = mc |> keep_stamp(st, err)
        return if (!pso_ok(t, tag, pso, err))
        var inscope w : array<float>
        w |> reserve(ndim * kdim)   // the gemma4uv shapes pass the unreserved-resize guard
        w |> resize(ndim * kdim)
        var inscope xf : array<float>
        xf |> reserve(mp * kdim)
        xf |> resize(mp * kdim)
        if (exact) {
            let lut = fixed_array(0.25, 0.5, 1.0, 2.0, -0.25, -0.5, -1.0, -2.0)
            for (i in range(ndim * kdim)) {
                w[i] = lut[(i * 5 + 1) % 8] * float((i * 3) % 5)
            }
            for (i in range(m * kdim)) {
                xf[i] = lut[(i * 7 + 3) % 8]
            }
        } else {
            for (i in range(ndim * kdim)) {
                w[i] = 0.13 * float((i * 3) % 13) - 0.8
            }
            for (i in range(m * kdim)) {
                xf[i] = 0.11 * float(i % 37) - 1.9
            }
        }
        var inscope ref <- strided_ref(dense_ref_rows(w, xf, m, kdim, ndim), ndim, ndim, mp, 0.0)   //! the pad rows' X is zero: their outputs pin at 0
        let bw = mc |> in_(w)
        let bx = mc |> in_(xf)
        let by = mc |> out_words(mp * ndim, -1000.0)
        let bk = mc |> in_plain([uint(kdim)])
        let bn = mc |> in_plain([uint(ndim)])
        let ran = mc |> cell_record(t, tag) $(enc : MetalComputeEncoder?) {
            stamp_run(st, pso) {
                invoke(st.enc, enc, mc.buf[bw], 0ul, mc.buf[bx], mc.buf[by], 0ul, mc.buf[bk], mc.buf[bn], int64(mp), int64(ndim))
            }
        }
        return if (!ran)
        check_buf(t, tag, mc.buf[by], ref.want, env_bar(exact ? 2e-3 : 4e-3, ref.env), Shape(row_width = ndim), ref.env)
    }
}

def private bf16_mulmm_gate(t : T?; var dev, queue; tensor : bool; m, kdim, ndim : int; halfx : bool = false; mt : int = 32) {
    let tag = "bf16_mulmm{tensor ? "_t" : ""}{halfx ? " halfx" : ""} mt{mt} m={m} k={kdim} n={ndim}"
    with_metal_cell(dev, queue) $(var mc : MetalCell) {
        var err : string
        let tk = mt == 128 || tensor
        let kt = (mt == 128 ? (halfx ? kernel_of(@@pf_enc_bf16_mm_th128_c) : kernel_of(@@pf_enc_bf16_mm_t128_c))
            : (halfx ? kernel_of(@@pf_enc_bf16_mm_th_c) : kernel_of(@@pf_enc_bf16_mm_t_c)))
        let kb = kernel_of(@@pf_enc_bf16_mm_c)
        var pso = tk ? (mc |> keep_stamp(kt, err)) : (mc |> keep_stamp(kb, err))
        // the simdgroup BASE asserts — production compiles it unconditionally, and this arm is its
        // only coverage; a broken bf16 A-stage must red here, not skip
        return if (!(tensor ? tensor_pso_or_skip(t, tag, pso, dev, err) : pso_ok(t, tag, pso, err)))
        var inscope wb : array<uint16>
        wb |> resize(ndim * kdim)
        var inscope wdeq : array<float>
        wdeq |> resize(ndim * kdim)
        for (i in range(ndim * kdim)) {
            wb[i] = uint16(float_bits_to_uint(0.125 * float((i * 3) % 13 - 6)) >> 16u)
            wdeq[i] = uint_bits_to_float(uint(wb[i]) << 16u)
        }
        var inscope xf : array<float>
        xf |> resize(m * kdim)
        for (i in range(m * kdim)) {
            // halfx: round-trip through f16 so the oracle dot below stays the arm's exact reference
            xf[i] = halfx ? float(float16(0.11 * float(i % 37) - 1.9)) : 0.11 * float(i % 37) - 1.9
        }
        var inscope xh : array<float16>
        if (halfx) {
            xh |> resize(m * kdim)
            for (i in range(m * kdim)) {
                xh[i] = float16(xf[i])
            }
        }
        var inscope ref <- dense_ref_rows(wdeq, xf, m, kdim, ndim)
        let bw = mc |> in_plain(wb)
        let bx = halfx ? (mc |> in_plain(xh)) : (mc |> in_(xf))
        let by = mc |> out_words(m * ndim, -1000.0)
        let bk = mc |> in_plain([uint(kdim)])
        let bn = mc |> in_plain([uint(ndim)])
        let ran = mc |> cell_record(t, tag) $(enc : MetalComputeEncoder?) {
            if (tk) {
                invoke(kt.enc, enc, pso, mc.buf[bw], 0ul, mc.buf[bx], 0ul, mc.buf[by], 0ul, mc.buf[bk], mc.buf[bn], int64(m), int64(ndim))
            } else {
                invoke(kb.enc, enc, pso, mc.buf[bw], 0ul, mc.buf[bx], mc.buf[by], mc.buf[bk], mc.buf[bn], int64(m), int64(ndim))
            }
        }
        return if (!ran)
        check_buf(t, tag, mc.buf[by], ref.want, env_bar(2e-3, ref.env), Shape(row_width = ndim), ref.env)
    }
}

// The M-pad-32 batched-decode GEMM (MetalQ8GemmB) and its Metal-4 tensor twin: X [mp32 x kdim]
// f32 token rows, dual-view q8 blob W, Y stored at TOKEN-row stride ys — the fused-QKV segment
// stride at QKV sites, so ys > ndim must leave the gap columns untouched (the sentinel
// whole-buffer compare asserts exactly that).
def private q8_gemm_b_gate(t : T?; var dev, queue; tensor : bool; mp32, kdim, ndim, ys : int) {
    let tag = "q8_gemm_b{tensor ? "t" : ""} m={mp32} k={kdim} n={ndim} ys={ys}"
    with_metal_cell(dev, queue) $(var mc : MetalCell) {
        var err : string
        let st = tensor ? stamp_of(@@enc_gemm_b_t_c) : stamp_of(@@enc_gemm_b_c)
        var pso = mc |> keep_stamp(st, err)
        return if (!(tensor ? tensor_pso_or_skip(t, tag, pso, dev, err) : pso_ok(t, tag, pso, err)))
        var inscope blob : array<uint8>
        var inscope wdeq : array<float>
        q8_blob_fill(ndim, kdim / 32, blob, wdeq)
        var inscope xf : array<float>
        q8_x_fill(mp32 * kdim, xf)
        var inscope ref <- strided_ref(dense_ref_rows(wdeq, xf, mp32, kdim, ndim), ndim, ys, mp32, -1000.0)
        var ka = gemm_ka(int64(kdim), int64(ndim), int64(ys))
        let bw = mc |> in_plain(blob)
        let bx = mc |> in_(xf)
        let by = mc |> out_words(mp32 * ys, -1000.0)
        let ran = mc |> cell_record(t, tag) $(enc : MetalComputeEncoder?) {
            stamp_run(st, pso) {   // one blob, its byte and half-scale views
                invoke(st.enc, enc, mc.buf[bw], 0ul, mc.buf[bw], mc.buf[bx], mc.buf[by], 0ul, ka, int64((mp32 / 32) * (ndim / 32)))
            }
        }
        return if (!ran)
        check_buf(t, tag, mc.buf[by], ref.want, env_bar(2e-3, ref.env), Shape(row_width = ys), ref.env)
    }
}

// The split-K pair (MetalQ8GemmBSk[T] + MetalSkReduce) run as the production two-dispatch chain
// in one serial encoder: grid.y is the k-slice axis, each slice's partial-C plane is checked
// against a per-slice CPU dot, then the reduce's summed y against the full dot. mp == 32 is the
// family's policy gate.
def private q8_gemm_sk_gate(t : T?; var dev, queue; tensor : bool; kdim, ndim, ksplit : int) {
    let tag = "q8_gemm_sk{tensor ? "t" : ""} k={kdim} n={ndim} sk={ksplit}"
    with_metal_cell(dev, queue) $(var mc : MetalCell) {
        var err : string
        let st = tensor ? stamp_of(@@enc_gemm_sk_b_t_c) : stamp_of(@@enc_gemm_sk_b_c)
        let sr = stamp_of(@@enc_sk_reduce_c)
        var pso = mc |> keep_stamp(st, err)
        return if (!(tensor ? tensor_pso_or_skip(t, tag, pso, dev, err) : pso_ok(t, tag, pso, err)))
        var psoR = mc |> keep_stamp(sr, err)
        return if (!pso_ok(t, "{tag} reduce", psoR, err))
        let mp = 32
        let kslice = kdim / ksplit
        var inscope blob : array<uint8>
        var inscope wdeq : array<float>
        q8_blob_fill(ndim, kdim / 32, blob, wdeq)
        var inscope xf : array<float>
        q8_x_fill(mp * kdim, xf)
        var inscope pwant, penv, ywant, yenv : array<float>
        pwant |> resize(ksplit * mp * ndim)
        penv |> resize(ksplit * mp * ndim)
        ywant |> resize(mp * ndim)
        yenv |> resize(mp * ndim)
        for (r in range(mp)) {
            for (j in range(ndim)) {
                var yacc = 0.0lf
                var yev = 0.0lf
                for (sl in range(ksplit)) {
                    var acc = 0.0lf
                    var ev = 0.0lf
                    for (c in range(sl * kslice, (sl + 1) * kslice)) {
                        let p = double(wdeq[j * kdim + c]) * double(xf[r * kdim + c])
                        acc += p
                        ev += abs(p)
                    }
                    pwant[sl * mp * ndim + r * ndim + j] = float(acc)
                    penv[sl * mp * ndim + r * ndim + j] = float(ev)
                    yacc += acc
                    yev += ev
                }
                ywant[r * ndim + j] = float(yacc)
                yenv[r * ndim + j] = float(yev)
            }
        }
        var ka = gemm_sk_ka(int64(kdim), int64(ndim), int64(ksplit))
        var kaR = SkReduceArgs(total = uint(mp * ndim), ksplit = uint(ksplit))
        let bw = mc |> in_plain(blob)
        let bx = mc |> in_(xf)
        let bpart = mc |> out_words(ksplit * mp * ndim, -1000.0)
        let by = mc |> out_words(mp * ndim, -1000.0)
        let ran = mc |> cell_record(t, tag) $(enc : MetalComputeEncoder?) {
            stamp_run(st, pso) {   // one blob, its byte and half-scale views
                invoke(st.enc, enc, mc.buf[bw], 0ul, mc.buf[bw], mc.buf[bx], mc.buf[bpart], 0ul, ka, int64(ndim), int64(ksplit))
            }
            stamp_run(sr, psoR) {
                invoke(sr.enc, enc, mc.buf[bpart], mc.buf[by], kaR, int64(mp * ndim))
            }
        }
        return if (!ran)
        check_buf(t, "{tag} part", mc.buf[bpart], pwant, env_bar(2e-3, penv), Shape(row_width = ndim), penv)
        check_buf(t, "{tag} y", mc.buf[by], ywant, env_bar(2e-3, yenv), Shape(row_width = ndim), yenv)
    }
}

// The 64-wide N tile (MetalQ8Gemm64B[T], the classifier site): same M-pad-32 contract but
// ndim % 64 and y at DENSE stride ka.ndim — ys rides along as 0 exactly as the production
// classifier call does, proving the kernel never reads it.
def private q8_gemm64_b_gate(t : T?; var dev, queue; tensor : bool; mp32, kdim, ndim : int) {
    let tag = "q8_gemm64_b{tensor ? "t" : ""} m={mp32} k={kdim} n={ndim}"
    with_metal_cell(dev, queue) $(var mc : MetalCell) {
        var err : string
        let sp = stamp_of(@@enc_gemm64_b_c)
        let stt = stamp_of(@@enc_gemm64_b_t_c)
        var pso = tensor ? (mc |> keep_stamp(stt, err)) : (mc |> keep_stamp(sp, err))
        return if (!(tensor ? tensor_pso_or_skip(t, tag, pso, dev, err) : pso_ok(t, tag, pso, err)))
        var inscope blob : array<uint8>
        var inscope wdeq : array<float>
        q8_blob_fill(ndim, kdim / 32, blob, wdeq)
        var inscope xf : array<float>
        q8_x_fill(mp32 * kdim, xf)
        var inscope ref <- dense_ref_rows(wdeq, xf, mp32, kdim, ndim)
        var ka = gemm_ka(int64(kdim), int64(ndim), 0l)
        let bw = mc |> in_plain(blob)
        let bx = mc |> in_(xf)
        let by = mc |> out_words(mp32 * ndim, -1000.0)
        let ntg = int64((mp32 / 32) * (ndim / 64))
        let ran = mc |> cell_record(t, tag) $(enc : MetalComputeEncoder?) {
            if (tensor) {   // one blob, its byte and half-scale views
                stamp_run(stt, pso) {
                    invoke(stt.enc, enc, mc.buf[bw], 0ul, mc.buf[bw], mc.buf[bx], mc.buf[by], 0ul, ka, ntg)
                }
            } else {
                stamp_run(sp, pso) {
                    invoke(sp.enc, enc, mc.buf[bw], 0ul, mc.buf[bw], mc.buf[bx], mc.buf[by], ka, ntg)
                }
            }
        }
        return if (!ran)
        check_buf(t, tag, mc.buf[by], ref.want, env_bar(2e-3, ref.env), Shape(row_width = ndim), ref.env)
    }
}

// ===== the MoE gathered mul_mm family (Wave C) =====

// Padded CSR route fixture: expert e's rows start at a 32-aligned base (the route prefix
// contract). gather=1 rows read X through bkt (flat slot / nk); padded tail rows clamp to the
// LAST live entry exactly as the kernels do, so srcrow carries the already-clamped answer.
struct private MoeFix {
    ne : int
    pad : int           // total padded bucket rows
    maxtiles : int      // grid.x — experts with fewer padded tiles exit in-kernel
    cnt : array<uint>
    basep : array<uint>
    bkt : array<uint>
    srcrow : array<int> // per padded bucket row: the X row its result reads
}

def private moe_fix(counts : array<int>; gather : bool; nk, nx : int) : MoeFix {
    var fx : MoeFix
    fx.ne = length(counts)
    var (cnt, basep, pad) <- padded_bases(counts)
    fx.cnt <- cnt
    fx.basep <- basep
    fx.pad = pad
    fx.maxtiles = 1
    for (c in counts) {
        fx.maxtiles = max(fx.maxtiles, (c + 31) / 32)
    }
    fx.srcrow |> resize(pad)
    if (gather) {
        fx.bkt |> resize(pad)
        for (e in range(fx.ne)) {
            let rb = int(fx.basep[e])
            let rows = (counts[e] + 31) / 32 * 32
            for (r in range(rows)) {
                if (r < counts[e]) {
                    fx.bkt[rb + r] = uint((r * 7 + e * 5 + 3) % (nx * nk))
                }
                fx.srcrow[rb + r] = int(fx.bkt[rb + min(r, counts[e] - 1)]) / nk
            }
        }
    } else {
        for (r in range(pad)) {
            fx.srcrow[r] = r
        }
    }
    return <- fx
}

// shared truth walk: want/env over the whole padded bucket panel from a flat [nrows_w x kdim]
// dequant plane (row of expert e, col j at wdeq[(e*ndim + j)*kdim]), plus an optional bias row
def private moe_mm_truth(fx : MoeFix; wdeq, xf, wb : array<float>; kdim, ndim : int; hasb : bool;
                         var want, env : array<float>) {
    want |> resize(fx.pad * ndim)
    env |> resize(fx.pad * ndim)
    for (e in range(fx.ne)) {
        let rows = (int(fx.cnt[e]) + 31) / 32 * 32
        for (r in range(rows)) {
            let row = int(fx.basep[e]) + r
            let src = fx.srcrow[row]
            for (j in range(ndim)) {
                var acc = 0.0lf
                var ev = 0.0lf
                for (c in range(kdim)) {
                    let p = double(wdeq[(e * ndim + j) * kdim + c]) * double(xf[src * kdim + c])
                    acc += p
                    ev += abs(p)
                }
                if (hasb) {
                    acc += double(wb[e * ndim + j])
                    ev += abs(double(wb[e * ndim + j]))
                }
                want[row * ndim + j] = float(acc)
                env[row * ndim + j] = float(ev)
            }
        }
    }
}

// The gathered K-quant mul_mm trio (MetalMoeMulMmK4/K5/K6): per-expert superblock planes off
// the shared kq fixture, truth through the shipped dequant_k*_plane_superblock. Binds at the
// MetalMoeMulMmBase family numbers (0..2 weight views, x 3, y 4, cnt/base/bkt 7..9); k6's
// binding 0 is the f16 d-tail view of the scale plane.
def private moe_mulmm_kq_gate(t : T?; var dev, queue; fmt : int; kdim, ndim : int; counts : array<int>; gather : bool) {
    let tag = "moe_mulmm_k{fmt}{gather ? " gather" : ""} k={kdim} n={ndim}"
    with_metal_cell(dev, queue) $(var mc : MetalCell) {
        var err : string
        let f = kq_fmt_of_id(fmt, "moe_mulmm_kq_gate")
        let k45 = fmt == 4 ? kernel_of(@@pf_enc_moe_mm_k4_c) : kernel_of(@@pf_enc_moe_mm_k5_c)   //! k4 / k5 bind both scale views at one offset; k6 its d tail apart
        let k6 = kernel_of(@@pf_enc_moe_mm_k6_c)
        var pso = fmt == 6 ? (mc |> keep_stamp(k6, err)) : (mc |> keep_stamp(k45, err))
        return if (!pso_ok(t, tag, pso, err))
        let ne = length(counts)
        let nb = kdim / 256
        let eplane = ndim * nb
        let nsb = ne * eplane
        var inscope kq, ks : array<uint8>
        kq_fill_planes(fmt, nsb, kq, ks)
        let nk = 2
        let nx = 24
        var inscope fx <- moe_fix(counts, gather, nk, nx)
        var inscope xf : array<float>
        xf |> resize((gather ? nx : fx.pad) * kdim)
        for (i in range(length(xf))) {
            xf[i] = float((i * 5) % 9 - 4)
        }
        var inscope wdeq : array<float>
        wdeq |> resize(ne * ndim * kdim)
        var inscope wrow : array<float>
        wrow |> resize(kdim)
        for (e in range(ne)) {
            for (j in range(ndim)) {
                kq_row_ref(fmt, e * eplane + j * nb, nb, nsb, kq, ks, wrow)
                for (c in range(kdim)) {
                    wdeq[(e * ndim + j) * kdim + c] = wrow[c]
                }
            }
        }
        var inscope wb, want, env : array<float>
        moe_mm_truth(fx, wdeq, xf, wb, kdim, ndim, false, want, env)
        var ka = MoeMmArgs(kdim = uint(kdim), ndim = uint(ndim), eplane = uint(eplane),
            nk = uint(nk), gather = gather ? 1u : 0u)
        let bks = mc |> in_plain(ks)
        let bkq = mc |> in_plain(kq)
        let bx = mc |> in_(xf)
        let by = mc |> out_words(fx.pad * ndim, -1000.0)
        let bcnt = mc |> in_plain(fx.cnt)
        let bbase = mc |> in_plain(fx.basep)
        let bbkt = gather ? (mc |> in_plain(fx.bkt)) : (mc |> in_plain([0u]))   // gather=0: unread 4B stand-in
        let doff = kq_split_doff(fmt, nsb)
        let ran = mc |> cell_record(t, tag) $(enc : MetalComputeEncoder?) {
            if (fmt == 6) {
                invoke(k6.enc, enc, pso, mc.buf[bks], doff, mc.buf[bks], 0ul, mc.buf[bkq], 0ul, mc.buf[bx], mc.buf[by], ka, mc.buf[bcnt], mc.buf[bbase], mc.buf[bbkt], int64(ndim), int64(fx.maxtiles * 32), int64(ne))
            } else {
                invoke(k45.enc, enc, pso, mc.buf[bks], doff, mc.buf[bks], mc.buf[bkq], 0ul, mc.buf[bx], mc.buf[by], ka, mc.buf[bcnt], mc.buf[bbase], mc.buf[bbkt], int64(ndim), int64(fx.maxtiles * 32), int64(ne))
            }
        }
        if (ran) {
            check_buf(t, tag, mc.buf[by], want, env_bar(2e-4, env), Shape(row_width = ndim), env)
        }
        // the contiguous tensor twin (TH stamp over an f16 X): same oracle, per-expert plane folds
        return if (gather)
        var terr : string
        let kw = moe_kq_twin(f, false)
        var tw_pso = mc |> keep_stamp(kw, terr)
        return if (!tensor_pso_or_skip(t, "{tag} twin", tw_pso, dev, terr))
        var inscope xh <- [for (v in xf); float16(v)]
        let bxh = mc |> in_plain(xh)
        let by_t = mc |> out_words(fx.pad * ndim, -500.0)
        let rant = mc |> cell_record(t, "{tag} twin") $(enc : MetalComputeEncoder?) {
            invoke(kw.enc, enc, tw_pso, mc.buf[bks], doff, mc.buf[bks], 0ul, mc.buf[bkq], 0ul, mc.buf[bxh], mc.buf[by_t], ka, mc.buf[bcnt], mc.buf[bbase], int64(ndim), int64(fx.maxtiles * 32), int64(ne))
        }
        if (rant) {
            check_buf(t, "{tag} twin", mc.buf[by_t], want, env_bar(2e-4, env), Shape(row_width = ndim), env)
        }
        // the tall pair: 128-floor stamp + remainder stamp over the same fixtures — the
        // production >=256-row moe dispatch shape (counts crossing 128 exercise both)
        let kl = moe_kq_twin(f, true)
        let kr = moe_kq_thr(f)
        var tl_pso = mc |> keep_stamp(kl, terr)
        var rm_pso = mc |> keep_stamp(kr, terr)
        // the toolchain is proven (tw_pso compiled), so a tall-pair PSO that fails to
        // compile is a RED, never a silent skip - this gate is the pair's only coverage
        t |> success(tl_pso != null && rm_pso != null, "{tag} tall pair: pipeline: {terr}")
        return if (tl_pso == null || rm_pso == null)
        var maxce = 0
        for (c in fx.cnt) {
            maxce = max(maxce, int(c))
        }
        let by_p = mc |> out_words(fx.pad * ndim, -700.0)
        let ranp2 = mc |> cell_record(t, "{tag} tall pair") $(enc : MetalComputeEncoder?) {
            invoke(kl.enc, enc, tl_pso, mc.buf[bks], doff, mc.buf[bks], 0ul, mc.buf[bkq], 0ul, mc.buf[bxh], mc.buf[by_p], ka, mc.buf[bcnt], mc.buf[bbase], int64(ndim), int64(maxce), int64(ne))
            invoke(kr.enc, enc, rm_pso, mc.buf[bks], doff, mc.buf[bks], 0ul, mc.buf[bkq], 0ul, mc.buf[bxh], mc.buf[by_p], ka, mc.buf[bcnt], mc.buf[bbase], int64(ndim), int64(ne))
        }
        return if (!ranp2)
        check_buf(t, "{tag} tall pair", mc.buf[by_p], want, env_bar(2e-4, env), Shape(row_width = ndim), env)
    }
}

//! a form of an expert-stamp family - th, th128 or thr - off the family's three kernels
def private moe_form(form : MoeSplitForm; th, th128, thr : auto(K)) : K {
    if (form == MoeSplitForm.th) return th
    if (form == MoeSplitForm.th128) return th128
    if (form != MoeSplitForm.thr) {
        panic("moe_form: no {form} form in the table")
    }
    return thr
}

//! the moe_mm family's expert tensor twin of k4, k5 or k6: the th stamp, or its 128-row `tall` form
def private moe_kq_twin(f; tall : bool) {   //! generic, as the gates are: the Apple half alone instantiates it
    if (f == KqFmt.k4) return tall ? kernel_of(@@pf_enc_moe_mm_k4_th128_c) : kernel_of(@@pf_enc_moe_mm_k4_th_c)
    if (f == KqFmt.k5) return tall ? kernel_of(@@pf_enc_moe_mm_k5_th128_c) : kernel_of(@@pf_enc_moe_mm_k5_th_c)
    if (f != KqFmt.k6) {
        panic("moe_kq_twin: no moe_mm expert twin for {f}")
    }
    return tall ? kernel_of(@@pf_enc_moe_mm_k6_th128_c) : kernel_of(@@pf_enc_moe_mm_k6_th_c)
}

//! the moe_mm family's remainder stamp of k4, k5 or k6 - its builder takes no row count, so it rides apart from the twins
def private moe_kq_thr(f) {   //! generic, as the gates are: the Apple half alone instantiates it
    if (f == KqFmt.k4) return kernel_of(@@pf_enc_moe_mm_k4_thr_c)
    if (f == KqFmt.k5) return kernel_of(@@pf_enc_moe_mm_k5_thr_c)
    if (f != KqFmt.k6) {
        panic("moe_kq_thr: no moe_mm remainder stamp for {f}")
    }
    return kernel_of(@@pf_enc_moe_mm_k6_thr_c)
}

//! the split family's expert tensor twins per format and form (th, th128, thr): k3 and the iquants ride their dense split
//! classes under the MoE shell
def private moe_split_twin(f; form : MoeSplitForm) {   //! generic, as the gates are: the Apple half alone instantiates it
    if (f == KqFmt.k3) return moe_form(form, kernel_of(@@pf_enc_moe_mm_k3_th_c), kernel_of(@@pf_enc_moe_mm_k3_th128_c), kernel_of(@@pf_enc_moe_mm_k3_thr_c))
    if (f == KqFmt.iq4xs) return moe_form(form, kernel_of(@@pf_enc_moe_mm_iq4xs_th_c), kernel_of(@@pf_enc_moe_mm_iq4xs_th128_c), kernel_of(@@pf_enc_moe_mm_iq4xs_thr_c))
    if (f == KqFmt.q40) return moe_form(form, kernel_of(@@pf_enc_moe_mm_q40_th_c), kernel_of(@@pf_enc_moe_mm_q40_th128_c), kernel_of(@@pf_enc_moe_mm_q40_thr_c))
    if (f == KqFmt.iq3s) return moe_form(form, kernel_of(@@pf_enc_moe_mm_iq3s_th_c), kernel_of(@@pf_enc_moe_mm_iq3s_th128_c), kernel_of(@@pf_enc_moe_mm_iq3s_thr_c))
    if (f == KqFmt.iq3xxs) return moe_form(form, kernel_of(@@pf_enc_moe_mm_iq3xxs_th_c), kernel_of(@@pf_enc_moe_mm_iq3xxs_th128_c), kernel_of(@@pf_enc_moe_mm_iq3xxs_thr_c))
    if (f == KqFmt.iq2s) return moe_form(form, kernel_of(@@pf_enc_moe_mm_iq2s_th_c), kernel_of(@@pf_enc_moe_mm_iq2s_th128_c), kernel_of(@@pf_enc_moe_mm_iq2s_thr_c))
    if (f == KqFmt.iq2xs) return moe_form(form, kernel_of(@@pf_enc_moe_mm_iq2xs_th_c), kernel_of(@@pf_enc_moe_mm_iq2xs_th128_c), kernel_of(@@pf_enc_moe_mm_iq2xs_thr_c))
    if (f != KqFmt.iq2xxs) {
        panic("moe_split_twin: no expert twin for {f}")
    }
    return moe_form(form, kernel_of(@@pf_enc_moe_mm_iq2xxs_th_c), kernel_of(@@pf_enc_moe_mm_iq2xxs_th128_c), kernel_of(@@pf_enc_moe_mm_iq2xxs_thr_c))
}

//! one split-format expert twin dispatch of stamp `k` over the contiguous fixtures: the dense binding layout (weights, X,
//! y, the kdim/ndim uniforms) plus the bucket counts and bases at 7/8, `tiles` M-tiles over `mp` padded rows
def private moe_split_dispatch(enc; fmt : int; k; pso;
                               bks, bkq, bxh, by, bk, bn, bcnt, bbase; nsb : int; mp, tiles, ndim, ne : int) {
    invoke(k.enc, enc, pso, bks, fmt == 45 || fmt == 40 ? 0ul : uint64(nsb * 16), bks, 0ul, bkq, 0ul, bxh, 0ul, by, 0ul, bk, bn, bcnt, bbase,
        int64(mp), int64(ndim), int64(ne), int64(tiles))
}

// the split-format expert twins against the same per-expert oracle as the K-quant MoE gate: the
// TH stamp, then the tall pair (128-floor stamp + remainder) where the counts cross 128
def private moe_mulmm_split_gate(t : T?; var dev, queue; fmt : int; kdim, ndim : int; counts : array<int>) {
    let tag = "moe_mulmm_split_k{fmt} k={kdim} n={ndim}"
    with_metal_cell(dev, queue) $(var mc : MetalCell) {
        var err : string
        let f = kq_fmt_of_id(fmt, "moe_mulmm_split_gate")
        let th = moe_split_twin(f, MoeSplitForm.th)
        var tw_pso = mc |> keep_stamp(th, err)
        return if (!tensor_pso_or_skip(t, tag, tw_pso, dev, err))
        let ne = length(counts)
        let nb = kdim / 256
        let eplane = ndim * nb
        let nsb = ne * eplane
        var inscope kq, ks : array<uint8>
        kq_fill_planes(fmt, nsb, kq, ks)
        let nk = 2
        var inscope fx <- moe_fix(counts, false, nk, 24)
        var inscope xf : array<float>
        xf |> resize(fx.pad * kdim)
        for (i in range(length(xf))) {
            xf[i] = float((i * 5) % 9 - 4)
        }
        var inscope wdeq : array<float>
        wdeq |> resize(ne * ndim * kdim)
        var inscope wrow : array<float>
        wrow |> resize(kdim)
        for (e in range(ne)) {
            for (j in range(ndim)) {
                kq_row_ref(fmt, e * eplane + j * nb, nb, nsb, kq, ks, wrow)
                for (c in range(kdim)) {
                    wdeq[(e * ndim + j) * kdim + c] = wrow[c]
                }
            }
        }
        var inscope wb, want, env : array<float>
        moe_mm_truth(fx, wdeq, xf, wb, kdim, ndim, false, want, env)
        var inscope xh <- [for (v in xf); float16(v)]
        let bks = mc |> in_plain(ks)
        let bkq = mc |> in_plain(kq)
        let bxh = mc |> in_plain(xh)
        let by = mc |> out_words(fx.pad * ndim, -500.0)
        let bk = mc |> in_plain([uint(kdim)])
        let bn = mc |> in_plain([uint(ndim)])
        let bcnt = mc |> in_plain(fx.cnt)
        let bbase = mc |> in_plain(fx.basep)
        let ran = mc |> cell_record(t, tag) $(enc : MetalComputeEncoder?) {
            moe_split_dispatch(enc, fmt, th, tw_pso, mc.buf[bks], mc.buf[bkq], mc.buf[bxh], mc.buf[by], mc.buf[bk], mc.buf[bn], mc.buf[bcnt], mc.buf[bbase], nsb, fx.pad, fx.maxtiles, ndim, ne)
        }
        if (ran) {
            check_buf(t, tag, mc.buf[by], want, env_bar(2e-4, env), Shape(row_width = ndim), env)
        }
        var maxce = 0
        for (c in fx.cnt) {
            maxce = max(maxce, int(c))
        }
        return if (maxce < 128)
        let tl = moe_split_twin(f, MoeSplitForm.th128)
        let rm = moe_split_twin(f, MoeSplitForm.thr)
        var tl_pso = mc |> keep_stamp(tl, err)
        var rm_pso = mc |> keep_stamp(rm, err)
        t |> success(tl_pso != null && rm_pso != null, "{tag} tall pair: pipeline: {err}")
        return if (tl_pso == null || rm_pso == null)
        let by_p = mc |> out_words(fx.pad * ndim, -700.0)
        let ranp = mc |> cell_record(t, "{tag} tall pair") $(enc : MetalComputeEncoder?) {
            moe_split_dispatch(enc, fmt, tl, tl_pso, mc.buf[bks], mc.buf[bkq], mc.buf[bxh], mc.buf[by_p], mc.buf[bk], mc.buf[bn], mc.buf[bcnt], mc.buf[bbase], nsb, fx.pad, (maxce + 127) / 128, ndim, ne)
            moe_split_dispatch(enc, fmt, rm, rm_pso, mc.buf[bks], mc.buf[bkq], mc.buf[bxh], mc.buf[by_p], mc.buf[bk], mc.buf[bn], mc.buf[bcnt], mc.buf[bbase], nsb, fx.pad, 4, ndim, ne)
        }
        return if (!ranp)
        check_buf(t, "{tag} tall pair", mc.buf[by_p], want, env_bar(2e-4, env), Shape(row_width = ndim), env)
    }
}

// The gathered q8 mul_mm (MetalMoeMulMmQ8) and its tensor twin — the twin serves CONTIGUOUS
// (gather=0, down-site) shapes only and never binds bkt.
def private moe_mulmm_q8_gate(t : T?; var dev, queue; tensor : bool; kdim, ndim : int; counts : array<int>; gather : bool; halfx : bool = false) {
    let tag = "moe_mulmm_q8{tensor ? "t" : ""}{halfx ? " halfx" : ""}{gather ? " gather" : ""} k={kdim} n={ndim}"
    with_metal_cell(dev, queue) $(var mc : MetalCell) {
        var err : string
        let kt = halfx ? kernel_of(@@pf_enc_moe_mm_q8_th_c) : kernel_of(@@pf_enc_moe_mm_q8_t_c)
        let kb = kernel_of(@@pf_enc_moe_mm_q8_c)
        var pso = tensor ? (mc |> keep_stamp(kt, err)) : (mc |> keep_stamp(kb, err))
        return if (!(tensor ? tensor_pso_or_skip(t, tag, pso, dev, err) : pso_ok(t, tag, pso, err)))
        let ne = length(counts)
        let nkb = kdim / 32
        var inscope blob : array<uint8>
        var inscope wdeq : array<float>
        q8_blob_fill(ne * ndim, nkb, blob, wdeq)
        let nk = 2
        let nx = 24
        var inscope fx <- moe_fix(counts, gather, nk, nx)
        var inscope xf : array<float>
        q8_x_fill((gather ? nx : fx.pad) * kdim, xf)
        var inscope xh : array<float16>
        if (halfx) {
            // round-trip through f16 so moe_mm_truth stays the arm's exact reference
            xh |> resize(length(xf))
            for (xv, hv in xf, xh) {
                hv = float16(xv)
                xv = float(hv)
            }
        }
        var inscope wb, want, env : array<float>
        moe_mm_truth(fx, wdeq, xf, wb, kdim, ndim, false, want, env)
        var ka = MoeMmArgs(kdim = uint(kdim), ndim = uint(ndim), eplane = uint(ndim * nkb),
            nk = uint(nk), gather = gather ? 1u : 0u)
        let bw = mc |> in_plain(blob)
        let bx = halfx ? (mc |> in_plain(xh)) : (mc |> in_(xf))
        let by = mc |> out_words(fx.pad * ndim, -1000.0)
        let bcnt = mc |> in_plain(fx.cnt)
        let bbase = mc |> in_plain(fx.basep)
        let bbkt = gather ? (mc |> in_plain(fx.bkt)) : (mc |> in_plain([0u]))
        let ran = mc |> cell_record(t, tag) $(enc : MetalComputeEncoder?) {
            if (tensor) {   // the tensor twin keeps its compact layout (contiguous-only, no bkt); one blob, its half-scale and byte views
                invoke(kt.enc, enc, pso, mc.buf[bw], 0ul, mc.buf[bw], mc.buf[bx], mc.buf[by], ka, mc.buf[bcnt], mc.buf[bbase], int64(ndim), int64(fx.maxtiles * 32), int64(ne))
            } else {
                invoke(kb.enc, enc, pso, mc.buf[bw], 0ul, mc.buf[bw], mc.buf[bx], mc.buf[by], ka, mc.buf[bcnt], mc.buf[bbase], mc.buf[bbkt], int64(ndim), int64(fx.maxtiles * 32), int64(ne))
            }
        }
        return if (!ran)
        check_buf(t, tag, mc.buf[by], want, env_bar(2e-3, env), Shape(row_width = ndim), env)
    }
}

// The gathered MXFP4 mul_mm (MetalMoeMulMmMx4) and its tensor twin: nibble plane + e8m0 scale
// plane + host-built vtab, per-expert bias folded when hasb (accumulator seed in the base,
// rank-1 chunk in the twin). Truth replicates the kernels' exact e8m0 decode.
def private moe_mulmm_mx4_gate(t : T?; var dev, queue; tensor : bool; kdim, ndim : int; counts : array<int>; gather, hasb : bool; halfx : bool = false) {
    let tag = "moe_mulmm_mx4{tensor ? "t" : ""}{halfx ? " halfx" : ""}{gather ? " gather" : ""}{hasb ? " bias" : ""} k={kdim} n={ndim}"
    with_metal_cell(dev, queue) $(var mc : MetalCell) {
        var err : string
        let kt = halfx ? kernel_of(@@pf_enc_moe_mm_mx4_th_c) : kernel_of(@@pf_enc_moe_mm_mx4_t_c)
        let kb = kernel_of(@@pf_enc_moe_mm_mx4_c)
        var pso = tensor ? (mc |> keep_stamp(kt, err)) : (mc |> keep_stamp(kb, err))
        return if (!(tensor ? tensor_pso_or_skip(t, tag, pso, dev, err) : pso_ok(t, tag, pso, err)))
        let ne = length(counts)
        let nkb = kdim / 32
        let tblk = ne * ndim * nkb
        var inscope vtab : array<float>
        vtab |> resize(16)
        for (i in range(8)) {
            let v = float(fixed_array(0, 1, 2, 3, 4, 6, 8, 12)[i])
            vtab[i] = v
            vtab[i + 8] = -v
        }
        var inscope nplane : array<uint>
        nplane |> resize(tblk * 4)
        for (i in range(tblk * 4)) {
            nplane[i] = uint(i * 3 + 1) * 2654435761u
        }
        var inscope eplane8 : array<uint8>
        eplane8 |> resize(tblk)
        for (i in range(tblk)) {
            eplane8[i] = uint8(124 + (i * 3 + 1) % 8)   // pow2 scales 2^-4..2^3 — f16-exact products
        }
        var inscope wdeq : array<float>
        wdeq |> resize(ne * ndim * kdim)
        for (blk in range(tblk)) {
            let sc = uint(eplane8[blk])
            let d = uint_bits_to_float(sc < 2u ? (0x00200000u << sc) : ((sc - 1u) << 23u))
            for (k in range(32)) {
                let byi = k % 16
                let nby = (nplane[blk * 4 + byi / 4] >> uint((byi % 4) * 8)) & 255u
                wdeq[blk * 32 + k] = vtab[int(k < 16 ? (nby & 15u) : (nby >> 4u))] * d
            }
        }
        var inscope wb : array<float>
        wb |> resize(ne * ndim)
        for (i in range(ne * ndim)) {
            wb[i] = float((i * 11) % 9 - 4) * 25.0   // bias >> the envelope band, or a dropped fold hides under it
        }
        let nk = 2
        let nx = 24
        var inscope fx <- moe_fix(counts, gather, nk, nx)
        var inscope xf : array<float>
        q8_x_fill((gather ? nx : fx.pad) * kdim, xf)
        var inscope xh : array<float16>
        if (halfx) {
            // round-trip through f16 so moe_mm_truth stays the arm's exact reference
            xh |> resize(length(xf))
            for (xv, hv in xf, xh) {
                hv = float16(xv)
                xv = float(hv)
            }
        }
        var inscope want, env : array<float>
        moe_mm_truth(fx, wdeq, xf, wb, kdim, ndim, hasb, want, env)
        var ka = MoeMmArgs(kdim = uint(kdim), ndim = uint(ndim), eplane = uint(ndim * nkb),
            nk = uint(nk), gather = gather ? 1u : 0u, hasb = hasb ? 1u : 0u)
        let bnq = mc |> in_plain(nplane)
        let bee = mc |> in_plain(eplane8)
        let bvt = mc |> in_(vtab)
        let bwb = mc |> in_(wb)
        let bx = halfx ? (mc |> in_plain(xh)) : (mc |> in_(xf))
        let by = mc |> out_words(fx.pad * ndim, -1000.0)
        let bcnt = mc |> in_plain(fx.cnt)
        let bbase = mc |> in_plain(fx.basep)
        let bbkt = gather ? (mc |> in_plain(fx.bkt)) : (mc |> in_plain([0u]))
        let ran = mc |> cell_record(t, tag) $(enc : MetalComputeEncoder?) {
            if (tensor) {
                invoke(kt.enc, enc, pso, mc.buf[bnq], 0ul, mc.buf[bee], 0ul, mc.buf[bx], mc.buf[by], ka, mc.buf[bcnt], mc.buf[bbase], mc.buf[bwb], mc.buf[bvt], int64(ndim), int64(fx.maxtiles * 32), int64(ne))
            } else {
                invoke(kb.enc, enc, pso, mc.buf[bnq], 0ul, mc.buf[bee], 0ul, mc.buf[bx], mc.buf[by], ka, mc.buf[bcnt], mc.buf[bbase], mc.buf[bbkt], mc.buf[bwb], mc.buf[bvt], int64(ndim), int64(fx.maxtiles * 32), int64(ne))
            }
        }
        return if (!ran)
        check_buf(t, tag, mc.buf[by], want, env_bar(2e-3, env), Shape(row_width = ndim), env)
    }
}

// ===== the prefill attention GEMM pair + Metal-4 tensor twins =====

// MetalAttnQKMm[T]: raw scores att[h x qrows x np32] = scale * q · k over surviving 32x64
// blocks — blocks fully above the causal diagonal (or fully outside the sliding window) exit
// without writing, which the sentinel whole-slab compare asserts. QK arms keep npos == np32:
// the tensor twin stages K pad rows RAW (no zero guard — the base has one), so pads must not
// exist for the pair to agree. `pair_mt` (32 or 128) names the device pair's stamp of that tile
// height instead - MetalAttnQKDev[128] on the Q twin and the scaled K twin; its operands are
// f16-exact, so the device differs by the half accumulate and the half store, 2^-11 of a
// magnitude each, under the bar's 2e-3 of the envelope.
//! `klead` > 0 puts that many foreign rows ahead of the device pair's K halves and binds K past them - a layer's slab inside a shared buffer.
def private attn_qk_mm_gate(t : T?; var dev, queue; tensor : bool; heads, hs, kv_mul, mp, np32, qoff, window : int;
                            uend : int = 0; halfq : bool = false; pair_mt : int = 0; klead : int = 0) {
    let pair = pair_mt > 0
    let mt = pair ? pair_mt : 32
    let form = pair ? "_dev mt={mt}" : "_mm{tensor ? "t" : ""}{halfq ? " halfq" : ""}"
    let tag = "attn_qk{form} h={heads} hs={hs} m={mp} n={np32} qoff={qoff} w={window} u={uend} klead={klead}"
    with_metal_cell(dev, queue) $(var mc : MetalCell) {
        var err : string
        var pso : MetalComputePipeline?
        if (pair_mt == 128) {
            pso = mc |> keep_stamp(kernel_of(@@enc_qk_dev128_c), err)
        } elif (pair) {
            pso = mc |> keep_stamp(kernel_of(@@enc_qk_dev_c), err)
        } else {
            pso = mc |> keep_stamp(halfq ? kernel_of(@@enc_qk_mm_th_c) : tensor ? kernel_of(@@enc_qk_mm_t_c) : kernel_of(@@enc_qk_mm_c), err)
        }
        return if (!(tensor || pair ? tensor_pso_or_skip(t, tag, pso, dev, err) : pso_ok(t, tag, pso, err)))
        let qd = heads * hs
        let kvd = (heads / kv_mul) * hs
        let scale = 0.125
        var inscope qv : array<float>
        q8_x_fill(mp * qd, qv)
        if (halfq || pair) {
            for (i in range(mp * qd)) {
                qv[i] = float(float16(qv[i]))   // f16-exact fixture: the oracle dot stays the reference
            }
        }
        var inscope kv : array<float>
        kv |> resize(np32 * kvd)
        for (i in range(np32 * kvd)) {
            kv[i] = float((i * 7 + 3) % 15 - 7) * 0.5
        }
        var inscope want, env : array<float>
        want |> resize(heads * mp * np32)
        env |> resize(heads * mp * np32)
        for (i in range(heads * mp * np32)) {
            want[i] = -1000.0
            env[i] = 0.0
        }
        for (h in range(heads)) {
            let kvhoff = (h / kv_mul) * hs
            for (m in range(mp)) {
                let mBase = m / mt * mt
                for (nb in range(np32 / 64)) {
                    let nBase = nb * 64
                    // fused span: tail rows above uend stay causal, so the skip floor is
                    // max(uend, the block's causal reach) — mirrors the kernel's gate
                    let skip = ((uend > 0 ? nBase >= max(uend, qoff + mBase + mt) : nBase > qoff + mBase + mt - 1)
                        || (window > 0 && qoff + mBase + 1 > window && nBase + 64 <= qoff + mBase + 1 - window))
                    continue if (skip)
                    for (pos in range(nBase, nBase + 64)) {
                        var acc = 0.0lf
                        var ev = 0.0lf
                        for (c in range(hs)) {
                            let p = double(qv[m * qd + h * hs + c]) * double(scale) * double(kv[pos * kvd + kvhoff + c])
                            acc += p
                            ev += abs(p)
                        }
                        want[(h * mp + m) * np32 + pos] = float(acc)
                        env[(h * mp + m) * np32 + pos] = float(ev)
                    }
                }
            }
        }
        var ka = AttnArgs(qd = uint(qd), kv_dim = uint(kvd), head_size = uint(hs), kv_mul = uint(kv_mul),
            npos = uint(np32), np32 = uint(np32), scale = scale, qoff = uint(qoff), qrows = uint(mp),
            window = uint(window), uend = uint(uend))
        var inscope qh <- [for (v in qv); float16(v)]
        var inscope kh : array<float16>
        kh |> reserve((klead + np32) * kvd)
        for (_i in range(klead * kvd)) {
            kh |> push(float16(977.0))
        }
        for (v in kv) {
            kh |> push(float16(v * scale))   // k x scale is f16-exact here: the K twin's own values
        }
        let bq = halfq || pair ? (mc |> in_plain(qh)) : (mc |> in_(qv))
        let bk = pair ? (mc |> in_plain(kh)) : (mc |> in_(kv))
        let batt = mc |> out_words(heads * mp * np32, float16(-1000.0))   // f16 slab; the sentinel is f16-exact
        let ran = mc |> cell_record(t, tag) $(enc : MetalComputeEncoder?) {
            if (pair_mt == 128) {
                enc_qk_dev128_c_pso(enc, pso, mc.buf[bq], mc.buf[bk], uint64(klead * kvd * 2), mc.buf[batt], ka, int64(mp), int64(np32), int64(heads))
            } elif (pair) {
                enc_qk_dev_c_pso(enc, pso, mc.buf[bq], mc.buf[bk], uint64(klead * kvd * 2), mc.buf[batt], ka, int64(mp), int64(np32), int64(heads))
            } elif (halfq) {
                enc_qk_mm_th_c_pso(enc, pso, mc.buf[bq], mc.buf[bk], mc.buf[batt], ka, int64(mp), int64(np32), int64(heads))
            } elif (tensor) {
                enc_qk_mm_t_c_pso(enc, pso, mc.buf[bq], mc.buf[bk], mc.buf[batt], ka, int64(mp), int64(np32), int64(heads))
            } else {
                enc_qk_mm_c_pso(enc, pso, mc.buf[bq], mc.buf[bk], mc.buf[batt], ka, int64(mp), int64(np32), int64(heads))
            }
        }
        return if (!ran)
        check_buf_h(t, tag, mc.buf[batt], want, env_bar(2e-3, env), Shape(row_width = np32), env)
    }
}

// MetalAttnAVMm[T]: xb[m x qd] = P · V where P = pf_p_weight(raw f16 score, rowstat) applied
// at staging — the gate feeds RAW scores + CPU-minted stats and oracles the same transform.
// The tensor twin zero-guards V rows >= npos — the npos < np32 arm fills the pad rows with
// garbage to prove the guard is live (their P weights are zero past each row's live length).
def private attn_av_mm_gate(t : T?; var dev, queue; tensor : bool; heads, hs, kv_mul, mp, np32, npos, qoff : int; uend : int = 0; padfree : bool = false) {
    let tag = "attn_av_mm{tensor ? "t" : ""}{padfree ? " padfree" : ""} h={heads} hs={hs} m={mp} n={np32} npos={npos} u={uend}"
    with_metal_cell(dev, queue) $(var mc : MetalCell) {
        var err : string
        let kav = (tensor ? (padfree ? kernel_of(@@enc_av_mm_tp_c) : kernel_of(@@enc_av_mm_t_c))
            : (padfree ? kernel_of(@@enc_av_mm_p_c) : kernel_of(@@enc_av_mm_c)))
        var pso = mc |> keep_stamp(kav, err)
        return if (!(tensor ? tensor_pso_or_skip(t, tag, pso, dev, err) : pso_ok(t, tag, pso, err)))
        let qd = heads * hs
        let kvd = (heads / kv_mul) * hs
        var inscope attf : array<float16>
        attf |> resize(heads * mp * np32)
        for (i in range(heads * mp * np32)) {
            attf[i] = float16(float((i * 3) % 13 - 6) * 0.25)   // raw scores, every slot populated
        }
        var inscope stats : array<float>
        stats |> resize(heads * mp * 2)
        for (h in range(heads)) {
            for (m in range(mp)) {
                let qpos = qoff + m
                let cnt = uend > 0 && qpos < uend ? uend : qpos + 1
                var rm = -1.0e30
                for (pos in range(cnt)) {
                    rm = max(rm, float(attf[(h * mp + m) * np32 + pos]))
                }
                var sum = 0.0
                for (pos in range(cnt)) {
                    sum += exp(float(attf[(h * mp + m) * np32 + pos]) - rm)
                }
                stats[(h * mp + m) * 2] = rm
                stats[(h * mp + m) * 2 + 1] = 1.0 / sum
            }
        }
        var inscope vv : array<float>
        vv |> resize(np32 * kvd)
        for (i in range(np32 * kvd)) {
            vv[i] = i / kvd < npos ? float((i * 7 + 3) % 15 - 7) * 0.5 : 999.0   // garbage pads: the guard's proof
        }
        var inscope want, env : array<float>
        want |> resize(mp * qd)
        env |> resize(mp * qd)
        for (h in range(heads)) {
            let kvhoff = (h / kv_mul) * hs
            for (m in range(mp)) {
                let qpos = qoff + m
                for (col in range(hs)) {
                    var acc = 0.0lf
                    var ev = 0.0lf
                    for (pos in range(npos)) {
                        let pw = pf_p_weight(float(attf[(h * mp + m) * np32 + pos]), uint(pos), uint(qpos),
                            0u, 0.0, uint(uend), 0u, stats[(h * mp + m) * 2], stats[(h * mp + m) * 2 + 1])
                        let p = double(pw) * double(vv[pos * kvd + kvhoff + col])
                        acc += p
                        ev += abs(p)
                    }
                    want[m * qd + h * hs + col] = float(acc)
                    env[m * qd + h * hs + col] = float(ev)
                }
            }
        }
        var ka = AttnArgs(qd = uint(qd), kv_dim = uint(kvd), head_size = uint(hs), kv_mul = uint(kv_mul),
            npos = uint(npos), np32 = uint(np32), scale = 1.0, qoff = uint(qoff), qrows = uint(mp),
            uend = uint(uend))
        let batt = mc |> in_plain(attf)
        let bstat = mc |> in_(stats)
        let bv = mc |> in_(vv)
        let bxb = mc |> out_words(mp * qd, -1000.0)
        let ran = mc |> cell_record(t, tag) $(enc : MetalComputeEncoder?) {
            invoke(kav.enc, enc, pso, mc.buf[batt], mc.buf[bv], mc.buf[bxb], ka, mc.buf[bstat], int64(mp), int64(heads), int64(hs))
        }
        return if (!ran)
        check_buf(t, tag, mc.buf[bxb], want, env_bar(2e-3, env), Shape(row_width = qd, head_width = hs), env)
    }
}

//! MetalHalfRows against the RNE narrow of the scaled f32 values - one multiply a value, so the device rounds as the host
//! does and the bar is no difference at all - written past `lead` halves of a target that keeps them and the slack after
//! `total` under a sentinel: the offset bind and the grid guard both hold. Two inputs sit past the half range on either sign
//! and must land the clamp (a bare narrow writes an infinity). A poisoned input reruns each scale as the control.
def private half_rows_gate(t : T?; var dev, queue) {
    let tag = "half_rows"
    with_metal_cell(dev, queue) $(var mc : MetalCell) {
        var err : string
        let kh = kernel_of(@@enc_half_rows)
        var pso = mc |> keep_stamp(kh, err)
        return if (!pso_ok(t, tag, pso, err))
        let total = 3 * 256 - 40
        let lead = 512
        let slack = 256
        for (scale in [1.0, 0.0883883]) {
            var inscope xv <- [for (i in range(total + slack)); float(float16(0.07 * float((i * 13) % 41) - 1.3))]
            xv[5] = 70000.0
            xv[total - 3] = -70000.0
            var inscope want <- [for (i in range(lead + total + slack)); i >= lead && i < lead + total ? float(float16(clamp(xv[i - lead] * scale, -65504.0, 65504.0))) : -77.0]
            let bx = mc |> in_(xv)
            let bh = mc |> out_words(lead + total + slack, float16(-77.0))
            var ka = HalfRowsArgs(total = uint(total), scale = scale)
            for (leg in range(2)) {
                if (leg == 1) {
                    mc |> plane_upload(bx, poisoned_copy(xv, 300, 2.0))
                    mc |> plane_refill(bh)   // the sentinel back before the run that must write the plane anew
                }
                let ran = mc |> cell_record(t, tag) $(enc : MetalComputeEncoder?) {
                    invoke(kh.enc, enc, pso, mc.buf[bx], mc.buf[bh], uint64(lead * 2), ka, int64(total + slack))
                }
                return if (!ran)
                if (leg == 0) {
                    check_buf_h(t, "{tag} scale={scale}", mc.buf[bh], want, Bar(rel = 0.0), Shape(row_width = 256))
                } else {
                    var inscope halves <- buf_download(mc.buf[bh], lead + total + slack, type<float16>)
                    var inscope got <- [for (h in halves); float(h)]
                    t |> success(mismatch_rel("{tag} poison (MUST mismatch)", got, want, 0.0, 0.0) > 0,
                        "{tag} scale={scale}: a poisoned element reds its half")
                }
            }
        }
    }
}

//! MetalAttnPExp + MetalAttnAVDev[128] in one record against a double softmax; garbage V pads and raw columns past each span red a walk past it.
//! The bar: each weight is stored a half (2^-11 of its magnitude) and the products accumulate in half, under 2e-3 of the envelope
//! `vlead` > 0 puts that many foreign rows ahead of the V halves and binds V past them.
def private attn_dev_pair_gate(t : T?; var dev, queue; mt, heads, hs, kv_mul, mp, np32, npos, qoff, window : int;
                               uend : int = 0; softcap : float = 0.0; hass : bool = false; ulo : int = 0; vlead : int = 0) {
    let tag = "attn_pexp+av_dev mt={mt} h={heads} hs={hs} m={mp} n={np32} npos={npos} qoff={qoff} w={window} u={ulo}..{uend} cap={softcap} sink={hass} vlead={vlead}"
    with_metal_cell(dev, queue) $(var mc : MetalCell) {
        var err : string
        let kpx = kernel_of(@@enc_attn_pexp)
        let kav = mt == 128 ? kernel_of(@@enc_av_dev128_c) : kernel_of(@@enc_av_dev_c)
        var pso_px = mc |> keep_stamp(kpx, err)
        return if (!pso_ok(t, tag, pso_px, err))
        var pso_av = mc |> keep_stamp(kav, err)
        return if (!tensor_pso_or_skip(t, tag, pso_av, dev, err))
        let tail = hs % 64 != 0   //! a head off the 64 lattice: its last 32 columns ride the tail stamp
        let kavt = mt == 128 ? kernel_of(@@enc_av_dev128_t_c) : kernel_of(@@enc_av_dev_t_c)
        var pso_avt = tail ? (mc |> keep_stamp(kavt, err)) : null
        return if (tail && !tensor_pso_or_skip(t, tag, pso_avt, dev, err))
        let qd = heads * hs
        let kvd = (heads / kv_mul) * hs
        var inscope attf : array<float16>
        attf |> resize(heads * mp * np32)
        for (i in range(heads * mp * np32)) {
            attf[i] = float16(float((i * 3) % 13 - 6) * 0.25)   // raw scores, every slot populated
        }
        var inscope sinks <- [for (h in range(heads)); 0.5 - 0.25 * float(h)]
        var inscope vv : array<float>
        vv |> resize(np32 * kvd)
        for (i in range(np32 * kvd)) {
            vv[i] = i / kvd < npos ? float((i * 7 + 3) % 15 - 7) * 0.5 : 999.0   // garbage pads: no tile's span reaches them
        }
        var inscope want, env : array<float>
        var inscope pw : array<double>
        want |> resize(mp * qd)
        env |> resize(mp * qd)
        pw |> resize(npos)
        for (h in range(heads)) {
            let kvhoff = (h / kv_mul) * hs
            for (m in range(mp)) {
                let qpos = qoff + m
                let cnt = uend > 0 && qpos >= ulo && qpos < uend ? uend : qpos + 1
                let jlo = window > 0 && qpos + 1 > window ? qpos + 1 - window : 0
                var rm = -1.0e30lf
                for (pos in range(jlo, cnt)) {
                    let sr = double(float(attf[(h * mp + m) * np32 + pos]))
                    let cap = double(softcap)
                    pw[pos] = softcap > 0.0 ? cap * (1.0lf - 2.0lf / (exp(sr * 2.0lf / cap) + 1.0lf)) : sr   // cap x tanh(s / cap)
                    rm = max(rm, pw[pos])
                }
                if (hass) {
                    rm = max(rm, double(sinks[h]))
                }
                var tot = hass ? exp(max(double(sinks[h]) - rm, -80.0lf)) : 0.0lf
                for (pos in range(jlo, cnt)) {
                    pw[pos] = exp(pw[pos] - rm)
                    tot += pw[pos]
                }
                for (col in range(hs)) {
                    var acc = 0.0lf
                    var ev = 0.0lf
                    for (pos in range(jlo, cnt)) {
                        let p = pw[pos] / tot * double(vv[pos * kvd + kvhoff + col])
                        acc += p
                        ev += abs(p)
                    }
                    want[m * qd + h * hs + col] = float(acc)
                    env[m * qd + h * hs + col] = float(ev)
                }
            }
        }
        var ka = AttnArgs(qd = uint(qd), kv_dim = uint(kvd), head_size = uint(hs), kv_mul = uint(kv_mul),
            npos = uint(npos), np32 = uint(np32), scale = 1.0, qoff = uint(qoff), qrows = uint(mp),
            window = uint(window), softcap = softcap, hass = hass ? 1u : 0u, uend = uint(uend), ulo = uint(ulo))
        var inscope vh : array<float16>
        vh |> reserve((vlead + np32) * kvd)
        for (_i in range(vlead * kvd)) {
            vh |> push(float16(977.0))
        }
        for (v in vv) {
            vh |> push(float16(v))
        }
        let batt = mc |> in_plain(attf)
        let bstat = mc |> out_words(heads * mp, -1000.0)
        let bsink = mc |> in_(sinks)
        let bv = mc |> in_plain(vh)
        let bxb = mc |> out_words(mp * qd, -1000.0)
        let ran = mc |> cell_record(t, tag) $(enc : MetalComputeEncoder?) {
            invoke(kpx.enc, enc, pso_px, mc.buf[batt], ka, mc.buf[bstat], mc.buf[bsink], int64(mp), int64(heads))
            invoke(kav.enc, enc, pso_av, mc.buf[batt], mc.buf[bv], uint64(vlead * kvd * 2), mc.buf[bxb], ka, mc.buf[bstat], int64(mp), int64(heads), int64(hs / 64))
            if (tail) {
                invoke(kavt.enc, enc, pso_avt, mc.buf[batt], mc.buf[bv], uint64(vlead * kvd * 2), mc.buf[bxb], ka, mc.buf[bstat], int64(mp), int64(heads), int64(hs))
            }
        }
        return if (!ran)
        check_buf(t, tag, mc.buf[bxb], want, env_bar(2e-3, env), Shape(row_width = qd, head_width = hs), env)
    }
}

// the stage width is a stamp constant the emitter's census records: the double-buffered stamps
// read the 128-wide body, the single-tile stamps the 64-wide one - device-free
[test]
def test_q8_mulmm_stamp_widths(t : T?) {
    t |> run("q8 mul_mm stamps read the stage width their constants set") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(das_metal)) {
            t |> success(find(MetalQ8MulMmTH_metal_q8_mulmm_t_msl_census, "call.tmm2d_q8u_f32.k64\n") >= 0, "TH width")
            t |> success(find(MetalQ8MulMmTH128_metal_q8_mulmm_t_msl_census, "call.tmm2d_q8u_f32.k64\n") >= 0, "TH128 width")
            t |> success(find(MetalQ8MulMmTHDb_metal_q8_mulmm_t_msl_census, "call.tmm2d_q8u_f32.k128\n") >= 0, "THDb width")
            t |> success(find(MetalQ8MulMmTHDb128_metal_q8_mulmm_t_msl_census, "call.tmm2d_q8u_f32.k128\n") >= 0, "THDb128 width")
        } else {
            t |> skip("no dasMetal in this build; the q8 stamp widths are Metal emission")
        }
    }
}

[test]
def test_metal_gemm_kernels(t : T?) {
    t |> run("prefill GEMM corpus (kq mul_mm trio + bf16 + Metal-4 tensor twins) == CPU references") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(das_metal)) {
            with_metal_gate(t) $(var dev : MetalDevice?; var queue : MetalCommandQueue?) {
                // the simdgroup kq trio + the tensor twins (twins skip without Metal-4)
                for (fmt in [4, 5, 6]) {
                    kq_mulmm_gate(t, dev, queue, fmt, false, 64, 512, 64)
                    kq_mulmm_gate(t, dev, queue, fmt, true, 64, 512, 64)
                    kq_mulmm_gate(t, dev, queue, fmt, true, 64, 512, 64, halfx = true)
                    // tall 128-row stamp: the deep-class in-kernel-dequant form (m = two tall tiles)
                    kq_mulmm_gate(t, dev, queue, fmt, true, 256, 512, 64, halfx = true, tall = true)
                    kq_mulmm_gate(t, dev, queue, fmt, true, 320, 512, 64, halfx = true, tall = true)   // off the 128 floor: the remainder pair at nonzero x/y offsets
                }
                // the gemma-4 dense site width: kdim 3840 is 15 superblocks per row - an odd,
                // non-power-of-two superblock count, off every other cell's 256/512 K. The K-quant
                // stamps index the quant plane by superblock, so the stride arithmetic only ever
                // divides evenly here by accident on a power-of-two K
                for (fmt in [4, 5, 6]) {
                    kq_mulmm_gate(t, dev, queue, fmt, false, 128, 3840, 128)
                    kq_mulmm_gate(t, dev, queue, fmt, true, 128, 3840, 128, halfx = true)
                    kq_mulmm_gate(t, dev, queue, fmt, true, 128, 3840, 128, halfx = true, tall = true)
                }
                kq_mulmm_gate(t, dev, queue, 4, false, 32, 256, 128)   // second shape: 2 col tiles
                kq_mulmm_gate(t, dev, queue, 44, false, 64, 512, 64)
                kq_mulmm_gate(t, dev, queue, 44, false, 32, 256, 128)
                kq_mulmm_gate(t, dev, queue, 3, false, 64, 512, 64)
                kq_mulmm_gate(t, dev, queue, 3, false, 32, 256, 128)
                kq_mulmm_gate(t, dev, queue, 33, false, 64, 512, 64)
                kq_mulmm_gate(t, dev, queue, 33, false, 32, 256, 128)
                // the iq3s tall stamp (form 1, the deep-K arm the K-quant sites take): two tall
                // tiles, then off the 128 floor with the 32-row remainder at nonzero x/y offsets
                kq_mulmm_gate(t, dev, queue, 33, true, 256, 512, 64, halfx = true, tall = true)
                kq_mulmm_gate(t, dev, queue, 33, true, 320, 512, 64, halfx = true, tall = true)
                kq_mulmm_gate(t, dev, queue, 34, false, 64, 512, 64)
                kq_mulmm_gate(t, dev, queue, 34, false, 32, 256, 128)
                kq_mulmm_gate(t, dev, queue, 34, true, 256, 512, 64, halfx = true, tall = true)
                kq_mulmm_gate(t, dev, queue, 34, true, 320, 512, 64, halfx = true, tall = true)
                kq_mulmm_gate(t, dev, queue, 45, false, 64, 512, 64)
                kq_mulmm_gate(t, dev, queue, 45, false, 32, 256, 128)
                kq_mulmm_gate(t, dev, queue, 40, false, 64, 512, 64)
                kq_mulmm_gate(t, dev, queue, 40, false, 32, 256, 128)
                kq_mulmm_gate(t, dev, queue, 2, false, 64, 512, 64)
                kq_mulmm_gate(t, dev, queue, 2, false, 32, 256, 128)
                kq_mulmm_gate(t, dev, queue, 23, false, 64, 512, 64)
                kq_mulmm_gate(t, dev, queue, 23, false, 32, 256, 128)
                kq_mulmm_gate(t, dev, queue, 24, false, 64, 512, 64)
                kq_mulmm_gate(t, dev, queue, 24, false, 32, 256, 128)
                kq_mulmm_gate(t, dev, queue, 25, false, 64, 512, 64)
                kq_mulmm_gate(t, dev, queue, 25, false, 32, 256, 128)
                // the iquant tensor twins (T + TH stamps; twins skip without Metal-4)
                for (fmt in [44, 45, 40, 3, 33, 34, 2, 23, 24, 25]) {
                    kq_mulmm_gate(t, dev, queue, fmt, true, 64, 512, 64)
                    kq_mulmm_gate(t, dev, queue, fmt, true, 64, 512, 64, halfx = true)
                }
                q8_mulmm_t_gate(t, dev, queue, 64, 256, 64)
                q8_mulmm_t_gate(t, dev, queue, 64, 256, 64, halfx = true)
                q8_mulmm_t_gate(t, dev, queue, 64, 160, 64)               // kdim % 64 == 32: the bk=64 helper's 32-tail chunk
                q8_mulmm_t_gate(t, dev, queue, 64, 160, 64, halfx = true)
                q8_mulmm_t_gate(t, dev, queue, 128, 256, 64, mt = 128)   // tall M-tile stamps
                q8_mulmm_t_gate(t, dev, queue, 256, 256, 64, halfx = true, mt = 128)
                q8_devw_gate(t, dev, queue, 64, 256, 64)
                q8_devw_gate(t, dev, queue, 32, 288, 128)   // non-256-multiple block count arms the dequant tail guard
                q8_devw_gate(t, dev, queue, 128, 256, 128, mt = 128)   // tall dev-W GEMM stamp
                for (fmt in [4, 5, 6, 44, 45, 40, 3, 33, 34, 2, 23, 24, 25]) {
                    kq_devw_gate(t, dev, queue, fmt, 64, 512, 64)
                    kq_devw_gate(t, dev, queue, fmt, 32, 512, 128)   // ndim%128: the tiled leg runs
                }
                for (fmt in [0, 4, 5, 6, 44, 45, 40, 3, 33, 34, 2, 23, 24, 25]) {
                    devw_bake_mirror_gate(t, dev, queue, fmt, 64, 512)
                }
                q8_dbuf_gate(t, dev, queue, 128, 256, 64)
                q8_dbuf_gate(t, dev, queue, 128, 288, 128)   //! kdim%64 == 32: the tail chunk on both arms
                q8_dbuf_gate(t, dev, queue, 256, 512, 128)   //! two M-tiles, deeper K
                q8_dbuf_gate(t, dev, queue, 64, 256, 64, mt = 32)   //! the 32-row stamp pair
                for (fmt in [4, 5, 6]) {
                    kq_dbuf_gate(t, dev, queue, fmt, 128, 512, 64)
                }
                bf16_mulmm_gate(t, dev, queue, false, 64, 256, 64)
                bf16_mulmm_gate(t, dev, queue, true, 64, 256, 64)
                bf16_mulmm_gate(t, dev, queue, true, 64, 256, 64, halfx = true)
                bf16_mulmm_gate(t, dev, queue, true, 128, 256, 64, mt = 128)   // tall M-tile stamps
                bf16_mulmm_gate(t, dev, queue, true, 256, 256, 64, halfx = true, mt = 128)

                // ===== the dense f32 tower stamp =====
                f32_mulmm_gate(t, dev, queue, true, 32, 32, 384, 384)      // whisper-tiny qkv/o, exact stage
                f32_mulmm_gate(t, dev, queue, false, 32, 32, 384, 1536)    // tiny fc1, rounded stage
                f32_mulmm_gate(t, dev, queue, false, 32, 32, 1536, 384)    // tiny fc2
                f32_mulmm_gate(t, dev, queue, false, 64, 64, 896, 896)     // qwen3a qkv/o
                f32_mulmm_gate(t, dev, queue, true, 1504, 1504, 1280, 1280)  // whisper-large T=1500 tile count
                f32_mulmm_gate(t, dev, queue, false, 64, 64, 6912, 3840)   // gemma4uv patch GEMM - longest dot
                f32_mulmm_gate(t, dev, queue, false, 100, 128, 3840, 3840) // gemma4uv proj + M-pad rows pinned 0
                // the batched-decode q8 GEMM family: dense + fused-QKV ys stride, both twins
                q8_gemm_b_gate(t, dev, queue, false, 64, 128, 64, 64)
                q8_gemm_b_gate(t, dev, queue, false, 64, 128, 64, 128)
                q8_gemm_b_gate(t, dev, queue, true, 64, 128, 64, 64)
                q8_gemm_b_gate(t, dev, queue, true, 64, 128, 64, 128)
                q8_gemm_sk_gate(t, dev, queue, false, 128, 64, 2)
                q8_gemm_sk_gate(t, dev, queue, false, 128, 64, 4)      // kbn == 1 edge
                q8_gemm_sk_gate(t, dev, queue, true, 128, 64, 2)
                q8_gemm64_b_gate(t, dev, queue, false, 64, 128, 128)
                q8_gemm64_b_gate(t, dev, queue, true, 64, 128, 128)
                // the MoE gathered mul_mm family: kq trio + q8/mx4 with their tensor twins
                // (a zero-count expert rides the first arm; twins are contiguous-only by design)
                moe_mulmm_kq_gate(t, dev, queue, 4, 256, 64, [32, 0, 40, 8], true)
                moe_mulmm_kq_gate(t, dev, queue, 5, 256, 64, [32, 40, 8], true)
                moe_mulmm_kq_gate(t, dev, queue, 6, 256, 64, [32, 40, 8], true)
                moe_mulmm_kq_gate(t, dev, queue, 4, 256, 128, [8, 33], false)   // 2 col tiles + contiguous
                moe_mulmm_kq_gate(t, dev, queue, 5, 256, 128, [8, 33], false)   // twin legs per format
                moe_mulmm_kq_gate(t, dev, queue, 6, 256, 128, [8, 33], false)
                moe_mulmm_kq_gate(t, dev, queue, 4, 256, 128, [130, 40, 200], false)   // tall pair: counts cross 128
                for (fmt in [3, 44, 40, 33, 34, 23, 24, 25]) {
                    moe_mulmm_split_gate(t, dev, queue, fmt, 256, 128, [8, 33])
                    moe_mulmm_split_gate(t, dev, queue, fmt, 256, 128, [130, 40, 200])
                }
                // the q40 expert site at the gemma-4-26B-A4B geometry: kdim = dim 2816, ndim =
                // n_ff_exp 704 (the widest superblock count a routed q40 stack walks)
                moe_mulmm_split_gate(t, dev, queue, 40, 2816, 704, [8, 33])
                moe_mulmm_q8_gate(t, dev, queue, false, 96, 64, [32, 40, 8], true)
                moe_mulmm_q8_gate(t, dev, queue, false, 128, 64, [8, 33], false)
                moe_mulmm_q8_gate(t, dev, queue, true, 128, 64, [8, 33], false)
                moe_mulmm_q8_gate(t, dev, queue, true, 128, 64, [8, 33], false, halfx = true)
                moe_mulmm_mx4_gate(t, dev, queue, false, 96, 64, [32, 40, 8], true, true)
                moe_mulmm_mx4_gate(t, dev, queue, false, 128, 64, [8, 33], false, false)
                moe_mulmm_mx4_gate(t, dev, queue, true, 128, 64, [8, 33], false, true)
                moe_mulmm_mx4_gate(t, dev, queue, true, 128, 64, [8, 33], false, false)
                moe_mulmm_mx4_gate(t, dev, queue, true, 128, 64, [8, 33], false, true, halfx = true)
                // the prefill attention GEMM pair: causal-skip, window-skip, GQA, 2-slab hs,
                // and the AV pad-guard arm (npos < np32, garbage V pads)
                attn_qk_mm_gate(t, dev, queue, false, 4, 64, 2, 64, 128, 0, 0)
                attn_qk_mm_gate(t, dev, queue, true, 4, 64, 2, 64, 128, 0, 0)
                attn_qk_mm_gate(t, dev, queue, true, 4, 64, 2, 64, 128, 0, 0, halfq = true)   // half-Q stamp
                attn_qk_mm_gate(t, dev, queue, false, 4, 64, 2, 32, 256, 224, 128)
                attn_qk_mm_gate(t, dev, queue, true, 4, 64, 2, 32, 256, 224, 128)
                attn_qk_mm_gate(t, dev, queue, true, 4, 64, 2, 32, 256, 224, 128, halfq = true)
                attn_qk_mm_gate(t, dev, queue, false, 2, 128, 1, 32, 64, 32, 0)   // two K-slabs
                attn_qk_mm_gate(t, dev, queue, true, 2, 128, 1, 32, 64, 32, 0)
                attn_av_mm_gate(t, dev, queue, false, 4, 64, 2, 64, 128, 128, 64)
                attn_av_mm_gate(t, dev, queue, true, 4, 64, 2, 64, 128, 128, 64)
                attn_av_mm_gate(t, dev, queue, false, 4, 64, 2, 64, 128, 96, 64)
                attn_av_mm_gate(t, dev, queue, true, 4, 64, 2, 64, 128, 96, 64)
                attn_av_mm_gate(t, dev, queue, false, 2, 128, 1, 32, 64, 64, 32)  // vBase=64 tile
                attn_av_mm_gate(t, dev, queue, true, 2, 128, 1, 32, 64, 64, 32)
                // the PADFREE (clean) stamps: npos == np32 divisible, no pad row in any chunk
                attn_av_mm_gate(t, dev, queue, false, 4, 64, 2, 64, 128, 128, 64, padfree = true)
                attn_av_mm_gate(t, dev, queue, true, 4, 64, 2, 64, 128, 128, 64, padfree = true)
                attn_av_mm_gate(t, dev, queue, false, 4, 64, 2, 64, 64, 64, 0, padfree = true)   // fresh square (the span-fused head shape)
                attn_av_mm_gate(t, dev, queue, false, 4, 64, 2, 32, 96, 96, 64, uend = 96, padfree = true)   // span rows under uend
                attn_av_mm_gate(t, dev, queue, true, 4, 64, 2, 64, 128, 128, 0, uend = 128, padfree = true)
                // the uniform-span (uend) arms: every query row attends [0, uend) — base AND
                // tensor twins (the _t twins' only span coverage; they skip without Metal-4)
                attn_qk_mm_gate(t, dev, queue, false, 4, 64, 2, 64, 128, 0, 0, uend = 128)
                attn_qk_mm_gate(t, dev, queue, true, 4, 64, 2, 64, 128, 0, 0, uend = 128)
                attn_qk_mm_gate(t, dev, queue, false, 4, 64, 2, 32, 256, 224, 128, uend = 192)   // span past the window floor
                attn_qk_mm_gate(t, dev, queue, true, 4, 64, 2, 32, 256, 224, 128, uend = 192)
                attn_av_mm_gate(t, dev, queue, false, 4, 64, 2, 64, 128, 96, 0, uend = 96)
                attn_av_mm_gate(t, dev, queue, true, 4, 64, 2, 64, 128, 96, 0, uend = 96)
                // the device pair: both tile heights, continuation, window skip, GQA, uniform span, V pads, softcap, sinks
                attn_qk_mm_gate(t, dev, queue, true, 4, 64, 2, 64, 128, 0, 0, pair_mt = 32)
                attn_qk_mm_gate(t, dev, queue, true, 4, 64, 2, 128, 256, 128, 0, pair_mt = 128)
                attn_qk_mm_gate(t, dev, queue, true, 4, 64, 2, 32, 256, 224, 128, pair_mt = 32)
                attn_qk_mm_gate(t, dev, queue, true, 2, 128, 1, 128, 384, 256, 192, pair_mt = 128)
                attn_qk_mm_gate(t, dev, queue, true, 4, 64, 2, 64, 128, 0, 0, uend = 128, pair_mt = 32)
                attn_qk_mm_gate(t, dev, queue, true, 4, 64, 2, 32, 256, 224, 128, uend = 192, pair_mt = 32)
                attn_qk_mm_gate(t, dev, queue, true, 4, 64, 2, 32, 256, 224, 0, pair_mt = 32, klead = 96)
                attn_qk_mm_gate(t, dev, queue, true, 2, 128, 1, 128, 384, 256, 0, pair_mt = 128, klead = 96)
                attn_qk_mm_gate(t, dev, queue, true, 4, 96, 2, 64, 128, 0, 0, pair_mt = 32)
                attn_qk_mm_gate(t, dev, queue, true, 4, 96, 2, 128, 256, 128, 0, pair_mt = 128)
                attn_dev_pair_gate(t, dev, queue, 32, 4, 64, 2, 32, 256, 256, 224, 0, vlead = 96)
                attn_dev_pair_gate(t, dev, queue, 128, 4, 64, 2, 128, 256, 256, 128, 0, vlead = 96)
                half_rows_gate(t, dev, queue)
                attn_dev_pair_gate(t, dev, queue, 32, 4, 64, 2, 64, 128, 128, 64, 0)
                attn_dev_pair_gate(t, dev, queue, 32, 4, 64, 2, 64, 128, 96, 32, 0)        // V pads past npos
                attn_dev_pair_gate(t, dev, queue, 32, 4, 96, 2, 64, 128, 100, 32, 0)
                attn_dev_pair_gate(t, dev, queue, 128, 4, 96, 2, 128, 256, 200, 64, 40, vlead = 96)
                attn_dev_pair_gate(t, dev, queue, 32, 4, 96, 2, 64, 128, 100, 32, 24, softcap = 0.5, hass = true)   // the tail under a window, the cap and sinks
                attn_dev_pair_gate(t, dev, queue, 32, 4, 96, 2, 32, 96, 96, 64, 0, uend = 96)                      // ... and over a uniform span
                attn_dev_pair_gate(t, dev, queue, 128, 4, 96, 2, 128, 256, 200, 0, 0, uend = 128, softcap = 0.5, ulo = 32)
                attn_dev_pair_gate(t, dev, queue, 128, 4, 64, 2, 128, 256, 256, 128, 0)
                attn_dev_pair_gate(t, dev, queue, 128, 2, 128, 1, 128, 320, 300, 172, 0)   // the last tile's span ends at npos
                attn_dev_pair_gate(t, dev, queue, 32, 4, 64, 2, 32, 256, 256, 224, 128)    // a sliding window
                attn_dev_pair_gate(t, dev, queue, 128, 4, 64, 2, 128, 384, 384, 256, 160)
                attn_dev_pair_gate(t, dev, queue, 32, 4, 64, 2, 64, 128, 96, 0, 0, uend = 96)
                attn_dev_pair_gate(t, dev, queue, 32, 4, 64, 2, 64, 128, 96, 0, 0, uend = 48, ulo = 16)   // causal rows either side of the uniform span
                attn_dev_pair_gate(t, dev, queue, 128, 4, 64, 2, 128, 256, 200, 64, 0, uend = 160, ulo = 100)
                attn_dev_pair_gate(t, dev, queue, 32, 4, 64, 2, 64, 128, 128, 64, 0, softcap = 2.0)
                attn_dev_pair_gate(t, dev, queue, 32, 4, 64, 2, 64, 128, 128, 64, 0, hass = true)
            }
        } else {
            t |> skip("das_metal is not built on this platform; GEMM kernel checks skipped")
        }
    }
}
