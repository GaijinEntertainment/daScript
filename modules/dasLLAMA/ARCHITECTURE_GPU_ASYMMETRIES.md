# dasLLAMA Architecture - the allowed asymmetries between the GPU backends

Companion to `ARCHITECTURE_GPU.md`; a section is cited by its anchor. This document carries
the closed list of the ways the GPU backends - Metal, Vulkan and CUDA - are allowed to differ. The GPU
backend role table the list reads against - the role files, the tier's seats, the kernel-binding
asymmetries a stamp family ledgers - stays in `ARCHITECTURE_GPU.md#gpu-backends`.

### The allowed asymmetries between the backends {#gpu-asymmetries}

**This list is closed; a new asymmetry lands with its entry here:**

- **The whisper-class block-hook pin is Vulkan-only** (`set_vulkan_audio_blocks`: the block hooks pinned off while the conv stem still serves, the stem-flush and lifetime cells' seat; the Metal tower serves stem and blocks as one chain, nothing to pin apart).
- **`metal_tower_mel_encodes` is Metal's mel counter, a function of its own**: it rises once a mel the device mel seat served (`register_whisper_mel_gpu`), the qwen3a mel and the chat towers' chunked mel alike. Vulkan counts the same seat as the `mels` field of `vulkan_tower_stats()`, where Metal's `metal_tower_stats()` carries no mel field. The caller outside the backend is `test_log_mel_chunks_gpu` (`tests/test_audio.das`).
- **The streaming lane mint and the trimmed lane are Vulkan-only** (`vulkan_mint_begin` / `vulkan_mint_end`, the lever
  `set_vulkan_trim` / `vulkan_trim` with its `restore_vulkan_trim` form): the Vulkan lane carries a device twin the GPU
  walk collects, so its mint streams the walk's entries into the save and a trim can drop the planar families the arena
  holds whole; the Metal flavor bakes device-form blobs the CPU planes convert into, with no walk to collect from and no
  family to drop.
- **The `dasllama_gpu_tier` cooperation SPI is Vulkan-only**: every hook seat the tier exposes (`install_moe_gpu_tier` and the `set_moe_gpu_*_hooks` setters) is registered by the
  Vulkan family alone, and the tier's entry in `ARCHITECTURE_GPU.md#gpu-backends` enumerates the seats; a new seat lands in that
  entry, not as a new entry here. The one seat outside that rule is the deltanet mirror room seat, its own entry
  here.
- **The split token command with the routed experts on the host is Vulkan-only**
  (`vk_rdec_token_split`, `ARCHITECTURE_GPU_VULKAN_HC.md#hc-token-command`): the Vulkan whole-model driver
  leaves a hyper-connection MoE's expert stacks on the host by construction (`resident_host_experts`, the
  model's shape, not the card's room), so it cuts its command at every routed block and the host sums the experts between the segments; Metal serves the
  same model whole from unified memory and its drivers split nothing.
- **The window's f32 per-layer-embedding gate and projection run through an f16 stage on Vulkan alone**
  (`ARCHITECTURE_GPU_VULKAN_KHR.md#f32-rows-staged-tile`, the knob `DASLLAMA_VK_PLE_F32_TILE`, Vulkan's alone): the
  staged KHR tile reads the file's f32 rows, rounds each weight to f16 on its stage, feeds f16 rows and accumulates in
  f16, because the f32 scalar tile is the only f32-exact form the Vulkan tier has and runs at a twenty-fifth of its rate; Metal
  serves the same planes on its f32-exact tile, and has no form to pick between.
- **The hot expert pool is Vulkan-only** (`ARCHITECTURE_GPU_VULKAN_POOL.md#hc-hot-pool`, the tier's four hot
  seats, `DASLLAMA_GPU_HEAT`): where routed experts live on the host the Vulkan driver keeps the hottest in
  per-layer device slots; Metal has every expert resident, so nothing is hot or cold.
- **A plain MoE's host-layer form is Vulkan-only** (`ARCHITECTURE_GPU_VULKAN_HC.md#hc-token-command`,
  `#hc-window-chain`, the host-rows seat): a plain MoE a discrete card does not hold whole beside a context
  asked whole, or one whose host-layers ask names them, moves its first routed layers' experts to the host,
  the token command and the window cut at those layers; Metal holds the model whole from unified memory and
  cuts nothing.
