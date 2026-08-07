options gen2
options _function_length = 0        // per-kernel gates: hand-bound buffers/uniforms + dispatch + oracle compare, one flat run each
options _cyclomatic_complexity = 0  // same — the arm ladders follow the kernel's own binding/variant axes

require dastest/testing_boost public
require dasllama/dasllama_convert   // dequant_k{4,5,6}_plane_superblock — the reference plane dequant
require ?das_metal dasllama/dasllama_metal_kernels  // the kq GEMV / KqMv / MoeGemv kernel set (Apple builds)
require ?das_metal metal/das_metal_boost  // device/pipeline/buffer plumbing + the leak gate
require ?das_metal _metal_kernel_common   // shared buf helpers + eyeball-dump compares
require llvm/daslib/f16_cvt         // f32_to_f16 — packing the synthetic scale planes
require math

// The K-quant decode GEMV corpus (single-stream KqGemv K4/K5/K5C/K6, the small-batch KqMv
// B2/B4/B8 twins, the expert-indexed MoeGemv kq trio) vs a CPU reference on synthetic planes:
// random bit patterns are valid quant data, scales come from finite f16 values, and the truth
// side dequants each row through the SHIPPED dequant_k*_plane_superblock (the same math the
// loaders and repack verifiers trust) and dots in double. Float GPU accumulation reassociates,
// so the compare bar rides each output's DOT ENVELOPE (sum |w·x| — the test_metal_gemm
// precedent): |gpu-cpu| <= max(2e-4 * env, 1e-3). y buffers are sentinel-filled and compared
// WHOLE, so a write outside the guarded rows/columns is a red, not silence. Off-Apple (or with
// no Metal device) every check feints cleanly.

def private hash_byte(i : int) : uint8 {
    var h = uint(i) * 2654435761u
    h ^= h >> 15u
    h *= 2246822519u
    return uint8((h >> 8u) & 0xFFu)
}

let private SAFE_SCALES = fixed_array(0.25, 0.5, 1.0, 0.125, 2.0, 0.75, 1.5, 0.375)

// Synthetic split planes at the metal layout: quant plane = hash bytes; scale plane K4/K5 =
// [d f16][dmin f16][12 packed 6-bit bytes] per superblock, K6 = [16B sub-scales] x nsb + f16 d tail.
def private kq_fill_planes(fmt, nsb : int; var kq : array<uint8>; var ks : array<uint8>) {
    let qsb = fmt == 4 ? 128 : (fmt == 5 ? 160 : 192)
    kq |> resize(nsb * qsb)
    for (i in range(nsb * qsb)) {
        kq[i] = hash_byte(i * 3 + fmt)
    }
    if (fmt == 6) {
        ks |> resize(nsb * 18)
        for (i in range(nsb * 16)) {
            ks[i] = hash_byte(i * 7 + 13)
        }
        for (b in range(nsb)) {
            let dbits = f32_to_f16(SAFE_SCALES[b % 8])
            ks[nsb * 16 + b * 2] = uint8(dbits & 0xFFu)
            ks[nsb * 16 + b * 2 + 1] = uint8(dbits >> 8u)
        }
    } else {
        ks |> resize(nsb * 16)
        for (b in range(nsb)) {
            let dbits = f32_to_f16(SAFE_SCALES[b % 8])
            let mbits = f32_to_f16(SAFE_SCALES[(b + 3) % 8])
            ks[b * 16] = uint8(dbits & 0xFFu)
            ks[b * 16 + 1] = uint8(dbits >> 8u)
            ks[b * 16 + 2] = uint8(mbits & 0xFFu)
            ks[b * 16 + 3] = uint8(mbits >> 8u)
            for (i in range(12)) {
                ks[b * 16 + 4 + i] = hash_byte(b * 31 + i * 5 + 7)
            }
        }
    }
}

// Reference dequant of one row (nb superblocks starting at superblock sb_base) off the planes.
def private kq_row_ref(fmt, sb_base, nb, nsb_total : int; kq, ks : array<uint8>; var wrow : array<float>) {
    for (ib in range(nb)) {
        let sb = sb_base + ib
        if (fmt == 4) {
            dequant_k4_plane_superblock(kq, int64(sb * 128), ks, int64(sb * 16), wrow, int64(ib * 256))
        } elif (fmt == 5) {
            dequant_k5_plane_superblock(kq, int64(sb * 160), ks, int64(sb * 16), wrow, int64(ib * 256))
        } else {
            dequant_k6_plane_superblock_at(kq, int64(sb * 192), ks, int64(sb * 16),
                int64(nsb_total * 16 + sb * 2), wrow, int64(ib * 256))
        }
    }
}

