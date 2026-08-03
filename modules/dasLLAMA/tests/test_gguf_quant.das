options gen2

require dastest/testing_boost public
require dasllama/dasllama_convert  // the codec layer under test — direct require pins the new module API
require dasllama/dasllama_gguf
require math

// Byte-level GGUF quant-format gates on a synthetic in-memory file: the dequant arm must match an
// independently hand-rolled ggml reference, and the exact-transcode rails must be bit-exact against
// the dequant. Token-level gates on real files live in test_parity.das.

// Q5_0 fixture: 3 blocks x 32 elems, 22 bytes each (fp16 d, 4B high bits, 16B split-half nibbles).
// d bit patterns with exact float values; qh covers none/all/mixed high bits; qs is a pseudo-ramp.
let Q5_D_BITS <- fixed_array(0x3C00, 0xB800, 0x3400)
let Q5_D_VALS <- fixed_array(1.0, -0.5, 0.25)
let Q5_QH <- fixed_array(0x00000000, 0xFFFFFFFF, 0x5A5A3CC3)

def private q5_qs_byte(blk, j : int) : uint {
    return uint((j * 7 + blk * 13) & 255)
}

// Independent reference: lane k of block blk (ggml dequantize_row_q5_0 semantics — the high bit of
// lane k is bit k of qh; the nibble is split-half packed).
def private q5_ref(blk, k : int) : float {
    let nib = k < 16 ? int(q5_qs_byte(blk, k)) & 15 : int(q5_qs_byte(blk, k - 16)) >> 4
    let hi = (int(Q5_QH[blk] >> uint(k)) & 1) << 4
    return float((nib | hi) - 16) * Q5_D_VALS[blk]
}

def private push_u16(var b : array<uint8>; v : uint) {
    b |> push(uint8(v & 0xFFu))
    b |> push(uint8((v >> 8u) & 0xFFu))
}

def private push_u32(var b : array<uint8>; v : uint) {
    push_u16(b, v & 0xFFFFu)
    push_u16(b, v >> 16u)
}

def private push_u64(var b : array<uint8>; v : uint64) {
    b |> reserve(length(b) + 8)
    for (sh in range(8)) {
        b |> push(uint8(uint(v >> uint64(sh * 8)) & 0xFFu))
    }
}

// Minimal single-tensor GGUF: v3 header, 0 KVs, one 1-D tensor "t" of `gtype`, data at alignment 32.
def private synth_gguf(gtype : int; n_elem : int64; data : array<uint8>) : array<uint8> {
    var b <- [uint8('G'), uint8('G'), uint8('U'), uint8('F')]
    push_u32(b, 3u)
    push_u64(b, 1ul)   // n_tensors
    push_u64(b, 0ul)   // n_kv
    push_u64(b, 1ul)   // name len
    b |> push(uint8('t'))
    push_u32(b, 1u)    // n_dims
    push_u64(b, uint64(n_elem))
    push_u32(b, uint(gtype))
    push_u64(b, 0ul)   // offset in data blob
    while ((long_length(b) % 32l) != 0l) {
        b |> push(uint8(0))
    }
    b |> push_from(data)
    return <- b
}

def private synth_q5_0_bytes : array<uint8> {
    var data : array<uint8>
    data |> reserve(3 * 22)
    for (blk in range(3)) {
        push_u16(data, Q5_D_BITS[blk])   // fp16 d
        push_u32(data, Q5_QH[blk])       // 32 high bits
        for (j in range(16)) {
            data |> push(uint8(q5_qs_byte(blk, j)))
        }
    }
    return <- data
}

