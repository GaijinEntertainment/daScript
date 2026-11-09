options gen2
options _function_length = 0   // one flat gate per kernel arm: fixture build, dispatch, compare (the tier suites' shape)
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options _dasllama_internal = true
options persistent_heap   // a dastest root frees what it deletes only on this heap; the linear heap holds every carrier a cell deleted until the context ends

require dastest/testing_boost public
require math
require strings
require daslib/enum_trait
require dasllama/dasllama_math
require _compares
require _attn_oracle   // the batched decode attention cells' oracle
require _moe_routing_oracles
require _vk_kq_fixtures
require ?vulkan _vkd_toy
require ?vulkan _vkd_oracles
require ?vulkan _vk_codec_attn
require ?vulkan vulkan
require ?vulkan vulkan/vulkan_boost
require ?vulkan dasllama/dasllama_vulkan_common
require ?vulkan dasllama/dasllama_vulkan_classes
require dasllama/dasllama_gpu_math   // the softcap cells' oracle
require ?vulkan dasllama/dasllama_vulkan_prefill
require ?vulkan dasllama/dasllama_vulkan_decode   // the batched decode attention classes
require ?vulkan dasllama/dasllama_kqformat
require dasllama/dasllama_gemm_schema   // nolint:STYLE029,STYLE030,LINT019 — the darwin world reads its kqformat re-export; the vulkan arm requires kqformat itself
require dasllama/dasllama_common
require dasllama/dasllama_arch_qwen35
require daslib/strings_boost
require daslib/math_bits   // nolint:STYLE030,LINT019 — the vulkan cells' NaN sentinel and read-back planes by bit pattern, unused in the darwin world
require daslib/f16_cvt   // nolint:STYLE030,LINT019 — the half conversions of the vulkan cells, unused in the darwin world
require _discrete_cells   // nolint:STYLE030,LINT019 — the cells both homes run, unused in the darwin world

//! The Vulkan tier's attention classes against their CPU oracles, one cell a class family: decode attention over every
//! codec arm and the f32 mirror, the prefill attention class, the cm2 and KHR flash tiles (plain, windowed, with sinks,
//! gated, softcapped), the batched decode and chunked attention tiles, the h128 coopmat tile and the f16-mirror reader
//! stamps. Skips: every cell without dasVulkan or a Vulkan device; the cm2 arms off a coopmat2 device created in cm2 mode;
//! the KHR arms without KHR cooperative matrices at subgroup 32; the batched decode attention cells on a device under the
//! classes' workgroup footprint; a class rail a device declines.

//! the kernel cells' approx bar with a poison past the expectation's largest magnitude, outside the bar at any scale
let private BAR_APPROX_CTRL = Bar(rel = APPROX_REL, poison_of_max = true, poison_abs = 0.5)

[test(description = "Decode attention with the Q8_0 or Q8_K requant folded into its store matches the CPU oracle row quantized at 1e-4 relative, unsplit and split, heads of 64, 256 and 512, the two-head slab bit for bit the four-head stamp")]
def test_vkd_da_attn_rqk(t0 : T?) {
    t0 |> run("decode attention with the requant folded into its store == the CPU oracle's row, quantized: Q8_0 and Q8_K, the pass and the combine, the 256 and 512 heads") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!da_attn_stamps_ensure(KVDtype.f32)) {
                t |> skip("no Vulkan device")
                return
            }
            //! the two-head slab on the groups it serves - two heads a group (gemma-4's shape at 256 and 512), one head a group
            //! (the dead lane through the pass's store and through the combine) - each held bit for bit to the four-head stamp
            codec_attn_arm(t, CodecAttn(kv = KVDtype.f32, hs = 64, nheads = 4, kv_mul = 2, fold = Fold.q8_0, g4_twin = true))   //! the pass's own Q8_0 store
            codec_attn_arm(t, CodecAttn(kv = KVDtype.f32, hs = 64, nheads = 4, kv_mul = 2, fold = Fold.q8_0, nsplit = 8, g4_twin = true))   //! the last piece's combine (three empty pieces)
            codec_attn_arm(t, CodecAttn(kv = KVDtype.f32, hs = 256, nheads = 2, kv_mul = 2, keys = 70, fold = Fold.q8_k, g4_twin = true))   //! one superblock a head, the pass
            codec_attn_arm(t, CodecAttn(kv = KVDtype.f32, hs = 256, nheads = 2, kv_mul = 1, keys = 70, fold = Fold.q8_k, g4_twin = true))
            codec_attn_arm(t, CodecAttn(kv = KVDtype.f32, hs = 256, nheads = 2, kv_mul = 1, keys = 70, fold = Fold.q8_k, nsplit = 4, g4_twin = true))   //! ... the last piece's combine
            codec_attn_arm(t, CodecAttn(kv = KVDtype.f32, hs = 512, nheads = 2, kv_mul = 2, keys = 70, fold = Fold.q8_k, nsplit = 4, g4_twin = true))   //! the wide head: two superblocks
            codec_attn_arm(t, CodecAttn(kv = KVDtype.f32, hs = 512, nheads = 2, kv_mul = 2, keys = 70, fold = Fold.q8_0))   //! the wide head's Q8_0 blocks, the pass
            codec_attn_arm(t, CodecAttn(kv = KVDtype.f32, hs = 64, nheads = 4, kv_mul = 4, fold = Fold.q8_0))   //! the four-head slab's own store
            codec_attn_arm(t, CodecAttn(kv = KVDtype.f32, hs = 64, nheads = 4, kv_mul = 4, fold = Fold.q8_0, nsplit = 8))   //! ... and its combine's
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

[test(description = "Decode attention over the f32 mirror matches the CPU oracle at 1e-4 relative on every codec arm path, plus a 4.0 softcap and the gpt-oss sink stamps over f32 and f16 mirrors, whole and split")]
def test_vkd_da_attn(t0 : T?) {
    t0 |> run("decode attention class kernel == the CPU oracle (chunk boundary)") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!(da_attn_stamps_ensure(KVDtype.f32) && da_attn_stamps_ensure(KVDtype.f16))) {
                t |> skip("no Vulkan device")
                return
            }
            codec_attn_arms(t, KVDtype.f32)
            //! a two-head group's softcap (gemma-2's 4.0 cap the scaled scores overrun), whole and in eight key pieces
            codec_attn_arm(t, CodecAttn(kv = KVDtype.f32, hs = 64, nheads = 4, kv_mul = 2, cap = 4.0))
            codec_attn_arm(t, CodecAttn(kv = KVDtype.f32, hs = 64, nheads = 4, kv_mul = 2, cap = 4.0, nsplit = 8))
            //! the sink stamp over a two-head group (gpt-oss): the pass folding the sink at its store, and the last piece's
            //! combine seeding it - over the f32 mirror and over the f16 one (the resident driver's kv16 form)
            for (kv in fixed_array(KVDtype.f32, KVDtype.f16)) {
                codec_attn_arm(t, CodecAttn(kv = kv, hs = 64, nheads = 4, kv_mul = 2, sink = true))
                codec_attn_arm(t, CodecAttn(kv = kv, hs = 64, nheads = 4, kv_mul = 2, sink = true, nsplit = 8))
            }
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

[test(description = "Decode attention matches the CPU oracle at 1e-4 relative on ragged key groups, a full four-head slab, a short last slab, a 24-word head and two or four rows a dispatch, split arms resubmitted on reset counters")]
def test_vkd_da_attn_shapes(t0 : T?) {
    t0 |> run("decode attention at the shapes the fixed cells leave out == the CPU oracle: ragged key groups, a full four-head slab, a short last slab, a 24-word head, two rows a dispatch") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!da_attn_stamps_ensure(KVDtype.f32)) {
                t |> skip("no Vulkan device")
                return
            }
            //! every split arm dispatches twice over the counters the first left: the arrival counters the pass resets rearm
            codec_attn_arm(t, CodecAttn(kv = KVDtype.f32, hs = 96, nheads = 4, kv_mul = 1))   //! 21 key groups of 12 words: the ragged group count, no fold
            codec_attn_arm(t, CodecAttn(kv = KVDtype.f32, hs = 96, nheads = 4, kv_mul = 1, nsplit = 4, resubmit = true))
            codec_attn_arm(t, CodecAttn(kv = KVDtype.f32, hs = 128, nheads = 8, kv_mul = 4))   //! a full four-head slab (`live` 4): two fold rounds at subgroup 32
            codec_attn_arm(t, CodecAttn(kv = KVDtype.f32, hs = 128, nheads = 8, kv_mul = 4, nsplit = 8, resubmit = true))
            codec_attn_arm(t, CodecAttn(kv = KVDtype.f32, hs = 64, nheads = 10, kv_mul = 5))   //! a short last slab: `live` 1 on each kv head's second slab
            codec_attn_arm(t, CodecAttn(kv = KVDtype.f32, hs = 192, nheads = 2, kv_mul = 2, keys = 70))   //! 24 words a V row, not a power of two: no fold
            //! two rows a dispatch, each over its own mirror region: the row index and the slab arithmetic together, the
            //! four-head stamp over the same rows held to the two-head slab bit for bit
            codec_attn_arm(t, CodecAttn(kv = KVDtype.f32, hs = 64, nheads = 4, kv_mul = 2, nrows = 2, g4_twin = true))
            codec_attn_arm(t, CodecAttn(kv = KVDtype.f32, hs = 64, nheads = 4, kv_mul = 2, nrows = 2, nsplit = 4, resubmit = true, g4_twin = true))
            codec_attn_arm(t, CodecAttn(kv = KVDtype.f32, hs = 64, nheads = 4, kv_mul = 2, nrows = 4, nsplit = 4, resubmit = true))
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

