options gen2
options stack = 524288
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama_tower
require dasllama/dasllama_audio
require dasllama/dasllama_tts_blocks
require dasllama/dasllama_math
require dasllama/dasllama_plane
require daslib/jobque_boost
require math
require _compares   // nan_plane: the outputs' sentinel fill

//! Model-free CPU units for the public tower, audio and TTS kernels the ASR and TTS paths run: every bar is an in-test fp64 reference over a seeded-LCG fixture, and every bar carries a must-EXCEED poison.
var private g_lcg = 0x2545F4914F6CDD1Dul

[sideeffects]
def private lcg_seed(s : uint64) {
    g_lcg = s * 2862933555777941757ul + 3037000493ul
}

[sideeffects]
def private lcg_unit() : float {
    g_lcg = g_lcg * 6364136223846793005ul + 1442695040888963407ul
    return float(uint((g_lcg >> 40ul) & 0xFFFFFFul)) / 8388608.0 - 1.0
}

def private fill(var a : array<float>; n : int64; scale : float) {
    a |> resize(n)
    for (v in a) {
        v = lcg_unit() * scale
    }
}

//! `got` within `rel` of each element's fp64 envelope `env`, floored at `floor`; the control moves the middle expected element
//! by `delta`
def private held_env(t : T?; name : string; got : array<float>; want : array<double>; env : array<double>; rel, floor, delta : double) {
    check(t, name, got, want, Bar(kind = BarKind.envelope, rel = float(rel), abs_floor = float(floor), poison = float(delta), poison_at = length(want) / 2), FLAT_SHAPE, env)
}

//! `got` bit for bit `want`; the control moves the middle expected element by `delta`
def private held_exact(t : T?; name : string; got : array<float>; want : array<float>; delta : float) {
    check(t, name, got, want, Bar(exact = true, poison = delta, poison_at = length(want) / 2), FLAT_SHAPE)
}

//! `a` borrowed as the plane of its element type: `PlaneF` over floats, `PlaneU16` over bf16 words
def private borrowed(a : array<auto(TT)>) {
    static_if (typeinfo is_float(type<TT>)) {
        return PlaneF(p = unsafe(addr<float const?>(a[0])), n = long_length(a))
    } else {
        return PlaneU16(p = unsafe(addr<uint16 const?>(a[0])), n = long_length(a))
    }
}

let MM_N = 128l
let MM_D = 96l
let MM_T = 40l
let MM_WOFF = 64l
let MM_NPAD = 192l
let MM_REL = 2.0e-5lf
let MM_FLOOR = 1.0e-6lf

def private gemm_ref(w : array<float>; woff : int64; x : array<float>; n : int64; d : int64; npos : int64;
                     var want : array<double>; var env : array<double>) {
    want |> resize(npos * d)
    env |> resize(npos * d)
    for (p in range64(npos)) {
        for (j in range64(d)) {
            var acc = 0.0lf
            var mag = 0.0lf
            for (i in range64(n)) {
                let prod = double(w[woff + j * n + i]) * double(x[p * n + i])
                acc += prod
                mag += abs(prod)
            }
            want[p * d + j] = acc
            env[p * d + j] = mag
        }
    }
}

def private mm_fixture(var blob : array<float>; var x : array<float>) {
    lcg_seed(0xA5A5ul)
    fill(blob, MM_WOFF + MM_N * MM_D, 1.0)
    fill(x, MM_T * MM_N, 1.0)
}

def private mm_bf16_fixture(var wq : array<uint16>; var wf : array<float>; var x : array<float>) {
    lcg_seed(0xB16Bul)
    fill(wf, MM_WOFF + MM_N * MM_D, 1.0)
    wq |> resize(long_length(wf))
    for (f, q in wf, wq) {
        q = bf16_bits_truncated(f)
        f = bf16_value(q)
    }
    fill(x, MM_T * MM_N, 1.0)
    for (v in x) {
        v = bf16_value(bf16_bits_truncated(v))
    }
}

def private mm_blob_run(blob : array<float>; x : array<float>) : array<float> {
    var y <- nan_plane(int(MM_T * MM_D))
    mm_blob_b(y, blob, MM_WOFF, x, MM_N, MM_D, MM_T)
    return <- y
}

def private mm_blob_case(t : T?) {
    var blob : array<float>
    var x : array<float>
    mm_fixture(blob, x)
    var want : array<double>
    var env : array<double>
    gemm_ref(blob, MM_WOFF, x, MM_N, MM_D, MM_T, want, env)
    let y <- mm_blob_run(blob, x)
    held_env(t, "mm_blob_b blob+woff", y, want, env, MM_REL, MM_FLOOR, 0.05lf)
}

def private mm_plane_case(t : T?) {
    var blob : array<float>
    var x : array<float>
    mm_fixture(blob, x)
    let y_arr <- mm_blob_run(blob, x)
    var y_pl <- nan_plane(int(MM_T * MM_D))
    mm_blob_b(y_pl, borrowed(blob), MM_WOFF, x, MM_N, MM_D, MM_T)
    held_exact(t, "mm_blob_b borrowed plane == owned blob", y_pl, y_arr, 1.0e-5)
}

