options gen2
options _dasllama_internal = true
options _function_length = 0   // the race's main is one flat ladder: the planes once, then a depth a row

// THE FUSED DECODE ATTENTION BY K/V CODEC AT DEPTH - the shipped single-pass stamps (attn_d + the combine) timed on one
// decode step's attention of a Qwen3-30B-A3B-shaped layer (32 query heads of 128 over 4 K/V heads, two rows) at depths
// 512 to 16384, the f16 mirror against the q8_0 and tq4 block codecs. Every arm dispatches the shipped stamps through
// their generated builders on the race's own planes; the figure a codec earns is the bytes its K/V rows hold a key over
// its time - the slope the per-step decode trace reads as the codec's cost past 512 keys. The K/V planes are a ring of
// RING_SLABS slabs at the deepest depth, every round of a pair on the next slot of its own, so a shallow depth streams
// its rows past the device's last-level cache rather than re-reading one slab from it.
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
require dasllama/dasllama_kv_dtype   // kv_row_bytes: a codec's bytes a row
require _race_fixture

let DEPTHS <- [512, 2048, 8192, 16384]
let RING_SLABS = 8   //! slabs of the deepest depth a plane holds: 256 MiB of f16 rows a plane on the 30B shape

//! `copies` of `src` back to back
def private tiled(src : array<float>; copies : int) : array<float> {
    var out : array<float>
    out |> reserve(length(src) * copies)   // the ring's planes pass the unreserved-growth guard
    out |> resize(length(src) * copies)
    for (c in range(copies)) {
        unsafe {
            memcpy(addr(out[c * length(src)]), addr(src[0]), long_length(src) * 4l)
        }
    }
    return <- out
}

//! a K or V slab of `rows` keys a stream over `nrows` streams, every element pseudo-random in [-0.5, 0.5)
def private slab_rows(nrows, rows, kv_dim, salt : int) : array<float> => race_fill(nrows * rows * kv_dim, salt, 0.5, 0.0, uint(3 * salt + 1))

//! the rows' route table on the device: row r's K and V base at `base + r * stride` (elements under f16, bytes under a block
//! codec), its capacity `cap` and its count `cnt`
def private route_buf(dev : MetalDevice?; nrows, stride, cap, cnt, base : int) : MetalBuffer? {
    var inscope rt <- [for (r in range(nrows)); uint4(uint(base + r * stride), uint(base + r * stride), uint(cap), uint(cnt))]
    return metal_buffer_from(dev, uint64(nrows) * 16ul, unsafe(addr<void?>(rt[0])))
}

