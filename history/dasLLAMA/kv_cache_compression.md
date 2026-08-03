# KV Cache Compression — Design Notes

Status: **ACTIVE design, not started.** Boris framed this as a *redesign on multiple axes*, not a
single FP32→F16 swap. This doc captures the full investigation (2026-07-07) so the redesign can be
scoped against real facts, not memory. **Fact-checked 2026-07-09** against current sources
(origin/master 773ca80dc), the local ggml checkout, and the primary papers; corrections applied in place.

dasLLAMA today ships a **fully uncompressed FP32 KV cache** — the baseline of baselines. That's the
starting point and the opportunity.

---

## 0. Why this matters (the physics)

- **Decode is memory-bandwidth-bound, not compute-bound.** Generating one token streams the entire
  model once; the ALUs sit idle waiting on weights. `tokens/sec(batch=1) ≈ mem_bandwidth / model_bytes`.
- **Batching amortizes weight loads across sequences** (GEMV→GEMM, weights read once for B sequences).
  **dasLLAMA does not batch yet.** Even with batching, *attention does not batch* — each sequence
  attends to its **own** KV cache, so the attention read stays per-sequence and memory-bound.
- **The KV cache is the part that (a) doesn't batch, (b) grows with context, (c) dominates the
  memory-bound attention read at long context.** Shrinking it buys **both** RAM capacity **and**
  decode bandwidth (fewer bytes streamed each step on the step that hurts most).

So KV compression is the highest-leverage single change for long-context decode on CPU.

---

## 1. Current state — FP32, hardcoded

- `key_cache : array<float>` / `value_cache : array<float>` — `dasllama_common.das:1429-1430`,
  sized at `:1558-1559` via `resize(c.seq_len * kv_row_total(c))`.
- **The layout is ragged per-layer, NOT flat `n_layers * seq_len * kv_dim`** (the `:1429` source
  comment claims that; only true for uniform models). `kv_row_total` (`:2154-2159`) *sums* per-layer
  `layer_kv_dim(c, l) = layer_n_kv_heads(c, l) * layer_head_size(c, l)` (`:2135`) — layers differ
  under Gemma's SWA/global split (`head_size_swa`, `has_dual_rope`). Layer base offset is a **prefix
  sum**: `loff = kv_cache_off(t, l) = t.kv_row_prefix[l] * seq_len` (`:2175`). A codec must key
  strides/offsets off the per-layer prefix array, not one flat `kv_dim`.
- **Cross-layer KV sharing stores nothing:** `kv_shared = t.kv_src[l] != l` (decode `:2737`, prefill
  `:4009`); both store sites are guarded `if (!kv_shared)` (`:2802`, `:4080`) — shared layers alias
  another layer's cache slice. A codec must preserve that aliasing (a shared layer's `loff` points
  into the *source* layer's compressed region).
- **FP32 end to end** — no hidden narrowing, no `cache_type` knob (verified: every `f16`/`half` hit
  in the module is GGUF weight loading, the gelu LUT, or RoPE "half"):
  - store: `copy_floats(...)` decode (`:2805-2806`), `kv_store_batch(...)` prefill (def `:1844`,
    single call site `:4082`)
  - read: attention reinterprets to `float const?` (decode `:2825-2826`, prefill `:3844-3845`)

### Size ladder (bytes per KV value)

| | bytes/val | vs dasLLAMA |
|---|---|---|
| **dasLLAMA today (FP32)** | 4.0 | 1× |
| llama.cpp default (F16) | 2.0 | ½ |
| llama.cpp `q8_0` | ~1.06 | ~¼ |
| llama.cpp `q4_0` / `IQ4_NL` | ~0.56 | ~⅐ |
| TurboQuant (3.5-bit) | ~0.44 | ~1/9 |

dasLLAMA's KV is **2× larger than llama.cpp's uncompressed default** and **~9× larger than TurboQuant**.

---

## 2. The compression landscape (four orthogonal families + compressive memory)

They compose. Ordered by fit for dasLLAMA (CPU, retrofit onto existing GGUFs):

1. **Quantize** (lossy, per-value) — F16 / int8 / int4. KIVI = per-channel K, per-token V. **The rung
   dasLLAMA should climb.** F16 is the safe first step; **TurboQuant** is the SOTA of this family.
