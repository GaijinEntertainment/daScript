options gen2
options indenting = 4
options _dasllama_internal = true

//! The Metal home's tile sites: the stored-plane and conv tile ladders over explicit planes, which the home's own tower
//! driver dispatches from its slab blobs and the shared TTS driver's sites (`rail_tts_*` over `MtRail`) from the planes of
//! the home's device block.
module dasllama_metal_sites shared public

require dasllama/dasllama_lint public
require daslib/math_boost   // round_up
require metal/das_metal_boost
require dasllama/dasllama_kqformat
require dasllama/dasllama_gpu_math
require dasllama/dasllama_metal_common
require dasllama/dasllama_layout   // kq_scale_d_tail: a split scale plane's bind
require dasllama/dasllama_metal_kernels public
require dasllama/dasllama_gpu_rail   // TtsConvSite, RailBuf
require dasllama/dasllama_metal_classes_tts public   // the rail, and the shared stamps a site runs beside its tile

//! Where a stored leaf sits on the device: its quant plane and the byte its first block starts at (the halves' own plane; a
//! Q8_0 plane's 34-byte blocks, scales inside), and a K-quant member's scale plane with its scale rows' byte and - a split
//! format - its d tail's
struct MtlStored {
    halves : bool
    fmt : KqFmt
    bq : MetalBuffer?
    qoff : uint64
    bs : MetalBuffer?
    soff : uint64
    doff : uint64
}

//! the formats with no stored-plane K-quant stamp on this home: every member but Q4_K, Q5_K and Q6_K
let private MTL_ST2_SKIP = "q8 q40 q51 iq4xs k3 iq3s iq3xxs iq4nl k2 iq2s iq2xs iq2xxs mx4 iq4nl32 iq3s4 q4032 q5032 nv4"
let private MTL_ST2_CONV = "conv_mm conv_exact_mm"   //! the stored tile's stamps by tile type - f16-staged, float - of the forward conv ...
let private MTL_ST2_CONV_TR = "conv_tr_mm conv_tr_exact_mm"   //! ... and the transposed conv

//! a K-quant member's slot-0 view offset: the compact formats' scale rows again, a split format's d tail
def mtl_s0off(f : KqFmt; soff, doff : uint64) : uint64 => kq_scale_d_tail(f) == 0l ? soff : doff

//! The dense GEMM over a stored plane (`MetalSt2StoredMmT`): its form's float-tile stamp - half floats, or its format's
def mtl_stored_mm(enc : MetalComputeEncoder?; w : MtlStored; bx, by, u_kdim, u_n : MetalBuffer?; mp, d : int64; yoff : uint64 = 0ul) {
    let f = w.fmt
    let qoff = w.qoff
    let soff = w.soff
    if (w.halves) {
        enc_st2_f16_exact_mm(enc, w.bq, qoff, bx, by, yoff, u_kdim, u_n, mp, d)
        return
    }
    let s0 = mtl_s0off(f, soff, w.doff)
    if (f == KqFmt.q8) {
        enc_st2_q8_exact_mm(enc, w.bq, qoff, w.bq, bx, by, yoff, u_kdim, u_n, mp, d)
    } else {
        let fi = int(f)
        kq_tile_stamp(fi, MTL_ST2_SKIP, "st2") <| $() {
            tile_enc_exact_mm(enc, w.bs, s0, w.bs, soff, w.bq, qoff, bx, by, yoff, u_kdim, u_n, mp, d)
        }
    }
}

//! The conv GEMM over a stored plane of half floats: the stamp for the direction (`bco` the transposed stamps' channel count, null forward) and the tile type
def private mtl_f16_conv(enc : MetalComputeEncoder?; w : MtlStored; exact : bool; bx, by, u_cout : MetalBuffer?; var ka : St2ConvArgs; bco : MetalBuffer?; mp, d : int64) {
    if (bco != null) {
        if (exact) {
            enc_st2_f16_conv_tr_exact_mm(enc, w.bq, w.qoff, bx, by, 0ul, u_cout, ka, bco, mp, d)
        } else {
            enc_st2_f16_conv_tr_mm(enc, w.bq, w.qoff, bx, by, 0ul, u_cout, ka, bco, mp, d)
        }
    } elif (exact) {
        enc_st2_f16_conv_exact_mm(enc, w.bq, w.qoff, bx, by, 0ul, u_cout, ka, mp, d)
    } else {
        enc_st2_f16_conv_mm(enc, w.bq, w.qoff, bx, by, 0ul, u_cout, ka, mp, d)
    }
}

