options gen2
options persistent_heap
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options _dasllama_internal = true

require daslib/clargs
require daslib/enum_trait
require daslib/jobque_boost
require dasllama/dasllama_par
require dasllama/dasllama_math_gen
require dasllama/dasllama_math
require dasllama/dasllama_kqformat
require dasllama/dasllama_gemm_schema
require llvm   // nolint:STYLE030 - the witness, unguarded on purpose: the `_variants()` registries this bench runs exist only with dasLLVM, so a host without it skips this program as missing its prerequisite
require llvm/daslib/llvm_env
require math
require strings
require daslib/strings_boost

// ATTRIBUTION SWEEP: every mode - each row times one registry spelling of a kq / q8 kernel at the decode (GEMV)
// or prefill (TILE) shape and attributes kernel time to it; no arm's output is read back or compared.
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
    @clarg_doc = "KqFmt names, comma-separated (q8,k4,k5,k6,q40,q51,iq4xs,k3,iq3s,iq3xxs,iq4nl,k2,iq2s,iq2xs,iq2xxs,iq4nl32,iq3s4; mx4 = mxfp4; q8s16 = q8 over binary16 group scales, the GGUF q8_0 tensor's path), or all"
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
typedef LayoutFn = function<() : int>

let TILE_TOKENS = 4l   // tokens one kq / q51 / mx4 tile call covers (q8's tokstep companion says its own)
let MX4_NAME = "mx4"   // mxfp4's KqFmt row carries no family of its own; it rides the q8q8 grid as a companion
let S16_NAME = "q8s16"   // q8 over binary16 group scales - the wscale_f16 rail a GGUF q8_0 tensor runs (8.5 bits/weight, the reference build's q8_0 bytes)
var g_s16 = false        // the current q8-family row is the s16 flavor

def fmt_of_name(name : string) : KqFmt {
    var f : KqFmt
    if (kq_fmt_of_name(name, f)) {
        return f
    }
    panic("unknown kq format '{name}'")
    return KqFmt.q8
}