// envelope-aware compare: |gpu-cpu| <= max(rel * env, 1e-3); sentinel positions carry env 0
def private buf_mismatch_env(buf; want, env : array<float>; rel : float; tag : string) : int {
    let p = unsafe(reinterpret<float const?>(metal_buffer_contents(buf)))
    var bad = 0
    for (k in range(length(want))) {
        let g = unsafe(p[k])
        let w = want[k]
        if (abs(g - w) > max(rel * env[k], 1e-3)) {
            if (bad < 6) {
                to_log(LOG_ERROR, "  {tag} mismatch [{k}] gpu {g} cpu {w} env {env[k]}\n")
            }
            bad++
        }
    }
    return bad
}

// ===== single-stream kq GEMV (MetalKqGemvK4 / K5 / K5C / K6) =====

def private kq_gemv_gate(t : T?; dev, queue; vform : string; n, d : int) {
    let fmt = vform == "k4" ? 4 : (vform == "k6" ? 6 : 5)
    let tag = "kq_gemv_{vform} n={n} d={d}"
    var err : string
    let src = vform == "k4" ? metal_kq_gemv_k4_msl : (vform == "k5" ? metal_kq_gemv_k5_msl : (vform == "k5c" ? metal_kq_gemv_k5c_msl : metal_kq_gemv_k6_msl))
    let entry = vform == "k4" ? metal_kq_gemv_k4_msl_entry : (vform == "k5" ? metal_kq_gemv_k5_msl_entry : (vform == "k5c" ? metal_kq_gemv_k5c_msl_entry : metal_kq_gemv_k6_msl_entry))
    let fm = vform == "k4" ? metal_kq_gemv_k4_msl_fastmath : (vform == "k5" ? metal_kq_gemv_k5_msl_fastmath : (vform == "k5c" ? metal_kq_gemv_k5c_msl_fastmath : metal_kq_gemv_k6_msl_fastmath))
    var pso = pipeline_from_source(dev, src, entry, fm, err)
    t |> success(pso != null, "{tag}: pipeline: {err}")
    return if (pso == null)
    let nb = n / 256
    let nsb = d * nb
    var kq : array<uint8>
    var ks : array<uint8>
    kq_fill_planes(fmt, nsb, kq, ks)
    var xv : array<float>
    xv |> resize(n)
    for (i in range(n)) {
        xv[i] = 0.11 * float(i % 41) - 2.2
    }
    var want : array<float>
    var env : array<float>
    var wrow : array<float>
    want |> resize(d)
    env |> resize(d)
    wrow |> resize(n)
    for (r in range(d)) {
        kq_row_ref(fmt, r * nb, nb, nsb, kq, ks, wrow)
        var acc = 0.0lf
        var ev = 0.0lf
        for (c in range(n)) {
            let p = double(wrow[c]) * double(xv[c])
            acc += p
            ev += abs(p)
        }
        want[r] = float(acc)
        env[r] = float(ev)
    }
    var bks = buf_upload(dev, ks)
    var bkq = buf_upload(dev, kq)
    var bx = buf_upload(dev, xv)
    var by = buf_fill(dev, d, -1000.0)
    var bn = buf_u32(dev, uint(n))
    var bd = buf_u32(dev, uint(d))
    let groups = vform == "k5c" ? (d + 1) / 2 : (d + 3) / 4
    let ran = with_compute_encoder(queue, err) $(enc : MetalComputeEncoder?) {
        metal_set_pipeline(enc, pso)
        metal_set_buffer(enc, bks, fmt == 6 ? uint64(nsb * 16) : 0ul, 0)   // k6: the f16 d tail
        metal_set_buffer(enc, bks, 0ul, 1)
        metal_set_buffer(enc, bkq, 0ul, 2)
        metal_set_buffer(enc, bx, 0ul, 3)
        metal_set_buffer(enc, by, 0ul, 4)
        metal_set_buffer(enc, bn, 0ul, 5)
        metal_set_buffer(enc, bd, 0ul, 6)
        metal_dispatch_threadgroups(enc, uint3(uint(groups), 1u, 1u), uint3(64u, 1u, 1u))
    }
    t |> success(ran, "{tag}: encode: {err}")
    if (ran) {
        t |> equal(buf_mismatch_env(by, want, env, 2e-4, tag), 0)
    }
    metal_release(bks)
    metal_release(bkq)
    metal_release(bx)
    metal_release(by)
    metal_release(bn)
    metal_release(bd)
    metal_release(pso)
    delete kq
    delete ks
    delete xv
    delete want
    delete env
    delete wrow
}

