options gen2

require dastest/testing_boost public

// cpu_supports — runtime cpuid feature truth for kernel/backend gating. Fail-closed by design:
// unknown feature names and non-x86 architectures report false; on x86 the OSXSAVE/XGETBV dance
// makes "avx*" answers OS-honest. Assertions here are box-independent (no "this CI runner has
// AVX2" claims — an x86_64 binary under emulation may legitimately lack it).

[test]
def test_cpu_supports(t : T?) {
    t |> run("unknown names fail closed") @(t : T?) {
        t |> success(!cpu_supports("bogus-feature-name"), "unknown feature reports false")
        t |> success(!cpu_supports(""), "empty feature reports false")
    }
    t |> run("architecture consistency") @(t : T?) {
        let arch = get_architecture_name()
        if (arch == "x86_64" || arch == "x86") {
            // implication ladder on real silicon (shared OS-state requirement makes these safe)
            if (cpu_supports("avx2")) {
                t |> success(cpu_supports("avx"), "avx2 implies avx")
            }
            if (cpu_supports("avx512vnni")) {
                t |> success(cpu_supports("avx512f"), "avx512vnni implies avx512f")
            }
        } else {
            t |> success(!cpu_supports("avx2"), "x86 feature names report false off-x86")
            t |> success(!cpu_supports("sse4.2"), "x86 feature names report false off-x86")
        }
    }
}
