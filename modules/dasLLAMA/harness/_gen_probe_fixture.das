options gen2
options indenting = 4
options _jit_fast_math = true   // match gemm_1core_probe / the end-to-end config
options _dasllama_internal = true

module _gen_probe_fixture shared public

//! The q8 generator-grid fixture gen_tune_probe and gen_parity_probe share: the row-major
//! master, the per-token activations and their bsums, the hand laneq oracle, the tile and gemv
//! sweeps, and the tolerance compare.

require dasllama/dasllama_math_aarch64_neon
require dasllama/dasllama_repack
require dasllama/dasllama_convert
require math

let CLOSE_REL = 1e-4
let CLOSE_ABS = 1e-5

//! a bf16 tile rounds every weight and activation to bf16 before the dot: a row sits within 1e-2 of the image's largest magnitude
let BF16_ENVELOPE_REL = 1.0e-2

struct Fixture {
    n, d, ntok, nb : int64
    q : array<int8>       // row-major master — repacked per variant into a copy
    s : array<float>
    xq : array<int8>
    xs : array<float>
    xbs : array<int>      // −128·Σx per (token, block) — the bias128 tile's bsums plane
    yref : array<float>
}

//! token `tk`'s activation row: the index fill every generator fixture quantizes
def act_row(var xrow : array<float>; n, tk : int64) {
    for (i in range64(n)) {
        xrow[i] = float((i + tk * 7l) % 23l) * 0.07 - 0.8
    }
}

def build_fixture(n, d, ntok : int64) : Fixture {
    var fx = Fixture(n = n, d = d, ntok = ntok, nb = n / 32l)
    var w : array<float>
    w |> resize(n * d)
    for (i in range64(n * d)) {
        w[i] = float(i % 17l) * 0.13 - 1.1
    }
    fx.q |> resize(n * d)
    fx.s |> resize(d * fx.nb)
    quantize_q8_0_into(w, n * d, fx.q, fx.s, 0l, 0l)

    var xrow : array<float>
    xrow |> resize(n)
    fx.xq |> resize(ntok * n)
    fx.xs |> resize(ntok * fx.nb)
    for (tk in range64(ntok)) {
        act_row(xrow, n, tk)
        quantize_q8_0_into(xrow, n, fx.xq, fx.xs, tk * n, tk * fx.nb)
    }
    fx.xbs |> resize(ntok * fx.nb)
    for (tk in range64(ntok)) {
        for (bi in range64(fx.nb)) {
            var bsum = 0
            for (j in range64(32l)) {
                bsum += int(fx.xq[tk * n + bi * 32l + j])
            }
            fx.xbs[tk * fx.nb + bi] = -128 * bsum
        }
    }

    // the oracle: hand laneq semantics off a grp4 copy — the per-block fold order is the same
    // in every variant, so one oracle serves all layouts
    fx.yref |> resize(ntok * d)
    var q4 := fx.q
    var s4 := fx.s
    unsafe {
        repack_q8q8_grp(addr(q4[0]), addr(s4[0]), n, d, 4l)
        let wp = addr(q4[0])
        let sp = addr(s4[0])
        let xqp = addr(fx.xq[0])
        let xsp = addr(fx.xs[0])
        var yrp = addr(fx.yref[0])
        for (g in range64(d / 4l)) {
            let wgp = wp + g * 4l * n
            let sgp = sp + g * 4l * fx.nb
            for (tk in range64(ntok)) {
                let f = dot_q8q8_laneq4(wgp, sgp, xqp + tk * n, xsp + tk * fx.nb, n)
                yrp[tk * d + g * 4l + 0l] = f.x
                yrp[tk * d + g * 4l + 1l] = f.y
                yrp[tk * d + g * 4l + 2l] = f.z
                yrp[tk * d + g * 4l + 3l] = f.w
            }
        }
    }
    delete q4
    delete s4
    return <- fx
}

