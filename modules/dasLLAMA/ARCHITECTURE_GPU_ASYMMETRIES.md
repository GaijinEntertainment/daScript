# dasLLAMA Architecture - the allowed asymmetries between the GPU backends

Companion to `ARCHITECTURE_GPU.md`; a section is cited by its anchor. This document carries
the closed list of the ways the GPU backends - Metal, Vulkan and CUDA - are allowed to differ. The GPU
backend role table the list reads against - the role files, the tier's seats, the kernel-binding
asymmetries a stamp family ledgers - stays in `ARCHITECTURE_GPU.md#gpu-backends`.

### The allowed asymmetries between the backends {#gpu-asymmetries}

**This list is closed; a new asymmetry lands with its entry here** - one of the CUDA home alone (the int8 MMQ
prefill, the FP4 expert rung, the nv4 nibble decode, the MMQ pin) in `ARCHITECTURE_GPU_ASYMMETRIES_CUDA.md#cuda-asymmetries`:

- **The whisper-class block-hook pin is Vulkan-only** (`set_vulkan_audio_blocks`: the block hooks pinned off while the conv stem still serves, the stem-flush and lifetime cells' seat; the Metal tower serves stem and blocks as one chain, nothing to pin apart).
- **`metal_tower_mel_encodes` is Metal's mel counter, a function of its own**: it rises once a mel the device mel seat served (`register_whisper_mel_gpu`), the qwen3a mel and the chat towers' chunked mel alike. Vulkan counts the same seat as the `mels` field of `vulkan_tower_stats()`, where Metal's `metal_tower_stats()` carries no mel field. The caller outside the backend is `test_log_mel_chunks_gpu` (`tests/test_audio.das`).
- **The streaming lane mint and the trimmed lane are Vulkan-only** (`vulkan_mint_begin` / `vulkan_mint_end`, the lever
  `set_vulkan_trim` / `vulkan_trim` with its `restore_vulkan_trim` form): the Vulkan lane carries a device twin the GPU
  walk collects, so its mint streams the walk's entries into the save and a trim can drop the planar families the arena
  holds whole; the Metal flavor bakes device-form blobs the CPU planes convert into, with no walk to collect from and no
  family to drop.
- **The per-op tier's hook seats are the Vulkan family's; the whole-model driver's seats both discrete families fill.** The
  per-op hooks of `install_moe_gpu_tier` and the `set_moe_gpu_*_hooks` setters (ffn, cls, dense, dn, attn, attn_dec, ffn_tail,
  span_dec, ffn_bias, heat, qkv) are the Vulkan family's - the CUDA home installs the tier with stubs that decline or panic by
  name, having no per-op tier (`dasllama_math_cuda.das`) - while every `install_rdec_*` / `install_moe_gpu_resident_*` seat of
  the resident driver both homes register (the prepared-image seats among them: each home mints its own lane,
  `ARCHITECTURE_IMAGE_VULKAN.md#image-vulkan-mint`; the assistant drafter among them, each discrete home's own
  `register_mtp_assistant`), and registering the hyper-connection seats as stubs
  that decline the chain by name. Nine seats the Vulkan registration fills stay off the CUDA registration by this entry - `REVIEW.das`'s
  `check_discrete_seat_parity`'s licensed set, `CUDA_SEATS_RULED`: the per-op tier hooks `set_moe_gpu_attn_dec_hooks`,
  `set_moe_gpu_ffn_bias_hook`, `set_moe_gpu_ffn_tail_hooks`, `set_moe_gpu_heat_hooks`, `set_moe_gpu_qkv_hook` and
  `set_moe_gpu_span_dec_hook`, the n-gram conv ring's checkpoint `install_rdec_dn_snap_ring` (a hyper-connection seat), the
  mint census's host-bytes line `set_moe_gpu_host_bytes_fn` (the CUDA home keeps no host-visible count) and the mapping's
  unmap notice `set_moe_gpu_unmap_notify` (the CUDA home imports no mapping). The property the nine share: each serves a
  mechanism the CUDA home does not have - a per-op tier, the hyper-connection chain, a host-visible plane count, a mapped
  import - so no CUDA code could fill it; a seat the CUDA home could fill and does not is a finding, never a ruling. The
  check finds a seat the Vulkan registration installs that the CUDA registration neither installs nor rules, and a ruled
  seat either registration contradicts. The tier's
  entry in `ARCHITECTURE_GPU.md#gpu-backends` enumerates the seats; a new seat lands in that entry, not as a new entry here.
  The one seat outside that rule is the deltanet mirror room seat, its own entry here.
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
- **The hot expert pool is the discrete homes'** (`ARCHITECTURE_GPU_VULKAN_POOL.md#hc-hot-pool`, the tier's four hot
  seats, `DASLLAMA_GPU_HEAT`): where routed experts live on the host the Vulkan and the CUDA driver keep the hottest in
  per-layer device slots (the CUDA home's slots, hit chain and the pool's place in its segments: `ARCHITECTURE_GPU.md#gpu-backends`;
  its window's pool prefix, its DMA fill and its placement batch the Vulkan form's over the home's copy stream); Metal has every expert
  resident, so nothing is hot or cold.
- **A plain MoE's host-layer form is the discrete homes'** (`ARCHITECTURE_GPU_VULKAN_HC.md#hc-token-command`,
  `#hc-window-chain`, the host-rows seat): a plain MoE a discrete card does not hold whole beside a context
  asked whole, or one whose host-layers ask names them, moves its first routed layers' experts to the host,
  the token command and the window cut at those layers (the CUDA home's cut and segments: `ARCHITECTURE_GPU.md#gpu-backends`); Metal holds the model whole from unified memory and
  cuts nothing.
- **The host-layers ask is read by the discrete homes' plan alone** (`set_gpu_host_layers`,
  `DASLLAMA_GPU_HOST_LAYERS`, `ARCHITECTURE_GPU_VULKAN_POOL.md#hc-hot-pool`): it names how many routed
  layers' experts the resident plan moves to the host whatever the card holds; Metal moves no expert off the
  device, so the ask has nothing to move.
- **The streamed prompt and its levers are the discrete homes'** (`set_gpu_want_pf_stream`,
  `DASLLAMA_GPU_PF_STREAM`, `DASLLAMA_GPU_PF_STREAM_LAYERS`, `set_pf_window_rows_pin`, `DASLLAMA_GPU_PF_ROWS`,
  `set_expert_cache_mb`, `DASLLAMA_EXPERT_CACHE_MB`, `DASLLAMA_EXPERT_RING`, `DASLLAMA_EXPERT_READERS`,
  `DASLLAMA_VK_XF_BUNDLE`, `DASLLAMA_GPU_HOT_DMA`, `DASLLAMA_GPU_DEC_MEET`,
  `ARCHITECTURE_GPU_VULKAN_STREAM.md#streamed-layer-split`, `#expert-cache`,
  `ARCHITECTURE_GPU_VULKAN_POOL.md#pool-dma-fill`): a routed layer whose experts serve on the host streams
  its stack to the device for the prompt window - the Vulkan tier over its transfer queue, the CUDA home over its
  copy stream (`dasllama_cuda_stream`) - the window's rows cut at `DASLLAMA_GPU_PF_ROWS`, the stacks held in the
  pinned memory of the expert cache the two share, the pool's misses filled by DMA and a decode token's picks met
  over the pool under the two experiment knobs; the cache's partial form - a quota of a layer's experts resident,
  the rest read off the lane by its readers under `DASLLAMA_EXPERT_CACHE_MB` - rides either home's own lane
  (`ARCHITECTURE_GPU_VULKAN_STREAM.md#expert-cache`; a load with no lane keeps every streamed stack whole, the ask
  said once); Metal holds every expert in unified memory, streams nothing and reads none of these.
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
- **The short window's split-K lever is Metal-only** (`DASLLAMA_METAL_SPLIT_K`, `set_metal_prefill_split_k`): `0` keeps every
  prefill tile whole - the split sums a tile's K in another f32 order, so the window's numerics move with it
  (`ARCHITECTURE_GPU_PREFILL_GEMM.md#prefill-split-k`); Vulkan's short-window split has no environment lever, its one control
  the sidecar knee `vk_split_k_tgs` (`ARCHITECTURE_GPU_VULKAN_GEMM.md#vk-short-split`, `0` runs the window whole).
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
- **The FastConformer chain's profile replay is Metal-only** (`set_metal_fc_prof_replay`): the Metal chain is one
  command buffer, which leaves no per-kernel split, so under the asr_prof rail the lever replays the rotary form's head
  restrides and slab trio after the readback (`ARCHITECTURE_GPU_TOWER_FC.md#tower-fc-rotary`); the Vulkan chain
  carries no replay.
