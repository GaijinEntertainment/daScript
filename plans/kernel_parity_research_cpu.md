# CPU grid-quant dot: what llama.cpp does, what we do, and the gap

Read-only study of five codebook ("grid") formats — IQ2_XXS, IQ2_XS, IQ2_S, IQ3_XXS, IQ3_S —
plus Q2_K as the near-parity control, for the single-token decode shape (GEMV).

Trees read:

- `D:\Work\llama.cpp` — `ggml/src/ggml-cpu/arch/x86/quants.c`, `arch/arm/quants.c`,
  `quants.c`, `ggml-cpu.c`, `repack.cpp`, `llamafile/sgemm.cpp`, `ggml/src/ggml-common.h`
- `D:\Work\daScript-iquant` — `modules/dasLLAMA/dasllama/dasllama_gemm_gen.das`,
  `dasllama_math_gen.das`, `dasllama_repack.das`, `modules/dasLLVM/daslib/llvm_tune.das`

Nothing was modified in either tree.

---

## 0. The one structural fact that frames everything

llama.cpp and we solve a *different shape*, and almost every technique difference follows
from that.

**llama.cpp is one row at a time.** `ggml_vec_dot_iq3_s_q8_K(n, s, bs, vx, bx, vy, by, 1)`
walks one weight row against one activation row. Thirty-two consecutive weights of that one
row form one 256-bit register. Because the activation vector is *the same 32 bytes* the row
is being dotted against, llama.cpp is free to move the per-weight sign onto the activation.
Each row ends with a horizontal float reduction (`hsum_float_8`, x86 quants.c:2713) or, on
ARM, with a cross-lane `vaddvq_s32` per 32-weight sub-block (arm quants.c:3678).

**We are `mr` rows at a time, activation broadcast.** `setup_tile_emit`
(gemm_gen.das:2123-2141) sets `rv = width/32` and `rq = mr/rv`; one weight vector holds
**4 bytes from each of `rv` different output rows**. `dot_lane` (gemm_gen.das:328-366) splats
one dword-group of the activation across every lane and dots. For the seated x86 perm
(`width = 256, mr = 8`) one `<32 x i8>` weight vector = 8 rows × 4 weights; for AVX-512
(`width = 512, mr = 16`) it is 16 rows × 4 weights; on NEON (`width = 128, mr = 8, rq = 2`)
it is two vectors of 4 rows × 4 weights.

Consequences, both directions:

- **We win the reduction.** Our accumulator lanes *are* the output rows. There is no
  horizontal sum anywhere in the kernel — `emit_block_iq4xs` ends with one `sitofp` + one
  FMA per superblock per row-lane (gemm_gen.das:1943-1953). llama.cpp's ARM kernels pay a
  `vaddvq_s32` per 32-weight block per row (arm:3678, 3845, 3911, 4021), i.e. 8 cross-lane
  reductions per superblock per row on a serial dependency chain. This is almost certainly
  why our prefill/tile numbers on M1 are 2.1x-6.4x while decode lags.
- **We cannot put signs on the activation.** The 8 lanes of one weight vector belong to 8
  *different* rows with 8 *different* sign patterns, while the activation dword is shared
  across all lanes. Signs must go on the weights. This is a hard consequence of the layout,
  not a choice, and it is the part of followup entry 61 that needs correcting (see §8).
- **Grid decode cannot stream.** A grid word is 4 or 8 bytes of *one* row. In llama.cpp's
  layout, 8 consecutive grid words of one row *are* the next vector, so the composition
  `_mm256_set_epi32(grid[i7],...,grid[i0])` writes the register directly. In our layout a
  weight vector needs one grid dword from each of 8 different rows — which is *also* exactly
  a `set_epi32` of 8 table reads. The composition is available to us; we just do not use it.
  Today we round-trip through an `alloca` panel instead.

---

## 1. llama.cpp's mul_mat path selection for these types

`ggml_compute_forward_mul_mat` (ggml-cpu.c:1254).

- `src1` (activations) are quantized to the row type's `vec_dot_type` **once**, cooperatively
  by all threads, into `params->wdata` (ggml-cpu.c:1321-1355). For all six formats
  `vec_dot_type = GGML_TYPE_Q8_K` (ggml-cpu.c:298-366).
- `nrows = 1` for all six (ggml-cpu.c:302, 338, 344, 350, 356, 362), so
  `num_rows_per_vec_dot` is 1 (ggml-cpu.c:1435) — no 2-row MMLA fast path, unlike Q4_K/Q6_K
  which get `nrows = 2` under `__ARM_FEATURE_MATMUL_INT8` (ggml-cpu.c:313-317, 331-335).
- Work is chunked over rows and dispatched to
  `ggml_compute_forward_mul_mat_one_chunk` (ggml-cpu.c:1164), which calls `vec_dot` per row.
- **`from_float` is NULL for all five grid formats** (ggml-cpu.c:337, 343, 348-349, 356, 362)
  — these types are never produced on the fly, only read.

**Repack / "extra buffer type": none of the five grid formats has one.**
`ggml_repack_get_optimal_repack_type` (repack.cpp:4528) covers Q4_0, Q8_0, Q4_K, Q5_K, Q6_K,
MXFP4, IQ4_NL, and **Q2_K** — nothing else. So for IQ2_XXS/IQ2_XS/IQ2_S/IQ3_XXS/IQ3_S,
decode is *always* the per-row `vec_dot` above.

**Q2_K is the exception and it matters for the control measurement.** repack.cpp:4627-4632
selects `q2_K_8x8_q8_K` when `ggml_cpu_has_avx512()` and `ne[1] % 8 == 0`; the GEMV entry is
`gemv<block_q2_K, 8, 8, GGML_TYPE_Q8_K>` (repack.cpp:3976-3984) → `ggml_gemv_q2_K_8x8_q8_K`.
That is an 8-row-interleaved GEMV — *our* shape. There is also a RISC-V `q2_K_16x1_q8_K`
(repack.cpp:4633-4642). So a Q2_K comparison on zen2 (no AVX-512) races
`ggml_vec_dot_q2_K_q8_K`, and the same comparison on zen4 races the repacked 8x8 gemv. Worth
keeping straight when reading the 1.02x/0.95x control numbers.

`llamafile_sgemm` (ggml-cpu.c:1293-1318, 1364-1385) covers only F32, BF16, F16, Q8_0, Q4_0,
Q5_0, IQ4_NL (sgemm.cpp:3829, 3893, 3957, 4041, 4078, 4115, 4131) — none of the six.

---

## 2. Grid and sign tables (shared by both trees)

