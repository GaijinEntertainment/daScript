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
    } elif (fmt == int(KqFmt.k3)) {
        k = new KqGemvK3()
    } elif (fmt == int(KqFmt.iq3s)) {
        var ki = new KqGemvIq3s()
        for (i in range(512)) {   // the CPU replay never runs the staging kernel head - fill the workgroup grid here
            ki.gridw[i] = IQ3S_GRID[i]
        }
        k = ki
    } elif (fmt == int(KqFmt.iq3xxs)) {
        var kx = new KqGemvIq3xxs()
        for (i in range(256)) {   // same replay rule: the halved grid fills here
            kx.gridw[i] = IQ3XXS_GRID[i]
        }
        k = kx
    } elif (fmt == int(KqFmt.iq4nl)) {
        k = new KqGemvIq4nl()   // pure-ALU codebook - no workgroup state to fill
    } elif (fmt == int(KqFmt.k2)) {
        k = new KqGemvK2()   // pure ALU - no workgroup state to fill
    } elif (fmt == int(KqFmt.iq2s)) {
        var k2s = new KqGemvIq2s()
        for (i in range(2048)) {   // same replay rule: the u64 grid's word pairs fill here
            k2s.gridw[i] = IQ2S_GRID[i]
        }
        k = k2s
    } elif (fmt == int(KqFmt.iq2xs)) {
        var k2x = new KqGemvIq2xs()
        for (i in range(1024)) {   // same replay rule, the 512-entry grid's word pairs
            k2x.gridw[i] = IQ2XS_GRID[i]
        }
        k = k2x
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

// Q3_K GEMV float oracle, INDEPENDENT of the class bit-math: w = d * sc16 * (q - 4) straight off
// the plane bytes (qs lane at shift 2*(blk%4), hmask bit blk, k6's 5-word scale row), x off the
// Q8_K bytes — the witness a wrong lane/mask pick in k3_dot cannot pass while staying self-consistent
def k3_gemv_float_oracle(wq, ws, xq : array<uint>; xs : array<float>;
                         regions : array<uint>; n, d, nreg : int) : array<float> {
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
                let dd = half_at(ws, sb * 10 + 8)
                for (blk in range(8)) {
                    let sh = 2 * (blk % 4)
                    let s0 = byte_s8(ws, sb * 20 + 2 * blk)
                    let s1 = byte_s8(ws, sb * 20 + 2 * blk + 1)
                    var bacc = 0.0
                    for (l in range(32)) {
                        let q2 = (byte_u8(wq, sb * 96 + (blk / 4) * 32 + l) >> sh) & 3
                        let hb = (byte_u8(wq, sb * 96 + 64 + l) >> blk) & 1
                        let q = (q2 | (hb << 2)) - 4
                        bacc += float(l < 16 ? s0 : s1) * float(q) * float(byte_s8(xq, xsb * 256 + blk * 32 + l))
                    }
                    acc += xs[xsb] * dd * bacc
                }
            }
            y[r * d + row] = acc
        }
    }
    return <- y
}

// IQ3_S GEMV float oracle, INDEPENDENT of the class bit-math: w = d * (1 + 2s) * grid_byte *
// sign straight off the plane bytes ([qs 64][qh 8][signs 32], the k4-shaped scale row), x off
// the Q8_K bytes - the witness a wrong index/sign pick in the gather cannot pass
def iq3s_gemv_float_oracle(wq, ws, xq : array<uint>; xs : array<float>;
                           regions : array<uint>; n, d, nreg : int) : array<float> {
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
                let dd = half_at(ws, sb * 10)
                for (blk in range(8)) {
                    let sc = float(byte_s8(ws, sb * 20 + 4 + blk))   // the plane byte IS the signed (1 + 2s) sub-scale
                    var bacc = 0.0
                    for (e in range(32)) {
                        let l = e / 8
                        let hj = e % 8
                        let qi = byte_u8(wq, sb * 104 + blk * 8 + 2 * l + hj / 4) | ((byte_u8(wq, sb * 104 + 64 + blk) << (hj < 4 ? 8 - 2 * l : 7 - 2 * l)) & 256)
                        let g = float(int(IQ3S_GRID[qi] >> uint(8 * (hj % 4))) & 0xFF)
                        let sgn = ((byte_u8(wq, sb * 104 + 72 + blk * 4 + l) >> hj) & 1) != 0 ? -1.0 : 1.0
                        bacc += g * sgn * float(byte_s8(xq, xsb * 256 + blk * 32 + e))
                    }
                    acc += xs[xsb] * dd * sc * bacc
                }
            }
            y[r * d + row] = acc
        }
    }
    return <- y
}

