options gen2
options _comment_hygiene = true
options _dasllama_internal = true
options persistent_heap   // a dastest root frees what it deletes only on this heap; the linear heap holds every carrier a cell deleted until the context ends

module test_vulkan_dec_tail

require dastest/testing_boost public
require dasllama/dasllama_math
require dasllama/dasllama_math_default   // dot_mx4q8_scalar: the routed mx4 cell's reference dot
require dasllama/dasllama_gpu_math
require dasllama/dasllama_rope
require dasllama/dasllama_common   // MoeGate, select_decode_override
require dasllama/dasllama_blocks   // registers the vulkan_moe_span decode override at init
require dasllama/dasllama_moe   // moe_select_core: the host select the span's device top-k mirrors
require ?vulkan dasllama/dasllama_math_vulkan  // the decode attention block + FFN tail under test; the test installs the hooks explicitly (no env gate)
require ?vulkan dasllama/dasllama_vulkan_decode   // vk_span_reset: the span records once per model, so the second span cell forgets the first's record
require math
require _vk_kq_fixtures
require _kq_fixtures   // q8_roundtrip: the feeds and requants the device quantizes, through the engine's quantizers
require _compares
require _kernel_oracles
require _attn_oracle   // attn_row_oracle: the block reference's softmax rows

//! The per-op tier's decode attention block and the decode FFN tail against a CPU reference:
//! two attention quads (layers A and B), one expert stack pair, two positions, in both acts
//! forms (K-quant quads feed Q8_K, q8 quads feed Q8_0). Token flow per position: block(A) ->
//! host residual -> tail(FFN A + combine + B's feed on the device) -> block(B) collects the
//! residual row. Weights are synthetic stacks (the fixtures).

let private N = 256l        // dim (one Q8_K superblock)
let private HS = 64l
let private NH = 4l
let private QD = NH * HS
let private KVD = HS        // one kv head
let private KVM = NH
let private NFE = 256l
let private NE = 4l         // experts in the stacks; the tail routes 0 and 1, the span's top-k picks
let private K = 2l
let private EPS = 1.0e-6
let private ATTN_SCALE = 0.125   // 1 / sqrt(HS)
let private STRIDE = 1_000_000l  // plane offsets, far apart
let private ABS_BAR = 2.0e-3     // the reference mirrors every device rounding: rows agree to ~1e-4 of their max
let private REL_BAR = 1.0e-3
//! a row's bar: within REL_BAR of the reference row's max, floored at ABS_BAR; the control moves element 0 by 1 + 2 x that max
let private ROW_BAR = Bar(kind = BarKind.rel_of_max, rel = REL_BAR, abs_floor = ABS_BAR, poison = 2.0, poison_of_max = true, poison_abs = 1.0, poison_at = 0)

def private fill_row(var x : array<float>; n : int64; phase : float) {
    x |> resize(n)
    for (i in range64(n)) {
        x[i] = 0.5 * sin(float(i) * 0.37 + phase) + 0.05 * cos(float(i) * 1.3)
    }
}

def private fill_norm(var w : array<float>; n : int64; phase : float) {
    w |> resize(n)
    for (i in range64(n)) {
        w[i] = 1.0 + 0.1 * sin(float(i) * 0.11 + phase)
    }
}

// y = W x over `rows` rows of width n (row-major dequantized plane)
def private gemv_ref(w : array<float>; rows, n : int64; x : array<float>; var y : array<float>) {
    var r <- dense_ref_rows(w, x, 1, int(n), int(rows))
    delete y
    y <- r.want
    delete r
}

//! the device's feed: rms-normed, then quantized in the plane's acts form (Q8_0 for q8 planes, Q8_K for K-quant ones)
def private feed_ref(x : array<float>; wn : array<float>; fmt : int) : Q8Round {
    var xb : array<float>
    xb |> resize(N)
    rmsnorm(xb, x, wn, N, EPS)
    return <- q8_roundtrip(xb, N, fmt != 0)
}

// one head group's decode attention over the f16 mirror rows [0, cnt)
def private attn_ref(q : array<float>; kmir, vmir : array<float>; cnt : int64; var y : array<float>) {
    y |> resize(QD)
    attn_heads_oracle(q, 0, kmir, 0, vmir, 0, int(KVD), int(NH), int(HS), int(KVM), int(cnt), ATTN_SCALE, y, 0)
}

//! `got` against a non-trivial reference row at ROW_BAR
def private held_row(t : T?; tag : string; got, want : array<float>) {
    t |> success(logits_maxabs(want) > 0.0, "{tag}: the reference is non-trivial")
    check(t, tag, got, want, ROW_BAR, FLAT_SHAPE)
}

struct private Quad {
    fmt : int           // the quad's plane format (0 = q8, 1 = k4): the acts form follows it
    neox : bool         // the rope pairing the quad's chain runs
    woq : int64
    wok : int64
    wov : int64
    woo : int64
    wq : array<float>   // dequantized planes for the reference
    wk : array<float>
    wv : array<float>
    wo : array<float>
    norm : array<float> // the layer's attention rms weight row
    bq : array<float>   // the q/k/v projection bias rows (empty = the model has none)
    bk : array<float>
    bv : array<float>
    kmir : array<float> // the reference mirror (f16-rounded rows at absolute positions)
    vmir : array<float>
}

//! a bias row's pointer for the block hook: null when the quad carries no bias
def private bias_ptr(b : array<float>) : float const? => !empty(b) ? unsafe(addr<float const?>(b[0])) : null

// the expert stack pair (dequantized for the reference), the FFN norm row, the routing
struct private Ffn {
    w1 : array<float>
    w3 : array<float>
    w2 : array<float>
    norm : array<float>
    offs1 : array<int64>
    offs3 : array<int64>
    offs2 : array<int64>
    rw : array<float>
}

//! one attention quad's four planes, uploaded and dequantized; false = no device, hasb = the quad carries bias rows
def private quad_up(var q : Quad; fmt : int; base : int64; seed : int64; phase : float; neox, hasb : bool) : bool {
    q.fmt = fmt
    q.neox = neox
    if (hasb) {
        q.bq <- [for (i in range64(QD)); 0.05 * sin(float(i) * 0.37 + phase)]
        q.bk <- [for (i in range64(KVD)); 0.05 * cos(float(i) * 0.29 + phase)]
        q.bv <- [for (i in range64(KVD)); 0.04 * sin(float(i) * 0.53 - phase)]
    }
    q.woq = base
    q.wok = base + STRIDE
    q.wov = base + 2l * STRIDE
    q.woo = base + 3l * STRIDE
    if (!plane_up(fmt, QD, N, seed, q.woq, q.wq)) {
        return false
    }
    verify(plane_up(fmt, KVD, N, seed + 1l, q.wok, q.wk), "k plane upload")
    verify(plane_up(fmt, KVD, N, seed + 2l, q.wov, q.wv), "v plane upload")
    verify(plane_up(fmt, N, QD, seed + 3l, q.woo, q.wo), "o plane upload")
    fill_norm(q.norm, N, phase)
    q.kmir |> resize(4l * KVD)
    q.vmir |> resize(4l * KVD)
    return true
}

// the expert stacks: gate/up k4 over N, down k6 over NFE; expert e's plane starts at e * NFE * N;
// slot j routes expert j
def private ffn_up(t : T?; var f : Ffn; base_slot : int64 = 30l) : bool {
    let woff1 = base_slot * STRIDE
    let woff3 = (base_slot + 1l) * STRIDE
    let woff2 = (base_slot + 2l) * STRIDE
    if (!plane_up(1, NE * NFE, N, 41l, woff1, f.w1)) {
        return false
    }
    t |> success(plane_up(1, NE * NFE, N, 43l, woff3, f.w3), "dec tail: up stack uploaded")
    t |> success(plane_up(3, NE * N, NFE, 47l, woff2, f.w2, false), "dec tail: down stack uploaded")
    fill_norm(f.norm, N, 1.1)
    let ege = NFE * N
    f.offs1 <- [woff1, 0l, 1l, woff1 + ege, 1l, 1l]
    f.offs3 <- [woff3, 0l, 1l, woff3 + ege, 1l, 1l]
    f.offs2 <- [woff2, 0l, 1l, woff2 + ege, 1l, 1l]
    f.rw <- [0.6, 0.4]
    return true
}

