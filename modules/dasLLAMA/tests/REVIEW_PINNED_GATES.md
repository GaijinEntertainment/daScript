# dasLLAMA tests - Pinned Gates Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
doc: `../ARCHITECTURE.md`. Planned work: `../followup_general.md`.

A cell is one `t |> run` subtest, or a `[test]` function that runs no subtest; a helper's
asserts belong to every cell that calls it. A pinned test cell is a cell the pinned set lists,
or any cell of a file the pinned set lists whole - a file entry, one naming the file and no cell -
and the pinned set is the only test of whether a cell is one; a named-cell entry names a `[test]`
function and pins every cell that function runs. The pinned set, one entry a line, each with what it
must keep holding:

- `test_run_suites.das` must hold the per-PR split, the folder census, the area tables and the
  `--exclude` filter.
- `test_program_roots.das` must hold the `ROOT_DIRS` sweep, `options stack = 524288` and the
  prefill intent.
- `test_env_registry.das` must hold the `../ENVIRONMENT.md` knob contract.
- `test_model_specs.das` must hold `../performance/model_specs.das`'s model-set table, and
  `spec_by_file` answering by the models-dir path and by the gguf's base name (a split-shard row
  sits in a subfolder).
- `test_metal_prefill_kernels.das`'s `test_metal_prefill_kernels` cell must hold every kernel gate
  it calls, at the bar each gate sets, with its `attn_trio_gate` calls whose `AttnKeys` sets
  `softcap`, `hass`, `uend` or `ulo`; and by name:
  - `qk_rope_gate`'s V-panel arms (`QKR_V_FK`, `QKR_V_NORM`), and the V panel untouched on every
    other arm;
  - `moe_select_gate`'s down-scale arms: the weights within `fapprox` of the host select times
    the down scales, and through `moe_select_fold_exact` bit for bit the plain twin's weight
    under the global scale times the expert's down scale - the kernel's fold order;
  - under the global scale, the swapped fold order (down scale, then global scale) parting from
    the kernel's order on the fixture and missing the device's weights - a reference-side
    control: the Metal compiler reorders the two multiplies, so a swap of them in the kernel's
    source reads the same bits on the device and this cell cannot see it, while a wrong fold
    value reds every weight;
  - the global scale on the softmax_weight picks;
  - `moe_comb_g4_gate` at the decode row and the 26B's row;
  - `ple_proj_bf16_gate` on both stamps, exact, with its poisoned lead element;
  - `geglu_side_gate` over the position-major side rows, the contiguous read its control;
  - `ple_finish_gate` on the position-major side rows (`ple_fin_args`), at the E2B's slices
    (35 x 256) too, the finish without the prescale its control.
  The PLE side-row gather and the gemma-4 down-scale fold left this cell: the gather is
  `test_metal_misc_kernels`' embed gates (entry below), the fold the select's down-scale arms.
- `test_metal_misc_kernels.das`'s `test_metal_misc_kernels` cell must hold its token-table
  gathers bit for bit: `embed_q8_gate`, `embed_b32_gate` on q51, iq4nl32, q4032 and q5032 at 96
  wide and at 2048 wide past a whole other table's blocks, `embed_kq_gate` on every K-quant
  format it lists, and `embed_f32_gate` past a lead of foreign floats.
- `test_metal_gemm_kernels.das`'s `test_metal_gemm_kernels` cell must hold every kernel gate it calls, at the bar
  each gate sets - the census's evidence among them: `kq_mulmm_gate`, `kq_devw_gate`, `kq_dbuf_gate`,
  `moe_mulmm_kq_gate`, `moe_mulmm_split_gate`, `attn_qk_mm_gate` and `attn_dev_pair_gate`.
- `test_metal_gemv_kernels.das`'s `test_metal_kq_gemv_kernels` cell must hold every GEMV gate it calls, at the bar
  each gate sets - `kq_mvb_gate` and `moe_kq_gemv_gate` among them - and its `test_metal_kq_half_forms` cell every
  format's half-x route against the CPU reference at every row count the dispatcher serves.
