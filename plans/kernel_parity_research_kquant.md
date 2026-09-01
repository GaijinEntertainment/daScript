# Why our zen2 decode kernel loses on Q3_K / Q5_K / Q6_K and wins on Q4_K

Read-only study. Reference tree `D:\Work\llama.cpp`, ours `D:\Work\daScript-iquant`. No file in
either tree was modified.

Measured, zen2 AVX2, 1 thread, m=4096 k=14336, one token (us/call, ours vs llama.cpp):

| fmt | ours | llama.cpp | ratio | weight+scale bytes / 256-weight row |
|---|---|---|---|---|
| q4_K | 2089 | 2539 | **1.22x ahead** | ours 148, llama 144 |
| q2_K | 2146 | 2121 | 0.99x | ours 84, llama 84 |
| q3_K | 3317 | 2638 | **0.80x** | ours 114, llama 110 |
| q5_K | 3628 | 3286 | **0.91x** | ours 180, llama 176 |
| q6_K | 4203 | 3785 | **0.90x** | ours 210, llama 210 |

---

## 0. Which llama.cpp code path the reference numbers came from

**The plain `ggml_vec_dot_*_q8_K` in `ggml/src/ggml-cpu/arch/x86/quants.c`, not the repack path.**

- The repack ("extra buffer type") GEMV only fires when the weight tensor was allocated in
  `ggml_backend_cpu_repack_buffer_type()` — `D:\Work\llama.cpp\ggml\src\ggml-cpu\repack.cpp:4774-4781`.
- `test-backend-ops` allocates every tensor with `ggml_backend_alloc_ctx_tensors(ctx, backend)`, i.e.
  the backend's *default* buffer type — `D:\Work\llama.cpp\tests\test-backend-ops.cpp:1546`,
  `:1555`, `:1805`. So the repack buffer type is never selected and the repack GEMV never runs.

What repack support even exists on x86 matters for the endgame, though:

- **q4_K has an AVX2 repack GEMV** — `repack.cpp:4599-4604` selects `q4_K_8x8_q8_K` on
  `ggml_cpu_has_avx2()`; the kernel is `ggml_gemv_q4_K_8x8_q8_K`,
  `ggml/src/ggml-cpu/arch/x86/repack.cpp:1464`, AVX2-guarded at `:1486`.
- **q2_K's repack is AVX512-only** — `repack.cpp:4627-4632` gates on `ggml_cpu_has_avx512()`;
  the kernel `ggml_gemv_q2_K_8x8_q8_K` (`arch/x86/repack.cpp:1713`) is AVX2-guarded at `:1732`
  but is never *selected* on a zen2 host.
- **q3_K, q5_K and q6_K have no x86 repack at all** — `repack.cpp:4644-4659` offers q5_K/q6_K
  only under `ggml_cpu_has_neon()`; q3_K appears nowhere in the selector.

So for q3/q5/q6 on zen2, the vec_dot we compared against is the only thing llama.cpp has, and our
comparison is fair. **For q4_K it is not the whole story**: a real llama.cpp run with a loaded model
would take `ggml_gemv_q4_K_8x8_q8_K` instead, which is a row-interleaved 8-column kernel much closer
in shape to ours. Our +22% on q4_K is against llama's *fallback*.

**AVX512 does not change any of these four kernels.** Each function has exactly one
`#if defined __AVX2__ ... #elif defined __AVX__` pair and no AVX512 arm: q3_K `quants.c:1784`/`:1884`,
q4_K `:2057`/`:2122`, q5_K `:2235`/`:2314`, q6_K `:2439`/`:2510`. The VNNI helper that does exist
(`mul_sum_us8_pairs_float`, `quants.c:105-119`) is used only by the q4_0/q8_0-family dots; none of
the K-quant dots call it. On an AVX512 host llama.cpp runs the identical AVX2 code for these four.

---

## 1. Per-format op budgets, side by side

### Ground rules for the counts

- llama.cpp: one 32-byte vector holds **32 consecutive weights of one row**; the activation vector
  is a plain 32-byte `q8` load shared by that vector.
