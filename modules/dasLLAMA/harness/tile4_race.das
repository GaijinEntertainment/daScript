options gen2
options persistent_heap
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options _dasllama_internal = true

require ../performance/profile_common.das   // tune_gate - the standing pre-measure stack
require _race_fixture                       // the round loop, the poison, the compare and the refusal tally
require dasllama/dasllama_math
require dasllama/dasllama_math_gen
require dasllama/dasllama_repack
require dasllama/dasllama_convert
require dasllama/dasllama_par
require daslib/jobque_boost
require math

// The tile4 companion's shape, raced: one process, a four-token step over `PLANES_BYTES` of q8
// planes walked plane after plane, so every weight group streams from DRAM the way a decode
// step's layers do. Every arm is timed once a round in turn (`ROUNDS` rounds after `WARMUP`
// dropped): the stamped GEMV a token (what a batch below the amx tile step ran before the
// companion), the stamped tile4, and the vector tile at each (kstep, nrsplit, prefetch distance)
// candidate over the same planes. BASELINE is the shape `tile4_perm` ships; the stamped tile4 is
// held bit for bit to it, every other arm to the GEMV arm within the f32 fold order. The groups
// dispatch as the batch kernel's vector walk dispatches them. Lanes: DAS_JOBQUE_THREADS. A box
// whose q8 stamp is no amx tile, or an arm outside its bound, exits non-zero.
//
//   bin/daslang -jit modules/dasLLAMA/harness/tile4_race.das

let BASELINE = "k4 n4 pf2048"
let ROUNDS = 31
let WARMUP = 3
let NTOK = 4l
let PLANES_BYTES = 1536l << 20l   // under 2^31: the planes sit in one array
//! a vector tile sums a row's blocks in another order than the GEMV: the same integer products, f32 rounding apart
let FLOAT_ORDER = 1.0e-5

let ARM_GEMV = 0
let ARM_STAMPED = 1
let ARM_NAMES <- ["gemv x4 (stamped)", "tile4 (stamped)", "k1 n4 pf0", "k2 n2 pf0", "k2 n4 pf0", "k4 n4 pf0",
    "k2 n4 pf1024", "k2 n4 pf2048", "k2 n4 pf4096", "k1 n4 pf2048", "k2 n2 pf2048", "k4 n4 pf2048", "k4 n4 pf4096"]

[llvm_code(name = "dasllama_gemm_gen::q8q8_tile", dot = "vpdpbusd", width = 512, mr = 16, kstep = 1, nrsplit = 4),
 unused_argument(yp, wg, sg, xqp, xsp, xbsp, n, d, g, t0), hint(unsafe_range_check, noalias = wg, noalias = sg, noalias = xqp, noalias = xsp, noalias = xbsp)]
def tile_k1n4p0(var yp : float?; wg : int8 const?; sg : float const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n, d, g, t0 : int64) : void {
    panic("tile4_race: the generator declined k1 n4 pf0")
}

[llvm_code(name = "dasllama_gemm_gen::q8q8_tile", dot = "vpdpbusd", width = 512, mr = 16, kstep = 2, nrsplit = 2),
 unused_argument(yp, wg, sg, xqp, xsp, xbsp, n, d, g, t0), hint(unsafe_range_check, noalias = wg, noalias = sg, noalias = xqp, noalias = xsp, noalias = xbsp)]
def tile_k2n2p0(var yp : float?; wg : int8 const?; sg : float const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n, d, g, t0 : int64) : void {
    panic("tile4_race: the generator declined k2 n2 pf0")
}

[llvm_code(name = "dasllama_gemm_gen::q8q8_tile", dot = "vpdpbusd", width = 512, mr = 16, kstep = 2, nrsplit = 4),
 unused_argument(yp, wg, sg, xqp, xsp, xbsp, n, d, g, t0), hint(unsafe_range_check, noalias = wg, noalias = sg, noalias = xqp, noalias = xsp, noalias = xbsp)]
def tile_k2n4p0(var yp : float?; wg : int8 const?; sg : float const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n, d, g, t0 : int64) : void {
    panic("tile4_race: the generator declined k2 n4 pf0")
}

[llvm_code(name = "dasllama_gemm_gen::q8q8_tile", dot = "vpdpbusd", width = 512, mr = 16, kstep = 4, nrsplit = 4),
 unused_argument(yp, wg, sg, xqp, xsp, xbsp, n, d, g, t0), hint(unsafe_range_check, noalias = wg, noalias = sg, noalias = xqp, noalias = xsp, noalias = xbsp)]
