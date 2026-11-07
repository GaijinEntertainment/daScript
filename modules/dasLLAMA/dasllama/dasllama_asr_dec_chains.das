options gen2
options indenting = 4
options _dasllama_internal = true

//! The whisper decoder's dispatch sequences, written once for both discrete homes (`ARCHITECTURE_GPU_TOWER_RAIL.md#whisper-decoder`):
//! the per-window cross K / V over the encoder rows and the decode step - the row passes with their Q8_0 feeds, the q8 GEMV sites,
//! the K / V stores, the chunked attention and the tied-embedding logits - generic over the rail (`VtRail`, `CutRail`); and the
//! home-free shape both decoder seats read: the decline reasons, the window identity, the geometry, the buffer sizes, the q8 regions
//! a bake gathers, the f32 rows a seat uploads and the schedule's layout.
module dasllama_asr_dec_chains shared public

require dasllama/dasllama_lint public
require math
require daslib/math_boost   // round_up
require dasllama/dasllama_kqformat
require dasllama/dasllama_tower
require dasllama/dasllama_plane
require dasllama/dasllama_gpu_math
require dasllama/dasllama_gpu_kernels_common
require dasllama/dasllama_gpu_kernels_discrete
require dasllama/dasllama_whisper
require dasllama/dasllama_tower_rail
require ?cuda dasllama/dasllama_cuda_classes_asr   // the decoder's stamps on the CUDA rail

let WDEC_ROW_CAP = 8l          //! the rows a decode batch may carry: the N-column GEMV's widest form
let WDEC_LAYER_SITES = 6l      //! a layer's GEMV records: qkv (three regions), o, cq, co, fc1, fc2
let private WDEC_S_QKV = 0l
let private WDEC_S_O = 1l
let private WDEC_S_CQ = 2l
let private WDEC_S_CO = 3l
let private WDEC_S_FC1 = 4l
let private WDEC_S_FC2 = 5l

//! why a discrete home's whisper decoder leaves a window or a step to the CPU chain; the decline counters key on the value names
enum WdecDecline {
    knob         //! the home's decoder knob is off
    quant_mode   //! the decoder is the f32 oracle rail, not the q8 image the driver reads
    shape        //! a width off the tile's lattice, or a head width other than the family's 64
    device       //! the tier has no device, or a class failed to build
    memory       //! a device or pinned allocation failed
    flavor       //! the model is not the home's flavored image: each backend serves its own (`#tower-dev-block`)
    align        //! the device's storage-offset alignment does not divide a GEMV record's (`RAIL_META_ALIGN`)
}

//! a decoder decline in words: `home` the home's name in its image flavor, `knob`, `device` and `memory` its own spelling of those reasons
[cold_path]
def wdec_decline_words(why : WdecDecline; home, knob, device, memory : string) : string {
    if (why == WdecDecline.knob) {
        return knob
    } elif (why == WdecDecline.quant_mode) {
        return "the decoder's planes are the f32 rail, not the q8 image the driver serves"
    } elif (why == WdecDecline.shape) {
        return "a width or head size the stamps do not serve"
    } elif (why == WdecDecline.memory) {
        return memory
    } elif (why == WdecDecline.flavor) {
        return "the model is not the {home}-flavored image"
    } elif (why == WdecDecline.align) {
        return "the device's storage-offset alignment does not divide the {RAIL_META_ALIGN}-byte step of the GEMV records' schedule views"
    }
    return device
}

//! the decoder's say-once line where a window's encoder rows came by host upload: `home` the home's log word (`vulkan`, `cuda`); `said` set
[cold_path]
def wdec_miss_say(var said : bool&; home : string) {
    said = true
    to_log(LOG_INFO, "dasLLAMA {home} wdec: the encoder rows arrive by host upload - the tower landed no device plane for this window (the encoder ran on the CPU, or declined)\n")
}

//! the decoder's serves line, said once a resident: `home` the home's log word; `said` set
[cold_path]
def wdec_announce(var said : bool&; home : string; g : WdecGeom) {
    said = true
    to_log(LOG_INFO, "dasLLAMA {home} wdec: the whisper decoder serves on the device (d {g.d}, {g.nl} layers)\n")
}

