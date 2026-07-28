options gen2

// Per-box kernel tuner — sweeps the FULL grid (20 perms) for every [tuned] kernel (the float
// elementwise set, softmax, rmsnorm, the quantized dots, the activation requant, the NEOX rope
// table leaf, the fp32 GEMM tile, and the arm64 laneq4x4 tile) and writes the winning permutation
// of each to box_profile.json. [dasllama_grid] emits, cross-module, each template's
// <name>__<suffix> variant clones plus a <name>_template_variants() fn-ptr registry; the bench runs
// the registry INTERLEAVED under the JIT (loop hints only bite there — see dasllama_tune),
// correctness-gates every variant vs an f64 reference (exact equality for the requant), and reports
// each variant's time as a DELTA vs that kernel's SHIPPED fallback perm so a "win" that is really
// noise is obvious.
//
// Run:  bin/daslang -jit modules/dasLLAMA/harness/tune_kernels.das
// Quick pass: append `-- --tune-fast` for progressive 4/8 screening / 20 maximum rounds.
//   then rebuild/rerun the model with -jit to pick up box_profile.json (it re-keys the JIT DLL cache).

require dasllama/dasllama_math   // nolint:STYLE030 — side-effect: loads the *_template kernels for the cross-module scan
require dasllama/dasllama_math_default   // nolint:STYLE030 — dot_q8q8_template lives here
require dasllama/dasllama_math_aarch64_neon   // laneq4x4 template (the repack backend "arm64-gen" registers via dasllama_common's math_gen require)
require dasllama/dasllama_quant  // quantize templates + the Q4/Q8 reference quantizers
require dasllama/dasllama_common // the runtime-knob getters (the profile's "runtime" section snapshots them)
require dasllama/dasllama_tune   // [dasllama_grid] emitter
require llvm/daslib/llvm_tune    // tune_sidecar_merge — the app-sidecar upsert writer
require tuner_cli               // --tune-fast, shared with the other half
require llvm/daslib/aarch64_neon // nolint:STYLE030 — sdot4_laneq (referenced by the cloned laneq template bodies)
require llvm/daslib/f16_cvt      // f16 references + the cloned f16-kernel template bodies
require math                     // mad (referenced by the cloned template bodies)
require daslib/json
require daslib/json_boost
require daslib/fio

// The shipped-fallback registry: dasllama_tuned_fallbacks() maps every [tuned] kernel to the
// perm it falls back to ON THIS BOX, so report() never restates one as a literal.
[dasllama_fallbacks]
struct private FallbackRegistry {}

// One [dasllama_grid] per kernel: emits <src>__<suffix> for all 20 perms + <src>_variants().
[dasllama_grid(src = "dot_template")]
struct GridDot {}
[dasllama_grid(src = "axpy_template")]
struct GridAxpy {}
[dasllama_grid(src = "add_inplace_template")]
struct GridAdd {}
[dasllama_grid(src = "mul_inplace_template")]
struct GridMul {}
[dasllama_grid(src = "scale_inplace_template")]
struct GridScale {}
[dasllama_grid(src = "copy_floats_template")]
struct GridCopy {}
[dasllama_grid(src = "softmax_template")]
struct GridSoftmax {}
[dasllama_grid(src = "rmsnorm_template")]
struct GridRms {}
[dasllama_grid(src = "dot_q4_template")]
struct GridDotQ4 {}
[dasllama_grid(src = "dot_q8q8_template")]
struct GridDotQ8Q8 {}
[dasllama_grid(src = "dot_mx4q8_template")]
struct GridDotMx4Q8 {}
[dasllama_grid(src = "quantize_q8_0_into_ptr_template")]
struct GridQuant {}
[dasllama_grid(src = "quantize_q8_0_bs_into_ptr_template")]
struct GridQuantBs {}
[dasllama_grid(src = "rope_scaled_neox_tab_template")]
struct GridRopeTab {}
[dasllama_grid(src = "gemm_f32_uk_4x16_template")]
struct GridGemm {}
[dasllama_grid(src = "dot_q8q8_laneq4x4_template")]
struct GridLaneq {}
[dasllama_grid(src = "dot_f16_template")]
struct GridDotF16 {}
[dasllama_grid(src = "axpy_f16_template")]
struct GridAxpyF16 {}
[dasllama_grid(src = "cvt_f32_to_f16_template")]
struct GridCvtToF16 {}
[dasllama_grid(src = "cvt_f16_to_f32_template")]
struct GridCvtToF32 {}
[dasllama_grid(src = "dot_q8kv_template")]
struct GridDotQ8Kv {}
[dasllama_grid(src = "dot_q8q8kv_template")]
struct GridDotQ8Q8Kv {}
[dasllama_grid(src = "axpy_q8kv_template")]
struct GridAxpyQ8Kv {}
[dasllama_grid(src = "cvt_q8kv_to_f32_template")]
struct GridCvtQ8Kv {}
[dasllama_grid(src = "quantize_q8kv_row_template")]
struct GridQuantQ8Kv {}

// Registry fn-ptr signatures (== function_to_type of each template).
typedef DotFn = function<(a : float const?; b : float const?; n : int64) : float>
typedef BinFn = function<(var d : float?; s : float const?; n : int64) : void>
typedef AxpyFn = function<(var d : float?; s : float const?; scale : float; n : int64) : void>
typedef ScaleFn = function<(var d : float?; scale : float; n : int64) : void>

let N = 4096l
var private FAST_MODE = false
var private SCREEN_ROUNDS = 20
var private ROUNDS = 80
let MIN_FINALISTS = 4
let FINALIST_MARGIN = 1.05lf
let FAST_MID_ROUNDS = 8
let FAST_MID_FINALISTS = 8
let FAST_MID_MARGIN = 1.10lf
let REPS = 2000
// The outer progress denominator, needed before the first kernel runs so it cannot be derived
// from the results. A mismatch only skews the bar, and main says so at the end.
let TUNED_KERNEL_COUNT = 25

struct private KernelResult {
    kernel : string
    winner : string
    fallback : string
}

// One row per benched kernel, in call order. report() is the single place that holds a kernel's
// name, its winner, and the shipped fallback it was judged against — the summary reads them back
// from here rather than restating all three in parallel arrays that can drift apart.
var private g_results : array<KernelResult>
var private g_shipped : table<string; string>
let TOL = 1.0e-3lf
let BASELINE = "vec8_u2"   // the majority hand default; kernels with their own [tuned(fallback=)] pass it to report()

// ===== shared reporting =====

// Keep the fastest rows plus anything within the supplied leader margin. Normal mode measures
// the full grid for 20 rounds before the 80-round finalist phase. Fast mode narrows progressively:
// full grid through round 4, top 8 / within 10% through round 8, then top 4 / within 5% to round 20.
def screening_keep(best : array<double>; ok : array<bool>; i : int;
                   min_finalists : int; margin : double) : bool {
    if (!ok[i]) {
        return false
    }
    var leader = 1.0e30lf
    var faster = 0
    for (j in range(length(best))) {
        if (ok[j]) {
            if (best[j] < leader) {
                leader = best[j]
            }
            if (best[j] < best[i]) {
                faster ++
            }
        }
    }
    return faster < min_finalists || best[i] <= leader * margin
}

def private kernel_begin(name : string) : int64 {
    tune_progress_kernel_begin(name, ROUNDS)
    return ref_time_ticks()
}

def private kernel_done(name : string; t0 : int64) {
    // A bench that returned early — an unsupported ISA, no -jit — never reached report(), so it
    // emitted no end event and recorded no row. Close it here, or the bar stops one short of its
    // plan forever and every consumer's counters stall with it.
    if (empty(g_results) || g_results[length(g_results) - 1].kernel != name) {
        tune_progress_kernel_end(name, "", "skipped")
        g_results |> emplace(KernelResult(kernel = name, winner = "", fallback = shipped_fallback(name)))
    }
    tune_detail("TUNE_KERNEL_TIME {name} {get_time_usec(t0) / 1000} ms\n")
}

// One progress step per measured round: which pass we are in, and how many grid rows are still
// in contention. Both are already computed by the screening rule — this only reports them.
def private emit_round(round : int; best : array<double>; ok : array<bool>) {
    if (round < 0) return
    var live = 0
    for (i in range(length(best))) {
        if (ok[i] && tune_candidate_active(round, best, ok, i)) {
            live++
        }
    }
    let phase = (round < SCREEN_ROUNDS
        ? "screening"
        : ((FAST_MODE && round < FAST_MID_ROUNDS) ? "narrowing" : "finalists"))
    tune_progress_kernel_step(round + 1, phase, live)
}

def private tune_candidate_active(round : int; best : array<double>; ok : array<bool>; i : int) : bool {
    if (round < SCREEN_ROUNDS) {
        return true
    }
    if (FAST_MODE && round < FAST_MID_ROUNDS) {
        return screening_keep(best, ok, i, FAST_MID_FINALISTS, FAST_MID_MARGIN)
    }
    return screening_keep(best, ok, i, MIN_FINALISTS, FINALIST_MARGIN)
}

// Print each passing variant's best time as a % delta vs the kernel's SHIPPED baseline (its
// [tuned] fallback perm — vec8_u2 for most, but e.g. dot_q8q8 ships vec16), mark the winner, and
// return the winning suffix (or "" if nothing passed). A win of only a couple % is flagged but is
// likely noise — vec16 was a real 2% loss, so the bar for "genuinely new" is a repeatable margin.
def private shipped_fallback(kernel : string) : string {
    if (empty(g_shipped)) {
        g_shipped <- dasllama_tuned_fallbacks()
    }
    return g_shipped?[kernel] ?? BASELINE
}