// ===== the small-batch KqMv twins (B2/B4/B8 x K4/K5/K6): y[col * ys + row] =====

def private kq_mvb_gate(t : T?; dev, queue; fmt, bwidth, n, d, nr, ys : int) {
    let tag = "kq_mvb{bwidth}_k{fmt} n={n} d={d} nr={nr} ys={ys}"
    var err : string
    let src = (bwidth == 2 ? (fmt == 4 ? metal_kq_mvb2_k4_msl : (fmt == 5 ? metal_kq_mvb2_k5_msl : metal_kq_mvb2_k6_msl))
        : (bwidth == 4 ? (fmt == 4 ? metal_kq_mvb4_k4_msl : (fmt == 5 ? metal_kq_mvb4_k5_msl : metal_kq_mvb4_k6_msl))
            : (fmt == 4 ? metal_kq_mvb8_k4_msl : (fmt == 5 ? metal_kq_mvb8_k5_msl : metal_kq_mvb8_k6_msl))))
    let entry = (bwidth == 2 ? (fmt == 4 ? metal_kq_mvb2_k4_msl_entry : (fmt == 5 ? metal_kq_mvb2_k5_msl_entry : metal_kq_mvb2_k6_msl_entry))
        : (bwidth == 4 ? (fmt == 4 ? metal_kq_mvb4_k4_msl_entry : (fmt == 5 ? metal_kq_mvb4_k5_msl_entry : metal_kq_mvb4_k6_msl_entry))
            : (fmt == 4 ? metal_kq_mvb8_k4_msl_entry : (fmt == 5 ? metal_kq_mvb8_k5_msl_entry : metal_kq_mvb8_k6_msl_entry))))
    let fm = (bwidth == 2 ? (fmt == 4 ? metal_kq_mvb2_k4_msl_fastmath : (fmt == 5 ? metal_kq_mvb2_k5_msl_fastmath : metal_kq_mvb2_k6_msl_fastmath))
        : (bwidth == 4 ? (fmt == 4 ? metal_kq_mvb4_k4_msl_fastmath : (fmt == 5 ? metal_kq_mvb4_k5_msl_fastmath : metal_kq_mvb4_k6_msl_fastmath))
            : (fmt == 4 ? metal_kq_mvb8_k4_msl_fastmath : (fmt == 5 ? metal_kq_mvb8_k5_msl_fastmath : metal_kq_mvb8_k6_msl_fastmath))))
    var pso = pipeline_from_source(dev, src, entry, fm, err)
    t |> success(pso != null, "{tag}: pipeline: {err}")
    return if (pso == null)
    let nb = n / 256
    let nsb = d * nb
    var kq : array<uint8>
    var ks : array<uint8>
    kq_fill_planes(fmt, nsb, kq, ks)
    // x is padded to the kernel's column-group width (the driver's mp guarantee); pad rows zero
    let gcols = bwidth == 4 ? (nr + 3) / 4 : 1
    let xpad = bwidth == 2 ? 2 : (bwidth == 4 ? gcols * 4 : 8)
    var xv : array<float>
    xv |> resize(xpad * n)
    for (i in range(xpad * n)) {
        xv[i] = i < nr * n ? 0.09 * float(i % 43) - 1.8 : 0.0
    }
    var want : array<float>
    var env : array<float>
    var wrow : array<float>
    want |> resize(nr * ys)
    env |> resize(nr * ys)
    wrow |> resize(n)
    for (i in range(nr * ys)) {
        want[i] = -1000.0
        env[i] = 0.0
    }
    for (r in range(d)) {
        kq_row_ref(fmt, r * nb, nb, nsb, kq, ks, wrow)
        for (col in range(nr)) {
            var acc = 0.0lf
            var ev = 0.0lf
            for (c in range(n)) {
                let p = double(wrow[c]) * double(xv[col * n + c])
                acc += p
                ev += abs(p)
            }
            want[col * ys + r] = float(acc)
            env[col * ys + r] = float(ev)
        }
    }
    var bks = buf_upload(dev, ks)
    var bkq = buf_upload(dev, kq)
    var bx = buf_upload(dev, xv)
    var by = buf_fill(dev, nr * ys, -1000.0)
    var ka = KqMvArgs(ndim = uint(n), ddim = uint(d), ys = uint(ys), nr = uint(nr))
    let ran = with_compute_encoder(queue, err) $(enc : MetalComputeEncoder?) {
        metal_set_pipeline(enc, pso)
        if (bwidth == 8) {
            metal_set_threadgroup_memory_length(enc,
                fmt == 4 ? metal_kq_mvb8_k4_msl_tgmem : (fmt == 5 ? metal_kq_mvb8_k5_msl_tgmem : metal_kq_mvb8_k6_msl_tgmem), 0)
        }
        metal_set_buffer(enc, bks, fmt == 6 ? uint64(nsb * 16) : 0ul, 0)   // k6: the f16 d tail
        metal_set_buffer(enc, bks, 0ul, 1)
        metal_set_buffer(enc, bkq, 0ul, 2)
        metal_set_buffer(enc, bx, 0ul, 3)
        metal_set_buffer(enc, by, 0ul, 4)
        metal_set_bytes(enc, unsafe(addr<void?>(ka)), uint64(typeinfo sizeof(ka)), 5)
        metal_dispatch_threadgroups(enc, uint3(uint((d + 7) / 8), uint(gcols), 1u), uint3(64u, 1u, 1u))
    }
    t |> success(ran, "{tag}: encode: {err}")
    if (ran) {
        t |> equal(buf_mismatch_env(by, want, env, 2e-4, tag), 0)
    }
    metal_release(bks)
    metal_release(bkq)
    metal_release(bx)
    metal_release(by)
    metal_release(pso)
    delete kq
    delete ks
    delete xv
    delete want
    delete env
    delete wrow
}

