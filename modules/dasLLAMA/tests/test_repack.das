options gen2
options _comment_hygiene = true
options _dasllama_internal = true

// Targeted tests for dasllama_repack — the grp<mr> weight repacks, the disk-order K-quant
// extractors, and the kq panel unpack. Byte-level, model-free, platform-neutral (pure CPU).
// The K-quant gate is cross-implementation: dequant_kq_row_grp (the grp reader in
// dasllama_math_default) must agree exactly with dequant_*_plane_superblock (the disk-order
// reader in dasllama_gguf) after repack — two independent readers over the two layouts.

require dastest/testing_boost public
require dasllama/dasllama_repack
require dasllama/dasllama_math_default   // dequant_kq_row_grp — the independent grp reader
require dasllama/dasllama_gguf           // dequant_*_plane_superblock — the independent disk reader
require dasllama/dasllama_gemm_schema    // kq_qsb/kq_ssb — the fmt-id plane strides
require llvm/daslib/f16_cvt              // plausible f16 scale halfwords

// deterministic byte source (LCG) — no Math.random in tests
struct private Rng {
    state : uint64
}

def private next_u32(var rng : Rng) : uint {
    rng.state = rng.state * 6364136223846793005ul + 1442695040888963407ul
    return uint(rng.state >> 33ul)
}

def private fill_bytes(var a : array<uint8>; var rng : Rng) {
    for (c in a) {
        c = uint8(next_u32(rng))
    }
}

def private fill_sbytes(var a : array<int8>; var rng : Rng) {
    for (c in a) {
        c = int8(next_u32(rng))
    }
}

// write a plausible f16 halfword (LE) so dequant never sees NaN/Inf
def private put_f16(var a : array<uint8>; off : int64; v : float) {
    let h = f32_to_f16(v)
    a[off] = uint8(h & 0xFFu)
    a[off + 1l] = uint8(h >> 8u)
}

def private f16_val(var rng : Rng; lo, hi : float) : float {
    let u = next_u32(rng) & 0xFFFFu
    return lo + (hi - lo) * (float(u) / 65535.0)
}

// ===== the disk-order extractors, gated by a semantics-first WRITER =====
// The writer places known values by the ggml packing rule (the inverse direction of the
// extractor), so reader and expected value come from independent formulations.

[test]
def test_extractors(t : T?) {
    t |> run("k45_nib reads the ggml k4/k5 nibble packing") @(t : T?) {
        // packing rule: byte js*32+i carries weight js*64+i in its low nibble and
        // weight js*64+32+i in its high nibble
        var row : array<uint8>
        row |> resize(128)
        for (js in range64(2l)) {
            for (i in range64(32l)) {
                let lo = uint((js * 64l + i) % 16l)          // value for weight k = js*64+i
                let hi = uint((js * 64l + 32l + i) % 16l)    // value for weight k = js*64+32+i
                row[int(js * 32l + i)] = uint8(lo | (hi << 4u))
            }
        }
        unsafe {
            let rp = addr(row[0])
            for (k in range64(128l)) {
                t |> equal(int(k45_nib(rp, k)), int(k % 16l))
            }
        }
    }
    t |> run("k5_hbit reads the ggml k5 qh packing") @(t : T?) {
        // packing rule: qh byte i, bit (2*js + half) carries the 5th bit of weight
        // js*64 + half*32 + i
        var qh : array<uint8>
        qh |> resize(32)
        // choose bit k -> (k*7+3)&1 as the known pattern
        for (i in range64(32l)) {
            var b = 0u
            for (js in range64(2l)) {
                for (half in range64(2l)) {
                    let k = js * 64l + half * 32l + i
                    let bit = uint((k * 7l + 3l) & 1l)
                    b |= bit << uint(2l * js + half)
                }
            }
            qh[int(i)] = uint8(b)
        }
        unsafe {
            let hp = addr(qh[0])
            for (k in range64(128l)) {
                t |> equal(int(k5_hbit(hp, k)), int((k * 7l + 3l) & 1l))
            }
        }
    }
    t |> run("k6_nib reads the ggml k6 ql packing") @(t : T?) {
        // packing rule: half h owns weights h*128..h*128+127; byte h*64 + (g%2)*32 + (k%32)
        // carries group g<2 in its low nibble, g>=2 in its high nibble (g = (k%128)/32)
        var row : array<uint8>
        row |> resize(128)
        for (h in range64(2l)) {
            for (g in range64(4l)) {
                for (i in range64(32l)) {
                    let k = h * 128l + g * 32l + i
                    let v = uint((k * 5l + 1l) % 16l)
                    let bi = int(h * 64l + (g % 2l) * 32l + i)
                    if (g < 2l) {
                        row[bi] = uint8((uint(row[bi]) & 0xF0u) | v)
                    } else {
                        row[bi] = uint8((uint(row[bi]) & 0x0Fu) | (v << 4u))
                    }
                }
            }
        }
        unsafe {
            let rp = addr(row[0])
            for (k in range64(256l)) {
                t |> equal(int(k6_nib(rp, k)), int((k * 5l + 1l) % 16l))
            }
        }
    }
}

