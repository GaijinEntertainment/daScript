options gen2
options _dasllama_internal = true

// CPU-oracle helpers of the kernel suite that need vulkan-present symbols — the
// shader_lingua_franca exact bodies (sdot4, unpackHalf2x16) and the kq class instances.
// A separate fixture so test_vulkan_kernels.das itself compiles module-less: the CI lint
// bar runs without dasVulkan, and this module drops out whole behind `require ?vulkan`.
module _vkd_oracles shared public

require dasllama/dasllama_lint public
require daslib/shader_lingua_franca
require dasllama/dasllama_kqformat
require dasllama/dasllama_vulkan_classes

def to_u4(w : array<uint>) : array<uint4> {
    var out : array<uint4>
    out |> resize(length(w) / 4)
    for (i in range(length(out))) {
        out[i] = uint4(w[i * 4], w[i * 4 + 1], w[i * 4 + 2], w[i * 4 + 3])
    }
    return <- out
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

def half_bytes(a : array<uint>; i : int) : float => unpackHalf2x16(uint(byte_u8(a, i) | (byte_u8(a, i + 1) << 8))).x

// q8 batch GEMM oracle over region records ([wblk0, row0, cnt, wg0] x nrec): per (row, col),
// int dots per 32-block (exact) folded by the f16 weight scale and the f32 x scale in block
// order — the device tiles reassociate, so approx bars
def q8_gemm_oracle(wq : array<uint>; wsw : array<uint>; xq : array<uint>; xs : array<float>;
                   recs : array<uint>; nrec, n, d : int; var y : array<float>) {
    let nbb = n / 32
    for (rr in range(nrec)) {
        let wblk0 = int(recs[rr * 4])
        let row0 = int(recs[rr * 4 + 1])
        let cnt = int(recs[rr * 4 + 2])
        for (r in range(cnt)) {
            for (c in range(d)) {
                var acc = 0.0
                for (b in range(nbb)) {
                    let wb = wblk0 + c * nbb + b
                    let xb = (row0 + r) * nbb + b
                    var idot = 0
                    for (j in range(8)) {
                        idot += sdot4(wq[wb * 8 + j], xq[xb * 8 + j])
                    }
                    let wsf = wb % 2 == 0 ? unpackHalf2x16(wsw[wb / 2]).x : unpackHalf2x16(wsw[wb / 2]).y
                    acc += float(idot) * wsf * xs[xb]
                }
                y[(row0 + r) * d + c] = acc
            }
        }
    }
}

// fmt-0 x f16 GEMM oracle (the cm2 l-tile's shape): q8 weight blocks dequantized by the wsh f16
// scale, f16 activations, per (row, col) in block order — the device tiles reassociate, so
// approx bars
def q8f16_gemm_oracle(wq : array<uint>; wsw : array<uint>; xf : array<uint>;
                      recs : array<uint>; nrec, n, d : int; var y : array<float>) {
    let nbb = n / 32
    for (rr in range(nrec)) {
        let wblk0 = int(recs[rr * 4])
        let row0 = int(recs[rr * 4 + 1])
        let cnt = int(recs[rr * 4 + 2])
        for (r in range(cnt)) {
            for (c in range(d)) {
                var acc = 0.0
                for (b in range(nbb)) {
                    let wb = wblk0 + c * nbb + b
                    let wsf = wb % 2 == 0 ? unpackHalf2x16(wsw[wb / 2]).x : unpackHalf2x16(wsw[wb / 2]).y
                    var bacc = 0.0
                    for (j in range(32)) {
                        bacc += float(byte_s8(wq, wb * 32 + j)) * half_at(xf, (row0 + r) * n + b * 32 + j)
                    }
                    acc += bacc * wsf
                }
                y[(row0 + r) * d + c] = acc
            }
        }
    }
}

// Q4_K x f16 GEMM oracle (the cm2 k4 tile's shape): per weight d*sc*q - dmin*mn off the
// repacked planes, rounded f16 as the decode callback rounds, f16 activations — approx bars
def k4f16_gemm_oracle(wq : array<uint>; wsu : array<uint>; xf : array<uint>;
                      recs : array<uint>; nrec, n, d : int; var y : array<float>) {
    let nsb = n / 256
    for (rr in range(nrec)) {
        let wsb0 = int(recs[rr * 4])
        let row0 = int(recs[rr * 4 + 1])
        let cnt = int(recs[rr * 4 + 2])
        for (r in range(cnt)) {
            for (c in range(d)) {
                var acc = 0.0
                for (s in range(nsb)) {
                    let sb = wsb0 + c * nsb + s
                    let dm = unpackHalf2x16(wsu[sb * 5])
                    for (g in range(8)) {
                        let sc = float(int((wsu[sb * 5 + 1 + g / 4] >> uint((g % 4) * 8)) & 0xFFu))
                        let mn = float(int((wsu[sb * 5 + 3 + g / 4] >> uint((g % 4) * 8)) & 0xFFu))
                        for (e in range(32)) {
                            let by = byte_u8(wq, sb * 128 + g * 16 + e % 16)
                            let q = float((by >> ((e / 16) * 4)) & 0xF)
                            let w = float(float16(dm.x * sc * q - dm.y * mn))
                            acc += w * half_at(xf, (row0 + r) * n + s * 256 + g * 32 + e)
                        }
                    }
                }
                y[(row0 + r) * d + c] = acc
            }
        }
    }
}

// Q6_K x f16 GEMM oracle (the cm2 k6 tile's shape): 6-bit compose - 32, per-16 signed
// sub-scale times the superblock d, rounded f16 as the decode rounds — approx bars
def k6f16_gemm_oracle(wq : array<uint>; wsu : array<uint>; xf : array<uint>;
                      recs : array<uint>; nrec, n, d : int; var y : array<float>) {
    let nsb = n / 256
    for (rr in range(nrec)) {
        let wsb0 = int(recs[rr * 4])
        let row0 = int(recs[rr * 4 + 1])
        let cnt = int(recs[rr * 4 + 2])
        for (r in range(cnt)) {
            for (c in range(d)) {
                var acc = 0.0
                for (s in range(nsb)) {
                    let sb = wsb0 + c * nsb + s
                    let dd = unpackHalf2x16(wsu[sb * 5 + 4]).x
                    for (e in range(256)) {
                        let bu = e / 32
                        let hh = (e / 16) % 2
                        let j = e % 16
                        let lo = byte_u8(wq, sb * 192 + bu * 16 + j)
                        let hby = byte_u8(wq, sb * 192 + 128 + (bu / 4) * 32 + hh * 16 + j)
                        let q6 = (((lo >> (hh * 4)) & 0xF) | (((hby >> ((bu % 4) * 2)) & 3) << 4)) - 32
                        let sidx = e / 16
                        let scb = int((wsu[sb * 5 + sidx / 4] >> uint((sidx % 4) * 8)) & 0xFFu)
                        let sc = scb >= 128 ? scb - 256 : scb
                        let w = float(float16(dd * float(sc) * float(q6)))
                        acc += w * half_at(xf, (row0 + r) * n + s * 256 + e)
                    }
                }
                y[(row0 + r) * d + c] = acc
            }
        }
    }
}

// a kq class instance with plane members bound — its blk_contrib IS the per-block dequant
// bit-math under test, callable on the CPU (no subgroup machinery inside)
def kq_cls_ref(fmt : int; wq, ws, xq : array<uint>; xs : array<float>) : KqGemvBase? {
    var k : KqGemvBase?
    if (fmt == int(KqFmt.k4)) {
        k = new KqGemvK4()
    } elif (fmt == int(KqFmt.q40)) {
        k = new KqGemvQ40()
    } elif (fmt == int(KqFmt.k5)) {
        k = new KqGemvK5()
    } elif (fmt == int(KqFmt.iq4xs)) {
        k = new KqGemvIq4xs()
    } else {
        k = new KqGemvK6()
    }
    k.wq := wq
    k.wq4 <- to_u4(wq)
    k.wsu := ws
    k.xq4 <- to_u4(xq)
    k.axs := xs
    return k
}

// kq GEMV oracle: blk_contrib driven sequentially over the region walk — the device's
// subgroup fold reassociates, so approx bars
def kq_gemv_oracle(fmt : int; wq, ws, xq : array<uint>; xs : array<float>;
                   regions : array<uint>; n, d, nreg : int) : array<float> {
    var k = kq_cls_ref(fmt, wq, ws, xq, xs)
    var y : array<float>
    y |> resize(nreg * d)
    let nsb = uint(n / 256)
    for (r in range(nreg)) {
        for (row in range(d)) {
            let w0 = regions[r * 2] + uint(row) * nsb
            let x0 = regions[r * 2 + 1]
            var acc = 0.0
            for (bb in range(int(nsb) * 8)) {
                acc += k->blk_contrib(w0 + uint(bb) / 8u, uint(bb) % 8u, x0 + uint(bb) / 8u)
            }
            y[r * d + row] = acc
        }
    }
    unsafe {
        delete k
    }
    return <- y
}

// IQ4_XS GEMV float oracle, INDEPENDENT of the class bit-math: w = d*sc*LUT[q] straight off the
// plane bytes (q40 nibble tiling, the k4 row shape with signed sc), x off the Q8_K bytes — the
// gate that a wrong codebook pack in iq4_word cannot pass while staying self-consistent
def iq4xs_gemv_float_oracle(wq, ws, xq : array<uint>; xs : array<float>;
                            regions : array<uint>; n, d, nreg : int) : array<float> {
    let lut = iq4nl_lut()
    var y : array<float>
    y |> resize(nreg * d)
    let nsb = n / 256
    for (r in range(nreg)) {
        for (row in range(d)) {
            let w0 = int(regions[r * 2]) + row * nsb
            let x0 = int(regions[r * 2 + 1])
            var acc = 0.0
            for (s in range(nsb)) {
                let sb = w0 + s
                let xsb = x0 + s
                let dd = unpackHalf2x16(ws[sb * 5]).x
                for (blk in range(8)) {
                    let sc = byte_s8(ws, sb * 20 + 4 + blk)
                    var bacc = 0.0
                    for (m in range(16)) {
                        let b = byte_u8(wq, sb * 128 + blk * 16 + m)
                        let xlo = byte_s8(xq, xsb * 256 + blk * 32 + m)
                        let xhi = byte_s8(xq, xsb * 256 + blk * 32 + 16 + m)
                        bacc += float(lut[b & 15]) * float(xlo) + float(lut[b >> 4]) * float(xhi)
                    }
                    acc += xs[xsb] * dd * float(sc) * bacc
                }
            }
            y[r * d + row] = acc
        }
    }
    return <- y
}

// kq batch GEMM oracle over region records — same blk_contrib per (row, col, block)
def kq_gemm_oracle(fmt : int; wq, ws, xq : array<uint>; xs : array<float>;
                   recs : array<uint>; nrec, n, d : int; var y : array<float>) {
    var k = kq_cls_ref(fmt, wq, ws, xq, xs)
    let nsb = n / 256
    for (rr in range(nrec)) {
        let wblk0 = int(recs[rr * 4])
        let row0 = int(recs[rr * 4 + 1])
        let cnt = int(recs[rr * 4 + 2])
        for (r in range(cnt)) {
            for (c in range(d)) {
                var acc = 0.0
                for (sb in range(nsb)) {
                    for (blk in range(8)) {
                        acc += k->blk_contrib(uint(wblk0 + c * nsb + sb), uint(blk), uint((row0 + r) * nsb + sb))
                    }
                }
                y[(row0 + r) * d + c] = acc
            }
        }
    }
    unsafe {
        delete k
    }
}
