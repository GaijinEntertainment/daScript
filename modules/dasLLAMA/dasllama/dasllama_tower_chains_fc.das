options gen2
options indenting = 4
options _dasllama_internal = true

//! The FastConformer trio's chains written once for both discrete homes - the block loop in its rel-position form (canary,
//! parakeet) and its rotary form (GigaAM) over the families' weight lanes, the subsample fronts and GigaAM's device mel, canary's
//! projector - and the decoders the trio hands its rows to, the transducer's rounds and the CTC head; beside them the family
//! descriptor both homes' seats read and the device-block bakes both homes' flavors mint. Every body is generic over the rail
//! (`dasllama_tower_rail`); a seat keeps the gate, the uploads, the scratch, the schedule staging and the copy-out.
module dasllama_tower_chains_fc shared public

require dasllama/dasllama_lint public
require math
require daslib/math_boost   // round_up: the GigaAM front's 64 and 128 lattices
require dasllama/dasllama_kqformat
require dasllama/dasllama_gemm_schema   // dev_rows_bytes, kq_dev_ssb: the kq lane's device plane strides
require dasllama/dasllama_math   // active_q8_repack_layout, active_kq_layout_mr
require dasllama/dasllama_layout   // kq_gather_rows_dev
require dasllama/dasllama_plane
require dasllama/dasllama_convert   // cvt_f32_to_f16_sat_array: the front slabs
require dasllama/dasllama_asr_types   // CanaryLayerOffs
require dasllama/dasllama_tower
require dasllama/dasllama_tower_image   // tower_gather_q8_regions
require dasllama/dasllama_gpu_math
require dasllama/dasllama_gpu_tier   // TILE_READ_SLACK: the f16 feeds' read slack behind their rows
require dasllama/dasllama_gpu_kernels_common
require dasllama/dasllama_gpu_kernels_discrete
require dasllama/dasllama_canary
require dasllama/dasllama_parakeet
require dasllama/dasllama_gigaam
require dasllama/dasllama_rnnt
require dasllama/dasllama_tower_rail
require dasllama/dasllama_tower_chains   // tower_attn_padded: the rotary form's padded route
require ?cuda dasllama/dasllama_cuda_classes_front   // the fronts' stamps on the CUDA rail
require ?cuda dasllama/dasllama_cuda_classes_blocks   // the FastConformer blocks' stamps on the CUDA rail
require ?cuda dasllama/dasllama_cuda_classes_asr   // the transducer's and the CTC head's stamps on the CUDA rail

let FC_REC_FF1A = 0l   //! a FastConformer block's GEMM records, their positions in `fc_gemm_records`
let FC_REC_FF1B = 1l
let FC_REC_Q = 2l
let FC_REC_K = 3l
let FC_REC_V = 4l
let FC_REC_POS = 5l
let FC_REC_O = 6l
let FC_REC_PW1 = 7l
let FC_REC_PW2 = 8l
let FC_REC_FF2A = 9l
let FC_REC_FF2B = 10l

let FC_FRONT_REC_C3 = 0l    //! the subsample front's GEMM records: the two pointwise convs, the input projection
let FC_FRONT_REC_C6 = 1l
let FC_FRONT_REC_PRE = 2l
let FC_FRONT_RECORDS = 3l

let TOWER_FC_MEL_FLOOR = 5.960464477539063e-08   //! 2^-24, canary's log-mel floor (the CPU front's own)

let TOWER_KQ_Q = fixed_array(TowerBuf.kq_q0, TowerBuf.kq_q1, TowerBuf.kq_q2, TowerBuf.kq_q3)   //! the kq lane's member pairs a rail seats, in format-slot order: the quants
let TOWER_KQ_S = fixed_array(TowerBuf.kq_s0, TowerBuf.kq_s1, TowerBuf.kq_s2, TowerBuf.kq_s3)   //! ... and the scales
let TOWER_KQ_MEMBERS = 4   //! the K-quant members a carrier's kq lane seats at most

//! A FastConformer family as the chain reads it - canary (biased GEMMs, channel-major taps, the mel on the device), parakeet (no
//! GEMM biases, tap-major taps, the CPU mel uploaded) and gigaam (the rotary form); the family's CPU loops are the specification,
//! its block offsets the seat's `CanaryLayerOffs` records
[arch(at = "../ARCHITECTURE_GPU_TOWER_RAIL.md#fc-chains")]
struct TowerFc {
    family : string
    front_name : string
    q8 : bool
    qblob : PlaneI8 = PlaneI8()
    qscales : PlaneF = PlaneF()
    blob : PlaneF = PlaneF()
    f16w : bool          //! the exact lane: every GEMM on the f16 GEMM over the halfwords in `wblob`, `w_n` of them at `w_off` - gigaam's carrier's own, canary's and parakeet's staged
    wblob : PlaneU16 = PlaneU16()
    w_off : int64
    w_n : int64
    taps : PlaneU16 = PlaneU16()   //! gigaam's f16 depthwise taps: `taps_n` halfwords at `taps_off`; empty: f32 in `blob`
    taps_off : int64
    taps_n : int64
    kq_lane : bool       //! gigaam's kq lane: the block GEMMs the carrier's own planes, a seat a GEMM record
    kq : PlaneU8 = PlaneU8()
    ks : PlaneU8 = PlaneU8()
    kq_repacked : bool   //! the K-quant planes in the CPU backend's grp interleave, which the gather un-groups; else disk order
    kq_fmts : uint       //! the K-quant members' format slots, a bit each
    dev_backend : TowerDev
    dev : PlaneU8 = PlaneU8()
    devplan : PlaneI64 = PlaneI64()
    n_layer : int64
    d : int64
    ff : int64
    n_head : int64
    conv_kernel : int64
    eps : float
    biased : bool        //! the GEMMs carry bias rows; off, the seams add the branch bare and the activations run alone
    tap_major : bool     //! the depthwise taps are [tap][ch], else [ch][tap]
    device_mel : bool    //! the front starts from the windowed frames (the spectrum and the mel on the device), else from the CPU mel rows
    rotary : bool        //! gigaam: no rel table; rope on the attention's normed input rows, the heads restrided onto the flash tile's width
    conv_ln : bool       //! gigaam: the depthwise conv biased, then a LayerNorm over the channels (the `bn_w` / `bn_b` slots) and SiLU
    n_mel : int64
    n_fft : int64
    n_fb : int64
    nch : int64
    c0_w : int64
    c0_b : int64
    c2_w : int64
    c2_b : int64
    c3_w : int64
    c3_b : int64
    c5_w : int64
    c5_b : int64
    c6_w : int64
    c6_b : int64
    pre_w : int64
    pre_b : int64
    proj_w : int64 = -1l   //! canary's projector behind the blocks, `proj_dim` rows over `d`
    proj_b : int64 = -1l
    proj_dim : int64
    twiddles : PlaneF = PlaneF()
    filters : PlaneF = PlaneF()
}

//! a kq-lane GEMM record's seat: the format its tile decodes, its first block in its planes, and the member pair those planes
//! sit in (-1: the q8 planes)
struct TowerFcSeat {
    fmt : KqFmt
    wblk0 : int64
    member : int = -1
}

//! a device block piece of the FastConformer chain on home `home` (`vk`, `cu`)
def tower_fc_piece(home, name : string) : string => "{home}.fc.{name}"

//! a K-quant member's device planes' piece names
def tower_fc_kq_piece(home : string; fmt : int; scales : bool) : string => scales ? "{home}.fc.kq.{fmt}.s" : "{home}.fc.kq.{fmt}.q"

//! the window's element base in GigaAM's twiddle plane: past the [n_fft][2 n_bins] twiddles
def tower_ga_mel_woff(n_fft : int64) : int64 => n_fft * 2l * (n_fft / 2l + 1l)

//! the member pair format slot `fmt` takes among the carrier's K-quant members `kq_fmts`: its rank among their slots
def tower_fc_member(kq_fmts : uint; fmt : KqFmt) : int {
    var m = 0
    for (f in range(int(fmt))) {
        m += (kq_fmts & (1u << uint(f))) != 0u ? 1 : 0
    }
    return m
}