// ===== deterministic fixture builders (same seed => same planes; no cross-block captures) =====

def private build_q8q8(n, d : int64; var w : array<int8>; var s : array<float>) {
    var rng = Rng(state = 0x51ab1001ul)
    w |> resize(d * n)
    s |> resize(d * (n / 32l))
    fill_sbytes(w, rng)
    for (c in s) {
        c = float(next_u32(rng) % 1000u) * 0.001
    }
}

def private build_mx4(n, d : int64; var np : array<uint8>; var ep : array<uint8>) {
    var rng = Rng(state = 0x4d78f0f0ul)
    np |> resize(d * (n / 2l))
    ep |> resize(d * (n / 32l))
    fill_bytes(np, rng)
    fill_bytes(ep, rng)
}

def private build_q51(n, d : int64; var q : array<uint8>; var s : array<uint8>) {
    var rng = Rng(state = 0x0051beeful)
    q |> resize(d * (n / 32l) * 20l)
    s |> resize(d * (n / 32l) * 4l)
    fill_bytes(q, rng)
    fill_bytes(s, rng)
}

// ===== q8q8: identity at mr=1, inverse map at mr=4, wbias, tails =====

[test]
def test_repack_q8q8_identity(t : T?) {
    t |> run("mr=1 is the identity") @(t : T?) {
        let n = 64l
        let d = 10l
        var w0 : array<int8>
        var s0 : array<float>
        build_q8q8(n, d, w0, s0)
        var w := w0
        var s := s0
        unsafe {
            repack_q8q8_grp(addr(w[0]), addr(s[0]), n, d, 1l)
        }
        var same = true
        for (a, b in w, w0) {
            same &&= a == b
        }
        for (a, b in s, s0) {
            same &&= a == b
        }
        t |> success(same, "mr=1 must leave both planes byte-identical")
        delete w0
        delete s0
        delete w
        delete s
    }
}