- `test_site_records.das` must hold the byte-compare of `site/files/dasllama/bench_records.json`
  and of `site/files/dasllama/bench_cells.json` (repo root) - the smaller copy the site page loads
  first - against a fresh `merge_site_records` run, and `bench_cells.json` keeping every field
  that page renders at load while dropping the fields it reads from `bench_records.json` only on
  demand.
- `test_exchange_schema.das` must hold the exchange validator's corpus sweeps and the
  `[tune_scope]` wire-key pin read out of `../dasllama/dasllama_tune_scope.das`.
- `test_bench_records_schema.das` must hold the `write_bench_records` output, corpus sweeps
  included; the llama-batched-bench table parse - which cell at which `npl`, 0 on every refusal;
  the batched reference row's command line - the engine's extra flags follow `-npl N -fa on`, and
  a run that prints no row names why and reads 0; and the committed stores' batched receipts - the
  cpu legs' das child at `--npl-plen 128 --npl-reps 3` and reference at `-npp 128`,
  `--no-op-offload` on the stock cpu arm alone, the metal leg at `-npp 512` with neither.
- `test_scheduler.das`'s `test_scheduler_media_splice` cell must hold a media span's text and rows
  as one prefill call, and a body cut at the chunk taking the rest a chunk a call; on a paged
  scheduler, the prefix cache matching across a span by its rows' content - the text ahead of a new
  span attaching and the hit stopping at it, the media stream donating its pages, the same span
  asked again attaching past it, a rowless request for it served off the cache, other rows at the same place sharing only
  the text ahead of them - and a rowless request on a cold cache finishing `media_lost` with
  nothing donated.
- `test_vulkan_kernels.das`'s `test_vk_coopmat_default_and_tile_pick` cell must hold which tile
  the Vulkan matmul picks, and whether that dispatch splits its reduction across partial planes,
  on every input of the prefill's tile-and-split pick.
- `test_vulkan_kernels.das`'s `test_vkd_ext_roster` cell must hold the device-init roster's
  entries against the arming's fields.
- `test_vulkan_kernels.das`'s `test_vkd_ple_finish` cell must hold the finish's 256-wide slices
  in place against the CPU finish a slice at a time, one norm over every slice its control.
- `test_vulkan_kernels.das`'s `test_vkd_ple_act_f32` cell must hold gelu(gate) x the side rows'
  layer slice, position-major rows apart, the contiguous read its control.
- `test_vulkan_kernels.das`'s `test_vkd_ple_bf16_cvt` cell must hold the bf16 rows as f16 bit
  for bit the widened value's half, saturated past the half range, the elements past the count
  kept.
- `test_vulkan_kernels.das`'s `test_vkd_router_gemv_bf16` cell must hold the bf16 router-form
  rows against the CPU dot at a row base and an output base, the two-base form and the
  norm-folded form.
- `test_vulkan_kernels.das`'s `test_vkd_router_gemv_cols` cell must hold the router GEMV over x
  rows as columns against the one-row dispatch bit for bit and the CPU dot, the bf16 rows'
  halfword stamp at an odd dim among its arms.
- `test_vulkan_kernels.das`'s `test_vkd_f32_khr_batch` cell must hold the staged KHR tile over
  f32 rows against the CPU oracle with each weight rounded to f16, whole and in two k chunks
  through the reduce.
- `test_vulkan_kernels.das`'s `test_vkd_kq_gemv_k4_gu` cell must hold the fused gate + up + act
  + requant on Q4_K, IQ4_XS, iq3s4 and IQ2_S against the three-kernel path byte for byte and the
  CPU chain.
- `test_vulkan_kernels.das`'s `test_vkd_q51_gemv`, `test_vkd_iq4nl32_gemv`,
  `test_vkd_q4032_gemv`, `test_vkd_q5032_gemv` and `test_vkd_mx4_gemv` cells must hold the
  format's GEMV against its scalar dot, and its summed twin (`per32_sum_rows_cell`) over one,
  two and four token rows - every row its slots' dots under their weights, a poisoned last row
  the control, a wider dispatch's rows bit for bit the narrower one's.
