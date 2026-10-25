# dasLLAMA Architecture - GPU backends

Companion to `ARCHITECTURE.md`; a section is cited by its anchor. This document carries the GPU backend role table with the kernel-binding asymmetries a stamp family ledgers. The closed list of the allowed asymmetries between the backends is
`ARCHITECTURE_GPU_ASYMMETRIES.md#gpu-asymmetries`. The companions beside it carry the sections built on it, and each one's opening routes to its own companions:
`ARCHITECTURE_GPU_RACE_SHAPES.md` (the tensor-GEMM and fused-attention shapes that measured
out); `ARCHITECTURE_GPU_TOWER.md` (the Metal tower's attention routes and encode chains and the
StyleTTS2 chain); `ARCHITECTURE_GPU_TOWER_POCKET.md` (the Metal tower's Pocket TTS seats); `ARCHITECTURE_GPU_QUANT_PLANES.md` (the Metal quant
plane reads and the GEMV site abstraction); `ARCHITECTURE_GPU_BATCH.md` (the per-layer FFN widths and the Metal batch
driver's row shapes, its split single-pass attention included); `ARCHITECTURE_GPU_MTP.md` (the Metal speculative
round, the depth a round drafts, and the verify and batch-driver mechanics); `ARCHITECTURE_GPU_MTP_ASSISTANT.md` (the assistant drafter on both homes);
`ARCHITECTURE_GPU_KV_RING.md` (both homes' sliding-layer K/V ring); `ARCHITECTURE_GPU_SHARED_KERNELS.md` (the
kernels more than one home stamps: the template mechanism and the pair ledger); `ARCHITECTURE_GPU_SHARED_READINGS.md` (what
the shared stamps read on their homes); `ARCHITECTURE_GPU_SHARED_GEMM.md` (the GEMM tiles and GEMVs both discrete homes stamp); `ARCHITECTURE_GPU_SHARED_ROWS.md` (the row kernels of the token step and the routed block the homes share); `ARCHITECTURE_GPU_SHARED_ATTN.md` (the attention kernels and K/V block codecs both discrete homes stamp); `ARCHITECTURE_GPU_VULKAN.md` (the Vulkan resident driver).

### GPU backends {#gpu-backends}

A GPU backend is a FAMILY of role files - matching things in matching files across backends, so
that a question answered for one backend has an obvious address in the other. A backend's entry
(`dasllama_math_vulkan`, `dasllama_math_cuda`) registers the tier's seats at `[init]`; one entry
fills them a process - under `DASLLAMA_CUDA` the CUDA home's entry takes the seats and the Vulkan
entry stands down, otherwise the Vulkan entry takes them and the CUDA entry registers nothing. The
knob reads as off in a build without dasCUDA (`gpu_want_cuda` is `false` where the `cuda` witness
module is absent, one warning said): the engine never requires the CUDA home there, and the Vulkan
entry keeps the seats rather than standing down for a home that does not exist. The
resident rail's override key stays the literal `"vulkan"` under either home: it names the whole-model
tier, not the backend serving it. The roles:

