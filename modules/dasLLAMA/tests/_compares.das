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
require dastest/testing_boost   // T?: check_rows asserts through the cell's handle

let POISON = 7.5   //! the slots a kernel must never read or write: a large finite value, so a stray read lands in the sum
let SLAB_GAP = 3   //! the poisoned slots `put` leaves before a run of a slab plane

//! `n` patterned floats: (i * mul + add) % 97 scaled and biased - the kernel cells' input fill
def row_fill(n : int; mul, add : int; scale, off : float) : array<float> {
    var a : array<float>
    a |> resize(n)
    for (i in range(n)) {
        a[i] = float((i * mul + add) % 97) * scale - off
    }
    return <- a
}

//! `n` slots of `POISON`
def poison_plane(n : int) : array<float> {
    var a : array<float>
    a |> resize(n)
    a |> fill(POISON)
    return <- a
}

//! `a` behind `base` slots of `POISON` and followed by `slack` more: a device plane whose run starts at an element base
def at_base(a : array<float>; base : int; slack : int = 0) : array<float> {
    var out <- poison_plane(base + length(a) + slack)
    for (i in range(length(a))) {
        out[base + i] = a[i]
    }
    return <- out
}

//! `a` appended to `plane` behind `gap` poisoned slots; its element base in the plane
def put(var plane : array<float>; a : array<float>; gap : int = SLAB_GAP) : int {
    plane |> push_from(poison_plane(gap))
    let off = length(plane)
    plane |> push_from(a)
    return off
}

//! `put` at the next run base on the four lattice: a float4 row's element base
def put4(var plane : array<float>; a : array<float>) : int {
    while ((length(plane) + SLAB_GAP) % 4 != 0) {
        plane |> push(POISON)
    }
    return put(plane, a)
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

//! the index of the largest |got - want| (0 when every element agrees)
def maxdiff_at(got, want : array<float>) : int {
    var m = 0.0
    var at = 0
    for (i in range(length(got))) {
        let dd = abs(got[i] - want[i])
        if (dd > m) {
            m = dd
            at = i
        }
    }
    return at
}

//! the NaN elements of `a`: the sentinel words a dispatch left
def nan_count(a : array<float>) : int => count(a, $(v) => is_nan(v))

//! true when every element of `a` is bit-equal to `v`
def elem_all(a : array<float>; v : float) : bool => all(a, $(e) => e == v)

//! Max |a[i] - b[i]| over two float arrays of one length (the shorter's, where they differ) - the compare behind every
//! logits or kernel-output bar in the suites.
def logits_maxdiff(a, b : array<float>) : float {
    var m = 0.0
    for (x, y in a, b) {
        m = max(m, abs(x - y))
    }
    return m
}

//! Max |a[i]| - the scale a relative logits bar multiplies.
def logits_maxabs(a : array<float>) : float {
    var m = 0.0
    for (x in a) {
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

//! within `absf` absolute, or within `rel` of the larger magnitude floored at 1 - the kernel cells' bar on one element
def bar_ok(g, w, rel : float; absf : float = 0.0) : bool => abs(g - w) <= absf || abs(g - w) <= rel * max(1.0, max(abs(g), abs(w)))

//! within `APPROX_REL` of the larger magnitude, floored at 1
def approx(x, y : float) : bool => bar_ok(x, y, APPROX_REL)

//! the count of `got` outside `bar_ok`'s bars, the first six logged under `tag` - enough to tell tolerance drift from garbage
def mismatch_rel(got, want : array<float>; rel : float; absf : float = 0.0; tag : string = "") : int {
    var bad = 0
    for (i in range(length(got))) {
        if (!bar_ok(got[i], want[i], rel, absf)) {
            if (bad < 6) {
                to_log(LOG_ERROR, "  {tag} mismatch [{i}] gpu {got[i]} cpu {want[i]}\n")
            }
            bad++
        }
    }
    return bad
}

//! `mismatch_rel` at the kernel cells' bar.
def mismatch_approx(got, want : array<float>) : int => mismatch_rel(got, want, APPROX_REL)

//! A kernel cell's bar and its poisoned-element control: bit-exact, or `bar_ok` at `rel` and `absf`; the control's expected
//! element moves by `poison` (times its magnitude floored at 1 under `scaled`) and must land outside the bar.
struct Bar {
    exact : bool
    rel : float
    absf : float
    poison : float = 0.5
    scaled : bool
}

let BAR_EXACT = Bar(exact = true)
let BAR_APPROX = Bar(rel = APPROX_REL)

//! the bar `rel` relative and `absf` absolute, its control's expectation `poison` off (times the magnitude under `scaled`)
def bar_rel(rel : float; poison : float; absf : float = 0.0; scaled : bool = false) : Bar => Bar(rel = rel, absf = absf, poison = poison, scaled = scaled)

def bar_holds(bar : Bar; g, w : float) : bool => bar.exact ? g == w : bar_ok(g, w, bar.rel, bar.absf)

//! The elements of `got` outside `bar` of `want`, the first six logged under `tag` on a tolerance bar.
def bar_misses(tag : string; got, want : array<float>; bar : Bar) : int => bar.exact ? mismatch_exact(got, want) : mismatch_rel(got, want, bar.rel, bar.absf, tag)

//! a cell's verdict: `got` inside `bar` of `want`, the max difference logged at its row (`width`) and head (`sub`), the last element poisoned
def check_rows(t : T?; tag : string; got, want : array<float>; bar : Bar; width : int; sub : int = 0) {
    let n = length(want)
    let bad = bar_misses(tag, got, want, bar)
    let mi = maxdiff_at(got, want)
    let col = width > 0 ? mi % width : mi
    let at = width <= 0 ? "element {mi}" : (sub > 0 ? "row {mi / width} head {col / sub} col {col % sub}" : "row {mi / width} col {col}")
    to_log(LOG_INFO, "{tag}: max |gpu - cpu| {abs(got[mi] - want[mi])} at {at}\n")
    t |> success(bad == 0, "{tag}: every element within the bar of the CPU form ({bad} of {n} off)")
    let w = want[n - 1]
    let poisoned = w + (bar.scaled ? bar.poison * max(1.0, abs(w)) : bar.poison)
    t |> success(!bar_holds(bar, got[n - 1], poisoned), "{tag}: the bar reds a poisoned element")
}

//! the count of `got` not equal to `want` - the exact bar's compare, over floats, bytes or words alike
def mismatch_exact(got, want : array<auto(TT)>) : int {
    var bad = 0
    for (g, w in got, want) {
        if (g != w) {
            bad++
        }
    }
    return bad
}

//! The Q8_0 planes' differences, scale for scale then byte for byte: a scale off by more than `scale_bar` relative (floored at
//! 1; 0 = bit-exact), a quant byte off by more than `qtol` steps.
def mismatch_q8(qd : array<int8>; sd : array<float>; qr : array<int8>; sr : array<float>; scale_bar : float; qtol : int) : int {
    var bad = scale_bar == 0.0 ? mismatch_exact(sd, sr) : mismatch_rel(sd, sr, scale_bar)
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
