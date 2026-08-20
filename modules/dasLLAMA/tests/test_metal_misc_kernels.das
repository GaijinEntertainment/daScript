options gen2
options stack = 524288   // every dasLLAMA program root takes this budget
options _function_length = 0        // per-kernel gates: hand-bound buffers/uniforms + dispatch + oracle compare, one flat run each
options _cyclomatic_complexity = 0  // same — the arm ladders follow the kernel's own binding/variant axes
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama_convert   // dequant_k6_plane_superblock_at — the embed-k6 reference
require dasllama/dasllama_tower     // layernorm + the shipped gelu flavors + the gemma4v pieces — the tower oracles
require dasllama/dasllama_rope      // build_rope_tabs_rows — the rope2d gate's tables
require ?das_metal dasllama/dasllama_metal_kernels  // the decode elementwise/misc kernel set (Apple builds)
require ?das_metal metal/das_metal_boost  // device/pipeline/buffer plumbing + the leak gate
require ?das_metal _metal_kernel_common   // shared buf helpers + eyeball-dump compares
require ?das_metal _mtl_toy               // the [metal_dispatch] multi-kernel fixture
require llvm/daslib/f16_cvt         // f32_to_f16 — packing the synthetic scale planes
require math

// The decode-side elementwise/misc corpus vs direct CPU oracles on synthetic data: copy_row,
// argmax (the CPU sampler's exact lowest-index tie-break, exercised with deliberate exact
// ties), the q8/k6 embed gathers (pow2 scales -> bit-exact), the fused add+rms family (simd_sum
// reassociates -> the shared 1e-4 bar), per-head QK-norm in both its in-place and V-fusion
// binding shapes, geglu, and the logit suppress/softcap pins. Outputs sit in sentinel-padded
// buffers compared WHOLE, so a write outside the target span is a red. Off-Apple (or with no
// Metal device) every check feints cleanly.

let private EPS = 1.0e-5

def private copy_row_gate(t : T?; dev, queue; n : int) {
    let tag = "copy_row n={n}"
    var err : string
    var pso = pipeline_from_source(dev, metal_copy_row_msl, metal_copy_row_msl_entry, metal_copy_row_msl_fastmath, err)
    t |> success(pso != null, "{tag}: pipeline: {err}")
    return if (pso == null)
    var src : array<float>
    src |> resize(n)
    for (i in range(n)) {
        src[i] = 0.13 * float(i % 51) - 3.1
    }
    var want : array<float>
    want |> resize(n + 16)
    for (i in range(n + 16)) {
        want[i] = i < n ? src[i] : -1000.0   // the guard must leave the pad untouched
    }
    var bs = buf_upload(dev, src)
    var bd = buf_fill(dev, n + 16, -1000.0)
    var bn = buf_u32(dev, uint(n))
    let ran = with_compute_encoder(queue, err) $(enc : MetalComputeEncoder?) {
        metal_set_pipeline(enc, pso)
        metal_set_buffer(enc, bs, 0ul, 0)
        metal_set_buffer(enc, bd, 0ul, 1)
        metal_set_buffer(enc, bn, 0ul, 2)
        metal_dispatch_threadgroups(enc, uint3(uint((n + 63) / 64), 1u, 1u), uint3(64u, 1u, 1u))
    }
    t |> success(ran, "{tag}: encode: {err}")
    if (ran) {
        t |> equal(buf_mismatch_exact(bd, want, tag), 0)
    }
    metal_release(bs)
    metal_release(bd)
    metal_release(bn)
    metal_release(pso)
    delete src
    delete want
}

def private argmax_gate(t : T?; dev, queue; n, hi1, hi2 : int) {
    let tag = "argmax n={n} ties at {hi1},{hi2}"
    var err : string
    var pso = pipeline_from_source(dev, metal_argmax_msl, metal_argmax_msl_entry, metal_argmax_msl_fastmath, err)
    t |> success(pso != null, "{tag}: pipeline: {err}")
    return if (pso == null)
    var lg : array<float>
    lg |> resize(n)
    for (i in range(n)) {
        lg[i] = 0.07 * float(i % 89) - 5.0
    }
    lg[hi1] = 9.5   // exact tie — the LOWER index must win (the CPU sampler's rule)
    lg[hi2] = 9.5
    var blg = buf_upload(dev, lg)
    var btok = buf_fill(dev, 1, 0xFFFFFFFFu)
    var bn = buf_u32(dev, uint(n))
    let ran = with_compute_encoder(queue, err) $(enc : MetalComputeEncoder?) {
        metal_set_pipeline(enc, pso)
        metal_set_threadgroup_memory_length(enc, metal_argmax_msl_tgmem, 0)
        metal_set_buffer(enc, blg, 0ul, 0)
        metal_set_buffer(enc, btok, 0ul, 1)
        metal_set_buffer(enc, bn, 0ul, 2)
        metal_dispatch_threadgroups(enc, uint3(1u, 1u, 1u), uint3(1024u, 1u, 1u))
    }
    t |> success(ran, "{tag}: encode: {err}")
    if (ran) {
        var want : array<uint>
        want |> resize(1)
        want[0] = uint(min(hi1, hi2))
        t |> equal(buf_mismatch_exact(btok, want, tag), 0)
        delete want
    }
    metal_release(blg)
    metal_release(btok)
    metal_release(bn)
    metal_release(pso)
    delete lg
}

def private embed_q8_gate(t : T?; dev, queue; dim, vocab, tok : int) {
    let tag = "embed_q8 dim={dim} vocab={vocab} tok={tok}"
    var err : string
    var pso = pipeline_from_source(dev, metal_embed_q8_msl, metal_embed_q8_msl_entry, metal_embed_q8_msl_fastmath, err)
    t |> success(pso != null, "{tag}: pipeline: {err}")
    return if (pso == null)
    let nblk = vocab * dim / 32
    let escale = 2.0
    let lut = fixed_array(0.25, 0.5, 1.0, 2.0)
    // the embed blob: nblk 34-byte q8 blocks (pow2 scales + int8 quants -> the gather is exact)
    var blob : array<uint8>
    blob |> resize(nblk * 34)
    var want : array<float>
    want |> resize(dim)
    for (b in range(nblk)) {
        let d = lut[(b * 7 + 1) % 4]
        let dbits = f32_to_f16(d)
        blob[b * 34] = uint8(dbits & 0xFFu)
        blob[b * 34 + 1] = uint8(dbits >> 8u)
        for (c in range(32)) {
            let q = (b * 5 + c * 11) % 255 - 127
            blob[b * 34 + 2 + c] = uint8(q & 0xFF)
            let e = b * 32 + c
            if (e >= tok * dim && e < (tok + 1) * dim) {
                want[e - tok * dim] = float(q) * d * escale
            }
        }
    }
    var bblob = buf_upload(dev, blob)
    var btok = buf_fill(dev, 1, uint(tok))
    var bxo = buf_fill(dev, dim, -1000.0)
    var bdim = buf_u32(dev, uint(dim))
    var besc = buf_f32(dev, escale)
    let ran = with_compute_encoder(queue, err) $(enc : MetalComputeEncoder?) {
        metal_set_pipeline(enc, pso)
        metal_set_buffer(enc, bblob, 0ul, 0)
        metal_set_buffer(enc, bblob, 0ul, 1)
        metal_set_buffer(enc, btok, 0ul, 2)
        metal_set_buffer(enc, bxo, 0ul, 3)
        metal_set_buffer(enc, bdim, 0ul, 4)
        metal_set_buffer(enc, besc, 0ul, 5)
        metal_dispatch_threadgroups(enc, uint3(uint((dim + 63) / 64), 1u, 1u), uint3(64u, 1u, 1u))
    }
    t |> success(ran, "{tag}: encode: {err}")
    if (ran) {
        t |> equal(buf_mismatch_exact(bxo, want, tag), 0)
    }
    metal_release(bblob)
    metal_release(btok)
    metal_release(bxo)
    metal_release(bdim)
    metal_release(besc)
    metal_release(pso)
    delete blob
    delete want
}

def private embed_k6_gate(t : T?; dev, queue; dim, vocab, tok : int) {
    let tag = "embed_k6 dim={dim} vocab={vocab} tok={tok}"
    var err : string
    var pso = pipeline_from_source(dev, metal_embed_k6_msl, metal_embed_k6_msl_entry, metal_embed_k6_msl_fastmath, err)
    t |> success(pso != null, "{tag}: pipeline: {err}")
    return if (pso == null)
    let nb = dim / 256
    let nsb = vocab * nb
    let escale = 2.0
    let lut = fixed_array(0.25, 0.5, 1.0, 2.0)
    var kq : array<uint8>
    var ks : array<uint8>
    kq |> resize(nsb * 192)
    ks |> resize(nsb * 18)
    for (i in range(nsb * 192)) {
        var h = uint(i) * 2654435761u
        h ^= h >> 15u
        kq[i] = uint8((h >> 7u) & 0xFFu)
    }
    for (i in range(nsb * 16)) {
        let h = uint(i * 13 + 5) * 2246822519u
        ks[i] = uint8((h >> 11u) & 0xFFu)
    }
    for (b in range(nsb)) {
        let dbits = f32_to_f16(lut[(b * 3 + 2) % 4])
        ks[nsb * 16 + b * 2] = uint8(dbits & 0xFFu)
        ks[nsb * 16 + b * 2 + 1] = uint8(dbits >> 8u)
    }
    var want : array<float>
    want |> resize(dim)
    for (ib in range(nb)) {
        let sb = tok * nb + ib
        dequant_k6_plane_superblock_at(kq, int64(sb * 192), ks, int64(sb * 16), int64(nsb * 16 + sb * 2), want, int64(ib * 256))
    }
    for (i in range(dim)) {
        want[i] *= escale
    }
    var bks = buf_upload(dev, ks)
    var bkq = buf_upload(dev, kq)
    var btok = buf_fill(dev, 1, uint(tok))
    var bxo = buf_fill(dev, dim, -1000.0)
    var bdim = buf_u32(dev, uint(dim))
    var besc = buf_f32(dev, escale)
    let ran = with_compute_encoder(queue, err) $(enc : MetalComputeEncoder?) {
        metal_set_pipeline(enc, pso)
        metal_set_buffer(enc, bks, uint64(nsb * 16), 0)   // the f16 d tail
        metal_set_buffer(enc, bks, 0ul, 1)
        metal_set_buffer(enc, bkq, 0ul, 2)
        metal_set_buffer(enc, btok, 0ul, 3)
        metal_set_buffer(enc, bxo, 0ul, 4)
        metal_set_buffer(enc, bdim, 0ul, 5)
        metal_set_buffer(enc, besc, 0ul, 6)
        metal_dispatch_threadgroups(enc, uint3(uint((dim + 63) / 64), 1u, 1u), uint3(64u, 1u, 1u))
    }
    t |> success(ran, "{tag}: encode: {err}")
    if (ran) {
        t |> equal(buf_mismatch_exact(bxo, want, tag), 0)
    }
    metal_release(bks)
    metal_release(bkq)
    metal_release(btok)
    metal_release(bxo)
    metal_release(bdim)
    metal_release(besc)
    metal_release(pso)
    delete kq
    delete ks
    delete want
}

