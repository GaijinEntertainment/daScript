# dasLLAMA Architecture - instrumentation and support charters

Companion to `ARCHITECTURE.md` beside `ARCHITECTURE_ENGINE.md`; a section is cited by its
anchor.

### Instrumentation and support {#instrumentation-and-support}

- **`dasllama_parity.das`** - CPU-reference caches for the parity instruments. Test-facing, but
  library-side because the caches outlive a single suite.
- **`dasllama_prefix.das`** - the prefix/page cache for evaluated token history.
- **`dasllama_env.das`** - the `[EnvConfig]` knob declarations, loaded once into the `g_env_*`
  globals, and the paths derived from them: the per-user `~/.dasllama` directory and the config
  lookup the server, its CLI and the exchange share.
- **`dasllama_fat_start.das`** - a fat exe's first start: the runtime-section snapshot the tuner's
  kernel half also writes, the Metal twin crown race and the three homes' short-window knee races
  (`dasllama_metal_knee_race`, `dasllama_vulkan_knee_race`, `dasllama_cuda_knee_race`; synthetic, no model), the first-start hook
  it registers with the box-profile apply, and the per-user sidecar placement - the path, the
  one-time adoption of a sidecar beside the exe, and the placement hook the apply runs first
  (`ARCHITECTURE_MEASUREMENT_FAT_START.md#fat-first-start`). The placement hook is optional, unlike a
  hook that does the work itself: unset, the apply reads the sidecar where the tune framework put
  it, which is what every build but a fat exe wants.
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
  one step per tick (`ARCHITECTURE_MEASUREMENT.md#one-benchmark-rig`).
- **`dasllama_lint.das`** - the facade boundary as a compile-time lint (DASLLAMA001): every engine
  module carries it, so a consumer requiring anything under `modules/dasLLAMA/` but the entry
  modules (facade, scheduler, exchange pair, bench, the `performance/box_ident.das` box probes)
  fails to compile. Escape:
  `options _dasllama_internal = true` - engine files, this module's own tests/harnesses/
  benchmarks/rigs, and the ruled consumers: `utils/dasllama-convert` (the bake tool reads the mint
  rail), `utils/dasllama-server/model_catalog.das` (the env rail + the model-set serving view; it
  re-exports `model_specs` so the server's catalog tests can gate the serve rows),
  `utils/dasllama-server/test_openai_server*.das` (env-registry test knobs),
  `utils/dasllama-server/user_state.das` (the env rail: the per-user directory and config lookup),
  `modules/dasLLVM/daslib/llvm_user_modules.das` + `modules/dasLLVM/tests/test_{grid,tune,tuned}.das`
  (the tune-generator contract).
- **`performance/model_specs.das`** - the ONE model-set table: per carrier, the profiled-catalog
  fields, the official-board flag, the serving-catalog fields (`serve_*`, incl. `serve_vision`
  and `serve_drafter` naming a tower or drafter companion by name), the provenance pin (exact HF repo + revision + sha256, or the
  on-box conversion recipe; companions hang off their owning entry, and a companion shared by
  several rows is referenced from the others by name), and the frozen parity evidence as data
  (pinned ids + arms). `llm_catalog` / `official_catalog` / `models_provenance` are views over it;
  `serve_companion` resolves a tower or drafter name to its pinned companion, and `serve_asr_tower` is the
  one standalone pinned artifact the file carries - the serving ASR tower (parakeet v3), consumed
  by `asr_catalog`'s v3 row in `profile_common.das` (the pin's single source lives here, not there).
- **`performance/box_ident.das`** - the hardware-keyed box tag: the fleet table maps every
  recorded box's CPU brand to its store tag (the M3 row is an EXACT match so bigger M3s slug),
  the `DASLLAMA_BOX` override wins, and unknown hardware slugs from the brand - never another
  box's store. `profile_common.das`'s `box_name()` and `tests/run.das`'s manifest pick both resolve through it.
  It is also the one home of the engine-free box probes the bench's hardware identity and the
  server's hardware line and checkpoint budget share: the CPU brand, the RAM (a cgroup limit capping
  it on Linux), the WMI query `wmi_lines` (wmic where the box ships it, Get-CimInstance where a clean
  Windows 11 24H2+ install dropped it) and the argv capture `run_capture_argv` every probe and
  reference exe spawns through (no shell: a missing program is an empty answer, never a console
  line). A sanctioned public entry point, required by path, like the exchange: the fifth door in
  `dasllama_lint`'s allowed table, and the SDK bundle installs it beside `model_specs.das`.
- **`performance/fetch_models.das`** - the fetch/verify driver over the provenance view.
  Verify by default, `--fetch` downloads; it never converts on `--fetch`, never benches, and
  it touches no tune state (`tune_policy(missing="fallback")`). BRINGUP.md sec.2 is the runbook.
- **`dasllama/dasllama_exchange_schema.das`** - engine-free validation for exchange submissions
  (record stores + tune sidecars) and the board policy the recorder, the site projection and the
  ladder share (`RETIRED_BOXES`, the audio outlier rule and its clip rates, the run-key matcher);
  the dasllama.io ladder service builds on it.
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

`REVIEW.das`'s `check_harness_dashv` walks `harness/` and reds a tool that requires `dashv` and
names `dasllama.io`: the exchange module is the sidecar exchange host's one client, and a harness
tool reaches it through `dasllama/dasllama_exchange`. It licenses no names.

The decode pick tape: `ARCHITECTURE_ENGINE_MECHANISMS.md#moe-pick-tape`.
