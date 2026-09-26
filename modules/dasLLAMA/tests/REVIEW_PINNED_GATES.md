# dasLLAMA tests - Pinned Gates Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
doc: `CLAUDE.md`. Planned work: `../followup_general.md`.

**A diff that changes the contract a pinned test cell holds fixed - what its asserts hold, an
axis gained or lost - updates that cell's entry in the pinned set in the same change.** An axis
is one distinct behaviour the cell asserts - an output form, a refusal path, an argument's order;
a new input row on an axis the cell already asserts is not an axis gained.

**A diff that removes a pinned test cell's assert, loosens its bound, drops its input, or drops a
run that reaches it (the per-PR `stocked` run is one) is a defect; a diff that changes the value
the cell's asserted function answers on an input the cell keeps is not one of these.**

**A diff that adds a cell or an assert whose expected value must be kept in step with something
maintained outside the cell - a document, a checked-in table, a committed artifact's form, a
roster, a knob list; not a value the cell's own claim defines - or that a checked-in table names as
its evidence, adds the cell carrying it to the pinned set in the same change** - as a named cell;
or, when every cell of its file is pinned, as that file's entry naming what it pins; or, when the
file already has an entry, by adding each axis the new cell or assert asserts to that entry's
parenthetical where it is missing.

**The pinned set - a cell listed here, or every cell of a listed file, is a pinned test cell,
and this list is the only test of whether a cell is one:**
`test_run_suites.das` (the per-PR split, the folder census, the area tables, the `--exclude`
filter); `test_program_roots.das` (the `ROOT_DIRS` sweep, `options stack = 524288`, prefill
intent); `test_env_registry.das` (the `../ENVIRONMENT.md` knob contract); `test_model_specs.das`
(`../performance/model_specs.das`'s model-set table); `test_metal_prefill_kernels.das`'s
`test_metal_prefill_kernels` cell (its `attn_trio_gate` calls whose `AttnKeys` sets `softcap`,
`hass`, `uend` or `ulo` - the softcap, sink, uniform-span and mixed-span arms); `test_site_records.das` (the byte-compare of
`site/files/dasllama/bench_records.json` (repo root) against a fresh `merge_site_records`
run); `test_exchange_schema.das` (the exchange validator's corpus sweeps, and the `[tune_scope]`
wire-key pin read out of `../dasllama/dasllama_tune_scope.das`) and
`test_bench_records_schema.das` (the `write_bench_records` output, corpus sweeps included, the
llama-batched-bench table parse - which cell at which `npl`, 0 on every refusal - and the
batched reference row's command line: the engine's extra flags follow `-npl N -fa on`, and a run
that prints no row names why and reads 0 - and the committed stores' batched receipts: the cpu
legs' das child at `--npl-plen 128 --npl-reps 3` and reference at `-npp 128`, `--no-op-offload`
on the stock cpu arm alone, the metal leg at `-npp 512` with neither);
`test_scheduler.das`'s `test_scheduler_media_splice` cell (a media stream takes no cached hit at
`prefix_attach` and donates no pages at `donate_stream`); `test_vulkan_kernels.das`'s `test_vk_coopmat_default_and_tile_pick`
(which tile the Vulkan matmul picks and whether that dispatch splits its reduction across
partial planes, on every input of the prefill's tile-and-split pick) and its `test_vkd_ext_roster` cell (the device-init roster's entries
against the arming's fields); `test_tts_pocket.das`'s `test_pocket_seat_stats` (`pocket_gpu_seats()`'s stage names in dispatch order, and `pocket_gpu_stats` panicking on a name it does not list), `test_pocket_q8_file`,
`test_pocket_kq_file` and `test_pocket_quiet_floor` cells (the published file's tensor formats
against the f16 load-time quants, the kq lane against the q8 lane, the served lane's quiet floor
against the f32 lane's); `utils/dasllama-server/test_worker_dispatch.das` (repo root) -
worker-local fork pools, shared queue policy.
