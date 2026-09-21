# dasLLAMA Architecture - engine file charters

Companion to `ARCHITECTURE.md`; section numbers are that document's.

`ARCHITECTURE_ENGINE_FORMATS.md` holds sec.1.2-1.4: the format, load-rail and CPU-kernel-tier
charters.

`REVIEW.das`'s `check_exe_fn_global_restore` walks every `dasllama/` file and licenses no
names: each `var g_x = @@fn` declaration carries an `if (g_x == null)` boot-restore guard in
the same file - a serialized exe restores globals as data, and only a boot-restore `[init]`
heals the null function value. `check_test_file_placement` walks every folder under
`modules/dasLLAMA/` but `tests/` and reds a `[test]` file that requires a `dasllama/` module
without requiring a file of its own folder by bare name, licensing no names - the bench
self-check beside its subject is the shape the bare require admits.

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
  override registries (the accept walk's row-sampler seam among them), the runtime knobs,
  `SamplingParams` (the struct a `Session` points at, so a speculative round draws with its
  caller's sampler), and the MTP per-position accept telemetry (`mtp_pos_*`) the round-override
  registry's rounds feed. The standard attention's Config-keyed arms live beside it - the gated
  projection (`q_gated`: a 2x-wide q whose second half sigmoid-gates the output) among them - one
  kernel every arch shares, its arms chosen by the model's flags. **Not** the load walk (`ARCHITECTURE_ENGINE_FORMATS.md` sec.1.3) and **not** GPU residency
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
  is the only entry; the classic/flash head kernels and their KV-codec ladders are its
  implementation.
- **`dasllama_batch.das`** - the batched decode step: one pass of the weights over B sessions,
  GEMVs widened to B-row GEMMs, attention still per-(row, head) against each session's own cache.
  A step is served one of three ways, and `batch_step_census` counts each since load: the armed
  device driver claimed the whole stack, the CPU batched stack ran it, or the rows stepped one at
  a time through the single-row forward - one row, a non-standard graph whose arch
  names no batched layer (`ArchBlocks.attn_batch`) with no device driver armed, or a step the
  device driver declined on a blob-only model or on such a graph (the CPU stack has no form for
  either, so the decline falls to the single-row forward). A non-standard graph reaches an armed
  device driver before the CPU stack; the deltanet hybrids name `attention_qwen35_batch` as their
  batched layer - the recurrent layers as B-row projections and the delta rule laned over (row,
  v-head) against each session's own state, the attention layers through the std batched
  attention, whose `q_gated` arms carry the 2x-wide [q | gate] projection and the sigmoid
  out-gate - so a hybrid step with no device driver rides the CPU stack too.
- **`dasllama_mtp_gemma.das`** - the gemma-4 assistant drafter (`gemma4-assistant`), which is a
  SIDECAR head, not a trunk block: it owns no K/V projection and borrows the target trunk's cache
  at two capture layers, so it never rides the arch registry or `forward_mtp`. The file holds the
  sidecar resolver, the refusing loader (`GemmaDrafter`, whose `blob` is raw GGUF Q8_0 = the Metal
  34B form, so the GPU encoder uploads it with no repack), and the CPU reference forward the GPU
  encoder is scored against. That oracle carries its own Q8_0 dot and GEMV and its own attention
  dot over decoded rows - reference arithmetic, not a tier kernel or a codec primitive, so no
  tier specializes it. A refusal is `ok = false` plus a `why`, never a panic - a drafter is
  optional and a bad sidecar must degrade to plain decode.
- **`dasllama_sampling.das`** - token sampling and the generation drivers: the sampler over
  `dasllama_common`'s `SamplingParams`. It registers the accept walk's row sampler
  (`register_mtp_sample_row`) at init, and that registration is the only way the engine reaches
  this file.
- **`dasllama_ple.das`** - gemma-4 E-series per-layer embeddings and the gemma4 MoE FFN. The
  forward sequence reaches it only through the hooks it registers at init. The Metal token-table
  gather (`dasllama_metal_prefill.das`) carries one format list in three places - the compiled
  `g_pf_pso_ple_gather*` pipelines, the `ple_gather_pso_of` pick and the `pf_enc_ple_gather_fmt`
  encode ladder - and `REVIEW.das`'s `check_ple_gather_sites` holds the three sets equal,
  licensing no names.
- **`dasllama_config.das`** - `DlimConfiguration`: every input that changes `.dlim` image BYTES,
  in one struct, plus its identity formatter. A knob that does not change image bytes does not
  belong here; a knob that does and is missing is an image-aliasing bug.
- **`dasllama_tools.das`** - the per-ToolMode tool wire codecs (definition serializers,
  replay/result text builders, reply parsers; Harmony namespace/channels, gemma-4 DSL, mistral
  control tokens, llama JSON). Every byte of tool wire text is produced here - pure string+JSON
  functions, model-free testable; the chat layer assembles the output into ChatParts and the
  server parses through the parsers.
- **`dasllama_chat.das`** - conversation turns and chat-template application. Per-arch template
  *content* is registered by the arch file (sec.1.6), not written here.
- **`dasllama_par.das`** - `maybe_parallel_for` plus the dispatch counters its arms call at RUN
  time, so the module AOTs, and the single-thread gate those arms read (`set_single_thread_`, `ARCHITECTURE_RUNTIME.md` sec.2.44a). Threading policy (job counts, thresholds) belongs to the caller.

### 1.6 Architecture registrations

Thirteen files registering eighteen names:
`dasllama_arch_llama.das` * `dasllama_arch_phi3.das` * `dasllama_arch_qwen2.das` * `dasllama_arch_qwen2moe.das` * `dasllama_arch_qwen3.das` * `dasllama_arch_qwen3moe.das` * `dasllama_arch_qwen35.das` * `dasllama_arch_gemma2.das` * `dasllama_arch_gemma3.das` * `dasllama_arch_gemma4.das` * `dasllama_arch_glm4moe.das` * `dasllama_arch_gptoss.das` * `dasllama_arch_mistral3.das`. They are DECLARATIVE: an arch
file builds an `ArchDesc` (name * `configure` * the `ArchBlocks` fn-ptr set - `attn_decode`,
`ffn_decode`, `attn_prefill`, `ffn_prefill`, and the optional `attn_batch` a non-standard graph
names * `ChatTemplate` * `LlmCaps`) and calls `register_arch` at `[init]`. Adding an arch touches no
forward loop.

### 1.8 Instrumentation and support

- **`dasllama_parity.das`** - CPU-reference caches for the parity instruments. Test-facing, but
  library-side because the caches outlive a single suite.
- **`dasllama_prefix.das`** - the prefix/page cache for evaluated token history.
- **`dasllama_fat_start.das`** - a fat exe's first start: the runtime-section snapshot the tuner's
  kernel half also writes, the Metal twin crown race (synthetic, no model), and the first-start
  hook it registers with the box-profile apply (`ARCHITECTURE_MEASUREMENT.md` sec.2.42a).
- **`dasllama_bench.das`** - the benchmark rows as one-rep steps over a `Model` and a `Session`:
  the pp warmup and timed prefill, the tg warmup and timed single-token forwards (whole-rep, or
  one token at a time for a driver that keeps its tick loop live), the warmup logit sanity check,
  the row-sized session under a caller's KV codec, the row statistic, and the batched row
  `bench_tg_batched_rep` - one timed rep of `npl` streams through the scheduler, device-home
  where a whole-model driver homes them and host-cached otherwise (a `Scheduler` over N sessions,
  prefilled untimed, `ngen` batched steps timed, the summed rate; a timed step the census counted
  per-row, or on the CPU batched stack under a device driver, refuses the row by name), so the
  file requires `dasllama_scheduler`, `dasllama_batch` and `dasllama_gpu_tier`. A sanctioned
  public entry point, like the exchange: the fourth door in `dasllama_lint`'s allowed table.
  `benchmarks/lcpp_bench.das` drives it from its loop; dasllama-server's in-process `/bench` runs
  one step per tick (`ARCHITECTURE_MEASUREMENT.md` sec.2.5).
- **`dasllama_lint.das`** - the facade boundary as a compile-time lint (DASLLAMA001): every engine
  module carries it, so a consumer requiring anything under `modules/dasLLAMA/` but the entry
  modules (facade, scheduler, exchange pair, bench) fails to compile. Escape:
  `options _dasllama_internal = true` - engine files, this module's own tests/harnesses/
  benchmarks/rigs, and the ruled consumers: `utils/dasllama-convert` (the bake tool reads the mint
  rail), `utils/dasllama-server/model_catalog.das` (the env rail + the model-set serving view; it
  re-exports `model_specs` so the server's catalog tests can gate the serve rows),
  `utils/dasllama-server/test_openai_server*.das` (env-registry test knobs),
  `modules/dasLLVM/daslib/llvm_user_modules.das` + `modules/dasLLVM/tests/test_{grid,tune,tuned}.das`
  (the tune-generator contract).
- **`performance/model_specs.das`** - the ONE model-set table: per carrier, the profiled-catalog
  fields, the official-board flag, the serving-catalog fields (`serve_*`, incl. `serve_vision`
  naming a tower companion by name), the provenance pin (exact HF repo + revision + sha256, or the
  on-box conversion recipe; companions hang off their owning entry, and a companion shared by
  several rows is referenced from the others by name), and the frozen parity evidence as data
  (pinned ids + arms). `llm_catalog` / `official_catalog` / `models_provenance` are views over it;
  `serve_vision_tower` resolves a tower name to its pinned companion, and `serve_asr_tower` is the
  one standalone pinned artifact the file carries - the serving ASR tower (parakeet v3), consumed
  by `asr_catalog`'s v3 row in `profile_common.das` (the pin's single source lives here, not there).
- **`performance/box_ident.das`** - the hardware-keyed box tag: the fleet table maps every
  recorded box's CPU brand to its store tag (the M3 row is an EXACT match so bigger M3s slug),
  the `DASLLAMA_BOX` override wins, and unknown hardware slugs from the brand - never another
  box's store. `profile_common.das`'s `box_name()` and `tests/run.das`'s manifest pick both resolve through it.
- **`performance/fetch_models.das`** - the fetch/verify driver over the provenance view.
  Verify by default, `--fetch` downloads; it never converts on `--fetch`, never benches, and
  it touches no tune state (`tune_policy(missing="fallback")`). BRINGUP.md sec.2 is the runbook.
- **`dasllama/dasllama_exchange_schema.das`** - engine-free validation for exchange submissions
  (record stores + tune sidecars); the dasllama.io ladder service builds on it.
- **`dasllama/dasllama_exchange.das`** - the sidecar-exchange client (a sanctioned public entry
  point, and the engine's only `dashv` requirer: `performance/REVIEW.das` walks `dasllama/` for a
  second one; a measurement harness under `harness/` talking HTTP to a reference engine's server
  sits outside that walk by design): the boot-time lookup/apply (llvm_tune's scope resolver - a
  verified per-box match downloads instead of a ~12-minute tune), the privacy-stripped submit
  rails, and the control-page surface dasllama-server serves at `/exchange`. The exchange is
  closed: both policies default to `off`, so no boot contacts it and no consent question is asked
  unless an explicit `exchange_*` key opts in - the shipped class profiles cover the boxes the
  exchange served. The first-contact consent gate (GDPR) sits ahead of every lookup once a policy
  is on: an explicit `exchange_*` config counts as the expressed choice, otherwise the
  `<stem>.consent` sidecar-sibling file governs - unset asks on a real terminal, or emits
  `@sidecar consent state=needed` for the control page, and no request leaves until a surface
  records "accepted". The client is meaningless without a sidecar, so every requirer takes it
  through the guard `require ?llvm dasllama/dasllama_exchange` (`llvm` is the C++ witness module
  dasLLVM compiles in exactly when the build is configured with it) and wraps each use in
  `static_if (typeinfo module_exists(dasllama_exchange))`: with no tune framework in the build the
  server's `/exchange` routes answer `not_found`, the boot resolver and submit check are never
  registered, and `lcpp_bench --submit` refuses before it measures.
- **`benchmarks/asr/mem_census.sh`** - the peak-memory census (`/usr/bin/time -l` around one
  asr_bench process per cell; macOS only) - the interim footprint instrument until a footprint
  leg lands in `gen_bench_records`; its numbers live in `PERF_LEDGER.md`, never the stores.

### 1.9 Serving {#scheduler-step}

- **`dasllama_scheduler.das`** - the continuous-batching scheduler, the serving layer over the
  facade (its one engine require is `dasllama/dasllama`). One synchronous thread: each
  `scheduler_step` admits queued requests, runs one `eval_batch` decode step over every
  decoding stream (a self-speculative scheduler instead ticks every stream's round through
  `mtp_spec_eval_batch`, one joint verify where a driver seats one, and counts the tick as a
  batched step only when every stream's rows rode it), then at most one prefill chunk FCFS; paged serving donates finished streams'
  KV pages to the prefix cache, device mode parks their regions. Results flow out as `SchedEvent`s - no HTTP here.
  `utils/dasllama-server` owns the writers; `tutorials/dasLLAMA/13_serving.das` is the
  teaching consumer; `tests/test_scheduler.das` gates it against `generate()` references.
  The step clears its gather arrays (`batch_rows`, `batch_toks`, `batch_idx`) before it reaps
  finished streams: `batch_rows` holds borrowed pointers into the sessions the reap deletes,
  and a validating heap collect between steps walks every pointer the array still holds.

