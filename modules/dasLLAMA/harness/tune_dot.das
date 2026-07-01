options gen2

// Per-box kernel tuner — v1 (dot only). [dasllama_grid] emits dot_template's grid variants
// (cross-module clones with each perm's loop hints stamped on the [tune=1] loop) plus the
// dot_template_variants() fn-ptr registry. The bench runs them INTERLEAVED under the JIT
// (loop hints only bite there — see the AOT/interp note in dasllama_tune), correctness-gates
// each vs a scalar reference, and writes the winning permutation to box_profile.json.
//
// Run:  bin/daslang -jit modules/dasLLAMA/harness/tune_dot.das

require dasllama/dasllama_math   // nolint:STYLE030 — side-effect: loads dot_template so the cross-module scan finds it
require dasllama/dasllama_tune   // [dasllama_grid] emitter
require math                     // mad (referenced by the cloned template body)
require daslib/json
require daslib/json_boost
require daslib/fio

// One line replaces the per-perm [tuned] stubs + the hand-written variants() array: emits
// dot_template__<suffix> for every grid perm and a dot_template_variants() : array<tuple<...>>
// registry of (suffix, @@fn) pairs.
[dasllama_grid(src = "dot_template")]
struct DotGrid {}

// f64 scalar reference — the correctness oracle (order-fixed, higher precision).
def ref_dot(a, b : float const?; n : int64) : double {
    var s = 0.0lf
    for (j in range64(n)) {
        s += double(unsafe(a[j])) * double(unsafe(b[j]))
    }
    return s
}

[export]
def main {
    let N = 4096l
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

    // Correctness gate: reject any variant whose result drifts (vectorize reassociates
    // FP, so compare to the f64 reference within a relative tolerance).
    var ok : array<bool>
    ok |> resize(nv)
    for (i in range(nv)) {
        let f = vs[i]._1
        let got = double(f(pa, pb, N))
        let rel = abs(got - want) / (abs(want) + 1.0lf)
        ok[i] = rel <= 1.0e-3lf
        if (!ok[i]) print("REJECT {vs[i]._0}: rel err {rel}\n")
    }

    // Interleaved best-of-N: every round touches all variants (thermal/scheduler robust);
    // keep the best (min) time per variant. Indirect fn-ptr calls so the JIT can't fold.
    let ROUNDS = 200
    let REPS = 2000
    var best : array<double>
    best |> resize(nv)
    for (i in range(nv)) { best[i] = 1.0e30lf }
    var sink = 0.0f
    for (_round in range(ROUNDS)) {
        for (i in range(nv)) {
            if (!ok[i]) { continue }
            let f = vs[i]._1
            let t0 = ref_time_ticks()
            for (_rep in range(REPS)) {
                sink += f(pa, pb, N)
            }
            let us = double(get_time_usec(t0))
            if (us < best[i]) { best[i] = us }
        }
    }

    // Winner = fastest passing variant.
    var winner = -1
    for (i in range(nv)) {
        if (ok[i] && (winner < 0 || best[i] < best[winner])) { winner = i }
    }
    for (i in range(nv)) {
        let status = ok[i] ? "{best[i]} us" : "REJECTED"
        print("{vs[i]._0}: {status} (best of {ROUNDS}x{REPS})\n")
    }
    if (winner < 0) {
        print("no passing variant — not writing profile\n")
        return
    }
    print("winner: {vs[winner]._0}  (sink={sink})\n")

    let profile = JV((dot = vs[winner]._0))
    let js = write_json(profile)
    let path = "box_profile.json"
    if (fwrite(path, js)) {
        print("wrote {path}: {js}\n")
    } else {
        print("failed to write {path}\n")
    }
}