[test(description = "The prefill attention class matches the causal CPU oracle within 2e-3 on a partial 8-row tile past a nonzero window base, at head 64 plain and head 256 under the sigmoid gate")]
def test_vkd_at_attn(t0 : T?) {
    t0 |> run("prefill attention class kernel == the CPU oracle (partial tiles, gated)") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!vk_moe_init()) {
                t |> skip("no Vulkan device")
                return
            }
            if (!ensure_at_attn_cls()) {
                t |> skip("at_attn class declined on this device (workgroup cap)")
                return
            }
            let nheads = 4
            let kv_mul = 2
            let rows = 13   // partial 8-tile: q0=8 has 5 valid rows — the row masks run
            let w0 = 32     // nonzero window base: kend crosses the 32-position K tile → alpha rescale runs
            let scale = 0.125
            for (hs in [64, 256]) {   // dpl 2 (o0/o1 only) and dpl 8 (the o4..o7 arm); gate the wide one
                let gated = hs == 256
                let qd = nheads * hs
                let kvd = (nheads / kv_mul) * hs
                var f <- da_attn_b_fix(rows, nheads, kv_mul, hs, w0, 0, 0, 0.05, 1.8, 0.03, 1.4, false)   // K/V panels hold absolute positions [0, w0+rows)
                var inscope gh <- row_fill(rows * qd, 29, 0, 0.06, 1.9, 61)
                // oracle: causal rows (row r attends [0, w0 + r]); gated = out * sigmoid(gate)
                var inscope y_ref <- da_attn_b_ref(f, scale, 0, 0.0)
                if (gated) {
                    for (i in range(rows * qd)) {
                        y_ref[i] = sig_gate(y_ref[i], gh[i])
                    }
                }
                with_cell_planes() $(var cp : CellPlanes) {
                    // the class kernel: q@0, K@1, V@2, out@3 (pre-loaded with the gates)
                    let pq = cp |> in_(f.qh)
                    let pk = cp |> in_(f.kh)
                    let pv = cp |> in_(f.vh)
                    let py = cp |> io_(gh)
                    var sc = cp |> cls_set(fixed_array(pq, pk, pv, py), @@set_at_attn_cls)
                    var pc = AtAttnArgs(rows = uint(rows), w0 = uint(w0), qd = uint(qd),
                        kvd = uint(kvd), hs = uint(hs), kv_mul = uint(kv_mul),
                        flags = gated ? AT_ATTN_FLAG_GATED : 0u, dpl = uint(hs / 32), scale = scale)
                    cp |> cell_record() $(raw2 : VkCommandBuffer; var h2 : VkHaz) {
                        enc_at_attn_cls(raw2, h2, sc, pc, int64(nheads * ((rows + 7) / 8)))
                    }
                    check_rows(t, "at_attn hs {hs}{gated ? " gated" : ""} against the CPU oracle", cp |> plane_run(py), y_ref, BAR_APPROX_CTRL, qd, hs)
                }
                delete f
                delete gh
                delete y_ref
            }
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

//! the cells' dispatch of one flash stamp through the prefill's ladders, the grid from the arm's rows a workgroup; generic so a build without the vulkan module never infers it
def private enc_fa_tile(khr : bool; hs : int; gated, cap, sink, f16 : bool; raw; var h; var s; var pc; nheads, rows : int) {
    fa_stamp_enc(khr, int64(hs), gated, cap, sink, f16, raw, h, s, pc, fa_stamp_wgs(khr, int64(nheads), int64(rows)))
}

def private fa_arm_ensure(khr : bool) : bool {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        for (hs in fixed_array(64l, 128l, 256l, 512l)) {
            if (!fa_stamp_pair_ensure(khr, hs, false, false, false, true)) return false
        }
        let h96_ok = khr || fa_stamp_pair_ensure(false, 96l, false, false, false, true)
        if (!h96_ok || !fa_stamp_pair_ensure(khr, 64l, false, false, true, true)) return false
    }
    return true
}

//! a flash stamp's set over the cell planes `p` (q, the K and V shadows, the out plane), and the sink plane `psink` past them on a
//! sink stamp; generic so a build without the vulkan module never infers it
def private fa_cell_set(cp; khr, sink : bool; p; psink : int) {
    if (sink) {
        let p5 = fixed_array(p[0], p[1], p[2], p[3], psink)
        return fa_stamp_sink_set(khr, gather_planes(cp.dev, p5), gather_planes(cp.bytes, p5), gather_planes(cp.hazard_bit, p5))
    }
    return fa_stamp_set(khr, gather_planes(cp.dev, p), gather_planes(cp.bytes, p), gather_planes(cp.hazard_bit, p))
}

//! the f16-out twin is held to the f32 stamp's output through the clamp-convert, not to the oracle: the two stamps agree bit for bit
//! on a device that rounds the product to f32 before the f16 store, and within one f16 ulp on one that rounds the fused
//! multiply-convert once (RDNA3's compiler), where an f32 product on an f16 midpoint lands one step apart - `f16_store_rounds_once`
//! reads which off the device
def private f16_store_rounds_once : bool {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        if (!ensure_vkd_mulcvt()) {
            return false
        }
        let n = 64
        //! a times b is 1 + 2^-11 + 2^-34 - 2^-46: past the f16 midpoint 1 + 2^-11 by less than half an f32 ulp, so an f32 product sits on
        //! the midpoint and ties to even (1), while the exact product rounds up (1 + 2^-10)
        var xs, ys : array<float>
        xs |> resize(n)
        ys |> resize(n)
        for (i in range(n)) {
            xs[i] = 1.0 + 0.00048828125 - 0.00000011920928955078125
            ys[i] = 1.0 + 0.00000011920928955078125
        }
        var hs : array<float16>
        with_cell_planes() $(var cp : CellPlanes) {
            let px = cp |> in_(xs)
            let py = cp |> in_(ys)
            let ph = cp |> out_words(int64(n) * 2l, NAN_SENTINEL_BITS)
            var s = cp |> cls_set(fixed_array(px, py, ph), @@set_vkd_mulcvt)
            var pc = VdArgs(n = uint(n))
            cp |> cell_record() $(raw : VkCommandBuffer; var h : VkHaz) {
                enc_vkd_mulcvt(raw, h, s, pc, int64(n))
            }
            delete hs
            hs <- cp |> plane_as(ph, type<float16>, n)
        }
        let once = float(hs[0]) > 1.0
        to_log(LOG_INFO, "the f16 store probe: (1 + 2^-11 - 2^-23) x (1 + 2^-23) stores as {float(hs[0])} - the device rounds the product {once ? "once" : "to f32 first"}\n")
        delete xs
        delete ys
        delete hs
        return once
    } else {
        return false
    }
}

//! one f16 ulp at `v`
def private f16_ulp(v : float) : float => v == 0.0 ? 0.0 : exp2(floor(log2(abs(v))) - 10.0)

//! a ring arm's control holds: the ring's rows read a position a row (`flat`, empty on an arm with no ring) miss the oracle
def private fa_ring_control(t : T?; tag : string; flat, want : array<float>) {
    return if (empty(flat))
    t |> success(mismatch_approx(flat, want) > 0, "{tag} control: the ring's rows read a position a row miss the oracle")
}

//! a ring arm's control: the flash tile's dispatch again with `ring` 0, its out plane read back; nothing off an arm with no ring
def private fa_flat_read(var cp; khr : bool; hs : int; sink : bool; var sfa; pcf; nheads, rows, py : int) : array<float> {
    return <- array<float>() if (pcf.ring == 0u)
    var pflat = pcf
    pflat.ring = 0u
    cp |> plane_refill(py)
    cp |> cell_record() $(raw2 : VkCommandBuffer; var h2 : VkHaz) {
        enc_fa_tile(khr, hs, false, false, sink, false, raw2, h2, sfa, pflat, nheads, rows)
    }
    return <- cp |> plane_run(py)
}

