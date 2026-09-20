# dasLLAMA Architecture - the refuted kernel shapes

Companion to `ARCHITECTURE_GPU.md`; section numbers are `ARCHITECTURE.md`'s. This document
carries section 2.2b: the tensor-GEMM and fused-attention shapes that measured out, the retained
reference arms beside them, and the sanctioned float-A stamps. The GPU backend role table this
section builds on stays in `ARCHITECTURE_GPU.md` sec.1.5.

### 2.2b Tensor-GEMM shapes that measured out (M5, interleaved-race evidence)

The forms below were built, raced against the crowned tg-staged q8u GEMM at real model
shapes, and LOST; the probe that holds the GEMM numbers is
`benchmarks/matmul/bench_metal_nax_probe.das`, and the fused-attention numbers live in
`benchmarks/attn/bench_metal_pf_fused_attn.das`. Re-attempting one without new structure is
re-buying a measured loss.

The probe also RETAINS the decided-and-shipped arms (the half-A stream, the dev-W all-device
form, the tall M-tile twin, the bk staging depths, the no-zero-init form) as hand-written MSL
reference implementations beside the refuted ones - they are the arc's bisect ledger, and the
sync duty is `REVIEW_GPU_RACE.md`'s. The attention lab keeps the same class of retained references:
its PORT/PORTH/PORTC arms and `benchmarks/attn/lcpp_flash_dk72.metal` are the oracle-exact
external reference the shipped `MetalTowerFlash` was decided against, and the DIAL arm races
the production dialect against them - the bisect seat when the flash regresses.

- **Per-simdgroup register-fragment matmul2d (16x32x16, device -> `vec<T,8>` fragments):**
  1.7-2.0x slower for weight GEMMs, vectorized loads and deep n-blocking included. The
  fragment architecture pays for attention's streaming operand reuse (one resident Q against
  a K/V walk), not for a GEMM's operand traffic - cooperative tg staging wins there.
- **Mixed-integer matmul2d operands (float x int8, and the i8 x i8 per-block-fold form):**
  3.5x slower - the mixed-int combinations exist in MPP's type lists but lower off the NAX
  fast path. W8A8 claims from other stacks do not transfer through MPP.
- **Matrix forms for the K-quant small-batch GEMV (2-8 verify columns), against the
  four-column twin:** the simdgroup 8x8 multiply-accumulate runs at plain FMA rate on this GPU
  (its tile loop is 65% of the kernel; without it the dequant-to-half stage alone is 1.5x a
  single pass), so it buys 20-25% per column; the Metal-4 tensor op at an m = 8 tile is nearly
  free per column but the dequant stage is the cost - 3.4x a single pass for eight columns where
  the twin does four at 2.7x. Neither moves 3-4 rows; the tensor form beats the eight-column twin
  by 40% at 5-8 rows. Arms `k4_mm8*`, `k4_tmm*`, `k4_tmv8*` in `benchmarks/matmul/bench_metal_gemv_kernels.das`
  (the `_depths` ruler records say no depth reaches those rows).

**Sanctioned float-A stamps** - the kernel classes stamped `[metal_kernel(float_a_ok=true)]`:
every tensor template's `XT = float` stamp - the live fallback wherever the half panel is absent
(below the convert row floor, panel does not fit, half-X pinned off) - the
batch-decode/classifier `MetalQ8GemmTensorT` family, whose half-X extension is an open ledger
item, and the double-buffered `*Db` staging stamps (`MetalQ8MulMmTensorT`'s `BK = 128` stamps,
`MetalKqMulMmK45DbT`, `MetalKqMulMmK6DbT`), which pin `XT = float16` today - there the flag is scaffolding a future
float stamp would need, not a live float operand - and the verify-width lab template
`MetalKqLabK4Tmv8T` in the GEMV lab, whose A operand is the decode driver's f32 x panel.
- **Fused single-kernel attention (scores in threadgroup, online softmax):** loses 10-80% to
  the pipelined three-pass at real shapes (`benchmarks/attn/bench_metal_pf_fused_attn.das`) -
  Metal's cross-kernel pipelining plus full-width softmax beat tg-scope fusion.

The positive laws these races established - half operands, stage-only-to-transform, consecutive staging
runs, relaxed_precision always - are `REVIEW_GPU.md` rules and the `modules/dasMetal/REVIEW.das` descriptor
gate; this section keeps only the refuted shapes and why they lose.
