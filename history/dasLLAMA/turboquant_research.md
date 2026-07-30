# TurboQuant research notes (2026-07-12)

Two agent reports, verbatim: (A) implementation survey with file-level verification,
(B) implementation-grade paper digest of arXiv 2504.19874 (+ QJL 2406.03482).
Companion to `kv_cache_compression.md` §8. Corrections to that file discovered here:
the "popcount is the natural QJL dot primitive" claim is off-paper (estimator is
asymmetric — query stays fp, so QJL dot is sign-XOR on fp lanes; popcount only if you
also binarize the query), and "3.5-bit" comes from mixed per-channel bit allocation,
not fractional coding.

---

## A. Existing implementations — survey & verdicts

**Context**: Paper by Zandieh, Daliri, Hadian, Mirrokni (Google Research). Went viral ~March 25, 2026 after the Google Research blog post. The ecosystem is now large: 378 GitHub issues with "turboquant" in the title, ~911 repos matching the name. **Google released no official code.** The QJL authors' own repo (`amirzandieh/QJL`, Apache-2.0, CUDA+Python for Llama-2/3) was never extended to TurboQuant — it remains the 2024 QJL-only codebase, ~23 commits, dormant.

### The core verdict: what's actually in llama.cpp

**Upstream `ggml-org/llama.cpp` master: NOT TurboQuant.** What merged is PR **#21038** ("llama : rotate activations for better quantization", author **ggerganov**, merged 2026-04-01): Hadamard rotation of Q/K/V around attention (64×64 Hadamard, `LLAMA_ATTN_ROT_DISABLE` env, changes in `src/llama-graph.cpp` / `src/llama-kv-cache.cpp`), plus follow-up FWHT kernels (CPU #22631, CUDA #23615, Vulkan #23687). It cites TurboQuant but explicitly contains **no Lloyd-Max codebooks, no QJL, no new quant type** — it is exactly the "Hadamard + existing int4 cache" (QuaRot/KIVI-style) configuration. PolarQuant/QJL were explicitly deferred. The feature request (#20977, 108 comments) has an unmerged community PR #21089 (elusznik, CPU-only, 47% tg throughput loss). Discussion hub: `ggml-org/llama.cpp` discussion #20969.

**The real thing lives in forks.** Verified at file level:

#### 1. TheTom/llama-cpp-turboquant (branch `feature/turboquant-kv-cache`) — genuine Stage-1 TurboQuant, QJL present but disabled
- **File evidence**: `ggml/src/ggml-turbo-quant.c` (36 KB, dedicated file). Contains:
  - Lloyd-Max **Beta-distribution** centroids (the paper's continuous-k-means output), e.g. `CENTROIDS_3BIT[8] = {-0.190207, -0.118786, -0.066822, -0.021663, +mirror}`, `CENTROIDS_4BIT[16] = {-0.241529 ... +0.241529}`, `CENTROIDS_2BIT[4]`; separate N(0,1) Lloyd-Max tables for the TQ3_1S/TQ4_1S *weight* formats.
  - FWHT rotation: `turbo_cpu_fwht()` / `turbo_cpu_fwht_inverse()`, precomputed ±1 sign arrays `turbo_cpu_s1/s2[128]`, group `TURBO_D=128`, seeds `TURBO_SEED_ROTATION=42` / `TURBO_SEED_QJL=1042`, LCG + Box-Muller.
  - **QJL IS implemented**: legacy `TURBO4_0` = 3-bit PolarQuant + 1-bit QJL sign-of-projected-residual (`turbo_qjl_matrix`, `TURBO_QJL_CONST = 1.2533141` = sqrt(π/2), the QJL unbiasedness constant). But the modern default (`TURBO4_USE_4BIT`) **drops QJL** for pure 4-bit Lloyd-Max.
  - Block formats: `block_turbo3_0` = 2-bit qs + separate sign-bit plane + fp16 norm (per-vector L2 norm extraction — a real TurboQuant feature absent from plain int4 caches).
- **Why QJL is off**: `docs/papers/turbo4-resurrection.md` in TheTom/turboquant_plus — "QJL eliminates bias but explodes variance; softmax tolerates uniform bias, not variance." Removing QJL *improved* PPL 6.1894→6.1756; QJL degradation grows with context (−0.28% @2K → +3.69% @64K, buun's CUDA data). **Five independent groups confirmed** (scos-lab: MSE beats QJL by 7.6% on GPT-2; Arclabs001; Aaryan-Kapoor; quantumaikr; AmesianX — who found QJL works *if the QJL projection uses a seed independent of the rotation*, i.e. rotation/projection correlation was a real bug in early impls).
- Target: Metal (production quality), CUDA, HIP, CPU. Prebuilt binaries released. The quant codec file itself is **scalar C** — CPU speed comes from dequant into existing ggml dot paths, not turbo-specific SIMD.
- Companion research repo `TheTom/turboquant_plus` (6989 stars, Apache-2.0, 141+ tests, active through July 2026) holds the NumPy reference (`polar_quant.py`, `lloyd_max.py`, `qjl.py`, `rotation.py`) and the follow-on findings (V-compression is free; all damage comes from K; boundary-layer protection; block 32→128 gives 3.125 bpv at no quality cost). Fork license: MIT (llama.cpp).
- **Verdict: real TurboQuant Algorithm 1 (MSE quantizer) + norm extraction; Algorithm 2 (QJL) implemented but deliberately not used. The most complete and battle-tested engine integration anywhere.**

#### 2. Aaryan-Kapoor/llama.cpp, branch `turboquant-tq3_0` — cleanest minimal CPU reference
Single commit `1fb1fb3ab6b3` (2026-03-25), 12 files: `GGML_TYPE_TQ3_0`, WHT + deterministic sign flips from golden-ratio hash (0x9E3779B9) + 3-bit Lloyd-Max, block 32 / 14 bytes (3.5 bpw), K and V via vec_dot (dequant + dot vs Q8_0). No QJL (author independently converged on dropping it). CPU benchmarks on Qwen3.5-35B-A3B: pp 20.1 t/s vs f16's 19.3, tg 11.4 vs 10.6, 4.4x compression. MIT. **Verdict: Stage-1-only TurboQuant, correct constants, smallest readable diff for a ggml-style KV type.**

#### 3. Other llama.cpp-family forks (briefly)
- `spiritbuun/buun-llama-cpp` (CUDA, 677 stars, `TURBO_LAYER_ADAPTIVE`): tested by ikawrakow — turbo3 PPL 6.6112 vs f16 6.5791 (Qwen3.5-35B); author admits multi-GPU is broken.
- `Madreag/turbo3-cuda` (Metal→CUDA port, RTX 5090, 4.6x, 700K-ctx NIAH 6/6, dequant-then-cuBLAS).
- `unixsysdev/llama-turboquant` (Strix Halo), `animehacker/llama-turboquant` (72K ctx Llama-3.3-70B), `AmesianX/TurboQuant` (C++, source of the QJL-independent-seeds finding), `BoFan-tunning/llama.cpp-MTP-TurboQuant`, Vulkan work by jesusmb1995 (mixed K/V types, coopmat FA, 2.2–4.4x on RTX 4090 — branches referenced in discussion #20969, no standalone repo).
- Gist by veritatisquaesitoressumus (`gist.github.com/.../6aa5973955007ffd858889c76aa60408`): clean C + CUDA + Python + codebook JSON, QR-of-Gaussian rotation, 18/18 tests, MSE within paper's Theorem-1 bounds (b=3: ~0.034). QJL only in the Python prototype, absent from C/CUDA. **No license** — read-only reference. Proposed to ik_llama.cpp as issue #1509.

### Merged production implementations outside llama.cpp

- **vLLM upstream (real, merged)**: PR **#38479**, merged 2026-04-15 — `--kv-cache-dtype turboquant_k8v4 / turboquant_4bit_nc / turboquant_k3v4_nc / turboquant_3bit_nc`. WHT + random sign flips, **Lloyd-Max keys**, uniform values, norm correction variants, fused Triton store (`_tq_fused_store_mse`) + CUDA/Triton decode kernels. **QJL intentionally omitted** "per community consensus." Apache-2.0. Actively maintained with open bugs (tracking issue #40069; crashes with spec-decode/chunked-prefill #40807/#40831/#43357).
- **vllm-project/vllm-metal** (`docs/turboquant.md`): WHT + per-block quant on MLX/Metal, Lloyd-Max values, affine keys, FWHT kernel for d=64/128/256, paged attention required. No QJL.
- **MLX world**: `ml-explore/mlx-lm` issue #1060 open (not merged in official mlx-lm); community: `ekryski/mlx-swift-lm` (fastest Apple path, turbo4v2), `lablup/mlxcel` (Rust), two `turboquant-mlx` repos.
- Vector DBs: qdrant #8524 and faiss #4990 closed without implementation, milvus #48519 open; ollama #15051 open (go-engine impl in progress, 69 comments).

### Standalone algorithm implementations

- **0xSero/turboquant** (Python + Triton + vLLM monkey-patch, 1658 stars): **highest paper fidelity found** — Lloyd-Max Beta codebooks (`codebook.py`), QR-of-Gaussian rotation + QJL projection (`rotation.py`), **both Algorithm 1 (TurboQuantMSE) and Algorithm 2 (TurboQuantProd with QJL)** (`quantizer.py`), and a `validate_paper.py` that checks Theorems 1–3 including **unbiasedness (relative bias < 0.1%)** and the 1/4^b distortion scaling. Includes an unusually honest `audit_claims.py` (e.g. "5.1x compression is misleading — doesn't count Π/S matrices"). **License: GPL-3.0** — use for validation methodology, do not lift code.
- **RyanCodrai/turbovec** (Rust + Python bindings, 12,661 stars, **MIT**): TurboQuant as a vector index. `turbovec/src/codebook.rs` is a textbook Lloyd-Max on Beta((d−1)/2,(d−1)/2) via CDF + adaptive Simpson; `rotation.rs` is exact paper Stage 1 (QR of seeded Gaussian via faer, sign-corrected). No QJL; adds "TQ+" per-coordinate shift/scale calibration and an **unbiased inner-product estimator** via stored `||v||/⟨u,x̂⟩` correction. **Hand-written SIMD search kernels: AVX-512BW primary, AVX2 fallback, NEON — nibble-split LUT scoring (FastScan-style), runtime dispatch.** Beats FAISS FastScan by 10–19% on ARM. 2-bit and 4-bit.
- `tonbistudio/turboquant-pytorch` (1028 stars, from-scratch PyTorch, incorporates layer-adaptive + Sparse-V findings), `varjoranta/turboquant-vllm` (block_size=128 + K-dominance), `teamchong/turboquant-wasm` (Zig, relaxed SIMD), `AbdelStark/turboquant` (Rust), several Python WIPs (pyturboquant, OnlyTerp, OmarHory, yashkc2025).

### The honest counter-evidence

ikawrakow (ik_llama.cpp issue #1509, closed without adoption) is the strongest dissent, with measurements:
- ubergarm, CPU, Qwen3.5-35B-A3B: f16 6.5792 / **q4_0+Hadamard 6.5939 @ 1279 t/s** / **tq3_0 6.6967 @ 484 t/s** — turbo3 is worse than q4_0+Hadamard on both quality and speed there.
- Gemma3-12B @ 65K ctx: q4_0 +0.65% PPL vs turbo3 **+5.40%** — the long-context degradation is real (TheTom later shipped a partial fix, `docs/context-scaling-deep-dive.md`).
- Qwen3-0.6B: turbo3 PPL 1216 vs 13.5 f16 — catastrophic on small dense models. His framing: the method "does not turn into a complete disaster for Qwen-3.5" only because those models are hybrid (few full-attention layers), and advertised 6x is vs fp16, not vs the q4/q8 caches people actually use.

### Ranking as reference for a CPU (AVX2/AVX-512/NEON) implementation

1. **turbovec (MIT)** — the only impl with real hand-written AVX-512BW/AVX2/NEON kernels over Lloyd-Max codes (LUT-based asymmetric dot, no dequant). Best SIMD blueprint; caveat: vector-search framing, not a KV-attention pipeline.
2. **TheTom fork + turboquant_plus (MIT/Apache-2.0)** — the complete engine-integration reference: block formats, norm extraction, FWHT with precomputed signs, the asymmetric-K/V and block-128 findings, QJL code you can flip on for experiments. Codec is scalar C — pair it with turbovec's kernel style.
3. **Aaryan-Kapoor TQ3_0 branch (MIT)** — single readable commit for a ggml-style CPU KV type incl. vec_dot wiring; exact constants match the fork's.
4. **vLLM #38479 (Apache-2.0)** — production dtype/kernel architecture and the norm-correction ("_nc") variants; GPU-oriented but the fused store/decode structure translates.
5. **0xSero (GPL-3.0)** — read for the theorem-validation and audit harness (unbiasedness, 1/4^b scaling, recall floors); do not copy code.
6. **veritatis gist (no license)** — clean minimal C + codebook JSON; reference-only.

**Bottom line**: No implementation anywhere ships paper-complete two-stage TurboQuant (rotation + Lloyd-Max + QJL residual) in production. Upstream llama.cpp has only the rotation (not TurboQuant). The TheTom fork and vLLM upstream ship genuine Stage-1 TurboQuant (rotation + MSE-optimal Beta codebooks + norm), and the entire community — five independent groups — converged on dropping Stage-2 QJL for KV attention because its unbiasedness buys variance that softmax amplifies (with AmesianX's caveat that seed-decorrelation may rehabilitate it). For a CPU engine: FWHT + seeded signs + fp16 per-block norm + the converged 3/4-bit Beta centroids (all impls agree on the same table, ~±{0.0217, 0.0668, 0.1188, 0.1902} for 3-bit at d=128), block 32–128, skip QJL, consider q8-K/turbo-V asymmetric — and benchmark against q4_0+Hadamard, not f16, because that is the comparison the method has repeatedly lost on small/dense models and long contexts.

---

## B. Paper digest — arXiv 2504.19874, implementation-grade

**Paper**: "TurboQuant: Online Vector Quantization with Near-optimal Distortion Rate" — Zandieh (Google Research), Daliri (NYU), Hadian (Google DeepMind), Mirrokni (Google Research). arXiv v1 2025-04-28, 25 pp. (The abs page lists no venue; only one arXiv version exists.)

**Prior paper it builds on**: QJL, arXiv 2406.03482 (Zandieh, Daliri, Han) — cited as [62]/[63]. **PolarQuant (2502.02617) appears ONLY as a baseline** in Fig. 4 / Table 1; nothing of PolarQuant's polar-coordinate construction is inside TurboQuant.

Notation: `x ∈ S^{d-1}` is the vector being stored (a key or value), `y` is the query-side vector, `b` = bits per coordinate.

### 1. Pipeline structure (store time vs read time; K vs V)

**Store time** (per vector `x`, assumed unit norm; otherwise store `‖x‖₂` in fp and rescale at read):

- **`TurboQuant_mse`** (Algorithm 1): `y ← Π·x` (random rotation); for each coordinate `j`: `idx_j ← argmin_k |y_j − c_k|` over a fixed `2^b`-entry scalar codebook. Store `idx` (`b·d` bits).
- **`TurboQuant_prod`** (Algorithm 2), the inner-product/unbiased variant: run `Quant_mse` at bit-width **b−1**; compute residual `r ← x − DeQuant_mse(idx)` (**in the original basis** — DeQuant includes the back-rotation `Π^T`); compute `qjl ← sign(S·r)` (`d` sign bits, `S` is `d×d` i.i.d. N(0,1)); store `(idx, qjl, γ=‖r‖₂)`. Total `b·d` bits + one fp scalar `γ` (+ one fp scalar `‖x‖₂` for non-unit inputs).

**Read time**: dequantization is linear: `x̃ = Π^T·c[idx] + (√(π/2)/d)·γ·S^T·qjl`. Inner product with query `y`:

```
⟨y,x⟩ ≈ ⟨y, x̃_mse⟩ + γ·(√(π/2)/d)·⟨S·y, qjl⟩  =  ⟨Π·y, c[idx]⟩ + γ·(√(π/2)/d)·⟨S·y, qjl⟩
```

so per query you compute `Π·y` and `S·y` once, then per stored vector do a codebook-LUT dot plus a ±1-weighted sum. The estimator is **asymmetric — the query is never quantized** (stated as essential in QJL Def. 3.1: symmetric sign-quantization gives an angle estimator, and cos(·) of it is biased).

**K vs V: the paper does NOT differentiate them.** No sentence assigns different variants or bit widths to keys vs values; all KV experiments treat "KV cache" as a unit. Deliberate contrast with the prior QJL paper, which used QJL for keys and plain per-token integer quantization for values. *Inference (not in paper)*: since K feeds `q·kᵀ`, `Q_prod` is the natural choice for K (unbiasedness matters pre-softmax); V is consumed as a convex combination, where `Q_mse`'s bias largely averages out — an implementer could use `Q_mse` for V and save the sign-bit plane + γ, but the paper neither says nor evaluates this.

### 2. Stage 1 rotation

- **Construction (explicit)**: "Generate Π by applying QR decomposition on a random matrix with i.i.d Normal entries" — a Haar-random `d×d` orthogonal matrix. **One fixed Π per quantizer instance** (Global Parameters), shared by all vectors that instance quantizes, reused at read time.
- **Granularity for KV (partially explicit)**: the LongBench setup splits the **128 channels** (= head_dim of Llama-3.1-8B and Ministral-7B) into outlier/non-outlier sets and runs "two independent instances of TurboQuant" on them — so `d` is (a partition of) the head dimension, e.g. a 32×32 and a 96×96 rotation for the 2.5-bit point. Whether Π is shared across layers/heads, and how outlier channels are identified, is **not stated**; the sibling QJL paper identifies outlier channels "during the prompt phase" per layer — *reasonable default, flagged as inference*.
- **Seed handling: not discussed at all.** *Implementer decides*: any PRNG; Π (and S) must be reproducible/stored between store and read.
- **Hadamard: zero mentions in the paper** (grep-verified). No complexity discussion of the rotation either. *Community-knowledge inference*: a randomized Hadamard transform (D·H, random signs + Walsh-Hadamard) is the standard O(d log d) substitution and preserves the "coordinates ≈ i.i.d. N(0,1/d)" property; the theory's exact Beta-marginal claim (Lemma 1) technically requires Haar uniformity, so the constants become approximate. Note the QJL paper *does* report that orthogonalizing S's rows (QR on Gaussian) "almost always improves" quality — TurboQuant's Alg. 2 spells plain i.i.d. Gaussian S, no orthogonalization.

### 3. Stage 1 codebooks (Lloyd-Max on the Beta/Gaussian marginal)

- **Target distribution (explicit, Lemma 1)**: after rotation, each coordinate of a unit vector has density `f_X(x) = Γ(d/2)/(√π·Γ((d−1)/2))·(1−x²)^((d−3)/2)` on [−1,1] (a scaled/shifted Beta), converging to **N(0, 1/d)** for large d. Coordinates nearly independent in high d, so scalar quantization per coordinate is near-optimal.
- **Construction (explicit)**: 1-D continuous k-means / Lloyd-Max, Eq. (4): minimize `Σ_i ∫ |x−c_i|²·f_X(x) dx` over Voronoi cells (boundaries = midpoints). "We solve [it] for a range of practically relevant bit-widths b **once, and store the results**" — fixed constants, computable offline, **one shared codebook for all coordinates**, scaling as `1/√d`.
- **Values the paper lists (explicit, Gaussian-limit)**: b=1: `{±√(2/π)/√d}` (=±0.7979/√d); b=2: `{±0.453/√d, ±1.51/√d}`. b=3,4 not printed — *inference*: classic Max(1960) Lloyd-Max Gaussian levels scaled by 1/√d (8-level: ±0.2451, ±0.7560, ±1.3439, ±2.1520; 16-level: ±0.1284, ±0.3881, ±0.6568, ±0.9424, ±1.2562, ±1.6181, ±2.0690, ±2.7326). The paper's own D_mse values 0.36/0.117/0.03/0.009 match the standard Gaussian Lloyd-Max MSEs 0.3634/0.1175/0.0345/0.0095, confirming this reading.
- **Scales/normalization (explicit)**: input assumed `‖x‖=1`; otherwise store the L2 norm in fp and rescale after dequantization. **No per-block scales exist** — the rotation replaces per-group max/min scaling entirely. (Considered and rejected: entropy-coding the indices saves ~5% at b=4; skipped for speed.)

### 4. Stage 2 QJL

- **Projection (explicit)**: `S ∈ R^{d×d}`, i.i.d. N(0,1) — QJL with **m = d** projections, 1 bit per coordinate. (In the QJL paper m is free; `m ≈ ε⁻²·log n` suffices for n keys.)
- **Quantizer / dequantizer (explicit, Def. 1)**: `Q_qjl(r) = sign(S·r) ∈ {−1,+1}^d`; `Q_qjl⁻¹(z) = (√(π/2)/d)·S^T·z`.
- **Residual (explicit)**: `r = x − Q_mse⁻¹(Q_mse(x))`, with `E[‖r‖²] = D_mse at (b−1) bits`; `γ = ‖r‖₂` stored exactly (fp).
- **Estimator (explicit)**: `⟨y, x̃⟩ = ⟨y, x̃_mse⟩ + γ·(√(π/2)/d)·⟨S·y, sign(S·r)⟩`; unbiased (Theorem 2, via QJL Lemma 3.2 applied to r/γ).
- **Variance (explicit, Lemma 4 + Thm 2 proof)**: `Var ≤ (π/2d)·‖r‖²·‖y‖²`, hence `D_prod ≤ (π/2d)·‖y‖²·D_mse(b−1) ≤ (√3·π²·‖y‖²/d)·4^{−b}`. At b=1..4: `D_prod ≈ 1.57/d, 0.56/d, 0.18/d, 0.047/d` (×‖y‖²). QJL high-probability form: `|est − ⟨q,k⟩| ≤ ε‖q‖‖k‖` w.p. 1−δ for `m ≥ (4/3)(1+ε)ε⁻²log(2/δ)`; attention scores get (1±3ε) relative error for `m ≥ 2r²ε⁻²·log n`.
- Why two stages: `Q_mse` alone is biased for inner products — at b=1 it degenerates to `sign(Π·x)` with `E[⟨y,x̃⟩] = (2/π)·⟨y,x⟩` (explicit example §3.2); bias shrinks with b but never vanishes. Stage 2 makes the estimator exactly unbiased while adding only the residual's variance.

### 5. Bit accounting

- **Core accounting (explicit)**: bit-width b means `B = b·d` payload bits per vector: `(b−1)·d` index bits + `d` sign bits for `Q_prod`, or `b·d` index bits for `Q_mse`. **Metadata is extra and uncounted**: `γ` (per vector, `Q_prod` only) and `‖x‖₂` (per vector, non-unit inputs) in fp. No block/group scales.
- **Where 3.5 (and 2.5) comes from (explicit, §4.3)**: NOT fractional-bit coding — **mixed per-channel-set allocation over head_dim=128**: "in our 2.5-bit setup, 32 outlier channels are quantized at 3 bits, while the remaining 96 channels use 2 bits → (32×3+96×2)/128 = 2.5. For 3.5-bit quantization, a different ratio of outliers and regular channels leads to a higher effective bit precision" — the 3.5 split ratio is **not given** (64/64 at 4/3 bits would produce it; *inference*). Two independent TurboQuant instances, one per channel set; outlier strategy credited to QJL [63] and RotateKV [51].
- **Other operating points**: theory covers all `b ≥ 1` (Q_prod) / `b ≥ 0` (Q_mse); experiments use 2.5 and 3.5 bits for KV, 4-bit for NNS timing, NIAH at "memory compression ratio 0.25" (~4 bits effective vs fp16; *arithmetic inference*). Claimed compression: "at least 4.5×" at 3.5 bits, "exceeding 5×" at 2.5 bits (metadata not counted).

### 6. Hot path at decode time

No kernel-level description in the paper; below the first sentence of each bullet is *implementation inference from the algebra*:

- **Per query (once per rotation instance)**: two `d×d` matvecs, `u = Π·y` and `v = S·y`. No per-key dequantization to a full fp vector is ever needed.
- **Per K vector (dot product)**: `Σ_j u_j·c[idx_j] + γ·(√(π/2)/d)·⟨v, qjl⟩`.
  - MSE part: single shared ≤16-entry codebook → PQ-style in-register LUT: unpack 2–4-bit indices, `vpshufb`/`vqtbl` idx→centroid, FMA with `u`.
  - QJL part: ±1-weighted sum of `v` — XOR the sign bits into `v`'s fp sign bit (or masked add/sub), horizontal reduce. **Pure XOR+popcount is NOT available as specified** — the estimator is asymmetric (query side stays real-valued); popcount form requires additionally binarizing/int-quantizing `v`, an off-paper accuracy tradeoff.
- **Per V vector (weighted sum)**: dequantization is linear, so accumulate in quantized domain: `Σ_i w_i·ṽ_i = Π^T(Σ_i w_i·c[idx_i]) + (√(π/2)/d)·S^T(Σ_i w_i·γ_i·qjl_i)` — LUT-weighted accumulations per token, then **one** `Π^T`/`S^T` application per attention output, not per token. (*Inference; the paper never spells out the V-side read path.*)
- Natural SIMD primitives: byte shuffle (LUT), integer unpack, FMA, sign-bit XOR; popcount only in the off-paper symmetric variant. Store-side: matvec (batchable), nearest-centroid = branchless binary search over 2^b−1 midpoint boundaries.

### 7. Quality results

- **NIAH** (Llama-3.1-8B-Instruct, 4k–104k tokens, all methods at 25% memory): TurboQuant **0.997 = full-precision 0.997**; PolarQuant 0.995, KIVI 0.981, PyramidKV 0.895, SnapKV 0.858.
- **LongBench-E** (Llama-3.1-8B-Instruct averages): Full 50.06; **TurboQuant 3.5-bit 50.06 (exact tie)**; TurboQuant 2.5-bit 49.44; PolarQuant 3.9-bit 49.78; KIVI 5-bit 50.16; KIVI 3-bit 48.50. Ministral-7B: Full 49.89 vs TurboQuant 2.5-bit 49.62. Note TurboQuant quantizes **generated tokens too** ("streaming"), while KIVI/PolarQuant leave generated tokens unquantized — stricter regime.
- **Where it degrades**: at 2.5 bits the weak category is Summarization (24.80 vs 26.55 full on Llama; 24.91 vs 26.09 on Ministral); everything else within noise. No breakdown point below 2.5 bits reported.
- **NNS**: DBpedia-OpenAI3 (d=1536, 3072) + GloVe (d=200), 100k base / 1k queries — TurboQuant wins recall vs PQ (LUT256) and RabitQ everywhere; indexing time at 4-bit is 0.0007–0.0021 s vs 37–494 s (PQ) and 597–3957 s (RabitQ) on an A100.
- **"Within 2.7× of the bound" (explicit)**: applies to the **MSE variant**: upper `(√3π/2)·4^{−b}` vs lower `4^{−b}` (Theorem 3) — ratio `√3π/2 ≈ 2.72` asymptotic, ≈1.45 at b=1. The **inner-product variant's** gap is bigger: `√3π² ≈ 17.1` vs `(‖y‖²/d)·4^{−b}` — the abstract's 2.7 does *not* apply to Q_prod.

### 8. Theory-vs-practice gaps (implementer must decide)

Explicitly absent from the paper (each verified by search):
- **RoPE**: never mentioned. *Inference*: quantize keys post-RoPE (the estimator reconstructs `q·kᵀ` of whatever was stored); pre-RoPE storage would require dequantize→rotate→dot and breaks the LUT read path.
- **Which variant for K and V** — not stated. **Per-head vs fused** — 32/96 split of head_dim=128 implied; sharing of Π/S across heads/layers unstated. **Outlier-channel identification** — unstated (QJL: by magnitude during prompt phase, per layer).
- **Attention sinks / recent full-precision window: none** — quantizes everything, including streamed generated tokens.
- **Seeds/PRNG, storage of Π and S**, fp precision of γ and norms, metadata overhead accounting (γ + norm per key per channel-set ≈ ~0.5 bit/coordinate at fp16 if per-128-dim; the paper's "b bits per channel" excludes this).
- **The 3.5-bit outlier/regular split ratio** and bit assignment (only the 2.5-bit example given).
- Exact-Beta vs Gaussian-limit codebooks at small d (at d=32 the Beta correction is non-negligible — solve Eq. (4) against the true `f_X` per sub-dimension if channels are split).
- No code release mentioned.

### 9. Compute overhead

- **Store time per vector**: one `d×d` matvec (`Π·x`) + nearest-centroid per coordinate; `Q_prod` adds a second matvec (`S·r`), a subtraction, a norm. All batchable — substance of the "indexing time virtually zero" claim (100k vectors in ~1–2 ms at d=1536–3072 on A100). No FLOP analysis of the rotation in the paper; no O(d log d) alternative discussed.
- **Query/read time**: query is **rotated but never quantized** — `Π·y` and `S·y` once per query per instance (O(d²) each; per channel-set if split), then per stored vector only LUT-dot + sign-weighted-sum (O(d), tiny constants). Back-rotation `Π^T` only when a full vector is needed (V-side output, once per output vector — *inference*).
- KV at head_dim 128 (32/96 split): store ≈ 32²+96² ≈ 10.2k MACs per key per head (×2 for Q_prod) — small next to the layer matmuls; query cost identical, once per decode step per head. (*Arithmetic inference.*)

**Cross-check identity for unit tests**: `D_prod(b) = (π/2d)·D_mse(b−1)`: 1.57=π/2·1, 0.56≈π/2·0.36, 0.18≈π/2·0.117, 0.047≈π/2·0.03 — measure empirical MSE at b−1 and empirical inner-product variance at b; their ratio must be π/(2d).