//! a non-q8 format's gemv, tile and layout registries: a kq family's through kq_registries, the per-32
//! families' (q51, iq4nl32 - no kq family) by name; empty for q8 and mx4, which bench_q8_family races
def fmt_registries(f : KqFmt) : KqRegistries {
    if (f == KqFmt.q51) {
        return <- KqRegistries(gemv <- q51q8_gemv_gen_variants(), tile <- q51q8_tile_gen_variants(), layout <- q51q8_layout_gen_variants())
    }
    if (f == KqFmt.iq4nl32) {
        return <- KqRegistries(gemv <- iq4nl32q8_gemv_gen_variants(), tile <- iq4nl32q8_tile_gen_variants(), layout <- iq4nl32q8_layout_gen_variants())
    }
    var regs : KqRegistries
    kq_fmt_stamp(f) $(F) {
        regs <- kq_registries(F)
    }
    return <- regs
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

//! plane bytes per row: kq formats per 256-superblock, q51 per 32-block (20 + 4), iq4nl32 per 32-block (16 + 2), q8 n int8 + n/32 f32, mx4 n/2 nibbles + n/32 e8m0
struct PlaneShape {
    gemvW_row : int64   // the gemv's weight plane bytes per row
    tileW_row : int64   // the tile's weight plane bytes per row
    ws_row : int64   // scale plane bytes per row
}

def plane_shape(f : KqFmt; mx4 : bool; n : int64) : PlaneShape {
    if (mx4) {
        return PlaneShape(gemvW_row = n / 2l, tileW_row = n / 2l, ws_row = n / 32l)
    }
    if (f == KqFmt.q8) {
        return PlaneShape(gemvW_row = n, tileW_row = n, ws_row = (n / 32l) * (g_s16 ? 2l : 4l))
    }
    if (f == KqFmt.q51) {
        return PlaneShape(gemvW_row = (n / 32l) * 20l, tileW_row = (n / 32l) * 20l, ws_row = (n / 32l) * 4l)
    }
    if (f == KqFmt.iq4nl32) {
        return PlaneShape(gemvW_row = (n / 32l) * IQ4NL32_QB, tileW_row = (n / 32l) * IQ4NL32_QB, ws_row = (n / 32l) * IQ4NL32_SB)
    }
    let nsb = n / 256l
    return PlaneShape(gemvW_row = nsb * kq_qsb(f), tileW_row = nsb * tile_kq_bytes_per_sb(f), ws_row = nsb * kq_ssb(f))
}

var BASE_ALIGN = 64l   // the arena's start alignment - --base-align; the planes' phase against 4 KB pages follows it
var BASE_OFFSET = 0l   // --base-offset, added past the aligned start
let ALIGN = 64l   // a 64-byte load split across two cache lines is the 2x penalty the fixture must not introduce
let STAGGER = 1088l   // 1024 + 64: keeps the six plane starts distinct in their low 12 address bits
let SCALE_BYTE = uint8(0x3C)   // as f16 0x3C3C = 1.06, as f32 0x3C3C3C3C = 0.0115, as e8m0 2^-67, as a 6-bit sub-scale 60 - normal in every scale form

//! every plane of one format in ONE arena at fixed offsets
struct Planes {
    arena : array<uint8>
    gemvKq : int64    // the gemv's weight plane
    tileKq : int64   // the tile's weight plane (the unpacked panel form where the format has one)
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

[arch(at = "../../ARCHITECTURE_MEASUREMENT_KERNEL_RACE.md#cpu-kernel-bench-fixture")]
def fill_planes(var p : Planes; sh : PlaneShape; n, d, ntok : int64) {
    var end = 0l
    p.gemvKq = next_plane(end, d * sh.gemvW_row, 0)
    p.tileKq = next_plane(end, d * sh.tileW_row, 1)
    p.ks = next_plane(end, d * sh.ws_row, 2)
    p.xq = next_plane(end, ntok * n, 3)
    p.xs = next_plane(end, ntok * n / 32l * 4l, 4)
    p.xbs = next_plane(end, ntok * n / 16l * 4l, 5)
    p.y = next_plane(end, ntok * d * 4l, 6)
    p.arena |> reserve(end + BASE_ALIGN + BASE_OFFSET)
    p.arena |> resize(end + BASE_ALIGN + BASE_OFFSET)
    let base = arena_base(p)
    p.gemvKq += base
    p.tileKq += base
    p.ks += base
    p.xq += base
    p.xs += base
    p.xbs += base
    p.y += base
    var seed = 0x1234u
    for (i in range64(p.gemvKq, p.gemvKq + d * sh.gemvW_row)) {
        seed = seed * 1664525u + 1013904223u
        p.arena[i] = uint8(seed >> 24u)
    }
    for (i in range64(p.tileKq, p.tileKq + d * sh.tileW_row)) {
        seed = seed * 1664525u + 1013904223u
        p.arena[i] = uint8(seed >> 24u)
    }
    for (i in range64(p.ks, p.ks + d * sh.ws_row)) {
        p.arena[i] = SCALE_BYTE
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
let SOLO_BURN = 3   // the solo arm's unmeasured warm rounds (a core ramps for ~3)
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

//! team_gemv for a q8-family rows function (no activation sums), over the flavor's weight and scale plane types
def team_gemv(fn : auto(FT); var yp : float?; wp : auto(WP); sp : auto(SP); xqp : int8 const?; xsp : float const?; n, d : int64; var laneP : int64?) {
    maybe_parallel_for_indexed(0, int(d), matmul_chunks_gemv(int(d), 1, n * d)) $(slot : int; rb : int; re : int) {
        let l0 = ref_time_ticks()
        invoke(fn, yp, wp, sp, xqp, xsp, n, int64(rb), int64(re))
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
    for (st in stats) {
        st.lane_lo = 999999999l
    }
    time_rows(stats, rounds, TEAM_BURN, $(row : int) {
        invoke(blk, row, laneP)
    }, $(_row : int) {
        for (u in laneUs) {
            u = 0l
        }
    }, $(row : int) {
        for (u in laneUs) {
            if (u > 0l) {
                stats[row].lane_lo = min(stats[row].lane_lo, u)
                stats[row].lane_hi = max(stats[row].lane_hi, u)
            }
        }
    })
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
    let mode = g_env_tune.tune_mode
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

//! interleaved timing: every row warmed `burn` unmeasured calls, then round r calls every selected row once
//! before round r + 1 starts; `before` and `after` run outside each timed call
[arch(at = "../../ARCHITECTURE_MEASUREMENT_KERNEL_RACE.md#cpu-kernel-bench-fixture")]
def time_rows(var stats : array<RowStat>; rounds, burn : int; blk, before, after : block<(row : int) : void>) {
    for (row in range(length(stats))) {
        stats[row].us |> reserve(rounds)
        for (_w in range(burn)) {
            invoke(blk, row)
        }
    }
    to_log(LOG_INFO, "kq_kernel_bench: timed rounds begin\n")   // external profilers key on this line to skip the JIT startup
    for (_r in range(rounds)) {
        for (row in range(length(stats))) {
            invoke(before, row)
            let t0 = ref_time_ticks()
            invoke(blk, row)
            stats[row].us |> push(int64(get_time_usec(t0)))
            invoke(after, row)
        }
    }
}

//! the solo arm's timing: SOLO_BURN warm rounds, nothing around the timed call
def time_rows(var stats : array<RowStat>; rounds : int; blk : block<(row : int) : void>) {
    time_rows(stats, rounds, SOLO_BURN, blk, $(_row : int) {}, $(_row : int) {})
}

def bench_gemv_kq(var rows : int&; cfg : BenchArgs; name : string; f : KqFmt; var p : Planes; nsb : int64; sh : PlaneShape) {
    let n = int64(cfg.n)
    let d = int64(cfg.d)
    let plane_bytes = int64(cfg.d) * (sh.gemvW_row + sh.ws_row)
    var regs <- fmt_registries(f)
    var stats : array<RowStat>
    var fns : array<GemvFn>
    select_rows(cfg, regs.gemv, stats, fns)
    unsafe {
        let kqp = addr<uint8 const?>(p.arena[p.gemvKq])
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
        if (cfg.tsv) {   // the row's answer, one solo call each: modes and seats must agree here before their times are compared
            for (fn, st in fns, stats) {
                invoke(fn, yp, kqp, ksp, xqp, xsp, xbsp, n, 0l, d)
                var acc = 0.0lf
                for (i in range64(d)) {
                    acc += double(yp[i])
                }
                print("# ysum\t{name}\t{st.perm}\t{acc}\t{yp[0]}\t{yp[d - 1l]}\n")
            }
        }
    }
    for (st in stats) {
        emit_row(rows, name, "gemv", st, n, d, 1l, nsb, plane_bytes)
    }
    delete stats
    delete fns
    delete regs
}

def bench_tile_kq(var rows : int&; cfg : BenchArgs; name : string; f : KqFmt; var p : Planes; nsb : int64; sh : PlaneShape) {
    let plane_bytes = int64(cfg.d) * (sh.tileW_row + sh.ws_row)
    var regs <- fmt_registries(f)
    var mrs <- companion_ints(regs.layout)
    let perCall : table<string; int64>   // a kq / per-32 tile covers TILE_TOKENS a call
    var stats : array<RowStat>
    var fns : array<TileFn>
    var mrOf : array<int64>
    var stepOf : array<int64>
    select_tile_rows(cfg, name, int64(cfg.d), regs.tile, mrs, perCall, stats, fns, mrOf, stepOf)
    unsafe {
        time_tile_rows(stats, cfg, fns, mrOf, stepOf, addr<uint8 const?>(p.arena[p.tileKq]), addr<uint8 const?>(p.arena[p.ks]), sh, p,
            type<uint8 const?>, type<uint8 const?>)
    }
    for (st in stats) {
        emit_row(rows, name, "tile", st, int64(cfg.n), int64(cfg.d), int64(cfg.ntok), nsb, plane_bytes)
    }
    delete stats
    delete fns
    delete mrOf
    delete stepOf
    delete regs
    delete mrs
}

//! the registry rows --perm selects, into stats and the typed function list
def select_rows(cfg : BenchArgs; vs : array<tuple<string; auto(VT)>>; var stats : array<RowStat>; var fns : array<auto(FT)>) {
    for (v in vs) {
        if (perm_wanted(cfg.perm, v._0)) {
            stats |> emplace(RowStat(perm = v._0))
            fns |> push(v._1)
        }
    }
}

//! the tile rows --perm selects, each with its layout mr and tokens a call (the `steps` companion's row, else
//! TILE_TOKENS); a row whose mr the row count does not divide is logged and skipped
def select_tile_rows(cfg : BenchArgs; name : string; d : int64; vs : array<tuple<string; auto(VT)>>; mrs, steps : table<string; int64>;
                     var stats : array<RowStat>; var fns : array<auto(FT)>; var mrOf, stepOf : array<int64>) {
    for (v in vs) {
        continue if (!perm_wanted(cfg.perm, v._0))
        let mr = mrs?[v._0] ?? (mrs?["reference"] ?? 0l)
        if (mr <= 0l || d % mr != 0l) {
            to_log(LOG_WARNING, "kq_kernel_bench: {name} tile '{v._0}' has no layout row (mr={mr}) - skipped\n")
            continue
        }
        stats |> emplace(RowStat(perm = v._0))
        fns |> push(v._1)
        mrOf |> push(mr)
        stepOf |> push(steps?[v._0] ?? (steps?["reference"] ?? TILE_TOKENS))
    }
}

//! the TILE arm's timing: every row walks its mr-row groups over the batch, stepOf[row] tokens a call, off the weight
//! plane `wp` and the scale plane `sp` - each group's base cast to the row function's plane types `wt` / `st`
def time_tile_rows(var stats : array<RowStat>; cfg : BenchArgs; fns : array<auto(FT)>; mrOf, stepOf : array<int64>; wp, sp : uint8 const?;
                   sh : PlaneShape; var p : Planes; wt : auto(WT); st : auto(ST)) {
    let n = int64(cfg.n)
    let d = int64(cfg.d)
    let ntok = int64(cfg.ntok)
    unsafe {
        let xqp = addr<int8 const?>(p.arena[p.xq])
        let xsp = addr<float const?>(p.arena[p.xs])
        let xbsp = addr<int const?>(p.arena[p.xbs])
        var yp = addr<float?>(p.arena[p.y])
        time_rows(stats, cfg.rounds) $(row : int) {
            let mr = mrOf[row]
            for (g in range64(d / mr)) {
                var t0k = 0l
                while (t0k < ntok) {
                    invoke(fns[row], yp, reinterpret<WT>(wp + g * mr * sh.tileW_row), reinterpret<ST>(sp + g * mr * sh.ws_row), xqp, xsp, xbsp, n, d, g, t0k)
                    t0k += stepOf[row]
                }
            }
        }
    }
}

//! one q8-family flavor's two arms over the shared planes: the gemv rows, then the tile rows - the tile reads
//! the gemv plane (one weight form); `wt` / `st` are the flavor's weight and scale plane pointer types
def bench_q8_flavor(var rows : int&; cfg : BenchArgs; name : string; var p : Planes; nsb : int64; sh : PlaneShape;
                    gvs : array<tuple<string; auto(GF)>>; tvs : array<tuple<string; auto(TF)>>; mrs, steps : table<string; int64>;
                    wt : auto(WT); st : auto(ST)) {
    let n = int64(cfg.n)
    let d = int64(cfg.d)
    let ntok = int64(cfg.ntok)
    let plane_bytes = int64(cfg.d) * (sh.gemvW_row + sh.ws_row)
    unsafe {
        let kqp = addr<uint8 const?>(p.arena[p.gemvKq])
        let ksp = addr<uint8 const?>(p.arena[p.ks])
        let xqp = addr<int8 const?>(p.arena[p.xq])
        let xsp = addr<float const?>(p.arena[p.xs])
        var yp = addr<float?>(p.arena[p.y])
        if (!cfg.no_gemv) {
            var stats : array<RowStat>
            var fns : array<GF>
            select_rows(cfg, gvs, stats, fns)
            let wp = reinterpret<WT>(kqp)
            let sp = reinterpret<ST>(ksp)
            if (cfg.team) {
                time_rows_team(stats, cfg.rounds) $(row : int; var laneP : int64?) {
                    team_gemv(fns[row], yp, wp, sp, xqp, xsp, n, d, laneP)
                }
            } else {
                time_rows(stats, cfg.rounds) $(row : int) {
                    invoke(fns[row], yp, wp, sp, xqp, xsp, n, 0l, d)
                }
            }
            for (s in stats) {
                emit_row(rows, name, "gemv", s, n, d, 1l, nsb, plane_bytes)
            }
            delete stats
            delete fns
        }
        if (ntok > 0l) {
            var stats : array<RowStat>
            var fns : array<TF>
            var mrOf : array<int64>
            var stepOf : array<int64>
            select_tile_rows(cfg, name, d, tvs, mrs, steps, stats, fns, mrOf, stepOf)
            time_tile_rows(stats, cfg, fns, mrOf, stepOf, kqp, ksp, sh, p, type<WT>, type<ST>)
            for (s in stats) {
                emit_row(rows, name, "tile", s, n, d, ntok, nsb, plane_bytes)
            }
            delete stats
            delete fns
            delete mrOf
            delete stepOf
        }
    }
}

//! q8 (the q8q8 grid) and its mx4 companion: int8 / nibble weight planes with f32 / f16 / e8m0 scales
def bench_q8_family(var rows : int&; cfg : BenchArgs; mx4 : bool; var p : Planes; nsb : int64; sh : PlaneShape) {
    var wvs <- q8q8_family_live_variants()   // the family's witness rows: the amx one takes the per-process tile permission its tiles need (SIGILL without it), as the probe does
    for (w in wvs) {
        invoke(w._1)
    }
    delete wvs
    var mrs <- companion_ints(q8q8_layout_gen_variants())
    var steps <- companion_ints(q8q8_tokstep_gen_variants())
    if (mx4) {
        var gvs <- mx4q8_gemv_gen_variants()
        var tvs <- mx4q8_tile_gen_variants()
        let perCall : table<string; int64>   // the mx4 tile covers TILE_TOKENS a call, whatever the q8 row's tokstep
        bench_q8_flavor(rows, cfg, MX4_NAME, p, nsb, sh, gvs, tvs, mrs, perCall, type<uint8 const?>, type<uint8 const?>)
        delete gvs
        delete tvs
    } elif (g_s16) {
        var gvs <- q8q8_gemv_s16_gen_variants()
        var tvs <- q8q8_tile_s16_gen_variants()
        bench_q8_flavor(rows, cfg, S16_NAME, p, nsb, sh, gvs, tvs, mrs, steps, type<int8 const?>, type<uint16 const?>)
        delete gvs
        delete tvs
    } else {
        var gvs <- q8q8_gemv_gen_variants()
        var tvs <- q8q8_tile_gen_variants()
        bench_q8_flavor(rows, cfg, "q8", p, nsb, sh, gvs, tvs, mrs, steps, type<int8 const?>, type<float const?>)
        delete gvs
        delete tvs
    }
    delete mrs
    delete steps
}

//! one --fmt name as (name, KqFmt, is_mx4): mx4 and q8s16 are q8-family flavors bench_q8_family races
def format_entry(name : string) : tuple<string; KqFmt; bool> {
    if (name == MX4_NAME) {
        return (MX4_NAME, KqFmt.q8, true)
    }
    if (name == S16_NAME) {
        return (S16_NAME, KqFmt.q8, false)
    }
    return (name, fmt_of_name(name), false)
}

//! the requested formats; "all" = every KqFmt row the bench races (the q8 family's, and the rest where
//! fmt_registries has rows), then q8s16
def formats_of(spec : string) : array<tuple<string; KqFmt; bool>> {
    var fs : array<tuple<string; KqFmt; bool>>
    if (spec == "all") {
        for (f in type<KqFmt>) {
            var regs <- fmt_registries(f)
            if (f == KqFmt.q8 || f == KqFmt.mx4 || !empty(regs.gemv) || !empty(regs.tile)) {
                fs |> push(format_entry("{f}"))
            }
            delete regs
        }
        fs |> push(format_entry(S16_NAME))
        return <- fs
    }
    for (tok in split(spec, ",")) {
        let name = strip(tok)
        if (empty(name)) {
            continue
        }
        fs |> push(format_entry(name))
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
        print("kq_kernel_bench: n={cfg.n} d={cfg.d} ntok={cfg.ntok} rounds={cfg.rounds} team={cfg.team} tune_mode={g_env_tune.tune_mode}\n")
        print("fmt\ttier\tperm\tn\td\tntok\tbest_us\tmedian_us\tns_per_sb\tGB/s\tGFLOP/s{cfg.team ? "\tlane_lo\tlane_hi" : ""}\n")
    }
    var rows = 0
    if (cfg.team) {
        with_dasllama_jobque() {
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
        g_s16 = fe._0 == S16_NAME
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
