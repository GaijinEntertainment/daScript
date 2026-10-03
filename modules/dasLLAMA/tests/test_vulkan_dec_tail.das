options gen2
options _comment_hygiene = true
options _dasllama_internal = true

module test_vulkan_dec_tail

require dastest/testing_boost public
require dasllama/dasllama_math
require dasllama/dasllama_gpu_math
require dasllama/dasllama_rope
require dasllama/dasllama_common   // MoeGate, select_decode_override
require dasllama/dasllama_blocks   // registers the vulkan_moe_span decode override at init
require dasllama/dasllama_moe   // moe_select_core: the host select the span's device top-k mirrors
require ?vulkan dasllama/dasllama_math_vulkan  // the decode attention block + FFN tail under test; the test installs the hooks explicitly (no env gate)
require ?vulkan dasllama/dasllama_vulkan_decode   // vk_span_reset: the span records once per model, so the second span cell forgets the first's record
require daslib/f16_cvt
require math
require _vk_kq_fixtures
require _kq_fixtures   // q8_roundtrip: the feeds and requants the device quantizes, through the engine's quantizers
require _compares
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
    y |> resize(rows)
    for (r in range64(rows)) {
        var acc = 0.0
        for (c in range64(n)) {
            acc += w[r * n + c] * x[c]
        }
        y[r] = acc
    }
}

//! the device's feed: rms-normed, then quantized in the plane's acts form (Q8_0 for q8 planes, Q8_K for K-quant ones)
def private feed_ref(x : array<float>; wn : array<float>; fmt : int) : Q8Round {
    var xb : array<float>
    xb |> resize(N)
    rmsnorm(xb, x, wn, N, EPS)
    return <- q8_roundtrip(xb, N, fmt != 0)
}

// the device rope's two pairings: neox rotates (j, j + half), the classic form (2j, 2j + 1)
def private rope_ref(var v : array<float>; nheads : int64; cossin : array<float>; neox : bool) {
    unsafe {
        let cosrow = addr<float const?>(cossin[0])
        let sinrow = addr<float const?>(cossin[HS / 2l])
        if (neox) {
            rope_scaled_neox_tab(addr<float?>(v[0]), HS, nheads * HS, cosrow, sinrow)
        } else {
            rope_scaled_tab(addr<float?>(v[0]), HS, nheads * HS, cosrow, sinrow)
        }
    }
}