//! canary's chain descriptor, its block offsets into `offs`
def tower_fc_of_cn(t : CanaryEncoder; @scratch @exact_size var offs : array<CanaryLayerOffs>) : TowerFc {
    offs |> grow_resize(t.n_layer)
    for (l in range64(t.n_layer)) {
        offs[l] = t.layers[l]
    }
    return TowerFc(family = "canary", front_name = "canary-front", q8 = t.q8, qblob = t.qblob, qscales = t.qscales, blob = t.blob,
        f16w = t.gemm_f16, wblob = t.wblob, w_off = 0l, w_n = t.wblob.n, n_layer = t.n_layer, d = t.d_model, ff = t.n_ff, n_head = t.n_head, conv_kernel = t.conv_kernel, eps = t.eps,
        biased = true, tap_major = false, device_mel = true, n_mel = t.n_mel, n_fft = t.n_fft, n_fb = t.n_fb, nch = t.n_sub_ch,
        c0_w = t.c0_w, c0_b = t.c0_b, c2_w = t.c2_w, c2_b = t.c2_b, c3_w = t.c3_w, c3_b = t.c3_b, c5_w = t.c5_w, c5_b = t.c5_b,
        c6_w = t.c6_w, c6_b = t.c6_b, pre_w = t.pre_w, pre_b = t.pre_b, twiddles = t.twiddles, filters = t.filters,
        proj_w = t.proj_w, proj_b = t.proj_b, proj_dim = t.proj_dim, dev_backend = t.dev_backend, dev = t.dev, devplan = t.devplan)
}

//! parakeet's chain descriptor, its block offsets mapped onto canary's record into `offs`
def tower_fc_of_pk(m : ParakeetModel; @scratch @exact_size var offs : array<CanaryLayerOffs>) : TowerFc {
    offs |> grow_resize(m.n_layer)
    for (l in range64(m.n_layer)) {
        offs[l] = pk_fc_layer(pk_layer_offsets(m, l))
    }
    return TowerFc(family = "parakeet", front_name = "parakeet-front", q8 = m.q8, qblob = m.qblob, qscales = m.qscales, blob = m.blob,
        f16w = m.gemm_f16, wblob = m.wblob, w_off = 0l, w_n = m.wblob.n, n_layer = m.n_layer, d = m.n_state, ff = 4l * m.n_state, n_head = m.n_head, conv_kernel = m.n_conv_kernel, eps = m.eps,
        biased = false, tap_major = true, device_mel = false, n_mel = m.n_mel, n_fft = m.n_fft, n_fb = m.n_fft / 2l + 1l, nch = m.n_sub_ch,
        c0_w = m.c0_w, c0_b = m.c0_b, c2_w = m.c2_w, c2_b = m.c2_b, c3_w = m.c3_w, c3_b = m.c3_b, c5_w = m.c5_w, c5_b = m.c5_b,
        c6_w = m.c6_w, c6_b = m.c6_b, pre_w = m.pre_w, pre_b = m.pre_b, twiddles = PlaneF(), filters = m.filters,
        dev_backend = m.dev_backend, dev = m.dev, devplan = m.devplan)
}

//! GigaAM's chain descriptor on home `home`'s image, offsets into `offs`; a CPU image's kq lane fills `gseats`, a GPU image carries its own
def tower_fc_of_ga(m : GigaamModel; home : string; @scratch @exact_size var offs : array<CanaryLayerOffs>; @scratch @exact_size var gseats : array<TowerGemmSeat>) : TowerFc {
    offs |> grow_resize(m.n_layer)
    for (l in range64(m.n_layer)) {
        offs[l] = gigaam_layer_offsets(m, l)
    }
    let nseats = m.kq_lane && m.dev_backend == TowerDev.none ? m.n_layer * FC_GEMM_RECORDS : 0l
    gseats |> grow_resize(nseats)
    let kq_fmts = (nseats > 0l ? gigaam_fc_record_seats(m, gseats)
        : uint(tower_dev_word(m.dev, m.devplan, tower_fc_piece(home, "kq.fmts"), 0l)))
    let f16w = !m.q8 && m.gemm_f16
    return TowerFc(family = "gigaam", front_name = "gigaam-front", q8 = m.q8, qblob = m.qblob, qscales = m.qscales, blob = m.blob,
        f16w = f16w, wblob = f16w ? m.wblob : PlaneU16(), w_off = m.gemm_off, w_n = f16w ? m.n_layer * m.gemm_stride : 0l,
        taps = m.dw_f16 ? m.wblob : PlaneU16(), taps_off = m.dw_off, taps_n = m.dw_f16 ? m.n_layer * m.conv_kernel * m.d_model : 0l,
        kq_lane = m.kq_lane, kq = m.kq_lane ? m.kq : PlaneU8(), ks = m.kq_lane ? m.ks : PlaneU8(), kq_repacked = m.kq_repacked,
        kq_fmts = kq_fmts, dev_backend = m.dev_backend, dev = m.dev, devplan = m.devplan,
        n_layer = m.n_layer, d = m.d_model, ff = m.n_ff, n_head = m.n_head, conv_kernel = m.conv_kernel, eps = m.eps,
        biased = true, tap_major = true, device_mel = false, rotary = true, conv_ln = true, n_mel = m.n_mel, n_fft = m.n_fft,
        n_fb = m.n_fft / 2l + 1l, nch = m.n_sub_ch, c0_w = m.c0_w, c0_b = m.c0_b, c2_w = m.c2_w, c2_b = m.c2_b,
        c3_w = -1l, c3_b = -1l, c5_w = -1l, c5_b = -1l, c6_w = -1l, c6_b = -1l, pre_w = -1l, pre_b = -1l, twiddles = PlaneF(), filters = m.filters)
}

//! The kq lane's device plan: every GEMM record's seat in the device planes, the Q8_0 seats' regions in ``qblob``, and each
//! K-quant member's quant and scale bytes
[arch(at = "../ARCHITECTURE_GPU_TOWER_VULKAN_FC.md#vk-tower-fc-kq")]
struct TowerFcKqPlan {
    @exact_size seats : array<TowerFcSeat>
    regions : array<Q8Region>
    qbytes : table<int; int64>
    sbytes : table<int; int64>
}

[cold_path]
def private tower_fc_kq_plan(fc : TowerFc; offs : array<CanaryLayerOffs>; gseats : array<TowerGemmSeat>) : TowerFcKqPlan {
    let q8e = kq_elems(KqFmt.q8)
    let nrec = fc.n_layer * FC_GEMM_RECORDS
    var p : TowerFcKqPlan
    p.seats |> reserve_resize(nrec)
    for (l in range64(fc.n_layer)) {
        let recs = fc_gemm_records(offs[l], fc.d, fc.ff)
        for (g in range64(FC_GEMM_RECORDS)) {
            let rid = l * FC_GEMM_RECORDS + g
            let seat = gseats[rid]
            p.seats[rid] = TowerFcSeat(fmt = KqFmt.q8, wblk0 = 0l)
            continue if (recs[g].rows == 0l)
            if (seat.fmt == KqFmt.q8) {
                p.regions |> push(Q8Region(woff = seat.qoff, n = recs[g].n, rows = recs[g].rows))
                p.seats[rid] = TowerFcSeat(fmt = KqFmt.q8, wblk0 = seat.qoff / q8e)
            } else {
                let fmt = int(seat.fmt)
                let bytes = dev_rows_bytes(seat.fmt, recs[g].n, recs[g].rows)
                let q0 = p.qbytes?[fmt] ?? 0l
                p.seats[rid] = TowerFcSeat(fmt = seat.fmt, wblk0 = q0 / kq_qsb(seat.fmt))
                p.qbytes[fmt] = q0 + bytes.q
                p.sbytes[fmt] = (p.sbytes?[fmt] ?? 0l) + bytes.s
            }
        }
    }
    return <- p
}

//! one K-quant member's seats gathered in the tier's device layout at their block bases (`seats`) into `wq` / `ws`
[cold_path]
def private tower_fc_gather_kq_member(fc : TowerFc; offs : array<CanaryLayerOffs>; gseats : array<TowerGemmSeat>; fmt : int; seats : array<TowerFcSeat>;
                                      q_bytes, s_bytes : int64; var wq, ws : array<uint8>) {
    var inscope no_site_words : array<uint>
    wq |> reserve_resize(q_bytes)
    ws |> reserve_resize(s_bytes)
    for (l in range64(fc.n_layer)) {
        let recs = fc_gemm_records(offs[l], fc.d, fc.ff)
        for (g in range64(FC_GEMM_RECORDS)) {
            let rid = l * FC_GEMM_RECORDS + g
            let seat = gseats[rid]
            continue if (recs[g].rows == 0l || int(seat.fmt) != fmt)
            let kf = seat.fmt
            let base = seats[rid].wblk0
            kq_gather_rows_dev(kf, plane_at(fc.kq, seat.qoff), plane_at(fc.ks, seat.soff), recs[g].n, recs[g].rows, recs[g].rows,
                fc.kq_repacked, fc.kq_repacked ? active_kq_layout_mr(kq_schema_id(kf)) : 1l,
                wq, ws, base * kq_qsb(kf), base * kq_dev_ssb(kf), no_site_words)
        }
    }
}

