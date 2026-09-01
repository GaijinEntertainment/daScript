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

### M1 v4 - closing tables (2026-09-01, after the k6/k3 transposes; one thread at the engine phase, and the engine shape: 8 lanes, d=32768)

| fmt | decode ours / ref us | ratio | engine 8 lanes ours / ref us | ratio |
|---|---|---|---|---|
| q8 | 1059 / 1537 | 1.45 | 4187 / 4519 | 1.08 |
| k4 | 840 / 1343 | 1.60 | 2240 / 2271 | 1.01 |
| k5 | 2068 / 2184 | 1.06 | 2757 / 2715 | 0.98 |
| k6 | 1699 / 2405 | 1.42 | 3224 / 3284 | 1.02 |
| q40 | 768 / 1623 | 2.11 | 2176 / 2231 | 1.03 |
| q51 | 2075 / 2752 | 1.33 | 2922 / 3145 | 1.08 |
| iq4xs | 987 / 1699 | 1.72 | 2234 / 2125 | 0.95 |
| k3 | 1876 / 2621 | 1.40 | 2016 / 2831 | 1.40 |
| iq3s | 4924 / 5714 | 1.16 | 5282 / 6096 | 1.15 |
| iq3xxs | 3813 / 4876 | 1.28 | 3949 / 5208 | 1.32 |
| iq4nl | 1036 / 1828 | 1.76 | 2175 / 2272 | 1.04 |
| k2 | 1390 / 1918 | 1.38 | 1463 / 2138 | 1.46 |
| iq2s | 3255 / 4932 | 1.52 | 3669 / 5366 | 1.46 |
| iq2xs | 2637 / 3261 | 1.24 | 2835 / 3490 | 1.23 |
| iq2xxs | 2913 / 3440 | 1.18 | 3112 / 3718 | 1.19 |
| mx4 | 1272 / 1910 | 1.50 | 1990 / 2147 | 1.08 |

One thread: every decode >= 1.06 (k5 the floor; k6 1.42, k3 1.40). Engine shape: every format >= 0.95 (iq4xs),
most 1.0-1.5. The M1 CPU is closed at both shapes.

### Intel v2 - Granite Rapids (2026-09-01, c8i.4xlarge Xeon 6975P-C, class x86-amx, branch 095f4030c; TEST 90/90)

The bootstrap's two ladders both ran the new default (engine shape: 16 lanes, d=32768) - the one-thread table and
the knob A/B run in round two.

| fmt | engine 16 lanes ours / ref us | ratio |
|---|---|---|
| q8 | 4539 / 4835 | 1.07 |
| k4 | 2332 / 2849 | 1.22 |
| k5 | 3073 / 3862 | 1.26 |
| k6 | 5230 / 3587 | 0.69 |
| q40 | 2345 / 4923 | 2.10 |
| q51 | 3196 / 5238 | 1.64 |
| iq4xs | 2367 / 4355 | 1.84 |
| k3 | 2703 / 2579 | 0.95 |
| iq3s | 7310 / 7575 | 1.04 |
| iq3xxs | 5396 / 6121 | 1.13 |
| iq4nl | 2359 / 5928 | 2.51 |
| k2 | 1229 / 1598 | 1.30 |
| iq2s | 4283 / 3786 | 0.88 |
| iq2xs | 3962 / 4579 | 1.16 |
| iq2xxs | 3805 / 3853 | 1.01 |
| mx4 | 2467 / 5802 | 2.35 |

The reference does not reach this box's DRAM on most formats (q4_0 4.9 ms for 264 MB = 54 GB/s) where ours does
(2.3 ms = 115 GB/s) - hence 2.1-2.5x on q40/iq4nl/mx4. Tails: k6 0.69 (5.2 ms against 3.5 - the reference
saturates DRAM on q6_K, our k6 stays compute-bound; the vpdpbusd512 seat has no i16 chain for the fused flush),
iq2s 0.88, k3 0.95.

Round two, one thread at the engine phase (branch eb94f1d9a):

| fmt | decode ours / ref us | ratio | tile ours / ref us | ratio |
|---|---|---|---|---|
| q8 | 2224 / 2687 | 1.21 | 145999 / 578161 | 3.96 |
| k4 | 1140 / 1748 | 1.53 | 263222 / 787687 | 2.99 |
| k5 | 1525 / 2447 | 1.60 | 175535 / 1133333 | 6.46 |
| k6 | 1840 / 2040 | 1.11 | 239252 / 834122 | 3.49 |
| q40 | 1108 / 2288 | 2.06 | 175513 / 651009 | 3.71 |
| q51 | 1488 / 2946 | 1.98 | 214319 / 1410780 | 6.58 |
| iq4xs | 1126 / 2380 | 2.11 | 272987 / 1144376 | 4.19 |
| k3 | 1277 / 2266 | 1.77 | 331164 / 1115864 | 3.37 |
| iq3s | 5207 / 6783 | 1.30 | 252732 / 3499939 | 13.85 |
| iq3xxs | 4505 / 5053 | 1.12 | 252332 / 2480068 | 9.83 |
| iq4nl | 1122 / 2686 | 2.39 | 263188 / 717486 | 2.73 |
| k2 | 716 / 1468 | 2.05 | 356125 / 688367 | 1.93 |
| iq2s | 3996 / 3709 | 0.93 | 272607 / 1851295 | 6.79 |
| iq2xs | 3955 / 4342 | 1.10 | 272793 / 2184555 | 8.01 |
| iq2xxs | 3675 / 3760 | 1.02 | 252192 / 1799256 | 7.13 |
| mx4 | 1285 / 2990 | 2.33 | 273869 / 1247422 | 4.55 |