// ===== the expert-indexed MoeGemv kq trio (MetalMoeGemvK4 / K5 / K6) =====

def private moe_kq_gemv_gate(t : T?; dev, queue; fmt, n, d, net, k, nst : int; perslot : bool) {
    let tag = "moe_gemv_k{fmt} n={n} d={d} net={net} k={k} nst={nst} perslot={perslot}"
    var err : string
    let src = fmt == 4 ? metal_moe_gemv_k4_msl : (fmt == 5 ? metal_moe_gemv_k5_msl : metal_moe_gemv_k6_msl)
    let entry = fmt == 4 ? metal_moe_gemv_k4_msl_entry : (fmt == 5 ? metal_moe_gemv_k5_msl_entry : metal_moe_gemv_k6_msl_entry)
    let fm = fmt == 4 ? metal_moe_gemv_k4_msl_fastmath : (fmt == 5 ? metal_moe_gemv_k5_msl_fastmath : metal_moe_gemv_k6_msl_fastmath)
    var pso = pipeline_from_source(dev, src, entry, fm, err)
    t |> success(pso != null, "{tag}: pipeline: {err}")
    return if (pso == null)
    let nb = n / 256
    let nsb = net * d * nb              // the whole expert stack's superblocks
    var kq : array<uint8>
    var ks : array<uint8>
    kq_fill_planes(fmt, nsb, kq, ks)
    // selection rows: [k idx u32][k w f32] per stream — only the idx section is read here
    var sel : array<uint>
    sel |> resize(nst * 2 * k)
    for (s in range(nst)) {
        for (j in range(k)) {
            sel[s * 2 * k + j] = uint((s + j * 3 + net - 1) % net)
        }
    }
    let xrows = perslot ? nst * k : nst
    var xv : array<float>
    xv |> resize(xrows * n)
    for (i in range(xrows * n)) {
        xv[i] = 0.07 * float(i % 37) - 1.2
    }
    var want : array<float>
    var env : array<float>
    var wrow : array<float>
    want |> resize(nst * k * d)
    env |> resize(nst * k * d)
    wrow |> resize(n)
    for (s in range(nst)) {
        for (j in range(k)) {
            let e = int(sel[s * 2 * k + j])
            let x0 = (perslot ? s * k + j : s) * n
            for (r in range(d)) {
                kq_row_ref(fmt, (e * d + r) * nb, nb, nsb, kq, ks, wrow)
                var acc = 0.0lf
                var ev = 0.0lf
                for (c in range(n)) {
                    let p = double(wrow[c]) * double(xv[x0 + c])
                    acc += p
                    ev += abs(p)
                }
                want[(s * k + j) * d + r] = float(acc)
                env[(s * k + j) * d + r] = float(ev)
            }
        }
    }
    var bks = buf_upload(dev, ks)
    var bkq = buf_upload(dev, kq)
    var bx = buf_upload(dev, xv)
    var by = buf_fill(dev, nst * k * d, -1000.0)
    var bsel = buf_upload(dev, sel)
    var ka = MoeGemvArgs(ndim = uint(n), ddim = uint(d), eplane = uint(d * nb),
        xs = perslot ? uint(n) : 0u, ss = uint(2 * k),
        bxs = uint((perslot ? k : 1) * n), bys = uint(k * d), hasb = 0u)
    let ran = with_compute_encoder(queue, err) $(enc : MetalComputeEncoder?) {
        metal_set_pipeline(enc, pso)
        metal_set_buffer(enc, bks, fmt == 6 ? uint64(nsb * 16) : 0ul, 0)   // k6: the f16 d tail
        metal_set_buffer(enc, bks, 0ul, 1)
        metal_set_buffer(enc, bkq, 0ul, 2)
        metal_set_buffer(enc, bx, 0ul, 3)
        metal_set_buffer(enc, by, 0ul, 4)
        metal_set_bytes(enc, unsafe(addr<void?>(ka)), uint64(typeinfo sizeof(ka)), 5)
        metal_set_buffer(enc, bsel, 0ul, 7)
        metal_dispatch_threadgroups(enc, uint3(uint((d + 3) / 4), uint(k), uint(nst)), uint3(64u, 1u, 1u))
    }
    t |> success(ran, "{tag}: encode: {err}")
    if (ran) {
        t |> equal(buf_mismatch_env(by, want, env, 2e-4, tag), 0)
    }
    metal_release(bks)
    metal_release(bkq)
    metal_release(bx)
    metal_release(by)
    metal_release(bsel)
    metal_release(pso)
    delete kq
    delete ks
    delete sel
    delete xv
    delete want
    delete env
    delete wrow
}

