# dasLLAMA tests - Pinned Kernel Gates Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
docs: `../ARCHITECTURE.md`, `../ARCHITECTURE_CELL_BARS.md`. Planned work: `../followup_general.md`.

**A diff that touches a cell an entry below pins, or any cell of a file an entry below pins whole, applies
`REVIEW_PINNED_GATES.md` (beside this file), whose rules bind every entry here.**

Terms this checklist does not define are as `REVIEW_PINNED_GATES.md` and `REVIEW.md` (both beside this file) define
them; a whole run is as `../REVIEW_GPU_KERNEL_CLASSES.md` defines it. The pinned set's entries for the
`test_*_kernels*.das` files, one entry a bullet, each with what it must keep holding:

- `test_metal_prefill_kernels.das`'s `test_metal_prefill_kernels` cell must hold every kernel gate it calls, at the
  bar each gate sets, with its `attn_trio_gate` calls whose `AttnKeys` sets `softcap`, `hass`, `uend` or `ulo`; and by
  name:
  - `qk_rope_gate`'s V-panel arms (`QKR_V_FK`, `QKR_V_NORM`), and the V panel untouched on every other arm;
  - `moe_select_gate`'s down-scale arms: the weights within `fapprox` of the host select times the down scales, and
    through `moe_select_fold_exact` bit for bit the plain twin's weight under the global scale times the expert's down
    scale - the kernel's fold order;
  - under the global scale, the swapped fold order (down scale, then global scale) parting from the kernel's order on
    the fixture and missing the device's weights - a reference-side control (the compiler reorders the kernel's two
    multiplies, so a kernel-side swap reads the same bits);
  - the global scale on the softmax_weight picks;
  - `moe_comb_g4_gate` at the decode row and the 26B's row;
  - `ple_proj_bf16_gate` on the pair stamps and on the halfword stamps - an odd dim and an odd base among its arms -
    exact, with its poisoned lead element;
  - `f32_rows_gate` on the float4 router and on the element-walk stamps at a dim and a base off the float4 lattice;
  - `geglu_side_gate` over the position-major side rows, the contiguous read its control;
  - `ple_finish_gate` on the position-major side rows (`ple_fin_args`), at the E2B's slices (35 x 256), at a ple width
    past 1024 and at one off the 32-lane step, the finish without the prescale its control.
  - `b32_mm_split_gate` on every per-32 format's split-K tensor stamps (half and f32 activations, one and four 64-wide
    shares, the 16-row stamp) and `b32_mm_base_split_gate` on its simdgroup base split stamp, each through the
    partial-plane reduce and bit for bit against the CPU product.
  - the affine members' rows (mlx4, mlx8, mlx4b, mlx8b, mlx6b) on the fixture the engine's own block dequant reads
    back bit for bit: `moe_gemv_gate` (the shared-x and the per-slot forms), `b32_gemv_gate`, `b32_mm_gate` and
    `moe_mulmm_b32_gate` (the routed base, the split twins and the tall pair) exact against the CPU product,
    `moe_b32_rows_gate` and `b32_mvb_gate` (two-row and K-split) bit for bit the one-stream / one-row dispatches, and
    `b32_dense_twins`; and `moe_gemv_gate`'s wide rows exact on every `B32_MOE_WIDE_FMTS` member (shared-x, per-slot
    with an odd row past the last pair, a stride and a tail under batch streams).
- `test_metal_prefill_kernels.das`'s `test_pf_bf16_tile_pick` cell must hold the window projection's tile pick: the
  bf16 tile only where layers x ple is a multiple of 64 and the dim a multiple of 32.
