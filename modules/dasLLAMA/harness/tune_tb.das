options gen2

// Per-box TB (token-block) measurement harness for the laneq batched Q8·Q8 GEMM.
// TB is a RUNTIME knob (set_q8_token_block) read only by q8q8_batch_kernel_neon_laneq: it tiles the
// token loop so a TB×n activation slice stays L2-resident and is reused across the d/4 weight groups,
// instead of re-streaming the whole weight matrix once per token-block. Physics: TB_opt ≈ L2/n — n
// sets the ceiling (the slice must fit L2), d only scales the payoff (bigger weights = more to save by
// not re-streaming), so the optimum should track n and be ~flat in d. This harness MEASURES that:
//   1. n-sweep (d fixed): TB*(n) — expect it to fall as n rises (~L2/n).
//   2. d-check (n fixed): does TB* shift with d, or only the margin?
// It REPORTS only (no box_profile write) — the data decides whether we key TB by n alone or add a d
// term before we wire the load-time roundtrip.
//
// TB only reschedules the token loop; every output's math is unchanged, so all TB values must be
// BIT-identical (that is the correctness gate). Run:
//   bin/daslang -jit modules/dasLLAMA/harness/tune_tb.das

require dasllama/dasllama_math
require dasllama/dasllama_math_default          // nolint:STYLE030 — portable fallback + [init] ordering
require dasllama/dasllama_math_aarch64_neon      // nolint:STYLE030 — registers the arm64-laneq backend at [init]
require dasllama/dasllama_quant
require daslib/jobque_boost
require math

// TB only bites when the FULL activation (ntok×n) spills L2 (~12MB on M1 Max) — then blocking keeps a
// TB×n slice resident. So ntok must be large and the grid must reach the ~L2/n knee (n=8192 → ~1500).
let TB_GRID = fixed_array(128l, 256l, 512l, 1024l, 2048l)
let NTOK = 2048l
let ROUNDS = 20
let REPS = 1
let BASE_TB = 128l   // today's hand default — the delta reference

def fillf(var a : array<float>; cnt : int64) {
    a |> resize(int(cnt))
    for (i in range64(cnt)) {
        a[i] = float(i % 13l) * 0.1f - 0.6f
    }
}

def all_equal(a, b : array<float>; len : int64) : bool {
    for (k in range64(len)) {
        if (a[k] != b[k]) {
            return false
        }
    }
    return true
}

