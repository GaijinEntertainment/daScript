options gen2
options persistent_heap
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options _dasllama_internal = true

require daslib/clargs
require daslib/enum_trait
require daslib/fio
require daslib/jobque_boost
require dasllama/dasllama_par
require dasllama/dasllama_math_gen
require dasllama/dasllama_math
require dasllama/dasllama_kqformat
require dasllama/dasllama_gemm_schema
require math
require strings
require daslib/strings_boost

//! The kernel microbench - the inner loop of kernel work. Raw kernels, one thread, no jobque,
//! no model; synthetic planes, because the timing is data-independent. GEMV is the decode shape
//! (one token), TILE the prefill batch. Every row of a kernel's `_variants()` registry is timed,
//! the rows of one format interleaved round-robin so frequency drift cancels: under
//! DAS_TUNE_MODE=tune that is the whole [tune_perm] grid, under normal mode the stamped winner
//! alone. Timing only - a spelling's correctness is gen_tune_probe's TEST mode.
//! Run: DAS_TUNE_MODE=tune bin/daslang -jit modules/dasLLAMA/benchmarks/matmul/kq_kernel_bench.das -- --fmt iq3s
//! The reference row at the same shape: GGML_BENCH_THREADS=1 test-backend-ops perf -o MUL_MAT -p "type_a=iq3_s"
//! (its default MUL_MAT perf case is m=4096 rows, k=14336, n in {1, 512} tokens - this bench's defaults);
//! harness/kernel_ladder.sh runs both sides for every format and joins the table.

[CommandLineArgs]
struct BenchArgs {
    @clarg_short = "f"
    @clarg_doc = "KqFmt names, comma-separated (q8,k4,k5,k6,q40,q51,iq4xs,k3,iq3s,iq3xxs,iq4nl,k2,iq2s,iq2xs,iq2xxs; mx4 = mxfp4), or all"
    fmt : string = "all"
    @clarg_doc = "Reduction length - the weight row length (k in test-backend-ops terms)"
    n : int = 14336
    @clarg_doc = "Rows - output features (m in test-backend-ops terms)"
    d : int = 4096
    @clarg_doc = "Tokens for the TILE arm (the prefill batch); 0 skips the arm"
    ntok : int = 512
    @clarg_doc = "Timed rounds per row, interleaved across the format's rows; best and median are reported"
    rounds : int = 5
    @clarg_doc = "Skip the GEMV (decode) arm"
    no_gemv : bool
    @clarg_short = "p"
    @clarg_doc = "Registry rows to time - comma-separated substrings of the perm suffix, =name for an exact row (default: every row, the reference body included)"
    perm : string
    @clarg_doc = "TSV rows only, no banner"
    tsv : bool
    @clarg_doc = "Arena base alignment in bytes (64; 4096 fixes the planes' phase against page boundaries, which the heap otherwise draws per process)"
    base_align : int = 64
    @clarg_doc = "Bytes added past the aligned arena base (with --base-align 4096: the planes' phase inside the page)"
    base_offset : int = 0
    @clarg_doc = "GEMV the engine's way: the engine's own splitter (matmul_chunks_gemv) and dispatcher (maybe_parallel_for_indexed) over DAS_JOBQUE_THREADS lanes, wall = the slowest lane, the lanes' fastest and slowest summed call time in two extra columns; 0 = one raw call on this thread"
    team : bool
    @clarg_doc = "Append every round's sample (us, in round order) as one more column - the per-round view of a bimodal row"
    each : bool
    @clarg_doc = "--team: gemv chunks per lane for the engine's splitter (0 = the engine default, 8; 1 = one equal chunk per lane)"
    chunks_per_lane : int = 0
}

typedef GemvFn = function<(var yp : float?; kqp : uint8 const?; ksp : uint8 const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n, rb, re : int64) : void>
typedef TileFn = function<(var yp : float?; kqg : uint8 const?; ksg : uint8 const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n, d, g, t0 : int64) : void>
typedef GemvQ8Fn = function<(var yp : float?; wp : int8 const?; sp : float const?; xqp : int8 const?; xsp : float const?; n, rb, re : int64) : void>
typedef TileQ8Fn = function<(var yp : float?; wg : int8 const?; sg : float const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n, d, g, t0 : int64) : void>
typedef GemvMx4Fn = function<(var yp : float?; wn : uint8 const?; we : uint8 const?; xqp : int8 const?; xsp : float const?; n, rb, re : int64) : void>
typedef LayoutFn = function<() : int>

