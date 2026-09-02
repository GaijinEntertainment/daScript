# dasLLAMA Architecture - engine file charters

Companion to `ARCHITECTURE.md`; section numbers are that document's.

`REVIEW.das`'s `check_exe_fn_global_restore` walks every `dasllama/` file and licenses no
names: each `var g_x = @@fn` declaration carries an `if (g_x == null)` boot-restore guard in
the same file - a serialized exe restores globals as data, and only a boot-restore `[init]`
heals the null function value.

`REVIEW.das`'s `check_race_bind_numbers` walks every `dasllama/` file: each hand-written
`kn_buffer`/`kn_kargs` literal in a race or knockout arm lands on a binding number its
target kernel class declares (the class resolves from the arm's `kn_tgmem` constant - with
or without the kernel's own `_msl` suffix - or from `kn_pipeline`'s pso built by
`pipeline_from_source` in the same function; a `@uniform` slot legitimately takes a 4-byte
buffer). The licensed residue is exactly the arms whose pso or tgmem arrives as a FUNCTION
PARAMETER (the parameterized race helpers) - those the check cannot resolve textually and
stay the reviewer's. A mis-numbered arm dispatches, reads the wrong buffer, and
`race_envelope_ok` fails closed - the twin silently loses its crown.

### 1.1 Engine core

- **`dasllama.das`** - the public API facade and nothing else: `load_model` -> `create_session` ->
  generate, re-exported names, the doc surface. No engine logic; a function that does work belongs
  in the module that owns the concern, and the facade re-exports it.
- **`dasllama_version.das`** - `DASLLAMA_RELEASE`, the module's release number, and nothing
  else. Decoupled from the daslang version, `LLVM_JIT_CODEGEN_VERSION` and `IMAGE_VERSION`;
  bumped only on a declared release (REVIEW.md), so tune sidecars and board rows live for a
  whole release - the sidecar exchange keys validity on (release, box), under the
  schema-stable wire name `dasllama_version`. A zero-require leaf so the tuner's noise half,
  the benches, and the facade chain (`dasllama_common` re-exports it) all reach it without
  weight.
- **`dasllama_common.das`** - the engine: `Model`/`Session`/`Config`, the forward loops, the
  override registries, the runtime knobs. **Not** the load walk (sec.1.3) and **not** GPU residency
  (`ARCHITECTURE_GPU.md` sec.1.5) - both left, and the seam each left behind is a registered hook,
  so neither comes back.
  It remains the module's debt sink; what sits here that is family-specific or platform-specific is
  debt, **not precedent**. Ledgered exceptions the sidecar-apply seam forces
  (the knob rail lives here, and the backends require this module, so their sidecar knobs cannot):
  the metal tensor crowns, and the three metal prefill raced-constant knobs beside them - both are
  applied by `apply_box_profile_runtime` and CONSUMED by the Metal backends at their own init.
  `REVIEW.das`'s `check_fastmath_default` licensed set is empty by design: no engine kernel opts
  out of fastmath, and an entrant ships its failing-under-fastmath test with the kernel.
- **`dasllama_transformer.das`** - the block-composition seam, and the require umbrella: the
  `[init]`-only side-effect requires (arch registrations, GPU tiers, and any module that
  requires the engine back, which the umbrella therefore hosts to break the cycle).
- **`dasllama_blocks.das`** - the std/dense/MoE transformer block kernels, decode and prefill,
  plus the two default block sets the arch files bind. `forward()` never names a kernel here - it
  dispatches through the `ArchBlocks` function pointers, which is why the family lives outside the
  hub with no hook.
- **`dasllama_moe.das`** - MoE expert routing and dispatch: the top-k router, per-expert FFN
  accumulation, the shared expert, and the decode instruments. The block kernels reach it only
  through `moe_ffn_core`.
- **`dasllama_attn_prefill.das`** - prefill attention, threaded over heads. `prefill_attention`
  is the only entry; the classic/blocked/flash head kernels and their KV-codec ladders are its
  implementation.