def private fa_tile_run(t : T?; khr : bool) {   // nolint:STYLE037,LINT019 — one arm table over both flash arms; the head-96 skip is its one branch past the limit
    static_if (typeinfo builtin_module_exists(vulkan)) {
        let tag = khr ? "fa_khr" : "fa_cm2"
        let once = f16_store_rounds_once()
        let nheads = 4
        let kv_mul = 2
        let rows = 77    // partial 64-tile: q0=64 has 13 valid rows — clamp loads + store discard run (the KHR tile: five 16-row tiles, the last 13 deep)
        let w0 = 64      // nonzero window base: the causal bound crosses K tiles mid-window
        let scale = 0.125
        //! the sink arms (gpt-oss): the h64 stamps seeded with a per-head sink logit, whole and under a sliding window
        //! the lead arm: the rows sit `lead` rows into the q and out planes (`FaArgs.row0`, a segment's window row base), the lead rows poison the tile must leave
        //! the ring arms: the K/V planes hold the last `ring` positions at their `kv_ring_row` slots, the rows past the ring the slack's value
        for (arm in fixed_array((hs = 64, window = 0, sink = false, lead = 0, ring = 0), (hs = 128, window = 0, sink = false, lead = 0, ring = 0),
                (hs = 256, window = 0, sink = false, lead = 0, ring = 0), (hs = 512, window = 0, sink = false, lead = 0, ring = 0),
                (hs = 128, window = 40, sink = false, lead = 0, ring = 0), (hs = 64, window = 0, sink = true, lead = 0, ring = 0),
                (hs = 64, window = 40, sink = true, lead = 0, ring = 0), (hs = 64, window = 0, sink = false, lead = 8, ring = 0),
                (hs = 128, window = 40, sink = false, lead = 0, ring = 128), (hs = 256, window = 40, sink = false, lead = 0, ring = 128),
                (hs = 96, window = 0, sink = false, lead = 0, ring = 0), (hs = 96, window = 40, sink = false, lead = 0, ring = 0))) {
            let hs = arm.hs
            continue if (khr && hs == 96)   // the KHR flash tile has no head-96 stamp (../ARCHITECTURE_GPU_SHARED_ATTN.md#flash-attn-tile)
            let window = arm.window   //! a sliding layer's arm: the queries attend the `window` positions up to their own
            let sink = arm.sink
            let lead = arm.lead
            let ring = arm.ring
            let qrows = lead + rows
            let stag = sink ? " sink" : (lead > 0 ? " row0 {lead}" : (ring > 0 ? " ring {ring}" : ""))
            var inscope sinkh : array<float>
            sinkh |> resize(nheads)
            for (hh in range(nheads)) {
                sinkh[hh] = 0.45 * float(hh) - 0.3   //! sink logits inside the scaled scores' range, one under and the rest over the row's typical max
            }
            let qd = nheads * hs
            let kvd = (nheads / kv_mul) * hs
            let kpos = w0 + rows
            let kpad = kpos + FA_SHADOW_SLACK_ROWS
            var inscope qh <- row_fill(rows * qd, 41, 0, 0.05, 1.8, 73)
            var inscope qfull : array<float>   //! the q plane the tile reads: `lead` rows of poison, then the rows
            qfull |> resize(qrows * qd)
            for (i in range(qrows * qd)) {
                qfull[i] = i < lead * qd ? 10000.0 : qh[i - lead * qd]
            }
            var inscope kh <- row_fill(kpos * kvd, 17, 0, 0.03, 1.4, 97)
            var inscope vh <- row_fill(kpos * kvd, 53, 0, 0.04, 1.7, 89)
            kh |> resize(kpad * kvd)
            vh |> resize(kpad * kvd)
            for (i in range(kpos * kvd, kpad * kvd)) {   //! the slack rows carry a large finite value, not zero: a masked probability that is not exactly zero would drag it into the output
                kh[i] = 10000.0
                vh[i] = 10000.0
            }
            var inscope kplane <- ring_laid_rows(kh, 1, kpad, kpos, kvd, ring, 0, 10000.0)
            var inscope vplane <- ring_laid_rows(vh, 1, kpad, kpos, kvd, ring, 0, 10000.0)
            var inscope y_cls : array<float>
            var inscope y_flat_read : array<float>   //! a ring arm's control: the ring's rows read a position a row
            var inscope y16_fused, y16_split : array<float16>
            with_cell_planes() $(var cp : CellPlanes) {
                let pq = cp |> in_(qfull)
                let pk = cp |> in_(kplane)
                let pv = cp |> in_(vplane)
                let pk_shadow16 = cp |> out_words(int64(kpad * kvd) * 2l, NAN_SENTINEL_BITS)
                let pv_shadow16 = cp |> out_words(int64(kpad * kvd) * 2l, NAN_SENTINEL_BITS)
                let py = cp |> out_(qrows * qd)
                //! the compared f16 planes under a NaN in both halves: a dead leg on either side reads as a mismatch
                let py16f = cp |> out_words(int64(qrows * qd) * 2l, NAN_SENTINEL_BITS)   // the f16-out twin's plane
                let py16s = cp |> out_words(int64(qrows * qd) * 2l, NAN_SENTINEL_BITS)   // the split pair's: f32 out -> the clamp-convert
                let psink = cp |> in_(sinkh)
                var sck = cp |> cls_set(fixed_array(pk, pk_shadow16), @@set_tower_clamp_cvt_cls)
                var scv = cp |> cls_set(fixed_array(pv, pv_shadow16), @@set_tower_clamp_cvt_cls)
                var sfa = fa_cell_set(cp, khr, sink, fixed_array(pq, pk_shadow16, pv_shadow16, py), psink)
                var sfa16 = fa_cell_set(cp, khr, sink, fixed_array(pq, pk_shadow16, pv_shadow16, py16f), psink)
                var scy = cp |> cls_set(fixed_array(py, py16s), @@set_tower_clamp_cvt_cls)
                var pcv = gk_f16_rows_args(uint(kpad * kvd))
                let cvtwgs = int64((kpad * kvd / 4 + 255) / 256)
                var pcf = FaArgs(rows = uint(rows), w0 = uint(w0), qd = uint(qd),
                    kvd = uint(kvd), kv_mul = uint(kv_mul), kvbase = 0u, scale = scale, window = uint(window), sinkoff = 0u, row0 = uint(lead),
                    ring = uint(ring))
                var pcy = gk_f16_rows_args(uint(qrows * qd))
                cp |> cell_record() $(raw2 : VkCommandBuffer; var h2 : VkHaz) {
                    enc_tower_clamp_cvt_cls(raw2, h2, sck, pcv, cvtwgs)
                    enc_tower_clamp_cvt_cls(raw2, h2, scv, pcv, cvtwgs)
                    enc_fa_tile(khr, hs, false, false, sink, false, raw2, h2, sfa, pcf, nheads, rows)
                    enc_fa_tile(khr, hs, false, false, sink, true, raw2, h2, sfa16, pcf, nheads, rows)
                    enc_tower_clamp_cvt_cls(raw2, h2, scy, pcy, int64((qrows * qd / 4 + 255) / 256))   // the split pair's own convert over the f32 out: the fused twin's true peer
                }
                delete y_cls
                delete y16_fused
                delete y16_split
                var inscope y_full <- cp |> plane_run(py)
                var inscope y16f_full <- cp |> plane_as(py16f, type<float16>, qrows * qd)
                var inscope y16s_full <- cp |> plane_as(py16s, type<float16>, qrows * qd)
                var lead_moved = 0
                var lead_first = -1
                for (i in range(lead * qd)) {   //! the lead rows stay the out plane's NaN sentinel: the tile wrote nothing under its row base
                    if (unsafe(reinterpret<uint>(y_full[i])) != NAN_SENTINEL_BITS) {
                        lead_first = lead_first < 0 ? i : lead_first
                        lead_moved++
                    }
                }
                t |> equal(lead_moved, 0, "{tag} hs {hs}{stag}: the {lead} lead rows under the row base stay untouched ({lead_moved} elements written, the first at {lead_first})")
                y_cls <- [for (i in range(rows * qd)); y_full[lead * qd + i]]
                y16_fused <- [for (i in range(rows * qd)); y16f_full[lead * qd + i]]
                y16_split <- [for (i in range(rows * qd)); y16s_full[lead * qd + i]]
                delete y_flat_read
                y_flat_read <- fa_flat_read(cp, khr, hs, sink, sfa, pcf, nheads, rows, py)
            }
            // oracle mirrors the kernel's f16 grid: q pre-scales then rounds (the kernel's
            // MatrixTimesScalar-then-convert), K/V round through the shadow; softmax stays f32. The cm2 kernel's
            // O accumulates in f16 under the row-max bias (every P at an eighth or under, the normalized row a
            // convex combination of V rows with |v| <= 1 on this fixture), so the f16 running sum's rounding
            // stays under the 2e-3 relative bar mismatch_approx holds against the f32 oracle, and the f16 O
            // twin against the f32 stamp pins the same bar - the bound is this fixture's magnitudes, not a proof.
            // The KHR kernel rounds P to f16, sums each 64-key step in f32 and rounds the step once to f16 before its f32 o row: the same bar.
            var inscope nosink : array<float>
            var inscope y_ref <- sink ? fa_oracle(qh, kh, vh, rows, w0, nheads, hs, kv_mul, kvd, scale, window, false, sinkh) : fa_oracle(qh, kh, vh, rows, w0, nheads, hs, kv_mul, kvd, scale, window, false, nosink)
            check_rows(t, "{tag} hs {hs} window {window}{stag} against the CPU oracle", y_cls, y_ref, BAR_APPROX_CTRL, qd, hs)
            fa_ring_control(t, "{tag} hs {hs} window {window}{stag}", y_flat_read, y_ref)
            if (sink) {   //! the sink control: the sink-free oracle over the same rows must miss - the phantom key carries mass
                var inscope y_nosink <- fa_oracle(qh, kh, vh, rows, w0, nheads, hs, kv_mul, kvd, scale, window, false, nosink)
                check_exceeds(t, "{tag} hs {hs} window {window} sink control: the sink moves the output off the sink-free oracle", y_cls, y_nosink, BAR_APPROX)
            }
            var bad16 = 0
            var one_off = 0
            for (i in range(rows * qd)) {
                let fused = float(y16_fused[i])
                let split = float(y16_split[i])
                if (fused != split) {
                    if (once && abs(fused - split) <= f16_ulp(split)) {
                        one_off++
                        continue
                    }
                    if (bad16 < 6) {
                        to_log(LOG_INFO, "  {tag} hs {hs}{stag} f16-out twin differs at [{i}]: fused {fused} split {split}, the f32 stamp's {y_cls[i]} (bits {unsafe(reinterpret<uint>(y_cls[i]))})\n")
                    }
                    bad16++
                }
            }
            let how = once ? " within one f16 ulp (the device rounds the store once; {one_off} one step apart)" : ""
            t |> success(bad16 == 0, "{tag} hs {hs}{stag} f16-out twin == the f32 stamp's clamp-convert{how} ({bad16} of {rows * qd} off)")
            delete sinkh
            delete qh
            delete kh
            delete vh
            delete y_ref
            delete y_cls
            delete y16_fused
            delete y16_split
        }
    }
}

