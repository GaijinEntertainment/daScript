options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
require dastest/testing_boost public
require dasllama/dasllama_math
require dasllama/dasllama_par
require daslib/jobque_boost

// The load-path contract #3586 broke on M1: the eager loader fans its weight repack across
// jobque lanes (repack_regions), and pool-lane contexts DO NOT see the arming context's
// dasllama_math globals — fifo lanes read initializer defaults, fork/team lanes read zeros.
// A repack slot must therefore be context-proof: same bytes from a pool lane as from the
// arming thread. Pre-fix, the gen backend's slot read activation-cached globals at lane time
// and div0'd (q8: mr/kgroup zero) or invoked null (kq layout fn).

def private fill_pattern(var q : array<int8>; var s : array<float>) {
    for (i in range(length(q))) {
        q[i] = int8((i * 7 + 3) % 251 - 125)
    }
    for (i in range(length(s))) {
        s[i] = 0.5 + 0.25 * float(i % 8)
    }
}

[test]
def test_repack_slot_is_context_proof(t : T?) {
    t |> run("gen repack slot: pool-lane bytes == main-thread bytes") @(t : T?) {
        select_matmul_backend_for_load()
        if (!kernel_backend_needs_repack(active_kernel_backend())) {
            t |> success(true, "active backend has no repack - nothing to prove on this box")
            return
        }
        let n = 64l    // k-elems per row (multiple of 32)
        let d = 32l    // rows (multiple of every backend mr)
        var q_main : array<int8>
        var s_main : array<float>
        q_main |> resize(int(n * d))
        s_main |> resize(int(n * d / 32l))
        fill_pattern(q_main, s_main)
        var q_lane := q_main
        var s_lane := s_main

        // main-thread repack — the reference bytes
        repack_q8q8_weight(q_main, s_main, 0l, n, d)

        // the same repack from a plain fifo jobque lane — the eager loader's fan shape:
        // the slot FN is hoisted as a value (repack_regions' discipline), the body runs in the
        // lane. NO team setup on purpose: pool contexts are the divergent ones.
        let rq8 = active_repack_q8q8()
        with_job_que() {
            unsafe {
                var qp = addr(q_lane[0])
                var sp = addr(s_lane[0])
                var done = 0
                var dp = addr(done)
                parallel_for(0, 1, 1) $(rb, re, wg) {
                    feint("{rb} {re}")
                    unsafe {
                        invoke(rq8, qp, sp, n, d)
                        dp[0] = 1
                    }
                    wg |> notify_and_release
                }
                t |> equal(done, 1)
            }
        }

        var diff = 0
        for (a, b in q_main, q_lane) {
            if (a != b) {
                diff++
            }
        }
        t |> equal(diff, 0)
        var sdiff = 0
        for (a, b in s_main, s_lane) {
            if (a != b) {
                sdiff++
            }
        }
        t |> equal(sdiff, 0)
    }
}