def private mm_bf16_case(t : T?) {
    var wq : array<uint16>
    var wf : array<float>
    var x : array<float>
    mm_bf16_fixture(wq, wf, x)
    var want : array<double>
    var env : array<double>
    gemm_ref(wf, MM_WOFF, x, MM_N, MM_D, MM_T, want, env)
    var y <- nan_plane(int(MM_T * MM_D))
    mm_bf16_b(y, borrowed(wq), MM_WOFF, x, MM_N, MM_D, MM_T)
    held_env(t, "mm_bf16_b on bf16-exact operands", y, want, env, MM_REL, MM_FLOOR, 0.05lf)
}

def private mm_route_case(t : T?; bf16 : bool) {
    var wq : array<uint16>
    var wf : array<float>
    var x : array<float>
    mm_bf16_fixture(wq, wf, x)
    var want <- nan_plane(int(MM_T * MM_D))
    var got <- nan_plane(int(MM_T * MM_D))
    if (bf16) {
        mm_bf16_b(want, borrowed(wq), MM_WOFF, x, MM_N, MM_D, MM_T)
    } else {
        mm_blob_b(want, borrowed(wf), MM_WOFF, x, MM_N, MM_D, MM_T)
    }
    mm_plane_b(got, bf16, borrowed(wq), borrowed(wf), MM_WOFF, x, MM_N, MM_D, MM_T)
    held_exact(t, "mm_plane_b bf16={bf16} routes to its wrapper", got, want, 1.0e-5)
}

def private mm_pad_case(t : T?) {
    lcg_seed(0x9AD0ul)
    var w : array<float>
    var x : array<float>
    fill(w, MM_N * MM_D, 1.0)
    fill(x, MM_T * MM_N, 1.0)
    var wp : array<float>
    var xp : array<float>
    wp |> resize(MM_NPAD * MM_D)
    xp |> resize(MM_T * MM_NPAD)
    for (j in range64(MM_D)) {
        for (i in range64(MM_N)) {
            wp[j * MM_NPAD + i] = w[j * MM_N + i]
        }
    }
    for (p in range64(MM_T)) {
        for (i in range64(MM_N)) {
            xp[p * MM_NPAD + i] = x[p * MM_N + i]
        }
    }
    var y_tight <- nan_plane(int(MM_T * MM_D))
    var y_pad <- nan_plane(int(MM_T * MM_D))
    mm_blob_b(y_tight, w, 0l, x, MM_N, MM_D, MM_T)
    mm_blob_b(y_pad, wp, 0l, xp, MM_NPAD, MM_D, MM_T)
    held_exact(t, "mm_blob_b zero-padded width {MM_NPAD} == unpadded {MM_N}", y_pad, y_tight, 1.0e-5)
}

let LN_DIM = 197l
let LN_POS = 33l
let LN_OFF = 8l
let LN_EPS = 1.0e-5
let LN_REL = 1.0e-5lf
let LN_FLOOR = 1.0e-6lf

def private ln_fixture(var x : array<float>; var wb : array<float>) {
    lcg_seed(0x1EAFul)
    fill(x, LN_POS * LN_DIM, 3.0)
    fill(wb, LN_OFF + 2l * LN_DIM, 1.0)
}

def private ln_ref(x : array<float>; wb : array<float>; var want : array<double>; var env : array<double>) {
    want |> resize(LN_POS * LN_DIM)
    env |> resize(LN_POS * LN_DIM)
    for (p in range64(LN_POS)) {
        let off = p * LN_DIM
        var mean = 0.0lf
        for (i in range64(LN_DIM)) {
            mean += double(x[off + i])
        }
        mean /= double(LN_DIM)
        var vr = 0.0lf
        for (i in range64(LN_DIM)) {
            let dv = double(x[off + i]) - mean
            vr += dv * dv
        }
        vr /= double(LN_DIM)
        let scale = 1.0lf / sqrt(vr + double(LN_EPS))
        for (i in range64(LN_DIM)) {
            let nrm = (double(x[off + i]) - mean) * scale * double(wb[LN_OFF + i])
            let bias = double(wb[LN_OFF + LN_DIM + i])
            want[off + i] = nrm + bias
            env[off + i] = abs(nrm) + abs(bias)
        }
    }
}

def private ln_scalar_run(x : array<float>; wb : array<float>) : array<float> {
    var out <- nan_plane(int(LN_POS * LN_DIM))
    unsafe {
        var op = addr(out[0])
        let ip = addr<float const?>(x[0])
        let wp = addr<float const?>(wb[LN_OFF])
        let bp = addr<float const?>(wb[LN_OFF + LN_DIM])
        for (p in range64(LN_POS)) {
            layernorm(op + p * LN_DIM, ip + p * LN_DIM, wp, bp, LN_DIM, LN_EPS)
        }
    }
    return <- out
}

def private ln_scalar_case(t : T?) {
    var x : array<float>
    var wb : array<float>
    ln_fixture(x, wb)
    var want : array<double>
    var env : array<double>
    ln_ref(x, wb, want, env)
    let out <- ln_scalar_run(x, wb)
    held_env(t, "layernorm row form", out, want, env, LN_REL, LN_FLOOR, 0.01lf)
}

