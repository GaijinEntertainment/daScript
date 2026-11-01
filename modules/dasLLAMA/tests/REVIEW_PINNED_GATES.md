# dasLLAMA tests - Pinned Gates Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
doc: `../ARCHITECTURE.md`. Planned work: `../followup_general.md`.

A cell is one `t |> run` subtest, or a `[test]` function that runs no subtest, and owns the asserts
of every helper it calls. A pinned test cell is a cell the pinned set lists, or any cell of a file
the pinned set lists whole - a file entry, one naming the file and no cell - in the set before the
diff or after it, and the pinned set is the only test of whether a cell is one; a named-cell entry names a `[test]`
function and pins every cell that function runs. The pinned set, one entry a line, each with what it
must keep holding:

- `test_run_suites.das` must hold the per-PR split, the folder census, the area tables, the
  `--exclude` and `--only` filters, the `--shard` partition, the reach plan behind `--changed` -
  which test files each kind of changed path runs, every `MODULE_WITNESSES` row and
  `LLM_CORE_WITNESSES` naming only census files, and every witness row's module existing with a
  `MODULE_AREAS` row that holds `llm` -, the children's module-cache folder, and the
  `options persistent_heap` line in every test file `run.das`'s `stocked` suite lists.
- `test_program_roots.das` must hold the `ROOT_DIRS` sweep, `options stack = 524288` and the
  prefill intent.
- `test_env_registry.das` must hold the `../ENVIRONMENT.md` knob contract.
- `test_model_specs.das` must hold `../performance/model_specs.das`'s model-set table,
  `spec_by_file` answering by the models-dir path and by the gguf's base name (a split-shard row
  sits in a subfolder), the fixture compare (`prefix_match_len` / `fixture_ok`: a flipped last id
  red under the strict form and held under the half-span one, a flip inside the first half red
  under both), `fetch_models --convert`'s route (`convert_route`) and quantize mint steps
  (`mint_steps`), and the board fetch (`board_provenance`) holding every file the board sweeps read
  and no off-board row and no on-box parity oracle (`tts_oracle*`).
- `test_metal_float_a_gate.das` must hold the MSL emitter's refusal of a float A operand (the
  activation input) to `tmm2d_q8u_f32` in a kernel that lacks `[metal_kernel(float_a_ok=true)]`,
  the error naming the FLOAT A operand, and the same kernel compiling under that annotation.
- `test_rope.das` must hold `../dasllama/dasllama_rope.das`'s table builders (`build_rope_tabs`
  and its packed, rows, imrope, mrope and vision forms) against their per-element references and
  against each other bit for bit, `mrope_span_positions`' map and its refusal of a grid that does
  not match the span, `rope_factor_tensor`'s pick, and `../dasllama/dasllama_math.das`'s `rope`,
  at the bar each cell sets.
- `test_rope_apply.das` must hold `../dasllama/dasllama_math.das`'s rope application
  (`rope_scaled_*`, `rope_apply`) and `yarn_rope_freqs` against their fp64 references, each
  tabled form against its un-tabled twin, and `build_rope_tabs` over 120000 rows and over none,
  at the bar each cell sets.
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
    control (the compiler reorders the kernel's two multiplies, so a kernel-side swap reads the
    same bits);
  - the global scale on the softmax_weight picks;
  - `moe_comb_g4_gate` at the decode row and the 26B's row;
  - `ple_proj_bf16_gate` on the pair stamps and on the halfword stamps - an odd dim and an odd
    base among its arms - exact, with its poisoned lead element;
  - `f32_rows_gate` on the float4 router and on the element-walk stamps at a dim and a base off
    the float4 lattice;
  - `geglu_side_gate` over the position-major side rows, the contiguous read its control;
  - `ple_finish_gate` on the position-major side rows (`ple_fin_args`), at the E2B's slices
    (35 x 256), at a ple width past 1024 and at one off the 32-lane step, the finish without
    the prescale its control.
  - `b32_mm_split_gate` on every per-32 format's split-K tensor stamps (half and f32 activations, one
    and four 64-wide shares, the 16-row stamp) and `b32_mm_base_split_gate` on its simdgroup base
    split stamp, each through the partial-plane reduce and bit for bit against the CPU product.
- `test_metal_prefill_parity.das`'s `test_metal_prefill_kq_tail` cell must hold every stocked
  K-quant, grid and per-32 short window's served form - the row-batched GEMV peel past the whole
  tiles and the split stamps of a window under one tile, the 16-row split stamps on a window of 16
  rows or fewer - against the CPU control at its bar.