- Ours (seat `dot_maddubs_width256_mr8`, from
  `D:\Work\daScript-iquant\modules\dasLLAMA\performance\defaults\x86-avx2.tune-defaults.json:35-38`,
  giving `width=256`, `mr=8`, `dot=maddubs`; `rv = width/32 = 8`, `rq = mr/rv = 1` —
  `dasllama_gemm_gen.das:1883-1884`): one 32-byte vector holds **4 bytes from each of 8 rows**; the
  activation is one dword broadcast from memory (`bcast_dword`, `dasllama_gemm_gen.das:844-853`).
  So one of our vectors also delivers 32 weight slots — the *unpack cost per vector* is the
  apples-to-apples comparison.
- Our unit of emission is a **site** = one `(blk, j)` pair, `blk` in 0..7, `j` in 0..3, 32 sites per
  superblock — `dasllama_gemm_gen.das:1002` (`for blk`), `:1027` (`for j`). Each site produces two
  32-byte vectors (`wlo`, `whi`). One superblock-group = 8 rows x 256 weights = 2048 slots.
- I count AVX2-level vector operations after the obvious InstCombine folds (in particular
  `shl(and(lshr(X,a),m),b)` collapses to one shift plus one mask; LLVM does this before i8-vector
  shift legalization, so our `(x >> a) & m << b` chains realize as 2 ops, not 4). Scalar prologue
  arithmetic is not counted on either side. These are static counts, not measured uops.

### Q4_K — the control we win

**llama.cpp** (`quants.c:2057-2113`). Prologue: the 12-byte 6-bit scale/min packing is unpacked
with three scalar 32-bit words — `:2069-2074` (`memcpy(utmp,...,12)` then the kmask1/2/3 shuffle,
i.e. `get_scale_min_k4` done four sub-blocks at a time), widened once with `vpmovzxbw` at `:2079`.
The min term is folded **once per superblock**: bsums load `:2081`, `phaddw` `:2082`,
`pmaddwd` against the 8 mins `:2083`, one `fmadd` `:2084` — the whole eight-sub-block min dot is a
single `pmaddwd`, because the 8 mins of one row sit in one 128-bit register.

Main loop `:2091-2110`, 4 iterations of 64 weights:

| what | line | ops |
|---|---|---|
| 2 scale shuffles | 2093-2094 | 2 |
| 1 nibble load | 2096 | 1 |
| `q4l = and(bits, 0xF)` | 2097 | 1 |
| `q4h = and(srli(bits,4), 0xF)` | 2098 | 2 |
| 2 activation loads | 2100, 2104 | 2 |
| 2 `vpmaddubsw` | 2101, 2105 | 2 |
| 2 `vpmaddwd` (widen **and** scale in one op) | 2102, 2106 | 2 |
| 2 `vpaddd` | 2107, 2109 | 2 |

15 per 64 weights x 4 = 60, plus ~15 prologue/epilogue vector ops -> **~75 ops per 256 weights of
one row**. Weight-plane loads issued: 4, distinct: 4 (1.00x).

**Ours** (`dasllama_gemm_gen.das:1059-1062` for the unpack, `:1081-1108` for the dots):

| what | line | ops/site |
|---|---|---|
| 1 nibble load (32B) | 1060 | 1 |
| `wlo = and(nv,15)` | 1061 | 1 |
| `whi = lshr(nv,4)` (vpsrlw+vpand on i8) | 1062 | 2 |
| 2 `vpbroadcastd` from memory | 1085-1086 / 844-853 | 2 |
| 2 `vpmaddubsw` + 2 `vpaddw` (i16 chain) | 1097-1101 / 872-878 | 4 |

10 x 32 sites = 320. Per sub-block: one i16 flush (`vpmaddwd`+`vpaddd`, `:1123-1128`) = 2 x 8 = 16.
Per sub-block fold (`:1154-1160` scale/min loads, `:1181-1189` the integer fold): sc load + `vpmovzxbd`
= 2, mn load + `vpmovzxbd` = 2, two scalar bsum loads + add + broadcast = 4, `vpmulld`+`vpaddd` for
`iacc` = 2, same for `bacc` = 2 -> 12 x 8 = 96. Superblock epilogue (`:1232-1250`) ~14.

Total **~446 per 2048 slots = 55.8 ops per 256 weights of one row**. Weight-plane loads issued: 32,
distinct: 32 (1.00x).