[test(description = "The cm2 flash tile at heads 64 to 512 matches the f16-grid oracle within 2e-3 plain, windowed, with sinks, at a row base past poison lead rows and over a ring, its f16-out twin equal to the f32 stamp converted")]
def test_vkd_fa_cm2(t0 : T?) {
    t0 |> run("cm2 flash-attention family == the CPU oracle (h64 + h128 + h256, clamp-convert-staged planes)") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!ensure_tower_clamp_cvt_cls()) {   // engage the device without touching coopmat rails
                t |> skip("no Vulkan device")
                return
            }
            if (!g_gpu.has_coopmat2_fa) {
                t |> skip("device lacks the cm2 flash-attention trio")
                return
            }
            verify(fa_arm_ensure(false), "the fa class rails and their f16-out twins must engage on an fa-capable device")
            fa_tile_run(t, false)
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

[test(description = "The KHR flash tile at heads 64 to 512 matches the f16-grid oracle within 2e-3 plain, windowed, with sinks, at a row base past poison lead rows and over a ring, its f16-out twin equal to the f32 stamp converted")]
def test_vkd_fa_khr(t0 : T?) {
    t0 |> run("KHR flash-attention family == the CPU oracle (h64 + h128 + h256 + h512, the window arm, clamp-convert-staged planes)") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!ensure_tower_clamp_cvt_cls()) {
                t |> skip("no Vulkan device")
                return
            }
            if (!khr_kq_tile_on()) {
                t |> skip("device lacks KHR cooperative matrices at subgroup 32")
                return
            }
            if (!fa_arm_ensure(true)) {
                t |> skip("the KHR fa stamps decline on this device: their 38912 B of workgroup memory is past its floor")
                return
            }
            fa_tile_run(t, true)
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

//! the h256 flash stamps at one geometry, the f32 and f16-out twins against the CPU oracle over the kernel's f16 grid. `gated`:
//! a [q | gate] q panel (per-head stride 2 x hs), the oracle scaled by the sigmoid of the gate half - the Qwen3.5 attention shape;
//! else the softcap stamps at gemma-2's shape, wide q and k values whose scaled scores run past `cap`, the oracle under the same
//! cap. The control is the plain oracle - ungated, or uncapped - which must miss the kernel. Returns (f32 mismatches, f16-vs-f32
//! mismatches, outputs compared - 0 when the control does not miss, f32 mismatches with one oracle element poisoned, f16-vs-f32
//! mismatches with one f32 element poisoned, and the out planes' elements past the rows the last tile covers that moved).
def private fa_h256_run(nheads, kv_mul, rows, w0 : int; gated : bool; cap : float; khr : bool) : tuple<bad : int; bad16 : int; n : int; poisoned : int; poisoned16 : int; slack_moved : int> {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        let rows_pad = ((rows + 63) / 64) * 64   //! the cm2 tile covers 64 rows, the KHR tile 16: the out planes carry the larger tile's rows past `rows`
        var slack_moved = 0
        let hs = 256
        let qhs = gated ? 2 * hs : hs
        let scale = 0.0625
        let qd = nheads * hs
        let qrow = nheads * qhs
        let kvd = (nheads / kv_mul) * hs
        let kpos = w0 + rows
        let kpad = kpos + FA_SHADOW_SLACK_ROWS
        //! the softcap fixture's wide q and k values: the scaled scores run past the cap, so the cap is load-bearing
        var qp <- row_fill(rows * qrow, 41, 0, gated ? 0.05 : 0.25, gated ? 1.8 : 9.0, 73)
        var kh <- row_fill(kpos * kvd, 17, 0, gated ? 0.03 : 0.15, gated ? 1.4 : 7.0, 97)
        var vh <- row_fill(kpos * kvd, 53, 0, 0.04, 1.7, 89)
        kh |> resize(kpad * kvd)
        vh |> resize(kpad * kvd)
        var y_cls : array<float>
        var y16 : array<float16>
        with_cell_planes() $(var cp : CellPlanes) {
            let pq = cp |> in_(qp)
            let pk = cp |> in_(kh)
            let pv = cp |> in_(vh)
            let pk_shadow16 = cp |> out_words(int64(kpad * kvd) * 2l, NAN_SENTINEL_BITS)
            let pv_shadow16 = cp |> out_words(int64(kpad * kvd) * 2l, NAN_SENTINEL_BITS)
            let py = cp |> out_(rows * qd, 0, (rows_pad - rows) * qd)   //! the last tile's rows past `rows` stay under the sentinel
            let py16 = cp |> out_words(int64(rows_pad * qd) * 2l, NAN_SENTINEL_BITS)
            var sck = cp |> cls_set(fixed_array(pk, pk_shadow16), @@set_tower_clamp_cvt_cls)
            var scv = cp |> cls_set(fixed_array(pv, pv_shadow16), @@set_tower_clamp_cvt_cls)
            var sfa = fa_cell_set(cp, khr, false, fixed_array(pq, pk_shadow16, pv_shadow16, py), -1)
            var sfa16 = fa_cell_set(cp, khr, false, fixed_array(pq, pk_shadow16, pv_shadow16, py16), -1)
            var pcv = gk_f16_rows_args(uint(kpad * kvd))
            let cvtwgs = int64((kpad * kvd / 4 + 255) / 256)
            var pcf = FaArgs(rows = uint(rows), w0 = uint(w0), qd = uint(qd), kvd = uint(kvd), kv_mul = uint(kv_mul), kvbase = 0u,
                scale = scale, cap = cap, qstride = gated ? uint(qrow) : 0u, qhs = gated ? uint(qhs) : 0u)
            let capped = !gated
            cp |> cell_record() $(raw2 : VkCommandBuffer; var h2 : VkHaz) {
                enc_tower_clamp_cvt_cls(raw2, h2, sck, pcv, cvtwgs)
                enc_tower_clamp_cvt_cls(raw2, h2, scv, pcv, cvtwgs)
                enc_fa_tile(khr, hs, gated, capped, false, false, raw2, h2, sfa, pcf, nheads, rows)
                enc_fa_tile(khr, hs, gated, capped, false, true, raw2, h2, sfa16, pcf, nheads, rows)
            }
            delete y_cls
            delete y16
            y_cls <- cp |> plane_run(py)
            y16 <- cp |> plane_as(py16, type<float16>, rows * qd)
            if (rows_pad > rows) {   //! a whole number of tiles leaves no slack to read
                var inscope y_whole <- cp |> plane_whole(py)
                var inscope slack16 <- cp |> plane_as(py16, type<uint>, (rows_pad - rows) * qd / 2, rows * qd / 2)
                for (i in range(rows * qd, rows_pad * qd)) {
                    slack_moved += float_bits_to_uint(y_whole[i]) != NAN_SENTINEL_BITS ? 1 : 0
                }
                for (w in slack16) {
                    slack_moved += w != NAN_SENTINEL_BITS ? 1 : 0
                }
            }
        }
        // oracle: q (the gate halves too) pre-scales and rounds to f16 like the kernel, K / V round through the shadow; the gate reads the f32 panel
        let nosink : array<float>
        var y_ref <- fa_oracle(qp, kh, vh, rows, w0, nheads, hs, kv_mul, kvd, scale, 0, gated, nosink, cap)
        var qplain <- q_halves(qp, rows, nheads, hs, qhs)
        var y_plain <- fa_oracle(qplain, kh, vh, rows, w0, nheads, hs, kv_mul, kvd, scale, 0, false, nosink)   //! the plain rows: ungated, uncapped
        let bad = mismatch_approx(y_cls, y_ref)
        // the largest difference, pass or fail - the worst (row, head) by relative error and how much of it sits off: a whole
        // row-head off is its softmax, one column its store
        var worst = 0.0
        var worst_at = 0
        for (i in range(rows * qd)) {
            let rel = abs(y_cls[i] - y_ref[i]) / max(abs(y_ref[i]), 1e-3)
            if (rel > worst) {
                worst = rel
                worst_at = i
            }
        }
        let wr = worst_at / qd
        let wh = (worst_at % qd) / hs
        var past = 0
        var rel_sum = 0.0
        for (c in range(hs)) {
            let i = wr * qd + wh * hs + c
            let rel = abs(y_cls[i] - y_ref[i]) / max(abs(y_ref[i]), 1e-3)
            rel_sum += rel
            past += rel > 1e-2 ? 1 : 0
        }
        let at = element_at(worst_at, Shape(row_width = qd, head_width = hs))
        to_log(LOG_INFO, "  fa h256 {khr ? "khr" : "cm2"}{gated ? " gated" : ""} cap {cap}: the largest difference {at} kernel {y_cls[worst_at]} oracle {y_ref[worst_at]} ({worst} relative, {bad} of {rows * qd} past the bar); that row-head's mean relative error {rel_sum / float(hs)}, {past} of {hs} columns past 1e-2\n")
        let bad_plain = mismatch_approx(y_cls, y_plain)   //! the control: the same rows without the gate or the cap must miss, or the fixture never reached them
        y_ref[hs + 3] += 0.5
        let poisoned = mismatch_approx(y_cls, y_ref)
        y_ref[hs + 3] -= 0.5
        // the f16 twin's 2e-3 bar: the cm2 arm accumulates O in f16 under the row-max bias; the KHR arm packs P to f16 and rounds
        // each 64-key step's partial sum to f16 once before its f32 row - on this fixture |v| <= 1.7, every row a convex
        // combination of V rows, so one f16 rounding of a partial sum is under 1.7 * 2^-11 = 8.3e-4 and the steps' roundings
        // carry both signs; the bar is this fixture's magnitudes, not a proof (test_vkd_fa_cm2's oracle note says the same)
        let bad16 = f16_twin_off(y16, y_cls)
        y_cls[hs + 5] += 0.5   //! the control: one poisoned f32 element reds the f16 compare by exactly one
        let poisoned16 = f16_twin_off(y16, y_cls)
        y_cls[hs + 5] -= 0.5
        delete qp
        delete kh
        delete vh
        delete qplain
        delete y_ref
        delete y_plain
        delete y_cls
        delete y16
        return (bad = bad, bad16 = bad16, n = bad_plain > 0 ? rows * qd : 0, poisoned = poisoned, poisoned16 = poisoned16, slack_moved = slack_moved)
    } else {
        return (bad = 0, bad16 = 0, n = 0, poisoned = 0, poisoned16 = 0, slack_moved = 0)
    }
}