- `test_metal_prefill_kernels.das`'s `test_pf_bf16_tile_pick` cell must hold the window
  projection's tile pick: the bf16 tile only where layers x ple is a multiple of 64 and the dim a
  multiple of 32.
- `test_metal_misc_kernels.das`'s `test_metal_misc_kernels` cell must hold every kernel gate it
  calls, at the bar each gate sets; and by name: its token-table gathers bit for bit:
  `embed_q8_gate`, `embed_b32_gate` on q51, iq4nl32, q4032 and q5032 at 96 wide, at 2048 wide
  past a whole other table's blocks, and at a lead off a superblock boundary (the bind's block
  skip), `embed_kq_gate` on every K-quant format it lists, and `embed_f32_gate` past a lead of
  foreign floats; the residual seam's `add_rms_gate` on every stamp at every shipped width, 5376
  and 8192 past the slab among them; and `cls_select_gate` at a wide, a narrow and the served
  vocab: the ids at or above the row's K-th largest logit and their logits exact against the host
  oracle, the row's max logit, the overflow flag and the recent ids' logits, the denominator
  within 2e-5, a poisoned id and a poisoned denominator its controls.
- `test_metal_gemm_kernels.das`'s `test_metal_gemm_kernels` cell must hold every kernel gate it
  calls, at the bar each gate sets; and the short window's split-K stamps through the partial-plane reduce,
  NaN-poisoned outputs and planes their controls, each at its derived staging bar against the
  whole-K tile: `q8_split_gate`, `kq_split_gate` on every split K-quant format, `base_split_gate`
  on the q8 and K-quant base split stamps, `bf16_split_gate`, and the dev-W half tile's split
  (`q8_devw_split_leg`) through the reduce's strided mode, a column tile narrower than its row.
- `test_metal_gemv_kernels.das`'s `test_metal_kq_gemv_kernels` cell must hold every GEMV gate it
  calls, at the bar each gate sets - `kq_mvb_gate` and `moe_kq_gemv_gate` among them - and its
  `test_metal_kq_half_forms` cell every format's half-x route against the CPU reference at every
  row count the dispatcher serves.
- `test_site_records.das` must hold the byte-compare of `site/files/dasllama/bench_records.json`
  and of `site/files/dasllama/bench_cells.json` (repo root) - the smaller copy the site page loads
  first - against a fresh `merge_site_records` run, and `bench_cells.json` keeping every field
  that page renders at load while dropping the fields it reads from `bench_records.json` only on
  demand; and `test_board_projection`'s filters - an older release's row, a retired board's row,
  a held row and an unpaired reference kept off the board, and the stored audio outlier voided on
  the projected row.
- `test_exchange_schema.das` must hold the exchange validator's corpus sweeps and the
  `[tune_scope]` wire-key pin read out of `../dasllama/dasllama_tune_scope.das`.
- `test_bench_records_schema.das` must hold the `write_bench_records` output, corpus sweeps
  included; the llama-batched-bench table parse - which cell at which `npl`, 0 on every refusal;
  the batched reference row's command line - the engine's extra flags follow `-npl N -fa on`, and
  a run that prints no row names why and reads 0; the audio outlier rule and the recorder's void
  pass (`audio_outlier_clips`, `void_outlier_clips`); the `void_clips` and `held` annotations
  reaching only the run they select; and the committed stores' batched receipts - the cpu legs'
  das child at `--npl-plen 128 --npl-reps 3` and reference at `-npp 128`, `--no-op-offload` on
  the stock cpu arm alone, every GPU leg's reference at `-npp 512` with neither cpu-only flag.
- `test_scheduler.das`'s `test_scheduler_media_splice` cell must hold a media span's text and rows
  as one prefill call, and a body cut at the chunk taking the rest a chunk a call; on a paged
  scheduler, the prefix cache matching across a span by its rows' content - the text ahead of a new
  span attaching and the hit stopping at it, the media stream donating its pages, the same span
  asked again attaching past it, a rowless request for it served off the cache, other rows at the
  same place sharing only the text ahead of them - and a rowless request on a cold cache finishing
  `media_lost` with nothing donated.
- `test_vulkan_kernels.das`'s `test_vk_coopmat_default_and_tile_pick` cell must hold which tile
  the Vulkan matmul picks, and whether that dispatch splits its reduction across partial planes,
  on every input of the prefill's tile-and-split pick.
