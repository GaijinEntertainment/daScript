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
pow mirror `v_exp2`, `v_log2_est_p5`, `v_log` and `v_pow`. The gates: log2 and log take the
rail on every target (`vmath_vector_float`) - masks and Horner steps with no guard branch; on
x64 `log2(float4)` runs in 2.4 ns per vector against 15-20 ns for four scalarized `log2f` calls
(`log` 2.8 against 17-19) - and pow always did, riding that log2 plus exp2's clamp; the rest
takes it by measurement, target by target (`vmath_poly_gate`, `vmath_exp_gate`,
`vmath_hyper_gate`): aarch64 all of it, a wasm target all but `cosh`, x64 all but `sinh` and
`cosh`. `exp` has two shapes (`vmath_exp_gate`). Where the range guard is cheap - aarch64, and
x64 with AVX-512 - it is the range-guarded polynomial (`build_vector_expf`), 3.9e-6 relative off the
interpreter: 2.1 ns per vector on an M1 against 2.2 for the other shape, 1.8 against 2.3 on a
Zen 4. Every other target takes the interpreter's own: `v_exp` is `exp2(x * log2(e))`, so the
exp2 emitter serves it and the JIT's bits are the interpreter's - 2.4 ns against the guarded
shape's 4.4 under the AVX2 class, 3.9 against 8.3 on a Zen 2, 3.3 against 8.0 in wasm.
In nanoseconds a float4 vector, four scalar calls against the rail - a
Zen 4 box (glibc) under its own AVX-512 and under the AVX2 class, a Zen 2 box (the MSVC runtime),
and wasm64 under V8:

| entry | Zen 4 calls | AVX-512 | AVX2 class | Zen 2 calls | Zen 2 rail | wasm calls | wasm rail |
|---|---|---|---|---|---|---|---|
| `sin` | 8.9 | 3.1 | 3.4 | 21.8 | 5.3 | 20 | 3.8 |
| `cos` | 9.7 | 3.2 | 3.5 | 20.5 | 5.6 | 20 | 3.7 |
| `tan` | 15 | 4.4 | 5.4 | 31.4 | 8.2 | 28 | 5.9 |
| `exp2` | 8.9 | 2.2 | 2.2 | 10.3 | 3.5 | 10.9 | 3.1 |
| `tanh` | 33 | 3.7 | 8.4 | 39.4 | 11.4 | 34 | 17 |
| `exp` | 8.9 | 1.8 | 2.4 | 9.0 | 3.9 | 9.1 | 3.3 |
| `sinh` | 35.5 | 4.6 | 12.9 | 23.8 | 26.8 | 34 | 23 |
| `cosh` | 17.6 | 3.8 | 11.9 | 22.4 | 24.7 | 17 | 21 |

`sinh` and `cosh` pay the exp polynomial's range guard twice. Those two keep the scalar calls
on x64: Zen 4 gains on them, Zen 2 is behind, and a class cannot tell the two boxes apart. On a
wasm target `cosh` loses outright.
Scalar float and double keep the libm intrinsic on every target: libm is correctly
rounded and one scalar call carries no scalarization penalty.

### 8.1 Fusion is part of the polynomial {#vector-poly-fusion}

A Horner step vecmath writes as `v_add(v_mul(..))` is emitted unfused - `vmath_poly_step`, an
fmul then an fadd - and only the chains vecmath writes as `v_madd` / `v_nmsub` go through
`vmath_fma`. The exp2 and log2 chains alternate sign heavily enough that one contracted step
moves the result by several ulp, so emitting `@llvm.fmuladd` for them costs bit-exact agreement
with the interpreter and AOT: measured over 200k lanes, unfused is identical and fused is up to
1.9e-6 apart. Where vecmath does fuse, fusion is load-bearing rather than optional - the sincos
Cody-Waite reduction rounds `x - qf*KC1` into noise at |x| ~ 1e5 without it.

A target with no fused multiply-add - wasm, an x64 class without FMA - emits `@llvm.fmuladd` as a
multiply and an add. There `sin` and `cos` stay within 1e-6 of libm for |x| up to 20, within 3e-5
for |x| up to 1000 and within 4e-3 at |x| ~ 1e5, the reduction's rounding growing with |x|, and
`tan`, `exp2` and the hyperbolics hold their aarch64 bounds. An x64 target with FMA fuses, and its
`sin` and `cos` are within one ulp of libm over |x| up to 1e5.

The interpreter's fusion is its build's: vecmath's `v_madd` is an FMA on aarch64 and in an MSVC x64
build with AVX2, a multiply and an add in a clang or gcc x64 build. The emitted `sin`, `cos` and
`tan` are the interpreter's bit for bit where the two fuse alike; where they do not, they differ
by the unfused side's error. `exp2`, which fuses nowhere, is bit for bit on every pairing.

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
out-of-range input is poison. The guarded `exp` is the one member of the rail that still
diverges on NaN: it answers NaN for `exp(NaN)` where the interpreter, and the exp2-shaped `exp`
of every other target, answers inf.

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
