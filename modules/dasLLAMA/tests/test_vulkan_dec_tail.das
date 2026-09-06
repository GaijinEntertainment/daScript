options gen2
options _comment_hygiene = true
options _dasllama_internal = true

module test_vulkan_dec_tail

require dastest/testing_boost public
require dasllama/dasllama_math
require dasllama/dasllama_convert
require dasllama/dasllama_rope
require dasllama/dasllama_common   // MoeGate, select_decode_override
require dasllama/dasllama_blocks   // registers the vulkan_moe_span decode override at init
require dasllama/dasllama_moe   // moe_select_core: the host select the span's device top-k mirrors
require ?vulkan dasllama/dasllama_math_vulkan  // the decode attention block + FFN tail under test; the test installs the hooks explicitly (no env gate)
require daslib/f16_cvt
require math
require _vk_kq_fixtures

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

def private f16r(v : float) : float => f16_to_f32(f32_to_f16(v))

def private silu(v : float) : float => v / (1.0 + exp(-v))

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

// the device's feed: rms-normed, then quantized in the plane's acts form (Q8_0 for q8 planes,
// Q8_K for K-quant ones) and dequantized - what the GEMVs read
def private feed_ref(x : array<float>; wn : array<float>; fmt : int; var xd : array<float>; var xq : array<int8>; var xs : array<float>) {
    var xb : array<float>
    xb |> resize(N)
    rmsnorm(xb, x, wn, N, EPS)
    xq |> resize(N)
    xd |> resize(N)
    if (fmt == 0) {
        xs |> resize(N / 32l)
        quantize_q8_0_into(xb, N, xq, xs, 0l, 0l)
        for (i in range64(N)) {
            xd[i] = float(xq[i]) * xs[i / 32l]
        }
    } else {
        var xbs : array<int>
        xs |> resize(N / 256l)
        xbs |> resize(N / 16l)
        quantize_q8_k_into(xb, N, xq, xs, xbs, 0l, 0l, 0l)
        for (i in range64(N)) {
            xd[i] = float(xq[i]) * xs[i / 256l]
        }
    }
}

// the device rope's two pairings: neox rotates (j, j + half), the classic form (2j, 2j + 1)
def private rope_ref(var v : array<float>; nheads : int64; cossin : array<float>; neox : bool) {
    let half = HS / 2l
    for (h in range64(nheads)) {
        for (j in range64(half)) {
            let e0 = neox ? h * HS + j : h * HS + 2l * j
            let e1 = neox ? e0 + half : e0 + 1l
            let a0 = v[e0]
            let a1 = v[e1]
            v[e0] = a0 * cossin[j] - a1 * cossin[half + j]
            v[e1] = a0 * cossin[half + j] + a1 * cossin[j]
        }
    }
}

// one head group's decode attention over the f16 mirror rows [0, cnt)
def private attn_ref(q : array<float>; kmir, vmir : array<float>; cnt : int64; var y : array<float>) {
    y |> resize(QD)
    var sc : array<float>
    sc |> resize(cnt)
    for (h in range64(NH)) {
        let kvh = h / KVM
        var m = -3.0e38
        for (p in range64(cnt)) {
            var s = 0.0
            for (d in range64(HS)) {
                s += q[h * HS + d] * kmir[p * KVD + kvh * HS + d]
            }
            sc[p] = s * ATTN_SCALE
            m = max(m, sc[p])
        }
        var den = 0.0
        for (p in range64(cnt)) {
            sc[p] = exp(sc[p] - m)
            den += sc[p]
        }
        for (c in range64(HS)) {
            var acc = 0.0
            for (p in range64(cnt)) {
                acc += sc[p] * vmir[p * KVD + kvh * HS + c]
            }
            y[h * HS + c] = acc / den
        }
    }
}