One thread: every decode >= 1.02 except iq2s 0.93; k6 1.11. Tiles 1.9-13.9x.
The seat races (tune mode, every registry row; us): k6 one thread maddubs256 1648 / vpdpbusd256 1636 /
vpdpbusd512 1615 (reference 1952), at 16 lanes d=32768 3489 / 3424 / 3419 against the reference's 3587 -
0.98-1.05x, so the bootstrap ladder's k6 0.69 was that first run's artifact, not the kernel (round three
re-measures it cold). k3: 1246 / 1125 / 922 alone (reference 2258), 1780 / 1797 / 1525 at 16 lanes
(2579). iq2s: 4060 / 3855 / 3957 alone (3702), 4242 / 4090 / 4208 at 16 lanes (3786) - the crowned width-512
seat loses 3% to vpdpbusd256 at the engine shape, as on zen4: the gemv inherits the tile's crown
(tune_companion), and the tile and the gemv do not agree on width. The knob A/B (the table-sign row form):
iq2s 5345, iq2xs 5033, iq2xxs 3754-3820, iq3s 9763, iq3xxs 5606 against the panel's 3956 / 3902 / 3904 /
5276 / 4601 - the panel everywhere but iq2xxs (marginal); the column-sign form is round three's question.
Round three (f68534c06, the column-sign row form), one thread panel -> rows: iq2xxs 3678 -> 2518 (reference 3708:
1.47x), iq3xxs 4504 -> 4392 (5092: 1.16x); iq2s 3994 -> 5428, iq2xs 3962 -> 5504, iq3s 5209 -> 9785 stay panel.
At 16 lanes: iq2xxs 3811 -> 2614 (3853: 1.47x), iq3xxs 5448 -> 4515 (6121: 1.36x), the other three worse. Eight
lanes (one per core) against 16: k6 6562 -> 3420-5219, k3 3720 -> 2723, iq2s 4431 -> 4287, iq3s 9278 -> 7339 -
more lanes help every format here, 16 stands. LANDED (d9928f985): the x86-amx class takes the row form for iq2xxs
and iq3xxs; the default verified on the box - TEST 90/90, iq2xxs 2473-2496 alone / 2606 at 16 lanes (1.49x /
1.48x), iq3xxs 4390 / 4531 (1.16x / 1.35x), iq2s 3965-3991 / 4280 (0.93 / 0.88), iq2xs 3927-3973 / 3926 (1.10 /
1.17), iq3s 5081-5182 / 7254 (1.30 / 1.05), k6 at 16 lanes 5316 in normal mode against 3419-3489 for the same
stamped seat in the tune-mode race - a mode difference, not the kernel (the back-to-back probe runs next).
The probe: same 16 lanes, same 128 x 256-row split, same seat - 5270 us when its calls run back to back (normal
mode), 3284-3448 when interleaved with ~60 ms of scalar rows (tune mode), three times over. That is the shape of
Intel's AVX-512 frequency license (sustained 512-bit work drops the core clock; the reference's q6_K runs 256-bit);
the seat-alone runs (each vector seat back to back at 16 lanes) decide it, and the mint's tile race (bursty per
seat) may have crowned width 512 where the sustained gemv wants 256 - the gemv-own-crown item once more.
The seat-alone runs said no to the license: 512 alone 3713, 256 alone 3551, maddubs alone 3541 (reference 3441) -
back to back, all near parity - so it is the MODE: the same seat generated under the adopted profile (normal mode)
runs 5159-5270 at 16 lanes and 1861-2244 / 1939-3622 (bimodal within a process) at one thread, where the tune-mode
race of the same name reads 3284-3713 and 1615-1789 / 1648-2059. zen2 shows no such gap (normal 3446-4033, tune
3657-4076). The 64-byte frame realignment (e0b3892b7) did not move it. The JIT'd body is emitted at runtime and is
not in the cache, so the two bodies could not be diffed here. OPEN, on the Intel box: whether the adopted perm
parses to the grid row's fields (defaults) or the runtime differs; the gemv's own in-situ race (queued) measures
every seat the same way and stamps what is fastest, which closes it either way.


### M4 Pro v1 - first tables (2026-09-01, Apple M4 Pro 10P+4E, class arm-i8mm, on the arm-neon PROFILE through the chain - no arm-i8mm profile existed; TEST 90/90)

One thread at the engine phase; the engine shape = 10 performance cores, d=32768. The SMMLA lattice's first real race is
the tile column.

| fmt | decode ours / ref us | ratio | tile ours / ref us | ratio | engine 10 lanes ours / ref us | ratio |
|---|---|---|---|---|---|---|
| q8 | 693 / 1125 | 1.62 | 184237 / 425873 | 2.31 | 2179 / 2078 | 0.95 |
| k4 | 649 / 916 | 1.41 | 210798 / 458459 | 2.17 | 1107 / 1132 | 1.02 |
| k5 | 1625 / 1464 | 0.90 | 201095 / 766460 | 3.81 | 1556 / 1521 | 0.98 |
| k6 | 1304 / 1731 | 1.33 | 329965 / 732022 | 2.22 | 1611 / 1824 | 1.13 |
| q40 | 596 / 1262 | 2.12 | 218731 / 516770 | 2.36 | 1055 / 1342 | 1.27 |
| q51 | 1637 / 1996 | 1.22 | 356194 / 1035551 | 2.91 | 1445 / 2306 | 1.60 |
| iq4xs | 780 / 1308 | 1.68 | 208597 / 670258 | 3.21 | 1091 / 1269 | 1.16 |
| k3 | 1435 / 1824 | 1.27 | 368506 / 955375 | 2.59 | 1257 / 1864 | 1.48 |
| iq3s | 3904 / 4061 | 1.04 | 155531 / 2102599 | 13.52 | 3298 / 3841 | 1.16 |
| iq3xxs | 2661 / 3755 | 1.41 | 155576 / 1927299 | 12.39 | 2263 / 3539 | 1.56 |
| iq4nl | 788 / 1430 | 1.81 | 231408 / 734682 | 3.17 | 1047 / 1387 | 1.32 |
| k2 | 969 / 1395 | 1.44 | 279559 / 710512 | 2.54 | 854 / 1302 | 1.52 |
| iq2s | 2386 / 2975 | 1.25 | 183660 / 1504690 | 8.19 | 2233 / 2572 | 1.15 |
| iq2xs | 1786 / 2435 | 1.36 | 184119 / 1250945 | 6.79 | 1530 / 2240 | 1.46 |
| iq2xxs | 2226 / 2623 | 1.18 | 155669 / 1336029 | 8.58 | 1892 / 2328 | 1.23 |
| mx4 | 966 / 1322 | 1.37 | 263140 / 679457 | 2.58 | 982 / 1521 | 1.55 |

