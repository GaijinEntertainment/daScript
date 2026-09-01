# Kernel parity pass - CPU and Vulkan kernels to same-speed-or-better

The arc after PR #3915 (the iquant format ladder). The bar is Boris's: **parity is >= 1.0x of
llama.cpp on the row; 0.95x is a gap.** The method is the compile-profile loop below - never a
model load, a mint, or a 48 s JIT re-emit per try.

Branch `bbatkin/kernel-parity`, stacked on the PR-1 tip; rebases onto master after that merge.

## 1. The procedure - three rigs, inner to outer

1. **Inner loop: the kernel microbench, seconds per try, no model.**
   `modules/dasLLAMA/benchmarks/matmul/kq_kernel_bench.das` - raw kernels, one thread, synthetic
   planes, every row of a kernel's `_variants()` registry timed. Under `DAS_TUNE_MODE=tune` that is
   the whole `[tune_perm]` grid (gemv and tile, since the gemv is the tile's `tune_companion`);
   under normal mode the stamped winner alone. `--perm <substr>` narrows the race (on zen2 the two
   VNNI seats decline and run the reference body - visible as reference-speed rows). Timing only:
   a spelling's correctness is `DAS_TUNE_MODE=test harness/gen_tune_probe.das`.
   ```
   DAS_TUNE_MODE=tune bin/Release/daslang.exe -jit modules/dasLLAMA/benchmarks/matmul/kq_kernel_bench.das -- --fmt iq3s --perm maddubs --ntok 0
   ```
2. **The reference row, isolated the same way.** llama.cpp's `test-backend-ops perf -o MUL_MAT`
   times one mul_mat per quant type; its default case is m=4096 rows, k=14336, n in
   {1,2,3,4,5,8,512} tokens - the microbench's default shape. Thread parity through the
   `GGML_BENCH_THREADS` define the harness patch carries (re-applied by hand at the reference tip
   6c84c7d5d; `harness/backend_ops_shapes.patch` no longer applies there):
   ```
   GGML_BENCH_THREADS=1 D:/Work/llama.cpp/build-clean-cpu/bin/Release/test-backend-ops.exe perf -b CPU -o MUL_MAT -p "type_a=iq3_s,type_b=f32,m=4096,n=1,"
   D:/Work/llama.cpp/build-vulkan/bin/Release/test-backend-ops.exe perf -b Vulkan0 -o MUL_MAT -p "type_a=q4_K"
   ```
   Vulkan has the same rig on our side in `harness/vk_gemm_probe.das` (`ref` arm = llama.cpp's own
   coopmat2 SPV blob inside our harness; `k6x flat` = compose without scale reads).
3. **Outer loop: the app, only when a spelling wins rig 1.** `-jit -module-cache
   .jitted_scripts/module_cache/<app>.dascache <app>.das -- --jit-split-modules=-1` (5 s warm,
   65 s after a kernel edit); `--tune-only <family>` re-crowns one family; `DAS_TUNE_POLICY=reference`
   is the A/B baseline. The stock monolith invocation runs once per landed kernel for the table
   row. `rm -rf .jitted_scripts` after an emitter edit (`dasllama_gemm_gen.das`) - QUIRK 21.

Rules that ride along: bench at `DAS_JOBQUE_THREADS=16`, tests at 8; one row at a time; one
proven fact per push; research before any kernel edit (the two memos below).

## 2. The fact base (zen2 3990X, one thread, m=4096 k=14336, us per run; ours = `dot_maddubs_width256_mr8`)

| format | ours gemv | llama.cpp n=1 | ours/theirs | ours tile n=512 | llama.cpp n=512 |
|---|---|---|---|---|---|
| k4 | 1969 (8.6 ns/sb) | 2421 | 1.23x | 120.9 GFLOP/s | 71.1 GFLOP/s (1.70x) |
| k2 | 2017 (8.8 ns/sb) | 2132 | 1.06x | - | - |
| iq3s | 11406 (49.7 ns/sb); sign=vec 7562 (33.0) | 10340 | 0.91x -> 1.37x | - | - |
| iq3xxs | 11578 (50.5); sign column + column read 6641 (29.0) | 6590 | 0.57x -> 0.99x | - | - |
| iq2s | 11732 (51.1); sign column + u64 pair 5039-5202 | 5074 | 0.43x -> 0.98-1.01x | - | - |
| iq2xs | 11490 (50.1); sign column + u64 pair + column read 4831 (21.1) | 5386 | 0.47x -> 1.11x | - | - |
| iq2xxs | 11061 (48.2); sign column + u64 pair 5121-5487 (noise band) | 5124 | 0.46x -> 0.93-1.00x | - | - |

The zen2 ladder after the k3/k2 step and the bench fixes (64-byte-aligned planes, normal scale bytes;
`harness/kernel_ladder.sh`, 2026-09-01, one thread, the stamped seat per format, best of 5 interleaved
rounds; ratio = reference / ours, >= 1.00 = ours faster):

| format | gemv ours us | gemv ref us | ratio | tile ours us | tile ref us | ratio |
|---|---|---|---|---|---|---|
| q8 | 3179 | 4325.63 | 1.36 | 576089 | 923211.00 | 1.60 |
| k4 | 1948 | 2391.08 | 1.23 | 547996 | 821706.50 | 1.50 |
| k5 | 3700 | 3264.03 | 0.88 | 583516 | 1311611.00 | 2.25 |
| k6 | 4204 | 3647.44 | 0.87 | 679148 | 1083505.00 | 1.60 |
| q40 | 1865 | 2839.84 | 1.52 | 387002 | 1018999.00 | 2.63 |
| q51 | 2824 | 4152.78 | 1.47 | 677326 | 1915920.00 | 2.83 |
| iq4xs | 2101 | 3357.24 | 1.60 | 671645 | 1645997.00 | 2.45 |
| k3 | 3092 | 2630.86 | 0.85 | 766074 | 1284702.00 | 1.68 |
| iq3s | 7668 | 10103.66 | 1.32 | 673687 | 5115105.00 | 7.59 |
| iq3xxs | 6601 | 6547.02 | 0.99 | 679775 | 3281595.00 | 4.83 |
| iq4nl | 2208 | 3027.06 | 1.37 | 592394 | 1125061.00 | 1.90 |
| k2 | 1370 | 1973.20 | 1.44 | 692253 | 770237.50 | 1.11 |
| iq2s | 5052 | 5069.15 | 1.00 | 711298 | 2513290.00 | 3.53 |
| iq2xs | 4506 | 5265.71 | 1.17 | 724580 | 2738442.00 | 3.78 |
| iq2xxs | 4859 | 5119.28 | 1.05 | 659785 | 2542052.00 | 3.85 |
| mx4 | 2205 | 3009.07 | 1.36 | 623427 | 1421391.00 | 2.28 |

k3 decode is the one row that disagrees with its own history (2445 us after its fix, 3092 here) - a
run-to-run spread like Intel's bimodal k6, under investigation.

The first zen2 ladder (before the k3/k2 step, unaligned planes, random scales):

| format | gemv ours us | gemv ref us | ratio | tile ours us | tile ref us | ratio |
|---|---|---|---|---|---|---|
| q8 | 3324 | 4186.70 | 1.26 | 698131 | 896060.00 | 1.28 |
| k4 | 2089 | 2539.11 | 1.22 | 493006 | 830068.50 | 1.68 |
| k5 | 3628 | 3286.40 | 0.91 | 512129 | 1344947.00 | 2.63 |
| k6 | 4203 | 3785.47 | 0.90 | 640544 | 1080212.00 | 1.69 |
| q40 | 1928 | 2918.57 | 1.51 | 380774 | 1004920.00 | 2.64 |
| q51 | 2816 | 4208.36 | 1.49 | 664024 | 1843932.00 | 2.78 |
| iq4xs | 2100 | 3396.72 | 1.62 | 721313 | 1682846.00 | 2.33 |
| k3 | 3317 | 2638.26 | 0.80 | 775632 | 1267418.00 | 1.63 |
| iq3s | 7671 | 10154.70 | 1.32 | 738713 | 5220515.00 | 7.07 |
| iq3xxs | 6622 | 6564.89 | 0.99 | 741832 | 3395150.00 | 4.58 |
| iq4nl | 2120 | 3134.77 | 1.48 | 599955 | 1076032.00 | 1.79 |
| k2 | 2146 | 2120.78 | 0.99 | 670297 | 783419.00 | 1.17 |
| iq2s | 5059 | 5083.57 | 1.00 | 790327 | 2545281.00 | 3.22 |
| iq2xs | 4554 | 5372.16 | 1.18 | 778490 | 2734115.00 | 3.51 |
| iq2xxs | 5143 | 5157.27 | 1.00 | 745463 | 2534210.00 | 3.40 |
| mx4 | 2572 | 3106.12 | 1.21 | 776635 | 1443088.00 | 1.86 |

After the k3/k2 step (c720aea95): k3 decode 2445 us (1.08-1.13x), k2 1286-1423 (1.37-1.65x).

The M1 ladder (Apple M1 Max, one thread, the sdot lattice at width 128, tip b88fc4100; ratio =
reference / ours): decode q8 1.46, k4 1.60, k5 1.06, k6 1.35, q40 1.96, q51 1.32, iq4xs 1.71, k3
1.37, iq4nl 1.82, k2 1.42, mx4 1.50, iq2s 1.00, iq3s 0.93, iq3xxs 0.72, iq2xxs 0.57, iq2xs 0.51;
every tile 1.9x-15.3x ahead. On ARM the grid formats are the tails - the reference exe's NEON grid
kernels run iq2xs/iq2xxs at 3.3-3.4 ms against our 6.0-6.4 ms while the k-quants are 1.06x-1.60x.

The zen4 ladder (AWS c7a.4xlarge, EPYC 9R14, one thread, the x86-vnni512 profile's seats =
dot_vpdpbusd_width512_mr16; gen_tune_probe TEST 65/65 ok on the first run of those seats): decode q40
3.87, iq4nl 2.82, q51 2.65, mx4 2.51, k4 2.25, iq4xs 2.13, k2 2.04, k3 1.88, q8 1.70, k5 1.28, k6 0.91,
iq3xxs 0.88, iq3s 0.81, iq2xxs 0.81, iq2xs 0.79, iq2s 0.77; every tile 2.1x-12x ahead. Where the dot is
the work, the 512-bit VNNI seat pays (k4 1.22x on zen2 -> 2.25x); the five grid formats lose ground
because the reference's grid kernels gain ~1.5x from AVX-512 while ours are bound by the scalar per-dword
gather (iq2s 4898 us on zen4 vs 5059 on zen2) - the ARM memo's diagnosis, on x86.
The Intel ladder (AWS c8i.4xlarge, Xeon 6975P-C Granite Rapids, one thread, the new x86-amx class -
its profile minted here and shipped; TEST 65/65 ok with the AMX leg; the bench with 64-byte-aligned
planes and normal scale bytes): decode q51 5.07, mx4 3.26, q8 3.21, iq4xs 2.99, q40 2.92, iq4nl 2.88,
k3 2.47, k2 2.20, k5 1.94, k4 1.54, iq2xs 1.15, iq3s 1.10, iq2xxs 1.10, iq3xxs 0.94, iq2s 0.91,
k6 0.39 (bimodal on this box: 2.1 ms in two runs, 5.8-6.7 in two - allocation-dependent past 64-byte
alignment, open); every tile 2.06x-14.2x ahead. The reference exe's own rows swung up to 2x between runs
on the VM (q8_0 4036 -> 7487), so single-run Intel ratios carry that error bar. Two bench artifacts were
found and fixed on this box: 16-byte-aligned planes (Intel splits a 64-byte load across a cache line -
k6 6703 -> 2234 us) and random scale bytes (denormal math - q8 tile 1022 -> 155 ms). Open: the bench's
tune-mode q8 tile SIGILLs on the AMX box even after invoking the witness variants; the tuner's own arming
works (the mint raced the AMX tiles), so the bench arms AMX differently from gen_tune_probe - find how.

Model level on the zen4 (Q4_K_M 1B, 16 threads, debug-jit rows): pp512 1172 vs 927 (1.26x), tg128 88.7
vs 86.0 (1.03x). The kernel ladder's k4 2.25x is against the reference's fallback vec_dot; a model run
uses its AVX2/AVX-512 repack 8x8 GEMV for q4_K - so the ladder's k4/k5/k6/q4_0/q8_0/iq4_nl/q2_K rows
overstate against real serving where the reference repacks (repack.cpp covers those eight types); the
grid formats and q3_K have no repack path and their rows are the fair ones.

zen2 reading: every tile row is ahead (1.17x-7.07x). Decode tails were k3 0.80x, k6 0.90x, k5 0.91x; iq3xxs, k2, iq2s,
iq2xxs at 0.99-1.00 (inside the noise band). The 4-bit class (q40, q51, iq4xs, iq4nl, k4, q8, mx4)
sits at 1.2x-1.6x.

Model level, zen2 16 threads, 1B vehicles, tg128 ours vs the clean-CPU llama-bench (before the pass):
IQ3_M (iq3s) 69.9 vs 56.5 = 1.24x (0.92x); IQ3_XXS-local 74.9 vs 74.3 = 1.01x (0.78x); IQ2_XS-local
93.8 vs 85.9 = 1.09x (0.70x); IQ2_XXS-local 88.6 vs 86.2 = 1.03x (0.70x); i1-IQ3_XXS (the mixed
iq2s/iq3xxs/iq3s vehicle) 71.9 +-4.2 vs 74.5 = 0.97x (0.76x). pp512 3.6x-5.0x throughout. Debug-jit
rows (`--for-debug-purposes`), 3 reps; the record-grade rows come with the released exe at arc end.

Reading: the five grid formats cost 48-51 ns per superblock regardless of what each decodes,
against 8.6-8.8 for k4/k2. A flat cost independent of the format is a shared mechanism, not five
decode problems. The model-level tg rows (0.70-0.92x) are this 2x kernel gap hidden behind memory
bandwidth; on the 1B vehicles it shows, on a 27B it mostly does not - but the kernel is the same.

### zen2 v3 - the arena bench (2026-09-01, `harness/kernel_ladder.sh all`, one thread, best of 5 interleaved rounds)

Every plane in ONE arena at fixed 64-byte-aligned staggered offsets; the reference is test-backend-ops at
6c84c7d5d with the GGML_BENCH_THREADS define. k6 read 5356 us here against 4.0-4.4 ms in the arena smoke -
its process-to-process swing is narrowed, not gone.

| fmt | decode ours / ref us | ratio | tile ours / ref us | ratio |
|---|---|---|---|---|
| q8 | 3142 / 4220 | 1.34 | 567862 / 907771 | 1.60 |
| k4 | 2005 / 2434 | 1.21 | 485803 / 846691 | 1.74 |
| k5 | 3467 / 3306 | 0.95 | 514648 / 1296856 | 2.52 |
| k6 | 5356 / 3683 | 0.69 | 634588 / 1076847 | 1.70 |
| q40 | 1901 / 2971 | 1.56 | 374388 / 1005240 | 2.69 |
| q51 | 2815 / 4107 | 1.46 | 662669 / 1830207 | 2.76 |
| iq4xs | 2076 / 3375 | 1.63 | 723720 / 1684220 | 2.33 |
| k3 | 2699 / 2700 | 1.00 | 733239 / 1245200 | 1.70 |
| iq3s | 7593 / 10345 | 1.36 | 743364 / 5194190 | 6.99 |
| iq3xxs | 6554 / 6524 | 1.00 | 739558 / 3293421 | 4.45 |
| iq4nl | 2126 / 3055 | 1.44 | 581628 / 1094468 | 1.88 |
| k2 | 1248 / 2158 | 1.73 | 636260 / 764748 | 1.20 |
| iq2s | 5044 / 5089 | 1.01 | 778057 / 2512394 | 3.23 |
| iq2xs | 4709 / 5320 | 1.13 | 799146 / 2692158 | 3.37 |
| iq2xxs | 4897 / 5086 | 1.04 | 738382 / 2537062 | 3.44 |
| mx4 | 2216 / 3113 | 1.41 | 618514 / 1441041 | 2.33 |

Decode below 1.0: k6 0.69, k5 0.95; at 1.0: k3 1.00, iq3xxs 1.00, iq2s 1.01. Every tile ahead (1.20-6.99).

M1 note: the ladder's reference must be `~/Work/llama.cpp/build/bin/test-backend-ops` (the fork checkout,
98c4764b6, carries the define); `build-cpu/` does not and ran every core - the ladder now refuses such a binary.

