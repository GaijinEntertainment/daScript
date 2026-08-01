options gen2
options persistent_heap
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)

require dasllama/dasllama_math_gen
require dasllama/dasllama_math
require math

// Q4_K (k4q8) kernel microbench — the seam-bisection ground floor: raw kernel, one thread,
// no jobque, synthetic planes (timing is data-independent). GEMV = decode shape, TILE = the
// prefill batch shape (64 tokens). Run under -jit (the generators only fire there).
// Usage: bin/daslang -jit modules/dasLLAMA/harness/k4_kernel_bench.das

let N = 2560    // Qwen3-4B dim
let D = 2560    // rows (attn proj shape)
let NTOK = 64   // prefill chunk
let ROUNDS = 5

[export]
def main {
    if (!jit_enabled()) {
        print("run with -jit: the kq generators only fire under the JIT\n")
        return
    }
    let mr = int64(q8q8_layout_gen())
    let nsb = int64(N) / 256l
    let kq_bytes = int64(D) * nsb * 128l
    let ks_bytes = int64(D) * nsb * 32l
    print("mr={mr}, nsb={nsb}, planes: kq {kq_bytes / 1024l}KB + ks {ks_bytes / 1024l}KB\n")

    var kq : array<uint8>
    var ks : array<uint8>
    kq |> resize(int(kq_bytes))
    ks |> resize(int(ks_bytes))
    var seed = 0x1234u
    for (b in kq) {
        seed = seed * 1664525u + 1013904223u
        b = uint8(seed >> 24u)
    }
    for (b in ks) {
        seed = seed * 1664525u + 1013904223u
        b = uint8(seed >> 24u)
    }
    var xq : array<int8>
    var xs : array<float>
    var xbs : array<int>
    var y : array<float>
    xq |> resize(NTOK * N)
    xs |> resize(NTOK * N / 32)
    xbs |> resize(NTOK * N / 16)
    y |> resize(NTOK * D)
    for (q in xq) {
        seed = seed * 1664525u + 1013904223u
        q = int8(seed >> 24u)
    }
    for (s in xs) {
        s = 0.01
    }
    for (bsum in xbs) {
        bsum = 128
    }

    unsafe {
        let kqp = addr<uint8 const?>(kq[0])
        let ksp = addr<uint8 const?>(ks[0])
        let xqp = addr<int8 const?>(xq[0])
        let xsp = addr<float const?>(xs[0])
        let xbsp = addr<int const?>(xbs[0])
        var yp = addr(y[0])

        // GEMV (decode shape): full matrix, one token
        k4q8_gemv_gen(yp, kqp, ksp, xqp, xsp, xbsp, int64(N), 0l, int64(D))   // warmup + codegen
        var best_us = 999999999l
        for (_r in range(ROUNDS)) {
            let t0 = ref_time_ticks()
            k4q8_gemv_gen(yp, kqp, ksp, xqp, xsp, xbsp, int64(N), 0l, int64(D))
            let us = int64(get_time_usec(t0))
            best_us = min(best_us, us)
        }
        let gemv_gflops = 2.0 * float(N) * float(D) / (float(best_us) * 1000.0)
        let gemv_gbs = float(kq_bytes + ks_bytes) / (float(best_us) * 1000.0)
        print("k4q8_gemv: best {best_us}us -> {gemv_gflops} GFLOP/s, {gemv_gbs} GB/s weight-read\n")

        // TILE (prefill shape): 64 tokens x full matrix, weight-stationary 4-token blocks
        let ngroups = int64(D) / mr
        var best_tile_us = 999999999l
        for (_r in range(ROUNDS)) {
            let t0 = ref_time_ticks()
            for (g in range64(ngroups)) {
                var t0k = 0l
                while (t0k < int64(NTOK)) {
                    k4q8_tile_gen(yp, kqp + g * mr * nsb * 128l, ksp + g * mr * nsb * 32l,
                        xqp, xsp, xbsp, int64(N), int64(D), g, t0k)
                    t0k += 4l
                }
            }
            let us = int64(get_time_usec(t0))
            best_tile_us = min(best_tile_us, us)
        }
        let tile_gflops = 2.0 * float(N) * float(D) * float(NTOK) / (float(best_tile_us) * 1000.0)
        print("k4q8_tile ({NTOK} tok): best {best_tile_us}us -> {tile_gflops} GFLOP/s\n")
    }
    delete kq
    delete ks
    delete xq
    delete xs
    delete xbs
    delete y
}