// IQ3_XXS GEMV float oracle, INDEPENDENT of the class bit-math: w = d_halved * (2ls + 1) *
// halved_grid_byte * sign straight off the plane bytes ([qs 64][aux 32], the iq3s-shaped
// scale row), signs through the CPU-side ksigns table.
def iq3xxs_gemv_float_oracle(wq, ws, xq : array<uint>; xs : array<float>;
                             regions : array<uint>; n, d, nreg : int) : array<float> {
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
                let dd = half_at(ws, sb * 10)
                for (blk in range(8)) {
                    let sc = float(byte_s8(ws, sb * 20 + 4 + blk))   // the plane byte IS the (2ls + 1) sub-scale
                    let aux = byte_u8(wq, sb * 96 + 64 + blk * 4) | (byte_u8(wq, sb * 96 + 64 + blk * 4 + 1) << 8) | (byte_u8(wq, sb * 96 + 64 + blk * 4 + 2) << 16) | (byte_u8(wq, sb * 96 + 64 + blk * 4 + 3) << 24)
                    var bacc = 0.0
                    for (e in range(32)) {
                        let l = e / 8
                        let hj = e % 8
                        let qi = byte_u8(wq, sb * 96 + blk * 8 + 2 * l + hj / 4)
                        let g = float(int(IQ3XXS_GRID[qi] >> uint(8 * (hj % 4))) & 0xFF)
                        let sgn = ((int(KSIGNS_IQ2XS[(aux >> (7 * l)) & 127]) >> hj) & 1) != 0 ? -1.0 : 1.0
                        bacc += g * sgn * float(byte_s8(xq, xsb * 256 + blk * 32 + e))
                    }
                    acc += xs[xsb] * dd * sc * bacc
                }
            }
            y[r * d + row] = acc
        }
    }
    return <- y
}

// IQ2_S GEMV float oracle, INDEPENDENT of the class bit-math: w = d8 * (1 + 2s) * grid_byte *
// sign straight off the plane bytes ([idx 32][signs 32][qh 8], the k4-shaped scale row with
// UNSIGNED per-16 strips and the pre-eighth-ed d).
def iq2s_gemv_float_oracle(wq, ws, xq : array<uint>; xs : array<float>;
                           regions : array<uint>; n, d, nreg : int) : array<float> {
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
                let dd = half_at(ws, sb * 10)
                for (blk in range(8)) {
                    let qh = byte_u8(wq, sb * 72 + 64 + blk)
                    for (hh in range(2)) {
                        let sc = float(byte_u8(ws, sb * 20 + 4 + blk * 2 + hh))
                        var bacc = 0.0
                        for (e in range(16)) {
                            let ee = hh * 16 + e
                            let l = ee / 8
                            let hj = ee % 8
                            let qi = (byte_u8(wq, sb * 72 + blk * 4 + l) | ((qh << (8 - 2 * l)) & 0x300)) * 2 + hj / 4
                            let g = float(int(IQ2S_GRID[qi] >> uint(8 * (hj % 4))) & 0xFF)
                            let sgn = ((byte_u8(wq, sb * 72 + 32 + blk * 4 + l) >> hj) & 1) != 0 ? -1.0 : 1.0
                            bacc += g * sgn * float(byte_s8(xq, xsb * 256 + blk * 32 + ee))
                        }
                        acc += xs[xsb] * dd * sc * bacc
                    }
                }
            }
            y[r * d + row] = acc
        }
    }
    return <- y
}