def report(kernel : string; names : array<string>; best : array<double>; ok : array<bool>) : string {
    let baseline = shipped_fallback(kernel)
    var bi = -1
    for (i, nm in count(), names) {
        if (nm == baseline) {
            bi = i
        }
    }
    let base = (bi >= 0 && ok[bi]) ? best[bi] : -1.0lf
    var winner = -1
    for (i in range(length(names))) {
        if (ok[i] && (winner < 0 || best[i] < best[winner])) {
            winner = i
        }
    }
    tune_detail("--- {kernel} ---\n")
    for (i in range(length(names))) {
        if (!ok[i]) {
            tune_detail("  {names[i]}: REJECTED\n")
            continue
        }
        let delta = base > 0.0lf ? (best[i] - base) / base * 100.0lf : 0.0lf
        let mark = i == winner ? "  <== winner" : ""
        tune_detail("  {names[i]}: {best[i]} us  ({delta}% vs {baseline}){mark}\n")
    }
    if (winner < 0) {
        tune_progress_note("  {kernel}: no passing variant\n")
        tune_progress_kernel_end(kernel, "", "rejected")
        g_results |> emplace(KernelResult(kernel = kernel, winner = "", fallback = baseline))
        return ""
    }
    var wname = names[winner]
    // a tie or sub-noise "win" HOLDS the shipped baseline: flipping the perm re-keys the JIT and
    // reorders the kernel's float math, which is only worth paying for a real, repeatable margin
    let margin = base > 0.0lf ? (base - best[winner]) / base * 100.0lf : 0.0lf
    if (wname != baseline && base > 0.0lf && margin < 0.5lf) {
        wname = baseline
        tune_detail("  => {kernel}: {baseline} holds ({names[winner]} within the noise floor, {margin}%)\n")
        tune_progress_kernel_end(kernel, wname, "holds")
    } elif (wname == baseline) {
        tune_detail("  => {kernel}: {baseline} holds (the shipped default is already best)\n")
        tune_progress_kernel_end(kernel, wname, "holds")
    } else {
        tune_detail("  => {kernel}: {wname} beats {baseline} by {margin}%  <<< the tuner found something\n")
        tune_progress_kernel_end(kernel, wname, "beats")
    }
    g_results |> emplace(KernelResult(kernel = kernel, winner = wname, fallback = baseline))
    return wname
}

def reset(var d : array<float>; d0 : array<float>) {
    for (i in range(length(d))) {
        d[i] = d0[i]
    }
}

// ===== dot: (a,b)->float, repeatable (no mutation) =====

def ref_dot(a : float const?; b : float const?; n : int64) : double {
    var s = 0.0lf
    for (j in range64(n)) {
        s += double(unsafe(a[j])) * double(unsafe(b[j]))
    }
    return s
}

def bench_dot() : string {
    var a : array<float>
    var b : array<float>
    a |> resize(int(N))
    b |> resize(int(N))
    for (i in range(int(N))) {
        a[i] = sin(float(i) * 0.1f)
        b[i] = cos(float(i) * 0.07f)
    }
    let pa = unsafe(addr(a[0]))
    let pb = unsafe(addr(b[0]))
    let want = ref_dot(pa, pb, N)

    let vs <- dot_template_variants()
    let nv = length(vs)
    var names : array<string>
    var ok : array<bool>
    var best : array<double>
    names |> resize(nv)
    ok |> resize(nv)
    best |> resize(nv)
    for (i in range(nv)) {
        names[i] = vs[i]._0
        let f = vs[i]._1
        let got = double(f(pa, pb, N))
        ok[i] = abs(got - want) / (abs(want) + 1.0lf) <= TOL
        best[i] = 1.0e30lf
    }

    var sink = 0.0f
    for (tpass in range(ROUNDS + 1)) {
        let round = tpass - 1   // pass 0 warms every valid row; measured rounds start at 0
        emit_round(round, best, ok)
        for (slot in range(nv)) {
            let i = (slot + tpass) % nv   // rotate order so every row occupies every slot
            if (!ok[i] || !tune_candidate_active(round, best, ok, i)) {
                continue
            }
            let f = vs[i]._1
            let t0 = ref_time_ticks()
            for (_rep in range(REPS)) {
                sink += f(pa, pb, N)
            }
            let us = double(get_time_usec(t0))
            if (round >= 0 && us < best[i]) {
                best[i] = us
            }
        }
    }
    if (sink == 12345.678f) {
        print("")   // keep sink live
    }
    return report("dot", names, best, ok)
}

// ===== binary in-place: (var d, s)->void — add / mul / copy =====
// ref_elem(d0[j], s[j]) is the expected element; the timing loop resets d=d0 before each block so an
// accumulating/multiplicative kernel stays in normal float range over REPS (no denormal/inf slowdown).

def bench_binary(kernel : string; vs : array<tuple<string; BinFn>>;
                 d0 : array<float>; s : array<float>; ref_elem : function<(a : float; b : float) : double>) : string {
    let nv = length(vs)
    var d : array<float>
    d |> resize(int(N))
    var dp = unsafe(addr(d[0]))
    let sp = unsafe(addr(s[0]))

    var names : array<string>
    var ok : array<bool>
    var best : array<double>
    names |> resize(nv)
    ok |> resize(nv)
    best |> resize(nv)
    for (i in range(nv)) {
        names[i] = vs[i]._0
        best[i] = 1.0e30lf
        reset(d, d0)
        let f = vs[i]._1
        invoke(f, dp, sp, N)
        var maxrel = 0.0lf
        for (j in range(int(N))) {
            let want = ref_elem(d0[j], s[j])
            let got = double(d[j])
            let rel = abs(got - want) / (abs(want) + 1.0lf)
            if (rel > maxrel) {
                maxrel = rel
            }
        }
        ok[i] = maxrel <= TOL
    }

    var sink = 0.0f
    for (tpass in range(ROUNDS + 1)) {
        let round = tpass - 1   // pass 0 warms every valid row; measured rounds start at 0
        emit_round(round, best, ok)
        for (slot in range(nv)) {
            let i = (slot + tpass) % nv   // rotate order so every row occupies every slot
            if (!ok[i] || !tune_candidate_active(round, best, ok, i)) {
                continue
            }
            reset(d, d0)
            let f = vs[i]._1
            let t0 = ref_time_ticks()
            for (_rep in range(REPS)) {
                invoke(f, dp, sp, N)
            }
            let us = double(get_time_usec(t0))
            if (round >= 0 && us < best[i]) {
                best[i] = us
            }
        }
        sink += d[0]
    }
    tune_detail("  checksum {sink}\n")
    return report(kernel, names, best, ok)
}

// ===== axpy: (var d, s, scale)->void =====

def bench_axpy() : string {
    let vs <- axpy_template_variants()
    let nv = length(vs)
    let scale = 0.5f
    var d0 : array<float>
    var s : array<float>
    var d : array<float>
    d0 |> resize(int(N))
    s |> resize(int(N))
    d |> resize(int(N))
    for (i in range(int(N))) {
        d0[i] = 0.5f + 0.3f * sin(float(i) * 0.05f)
        s[i] = cos(float(i) * 0.03f)
    }
    var dp = unsafe(addr(d[0]))
    let sp = unsafe(addr(s[0]))

    var names : array<string>
    var ok : array<bool>
    var best : array<double>
    names |> resize(nv)
    ok |> resize(nv)
    best |> resize(nv)
    for (i in range(nv)) {
        names[i] = vs[i]._0
        best[i] = 1.0e30lf
        reset(d, d0)
        let f = vs[i]._1
        invoke(f, dp, sp, scale, N)
        var maxrel = 0.0lf
        for (j in range(int(N))) {
            let want = double(d0[j]) + double(scale) * double(s[j])
            let rel = abs(double(d[j]) - want) / (abs(want) + 1.0lf)
            if (rel > maxrel) {
                maxrel = rel
            }
        }
        ok[i] = maxrel <= TOL
    }

    var sink = 0.0f
    for (tpass in range(ROUNDS + 1)) {
        let round = tpass - 1   // pass 0 warms every valid row; measured rounds start at 0
        emit_round(round, best, ok)
        for (slot in range(nv)) {
            let i = (slot + tpass) % nv   // rotate order so every row occupies every slot
            if (!ok[i] || !tune_candidate_active(round, best, ok, i)) {
                continue
            }
            reset(d, d0)
            let f = vs[i]._1
            let t0 = ref_time_ticks()
            for (_rep in range(REPS)) {
                invoke(f, dp, sp, scale, N)
            }
            let us = double(get_time_usec(t0))
            if (round >= 0 && us < best[i]) {
                best[i] = us
            }
        }
        sink += d[0]
    }
    tune_detail("  checksum {sink}\n")
    return report("axpy", names, best, ok)
}

// ===== f16 codec kernels: dot_f16 / axpy_f16 / cvt_f32_to_f16 / cvt_f16_to_f32 =====
// The cache side is binary16-in-uint16; references convert via the exact f16_to_f32 bit-twiddle,
// so the dots gate f64-relative and the converts gate EXACT (bit-equality per element).

def bench_dot_f16() : string {
    var a : array<float>
    var b : array<uint16>
    a |> resize(int(N))
    b |> resize(int(N))
    for (i in range(int(N))) {
        a[i] = sin(float(i) * 0.1f)
        b[i] = uint16(f32_to_f16(cos(float(i) * 0.07f)))
    }
    let pa = unsafe(addr(a[0]))
    let pb = unsafe(addr(b[0]))
    var want = 0.0lf
    for (j in range(int(N))) {
        want += double(a[j]) * double(f16_to_f32(uint(b[j])))
    }

    let vs <- dot_f16_template_variants()
    let nv = length(vs)
    var names : array<string>
    var ok : array<bool>
    var best : array<double>
    names |> resize(nv)
    ok |> resize(nv)
    best |> resize(nv)
    for (i in range(nv)) {
        names[i] = vs[i]._0
        let f = vs[i]._1
        let got = double(f(pa, pb, N))
        ok[i] = abs(got - want) / (abs(want) + 1.0lf) <= TOL
        best[i] = 1.0e30lf
    }

    var sink = 0.0f
    for (tpass in range(ROUNDS + 1)) {
        let round = tpass - 1   // pass 0 warms every valid row; measured rounds start at 0
        emit_round(round, best, ok)
        for (slot in range(nv)) {
            let i = (slot + tpass) % nv   // rotate order so every row occupies every slot
            if (!ok[i] || !tune_candidate_active(round, best, ok, i)) {
                continue
            }
            let f = vs[i]._1
            let t0 = ref_time_ticks()
            for (_rep in range(REPS)) {
                sink += f(pa, pb, N)
            }
            let us = double(get_time_usec(t0))
            if (round >= 0 && us < best[i]) {
                best[i] = us
            }
        }
    }
    if (sink == 12345.678f) {
        print("")   // keep sink live
    }
    return report("dot_f16", names, best, ok)
}