let TILE_TOKENS = 4l   // tokens one kq / q51 / mx4 tile call covers (q8's tokstep companion says its own)
let MX4_NAME = "mx4"   // mxfp4 has no KqFmt row; it rides the q8q8 grid as a companion

def fmt_of_name(name : string) : KqFmt {
    for (f in type<KqFmt>) {
        if ("{f}" == name) {
            return f
        }
    }
    panic("unknown kq format '{name}'")
    return KqFmt.q8
}

def gemv_variants(f : KqFmt) : array<tuple<string; GemvFn>> {
    if (f == KqFmt.k4) return <- k4q8_gemv_gen_variants()
    if (f == KqFmt.k5) return <- k5q8_gemv_gen_variants()
    if (f == KqFmt.k6) return <- k6q8_gemv_gen_variants()
    if (f == KqFmt.q40) return <- q40q8_gemv_gen_variants()
    if (f == KqFmt.iq4xs) return <- iq4xsq8_gemv_gen_variants()
    if (f == KqFmt.k3) return <- k3q8_gemv_gen_variants()
    if (f == KqFmt.iq3s) return <- iq3sq8_gemv_gen_variants()
    if (f == KqFmt.iq3xxs) return <- iq3xxsq8_gemv_gen_variants()
    if (f == KqFmt.iq4nl) return <- iq4nlq8_gemv_gen_variants()
    if (f == KqFmt.k2) return <- k2q8_gemv_gen_variants()
    if (f == KqFmt.iq2s) return <- iq2sq8_gemv_gen_variants()
    if (f == KqFmt.iq2xs) return <- iq2xsq8_gemv_gen_variants()
    if (f == KqFmt.iq2xxs) return <- iq2xxsq8_gemv_gen_variants()
    if (f == KqFmt.q51) return <- q51q8_gemv_gen_variants()
    var none : array<tuple<string; GemvFn>>
    return <- none
}

def tile_variants(f : KqFmt) : array<tuple<string; TileFn>> {
    if (f == KqFmt.k4) return <- k4q8_tile_gen_variants()
    if (f == KqFmt.k5) return <- k5q8_tile_gen_variants()
    if (f == KqFmt.k6) return <- k6q8_tile_gen_variants()
    if (f == KqFmt.q40) return <- q40q8_tile_gen_variants()
    if (f == KqFmt.iq4xs) return <- iq4xsq8_tile_gen_variants()
    if (f == KqFmt.k3) return <- k3q8_tile_gen_variants()
    if (f == KqFmt.iq3s) return <- iq3sq8_tile_gen_variants()
    if (f == KqFmt.iq3xxs) return <- iq3xxsq8_tile_gen_variants()
    if (f == KqFmt.iq4nl) return <- iq4nlq8_tile_gen_variants()
    if (f == KqFmt.k2) return <- k2q8_tile_gen_variants()
    if (f == KqFmt.iq2s) return <- iq2sq8_tile_gen_variants()
    if (f == KqFmt.iq2xs) return <- iq2xsq8_tile_gen_variants()
    if (f == KqFmt.iq2xxs) return <- iq2xxsq8_tile_gen_variants()
    if (f == KqFmt.q51) return <- q51q8_tile_gen_variants()
    var none : array<tuple<string; TileFn>>
    return <- none
}

def layout_variants(f : KqFmt) : array<tuple<string; LayoutFn>> {
    if (f == KqFmt.k4) return <- k4q8_layout_gen_variants()
    if (f == KqFmt.k5) return <- k5q8_layout_gen_variants()
    if (f == KqFmt.k6) return <- k6q8_layout_gen_variants()
    if (f == KqFmt.q40) return <- q40q8_layout_gen_variants()
    if (f == KqFmt.iq4xs) return <- iq4xsq8_layout_gen_variants()
    if (f == KqFmt.k3) return <- k3q8_layout_gen_variants()
    if (f == KqFmt.iq3s) return <- iq3sq8_layout_gen_variants()
    if (f == KqFmt.iq3xxs) return <- iq3xxsq8_layout_gen_variants()
    if (f == KqFmt.iq4nl) return <- iq4nlq8_layout_gen_variants()
    if (f == KqFmt.k2) return <- k2q8_layout_gen_variants()
    if (f == KqFmt.iq2s) return <- iq2sq8_layout_gen_variants()
    if (f == KqFmt.iq2xs) return <- iq2xsq8_layout_gen_variants()
    if (f == KqFmt.iq2xxs) return <- iq2xxsq8_layout_gen_variants()
    if (f == KqFmt.q51) return <- q51q8_layout_gen_variants()
    return <- q8q8_layout_gen_variants()   // q8 and its mx4 companion share the q8q8 layout
}

