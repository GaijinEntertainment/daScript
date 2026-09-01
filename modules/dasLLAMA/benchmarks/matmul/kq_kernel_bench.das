options gen2
options persistent_heap
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options _dasllama_internal = true

require daslib/clargs
require daslib/enum_trait
require dasllama/dasllama_math_gen
require dasllama/dasllama_math
require dasllama/dasllama_kqformat
require daslib/fio
require math
require strings
require daslib/strings_boost

//! The kq kernel microbench - the inner loop of kernel work. Raw kernels, one thread, no jobque,
//! no model; synthetic planes, because the timing is data-independent. GEMV is the decode shape
//! (one token), TILE the prefill batch. Every row of a kernel's `_variants()` registry is timed:
//! under DAS_TUNE_MODE=tune that is the whole [tune_perm] grid, under normal mode the stamped
//! winner alone. Timing only - a spelling's correctness is gen_tune_probe's TEST mode.
//! Run: DAS_TUNE_MODE=tune bin/daslang -jit modules/dasLLAMA/benchmarks/matmul/kq_kernel_bench.das -- --fmt iq3s
//! The reference row at the same shape: GGML_BENCH_THREADS=1 test-backend-ops perf -o MUL_MAT -p "type_a=iq3_s"
//! (its default MUL_MAT perf case is m=4096 rows, k=14336, n in {1, 512} tokens - this bench's defaults).

[CommandLineArgs]
struct BenchArgs {
    @clarg_short = "f"
    @clarg_doc = "KqFmt names, comma-separated (k4,k5,k6,q40,iq4xs,k3,iq3s,iq3xxs,iq4nl,k2,iq2s,iq2xs,iq2xxs), or all"
    fmt : string = "all"
    @clarg_doc = "Reduction length - the weight row length (k in test-backend-ops terms)"
    n : int = 14336
    @clarg_doc = "Rows - output features (m in test-backend-ops terms)"
    d : int = 4096
    @clarg_doc = "Tokens for the TILE arm (the prefill batch); 0 skips the arm"
    ntok : int = 512
    @clarg_doc = "Timed repetitions per row; the best one is reported"
    rounds : int = 5
    @clarg_doc = "Skip the GEMV (decode) arm"
    no_gemv : bool
    @clarg_short = "p"
    @clarg_doc = "Registry rows to time - comma-separated substrings of the perm suffix, =name for an exact row (default: every row, the reference body included)"
    perm : string
    @clarg_doc = "TSV rows only, no banner"
    tsv : bool
}

typedef GemvFn = function<(var yp : float?; kqp : uint8 const?; ksp : uint8 const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n, rb, re : int64) : void>
typedef TileFn = function<(var yp : float?; kqg : uint8 const?; ksg : uint8 const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n, d, g, t0 : int64) : void>
typedef LayoutFn = function<() : int>

let TILE_TOKENS = 4l   // tokens one kq tile call covers

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
    var none : array<tuple<string; LayoutFn>>
    return <- none
}

//! mr per registry suffix; the layout companion is stamped in lockstep with the tile, so a tile
//! row and the layout row of the same suffix belong together
def layout_mrs(f : KqFmt) : table<string; int64> {
    var mrs : table<string; int64>
    var lvs <- layout_variants(f)
    for (v in lvs) {
        mrs[v._0] = int64(invoke(v._1))
    }
    delete lvs
    return <- mrs
}

//! the five grid formats' TILE reads a caller-unpacked panel (256 bytes per row per superblock), not the packed plane the gemv gathers from
def tile_kq_bytes_per_sb(f : KqFmt) : int64 => (f == KqFmt.iq3s || f == KqFmt.iq3xxs || f == KqFmt.iq2s || f == KqFmt.iq2xs || f == KqFmt.iq2xxs) ? 256l : kq_qsb(f)

struct Planes {
    kq : array<uint8>
    tkq : array<uint8>   // the tile arm's weight plane (the unpacked panel form where the format has one)
    ks : array<uint8>
    xq : array<int8>
    xs : array<float>
    xbs : array<int>
    y : array<float>
}

def sized(var a : array<auto(numT)>; n : int64) {
    a |> reserve(n)
    a |> resize(n)
}

def fill_planes(var p : Planes; f : KqFmt; n, d, ntok : int64) {
    let nsb = n / 256l
    p.kq |> sized(d * nsb * kq_qsb(f))
    p.tkq |> sized(d * nsb * tile_kq_bytes_per_sb(f))
    p.ks |> sized(d * nsb * kq_ssb(f))
    p.xq |> sized(ntok * n)
    p.xs |> sized(ntok * n / 32l)
    p.xbs |> sized(ntok * n / 16l)
    p.y |> sized(ntok * d)
    var seed = 0x1234u
    for (b in p.kq) {
        seed = seed * 1664525u + 1013904223u
        b = uint8(seed >> 24u)
    }
    for (b in p.tkq) {
        seed = seed * 1664525u + 1013904223u
        b = uint8(seed >> 24u)
    }
    for (b in p.ks) {
        seed = seed * 1664525u + 1013904223u
        b = uint8(seed >> 24u)
    }
    for (q in p.xq) {
        seed = seed * 1664525u + 1013904223u
        q = int8(seed >> 24u)
    }
    for (s in p.xs) {
        s = 0.01
    }
    for (bsum in p.xbs) {
        bsum = 128
    }
}