// one full tile sweep: every (ts-token, gs-group) unit through `tile`, on the variant's own
// repacked buffers (q/s); the fixture's bsums plane rides along (biased variants read it,
// everything else ignores the parameter). Fixtures divide evenly (ntok % ts == 0, groups %
// gs == 0) — production tails are the batch wrapper's job, not the tile's.
def run_tile(tile, cfg; fx : Fixture; q : array<int8>; s : array<float>; mr, ts, gs : int64; var y : array<float>) {
    let ng = fx.d / mr
    verify(fx.ntok % ts == 0l && ng % gs == 0l)
    invoke(cfg)     // latch rows: load the TMUL palette (the wrapper's per-chunk call)
    unsafe {
        let wp = addr(q[0])
        let sp = addr(s[0])
        let xqp = addr(fx.xq[0])
        let xsp = addr(fx.xs[0])
        let xbsp = addr(fx.xbs[0])
        var yp = addr(y[0])
        for (u in range64(ng / gs)) {
            let g = u * gs
            let wgp = wp + g * mr * fx.n
            let sgp = sp + g * mr * fx.nb
            var t0 = 0l
            while (t0 < fx.ntok) {
                invoke(tile, yp, wgp, sgp, xqp, xsp, xbsp, fx.n, fx.d, g, t0)
                t0 += ts
            }
        }
    }
}

// one gemv rows-core sweep over all group rows for token 0 (the group loop is inside the
// generated function — one call, like a production dispatch chunk), off any fixture carrying
// xq / xs, on the variant's own weight and scale planes `w` / `s`
def run_gemv(gemv; fx; w; s; mr : int64; var y : array<float>) {
    unsafe {
        invoke(gemv, addr(y[0]), addr(w[0]), addr(s[0]), addr(fx.xq[0]), addr(fx.xs[0]), fx.n, 0l, (fx.d / mr) * mr)
    }
}

//! run_gemv for a rows core that also reads the fixture's activation-sums plane xbs
def run_gemv_sums(gemv; fx; w; s; mr : int64; var y : array<float>) {
    unsafe {
        invoke(gemv, addr(y[0]), addr(w[0]), addr(s[0]), addr(fx.xq[0]), addr(fx.xs[0]), addr(fx.xbs[0]), fx.n, 0l, (fx.d / mr) * mr)
    }
}

//! the largest magnitude among the first `count` elements of `v`
def max_abs(v : array<float>; count : int) : float {
    var mag = 0.0
    for (i in range(count)) {
        mag = max(mag, abs(v[i]))
    }
    return mag
}

//! the element-relative bound a reference value `want` holds a kernel's output to
def close_tol(want : float) : float {
    return max(CLOSE_ABS, CLOSE_REL * abs(want))
}

//! whether each of the first `count` elements of `y` sits within `tol(i)` of `yref`; every
//! element's difference folds into `maxdiff`
def close_within(y, yref : array<float>; count : int; var maxdiff : float&; tol : block<(i : int) : float>) : bool {
    var ok = true
    for (i in range(count)) {
        let d = abs(y[i] - yref[i])
        maxdiff = max(maxdiff, d)
        ok &&= d <= invoke(tol, i)
    }
    return ok
}

//! the bound a bf16 tile form's compare holds: BF16_ENVELOPE_REL of the reference image's largest magnitude; 0 off a bf16 form
def bf16_envelope(yref : array<float>; count : int; form : int64) : float {
    return BF16_ENVELOPE_REL * (form > 0l ? max_abs(yref, count) : 0.0)
}

//! `y` against the fixture's reference image at close_tol, widened to the bf16 envelope on a bf16 tile form
def close_to_ref(fx; y : array<float>; var maxdiff : float&; form : int64 = 0l) : bool {
    let bf16Envelope = bf16_envelope(fx.yref, length(y), form)
    return close_within(y, fx.yref, length(y), maxdiff) $(i : int) {
        return max(close_tol(fx.yref[i]), bf16Envelope)
    }
}
