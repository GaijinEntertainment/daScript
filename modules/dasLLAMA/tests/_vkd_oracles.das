options gen2

// CPU-oracle helpers of the kernel suite that need vulkan-present symbols — the
// shader_lingua_franca exact bodies (sdot4, unpackHalf2x16) and the kq class instances.
// A separate fixture so test_vulkan_kernels.das itself compiles module-less: the CI lint
// bar runs without dasVulkan, and this module drops out whole behind `require ?vulkan`.
module _vkd_oracles shared public

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

// q8n (gguf-native 34B blocks: f16 d + 32 int8) GEMM oracle over region records, f16 x plane
def q8n_gemm_oracle(wq, xf : array<uint>; recs : array<uint>; nrec, n, d : int; var y : array<float>) {
    let nbb = n / 32
    for (rr in range(nrec)) {
        let wblk0 = int(recs[rr * 4])
        let row0 = int(recs[rr * 4 + 1])
        let cnt = int(recs[rr * 4 + 2])
        for (r in range(cnt)) {
            for (c in range(d)) {
                var acc = 0.0
                for (kb in range(nbb)) {
                    let bb = (wblk0 + c * nbb + kb) * 34
                    let dv = half_bytes(wq, bb)
                    for (j in range(32)) {
                        acc += dv * float(byte_s8(wq, bb + 2 + j)) * half_at(xf, (row0 + r) * n + kb * 32 + j)
                    }
                }
                y[(row0 + r) * d + c] = acc
            }
        }
    }
}

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