### M1 v3 - the full ladder after the ARM grid landing (2026-09-01, one thread, best of 5 interleaved rounds)

`~/Work/llama.cpp/build/bin/test-backend-ops` (the fork checkout at 98c4764b6, the define in). EVERY row at or
above parity - the M1 CPU is closed for this pass.

| fmt | decode ours / ref us | ratio | tile ours / ref us | ratio |
|---|---|---|---|---|
| q8 | 1059 / 1562 | 1.48 | 214129 / 434961 | 2.03 |
| k4 | 839 / 1343 | 1.60 | 268955 / 703992 | 2.62 |
| k5 | 2067 / 2185 | 1.06 | 252434 / 1129322 | 4.47 |
| k6 | 1777 / 2401 | 1.35 | 401658 / 1243030 | 3.09 |
| q40 | 863 / 1612 | 1.87 | 276856 / 531679 | 1.92 |
| q51 | 2075 / 2749 | 1.33 | 453329 / 1436227 | 3.17 |
| iq4xs | 988 / 1680 | 1.70 | 262229 / 870313 | 3.32 |
| k3 | 1925 / 2619 | 1.36 | 486461 / 1348956 | 2.77 |
| iq3s | 4914 / 5680 | 1.16 | 192154 / 2922617 | 15.21 |
| iq3xxs | 3572 / 4902 | 1.37 | 192186 / 2492096 | 12.97 |
| iq4nl | 1008 / 1847 | 1.83 | 290154 / 949701 | 3.27 |
| k2 | 1348 / 1915 | 1.42 | 357321 / 986778 | 2.76 |
| iq2s | 3259 / 4915 | 1.51 | 255732 / 2516761 | 9.84 |
| iq2xs | 2635 / 3249 | 1.23 | 255582 / 1669316 | 6.53 |
| iq2xxs | 2871 / 3435 | 1.20 | 192075 / 1765362 | 9.19 |
| mx4 | 1271 / 1900 | 1.50 | 331361 / 978109 | 2.95 |