def tile_k4n4p0(var yp : float?; wg : int8 const?; sg : float const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n, d, g, t0 : int64) : void {
    panic("tile4_race: the generator declined k4 n4 pf0")
}

[llvm_code(name = "dasllama_gemm_gen::q8q8_tile", dot = "vpdpbusd", width = 512, mr = 16, kstep = 2, nrsplit = 4, pf = 1024),
 unused_argument(yp, wg, sg, xqp, xsp, xbsp, n, d, g, t0), hint(unsafe_range_check, noalias = wg, noalias = sg, noalias = xqp, noalias = xsp, noalias = xbsp)]
def tile_k2n4p1024(var yp : float?; wg : int8 const?; sg : float const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n, d, g, t0 : int64) : void {
    panic("tile4_race: the generator declined k2 n4 pf1024")
}

[llvm_code(name = "dasllama_gemm_gen::q8q8_tile", dot = "vpdpbusd", width = 512, mr = 16, kstep = 2, nrsplit = 4, pf = 2048),
 unused_argument(yp, wg, sg, xqp, xsp, xbsp, n, d, g, t0), hint(unsafe_range_check, noalias = wg, noalias = sg, noalias = xqp, noalias = xsp, noalias = xbsp)]
def tile_k2n4p2048(var yp : float?; wg : int8 const?; sg : float const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n, d, g, t0 : int64) : void {
    panic("tile4_race: the generator declined k2 n4 pf2048")
}

[llvm_code(name = "dasllama_gemm_gen::q8q8_tile", dot = "vpdpbusd", width = 512, mr = 16, kstep = 2, nrsplit = 4, pf = 4096),
 unused_argument(yp, wg, sg, xqp, xsp, xbsp, n, d, g, t0), hint(unsafe_range_check, noalias = wg, noalias = sg, noalias = xqp, noalias = xsp, noalias = xbsp)]
def tile_k2n4p4096(var yp : float?; wg : int8 const?; sg : float const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n, d, g, t0 : int64) : void {
    panic("tile4_race: the generator declined k2 n4 pf4096")
}

[llvm_code(name = "dasllama_gemm_gen::q8q8_tile", dot = "vpdpbusd", width = 512, mr = 16, kstep = 1, nrsplit = 4, pf = 2048),
 unused_argument(yp, wg, sg, xqp, xsp, xbsp, n, d, g, t0), hint(unsafe_range_check, noalias = wg, noalias = sg, noalias = xqp, noalias = xsp, noalias = xbsp)]
def tile_k1n4p2048(var yp : float?; wg : int8 const?; sg : float const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n, d, g, t0 : int64) : void {
    panic("tile4_race: the generator declined k1 n4 pf2048")
}

[llvm_code(name = "dasllama_gemm_gen::q8q8_tile", dot = "vpdpbusd", width = 512, mr = 16, kstep = 2, nrsplit = 2, pf = 2048),
 unused_argument(yp, wg, sg, xqp, xsp, xbsp, n, d, g, t0), hint(unsafe_range_check, noalias = wg, noalias = sg, noalias = xqp, noalias = xsp, noalias = xbsp)]
def tile_k2n2p2048(var yp : float?; wg : int8 const?; sg : float const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n, d, g, t0 : int64) : void {
    panic("tile4_race: the generator declined k2 n2 pf2048")
}

[llvm_code(name = "dasllama_gemm_gen::q8q8_tile", dot = "vpdpbusd", width = 512, mr = 16, kstep = 4, nrsplit = 4, pf = 2048),
 unused_argument(yp, wg, sg, xqp, xsp, xbsp, n, d, g, t0), hint(unsafe_range_check, noalias = wg, noalias = sg, noalias = xqp, noalias = xsp, noalias = xbsp)]
def tile_k4n4p2048(var yp : float?; wg : int8 const?; sg : float const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n, d, g, t0 : int64) : void {
    panic("tile4_race: the generator declined k4 n4 pf2048")
}

[llvm_code(name = "dasllama_gemm_gen::q8q8_tile", dot = "vpdpbusd", width = 512, mr = 16, kstep = 4, nrsplit = 4, pf = 4096),
 unused_argument(yp, wg, sg, xqp, xsp, xbsp, n, d, g, t0), hint(unsafe_range_check, noalias = wg, noalias = sg, noalias = xqp, noalias = xsp, noalias = xbsp)]