def private ln_batch_case(t : T?) {
    var x : array<float>
    var wb : array<float>
    ln_fixture(x, wb)
    var want : array<double>
    var env : array<double>
    ln_ref(x, wb, want, env)
    var out <- nan_plane(int(LN_POS * LN_DIM))
    layernorm_batch(out, x, wb, LN_OFF, LN_OFF + LN_DIM, LN_DIM, LN_POS, LN_EPS)
    held_env(t, "layernorm_batch blob+offsets", out, want, env, LN_REL, LN_FLOOR, 0.01lf)
}

def private ln_plane_case(t : T?) {
    var x : array<float>
    var wb : array<float>
    ln_fixture(x, wb)
    var y_arr <- nan_plane(int(LN_POS * LN_DIM))
    var y_pl <- nan_plane(int(LN_POS * LN_DIM))
    layernorm_batch(y_arr, x, wb, LN_OFF, LN_OFF + LN_DIM, LN_DIM, LN_POS, LN_EPS)
    layernorm_batch(y_pl, x, borrowed(wb), LN_OFF, LN_OFF + LN_DIM, LN_DIM, LN_POS, LN_EPS)
    held_exact(t, "layernorm_batch borrowed plane == owned blob", y_pl, y_arr, 1.0e-5)
}

let AB_D = 37l
let AB_POS = 41l
let AB_OFF = 5l
let AI_N = 4093l

def private ab_fixture(var y : array<float>; var blob : array<float>) {
    lcg_seed(0xB1A5ul)
    fill(y, AB_POS * AB_D, 2.0)
    fill(blob, AB_OFF + AB_D, 1.0)
}

def private ab_ref(y : array<float>; blob : array<float>) : array<float> {
    var want : array<float>
    want |> resize(AB_POS * AB_D)
    for (p in range64(AB_POS)) {
        for (i in range64(AB_D)) {
            want[p * AB_D + i] = y[p * AB_D + i] + blob[AB_OFF + i]
        }
    }
    return <- want
}

def private add_bias_case(t : T?; as_plane : bool) {
    var y : array<float>
    var blob : array<float>
    ab_fixture(y, blob)
    let want <- ab_ref(y, blob)
    if (as_plane) {
        add_bias_rows(y, borrowed(blob), AB_OFF, AB_D, AB_POS)
    } else {
        add_bias_rows(y, blob, AB_OFF, AB_D, AB_POS)
    }
    held_exact(t, "add_bias_rows plane={as_plane} d={AB_D} off the float4 block", y, want, 1.0e-5)
}

def private add_inplace_case(t : T?) {
    lcg_seed(0xADD1ul)
    var x : array<float>
    var r : array<float>
    fill(x, AI_N, 2.0)
    fill(r, AI_N, 1.0)
    var want : array<float>
    want |> resize(AI_N)
    for (a, b, w in x, r, want) {
        w = a + b
    }
    add_inplace_rows(x, r, AI_N)
    held_exact(t, "add_inplace_rows n={AI_N} off the float4 block", x, want, 1.0e-5)
}

let GE_D = 64l
let GE_POS = 25l
let GE_REL = 1.0e-6lf
let GE_FLOOR = 1.0e-6lf

def private erf_ref(z : double) : double {
    var term = z
    var sum = z
    for (k in range(1, 60)) {
        term *= -z * z / double(k)
        sum += term / double(2 * k + 1)
    }
    return sum * 2.0lf / sqrt(DBL_PI)
}

def private gelu_fixture(var x : array<float>) {
    lcg_seed(0x9E10ul)
    fill(x, GE_D * GE_POS, 4.0)
}

def private gelu_ref(x : array<float>; var want : array<double>; var env : array<double>) {
    want |> resize(long_length(x))
    env |> resize(long_length(x))
    for (v, w, e in x, want, env) {
        let d = double(v)
        w = 0.5lf * d * (1.0lf + erf_ref(d * 0.7071067811865476lf))
        e = abs(d)
    }
}

def private gelu_batch_case(t : T?) {
    var x : array<float>
    gelu_fixture(x)
    var want : array<double>
    var env : array<double>
    gelu_ref(x, want, env)
    gelu_erf_batch(x, GE_D, GE_POS)
    held_env(t, "gelu_erf_batch vs the fp64 erf series", x, want, env, GE_REL, GE_FLOOR, 0.001lf)
}

def private gelu_shape_case(t : T?) {
    var x : array<float>
    gelu_fixture(x)
    var flat : array<float>
    flat := x
    gelu_erf_batch(x, GE_D, GE_POS)
    gelu_erf(flat, GE_D * GE_POS)
    held_exact(t, "gelu_erf_batch == the whole-image gelu_erf", x, flat, 1.0e-5)
}

let RP_POS = 17l
let RP_HEADS = 5l
let RP_HS = 16l
let RP_REL = 2.0e-6lf
let RP_FLOOR = 1.0e-7lf