Decode floor k5 1.06x; the grid formats 1.16-1.51x (from 0.51-0.93x before the row-pair decode). Tiles 1.92-15.2x.

## 3. Research memos (read before touching the kernels)

- `kernel_parity_research_cpu.md` - llama.cpp's CPU vec_dot for the five grid formats + Q2_K,
  x86 and arm64, against our emitter arms. Headline: every one of our five gathers ends each
  decoded dword with a 4-byte store into `te.iq3s_panel` and reloads 32 bytes spanning eight such
  stores - a wide load over narrow stores, which x86 does not forward. llama.cpp composes grid
  words into registers on both arches. The signs-on-activation half of followup 61 does not port
  to our 8-rows-per-vector layout; llama.cpp's arm64 arms put signs on the weights for the same
  reason. Ranked spellings: (1) `gather="reg"` - group-major compose, `insertelement` per row,
  no panel; (2) `psign="mask"` - a vector sign mask instead of GPR sign math + `llvm.abs`;
  (3) a `vpdpbssd` seat (AVX-VNNI-INT8 hosts, no generator change); (4) repack-baked parity sign
  byte (free for IQ2_XXS/IQ3_XXS). Incidental waste: IQ2_XXS reads its aux word as four i8 loads
  + shifts where IQ3_XXS uses one i32 load; IQ2_XS builds a u16 from two i8 loads.
