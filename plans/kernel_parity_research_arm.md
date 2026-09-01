# ARM NEON grid-format decode: what llama.cpp does per 32 weights that we do not

Scope: the five grid ("codebook") formats on Apple M1 Max, one thread, decode/GEMV
(m=4096, k=14336, one token). Read-only study of
`D:\Work\llama.cpp\ggml\src\ggml-cpu\arch\arm\quants.c` and
`D:\Work\daScript-iquant\modules\dasLLAMA\dasllama\dasllama_gemm_gen.das`.
No file in either tree was modified.

Measured (us per call, ours vs llama.cpp `test-backend-ops perf -b CPU`):

| format | ours | llama.cpp | ratio |
|---|---|---|---|
| iq2_xs  | 6375 | 3274 | 0.51x |
| iq2_xxs | 6063 | 3437 | 0.57x |
| iq3_xxs | 6735 | 4867 | 0.72x |
| iq3_s   | 6089 | 5682 | 0.93x |
| iq2_s   | 4932 | 4917 | 1.00x |

---

## 0. Ground facts established first

**The M1 takes the plain `__ARM_NEON` + dotprod path; there is no i8mm arm for any of the
five.** `__ARM_FEATURE_MATMUL_INT8` appears in `arch/arm/quants.c` only at lines 302, 315,
595, 608, 1155, 1168, 2336, 2360, 2569, 2966, 2984, 3175 — all inside the q4_0/q8_0/q4_K/q5_K
family. The five grid dots start at 3631 (iq2_xxs), 3693 (iq2_xs), 3767 (iq2_s), 3864
(iq3_xxs), 3926 (iq3_s) and each has exactly two arms: `#if defined(__ARM_NEON)` and the
scalar generic fallback. `ggml_vdotq_s32` is the native `vdotq_s32` when
`__ARM_FEATURE_DOTPROD` is set (`ggml/src/ggml-cpu/ggml-cpu-impl.h:319`), else a
`vmull_s8`+`vpaddlq_s16` emulation (`ggml-cpu-impl.h:310-316`). Our stamped M1 profile
records `"features": "neon;dotprod;fullfp16;lse"`
(`modules/dasLLAMA/performance/defaults/arm-neon.tune-defaults.json`, provenance block) —
dotprod yes, i8mm no. So both sides run the same primitive: `sdot`.

**Grid table element widths** (`ggml/src/ggml-common.h`):

| table | line | element | entries | bytes | weights per entry |
|---|---|---|---|---|---|
| `iq2xxs_grid` | 560 | `uint64_t` | 256 | 2048 | 8 |
| `iq2xs_grid` | 627 | `uint64_t` | 512 | 4096 | 8 |
| `iq2s_grid` | 758 | `uint64_t` | 1024 | 8192 | 8 |
| `iq3xxs_grid` | 1017 | `uint32_t` | 256 | 1024 | 4 |
| `iq3s_grid` | 1052 | `uint32_t` | 512 | 2048 | 4 |
| `ksigns_iq2xs` | 513 | `uint8_t` | 128 | 128 | (7-bit code -> sign byte) |
| `ksigns64` | 524 | `uint64_t` | 128 | 1024 | (7-bit code -> 8 x 0x00/0xFF) |
| `keven_signs_q2xs` | `arch/arm/quants.c:3595` | `int8_t[1024]` | 128 x 8 | 1024 | (7-bit code -> 8 x +/-1) |

`keven_signs_q2xs` is the ARM-only table: 128 entries of eight `+1`/`-1` bytes, read as
`uint64_t` through `signs64` (`quants.c:3646, 3708, 3879`). It is a **pre-expanded +/-1
vector**, so applying signs is one `vmulq_s8` with zero bit manipulation. This single table
is the largest structural difference in the whole comparison.

**Our shape on NEON.** `setup_tile_emit` pins `te.width = 128` on the sdot leg
(`dasllama_gemm_gen.das:1900`), `te.rv = width/32 = 4`, `te.rq = interleave/rv`
(1901-1902). The stamped `mr` is 8 for all five formats on ARM
(`arm-neon.tune-defaults.json`: `iq2sq8_tile_gen`..`iq3xxsq8_tile_gen` all `"mr8"`), so
rv = 4 rows per 16-byte vector, rq = 2 vectors per 8-row group. `sdot_lane`
(`dasllama_gemm_gen.das:309-316`) builds the **indexed** form: the activation is bitcast to
`v4i32`, one dword lane splatted, and the backend folds that into `sdot v,v,v[lane]`. So one
sdot = 4 rows x 4 weights = 16 byte-products.

