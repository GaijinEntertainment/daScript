# dasLLAMA tests - Pinned Gates Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
doc: `CLAUDE.md`. Planned work: `../followup_general.md`.

**Routed from `REVIEW.md` (beside this file): a diff that checklist routes here applies this
list together with `REVIEW.md`'s.** A pinned test cell is one whose expected value must be kept
in step with something maintained outside the cell - a document, a checked-in table, a committed
artifact's form, a roster, a knob list - wherever the value is written; a value the cell's own
claim defines is not pinned.

**A diff that changes the contract a pinned test cell holds fixed - what its asserts hold, an
axis gained or lost - updates that cell's entry in the pinned set below in the same change.**

**A pinned test cell's coverage never shrinks: a diff that removes one of its asserts, loosens
one of its bounds, or drops an input or a run that reaches it is a defect.** Changing what a pinned
predicate answers on an input the cell already asserts on is not a shrink; dropping the input is.
A pinned file that reaches a fixture root sits in `stocked`, where the per-PR run reaches it;
that is not a shrink. The pinned set, each with what it pins:
`test_run_suites.das` (the per-PR split, the folder census, the area tables, the `--exclude`
filter); `test_program_roots.das` (the `ROOT_DIRS` sweep, `options stack = 524288`, prefill
intent); `test_env_registry.das` (the `../ENVIRONMENT.md` knob contract); `test_model_specs.das`
(`../performance/model_specs.das`'s model-set table); `test_metal_prefill_kernels.das`'s
softcap, sink (`hass`) and span cells; `test_site_records.das` (the byte-compare of
`site/files/dasllama/bench_records.json` (repo root) against a fresh `merge_site_records`
run); `test_exchange_schema.das` (the exchange validator's corpus sweeps, and the `[tune_scope]`
wire-key pin read out of `../dasllama/dasllama_tune_scope.das`) and
`test_bench_records_schema.das` (the `write_bench_records` output, corpus sweeps included, the
llama-batched-bench table parse - which cell at which `npl`, 0 on every refusal);
`test_scheduler.das`'s media-stream bypass check (no cached hit at `prefix_attach`, no donated
pages at `donate_stream`); `test_vulkan_kernels.das`'s `test_vk_coopmat_default_and_tile_pick`
(which tile the Vulkan matmul picks and whether that dispatch splits its reduction across
partial planes, on every input of the prefill's tile-and-split pick; added rows on those inputs
are not an axis gained) and its `test_vkd_ext_roster` cell (the device-init roster's entries
against the arming's fields); `test_tts_pocket.das`'s `test_pocket_q8_file`,
`test_pocket_kq_file` and `test_pocket_quiet_floor` cells (the published file's tensor formats
against the f16 load-time quants, the kq lane against the q8 lane, the served lane's quiet floor
against the f32 lane's); `utils/dasllama-server/test_worker_dispatch.das` (repo root) -
worker-local fork pools, shared queue policy.

**A diff that adds a pinned test cell adds it to the pinned set above in the same change** - as
a file when every cell of it pins, as a named cell otherwise.
