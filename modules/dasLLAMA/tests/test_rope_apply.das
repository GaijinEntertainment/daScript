//! The CPU rope APPLICATION leaves (dasllama_math's rope_scaled_* and rope_apply) against an in-test fp64 angle reference - a subject on every platform, not just the Apple Metal rope oracle.
options gen2
options stack = 524288
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama_math
require dasllama/dasllama_rope
require math

var g_taint = 0.0

[sideeffects]
def private opaque(x : float) : float => x + g_taint

let ROPE_BAR = 2.0e-3
let ROPE_POISON = 0.05
let TWIN_BAR = 1.0e-5

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

def private maxdiff(a : array<float>; b : array<float>; n : int64) : float {
    var worst = 0.0
    for (i in range64(n)) {
        worst = max(worst, abs(a[i] - b[i]))
    }
    return worst
}

def private poisoned_maxdiff(a : array<float>; b : array<float>; n : int64) : float {
    var worst = 0.0
    for (i in range64(n)) {
        let w = i == n / 2l ? b[i] + ROPE_POISON : b[i]
        worst = max(worst, abs(a[i] - w))
    }
    return worst
}

def private cmp_bar(t : T?; got : array<float>; want : array<float>; n : int64; bar : float; tag : string) {
    let worst = maxdiff(got, want, n)
    t |> success(worst <= bar, "{tag}: maxdiff {worst} exceeds the bar {bar}")
    let pworst = poisoned_maxdiff(got, want, n)
    t |> success(pworst > bar, "{tag}: a +{ROPE_POISON} poison on one expected element scored {pworst}, which must exceed {bar}")
}

def private cmp_exact(t : T?; got : array<float>; want : array<float>; n : int64; tag : string) {
    var bad = 0
    var poisoned = 0
    for (i in range64(n)) {
        if (got[i] != want[i]) {
            bad++
        }
        if (got[i] != (i == n / 2l ? want[i] + ROPE_POISON : want[i])) {
            poisoned++
        }
    }
    t |> success(bad == 0, "{tag}: {bad} of {int(n)} elements differ")
    t |> success(poisoned > 0, "{tag}: a +{ROPE_POISON} poison on one expected element must be seen")
}

def private norm_gate(t : T?; pos, hs, nh : int64; theta, fscale, mscale : float) {
    let n = hs * nh
    var v : array<float>
    fill_vec(v, n)
    var want : array<float>
    var ff <- ff_none()
    ref_norm(v, want, pos, hs, n, theta, fscale, mscale, ff, false)
    var got : array<float>
    copy_vec(v, got, n)
    rope_scaled(got, pos, hs, n, theta, fscale, ff, mscale)
    cmp_bar(t, got, want, n, ROPE_BAR, "rope_scaled hs={int(hs)} nh={int(nh)} pos={int(pos)}")
    delete v
    delete got
    delete want
    delete ff
}

def private neox_gate(t : T?; pos, hs, nh : int64; theta, fscale, mscale : float) {
    let n = hs * nh
    var v : array<float>
    fill_vec(v, n)
    var want : array<float>
    var ff <- ff_none()
    ref_neox(v, want, pos, hs, hs, n, theta, fscale, mscale, ff, false)
    var got : array<float>
    copy_vec(v, got, n)
    rope_scaled_neox(got, pos, hs, n, theta, fscale, ff, mscale)
    cmp_bar(t, got, want, n, ROPE_BAR, "rope_scaled_neox hs={int(hs)} nh={int(nh)} pos={int(pos)}")
    delete v
    delete got
    delete want
    delete ff
}

[test]
def test_rope_apply_norm(t : T?) {
    for (pos in [0l, 1l, 7l, 37l, 512l]) {
        t |> run("rope_scaled at pos {pos}: head 64x2, 96x1 and the 40-wide tail head vs the fp64 angles") @(t : T?) {
            norm_gate(t, pos, 64l, 2l, 10000.0, 1.0, 1.0)
            norm_gate(t, pos, 96l, 1l, 1000000.0, 1.0, 1.0)
            norm_gate(t, pos, 40l, 3l, 10000.0, 0.5, 1.19)
        }
    }
}

[test]
def test_rope_apply_neox(t : T?) {
    for (pos in [0l, 1l, 7l, 37l, 512l]) {
        t |> run("rope_scaled_neox at pos {pos}: head 64x2, 96x1 and the 40-wide tail head vs the fp64 angles") @(t : T?) {
            neox_gate(t, pos, 64l, 2l, 10000.0, 1.0, 1.0)
            neox_gate(t, pos, 96l, 1l, 1000000.0, 1.0, 1.0)
            neox_gate(t, pos, 40l, 3l, 10000.0, 0.5, 1.19)
        }
    }
}

def private tab_gate(t : T?; pos, hs, nh : int64; theta, fscale, mscale : float) {
    let n = hs * nh
    var v : array<float>
    fill_vec(v, n)
    var ff <- ff_none()
    var want : array<float>
    ref_norm(v, want, pos, hs, n, theta, fscale, mscale, ff, false)
    var direct : array<float>
    copy_vec(v, direct, n)
    rope_scaled(direct, pos, hs, n, theta, fscale, ff, mscale)
    var ct : array<float>
    var st : array<float>
    build_rope_tabs(ct, st, theta, fscale, mscale, ff, true, pos, 1l, hs)
    var got : array<float>
    copy_vec(v, got, n)
    unsafe {
        rope_scaled_tab(addr(got[0]), hs, n, addr<float const?>(ct[0]), addr<float const?>(st[0]))
    }
    cmp_bar(t, got, want, n, ROPE_BAR, "rope_scaled_tab hs={int(hs)} nh={int(nh)} pos={int(pos)}")
    cmp_bar(t, got, direct, n, TWIN_BAR, "rope_scaled_tab twin of rope_scaled hs={int(hs)} pos={int(pos)}")
    delete v
    delete got
    delete direct
    delete want
    delete ct
    delete st
    delete ff
}