def tile_k4n4p4096(var yp : float?; wg : int8 const?; sg : float const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n, d, g, t0 : int64) : void {
    panic("tile4_race: the generator declined k4 n4 pf4096")
}

struct Shape {
    n : int64
    d : int64
}

struct Planes {
    count : int64
    wbytes : int64          //! one plane's weight bytes
    sfloats : int64         //! one plane's scale floats
    w : array<int8>         //! `count` planes end to end, each in the stamp's layout
    s : array<float>
    xq : array<int8>
    xs : array<float>
    xbs : array<int>        //! the bias sums an unbiased stamp never reads
}

//! `count` copies of one q8 plane in the stamped layout - the bytes repeat, the addresses do not, which is what the stream reads
def private build_planes(sh : Shape; mr : int64) : Planes {
    var p : Planes
    let nb = sh.n / 32l
    p.wbytes = sh.n * sh.d
    p.sfloats = sh.d * nb
    p.count = max(PLANES_BYTES / p.wbytes, 1l)
    var src : array<float>
    src |> reserve(p.wbytes)
    src |> resize(p.wbytes)
    for (i in range64(p.wbytes)) {
        src[i] = 0.5 * sin(float(i) * 0.173) - 0.25 * cos(float(i) * 0.031)
    }
    var q : array<int8>
    var s : array<float>
    q |> resize(p.wbytes)
    s |> resize(p.sfloats)
    quantize_q8_0_into(src, p.wbytes, q, s, 0l, 0l)
    unsafe {
        repack_q8q8_grp(addr(q[0]), addr(s[0]), sh.n, sh.d, mr, int64(q8q8_wbias_gen()), int64(q8q8_kgroup_gen()))
    }
    p.w |> reserve(p.count * p.wbytes)
    p.s |> reserve(p.count * p.sfloats)
    for (_c in range64(p.count)) {
        p.w |> push_from(q)
        p.s |> push_from(s)
    }
    var x : array<float>
    x |> resize(NTOK * sh.n)
    for (i in range64(NTOK * sh.n)) {
        x[i] = 0.9 * sin(float(i) * 0.377) - 0.4 * cos(float(i) * 0.061)
    }
    p.xq |> resize(NTOK * sh.n)
    p.xs |> resize(NTOK * nb)
    p.xbs |> resize(NTOK * nb)
    for (tk in range64(NTOK)) {
        quantize_q8_0_into(x, sh.n, p.xq, p.xs, tk * sh.n, tk * nb)
    }
    delete src
    delete q
    delete s
    delete x
    return <- p
}

//! one arm's four tokens over row groups [rb, re) of the plane at `wp` / `sp`
def private run_groups(arm : int; var yp : float?; wp : int8 const?; sp : float const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n, d, mr : int64; rb, re : int) {
    let nb = n / 32l
    unsafe {
        if (arm == ARM_GEMV) {
            for (tk in range64(NTOK)) {
                q8q8_gemv_gen(yp + tk * d, wp, sp, xqp + tk * n, xsp + tk * nb, n, int64(rb) * mr, int64(re) * mr)
            }
            return
        }
        for (gi in range(rb, re)) {
            let g = int64(gi)
            let wg = wp + g * mr * n
            let sg = sp + g * mr * nb
            if (arm == ARM_STAMPED) {
                q8q8_tile4_gen(yp, wg, sg, xqp, xsp, xbsp, n, d, g, 0l)
            } elif (arm == 2) {
                tile_k1n4p0(yp, wg, sg, xqp, xsp, xbsp, n, d, g, 0l)
            } elif (arm == 3) {
                tile_k2n2p0(yp, wg, sg, xqp, xsp, xbsp, n, d, g, 0l)
            } elif (arm == 4) {
                tile_k2n4p0(yp, wg, sg, xqp, xsp, xbsp, n, d, g, 0l)
            } elif (arm == 5) {
                tile_k4n4p0(yp, wg, sg, xqp, xsp, xbsp, n, d, g, 0l)
            } elif (arm == 6) {
                tile_k2n4p1024(yp, wg, sg, xqp, xsp, xbsp, n, d, g, 0l)
            } elif (arm == 7) {
                tile_k2n4p2048(yp, wg, sg, xqp, xsp, xbsp, n, d, g, 0l)
            } elif (arm == 8) {
                tile_k2n4p4096(yp, wg, sg, xqp, xsp, xbsp, n, d, g, 0l)
            } elif (arm == 9) {
                tile_k1n4p2048(yp, wg, sg, xqp, xsp, xbsp, n, d, g, 0l)
            } elif (arm == 10) {
                tile_k2n2p2048(yp, wg, sg, xqp, xsp, xbsp, n, d, g, 0l)
            } elif (arm == 11) {
                tile_k4n4p2048(yp, wg, sg, xqp, xsp, xbsp, n, d, g, 0l)
            } else {
                tile_k4n4p4096(yp, wg, sg, xqp, xsp, xbsp, n, d, g, 0l)
            }
        }
    }
}

