options gen2
options stack = 524288   // every dasLLAMA program root takes this budget
options _function_length = 0        // per-kernel gates: hand-bound buffers/uniforms + dispatch + oracle compare, one flat run each
options _cyclomatic_complexity = 0  // same — the arm ladders follow the kernel's own binding/variant axes

require dastest/testing_boost public
require dasllama/dasllama_convert   // dequant_k6_plane_superblock_at — the embed-k6 reference
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
    t |> run("decode elementwise/misc kernels == direct CPU oracles") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(das_metal)) {
            with_metal_device() <| $(dev : MetalDevice?) {
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