- `kernel_parity_research_vk.md` - llama.cpp's cm2 `mul_mm` against our cm2 tile. Headline: the
  same kernel design (tile geometry, workgroup, decode-in-load callback, one coopMatMulAdd per
  k-step, zero barriers in the k loop, the same split-k heuristic) - the gap is not the tile.
  Real differences: llama.cpp hoists Q4_K/Q5_K scales into shared memory as ready `(d,m)` pairs
  (`shAscales`), ours re-extracts from a second SSBO with a variable shift per decoded element;
  its scales sit in the quant block, ours in a separate plane. Ranked: (1) split the end-to-end
  pp512 budget first - `GGML_VK_PERF_LOGGER=1` per-shape MUL_MAT rows against our `pfq_ts` roles
  decides whether the GEMM is even the gap (367 barriers per window in the chain); (2) the scale
  hoist (10-20% on k4/k5); (3) interleave the scale plane into the quant block; (4) pad N to the
  tile width; then narrow-tail accumulator, unroll, vec4 decode. Corrections: followup_vulkan 34's
  "same band" (it is 0.67-0.90x), "shared tier not decode" (Q8 rows sit at 1.00x+), and "tg holds
  parity" (IQ2_XS 0.54x) are stale. glslc here lacks NV_cooperative_matrix_decode_vector, so
  llama.cpp runs its scalar decode too - apples to apples today, and its vec4 twin is a widening
  waiting to happen.