def private neox_tab_gate(t : T?; pos, hs, nh : int64; theta, fscale, mscale : float) {
    let n = hs * nh
    var v : array<float>
    fill_vec(v, n)
    var ff <- ff_none()
    var want : array<float>
    ref_neox(v, want, pos, hs, hs, n, theta, fscale, mscale, ff, false)
    var direct : array<float>
    copy_vec(v, direct, n)
    rope_scaled_neox(direct, pos, hs, n, theta, fscale, ff, mscale)
    var ct : array<float>
    var st : array<float>
    build_rope_tabs(ct, st, theta, fscale, mscale, ff, true, pos, 1l, hs)
    var got : array<float>
    copy_vec(v, got, n)
    unsafe {
        rope_scaled_neox_tab(addr(got[0]), hs, n, addr<float const?>(ct[0]), addr<float const?>(st[0]))
    }
    cmp_bar(t, got, want, n, ROPE_BAR, "rope_scaled_neox_tab hs={int(hs)} nh={int(nh)} pos={int(pos)}")
    cmp_bar(t, got, direct, n, TWIN_BAR, "rope_scaled_neox_tab twin of rope_scaled_neox hs={int(hs)} pos={int(pos)}")
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
            tab_gate(t, pos, 64l, 2l, 10000.0, 1.0, 1.0)
            tab_gate(t, pos, 40l, 3l, 10000.0, 0.5, 1.19)
        }
    }
}

[test]
def test_rope_apply_neox_tab(t : T?) {
    for (pos in [0l, 1l, 37l, 512l]) {
        t |> run("rope_scaled_neox_tab at pos {pos}: the fp64 angles, and the un-tabled twin") @(t : T?) {
            neox_tab_gate(t, pos, 64l, 2l, 10000.0, 1.0, 1.0)
            neox_tab_gate(t, pos, 40l, 3l, 10000.0, 0.5, 1.19)
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
    unsafe {
        rope_scaled_neox_part(addr(got[0]), pos, hs, rot, n, theta, fscale, false, null, mscale)
    }
    cmp_bar(t, got, want, n, ROPE_BAR, "rope_scaled_neox_part hs={int(hs)} rot={int(rot)} pos={int(pos)}")
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
    cmp_exact(t, got, full, n, "rope_scaled_neox_part == the full leaf over the rotated prefix hs={int(hs)} rot={int(rot)}")
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

def private tab_part_gate(t : T?; pos, hs, rot, nh : int64; theta, fscale, mscale : float) {
    let n = hs * nh
    var v : array<float>
    fill_vec(v, n)
    var ff <- ff_none()
    var want : array<float>
    ref_neox(v, want, pos, hs, rot, n, theta, fscale, mscale, ff, false)
    var direct : array<float>
    copy_vec(v, direct, n)
    unsafe {
        rope_scaled_neox_part(addr(direct[0]), pos, hs, rot, n, theta, fscale, false, null, mscale)
    }
    var ct : array<float>
    var st : array<float>
    build_rope_tabs(ct, st, theta, fscale, mscale, ff, true, pos, 1l, rot)
    var got : array<float>
    copy_vec(v, got, n)
    unsafe {
        rope_scaled_neox_tab_part(addr(got[0]), hs, rot, n, addr<float const?>(ct[0]), addr<float const?>(st[0]))
    }
    cmp_bar(t, got, want, n, ROPE_BAR, "rope_scaled_neox_tab_part hs={int(hs)} rot={int(rot)} pos={int(pos)}")
    cmp_bar(t, got, direct, n, TWIN_BAR, "rope_scaled_neox_tab_part twin of the direct part form hs={int(hs)} rot={int(rot)}")
    delete v
    delete got
    delete direct
    delete want
    delete ct
    delete st
    delete ff
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
            tab_part_gate(t, pos, 256l, 64l, 2l, 1000000.0, 1.0, 1.0)
            tab_part_gate(t, pos, 40l, 20l, 3l, 10000.0, 1.0, 1.0)
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
            unsafe {
                rope_apply(addr(pform[0]), pos, hs, n, 10000.0, 1.0, false, null, neox, 1.19)
            }
            cmp_exact(t, pform, leaf, n, "rope_apply(ptr, neox={neox}) == the leaf")
            var aform : array<float>
            copy_vec(v, aform, n)
            rope_apply(aform, pos, hs, n, 10000.0, 1.0, ff, neox, 1.19, true)
            cmp_exact(t, aform, leaf, n, "rope_apply(array, neox={neox}) == the leaf")
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
            cmp_exact(t, off, bare, n, "use_ff=false (neox={neox}) == the no-factors apply")
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
    var ff : array<float>
    ff |> resize(hs / 2l)
    for (j in range64(hs / 2l)) {
        ff[j] = 1.0 + 0.5 * float(j % 7l)
    }
    var want : array<float>
    if (neox) {
        ref_neox(v, want, pos, hs, hs, n, 500000.0, 1.0, 1.0, ff, true)
    } else {
        ref_norm(v, want, pos, hs, n, 500000.0, 1.0, 1.0, ff, true)
    }
    var got : array<float>
    copy_vec(v, got, n)
    unsafe {
        rope_apply(addr(got[0]), pos, hs, n, 500000.0, 1.0, true, ff_ptr(ff), neox, 1.0)
    }
    cmp_bar(t, got, want, n, ROPE_BAR, "freq-factors (neox={neox}) hs={int(hs)} pos={int(pos)}")
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
    cmp_exact(t, with_ones, without, n, "all-ones factors (neox={neox}) must divide away exactly")
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
