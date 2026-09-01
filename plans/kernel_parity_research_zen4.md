# zen4 grid decode: why llama.cpp wins there and loses on zen2

Read-only study of the five codebook formats (IQ2_XXS, IQ2_XS, IQ2_S, IQ3_XXS, IQ3_S; our
kq ids 25/24/23/34/33) at the n=1 decode shape, on AMD EPYC 9R14 (zen4, AVX-512 + VNNI,
our class `x86-vnni512`) against zen2 (3990X, AVX2, class `x86-avx2`).

Trees read: `D:\Work\llama.cpp` at 6c84c7d5d (`ggml/src/ggml-cpu/arch/x86/quants.c`,
`arch/x86/repack.cpp`, `repack.cpp`, `ggml-cpu.c`, `ggml-common.h`, `ggml-cpu/CMakeLists.txt`,
and the MSVC object `build-clean-cpu/ggml/src/ggml-cpu.dir/Release/ggml-cpu/arch/x86/quants.c.obj`);
`D:\Work\daScript-iquant` (`dasllama_gemm_gen.das`, `dasllama_repack.das`, `llvm_tune.das`,
`llvm_jit_common.das`). Nothing modified in either tree. `x86:N` = `arch/x86/quants.c` line N.

---

## 0. The answer in four lines

llama.cpp's five grid kernels have **no AVX-512 branch at all** - a znver4 build compiles the
same `#if defined(__AVX2__)` body it compiles on zen2 (x86:2673, 2790, 3088, 3273, 3397). Their
1.42-1.90x zen2 -> zen4 gain is pure microarchitecture on unchanged code: their kernel is a wall
of independent 64-bit table loads, and zen3/zen4 went from 2 loads/cycle to 3 and grew the OOO
window. Their IPC rises from 2.2-3.9 to 3.8-5.9. Ours gains only 1.14-1.26x because the panel
form is **stall-bound, not instruction-bound** - we already execute fewer instructions per weight
than they do and still take longer.

---

## 1. What a zen4 build actually runs

- Flags: `GGML_NATIVE=ON` -> `-march=native` on gcc/clang (`ggml-cpu/CMakeLists.txt:305-306`);
  the local zen2 reference is MSVC (`build-clean-cpu/CMakeCache.txt`: Visual Studio generator,
  `/O2`, `GGML_AVX512:BOOL=OFF`) and goes through the FindSIMD probe at CMakeLists.txt:247-296.
- **All five vec_dots are `#if defined(__AVX2__)` / `#elif defined(__AVX__)` / generic.** There
  is no `__AVX512F__` and no `__AVX512VNNI__` arm anywhere in them: iq2_xxs x86:2673, iq2_xs
  x86:2790, iq2_s x86:3088, iq3_xxs x86:3273, iq3_s x86:3397. On znver4 `__AVX2__` is defined,
  so the AVX2 body wins and every vector is 256 bits.
- The one VNNI-aware helper in the file, `mul_sum_us8_pairs_float` (x86:105-119, `_mm256_dpbusd_epi32`
  under `__AVX512VNNI__ && __AVX512VL__`), is **not called by any of the five** - they all call
  `_mm256_maddubs_epi16` directly (x86:2699, 2887, 3145, 3303, 3464) because the per-block scale
  is folded into the following `_mm256_madd_epi16`, which VNNI cannot do.
- **No repack GEMV for any of the five.** `ggml_repack_get_optimal_repack_type`
  (`repack.cpp:4528-4722`) covers Q4_0, Q4_K, Q2_K (AVX-512 only, :4627-4632), Q5_K, Q6_K,
  IQ4_NL, MXFP4, Q8_0 and nothing else; `grep IQ2\|IQ3` over `repack.cpp` and
  `arch/x86/repack.cpp` is empty. `llamafile_sgemm` covers only IQ4_NL among the iq types
  (`sgemm.cpp:4131`). Type traits are `nrows = 1`, `vec_dot_type = Q8_K`, `from_float = NULL`
  (`ggml-cpu.c:336-366`), so real serving takes the per-row `vec_dot` above. **The grid rows of
  the ladder are the fair ones** - unlike k4/k5/k6/q4_0/q8_0/iq4_nl/q2_K.

---

