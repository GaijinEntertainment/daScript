//! The CPU rope APPLICATION leaves (dasllama_math's rope_scaled_* and rope_apply) against an in-test fp64 angle reference - a subject on every platform, not just the Apple Metal rope oracle.
options gen2
options stack = 524288
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama_math
require dasllama/dasllama_rope
require math
require _compares   // check: the rope compares' bar, log and poison; opaque: the fixture's taint seam

let ROPE_POISON = 0.05
let ROPE = Bar(abs_floor = 2.0e-3, poison = ROPE_POISON)   //! one rotation's f32 accumulation plus the f32 angle's own rounding, against the fp64 angles
let TWIN = Bar(abs_floor = 1.0e-5, poison = ROPE_POISON)   //! a tabled form against its un-tabled twin: the cross-compilation-unit cos/sin ulp drift
let SAME = Bar(exact = true, poison = ROPE_POISON)   //! two forms of one rotation, bit for bit

//! the compare of `n = hs x heads` rope outputs, named by head and dim
def private rope_check(t : T?; got, want : array<float>; hs : int64; bar : Bar; tag : string) {
    check(t, tag, got, want, bar, Shape(row_width = int(hs)))
}

def private ff_none() : array<float> {
    var e : array<float>
    return <- e
}

def private ff_ptr(ff : array<float>) : float const? {
    unsafe {
        return !empty(ff) ? addr<float const?>(ff[0]) : null
    }
}

def private fill_vec(var v : array<float>; n : int64) {
    v |> resize(n)
    for (i in range64(n)) {
        v[i] = opaque(1.3 * sin(float(i) * 0.31 + 0.2) - 0.7 * cos(float(i) * 0.077))
    }
}

def private copy_vec(src : array<float>; var dst : array<float>; n : int64) {
    dst |> resize(n)
    for (i in range64(n)) {
        dst[i] = src[i]
    }
}

def private ref_freq(hd, hs : int64; theta : float; ff : array<float>; has_ff : bool; fj : int64) : double {
    var f = 1.0lf / pow(double(theta), double(hd) / double(hs))
    if (has_ff) {
        f /= double(ff[fj])
    }
    return f
}

def private ref_norm(v : array<float>; var outv : array<float>; pos, hs, n : int64;
                     theta, fscale, mscale : float; ff : array<float>; has_ff : bool) {
    copy_vec(v, outv, n)
    let posf = double(pos) * double(fscale)
    for (p in range64(n / 2l)) {
        let i = p * 2l
        let hd = i % hs
        let a = posf * ref_freq(hd, hs, theta, ff, has_ff, hd / 2l)
        let fcr = cos(a) * double(mscale)
        let fci = sin(a) * double(mscale)
        let v0 = double(v[i])
        let v1 = double(v[i + 1l])
        outv[i] = float(v0 * fcr - v1 * fci)
        outv[i + 1l] = float(v0 * fci + v1 * fcr)
    }
}

def private ref_neox(v : array<float>; var outv : array<float>; pos, hs, rot, n : int64;
                     theta, fscale, mscale : float; ff : array<float>; has_ff : bool) {
    copy_vec(v, outv, n)
    let half = rot / 2l
    let posf = double(pos) * double(fscale)
    for (h in range64(n / hs)) {
        let base = h * hs
        for (j in range64(half)) {
            let a = posf * ref_freq(2l * j, rot, theta, ff, has_ff, j)
            let fcr = cos(a) * double(mscale)
            let fci = sin(a) * double(mscale)
            let v0 = double(v[base + j])
            let v1 = double(v[base + j + half])
            outv[base + j] = float(v0 * fcr - v1 * fci)
            outv[base + j + half] = float(v0 * fci + v1 * fcr)
        }
    }
}

