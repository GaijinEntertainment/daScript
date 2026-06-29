options gen2
options persistent_heap

require dastest/testing_boost
require dasllama/dasllama_math
require dasllama/dasllama_quant
require daslib/jobque_boost
require math

// Cache-blocking experiment for the batched Q8 GEMM. Three bit-exact nests, to see whether
// keeping weight chunks L1-resident (ntile) or batching tokens (ttok) beats the flat nest:
//   flat  — production matmul_q8q8_batch: row outer, all tokens inner (weights DRAM once,
//           activations re-read from L2 every row).
//   ntile — chunk each weight row into L1-sized sub-rows, reuse each across tokens; a carried
//           float accumulator (dot_q8q8_acc) keeps the per-row sum in strict block order, so
//           it stays bit-for-bit equal to a single dot_q8q8 over the whole row.
//   ttok  — token-tiled: sweep all weights for a tile of `tb` tokens, then the next tile —
//           re-streams the whole weight matrix once per tile.
// Run across the L2 boundary (qo fits L2, big16k exceeds it) and watch where each one breaks.
//
//   bin/daslang -jit dastest/dastest.das -- --bench --test modules/dasLLAMA/bench_matmul_batch_cache.das

def fillf(var a : array<float>; cnt : int64) {
    a |> resize(cnt)
    for (i in range64(cnt)) {
        a[i] = float(i % 13l) * 0.1 - 0.6
    }
}

// Q8·Q8 dot that ADDS into a running accumulator over `nblocks` 32-blocks (returns the new
// total). Carried across sub-rows in block order, the float sum matches dot_q8q8 exactly.
def dot_q8q8_acc(acc : float; wq : int8 const?; ws : float const?; xq : int8 const?; xs : float const?; nblocks : int64) : float {
    var a = acc
    for (bi in range64(nblocks)) {
        let base = bi * 32l
        var iacc = 0
        for [vectorize, vectorize_width = 16] (k in range64(32l)) {
            iacc += int(unsafe(wq[base + k])) * int(unsafe(xq[base + k]))
        }
        a += float(iacc) * unsafe(ws[bi]) * unsafe(xs[bi])
    }
    return a
}

// ntile: row outer, n split into tiles of `tile_blocks` 32-blocks (each weight sub-row L1-sized),
// reused across all ntok tokens via a per-token carried accumulator.
def matmul_q8q8_batch_ntile(var y : array<float>; wq : array<int8> | #; ws : array<float> | #; xq : array<int8>; xs : array<float>; n, d, ntok, tile_blocks : int64) {
    let nb = n / 32l
    unsafe {
        let yp = addr(y[0])
        let wp = reinterpret<int8 const?>(addr(wq[0]))
        let sp = reinterpret<float const?>(addr(ws[0]))
        let xqp = reinterpret<int8 const?>(addr(xq[0]))
        let xsp = reinterpret<float const?>(addr(xs[0]))
        parallel_for(0, int(d), get_total_hw_jobs()) $(rb, re, wg) {
            unsafe {
                var myp = reinterpret<float?>(yp)
                var acc : array<float>
                acc |> resize(ntok)
                for (i in range(rb, re)) {
                    let wrow = wp + int64(i) * n
                    let srow = sp + int64(i) * nb
                    for (tk in range64(ntok)) {
                        acc[tk] = 0.0
                    }
                    var bb = 0l
                    while (bb < nb) {
                        let bcount = min(tile_blocks, nb - bb)
                        let koff = bb * 32l
                        let wsub = wrow + koff
                        let ssub = srow + bb
                        for (tk in range64(ntok)) {
                            acc[tk] = dot_q8q8_acc(acc[tk], wsub, ssub, xqp + tk * n + koff, xsp + tk * nb + bb, bcount)
                        }
                        bb += bcount
                    }
                    for (tk in range64(ntok)) {
                        myp[tk * d + int64(i)] = acc[tk]
                    }
                }
            }
            wg |> notify_and_release
        }
    }
}