def private rope_tabs(var ct : array<float>; var st : array<float>; live_row : int64) {
    let half = RP_HS / 2l
    ct |> resize(RP_POS * half)
    st |> resize(RP_POS * half)
    for (r in range64(RP_POS)) {
        for (j in range64(half)) {
            let live = live_row < 0l || r == live_row
            let ang = live ? 0.37 * float(r + 1l) / pow(2.0, float(j)) : 0.0
            ct[r * half + j] = cos(ang)
            st[r * half + j] = sin(ang)
        }
    }
}

def private rope_ref(v : array<float>; ct : array<float>; st : array<float>;
                     var want : array<double>; var env : array<double>) {
    let d = RP_HEADS * RP_HS
    let half = RP_HS / 2l
    want |> resize(RP_POS * d)
    env |> resize(RP_POS * d)
    for (r in range64(RP_POS)) {
        for (h in range64(RP_HEADS)) {
            let base = r * d + h * RP_HS
            for (j in range64(half)) {
                let c = double(ct[r * half + j])
                let s = double(st[r * half + j])
                let v0 = double(v[base + j])
                let v1 = double(v[base + j + half])
                want[base + j] = v0 * c - v1 * s
                want[base + j + half] = v0 * s + v1 * c
                env[base + j] = abs(v0 * c) + abs(v1 * s)
                env[base + j + half] = abs(v0 * s) + abs(v1 * c)
            }
        }
    }
}

def private rope_case(t : T?) {
    lcg_seed(0x40BEul)
    var v : array<float>
    var ct : array<float>
    var st : array<float>
    fill(v, RP_POS * RP_HEADS * RP_HS, 2.0)
    rope_tabs(ct, st, -1l)
    var want : array<double>
    var env : array<double>
    rope_ref(v, ct, st, want, env)
    rope_neox_tab_rows(v, RP_POS, RP_HEADS, RP_HS, ct, st)
    held_env(t, "rope_neox_tab_rows one table row per token, shared by every head", v, want, env, RP_REL, RP_FLOOR, 0.001lf)
}

def private rope_row_index_case(t : T?; live_row : int64) {
    lcg_seed(0x41BEul)
    var v : array<float>
    var ct : array<float>
    var st : array<float>
    fill(v, RP_POS * RP_HEADS * RP_HS, 2.0)
    let before := v
    rope_tabs(ct, st, live_row)
    rope_neox_tab_rows(v, RP_POS, RP_HEADS, RP_HS, ct, st)
    let d = RP_HEADS * RP_HS
    var got : array<float>
    var want : array<float>
    got |> resize((RP_POS - 1l) * d)
    want |> resize((RP_POS - 1l) * d)
    var o = 0l
    for (r in range64(RP_POS)) {
        if (r == live_row) {
            continue
        }
        for (i in range64(d)) {
            got[o * d + i] = v[r * d + i]
            want[o * d + i] = before[r * d + i]
        }
        o++
    }
    let nm = "rope_neox_tab_rows: only token {live_row}'s table row rotates"
    var moved = false
    for (i in range64(d)) {
        if (v[live_row * d + i] != before[live_row * d + i]) {
            moved = true
        }
    }
    t |> success(moved, "{nm}: the live row itself must rotate")
    held_exact(t, nm, got, want, 1.0e-5)
}

let AW_D = 32l
let AW_HEADS = 4l
let AW_SCALE = 0.35
let AW_REL = 1.0e-5lf
let AW_FLOOR = 1.0e-6lf

def private aw_win() : array<int64> {
    return <- [0l, 7l, 23l, 26l, 47l, 48l]
}

def private aw_fixture(win : array<int64>; var qq : array<float>; var kk : array<float>; var vv : array<float>) {
    lcg_seed(0x77A0ul)
    let rows = win[long_length(win) - 1l]
    fill(qq, rows * AW_D, 1.0)
    fill(kk, rows * AW_D, 1.0)
    fill(vv, rows * AW_D, 1.0)
    for (wi in range64(long_length(win) - 1l)) {
        let amp = 1.0 + 3.0 * float(wi)
        for (idx in range64(win[wi] * AW_D, win[wi + 1l] * AW_D)) {
            kk[idx] = kk[idx] * amp + float(wi)
            vv[idx] = vv[idx] * amp - float(wi)
        }
    }
}

def private aw_ref_head(qq : array<float>; kk : array<float>; vv : array<float>; var sc : array<double>;
                        var want : array<double>; var env : array<double>;
                        r0 : int64; wr : int64; hoff : int64; hs : int64) {
    for (p in range64(wr)) {
        var mx = -1.0e300lf
        for (q in range64(wr)) {
            var s = 0.0lf
            for (j in range64(hs)) {
                s += double(qq[(r0 + p) * AW_D + hoff + j]) * double(kk[(r0 + q) * AW_D + hoff + j])
            }
            sc[q] = s * double(AW_SCALE)
            mx = max(mx, sc[q])
        }
        var sum = 0.0lf
        for (q in range64(wr)) {
            sc[q] = exp(sc[q] - mx)
            sum += sc[q]
        }
        for (j in range64(hs)) {
            var acc = 0.0lf
            var mag = 0.0lf
            for (q in range64(wr)) {
                let wgt = sc[q] / sum
                let vq = double(vv[(r0 + q) * AW_D + hoff + j])
                acc += wgt * vq
                mag += wgt * abs(vq)
            }
            want[(r0 + p) * AW_D + hoff + j] = acc
            env[(r0 + p) * AW_D + hoff + j] = mag
        }
    }
}