[test]
def test_repack_q8q8_inverse_map(t : T?) {
    t |> run("mr=4 kgroup=4 matches the documented inverse map; tails stay row-major") @(t : T?) {
        let n = 64l
        let d = 10l   // mr=4 leaves 2 tail rows
        let nb = n / 32l
        let mr = 4l
        let kgroup = 4l
        var w0 : array<int8>
        var s0 : array<float>
        build_q8q8(n, d, w0, s0)
        var w := w0
        var s := s0
        unsafe {
            repack_q8q8_grp(addr(w[0]), addr(s[0]), n, d, mr, 0l, kgroup)
        }
        let ng = d / mr
        var ok = true
        for (g in range64(ng)) {
            for (kg in range64(n / kgroup)) {
                for (r in range64(mr)) {
                    for (j in range64(kgroup)) {
                        let grp = w[int(g * mr * n + kg * kgroup * mr + r * kgroup + j)]
                        let row = w0[int((g * mr + r) * n + kg * kgroup + j)]
                        ok &&= grp == row
                    }
                }
            }
            for (bi in range64(nb)) {
                for (r in range64(mr)) {
                    ok &&= s[int(g * mr * nb + bi * mr + r)] == s0[int((g * mr + r) * nb + bi)]
                }
            }
        }
        t |> success(ok, "group region must match the inverse map")
        var tails = true
        for (i in range64(ng * mr * n, d * n)) {
            tails &&= w[int(i)] == w0[int(i)]
        }
        for (i in range64(ng * mr * nb, d * nb)) {
            tails &&= s[int(i)] == s0[int(i)]
        }
        t |> success(tails, "tail rows must stay row-major and untouched")
        delete w0
        delete s0
        delete w
        delete s
    }
}

[test]
def test_repack_q8q8_wbias(t : T?) {
    t |> run("wbias=128 biases the group region only") @(t : T?) {
        let n = 64l
        let d = 10l
        let mr = 4l
        var w0 : array<int8>
        var s0 : array<float>
        build_q8q8(n, d, w0, s0)
        var w := w0
        var s := s0
        var wu := w0
        var su := s0
        unsafe {
            repack_q8q8_grp(addr(w[0]), addr(s[0]), n, d, mr, 128l)
            repack_q8q8_grp(addr(wu[0]), addr(su[0]), n, d, mr, 0l)
        }
        let ng = d / mr
        var ok = true
        for (i in range64(ng * mr * n)) {
            ok &&= uint(uint8(w[int(i)])) == (uint(uint8(wu[int(i)])) ^ 0x80u)
        }
        for (i in range64(ng * mr * n, d * n)) {
            ok &&= w[int(i)] == wu[int(i)]   // tails unbiased
        }
        t |> success(ok, "bias128 must equal xor 0x80 on the group region, tails untouched")
        delete w0
        delete s0
        delete w
        delete s
        delete wu
        delete su
    }
}

// ===== mx4: identity at mr=1, inverse map at mr=4 =====

[test]
def test_repack_mx4(t : T?) {
    t |> run("mr=1 is the identity") @(t : T?) {
        let n = 64l
        let d = 8l
        var n0 : array<uint8>
        var e0 : array<uint8>
        build_mx4(n, d, n0, e0)
        var np := n0
        var ep := e0
        unsafe {
            repack_mx4_grp(addr(np[0]), addr(ep[0]), n, d, 1l)
        }
        var same = true
        for (a, b in np, n0) {
            same &&= a == b
        }
        for (a, b in ep, e0) {
            same &&= a == b
        }
        t |> success(same, "mr=1 must leave both planes byte-identical")
        delete n0
        delete e0
        delete np
        delete ep
    }
    t |> run("mr=4 matches the documented inverse map") @(t : T?) {
        let n = 64l
        let d = 8l
        let nrow = n / 2l
        let nbb = n / 32l
        let mr = 4l
        var n0 : array<uint8>
        var e0 : array<uint8>
        build_mx4(n, d, n0, e0)
        var np := n0
        var ep := e0
        unsafe {
            repack_mx4_grp(addr(np[0]), addr(ep[0]), n, d, mr)
        }
        let ng = d / mr
        var ok = true
        for (g in range64(ng)) {
            for (bi in range64(nbb)) {
                for (j in range64(4l)) {
                    for (r in range64(mr)) {
                        for (b in range64(4l)) {
                            let grp = np[int(g * mr * nrow + bi * 16l * mr + j * 4l * mr + r * 4l + b)]
                            let row = n0[int((g * mr + r) * nrow + bi * 16l + j * 4l + b)]
                            ok &&= grp == row
                        }
                    }
                }
                for (r in range64(mr)) {
                    ok &&= ep[int(g * mr * nbb + bi * mr + r)] == e0[int((g * mr + r) * nbb + bi)]
                }
            }
        }
        t |> success(ok, "nibble dwords + scale plane must match the inverse map")
        delete n0
        delete e0
        delete np
        delete ep
    }
}

