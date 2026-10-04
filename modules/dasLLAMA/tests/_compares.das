// The compares behind the suites' bars - the logits and kernel-output maxdiff, the relative bar's scale, the
// id-list equality - with no fixture root beside them, so a model-free file requires this and never
// `_model_tier` (the per-PR split reads a same-folder `require _<fixture>` as reaching that fixture's roots).

options gen2
options _dasllama_internal = true

module _compares shared public

require dasllama/dasllama_lint public
require math
require daslib/linq   // sequence_equal, count, all
require daslib/algorithm   // fill
require daslib/math_bits   // uint_bits_to_float: the NaN sentinel as a float
require dastest/testing_boost   // T?: check asserts through the cell's handle
require dasllama/dasllama_math   // parallel_argmax

let POISON = 7.5   //! the slots a kernel must never read or write: a large finite value, so a stray read lands in the sum
let SLAB_GAP = 3   //! the poisoned slots `put_run` leaves before a run of a slab plane

//! `n` patterned floats: (i * mul + add) % modulus scaled and biased - the kernel cells' input fill
def row_fill(n : int; mul, add : int; scale, off : float; modulus : int = 97) : array<float> {
    var a : array<float>
    a |> resize(n)
    for (i in range(n)) {
        a[i] = float((i * mul + add) % modulus) * scale - off
    }
    return <- a
}

//! `n` slots of `v`
def plane_of(n : int; v : float) : array<float> {
    var a : array<float>
    a |> resize(n)
    a |> fill(v)
    return <- a
}

//! `n` slots of `POISON`
def poison_plane(n : int) : array<float> => plane_of(n, POISON)

//! `n` slots of `NAN_SENTINEL`: a kernel output before its run
def nan_plane(n : int) : array<float> => plane_of(n, NAN_SENTINEL)

//! every element of `a` set to `NAN_SENTINEL`: a kernel output refilled before its run
def nan_fill(var a : array<float>) {
    a |> fill(NAN_SENTINEL)
}

//! `a` behind `base` slots of `POISON` and followed by `slack` more: a device plane whose run starts at an element base
def at_base(a : array<float>; base : int; slack : int = 0) : array<float> {
    var out <- poison_plane(base + length(a) + slack)
    for (i in range(length(a))) {
        out[base + i] = a[i]
    }
    return <- out
}

//! `a` appended to `plane` behind `lead` poisoned slots; its element base in the plane
def put_run(var plane : array<float>; a : array<float>; lead : int = SLAB_GAP) : int {
    plane |> push_from(poison_plane(lead))
    let off = length(plane)
    plane |> push_from(a)
    return off
}

//! `put_run` at the next run base on the four lattice: a float4 row's element base
def put_run4(var plane : array<float>; a : array<float>) : int {
    while ((length(plane) + SLAB_GAP) % 4 != 0) {
        plane |> push(POISON)
    }
    return put_run(plane, a)
}

//! `n` elements of `a` from `at`
def copy_span(a : array<auto(TT)>; at, n : int64) : array<TT> => subarray(a, range64(at, at + n))

def copy_span(a : array<auto(TT)>; at, n : int) : array<TT> => copy_span(a, int64(at), int64(n))

//! `v` with element `at` moved by `by`: the poisoned expectation a compare must see
def poisoned_copy(v : array<auto(TT)>; at : int; by : TT) : array<TT> {
    var c := v
    static_if (typeinfo is_float(type<TT>) || typeinfo is_double(type<TT>)) {
        c[at] += by
    } else {
        c[at] = TT(int64(c[at]) + int64(by))
    }
    return <- c
}

//! true when every element of `whole` outside [lo, lo + n) is `kept` (any NaN for a NaN) - the slots a run leaves as they were
def outside_run(whole : array<float>; lo, n : int; kept : float) : bool {
    for (i in range(length(whole))) {
        continue if (i >= lo && i < lo + n)
        return false if (is_nan(kept) ? !is_nan(whole[i]) : whole[i] != kept)
    }
    return true
}