//! a window's gate ahead of any device work, both homes': false with `decline` told why - knob `on` off, not the q8 image, not `home`'s flavor, a width off the lattice
def wdec_gate(w : WhisperModel; on : bool; home : TowerDev; decline : block<(why : WdecDecline) : bool>) : bool {
    return invoke(decline, WdecDecline.knob) if (!on)
    return invoke(decline, WdecDecline.quant_mode) if (!w.dq8)
    return invoke(decline, WdecDecline.flavor) if (w.enc.dev_backend != home)
    let d = w.n_text_state
    return invoke(decline, WdecDecline.shape) if (d % 64l != 0l || d / w.n_text_head != int64(WDEC_HS))
    return true
}

//! a decode batch of `n` rows admitted on `home`'s live window (`live`, claimed by state `window_uid`): false where the window
//! is another state's; a window evicted mid-decode or a batch past the step cap panics (`flavor` the image's word: `Vulkan`, `CUDA`)
def wdec_step_admits(ds : DecoderState; live : bool; window_uid : uint64; n : int64; home, flavor : string) : bool {
    if (!live || window_uid != ds.gpu_uid) {
        if (ds.gpu_kv_gen != 0l && ds.n_past > 0l) {
            panic("dasLLAMA {home} wdec: the device decoder window was evicted mid-window - ONE live decoder window per process (a second DecoderState may not interleave whisper_cross_kv/whisper_decode_batch)")
        }
        return false
    }
    static_assert(WHISPER_DEV_CHUNK <= WDEC_ROW_CAP, "the decode's device chunk fits the N-column GEMV")
    if (n > WDEC_ROW_CAP) {
        panic("dasLLAMA {home} wdec: decode batch of {n} exceeds the {WDEC_ROW_CAP}-row step cap - a {flavor}-flavored model's decode hands the seat {WHISPER_DEV_CHUNK} rows at most")
    }
    return true
}

var private g_wdec_uid_next = 1ul

//! a decoder state's identity, minted at its first window: an address is not identity - a state re-created at a freed address inherits no window
def wdec_uid(var ds : DecoderState) : uint64 {
    if (ds.gpu_uid == 0ul) {
        ds.gpu_uid = g_wdec_uid_next++
    }
    return ds.gpu_uid
}

//! a decoder's geometry and its planes' layout, the same on both homes: the widths, the window, the layers' f32 rows uploaded from
//! the layers' first row (`rows_n`) with the final norm's rows behind them at plane offsets `lnf_w` / `lnf_b`, and the schedule -
//! the cross K / V records from word 0, the GEMV records' region pairs from byte `gemv_off`, `RAIL_META_ALIGN` apart, the first at
//! record id `gemv0`
struct WdecGeom {
    d : int64
    hh : int64
    hs : int64
    ta : int64
    tmax : int64
    nl : int64
    nvocab : int64
    rows_n : int64
    lnf_w : int64
    lnf_b : int64
    gemv0 : int64
    gemv_off : int64
    meta_bytes : int64
}

//! the decoder geometry of `w` with its cross K / V records' `groups` (the home's tile over the encoder rows)
def wdec_geom(w : WhisperModel; groups : int64) : WdecGeom {
    let nl = w.n_text_layer
    let d = w.n_text_state
    let rows_n = nl * w.dlayer_stride
    let cross_bytes = (2l * nl * 4l + 2l * nl * groups) * 4l
    let gemv_off = round_up(cross_bytes, RAIL_META_ALIGN)
    return WdecGeom(d = d, hh = w.n_text_head, hs = d / w.n_text_head, ta = w.enc.n_ctx, tmax = w.n_text_ctx, nl = nl, nvocab = w.n_vocab, rows_n = rows_n,
        lnf_w = rows_n, lnf_b = rows_n + d, gemv0 = 2l * nl, gemv_off = gemv_off, meta_bytes = gemv_off + (nl * WDEC_LAYER_SITES + 1l) * RAIL_META_ALIGN)
}

