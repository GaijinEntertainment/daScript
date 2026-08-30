# Benchmark follow-ups

Compiled-tier and runtime findings from the dasProfile records that are not being acted on in
the arc that found them. Numbers are per-call medians from the budgeted-batch records of
2026-08-30 (3990X cold, M1 Max); the interpreter's own list is `interpreter_followups.md`.

## mandelbrot on the JIT and AOT lanes

3990X: C++ 304 us, .NET 311 us, das JIT 449 us, das AOT 632 us. M1: .NET 380 us, das JIT
463 us, das AOT 565 us, C++ 566 us. The interpreter wins its board on both boxes (3.26 ms
against Quirrel's 8.9 ms), but the compiled lanes trail C++ by 1.5x (JIT) and 2.1x (AOT) on
the zen2 box on a tight float loop. Candidates: the AOT's scalar float math going through the
vec4f `SimPolicy<float>` shape (the same path the exp-loop fix touched), and what the JIT's
loop vectorizer makes of the inner iteration.

## float2string

3990X: Luau `--codegen` 2.53 ms, .NET 3.56 ms, das AOT 3.64 ms, das JIT 3.69 ms, LuaJIT
6.33 ms, C++ 10.2 ms. M1: das JIT 1.64 ms, das AOT 1.82 ms, Luau `--codegen` 2.06 ms, .NET
2.62 ms, C++ 5.9 ms. Luau's number formatting beats das by 1.4x on zen2 while das wins on the
M1, and the C++ reference lane is the slowest compiled lane on both - the float-to-string
path (the formatter behind `string(float)` and `"{f}"`, and the C++ twin's `snprintf`) wants a
look on its own: which algorithm, what precision contract, and why the two boxes disagree.

## scalar exp through the vecmath lane

The exp-loop record: 3990X das AOT 9215 us against das JIT 3383 us and C++ 3240 us; M1
das AOT 3592 us against JIT 1629 us and C++ 1682 us. A C++ probe of the loop body under the
AOT unit's own flags (MSVC /O2, x64 default = SSE2, 1e6 iterations, best of 7) reproduces
every row: `SimPolicy<float>::Exp(RcpEst(..))` as AOT emits it - extract, `v_set_x`, `v_exp`,
extract - 9.45 ns/iter; the same composed directly in vecmath 6.11; `expf` (the C++ twin, and
what the JIT's `@llvm.exp` calls) 3.26; `expf` over `rcp_est` 3.43; a `v_exp2_est_p4` scalar
lane 4.44. With /arch:AVX2 the lane version drops to 5.88 and `expf` stays 3.25. So scalar
`exp` through the vecmath lane trick is 2-3x the CRT's `expf`: the `v_set_x`/`v_extract_x`
round-trip is a partial-xmm dependency chain MSVC does not break, and the polynomial itself is
latency-bound in a scalar loop while UCRT's `expf` is a dispatching, dependency-broken routine.
`benchmarks/core/math/exp.das` holds the das-side rows (interp 8.0 / 7.5 / 7.0 ns/op for
`exp` / `exp_std` / `exp_est`; JIT 2.7 / 3.1 / 3.0 - the interpreter's dispatch overlaps the
chain, which is why the interp row hides it).

Proposal: the scalar arms of `SimPolicy<float>::Exp/Exp2/Log/Log2/Pow` call the CRT (`expf`,
`exp2f`, `logf`, `log2f`, `powf`); the `vec4f` arms stay on vecmath, where four lanes pay.
Expected: AOT exp loop 9.2 to about 3.4 ns/iter on the 3990X (level with C++ and JIT), interp
about 1 ns/op, and interp/AOT scalar results bit-identical to the JIT's libm ones. A precision
sweep over [-20, 20) has vecmath within 5e-6 relative of `expf`, so the test corpus is the
only gate. dasllama is untouched by any of this: it is `-jit`-only and every hot `exp`/`pow`
there lowers to `@llvm.exp`/libm or to its own `exp4` polynomial.

`exp_est` as a JIT intrinsic: the guard-free inline polynomial (`build_vector_expf` minus the
overflow select tree that made it 1.12x scalar on znver2), clamped instead of guarded, inlines
and vectorizes 8-wide - sub-ns per element in array loops against 2.7 for libm. As a scalar
extern it is pointless (4.4 > 3.3 in the probe), which is what the benchmark row shows.

Direction (2026-08-30): the AOT is slower on the M1 too, so the follow-up is an AOT cleanup with
proper benchmarks - bind the CRT twins of every scalar math builtin in the dasTest module, where
the test cycle is short (no core rebuild), compare side by side in all three tiers (AOT,
interpreter, JIT), and keep the winners; `benchmarks/core/math/exp.das` and the two bound
functions stay.

## the M1 AOT exp-loop residual after the CRT switch

The 2026-08-30 re-profile: zen2 AOT exp loop 9215 -> 3412 us (level with JIT 3383 and C++ 3240),
but the M1's went only 3592 -> 3414 while its JIT and C++ sit at ~1650. The AOT TU verifiably
recompiled against the switched header, so exp is the CRT there - the residual is the AOT loop
shape itself: the `das_iterator<range>` machinery and the `rcp_est` lane round-trip are the
suspects, and neither shows on zen2 because MSVC's codegen was the bottleneck there. Wants its
own probe on the M1 (the family-probe pattern, loop shapes instead of math). The zen2 dictionary
JIT row swung +43% in the same capture - the known bimodal per-process lane, not a change.