- `test_cuda_kernels.das`'s `test_cud_short_window_pick` cell must hold the tile and split that
  `cud_cm2_pick` returns for a window under 32 rows at two knee values. The knee is the value
  `set_cud_split_k_tgs` sets: how small the tile grid must be before the K splits. At knee 0 the cell
  must hold the s column (the 32-row tile column, `tc` 32) whole - one chunk, no split. At a knee that
  splits, it must hold K split into chunks whose depth is a whole multiple of 256: four 256-deep chunks
  at K 1024, and two 768-deep chunks at K 1536, where a four-way split would leave 384-deep chunks. It
  must also hold that a 32-row window gets the same pick at either knee.
- `test_gpu_tier.das`'s `test_home_words` cell must hold `rdec_fallback_words` (the text naming what
  serves a model the whole-model driver declined) in two cases. It must read as the per-op rails with
  no CUDA ask, and also under the CUDA ask on a build without dasCUDA. It must read as the CPU rails
  under `DASLLAMA_CUDA=1` on a dasCUDA build. The cell must also hold that each discrete home's dry
  bake (the lane's plan built without calling the device) refuses a config whose home is the other
  one, with a message naming the config's home.
- `test_vulkan_kernels.das`'s `test_vkd_ext_roster` cell must hold the device-init roster's
  entries against the arming's fields.
- `test_vulkan_kernels.das`'s `test_vkd_ple_finish` cell must hold the finish's 256-wide slices
  in place against the CPU finish a slice at a time, one norm over every slice its control.
- `test_vulkan_kernels.das`'s `test_vkd_ple_act_f32` cell must hold gelu(gate) x the side rows'
  layer slice, position-major rows apart, the contiguous read its control.
- `test_vulkan_kernels.das`'s `test_vkd_ple_bf16_cvt` cell must hold the bf16 rows as f16 bit
  for bit the widened value's half, saturated past the half range, the elements past the count
  kept.
- `test_vulkan_kernels.das`'s `test_vkd_dn_cap_tail` cell must hold the checkpoint capture's
  history write (`dn_cap_tail_cls`) bit for bit to the ring hand-off's CPU form for a window cut at
  the capture's row - two captures in one dispatch, one past the taps and one under them reading
  its first row off the entry image - the words between the two rows' images keeping their
  sentinel.
- `test_vulkan_kernels.das`'s `test_vkd_cls_select` cell must hold the selection kernel's landing
  against the host oracle on every row: the ids at or above the K-th largest logit with their
  logits, the max, the denominator at the row's temperature, the overflow flag past the slots and
  the recent ids' logits, at a wide and a narrow vocabulary, a poisoned denominator reddening the
  bar.
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
- `test_vulkan_kernels.das`'s `test_vkd_q51_cm2_batch`, `test_vkd_mx4_cm2_batch`,
  `test_vkd_iq4nl32_cm2_batch`, `test_vkd_iq3s4_cm2_batch`, `test_vkd_mx4_gemv_gu`,
  `test_vkd_gemv_ncol`, `test_vkd_q8_gemv_gu_n`, `test_vkd_q8_gemv_pleact`, `test_vkd_act_family`,
  `test_vkd_moe_routing`, `test_vkd_dn_family`, `test_vkd_hc_seams`, `test_vkd_hc_rows`,
  `test_vkd_ple_side`, `test_vkd_f16_gemm_khr`, `test_vkd_fa_cm2`, `test_vkd_fa_khr`,
  `test_vkd_fa_khr_h256_softcap`, `test_vkd_fa_khr_h256_gated`, `test_vkd_da_attn`, `test_vkd_da_attn_rqk`, `test_vkd_da_attn_bw`,
  `test_vkd_moe_gather_row`, `test_vkd_combine_row`, `test_vkd_kq_batch_family`,
  `test_vkd_emb_gather_family` and `test_vkd_nv4_cm2_batch` cells - the census's evidence for the
  classes no zoo row dispatches - must hold each class against its CPU oracle at the bar the cell
  sets, with the cell's poisoned-element control.
- `test_vulkan_tower_kernels.das`'s `test_vkt_fa_bidir`, `test_vkt_tower_rows`,
  `test_vkt_tower_ln_rows`, `test_vkt_tower_f16_feeds`, `test_vkt_tower_vision_rows`,
  `test_vkt_tower_win_rows`, `test_vkt_tower_tail_rows`, `test_vkt_tower_front_movers`,
  `test_vkt_tower_front_mel`, `test_vkt_tower_ga_mel`, `test_vkt_tower_cn_attn_rel_plane`,
  `test_vkt_tower_conformer`, `test_vkt_tower_fastconformer`, `test_vkt_tower_ga_dwh`, `test_vkt_tower_dw2d`,
  `test_vkt_wdec_classes`, `test_vkt_wdec_fused_rq`, `test_vkt_rnnt`, `test_vkt_ctc_pick`, `test_vkt_nmt` and
  `test_vkt_nmt_attn` cells - the census's evidence for the tower, transducer-decode, CTC-pick and translation classes -
  must hold each class against its CPU oracle at the bar the cell sets, with the cell's poisoned-element control.
- `test_vulkan_kv_codec_kernels.das`'s `test_vkc_q8_da_attn` and `test_vkc_tq4_da_attn` cells must
  hold the codec attention arms (`codec_attn_arms`) against the CPU attention over the codec's
  rows, each arm with its controls.
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
- `test_tts_pocket.das`'s `test_pocket_small_models` cell must hold, for each `SMALL` row's
  published file, the 19-voice English roster and the four character replacements its config
  carries, as the reference build's config lists them, and each seat serving, or declining with the
  `shape` reason, as that row's seat columns say.
- `test_whisper.das`'s `test_canary_qwen_oracle` cell, an external-reference compare, must hold
  the forced walk over `../harness/canary_qwen_expected.tsv`'s ids within `CN_TIE_BAR` of our
  argmax at every step of its three clips, on the CPU chain and on the Metal tower, its control the
  same walk fed each id one step early with the oracle id outside the bar at every step that input
  moved, and the greedy ids equal to the oracle's up to the first tie.
- `test_nmt.das`'s `test_nmt_spm` cell, an external-reference compare, must hold the model-file
  tokenizer id for id against `nmt_oracle/<dir>/`'s `tokens.json` source ids and `spm_edge.json` on
  every stocked student, its control the charsmap dropped spelling the normalizer fixture apart.
- `test_nmt.das`'s `test_nmt_exact_lane` cell, an external-reference compare, must hold every tapped
  stage of the exact lane under the oracle's own tokens within `STAGE_BAR` (1e-4 of the stage's
  largest magnitude) of the `.npy` dumps, the element counts equal, a displaced source token landing
  the last encoder layer outside the bar; and the free run at `EXACT_LINES_FLOOR` (28 of 30) lines
  of `tokens.json`'s ids, the lines fed the next line's source under it.