- **The tower's decline stand-ins are Metal-only setters** (`set_metal_take_max_probe_`: the device's largest
  buffer under the pooled scratch takes, the seam of the tower's `memory` decline and the translation driver's
  `device` decline; `set_metal_vad_lanes_probe_`: the VAD recurrence pipeline's thread cap, the `device` decline's
  seam): Metal reads both limits off the device and the pipeline, so a
  cell reaches those declines through a stand-in; the Vulkan limits are fields of the device record (`g_gpu.msr`,
  `g_gpu.has_coopmat`) a cell sets and restores directly, so the Vulkan tower has no such setter.
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
- **The NextN draft seat is the discrete homes'.** `register_mtp_draft_override("vulkan", ...)` has one registrant, the resident driver's head (`ARCHITECTURE_GPU_VULKAN_MTP.md#resident-draft-head`), which the Vulkan and the CUDA home fill through the tier's seats (`install_rdec_draft`, `install_rdec_verify`, `install_rdec_rollback`, `install_rdec_dn_snaps`, `install_rdec_head_warm`; the CUDA home's forms in `ARCHITECTURE_GPU.md#gpu-backends`); Metal drafts inside its round seat. Each backend's owner (`register_mtp_seat_owner`, `mtp_seats_own`) claims the round its own way: Metal's a blob model alone, so a planar one under the metal overrides runs the CPU round, Vulkan's the resident driver armed on a NextN model. The head's prompt warm is a seat on the discrete homes (`install_rdec_head_warm`: the window chain's extra layer, `ARCHITECTURE_GPU_VULKAN_MTP.md#resident-head-prompt-warm`), where Metal's prefill driver warms the slab inside its own loop; the CPU warm stands down behind either once the driver landed the logits (`prefill_override_logits_done`).
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
- **An nv4 expert stack under a per-expert `.scale` serves on the discrete homes alone**
  (`ARCHITECTURE_ENGINE_FORMATS_NV4.md#nv4-site-scale`): Vulkan and CUDA fold the factor off the word the gather writes
  into each slice's device scale rows; the Metal routed block binds the compact scale row, which carries no word for it,
  so `moe_fmts_unserved` and `shexp_fmts_unserved` decline such a stack by layer, site and member and the CPU serves the
  call (`followup_general.md` row 246 (3) is the Metal gap).