// the fused add+rms family: sequential CPU mirror, simd_sum reassociation under the 1e-4 bar
def private add_rms_gate(t : T?; dev, queue; batched : bool; dim, nrows : int; ascale : float) {
    let tag = "add_rms{batched ? "_b" : ""} dim={dim} rows={nrows} ascale={ascale}"
    var err : string
    var pso = pipeline_from_source(dev, metal_add_rms_msl, metal_add_rms_msl_entry, metal_add_rms_msl_fastmath, err)
    t |> success(pso != null, "{tag}: pipeline: {err}")
    return if (pso == null)
    let total = nrows * dim
    var xv : array<float>
    var av : array<float>
    var w : array<float>
    xv |> resize(total)
    av |> resize(total)
    w |> resize(dim)
    for (i in range(total)) {
        xv[i] = 0.11 * float(i % 43) - 2.3
        av[i] = 0.07 * float(i % 31) - 1.1
    }
    for (i in range(dim)) {
        w[i] = 0.5 + 0.04 * float(i % 13)
    }
    var want_x : array<float>
    var want_y : array<float>
    want_x |> resize(total)
    want_y |> resize(total)
    for (r in range(nrows)) {
        var ss = 0.0
        for (i in range(dim)) {
            let vv = (xv[r * dim + i] + av[r * dim + i]) * ascale
            want_x[r * dim + i] = vv
            ss += vv * vv
        }
        let inv = 1.0 / sqrt(ss / float(dim) + EPS)
        for (i in range(dim)) {
            want_y[r * dim + i] = w[i] * (want_x[r * dim + i] * inv)
        }
    }
    var bx = buf_upload(dev, xv)
    var ba = buf_upload(dev, av)
    var bw = buf_upload(dev, w)
    var by = buf_fill(dev, total, -1000.0)
    var bdim = buf_u32(dev, uint(dim))
    var beps = buf_f32(dev, EPS)
    var basc = buf_f32(dev, ascale)
    let ran = with_compute_encoder(queue, err) $(enc : MetalComputeEncoder?) {
        metal_set_pipeline(enc, pso)
        metal_set_threadgroup_memory_length(enc, metal_add_rms_msl_tgmem, 0)
        metal_set_buffer(enc, bx, 0ul, 0)
        metal_set_buffer(enc, ba, 0ul, 1)
        metal_set_buffer(enc, bw, 0ul, 2)
        metal_set_buffer(enc, by, 0ul, 3)
        metal_set_buffer(enc, bdim, 0ul, 4)
        metal_set_buffer(enc, beps, 0ul, 5)
        metal_set_buffer(enc, basc, 0ul, 6)
        metal_dispatch_threadgroups(enc, uint3(uint(nrows), 1u, 1u), uint3(1024u, 1u, 1u))
    }
    t |> success(ran, "{tag}: encode: {err}")
    if (ran) {
        t |> equal(buf_mismatch_approx(bx, want_x, "{tag} x"), 0)
        t |> equal(buf_mismatch_approx(by, want_y, "{tag} y"), 0)
    }
    metal_release(bx)
    metal_release(ba)
    metal_release(bw)
    metal_release(by)
    metal_release(bdim)
    metal_release(beps)
    metal_release(basc)
    metal_release(pso)
    delete xv
    delete av
    delete w
    delete want_x
    delete want_y
}

def private pre_add_rms_gate(t : T?; dev, queue; dim : int) {
    let tag = "pre_add_rms dim={dim}"
    var err : string
    var pso = pipeline_from_source(dev, metal_pre_add_rms_msl, metal_pre_add_rms_msl_entry, metal_pre_add_rms_msl_fastmath, err)
    t |> success(pso != null, "{tag}: pipeline: {err}")
    return if (pso == null)
    var xv : array<float>
    var av : array<float>
    var wpa : array<float>
    var wffn : array<float>
    xv |> resize(dim)
    av |> resize(dim)
    wpa |> resize(dim)
    wffn |> resize(dim)
    for (i in range(dim)) {
        xv[i] = 0.09 * float(i % 41) - 1.9
        av[i] = 0.13 * float(i % 37) - 2.1
        wpa[i] = 0.4 + 0.05 * float(i % 11)
        wffn[i] = 0.6 + 0.03 * float(i % 17)
    }
    var want_x : array<float>
    var want_y : array<float>
    want_x |> resize(dim)
    want_y |> resize(dim)
    var ssa = 0.0
    for (i in range(dim)) {
        ssa += av[i] * av[i]
    }
    let inva = 1.0 / sqrt(ssa / float(dim) + EPS)
    var ssx = 0.0
    for (i in range(dim)) {
        let vv = xv[i] + av[i] * inva * wpa[i]
        want_x[i] = vv
        ssx += vv * vv
    }
    let invx = 1.0 / sqrt(ssx / float(dim) + EPS)
    for (i in range(dim)) {
        want_y[i] = wffn[i] * (want_x[i] * invx)
    }
    var bx = buf_upload(dev, xv)
    var ba = buf_upload(dev, av)
    var bwpa = buf_upload(dev, wpa)
    var bwffn = buf_upload(dev, wffn)
    var by = buf_fill(dev, dim, -1000.0)
    var bdim = buf_u32(dev, uint(dim))
    var beps = buf_f32(dev, EPS)
    let ran = with_compute_encoder(queue, err) $(enc : MetalComputeEncoder?) {
        metal_set_pipeline(enc, pso)
        metal_set_threadgroup_memory_length(enc, metal_pre_add_rms_msl_tgmem, 0)
        metal_set_buffer(enc, bx, 0ul, 0)
        metal_set_buffer(enc, ba, 0ul, 1)
        metal_set_buffer(enc, bwpa, 0ul, 2)
        metal_set_buffer(enc, bwffn, 0ul, 3)
        metal_set_buffer(enc, by, 0ul, 4)
        metal_set_buffer(enc, bdim, 0ul, 5)
        metal_set_buffer(enc, beps, 0ul, 6)
        metal_dispatch_threadgroups(enc, uint3(1u, 1u, 1u), uint3(1024u, 1u, 1u))
    }
    t |> success(ran, "{tag}: encode: {err}")
    if (ran) {
        t |> equal(buf_mismatch_approx(bx, want_x, "{tag} x"), 0)
        t |> equal(buf_mismatch_approx(by, want_y, "{tag} y"), 0)
    }
    metal_release(bx)
    metal_release(ba)
    metal_release(bwpa)
    metal_release(bwffn)
    metal_release(by)
    metal_release(bdim)
    metal_release(beps)
    metal_release(pso)
    delete xv
    delete av
    delete wpa
    delete wffn
    delete want_x
    delete want_y
}

// per-head QK-norm: vfusion=false is the in-place decode shape (soffe == koffe), vfusion=true
// the no-wv form (reads the pre-norm K slice, writes the v segment)
def private qk_norm_gate(t : T?; dev, queue; hs, nq, nk, nrows : int; vfusion : bool) {
    let tag = "qk_norm hs={hs} nq={nq} nk={nk} rows={nrows} vfusion={vfusion}"
    var err : string
    var pso = pipeline_from_source(dev, metal_qk_norm_msl, metal_qk_norm_msl_entry, metal_qk_norm_msl_fastmath, err)
    t |> success(pso != null, "{tag}: pipeline: {err}")
    return if (pso == null)
    let qd = nq * hs
    let kvd = nk * hs
    let rstride = qd + 2 * kvd
    let soffe = qd                          // K region read base
    let koffe = vfusion ? qd + kvd : qd     // write base: v segment under fusion, in-place otherwise
    var xv : array<float>
    var w : array<float>
    xv |> resize(nrows * rstride)
    w |> resize(2 * hs)
    for (i in range(nrows * rstride)) {
        xv[i] = 0.11 * float(i % 47) - 2.5
    }
    for (i in range(2 * hs)) {
        w[i] = 0.5 + 0.04 * float(i % 19)
    }
    var want := xv
    for (r in range(nrows)) {
        for (tg2 in range(nq + nk)) {
            let isK = tg2 >= nq
            let h = isK ? tg2 - nq : tg2
            let rbase = r * rstride + (isK ? soffe : 0) + h * hs
            let wbase = r * rstride + (isK ? koffe : 0) + h * hs
            var ss = 0.0
            for (i in range(hs)) {
                ss += xv[rbase + i] * xv[rbase + i]
            }
            let inv = 1.0 / sqrt(ss / float(hs) + EPS)
            for (i in range(hs)) {
                want[wbase + i] = w[(isK ? hs : 0) + i] * (xv[rbase + i] * inv)
            }
        }
    }
    var bx = buf_upload(dev, xv)
    var bw = buf_upload(dev, w)
    var ka = QkNormArgs(nq = uint(nq), koffe = uint(koffe), rstride = uint(rstride), hs = uint(hs),
        woffe = uint(hs), eps = EPS, soffe = uint(soffe))
    let ran = with_compute_encoder(queue, err) $(enc : MetalComputeEncoder?) {
        metal_set_pipeline(enc, pso)
        metal_set_threadgroup_memory_length(enc, metal_qk_norm_msl_tgmem, 0)
        metal_set_buffer(enc, bx, 0ul, 0)
        metal_set_buffer(enc, bw, 0ul, 1)
        metal_set_bytes(enc, unsafe(addr<void?>(ka)), uint64(typeinfo sizeof(ka)), 2)
        metal_dispatch_threadgroups(enc, uint3(uint(nq + nk), uint(nrows), 1u), uint3(uint(hs), 1u, 1u))
    }
    t |> success(ran, "{tag}: encode: {err}")
    if (ran) {
        t |> equal(buf_mismatch_approx(bx, want, tag), 0)
    }
    metal_release(bx)
    metal_release(bw)
    metal_release(pso)
    delete xv
    delete w
    delete want
}