- `iq2xxs_grid` `uint64_t[256]` (ggml-common.h:560), `iq2xs_grid` `uint64_t[512]` (:627),
  `iq2s_grid` `uint64_t[1024]` (:758) — 8 weights per entry.
- `iq3xxs_grid` `uint32_t[256]` (:1017), `iq3s_grid` `uint32_t[512]` (:1052) — 4 weights per
  entry.
- **Every grid byte is a small non-negative magnitude**: iq2xxs bytes are `{0x08,0x19,0x2b}`
  (ggml-common.h:561-566), iq3xxs `0x04..0x3e` (:1018), iq3s `0x01..0x0f` (:1053). Two
  things follow: (i) the magnitudes are legal `u8` operands for `VPMADDUBSW`/`VPDPBUSD`, and
  the worst-case pair sum `2 × 62 × 127 = 15748` cannot saturate `i16`; (ii) no magnitude is
  zero, so per-byte two's-complement negation inside a 32-bit word never carries across a
  byte lane — a fact both trees rely on (ours explicitly, gemm_gen.das:1477-1479).
- `ksigns_iq2xs` `uint8_t[128]` (ggml-common.h:513): 7 sign bits → the 8-bit sign byte with
  bit 7 set to the parity completion. `kmask_iq2xs[8] = {1,2,4,...,128}` (:509).
- `keven_signs_q2xs` `int8_t[1024]` = 128 × 8 `±1` bytes, defined *per arch file*
  (x86:2624, arm:3595, loongarch:1577, powerpc:1388). Read as `const uint64_t * signs64`
  (x86:2675, arm:3646) so one 64-bit load gives 8 `±1` bytes.

---

## 3. IQ2_XXS

Packing: per 32 weights, four 8-bit grid indices + a 32-bit aux word carrying four 7-bit
sign codes and a 4-bit scale in the top nibble.

### (a) llama.cpp x86 (AVX2), `ggml_vec_dot_iq2_xxs_q8_K`, quants.c:2660-2714

1. Staging: `memcpy(aux32, q2, 16)` into a 16-byte scalar array, aliased as `aux8`
   (x86:2677-2678, 2690). Scalar staging of *packed bytes only* — never of decoded weights.
2. Grid compose: `_mm256_set_epi64x(iq2xxs_grid[aux8[3]], ..., aux8[0])` (x86:2691-2692) —
   four 64-bit table reads written straight into a ymm. No gather instruction, no store.
3. Signs: `signs64[(aux32[1] >> 21) & 127]` etc. (x86:2693-2696) — four 64-bit reads of
   `keven_signs_q2xs` composed into a second ymm of `±1` bytes. The parity-completed 8th
   sign is *in the table*; no parity arithmetic at runtime.
4. Sign application: `_mm256_sign_epi8(q8_1, s2_1)` (x86:2697-2698) — **applied to the
   activation**, one instruction per 32 bytes. Grid magnitudes stay unsigned.
5. Dot: `_mm256_maddubs_epi16(q2_1, q8s_1)` (x86:2699-2700), u8 × s8 → i16 pairs.
6. Scale: `_mm256_madd_epi16(dot, _mm256_set1_epi16(2*ls+1))` (x86:2703-2704) — the per-32
   scale is folded into the *same* `vpmaddwd` that does the i16→i32 pair reduction. One
   instruction does scale and widening together.
7. Accumulate: two independent i32 ymm chains `sumi1`/`sumi2` (x86:2705-2706), added once
   per superblock, `cvtepi32_ps` + `fmadd` with `d = x.d * y.d` (x86:2709). The `0.125f`
   global factor is applied once at the very end (x86:2713).

No intermediate weight buffer. Only the 16-byte `aux32` staging array touches memory.

An SSE/AVX-only fallback exists (x86:2715-2770) using `_mm_set_epi64x` pairs.

### (b) llama.cpp arm64 (NEON+dotprod), quants.c:3631-3689

1. Same 16-byte `aux32` staging (arm:3644-3645, 3663).
2. Grid compose: `vcombine_s8(vld1_s8(iq2xxs_grid + aux8[0]), vld1_s8(... + aux8[1]))`
   (arm:3664-3667) — two 8-byte table loads combined into a q-register. NEON's equivalent of
   `set_epi64x`.
3. Signs: same shape from `signs64` (arm:3668-3671).
4. Sign application: `vmulq_s8(q2u.val[i], q2s.val[i])` (arm:3672-3675) — **applied to the
   weights**, because `sdot` is signed×signed and there is no `u8×s8` NEON dot. This is the
   arch-forced mirror of the x86 choice.
5. Dot: `ggml_vdotq_s32` chained twice per 32 weights (arm:3676-3677).
6. Scale + accumulate: `sumf1 += vaddvq_s32(p1) * (0.5f + (aux32[1] >> 28))`
   (arm:3678-3679) — a **cross-lane reduction and a float multiply-add per 32-weight
   block**, 8 per superblock. This is the weakest part of llama.cpp's ARM grid kernels.
7. `*s = 0.25f * sumf` (arm:3684).

### (c) What ours does today

Seat: `dasllama_math_gen.das` — the `iq2xxsq8_tile_gen` block, four perms only
(`mr=8` NEON sdot; `maddubs/256/8`; `vpdpbusd/256/8`; `vpdpbusd/512/16`), same list as every
other grid format. GEMV generator: `iq2xxs_gemv_gen` → `kq_gemv_gen_impl(gc, 25)`
(gemm_gen.das:3344).

- Panel alloca `256 * mr` bytes, `align 32`, one per GEMV call
  (gemm_gen.das:3081-3082).
- Per superblock, `emit_iq2xxs_gather` (gemm_gen.das:1749-1806) runs an **`mr`-iteration row
  loop**; inside, for each of 8 blocks and 4 sub-lanes:
  - the 4-byte aux word is read as **four separate `i8` loads plus three shifts and three
    ORs** (gemm_gen.das:1766-1770) — note IQ3_XXS reads the identical field with **one
    aligned `i32` load** (gemm_gen.das:1438-1439). The offsets `(2*blk+1)*mr*4 + 0..3` are
    contiguous and 4-aligned, so the four-load spelling is pure waste.
  - `ksigns` lookup: one `i32` load from the `[128 x i32]` global (gemm_gen.das:1633-1687,
    1775-1776).
  - grid: two `i32` loads from the `[1024 x i32]` low/high-word-pair global
    (gemm_gen.das:1782-1784).
  - sign apply: `w = (gw ^ mw) + (mw & 0x01010101)` where `mw` is an `i32` from a
    16-entry nibble→byte-mask table (gemm_gen.das:1785-1787) — **three GPR ops and two loads
    per 4 weights**, i.e. ~40 scalar ops per 32 bytes of weight.
  - store: one 4-byte store into the panel (gemm_gen.das:1793-1794).