| role | holds | must not hold |
|---|---|---|
| the kernel home<br>`dasllama_metal_kernels`, `dasllama_vulkan_classes`, `dasllama_cuda_classes` (CUDA: every kernel a `[cuda_kernel]` / `[cuda_dispatch]` class stamping a shared template under the Vulkan home's dispatch stems, so the format ladders read alike; the `gk_subgroup_*` shims both discrete homes fold through are the discrete file's) | kernel source, the threadgroup reductions every Metal body folds through (`tg_sum_all` / `tg_max_all`, `MetalTgReduceBase` over the shared `GkWgReduce`'s slab), the workgroup folds every Vulkan body whose fold is one value derives (`WgReduceBase`'s `wg_sum` / `wg_max` / `wg_rms_inv`, one value over every lane, returned to every lane - the value form of the shared `GkWgReduce` in `dasllama_gpu_kernels_common`, which a kernel both homes stamp derives directly) - one Vulkan body folds by hand by design, because its fold is split: `DnStepFused`'s q and k halves (`subgroupAdd` into per-subgroup arrays, a lane-0 loop over them, one set of results every lane reads), the kernel-side quant-decode helpers and per-word codebook accessors only this home splices (`[grid_words]` bakes each accessor from `dasllama_kqformat`'s one literal at compile time - the bytes live there, the home carries the baked copy; the table-free arithmetic the homes share is `dasllama_gpu_math`'s, and a helper or accessor both discrete homes splice is the discrete layer's), the derived-access/PSO census; on Metal the host encode wrappers over its own PSOs that both Metal drivers call - `dasllama_metal_common` sits below this file, so no host file above it is shared - the residual step `enc_residual` with its seams (`ResidualSeam`, `seam`, `seam_norms`), its one dispatch `enc_residual_fused` (the decode rows' 1024-lane pick `residual_wide`) and its split chain `enc_residual_split` with the chain's norm `enc_rms_form`, which the residual bench dispatches too, the table gather `enc_embed_table` its own `enc_embed_dev` picks, the gemma-4 MoE step's args `moe_g4_layer_args`; on Vulkan the one device buffer kernel data fills (`kq_grid_dev`, the grid codebooks) and the host-side ensure/set/enc pick ladders and grid rules over its own class stamps (`gemv_*`, `q8_gemv_gu_n_*`, `q8_batch_cls_*`, `kq_batch_cls_*`, `fa_stamp_*`, `f16_gemm_*`, `router_enc_cols` (the router GEMV's f32 rows, bf16 word pairs and bf16 halfwords, by `gk_bf16_pairs`), `da_slab_*`, `da_attn_stamp_*` with the block codecs' `kv_codec_cls_ensure`, `da_attn_b_stamp_*`, `rope_kv_b_stamp_*`, `emb_cls_*` (the token-table gather), `topk_*` and `moe_sched*` (the routed top-k and the expert schedule, each a depth-one or depth-two stamp picked by the expert count), and the tile trios `cm2_cls_*` (the engine's, over every tile tail) and `khr_cls_*` (the kernel cells' KHR arm); `kq_tile_stamp` stamps the tile trios, `kq_batch_cls_*`, the GEMV ladders (its `gemv` family) and the gather's (its `emb` family) over `KqFmt`, `stamp_ladder` the `fa_stamp_*` and `da_attn_stamp_*` ladders over their stamp keys); the tower row classes' mode selectors and family constants - the shared bias rows' act code (`GkBiasActArgs.act`, the `BIAS_ACT_*` codes `dasllama_gpu_math.das` carries, relu for the subsample stacks) and `Q3A_TOK_PER_CHUNK`, `GK_G4A_ATTN_PAST`, `G4A_ATTN_CAP`, `WDEC_HS`, which the drivers check against the family before they serve | device state other than `kq_grid_dev`, engine types |
| the discrete kernel layer<br>`dasllama_gpu_kernels_discrete` | the kernel bodies the Vulkan and CUDA homes stamp and Metal does not - the decode GEMV family over every `KqFmt` with its decode helpers and the per-word codebook accessors both homes read, the token step's row families (the residual step, the requants, the token-table gather, the classifier pick, the decode attention, the rope and K/V mirror stores) with their args structs and the token block `TokMeta`, and the prefill's batch GEMM tiles (the cm2 tiles over every `KqFmt` and the region rail `MoeCmBase` they share) and flash attention (`FaT` and its head shapes, the batched mirror passes), as class templates under the `dasllama_gpu_kernels_common` contract (`ARCHITECTURE_GPU_SHARED_KERNELS.md#gpu-shared-kernels`); a template stamped by one home carries a followup row naming the family that brings the second stamp | a kernel method, a dispatch annotation, device state, a tile class only one home can run (the Vulkan home's KHR f16, mm and per-32 sdot4 roads for cards without the cm2 extension live in its classes file; a `static_if` arm of a template both homes stamp, such as the cm2 tile's KHR axis, is the template's) |
| `dasllama_<gpu>_common`<br>`dasllama_metal_common`, `dasllama_vulkan_common`, `dasllama_cuda_common` | device state, buffer/command plumbing, hazard + capture rail, profiler, host-side quant-decode helpers (Metal's `iq4_lut`), the family's registrant of a tier seat that names a size the device state keeps (Metal's `dn_mirror_room`); on CUDA the context and stream (`CudGpu`), the class-kernel rail the lens's generated code calls (`CudClass`: a function a dispatch ordinal, loaded from the shared cubin cache; `cud_launch`, whose parameter blob is the buffer addresses in binding order then the args struct - a CUDA kernel has no sets and no hazard rail) and the launch census | driver policy |
| `dasllama_<gpu>_decode`<br>`dasllama_metal_decode`, `dasllama_vulkan_decode`, `dasllama_cuda_decode` | the resident token-step driver + decode-time arms; on CUDA the dense slice alone - a dense model's planes in one arena a format (Q8_0, the superblock K-quants, the dense per-32 formats q51 / q5032 / q4032 / iq4nl32, NVFP4's nv4 planes with the per-tensor factor in each scale row's pad word; the token table gathered through its format's leaf), a mirror region a stream, the token step's chain over one row or the N-row command's rows (the Vulkan home's one recorder over both commands, `ARCHITECTURE_GPU_VULKAN_NROW.md#nrow-token-command`: every GEMV an N-column dispatch, every row kernel a row a workgroup, each row's position, count and region in its token block) captured once a row count and mode (the logits down, the rows gathered off their ids, the attention's unsplit form under the unsplit position - the Vulkan home's unsplit twin, `ARCHITECTURE_GPU_SHARED_ATTN.md#vk-decode-attn-split`) into a CUDA graph and replayed (the rows, their rope rows and token blocks ride pinned host buffers a words-copy kernel reads into the device planes; the picks land in the mapped out block from the argmax's own store, the logits through a copy-engine node; at one row the gate and up GEMVs of one format run as one two-region GEMV (`CudLayer.gu_merged`: an arena holds one format, so a shared format puts both planes in one arena) that lands [gate | up] in the gate buffer, sized two rows at the least, and the act reads its up side `hid` past the gate's - the N-row command keeps one GEMV a plane, whose y rows sit `hid` apart a column; the Vulkan home's merged form is `ARCHITECTURE_GPU_VULKAN_DECODE.md#hybrid-token-command`), the prefill as the Vulkan home's window chain over the f16-fed cm2 tiles, or over the home's own MMQ leaves (`MmqT`'s int8 stamps a format and the FP4 leaf `MmqFp4T` for nv4, the `cuda_intrinsics` tier), picked a site at a time (`crd_site_mmq`) where the site's format has a leaf and its shape fits the tile, under `DASLLAMA_CUDA_MMQ` (`ARCHITECTURE_GPU_ASYMMETRIES.md#gpu-asymmetries`, the int8 MMQ entry) - one session's rows, or several sessions' chunks as the segmented window (`ARCHITECTURE_GPU_VULKAN_WINDOW.md#vk-prefill-segments`) - (a window of the engine's prompt rows at a time; a GEMM's region schedule is a device buffer a role a layer; a grid that fills the card's two blocks an SM runs whole - at full load a block's time grows faster than its piece shrinks on this home - and a shorter grid splits k until it fills it, the l column first: the k/v, q/o and down shapes of a 512-row window launch 8 to 32 tiles on 36 SMs, and whole they left the card half idle), and a decline by name for every feature outside the slice (a recurrent or routed layer, a NextN head, a gated or biased attention, a plane format outside the arena set (mx4)); a q/k-normed model's norm, rope and K/V store run as the fused pass `CuQknRopeKv` in the token command and the window alike (the Vulkan home's `QknRopeKvT` stamped for this home); every device copy and clear the home issues names `g_cud.stream`, because that stream is non-blocking and a stream-less call runs on the legacy stream, which never orders against it - a stream-less clear of a fresh block lands after the first window's writes into it and zeroes a prefill's K/V rows (`REVIEW.das`'s `check_cuda_stream_discipline` walks `dasllama_cuda_*.das` and `dasllama_math_cuda.das` for a `CUDA_STREAM_OPS` call - `cuda_memset_d8`, `cuda_memcpy_htod`, `cuda_memcpy_dtoh`, `cuda_memcpy_dtod` - whose last argument is not `s`, `stream` or a `.stream` field, each the launch's own stream; it licenses no names); its block-codec mirrors run the Vulkan home's codec shape on the shared stamps - the rope stages the floats, the codec's store quantizes them into the mirror (tq4's rotating the q heads in place, under the emitter's `precise` option so its bytes are the CPU's), the decode attention's block stamps score the blocks (tq4's un-rotation after the pass, ahead of the row's requant), the window chain attends the f16 shadow `KvShadowT` dequantizes (`ARCHITECTURE_GPU_SHARED_ATTN.md#vk-kv-block-codecs`); on Vulkan, for a model with routed experts on the host (a hyper-connection MoE, `Config.hyper_conn`, or a plain MoE's host-layers form), the split token command and the hot pool's device slots, stacks and hit chains (`ARCHITECTURE_GPU_VULKAN_HC.md#hc-token-command`, `ARCHITECTURE_GPU_VULKAN_POOL.md#hc-hot-pool`) | kernel bodies |
| `dasllama_<gpu>_prefill`<br>`dasllama_metal_prefill`, `dasllama_vulkan_prefill` | the batched prefill driver + batch arms; on Vulkan, for a model with routed experts on the host, the window chain cut after each router (`ARCHITECTURE_GPU_VULKAN_HC.md#hc-window-chain`) | kernel bodies |
| `dasllama_<gpu>_shapes`<br>`dasllama_metal_shapes` | PORTABLE servability gates - no GPU C++ require, so any box can bake | device calls |
| the tower driver<br>`dasllama_metal_tower`, `dasllama_vulkan_tower` (the vision ViT chains on both weight lanes - the q8 image on the q8 GEMM, the f32 planes on the f32 tiles - qwen25v's over the halfword twin, and the audio chains over the q8 image - the whisper-class towers with their conv stem, the gemma4a Conformer with its chunk front and projector tail, the canary FastConformer with its front, the qwen3a front and the whisper mel seat; on a build without das_metal it fills the gemma4uv, gemma4v, gemma3v, qwen3v and qwen25v hook slots whole through `register_gemma4uv_gpu`, `register_gemma4v_gpu`, `register_gemma3v_gpu`, `register_qwen3v_gpu` and `register_qwen25v_gpu` (`ARCHITECTURE_GPU_TOWER_VULKAN_VISION.md#vk-vision-chains`) and the audio seats `register_tower_blocks_gpu`, `register_tower_blocks_ln_post_gpu`, `register_tower_conv_gpu`, `register_qwen3a_front_gpu`, `register_whisper_mel_gpu`, `register_gemma4a_gpu`, `register_gemma4a_chunk_gpu`, `register_canary_gpu` and `register_canary_front_gpu`; parakeet's `register_parakeet_gpu_encode` - both homes register them) | one-shot embedder/encoder encodes (gemma4uv chain, the gemma4v ViT with its patch-conv stem and projector tail, gemma3v SigLIP with its patch conv and projector tail, and qwen3v block loops - qwen3v adds the vision NEOX rope, the fused-qkv weight-offset GEMMs, and the inline deepstack tap + tail merger chains - the whisper-class block loop with its post-norm or its projector tail behind it, the qwen25v window ViT, the gemma4a Conformer chain with its mel/conv front, the FastConformer chain canary and parakeet share - one block body over `CanaryLayerOffs`, parakeet's offsets mapped onto it with no GEMM biases and the tap-major depthwise stamp - the Pocket TTS codec, frame loop and text prompt (`ARCHITECTURE_GPU_TOWER_POCKET.md#tower-pocket-codec`, `ARCHITECTURE_GPU_TOWER_POCKET.md#tower-pocket-frames`) - the whole StyleTTS2 synthesis (kitten and kokoro on both weight lanes: seven seats from PL-BERT to the inverse STFT, the front end on the f32-exact GEMM stamp, the source chain the CPU's operation for operation, `ARCHITECTURE_GPU_TOWER.md#tower-tts-chain`) - the conv frontends + the qwen3a padded-weight slab and GPU front/mel) - no session, no mirror - the Pocket frame loop's per-voice device K/V slot is the one state kept across calls; registers the gemma4uv, gemma4v (`register_gemma4v_gpu`: the chain takes the image planes and runs the stem, the blocks and the tail), gemma3v, qwen3v, qwen25v, whisper-class blocks (`register_tower_blocks_gpu`, `register_tower_blocks_ln_post_gpu` with the post-norm behind them, `register_tower_blocks_tail_gpu` with the projector tail behind them), tower-conv, qwen3a-front, whisper-mel (`register_whisper_mel_gpu`, the spectrum of the qwen3a mel and of the chat towers' chunked mel), gemma4a, gemma4a-chunk, canary (`register_canary_gpu` for the blocks, `register_canary_front_gpu` for the front), parakeet (`register_parakeet_gpu` for the blocks, `register_parakeet_gpu_encode` for the whole encode with the subsample front) and StyleTTS2 (`register_styletts2_gpu`, the seven-seat record) and Pocket (`register_pocket_gpu`, the codec, frames and prompt seats, `ARCHITECTURE_GPU_TOWER_POCKET.md#tower-pocket-codec` and `ARCHITECTURE_GPU_TOWER_POCKET.md#tower-pocket-frames`) hooks; the whisper-class block GEMMs and its conv stem's second GEMM on a q8 tower borrow `pf_enc_q8_mm`, the projector tails borrow `enc_tw_pool2` and `enc_tw_swiglu_rows` (whisper-class), `enc_tw_pool2d` (gemma4v, gemma3v) and `enc_tw_affine_rows` (gemma4v), the canary front borrows `enc_cn_melnorm`; the FastConformer chain borrows `enc_fc_pack` / `enc_fc_softmax` / `enc_fc_unpack` around `enc_f32_mm`, `enc_cn_dw` / `enc_pk_dw`, the prefill's `pf_enc_fc_attn_dev` (every head's attention at once on the device pair, under its crown) and the parakeet front's `enc_pk_dw2d` / `enc_pk_xf` (its first conv rides `enc_im2col2d`, the f32 GEMM and the bias rows), and the `enc_st2_*` row, LSTM, attention and source kernels around `enc_st2_conv_mm` / `enc_st2_conv_exact_mm`, the `enc_pk_*` row, add-and-norm, attention-row and prologue/epilogue GEMV kernels for the Pocket codec and frame loop beside the decode rail's `enc_gemv` / `enc_rpst32_c` and the prefill's `enc_rope`; on Vulkan the whisper-class encoder-output handoff - the served post-norm chain's `xb` plane, kept for the encode in flight alone, which the ASR-decoder driver's cross-KV chain reads (`vulkan_tower_enc_out`) - is the second state kept across calls beside the Pocket slot | LLM or ASR decoder session state |
| the ASR-decoder driver<br>`dasllama_metal_asr_dec`, `dasllama_vulkan_asr_dec` | the whisper decoder on the GPU: Metal's 34B weight blob or Vulkan's row-major q8 gather, the f16 resident cross/self K/V, window-granular cross-KV + decode-step serves; registers the whisper cross-KV and decode hooks (`register_whisper_cross_kv_gpu`, `register_whisper_decode_gpu`; family registries in `dasllama_whisper`); on a build without das_metal the Vulkan driver fills them (`ARCHITECTURE_GPU_TOWER_VULKAN.md#vk-asr-decoder`) | kernel bodies, LLM session state |
| the TTS driver<br>`dasllama_vulkan_tts` (Vulkan; the Metal tower driver's own row carries the TTS seats on an Apple build) | the StyleTTS2 synthesis seats and the Pocket codec, frames and prompt seats on the tower's knob - the f32 weight slabs the shared host writer (`dasllama_tts_slab`) lays out, the front end on the f32-exact tile GEMM, the seats registered through `register_styletts2_gpu` and `register_pocket_gpu` on a build without das_metal; the seats serve only while the tier's want arms the device (`DASLLAMA_GPU=1`, off by default), so a run with no flags and no environment overrides synthesizes on the CPU chain on a Vulkan build, where on an Apple build the Metal tower driver's TTS seats serve by default (`DASLLAMA_METAL_TOWER` on) | kernel bodies, the CPU chain's arithmetic (the CPU chain is the specification), LLM or ASR session state |
| the assistant-drafter driver<br>`dasllama_metal_mtp_gemma`, `dasllama_vulkan_mtp_gemma` | the gemma-4 assistant drafter on the GPU: the sidecar's q8 weights on the device (Metal's 34-byte blob as it lies, Vulkan's quant and scale planes), the Q-only layer chain reading the TARGET mirror at the two capture layers with the decode's own attention kernels, the speculative rounds of a tick's streams over one verify (Metal's batch driver in same-slab groups, Vulkan's N-row token command); registers the backend's `MtpAssistantSeams` (`register_mtp_assistant`), the sidecar resolver and the CPU oracle staying in the backend-free `dasllama_mtp_gemma` | kernel bodies, mirror ownership |
| the kernel-access lens<br>`dasllama_metal_lens` (Metal), `dasllama_vulkan_dispatch` (Vulkan - the `[vk_dispatch]` macro derives access per class), `dasllama_cuda_dispatch` (CUDA - `[cuda_dispatch]`, the same derived access over a `[cuda_kernel]` class, generating `ensure_<name>` and `enc_<name>` over the `cud_` rail) | the kernel-access macro and its dispatch-support macros (`compile_stamp`, `release_handles`) | anything else |

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
  `tts`, `mtp_gemma`; CUDA's `classes`, `common`, `dispatch`, `decode` - and finds a
  `dasllama_<backend>_<role>.das` with any other role.
- **`REVIEW.das`'s three shared-file checks hold the discrete and common kernel files to this table.** `check_shared_template_homes` finds a `class template` of `dasllama_gpu_kernels_discrete.das` that the Vulkan and the CUDA homes do not both stamp, and one of `dasllama_gpu_kernels_common.das` that the Metal and the Vulkan homes do not both stamp, a stamp counted through any chain of intermediate templates; it licenses the names in `DISCRETE_ONE_HOME_STAMPS` - discrete templates whose CUDA stamp waits on a family the CUDA home's dense slice does not serve yet, each named in `followup_general.md` item 226 - and finds a licensed name once both homes stamp it. `check_shared_file_spelling` holds the two shared files and `dasllama_cuda_classes.das` to `spirv/spirv_markers` as their one `spirv/` require, and finds a `gk_subgroup_*` definition outside the discrete file and the Metal kernel home; `check_shared_file_cites` finds an `[arch]` citation in a shared file that names a document carrying a home's name (`VULKAN`, `METAL`, `CUDA`). Those two license no names.
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
  on the driver alike (its `serves` answer to their lane policy is no); the vision chains serve both lanes with the q8 image as the policy's default (the f32 tile's cost), where Metal's prefer the file's planes. Likewise the
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
  lists, one per direction, every home registering one gate in each: `register_ple_gpu_gate` for a
  prefill override that builds the side input on device off the stashed token ids,
  `register_ple_gpu_decode_gate` for a decode override that builds it off the step's own ids; a build
  carries both homes, each gate answers for its own armed driver, and the hub skips the CPU pre-step
  for the call a gate takes. Both homes gather the side rows through the token-table gather family at
  the width layers x ple under sqrt(ple) (Metal's `enc_embed_table`, Vulkan's `emb_cls` stamps), project
  the embedded rows off the file's own bf16 rows on the bf16 word form of `GkRouterGemv` under the
  1/sqrt(dim), and finish them on device at every decode shape and the window, Metal's side rows
  position-major; Vulkan's token commands gather each row off its id through one id plane (the
  one-row and N-row commands and the short-window peel), and only a prompt whose rows the host
  embeds (media) gathers its side rows on the host. qwen4exp's n-gram
  side input splits the same way with no gate: the CPU pre-stack hook gathers the token's hashed heads (the 28 GB table stays host-side), the Metal decode projects, gates and convolves them (`ARCHITECTURE_GPU_MTP_DECODE.md#metal-layer-enc`).
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
  keep the pre-family compact kargs slots while their base classes bind the family numbers; the MoE combine pair (`MetalMoeCombine` y/dim/nk at
  2/3/4, `MetalMoeReduce` at 3/4/5 under its gated `inv`) keeps each leaf's numbers; the in-engine moe mul_mm A/B race harnesses
  (`dasllama_metal_prefill.das`) encode through `kn_moe_mm_family_tail` rather than a per-class
  `enc_*` builder; the iq4 family's iq4nl stamps (`MetalKqGemvIq4T`, `MetalKqMvIq4T`, `MetalKqMvB8Iq4T`) and the mul_mm tensor
  template's compact-scale stamps off the same family (`MetalKqMulMmIq4xsTensorT` at IQ4NL, the dense `MetalKqMulMmIq4nlT` / `TH`
  and `MetalKqMulMmQ40T` / `TH`, the four `MetalMoeMulMmQ40*` expert stamps) bind the strip plane unread, so both formats share
  one set layout and one host bind path, and the family's nv4 stamps (every class at `NV4`: the GEMV and routed GEMV, the
  small-batch forms and their half-x twins, the simdgroup and routed base tiles, the tensor twins, the dequant pass, the
  token gather) bind the d plane unread instead - the sixteen UE4M3 bytes are the strip view - so nv4 keeps the same layout and
  path; every dev-W dequant stamp (`MetalKqDequant<Fmt>`) inherits the mul_mm scaffold's
  `xf`, `y` and `ndim` bindings unread (it writes only the f16 panel at binding 7) and the per-32 tensor template
  (`MetalKqMulMmB32TensorT`) its `ksc` strip view unread (a per-32 scale plane has no strips), so every dense format's
  dequant pass and twins keep one set layout and one host bind path (`pf_enc_kq_dense_site`); the MoE tile lab's knockout stamps (`LabMoe*Ko*`,
  `benchmarks/matmul/bench_metal_moe_tile_lab.das`) keep the full bind lists, the dropped stage's buffer unread; and the Vulkan
  `kq_gemv_cls` family binds `gridb` (the grid formats' codebook plane, `kq_grid_dev`) at binding 6 on
  every stamp, one set layout for the family - the grid stamps (iq2xxs, iq2xs, iq2s, iq3xxs, iq3s) and
  their N leaves read it; every other stamp (k2 k3 k4 k5 k6 q40 iq4xs iq4nl, iq3s4 - whose codebook is arithmetic - and the fused `kq_gemv_k4_gu_cls`) binds it unread;
  and the `GkQ8BlockStore` shell's gated `OUT_SUM` plane `outsum` (each block's scale x quant sum, the int8 MMQ leaves' min
  term) sits at binding 13 under `GkAddRmsRows` and at 3 and 4 under the discrete `RqPlainT` and `ActRqT`, as the shell's
  `outq` / `outs` pair already sits at 3 / 10 and 6 / 7 - each stamp's bindings follow its own class's layout, the shell's
  fields taking the numbers that class leaves free.