def private aw_ref(qq : array<float>; kk : array<float>; vv : array<float>; win : array<int64>;
                   var want : array<double>; var env : array<double>) {
    let rows = win[long_length(win) - 1l]
    let hs = AW_D / AW_HEADS
    want |> resize(rows * AW_D)
    env |> resize(rows * AW_D)
    var sc : array<double>
    for (wi in range64(long_length(win) - 1l)) {
        let r0 = win[wi]
        let wr = win[wi + 1l] - r0
        sc |> resize(wr)
        for (h in range64(AW_HEADS)) {
            aw_ref_head(qq, kk, vv, sc, want, env, r0, wr, h * hs, hs)
        }
    }
}

def private aw_case(t : T?) {
    var win <- aw_win()
    var qq : array<float>
    var kk : array<float>
    var vv : array<float>
    aw_fixture(win, qq, kk, vv)
    var want : array<double>
    var env : array<double>
    aw_ref(qq, kk, vv, win, want, env)
    var out <- nan_plane(length(qq))
    var hp : array<float>
    var att : array<float>
    attention_bidir_windows(out, qq, kk, vv, hp, att, win, AW_D, AW_HEADS, AW_SCALE)
    held_env(t, "attention_bidir_windows vs an oracle attending strictly inside each ragged window", out, want, env, AW_REL, AW_FLOOR, 0.05lf)
}

let IG_REL = 5.0e-6lf
let IG_FLOOR = 1.0e-7lf

def private interp_ref(src : array<float>; src_off : int64; sw : int64; sh : int64; dw : int64; dh : int64;
                       d : int64; var want : array<double>; var env : array<double>) {
    want |> resize(dw * dh * d)
    env |> resize(dw * dh * d)
    let sf0 = float(dw) / float(sw)
    let sf1 = float(dh) / float(sh)
    let sup0 = max(1.0, 1.0 / sf0)
    let sup1 = max(1.0, 1.0 / sf1)
    let iv0 = 1.0 / sup0
    let iv1 = 1.0 / sup1
    for (i1 in range64(dh)) {
        let yc = (float(i1) + 0.5) / sf1
        let ylo = max(int64(yc - sup1 + 0.5), 0l)
        let yhi = min(int64(yc + sup1 + 0.5), sh)
        for (i0 in range64(dw)) {
            let xc = (float(i0) + 0.5) / sf0
            let xlo = max(int64(xc - sup0 + 0.5), 0l)
            let xhi = min(int64(xc + sup0 + 0.5), sw)
            let obase = (i1 * dw + i0) * d
            for (c in range64(d)) {
                want[obase + c] = 0.0lf
                env[obase + c] = 0.0lf
            }
            var tw = 0.0lf
            for (sy in range64(ylo, yhi)) {
                let wy = max(1.0 - abs((float(sy) - yc + 0.5) * iv1), 0.0)
                for (sx in range64(xlo, xhi)) {
                    let wgt = max(1.0 - abs((float(sx) - xc + 0.5) * iv0), 0.0) * wy
                    if (wgt <= 0.0) {
                        continue
                    }
                    let sbase = src_off + (sy * sw + sx) * d
                    for (c in range64(d)) {
                        let term = double(src[sbase + c]) * double(wgt)
                        want[obase + c] += term
                        env[obase + c] += abs(term)
                    }
                    tw += double(wgt)
                }
            }
            if (tw > 0.0lf) {
                for (c in range64(d)) {
                    want[obase + c] /= tw
                    env[obase + c] /= tw
                }
            }
        }
    }
}

def private interp_case(t : T?; sw : int64; sh : int64; dw : int64; dh : int64; d : int64; src_off : int64) {
    lcg_seed(0x1E40ul + uint64(dw * 131l + sh))
    var src : array<float>
    fill(src, src_off + sw * sh * d, 2.0)
    var want : array<double>
    var env : array<double>
    interp_ref(src, src_off, sw, sh, dw, dh, d, want, env)
    var out <- nan_plane(int(dw * dh * d))
    interpolate_grid_bilinear_aa(out, borrowed(src), src_off, sw, sh, dw, dh, d)
    held_env(t, "interpolate_grid_bilinear_aa {sw}x{sh} -> {dw}x{dh} d={d} at plane offset {src_off}", out, want, env, IG_REL, IG_FLOOR, 0.01lf)
}

let FFT_REL = 2.0e-6lf
let FFT_FLOOR = 1.0e-6lf
let DFT_N = 400l