//! the decoder's buffer bytes: row-cap planes (`bx`, Q8_0 `bq` / `bs`, FFN `bh`), self K / V `bkc`, cross K / V halves `bkx`, encoder rows `bt`, partials `bpms` / `bpy`
def wdec_sizes(g : WdecGeom) : tuple<bx : int64; bq : int64; bs : int64; bh : int64; bkc : int64; bkx : int64; bt : int64; bpms : int64; bpy : int64> {
    let d = g.d
    let bq = WDEC_ROW_CAP * 4l * d
    let nch_cap = (max(g.ta, g.tmax) + int64(WDEC_CHUNK) - 1l) / int64(WDEC_CHUNK)
    return (bx = WDEC_ROW_CAP * d * 4l, bq = bq, bs = bq / kq_elems(KqFmt.q8) * 4l, bh = WDEC_ROW_CAP * 4l * d * 4l,
        bkc = g.nl * d * g.tmax * 2l, bkx = g.nl * d * g.ta * 2l, bt = g.ta * d * 4l,
        bpms = nch_cap * g.hh * WDEC_ROW_CAP * 2l * 4l, bpy = nch_cap * g.hh * WDEC_ROW_CAP * g.hs * 4l)
}

//! the whisper decoder's q8 regions a bake gathers: the token table the logits read, then each layer's eight attention GEMMs and its FFN pair
def wdec_regions(w : WhisperModel) : array<Q8Region> {
    let d = w.n_text_state
    let nl = w.n_text_layer
    var regions : array<Q8Region>
    regions |> reserve(nl * 10l + 1l)
    regions |> push(Q8Region(woff = w.d_te_q_off, n = d, rows = w.n_vocab))
    for (l in range64(nl)) {
        let lo = dec_layer_offsets(w, l)
        for (woff in fixed_array(lo.q_w, lo.k_w, lo.v_w, lo.o_w, lo.cq_w, lo.ck_w, lo.cv_w, lo.co_w)) {
            regions |> push(Q8Region(woff = woff, n = d, rows = d))
        }
        regions |> push(Q8Region(woff = lo.fc1_w, n = d, rows = 4l * d))
        regions |> push(Q8Region(woff = lo.fc2_w, n = 4l * d, rows = d))
    }
    return <- regions
}

//! the decoder's f32 plane as both homes upload it: the layers' rows from the first layer's, then the final norm's weight and bias
def wdec_norm_rows(w : WhisperModel; g : WdecGeom) : array<float> {
    var rows : array<float>
    rows |> reserve(g.rows_n + 2l * g.d)
    for (i in range64(g.rows_n)) {
        rows |> push(w.dblob[w.dlayers_off + i])
    }
    for (i in range64(g.d)) {
        rows |> push(w.dblob[w.d_ln_w_off + i])
    }
    for (i in range64(g.d)) {
        rows |> push(w.dblob[w.d_ln_b_off + i])
    }
    return <- rows
}

//! the cross K / V site's bind - the encoder rows' f16 feed and Q8_0 feed - and the GEMV site's, its records from `gemv_off`
def wdec_binds(var rl : auto(R); g : WdecGeom) {
    rail_binds_reset(rl)
    rail_bind_feeds(rl, RailSite.cross, TowerBuf.enc_h, TowerBuf.enc_q, TowerBuf.enc_s)
    rl.binds[int(RailSite.gemv)].meta_off = g.gemv_off
}

//! the GEMV record of layer `l`'s site `k`
def private wdec_gemv_rid(g : WdecGeom; l, k : int64) : int64 => g.gemv0 + l * WDEC_LAYER_SITES + k

//! the tied-embedding logits' GEMV record
def private wdec_logits_rid(g : WdecGeom) : int64 => g.gemv0 + g.nl * WDEC_LAYER_SITES

//! the schedule's word a step rewrites: the logits record's first region's activation block (the batch's last row)
def wdec_logits_word(g : WdecGeom) : int64 => (g.gemv_off + (wdec_logits_rid(g) - g.gemv0) * RAIL_META_ALIGN) / 4l + 1l

//! the activation block of the batch's last row, `n` rows in: the logits read that row alone
def wdec_logits_row(g : WdecGeom; n : int64) : uint => uint((n - 1l) * (g.d / kq_elems(KqFmt.q8)))