- **The host-layers ask is read by the Vulkan plan alone** (`set_gpu_host_layers`,
  `DASLLAMA_GPU_HOST_LAYERS`, `ARCHITECTURE_GPU_VULKAN_POOL.md#hc-hot-pool`): it names how many routed
  layers' experts the Vulkan plan moves to the host whatever the card holds; Metal moves no expert off the
  device, so the ask has nothing to move.
- **The streamed prompt and its levers are the Vulkan driver's alone** (`set_gpu_want_pf_stream`,
  `DASLLAMA_GPU_PF_STREAM`, `DASLLAMA_GPU_PF_STREAM_LAYERS`, `set_pf_window_rows_pin`, `DASLLAMA_GPU_PF_ROWS`,
  `set_expert_cache_mb`, `DASLLAMA_EXPERT_CACHE_MB`, `DASLLAMA_EXPERT_RING`, `DASLLAMA_EXPERT_READERS`,
  `DASLLAMA_VK_XF_BUNDLE`, `DASLLAMA_GPU_HOT_DMA`, `DASLLAMA_GPU_DEC_MEET`,
  `ARCHITECTURE_GPU_VULKAN_STREAM.md#streamed-layer-split`, `#expert-cache`,
  `ARCHITECTURE_GPU_VULKAN_POOL.md#pool-dma-fill`): a routed layer whose experts serve on the host streams
  its stack to the device for the prompt window, the window's rows cut at `DASLLAMA_GPU_PF_ROWS`, the
  stacks held in pinned memory under the expert cache's budget and read off the lane by its readers, the
  pool's misses filled by DMA and a decode token's picks met over the pool under the two experiment knobs;
  Metal holds every expert in unified memory, streams nothing and reads none of these.
- **The strict context pin is read by the Vulkan plan alone** (`set_gpu_ctx_strict`,
  `DASLLAMA_GPU_CTX_STRICT`, `ARCHITECTURE_GPU_VULKAN_RESIDENCY.md#resident-plan`): Metal's mirror is the
  model's own context in unified memory, so no plan shortens it and the pin has nothing to hold.
- **The K/V mirror codec pin is read by the Vulkan plan alone** (`set_gpu_kv_dtype`, `DASLLAMA_GPU_KV`,
  `ARCHITECTURE_GPU_SHARED_ATTN.md#vk-kv-block-codecs`): the Vulkan driver arms one mirror for every
  session at load, so its codec is asked before the load; Metal mirrors each session in that session's
  own codec, and the pin has nothing to pick.
- **The NextN head on the hyper-connection chain is Vulkan-only** (`ARCHITECTURE_GPU_VULKAN_HC.md#hc-draft-head`,
  `#hc-verify-rows`): the Vulkan driver runs a routed head as one more chain layer, its experts summed
  on the host, and verifies two rows through the split command; Metal's batch rail runs the same head
  whole from unified memory (`ARCHITECTURE_GPU_MTP_DECODE.md`) and splits nothing.