def private dft_ref(reim : array<float>; n : int64; var want : array<double>; var env : array<double>) {
    want |> resize(2l * n)
    env |> resize(2l * n)
    var envsum = 0.0lf
    for (j in range64(n)) {
        envsum += abs(double(reim[2l * j])) + abs(double(reim[2l * j + 1l]))
    }
    for (k in range64(n)) {
        var acc_re = 0.0lf
        var acc_im = 0.0lf
        for (j in range64(n)) {
            let th = 2.0lf * DBL_PI * double((j * k) % n) / double(n)
            let cs = cos(th)
            let sn = sin(th)
            acc_re += double(reim[2l * j]) * cs + double(reim[2l * j + 1l]) * sn
            acc_im += double(reim[2l * j + 1l]) * cs - double(reim[2l * j]) * sn
        }
        want[2l * k] = acc_re
        want[2l * k + 1l] = acc_im
        env[2l * k] = envsum
        env[2l * k + 1l] = envsum
    }
}

def private fft_case(t : T?; n : int64) {
    lcg_seed(0xFF70ul + uint64(n))
    var reim : array<float>
    fill(reim, 2l * n, 1.0)
    var want : array<double>
    var env : array<double>
    dft_ref(reim, n, want, env)
    var p <- build_fft_plan(n)
    unsafe {
        fft_pow2_run(n, addr<int const?>(p.rev[0]), addr<float const?>(p.tw[0]), addr(reim[0]))
    }
    held_env(t, "fft_pow2_run n={n} vs the fp64 O(n^2) DFT of the same forward convention", reim, want, env, FFT_REL, FFT_FLOOR, 0.05lf)
}

def private fft_plan_tw_case(t : T?; n : int64) {
    let p <- build_fft_plan(n)
    var want : array<double>
    var env : array<double>
    want |> resize(n)
    env |> resize(n)
    for (k in range64(n / 2l)) {
        let th = 2.0lf * DBL_PI * double(k) / double(n)
        want[2l * k] = cos(th)
        want[2l * k + 1l] = -sin(th)
        env[2l * k] = 1.0lf
        env[2l * k + 1l] = 1.0lf
    }
    held_env(t, "build_fft_plan n={n} twiddles vs fp64 (cos, -sin)", p.tw, want, env, FFT_REL, FFT_FLOOR, 0.001lf)
}

def private fft_plan_rev_bad(n : int64) : int {
    let p <- build_fft_plan(n)
    var seen : array<bool>
    seen |> resize(n)
    var bad = 0
    for (i in range64(n)) {
        let r = int64(p.rev[i])
        if (r < 0l || r >= n || seen[r] || int64(p.rev[r]) != i) {
            bad++
        } else {
            seen[r] = true
        }
    }
    if (p.rev[0] != 0 || int64(p.rev[1]) != n / 2l) {
        bad++
    }
    return bad
}

def private fft_plan_refuses(n : int64) : bool {
    var tripped = false
    try {
        let p <- build_fft_plan(n)
        tripped = long_length(p.rev) < 0l
    } recover {
        tripped = true
    }
    return tripped
}

def private dft_twiddle_case(t : T?) {
    let n_bins = DFT_N / 2l + 1l
    let b <- build_dft_twiddles(DFT_N)
    var want : array<double>
    var env : array<double>
    want |> resize(DFT_N * n_bins * 2l)
    env |> resize(DFT_N * n_bins * 2l)
    for (j in range64(DFT_N)) {
        for (k in range64(n_bins)) {
            let th = 2.0lf * DBL_PI * double((j * k) % DFT_N) / double(DFT_N)
            let at = j * n_bins * 2l + k * 2l
            want[at] = cos(th)
            want[at + 1l] = -sin(th)
            env[at] = 1.0lf
            env[at + 1l] = 1.0lf
        }
    }
    held_env(t, "build_dft_twiddles n={DFT_N} vs fp64 (cos, -sin) at (j*k) mod n", b, want, env, FFT_REL, FFT_FLOOR, 0.001lf)
}

let RS_C = 5l
let RS_T = 41l
let RS_REL = 2.0e-6lf
let RS_FLOOR = 1.0e-7lf

def private rs_fixture(var x : array<float>; scale : float) {
    lcg_seed(0x2E51ul + uint64(int64(scale * 1024.0)))
    fill(x, RS_C * RS_T, 2.0)
}

def private rl_onnx_ref(x : array<float>; scale : float; var want : array<double>; var env : array<double>) : int64 {
    let t_out = int64(floor(float(RS_T) * scale))
    want |> resize(RS_C * t_out)
    env |> resize(RS_C * t_out)
    for (to in range64(t_out)) {
        let in_x = clamp((float(to) + 0.5) / scale - 0.5, 0.0, float(RS_T - 1l))
        let i1 = min(int64(in_x), RS_T - 1l)
        let i2 = min(i1 + 1l, RS_T - 1l)
        var dx1 = double(abs(in_x - float(i1)))
        var dx2 = double(abs(in_x - float(i2)))
        if (i1 == i2) {
            dx1 = 0.5lf
            dx2 = 0.5lf
        }
        for (ci in range64(RS_C)) {
            let a = double(x[ci * RS_T + i1])
            let b = double(x[ci * RS_T + i2])
            want[ci * t_out + to] = dx2 * a + dx1 * b
            env[ci * t_out + to] = dx2 * abs(a) + dx1 * abs(b)
        }
    }
    return t_out
}