- `test_metal_misc_kernels.das`'s `test_metal_misc_kernels` cell must hold every kernel gate it calls, at the bar each
  gate sets; and by name: its token-table gathers bit for bit: `embed_q8_gate`, `embed_b32_gate` on q51, iq4nl32,
  q4032 and q5032 at 96 wide, at 2048 wide past a whole other table's blocks, and at a lead off a superblock boundary
  (the bind's block skip), and on the affine members at whole 64-groups (128 and 2048 wide, a lead six blocks past the
  bind), `embed_kq_gate` on every K-quant format it lists, and `embed_f32_gate` past a lead of foreign floats; the
  residual seam's `add_rms_gate` on every stamp at every shipped width, 5376 and 8192 past the slab among them; and
  `cls_select_gate` at a wide, a narrow and the served vocab: the ids at or above the row's K-th largest logit and
  their logits exact against the host oracle, the row's max logit, the overflow flag and the recent ids' logits, the
  denominator within 2e-5, a poisoned id and a poisoned denominator its controls.
- `test_metal_decode_kernels.das`'s `test_metal_attn_codec_kernels` cell must hold, by name, `sq_tile_gate` - the
  census's evidence for the tile attention's two stamps: each stamp with the shipped combine against the CPU attention
  at the bar the gate sets, over a row under 64 keys, rows ending on a step and off one, a lead, a row past 8192 keys
  (two steps a piece), and on the packed stamp a gated q and a chunk with idle slots.
- `test_metal_gemm_kernels.das`'s `test_metal_gemm_kernels` cell must hold every kernel gate it calls, at the bar each
  gate sets; and the short window's split-K stamps through the partial-plane reduce, NaN-poisoned outputs and planes
  their controls, each at its derived staging bar against the whole-K tile: `q8_split_gate`, `kq_split_gate` on every
  split K-quant format, `base_split_gate` on the q8 and K-quant base split stamps, `bf16_split_gate`, and the dev-W
  half tile's split (`q8_devw_split_leg`) through the reduce's strided mode, a column tile narrower than its row.
- `test_metal_tts_kernels.das` must hold every case its cells call, at the bar each case sets and with that case's
  control; and by name: the causal cached attention row's decode step over 300 keys, past one 256-key stride of its
  staged scores; the dense tile reading zero past a layer's inputs; the conv tile's float and half stamps, picked by
  the site's half flag; the profile ledger's shot - a command buffer a mark, each marked role charged device time,
  its output bit for bit the unprofiled shot's, nothing reported with the pin off, the last report dropped at a
  model's discharge; and the home's count of dense dispatches over widened rows on the hand-built frame model - the
  prompt's rows GEMM once a layer and the frame loop's fused projection once a layer a frame over a projection of
  halves, the row GEMV once a frame over an input linear of halves, none over the model of floats.
- `test_metal_gemv_kernels.das`'s `test_metal_kq_gemv_kernels` cell must hold every GEMV gate it calls, at the bar
  each gate sets - `kq_mvb_gate` and `moe_kq_gemv_gate` among them - and its `test_metal_kq_half_forms` cell every
  format's half-x route against the CPU reference at every row count the dispatcher serves.
- `test_vulkan_kernels_gemm.das`'s `test_vk_coopmat_default_and_tile_pick` cell must hold which tile the Vulkan matmul
  picks, and whether that dispatch splits its reduction across partial planes, on every input of the prefill's
  tile-and-split pick.
- `test_cuda_kernels.das`'s `test_cud_short_window_pick` cell must hold the tile and split that `cud_cm2_pick` returns
  for a window under 32 rows at two knee values. The knee is the value `set_cud_split_k_tgs` sets: how small the tile
  grid must be before the K splits. At knee 0 the cell must hold the s column (the 32-row tile column, `tc` 32) whole -
  one chunk, no split. At a knee that splits, it must hold K split into chunks whose depth is a whole multiple of 256:
  four 256-deep chunks at K 1024, and two 768-deep chunks at K 1536, where a four-way split would leave 384-deep
  chunks. It must also hold that a 32-row window gets the same pick at either knee.
- `test_vulkan_kernels.das`'s `test_vkd_ext_roster` cell must hold the device-init roster's entries against the
  arming's fields.
- `test_vulkan_kernels.das`'s `test_vkd_ple_finish` cell must hold the finish's 256-wide slices in place against the
  CPU finish a slice at a time, one norm over every slice its control.
