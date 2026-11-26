options gen2
options _dasllama_internal = true
options _function_length = 0   // the race's main is one flat ladder: the planes once, then a row count and a depth a pair of rows

// THE STEP'S ATTENTION AT DEPTH, FUSED AGAINST TILE - the two forms a decode or verify step's full-context layer can take
// (`ARCHITECTURE_GPU_BATCH.md#metal-attn-tile`), each with the shipped combine, on `rows` rows of one stream, each a key
// deeper than the last and none on a whole 64-key step, over an f16 mirror: ROWS x DEPTHS in one process, so both sides
// of the far count are timed side by side at every row count. The baseline arm is `fused` (`MetalSqAttnD*F16`); `tile` is
// `MetalSqAttnTileF16`, or `tile-packed` (`MetalSqAttnTilePkF16`) where a K/V head's q heads are no multiple of eight or
// the trailing `pack` asks. The compare region is the deepest row's output, every head: `fused` is held to a CPU softmax
// attention over the same halves, the tile arm to `fused`. Both arms dispatch the shipped stamps through their generated
// builders on the race's own planes; the K/V planes hold the deepest depth once, and a shallower depth walks them a slot a
// round, each arm on its own count, so its rows stream past the device's last-level cache. A pair burns 150 ms of both
// arms, drops its first round and reports each arm's best of five.
//
//   bin/daslang -jit modules/dasLLAMA/harness/attn_tile_race.das [-- <heads> <hs> <kv_heads> [pack]]

require math
require metal/msl_shader
require metal/das_metal_boost
require ?das_metal das_metal
require dasllama/dasllama_metal_common
require dasllama/dasllama_metal_kernels
require _race_fixture

let DEPTHS <- [2048, 4096, 8192, 16384, 32768, 65536, 131072]
let ROWS <- [1, 2, 3, 4, 8]
let HELD_REL = 0.02      //! the compare region's bound, of its largest reference element
let TAIL_SHY = 7         //! the keys the deepest row stops short of its slot: every row ends inside a step

//! row `cnt - 1`'s attention over keys `[0, cnt)` of a slot at element `base`, every head: softmax(q K^T x scale) V on the halves
def private cpu_attn(q : array<float>; qoff : int; kh, vh : array<float16>; base, cnt : int; sh : AttnRaceShape) : array<float> {
    var out : array<float>
    out |> resize(sh.qd)
    var p : array<float>
    p |> resize(cnt)
    let scale = 1.0 / sqrt(float(sh.hs))
    for (h in range(sh.heads)) {
        let kvo = (h / sh.kv_mul) * sh.hs
        var top = -3.0e38
        for (j in range(cnt)) {
            var dot = 0.0
            let kr = base + j * sh.kv_dim + kvo
            for (d in range(sh.hs)) {
                dot += q[qoff + h * sh.hs + d] * float(kh[kr + d])
            }
            p[j] = dot * scale
            top = max(top, p[j])
        }
        var sum = 0.0
        for (j in range(cnt)) {
            p[j] = exp(p[j] - top)
            sum += p[j]
        }
        for (j in range(cnt)) {
            let w = p[j] / sum
            let vr = base + j * sh.kv_dim + kvo
            for (d in range(sh.hs)) {
                out[h * sh.hs + d] += w * float(vh[vr + d])
            }
        }
    }
    delete p
    return <- out
}

//! `count` floats of a device plane from float `first` on
def private plane_floats(buf : MetalBuffer?; first, count : int) : array<float> {
    var out : array<float>
    out |> resize(count)
    unsafe {
        memcpy(addr(out[0]), reinterpret<float?>(metal_buffer_contents(buf)) + first, int64(count) * 4l)
    }
    return <- out
}