- `test_vulkan_kernels.das`'s `test_vkd_q51_cm2_batch`, `test_vkd_mx4_cm2_batch`, `test_vkd_iq4nl32_cm2_batch`,
  `test_vkd_iq3s4_cm2_batch`, `test_vkd_mx4_gemv_gu`, `test_vkd_gemv_ncol`, `test_vkd_q8_gemv_gu_n`,
  `test_vkd_q8_gemv_pleact`, `test_vkd_act_family`, `test_vkd_moe_routing`, `test_vkd_dn_family`, `test_vkd_hc_seams`,
  `test_vkd_hc_rows`, `test_vkd_ple_side`, `test_vkd_f16_gemm_khr`, `test_vkd_fa_cm2`, `test_vkd_fa_khr`,
  `test_vkd_fa_khr_h256_softcap`, `test_vkd_da_attn`, `test_vkd_da_attn_rqk` and `test_vkd_da_attn_bw` cells - the
  census's evidence for the classes no zoo row dispatches - must hold each class against its CPU oracle at the bar the
  cell sets, with the cell's poisoned-element control.
- `test_vulkan_tower_kernels.das`'s `test_vkt_fa_bidir`, `test_vkt_tower_rows`, `test_vkt_tower_ln_rows`,
  `test_vkt_tower_f16_feeds`, `test_vkt_tower_vision_rows`, `test_vkt_tower_win_rows`, `test_vkt_tower_tail_rows`,
  `test_vkt_tower_front_movers`, `test_vkt_tower_front_mel`, `test_vkt_tower_cn_attn_rel_plane`,
  `test_vkt_tower_conformer`, `test_vkt_tower_fastconformer`, `test_vkt_tower_dw2d`, `test_vkt_wdec_classes` and
  `test_vkt_wdec_fused_rq` cells - the census's evidence for the tower classes - must hold each class against its
  CPU oracle at the bar the cell sets, with the cell's poisoned-element control.
- `test_vulkan_kv_codec_kernels.das`'s `test_vkc_q8_da_attn` and `test_vkc_tq4_da_attn` cells must hold the codec
  attention arms (`codec_attn_arms`) against the CPU attention over the codec's rows, each arm with its controls.
- `test_gpu_tier.das`'s `test_override_announces` cell must hold every override line the GPU
  homes announce - its setter and env spellings - in step with the knob list
  (`../ENVIRONMENT.md`), and no line where the override takes no effect.
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
axis gained or lost - updates that cell's entry in the pinned set in the same change; a diff
adding a cell to a file with a file entry adds to that entry each axis the cell asserts that the
entry does not name.**
An axis is one distinct behaviour the cell asserts - an output form, a refusal path, an argument's
order; a new input row on an axis the cell already asserts is not an axis gained.

**A diff that widens a pinned test cell's tolerance, swaps its comparison for one more outputs
pass, rewrites an expected value without updating that cell's pinned-set entry in the same change,
drops its input, or drops a `run.das` suite listing that reaches it is a defect - fix the code
that fails the cell instead.**

**A diff that removes a pinned test cell's assert while the code it checks stays adds to the same
cell, in the same change, an assert that holds the same values to an equal or tighter bound, with
its control (an extra assert in the same cell proving the compare can fail).**

**A diff that deletes the kernel or function a pinned test cell's assert checks and routes its
callers to other code - the replacement - adds, in the same change, an assert holding the
replacement's output to the deleted assert's bound or tighter, with its control, to a pinned cell
testing the replacement, or names in the old cell's pinned-set entry the pinned cell that already
asserts the replacement's output to that bound.**

**A diff that adds a cell or an assert whose expected value must be kept in step with something
maintained outside the cell - a document, a checked-in table, a committed artifact's form, a
roster, a knob list; not a value the cell's own claim defines - or that a checked-in table names
as its evidence, adds the cell carrying it to the pinned set in the same change** - as a named
cell, or, when the new cell leaves every cell of its file pinned, by replacing the file's named
cells with one file entry naming what it pins. A new entry in an existing cell's expectation list
is not a new assert.