def bench_axpy_f16() : string {
    let vs <- axpy_f16_template_variants()
    let nv = length(vs)
    let scale = 0.5f
    var d0 : array<float>
    var s : array<uint16>
    var d : array<float>
    d0 |> resize(int(N))
    s |> resize(int(N))
    d |> resize(int(N))
    for (i in range(int(N))) {
        d0[i] = 0.5f + 0.3f * sin(float(i) * 0.05f)
        s[i] = uint16(f32_to_f16(cos(float(i) * 0.03f)))
    }
    var dp = unsafe(addr(d[0]))
    let sp = unsafe(addr(s[0]))

    var names : array<string>
    var ok : array<bool>
    var best : array<double>
    names |> resize(nv)
    ok |> resize(nv)
    best |> resize(nv)
    for (i in range(nv)) {
        names[i] = vs[i]._0
        best[i] = 1.0e30lf
        reset(d, d0)
        let f = vs[i]._1
        invoke(f, dp, sp, scale, N)
        var maxrel = 0.0lf
        for (j in range(int(N))) {
            let want = double(d0[j]) + double(scale) * double(f16_to_f32(uint(s[j])))
            let rel = abs(double(d[j]) - want) / (abs(want) + 1.0lf)
            if (rel > maxrel) {
                maxrel = rel
            }
        }
        ok[i] = maxrel <= TOL
    }

    var sink = 0.0f
    for (tpass in range(ROUNDS + 1)) {
        let round = tpass - 1   // pass 0 warms every valid row; measured rounds start at 0
        emit_round(round, best, ok)
        for (slot in range(nv)) {
            let i = (slot + tpass) % nv   // rotate order so every row occupies every slot
            if (!ok[i] || !tune_candidate_active(round, best, ok, i)) {
                continue
            }
            reset(d, d0)
            let f = vs[i]._1
            let t0 = ref_time_ticks()
            for (_rep in range(REPS)) {
                invoke(f, dp, sp, scale, N)
            }
            let us = double(get_time_usec(t0))
            if (round >= 0 && us < best[i]) {
                best[i] = us
            }
        }
        sink += d[0]
    }
    tune_detail("  checksum {sink}\n")
    return report("axpy_f16", names, best, ok)
}

def bench_cvt_f32_to_f16() : string {
    let vs <- cvt_f32_to_f16_template_variants()
    let nv = length(vs)
    var s : array<float>
    var d : array<uint16>
    s |> resize(int(N))
    d |> resize(int(N))
    for (i in range(int(N))) {
        s[i] = 300.0f * sin(float(i) * 0.11f)   // spans the normal f16 range
    }
    s[7] = 1.0e6f      // clamp pin
    s[13] = -1.0e6f
    var dp = unsafe(addr(d[0]))
    let sp = unsafe(addr(s[0]))

    var names : array<string>
    var ok : array<bool>
    var best : array<double>
    names |> resize(nv)
    ok |> resize(nv)
    best |> resize(nv)
    for (i in range(nv)) {
        names[i] = vs[i]._0
        best[i] = 1.0e30lf
        let f = vs[i]._1
        invoke(f, dp, sp, N)
        var exact = true
        for (j in range(int(N))) {
            exact &&= uint(d[j]) == f32_to_f16(clamp(s[j], -65504.0, 65504.0))
        }
        ok[i] = exact
    }

    for (tpass in range(ROUNDS + 1)) {
        let round = tpass - 1   // pass 0 warms every valid row; measured rounds start at 0
        emit_round(round, best, ok)
        for (slot in range(nv)) {
            let i = (slot + tpass) % nv   // rotate order so every row occupies every slot
            if (!ok[i] || !tune_candidate_active(round, best, ok, i)) {
                continue
            }
            let f = vs[i]._1
            let t0 = ref_time_ticks()
            for (_rep in range(REPS)) {
                invoke(f, dp, sp, N)   // pure overwrite — no reset needed
            }
            let us = double(get_time_usec(t0))
            if (round >= 0 && us < best[i]) {
                best[i] = us
            }
        }
    }
    return report("cvt_f32_to_f16", names, best, ok)
}

def bench_cvt_f16_to_f32() : string {
    let vs <- cvt_f16_to_f32_template_variants()
    let nv = length(vs)
    var s : array<uint16>
    var d : array<float>
    s |> resize(int(N))
    d |> resize(int(N))
    for (i in range(int(N))) {
        s[i] = uint16(f32_to_f16(300.0f * cos(float(i) * 0.09f)))
    }
    var dp = unsafe(addr(d[0]))
    let sp = unsafe(addr(s[0]))

    var names : array<string>
    var ok : array<bool>
    var best : array<double>
    names |> resize(nv)
    ok |> resize(nv)
    best |> resize(nv)
    for (i in range(nv)) {
        names[i] = vs[i]._0
        best[i] = 1.0e30lf
        let f = vs[i]._1
        invoke(f, dp, sp, N)
        var exact = true
        for (j in range(int(N))) {
            exact &&= d[j] == f16_to_f32(uint(s[j]))
        }
        ok[i] = exact
    }

    for (tpass in range(ROUNDS + 1)) {
        let round = tpass - 1   // pass 0 warms every valid row; measured rounds start at 0
        emit_round(round, best, ok)
        for (slot in range(nv)) {
            let i = (slot + tpass) % nv   // rotate order so every row occupies every slot
            if (!ok[i] || !tune_candidate_active(round, best, ok, i)) {
                continue
            }
            let f = vs[i]._1
            let t0 = ref_time_ticks()
            for (_rep in range(REPS)) {
                invoke(f, dp, sp, N)   // pure overwrite — no reset needed
            }
            let us = double(get_time_usec(t0))
            if (round >= 0 && us < best[i]) {
                best[i] = us
            }
        }
    }
    return report("cvt_f16_to_f32", names, best, ok)
}

// ===== scale: (var d, scale)->void =====

def bench_scale() : string {
    let vs <- scale_inplace_template_variants()
    let nv = length(vs)
    let scale = 1.0001f   // near 1.0 so scale^REPS stays in normal float range with per-block reset
    var d0 : array<float>
    var d : array<float>
    d0 |> resize(int(N))
    d |> resize(int(N))
    for (i in range(int(N))) {
        d0[i] = 0.5f + 0.3f * sin(float(i) * 0.05f)
    }
    var dp = unsafe(addr(d[0]))

    var names : array<string>
    var ok : array<bool>
    var best : array<double>
    names |> resize(nv)
    ok |> resize(nv)
    best |> resize(nv)
    for (i in range(nv)) {
        names[i] = vs[i]._0
        best[i] = 1.0e30lf
        reset(d, d0)
        let f = vs[i]._1
        invoke(f, dp, scale, N)
        var maxrel = 0.0lf
        for (j in range(int(N))) {
            let want = double(d0[j]) * double(scale)
            let rel = abs(double(d[j]) - want) / (abs(want) + 1.0lf)
            if (rel > maxrel) {
                maxrel = rel
            }
        }
        ok[i] = maxrel <= TOL
    }

    var sink = 0.0f
    for (tpass in range(ROUNDS + 1)) {
        let round = tpass - 1   // pass 0 warms every valid row; measured rounds start at 0
        emit_round(round, best, ok)
        for (slot in range(nv)) {
            let i = (slot + tpass) % nv   // rotate order so every row occupies every slot
            if (!ok[i] || !tune_candidate_active(round, best, ok, i)) {
                continue
            }
            reset(d, d0)
            let f = vs[i]._1
            let t0 = ref_time_ticks()
            for (_rep in range(REPS)) {
                invoke(f, dp, scale, N)
            }
            let us = double(get_time_usec(t0))
            if (round >= 0 && us < best[i]) {
                best[i] = us
            }
        }
        sink += d[0]
    }
    tune_detail("  checksum {sink}\n")
    return report("scale_inplace", names, best, ok)
}

// ===== softmax: (var x, size) in-place — reset before each timing block; repeated softmax of a
// distribution stays a bounded distribution, so no denormal/inf drift over REPS.

def bench_softmax() : string {
    let vs <- softmax_template_variants()
    let nv = length(vs)
    var d0 : array<float>
    var d : array<float>
    d0 |> resize(int(N))
    d |> resize(int(N))
    for (i in range(int(N))) {
        d0[i] = 4.0f * sin(float(i) * 0.11f)   // scores in [-4,4], a realistic logit spread
    }
    var dp = unsafe(addr(d[0]))
    var want : array<double>
    want |> resize(int(N))
    var mx = -1.0e30lf
    for (i in range(int(N))) {
        mx = max(mx, double(d0[i]))
    }
    var wsum = 0.0lf
    for (i in range(int(N))) {
        let e = exp(double(d0[i]) - mx)
        want[i] = e
        wsum += e
    }
    for (i in range(int(N))) {
        want[i] /= wsum
    }

    var names : array<string>
    var ok : array<bool>
    var best : array<double>
    names |> resize(nv)
    ok |> resize(nv)
    best |> resize(nv)
    for (i in range(nv)) {
        names[i] = vs[i]._0
        best[i] = 1.0e30lf
        reset(d, d0)
        let f = vs[i]._1
        invoke(f, dp, N)
        var maxrel = 0.0lf
        for (j in range(int(N))) {
            let rel = abs(double(d[j]) - want[j]) / (abs(want[j]) + 1.0lf)
            if (rel > maxrel) {
                maxrel = rel
            }
        }
        ok[i] = maxrel <= TOL
    }

    var sink = 0.0f
    for (tpass in range(ROUNDS + 1)) {
        let round = tpass - 1   // pass 0 warms every valid row; measured rounds start at 0
        emit_round(round, best, ok)
        for (slot in range(nv)) {
            let i = (slot + tpass) % nv   // rotate order so every row occupies every slot
            if (!ok[i] || !tune_candidate_active(round, best, ok, i)) {
                continue
            }
            reset(d, d0)
            let f = vs[i]._1
            let t0 = ref_time_ticks()
            for (_rep in range(REPS)) {
                invoke(f, dp, N)
            }
            let us = double(get_time_usec(t0))
            if (round >= 0 && us < best[i]) {
                best[i] = us
            }
        }
        sink += d[0]
    }
    tune_detail("  checksum {sink}\n")
    return report("softmax", names, best, ok)
}