//! the fp64 reference of the full-head rope `neox` names
def private ref_full(v : array<float>; var outv : array<float>; pos, hs, n : int64; theta, fscale, mscale : float; neox : bool) {
    var ff <- ff_none()
    if (neox) {
        ref_neox(v, outv, pos, hs, hs, n, theta, fscale, mscale, ff, false)
    } else {
        ref_norm(v, outv, pos, hs, n, theta, fscale, mscale, ff, false)
    }
    delete ff
}

//! the leaf's name for a compare tag: `rope_scaled` or `rope_scaled_neox`
def private leaf_name(neox : bool) : string => neox ? "rope_scaled_neox" : "rope_scaled"

//! `rope_scaled` or `rope_scaled_neox`, as `neox` names, against the fp64 angles
def private apply_gate(t : T?; pos, hs, nh : int64; theta, fscale, mscale : float; neox : bool) {
    let n = hs * nh
    var v : array<float>
    fill_vec(v, n)
    var want : array<float>
    ref_full(v, want, pos, hs, n, theta, fscale, mscale, neox)
    var ff <- ff_none()
    var got : array<float>
    copy_vec(v, got, n)
    if (neox) {
        rope_scaled_neox(got, pos, hs, n, theta, fscale, ff, mscale)
    } else {
        rope_scaled(got, pos, hs, n, theta, fscale, ff, mscale)
    }
    rope_check(t, got, want, hs, ROPE, "{leaf_name(neox)} hs={int(hs)} nh={int(nh)} pos={int(pos)}")
    delete v
    delete got
    delete want
    delete ff
}

[test]
def test_rope_apply_norm(t : T?) {
    for (pos in [0l, 1l, 7l, 37l, 512l]) {
        t |> run("rope_scaled at pos {pos}: head 64x2, 96x1 and the 40-wide tail head vs the fp64 angles") @(t : T?) {
            apply_gate(t, pos, 64l, 2l, 10000.0, 1.0, 1.0, false)
            apply_gate(t, pos, 96l, 1l, 1000000.0, 1.0, 1.0, false)
            apply_gate(t, pos, 40l, 3l, 10000.0, 0.5, 1.19, false)
        }
    }
}

[test]
def test_rope_apply_neox(t : T?) {
    for (pos in [0l, 1l, 7l, 37l, 512l]) {
        t |> run("rope_scaled_neox at pos {pos}: head 64x2, 96x1 and the 40-wide tail head vs the fp64 angles") @(t : T?) {
            apply_gate(t, pos, 64l, 2l, 10000.0, 1.0, 1.0, true)
            apply_gate(t, pos, 96l, 1l, 1000000.0, 1.0, 1.0, true)
            apply_gate(t, pos, 40l, 3l, 10000.0, 0.5, 1.19, true)
        }
    }
}

//! `rope_scaled_tab` or `rope_scaled_neox_tab`, as `neox` names, against the fp64 angles and against its un-tabled twin
def private tab_gate(t : T?; pos, hs, nh : int64; theta, fscale, mscale : float; neox : bool) {
    let n = hs * nh
    var v : array<float>
    fill_vec(v, n)
    var ff <- ff_none()
    var want : array<float>
    ref_full(v, want, pos, hs, n, theta, fscale, mscale, neox)
    var direct : array<float>
    copy_vec(v, direct, n)
    if (neox) {
        rope_scaled_neox(direct, pos, hs, n, theta, fscale, ff, mscale)
    } else {
        rope_scaled(direct, pos, hs, n, theta, fscale, ff, mscale)
    }
    var ct : array<float>
    var st : array<float>
    build_rope_tabs(ct, st, theta, fscale, mscale, ff, true, pos, 1l, hs)
    var got : array<float>
    copy_vec(v, got, n)
    unsafe {
        if (neox) {
            rope_scaled_neox_tab(addr(got[0]), hs, n, addr<float const?>(ct[0]), addr<float const?>(st[0]))
        } else {
            rope_scaled_tab(addr(got[0]), hs, n, addr<float const?>(ct[0]), addr<float const?>(st[0]))
        }
    }
    let leaf = leaf_name(neox)
    rope_check(t, got, want, hs, ROPE, "{leaf}_tab hs={int(hs)} nh={int(nh)} pos={int(pos)}")
    rope_check(t, got, direct, hs, TWIN, "{leaf}_tab twin of {leaf} hs={int(hs)} pos={int(pos)}")
    delete v
    delete got
    delete direct
    delete want
    delete ct
    delete st
    delete ff
}