## 2. Instruction shape per 32 weights of ONE row

Counted from the MSVC `/arch:AVX2` object's inner loops (the zen2 reference binary; the zen4
build compiles the same source with gcc, so counts shift but the shape does not).

| fmt | loop | weights/iter | instr | per 32w | mem ops | vec instr | scalar instr |
|---|---|---|---|---|---|---|---|
| iq2_xxs | 0xb1..0x26f | 64 | 85 | 42.5 | 33 | 34 | 51 |
| iq2_xs | 0x120..0x3cb | 128 | 110 | **27.5** | 34 | 100 | 10 |
| iq2_s | 0x150..0x2f2 | 64 | 90 | 45.0 | 26 | 34 | 56 |
| iq3_xxs | 0xc1..0x2e2 | 64 | 96 | 48.0 | 43 | 42 | 54 |
| iq3_s | 0x110..0x39f | 64 | 119 | **59.5** | 31 | 84 | 35 |

Per 32 weights, by stage:

- **IQ2_XXS** (x86:2687-2706): 4 `movzbl` index bytes; 4 x u64 `iq2xxs_grid[]` and 4 x u64
  `keven_signs_q2xs[]` reads, all folded into `vmovq`/`vpinsrq` pairs plus 2 `vinsertf128`
  (8 `vmovq` + 8 `vpinsrq` + 4 `vinsertf128` per 64 weights); 4 `shr` + 4 `and` for the 7-bit
  sign codes; 1 `vpsignb` on the ACTIVATION; 1 `vpmaddubsw`; scale `2*ls+1` as `vmovd` +
  `vpbroadcastw` + 1 `vpmaddwd`; 1 `vpaddd`. 16.5 memory ops, of which 8 are 64-bit table reads.
- **IQ2_XS** (x86:2841-2900) is the outlier and the fastest: the index extraction is vectorized
  (`_mm256_and_si256(q2_data, m511)`, x86:2842) and the 8th sign bit is a **4-bit parity by
  `vpshufb`** (x86:2844-2849, the `k_bit_helper` table) instead of a `ksigns` read. MSVC pulls the
  16 indices back out with 14 `vpextrw` rather than the source's stack union (x86:2823-2824).
  Only 8.5 memory ops per 32 weights, 4 of them u64 grid reads. Sign expansion is
  `vpshufb`+`vpand`+`vpcmpeqb`+`vpor`+`vpsignb` (x86:2871-2885), 5 vector ops per 32 bytes.
- **IQ2_S** (x86:3120-3151): index arithmetic in the ADDRESS expression (`qs[3] | ((qh[ib32]<<2)
  & 0x300)`, x86:3123-3130) - 10 `movzbl`, 11 `shl`, 10 `or`, 8 `and` per 64 weights; 4 u64 grid
  reads per 32 weights; signs from an explicit plane byte via `vpbroadcastd`+`vpshufb`+`vpand`+
  `vpcmpeqb` and the cheap **`sub(xor(mask,q8), mask)`** conditional negate (x86:3136) - 2 ops,
  not `or(mask,1)`+`vpsignb`.
- **IQ3_XXS** (x86:3287-3310): the most memory ops of the five (24.5 per 32 weights) because its
  grid entry is only 4 weights: **8 dword** `iq3xxs_grid[]` reads per 32 weights (16 `movzbl` +
  12 `vpinsrd` + `vmovd` per 64 weights) plus 4 u64 sign reads.
- **IQ3_S** (x86:3429-3471): the most instructions. The 9-bit index is built in a vector
  (`_mm256_sllv_epi32` + `_mm256_or_si256`, x86:3433-3438) and then taken back out - MSVC uses 12
  `vpextrd` where the source writes a `__m256i`/`uint32_t[16]` union (x86:3413-3418). 8 dword grid
  reads + 12 `vpinsrd` per 64 weights, then the same `sub(xor)` sign negate.

Everything is 256-bit. Nothing is 512-bit. Nothing uses `vpdpbusd`.

---

## 3. Why zen4 favors that shape, and where our panel loses

Machine facts (Zen 2 -> Zen 4): macro-op **dispatch stayed 6/cycle**; **loads went 2/cycle ->
3/cycle** and stores 1 -> 2; the ROB grew 224 -> 320; AVX-512 ops are executed as two 256-bit
passes over the same four FP pipes, so a 512-bit instruction has the same bytes/cycle as two
256-bit ones and only saves front-end slots.