def emit_row(var rows : int&; f : KqFmt; tier, perm : string; n, d, ntok, best_us : int64; nsb : int64; plane_bytes : int64) {
    let ns_per_sb = float(best_us) * 1000.0 / float(d * nsb * ntok)
    let gbs = float(plane_bytes) / (float(best_us) * 1000.0)
    let gflops = 2.0 * float(n) * float(d) * float(ntok) / (float(best_us) * 1000.0)
    print("{f}\t{tier}\t{perm}\t{n}\t{d}\t{ntok}\t{best_us}\t{ns_per_sb}\t{gbs}\t{gflops}\n")
    rows++
}

def bench_gemv(var rows : int&; cfg : BenchArgs; f : KqFmt; var p : Planes) {
    let n = int64(cfg.n)
    let d = int64(cfg.d)
    let nsb = n / 256l
    let plane_bytes = long_length(p.kq) + long_length(p.ks)
    var vs <- gemv_variants(f)
    unsafe {
        let kqp = addr<uint8 const?>(p.kq[0])
        let ksp = addr<uint8 const?>(p.ks[0])
        let xqp = addr<int8 const?>(p.xq[0])
        let xsp = addr<float const?>(p.xs[0])
        let xbsp = addr<int const?>(p.xbs[0])
        var yp = addr(p.y[0])
        for (v in vs) {
            if (!perm_wanted(cfg.perm, v._0)) {
                continue
            }
            invoke(v._1, yp, kqp, ksp, xqp, xsp, xbsp, n, 0l, d)   // warmup + codegen
            var best_us = 999999999l
            for (_r in range(cfg.rounds)) {
                let t0 = ref_time_ticks()
                invoke(v._1, yp, kqp, ksp, xqp, xsp, xbsp, n, 0l, d)
                best_us = min(best_us, int64(get_time_usec(t0)))
            }
            emit_row(rows, f, "gemv", v._0, n, d, 1l, best_us, nsb, plane_bytes)
        }
    }
    delete vs
}

def bench_tile(var rows : int&; cfg : BenchArgs; f : KqFmt; var p : Planes) {
    let n = int64(cfg.n)
    let d = int64(cfg.d)
    let ntok = int64(cfg.ntok)
    let nsb = n / 256l
    let qsb = tile_kq_bytes_per_sb(f)
    let ssb = kq_ssb(f)
    let plane_bytes = long_length(p.tkq) + long_length(p.ks)
    var mrs <- layout_mrs(f)
    var vs <- tile_variants(f)
    unsafe {
        let kqp = addr<uint8 const?>(p.tkq[0])
        let ksp = addr<uint8 const?>(p.ks[0])
        let xqp = addr<int8 const?>(p.xq[0])
        let xsp = addr<float const?>(p.xs[0])
        let xbsp = addr<int const?>(p.xbs[0])
        var yp = addr(p.y[0])
        for (v in vs) {
            if (!perm_wanted(cfg.perm, v._0)) {
                continue
            }
            let mr = mrs?[v._0] ?? (mrs?["reference"] ?? 0l)
            if (mr <= 0l || d % mr != 0l) {
                to_log(LOG_WARNING, "kq_kernel_bench: {f} tile '{v._0}' has no layout row (mr={mr}) - skipped\n")
                continue
            }
            let ngroups = d / mr
            var best_us = 999999999l
            for (r in range(cfg.rounds + 1)) {   // the first pass is warmup + codegen
                let t0 = ref_time_ticks()
                for (g in range64(ngroups)) {
                    var t0k = 0l
                    while (t0k < ntok) {
                        invoke(v._1, yp, kqp + g * mr * nsb * qsb, ksp + g * mr * nsb * ssb, xqp, xsp, xbsp, n, d, g, t0k)
                        t0k += TILE_TOKENS
                    }
                }
                if (r > 0) {
                    best_us = min(best_us, int64(get_time_usec(t0)))
                }
            }
            emit_row(rows, f, "tile", v._0, n, d, ntok, best_us, nsb, plane_bytes)
        }
    }
    delete vs
    delete mrs
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

def formats_of(spec : string) : array<KqFmt> {
    var fs : array<KqFmt>
    if (spec == "all") {
        for (f in type<KqFmt>) {
            if (kq_sb(f)) {
                fs |> push(f)
            }
        }
        return <- fs
    }
    for (tok in split(spec, ",")) {
        let name = strip(tok)
        if (!empty(name)) {
            fs |> push(fmt_of_name(name))
        }
    }
    return <- fs
}

[export]
def main : int {
    var cfg = BenchArgs()
    let rc = parse_args_with_help(cfg, "kq_kernel_bench")
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
        print("kq_kernel_bench: n={cfg.n} d={cfg.d} ntok={cfg.ntok} rounds={cfg.rounds} tune_mode={get_env_variable("DAS_TUNE_MODE")}\n")
        print("fmt\ttier\tperm\tn\td\tntok\tbest_us\tns_per_sb\tGB/s\tGFLOP/s\n")
    }
    var rows = 0
    for (f in fs) {
        var p : Planes
        fill_planes(p, f, int64(cfg.n), int64(cfg.d), int64(max(cfg.ntok, 1)))
        if (!cfg.no_gemv) {
            bench_gemv(rows, cfg, f, p)
        }
        if (cfg.ntok > 0) {
            bench_tile(rows, cfg, f, p)
        }
        delete p
    }
    delete fs
    if (rows == 0) {
        to_log(LOG_ERROR, "kq_kernel_bench: no rows - nothing matched --fmt '{cfg.fmt}' or every arm was skipped\n")
        return 1
    }
    return 0
}