- **`dasllama_batch.das`** - the batched decode step: one pass of the weights over B sessions,
  GEMVs widened to B-row GEMMs, attention still per-(row, head) against each session's own cache.
- **`dasllama_mtp_gemma.das`** - the gemma-4 assistant drafter's loader (`GemmaDrafter`: the
  sidecar's Q8_0 weights as one q8 blob + its F32 norms) and the CPU reference forward of one draft
  step - the oracle the GPU drafter is measured against, never the serving path.
- **`dasllama_mtp_gemma.das`** - the gemma-4 assistant drafter (`gemma4-assistant`), which is a
  SIDECAR head, not a trunk block: it owns no K/V projection and borrows the target trunk's cache
  at two capture layers, so it never rides the arch registry or `forward_mtp`. The file holds the
  sidecar resolver, the refusing loader (`GemmaDrafter`, whose `blob` is raw GGUF Q8_0 = the Metal
  34B form, so the GPU encoder uploads it with no repack), and the CPU reference forward the GPU
  encoder is scored against. A refusal is `ok = false` plus a `why`, never a panic - a drafter is
  optional and a bad sidecar must degrade to plain decode.
- **`dasllama_sampling.das`** - token sampling and the generation drivers. A leaf on top of
  `forward`/`eval_batch`; the engine never calls back in.
- **`dasllama_ple.das`** - gemma-4 E-series per-layer embeddings and the gemma4 MoE FFN. The
  forward sequence reaches it only through the hooks it registers at init.
- **`dasllama_config.das`** - `DlimConfiguration`: every input that changes `.dlim` image BYTES,
  in one struct, plus its identity formatter. A knob that does not change image bytes does not
  belong here; a knob that does and is missing is an image-aliasing bug.
- **`dasllama_tools.das`** - the per-ToolMode tool wire codecs (definition serializers,
  replay/result text builders, reply parsers; Harmony namespace/channels, gemma-4 DSL, mistral
  control tokens, llama JSON). Every byte of tool wire text is produced here - pure string+JSON
  functions, model-free testable; the chat layer assembles the output into ChatParts and the
  server parses through the parsers. Wire-format provenance: `history/dasLLAMA/tool_formats_plan.md`.
- **`dasllama_chat.das`** - conversation turns and chat-template application. Per-arch template
  *content* is registered by the arch file (sec.1.6), not written here.
- **`dasllama_par.das`** - `maybe_parallel_for` and nothing else. Threading policy (job counts,
  thresholds) belongs to the caller that knows the shape.

### 1.2 Formats and data movement

- **`dasllama_plane.das`** - the borrowed-plane vocabulary: a plane is a pointer into a prepared
  image plus its element count; the image owns the bytes, a carrier owns nothing but its backing.
  Requires nothing in dasllama - the image rail binds planes, every carrier holds them.
- **`dasllama_kqformat.das`** - format IDENTITY: the `KqFmt` enum, the per-format descriptor table
  (plane strides, block geometry, stream codes), format predicates, and the shared decode
  tables the grid and codebook formats key off - each as a builder function (`iq3s_grid()`,
  `iq4nl_lut()`) for kernels that may run on a team lane, plus a global twin for tests,
  oracles and the emitter's constant bake. It requires nothing else in
  dasllama, because it is the taxonomy everything keys off. ONE id space - the enum; integer ids
  exist only at the IR/kernel-param boundary. `kq_sb` is the superblock-lattice predicate: a
  `fmt != q8` test does not imply the lattice, so branch on the predicate.
- **`dasllama_convert.das`** - every tensor format CONVERSION: quantize/dequantize/transcode/encode,
  codec byte readers, numeric widen/narrow - regardless of platform or caller; plus the Q8_0/Q4_0
  format identity itself (block geometry, scale layout, the quantization-quality detector).
  Metadata-coupled drivers (GGUF lookup, threading, guards) stay with their containers and
  dispatch in. ONE carve-out: a conversion that IS a KV-cache format's store/read half lives with
  its codec family (sec. below).
