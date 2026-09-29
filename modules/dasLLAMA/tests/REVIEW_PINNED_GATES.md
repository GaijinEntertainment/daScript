# dasLLAMA tests - Pinned Gates Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
doc: `CLAUDE.md`. Planned work: `../followup_general.md`.

A cell is one `t |> run` subtest, or a `[test]` function that runs no subtest; a helper's
asserts belong to every cell that calls it. A pinned test cell is a cell the pinned set lists,
or any cell of a file it lists, and the pinned set is the only test of whether a cell is one.
The pinned set, one entry a line, each with what it must keep holding:

- `test_run_suites.das` must hold the per-PR split, the folder census, the area tables and the
  `--exclude` filter.
- `test_program_roots.das` must hold the `ROOT_DIRS` sweep, `options stack = 524288` and the
  prefill intent.
- `test_env_registry.das` must hold the `../ENVIRONMENT.md` knob contract.
- `test_model_specs.das` must hold `../performance/model_specs.das`'s model-set table.
- `test_metal_prefill_kernels.das`'s `test_metal_prefill_kernels` cell must hold its
  `attn_trio_gate` calls whose `AttnKeys` sets `softcap`, `hass`, `uend` or `ulo` - the softcap,
  sink, uniform-span and mixed-span arms.
- `test_site_records.das` must hold the byte-compare of `site/files/dasllama/bench_records.json`
  (repo root) against a fresh `merge_site_records` run.
- `test_exchange_schema.das` must hold the exchange validator's corpus sweeps and the
  `[tune_scope]` wire-key pin read out of `../dasllama/dasllama_tune_scope.das`.
- `test_bench_records_schema.das` must hold the `write_bench_records` output, corpus sweeps
  included; the llama-batched-bench table parse - which cell at which `npl`, 0 on every refusal;
  the batched reference row's command line - the engine's extra flags follow `-npl N -fa on`, and
  a run that prints no row names why and reads 0; and the committed stores' batched receipts - the
  cpu legs' das child at `--npl-plen 128 --npl-reps 3` and reference at `-npp 128`,
  `--no-op-offload` on the stock cpu arm alone, the metal leg at `-npp 512` with neither.
- `test_scheduler.das`'s `test_scheduler_media_splice` cell must hold that a media stream takes no
  cached hit at `prefix_attach` and donates no pages at `donate_stream`.
- `test_vulkan_kernels.das`'s `test_vk_coopmat_default_and_tile_pick` cell must hold which tile
  the Vulkan matmul picks, and whether that dispatch splits its reduction across partial planes,
  on every input of the prefill's tile-and-split pick.
- `test_vulkan_kernels.das`'s `test_vkd_ext_roster` cell must hold the device-init roster's
  entries against the arming's fields.
- `test_tts_pocket.das`'s `test_pocket_seat_stats` cell must hold `pocket_gpu_seats()`'s stage
  names in dispatch order, and `pocket_gpu_stats` panicking on a name it does not list.
- `test_tts_pocket.das`'s `test_pocket_q8_file` cell must hold the published file's tensor
  formats against the f16 file's load-time quants.
- `test_tts_pocket.das`'s `test_pocket_kq_file` cell must hold the small file's load-time tensor
  formats on each lane (unpinned: the backbone's and the codec transformers' matrices as Q4_K
  planes, the flow head as Q8_0 blocks, the speaker projection and the embedding table f32; pinned
  q8: Q8_0 blocks; pinned f32: f32 planes, the head included); the kq lane against the q8 lane of
  the same file under `Q8_FILE_BAR`; the kq lane against the exact lane under `KQ_EXACT_BAR`; the
  flow head on its Q8_0 route against the oracle's frames under `KQ_HEAD_BAR`; the stored roster
  voice against the reference encoder; and a clone over a roster name keeping the roster at 19
  voices and speaking.
- `test_tts_pocket.das`'s `test_pocket_quiet_floor` cell must hold the served lane's quiet floor
  against the f32 lane's.

**A diff that changes the contract a pinned test cell holds fixed - what its asserts hold, an
axis gained or lost - updates that cell's entry in the pinned set in the same change.** An axis
is one distinct behaviour the cell asserts - an output form, a refusal path, an argument's order;
a new input row on an axis the cell already asserts is not an axis gained.

**A diff that removes a pinned test cell's assert, loosens its bound, drops its input, or drops a
`run.das` suite listing that reaches it is a defect - keep the cell's asserts and fix the code
that fails them instead.** A diff that changes the value the cell's asserted function answers on
an input the cell keeps is not one of these.

**A diff that adds a cell or an assert whose expected value must be kept in step with something
maintained outside the cell - a document, a checked-in table, a committed artifact's form, a
roster, a knob list; not a value the cell's own claim defines, an engine constant it asserts
included - or that a checked-in table names as
its evidence, adds the cell carrying it to the pinned set in the same change** - as a named cell;
or, when the new cell leaves every cell of its file pinned, by replacing the file's named cells
with one file entry naming what it pins; or, when the file already has a file entry, by adding
each axis the new cell or assert asserts to that entry's must-hold list where it is missing. A new
entry in an existing cell's expectation list is not a new assert.