//! the decoder's schedule at `mp` (`g.meta_bytes`): the cross records two a layer at the `cross` pick, each GEMV record's region pairs, every map into `rl.maps`
[arch(at = "../ARCHITECTURE_GPU_TOWER_RAIL.md#whisper-decoder")]
def wdec_meta(var rl : auto(R); w : WhisperModel; g : WdecGeom; var mp : uint?) {
    let q8e = kq_elems(KqFmt.q8)
    let rows = rail_site_rows(rl, RailSite.cross, g.ta)
    let groups = rail_site_groups(rl, RailSite.cross, g.d, g.ta)
    tower_sched_walk(rl.maps, mp, 2l * g.nl) $(rid : int64) : TowerSchedRec {
        let lo = dec_layer_offsets(w, rid / 2l)
        return TowerSchedRec(wblk0 = (rid % 2l == 0l ? lo.ck_w : lo.cv_w) / q8e, rows = rows, groups = groups)
    }
    let nrec = g.nl * WDEC_LAYER_SITES + 1l
    rl.maps |> reserve(g.gemv0 + nrec)
    rl.maps |> resize(g.gemv0 + nrec)
    let w0 = g.gemv_off / 4l
    let wrec = RAIL_META_ALIGN / 4l
    unsafe {
        for (k in range64(nrec)) {
            rl.maps[g.gemv0 + k] = uint(k * wrec)
            for (j in range64(RAIL_GEMV_META_BYTES / 4l)) {
                mp[w0 + k * wrec + j] = 0u
            }
        }
        for (l in range64(g.nl)) {
            let lo = dec_layer_offsets(w, l)
            let regions = fixed_array(fixed_array(lo.q_w, lo.k_w, lo.v_w), fixed_array(lo.o_w, -1l, -1l), fixed_array(lo.cq_w, -1l, -1l),
                fixed_array(lo.co_w, -1l, -1l), fixed_array(lo.fc1_w, -1l, -1l), fixed_array(lo.fc2_w, -1l, -1l))
            for (k in range64(WDEC_LAYER_SITES)) {
                let at = w0 + (l * WDEC_LAYER_SITES + k) * wrec
                for (i in range64(3l)) {
                    let woff = regions[k][i]
                    mp[at + i * 2l] = woff >= 0l ? uint(woff / q8e) : 0u
                }
            }
        }
        mp[wdec_logits_word(g) - 1l] = uint(w.d_te_q_off / q8e)
    }
}

//! the cross K / V of one window over `rows_out` (taken off the tower's `xb` under `handoff`): records `2 l` / `2 l + 1` into `kxr` / `vxr`
[arch(at = "../ARCHITECTURE_GPU_TOWER_RAIL.md#whisper-decoder")]
def asr_dec_whisper_cross_kv(var rl : auto(R); w : WhisperModel; g : WdecGeom; handoff : bool) {
    let d = g.d
    let ta = g.ta
    if (handoff) {
        rail_copy(rl, TowerBuf.xb, 0l, TowerBuf.rows_out, 0l, ta * d * 4l)
        rail_pt(rl, RailProfRole.copy_out)
    }
    rail_site_feed(rl, RailSite.cross, TowerBuf.rows_out, ta * d, -NO_CLAMP, NO_CLAMP)
    rail_pt(rl, RailProfRole.rq)
    let wg_kv = (ta * g.hh * g.hs + 255l) / 256l
    var pc16 = GkWdecKvArgs(rows = uint(ta), sstride = uint(d), soff = 0u, nheads = uint(g.hh), tstride = uint(ta), p0 = 0u, dst_off = 0u, boff = 0u,
        bias_on = 0u, mode = WDEC_KV_F16, hs = uint(g.hs), kscale = wdec_kscale(g.hs))
    for (l in range64(g.nl)) {
        let lo = dec_layer_offsets(w, l)
        pc16.dst_off = uint(l * d * ta)
        rail_mm_kq(rl, RailSite.cross, 2l * l, TowerBuf.enc_proj, d, d, ta)
        rail_pt(rl, RailProfRole.gemm_k)
        pc16.boff = 0u
        pc16.bias_on = 0u
        enc_tower_wdec_kv16_cls(rl, fixed_array(TowerBuf.enc_proj, TowerBuf.kxr, TowerBuf.norms), pc16, wg_kv)
        rail_pt(rl, RailProfRole.kv_store)
        rail_mm_kq(rl, RailSite.cross, 2l * l + 1l, TowerBuf.enc_proj, d, d, ta)
        rail_pt(rl, RailProfRole.gemm_v)
        pc16.boff = uint(lo.cv_b - rl.norms_base)
        pc16.bias_on = 1u
        enc_tower_wdec_kv16_cls(rl, fixed_array(TowerBuf.enc_proj, TowerBuf.vxr, TowerBuf.norms), pc16, wg_kv)
        rail_pt(rl, RailProfRole.kv_store)
    }
}