//! a FastConformer chain's planes for home `home`'s device block: the q8 planes or the kq lane's member planes and seats, none on the exact lane
[cold_path, arch(at = "../ARCHITECTURE_IMAGE_GPU_PLANES.md#tower-dev-block")]
def tower_fc_bake(fc : TowerFc; offs : array<CanaryLayerOffs>; gseats : array<TowerGemmSeat>; var b : TowerDevBuild; home : string) {
    let lay = active_q8_repack_layout(false)
    var wq, ws : array<uint8>
    if (fc.kq_lane) {
        var inscope p <- tower_fc_kq_plan(fc, offs, gseats)
        if (fc.qblob.n > 0l) {
            wq |> reserve_resize(fc.qblob.n)
            ws |> reserve_resize(fc.qscales.n * 2l)
            tower_gather_q8_regions(fc.qblob, fc.qscales, p.regions, lay, wq, ws)
        }
        var seats : array<int64>
        for (s in p.seats) {
            seats |> push_from([int64(s.fmt), s.wblk0])
        }
        tower_dev_put(b, tower_fc_piece(home, "kq.seats"), seats)
        delete seats
        let fmts = [int64(fc.kq_fmts)]
        tower_dev_put(b, tower_fc_piece(home, "kq.fmts"), fmts)
        for (fmt, qb in keys(p.qbytes), values(p.qbytes)) {
            var kq, ks : array<uint8>
            tower_fc_gather_kq_member(fc, offs, gseats, fmt, p.seats, qb, p.sbytes?[fmt] ?? 0l, kq, ks)
            tower_dev_put(b, tower_fc_kq_piece(home, fmt, false), unsafe(addr<uint8 const?>(kq[0])), long_length(kq))
            tower_dev_put(b, tower_fc_kq_piece(home, fmt, true), unsafe(addr<uint8 const?>(ks[0])), long_length(ks))
            delete kq
            delete ks
        }
    } elif (fc.q8) {
        var front : Q8Region[3]
        if (!fc.rotary) {   // the rotary family's front convs stay f32 in the blob, uploaded as the f16 slab
            front = fc_front_records(fc.c3_w, fc.c6_w, fc.pre_w, fc.nch, fc.n_mel, fc.d)
        }
        let proj = Q8Region(woff = fc.proj_w, n = fc.d, rows = fc.proj_w >= 0l ? fc.proj_dim : 0l)
        var inscope regions <- fc_q8_regions(offs, fc.n_layer, fc.d, fc.ff, front, proj)
        wq |> reserve_resize(fc.qblob.n)
        ws |> reserve_resize(fc.qscales.n * 2l)
        tower_gather_q8_regions(fc.qblob, fc.qscales, regions, lay, wq, ws)
    }
    if (!empty(wq)) {
        tower_dev_put(b, tower_fc_piece(home, "wq"), wq)
        tower_dev_put(b, tower_fc_piece(home, "ws"), ws)
    }
    delete wq
    delete ws
}

//! the kq lane's record seats off a GPU image's device block (`tower_fc_bake`'s seats piece) into `seats`, each with its member pair
[cold_path]
def tower_fc_dev_seats(fc : TowerFc; home : string; @scratch @exact_size var seats : array<TowerFcSeat>) {
    seats |> clear()
    return if (!fc.kq_lane)
    let sp = tower_dev_find(fc.devplan, tower_fc_piece(home, "kq.seats"))
    let n = sp.bytes / 16l
    seats |> grow_resize(n)
    unsafe {
        let w = reinterpret<int64 const?>(plane_at(fc.dev, sp.off))
        for (i in range64(n)) {
            let fmt = kq_fmt_of_slot(int(w[2l * i]))
            seats[i] = TowerFcSeat(fmt = fmt, wblk0 = w[2l * i + 1l], member = fmt == KqFmt.q8 ? -1 : tower_fc_member(fc.kq_fmts, fmt))
        }
    }
}

//! the FastConformer front's f16 conv panel for home `home`'s device block, staged as the upload stages it
[cold_path]
def tower_cn_front_bake(fc : TowerFc; var b : TowerDevBuild; home : string) {
    var slab : array<float>
    tower_stage_conv0_panel(slab, fc.blob, fc.c0_w, fc.nch, 64l, fc.tap_major)
    var inscope slab16 <- cvt_f32_to_f16_sat_array(slab)
    delete slab
    tower_dev_put(b, tower_fc_piece(home, "front16"), unsafe(addr<uint8 const?>(slab16[0])), long_length(slab16) * 2l)
}

//! GigaAM's device-mel twin piece: the DFT twiddles [n_fft][2 n_bins], the window behind them (`tower_ga_mel_woff`)
def tower_ga_twin_piece(home : string) : string => "{home}.ga.twin"

//! GigaAM's front for `home`'s device block: conv0 [nch][kd0] then conv2 [d][kd2] as one f16 slab on the 64 lattice, then the mel's twiddles and window
[cold_path]
def tower_ga_front_bake(m : GigaamModel; var b : TowerDevBuild; home : string) : bool {
    let k = m.subs_kernel
    let nch = m.n_sub_ch
    let d = m.d_model
    let kd0 = round_up(k * m.n_mel, 64l)
    let kd2 = round_up(k * nch, 64l)
    let n_bins = m.n_fft / 2l + 1l
    let ntw = m.n_fft * 2l * n_bins
    return false if (long_length(m.tabs.twiddles) < ntw || length(m.window) < m.n_fft || length(m.filters) < m.n_mel * n_bins)
    var slab : array<float>
    slab |> reserve_resize(nch * kd0 + d * kd2)
    tower_stage_f32_pad_row_stride(slab, m.blob, m.c0_w, 0l, nch, k * m.n_mel, kd0)
    tower_stage_f32_pad_row_stride(slab, m.blob, m.c2_w, nch * kd0, d, k * nch, kd2)
    var inscope slab16 <- cvt_f32_to_f16_sat_array(slab)
    delete slab
    tower_dev_put(b, tower_fc_piece(home, "front16"), unsafe(addr<uint8 const?>(slab16[0])), long_length(slab16) * 2l)
    var inscope twin : array<float>
    twin |> reserve(ntw + m.n_fft)
    for (i in range64(ntw)) {
        twin |> push(m.tabs.twiddles[i])
    }
    for (j in range64(m.n_fft)) {
        twin |> push(m.window[j])
    }
    tower_dev_put(b, tower_ga_twin_piece(home), twin)
    return true
}

//! the rail site a block GEMM record rides: the ff-wide ups, the 2d-wide pointwise conv, the rel table's projection, the d-wide rest
def tower_fc_site(g : int64) : RailSite {
    if (g == FC_REC_FF1A || g == FC_REC_FF2A) {
        return RailSite.ff
    } elif (g == FC_REC_PW1) {
        return RailSite.d2
    } elif (g == FC_REC_POS) {
        return RailSite.rel
    }
    return RailSite.d
}

//! block record `rid`'s schedule row: its first weight block (the kq seat's), rows and workgroups; the rotary form's empty rel record carries none
def tower_fc_rec(rl : auto(R); fc : TowerFc; offs : array<CanaryLayerOffs>; seats : array<TowerFcSeat>; npos, ww, rid : int64) : TowerSchedRec {
    let g = rid % FC_GEMM_RECORDS
    let recs = fc_gemm_records(offs[rid / FC_GEMM_RECORDS], fc.d, fc.ff)
    let wblk0 = fc.kq_lane ? seats[rid].wblk0 : recs[g].woff / kq_elems(KqFmt.q8)
    return TowerSchedRec(wblk0 = wblk0, rows = 0l, groups = 0l) if (recs[g].rows == 0l)
    let site = tower_fc_site(g)
    let rows = g == FC_REC_POS ? ww : npos
    return TowerSchedRec(wblk0 = wblk0, rows = rail_site_rows(rl, site, rows), groups = rail_site_groups(rl, site, recs[g].rows, rows))
}

//! the site's planes bound to kq record `rid`'s seat: its member's pair and format, or the q8 planes
def private tower_fc_bind_seat(var rl : auto(R); site : RailSite; s : TowerFcSeat) {
    let i = int(site)
    rl.binds[i].fmt = s.fmt
    rl.binds[i].wq = s.member < 0 ? TowerBuf.wq : TOWER_KQ_Q[s.member]
    rl.binds[i].ws = s.member < 0 ? TowerBuf.ws : TOWER_KQ_S[s.member]
}

//! one GEMM x [npos x n] -> y [npos x dout] on the rail's lane (`rail_lane_mm`): the exact lane at `woff` rebased on `w_off`, else record `rid`'s tile
def private tower_fc_lane_mm(var rl : auto(R); fc : TowerFc; site : RailSite; rid : int64; x, y : TowerBuf; n, dout, npos, woff : int64) {
    rail_lane_mm(rl, site, rid, x, y, n, dout, npos, woff - fc.w_off)
}

