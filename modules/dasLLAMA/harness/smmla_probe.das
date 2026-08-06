options gen2
require daslib/jobque_boost   // nolint:STYLE030 — ref_time_ticks/get_time_usec
require llvm/daslib/aarch64_neon
require dasllama/dasllama_env

// SMMLA (i8mm) single-core scout: correctness + ALU-throughput ratio vs SDOT on the same
// synthetic Q8 bytes. Run with -jit on i8mm silicon ONLY (M2/M3+/A15+ — the emitter is
// aarch64-gated, not i8mm-gated, so pre-i8mm cores SIGILL); interpreted anywhere for the
// scalar-body correctness half. This is the M3-Air scout for a future dot="smmla" generator
// leg: smmla = 32 MACs/instr vs sdot4's 16 with no horizontal reduce — the register-resident
// ratio here is the ceiling the leg chases (2.0 nominal).
//
//   bin/daslang -jit modules/dasLLAMA/harness/smmla_probe.das

let K = 4096            // k-bytes per row pass (L1-resident: 2 rows a + 2 rows b = 16KB)
let REPS = 40000        // outer repetitions
let CHAINS = 4          // independent accumulator chains (hide the 3-4 cycle MMA latency)

[export]
def main() {   // nolint:STYLE038 — flat probe phases (gate, fill, run, report); no seam stands alone
    print("smmla probe: jit={jit_enabled()}\n")
    // pre-i8mm host rail: DAS_JIT_ARM64_FORCE_FEATURES=i8mm + this gate jits/links the DLL
    // without executing SMMLA (which would SIGILL here); ship .jitted_scripts to the i8mm box
    // and run there with the same env — the cache key matches, no linker needed on the target
    if (g_env_harness.smmla_probe_compile_only) {
        print("compile-only: DLL linked, execution skipped (pre-i8mm host)\n")
        return
    }
    var a : array<int8>
    var b : array<int8>
    a |> resize(2 * K)   // 2 interleaved token rows
    b |> resize(2 * K)   // 2 interleaved weight rows
    for (i in range(2 * K)) {
        a[i] = int8((i * 37 + 11) % 255 - 127)
        b[i] = int8((i * 73 + 5) % 255 - 127)
    }
    unsafe {
        let ap = addr<int8 const?>(a[0])
        let bp = addr<int8 const?>(b[0])

        // correctness: one smmla vs its 2x2 definition computed with plain int math
        {
            let r = smmla(int4(1, 2, 3, 4), ap, bp)
            var d00 = 1
            var d01 = 2
            var d10 = 3
            var d11 = 4
            for (k in range(8)) {
                d00 += int(ap[k]) * int(bp[k])
                d01 += int(ap[k]) * int(bp[8 + k])
                d10 += int(ap[8 + k]) * int(bp[k])
                d11 += int(ap[8 + k]) * int(bp[8 + k])
            }
            let ok = r.x == d00 && r.y == d01 && r.z == d10 && r.w == d11
            print("correctness: {ok ? "OK" : "MISMATCH"}  smmla={r} expected=[{d00},{d01},{d10},{d11}]\n")
            if (!ok) {
                return
            }
        }

        // sdot4 throughput: CHAINS independent accumulators, 16 MACs per call
        var sink = 0
        let t0 = ref_time_ticks()
        for (_r in range(REPS)) {
            var acc0 = int4(0)
            var acc1 = int4(0)
            var acc2 = int4(0)
            var acc3 = int4(0)
            var off = 0
            while (off + 64 <= K) {
                acc0 = sdot4(acc0, bp + off, ap + off)
                acc1 = sdot4(acc1, bp + off + 16, ap + off + 16)
                acc2 = sdot4(acc2, bp + off + 32, ap + off + 32)
                acc3 = sdot4(acc3, bp + off + 48, ap + off + 48)
                off += 64
            }
            sink += acc0.x + acc1.y + acc2.z + acc3.w
        }
        let sdot_us = get_time_usec(t0)
        let sdot_macs = double(REPS) * double(K / 64 * 4) * 16.0lf
        let sdot_gmacs = sdot_macs / double(sdot_us) / 1000.0lf
        print("sdot4:  {sdot_us}us  {sdot_gmacs} GMAC/s  (sink {sink})\n")

        // smmla throughput: same byte traffic, 32 MACs per call
        var sink2 = 0
        let t1 = ref_time_ticks()
        for (_r in range(REPS)) {
            var acc0 = int4(0)
            var acc1 = int4(0)
            var acc2 = int4(0)
            var acc3 = int4(0)
            var off = 0
            while (off + 64 <= K) {
                acc0 = smmla(acc0, ap + off, bp + off)
                acc1 = smmla(acc1, ap + off + 16, bp + off + 16)
                acc2 = smmla(acc2, ap + off + 32, bp + off + 32)
                acc3 = smmla(acc3, ap + off + 48, bp + off + 48)
                off += 64
            }
            sink2 += acc0.x + acc1.y + acc2.z + acc3.w
        }
        let smmla_us = get_time_usec(t1)
        let smmla_macs = double(REPS) * double(K / 64 * 4) * 32.0lf
        let smmla_gmacs = smmla_macs / double(smmla_us) / 1000.0lf
        print("smmla:  {smmla_us}us  {smmla_gmacs} GMAC/s  (sink {sink2})\n")
        print("ratio:  {smmla_gmacs / sdot_gmacs}x\n")
    }
    delete a
    delete b
}