//! the f16-out twin's elements off the f32 stamp past 2e-3 of the larger magnitude
def private f16_twin_off(y16 : array<float16>; y : array<float>) : int {
    var off = 0
    for (h, v in y16, y) {
        if (!(abs(float(h) - v) <= 2e-3 * max(1.0, abs(v)))) {
            off++
        }
    }
    return off
}

//! a gated h256 cell at one geometry on the cm2 tile or the KHR one (`khr`): the gate moves the output, the kernel matches the
//! gated oracle, the bar reds one poisoned oracle element, the f16-out twin matches the f32 stamp rounded
def private fa_h256_gated_cell(t : T?; tag : string; nheads, kv_mul, rows, w0 : int; khr : bool) {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        if (!ensure_tower_clamp_cvt_cls()) {
            t |> skip("no Vulkan device")
            return
        }
        if (khr ? !(g_gpu.has_coopmat && khr_kq_tile_on()) : !g_gpu.has_coopmat2_fa) {
            t |> skip(khr ? "device lacks KHR cooperative matrices at subgroup 32" : "device lacks the cm2 flash-attention trio")
            return
        }
        verify(fa_stamp_pair_ensure(khr, 256l, true, false, false, true), "the gated h256 fa rails must engage on a device that serves the tile")
        let r = fa_h256_run(nheads, kv_mul, rows, w0, true, 0.0, khr)
        t |> success(r.n > 0, "{tag}: the gate moves the output (the ungated oracle misses the kernel)")
        t |> success(r.bad == 0, "{tag}: matches the CPU oracle ({r.bad} of {r.n} off)")
        t |> equal(r.poisoned, 1, "{tag}: the bar reds one poisoned oracle element")
        t |> success(r.bad16 == 0, "{tag}: f16-out twin == the f32 stamp rounded ({r.bad16} of {r.n} off)")
        t |> equal(r.poisoned16, r.bad16 + 1, "{tag}: the f16 bar reds one poisoned f32 element")
        t |> equal(r.slack_moved, 0, "{tag}: the out planes' rows past the last tile's rows stay under the sentinel ({r.slack_moved} moved)")
    } else {
        t |> skip("dasVulkan not present")
    }
}

[test(description = "The cm2 gated h256 flash stamp, q at the head stride and the output under the sigmoid gate, matches the gated oracle within 2e-3 on a partial tile while the ungated oracle misses, its f16 twin within 2e-3")]
def test_vkd_fa_cm2_h256_gated(t0 : T?) {
    t0 |> run("cm2 flash-attention h256 gated == the CPU oracle (q at the head's stride, sigmoid gate on the output)") <| @(t : T?) {
        fa_h256_gated_cell(t, "fa_cm2 h256 gated", 4, 2, 77, 64, false)   // partial 64-tile, a nonzero window base
    }
}

[test(description = "The KHR gated h256 flash stamp matches the gated oracle within 2e-3 on a partial tile and at the 35B-A3B geometry, the ungated oracle missing, its f16 twin within 2e-3 and the slack rows untouched")]
def test_vkd_fa_khr_h256_gated(t0 : T?) {
    t0 |> run("KHR flash-attention h256 gated == the CPU oracle (q at the head's stride, sigmoid gate on the output)") <| @(t : T?) {
        fa_h256_gated_cell(t, "fa_khr h256 gated", 4, 2, 77, 64, true)   // a partial 16-row tile, a nonzero window base
    }
    t0 |> run("KHR flash-attention h256 gated at the 35B-A3B geometry (16 heads / 2 kv, 512 rows at w0 0) == the CPU oracle") <| @(t : T?) {
        fa_h256_gated_cell(t, "fa_khr h256 gated 35B", 16, 8, 512, 0, true)
    }
}

//! the h256 softcap stamps under cap 50 on the cm2 tile or the KHR one (`khr`), 4 heads over 2 kv heads at 77 rows from
//! position 64 (a partial 64-tile, a nonzero window base): the capped oracle matches, the uncapped one misses, the f16-out twin
//! matches the f32 stamp rounded
def private fa_h256_softcap_cell(t : T?; khr : bool) {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        if (!ensure_tower_clamp_cvt_cls()) {
            t |> skip("no Vulkan device")
            return
        }
        if (khr ? !(g_gpu.has_coopmat && khr_kq_tile_on()) : !g_gpu.has_coopmat2_fa) {
            t |> skip(khr ? "device lacks KHR cooperative matrices at subgroup 32" : "device lacks the cm2 flash-attention trio")
            return
        }
        let tag = khr ? "fa_khr" : "fa_cm2"
        verify(fa_stamp_pair_ensure(khr, 256l, false, true, false, true), "the softcap h256 fa rails must engage on a device that serves the tile")
        let r = fa_h256_run(4, 2, 77, 64, false, 50.0, khr)
        t |> success(r.n > 0, "the softcap fixture's scores reach the cap (the uncapped oracle misses the kernel)")
        t |> success(r.bad == 0, "{tag} h256 softcap matches the capped CPU oracle ({r.bad} of {r.n} off)")
        t |> equal(r.poisoned, 1, "{tag} h256 softcap: the bar reds one poisoned oracle element")
        t |> success(r.bad16 == 0, "{tag} h256 softcap f16-out twin == the f32 stamp rounded ({r.bad16} of {r.n} off)")
        t |> equal(r.poisoned16, r.bad16 + 1, "{tag} h256 softcap: the f16 bar reds one poisoned f32 element")
        t |> equal(r.slack_moved, 0, "{tag} h256 softcap: the out planes' rows past the last tile's rows stay under the sentinel ({r.slack_moved} moved)")
    } else {
        t |> skip("dasVulkan not present")
    }
}

[test(description = "The cm2 h256 softcap flash stamp matches the oracle capped at 50 within 2e-3 on scores that reach the cap, the uncapped oracle missing, its f16-out twin within 2e-3 of the f32 stamp")]
def test_vkd_fa_cm2_h256_softcap(t0 : T?) {
    t0 |> run("cm2 flash-attention h256 softcap stamps == the CPU oracle under cap 50 (gemma-2's shape; the uncapped oracle as the control)") <| @(t : T?) {
        fa_h256_softcap_cell(t, false)
    }
}

[test(description = "The KHR h256 softcap flash stamp matches the oracle capped at 50 within 2e-3 on scores that reach the cap, the uncapped oracle missing, its f16-out twin within 2e-3 of the f32 stamp")]
def test_vkd_fa_khr_h256_softcap(t0 : T?) {
    t0 |> run("KHR flash-attention h256 softcap stamps == the CPU oracle under cap 50 (gemma-2's shape; the uncapped oracle as the control)") <| @(t : T?) {
        fa_h256_softcap_cell(t, true)
    }
}

// the 9B's own shape: 16 heads over 4 kv heads, a full 512-row window from position 0
[test(description = "The cm2 gated h256 flash stamp at the 9B geometry, 16 heads over 4 kv and 512 rows from position 0, matches the gated oracle within 2e-3, the ungated oracle missing")]
def test_vkd_fa_cm2_h256_9b(t0 : T?) {
    t0 |> run("cm2 flash-attention h256 gated at the 9B geometry (16 heads / 4 kv, 512 rows at w0 0) == the CPU oracle") <| @(t : T?) {
        fa_h256_gated_cell(t, "fa_cm2 h256 gated 9B", 16, 4, 512, 0, false)
    }
}

let private DAB_B = 0          //! the batched decode attention stamps: the 8-row tile, the wide-head 4-row tile, the h128 coopmat tile, and the f16-mirror twins of all three
let private DAB_BW = 1
let private DAB_H128 = 2
let private DAB_B_F16 = 3
let private DAB_H128_F16 = 4
let private DAB_BW_F16 = 5

//! whether a DAB_* stamp reads f16 mirrors
def private dab_f16(stamp : int) : bool => stamp == DAB_B_F16 || stamp == DAB_H128_F16 || stamp == DAB_BW_F16

//! a batched decode attention fixture: `npos` query rows over mirrors holding positions [0, w0 + npos) past `kbase` / `vbase` words
struct private DaAttnBFix {
    qh, kh, vh : array<float>
    gate : array<float>   //! a gated model's gate half beside each head's q (`npos` x `nheads` x `hs`); empty = ungated
    npos, nheads, kv_mul, hs, w0, kbase, vbase : int
}