//! one block GEMM: `tower_fc_lane_mm`, the site bound to the kq lane's seat of record `rid` first (its member and format)
def private tower_fc_mm(var rl : auto(R); fc : TowerFc; seats : array<TowerFcSeat>; site : RailSite; rid : int64; x, y : TowerBuf; n, dout, npos, woff : int64) {
    if (fc.kq_lane) {
        tower_fc_bind_seat(rl, site, seats[rid])
    }
    tower_fc_lane_mm(rl, fc, site, rid, x, y, n, dout, npos, woff)
}

//! the GEMM input `x`'s `nin` elements fed to the site's feed, then `tower_fc_mm`
def private tower_fc_fed_mm(var rl : auto(R); fc : TowerFc; seats : array<TowerFcSeat>; site : RailSite; rid : int64; x : TowerBuf; nin : int64; y : TowerBuf;
                            n, dout, npos, woff : int64) {
    rail_site_feed(rl, site, x, nin, -NO_CLAMP, NO_CLAMP)
    tower_fc_mm(rl, fc, seats, site, rid, x, y, n, dout, npos, woff)
}

//! a GEMM's bias row (a model offset) and activation over `nelem` of `x` where the family is biased, else the activation alone
def private tower_fc_bias(var rl : auto(R); biased : bool; x : TowerBuf; dout, nelem, boff : int64; act : uint) {
    if (biased) {
        var pc = GkBiasActArgs(d = uint(dout), nelem = uint(nelem), boff = uint(boff - rl.norms_base), act = act)
        enc_tower_bias_act_cls(rl, fixed_array(x, TowerBuf.norms), pc, rq_wgs(false, nelem))
    } elif (act != BIAS_ACT_NONE) {
        var pc = GkBiasActArgs(d = uint(dout), nelem = uint(nelem), boff = 0u, act = act)
        enc_tower_act_cls(rl, fixed_array(x), pc, rq_wgs(false, nelem))
    }
}

//! a block's seam x += a y (+ b) and its next layernorm into `xb`, on the biased stamp or the bare one
def private tower_fc_post(var rl : auto(R); biased : bool; y : TowerBuf; var pc : GkPostAddLnArgs; npos : int64) {
    if (biased) {
        enc_tower_post_add_ln_cls(rl, fixed_array(TowerBuf.x, y, TowerBuf.xb, TowerBuf.norms), pc, npos)
    } else {
        enc_tower_post_add_ln_nb_cls(rl, fixed_array(TowerBuf.x, y, TowerBuf.xb, TowerBuf.norms), pc, npos)
    }
}

//! the rel-position attention off the fed normed rows: q, k, v, the rel projection, per head R = (q + v) P^T on the f16 GEMM, into `att`
def private tower_fc_rel_attn(var rl : auto(R); fc : TowerFc; seats : array<TowerFcSeat>; lo : CanaryLayerOffs; rb, npos, ww : int64) {
    let nb = rl.norms_base
    let d = fc.d
    let nd = npos * d
    let hs = tower_fc_hs(d, fc.n_head)
    tower_fc_mm(rl, fc, seats, RailSite.d, rb + FC_REC_Q, TowerBuf.xb, TowerBuf.q, d, d, npos, lo.q_w)
    rail_pt(rl, RailProfRole.gemm_q)
    tower_fc_bias(rl, fc.biased, TowerBuf.q, d, nd, lo.q_b, BIAS_ACT_NONE)
    rail_pt(rl, RailProfRole.bias)
    tower_fc_mm(rl, fc, seats, RailSite.d, rb + FC_REC_K, TowerBuf.xb, TowerBuf.k, d, d, npos, lo.k_w)
    rail_pt(rl, RailProfRole.gemm_k)
    tower_fc_bias(rl, fc.biased, TowerBuf.k, d, nd, lo.k_b, BIAS_ACT_NONE)
    rail_pt(rl, RailProfRole.bias)
    tower_fc_mm(rl, fc, seats, RailSite.d, rb + FC_REC_V, TowerBuf.xb, TowerBuf.v, d, d, npos, lo.v_w)
    rail_pt(rl, RailProfRole.gemm_v)
    tower_fc_bias(rl, fc.biased, TowerBuf.v, d, nd, lo.v_b, BIAS_ACT_NONE)
    rail_pt(rl, RailProfRole.bias)
    tower_fc_mm(rl, fc, seats, RailSite.rel, rb + FC_REC_POS, TowerBuf.rel_tab, TowerBuf.rel_proj, d, d, ww, lo.pos_w)
    rail_pt(rl, RailProfRole.gemm_rel)
    var pc_attn = TowerCnAttnRArgs(d = uint(d), npos = uint(npos), buoff = uint(lo.bias_u - nb), hoff = 0u)
    let wg_attn = (npos + 63l) / 64l
    for (hh in range64(fc.n_head)) {
        let hoff = hh * hs
        var pc_gq = GkRestrideArgs(rows = uint(npos), rows_pad = uint(npos), nheads = 1u, hs = uint(hs), hsp = uint(hs), sstride = uint(d), soff = uint(hoff),
            boff = uint(lo.bias_v + hoff - nb))
        enc_tower_hpad16_b_cls(rl, fixed_array(TowerBuf.q, TowerBuf.rel_qh, TowerBuf.norms), pc_gq, (npos * hs + 255l) / 256l)
        var pc_gp = GkRestrideArgs(rows = uint(ww), rows_pad = uint(ww), nheads = 1u, hs = uint(hs), hsp = uint(hs), sstride = uint(d), soff = uint(hoff))
        enc_tower_hpad16_cls(rl, fixed_array(TowerBuf.rel_proj, TowerBuf.rel_ph), pc_gp, (ww * hs + 255l) / 256l)
        rail_mm16(rl, TowerBuf.rel_ph, TowerBuf.rel_qh, TowerBuf.rel_r, hs, ww, npos, 0l, ww)
        pc_attn.hoff = uint(hoff)
        let roles = fixed_array(TowerBuf.q, TowerBuf.k, TowerBuf.v, TowerBuf.att, TowerBuf.rel_r, TowerBuf.norms)
        if (hs == 64l) {
            enc_tower_cn_attn_r64_cls(rl, roles, pc_attn, wg_attn)
        } else {
            enc_tower_cn_attn_r_cls(rl, roles, pc_attn, wg_attn)
        }
    }
    rail_pt(rl, RailProfRole.attn)
}

//! the rotary attention off `xb`: v unrotated, the rope in place (`tabs`: cos at 0, sin at `npos * hs / 2`), q and k off the rotated rows, into `att`
def private tower_fc_rot_attn(var rl : auto(R); fc : TowerFc; seats : array<TowerFcSeat>; lo : CanaryLayerOffs; rb, npos : int64) {
    let d = fc.d
    let nd = npos * d
    let hs = tower_fc_hs(d, fc.n_head)
    let half = hs / 2l
    tower_fc_mm(rl, fc, seats, RailSite.d, rb + FC_REC_V, TowerBuf.xb, TowerBuf.v, d, d, npos, lo.v_w)
    rail_pt(rl, RailProfRole.gemm_v)
    var pc_rope = GkRopeTabArgs(rows = uint(npos), qs = uint(d), col = 0u, d = uint(d), dh = uint(hs), coff = 0u, soff = uint(npos * half))
    enc_tower_rope_tab_cls(rl, fixed_array(TowerBuf.xb, TowerBuf.tabs, TowerBuf.tabs), pc_rope, (npos * (d / 2l) + 255l) / 256l)
    rail_site_feed(rl, RailSite.d, TowerBuf.xb, nd, -NO_CLAMP, NO_CLAMP)
    rail_pt(rl, RailProfRole.rq)
    tower_fc_mm(rl, fc, seats, RailSite.d, rb + FC_REC_Q, TowerBuf.xb, TowerBuf.q, d, d, npos, lo.q_w)
    rail_pt(rl, RailProfRole.gemm_q)
    tower_fc_mm(rl, fc, seats, RailSite.d, rb + FC_REC_K, TowerBuf.xb, TowerBuf.k, d, d, npos, lo.k_w)
    rail_pt(rl, RailProfRole.gemm_k)
    tower_attn_padded(rl, TowerBuf.q, TowerBuf.k, TowerBuf.v, d, npos, fc.n_head, hs, tower_fc_hsp(hs), lo.q_b, lo.k_b, lo.v_b, 0l, 0l, 0l, false)
}