def private check_row(t : T?; tag : string; gpu, refv : array<float>) {
    let mr = max_abs(refv)
    let rel_bar = REL_BAR * mr
    let bad = count_bad(gpu, refv, ABS_BAR, rel_bar)
    let md = max_diff(gpu, refv)
    to_log(LOG_INFO, "{tag}: {bad} of {length(refv)} off the reference (maxdiff {md}, maxref {mr}, rel bar {rel_bar})\n")
    t |> success(mr > 0.0, "{tag}: the reference is non-trivial")
    t |> success(bad == 0, "{tag}: matches the CPU reference ({bad} of {length(refv)} off, maxdiff {md} vs abs {ABS_BAR} / rel {rel_bar})")
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
    kmir : array<float> // the reference mirror (f16-rounded rows at absolute positions)
    vmir : array<float>
}

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

// one attention quad's four planes, uploaded and dequantized; false = no device
def private quad_up(var q : Quad; fmt : int; base : int64; seed : int64; phase : float; neox : bool) : bool {
    q.fmt = fmt
    q.neox = neox
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
def private ffn_up(t : T?; var f : Ffn) : bool {
    var wq1 : array<uint8>
    var ws1 : array<uint8>
    var wq3 : array<uint8>
    var ws3 : array<uint8>
    var wq2 : array<uint8>
    var ws2 : array<uint8>
    fill_kq_stack(1, NE * NFE, N, 41l, true, wq1, ws1)
    fill_kq_stack(1, NE * NFE, N, 43l, true, wq3, ws3)
    fill_kq_stack(3, NE * N, NFE, 47l, false, wq2, ws2)
    let woff1 = 30l * STRIDE
    let woff3 = 31l * STRIDE
    let woff2 = 32l * STRIDE
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

// the block's reference for one position: the feed, q/k/v, rope, the mirror row, attention, wo
def private block_ref(var q : Quad; x : array<float>; pos : int64; cossin : array<float>; var xb2 : array<float>; var kraw : array<float>) {
    var xd : array<float>
    var xq : array<int8>
    var xs : array<float>
    feed_ref(x, q.norm, q.fmt, xd, xq, xs)
    var qr : array<float>
    var kr : array<float>
    var vr : array<float>
    gemv_ref(q.wq, QD, N, xd, qr)
    gemv_ref(q.wk, KVD, N, xd, kr)
    gemv_ref(q.wv, KVD, N, xd, vr)
    kraw := kr
    rope_ref(qr, NH, cossin, q.neox)
    rope_ref(kr, 1l, cossin, q.neox)
    for (i in range64(KVD)) {
        q.kmir[pos * KVD + i] = f16r(kr[i])
        q.vmir[pos * KVD + i] = f16r(vr[i])
    }
    var y : array<float>
    attn_ref(qr, q.kmir, q.vmir, pos + 1l, y)
    // the device requantizes the attention row in the wo plane's form before the wo GEMV
    var yq : array<int8>
    var ys : array<float>
    yq |> resize(QD)
    if (q.fmt == 0) {
        ys |> resize(QD / 32l)
        quantize_q8_0_into(y, QD, yq, ys, 0l, 0l)
        for (i in range64(QD)) {
            y[i] = float(yq[i]) * ys[i / 32l]
        }
    } else {
        var ybs : array<int>
        ys |> resize(QD / 256l)
        ybs |> resize(QD / 16l)
        quantize_q8_k_into(y, QD, yq, ys, ybs, 0l, 0l, 0l)
        for (i in range64(QD)) {
            y[i] = float(yq[i]) * ys[i / 256l]
        }
    }
    gemv_ref(q.wo, N, QD, y, xb2)
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
    var xd : array<float>
    var xq : array<int8>
    var xs : array<float>
    feed_ref(x, qa.norm, qa.fmt, xd, xq, xs)
    matmul_moe_gpu_attn_dec(xb2, kout, vout, qa.woq, qa.wok, qa.wov, qa.woo, qa.fmt, qa.fmt, qa.fmt, qa.fmt, xq, xs, cossin, null, null,
        pos, N, QD, KVD, HS, NH, KVM, ATTN_SCALE, EPS, qa.neox, false)
    var xb2_ref : array<float>
    var kraw_ref : array<float>
    block_ref(qa, x, pos, cossin, xb2_ref, kraw_ref)
    check_row(t, "{tag} pos {pos} layer A wo row", xb2, xb2_ref)
    check_row(t, "{tag} pos {pos} layer A raw k row", kout, kraw_ref)
    // the host residual, then the tail: FFN A + combine + layer B's feed on the device
    for (i in range64(N)) {
        x[i] += xb2[i]
    }
    var x1_ref := x
    var fd : array<float>
    var gxq1 : array<int8>
    var gxs1 : array<float>
    feed_ref(x, f.norm, 1, fd, gxq1, gxs1)
    var gxq : array<int8>
    var gxs : array<float>
    k_copies(gxq1, gxs1, gxq, gxs)
    unsafe {
        matmul_moe_gpu_ffn_tail(f.offs1, f.offs3, f.offs2, K, gxq, gxs, N, NFE, K, 1, 1, 3, false, f.rw, x, qb.woq, qb.fmt,
            addr<float const?>(qb.norm[0]), EPS)
    }
    t |> success(moe_gpu_ffn_tail_pending(), "{tag} pos {pos}: the tail is pending until layer B's attention collects it")
    let pair <- [0l, 1l]   // the tail arm routes slot j to expert j
    ffn_ref_sel(f, fd, pair, f.rw, x1_ref)
    // layer B on the block: its feed came from the tail; the host acts are ignored
    matmul_moe_gpu_attn_dec(xb2, kout, vout, qb.woq, qb.wok, qb.wov, qb.woo, qb.fmt, qb.fmt, qb.fmt, qb.fmt, xq, xs, cossin, null, null,
        pos, N, QD, KVD, HS, NH, KVM, ATTN_SCALE, EPS, qb.neox, false)
    t |> success(!moe_gpu_ffn_tail_pending(), "{tag} pos {pos}: layer B's attention collected the tail")
    check_row(t, "{tag} pos {pos} residual row after FFN A (the tail's combine)", x, x1_ref)
    var xb2b_ref : array<float>
    var krawb_ref : array<float>
    block_ref(qb, x, pos, cossin, xb2b_ref, krawb_ref)
    check_row(t, "{tag} pos {pos} layer B wo row (fed by the tail)", xb2, xb2b_ref)
    check_row(t, "{tag} pos {pos} layer B raw k row", kout, krawb_ref)
    if (pos == 0l) {
        var poison := xb2   // the bar's control: one element pushed past both bars must red
        poison[0] += 1.0 + 2.0 * max_abs(xb2b_ref)
        t |> success(count_bad(poison, xb2b_ref, ABS_BAR, REL_BAR * max_abs(xb2b_ref)) == 1, "{tag}: the bars catch a poisoned element")
    }
}

// one quad pair in one plane format through two positions; false = no device
def private pair_cell(t : T?; f : Ffn; fmt : int; base : int64; tag : string; neox : bool) : bool {
    var qa : Quad
    var qb : Quad
    if (!quad_up(qa, fmt, base, 21l + int64(fmt), 0.3, neox)) {
        return false
    }
    t |> success(quad_up(qb, fmt, base + 10l * STRIDE, 33l + int64(fmt), 0.7, neox), "{tag}: layer B's quad uploaded")
    let gen = moe_gpu_attn_dec_claim(true)
    // a layer the block has never seen hydrates first (zero rows at position 0 creates it), then serves
    t |> success(moe_gpu_attn_dec_ok(qa.woq, 0l, gen) == DAT_HYDRATE, "{tag}: an unseen layer asks for a hydrate at position 0")
    matmul_moe_gpu_attn_dec_hydrate(qa.woq, null, null, 0l, KVD, true)
    matmul_moe_gpu_attn_dec_hydrate(qb.woq, null, null, 0l, KVD, true)
    t |> success(moe_gpu_attn_dec_ok(qa.woq, 0l, gen) == DAT_SERVE && moe_gpu_attn_dec_ok(qb.woq, 0l, gen) == DAT_SERVE,
        "{tag}: both layers serve position 0 after the empty hydrate")
    for (pos in range64(2)) {
        token_cell(t, qa, qb, f, pos, tag)
    }
    t |> success(moe_gpu_attn_dec_ok(qa.woq, 2l, gen) == DAT_SERVE && moe_gpu_attn_dec_ok(qb.woq, 2l, gen) == DAT_SERVE,
        "{tag}: both mirrors hold [0, 2) after two positions")
    // the hydrate arms: a fresh claim empties both mirrors; A refills from f32 rows, B from f16 rows
    let gen2 = moe_gpu_attn_dec_claim(true)
    t |> success(moe_gpu_attn_dec_ok(qa.woq, 2l, gen2) == DAT_HYDRATE, "{tag}: a fresh claim empties the mirrors - position 2 asks for a hydrate")
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
        matmul_moe_gpu_attn_dec_hydrate(qa.woq, addr<uint8 const?>(qa.kmir[0]), addr<uint8 const?>(qa.vmir[0]), 2l, KVD, false)
        matmul_moe_gpu_attn_dec_hydrate(qb.woq, addr<uint8 const?>(kb[0]), addr<uint8 const?>(vb[0]), 2l, KVD, true)
    }
    t |> success(moe_gpu_attn_dec_ok(qa.woq, 2l, gen2) == DAT_SERVE && moe_gpu_attn_dec_ok(qb.woq, 2l, gen2) == DAT_SERVE,
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
            hrow[i] = silu(gg[i]) * u[i]
        }
        var hq : array<int8>
        var hsc : array<float>
        var hbs : array<int>
        hq |> resize(NFE)
        hsc |> resize(NFE / 256l)
        hbs |> resize(NFE / 16l)
        quantize_q8_k_into(hrow, NFE, hq, hsc, hbs, 0l, 0l, 0l)
        for (i in range64(NFE)) {
            hrow[i] = float(hq[i]) * hsc[i / 256l]
        }
        for (i in range64(N * NFE)) {
            wsl[i] = f.w2[e * ege + i]
        }
        var d : array<float>
        gemv_ref(wsl, N, NFE, hrow, d)
        for (i in range64(N)) {
            acc[i] += w[j] * d[i]
        }
    }
    for (i in range64(N)) {
        x1[i] += acc[i]
    }
}