- `test_nmt.das`'s `test_nmt_q8_lane` cell, an external-reference compare, must hold the q8 lane at
  `Q8_LINES_FLOOR` (25 of 30) lines of `tokens.json`'s ids on the ru/en base-memory pairs, the same
  file with its classifier's Q8_0 copy zeroed moving every line and falling under the floor.

**A diff that changes the contract a pinned test cell holds fixed - what its entry says it must
keep holding, an axis gained or lost - updates that cell's entry in the pinned set in the same
change; a diff adding a cell to a file with a file entry adds to that entry each axis the cell
asserts that the entry does not name.**
An axis is one distinct behaviour the cell asserts - an output form, a refusal path, an argument's
order; a new input row is an axis gained only where it runs a kernel stamp, an output form or a
branch of the kernel or its router that no earlier row of the cell ran.

**A diff that widens a pinned test cell's tolerance, swaps its comparison for a looser one,
rewrites an expected value its entry ties to an outside source (a document, a checked-in table, a
knob list) to a value that source does not state, drops its input, or drops a `run.das` suite
listing that reaches it is a defect - fix the code that fails the cell instead.**

**A diff that removes a pinned test cell's assert while the code it checks stays adds to the same
cell, in the same change, an assert that holds the same values to an equal or tighter bound, with
its control (an extra assert in the same cell proving the compare can fail).**

**A diff that deletes the kernel or function a pinned test cell's assert checks and routes its
callers to other code - the replacement - adds, in the same change, an assert holding the
replacement's output to the deleted assert's bound or tighter, with its control, to a pinned cell
testing the replacement, or names in the old cell's pinned-set entry the pinned cell that already
asserts the replacement's output to that bound.**

**A diff that adds a cell or an assert whose expected value must be kept in step with something
maintained outside the cell - a value the cell restates as a literal or a copied condition, or
reads from an oracle dump, a truth cache or another expected-value file under `models_dir()` that
it does not build itself, never a value the cell's own claim defines - or
that a checked-in table (the model table in `../README.md`, or a list of kernel names whose
coverage note names a test cell as each one's evidence) names as its evidence, adds the cell
carrying it to the pinned set in the same change** - as a named cell, or, when the new cell leaves
every cell of its file pinned, by replacing the file's named cells with one file entry naming what
it pins. A new entry in an existing cell's expectation list is not a new assert, and an assert
comparing against a `../dasllama/` constant by name keeps nothing in step.
