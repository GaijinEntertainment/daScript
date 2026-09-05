# dasLLAMA Architecture - inherited invariants

Companion to `ARCHITECTURE.md`; section numbers are that document's.

## 3. Inherited invariants

Durable "why it is built this way" facts harvested from the design docs archived under
`history/dasLLAMA/`. Violating one of these is a bug, not a style choice.

**From `x64_arch.md`:**

- **The LIBRARY never runs interpreted - the scaffolding may.** Inference - anything that runs
  a model (eval / decode / prefill / generate) - runs on a compiled tier, hard stop: an
  interpreted model run is not "slow but correct", it is out of contract, and the library panics
  on it. Three tiers pass: the JIT (the crown - loop hints, intrinsic lowering and the tuned
  kernels exist only there, and it is the tier the oracles gate), a standalone `-exe`, and a host
  that linked the engine's AOT stubs (the portable tier: the reference bodies as the C++ the AOT
  emitter produced, `examples/dasLLAMA`, `-use-aot`). **Scaffolding is exempt:** conversion
  utilities (`.dlim` bake, image processing), debug scripts, and batch/driver tools that spawn
  the real runner as a child may run interpreted. Enforcement is `guard_interp_gguf_load`
  (`dasllama_image.das` - big-load panic + `DASLLAMA_ALLOW_INTERP_LOAD` escape for the
  conversion path) plus `guard_interp_inference` (`dasllama_math.das`), which panics at the
  library seams: `make_run_state`, ASR session creation, and `vad_chunk_prob`. Both read the
  AOT tier through `aot_kernels_linked` - a runtime probe of one kernel's binding, never
  `aot_enabled()`, which folds at compile time and desyncs the semantic hash between AOT
  generation and the AOT run. Intrinsic fallback bodies are what the AOT tier runs where the
  JIT would lower an instruction; inside the JIT they exist for off-ARCH correctness.
- **Correctness before speed, token-for-token.** The engine is validated against external oracles
  (llama2.c + upstream `simple_ids`) plus per-arch parity fixtures. A new kernel passes the suite
  *and* the oracles with the new backend active before any perf claim.
- **Peak memory before cold-start latency - a DELIBERATE trade, and the standing tiebreak.** When a
  load-time choice pits footprint against wall-clock, dasLLAMA takes the smaller footprint. The
  asymmetry is not close: overshooting RAM on a big model is fatal (the OOM killer, or swap that
  makes the whole box unusable), while a slower cold start costs seconds *once per process* and
  costs the warm path - the common one - nothing at all. The prepared-image rail is where this bites
  hardest and where the shape is set: a cold load writes its `.dlim`, drops the model, and maps the
  file back rather than serving the copy it already has in RAM. That is a real close/reopen and a
  real re-fault of a multi-GB file, and on an image larger than the page cache it is a re-read from
  disk. Measured on Llama-3.1-8B-Q8_0 (9.6 GB image, M1 Max, `phys_footprint_peak`): planar cold
  **3.7 s -> 4.5 s**, peak **12.3 GB -> 5.4 GB**. We bought a 2.3x footprint cut with ~20% of one cold
  start. Apply the same tiebreak everywhere - bake, convert, KV growth, GPU staging - and when a
  change goes the other way, it needs the measured pair (peak AND wall) and an explicit call, not an
  assumption that faster is better.