def private geglu_gate(t : T?; dev, queue; total : int) {
    let tag = "geglu total={total}"
    var err : string
    var pso = pipeline_from_source(dev, MetalGeglu_metal_ew2_msl, MetalGeglu_metal_ew2_msl_entry, MetalGeglu_metal_ew2_msl_fastmath, err)
    t |> success(pso != null, "{tag}: pipeline: {err}")
    return if (pso == null)
    var g : array<float>
    var u : array<float>
    g |> resize(total)
    u |> resize(total)
    for (i in range(total)) {
        g[i] = 0.21 * float(i % 43) - 4.3
        u[i] = 0.17 * float(i % 23) - 1.9
    }
    var want : array<float>
    want |> resize(total)
    for (i in range(total)) {
        // the CPU gelu formula (tanh approx) — the GPU's exp identity agrees inside the bar
        let vv = g[i]
        want[i] = 0.5 * vv * (1.0 + tanh(0.7978845608028654 * (vv + 0.044715 * vv * vv * vv))) * u[i]
    }
    var bg = buf_upload(dev, g)
    var bu = buf_upload(dev, u)
    var btot = buf_u32(dev, uint(total))
    let ran = with_compute_encoder(queue, err) $(enc : MetalComputeEncoder?) {
        metal_set_pipeline(enc, pso)
        metal_set_buffer(enc, bg, 0ul, 0)
        metal_set_buffer(enc, bu, 0ul, 1)
        metal_set_buffer(enc, btot, 0ul, 2)
        metal_dispatch_threadgroups(enc, uint3(uint((total + 63) / 64), 1u, 1u), uint3(64u, 1u, 1u))
    }
    t |> success(ran, "{tag}: encode: {err}")
    if (ran) {
        t |> equal(buf_mismatch_approx(bg, want, tag), 0)
    }
    metal_release(bg)
    metal_release(bu)
    metal_release(btot)
    metal_release(pso)
    delete g
    delete u
    delete want
}

// ===== the gemma4v tower primitives =====

// clamp: dst = clamp(src, lo, hi) with the bounds read off a clamp record in a blob-shaped buffer
// (`side` picks the input or output pair); exact vs the CPU clamp, the pad sentinel untouched.
let private CLAMP_IN = 0    // the record's input pair (imin, imax)
let private CLAMP_OUT = 1   // its output pair (omin, omax)

def private clamp_gate(t : T?; dev, queue; total : int; side : int; inplace : bool) {
    let tag = "clamp total={total} {side == CLAMP_IN ? "in" : "out"} inplace={inplace}"
    var err : string
    var pso = pipeline_from_source(dev, metal_clamp_msl, metal_clamp_msl_entry, metal_clamp_msl_fastmath, err)
    t |> success(pso != null, "{tag}: pipeline: {err}")
    return if (pso == null)
    var src : array<float>
    src |> resize(total)
    for (i in range(total)) {
        src[i] = 0.37 * float(i % 97) - 17.0   // -17 .. 18.5
    }
    // a record like the tower's: imin imax omin omax active, sitting past some junk floats
    let rec <- [0.0, 0.0, 0.0, -6.0, 5.0, -2.5, 9.25, 1.0]
    let lo = side == CLAMP_IN ? rec[3] : rec[5]
    let hi = side == CLAMP_IN ? rec[4] : rec[6]
    var want : array<float>
    want |> resize(total + 16)
    for (i in range(total + 16)) {
        want[i] = i < total ? clamp(src[i], lo, hi) : -1000.0
    }
    var bs = buf_upload(dev, src)
    var bd = inplace ? bs : buf_fill(dev, total + 16, -1000.0)
    var brec = buf_upload(dev, rec)
    var bside = buf_u32(dev, uint(side))
    var btot = buf_u32(dev, uint(total))
    let ran = with_compute_encoder(queue, err) $(enc : MetalComputeEncoder?) {
        metal_set_pipeline(enc, pso)
        metal_set_buffer(enc, bs, 0ul, 0)
        metal_set_buffer(enc, bd, 0ul, 1)
        metal_set_buffer(enc, brec, 12ul, 2)   // the record starts 3 floats in
        metal_set_buffer(enc, bside, 0ul, 3)
        metal_set_buffer(enc, btot, 0ul, 4)
        metal_dispatch_threadgroups(enc, uint3(uint((total + 63) / 64), 1u, 1u), uint3(64u, 1u, 1u))
    }
    t |> success(ran, "{tag}: encode: {err}")
    if (ran) {
        if (inplace) {
            want |> resize(total)
        }
        t |> equal(buf_mismatch_exact(bd, want, tag), 0)
    }
    metal_release(bs)
    if (!inplace) {
        metal_release(bd)
    }
    metal_release(brec)
    metal_release(bside)
    metal_release(btot)
    metal_release(pso)
    delete src
    delete want
}

// GEGLU-quick: gelu_quick(g)*u through the CPU's f16-table form (dasllama_audio's
// geglu_quick_lut_rows is the oracle) — the in-kernel binary16 round-trips must land on the
// same table values, so the bar is the approx one (one f16 ulp of form slack at the boundary).
def private geglu_quick_gate(t : T?; dev, queue; ff, rows : int) {
    let total = ff * rows
    let tag = "geglu_quick total={total}"
    var err : string
    var pso = pipeline_from_source(dev, MetalGegluQuick_metal_ew2_msl, MetalGegluQuick_metal_ew2_msl_entry, MetalGegluQuick_metal_ew2_msl_fastmath, err)
    t |> success(pso != null, "{tag}: pipeline: {err}")
    return if (pso == null)
    var g : array<float>
    var u : array<float>
    g |> resize(total)
    u |> resize(total)
    for (i in range(total)) {
        g[i] = 0.21 * float(i % 43) - 4.3
        u[i] = 0.17 * float(i % 23) - 1.9
    }
    var want : array<float>
    want |> resize(total)
    geglu_quick_lut_rows(want, g, u, int64(ff), int64(rows))
    var bg = buf_upload(dev, g)
    var bu = buf_upload(dev, u)
    var btot = buf_u32(dev, uint(total))
    let ran = with_compute_encoder(queue, err) $(enc : MetalComputeEncoder?) {
        metal_set_pipeline(enc, pso)
        metal_set_buffer(enc, bg, 0ul, 0)
        metal_set_buffer(enc, bu, 0ul, 1)
        metal_set_buffer(enc, btot, 0ul, 2)
        metal_dispatch_threadgroups(enc, uint3(uint((total + 63) / 64), 1u, 1u), uint3(64u, 1u, 1u))
    }
    t |> success(ran, "{tag}: encode: {err}")
    if (ran) {
        t |> equal(buf_mismatch_approx(bg, want, tag), 0)
    }
    metal_release(bg)
    metal_release(bu)
    metal_release(btot)
    metal_release(pso)
    delete g
    delete u
    delete want
}

// head restride (gemma3v): token-major rows repack [npos x n_head*hs_src] -> [npos x n_head*hs_dst]
// with dst lanes past hs_src zeroed; exact both directions, the pad sentinel untouched.
def private head_restride_gate(t : T?; dev, queue; npos, n_head, hs_src, hs_dst : int) {
    let total = npos * n_head * hs_dst
    let tag = "head_restride npos={npos} heads={n_head} {hs_src}->{hs_dst}"
    var err : string
    var pso = pipeline_from_source(dev, metal_head_restride_msl, metal_head_restride_msl_entry, metal_head_restride_msl_fastmath, err)
    t |> success(pso != null, "{tag}: pipeline: {err}")
    return if (pso == null)
    var src : array<float>
    src |> resize(npos * n_head * hs_src)
    for (i in range(length(src))) {
        src[i] = 0.31 * float(i % 89) - 13.0
    }
    var want : array<float>
    want |> resize(total + 16)
    for (i in range(total + 16)) {
        if (i >= total) {
            want[i] = -1000.0   // the pad sentinel
        } else {
            let lane = i % hs_dst
            let hp = i / hs_dst
            want[i] = lane < hs_src ? src[hp * hs_src + lane] : 0.0
        }
    }
    var bs = buf_upload(dev, src)
    var bd = buf_fill(dev, total + 16, -1000.0)
    var bsrc_hs = buf_u32(dev, uint(hs_src))
    var bdst_hs = buf_u32(dev, uint(hs_dst))
    var btot = buf_u32(dev, uint(total))
    let ran = with_compute_encoder(queue, err) $(enc : MetalComputeEncoder?) {
        metal_set_pipeline(enc, pso)
        metal_set_buffer(enc, bs, 0ul, 0)
        metal_set_buffer(enc, bd, 0ul, 1)
        metal_set_buffer(enc, bsrc_hs, 0ul, 2)
        metal_set_buffer(enc, bdst_hs, 0ul, 3)
        metal_set_buffer(enc, btot, 0ul, 4)
        metal_dispatch_threadgroups(enc, uint3(uint((total + 63) / 64), 1u, 1u), uint3(64u, 1u, 1u))
    }
    t |> success(ran, "{tag}: encode: {err}")
    if (ran) {
        t |> equal(buf_mismatch_exact(bd, want, tag), 0)
    }
    metal_release(bs)
    metal_release(bd)
    metal_release(bsrc_hs)
    metal_release(bdst_hs)
    metal_release(btot)
    metal_release(pso)
    delete src
    delete want
}

