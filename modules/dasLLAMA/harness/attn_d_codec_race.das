options gen2
options _dasllama_internal = true

// THE FUSED DECODE ATTENTION BY K/V CODEC AT DEPTH - the shipped single-pass stamps (attn_d + the combine) timed on one
// decode step's attention of a Qwen3-30B-A3B-shaped layer (32 query heads of 128 over 4 K/V heads, two rows) at depths
// 512 to 16384, the f16 mirror against the q8_0 and tq4 block codecs. Every arm dispatches the shipped stamps through
// their generated builders on the race's own planes; the figure a codec earns is the bytes its K/V rows hold a key over
// its time - the slope the per-step decode trace reads as the codec's cost past 512 keys.
//
//   bin/daslang -jit modules/dasLLAMA/harness/attn_d_codec_race.das [-- <rows> <heads> <hs> <kv_heads>]

require math
require strings
require metal/msl_shader
require metal/das_metal_boost
require ?das_metal das_metal
require dasllama/dasllama_metal_common
require dasllama/dasllama_metal_kernels
require dasllama/dasllama_kv_codec
require _race_fixture

let DEPTHS <- [512, 2048, 8192, 16384]
let Q8_BLOCK = 34
let TQ4_BLOCK = 18

//! a K or V slab of `rows` keys a stream over `nrows` streams, every element pseudo-random in [-0.5, 0.5)
def private slab_rows(nrows, rows, kv_dim, salt : int) : array<float> => race_fill(nrows * rows * kv_dim, salt, 0.5, 0.0, uint(3 * salt + 1))

//! the slab as the f16 mirror holds it
def private half_of(src : array<float>) : array<float16> {
    var xa : array<float16>
    xa |> resize(length(src))
    for (d, s in xa, src) {
        d = float16(s)
    }
    return <- xa
}

//! the slab quantized row by row through the shipped codec store: `blk` bytes a 32-block
def private slab_blocks(src : array<float>; nrow_total, kv_dim, blk : int; tq4 : bool) : array<uint8> {
    var out : array<uint8>
    out |> resize(nrow_total * (kv_dim / 32) * blk)
    for (r in range(nrow_total)) {
        unsafe {
            var dst = addr(out[r * (kv_dim / 32) * blk])
            let s = addr(src[r * kv_dim])
            if (tq4) {
                quantize_tq4kv_row(dst, s, int64(kv_dim))
            } else {
                quantize_q8kv_row(dst, s, int64(kv_dim))
            }
        }
    }
    return <- out
}