- `test_vulkan_kernels.das`'s `test_vkd_ple_act_f32` cell must hold gelu(gate) x the side rows' layer slice,
  position-major rows apart, the contiguous read its control.
- `test_vulkan_kernels.das`'s `test_vkd_ple_bf16_cvt` cell must hold the bf16 rows as f16 bit for bit the widened
  value's half, saturated past the half range, the elements past the count kept.
- `test_vulkan_kernels.das`'s `test_vkd_dn_cap_tail` cell must hold the checkpoint capture's history write
  (`dn_cap_tail_cls`) bit for bit to the ring hand-off's CPU form for a window cut at the capture's row - two captures
  in one dispatch, one past the taps and one under them reading its first row off the entry image - the words between
  the two rows' images keeping their sentinel.
- `test_vulkan_kernels.das`'s `test_vkd_cls_select` cell must hold the selection kernel's landing against the host
  oracle on every row: the ids at or above the K-th largest logit with their logits, the max, the denominator at the
  row's temperature, the overflow flag past the slots and the recent ids' logits, at a wide and a narrow vocabulary, a
  poisoned denominator reddening the bar.
- `test_vulkan_kernels.das`'s `test_vkd_router_gemv_bf16` cell must hold the bf16 router-form rows against the CPU dot
  at a row base and an output base, the two-base form and the norm-folded form.
- `test_vulkan_kernels.das`'s `test_vkd_router_gemv_cols` cell must hold the router GEMV over x rows as columns
  against the one-row dispatch bit for bit - at the approx bar where `vk_rounds_alike` is false, a device whose compiler
  rounds a shader at a time (`../ARCHITECTURE_GPU_SHARED_ROWS.md#token-table-gather`) - and the CPU dot, the bf16 rows'
  halfword stamp at an odd dim among its arms.
- `test_vulkan_kernels_gemm.das`'s `test_vkd_f32_khr_batch` cell must hold the staged KHR tile over f32 rows against
  the CPU oracle with each weight rounded to f16, whole and in two k chunks through the reduce.
- `test_vulkan_kernels.das`'s `test_vkd_kq_gemv_k4_gu` cell must hold the fused gate + up + act + requant on Q4_K,
  IQ4_XS, iq3s4 and IQ2_S against the three-kernel path byte for byte and the CPU chain.
- `test_vulkan_kernels.das`'s `test_vkd_q51_gemv`, `test_vkd_iq4nl32_gemv`, `test_vkd_q4032_gemv`,
  `test_vkd_q5032_gemv`, `test_vkd_q20_gemv`, `test_vkd_q10_gemv` and `test_vkd_mx4_gemv` cells must hold the format's GEMV against its scalar dot, and its
  summed twin (`per32_sum_rows_cell`) over one, two and four token rows - every row its slots' dots under their
  weights, a poisoned last row the control, a wider dispatch's rows bit for bit the narrower one's.
- `test_vulkan_kernels_gemm.das`'s `test_vkd_mmq_tiles`, `test_vkd_mmq_feed` and `test_vkd_mmq_inv` cells must hold the
  int8 tiles against the f16 oracles and the feeds and the row map against their host forms, each with its
  poisoned-element control, and `test_vkd_mmq_device_check` the device check passing on an AMD KHR device.
- `test_vulkan_kernels_gemm.das`'s `test_vkd_q51_cm2_batch`, `test_vkd_mx4_cm2_batch`, `test_vkd_iq4nl32_cm2_batch`,
  `test_vkd_iq3s4_cm2_batch`, `test_vkd_f16_gemm_khr`, `test_vkd_f16_gemm_s`, `test_vkd_kq_batch_family`,
  `test_vkd_khr_rot_tiles`, `test_vkd_nv4_cm2_batch`, `test_vkd_q20_cm2_batch`, `test_vkd_q10_cm2_batch`, `test_vkd_iq1s_cm2_batch`
  and `test_vkd_iq1m_cm2_batch` cells - the census's evidence for the classes no cell of a whole run dispatches - must
  hold each class against its CPU oracle at the bar the cell sets, with the cell's poisoned-element control.