**Verdict: 55.8 vs 75 — we do 0.74x the work per weight.** Predicted 1.34x, measured 1.22x.

Where the 19 ops come from: our vector covers 8 rows, so every *per-sub-block* cost (the scale
multiply, the min multiply, the accumulate, the flush) is paid once for 8 rows where llama pays it
once per row. Our fold is 12+2 = 14 ops per sub-block per 8 rows = 1.75 per row; llama's is
2 shuffles + 2 `vpmaddwd` + 2 `vpaddd` per 64 weights = 3 per 32 weights. That is the entire margin.
The *unpack* is a dead heat: 1 load + 3 ALU for two vectors, on both sides.

### Q5_K — we lose 9%

**llama.cpp** (`quants.c:2235-2308`). Scale unpack identical to q4_K (`:2252-2257`); min term folded
once per superblock via bsums (`:2261-2265`). **The whole 32-byte `qh` plane is loaded once, at
`:2270`, into one register, and a running `hmask` register walks it** (`:2271`, advanced by
`slli` at `:2287` and `:2292`).

Main loop `:2278-2305`, 4 iterations of 64 weights:

| what | line | ops |
|---|---|---|
| 2 scale shuffles | 2279-2280 | 2 |
| 1 nibble load | 2282 | 1 |
| `q5l_0 = and(bits, 0xF)` | 2284 | 1 |
| `q5h_0 = slli(srli(and(hbits,hmask),bit),4)` | 2285 | 3 |
| `q5_0 = q5l_0 + q5h_0` (`vpaddb`) | 2286 | 1 |
| `hmask <<= 1` | 2287 | 1 |
| `q5l_1 = and(srli(bits,4),0xF)` | 2289 | 2 |
| `q5h_1` | 2290 | 3 |
| `q5_1` + `hmask <<= 1` | 2291-2292 | 2 |
| 2 activation loads | 2294-2295 | 2 |
| 2 `vpmaddubsw` | 2297-2298 | 2 |
| 2 `vpmaddwd` (widen + scale) | 2300-2301 | 2 |
| 2 `vpaddd` | 2303 | 2 |

22 x 4 = 88, plus ~16 prologue/epilogue -> **~104 ops per 256 weights of one row**. Weight-plane
loads issued: 4 nibble + 1 qh = 5, distinct 5 (1.00x). **Zero memory touches for the high bit inside
the loop.**

**Ours** (`dasllama_gemm_gen.das:1064-1068`). k4's 4 ops for the nibbles, then per site:

| what | line | ops/site |
|---|---|---|
| load 8 qh bytes (one per row) + broadcast to 4 lanes each | 1066 / `load_row_bytes_x4` 889-896 | 1 load + 2 (vpbroadcastq + vpshufb) |
| `or_bit_x10(wlo, hb, maskLo)` = and, cmpeqb, andn/and, or | 1067 / `or_bit_x10` 926-932 | 4 |
| `or_bit_x10(whi, hb, maskHi)` | 1068 | 4 |

Site total: 2 loads + 13 ALU + 2 broadcasts + 4 madd/add = 21 -> 672. Flush at `j==1,3`
(`:1112`), one chain only since k5 is not per-16, 2 x 2 x 8 = 32. Fold 96. Epilogue ~14.

Total **~814 per 2048 slots = 101.8 ops per 256 weights of one row**. Weight-plane loads issued:
32 nibble + 32 qh = 64, distinct 64 (1.00x).

**Verdict: 101.8 vs 104 — op parity.** Predicted 1.02x, measured 0.91x. The 11% residual is not in
the op count; see section 3.

Note the shape difference precisely: llama's high-bit deposit is **5 ops and 0 loads per 32
weights**; ours is **4 ops per 32 slots plus a 3-op memory touch per 64 slots = 5.5 ops and 0.5
loads per 32 slots**. Almost identical arithmetic, but ours re-enters memory 32 times per superblock
where llama enters it once. The reason is structural: llama's 32 lanes all want *the same* bit index,
so one scalar `bit` serves the whole vector; our 4 lanes of a row want bits t=0,1,2,3, so no uniform
shift exists and the bit must be selected with a per-lane constant mask (`maskLo`/`maskHi`,
`dasllama_gemm_gen.das:953-956`) that has to be re-applied to freshly loaded bytes each site.