// two-axis NEOX rope over token-major rows vs the CPU rope_neox_2d_rows on the same tables (the
// kernel reads the four tables packed per row as [cx|sx|cy|sy]); approx bar (fma ordering).
def private rope2d_gate(t : T?; dev, queue; npos, n_head, hs : int) {
    let d = n_head * hs
    let pairs = hs / 4   // rotated pairs per axis, one packed table row
    let tag = "rope2d npos={npos} heads={n_head} hs={hs}"
    var err : string
    var pso = pipeline_from_source(dev, metal_rope2d_msl, metal_rope2d_msl_entry, metal_rope2d_msl_fastmath, err)
    t |> success(pso != null, "{tag}: pipeline: {err}")
    return if (pso == null)
    var pos_x : array<int64>
    var pos_y : array<int64>
    pos_x |> resize(npos)
    pos_y |> resize(npos)
    for (i in range(npos)) {
        pos_x[i] = int64(i % 7)
        pos_y[i] = int64(i / 7)
    }
    var cx, sx, cy, sy : array<float>
    var no_ff : array<float>
    build_rope_tabs_rows(cx, sx, 100.0, 1.0, 1.0, no_ff, false, pos_x, int64(npos), int64(hs / 2))
    build_rope_tabs_rows(cy, sy, 100.0, 1.0, 1.0, no_ff, false, pos_y, int64(npos), int64(hs / 2))
    var x : array<float>
    x |> resize(npos * d)
    for (i in range(npos * d)) {
        x[i] = 0.13 * float(i % 29) - 1.7
    }
    var want := x
    rope_neox_2d_rows(want, int64(npos), int64(n_head), int64(hs), cx, sx, cy, sy)
    var tabs : array<float>
    tabs |> resize(npos * 4 * pairs)
    for (r in range(npos)) {
        for (j in range(pairs)) {
            tabs[r * 4 * pairs + j] = cx[r * pairs + j]
            tabs[r * 4 * pairs + pairs + j] = sx[r * pairs + j]
            tabs[r * 4 * pairs + 2 * pairs + j] = cy[r * pairs + j]
            tabs[r * 4 * pairs + 3 * pairs + j] = sy[r * pairs + j]
        }
    }
    let total = npos * n_head * pairs
    var bx = buf_upload(dev, x)
    var btabs = buf_upload(dev, tabs)
    var bd = buf_u32(dev, uint(d))
    var bpairs = buf_u32(dev, uint(pairs))
    var btot = buf_u32(dev, uint(total))
    let ran = with_compute_encoder(queue, err) $(enc : MetalComputeEncoder?) {
        metal_set_pipeline(enc, pso)
        metal_set_buffer(enc, bx, 0ul, 0)
        metal_set_buffer(enc, btabs, 0ul, 1)
        metal_set_buffer(enc, bd, 0ul, 2)
        metal_set_buffer(enc, bpairs, 0ul, 3)
        metal_set_buffer(enc, btot, 0ul, 4)
        metal_dispatch_threadgroups(enc, uint3(uint((total + 63) / 64), 1u, 1u), uint3(64u, 1u, 1u))
    }
    t |> success(ran, "{tag}: encode: {err}")
    if (ran) {
        t |> equal(buf_mismatch_approx(bx, want, tag), 0)
    }
    metal_release(bx)
    metal_release(btabs)
    metal_release(bd)
    metal_release(bpairs)
    metal_release(btot)
    metal_release(pso)
    delete x
    delete want
    delete tabs
}

// LayerNorm w+b vs the shipped CPU layernorm (dasllama_audio) — the tower/embedder norm.
// The bias fixture keeps |b| >= 0.1 so a bias-blind kernel cannot pass.
def private layernorm_gate(t : T?; dev, queue; dim, nrows : int; eps : float; inplace : bool) {
    let tag = "layernorm dim={dim} nrows={nrows} eps={eps} inplace={inplace}"
    var err : string
    var pso = pipeline_from_source(dev, metal_layernorm_msl, metal_layernorm_msl_entry, metal_layernorm_msl_fastmath, err)
    t |> success(pso != null, "{tag}: pipeline: {err}")
    return if (pso == null)
    var x : array<float>
    var w : array<float>
    var b : array<float>
    x |> resize(nrows * dim)
    w |> resize(dim)
    b |> resize(dim)
    for (i in range(nrows * dim)) {
        x[i] = 0.23 * float(i % 31) - 3.1
    }
    if (nrows > 0 && dim > 0) {
        for (i in range(dim)) {   // one constant row exercises the zero-variance eps path
            x[(nrows - 1) * dim + i] = 1.75
        }
    }
    for (i in range(dim)) {
        w[i] = 0.5 + 0.04 * float(i % 13)
        b[i] = -0.7 + 0.05 * float(i % 17)
    }
    var want : array<float>
    want |> resize(nrows * dim)
    unsafe {
        for (r in range(nrows)) {
            layernorm(addr(want[r * dim]), addr < float const? >(x[r * dim]),
                addr < float const? >(w[0]), addr < float const? >(b[0]), int64(dim), eps)
        }
    }
    var bx = buf_upload(dev, x)
    var bw = buf_upload(dev, w)
    var bb = buf_upload(dev, b)
    var by = inplace ? bx : buf_fill(dev, nrows * dim, 0.0)
    var bdim = buf_u32(dev, uint(dim))
    var beps = buf_f32(dev, eps)
    let ran = with_compute_encoder(queue, err) $(enc : MetalComputeEncoder?) {
        metal_set_pipeline(enc, pso)
        metal_set_threadgroup_memory_length(enc, metal_layernorm_msl_tgmem, 0)
        metal_set_buffer(enc, bx, 0ul, 0)
        metal_set_buffer(enc, bw, 0ul, 1)
        metal_set_buffer(enc, bb, 0ul, 2)
        metal_set_buffer(enc, by, 0ul, 3)
        metal_set_buffer(enc, bdim, 0ul, 4)
        metal_set_buffer(enc, beps, 0ul, 5)
        metal_dispatch_threadgroups(enc, uint3(uint(nrows), 1u, 1u), uint3(256u, 1u, 1u))
    }
    t |> success(ran, "{tag}: encode: {err}")
    if (ran) {
        t |> equal(buf_mismatch_approx(by, want, tag), 0)
    }
    metal_release(bx)
    metal_release(bw)
    metal_release(bb)
    if (!inplace) {
        metal_release(by)
    }
    metal_release(bdim)
    metal_release(beps)
    metal_release(pso)
    delete x
    delete w
    delete b
    delete want
}

// The two FORM-MATCHED tower GELUs vs their shipped CPU flavors on the same fixture. The LUT
// flavor runs the CPU's own clamps + f16 round-trips in kernel (bar = one f16 ulp at the range
// edge, tie flips only); the erf flavor's A&S polynomial sits within ~1e-5 of the CPU's
// double-precision erf. Saturation/NaN probes ride the tail slots of every arm.
def private gelu_flavor_gate(t : T?; dev, queue; total : int; lut : bool) {
    let tag = "gelu_{lut ? "lut" : "erf"} total={total}"
    var err : string
    var pso = (lut
        ? pipeline_from_source(dev, MetalGeluLut_metal_ew1_msl, MetalGeluLut_metal_ew1_msl_entry, MetalGeluLut_metal_ew1_msl_fastmath, err)
        : pipeline_from_source(dev, MetalGeluErf_metal_ew1_msl, MetalGeluErf_metal_ew1_msl_entry, MetalGeluErf_metal_ew1_msl_fastmath, err))
    t |> success(pso != null, "{tag}: pipeline: {err}")
    return if (pso == null)
    var g : array<float>
    g |> resize(total)
    for (i in range(total)) {
        g[i] = 0.21 * float(i % 43) - 4.3
    }
    if (total >= 8) {   // the saturation + zero probes ride the tail slots
        g[total - 1] = 50.0
        g[total - 2] = -50.0
        g[total - 3] = 100.0
        g[total - 4] = -100.0
        g[total - 5] = 0.0
        g[total - 6] = 1e-30
        g[total - 7] = -1e-30
    }
    var want := g
    if (lut) {
        gelu_tanh_lut(want, int64(total))
    } else {
        gelu_erf(want, int64(total))
    }
    var bg = buf_upload(dev, g)
    var btot = buf_u32(dev, uint(total))
    let ran = with_compute_encoder(queue, err) $(enc : MetalComputeEncoder?) {
        metal_set_pipeline(enc, pso)
        metal_set_buffer(enc, bg, 0ul, 0)
        metal_set_buffer(enc, btot, 0ul, 1)
        metal_dispatch_threadgroups(enc, uint3(uint((total + 63) / 64), 1u, 1u), uint3(64u, 1u, 1u))
    }
    t |> success(ran, "{tag}: encode: {err}")
    if (ran && total >= 8) {
        var back : array<float>
        back |> resize(total)
        unsafe {
            memcpy(addr(back[0]), metal_buffer_contents(bg), total * 4)
        }
        var maxd = 0.0
        for (bv, wv in back, want) {
            maxd = max(maxd, abs(bv - wv))
        }
        to_log(LOG_INFO, "{tag}: max |gpu - cpu flavor| = {maxd}\n")
        // LUT: one f16 ulp at |g|~10 (RNE tie flips between the tanh and exp-identity evals);
        // erf: two ~1.5e-7-accurate erf approximations (NR double vs A&S f32) plus f32 eval noise
        t |> success(maxd <= (lut ? 1e-2 : 1e-5), "{tag}: form-matched inside the flavor bar")
        t |> success(back[total - 1] == 50.0 && back[total - 3] == 100.0, "{tag}: positive saturation exact")
        t |> success(back[total - 2] == 0.0 && back[total - 4] == 0.0, "{tag}: negative saturation exact")
        var nan_found = false
        for (v in back) {
            if (v != v) {
                nan_found = true
            }
        }
        t |> success(!nan_found, "{tag}: no NaN anywhere")
        delete back
    }
    metal_release(bg)
    metal_release(btot)
    metal_release(pso)
    delete g
    delete want
}

// The form-delta cell: the analytic tanh-GELU vs the two shipped CPU flavors — the measured
// per-element distances whose 32-layer coherent amplification (~4e-2 rel_l2 on qwen2audio)
// is WHY the GPU ships form-matched flavors instead of one substituted form.
def private gelu_form_delta(t : T?) {
    let n = 65536
    var xs : array<float>
    var lut : array<float>
    var erfv : array<float>
    xs |> resize(n)
    lut |> resize(n)
    erfv |> resize(n)
    for (i in range(n)) {
        xs[i] = -10.0 + 20.0 * float(i) / 65535.0
        lut[i] = xs[i]
        erfv[i] = xs[i]
    }
    gelu_tanh_lut(lut, int64(n))   // the SHIPPED whisper flavor (f16-LUT round trip)
    gelu_erf(erfv, int64(n))       // the SHIPPED mtmd-family flavor
    var max_lut = 0.0
    var max_erf = 0.0
    for (i in range(n)) {
        let x = xs[i]
        let analytic = 0.5 * x * (1.0 + tanh(0.7978845608028654 * (x + 0.044715 * x * x * x)))
        max_lut = max(max_lut, abs(analytic - lut[i]))
        max_erf = max(max_erf, abs(analytic - erfv[i]))
    }
    to_log(LOG_INFO, "gelu_form_delta over [-10,10]: |analytic-lut| max {max_lut}, |analytic-erf| max {max_erf}\n")
    // measured 3.90625e-3 = the f16 HALF-ULP at the range edge (|x|~10, spacing 2^-7): the LUT
    // quantizes its INPUT, so the delta is absolute half-ulp * gelu' ~ 1, not relative
    t |> success(max_lut <= 4.5e-3, "gelu analytic-vs-LUT substitution bounded (measured {max_lut})")
    t |> success(max_erf <= 1e-3, "gelu analytic-vs-erf substitution bounded (measured {max_erf})")
    delete xs
    delete lut
    delete erfv
}