- **`dasllama_gpu_tier.das`** - the device-cooperation SPI: hook types, install/unset slots, route/mark/want/status
  state, engine-facing forwarders, the backends' shared kernel-coverage census log (`kernel_coverage_report`), the layout both discrete drivers read - the norms plane's per-layer rows `RDEC_NORM_*` with their offset `rdec_norm_off`, the residual step's row-stash bound `RDEC_AR_MAX_DIM` and the slack past each K/V mirror plane sized from it (`RDEC_MIR_SLACK*`), a region's mirror base `rdec_mir_region_base`, and the window's split into its segments' parts `rdec_window_segs` with `rdec_seg_ending_at` - and `arm_say`, a tier entry's arm line said once an outcome under the noisy gate. Vulkan implements it (per-op offload plus resident plumbing, and the decode-era
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
  host, `ARCHITECTURE_GPU_VULKAN_POOL.md#hc-hot-pool`), the prefill seats' `experts` argument a plain MoE's cut prefill window asks
  for the host's routed sums through (the hyper-connection window chain's twin), its mirror-region seat `install_moe_gpu_resident_regions`
  (`rdec_select_region` names the region every mirror address and the next token command resolve against; a tier
  without it serves one region), its N-row batch seat `install_moe_gpu_resident_batch` (the N-row token command beside
  the resident driver: `rows` answers how many rows it steps at once on the armed model, 0 = none; `rdec_token_n` steps
  them), all behind the route lever `set_gpu_resident_route` / `gpu_want_resident`, the streamed layers' seats
  both homes fill - the classifier tail's selection landing `install_rdec_select` (`ARCHITECTURE_GPU_VULKAN_LANDING.md#logits-selection-landing`, `ARCHITECTURE_GPU_MTP_DECODE_PICK.md#metal-selection-landing`) - and the streamed layers' seats
  the Vulkan family alone fills - the pool's DMA fill `install_moe_gpu_resident_hot_dma`, the pool's placement batch
  `install_moe_gpu_resident_hot_batch`, the meet's device/host claim
  `set_moe_gpu_ffn_xf_meet_hooks`, the lane's stream sources `set_moe_gpu_stream_source_fn`, the host twin
  `set_moe_gpu_host_twin_fns`, the checkpoint row's ring `install_rdec_dn_snap_ring`, the slot's carve `set_moe_gpu_carve_fn`,
  the expert cache's view, order, stats, slot and request seats `set_moe_gpu_stream_view_fn` / `_cache_order_fn` /
  `_cache_stats_fn` / `_cache_fns` / `_request_fns`, the pool window `set_moe_gpu_xf_pool_window_fn`, the dry mint
  `set_moe_gpu_mint_dry_fn` and the host-bytes census `set_moe_gpu_host_bytes_fn` (`ARCHITECTURE_GPU_VULKAN_STREAM.md`,
  `ARCHITECTURE_GPU_VULKAN_POOL.md`) - the
  OS video-memory seat `install_moe_gpu_os_memory` the residency plan sizes against, the weight-bytes seat
  `install_rdec_note_weight_bytes` the decode warm-up guard reads, the segmented prefill `install_rdec_prefill_segs` (`ARCHITECTURE_GPU_VULKAN_WINDOW.md#vk-prefill-segments`), the next call's checkpoint captures `install_rdec_prefill_caps` (`ARCHITECTURE_GPU_VULKAN_WINDOW.md#vk-prefill-dn-block`), the speculative carry's landing `install_rdec_carry` (`ARCHITECTURE_GPU_VULKAN_LANDING.md#logits-transfer-queue`), the NextN draft head's seat `install_rdec_draft` (`ARCHITECTURE_GPU_VULKAN_MTP.md#resident-draft-head`), its prompt warm on the window chain `install_rdec_head_warm` (the ask and the rows written; the head's K/V row ranges move up and down through the resident driver's two K/V row seats, `rdec_kv_sync` / `rdec_kv_read` (filled by `install_moe_gpu_resident`'s `kv_sync` / `kv_read` arguments), at layer `n_layers`; `ARCHITECTURE_GPU_VULKAN_MTP.md#resident-head-prompt-warm`), the same-slab verify's `install_rdec_verify` (`ARCHITECTURE_GPU_VULKAN_NROW.md#nrow-verify-command`), the reject's device rollback `install_rdec_rollback` (`ARCHITECTURE_GPU_VULKAN_MTP.md#resident-verify-rollback`), and the per-layer-embedding seats `install_rdec_ple`
  - the branch's width, its per-layer gate and proj planes, the pre-step's projection and the token table on the
  device), the short window's token sequence `install_rdec_token_seq` (`ARCHITECTURE_GPU_VULKAN_WINDOW.md#vk-prefill-peel`), the greedy chain `install_rdec_chain` (`ARCHITECTURE_GPU_VULKAN_LANDING.md#vk-greedy-chain`), the device-home recurrent checkpoints `install_rdec_dn_snaps` and their read-back `install_rdec_dn_peek`, the host pool's batched K/V row move `install_rdec_kv_xfer` (`ARCHITECTURE_ENGINE_SERVING.md#device-home-pool`), and the routed experts' gate/up bias planes `set_moe_gpu_ffn_bias_hook`. The installs are one-way (a test that arms the tier never restores them) but for `unset_rdec_draft`, the CPU
  draft's control, which answers the seat for `reinstall_rdec_draft`; a seat serves whatever model loads next. Metal deliberately does not - UMA makes residency moot there, and
  Metal integrates as a whole-forward driver through common's override registries (the ASR-decoder driver is the one
  exception: whisper is not a `Model`, so its hooks are family registries in `dasllama_whisper`, same decline contract).