//! one decoder attention pass: the chunk partials over the K / V planes at `kvoff` (query bias at model offset `qb`), then the combine with its Q8_0 requant
def private wdec_attn(var rl : auto(R); q, kk, vv : TowerBuf; var pc : GkWdecAttnArgs; var pc_comb : GkWdecCombArgs; kvoff, qb, wg_part, wg_comb : int64) {
    pc.koff = uint(kvoff)
    pc.voff = uint(kvoff)
    pc.qboff = uint(qb - rl.norms_base)
    enc_tower_wdec_attn_part_cls(rl, fixed_array(q, kk, vv, TowerBuf.pms, TowerBuf.py, TowerBuf.norms), pc, wg_part)
    enc_tower_wdec_attn_comb_rq_cls(rl, fixed_array(TowerBuf.pms, TowerBuf.py, TowerBuf.xq, TowerBuf.xs), pc_comb, wg_comb)
    rail_pt(rl, RailProfRole.attn)
}

//! a GEMV record's site over `n` rows of `n_in` into `y`, then the seam adding its bias at `b` and norming the next input at `lnoff` / `lnboff`
def private wdec_gemv_post(var rl : auto(R); g : WdecGeom; rid, n_in, d_out, n : int64; role : RailProfRole; var pc_post : GkPostAddLnArgs; b, lnoff, lnboff : int64) {
    rail_gemv_q8(rl, RailSite.gemv, rid, TowerBuf.yo, n_in, d_out, 1l, n)
    rail_pt(rl, role)
    pc_post.boff = uint(b - rl.norms_base)
    pc_post.lnoff = uint(lnoff)
    pc_post.lnboff = uint(lnboff)
    enc_tower_post_add_ln_rq_cls(rl, fixed_array(TowerBuf.x, TowerBuf.yo, TowerBuf.norms, TowerBuf.xq, TowerBuf.xs), pc_post, n)
    rail_pt(rl, RailProfRole.post_add)
}