//! an int-valued companion registry (layout mr, q8's tokstep) keyed by suffix; the companion is
//! stamped in lockstep with its tile, so the row of the same suffix belongs to it
def companion_ints(var lvs : array<tuple<string; LayoutFn>>) : table<string; int64> {
    var vals <- {for (v in lvs); v._0 => int64(invoke(v._1))}
    delete lvs
    return <- vals
}

//! a TILE reads either the packed quant plane or a caller-unpacked byte panel (256 bytes per row per superblock: k5, k6 and the five grid formats) - kq_reads_packed_planes is the truth
def tile_kq_bytes_per_sb(f : KqFmt) : int64 => kq_reads_packed_planes(int64(kq_schema_id(f))) ? kq_qsb(f) : 256l

//! plane bytes per row: kq formats per 256-superblock, q51 per 32-block (20 + 4), q8 n int8 + n/32 f32, mx4 n/2 nibbles + n/32 e8m0
struct PlaneShape {
    wq_row : int64   // the gemv's weight plane bytes per row
    tq_row : int64   // the tile's weight plane bytes per row
    ws_row : int64   // scale plane bytes per row
}

def plane_shape(f : KqFmt; mx4 : bool; n : int64) : PlaneShape {
    if (mx4) {
        return PlaneShape(wq_row = n / 2l, tq_row = n / 2l, ws_row = n / 32l)
    }
    if (f == KqFmt.q8) {
        return PlaneShape(wq_row = n, tq_row = n, ws_row = (n / 32l) * 4l)
    }
    if (f == KqFmt.q51) {
        return PlaneShape(wq_row = (n / 32l) * 20l, tq_row = (n / 32l) * 20l, ws_row = (n / 32l) * 4l)
    }
    let nsb = n / 256l
    return PlaneShape(wq_row = nsb * kq_qsb(f), tq_row = nsb * tile_kq_bytes_per_sb(f), ws_row = nsb * kq_ssb(f))
}

var BASE_ALIGN = 64l   // the arena's start alignment - --base-align; the planes' phase against 4 KB pages follows it
var BASE_OFFSET = 0l   // --base-offset, added past the aligned start
let ALIGN = 64l   // the image's planes are page-aligned (ARCHITECTURE_IMAGE.md, image-page-alignment); a 64-byte load split across cache lines is what Intel penalizes 2x
let STAGGER = 1088l   // plane starts differ in their low 12 address bits (1088 = 1024 + 64 keeps six starts distinct mod 4096): planes that alias in the L1/L2 set logic make a run's time depend on where the heap put them
let SCALE_BYTE = uint8(0x3C)   // as f16 0x3C3C = 1.06, as f32 0x3C3C3C3C = 0.0115, as e8m0 2^-67, as a 6-bit sub-scale 60 - normal in every scale form

//! every plane of one format in ONE arena at fixed offsets: the heap places separate arrays at
//! run-dependent relative addresses, and k6 decode read 3.9-6.0 ms across identical runs from that alone
struct Planes {
    arena : array<uint8>
    kq : int64    // the gemv's weight plane
    tkq : int64   // the tile's weight plane (the unpacked panel form where the format has one)
    ks : int64    // scale plane
    xq : int64    // int8 activations
    xs : int64    // f32 activation scales
    xbs : int64   // int per-16 activation sums
    y : int64     // f32 outputs
}

//! the next plane start: 64-byte aligned, `slot` staggers past the running end
def next_plane(var end : int64&; bytes : int64; slot : int) : int64 {
    let start = ((end + ALIGN - 1l) / ALIGN) * ALIGN + int64(slot) * STAGGER
    end = start + bytes
    return start
}

//! the arena's first BASE_ALIGN-aligned byte
def arena_base(var p : Planes) : int64 {
    unsafe {
        return ((BASE_ALIGN - int64(intptr(addr<void?>(p.arena[0])) & uint64(BASE_ALIGN - 1l))) & (BASE_ALIGN - 1l)) + BASE_OFFSET
    }
}

