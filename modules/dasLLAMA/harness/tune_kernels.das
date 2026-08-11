options gen2
options stack = 524288

// Per-box kernel tuner — sweeps the FULL grid (20 perms) for every [tuned] kernel and writes
// the winning permutation of each to box_profile.json. [dasllama_grid] emits, cross-module, each template's
// <name>__<suffix> variant clones plus a <name>_template_variants() fn-ptr registry; the bench runs
// the registry INTERLEAVED under the JIT (loop hints only bite there — see dasllama_tune),
// correctness-gates every variant vs an f64 reference (exact equality for the requant), and reports
// each variant's time as a DELTA vs that kernel's SHIPPED fallback perm so a "win" that is really
// noise is obvious.
//
// Run:  bin/daslang -jit modules/dasLLAMA/harness/tune_kernels.das   (append `-- --tune-paranoid`
// for the 3x budget), then rebuild/rerun the model with -jit to pick up box_profile.json (it
// re-keys the JIT DLL cache). There is no fast RACE mode — a cheap race is a systematic failure
// to measure; the debugging concession is accepting an existing sidecar (daspkg --quick).

require dasllama/dasllama_math   // side-effect: loads the *_template kernels for the cross-module scan
require dasllama/dasllama_math_default   // dot_q8q8_template lives here
require dasllama/dasllama_math_aarch64_neon   // laneq4x4 template (the repack backend "arm64-gen" registers via dasllama_common's math_gen require)
require dasllama/dasllama_kv_codec   // nolint:STYLE030 — side-effect: the KV-codec *_template kernels for the cross-module scan
require dasllama/dasllama_convert    // side-effect: the f16 row-convert templates for the cross-module scan
require dasllama/dasllama_common // the runtime-knob getters (the profile's "runtime" section snapshots them)
require dasllama/dasllama_tune   // [dasllama_grid] emitter
require llvm/daslib/llvm_tune    // tune_sidecar_merge — the app-sidecar upsert writer
require ?das_metal dasllama/dasllama_metal_prefill  // metal_tensor_race (Apple static_if half)
require ?das_metal dasllama/dasllama_metal_kernels  // metal_tensor_race_decode (Apple static_if half)
require tuner_cli               // --tune-paranoid, shared with the other half
require tuner_noise             // the noise gate + the measured floor, shared with the other half
require llvm/daslib/aarch64_neon // sdot4_laneq (referenced by the cloned laneq template bodies)
require llvm/daslib/f16_cvt      // f16 references + the cloned f16-kernel template bodies
require math                     // mad (referenced by the cloned template bodies)
require daslib/json
require daslib/json_boost
require daslib/fio
require daslib/strings_boost   // nolint:STYLE030,LINT019 — join for the metal_tensor crown list, only inside the das_metal static_if half
require dasllama/dasllama_env

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

// Pin every race stream to a 1MB-aligned base inside a fat (>=4MB) backing allocation. The M4
// mint flips traced to per-allocation placement: L1 way size == the 16KB page on Apple
// silicon, so the set geometry between a bench's streams is decided entirely by the
// allocator's dice — pinning all bases to the same 1MB boundary makes the geometry identical
// on every race, every run, every box (the deterministic degenerate case, not a lucky one).
let PROBE_ALIGN = 1024l * 1024l           // every stream base lands on a 1MB boundary
let PROBE_BACKING = 4l * 1024l * 1024l    // fat backing so the alignment always fits

def private aligned_f32(@scratch var arr : array<float>; n : int64) : float? {
    arr |> resize(max((n * 4l + PROBE_ALIGN) / 4l, PROBE_BACKING / 4l))
    var p = unsafe(addr(arr[0]))
    let mis = int64(unsafe(reinterpret<uint64>(p)) & uint64(PROBE_ALIGN - 1l))
    if (mis != 0l) {
        unsafe {
            p = p + (PROBE_ALIGN - mis) / 4l
        }
    }
    return p
}

let SCREEN_ROUNDS = 20
var private ROUNDS = 80   // paranoid triples this
let MIN_FINALISTS = 4
let FINALIST_MARGIN = 1.05lf
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
// per-candidate finalist-phase samples for the CURRENT kernel — reset at kernel_begin. The
// median of these ranks the finalists in report(); best-of stays min-over-all-rounds (the
// screening rule still runs on it) and is printed alongside as the per-kernel noise witness.
var private g_samples : array<array<double>>
// per-kernel race tables for the sidecar "race" section — "how much did it flip/win by" must
// be answerable from the file long after the scrollback is gone
var private g_race : table<string; JsonValue?>
// the validation phase: a heavy subset re-races twice after the sweep; the verdict is
// tune_validate_verdict over the windows — same-window ORDER only, drift reported never failed
var private g_validating = false
var private g_validate_expect : table<string; string>
var private g_validate_med : table<string; double>
var private g_validate_rows : table<string; array<TuneValidateRow>>
var private VALIDATE_DRIFT_PCT = 5.0lf   // stationarity NOTE threshold (never fails); paranoid tightens in main
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
    for (bj, okj in best, ok) {
        if (okj) {
            if (bj < leader) {
                leader = bj
            }
            if (bj < best[i]) {
                faster ++
            }
        }
    }
    return faster < min_finalists || best[i] <= leader * margin
}

def private kernel_begin(name : string) : int64 {
    interrupt_gate(name)   // between families only — the race in flight always completes
    delete g_samples
    tune_progress_kernel_begin(name, ROUNDS)
    return ref_time_ticks()
}

// one timing sample: best[] keeps min-over-all-rounds (screening ranks on it); finalist-phase
// rounds also collect into g_samples for the median ranking
def private record_sample(round, i : int; us : double; var best : array<double>) {
    if (round < 0) {
        return
    }
    if (us < best[i]) {
        best[i] = us
    }
    return if (round < SCREEN_ROUNDS)
    if (length(g_samples) <= i) {
        g_samples |> resize(i + 1)
    }
    g_samples[i] |> push(us)
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
    let phase = round < SCREEN_ROUNDS ? "screening" : "finalists"
    tune_progress_kernel_step(round + 1, phase, live)
}

def private tune_candidate_active(round : int; best : array<double>; ok : array<bool>; i : int) : bool {
    if (round < SCREEN_ROUNDS) {
        return true
    }
    return screening_keep(best, ok, i, MIN_FINALISTS, FINALIST_MARGIN)
}