[test]
def test_rope_apply_tab(t : T?) {
    for (pos in [0l, 1l, 37l, 512l]) {
        t |> run("rope_scaled_tab at pos {pos}: the fp64 angles, and the un-tabled twin") @(t : T?) {
            tab_gate(t, pos, 64l, 2l, 10000.0, 1.0, 1.0, false)
            tab_gate(t, pos, 40l, 3l, 10000.0, 0.5, 1.19, false)
        }
    }
}

[test]
def test_rope_apply_neox_tab(t : T?) {
    for (pos in [0l, 1l, 37l, 512l]) {
        t |> run("rope_scaled_neox_tab at pos {pos}: the fp64 angles, and the un-tabled twin") @(t : T?) {
            tab_gate(t, pos, 64l, 2l, 10000.0, 1.0, 1.0, true)
            tab_gate(t, pos, 40l, 3l, 10000.0, 0.5, 1.19, true)
        }
    }
}

def private part_gate(t : T?; pos, hs, rot, nh : int64; theta, fscale, mscale : float) {
    let n = hs * nh
    var v : array<float>
    fill_vec(v, n)
    var ff <- ff_none()
    var want : array<float>
    ref_neox(v, want, pos, hs, rot, n, theta, fscale, mscale, ff, false)
    var got : array<float>
    copy_vec(v, got, n)
    rope_scaled_neox_part(unsafe(addr(got[0])), pos, hs, rot, n, theta, fscale, false, null, mscale)
    rope_check(t, got, want, hs, ROPE, "rope_scaled_neox_part hs={int(hs)} rot={int(rot)} pos={int(pos)}")
    var compact : array<float>
    compact |> resize(nh * rot)
    for (h in range64(nh)) {
        for (j in range64(rot)) {
            compact[h * rot + j] = v[h * hs + j]
        }
    }
    rope_scaled_neox(compact, pos, rot, nh * rot, theta, fscale, ff, mscale)
    var full : array<float>
    copy_vec(v, full, n)
    for (h in range64(nh)) {
        for (j in range64(rot)) {
            full[h * hs + j] = compact[h * rot + j]
        }
    }
    rope_check(t, got, full, hs, SAME, "rope_scaled_neox_part == the full leaf over the rotated prefix hs={int(hs)} rot={int(rot)}")
    var tail_moved = 0
    for (h in range64(nh)) {
        for (j in range64(rot, hs)) {
            if (got[h * hs + j] != v[h * hs + j]) {
                tail_moved++
            }
        }
    }
    t |> success(tail_moved == 0, "the {int(hs - rot)} un-rotated dims of each head must pass through ({tail_moved} moved)")
    delete v
    delete got
    delete full
    delete want
    delete compact
    delete ff
}