def private rl_torch_ref(x : array<float>; scale : float; var want : array<double>; var env : array<double>) : int64 {
    let t_out = int64(floor(float(RS_T) * scale))
    want |> resize(RS_C * t_out)
    env |> resize(RS_C * t_out)
    let inv = double(1.0 / scale)
    for (to in range64(t_out)) {
        let src = max(0.0, float(inv * (double(to) + 0.5lf) - 0.5lf))
        let i0 = min(int64(src), RS_T - 1l)
        let i1 = min(i0 + 1l, RS_T - 1l)
        let l1 = double(src - float(i0))
        let l0 = 1.0lf - l1
        for (ci in range64(RS_C)) {
            let a = double(x[ci * RS_T + i0])
            let b = double(x[ci * RS_T + i1])
            want[ci * t_out + to] = l0 * a + l1 * b
            env[ci * t_out + to] = l0 * abs(a) + l1 * abs(b)
        }
    }
    return t_out
}

def private resize_case(t : T?; torch : bool; scale : float) {
    var x : array<float>
    rs_fixture(x, scale)
    var want : array<double>
    var env : array<double>
    let want_t = torch ? rl_torch_ref(x, scale, want, env) : rl_onnx_ref(x, scale, want, env)
    var y <- nan_plane(int(RS_C * want_t))
    let got_t = resize_linear(torch, x, RS_C, RS_T, scale, y)
    let nm = torch ? "resize_linear_torch (align_corners false) scale={scale}" : "resize_linear (ONNX half-pixel) scale={scale}"
    t |> equal(got_t, want_t, "{nm}: the output length")
    held_env(t, nm, y, want, env, RS_REL, RS_FLOOR, 0.001lf)
}

let MP_BINS = 13l
let MP_FRAMES = 37l
let MP_EPS = 1.0e-9
let MP_PHASE_ENV = 4.0lf
let MP_REL = 2.0e-6lf
let MP_FLOOR = 1.0e-6lf

[sideeffects]
def private neg_zero() : float {
    return unsafe(reinterpret<float>(0x80000000u))
}

def private mp_fixture(var re : array<float>; var im : array<float>) {
    lcg_seed(0x3A9Ful)
    fill(re, MP_BINS * MP_FRAMES, 2.0)
    fill(im, MP_BINS * MP_FRAMES, 2.0)
    re[0] = -1.5
    im[0] = 0.0
    re[1] = -1.5
    im[1] = neg_zero()
    re[2] = 2.5
    im[2] = 0.0
    re[3] = 2.5
    im[3] = neg_zero()
    re[4] = neg_zero()
    im[4] = neg_zero()
}

def private mp_ref(re : array<float>; im : array<float>; var want : array<double>; var env : array<double>) {
    let nb = MP_BINS * MP_FRAMES
    want |> resize(2l * nb)
    env |> resize(2l * nb)
    for (i in range64(nb)) {
        let r = double(re[i])
        let m = double(im[i])
        let mag = sqrt(r * r + m * m + double(MP_EPS))
        want[i] = mag
        env[i] = mag
        want[nb + i] = im[i] == 0.0 ? (re[i] < 0.0 ? DBL_PI : 0.0lf) : atan2(m, r)
        env[nb + i] = MP_PHASE_ENV
    }
}

def private mp_case(t : T?) {
    var re : array<float>
    var im : array<float>
    mp_fixture(re, im)
    var want : array<double>
    var env : array<double>
    mp_ref(re, im, want, env)
    var out <- nan_plane(int(2l * MP_BINS * MP_FRAMES))
    magnitude_phase(re, im, MP_BINS, MP_FRAMES, MP_EPS, out)
    held_env(t, "magnitude_phase {MP_BINS}x{MP_FRAMES} with the zero-imaginary branch planted", out, want, env, MP_REL, MP_FLOOR, 0.01lf)
}

let IS_REL = 4.0e-6lf
let IS_FLOOR = 1.0e-7lf

def private is_window(var w : array<float>; k : int64; zeroed : bool) {
    w |> resize(k)
    for (v in w) {
        let u = 0.4 + 0.6 * abs(lcg_unit())
        v = zeroed ? 0.0 : u
    }
}

def private istft_ref(wave0 : array<float>; n : int64; window : array<float>; k : int64; hop : int64;
                      pad : int64; var want : array<double>; var env : array<double>) {
    want |> resize(n)
    env |> resize(n)
    for (i in range64(n)) {
        let m = i + pad
        var acc = 0.0lf
        var f = (m - k + hop) / hop
        if (f < 0l) {
            f = 0l
        }
        while (f * hop <= m) {
            let j = m - f * hop
            if (j < k) {
                acc += double(window[j]) * double(window[j])
            }
            f++
        }
        let w0 = double(wave0[i])
        want[i] = acc > 1.0e-11lf ? w0 / acc : w0
        env[i] = abs(want[i])
    }
}