// Print each passing variant's best time as a % delta vs the kernel's SHIPPED baseline (its
// [tuned] fallback perm — vec8_u2 for most, but e.g. dot_q8q8 ships vec16), mark the winner, and
// return the winning suffix (or "" if nothing passed). A win of only a couple % is flagged but is
// likely noise — vec16 was a real 2% loss, so the bar for "genuinely new" is a repeatable margin.
def private shipped_fallback(kernel : string) : string {
    if (empty(g_shipped)) {
        g_shipped <- dasllama_tuned_fallbacks()   // nolint:PERF030 — first and only assignment, under the empty() guard: nothing to release
    }
    return g_shipped?[kernel] ?? BASELINE
}

// deterministic tie-break: every candidate whose median sits within the noise floor of the
// best is the SAME measurement — prefer the shipped baseline, else the first tied row in grid
// order, so back-to-back mints reproduce instead of flipping between statistical twins
// (measured: two quiet fast mints flipped 8/34 winners before this)
def private tie_break(med : array<double>; ok : array<bool>; bi, winner : int; need : double) : int {
    let cut = med[winner] * (1.0lf + need / 100.0lf)
    var pick = -1
    for (i in range(length(med))) {
        if (ok[i] && med[i] <= cut) {
            if (i == bi) return i
            if (pick < 0) {
                pick = i
            }
        }
    }
    return pick
}

// the kernel's race table: every passing candidate's median + best in us, the winner, the
// shipped fallback it was judged against, and the noise floor the verdict was made under
def private race_row(names : array<string>; med, best : array<double>; ok : array<bool>;
                     wname, baseline : string; floor : double) : JsonValue? {
    var rows : table<string; JsonValue?>
    var rej = ""
    for (nm, okv, m, bst in names, ok, med, best) {
        if (okv) {
            rows[nm] = JV((med_us = m, best_us = bst))
        } else {
            rej = empty(rej) ? nm : "{rej}; {nm}"
        }
    }
    var obj <- {
        "winner" => JV(wname), "fallback" => JV(baseline),
        "floor_pct" => JV(floor), "rows" => JV(rows)
    }
    if (!empty(rej)) {
        obj["rejected"] = JV(rej)
    }
    return JV(obj)
}

def private print_race(kernel : string; names : array<string>; med, best : array<double>;
                       ok : array<bool>; winner : int; base : double; baseline : string) {
    tune_detail("--- {kernel} ---\n")
    for (i in range(length(names))) {
        if (!ok[i]) {
            tune_detail("  {names[i]}: REJECTED\n")
            continue
        }
        let delta = base > 0.0lf ? (med[i] - base) / base * 100.0lf : 0.0lf
        let mark = i == winner ? "  <== winner" : ""
        tune_detail("  {names[i]}: med {med[i]:0.1f} us (best {best[i]:0.1f})  ({delta:0.2f}% vs {baseline}){mark}\n")
    }
}

def report(kernel : string; names : array<string>; best : array<double>; ok : array<bool>) : string {
    let baseline = shipped_fallback(kernel)
    // rank by MEDIAN of finalist rounds: min-over-rounds is a lottery on a non-silent box.
    // Rows screened out before the finalist phase fall back to their best-of — they cannot
    // win, and their line stays honest about what was measured.
    var inscope med : array<double>
    med |> resize(length(names))
    for (i in range(length(names))) {
        med[i] = (length(g_samples) > i && !empty(g_samples[i])) ? tune_median(g_samples[i]) : best[i]
    }
    var bi = -1
    for (i, nm in count(), names) {
        if (nm == baseline) {
            bi = i
        }
    }
    let base = (bi >= 0 && ok[bi]) ? med[bi] : -1.0lf
    var winner = -1
    for (i in range(length(names))) {
        if (ok[i] && (winner < 0 || med[i] < med[winner])) {
            winner = i
        }
    }
    let need = max(0.5lf, noise_floor_pct())
    if (winner >= 0) {
        winner = tie_break(med, ok, bi, winner, need)
    }
    print_race(kernel, names, med, best, ok, winner, base, baseline)
    if (winner < 0) {
        tune_progress_note("  {kernel}: no passing variant\n")
        tune_progress_kernel_end(kernel, "", "rejected")
        if (g_validating) {
            // only kernels with a recorded winner have anything to reproduce — a kernel that
            // legitimately shipped winner="" must not render a false "rejected" verdict
            if (!empty(g_validate_expect?[kernel] ?? "")) {
                g_validate_rows[kernel] |> push(TuneValidateRow(rejected = true))
            }
            tune_progress_note("VALIDATE {kernel}: no passing variant in the re-race\n")
        } else {
            g_results |> emplace(KernelResult(kernel = kernel, winner = "", fallback = baseline))
            g_race[kernel] = race_row(names, med, best, ok, "", baseline, noise_floor_pct())
        }
        delete med
        return ""
    }
    // a winner's best-of sitting far under its own median = interference landed on this
    // kernel's rounds — the per-kernel noise flag (informational; the median already absorbed it)
    let div = med[winner] > 0.0lf ? (med[winner] - best[winner]) / med[winner] * 100.0lf : 0.0lf
    let divThr = max(3.0lf * noise_floor_pct(), 2.0lf)
    if (div > divThr) {
        tune_progress_note("  {kernel}: NOISY - winner's best-of runs {div:0.1f}% under its median (flag at {divThr:0.1f}%)\n")
    }
    var wname = names[winner]
    // a winner inside the measured noise floor HOLDS the shipped baseline: flipping the perm
    // re-keys the JIT and reorders float math for a margin the box cannot even measure —
    // deterministic beats lottery
    let margin = base > 0.0lf ? (base - med[winner]) / base * 100.0lf : 0.0lf
    if (wname != baseline && base > 0.0lf && margin < need) {
        wname = baseline
        tune_detail("  => {kernel}: {baseline} holds ({names[winner]} within the noise floor: {margin:0.2f}% < {need:0.2f}%)\n")
        tune_progress_kernel_end(kernel, wname, "holds")
    } elif (wname == baseline) {
        tune_detail("  => {kernel}: {baseline} holds (the shipped default is already best)\n")
        tune_progress_kernel_end(kernel, wname, "holds")
    } else {
        tune_detail("  => {kernel}: {wname} beats {baseline} by {margin:0.2f}%  <<< the tuner found something\n")
        tune_progress_kernel_end(kernel, wname, "beats")
    }
    if (g_validating) {
        validate_check(kernel, names, med, ok, wname, need)
    } else {
        record_kernel(kernel, names, med, best, ok, wname, baseline, winner)
    }
    delete med
    return wname
}

