options gen2
options indenting = 4
options _dasllama_internal = true

//! the CPU references of the CUDA home's MMQ leaves over word planes: the GEMM race and the kernel cells read one copy

require dasllama/dasllama_kqformat   // KqFmt, MX4_LUT
require dasllama/dasllama_gpu_kernels_discrete   // iq4_word, iq4xs_sc, q40_d_of - the leaves' own decodes
require dasllama/dasllama_gpu_math   // ue4m3_half_f32
require dasllama/dasllama_convert   // dequant_b32_plane_block
require daslib/shader_lingua_franca   // unpackHalf2x16
require _fp4_feed   // Fp4Feed, fp4_feed_value

//! byte `i` of a word plane as a signed int8
def word_i8_at(words : array<uint>; i : int) : int {
    let b = int((words[i / 4] >> uint((i % 4) * 8)) & 255u)
    return b >= 128 ? b - 256 : b
}

//! a word plane as its bytes
def word_plane_bytes(words : array<uint>) : array<uint8> => [for (i in range(length(words) * 4)); uint8((words[i / 4] >> uint((i % 4) * 8)) & 0xFFu)]

//! each Q8 block's scale x quant sum over `rows` rows of `k` - the min-term leaves' third feed, as the requant kernels write it
def q8_block_sums(xq : array<uint>; xs : array<float>; rows, k : int) : array<float> {
    let nb = k / 32
    var xsum : array<float>
    xsum |> resize(rows * nb)
    for (tok in range(rows)) {
        for (blk in range(nb)) {
            var qsum = 0
            for (i in range(32)) {
                qsum += word_i8_at(xq, tok * k + blk * 32 + i)
            }
            xsum[tok * nb + blk] = xs[tok * nb + blk] * float(qsum)
        }
    }
    return <- xsum
}

//! the Q4_K reference over the engine's repacked superblocks (element i the low nibble, i + 16 the high; five scale words a superblock):
//! every block's xs x (d sc dot - dmin mn qsum)
def k4_mmq_ref(wq, ws, xq : array<uint>; xs : array<float>; k, tok, row : int) : float {
    let nb = k / 32
    let nsb = k / 256
    var acc = 0.0
    for (b in range(nb)) {
        let sb = row * nsb + b / 8
        let g = b % 8
        let dm = unpackHalf2x16(ws[sb * 5])
        let sc = float((ws[sb * 5 + 1 + g / 4] >> uint((g % 4) * 8)) & 255u)
        let mn = float((ws[sb * 5 + 3 + g / 4] >> uint((g % 4) * 8)) & 255u)
        var dot = 0
        var qsum = 0
        for (i in range(32)) {
            let wb = (sb * 8 + g) * 16 + i % 16
            let byte = int((wq[wb / 4] >> uint((wb % 4) * 8)) & 255u)
            let xv = word_i8_at(xq, tok * k + b * 32 + i)
            dot += (i < 16 ? byte & 15 : byte >> 4) * xv
            qsum += xv
        }
        acc += xs[tok * nb + b] * (dm.x * sc * float(dot) - dm.y * mn * float(qsum))
    }
    return acc
}

//! the min-term-free superblock leaves over the Q4_K nibble tiling - IQ4_XS's codebook under d x (ls - 32) off its two-word scale row,
//! Q4_0's nibbles less eight and IQ4_NL's codebook under the five-word row's d pairs: every block's xs x d x dot
def sb_mmq_ref(fmt : int; wq, ws, xq : array<uint>; xs : array<float>; k, tok, row : int) : float {
    let nb = k / 32
    let nsb = k / 256
    let is_codebook = fmt != int(KqFmt.q40)
    let two_word_scales = fmt == int(KqFmt.iq4xs)
    var acc = 0.0
    for (b in range(nb)) {
        let sb = row * nsb + b / 8
        let g = b % 8
        var dot = 0
        for (i in range(32)) {
            let wb = (sb * 8 + g) * 16 + i % 16
            let byte = int((wq[wb / 4] >> uint((wb % 4) * 8)) & 255u)
            let nib = i < 16 ? byte & 15 : byte >> 4
            let code = is_codebook ? int(iq4_word(uint(nib)) & 255u) : nib - 8
            dot += (code >= 128 ? code - 256 : code) * word_i8_at(xq, tok * k + b * 32 + i)
        }
        let sc = two_word_scales ? unpackHalf2x16(ws[sb * 2]).x * float(iq4xs_sc(ws[sb * 2], ws[sb * 2 + 1], uint(g))) : q40_d_of(ws[sb * 5 + g / 2], uint(g))
        acc += float(dot) * sc * xs[tok * nb + b]
    }
    return acc
}