//! true when every element of `whole` outside [lo, lo + n) equals the same element of `kept` - the input a run leaves
def outside_run(whole : array<float>; lo, n : int; kept : array<float>) : bool {
    for (i in range(length(whole))) {
        continue if (i >= lo && i < lo + n)
        return false if (whole[i] != kept[i])
    }
    return true
}

def scaled_copy(a : array<float>; k : float) : array<float> {
    return <- [for (v in a); v * k]
}

//! the index of the largest |got - want| (0 when every element agrees; a NaN on either side is the largest)
def maxdiff_at(got, want : array<float>) : int => span_maxdiff(got, want, 0, 0, length(got)).at

//! the NaN elements of `a`: the sentinel words a dispatch left
def nan_count(a : array<float>) : int => count(a, $(v) => is_nan(v))

//! true when every element of `a` is finite: no NaN, no infinity
def all_finite(a : array<float>) : bool => all(a, $(v) => is_finite(v))

//! the largest |got[gbase + i] - want[wbase + i]| over `n` elements and its index `i` (0 where every element agrees; a NaN on
//! either side counts as the largest difference) - the exact compares' log line, with the index split into a row and a column by the caller
def span_maxdiff(got, want : array<float>; gbase, wbase, n : int) : tuple<diff : float; at : int> {
    var m = 0.0
    var at = 0
    for (i in range(n)) {
        let g = got[gbase + i]
        let w = want[wbase + i]
        let dd = (is_nan(g) || is_nan(w)) ? INFINITE_DIFF : abs(g - w)
        if (dd > m) {
            m = dd
            at = i
        }
    }
    return (diff = m, at = at)
}

let INFINITE_DIFF = uint_bits_to_float(0x7f800000u)   //! what a compare reads where no bar may hold: a NaN on either side, or rows of two lengths

//! true when every element of `a` is bit-equal to `v`
def elem_all(a : array<float>; v : float) : bool => all(a, $(e) => e == v)

//! Max |a[i] - b[i]| over two float arrays - the compare behind every logits or kernel-output bar in the suites. A NaN on
//! either side, or arrays of two lengths, read as infinite: outside every bar.
def logits_maxdiff(a, b : array<float>) : float {
    return INFINITE_DIFF if (length(a) != length(b))
    return span_maxdiff(a, b, 0, 0, length(a)).diff
}

//! Max |a[i]| - the scale a relative logits bar multiplies; infinite where an element is a NaN.
def logits_maxabs(a : array<float>) : float {
    var m = 0.0
    for (x in a) {
        return INFINITE_DIFF if (is_nan(x))
        m = max(m, abs(x))
    }
    return m
}

//! Element-for-element equality of two id lists - the token oracles' compare, named for the sites that read it.
def ids_equal(a, b : array<int64>) : bool => sequence_equal(a, b)

//! The kernel cells' relative bar - the device's reassociation and its transcendentals against the CPU oracle: an f16 store
//! rounds at 2^-11 = 4.9e-4 relative, f32 sums of up to 4096 terms drift n x 6e-8 <= 2.5e-4, the device transcendentals ~1e-6;
//! 2e-3 covers each with margin.
let APPROX_REL = 2e-3

//! within `abs_floor` absolute, or within `rel` of the larger magnitude floored at 1 - the kernel cells' bar on one element
def bar_ok(g, w, rel : float; abs_floor : float = 0.0) : bool => abs(g - w) <= abs_floor || abs(g - w) <= rel * max(1.0, max(abs(g), abs(w)))

//! within `APPROX_REL` of the larger magnitude, floored at 1
def approx(x, y : float) : bool => bar_ok(x, y, APPROX_REL)

//! the count of `got` outside `bar_ok`'s bars, the first six logged under `tag` - enough to tell tolerance drift from garbage
def mismatch_rel(tag : string; got, want : array<float>; rel : float; abs_floor : float = 0.0) : int {
    var bad = 0
    for (i in range(length(got))) {
        if (!bar_ok(got[i], want[i], rel, abs_floor)) {
            if (bad < 6) {
                to_log(LOG_ERROR, "  {tag} mismatch [{i}] gpu {got[i]} cpu {want[i]}\n")
            }
            bad++
        }
    }
    return bad
}