### Q6_K — we lose 10%

**llama.cpp** (`quants.c:2439-2505`). Scales are native `int8[16]`, loaded raw at `:2455`. The `-32`
offset is removed **once per superblock** with a bsums trick: `q8sclsub = slli(madd(q8sums,
scales_16), 5)` at `:2457`, subtracted from `sumi` at `:2504`. So the dot itself is pure unsigned
`maddubs` on 0..63 values with no per-block correction at all.

Main loop `:2463-2502`, 2 iterations of 128 weights. **One `qh` load feeds four sub-fields**:

| what | line | ops |
|---|---|---|
| 2 `ql` loads + 1 `qh` load | 2464-2466 | 3 |
| `q4h_0 = slli(and(H,3),4)` | 2468 | 2 |
| `q4h_1 = slli(and(H,12),2)` | 2469 | 2 |
| `q4h_2 = and(H,48)` | 2470 | 1 |
| `q4h_3 = srli(and(H,0xC0),2)` | 2471 | 2 |
| `q4_0..q4_3 = or(and(ql,15) or and(srli(ql,4),15), q4h_*)` | 2473-2476 | 2+2+3+3 |
| 4 activation loads | 2478-2481 | 4 |
| 4 `vpmaddubsw` | 2483-2486 | 4 |
| 4 xmm scale shuffles | 2488-2491 | 4 |
| 4 `vpmovsxbw` + 4 `vpmaddwd` | 2494-2497 | 8 |
| 4 `vpaddd` | 2499-2500 | 4 |

44 x 2 = 88, plus ~9 prologue/epilogue -> **~97 ops per 256 weights of one row**. Weight-plane loads
issued: 4 ql + 2 qh = 6, distinct 6 (1.00x). The mask constants are chosen so each 2-bit field is
isolated *and* positioned in one shift — no cleanup mask, because the mask already zeroed the bits
that would bleed across the 16-bit shift lane.

**Ours** (`dasllama_gemm_gen.das:1069-1079`):

| what | line | ops/site |
|---|---|---|
| 1 `ql` load + `and 15` + `lshr 4` | 1060-1062 | 1 load + 3 |
| **2 `qh` loads** (columns j and j+4) | 1074-1075 | 2 loads |
| `dlo = and(lshr(qhlo,g2),3)` | 1076 | 2 |
| `dhi = and(lshr(qhhi,g2),3)` | 1077 | 2 |
| `wlo |= dlo<<4`, `whi |= dhi<<4` | 1078-1079 | 4 |
| 2 broadcasts + 2 `vpmaddubsw` + 2 `vpaddw` | 1085-1101 | 6 |

Site total 3 loads + 15 -> 18 x 32 = 576. Flush at `j==1,3` with **both** chains (k6 is per-16,
`:945`, `:1112-1120`) = 8 x 8 = 64. Fold per sub-block for the k63 arm (`:1139-1143` scale loads,
`:1188-1196` the two-scale integer fold): 2 int8 scale loads + 2 `vpmovsxbd` = 4, 2 scalar bsums + 2
broadcasts = 4, `ia = sc*a0 + mn*a1` = 3, `ba = sc*bs0 + mn*bs1` = 3, 2 accumulate adds = 2 -> 16 x 8
= 128. Epilogue ~9.

Total **~777 per 2048 slots = 97.1 ops per 256 weights of one row.**

**Verdict: 97.1 vs 97 — exact op parity.** Predicted 1.00x, measured 0.90x.

But: **weight-plane loads issued 96, distinct 48 — a 2.00x redundancy.** The offset at `:1071-1073`
is `hbase = 128*mr + (blk/4)*8*4*mr`, `hloOff = hbase + j*4*mr`, `hhiOff = hbase + (j+4)*4*mr` — it
depends on `blk/4` and `j` only, so **each of the 16 qh columns is re-loaded by all four `blk` values
in its group of four.** llama reads its qh plane exactly once.

### Q3_K — we lose 20%

**llama.cpp** (`quants.c:1784-1880`). The 12-byte 6-bit scale packing is decoded with four scalar
32-bit expressions into one xmm at `:1800-1805`, the `-32` bias applied with one `psubb` at `:1806`,
widened at `:1807`, split into two broadcast halves at `:1808-1810`. **The whole 32-byte `hmask` is
loaded once at `:1813`** and never re-read.