// ttok: token-tiled — one full weight sweep (parallel over rows) per tile of `tb` tokens.
def matmul_q8q8_batch_ttok(var y : array<float>; wq : array<int8> | #; ws : array<float> | #; xq : array<int8>; xs : array<float>; n, d, ntok, tb : int64) {
    let nb = n / 32l
    unsafe {
        let yp = addr(y[0])
        let wp = reinterpret<int8 const?>(addr(wq[0]))
        let sp = reinterpret<float const?>(addr(ws[0]))
        let xqp = reinterpret<int8 const?>(addr(xq[0]))
        let xsp = reinterpret<float const?>(addr(xs[0]))
        var t0 = 0l
        while (t0 < ntok) {
            let t1 = min(t0 + tb, ntok)
            parallel_for(0, int(d), get_total_hw_jobs()) $(rb, re, wg) {
                unsafe {
                    var myp = reinterpret<float?>(yp)
                    for (i in range(rb, re)) {
                        let wrow = wp + int64(i) * n
                        let srow = sp + int64(i) * nb
                        for (tk in range64(t0, t1)) {
                            myp[tk * d + int64(i)] = dot_q8q8(wrow, srow, xqp + tk * n, xsp + tk * nb, n)
                        }
                    }
                }
                wg |> notify_and_release
            }
            t0 = t1
        }
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

def cache_shape(b : B?; name : string; n, d : int64) {
    let nb = n / 32l
    var w : array<float>
    fillf(w, n * d)
    let qt8 <- quantize_q8_0(w, n * d)
    var x : array<float>
    fillf(x, n)
    var xq : array<int8>
    var xs : array<float>
    xq |> resize(n)
    xs |> resize(nb)
    quantize_q8_0_into(x, n, xq, xs, 0l, 0l)

    let NT <- [32l, 128l]
    for (ntok in NT) {
        var xqb : array<int8>
        var xsb : array<float>
        xqb |> resize(ntok * n)
        xsb |> resize(ntok * nb)
        for (tk in range64(ntok)) {
            for (i in range64(n)) {
                xqb[tk * n + i] = xq[i]
            }
            for (i in range64(nb)) {
                xsb[tk * nb + i] = xs[i]
            }
        }
        var yf : array<float>
        var yn : array<float>
        var yt : array<float>
        yf |> resize(ntok * d)
        yn |> resize(ntok * d)
        yt |> resize(ntok * d)

        // correctness gate: both variants must be bit-for-bit equal to the flat kernel
        matmul_q8q8_batch(yf, qt8.q, qt8.scales, xqb, xsb, n, d, ntok)
        matmul_q8q8_batch_ntile(yn, qt8.q, qt8.scales, xqb, xsb, n, d, ntok, 16l)
        matmul_q8q8_batch_ttok(yt, qt8.q, qt8.scales, xqb, xsb, n, d, ntok, 32l)
        if (!all_equal(yf, yn, ntok * d)) {
            b->failNow()
        }
        if (!all_equal(yf, yt, ntok * d)) {
            b->failNow()
        }

        b |> run("{name} flat  n={ntok}", int(ntok)) {
            matmul_q8q8_batch(yf, qt8.q, qt8.scales, xqb, xsb, n, d, ntok)
        }
        b |> run("{name} ntile n={ntok}", int(ntok)) {
            matmul_q8q8_batch_ntile(yn, qt8.q, qt8.scales, xqb, xsb, n, d, ntok, 16l)
        }
        b |> run("{name} ttok  n={ntok}", int(ntok)) {
            matmul_q8q8_batch_ttok(yt, qt8.q, qt8.scales, xqb, xsb, n, d, ntok, 32l)
        }
    }
}

[benchmark]
def bench_matmul_batch_cache(b : B?) {
    with_job_que() {
        set_jobque_fork_pool(true, true)
        cache_shape(b, "qo    2048x2048 ", 2048l, 2048l)    // 4.2 MB Q8 — fits L2
        cache_shape(b, "big16 2048x16000", 2048l, 16000l)   // 32 MB Q8 — exceeds L2
    }
}
