# dasLLVM Architecture - the inline-polynomial vector math rail

Companion of `ARCHITECTURE.md` (sec.8 routes here). Contract: `ARCHITECTURE_COMMON.md` (repo
root). Section 8 and its subsections keep their numbers from the parent.

## 8. The inline-polynomial rail {#vector-poly-rail}

`math::exp`, `sin`, `cos`, `tan`, `exp2`, `log2`, `log`, `pow` and the three hyperbolics on a
float VECTOR type are emitted as inline IR by the `build_vector_*` emitters in
`llvm_jit_intrin.das`. The default lowering is `@llvm.<op>.vNf32`, which scalarizes to N libm
calls on any target without a vector libm - all of ours. Each emitter replaces that call with
the SAME polynomial the interpreter and AOT already run (vecmath, `include/vecmath/`), written
as generic vector IR (`fmuladd`, `trunc`, `roundeven`, `fptosi.sat`, integer masks and selects)
that the backend lowers to one vector instruction apiece - so the three rails agree instead of
merely being close (sec.8.3 is the one family that cannot). sin and cos mirror `v_sincos`
(quadrant = round(x*2/pi), the two-constant Cody-Waite reduction, a degree-3-in-x^2 pair); tan
mirrors `v_tan` (4/pi octants, three reduction constants, its own minimax); exp2, log2, log and
pow mirror `v_exp2`, `v_log2_est_p5`, `v_log` and `v_pow`. Two gates: log2 and log take the
rail on every target (`vmath_vector_float`) - masks and Horner steps with no guard branch; on
x64 `log2(float4)` runs in 2.4 ns per vector against 15-20 ns for four scalarized `log2f` calls
(`log` 2.8 against 17-19) - and pow always did, riding that log2 plus exp2's clamp; the rest
is aarch64-only (`vmath_aarch64_poly_gate`) by measurement, since the range guards of `exp2`,
`sin`, `cos` and `tan` cost more on x64 than the scalar calls they replace. Scalar float and double keep the libm intrinsic on every target: libm is correctly
rounded and one scalar call carries no scalarization penalty. exp keeps its ggml polynomial
(3.9e-6 relative off the interpreter); routing it through the exp2 emitter would make it exact.

### 8.1 Fusion is part of the polynomial {#vector-poly-fusion}

A Horner step vecmath writes as `v_add(v_mul(..))` is emitted unfused - `vmath_poly_step`, an
fmul then an fadd - and only the chains vecmath writes as `v_madd` / `v_nmsub` go through
`vmath_fma`. The exp2 and log2 chains alternate sign heavily enough that one contracted step
moves the result by several ulp, so emitting `@llvm.fmuladd` for them costs bit-exact agreement
with the interpreter and AOT: measured over 200k lanes, unfused is identical and fused is up to
1.9e-6 apart. Where vecmath does fuse, fusion is load-bearing rather than optional - the sincos
Cody-Waite reduction rounds `x - qf*KC1` into noise at |x| ~ 1e5 without it.

The interpreter side of that bit-exactness is a precondition the emitters cannot enforce: it holds
while the host compiler does not contract vecmath's POLY macros itself. clang's default `-ffp-contract=on`
contracts only inside one source expression, so the inlined `v_add(v_mul(..))` pair stays two instructions;
GCC's default `fast` contracts across statements and would fuse them. CMake pins neither flag, so on a
GCC-built interpreter it is the vecmath rail that moves, not the emitted one.

NaN carries lane for lane on this rail, and that has to be built in: a clamp or a float-to-int
conversion written with ordered compares replaces a NaN lane with a number, and no accuracy bound
can see the substitution because a bound only reads lanes that produced a number. So `tanh` selects
its operand back over its `[-9,9]` clamp through `fcmp uno`, and the sincos quadrant and the tan
octant convert through `llvm.fptosi.sat` rather than `fptosi`, whose result for NaN and for
out-of-range input is poison. `exp` is the one member of the rail that still diverges on NaN: the
JIT answers NaN for `exp(NaN)` where the interpreter answers inf.

### 8.2 log2 is an estimate, and its specials are not IEEE {#vector-log2-estimate}

`build_vector_log2` mirrors vecmath's `v_log2_est_p5`: the exponent field gives the integer part,
the mantissa is forced into [1,2) and fed to a degree-5 minimax, and the `p*(m-1)` shape is what
makes log2(1) exactly 0. It is an ESTIMATE (3.7e-5 relative at worst, just below x == 1 where that
combine cancels) the interpreter and AOT have always used, so the JIT matching it is the point: the
tiers agreeing is the property that wins, and a better log2 goes into vecmath for every tier, never
into one rail. `math::log` is it scaled by ln2 and `math::pow` is `exp2(log2_est(x) * y)`, so both
inherit the error, pow amplified by |y|. The estimate reads the exponent through a mask and never
produces -inf or NaN: log2(0) is -127 and log2(-x) == log2(|x|); `pow_est(-2, 3)` is +8 for the
same reason, and `pow` puts the sign back for an odd integer exponent (`v_pow_signed`, the
emitter's XOR) to answer libm's -8 - all on every tier and target, since the three take the rail
everywhere; a caller that needs IEEE's -inf and NaN has the scalar `log2`.
`tests/llvm_vector_math.das` pins the bounds and the special values.

### 8.3 The hyperbolics have no interpreter twin {#vector-hyperbolic-divergence}

vecmath carries no vector sinh/cosh/tanh, so `SimPolicy` binds `vsinh`/`vcosh`/`vtanh`
(`aot_builtin_math.h`), which call libm once per lane. `build_vector_hyper` builds all three on
`build_vector_expf` instead, so on this one family the JIT diverges from interp and AOT by the
exp polynomial's error rather than agreeing with them - the opposite trade from every other
emitter on the rail, taken because the consumer (GELU over float4 rows) otherwise pays four
libm calls per vector. `tests/llvm_vector_math.das` asserts the size of that divergence.