//! a shared expert's q8 triple (dequantized for the reference), its plane offsets and the two
//! layers' gate rows; width NFE like the routed experts
struct private Shexp {
    w1 : array<float>
    w3 : array<float>
    w2 : array<float>
    off1 : int64
    off3 : int64
    off2 : int64
    gate : array<float>   //! 2 x N, layer-major
}

def private shexp_up(t : T?; var sh : Shexp; base : int64) : bool {
    sh.off1 = base
    sh.off3 = base + STRIDE
    sh.off2 = base + 2l * STRIDE
    if (!plane_up(0, NFE, N, 71l, sh.off1, sh.w1)) {
        return false
    }
    t |> success(plane_up(0, NFE, N, 73l, sh.off3, sh.w3), "dec span shexp: the shared up plane uploaded")
    t |> success(plane_up(0, N, NFE, 79l, sh.off2, sh.w2), "dec span shexp: the shared down plane uploaded")
    sh.gate |> resize(2l * N)
    for (i in range64(2l * N)) {
        sh.gate[i] = 0.03 * sin(float(i) * 0.47 + 0.9) + 0.02 * cos(float(i / N) * 1.7)
    }
    return true
}

// the block's reference for one position: the feed, q/k/v, rope, the mirror row, attention, wo
def private block_ref(var q : Quad; x : array<float>; pos : int64; cossin : array<float>; var xb2 : array<float>; var kraw : array<float>) {
    let feed <- feed_ref(x, q.norm, q.fmt)
    var qr, kr, vr : array<float>
    gemv_ref(q.wq, QD, N, feed.deq, qr)
    gemv_ref(q.wk, KVD, N, feed.deq, kr)
    gemv_ref(q.wv, KVD, N, feed.deq, vr)
    kraw := kr
    if (!empty(q.bq)) {
        for (i in range64(QD)) {
            qr[i] += q.bq[i]
        }
        for (i in range64(KVD)) {
            kr[i] += q.bk[i]
            vr[i] += q.bv[i]
        }
    }
    rope_tab_ref(qr, 0, int(HS), 0, int(NH * HS), q.neox, cossin, cossin, 0, int(HS / 2l))
    rope_tab_ref(kr, 0, int(HS), 0, int(HS), q.neox, cossin, cossin, 0, int(HS / 2l))
    for (i in range64(KVD)) {
        q.kmir[pos * KVD + i] = f16r(kr[i])
        q.vmir[pos * KVD + i] = f16r(vr[i])
    }
    var y : array<float>
    attn_ref(qr, q.kmir, q.vmir, pos + 1l, y)
    // the device requantizes the attention row in the wo plane's form before the wo GEMV
    let yr <- q8_roundtrip(y, QD, q.fmt != 0)
    gemv_ref(q.wo, N, QD, yr.deq, xb2)
}

// the decode FFN's gathered activation image: the one quantized row, k times
def private k_copies(gxq1 : array<int8>; gxs1 : array<float>; var gxq : array<int8>; var gxs : array<float>) {
    let nb = N / 256l
    gxq |> resize(K * N)
    gxs |> resize(K * nb)
    for (j in range64(K)) {
        for (i in range64(N)) {
            gxq[j * N + i] = gxq1[i]
        }
        for (i in range64(nb)) {
            gxs[j * nb + i] = gxs1[i]
        }
    }
}

// one position through both layers: block(A) from host acts, the host residual, the tail, block(B)
def private token_cell(t : T?; var qa : Quad; var qb : Quad; f : Ffn; pos : int64; tag : string) {
    var no_freqs, cossin : array<float>
    cossin |> resize(HS)
    build_rope_row_packed(cossin, 0l, pos, HS, 10000.0, 1.0, 1.0, no_freqs)
    var x : array<float>
    fill_row(x, N, 0.2 + 0.9 * float(pos))
    var xb2, kout, vout : array<float>
    xb2 |> resize(N)
    kout |> resize(KVD)
    vout |> resize(KVD)
    var feed <- feed_ref(x, qa.norm, qa.fmt)
    matmul_moe_gpu_attn_dec(xb2, kout, vout, qa.woq, qa.wok, qa.wov, qa.woo, qa.fmt, qa.fmt, qa.fmt, qa.fmt, feed.xq, feed.xs, cossin, null, null,
        bias_ptr(qa.bq), bias_ptr(qa.bk), bias_ptr(qa.bv), pos, N, QD, KVD, HS, NH, KVM, ATTN_SCALE, EPS, qa.neox, false)
    var xb2_ref, kraw_ref : array<float>
    block_ref(qa, x, pos, cossin, xb2_ref, kraw_ref)
    held_row(t, "{tag} pos {pos} layer A wo row", xb2, xb2_ref)
    held_row(t, "{tag} pos {pos} layer A raw k row", kout, kraw_ref)
    // the host residual, then the tail: FFN A + combine + layer B's feed on the device
    for (i in range64(N)) {
        x[i] += xb2[i]
    }
    var x1_ref := x
    let ffeed <- feed_ref(x, f.norm, 1)
    var gxq : array<int8>
    var gxs : array<float>
    k_copies(ffeed.xq, ffeed.xs, gxq, gxs)
    matmul_moe_gpu_ffn_tail(f.offs1, f.offs3, f.offs2, K, gxq, gxs, N, NFE, K, 1, 1, 3, false, f.rw, x, qb.woq, qb.fmt,
        unsafe(addr<float const?>(qb.norm[0])), EPS)
    t |> success(moe_gpu_ffn_tail_pending(), "{tag} pos {pos}: the tail is pending until layer B's attention collects it")
    let pair <- [0l, 1l]   // the tail arm routes slot j to expert j
    ffn_ref_sel(f, ffeed.deq, pair, f.rw, x1_ref)
    // layer B on the block: its feed came from the tail; the host acts are ignored
    matmul_moe_gpu_attn_dec(xb2, kout, vout, qb.woq, qb.wok, qb.wov, qb.woo, qb.fmt, qb.fmt, qb.fmt, qb.fmt, feed.xq, feed.xs, cossin, null, null,
        bias_ptr(qb.bq), bias_ptr(qb.bk), bias_ptr(qb.bv), pos, N, QD, KVD, HS, NH, KVM, ATTN_SCALE, EPS, qb.neox, false)
    t |> success(!moe_gpu_ffn_tail_pending(), "{tag} pos {pos}: layer B's attention collected the tail")
    held_row(t, "{tag} pos {pos} residual row after FFN A (the tail's combine)", x, x1_ref)
    var xb2b_ref, krawb_ref : array<float>
    block_ref(qb, x, pos, cossin, xb2b_ref, krawb_ref)
    held_row(t, "{tag} pos {pos} layer B wo row (fed by the tail)", xb2, xb2b_ref)
    held_row(t, "{tag} pos {pos} layer B raw k row", kout, krawb_ref)
}