// Bench one weight shape (n = contraction dim, d = output rows) across the TB grid, interleaved
// best-of-N. Returns the winning TB (or -1 if a variant broke the bit-identity gate).
def bench_shape(name : string; n, d : int64) : int64 {
    let nb = n / 32l
    // Q8 weights, repacked into the laneq interleaved layout (load-time cost, done once, untimed).
    var w : array<float>
    fillf(w, n * d)
    var qt <- quantize_q8_0(w, n * d)
    delete w
    repack_q8q8_weight(qt.q, qt.scales, 0l, n, d)

    // One activation column, replicated across NTOK tokens (values don't affect timing; the bit-identity
    // gate below doesn't depend on them either).
    var x : array<float>
    fillf(x, n)
    var xq : array<int8>
    var xs : array<float>
    xq |> resize(int(n))
    xs |> resize(int(nb))
    quantize_q8_0_into(x, n, xq, xs, 0l, 0l)
    delete x
    var xqb : array<int8>
    var xsb : array<float>
    xqb |> resize(int(NTOK * n))
    xsb |> resize(int(NTOK * nb))
    for (tk in range64(NTOK)) {
        for (i in range64(n)) {
            xqb[int(tk * n + i)] = xq[int(i)]
        }
        for (i in range64(nb)) {
            xsb[int(tk * nb + i)] = xs[int(i)]
        }
    }
    delete xq
    delete xs

    let ntb = length(TB_GRID)
    var y : array<float>
    var yref : array<float>
    y |> resize(int(NTOK * d))
    yref |> resize(int(NTOK * d))

    var best : array<double>
    var ok : array<bool>
    best |> resize(ntb)
    ok |> resize(ntb)

    // Correctness gate: reference at the default TB, then every TB must reproduce it bit-for-bit
    // (TB only reschedules the token loop — any difference is a real scheduling bug).
    set_q8_token_block(BASE_TB)
    matmul_q8q8_batch(yref, qt.q, qt.scales, xqb, xsb, n, d, NTOK)
    for (ti in range(ntb)) {
        set_q8_token_block(TB_GRID[ti])
        matmul_q8q8_batch(y, qt.q, qt.scales, xqb, xsb, n, d, NTOK)
        ok[ti] = all_equal(yref, y, NTOK * d)
        best[ti] = 1.0e30lf
        if (!ok[ti]) {
            print("  !! {name} TB={TB_GRID[ti]} NOT bit-identical to TB={BASE_TB}\n")
        }
    }

    var sink = 0.0f
    for (_round in range(ROUNDS)) {
        for (ti in range(ntb)) {
            if (!ok[ti]) {
                continue
            }
            set_q8_token_block(TB_GRID[ti])
            let t0 = ref_time_ticks()
            for (_rep in range(REPS)) {
                matmul_q8q8_batch(y, qt.q, qt.scales, xqb, xsb, n, d, NTOK)
            }
            let us = double(get_time_usec(t0))
            if (us < best[ti]) {
                best[ti] = us
            }
        }
        sink += y[0]
    }

    // Report the TB curve as %delta vs BASE_TB, mark the winner.
    var bi = -1
    for (ti in range(ntb)) {
        if (TB_GRID[ti] == BASE_TB) {
            bi = ti
        }
    }
    let base = (bi >= 0 && ok[bi]) ? best[bi] : -1.0lf
    var winner = -1
    for (ti in range(ntb)) {
        if (ok[ti] && (winner < 0 || best[ti] < best[winner])) {
            winner = ti
        }
    }
    print("--- {name}  (n={n} d={d} ntok={NTOK}) ---\n")
    for (ti in range(ntb)) {
        if (!ok[ti]) {
            print("  TB={TB_GRID[ti]}: REJECTED\n")
            continue
        }
        let delta = base > 0.0lf ? (best[ti] - base) / base * 100.0lf : 0.0lf
        let mark = ti == winner ? "  <== TB*" : ""
        print("  TB={TB_GRID[ti]}: {best[ti]} us  ({delta}% vs {BASE_TB}){mark}\n")
    }
    print("  checksum {sink}\n")

    delete qt
    delete xqb
    delete xsb
    delete y
    delete yref
    return winner >= 0 ? TB_GRID[winner] : -1l
}

[export]
def main {
    if (!jit_enabled()) {
        print("skipped: interpreted — the laneq backend (the only TB consumer) registers under -jit only\n")
        return
    }
    with_job_que() {
        set_jobque_fork_pool(true, true)
        let need_repack = select_matmul_backend_for_load()
        print("active backend: {active_kernel_backend()} (needs_repack={need_repack})\n")
        if (active_kernel_backend() != "arm64-laneq") {
            print("skipped: laneq backend not active (needs arm64+jit) — TB is only read by the laneq batch kernel\n")
            return
        }
        set_q8_l2_budget(1l << 40l)   // disable the production cliff-guard clamp — this harness MEASURES the raw TB curve (incl. the cliff)
        print("TB grid {TB_GRID}, ntok={NTOK}, {ROUNDS} rounds x {REPS} reps, interleaved best-of\n\n")

        // n-sweep (d fixed = 2048): n=2048 fits L2 even at ntok=2048 (control, TB should not matter);
        // n>=8192 spills (ntok*n > 12MB), so TB* should appear and track ~L2/n.
        print("===== n-sweep (d=2048) =====\n")
        let n1 = bench_shape("n=2048  ", 2048l, 2048l)    // ntok*n = 4MB  — fits L2 (control)
        let n2 = bench_shape("n=8192  ", 8192l, 2048l)    // 16MB — spills; knee ~L2/n ≈ 1500
        let n3 = bench_shape("n=11008 ", 11008l, 2048l)   // 22MB — spills more; knee ~1100
        let n4 = bench_shape("n=14336 ", 14336l, 2048l)   // 29MB — spills most; knee ~850

        // d-check: hold n=8192 (in-regime), vary d — does TB* move, or only the margin?
        print("\n===== d-check (n=8192) =====\n")
        let d2 = bench_shape("n=8192 d=8192", 8192l, 8192l)

        print("\n===== summary: TB* per shape =====\n")
        print("  n-sweep (d=2048):  n=2048 -> {n1}   n=8192 -> {n2}   n=11008 -> {n3}   n=14336 -> {n4}\n")
        print("  d-check (n=8192):  d=2048 -> {n2}   d=8192 -> {d2}\n")
        print("\nread: does TB* fall as n rises (~L2/n)? is n=2048 flat (fits L2)? does d shift TB* or only the margin?\n")
    }
}