// ===== q51: identity at mr=1 (grp1 IS disk order), inverse map at mr=2 =====

[test]
def test_repack_q51(t : T?) {
    t |> run("mr=1 is the identity (grp1 == disk order)") @(t : T?) {
        let n = 64l
        let d = 8l
        var q0 : array<uint8>
        var s0 : array<uint8>
        build_q51(n, d, q0, s0)
        var q := q0
        var s := s0
        unsafe {
            repack_q51_grp(addr(q[0]), addr(s[0]), n, d, 1l)
        }
        var same = true
        for (a, b in q, q0) {
            same &&= a == b
        }
        for (a, b in s, s0) {
            same &&= a == b
        }
        t |> success(same, "grp1 must be the disk order byte-for-byte")
        delete q0
        delete s0
        delete q
        delete s
    }
    t |> run("mr=2 matches the documented inverse map") @(t : T?) {
        let n = 64l
        let d = 8l
        let nb = n / 32l
        let mr = 2l
        var q0 : array<uint8>
        var s0 : array<uint8>
        build_q51(n, d, q0, s0)
        var q := q0
        var s := s0
        unsafe {
            repack_q51_grp(addr(q[0]), addr(s[0]), n, d, mr)
        }
        let ng = d / mr
        var ok = true
        for (g in range64(ng)) {
            for (bi in range64(nb)) {
                let dst = g * mr * nb * 20l + bi * 20l * mr
                for (r in range64(mr)) {
                    let src = ((g * mr + r) * nb + bi) * 20l
                    for (j in range64(4l)) {
                        for (b in range64(4l)) {
                            ok &&= q[int(dst + j * 4l * mr + r * 4l + b)] == q0[int(src + j * 4l + b)]
                        }
                    }
                    for (b in range64(4l)) {
                        ok &&= q[int(dst + 16l * mr + r * 4l + b)] == q0[int(src + 16l + b)]   // qh tail
                    }
                    let ds = g * mr * nb * 4l + bi * 4l * mr + r * 4l
                    let ss = ((g * mr + r) * nb + bi) * 4l
                    for (b in range64(4l)) {
                        ok &&= s[int(ds + b)] == s0[int(ss + b)]
                    }
                }
            }
        }
        t |> success(ok, "q51 grp bytes must match the inverse map")
        delete q0
        delete s0
        delete q
        delete s
    }
}

// ===== K-quant formats: the cross-implementation dequant gate =====
// disk planes -> repack -> per row the grp reader (dequant_kq_row_grp) must agree EXACTLY
// with the disk reader (dequant_*_plane_superblock) on the pristine copy; tails stay disk-order.

def private build_kq_disk(var kq : array<uint8>; var ks : array<uint8>; fmt : int64; n, d : int64; var rng : Rng) {
    let nsb = n / 256l
    let qsb = kq_qsb(int(fmt))
    let ssb = kq_ssb(int(fmt))
    kq |> resize(d * nsb * qsb)
    ks |> resize(d * nsb * ssb)
    fill_bytes(kq, rng)
    fill_bytes(ks, rng)
    // overwrite the f16 scale halfwords with plausible values (random bytes can encode NaN/Inf)
    for (row in range64(d)) {
        for (sbi in range64(nsb)) {
            let so = (row * nsb + sbi) * ssb
            if (fmt == 4l || fmt == 5l) {
                put_f16(ks, so, f16_val(rng, 0.25, 2.0))          // d
                put_f16(ks, so + 2l, f16_val(rng, 0.0, 1.0))      // dmin
            } elif (fmt == 6l) {
                put_f16(ks, so + 16l, f16_val(rng, 0.25, 2.0))    // d after the 16B sub-scales
            } else {   // q40: 8 x f16 d
                for (blk in range64(8l)) {
                    put_f16(ks, so + blk * 2l, f16_val(rng, 0.25, 2.0))
                }
            }
        }
    }
}