def fill_planes(var p : Planes; sh : PlaneShape; n, d, ntok : int64) {
    var end = 0l
    p.kq = next_plane(end, d * sh.wq_row, 0)
    p.tkq = next_plane(end, d * sh.tq_row, 1)
    p.ks = next_plane(end, d * sh.ws_row, 2)
    p.xq = next_plane(end, ntok * n, 3)
    p.xs = next_plane(end, ntok * n / 32l * 4l, 4)
    p.xbs = next_plane(end, ntok * n / 16l * 4l, 5)
    p.y = next_plane(end, ntok * d * 4l, 6)
    p.arena |> reserve(end + BASE_ALIGN + BASE_OFFSET)
    p.arena |> resize(end + BASE_ALIGN + BASE_OFFSET)
    let base = arena_base(p)
    p.kq += base
    p.tkq += base
    p.ks += base
    p.xq += base
    p.xs += base
    p.xbs += base
    p.y += base
    var seed = 0x1234u
    for (i in range64(p.kq, p.kq + d * sh.wq_row)) {
        seed = seed * 1664525u + 1013904223u
        p.arena[i] = uint8(seed >> 24u)
    }
    for (i in range64(p.tkq, p.tkq + d * sh.tq_row)) {
        seed = seed * 1664525u + 1013904223u
        p.arena[i] = uint8(seed >> 24u)
    }
    for (i in range64(p.ks, p.ks + d * sh.ws_row)) {
        p.arena[i] = SCALE_BYTE   // scale planes hold normal numbers in every scale form, never random bytes: Intel runs denormal math ~100x slower
    }
    for (i in range64(p.xq, p.xq + ntok * n)) {
        seed = seed * 1664525u + 1013904223u
        p.arena[i] = uint8(seed >> 24u)
    }
    unsafe {
        var xs = addr<float?>(p.arena[p.xs])
        for (i in range64(ntok * n / 32l)) {
            xs[i] = 0.01
        }
        var xbs = addr<int?>(p.arena[p.xbs])
        for (i in range64(ntok * n / 16l)) {
            xbs[i] = 128
        }
    }
}

//! one timed row: the samples of every round; best and median are reported
struct RowStat {
    perm : string
    us : array<int64>
    lane_lo : int64   // --team: the fastest single lane call seen across the rounds
    lane_hi : int64   // --team: the slowest
}

let TEAM_BURN = 6   // warmup dispatches per row before the team arm times anything
var g_each_round = false   // --each

//! the engine's decode dispatch, verbatim (dasllama_math's matmul shape): the splitter's chunk count over the
//! rows, the team self-serving them; each lane adds its call time to laneP[slot]
def team_gemv(fn : GemvFn; var yp : float?; kqp, ksp : uint8 const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n, d : int64; var laneP : int64?) {
    maybe_parallel_for_indexed(0, int(d), matmul_chunks_gemv(int(d), 1, n * d)) $(slot : int; rb : int; re : int) {
        let l0 = ref_time_ticks()
        invoke(fn, yp, kqp, ksp, xqp, xsp, xbsp, n, int64(rb), int64(re))
        unsafe {
            laneP[slot] += int64(get_time_usec(l0))
        }
    }
}

def team_gemv_q8(fn : GemvQ8Fn; var yp : float?; wp : int8 const?; sp : float const?; xqp : int8 const?; xsp : float const?; n, d : int64; var laneP : int64?) {
    maybe_parallel_for_indexed(0, int(d), matmul_chunks_gemv(int(d), 1, n * d)) $(slot : int; rb : int; re : int) {
        let l0 = ref_time_ticks()
        invoke(fn, yp, wp, sp, xqp, xsp, n, int64(rb), int64(re))
        unsafe {
            laneP[slot] += int64(get_time_usec(l0))
        }
    }
}

def team_gemv_mx4(fn : GemvMx4Fn; var yp : float?; wn, we : uint8 const?; xqp : int8 const?; xsp : float const?; n, d : int64; var laneP : int64?) {
    maybe_parallel_for_indexed(0, int(d), matmul_chunks_gemv(int(d), 1, n * d)) $(slot : int; rb : int; re : int) {
        let l0 = ref_time_ticks()
        invoke(fn, yp, wn, we, xqp, xsp, n, int64(rb), int64(re))
        unsafe {
            laneP[slot] += int64(get_time_usec(l0))
        }
    }
}