- **`dasllama_repack.das`** - every disk-order -> compute-order kernel-LAYOUT transform (grp
  interleaves, disk-order extractors, panel unpacks), any format, any platform. Number sources
  (tune stamps, bake overrides) stay with their owners and pass plain parameters in.
- **`dasllama_kv_codec.das`** - the KV-cache runtime codec, one family per cache format, kept
  WHOLE: store (quantize), read (dequant), the attention score dot, and the V-accumulate axpy all
  share the format's block byte geometry, so a layout change can never span modules. These are
  per-token and per-tokenxposition `[tuned]` hot kernels, not load-time converters. The tq4 FWHT
  rotation and sign vector live with their family. Codec DISPATCH (`KVDtype`) stays at common's
  `kv_store_row`/`kv_load_row`/`kv_dot`/`kv_axpy` seam; the f16 row converts are the generic pair
  in `dasllama_convert` because they are dual-use beyond the cache.
- **`dasllama_rope.das`** - RoPE angle and TABLE GENERATION: the theta schedule, `rope_freqs`,
  fscale/mscale, every materialized layout - pure functions over plain parameters. Model-facing
  wrappers stay in common. APPLICATION kernels stay with their backends: the CPU `rope_scaled_*`
  leaves and the GPU fused rope-store kernels are shape-specialized deliberately, and
  single-sourcing them is generator-rail work, never a hand-merge. Float multiply order in the
  builders is contractual (parity-pinned).
- **`dasllama_gguf.das`** - the GGUF container: KV/tensor descriptors, the byte-level reader, the
  mapping. Codecs live in `dasllama_convert`; this file finds bytes, it does not decode them.