//! the conv module's depthwise conv `hu` -> `yd`: GigaAM's biased taps then LayerNorm and SiLU, else the centered conv with folded BatchNorm and silu
def private tower_fc_dw(var rl : auto(R); fc : TowerFc; lo : CanaryLayerOffs; npos : int64) {
    let nb = rl.norms_base
    let d = fc.d
    let nd = npos * d
    var pc_dw = GkDwConvRowsArgs(c = uint(d), k = uint(fc.conv_kernel), t_in = uint(npos), woff = uint(lo.dw_w - nb), bnwoff = uint(lo.bn_w - nb),
        bnboff = uint(lo.bn_b - nb), boff = uint(max(lo.dw_b - nb, 0l)))
    let wg = (nd + 255l) / 256l
    if (fc.conv_ln) {
        if (fc.taps_n > 0l) {
            pc_dw.woff = uint(lo.dw_w - fc.taps_off)
            enc_tower_ga_dwh_cls(rl, fixed_array(TowerBuf.hu, TowerBuf.yd, TowerBuf.norms, TowerBuf.taps), pc_dw, wg)
        } else {
            enc_tower_ga_dw_cls(rl, fixed_array(TowerBuf.hu, TowerBuf.yd, TowerBuf.norms), pc_dw, wg)
        }
        rail_pt(rl, RailProfRole.dwconv)
        var pc_lncv = gk_ln_args(uint(d), uint(npos), fc.eps, uint(lo.bn_w - nb), uint(lo.bn_b - nb))
        enc_tower_ln_cls(rl, fixed_array(TowerBuf.yd, TowerBuf.yd, TowerBuf.norms), pc_lncv, npos)
        rail_pt(rl, RailProfRole.ln)
        var pc_silu = GkBiasActArgs(d = uint(d), nelem = uint(nd), boff = 0u, act = BIAS_ACT_SILU)
        enc_tower_act_cls(rl, fixed_array(TowerBuf.yd), pc_silu, rq_wgs(false, nd))
        rail_pt(rl, RailProfRole.silu)
    } elif (fc.tap_major) {
        enc_tower_pk_dw_cls(rl, fixed_array(TowerBuf.hu, TowerBuf.yd, TowerBuf.norms), pc_dw, wg)
    } else {
        enc_tower_cn_dw_cls(rl, fixed_array(TowerBuf.hu, TowerBuf.yd, TowerBuf.norms), pc_dw, wg)
    }
    rail_pt(rl, RailProfRole.dwconv)
}

//! the FastConformer block loop over the `npos` rows in `x` (`ww` the rel window, `tabs` the rotary tables), each GEMM on the chain's lane; the rows stay in `x`
[arch(at = "../ARCHITECTURE_GPU_TOWER_RAIL.md#fc-chains")]
def tower_fc_blocks(var rl : auto(R); fc : TowerFc; offs : array<CanaryLayerOffs>; seats : array<TowerFcSeat>; npos, ww : int64) {   // nolint:STYLE038 — the dispatch chain is the shape
    let nb = rl.norms_base
    let d = fc.d
    let ff = fc.ff
    let nd = npos * d
    let nff = npos * ff
    let n2d = npos * 2l * d
    if (!fc.rotary) {
        var pc_pos = GkRelSinCosArgs(d = uint(d), tt = uint(npos), rows_pad = uint(ww))
        enc_tower_rel_sincos_cls(rl, fixed_array(TowerBuf.rel_tab), pc_pos, (ww * d + 255l) / 256l)
        rail_site_feed(rl, RailSite.rel, TowerBuf.rel_tab, ww * d, -NO_CLAMP, NO_CLAMP)
        rail_pt(rl, RailProfRole.rq)
    }
    var pc_ln = gk_ln_args(uint(d), uint(npos), fc.eps, 0u, 0u)
    var pc_post = GkPostAddLnArgs(d = uint(d), rows = uint(npos), woff = 0u, boff = 0u, eps = fc.eps, ln_on = 1u, lnoff = 0u, lnboff = 0u, ascale = 1.0, xboff = 0u)
    for (l in range64(fc.n_layer)) {
        let lo = offs[l]
        let rb = l * FC_GEMM_RECORDS
        pc_ln.lnoff = uint(lo.nf1_w - nb)
        pc_ln.lnboff = uint(lo.nf1_b - nb)
        enc_tower_ln_cls(rl, fixed_array(TowerBuf.x, TowerBuf.xb, TowerBuf.norms), pc_ln, npos)
        rail_pt(rl, RailProfRole.ln)
        tower_fc_fed_mm(rl, fc, seats, RailSite.ff, rb + FC_REC_FF1A, TowerBuf.xb, nd, TowerBuf.hg, d, ff, npos, lo.ff1a_w)
        rail_pt(rl, RailProfRole.gemm_fc1)
        tower_fc_bias(rl, fc.biased, TowerBuf.hg, ff, nff, lo.ff1a_b, BIAS_ACT_SILU)
        rail_pt(rl, RailProfRole.bias)
        tower_fc_fed_mm(rl, fc, seats, RailSite.d, rb + FC_REC_FF1B, TowerBuf.hg, nff, TowerBuf.yd, ff, d, npos, lo.ff1b_w)
        rail_pt(rl, RailProfRole.gemm_fc2)
        pc_post.boff = uint(lo.ff1b_b - nb)
        pc_post.ascale = 0.5
        pc_post.lnoff = uint(lo.nat_w - nb)
        pc_post.lnboff = uint(lo.nat_b - nb)
        tower_fc_post(rl, fc.biased, TowerBuf.yd, pc_post, npos)
        rail_pt(rl, RailProfRole.post_add)
        rail_site_feed(rl, RailSite.d, TowerBuf.xb, nd, -NO_CLAMP, NO_CLAMP)
        rail_pt(rl, RailProfRole.rq)
        if (fc.rotary) {
            tower_fc_rot_attn(rl, fc, seats, lo, rb, npos)
        } else {
            tower_fc_rel_attn(rl, fc, seats, lo, rb, npos, ww)
        }
        tower_fc_fed_mm(rl, fc, seats, RailSite.d, rb + FC_REC_O, TowerBuf.att, nd, TowerBuf.yo, d, d, npos, lo.o_w)
        rail_pt(rl, RailProfRole.gemm_o)
        pc_post.boff = uint(lo.o_b - nb)
        pc_post.ascale = 1.0
        pc_post.lnoff = uint(lo.ncv_w - nb)
        pc_post.lnboff = uint(lo.ncv_b - nb)
        tower_fc_post(rl, fc.biased, TowerBuf.yo, pc_post, npos)
        rail_pt(rl, RailProfRole.post_add)
        tower_fc_fed_mm(rl, fc, seats, RailSite.d2, rb + FC_REC_PW1, TowerBuf.xb, nd, TowerBuf.hg, d, 2l * d, npos, lo.pw1_w)
        rail_pt(rl, RailProfRole.gemm_pw)
        tower_fc_bias(rl, fc.biased, TowerBuf.hg, 2l * d, n2d, lo.pw1_b, BIAS_ACT_NONE)
        rail_pt(rl, RailProfRole.bias)
        var pc_glu = GkGatedRowsArgs(d = uint(d), nelem = uint(nd))
        enc_tower_glu_sig_cls(rl, fixed_array(TowerBuf.hg, TowerBuf.hu), pc_glu, (nd + 255l) / 256l)
        rail_pt(rl, RailProfRole.glu)
        tower_fc_dw(rl, fc, lo, npos)
        tower_fc_fed_mm(rl, fc, seats, RailSite.d, rb + FC_REC_PW2, TowerBuf.yd, nd, TowerBuf.yo, d, d, npos, lo.pw2_w)
        rail_pt(rl, RailProfRole.gemm_pw)
        pc_post.boff = uint(lo.pw2_b - nb)
        pc_post.lnoff = uint(lo.nf2_w - nb)
        pc_post.lnboff = uint(lo.nf2_b - nb)
        tower_fc_post(rl, fc.biased, TowerBuf.yo, pc_post, npos)
        rail_pt(rl, RailProfRole.post_add)
        tower_fc_fed_mm(rl, fc, seats, RailSite.ff, rb + FC_REC_FF2A, TowerBuf.xb, nd, TowerBuf.hg, d, ff, npos, lo.ff2a_w)
        rail_pt(rl, RailProfRole.gemm_fc1)
        tower_fc_bias(rl, fc.biased, TowerBuf.hg, ff, nff, lo.ff2a_b, BIAS_ACT_SILU)
        rail_pt(rl, RailProfRole.bias)
        tower_fc_fed_mm(rl, fc, seats, RailSite.d, rb + FC_REC_FF2B, TowerBuf.hg, nff, TowerBuf.yd, ff, d, npos, lo.ff2b_w)
        rail_pt(rl, RailProfRole.gemm_fc2)
        pc_post.boff = uint(lo.ff2b_b - nb)
        pc_post.ascale = 0.5
        pc_post.ln_on = 0u
        tower_fc_post(rl, fc.biased, TowerBuf.yd, pc_post, npos)
        rail_pt(rl, RailProfRole.post_add)
        pc_post.ln_on = 1u
        pc_ln.lnoff = uint(lo.nout_w - nb)
        pc_ln.lnboff = uint(lo.nout_b - nb)
        enc_tower_ln_cls(rl, fixed_array(TowerBuf.x, TowerBuf.x, TowerBuf.norms), pc_ln, npos)
        rail_pt(rl, RailProfRole.ln)
    }
}