//! the team arm's interleaved timing: the block dispatches row `row` the engine's way and every lane adds its
//! call time to laneP[slot]; the row keeps the wall (best, median) and the lanes' fastest and slowest sums
def time_rows_team(var stats : array<RowStat>; rounds : int; blk : block<(row : int; var laneP : int64?) : void>) {
    var laneUs : array<int64>
    laneUs |> resize(get_total_hw_jobs() + 1)
    var laneP = unsafe(addr(laneUs[0]))
    for (row in range(length(stats))) {
        stats[row].us |> reserve(rounds)
        stats[row].lane_lo = 999999999l
        for (_w in range(TEAM_BURN)) {   // codegen, then a burn: the first format's team rows read one slot at 8 ms without it
            invoke(blk, row, laneP)
        }
    }
    for (_r in range(rounds)) {
        for (row in range(length(stats))) {
            for (u in laneUs) {
                u = 0l
            }
            let t0 = ref_time_ticks()
            invoke(blk, row, laneP)
            stats[row].us |> push(int64(get_time_usec(t0)))
            for (u in laneUs) {
                if (u > 0l) {
                    stats[row].lane_lo = min(stats[row].lane_lo, u)
                    stats[row].lane_hi = max(stats[row].lane_hi, u)
                }
            }
        }
    }
}

def median_us(var us : array<int64>) : int64 {
    var s := us
    sort(s)
    let m = length(s) / 2
    let r = length(s) % 2 == 1 ? s[m] : (s[m - 1] + s[m]) / 2l
    delete s
    return r
}

def emit_row(var rows : int&; name, tier : string; var st : RowStat; n, d, ntok, nsb : int64; plane_bytes : int64) {
    var best = 999999999l
    for (u in st.us) {
        best = min(best, u)
    }
    let med = median_us(st.us)
    let ns_per_sb = float(best) * 1000.0 / float(d * nsb * ntok)
    let gbs = float(plane_bytes) / (float(best) * 1000.0)
    let gflops = 2.0 * float(n) * float(d) * float(ntok) / (float(best) * 1000.0)
    let mode = get_env_variable("DAS_TUNE_MODE")
    let perm = (st.perm == "reference" && mode != "tune" && mode != "test") ? "stamped" : st.perm   // normal mode: the registry's one row IS the stamped winner
    let lanes = st.lane_hi > 0l ? "\t{st.lane_lo}\t{st.lane_hi}" : ""
    var each = ""
    if (g_each_round) {
        each = "\t" + join(st.us, ",")
    }
    print("{name}\t{tier}\t{perm}\t{n}\t{d}\t{ntok}\t{best}\t{med}\t{ns_per_sb}\t{gbs}\t{gflops}{lanes}{each}\n")
    rows++
}

def perm_wanted(spec, suffix : string) : bool {
    if (empty(spec)) {
        return true
    }
    for (tok in split(spec, ",")) {
        let tk = strip(tok)
        if (empty(tk)) {
            continue
        }
        if (starts_with(tk, "=") ? suffix == slice(tk, 1) : find(suffix, tk) >= 0) {
            return true
        }
    }
    return false
}

//! interleaved timing: round r calls every selected row once before round r + 1 starts
def time_rows(var stats : array<RowStat>; rounds : int; blk : block<(row : int) : void>) {
    for (row in range(length(stats))) {
        stats[row].us |> reserve(rounds)
        invoke(blk, row)   // warmup + codegen, unmeasured
    }
    for (_r in range(rounds)) {
        for (row in range(length(stats))) {
            let t0 = ref_time_ticks()
            invoke(blk, row)
            stats[row].us |> push(int64(get_time_usec(t0)))
        }
    }
}

def bench_gemv_kq(var rows : int&; cfg : BenchArgs; name : string; f : KqFmt; var p : Planes; nsb : int64; sh : PlaneShape) {
    let n = int64(cfg.n)
    let d = int64(cfg.d)
    let plane_bytes = int64(cfg.d) * (sh.wq_row + sh.ws_row)
    var vs <- gemv_variants(f)
    var stats : array<RowStat>
    var fns : array<GemvFn>
    for (v in vs) {
        if (perm_wanted(cfg.perm, v._0)) {
            stats |> emplace(RowStat(perm = v._0))
            fns |> push(v._1)
        }
    }
    unsafe {
        let kqp = addr<uint8 const?>(p.arena[p.kq])
        let ksp = addr<uint8 const?>(p.arena[p.ks])
        let xqp = addr<int8 const?>(p.arena[p.xq])
        let xsp = addr<float const?>(p.arena[p.xs])
        let xbsp = addr<int const?>(p.arena[p.xbs])
        var yp = addr<float?>(p.arena[p.y])
        if (cfg.team) {
            time_rows_team(stats, cfg.rounds) $(row : int; var laneP : int64?) {
                team_gemv(fns[row], yp, kqp, ksp, xqp, xsp, xbsp, n, d, laneP)
            }
        } else {
            time_rows(stats, cfg.rounds) $(row : int) {
                invoke(fns[row], yp, kqp, ksp, xqp, xsp, xbsp, n, 0l, d)
            }
        }
    }
    for (st in stats) {
        emit_row(rows, name, "gemv", st, n, d, 1l, nsb, plane_bytes)
    }
    delete stats
    delete fns
    delete vs
}