//! `mismatch_rel` at the kernel cells' bar.
def mismatch_approx(got, want : array<float>) : int => mismatch_rel("", got, want, APPROX_REL)

//! What a compare holds each element to. `rel_of_larger`: `bar_ok` - within `abs_floor`, or within `rel` of the larger
//! magnitude floored at 1; `exact`: bit-equal; `rel_of_max`: within `rel` of the expectation's largest magnitude, floored at
//! `abs_floor`; `envelope`: within `rel` of the element's envelope, floored at `abs_floor`; `per_element`: within the element's
//! own bar. The last two read the envelope or the bars `check` takes beside the expectation.
enum BarKind {
    rel_of_larger
    exact
    rel_of_max
    envelope
    per_element
}

//! A kernel cell's bar and its poisoned-expectation control: `exact` is `kind = exact` spelled as a flag. The control's expected
//! element `poison_at` (the last under -1) moves by `poison` - times its own magnitude floored at 1 under `poison_scaled`, times
//! the expectation's largest magnitude under `poison_of_max`, plus `poison_abs`, or at least `poison_rel` of its own magnitude
//! where that is nonzero - and must land outside the bar.
struct Bar {
    exact : bool
    kind : BarKind
    rel : float
    abs_floor : float
    poison : float = 0.5
    poison_scaled : bool
    poison_of_max : bool
    poison_abs : float
    poison_rel : float
    poison_at : int = -1
}

let BAR_EXACT = Bar(exact = true)
let BAR_APPROX = Bar(rel = APPROX_REL)

let FLAT_ROWS = 0   //! `check_rows`' row width for a compare with no row structure: the max difference logs at its element

//! How a compare names an element in its log: rows `row_width` wide (0 = flat), each split into heads `head_width` wide (0 = none).
struct Shape {
    row_width : int
    head_width : int
}

let FLAT_SHAPE = Shape()

//! `g` inside `bar` of `w`: bit-equal on an exact bar, else `bar_ok` at the bar's `rel` and `abs_floor`
def within_bar(bar : Bar; g, w : float) : bool => bar.exact ? g == w : bar_ok(g, w, bar.rel, bar.abs_floor)

//! The elements of `got` outside `bar` of `want`, the first six logged under `tag` on a tolerance bar.
def mismatch_bar(tag : string; got, want : array<float>; bar : Bar) : int => bar.exact ? mismatch_exact(got, want) : mismatch_rel(tag, got, want, bar.rel, bar.abs_floor)

//! `check` at `bar` with rows `row_width` wide and heads `head_width` wide
def check_rows(t : T?; tag : string; got, want : array<float>; bar : Bar; row_width : int; head_width : int = 0) {
    check(t, tag, got, want, bar, Shape(row_width = row_width, head_width = head_width))
}

//! element `i` named by `shape`: its row, head and column, or its flat index
def element_at(i : int; shape : Shape) : string {
    return "element {i}" if (shape.row_width <= 0)
    let col = i % shape.row_width
    return "row {i / shape.row_width} col {col}" if (shape.head_width <= 0)
    return "row {i / shape.row_width} head {col / shape.head_width} col {col % shape.head_width}"
}

def private bar_kind(bar : Bar) : BarKind => bar.exact ? BarKind.exact : bar.kind

//! the bar element `i` holds at: 0 on the exact bar; `scale` is the expectation's largest magnitude, `env` the envelope or the
//! per-element bars (an element past its end holds at 0)
def private bar_at(bar : Bar; g, w, scale : auto(WT); env : array<auto(ET)>; i : int) : WT {
    let kind = bar_kind(bar)
    if (kind == BarKind.rel_of_larger) {
        return max(WT(bar.abs_floor), WT(bar.rel) * max(WT(1), max(abs(g), abs(w))))
    } elif (kind == BarKind.rel_of_max) {
        return max(WT(bar.abs_floor), WT(bar.rel) * scale)
    } elif (kind == BarKind.exact || i >= length(env)) {
        return WT(0)
    } elif (kind == BarKind.envelope) {
        return max(WT(bar.abs_floor), WT(bar.rel) * WT(env[i]))
    }
    return WT(env[i])
}