// IQ2_XS GEMV float oracle, INDEPENDENT of the class bit-math: w = d8 * (1 + 2s) * grid_byte
// * ksign straight off the plane words (32 u16 qs words per superblock, no sign/qh planes -
// the 7-bit ksigns index rides bits 9..15; the k4-shaped scale row with UNSIGNED strips).
def iq2xs_gemv_float_oracle(wq, ws, xq : array<uint>; xs : array<float>;
                            regions : array<uint>; n, d, nreg : int) : array<float> {
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
                let dd = half_at(ws, sb * 10)
                for (wi in range(32)) {
                    let w16 = int((wq[sb * 16 + wi / 2] >> uint((wi % 2) * 16)) & 0xFFFFu)
                    let sidx = w16 >> 9
                    var tt = sidx ^ (sidx >> 4)
                    tt = tt ^ (tt >> 2)
                    tt = tt ^ (tt >> 1)
                    let sgb = sidx | ((tt & 1) << 7)
                    let sc = float(byte_u8(ws, sb * 20 + 4 + wi / 2))
                    var bacc = 0.0
                    for (j in range(8)) {
                        let gb = float(int(IQ2XS_GRID[(w16 & 511) * 2 + j / 4] >> uint(8 * (j % 4))) & 0xFF)
                        let sgn = ((sgb >> j) & 1) != 0 ? -1.0 : 1.0
                        bacc += gb * sgn * float(byte_s8(xq, xsb * 256 + wi * 8 + j))
                    }
                    acc += xs[xsb] * dd * sc * bacc
                }
            }
            y[r * d + row] = acc
        }
    }
    return <- y
}

// IQ3_S f16-feed GEMM reference over the device form: grid gather + sign flip per element,
// f16-rounded per weight like its siblings ([qs 64][qh 8][signs 32] rows, the k4-shaped
// scale row with signed (1 + 2s) sub-scales).
def iq3sf16_gemm_oracle(wq : array<uint>; wsu : array<uint>; xf : array<uint>;
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
                    let dv = unpackHalf2x16(wsu[sb * 5]).x
                    for (g in range(8)) {
                        let sc = float(int(wsu[sb * 5 + 1 + g / 4] << uint((3 - g % 4) * 8)) >> 24)
                        for (e in range(32)) {
                            let wj = e / 4
                            let qi = byte_u8(wq, sb * 104 + g * 8 + wj) | ((byte_u8(wq, sb * 104 + 64 + g) << (8 - wj)) & 256)
                            let gb = float(int(IQ3S_GRID[qi] >> uint(8 * (e % 4))) & 0xFF)
                            let sbit = (byte_u8(wq, sb * 104 + 72 + g * 4 + e / 8) >> (((e / 4) % 2) * 4 + e % 4)) & 1
                            let w = float(float16(dv * sc * (sbit != 0 ? -gb : gb)))
                            acc += w * half_at(xf, (row0 + r) * n + s * 256 + g * 32 + e)
                        }
                    }
                }
                y[(row0 + r) * d + c] = acc
            }
        }
    }
}

// IQ3_XXS f16-feed GEMM reference over the device form: halved-grid gather + ksigns sign per
// element, f16-rounded per weight like its siblings ([qs 64][aux 32] rows, the iq3s-shaped
// scale row with (2ls + 1) sub-scales and the halved d).
def iq3xxsf16_gemm_oracle(wq : array<uint>; wsu : array<uint>; xf : array<uint>;
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
                    let dv = unpackHalf2x16(wsu[sb * 5]).x
                    for (g in range(8)) {
                        let sc = float(int(wsu[sb * 5 + 1 + g / 4] << uint((3 - g % 4) * 8)) >> 24)
                        let aux = byte_u8(wq, sb * 96 + 64 + g * 4) | (byte_u8(wq, sb * 96 + 64 + g * 4 + 1) << 8) | (byte_u8(wq, sb * 96 + 64 + g * 4 + 2) << 16) | (byte_u8(wq, sb * 96 + 64 + g * 4 + 3) << 24)
                        for (e in range(32)) {
                            let qi = byte_u8(wq, sb * 96 + g * 8 + e / 4)
                            let gb = float(int(IQ3XXS_GRID[qi] >> uint(8 * (e % 4))) & 0xFF)
                            let sbit = (int(KSIGNS_IQ2XS[(aux >> (7 * (e / 8))) & 127]) >> (e % 8)) & 1
                            let w = float(float16(dv * sc * (sbit != 0 ? -gb : gb)))
                            acc += w * half_at(xf, (row0 + r) * n + s * 256 + g * 32 + e)
                        }
                    }
                }
                y[(row0 + r) * d + c] = acc
            }
        }
    }
}

