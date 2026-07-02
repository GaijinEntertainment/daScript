options gen2

// Per-box kernel tuner — sweeps the FULL grid (20 perms) for every tunable dasllama_math kernel
// (dot + axpy/add/mul/scale/copy) and writes the winning permutation of each to box_profile.json.
// [dasllama_grid] emits, cross-module, each template's <name>__<suffix> variant clones plus a
// <name>_template_variants() fn-ptr registry; the bench runs the registry INTERLEAVED under the JIT
// (loop hints only bite there — see dasllama_tune), correctness-gates every variant vs an f64
// reference, and reports each variant's time as a DELTA vs vec8_u2 (the hand default) so a "win" that
// is really noise is obvious. The experiment: does an empirical sweep beat vec8_u2 per-kernel here?
//
// Run:  bin/daslang -jit modules/dasLLAMA/harness/tune_kernels.das
//   then rebuild/rerun the model with -jit to pick up box_profile.json (it re-keys the JIT DLL cache).

require dasllama/dasllama_math   // nolint:STYLE030 — side-effect: loads the *_template kernels for the cross-module scan
require dasllama/dasllama_tune   // [dasllama_grid] emitter
require math                     // mad (referenced by the cloned template bodies)
require daslib/json
require daslib/json_boost
require daslib/fio

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

// Registry fn-ptr signatures (== function_to_type of each template).
typedef DotFn = function<(a : float const?; b : float const?; n : int64) : float>
typedef BinFn = function<(var d : float?; s : float const?; n : int64) : void>
typedef AxpyFn = function<(var d : float?; s : float const?; scale : float; n : int64) : void>
typedef ScaleFn = function<(var d : float?; scale : float; n : int64) : void>

let N = 4096l
let ROUNDS = 80
let REPS = 2000
let TOL = 1.0e-3lf
let BASELINE = "vec8_u2"   // the hand-tuned default every kernel ships with today

// ===== shared reporting =====

// Print each passing variant's best time as a % delta vs BASELINE, mark the winner, and return the
// winning suffix (or "" if nothing passed). A win that beats BASELINE by only a couple % is flagged
// but is likely noise — vec16 was a real 2% loss, so the bar for "genuinely new" is a repeatable margin.
def report(kernel : string; names : array<string>; best : array<double>; ok : array<bool>) : string {
    var bi = -1
    for (i, nm in count(), names) {
        if (nm == BASELINE) {
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
    print("--- {kernel} ---\n")
    for (i in range(length(names))) {
        if (!ok[i]) {
            print("  {names[i]}: REJECTED\n")
            continue
        }
        let delta = base > 0.0lf ? (best[i] - base) / base * 100.0lf : 0.0lf
        let mark = i == winner ? "  <== winner" : ""
        print("  {names[i]}: {best[i]} us  ({delta}% vs {BASELINE}){mark}\n")
    }
    if (winner < 0) {
        print("  {kernel}: no passing variant\n")
        return ""
    }
    let wname = names[winner]
    if (wname == BASELINE) {
        print("  => {kernel}: {BASELINE} holds (hand default already best)\n")
    } else {
        let margin = base > 0.0lf ? (base - best[winner]) / base * 100.0lf : 0.0lf
        print("  => {kernel}: {wname} beats {BASELINE} by {margin}%  <<< the tuner found something\n")
    }
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
    for (_round in range(ROUNDS)) {
        for (i in range(nv)) {
            if (!ok[i]) {
                continue
            }
            let f = vs[i]._1
            let t0 = ref_time_ticks()
            for (_rep in range(REPS)) {
                sink += f(pa, pb, N)
            }
            let us = double(get_time_usec(t0))
            if (us < best[i]) {
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
    for (_round in range(ROUNDS)) {
        for (i in range(nv)) {
            if (!ok[i]) {
                continue
            }
            reset(d, d0)
            let f = vs[i]._1
            let t0 = ref_time_ticks()
            for (_rep in range(REPS)) {
                invoke(f, dp, sp, N)
            }
            let us = double(get_time_usec(t0))
            if (us < best[i]) {
                best[i] = us
            }
        }
        sink += d[0]
    }
    print("  checksum {sink}\n")
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
    for (_round in range(ROUNDS)) {
        for (i in range(nv)) {
            if (!ok[i]) {
                continue
            }
            reset(d, d0)
            let f = vs[i]._1
            let t0 = ref_time_ticks()
            for (_rep in range(REPS)) {
                invoke(f, dp, sp, scale, N)
            }
            let us = double(get_time_usec(t0))
            if (us < best[i]) {
                best[i] = us
            }
        }
        sink += d[0]
    }
    print("  checksum {sink}\n")
    return report("axpy", names, best, ok)
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
    for (_round in range(ROUNDS)) {
        for (i in range(nv)) {
            if (!ok[i]) {
                continue
            }
            reset(d, d0)
            let f = vs[i]._1
            let t0 = ref_time_ticks()
            for (_rep in range(REPS)) {
                invoke(f, dp, scale, N)
            }
            let us = double(get_time_usec(t0))
            if (us < best[i]) {
                best[i] = us
            }
        }
        sink += d[0]
    }
    print("  checksum {sink}\n")
    return report("scale_inplace", names, best, ok)
}

[export]
def main {
    print("kernel tuner: N={N}, {ROUNDS} rounds x {REPS} reps, interleaved best-of\n\n")

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

    let dotw = bench_dot()
    let axpyw = bench_axpy()
    let addw = bench_binary("add_inplace", add_vs, add_d0, add_s, @@(a, b : float) : double => double(a) + double(b))
    let mulw = bench_binary("mul_inplace", mul_vs, mul_d0, mul_s, @@(a, b : float) : double => double(a) * double(b))
    let scalew = bench_scale()
    let copyw = bench_binary("copy_floats", cpy_vs, cpy_d0, cpy_s, @@(_a, b : float) : double => double(b))

    // Summary: which kernels the tuner would re-tune away from the hand default.
    print("\n===== summary (winner per kernel) =====\n")
    let knames = fixed_array("dot", "axpy", "add_inplace", "mul_inplace", "scale_inplace", "copy_floats")
    let kwin = fixed_array(dotw, axpyw, addw, mulw, scalew, copyw)
    var changed = 0
    for (k, w in knames, kwin) {
        let note = (w == "" || w == BASELINE) ? "" : "  (was {BASELINE})"
        print("  {k}: {w}{note}\n")
        if (w != "" && w != BASELINE) {
            changed++
        }
    }
    print(changed == 0
        ? "\nall kernels keep {BASELINE} — the hand default holds; the tuner found nothing new.\n"
        : "\n{changed} kernel(s) prefer a non-default perm on this box.\n")

    let profile = JV((dot = dotw, axpy = axpyw, add_inplace = addw,
        mul_inplace = mulw, scale_inplace = scalew, copy_floats = copyw))
    let js = write_json(profile)
    if (fwrite("box_profile.json", js)) {
        print("wrote box_profile.json: {js}\n")
    } else {
        print("failed to write box_profile.json\n")
    }
}