**Both sides issue the same number of sdot per weight.** 32 weights x 1 row = 32
byte-products = 2 sdot for llama.cpp (`quants.c:3747-3750` etc.). Ours: per block per qd,
`for j in 0..3` x lo/hi = 8 sdot covering 4 rows x 32 weights = 128 byte-products, i.e.
2 sdot per row per 32 weights. **The dot is at exact parity. Everything below is the
surrounding work.**

**Our GEMV re-uses nothing.** `kq_gemv_gen_impl` drives `emit_slice(te, sa, ghead, 0, 1, "")`
(`dasllama_gemm_gen.das:2880`) — tokCount = 1. Every decoded weight is consumed by exactly
one sdot. The alloca panel (`LLVMBuildAlloca(..., 256 * te.interleave, "iq3s.panel")`, line
2845 — 2048 bytes at mr=8) therefore buys **no reuse whatsoever in decode**; it exists only
to separate the rolled gather loop from the unrolled dot body. The prefill/tile path is the
one that benefits, and there the panel is produced once per group by
`unpack_kq_panel_grp` (`dasllama_repack.das:953`) and amortized over 4 tokens — which is
exactly why every prefill tile is 1.9x-15x ahead while decode is behind.

---

## 1. Per-format op budget, side by side

Counting is per **one row x 32 weights** (llama.cpp's natural unit; ours divided down from
the 4-rows-per-vector shape). "mem" = load/store uops, "NEON" = vector ALU uops, "int" =
scalar integer ALU uops.

### 1.1 llama.cpp

**iq2_xxs** (`quants.c:3660-3680`, body covers 64 weights)
- `ggml_vld1q_s8_x4(q8)` (3662): 4 x 16B activation loads / 64w = **2 mem**
- `memcpy(aux32, q2, 16)` (3663): 2 x `ldr x` / 64w = **1 mem**
- grid: 8 x `vld1_s8(iq2xxs_grid + aux8[i])` (3664-3667), paired by `vcombine_s8` into
  `ldr d` + `ld1 {v.d}[1]` = **4 mem** + 4 byte-extracts = **4 int**
- signs: 8 x `vld1_s8(signs64 + ((aux32[k] >> s) & 127))` (3668-3671) = **4 mem** +
  4 `ubfx` = **4 int**
- `vmulq_s8` x4 (3672-3675) = **2 NEON**
- `ggml_vdotq_s32` x4 (3676-3677) = **2 NEON**
- epilogue `vaddvq_s32` + float scale (3678-3679) ~ **3 mixed**

**Total: 11 mem, 4 NEON, 8 int, ~3 epilogue ~ 26 uops.**

**iq2_xs** (`quants.c:3733-3753`, body covers 64 weights)
- q8 (3734) = **2 mem**
- grid: 8 x `vld1_s8(iq2xs_grid + (q2[i] & 511))` (3735-3738) = 4 index `ldrh` + 4 `and`
  + 4 grid `ldr d` (the `vcombine` pair is `ldr d` + `ld1 {v.d}[1]`, so 8 half-loads per
  64w = 4 per 32w) -> **8 mem + 4 int**
- signs: 8 x `vld1_s8(signs64 + (q2[i] >> 9))` (3739-3742) = **4 mem + 4 int** (the
  `ldrh` is shared with the grid lookup)
- `vmulq_s8` x4 (3743-3746) = **2 NEON**
- `vdotq` x4 (3747-3750) = **2 NEON**
- `vpaddq_s32` x3 + `vmlaq_s32` (3751-3752) = **2 NEON**
- per-superblock scale prep (3719-3729) amortized over 8 blocks ~ **2**

**Total: 14 mem, 6 NEON, 8 int ~ 28-30 uops.**

**iq2_s** (`quants.c:3808-3848`)
- q8 (3809) = **2 mem**
- grid: 8 x `vld1_s8(iq2s_grid + (qs[i] | ((qh[..] << s) & 0x300)))` (3810-3817) = 4 `ldrb`
  + 4 grid `ldr d` = **8 mem**, plus `<<`/`&`/`|` x3 per index = **12 int**
- signs are **plane bits**, expanded with the mask1/mask2 shuffle (3820-3824, 3829-3833):
  per 32 weights = 1 `vdupq_n_u32` + 2 `vqtbl1q_u8` + 2 `vandq` + 2 `vceqq` = **7 NEON**