def bench_tile_kq(var rows : int&; cfg : BenchArgs; name : string; f : KqFmt; var p : Planes; nsb : int64; sh : PlaneShape) {
    let n = int64(cfg.n)
    let d = int64(cfg.d)
    let ntok = int64(cfg.ntok)
    let qrow = sh.tq_row
    let srow = sh.ws_row
    let plane_bytes = int64(cfg.d) * (sh.tq_row + sh.ws_row)
    var mrs <- companion_ints(layout_variants(f))
    var vs <- tile_variants(f)
    var stats : array<RowStat>
    var fns : array<TileFn>
    var mrOf : array<int64>
    for (v in vs) {
        if (!perm_wanted(cfg.perm, v._0)) {
            continue
        }
        let mr = mrs?[v._0] ?? (mrs?["reference"] ?? 0l)
        if (mr <= 0l || d % mr != 0l) {
            to_log(LOG_WARNING, "kq_kernel_bench: {name} tile '{v._0}' has no layout row (mr={mr}) - skipped\n")
            continue
        }
        stats |> emplace(RowStat(perm = v._0))
        fns |> push(v._1)
        mrOf |> push(mr)
    }
    unsafe {
        let kqp = addr<uint8 const?>(p.arena[p.tkq])
        let ksp = addr<uint8 const?>(p.arena[p.ks])
        let xqp = addr<int8 const?>(p.arena[p.xq])
        let xsp = addr<float const?>(p.arena[p.xs])
        let xbsp = addr<int const?>(p.arena[p.xbs])
        var yp = addr<float?>(p.arena[p.y])
        time_rows(stats, cfg.rounds) $(row : int) {
            let mr = mrOf[row]
            for (g in range64(d / mr)) {
                var t0k = 0l
                while (t0k < ntok) {
                    invoke(fns[row], yp, kqp + g * mr * qrow, ksp + g * mr * srow, xqp, xsp, xbsp, n, d, g, t0k)
                    t0k += TILE_TOKENS
                }
            }
        }
    }
    for (st in stats) {
        emit_row(rows, name, "tile", st, n, d, ntok, nsb, plane_bytes)
    }
    delete stats
    delete fns
    delete mrOf
    delete vs
    delete mrs
}

//! the q8q8 grid's gemv rows (q8 or its mx4 companion) into stats + typed function lists
def q8_gemv_rows(cfg : BenchArgs; mx4 : bool; var stats : array<RowStat>; var fnsQ : array<GemvQ8Fn>; var fnsM : array<GemvMx4Fn>) {
    if (mx4) {
        var vs <- mx4q8_gemv_gen_variants()
        for (v in vs) {
            if (perm_wanted(cfg.perm, v._0)) {
                stats |> emplace(RowStat(perm = v._0))
                fnsM |> push(v._1)
            }
        }
        delete vs
    } else {
        var vs <- q8q8_gemv_gen_variants()
        for (v in vs) {
            if (perm_wanted(cfg.perm, v._0)) {
                stats |> emplace(RowStat(perm = v._0))
                fnsQ |> push(v._1)
            }
        }
        delete vs
    }
}

