options gen2
options persistent_heap
options _jit_fast_math = true   // match gemm_1core_probe / the end-to-end config
options _dasllama_internal = true

// M2/M4 parity gate (gemm_generator_plan.md): the [llvm_code]-generated tile (q8q8_tile_gen,
// whatever perm is stamped — fallback or manifest) vs the hand laneq semantics. The oracle is
// computed ONCE from a grp4 copy via dot_q8q8_laneq4 — the per-block fold order is layout-
// independent, so the same oracle serves every stamped mr. The gate is a TOLERANCE
// (CLOSE_REL/CLOSE_ABS), not equality: under full fast-math the backend may legally regroup
// a fold per loop shape (~1e-6 relative — the same laxity the reference runs with); real
// generator bugs are orders louder. The working buffers go through the backend repack, which
// reads the q8q8_layout_gen companion — so this probe exercises the production two-function
// coherence: manifest stamp -> layout report -> grp<mr> repack -> generated tile. Shapes
// cover the paired-block loop (nb even), the odd-block tail (nb odd), and a production-ish
// slice. Run: daslang -jit gen_parity_probe.das
// (--jit-dump to eyeball the generated IR — look for the "sdot" calls + "wv0..." loads)

require dasllama/dasllama_math
require dasllama/dasllama_math_default
require dasllama/dasllama_math_gen
require _gen_probe_fixture   // the q8 fixture, its laneq oracle, the tile/gemv sweeps and the compare, shared with gen_tune_probe

def test_shape(n, d, ntok : int64) : bool {
    var fx <- build_fixture(n, d, ntok)

    // the working buffers: the backend repack — grp<mr> per the layout companion
    select_kernel_backend("arm64-gen")
    var q := fx.q
    var s := fx.s
    repack_q8q8_weight(q, s, 0l, n, d)
    let mr = int64(q8q8_layout_gen())

    // the bsums plane rides along: this probe is arm64-gen-gated (main), and sdot stamps never read it
    var ygen : array<float>
    ygen |> resize(ntok * d)
    run_tile(@@q8q8_tile_gen, @@q8q8_amx_cfg_gen, fx, q, s, mr, 4l, 1l, ygen)
    var md = 0.0
    let ok = close_to_ref(fx, ygen, md)
    var exact = 0
    for (yg, yr in ygen, fx.yref) {
        exact += yg == yr ? 1 : 0
    }

    // the stamped gemv rows core on the same buffers: token 0's row of the oracle
    var ygv : array<float>
    ygv |> resize(d)
    run_gemv(@@q8q8_gemv_gen, fx, q, s, mr, ygv)
    var gmd = 0.0
    let gok = close_to_ref(fx, ygv, gmd)
    print("n={n} d={d} ntok={ntok} mr={mr}: tile {exact}/{length(ygen)} exact, maxdiff {md} - {ok ? "OK" : "MISMATCH"}; gemv maxdiff {gmd} - {gok ? "OK" : "MISMATCH"}\n")
    return ok && gok
}

[export]
def main {
    if (!jit_enabled()) {
        print("run with -jit - the generator only fires under the LLVM JIT\n")
        return
    }
    if (!kernel_backend_available_("arm64-gen")) {
        print("arm64-gen not registered (non-arm64 host?) - nothing to test\n")
        return
    }
    var ok = test_shape(64l, 8l, 4l)      // nb=2: paired-block loop only
    ok = test_shape(96l, 8l, 4l) && ok    // nb=3: odd tail
    ok = test_shape(2048l, 64l, 8l) && ok // production-ish slice
    print(ok ? "GEN PARITY OK\n" : "GEN PARITY FAIL\n")
}