//! q at (i * 41) % 73 x `qm` - `qa`, K at (i * 17) % 97 x `km` - `ka`, V at (i * 53) % 89 x 0.04 - 1.7; `f16` puts K and V on the f16 grid the f16 mirrors hold
def private da_attn_b_fix(npos, nheads, kv_mul, hs, w0, kbase, vbase : int; qm, qa, km, ka : float; f16 : bool) : DaAttnBFix {
    let qd = nheads * hs
    let kvd = (nheads / kv_mul) * hs
    let kpos = w0 + npos
    var f = DaAttnBFix(npos = npos, nheads = nheads, kv_mul = kv_mul, hs = hs, w0 = w0, kbase = kbase, vbase = vbase)
    f.qh <- [for (i in range(npos * qd)); float((i * 41) % 73) * qm - qa]
    f.kh <- [for (i in range(kbase + kpos * kvd)); f16 ? f16r(float((i * 17) % 97) * km - ka) : float((i * 17) % 97) * km - ka]
    f.vh <- [for (i in range(vbase + kpos * kvd)); f16 ? f16r(float((i * 53) % 89) * 0.04 - 1.7) : float((i * 53) % 89) * 0.04 - 1.7]
    return <- f
}

//! the batched cells' oracle over the offset mirrors: row r attends [kstart, w0 + r], kstart the `window`'s start, every score under `cap`
def private da_attn_b_ref(f : DaAttnBFix; scale : float; window : int; cap : float) : array<float> {
    let qd = f.nheads * f.hs
    let kvd = (f.nheads / f.kv_mul) * f.hs
    var y : array<float>
    y |> resize(f.npos * qd)
    for (r in range(f.npos)) {
        let cnt = f.w0 + r + 1
        let kstart = window > 0 && cnt > window ? cnt - window : 0
        attn_heads_oracle(f.qh, r * qd, f.kh, f.kbase, f.vh, f.vbase, kvd, f.nheads, f.hs, f.kv_mul, cnt, scale, y, r * qd, kstart, cap)
    }
    if (!empty(f.gate)) {
        for (i in range(length(y))) {
            y[i] = sig_gate(y[i], f.gate[i])
        }
    }
    return <- y
}

//! the [q | gate] panel a gated model's q rows take: each head's `hs` q values, then its `hs` gate values
def private gated_q_panel(f : DaAttnBFix) : array<float> {
    let hs = f.hs
    return <- [for (i in range(2 * length(f.qh))); (i / hs) % 2 == 0 ? f.qh[(i / (2 * hs)) * hs + i % hs] : f.gate[(i / (2 * hs)) * hs + i % hs]]
}

//! one batched decode attention dispatch through `stamp` (a DAB_*) over the fixture: q, the K and V mirrors (f16 halves on an f16
//! stamp), the out plane under the sentinel, a workgroup a head per stamp tile of rows; the rows come back
//! under `lead` > 0 the rows sit `lead` rows into the q and out planes (`DaAttnBArgs.row0`, a segment's window row base) behind
//! poison rows, and the WHOLE out plane comes back - the lead rows at the NaN sentinel, then the rows
//! `lead` poison rows ahead of the rows and `trail` behind them in the q and out planes (a co-segment's rows, left alone)
def private da_attn_b_run(f : DaAttnBFix; stamp : int; scale, cap : float; window : int; lead : int = 0; trail : int = 0; ring : int = 0) : array<float> {
    var y : array<float>
    static_if (typeinfo builtin_module_exists(vulkan)) {
        let f16 = dab_f16(stamp)
        let tile = stamp == DAB_BW || stamp == DAB_BW_F16 ? 4 : (stamp == DAB_H128 || stamp == DAB_H128_F16 ? 32 : 8)
        let qd = f.nheads * f.hs
        let kvd = (f.nheads / f.kv_mul) * f.hs
        let gated = !empty(f.gate)
        let qrow = gated ? 2 * qd : qd
        var qlead <- [for (_i in range(lead * qrow)); 10000.0]
        with_cell_planes() $(var cp : CellPlanes) {
            var qrows <- gated ? gated_q_panel(f) : clone_to_move(f.qh)
            qlead |> push_from(qrows)
            for (_i in range(trail * qrow)) {
                qlead |> push(10000.0)
            }
            let pq = cp |> in_(qlead)
            let pk = f16 ? in_plain(cp, pack_f16(f.kh)) : in_(cp, f.kh)
            let pv = f16 ? in_plain(cp, pack_f16(f.vh)) : in_(cp, f.vh)
            let py = cp |> out_((lead + f.npos + trail) * qd)
            let planes = fixed_array(pq, pk, pv, py)
            let bufs = gather_planes(cp.dev, planes)
            let sizes = gather_planes(cp.bytes, planes)
            let bits = gather_planes(cp.hazard_bit, planes)
            var pc = DaAttnBArgs(npos = uint(f.npos), nh = uint(f.nheads), kvd = uint(kvd), hs = uint(f.hs), kv_mul = uint(f.kv_mul),
                kbase = uint(f.kbase), vbase = uint(f.vbase), w0 = uint(f.w0), scale = scale, window = uint(window), cap = cap,
                qhs = gated ? uint(2 * f.hs) : 0u, gated = gated ? 1u : 0u, row0 = uint(lead), ring = uint(ring))
            let wgs = int64(f.nheads * ((f.npos + tile - 1) / tile))
            cp |> cell_record() $(raw2 : VkCommandBuffer; var h2 : VkHaz) {
                if (stamp == DAB_B) {
                    var sc = set_da_attn_b_cls(bufs, sizes, bits)
                    enc_da_attn_b_cls(raw2, h2, sc, pc, wgs)
                } elif (stamp == DAB_BW) {
                    var sc = set_da_attn_bw_cls(bufs, sizes, bits)
                    enc_da_attn_bw_cls(raw2, h2, sc, pc, wgs)
                } elif (stamp == DAB_H128) {
                    var sc = set_da_attn_b_h128_cls(bufs, sizes, bits)
                    enc_da_attn_b_h128_cls(raw2, h2, sc, pc, wgs)
                } elif (stamp == DAB_B_F16) {
                    var sc = set_da_attn_b_f16_cls(bufs, sizes, bits)
                    enc_da_attn_b_f16_cls(raw2, h2, sc, pc, wgs)
                } elif (stamp == DAB_BW_F16) {
                    var sc = set_da_attn_bw_f16_cls(bufs, sizes, bits)
                    enc_da_attn_bw_f16_cls(raw2, h2, sc, pc, wgs)
                } else {
                    var sc = set_da_attn_b_h128_f16_cls(bufs, sizes, bits)
                    enc_da_attn_b_h128_f16_cls(raw2, h2, sc, pc, wgs)
                }
            }
            y <- cp |> plane_run(py) // nolint:PERF030,LINT019 - fresh; Vulkan cell runs once
        }
    }
    return <- y
}

//! the batched decode attention at a row base past zero on the three stamps with a row-base expression of their own - the 8-row
//! tile, the wide-head tile and the h128 coopmat tile: the rows sit three rows into the q and out planes behind poison, the lead
//! rows of the out plane stay at the sentinel and the rows read the oracle's
[test(description = "The 8-row, wide and h128 batched decode attention tiles at a row base three rows in match the CPU oracle, the lead rows and the two trailing segment rows left at their NaN sentinel")]
def test_vkd_da_attn_b_row0(t0 : T?) {
    t0 |> run("da_attn_b at a row base: the lead rows untouched, the rows from row0 the CPU oracle's") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!vk_moe_init()) {
                t |> skip("no Vulkan device")
                return
            }
            if (!ensure_da_attn_b_cls()) {
                t |> skip("da_attn_b class declined on this device (workgroup cap)")
                return
            }
            let lead = 3
            let trail = 2   //! the rows of a segment behind this one: the tile's padded store must stop at the segment's last row
            for (stamp in fixed_array(DAB_B, DAB_BW, DAB_H128)) {
                let tag = stamp == DAB_BW ? "da_attn_bw" : (stamp == DAB_H128 ? "da_attn_b_h128" : "da_attn_b")
                if (stamp == DAB_BW && !ensure_da_attn_bw_cls()) {
                    t |> skip("{tag} declined on this device (workgroup cap)")
                    continue
                }
                if (stamp == DAB_H128 && !h128_twin_on()) {
                    t |> skip("{tag}: the h128 twin serves on a coopmat device at subgroup 32 alone")
                    continue
                }
                verify(stamp != DAB_H128 || ensure_da_attn_b_h128_cls(), "the h128 class rail must engage on a coopmat device")
                let wide = stamp == DAB_BW   //! the gemma-4 global heads' shape and score scale on the wide tile, the 128-head shape on the others
                let scale = wide ? 1.0 : 0.125
                var f <- wide ? da_attn_b_fix(13, 4, 4, 512, 32, 64, 160, 0.005, 0.18, 0.003, 0.14, false) : da_attn_b_fix(13, 4, 2, 128, 32, 64, 160, 0.05, 1.8, 0.03, 1.4, false)
                let qd = f.nheads * f.hs
                var inscope y_full <- da_attn_b_run(f, stamp, scale, 0.0, 0, lead, trail)
                var inscope y_ref <- da_attn_b_ref(f, scale, 0, 0.0)
                var lead_moved = 0
                var lead_first = -1
                for (i in range(lead * qd)) {
                    if (unsafe(reinterpret<uint>(y_full[i])) != NAN_SENTINEL_BITS) {
                        lead_first = lead_first < 0 ? i : lead_first
                        lead_moved++
                    }
                }
                t |> equal(lead_moved, 0, "{tag}: the {lead} lead rows under the row base stay at the sentinel ({lead_moved} elements written, the first at {lead_first})")
                var trail_moved = 0
                var trail_first = -1
                for (i in range((lead + f.npos) * qd, (lead + f.npos + trail) * qd)) {
                    if (unsafe(reinterpret<uint>(y_full[i])) != NAN_SENTINEL_BITS) {
                        trail_first = trail_first < 0 ? i : trail_first
                        trail_moved++
                    }
                }
                t |> equal(trail_moved, 0, "{tag}: the {trail} rows past the segment stay at the sentinel ({trail_moved} elements written, the first at {trail_first})")
                var inscope y_cls <- [for (i in range(f.npos * qd)); y_full[lead * qd + i]]
                check_rows(t, "{tag} at row0 {lead} against the CPU oracle", y_cls, y_ref, stamp == DAB_H128 ? cm_bar(y_ref) : BAR_APPROX_CTRL, qd, f.hs)
                delete f
            }
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