[export]
def main {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let args <- get_command_line_arguments()
        var nrows = 2
        var heads = 32
        var hs = 128
        var kvh = 4
        if (length(args) >= 4 && to_int(args[length(args) - 4]) > 0) {
            nrows = to_int(args[length(args) - 4])
            heads = to_int(args[length(args) - 3])
            hs = to_int(args[length(args) - 2])
            kvh = to_int(args[length(args) - 1])
        }
        let kv_dim = kvh * hs
        let kv_mul = heads / kvh
        let qd = heads * hs
        let qstride = qd + 2 * kv_dim   // the decode's [q | k | v] row
        let maxd = DEPTHS[length(DEPTHS) - 1]
        var dev = metal_create_system_default_device()
        var queue = metal_new_command_queue(dev)
        var err = ""
        var p16 = pipeline_from_source(dev, MetalSqAttnDF16_metal_sq_attn_d_msl, MetalSqAttnDF16_metal_sq_attn_d_msl_entry, MetalSqAttnDF16_metal_sq_attn_d_msl_fastmath, err)
        var pq8 = pipeline_from_source(dev, MetalSqAttnDQ8_metal_sq_attn_d_msl, MetalSqAttnDQ8_metal_sq_attn_d_msl_entry, MetalSqAttnDQ8_metal_sq_attn_d_msl_fastmath, err)
        var ptq = pipeline_from_source(dev, MetalSqAttnDTq4_metal_sq_attn_d_msl, MetalSqAttnDTq4_metal_sq_attn_d_msl_entry, MetalSqAttnDTq4_metal_sq_attn_d_msl_fastmath, err)
        var pc = pipeline_from_source(dev, metal_sq_attn_comb_d_msl, metal_sq_attn_comb_d_msl_entry, metal_sq_attn_comb_d_msl_fastmath, err)
        if (p16 == null || pq8 == null || ptq == null || pc == null) {
            print("pso: {err}\n")
            return
        }
        print("fused decode attention by codec: {nrows} rows, {heads} heads of {hs} over {kvh} K/V heads (kv_mul {kv_mul}), kv_dim {kv_dim}\n")
        // the planes at the deepest depth; a shallower depth reads a prefix of each row's slab
        var kf <- slab_rows(nrows, maxd, kv_dim, 2)
        var vf <- slab_rows(nrows, maxd, kv_dim, 3)
        var kh <- half_of(kf)
        var vh <- half_of(vf)
        var k8 <- slab_blocks(kf, nrows * maxd, kv_dim, Q8_BLOCK, false)
        var v8 <- slab_blocks(vf, nrows * maxd, kv_dim, Q8_BLOCK, false)
        var k4 <- slab_blocks(kf, nrows * maxd, kv_dim, TQ4_BLOCK, true)
        var v4 <- slab_blocks(vf, nrows * maxd, kv_dim, TQ4_BLOCK, true)
        var qf <- race_fill(nrows * qstride, 1, 0.5, 0.0, 3u)
        var bkh = race_buf(dev, uint64(length(kh) * 2), unsafe(addr<void?>(kh[0])))
        var bvh = race_buf(dev, uint64(length(vh) * 2), unsafe(addr<void?>(vh[0])))
        var bk8 = race_buf(dev, uint64(length(k8)), unsafe(addr<void?>(k8[0])))
        var bv8 = race_buf(dev, uint64(length(v8)), unsafe(addr<void?>(v8[0])))
        var bk4 = race_buf(dev, uint64(length(k4)), unsafe(addr<void?>(k4[0])))
        var bv4 = race_buf(dev, uint64(length(v4)), unsafe(addr<void?>(v4[0])))
        var bq = race_buf(dev, uint64(length(qf) * 4), unsafe(addr<void?>(qf[0])))
        let nsg_max = int(attn_d_groups(int64(maxd)))
        var bpart_a = race_buf(dev, uint64(nrows * heads * nsg_max * (hs + 4) * 4), null)
        var bpart_b = race_buf(dev, uint64(nrows * heads * nsg_max * (hs + 4) * 4), null)
        var by_a = race_buf(dev, uint64(nrows * qd * 4), null)
        var by_b = race_buf(dev, uint64(nrows * qd * 4), null)
        var ones <- [for (_ in range(heads)); 1.0]
        var bsink = race_buf(dev, uint64(heads * 4), unsafe(addr<void?>(ones[0])))   // the combine's sink plane, unread with hass 0
        let scale = 1.0 / sqrt(float(hs))
        let nblk = kv_dim / 32
        print("depth | nsgs | f16 ms | q8_0 ms | tq4 ms | f16 GB/s | q8_0 GB/s | tq4 GB/s | q8_0 vs f16 out\n")
        for (depth in DEPTHS) {
            let nsgs = int(attn_d_groups(int64(depth)))
            // the route table: a row's K and V bases (elements under f16, bytes under a block codec), its capacity and its count
            var rt16 : array<uint4>
            var rt8 : array<uint4>
            var rt4 : array<uint4>
            for (r in range(nrows)) {
                rt16 |> push(uint4(uint(r * maxd * kv_dim), uint(r * maxd * kv_dim), uint(maxd), uint(depth)))
                rt8 |> push(uint4(uint(r * maxd * nblk * Q8_BLOCK), uint(r * maxd * nblk * Q8_BLOCK), uint(maxd), uint(depth)))
                rt4 |> push(uint4(uint(r * maxd * nblk * TQ4_BLOCK), uint(r * maxd * nblk * TQ4_BLOCK), uint(maxd), uint(depth)))
            }
            var brt16 = race_buf(dev, uint64(nrows * 16), unsafe(addr<void?>(rt16[0])))
            var brt8 = race_buf(dev, uint64(nrows * 16), unsafe(addr<void?>(rt8[0])))
            var brt4 = race_buf(dev, uint64(nrows * 16), unsafe(addr<void?>(rt4[0])))
            var ka16 = SqAttnDArgs(lay = 0u, layr = 0u, ring = 0u, kstride = uint(kv_dim), nheads = uint(heads), kv_mul = uint(kv_mul), hs = uint(hs),
                scale = scale, qstride = uint(qstride), nsgs = uint(nsgs), window = 0u, softcap = 0.0, qhs = 0u, gated = 0u)
            var kaq = ka16
            kaq.kstride = uint(nblk)
            var kac = SqAttnCombDArgs(nchmax = uint(nsgs), nheads = uint(heads), hs = uint(hs), hass = 0u, hdiv = 1u)
            var t16 = -1.0lf
            var t8 = -1.0lf
            var t4 = -1.0lf
            var t16b = -1.0lf
            race_pair_ms(queue, 5, t16, t8, $(enc) {
                enc_attnd16_c_pso(enc, p16, bkh, bvh, bq, bpart_a, brt16, 0ul, ka16, int64(heads), int64(nsgs), int64(nrows))
                enc_attncombd_c_pso(enc, pc, bpart_a, by_a, brt16, 0ul, kac, bsink, int64(heads), int64(nrows))
            }, $(enc) {
                enc_attnd_q8_c_pso(enc, pq8, bk8, bk8, bv8, bv8, bq, bpart_b, brt8, 0ul, kaq, int64(heads), int64(nsgs), int64(nrows))
                enc_attncombd_c_pso(enc, pc, bpart_b, by_b, brt8, 0ul, kac, bsink, int64(heads), int64(nrows))
            })
            var note = ""
            race_envelope_ok(by_a, by_b, nrows * qd, note)
            race_pair_ms(queue, 5, t16b, t4, $(enc) {
                enc_attnd16_c_pso(enc, p16, bkh, bvh, bq, bpart_a, brt16, 0ul, ka16, int64(heads), int64(nsgs), int64(nrows))
                enc_attncombd_c_pso(enc, pc, bpart_a, by_a, brt16, 0ul, kac, bsink, int64(heads), int64(nrows))
            }, $(enc) {
                enc_attnd_tq4_c_pso(enc, ptq, bk4, bk4, bv4, bv4, bq, bpart_b, brt4, 0ul, kaq, int64(heads), int64(nsgs), int64(nrows))
                enc_attncombd_c_pso(enc, pc, bpart_b, by_b, brt4, 0ul, kac, bsink, int64(heads), int64(nrows))
            })
            let keys = double(nrows * depth)
            let gb16 = keys * double(kv_dim) * 2.0lf * 2.0lf / 1.0e9lf
            let gb8 = keys * double(nblk * Q8_BLOCK) * 2.0lf / 1.0e9lf
            let gb4 = keys * double(nblk * TQ4_BLOCK) * 2.0lf / 1.0e9lf
            let tf = min(t16, t16b)
            print("{depth} | {nsgs} | {tf} | {t8} | {t4} | {gb16 / (tf / 1000.0lf)} | {gb8 / (t8 / 1000.0lf)} | {gb4 / (t4 / 1000.0lf)} | {note}\n")
        }
    } else {
        print("attn_d_codec_race: das_metal is not built on this platform\n")
    }
}