//! the partial-rotary table twin against the fp64 reference and the direct part leaf; `ff` empty = no per-pair
//! factors, else rot/2 divisors (the YaRN fold on a qwen35-shaped head) that both forms must take alike
def private tab_part_gate(t : T?; pos, hs, rot, nh : int64; theta, fscale, mscale : float; ff : array<float>) {
    let n = hs * nh
    let has_ff = !empty(ff)
    let tag = has_ff ? " with factors" : ""
    var v : array<float>
    fill_vec(v, n)
    var want : array<float>
    ref_neox(v, want, pos, hs, rot, n, theta, fscale, mscale, ff, has_ff)
    var direct : array<float>
    copy_vec(v, direct, n)
    rope_scaled_neox_part(unsafe(addr(direct[0])), pos, hs, rot, n, theta, fscale, has_ff, ff_ptr(ff), mscale)
    rope_check(t, direct, want, hs, ROPE, "rope_scaled_neox_part{tag} hs={int(hs)} rot={int(rot)} pos={int(pos)}")
    var ct : array<float>
    var st : array<float>
    build_rope_tabs(ct, st, theta, fscale, mscale, ff, true, pos, 1l, rot)
    var got : array<float>
    copy_vec(v, got, n)
    unsafe {
        rope_scaled_neox_tab_part(addr(got[0]), hs, rot, n, addr<float const?>(ct[0]), addr<float const?>(st[0]))
    }
    rope_check(t, got, want, hs, ROPE, "rope_scaled_neox_tab_part{tag} hs={int(hs)} rot={int(rot)} pos={int(pos)}")
    rope_check(t, got, direct, hs, TWIN, "rope_scaled_neox_tab_part{tag} twin of the direct part form hs={int(hs)} rot={int(rot)}")
    delete v
    delete got
    delete direct
    delete want
    delete ct
    delete st
}

def private ff_ramp(half, period : int64) : array<float> {
    return <- [for (j in range64(half)); 1.0 + 0.5 * float(j % period)]
}

[test]
def test_rope_apply_part_factors(t : T?) {
    for (pos in [1l, 37l, 4096l]) {
        t |> run("partial rotary takes per-pair factors at pos {pos}: 64 of 256 (qwen35), 20 of 40") @(t : T?) {
            var ff64 <- ff_ramp(32l, 5l)
            var ff20 <- ff_ramp(10l, 5l)
            tab_part_gate(t, pos, 256l, 64l, 2l, 10000000.0, 1.0, 1.139, ff64)
            tab_part_gate(t, pos, 40l, 20l, 3l, 10000.0, 1.0, 1.0, ff20)
            delete ff64
            delete ff20
        }
    }
}

//! llama.cpp's YaRN band in fp64, as `rope_yarn_corr_dims` + `rope_yarn_ramp` spell it: the correction dims
//! over the rotated span, the ramp against the PAIR index (i0 / 2), the divisor 1 / (fscale + ramp (1 - fscale))
def private ref_yarn_divisors(rot : int64; theta, factor, beta_fast, beta_slow : double; orig_ctx : int64) : array<float> {
    let denom = 2.0lf * log(theta)
    let lo = max(0.0lf, floor(double(rot) * log(double(orig_ctx) / (beta_fast * 2.0lf * 3.14159265358979323846lf)) / denom))
    let hi = min(double(rot - 1l), ceil(double(rot) * log(double(orig_ctx) / (beta_slow * 2.0lf * 3.14159265358979323846lf)) / denom))
    let fscale = 1.0lf / factor
    return <- [for (j in range64(rot / 2l)); float(1.0lf / (fscale + (1.0lf - clamp((double(j) - lo) / max(0.001lf, hi - lo), 0.0lf, 1.0lf)) * (1.0lf - fscale)))]
}