//! `g` inside bar `b` of `w`; a NaN on either side never is
def private inside(exact : bool; g, w, b : auto(WT)) : bool {
    return false if (is_nan(g) || is_nan(w))
    return exact ? g == w : abs(g - w) <= b
}

//! A cell's verdict on `got` against `want` (float or double) at `bar`: every element inside it and the two of one length (a
//! NaN on either side, and every element past the shorter one's end, is off); the largest difference logged with its element
//! (`shape`) and the bar there, or on the exact bar the count and the first differing element; the poisoned expectation
//! landing outside. The count off.
def check(t : T?; tag : string; got : array<float>; want : array<auto(WT)>; bar : Bar; shape : Shape) : int {
    let none : array<WT -const>
    return check(t, tag, got, want, bar, shape, none)
}

//! the largest magnitude of `want`'s non-NaN elements: the scale a `rel_of_max` bar multiplies
def private max_magnitude(want : array<auto(WT)>) : WT {
    var scale = WT(0)
    for (w in want) {
        if (!is_nan(w)) {
            scale = max(scale, abs(w))
        }
    }
    return scale
}

//! `check`'s count alone, with no assert and no log: the elements of `got` outside `bar` of `want` (a NaN on either side, and
//! every element past the shorter one's end, is off) - what a control that poisons the kernel's INPUT asserts above 0
def check_count(got : array<float>; want : array<auto(WT)>; bar : Bar) : int {
    let none : array<WT -const>
    return check_count(got, want, bar, none)
}

//! `check_count` on an `envelope` or `per_element` bar: `env` holds each element's envelope or its own bar
def check_count(got : array<float>; want : array<auto(WT)>; bar : Bar; env : array<auto(ET)>) : int {
    let exact = bar_kind(bar) == BarKind.exact
    let scale = max_magnitude(want)
    var bad = abs(length(got) - length(want))
    for (i in range(min(length(got), length(want)))) {
        let g = WT(got[i])
        let w = want[i]
        if (!inside(exact, g, w, bar_at(bar, g, w, scale, env, i))) {
            bad++
        }
    }
    return bad
}

//! `check` on an `envelope` or `per_element` bar: `env` holds each element's envelope or its own bar
def check(t : T?; tag : string; got : array<float>; want : array<auto(WT)>; bar : Bar; shape : Shape; env : array<auto(ET)>) : int {
    let exact = bar_kind(bar) == BarKind.exact
    let n = min(length(got), length(want))
    let scale = max_magnitude(want)
    var bad = abs(length(got) - length(want))
    var first = n
    var worst = WT(0)
    var worst_at = 0
    var worst_bar = WT(0)
    var logged = 0
    for (i in range(n)) {
        let g = WT(got[i])
        let w = want[i]
        let b = bar_at(bar, g, w, scale, env, i)
        let d = (is_nan(g) || is_nan(w)) ? WT(INFINITE_DIFF) : abs(g - w)
        if (!inside(exact, g, w, b)) {
            first = min(first, i)
            if (!exact && logged < 6) {
                to_log(LOG_ERROR, "  {tag} off at {element_at(i, shape)}: kernel {g} reference {w} bar {b}\n")
                logged++
            }
            bad++
        }
        if (i == 0 || d > worst) {
            worst = d
            worst_at = i
            worst_bar = b
        }
    }
    let total = max(length(got), length(want))
    if (length(got) != length(want)) {
        to_log(LOG_ERROR, "{tag}: the kernel holds {length(got)} elements, the reference {length(want)}\n")
    }
    let verdict = exact ? "{bad} of {total} elements differ, the first at {element_at(first, shape)}" : "{bad} of {total} elements outside the bar, the largest difference {worst} at {element_at(worst_at, shape)} against a bar there of {worst_bar}"
    to_log(LOG_INFO, "{tag}: {verdict}\n")
    t |> success(bad == 0, "{tag}: {verdict}")
    let at = bar.poison_at < 0 ? n - 1 : bar.poison_at
    if (at < 0 || at >= n) {
        t |> success(false, "{tag}: no element {at} to poison in {n}")
        return bad
    }
    let w = want[at]
    let delta = WT(bar.poison) * (bar.poison_scaled ? max(WT(1), abs(w)) : (bar.poison_of_max ? scale : WT(1))) + WT(bar.poison_abs)
    let pw = w + (bar.poison_rel != 0.0 ? max(delta, WT(bar.poison_rel) * abs(w)) : delta)
    let g = WT(got[at])
    t |> success(!inside(exact, g, pw, bar_at(bar, g, pw, scale, env, at)), "{tag}: the bar reds a poisoned expected element {at}")
    return bad
}