//! The conv GEMM over a stored K-quant plane: the member's stamp for the direction and the tile type, `s0` its slot-0 view offset
def private mtl_kq_conv(enc : MetalComputeEncoder?; w : MtlStored; s0 : uint64; exact, transposed : bool; bx, by, u_cout : MetalBuffer?; var ka : St2ConvArgs; bco : MetalBuffer?;   // nolint:STYLE037 - kq_tile_stamp's generated arms, a format ladder a direction
                        mp, d : int64) {
    let fi = int(w.fmt)
    let qoff = w.qoff
    let soff = w.soff
    let tile = exact ? 1 : 0
    if (transposed) {
        kq_tile_stamp(fi, MTL_ST2_SKIP, "st2", tile, MTL_ST2_CONV_TR) <| $() {
            tile_enc(enc, w.bs, s0, w.bs, soff, w.bq, qoff, bx, by, 0ul, u_cout, ka, bco, mp, d)
        }
    } else {
        kq_tile_stamp(fi, MTL_ST2_SKIP, "st2", tile, MTL_ST2_CONV) <| $() {
            tile_enc(enc, w.bs, s0, w.bs, soff, w.bq, qoff, bx, by, 0ul, u_cout, ka, mp, d)
        }
    }
}

//! The conv GEMM over a stored plane: its format's stamp for the direction - a transposed conv's over the lift - and the tile type
def mtl_stored_conv(enc : MetalComputeEncoder?; w : MtlStored; exact, transposed : bool; bx, by, u_cout : MetalBuffer?; var ka : St2ConvArgs; bco : MetalBuffer?; mp, d : int64) {
    let qoff = w.qoff
    let soff = w.soff
    if (w.halves) {
        mtl_f16_conv(enc, w, exact, bx, by, u_cout, ka, bco, mp, d)
        return
    }
    let s0 = mtl_s0off(w.fmt, soff, w.doff)
    if (w.fmt == KqFmt.q8) {
        if (transposed) {
            if (exact) {
                enc_st2_q8_conv_tr_exact_mm(enc, w.bq, qoff, w.bq, bx, by, 0ul, u_cout, ka, bco, mp, d)
            } else {
                enc_st2_q8_conv_tr_mm(enc, w.bq, qoff, w.bq, bx, by, 0ul, u_cout, ka, bco, mp, d)
            }
        } elif (exact) {
            enc_st2_q8_conv_exact_mm(enc, w.bq, qoff, w.bq, bx, by, 0ul, u_cout, ka, mp, d)
        } else {
            enc_st2_q8_conv_mm(enc, w.bq, qoff, w.bq, bx, by, 0ul, u_cout, ka, mp, d)
        }
    } else {
        mtl_kq_conv(enc, w, s0, exact, transposed, bx, by, u_cout, ka, bco, mp, d)
    }
}

//! The conv GEMM over f32 rows of a slab at byte `woff`: the stamp for the direction and the tile type
def mtl_rows_conv(enc : MetalComputeEncoder?; bslab : MetalBuffer?; woff : uint64; exact, transposed : bool; bx, by, u_cout : MetalBuffer?; var ka : St2ConvArgs; mp, d : int64) {
    if (transposed) {
        if (exact) {
            enc_st2_conv_tr_exact_mm(enc, bslab, woff, bx, by, 0ul, u_cout, ka, mp, d)
        } else {
            enc_st2_conv_tr_mm(enc, bslab, woff, bx, by, 0ul, u_cout, ka, mp, d)
        }
    } elif (exact) {
        enc_st2_conv_exact_mm(enc, bslab, woff, bx, by, 0ul, u_cout, ka, mp, d)
    } else {
        enc_st2_conv_mm(enc, bslab, woff, bx, by, 0ul, u_cout, ka, mp, d)
    }
}

//! the stored leaf of a site in the rail's planes: its quant plane at `qoff`, its scale plane at `soff` / `doff`
def private rail_stored(wq, ws : RailBuf; halves : bool; fmt : KqFmt; qoff, soff, doff : int64) : MtlStored {
    return MtlStored(halves = true, fmt = fmt, bq = rail_mtl_buf(wq), qoff = uint64(qoff * 2l)) if (halves)   // a half leaf's place is its first element
    return MtlStored(fmt = fmt, bq = rail_mtl_buf(wq), qoff = uint64(qoff), bs = rail_mtl_buf(ws), soff = uint64(soff), doff = uint64(doff))
}