// Weightless rms via MetalRmsNorm + a ones vector at the meta eps — the gemma4uv chain's idiom.
def private rms_weightless_gate(t : T?; dev, queue; dim, nrows : int; eps : float) {
    let tag = "rms_weightless dim={dim} nrows={nrows}"
    var err : string
    var pso = pipeline_from_source(dev, metal_rmsnorm_msl, metal_rmsnorm_msl_entry, metal_rmsnorm_msl_fastmath, err)
    t |> success(pso != null, "{tag}: pipeline: {err}")
    return if (pso == null)
    var x : array<float>
    var ones : array<float>
    x |> resize(nrows * dim)
    ones |> resize(dim)
    for (i in range(nrows * dim)) {
        x[i] = 0.19 * float(i % 37) - 2.9
    }
    for (i in range(dim)) {
        ones[i] = 1.0
    }
    var want : array<float>
    want |> resize(nrows * dim)
    for (r in range(nrows)) {
        var ss = 0.0
        for (i in range(dim)) {
            let vv = x[r * dim + i]
            ss += vv * vv
        }
        let inv = 1.0 / sqrt(ss / float(dim) + eps)
        for (i in range(dim)) {
            want[r * dim + i] = x[r * dim + i] * inv
        }
    }
    var bx = buf_upload(dev, x)
    var bw = buf_upload(dev, ones)
    var by = buf_fill(dev, nrows * dim, 0.0)
    var bdim = buf_u32(dev, uint(dim))
    var beps = buf_f32(dev, eps)
    let ran = with_compute_encoder(queue, err) $(enc : MetalComputeEncoder?) {
        metal_set_pipeline(enc, pso)
        metal_set_threadgroup_memory_length(enc, metal_rmsnorm_msl_tgmem, 0)
        metal_set_buffer(enc, bx, 0ul, 0)
        metal_set_buffer(enc, bw, 0ul, 1)
        metal_set_buffer(enc, by, 0ul, 2)
        metal_set_buffer(enc, bdim, 0ul, 3)
        metal_set_buffer(enc, beps, 0ul, 4)
        metal_dispatch_threadgroups(enc, uint3(uint(nrows), 1u, 1u), uint3(256u, 1u, 1u))
    }
    t |> success(ran, "{tag}: encode: {err}")
    if (ran) {
        t |> equal(buf_mismatch_approx(by, want, tag), 0)
    }
    metal_release(bx)
    metal_release(bw)
    metal_release(by)
    metal_release(bdim)
    metal_release(beps)
    metal_release(pso)
    delete x
    delete ones
    delete want
}

// The gemma4uv 2-axis position add — bit-EXACT vs the CPU's (row + ex) + ey order; the
// split-exponent fixture makes any reassociation visible, the non-square grid any axis swap.
def private pos_add_gate(t : T?; dev, queue; npos, dim, ncols, pos_entries : int) {
    let tag = "posadd2d npos={npos} ncols={ncols}"
    var err : string
    var pso = pipeline_from_source(dev, metal_posadd2d_msl, metal_posadd2d_msl_entry, metal_posadd2d_msl_fastmath, err)
    t |> success(pso != null, "{tag}: pipeline: {err}")
    return if (pso == null)
    let total = npos * dim
    var x : array<float>
    var tbl : array<float>
    x |> resize(total)
    tbl |> resize(2 * pos_entries * dim)
    for (i in range(total)) {
        x[i] = 1.0e3 + 0.37 * float(i % 29)          // large exponent
    }
    for (i in range(pos_entries * dim)) {
        tbl[i] = 1.0e-3 * float(i % 41 + 1)          // x-table rows: tiny exponent
        tbl[pos_entries * dim + i] = 9.0e2 + 0.21 * float(i % 19)   // y-table rows: large again
    }
    var want : array<float>
    want |> resize(total)
    for (p in range(npos)) {
        for (i in range(dim)) {
            let ex = tbl[(p % ncols) * dim + i]
            let ey = tbl[(pos_entries + p / ncols) * dim + i]
            want[p * dim + i] = (x[p * dim + i] + ex) + ey
        }
    }
    var bx = buf_upload(dev, x)
    var btbl = buf_upload(dev, tbl)
    var bdim = buf_u32(dev, uint(dim))
    var bcols = buf_u32(dev, uint(ncols))
    var bpe = buf_u32(dev, uint(pos_entries))
    var btot = buf_u32(dev, uint(total))
    let ran = with_compute_encoder(queue, err) $(enc : MetalComputeEncoder?) {
        metal_set_pipeline(enc, pso)
        metal_set_buffer(enc, bx, 0ul, 0)
        metal_set_buffer(enc, btbl, 0ul, 1)
        metal_set_buffer(enc, bdim, 0ul, 2)
        metal_set_buffer(enc, bcols, 0ul, 3)
        metal_set_buffer(enc, bpe, 0ul, 4)
        metal_set_buffer(enc, btot, 0ul, 5)
        metal_dispatch_threadgroups(enc, uint3(uint((total + 63) / 64), 1u, 1u), uint3(64u, 1u, 1u))
    }
    t |> success(ran, "{tag}: encode: {err}")
    if (ran) {
        t |> equal(buf_mismatch_exact(bx, want, tag), 0)
    }
    metal_release(bx)
    metal_release(btbl)
    metal_release(bdim)
    metal_release(bcols)
    metal_release(bpe)
    metal_release(btot)
    metal_release(pso)
    delete x
    delete tbl
    delete want
}

// The whisper cross-KV shuffles — pure scatter (+ one multiply for kx), bit-EXACT vs the CPU
// walk. The offset leg writes the SECOND layer slot and proves the first stays untouched.
// Negative controls recorded 2026-08-16: dropped kscale in the kx oracle and swapped t/j in
// the vx oracle each redded with dumps (the kx mismatch ratio was exactly kscale).
def private cross_kx_gate(t : T?; dev, queue; ta, dim, hh : int; off_layer : bool) {
    let tag = "cross_kx ta={ta} dim={dim} hh={hh} off={off_layer}"
    var err : string
    var pso = pipeline_from_source(dev, metal_cross_kx_msl, metal_cross_kx_msl_entry, metal_cross_kx_msl_fastmath, err)
    t |> success(pso != null, "{tag}: pipeline: {err}")
    return if (pso == null)
    let hs = dim / hh
    let total = ta * dim
    let kscale = 1.0 / sqrt(sqrt(float(hs)))
    var src : array<float>
    src |> resize(total)
    for (i in range(total)) {
        src[i] = 0.25 * float(i % 97) - 3.0
    }
    let sentinel = -777.0
    var want : array<float>
    want |> resize(2 * total)
    for (i in range(2 * total)) {
        want[i] = sentinel
    }
    let base = off_layer ? total : 0
    for (h in range(hh)) {
        for (j in range(hs)) {
            for (tt in range(ta)) {
                want[base + (h * hs + j) * ta + tt] = src[tt * dim + h * hs + j] * kscale
            }
        }
    }
    var bsrc = buf_upload(dev, src)
    var bkx = buf_fill(dev, 2 * total, sentinel)
    var bdim = buf_u32(dev, uint(dim))
    var bhs = buf_u32(dev, uint(hs))
    var bta = buf_u32(dev, uint(ta))
    var bks = buf_f32(dev, kscale)
    var btot = buf_u32(dev, uint(total))
    let ran = with_compute_encoder(queue, err) $(enc : MetalComputeEncoder?) {
        metal_set_pipeline(enc, pso)
        metal_set_buffer(enc, bsrc, 0ul, 0)
        metal_set_buffer(enc, bkx, uint64(base * 4), 1)
        metal_set_buffer(enc, bdim, 0ul, 2)
        metal_set_buffer(enc, bhs, 0ul, 3)
        metal_set_buffer(enc, bta, 0ul, 4)
        metal_set_buffer(enc, bks, 0ul, 5)
        metal_set_buffer(enc, btot, 0ul, 6)
        metal_dispatch_threadgroups(enc, uint3(uint((total + 63) / 64), 1u, 1u), uint3(64u, 1u, 1u))
    }
    t |> success(ran, "{tag}: encode: {err}")
    if (ran) {
        t |> equal(buf_mismatch_exact(bkx, want, tag), 0)
    }
    metal_release(bsrc)
    metal_release(bkx)
    metal_release(bdim)
    metal_release(bhs)
    metal_release(bta)
    metal_release(bks)
    metal_release(btot)
    metal_release(pso)
    delete src
    delete want
}

// The Vx form doubles as the KV APPEND: the offset leg writes n rows at t0 into a wider
// tstride slab (sentinel-checked whole), the bias leg folds a per-column bias, and soff picks
// a column block out of a fused-QKV row (src bound at a byte offset with the fused row stride).
def private cross_vx_gate(t : T?; dev, queue; ta, dim, hh : int; t0, tstride : int; hasb : bool; soff_cols : int = 0; src_stride : int = 0) {
    let tag = "cross_vx ta={ta} dim={dim} hh={hh} t0={t0} hasb={hasb} soff={soff_cols}"
    var err : string
    var pso = pipeline_from_source(dev, metal_cross_vx_msl, metal_cross_vx_msl_entry, metal_cross_vx_msl_fastmath, err)
    t |> success(pso != null, "{tag}: pipeline: {err}")
    return if (pso == null)
    let hs = dim / hh
    let sstride = src_stride > 0 ? src_stride : dim
    let total = ta * dim
    let sentinel = -555.0
    var src : array<float>
    src |> resize(ta * sstride)
    for (i in range(ta * sstride)) {
        src[i] = 0.5 * float(i % 89) - 11.0
    }
    var bias : array<float>
    bias |> resize(dim)
    for (i in range(dim)) {
        bias[i] = 0.125 * float(i % 13) - 0.5
    }
    let slab = hh * tstride * hs
    var want : array<float>
    want |> resize(slab)
    for (i in range(slab)) {
        want[i] = sentinel
    }
    for (h in range(hh)) {
        for (tt in range(ta)) {
            for (j in range(hs)) {
                let v = src[tt * sstride + soff_cols + h * hs + j] + (hasb ? bias[h * hs + j] : 0.0)
                want[h * tstride * hs + (t0 + tt) * hs + j] = v
            }
        }
    }
    var bsrc = buf_upload(dev, src)
    var bvx = buf_fill(dev, slab, sentinel)
    var bb = buf_upload(dev, bias)
    var bdim = buf_u32(dev, uint(sstride))
    var bhs = buf_u32(dev, uint(hs))
    var bta = buf_u32(dev, uint(ta))
    var bt0 = buf_u32(dev, uint(t0))
    var bts = buf_u32(dev, uint(tstride))
    var bhb = buf_u32(dev, hasb ? 1u : 0u)
    var btot = buf_u32(dev, uint(total))
    let ran = with_compute_encoder(queue, err) $(enc : MetalComputeEncoder?) {
        metal_set_pipeline(enc, pso)
        metal_set_buffer(enc, bsrc, uint64(soff_cols * 4), 0)
        metal_set_buffer(enc, bvx, 0ul, 1)
        metal_set_buffer(enc, bb, 0ul, 2)
        metal_set_buffer(enc, bdim, 0ul, 3)
        metal_set_buffer(enc, bhs, 0ul, 4)
        metal_set_buffer(enc, bta, 0ul, 5)
        metal_set_buffer(enc, bt0, 0ul, 6)
        metal_set_buffer(enc, bts, 0ul, 7)
        metal_set_buffer(enc, bhb, 0ul, 8)
        metal_set_buffer(enc, btot, 0ul, 9)
        metal_dispatch_threadgroups(enc, uint3(uint((total + 63) / 64), 1u, 1u), uint3(64u, 1u, 1u))
    }
    t |> success(ran, "{tag}: encode: {err}")
    if (ran) {
        t |> equal(buf_mismatch_exact(bvx, want, tag), 0)
    }
    metal_release(bsrc)
    metal_release(bvx)
    metal_release(bb)
    metal_release(bdim)
    metal_release(bhs)
    metal_release(bta)
    metal_release(bt0)
    metal_release(bts)
    metal_release(bhb)
    metal_release(btot)
    metal_release(pso)
    delete src
    delete bias
    delete want
}