//! a fresh claim with both layers hydrated empty and serving position 0 after; with `unseen` (two layers the block has not
//! seen) each asks for its hydrate at position 0 first (zero rows create it), the second still asking after the first one's; the claim
def private claim_hydrated(t : T?; qa, qb : Quad; tag : string; unseen : bool = true) : int64 {
    let gen = moe_gpu_attn_dec_claim(true)
    t |> success(!unseen || moe_gpu_attn_dec_ok(qa.woq, qa.fmt, 0l, gen) == DAT_HYDRATE, "{tag}: an unseen layer asks for a hydrate at position 0")
    matmul_moe_gpu_attn_dec_hydrate(qa.woq, qa.fmt, null, null, 0l, KVD, true)
    t |> success(!unseen || moe_gpu_attn_dec_ok(qb.woq, qb.fmt, 0l, gen) == DAT_HYDRATE, "{tag}: the second layer is still unseen after the first one's hydrate")
    matmul_moe_gpu_attn_dec_hydrate(qb.woq, qb.fmt, null, null, 0l, KVD, true)
    t |> success(moe_gpu_attn_dec_ok(qa.woq, qa.fmt, 0l, gen) == DAT_SERVE && moe_gpu_attn_dec_ok(qb.woq, qb.fmt, 0l, gen) == DAT_SERVE,
        "{tag}: both layers serve position 0 after the empty hydrates")
    return gen
}

//! two layers whose q planes sit at ONE offset in two format planes (a mixed-format file's shape): the block keys a layer by
//! its offset under its format, so each layer serves its own quad and mirror; false = no device
def private collide_cell(t : T?; f : Ffn; base : int64; tag : string) : bool {
    var qa, qb : Quad
    if (!quad_up(qa, 1, base, 51l, 0.4, true, false)) {
        return false
    }
    t |> success(quad_up(qb, 0, base, 63l, 0.8, true, false), "{tag}: the q8 quad uploaded at the k4 quad's offset")
    two_positions(t, qa, qb, f, claim_hydrated(t, qa, qb, tag), tag)
    return true
}

//! positions 0 and 1 through both layers, then both mirrors holding [0, 2) under claim `gen`
def private two_positions(t : T?; var qa : Quad; var qb : Quad; f : Ffn; gen : int64; tag : string) {
    for (pos in range64(2)) {
        token_cell(t, qa, qb, f, pos, tag)
    }
    t |> success(moe_gpu_attn_dec_ok(qa.woq, qa.fmt, 2l, gen) == DAT_SERVE && moe_gpu_attn_dec_ok(qb.woq, qb.fmt, 2l, gen) == DAT_SERVE,
        "{tag}: both mirrors hold [0, 2) after two positions")
}

// one quad pair in one plane format through two positions; false = no device
def private pair_cell(t : T?; f : Ffn; fmt : int; base : int64; tag : string; neox, hasb : bool) : bool {
    var qa, qb : Quad
    if (!quad_up(qa, fmt, base, 21l + int64(fmt), 0.3, neox, hasb)) {
        return false
    }
    t |> success(quad_up(qb, fmt, base + 10l * STRIDE, 33l + int64(fmt), 0.7, neox, hasb), "{tag}: layer B's quad uploaded")
    two_positions(t, qa, qb, f, claim_hydrated(t, qa, qb, tag), tag)
    // the hydrate arms: a fresh claim empties both mirrors; A refills from f32 rows, B from f16 rows
    let gen2 = moe_gpu_attn_dec_claim(true)
    t |> success(moe_gpu_attn_dec_ok(qa.woq, qa.fmt, 2l, gen2) == DAT_HYDRATE, "{tag}: a fresh claim empties the mirrors - position 2 asks for a hydrate")
    var kb, vb : array<uint8>
    kb |> resize(2l * KVD * 2l)
    vb |> resize(2l * KVD * 2l)
    for (i in range64(2l * KVD)) {
        put_f16(kb, i * 2l, qb.kmir[i])
        put_f16(vb, i * 2l, qb.vmir[i])
    }
    unsafe {
        matmul_moe_gpu_attn_dec_hydrate(qa.woq, qa.fmt, addr<uint8 const?>(qa.kmir[0]), addr<uint8 const?>(qa.vmir[0]), 2l, KVD, false)
        matmul_moe_gpu_attn_dec_hydrate(qb.woq, qb.fmt, addr<uint8 const?>(kb[0]), addr<uint8 const?>(vb[0]), 2l, KVD, true)
    }
    t |> success(moe_gpu_attn_dec_ok(qa.woq, qa.fmt, 2l, gen2) == DAT_SERVE && moe_gpu_attn_dec_ok(qb.woq, qb.fmt, 2l, gen2) == DAT_SERVE,
        "{tag}: both layers serve position 2 after the hydrates (f32 rows converted, f16 rows copied)")
    token_cell(t, qa, qb, f, 2l, tag)
    return true
}

// the reference routed FFN over a host-selected slot list (the span's device router + top-k
// picks) onto the residual row - ffn_ref over (idx, w) instead of the fixed pair
//! expert `e`'s FFN row over the feed `fd`, its hidden row through the engine's requant (Q8_K under `qk`, else Q8_0)
def private expert_ffn_ref(w1, w3, w2, fd : array<float>; e : int64; qk : bool) : array<float> {
    var d <- ffn_expert_ref(w1, w3, w2, fd, 1, int(N), int(NFE), int(e * NFE), int(e * NFE), int(e * N)) $(var h) {
        q8_round_deq(h, qk)
    }
    return <- d
}

def private ffn_ref_sel(f : Ffn; fd : array<float>; idx : array<int64>; w : array<float>; var x1 : array<float>) {
    var acc : array<float>
    acc |> resize(N)
    for (j in range64(K)) {
        var inscope d <- expert_ffn_ref(f.w1, f.w3, f.w2, fd, idx[j], true)
        for (i in range64(N)) {
            acc[i] += w[j] * d[i]
        }
    }
    for (i in range64(N)) {
        x1[i] += acc[i]
    }
}

//! a span layer's route: the FFN-normed row (f32) and the host select's slots and weights (softmax, top-k, renorm)
struct private SpanRoute {
    xb : array<float>
    idx : array<int64>
    w : array<float>
}

//! one span layer's block and residual onto `x`, then its route off the router's logits
def private span_route(var q : Quad; f : Ffn; router : array<float>; li : int64; var x : array<float>; pos : int64; cossin : array<float>; var kraw : array<float>) : SpanRoute {
    var xb2 : array<float>
    block_ref(q, x, pos, cossin, xb2, kraw)
    for (i in range64(N)) {
        x[i] += xb2[i]
    }
    var r : SpanRoute
    r.xb |> resize(N)
    rmsnorm(r.xb, x, f.norm, N, EPS)
    var lg : array<float>
    lg |> resize(NE)
    for (e in range64(NE)) {
        var acc = 0.0
        for (i in range64(N)) {
            acc += router[(li * NE + e) * N + i] * r.xb[i]
        }
        lg[e] = acc
    }
    r.idx |> resize(K)
    r.w |> resize(K)
    unsafe {
        moe_select_core(MoeGate.softmax, NE, K, true, 1.0, addr(lg[0]), addr(r.idx[0]), addr(r.w[0]), 0l)
    }
    return <- r
}

//! one span layer's reference: the block, the residual, the route, the routed FFN on the requantized feed
def private span_layer_ref(var q : Quad; f : Ffn; router : array<float>; li : int64; var x : array<float>; pos : int64; cossin : array<float>; var kraw : array<float>) {
    let r <- span_route(q, f, router, li, x, pos, cossin, kraw)
    let fd <- feed_ref(x, f.norm, 1)
    ffn_ref_sel(f, fd.deq, r.idx, r.w, x)
}

//! the span's planes: router rows (2 layers x NE x N), the [A | B] attention norms, the FFN norm twice
struct private SpanPlanes {
    router : array<float>
    att_norms : array<float>
    ffn_norms : array<float>
}