- The dot then reloads the panel as `<32 x i8>` vectors (gemm_gen.das:1880-1883), takes
  `abs_w` to recover the magnitudes it just threw away (gemm_gen.das:1888-1889,
  370-375 — `llvm.abs` → `VPABSB`), signs the *splatted activation* with `VPSIGNB`
  (gemm_gen.das:349-357), then `maddubs` + `pmaddwd` (gemm_gen.das:361-365).
- Scale: per-32 `i8` scale row loaded as `<8 x i8>`, `sext` to `<8 x i32>`, then a **`vpmulld`
  per block per row-lane** (gemm_gen.das:1922-1924, 1935). Integer accumulation across the
  superblock, one `sitofp` + FMA at the end (gemm_gen.das:1943-1953). The `0.25f` global
  factor is pre-baked into the repacked scale row, so it costs nothing at runtime.

**The memory shape is the wrong way round.** The store address is
`panel + r*4 + (blk*4 + g)*mr*4 + half` (gemm_gen.das:1793); consecutive stores for one row
are `mr*4 = 32` bytes apart, and the reload at
`(blk*16 + j*4)*mr` (gemm_gen.das:1875) is a 32-byte load covering **8 distinct 4-byte
stores from 8 different loop iterations**. A wide load spanning several narrow stores is the
one direction x86 store-to-load forwarding cannot take. llama.cpp does round-trip through
memory — but only for *indices*, and only in the safe direction (§6c): one wide store, then
narrow fully-contained loads.

Rough uop budget per 32-byte weight vector (mr = 8, AVX2):
ours ≈ 8×(2 loads + 3 ALU + 1 store) + 1 reload + `abs` + `psign` ≈ 51;
a register-composed form ≈ 9 (`vpinsrd` with folded grid load) + 9 (mask) + `vpor` +
`vpsignb` ≈ 20.

### (d) Deltas, ranked

| # | Delta | Candidate seat | Why / estimated payoff |
|---|---|---|---|
| 1 | **Delete the panel.** Compose the 8 grid dwords of one weight vector directly with `insertelement` (the `set_epi32` shape) instead of storing them and reloading. | `tune_perm(dot="maddubs", width=256, mr=8, gather="reg", requires="avx2")` and the `vpdpbusd` twins | Removes 512 scattered 4-byte stores + 64 wide reloads per superblock per row-group, and the store-forwarding hazard. Biggest single item; ~2.5x fewer decode uops. |
| 2 | **Sign in vectors, not GPRs.** Compose a second vector of the 0x00/0xFF byte masks and apply `vpor(mask,1)` + `vpsignb` to the activation, keeping the raw grid vector as the unsigned `maddubs` operand. Deletes both the 3-GPR-op-per-dword negate *and* `llvm.abs`. | folded into `gather="reg"`, or split as `psign="mask"` | ~24 GPR ops → 2 vector ops per 32 bytes; also removes `VPABSB` from the inner loop. |
| 3 | **One `i32` load for the aux word.** Replace the four `i8` loads + 3 shifts + 3 ORs at gemm_gen.das:1766-1770 with the single aligned `i32` load IQ3_XXS already uses (gemm_gen.das:1438-1439). | no seat needed — a straight bug-shaped fix | 6 uops × 8 blocks × mr rows per superblock, for free. Do this regardless. |
| 4 | **Bake the parity-completed sign byte at repack time.** The aux word is 4×7 sign bits + a 4-bit scale, and the scale already moved to the scale row (gemm_gen.das:1746-1748) — so 4 full sign *bytes* fit in the same 4 bytes. Deletes the `ksigns` load entirely and makes the sign bits plane-resident like IQ3_S's. | repack-side; pairs with delta 2 | Removes one dependent table load per 8 weights. Zero size change for this format. |
| 5 | **Seat `vpdpbssd`.** The panel bytes are already signed and the activation is signed; `dot_lane` takes that path with no `abs` and no `psign` (gemm_gen.das:344-347, 370-372). Currently unseated for every grid format. | `tune_perm(dot="vpdpbssd", width=256, mr=8, requires="avxvnniint8")` | One-line seat, zero generator change. Only helps AVX-VNNI-INT8 hosts (not zen2/zen4). |
| 6 | **Seat `bias=128`.** Under `gather="reg"` the biased byte is free — `((mag^mask) + (mask&0x01010101)) ^ 0x80808080` — and `bias128` drops `abs` and `psign` on VNNI hosts. Blocked today: `setup_tile_emit` forces `te.bias = 0` whenever `te.kq != 0` (gemm_gen.das:2136). | `tune_perm(dot="vpdpbusd", width=256, mr=8, bias=128, gather="reg", requires="avxvnni\|avx512vnni")` | Needs the `te.bias` clamp lifted for panel formats. Not valid under `maddubs` — `(w+128)×x` pair sums saturate `i16`. |
| 7 | **Fold the scale into `pmaddwd`.** llama.cpp gets scale + widening in one `vpmaddwd` (x86:2703). We cannot broadcast a scalar scale (each lane is a different row), but we can keep an `<16 x i16>` per-row scale vector and use `pmaddwd` against the `maddubs` output instead of `vpmulld` on the i32 accumulator. | `scale="madd16"` | Replaces a 10-cycle `vpmulld` per block per row-lane with a 5-cycle `vpmaddwd` that also removes the separate ones-vector `pmaddwd`. Modest but cheap. |

---

## 4. IQ2_XS

Packing: per 32 weights, four `uint16` words, each = 9-bit grid index + 7 sign bits; the 8th
sign is parity. Per-16 scales in a nibble-packed `scales[QK_K/32]`.

### (a) llama.cpp x86 (AVX2), quants.c:2778-2900