[test(description = "The 8-row batched decode attention over offset mirrors matches the CPU oracle within 2e-3 at heads 32, 128 and 256 on a partial tile, and under a 4.0 softcap that moves the uncapped rows")]
def test_vkd_da_attn_b(t0 : T?) {
    t0 |> run("batched decode attention class kernel == the CPU oracle (mirror offsets)") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!vk_moe_init()) {
                t |> skip("no Vulkan device")
                return
            }
            if (!ensure_da_attn_b_cls()) {
                t |> skip("da_attn_b class declined on this device (workgroup cap)")
                return
            }
            let scale = 0.125
            for (hs in [32, 128, 256]) {   // dpl 1 / 4 / 8 — every guarded accumulator arm
                //! 13 rows: a partial 8-tile, q0 = 8 has 5 valid rows - the row masks run; w0 32: kend crosses the 32-position K
                //! tile, the alpha rescale runs; the mirrors' word offsets 64 / 160 - the offset plumbing must survive the port
                var f <- da_attn_b_fix(13, 4, 2, hs, 32, 64, 160, 0.05, 1.8, 0.03, 1.4, false)
                let n = length(f.qh)
                var inscope y_cls <- da_attn_b_run(f, DAB_B, scale, 0.0, 0)
                var inscope y_ref <- da_attn_b_ref(f, scale, 0, 0.0)
                let bad = mismatch_approx(y_cls, y_ref)
                t |> success(bad == 0, "da_attn_b hs {hs} matches the CPU oracle ({bad} of {n} off)")
                // the softcap arm (gemma-2): the same tile under a cap the scaled scores overrun
                let cap = 4.0
                var inscope y_cap <- da_attn_b_run(f, DAB_B, scale, cap, 0)
                let moved = mismatch_approx(y_cap, y_ref)
                t |> success(moved > 0, "control: the {cap} cap moves the uncapped rows ({moved} of {n})")
                var inscope y_ref_cap <- da_attn_b_ref(f, scale, 0, cap)
                let badc = mismatch_approx(y_cap, y_ref_cap)
                t |> success(badc == 0, "da_attn_b hs {hs} under a {cap} softcap matches the CPU oracle ({badc} of {n} off)")
                delete f
                delete y_ref
                delete y_cls
                delete y_cap
                delete y_ref_cap
            }
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

// the wide-head twin (heads past 256): the 4-row q tile over 16-position K/V stages, at 384 (the
// guarded eighth dim lane) and 512, with the window mask on the long arm
[test(description = "The wide-head batched decode attention at heads 384 and 512 over offset mirrors matches the CPU oracle within 2e-3, one arm under a 24-position window, its f16-mirror twin beside every arm")]
def test_vkd_da_attn_bw(t0 : T?) {
    t0 |> run("wide-head batched decode attention class kernel == the CPU oracle (mirror offsets, the window)") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!vk_moe_init()) {
                t |> skip("no Vulkan device")
                return
            }
            if (!ensure_da_attn_bw_cls()) {
                t |> skip("da_attn_bw class declined on this device (workgroup cap)")
                return
            }
            if (!ensure_da_attn_bw_f16_cls()) {
                t |> skip("da_attn_bw_f16 class declined on this device (workgroup cap) - the f16-mirror arms")
            }
            let scale = 1.0     // the gemma-4 score scale
            let f16_on = ensure_da_attn_bw_f16_cls()   //! the f16-mirror twin (the gemma-4 global heads on the f16 mirror) runs beside every arm
            for (arm in range(4)) {
                let hs = arm == 0 ? 384 : 512
                let npos = arm >= 2 ? 120 : 13    //! 13: a partial 4-tile and a stage crossing; 120: thirty 4-tiles over a 620-position mirror (the second prefill window's shape)
                let w0 = arm >= 2 ? 500 : 32
                let window = arm == 3 ? 24 : 0    //! the last arm slides: every row sees the 24 positions up to its own
                //! four heads over one kv head, the gemma-4 global layers' shape; the mirrors' word offsets 64 / 160
                var f <- da_attn_b_fix(npos, 4, 4, hs, w0, 64, 160, 0.005, 0.18, 0.003, 0.14, false)
                var inscope y_cls <- da_attn_b_run(f, DAB_BW, scale, 0.0, window)
                var inscope y_ref <- da_attn_b_ref(f, scale, window, 0.0)
                let bad = mismatch_approx(y_cls, y_ref)
                t |> success(bad == 0, "da_attn_bw hs {hs} npos {npos} window {window} matches the CPU oracle ({bad} of {length(y_ref)} off)")
                y_ref[0] += 0.5
                t |> success(mismatch_approx(y_cls, y_ref) > 0, "da_attn_bw hs {hs} npos {npos} window {window}: the bar reds a poisoned oracle element")
                if (f16_on) {
                    var f16 <- da_attn_b_fix(npos, 4, 4, hs, w0, 64, 160, 0.005, 0.18, 0.003, 0.14, true)
                    var inscope y16 <- da_attn_b_run(f16, DAB_BW_F16, scale, 0.0, window)
                    var inscope y16_ref <- da_attn_b_ref(f16, scale, window, 0.0)
                    check_rows(t, "da_attn_bw_f16 hs {hs} npos {npos} window {window} against the CPU oracle", y16, y16_ref, BAR_APPROX_CTRL, 4 * hs, hs)
                    delete f16
                    delete y16
                    delete y16_ref
                }
                delete f
                delete y_ref
                delete y_cls
            }
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

//! the chunked prefill tiles over a sliding layer's ring: 120 rows at w0 500 under a 40-position window over a 192-row ring (the
//! window's keys across the slots' wrap), each stamp against the CPU oracle over the flat rows, the same rows read a position a row
//! (`ring` 0) missing it as the control
[test(description = "The 8-row, wide and h128 chunked attention tiles reading a 192-row ring under a 40-position window match the oracle over flat positions, and the same rows read a position a row miss it")]
def test_vkd_da_attn_b_ring(t0 : T?) {
    t0 |> run("the chunked attention tiles read a sliding layer's ring == the CPU oracle over the positions") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!vk_moe_init()) {
                t |> skip("no Vulkan device")
                return
            }
            if (!(ensure_da_attn_b_cls() && ensure_da_attn_bw_cls())) {
                t |> skip("da_attn_b classes declined on this device (workgroup cap)")
                return
            }
            for (stamp in fixed_array(DAB_B, DAB_BW, DAB_H128)) {
                if (stamp == DAB_H128 && !h128_twin_on()) {
                    t |> skip("da_attn_b_h128: the h128 twin serves on a coopmat device at subgroup 32 alone")
                    continue
                }
                verify(stamp != DAB_H128 || ensure_da_attn_b_h128_cls(), "the h128 class rail must engage on a coopmat device")
                let wide = stamp == DAB_BW
                let tag = wide ? "da_attn_bw" : (stamp == DAB_H128 ? "da_attn_b_h128" : "da_attn_b")
                let scale = wide ? 1.0 : 0.125
                var f <- wide ? da_attn_b_fix(120, 4, 4, 512, 500, 64, 160, 0.005, 0.18, 0.003, 0.14, false) : da_attn_b_fix(120, 4, 2, 128, 500, 64, 160, 0.05, 1.8, 0.03, 1.4, false)
                var g := f   //! `f`'s K and V mirrors as rings of 192 rows past their bases
                let kvd = (f.nheads / f.kv_mul) * f.hs
                g.kh <- ring_laid_rows(f.kh, 1, f.w0 + f.npos, f.w0 + f.npos, kvd, 192, f.kbase, 0.0)
                g.vh <- ring_laid_rows(f.vh, 1, f.w0 + f.npos, f.w0 + f.npos, kvd, 192, f.vbase, 0.0)
                var inscope y <- da_attn_b_run(g, stamp, scale, 0.0, 40, 0, 0, 192)
                var inscope y_ref <- da_attn_b_ref(f, scale, 40, 0.0)
                let bar = stamp == DAB_H128 ? cm_bar(y_ref) : BAR_APPROX_CTRL
                check_rows(t, "{tag} over a 192-row ring, a 40-position window, against the CPU oracle", y, y_ref, bar, 4 * f.hs, f.hs)
                var inscope y_flat <- da_attn_b_run(g, stamp, scale, 0.0, 40)
                t |> success(check_count(y_flat, y_ref, bar) > 0, "{tag} control: the ring's rows read a position a row miss the oracle")
                delete f
                delete g
            }
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