def private span_planes(qa, qb : Quad; f : Ffn) : SpanPlanes {
    var sp : SpanPlanes
    sp.router |> resize(2l * NE * N)
    for (i in range64(2l * NE * N)) {
        sp.router[i] = 0.02 * sin(float(i) * 0.61 + 0.3) + 0.01 * cos(float(i / N) * 2.1)
    }
    sp.att_norms |> resize(2l * N)
    sp.ffn_norms |> resize(2l * N)
    for (i in range64(N)) {
        sp.att_norms[i] = qa.norm[i]
        sp.att_norms[N + i] = qb.norm[i]
        sp.ffn_norms[i] = f.norm[i]
        sp.ffn_norms[N + i] = f.norm[i]
    }
    return <- sp
}

//! A span position's rows: the device residual row after both layers, and its input, which the caller's reference advances.
struct private SpanRows {
    x : array<float>
    x_ref : array<float>
}

//! one position through the device span: the rope row at `pos`, the patterned input row, layer A's feed, the per-token pointers,
//! the dispatch; the raw k rows land in `kout`
def private span_position(var a : MoeSpanDec; qa : Quad; sp : SpanPlanes; var cossin : array<float>; pos : int64; var kout, vout : array<float>) : SpanRows {
    var no_freqs : array<float>
    build_rope_row_packed(cossin, 0l, pos, HS, 10000.0, 1.0, 1.0, no_freqs)
    var rows : SpanRows
    fill_row(rows.x, N, 0.5 + 0.7 * float(pos))
    rows.x_ref := rows.x
    var feed <- feed_ref(rows.x, qa.norm, 1)
    unsafe {
        a.pos = pos
        a.router = addr<float const?>(sp.router[0])
        a.att_norm = addr<float const?>(sp.att_norms[0])
        a.ffn_norm = addr<float const?>(sp.ffn_norms[0])
        a.xq = addr<int8 const?>(feed.xq[0])
        a.xs = addr<float const?>(feed.xs[0])
        a.cossin = addr<float const?>(cossin[0])
        a.x = addr(rows.x[0])
        a.kout = addr(kout[0])
        a.vout = addr(vout[0])
    }
    matmul_moe_gpu_span_dec(a)
    return <- rows
}

// the span's per-model arguments over the two quads and the shared expert pair (both layers
// route through the same stacks; the per-token pointers are filled per position)
def private span_args(qa, qb : Quad; f : Ffn) : MoeSpanDec {
    var a <- MoeSpanDec(dim = N, qd = QD, kvd = KVD, hs = HS, n_heads = NH, kv_mul = KVM, ne = NE, k = K, nfe = NFE,
        scale = ATTN_SCALE, eps = EPS, wscale = 1.0, neox = true, qk_norm = false, norm_topk = true, is_gelu = false,
        woq <- [qa.woq, qb.woq], wok <- [qa.wok, qb.wok], wov <- [qa.wov, qb.wov], woo <- [qa.woo, qb.woo],
        fq <- [1, 1], fk <- [1, 1], fv <- [1, 1], fo <- [1, 1],
        we1 <- [f.offs1[0], f.offs1[0]], we3 <- [f.offs3[0], f.offs3[0]], we2 <- [f.offs2[0], f.offs2[0]],
        f1 <- [1, 1], f3 <- [1, 1], f2 <- [3, 3])
    a.rmsq |> resize(2)   // no qk-norm: null rows
    a.rmsk |> resize(2)
    a.bq |> resize(2)   // no projection bias: null rows
    a.bk |> resize(2)
    a.bv |> resize(2)
    return <- a
}

//! the span's arguments with a shared expert beside the routed pair (both layers share its triple), gated or not
def private span_args_sh(qa, qb : Quad; f : Ffn; sh : Shexp; gated : bool) : MoeSpanDec {
    var a <- span_args(qa, qb, f)
    a.nsh = NFE
    a.sh_gated = gated
    a.wsh1 <- [sh.off1, sh.off1]
    a.wsh3 <- [sh.off3, sh.off3]
    a.wsh2 <- [sh.off2, sh.off2]
    let q8 = int(KqFmt.q8)   //! the q8 triple on both layers
    a.fsh1 <- [q8, q8]
    a.fsh3 <- [q8, q8]
    a.fsh2 <- [q8, q8]
    return <- a
}

//! one span layer's reference with a shared expert: span_layer_ref's steps, the shared gate on the
//! same f32 normed row, then the shared FFN on the Q8_0 feed (its hidden row requantized Q8_0 as
//! the q8 down GEMV reads it) scaled by sigmoid(gate) - or by one, ungated - onto the residual
def private span_layer_ref_sh(var q : Quad; f : Ffn; sh : Shexp; router : array<float>; li : int64; var x : array<float>; pos : int64; cossin : array<float>; var kraw : array<float>; gated : bool) {
    let r <- span_route(q, f, router, li, x, pos, cossin, kraw)
    var gl = 0.0
    for (i in range64(N)) {
        gl += sh.gate[li * N + i] * r.xb[i]
    }
    let g = gated ? sigmoid_f32(gl) : 1.0
    let fd <- feed_ref(x, f.norm, 1)
    let fd0 <- feed_ref(x, f.norm, 0)
    ffn_ref_sel(f, fd.deq, r.idx, r.w, x)
    let d <- expert_ffn_ref(sh.w1, sh.w3, sh.w2, fd0.deq, 0l, false)
    for (i in range64(N)) {
        x[i] += g * d[i]
    }
}

[test]
def test_vulkan_dec_span_cache(t : T?) {
    t |> run("dec span: the cached arguments follow the carrier, not only the offsets") @(t : T?) {
        // two carriers with the same layout - the same first attention offset - own different rows
        var ma = Model()
        var mb = Model()
        ma.wq_offs <- [7l, 8l]
        mb.wq_offs <- [7l, 8l]
        ma.fblob |> resize(4)
        mb.fblob |> resize(4)
        var a <- MoeSpanDec(woq <- [7l, 8l])
        a.blob = unsafe(addr<float const?>(ma.fblob[0]))
        t |> success(span_args_current(a, ma, 0l, 2l), "the carrier that filled the cache is current")
        t |> success(!span_args_current(a, mb, 0l, 2l), "a same-layout carrier is NOT current - its rows live elsewhere")
        t |> success(!span_args_current(a, ma, 1l, 1l), "another suffix is not current")
        t |> success(!span_args_current(a, ma, 0l, 1l), "another suffix length is not current")
        delete ma
        delete mb
    }
}

[test]
def test_vulkan_dec_span(t : T?) {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        install_vk_moe_hooks()
        var f : Ffn
        if (!ffn_up(t, f)) {
            t |> skip("dec span: no Vulkan device (or probe/budget refused the upload)")
            return
        }
        var qa, qb : Quad
        t |> success(quad_up(qa, 1, 60l * STRIDE, 51l, 0.4, true, false), "dec span: layer A's quad uploaded")
        t |> success(quad_up(qb, 1, 70l * STRIDE, 63l, 0.8, true, false), "dec span: layer B's quad uploaded")
        let gen = claim_hydrated(t, qa, qb, "dec span")
        var sp <- span_planes(qa, qb, f)
        var a <- span_args(qa, qb, f)
        var kout, vout : array<float>
        kout |> resize(2l * KVD)
        vout |> resize(2l * KVD)
        var cossin : array<float>
        cossin |> resize(HS)
        for (pos in range64(2)) {
            var rows <- span_position(a, qa, sp, cossin, pos, kout, vout)
            var inscope kraw_a, kraw_b : array<float>
            span_layer_ref(qa, f, sp.router, 0l, rows.x_ref, pos, cossin, kraw_a)
            span_layer_ref(qb, f, sp.router, 1l, rows.x_ref, pos, cossin, kraw_b)
            held_row(t, "span pos {pos} layer A raw k row", copy_span(kout, 0l, KVD), kraw_a)
            held_row(t, "span pos {pos} layer B raw k row", copy_span(kout, KVD, KVD), kraw_b)
            held_row(t, "span pos {pos} residual row after both layers", rows.x, rows.x_ref)
        }
        t |> success(moe_gpu_attn_dec_ok(qa.woq, qa.fmt, 2l, gen) == DAT_SERVE && moe_gpu_attn_dec_ok(qb.woq, qb.fmt, 2l, gen) == DAT_SERVE,
            "dec span: both mirrors hold [0, 2) after two positions")
        // the override reaches the engine through its registry: the init registers, the MoE placement
        // selects by name (dastest runs no [init], so the test registers through the same entry)
        dasllama_blocks_span_register()
        t |> success(select_decode_override("vulkan_moe_span"), "dec span: the vulkan_moe_span decode override is registered")
        select_decode_override("")
    } else {
        t |> skip("dasVulkan not installed; the decode span is untestable here")
    }
}