// IQ2_S f16-feed GEMM reference over the device form: u64-grid gather (two words per 10-bit
// index) + the block's own sign bytes, f16-rounded per weight like its siblings ([idx 32]
// [signs 32][qh 8] rows, the k4-shaped scale row with UNSIGNED per-16 strips, d pre-eighth-ed).
def iq2sf16_gemm_oracle(wq : array<uint>; wsu : array<uint>; xf : array<uint>;
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
                    let dv = unpackHalf2x16(wsu[sb * 5]).x
                    for (g in range(8)) {
                        let qh = byte_u8(wq, sb * 72 + 64 + g)
                        for (e in range(32)) {
                            let l = e / 8
                            let si = g * 2 + e / 16
                            let sc = float((wsu[sb * 5 + 1 + si / 4] >> uint((si % 4) * 8)) & 0xFFu)
                            let qi = (byte_u8(wq, sb * 72 + g * 4 + l) | ((qh << (8 - 2 * l)) & 0x300)) * 2 + (e % 8) / 4
                            let gb = float(int(IQ2S_GRID[qi] >> uint(8 * (e % 4))) & 0xFF)
                            let sbit = (byte_u8(wq, sb * 72 + 32 + g * 4 + l) >> (e % 8)) & 1
                            let w = float(float16(dv * sc * (sbit != 0 ? -gb : gb)))
                            acc += w * half_at(xf, (row0 + r) * n + s * 256 + g * 32 + e)
                        }
                    }
                }
                y[(row0 + r) * d + c] = acc
            }
        }
    }
}

// IQ2_XS f16-feed GEMM reference over the device form: u64-grid gather (two words per 9-bit
// index) + ksigns-by-parity off each u16 qs word, f16-rounded per weight like its siblings
// (32 u16 words per superblock, the k4-shaped scale row with UNSIGNED strips, d pre-eighth-ed).
def iq2xsf16_gemm_oracle(wq : array<uint>; wsu : array<uint>; xf : array<uint>;
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
                    let dv = unpackHalf2x16(wsu[sb * 5]).x
                    for (e in range(256)) {
                        let wi = e / 8
                        let j = e % 8
                        let w16 = int((wq[sb * 16 + wi / 2] >> uint((wi % 2) * 16)) & 0xFFFFu)
                        let sidx = w16 >> 9
                        var tt = sidx ^ (sidx >> 4)
                        tt = tt ^ (tt >> 2)
                        tt = tt ^ (tt >> 1)
                        let sgb = sidx | ((tt & 1) << 7)
                        let si = e / 16
                        let sc = float((wsu[sb * 5 + 1 + si / 4] >> uint((si % 4) * 8)) & 0xFFu)
                        let gb = float(int(IQ2XS_GRID[(w16 & 511) * 2 + j / 4] >> uint(8 * (j % 4))) & 0xFF)
                        let w = float(float16(dv * sc * (((sgb >> j) & 1) != 0 ? -gb : gb)))
                        acc += w * half_at(xf, (row0 + r) * n + s * 256 + e)
                    }
                }
                y[(row0 + r) * d + c] = acc
            }
        }
    }
}

// IQ4_NL f16-feed GEMM reference over the device form: q40's walk with the codebook in place
// of (q - 8), f16-rounded per weight like its siblings (q40's 32-word rows + 8 f16 d).
def iq4nlf16_gemm_oracle(wq : array<uint>; wsu : array<uint>; xf : array<uint>;
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
                    for (g in range(8)) {
                        let dp = unpackHalf2x16(wsu[sb * 5 + g / 2])
                        let dv = g % 2 == 0 ? dp.x : dp.y
                        for (e in range(32)) {
                            let by = byte_u8(wq, sb * 128 + g * 16 + e % 16)
                            let q = (by >> ((e / 16) * 4)) & 0xF
                            let w = float(float16(dv * float(int(IQ4NL_LUT[q]))))
                            acc += w * half_at(xf, (row0 + r) * n + s * 256 + g * 32 + e)
                        }
                    }
                }
                y[(row0 + r) * d + c] = acc
            }
        }
    }
}

// Q2_K f16-feed GEMM reference over the device form: per-16 sc/min nibble pairs x d/dmin,
// f16-rounded per weight like its siblings (16-word rows + [d|dmin][16 pair bytes] scales).
def k2f16_gemm_oracle(wq : array<uint>; wsu : array<uint>; xf : array<uint>;
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
                    for (g in range(16)) {
                        let pb = (int(wsu[sb * 5 + 1 + g / 4]) >> ((g % 4) * 8)) & 0xFF
                        for (l in range(16)) {
                            let qi = (g / 8) * 32 + (g % 2) * 16 + l
                            let by = byte_u8(wq, sb * 64 + qi)
                            let q = (by >> (((g / 2) % 4) * 2)) & 3
                            let w = float(float16(dm.x * float(pb & 0xF) * float(q) - dm.y * float(pb >> 4)))
                            acc += w * half_at(xf, (row0 + r) * n + s * 256 + g * 16 + l)
                        }
                    }
                }
                y[(row0 + r) * d + c] = acc
            }
        }
    }
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