//! one decode batch of `n` embedded rows in `x` from `p0`: per layer self, cross and FFN on the GEMV sites, then the tied-embedding logits of the last row
[arch(at = "../ARCHITECTURE_GPU_TOWER_RAIL.md#whisper-decoder")]
def asr_dec_whisper_step(var rl : auto(R); w : WhisperModel; g : WdecGeom; n, p0 : int64) {
    let nb = rl.norms_base
    let q8e = kq_elems(KqFmt.q8)
    let d = g.d
    let hh = g.hh
    let hs = g.hs
    let nff = n * 4l * d
    let wg_nff = rq_wgs(false, nff)
    let wg_kv = (n * hh * hs + 255l) / 256l
    let nch_s = (p0 + n + int64(WDEC_CHUNK) - 1l) / int64(WDEC_CHUNK)
    let nch_c = (g.ta + int64(WDEC_CHUNK) - 1l) / int64(WDEC_CHUNK)
    var pc_kv = GkWdecKvArgs(rows = uint(n), sstride = uint(3l * d), soff = uint(d), nheads = uint(hh), tstride = uint(g.tmax), p0 = uint(p0),
        dst_off = 0u, boff = 0u, bias_on = 0u, mode = WDEC_KV_F16, hs = uint(hs), kscale = wdec_kscale(hs))
    let wd_scale = 1.0 / sqrt(float(WDEC_HS))
    var pc_as = GkWdecAttnArgs(dim = uint(3l * d), tstride = uint(g.tmax), len0 = uint(p0 + 1l), causal = 1u, nheads = uint(hh),
        nrows = uint(n), koff = 0u, voff = 0u, qboff = 0u, hs = WDEC_HS, scale = wd_scale)
    var pc_ac = GkWdecAttnArgs(dim = uint(d), tstride = uint(g.ta), len0 = uint(g.ta), causal = 0u, nheads = uint(hh),
        nrows = uint(n), koff = 0u, voff = 0u, qboff = 0u, hs = WDEC_HS, scale = wd_scale)
    var pc_cs = GkWdecCombArgs(dim = uint(d), nchunks = uint(nch_s), nheads = uint(hh), nrows = uint(n), hs = WDEC_HS)
    var pc_cc = pc_cs
    pc_cc.nchunks = uint(nch_c)
    var pc_gelu = TowerBiasRqArgs(d = uint(4l * d), nblk = uint(nff / q8e), boff = 0u)
    var pc_post = GkPostAddLnArgs(d = uint(d), rows = uint(n), eps = w.eps, ln_on = 1u, ascale = 1.0, xboff = 0u)
    let wg_attn_s = nch_s * hh * n
    let wg_attn_c = nch_c * hh * n
    let wg_comb = hh * n
    for (l in range64(g.nl)) {
        let lo = dec_layer_offsets(w, l)
        if (l == 0l) {
            var pc = gk_ln_args(uint(d), uint(n), w.eps, uint(lo.ln1_w - nb), uint(lo.ln1_b - nb))
            enc_tower_ln_rq_cls(rl, fixed_array(TowerBuf.x, TowerBuf.norms, TowerBuf.xq, TowerBuf.xs), pc, n)
            rail_pt(rl, RailProfRole.ln)
        }
        rail_gemv_q8(rl, RailSite.gemv, wdec_gemv_rid(g, l, WDEC_S_QKV), TowerBuf.qkv, d, d, 3l, n)
        rail_pt(rl, RailProfRole.gemm_q)
        pc_kv.soff = uint(d)
        pc_kv.dst_off = uint(l * d * g.tmax)
        pc_kv.bias_on = 0u
        enc_tower_wdec_kv16_cls(rl, fixed_array(TowerBuf.qkv, TowerBuf.kc, TowerBuf.norms), pc_kv, wg_kv)
        pc_kv.soff = uint(2l * d)
        pc_kv.bias_on = 1u
        pc_kv.boff = uint(lo.v_b - nb)
        enc_tower_wdec_kv16_cls(rl, fixed_array(TowerBuf.qkv, TowerBuf.vc, TowerBuf.norms), pc_kv, wg_kv)
        rail_pt(rl, RailProfRole.kv_store)
        wdec_attn(rl, TowerBuf.qkv, TowerBuf.kc, TowerBuf.vc, pc_as, pc_cs, l * d * g.tmax, lo.q_b, wg_attn_s, wg_comb)
        wdec_gemv_post(rl, g, wdec_gemv_rid(g, l, WDEC_S_O), d, d, n, RailProfRole.gemm_o, pc_post, lo.o_b, lo.cln_w - nb, lo.cln_b - nb)
        rail_gemv_q8(rl, RailSite.gemv, wdec_gemv_rid(g, l, WDEC_S_CQ), TowerBuf.cq, d, d, 1l, n)
        rail_pt(rl, RailProfRole.gemm_q)
        wdec_attn(rl, TowerBuf.cq, TowerBuf.kxr, TowerBuf.vxr, pc_ac, pc_cc, l * d * g.ta, lo.cq_b, wg_attn_c, wg_comb)
        wdec_gemv_post(rl, g, wdec_gemv_rid(g, l, WDEC_S_CO), d, d, n, RailProfRole.gemm_o, pc_post, lo.co_b, lo.ln2_w - nb, lo.ln2_b - nb)
        rail_gemv_q8(rl, RailSite.gemv, wdec_gemv_rid(g, l, WDEC_S_FC1), TowerBuf.hg, d, 4l * d, 1l, n)
        rail_pt(rl, RailProfRole.gemm_fc1)
        pc_gelu.boff = uint(lo.fc1_b - nb)
        enc_tower_bias_act_rq_cls(rl, fixed_array(TowerBuf.hg, TowerBuf.norms, TowerBuf.xq, TowerBuf.xs), pc_gelu, wg_nff)
        rail_pt(rl, RailProfRole.bias)
        let last = l + 1l == g.nl
        let nx = last ? lo : dec_layer_offsets(w, l + 1l)
        wdec_gemv_post(rl, g, wdec_gemv_rid(g, l, WDEC_S_FC2), 4l * d, d, n, RailProfRole.gemm_fc2, pc_post, lo.fc2_b, last ? g.lnf_w : nx.ln1_w - nb,
            last ? g.lnf_b : nx.ln1_b - nb)
    }
    rail_gemv_q8(rl, RailSite.gemv, wdec_logits_rid(g), TowerBuf.logits, d, g.nvocab, 1l, 1l)
    rail_pt(rl, RailProfRole.logits)
}