2. **Share across heads** (architectural, training-time) — MQA/GQA (Google). Llama-2/3, Mistral, Qwen
   already use GQA → dasLLAMA **inherits** a 4–8× KV shrink for free from the model.
3. **Low-rank / latent** (architectural) — **MLA** (DeepSeek, ~10-15× shrink). Needs training: no
   zero-shot inference-time retrofit, though post-training conversion exists (TransMLA arXiv
   2502.07864, MHA2MLA 2502.14837 — both need a fine-tuning pass). Model-choice decision, not a
   dasLLAMA kernel change.
4. **Evict / sparsify** — H2O, StreamingLLM (attention sinks + window), SnapKV. Retrofittable at
   runtime, lossy in recall. Sliding-window attention is the architectural version.
5. **Compressive memory** — Infini-attention (Google), Compressive Transformers (DeepMind). Old
   context folded into bounded linear-attention state → cache stops growing. Needs training-in.

**Key distinction:** the outlier-channel problem that wrecks *fixed-point* quant (int8/int4) does
**not** hurt *floating-point* (F16) — float keeps relative precision, so outliers cost F16 nothing.
That's why F16 is near-lossless and why TurboQuant's rotation is only needed once you go sub-byte.

---

## 3. FP32 → F16 safety analysis

- **What's stored:** K = RoPE-rotated keys (`RoPE(W_k·x)`, norm-preserving), V = raw values (`W_v·x`).
  Both are projections of a **normalized** hidden state → activation-scale, not residual-scale.
- **Range:** bulk ~±[0.001 .. ~10]; outlier channels ~100s, occasionally low thousands. Attention-sink
  (BOS) K/V runs hot. **Arch-dependent** — Gemma-without-QK-norm is the worst case, Qwen3/Gemma3
  (QK-norm bounds K) the best-behaved.
- **F16 covers it decisively:** max normal ≈ **65504** (a 1000-magnitude outlier is ~1.5% of ceiling →
  overflow essentially impossible for post-norm KV). Relative precision ≈ **2⁻¹¹ ≈ 0.049%** (10
  mantissa bits), held across the whole exponent range → outliers stored as accurately (relatively) as
  small values.
- **Net loss:** ~0.05%/element (drops 13 of 23 mantissa bits), **no range clipping**. Dot products
  over head_dim partially cancel; softmax smooths. Empirically indistinguishable from FP32 (settled in
  llama.cpp). Only failure mode is a value >65504 — a one-time max-abs assert catches it.
- **F16 > BF16 for KV:** BF16 buys range you don't need at the cost of 3 mantissa bits (~0.4%).
  Precision is the binding constraint here, not range.

**Proof step still owed:** instrument a real forward pass, dump per-layer `min/max/abs-max/histogram`
of `key_cache`/`value_cache`, run on a Gemma model (worst) and a Qwen3 model (best), confirm abs-max ≪
65504 before committing. Measure, don't assume.

---

## 4. Reference — ggml F32↔F16 conversion (port targets)

Three tiers (`D:\Work\llama.cpp`):

1. **Hardware F16C** — `ggml/src/ggml-cpu/simd-mappings.h:56-63`. Scalar `_cvtsh_ss`/`_cvtss_sh`
   (MSVC: `_mm_cvtph_ps`/`_mm_cvtps_ph`). **Real kernel wants the packed forms** `_mm256_cvtph_ps`/
   `_mm512_*` (8/16 lanes).
2. **64K LUT (f16→f32 only)** — `simd-mappings.h:115-149`, `float ggml_table_f32_f16[65536]`. Only 65536
   possible halves. No reverse LUT (f32→f16 has 4B inputs → always computes).
3. **Portable scalar bit-twiddle** — `ggml/src/ggml-impl.h:384-431`, from Marat Dukhan's FP16
   library (credited at `:363-364`, `github.com/Maratyszcza/FP16`). Correctly rounded
   (round-to-nearest-even via an FPU-rounding trick: `(|f|·2¹¹²)·2⁻¹¹⁰`), handles subnormals (magic
   `0.5` bias), overflow→inf, NaN→qNaN. **This is the one to port** for interp/fallback.