- **`dasllama_layout.das`** - disk-format -> compute-layout transforms at LOAD scope: the blob
  transform, the CPU repack walkers, the GPU tier gathers, the per-region q8->Metal-34B
  un-repack (`q8_region_to_metal_blob`, the ASR-decoder driver's upload form), and the refusal
  half (`can this model take the blob form`) split out so the image writer can commit without
  loading.
- **`dasllama_tokenizer.das`** - the tokenizer facade: backend selection off the GGUF metadata and
  the one encode/decode/piece surface models and the chat layer call. Re-exports both backends, so
  a consumer requires this file and never picks a backend by hand.
- **`dasllama_spm.das`** - the SentencePiece backend (Llama-2 family, Phi-3, Gemma): score-greedy
  merges over vocab pieces, `<0xXX>` byte fallback.
- **`dasllama_bpe.das`** - the byte-level BPE backend (Llama-3 / tiktoken family): vocab load, the
  GPT-2 byte alphabet, ranked merges, encode/decode. Split from SPM because the two algorithms
  share no state; a third merge algorithm gets a fourth file. Two sanctioned family-name tests
  live here rather than in `dasllama_pretok`: the `pre`-name selector inside `bpe_encode`, and the
  gemma-4 newline-run split in `bpe_encode_spm_space`; `load_bpe_tokenizer_gguf`'s per-family
  metadata defaults are the third and last.
- **`dasllama_pretok.das`** - the pre-tokenizer: one hand-compiled split function per family
  (llama3/qwen2/qwen35, gpt-2, gpt-4o, tekken), selected by the BPE `pre` name. Regex-port growth
  lands here, never in the merge engine - the two change for different reasons (new model family
  vs. algorithm work). Every arm with an on-disk upstream corpus vocab is gated by its case in
  `test_tokenizer.das` (llama3, qwen2, qwen35, gpt-2); tekken has no corpus case, and gpt-4o is
  pinned by frozen ids in `test_parity.das` only.

### 1.3 The load and image rail

**`dasllama_load.das`** is the GGUF load walk: metadata to `Config`, the plane layout, disk-format
detection, the eager and streamed conversion ladders, and the load entry points. It owns nothing the
forward path touches at run time - a loaded `Model` is the whole handoff - and it requires
`dasllama_common` back for `Model`/`Session`, so the transformer umbrella requires it `public` and
breaks the cycle. That re-export is what keeps every consumer on the facade.

- **`dasllama_image.das`** - the prepared-model `.dlim` rail, and it is ONE rail
  (`ARCHITECTURE_IMAGE.md` sec.2.1). Nothing
  outside this file may read weights into a live carrier; image backings are released only
  through the single release path in `dasllama_common` that the carrier finalizers call - never
  an ad-hoc unmap.

A split NextN/MTP head (the ggml-org `mtp-<model>.gguf` form, the trunk converted without its
head) rides the load as the LAST shard of the trunk's shard walk: `gguf_shard_paths` appends the
`mtp-<trunk basename>` sibling, or the `DASLLAMA_MTP_HEAD` file, and `parse_gguf_meta_shards`
promotes the head shard's `nextn_predict_layers` onto the meta when the trunk's KVs lack it. The
head's `blk.<n_layers>.*` tensors are the draft block; its copies of the trunk's embedding,
classifier and final norm are dead by construction (tensor lookup is first-match and the head
is last). The prepared image folds the head's name and size into its path hash, so the
trunk-only and trunk+head images never collide and one image file serves both trunk and head.

### 1.4 CPU kernel tiers

- **`dasllama_math.das`** - the numeric ABSTRACTION: typedefs, active backend pointers, public
  wrappers, dispatch shaping. Kernels themselves live in a tier file; a kernel body here is a
  placement defect.
- **`dasllama_math_default.das`** - the portable backend, always registered, always correct,
  out-ranked by any platform tier.
- **`dasllama_math_aarch64_neon.das`** - the arm64 SDOT/tbl tier. Its `[init]` never fires
  off-arch (`ARCHITECTURE_INVARIANTS.md` sec.3, three-layer safety model), so an intrinsic here
  needs a correct scalar fallback body, not a guard at the call site.
- **`dasllama_math_accelerate.das`** - the Accelerate/BNNS float tier (AMX on M1-M3, SME on M4+),
  for genuinely-float planes only. BLAS-for-quant is ruled out structurally
  (`ARCHITECTURE_INVARIANTS.md` sec.3).
- **`dasllama_math_gen.das`** / **`dasllama_gemm_gen.das`** / **`dasllama_gemm_schema.das`** /
  **`dasllama_gemm_register.das`** - the generated GEMM tier: the runtime registration, the tile
  generator, the layout/perm schema shared by generator and runtime, and the `[tune]` family
  registration. A hand-written tile that the generator could emit belongs in the generator.
- **`dasllama_tune.das`** - the per-box loop-hint tuner (`[tuned]` / `[dasllama_grid]`). Tuning
  POLICY lives here; tuned VALUES live in the box's sidecar, never in source.

### 1.6 Architecture registrations

Thirteen files registering eighteen names:
`dasllama_arch_llama.das` * `dasllama_arch_phi3.das` * `dasllama_arch_qwen2.das` * `dasllama_arch_qwen2moe.das` * `dasllama_arch_qwen3.das` * `dasllama_arch_qwen3moe.das` * `dasllama_arch_qwen35.das` * `dasllama_arch_gemma2.das` * `dasllama_arch_gemma3.das` * `dasllama_arch_gemma4.das` * `dasllama_arch_glm4moe.das` * `dasllama_arch_gptoss.das` * `dasllama_arch_mistral3.das`. They are DECLARATIVE: an arch
file builds an `ArchDesc` (name * `configure` * the `ArchBlocks` fn-ptr quad * `ChatTemplate` *
`LlmCaps`) and calls `register_arch` at `[init]`. Adding an arch touches no forward loop.

### 1.8 Instrumentation and support

- **`dasllama_parity.das`** - CPU-reference caches for the parity instruments. Test-facing, but
  library-side because the caches outlive a single suite.
- **`dasllama_prefix.das`** - the prefix/page cache for evaluated token history.
- **`dasllama_lint.das`** - the facade boundary as a compile-time lint (DASLLAMA001): every
  engine module carries it, so a consumer requiring anything under `modules/dasLLAMA/` but the
  entry modules (facade, scheduler, exchange pair) fails to compile. Escape:
  `options _dasllama_internal = true` - engine files, this module's own tests/harnesses/
  benchmarks/rigs, and the ruled consumers: `utils/dasllama-convert` (the bake tool reads
  the mint rail), `utils/dasllama-server/model_catalog.das` (the env rail + the model-set
  serving view; it re-exports `model_specs` so the server's catalog tests can gate the
  serve rows), `utils/dasllama-server/test_openai_server*.das` (env-registry test knobs),
  `modules/dasLLVM/daslib/llvm_user_modules.das` + `modules/dasLLVM/tests/test_{grid,tune,tuned}.das`
  (the tune-generator contract).
- **`performance/model_specs.das`** - the ONE model-set table: per carrier, the profiled-
  catalog fields, the official-board flag, the serving-catalog fields (`serve_*`, incl.
  `serve_vision` naming a tower companion by name), the provenance pin (exact HF repo +
  revision + sha256, or the on-box conversion recipe; companions hang off their owning entry,
  and a companion shared by several rows is referenced from the others by name), and the
  frozen parity evidence as data (pinned ids + arms). `llm_catalog` / `official_catalog` /
  `models_provenance` are views over it; `serve_vision_tower` resolves a tower name to its
  pinned companion, and `serve_asr_tower` is the one standalone pinned artifact the file
  carries - the serving ASR tower (parakeet v3), consumed by `asr_catalog`'s v3 row in
  `profile_common.das` (the pin's single source lives here, not there).
- **`performance/box_ident.das`** - the hardware-keyed box tag: the fleet table maps every
  recorded box's CPU brand to its store tag (the M3 row is an EXACT match so bigger M3s slug),
  the `DASLLAMA_BOX` override wins, and unknown hardware slugs from the brand - never another
  box's store. `profile_common.das`'s `box_name()` and `tests/run.das`'s manifest pick both
  resolve through it; the old per-file `"m1"` literals are gone.
- **`performance/fetch_models.das`** - the fetch/verify driver over the provenance view.
  Verify by default, `--fetch` downloads; it never converts on `--fetch`, never benches, and
  it touches no tune state (`tune_policy(missing="fallback")`). BRINGUP.md sec.2 is the runbook.
- **`dasllama/dasllama_exchange_schema.das`** - engine-free validation for exchange submissions
  (record stores + tune sidecars); the dasllama.io ladder service builds on it.
- **`dasllama/dasllama_exchange.das`** - the sidecar-exchange client (a sanctioned public entry point): the boot-time
  lookup/apply (llvm_tune's scope resolver - a verified per-box match downloads instead of a
  ~12-minute tune), the privacy-stripped submit rails, and the control-page surface
  dasllama-server serves at `/exchange`. The first-contact consent gate (GDPR) sits ahead of
  every lookup: an explicit `exchange_*` config counts as the expressed choice, otherwise the
  `<stem>.consent` sidecar-sibling file governs - unset asks on a real terminal, or emits
  `@sidecar consent state=needed` for the watchdog dialog / control page, and no request
  leaves until a surface records "accepted".
- **`benchmarks/asr/mem_census.sh`** - the peak-memory census (`/usr/bin/time -l` around one
  asr_bench process per cell; macOS only) - the interim footprint instrument until a footprint
  leg lands in `gen_bench_records`; its numbers live in `PERF_LEDGER.md`, never the stores.

### 1.9 Serving

- **`dasllama_scheduler.das`** - the continuous-batching scheduler, the serving layer over the
  facade (its one engine require is `dasllama/dasllama`). One synchronous thread: each
  `scheduler_step` admits queued requests, runs one `eval_batch` decode step over every
  decoding stream, then at most one bounded prefill chunk FCFS; paged serving donates finished
  streams' KV pages to the prefix cache. Results flow out as `SchedEvent`s - no HTTP here.
  `utils/dasllama-server` owns the writers; `tutorials/dasLLAMA/13_serving.das` is the
  teaching consumer; `tests/test_scheduler.das` gates it against `generate()` references.