//! canary's projector: the rows in `x` fed to the `tail1` site, its GEMM on the chain's lane (q8 record 0) into `hg` with its bias
[arch(at = "../ARCHITECTURE_GPU_TOWER_RAIL.md#fc-chains")]
def tower_fc_proj(var rl : auto(R); fc : TowerFc; npos : int64) {
    let pdim = fc.proj_dim
    rail_site_feed(rl, RailSite.tail1, TowerBuf.x, npos * fc.d, -NO_CLAMP, NO_CLAMP)
    tower_fc_lane_mm(rl, fc, RailSite.tail1, 0l, TowerBuf.x, TowerBuf.hg, fc.d, pdim, npos, fc.proj_w)
    tower_fc_bias(rl, true, TowerBuf.hg, pdim, npos * pdim, fc.proj_b, BIAS_ACT_NONE)
}

//! the subsample stack's geometry over `n_len` mel frames: each stride-2 stage's frames (`t`) and mel width (`w`)
struct TowerFcGeom {
    t1 : int64
    w1 : int64
    t2 : int64
    w2 : int64
    t3 : int64
    w3 : int64
}

def tower_fc_geom(n_len, n_mel : int64) : TowerFcGeom {
    let t1 = conv_s2_out(n_len)
    let w1 = conv_s2_out(n_mel)
    let t2 = conv_s2_out(t1)
    let w2 = conv_s2_out(w1)
    return TowerFcGeom(t1 = t1, w1 = w1, t2 = t2, w2 = w2, t3 = conv_s2_out(t2), w3 = conv_s2_out(w2))
}

//! the canary / parakeet front's plane bytes at `cap` mel frames, a plane a role: the frames, the spectrum, the mel rows, the
//! column panel, the stage rows, the feature rows, the f16 feed (the tile's read slack behind its rows), the input projection's
//! rows, the twiddles and the filters
struct TowerFcFrontBytes {
    xw : int64
    pow : int64
    mel : int64
    col : int64
    y0 : int64
    y2 : int64
    y3 : int64
    xf : int64
    h : int64
    x : int64
    tw : int64
    fil : int64
}

def tower_fc_front_bytes(fc : TowerFc; cap : int64) : TowerFcFrontBytes {
    let g = tower_fc_geom(cap, fc.n_mel)
    let rows0 = g.t1 * g.w1
    let rows2 = g.t2 * g.w2
    let rows3 = g.t3 * g.w3
    return TowerFcFrontBytes(xw = cap * fc.n_fft * 4l, pow = cap * fc.n_fb * 4l, mel = cap * fc.n_mel * 4l, col = rows0 * 64l * 2l, y0 = rows0 * fc.nch * 4l,
        y2 = rows2 * fc.nch * 4l, y3 = rows3 * fc.nch * 4l, xf = g.t3 * g.w3 * fc.nch * 4l, h = (rows2 + TILE_READ_SLACK) * fc.nch * 2l, x = g.t3 * fc.d * 4l,
        tw = fc.n_fft * 2l * fc.n_fb * 4l, fil = fc.n_mel * fc.n_fb * 4l)
}

let TOWER_GA_COL_LATTICE = 128l   //! GigaAM's front column panel rows: the widest f16 tile's (the cm2 tile's 128, the KHR tile's 64)

//! GigaAM's front plane bytes at `cap` mel frames: the samples the frames reach, the frames, the power and mel rows, one column panel
//! both convs share (on the widest f16 tile's row lattice, its read slack behind), conv0's rows, the residual rows, the twiddle
//! plane (the window behind the twiddles) and the filters
struct TowerGaFrontBytes {
    samples : int64
    frames : int64
    pow : int64
    mel : int64
    col : int64
    y1 : int64
    x : int64
    tw : int64
    fil : int64
}

def tower_ga_front_bytes(m : GigaamModel; cap : int64) : TowerGaFrontBytes {
    let k = m.subs_kernel
    let t1 = gigaam_conv_out(cap, k)
    let tt = gigaam_conv_out(t1, k)
    let kd0 = round_up(k * m.n_mel, 64l)
    let kd2 = round_up(k * m.n_sub_ch, 64l)
    let n_bins = m.n_fft / 2l + 1l
    return TowerGaFrontBytes(samples = ((cap - 1l) * m.hop + m.n_fft) * 4l, frames = cap * m.n_fft * 4l, pow = cap * n_bins * 4l, mel = cap * m.n_mel * 4l,
        col = (round_up(max(t1, tt), TOWER_GA_COL_LATTICE) + TILE_READ_SLACK) * max(kd0, kd2) * 2l, y1 = t1 * m.n_sub_ch * 4l, x = tt * m.d_model * 4l,
        tw = (tower_ga_mel_woff(m.n_fft) + m.n_fft) * 4l, fil = m.n_mel * n_bins * 4l)
}

//! a front stage's rows cast to its GEMM's f16 feed
def private tower_fc_cvt16(var rl : auto(R); src, dst : TowerBuf; nelem : int64) {
    var pc = gk_f16_rows_args(uint(nelem))
    enc_tower_clamp_cvt_cls(rl, fixed_array(src, dst), pc, rq_wgs(false, nelem))
}

//! a front GEMM x [rows x n] -> y [rows x dout]: the rows cast to the `feat16` feed, then `tower_fc_lane_mm` (front record `rid` on the q8 lane)
def private tower_fc_front_mm(var rl : auto(R); fc : TowerFc; site : RailSite; rid : int64; x, y : TowerBuf; n, dout, rows, woff : int64) {
    tower_fc_cvt16(rl, x, TowerBuf.feat16, rows * n)
    tower_fc_lane_mm(rl, fc, site, rid, x, y, n, dout, rows, woff)
}

//! the subsample depthwise conv2d on the family's tap layout
def private tower_fc_dw2d(var rl : auto(R); tap_major : bool; src, dst : TowerBuf; var pc : GkDwConv2dArgs; wgs : int64) {
    if (tap_major) {
        enc_tower_pk_dw2d_cls(rl, fixed_array(src, dst, TowerBuf.norms), pc, wgs)
    } else {
        enc_tower_dw_conv_cls(rl, fixed_array(src, dst, TowerBuf.norms), pc, wgs)
    }
}