[export]
def main : int {
    var tally_ = RaceTally()
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let argv <- get_command_line_arguments()
        var inscope args <- [for (a in argv); a; where a != "pack"]
        let asked = length(args) != length(argv)
        let dims <- race_int_args(args, [8, 512, 1])   // gemma-4-E2B's full-context layer
        let sh = attn_race_shape(dims[0], dims[1], dims[2])
        let pack = asked || sh.kv_mul % 8 != 0
        if (!attn_tile_layer_ok(int64(sh.hs), int64(sh.kv_mul), 0u, 0u, pack)) {
            print("attn_tile_race: no {pack ? "packed " : ""}tile stamp serves {sh.heads} heads of {sh.hs} over {sh.kvh} K/V heads\n")
            return race_exit(tally_, "attn_tile_race")
        }
        let arm = pack ? "tile-packed" : "tile"
        let halves = int(attn_d_halves(int64(sh.hs)))
        let maxd = DEPTHS[length(DEPTHS) - 1]
        let maxr = ROWS[length(ROWS) - 1]
        var dev = metal_create_system_default_device()
        var queue = metal_new_command_queue(dev)
        var err = ""
        var pd = (sh.hs == 512 ? pipeline_from_source(dev, MetalSqAttnDXF16_metal_sq_attn_d_msl, MetalSqAttnDXF16_metal_sq_attn_d_msl_entry, MetalSqAttnDXF16_metal_sq_attn_d_msl_fastmath, err)
            : sh.hs == 256 ? pipeline_from_source(dev, MetalSqAttnDWF16_metal_sq_attn_d_msl, MetalSqAttnDWF16_metal_sq_attn_d_msl_entry, MetalSqAttnDWF16_metal_sq_attn_d_msl_fastmath, err)
            : pipeline_from_source(dev, MetalSqAttnDF16_metal_sq_attn_d_msl, MetalSqAttnDF16_metal_sq_attn_d_msl_entry, MetalSqAttnDF16_metal_sq_attn_d_msl_fastmath, err))
        var pt = (pack ? pipeline_from_source(dev, metal_sq_attn_tile_pk_msl, metal_sq_attn_tile_pk_msl_entry, metal_sq_attn_tile_pk_msl_fastmath, err)
            : pipeline_from_source(dev, metal_sq_attn_tile_msl, metal_sq_attn_tile_msl_entry, metal_sq_attn_tile_msl_fastmath, err))
        var pc = pipeline_from_source(dev, metal_sq_attn_comb_d_msl, metal_sq_attn_comb_d_msl_entry, metal_sq_attn_comb_d_msl_fastmath, err)
        if (pd == null || pt == null || pc == null) {
            print("attn_tile_race: REFUSED: a pipeline did not build ({err})\n")
            tally(tally_, -1)
            return race_exit(tally_, "attn_tile_race")
        }
        print("step attention, fused against {arm}: {sh.heads} heads of {sh.hs} over {sh.kvh} K/V heads (kv_mul {sh.kv_mul}), kv_dim {sh.kv_dim}, rows of one stream\n")
        var kh <- half_of(race_fill(maxd * sh.kv_dim, 2, 0.5, 0.0, 7u))
        var vh <- half_of(race_fill(maxd * sh.kv_dim, 3, 0.5, 0.0, 10u))
        var qf <- race_fill(maxr * sh.qstride, 1, 0.5, 0.0, 3u)
        var bkh = metal_buffer_from(dev, uint64(long_length(kh)) * 2ul, unsafe(addr<void?>(kh[0])))
        var bvh = metal_buffer_from(dev, uint64(long_length(vh)) * 2ul, unsafe(addr<void?>(vh[0])))
        var bq = metal_buffer_from(dev, uint64(long_length(qf)) * 4ul, unsafe(addr<void?>(qf[0])))
        let entries_mx = attn_d_entries(attn_d_groups(int64(maxd)), int64(maxd))
        let part_bytes = uint64(attn_d_part_floats(int64(maxr), int64(sh.heads), int64(sh.hs), entries_mx) * 4l)
        var bpart_a = metal_buffer_from(dev, part_bytes, null)
        var bpart_b = metal_buffer_from(dev, part_bytes, null)
        var by_a = metal_buffer_from(dev, uint64(maxr) * uint64(sh.qd) * 4ul, null)
        var by_b = metal_buffer_from(dev, uint64(maxr) * uint64(sh.qd) * 4ul, null)
        var ones <- [for (_ in range(sh.heads * halves)); 1.0]
        var bsink = metal_buffer_from(dev, uint64(sh.heads * halves * 4), unsafe(addr<void?>(ones[0])))   // the combine's sink plane, unread with hass 0
        let scale = 1.0 / sqrt(float(sh.hs))
        print("arm | rows | depth | slots | ms | Gfma/ms | lead | compare\n")
        for (nrows in ROWS) {
            for (depth in DEPTHS) {
                let nsgs = attn_d_groups(int64(depth))
                let pieces = attn_tile_pieces(int64(depth))
                let nslots = maxd / depth
                // a route table a slot: one stream's rows at the slot's base, row r a key deeper than row r - 1, the last at `depth`
                var rts : array<MetalBuffer?>   // released below; the array holds borrowed handles, so it is cleared, never deleted
                rts |> reserve(nslots)
                for (slot in range(nslots)) {
                    var inscope rt <- [for (r in range(nrows)); uint4(uint(slot * depth * sh.kv_dim), uint(slot * depth * sh.kv_dim), uint(depth), uint(depth - TAIL_SHY - nrows + 1 + r))]
                    rts |> push(metal_buffer_from(dev, uint64(nrows) * 16ul, unsafe(addr<void?>(rt[0]))))
                }
                var kad = SqAttnDArgs(lay = 0u, layr = 0u, ring = 0u, kstride = uint(sh.kv_dim), nheads = uint(sh.heads), kv_mul = uint(sh.kv_mul),
                    hs = uint(sh.hs), scale = scale, qstride = uint(sh.qstride), nsgs = uint(nsgs), window = 0u, softcap = 0.0, qhs = 0u, gated = 0u)
                var kacd = SqAttnCombDArgs(nchmax = uint(nsgs), nheads = uint(sh.heads * halves), hs = uint(sh.hs / halves), hass = 0u, hdiv = uint(halves))
                var kat = SqAttnTileArgs(lay = 0u, layr = 0u, kstride = uint(sh.kv_dim), nheads = uint(sh.heads), kv_mul = uint(sh.kv_mul),
                    hs = uint(sh.hs), scale = scale, qstride = uint(sh.qstride), keys = uint(depth), rows = uint(nrows))
                var kact = SqAttnCombDArgs(nchmax = uint(pieces), nheads = uint(sh.heads * halves), hs = uint(sh.hs / halves), hass = 0u, hdiv = uint(halves))
                var td = -1.0lf
                var tt = -1.0lf
                var sa = 0   // each arm walks the slots on its own count: the two arms' last rounds land on the same slot
                var sb = 0
                race_pair_ms(queue, 5, td, tt, $(enc) {
                    sa = (sa + 1) % nslots
                    if (sh.hs == 512) {
                        enc_attndx16_c_pso(enc, pd, bkh, bvh, bq, bpart_a, rts[sa], 0ul, kad, int64(sh.heads), nsgs, int64(nrows))
                    } elif (sh.hs == 256) {
                        enc_attndw16_c_pso(enc, pd, bkh, bvh, bq, bpart_a, rts[sa], 0ul, kad, int64(sh.heads), nsgs, int64(nrows))
                    } else {
                        enc_attnd16_c_pso(enc, pd, bkh, bvh, bq, bpart_a, rts[sa], 0ul, kad, int64(sh.heads), nsgs, int64(nrows))
                    }
                    enc_attncombd_c_pso(enc, pc, bpart_a, by_a, rts[sa], 0ul, kacd, bsink, int64(sh.heads * halves), int64(nrows))
                }, $(enc) {
                    sb = (sb + 1) % nslots
                    if (pack) {
                        enc_attn_tilepk16_c_pso(enc, pt, bkh, bvh, bq, bpart_b, rts[sb], 0ul, kat, pieces,
                            int64(sh.kvh) * attn_tile_pack_chunks(int64(nrows), int64(sh.kv_mul)))
                    } else {
                        enc_attn_tile16_c_pso(enc, pt, bkh, bvh, bq, bpart_b, rts[sb], 0ul, kat, pieces, int64(sh.heads / 8), int64(nrows))
                    }
                    enc_attncombd_c_pso(enc, pc, bpart_b, by_b, rts[sb], 0ul, kact, bsink, int64(sh.heads * halves), int64(nrows))
                })
                for (b in rts) {
                    metal_release(b)
                }
                rts |> clear()
                let head = "| {nrows} | {depth} | {nslots} |"
                if (td < 0.0lf || tt < 0.0lf) {
                    print("fused {head} REFUSED: dispatch failed\n{arm} {head} REFUSED: dispatch failed\n")
                    tally(tally_, -1)
                    continue
                }
                // both arms' last rounds read slot `sa`; the compare region is the deepest row's output
                var inscope want <- cpu_attn(qf, (nrows - 1) * sh.qstride, kh, vh, sa * depth * sh.kv_dim, depth - TAIL_SHY, sh)
                var inscope got_a <- plane_floats(by_a, (nrows - 1) * sh.qd, sh.qd)
                var inscope got_b <- plane_floats(by_b, (nrows - 1) * sh.qd, sh.qd)
                let ha = held(got_a, want, HELD_REL)
                let hb = held(got_b, got_a, HELD_REL)
                let gfma = double(nrows * sh.heads) * double(depth) * double(sh.hs) * 2.0lf / 1.0e9lf
                let region = "the deepest row's {sh.qd} outputs"
                if (ha.ok) {
                    print("fused {head} {td} | {gfma / td} | | {compare_words(ha, "the CPU attention", region)}\n")
                } else {
                    print("fused {head} {compare_words(ha, "the CPU attention", region)}\n")
                }
                if (ha.ok && hb.ok) {
                    print("{arm} {head} {tt} | {gfma / tt} | {td / tt} | {compare_words(hb, "fused", region)}\n")
                    tally(tally_, 2)
                } else {
                    print("{arm} {head} {compare_words(hb, "fused", region)}\n")
                    tally(tally_, -1)
                }
            }
        }
    } else {
        print("attn_tile_race: das_metal is not built on this platform\n")
    }
    return race_exit(tally_, "attn_tile_race")
}