[test(description = "The h128 coopmat attention tile matches the CPU oracle at the coopmat bar over two q tiles, and under a 40-key window, a 0.5 softcap and both, the unmasked uncapped oracle missing each")]
def test_vkd_da_attn_b_h128(t0 : T?) {
    t0 |> run("h128 coopmat attention class kernel == the CPU oracle") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!vk_moe_init()) {
                t |> skip("no Vulkan device")
                return
            }
            if (!ensure_da_attn_b_cls()) {   // probe via the NON-coopmat twin — a cm pipeline build
                t |> skip("da_attn_b class declined on this device (workgroup cap)")   // on a no-coopmat device panics, not feints
                return
            }
            if (!h128_twin_on()) {
                t |> skip("the h128 twin serves on a coopmat device at subgroup 32 alone")
                return
            }
            verify(ensure_da_attn_b_h128_cls(), "the h128 class rail must engage on a coopmat device")
            let scale = 0.125
            var f <- da_attn_b_fix(45, 4, 2, 128, 32, 64, 160, 0.05, 1.8, 0.03, 1.4, false)   // 45 rows, two q tiles: the second has 13 valid rows - the row masks run
            var y_cls <- da_attn_b_run(f, DAB_H128, scale, 0.0, 0)
            // the h128 twin stages q/K/V through f16 pairs for the MMAs — floor-barred like the
            // cm GEMMs (softmax weights bound the V fold, so the peak floor is generous enough)
            var y_ref <- da_attn_b_ref(f, scale, 0, 0.0)
            check_rows(t, "da_attn_b_h128 against the CPU oracle", y_cls, y_ref, cm_bar(y_ref), 4 * 128, 128)
            delete f
            delete y_ref
            delete y_cls
            //! a sliding window and the logit softcap, alone and together, over 120 rows at w0 500: the plain oracle misses each arm
            for (arm in range(3)) {
                let window = arm == 1 ? 0 : 40
                let cap = arm == 0 ? 0.0 : 0.5
                var fw <- da_attn_b_fix(120, 4, 2, 128, 500, 64, 160, 0.05, 1.8, 0.03, 1.4, false)
                var inscope yw <- da_attn_b_run(fw, DAB_H128, scale, cap, window)
                var inscope yw_ref <- da_attn_b_ref(fw, scale, window, cap)
                let tag = "da_attn_b_h128 window {window} cap {cap}"
                check_rows(t, "{tag} against the CPU oracle", yw, yw_ref, cm_bar(yw_ref), 4 * 128, 128)
                var inscope yw_plain <- da_attn_b_ref(fw, scale, 0, 0.0)
                t |> success(check_count(yw, yw_plain, cm_bar(yw_plain)) > 0, "{tag}: the unmasked, uncapped oracle misses the kernel")
                delete fw
            }
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

//! one gated chunked-attention arm: q read at the head's [q | gate] stride, the output under the sigmoid of the gate half -
//! the gated oracle matches, the ungated one misses, the bar reds a poisoned element
//! `bar_of` reads the bar off the oracle rows: a stamp staging q/K/V through halves takes `cm_bar`, the f32 stamps a fixed bar
def private da_attn_b_gated_arm(t : T?; tag : string; stamp, npos, kv_mul, hs, w0 : int; qm, qa, km, ka : float; bar_of : block<(want : array<float>) : Bar>) {
    var f <- da_attn_b_fix(npos, 4, kv_mul, hs, w0, 64, 160, qm, qa, km, ka, dab_f16(stamp))
    f.gate <- [for (i in range(length(f.qh))); float((i * 29) % 61) * 0.08 - 2.4]
    let scale = hs > 256 ? 1.0 : 0.125
    var y_cls <- da_attn_b_run(f, stamp, scale, 0.0, 0)
    var y_ref <- da_attn_b_ref(f, scale, 0, 0.0)
    let bar = invoke(bar_of, y_ref)
    check_rows(t, tag, y_cls, y_ref, bar, 4 * hs, hs)
    delete f.gate
    var y_plain <- da_attn_b_ref(f, scale, 0, 0.0)
    t |> success(check_count(y_cls, y_plain, bar) > 0, "{tag}: the gate moves the output (the ungated oracle misses the kernel)")
    delete f
    delete y_cls
    delete y_ref
    delete y_plain
}

[test(description = "Gated chunked attention, q at the head [q | gate] stride and the output under the gate sigmoid, matches the gated oracle on the 8-row, wide, wide-f16 and h128 tiles while the ungated oracle misses")]
def test_vkd_da_attn_b_gated(t0 : T?) {
    t0 |> run("gated chunked attention == the gated CPU oracle on the 8-row, wide and h128 stamps") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!vk_moe_init()) {
                t |> skip("no Vulkan device")
                return
            }
            if (!(ensure_da_attn_b_cls() && ensure_da_attn_bw_cls())) {
                t |> skip("da_attn_b classes declined on this device (workgroup cap)")
                return
            }
            da_attn_b_gated_arm(t, "da_attn_b gated hs 128", DAB_B, 13, 2, 128, 32, 0.05, 1.8, 0.03, 1.4) $(want : array<float>) : Bar {
                return BAR_APPROX
            }
            da_attn_b_gated_arm(t, "da_attn_bw gated hs 512", DAB_BW, 13, 4, 512, 32, 0.005, 0.18, 0.003, 0.14) $(want : array<float>) : Bar {
                return BAR_APPROX
            }
            if (ensure_da_attn_bw_f16_cls()) {
                da_attn_b_gated_arm(t, "da_attn_bw_f16 gated hs 512", DAB_BW_F16, 13, 4, 512, 32, 0.005, 0.18, 0.003, 0.14) $(want : array<float>) : Bar {
                    return BAR_APPROX_CTRL
                }
            } else {
                t |> skip("da_attn_bw_f16 class declined on this device (workgroup cap) - the f16 gated arm")
            }
            if (!h128_twin_on()) {
                t |> skip("the h128 twin serves on a coopmat device at subgroup 32 alone - the h128 gated arm")
                return
            }
            if (!ensure_da_attn_b_h128_cls()) {
                t |> failure("the h128 class rail must engage on a coopmat device")
                return
            }
            da_attn_b_gated_arm(t, "da_attn_b_h128 gated", DAB_H128, 45, 2, 128, 32, 0.05, 1.8, 0.03, 1.4) $(want : array<float>) : Bar {
                return cm_bar(want)   //! the h128 stamp stages q/K/V through f16 pairs for its MMAs
            }
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

// f32 -> f16 rows for upload (the session/mirror codec form)
def private pack_f16(src : array<float>) : array<float16> {
    var out : array<float16>
    out |> resize(length(src))
    for (s, o in src, out) {
        o = float16(s)
    }
    return <- out
}

[test(description = "The f16-mirror reader stamps match the CPU oracle: decode attention on every codec arm path and heads 64 to 512 at 1e-4 relative, the batched tile within 2e-3, the h128 coopmat twin at the coopmat bar")]
def test_vkd_kv16_readers(t0 : T?) {
    t0 |> run("f16-mirror reader instances == the CPU oracle (da_attn / da_attn_b / h128)") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!vk_moe_init()) {
                t |> skip("no Vulkan device")
                return
            }
            if (!(da_attn_stamps_ensure(KVDtype.f16) && ensure_da_attn_b_f16_cls())) {
                t |> skip("f16 attention classes declined on this device")
                return
            }
            // --- da_attn_f16: hs 64 the scalar head, 256 the one-word lane, 512 the two-word lane ---
            codec_attn_arms(t, KVDtype.f16)
            for (hs in [64, 256, 512]) {
                codec_attn_arm(t, CodecAttn(kv = KVDtype.f16, hs = hs, nheads = 4, kv_mul = 2, g4_twin = true))
            }
            // --- da_attn_b_f16: batched window over f16 mirrors (dpl 1 and 8 arms) ---
            for (hs in [32, 256]) {
                var f <- da_attn_b_fix(13, 4, 2, hs, 32, 64, 160, 0.05, 1.8, 0.03, 1.4, true)
                var inscope y_cls <- da_attn_b_run(f, DAB_B_F16, 0.125, 0.0, 0)
                var inscope y_ref <- da_attn_b_ref(f, 0.125, 0, 0.0)
                check_rows(t, "da_attn_b_f16 hs {hs} against the CPU oracle", y_cls, y_ref, BAR_APPROX_CTRL, 4 * hs, hs)
                delete f
                delete y_ref
                delete y_cls
            }
            // --- da_attn_b_h128_f16: the coopmat twin over f16 mirrors ---
            if (h128_twin_on()) {
                verify(ensure_da_attn_b_h128_f16_cls(), "the h128 f16 class rail must engage on a coopmat device")
                var f <- da_attn_b_fix(45, 4, 2, 128, 32, 64, 160, 0.05, 1.8, 0.03, 1.4, true)
                var y_cls <- da_attn_b_run(f, DAB_H128_F16, 0.125, 0.0, 0)
                var y_ref <- da_attn_b_ref(f, 0.125, 0, 0.0)
                check_rows(t, "da_attn_b_h128_f16 against the CPU oracle", y_cls, y_ref, cm_bar(y_ref), 4 * 128, 128)
                delete f
                delete y_ref
                delete y_cls
            } else {
                t |> skip("the h128 twin serves on a coopmat device at subgroup 32 alone - the h128 f16 arm")
            }
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}