// ===== rmsnorm: (var o, x, weight, size, eps) out-of-place — repeatable, no reset needed.

def bench_rmsnorm() : string {
    let vs <- rmsnorm_template_variants()
    let nv = length(vs)
    let eps = 1.0e-5f
    var x : array<float>
    var w : array<float>
    var o : array<float>
    x |> resize(int(N))
    w |> resize(int(N))
    o |> resize(int(N))
    for (i in range(int(N))) {
        x[i] = sin(float(i) * 0.1f)
        w[i] = 1.0f + 0.2f * cos(float(i) * 0.07f)
    }
    var op = unsafe(addr(o[0]))
    let xp = unsafe(addr(x[0]))
    let wp = unsafe(addr(w[0]))
    var ss = 0.0lf
    for (i in range(int(N))) {
        ss += double(x[i]) * double(x[i])
    }
    ss = 1.0lf / sqrt(ss / double(N) + double(eps))

    var names : array<string>
    var ok : array<bool>
    var best : array<double>
    names |> resize(nv)
    ok |> resize(nv)
    best |> resize(nv)
    for (i in range(nv)) {
        names[i] = vs[i]._0
        best[i] = 1.0e30lf
        let f = vs[i]._1
        invoke(f, op, xp, wp, N, eps)
        var maxrel = 0.0lf
        for (j in range(int(N))) {
            let want = double(w[j]) * double(x[j]) * ss
            let rel = abs(double(o[j]) - want) / (abs(want) + 1.0lf)
            if (rel > maxrel) {
                maxrel = rel
            }
        }
        ok[i] = maxrel <= TOL
    }

    var sink = 0.0f
    for (tpass in range(ROUNDS + 1)) {
        let round = tpass - 1   // pass 0 warms every valid row; measured rounds start at 0
        emit_round(round, best, ok)
        for (slot in range(nv)) {
            let i = (slot + tpass) % nv   // rotate order so every row occupies every slot
            if (!ok[i] || !tune_candidate_active(round, best, ok, i)) {
                continue
            }
            let f = vs[i]._1
            let t0 = ref_time_ticks()
            for (_rep in range(REPS)) {
                invoke(f, op, xp, wp, N, eps)
            }
            let us = double(get_time_usec(t0))
            if (round >= 0 && us < best[i]) {
                best[i] = us
            }
        }
        sink += o[0]
    }
    tune_detail("  checksum {sink}\n")
    return report("rmsnorm", names, best, ok)
}

// ===== dot_q4 / dot_q8q8: quantized dots, repeatable (no mutation). References run in f64 on the
// DEQUANTIZED values (q4) / the exact int block sums (q8q8).

def bench_dot_q4() : string {
    let vs <- dot_q4_template_variants()
    let nv = length(vs)
    var wf : array<float>
    var x : array<float>
    wf |> resize(int(N))
    x |> resize(int(N))
    for (i in range(int(N))) {
        wf[i] = sin(float(i) * 0.1f)
        x[i] = cos(float(i) * 0.07f)
    }
    let qt <- quantize_q4_0(wf, N)
    var df : array<float>
    df |> resize(int(N))
    dequantize_q4_0(qt, df)
    var want = 0.0lf
    for (i in range(int(N))) {
        want += double(df[i]) * double(x[i])
    }
    let wqp = unsafe(addr(qt.q[0]))
    let wsp = unsafe(addr(qt.scales[0]))
    let xp = unsafe(addr(x[0]))

    var names : array<string>
    var ok : array<bool>
    var best : array<double>
    names |> resize(nv)
    ok |> resize(nv)
    best |> resize(nv)
    for (i in range(nv)) {
        names[i] = vs[i]._0
        best[i] = 1.0e30lf
        let f = vs[i]._1
        let got = double(f(wqp, wsp, xp, N))
        ok[i] = abs(got - want) / (abs(want) + 1.0lf) <= TOL
    }

    var sink = 0.0f
    for (tpass in range(ROUNDS + 1)) {
        let round = tpass - 1   // pass 0 warms every valid row; measured rounds start at 0
        emit_round(round, best, ok)
        for (slot in range(nv)) {
            let i = (slot + tpass) % nv   // rotate order so every row occupies every slot
            if (!ok[i] || !tune_candidate_active(round, best, ok, i)) {
                continue
            }
            let f = vs[i]._1
            let t0 = ref_time_ticks()
            for (_rep in range(REPS)) {
                sink += f(wqp, wsp, xp, N)
            }
            let us = double(get_time_usec(t0))
            if (round >= 0 && us < best[i]) {
                best[i] = us
            }
        }
    }
    if (sink == 12345.678f) {
        print("")   // keep sink live
    }
    return report("dot_q4", names, best, ok)
}

def bench_dot_q8q8() : string {
    let vs <- dot_q8q8_template_variants()
    let nv = length(vs)
    var wf : array<float>
    var xf : array<float>
    wf |> resize(int(N))
    xf |> resize(int(N))
    for (i in range(int(N))) {
        wf[i] = sin(float(i) * 0.1f)
        xf[i] = cos(float(i) * 0.07f)
    }
    let wq <- quantize_q8_0(wf, N)
    let xq <- quantize_q8_0(xf, N)
    let nb = N / 32l
    var want = 0.0lf
    for (bi in range64(nb)) {
        var iacc = 0
        for (k in range64(32l)) {
            iacc += int(wq.q[bi * 32l + k]) * int(xq.q[bi * 32l + k])
        }
        want += double(iacc) * double(wq.scales[bi]) * double(xq.scales[bi])
    }
    let wqp = unsafe(addr(wq.q[0]))
    let wsp = unsafe(addr(wq.scales[0]))
    let xqp = unsafe(addr(xq.q[0]))
    let xsp = unsafe(addr(xq.scales[0]))

    var names : array<string>
    var ok : array<bool>
    var best : array<double>
    names |> resize(nv)
    ok |> resize(nv)
    best |> resize(nv)
    for (i in range(nv)) {
        names[i] = vs[i]._0
        best[i] = 1.0e30lf
        let f = vs[i]._1
        let got = double(f(wqp, wsp, xqp, xsp, N))
        ok[i] = abs(got - want) / (abs(want) + 1.0lf) <= TOL
    }

    var sink = 0.0f
    for (tpass in range(ROUNDS + 1)) {
        let round = tpass - 1   // pass 0 warms every valid row; measured rounds start at 0
        emit_round(round, best, ok)
        for (slot in range(nv)) {
            let i = (slot + tpass) % nv   // rotate order so every row occupies every slot
            if (!ok[i] || !tune_candidate_active(round, best, ok, i)) {
                continue
            }
            let f = vs[i]._1
            let t0 = ref_time_ticks()
            for (_rep in range(REPS)) {
                sink += f(wqp, wsp, xqp, xsp, N)
            }
            let us = double(get_time_usec(t0))
            if (round >= 0 && us < best[i]) {
                best[i] = us
            }
        }
    }
    if (sink == 12345.678f) {
        print("")   // keep sink live
    }
    return report("dot_q8q8", names, best, ok)
}

// ===== q8_0 KV-codec kernels: dot_q8kv / dot_q8q8kv / axpy_q8kv / cvt_q8kv_to_f32 /
// quantize_q8kv_row. The cache side is interleaved ggml block_q8_0 ({f16 d; int8 qs[32]} = 34
// bytes per 32 elements); the reference row below IS the quantize_q8kv_row contract, so the
// quantize bench gates byte-EXACT against it, the cvt gates element-exact, and the dots gate
// f64-relative on the exact int block sums / dequantized values.

// reference q8_0 row builder — scalar amax (order-free == the kernel's float4 form), f16 scale,
// f32-id rounding: quantize_q8kv_row's exact semantics
def private build_q8kv_row(src : array<float>; n : int64; var row : array<uint8>) {
    row |> resize(int(n / 32l * 34l))
    for (bi in range64(n / 32l)) {
        let base = bi * 32l
        var amax = 0.0
        for (k in range64(32l)) {
            amax = max(amax, abs(src[base + k]))
        }
        let d = amax / 127.0
        let id = d != 0.0 ? 1.0 / d : 0.0
        let dh = f32_to_f16(d)
        let bb = bi * 34l
        row[bb] = uint8(dh & 0xFF)
        row[bb + 1l] = uint8((dh >> 8u) & 0xFF)
        for (k in range64(32l)) {
            row[bb + 2l + k] = uint8(int(round(src[base + k] * id)) & int(0xFF))
        }
    }
}

// dequantized element (bi*32 + k) of a reference row, in f64 — for the dot/axpy references
def private q8kv_elem(row : array<uint8>; i : int64) : double {
    let bb = i / 32l * 34l
    let dh = uint(row[bb]) | (uint(row[bb + 1l]) << 8u)
    let q = int(int8(row[bb + 2l + i % 32l]))
    return double(q) * double(f16_to_f32(dh))
}

def bench_dot_q8kv() : string {
    var a : array<float>
    var wf : array<float>
    var row : array<uint8>
    a |> resize(int(N))
    wf |> resize(int(N))
    for (i in range(int(N))) {
        a[i] = sin(float(i) * 0.1f)
        wf[i] = cos(float(i) * 0.07f)
    }
    build_q8kv_row(wf, N, row)
    var want = 0.0lf
    for (j in range64(N)) {
        want += double(a[j]) * q8kv_elem(row, j)
    }
    let pa = unsafe(addr(a[0]))
    let pb = unsafe(addr(row[0]))

    let vs <- dot_q8kv_template_variants()
    let nv = length(vs)
    var names : array<string>
    var ok : array<bool>
    var best : array<double>
    names |> resize(nv)
    ok |> resize(nv)
    best |> resize(nv)
    for (i in range(nv)) {
        names[i] = vs[i]._0
        best[i] = 1.0e30lf
        let f = vs[i]._1
        let got = double(f(pa, pb, N))
        ok[i] = abs(got - want) / (abs(want) + 1.0lf) <= TOL
    }

    var sink = 0.0f
    for (tpass in range(ROUNDS + 1)) {
        let round = tpass - 1   // pass 0 warms every valid row; measured rounds start at 0
        emit_round(round, best, ok)
        for (slot in range(nv)) {
            let i = (slot + tpass) % nv   // rotate order so every row occupies every slot
            if (!ok[i] || !tune_candidate_active(round, best, ok, i)) {
                continue
            }
            let f = vs[i]._1
            let t0 = ref_time_ticks()
            for (_rep in range(REPS)) {
                sink += f(pa, pb, N)
            }
            let us = double(get_time_usec(t0))
            if (round >= 0 && us < best[i]) {
                best[i] = us
            }
        }
    }
    if (sink == 12345.678f) {
        print("")   // keep sink live
    }
    return report("dot_q8kv", names, best, ok)
}