1. **Vectorized index extraction.** One 32-byte load of eight `uint16` words; `aux_gindex =
   _mm256_and_si256(q2_data, m511)` (x86:2842). The result is written to a stack union and
   read back as `uint16 gindex[16]` (x86:2823-2824, commented *"somewhat hacky, but gives a
   significant boost in performance"*).
2. **Parity by `pshufb`.** `partial_sign_bits = q2 >> 9`, `partial_sign_bits_upper = q2 >> 13`,
   XOR them, then `_mm256_shuffle_epi8(bit_helper, ...)` with a 16-entry nibble table that
   returns 0x00/0x80 (x86:2844-2849). That is a 4-bit population-parity computed as a
   single `vpshufb`, giving the 8th sign bit in bit 7. No `ksigns` table read at all on this
   arch.
3. Grid compose: four `_mm256_set_epi64x` of four `iq2xs_grid[...]` reads each
   (x86:2856-2863).
4. **Sign expansion by shuffle + cmpeq.** For each 32-byte group: `vpshufb` broadcasts the
   group's sign byte across 8 lanes (`block_sign_shuffle_1/2`, x86:2796-2803), `AND` with
   `bit_selector_mask` = `{1,2,4,...,128}` repeated (x86:2804-2807), `cmpeq` against the same
   mask → 0x00/0xFF, `OR` with `mone` → `±1`, then `_mm256_sign_epi8(q8, ...)`
   (x86:2871-2885). **Signs on the activation; grid stays unsigned.** Four vector ops per 32
   weights.
5. Scales: one 8-byte `memcpy`, nibble split and interleave, `scales = (stmp<<1) | 1`
   (x86:2832-2835). Per-block broadcast via `_mm_shuffle_epi8(scales, get_scale_shuffle(ib32))`
   + `cvtepi8_epi16` (x86:2892-2895), then `madd_epi16` (x86:2897-2900).
6. Two i32 accumulators, `fmadd`, `0.125f` at the end (x86:2903-2907).

An SSE path mirrors all of this at 128 bits (x86:2909-3070).

### (b) llama.cpp arm64, quants.c:3693-3763

1. **No parity arithmetic** — `signs64 + (q2[i] >> 9)` reads `keven_signs_q2xs` directly with
   the 7-bit code (arm:3739-3742), and the table already carries the parity completion. The
   x86 `pshufb` parity trick exists only because a 128-entry × 8-byte table read does not
   vectorize on x86 the way `vld1_s8` does on NEON.
2. Grid: `vcombine_s8(vld1_s8(iq2xs_grid + (q2[0] & 511)), ...)` (arm:3735-3738).
3. Signs on the **weights**, `vmulq_s8` (arm:3743-3746).
4. Four `ggml_vdotq_s32` (arm:3747-3750).
5. **The best scale/reduction shape in the whole ARM file**:
   `p = vpaddq_s32(vpaddq_s32(p1,p2), vpaddq_s32(p3,p4))` collapses four per-block i32x4 dot
   vectors into one vector whose 4 lanes are the 4 block sums, then a single
   `vmlaq_s32(sumi, p, scales32.val[ib64])` applies four per-block scales at once
   (arm:3751-3752). Integer accumulation throughout; one `vaddvq_s32` per *superblock*
   (arm:3755), not per block. Scales pre-expanded to `int32x4x4_t` outside the k-loop
   (arm:3722-3731).

### (c) What ours does today

`emit_iq2xs_gather` (gemm_gen.das:1689-1745). Identical skeleton to IQ2_XXS:

- The `uint16` word is assembled from **two separate `i8` loads plus a shift and an OR**
  (gemm_gen.das:1711-1716). The offsets are `lp%4 ∈ {0,2}` within the same 4-byte column, so
  a single 2-byte load would do.
- `ksigns` lookup from the 7-bit high field (gemm_gen.das:1718-1720) — the parity path, same
  as NEON's, not x86's `pshufb` trick. Correct choice for a scalar decode.
- Grid: two `i32` loads from the `[1024 x i32]` low/high pair global
  (gemm_gen.das:1578-1632, 1725-1727).
- Same 3-GPR-op negate and same 4-byte panel store (gemm_gen.das:1728-1732).
- Consumption: `iq2 = true` (gemm_gen.das:1817), so the lo and hi 16-weight halves keep
  **split accumulators** `a[]`/`a1[]` and get separate per-16 unsigned scales
  (gemm_gen.das:1893-1898, 1912-1919, 1932-1937) — the per-16 granularity IQ2_XS and IQ2_S
  need. That part is right and is structurally what ARM's `vpaddq` + `vmlaq_s32` achieves,
  without the reduction.

### (d) Deltas, ranked

Same 1/2/5/6/7 as IQ2_XXS, plus:

| # | Delta | Candidate seat | Why |
|---|---|---|---|
| 3' | **One `i16` load for the packed word.** Replace the two `i8` loads + shift + OR (gemm_gen.das:1711-1716) with one aligned 2-byte load. | none — direct fix | 3 uops × 32 × mr per superblock, free. |
| 4' | **Split the sign byte into the plane at repack time.** IQ2_XS is the one format where the parity-completed byte does not fit for free (9 + 8 = 17 bits > 16). Costs +1 bit per 8 weights if adopted; race it against the `ksigns` load. | repack-side | Lower priority than for IQ2_XXS / IQ3_XXS where it is free. |
| 8 | **Race the `pshufb` parity form** as an alternative to the `ksigns` table read, once the sign path is vectorized (delta 2). llama.cpp measured it as *"a significant boost"* (x86:2822) on their layout. | `psign="parity"` | Only interesting if delta 2 lands first; on a scalar-index decode the table read wins. |

---

## 5. IQ2_S

Packing: 8-bit `qs` index + 2 bits from `qh` = 10-bit index into `iq2s_grid[1024]`;
**explicit sign bits** (`QK_K/8` bytes at `qs + QK_K/8`), no parity trick; nibble-packed
per-16 scales.

### (a) llama.cpp x86 (AVX2), quants.c:3075-3160

1. Grid compose: `_mm256_set_epi64x(iq2s_grid[qs[3] | ((qh[ib32] << 2) & 0x300)], ...)`
   (x86:3123-3130) — index arithmetic done **scalar, in the address expression**, four
   64-bit reads per ymm. Note the shift amounts run `8,6,4,2` down the lanes so the same
   `qh` byte feeds all four.
2. Sign expansion: `_mm256_set1_epi32(signs[0] | (signs[1] << 16))` broadcasts 4 sign bytes,
   `shuffle_epi8(aux256, mask1)` spreads each byte across 8 lanes (`k_mask1`, x86:3090-3092),
   `AND` with `k_mask2` = `{1,2,...,128}`×4 (x86:3094-3096), `cmpeq` → 0x00/0xFF
   (x86:3133-3135).
3. Sign application: `_mm256_sub_epi8(_mm256_xor_si256(s2_1, q8_1), s2_1)` (x86:3136) — a
   **two-instruction conditional negate under a 0/-1 mask**, cheaper than
   `or(mask,1)` + `psign` used by IQ2_XS, and applied **to the activation**.
4. `maddubs` (x86:3146-3147), then scale via
   `_mm256_shuffle_epi8(scales16, get_scale_shuffle_k4(ib32))` + `madd_epi16`
   (x86:3148-3149). Scales precomputed once per superblock as
   `(nibble << 1) | 1`, `cvtepi8_epi16` (x86:3114-3116).
5. Two i32 accumulators, `fmadd`, `0.125f` at the end (x86:3155-3159).

### (b) llama.cpp arm64, quants.c:3767-3859

1. Grid: four `vcombine_s8(vld1_s8(iq2s_grid + (qs[i] | ((qh[..] << k) & 0x300))), ...)`
   (arm:3808-3816).
2. Sign expansion: `vdupq_n_u32(signs[0] | (signs[1]<<16))`, two `ggml_vqtbl1q_u8` with
   `k_mask1` halves, `vandq_u8` with `k_mask2`, `vceqq_u8` (arm:3820-3825) — the direct
   NEON analogue of the x86 shuffle+cmpeq.
3. Sign application: `vmulq_s8(vorrq_u8(vs, m1), q2s)` (arm:3826-3827) — `±1` on the
   **weights**.
4. Four `ggml_vdotq_s32` (arm:3838-3841).
5. Scale: `sumi1 += vaddvq_s32(p1) * (1 + 2*(x[i].scales[ib32] & 0xf))` (arm:3843-3846) —
   scalar integer accumulate, **four `vaddvq_s32` cross-lane reductions per 64 weights**.
   Weaker than the IQ2_XS `vpaddq`+`vmlaq_s32` form on the same arch, for no obvious reason.

### (c) What ours does today

`emit_iq2s_gather` (gemm_gen.das:1518-1575).

- Index: one `i8` load of `qs`, one `i8` load of the `qh` byte per block, `shl`/`and`/`or`
  to build the 10-bit index, then `shl 1` for the low/high word pair
  (gemm_gen.das:1543-1550).
- **Sign byte is read straight out of the plane** (gemm_gen.das:1541-1543) — no `ksigns`
  indirection, because IQ2_S stores signs explicitly. This is the format where delta 2
  (vector sign expansion) is cheapest to reach: the sign bits are already plane-resident
  and, given the `(p/4)*mr*4 + p%4` column layout (gemm_gen.das:1542), **one 32-byte load
  covers 8 rows × 4 consecutive sign bytes at stride 4** — exactly the input a
  `vpshufb`+`cmpeq` expansion wants.
- Same 3-GPR-op negate, same 4-byte panel store (gemm_gen.das:1557-1562).
- Consumption: the `iq2` split-accumulator per-16 path (gemm_gen.das:1817, 1893-1898).

### (d) Deltas, ranked

| # | Delta | Candidate seat | Why |
|---|---|---|---|
| 1 | Delete the panel (as §3). | `gather="reg"` | Same as every format. |
| 2* | **Vector sign expansion, promoted.** For IQ2_S specifically the 8 rows' sign bytes are one 32-byte strided column load away (gemm_gen.das:1542); `vpshufb` to select the wanted byte per row, `vpshufb`/`and`/`cmpeq` to expand to 32 mask bytes. ~5 vector ops replace 8 loads + 8 GPR negates. | `psign="expand"` | Highest-value sign delta of the five, because no `ksigns` table read stands in the way. |
| 3 | Adopt the **`xor`+`sub` conditional negate** (x86:3136) in place of `or(mask,1)`+`psign` wherever the mask is already 0/-1. | folded into `psign="expand"` | 2 ops vs 2 ops on x86, but frees `VPSIGNB` port pressure and works at 512 bits where `llvm.x86.avx2.psign.b` does not exist (see §9). |
| 5,6,7 | `vpdpbssd` seat, `bias=128` seat, `pmaddwd` scale fold — as §3. | | |

---

## 6. IQ3_XXS

Packing: 8-bit grid index into `iq3xxs_grid[256]` (4 weights each); a per-32 aux word with
four 7-bit sign codes + 4-bit scale, at `qs + QK_K/4`.

### (a) llama.cpp x86 (AVX2), quants.c:3260-3316

1. Grid compose: `_mm256_set_epi32(iq3xxs_grid[q3[7]], ..., iq3xxs_grid[q3[0]])`
   (x86:3290-3295) — **eight 32-bit table reads written directly into one ymm**, twice per
   loop iteration. This is the exact composition our layout also needs.
2. `memcpy(aux32, gas, 8)` staging (x86:3296).
3. Signs: four `signs64[(aux32[k] >> 7j) & 127]` per ymm (x86:3297-3300), `keven` table,
   parity already baked.
4. `_mm256_sign_epi8` on the activation (x86:3301-3302); `maddubs` (x86:3303-3304).
5. Scale `2*ls+1` via `set1_epi16` + `madd_epi16` (x86:3307-3308); two i32 accumulators;
   `fmadd`; `*s = 0.25f * hsum_float_8` (x86:3316).

### (b) llama.cpp arm64, quants.c:3864-3922

1. Grid compose: `ggml_vld1q_u32(iq3xxs_grid[q3[0]], ..., q3[3])` (arm:3896-3899) — the macro
   (ggml-cpu-impl.h:82-84) is a brace-initialised `uint32x4_t`, i.e. four scalar dword reads
   the compiler materialises with `ld1 {v.s}[lane]` or `ins`. Same shape as `set_epi32`.
2. Signs from `keven_signs_q2xs`, `vcombine_s8` of two 8-byte reads (arm:3901-3904).
3. `vmulq_s8` on the **weights** (arm:3905-3908).
4. Chained `ggml_vdotq_s32` ×2 per 32 weights (arm:3909-3910).
5. `sumf1 += vaddvq_s32(p1) * (0.5f + (aux32[0] >> 28))` (arm:3911-3912) — again the
   per-block cross-lane reduction and **float** accumulate; `*s = 0.5f * sumf` (arm:3916).

### (c) What ours does today

`emit_iq3xxs_gather` (gemm_gen.das:1418-1474). Cleanest of our five gathers:

- **One aligned `i32` load** for the aux word (gemm_gen.das:1438-1439) — the spelling
  IQ2_XXS should copy.
- `ksigns` lookup per sub-lane (gemm_gen.das:1442-1444).
- Two `i8` index loads (gemm_gen.das:1445-1450); grid is the **halved** `[256 x i32]` global
  (gemm_gen.das:1362-1377, 1455-1457).
- Same 3-GPR-op negate + 4-byte panel store (gemm_gen.das:1458-1462).
- Consumption: `iq2 = false` (gemm_gen.das:1817), so lo and hi share one accumulator and one
  per-32 signed scale (gemm_gen.das:1899-1901, 1920-1921) — matching the per-32 granularity.

### (d) Deltas, ranked

1. **Delete the panel** (`gather="reg"`) — the single biggest item, and the composition it
   needs is literally x86:3290's `set_epi32`, transposed from "8 grid words of one row" to
   "one grid word from each of 8 rows".
2. **Vector sign apply** (`psign="mask"` / folded into delta 1) — deletes 24 GPR ops and
   `VPABSB` per 32-byte vector.
3. **Bake the parity-completed sign byte at repack time.** Aux is 4×7 sign bits + a 4-bit
   scale, and the scale already lives on the scale row — so four full sign bytes fit in the
   same 4 bytes, exactly as for IQ2_XXS. Free; deletes the `ksigns` load.
4. `vpdpbssd` seat; `bias=128` seat (needs the `te.bias` clamp lifted, gemm_gen.das:2136);
   `pmaddwd` scale fold — as §3.

---

## 7. IQ3_S

Packing: 8-bit `qs` + one bit from `qh` = 9-bit index into `iq3s_grid[512]`; **explicit
sign bytes** (`x[i].signs`); nibble-packed per-32 scales.

### (a) llama.cpp x86 (AVX2), quants.c:3384-3475

1. **Vectorized index compose.** `idx_l = _mm256_cvtepu8_epi16(load 16 qs bytes)`;
   `idx.vec[j] = _mm256_and_si256(_mm256_sllv_epi32(set1_epi32(qh[..]), idx_shift), idx_mask)`
   with `idx_shift = {1..8}` and `idx_mask = 256`, then `OR` with the widened `qs`
   (x86:3430-3438). Eight 9-bit indices built in one ymm with a variable shift.
2. **The index goes through memory, in the safe direction.** `index_t` is a union of
   `__m256i vec[2]` and `uint32_t index[16]` (x86:3406-3411); the vector is stored and the
   dwords are read back scalar to address the grid (x86:3441-3450). One wide store feeding
   16 narrow, fully-contained loads forwards fine — the opposite of our panel's one wide
   load over 8 narrow stores.
3. **An explicit negative result on hardware gather**, x86:3440:
   `// At leat on my CPU (Ryzen 7950X), using _mm256_i32gather_epi32 is slower than
   _mm256_set_epi32. Strange.` — with the `vpgatherdd` version left commented out at
   x86:3441-3442. Do not spend time on a gather-instruction spelling.
4. Sign expansion: identical to IQ2_S — `set1_epi32(signs[0] | signs[1]<<16)`,
   `shuffle_epi8(mask1)`, `and(mask2)`, `cmpeq` (x86:3452-3459).
5. Sign application: `sub(xor(s2, q8), s2)` (x86:3455, 3460) — **on the activation**.
6. `maddubs` (x86:3464-3465); scale `2*ls+1` from the nibble pair via `set1_epi16` +
   `madd_epi16` (x86:3466-3469); two i32 accumulators; `fmadd`; **no global factor** —
   `*s = hsum_float_8(accumf)` (x86:3475).

### (b) llama.cpp arm64, quants.c:3926-4032

1. Index compose in vector: `vorrq_u16(vmovl_u8(qs_half), vandq_u16(vshlq_u16(vdupq_n_u16(qh),
   hshift), m256))` with `k_shift = {8,7,...,1}` (arm:3985-3986, 3992).
2. Same union round trip — `vec_index_t { uint16x8_t vec_index; uint16_t index[8]; }`
   (arm:3944-3947), stored then read back scalar for `ggml_vld1q_u32(iq3s_grid[idx.index[0]],
   ...)` (arm:3987-3996).
3. Sign expansion via `vqtbl1q_u8` + `vceqq_u8` + `vorrq_u8(..., m1)` → `±1`
   (arm:3998-4012); applied to the **weights** with `vmulq_s8` (arm:4004-4005, 4016-4017).
4. **Scales precomputed with a SWAR trick**: `memcpy(scales32, x[i].scales, 4)`, then
   `scales32[1] = (((scales32[0] >> 4) & 0x0f0f0f0f) << 1) | 0x01010101` and the twin for the
   low nibbles (arm:3976-3979) — eight `(2s+1)` scale bytes computed with four GPR ops per
   superblock.
5. `sumi1 += vaddvq_s32(p1) * scales8[ib32/2]` (arm:4020-4023) — per-block cross-lane
   reduction, scalar integer accumulate; `*s = sumf` (arm:4028).

### (c) What ours does today

`emit_iq3s_gather` (gemm_gen.das:1300-1359), the arm the followup entry describes.

- `qh` byte load per block (gemm_gen.das:1319-1321), sign byte load per sub-lane
  (gemm_gen.das:1323-1325), `qs` byte load + `shl`/`and`/`or` per grid word
  (gemm_gen.das:1327-1333). **All scalar, one byte at a time** — no `sllv`-style vector index
  compose.
- Grid: `[512 x i32]` private constant, `align 64` (gemm_gen.das:1260-1272), one `i32` load
  per 4 weights (gemm_gen.das:1339-1341).
- Sign: 16-entry nibble→byte-mask table (`dasllama.iq3s.smask`, gemm_gen.das:1274-1293), then
  `(gw ^ mw) + (mw & 0x01010101)` (gemm_gen.das:1342-1344).
- 4-byte panel store (gemm_gen.das:1345-1347).
- Consumption: shared per-32 signed scale path (gemm_gen.das:1899-1901, 1920-1921); scale
  applied as `vpmulld` on the i32 accumulator; one `sitofp` + FMA per superblock; global
  factor pre-baked into the scale row.

### (d) Deltas, ranked

| # | Delta | Candidate seat | Why |
|---|---|---|---|
| 1 | **Delete the panel** — compose 8 grid dwords per weight vector with `insertelement`. | `tune_perm(dot="maddubs", width=256, mr=8, gather="reg", requires="avx2")` | The 0.92x tail's stated cause, and the measurement in followup 61 supports it. |
| 2 | **Vector sign expansion off the plane.** Like IQ2_S, the sign bytes are explicit and plane-resident at stride 4 across rows (gemm_gen.das:1324) — one 32-byte column load + `vpshufb`/`and`/`cmpeq` gives the full 32-byte `±1`/mask vector. | `psign="expand"` | Deletes the `smask` table read *and* the 3-GPR-op negate *and* `llvm.abs`. |
| 3 | **Vectorized index compose** (`sllv` shape, x86:3430-3438 / arm:3985-3986) feeding a *wide-store / narrow-load* union — the safe memory direction — instead of per-byte `qs` loads. | `idx="vec"` | Only worth racing after delta 1; the win is ~4 GPR ops per grid word. Note llama.cpp deliberately keeps the *grid read* scalar even here. |
| 4 | Do **not** race `vpgatherdd` for the grid read. | — | x86:3440 records it losing to `set_epi32` on Zen 4. |
| 5 | Adopt the ARM SWAR scale expansion (arm:3976-3979) if the scale row is ever computed at runtime. | — | Ours pre-bakes scales in the repack, so this is already better; noted for completeness. |
| 6,7 | `vpdpbssd` seat; `bias=128` seat; `pmaddwd` scale fold — as §3. | | |

---

## 8. Verdict on `modules/dasLLAMA/followup_general.md` entry 61

Entry 61 says:

> The stamped gemv gathers each superblock into an alloca panel and then runs the vector dot
> — a store/load round trip per superblock that a single token never amortizes; llama.cpp's
> per-row form (grid words composed straight into vectors, signs applied to the ACTIVATION
> via shuffle+cmpeq/xor-sub, magnitudes kept unsigned for maddubs) edges it 57.0 vs 52.4
> tg128 on the zen2.

**Confirmed, for all five formats, not just IQ3_S:**

- The panel round trip is real and shared. Every one of `emit_iq3s_gather`,
  `emit_iq3xxs_gather`, `emit_iq2s_gather`, `emit_iq2xs_gather`, `emit_iq2xxs_gather` ends
  each decoded dword with `LLVMBuildStore(b, w, dp)` into `te.iq3s_panel`
  (gemm_gen.das:1345-1347, 1460-1462, 1560-1562, 1730-1732, 1792-1794), and
  `emit_block_iq4xs` reloads it as `<width/8 x i8>` (gemm_gen.das:1880-1883). The panel is a
  GEMV-only cost: in tile mode `te.iq3s_panel` is null (`kq_tile_gen_impl` never sets it,
  gemm_gen.das:3200-3244) and the byte-expanded panel is built once per row-group per
  token-block by `unpack_kq_panel_grp` (math_gen.das:2091), amortized over up to TB tokens.
  That is exactly why prefill wins and decode loses.
- llama.cpp does compose grid words straight into vectors (x86:2691, 2856, 3123, 3290, 3444;
  arm:3664, 3735, 3808, 3896, 3987) and never materializes a decoded-weight buffer.
- Magnitudes-stay-unsigned-for-`maddubs` is right on x86 (grid is the u8 operand at
  x86:2699, 2887, 3146, 3303, 3464).

**One clause needs correcting:**

- *"signs applied to the ACTIVATION"* is **not portable to our layout.** It is true of
  llama.cpp's x86 kernels only, and it is a consequence of their one-row-at-a-time shape.
  In our `mr`-interleaved GEMV the lanes of one weight vector are 8 different output rows
  with 8 different sign patterns, while the activation dword is splatted across all lanes —
  so a single `psign` on the activation cannot be correct. llama.cpp's own **arm64** kernels
  make the same concession for the same reason (`sdot` is signed×signed): they apply signs to
  the **weights** with `vmulq_s8` at arm:3672, 3743, 3826, 3905, 4004. Adopting
  "signs-on-activation" would mean abandoning row interleave (`mr = 1`), which would give
  back the per-row horizontal reduction we currently avoid entirely. Not worth it.
- What *is* portable, and is the real content of the delta, is: **keep the grid magnitudes
  unsigned and build the sign mask as a vector**, so `psign(x_splat, mask|1)` replaces
  `abs(panel_bytes)` + `psign(x_splat, panel_bytes)` and the 3-GPR-op-per-dword negate
  disappears with the panel. The `abs`+`psign` pair is dropped outright only by
  `vpdpbssd` (signed×signed, gemm_gen.das:344-347) or by `bias=128`
  (gemm_gen.das:339-343) — neither of which is seated for any grid format today.
- Entry 61 is also **under-scoped**: it is filed as an IQ3_S item, but the panel, the gather
  shape, the scalar sign negate and the `abs`+`psign` pair are byte-for-byte identical in all
  five gathers. One `gather="reg"` spelling in `emit_block_iq4xs` covers all five at once.

---

## 9. Cross-format section — what one spelling would cover

All five gathers are the same function with a different index/sign preamble. Ranked by
estimated decode-shape payoff:

### Tier 1 — one change, all five formats

**`gather="reg"`: compose the weight vector in registers, delete the panel.**

Where: `emit_block_iq4xs` (gemm_gen.das:1811-1957) and the five gather emitters. Restructure
the gather loop from *row-major* (`for r in mr { for blk, l, h }`, gemm_gen.das:1305-1352)
to *group-major* (`for blk, j, half { for r in mr }`), emitting `insertelement` of each row's
decoded dword into an `<rv x i32>` and bitcasting to `te.vwi8` — the same value
`load_vec(te, te.vwi8, wbase, noff)` returns today (gemm_gen.das:1880-1883), produced without
touching memory.

Why it is the top item:

- Removes `256 × mr / 4 = 512` scattered 4-byte stores per superblock (mr = 8) and the
  64 wide reloads, plus the `alloca` traffic.
- Removes the **wrong-direction store-forwarding hazard**: the reload at
  `(blk*16 + j*4)*mr` (gemm_gen.das:1875) is a 32-byte load spanning 8 distinct 4-byte
  stores written `mr*4 = 32` bytes apart (gemm_gen.das:1345). x86 forwards a narrow load out
  of a wide store; it does not forward a wide load out of several narrow stores. llama.cpp's
  own memory round trips (x86:2823 `aux_gindex`, x86:3406 `index_t`, arm:3944
  `vec_index_t`) are all in the *safe* direction and carry indices, never decoded weights.
- Worse at width 512 today, better after: the `mr = 16` seat writes 1024 stores into a
  4096-byte panel and reloads with 64-byte loads spanning 16 stores each.
- Rough per-32-byte-vector uop budget: ~51 now → ~20 composed.

Seats (one per existing perm row, five formats × four rows):

```
tune_perm(mr = 8, gather = "reg"),
tune_perm(dot = "maddubs",  width = 256, mr = 8,  gather = "reg", requires = "avx2"),
tune_perm(dot = "vpdpbusd", width = 256, mr = 8,  gather = "reg", requires = "avxvnni|avx512vnni"),
tune_perm(dot = "vpdpbusd", width = 512, mr = 16, gather = "reg", requires = "avx512vnni,avx512bw"),
```

(Suffix derivation: `suffix_piece` in llvm_tune.das:1317-1323 renders a string arg as
`name_value`, so these stamp as `..._gather_reg`; add them to each `tune(... fallback = ...)`
chain in priority order.)

### Tier 2 — one change, all five formats

**`psign="mask"` / `psign="expand"`: build the sign vector, stop negating in GPRs.**

Composed with tier 1: keep the raw grid dword as the unsigned magnitude (it already is —
ggml-common.h:561, 1018, 1053), compose a second vector of 0x00/0xFF masks, and apply
`vpor(mask, 1)` + `VPSIGNB` to the splatted activation — or the cheaper
`sub(xor(mask, x), mask)` conditional negate llama.cpp uses at x86:3136/3455. Deletes:

- the three GPR ops per dword at gemm_gen.das:1342-1344 / 1458-1460 / 1557-1559 /
  1728-1730 / 1790-1792 (~24 ops per 32-byte vector), and
- `abs_w` / `llvm.abs` / `VPABSB` from the inner loop (gemm_gen.das:370-375, 1888-1889).

Two sub-variants worth racing separately:

- `psign="mask"` — compose the mask vector from the existing 16-entry `smask` global with
  `insertelement` (8 more folded loads).
- `psign="expand"` — read the sign bits vectorized off the plane and expand with
  `vpshufb`+`and`+`cmpeq` (llama.cpp's `k_mask1`/`k_mask2`, x86:3090-3096 / arm:3782-3786).
  Cheapest for **IQ2_S** and **IQ3_S**, whose sign bytes are plane-resident at stride 4
  across rows (gemm_gen.das:1542, 1324); needs one `ksigns` scalar read per 8 weights for
  IQ2_XXS/IQ2_XS/IQ3_XXS unless the repack change below lands.

Note `llvm.x86.avx2.psign.b` is only wired at `width == 256` (gemm_gen.das:2185-2190); the
512-bit leg already falls back to a `select` (gemm_gen.das:358-362). The `xor`+`sub` form
works at both widths and would simplify that split.

### Tier 3 — seats only, zero generator change

**`dot = "vpdpbssd"` for all five formats.** The panel/composed bytes are signed and the
activation is signed, so `dot_lane` takes the native `s8·s8` path with no `abs` and no
`psign` (gemm_gen.das:344-347, 370-372). `perm_declines` already permits it at width 256
under `avxvnniint8` (gemm_gen.das:212-220). One line per format:

```
tune_perm(dot = "vpdpbssd", width = 256, mr = 8, requires = "avxvnniint8"),
```

Only helps AVX-VNNI-INT8 hosts (not zen2, not zen4), so file it as cheap coverage, not as
the fix for the measured tail.

### Tier 4 — repack-side, three of five formats

**Bake the parity-completed sign byte into the plane.** `ksigns_iq2xs` maps a 7-bit code to
the 8-bit sign byte (ggml-common.h:513). For **IQ2_XXS** and **IQ3_XXS** the aux word is
4×7 sign bits + a 4-bit scale, and the scale nibble already moved to the scale row
(gemm_gen.das:1746-1748) — so four full sign *bytes* occupy the same four bytes, at zero
size cost, and the `ksigns` load disappears (gemm_gen.das:1442-1444, 1775-1776). For
**IQ2_XS** it costs +1 bit per 8 weights (9 index bits + 8 sign bits > 16), so race it. This
also promotes IQ2_XXS/IQ3_XXS into the cheap `psign="expand"` class alongside IQ2_S/IQ3_S.

### Tier 5 — small, local, do anyway

- **IQ2_XXS aux word**: four `i8` loads + 3 shifts + 3 ORs (gemm_gen.das:1766-1770) where
  IQ3_XXS uses one aligned `i32` load of the identically-aligned field
  (gemm_gen.das:1438-1439). 6 uops × 8 blocks × mr rows per superblock, free to reclaim.
- **IQ2_XS packed word**: two `i8` loads + shift + OR (gemm_gen.das:1711-1716) where one
  2-byte load suffices (`lp % 4 ∈ {0,2}`, so the pair never straddles the 4-byte column).
- **Panel/vector load alignment**: `load_vec` hard-codes `align 1`
  (gemm_gen.das:376-379) although the alloca is `align 32` (gemm_gen.das:3082) and every
  offset is a multiple of `mr*4`. Moot once the panel is gone, but it also affects the tile's
  reads of the unpacked panel.
- **`gkstep` is inert for these formats**: `kq_gemv_gen_impl` pins `SliceArgs(kstep = 1)`
  (gemm_gen.das:3084) and never reads `p.gkstep`, while `perm_declines` still validates it
  (gemm_gen.das:169). Any `gkstep` seat added to a grid format would be a no-op clone.
- **`bias = 128` is clamped off** for every `kq` format at gemm_gen.das:2136
  (`te.bias = (te.mx4 || te.q51 || te.kq != 0) ? 0 : p.bias`). Since we synthesize the panel
  bytes ourselves, `w ^ 0x80808080` is a free per-byte `+128` (no carries), so lifting the
  clamp for the five panel formats would make the `bias128` lattice — which drops `abs` and
  `psign` entirely and folds the `-128·Σx` correction into the accumulator init
  (`bias_acc_init`, gemm_gen.das:400-427) — available on VNNI hosts. **Not** valid under
  `maddubs`: `(w+128) × x` pair sums reach 65280 and saturate `i16`, which is why the knob is
  documented `vpdpbusd`-only (gemm_gen.das:44-47, 170-171).

### What we already do better, and should not regress

- **No horizontal reduction anywhere.** llama.cpp's arm64 kernels pay 4-8 `vaddvq_s32` per
  superblock per row (arm:3678, 3752, 3845, 3911, 4021); IQ2_XXS and IQ3_XXS additionally
  accumulate in **float** per 32-weight block (arm:3678-3679, 3911-3912). Our lanes are rows,
  so the superblock accumulates in `i32` and converts once (gemm_gen.das:1943-1953).
- **Global factor pre-baked.** llama.cpp multiplies by `0.125f` / `0.25f` / `0.5f` per row at
  the end (x86:2713, 3161, 3316; arm:3684, 3760, 3916). Ours rides the repacked scale row.
- **Per-16 split accumulators for IQ2_S/IQ2_XS** (gemm_gen.das:1817, 1893-1898) match the
  formats' actual scale granularity without any extra reduction — structurally what
  llama.cpp's best ARM arm (IQ2_XS `vpaddq`+`vmlaq_s32`, arm:3751-3752) achieves, and better
  than what its IQ2_S arm does (scalar `vaddvq` × scalar scale, arm:3843-3846).
- **Q2_K needs no work.** Our `k2` arm decodes 2-bit lanes in-register with a shift and an
  `and` (gemm_gen.das:1049-1056) — the same technique as llama.cpp AVX2 (x86:1626-1629) and
  NEON (arm:1988-1996) — with no panel, no gather, no table. That is exactly why it measures
  at parity, and it is the control that proves the gap in the other five is the *decode*,
  not the dot or the fold.