- `vorrq_u8(vs, m1)` x2 + `vmulq_s8` x2 (3826-3827) = **4 NEON**
- `vdotq` x2 (3840-3843) = **2 NEON**
- 2 x (`vaddvq_s32` + nibble scale extract + `madd`) (3845-3848) ~ **6 mixed**

**Total: 10 mem, 13 NEON, 12 int ~ 37 uops.** llama.cpp's most expensive of the five, and
indeed its slowest but one.

**iq3_xxs** (`quants.c:3894-3912`)
- q8 (3894) = **2 mem**, `memcpy(aux32, gas, 8)` (3895) = **0.5 mem**
- grid: `ggml_vld1q_u32(iq3xxs_grid[q3[0]], .., [q3[3]])` (3896-3899). That macro is a
  plain brace initializer on non-MSVC (`ggml-cpu-impl.h:84`), so clang emits 4 index `ldrb`
  + 4 `ldr w` + 3 lane inserts per q register, 4 registers per 64 weights ->
  **16 mem + 12 int per 64w = 8 mem + 6 int per 32w**
- signs: 8 x `vld1_s8(signs64 + ...)` (3900-3903) = **4 mem + 4 int**
- `vmulq_s8` x4 (3905-3908) = **2 NEON**, `vdotq` x4 (3909-3910) = **2 NEON**
- epilogue (3911-3912) ~ **4 mixed**

**Total: 14.5 mem, 4 NEON, 10 int ~ 40 uops.**

**iq3_s** (`quants.c:3983-4022`)
- q8 (3983) = **2 mem**
- index vector: `vld1q_u8(qs)` (3985) then `vmovl_u8`/`vshlq_u16`/`vandq`/`vorrq`
  (3986, 3991) = **0.5 mem + ~6 NEON per 32w**
- **the `vec_index_t` union (3941-3944)**: `idx.vec_index` is written as a vector and then
  read back as `idx.index[0..7]` scalars (3987-3990, 3992-3995). That is a store-forwarding
  round trip through the stack — 1 vector store + 8 `ldrh` per half, **~9 mem per 32w**
- grid: 8 x `ldr w` + 6 lane inserts per 32w = **8 mem + 6 int**
- signs: the mask1/mask2 expansion (3998-4002, 4007-4011) = **7 NEON**, plus `vmulq_s8` x2
  (4004-4005, 4015-4016) = **2 NEON**
- `vdotq` x2 (4018-4019) = **2 NEON**, epilogue (4021-4022) ~ **3**

**Total: ~19.5 mem, ~17 NEON, ~6 int ~ 49 uops.** llama.cpp's slowest of the five, matching
its 5682 us.

### 1.2 Ours

Two loops per superblock, both walked per row group of mr = 8:

**(A) `emit_kq_gather` (`dasllama_gemm_gen.das:1451-1486`)** — a rolled loop over the mr
rows; each iteration emits **64 straight-line dword decodes** (`for blk in 0..7`,
`for l in 0..3`, `for h in 0..1`, line 1470-1477), each decoding **4 weights of one row**
and storing them with `LLVMBuildStore` into the panel at `panel_group_off` (1443-1449,
1478-1479).

**(B) the dot body inside `emit_block_iq4xs` (1573-1712)** — per block: 2 activation
`v16i8` loads shared across all 8 rows (1620-1621); per (blk, qd) one `kq_sign_bytes`
(1632); per (blk, j, qd) two panel `v16i8` loads (1642-1643), two `apply_sign_col`
(1645-1646), two `dot_lane` (1659-1662).

Per **one row x 32 weights**, gather side:

| format | decode site | mem uops | int uops |
|---|---|---|---|
| iq2_xs | `decode_iq2xs_dword` 1358-1367 | 2 column `ldr w` (`wg_col_byte` 1308-1315) + 4 grid `ldr x` (`grid_pair_half` 1324-1330) + **8 panel `str w`** = **14** | 12 byte extracts + 8 `orr/lsl` for the u16 + 4 `and #511` + 4 `lsr #32` = **28** |
| iq2_xxs | `decode_iq2xxs_dword` 1369-1373 | 4 `ldrb` (`wg_byte` 1293-1296) + 4 grid `ldr x` + **8 `str w`** = **16** | ~8 |
| iq2_s | `decode_iq2s_dword` 1347-1356 | 1 qh `ldrb` + 4 qs `ldrb` + 4 grid `ldr x` + **8 `str w`** = **17** | ~16 |
| iq3_xxs | `decode_iq3xxs_dword` 1342-1344 | 2 column `ldr w` + 8 grid `ldr w` (`grid_word` 1317-1321) + **8 `str w`** = **18** | ~12 |
| iq3_s | `decode_iq3s_dword` 1332-1340 | 1 qh `ldrb` + 8 qs `ldrb` + 8 grid `ldr w` + **8 `str w`** = **25** | ~24 |