// The chunked twin pair vs the SAME oracle: Part writes per-chunk (m, s, y) partials, Comb
// merges by log-sum-exp — the pair must land on the single-pass answer (1e-4, exp reassoc).
// Negative control recorded 2026-08-16: an off-by-one oracle len redded all three cells.
def private wdec_attn_pc_gate(t : T?; dev, queue; n, dim, hh, tstride, len0 : int; causal : bool) {
    let tag = "wdec_attn_pc n={n} dim={dim} hh={hh} len0={len0} causal={causal}"
    var err : string
    var pp = pipeline_from_source(dev, metal_wdec_attn_part_msl, metal_wdec_attn_part_msl_entry, metal_wdec_attn_part_msl_fastmath, err)
    t |> success(pp != null, "{tag}: part pipeline: {err}")
    return if (pp == null)
    var pc = pipeline_from_source(dev, metal_wdec_attn_comb_msl, metal_wdec_attn_comb_msl_entry, metal_wdec_attn_comb_msl_fastmath, err)
    t |> success(pc != null, "{tag}: comb pipeline: {err}")
    if (pc == null) {
        metal_release(pp)
        return
    }
    let hs = dim / hh
    let scale = 1.0 / sqrt(float(hs))
    let maxlen = causal ? len0 + n - 1 : len0
    let nch = (maxlen + 255) / 256
    var q : array<float>
    var qb : array<float>
    var kvh : array<uint16>
    var vvh : array<uint16>
    var kv : array<float>
    var vv : array<float>
    q |> resize(n * dim)
    qb |> resize(dim)
    kvh |> resize(hh * tstride * hs)
    vvh |> resize(hh * tstride * hs)
    kv |> resize(hh * tstride * hs)
    vv |> resize(hh * tstride * hs)
    for (i in range(n * dim)) {
        q[i] = 0.11 * float(i % 37) - 1.7
    }
    for (i in range(dim)) {
        qb[i] = 0.07 * float(i % 11) - 0.3
    }
    for (i in range(hh * tstride * hs)) {
        kvh[i] = uint16(f32_to_f16(0.09 * float(i % 41) - 1.9))
        vvh[i] = uint16(f32_to_f16(0.13 * float(i % 29) - 1.1))
        kv[i] = f16_to_f32(uint(kvh[i]))
        vv[i] = f16_to_f32(uint(vvh[i]))
    }
    var want : array<float>
    want |> resize(n * dim)
    var scores : array<float>
    scores |> resize(tstride)
    for (h in range(hh)) {
        for (r in range(n)) {
            let len = causal ? len0 + r : len0
            var mx = -3.0e38
            for (tt in range(len)) {
                var sacc = 0.0
                for (j in range(hs)) {
                    sacc += (q[r * dim + h * hs + j] + qb[h * hs + j]) * kv[(h * tstride + tt) * hs + j]
                }
                scores[tt] = sacc * scale
                mx = max(mx, scores[tt])
            }
            var tot = 0.0
            for (tt in range(len)) {
                scores[tt] = exp(scores[tt] - mx)
                tot += scores[tt]
            }
            for (j in range(hs)) {
                var acc = 0.0
                for (tt in range(len)) {
                    acc += scores[tt] * vv[(h * tstride + tt) * hs + j]
                }
                want[r * dim + h * hs + j] = acc / tot
            }
        }
    }
    var bq = buf_upload(dev, q)
    var bk = buf_upload(dev, kvh)
    var bv = buf_upload(dev, vvh)
    var bqb = buf_upload(dev, qb)
    var bpms = buf_fill(dev, nch * hh * n * 2, -888.0)
    var bpy = buf_fill(dev, nch * hh * n * hs, -888.0)
    var by = buf_fill(dev, n * dim, -444.0)
    var bdim = buf_u32(dev, uint(dim))
    var bhs = buf_u32(dev, uint(hs))
    var bts = buf_u32(dev, uint(tstride))
    var blen = buf_u32(dev, uint(len0))
    var bca = buf_u32(dev, causal ? 1u : 0u)
    var bsc = buf_f32(dev, scale)
    var bhh = buf_u32(dev, uint(hh))
    var bn = buf_u32(dev, uint(n))
    var bnch = buf_u32(dev, uint(nch))
    let ran = with_compute_encoder(queue, err) $(enc : MetalComputeEncoder?) {
        metal_set_pipeline(enc, pp)
        metal_set_threadgroup_memory_length(enc, metal_wdec_attn_part_msl_tgmem, 0)
        metal_set_buffer(enc, bq, 0ul, 0)
        metal_set_buffer(enc, bk, 0ul, 1)
        metal_set_buffer(enc, bv, 0ul, 2)
        metal_set_buffer(enc, bpms, 0ul, 3)
        metal_set_buffer(enc, bpy, 0ul, 4)
        metal_set_buffer(enc, bqb, 0ul, 5)
        metal_set_buffer(enc, bdim, 0ul, 6)
        metal_set_buffer(enc, bhs, 0ul, 7)
        metal_set_buffer(enc, bts, 0ul, 8)
        metal_set_buffer(enc, blen, 0ul, 9)
        metal_set_buffer(enc, bca, 0ul, 10)
        metal_set_buffer(enc, bsc, 0ul, 11)
        metal_set_buffer(enc, bhh, 0ul, 12)
        metal_set_buffer(enc, bn, 0ul, 13)
        metal_dispatch_threadgroups(enc, uint3(uint(hh), uint(n), uint(nch)), uint3(128u, 1u, 1u))
        metal_memory_barrier(enc)
        metal_set_pipeline(enc, pc)
        metal_set_buffer(enc, bpms, 0ul, 0)
        metal_set_buffer(enc, bpy, 0ul, 1)
        metal_set_buffer(enc, by, 0ul, 2)
        metal_set_buffer(enc, bdim, 0ul, 3)
        metal_set_buffer(enc, bhs, 0ul, 4)
        metal_set_buffer(enc, bnch, 0ul, 5)
        metal_set_buffer(enc, bhh, 0ul, 6)
        metal_set_buffer(enc, bn, 0ul, 7)
        metal_dispatch_threadgroups(enc, uint3(uint(hh), uint(n), 1u), uint3(64u, 1u, 1u))
    }
    t |> success(ran, "{tag}: encode: {err}")
    if (ran) {
        t |> equal(buf_mismatch_approx(by, want, tag), 0)
    }
    metal_release(bq)
    metal_release(bk)
    metal_release(bv)
    metal_release(bqb)
    metal_release(bpms)
    metal_release(bpy)
    metal_release(by)
    metal_release(bdim)
    metal_release(bhs)
    metal_release(bts)
    metal_release(blen)
    metal_release(bca)
    metal_release(bsc)
    metal_release(bhh)
    metal_release(bn)
    metal_release(bnch)
    metal_release(pp)
    metal_release(pc)
    delete q
    delete qb
    delete kvh
    delete vvh
    delete kv
    delete vv
    delete want
    delete scores
}

def private bias_add_res_gate(t : T?; dev, queue; nrows, dim : int) {
    let tag = "bias_add_res nrows={nrows} dim={dim}"
    var err : string
    var pso = pipeline_from_source(dev, metal_bias_add_res_msl, metal_bias_add_res_msl_entry, metal_bias_add_res_msl_fastmath, err)
    t |> success(pso != null, "{tag}: pipeline: {err}")
    return if (pso == null)
    let total = nrows * dim
    var x : array<float>
    var y : array<float>
    var b : array<float>
    x |> resize(total)
    y |> resize(total)
    b |> resize(dim)
    for (i in range(total)) {
        x[i] = 0.3 * float(i % 23) - 2.0
        y[i] = 0.17 * float(i % 31) - 1.0
    }
    for (i in range(dim)) {
        b[i] = 0.05 * float(i % 7) - 0.1
    }
    var want : array<float>
    want |> resize(total)
    for (i in range(total)) {
        want[i] = x[i] + (y[i] + b[i % dim])
    }
    var bx = buf_upload(dev, x)
    var by = buf_upload(dev, y)
    var bb = buf_upload(dev, b)
    var bdim = buf_u32(dev, uint(dim))
    var btot = buf_u32(dev, uint(total))
    let ran = with_compute_encoder(queue, err) $(enc : MetalComputeEncoder?) {
        metal_set_pipeline(enc, pso)
        metal_set_buffer(enc, bx, 0ul, 0)
        metal_set_buffer(enc, by, 0ul, 1)
        metal_set_buffer(enc, bb, 0ul, 2)
        metal_set_buffer(enc, bdim, 0ul, 3)
        metal_set_buffer(enc, btot, 0ul, 4)
        metal_dispatch_threadgroups(enc, uint3(uint((total + 63) / 64), 1u, 1u), uint3(64u, 1u, 1u))
    }
    t |> success(ran, "{tag}: encode: {err}")
    if (ran) {
        t |> equal(buf_mismatch_approx(bx, want, tag), 0)
    }
    metal_release(bx)
    metal_release(by)
    metal_release(bb)
    metal_release(bdim)
    metal_release(btot)
    metal_release(pso)
    delete x
    delete y
    delete b
    delete want
}