- `test_vulkan_kernels_attn.das`'s `test_vkd_fa_cm2`, `test_vkd_fa_khr`, `test_vkd_fa_khr_h256_softcap`,
  `test_vkd_fa_khr_h256_gated`, `test_vkd_fa_khr_tfirst`, `test_vkd_fa_khr_small_twins`, `test_vkd_da_attn`,
  `test_vkd_da_attn_rqk`, `test_vkd_da_attn_tile` and `test_vkd_da_attn_bw` cells - the census's evidence for the
  classes no cell of a whole run dispatches - must hold each class against its CPU oracle at the bar the cell sets,
  with the cell's poisoned-element control.
- `test_vulkan_kernels.das`'s `test_vkd_mx4_gemv_gu`, `test_vkd_gemv_ncol`, `test_vkd_q8_gemv_gu_n`,
  `test_vkd_q8_gemv_pleact`, `test_vkd_act_family`, `test_vkd_moe_routing`, `test_vkd_dn_family`, `test_vkd_hc_seams`,
  `test_vkd_hc_rows`, `test_vkd_ple_side`, `test_vkd_moe_gather_row`, `test_vkd_combine_row` and
  `test_vkd_emb_gather_family` cells - the census's evidence for the classes no cell of a whole run dispatches - must
  hold each class against its CPU oracle at the bar the cell sets, with the cell's poisoned-element control;
  `test_vkd_gemv_ncol` must also hold every column of the N-column stamps to the one-column class bit for bit - within
  the float bar on the rotated twins' leg (`../ARCHITECTURE_GPU_SHARED_GEMM.md#ncol-rotated-walk`) and where the
  one-column slot serves its load-ahead alternate at another subgroup width
  (`../ARCHITECTURE_GPU_VULKAN_GEMM.md#vk-alt-stamps`).
- `test_vulkan_tower_kernels.das`'s `test_vkt_fa_bidir`, `test_vkt_tower_rows`, `test_vkt_tower_ln_rows`,
  `test_vkt_tower_f16_feeds`, `test_vkt_tower_vision_rows`, `test_vkt_tower_win_rows`, `test_vkt_tower_tail_rows`,
  `test_vkt_tower_front_movers`, `test_vkt_tower_front_mel`, `test_vkt_tower_ga_mel`,
  `test_vkt_tower_cn_attn_rel_plane`, `test_vkt_tower_conformer`, `test_vkt_tower_fastconformer`,
  `test_vkt_tower_ga_dwh`, `test_vkt_tower_dw2d`, `test_vkt_wdec_classes`, `test_vkt_wdec_fused_rq`, `test_vkt_rnnt`,
  `test_vkt_ctc_pick`, `test_vkt_nmt`, `test_vkt_nmt_attn`, `test_vkt_dflash_pass_kernels`, `test_vkt_dflash_topk` and
  `test_vkt_dflash_walk` cells - the census's evidence for the tower, transducer-decode, CTC-pick, translation and
  block-drafter classes - must hold each class against its CPU oracle at the bar the cell sets, with the cell's
  poisoned-element control; `test_vkt_wdec_fused_rq` must also hold each fused stamp to the separate pass's Q8_0 bytes
  and scales exactly, and `test_vkt_tower_vision_rows` the position add to the CPU form bit for bit - where
  `vk_rounds_alike` is false, the first within a quant step with the scales at the approx bar and the second at the
  approx bar (`../ARCHITECTURE_GPU_SHARED_ROWS.md#token-table-gather`).
- `test_vulkan_kv_codec_kernels.das`'s `test_vkc_q8_da_attn` and `test_vkc_tq4_da_attn` cells must hold the codec
  attention arms (`codec_attn_arms`) against the CPU attention over the codec's rows, each arm with its controls.