//! the canary / parakeet subsample front over `n_len` frames (`n_valid` live): canary's mel off `planes` or parakeet's CPU rows in `mel`, into `rows_out`
[arch(at = "../ARCHITECTURE_GPU_TOWER_RAIL.md#fc-chains")]
def tower_fc_front(var rl : auto(R); fc : TowerFc; n_len, n_valid : int64) {   // nolint:STYLE038 — the dispatch chain is the shape
    let nb = rl.norms_base
    let n_mel = fc.n_mel
    let n_fb = fc.n_fb
    let nch = fc.nch
    let d = fc.d
    let g = tower_fc_geom(n_len, n_mel)
    let rows0 = g.t1 * g.w1
    let rows2 = g.t2 * g.w2
    let rows3 = g.t3 * g.w3
    let tt = g.t3
    let feat = g.w3 * nch
    let l0 = conv_s2_out(n_valid)
    let l2 = conv_s2_out(l0)
    if (fc.device_mel) {
        var pc_dft = TowerSpecDftArgs(n_fft = uint(fc.n_fft), n_frames = uint(n_len), mag = TOWER_SPEC_POWER)
        enc_tower_spec_dft_cls(rl, fixed_array(TowerBuf.planes, TowerBuf.twiddle, TowerBuf.spec), pc_dft, (n_len * n_fb + 255l) / 256l)
        rail_pt(rl, RailProfRole.spectrum)
        var pc_mel = GkMelLogArgs(n_bins = uint(n_fb), n_mel = uint(n_mel), frames = uint(n_len),
            lo = TOWER_FC_MEL_FLOOR, hi = 0.0, scale = 1.0, frame_major = 1u, form = GK_MEL_LOG_ADD)
        enc_tower_mel_log_cls(rl, fixed_array(TowerBuf.spec, TowerBuf.filters, TowerBuf.mel), pc_mel, (n_len * n_mel + 255l) / 256l)
        var pc_norm = GkMelNormArgs(n_mel = uint(n_mel), n_frames = uint(n_len), valid = uint(min(n_valid, n_len)))
        enc_tower_mel_norm_cls(rl, fixed_array(TowerBuf.mel), pc_norm, (n_mel + 63l) / 64l)
        rail_pt(rl, RailProfRole.mel)
    }
    var pc_im0 = GkIm2col2dArgs(iw = uint(n_mel), ih = uint(n_len), cin = 1u, sstride = 1u, kdp = 64u, rows = uint(rows0), soff = 0u)
    enc_tower_im2col_cls(rl, fixed_array(TowerBuf.mel, TowerBuf.col), pc_im0, (rows0 * 64l + 255l) / 256l)
    rail_pt(rl, RailProfRole.im2col)
    rail_mm16(rl, TowerBuf.fw16, TowerBuf.col, TowerBuf.stage1, 64l, nch, rows0, 0l)
    rail_pt(rl, RailProfRole.gemm_pw)
    tower_fc_bias(rl, true, TowerBuf.stage1, nch, rows0 * nch, fc.c0_b, BIAS_ACT_RELU)
    rail_pt(rl, RailProfRole.bias)
    var pc_dw2 = GkDwConv2dArgs(iw = uint(g.w1), ih = uint(g.t1), ih_valid = uint(l0), ch = uint(nch), woff = uint(fc.c2_w - nb), boff = uint(fc.c2_b - nb))
    tower_fc_dw2d(rl, fc.tap_major, TowerBuf.stage1, TowerBuf.stage2, pc_dw2, (rows2 * nch + 255l) / 256l)
    rail_pt(rl, RailProfRole.dwconv)
    tower_fc_front_mm(rl, fc, RailSite.front1, FC_FRONT_REC_C3, TowerBuf.stage2, TowerBuf.stage3, nch, nch, rows2, fc.c3_w)
    rail_pt(rl, RailProfRole.gemm_pw)
    tower_fc_bias(rl, true, TowerBuf.stage3, nch, rows2 * nch, fc.c3_b, BIAS_ACT_RELU)
    rail_pt(rl, RailProfRole.bias)
    var pc_dw5 = GkDwConv2dArgs(iw = uint(g.w2), ih = uint(g.t2), ih_valid = uint(l2), ch = uint(nch), woff = uint(fc.c5_w - nb), boff = uint(fc.c5_b - nb))
    tower_fc_dw2d(rl, fc.tap_major, TowerBuf.stage3, TowerBuf.stage4, pc_dw5, (rows3 * nch + 255l) / 256l)
    rail_pt(rl, RailProfRole.dwconv)
    tower_fc_front_mm(rl, fc, RailSite.front2, FC_FRONT_REC_C6, TowerBuf.stage4, TowerBuf.stage5, nch, nch, rows3, fc.c6_w)
    rail_pt(rl, RailProfRole.gemm_pw)
    tower_fc_bias(rl, true, TowerBuf.stage5, nch, rows3 * nch, fc.c6_b, BIAS_ACT_RELU)
    rail_pt(rl, RailProfRole.bias)
    var pc_sh = GkFeatShuffleArgs(w = uint(g.w3), ch = uint(nch), t = uint(tt), rows_pad = uint(tt))
    enc_tower_feat_shuffle_cls(rl, fixed_array(TowerBuf.stage5, TowerBuf.feat), pc_sh, (tt * feat + 255l) / 256l)
    rail_pt(rl, RailProfRole.shuffle)
    tower_fc_front_mm(rl, fc, RailSite.front3, FC_FRONT_REC_PRE, TowerBuf.feat, TowerBuf.rows_out, feat, d, tt, fc.pre_w)
    rail_pt(rl, RailProfRole.gemm_pw)
    tower_fc_bias(rl, true, TowerBuf.rows_out, d, tt * d, fc.pre_b, BIAS_ACT_NONE)
    rail_pt(rl, RailProfRole.bias)
}

//! the front's three schedule records, each on its own front site, at their raw rows: the stage planes hold exactly them
def tower_fc_front_rec(rl : auto(R); fc : TowerFc; n_len, rid : int64) : TowerSchedRec {
    let g = tower_fc_geom(n_len, fc.n_mel)
    let q8e = kq_elems(KqFmt.q8)
    if (rid == FC_FRONT_REC_C3) {
        return TowerSchedRec(wblk0 = fc.c3_w / q8e, rows = g.t2 * g.w2, groups = rail_site_groups(rl, RailSite.front1, fc.nch, g.t2 * g.w2))
    } elif (rid == FC_FRONT_REC_C6) {
        return TowerSchedRec(wblk0 = fc.c6_w / q8e, rows = g.t3 * g.w3, groups = rail_site_groups(rl, RailSite.front2, fc.nch, g.t3 * g.w3))
    }
    return TowerSchedRec(wblk0 = fc.pre_w / q8e, rows = g.t3, groups = rail_site_groups(rl, RailSite.front3, fc.d, g.t3))
}

//! the front's three GEMM sites fed off the feature rows' f16 feed
def tower_fc_front_binds(var rl : auto(R)) {
    rail_bind_f16(rl, fixed_array(RailSite.front1, RailSite.front2, RailSite.front3), TowerBuf.feat16)
}

//! GigaAM's device mel off the samples in `planes` (`#gigaam-log-mel`): the window into `frames`, the power spectrum into `spec`, the log rows into `mel`
[arch(at = "../ARCHITECTURE_GPU_TOWER_RAIL.md#fc-chains")]
def tower_ga_mel(var rl : auto(R); m : GigaamModel; n_len : int64) {
    let n_bins = m.n_fft / 2l + 1l
    var pc_fw = GkFrameWindowArgs(n_fft = uint(m.n_fft), hop = uint(m.hop), frames = uint(n_len), rows = uint(n_len), woff = uint(tower_ga_mel_woff(m.n_fft)))
    enc_tower_ga_frames_cls(rl, fixed_array(TowerBuf.planes, TowerBuf.twiddle, TowerBuf.frames), pc_fw, (n_len * m.n_fft + 255l) / 256l)
    var pc_dft = TowerSpecDftArgs(n_fft = uint(m.n_fft), n_frames = uint(n_len), mag = TOWER_SPEC_POWER)
    enc_tower_spec_dft_cls(rl, fixed_array(TowerBuf.frames, TowerBuf.twiddle, TowerBuf.spec), pc_dft, (n_len * n_bins + 255l) / 256l)
    rail_pt(rl, RailProfRole.spectrum)
    var pc_mel = GkMelLogArgs(n_bins = uint(n_bins), n_mel = uint(m.n_mel), frames = uint(n_len),
        lo = GIGAAM_MEL_FLOOR, hi = GIGAAM_MEL_CEIL, scale = 1.0, frame_major = 1u, form = GK_MEL_LOG_CLAMP)
    enc_tower_mel_log_cls(rl, fixed_array(TowerBuf.spec, TowerBuf.filters, TowerBuf.mel), pc_mel, (n_len * m.n_mel + 255l) / 256l)
    rail_pt(rl, RailProfRole.mel)
}

//! GigaAM's conv front over `n_len` mel frames (the device mel first under `dev_mel`, else the CPU rows in `mel`): two stride-2 convs into `rows_out`
[arch(at = "../ARCHITECTURE_GPU_TOWER_RAIL.md#fc-chains")]
def tower_ga_front(var rl : auto(R); m : GigaamModel; n_len : int64; dev_mel : bool) {
    let k = m.subs_kernel
    let nch = m.n_sub_ch
    let d = m.d_model
    let t1 = gigaam_conv_out(n_len, k)
    let tt = gigaam_conv_out(t1, k)
    let kd0 = round_up(k * m.n_mel, 64l)
    let kd2 = round_up(k * nch, 64l)
    let pad = uint((k - 1l) / 2l)
    let mp1 = round_up(t1, TOWER_GA_COL_LATTICE)
    let mp2 = round_up(tt, TOWER_GA_COL_LATTICE)
    if (dev_mel) {
        tower_ga_mel(rl, m, n_len)
    }
    var pc_im0 = GkIm2col1dArgs(cin = uint(m.n_mel), tin = uint(n_len), tout = uint(t1), k = uint(k), stride = 2u, pad_l = pad, dilation = 1u,
        kdim = uint(kd0), row_major = 1u, r0 = 0u, mp = uint(mp1))
    enc_tts_im2col16_cls(rl, fixed_array(TowerBuf.mel, TowerBuf.col), pc_im0, mp1 * kd0)
    rail_pt(rl, RailProfRole.im2col)
    rail_mm16(rl, TowerBuf.fw16, TowerBuf.col, TowerBuf.stage1, kd0, nch, t1, 0l)
    rail_pt(rl, RailProfRole.gemm_pw)
    tower_fc_bias(rl, true, TowerBuf.stage1, nch, t1 * nch, m.c0_b, BIAS_ACT_RELU)
    rail_pt(rl, RailProfRole.bias)
    var pc_im2 = GkIm2col1dArgs(cin = uint(nch), tin = uint(t1), tout = uint(tt), k = uint(k), stride = 2u, pad_l = pad, dilation = 1u,
        kdim = uint(kd2), row_major = 1u, r0 = 0u, mp = uint(mp2))
    enc_tts_im2col16_cls(rl, fixed_array(TowerBuf.stage1, TowerBuf.col), pc_im2, mp2 * kd2)
    rail_pt(rl, RailProfRole.im2col)
    rail_mm16(rl, TowerBuf.fw16, TowerBuf.col, TowerBuf.rows_out, kd2, d, tt, nch * kd0)
    rail_pt(rl, RailProfRole.gemm_pw)
    tower_fc_bias(rl, true, TowerBuf.rows_out, d, tt * d, m.c2_b, BIAS_ACT_RELU)
    rail_pt(rl, RailProfRole.bias)
}