[arch(at="../ARCHITECTURE_GPU_TOWER.md#tower-tts-chain")]
def rail_tts_conv(var rl : MtRail; rows, wq, ws : RailBuf; site : TtsConvSite; x, y : RailBuf) : bool {
    let mp = round_up(site.tout, 32l)
    let quant = site.stored && !site.halves
    return false if (quant && !site.native)
    let cin_step = site.stored && site.transposed ? 32l : 8l
    let shaped = site.cin % cin_step == 0l && site.cout % 64l == 0l && site.cout_p == site.cout && x.off == 0l
    let direct = y.off == 0l && y.bytes >= mp * site.cout * 4l
    if (!shaped && quant) {
        panic("dasLLAMA metal sites: a conv leaf in the home's layout off its tile's lattice (cin {site.cin}, cout {site.cout}) - the bake's own-layout rule and the tile's shape parted")
    }
    return false if (!shaped || (!direct && !quant))
    var ka = St2ConvArgs(cin = uint(site.cin), k = uint(site.k), stride = uint(site.stride), pad_l = uint(site.pad_l), dil = uint(site.dilation),
        t_in = uint(site.tin), r0 = 0u, rows = uint(site.tout), xs = uint(site.cin))
    let land = direct ? y : mtl_rail_tmp(mp * site.cout * 4l)
    var bx = rail_mtl_buf(x)
    var by = rail_mtl_buf(land)
    var u_cout = mtl_rail_u32(uint(site.cout_p))
    if (site.stored) {
        var bco = site.transposed ? mtl_rail_u32(uint(site.cout)) : null
        mtl_stored_conv(rl.enc, rail_stored(wq, ws, site.halves, site.fmt, site.qoff, site.soff, site.doff), !site.half, site.transposed, bx, by, u_cout, ka, bco, mp, site.cout_p)
    } else {
        mtl_rows_conv(rl.enc, rail_mtl_buf(rows), uint64(site.woff * 4l), !site.half, site.transposed, bx, by, u_cout, ka, mp, site.cout_p)
    }
    if (!direct) {
        var pc = GkPkRowsArgs(rows = uint(site.tout), width = uint(site.cout), ss = uint(site.cout), sc = 0u, sr0 = 0u, ds = uint(site.cout), dc = 0u, dr0 = 0u)
        enc_tts_pk_rows_cls(rl, rail_seats(rl, fixed_array(land, y)), pc, site.tout * site.cout)
    }
    var pb = GkBiasActArgs(d = uint(site.cout), nelem = uint(site.tout * site.cout), boff = uint(site.boff), act = BIAS_ACT_NONE)
    enc_tower_bias_act_cls(rl, rail_seats(rl, fixed_array(y, rows)), pb, (site.tout * site.cout / 4l + 255l) / 256l)
    return true
}

[arch(at="../ARCHITECTURE_GPU_TOWER.md#tower-tts-chain")]
def rail_tts_mm(var rl : MtRail; rows, wq, ws : RailBuf; site : TtsMmSite; x, y : RailBuf) : bool {
    let quant = site.stored && !site.halves
    return false if (quant && !site.native)
    let mp = round_up(site.rows, 32l)
    let k = site.kdim_p
    let d = site.cout_p
    let xin = site.xoff == 0l && x.off == 0l && site.nin == k && x.bytes >= mp * k * 4l
    let yout = site.yoff == 0l && y.off == 0l && site.nout == d && y.bytes >= mp * d * 4l
    return false if (!quant && (!xin || !yout || site.nin != k))
    if (site.nin > k || site.nout > d) {
        panic("dasLLAMA metal sites: a dense layer [{site.nout}][{site.nin}] past its leaf's [{d}][{k}]")
    }
    var xt = x
    if (!xin) {   // the rows re-laid from element `xoff` at the weights' stride; the columns past nin and the rows past the call's are the tile's to read as they lie
        xt = mtl_rail_tmp(mp * k * 4l, 0)
        if (site.nin < k) {
            rail_zero(rl, rail_seats(rl, fixed_array(xt))[0], 0l, site.rows * k * 4l)
        }
        var pc = GkPkRowsArgs(rows = uint(site.rows), width = uint(site.nin), ss = uint(site.nin), sc = uint(site.xoff), sr0 = 0u, ds = uint(k), dc = 0u, dr0 = 0u)
        enc_tts_pk_rows_cls(rl, rail_seats(rl, fixed_array(x, xt)), pc, site.rows * site.nin)
    }
    let yt = yout ? y : mtl_rail_tmp(mp * d * 4l, 1)
    var u_k = mtl_rail_u32(uint(k))
    var u_n = mtl_rail_u32(uint(d))
    if (site.stored) {
        mtl_stored_mm(rl.enc, rail_stored(wq, ws, site.halves, site.fmt, site.qoff, site.soff, site.doff), rail_mtl_buf(xt), rail_mtl_buf(yt), u_k, u_n, mp, d)
    } else {
        enc_f32_exact_mm(rl.enc, rail_mtl_buf(rows), uint64(site.woff * 4l), rail_mtl_buf(xt), rail_mtl_buf(yt), 0ul, u_k, u_n, mp, d)
    }
    if (!yout) {
        var pc = GkPkRowsArgs(rows = uint(site.rows), width = uint(site.nout), ss = uint(d), sc = 0u, sr0 = 0u, ds = uint(site.nout), dc = uint(site.yoff), dr0 = 0u)
        enc_tts_pk_rows_cls(rl, rail_seats(rl, fixed_array(yt, y)), pc, site.rows * site.nout)
    }
    if (site.biased) {
        var pb = GkBiasActArgs(d = uint(site.nout), nelem = uint(site.rows * site.nout), boff = uint(site.boff), act = BIAS_ACT_NONE, xoff = uint(site.yoff))
        enc_tower_bias_act_cls(rl, rail_seats(rl, fixed_array(y, rows)), pb, (site.rows * site.nout / 4l + 255l) / 256l)
    }
    return true
}