- **Cold builds STREAM; nothing materializes a model to write one.** The rail that makes the trade
  above cheap: a cold build transcodes each plane from the gguf mapping straight into the image, so
  the model never exists in RAM at all and the peak is a few per-tensor temps over the small
  RAM-resident scale planes. This covers the planar flavor and the metal blob flavor alike - the
  blob's gguf-native 34B blocks assemble per tensor on the way out, and because a blob forbids CPU
  repack, it does not even need the resident `qscales` plane the planar stream keeps. Metal cold on
  the 8B: **19.5 GB -> 4.8 GB, 4.1 s -> 3.1 s** (faster AND smaller - the eager rail had been
  splitting gguf's interleaved blocks apart only to reassemble them). The vulkan flavor rides it
  too: its bake collects from the GPU walk of a model that is **already serving**, so it needs a
  mapping rather than a load - which is what the warm path always did, and now the cold path does
  the same instead of loading eagerly (**15.4 GB -> 6.6 GB**). A new flavor joins the streamed rail
  or explains why it cannot.
- **Token-exact oracle tests pin the bit-exact path** (classic attention, scalar activation);
  approximate/fast paths get separate tolerance tests. Rerouting an oracle test through a
  non-bit-exact default makes it pass on the machine it was frozen on and flip elsewhere.
- **The three-layer safety model that makes a new ISA free:** (1) *registration gate* - an ISA's
  `[init]` never fires off-arch, so `portable` stays active and every run is correct; (2) *scalar
  fallbacks* - an intrinsic called off-target computes the right answer from its fallback body, only
  the JIT emitter lowers it to hardware; (3) *tests drive public wrappers*, asserting only that *a*
  row-major default is active, never an arch-specific name. A new backend is a file plus an `[init]`.
- **The u8xs8 sign trick, and which operand takes the `|a|`.** ARM `sdot` is signedxsigned; x64
  `vpdpbusd` is *unsignedxsigned*. The exact integer identity is
  `dot(a,b) == dot(|a| as u8, sign(a) applied to b)` - and `|a|` goes on the **unsigned** operand,
  because `|-128| = 128` fits u8 but not s8. Products fit s16, four-product sums fit s32: bit-exact,
  no saturation.

**From `kv_cache_compression.md`:**

- **The KV layout is ragged per-layer, not flat.** `kv_row_total` *sums* per-layer `layer_kv_dim`
  (layers differ under SWA/global splits), and a layer's base offset is a **prefix sum**
  (`kv_row_prefix[l] * seq_len`). A codec keys strides and offsets off the prefix array - never off
  one flat `kv_dim`.
- **Cross-layer KV sharing stores nothing.** When `kv_src[l] != l` both store sites are skipped and
  the layer's offset **aliases the source layer's region**. Any codec must preserve that aliasing.
- **Convert the query, not the cache.** The query is `O(head_dim)` per step; the cache is
  `O(context x head_dim)`. Dequantizing the cache on the decode read costs *read compressed + write
  f32 + read f32* ~ 2x the f32 baseline - strictly worse than storing uncompressed.
- **Prefill and decode are asymmetric on purpose.** Prefill is compute-bound and tolerates external,
  one-layer-at-a-time decompression (a fresh prompt reads the f32 `k_b`/`v_b` scratch and
  decompresses nothing); the decode read is memory-bound and **must fuse** dequant in-loop.
  Attention consumes one layer at a time - never materialize all layers at once.
- **BLAS is ruled out structurally, not incidentally.** `cblas_sgemm` is f32-only (no hgemm, no
  int8), so any quantized operand must expand 4x into f32 scratch - and the weight matmul is bound on
  bytes streamed, so quantized-native `sdot`/VNNI wins regardless of FLOPs.
  `dasllama_math_accelerate.das` ships an Accelerate float-batch override plus a BNNS-f16 lane
  for bf16 planes (`DASLLAMA_ACCEL` / `DASLLAMA_ACCEL_F16`). BLAS-for-quant stays ruled out;
  Accelerate/BNNS on genuinely-float planes is live and measured.

**From `turboquant_research.md`:**

- **Stage-2 QJL is skipped deliberately** in the tq4 KV codec - its unbiasedness buys variance that
  softmax amplifies, and the damage grows with context. Five independent groups plus vLLM upstream
  converged on Stage-1 only (rotation + Lloyd-Max Beta codebooks + per-block norm). Do not "restore"
  QJL as a completeness fix.
- **Benchmark tq4 against `q4_0` + Hadamard, not f16.** f16 is the comparison the method wins by
  construction; `q4_0`+Hadamard is the one it has repeatedly *lost* on small/dense models and long
  contexts. Quoting an f16 ratio is the mistake this doc exists to prevent.
- **Two corrections that travel with the codec:** the estimator is **asymmetric** - the query is never
  quantized, so a QJL dot is sign-XOR over fp lanes, not popcount (popcount needs the query binarized
  too, an off-paper accuracy trade); and "3.5-bit" is **mixed per-channel bit allocation** over
  head_dim, not fractional coding.

**From `API_REWORK.md`:**

- **The Tier-1/2/3 scope model.** Tier 1 = scalar `Config` knobs (RoPE variant/scaling, norm kind and
  placement, activation, QKV bias, softcaps, sliding window, embed scale). Tier 2 = block swaps that
  change dataflow (MoE, QK-norm, per-layer attention patterns, shared KV, PLE, sinks, parallel
  attn+FFN). Tier 3 = deep forks needing a pluggable KV/attention core. The hybrid
  Gated-DeltaNet lane is a Tier-3 fork that serves: `dasllama_arch_qwen35.das`
  registers `qwen35`/`qwen35moe`/`qwen3next`, gated by `tests/test_deltanet.das`. The arch registry
  (18 names across 13 `dasllama/dasllama_arch_*.das` files) is the truth for what exists.
- **Family behavior is distributed by `Config` flag, not dispatched by name.** GeGLU-vs-SwiGLU is an
  `ffn_act` flag *inside* the shared dense FFN block; only a genuinely new dataflow (MoE) earns its
  own `ffn_*` block pointer. An `if (arch == "...")` on a shared path is the anti-pattern.
- **`register_arch` MOVES the `ArchDesc`** (`g_arch_registry[name] <- desc`). Clone the template
  **before** registering an alias, or the second call registers a gutted descriptor - the live
  pattern in the qwen2 / qwen3 / qwen3moe / qwen35 files, each with the comment saying so.
- **What rides where:** `ArchBlocks` (copyable fn-ptrs) is copied onto the `Model` at load; the full
  `ArchDesc` (non-copyable - it holds a `ChatTemplate`) stays in the registry. Image loads re-bind
  blocks via `rebind_arch_blocks` and must **not** re-run `configure` - the serialized config already
  carries configure's flags *plus* the GGUF's overrides.
- **The kernel/ISA seam is symmetric with the arch seam, plus one extra rule.** `dasllama_math.das`
  owns the abstraction (typedefs, active pointers, public wrappers); ISA modules self-register at
  `[init]`. Two selection tiers: `register_kernel_backend` auto-activates only the best **no-repack**
  backend, so direct callers (tests, benches, row-major weights) always get a kernel that works on
  unrepacked data; `select_matmul_backend_for_load()` is the **only** path that activates a
  `needs_repack` backend - eagerly activating one runs an interleaved kernel over row-major data,
  i.e. silent garbage.