// the four remaining kq formats' f16-feed GEMM references, over the same device forms the cm2
// decode callbacks read (k5 = the k4 row plus the packed 5th-bit plane; q40 = 8 f16 d; iq4xs =
// d + signed sub-scales + the iq4nl codebook; k3 = 2-bit lanes + hmask over the k6 scale row)
def k5f16_gemm_oracle(wq : array<uint>; wsu : array<uint>; xf : array<uint>;
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
                            let j = e % 16
                            let h = e / 16
                            let by = byte_u8(wq, sb * 160 + g * 16 + j)
                            let hby = byte_u8(wq, sb * 160 + 128 + g * 4 + j / 4)
                            let q = float(((by >> (h * 4)) & 0xF) | (((hby >> (j % 4 + h * 4)) & 1) << 4))
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

def q40f16_gemm_oracle(wq : array<uint>; wsu : array<uint>; xf : array<uint>;
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
                    for (g in range(8)) {
                        let dp = unpackHalf2x16(wsu[sb * 5 + g / 2])
                        let dv = g % 2 == 0 ? dp.x : dp.y
                        for (e in range(32)) {
                            let by = byte_u8(wq, sb * 128 + g * 16 + e % 16)
                            let q = float((by >> ((e / 16) * 4)) & 0xF)
                            let w = float(float16(dv * (q - 8.0)))
                            acc += w * half_at(xf, (row0 + r) * n + s * 256 + g * 32 + e)
                        }
                    }
                }
                y[(row0 + r) * d + c] = acc
            }
        }
    }
}

def iq4xsf16_gemm_oracle(wq : array<uint>; wsu : array<uint>; xf : array<uint>;
                         recs : array<uint>; nrec, n, d : int; var y : array<float>) {
    let nsb = n / 256
    let lut = iq4nl_lut()
    for (rr in range(nrec)) {
        let wsb0 = int(recs[rr * 4])
        let row0 = int(recs[rr * 4 + 1])
        let cnt = int(recs[rr * 4 + 2])
        for (r in range(cnt)) {
            for (c in range(d)) {
                var acc = 0.0
                for (s in range(nsb)) {
                    let sb = wsb0 + c * nsb + s
                    let dv = unpackHalf2x16(wsu[sb * 5]).x
                    for (g in range(8)) {
                        let sc = float(int(wsu[sb * 5 + 1 + g / 4] << uint((3 - g % 4) * 8)) >> 24)
                        for (e in range(32)) {
                            let by = byte_u8(wq, sb * 128 + g * 16 + e % 16)
                            let q = (by >> ((e / 16) * 4)) & 0xF
                            let w = float(float16(dv * sc * float(lut[q])))
                            acc += w * half_at(xf, (row0 + r) * n + s * 256 + g * 32 + e)
                        }
                    }
                }
                y[(row0 + r) * d + c] = acc
            }
        }
    }
}

def k3f16_gemm_oracle(wq : array<uint>; wsu : array<uint>; xf : array<uint>;
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
                    let dv = unpackHalf2x16(wsu[sb * 5 + 4]).x
                    for (g in range(8)) {
                        for (e in range(32)) {
                            let sidx = g * 2 + e / 16
                            let sc = float(int(wsu[sb * 5 + sidx / 4] << uint((3 - sidx % 4) * 8)) >> 24)
                            let qby = byte_u8(wq, sb * 96 + (g / 4) * 32 + e)
                            let hby = byte_u8(wq, sb * 96 + 64 + e)
                            let q3 = float(((qby >> ((g % 4) * 2)) & 3) - (((hby >> g) & 1) == 0 ? 4 : 0))
                            let w = float(float16(dv * sc * q3))
                            acc += w * half_at(xf, (row0 + r) * n + s * 256 + g * 32 + e)
                        }
                    }
                }
                y[(row0 + r) * d + c] = acc
            }
        }
    }
}