Per **one row x 32 weights**, dot side (shared shape, divided by 4 rows per vector):

- panel loads: 8 x `ldr q` / 4 rows = **2 mem**
- activation: 2 x `ldr q` / 8 rows = **0.25 mem**
- `apply_sign_col` (1416-1429): the `idx` shuffle depends only on `l`, so 4 distinct `tbl`
  per (blk, qd) plus 8 x (`and` + `cmpeq` + `eor` + `sub`) = 36 NEON / 4 rows = **9 NEON**
- `sdot`: 8 / 4 rows = **2 NEON**
- epilogue (sub-scale load, `mul`, `add`, 1683-1706) ~ **1.5 NEON**
- `kq_sign_bytes` (1378-1414):
  - **iq2_s (kq 23) and iq3_s (kq 33)**: the plane carries the sign bytes, so it is
    **one `ldr q`** (1385-1388) = **0.25 mem, 0 NEON**
  - **iq2_xs (24), iq2_xxs (25), iq3_xxs (34)**: 1-2 `ldr q` plus, for each of 4 sign
    groups, code extraction + a **3-step parity chain** (`lshr`/`xor` x3, lines 1404-1406)
    + `and`/`shl`/`orr` + the byte pack = ~13 NEON x 4 = **53 NEON / 4 rows = 13.25 NEON**

**This is the single biggest asymmetry.** llama.cpp never computes a parity: it indexes
`keven_signs_q2xs` (`quants.c:3595`), a table whose entries already carry the eighth,
parity-implied sign. We recompute the eighth sign in registers, for every sign group, of
every block, of every superblock, of every row.

### 1.3 A port model that fits the measurements

M1 Firestorm: 8-wide issue, ~3 load units, ~2 store units (~3 memory uops/cycle sustained),
4 NEON pipes, ~6 integer ALU pipes, 3.22 GHz. Modelling each loop as
`max(mem/3, NEON/4, int/6)` cycles per row per 32 weights:

| format | gather cyc | dot cyc | total cyc | predicted us | measured us | error |
|---|---|---|---|---|---|---|
| iq2_s   | 5.7 | 3.2  | 8.9  | 5070 | 4932 | +3% |
| iq2_xs  | 4.7 | 6.45 | 11.15| 6354 | 6375 | 0% |
| iq2_xxs | 5.3 | 6.45 | 11.75| 6696 | 6063 | +10% |
| iq3_s   | 8.3 | 3.2  | 11.5 | 6553 | 6089 | +8% |
| iq3_xxs | 6.0 | 6.45 | 12.45| 7094 | 6735 | +5% |

(predicted us = cycles x 1.835e6 row-32w units x 1e6 / 3.22e9; 4096 x 14336 / 32 = 1.835e6.)

**The model tracks all five within 10%, which means our kernel is genuinely port-saturated.**
The same model applied to llama.cpp under-predicts by 20-50% (iq2_xs: 4.67 cyc -> 2661 us vs
3274 measured; iq3_s: 6.5 cyc -> 3705 vs 5682), because llama.cpp's serial
index-load -> grid-load chains and per-32 float accumulation leave it latency-bound with
issue slack. Practical consequence: **op count is our currency and only our currency; we
cannot buy time back with better scheduling, and llama.cpp's measured times are an upper
bound on what its op mix actually costs.**

The model also reproduces the *ranking* exactly, and it decomposes into two independent
additive penalties:

- **+3.25 cyc/row/32w** if the format's signs are ksigns codes rather than plane bytes
  (iq2_xs, iq2_xxs, iq3_xxs).
- **+2.6 cyc/row/32w** if the grid is `u32` (4 weights/entry) rather than `u64`
  (8 weights/entry), because the gather issues twice the grid loads (iq3_s, iq3_xxs).