def private dequant_disk_row(fmt : int64; kq : array<uint8>; ks : array<uint8>; row, n : int64; var dst : array<float>) {
    let nsb = n / 256l
    let qsb = kq_qsb(int(fmt))
    let ssb = kq_ssb(int(fmt))
    for (sbi in range64(nsb)) {
        let qo = (row * nsb + sbi) * qsb
        let so = (row * nsb + sbi) * ssb
        if (fmt == 4l) {
            dequant_k4_plane_superblock(kq, qo, ks, so, dst, sbi * 256l)
        } elif (fmt == 5l) {
            dequant_k5_plane_superblock(kq, qo, ks, so, dst, sbi * 256l)
        } elif (fmt == 6l) {
            dequant_k6_plane_superblock(kq, qo, ks, so, dst, sbi * 256l)
        } else {
            dequant_q40_plane_superblock(kq, qo, ks, so, dst, sbi * 256l)
        }
    }
}

def private check_kq_fmt(t : T?; fmt : int64; n, d, mr : int64) {
    var rng = Rng(state = uint64(fmt) * 0x9e3779b9ul + uint64(mr))
    var kq0 : array<uint8>
    var ks0 : array<uint8>
    build_kq_disk(kq0, ks0, fmt, n, d, rng)
    var kq := kq0
    var ks := ks0
    unsafe {
        if (fmt == 4l) {
            repack_k4_grp(addr(kq[0]), addr(ks[0]), n, d, mr)
        } elif (fmt == 5l) {
            repack_k5_grp(addr(kq[0]), addr(ks[0]), n, d, mr)
        } elif (fmt == 6l) {
            repack_k6_grp(addr(kq[0]), addr(ks[0]), n, d, mr)
        } else {
            repack_q40_grp(addr(kq[0]), addr(ks[0]), n, d, mr)
        }
    }
    let nsb = n / 256l
    let qrow = nsb * kq_qsb(int(fmt))
    let srow = nsb * kq_ssb(int(fmt))
    let ng = d / mr
    var got : array<float>
    var want : array<float>
    got |> resize(n)
    want |> resize(n)
    var grp_ok = true
    var tail_ok = true
    for (row in range64(d)) {
        dequant_disk_row(fmt, kq0, ks0, row, n, want)
        if (row < ng * mr) {
            let g = row / mr
            unsafe {
                dequant_kq_row_grp(fmt, addr(kq[0]) + g * mr * qrow, addr(ks[0]) + g * mr * srow,
                                   row % mr, mr, n, addr(got[0]))
            }
            for (i in range64(n)) {
                grp_ok &&= got[i] == want[i]
            }
        } else {
            // tail rows stay disk-order in the repacked planes — the disk reader must still serve them
            dequant_disk_row(fmt, kq, ks, row, n, got)
            for (i in range64(n)) {
                tail_ok &&= got[i] == want[i]
            }
        }
    }
    t |> success(grp_ok, "fmt={fmt} mr={mr}: grp reader must match the disk reader exactly")
    t |> success(tail_ok, "fmt={fmt} mr={mr}: tail rows must stay disk-order")
    delete kq0
    delete ks0
    delete kq
    delete ks
    delete got
    delete want
}

[test]
def test_repack_kq_cross_reader(t : T?) {
    let n = 512l   // 2 superblocks per row
    for (fmt in [4l, 5l, 6l, 40l]) {
        t |> run("fmt {fmt}: mr 1/2/4 + tails") @(t : T?) {
            check_kq_fmt(t, fmt, n, 8l, 1l)
            check_kq_fmt(t, fmt, n, 8l, 2l)
            check_kq_fmt(t, fmt, n, 8l, 4l)
            check_kq_fmt(t, fmt, n, 10l, 4l)   // 2 tail rows
        }
    }
}