- **The deltanet mirror room seat is Metal-only.** `set_moe_gpu_dn_room_hook` (the engine half
  `gpu_dn_room_`, the facade's `gpu_dn_room`) carries a scheduler's stream count to whatever
  keeps sessions' recurrent state device-resident; `_common`'s `dn_mirror_room` is its one
  registrant, because Metal's per-session `DnMirror` cache evicts by LRU and Vulkan's resident
  driver homes state per region with no cache to size.
- **Metal sits ABOVE `dasllama_common`** (typed `Model`/`Session` access, shapes unconditional);
  **Vulkan sits BELOW it** (untyped pointer/array seams - the family never requires common).
  Both tiers ENTER from the transformer umbrella (`?das_metal` requires; the single `?vulkan`
  require of the `dasllama_math_vulkan` facade); the inversion that remains is the
  kernels<->common require DIRECTION, and it is why their seam shapes differ.
- **UMA vs discrete VRAM**: Metal never grows Vulkan's VRAM machinery - arenas, upload
  economics, mirrors and hydration are Vulkan's alone. Metal's residency artifacts are the
  `MTLResidencySet` pin in `_common` (`DASLLAMA_METAL_RESIDENCY`) plus its keep-alive
  heartbeat (`DASLLAMA_METAL_HEARTBEAT_S`, a dasMetal background re-request that stops the OS
  collecting the set over a CPU-only window, `ARCHITECTURE_RUNTIME.md#the-post-cpu-burn-gpu-ramp-and`) - a
  driver-cost shield, not a placement mechanism; memory is still memory. The collection is per
  allocation: a buffer outside the set that no submission references for about a second is
  un-wired while the GPU stays busy on other buffers - a decode turn is such a window for a plane
  only the prefill reads - and the next submission to reference it pays the driver span for its
  bytes (20 ms for the 5.4 GB baked dev-W plane of Llama-3.2-3B Q8_0). The first `residency_note`
  therefore makes the set, so a region cached before any driver entry flushes is in it.
- **The weights-epoch drop reaches the two tiers through different seams.** `bump_weights_epoch`'s
  listener seat (`register_weights_epoch_listener`) has two subscribers: `_common`'s `metal_weights_drop`,
  which runs the registered reload preps (`register_reload_prep`; the decode driver registers
  `discard_pre_encoded_steps`, the tower `tw_weights_drop`, dropping its TTS slabs), quiesces, and releases
  the address-keyed region caches; and the Vulkan TTS driver's `ts_forget`, dropping its slabs and the Pocket
  state - both TTS drivers listen because a TTS slab's key is weight addresses a reload reuses. Vulkan's LLM
  reload story stays the unmap notify (`set_moe_gpu_unmap_notify`) - a different seam for a different ownership model.
- **`vulkan_tts_stats` / `vulkan_tts_declines` have no Metal twin**: the Vulkan TTS driver counts its encodes
  and its declines by reason in its own pair, where Metal's TTS seats report through `metal_tower_stats` beside `styletts2_gpu_stats`.
- **The Vulkan block-codec decode attention quantizes the query to q8_0 blocks; Metal's keeps it f32**
  (`DaAttnT`'s `KVBLK` stamps, `ARCHITECTURE_GPU_SHARED_ATTN.md#vk-kv-block-codecs`, against `MetalSqAttnDQuantT`):
  Vulkan scores a K word against a query word in one packed integer dot, the CPU chain's own query quantization, so
  the two agree to it; Metal's per-key loop dequantizes the block into float multiplies against the f32 query. The
  codec cells of each home hold their own chain to the CPU's.
- **The batched pre-encoded step is Metal-only**: the batch driver encodes the next step under the
  current one's GPU run (`ARCHITECTURE_GPU_MTP_DECODE.md#batch-pre-encode`, `DASLLAMA_METAL_BATCH_PRE`);
  Vulkan's N-row token command records once and resubmits, so it has no encode to move.
- **The K-quant rows sites' half-x rail is Metal-only** (`DASLLAMA_METAL_KQ_HALF`, `set_metal_kq_half`): under a
  `kq_rows_half_<fmt>` crown the decode's two-to-eight-row K-quant sites read x as the f16 twin the row producer stores
  beside it (`ARCHITECTURE_GPU_MTP_DECODE.md#kq-rows-crown`), and the knob keeps the f32 forms for an A/B; Vulkan's rows
  sites read f32 x alone.
- **The superblock MoE twins' knee is Metal-only** (`set_metal_moe_kq_twin_avg`): on a box whose sidecar carries no
  `moe_mulmm_<fmt>` crown for a routed format the site takes the tensor twin where the window's mean rows an expert sit
  at or under the knee (`ARCHITECTURE_GPU_PREFILL_MOE.md#prefill-moe-kq-twin-pick`); Vulkan's expert tiles have no twin
  to pick.
- **The window chain's form levers are Metal-only** (`set_metal_prefill_held` holding one crowned form off for an
  A/B on one loaded model, `set_metal_moe_gather_min` / `set_metal_moe_tall_avg` moving the routed block's two
  engage points, `metal_prefill_keep_moe_rows` / `metal_prefill_moe_rows` recording the rows a window routed, and the
  tower's `set_metal_fc_attn_dev`): each picks between two forms the Metal prefill or
  tower driver carries side by side, and the Vulkan chain has one form at each of those sites, so nothing to pick.
  The callers outside the backend are the probe `harness/p0_ko_probe.das` and the cells that hold both forms.
- **The decode step's per-form A/B levers are Metal-only setters** (`set_metal_batch_gather`: the batched step takes the
  rows the host embeds instead of gathering them off their ids; `set_metal_moe_g4_unfused`: the gemma-4 MoE tail on its
  unfused chain instead of the one-step `MetalMoeCombG4`): the Metal decode driver encodes each step afresh, so a setter
  flips one form between two steps and drops the pre-encoded step; the Vulkan driver builds its token commands' sets at
  the load and resubmits them, so its counterparts are load-time pins over every command - `set_gpu_resident_embed(0)` for the host-embedded
  rows, the `g_vk_fuse` pin of `DASLLAMA_VK_FUSE` for the split MoE tail - and no per-step setter.
- **The round's cold seat is Metal-only.** `register_mtp_cold_override("metal", ...)` has one registrant, `metal_mtp_cold_why`
  (`ARCHITECTURE_GPU_MTP.md#mtp-round-one-join`): a blob model has no CPU rails, so a round the Metal seats cannot take steps
  plain before any seat is reached; the Vulkan seats decline per call into the CPU verify window instead
  (`ARCHITECTURE_GPU_VULKAN_MTP.md#resident-verify-window`). The draft, verify, verify-rows and rollback seats have a registrant
  on both homes, and the round is the common one (`mtp_spec_round_k`) everywhere.
- **The assistant drafter's drafts reach its verify on the device on Metal alone.** Metal's verify gathers the draft rows' embeds off
  the chain's token buffer, so drafts and verify run behind one another with no join between
  (`ARCHITECTURE_GPU_MTP.md#mtp-round-one-join`). The Vulkan N-row command takes host-staged ids and rope rows - each row's id
  staged for its gather and its rope row built on the host at its own position - so the draft command lands its ids on the host first, one join a round
  (`ARCHITECTURE_GPU_VULKAN_MTP.md#resident-assistant-drafter`).
- **The device KV room seat is Metal-only.** `set_gpu_kv_room_hook` has one registrant, the Metal mirror's ceiling
  (`mirror_rows_within_ceiling`); `gpu_kv_room_rows` reads unbounded elsewhere and `device_kv_max_rows` the model's
  own context, since the Vulkan plan sizes its regions at the load (`set_gpu_ctx_max`).
- **The mirror-fed prefill window, the panel forms' mirror feed and mirror adoption are Metal-only**
  (`ARCHITECTURE_GPU_PREFILL_WINDOW.md#prefill-kv-mirror`, the knobs `DASLLAMA_METAL_PF_MIRROR` and
  `DASLLAMA_METAL_MIRROR_ADOPT`, the counters `metal_prefill_mirror_served`, `metal_prefill_mirror_panel_served`,
  `metal_mirror_uploaded_rows` and `metal_mirror_adopted_rows`): the Vulkan resident driver's window chain writes its
  K/V into the region the decode reads, so it has no mirror to feed or adopt.
- **The NextN draft seat is Vulkan-only.** `register_mtp_draft_override("vulkan", ...)` has one registrant, the resident driver's head (`ARCHITECTURE_GPU_VULKAN_MTP.md#resident-draft-head`); Metal drafts inside its round seat. Each backend's owner (`register_mtp_seat_owner`, `mtp_seats_own`) claims the round its own way: Metal's a blob model alone, so a planar one under the metal overrides runs the CPU round, Vulkan's the resident driver armed on a NextN model. The head's prompt warm is a seat on Vulkan alone (`install_rdec_head_warm`: the window chain's extra layer, `ARCHITECTURE_GPU_VULKAN_MTP.md#resident-head-prompt-warm`), where Metal's prefill driver warms the slab inside its own loop; the CPU warm stands down behind either once the driver landed the logits (`prefill_override_logits_done`).
- **Lens depth**: both lenses generate `enc_*` builders from kernel classes - Metal via
  `[metal_dispatch]`, Vulkan via `[vk_dispatch]` (per-class set layouts + push constants, and
  NonWritable derived per binding from the access classification - `ARCHITECTURE_GPU_VULKAN.md#vk-readonly-lens` carries the rule, its refusal and its reading; Metal lowers a read role to `device const`
  already) - and both speak the multi-kernel form (`kernel=` names the method, one macro instance per kernel, declared roles must cover every kernel).