//! the span with a gated shared expert: the routed slots plus sigmoid(gate) times the shared down
//! row in one combine, the shared feed off the same normed row, one submit per position
[test]
def test_vulkan_dec_span_shexp(t : T?) {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        vk_span_reset()   //! the span records once per model: this cell is the second model in the process
        install_vk_moe_hooks()
        //! bases past every other cell's (the tail pairs sit at 10-23, 40-53 and 80-93 x STRIDE, the
        //! routed stacks at 30-32, the plain span's quads at 60-73): first-hit stack lookup
        var f : Ffn
        if (!ffn_up(t, f, 100l)) {
            t |> skip("dec span shexp: no Vulkan device (or probe/budget refused the upload)")
            return
        }
        var sh : Shexp
        t |> success(shexp_up(t, sh, 103l * STRIDE), "dec span shexp: the shared gate plane uploaded")
        var qa, qb : Quad
        t |> success(quad_up(qa, 1, 110l * STRIDE, 51l, 0.4, true, false), "dec span shexp: layer A's quad uploaded")
        t |> success(quad_up(qb, 1, 120l * STRIDE, 63l, 0.8, true, false), "dec span shexp: layer B's quad uploaded")
        var sp <- span_planes(qa, qb, f)
        var cossin : array<float>
        cossin |> resize(HS)
        for (gated in fixed_array(true, false)) {
            vk_span_reset()   //! the record keys on the shared expert's gate: the ungated arm is a new span
            let tag = gated ? "gated" : "ungated"
            claim_hydrated(t, qa, qb, "dec span shexp ({tag})", false)
            var a <- span_args_sh(qa, qb, f, sh, gated)
            var inscope kout, vout : array<float>
            kout |> resize(2l * KVD)
            vout |> resize(2l * KVD)
            a.shgate = unsafe(addr<float const?>(sh.gate[0]))
            for (pos in range64(2)) {
                var rows <- span_position(a, qa, sp, cossin, pos, kout, vout)
                var inscope kraw_a, kraw_b : array<float>
                span_layer_ref_sh(qa, f, sh, sp.router, 0l, rows.x_ref, pos, cossin, kraw_a, gated)
                span_layer_ref_sh(qb, f, sh, sp.router, 1l, rows.x_ref, pos, cossin, kraw_b, gated)
                held_row(t, "span shexp ({tag}) pos {pos} residual row after both layers", rows.x, rows.x_ref)
                if (pos == 0l) {
                    var inscope x_no : array<float>
                    fill_row(x_no, N, 0.5)
                    var inscope ka, kb : array<float>
                    span_layer_ref(qa, f, sp.router, 0l, x_no, 0l, cossin, ka)
                    span_layer_ref(qb, f, sp.router, 1l, x_no, 0l, cossin, kb)
                    t |> success(check_count(rows.x, x_no, ROW_BAR) > 0, "span shexp ({tag}): the reference without the shared expert misses the device row")
                }
            }
        }
    } else {
        t |> skip("dasVulkan not installed; the decode span is untestable here")
    }
}

//! the shared expert's three dense GEMMs over the rows' images: the gate's Q8_0 image, the up's Q8_K one, the gated hidden
//! row requantized to Q8_K for the k6 down - each plane's reference over the image it reads
def private shexp_mixed_ref(w1, w3, w2 : array<float>; x : array<float>; rows : int64) : array<float> {
    let g8 <- q8_roundtrip(x, rows * N, false)
    let uk <- q8_roundtrip(x, rows * N, true)
    var g <- dense_ref_rows(w1, g8.deq, int(rows), int(N), int(NFE))
    var u <- dense_ref_rows(w3, uk.deq, int(rows), int(N), int(NFE))
    gate_batch(g.want, u.want, FfnAct.silu, NFE, rows)
    let hk <- q8_roundtrip(g.want, rows * NFE, true)
    var d <- dense_ref_rows(w2, hk.deq, int(rows), int(NFE), int(N))
    var y <- d.want
    delete g
    delete u
    delete d
    return <- y
}

//! a shared expert whose gate and up take two activation forms - a q8 gate fed Q8_0 beside a k4 up fed Q8_K, a k6 down - through
//! `shexp_gpu_gemms`: every plane read in its own format over its own image, at one row (a decode step) and at three (a prefill);
//! the k4 up fed the gate's Q8_0 image - one image for both - misses its row (the control that the two images are load-bearing)
[test]
def test_vulkan_shexp_mixed_forms(t : T?) {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        install_vk_moe_hooks()
        let o1 = 170l * STRIDE   //! slots 170-172: past every plane the file's other cells upload
        let o3 = 171l * STRIDE
        let o2 = 172l * STRIDE
        var w1, w3, w2 : array<float>
        if (!plane_up(int(KqFmt.q8), NFE, N, 131l, o1, w1)) {
            t |> skip("shexp mixed forms: no Vulkan device (or probe/budget refused the upload)")
            return
        }
        t |> success(plane_up(int(KqFmt.k4), NFE, N, 137l, o3, w3), "shexp mixed forms: the k4 up plane uploaded")
        t |> success(plane_up(int(KqFmt.k6), N, NFE, 139l, o2, w2, false), "shexp mixed forms: the k6 down plane uploaded")
        set_moe_gpu_shexp_layer(0l, ShexpGpuPlanes(f1 = int(KqFmt.q8), f3 = int(KqFmt.k4), f2 = int(KqFmt.k6), o1 = o1, o3 = o3, o2 = o2))
        set_moe_gpu_shexp_layer(1l, ShexpGpuPlanes(f1 = int(KqFmt.k4), f3 = int(KqFmt.k4), f2 = int(KqFmt.k6), o1 = o3, o3 = o3, o2 = o2))
        t |> success(shexp_gpu_mixed_forms(0l), "shexp mixed forms: a q8 gate beside a k4 up takes two activation forms")
        t |> success(!shexp_gpu_mixed_forms(1l), "shexp mixed forms: a k4 gate beside a k4 up takes one (the predicate's control)")
        var m = Model()
        m.config.dim = N
        m.config.n_ff_shexp = NFE
        m.config.ffn_act = FfnAct.silu
        for (rows in fixed_array(1l, 3l)) {
            var inscope x : array<float>
            fill_row(x, rows * N, 0.3 + float(rows))
            var inscope y : array<float>
            let before = shexp_gpu_gemm_rows()
            shexp_gpu_gemms(m, 0l, rows, x, y)
            t |> equal(shexp_gpu_gemm_rows() - before, rows, "shexp mixed forms ({rows} rows): the dense GEMMs served every row")
            var inscope want <- shexp_mixed_ref(w1, w3, w2, x, rows)
            held_row(t, "shexp mixed forms ({rows} rows): the shared expert's rows", copy_span(y, 0l, rows * N), want)
            let g8 <- q8_roundtrip(x, rows * N, false)
            let uk <- q8_roundtrip(x, rows * N, true)
            var inscope u_own, u_one : array<float>
            u_own |> resize(rows * NFE)
            u_one |> resize(rows * NFE)
            matmul_gpu_dense(u_own, o3, uk.xq, uk.xs, N, NFE, rows, int(KqFmt.k4))
            matmul_gpu_dense(u_one, o3, g8.xq, g8.xs, N, NFE, rows, int(KqFmt.k4))
            var inscope u_ref <- dense_ref_rows(w3, uk.deq, int(rows), int(N), int(NFE))
            held_row(t, "shexp mixed forms ({rows} rows): the k4 up over its own Q8_K image", u_own, u_ref.want)
            t |> success(check_count(u_one, u_ref.want, ROW_BAR) > 0, "shexp mixed forms ({rows} rows): the k4 up fed the gate's Q8_0 image misses its row")
        }
        delete m
    } else {
        t |> skip("dasVulkan not installed; the shared expert's dense GEMMs are untestable here")
    }
}