// the record step: the summary row, the race table, and the validation expectations (the
// shipped winner's name + its median — what the re-race must reproduce)
def private record_kernel(kernel : string; names : array<string>; med, best : array<double>;
                          ok : array<bool>; wname, baseline : string; winner : int) {
    g_results |> emplace(KernelResult(kernel = kernel, winner = wname, fallback = baseline))
    g_race[kernel] = race_row(names, med, best, ok, wname, baseline, noise_floor_pct())
    var wi = winner
    for (i, nm in count(), names) {
        if (nm == wname) {
            wi = i
        }
    }
    g_validate_expect[kernel] = wname
    g_validate_med[kernel] = med[wi]
}

// one validation window: fill a TuneValidateRow for the recorded winner. No verdict here —
// judging needs both windows and happens in run_validation
def private validate_check(kernel : string; names : array<string>; med : array<double>;
                           ok : array<bool>; wname : string; need : double) {
    let expect0 = g_validate_expect?[kernel] ?? ""
    return if (empty(expect0))
    var ei = -1
    var wi = -1
    for (i, nm in count(), names) {
        if (nm == expect0) {
            ei = i
        }
        if (nm == wname) {
            wi = i
        }
    }
    var row = TuneValidateRow()
    if (ei < 0 || !ok[ei]) {
        row.rejected = true
        tune_progress_note("VALIDATE {kernel}: recorded winner {expect0} rejected in this re-race\n")
    } else {
        let recorded = g_validate_med?[kernel] ?? med[ei]
        row.gap = (wi >= 0 && med[wi] > 0.0lf) ? (med[ei] - med[wi]) / med[wi] * 100.0lf : 0.0lf
        row.drift = recorded > 0.0lf ? abs(med[ei] - recorded) / recorded * 100.0lf : 0.0lf
        // the reproduce band sits ABOVE the tie floor: statistical twins drift up to ~1.2%
        // between QUIET back-to-back mints (measured), and a twin flip says nothing about box noise
        let band = max(need * 1.5lf, 1.0lf)
        if (wname != expect0 && row.gap > band) {
            row.challenger = wname
            tune_progress_note("VALIDATE {kernel}: {wname} beat the recorded {expect0} by {row.gap:0.2f}% this window (band {band:0.2f}%)\n")
        } else {
            let fliptxt = wname != expect0 ? " (inside-floor flip vs {expect0})" : ""
            tune_progress_note("VALIDATE {kernel}: window OK - winner {wname}{fliptxt}, level drift {row.drift:0.2f}%\n")
        }
    }
    g_validate_rows[kernel] |> push(row)
}

// re-race the heavy subset twice at the full budget; a winner that consistently fails to
// reproduce fails the whole mint. DAS_TUNE_NOISE_OVERRIDE mints through a failed verdict, stamped
def private run_validation() : bool {
    var subset <- [
        ("dot" => @@bench_dot), ("dot_q8q8" => @@bench_dot_q8q8), ("rmsnorm" => @@bench_rmsnorm),
        ("gemm_f32_uk_4x16" => @@bench_gemm_tile), ("quantize_q8_0_into_ptr" => @@bench_quantize)
    ]
    tune_progress_note("validation: re-racing {length(subset)} heavy kernels twice at full budget\n")
    g_validating = true
    for (_vrep in range(2)) {
        for (s in subset) {
            kernel_begin(s._0)
            invoke(s._1)
        }
    }
    g_validating = false
    delete subset
    var fails = 0
    var maxdrift = 0.0lf
    for (kernel, rows in keys(g_validate_rows), values(g_validate_rows)) {
        for (r in rows) {
            maxdrift = max(maxdrift, r.drift)
        }
        let verdict = tune_validate_verdict(rows)
        if (!empty(verdict)) {
            fails ++
            tune_progress_note("VALIDATE {kernel}: FAIL - {verdict}\n")
        }
    }
    // stationarity is reported, not judged: the level moving between windows says the box's
    // clock state changed, not that any winner is wrong — same-window order above decides
    if (maxdrift > VALIDATE_DRIFT_PCT) {
        tune_progress_note("validation: level drifted up to {maxdrift:0.2f}% between race and re-race (note at {VALIDATE_DRIFT_PCT:0.1f}%) - box is not stationary; order verdicts are same-window and unaffected\n")
    }
    tune_provenance_note("validation_max_drift_pct", "{maxdrift:0.2f}")
    if (fails > 0) {
        if (tune_noise_override()) {
            // the bar is still live here and the run continues — a raw print would be redrawn over
            tune_progress_note("VALIDATION: {fails} winner(s) did not reproduce - DAS_TUNE_NOISE_OVERRIDE is set, minting anyway; the sidecar will carry 'validation: overridden'\n")
            tune_provenance_note("validation", "overridden")
            tune_provenance_note("validation_fails", "{fails}")
            return true
        }
        tune_progress_finish()
        print("VALIDATION FAILED: {fails} winner(s) consistently did not reproduce - the winners are weather, not measurement\n")
        print("  escape hatch: DAS_TUNE_NOISE_OVERRIDE=1 mints anyway, stamped in provenance\n")
        return false
    }
    tune_provenance_note("validation", "ok")
    tune_progress_note("validation: every winner reproduced (same-window order, both re-races)\n")
    return true
}