[test]
def test_yarn_rope_freqs_band(tst : T?) {
    tst |> run("yarn_rope_freqs: the NTK-by-parts band of llama.cpp's rope_yarn, spelled out in fp64") @(t : T?) {
        let bar = 1.0e-5
        let band = Bar(abs_floor = bar, poison = ROPE_POISON)
        for (geom in [(rot = 64l, theta = 10000000.0, factor = 4.0, orig = 32768l), (rot = 64l, theta = 10000000.0, factor = 4.0, orig = 262144l),
                      (rot = 128l, theta = 150000.0, factor = 32.0, orig = 4096l), (rot = 20l, theta = 10000.0, factor = 2.5, orig = 512l)]) {
            var ff <- yarn_rope_freqs(geom.rot, geom.theta, geom.factor, 32.0, 1.0, geom.orig)
            var want <- ref_yarn_divisors(geom.rot, double(geom.theta), double(geom.factor), 32.0lf, 1.0lf, geom.orig)
            t |> equal(long_length(ff), geom.rot / 2l, "one divisor a pair (rot {geom.rot})")
            check(t, "yarn_rope_freqs rot={int(geom.rot)} x{geom.factor} over {geom.orig}", ff, want, band, FLAT_SHAPE)
            t |> success(abs(ff[0] - 1.0) < 1.0e-6 && abs(ff[geom.rot / 2l - 1l] - geom.factor) < 1.0e-5,
                "the highest pair keeps full frequency, the lowest interpolates by the factor ({ff[0]} .. {ff[geom.rot / 2l - 1l]})")
            // the poison: the band of a wider beta_fast sits elsewhere, so a ramp in the wrong units or at the wrong edge reads past the bar
            var shifted <- ref_yarn_divisors(geom.rot, double(geom.theta), double(geom.factor), 4.0lf, 1.0lf, geom.orig)
            t |> success(logits_maxdiff(ff, shifted) > bar, "a shifted band lands outside the bar (maxdiff {logits_maxdiff(ff, shifted)})")
            delete ff
            delete want
            delete shifted
        }
    }
}

[test]
def test_rope_apply_part(t : T?) {
    for (pos in [0l, 1l, 37l, 512l]) {
        t |> run("rope_scaled_neox_part at pos {pos}: 64 of 256 (qwen35), 32 of 96, 20 of 40") @(t : T?) {
            part_gate(t, pos, 256l, 64l, 2l, 1000000.0, 1.0, 1.0)
            part_gate(t, pos, 96l, 32l, 2l, 10000.0, 0.5, 1.19)
            part_gate(t, pos, 40l, 20l, 3l, 10000.0, 1.0, 1.0)
        }
        t |> run("rope_scaled_neox_tab_part at pos {pos}: the fp64 angles, and the direct twin") @(t : T?) {
            var none <- ff_none()
            tab_part_gate(t, pos, 256l, 64l, 2l, 1000000.0, 1.0, 1.0, none)
            tab_part_gate(t, pos, 40l, 20l, 3l, 10000.0, 1.0, 1.0, none)
            delete none
        }
    }
}

[test]
def test_rope_apply_dispatch(t : T?) {
    t |> run("rope_apply lands on the leaf its `neox` flag names, pointer and array forms alike") @(t : T?) {
        let hs = 64l
        let n = 128l
        let pos = 37l
        var v : array<float>
        fill_vec(v, n)
        var ff <- ff_none()
        for (neox in [false, true]) {
            var leaf : array<float>
            copy_vec(v, leaf, n)
            if (neox) {
                rope_scaled_neox(leaf, pos, hs, n, 10000.0, 1.0, ff, 1.19)
            } else {
                rope_scaled(leaf, pos, hs, n, 10000.0, 1.0, ff, 1.19)
            }
            var pform : array<float>
            copy_vec(v, pform, n)
            rope_apply(unsafe(addr(pform[0])), pos, hs, n, 10000.0, 1.0, false, null, neox, 1.19)
            rope_check(t, pform, leaf, hs, SAME, "rope_apply(ptr, neox={neox}) == the leaf")
            var aform : array<float>
            copy_vec(v, aform, n)
            rope_apply(aform, pos, hs, n, 10000.0, 1.0, ff, neox, 1.19, true)
            rope_check(t, aform, leaf, hs, SAME, "rope_apply(array, neox={neox}) == the leaf")
            delete leaf
            delete pform
            delete aform
        }
        delete v
        delete ff
    }
    t |> run("use_ff = false drops loaded freq-factors for this call (the gemma4 p-RoPE arm)") @(t : T?) {
        let hs = 64l
        let n = 128l
        let pos = 37l
        var v : array<float>
        fill_vec(v, n)
        var ff : array<float>
        ff |> resize(hs / 2l)
        for (j in range64(hs / 2l)) {
            ff[j] = 1.0 + 0.25 * float(j)
        }
        var none <- ff_none()
        for (neox in [false, true]) {
            var off : array<float>
            copy_vec(v, off, n)
            rope_apply(off, pos, hs, n, 10000.0, 1.0, ff, neox, 1.0, false)
            var bare : array<float>
            copy_vec(v, bare, n)
            rope_apply(bare, pos, hs, n, 10000.0, 1.0, none, neox, 1.0, true)
            rope_check(t, off, bare, hs, SAME, "use_ff=false (neox={neox}) == the no-factors apply")
            var on : array<float>
            copy_vec(v, on, n)
            rope_apply(on, pos, hs, n, 10000.0, 1.0, ff, neox, 1.0, true)
            var moved = 0
            for (i in range64(n)) {
                if (on[i] != off[i]) {
                    moved++
                }
            }
            t |> success(moved > 0, "the factors must actually move the rotation when use_ff is true (neox={neox})")
            delete off
            delete bare
            delete on
        }
        delete v
        delete ff
        delete none
    }
}