//! a q51 plane of `rows` rows of `n` on the upload wire (20-byte blocks, an f16 d | m pair a block) at `woff`, its bytes kept for
//! the reference; false = the upload was refused
def private q51_up(rows, n, seed, woff : int64; var wq : array<uint8>; var ws : array<uint8>) : bool {
    let nb = rows * (n / 32l)
    wq |> resize(nb * int64(KQ_Q51_QWORDS) * 4l)
    ws |> resize(nb * 4l)
    var st = uint64(seed) * 2654435761ul + 12345ul
    for (c in wq) {
        c = lcg_byte(st)
    }
    for (b in range64(nb)) {
        put_f16(ws, b * 4l, 0.004 * float(1l + b % 7l))
        put_f16(ws, b * 4l + 2l, 0.002 * float(b % 5l) - 0.004)
    }
    return moe_gpu_upload_stack(wq, ws, woff, n, rows, int(KqFmt.q51), false)
}

//! the q51 plane's rows over a Q8_0 image: the engine's scalar dot, a row at a time
def private q51_rows_ref(wq, ws : array<uint8>; xq : array<int8>; xs : array<float>; rows, n, nrows : int64) : array<float> {
    let nb = n / 32l
    var y : array<float>
    y |> resize(nrows * rows)
    for (r in range64(nrows)) {
        for (j in range64(rows)) {
            unsafe {
                y[r * rows + j] = dot_q51q8_scalar(addr(wq[j * nb * int64(KQ_Q51_QWORDS) * 4l]), addr(ws[j * nb * 4l]),
                    addr(xq[r * n]), addr(xs[r * nb]), n)
            }
        }
    }
    return <- y
}

//! a dense FFN triple of two activation forms - a q51 gate fed Q8_0 beside a k4 up fed Q8_K, a k6 down - through `ffn_gpu_gemms`,
//! the per-op FFN rail's form for such a layer: the q51 gate on the per-32 batch tile, every plane over its own image, at one row
//! and at three; the k4 up fed the gate's Q8_0 image misses its row (the control)
[test]
def test_vulkan_dense_ffn_mixed_forms(t : T?) {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        install_vk_moe_hooks()
        let o1 = 173l * STRIDE   //! slots 173-175: past the shared-expert cell's
        let o3 = 174l * STRIDE
        let o2 = 175l * STRIDE
        var gq, gs : array<uint8>
        if (!q51_up(NFE, N, 151l, o1, gq, gs)) {
            t |> skip("dense ffn mixed forms: no Vulkan device (or probe/budget refused the upload)")
            return
        }
        var w3, w2 : array<float>
        t |> success(plane_up(int(KqFmt.k4), NFE, N, 157l, o3, w3), "dense ffn mixed forms: the k4 up plane uploaded")
        t |> success(plane_up(int(KqFmt.k6), N, NFE, 163l, o2, w2, false), "dense ffn mixed forms: the k6 down plane uploaded")
        for (rows in fixed_array(1l, 3l)) {
            var inscope x : array<float>
            fill_row(x, rows * N, 0.7 + float(rows))
            var inscope y : array<float>
            ffn_gpu_gemms(o1, o3, o2, int(KqFmt.q51), int(KqFmt.k4), int(KqFmt.k6), FfnAct.silu, N, NFE, rows, x, y)
            let g8 <- q8_roundtrip(x, rows * N, false)
            let uk <- q8_roundtrip(x, rows * N, true)
            var inscope g <- q51_rows_ref(gq, gs, g8.xq, g8.xs, NFE, N, rows)
            var inscope u <- dense_ref_rows(w3, uk.deq, int(rows), int(N), int(NFE))
            gate_batch(g, u.want, FfnAct.silu, NFE, rows)
            let hk <- q8_roundtrip(g, rows * NFE, true)
            var inscope d <- dense_ref_rows(w2, hk.deq, int(rows), int(NFE), int(N))
            held_row(t, "dense ffn mixed forms ({rows} rows): the FFN rows", copy_span(y, 0l, rows * N), d.want)
            var inscope u_one : array<float>
            u_one |> resize(rows * NFE)
            matmul_gpu_dense(u_one, o3, g8.xq, g8.xs, N, NFE, rows, int(KqFmt.k4))
            t |> success(check_count(u_one, u.want, ROW_BAR) > 0, "dense ffn mixed forms ({rows} rows): the k4 up fed the gate's Q8_0 image misses its row")
        }
    } else {
        t |> skip("dasVulkan not installed; the dense FFN's GEMMs are untestable here")
    }
}

//! an mx4 stack of `rows` rows of `n` on the upload wire (16 nibble bytes a block, an E8M0 byte a block) at `woff`, its bytes kept
//! for the reference; false = the upload was refused
def private mx4_up(rows, n, seed, woff : int64; var wq : array<uint8>; var we : array<uint8>; stream : bool = false) : bool {
    let nb = rows * (n / 32l)
    wq |> resize(nb * 16l)
    we |> resize(nb)
    var st = uint64(seed) * 2654435761ul + 12345ul
    for (c in wq) {
        c = lcg_byte(st)
    }
    for (b in range64(nb)) {
        we[b] = uint8(119l + b % 5l)   //! 2^-8 .. 2^-4 a block
    }
    return moe_gpu_upload_stack(wq, we, woff, n, rows, int(KqFmt.mx4), stream)
}

//! row `j` of an mx4 stack over the Q8_0 activation row at `xo`: the engine's scalar dot
def private mx4_row_dot(wq, we : array<uint8>; j, n : int64; xq : array<int8>; xs : array<float>; xo : int64) : float {
    var lut : int8[16]
    mxfp4_fill_lut(lut)
    let nb = n / 32l
    unsafe {
        return dot_mx4q8_scalar(addr(lut[0]), addr(wq[j * nb * 16l]), addr(we[j * nb]), addr(xq[xo]), addr(xs[xo / 32l]), n)
    }
}

//! the routed experts' fixture: three mx4 stacks of NE experts and the gate / up bias rows
struct private Mx4Experts {
    w1q : array<uint8>
    w1e : array<uint8>
    w3q : array<uint8>
    w3e : array<uint8>
    w2q : array<uint8>
    w2e : array<uint8>
    gb : array<float>
    ub : array<float>
    o1 : int64
    o3 : int64
    o2 : int64
}