iq2_s pays neither and sits at 1.00x. iq3_s pays only the second and sits at 0.93x. The
three at 0.51x-0.72x are exactly the three that pay the parity chain. **The 0.51x/0.57x
outliers are not "ARM is hard"; they are one identifiable code path.**

---

## 2. The deltas, ranked by decode payoff on M1

Each entry is tagged **emitter change** / **repack change** / **seat**, with its x86
exposure. Zen2 is at 0.99x-1.32x on these five, so nothing here may touch the AVX2 lattice
without a re-measure.

### D1 — the gather decodes one dword per scalar step and round-trips 32 B/row/block through an alloca

**Payoff: the largest. Tag: emitter change (GEMV-only, sdot-leg-only). x86: none.**

`emit_kq_gather` decodes 4 weights at a time (`decode_kq_dword`, 1431-1438) and stores them
as a 4-byte `str` (1478-1479); the dot body then reloads them as `v16i8`
(1642-1643). Per row per 32 weights that is **8 stores + 2 loads = 10 of the ~14-25 memory
uops** in the gather. Since tokCount = 1 in decode (line 2880), those 2 KB are written and
read back once, for nothing.

Worse, the 4-byte granularity is *forced* by the panel's element order. For the u64-grid
formats one grid entry is 8 consecutive weights `l*8 .. l*8+7`, but
`panel_group_off` (1443-1449) sends `e = l*8` and `e = l*8+4` to different dword groups
`mr*4 = 32` bytes apart (the same split is visible in the tile's unpack:
`dasllama_repack.das:896-901`, `d1`/`d2` computed separately, then two 4-byte
`wp1[0]`/`wp2[0]` stores). So a single 8-byte grid load costs two 4-byte stores plus a
`lsr #32` to split it (`grid_pair_half`, 1324-1330).

llama.cpp never splits: `vcombine_s8(vld1_s8(grid+i), vld1_s8(grid+j))` (3735) puts two
whole u64 entries — 16 consecutive weights of one row — into one q register with two loads
and no store at all.

### D2 — the three ksigns formats synthesize the eighth sign bit with a 3-step parity chain, per group, per block, per row

**Payoff: 3.25 cyc/row/32w, ~29% of iq2_xs's time. Tag: emitter change (if D1 lands) or
repack change (standalone). x86: shared code, must re-measure zen2.**

`kq_sign_bytes` lines 1400-1412: `code` -> `par = code ^ (code>>4)`, `par ^= par>>2`,
`par ^= par>>1`, `sb = code | ((par&1)<<7)`, then a 4-way byte pack. 13 vector ops per sign
group, 4 groups per block, on `<4 x i32>` — 53 NEON uops per (4 rows, 32 weights).

llama.cpp: `vld1_s8(signs64 + code)` — one load of a pre-expanded +/-1 vector
(`quants.c:3739-3742`, table at 3595), then one `vmulq_s8`. The parity is baked into the
table.

Note the comment drift here: lines 1484-1485, 1507, 1529 and 1549 all describe "the shared
ksigns table" and "the shared smask table" as emitted globals, but the `*_emit_globals`
functions only create the grid global — no ksigns or smask constant is emitted, and
`kq_sign_bytes` reads none. Those four comment fragments are stale.

### D3 — the u64 grid's 8 weights are decoded as two separate 4-weight steps

**Payoff: it is what makes D1's 8 stores 8 instead of 4, and it doubles the index work for
iq2_xs. Tag: emitter change. x86: same code path, gated by the same `gather` flag.**

`grid_pair_half` (1324-1330) issues one 8-byte load and then `trunc`s the low or high half
per call. LLVM's CSE folds the two calls onto one load, but the *consumers* are two
independent 4-byte stores at unrelated panel offsets, so the pair can never become a single
8-byte store.

### D4 — the packed plane's 4-byte column layout costs a dword load plus shift and mask per index byte

**Payoff: ~2 cyc/row/32w on iq2_xs specifically. Tag: repack change or seat. x86: the
current shape was chosen ON x86.**

`wg_col(mr, pos) = (pos/4)*mr*4 + pos%4` (1304) interleaves rows in 4-byte columns, so
`wg_col_byte` (1308-1315) reads a dword and shifts the byte out. For iq2_xs the 16-bit qs
word is then rebuilt from two such bytes (`decode_iq2xs_dword`, 1358-1367: `qlo | (qhi<<8)`),
where llama.cpp does one `ldrh` (3735). That is 12 extraction + 8 reassembly ops against
llama.cpp's 4 loads.