- **`dasllama_gpu_resident.das`** - the WHOLE-MODEL residency rail, and the Model's per-format plane-pair accessors (`kq_plane_pair` / `kq_plane_pair_names`) the rail's walk and the mint's section names both read: bake the device layout offline into the flavor
  image, upload a model's stacks to the tier, drive decode/prefill entirely on device, record the routed pick trace
  (`DASLLAMA_MOE_TRACE`, the pool's offline instrument), move the host pool's rows for the device-home regions - the trunk's and the draft head's (`ARCHITECTURE_ENGINE_SERVING.md#device-home-pool`) - and decide the trimmed lane -
  its admission (`resident_would_serve`) and the emb region the CPU keeps (`trim_pack_emb`). It is device-AGNOSTIC - it
  holds no device call and requires no GPU module, reaching the hardware only through the `dasllama_gpu_tier` SPI and
  entering the engine only through common's override registries. `"vulkan"` is the tier string it registers under, not
  a dependency, which is why it compiles on every box. It requires common back for `Model`/`Session`, so like the
  Metal drivers it is required from the transformer umbrella, never from common; it requires `dasllama_moe` and
  `dasllama_blocks` for the routed sums it runs on the host. On a hyper-connection MoE it keeps the routed experts
  on the host and sums them between the command's segments (`ARCHITECTURE_GPU_VULKAN_HC.md#hc-token-command`),
  and it owns the hot expert pool's POLICY - which experts sit in the tier's slots, which picks the device serves
  (`ARCHITECTURE_GPU_VULKAN_POOL.md#hc-hot-pool`); the pool's device side is the tier's four hot seats.
- **A dry bake runs the whole resident arm with no device.** `vulkan_bake_role` puts the tier in
  bake mode, and each `rdec_*` device seam answers for itself so the arm walk reaches the end: a
  seam that only records a layout (`vk_rdec_set_emb`) answers true, one that would upload a device
  mirror of its own (`vk_rdec_set_ple_proj`) answers false without touching a device - which keeps a baked `.dlim` layout equal to the one a real arm produces.
- **`dasllama_gpu_math.das`** - the ALU helpers both kernel homes splice into their shader bodies
  (`ksign7`, `iq3s_signed`, `softcap_exp`, the MXFP4 scale `e8m0_half_f32`): pure arithmetic, no table, no backend lowering, so one
  owner serves the Metal and the Vulkan bodies alike; it also holds the CPU chains' matching scalar forms (the sigmoid, the SiLU, the logistic gate, the softplus, the deltanet out-gate, the routed renorm's floored sum, the inverse rms in either spelling: `sigmoid_f32`, `silu_f32`, `sig_gate`, `softplus`, `dn_zgate`, `moe_renorm_sum`, `rms_inv`), and the sliding-window K/V ring's address (`ARCHITECTURE_GPU_KV_RING.md#kv-sliding-ring`): `kv_ring_row` in its shader form both homes' kernels splice and its host form for the row movers and the plan, and `kv_ring_runs`, the host's walk of a position span as runs of ring rows; the host-side launch picks both discrete drivers make alike - the cm2 tile column by whole waves of the card's slots (`cm2_wave_col`, `cm2_wave_units`), the tiles a GEMM takes at a column (`cm2_tiles_at`), the k split that fits the partials scratch (`cm2_split_fit`, `cm2_chunk`), the GEMV and residual-epilogue grids (`gemv_wgs_of`, `ar_epilogue_wgs`), the decode attention's key splits a head (`da_nsplit` under `DA_NSPLIT_MAX`), the position under which the token step runs the attention's unsplit form (`RD_UNSPLIT_POS`) and a block-codec shadow's first key under a sliding window (`kv_shadow_pos0`); the codebook tables stay per kernel home.
  `mad` is the fused instruction by definition and leaves a driver nothing to choose, so two bodies
  held bit for bit spell as `mad` each multiply that feeds an add.