- **`family=` is Vulkan-only.** A vulkan family shares the per-class surface - the `VkdClass`
  global, the `set_*` builder, the pipe slots - across classes with one binding layout. Metal's
  generated surface is the `enc_*` builder and its `<builder>_pso` twin, both per class, so there
  is nothing for a family to share; cross-class PSO/source sharing on Metal is a PSO-lifecycle
  question, not a lens one.
- **The `<builder>_pso` form is Metal-only.** `[metal_dispatch]` also generates each builder over a
  pipeline the caller passes (`ARCHITECTURE_GPU.md` names it), so a kernel gate under `tests/`
  dispatches its own pipeline through the production bind list while the engine's builder reads
  its module-private pipeline global; `[vk_dispatch]` has no counterpart, since a Vulkan cell binds
  through the class's `VkdClass` global and its `set_*` builder, which the tests reach directly.
- **`@default` is Metal-only.** A `[metal_dispatch]` field may name a fallback global
  (`@default = g_one`) that the generated builder binds when the caller passes null;
  `[vk_dispatch]` has no counterpart - vulkan callers pass a real buffer at every slot, and an
  optional-bind shape there takes this same annotation, not a new spelling.
- **The workgroup-footprint gate is Vulkan-only.** `[vk_dispatch]` sums a class's `@workgroup`
  members and its generated `ensure_*` declines by name (`vkd_wg_fits`) before the pipeline
  build, because MoltenVK's over-cap failure is an opaque `INITIALIZATION_FAILED` - and the
  resident driver declines residency with it (`vk_rdec_prepare`). `[metal_dispatch]` has no
  footprint gate: Metal's own pipeline compile fails loudly with the footprint in the error.