//! why a discrete home's transducer seat leaves a clip to the CPU loop; the decline counters key on the value names
enum RnntDevDecline {
    knob     //! the home's transducer knob is off
    flavor   //! a CPU-flavored net (`#tower-dev-block`): its image carries no device halves, the CPU decodes the lane whole
    shape    //! a predictor or joint past the bodies' staged widths, a third layer, a duration table past the state's, a buffer past the device's range
    device   //! the home has no device, or a class failed to build
    memory   //! a device or host allocation failed
    overrun  //! the rounds ran past the clip's budget with the clip not done
}

//! a transducer decline in words: `home` the home's name in its image flavor, `knob`, `device` and `memory` the home's own spelling of those reasons
[cold_path]
def rnnt_dev_decline_words(why : RnntDevDecline; home, knob, device, memory : string) : string {
    if (why == RnntDevDecline.knob) {
        return knob
    } elif (why == RnntDevDecline.flavor) {
        return "the model is not the {home}-flavored image"
    } elif (why == RnntDevDecline.shape) {
        return "a predictor, joint or duration table past the stamps' staged widths, or a buffer past the device's range"
    } elif (why == RnntDevDecline.memory) {
        return memory
    } elif (why == RnntDevDecline.overrun) {
        return "the rounds ran past the clip's budget with the clip not done"
    }
    return device
}

let RNNT_DEV_RATE0 = 0.3lf   //! the rounds a frame a transducer seat estimates its first clip's command at, before any clip served

//! why a decoder seat uploads its encoder rows: the session recorded no device encode (`gen` 0), or the rows on the device are another's
def private asr_dec_miss_why(gen : uint64) : string
    => gen == 0ul ? "the session recorded no device encode" : "another seat ran since the encode, or the rows are another clip's"

//! the transducer seat's say-once line where a projecting decode uploads its encoder rows: `home` the home's log word; `said` set
[cold_path]
def rnnt_dev_miss_say(var said : bool&; home : string; gen : uint64) {
    said = true
    to_log(LOG_INFO, "dasLLAMA {home} rnnt: the decode uploads its encoder rows - {asr_dec_miss_why(gen)}\n")
}

//! the transducer seat's serves line, said once a resident: `home` the home's log word, `projects` the joint's encoder projection on the device; `said` set
[cold_path]
def rnnt_dev_announce(var said : bool&; home : string; pd, jd, n_logits : int64; projects : bool) {
    said = true
    let proj = (projects
        ? "the joint's encoder projection on the device"
        : "the joint's encoder projection on the CPU, uploaded - the q8 lane holds it in a q8 plane and the device projects the f32 plane alone")
    to_log(LOG_INFO, "dasLLAMA {home} rnnt: the transducer decode serves on the device (pred {pd}, joint {jd}, {n_logits} logits; {proj})\n")
}

//! a FastConformer front's line where it starts from the CPU mel (`TowerFc.device_mel` off), said with the seat's first served encode: `home` the home's log word
[cold_path]
def tower_fc_cpu_mel_say(home, family : string) {
    to_log(LOG_INFO, "dasLLAMA {home} tower: {family}'s front starts from the CPU mel - its encode hook takes the CPU's log-mel rows, uploaded, and no home serves its mel on the device\n")
}

//! why a discrete home's CTC head leaves a clip to the CPU head; the decline counters key on the value names
enum CtcDevDecline {
    knob     //! the home's tower knob is off
    shape    //! d off the f16 GEMM's 64-wide K lattice, no frames, or a buffer past the device's range
    device   //! the home has no device, a class failed to build, or an f16 head on a device with no f16 GEMM tile
    memory   //! a device or host allocation failed
    flavor   //! the model is not the home's flavored image: each backend serves its own (`#tower-dev-block`)
}

//! a CTC head decline in words: `home` the home's name in its image flavor, `knob`, `device` and `memory` the home's own spelling of those reasons
[cold_path]
def ctc_dev_decline_words(why : CtcDevDecline; home, knob, device, memory : string) : string {
    if (why == CtcDevDecline.knob) {
        return knob
    } elif (why == CtcDevDecline.shape) {
        return "d off the f16 GEMM's 64-wide lattice, no frames, or a buffer past the device's range"
    } elif (why == CtcDevDecline.memory) {
        return memory
    } elif (why == CtcDevDecline.flavor) {
        return "the model is not the {home}-flavored image"
    }
    return device
}

//! the CTC seat's say-once line where a head uploads its encoder rows: `home` the home's log word; `said` set
[cold_path]
def ctc_dev_miss_say(var said : bool&; home : string; gen : uint64) {
    said = true
    to_log(LOG_INFO, "dasLLAMA {home} ctc: the head uploads its encoder rows - {asr_dec_miss_why(gen)}\n")
}

//! the CTC seat's serves line, said once a resident: `home` the home's log word; `said` set
[cold_path]
def ctc_dev_announce(var said : bool&; home : string; nc, d : int64; f16 : bool) {
    said = true
    to_log(LOG_INFO, "dasLLAMA {home} ctc: the CTC head serves on the device ({nc} classes over {d}, {f16 ? "the f16 GEMM over the carrier's halfwords" : "the f32 tile"})\n")
}

//! one command buffer of the transducer's greedy decode (`#rnnt-device-decode`): the encoder projection under `project`, then `rounds` rounds on `dec_state`
[arch(at = "../ARCHITECTURE_GPU_TOWER_RAIL.md#fc-chains")]
def asr_dec_rnnt_rounds(var rl : auto(R); net : RnntNet; lay : RnntDevLayout; ka : GkRnntArgs; d, n_frames, rounds : int64; project : bool) {
    let jd = net.joint_dim
    if (project) {
        rail_mm32(rl, TowerBuf.enc_proj_w, false, TowerBuf.rows_out, TowerBuf.enc_proj, d, jd, n_frames, 0l, TOWER_NO_BIAS, 0l)
    }
    let n_logits = rnnt_n_logits(net)
    for (_r in range64(rounds)) {
        for (l in range64(net.n_layers)) {
            var kl = rnnt_dev_layer_args(ka, lay, l)
            enc_rnnt_gates_cls(rl, fixed_array(TowerBuf.dec_state, TowerBuf.pred_w16, TowerBuf.net_rows, TowerBuf.pred_in, TowerBuf.lstm_h, TowerBuf.lstm_c), kl,
                (net.pred_dim + 7l) / 8l)
        }
        var kr = ka
        enc_rnnt_proj_cls(rl, fixed_array(TowerBuf.dec_state, TowerBuf.pred_w16, TowerBuf.net_rows, TowerBuf.lstm_h, TowerBuf.pred_out), kr, (jd + 7l) / 8l)
        enc_rnnt_joint_cls(rl, fixed_array(TowerBuf.dec_state, TowerBuf.enc_proj, TowerBuf.net_rows, TowerBuf.pred_out, TowerBuf.joint_w16, TowerBuf.logits), kr,
            rnnt_joint_blocks(n_logits) * 4l)
        enc_rnnt_pick_cls(rl, fixed_array(TowerBuf.dec_state, TowerBuf.logits, TowerBuf.net_rows, TowerBuf.pred_in, TowerBuf.picks), kr, 1l)
    }
}

//! GigaAM's CTC head over `n_frames` rows in `rows_out` (`#gigaam-planes`): the f16 or the f32 head, `nc` logits `ncp` apart into `logits`, picks into `picks`
[arch(at = "../ARCHITECTURE_GPU_TOWER_RAIL.md#fc-chains")]
def asr_dec_ctc(var rl : auto(R); f16 : bool; d, nc, ncp, n_frames : int64) {
    if (f16) {
        tower_fc_cvt16(rl, TowerBuf.rows_out, TowerBuf.xh, n_frames * d)
        rail_mm16(rl, TowerBuf.head_w, TowerBuf.xh, TowerBuf.logits, d, nc, n_frames, 0l, ncp)
    } else {
        rail_mm32(rl, TowerBuf.head_w, false, TowerBuf.rows_out, TowerBuf.logits, d, nc, n_frames, 0l, TOWER_NO_BIAS, 0l, ncp)
    }
    var pc_pick = GkCtcPickArgs(nc = uint(nc), ldl = uint(ncp), boff = 0u)
    enc_ctc_pick_cls(rl, fixed_array(TowerBuf.logits, TowerBuf.head_b, TowerBuf.picks), pc_pick, n_frames)
}