[test]
def test_metal_kq_gemv_kernels(t : T?) {
    t |> run("kq GEMV / KqMv / MoeGemv-kq kernels == the plane-dequant CPU reference") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(das_metal)) {
            with_metal_device() <| $(dev : MetalDevice?) {
                if (dev == null) {
                    feint("no Metal device on this box; GPU kernel checks skipped\n")
                    return
                }
                var queue = metal_new_command_queue(dev)
                // single-stream: all four variants; d = 11 exercises the row-guard tails
                kq_gemv_gate(t, dev, queue, "k4", 1280, 11)
                kq_gemv_gate(t, dev, queue, "k5", 1280, 11)
                kq_gemv_gate(t, dev, queue, "k5c", 1280, 11)
                kq_gemv_gate(t, dev, queue, "k6", 1280, 11)
                kq_gemv_gate(t, dev, queue, "k4", 512, 30)
                kq_gemv_gate(t, dev, queue, "k6", 512, 30)
                // the small-batch twins: nr = 2 / 3 (b4 col guard) / 6 (b8 pad), ys > d
                for (fmt in [4, 5, 6]) {
                    kq_mvb_gate(t, dev, queue, fmt, 2, 768, 17, 2, 22)
                    kq_mvb_gate(t, dev, queue, fmt, 4, 768, 17, 3, 22)
                    kq_mvb_gate(t, dev, queue, fmt, 8, 768, 17, 6, 22)
                }
                // expert-indexed: shared-x (gate/up) and per-slot (down) forms
                for (fmt in [4, 5, 6]) {
                    moe_kq_gemv_gate(t, dev, queue, fmt, 512, 9, 5, 3, 2, false)
                    moe_kq_gemv_gate(t, dev, queue, fmt, 512, 9, 5, 3, 2, true)
                }
                metal_release(queue)
            }
            t |> equal(metal_log_leaks_if_any(), 0l)
        } else {
            feint("das_metal is not built on this platform; kq GEMV kernel checks skipped\n")
        }
    }
}
