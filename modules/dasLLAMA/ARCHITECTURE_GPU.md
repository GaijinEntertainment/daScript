# dasLLAMA Architecture - GPU backends

Companion to `ARCHITECTURE.md`; a section is cited by its anchor. This document carries the
GPU backend role table with the kernel-binding asymmetries a stamp family ledgers. The closed
list of the allowed asymmetries between the two backends is
`ARCHITECTURE_GPU_ASYMMETRIES.md#gpu-asymmetries`. The companions beside it carry the
sections built on it, and each one's opening routes to its own companions:
`ARCHITECTURE_GPU_RACE_SHAPES.md` (the tensor-GEMM and fused-attention shapes that measured
out); `ARCHITECTURE_GPU_TOWER.md` (the Metal tower's attention routes and encode chains and the
StyleTTS2 chain); `ARCHITECTURE_GPU_TOWER_POCKET.md` (the Metal tower's Pocket TTS seats); `ARCHITECTURE_GPU_QUANT_PLANES.md` (the Metal quant
plane reads and the GEMV site abstraction); `ARCHITECTURE_GPU_MTP.md` (the Metal speculative
round, the depth a round drafts, and the verify, drafter and batch-driver mechanics);
`ARCHITECTURE_GPU_VULKAN.md` (the Vulkan resident driver).

### GPU backends {#gpu-backends}

A GPU backend is a FAMILY of role files - matching things in matching files across backends, so
that a question answered for one backend has an obvious address in the other. The roles:

| role | holds | must not hold |
|---|---|---|
| the kernel home<br>`dasllama_metal_kernels`, `dasllama_vulkan_classes` | kernel source, the threadgroup reductions every Metal body folds through (`tg_sum_all` / `tg_max_all`, `MetalTgReduceBase` over its own `partial[]`), the workgroup folds every Vulkan body whose fold is one value derives (`WgReduceBase`'s `wg_sum` / `wg_max` / `wg_rms_inv`, one value over every lane, returned to every lane - the value form of the shared `GkWgReduce` in `dasllama_gpu_kernels_common`, which a kernel both homes stamp derives directly) - two Vulkan bodies fold by hand by design, because their folds are split: `DnStepFused`'s q and k halves (`subgroupAdd` into per-subgroup arrays, a lane-0 loop over them, one set of results every lane reads) and `TtsPkAttn`'s `po` quarters (four partial value sums a column, added by the column's lane), the kernel-side quant-decode helpers that read a codebook table (the table-free arithmetic both homes splice is `dasllama_gpu_math`'s) and the per-word codebook accessors (`[grid_words]` bakes each from `dasllama_kqformat`'s one literal at compile time - the bytes live there, the home carries the baked copy), the derived-access/PSO census; on Vulkan the one device buffer kernel data fills (`kq_grid_dev`, the grid codebooks) and the host-side ensure/set/enc pick ladders and grid rules over its own class stamps (`gemv_*`, `q8_gemv_gu_n_*`, `q8_batch_cls_*`, `kq_batch_cls_*`, `fa_stamp_*`, `f16_gemm_*`, `da_slab_*`, `da_attn_stamp_*` with the block codecs' `kv_codec_cls_ensure`, `da_attn_b_stamp_*`, `rope_kv_b_stamp_*`, and the tile trios `cm2_cls_*` (the engine's, over every tile tail) and `khr_cls_*` (the kernel cells' KHR arm); `kq_tile_stamp` stamps the tile trios, `kq_batch_cls_*` and the GEMV ladders (its `gemv` family) over `KqFmt`, `stamp_ladder` the `fa_stamp_*` and `da_attn_stamp_*` ladders over their stamp keys); the tower row classes' mode selectors and family constants - the shared bias rows' act code (`GkBiasActArgs.act`, the `BIAS_ACT_*` codes `dasllama_gpu_math.das` carries, relu for the subsample stacks) and `Q3A_TOK_PER_CHUNK`, `GK_G4A_ATTN_PAST`, `G4A_ATTN_CAP`, `WDEC_HS`, which the drivers check against the family before they serve | device state other than `kq_grid_dev`, engine types |
| `dasllama_<gpu>_common`<br>`dasllama_metal_common`, `dasllama_vulkan_common` | device state, buffer/command plumbing, hazard + capture rail, profiler, host-side quant-decode helpers (Metal's `iq4_lut`), the family's registrant of a tier seat that names a size the device state keeps (Metal's `dn_mirror_room`) | driver policy |
| `dasllama_<gpu>_decode`<br>`dasllama_metal_decode`, `dasllama_vulkan_decode` | the resident token-step driver + decode-time arms; on Vulkan, for a hyper-connection MoE (`Config.hyper_conn`), the split token command and the hot pool's device slots, stacks and hit chains (`ARCHITECTURE_GPU_VULKAN_HC.md#hc-token-command`, `#hc-hot-pool`) | kernel bodies |
| `dasllama_<gpu>_prefill`<br>`dasllama_metal_prefill`, `dasllama_vulkan_prefill` | the batched prefill driver + batch arms; on Vulkan, for a hyper-connection MoE, the window chain cut after each router (`ARCHITECTURE_GPU_VULKAN_HC.md#hc-window-chain`) | kernel bodies |
| `dasllama_<gpu>_shapes`<br>`dasllama_metal_shapes` | PORTABLE servability gates - no GPU C++ require, so any box can bake | device calls |
| the tower driver<br>`dasllama_metal_tower`, `dasllama_vulkan_tower` (the vision ViT chains on both weight lanes - the q8 image on the q8 GEMM, the f32 planes on the f32 tiles - qwen25v's over the halfword twin, and the audio chains over the q8 image - the whisper-class towers with their conv stem, the gemma4a Conformer with its chunk front and projector tail, the canary FastConformer with its front, the qwen3a front and the whisper mel seat; on a build without das_metal it fills the gemma4uv, gemma4v, gemma3v, qwen3v and qwen25v hook slots whole (`ARCHITECTURE_GPU_TOWER_VULKAN_VISION.md#vk-vision-chains`) and the audio seats `register_tower_blocks_gpu`, `register_tower_blocks_ln_post_gpu`, `register_tower_conv_gpu`, `register_qwen3a_front_gpu`, `register_whisper_mel_gpu`, `register_gemma4a_gpu`, `register_gemma4a_chunk_gpu`, `register_canary_gpu` and `register_canary_front_gpu`; parakeet's `register_parakeet_gpu_encode` - both homes register them) | one-shot embedder/encoder encodes (gemma4uv chain, the gemma4v ViT with its patch-conv stem and projector tail, gemma3v SigLIP with its patch conv and projector tail, and qwen3v block loops - qwen3v adds the vision NEOX rope, the fused-qkv weight-offset GEMMs, and the inline deepstack tap + tail merger chains - the whisper-class block loop with its post-norm or its projector tail behind it, the qwen25v window ViT, the gemma4a Conformer chain with its mel/conv front, the FastConformer chain canary and parakeet share - one block body over `CanaryLayerOffs`, parakeet's offsets mapped onto it with no GEMM biases and the tap-major depthwise stamp - the Pocket TTS codec, frame loop and text prompt (`ARCHITECTURE_GPU_TOWER_POCKET.md#tower-pocket-codec`, `ARCHITECTURE_GPU_TOWER_POCKET.md#tower-pocket-frames`) - the whole StyleTTS2 synthesis (kitten and kokoro on both weight lanes: seven seats from PL-BERT to the inverse STFT, the front end on the f32-exact GEMM stamp, the source chain the CPU's operation for operation, `ARCHITECTURE_GPU_TOWER.md#tower-tts-chain`) - the conv frontends + the qwen3a padded-weight slab and GPU front/mel) - no session, no mirror - the Pocket frame loop's per-voice device K/V slot is the one state kept across calls; registers the gemma4uv, gemma4v (`register_gemma4v_gpu`: the chain takes the image planes and runs the stem, the blocks and the tail), gemma3v, qwen3v, qwen25v, whisper-class blocks (`register_tower_blocks_gpu`, `register_tower_blocks_ln_post_gpu` with the post-norm behind them, `register_tower_blocks_tail_gpu` with the projector tail behind them), tower-conv, qwen3a-front, whisper-mel (`register_whisper_mel_gpu`, the spectrum of the qwen3a mel and of the chat towers' chunked mel), gemma4a, gemma4a-chunk, canary (`register_canary_gpu` for the blocks, `register_canary_front_gpu` for the front), parakeet (`register_parakeet_gpu` for the blocks, `register_parakeet_gpu_encode` for the whole encode with the subsample front) and StyleTTS2 (`register_styletts2_gpu`, the seven-seat record) and Pocket (`register_pocket_gpu`, the codec, frames and prompt seats, `ARCHITECTURE_GPU_TOWER_POCKET.md#tower-pocket-codec` and `ARCHITECTURE_GPU_TOWER_POCKET.md#tower-pocket-frames`) hooks; the whisper-class block GEMMs and its conv stem's second GEMM on a q8 tower borrow `pf_enc_q8_mm`, the projector tails borrow `enc_tw_pool2` and `enc_tw_swiglu_rows` (whisper-class), `enc_tw_pool2d` (gemma4v, gemma3v) and `enc_tw_affine_rows` (gemma4v), the canary front borrows `enc_cn_melnorm`; the FastConformer chain borrows `enc_fc_pack` / `enc_fc_softmax` / `enc_fc_unpack` around `enc_f32_mm`, `enc_cn_dw` / `enc_pk_dw`, the prefill's `pf_enc_fc_attn_dev` (every head's attention at once on the device pair, under its crown) and the parakeet front's `enc_pk_dw2d` / `enc_pk_xf` (its first conv rides `enc_im2col2d`, the f32 GEMM and the bias rows), and the `enc_st2_*` row, LSTM, attention and source kernels around `enc_st2_conv_mm` / `enc_st2_conv_exact_mm`, the `enc_pk_*` row, add-and-norm, attention-row and prologue/epilogue GEMV kernels for the Pocket codec and frame loop beside the decode rail's `enc_gemv` / `enc_rpst32_c` and the prefill's `enc_rope`; on Vulkan the whisper-class encoder-output handoff - the served post-norm chain's `xb` plane, kept for the encode in flight alone, which the ASR-decoder driver's cross-KV chain reads (`vulkan_tower_enc_out`) - is the second state kept across calls beside the Pocket slot | LLM or ASR decoder session state |
| the ASR-decoder driver<br>`dasllama_metal_asr_dec`, `dasllama_vulkan_asr_dec` | the whisper decoder on the GPU: Metal's 34B weight blob or Vulkan's row-major q8 gather, the f16 resident cross/self K/V, window-granular cross-KV + decode-step serves; registers the whisper cross-KV and decode hooks (`register_whisper_cross_kv_gpu`, `register_whisper_decode_gpu`; family registries in `dasllama_whisper`); on a build without das_metal the Vulkan driver fills them (`ARCHITECTURE_GPU_TOWER_VULKAN.md#vk-asr-decoder`) | kernel bodies, LLM session state |
| the TTS driver<br>`dasllama_vulkan_tts` (Vulkan; the Metal tower driver's own row carries the TTS seats on an Apple build) | the StyleTTS2 synthesis seats and the Pocket codec, frames and prompt seats on the tower's knob - the f32 weight slabs the shared host writer (`dasllama_tts_slab`) lays out, the front end on the f32-exact tile GEMM, the seats registered through `register_styletts2_gpu` and `register_pocket_gpu` on a build without das_metal; the seats serve only while the tier's want arms the device (`DASLLAMA_GPU=1`, off by default), so a run with no flags and no environment overrides synthesizes on the CPU chain on a Vulkan build, where on an Apple build the Metal tower driver's TTS seats serve by default (`DASLLAMA_METAL_TOWER` on) | kernel bodies, the CPU chain's arithmetic (the CPU chain is the specification), LLM or ASR session state |
| the assistant-drafter driver<br>`dasllama_metal_mtp_gemma` | the gemma-4 assistant drafter on Metal: the sidecar blob upload, the Q-only layer chain reading the TARGET mirror at the two capture layers with the decode's own attention kernels, the speculative round over the batch driver's same-slab verify; registers the `metal` round override and delegates head-less-drafter-less models to `metal_mtp_spec_round` | kernel bodies, mirror ownership |
| the kernel-access lens<br>`dasllama_metal_lens` (Metal), `dasllama_vulkan_dispatch` (Vulkan - the `[vk_dispatch]` macro derives access per class) | the kernel-access macro and its dispatch-support macros (`compile_stamp`, `release_handles`) | anything else |

- **Vulkan additionally has an ENTRY, `dasllama_math_vulkan.das`** - capability probe/arm, `.dlim`
  identity source, cross-arm routers, the `[init]` installs. It re-exports the family `public`,
  and its NAME is the transformer umbrella's `?vulkan` require contract (deliberately LAST in the
  umbrella: the vulkan drivers are the hot-edit modules, and require order is the jit obj-cache
  layout).
- **Vulkan additionally has `dasllama_vulkan_seams.das`** - the thin whole-op call seams the tier
  and suites dispatch through (`vk_add_rms`, `vk_rope_kv_store`, `vk_decode_attn`). It exists
  because of a require direction: common cannot require the classes module (classes requires
  common back), so any seam that encodes a class kernel must sit above both. A seam here wraps
  ensure/set/enc - kernel bodies and driver policy stay out.
- **Metal has NO `math_` entry** - the family enters via the transformer's `?das_metal` requires
  plus unconditional shapes. Its below-common piece is **`dasllama_metal_gemm.das`** (the batch
  GEMM donor that common requires `?das_metal`), which owns its device by necessity:
  metal_common -> dasllama_common -> metal_gemm would cycle. `REVIEW.das`'s
  `check_device_creation_sites` walks `dasllama/` for a device or queue creation outside the two
  `_common` files and licenses `dasllama_metal_gemm.das` plus the two tuner race entries
  (`metal_tensor_race`, `metal_tensor_race_decode`), which run before the driver inits.
- **`REVIEW.das`'s `check_gpu_role_partition` licenses this table's roles** - Metal's `kernels`,
  `common`, `decode`, `prefill`, `gemm`, `shapes`, `tower`, `asr_dec`, `lens`, `mtp_gemma`;
  Vulkan's `classes`, `common`, `decode`, `prefill`, `seams`, `dispatch`, `tower`, `asr_dec`,
  `tts` - and finds a `dasllama_<backend>_<role>.das` with any other role.
- **Backend-only capabilities live in their matching ROLE file, not in new grab-bags** - vulkan's
  weight arena, streamed mirrors, heat cache, host-import, coopmat; metal's blob transform and MTP.
- **The tower driver owns NO PSOs.** Its kernels (LN, f32 mul_mm, the two gelu flavors,
  posadd, the gemma4v clamp / rope2d / GEGLU-quick, the head restride - gemma3v's and, offset-bound, the qwen3v/prefill slicers) live in the kernel home, so `metal_decode_init` compiles and `metal_kernels_release`
  releases them like every other registry PSO; the borrowed prefill builders (`pf_enc_bf16_mm`,
  `pf_enc_hmm`, `pf_enc_rms`, `tw_half`, `enc_rope` - the qwen3v
  vision NEOX apply and the Pocket codec rope over its row stride - the attention trio `enc_qk_mm`/`enc_rowstat`/`enc_av_mm`, the tower
  specials `enc_tower_flash`/`enc_tower_kv_hc`/`enc_tower_win_attn`,
  and the `enc_g4a_*`/`enc_cn_*`/`enc_pk_*`/`enc_fc_*`/`enc_q3a_*` chain families; this list is the closed borrowed
  set, and every class a route dispatches sits in that route's ensure chain) come up through
  `metal_prefill_pso_init`, prefill's public bring-up seat, and `plane_buffer` in common is
  public for the same wrap-a-plane reason. The tower-only kernel classes COMPILE in the
  prefill file beside the builders that bind their PSOs - the same convergence debt `ARCHITECTURE_GPU.md#gpu-backends`'s
  kernel-home entry ledgers, not a new placement. The tower's own objects (the ones buffer,
  its scratch pool, the qwen3a padded-weight slab) release through `metal_tower_shutdown`,
  and the slab additionally drops with the weights epoch through the tower's reload prep.
- **The Vulkan tower driver serves the vision ViT chains, the audio block loops and the audio
  fronts, the Vulkan ASR-decoder driver the whisper decoder, and the Vulkan TTS driver the StyleTTS2
  and Pocket seats** (`ARCHITECTURE_GPU_TOWER_VULKAN_TTS.md#vk-tts-chain`,
  `ARCHITECTURE_GPU_TOWER_VULKAN_TTS.md#vk-pocket-chain`); the audio towers serve their q8 lanes on the CPU chain and
  on the driver alike (its `serves` answer to their lane policy is no); the vision chains serve both lanes and register the f32 lane as served, as Metal's do. Likewise the
  non-causal media span: Metal serves it through `AttnArgs.uend` - including the FUSED image turn
  (head + media rows + tail as ONE eval, the per-query mask through `AttnArgs.ulo`); the Vulkan
  resident prefill declines span evals (`followup_general.md` #23's remaining half) and registers
  the split-span capability (`register_prefill_override_split_span`), so `eval_embd_span_` keeps
  the three-eval splice while vulkan is the active override. The qwen mrope quantum rides the same
  shape through a second capability seat (`register_prefill_override_mrope_tables`): Metal's
  `enc_rope` reads the per-token table rows `prefill_rope_tables` builds from the grid map, so it
  serves mrope unchanged and registers the seat; an override without it (vulkan builds angles
  from a scalar position) declines the quantum to the CPU loop by name. The deepstack quantum is
  the third seat (`register_prefill_override_ds_adds`): Metal uploads the caller's wide quantum
  WHOLE (the `Session.wide_src` borrow), slices x and the slice-major ds planes on-device through
  the offset-bound head restride, and encodes one `enc_add` at the slice offset after each tapped
  layer's residual - no new kernel (the CPU-side split, `ds_split_quantum`, survives as the
  CPU-loop fallback and the warm/MTP edge); an override without the seat declines deepstack
  quanta by name, so Metal serves them and Vulkan does not. The E-series PLE pre-step is two gate
  seats, one per direction: `register_ple_gpu_gate` for a prefill override that builds the side input
  on device off the stashed token ids (Metal and Vulkan), `register_ple_gpu_decode_gate` for a decode
  override that gathers the token's row on device (Vulkan alone); the hub skips the CPU pre-step only
  for the direction whose gate answers yes, so the Metal decode reads the CPU-built side input. qwen4exp's n-gram
  side input splits the same way with no gate: the CPU pre-stack hook gathers the token's hashed heads (the 28 GB table stays host-side), the Metal decode projects, gates and convolves them (`ARCHITECTURE_GPU_MTP_DECODE.md#metal-layer-enc`).
- **Per-layer FFN widths (MatFormer E-series, at most two - `ffn_second_hidden`) serve on Metal
  and on the Vulkan whole-model driver**: the Metal decode, batch and prefill drivers bind the
  width per layer (dense trunks, no MTP; the batch sizes its panels to the wider width and carries
  the second width's row-total twin), and the Vulkan driver's per-layer geometry (`RLayer.hid`)
  carries it beside the PLE branch. The Metal batch serves the rest of the E-series shape the same
  way the single row does: the PLE branch as a rows form over a layer-major side plane (the CPU
  pre-step's position-major rows transposed at the poke), and a shared-KV layer as Q-only rows -
  the rope-store grid stops at the Q pairs, so nothing is written into the source slab the layer's
  attention reads through the aliased row prefix. A MoE's shared expert rides the batch the same
  way it rides the verify rows: the gate dot as a one-row router GEMV over the rows, the gate|up
  pair and the down as rows forms over the expert panel once the routed W2 has consumed it. A
  deltanet hybrid's recurrent layer runs its projections as rows GEMVs and then the conv, history,
  norm, scan and gate a row at a time against that session's own `DnMirror` (`recurrent_batch`):
  the `DnArgs.row` field picks the row's slice of the batch planes, the mirror's live-region bases
  pick its state, and the mirrors advance when the step lands (`BatchLanding.dn_uids`); a step whose
  rows have no resident or CPU-synced state declines `dn_state`. The mirror cache (`g_dn_mirrors`,
  an LRU) rests at four sessions and grows to whatever a scheduler names through the tier's
  `gpu_dn_room_` seam at its creation (`create_scheduler` -> the facade's `gpu_dn_room`), and the batch
  build names its own row count too: a cache narrower than the batch would evict a row of the step
  it prepares, and that stream's next step would decline `dn_state` with no CPU chain to fall to
  on a blob-only model. Every plane a rows GEMV lands in is sized to the rows form's four-row tile
  reach (`mr`), because the tile writes whole tiles past the live rows. `eval_batch_` hands a
  hybrid's rows to an armed device driver first and to the CPU stack's own hybrid form
  (`ArchBlocks.attn_batch`) when none is armed or the driver declines on a planar model; only a
  blob-only model's declined step falls to the single-row forward. The batch's split single-pass
  attention (`MetalSqAttnDKvT` and its combine) takes the head width at run time from one
  compiled variant - a lane owns one quad of the head, a head of 128 fills the simdgroup and a
  head of 64 or 96 idles the lanes past it (zero query, no store) - serving all three on every mirror codec
  with no per-head stamp (the block codecs' twin template, `MetalSqAttnDQuantT`, reads a lane's
  quad as four quants of a 32-element block and its scale). A head of 256 or 512 takes the WIDE
  stamps of either template: a threadgroup is one slice group, and its two or four simdgroups each
  own 128 dims of the head - four blocks of a block codec. A key's score is the sum of every
  simdgroup's share, posted to threadgroup memory and read back between two barriers a slice
  (`sqd_wide_sum`; the two templates differ only in how a lane reads its quad, and share the slice
  walk, the weights, the merge and the store as `sqd_*` helpers), so a slice costs a wide head what it costs a head of 128 - a lane
  walking the head's quads in turn costs two to four times that and loses to the chunked pair on
  a model of few heads. The partials land as two or four heads of 128 under one softmax - each an
  entry carrying the slice group's max and sum - so the combine kernel reads them with that many
  times the heads and no stamp of its own (`attn_d_halves`, `attn_d_part_floats`). A model's two
  head classes (gemma-4: a head of 512 on the full-context layers, of 256 on
  the sliding ones) each take their own stamp layer by layer - the encode derives the heads a K/V
  head and the score scale from the layer's own head and K/V row, and the partial plane is sized
  for the wider class. The single row takes the fused form past the
  single-dispatch ceiling, and so do the speculative verify's rows (`attn_d_serves` is the one
  gate the three row shapes ask, `encode_attn_d` the one encode): the single row's route table is
  one row, written when the step's resources are acquired, and the verify binds its per-layer
  table at the layer's offset, each base already the layer's. A sliding layer hands the kernel its
  span: a slice group starts at its first slice that reaches the window and a key below the
  window weighs nothing, so a 128-key layer reads four or five slices whatever the context. A
  layer's attention sinks join the combine alone - one more logit a head in the final max and sum,
  carrying no value. An attention logit soft cap is a uniform of the kernel, applied to a live
  key's scaled score before the slice's max - a lane past the slice's rows is named dead there,
  since the cap would lift its seed to minus the cap. The single-row driver's NextN verify rows
  alone keep the chunked pair, on a model with a window or sinks - the base geometry that pair
  serves, and a shape no served NextN carrier has. The fused form's cost per cached
  key is about a quarter of the chunked pair's, so it is what keeps a stream's decode rate from
  falling with context.
- **Family-shared kernel classes live in `dasllama_metal_kernels`.** The `[metal_dispatch]` lens
  generates `enc_*` builders and MSL globals into the module the class COMPILES in, so co-location
  follows the class, never "the builder needs the driver module". Each builder has a
  `<builder>_pso(enc, pso, ...)` twin that encodes the same binds and grid through the pipeline it
  is handed rather than the `pso =` global. Prefill's prefill-only classes are convergence debt, not precedent. Kernel twins - classes whose bodies differ only on one axis - carry that difference as a `@template_constant`, an overridden method the emitter splices flat, or a run-time value the builder passes.
  A per-format family stamp is `[metal_dispatch(stamp = "<family>:<fmt>:<form>")]` (`kq_mm`, `moe_mm`,
  `moe_mm_split`): the lens derives every string the long form spells, an explicit argument wins, and the
  threadgroup-memory global is always `<Class>_<kernel method>_msl_tgmem`. Hosts compile through
  `compile_stamp(<stem>_msl, ok)` - one spelling, so a source never pairs with another kernel's entry.
  A stamp's `params=` are its family's shared builder signature - the forms are taken by address
  into one table - so the lens's unread-param refusal (a `params=` name no `grid=`, `tg=`,
  `requires=` or `@span` reads) exempts stamp-derived params; a long-form class is refused.
- **Ledgered kernel-binding asymmetries** - a REVIEW rule firing on one of these is expected, and
  this entry is the sanction: a Metal kernel gate under `tests/` builds its own pipeline from the stamp's MSL and dispatches it through the class's generated `enc_*` builder (its `_pso` twin where the pipeline global is private to its driver), so a gate proves the body and the builder's bind list together - it binds by number only where it dispatches a grid or a bind the builder cannot express, the site saying which; the moe mul_mm TENSOR twins (`MetalMoeMulMmQ8T` / `MetalMoeMulMmMx4T`)
  keep the pre-family compact kargs slots while their base classes bind the family numbers, so no
  shared bind path may span the two layouts; the MoE combine pair (`MetalMoeCombine` y/dim/nk at
  2/3/4, `MetalMoeReduce` at 3/4/5 under its gated `inv`) keeps each leaf's numbers; the in-engine moe mul_mm A/B race harnesses
  (`dasllama_metal_prefill.das`) encode through `kn_moe_mm_family_tail` rather than a per-class
  `enc_*` builder; the iq4 family's iq4nl stamps (`MetalKqGemvIq4T`, `MetalKqMvIq4T`, `MetalKqMvB8Iq4T`)
  and the mul_mm tensor template's compact-scale stamps off the same family (`MetalKqMulMmIq4xsTensorT`
  at IQ4NL, the dense `MetalKqMulMmIq4nlT` / `TH` and `MetalKqMulMmQ40T` / `TH`, and the four
  `MetalMoeMulMmQ40*` expert stamps) bind the strip plane unread, so both formats share one set layout
  and one host bind path; the split-scale dev-W dequant stamps (`MetalKqDequant<Fmt>`) inherit the
  mul_mm scaffold's `xf` and `y` bindings unread - they write only the f16 panel at binding 7 - so a
  format's dequant pass and its mul_mm twins keep one set layout and one host bind path; and the Vulkan
  `kq_gemv_cls` family binds `gridb` (the grid formats' codebook plane, `kq_grid_dev`) at binding 6 on
  every stamp, one set layout for the family - the grid stamps (iq2xxs, iq2xs, iq2s, iq3xxs, iq3s) and
  their N leaves read it; every other stamp (k2 k3 k4 k5 k6 q40 iq4xs iq4nl, iq3s4 - whose codebook is arithmetic - and the fused `kq_gemv_k4_gu_cls`) binds it unread.
- **`dasllama_gpu_tier.das`** - the device-cooperation SPI: hook types, install/unset slots, route/mark/want/status
  state, engine-facing forwarders, and the backends' shared kernel-coverage census log (`kernel_coverage_report`). Vulkan implements it (per-op offload plus resident plumbing, and the decode-era
  seats it alone fills: the cm2 expert chain `set_moe_gpu_ffn_xf_hooks` / `_async_hooks`, the decode attention block
  `set_moe_gpu_attn_dec_hooks`, the decode FFN tail `set_moe_gpu_ffn_tail_hooks`, the deltanet decode step's state
  seams `set_moe_gpu_dn_state_hooks` (flush, invalidate, release), the whole-token span `set_moe_gpu_span_dec_hook` -
  the span rides common's decode override registry as `vulkan_moe_span`, selected by the MoE placement and declining
  per token - the resident driver's q/k/v projection-bias seat `install_moe_gpu_resident_bias`, its attention-sink seat
  `install_moe_gpu_resident_sinks` (the per-head sink plane, `ARCHITECTURE_GPU_VULKAN_ATTN.md#vk-attn-planes`), its MoE seats
  `install_moe_gpu_resident_moe` (the tile admission per expert triple, the routing geometry with the router plane, an
  MoE layer, and the routed block on a layer another seat built), its hyper-connection seats `install_moe_gpu_resident_hc`
  (the mixer sites' planes, the n-gram side input's, the host-experts switch and the split token command,
  `ARCHITECTURE_GPU_VULKAN_HC.md#hc-token-command`), its hot expert pool seats `install_moe_gpu_resident_hot` (a routed
  layer's slots, an expert's planes into a slot, a step's hits, a verify's rows' hits; a tier without them serves every routed expert on the
  host, `ARCHITECTURE_GPU_VULKAN_HC.md#hc-hot-pool`), the prefill seats' `experts` argument a plain MoE's cut prefill window asks
  for the host's routed sums through (the hyper-connection window chain's twin), its mirror-region seat `install_moe_gpu_resident_regions`
  (`rdec_select_region` names the region every mirror address and the next token command resolve against; a tier
  without it serves one region), its N-row batch seat `install_moe_gpu_resident_batch` (the N-row token command beside
  the resident driver: `rows` answers how many rows it steps at once on the armed model, 0 = none; `rdec_token_n` steps
  them), all behind the route lever `set_gpu_resident_route` / `gpu_want_resident`, the
  OS video-memory seat `install_moe_gpu_os_memory` the residency plan sizes against, the weight-bytes seat
  `install_rdec_note_weight_bytes` the decode warm-up guard reads, the speculative carry's landing `install_rdec_carry` (`ARCHITECTURE_GPU_VULKAN_RESIDENCY.md#logits-transfer-queue`), the NextN draft head's seat `install_rdec_draft` (`ARCHITECTURE_GPU_VULKAN_MTP.md#resident-draft-head`), its prompt warm on the window chain `install_rdec_head_warm` (the ask and the rows written; the head's K/V row ranges move up and down through the resident driver's two K/V row seats, `rdec_kv_sync` / `rdec_kv_read` (filled by `install_moe_gpu_resident`'s `kv_sync` / `kv_read` arguments), at layer `n_layers`; `ARCHITECTURE_GPU_VULKAN_MTP.md#resident-head-prompt-warm`), the same-slab verify's `install_rdec_verify` (`ARCHITECTURE_GPU_VULKAN_NROW.md#nrow-verify-command`), the reject's device rollback `install_rdec_rollback` (`ARCHITECTURE_GPU_VULKAN_MTP.md#resident-verify-rollback`), and the per-layer-embedding seats `install_rdec_ple`
  - the branch's width, its per-layer gate and proj planes, the pre-step's projection and the token table on the
  device). The installs are one-way (a test that arms the tier never restores them) but for `unset_rdec_draft`, the CPU
  draft's control, which answers the seat for `reinstall_rdec_draft`; a seat serves whatever model loads next. Metal deliberately does not - UMA makes residency moot there, and
  Metal integrates as a whole-forward driver through common's override registries (the ASR-decoder driver is the one
  exception: whisper is not a `Model`, so its hooks are family registries in `dasllama_whisper`, same decline contract).
- **`dasllama_gpu_resident.das`** - the WHOLE-MODEL residency rail, and the Model's per-format plane-pair accessors (`kq_plane_pair` / `kq_plane_pair_names`) the rail's walk and the mint's section names both read: bake the device layout offline into the flavor
  image, upload a model's stacks to the tier, drive decode/prefill entirely on device, record the routed pick trace
  (`DASLLAMA_MOE_TRACE`, the pool's offline instrument), and decide the trimmed lane -
  its admission (`resident_would_serve`) and the emb region the CPU keeps (`trim_pack_emb`). It is device-AGNOSTIC - it
  holds no device call and requires no GPU module, reaching the hardware only through the `dasllama_gpu_tier` SPI and
  entering the engine only through common's override registries. `"vulkan"` is the tier string it registers under, not
  a dependency, which is why it compiles on every box. It requires common back for `Model`/`Session`, so like the
  Metal drivers it is required from the transformer umbrella, never from common; it requires `dasllama_moe` and
  `dasllama_blocks` for the routed sums it runs on the host. On a hyper-connection MoE it keeps the routed experts
  on the host and sums them between the command's segments (`ARCHITECTURE_GPU_VULKAN_HC.md#hc-token-command`),
  and it owns the hot expert pool's POLICY - which experts sit in the tier's slots, which picks the device serves
  (`ARCHITECTURE_GPU_VULKAN_HC.md#hc-hot-pool`); the pool's device side is the tier's four hot seats.
- **A dry bake runs the whole resident arm with no device.** `vulkan_bake_role` puts the tier in
  bake mode, and each `rdec_*` device seam answers for itself so the arm walk reaches the end: a
  seam that only records a layout (`vk_rdec_set_emb`) answers true, one that would allocate device
  memory (`vk_rdec_upload_emb_f32`) answers false without touching a device - which keeps a baked `.dlim` layout equal to the one a real arm produces.
- **`dasllama_gpu_math.das`** - the ALU helpers both kernel homes splice into their shader bodies
  (`ksign7`, `iq3s_signed`, `softcap_exp`): pure arithmetic, no table, no backend lowering, so one
  owner serves the Metal and the Vulkan bodies alike; it also holds the CPU chains' matching scalar forms (the sigmoid, the SiLU, the logistic gate, the softplus: `sigmoid_f32`, `silu_f32`, `sig_gate`, `softplus`); the codebook tables stay per kernel home.
  `mad` is the fused instruction by definition and leaves a driver nothing to choose, so two bodies
  held bit for bit spell as `mad` each multiply that feeds an add.
- **`dasllama_gpu_kernels_common.das`** - the kernels both kernel homes stamp, each one `class template`
  with its buffers, its argument struct and its body (`#gpu-shared-kernels`); it requires no backend.
- **`dasllama_kernel_access.das`** - the shared body-walk read/write classifier both GPU lenses run
  on, plus the dispatch-lens micro-grammar (the grid/tg/params spec tokenizers and the shared
  AST-emission core: `is_digit_tok`, `role_ok`, `derived_role`, `mk_uint_cast`, `mk_call1`,
  `mk_grid_dim`, `param_type`, and `kargs_as`, which gives a shared kernel's `@kargs` member the argument
  form each lens's emitter reads). One owner by design: a private copy per lens drifts (metal folding
  only the literal "1" where vulkan folds any integer). Backend-specific lowering stays in that backend's lens.
- **The authoritative site of each constant kind.** Tile constant in a kernel body: the literal in the generated `*_msl` global or the
  SPIR-V dump (`DASLLAMA_VK_SPV_DUMP=<dir>` writes every class kernel's words). Grid constant: the class's `[metal_dispatch]` / `[vk_dispatch]`
  `grid=` spec (`"n/c"` is a CEIL-divide); a `grid = "wgs"` class carries no number there - its grid is the kernel body's workgroup-index
  decode with the host helper that computes `wgs`. Threadgroup constant: Metal's `tg=` spec or Vulkan's `[spirv_kernel(local_size_x=)]`. Uniform: the single writer that fills its buffer. A kernel class's compile-time constants - its `@template_constant`s, its literals and the module `let`s its body reads - are stamps: a module `let` is a stamp; a module `var` is not, because its value at stamp time is whatever the host last wrote.

**PSO lifecycle - the family shares ONE device and queue** (`metal_common_init`; the tune-time
race arms' transient queue is `ARCHITECTURE_MEASUREMENT_KERNEL_RACE.md#kernel-race-fidelity`'s). The decode
PSO set lives as `g_pso_*` in `dasllama_metal_common`, is compiled by `metal_decode_init` in
`dasllama_metal_kernels` and released by `metal_kernels_release` there - the kernels module owns
its set's lifecycle even though the vars live with the device state. Prefill's `g_pf_pso_*` set
is prefill-private end to end: `metal_prefill_init` compiles, `metal_prefill_shutdown` releases.

**Race and tune code for a kernel family lives beside the family.** The shared scaffolding
(`race_buf`, `race_envelope_ok`, `race_pair_ms`, `MetalTensorRaceResult`) is `<gpu>_common`'s;
each module races its OWN families (`metal_tensor_race_decode` in kernels, `metal_tensor_race`
in prefill) and the tuner calls those public entries.

**Decline REASONS are enum values in the shapes module** (`MetalDecodeDecline`, `MetalPrefillDecline`);
decline COUNTING lives in `<gpu>_common` beside `require_or_panic`, for both paths.

Vulkan is the deliberately-designed model of this shape; Metal converges as it is touched.

### Kernels both homes stamp {#gpu-shared-kernels}

A kernel the Metal and the Vulkan home both run is one `class template` in
`dasllama_gpu_kernels_common.das`: its `@ssbo` buffers with their `@role`s, its argument struct and its
body. The module requires no backend and no shader vocabulary - a template's body resolves in the module
that stamps it. Each home stamps the template as a leaf class under its own dispatch annotation and its
own kernel method, which calls the body, so a kernel keeps the dispatch name, the pipeline and the census
key it has on that home.

- **The arguments are one `@kargs` member.** `kargs_as` (`dasllama_kernel_access.das`) runs in both
  dispatch lenses ahead of the kernel's emission and gives the member the form that home's emitter reads:
  a `@push_constant` block on Vulkan, a by-value `@uniform` at the binding past the class's highest on
  Metal. The struct is a member and not a parameter of the body: a parameter is passed by value, and the
  emitted kernel then copies the whole struct where a member is read a field at a time.
- **A body that reads a buffer at an offset takes it as an element offset in its arguments.** Vulkan caches
  a descriptor set by its binding tuple, so an offset bound per dispatch would be a set per offset; an offset
  in the arguments is one set. A kernel every site reads from element zero carries none.
- **The two emitters place the body differently.** The MSL emitter splices a called method flat into the
  kernel; the SPIR-V emitter keeps it a function the entry point calls. A kernel moved here therefore
  reads, on Vulkan, as its old words plus one call. Because the MSL emitter splices, a template method of
  more than one statement cannot sit in value position: a helper that returns a value - a workgroup
  reduction, a partial sum - is written as a statement method that lands its result in a `var` reference
  (`wg_max_into`, `wg_sum_into`, `value_sum_into` in `GkPkAttn`). The same splice names a parameter as
  many times as the body does, so an argument that is a computed expression is refused where the body
  names the parameter twice: the caller hoists it to a local first. A fixed-array local in a body hoists
  to a program-scope constant table only when every element is a scalar integer or float literal; a
  vector literal does not hoist, so a `float4` table is spelled as four scalars a word.
- **A home's lane primitive is reached by one name.** A template method resolves in the module that
  stamps it, so a shared reduction calls `gk_subgroup_add` / `gk_subgroup_max`, and each kernel home
  defines the pair over its own primitive (`subgroupAdd` on Vulkan, `simd_sum` on Metal). A free function
  in the common module resolves in the common module and sees neither home, so a body that needs a
  home's primitive keeps it in a method.
- **A binding a stamp does not use is gated, and a stamp names its element types.** `@template_gate`
  erases the field at stamping (`daslib/typemacro_boost`, before either emitter reads the class): a gated
  f16 output or bias row is no parameter of that stamp's builder (`GkRestride`). A body writing one output
  in two precisions spells its element type as a `typedef` each stamp declares (`OT`), which both emitters take.
- **A number the drivers verify is a `let` beside the template, and a scale both homes must round
  alike is computed on the host.** A template constant's default is a literal, so the window a kernel is
  stamped for is spelled twice - the default and the `let` each driver checks against (`GK_G4A_ATTN_PAST`).
  A shape-derived scale (`gemma4a_attn_scales`) is passed in the arguments: in the kernel, the homes' intrinsics round it apart.
- **What stays per home:** the GEMM and attention tiles, which each home builds on its own matrix
  primitives. A pair whose two forms differ in shape (the TTS elementwise maps run one element a thread on
  Metal and four an invocation on Vulkan) folds onto one body when both homes profile the same on it, and
  until then shares its per-element function through `dasllama_gpu_math.das`.
