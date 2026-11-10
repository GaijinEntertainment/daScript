options gen2
options stack = 524288   // every dasLLAMA program root takes this budget
options _function_length = 0        // per-kernel gates: fixture + dispatch + oracle compare, one flat run each
options _cyclomatic_complexity = 0  // same — the arm ladders follow the kernel's own binding/variant axes
options _dasllama_internal = true
options persistent_heap   // a dastest root frees what it deletes only on this heap; the linear heap holds every carrier a cell deleted until the context ends

require dastest/testing_boost public
require daslib/math_bits            // float_bits_to_uint / uint_bits_to_float — the bf16 halfword pun
require ?das_metal dasllama/dasllama_metal_kernels  // the kq mul_mm trio (Apple builds)
require ?das_metal dasllama/dasllama_metal_prefill  // the mul_mm twins (bf16 + Metal-4 tensor forms)
require ?das_metal dasllama/dasllama_metal_common   // the pipeline globals the generated builders read
require ?das_metal metal/das_metal_boost  // device/pipeline/buffer plumbing + the leak gate
require ?das_metal _metal_kernel_common   // the MetalCell record, check_buf, the device shell, kq plane builders
require ?das_metal _metal_kq_kernels   // kq_mm_form_kernel / kq_dq_kernel: the dense kq kernel pickers
require ?das_metal _rows8_probe   // rows8_probe_ok: the rows8 cells' layout skip
require dasllama/dasllama_convert         // devw_dequant_*_core — the image mint's dev-W bake mirrors
require dasllama/dasllama_gemm_schema     // kq_qsb — the ONE per-format quant-plane stride source
require dasllama/dasllama_kqformat        // KqFmt: the per-format stamp tables' key
require dasllama/dasllama_gpu_math        // e8m0_half_f32: the mx4 truth's scale decode, the kernels' own
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
// fails to compile is a RED, and the simdgroup bases always assert. A rows8 cell skips where the
// rows8 layout probe finds the matrix unit's cooperative layout another generation's (the engine
// keeps the form off). Without das_metal in the build, or with no Metal device, every check skips.

