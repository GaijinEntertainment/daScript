options gen2
options _dasllama_internal = true
options persistent_heap   // a dastest root frees what it deletes only on this heap

require dastest/testing_boost public
require math
require strings
require daslib/strings_boost   // split
require _compares   // nan_plane
require _f32_tile_pin   // the tile's grid clones, the lattice walk, the reference permutation
require daslib/rtti   // this_context: the re-raised panic's text
require dasllama/dasllama_math   // matmul_batch, batch_dot_rows, the float-batch override and the tile pin

// Every permutation of the fp32 batch tile against a double oracle at the inverse STFT's GEMM -
// the f32_batch_tile shape kitten mini's generator ends on (stft_bwd_re / stft_bwd_im: n = 11
// bins, d = 20 taps, ntok = 45481 frames on lj021 expr-voice-5-m) - and at a dot wide enough for
// every order's vector body, each held to the error bound of its own summation order, so a sidecar
// may crown any of them under a parity bar measured at one.

struct private TileShape {
    tag : string
    n : int64
    d : int64
    ntok : int64   //! on the 4x4 lattice: the tile's tokens alone
}

let private ISTFT = TileShape(tag = "the iSTFT GEMM", n = 11l, d = 20l, ntok = 45480l)   //! kitten mini's last frame is `dot`'s, not the tile's
//! an odd 301: past every permutation's lanes, with a scalar remainder past each vector body
let private WIDE = TileShape(tag = "a 301-wide dot", n = 301l, d = 20l, ntok = 2048l)
let private EPS_F32 = 1.1920929e-7lf   //! 2^-23
//! the bound's constant: |y - y_exact| <= C * EPS_F32 * sqrt(depth) * sum_j |w_j x_j|, the random-walk
//! bound over the `depth` roundings any one product passes through - c = 2 is several standard
//! deviations of that walk, and the deterministic bound is depth * EPS_F32 / 2 times the same sum
let private C = 2.0lf

//! the lanes a permutation's j loop accumulates in: `vecW` W lanes, `vecW_uK` W * K, a plain or
//! unroll-only row 1 (unrolling alone keeps the sequential order); 0 for a suffix of no known form
def private tile_lanes(perm : string) : int {
    var width = 1
    var interleave = 1
    var vectorized = false
    for (part in split(perm, "_")) {
        if (starts_with(part, "vec")) {
            width = to_int(slice(part, 3))
            vectorized = true
        } elif (starts_with(part, "u")) {
            interleave = to_int(slice(part, 1))
        } elif (part != "plain") {
            return 0
        }
    }
    return vectorized ? width * interleave : 1
}

//! the roundings one product passes through at dot length `n` over `lanes` partial sums: the
//! vector body's n / lanes steps, the reduction tree's log2(lanes) adds, the scalar remainder's
//! n % lanes steps - and n steps where the loop is sequential or too short to enter the vector body
def private order_depth(n : int64; lanes : int) : int64 {
    return n if (lanes <= 1 || n < int64(lanes))
    var tree = 0l
    var l = lanes
    while (l > 1) {
        l /= 2
        tree++
    }
    return n / int64(lanes) + tree + n % int64(lanes)
}

//! iSTFT-like operands: a windowed cosine basis [d][n], and rectangular spectra [ntok][n] whose
//! magnitudes swing across e and back, so every dot cancels as the inverse transform's does
def private basis(n, d : int64) : array<float> {
    var w : array<float>
    w |> resize(n * d)
    for (i in range64(d)) {
        let win = 0.5 - 0.5 * cos(2.0 * PI * float(i) / float(d))
        for (j in range64(n)) {
            w[i * n + j] = win * cos(2.0 * PI * float(i * j) / float(d)) / float(n)
        }
    }
    return <- w
}

def private spectra(n, ntok : int64) : array<float> {
    var x : array<float>
    x |> resize(n * ntok)
    for (tk in range64(ntok)) {
        for (j in range64(n)) {
            x[tk * n + j] = exp(sin(0.37 * float(tk) + float(j))) * cos(1.3 * float(tk) + 0.7 * float(j))
        }
    }
    return <- x
}

//! the plain-loop oracle in double: Y [ntok][d] and, per output, the bound's sum of |w_j x_j|
def private oracle(w, x : array<float>; n, d, ntok : int64) : tuple<y : array<double>; mag : array<double>> {
    var y, mag : array<double>
    y |> resize(ntok * d)
    mag |> resize(ntok * d)
    for (tk in range64(ntok)) {
        for (i in range64(d)) {
            var acc = 0.0lf
            var abs_sum = 0.0lf
            for (j in range64(n)) {
                let p = double(w[i * n + j]) * double(x[tk * n + j])
                acc += p
                abs_sum += abs(p)
            }
            y[tk * d + i] = acc
            mag[tk * d + i] = abs_sum
        }
    }
    return (y <- y, mag <- mag)
}