//! the routed chain's reference over the regions `ex` (an expert each, `cnt` rows each, the rows in order): each row's gate and up
//! over its Q8_0 image, the expert's bias rows where `biased`, the act, the Q8_0 hidden, the down
def private routed_ref(f : Mx4Experts; x : array<float>; ex, cnt : array<int64>; biased : bool; act : FfnAct) : array<float> {
    var nrows = 0l
    for (c in cnt) {
        nrows += c
    }
    let xi <- q8_roundtrip(x, nrows * N, false)
    var y : array<float>
    y |> resize(nrows * N)
    var row = 0l
    for (e, ce in ex, cnt) {
        for (_c in range64(ce)) {
            var inscope g, u : array<float>
            g |> resize(NFE)
            u |> resize(NFE)
            for (j in range64(NFE)) {
                g[j] = mx4_row_dot(f.w1q, f.w1e, e * NFE + j, N, xi.xq, xi.xs, row * N) + (biased ? f.gb[e * NFE + j] : 0.0)
                u[j] = mx4_row_dot(f.w3q, f.w3e, e * NFE + j, N, xi.xq, xi.xs, row * N) + (biased ? f.ub[e * NFE + j] : 0.0)
            }
            gate_batch(g, u, act, NFE, 1l)
            let hq <- q8_roundtrip(g, NFE, false)
            for (i in range64(N)) {
                y[row * N + i] = mx4_row_dot(f.w2q, f.w2e, e * N + i, NFE, hq.xq, hq.xs, 0l)
            }
            row++
        }
    }
    return <- y
}

//! the regions over the fixture's stacks: expert `ex[r]`'s planes at its offset, `cnt[r]` rows from the running row
def private routed_regions(f : Mx4Experts; ex, cnt : array<int64>; var o1, o3, o2 : array<int64>) {
    var row = 0l
    for (e, ce in ex, cnt) {
        o1 |> push_from([f.o1 + e * NFE * N, row, ce])
        o3 |> push_from([f.o3 + e * NFE * N, row, ce])
        o2 |> push_from([f.o2 + e * N * NFE, row, ce])
        row += ce
    }
}

//! native-MXFP4 routed stacks on the per-op expert chain, plain and under gpt-oss's form (`MoeGpuFfnForm`: the gate/up bias rows,
//! the clamped swiglu), on the decode GEMV arm and the batch arm against the engine's scalar dots; the plain reference misses the
//! biased rows (the control that the form reaches the act)
[test]
def test_vulkan_routed_mx4_form(t : T?) {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        install_vk_moe_hooks()
        var f = Mx4Experts(o1 = 176l * STRIDE, o3 = 177l * STRIDE, o2 = 178l * STRIDE)   //! slots 176-178: past the dense FFN cell's
        if (!mx4_up(NE * NFE, N, 181l, f.o1, f.w1q, f.w1e)) {
            t |> skip("routed mx4 form: no Vulkan device (or probe/budget refused the upload)")
            return
        }
        t |> success(mx4_up(NE * NFE, N, 183l, f.o3, f.w3q, f.w3e), "routed mx4 form: the up stack uploaded")
        t |> success(mx4_up(NE * N, NFE, 187l, f.o2, f.w2q, f.w2e), "routed mx4 form: the down stack uploaded")
        f.gb <- [for (i in range64(NE * NFE)); 0.3 * sin(float(i) * 0.71)]
        f.ub <- [for (i in range64(NE * NFE)); 0.25 * cos(float(i) * 0.53)]
        t |> success(moe_gpu_upload_ffn_bias(f.gb, f.ub), "routed mx4 form: the bias rows uploaded")
        let arms = fixed_array((name = "decode", ex = [2l, 0l, 3l, 1l], cnt = [1l, 1l, 1l, 1l]),
                               (name = "batch", ex = [1l, 3l, 0l], cnt = [15l, 9l, 16l]))
        for (arm in arms) {
            var inscope o1, o3, o2 : array<int64>
            routed_regions(f, arm.ex, arm.cnt, o1, o3, o2)
            var nrows = 0l
            for (c in arm.cnt) {
                nrows += c
            }
            var inscope x : array<float>
            fill_row(x, nrows * N, 0.9)
            let xi <- q8_roundtrip(x, nrows * N, false)
            for (biased in fixed_array(true, false)) {
                let tag = "{arm.name}, {biased ? "biased clamped swiglu" : "plain silu"}"
                if (biased) {
                    set_moe_gpu_ffn_form(MoeGpuFfnForm(oai = true, bias_off = 0l, e_base = f.o1, e_stride = NFE * N))
                }
                var inscope y : array<float>
                y |> resize(nrows * N)
                matmul_moe_gpu_ffn(y, o1, o3, o2, long_length(arm.ex), xi.xq, xi.xs, N, NFE, nrows,
                    int(KqFmt.mx4), int(KqFmt.mx4), int(KqFmt.mx4), false)
                clear_moe_gpu_ffn_form()
                var inscope want <- routed_ref(f, x, arm.ex, arm.cnt, biased, biased ? FfnAct.swiglu_oai : FfnAct.silu)
                held_row(t, "routed mx4 form ({tag}): the down rows", y, want)
                if (biased) {
                    var inscope plain <- routed_ref(f, x, arm.ex, arm.cnt, false, FfnAct.silu)
                    t |> success(check_count(y, plain, ROW_BAR) > 0, "routed mx4 form ({tag}): the plain reference misses the device rows")
                }
            }
        }
    } else {
        t |> skip("dasVulkan not installed; the routed expert chain is untestable here")
    }
}

//! a routed chain's rows over dequantized gate (`g8` Q8_0 rows), up (`uk` Q8_K rows) and down stacks: expert `ex[r]`'s rows for
//! `cnt[r]` rows in order, the silu act, the hidden row through the k6 down's Q8_K form
def private two_forms_ref(w1, w3, w2 : array<float>; g8, uk : array<float>; ex, cnt : array<int64>) : array<float> {
    var y : array<float>
    var rows_all = 0l
    for (ce in cnt) {
        rows_all += ce
    }
    y |> reserve(rows_all * N)
    var row = 0l
    for (e, ce in ex, cnt) {
        for (_c in range64(ce)) {
            var inscope g, u : array<float>
            g |> resize(NFE)
            u |> resize(NFE)
            for (j in range64(NFE)) {
                var ag = 0.0lf
                var au = 0.0lf
                for (i in range64(N)) {
                    ag += double(w1[(e * NFE + j) * N + i]) * double(g8[row * N + i])
                    au += double(w3[(e * NFE + j) * N + i]) * double(uk[row * N + i])
                }
                g[j] = float(ag)
                u[j] = float(au)
            }
            gate_batch(g, u, FfnAct.silu, NFE, 1l)
            let hk <- q8_roundtrip(g, NFE, true)
            for (i in range64(N)) {
                var ad = 0.0lf
                for (j in range64(NFE)) {
                    ad += double(w2[(e * N + i) * NFE + j]) * double(hk.deq[j])
                }
                y |> push(float(ad))
            }
            row++
        }
    }
    return <- y
}

