options gen2

// Per-box kernel tuner — v1 (dot only). Emits dot_template's grid variants via the
// cross-module [tuned] rail, benches them INTERLEAVED under the JIT (loop hints only
// bite there — see the AOT/interp note in dasllama_tune), correctness-gates each vs a
// scalar reference, and writes the winning permutation to box_profile.json.
//
// Run:  bin/daslang -jit modules/dasLLAMA/harness/tune_dot.das

require dasllama/dasllama_math   // nolint:STYLE030 — side-effect: loads dot_template so [tuned]'s cross-module scan finds it
require dasllama/dasllama_tune   // [tuned] rail
require math                     // mad (referenced by the cloned template body)
require daslib/json
require daslib/json_boost
require daslib/fio

// One [tuned] stub per grid permutation — cross-module clones of dot_template's body
// with that permutation's loop hints stamped onto the [tune=1] loop.
[hint(unsafe_range_check, noalias = a, noalias = b), tuned(src = "dot_template", perm = "plain")]
def dot_plain(a, b : float const?; n : int64) : float {}
[hint(unsafe_range_check, noalias = a, noalias = b), tuned(src = "dot_template", perm = "u2")]
def dot_u2(a, b : float const?; n : int64) : float {}
[hint(unsafe_range_check, noalias = a, noalias = b), tuned(src = "dot_template", perm = "vec8")]
def dot_vec8(a, b : float const?; n : int64) : float {}
[hint(unsafe_range_check, noalias = a, noalias = b), tuned(src = "dot_template", perm = "vec8_u2")]
def dot_vec8u2(a, b : float const?; n : int64) : float {}

typedef DotFn = function<(a, b : float const?; n : int64) : float>

struct Variant {
    suffix : string
    fn     : DotFn
}

def variants() : array<Variant> {
    return <- [
        Variant(suffix = "plain", fn = @@dot_plain),
        Variant(suffix = "u2", fn = @@dot_u2),
        Variant(suffix = "vec8", fn = @@dot_vec8),
        Variant(suffix = "vec8_u2", fn = @@dot_vec8u2)
    ]
}

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

    let vs <- variants()
    let nv = length(vs)

    // Correctness gate: reject any variant whose result drifts (vectorize reassociates
    // FP, so compare to the f64 reference within a relative tolerance).
    var ok : array<bool>
    ok |> resize(nv)
    for (i in range(nv)) {
        let got = double(vs[i].fn(pa, pb, N))
        let rel = abs(got - want) / (abs(want) + 1.0lf)
        ok[i] = rel <= 1.0e-3lf
        if (!ok[i]) print("REJECT {vs[i].suffix}: rel err {rel}\n")
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
            let t0 = ref_time_ticks()
            for (_rep in range(REPS)) {
                sink += vs[i].fn(pa, pb, N)
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
        print("{vs[i].suffix}: {status} (best of {ROUNDS}x{REPS})\n")
    }
    if (winner < 0) {
        print("no passing variant — not writing profile\n")
        return
    }
    print("winner: {vs[winner].suffix}  (sink={sink})\n")

    let profile = JV((dot = vs[winner].suffix))
    let js = write_json(profile)
    let path = "box_profile.json"
    if (fwrite(path, js)) {
        print("wrote {path}: {js}\n")
    } else {
        print("failed to write {path}\n")
    }
}