- **`dasllama_gpu_kernels_common.das`** - the kernels both kernel homes stamp, each one `class template`
  with its buffers, its argument struct and its body (`ARCHITECTURE_GPU_SHARED_KERNELS.md#gpu-shared-kernels`); it requires no backend.
  It also holds the table-free helpers outside any class that both homes' kernels splice that carry `[spirv_inline]` (the bf16 pair-word
  widenings `gk_bf16_lo` / `gk_bf16_hi` / `gk_bf16_at`, the strided side-row index `gk_strided_at`): the marker is SPIR-V's
  spelling from `spirv/spirv_markers`, the emitter-free module this file and the discrete file require (never the emitter
  behind `spirv_shader`), and `dasllama_gpu_math.das` carries no backend lowering - every CPU chain requires it.
  One family's branch rides a shared kernel here: the routed top-k select's (`GkTopKRow`) per-expert down-scale arm
  (`DSCALE`, with `DSOFF_SWITCH`) is gemma-4's MoE - `Config.moe_dense_shexp` with the file's `ffn_down_exps.scale`
  plane (`Model.moe_down_scale_off`), which folds into the picked weights; every other family's stamp or `GK_NO_DSCALE`
  leaves the arm off.
- **`dasllama_kernel_access.das`** - the shared body-walk read/write classifier both GPU lenses run
  on, plus the dispatch-lens micro-grammar (the grid/tg/params spec tokenizers and the shared
  AST-emission core: `is_digit_tok`, `role_ok`, `derived_role`, `mk_uint_cast`, `mk_call1`,
  `mk_grid_dim`, `param_type`, and `kargs_as`, which gives a shared kernel's `@kargs` member the argument
  form each lens's emitter reads). One owner by design: a private copy per lens drifts (metal folding
  only the literal "1" where vulkan folds any integer). Backend-specific lowering stays in that backend's lens.