One thread: every decode >= 1.04 except k5 0.90; tiles 2.2-13.5x. Engine shape: everything >= 0.95 (q8), k5 0.98,
the rest 1.02-1.60. The arm-i8mm mint (the 1B vehicle, --tune, export) runs next on this box and the tables re-run
on the minted profile.
MINTED (fd8e490f1: provenance ok / ok / neon;dotprod;i8mm;bf16;fullfp16;lse, 49 kernels): the kq tile crowns are
the neon profile's (`mr8` everywhere), q8q8_tile_gen moves mr8_budget -> kstep2, seven [tuned] loop kernels pick
other widths. One thread on the minted profile, us (reference): q8 727 (1128) 1.55, k4 666 (916) 1.38, k5 1643
(1466) 0.89, k6 1312 (1732) 1.32, q40 605 (1263) 2.09, q51 1651 (1994) 1.21, iq4xs 815 (1311) 1.61, k3 1454 (1829)
1.26, iq3s 3888 (4066) 1.05, iq3xxs 2678 (3762) 1.40, iq4nl 795 (1433) 1.80, k2 976 (1404) 1.44, iq2s 2383 (2952)
1.24, iq2xs 1787 (2441) 1.37, iq2xxs 2224 (2616) 1.18, mx4 995 (1322) 1.33 - the same table as before, as the
crowns say. k5 0.89 is the M4's one tail (the M1 has it at 1.06; the P-core's byte->lane deposit costs more).
Engine shape on the minted profile (10 P-cores, d=32768), ours / reference us: q8 2178 / 2082 (0.96), k4 1116 /
1146 (1.03), k5 1549 / 1546 (1.00), k6 1627 / 1843 (1.13), q40 1057 / 1368 (1.29), q51 1447 / 2344 (1.62), iq4xs
1120 / 1292 (1.15), k3 1244 / 1909 (1.53), iq3s 3308 / 4059 (1.23), iq3xxs 2258 / 3735 (1.65), iq4nl 1055 / 1451
(1.38), k2 850 / 1343 (1.58), iq2s 2242 / 2624 (1.17), iq2xs 1698 / 2343 (1.38), iq2xxs 1894 / 2416 (1.28), mx4
983 / 1553 (1.58). The M4 CPU is at or above parity on every engine-shape row; k5 0.89 alone was the one tail.
LANDED: the NEON per-lane byte shift for k5's high-bit deposit (`ushl` / `ushr` + `and` per half, where the mask
test took five ops; sdot lattice only - x86 has no byte shift): M4 k5 decode 1625-1643 -> 1357 us (reference
1466: 1.08x), engine shape 1549 -> 1360 (1.14x), tile unchanged, k4 unchanged; TEST 90/90 on the M4 and zen2.
The M4 is closed on every row at both shapes.

### zen2 v4 - the closing tables (2026-09-01, after the k6/k3 transposes and the fused flush)