- **Vulkan has no shapes module yet** - `resident_upload` declines ad hoc by feature name, and
  every decline says its reason: the whole-model driver's gates speak through one
  `resident driver declined - <reason>; the per-op rails serve` line, each per-op rail walk
  reports the layers it left on the CPU with the first layer's reason and its VRAM-budget stop,
  a dense model's FFN gets its own line (the per-op tier's dense-FFN rail is on by default;
  `DASLLAMA_GPU_DENSE_FFN=0` keeps a dense FFN on the CPU,
  `ARCHITECTURE_GPU_VULKAN_PEROP.md`), and the
  per-call resident overrides say each pass-to-CPU reason once per armed model
  (`rdec_pass_once`) and count every pass by reason in the tier (`gpu_cpu_passes`, zeroed as a
  model arms), which the server's `/v1/stats` carries as `gpu_cpu_passes`; the tier hands out each reason with a
  reader-facing sentence beside its count (`rdec_pass_words`; `REVIEW.das`'s `check_rdec_pass_words` holds every `RdecPass` value to an arm there,
  licensing no names). The gap is `followup_vulkan.md` item 1, not a precedent to copy.
- **Device-home sessions are Vulkan-only.** A session whose K/V lives only in a region of the
  resident driver's mirror (`create_device_session`, the scheduler's device mode, park and
  adopt: `ARCHITECTURE_GPU_VULKAN_RESIDENCY.md#resident-plan`) has no Metal twin, and
  `gpu_device_sessions` answers 0 there: Metal's whole-forward driver reads the host cache in
  unified memory, so it has no mirror to split, and the batched row homes its streams on the host
  (`ARCHITECTURE_MEASUREMENT.md#one-benchmark-rig`). The host pool behind them - openings and
  finished turns copied down to host RAM, taken back up into a region - is Vulkan-only with
  them; `DASLLAMA_DEVICE_POOL=0` keeps the regions' own claims alone
  (`ARCHITECTURE_ENGINE_SERVING.md#device-home-pool`).