def private bias_gelu_lut_gate(t : T?; dev, queue; nrows, dim : int) {
    let tag = "bias_gelu_lut nrows={nrows} dim={dim}"
    var err : string
    var pso = pipeline_from_source(dev, metal_bias_gelu_lut_msl, metal_bias_gelu_lut_msl_entry, metal_bias_gelu_lut_msl_fastmath, err)
    t |> success(pso != null, "{tag}: pipeline: {err}")
    return if (pso == null)
    let total = nrows * dim
    var x : array<float>
    var b : array<float>
    x |> resize(total)
    b |> resize(dim)
    for (i in range(total)) {
        x[i] = 0.02 * float(i % 512) - 5.0   // spans the LUT clamp on both sides
    }
    for (i in range(dim)) {
        b[i] = 0.03 * float(i % 17) - 0.25
    }
    var want : array<float>
    want |> resize(total)
    for (i in range(total)) {
        want[i] = x[i] + b[i % dim]
    }
    gelu_tanh_lut(want, int64(total))   // the CPU LUT form — the form-matched oracle
    var bx = buf_upload(dev, x)
    var bb = buf_upload(dev, b)
    var bdim = buf_u32(dev, uint(dim))
    var btot = buf_u32(dev, uint(total))
    let ran = with_compute_encoder(queue, err) $(enc : MetalComputeEncoder?) {
        metal_set_pipeline(enc, pso)
        metal_set_buffer(enc, bx, 0ul, 0)
        metal_set_buffer(enc, bb, 0ul, 1)
        metal_set_buffer(enc, bdim, 0ul, 2)
        metal_set_buffer(enc, btot, 0ul, 3)
        metal_dispatch_threadgroups(enc, uint3(uint((total + 63) / 64), 1u, 1u), uint3(64u, 1u, 1u))
    }
    t |> success(ran, "{tag}: encode: {err}")
    if (ran) {
        t |> equal(buf_mismatch_approx(bx, want, tag), 0)
    }
    metal_release(bx)
    metal_release(bb)
    metal_release(bdim)
    metal_release(btot)
    metal_release(pso)
    delete x
    delete b
    delete want
}

// The conv-frontend im2col vs the CPU walk — bit-exact scatter with the "same"-pad zero
// edges; both source layouts (chmajor mel vs row-major y1) and both strides.
// Negative control recorded 2026-08-16: dropping the -1 pad shift redded all four cells.
def private im2col3_gate(t : T?; dev, queue; rows, nch, srclen, stride : int; chmajor : bool) {
    let tag = "im2col3 rows={rows} nch={nch} stride={stride} chmajor={chmajor}"
    var err : string
    var pso = pipeline_from_source(dev, metal_im2col3_msl, metal_im2col3_msl_entry, metal_im2col3_msl_fastmath, err)
    t |> success(pso != null, "{tag}: pipeline: {err}")
    return if (pso == null)
    let total = rows * 3 * nch
    var src : array<float>
    src |> resize(srclen * nch)
    for (i in range(srclen * nch)) {
        src[i] = 0.21 * float(i % 53) - 4.0
    }
    var want : array<float>
    want |> resize(total)
    for (i in range(rows)) {
        for (dk in range(3)) {
            let sp = i * stride + dk - 1
            for (c in range(nch)) {
                want[(i * 3 + dk) * nch + c] = (sp >= 0 && sp < srclen
                    ? (chmajor ? src[c * srclen + sp] : src[sp * nch + c])
                    : 0.0)
            }
        }
    }
    var bsrc = buf_upload(dev, src)
    var bcol = buf_fill(dev, total, -333.0)
    var bnch = buf_u32(dev, uint(nch))
    var bsl = buf_u32(dev, uint(srclen))
    var bst = buf_u32(dev, uint(stride))
    var bch = buf_u32(dev, chmajor ? 1u : 0u)
    var btot = buf_u32(dev, uint(total))
    let ran = with_compute_encoder(queue, err) $(enc : MetalComputeEncoder?) {
        metal_set_pipeline(enc, pso)
        metal_set_buffer(enc, bsrc, 0ul, 0)
        metal_set_buffer(enc, bcol, 0ul, 1)
        metal_set_buffer(enc, bnch, 0ul, 2)
        metal_set_buffer(enc, bsl, 0ul, 3)
        metal_set_buffer(enc, bst, 0ul, 4)
        metal_set_buffer(enc, bch, 0ul, 5)
        metal_set_buffer(enc, btot, 0ul, 6)
        metal_dispatch_threadgroups(enc, uint3(uint((total + 63) / 64), 1u, 1u), uint3(64u, 1u, 1u))
    }
    t |> success(ran, "{tag}: encode: {err}")
    if (ran) {
        t |> equal(buf_mismatch_exact(bcol, want, tag), 0)
    }
    metal_release(bsrc)
    metal_release(bcol)
    metal_release(bnch)
    metal_release(bsl)
    metal_release(bst)
    metal_release(bch)
    metal_release(btot)
    metal_release(pso)
    delete src
    delete want
}

// The 2D conv im2col (k=3 s=2 p=1) vs the CPU conv2d_s2 walk — bit-exact scatter with the
// zero pad edges, the K pad (kd > cin*9) and the strided source (sstride > cin, the padded
// GEMM rows). Negative control recorded 2026-08-16: dropping the -1 pad shift redded every cell.
def private im2col2d_gate(t : T?; dev, queue; iw, ih, cin, kd, sstride : int) {
    let tag = "im2col2d iw={iw} ih={ih} cin={cin} kd={kd} ss={sstride}"
    var err : string
    var pso = pipeline_from_source(dev, metal_im2col2d_msl, metal_im2col2d_msl_entry, metal_im2col2d_msl_fastmath, err)
    t |> success(pso != null, "{tag}: pipeline: {err}")
    return if (pso == null)
    let ow = (iw - 1) / 2 + 1
    let oh = (ih - 1) / 2 + 1
    let rows = oh * ow
    let total = rows * kd
    var src : array<float>
    src |> resize(ih * iw * sstride)
    for (i in range(ih * iw * sstride)) {
        src[i] = 0.31 * float(i % 47) - 6.0
    }
    var want : array<float>
    want |> resize(total)
    for (oy in range(oh)) {
        for (ox in range(ow)) {
            let r = oy * ow + ox
            for (k in range(kd)) {
                var v = 0.0
                if (k < cin * 9) {
                    let ic = k / 9
                    let kh = (k % 9) / 3
                    let kw = k % 3
                    let iy = oy * 2 + kh - 1
                    let ix = ox * 2 + kw - 1
                    if (iy >= 0 && iy < ih && ix >= 0 && ix < iw) {
                        v = src[(iy * iw + ix) * sstride + ic]
                    }
                }
                want[r * kd + k] = v
            }
        }
    }
    var bsrc = buf_upload(dev, src)
    var bcol = buf_fill(dev, total, -222.0)
    var biw = buf_u32(dev, uint(iw))
    var bih = buf_u32(dev, uint(ih))
    var bcin = buf_u32(dev, uint(cin))
    var bow = buf_u32(dev, uint(ow))
    var bkd = buf_u32(dev, uint(kd))
    var bss = buf_u32(dev, uint(sstride))
    var btot = buf_u32(dev, uint(total))
    let ran = with_compute_encoder(queue, err) $(enc : MetalComputeEncoder?) {
        metal_set_pipeline(enc, pso)
        metal_set_buffer(enc, bsrc, 0ul, 0)
        metal_set_buffer(enc, bcol, 0ul, 1)
        metal_set_buffer(enc, biw, 0ul, 2)
        metal_set_buffer(enc, bih, 0ul, 3)
        metal_set_buffer(enc, bcin, 0ul, 4)
        metal_set_buffer(enc, bow, 0ul, 5)
        metal_set_buffer(enc, bkd, 0ul, 6)
        metal_set_buffer(enc, bss, 0ul, 7)
        metal_set_buffer(enc, btot, 0ul, 8)
        metal_dispatch_threadgroups(enc, uint3(uint((total + 63) / 64), 1u, 1u), uint3(64u, 1u, 1u))
    }
    t |> success(ran, "{tag}: encode: {err}")
    if (ran) {
        t |> equal(buf_mismatch_exact(bcol, want, tag), 0)
    }
    metal_release(bsrc)
    metal_release(bcol)
    metal_release(biw)
    metal_release(bih)
    metal_release(bcin)
    metal_release(bow)
    metal_release(bkd)
    metal_release(bss)
    metal_release(btot)
    metal_release(pso)
    delete src
    delete want
}

def private suppress_row_gate(t : T?; dev, queue; total, nids : int) {
    let tag = "suppress_row total={total} nids={nids}"
    var err : string
    var pso = pipeline_from_source(dev, metal_suppress_row_msl, metal_suppress_row_msl_entry, metal_suppress_row_msl_fastmath, err)
    t |> success(pso != null, "{tag}: pipeline: {err}")
    return if (pso == null)
    var xv : array<float>
    xv |> resize(total)
    for (i in range(total)) {
        xv[i] = 0.19 * float(i % 53) - 4.9
    }
    var ids : array<uint>
    ids |> resize(nids)
    for (i in range(nids)) {
        ids[i] = uint((i * 37 + 5) % total)
    }
    var want := xv
    for (i in range(nids)) {
        want[int(ids[i])] = -1.0e30
    }
    var bx = buf_upload(dev, xv)
    var bids = buf_upload(dev, ids)
    var bn = buf_u32(dev, uint(nids))
    let ran = with_compute_encoder(queue, err) $(enc : MetalComputeEncoder?) {
        metal_set_pipeline(enc, pso)
        metal_set_buffer(enc, bx, 0ul, 0)
        metal_set_buffer(enc, bids, 0ul, 1)
        metal_set_buffer(enc, bn, 0ul, 2)
        metal_dispatch_threadgroups(enc, uint3(uint((nids + 63) / 64), 1u, 1u), uint3(64u, 1u, 1u))
    }
    t |> success(ran, "{tag}: encode: {err}")
    if (ran) {
        t |> equal(buf_mismatch_exact(bx, want, tag), 0)
    }
    metal_release(bx)
    metal_release(bids)
    metal_release(bn)
    metal_release(pso)
    delete xv
    delete ids
    delete want
}