One thread at the HEAP phase (the ladder's engine-phase default came after this run; k6/k3 read 10-30%
better at the engine phase - k6 3527-3851, k3 2488-2893) and the engine shape (16 lanes, d=32768,
DRAM-bound; q8/mx4/k4 from the q8-family arm's validation run).

| fmt | decode ours / ref us | ratio | tile ours / ref us | ratio | engine 16 lanes ours / ref us | ratio |
|---|---|---|---|---|---|---|
| q8 | 3348 / 4260 | 1.27 | 565887 / 890979 | 1.57 | 8906 / 9248 | 1.04 |
| k4 | 2337 / 2528 | 1.08 | 518533 / 839960 | 1.62 | 4472 / 4425 | 0.99 |
| k5 | 3553 / 3327 | 0.94 | 540857 / 1325657 | 2.45 | 5400 / 5475 | 1.01 |
| k6 | 3460 / 3652 | 1.06 | 630854 / 1083547 | 1.72 | 6477 / 6402 | 0.99 |
| q40 | 1973 / 2868 | 1.45 | 387611 / 1003869 | 2.59 | 4275 / 4575 | 1.07 |
| q51 | 2866 / 4118 | 1.44 | 656416 / 1844397 | 2.81 | 5842 / 6235 | 1.07 |
| iq4xs | 2097 / 3349 | 1.60 | 675053 / 1674064 | 2.48 | 4577 / 4119 | 0.90 |
| k3 | 3080 / 2601 | 0.84 | 678487 / 1255396 | 1.85 | 3662 / 3605 | 0.98 |
| iq3s | 7603 / 10242 | 1.35 | 660738 / 5179767 | 7.84 | 3934 / 5300 | 1.35 |
| iq3xxs | 6650 / 6573 | 0.99 | 659839 / 3308544 | 5.01 | 3854 / 3509 | 0.91 |
| iq4nl | 2117 / 3078 | 1.45 | 589114 / 1090653 | 1.85 | 4542 / 4533 | 1.00 |
| k2 | 1261 / 2083 | 1.65 | 658936 / 779599 | 1.18 | 2555 / 2711 | 1.06 |
| iq2s | 5058 / 5022 | 0.99 | 721269 / 2510375 | 3.48 | 3070 / 2887 | 0.94 |
| iq2xs | 4645 / 5354 | 1.15 | 735654 / 2706628 | 3.68 | 2749 / 2886 | 1.05 |
| iq2xxs | 5192 / 5103 | 0.98 | 655125 / 2526484 | 3.86 | 2994 / 2751 | 0.92 |
| mx4 | 2193 / 3004 | 1.37 | 621096 / 1433680 | 2.31 | 4143 / 4364 | 1.05 |

Engine shape: every format 0.90-1.35 - q8 1.04, mx4 1.05, k4 0.99, k5 1.01, k6 0.99, q40 1.07, q51 1.07,
k3 0.98, iq3s 1.35, iq4nl 1.00, k2 1.06, iq2xs 1.05, iq2s 0.94, iq2xxs 0.92, iq3xxs 0.91, iq4xs 0.90. zen2
is closed for this pass: k5 0.94 at one thread is the one row under the bar, and it is 1.01 at the engine shape.

### zen4 v2 - after the transposes and the fused flush (2026-09-01, c7a.4xlarge EPYC 9R14, class x86-vnni512)

TEST 90/90. One thread at the heap phase (the bootstrap ran before the ladder's engine-phase default) and the
engine shape (16 lanes, d=32768, DRAM-bound). The q8/mx4 engine rows here predate the q8 family's team arm
(one raw thread against 16) - round two re-measures them.

| fmt | decode ours / ref us | ratio | tile ours / ref us | ratio | engine 16 lanes ours / ref us | ratio |
|---|---|---|---|---|---|---|
| q8 | 1614 / 3409 | 2.11 | 167078 / 500478 | 3.00 | 15344 / 6101 | 0.40 |
| k4 | 805 / 1865 | 2.32 | 328569 / 753160 | 2.29 | 3954 / 3028 | 0.77 |
| k5 | 2126 / 2615 | 1.23 | 178594 / 1120210 | 6.27 | 3788 / 3802 | 1.00 |
| k6 | 3045 / 2872 | 0.94 | 226619 / 782510 | 3.45 | 4840 / 4609 | 0.95 |
| q40 | 653 / 2743 | 4.20 | 172935 / 588322 | 3.40 | 3028 / 2963 | 0.98 |
| q51 | 1864 / 4607 | 2.47 | 239601 / 1431282 | 5.97 | 4149 / 4165 | 1.00 |
| iq4xs | 1180 / 2418 | 2.05 | 286066 / 1153787 | 4.03 | 2809 / 2848 | 1.01 |
| k3 | 833 / 2031 | 2.44 | 362356 / 978906 | 2.70 | 2318 / 2207 | 0.95 |
| iq3s | 7527 / 6087 | 0.81 | 259660 / 3089555 | 11.90 | 3903 / 3226 | 0.83 |
| iq3xxs | 5953 / 5234 | 0.88 | 260765 / 2664217 | 10.22 | 3122 / 2839 | 0.91 |
| iq4nl | 1066 / 3103 | 2.91 | 283855 / 625809 | 2.20 | 3084 / 3026 | 0.98 |
| k2 | 630 / 1276 | 2.03 | 429374 / 641108 | 1.49 | 1541 / 1605 | 1.04 |
| iq2s | 4917 / 3748 | 0.76 | 283651 / 1903473 | 6.71 | 2778 / 2116 | 0.76 |
| iq2xs | 4549 / 3616 | 0.80 | 283584 / 1844309 | 6.50 | 2312 / 2001 | 0.87 |
| iq2xxs | 4834 / 3915 | 0.81 | 260122 / 1991565 | 7.66 | 2464 / 2238 | 0.91 |
| mx4 | 1049 / 2740 | 2.61 | 293620 / 1348460 | 4.59 | 9599 / 2849 | 0.30 |

One thread: every non-grid decode 0.94-4.20x (k6 0.94), tiles 1.5-11.9x; the grid formats 0.76-0.88x -
the x86 tail, unchanged. Engine shape: k5/q51/iq4xs/k2 1.00-1.04, q40/iq4nl 0.98, k6/k3 0.95, iq3xxs/iq2xxs
0.91, iq2xs 0.87, iq3s 0.83, iq2s 0.76 - and k4 0.77 (67 GB/s where the reference and our k5 pull 86; 2.32x
at one thread). Round two on the same box: SOLO at the engine phase, the corrected team rows, a tune-mode race
of k4/k6/iq2s through the team arm, perf stat on iq2s.

Round two (the branch at 6d75a0956):

| fmt | one thread, engine phase ours / ref us | ratio | engine 16 lanes ours / ref us | ratio |
|---|---|---|---|---|
| q8 | 1874 / 3657 | 1.95 | 6575 / 6058 | 0.92 |
| k4 | 833 / 1906 | 2.29 | 3067 / 3022 | 0.99 |
| k5 | 2046 / 2885 | 1.41 | 4043 / 3794 | 0.94 |
| k6 | 2866 / 2686 | 0.94 | 4861 / 4600 | 0.95 |
| q40 | 737 / 2794 | 3.79 | 3163 / 2996 | 0.95 |
| q51 | 1690 / 4630 | 2.74 | 3940 / 4172 | 1.06 |
| iq4xs | 1176 / 2438 | 2.07 | 3036 / 2834 | 0.93 |
| k3 | 837 / 2016 | 2.41 | 2268 / 2209 | 0.97 |
| iq3s | 7571 / 6104 | 0.81 | 4128 / 3213 | 0.78 |
| iq3xxs | 5973 / 5242 | 0.88 | 3113 / 2802 | 0.90 |
| iq4nl | 1092 / 3055 | 2.80 | 2915 / 3025 | 1.04 |
| k2 | 623 / 1289 | 2.07 | 1461 / 1596 | 1.09 |
| iq2s | 4918 / 3763 | 0.77 | 2525 / 2129 | 0.84 |
| iq2xs | 4562 / 3633 | 0.80 | 2319 / 2001 | 0.86 |
| iq2xxs | 4857 / 3920 | 0.81 | 2461 / 2127 | 0.86 |
| mx4 | 1090 / 2763 | 2.54 | 2787 / 2844 | 1.02 |

Engine shape: q51 1.06, iq4nl 1.04, k2 1.09, mx4 1.02, k4 0.99 (the 0.77 above was a one-off), k3 0.97, k6 0.95,
q40 0.95, k5 0.94, iq4xs 0.93, q8 0.92 - and the grid formats iq3xxs 0.90, iq2xs/iq2xxs 0.86, iq2s 0.84, iq3s
0.78: compute-bound even at 16 lanes (iq3s pulls 49 GB/s of the box's 87). The tune race through the team arm
(d=32768, 16 lanes): k4 maddubs256 3084 / vpdpbusd256 3030 / vpdpbusd512 2969 and k6 4630 / 4611 / 4532 confirm
the crowns; iq2s 2528 / 2383 / 2530 - the crowned width-512 seat loses 6% to vpdpbusd256 at the engine shape,
the gemv inheriting the tile's crown (tune_companion). perf stat gave nothing (no PMU in the VM).

### zen4 v3 - the re-minted x86-vnni512 profile (2026-09-01, c7a.4xlarge EPYC 9R14, commit 11f69493e; `SOLO=1 NTOK=512` one thread at d=4096 for gemv/tile, then the team ladder: 16 lanes, d=32768, best of 5)

| fmt | gemv 1T ours / ref | ratio | tile 1T ours / ref | ratio | gemv 16 lanes ours / ref | ratio |
|---|---|---|---|---|---|---|
| q8 | 1983 / 3650 | 1.84 | 167509 / 501447 | 2.99 | 6671 / 5995 | 0.90 |
| k4 | 843 / 1878 | 2.23 | 350323 / 753000 | 2.15 | 2914 / 3049 | 1.05 |
| k5 | 2160 / 2685 | 1.24 | 178164 / 1124541 | 6.31 | 3873 / 3824 | 0.99 |
| k6 | 2478 / 2992 | 1.21 | 226354 / 787084 | 3.48 | 4637 / 4682 | 1.01 |
| q40 | 693 / 2726 | 3.93 | 173654 / 588905 | 3.39 | 3027 / 2940 | 0.97 |
| q51 | 1559 / 4640 | 2.98 | 285388 / 1428594 | 5.01 | 4327 / 4300 | 0.99 |
| iq4xs | 1129 / 2449 | 2.17 | 286750 / 1155395 | 4.03 | 3225 / 2856 | 0.89 |
| k3 | 942 / 2022 | 2.15 | 355086 / 979214 | 2.76 | 2212 / 2161 | 0.98 |
| iq3s | 7556 / 6092 | 0.81 | 261572 / 3093452 | 11.83 | 3896 / 3260 | 0.84 |
| iq3xxs | 5964 / 5226 | 0.88 | 260290 / 2660673 | 10.22 | 3176 / 2818 | 0.89 |
| iq4nl | 1075 / 3083 | 2.87 | 284054 / 624404 | 2.20 | 3115 / 2882 | 0.93 |
| k2 | 622 / 1281 | 2.06 | 430224 / 641204 | 1.49 | 1433 / 1611 | 1.12 |
| iq2s | 4915 / 3756 | 0.76 | 283819 / 1903444 | 6.71 | 2524 / 2124 | 0.84 |
| iq2xs | 4555 / 3621 | 0.79 | 283489 / 1845848 | 6.51 | 2559 / 2003 | 0.78 |
| iq2xxs | 2854 / 3916 | 1.37 | 260753 / 1992837 | 7.64 | 1657 / 2116 | 1.28 |
| mx4 | 1059 / 2746 | 2.59 | 292052 / 1349795 | 4.62 | 2895 / 2864 | 0.99 |

Reading: one thread, everything is past the reference except the four panel-form grids (iq3s 0.81, iq3xxs
0.88, iq2s 0.76, iq2xs 0.79; iq2xxs on the row form 1.37) - the compute-bound gap the zen4 memo targets. At
16 lanes the box is DRAM-bound (k4 ours 235 MB / 2.9 ms = 81 GB/s, the reference 77): the 0.89-0.99 rows
there are within the shared box's bandwidth noise, the grids repeat their one-thread losses (0.78-0.89).

### Intel v3 - the re-minted x86-amx profile (2026-09-01, c8i.4xlarge Xeon 6975P-C, commit 11f69493e; same ladders)

| fmt | gemv 1T ours / ref | ratio | tile 1T ours / ref | ratio | gemv 16 lanes ours / ref | ratio |
|---|---|---|---|---|---|---|
| q8 | 2226 / 2813 | 1.26 | 153424 / 578235 | 3.77 | 4414 / 4319 | 0.98 |
| k4 | 1138 / 1704 | 1.50 | 234490 / 847640 | 3.61 | 2057 / 3083 | 1.50 |
| k5 | 1567 / 2471 | 1.58 | 176873 / 1185485 | 6.70 | 2807 / 4063 | 1.45 |
| k6 | 1831 / 1883 | 1.03 | 218123 / 912830 | 4.18 | 5227 / 4353 | 0.83 |
| q40 | 1107 / 2289 | 2.07 | 168018 / 649524 | 3.87 | 2065 / 5143 | 2.49 |
| q51 | 1472 / 2915 | 1.98 | 227348 / 1462531 | 6.43 | 2907 / 5406 | 1.86 |
| iq4xs | 1109 / 2378 | 2.14 | 280699 / 1158466 | 4.13 | 2169 / 4817 | 2.22 |
| k3 | 1272 / 2269 | 1.78 | 297557 / 1143046 | 3.84 | 2642 / 2687 | 1.02 |
| iq3s | 5152 / 7079 | 1.37 | 251684 / 3550699 | 14.11 | 7426 / 7413 | 1.00 |
| iq3xxs | 4386 / 5073 | 1.16 | 250846 / 2597215 | 10.35 | 4533 / 6704 | 1.48 |
| iq4nl | 1113 / 2710 | 2.44 | 264126 / 714114 | 2.70 | 2079 / 6320 | 3.04 |
| k2 | 660 / 1470 | 2.23 | 287013 / 699910 | 2.44 | 957 / 1729 | 1.81 |
| iq2s | 3963 / 3690 | 0.93 | 275657 / 1885050 | 6.84 | 4266 / 3848 | 0.90 |
| iq2xs | 3914 / 4326 | 1.11 | 275710 / 2206824 | 8.00 | 3931 / 4610 | 1.17 |
| iq2xxs | 2505 / 3666 | 1.46 | 251397 / 1828919 | 7.28 | 2589 / 4255 | 1.64 |
| mx4 | 1207 / 3038 | 2.52 | 275059 / 1311864 | 4.77 | 2488 / 6361 | 2.56 |

Reading: 16 lanes at or past the reference except k6 0.83 (5227 us - the normal-mode gap, see the queue;
one thread shows it as best 1831 / median 2925 inside ONE process, so the slow mode is the common one and
flips between rounds, not between processes) and iq2s 0.90 (0.93 one thread). No gemv seat was written on
this box, so the k6 gemv still rides the 512 tile crown here.

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
- `kernel_parity_research_zen4.md` - why llama.cpp's five grid kernels beat ours on zen4 while losing on zen2.
  Headline: the reference has NO AVX-512 path for them - a znver4 build runs the same AVX2 body as zen2 (256-bit
  vpmaddubsw, never vpdpbusd) and its 1.4-1.9x zen2 -> zen4 gain is the core alone; ours gained 1.14-1.26x because
  we are stall-bound (fewer instructions, more cycles): the panel round trip (1024 scattered 4-byte stores per
  superblock-group re-read as 64-byte vectors) is the one structure we have and they do not. Ranked: the row form
  for iq3xxs on vnni512 (measured; loses at 16 SMT lanes - needs bare metal), `gather="qpanel"` (u64 grid entries
  stored whole, est 1.2-1.4x on the iq2 three), the parity-completed sign byte baked at repack (5-8%, free for
  iq2xxs/iq3xxs), mask-register signs at width 512 (vptestmb + merge-masked vpsubb), the 256-bit gemv seat (the
  harness's own crown now), and the VBMI symbol lattice - the grids are a 3-symbol (iq2) / 8-symbol (iq3)
  alphabet, so an entry is 12-16 bits and the decode is vpermi2b plus a few widen/shuffle ops, est 2-3x, needs
  avx512vbmi in TUNE_KNOWN_FEATURES. Do not seat hardware gathers (llama.cpp measured them losing on a 7950X)
  nor vpdpbssd (zen4 has no AVX-VNNI-INT8).
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

NEXT SESSION (Boris's rulings, 2026-09-01 evening): the gemv's own crown. The tuner races the TILE and the gemv
follows (`tune_companion`); on zen4 and Intel the gemv prefers `vpdpbusd_width256` where the tile crowns width
512 (iq2s 3-6%, k4 similar), and on Intel the stamped k6 gemv runs 1.5x slower than the same seat raced. The mint
races the gemv at decode shape between the TILE'S TOP TWO seats only (not the whole grid - "i don't see the point
of racing anything but top 2") and stamps the gemv separately; more mint time is fine ("performance matters
more"); `x86-amx` stays a class of its own, split from `x86-vnni512` ("for sure"). Then re-mint every class on
the emitter it ships with: x86-avx2 on zen2, x86-vnni512 and x86-amx rented (aws_bootstrap.sh, 35 min each),
arm-i8mm on the M4, arm-neon on the M1. After that: the zen4 grid kernels (the vnni512 panel form's compute), then
Vulkan gap 1. The CPU PR follows the Metal PR's merge (the make_pr chain on this branch).
LANDED (0251e8b2d): the kq grids gain `dot=vpdpbusd width=256 mr=16` (the same-layout alternative to the 512 tile
crown - the layout companion pins the plane mr, so only same-mr rows can differ); the harness picks the gemv among the
tile winner's same-mr rows, the two best by tile time, by the streamed-decode time past the margin over the tile
seat's own gemv (incumbent keeps a tie) and writes it under `<fmt>q8_gemv_gen`; llvm_tune stamps a companion from
its own entry when it names an eligible perm of the grid. TEST 90 -> 103 rows on zen2 (the new rows decline there).
The arm classes and x86-avx2 have no same-mr alternative (their grids collapse to one eligible row), so only
x86-vnni512 and x86-amx need the re-mint: two rentals with `aws_bootstrap_mint.sh` (build, TEST, the 1B vehicle's
--tune, export, both ladders) - running. The export copies every kernels entry, so the gemv seats ride into the
class profiles; the profile adoption merges them as ordinary entries.
zen2 smoke of the new harness path (the 1B vehicle, --tune, 16 lanes): provenance ok / ok / avx2;f16c;fma;sse4.2,
49 kernels, every crown unchanged (maddubs256_mr8), no gemv seat written - correct for a class with no same-layout
rival. The x86-avx2 profile stands.

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
   `(hb << (4 - j)) & 0x10` / `(hb >> j) & 0x10` - six ops per site against ten plus a shuffle.
   DONE-KILLED k5 (2026-09-01): the transpose was correct (TEST 90/90, the three tests green) and
   SLOWER - decode 4378 / 4414 / 4556 us heap phase, 4007 / 4664 / 5212 page-aligned, against
   3590-3760 before (reference 3307); tile 515 -> 559 ms. x86 has no byte-vector shift: `shl` /
   `lshr` on <32 x i8> lower to word shifts plus masks, so the per-lane mask test (no shift) was the
   cheaper deposit all along; k5's site-major byte and its shuffle stay. Reverted, nothing committed.
   The transposes are done: k6 and k3 landed, k5 was already right. On the M1 (sdot lattice, same
   IR): k6 1777 -> 1701, k3 1925 -> 1873 us, TEST 90/90.
   LANDED the fused scale flush (x86 maddubs lattice, k6/k3): the per-16 sub-scale rides the chain's
   widening - `pmaddwd(chain_i16, scale_i16_pair_splat)` straight into iacc, exact in i32 (|chain| <=
   32004 x |scale| <= 128) - instead of pmaddwd(ones) + i32 vpmulld into a0/a1 and a second add. 32 ->
   16 vpmulld, a0/a1 gone from the live set, k6 spills 36 + 67 -> 31 + 52. zen2 k6 decode at the
   engine phase 3527 / 3636 / 3851 us against the reference's 3683 (was 4444-4708): PARITY; heap 3808 /
   4007. k3 engine phase 2488 / 2613 / 2893 (reference 2701, was 2555-3504). Tiles unchanged within
   noise (k6 588-702 across runs, k3 660). TEST 90/90. The M1 is untouched (sdot has no i16 chain).
   Extended to k4/k5 (one 6-bit scale per sub-block, zero-extended): TEST 90/90; k5 decode at the engine
   phase 3399 / 3474 / 3511 (reference 3307, 0.94-0.97x; ~3600 before), k4 2030-2151 (reference 2435),
   tiles unchanged (k5 505, k4 482 ms). k5's last ~4% is the high-bit deposit itself (shuffle + two
   masked tests per site) - the per-sub-block layout lost to byte-shift emulation, the memo's D4 stands:
   k5 closes at 0.95x on zen2, 1.06x on the M1.

The 16-lane row and what it measures (2026-09-01, `--team`, the engine's own splitter: 64 self-served
chunks of 64 rows): k4 wall against the reference at the same thread count - 8 lanes 648 vs 675 us
(1.04x), 16 lanes 469 vs 341 (0.73x), 32 lanes 342 vs 112 (0.33x); k6 at 8 lanes 1165 vs 1041
(0.89x). The reference at 32 threads moves 33 MB in 112 us = 295 GB/s, three times this box's DRAM:
`test-backend-ops perf` repeats one small op and its row -> thread mapping is stable, so the weights
stay in the 3990X's 256 MB of L3 (16 MB per CCX); our self-served chunks move rows between CCX slices
every call and pay DRAM. At 8 lanes, where neither side gets the L3, we are at parity. A model's
weights (4-30 GB) never stay in L3 between tokens, so the fair many-lane comparison is a working set
above L3 - d=32768 (k4 264 MB) - measured next. If parity holds there, the many-lane gap seen at
m=4096 is the microbench's artifact, not the dispatch.
MEASURED (the reference given the m=32768 perf row - HOW_TO_GET_SIDECAR; the ladder's `BIG=1`):
k4 16 lanes 4560 vs 4549 us (1.00x), 32 lanes 4117 vs 5147 (1.25x); k6 16 lanes 6534 vs 6474
(0.99x) - both sides at the box's ~58-64 GB/s DRAM rate. PARITY at the engine's shape; the
"dispatch grain" question is closed - nothing to fix in the splitter. The m=4096 many-lane rows
stay in the ladder as a kernel-in-L3 view only; the BIG row is the one that speaks for decode.
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
   2026-09-01, the fact the two boxes give for free: zen2 -> zen4 sped every kernel up 1.5-3x (k4 2337 ->
   833 us, k6 3460 -> 2866) EXCEPT the grid decodes (iq2xs 4645 -> 4562, iq2s 5058 -> 4918, iq3s 7603 ->
   7571) - a wider, faster core buys them nothing, so they are bound by a latency chain (index -> grid
   load -> panel store -> reload), not by vector throughput. That is the case for the register compose
   on zen4 even though zen2 killed it: `DASLLAMA_GRID_ROWS_X86=1` re-arms the x86 row-group arms as a
   lab knob (env, read at emit time - clear `.jitted_scripts` between A/B runs, the JIT cache does not
   key on it). A/B on zen4 next; if it wins there and loses on zen2 the gate becomes the tune class.
   MEASURED on zen4 (one thread, engine phase; panel -> row form): iq2s 4918 -> 3559-3656 (reference 3757:
   PARITY), iq3s 7577 -> 6267-6419 (6105; but -10% at 16 lanes), iq2xxs 4860 -> 4302-4485 (3922); iq2xs
   4548 -> 8072 and iq3xxs 5973 -> 10744 LOSE 1.8x with either read shape (dword column or bytes). The
   sign path splits the ISAs too: the per-row +-1 table (the M1's win) costs x86 four scalar loads and
   three inserts per group, the vector-synthesized sign column plus one mask is cheaper there - with it
   zen2's row form reads iq2xxs 4696 (panel 4998) while iq2s/iq3s/iq2xs stay panel there. LANDED
   (c6e99f81a, f68534c06): `grid_rows_path` = the x86-vnni512 class takes the row form for iq2s and
   iq2xxs, every other x86 case the panel; the knob forces either. zen4 default check: iq2s 3595-3714
   (1.01-1.05x), iq2xxs 4383-4497 (0.87-0.89), iq2xs 4568-4577 (panel). OPEN: iq2s at 16 lanes on the
   c7a (8 cores x 2 SMT) reads a 5.9 ms wall with one slot at 8.8 ms, both forms - iq2s's 8 KB grid table
   plus panel plus x, twice per core, against a 32 KB L1 is the suspect (8-lane run next); the zen4 grid
   residue at the engine shape is iq3s 0.78, iq2xs 0.86, iq3xxs 0.90 - the panel form's compute, kernel
   work on the vnni512 lattice (a session of its own).
   zen4 at 8 lanes (one per core, d=32768; reference at 8 threads): iq2s 6398 (4236: 0.66), iq2xxs 2852
   (4206: 1.47x), iq2xs 4582 (3975: 0.87), iq3s 7668 (6410: 0.84), k6 5013 (4878: 0.97, 77 GB/s = the
   box's DRAM). iq2s scales at 56% of its one-thread rate (147 MB in 6.4 ms = 23 GB/s, not bandwidth)
   where iq2xxs scales at 96% - the 8 KB grid table's L1 pressure is the suspect.
   RESOLVED (the order test): the 8-9 ms slot belongs to whichever format is FIRST in a bench process - iq2s
   first 4887 (slot 786-8747), iq2xxs first 1924 / 4063 (342-7920); the same format second is flat (iq2s
   3257 at 2861-3377). The team arm now burns in (six warmup dispatches per row). With iq2s second, its row
   form at 16 lanes reads 3257 against the panel's 2525 (the ladder's row; reference 2159): the one-thread
   win does not survive the engine shape, so iq2s left the vnni512 gate and iq2xxs alone stays (1828-1924
   against the panel's 2456). zen4's grid residue at the engine shape stands: iq2s 0.85, iq3s 0.78, iq2xs
   0.86, iq3xxs 0.90 - the vnni512 panel form's compute.
   The frame realignment (every generated kernel's entry gets an alloca [64 x i8] align 64, so spill slots
   never straddle a cache line whatever RSP the caller arrives with): zen2 k6 heap phase 3344 / 3437 / 3737 /
   4389 (was 4.0-5.4 with two of three slow), engine phase 3423 / 3593 (reference 3683: 1.03-1.08x). The draw
   narrowed, one outlier remains - the stack phase was part of it, not all of it.
   Verified after the burn-in (8693a5b47), zen4 16 lanes with iq2s FIRST: iq2s 2535 (the ladder's 2525; reference
   2159 -> 0.85), iq2xxs 1696 (row form; 2134 -> 1.26x), iq2xs 2309 (2000 -> 0.87). Gate final: x86-vnni512 ->
   iq2xxs; x86-amx -> iq2xxs, iq3xxs; everything else the panel; the knob forces either.
   Round four, the row form with the COLUMN signs on zen4 (one thread, engine phase; reference in
   parentheses): iq2s 3541-3578 (3757: 1.05x), iq2xxs 2737-2771 (3922: 1.42x, from 0.89 with the table),
   iq3xxs 4234-4322 (5243: 1.22x, from 0.56 with the table; the panel 0.88), iq3s 6238-6309 (6105: 0.97),
   iq2xs 5381-5398 (3624: 0.67 - the panel's 0.80 stays). At 16 lanes on this 8-core x 2 SMT VM: iq2xxs
   1670 (panel 2456), iq3xxs 3415 (panel 3123 - loses), iq3s 4590 (panel 4145 - loses), iq2xs 2742 (panel
   2320), iq2s the 8.7 ms-slot anomaly. The one-thread wins that lose at 16 lanes point at SMT sharing
   (two lanes' instruction streams per core); the 8-lane run decides what the class gate should say for
   iq3xxs and iq3s. Tiles with the 64-aligned panel and scratch on zen4: unchanged (k5 178623, k6 225455,
   iq3s 258990, iq2s 283977, iq2xs 284079 us).

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
- 2026-09-01: re-mint round on the shipping emitter (commit 11f69493e). zen4 c7a (x86-vnni512, 50
  entries, TEST 103/103): the k3 tile crown moved 512_mr16 -> maddubs_width256_mr8 (tile race 51347
  vs 52840 us; the transposed k3 planes made the 256 maddubs seat the tile winner) and the k3 gemv
  took its own same-layout seat vpdpbusd_width256_mr8 (374 vs the tile seat's 401 us) - the first
  entry the gemv's own crown wrote; q51 tile 512_mr16 -> vpdpbusd_width256_mr8; axpy_q8kv plain ->
  vec8_u2, rope_scaled_neox_tab vec8_u2 -> plain. Intel c8i (x86-amx, 49 entries, TEST 103/103):
  every crown holds (k3 41768 vs 57342, k6 33760 vs 53832 for the 512 tile), axpy_f16 vec16 ->
  vec16_u2, no gemv seat - so the Intel k6 normal-mode 5.2 ms gap stays open.
- 2026-09-01: the new dot_vpdpbusd_width256_mr16 row was a dud on both boxes - its tile time equalled
  the reference row's (zen4 3168491 vs 3167365 us): perm_declines budgeted 16 ymm registers at width
  256, so a two-vectors-per-group row (nrsplit 4: 2*4*2+6 = 22) declined into the reference body. A
  row that requires AVX-512 runs where EVEX encodes ymm16-31; the budget is 32 there (461edda72,
  LLVM_JIT_CODEGEN_VERSION 0x66). The row is un-raced until the next mint.
- 2026-09-01: the qpanel spelling (DASLLAMA_GRID_QPANEL=1: eight qword stores into the alloca panel +
  one width load instead of the insert chain, commit eec5ce80c) measured on zen2 with the row form
  forced, one thread at the engine phase, qpanel 0 -> 1: iq2xs 8797 -> 8776, iq2s 6733 -> 7435, iq3s
  11886 -> 11647, iq3xxs 7212 -> 7262, iq2xxs 4624 -> 5841 us. A loss or a wash on zen2 (where the row
  form itself loses to the panel form: iq2xs 4565 panel); the zen4 A/B is the one that matters
  (iq2xxs's crown there IS the row form) - queued behind the box's ladder.
- 2026-09-01: qpanel killed - the zen4 and Intel A/Bs (row form forced, one thread, d=32768, qpanel 0 -> 1):
  zen4 iq2xs 33680 -> 40895, iq2s 47037 -> 44739, iq3s 68550 -> 70264, iq3xxs 43468 -> 46495, iq2xxs 22687 ->
  23111 us; Intel iq2xs 44067 -> 43923, iq2s 44660 -> 56802, iq3s 84893 -> 86998, iq3xxs 39726 -> 39662, iq2xxs
  22041 -> 21450. A loss or a wash on all three boxes; the helper and the knob are gone from the emitter.
  Same run, a find: on zen4 the row form beats the panel form for iq2xs at 16 lanes (2194 vs the ladder's panel
  2559 us - 0.91x of the reference instead of 0.78x) where the class gate picks the panel; the forms probe
  (zen4_forms.sh, both forms x five grids x two shapes) decides the gate.
- 2026-09-01: the width256_mr16 row raced alone on both boxes (post probes, d=32768, tune mode; full tables
  tmp zen4_post.log / intel_post.log). At 16 lanes every seat of every format sits within ~5% on both boxes -
  the minted crowns stand at the engine shape. One-thread streamed decode: on Intel 256_mr16 beats the 512
  gemv for k3 (12214 vs 16462), k5 (18972 vs 21184) and k6 (27321 vs 31723) while its tile loses ~14%, so
  the gemv's own seat is expected to take it at the next Intel mint; on zen4 it loses everywhere (k3 16193
  vs 7280). Intel iq2s: ALL seats ~0.90-0.94 of the reference at 16 lanes - the seat is not iq2s's lever.
- 2026-09-01: Intel k6 one thread, 40 rounds --each, normal mode: 3425, 1989, then steady 1637-1642 us -
  the slow reads were WARM-UP, steady state is 1.16x of the reference (1882); the ladder's 5-round
  best 1831 / median 2925 sampled the ramp. The 16-lane arm decides whether the 0.83 row is the same artifact.
- 2026-09-01: the VBMI lattice is CORRECT and WINS (Intel c8i, first races): TEST bit-exact on all five
  grid families (grid_vbmi maxdiff <= 2e-6); iq2xxs one thread 19334 vs the 512 row's 21823 us, 16 lanes
  2249 vs 2599 (the reference 4254 - 1.89x); iq3xxs one thread 30438 vs ~39700 (the panel form's post-probe
  read). The vbmi row's tile is the 512 body, so the tile race ties and the gemv's own seat crowns the
  lattice where it wins - a plain re-mint adopts it, no framework change.
- 2026-09-01: Intel k6 16-lane mode gap CONFIRMED mode-specific, not warm-up: same pinned seat, steady state
  ~4900-5300 us normal vs ~3236-3400 tune over 20 rounds; one thread differs 1% (1629 vs 1616). Arena phase
  is an 8% one-thread effect (off 64 best 1764, off 128-256 ~1920), not the 1.5x. intel_k6perf.sh counts the
  steady loop (perf stat + record by DSO) in both modes - kernel pages vs dispatcher decides the next step.
- 2026-09-01: the lattice's full Intel race (tune mode, d=32768; vbmi / old 512 seat / the reference at
  16 threads): 16 lanes iq2xxs 2249 / 2599 / 4255, iq3xxs 3376 / 4536 / 6704, iq3s 3726 / 7212 / 7413,
  iq2xs 2815 / 3951 / 4610, iq2s 3316 / 5706 / 3848; one thread iq2xxs 19334 / 21823, iq3xxs 30438 / 39773,
  iq3s 36485 / 72793, iq2xs 23533 / 32658, iq2s 28428 / 36371 us. Every Intel grid row clears 0.95 with the
  lattice - iq2s (the 0.90 row) reads 1.16x. The zen4 half runs next; then the re-mints adopt the seats.
- 2026-09-01: the lattice's zen4 race (tune mode, d=32768; vbmi / old 512 seat / the reference): one thread
  iq2xxs 17571 / 22402 / 32946, iq3xxs 27384 / 48023 / 43551, iq3s 26120 / 61810 / 49870, iq2xs 20518 /
  36590 / 30978, iq2s 26909 / 39628 / 32855; 16 lanes best iq2xxs 1551 / 1666 / 2106, iq3xxs 2520 / 3108 /
  2810, iq3s 2778 / 3882 / 3231, iq2xs 1618 / 2334 / 2006, iq2s 1935 / 2516 / 2113 us. TEST bit-exact.
  Every zen4 grid row clears 0.95 with the lattice - the four 0.76-0.90 one-thread rows read 1.2-1.9x.
- 2026-09-01: the Intel mode gap narrowed to the memory system: warm cache, both modes 96%+ of cycles in
  the kernel DLL, same clock, same THP rollup, and the instruction rate ratio equals the wall ratio exactly
  (identical instructions per gemv) - the normal body just stalls 1.54x more, 16 lanes only. Tune streams
  385 MB at 127 GB/s (partial cache residency), normal at 82.8. Round 5 sweeps d (8k/32k/64k), adds q8, and
  reads the arena VMA's own AnonHugePages plus LLC counters per mode.
- 2026-09-01: cold-start SIGSEGV at 0xc0 right after "Library linked - ok" (normal mode, 16-lane team k6
  d=32768, Intel; reproduced 3x cold, never warm, earlier bootstrap ladders passed) - a post-link rebind
  bug to chase before the PR.
- 2026-09-01: the cold-start SIGSEGV bisected. It needs only the PRESENCE of the five grid="vbmi"
  tune_perm rows in dasllama_math_gen.das - not the emitter body, globals, or intrinsic decls (zen4 B1/B2/B3
  disable each in turn, all still crash), and not the startup race (zen4 T1: fingerprint patched so nothing
  races, rows present -> crash; T2: rows removed, k6's own row made to race -> clean). It fires only cold +
  normal mode + --team; warm run 2 and every tune-mode race are clean, and the crashing format is k6, not a
  grid format - so it is a whole-module codegen/teardown effect of the extra generated variants (backtrace
  LLVMContextImpl::~LLVMContextImpl, fault at a small offset), not the lattice IR. Correctness is unaffected
  (TEST bit-exact, all races produced numbers). A focused C++ session owns the fix; the parity numbers come
  from warm/tune runs meanwhile.
- 2026-09-01: the cold-start crash FOUND and FIXED (17e664e7f). Cuts on zen4: L1 (permi2b calls, no table
  loads) clean vs L2 (table loads, no permi2b) crash pinned the lattice's 64-byte table-part loads; the core
  showed glibc's malloc_consolidate under ~LLVMContextImpl - heap corruption at compile time. Cause: the
  llvm_boost LLVMBuildGEP2 wrapper's `inbounds = true` default called LLVMSetIsInBounds on the result, and a
  GEP over a global with a constant index folds into a ConstantExpr - the cast to GetElementPtrInst wrote a
  flag into the constant's memory. Every emitter GEP before the lattice had a variable index or a parameter
  base, so nothing ever folded. The wrapper now builds inbounds GEPs through LLVMBuildInBoundsGEP2. Cold
  SOLO and cold 16-lane team both pass on zen4. Dead ends on the way: startup race path (T1/T2), frame size
  (five by-value grid tables), constant folding of the loads, split partitions, lane count.
- 2026-09-01: step 0 done - `kq_kernel_bench.das`, the reference rows, both memos, the fact base
  above. `test-backend-ops` built in `build-clean-cpu` and `build-vulkan` with the thread pin.