**daslang port (x86):** store cache as `array<uint16>` (= ggml's `ggml_fp16_t`); `reinterpret<uint>`/
`reinterpret<float>` for `fp32_to_bits`/`fp32_from_bits`; rest is `uint` shifts/masks. Emit packed
`vcvtph2ps`/`vcvtps2ph` in the AOT/JIT KV kernel; bit-twiddle or LUT in interp. **The bit-twiddle + LUT
tiers are x86-interp-only** — see below, they're never needed on ARM.

### ARM / Apple Silicon — native, no emulation
fp16 is a **hardware type** (`__fp16`); conversion is one instruction, mandatory since ARMv8.0 (no CPUID
gate, no software fallback):
- scalar: `(float)h` / `(__fp16)f` → `fcvt` (ggml `simd-mappings.h:44-54`)
- NEON widen/narrow: `vcvt_f32_f16` (`FCVTL`) / `vcvt_f16_f32` (`FCVTN`); 8-wide `vld1q_f16`/`vst1q_f16`
- M1+ has native f16 **arithmetic** (`__ARM_FEATURE_FP16_VECTOR_ARITHMETIC`, `ggml-cpu.c:3768`): ggml
  does 8-lane `vfmaq_f16` (`float16x8_t`, unrolled ×4 → step 32) accumulating **in f16**
  (`simd-mappings.h:261-270`) — fast, lower precision.
- **Best KV-dot primitive: `FMLAL`** (`vfmlalq_low/high_f16`, FEAT_FHM, on Apple Silicon): multiplies an
  f16 pair, **widens products to f32, accumulates in f32**, one instruction → K stays f16 in memory
  (bandwidth) *and* the dot accumulates in f32 (precision), no separate convert pass. ggml does NOT use
  it (no `vfmlal` in the CPU path) — available headroom. **Prefer FMLAL over f16-accumulate** so the
  ~0.05% storage error doesn't compound in the reduction.
- Lands in `dasllama_math_aarch64_neon.das`. F16 KV is **cheaper on Mac than x86** — the AOT C++ just
  uses `__fp16`/NEON; no bit-twiddle/LUT needed.

---

## 5. Affected kernels (the read/write surface)

Small and specific. **All are `float?`/`float const?` hardcoded.**

**Reads (consume the cache):**
- `dot(qp+qoff, kcp+koff, head_size)` — Q·Kᵀ scores — decode `dasllama_common.das:2838`
- `axpy(xbp+qoff, vcp+voff, arow[j], head_size)` — att·V weighted sum — decode `:2854`
- register-blocked attention kernel — `gemm_f32_uk_4x16_template` (`dasllama_math.das:453-489`,
  itself a `[tuned]` template via `[dasllama_grid]`) behind the `gemm_f32` driver (`:500-518`),
  called from `attn_head_flash` (`common.das:3774` Q·Kᵀ, `:3820` att·V). **Flash-mode nuance:**
  `gemm_f32` reads packed f32 scratch (`fa_k`/`fa_v`) — the raw cache is read in the *pack step*,
  so the flash dequant seam lives in the packing, not the GEMM.
- `dot`/`axpy` are `[tuned]` primitives — `dasllama_math.das:32-33`, `:52-53` (templates at `:45`)

**Writes (produce the cache):**
- `copy_floats(...)` — decode kv_store — `:2805-2806`
- `kv_store_batch(...)` — prefill — def `:1844`, called at `:4082`

**NOT affected:** the weight-matmul generator zoo (`q8q8_gemv_gen`, `mx4q8_gemv_gen`, `q8q8_tile_gen`,
batch cells, neon laneq…) in `dasllama_math_gen.das`. Those feed `mm()` for the Q/K/V/O + FFN *weight*
projections — they never touch the KV cache. **KV dtype is orthogonal to weight quant.**

---

## 6. Dispatch / permutation — dasLLAMA vs ggml (the core architectural axis)

### dasLLAMA `[tune]` is tuning-stage-only

- The `<name>__<suffix>` grid clones are emitted by the `[dasllama_grid]`/`[dasllama_tune]`
  structure macros (`dasllama_tune.das:153`, `:210`) and live **only in the offline tuner harness**
  (`harness/tune_kernels.das`, run via `daslang -jit`), which benchmarks the grid and records the
  winner in `box_profile.json`. (dasLLAMA has its own rig here — the generic llvm_tune `--tune`
  flag is NOT what triggers these kernels' grids.)
- In a **normal build**, `[tuned]` stamps the **single** recorded winner's hints onto the template →
  emits **exactly one** function (default `vec8_u2`). No shipped grid, no runtime dispatch, no bloat.
- **Therefore the KV-dtype axis is ADDITIVE in the shipped binary**, not `dtype × tune`. Native f16
  support = `dot_f32` + `dot_f16` (+ axpy, + blocked) = a handful of extra branch-free functions, each
  with its own already-tuned single hint set. **Only the offline `--tune` run's work multiplies**
  (each new kernel sweeps its own grid) — a one-time cost, not shipped, not runtime.

### ggml resolves dtype at runtime via a vtable

- Each `ggml_type` has a `type_traits`/`type_traits_cpu` row: `{from_float, vec_dot, vec_dot_type,
  to_float, nrows}` — `ggml-cpu.c:215-268` (F16 row `:221-225`, Q8_0 `:265-268`).
- **One** generic `mul_mat` and **one** flash-attn kernel index the table by `tensor->type`. SIMD
  variants live *inside* each `vec_dot` as compile-time `#ifdef` (`__F16C__`/`__AVX512F__`/NEON),
  chosen by `-march`. Neither axis is generated code.

### The trick worth stealing regardless of dispatch style: **convert the query, not the cache**

- `mul_mat` (`ggml-cpu.c:1266-1346`): `vec_dot_type = traits[K->type].vec_dot_type`; convert **src1
  (the query, O(head_dim))** to that type into `wdata` via `from_float` (`:1316-1346`); `vec_dot` reads
  **K at its native stored width** (`:1237`). K is never widened to f32 in memory.
- flash-attn (`ops.cpp` decls `:8422-8424`; calls `:8465`, `:8480`, `:8527`): `q_to_vec_dot(pq, Q_q,
  DK)` converts the query once per row; `kq_vec_dot(DK, &s, 0, k_data, ...)` reads K native;
  `v_to_float(v_data, V32, DV)` dequants **one V row at a time** into a tiny f32 scratch for that
  row's accumulate.
- **The inversion:** convert the thing that's `O(head_dim)` per step (query row), not the thing that's
  `O(context × head_dim)` (the cache). Keeps the bandwidth win.

### Consequence

dasLLAMA's **native-per-dtype** route is *better on the hot path* than ggml's vtable — no indirect
call, each dtype a branch-free kernel tuned to *this box*. Price is offline tuning time + source
volume, **not** runtime. My earlier "dtype × tune blows up" concern was wrong (conflated tuning-stage
with shipped binary — Boris corrected this).

---

## 7. Phase asymmetry — prefill vs decode (the materialization question)

**Attention is consumed one layer at a time** → you **never** need the whole (all-layers) cache in f32
at once; at most **one layer's window**, reusable across layers.

### Prefill (compute-bound, tolerates external decompression)
- Single-shot over the whole prompt: `eval_` (`:1573-1576`) calls `forward_batch(t, s, tokens, npos,
  s.n_past)` with `npos = length(tokens)`; `forward_prefill_alloc` sizes `k_b`/`v_b` to
  `npos × max_kv_dim` (`:4966-4967`) → **the entire prompt's K/V is already in f32 scratch**.
  (The "chunks"/`chain_stages` split matmul *work* for threading, not the KV into passes.)
- Per layer: `kv_store_batch` writes k_b/v_b → cache (`:4082`), then `prefill_attention` (def
  `:3836`, called `:4092`) reads keys `kstart..ap` from that layer's cache slice (blocked path
  `:3887-3894`).
- **Three prefill modes** (`g_attn_prefill_mode`): flash / blocked / classic. Flash packs the cache
  slice into f32 scratch (`fa_k`/`fa_v`) per head, then GEMMs — the cache read (and thus the
  dequant seam) is in the pack step, not the GEMM (§5).
- **Fresh prompt (`start_pos==0`):** attended range = current block = `k_b`/`v_b`, already f32 →
  redirect `prefill_attention` at the scratch, **decompress nothing**.
- **Continuation (`start_pos>0`):** prior context lives only in the (compressed) cache → decompress the
  current layer's prior range, one layer, **amortized against the O(npos²) attention GEMM**. Fine.

### Decode (memory-bound, the binding constraint)
- Each step reads the **whole current-layer cache** (`0..pos`). Decompress it externally to f32 every
  step → traffic = `read compressed + write f32 + read f32` ≈ **2× the f32 baseline → strictly WORSE
  than uncompressed.** The inversion. **Decode's read must be fused (in-loop dequant / convert-query).**

### Answer to "do I need to materialize the entire KV cache during prefill?"
**No** — never all layers at once; for a fresh prompt, zero decompression (read `k_b`/`v_b`). The
boundary-compress design *does* keep the full prompt f32 during prefill (its current footprint, so no
capacity win *during* prefill, only decode bandwidth after). **Prefill can be ~fully external; the
decode read cannot.** Pragmatic split: **compress-on-store (external) + fused dequant on the decode
read (in-loop)**, prefill reads scratch for a fresh prompt.

---

## 8. The redesign axes (why this isn't a one-liner)

1. **Storage dtype:** FP32 → F16 → block-int8 (KIVI-style per-channel K / per-token V) → **TurboQuant
   3.5-bit** (rotated + QJL). Endgame is TurboQuant; F16 is the proving rung.
2. **Dispatch style:** external boundary-compress · native generated per-dtype kernels (dasLLAMA
   style, additive in shipped binary) · runtime vtable (ggml style). *Native generated is the
   dasLLAMA-idiomatic, best-hot-path choice.*
3. **Which operand converts:** convert-the-query (ggml, keeps K at native width) vs dequant-the-cache
   (defeats bandwidth). **Always convert the query.**
4. **Phase:** prefill (external decompress OK, per-layer, fresh-prompt reads scratch) vs decode (must
   fuse). Different code paths, different constraints.
5. **K vs V asymmetry:** K consumed via `dot` (Q·Kᵀ), V via `axpy` (weighted sum) / per-row dequant.
   K and V can even carry different dtypes (llama.cpp does: `-ctk` vs `-ctv`) — asymmetrically:
   llama.cpp hard-errors on a **quantized V without flash attention** (`llama-context.cpp:401-403`)
   while quantized K is allowed without FA. The V-side read pattern is the constraining one.

### TurboQuant endgame (Google Research — arXiv 2504.19874, Apr 2025 / ICLR 2026) — design for sub-byte now
- Two stages (Zandieh/Daliri/Hadian/Mirrokni; one DeepMind coauthor — NOT "a DeepMind paper"):
  **(1)** random rotation — **QR-of-Gaussian in the paper, not Hadamard** (community impls substitute
  a randomized Hadamard transform for O(d log d)) → coords ≈Beta-distributed (outliers smeared) →
  MSE-optimal per-coordinate codebooks via "continuous k-means" (Lloyd-Max-style, calibration-free).
  **(2) QJL** (the authors' own prior paper, arXiv 2406.03482): 1-bit sign of a JL projection of the
  residual → an *unbiased estimator of the attention inner product*. The composite is **near-optimal**
  (within ~2.7× of the rate-distortion bound). NOTE: "PolarQuant" is a *separate* paper by the same
  group (2502.02617, polar-coordinate angle quant) that appears as a **baseline** in TurboQuant's
  tables — the Google blog blurs it into "stage 1"; don't repeat that.
- 3.5-bit, training-free, data-oblivious (no calibration), quality-neutral on LongBench/NIAH
  (Llama-3.1-8B + Ministral-7B in the paper; the blog also names Gemma).
- **Hot ops of the community implementations** (llama.cpp discussion #20969: CPU/Metal/CUDA/Vulkan
  forks, nothing merged) = fast Walsh-Hadamard + LUT-based dequant; popcount is the natural QJL dot
  primitive → lands squarely in the AVX-VNNI/512 kernels already tuned for MXFP4. Green field upstream.
- Design the dtype axis (axis 1) and the store/read seams (axes 3,4) so TurboQuant slots in later
  without re-plumbing — i.e. don't hardwire "f16" assumptions; parameterize on a KV codec.

---

## 9. BLAS / Accelerate on macOS — RULED OUT (measured)

**Measured: dasLLAMA's 8-bit `sdot` kernel beats the BLAS (Accelerate) path. Not a factor.**

Why it's structural, not incidental:
- **BLAS is f32-only.** Standard GEMM precisions are S/D/C/Z (f32/f64/complex); there is **no `hgemm`
  (f16), no int8, no quantized** in Accelerate. ggml's whole BLAS backend calls exactly one function,
  `cblas_sgemm` (`ggml/src/ggml-blas/ggml-blas.cpp:141`), and **dequantizes any non-f32 operand to f32
  into scratch first** (`:67-104`); its op gate requires `src1` be f32 and limits to large matrices
  (`:418-422`).
- **The weight matmul is memory-bound on the weight stream**, so bytes read decide the winner, not
  FLOPs. `sdot` (SDOT/UDOT / VPDPBUSD-VNNI) streams ~1 byte/weight and dots in int8. BLAS must expand
  the weight to f32 (4×) + a materialization pass, to feed AMX/SME whose only edge is f32 throughput
  the bottleneck doesn't care about. Quantized-native wins every time.
- BLAS/AMX would only win a **compute-bound, already-f32** GEMM — which doesn't exist in the hot path.
  The lone f32 GEMM (prefill attention) is small per-head and going F16 anyway (§7), so that sliver
  closes too.

Integration mechanics (recorded in case it ever resurfaces for a genuinely f32 path): link Accelerate
(ships with OS), `find_package(BLAS)` + `BLA_VENDOR=Apple`, define `ACCELERATE_NEW_LAPACK` /
`ACCELERATE_LAPACK_ILP64` (`ggml-blas/CMakeLists.txt:18-21`); `cblas_sgemm(CblasRowMajor, NoTrans,
Trans, …)`; **do not** wrap in the jobque — Accelerate owns AMX/SME threading, wrapping oversubscribes.
For Apple lower-precision matmul the framework is **BNNS** (CPU f16/int8) or **MPS/MLX** (GPU), NOT BLAS
— separate, larger integration stories, not pursued.

**Decision: don't route dasLLAMA matmuls through BLAS. Confirms narrow-backends north-star.**

## References
- TurboQuant primary: [arXiv 2504.19874](https://arxiv.org/abs/2504.19874) ·
  [Google Research blog](https://research.google/blog/turboquant-redefining-ai-efficiency-with-extreme-compression/) ·
  [llama.cpp #20969](https://github.com/ggml-org/llama.cpp/discussions/20969) (community impls).
  Related: QJL [2406.03482](https://arxiv.org/abs/2406.03482) · PolarQuant [2502.02617](https://arxiv.org/abs/2502.02617)
  (separate paper, baseline) · TransMLA [2502.07864](https://arxiv.org/abs/2502.07864) · MHA2MLA [2502.14837](https://arxiv.org/abs/2502.14837).
  Secondary (imprecise on authorship/technique — superseded above): [Towards Data Science](https://towardsdatascience.com/kv-cache-is-eating-your-vram-heres-how-google-fixed-it-with-turboquant/) ·
  [InfoQ](https://www.infoq.com/news/2026/04/turboquant-compression-kv-cache/)
- llama.cpp KV defaults: `common/common.h:340-341` (F16 both), `common/arg.cpp:300-310` (types), `:2174-2198`
  (`-ctk`/`-ctv`), `common/common.h:495` (flash-attn AUTO), `src/llama-context.cpp:401-403` (quantized V
  requires FA).
- ggml conversion: `ggml/src/ggml-impl.h:384-431`, `ggml/src/ggml-cpu/simd-mappings.h:56-149`.
- ggml dispatch: `ggml/src/ggml-cpu/ggml-cpu.c:215-268`, `:1266-1346`; `ggml/src/ggml-cpu/ops.cpp:8422-8527`.
- dasLLAMA (line refs re-verified 2026-07-09 @ origin/master 773ca80dc): `dasllama_common.das`
  (cache `:1429`, ragged layout `:2135-2175`, decode attn `:2805-2860`, prefill `:3836-4092`,
  prefill alloc `:4966`), `dasllama_math.das:32-53` (`dot`/`axpy`), `:453-518` (`gemm_f32`),
  `dasllama_tune.das` (`[tuned]`), `dasllama_math_gen.das` (weight kernels, orthogonal).