def bench_dot_q8q8kv() : string {
    var xf : array<float>
    var wf : array<float>
    var row : array<uint8>
    xf |> resize(int(N))
    wf |> resize(int(N))
    for (i in range(int(N))) {
        xf[i] = sin(float(i) * 0.1f)
        wf[i] = cos(float(i) * 0.07f)
    }
    build_q8kv_row(wf, N, row)
    let xq <- quantize_q8_0(xf, N)   // the query planes (int8 quants + f32 scales)
    var want = 0.0lf
    for (bi in range64(N / 32l)) {
        let bb = bi * 34l
        let dh = uint(row[bb]) | (uint(row[bb + 1l]) << 8u)
        var iacc = 0
        for (k in range64(32l)) {
            iacc += int(xq.q[bi * 32l + k]) * int(int8(row[bb + 2l + k]))
        }
        want += double(iacc) * double(xq.scales[bi]) * double(f16_to_f32(dh))
    }
    let qp = unsafe(addr(xq.q[0]))
    let sp = unsafe(addr(xq.scales[0]))
    let pb = unsafe(addr(row[0]))

    let vs <- dot_q8q8kv_template_variants()
    let nv = length(vs)
    var names : array<string>
    var ok : array<bool>
    var best : array<double>
    names |> resize(nv)
    ok |> resize(nv)
    best |> resize(nv)
    for (i in range(nv)) {
        names[i] = vs[i]._0
        best[i] = 1.0e30lf
        let f = vs[i]._1
        let got = double(f(qp, sp, pb, N))
        ok[i] = abs(got - want) / (abs(want) + 1.0lf) <= TOL
    }

    var sink = 0.0f
    for (tpass in range(ROUNDS + 1)) {
        let round = tpass - 1   // pass 0 warms every valid row; measured rounds start at 0
        emit_round(round, best, ok)
        for (slot in range(nv)) {
            let i = (slot + tpass) % nv   // rotate order so every row occupies every slot
            if (!ok[i] || !tune_candidate_active(round, best, ok, i)) {
                continue
            }
            let f = vs[i]._1
            let t0 = ref_time_ticks()
            for (_rep in range(REPS)) {
                sink += f(qp, sp, pb, N)
            }
            let us = double(get_time_usec(t0))
            if (round >= 0 && us < best[i]) {
                best[i] = us
            }
        }
    }
    if (sink == 12345.678f) {
        print("")   // keep sink live
    }
    return report("dot_q8q8kv", names, best, ok)
}

def bench_axpy_q8kv() : string {
    let vs <- axpy_q8kv_template_variants()
    let nv = length(vs)
    let scale = 0.5f
    var d0 : array<float>
    var wf : array<float>
    var row : array<uint8>
    var d : array<float>
    d0 |> resize(int(N))
    wf |> resize(int(N))
    d |> resize(int(N))
    for (i in range(int(N))) {
        d0[i] = 0.5f + 0.3f * sin(float(i) * 0.05f)
        wf[i] = cos(float(i) * 0.03f)
    }
    build_q8kv_row(wf, N, row)
    var dp = unsafe(addr(d[0]))
    let sp = unsafe(addr(row[0]))

    var names : array<string>
    var ok : array<bool>
    var best : array<double>
    names |> resize(nv)
    ok |> resize(nv)
    best |> resize(nv)
    for (i in range(nv)) {
        names[i] = vs[i]._0
        best[i] = 1.0e30lf
        reset(d, d0)
        let f = vs[i]._1
        invoke(f, dp, sp, scale, N)
        var maxrel = 0.0lf
        for (j in range64(N)) {
            let want = double(d0[j]) + double(scale) * q8kv_elem(row, j)
            let rel = abs(double(d[j]) - want) / (abs(want) + 1.0lf)
            if (rel > maxrel) {
                maxrel = rel
            }
        }
        ok[i] = maxrel <= TOL
    }

    var sink = 0.0f
    for (tpass in range(ROUNDS + 1)) {
        let round = tpass - 1   // pass 0 warms every valid row; measured rounds start at 0
        emit_round(round, best, ok)
        for (slot in range(nv)) {
            let i = (slot + tpass) % nv   // rotate order so every row occupies every slot
            if (!ok[i] || !tune_candidate_active(round, best, ok, i)) {
                continue
            }
            reset(d, d0)
            let f = vs[i]._1
            let t0 = ref_time_ticks()
            for (_rep in range(REPS)) {
                invoke(f, dp, sp, scale, N)
            }
            let us = double(get_time_usec(t0))
            if (round >= 0 && us < best[i]) {
                best[i] = us
            }
        }
        sink += d[0]
    }
    tune_detail("  checksum {sink}\n")
    return report("axpy_q8kv", names, best, ok)
}

def bench_cvt_q8kv_to_f32() : string {
    let vs <- cvt_q8kv_to_f32_template_variants()
    let nv = length(vs)
    var wf : array<float>
    var row : array<uint8>
    var d : array<float>
    wf |> resize(int(N))
    d |> resize(int(N))
    for (i in range(int(N))) {
        wf[i] = 300.0f * cos(float(i) * 0.09f)
    }
    build_q8kv_row(wf, N, row)
    var dp = unsafe(addr(d[0]))
    let sp = unsafe(addr(row[0]))

    var names : array<string>
    var ok : array<bool>
    var best : array<double>
    names |> resize(nv)
    ok |> resize(nv)
    best |> resize(nv)
    for (i in range(nv)) {
        names[i] = vs[i]._0
        best[i] = 1.0e30lf
        for (j in range(int(N))) {
            d[j] = -1.0e30f
        }
        let f = vs[i]._1
        invoke(f, dp, sp, N)
        var exact = true
        for (j in range64(N)) {
            exact &&= double(d[j]) == q8kv_elem(row, j)   // float(q)*f16(d) is exact in f32
        }
        ok[i] = exact
    }

    var sink = 0.0f
    for (tpass in range(ROUNDS + 1)) {
        let round = tpass - 1   // pass 0 warms every valid row; measured rounds start at 0
        emit_round(round, best, ok)
        for (slot in range(nv)) {
            let i = (slot + tpass) % nv   // rotate order so every row occupies every slot
            if (!ok[i] || !tune_candidate_active(round, best, ok, i)) {
                continue
            }
            let f = vs[i]._1
            let t0 = ref_time_ticks()
            for (_rep in range(REPS)) {
                invoke(f, dp, sp, N)
            }
            let us = double(get_time_usec(t0))
            if (round >= 0 && us < best[i]) {
                best[i] = us
            }
        }
        sink += d[0]
    }
    tune_detail("  checksum {sink}\n")
    return report("cvt_q8kv_to_f32", names, best, ok)
}

def bench_quantize_q8kv_row() : string {
    let vs <- quantize_q8kv_row_template_variants()
    let nv = length(vs)
    var src : array<float>
    var ref_row : array<uint8>
    var d : array<uint8>
    src |> resize(int(N))
    for (i in range(int(N))) {
        src[i] = 300.0f * sin(float(i) * 0.11f)
    }
    build_q8kv_row(src, N, ref_row)   // the contract: byte-exact ggml quantize_row_q8_0
    d |> resize(length(ref_row))
    var dp = unsafe(addr(d[0]))
    let sp = unsafe(addr(src[0]))

    var names : array<string>
    var ok : array<bool>
    var best : array<double>
    names |> resize(nv)
    ok |> resize(nv)
    best |> resize(nv)
    for (i in range(nv)) {
        names[i] = vs[i]._0
        best[i] = 1.0e30lf
        for (b in d) {
            b = uint8(0xA5)
        }
        let f = vs[i]._1
        invoke(f, dp, sp, N)
        var exact = true
        for (j in range(length(d))) {
            exact &&= d[j] == ref_row[j]
        }
        ok[i] = exact
    }

    var sink = 0
    for (tpass in range(ROUNDS + 1)) {
        let round = tpass - 1   // pass 0 warms every valid row; measured rounds start at 0
        emit_round(round, best, ok)
        for (slot in range(nv)) {
            let i = (slot + tpass) % nv   // rotate order so every row occupies every slot
            if (!ok[i] || !tune_candidate_active(round, best, ok, i)) {
                continue
            }
            let f = vs[i]._1
            let t0 = ref_time_ticks()
            for (_rep in range(REPS)) {
                invoke(f, dp, sp, N)
            }
            let us = double(get_time_usec(t0))
            if (round >= 0 && us < best[i]) {
                best[i] = us
            }
        }
        sink += int(d[0])
    }
    tune_detail("  checksum {sink}\n")
    return report("quantize_q8kv_row", names, best, ok)
}

// ===== dot_mx4q8: native-MXFP4 weight planes (deterministic nibbles + E8M0 bytes) vs Q8 activation.
// The vec* perms are inert (straight-line tbl16/sdot4_w intrinsic body — the hint is ignored), so
// only the unroll rows genuinely compete, exactly like laneq4x4.