The `-4` offset and the high bit are folded into a single subtraction: `q3h_j` is built with
`andnot(hbits, 1<<bit)` (`:1827`, `:1831`, `:1835`, `:1839`), so it is **4 when the high bit is
absent and 0 when present**; the true value is `q3l - q3h`. That costs a **second `maddubs` per 32
weights** (`:1851-1854` alongside `:1856-1859`) plus a `vpsubw` (`:1861-1864`).

Main loop `:1821-1877`, 2 iterations of 128 weights: 1 load `:1823`; four `q3l` = 1+2+2+2 = 7;
four `q3h` = 3 each = 12 (the `slli_epi16(mone,bit)` is a compile-time constant); 4 activation loads
`:1843-1846`; **8 `vpmaddubsw`**; 4 `vpsubw`; 4 shuffles + 4 `vpmaddwd` = 8; 4 `vpaddd`. 48 x 2 = 96,
plus ~12 prologue/epilogue -> **~108 ops per 256 weights of one row**. Weight-plane loads issued:
2 qs + 1 hmask = 3, distinct 3 (1.00x).

**Ours** (`dasllama_gemm_gen.das:1033-1046`). We do **not** pay llama's double-maddubs: we compose an
unsigned 0..7 byte (`wlo = lo2 | (hbit<<2)`, `:1045-1046`) and remove the `-4` once per superblock
through the bsum fold (`koff = 4`, `:946`; applied at `:1238-1240`). One `maddubs` per vector.

| what | line | ops/site |
|---|---|---|
| **4 loads**: `q3l` col `(blk/4)*8+j`, `q3h` col +4, `h3l` col `j`, `h3h` col `j+4` | 1037-1040 | 4 loads |
| `lo2 = and(lshr(q3l,sh),3)`, `hi2` | 1041-1042 | 4 |
| `lob = ((h3l>>blk)&1)<<2`, `hib` | 1043-1044 | 4 |
| 2 `vpor` | 1045-1046 | 2 |
| 2 broadcasts + 2 `vpmaddubsw` + 2 `vpaddw` | 1085-1101 | 6 |

Site total 4 loads + 16 -> 20 x 32 = 640. Flush (per-16, two chains, at `j==1,3`) 64. Fold (k63 arm)
128. Epilogue ~9. Total **~841 per 2048 slots = 105 ops per 256 weights of one row.**

**Verdict: 105 vs 108 — we are marginally *ahead* on op count.** Predicted 1.03x, measured 0.80x.

But: **weight-plane loads issued 128, distinct 24 — a 5.33x redundancy**, the worst of the four.
Two independent causes, both visible in the offsets:
- `c0 = (blk/4)*8 + j` (`:1035`) -> each of the 16 qs columns is loaded by the 4 `blk` values in its
  group (64 issued / 16 distinct).
- `h3l` offset `(64 + j*4)*mr` and `h3h` offset `(64 + (j+4)*4)*mr` (`:1039-1040`) **do not depend on
  `blk` at all** -> each of the 8 hmask columns is loaded by all 8 `blk` values (64 issued / 8
  distinct).

### The two tables together

Ops per 256 weights of one row, and weight-plane load redundancy per superblock:

| fmt | llama ops | our ops | our ops ratio | our loads issued/distinct | measured |
|---|---|---|---|---|---|
| q4_K | 75 | 55.8 | 0.74x (better) | 32 / 32 = **1.00x** | **1.22x** |
| q5_K | 104 | 101.8 | 0.98x | 64 / 64 = **1.00x** | 0.91x |
| q6_K | 97 | 97.1 | 1.00x | 96 / 48 = **2.00x** | 0.90x |
| q3_K | 108 | 105 | 0.97x | 128 / 24 = **5.33x** | 0.80x |

Op count alone predicts +34% / +2% / 0% / +3%. The measured results are +22% / -9% / -10% / -20%.
**The op count explains q4_K and nothing else. The load-redundancy column is what tracks the losses.**

---

## 2. The deltas, ranked by decode payoff