## 4. Work queue (one item at a time; a row is done at >= 1.0x on rig 2's shape AND the vehicle)

CPU decode (gap 2) - the k-quant decode kernels were the tails the full ladder exposed (k3 0.80x,
k6 0.90x, k5 0.91x):
1. DONE k3 + k2: one i16 flush per sub-block (the per-16 chains are bounded at 4 x 1778 / 4 x 762) -
   k2 decode 2061 -> 1286-1423 us (reference 1951-2121); and k3's decode re-loads its 24 shared
   qs/hmask columns per sub-block (volatile loads, decode shape only) instead of CSE keeping them live
   and spilling - k3 2828-3198 -> 2445 us (reference 2638-2773). The pin hurt the tiles (k3 776 ->
   1109 ms) so it is decode-only; the principled form is the memo's D1 group-major reorder.
2. OPEN k6 0.87-0.90x, k5 0.87-0.91x: the pin gained nothing on k6; the memo (research_cpu_kquant.md)
   sees no clean lever for k5 (D4) and offers a sub-block unroll knob (D5) for both.
   2026-09-01, the k6 swing dissected (zen2, one thread, fresh process each, us): alone 4153 / 4225 /
   5468 / 4711; pinned to ONE core (start /affinity) 4040 / 5804 / 4220 / 4252 and 4138 / 4368 / 5632 /
   5200 - it follows the process, not the core. k4 (1999-2005), k5 (3590-3760), k3 (2648-2850) and the
   reference q6_K (3664-3768) do not swing. Arena base page-aligned (`--base-align 4096`): 4862 / 5971
   / 6052 / 5916, at 65536: 6024 / 6364 / 5607 / 5567 - eight of eight slow, where the heap's 64-byte
   draws run 4.0-4.3 two times in three. The sub-page phase sweep (`--base-offset` 0..3840 by 256, one
   process each) reads 4383-6305 with no band (3840 -> 4383, 3584 -> 6231). Reading: the per-process
   draw is the L1-set phase of k6's spill slots (the native stack, ASLR) against the planes; k6 has the
   largest live set of the four (memo: 2 qh + 4 ql + 8 chains). The engine's image planes start on 16 KiB
   boundaries (ARCHITECTURE_IMAGE.md 2.1a), i.e. the slow phase - the engine's k6 decode is ~0.61x, not
   the ladder's 0.69-0.87. Spills counted in the cached JIT DLL (`objdump -d` over the `.map` range of
   the `implementation` symbol): k6 72 ymm spill stores + 95 stack ymm reloads per superblock body (1224
   instructions, 64 maddubs) against k5 20 + 8 and k3 42 + 13. Pinning the qh column loads (volatile, the
   k3 cure) cut the stores to 27 but left 90 reloads and made k6 uniformly SLOW - engine phase 5537-5895,
   heap phase 5574-6078 (the 4.0-4.3 mode gone), tile 634 -> 669 ms - so it was reverted; the memo's
   "the pin gained nothing on k6" stands, and the fast mode is not the spill count. NEXT (its own session,
   with counters - AMD uProf on this box): what the 4.0 and 6.0 modes differ in, at `--base-align 4096`
   with `--base-offset` as the phase knob; only then D2's hoist / D5's unroll.
   2026-09-01, Boris's ruling - the .dlim is hardware-specific (Vulkan and Metal bake their own; the
   flavors never co-exist), so the CPU flavor's planes are the CPU kernels' to lay out. k6's qh half
   was GGUF's packing copied verbatim (four SUB-BLOCKS per byte), which is why the kernel kept eight qh
   columns live across four sub-blocks or reloaded them. LANDED: the qh plane re-packed per sub-block
   (column 2blk + half holds that sub-block's four j sites at 2j) - same 64 bytes, two qh loads per
   sub-block, uniform shift, nothing lives past its sub-block; six readers moved (repack, tile unpack,
   gemv arm, row dequant, generic tail dot, layout inverse), IMAGE_VERSION 27, JIT 0x5b. TEST 90/90,
   test_gguf_quant / test_repack / test_kquant green. zen2 k6 decode, us: engine phase 4444 / 4619 /
   4708 (was 4862-6364), heap phase 4749 / 4729 / 5427 (was 4040-5804); spills 72 + 95 -> 36 + 67;
   TEAM=16 wall 715 / 759 (unchanged, balance-bound); tile 659 ms (unchanged). The engine's k6 goes
   ~0.61x -> ~0.80x for free; the 4.0 ms mode is not reached - the remaining spills are the i16
   chains and broadcasts.
   LANDED k3 (a387ed230): qs per sub-block at 2j (two loads) and hmask one column per sub-block at bit
   s (one load) - three loads per sub-block, the volatile pin gone, same 96 bytes; IMAGE_VERSION 28, JIT
   0x5c; TEST 90/90 + the three module tests green. zen2 one thread, us: decode 2590 / 2925 / 3012
   heap phase, 2555 / 2825 / 3504 page-aligned, against 2648-2850 with the pin (reference 2701) -
   equal at one thread with a wider draw; the TILE 733 -> 686 ms (the pin was decode-only, the layout
   is not). Body 1027 instructions, 41 + 39 spill ops (was 1152, 42 + 13 with the pin). k5 is already
   site-major (one qh byte per row per site) - its cost is the byte -> lane expansion (shuffle + two
   masked tests per site); the k3 hmask shape fits it bit for bit: one column per sub-block, deposit
   `(hb << (4 - j)) & 0x10` / `(hb >> j) & 0x10` - six ops per site against ten plus a shuffle. NEXT.
3. Noise: the one-thread bench and the reference both drift ~5-8% run to run; interleaved rounds hold
   ours steady, the reference is re-run per table. iq3xxs/iq2s/iq2xxs tie at 1.00; k2 now clear.

CPU decode, the grid formats (done):
1. DONE-KILLED `gather="reg"` (measured 1.85x slower - see the ledger).
2. DONE all five: `sign="vec"` - the sign-byte column negates whole vectors; iq3s/iq2s load it
   off the plane, iq3xxs/iq2xs/iq2xxs synthesize it from the 7-bit codes (parity = the 8th bit),
   so no plane layout change (the repack-baked byte was killed by the plane map: grp bytes are the
   disk bytes, ~30 CPU/Vulkan/Metal sites read them).
3. DONE: the iq2 formats' u64 grid pair as one 8-byte load, half split in registers.
4. DONE: the vector sign column IS the gemv path - the `sign` knob and its 20 seats are gone. The
   tuner races the TILE (a gemv-only spelling ties there; `--tune-only iq3sq8_tile_gen` crowned the
   old seat), and the M1 (sdot lattice, one thread) showed no losses: iq3s 7383 -> 6105 us, iq2s
   7161 -> 5012, iq2xxs 7602 -> 6293, iq2xs 7878 -> 7081, iq3xxs 7516 -> 7452. Shipped profiles
   stay valid (same seat names); LLVM_JIT_CODEGEN_VERSION 0x5a re-keys the JIT caches.
5. DONE where it won: the column dword read with in-register byte extraction - iq3xxs 7862 ->
   6602 us and iq2xs 6501 -> 4702; it LOST on iq3s (7537 -> 8671), iq2s (5202 -> 6187) and iq2xxs
   (5121 -> 5574), which keep byte loads. The u16 single load for iq2xs measured slower (6991 vs
   6306) and was dropped.
6. Residue at one thread: iq3xxs 0.99x, iq2s 0.98-1.01x, iq2xxs 0.93-1.00x (the noise band of the
   1-thread bench is ~7%; a longer run or the 16-thread engine shape decides the last 2%). 7. the
   16-thread stamp on the vehicles (needs `--for-debug-purposes` on lcpp_bench, or the released
   exe). 8. retro audit of IQ4_XS/Q3_K per followup 60.

CPU decode, grid formats, BOTH ISAs (M1 0.51-0.93x, zen4 0.77-0.88x): the scalar per-dword gather is
the bound; the fix is the vectorized u64-lane compose (2 rows x 8 weights per NEON vector, 4 per 256-bit,
8 per 512-bit) with one pairwise add recovering the accumulator layout - one emitter change under the
gather branch, no plane change, retires the panel and the parity chain. k6 0.90-0.91 on both x86 boxes
stays open.
1. DONE ARM (2026-09-01, two commits): the row-pair decode under the sdot lattice + the ksigns +-1
   table rows + the whole-u64 grid load - M1 iq2xs 1.24x, iq2xxs 1.20x, iq3xxs 1.36x, iq2s 1.51x,
   iq3s 1.15x (section 2, the LANDED notes). The x86 emission is byte-identical (gated DOT_SDOT).
2. DONE-KILLED x86 (2026-09-01): the row-QUAD form measured on zen2, us, panel -> rows: iq2xs 4646 ->
   6785, iq2s 5056 -> 6826, iq3s 7612 -> 11576, iq3xxs 6589 -> 7995, iq2xxs 5144 -> 7142 (TEST 90 ok, the
   form is correct). llama.cpp's own AVX2 grid path IS this insert form and runs iq2xs at 5320 there - so
   the insert shape loses to the panel on zen2 AND ours is slower than theirs in the same shape (the sign
   table's four scalar loads + inserts where theirs shuffles; the per-row index math). It cannot close
   zen4's 0.77-0.88x by being the reference's shape; the emitter keeps it for the sdot lattice only
   (`grid_rows_path`). zen4 needs a profile first (which port saturates) - item 3.
   The plan that was: four rows x 8 weights per 256-bit vector
   (four u64 grid loads, vmovq/vpinsrq/vinserti128), the activation's 8 bytes broadcast per lane
   (vpbroadcastq) and SIGNED per row by `vpsignb` against the same +-1 table rows - maddubs needs
   its unsigned operand, so on x86 the signs go on the activation copy, which the 4-rows-per-vector
   layout allows where the 8-rows-per-vector panel layout did not. Then vpmaddubsw + the existing
   fold. The zen2 `gather="reg"` that measured 1.85x slower was insertelement PER DWORD with GPR
   sign math; this is four qword inserts and no sign arithmetic. Measure on zen2 first (all five
   at 1.00-1.36x there - it must not lose), then zen4 (rent) where the gap lives.
3. NEXT x86 zen4 grid 0.77-0.88x: rent the c7a again with `perf stat` (uops per port, store-forwarding
   stalls) on iq2xs/iq3xxs decode against the reference exe's same shape - one proven fact before any form.

CPU decode on ARM (the M1 ladder): iq2xs 0.51x, iq2xxs 0.57x, iq3xxs 0.72x, iq3s 0.93x. The memo
(`kernel_parity_research_arm.md`): the sdot count is at parity (2 per 32 weights per row, both sides);
the gap is the surrounding work - the parity chain that rebuilds the 8th sign bit for the three ksigns
formats (+3.25 cycles per row per 32 weights: exactly iq2xs/iq2xxs/iq3xxs), the u32 grids' doubled
loads, and the alloca panel, which in decode (tokCount 1) buys back nothing; a port model reproduces
all five M1 numbers within 10%. THE FIX: a 2-rows x 8-weights-per-vector decode form under the sdot
lattice - non-indexed sdot with the activation as `ld1r {v.2d}`, one `addp` per row pair recovers the
`<4 x i32>` accumulator the fold already takes; zero x86 exposure (inside the gather branch, gated on
DOT_SDOT), and it makes llama.cpp's +-1 sign table (`keven_signs`) reachable, retiring the parity chain.
Model: iq2xs ~2570 us = 1.27x. vqtbl gathers are impossible (1-2 KB grids vs tbl's 64 bytes); our
masked negate already beats llama.cpp's own mask expansion. Fallback if deferred: plane-carried sign
bytes (size-neutral for iq2xxs/iq3xxs, +6% for iq2xs; touches the plane, both ISAs) -> 0.72-0.98x only.
Also from the memo: the column-read-vs-byte-read decision was made on x86 and should be a perm seat.
LANDED 2026-09-01 (the row-pair decode under the sdot lattice, gated DOT_SDOT + decode shape + width
128; x86 untouched): M1 one thread, us, before -> after (reference): iq2s 4932 -> 3256 (4917, 1.51x),
iq3s 6089 -> 4937 (5682, 1.15x), iq2xs 6375 -> 4244 (3274, 0.77x), iq2xxs 6063 -> 4085 (3437, 0.84x),
iq3xxs 6735 -> 5235 (4867, 0.93x); TEST 65/65 on the M1.
LANDED 2026-09-01, step two - signs and grid words: the ksigns formats take a +-1 table row per 7-bit
code (keven_signs form, [128 x i64]) and the iq2 formats load the u64 grid entry whole. M1 us, after
(reference): iq2xs 2636 (3274, 1.24x), iq2xxs 2873 (3437, 1.20x), iq3xxs 3576 (4867, 1.36x), iq2s 3260
(4917, 1.51x), iq3s 4929 (5682, 1.15x); TEST 90 ok. All five grid decodes ahead on the M1. LESSON: the
table for the plane-byte formats (iq3s/iq2s) REGRESSED them (iq2s 3256 -> 4167, iq3s 4937 -> 5608) -
four scalar loads per pair replaced one 16-byte column load plus four NEON ops, and the M1 sustains 3
loads against 4 NEON ops per cycle; those two keep the column mask. Count loads AND NEON ops per pair,
the bound is whichever is fuller.

Bench on AMX boxes: the tune-mode q8 tile SIGILL (above) - read how gen_tune_probe reaches the grant
(q8q8_family_live_variants + the amx cfg companion?) and do the same; k6 bimodality on Granite Rapids.

Vulkan pp (gap 1):
1. Budget split (measurement, no kernel edit) - is the GEMM the 30%? 2. `shAscales`-style scale
   hoist; 3. scale plane interleave; 4. N padding; 5+. the rest of the memo's list.

Vulkan grid tg (gap 3): followup_vulkan 35's levers, after gap 1 or 2 lands.

## 5. Ledger

- 2026-09-01: CPU item 1 measured and killed - `gather="reg"` (compose the weight vector with
  insertelement, no panel) ran iq3s decode at 90.6 ns/sb against the panel's 49.1; the emitted IR
  had 1025 insertelement chains and a 9904-instruction straight-line body against the panel loop's
  1893 (every load unique, so CSE was not the problem). The panel round trip is not the bottleneck.
- 2026-09-01: CPU item 2 landed as `sign="vec"` on iq3s and iq2s, the two formats whose sign bytes
  sit in the plane at row stride 4: the panel holds raw grid words, one 32-byte column load per
  block, a constant shuffle + and + cmpeq builds the byte mask, xor + sub negates the vector.
  One thread, m=4096 k=14336: iq3s 11581 -> 7732 us (1.34x of the reference's 10340), iq2s 12413
  -> 7076 us (0.72x of 5074, was 0.41x). Every variant bit-exact in gen_tune_probe TEST mode. The
  five gather emitters collapsed into one gather over per-format decode functions (-110 lines).
- 2026-09-01: research_cpu_kquant.md (llama.cpp q3_K/q5_K/q6_K vs ours): op counts are at parity; what
  tracks the losses is the high-bit plane's column vectors staying live across the superblock after
  CSE (k3 24 vectors, k6 16) against 16 ymm - the plane cannot be register-resident in our
  8-rows-per-vector layout the way it is in llama.cpp's row-major loop. Also: llama.cpp's q4_K has an
  AVX2 repack GEMV that test-backend-ops perf does not run, so the k4 1.22x is against its fallback.
- 2026-09-01: the lab completes - kq_kernel_bench carries all 16 formats (q8 + mx4 on the q8q8
  grid, q51 on its per-32 planes; the tile arm hands k5/k6 and the grid formats their byte-expanded
  panel per kq_reads_packed_planes), rows interleaved round-robin with best + median;
  harness/kernel_ladder.sh joins both sides into the per-box table (above).
- 2026-09-01: the 16-thread vehicle stamps (above) - every grid-format vehicle at or past the
  reference on tg except the mixed i1-IQ3_XXS at 0.97x with a +-6% error bar.
- 2026-09-01: the column dword read lands for iq3xxs and iq2xs only (measured per format, see the
  queue); zen2 one thread now: iq3s 7673, iq3xxs 6641, iq2s 5039-5174, iq2xs 4831, iq2xxs
  5281-5487 us; 65 variants ok in TEST mode.
- 2026-09-01: the sign knob collapsed into the one gemv path (-268/+58 lines in the emitter, the
  smask/ksigns globals gone); the plain seats now measure iq3s 7537, iq3xxs 7862, iq2s 5202, iq2xs
  6501, iq2xxs 5121 us; 65 variants ok in TEST mode; test_kquant 265 passed.
- 2026-09-01: sign=vec for iq3xxs/iq2xs/iq2xxs via the parity-synthesized column (no layout
  change) and the u64 grid pair load for the three iq2 formats: iq2s 5152 us (0.98x), iq2xxs 5209
  (0.98x), iq2xs 6297 (0.86x), iq3xxs 7893 (0.83x), iq3s 7562 (1.37x); 85 variants ok in TEST mode.
- 2026-09-01: step 0 done - `kq_kernel_bench.das`, the reference rows, both memos, the fact base
  above. `test-backend-ops` built in `build-clean-cpu` and `build-vulkan` with the thread pin.