def bench_dot_mx4q8() : string {
    let vs <- dot_mx4q8_template_variants()
    let nv = length(vs)
    let nb = N / 32l
    var wn : array<uint8>
    var we : array<uint8>
    wn |> resize(int(N / 2l))
    we |> resize(int(nb))
    for (i in range(int(N / 2l))) {
        wn[i] = uint8((i * 137 + 71) & 0xff)
    }
    for (i in range(int(nb))) {
        we[i] = uint8(120 + (i * 7) % 16)
    }
    var xf : array<float>
    xf |> resize(int(N))
    for (i in range(int(N))) {
        xf[i] = cos(float(i) * 0.07f)
    }
    let xq <- quantize_q8_0(xf, N)
    var lut : int8[16]
    mxfp4_fill_lut(lut)
    var want = 0.0lf
    for (bi in range64(nb)) {
        var iacc = 0
        for (j in range64(16l)) {
            let b = int(wn[bi * 16l + j])
            iacc += int(lut[b & 15]) * int(xq.q[bi * 32l + j])
            iacc += int(lut[b >> 4]) * int(xq.q[bi * 32l + j + 16l])
        }
        want += double(iacc) * double(e8m0_half(uint(we[bi]))) * double(xq.scales[bi])
    }
    let lutp = unsafe(addr(lut[0]))
    let wnp = unsafe(addr(wn[0]))
    let wep = unsafe(addr(we[0]))
    let xqp = unsafe(addr(xq.q[0]))
    let xsp = unsafe(addr(xq.scales[0]))

    var names : array<string>
    var ok : array<bool>
    var best : array<double>
    names |> resize(nv)
    ok |> resize(nv)
    best |> resize(nv)
    for (i in range(nv)) {
        names[i] = vs[i]._0
        best[i] = 1.0e30lf
        let f = vs[i]._1
        let got = double(f(lutp, wnp, wep, xqp, xsp, N))
        ok[i] = abs(got - want) / (abs(want) + 1.0lf) <= TOL
    }

    var sink = 0.0f
    for (tpass in range(ROUNDS + 1)) {
        let round = tpass - 1   // pass 0 warms every valid row; measured rounds start at 0
        emit_round(round, best, ok)
        for (slot in range(nv)) {
            let i = (slot + tpass) % nv   // rotate order so every row occupies every slot
            if (!ok[i] || !tune_candidate_active(round, best, ok, i)) {
                continue
            }
            let f = vs[i]._1
            let t0 = ref_time_ticks()
            for (_rep in range(REPS)) {
                sink += f(lutp, wnp, wep, xqp, xsp, N)
            }
            let us = double(get_time_usec(t0))
            if (round >= 0 && us < best[i]) {
                best[i] = us
            }
        }
    }
    if (sink == 12345.678f) {
        print("")   // keep sink live
    }
    return report("dot_mx4q8", names, best, ok)
}

// ===== quantize_q8_0_into_ptr: EXACT gate — every variant must produce byte-identical quants and
// scales to the module's quantize_q8_0 reference (the store loop is elementwise round+narrow; any
// difference under a perm is a miscompile, not a tolerance question).

def bench_quantize() : string {
    let vs <- quantize_q8_0_into_ptr_template_variants()
    let nv = length(vs)
    var src : array<float>
    src |> resize(int(N))
    for (i in range(int(N))) {
        src[i] = 2.0f * sin(float(i) * 0.13f)
    }
    let want <- quantize_q8_0(src, N)
    var qdst : array<int8>
    var sdst : array<float>
    qdst |> resize(int(N))
    sdst |> resize(int(N / 32l))
    let srcp = unsafe(addr(src[0]))
    var qp = unsafe(addr(qdst[0]))
    var sp = unsafe(addr(sdst[0]))

    var names : array<string>
    var ok : array<bool>
    var best : array<double>
    names |> resize(nv)
    ok |> resize(nv)
    best |> resize(nv)
    for (i in range(nv)) {
        names[i] = vs[i]._0
        best[i] = 1.0e30lf
        let f = vs[i]._1
        invoke(f, srcp, N, qp, sp, 0l, 0l)
        var exact = true
        for (j in range(int(N))) {
            if (qdst[j] != want.q[j]) {
                exact = false
            }
        }
        for (b in range(int(N / 32l))) {
            if (sdst[b] != want.scales[b]) {
                exact = false
            }
        }
        ok[i] = exact
    }

    var sink = 0.0f
    for (tpass in range(ROUNDS + 1)) {
        let round = tpass - 1   // pass 0 warms every valid row; measured rounds start at 0
        emit_round(round, best, ok)
        for (slot in range(nv)) {
            let i = (slot + tpass) % nv   // rotate order so every row occupies every slot
            if (!ok[i] || !tune_candidate_active(round, best, ok, i)) {
                continue
            }
            let f = vs[i]._1
            let t0 = ref_time_ticks()
            for (_rep in range(REPS)) {
                invoke(f, srcp, N, qp, sp, 0l, 0l)
            }
            let us = double(get_time_usec(t0))
            if (round >= 0 && us < best[i]) {
                best[i] = us
            }
        }
        sink += sdst[0]
    }
    tune_detail("  checksum {sink}\n")
    return report("quantize_q8_0_into_ptr", names, best, ok)
}

// quantize + per-16 block sums (the K-quant activation pass): quants/scales must be byte-identical
// to quantize_q8_0, sums exactly the integer sums of those quants.
def bench_quantize_bs() : string {
    let vs <- quantize_q8_0_bs_into_ptr_template_variants()
    let nv = length(vs)
    var src : array<float>
    src |> resize(int(N))
    for (i in range(int(N))) {
        src[i] = 2.0f * sin(float(i) * 0.13f)
    }
    let want <- quantize_q8_0(src, N)
    var wantbs : array<int>
    wantbs |> resize(int(N / 16l))
    for (h in range(int(N / 16l))) {
        var bs = 0
        for (i in range(16)) {
            bs += int(want.q[h * 16 + i])
        }
        wantbs[h] = bs
    }
    var qdst : array<int8>
    var sdst : array<float>
    var bdst : array<int>
    qdst |> resize(int(N))
    sdst |> resize(int(N / 32l))
    bdst |> resize(int(N / 16l))
    let srcp = unsafe(addr(src[0]))
    var qp = unsafe(addr(qdst[0]))
    var sp = unsafe(addr(sdst[0]))
    var bp = unsafe(addr(bdst[0]))

    var names : array<string>
    var ok : array<bool>
    var best : array<double>
    names |> resize(nv)
    ok |> resize(nv)
    best |> resize(nv)
    for (i in range(nv)) {
        names[i] = vs[i]._0
        best[i] = 1.0e30lf
        let f = vs[i]._1
        invoke(f, srcp, N, qp, sp, bp, 0l, 0l, 0l)
        var exact = true
        for (j in range(int(N))) {
            if (qdst[j] != want.q[j]) {
                exact = false
            }
        }
        for (b in range(int(N / 32l))) {
            if (sdst[b] != want.scales[b]) {
                exact = false
            }
        }
        for (h in range(int(N / 16l))) {
            if (bdst[h] != wantbs[h]) {
                exact = false
            }
        }
        ok[i] = exact
    }

    var sink = 0.0f
    for (tpass in range(ROUNDS + 1)) {
        let round = tpass - 1   // pass 0 warms every valid row; measured rounds start at 0
        emit_round(round, best, ok)
        for (slot in range(nv)) {
            let i = (slot + tpass) % nv   // rotate order so every row occupies every slot
            if (!ok[i] || !tune_candidate_active(round, best, ok, i)) {
                continue
            }
            let f = vs[i]._1
            let t0 = ref_time_ticks()
            for (_rep in range(REPS)) {
                invoke(f, srcp, N, qp, sp, bp, 0l, 0l, 0l)
            }
            let us = double(get_time_usec(t0))
            if (round >= 0 && us < best[i]) {
                best[i] = us
            }
        }
        sink += sdst[0]
    }
    tune_detail("  checksum {sink}\n")
    return report("quantize_q8_0_bs_into_ptr", names, best, ok)
}

// ===== rope_scaled_neox_tab: in-place rotation, norm-preserving — timing runs without reset.

def bench_rope_tab() : string {
    let vs <- rope_scaled_neox_tab_template_variants()
    let nv = length(vs)
    let head_size = 64l
    let half = head_size / 2l
    var v0 : array<float>
    var v : array<float>
    v0 |> resize(int(N))
    v |> resize(int(N))
    for (i in range(int(N))) {
        v0[i] = sin(float(i) * 0.1f)
    }
    var cosr : array<float>
    var sinr : array<float>
    cosr |> resize(int(half))
    sinr |> resize(int(half))
    for (j in range(int(half))) {
        let ang = 37.0f / pow(10000.0f, float(2 * j) / float(head_size))
        cosr[j] = cos(ang)
        sinr[j] = sin(ang)
    }
    var vp = unsafe(addr(v[0]))
    let cp = unsafe(addr(cosr[0]))
    let sp = unsafe(addr(sinr[0]))

    var names : array<string>
    var ok : array<bool>
    var best : array<double>
    names |> resize(nv)
    ok |> resize(nv)
    best |> resize(nv)
    for (i in range(nv)) {
        names[i] = vs[i]._0
        best[i] = 1.0e30lf
        reset(v, v0)
        let f = vs[i]._1
        invoke(f, vp, head_size, N, cp, sp)
        var maxrel = 0.0lf
        for (h in range(int(N / head_size))) {
            let base = h * int(head_size)
            for (j in range(int(half))) {
                let a = double(v0[base + j])
                let b = double(v0[base + j + int(half)])
                let wc = double(cosr[j])
                let ws = double(sinr[j])
                let want0 = a * wc - b * ws
                let want1 = a * ws + b * wc
                let r0 = abs(double(v[base + j]) - want0) / (abs(want0) + 1.0lf)
                let r1 = abs(double(v[base + j + int(half)]) - want1) / (abs(want1) + 1.0lf)
                maxrel = max(maxrel, max(r0, r1))
            }
        }
        ok[i] = maxrel <= TOL
    }

    var sink = 0.0f
    for (tpass in range(ROUNDS + 1)) {
        let round = tpass - 1   // pass 0 warms every valid row; measured rounds start at 0
        emit_round(round, best, ok)
        for (slot in range(nv)) {
            let i = (slot + tpass) % nv   // rotate order so every row occupies every slot
            if (!ok[i] || !tune_candidate_active(round, best, ok, i)) {
                continue
            }
            reset(v, v0)
            let f = vs[i]._1
            let t0 = ref_time_ticks()
            for (_rep in range(REPS)) {
                invoke(f, vp, head_size, N, cp, sp)
            }
            let us = double(get_time_usec(t0))
            if (round >= 0 && us < best[i]) {
                best[i] = us
            }
        }
        sink += v[0]
    }
    tune_detail("  checksum {sink}\n")
    return report("rope_scaled_neox_tab", names, best, ok)
}