//! the slab quantized through the shipped codec store: the rows are contiguous, so one call quantizes every block
def private slab_blocks(src : array<float>; nrow_total, kv_dim : int; dt : KVDtype) : array<uint8> {
    var out : array<uint8>
    out |> reserve(int(kv_row_bytes(dt, int64(kv_dim))) * nrow_total)
    out |> resize(int(kv_row_bytes(dt, int64(kv_dim))) * nrow_total)
    unsafe {
        if (dt == KVDtype.tq4) {
            quantize_tq4kv_row(addr(out[0]), addr(src[0]), int64(nrow_total * kv_dim))
        } else {
            quantize_q8kv_row(addr(out[0]), addr(src[0]), int64(nrow_total * kv_dim))
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
        print("fused decode attention by codec: {nrows} rows, {heads} heads of {hs} over {kvh} K/V heads (kv_mul {kv_mul}), kv_dim {kv_dim}, a ring of {RING_SLABS} slabs of {maxd} keys\n")
        // the planes: a ring of RING_SLABS slabs at the deepest depth; a shallower depth packs more slots into the same bytes
        var kf <- tiled(slab_rows(nrows, maxd, kv_dim, 2), RING_SLABS)
        var vf <- tiled(slab_rows(nrows, maxd, kv_dim, 3), RING_SLABS)
        var kh <- half_of(kf)
        var vh <- half_of(vf)
        var k8 <- slab_blocks(kf, nrows * maxd * RING_SLABS, kv_dim, KVDtype.q8_0)
        var v8 <- slab_blocks(vf, nrows * maxd * RING_SLABS, kv_dim, KVDtype.q8_0)
        var k4 <- slab_blocks(kf, nrows * maxd * RING_SLABS, kv_dim, KVDtype.tq4)
        var v4 <- slab_blocks(vf, nrows * maxd * RING_SLABS, kv_dim, KVDtype.tq4)
        var qf <- race_fill(nrows * qstride, 1, 0.5, 0.0, 3u)
        var bkh = metal_buffer_from(dev, uint64(long_length(kh)) * 2ul, unsafe(addr<void?>(kh[0])))
        var bvh = metal_buffer_from(dev, uint64(long_length(vh)) * 2ul, unsafe(addr<void?>(vh[0])))
        var bk8 = metal_buffer_from(dev, uint64(long_length(k8)), unsafe(addr<void?>(k8[0])))
        var bv8 = metal_buffer_from(dev, uint64(long_length(v8)), unsafe(addr<void?>(v8[0])))
        var bk4 = metal_buffer_from(dev, uint64(long_length(k4)), unsafe(addr<void?>(k4[0])))
        var bv4 = metal_buffer_from(dev, uint64(long_length(v4)), unsafe(addr<void?>(v4[0])))
        var bq = metal_buffer_from(dev, uint64(long_length(qf)) * 4ul, unsafe(addr<void?>(qf[0])))
        let nsg_max = int(attn_d_groups(int64(maxd)))
        let part_bytes = uint64(nrows) * uint64(heads) * uint64(nsg_max) * uint64(hs + 4) * 4ul
        var bpart_a = metal_buffer_from(dev, part_bytes, null)
        var bpart_b = metal_buffer_from(dev, part_bytes, null)
        var by_a = metal_buffer_from(dev, uint64(nrows) * uint64(qd) * 4ul, null)
        var by_b = metal_buffer_from(dev, uint64(nrows) * uint64(qd) * 4ul, null)
        var ones <- [for (_ in range(heads)); 1.0]
        var bsink = metal_buffer_from(dev, uint64(heads * 4), unsafe(addr<void?>(ones[0])))   // the combine's sink plane, unread with hass 0
        let scale = 1.0 / sqrt(float(hs))
        let nblk = kv_dim / 32
        let row8 = int(kv_row_bytes(KVDtype.q8_0, int64(kv_dim)))   // a row's bytes under each block codec
        let row4 = int(kv_row_bytes(KVDtype.tq4, int64(kv_dim)))
        print("depth | nsgs | slots | f16 ms | q8_0 ms | tq4 ms | f16 GB/s | q8_0 GB/s | tq4 GB/s | q8_0 vs f16 out\n")
        for (depth in DEPTHS) {
            let nsgs = int(attn_d_groups(int64(depth)))
            let nslots = RING_SLABS * maxd / depth   // the ring's slots at this depth: a slot a row's region of `depth` keys
            // a route table a slot: the rows' K and V bases (elements under f16, bytes under a block codec), capacity and count
            var rt16, rt8, rt4 : array<MetalBuffer?>   // released below; the arrays hold borrowed handles, so they are cleared, never deleted
            rt16 |> reserve(nslots)
            rt8 |> reserve(nslots)
            rt4 |> reserve(nslots)
            for (slot in range(nslots)) {
                rt16 |> push(route_buf(dev, nrows, depth * kv_dim, depth, depth, slot * nrows * depth * kv_dim))
                rt8 |> push(route_buf(dev, nrows, depth * row8, depth, depth, slot * nrows * depth * row8))
                rt4 |> push(route_buf(dev, nrows, depth * row4, depth, depth, slot * nrows * depth * row4))
            }
            var ka16 = SqAttnDArgs(lay = 0u, layr = 0u, ring = 0u, kstride = uint(kv_dim), nheads = uint(heads), kv_mul = uint(kv_mul), hs = uint(hs),
                scale = scale, qstride = uint(qstride), nsgs = uint(nsgs), window = 0u, softcap = 0.0, qhs = 0u, gated = 0u)
            var kaq = ka16
            kaq.kstride = uint(nblk)
            var kac = SqAttnCombDArgs(nchmax = uint(nsgs), nheads = uint(heads), hs = uint(hs), hass = 0u, hdiv = 1u)
            var t16 = -1.0lf
            var t8 = -1.0lf
            var t4 = -1.0lf
            var t16b = -1.0lf
            var sa = 0   // each arm walks the ring on its own count: the two arms' last rounds land on the same slot
            var sb = 0
            race_pair_ms(queue, 5, t16, t8, $(enc) {
                sa = (sa + 1) % nslots
                enc_attnd16_c_pso(enc, p16, bkh, bvh, bq, bpart_a, rt16[sa], 0ul, ka16, int64(heads), int64(nsgs), int64(nrows))
                enc_attncombd_c_pso(enc, pc, bpart_a, by_a, rt16[sa], 0ul, kac, bsink, int64(heads), int64(nrows))
            }, $(enc) {
                sb = (sb + 1) % nslots
                enc_attnd_q8_c_pso(enc, pq8, bk8, bk8, bv8, bv8, bq, bpart_b, rt8[sb], 0ul, kaq, int64(heads), int64(nsgs), int64(nrows))
                enc_attncombd_c_pso(enc, pc, bpart_b, by_b, rt8[sb], 0ul, kac, bsink, int64(heads), int64(nrows))
            })
            var note = ""
            race_envelope_ok(by_a, by_b, nrows * qd, note)
            sa = 0
            sb = 0
            race_pair_ms(queue, 5, t16b, t4, $(enc) {
                sa = (sa + 1) % nslots
                enc_attnd16_c_pso(enc, p16, bkh, bvh, bq, bpart_a, rt16[sa], 0ul, ka16, int64(heads), int64(nsgs), int64(nrows))
                enc_attncombd_c_pso(enc, pc, bpart_a, by_a, rt16[sa], 0ul, kac, bsink, int64(heads), int64(nrows))
            }, $(enc) {
                sb = (sb + 1) % nslots
                enc_attnd_tq4_c_pso(enc, ptq, bk4, bk4, bv4, bv4, bq, bpart_b, rt4[sb], 0ul, kaq, int64(heads), int64(nsgs), int64(nrows))
                enc_attncombd_c_pso(enc, pc, bpart_b, by_b, rt4[sb], 0ul, kac, bsink, int64(heads), int64(nrows))
            })
            let keys = double(nrows * depth)
            let gb16 = keys * double(kv_dim) * 2.0lf * 2.0lf / 1.0e9lf
            let gb8 = keys * double(row8) * 2.0lf / 1.0e9lf
            let gb4 = keys * double(row4) * 2.0lf / 1.0e9lf
            let tf = min(t16, t16b)
            print("{depth} | {nsgs} | {nslots} | {tf} | {t8} | {t4} | {gb16 / (tf / 1000.0lf)} | {gb8 / (t8 / 1000.0lf)} | {gb4 / (t4 / 1000.0lf)} | {note}\n")
            for (b in rt16) {
                metal_release(b)
            }
            for (b in rt8) {
                metal_release(b)
            }
            for (b in rt4) {
                metal_release(b)
            }
            rt16 |> clear()
            rt8 |> clear()
            rt4 |> clear()
        }
    } else {
        print("attn_d_codec_race: das_metal is not built on this platform\n")
    }
}
