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
  it registers with the box-profile apply, the per-user sidecar placement - the path, the
  one-time adoption of a sidecar beside the exe, and the placement hook the apply runs first
  (`ARCHITECTURE_MEASUREMENT_FAT_START.md#fat-first-start`) - and the per-box runtime file a run
  with no sidecar of its own reads: its path, its validity check, its race and its one writer
  (`ARCHITECTURE_MEASUREMENT_FAT_START.md#box-runtime-file`). The placement hook is optional, unlike a
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

### The test child {#test-child}

`tests/_child_root.das` is the one program a test under `tests/` starts as a child for a claim a
process decides - a knob the engine reads at context init, a line only a process's log carries, an
exit a panic decides. It dispatches a verb to a function of a `_child_<verb>.das` module, and it
starts no thread of its own: a thread is a second context, whose init would say every
once-a-process line again, so the spawning test's timer is the verb's wall (`run_verb_child`).
`tests/_child_run.das` owns its key, its build and its spawn. The runner (`tests/run.das`) builds
it once a run as a standalone exe, split by module, beside the module-cache record of the entry
program's compile - a record serves the root file it was written from, by path - under one key of
the runtime libraries, the build environment and the sources the build reads. A spawn then costs
a process start, where a root of its own costs an engine compile inside its test. A standalone exe
keeps its sidecar beside it, so the per-box file's hook stands down in the test child
(`ARCHITECTURE_MEASUREMENT_FAT_START.md#box-runtime-file`).

`tests/REVIEW.das`'s `check_one_test_child` reds a `_*_root.das` file, a `child_cmd` or
`child_cmd_at` call naming a root, and a `child_cmd_root` caller outside its licensed sets.
`OWN_ROOTS` licenses a program whose claim a verb cannot carry - the program's own compile, the
process it runs in, a model a stocked test loads, or a server or runner started by its command
line: `_child_root.das`, `_tuned_fallbacks_root.das`, `_exe_smoke_root.das`,
`_box_runtime_root.das`, `_audio_lane_whisper_root.das`,
`_mx4_mint_root.das`, `_fake_chat_root.das`, `run.das` and `dastest.das`. `ROOT_CMD_CALLERS`
licenses a test file that names its child's root itself: `test_f32_batch_tile_perms.das`, whose
root is a copy it writes.

### The model filters of a test run {#test-model-filters}

`tests/_model_categories.das` owns the two tables a run picks model files by. A category is a set
of files no run opens unless it names the category: a large model whose family's small one takes
the same paths, held for the cells only the large one reaches. A family is the set of decoder files
a `--family` run opens; a file's family is the row whose stem opens the most of its name.
`model_available` (`tests/_model_tier.das`) is the one gate that reads both, so a test names its
model file and nothing else. The runner hands `--category`, `--family` and `--arm` to a child as
dastest `--run-env` pairs, set once the child's program has compiled: a filter value is no part of
the compile record, and a run under another filter reads the same record. `TEST_CATEGORIES.md`
carries the categories for a reader, a table row a category under `## Categories`, and
`tests/REVIEW.das`'s `check_test_categories` holds the table and the document to each other.

### A model file loads once a form {#test-load-once}

`tests/_cpu_gpu_model.das` owns the load of a model file for the cells of a run area, on the
discrete homes and on the CPU. `with_cpu_and_gpu_model` takes the area, the file and a list of
forms - a device form is one set of the driver's load-time knobs, a CPU form one load mode and
loader - loads each form once, one loaded model at a time, and runs the caller's body on it; the
body collects what the compares across forms read, and those compares follow the call. Where the
GPU tier is not armed each device form is a skipped cell of its own. The CPU form answering the
device forms, and a `cpu_quant_form` reference, load only where a device form ran; a CPU form whose
cells stand alone (`cpu_cells_form`) loads on every box, so a CPU file's cells on a block keep
running on a box with no discrete home. A CPU form's load knobs - the load mode, the uncached
loader, the NextN head file pinned for the load (`mtp_head`), the kernel backend pinned for the load
and its cells (`backend`), the K-quant native switches set for the load (`kq_native`: `kept`, `on`,
`on_q40` with the native Q4_0 planes, `off`) - make CPU forms differing in any of them separate
loads, and a pinned or K-quant form answers no device form. The body sees a CPU load once a codec
the device forms armed, and `form.alone` holds on one pass a load, so a cell that stands alone runs
once a block. A device form marked `drops_device` holds the driver armed up to its one drop cell
(`device_drop_cell`), run last. The block accounts for every form it was asked: each form, and the parity row's
form where the knob is on, reaches the body or skips loudly, else the block fails naming it; a form asked alone that
never runs is a failure, and a CPU form not asked alone skips loudly where no device form ran (`form_owed`,
`alone_pass_of`). The forms, their pure parts and `own_load` are `tests/_model_form.das`'s,
a module free of the fixture root. The first device form streams the file, and where the
list holds three or more such forms each later one maps the file's device lane
(`ARCHITECTURE_IMAGE_LEDGER.md#suite-image-mints`). A call's `parity` knob adds the file's model_specs
parity row's CPU form (`parity_spec_form`, `tests/_parity_cells.das`) and runs the row's cell on it. A
call names its area and its file by string literals.

`tests/REVIEW.das`'s `check_models_load_once` reads the folder's test files as text. It holds one
call an area and file, and in each area `LOAD_ONCE_AREAS` names it reads every load site of the
area's test files (`run.das`'s `area_tests`): a call of an engine loader or of a helper `LOADERS`
lists, its model file a `.gguf` literal in the call's arguments or one an identifier is bound to in
the holding function, at the file's top level or in a fixture module. A site of a file a call loads,
or one of two sites of a file no call loads, is a finding unless the holding function calls
`own_load("<why>")` with a non-empty literal reason. A function that passes its own parameter to a
loader is itself a loader, and one `LOADERS` does not list is a finding; a site whose file is built at run time is
counted, not read. The llama2.c `stories15M.bin` checkpoint loads once a cell through `load_checkpoint`
(27 loads in 10 test files, under a second in all), outside the blocks: the check reads `.gguf`
loads alone, and a checkpoint load is not a site. `UNFOLDED` names the test files whose cells load
their own models; their findings print as held lines, and a listed file with no finding is a
finding. Its `check_cell_pins` reads the fixture cell modules (`tests/_*_cells.das`): a call there that pins,
clears or re-pins the kernel backend or the K-quant native knob is a finding, since both are a
property of the form's load; the check licenses none.

### The format repository's files {#format-matrix-files}

The format repository holds the files a test or a record reads byte for byte and no box makes for
itself: a carrier minted to put one weight format at one site (the format matrix), a cut of a large
model's first blocks (and its NextN draft block, renumbered after them, where the file has one)
that a per-PR test loads in the whole file's place, a published file kept beside them, and a drafter
a test fixture attaches, converted locally from its checkpoint. It is one Hugging Face
repository, a folder a model: `FORMATS_HF`
(`performance/model_specs.das`) pins its commit, and a row whose `url` opens with it names its file
there by `url`, `bytes` and `sha256`. Every box downloads such a file by its row: two runs of one
recipe are not promised the same bytes, so a box that made its own would read another file than the
row's checksum names. A `-local` row with no `url` is minted on each box by its `recipe`.

The decode pick tape: `ARCHITECTURE_ENGINE_MECHANISMS.md#moe-pick-tape`.