//! the q8q8 grid's tile rows with each row's mr and tokens-per-call; skips rows whose mr the row count does not divide
def q8_tile_rows(cfg : BenchArgs; mx4 : bool; d : int64; var stats : array<RowStat>; var fnsQ : array<TileQ8Fn>; var fnsM : array<TileFn>; var mrOf, stepOf : array<int64>) {
    var mrs <- companion_ints(q8q8_layout_gen_variants())
    var steps <- companion_ints(q8q8_tokstep_gen_variants())
    if (mx4) {
        var vs <- mx4q8_tile_gen_variants()
        for (v in vs) {
            let mr = mrs?[v._0] ?? (mrs?["reference"] ?? 0l)
            if (perm_wanted(cfg.perm, v._0) && mr > 0l && d % mr == 0l) {
                stats |> emplace(RowStat(perm = v._0))
                fnsM |> push(v._1)
                mrOf |> push(mr)
                stepOf |> push(TILE_TOKENS)
            }
        }
        delete vs
    } else {
        var vs <- q8q8_tile_gen_variants()
        for (v in vs) {
            let mr = mrs?[v._0] ?? (mrs?["reference"] ?? 0l)
            if (perm_wanted(cfg.perm, v._0) && mr > 0l && d % mr == 0l) {
                stats |> emplace(RowStat(perm = v._0))
                fnsQ |> push(v._1)
                mrOf |> push(mr)
                stepOf |> push(steps?[v._0] ?? (steps?["reference"] ?? TILE_TOKENS))
            }
        }
        delete vs
    }
    delete mrs
    delete steps
}

//! q8 (the q8q8 grid) and its mx4 companion: int8 / nibble weight planes with f32 / e8m0 scales
def bench_q8_family(var rows : int&; cfg : BenchArgs; mx4 : bool; var p : Planes; nsb : int64; sh : PlaneShape) {
    let n = int64(cfg.n)
    let d = int64(cfg.d)
    let ntok = int64(cfg.ntok)
    let name = mx4 ? MX4_NAME : "q8"
    let plane_bytes = int64(cfg.d) * (sh.wq_row + sh.ws_row)
    var wvs <- q8q8_family_live_variants()   // the family's witness rows: the amx one takes the per-process tile permission its tiles need (SIGILL without it), as the probe does
    for (w in wvs) {
        invoke(w._1)
    }
    delete wvs
    unsafe {
        let kqp = addr<uint8 const?>(p.arena[p.kq])
        let ksp = addr<uint8 const?>(p.arena[p.ks])
        let xqp = addr<int8 const?>(p.arena[p.xq])
        let xsp = addr<float const?>(p.arena[p.xs])
        let xbsp = addr<int const?>(p.arena[p.xbs])
        var yp = addr<float?>(p.arena[p.y])
        if (!cfg.no_gemv) {
            var stats : array<RowStat>
            var fnsQ : array<GemvQ8Fn>
            var fnsM : array<GemvMx4Fn>
            q8_gemv_rows(cfg, mx4, stats, fnsQ, fnsM)
            if (cfg.team) {
                time_rows_team(stats, cfg.rounds) $(row : int; var laneP : int64?) {
                    if (mx4) {
                        team_gemv_mx4(fnsM[row], yp, kqp, ksp, xqp, xsp, n, d, laneP)
                    } else {
                        team_gemv_q8(fnsQ[row], yp, reinterpret<int8 const?>(kqp), reinterpret<float const?>(ksp), xqp, xsp, n, d, laneP)
                    }
                }
            } else {
                time_rows(stats, cfg.rounds) $(row : int) {
                    if (mx4) {
                        invoke(fnsM[row], yp, kqp, ksp, xqp, xsp, n, 0l, d)
                    } else {
                        invoke(fnsQ[row], yp, reinterpret<int8 const?>(kqp), reinterpret<float const?>(ksp), xqp, xsp, n, 0l, d)
                    }
                }
            }
            for (st in stats) {
                emit_row(rows, name, "gemv", st, n, d, 1l, nsb, plane_bytes)
            }
            delete stats
            delete fnsQ
            delete fnsM
        }
        if (ntok > 0l) {
            var stats : array<RowStat>
            var fnsQ : array<TileQ8Fn>
            var fnsM : array<TileFn>
            var mrOf : array<int64>
            var stepOf : array<int64>
            q8_tile_rows(cfg, mx4, d, stats, fnsQ, fnsM, mrOf, stepOf)
            time_rows(stats, cfg.rounds) $(row : int) {
                let mr = mrOf[row]
                for (g in range64(d / mr)) {
                    var t0k = 0l
                    while (t0k < ntok) {
                        if (mx4) {
                            invoke(fnsM[row], yp, kqp + g * mr * sh.tq_row, ksp + g * mr * sh.ws_row, xqp, xsp, xbsp, n, d, g, t0k)
                        } else {
                            invoke(fnsQ[row], yp, reinterpret<int8 const?>(kqp + g * mr * sh.tq_row), reinterpret<float const?>(ksp + g * mr * sh.ws_row), xqp, xsp, xbsp, n, d, g, t0k)
                        }
                        t0k += stepOf[row]
                    }
                }
            }
            for (st in stats) {
                emit_row(rows, name, "tile", st, n, d, ntok, nsb, plane_bytes)
            }
            delete stats
            delete fnsQ
            delete fnsM
            delete mrOf
            delete stepOf
        }
    }
}