// ===== gemm_f32_uk_4x16: one 4×16 tile over K, C += A·B — reset C per round; over REPS the
// accumulator grows linearly (REPS × O(1) entries), well inside normal float range.

def bench_gemm_tile() : string {
    let vs <- gemm_f32_uk_4x16_template_variants()
    let nv = length(vs)
    let K = 256l
    let NN = 16l   // C/B row stride == tile width, so the 4×16 tile is the whole C
    var a : array<float>
    var b : array<float>
    var c : array<float>
    a |> resize(int(4l * K))
    b |> resize(int(K * NN))
    c |> resize(int(4l * NN))
    for (i in range(int(4l * K))) {
        a[i] = sin(float(i) * 0.1f)
    }
    for (i in range(int(K * NN))) {
        b[i] = cos(float(i) * 0.07f)
    }
    var cp = unsafe(addr(c[0]))
    let ap = unsafe(addr(a[0]))
    let bp = unsafe(addr(b[0]))

    var names : array<string>
    var ok : array<bool>
    var best : array<double>
    names |> resize(nv)
    ok |> resize(nv)
    best |> resize(nv)
    for (i in range(nv)) {
        names[i] = vs[i]._0
        best[i] = 1.0e30lf
        for (j in range(int(4l * NN))) {
            c[j] = 0.0f
        }
        let f = vs[i]._1
        invoke(f, cp, ap, bp, 0l, 0l, K, NN)
        var maxrel = 0.0lf
        for (r in range(4)) {
            for (col in range(int(NN))) {
                var want = 0.0lf
                for (kk in range(int(K))) {
                    want += double(a[r * int(K) + kk]) * double(b[kk * int(NN) + col])
                }
                let rel = abs(double(c[r * int(NN) + col]) - want) / (abs(want) + 1.0lf)
                if (rel > maxrel) {
                    maxrel = rel
                }
            }
        }
        ok[i] = maxrel <= TOL
    }

    var sink = 0.0f
    for (tpass in range(ROUNDS + 1)) {
        let round = tpass - 1   // pass 0 warms every valid row; measured rounds start at 0
        emit_round(round, best, ok)
        for (slot in range(nv)) {
            let i = (slot + tpass) % nv   // rotate order so every row occupies every slot
            if (!ok[i] || !tune_candidate_active(round, best, ok, i)) {
                continue
            }
            for (j in range(int(4l * NN))) {
                c[j] = 0.0f
            }
            let f = vs[i]._1
            let t0 = ref_time_ticks()
            for (_rep in range(REPS)) {
                invoke(f, cp, ap, bp, 0l, 0l, K, NN)
            }
            let us = double(get_time_usec(t0))
            if (round >= 0 && us < best[i]) {
                best[i] = us
            }
        }
        sink += c[0]
    }
    tune_detail("  checksum {sink}\n")
    return report("gemm_f32_uk_4x16", names, best, ok)
}

// ===== dot_q8q8_laneq4x4: arm64+JIT only (the repack backend must be registered so the pin lands
// and the intrinsic lowers to hardware SDOT). Runs LAST — the pin sticks until process exit.

def bench_laneq4x4() : string {
    if (get_architecture_name() != "arm64" || !jit_enabled()) {
        tune_detail("--- dot_q8q8_laneq4x4 ---\n  skipped (needs arm64 + -jit: lane-indexed SDOT + the repack backend)\n")
        return ""
    }
    let vs <- dot_q8q8_laneq4x4_template_variants()
    let nv = length(vs)
    let n = N
    let nb = n / 32l
    let d = 4l     // one interleaved 4-row group
    let ntok = 4l
    var wf : array<float>
    wf |> resize(int(d * n))
    for (i in range(int(d * n))) {
        wf[i] = sin(float(i) * 0.1f)
    }
    var wq <- quantize_q8_0(wf, d * n)
    var refq : array<int8>    // row-major copies for the reference (repack rewrites wq in place)
    var refs : array<float>
    refq |> resize(int(d * n))
    refs |> resize(int(d * nb))
    for (i in range(int(d * n))) {
        refq[i] = wq.q[i]
    }
    for (i in range(int(d * nb))) {
        refs[i] = wq.scales[i]
    }
    pin_kernel_backend("arm64-gen")               // activates the repack fn the next call uses; the
                                                  // arm64 default stamp is mr=4 — byte-identical to
                                                  // the laneq4x4 tile's expected interleave (the
                                                  // correctness gate below catches any mr drift)
    repack_q8q8_weight(wq.q, wq.scales, 0l, n, d)
    var xf : array<float>
    xf |> resize(int(ntok * n))
    for (i in range(int(ntok * n))) {
        xf[i] = cos(float(i) * 0.07f)
    }
    var xq : array<int8>
    var xs : array<float>
    xq |> resize(int(ntok * n))
    xs |> resize(int(ntok * nb))
    quantize_q8_0_into(xf, ntok * n, xq, xs, 0l, 0l)
    var y : array<float>
    y |> resize(int(ntok * d))
    var yp = unsafe(addr(y[0]))
    let wgp = unsafe(addr(wq.q[0]))
    let sgp = unsafe(addr(wq.scales[0]))
    let xqp = unsafe(addr(xq[0]))
    let xsp = unsafe(addr(xs[0]))

    var names : array<string>
    var ok : array<bool>
    var best : array<double>
    names |> resize(nv)
    ok |> resize(nv)
    best |> resize(nv)
    for (i in range(nv)) {
        names[i] = vs[i]._0
        best[i] = 1.0e30lf
        let f = vs[i]._1
        invoke(f, yp, wgp, sgp, xqp, xsp, n, d, 0l, 0l)
        var maxrel = 0.0lf
        for (t in range(int(ntok))) {
            for (r in range(int(d))) {
                var want = 0.0lf
                for (bi in range64(nb)) {
                    var iacc = 0
                    for (k in range64(32l)) {
                        iacc += int(refq[r * int(n) + int(bi * 32l + k)]) * int(xq[t * int(n) + int(bi * 32l + k)])
                    }
                    want += double(iacc) * double(refs[r * int(nb) + int(bi)]) * double(xs[t * int(nb) + int(bi)])
                }
                let got = double(y[t * int(d) + r])
                let rel = abs(got - want) / (abs(want) + 1.0lf)
                if (rel > maxrel) {
                    maxrel = rel
                }
            }
        }
        ok[i] = maxrel <= TOL
    }

    var sink = 0.0f
    for (tpass in range(ROUNDS + 1)) {
        let round = tpass - 1   // pass 0 warms every valid row; measured rounds start at 0
        emit_round(round, best, ok)
        for (slot in range(nv)) {
            let i = (slot + tpass) % nv   // rotate order so every row occupies every slot
            if (!ok[i] || !tune_candidate_active(round, best, ok, i)) {
                continue
            }
            let f = vs[i]._1
            let t0 = ref_time_ticks()
            for (_rep in range(REPS)) {
                invoke(f, yp, wgp, sgp, xqp, xsp, n, d, 0l, 0l)
            }
            let us = double(get_time_usec(t0))
            if (round >= 0 && us < best[i]) {
                best[i] = us
            }
        }
        sink += y[0]
    }
    tune_detail("  checksum {sink}\n")
    return report("dot_q8q8_laneq4x4", names, best, ok)
}