//! routed experts whose gate and up take two activation classes - a q8 gate stack fed Q8_0 beside a k4 up stack fed Q8_K, a k6
//! down - through `matmul_moe_gpu_ffn`, the up stacks reading the call's second image (`set_moe_gpu_image2`), on the decode GEMV
//! arm and the batch arm; the up stacks fed the gate's Q8_0 image miss the rows (the control that the second image reaches them)
[test]
def test_vulkan_routed_two_forms(t : T?) {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        install_vk_moe_hooks()
        let o1 = 179l * STRIDE   //! slots 179-181: past the routed mx4 cell's
        let o3 = 180l * STRIDE
        let o2 = 181l * STRIDE
        var w1, w3, w2 : array<float>
        if (!plane_up(int(KqFmt.q8), NE * NFE, N, 191l, o1, w1)) {
            t |> skip("routed two forms: no Vulkan device (or probe/budget refused the upload)")
            return
        }
        t |> success(plane_up(int(KqFmt.k4), NE * NFE, N, 193l, o3, w3), "routed two forms: the k4 up stack uploaded")
        t |> success(plane_up(int(KqFmt.k6), NE * N, NFE, 197l, o2, w2, false), "routed two forms: the k6 down stack uploaded")
        let arms = fixed_array((name = "decode", ex = [3l, 1l, 0l, 2l], cnt = [1l, 1l, 1l, 1l]),
                               (name = "batch", ex = [2l, 0l, 3l], cnt = [13l, 11l, 16l]))
        for (arm in arms) {
            var inscope o1s, o3s, o2s : array<int64>
            var row = 0l
            for (e, ce in arm.ex, arm.cnt) {
                o1s |> push_from([o1 + e * NFE * N, row, ce])
                o3s |> push_from([o3 + e * NFE * N, row, ce])
                o2s |> push_from([o2 + e * N * NFE, row, ce])
                row += ce
            }
            var inscope x : array<float>
            fill_row(x, row * N, 1.3)
            let g8 <- q8_roundtrip(x, row * N, false)
            let uk <- q8_roundtrip(x, row * N, true)
            var inscope want <- two_forms_ref(w1, w3, w2, g8.deq, uk.deq, arm.ex, arm.cnt)
            for (own in fixed_array(true, false)) {
                if (own) {
                    set_moe_gpu_image2(uk.xq, uk.xs)
                } else {
                    set_moe_gpu_image2(g8.xq, g8.xs)
                }
                var inscope y : array<float>
                y |> resize(row * N)
                matmul_moe_gpu_ffn(y, o1s, o3s, o2s, long_length(arm.ex), g8.xq, g8.xs, N, NFE, row,
                    int(KqFmt.q8), int(KqFmt.k4), int(KqFmt.k6), false)
                clear_moe_gpu_image2()
                if (own) {
                    held_row(t, "routed two forms ({arm.name}): the down rows, each stack over its own image", y, want)
                } else {
                    t |> success(check_count(y, want, ROW_BAR) > 0, "routed two forms ({arm.name}): the up stacks fed the gate's Q8_0 image miss the rows")
                }
            }
        }
    } else {
        t |> skip("dasVulkan not installed; the routed expert chain is untestable here")
    }
}

let private HEAT_LAYER = 11l   //! the heat cell's pool layer

//! the heat pool under gpt-oss's form: native-MXFP4 stacks streamed, two experts pinned, a top-k of three splitting into two hits
//! the pool serves on the async chain under the biased clamped swiglu - the hits' experts carried beside their slot regions
//! (`set_moe_gpu_ffn_experts`) - against the engine's scalar dots; the plain reference misses the hit rows (the control)
[test]
def test_vulkan_heat_mx4_form(t : T?) {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        install_vk_moe_hooks()
        set_moe_gpu_heat_hooks(@@vk_moe_heat_query, @@vk_moe_heat_advise, @@vk_moe_ffn_begin, @@vk_moe_ffn_join)
        set_gpu_tier_want_(GpuTierWant(heat = 2l))
        var f = Mx4Experts(o1 = 182l * STRIDE, o3 = 183l * STRIDE, o2 = 184l * STRIDE)   //! slots 182-184: past the two-forms cell's
        if (!mx4_up(NE * NFE, N, 201l, f.o1, f.w1q, f.w1e, true)) {
            set_gpu_tier_want_(GpuTierWant())
            t |> skip("heat mx4 form: no Vulkan device (or probe refused the stream uploads)")
            return
        }
        t |> success(mx4_up(NE * NFE, N, 203l, f.o3, f.w3q, f.w3e, true), "heat mx4 form: the up stack streamed")
        t |> success(mx4_up(NE * N, NFE, 207l, f.o2, f.w2q, f.w2e, true), "heat mx4 form: the down stack streamed")
        f.gb <- [for (i in range64(NE * NFE)); 0.3 * sin(float(i) * 0.37)]
        f.ub <- [for (i in range64(NE * NFE)); 0.25 * cos(float(i) * 0.61)]
        t |> success(moe_gpu_upload_ffn_bias(f.gb, f.ub), "heat mx4 form: the bias rows uploaded")
        let mx = int(KqFmt.mx4)
        var inscope ids <- [1l, 3l]
        moe_gpu_heat_advise(HEAT_LAYER, f.o1, f.o3, f.o2, mx, mx, mx, N, NFE, ids, 2l)
        var inscope idx <- [1l, 0l, 3l]
        var inscope hit : array<int8>
        var inscope h1, h3, h2 : array<int64>
        hit |> resize(3)
        h1 |> resize(3)
        h3 |> resize(3)
        h2 |> resize(3)
        let nh = matmul_moe_gpu_heat_query(HEAT_LAYER, idx, 3l, hit, h1, h3, h2)
        t |> equal(nh, 2l, "heat mx4 form: the mx4 pool takes the two pinned experts")
        if (nh == 2l) {
            var inscope o1s, o3s, o2s : array<int64>
            for (i in range64(nh)) {
                o1s |> push_from([h1[i], i, 1l])
                o3s |> push_from([h3[i], i, 1l])
                o2s |> push_from([h2[i], i, 1l])
            }
            var inscope x : array<float>
            fill_row(x, 2l * N, 0.4)
            let xi <- q8_roundtrip(x, 2l * N, false)
            var inscope ex <- [1l, 3l]
            set_moe_gpu_ffn_form(MoeGpuFfnForm(oai = true, bias_off = 0l, e_base = f.o1, e_stride = NFE * N))
            set_moe_gpu_ffn_experts(ex)
            matmul_moe_gpu_ffn_begin(o1s, o3s, o2s, nh, xi.xq, xi.xs, N, NFE, nh, mx, mx, mx, false)
            clear_moe_gpu_ffn_form()
            clear_moe_gpu_ffn_experts()
            var inscope y : array<float>
            y |> resize(2l * N)
            matmul_moe_gpu_ffn_join(y)
            var inscope want <- routed_ref(f, x, ex, [1l, 1l], true, FfnAct.swiglu_oai)
            held_row(t, "heat mx4 form: the hits' down rows under the biased clamped swiglu", y, want)
            var inscope plain <- routed_ref(f, x, ex, [1l, 1l], false, FfnAct.silu)
            t |> success(check_count(y, plain, ROW_BAR) > 0, "heat mx4 form: the plain reference misses the hit rows")
        }
        set_gpu_tier_want_(GpuTierWant())
    } else {
        t |> skip("dasVulkan not installed; the heat pool is untestable here")
    }
}

[test]
def test_vulkan_dec_tail(t : T?) {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        install_vk_moe_hooks()
        var f : Ffn
        if (!ffn_up(t, f)) {
            t |> skip("dec tail: no Vulkan device (or probe/budget refused the upload)")
            return
        }
        t |> success(pair_cell(t, f, 1, 10l * STRIDE, "k4 quads (Q8_K feed, neox rope)", true, false), "dec tail: the k4 pair uploaded")
        t |> success(pair_cell(t, f, 0, 40l * STRIDE, "q8 quads (Q8_0 feed, classic rope)", false, false), "dec tail: the q8 pair uploaded")
        t |> success(pair_cell(t, f, 0, 80l * STRIDE, "q8 quads with the q/k/v projection bias (qwen2moe, neox rope)", true, true), "dec tail: the biased q8 pair uploaded")
        t |> success(collide_cell(t, f, 140l * STRIDE, "a k4 quad and a q8 quad at one plane offset"), "dec tail: the shared-offset pair uploaded")   //! slot 140: past every quad and stack the file's other cells upload
    } else {
        t |> skip("dasVulkan not installed; the decode block and tail are untestable here")
    }
}