//! A typed exact compare - floats, bytes or words: every element equal and the two of one length, the count and the first
//! differing index logged; the last expected element moved by one must be seen. The count off.
def check_exact(t : T?; tag : string; got, want : array<auto(TT)>) : int {
    let bad = mismatch_exact(got, want)
    let n = min(length(got), length(want))
    let fm = first_mismatch(got, want, 0, 0, n)
    let first = fm >= 0 ? fm : n
    let verdict = "{bad} of {max(length(got), length(want))} elements differ, the first at {first}"
    to_log(LOG_INFO, "{tag}: {verdict}\n")
    t |> success(bad == 0, "{tag}: {verdict}")
    if (n == 0) {
        t |> success(false, "{tag}: no element to poison")
        return bad
    }
    var poisoned <- poisoned_copy(want, n - 1, TT(1))
    t |> success(mismatch_exact(got, poisoned) != bad, "{tag}: the compare sees a poisoned expected element {n - 1}")
    delete poisoned
    return bad
}

//! the count of `got` not equal to `want` - the exact bar's compare, over floats, bytes or words alike; every element one
//! array has past the other's end counts
def mismatch_exact(got, want : array<auto(TT)>) : int {
    var bad = abs(length(got) - length(want))
    for (g, w in got, want) {
        if (g != w) {
            bad++
        }
    }
    return bad
}

//! the count of `got[gbase + i]` not equal to `want[wbase + i]` over `n` elements - `mismatch_exact` over a span of each
def mismatch_exact(got, want : array<auto(TT)>; gbase, wbase, n : int) : int {
    var bad = 0
    for (i in range(n)) {
        if (got[gbase + i] != want[wbase + i]) {
            bad++
        }
    }
    return bad
}

//! the first `i` with `got[gbase + i]` not equal to `want[wbase + i]` over `n` elements, -1 where every one agrees
def first_mismatch(got, want : array<auto(TT)>; gbase, wbase, n : int) : int {
    for (i in range(n)) {
        return i if (got[gbase + i] != want[wbase + i])
    }
    return -1
}

//! The Q8_0 planes' differences, scale for scale then byte for byte: a scale off by more than `scale_bar` relative (floored at
//! 1; 0 = bit-exact), a quant byte off by more than `qtol` steps.
def mismatch_q8(qd : array<int8>; sd : array<float>; qr : array<int8>; sr : array<float>; scale_bar : float; qtol : int) : int {
    var bad = scale_bar == 0.0 ? mismatch_exact(sd, sr) : mismatch_rel("q8 scales", sd, sr, scale_bar)
    for (a, b in qd, qr) {
        if (abs(int(a) - int(b)) > qtol) {
            bad++
        }
    }
    return bad
}

//! The word a kernel cell fills an output with before its dispatch: a NaN, so a stale or unwritten element cannot sit
//! inside any bar.
let NAN_SENTINEL_BITS = 0x7fc00000u
let NAN_SENTINEL = uint_bits_to_float(NAN_SENTINEL_BITS)   //! the sentinel as a float, the `kept` value of an output's untouched slots

//! the float a bf16 word widens to: exact
def bf16_value(h : uint16) : float => uint_bits_to_float(uint(h) << 16u)

//! `x`'s bf16 word by TRUNCATION - its top 16 bits, rounding toward zero (the engine's own bf16 narrowing rounds to nearest-even)
def bf16_bits_truncated(x : float) : uint16 => uint16(float_bits_to_uint(x) >> 16u)

//! the index of the first largest of `v`'s first `n` elements - the engine's `parallel_argmax`
def argmax_of(v : array<float>; n : int64) : int64 => parallel_argmax(v, n)