def reset(var d : array<float>; d0 : array<float>) {
    for (di, d0i in d, d0) {
        di = d0i
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

// in-situ composite: matmul_at's row walk over a weight image sized past L2. The synthetic
// single-buffer hot loop flipped winners with allocation context on M4 (four refused mints —
// same micros, any pinning); the streaming walk measures the regime the kernel serves in,
// where per-allocation geometry washes into the signal that matters.
let DOT_WALK_ROWS = 8192l

def bench_dot() : string {
    var inscope w : array<float>
    var inscope x : array<float>
    var inscope o : array<float>
    w |> resize(DOT_WALK_ROWS * N)   // 128 MB — DRAM-streaming, like a decode GEMV
    x |> resize(N)
    o |> resize(DOT_WALK_ROWS)
    for (i in range64(DOT_WALK_ROWS * N)) {
        w[i] = sin(float(i) * 0.1f)
    }
    for (i in range64(N)) {
        x[i] = cos(float(i) * 0.07f)
    }
    let wp = unsafe(addr<float const?>(w[0]))
    let xp = unsafe(addr<float const?>(x[0]))
    var op = unsafe(addr(o[0]))

    let vs <- dot_template_variants()
    let nv = length(vs)
    var inscope names : array<string>
    var inscope ok : array<bool>
    var inscope best : array<double>
    names |> resize(nv)
    ok |> resize(nv)
    best |> resize(nv)
    for (i in range(nv)) {
        names[i] = vs[i]._0
        let f = vs[i]._1
        var exact = true
        for (r in range64(8l)) {   // spot rows vs the reference dot
            let rp = unsafe(wp + r * N)
            let got = double(f(rp, xp, N))
            let want = ref_dot(rp, xp, N)
            if (abs(got - want) / (abs(want) + 1.0lf) > TOL) {
                exact = false
            }
        }
        ok[i] = exact
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
            unsafe {
                for (r in range64(DOT_WALK_ROWS)) {
                    op[r] = f(wp + r * N, xp, N)
                }
            }
            sink += unsafe(op[0])
            record_sample(round, i, double(get_time_usec(t0)), best)
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
    var inscope d : array<float>
    d |> resize(N)
    var dp = unsafe(addr(d[0]))
    let sp = unsafe(addr(s[0]))

    var inscope names : array<string>
    var inscope ok : array<bool>
    var inscope best : array<double>
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
        for (j in range64(N)) {
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
            record_sample(round, i, double(get_time_usec(t0)), best)
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
    var inscope d0 : array<float>
    var inscope s : array<float>
    var inscope d : array<float>
    d0 |> resize(N)
    s |> resize(N)
    d |> resize(N)
    for (i in range64(N)) {
        d0[i] = 0.5f + 0.3f * sin(float(i) * 0.05f)
        s[i] = cos(float(i) * 0.03f)
    }
    var dp = unsafe(addr(d[0]))
    let sp = unsafe(addr(s[0]))

    var inscope names : array<string>
    var inscope ok : array<bool>
    var inscope best : array<double>
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
            record_sample(round, i, double(get_time_usec(t0)), best)
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
    var inscope a : array<float>
    var inscope b : array<uint16>
    a |> resize(N)
    b |> resize(N)
    for (i in range64(N)) {
        a[i] = sin(float(i) * 0.1f)
        b[i] = uint16(f32_to_f16(cos(float(i) * 0.07f)))
    }
    let pa = unsafe(addr(a[0]))
    let pb = unsafe(addr(b[0]))
    var want = 0.0lf
    for (j in range64(N)) {
        want += double(a[j]) * double(f16_to_f32(uint(b[j])))
    }

    let vs <- dot_f16_template_variants()
    let nv = length(vs)
    var inscope names : array<string>
    var inscope ok : array<bool>
    var inscope best : array<double>
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
            record_sample(round, i, double(get_time_usec(t0)), best)
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
    var inscope d0 : array<float>
    var inscope s : array<uint16>
    var inscope d : array<float>
    d0 |> resize(N)
    s |> resize(N)
    d |> resize(N)
    for (i in range64(N)) {
        d0[i] = 0.5f + 0.3f * sin(float(i) * 0.05f)
        s[i] = uint16(f32_to_f16(cos(float(i) * 0.03f)))
    }
    var dp = unsafe(addr(d[0]))
    let sp = unsafe(addr(s[0]))

    var inscope names : array<string>
    var inscope ok : array<bool>
    var inscope best : array<double>
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
            record_sample(round, i, double(get_time_usec(t0)), best)
        }
        sink += d[0]
    }
    tune_detail("  checksum {sink}\n")
    return report("axpy_f16", names, best, ok)
}

def bench_cvt_f32_to_f16() : string {
    let vs <- cvt_f32_to_f16_template_variants()
    let nv = length(vs)
    var inscope s : array<float>
    var inscope d : array<uint16>
    s |> resize(N)
    d |> resize(N)
    for (i in range64(N)) {
        s[i] = 300.0f * sin(float(i) * 0.11f)   // spans the normal f16 range
    }
    s[7] = 1.0e6f      // clamp pin
    s[13] = -1.0e6f
    var dp = unsafe(addr(d[0]))
    let sp = unsafe(addr(s[0]))

    var inscope names : array<string>
    var inscope ok : array<bool>
    var inscope best : array<double>
    names |> resize(nv)
    ok |> resize(nv)
    best |> resize(nv)
    for (i in range(nv)) {
        names[i] = vs[i]._0
        best[i] = 1.0e30lf
        let f = vs[i]._1
        invoke(f, dp, sp, N)
        var exact = true
        for (j in range64(N)) {
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
            record_sample(round, i, double(get_time_usec(t0)), best)
        }
    }
    return report("cvt_f32_to_f16", names, best, ok)
}

def bench_cvt_f16_to_f32() : string {
    let vs <- cvt_f16_to_f32_template_variants()
    let nv = length(vs)
    var inscope s : array<uint16>
    var inscope d : array<float>
    s |> resize(N)
    d |> resize(N)
    for (i in range64(N)) {
        s[i] = uint16(f32_to_f16(300.0f * cos(float(i) * 0.09f)))
    }
    var dp = unsafe(addr(d[0]))
    let sp = unsafe(addr(s[0]))

    var inscope names : array<string>
    var inscope ok : array<bool>
    var inscope best : array<double>
    names |> resize(nv)
    ok |> resize(nv)
    best |> resize(nv)
    for (i in range(nv)) {
        names[i] = vs[i]._0
        best[i] = 1.0e30lf
        let f = vs[i]._1
        invoke(f, dp, sp, N)
        var exact = true
        for (j in range64(N)) {
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
            record_sample(round, i, double(get_time_usec(t0)), best)
        }
    }
    return report("cvt_f16_to_f32", names, best, ok)
}

// ===== scale: (var d, scale)->void =====

def bench_scale() : string {
    let vs <- scale_inplace_template_variants()
    let nv = length(vs)
    let scale = 1.0001f   // near 1.0 so scale^REPS stays in normal float range with per-block reset
    var inscope d0 : array<float>
    var inscope d : array<float>
    d0 |> resize(N)
    d |> resize(N)
    for (i in range64(N)) {
        d0[i] = 0.5f + 0.3f * sin(float(i) * 0.05f)
    }
    var dp = unsafe(addr(d[0]))

    var inscope names : array<string>
    var inscope ok : array<bool>
    var inscope best : array<double>
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
        for (j in range64(N)) {
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
            record_sample(round, i, double(get_time_usec(t0)), best)
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
    var inscope d0 : array<float>
    var inscope d : array<float>
    d0 |> resize(N)
    d |> resize(N)
    for (i in range64(N)) {
        d0[i] = 4.0f * sin(float(i) * 0.11f)   // scores in [-4,4], a realistic logit spread
    }
    var dp = unsafe(addr(d[0]))
    var inscope want : array<double>
    want |> resize(N)
    var mx = -1.0e30lf
    for (i in range64(N)) {
        mx = max(mx, double(d0[i]))
    }
    var wsum = 0.0lf
    for (i in range64(N)) {
        let e = exp(double(d0[i]) - mx)
        want[i] = e
        wsum += e
    }
    for (i in range64(N)) {
        want[i] /= wsum
    }

    var inscope names : array<string>
    var inscope ok : array<bool>
    var inscope best : array<double>
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
        for (j in range64(N)) {
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
            record_sample(round, i, double(get_time_usec(t0)), best)
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
    var inscope x : array<float>
    var inscope w : array<float>
    var inscope o : array<float>
    var xp = aligned_f32(x, N)
    var wp = aligned_f32(w, N)
    var op = aligned_f32(o, N)
    unsafe {
        for (i in range64(N)) {
            xp[i] = sin(float(i) * 0.1f)
            wp[i] = 1.0f + 0.2f * cos(float(i) * 0.07f)
        }
    }
    var ss = 0.0lf
    for (i in range64(N)) {
        ss += double(unsafe(xp[i])) * double(unsafe(xp[i]))
    }
    ss = 1.0lf / sqrt(ss / double(N) + double(eps))

    var inscope names : array<string>
    var inscope ok : array<bool>
    var inscope best : array<double>
    names |> resize(nv)
    ok |> resize(nv)
    best |> resize(nv)
    for (i in range(nv)) {
        names[i] = vs[i]._0
        best[i] = 1.0e30lf
        let f = vs[i]._1
        invoke(f, op, xp, wp, N, eps)
        var maxrel = 0.0lf
        for (j in range64(N)) {
            let want = double(unsafe(wp[j])) * double(unsafe(xp[j])) * ss
            let rel = abs(double(unsafe(op[j])) - want) / (abs(want) + 1.0lf)
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
            record_sample(round, i, double(get_time_usec(t0)), best)
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
    var inscope wf : array<float>
    var inscope x : array<float>
    wf |> resize(N)
    x |> resize(N)
    for (i in range64(N)) {
        wf[i] = sin(float(i) * 0.1f)
        x[i] = cos(float(i) * 0.07f)
    }
    let qt <- quantize_q4_0(wf, N)
    var inscope df : array<float>
    df |> resize(N)
    dequantize_q4_0(qt, df)
    var want = 0.0lf
    for (i in range64(N)) {
        want += double(df[i]) * double(x[i])
    }
    let wqp = unsafe(addr(qt.q[0]))
    let wsp = unsafe(addr(qt.scales[0]))
    let xp = unsafe(addr(x[0]))

    var inscope names : array<string>
    var inscope ok : array<bool>
    var inscope best : array<double>
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
            record_sample(round, i, double(get_time_usec(t0)), best)
        }
    }
    if (sink == 12345.678f) {
        print("")   // keep sink live
    }
    return report("dot_q4", names, best, ok)
}

// the q8 weight image a composite walks: DOT_WALK_ROWS rows quantized from the analytic fill
def private build_q8_walk(var wqi : array<int8>; var wsi : array<float>; nb : int64) {
    var inscope wrow : array<float>
    wrow |> resize(N)
    for (r in range64(DOT_WALK_ROWS)) {
        for (i in range64(N)) {
            wrow[i] = sin(float(r * N + i) * 0.1f)
        }
        var inscope qt <- quantize_q8_0(wrow, N)
        for (i in range64(N)) {
            wqi[r * N + i] = qt.q[i]
        }
        for (bi in range64(nb)) {
            wsi[r * nb + bi] = qt.scales[bi]
        }
    }
    delete wrow
}

// in-situ composite: the q8 GEMV row walk (mm_at_q8's shape) over a quantized weight image
// sized past L2 — see bench_dot's rationale.
def bench_dot_q8q8() : string {
    let vs <- dot_q8q8_template_variants()
    let nv = length(vs)
    let nb = N / 32l
    var inscope wqi : array<int8>
    var inscope wsi : array<float>
    wqi |> resize(DOT_WALK_ROWS * N)    // 32 MB of q8 rows + 4 MB scales
    wsi |> resize(DOT_WALK_ROWS * nb)
    build_q8_walk(wqi, wsi, nb)
    var inscope xf : array<float>
    xf |> resize(N)
    for (i in range64(N)) {
        xf[i] = cos(float(i) * 0.07f)
    }
    let xq <- quantize_q8_0(xf, N)
    let wqp = unsafe(addr<int8 const?>(wqi[0]))
    let wsp = unsafe(addr<float const?>(wsi[0]))
    let xqp = unsafe(addr<int8 const?>(xq.q[0]))
    let xsp = unsafe(addr<float const?>(xq.scales[0]))
    var inscope o : array<float>
    o |> resize(DOT_WALK_ROWS)
    var op = unsafe(addr(o[0]))

    var inscope names : array<string>
    var inscope ok : array<bool>
    var inscope best : array<double>
    names |> resize(nv)
    ok |> resize(nv)
    best |> resize(nv)
    for (i in range(nv)) {
        names[i] = vs[i]._0
        best[i] = 1.0e30lf
        let f = vs[i]._1
        var exact = true
        for (r in range64(8l)) {   // spot rows vs the exact int block sums
            var want = 0.0lf
            for (bi in range64(nb)) {
                var iacc = 0
                for (k in range64(32l)) {
                    iacc += int(wqi[r * N + bi * 32l + k]) * int(xq.q[bi * 32l + k])
                }
                want += double(iacc) * double(wsi[r * nb + bi]) * double(xq.scales[bi])
            }
            let got = double(f(unsafe(wqp + r * N), unsafe(wsp + r * nb), xqp, xsp, N))
            if (abs(got - want) / (abs(want) + 1.0lf) > TOL) {
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
            unsafe {
                for (r in range64(DOT_WALK_ROWS)) {
                    op[r] = f(wqp + r * N, wsp + r * nb, xqp, xsp, N)
                }
            }
            sink += unsafe(op[0])
            record_sample(round, i, double(get_time_usec(t0)), best)
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
    row |> resize(n / 32l * 34l)
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
    var inscope a : array<float>
    var inscope wf : array<float>
    var inscope row : array<uint8>
    a |> resize(N)
    wf |> resize(N)
    for (i in range64(N)) {
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
    var inscope names : array<string>
    var inscope ok : array<bool>
    var inscope best : array<double>
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
            record_sample(round, i, double(get_time_usec(t0)), best)
        }
    }
    if (sink == 12345.678f) {
        print("")   // keep sink live
    }
    return report("dot_q8kv", names, best, ok)
}

def bench_dot_q8q8kv() : string {
    var inscope xf : array<float>
    var inscope wf : array<float>
    var inscope row : array<uint8>
    xf |> resize(N)
    wf |> resize(N)
    for (i in range64(N)) {
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
    var inscope names : array<string>
    var inscope ok : array<bool>
    var inscope best : array<double>
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
            record_sample(round, i, double(get_time_usec(t0)), best)
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
    var inscope d0 : array<float>
    var inscope wf : array<float>
    var inscope row : array<uint8>
    var inscope d : array<float>
    d0 |> resize(N)
    wf |> resize(N)
    d |> resize(N)
    for (i in range64(N)) {
        d0[i] = 0.5f + 0.3f * sin(float(i) * 0.05f)
        wf[i] = cos(float(i) * 0.03f)
    }
    build_q8kv_row(wf, N, row)
    var dp = unsafe(addr(d[0]))
    let sp = unsafe(addr(row[0]))

    var inscope names : array<string>
    var inscope ok : array<bool>
    var inscope best : array<double>
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
            record_sample(round, i, double(get_time_usec(t0)), best)
        }
        sink += d[0]
    }
    tune_detail("  checksum {sink}\n")
    return report("axpy_q8kv", names, best, ok)
}

def bench_cvt_q8kv_to_f32() : string {
    let vs <- cvt_q8kv_to_f32_template_variants()
    let nv = length(vs)
    var inscope wf : array<float>
    var inscope row : array<uint8>
    var inscope d : array<float>
    wf |> resize(N)
    d |> resize(N)
    for (i in range64(N)) {
        wf[i] = 300.0f * cos(float(i) * 0.09f)
    }
    build_q8kv_row(wf, N, row)
    var dp = unsafe(addr(d[0]))
    let sp = unsafe(addr(row[0]))

    var inscope names : array<string>
    var inscope ok : array<bool>
    var inscope best : array<double>
    names |> resize(nv)
    ok |> resize(nv)
    best |> resize(nv)
    for (i in range(nv)) {
        names[i] = vs[i]._0
        best[i] = 1.0e30lf
        for (j in range64(N)) {
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
            record_sample(round, i, double(get_time_usec(t0)), best)
        }
        sink += d[0]
    }
    tune_detail("  checksum {sink}\n")
    return report("cvt_q8kv_to_f32", names, best, ok)
}

def bench_quantize_q8kv_row() : string {
    let vs <- quantize_q8kv_row_template_variants()
    let nv = length(vs)
    var inscope src : array<float>
    var inscope ref_row : array<uint8>
    var inscope d : array<uint8>
    src |> resize(N)
    for (i in range64(N)) {
        src[i] = 300.0f * sin(float(i) * 0.11f)
    }
    build_q8kv_row(src, N, ref_row)   // the contract: byte-exact ggml quantize_row_q8_0
    d |> resize(length(ref_row))
    var dp = unsafe(addr(d[0]))
    let sp = unsafe(addr(src[0]))

    var inscope names : array<string>
    var inscope ok : array<bool>
    var inscope best : array<double>
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
        for (dj, rj in d, ref_row) {
            exact &&= dj == rj
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
            record_sample(round, i, double(get_time_usec(t0)), best)
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
    var inscope wn : array<uint8>
    var inscope we : array<uint8>
    wn |> resize(N / 2l)
    we |> resize(nb)
    for (i in range64(N / 2l)) {
        wn[i] = uint8((int(i) * 137 + 71) & 0xff)
    }
    for (i in range64(nb)) {
        we[i] = uint8(120 + (int(i) * 7) % 16)
    }
    var inscope xf : array<float>
    xf |> resize(N)
    for (i in range64(N)) {
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

    var inscope names : array<string>
    var inscope ok : array<bool>
    var inscope best : array<double>
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
            record_sample(round, i, double(get_time_usec(t0)), best)
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

// in-situ composite: requant_rows' batch walk — quantize every row of an activation image
// sized past L2, writing through the qoff/soff image contract. See bench_dot's rationale.
def bench_quantize() : string {
    let vs <- quantize_q8_0_into_ptr_template_variants()
    let nv = length(vs)
    let npos = DOT_WALK_ROWS / 2l   // 4096 rows × 16 KB = 64 MB read + 17 MB written per walk
    let nb = N / 32l
    var inscope src : array<float>
    src |> resize(npos * N)
    for (i in range64(npos * N)) {
        src[i] = 2.0f * sin(float(i) * 0.13f)
    }
    var inscope srcw : array<float>
    srcw |> resize(N)
    for (i in range64(N)) {   // row 0 is the byte-exact reference row
        srcw[i] = src[i]
    }
    let want <- quantize_q8_0(srcw, N)
    var inscope qdst : array<int8>
    var inscope sdst : array<float>
    qdst |> resize(npos * N)
    sdst |> resize(npos * nb)
    let srcp = unsafe(addr<float const?>(src[0]))
    var qp = unsafe(addr(qdst[0]))
    var sp = unsafe(addr(sdst[0]))

    var inscope names : array<string>
    var inscope ok : array<bool>
    var inscope best : array<double>
    names |> resize(nv)
    ok |> resize(nv)
    best |> resize(nv)
    for (i in range(nv)) {
        names[i] = vs[i]._0
        best[i] = 1.0e30lf
        let f = vs[i]._1
        invoke(f, srcp, N, qp, sp, 0l, 0l)
        var exact = true
        for (j in range64(N)) {
            if (qdst[j] != want.q[j]) {
                exact = false
            }
        }
        for (b in range64(nb)) {
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
            unsafe {
                for (p in range64(npos)) {
                    invoke(f, srcp + p * N, N, qp, sp, p * N, p * nb)
                }
            }
            record_sample(round, i, double(get_time_usec(t0)), best)
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
    var inscope src : array<float>
    src |> resize(N)
    for (i in range64(N)) {
        src[i] = 2.0f * sin(float(i) * 0.13f)
    }
    let want <- quantize_q8_0(src, N)
    var inscope wantbs : array<int>
    wantbs |> resize(N / 16l)
    for (h in range64(N / 16l)) {
        var bs = 0
        for (i in range64(16l)) {
            bs += int(want.q[h * 16l + i])
        }
        wantbs[h] = bs
    }
    var inscope qdst : array<int8>
    var inscope sdst : array<float>
    var inscope bdst : array<int>
    qdst |> resize(N)
    sdst |> resize(N / 32l)
    bdst |> resize(N / 16l)
    let srcp = unsafe(addr(src[0]))
    var qp = unsafe(addr(qdst[0]))
    var sp = unsafe(addr(sdst[0]))
    var bp = unsafe(addr(bdst[0]))

    var inscope names : array<string>
    var inscope ok : array<bool>
    var inscope best : array<double>
    names |> resize(nv)
    ok |> resize(nv)
    best |> resize(nv)
    for (i in range(nv)) {
        names[i] = vs[i]._0
        best[i] = 1.0e30lf
        let f = vs[i]._1
        invoke(f, srcp, N, qp, sp, bp, 0l, 0l, 0l)
        var exact = true
        for (j in range64(N)) {
            if (qdst[j] != want.q[j]) {
                exact = false
            }
        }
        for (b in range64(N / 32l)) {
            if (sdst[b] != want.scales[b]) {
                exact = false
            }
        }
        for (h in range64(N / 16l)) {
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
            record_sample(round, i, double(get_time_usec(t0)), best)
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
    var inscope v0 : array<float>
    var inscope v : array<float>
    v0 |> resize(N)
    v |> resize(N)
    for (i in range64(N)) {
        v0[i] = sin(float(i) * 0.1f)
    }
    var inscope cosr : array<float>
    var inscope sinr : array<float>
    cosr |> resize(half)
    sinr |> resize(half)
    for (j in range64(half)) {
        let ang = 37.0f / pow(10000.0f, float(2l * j) / float(head_size))
        cosr[j] = cos(ang)
        sinr[j] = sin(ang)
    }
    var vp = unsafe(addr(v[0]))
    let cp = unsafe(addr(cosr[0]))
    let sp = unsafe(addr(sinr[0]))

    var inscope names : array<string>
    var inscope ok : array<bool>
    var inscope best : array<double>
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
        for (h in range64(N / head_size)) {
            let base = h * head_size
            for (j in range64(half)) {
                let a = double(v0[base + j])
                let b = double(v0[base + j + half])
                let wc = double(cosr[j])
                let ws = double(sinr[j])
                let want0 = a * wc - b * ws
                let want1 = a * ws + b * wc
                let r0 = abs(double(v[base + j]) - want0) / (abs(want0) + 1.0lf)
                let r1 = abs(double(v[base + j + half]) - want1) / (abs(want1) + 1.0lf)
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
            record_sample(round, i, double(get_time_usec(t0)), best)
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
    var inscope a : array<float>
    var inscope b : array<float>
    var inscope c : array<float>
    var ap = aligned_f32(a, 4l * K)
    var bp = aligned_f32(b, K * NN)
    var cp = aligned_f32(c, 4l * NN)
    unsafe {
        for (i in range64(4l * K)) {
            ap[i] = sin(float(i) * 0.1f)
        }
        for (i in range64(K * NN)) {
            bp[i] = cos(float(i) * 0.07f)
        }
    }

    var inscope names : array<string>
    var inscope ok : array<bool>
    var inscope best : array<double>
    names |> resize(nv)
    ok |> resize(nv)
    best |> resize(nv)
    for (i in range(nv)) {
        names[i] = vs[i]._0
        best[i] = 1.0e30lf
        unsafe {
            for (j in range64(4l * NN)) {
                cp[j] = 0.0f
            }
        }
        let f = vs[i]._1
        invoke(f, cp, ap, bp, 0l, 0l, K, NN)
        var maxrel = 0.0lf
        for (r in range64(4l)) {
            for (col in range64(NN)) {
                var want = 0.0lf
                for (kk in range64(K)) {
                    want += double(unsafe(ap[r * K + kk])) * double(unsafe(bp[kk * NN + col]))
                }
                let rel = abs(double(unsafe(cp[r * NN + col])) - want) / (abs(want) + 1.0lf)
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
            for (j in range64(4l * NN)) {
                c[j] = 0.0f
            }
            let f = vs[i]._1
            let t0 = ref_time_ticks()
            for (_rep in range(REPS)) {
                invoke(f, cp, ap, bp, 0l, 0l, K, NN)
            }
            record_sample(round, i, double(get_time_usec(t0)), best)
        }
        sink += c[0]
    }
    tune_detail("  checksum {sink}\n")
    return report("gemm_f32_uk_4x16", names, best, ok)
}

// ===== dot_q8q8_laneq4x4: arm64+JIT only (the repack backend must be registered so the pin lands
// and the intrinsic lowers to hardware SDOT). Runs LAST — the pin sticks until process exit.

def bench_laneq4x4() : string { // nolint:STYLE038 — one bench: fixture, backend pin, gate and race are one coupled sequence
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
    var inscope wf : array<float>
    wf |> resize(d * n)
    for (i in range64(d * n)) {
        wf[i] = sin(float(i) * 0.1f)
    }
    var wq <- quantize_q8_0(wf, d * n)
    var inscope refq : array<int8>    // row-major copies for the reference (repack rewrites wq in place)
    var inscope refs : array<float>
    refq |> resize(d * n)
    refs |> resize(d * nb)
    for (i in range64(d * n)) {
        refq[i] = wq.q[i]
    }
    for (i in range64(d * nb)) {
        refs[i] = wq.scales[i]
    }
    pin_kernel_backend("arm64-gen")               // activates the repack fn the next call uses; the
                                                  // arm64 default stamp is mr=4 — byte-identical to
                                                  // the laneq4x4 tile's expected interleave (the
                                                  // correctness gate below catches any mr drift)
    repack_q8q8_weight(wq.q, wq.scales, 0l, n, d)
    var inscope xf : array<float>
    xf |> resize(ntok * n)
    for (i in range64(ntok * n)) {
        xf[i] = cos(float(i) * 0.07f)
    }
    var inscope xq : array<int8>
    var inscope xs : array<float>
    xq |> resize(ntok * n)
    xs |> resize(ntok * nb)
    quantize_q8_0_into(xf, ntok * n, xq, xs, 0l, 0l)
    var inscope y : array<float>
    y |> resize(ntok * d)
    var yp = unsafe(addr(y[0]))
    let wgp = unsafe(addr(wq.q[0]))
    let sgp = unsafe(addr(wq.scales[0]))
    let xqp = unsafe(addr(xq[0]))
    let xsp = unsafe(addr(xs[0]))

    var inscope names : array<string>
    var inscope ok : array<bool>
    var inscope best : array<double>
    names |> resize(nv)
    ok |> resize(nv)
    best |> resize(nv)
    for (i in range(nv)) {
        names[i] = vs[i]._0
        best[i] = 1.0e30lf
        let f = vs[i]._1
        invoke(f, yp, wgp, sgp, xqp, xsp, n, d, 0l, 0l)
        var maxrel = 0.0lf
        for (t in range64(ntok)) {
            for (r in range64(d)) {
                var want = 0.0lf
                for (bi in range64(nb)) {
                    var iacc = 0
                    for (k in range64(32l)) {
                        iacc += int(refq[r * n + bi * 32l + k]) * int(xq[t * n + bi * 32l + k])
                    }
                    want += double(iacc) * double(refs[r * nb + bi]) * double(xs[t * nb + bi])
                }
                let got = double(y[t * d + r])
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
            record_sample(round, i, double(get_time_usec(t0)), best)
        }
        sink += y[0]
    }
    tune_detail("  checksum {sink}\n")
    return report("dot_q8q8_laneq4x4", names, best, ok)
}

// fallback: the tuner rail drives this half explicitly (DAS_TUNE_MODE=tune) — the auto
// policy must not fire a nested tuner off this program's own missing per-app sidecar
[export, tune_policy(missing = "fallback")]
def main : int { // nolint:STYLE037,STYLE038 — flat one-bench-per-kernel rail; the noise gates are its checkpoints
    let paranoid = tuner_paranoid_requested()
    if (paranoid) {
        ROUNDS = 240   // ~3x the finalist budget; screening stays full-grid
        VALIDATE_DRIFT_PCT = 3.0lf
        noise_set_paranoid(true)
    }
    let affinity_inherited = is_some(g_env_core.jobque_affinity)
    if (!affinity_inherited) {
        set_jobque_affinity(2)   // JobQue pins its creator (this measurement thread) to physical CPU 0
    }
    create_job_que()   // child process exit restores affinity; runtime snapshot also reads its lane count
    let affinity_desc = (affinity_inherited
        ? "DAS_JOBQUE_AFFINITY={unwrap(g_env_core.jobque_affinity)}"
        : (get_platform_name() == "darwin" ? "QoS USER_INTERACTIVE (macOS has no core mask)" : "hard CPU0 (tuner default)"))
    let modeName = paranoid ? "PARANOID" : "default"
    tune_detail("kernel tuner: {modeName}, N={N}, {SCREEN_ROUNDS} full-grid screening rounds, finalists up to {ROUNDS} rounds x {REPS} reps, measurement affinity {affinity_desc}\n\n")
    // refuse before burning minutes measuring noise — a refusal costs seconds, a noisy sidecar
    // costs days of phantom regressions
    if (!noise_gate("start")) {
        return 1
    }
    // +10: the validation phase re-races 5 kernels twice
    tune_progress_plan("dasllama_kernels", TUNED_KERNEL_COUNT + 10)

    // add / mul / copy share the (var d, s) signature — one bench, per-kernel data + reference.
    var add_d0 : array<float>
    var add_s : array<float>
    add_d0 |> resize(N)
    add_s |> resize(N)
    for (i in range64(N)) {
        add_d0[i] = 0.5f + 0.3f * sin(float(i) * 0.05f)
        add_s[i] = cos(float(i) * 0.03f)
    }
    var mul_d0 : array<float>
    var mul_s : array<float>
    mul_d0 |> resize(N)
    mul_s |> resize(N)
    for (i in range64(N)) {
        mul_d0[i] = 0.5f + 0.3f * sin(float(i) * 0.05f)
        mul_s[i] = 1.0f + 0.0005f * sin(float(i) * 0.02f)   // near 1.0: s^REPS stays finite/normal
    }
    var cpy_d0 : array<float>
    var cpy_s : array<float>
    cpy_d0 |> resize(N)
    cpy_s |> resize(N)
    for (i in range64(N)) {
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
    if (!noise_gate("mid1")) {
        print("aborting mid-run - no sidecar written\n")
        return 1
    }
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
    if (!noise_gate("mid2")) {
        print("aborting mid-run - no sidecar written\n")
        return 1
    }
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

    // self-validation, inside the measurement window: the winners just picked must reproduce
    // in a fresh re-race or nothing ships
    if (!run_validation()) {
        print("aborting - no sidecar written\n")
        return 1
    }

    // the gate that closes the CPU measurement window: every sample the sidecar will carry was
    // raced between this probe and the start one — a noisy read here means NOTHING is written.
    // It runs BEFORE the Metal race so the bracket covers exactly the window that produced the
    // numbers (noise during the GPU phase cannot retroactively touch the CPU medians). The race
    // also blocks on the GPU, and a thread that blocked wakes cold (E-core placement, ~1.9x
    // medians until a couple of seconds of sustained load re-promotes it) — a post-race gate
    // would pay a mandatory settle just to measure through that. The Metal twin race guards
    // itself: interleaved base/twin A/B, so common-mode noise cancels.
    if (!noise_gate("end")) {
        print("aborting at the end-of-run gate - no sidecar written\n")
        return 1
    }

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
        ? "\nevery kernel keeps its shipped fallback - the tuner found nothing new on this box.\n"
        : "\n{changed} kernel(s) beat their shipped fallback on this box.\n")
    // the one count still written by hand; say so rather than let the bar quietly misreport
    if (length(g_results) != TUNED_KERNEL_COUNT) {
        print("note: TUNED_KERNEL_COUNT is {TUNED_KERNEL_COUNT} but {length(g_results)} kernels ran - the progress bar was scaled wrong; update the constant\n")
    }

    // Metal-4 tensor twin race: each tensor pso twin runs against its shipped simdgroup kernel
    // on this box's GPU; winners join the runtime "metal_tensor" crown list (empty = simdgroup
    // everywhere — the pre-M5 expectation). Skips cleanly with no Metal device.
    var metal_crowns : string   // nolint:LINT003,LINT019 — written by the das_metal arm
    static_if (typeinfo builtin_module_exists(das_metal)) {
        var mt_wins : array<string>
        var mt_ran = false
        var mt_all <- metal_tensor_race()
        var mt_dec <- metal_tensor_race_decode()
        mt_all |> reserve(length(mt_all) + length(mt_dec))
        for (r in mt_dec) {
            mt_all |> emplace(r)
        }
        delete mt_dec
        for (r in mt_all) {
            mt_ran = true
            let mt_note = empty(r.note) ? "" : " ({r.note})"
            tune_detail("METAL_TWIN {r.family}: base={r.base_ms}ms tensor={r.twin_ms}ms -> {r.winner}{mt_note}\n")
            if (r.winner == "tensor") {
                mt_wins |> push(r.family)
            }
        }
        if (!mt_ran) {
            tune_detail("METAL_TWIN: no Metal device — tensor race skipped\n")
        }
        metal_crowns = join(mt_wins, ",")
        delete mt_wins
        delete mt_all
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
        threads = get_total_hw_jobs() + 1, metal_tensor = metal_crowns))   // total lanes (workers + computing main), the DAS_JOBQUE_THREADS unit
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
    noise_provenance_stamp()
    let ppath = box_profile_path()
    var wrote = tune_sidecar_merge(winners, runtime)
    wrote = tune_sidecar_section_merge("race", g_race) && wrote
    if (wrote) {
        print("wrote {ppath}\n")
        // the sidecar body is what the display exists to replace — verbose only
        if (tune_verbosity() == "verbose") {
            print("{fread(ppath)}\n")
        }
    } else {
        print("FAILED to write {ppath} - if the app dir is read-only, set DAS_TUNE_MANIFEST to a writable path\n")
    }
    delete winners
    return wrote ? 0 : 1
}