- **`dasllama_dispatch_scan.das`** - the member scan the two discrete lenses (`[vk_dispatch]`, `[cuda_dispatch]`) share, each refusal in the calling lens's words (`LensWords`): the annotation's arguments and the family ordinal (`lens_scan`), the `@ssbo` / args / `@workgroup` member walk and the distinct bindings, the kernel method and its derived masks against every declared `@role` (`lens_access`), and the encoder's `params=` arguments (`lens_params`). What a lens generates stays in the lens.
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

**Metal device memory is the drivers', never the model's.** A model's weight regions, whole-plane buffers and concat
regions sit in `dasllama_metal_common`'s address-keyed tables, and a `Model` holds no Metal handle, so `delete` of a model
frees its host planes and leaves its regions cached until the weights epoch moves (the next load drops them,
`ARCHITECTURE_RUNTIME.md#weights-epoch-on-load`) or the drivers shut down. The KV mirror arena (grown up to its ceiling,
`DASLLAMA_METAL_KV_MIRROR_MB`), the deltanet mirror pool and the buffer pools live until `metal_decode_shutdown` /
`metal_prefill_shutdown`, bounded by the largest shape the process served; a boundary that must hand the device back calls
both. A released buffer leaves the process footprint because the binding reads `contents` inside an autorelease pool
(`../dasMetal/ARCHITECTURE.md`), and the footprint (`metal_footprint_settled`), not the live-object count, witnesses it -
the support matrix's release row (`metal_release_row`) holds three load-serve-delete cycles of E2B to their first close.

**Race and tune code for a kernel family lives beside the family.** The shared scaffolding
(`race_buf`, `race_envelope_ok`, `race_pair_ms`, `MetalTensorRaceResult`) is `<gpu>_common`'s;
each module races its OWN families (`metal_tensor_race_decode` in kernels, `metal_tensor_race`
in prefill) and the tuner calls those public entries.

**Decline REASONS are enum values in the shapes module** (`MetalDecodeDecline`, `MetalPrefillDecline`);
decline COUNTING lives in `<gpu>_common` beside `require_or_panic`, for both paths. A reason a plane format decided
(`kquant_native`; `graph` on a routed stack, a shared expert or a deltanet plane) keeps its enum value in the
declines-by-reason table; the shapes module's `metal_fmt_decline_say` logs the site and `KqFmt` member beside it once
per line and hands the same words to the decline, whose required-mode panic prints them in place of the bare reason.
Each driver has one decline enum: Metal's live in `dasllama/dasllama_metal_shapes.das`, and a Vulkan driver's lives in
that driver's own file.

Vulkan is the deliberately-designed model of this shape; Metal converges as it is touched.