//! the row GEMV's own-layout stamps built: every kind over Q8_0 and over Q4_K
def private mtl_pk_own_ensure : bool {
    return (ensure_q8_tts_pkown_dot_cls() && ensure_q8_tts_pkown_ln_silu_cls() && ensure_q8_tts_pkown_gate_cls() && ensure_q8_tts_pkown_add_silu_cls() && ensure_q8_tts_pkown_tail_cls() && ensure_q8_tts_pkown_ln_gelu_cls() && ensure_q8_tts_pkown_add_scale_cls() && ensure_q8_tts_pkown_add_cls() && ensure_q8_tts_pkown_ln_qkv_cls()
        && ensure_kq_tts_pkown_k4_dot_cls() && ensure_kq_tts_pkown_k4_ln_silu_cls() && ensure_kq_tts_pkown_k4_gate_cls() && ensure_kq_tts_pkown_k4_add_silu_cls() && ensure_kq_tts_pkown_k4_tail_cls() && ensure_kq_tts_pkown_k4_ln_gelu_cls() && ensure_kq_tts_pkown_k4_add_scale_cls() && ensure_kq_tts_pkown_k4_add_cls() && ensure_kq_tts_pkown_k4_ln_qkv_cls())
}

//! the formats the row GEMV reads no own-layout leaf of: every member but Q8_0 and Q4_K
let private MTL_PKOWN_SKIP = "k5 k6 q40 q51 iq4xs k3 iq3s iq3xxs iq4nl k2 iq2s iq2xs iq2xxs mx4 iq4nl32 iq3s4 q4032 q5032 nv4"