//! the tile's lattice through `perm` into a NaN-filled Y
def private run_perm(perm : string; s : TileShape; w, x : array<float>) : array<float> {
    var y <- nan_plane(int(s.ntok * s.d))
    unsafe {
        f32_tile_lattice(f32_tile_variant(perm), addr(y[0]), addr<float const?>(w[0]), addr<float const?>(x[0]), s.n, s.d, 0l, s.ntok)
    }
    return <- y
}

//! the outputs past `perm`'s bound, its largest difference logged with its token and row
def private outside_bound(tag : string; d : int64; y : array<float>; want, mag : array<double>; depth : int64) : int {
    var outside = 0
    var worst = 0.0lf
    var worst_bar = 0.0lf
    var at = 0l
    for (k in range64(long_length(y))) {
        let bar = C * EPS_F32 * sqrt(double(depth)) * mag[k]
        let diff = is_nan(y[k]) ? double(INFINITE_DIFF) : abs(double(y[k]) - want[k])
        if (diff > bar) {
            outside++
        }
        if (diff > worst) {
            worst = diff
            worst_bar = bar
            at = k
        }
    }
    to_log(LOG_INFO, "{tag}: {outside} of {length(y)} outputs past the bound, the largest difference {worst} at token {at / d} row {at % d} against a bound there of {worst_bar} (depth {depth})\n")
    return outside
}

//! every permutation of the tile at `s` within its own order's bound, each with its poisoned-weight control;
//! `wide` asserts every order's vector body entered (the dot at least the permutation's lanes)
def private race_perms(t : T?; s : TileShape; wide : bool) {
    let n = s.n
    let d = s.d
    let w <- basis(n, d)
    let x <- spectra(n, s.ntok)
    let ref <- oracle(w, x, n, d, s.ntok)
    let reference = f32_tile_reference_perm()
    var perms = 0
    var has_reference = false
    for (v in f32_batch_tile_template_variants()) {
        let perm = v._0
        perms++
        has_reference = has_reference || perm == reference
        let lanes = tile_lanes(perm)
        t |> success(lanes >= 1, "{perm}: a permutation suffix the bound reads")
        continue if (lanes < 1)
        if (wide && lanes > 1) {
            t |> success(n >= int64(lanes) && n % int64(lanes) != 0l, "{perm}: the {n}-wide dot enters the {lanes}-lane vector body and leaves a remainder")
        }
        let depth = order_depth(n, lanes)
        let tag = "f32_batch_tile {perm} at {s.tag}"
        var inscope y <- run_perm(perm, s, w, x)
        t |> success(all_finite(y), "{tag}: every lattice output written and finite")
        t |> equal(outside_bound(tag, d, y, ref.y, ref.mag, depth), 0, "{tag}: outputs past C * eps * sqrt({depth}) * sum |w x|")
        // the control: one weight moved so its output shifts by twice that output's bound
        let r = 6l
        let j = 4l
        let tk = 2l
        let k = tk * d + r
        let bar = C * EPS_F32 * sqrt(double(depth)) * ref.mag[k]
        var inscope wp := w
        wp[r * n + j] += float(2.0lf * bar / double(x[tk * n + j]))
        var inscope yp <- run_perm(perm, s, wp, x)
        t |> success(outside_bound("{tag} poisoned", d, yp, ref.y, ref.mag, depth) > 0, "control: {tag}'s compare reds on the poisoned weight")
        let moved = abs(double(yp[k]) - ref.y[k])
        t |> success(moved > bar, "control: {tag} with weight ({r}, {j}) poisoned by twice the bound of output (token {tk}, row {r}) lands it {moved} off, past {bar}")
    }
    t |> success(perms > 1, "the tile's grid carries {perms} permutations")
    t |> success(has_reference, "the grid carries the reference permutation '{reference}'")
}

[test]
def test_f32_batch_tile_perms(t : T?) {
    t |> run("every permutation of the fp32 batch tile within its own order's bound, at the iSTFT GEMM") @(t : T?) {
        race_perms(t, ISTFT, false)
    }
    t |> run("every permutation of the fp32 batch tile within its own order's bound, every vector body entered") @(t : T?) {
        race_perms(t, WIDE, true)
    }
}

var private g_fake_override_calls = 0

//! a float-batch override that counts the calls it is offered and serves none - the accelerate seat's stand-in
[unused_argument(yp, wp, xp, n, d, ntok)]
def private counting_override(var yp : float?; wp : float const?; xp : float const?; n, d, ntok : int64) : bool {
    g_fake_override_calls++
    return false
}

[unused_argument(yp, wp, xp, n, d, ntok)]
def private bf16_declines(var yp : float?; wp : uint16 const?; xp : float const?; n, d, ntok : int64) : bool => false