Derived cycles per 32 weights and IPC (clock assumed 4.2 GHz zen2 / 3.7 GHz zen4 - **estimates**;
m=4096 k=14336 = 1.835e6 groups of 32 weights):

| fmt | ref zen2 us / c32 / IPC | ref zen4 us / c32 / IPC | ours zen2 c32 | ours zen4 c32 | ratio gain ref / ours |
|---|---|---|---|---|---|
| iq2s | 5022 / 11.5 / 3.9 | 3757 / 7.6 / **5.9** | 11.6 | 9.9 | 1.51 / 1.17 |
| iq2xs | 5354 / 12.3 / 2.2 | 3624 / 7.3 / 3.8 | 10.6 | 9.2 | 1.68 / 1.16 |
| iq2xxs | 5103 / 11.7 / 3.6 | 3922 / 7.9 / 5.4 | 11.9 | 9.8 | 1.48 / 1.21 |
| iq3s | 10242 / 23.4 / 2.5 | 6105 / 12.3 / 4.8 | 17.4 | 15.2 | 1.90 / 1.14 |
| iq3xxs | 6573 / 15.0 / 3.2 | 5243 / 10.6 / 4.5 | 15.2 | 12.1 | 1.42 / 1.26 |

Readings:

1. **Their kernel was load-issue and window limited on zen2 and is near the dispatch ceiling on
   zen4.** IQ2_XXS issues 33 memory ops per 64 weights; at 2 loads/cycle that is a 16.5-cycle
   floor against 85 instructions / 6 = 14.2 cycles of dispatch, and it measured 23.4 - short of
   both, i.e. window-limited. On zen4 the load floor drops to 11 cycles and it measures 15.8
   against the same 14.2 dispatch bound: **90% of the machine's issue width.** They cannot get
   much more from instruction selection on zen4; only from fewer instructions.
2. **We are not instruction-bound - we are stall-bound.** Our iq2xxs panel emits, per row per
   superblock, 32 index byte loads + 32 shl + 32 u64 grid loads + 32 lshr + 64 stores (about 24
   instructions per 32 weights, `emit_kq_gather` gemm_gen.das:1585-1616, `iq2_idx2` :1421-1435,
   `grid_pair_half` :1381-1385), plus about 7 for the dot and 2 for the sign column: ~33 per 32
   weights against their 42.5. We run **fewer** instructions in **more** cycles - IPC ~3.4
   against their 5.4 (estimate). Nothing zen4 widened is our constraint.
3. **The panel is the only structure we have that they do not.** At mr=16 it is 4096 bytes; the
   gather writes 1024 scattered 4-byte stores per superblock-group, one per row into each of 64
   distinct cache lines, visiting the same 64 lines 16 times, and the dot then reads them back as
   64-byte vectors each spanning 16 of those stores (`panel_group_off`, gemm_gen.das:1577-1580;
   the reload in `emit_block_iq4xs`). Store issue alone is 512 of the measured ~1253 cycles per
   superblock-group. Zen4 doubled store issue (1 -> 2/cycle) and we still gained only 1.21x, so
   the cost is the round trip, not the store throughput - a wide load over many narrow stores is
   the one direction x86 does not forward. Their own memory round trips (x86:2823 `aux_gindex`,
   x86:3413 `index_t`) are wide-store -> narrow-load and carry indices, never weights.
4. **Why the row form (`grid_rows_path`, gemm_gen.das:1469-1479) wins on some formats only.** It
   replaces the panel with one u64 grid load per row per 8 weights and a `vmovq`/`vpinsrq`/
   `vinserti128`/`vinserti64x4` tree (`row_weights8` :1749-1759, `concat_rows` :1713-1719). The
   whole 8-row group's vector cannot issue until the slowest row's index chain finishes, so the
   form trades the panel's stall tolerance for a shorter but **exposed** dependency chain. It wins
   exactly where the per-row index is one byte with no arithmetic:
   - iq2xxs (1 index byte, `wg_byte` at column 2*blk): 4866 -> 2737 us = 5.5 c32, **ahead of
     their 7.9** - already seated for `x86-vnni512` (gemm_gen.das:1476).
   - iq3xxs (1 index byte per 4 weights, column signs): 5973 -> 4234 = 1.41x. Seated on
     `x86-amx` only (:1477), **not on vnni512**.
   - iq2s / iq3s (qs byte + qh bits: `shl`+`and`+`or` on the chain): 4918 -> 3559-3656 and
     7560 -> 6267 - win alone, lose or go mixed at 16 lanes.
   - iq2xs (u16 assembled from two byte loads, `iq2xs_word16` :1411-1417, and the sign code in
     the same word): 4558 -> 5381 - loses.