- **The dev-W f16 panels are a Metal-only knob.** `DASLLAMA_METAL_DEVW` /
  `set_metal_devw_panels` makes Metal's dense Q8_0, K-quant, iquant and per-32 prefill sites read a stored f16
  copy of their planes in place of the per-window scratch panels
  (`ARCHITECTURE_GPU_PREFILL_GEMM.md#devw-panel-knees`); Vulkan's prefill tiles read each plane's
  own blocks in the tile load, so that tier has no f16 weight panel for such a knob to keep.
- **The device-side token-embedding gather covers different tables and calls per backend.** The
  engine asks a probe before it embeds; on true it stashes the token ids, skips the CPU embed
  loop, and a late decline backfills the stash on the CPU (`embed_backfill_rows`). Vulkan answers a
  prefill through the one installed seat (`register_embed_gpu_gate`, `dasllama_common.das`) - the
  ids-form prefill installs SEPARATELY from the resident driver bundle (`install_rdec_prefill_ids`),
  so a tier without it never arms the gate - and a decode step through its own gate under its
  override name (`register_embed_dev_gates("vulkan")`); Metal registers both gates under its name
  (`register_embed_dev_gates("metal")`). A batched step's rows ride each backend's batch gate
  (`register_embed_batch_gate`, the hub embedding them late where the override declines). Both gather
  the window's rows, the decode step's row and a batched step's rows on the device off the table's q8
  or K-quant plane by its format - Vulkan off the tied
  classifier's plane or an untied table's own plane in the arena (`EmbGather`, the `KqEmb<Fmt>`
  leaves, `ARCHITECTURE_GPU_SHARED_ROWS.md#token-table-gather`), Metal off the blob planes
  (`MetalEmbedKqT`, one stamp a format; `emb_dev_gather_ok`) - and the step reads a 4-byte token id
  in place of the row (Vulkan's recorded command off a host-visible word, Metal's poke). An
  F32/F16/BF16 table both gather off its exact f32 rows - Metal reading the blob in place in unified
  memory, Vulkan off a device upload under `RDEC_EMB_F32_CAP` the plan counts first.
- **The deltanet-resident seats are Vulkan-only.** The whole-model driver's recurrent arm
  installs SEPARATELY from the resident bundle (`install_moe_gpu_resident_dn`: a recurrent
  layer's plane set, its beta/alpha rows, the per-token owner bind and the prefill's slot
  handoff), so a tier without the seats declines a recurrent layer by name
  (`resident_layer_decline`) and the per-op rails serve it. Metal has no seat to install: its
  whole-forward driver carries the recurrent branch inside its layer encoder.