The doc-comments on 1342 and 1358 record the measurements that chose this shape
("the column read measured 7862 -> 6602 us", "6501 -> 4702 us; one u16 load measured slower
than two bytes"). Those figures are from the x86 development box, and the same comment on
1342 already admits the column read *lost* for iq3s/iq2s/iq2xxs. **The decode shape is
currently one global decision made on one ISA.** It should be a seat.

### D5 — the sign application is a 5-op masked negate rather than one multiply

**Payoff: small on its own (~1 cyc/row/32w), large once D1 lands. Tag: emitter change.**

`apply_sign_col` (1416-1429): `tbl` broadcast of the row's sign byte, `and` with a per-lane
bit constant, `cmpeq`, `eor`, `sub`. Against llama.cpp's `vmulq_s8` with a table-loaded
+/-1 vector. Our form costs 4 ALU ops per weight vector plus a shared `tbl`; llama.cpp's
costs 1 multiply plus the 2 loads already counted.

But this is a *consequence*, not a cause: our 16-byte vector holds **4 different rows**, so
a +/-1 vector for it would need four separate table entries and four lane inserts. The
masked negate is the correct choice given the current layout. It is only fixable by changing
the layout (D1/section 3a).

Note the cost is already close to llama.cpp's for iq2_s/iq3_s: our 9 NEON/row/32w against
llama.cpp's 11 for the mask1/mask2 expansion (`quants.c:3820-3827`). This is precisely why
iq2_s measures 1.00x.

---

## 3. The three proposals evaluated

### (a) A width-128 gather composing u64 grid entries as 64-bit lanes: 2 rows x 8 weights per vector

**Verdict: this is the fix. Adopt it.**

Layout: vector `W = [ row A weights e..e+7 | row B weights e..e+7 ]`, one 16-byte register
from two 8-byte grid loads (`ldr d` into lane 0, `ld1 {v.d}[1]` into lane 1 — in LLVM, a
load plus an `insertelement` into a `<2 x i64>`).

The dot then uses the **non-indexed** `sdot Vd.4S, Vn.16B, Vm.16B`, whose lane i is
`Vn[4i..4i+3] . Vm[4i..4i+3]`. Feed it `X = [ x(e..e+3) | x(e+4..e+7) | x(e..e+3) |
x(e+4..e+7) ]`, i.e. the 8 activation bytes replicated — exactly `ld1r {v.2d}, [xp]`, one
instruction, no lane-splat shuffle at all. Then:

- lane 0 = row A . x(e..e+3), lane 1 = row A . x(e+4..e+7)
- lane 2 = row B . x(e..e+3), lane 3 = row B . x(e+4..e+7)

Accumulate all 4 element-groups of a block into **one** `<4 x i32>` per row pair; at block
end, one `addp` of two row-pair accumulators yields `[rowA, rowB, rowC, rowD]` — **the exact
`<4 x i32>` shape `a[]` already has** (line 1659-1662), so the entire scale/fold epilogue
(1683-1711) is untouched. `pairwise_add_i32` (909-917) already emits that `addp`.

Products per sdot: 2 rows x 8 weights = 16, identical to today's 4 x 4. **No dot regression.**

Same intrinsic as today (`llvm.aarch64.neon.sdot`, `<4 x i32>`/`<16 x i8>`, declared at
1911-1918) — only the operand construction changes, so no new decline rail.

Projected budget per **2 rows x 32 weights** for iq2_xs, keeping the panel:

| | uops |
|---|---|
| index loads (a row pair's 4+4 index bytes are contiguous in the column -> one `ldr x`) | 2 mem |
| grid: 8 u64 loads | 8 mem |
| signs: 8 loads from a +/-1 table, indexed by the 7-bit code | 8 mem |
| `vmulq_s8` x4 | 4 NEON |
| panel: 4 x 16-byte stores | 4 mem |
| dot: 4 panel loads + 4 sdot + 1 `ld1r` | 5 mem, 4 NEON |
| index extraction | ~16 int |

mem 27 -> 9 cyc, NEON 8 -> 2 cyc, int 16 -> 2.7 cyc. **9 cyc per 2 rows = 4.5 cyc/row/32w**
against today's 11.15. At the fitted 572 us/cycle-unit that is **iq2_xs ~2570 us, i.e.
1.27x AHEAD of llama.cpp's 3274**. Even a 50% miss lands at parity.

If the gather is *fused* into the dot body (legitimate in decode, where tokCount = 1 and the
panel buys nothing), the 4 stores and 4 loads vanish: ~19 mem -> 6.3 cyc per 2 rows =
**3.2 cyc/row/32w, ~1810 us**. Cost: the superblock body unrolls to 4 row-pairs x 32
element-groups x ~6 ops ~ 770 instructions, which is actually *smaller* than today's
combined dot body (~900) plus gather body. Worth trying second.

Notes and caveats:

- **The +/-1 sign table replaces D2 entirely.** Under this layout the sign vector for a row
  pair is two 8-byte loads from a `keven_signs`-equivalent global indexed by the 7-bit code —
  no parity chain, no plane change, no repack. D2 becomes unnecessary if (a) lands.
- **For iq2_s / iq3_s** (signs are plane *bits*, not codes) emit a 256-entry x 8-byte +/-1
  table indexed by the raw sign byte (2 KB private constant, same shape as the grid globals
  at 1273-1291) so all five formats share one sign path.
- **For the u32-grid formats** (iq3_xxs, iq3_s) a row's 8 weights are two grid entries, so
  building the half costs 2 loads (`ldr s` + `ld1 {v.s}[1]`) instead of 1 — 16 grid loads per
  (2 rows, 32w) instead of 8. Projected ~5 cyc/row/32w, still ~2.3x better than today and
  ahead of llama.cpp, which pays the same 8 u32 loads per row plus 6 lane inserts
  (`quants.c:3896-3899`, `ggml-cpu-impl.h:84`).
- **Blast radius is small and ARM-only.** The panel is the GEMV's private alloca (line 2845),
  the new shape lives entirely inside the `if (gather)` branch (line 1581), and the whole
  thing is gated on `te.dotKind == DOT_SDOT`. **No plane change, no repack change, no tile
  change, no x86 exposure.** Prefill (already 1.9x-15x ahead) is untouched.
- **Make it a seat for one tuning round.** Add a perm token (`gather="pair"` vs the current
  `"dword"`) parsed in `parse_perm` (101-125) so the M1 sidecar proves it per format before
  it becomes the default. The existing seats are one string per format
  (`iq2xsq8_tile_gen => "mr8"`), so the token composes with `mr8` without disturbing x86's
  `dot_maddubs_width256_mr8`.

### (b) ARM gather via vector table lookup (`vqtbl`)

**Verdict: impossible for the grid. Not worth an experiment.**

NEON `TBL`/`TBX` index into 1 to 4 *consecutive V registers* — a 16, 32, 48 or 64-byte
table, with out-of-range indices producing zero. The smallest grid here is `iq3xxs_grid`,
256 x `uint32_t` = **1024 bytes** (`ggml-common.h:1017`) — 64 V registers, 16x the ISA
maximum. `iq2xxs_grid` is 2048 bytes (`ggml-common.h:560`). There is no NEON gather
instruction of any kind, and the M1 has no SVE (profile features string:
`neon;dotprod;fullfp16;lse`). So the grid must be read with scalar loads on this ISA, and
llama.cpp does exactly that (`vld1_s8`/`ldr w` per entry) for the same reason.

Where `tbl` *is* the right tool, and where both sides already use it:

- the 16-entry nibble codebook for iq4_nl/iq4_xs — we already emit `llvm.aarch64.neon.tbl1`
  (`dasllama_gemm_gen.das:1929-1936`, `lut_lookup` 632-642)
- broadcasting a sign byte across its four lanes — `apply_sign_col`'s constant
  `shufflevector` (1421-1422) lowers to `tbl`, and llama.cpp's `mask1` shuffle
  (`quants.c:3822`, `4000`) is the same instruction

No format in this set has a grid small enough for a `vqtbl4q` sub-table.

### (c) Sign application: `vmulq_s8` with a table-derived sign vector vs our and+cmpeq+xor+sub

**Verdict: our masked negate is not the defect. The defect is the sign *source*, and the
multiply form is unreachable without (a).**

Cost accounting, per one row x 32 weights:

| scheme | ops |
|---|---|
| llama.cpp, ksigns formats (`quants.c:3739-3746`): 4 `vld1_s8` + 2 `vmulq_s8` | 4 mem + 2 NEON |
| llama.cpp, plane-bit formats (`quants.c:3820-3827`): dup + 2 tbl + 2 and + 2 ceq + 2 orr + 2 mul | 11 NEON |
| ours, `apply_sign_col` (1416-1429): 4 tbl + 8 x (and, cmpeq, eor, sub), / 4 rows | 9 NEON |
| ours, `kq_sign_bytes` ksigns arm (1400-1412): 53 NEON / 4 rows | 13.25 NEON |
| ours, `kq_sign_bytes` plane arm (1385-1388): 1 load / 4 rows | 0.25 mem |

So our *application* (9) already beats llama.cpp's bit expansion (11) and is within reach of
its multiply form (2 NEON + 4 mem). The 13.25 NEON of *code-to-byte synthesis* is the whole
problem, and it exists only because the format stores a 7-bit code where iq2_s and iq3_s
store a byte.

Two ways to kill it:

1. **Under (a)**: index a +/-1 table with the 7-bit code and multiply. Zero synthesis, zero
   plane change. This is llama.cpp's move, and it is only available once a vector holds
   whole 8-weight runs of a *single* row (a 4-row-interleaved vector would need 4 separate
   table entries plus 4 lane inserts, which is why the masked negate was correct until now).
2. **Standalone (repack change, if (a) is deferred)**: store the expanded 8-bit sign byte in
   the plane instead of the 7-bit code, turning iq2_xs/iq2_xxs/iq3_xxs into the iq2_s/iq3_s
   shape.
   - **iq2_xxs**: the aux32 column already holds 4 x 7-bit codes plus a 4-bit `ls` in 4
     bytes, and the scale already lives in the scale plane (`IQ2XXS_SSB` comment,
     `dasllama_kqformat.das:118`), so 4 sign bytes fit in the same 4 bytes.
     **Size-neutral** (`IQ2XXS_QSB` stays 64, line 117).
   - **iq3_xxs**: identical — 8 blocks x 4 aux bytes -> 8 x 4 sign bytes.
     **Size-neutral** (`IQ3XXS_QSB` stays 96, line 111).
   - **iq2_xs**: the 9-bit index does not fit a byte. Restructure to iq2_s's shape —
     32 index low bytes + 4 high-bit bytes + 32 sign bytes = 68 B against today's 64
     (`IQ2XS_QSB`, line 116). **+6%.**
   Projected under this alone: iq2_xs 6375 -> ~4520 (0.72x), iq2_xxs 6063 -> ~4390 (0.78x),
   iq3_xxs 6735 -> ~4980 (0.98x). Real, but short of parity for the iq2 pair, which is why
   (a) is the recommendation and this is the fallback.

   **x86 caution for option 2**: it changes the on-disk plane, the repack, the tile unpack
   (`dasllama_repack.das:874-949`) and both ISAs. The +6% on iq2_xs is affordable on
   bandwidth grounds — the decode moves 16 MB per call in 6375 us = 2.5 GB/s against M1
   Max's tens of GB/s single-core, so both implementations are ~15-25x off the memory roof
   and firmly compute-bound — but zen2's iq2_xs must be re-measured before it lands. iq2_xxs
   and iq3_xxs being size-neutral carry no bandwidth risk on either ISA.

---

## 4. Recommended order

1. **(a) as a seat**, u64-grid formats first (iq2_xs, iq2_xxs, iq2_s), panel retained,
   signs from an emitted +/-1 table. Emitter-only, GEMV-only, sdot-leg-only, no x86
   exposure. Model says 0.51x -> ~1.27x on iq2_xs; measure before believing.
2. **(a) extended to the u32-grid formats** (iq3_xxs, iq3_s) with the 2-loads-per-half
   build. Model says ~5 cyc/row/32w, comfortably ahead.
3. **Fuse the gather into the dot body** (drop the panel in decode only) once (a) is
   stamped. Model says a further ~30%. Watch generated code size.
4. **Make the decode shape a seat** regardless (D4): the column-read-vs-byte-read choice at
   lines 1342 and 1358 was measured on x86 and is currently binding on ARM.
5. **Only if (a) is abandoned**: the plane sign-byte repack for iq2_xxs and iq3_xxs
   (size-neutral, safe on both ISAs), and separately iq2_xs (+6%, needs a zen2 re-measure).

Incidental finding for whoever touches these lines: the "shared ksigns table" and "shared
smask table" described in the comments at `dasllama_gemm_gen.das:1484-1485, 1507, 1529, 1549`
are not emitted by the functions those comments head, and `kq_sign_bytes` reads neither.