// fallback: the tuner rail drives this half explicitly (DAS_TUNE_MODE=tune) — the auto
// policy must not fire a nested tuner off this program's own missing per-app sidecar
[export, tune_policy(missing = "fallback")]
def main {
    FAST_MODE = tuner_fast_requested()
    if (FAST_MODE) {
        SCREEN_ROUNDS = 4
        ROUNDS = 20
    }
    let affinity_inherited = has_env_variable("DAS_JOBQUE_AFFINITY")
    if (!affinity_inherited) {
        set_jobque_affinity(2)   // JobQue pins its creator (this measurement thread) to physical CPU 0
    }
    create_job_que()   // child process exit restores affinity; runtime snapshot also reads its lane count
    let affinity_desc = affinity_inherited ? "DAS_JOBQUE_AFFINITY={get_env_variable("DAS_JOBQUE_AFFINITY")}" : "hard CPU0 (tuner default)"
    tune_detail("kernel tuner: {FAST_MODE ? "FAST" : "default"}, N={N}, {SCREEN_ROUNDS} full-grid screening rounds, finalists up to {ROUNDS} rounds x {REPS} reps, measurement affinity {affinity_desc}\n\n")
    tune_progress_plan("dasllama_kernels", TUNED_KERNEL_COUNT)

    // add / mul / copy share the (var d, s) signature — one bench, per-kernel data + reference.
    var add_d0 : array<float>
    var add_s : array<float>
    add_d0 |> resize(int(N))
    add_s |> resize(int(N))
    for (i in range(int(N))) {
        add_d0[i] = 0.5f + 0.3f * sin(float(i) * 0.05f)
        add_s[i] = cos(float(i) * 0.03f)
    }
    var mul_d0 : array<float>
    var mul_s : array<float>
    mul_d0 |> resize(int(N))
    mul_s |> resize(int(N))
    for (i in range(int(N))) {
        mul_d0[i] = 0.5f + 0.3f * sin(float(i) * 0.05f)
        mul_s[i] = 1.0f + 0.0005f * sin(float(i) * 0.02f)   // near 1.0: s^REPS stays finite/normal
    }
    var cpy_d0 : array<float>
    var cpy_s : array<float>
    cpy_d0 |> resize(int(N))
    cpy_s |> resize(int(N))
    for (i in range(int(N))) {
        cpy_d0[i] = 0.0f
        cpy_s[i] = cos(float(i) * 0.03f)
    }

    let add_vs <- add_inplace_template_variants()
    let mul_vs <- mul_inplace_template_variants()
    let cpy_vs <- copy_floats_template_variants()

    var kernel_t0 = kernel_begin("dot")
    let dotw = bench_dot()
    kernel_done("dot", kernel_t0)
    kernel_t0 = kernel_begin("axpy")
    let axpyw = bench_axpy()
    kernel_done("axpy", kernel_t0)
    kernel_t0 = kernel_begin("dot_f16")
    let dotf16w = bench_dot_f16()
    kernel_done("dot_f16", kernel_t0)
    kernel_t0 = kernel_begin("axpy_f16")
    let axpyf16w = bench_axpy_f16()
    kernel_done("axpy_f16", kernel_t0)
    kernel_t0 = kernel_begin("cvt_f32_to_f16")
    let cvt16w = bench_cvt_f32_to_f16()
    kernel_done("cvt_f32_to_f16", kernel_t0)
    kernel_t0 = kernel_begin("cvt_f16_to_f32")
    let cvt32w = bench_cvt_f16_to_f32()
    kernel_done("cvt_f16_to_f32", kernel_t0)
    kernel_t0 = kernel_begin("add_inplace")
    let addw = bench_binary("add_inplace", add_vs, add_d0, add_s, @@(a, b : float) : double => double(a) + double(b))
    kernel_done("add_inplace", kernel_t0)
    kernel_t0 = kernel_begin("mul_inplace")
    let mulw = bench_binary("mul_inplace", mul_vs, mul_d0, mul_s, @@(a, b : float) : double => double(a) * double(b))
    kernel_done("mul_inplace", kernel_t0)
    kernel_t0 = kernel_begin("scale_inplace")
    let scalew = bench_scale()
    kernel_done("scale_inplace", kernel_t0)
    kernel_t0 = kernel_begin("copy_floats")
    let copyw = bench_binary("copy_floats", cpy_vs, cpy_d0, cpy_s, @@(_a, b : float) : double => double(b))
    kernel_done("copy_floats", kernel_t0)
    kernel_t0 = kernel_begin("softmax")
    let softmaxw = bench_softmax()
    kernel_done("softmax", kernel_t0)
    kernel_t0 = kernel_begin("rmsnorm")
    let rmsw = bench_rmsnorm()
    kernel_done("rmsnorm", kernel_t0)
    kernel_t0 = kernel_begin("dot_q4")
    let dq4w = bench_dot_q4()
    kernel_done("dot_q4", kernel_t0)
    kernel_t0 = kernel_begin("dot_q8q8")
    let dq8w = bench_dot_q8q8()
    kernel_done("dot_q8q8", kernel_t0)
    kernel_t0 = kernel_begin("dot_q8kv")
    let dq8kvw = bench_dot_q8kv()
    kernel_done("dot_q8kv", kernel_t0)
    kernel_t0 = kernel_begin("dot_q8q8kv")
    let dq8q8kvw = bench_dot_q8q8kv()
    kernel_done("dot_q8q8kv", kernel_t0)
    kernel_t0 = kernel_begin("axpy_q8kv")
    let axq8kvw = bench_axpy_q8kv()
    kernel_done("axpy_q8kv", kernel_t0)
    kernel_t0 = kernel_begin("cvt_q8kv_to_f32")
    let cvtq8kvw = bench_cvt_q8kv_to_f32()
    kernel_done("cvt_q8kv_to_f32", kernel_t0)
    kernel_t0 = kernel_begin("quantize_q8kv_row")
    let quantq8kvw = bench_quantize_q8kv_row()
    kernel_done("quantize_q8kv_row", kernel_t0)
    kernel_t0 = kernel_begin("dot_mx4q8")
    let dmx4w = bench_dot_mx4q8()
    kernel_done("dot_mx4q8", kernel_t0)
    kernel_t0 = kernel_begin("quantize_q8_0_into_ptr")
    let quantw = bench_quantize()
    kernel_done("quantize_q8_0_into_ptr", kernel_t0)
    kernel_t0 = kernel_begin("quantize_q8_0_bs_into_ptr")
    let quantbsw = bench_quantize_bs()
    kernel_done("quantize_q8_0_bs_into_ptr", kernel_t0)
    kernel_t0 = kernel_begin("rope_scaled_neox_tab")
    let ropew = bench_rope_tab()
    kernel_done("rope_scaled_neox_tab", kernel_t0)
    kernel_t0 = kernel_begin("gemm_f32_uk_4x16")
    let gemmw = bench_gemm_tile()
    kernel_done("gemm_f32_uk_4x16", kernel_t0)
    kernel_t0 = kernel_begin("dot_q8q8_laneq4x4")
    let laneqw = bench_laneq4x4()   // last: it pins the repack backend (sticks until process exit)
    kernel_done("dot_q8q8_laneq4x4", kernel_t0)

    // Summary: which kernels the tuner re-tuned away from THEIR OWN shipped fallback (the
    // [tuned(fallback=...)] perm; bare [tuned] ships the vec8_u2 majority = BASELINE).
    tune_progress_finish()
    print("\n===== summary (winner per kernel) =====\n")
    var changed = 0
    for (r in g_results) {
        let note = (r.winner == "" || r.winner == r.fallback) ? "" : "  (shipped fallback {r.fallback})"
        print("  {r.kernel}: {empty(r.winner) ? "skipped" : r.winner}{note}\n")
        if (r.winner != "" && r.winner != r.fallback) {
            changed++
        }
    }
    tune_detail(changed == 0
        ? "\nevery kernel keeps its shipped fallback — the tuner found nothing new on this box.\n"
        : "\n{changed} kernel(s) beat their shipped fallback on this box.\n")
    // the one count still written by hand; say so rather than let the bar quietly misreport
    if (length(g_results) != TUNED_KERNEL_COUNT) {
        print("note: TUNED_KERNEL_COUNT is {TUNED_KERNEL_COUNT} but {length(g_results)} kernels ran — the progress bar was scaled wrong; update the constant\n")
    }

    // softmax_sink shares softmax's loop shape exactly (one extra scalar joins the reduction), so it
    // mirrors softmax's winner instead of paying its own sweep.
    // The "runtime" section snapshots the CURRENT runtime knobs (defaults unless this process changed
    // them) so the written profile is complete and hand-editable: load_model applies it via
    // apply_box_profile_runtime. TB/L2-budget refinement stays manual (tune_tb.das shows the raw
    // cliff; edit the file under the knee); "threads" is advisory — set DAS_JOBQUE_THREADS to change it.
    var runtime = JV((q8_token_block = get_q8_token_block(), q8_l2_budget = get_q8_l2_budget(),
        matmul_min_chunk_rows = get_matmul_min_chunk_rows(),
        matmul_min_chunk_rows_gemv = get_matmul_min_chunk_rows_gemv(),
        target_chunk_work = get_target_chunk_work(), gemv_lane_cap = get_gemv_lane_cap(),
        batch_lane_cap = get_batch_lane_cap(), batch_grid_2d = get_batch_grid_2d(),
        dispatch_worker_limit = get_dispatch_worker_limit(),
        team_rank_gate = get_team_rank_gate(),   // -1 = unset (apply skips negatives; env/que default rules)
        attn_par_threshold = get_attn_par_threshold(),
        requant_par_threshold = get_requant_par_threshold(), norm_par_threshold = get_norm_par_threshold(),
        rope_par_threshold = get_rope_par_threshold(), kv_store_par_threshold = get_kv_store_par_threshold(),
        act_par_threshold = get_act_par_threshold(), q8_chunks_per_job = get_q8_chunks_per_job(),
        q4_chunks_per_job = get_q4_chunks_per_job(), q8_batch_chunks_per_job = get_q8_batch_chunks_per_job(),
        jobque_spin_us = get_jobque_spin_us(), jobque_join_poll = get_jobque_join_poll(),
        threads = get_total_hw_jobs()))
    var winners <- {
        "dot" => dotw, "axpy" => axpyw, "dot_f16" => dotf16w, "axpy_f16" => axpyf16w,
        "cvt_f32_to_f16" => cvt16w, "cvt_f16_to_f32" => cvt32w, "add_inplace" => addw,
        "mul_inplace" => mulw, "scale_inplace" => scalew, "copy_floats" => copyw,
        "softmax" => softmaxw, "softmax_sink" => softmaxw, "rmsnorm" => rmsw,
        "dot_q4" => dq4w, "dot_q8q8" => dq8w,
        "dot_q8kv" => dq8kvw, "dot_q8q8kv" => dq8q8kvw, "axpy_q8kv" => axq8kvw,
        "cvt_q8kv_to_f32" => cvtq8kvw, "quantize_q8kv_row" => quantq8kvw,
        "dot_mx4q8" => dmx4w, "quantize_q8_0_into_ptr" => quantw, "quantize_q8_0_bs_into_ptr" => quantbsw,
        "rope_scaled_neox_tab" => ropew, "gemm_f32_uk_4x16" => gemmw, "dot_q8q8_laneq4x4" => laneqw,
        // recorded-not-measured: [tuned] kernels this harness doesn't sweep yet keep their
        // SHIPPED fallback as an explicit entry — the scope completeness check demands every
        // non-pinned [tuned] kernel, and a missing key would re-tune on every start. Adding a
        // sweep for one of these replaces its literal with the measured winner.
        "dot_bf16" => "vec8_u2", "add_scale_inplace" => "vec8_u2",
        "axpy_tq4kv" => "vec8_u2", "cvt_tq4kv_to_f32" => "vec8_u2",
        "dot_q8tq4kv" => "vec16", "quantize_tq4kv_row" => "plain",
        "dot_q8q8_f16s" => "vec16", "dot_q51e" => "vec16"
    }
    // upsert into the app sidecar (same resolution [tuned] reads from): the loop-hint winners
    // join the "kernels" section next to the [tune] generator perms, "runtime" is replaced —
    // this tuner owns it — and everything else (other scopes' keys) survives
    let ppath = box_profile_path()
    if (tune_sidecar_merge(winners, runtime)) {
        print("wrote {ppath}\n")
        // the sidecar body is what the display exists to replace — verbose only
        if (tune_verbosity() == "verbose") {
            print("{fread(ppath)}\n")
        }
    } else {
        print("FAILED to write {ppath} — if the app dir is read-only, set DAS_TUNE_MANIFEST to a writable path\n")
    }
    delete winners
}
