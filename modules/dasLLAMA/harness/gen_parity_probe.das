options gen2
options persistent_heap
options _jit_fast_math = true   // match gemm_1core_probe / the end-to-end config

// M2/M4 parity gate (gemm_generator_plan.md): the [llvm_code]-generated tile (q8q8_tile_gen,
// whatever perm is stamped — fallback or manifest) vs the hand laneq semantics. The oracle is
// computed ONCE from a grp4 copy via dot_q8q8_laneq4 — the per-block fold order is layout-
// independent, so the same oracle serves every stamped mr. The gate is a TOLERANCE
// (CLOSE_REL/CLOSE_ABS), not equality: under full fast-math the backend may legally regroup
// a fold per loop shape (~1e-6 relative — the same laxity llama.cpp runs with); real
// generator bugs are orders louder. The working buffers go through the backend repack, which
// reads the q8q8_layout_gen companion — so this probe exercises the production two-function
// coherence: manifest stamp -> layout report -> grp<mr> repack -> generated tile. Shapes
// cover the paired-block loop (nb even), the odd-block tail (nb odd), and a production-ish
// slice. Run: daslang -jit gen_parity_probe.das
// (--jit-dump to eyeball the generated IR — look for the "sdot" calls + "wv0..." loads)

require dasllama/dasllama_math
require dasllama/dasllama_math_default
require dasllama/dasllama_math_aarch64_neon
require dasllama/dasllama_math_gen
require dasllama/dasllama_repack
require dasllama/dasllama_convert
require math

let CLOSE_REL = 1e-4
let CLOSE_ABS = 1e-5

def test_shape(n, d, ntok : int64) : bool {
    let nb = n / 32l
    var w : array<float>
    w |> resize(int(n * d))
    for (i in range64(n * d)) {
        w[i] = float(i % 17l) * 0.13 - 1.1
    }
    let qt <- quantize_q8_0(w, n * d)
    var q : array<int8>
    var s : array<float>
    q |> resize(int(n * d))
    s |> resize(int(d * nb))
    for (i in range64(n * d)) {
        q[i] = qt.q[i]
    }
    for (i in range64(d * nb)) {
        s[i] = qt.scales[i]
    }

    // distinct per-token activations
    var xrow : array<float>
    xrow |> resize(int(n))
    var xq : array<int8>
    var xs : array<float>
    xq |> resize(int(ntok * n))
    xs |> resize(int(ntok * nb))
    for (tk in range64(ntok)) {
        for (i in range64(n)) {
            xrow[i] = float((i + tk * 7l) % 23l) * 0.07 - 0.8
        }
        quantize_q8_0_into(xrow, n, xq, xs, tk * n, tk * nb)
    }

    // the oracle: hand laneq semantics off a grp4 copy of the row-major master
    var yref : array<float>
    yref |> resize(int(ntok * d))
    var q4 := q
    var s4 := s
    unsafe {
        repack_q8q8_grp(addr(q4[0]), addr(s4[0]), n, d, 4l)
        let wp = addr(q4[0])
        let sp = addr(s4[0])
        let xqp = addr(xq[0])
        let xsp = addr(xs[0])
        var yrp = addr(yref[0])
        for (g in range64(d / 4l)) {
            let wgp = wp + g * 4l * n
            let sgp = sp + g * 4l * nb
            for (tk in range64(ntok)) {
                let f = dot_q8q8_laneq4(wgp, sgp, xqp + tk * n, xsp + tk * nb, n)
                yrp[tk * d + g * 4l + 0l] = f.x
                yrp[tk * d + g * 4l + 1l] = f.y
                yrp[tk * d + g * 4l + 2l] = f.z
                yrp[tk * d + g * 4l + 3l] = f.w
            }
        }
    }
    delete q4
    delete s4

    // the working buffers: the backend repack — grp<mr> per the layout companion
    select_kernel_backend("arm64-gen")
    repack_q8q8_weight(q, s, 0l, n, d)
    let mr = int64(q8q8_layout_gen())
    let ng = d / mr

    var ygen : array<float>
    ygen |> resize(int(ntok * d))
    unsafe {
        let wp = addr(q[0])
        let sp = addr(s[0])
        let xqp = addr(xq[0])
        let xsp = addr(xs[0])
        var ygp = addr(ygen[0])
        for (g in range64(ng)) {
            let wgp = wp + g * mr * n
            let sgp = sp + g * mr * nb
            var t0 = 0l
            while (t0 < ntok) {
                // xbsp = null: this probe is arm64-gen-gated (main), and sdot stamps never bias
                q8q8_tile_gen(ygp, wgp, sgp, xqp, xsp, null, n, d, g, t0)
                t0 += 4l
            }
        }
    }
    var exact = 0
    var md = 0.0
    var ok = true
    for (i in range(length(ygen))) {
        let df = abs(ygen[i] - yref[i])
        if (df == 0.0) {
            exact++
        }
        md = max(md, df)
        ok &&= df <= max(CLOSE_ABS, CLOSE_REL * abs(yref[i]))
    }

    // the stamped gemv rows core on the same buffers: token 0's row of the oracle
    var ygv : array<float>
    ygv |> resize(int(d))
    unsafe {
        q8q8_gemv_gen(addr(ygv[0]), addr(q[0]), addr(s[0]), addr(xq[0]), addr(xs[0]), n, 0l, ng * mr)
    }
    var gmd = 0.0
    var gok = true
    for (i in range(length(ygv))) {
        let df = abs(ygv[i] - yref[i])
        gmd = max(gmd, df)
        gok &&= df <= max(CLOSE_ABS, CLOSE_REL * abs(yref[i]))
    }
    print("n={n} d={d} ntok={ntok} mr={mr}: tile {exact}/{length(ygen)} exact, maxdiff {md} — {ok ? "OK" : "MISMATCH"}; gemv maxdiff {gmd} — {gok ? "OK" : "MISMATCH"}\n")
    return ok && gok
}

[export]
def main {
    if (!jit_enabled()) {
        print("run with -jit — the generator only fires under the LLVM JIT\n")
        return
    }
    var found = false
    for (nm in kernel_backend_names()) {
        found ||= nm == "arm64-gen"
    }
    if (!found) {
        print("arm64-gen not registered (non-arm64 host?) — nothing to test\n")
        return
    }
    var ok = test_shape(64l, 8l, 4l)      // nb=2: paired-block loop only
    ok = test_shape(96l, 8l, 4l) && ok    // nb=3: odd tail
    ok = test_shape(2048l, 64l, 8l) && ok // production-ish slice
    print(ok ? "GEN PARITY OK\n" : "GEN PARITY FAIL\n")
}