//! the reference permutation's lattice and `dot` over the rest: the pinned GEMM's bits, built apart from the pin
def private reference_gemm(w, x : array<float>; n, d, ntok : int64) : array<float> {
    var y <- nan_plane(int(ntok * d))
    let t4 = ntok - ntok % 4l
    let d4 = d - d % 4l
    unsafe {
        var yp = addr(y[0])
        let wp = addr<float const?>(w[0])
        let xp = addr<float const?>(x[0])
        f32_tile_lattice(f32_tile_variant(f32_tile_reference_perm()), yp, wp, xp, n, d, 0l, t4)
        batch_dot_rows(yp, wp, xp, n, d, d4, d, 0l, t4)
        batch_dot_rows(yp, wp, xp, n, d, 0l, d, t4, ntok)
    }
    return <- y
}

//! both fp32 batch forms under the pin at the reference order, ahead of an armed float-batch override: the
//! accelerate tier where DASLLAMA_ACCEL=1 armed it at init, else a counting stand-in with its unpinned control. Both forms'
//! outputs sit within the bound of the deepest order the pinned rows take - the reference lattice and `dot`'s tail rows, at
//! most n roundings - of the double oracle; the override put back as the cell found it.
def private pin_serves_both_forms(t : T?) {
    let n = 37l
    let d = 22l   // two rows past the 4x4 lattice
    let ntok = 42l   // two tokens past it
    let w <- basis(n, d)
    let x <- spectra(n, ntok)
    let want <- reference_gemm(w, x, n, d, ntok)
    let ref <- oracle(w, x, n, d, ntok)
    let depth = max(order_depth(n, tile_lanes(f32_tile_reference_perm())), order_depth(n, 1))
    let found = float_batch_override()
    let accel_lane = found.active
    let lane = accel_lane ? "the DASLLAMA_ACCEL=1 accelerate seat" : "the counting stand-in (DASLLAMA_ACCEL unset)"
    to_log(LOG_INFO, "test_f32_batch_tile_perms: the armed override is {lane}\n")
    if (!accel_lane) {
        with_reference_f32_tile(t) {
            t |> success(!float_batch_override_active(), "the pin alone arms no override seat - the lane announce reads the seat")
        }
        set_float_batch_override(@@counting_override, @@bf16_declines, 1l)
    }
    t |> success(float_batch_override_active(), "{lane}: the float-batch override seat is armed ahead of the pin")
    let offered = g_fake_override_calls
    let served = f32_tile_pin_calls()
    with_reference_f32_tile(t) {
        t |> success(f32_tile_pinned(), "the pin is seated inside the block")
        t |> success(float_batch_override_active(), "{lane}: the armed override seat stays armed under the pin")
        var inscope y <- nan_plane(int(ntok * d))
        matmul_batch(y, w, x, n, d, ntok)
        check_exact(t, "the plain form under the pin", y, want)
        t |> equal(outside_bound("the plain form under the pin", d, y, ref.y, ref.mag, depth), 0, "the plain form within the bound at depth {depth}")
        var inscope yo <- nan_plane(int(ntok * d))
        matmul_batch(yo, w, 0l, x, n, d, ntok)
        check_exact(t, "the offset form under the pin, {lane} armed", yo, want)
        t |> equal(outside_bound("the offset form under the pin", d, yo, ref.y, ref.mag, depth), 0, "the offset form within the bound at depth {depth}")
    }
    t |> equal(f32_tile_pin_calls() - served, 2, "the pin served both GEMMs under {lane}")
    t |> success(!f32_tile_pinned(), "the pin is put back after the block")
    if (!accel_lane) {
        t |> equal(g_fake_override_calls - offered, 0, "no GEMM reached the override while pinned")
        // the control: unpinned, the offset form reaches the override and the plain form does not (master's route)
        var inscope y <- nan_plane(int(ntok * d))
        matmul_batch(y, w, 0l, x, n, d, ntok)
        t |> equal(g_fake_override_calls - offered, 1, "control: unpinned, the offset form is offered to the override")
        matmul_batch(y, w, x, n, d, ntok)
        t |> equal(g_fake_override_calls - offered, 1, "the plain form never reaches the override")
        if (found.active) {
            set_float_batch_override(found.fp, found.bf16, found.min_ntok)
        } else {
            clear_float_batch_override()
        }
    }
}

//! a panic inside the pinned block reaches the caller with the pin put back
def private pin_restored_on_panic(t : T?) {
    var raised = ""
    try {
        with_reference_f32_tile(t) {
            panic("the block's own panic")
        }
    } recover {
        raised = this_context().last_exception
    }
    t |> success(find(raised, "the block's own panic") >= 0, "the block's panic reaches the caller: '{raised}'")
    t |> success(!f32_tile_pinned(), "the pin is put back after a panic")
}

[test]
def test_f32_tile_pin_seat(t : T?) {
    t |> run("the pin serves both fp32 batch forms at the reference order, ahead of an armed float-batch override (DASLLAMA_ACCEL=1's accelerate seat, else a stand-in)") @(t : T?) {
        pin_serves_both_forms(t)
    }
    t |> run("a panic inside the pinned block puts the pin back and re-raises") @(t : T?) {
        pin_restored_on_panic(t)
    }
}