def private softcap_row_gate(t : T?; dev, queue; total : int; cap : float) {
    let tag = "softcap_row total={total} cap={cap}"
    var err : string
    var pso = pipeline_from_source(dev, metal_softcap_row_msl, metal_softcap_row_msl_entry, metal_softcap_row_msl_fastmath, err)
    t |> success(pso != null, "{tag}: pipeline: {err}")
    return if (pso == null)
    var xv : array<float>
    xv |> resize(total)
    for (i in range(total)) {
        xv[i] = 0.31 * float(i % 61) - 9.2   // spans well past +-cap so the saturation arms run
    }
    var want : array<float>
    want |> resize(total)
    for (i in range(total)) {
        want[i] = cap * tanh(xv[i] / cap)    // softcap4's scalar reference
    }
    var bx = buf_upload(dev, xv)
    var btot = buf_u32(dev, uint(total))
    var bcap = buf_f32(dev, cap)
    let ran = with_compute_encoder(queue, err) $(enc : MetalComputeEncoder?) {
        metal_set_pipeline(enc, pso)
        metal_set_buffer(enc, bx, 0ul, 0)
        metal_set_buffer(enc, btot, 0ul, 1)
        metal_set_buffer(enc, bcap, 0ul, 2)
        metal_dispatch_threadgroups(enc, uint3(uint((total + 63) / 64), 1u, 1u), uint3(64u, 1u, 1u))
    }
    t |> success(ran, "{tag}: encode: {err}")
    if (ran) {
        t |> equal(buf_mismatch_approx(bx, want, tag), 0)
    }
    metal_release(bx)
    metal_release(btot)
    metal_release(bcap)
    metal_release(pso)
    delete xv
    delete want
}

// The [metal_dispatch] multi-kernel mechanism, end to end: MtlToy's two kernel= instances
// dispatch through their GENERATED builders (kn_ rail, not hand binds) chained in one encoder
// — scale (ys = xs*gain) then shift (ys += gain). The per-kernel STAGING divergence (shift
// stages no xs) is proven at compile time — ast_dump of the builders + the lens cover rule;
// the hazard rail is un-armed in suites, so this gate witnesses binds/kargs/grid, not staging.
def private mtl_toy_lens_gate(t : T?; dev, queue; n : int) {
    let tag = "mtl_toy multi-kernel n={n}"
    var err : string
    g_mtl_toy_scale_pso = pipeline_from_source(dev, mtl_toy_scale_msl, mtl_toy_scale_msl_entry, mtl_toy_scale_msl_fastmath, err)
    t |> success(g_mtl_toy_scale_pso != null, "{tag}: scale pipeline: {err}")
    g_mtl_toy_shift_pso = pipeline_from_source(dev, mtl_toy_shift_msl, mtl_toy_shift_msl_entry, mtl_toy_shift_msl_fastmath, err)
    t |> success(g_mtl_toy_shift_pso != null, "{tag}: shift pipeline: {err}")
    if (g_mtl_toy_scale_pso == null || g_mtl_toy_shift_pso == null) {
        metal_release(g_mtl_toy_scale_pso)   // release-null is a no-op; the survivor must not leak
        metal_release(g_mtl_toy_shift_pso)
        g_mtl_toy_scale_pso = null
        g_mtl_toy_shift_pso = null
        return
    }
    let gain = 0.5
    var xv : array<float>
    xv |> resize(n)
    for (i in range(n)) {
        xv[i] = 0.11 * float(i % 73) - 4.2
    }
    var want : array<float>
    want |> resize(n + 16)
    for (i in range(n + 16)) {
        want[i] = i < n ? xv[i] * gain + gain : -1000.0   // the ka.n guard must leave the pad untouched
    }
    var bxs = buf_upload(dev, xv)
    var bys = buf_fill(dev, n + 16, -1000.0)
    var ka = MtToyArgs(n = uint(n), gain = gain)
    let ran = with_compute_encoder(queue, err) $(enc : MetalComputeEncoder?) {
        enc_mtl_toy_scale(enc, bxs, bys, ka, int64(n))
        enc_mtl_toy_shift(enc, bxs, bys, ka, int64(n))
    }
    t |> success(ran, "{tag}: encode: {err}")
    if (ran) {
        t |> equal(buf_mismatch_exact(bys, want, tag), 0)
    }
    metal_release(bxs)
    metal_release(bys)
    metal_release(g_mtl_toy_scale_pso)
    metal_release(g_mtl_toy_shift_pso)
    g_mtl_toy_scale_pso = null
    g_mtl_toy_shift_pso = null
    delete xv
    delete want
}

[test]
def test_metal_misc_kernels(t : T?) {
    t |> run("decode elementwise/misc kernels == direct CPU oracles") @(t : T?) {
        static_if (typeinfo builtin_module_exists(das_metal)) {
            with_metal_device() $(dev : MetalDevice?) {
                if (dev == null) {
                    feint("no Metal device on this box; GPU kernel checks skipped\n")
                    return
                }
                var queue = metal_new_command_queue(dev)
                copy_row_gate(t, dev, queue, 1000)
                argmax_gate(t, dev, queue, 5000, 4321, 1234)    // exact tie — lower index wins
                argmax_gate(t, dev, queue, 700, 0, 699)         // winner at index 0
                embed_q8_gate(t, dev, queue, 512, 3, 2)
                embed_k6_gate(t, dev, queue, 512, 3, 1)
                add_rms_gate(t, dev, queue, false, 1500, 1, 1.0)
                add_rms_gate(t, dev, queue, false, 1024, 1, 0.5)    // the gemma4 layer_out_scale slot
                add_rms_gate(t, dev, queue, true, 1500, 3, 1.0)
                pre_add_rms_gate(t, dev, queue, 1500)
                qk_norm_gate(t, dev, queue, 64, 4, 2, 2, false)     // in-place decode shape
                qk_norm_gate(t, dev, queue, 64, 4, 2, 2, true)      // the no-wv V-fusion shape
                qk_norm_gate(t, dev, queue, 128, 2, 1, 1, false)
                geglu_gate(t, dev, queue, 800)

                // ===== tower primitives =====
                layernorm_gate(t, dev, queue, 384, 4, 1.0e-5, false)    // whisper-tiny d; strided tail
                layernorm_gate(t, dev, queue, 896, 2, 1.0e-5, false)    // qwen3a d
                layernorm_gate(t, dev, queue, 1280, 2, 1.0e-5, false)   // whisper-large d
                layernorm_gate(t, dev, queue, 3840, 3, 1.0e-5, false)   // gemma4uv n_embd
                layernorm_gate(t, dev, queue, 6912, 3, 1.0e-5, false)   // gemma4uv patch_dim - widest row
                layernorm_gate(t, dev, queue, 1280, 1, 1.0e-5, false)   // single-row grid
                layernorm_gate(t, dev, queue, 100, 2, 1.0e-5, false)    // smaller than a simdgroup stride
                layernorm_gate(t, dev, queue, 1280, 2, 1.0e-3, false)   // non-default eps: the uniform is live
                layernorm_gate(t, dev, queue, 1280, 2, 1.0e-5, true)    // IN PLACE - the gemma4uv production shape
                gelu_flavor_gate(t, dev, queue, 800, true)     // %64 != 0 tail guard
                gelu_flavor_gate(t, dev, queue, 1000, true)
                gelu_flavor_gate(t, dev, queue, 5120, true)    // whisper-large n_ff
                gelu_flavor_gate(t, dev, queue, 800, false)
                gelu_flavor_gate(t, dev, queue, 5120, false)
                gelu_form_delta(t)
                rms_weightless_gate(t, dev, queue, 3840, 3, 1.0e-6)
                pos_add_gate(t, dev, queue, 98, 64, 14, 1120)   // 672x336 grid: NON-square, axis order visible
                pos_add_gate(t, dev, queue, 49, 64, 7, 1120)    // square control
                clamp_gate(t, dev, queue, 1000, CLAMP_IN, false)    // separate dst, %64 tail
                clamp_gate(t, dev, queue, 1000, CLAMP_OUT, true)    // in place
                geglu_quick_gate(t, dev, queue, 3072, 3)       // the E2B ffn width
                geglu_quick_gate(t, dev, queue, 100, 1)        // %64 tail
                rope2d_gate(t, dev, queue, 36, 12, 64)         // the E2B 96x96 grid shape (6x6 patches)
                rope2d_gate(t, dev, queue, 5, 2, 8)            // tiny: one head pair, q = 2
                head_restride_gate(t, dev, queue, 7, 16, 72, 128)   // the gemma3v pad direction
                head_restride_gate(t, dev, queue, 7, 16, 128, 72)   // and the compact
                head_restride_gate(t, dev, queue, 3, 2, 5, 5)       // identity at an odd width, %64 tail
                cross_kx_gate(t, dev, queue, 96, 128, 2, false)   // whisper hs=64; exact total%64
                cross_kx_gate(t, dev, queue, 12, 16, 2, true)     // ceil grid guard + the layer-offset slot
                cross_vx_gate(t, dev, queue, 96, 128, 2, 0, 96, false)     // the cross_kv shape (t0=0, tstride=ta)
                cross_vx_gate(t, dev, queue, 12, 16, 2, 0, 12, true)       // ceil grid + the folded bias
                cross_vx_gate(t, dev, queue, 3, 16, 2, 5, 20, true)        // the KV APPEND shape: t0>0 into a wider slab
                cross_vx_gate(t, dev, queue, 1, 16, 2, 0, 20, false, 16, 48)   // the fused-QKV column pick (soff, 3d row)
                wdec_attn_pc_gate(t, dev, queue, 1, 128, 2, 600, 600, false)   // 3 chunks, full last chunk boundary
                wdec_attn_pc_gate(t, dev, queue, 3, 128, 2, 300, 259, true)    // causal rows crossing a chunk edge
                wdec_attn_pc_gate(t, dev, queue, 1, 128, 2, 256, 256, false)   // exactly one chunk
                bias_add_res_gate(t, dev, queue, 3, 128)
                bias_gelu_lut_gate(t, dev, queue, 2, 640)
                im2col3_gate(t, dev, queue, 20, 16, 20, 1, true)    // conv1 shape: chmajor, s1, same len
                im2col3_gate(t, dev, queue, 10, 32, 20, 2, false)   // conv2 shape: row-major, s2, halved rows
                im2col3_gate(t, dev, queue, 7, 16, 13, 2, true)     // odd tail + the right pad edge
                im2col2d_gate(t, dev, queue, 10, 12, 1, 32, 1)      // conv1 shape: cin 1, K padded 9->32
                im2col2d_gate(t, dev, queue, 10, 8, 4, 36, 8)       // dense K, strided source (padded rows)
                im2col2d_gate(t, dev, queue, 5, 6, 4, 36, 4)        // odd grid, tight source
                suppress_row_gate(t, dev, queue, 2000, 37)
                softcap_row_gate(t, dev, queue, 900, 30.0)
                mtl_toy_lens_gate(t, dev, queue, 1000)      // n%64 != 0 — the tail workgroup's guard
                metal_release(queue)
            }
            t |> equal(metal_log_leaks_if_any(), 0l)
        } else {
            feint("das_metal is not built on this platform; misc kernel checks skipped\n")
        }
    }
}