// ===== unpack_kq_panel_grp: chained gate disk -> repack -> unpack == disk extraction =====

def private check_unpack(t : T?; fmt : int64; n, d, mr : int64) {
    var rng = Rng(state = uint64(fmt) * 0xc0ffee11ul)
    var kq0 : array<uint8>
    var ks0 : array<uint8>
    build_kq_disk(kq0, ks0, fmt, n, d, rng)
    var kq := kq0
    var ks := ks0
    unsafe {
        if (fmt == 5l) {
            repack_k5_grp(addr(kq[0]), addr(ks[0]), n, d, mr)
        } else {
            repack_k6_grp(addr(kq[0]), addr(ks[0]), n, d, mr)
        }
    }
    let nsb = n / 256l
    let qrow = nsb * kq_qsb(int(fmt))
    var panel : array<uint8>
    panel |> resize(nsb * 256l * mr)
    var ok = true
    unsafe {
        for (g in range64(d / mr)) {
            unpack_kq_panel_grp(fmt, addr(kq[0]) + g * mr * qrow, addr(panel[0]), mr, nsb)
            for (sbi in range64(nsb)) {
                for (blk in range64(8l)) {
                    for (j in range64(4l)) {
                        for (r in range64(mr)) {
                            for (b in range64(4l)) {
                                let k = blk * 32l + j * 4l + b
                                let row = g * mr + r
                                let rq = addr(kq0[0]) + row * qrow + sbi * kq_qsb(int(fmt))
                                var lo_want = 0u
                                var hi_want = 0u
                                if (fmt == 5l) {
                                    lo_want = k45_nib(rq, k) | (k5_hbit(rq + 128l, k) << 4u)
                                    hi_want = k45_nib(rq, k + 16l) | (k5_hbit(rq + 128l, k + 16l) << 4u)
                                } else {
                                    lo_want = k6_nib(rq, k)
                                    hi_want = k6_nib(rq, k + 16l)
                                    // k6 tops: qh bits 2 per weight, block-uniform shift
                                    let hb = uint(kq0[int(row * qrow + sbi * 192l + 128l + (k / 128l) * 32l + ((k % 128l) % 32l))])
                                    let hb16 = uint(kq0[int(row * qrow + sbi * 192l + 128l + ((k + 16l) / 128l) * 32l + (((k + 16l) % 128l) % 32l))])
                                    lo_want |= ((hb >> uint(2l * ((k % 128l) / 32l))) & 3u) << 4u
                                    hi_want |= ((hb16 >> uint(2l * (((k + 16l) % 128l) / 32l))) & 3u) << 4u
                                }
                                let ob = sbi * 256l * mr
                                let gcol = ((blk * 4l + j) * mr) * 4l
                                let lo_got = uint(panel[ob + gcol + r * 4l + b])
                                let hi_got = uint(panel[ob + 128l * mr + gcol + r * 4l + b])
                                ok &&= lo_got == lo_want && hi_got == hi_want
                            }
                        }
                    }
                }
            }
        }
    }
    t |> success(ok, "fmt={fmt} mr={mr}: unpacked panel bytes must equal disk-extracted weight values")
    delete kq0
    delete ks0
    delete kq
    delete ks
    delete panel
}

[test]
def test_unpack_kq_panel(t : T?) {
    t |> run("k5 unpack (mr=4, mr=8)") @(t : T?) {
        check_unpack(t, 5l, 512l, 8l, 4l)
        check_unpack(t, 5l, 512l, 8l, 8l)
    }
    t |> run("k6 unpack (mr=4, mr=8)") @(t : T?) {
        check_unpack(t, 6l, 512l, 8l, 4l)
        check_unpack(t, 6l, 512l, 8l, 8l)
    }
}