// one span layer's reference: the block, the residual, the router on the FFN-normed row (f32),
// the host select (softmax, top-k, renorm), the routed FFN on the requantized feed
def private span_layer_ref(var q : Quad; f : Ffn; router : array<float>; li : int64; var x : array<float>; pos : int64; cossin : array<float>; var kraw : array<float>) {
    var xb2 : array<float>
    block_ref(q, x, pos, cossin, xb2, kraw)
    for (i in range64(N)) {
        x[i] += xb2[i]
    }
    var xb : array<float>
    xb |> resize(N)
    rmsnorm(xb, x, f.norm, N, EPS)
    var lg : array<float>
    lg |> resize(NE)
    for (e in range64(NE)) {
        var acc = 0.0
        for (i in range64(N)) {
            acc += router[(li * NE + e) * N + i] * xb[i]
        }
        lg[e] = acc
    }
    var idx : array<int64>
    var w : array<float>
    idx |> resize(K)
    w |> resize(K)
    unsafe {
        moe_select_core(MoeGate.softmax, NE, K, true, 1.0, addr(lg[0]), addr(idx[0]), addr(w[0]), 0l)
    }
    var fd : array<float>
    var gxq1 : array<int8>
    var gxs1 : array<float>
    feed_ref(x, f.norm, 1, fd, gxq1, gxs1)
    ffn_ref_sel(f, fd, idx, w, x)
}