Each entry says what it is: emitter change (IR only, no plane bytes move), repack-layout change
(plane bytes move -> `IMAGE_VERSION` bump, `dasllama_image.das:41`), or seat/knob.

### D1 — q3_K: hoist the shared column loads out of the `blk` walk. **Emitter change.**
Payoff: largest single item on the board. 128 issued loads become 32 (the distinct count is 24), and
the CSE'd live set drops from 24 ymm values (16 qs columns + 8 hmask columns = 768 bytes, against 16
architectural ymm registers) to about 4.

Shape: keep the existing `(blk, j)` nest but split `blk` into a group of four
(`blk = 4*g + i`, g in 0..1, i in 0..3) and emit the loads at the `(g, j)` level.
For fixed `(g, j)` the four `blk` values share `q3l` (col `8g+j`), `q3h` (col `8g+j+4`), `h3l`
(col `j`) and `h3h` (col `j+4`) — see the offsets at `dasllama_gemm_gen.das:1037-1040` — and differ
only in the constant shifts `sh = 2*i` and `blk`. Live set inside the group: 4 loaded column vectors
+ 4 sub-blocks x 2 halves = 8 i16 chains, which fits.
No plane byte moves. No `IMAGE_VERSION` bump. No Vulkan/Metal impact (see D6 note).
Estimated: -96 loads/group (~-11% static ops) plus whatever spill traffic disappears; on the measured
0.80x this is the difference between "worst format" and "roughly at parity".

### D2 — q6_K: the same hoist for the `qh` columns. **Emitter change.**
`hloOff`/`hhiOff` (`dasllama_gemm_gen.das:1072-1073`) depend on `blk/4` and `j`, not `blk`. Emitting
the two `qh` loads once per `(blk/4, j)` and looping the four `blk` values inside turns 64 qh loads
into 16. `ql` loads stay per-site (they are already 1.00x). Live set: 2 qh + 4 ql + 8 chains.
Estimated -48 loads/group (~-6% static ops) plus spill relief.

### D3 — q3_K and q2_K: the i16 chain flushes twice as often as the bounds require. **Emitter change.**
`dasllama_gemm_gen.das:1112` flushes at `j==1` and `j==3` for every non-k4 format. The stated bound
(`madd16_acc`, `:869-871`) is one `vpmaddubsw` pair-sum <= `2*qmax*127`:
- k6 `2*63*127 = 16002` -> 2 madds = 32004, at the i16 limit. Correct as written.
- k5 `2*31*127 = 7874`, one chain carrying both halves -> 4 madds = 31496, at the limit. Correct.
- **k3 `2*7*127 = 1778`** -> 4 madds = 7112. A single flush per sub-block is safe (18 would be).
- **k2 `2*3*127 = 762`** -> 4 madds = 3048. Same.
The code comment at `:1110-1111` already says "k3, conservatively". Fix: make the flush predicate
`(j == 3)` for k3/k2. -4 ops per sub-block = -32/group, about -4% on k3 and -5% on k2.

### D4 — q5_K: no clean lever exists; say so and stop looking.
Our k5 site is 2 loads + 13 ALU (`:1064-1068` plus the k4 nibble ops), llama's equivalent is
1 load-free deposit of 5 ops per 32 weights. Per weight the two are within 5% of each other and the
total op counts are 101.8 vs 104. The three candidate rewrites all fail:
- Rewriting `or_bit_x10` (`:926-932`) from `icmp-ne + select` to `vpand(vpcmpeqb(and,mask),16)` is the
  same 4 ops. No win.
- **Repack-layout change**: pre-expanding the qh plane from 1 byte per (row, site) to 4 (so the bit is
  already lane-local and the broadcast+shuffle at `:1066` disappears) saves about 4 ALU/site =
  -128 ops/group (-16%), but grows the k5 weight plane from 160 to 256 bytes per superblock (+60%).
  Our k5 currently moves 4096 x 56 x 180 B = 41.3 MB per call in 3628 us = 11.4 GB/s, against the ~16.3 GB/s our k4 demonstrates
  is reachable on this core; the widened plane would be 58.7 MB, and even at 16.3 GB/s that is 3600 us -- no better than today. **Reject.**