//! the requested formats as (name, KqFmt, is_mx4); "all" = every KqFmt row plus mx4
def formats_of(spec : string) : array<tuple<string; KqFmt; bool>> {
    var fs : array<tuple<string; KqFmt; bool>>
    if (spec == "all") {
        for (f in type<KqFmt>) {
            fs |> emplace(("{f}", f, false))
        }
        fs |> emplace((MX4_NAME, KqFmt.q8, true))
        return <- fs
    }
    for (tok in split(spec, ",")) {
        let name = strip(tok)
        if (empty(name)) {
            continue
        }
        if (name == MX4_NAME) {
            fs |> emplace((MX4_NAME, KqFmt.q8, true))
        } else {
            fs |> emplace((name, fmt_of_name(name), false))
        }
    }
    return <- fs
}

[export]
def main : int {
    var cfg = BenchArgs()
    let rc = parse_args_with_help(cfg, "kq_kernel_bench")
    BASE_ALIGN = int64(cfg.base_align)
    BASE_OFFSET = int64(cfg.base_offset)
    g_each_round = cfg.each
    if (cfg.chunks_per_lane > 0) {
        set_gemv_chunks_per_lane(cfg.chunks_per_lane)
    }
    return rc if (rc >= 0)
    if (!jit_enabled()) {
        to_log(LOG_ERROR, "run with -jit: the kq generators only fire under the JIT\n")
        return 2
    }
    if (cfg.n % 256 != 0 || cfg.ntok % int(TILE_TOKENS) != 0) {
        to_log(LOG_ERROR, "--n must be a multiple of 256 and --ntok of {TILE_TOKENS}\n")
        return 2
    }
    var fs <- formats_of(cfg.fmt)
    if (!cfg.tsv) {
        print("kq_kernel_bench: n={cfg.n} d={cfg.d} ntok={cfg.ntok} rounds={cfg.rounds} team={cfg.team} tune_mode={get_env_variable("DAS_TUNE_MODE")}\n")
        print("fmt\ttier\tperm\tn\td\tntok\tbest_us\tmedian_us\tns_per_sb\tGB/s\tGFLOP/s{cfg.team ? "\tlane_lo\tlane_hi" : ""}\n")
    }
    var rows = 0
    if (cfg.team) {
        with_job_que() {
            setup_dasllama_jobque_()
            let chunks = matmul_chunks_gemv(cfg.d, 1, int64(cfg.n) * int64(cfg.d))
            print("# team: {get_total_hw_jobs() + 1} lanes, the engine's splitter gives {chunks} chunks of {cfg.d / chunks} rows\n")
            rows = run_formats(cfg, fs)
        }
    } else {
        rows = run_formats(cfg, fs)
    }
    delete fs
    if (rows == 0) {
        to_log(LOG_ERROR, "kq_kernel_bench: no rows - nothing matched --fmt '{cfg.fmt}' / --perm '{cfg.perm}' or every arm was skipped\n")
        return 1
    }
    return 0
}

//! every selected format's arms; the row count printed
def run_formats(cfg : BenchArgs; fs : array<tuple<string; KqFmt; bool>>) : int {
    var rows = 0
    let nsb = int64(cfg.n) / 256l
    for (fe in fs) {
        let sh = plane_shape(fe._1, fe._2, int64(cfg.n))
        var p : Planes
        fill_planes(p, sh, int64(cfg.n), int64(cfg.d), int64(max(cfg.ntok, 1)))
        if (fe._2 || fe._1 == KqFmt.q8) {
            bench_q8_family(rows, cfg, fe._2, p, nsb, sh)
        } else {
            if (!cfg.no_gemv) {
                bench_gemv_kq(rows, cfg, fe._0, fe._1, p, nsb, sh)
            }
            if (cfg.ntok > 0) {
                bench_tile_kq(rows, cfg, fe._0, fe._1, p, nsb, sh)
            }
        }
        delete p
    }
    return rows
}