// the span's planes: router rows (2 layers x NE x N), the [A | B] attention norms, the FFN norm twice
def private span_planes(qa, qb : Quad; f : Ffn; var router : array<float>; var att_norms : array<float>; var ffn_norms : array<float>) {
    router |> resize(2l * NE * N)
    for (i in range64(2l * NE * N)) {
        router[i] = 0.02 * sin(float(i) * 0.61 + 0.3) + 0.01 * cos(float(i / N) * 2.1)
    }
    att_norms |> resize(2l * N)
    ffn_norms |> resize(2l * N)
    for (i in range64(N)) {
        att_norms[i] = qa.norm[i]
        att_norms[N + i] = qb.norm[i]
        ffn_norms[i] = f.norm[i]
        ffn_norms[N + i] = f.norm[i]
    }
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
    return <- a
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
        unsafe {
            a.blob = addr<float const?>(ma.fblob[0])
        }
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
        t |> success(quad_up(qa, 1, 60l * STRIDE, 51l, 0.4, true), "dec span: layer A's quad uploaded")
        t |> success(quad_up(qb, 1, 70l * STRIDE, 63l, 0.8, true), "dec span: layer B's quad uploaded")
        let gen = moe_gpu_attn_dec_claim(true)
        matmul_moe_gpu_attn_dec_hydrate(qa.woq, null, null, 0l, KVD, true)
        matmul_moe_gpu_attn_dec_hydrate(qb.woq, null, null, 0l, KVD, true)
        t |> success(moe_gpu_attn_dec_ok(qa.woq, 0l, gen) == DAT_SERVE && moe_gpu_attn_dec_ok(qb.woq, 0l, gen) == DAT_SERVE,
            "dec span: both layers serve position 0")
        var router : array<float>
        var att_norms : array<float>
        var ffn_norms : array<float>
        span_planes(qa, qb, f, router, att_norms, ffn_norms)
        var a <- span_args(qa, qb, f)
        var kout : array<float>
        var vout : array<float>
        kout |> resize(2l * KVD)
        vout |> resize(2l * KVD)
        var no_freqs : array<float>
        var cossin : array<float>
        cossin |> resize(HS)
        for (pos in range64(2)) {
            build_rope_row_packed(cossin, 0l, pos, HS, 10000.0, 1.0, 1.0, no_freqs)
            var x : array<float>
            fill_row(x, N, 0.5 + 0.7 * float(pos))
            var x_ref := x
            var xd : array<float>
            var xq : array<int8>
            var xs : array<float>
            feed_ref(x, qa.norm, 1, xd, xq, xs)
            unsafe {
                a.pos = pos
                a.router = addr<float const?>(router[0])
                a.att_norm = addr<float const?>(att_norms[0])
                a.ffn_norm = addr<float const?>(ffn_norms[0])
                a.xq = addr<int8 const?>(xq[0])
                a.xs = addr<float const?>(xs[0])
                a.cossin = addr<float const?>(cossin[0])
                a.x = addr(x[0])
                a.kout = addr(kout[0])
                a.vout = addr(vout[0])
            }
            matmul_moe_gpu_span_dec(a)
            var kraw_a : array<float>
            var kraw_b : array<float>
            span_layer_ref(qa, f, router, 0l, x_ref, pos, cossin, kraw_a)
            span_layer_ref(qb, f, router, 1l, x_ref, pos, cossin, kraw_b)
            var ka : array<float>
            var kb : array<float>
            ka |> resize(KVD)
            kb |> resize(KVD)
            for (i in range64(KVD)) {
                ka[i] = kout[i]
                kb[i] = kout[KVD + i]
            }
            check_row(t, "span pos {pos} layer A raw k row", ka, kraw_a)
            check_row(t, "span pos {pos} layer B raw k row", kb, kraw_b)
            check_row(t, "span pos {pos} residual row after both layers", x, x_ref)
        }
        t |> success(moe_gpu_attn_dec_ok(qa.woq, 2l, gen) == DAT_SERVE && moe_gpu_attn_dec_ok(qb.woq, 2l, gen) == DAT_SERVE,
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
        t |> success(pair_cell(t, f, 1, 10l * STRIDE, "k4 quads (Q8_K feed, neox rope)", true), "dec tail: the k4 pair uploaded")
        t |> success(pair_cell(t, f, 0, 40l * STRIDE, "q8 quads (Q8_0 feed, classic rope)", false), "dec tail: the q8 pair uploaded")
    } else {
        t |> skip("dasVulkan not installed; the decode block and tail are untestable here")
    }
}