5. **SMT flips the verdict.** Two lanes per core share the four FP pipes and one 32 KB L1D. A
   stall-bound kernel gets its bubbles filled by the sibling, an instruction-bound one does not,
   so at 16 lanes the panel catches up and the row form's extra instructions start to cost: our
   one-thread ratios 0.76-0.88 become 0.84-0.90 at the engine shape for four of five formats, and
   iq2s's row form "wins alone, loses at 16 lanes" is exactly this. Corollary for every item
   below: **instruction cuts pay less at 16 lanes than at one thread, and plane-size increases
   cost more** (iq3s already pulls 49 GB/s of the box's 87 at 16 lanes).
6. **The zen2 -> zen4 comparison of THEIR numbers mixes compilers** (MSVC `/arch:AVX2` vs gcc
   `-march=znver4`). Part of their 1.42-1.90x is codegen, not silicon. It does not change our
   conclusions - ours is measured on our own emitter on both boxes - but do not quote their gain
   as a microarchitecture number without rebuilding one side.

---

## 4. Ranked spellings for the vnni512 lattice

Every item is class-gated, so zen2 keeps what it has unless stated.

**1. Seat the row form for iq3xxs on `x86-vnni512`.** One line: extend gemm_gen.das:1476 from
`te.kq == 25` to `te.kq == 25 || te.kq == 34`, matching what x86-amx already does (:1477).
Measured on the box: 5973 -> 4234 us, i.e. 0.88x -> **1.24x** of the reference. zen2 untouched
(the class gate). 16 lanes: x86-amx recorded 1.36x at the engine shape for the same change, so
the SMT risk is low. Do this first.

**2. `gather="qpanel"` - store the iq2 grid entry as ONE qword.** The three iq2 formats read a
u64 grid entry covering 8 weights and then split it into two dwords (`grid_pair_half`
:1381-1385) written to two panel groups 4*mr bytes apart. Re-lay the panel so a row's 8
consecutive weights are contiguous, store the u64 unsplit, and let the dot read 8-rows-x-8-weights
vectors - the shape `grid_rows_dot` + `pairwise_add_i32` already consume (:1723-1745, :1801).
Per 8 weights the gather goes 1 idx load + 1 shl + 1 grid load + 1 lshr + 2 stores = 6 down to 4,
and **panel stores halve** (64 -> 32 per row per superblock), which is the resource item 3 named.
Targets iq2s, iq2xs, iq2xxs. Estimate 1.2-1.4x on those three at one thread, less at 16 lanes.
No new ISA, no plane change, helps zen2 identically. iq3s/iq3xxs get nothing (their entry is 4
weights) and stay on item 1.

**3. Bake the parity-completed sign byte at repack.** `kq_sign_bytes` (:1514-1548) still computes
`ksigns[i] == i | parity(i)<<7` at runtime with a 3-deep XOR/shift chain per `l` per block:
about 40 vector instructions per (blk, qd), 320 per superblock per row group. For **iq2xxs and
iq3xxs it is free** - the aux dword is 4 x 7 sign bits + a 4-bit scale, and the scale already
moved to the scale row, so four full sign bytes fit the same 4 bytes; for **iq2xs** it costs +1
bit per 8 weights (9 index + 8 sign > 16), so race it. iq2s/iq3s already carry explicit sign
bytes (:1519-1521). Estimate 5-8% on the panel form and more on the row form, on every class and
both shapes; zen2 gains too.

**4. Mask-register signs at width 512.** `apply_sign_col` (:1552-1564) and `sign_col_pm1`
(:1455-1463) build a byte mask with shuffle + `and` + `icmp` + `sext`, then `xor` + `sub` - six
vector ops per weight vector, each two uops at 512 bits. AVX-512 wants `vptestmb` into a
k-register plus a merge-masked `vpsubb`: two instructions. Emit the compare so LLVM keeps the
mask in `k` (do not `sext` it to `<64 x i8>`). Estimate 3-4 vector instructions saved per weight
vector, ~25% of the dot side. Width-256 path (zen2) unchanged - it keeps `llvm.x86.avx2.psign.b`
(:1730-1732), which is already one instruction.

**5. Give the grid gemv a 256-bit seat on vnni512.** Measured in the pass's own races: iq2s at 16
lanes 2528 (maddubs256) / **2383 (vpdpbusd256)** / 2530 (vpdpbusd512) - the crowned 512 seat
loses 6%. Mechanism, not noise: a 512-bit op on zen4 is two 256-bit passes over the same pipes
(no throughput gain), mr=16 doubles the panel to 4 KB per lane (8 KB per core under SMT, a
quarter of L1D), and width 512 forces the `icmp`/`select` sign path instead of the single
`vpsignb`. Blocked on the already-queued "gemv's own crown" item (the gemv currently inherits the
tile's `tune_companion`). Estimate 3-6%, larger at 16 lanes.

**6. The VBMI symbol lattice - the big one, zen4/Granite/Ice Lake only.** Verified over
`ggml-common.h`: every grid byte comes from a tiny alphabet - `iq2xxs_grid` (:560),
`iq2xs_grid` (:627) and `iq2s_grid` (:758) use exactly **3** distinct bytes {0x08, 0x19, 0x2b},
and `iq3xxs_grid` (:1017) and `iq3s_grid` (:1052) exactly **8** ({0x04,0x0c,0x14,0x1c,0x24,0x2c,
0x34,0x3e} and the odd 0x01..0x0f). So a grid entry is **16 bits** (8 weights x 2) for the iq2
family and **12 bits** (4 weights x 3) for the iq3 family, and the entire decode can run in
vectors:
- The grp<mr> repack puts a 4-byte column's mr rows contiguous (`repack_iq2xxs_grp`,
  dasllama_repack.das:663-700: `kq[dq + (c*mr + r)*4 + t]`), so at mr=16 **one 64-byte load is 64
  grid index bytes** (16 rows x 4 groups).
- For the **256-entry** grids (iq2xxs, iq3xxs) the compact code is two 256-byte byte-planes, and
  a 256-byte byte-table lookup is 2 x `vpermi2b` + one blend on index bit 7: ~3 instructions per
  64 indices per plane, ~6-8 for both.
- Expand the 2-bit / 3-bit fields with `vpmovzxbd` + `vpmultishiftqb` (per-byte bit-offset
  extraction) + `vpandq` + `vpshufb` against the 3- or 8-entry alphabet: **4 instructions per
  64-byte weight vector**, i.e. 16 rows x 4 weights.
- `ksigns_iq2xs` is `uint8_t[128]` (ggml-common.h:513) - exactly two zmm, so the sign-byte lookup
  is ONE `vpermi2b` if item 3 is not taken.
Budget: ~5-7 vector instructions per 64-weight vector against today's ~24 scalar instructions per
32 weights per row. Estimate 2-3x on the five formats at one thread; at 16 lanes the win is
smaller (item 5 of sec.3) but positive, because it removes stores rather than adding traffic.
Prerequisites: add `avx512vbmi` to `TUNE_KNOWN_FEATURES` (llvm_tune.das:1235) and a gate beside
`g_target_x64_avx512vnni` (llvm_jit_common.das:626-628). zen2 declines the seat.
For the **512/1024-entry** grids (iq2xs, iq2s, iq3s) the compact table does not fit `vpermi2b`
(8-16 zmm of table); either carry the 12/16-bit code in the plane instead of the index (+1.33-1.5x
plane bytes, which at 16 lanes trades compute for bandwidth - measure before committing) or leave
those three on items 1-4.

**7. Do not seat a hardware gather.** llama.cpp measured `_mm256_i32gather_epi32` losing to
`_mm256_set_epi32` **on a Ryzen 7950X, which is zen4**, and left the code commented out with the
note at x86:3440-3442. Zen4 `vpgatherdd` is still microcoded (order 10+ cycles per instruction -
estimate). The column read plus `vpermi2b` (item 6) is the vector path that works here.

**8. Do not seat `vpdpbssd` or `bias=128` for this class.** Zen 4 has no AVX-VNNI-INT8; those
seats are Zen 5 / Sierra Forest coverage, not a zen4 fix.

---

## 5. Traps

1. **There is no AVX-512 reference kernel to copy.** All five are `#if defined(__AVX2__)`
   (x86:2673, 2790, 3088, 3273, 3397); a znver4 build runs 256-bit `vpmaddubsw`, never
   `vpdpbusd`. Anything you find that looks like a 512-bit grid kernel in ggml is for another type.
2. **`_mm256_set_epi64x(grid[a],grid[b],grid[c],grid[d])` is not a vector gather.** It is four
   scalar table loads folded into `vmovq`/`vpinsrq` plus a `vinsertf128` - the disassembly shows
   exactly that. Transposing it into our layout buys the store, not the loads; the win only
   arrives when scalar table reads per weight go DOWN.
3. **Signs-on-the-activation does not port** (already ruled, research_cpu.md sec.8): our lanes are
   different rows with different sign patterns while the activation is splatted. llama.cpp's own
   arm64 arms put signs on the weights for the same reason.
4. **Their memory round trips run the safe way.** `aux_gindex` (x86:2823-2824, commented "somewhat
   hacky, but gives a significant boost") and `index_t` (x86:3413-3418) are ONE wide store feeding
   narrow fully-contained loads, and they carry indices. Our panel is the opposite direction and
   carries weights. Do not "unify" them.
5. **The iq2_xs `vpshufb` parity trick (x86:2844-2849) is a workaround, not a win for us.** It
   exists because x86 cannot vectorize a 128-entry x 8-byte table read; we can bake the
   parity-completed byte at repack (item 3) and skip both the table and the parity.
6. **Their scale fold into `vpmaddwd` (x86:2703, 3307, 3468) does not port.** Their scale is one
   scalar broadcast over a whole row; ours differs per lane and already rides the repacked scale
   row with the global factor (0.125f/0.25f) pre-baked. Porting it is a regression.
7. **Do not reintroduce a horizontal reduction.** They pay `hsum_float_8` per row (x86:2713, 2907,
   3158, 3317, 3478); our accumulator lanes are the output rows. A decode that gets cheaper at the
   price of a per-row reduction is a net loss.
8. **`nrc == 1` is asserted in all five** (e.g. x86:2662). There is no multi-row reference grid
   kernel - our tile's 6.5-11.9x is against a kernel nobody wrote for that shape, and it is not
   evidence about the decode.
9. **The dword column read is format- and form-specific.** `wg_col_byte` (gemm_gen.das:1364-1371)
   deliberately falls back to a plain byte load under `te.gridRows` (:1365, "the row form measured
   1.8x slower on the dword read"), while the same read is a win in the panel form for iq3xxs
   (:1404, 7862 -> 6602) and iq2xs (:1410, 6501 -> 4702). Do not unify them; race per (format, form).
10. **Count tables per core, not per lane.** Under SMT two lanes share one 32 KB L1D. The iq2s grid
    is 8 KB, iq2xs 4 KB, `keven_signs_q2xs` 1 KB, and two mr=16 panels are another 8 KB. Any new
    table (item 6 adds 512 B - 1 KB of compact code plus register-resident `vpermi2b` halves) has
    to be budgeted against that, and a plane-size increase is charged at the DRAM ceiling
    (87 GB/s on the c7a; iq3s already draws 49).
11. **The reference's zen2 and zen4 numbers come from different compilers** (MSVC `/O2 /arch:AVX2`
    in `build-clean-cpu`, gcc `-march=native` on the AWS box). Before attributing any part of their
    1.42-1.90x to zen4, rebuild one side with the other toolchain.
12. **iq2_xs is their best format, not their worst** (27.5 instructions per 32 weights against
    42-60 for the rest) because it is the only one whose index extraction and parity are
    vectorized. Expect it to be the hardest of the five to pass, and do not read its 0.80x as the
    same problem as iq3s's 0.81x.