// one head group's decode attention over the f16 mirror rows [0, cnt)
def private attn_ref(q : array<float>; kmir, vmir : array<float>; cnt : int64; var y : array<float>) {
    y |> resize(QD)
    for (h in range64(NH)) {
        attn_row_oracle(q, int(h * HS), kmir, 0, vmir, 0, int(KVD), int(h), int(HS), int(KVM), int(cnt), ATTN_SCALE, y, int(h * HS))
    }
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

// one plane in the quad's format, uploaded at woff and dequantized; false = the upload was refused
def private plane_up(fmt : int; rows, n, seed, woff : int64; var w : array<float>) : bool {
    var wqb : array<uint8>
    var wsb : array<uint8>
    if (fmt == 0) {
        fill_q8_stack(rows, n, seed, wqb, wsb, w)
    } else {
        fill_kq_stack(fmt, rows, n, seed, true, wqb, wsb)
        dequant_kq_stack(fmt, wqb, wsb, rows, n, w)
    }
    return moe_gpu_upload_stack(wqb, wsb, woff, n, rows, fmt)
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
    var wq1 : array<uint8>
    var ws1 : array<uint8>
    var wq3 : array<uint8>
    var ws3 : array<uint8>
    var wq2 : array<uint8>
    var ws2 : array<uint8>
    fill_kq_stack(1, NE * NFE, N, 41l, true, wq1, ws1)
    fill_kq_stack(1, NE * NFE, N, 43l, true, wq3, ws3)
    fill_kq_stack(3, NE * N, NFE, 47l, false, wq2, ws2)
    let woff1 = base_slot * STRIDE
    let woff3 = (base_slot + 1l) * STRIDE
    let woff2 = (base_slot + 2l) * STRIDE
    if (!moe_gpu_upload_stack(wq1, ws1, woff1, N, NE * NFE, 1)) {
        return false
    }
    t |> success(moe_gpu_upload_stack(wq3, ws3, woff3, N, NE * NFE, 1), "dec tail: up stack uploaded")
    t |> success(moe_gpu_upload_stack(wq2, ws2, woff2, NFE, NE * N, 3), "dec tail: down stack uploaded")
    dequant_kq_stack(1, wq1, ws1, NE * NFE, N, f.w1)
    dequant_kq_stack(1, wq3, ws3, NE * NFE, N, f.w3)
    dequant_kq_stack(3, wq2, ws2, NE * N, NFE, f.w2)
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
    var qr : array<float>
    var kr : array<float>
    var vr : array<float>
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
    rope_ref(qr, NH, cossin, q.neox)
    rope_ref(kr, 1l, cossin, q.neox)
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
    var no_freqs : array<float>
    var cossin : array<float>
    cossin |> resize(HS)
    build_rope_row_packed(cossin, 0l, pos, HS, 10000.0, 1.0, 1.0, no_freqs)
    var x : array<float>
    fill_row(x, N, 0.2 + 0.9 * float(pos))
    var xb2 : array<float>
    var kout : array<float>
    var vout : array<float>
    xb2 |> resize(N)
    kout |> resize(KVD)
    vout |> resize(KVD)
    var feed <- feed_ref(x, qa.norm, qa.fmt)
    matmul_moe_gpu_attn_dec(xb2, kout, vout, qa.woq, qa.wok, qa.wov, qa.woo, qa.fmt, qa.fmt, qa.fmt, qa.fmt, feed.xq, feed.xs, cossin, null, null,
        bias_ptr(qa.bq), bias_ptr(qa.bk), bias_ptr(qa.bv), pos, N, QD, KVD, HS, NH, KVM, ATTN_SCALE, EPS, qa.neox, false)
    var xb2_ref : array<float>
    var kraw_ref : array<float>
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
    var xb2b_ref : array<float>
    var krawb_ref : array<float>
    block_ref(qb, x, pos, cossin, xb2b_ref, krawb_ref)
    held_row(t, "{tag} pos {pos} layer B wo row (fed by the tail)", xb2, xb2b_ref)
    held_row(t, "{tag} pos {pos} layer B raw k row", kout, krawb_ref)
}

//! two layers whose q planes sit at ONE offset in two format planes (a mixed-format file's shape): the block keys a layer by
//! its offset under its format, so each layer serves its own quad and mirror; false = no device
def private collide_cell(t : T?; f : Ffn; base : int64; tag : string) : bool {
    var qa : Quad
    var qb : Quad
    if (!quad_up(qa, 1, base, 51l, 0.4, true, false)) {
        return false
    }
    t |> success(quad_up(qb, 0, base, 63l, 0.8, true, false), "{tag}: the q8 quad uploaded at the k4 quad's offset")
    let gen = moe_gpu_attn_dec_claim(true)
    matmul_moe_gpu_attn_dec_hydrate(qa.woq, qa.fmt, null, null, 0l, KVD, true)
    t |> success(moe_gpu_attn_dec_ok(qb.woq, qb.fmt, 0l, gen) == DAT_HYDRATE, "{tag}: the second layer at the shared offset is still unseen after the first one's hydrate")
    matmul_moe_gpu_attn_dec_hydrate(qb.woq, qb.fmt, null, null, 0l, KVD, true)
    t |> success(moe_gpu_attn_dec_ok(qa.woq, qa.fmt, 0l, gen) == DAT_SERVE && moe_gpu_attn_dec_ok(qb.woq, qb.fmt, 0l, gen) == DAT_SERVE,
        "{tag}: both layers serve position 0")
    two_positions(t, qa, qb, f, gen, tag)
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
    var qa : Quad
    var qb : Quad
    if (!quad_up(qa, fmt, base, 21l + int64(fmt), 0.3, neox, hasb)) {
        return false
    }
    t |> success(quad_up(qb, fmt, base + 10l * STRIDE, 33l + int64(fmt), 0.7, neox, hasb), "{tag}: layer B's quad uploaded")
    let gen = moe_gpu_attn_dec_claim(true)
    // a layer the block has never seen hydrates first (zero rows at position 0 creates it), then serves
    t |> success(moe_gpu_attn_dec_ok(qa.woq, qa.fmt, 0l, gen) == DAT_HYDRATE, "{tag}: an unseen layer asks for a hydrate at position 0")
    matmul_moe_gpu_attn_dec_hydrate(qa.woq, qa.fmt, null, null, 0l, KVD, true)
    matmul_moe_gpu_attn_dec_hydrate(qb.woq, qb.fmt, null, null, 0l, KVD, true)
    t |> success(moe_gpu_attn_dec_ok(qa.woq, qa.fmt, 0l, gen) == DAT_SERVE && moe_gpu_attn_dec_ok(qb.woq, qb.fmt, 0l, gen) == DAT_SERVE,
        "{tag}: both layers serve position 0 after the empty hydrate")
    two_positions(t, qa, qb, f, gen, tag)
    // the hydrate arms: a fresh claim empties both mirrors; A refills from f32 rows, B from f16 rows
    let gen2 = moe_gpu_attn_dec_claim(true)
    t |> success(moe_gpu_attn_dec_ok(qa.woq, qa.fmt, 2l, gen2) == DAT_HYDRATE, "{tag}: a fresh claim empties the mirrors - position 2 asks for a hydrate")
    var kb : array<uint8>
    var vb : array<uint8>
    kb |> resize(2l * KVD * 2l)
    vb |> resize(2l * KVD * 2l)
    for (i in range64(2l * KVD)) {
        let hk = f32_to_f16(qb.kmir[i])
        let hv = f32_to_f16(qb.vmir[i])
        kb[i * 2l] = uint8(hk & 0xFFu)
        kb[i * 2l + 1l] = uint8((hk >> 8u) & 0xFFu)
        vb[i * 2l] = uint8(hv & 0xFFu)
        vb[i * 2l + 1l] = uint8((hv >> 8u) & 0xFFu)
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
def private ffn_ref_sel(f : Ffn; fd : array<float>; idx : array<int64>; w : array<float>; var x1 : array<float>) {
    let ege = NFE * N
    var acc : array<float>
    acc |> resize(N)
    var wsl : array<float>
    wsl |> resize(NFE * N)
    for (j in range64(K)) {
        let e = idx[j]
        var gg : array<float>
        var u : array<float>
        for (i in range64(NFE * N)) {
            wsl[i] = f.w1[e * ege + i]
        }
        gemv_ref(wsl, NFE, N, fd, gg)
        for (i in range64(NFE * N)) {
            wsl[i] = f.w3[e * ege + i]
        }
        gemv_ref(wsl, NFE, N, fd, u)
        var hrow : array<float>
        hrow |> resize(NFE)
        for (i in range64(NFE)) {
            hrow[i] = silu_f32(gg[i]) * u[i]
        }
        let hr <- q8_roundtrip(hrow, NFE, true)
        for (i in range64(N * NFE)) {
            wsl[i] = f.w2[e * ege + i]
        }
        var d : array<float>
        gemv_ref(wsl, N, NFE, hr.deq, d)
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
    var gg : array<float>
    var u : array<float>
    gemv_ref(sh.w1, NFE, N, fd0.deq, gg)
    gemv_ref(sh.w3, NFE, N, fd0.deq, u)
    var hrow : array<float>
    hrow |> resize(NFE)
    for (i in range64(NFE)) {
        hrow[i] = silu_f32(gg[i]) * u[i]
    }
    let hr <- q8_roundtrip(hrow, NFE, false)
    var d : array<float>
    gemv_ref(sh.w2, N, NFE, hr.deq, d)
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
        install_moe_gpu_tier(@@vk_moe_ffn, @@vk_moe_upload_stack, @@vk_moe_cls, @@vk_moe_dense, @@vk_moe_drop_stacks, @@vk_moe_dn, @@vk_moe_attn)
        set_moe_gpu_attn_dec_hooks(@@vk_moe_attn_dec, @@vk_moe_attn_dec_ok, @@vk_moe_attn_dec_claim, @@vk_moe_attn_dec_hydrate)
        set_moe_gpu_span_dec_hook(@@vk_moe_span_dec)
        var f : Ffn
        if (!ffn_up(t, f)) {
            t |> skip("dec span: no Vulkan device (or probe/budget refused the upload)")
            return
        }
        var qa : Quad
        var qb : Quad
        t |> success(quad_up(qa, 1, 60l * STRIDE, 51l, 0.4, true, false), "dec span: layer A's quad uploaded")
        t |> success(quad_up(qb, 1, 70l * STRIDE, 63l, 0.8, true, false), "dec span: layer B's quad uploaded")
        let gen = moe_gpu_attn_dec_claim(true)
        matmul_moe_gpu_attn_dec_hydrate(qa.woq, qa.fmt, null, null, 0l, KVD, true)
        matmul_moe_gpu_attn_dec_hydrate(qb.woq, qb.fmt, null, null, 0l, KVD, true)
        t |> success(moe_gpu_attn_dec_ok(qa.woq, qa.fmt, 0l, gen) == DAT_SERVE && moe_gpu_attn_dec_ok(qb.woq, qb.fmt, 0l, gen) == DAT_SERVE,
            "dec span: both layers serve position 0")
        var sp <- span_planes(qa, qb, f)
        var a <- span_args(qa, qb, f)
        var kout : array<float>
        var vout : array<float>
        kout |> resize(2l * KVD)
        vout |> resize(2l * KVD)
        var cossin : array<float>
        cossin |> resize(HS)
        for (pos in range64(2)) {
            var rows <- span_position(a, qa, sp, cossin, pos, kout, vout)
            var kraw_a : array<float>
            var kraw_b : array<float>
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
        install_moe_gpu_tier(@@vk_moe_ffn, @@vk_moe_upload_stack, @@vk_moe_cls, @@vk_moe_dense, @@vk_moe_drop_stacks, @@vk_moe_dn, @@vk_moe_attn)
        set_moe_gpu_attn_dec_hooks(@@vk_moe_attn_dec, @@vk_moe_attn_dec_ok, @@vk_moe_attn_dec_claim, @@vk_moe_attn_dec_hydrate)
        set_moe_gpu_span_dec_hook(@@vk_moe_span_dec)
        //! bases past every other cell's (the tail pairs sit at 10-23, 40-53 and 80-93 x STRIDE, the
        //! routed stacks at 30-32, the plain span's quads at 60-73): first-hit stack lookup
        var f : Ffn
        if (!ffn_up(t, f, 100l)) {
            t |> skip("dec span shexp: no Vulkan device (or probe/budget refused the upload)")
            return
        }
        var sh : Shexp
        t |> success(shexp_up(t, sh, 103l * STRIDE), "dec span shexp: the shared gate plane uploaded")
        var qa : Quad
        var qb : Quad
        t |> success(quad_up(qa, 1, 110l * STRIDE, 51l, 0.4, true, false), "dec span shexp: layer A's quad uploaded")
        t |> success(quad_up(qb, 1, 120l * STRIDE, 63l, 0.8, true, false), "dec span shexp: layer B's quad uploaded")
        var sp <- span_planes(qa, qb, f)
        var cossin : array<float>
        cossin |> resize(HS)
        for (gated in fixed_array(true, false)) {
            vk_span_reset()   //! the record keys on the shared expert's gate: the ungated arm is a new span
            let tag = gated ? "gated" : "ungated"
            let gen = moe_gpu_attn_dec_claim(true)
            matmul_moe_gpu_attn_dec_hydrate(qa.woq, qa.fmt, null, null, 0l, KVD, true)
            matmul_moe_gpu_attn_dec_hydrate(qb.woq, qb.fmt, null, null, 0l, KVD, true)
            t |> success(moe_gpu_attn_dec_ok(qa.woq, qa.fmt, 0l, gen) == DAT_SERVE && moe_gpu_attn_dec_ok(qb.woq, qb.fmt, 0l, gen) == DAT_SERVE,
                "dec span shexp ({tag}): both layers serve position 0")
            var a <- span_args_sh(qa, qb, f, sh, gated)
            var kout : array<float>
            var vout : array<float>
            kout |> resize(2l * KVD)
            vout |> resize(2l * KVD)
            a.shgate = unsafe(addr<float const?>(sh.gate[0]))
            for (pos in range64(2)) {
                var rows <- span_position(a, qa, sp, cossin, pos, kout, vout)
                var kraw_a : array<float>
                var kraw_b : array<float>
                span_layer_ref_sh(qa, f, sh, sp.router, 0l, rows.x_ref, pos, cossin, kraw_a, gated)
                span_layer_ref_sh(qb, f, sh, sp.router, 1l, rows.x_ref, pos, cossin, kraw_b, gated)
                held_row(t, "span shexp ({tag}) pos {pos} residual row after both layers", rows.x, rows.x_ref)
                if (pos == 0l) {
                    var x_no : array<float>
                    fill_row(x_no, N, 0.5)
                    var ka : array<float>
                    var kb : array<float>
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

[test]
def test_vulkan_dec_tail(t : T?) {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        install_moe_gpu_tier(@@vk_moe_ffn, @@vk_moe_upload_stack, @@vk_moe_cls, @@vk_moe_dense, @@vk_moe_drop_stacks, @@vk_moe_dn, @@vk_moe_attn)
        set_moe_gpu_attn_dec_hooks(@@vk_moe_attn_dec, @@vk_moe_attn_dec_ok, @@vk_moe_attn_dec_claim, @@vk_moe_attn_dec_hydrate)
        set_moe_gpu_ffn_tail_hooks(@@vk_moe_ffn_tail, @@vk_moe_ffn_tail_pending)
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