[test]
def test_gguf_q5_0(t : T?) {
    // The dequant arm (gguf_read_tensor_f32) must match the hand-rolled ggml reference exactly.
    t |> run("q5_0 dequant matches ggml reference") @(t : T?) {
        let gg <- synth_gguf(6, 96l, synth_q5_0_bytes())   // GGML_TYPE_Q5_0
        var m <- parse_gguf_meta(gg)
        var deq : array<float>
        deq |> resize(96)
        gguf_read_tensor_f32(m, gg, "t", deq, 0l, 96l)
        for (i in range(96)) {
            t |> success(deq[i] == q5_ref(i / 32, i % 32), "dequant lane must match reference")
        }
        delete m
    }
    // The Q8 transcode is exact by construction: scale = d verbatim, int8 = q - 16, so
    // float(q) * s reproduces every dequant value bit-for-bit.
    t |> run("q5_0 -> q8 transcode is bit-exact") @(t : T?) {
        let gg <- synth_gguf(6, 96l, synth_q5_0_bytes())
        var m <- parse_gguf_meta(gg)
        var q : array<int8>
        var s : array<float>
        q |> resize(96)
        s |> resize(3)
        gguf_transcode_q5_0_to_q8(m, gg, "t", q, s, 0l, 0l, 96l)
        for (blk in range(3)) {
            t |> success(s[blk] == Q5_D_VALS[blk], "scale must be d verbatim")
        }
        for (i in range(96)) {
            t |> success(float(q[i]) * s[i / 32] == q5_ref(i / 32, i % 32), "q8 form must be bit-exact")
        }
        delete m
    }
    // The src_off slice path (fused-tensor splits) must land the same bytes as the full run.
    t |> run("q5_0 transcode src_off slice matches full run") @(t : T?) {
        let gg <- synth_gguf(6, 96l, synth_q5_0_bytes())
        var m <- parse_gguf_meta(gg)
        var q : array<int8>
        var s : array<float>
        q |> resize(96)
        s |> resize(3)
        gguf_transcode_q5_0_to_q8(m, gg, "t", q, s, 0l, 0l, 96l)
        var q2 : array<int8>
        var s2 : array<float>
        q2 |> resize(32)
        s2 |> resize(1)
        gguf_transcode_q5_0_to_q8(m, gg, "t", q2, s2, 0l, 0l, 32l, 32l)
        t |> success(s2[0] == s[1], "sliced scale matches")
        for (i in range(32)) {
            t |> success(q2[i] == q[32 + i], "sliced quants match")
        }
        delete m
    }
}

// Deterministic pseudo-weights with a per-sub-block scale ladder (1x..8x across the 8 sub-blocks
// of a superblock), so a packing regression in the 6-bit split-nibble scale strip shows up as ONE
// band blowing up rather than a slightly worse whole-tensor average.
def private k5_src(sb, i : int64) : float {
    let amp = 0.001 * float(i / 32l + 1l)
    return amp * float(int((i * 37l + sb * 11l) % 31l) - 15)
}

def private sub_rel(x, y : array<float>; off : int64) : float {
    var se = 0.0
    var sx = 0.0
    for (l in range64(32l)) {
        let d = y[off + l] - x[off + l]
        se += d * d
        sx += x[off + l] * x[off + l]
    }
    return sx == 0.0 ? 0.0 : sqrt(se / 32.0) / sqrt(sx / 32.0)
}

[test]
def test_q5k_encoder(t : T?) {
    let nsb = 8l
    let n = nsb * 256l
    // The ceiling is a regression gate, not a spec: measured rel-rmse is ~0.036 on synthetic
    // gaussian and ~0.031 on GLM-4.5-Air's real ffn_down. A mis-packed scale strip lands near 1.0.
    t |> run("q5_k encode round-trip stays within the 5-bit budget") @(t : T?) {
        var x : array<float>
        var y : array<float>
        x |> resize(n)
        y |> resize(n)
        for (sb in range64(nsb)) {
            for (i in range64(256l)) {
                x[sb * 256l + i] = k5_src(sb, i)
            }
        }
        var kq : array<uint8>
        var ks : array<uint8>
        kq |> resize(nsb * 160l)
        ks |> resize(nsb * 20l)
        quantize_k5_plane(x, n, kq, ks, 0l)
        for (sb in range64(nsb)) {
            dequant_k5_plane_superblock(kq, sb * 160l, ks, sb * 20l, y, sb * 256l)
        }
        for (sb in range64(nsb)) {
            for (j in range64(8l)) {
                let rel = sub_rel(x, y, sb * 256l + j * 32l)
                t |> success(rel < 0.15, "superblock {sb} sub-block {j} rel-rmse {rel} must stay under 0.15")
            }
            // the repack + GPU upload read the 20B scale block whole; the 4B tail must stay zeroed
            for (b in range64(16l, 20l)) {
                t |> equal(int(ks[sb * 20l + b]), 0)
            }
        }
        delete x
        delete y
        delete kq
        delete ks
    }
    // All-zero input takes the d == 0 early-out; it must still emit a well-formed, zeroed block.
    t |> run("q5_k encode handles an all-zero superblock") @(t : T?) {
        var x : array<float>
        var y : array<float>
        x |> resize(256l)
        y |> resize(256l)
        var kq : array<uint8>
        var ks : array<uint8>
        kq |> resize(160l)
        ks |> resize(20l)
        quantize_k5_plane(x, 256l, kq, ks, 0l)
        dequant_k5_plane_superblock(kq, 0l, ks, 0l, y, 0l)
        for (i in range64(256l)) {
            t |> equal(y[i], 0.0)
        }
        delete x
        delete y
        delete kq
        delete ks
    }
}