//! a stored row GEMV over a leaf in this home's layout: its own stamp of the kind, over Q8_0 or Q4_K rows (`MtPkGemvStoredT`)
def rail_tts_pk_own(var rl : MtRail; s : TtsPkStSite; args : GkPkGemvArgs; nout : int64) : bool {   // nolint:STYLE037 - kq_tile_stamp's generated arms, a format ladder a kind
    var pa = args
    let fmt = s.fmt
    if (s.kind == TTS_PK_GEMV_DOT) {
        let seats = rail_seats(rl, fixed_array(s.rows, s.x, s.y, s.wq, s.ws))
        kq_tile_stamp(fmt, MTL_PKOWN_SKIP, "tts_pkown") <| $() {
            tile_enc_dot_cls(rl, seats, pa, nout)
        }
    } elif (s.kind == TTS_PK_GEMV_LN_SILU) {
        let seats = rail_seats(rl, fixed_array(s.rows, s.x, s.y, s.mod, s.wq, s.ws))
        kq_tile_stamp(fmt, MTL_PKOWN_SKIP, "tts_pkown") <| $() {
            tile_enc_ln_silu_cls(rl, seats, pa, nout)
        }
    } elif (s.kind == TTS_PK_GEMV_GATE) {
        let seats = rail_seats(rl, fixed_array(s.rows, s.x, s.y, s.mod, s.wq, s.ws))
        kq_tile_stamp(fmt, MTL_PKOWN_SKIP, "tts_pkown") <| $() {
            tile_enc_gate_cls(rl, seats, pa, nout)
        }
    } elif (s.kind == TTS_PK_GEMV_ADD_SILU) {
        let seats = rail_seats(rl, fixed_array(s.rows, s.x, s.y, s.wq, s.ws))
        kq_tile_stamp(fmt, MTL_PKOWN_SKIP, "tts_pkown") <| $() {
            tile_enc_add_silu_cls(rl, seats, pa, nout)
        }
    } elif (s.kind == TTS_PK_GEMV_TAIL) {
        let seats = rail_seats(rl, fixed_array(s.rows, s.x, s.y, s.mod, s.x0, s.lat, s.wq, s.ws))
        kq_tile_stamp(fmt, MTL_PKOWN_SKIP, "tts_pkown") <| $() {
            tile_enc_tail_cls(rl, seats, pa, nout)
        }
    } elif (s.kind == TTS_PK_GEMV_LN_GELU) {
        let seats = rail_seats(rl, fixed_array(s.rows, s.x, s.y, s.wq, s.ws))
        kq_tile_stamp(fmt, MTL_PKOWN_SKIP, "tts_pkown") <| $() {
            tile_enc_ln_gelu_cls(rl, seats, pa, nout)
        }
    } elif (s.kind == TTS_PK_GEMV_ADD_SCALE) {
        let seats = rail_seats(rl, fixed_array(s.rows, s.x, s.y, s.wq, s.ws))
        kq_tile_stamp(fmt, MTL_PKOWN_SKIP, "tts_pkown") <| $() {
            tile_enc_add_scale_cls(rl, seats, pa, nout)
        }
    } elif (s.kind == TTS_PK_GEMV_ADD) {
        let seats = rail_seats(rl, fixed_array(s.rows, s.x, s.y, s.wq, s.ws))
        kq_tile_stamp(fmt, MTL_PKOWN_SKIP, "tts_pkown") <| $() {
            tile_enc_add_cls(rl, seats, pa, nout)
        }
    } elif (s.kind == TTS_PK_GEMV_LN_QKV) {
        let seats = rail_seats(rl, fixed_array(s.rows, s.x, s.y, s.tabs, s.kc, s.vc, s.wq, s.ws))
        kq_tile_stamp(fmt, MTL_PKOWN_SKIP, "tts_pkown") <| $() {
            tile_enc_ln_qkv_cls(rl, seats, pa, nout)
        }
    } else {
        panic("dasLLAMA TTS: a stored row GEMV of kind {s.kind} has no stamp")
    }
    return true
}

[unused_argument(rl)]
def rail_tts_sites_ensure(rl : MtRail) : bool {
    //! the classes the conv and dense sites dispatch beside their tiles - the bias row, the row copy, the zero fill - built: a
    //! seat's ensure chain asks, so a site never meets a stamp the device refused
    return ensure_tower_bias_act_cls() && ensure_tts_pk_rows_cls() && ensure_rail_zero_cls() && mtl_pk_own_ensure()
}

let private MTL_COLSTATS_MAX_T = 1024l   //! the rows the one-pass statistics take: a threadgroup walks a channel's whole column, so past it the shared block passes win

def rail_tts_colstats(var rl : MtRail; x, stats : RailBuf; t, c : int64) : bool {
    //! The column statistics of x [t][c] in one pass - a threadgroup a channel - into the two stats rows the shared passes write:
    //! the means, then the squared deviations' sums; false past `MTL_COLSTATS_MAX_T` rows
    return false if (t > MTL_COLSTATS_MAX_T || x.off != 0l || stats.off != 0l || stats.bytes < 2l * c * 4l)
    enc_st2_colstats(rl.enc, rail_mtl_buf(x), rail_mtl_buf(stats), mtl_rail_u32(uint(t)), mtl_rail_u32(uint(c)), c)
    return true
}

def rail_tts_lstm_q8(var rl : MtRail; g, wq, y : RailBuf; qoff, t, hidden : int64; backward : bool) : bool {
    //! One LSTM direction over its recurrence's Q8_0 leaf in the home's layout, at byte `qoff` of `wq`: the gates [t][4H] of g,
    //! the h rows into y's half
    return false if (g.off != 0l || y.off != 0l || hidden % 32l != 0l)
    var ka = St2LstmArgs(t = uint(t), hidden = uint(hidden), xgs = uint(4l * hidden), ystride = uint(2l * hidden), yoff = backward ? uint(hidden) : 0u,
        reverse = backward ? 1u : 0u)
    var bq = rail_mtl_buf(wq)
    enc_st2_lstm_q8(rl.enc, rail_mtl_buf(g), rail_mtl_buf(y), bq, uint64(qoff), ka, bq, 4l * hidden)
    return true
}