//! the index of the first largest element of `v`
def argmax_of(v : array<float>) : int64 => parallel_argmax(v, long_length(v))

//! the index, counted from `off`, of the first largest of `v[off .. off + n)`
def argmax_of(v : array<float>; off, n : int64) : int64 => parallel_argmax(unsafe(addr(v[off])), n)

//! the largest of `a`'s first `n` elements less the second largest: the tie distance an argmax flip crossed
def top2_margin(a : array<float>; n : int64) : float {
    var m1 = -3.4e38
    var m2 = -3.4e38
    for (i in range64(n)) {
        let v = a[i]
        if (v > m1) {
            m2 = m1
            m1 = v
        } elif (v > m2) {
            m2 = v
        }
    }
    return m1 - m2
}

//! true under the JIT; else the cell has registered the JIT-only skip and its caller returns
def jit_only(t : T?) : bool {
    return true if (jit_enabled())
    t |> skip("interpreted (dasLLAMA model tests are JIT-only)")
    return false
}

var private g_opaque_taint = 0u

[init]
def private init_opaque_taint() {
    g_opaque_taint = 0u
}

//! `x` unchanged, through a global the optimizer cannot fold: a fixture value no constant propagation reaches
[sideeffects]
def opaque(x : auto(TT)) : TT {
    static_if (typeinfo is_float(x) || typeinfo is_double(x)) {
        return x + TT(g_opaque_taint)
    } else {
        return x ^ TT(g_opaque_taint)
    }
}

//! A length sweep's tally: the lengths run, the elements compared, those off their bar, the poison legs inside it, the
//! elements an in-place kernel left as it found them, and the largest difference with its bar, length and element.
struct Sweep {
    cases : int
    elems : int
    bad : int
    poison_inside : int
    inert : int
    worst : float
    worst_bar : float
    worst_n : int
    worst_at : int
}

//! One swept length `n` into `r`: every element of `got` within `bar` of `want` (0 = bit-exact; a NaN, and every element past
//! the shorter one's end, is off), and with `poison` nonzero the expectation moved by it landing outside the bar.
def sweep_check(var r : Sweep; n : int; bar : float; got, want : array<float>; poison : float = 0.0) {
    r.cases++
    r.bad += abs(length(got) - length(want))
    for (i in range(min(length(got), length(want)))) {
        let g = got[i]
        let w = want[i]
        r.elems++
        let d = (is_nan(g) || is_nan(w)) ? INFINITE_DIFF : abs(g - w)
        if (bar == 0.0 ? g != w : !(d <= bar)) {
            r.bad++
        }
        if (poison != 0.0 && (bar == 0.0 ? g == w + poison : abs(g - (w + poison)) <= bar)) {
            r.poison_inside++
        }
        if (d >= r.worst) {
            r.worst = d
            r.worst_bar = bar
            r.worst_n = n
            r.worst_at = i
        }
    }
}

//! the elements of `got` bit-equal to `before` into `r`: what an in-place kernel left where it found it
def sweep_inert(var r : Sweep; got, before : array<float>) {
    for (g, b in got, before) {
        if (g == b) {
            r.inert++
        }
    }
}

//! The sweep's verdict: `cases` lengths run (0 = at least one), none off its bar, no poison leg inside it, the largest
//! difference logged; `moved` also asks every element of an in-place run to have moved.
def report_sweep(t : T?; name : string; r : Sweep; cases : int = 0; moved : bool = false) {
    t |> success(cases > 0 ? r.cases == cases : r.cases > 0 && r.elems > 0, "{name}: {r.cases} lengths ran, {cases > 0 ? cases : 1} wanted")
    let worst = "worst |diff| {r.worst} against bar {r.worst_bar} at n={r.worst_n} element {r.worst_at}"
    to_log(LOG_INFO, "{name}: {r.elems} elements, {worst}\n")
    t |> success(r.bad == 0, "{name}: {r.bad} values past the bar, {worst}")
    t |> success(r.poison_inside == 0, "{name}: {r.poison_inside} poison legs inside the bar - a bar nothing exceeds discriminates nothing")
    if (moved) {
        t |> success(r.inert == 0, "{name}: {r.inert} elements the kernel left exactly where it found them")
    }
}