//! one arm's step over planes [first, first + count): the batch kernel's vector walk, a plane at a time
def private step(arm : int; p : Planes; sh : Shape; mr : int64; first, count : int64; var y : array<float>) {
    let ng = int(sh.d / mr)
    let grid = matmul_grid(ng, mr, get_q8_batch_chunks_per_job(), sh.n * sh.d * NTOK, NTOK, 4)
    unsafe {
        var yp = addr(y[0])
        let xqp = addr<int8 const?>(p.xq[0])
        let xsp = addr<float const?>(p.xs[0])
        let xbsp = addr<int const?>(p.xbs[0])
        for (c in range64(first, first + count)) {
            let wp = addr<int8 const?>(p.w[c * p.wbytes])
            let sp = addr<float const?>(p.s[c * p.sfloats])
            maybe_parallel_for(0, ng, grid.x) $(rb, re) {
                run_groups(arm, yp, wp, sp, xqp, xsp, xbsp, sh.n, sh.d, mr, rb, re)
            }
        }
    }
}

//! one shape's arms; answers the rows it reported, or -1 when an arm sat outside its bound
def private race(sh : Shape; mr : int64) : int {
    var p <- build_planes(sh, mr)
    let narms = length(ARM_NAMES)
    let base = find_index(ARM_NAMES, BASELINE)
    var y : array<float>
    y |> resize(NTOK * sh.d)
    var images : array<array<float>>
    images |> reserve(narms)
    for (arm in range(narms)) {
        poison(y)
        step(arm, p, sh, mr, 0l, 1l, y)
        var inscope img := y
        images |> emplace(img)
    }
    var times <- time_rounds(narms, ROUNDS, WARMUP) $(arm : int) {
        step(arm, p, sh, mr, 0l, p.count, y)
    }
    var rows = 0
    var good = base >= 0
    for (arm in range(narms)) {
        let st = stats(times[arm])
        let h = (arm == ARM_GEMV ? held(images[arm], images[arm], 0.0)
            : arm == ARM_STAMPED ? held(images[arm], images[base], 0.0)
            : held(images[arm], images[ARM_GEMV], FLOAT_ORDER))
        let words = (arm == ARM_GEMV ? "the reference arm"
            : compare_words(h, arm == ARM_STAMPED ? "the {BASELINE} arm" : "the gemv arm", "{length(images[arm])} elements"))
        let gmac = double(NTOK * sh.n * sh.d * p.count) / double(st.best) / 1.0e6lf
        print("K={sh.n} rows={sh.d} ntok={NTOK} planes={p.count} lanes={get_dispatch_lanes()} arm={ARM_NAMES[arm]}{arm == base ? " (baseline)" : ""} best_ms={st.best} median_ms={st.median} cv%={st.cv} GMAC/s={gmac} | {words}\n")
        good &&= h.ok
        rows++
    }
    delete p
    delete y
    delete images
    delete times
    return good ? rows : -1
}

[export]
def main() : int {
    var t : RaceTally
    with_dasllama_jobque_() {
        let mr = int64(q8q8_layout_gen())
        if (q8q8_tileform_gen() <= 0 || mr != 16l || q8q8_wbias_gen() != 0) {
            print("tile4_race: this box's q8 stamp is no amx bf16 tile (tile form {q8q8_tileform_gen()}, mr {mr}, wbias {q8q8_wbias_gen()}) - the candidates read that stamp's plane - no row\n")
            t.bad++
        } elif (!tune_gate()) {
            print("tile4_race: the tune gate refused - no row\n")
            t.bad++
        } else {
            print("tile4_race: {get_dispatch_lanes()} lanes, {ROUNDS} rounds after {WARMUP} dropped, {PLANES_BYTES >> 20l} MB of planes a shape\n")
            for (sh in [Shape(n = 1536l, d = 6144l), Shape(n = 6144l, d = 1536l), Shape(n = 2560l, d = 9728l), Shape(n = 9728l, d = 2560l)]) {
                t |> tally(race(sh, mr))
            }
        }
    }
    return race_exit(t, "tile4_race")
}