//! a per-32 leaf's reference: the block through the engine's own per-32 dequant, dotted with the activation's quants under its scale
def b32_mmq_ref(f : KqFmt; wq_bytes, ws_bytes : array<uint8>; xq : array<uint>; xs : array<float>; k, tok, row : int) : float {
    let nb = k / 32
    var w : array<float>
    w |> resize(32)
    var acc = 0.0
    for (b in range(nb)) {
        dequant_b32_plane_block(f, wq_bytes, ws_bytes, int64(row * nb + b), w, 0l)
        var dot = 0.0
        for (i in range(32)) {
            dot += w[i] * float(word_i8_at(xq, tok * k + b * 32 + i))
        }
        acc += dot * xs[tok * nb + b]
    }
    return acc
}

//! element `kk` of row `row` of a k4-tiled nv4 plane of `nsb` superblocks a row: the doubled e2m1 magnitude x its strip's UE4M3 code at half
def nv4_weight_at(wq, ws : array<uint>; nsb, row, kk : int) : float {
    let sb = kk / 256
    let g = (kk % 256) / 32
    let e = kk % 32
    let nib = (wq[((row * nsb + sb) * 8 + g) * 4 + (e % 16) / 4] >> uint(((e % 16) % 4) * 8 + (e / 16) * 4)) & 15u
    let strip = g * 2 + e / 16
    let code = (ws[(row * nsb + sb) * 5 + strip / 4] >> uint((strip % 4) * 8)) & 0xFFu
    return float(MX4_LUT[int(nib)]) * ue4m3_half_f32(code)
}

//! an mx4 plane's weight at element `kk` of row `row` (`nb` blocks a row, block `wb0` the plane's first): e2m1 x the block's UE8M0, the k4-tiled nibbles read as the FP4 leaf does (element j low in byte j, j + 16 high)
def mx4_fp4_weight_at(wq, ws : array<uint>; wb0, nb, row, kk : int) : float {
    let b = wb0 + row * nb + kk / 32
    let e = kk % 32
    let nib = (wq[b * 4 + (e % 16) / 4] >> uint(((e % 16) % 4) * 8 + (e / 16) * 4)) & 15u
    let code = (ws[b / 4] >> uint((b % 4) * 8)) & 0xFFu
    return float(MX4_LUT[int(nib)]) * e8m0_half_f32(code)
}

//! the mx4 FP4 leaf's reference: the plane's row against the MXFP4 feed's row
def mx4_fp4_mmq_ref(wq, ws : array<uint>; feed : Fp4Feed; wb0, k, tok, row : int) : float {
    let nb = k / 32
    var acc = 0.0
    for (kk in range(k)) {
        acc += mx4_fp4_weight_at(wq, ws, wb0, nb, row, kk) * fp4_feed_value(feed, k, tok, kk, true)
    }
    return acc
}

//! the FP4 leaf's reference: the plane's row against the feed's row, under the plane's site scale
def nv4_mmq_ref(wq, ws : array<uint>; feed : Fp4Feed; k, tok, row : int; site_scale : float) : float {
    let nsb = k / 256
    var acc = 0.0
    for (kk in range(k)) {
        acc += nv4_weight_at(wq, ws, nsb, row, kk) * fp4_feed_value(feed, k, tok, kk, false)
    }
    return acc * site_scale
}