def private kq_mulmm_gate(t : T?; var dev, queue; fmt : int; tensor : bool; m, kdim, ndim : int; halfx : bool = false; tall : bool = false; m8 : bool = false) {
    let tag = "kq_mulmm_k{fmt}{tensor ? "t" : ""}{tall ? " tall" : ""}{m8 ? " m8" : ""}{halfx ? " halfx" : ""} m={m} k={kdim} n={ndim}"
    with_metal_cell(dev, queue) $(var mc : MetalCell) {
        var err : string
        let f = kq_fmt_of_id(fmt, "kq_mulmm_gate")
        let sb = kq_mm_base_stamp(f)
        let kt = tall ? kq_mm_form_kernel(f, "th128") : kq_mm_form_kernel(f, m8 ? "t8" : (halfx ? "th" : "t"))   //! m8: the eight-row stamp over an eight-row panel
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
        let kr = kq_mm_form_kernel(f, "th")
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
            : mt == 8 ? kernel_of(@@enc_gemm_mm_t8_c)   //! the eight-row stamp (f32 X) over an eight-row panel
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
            q8_devw_split_leg(t, mc, tag, kd, dq_pso, bw, bx, bk, bn, m, kdim, ndim, ref.want, ref.env)
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

//! The dev-W column tiles with the half GEMM split along K (`MetalHalfMulMmTHSk`): each tile's partial planes summed into its
//! column slice of the wider output by the strided reduce (`SkReduceArgs.cols4` / `ld4`), the production split form
[arch(at="../ARCHITECTURE_GPU_PREFILL_GEMM.md#prefill-split-k")]
def private q8_devw_split_leg(t : T?; var mc : MetalCell; tag : string; kd; dq_pso; bw, bx, bk, bn : int; m, kdim, ndim : int;
                              want, env : array<float>) {
    var err : string
    let ks = kernel_of(@@enc_gemm_hmm_thsk)
    var sk_pso = mc |> keep_stamp(ks, err)
    return if (!tensor_pso_or_skip(t, "{tag} tiled split", sk_pso, mc.dev, err))
    let kr = stamp_of(@@enc_sk_reduce_c)
    var red_pso = mc |> keep_stamp(kr, err)
    return if (!pso_ok(t, "{tag} split reduce", red_pso, err))
    let nsk = 2
    return if (kdim % (nsk * 64) != 0)   //! a share is whole 64-wide chunks, as the production rule sizes it
    let td = ndim / 2
    let nbr = kdim / 32
    let by_s = mc |> out_words(m * ndim, NAN_SENTINEL)
    let bwh_a = mc |> out_words(td * kdim, float16(-100.0))
    let bwh_b = mc |> out_words(td * kdim, float16(-100.0))
    let bpart = mc |> out_words(nsk * m * td, NAN_SENTINEL)
    let bpart2 = mc |> out_words(nsk * m * td, NAN_SENTINEL)
    var tka = CvtArgs(total = uint(td * nbr))
    var ka = SkReduceArgs(total = uint(m * td / 4), ksplit = uint(nsk), cols4 = uint(td / 4), ld4 = uint(ndim / 4))
    let ran = mc |> cell_record(t, "{tag} tiled split") $(enc : MetalComputeEncoder?) {
        for (ti in range(2)) {
            let tbase = ti * td
            let bwh_ti = ti == 0 ? bwh_a : bwh_b
            let bp = ti == 0 ? bpart : bpart2
            invoke(kd.enc, enc, dq_pso, mc.buf[bw], uint64(tbase * nbr * 34), mc.buf[bw], mc.buf[bwh_ti], tka, int64(td * nbr))
            invoke(ks.enc, enc, sk_pso, mc.buf[bwh_ti], 0ul, mc.buf[bx], 0ul, mc.buf[bp], 0ul, mc.buf[bk], mc.buf[bn], int64(m), int64(td), int64(nsk))
            stamp_run(kr, red_pso) {
                invoke(kr.enc, enc, mc.buf[bp], mc.buf[by_s], uint64(tbase * 4), ka, int64(((m - 1) * ndim + td) / 4))   //! the writes' reach from the tile's column
            }
        }
    }
    if (ran) {
        check_buf(t, "{tag} tiled split", mc.buf[by_s], want, env_bar(2e-3, env), Shape(row_width = ndim), env)
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
    let compact = fmt == 4 || fmt == 5 || fmt == 45 || fmt == 40 || fmt == 49
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

def private kq_dbuf_gate(t : T?; var dev, queue; fmt : int; m, kdim, ndim : int; mt : int = 128) {
    let tag = "kq_dbuf_k{fmt} mt{mt} m={m} k={kdim} n={ndim}"
    with_metal_cell(dev, queue) $(var mc : MetalCell) {
        var err : string
        let f = kq_fmt_of_id(fmt, "kq_dbuf_gate")
        let ka = mt == 128 ? kq_mm_form_kernel(f, "th128") : kq_mm_form_kernel(f, "th")
        let kb = mt == 128 ? kq_mm_form_kernel(f, "thdb128") : kq_mm_form_kernel(f, "thdb")
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
        let compact = fmt == 4 || fmt == 5 || fmt == 45 || fmt == 40 || fmt == 49
        let doff = compact ? 0ul : uint64(nsb * 16)
        let ran = mc |> cell_record(t, tag) $(enc : MetalComputeEncoder?) {
            invoke(ka.enc, enc, pso_a, mc.buf[bks], doff, mc.buf[bks], 0ul, mc.buf[bkq], 0ul, mc.buf[bx], 0ul, mc.buf[by_a], 0ul, mc.buf[bk], mc.buf[bn], int64(m), int64(ndim))
            invoke(kb.enc, enc, pso_b, mc.buf[bks], doff, mc.buf[bks], 0ul, mc.buf[bkq], 0ul, mc.buf[bx], 0ul, mc.buf[by_b], 0ul, mc.buf[bk], mc.buf[bn], int64(m), int64(ndim))
        }
        return if (!ran)
        dbuf_identity(t, mc, tag, by_a, by_b, m * ndim)
        check_buf(t, tag, mc.buf[by_a], ref.want, env_bar(2e-4, ref.env), Shape(row_width = ndim), ref.env)
    }
}

//! the split-K form's reduce: `nsk` partial planes of `total` floats summed into `by` (the decode batch's shared stamp)
//! The q8 blob's split-K stamp (`MetalQ8MulMmTHSk` / `TSk`, 32 output columns a threadgroup) against the CPU product: at one
//! split it writes y itself, past one its partial planes go through the reduce; W staged to half, X f16-exact (one staged operand)
[arch(at="../ARCHITECTURE_GPU_PREFILL_GEMM.md#prefill-split-k")]
def private q8_split_gate(t : T?; var dev, queue; m, kdim, ndim, nsk : int; halfx : bool) {
    let tag = "q8_split{halfx ? " halfx" : ""} m={m} k={kdim} n={ndim} nsk={nsk}"
    with_metal_cell(dev, queue) $(var mc : MetalCell) {
        var err : string
        //! m = 16: the short window's 16-row stamps
        let kt = (m == 16 ? (halfx ? kernel_of(@@enc_gemm_mm_thsk16_c) : kernel_of(@@enc_gemm_mm_tsk16_c))
            : m == 8 ? kernel_of(@@enc_gemm_mm_tsk8_c)   //! the eight-row split stamp (f32 X)
            : (halfx ? kernel_of(@@enc_gemm_mm_thsk_c) : kernel_of(@@enc_gemm_mm_tsk_c)))
        var pso = mc |> keep_stamp(kt, err)
        return if (!tensor_pso_or_skip(t, tag, pso, dev, err))
        var inscope blob : array<uint8>
        var inscope wdeq : array<float>
        q8_blob_fill(ndim, kdim / 32, blob, wdeq)
        var inscope xf : array<float>
        q8_x_fill(m * kdim, xf)
        var inscope xh <- [for (x in xf); float16(x)]   // the fixture is f16-exact, so the oracle dot is unchanged
        var inscope ref <- dense_ref_rows(wdeq, xf, m, kdim, ndim)
        let bw = mc |> in_plain(blob)
        let bx = halfx ? (mc |> in_plain(xh)) : (mc |> in_(xf))
        split_cell(t, mc, tag, m, kdim, ndim, nsk, false, ref.want, ref.env, staged_bar(1, kdim, ref.env)) $(enc; bo, bk, bn) {
            invoke(kt.enc, enc, pso, mc.buf[bw], 0ul, mc.buf[bw], mc.buf[bx], 0ul, bo, 0ul, bk, bn, int64(m), int64(ndim), int64(nsk))
        }
    }
}

//! the fixture a K-quant split gate runs: the format's planes over `ndim` rows of `kdim`, `m` activation rows (`x(i)`), and the
//! dequantized rows' product
struct private KqSplitFixture {
    kq : array<uint8>
    ks : array<uint8>
    xf : array<float>
    ref : DenseRef
}

def private kq_split_fixture(fmt, m, kdim, ndim : int; x : block<(i : int) : float>) : KqSplitFixture {
    var fx : KqSplitFixture
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let nb = kdim / 256
        let nsb = ndim * nb
        kq_fill_planes(fmt, nsb, fx.kq, fx.ks)
        fx.xf <- [for (i in range(m * kdim)); invoke(x, i)]
        fx.ref <- dense_ref_env(m, kdim, ndim, fx.xf) $(j; var wrow) {
            kq_row_ref(fmt, j * nb, nb, nsb, fx.kq, fx.ks, wrow)
        }
    }
    return <- fx
}

//! A dense K-quant or grid format's split-K stamp (`kq_mm:<fmt>:thsk` / `tsk`) against the dequantized rows' product, the same
//! two paths as the q8 cell; W staged to half, X f16-exact
[arch(at="../ARCHITECTURE_GPU_PREFILL_GEMM.md#prefill-split-k")]
def private kq_split_gate(t : T?; var dev, queue; fmt : int; m, kdim, ndim, nsk : int; halfx : bool) {
    let tag = "kq_split_k{fmt}{halfx ? " halfx" : ""} m={m} k={kdim} n={ndim} nsk={nsk}"
    with_metal_cell(dev, queue) $(var mc : MetalCell) {
        var err : string
        let f = kq_fmt_of_id(fmt, "kq_split_gate")
        let kt = m == 16 ? kq_mm_split16_kernel(f, halfx) : (m == 8 ? kq_mm_split8_kernel(f) : kq_mm_split_kernel(f, halfx))   //! m = 16: the short window's 16-row stamps; m = 8: the rows sites' eight-row stamp
        var pso = mc |> keep_stamp(kt, err)
        return if (!tensor_pso_or_skip(t, tag, pso, dev, err))
        var inscope fx <- kq_split_fixture(fmt, m, kdim, ndim) $(i) => float(float16(0.05 * float(i % 41) - 0.9))   //! f16 round-trip: the oracle dot stays exact
        var inscope xh <- [for (x in fx.xf); float16(x)]
        let bks = mc |> in_plain(fx.ks)
        let bkq = mc |> in_plain(fx.kq)
        let bx = halfx ? (mc |> in_plain(xh)) : (mc |> in_(fx.xf))
        let doff = kq_split_doff(fmt, ndim * (kdim / 256))
        split_cell(t, mc, tag, m, kdim, ndim, nsk, false, fx.ref.want, fx.ref.env, staged_bar(1, kdim, fx.ref.env)) $(enc; bo, bk, bn) {
            invoke(kt.enc, enc, pso, mc.buf[bks], doff, mc.buf[bks], 0ul, mc.buf[bkq], 0ul, mc.buf[bx], 0ul, bo, 0ul, bk, bn, int64(m), int64(ndim), int64(nsk))
        }
    }
}

//! The simdgroup base tiles' split-K stamps (`MetalQ8MulMmSk`, `MetalKqMulMm<Fmt>Sk`) - the forms a box with no dense tensor
//! crown splits on - against the CPU product, the partial planes through the reduce at every count; `fmt` 0 is the q8 blob. The
//! base tiles stage W and X to half: the q8 X fixture is f16-exact (one staged operand), the K-quant one off the lattice (two)
[arch(at="../ARCHITECTURE_GPU_PREFILL_GEMM.md#prefill-split-k")]
def private base_split_gate(t : T?; var dev, queue; fmt : int; m, kdim, ndim, nsk : int) {
    let tag = "base_split fmt{fmt} m={m} k={kdim} n={ndim} nsk={nsk}"
    with_metal_cell(dev, queue) $(var mc : MetalCell) {
        var err : string
        if (fmt == 0) {
            let st = stamp_of(@@enc_gemm_mm_sk_c)
            var pso = mc |> keep_stamp(st, err)
            return if (!pso_ok(t, tag, pso, err))
            var inscope blob : array<uint8>
            var inscope wdeq : array<float>
            q8_blob_fill(ndim, kdim / 32, blob, wdeq)
            var inscope xf : array<float>
            q8_x_fill(m * kdim, xf)
            var inscope ref <- dense_ref_rows(wdeq, xf, m, kdim, ndim)
            let bw = mc |> in_plain(blob)
            let bx = mc |> in_(xf)
            split_cell(t, mc, tag, m, kdim, ndim, nsk, true, ref.want, ref.env, staged_bar(1, kdim, ref.env)) $(enc; bo, bk, bn) {
                stamp_run(st, pso) {
                    invoke(st.enc, enc, mc.buf[bw], 0ul, mc.buf[bw], mc.buf[bx], bo, 0ul, bk, bn, int64(m), int64(ndim), int64(nsk))
                }
            }
        } else {
            let st = kq_mm_base_split_stamp(kq_fmt_of_id(fmt, "base_split_gate"))
            var pso = mc |> keep_stamp(st, err)
            return if (!pso_ok(t, tag, pso, err))
            var inscope fx <- kq_split_fixture(fmt, m, kdim, ndim) $(i) => 0.07 * float(i % 39) - 1.3
            let bks = mc |> in_plain(fx.ks)
            let bkq = mc |> in_plain(fx.kq)
            let bx = mc |> in_(fx.xf)
            let doff = kq_split_doff(fmt, ndim * (kdim / 256))
            split_cell(t, mc, tag, m, kdim, ndim, nsk, true, fx.ref.want, fx.ref.env, staged_bar(2, kdim, fx.ref.env)) $(enc; bo, bk, bn) {
                stamp_run(st, pso) {
                    invoke(st.enc, enc, mc.buf[bks], doff, mc.buf[bks], 0ul, mc.buf[bkq], 0ul, mc.buf[bx], bo, 0ul, bk, bn, int64(m), int64(ndim), int64(nsk))
                }
            }
        }
    }
}

//! The bf16 plane's split-K stamp (`MetalBf16MulMmTHSk` / `TSk`) against the exact widened rows' product: the fixture's W is
//! f16-exact and its X too, so staging to half rounds nothing and the bar is the f32 sum's alone
[arch(at="../ARCHITECTURE_GPU_PREFILL_GEMM.md#prefill-split-k")]
def private bf16_split_gate(t : T?; var dev, queue; m, kdim, ndim, nsk : int; halfx : bool) {
    let tag = "bf16_split{halfx ? " halfx" : ""} m={m} k={kdim} n={ndim} nsk={nsk}"
    with_metal_cell(dev, queue) $(var mc : MetalCell) {
        var err : string
        let kt = halfx ? kernel_of(@@pf_enc_bf16_mm_thsk_c) : kernel_of(@@pf_enc_bf16_mm_tsk_c)
        var pso = mc |> keep_stamp(kt, err)
        return if (!tensor_pso_or_skip(t, tag, pso, dev, err))
        var inscope wb : array<uint16>
        wb |> resize(ndim * kdim)
        var inscope wdeq : array<float>
        wdeq |> resize(ndim * kdim)
        for (i in range(ndim * kdim)) {
            wb[i] = uint16(float_bits_to_uint(0.125 * float((i * 3) % 13 - 6)) >> 16u)
            wdeq[i] = uint_bits_to_float(uint(wb[i]) << 16u)
        }
        var inscope xf <- [for (i in range(m * kdim)); float(float16(0.11 * float(i % 37) - 1.9))]   //! f16-exact: one oracle for both X forms
        var inscope xh <- [for (x in xf); float16(x)]
        var inscope ref <- dense_ref_rows(wdeq, xf, m, kdim, ndim)
        let bw = mc |> in_plain(wb)
        let bx = halfx ? (mc |> in_plain(xh)) : (mc |> in_(xf))
        split_cell(t, mc, tag, m, kdim, ndim, nsk, false, ref.want, ref.env, staged_bar(0, kdim, ref.env)) $(enc; bo, bk, bn) {
            invoke(kt.enc, enc, pso, mc.buf[bw], 0ul, mc.buf[bx], 0ul, bo, 0ul, bk, bn, int64(m), int64(ndim), int64(nsk))
        }
    }
}

[arch(at="../ARCHITECTURE_IMAGE_GPU_PLANES.md#image-devw-plane")]
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

//! The prefill router's tile (`pf_enc_router_mm`): `MetalF32ExactMm` over f32 router rows and f32 routed rows, off the f16
//! lattice, at the f32 bar against the dot in double - the router rows' own bytes, no half rounding of either operand. The
//! half-staged f32 stamp over the same operands is the control that must miss the bar (a half panel of the plane lands there).
def private router_f32_gate(t : T?; var dev, queue; m, mp, kdim, ne : int) {
    let tag = "router f32 tile m={m}/{mp} k={kdim} ne={ne}"
    with_metal_cell(dev, queue) $(var mc : MetalCell) {
        var err : string
        let sx = stamp_of(@@enc_f32_exact_mm)
        let sh = stamp_of(@@enc_f32_mm)
        var pso_x = mc |> keep_stamp(sx, err)
        return if (!pso_ok(t, tag, pso_x, err))
        var pso_h = mc |> keep_stamp(sh, err)
        return if (!pso_ok(t, tag, pso_h, err))
        var inscope w : array<float>
        w |> resize(ne * kdim)
        var inscope xf : array<float>
        xf |> resize(mp * kdim)
        for (i in range(ne * kdim)) {
            w[i] = 0.0371 * float((i * 7 + 3) % 29) - 0.517 + 1.3e-4 * float(i % 11)
        }
        for (i in range(m * kdim)) {
            xf[i] = 0.093 * float((i * 5 + 1) % 41) - 1.87 + 2.1e-4 * float(i % 13)
        }
        var inscope ref <- strided_ref(dense_ref_rows(w, xf, m, kdim, ne), ne, ne, mp, 0.0)
        let bw = mc |> in_(w)
        let bx = mc |> in_(xf)
        let by = mc |> out_words(mp * ne, -1000.0)
        let byh = mc |> out_words(mp * ne, -1000.0)
        let bk = mc |> in_plain([uint(kdim)])
        let bn = mc |> in_plain([uint(ne)])
        let ran = mc |> cell_record(t, tag) $(enc : MetalComputeEncoder?) {
            stamp_run(sx, pso_x) {
                invoke(sx.enc, enc, mc.buf[bw], 0ul, mc.buf[bx], mc.buf[by], 0ul, mc.buf[bk], mc.buf[bn], int64(mp), int64(ne))
            }
            stamp_run(sh, pso_h) {
                invoke(sh.enc, enc, mc.buf[bw], 0ul, mc.buf[bx], mc.buf[byh], 0ul, mc.buf[bk], mc.buf[bn], int64(mp), int64(ne))
            }
        }
        return if (!ran)
        let bar = Bar(kind = BarKind.envelope, rel = 2e-6, abs_floor = 1e-5, poison = ref.env[0] * 0.1 + 1.0, poison_at = 0)
        check_buf(t, tag, mc.buf[by], ref.want, bar, Shape(row_width = ne), ref.env)
        var inscope gh <- plane_run(mc, byh)
        let miss = check_count(gh, ref.want, bar, ref.env)
        t |> success(miss > 0, "{tag}: the half-staged stamp over the same f32 rows misses the f32 bar ({miss} elements outside)")
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
        var kaR = SkReduceArgs(total = uint(mp * ndim / 4), ksplit = uint(ksplit))
        let bw = mc |> in_plain(blob)
        let bx = mc |> in_(xf)
        let bpart = mc |> out_words(ksplit * mp * ndim, -1000.0)
        let by = mc |> out_words(mp * ndim, -1000.0)
        let ran = mc |> cell_record(t, tag) $(enc : MetalComputeEncoder?) {
            stamp_run(st, pso) {   // one blob, its byte and half-scale views
                invoke(st.enc, enc, mc.buf[bw], 0ul, mc.buf[bw], mc.buf[bx], mc.buf[bpart], 0ul, ka, int64(ndim), int64(ksplit))
            }
            stamp_run(sr, psoR) {
                invoke(sr.enc, enc, mc.buf[bpart], mc.buf[by], 0ul, kaR, int64(mp * ndim / 4))
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

//! the tensor twins' truth over the padded panel: a 32-row tile runs its op at the rows its expert leaves it (8, 16 or 32,
//! `tile_rows`), so the rows past that height stay what the buffer held - the cell's sentinel - where the fixed op wrote them
def private adapt_pad_truth(fx : MoeFix; ndim : int; sentinel : float; var want, env : array<float>) {
    adapt_pad_rows(fx.cnt, fx.basep, ndim, sentinel, want, env)
}

//! the routed simdgroup base form of every superblock format (MetalMoeMulMmKqT) against the shipped plane-row dequant, bound
//! at the MetalMoeMulMmBase numbers (0..2 weight views, x 3, y 4, cnt/base/bkt 7..9; a split-scale format's 0 the f16 d-tail
//! view); the tensor twins ride moe_mulmm_split_gate
def private moe_mulmm_kq_gate(t : T?; var dev, queue; fmt : int; kdim, ndim : int; counts : array<int>; gather : bool) {
    let tag = "moe_mulmm_k{fmt}{gather ? " gather" : ""} k={kdim} n={ndim}"
    with_metal_cell(dev, queue) $(var mc : MetalCell) {
        var err : string
        let f = kq_fmt_of_id(fmt, "moe_mulmm_kq_gate")
        let kb = moe_kq_base(f)
        var pso = mc |> keep_stamp(kb, err)
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
            invoke(kb.enc, enc, pso, mc.buf[bks], doff, mc.buf[bks], 0ul, mc.buf[bkq], 0ul, mc.buf[bx], mc.buf[by], ka, mc.buf[bcnt], mc.buf[bbase], mc.buf[bbkt], int64(ndim), int64(fx.maxtiles * 32), int64(ne))
        }
        if (ran) {
            check_buf(t, tag, mc.buf[by], want, env_bar(2e-4, env), Shape(row_width = ndim), env)
        }
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

//! the routed simdgroup base form of a superblock format
def private moe_kq_base(f) {   //! generic, as the gates are: the Apple half alone instantiates it
    if (f == KqFmt.k4) return kernel_of(@@pf_enc_moe_mm_k4_c)
    if (f == KqFmt.k5) return kernel_of(@@pf_enc_moe_mm_k5_c)
    if (f == KqFmt.k6) return kernel_of(@@pf_enc_moe_mm_k6_c)
    if (f == KqFmt.k3) return kernel_of(@@pf_enc_moe_mm_k3_c)
    if (f == KqFmt.k2) return kernel_of(@@pf_enc_moe_mm_k2_c)
    if (f == KqFmt.iq4xs) return kernel_of(@@pf_enc_moe_mm_iq4xs_c)
    if (f == KqFmt.iq3s4) return kernel_of(@@pf_enc_moe_mm_iq3s4_c)
    if (f == KqFmt.iq4nl) return kernel_of(@@pf_enc_moe_mm_iq4nl_c)
    if (f == KqFmt.q40) return kernel_of(@@pf_enc_moe_mm_q40_c)
    if (f == KqFmt.nv4) return kernel_of(@@pf_enc_moe_mm_nv4_c)
    if (f == KqFmt.iq3s) return kernel_of(@@pf_enc_moe_mm_iq3s_c)
    if (f == KqFmt.iq3xxs) return kernel_of(@@pf_enc_moe_mm_iq3xxs_c)
    if (f == KqFmt.iq2s) return kernel_of(@@pf_enc_moe_mm_iq2s_c)
    if (f == KqFmt.iq2xs) return kernel_of(@@pf_enc_moe_mm_iq2xs_c)
    if (f != KqFmt.iq2xxs) {
        panic("moe_kq_base: no routed base form for {f}")
    }
    return kernel_of(@@pf_enc_moe_mm_iq2xxs_c)
}

//! the split family's expert tensor twins per format and form (th, th128, thr): every superblock format rides its dense
//! split class under the MoE shell
def private moe_split_twin(f; form : MoeSplitForm) {   //! generic, as the gates are: the Apple half alone instantiates it
    if (f == KqFmt.k3) return moe_form(form, kernel_of(@@pf_enc_moe_mm_k3_th_c), kernel_of(@@pf_enc_moe_mm_k3_th128_c), kernel_of(@@pf_enc_moe_mm_k3_thr_c))
    if (f == KqFmt.k4) return moe_form(form, kernel_of(@@pf_enc_moe_mm_k4_th_c), kernel_of(@@pf_enc_moe_mm_k4_th128_c), kernel_of(@@pf_enc_moe_mm_k4_thr_c))
    if (f == KqFmt.k5) return moe_form(form, kernel_of(@@pf_enc_moe_mm_k5_th_c), kernel_of(@@pf_enc_moe_mm_k5_th128_c), kernel_of(@@pf_enc_moe_mm_k5_thr_c))
    if (f == KqFmt.k6) return moe_form(form, kernel_of(@@pf_enc_moe_mm_k6_th_c), kernel_of(@@pf_enc_moe_mm_k6_th128_c), kernel_of(@@pf_enc_moe_mm_k6_thr_c))
    if (f == KqFmt.k2) return moe_form(form, kernel_of(@@pf_enc_moe_mm_k2_th_c), kernel_of(@@pf_enc_moe_mm_k2_th128_c), kernel_of(@@pf_enc_moe_mm_k2_thr_c))
    if (f == KqFmt.iq4nl) return moe_form(form, kernel_of(@@pf_enc_moe_mm_iq4nl_th_c), kernel_of(@@pf_enc_moe_mm_iq4nl_th128_c), kernel_of(@@pf_enc_moe_mm_iq4nl_thr_c))
    if (f == KqFmt.iq4xs) return moe_form(form, kernel_of(@@pf_enc_moe_mm_iq4xs_th_c), kernel_of(@@pf_enc_moe_mm_iq4xs_th128_c), kernel_of(@@pf_enc_moe_mm_iq4xs_thr_c))
    if (f == KqFmt.iq3s4) return moe_form(form, kernel_of(@@pf_enc_moe_mm_iq3s4_th_c), kernel_of(@@pf_enc_moe_mm_iq3s4_th128_c), kernel_of(@@pf_enc_moe_mm_iq3s4_thr_c))
    if (f == KqFmt.q40) return moe_form(form, kernel_of(@@pf_enc_moe_mm_q40_th_c), kernel_of(@@pf_enc_moe_mm_q40_th128_c), kernel_of(@@pf_enc_moe_mm_q40_thr_c))
    if (f == KqFmt.nv4) return moe_form(form, kernel_of(@@pf_enc_moe_mm_nv4_th_c), kernel_of(@@pf_enc_moe_mm_nv4_th128_c), kernel_of(@@pf_enc_moe_mm_nv4_thr_c))
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
                               bks, bkq, bxh, by, bk, bn, bcnt, bbase, bgact, bhout, bgu; nsb : int; mp, tiles, ndim, ne : int) {
    invoke(k.enc, enc, pso, bks, kq_split_doff(fmt, nsb), bks, 0ul, bkq, 0ul, bxh, 0ul, by, 0ul, bk, bn, bcnt, bbase, bgact, bhout, bgu,
        int64(mp), int64(ndim), int64(ne), int64(tiles))
}

//! the gated store's gate rows: a fixed pattern inside [-0.08, 0.06], where silu is a twentieth off its linear part (so a
//! store that forgot the gate's curve misses the bar) and the product of the fixtures' largest sums still fits a half
def private gated_gate(n : int) : array<float> => [for (i in range(n)); float((i * 7 + 3) % 8) * 0.02 - 0.08]

//! the gated store's truth over the plain store's: silu(gate) * y as halves where the plain run wrote y, the half sentinel
//! where `want` holds the NaN mark of a row the tile leaves (a value no sum lands on, where a finite sentinel can); its
//! envelope is the f32 truth's through the gate's value plus the half rounding (2.5 x the magnitude at the bar's 2e-4)
def private gated_truth(want, env, gate : array<float>; hsentinel : float; var want_h, env_h : array<float>) {
    want_h |> resize(length(want))
    env_h |> resize(length(want))
    for (w, e, g, wh, eh in want, env, gate, want_h, env_h) {
        if (w != w) {   // nolint:LINT007 — the NaN mark
            wh = hsentinel
            eh = 0.0
        } else {
            let s = g / (1.0 + exp(-g))
            wh = float(float16(s * w))
            eh = e * abs(s) + abs(wh) * 2.5 + 1.0
        }
    }
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
        let bg0 = mc |> in_plain([0u])
        let bg1 = mc |> in_plain([1u])
        let ran = mc |> cell_record(t, tag) $(enc : MetalComputeEncoder?) {
            moe_split_dispatch(enc, fmt, th, tw_pso, mc.buf[bks], mc.buf[bkq], mc.buf[bxh], mc.buf[by], mc.buf[bk], mc.buf[bn], mc.buf[bcnt], mc.buf[bbase],
                mc.buf[by], mc.buf[bxh], mc.buf[bg0], nsb, fx.pad, fx.maxtiles, ndim, ne)
        }
        var inscope want_t := want
        var inscope env_t := env
        adapt_pad_truth(fx, ndim, -500.0, want_t, env_t)
        if (ran) {
            check_buf(t, tag, mc.buf[by], want_t, env_bar(2e-4, env_t), Shape(row_width = ndim), env_t)
        }
        var inscope gate <- gated_gate(fx.pad * ndim)
        let bgate = mc |> in_plain(gate)
        let bh = mc |> out_words(fx.pad * ndim, float16(-1000.0))
        let rang = mc |> cell_record(t, "{tag} gated store") $(enc : MetalComputeEncoder?) {
            moe_split_dispatch(enc, fmt, th, tw_pso, mc.buf[bks], mc.buf[bkq], mc.buf[bxh], mc.buf[by], mc.buf[bk], mc.buf[bn], mc.buf[bcnt], mc.buf[bbase],
                mc.buf[bgate], mc.buf[bh], mc.buf[bg1], nsb, fx.pad, fx.maxtiles, ndim, ne)
        }
        if (rang) {
            var inscope want_m := want
            var inscope env_m := env
            adapt_pad_truth(fx, ndim, uint_bits_to_float(0x7fc00000u), want_m, env_m)
            var inscope want_h, env_h : array<float>
            gated_truth(want_m, env_m, gate, -1000.0, want_h, env_h)
            check_buf_h(t, "{tag} gated store", mc.buf[bh], want_h, env_bar(2e-4, env_h), Shape(row_width = ndim), env_h)
            check_buf(t, "{tag} gated store leaves y", mc.buf[by], want_t, env_bar(2e-4, env_t), Shape(row_width = ndim), env_t)
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
            moe_split_dispatch(enc, fmt, tl, tl_pso, mc.buf[bks], mc.buf[bkq], mc.buf[bxh], mc.buf[by_p], mc.buf[bk], mc.buf[bn], mc.buf[bcnt], mc.buf[bbase],
                mc.buf[by_p], mc.buf[bxh], mc.buf[bg0], nsb, fx.pad, (maxce + 127) / 128, ndim, ne)
            moe_split_dispatch(enc, fmt, rm, rm_pso, mc.buf[bks], mc.buf[bkq], mc.buf[bxh], mc.buf[by_p], mc.buf[bk], mc.buf[bn], mc.buf[bcnt], mc.buf[bbase],
                mc.buf[by_p], mc.buf[bxh], mc.buf[bg0], nsb, fx.pad, 4, ndim, ne)
        }
        return if (!ranp)
        var inscope want_p := want
        var inscope env_p := env
        adapt_pad_truth(fx, ndim, -700.0, want_p, env_p)
        check_buf(t, "{tag} tall pair", mc.buf[by_p], want_p, env_bar(2e-4, env_p), Shape(row_width = ndim), env_p)
        let bh_p = mc |> out_words(fx.pad * ndim, float16(-1000.0))
        let rangp = mc |> cell_record(t, "{tag} tall pair gated store") $(enc : MetalComputeEncoder?) {
            moe_split_dispatch(enc, fmt, tl, tl_pso, mc.buf[bks], mc.buf[bkq], mc.buf[bxh], mc.buf[by_p], mc.buf[bk], mc.buf[bn], mc.buf[bcnt], mc.buf[bbase],
                mc.buf[bgate], mc.buf[bh_p], mc.buf[bg1], nsb, fx.pad, (maxce + 127) / 128, ndim, ne)
            moe_split_dispatch(enc, fmt, rm, rm_pso, mc.buf[bks], mc.buf[bkq], mc.buf[bxh], mc.buf[by_p], mc.buf[bk], mc.buf[bn], mc.buf[bcnt], mc.buf[bbase],
                mc.buf[bgate], mc.buf[bh_p], mc.buf[bg1], nsb, fx.pad, 4, ndim, ne)
        }
        return if (!rangp)
        var inscope want_mp := want
        var inscope env_mp := env
        adapt_pad_truth(fx, ndim, uint_bits_to_float(0x7fc00000u), want_mp, env_mp)
        var inscope want_hp, env_hp : array<float>
        gated_truth(want_mp, env_mp, gate, -1000.0, want_hp, env_hp)
        check_buf_h(t, "{tag} tall pair gated store", mc.buf[bh_p], want_hp, env_bar(2e-4, env_hp), Shape(row_width = ndim), env_hp)
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
            let d = e8m0_half_f32(sc)
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
                enc_qk_dev128_c_pso(enc, pso, mc.buf[bq], 0ul, mc.buf[bk], uint64(klead * kvd * 2), mc.buf[batt], ka, int64(mp), int64(np32), int64(heads))
            } elif (pair) {
                enc_qk_dev_c_pso(enc, pso, mc.buf[bq], 0ul, mc.buf[bk], uint64(klead * kvd * 2), mc.buf[batt], ka, int64(mp), int64(np32), int64(heads))
            } elif (halfq) {
                enc_qk_mm_th_c_pso(enc, pso, mc.buf[bq], 0ul, mc.buf[bk], mc.buf[batt], ka, int64(mp), int64(np32), int64(heads))
            } elif (tensor) {
                enc_qk_mm_t_c_pso(enc, pso, mc.buf[bq], 0ul, mc.buf[bk], mc.buf[batt], ka, int64(mp), int64(np32), int64(heads))
            } else {
                enc_qk_mm_c_pso(enc, pso, mc.buf[bq], 0ul, mc.buf[bk], mc.buf[batt], ka, int64(mp), int64(np32), int64(heads))
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
            invoke(kav.enc, enc, pso, mc.buf[batt], mc.buf[bv], mc.buf[bxb], 0ul, ka, mc.buf[bstat], int64(mp), int64(heads), int64(hs))
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
            var ka = GkClampArgs(nelem = uint(total), lo = -65504.0, hi = 65504.0, scale = scale, hoff = uint(lead))
            for (leg in range(2)) {
                if (leg == 1) {
                    mc |> plane_upload(bx, poisoned_copy(xv, 300, 2.0))
                    mc |> plane_refill(bh)   // the sentinel back before the run that must write the plane anew
                }
                let ran = mc |> cell_record(t, tag) $(enc : MetalComputeEncoder?) {
                    invoke(kh.enc, enc, pso, mc.buf[bx], mc.buf[bh], ka, int64((total + slack + 3) / 4))
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
            invoke(kav.enc, enc, pso_av, mc.buf[batt], mc.buf[bv], uint64(vlead * kvd * 2), mc.buf[bxb], 0ul, ka, mc.buf[bstat], int64(mp), int64(heads), int64(hs / 64))
            if (tail) {
                invoke(kavt.enc, enc, pso_avt, mc.buf[batt], mc.buf[bv], uint64(vlead * kvd * 2), mc.buf[bxb], 0ul, ka, mc.buf[bstat], int64(mp), int64(heads), int64(hs))
            }
        }
        return if (!ran)
        check_buf(t, tag, mc.buf[bxb], want, env_bar(2e-3, env), Shape(row_width = qd, head_width = hs), env)
    }
}

// the stage width is a stamp constant the emitter's census records: the double-buffered stamps
// read the 128-wide body, the single-tile stamps the 64-wide one - device-free
[test(description = "The emitted census of the four q8 mul_mm tensor stamps records the stage width their constants set: the single-tile TH and TH128 stamps call the k64 body, the double-buffered THDb pair the k128 one.")]
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


//! the reference a rows8 cell checks y against: `ref`'s `live` token rows at the cell's row stride `ys`, the gap past each row's
//! `ndim` the y fill's sentinel
def private rows8_y_ref(ref : DenseRef; ndim, ys, live : int) : DenseRef => <- strided_ref(ref, ndim, ys, live, -1000.0)

//! the k4 rows8 kernel over an eight-row panel: the x-table prep, then the kernel writing y at row stride `ys` (the site's rows, or
//! wider: the gap past each row stays the fill), against the dense reference over the f16-rounded panel; skips where the box's
//! layout probe says the form is another generation's
def private rows8_k4_gate(t : T?; var dev, queue; kdim, ndim, ys : int) {
    let tag = "rows8 k4 k={kdim} n={ndim} ys={ys}"
    with_metal_cell(dev, queue) $(var mc : MetalCell) {
        var err : string
        let kp = kernel_of(@@enc_rows8_prep_c)
        let kg = kernel_of(@@enc_rows8_k4_c)
        var ppso = mc |> keep_stamp(kp, err)
        var gpso = mc |> keep_stamp(kg, err)
        return if (!tensor_pso_or_skip(t, tag, gpso, dev, err) || !pso_ok(t, tag, ppso, err))
        if (!rows8_probe_ok(t, mc, err)) {
            t |> skip("{tag}: the matrix unit's cooperative layout is not the rows8 kernels' on this box - the engine keeps the form off")
            return
        }
        let nb = kdim / 256
        let nsb = ndim * nb
        var inscope kq, ks : array<uint8>
        kq_fill_planes(4, nsb, kq, ks)
        var inscope xf : array<float>
        xf |> resize(8 * kdim)
        for (i in range(8 * kdim)) {
            xf[i] = float(float16(0.07 * float(i % 39) - 1.3))   //! the kernel reads the panel as halves: the oracle's panel is the same lattice
        }
        var inscope ref <- dense_ref_env(8, kdim, ndim, xf) $(j; var wrow) {
            kq_row_ref(4, j * nb, nb, nsb, kq, ks, wrow)
        }
        let bks = mc |> in_plain(ks)
        let bkq = mc |> in_plain(kq)
        let bx = mc |> in_(xf)
        let bxt = mc |> out_words(8 * kdim, float16(0.0))
        let bxs = mc |> out_words(kdim / 32 * 8, 0.0)
        let by = mc |> out_words(8 * ys, -1000.0)
        let bk = mc |> in_plain([uint(kdim)])
        let bys = mc |> in_plain([uint(ys)])
        let ran = mc |> cell_record(t, tag) $(enc : MetalComputeEncoder?) {
            invoke(kp.enc, enc, ppso, mc.buf[bx], 0ul, mc.buf[bxt], mc.buf[bxs], mc.buf[bk], int64(kdim / 32 * 8))
            invoke(kg.enc, enc, gpso, mc.buf[bks], 0ul, mc.buf[bkq], 0ul, mc.buf[bxt], mc.buf[by], 0ul, mc.buf[bxs], mc.buf[bk], mc.buf[bys], int64(ndim), int64(7 * ys + ndim))
        }
        return if (!ran)
        var inscope yref <- rows8_y_ref(ref, ndim, ys, 8)
        check_buf(t, tag, mc.buf[by], yref.want, env_bar(2e-4, yref.env), Shape(row_width = ys), yref.env)
    }
}

//! the q8 rows8 kernel over an eight-row panel, the same shape over the 34-byte blob; at `ndim` 96 a plane of six threadgroups,
//! off every 64-row grid
def private rows8_q8_gate(t : T?; var dev, queue; kdim, ndim, ys : int) {
    let tag = "rows8 q8 k={kdim} n={ndim} ys={ys}"
    with_metal_cell(dev, queue) $(var mc : MetalCell) {
        var err : string
        let kp = kernel_of(@@enc_rows8_prep_c)
        let kg = kernel_of(@@enc_rows8_q8_c)
        var ppso = mc |> keep_stamp(kp, err)
        var gpso = mc |> keep_stamp(kg, err)
        return if (!tensor_pso_or_skip(t, tag, gpso, dev, err) || !pso_ok(t, tag, ppso, err))
        if (!rows8_probe_ok(t, mc, err)) {
            t |> skip("{tag}: the matrix unit's cooperative layout is not the rows8 kernels' on this box - the engine keeps the form off")
            return
        }
        let nkb = kdim / 32
        var inscope blob : array<uint8>
        var inscope wdeq : array<float>
        q8_blob_fill(ndim, nkb, blob, wdeq)
        var inscope xf : array<float>
        q8_x_fill(8 * kdim, xf)
        for (v in xf) {
            v = float(float16(v))
        }
        var inscope ref <- dense_ref_rows(wdeq, xf, 8, kdim, ndim)
        let bw = mc |> in_plain(blob)
        let bx = mc |> in_(xf)
        let bxt = mc |> out_words(8 * kdim, float16(0.0))
        let bxs = mc |> out_words(kdim / 32 * 8, 0.0)
        let by = mc |> out_words(8 * ys, -1000.0)
        let bk = mc |> in_plain([uint(kdim)])
        let bys = mc |> in_plain([uint(ys)])
        let ran = mc |> cell_record(t, tag) $(enc : MetalComputeEncoder?) {
            invoke(kp.enc, enc, ppso, mc.buf[bx], 0ul, mc.buf[bxt], mc.buf[bxs], mc.buf[bk], int64(kdim / 32 * 8))
            invoke(kg.enc, enc, gpso, mc.buf[bw], 0ul, mc.buf[bw], mc.buf[bxt], mc.buf[by], 0ul, mc.buf[bxs], mc.buf[bk], mc.buf[bys], int64(ndim), int64(7 * ys + ndim))
        }
        return if (!ran)
        var inscope yref <- rows8_y_ref(ref, ndim, ys, 8)
        check_buf(t, tag, mc.buf[by], yref.want, env_bar(2e-3, yref.env), Shape(row_width = ys), yref.env)
    }
}

//! a split-plane format's rows8 kernel: k6's (kq 6) or iq4xs's (kq 44); `fmt` untyped: the stamps sit behind the optional das_metal require
def private rows8_split_kernel(fmt) {
    return kernel_of(@@enc_rows8_k6_c) if (fmt == 6)
    return kernel_of(@@enc_rows8_iq4xs_c)
}

//! A split-plane format's rows8 kernel (k6, kq 6; iq4xs, kq 44) over an eight-row panel writing y at row stride `ys`, against the
//! dense reference: `fed` off, the half conversion of the f32 panel (`enc_half_rows`, the site's own prep) feeds it, the reference
//! over the f16-rounded panel; `fed` on, the norm's f16 twin of `nrows` live rows (`enc_rms_bhx`, the pad rows NaN) feeds it, the
//! live rows against the reference over the twin's halves. The fixture's int8 sub-scales take both signs and differ block to block,
//! asserted. At `ndim` 96 a plane of six threadgroups, off every 64-row grid.
def private rows8_split_gate(t : T?; var dev, queue; fmt, kdim, ndim, ys : int; fed : bool; nrows : int = 8) {
    let feed = fed ? "twin-fed rows={nrows}" : "prep"
    let tag = "rows8 kq{fmt} k={kdim} n={ndim} ys={ys} {feed}"
    with_metal_cell(dev, queue) $(var mc : MetalCell) {
        var err : string
        let kh = kernel_of(@@enc_half_rows)
        let kn = stamp_of(@@enc_rms_bhx)
        let kg = rows8_split_kernel(fmt)
        var hpso = mc |> keep_stamp(kh, err)
        var npso = mc |> keep_stamp(kn, err)
        var gpso = mc |> keep_stamp(kg, err)
        return if (!tensor_pso_or_skip(t, tag, gpso, dev, err) || !pso_ok(t, tag, hpso, err) || !pso_ok(t, tag, npso, err))
        if (!rows8_probe_ok(t, mc, err)) {
            t |> skip("{tag}: the matrix unit's cooperative layout is not the rows8 kernels' on this box - the engine keeps the form off")
            return
        }
        let nb = kdim / 256
        let nsb = ndim * nb
        var inscope kq, ks : array<uint8>
        kq_fill_planes(fmt, nsb, kq, ks)
        let nsc = fmt == 6 ? 16 : 8   //! the strip's live sub-scales: k6 sixteen, iq4xs eight
        var neg = 0
        var blocks_differ = false
        for (b in range(nsb)) {
            for (s in range(nsc)) {
                let i = b * 16 + s
                neg += int(ks[i]) >= 128 ? 1 : 0
                blocks_differ = blocks_differ || (b > 0 && ks[i] != ks[i - 16])
            }
        }
        t |> success(neg > 0 && neg < nsb * nsc && blocks_differ, "{tag}: the sub-scales take both signs ({neg} of {nsb * nsc} negative) and differ block to block")
        let live = fed ? nrows : 8
        let nan = uint_bits_to_float(0x7FC00000u)
        var inscope xf, w : array<float>
        xf |> resize(8 * kdim)
        w |> resize(kdim)
        for (i in range(8 * kdim)) {
            xf[i] = float(float16(0.07 * float(i % 39) - 1.3))   //! the prep reads the panel as halves: the oracle's panel is the same lattice
        }
        for (i in range(kdim)) {
            w[i] = 0.5 + 0.04 * float(i % 13)
        }
        let bks = mc |> in_plain(ks)
        let bkq = mc |> in_plain(kq)
        let bx = mc |> in_(xf)
        let bw = mc |> in_(w)
        let by0 = mc |> out_words(8 * kdim, -1000.0)
        let bxh = mc |> out_words(8 * kdim, float16(nan))
        let by = mc |> out_words(8 * ys, -1000.0)
        let bk = mc |> in_plain([uint(kdim)])
        let bys = mc |> in_plain([uint(ys)])
        let beps = mc |> in_plain([1.0e-5])
        let doff = kq_split_doff(fmt, nsb)
        var ha = GkClampArgs(nelem = uint(8 * kdim), lo = -65504.0, hi = 65504.0, scale = 1.0, hoff = 0u)
        let ran = mc |> cell_record(t, tag) $(enc : MetalComputeEncoder?) {
            if (fed) {
                stamp_run(kn, npso) {
                    invoke(kn.enc, enc, mc.buf[bx], 0ul, mc.buf[bw], mc.buf[by0], mc.buf[bk], mc.buf[beps], mc.buf[bxh], int64(nrows))
                }
            } else {
                invoke(kh.enc, enc, hpso, mc.buf[bx], mc.buf[bxh], ha, int64(8 * kdim / 4))
            }
            invoke(kg.enc, enc, gpso, mc.buf[bks], doff, mc.buf[bks], 0ul, mc.buf[bxh], mc.buf[by], 0ul, mc.buf[bkq], 0ul, mc.buf[bk], mc.buf[bys], int64(ndim), int64(7 * ys + ndim))
        }
        return if (!ran)
        var inscope halves <- buf_download(mc.buf[bxh], live * kdim, type<float16>)
        var inscope xr <- [for (i in range(live * kdim)); fed ? float(halves[i]) : xf[i]]
        var inscope ref <- dense_ref_env(live, kdim, ndim, xr) $(j; var wrow) {
            kq_row_ref(fmt, j * nb, nb, nsb, kq, ks, wrow)
        }
        var inscope yref <- rows8_y_ref(ref, ndim, ys, live)
        check_buf(t, tag, mc.buf[by], yref.want, env_bar(2e-4, yref.env), Shape(row_width = ys), yref.env)
    }
}

//! the producers whose table mode writes the rows8 x table and token sums beside the f16 twin (`XT`)
enum private TabProducer {
    rms           //! the norm into bxb (`enc_rms_bhxt`)
    add_rms       //! the residual step's norm, 256 lanes
    add_rms_wide  //! ... its 1024-lane builders past the slab
    pre           //! the sandwich seam's norm, 256 lanes
    pre_wide      //! ... its 1024-lane builders
    swiglu        //! the gate panel's activation
    geglu
}

//! The rows8 x table and token sums a twin's `nrows` live rows of `width` (whole 32-blocks) make, in the prep's own slot law: slot
//! 4 kq + i of a 32-block from physical 8 kq + i, slot 16 + 4 kq + i from 8 kq + 4 + i; each block's sum of its halves; the rows
//! past `nrows` of the eight the sentinels
struct private Rows8Table {
    xt : array<float>   //! [8][width] halves widened, in k-slot order
    xs : array<float>   //! [width / 32][8]
}

def private rows8_table_of(h : array<float16>; nrows, width : int; tsent, ssent : float) : Rows8Table {
    var out : Rows8Table
    out.xt |> resize(8 * width)
    out.xs |> resize(width / 32 * 8)
    for (r in range(8)) {
        for (b in range(width / 32)) {
            var sum = 0.0lf
            for (s in range(32)) {
                let kq = (s & 15) >> 2
                let i = s & 3
                let phys = s < 16 ? 8 * kq + i : 8 * kq + 4 + i
                let at = r * width + b * 32
                if (r < nrows) {
                    out.xt[at + s] = float(h[at + phys])
                    sum += double(float(h[at + phys]))
                } else {
                    out.xt[at + s] = tsent
                }
            }
            out.xs[b * 8 + r] = r < nrows ? float(sum) : ssent
        }
    }
    return <- out
}

//! A producer's table mode against its HX stamp on the same inputs: the f32 rows and the twin bit for bit the HX stamp's, the
//! table the twin's halves at their k-slots bit for bit, the sums within 1e-5 of the double sum of each block's halves, the pad
//! rows past `nrows` untouched; the table read as the plain twin is the control the slot order must miss
def private rows8_table_gate(t : T?; var dev, queue; p : TabProducer; width, nrows : int) {
    let tag = "rows8 table {p} width={width} rows={nrows}"
    with_metal_cell(dev, queue) $(var mc : MetalCell) {
        var err : string
        let rh = stamp_of(@@enc_rms_bhx)
        let rt = stamp_of(@@enc_rms_bhxt)
        let ah = stamp_of(@@enc_add_rms_bhx_c)
        let at = stamp_of(@@enc_add_rms_bhxt_c)
        let awh = stamp_of(@@enc_add_rms_bhxw_c)
        let awt = stamp_of(@@enc_add_rms_bhxtw_c)
        let ph = stamp_of(@@enc_preaddrms_bhx_c)
        let pt = stamp_of(@@enc_preaddrms_bhxt_c)
        let pwh = stamp_of(@@enc_preaddrms_bhxw_c)
        let pwt = stamp_of(@@enc_preaddrms_bhxtw_c)
        let sh = stamp_of(@@enc_swiglu_hx)
        let st = stamp_of(@@enc_swiglu_hxt)
        let gh = stamp_of(@@enc_geglu_hx)
        let gt = stamp_of(@@enc_geglu_hxt)
        var hpso, tpso : MetalComputePipeline?
        if (p == TabProducer.rms) {
            hpso = mc |> keep_stamp(rh, err)
            tpso = mc |> keep_stamp(rt, err)
        } elif (p == TabProducer.add_rms) {
            hpso = mc |> keep_stamp(ah, err)
            tpso = mc |> keep_stamp(at, err)
        } elif (p == TabProducer.add_rms_wide) {
            hpso = mc |> keep_stamp(awh, err)
            tpso = mc |> keep_stamp(awt, err)
        } elif (p == TabProducer.pre) {
            hpso = mc |> keep_stamp(ph, err)
            tpso = mc |> keep_stamp(pt, err)
        } elif (p == TabProducer.pre_wide) {
            hpso = mc |> keep_stamp(pwh, err)
            tpso = mc |> keep_stamp(pwt, err)
        } elif (p == TabProducer.swiglu) {
            hpso = mc |> keep_stamp(sh, err)
            tpso = mc |> keep_stamp(st, err)
        } else {
            hpso = mc |> keep_stamp(gh, err)
            tpso = mc |> keep_stamp(gt, err)
        }
        return if (!pso_ok(t, tag, hpso, err) || !pso_ok(t, tag, tpso, err))
        let total = nrows * width
        let sent = -1000.0
        var inscope xv, av, w : array<float>
        xv |> resize(total)
        av |> resize(total)
        w |> resize(2 * width)   //! [post norm | norm] on a sandwich stamp; the others read the first row
        for (i in range(total)) {
            xv[i] = 0.11 * float(i % 43) - 2.3
            av[i] = 0.07 * float(i % 31) - 1.1
        }
        for (i in range(2 * width)) {
            w[i] = 0.4 + 0.05 * float(i % 11)
        }
        let ew = p == TabProducer.swiglu || p == TabProducer.geglu
        let bx1 = mc |> io_(xv)
        let bx2 = mc |> io_(xv)
        let ba = mc |> in_(av)
        let bw = mc |> in_(w)
        let by1 = mc |> out_words(total, sent)
        let by2 = mc |> out_words(total, sent)
        let bh1 = mc |> out_words(total, float16(sent))
        let bh2 = mc |> out_words(total, float16(sent))
        let bxt = mc |> out_words(8 * width, float16(sent))
        let bxs = mc |> out_words(width / 32 * 8, sent)
        let bdim = mc |> in_plain([uint(width)])
        let beps = mc |> in_plain([1.0e-5])
        let btot = mc |> in_plain([uint(total)])
        var ka = GkAddRmsArgs(dim = uint(width), add_on = 1u, eps = 1.0e-5, ascale = 0.75)
        let ran = mc |> cell_record(t, tag) $(enc : MetalComputeEncoder?) {
            let (x1, x2, a, wn, y1, y2, h1, h2, yt, ys) = (mc.buf[bx1], mc.buf[bx2], mc.buf[ba], mc.buf[bw], mc.buf[by1], mc.buf[by2],
                mc.buf[bh1], mc.buf[bh2], mc.buf[bxt], mc.buf[bxs])
            let n = int64(nrows)
            if (p == TabProducer.rms) {
                stamp_run(rh, hpso) {
                    invoke(rh.enc, enc, x1, 0ul, wn, y1, mc.buf[bdim], mc.buf[beps], h1, n)
                }
                stamp_run(rt, tpso) {
                    invoke(rt.enc, enc, x2, 0ul, wn, y2, mc.buf[bdim], mc.buf[beps], h2, yt, ys, n)
                }
            } elif (p == TabProducer.add_rms) {
                stamp_run(ah, hpso) {
                    invoke(ah.enc, enc, x1, a, wn, y1, h1, ka, n)
                }
                stamp_run(at, tpso) {
                    invoke(at.enc, enc, x2, a, wn, y2, h2, yt, ys, ka, n)
                }
            } elif (p == TabProducer.add_rms_wide) {
                stamp_run(awh, hpso) {
                    invoke(awh.enc, enc, x1, a, wn, y1, h1, ka, n)
                }
                stamp_run(awt, tpso) {
                    invoke(awt.enc, enc, x2, a, wn, y2, h2, yt, ys, ka, n)
                }
            } elif (p == TabProducer.pre) {
                stamp_run(ph, hpso) {
                    invoke(ph.enc, enc, x1, a, wn, y1, h1, ka, n)
                }
                stamp_run(pt, tpso) {
                    invoke(pt.enc, enc, x2, a, wn, y2, h2, yt, ys, ka, n)
                }
            } elif (p == TabProducer.pre_wide) {
                stamp_run(pwh, hpso) {
                    invoke(pwh.enc, enc, x1, a, wn, y1, h1, ka, n)
                }
                stamp_run(pwt, tpso) {
                    invoke(pwt.enc, enc, x2, a, wn, y2, h2, yt, ys, ka, n)
                }
            } elif (p == TabProducer.swiglu) {
                stamp_run(sh, hpso) {
                    invoke(sh.enc, enc, x1, 0ul, a, 0ul, mc.buf[btot], h1, int64(total))
                }
                stamp_run(st, tpso) {
                    invoke(st.enc, enc, x2, 0ul, a, 0ul, mc.buf[btot], h2, mc.buf[bdim], yt, ys, int64(total))
                }
            } else {
                stamp_run(gh, hpso) {
                    invoke(gh.enc, enc, x1, 0ul, a, 0ul, mc.buf[btot], h1, int64(total))
                }
                stamp_run(gt, tpso) {
                    invoke(gt.enc, enc, x2, 0ul, a, 0ul, mc.buf[btot], h2, mc.buf[bdim], yt, ys, int64(total))
                }
            }
        }
        return if (!ran)
        var inscope x1v <- buf_download(mc.buf[bx1], total, type<float>)
        check_buf_exact(t, "{tag} x", mc.buf[bx2], x1v)
        if (!ew) {
            var inscope y1v <- buf_download(mc.buf[by1], total, type<float>)
            check_buf_exact(t, "{tag} y", mc.buf[by2], y1v)
        }
        var inscope h1v <- buf_download(mc.buf[bh1], total, type<float16>)
        var inscope h1f <- [for (v in h1v); float(v)]
        check_buf_h(t, "{tag} twin", mc.buf[bh2], h1f, BAR_EXACT, Shape(row_width = width))
        var inscope tab <- rows8_table_of(h1v, nrows, width, float(float16(sent)), sent)
        check_buf_h(t, "{tag} table", mc.buf[bxt], tab.xt, BAR_EXACT, Shape(row_width = width))
        check_buf(t, "{tag} sums", mc.buf[bxs], tab.xs, Bar(rel = 1e-5), Shape(row_width = 8))
        var inscope plain <- [for (k in range(8 * width)); k < total ? h1f[k] : float(float16(sent))]
        var inscope got <- buf_download(mc.buf[bxt], 8 * width, type<float16>)
        var inscope gotf <- [for (v in got); float(v)]
        t |> success(check_count(gotf, plain, BAR_EXACT) > 0, "{tag}: the table reads off the plain twin (the k-slot order)")
    }
}

//! the k4 rows8 kernel fed by a producer's table: the norm's table mode writes the table and sums of `nrows` live rows of eight
//! (the pad rows' table columns and sums NaN, as stale as a pool buffer may hold), the kernel reads them and writes y at row stride
//! `ys`; the live rows against the dense reference over the norm's own halves
def private rows8_k4_fed_gate(t : T?; var dev, queue; kdim, ndim, ys, nrows : int) {
    let tag = "rows8 k4 table-fed k={kdim} n={ndim} ys={ys} rows={nrows}"
    with_metal_cell(dev, queue) $(var mc : MetalCell) {
        var err : string
        let kn = stamp_of(@@enc_rms_bhxt)
        let kg = kernel_of(@@enc_rows8_k4_c)
        var npso = mc |> keep_stamp(kn, err)
        var gpso = mc |> keep_stamp(kg, err)
        return if (!tensor_pso_or_skip(t, tag, gpso, dev, err) || !pso_ok(t, tag, npso, err))
        if (!rows8_probe_ok(t, mc, err)) {
            t |> skip("{tag}: the matrix unit's cooperative layout is not the rows8 kernels' on this box - the engine keeps the form off")
            return
        }
        let nb = kdim / 256
        let nsb = ndim * nb
        var inscope kq, ks : array<uint8>
        kq_fill_planes(4, nsb, kq, ks)
        var inscope xv, w : array<float>
        xv |> resize(nrows * kdim)
        w |> resize(kdim)
        for (i in range(nrows * kdim)) {
            xv[i] = 0.07 * float(i % 39) - 1.3
        }
        for (i in range(kdim)) {
            w[i] = 0.5 + 0.04 * float(i % 13)
        }
        let nan = uint_bits_to_float(0x7FC00000u)
        let bks = mc |> in_plain(ks)
        let bkq = mc |> in_plain(kq)
        let bx = mc |> in_(xv)
        let bw = mc |> in_(w)
        let by0 = mc |> out_words(nrows * kdim, -1000.0)
        let bh = mc |> out_words(nrows * kdim, float16(-1000.0))
        let bxt = mc |> out_words(8 * kdim, float16(nan))
        let bxs = mc |> out_words(kdim / 32 * 8, nan)
        let by = mc |> out_words(8 * ys, -1000.0)
        let bk = mc |> in_plain([uint(kdim)])
        let bys = mc |> in_plain([uint(ys)])
        let beps = mc |> in_plain([1.0e-5])
        let ran = mc |> cell_record(t, tag) $(enc : MetalComputeEncoder?) {
            stamp_run(kn, npso) {
                invoke(kn.enc, enc, mc.buf[bx], 0ul, mc.buf[bw], mc.buf[by0], mc.buf[bk], mc.buf[beps], mc.buf[bh], mc.buf[bxt], mc.buf[bxs], int64(nrows))
            }
            invoke(kg.enc, enc, gpso, mc.buf[bks], 0ul, mc.buf[bkq], 0ul, mc.buf[bxt], mc.buf[by], 0ul, mc.buf[bxs], mc.buf[bk], mc.buf[bys], int64(ndim), int64(7 * ys + ndim))
        }
        return if (!ran)
        var inscope halves <- buf_download(mc.buf[bh], nrows * kdim, type<float16>)
        var inscope xh <- [for (v in halves); float(v)]
        var inscope ref <- dense_ref_env(nrows, kdim, ndim, xh) $(j; var wrow) {
            kq_row_ref(4, j * nb, nb, nsb, kq, ks, wrow)
        }
        var inscope yref <- rows8_y_ref(ref, ndim, ys, nrows)
        check_buf(t, tag, mc.buf[by], yref.want, env_bar(2e-4, yref.env), Shape(row_width = ys), yref.env)
    }
}

//! an affine member's rows8 kernel
def private rows8_aff_kernel(f) {   //! generic, as the gates are: the Apple half alone instantiates it
    return kernel_of(@@enc_rows8_mlx4_c) if (f == KqFmt.mlx4)
    return kernel_of(@@enc_rows8_mlx8_c) if (f == KqFmt.mlx8)
    return kernel_of(@@enc_rows8_mlx4b_c) if (f == KqFmt.mlx4b)
    return kernel_of(@@enc_rows8_mlx6b_c) if (f == KqFmt.mlx6b)
    return kernel_of(@@enc_rows8_mlx8b_c)
}

//! An affine member's rows8 kernel over an eight-row panel writing y at row stride `ys`, against the dense reference: `fed` off,
//! the x-table prep over the f16-lattice panel, the reference over that panel; `fed` on, the norm's table mode (`enc_rms_bhxt`)
//! over `nrows` live rows, the pad rows' table and sums NaN, the live rows against the reference over the norm's own halves. At
//! `ndim` 96 a plane of six threadgroups, off every 64-row grid.
def private rows8_aff_gate(t : T?; var dev, queue; fmt : KqFmt; kdim, ndim, ys : int; fed : bool; nrows : int = 8) {
    let feed = fed ? "table-fed rows={nrows}" : "prep"
    let tag = "rows8 {fmt} k={kdim} n={ndim} ys={ys} {feed}"
    with_metal_cell(dev, queue) $(var mc : MetalCell) {
        var err : string
        let kp = kernel_of(@@enc_rows8_prep_c)
        let kn = stamp_of(@@enc_rms_bhxt)
        let kg = rows8_aff_kernel(fmt)
        var ppso = mc |> keep_stamp(kp, err)
        var npso = mc |> keep_stamp(kn, err)
        var gpso = mc |> keep_stamp(kg, err)
        return if (!tensor_pso_or_skip(t, tag, gpso, dev, err) || !pso_ok(t, tag, ppso, err) || !pso_ok(t, tag, npso, err))
        if (!rows8_probe_ok(t, mc, err)) {
            t |> skip("{tag}: the matrix unit's cooperative layout is not the rows8 kernels' on this box - the engine keeps the form off")
            return
        }
        var inscope qp, sp : array<uint8>
        var inscope wref : array<float>
        mlx_test_planes(fmt, ndim, kdim, qp, sp, wref)
        let live = fed ? nrows : 8
        let nan = uint_bits_to_float(0x7FC00000u)
        var inscope xf, w : array<float>
        xf |> resize(8 * kdim)
        w |> resize(kdim)
        for (i in range(8 * kdim)) {
            xf[i] = float(float16(0.07 * float(i % 39) - 1.3))   //! the kernel reads the panel as halves: the oracle's panel is the same lattice
        }
        for (i in range(kdim)) {
            w[i] = 0.5 + 0.04 * float(i % 13)
        }
        let bsp = mc |> in_plain(sp)
        let bqp = mc |> in_plain(qp)
        let bx = mc |> in_(xf)
        let bw = mc |> in_(w)
        let by0 = mc |> out_words(8 * kdim, -1000.0)
        let bh = mc |> out_words(8 * kdim, float16(-1000.0))
        let bxt = mc |> out_words(8 * kdim, float16(nan))
        let bxs = mc |> out_words(kdim / 32 * 8, nan)
        let by = mc |> out_words(8 * ys, -1000.0)
        let bk = mc |> in_plain([uint(kdim)])
        let bys = mc |> in_plain([uint(ys)])
        let beps = mc |> in_plain([1.0e-5])
        let ran = mc |> cell_record(t, tag) $(enc : MetalComputeEncoder?) {
            if (fed) {
                stamp_run(kn, npso) {
                    invoke(kn.enc, enc, mc.buf[bx], 0ul, mc.buf[bw], mc.buf[by0], mc.buf[bk], mc.buf[beps], mc.buf[bh], mc.buf[bxt], mc.buf[bxs], int64(nrows))
                }
            } else {
                invoke(kp.enc, enc, ppso, mc.buf[bx], 0ul, mc.buf[bxt], mc.buf[bxs], mc.buf[bk], int64(kdim / 32 * 8))
            }
            invoke(kg.enc, enc, gpso, mc.buf[bsp], 0ul, mc.buf[bqp], 0ul, mc.buf[bxt], mc.buf[by], 0ul, mc.buf[bxs], mc.buf[bk], mc.buf[bys], int64(ndim), int64(7 * ys + ndim))
        }
        return if (!ran)
        var inscope halves <- buf_download(mc.buf[bh], live * kdim, type<float16>)
        var inscope xr <- [for (i in range(live * kdim)); fed ? float(halves[i]) : xf[i]]
        var inscope ref <- dense_ref_env(live, kdim, ndim, xr) $(j; var wrow) {
            for (k in range(kdim)) {
                wrow[k] = wref[j * kdim + k]
            }
        }
        var inscope yref <- rows8_y_ref(ref, ndim, ys, live)
        check_buf(t, tag, mc.buf[by], yref.want, env_bar(2e-4, yref.env), Shape(row_width = ys), yref.env)
    }
}

//! the rows8 grid rule (`rows8_grid_of`) over the 27B's sites, each format's K block, rows off whole 16-row tiles and an empty
//! plane - device-free
[test(description = "`rows8_grid_of` gives one threadgroup per 16-row tile on the 27B sites across mlx, k4, k6 and q8 formats, and returns zero for K off the format block, rows off whole tiles and an empty plane.")]
def test_rows8_grid_rule(t : T?) {
    t |> run("rows8 grid: one threadgroup a 16-row tile, K whole blocks of the format, else no site") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(das_metal)) {
            t |> equal(rows8_grid_of(KqFmt.mlx4b, 17408l, 5120l), 1088l, "gate / up: 17408 rows, 1088 threadgroups")
            t |> equal(rows8_grid_of(KqFmt.mlx4b, 5120l, 17408l), 320l, "down: 5120 rows")
            t |> equal(rows8_grid_of(KqFmt.mlx4b, 1024l, 5120l), 64l, "attention k / v: 1024 rows")
            t |> equal(rows8_grid_of(KqFmt.mlx4b, 248320l, 5120l), 15520l, "the head: 248320 rows")
            t |> equal(rows8_grid_of(KqFmt.q8, 96l, 5120l), 6l, "the q8 [beta | alpha] site's 96 rows")
            t |> equal(rows8_grid_of(KqFmt.k4, 17408l, 5120l), 1088l, "k4 gate / up")
            t |> equal(rows8_grid_of(KqFmt.k6, 6144l, 5120l), 384l, "k6 wo")
            t |> equal(rows8_grid_of(KqFmt.q8, 64l, 96l), 4l, "q8: K whole 32-blocks")
            t |> equal(rows8_grid_of(KqFmt.mlx8, 64l, 192l), 4l, "an affine member: K whole 64-groups")
            t |> equal(rows8_grid_of(KqFmt.k4, 64l, 384l), 0l, "k4: K off whole superblocks declines")
            t |> equal(rows8_grid_of(KqFmt.mlx4, 64l, 96l), 0l, "an affine member: K off whole 64-groups declines")
            t |> equal(rows8_grid_of(KqFmt.k6, 72l, 5120l), 0l, "rows off whole 16-row tiles decline")
            t |> equal(rows8_grid_of(KqFmt.q8, 0l, 5120l), 0l, "an empty plane declines")
        } else {
            t |> skip("no dasMetal in this build; the grid rule lives in the Metal prefill module")
        }
    }
}

[test(description = "Every Metal prefill GEMM stamp - kq and iquant mul_mm with tensor, half-x, tall and eight-row forms, rows8, split-K, dev-W, bf16, f32, MoE gathered and the attention QK/AV pair - matches its CPU reference within the bar on aligned and ragged shapes.")]
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
                    kq_mulmm_gate(t, dev, queue, fmt, true, 8, 512, 64, m8 = true)   // the rows sites' eight-row stamp
                    kq_mulmm_gate(t, dev, queue, fmt, true, 8, 3840, 128, m8 = true)
                }
                // the rows sites' matrix-unit form over an eight-row panel: y at the site's own row stride and at a wider one
                for (wide in [0, 48]) {
                    rows8_k4_gate(t, dev, queue, 512, 64, 64 + wide)
                    rows8_k4_gate(t, dev, queue, 3840, 128, 128 + wide)
                    rows8_q8_gate(t, dev, queue, 512, 64, 64 + wide)
                    rows8_q8_gate(t, dev, queue, 3840, 128, 128 + wide)
                    rows8_k4_fed_gate(t, dev, queue, 3840, 128, 128 + wide, 8)
                    rows8_k4_fed_gate(t, dev, queue, 512, 64, 64 + wide, 5)
                    rows8_q8_gate(t, dev, queue, 5120, 96, 96 + wide)   // the [beta | alpha] site's 96 rows: six threadgroups
                    for (fs in [6, 44]) {   //! the split-plane arms: k6, iq4xs
                        rows8_split_gate(t, dev, queue, fs, 512, 64, 64 + wide, false)
                        rows8_split_gate(t, dev, queue, fs, 3840, 128, 128 + wide, false)
                        rows8_split_gate(t, dev, queue, fs, 5120, 96, 96 + wide, false)
                        rows8_split_gate(t, dev, queue, fs, 3840, 128, 128 + wide, true, 8)
                        rows8_split_gate(t, dev, queue, fs, 512, 64, 64 + wide, true, 5)
                    }
                    for (fa in [KqFmt.mlx4, KqFmt.mlx8, KqFmt.mlx4b, KqFmt.mlx8b, KqFmt.mlx6b]) {   //! the affine members, each its own stamp
                        rows8_aff_gate(t, dev, queue, fa, 512, 64, 64 + wide, false)
                        rows8_aff_gate(t, dev, queue, fa, 3840, 128, 128 + wide, false)
                        rows8_aff_gate(t, dev, queue, fa, 5120, 96, 96 + wide, false)
                        rows8_aff_gate(t, dev, queue, fa, 3840, 128, 128 + wide, true, 8)
                        rows8_aff_gate(t, dev, queue, fa, 512, 64, 64 + wide, true, 5)
                    }
                }
                // the producers' table mode against their HX stamps: a narrow width, the 27B's 5120 past the add-rms slab, the FFN width
                for (nrows in [5, 8]) {
                    rows8_table_gate(t, dev, queue, TabProducer.rms, 512, nrows)
                    rows8_table_gate(t, dev, queue, TabProducer.rms, 5120, nrows)
                    rows8_table_gate(t, dev, queue, TabProducer.add_rms, 2304, nrows)
                    rows8_table_gate(t, dev, queue, TabProducer.add_rms_wide, 5120, nrows)
                    rows8_table_gate(t, dev, queue, TabProducer.pre, 2304, nrows)
                    rows8_table_gate(t, dev, queue, TabProducer.pre_wide, 5120, nrows)
                    rows8_table_gate(t, dev, queue, TabProducer.swiglu, 17408, nrows)
                    rows8_table_gate(t, dev, queue, TabProducer.geglu, 1536, nrows)
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
                kq_mulmm_gate(t, dev, queue, 35, false, 64, 512, 64)
                kq_mulmm_gate(t, dev, queue, 35, false, 32, 256, 128)
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
                kq_mulmm_gate(t, dev, queue, 49, false, 64, 512, 64)
                kq_mulmm_gate(t, dev, queue, 49, false, 32, 256, 128)
                kq_mulmm_gate(t, dev, queue, 2, false, 64, 512, 64)
                kq_mulmm_gate(t, dev, queue, 2, false, 32, 256, 128)
                kq_mulmm_gate(t, dev, queue, 23, false, 64, 512, 64)
                kq_mulmm_gate(t, dev, queue, 23, false, 32, 256, 128)
                kq_mulmm_gate(t, dev, queue, 24, false, 64, 512, 64)
                kq_mulmm_gate(t, dev, queue, 24, false, 32, 256, 128)
                kq_mulmm_gate(t, dev, queue, 25, false, 64, 512, 64)
                kq_mulmm_gate(t, dev, queue, 25, false, 32, 256, 128)
                // the iquant tensor twins (T, TH and the tall TH128 stamps; twins skip without Metal-4)
                for (fmt in [44, 35, 45, 40, 49, 3, 33, 34, 2, 23, 24, 25]) {
                    kq_mulmm_gate(t, dev, queue, fmt, true, 64, 512, 64)
                    kq_mulmm_gate(t, dev, queue, fmt, true, 64, 512, 64, halfx = true)
                    kq_mulmm_gate(t, dev, queue, fmt, true, 256, 512, 64, halfx = true, tall = true)
                    kq_mulmm_gate(t, dev, queue, fmt, true, 320, 512, 64, halfx = true, tall = true)
                }
                q8_mulmm_t_gate(t, dev, queue, 64, 256, 64)
                q8_mulmm_t_gate(t, dev, queue, 64, 256, 64, halfx = true)
                q8_mulmm_t_gate(t, dev, queue, 64, 160, 64)               // kdim % 64 == 32: the bk=64 helper's 32-tail chunk
                q8_mulmm_t_gate(t, dev, queue, 64, 160, 64, halfx = true)
                q8_mulmm_t_gate(t, dev, queue, 128, 256, 64, mt = 128)   // tall M-tile stamps
                q8_mulmm_t_gate(t, dev, queue, 256, 256, 64, halfx = true, mt = 128)
                q8_mulmm_t_gate(t, dev, queue, 8, 256, 64, mt = 8)   // the rows sites' eight-row stamp
                q8_mulmm_t_gate(t, dev, queue, 8, 160, 128, mt = 8)
                q8_devw_gate(t, dev, queue, 64, 256, 64)
                q8_devw_gate(t, dev, queue, 32, 288, 128)   // non-256-multiple block count arms the dequant tail guard
                q8_devw_gate(t, dev, queue, 32, 512, 128)   //! the tiled chain's split leg: two 256-deep shares
                q8_devw_gate(t, dev, queue, 128, 256, 128, mt = 128)   // tall dev-W GEMM stamp
                for (fmt in [4, 5, 6, 44, 35, 45, 40, 49, 3, 33, 34, 2, 23, 24, 25]) {
                    kq_devw_gate(t, dev, queue, fmt, 64, 512, 64)
                    kq_devw_gate(t, dev, queue, fmt, 32, 512, 128)   // ndim%128: the tiled leg runs
                }
                for (fmt in [0, 4, 5, 6, 44, 45, 40, 49, 3, 33, 34, 2, 23, 24, 25]) {
                    devw_bake_mirror_gate(t, dev, queue, fmt, 64, 512)
                }
                q8_dbuf_gate(t, dev, queue, 128, 256, 64)
                q8_dbuf_gate(t, dev, queue, 128, 288, 128)   //! kdim%64 == 32: the tail chunk on both arms
                q8_dbuf_gate(t, dev, queue, 256, 512, 128)   //! two M-tiles, deeper K
                q8_dbuf_gate(t, dev, queue, 64, 256, 64, mt = 32)   //! the 32-row stamp pair
                for (fmt in [4, 5, 6, 44, 35, 45, 40, 49, 3, 33, 34, 2, 23, 24, 25]) {
                    kq_dbuf_gate(t, dev, queue, fmt, 128, 512, 64)
                    kq_dbuf_gate(t, dev, queue, fmt, 64, 512, 64, mt = 32)   //! the 32-row stamp pair
                }
                // ===== the short window's split-K forms: one split (the stamp writes y) and several (partial planes + reduce) =====
                for (halfx in [false, true]) {
                    q8_split_gate(t, dev, queue, 32, 1024, 64, 1, halfx)
                    q8_split_gate(t, dev, queue, 32, 1024, 64, 4, halfx)
                    q8_split_gate(t, dev, queue, 64, 1024, 96, 8, halfx)   //! two M tiles, three 32-column tiles, eight 4-block shares
                    q8_split_gate(t, dev, queue, 16, 1024, 64, 1, halfx)   //! the 16-row stamps, whole and split
                    q8_split_gate(t, dev, queue, 16, 1024, 64, 4, halfx)
                    if (!halfx) {   //! the eight-row split stamp has the f32 X form alone
                        q8_split_gate(t, dev, queue, 8, 1024, 64, 1, false)
                        q8_split_gate(t, dev, queue, 8, 1024, 96, 4, false)
                    }
                    bf16_split_gate(t, dev, queue, 32, 512, 64, 1, halfx)
                    bf16_split_gate(t, dev, queue, 64, 512, 128, 4, halfx)
                    for (fmt in [4, 5, 6, 44, 35, 45, 40, 49, 3, 33, 34, 2, 23, 24, 25]) {
                        kq_split_gate(t, dev, queue, fmt, 32, 1024, 64, 1, halfx)
                        kq_split_gate(t, dev, queue, fmt, 64, 1024, 128, 4, halfx)   //! a 256-K share: one superblock a split
                        kq_split_gate(t, dev, queue, fmt, 16, 1024, 64, 2, halfx)   //! the 16-row stamp
                        if (!halfx) {
                            kq_split_gate(t, dev, queue, fmt, 8, 1024, 64, 1, false)   //! the eight-row stamp, whole and split
                            kq_split_gate(t, dev, queue, fmt, 8, 1024, 128, 4, false)
                        }
                    }
                }
                //! the simdgroup base tiles' split-K stamps, every superblock format and the q8 blob
                base_split_gate(t, dev, queue, 0, 32, 1024, 64, 4)
                base_split_gate(t, dev, queue, 0, 64, 1024, 128, 2)
                for (fmt in [4, 5, 6, 44, 35, 45, 40, 49, 3, 33, 34, 2, 23, 24, 25]) {
                    base_split_gate(t, dev, queue, fmt, 32, 1024, 64, 4)
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
                router_f32_gate(t, dev, queue, 40, 64, 2048, 256)          // the 35B-A3B router: 256 experts over 2048, a 40-row window
                router_f32_gate(t, dev, queue, 96, 96, 512, 64)            // the 64-wide tile's lower bound, no pad rows
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
                for (fmt in [4, 5, 6, 3, 2, 44, 35, 45, 40, 49, 33, 34, 23, 24, 25]) {
                    moe_mulmm_kq_gate(t, dev, queue, fmt, 256, 64, [32, 0, 40, 8], true)   // the in-kernel gather; a zero-count expert
                    moe_mulmm_kq_gate(t, dev, queue, fmt, 256, 128, [8, 33], false)   // 2 col tiles + contiguous
                    moe_mulmm_split_gate(t, dev, queue, fmt, 256, 128, [8, 33])
                    moe_mulmm_split_gate(t, dev, queue, fmt, 256, 128, [12, 45])   // the adaptive op's 16-row walk: 12 live rows, and a second tile of 13
                    moe_mulmm_split_gate(t, dev, queue, fmt, 256, 128, [130, 40, 200])   // tall pair: counts cross 128
                }
                moe_mulmm_kq_gate(t, dev, queue, 2, 512, 64, [40, 8], false)   // two superblocks a row: the base walk's sb stride
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