def private ff_gate(t : T?; pos, hs, nh : int64; neox : bool) {
    let n = hs * nh
    var v : array<float>
    fill_vec(v, n)
    var ff <- ff_ramp(hs / 2l, 7l)
    var want : array<float>
    if (neox) {
        ref_neox(v, want, pos, hs, hs, n, 500000.0, 1.0, 1.0, ff, true)
    } else {
        ref_norm(v, want, pos, hs, n, 500000.0, 1.0, 1.0, ff, true)
    }
    var got : array<float>
    copy_vec(v, got, n)
    rope_apply(unsafe(addr(got[0])), pos, hs, n, 500000.0, 1.0, true, ff_ptr(ff), neox, 1.0)
    rope_check(t, got, want, hs, ROPE, "freq-factors (neox={neox}) hs={int(hs)} pos={int(pos)}")
    var ones : array<float>
    ones |> resize(hs / 2l)
    for (c in ones) {
        c = 1.0
    }
    var none <- ff_none()
    var with_ones : array<float>
    copy_vec(v, with_ones, n)
    rope_apply(with_ones, pos, hs, n, 500000.0, 1.0, ones, neox, 1.0, true)
    var without : array<float>
    copy_vec(v, without, n)
    rope_apply(without, pos, hs, n, 500000.0, 1.0, none, neox, 1.0, true)
    rope_check(t, with_ones, without, hs, SAME, "all-ones factors (neox={neox}) must divide away exactly")
    delete v
    delete got
    delete want
    delete ff
    delete ones
    delete none
    delete with_ones
    delete without
}

[test]
def test_rope_apply_freq_factors(t : T?) {
    for (pos in [1l, 37l, 512l]) {
        t |> run("rope_freqs divide the per-pair frequency at pos {pos}, both conventions") @(t : T?) {
            ff_gate(t, pos, 64l, 2l, false)
            ff_gate(t, pos, 64l, 2l, true)
            ff_gate(t, pos, 40l, 3l, true)
        }
    }
}

[test]
def test_rope_tabs_no_rows(t : T?) {
    t |> run("a table over no positions is empty, and a table that held rows is cut to it") @(t : T?) {
        var ff <- ff_none()
        var ct : array<float>
        var st : array<float>
        build_rope_tabs(ct, st, 10000.0, 1.0, 1.0, ff, false, 0l, 2l, 64l)
        t |> equal(length(ct), 64, "two positions fill two rows of half the head")
        build_rope_tabs(ct, st, 10000.0, 1.0, 1.0, ff, false, 0l, 0l, 64l)
        t |> equal(length(ct), 0, "no positions, no cos rows")
        t |> equal(length(st), 0, "no positions, no sin rows")
        delete ct
        delete st
        delete ff
    }
}