- **The short window's gathered expert route and its knee are Metal-only** (`set_metal_moe_gemv_max`, the
  sidecar's `metal_moe_gemv_max`, `ARCHITECTURE_GPU_PREFILL_MOE.md#prefill-moe-gemv-route`): the Metal prefill
  serves a routed block of few tokens through the decode step's gathered GEMVs instead of its bucketed tiles; the
  Vulkan window chain's routed block rides its expert tiles over a device-side gather at every window size - the
  cm2 expert chain (`ARCHITECTURE_GPU_VULKAN_GEMM.md#cm2-expert-chain`), the KHR tile
  (`ARCHITECTURE_GPU_SHARED_GEMM.md#khr-mm-kq-tile`) or, on a device with neither, the Q8-fed chain's batch tiles
  (`ARCHITECTURE_GPU_VULKAN_MOE.md#vk-moe-q8-chain`) - and has no tile-per-expert form to route away from.
- **The int8 MMQ prefill is the CUDA home's alone** (`DASLLAMA_CUDA_MMQ`, on by default; `crd_site_mmq`): a window
  GEMM site whose plane's format has a leaf - Q8_0; the superblock Q4_K, IQ4_XS, Q4_0 and IQ4_NL; the per-32 Q5_1, Q5_0
  and the off-lattice Q4_0 and IQ4_NL - whose output width is a multiple of 128 and whose reduction of 256 (512 where
  the leaf carries a min term), at a window of whole 128-row tiles, runs on that leaf over the activations requantized
  to Q8_0 blocks between the GEMMs, as the token step's feeds are; every other site - the other formats, a shape off the
  tile, `=0` - takes the f16-fed cm2 tiles, and a layer whose sites split produces both feeds (the residual step's
  requant, then its f16 norm pass with no add). `=1` leaves out the leaves whose windows did not beat the tiles on every
  measured card - Q5_1 lost on both (its five-word blocks and its m cost the leaf a third of the Q8 one's rate, under the
  feeds' cost), Q5_0 won on the RTX 5060 Ti and lost on the RTX PRO 4500, the first format the two cards split - and
  `=2` arms every leaf, a new card's own measurement; the split is what makes the pick a per-box race rather than a
  default (the ledger's row). The Q8_0 leaf stages raw blocks by cp.async; the other leaves
  (`MmqT` under `KQ` for the superblock formats, `B32` with the format's tag for the per-32 ones) decode their codes to
  int8 through registers in the weight stage - the codebook formats through the home's permute decode, Q4_0 and Q5_0
  with their offsets folded into the signed codes - a scale a block. The leaves with a min term (Q4_K's `dmin x mn`,
  Q5_1's `m`) carry it apart: the requant kernels write each activation block's scale x quant sum beside its scale (`OUT_SUM`), the stage
  keeps each row's `-dmin x mn` and each token's sum as halves over sixteen blocks, and one f16 tensor product a fragment
  pair every sixteen blocks folds the rank-sixteen correction into the f32 accumulators - a per-block fixup off shared
  memory cost the leaf a fifth of its rate. A format whose scale changes every sixteen elements (Q6_K, Q3_K, Q2_K, the
  iq2 strips) stays on the f16 tiles: the int8 product over sixteen issues at the k32 shape's cost for half its work, so
  a Q6_K leaf on it read 37 to 39 TFLOP/s against the Q6_K cm2 tile's 38 to 51 on the same shapes - MMQ is a
  per-32-scale property. NVFP4 takes the same slot on the FP4 tensor rung instead (`CuNv4Mmq` over `MmqFp4T`, the
  arch-specific `sm_120a` target the emitter marks): raw e2m1 nibbles on both sides under the block-scaled mma, whose
  UE4M3 scale a k16 strip is the format's own, so the weight stage re-pairs the k4-tiled 32-blocks into the feed's
  (k, k + 8) order and stages no int8 at all - the sixteen-wide scale that keeps Q6_K off the int8 leaf is what the
  instruction consumes. Its feed is W4A4: the activations quantized to e2m1 per token (`fp4_rows`, `fp4_actrows`, the
  residual step's `cls_ar_fp4_b`; a row scale of amax / (6 x 448), a UE4M3 code a strip - the nearest to its largest over
  6, stepped up once where that code would saturate the largest value (`fp4_strip_code`: the nearest alone saturates on
  half of all strips and reads 9% on the 1B's pp-path perplexity against 5.7% with the step; a least-squared-error pick
  over five candidate codes trades the outlier for the bulk and measured worse than either - `PERF_LEDGER.md`, the dasCUDA
  arc's W4A4 entry) - the nibbles in the same
  pairing; a feed reads its row twice, once for the row scale and once for the strips - a row kept in registers between the
  two passes measured no faster at 14336 wide and slower at 4096), the row scale and the plane's per-tensor factor applied to the accumulator's columns in
  the epilogue; a layer whose sites split across the three feeds produces each from the one residual step. On the RTX
  5060 Ti the leaf ran 110 to 121 TFLOP/s against the Q4_0 int8 leaf's 65 to 72 and the cm2 tiles' 40 to 56 at the 1B
  window's shapes (`PERF_LEDGER.md`, the dasCUDA arc's FP4-leaf entry: the command, the shapes, the forms not kept). The
  knob changes the prefill's rounding and its logits; the window log line names the planes on each form, and under the
  test pin `set_cuda_mmq_pin` it names the pin's value in place of the knob's. The first window sizes the feeds under the
  knob's answer and keeps it (`CudRdec.pf_knob`), so a pin moved after it changes no later pick onto a feed that was never
  allocated. The FP4 leaf is picked on a card at compute capability 120 or above (`FP4_RUNG_SM`: the block-scaled mma is
  an `sm_120a` text); below it an nv4 plane's window runs on the cm2 tiles under any knob value. Both forms stay compiled whichever one a card favors: the int8 against f16 tensor rate
  is the card's and the driver's, and the default is a static pick until a card flips it. The Vulkan window's form follows the device's coopmat mode
  (`ARCHITECTURE_GPU_VULKAN_GEMM.md#cm2-tile-pick-and-default`), and the knob reaches nothing there or on Metal.
- **The nv4 decode GEMV's nibble decode is a byte permute on CUDA, the SWAR formula on Vulkan and a staged codebook on
  Metal.** The shared leaf (`Nv4GemvT`) routes the e2m1 nibble decode through one virtual (`e2m1_split`); the CUDA home's
  `Nv4GemvPrmtT` overrides it with `byte_perm` over the doubled-magnitude table and its negation - 18 lane ops a quant word
  against the formula's 29, the same int8 lanes, so the sums agree bit for bit - and the decode reads at the Q4_0 leaf's
  rate at the 8B shapes where the formula read 4 to 5% behind (`PERF_LEDGER.md`, the dasCUDA arc's nv4 decode GEMV entry).
  Vulkan has no byte permute, so it keeps the formula. Metal does not stamp the shared leaf at all: nv4 rides the iq4
  family's templates under `NV4`, the doubled e2m1 table staged in threadgroup memory as iq4nl's codebook is and the
  UE4M3 byte pair read off the compact 16-byte row (`ARCHITECTURE_GPU_QUANT_PLANES.md#metal-kq-split-scale-plane`);
  the per-tensor `.scale` the Vulkan and CUDA rows carry in their pad word has no Metal carrier, so a plane under one
  declines there.
- **`set_cuda_mmq_pin` is a CUDA-only setter.** It pins the MMQ knob's answer ahead of a load, so one process can hold a
  carrier to the leaves and then to the tiles (the resident cells' knob-off control, the W4A4 instrument). The Vulkan
  window's form follows the device's coopmat mode and Metal has one window form, so neither home has a pin to set.