def private istft_case(t : T?; n : int64; k : int64; hop : int64; pad : int64; zeroed : bool) {
    lcg_seed(0x157Ful + uint64(k * 97l + hop))
    var window : array<float>
    is_window(window, k, zeroed)
    var wave : array<float>
    fill(wave, n, 2.0)
    let wave0 := wave
    var want : array<double>
    var env : array<double>
    istft_ref(wave0, n, window, k, hop, pad, want, env)
    istft_envelope_divide(wave, n, window, k, hop, pad)
    let nm = "istft_envelope_divide n={n} k={k} hop={hop} pad={pad} zero-window={zeroed}"
    held_env(t, nm, wave, want, env, IS_REL, IS_FLOOR, 0.01lf)
}

[test]
def test_tower_gemm_wrappers(t : T?) {
    with_dasllama_jobque() {
        t |> run("the wrappers against an fp64 GEMM, and the plane, per-tensor-flag and zero-padded forms against them") @(t : T?) {
            mm_blob_case(t)
            mm_bf16_case(t)
            mm_plane_case(t)
            mm_route_case(t, true)
            mm_route_case(t, false)
            mm_pad_case(t)
        }
    }
}

[test]
def test_tower_layernorm(t : T?) {
    with_dasllama_jobque() {
        t |> run("layernorm and layernorm_batch against the fp64 population-variance form") @(t : T?) {
            ln_scalar_case(t)
            ln_batch_case(t)
            ln_plane_case(t)
        }
    }
}

[test]
def test_tower_row_adds(t : T?) {
    with_dasllama_jobque() {
        t |> run("the bias and residual row adds against the scalar f32 loop") @(t : T?) {
            add_bias_case(t, false)
            add_bias_case(t, true)
            add_inplace_case(t)
        }
    }
}

[test]
def test_tower_gelu_erf_batch(t : T?) {
    with_dasllama_jobque() {
        t |> run("gelu_erf_batch against an fp64 erf series, and against its whole-image form") @(t : T?) {
            gelu_batch_case(t)
            gelu_shape_case(t)
        }
    }
}

[test]
def test_tower_rope_neox_tab_rows(t : T?) {
    with_dasllama_jobque() {
        t |> run("the full-head table rope against the fp64 rotation, and its per-row table index") @(t : T?) {
            rope_case(t)
            rope_row_index_case(t, 0l)
            rope_row_index_case(t, 11l)
        }
    }
}

[test]
def test_tower_attention_windows(t : T?) {
    with_dasllama_jobque() {
        t |> run("attention_bidir_windows against an oracle that attends strictly inside each window") @(t : T?) {
            aw_case(t)
        }
    }
}

[test]
def test_tower_interpolate_grid_aa(t : T?) {
    with_dasllama_jobque() {
        t |> run("the antialiased grid resample down and up, at a non-zero plane offset") @(t : T?) {
            interp_case(t, 48l, 48l, 14l, 14l, 8l, 0l)
            interp_case(t, 48l, 48l, 27l, 13l, 4l, 37l)
            interp_case(t, 4l, 4l, 9l, 9l, 3l, 11l)
        }
    }
}

[test]
def test_audio_fft_and_dft_tables(t : T?) {
    t |> run("the radix-2 plan's bit-reversal permutation and its twiddles") @(t : T?) {
        t |> equal(fft_plan_rev_bad(256l), 0, "build_fft_plan(256) rev is an involutive permutation with rev[1] = n/2")
        t |> equal(fft_plan_rev_bad(64l), 0, "build_fft_plan(64) rev is an involutive permutation with rev[1] = n/2")
        fft_plan_tw_case(t, 256l)
        t |> success(fft_plan_refuses(6l), "build_fft_plan refuses a non-power-of-2 size")
        t |> success(fft_plan_refuses(1l), "build_fft_plan refuses a size below 2")
    }
    t |> run("fft_pow2_run against an fp64 O(n^2) DFT of the same forward convention") @(t : T?) {
        fft_case(t, 64l)
        fft_case(t, 256l)
    }
    t |> run("the 400-point DFT twiddle matrix against fp64") @(t : T?) {
        dft_twiddle_case(t)
    }
}

[test]
def test_tts_linear_resamplers(t : T?) {
    t |> run("both interpolation conventions against an fp64 two-tap reference") @(t : T?) {
        resize_case(t, false, 2.0)
        resize_case(t, false, 1.5)
        resize_case(t, false, 0.5)
        resize_case(t, true, 2.0)
        resize_case(t, true, 1.5)
        resize_case(t, true, 0.5)
    }
}

[test]
def test_tts_magnitude_phase(t : T?) {
    t |> run("magnitude_phase against an fp64 reference, the zero-imaginary branch planted at both zero signs") @(t : T?) {
        mp_case(t)
    }
}

[test]
def test_tts_istft_envelope(t : T?) {
    t |> run("the istft window-envelope divide against an fp64 envelope, and its zero-envelope guard") @(t : T?) {
        istft_case(t, 200l, 32l, 8l, 16l, false)
        istft_case(t, 137l, 24l, 7l, 9l, false)
        istft_case(t, 96l, 32l, 8l, 0l, false)
        istft_case(t, 200l, 32l, 8l, 16l, true)
    }
}
