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

The full ladder (`harness/kernel_ladder.sh`, 2026-09-01, zen2 one thread, the stamped seat per format,
best of 5 interleaved rounds; ratio = reference / ours, >= 1.00 = ours faster):

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

Every tile row is ahead (1.17x-7.07x). Decode tails: k3 0.80x, k6 0.90x, k5 0.91x; iq3xxs, k2, iq2s,
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

CPU decode (gap 2) - the k-quant decode kernels are the tails the full ladder exposed: k3 0.80x,
k6 0.90x, k5 0.91x (next, k3 first); then the noise round on the 0.99-1.00 rows.

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