- Splitting the dot into a nibble part plus a `16 *` high-bit part (llama's q3 trick applied to q5)
  needs the same 4-op predicate to build the 0/1 vector, and adds a `maddubs`. **Reject.**
The honest read: q5_K's 9% is scheduling, not arithmetic — the per-site chain
`load(8B) -> vpbroadcastq -> vpshufb -> vpand -> vpcmpeqb -> vpandn -> vpor -> vpmaddubsw` is 8 deep
and repeats 32 times in one straight-line block. D5 is the lever that addresses it.

### D5 — a sub-block unroll knob for the kq GEMV. **Emitter change + schema knob.**
`emit_block_kqv2` emits **all 32 sites of a superblock as straight-line IR** — `dasllama_gemm_gen.das:1002`
(`for blk in range(8)`) and `:1027` (`for j in range(4)`) are das-level compile-time loops, and the kq
GEMV driver hardcodes one superblock per emitted iteration (`SliceArgs(..., kstep = 1)`,
`dasllama_gemm_gen.das:2830`). That is 600-900 IR instructions in one basic block with 16 ymm
registers available. The generic `gkstep` knob documented at `:29-31` is not reachable from the kq
GEMV path. Adding an `sbstep` (sub-blocks per emitted loop iteration; 8 = today's behaviour, 2 and 4
available) would give the tuner a handle on the live-set for every kq format at once, and is the only
proposal here that also helps q5_K. Costs a schema/perm-vector field; no plane bytes move.

### D6 — q4_K scale-plane padding: identified, and not worth taking.
Our k4/k5 scale plane is 20 bytes per superblock (`K4_SSB`/`K5_SSB`, `dasllama_kqformat.das:94-96`):
2 d + 2 dmin + 8 decoded `sc` + 8 decoded `mn`, written by
`repack_k4_grp` (`dasllama_repack.das:218-224`) and `repack_k5_grp` (`:270-276`) — the 6-bit packing
is decoded once at load via `k4_sc_mn` (`dasllama_math_default.das:381-391`). llama keeps the packed
12 bytes and decodes in-register (`quants.c:2069-2074`), so its row is 16 bytes. That is +4 bytes per
superblock = **+2.8% weight-plane bytes**, and q4_K is the one format where we are bandwidth-bound
(4096 x 56 x 148 B = 34.0 MB per call in 2089 us = 16.3 GB/s, the highest rate any of the five reaches on this core). Reverting to
a packed plane would cost about 8 ALU ops per superblock-group against 32 bytes saved — roughly
break-even at our measured 2.65 bytes per op — for a repack-layout change and an `IMAGE_VERSION`
bump. Not worth it. The same reasoning says **keep** the k3 pre-decode: `K3_SSB = 18`
(`dasllama_kqformat.das:106`, "6-bit scale - 32, decoded at transcode") costs 4 bytes over llama's 14
but buys us zero runtime scale decode, and k3 is compute-bound, not bandwidth-bound.

### D7 — the seat/knob lane is empty for these formats.
`mr` must be a multiple of `rv = width/32 = 8` (`setup_tile_emit`, `dasllama_gemm_gen.das:1883-1884`),
so on AVX2 the only legal values are 8 and 16; 16 doubles the register pressure without changing any
redundancy factor (a wider column is still re-read once per `blk`). `kstep` is pinned to 1 for kq
(`:2830`). `nrsplit` is irrelevant at tokCount = 1. Zen2 has no VNNI, so `dot` is fixed at `maddubs`.
**No retune can move these numbers; every item above is code.**

### Repack-layout / GPU-reader note
None of D1, D2, D3, D5 moves a plane byte, so none needs an `IMAGE_VERSION` bump. If a future item
does move the grp bytes, the blast radius is CPU-only: the grp repack runs at load and only when the
active backend asks for it (`dasllama_math.das:1182` `g_active_needs_repack`, recorded per model as
`kq_repacked`, `dasllama_load.das:2610`, `:2635`), while the GPU-resident tier reads the plane at the
**disk** stride (`t.k5q[sb0 * K5_QSB]`, `dasllama_gpu_resident.das:267-289`), not the grp layout. A
baked image does carry the repacked planes, so the `IMAGE_VERSION` bump is still required — but the
Vulkan and Metal readers are not on the hook. (Worth a one-line confirmation before anyone relies on
it.)

---

## 3. Why q4_K wins and q5_K/q6_K lose in the same emitter — the hmask/qh suspect is CONFIRMED

The suspect holds, and the correlation is perfect across all five formats:

| fmt | high-bit plane? | our loss/win |
|---|---|---|
| q4_K | none | **+22%** |
| q2_K | none | -1% |
| q5_K | `qh`, 1 bit/weight | -9% |
| q6_K | `qh`, 2 bits/weight | -10% |
| q3_K | `hmask`, 1 bit/weight | -20% |

**Every format with a separate high-bit plane loses; neither format without one does.** The mechanism
is not the arithmetic — section 1 shows we are at op parity or better on all three losers. It is
plane residency:

- In llama.cpp's row-major layout, a 32-byte register holds the high bits for **32 consecutive
  weights of one row**. Q3_K's entire 32-byte `hmask` covers the whole 256-weight superblock and is
  loaded once (`quants.c:1813`). Q5_K's entire 32-byte `qh` likewise (`:2270`). Q6_K's `qh` is 64
  bytes, so it is loaded twice, one register per 128-weight iteration (`:2466`). **The high-bit plane
  is register-resident for the whole loop, and extraction is pure ALU with a uniform shift, because
  all 32 lanes want the same bit index.**
- In our `grp<mr>` layout a 32-byte register holds 4 bytes from each of 8 rows, so the high-bit plane
  for the same amount of work is **mr = 8 times larger**: q5 256 bytes, q3 256 bytes (32 hmask x 8
  rows), q6 512 bytes — 8 to 16 ymm registers' worth, against 16 architectural registers on AVX2.
  **It cannot be register-resident.** Every extraction therefore re-enters memory
  (`dasllama_gemm_gen.das:1039-1040` for k3, `:1066` for k5, `:1074-1075` for k6), and because the
  four lanes of a row want four different bit positions, the extraction needs a per-lane constant mask
  (`maskLo`/`maskHi`, `:953-956`) rather than a uniform shift.
- Q4_K and Q2_K have no high-bit plane at all: their entire weight is inside the nibble/crumb byte
  (`dasllama_gemm_gen.das:1060-1062` for k4, `:1051-1054` for k2). There is nothing to re-touch, our
  loads are 1.00x, and the 8-row amortization of the fold shows up undiluted — hence +22% on k4.

Q2_K's near-parity rather than a q4-sized win is the secondary effect and is also visible in the
code: k2 is a per-16-scale format (`per16 = k63 || k2`, `:945`), so it runs split lo/hi accumulators,
two i16 chains, two flushes per sub-block, and four `vpmulld` per sub-block in the fold
(`:1200-1207`) where k4 uses two — plus its plane is only 84 bytes per superblock, giving it about
1.08 bytes per vector op against k4's 2.65, i.e. it is compute-bound where k4 is bandwidth-bound.

**Ranking of the three losers is set by the load-redundancy factor, not by the plane's bit width.**
q6_K carries twice as many high bits as q3_K but loses less, because its qh columns are re-read 4x
while q3_K's are re-read a weighted 5.33x overall (qs 4x *and* hmask 8x). That is why D1 (q3_K) is
the top item and D2 (q6_K) the second, and why both are pure loop-restructuring in the emitter with
no layout consequence.

---

## 4. Two things to keep in view

1. **The q4_K win is measured against llama.cpp's fallback.** `ggml_gemv_q4_K_8x8_q8_K`
   (`arch/x86/repack.cpp:1464`, selected on AVX2 at `repack.cpp:4599-4604`) is what a real llama.cpp
   model run uses on zen2, and it is an 8-column row-interleaved kernel structurally similar to ours.
   Any claim of "1.22x ahead on q4_K" should be re-measured against that kernel before it leaves the
   building. q3/q5/q6 have no such caveat — llama.cpp has no x86 repack for them.
2. **The prefill tiles are 1.6-2.6x ahead on the same formats**, which is consistent with everything
   above: at tokCount = 4 the per-site unpack (including the redundant high-plane loads) is amortized
   over four activation dwords instead of one, so the fraction of the loop spent on plane touching
   drops by roughly 4x and the losers stop being losers. The decode kernel is the only place where
   the unpack cost is paid at full weight.
