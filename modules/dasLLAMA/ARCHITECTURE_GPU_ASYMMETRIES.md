# dasLLAMA Architecture - the allowed asymmetries between the GPU backends

Companion to `ARCHITECTURE_GPU.md`; a section is cited by its anchor. This document carries
the closed list of the ways the Metal and the Vulkan backend are allowed to differ. The GPU
backend role table the list reads against - the role files, the tier's seats, the kernel-binding
asymmetries a stamp family ledgers - stays in `ARCHITECTURE_GPU.md#gpu-backends`.

### The allowed asymmetries between the backends {#gpu-asymmetries}

**This list is closed; a new asymmetry lands with its entry here:**

- **The whisper-class block-hook pin is Vulkan-only** (`set_vulkan_audio_blocks`: the block hooks pinned off while the conv stem still serves, the stem-flush and lifetime cells' seat; the Metal tower serves stem and blocks as one chain, nothing to pin apart).
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
  (`vk_rdec_token_split`, `ARCHITECTURE_GPU_VULKAN_HC.md#hc-token-command`): a hyper-connection MoE's expert
  stacks never fit a discrete card beside its other planes, so the Vulkan whole-model driver cuts its
  command at every routed block and the host sums the experts between the segments; Metal serves the
  same model whole from unified memory and its drivers split nothing.
- **The hot expert pool is Vulkan-only** (`ARCHITECTURE_GPU_VULKAN_HC.md#hc-hot-pool`, the tier's four
  hot seats, `DASLLAMA_GPU_HEAT`): where the routed experts live on the host, the Vulkan driver keeps
  the hottest ones in per-layer device slots and serves their picks there; Metal has every expert
  resident, so nothing is hot or cold.
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
  driver-cost shield, not a placement mechanism; memory is still memory.
- **The weights-epoch drop reaches the two tiers through different seams.** `bump_weights_epoch`'s
  listener seat (`register_weights_epoch_listener`) has two subscribers: `_common`'s `metal_weights_drop`,
  which runs the registered reload preps (`register_reload_prep`; the decode driver registers
  `discard_pre_encoded_steps`, the tower `tw_weights_drop`, dropping its TTS slabs), quiesces, and releases
  the address-keyed region caches; and the Vulkan TTS driver's `ts_forget`, dropping its slabs and the Pocket
  state - both TTS drivers listen because a TTS slab's key is weight addresses a reload reuses. Vulkan's LLM
  reload story stays the unmap notify (`set_moe_gpu_unmap_notify`) - a different seam for a different ownership model.
- **`vulkan_tts_stats` / `vulkan_tts_declines` have no Metal twin**: the Vulkan TTS driver counts its encodes
  and its declines by reason in its own pair, where Metal's TTS seats report through `metal_tower_stats` beside `styletts2_gpu_stats`.
- **The batched pre-encoded step is Metal-only**: the batch driver encodes the next step under the
  current one's GPU run (`ARCHITECTURE_GPU_MTP_DECODE.md#batch-pre-encode`, `DASLLAMA_METAL_BATCH_PRE`);
  Vulkan's N-row token command records once and resubmits, so it has no encode to move.
- **The speculative round is Metal-only.** `register_mtp_round_override("metal", ...)` has one registrant, `gemma_mtp_spec_round`
  (falling through to `metal_mtp_spec_round` with no drafter); Vulkan serves the CPU round around its draft and verify seats (`ARCHITECTURE_GPU_MTP.md`).
- **The same-slab verify seat is Vulkan-only.** `register_mtp_verify_override("vulkan", ...)` has one registrant, the resident driver's same-slab verify (`ARCHITECTURE_GPU_VULKAN_NROW.md#nrow-verify-command`), which the CPU round takes in place of its two-row prefill; a verify the seat declines runs the CPU two-row prefill inside a window the resident prefill declines by name (`ARCHITECTURE_GPU_VULKAN_MTP.md#resident-verify-window`). Metal's same-slab verify runs inside its round seat.
- **The device rollback seat is Vulkan-only.** `register_mtp_rollback_override("vulkan", ...)` has one registrant, the reject of a verify the resident seat served - the region's recurrent slots back from the copies the verify command took, no snapshot and no replayed step (`ARCHITECTURE_GPU_VULKAN_MTP.md#resident-verify-rollback`; the tier seat is `install_rdec_rollback`). Metal's reject replays on its shadow region inside its round seat.
- **The joint speculative tick is Metal-only.** `register_mtp_spec_batch_override("metal", ...)`
  has one registrant, `metal_mtp_spec_eval_batch`: the scheduler's tick hands every speculative
  stream to it and one same-slab verify carries all their rows (`ARCHITECTURE_GPU_MTP.md#mtp-joint-verify`); on Vulkan and the CPU the tick steps each stream through its own round.
- **The NextN draft seat is Vulkan-only.** `register_mtp_draft_override("vulkan", ...)` has one registrant, the resident driver's head (`ARCHITECTURE_GPU_VULKAN_MTP.md#resident-draft-head`); Metal drafts inside its round seat. Each backend's owner (`register_mtp_seat_owner`, `mtp_seats_own`) claims the round its own way: Metal's a blob model alone, so a planar one under the metal overrides runs the CPU round, Vulkan's the resident driver armed on a NextN model. The head's prompt warm is a seat on Vulkan alone (`install_rdec_head_warm`: the window chain's extra layer, `ARCHITECTURE_GPU_VULKAN_MTP.md#resident-head-prompt-warm`), where Metal's prefill driver warms the slab inside its own loop; the CPU warm stands down behind either once the driver landed the logits (`prefill_override_logits_done`).
- **Lens depth**: both lenses generate `enc_*` builders from kernel classes - Metal via
  `[metal_dispatch]`, Vulkan via `[vk_dispatch]` (per-class set layouts + push constants, and
  NonWritable derived per binding from the access classification - `ARCHITECTURE_GPU_VULKAN.md#vk-readonly-lens` carries the rule, its refusal and its reading; Metal lowers a read role to `device const`
  already) - and both speak the multi-kernel form (`kernel=` names the method, one macro instance per kernel, declared roles must cover every kernel).
- **`family=` is Vulkan-only.** A vulkan family shares the per-class surface - the `VkdClass`
  global, the `set_*` builder, the pipe slots - across classes with one binding layout. Metal's
  `enc_*` builder is the entire generated surface, so there is nothing for a family to share;
  cross-class PSO/source sharing on Metal is a PSO-lifecycle question, not a lens one.
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
  a dense model's FFN gets its own line (the per-op tier has no dense-FFN rail), and the
  per-call resident overrides say each pass-to-CPU reason once per armed model
  (`rdec_pass_once`) and count every pass by reason in the tier (`gpu_cpu_passes`, zeroed as a
  model arms), which the server's `/v1/stats` carries as `gpu_cpu_passes`. The gap is `followup_vulkan.md` item 1, not a precedent to copy.
- **Device-home sessions are Vulkan-only.** A session whose K/V lives only in a region of the
  resident driver's mirror (`create_device_session`, the scheduler's device mode, park and
  adopt: `ARCHITECTURE_GPU_VULKAN_RESIDENCY.md#resident-plan`) has no Metal twin, and
  `gpu_device_sessions` answers 0 there: Metal's whole-forward driver reads the host cache in
  unified memory, so it has no mirror to split, and the batched row homes its streams on the host
  (`ARCHITECTURE_MEASUREMENT.md#one-benchmark-rig`).
- **The device-side token-embedding gather is Vulkan-only.** The engine asks one probe before
  it embeds (`register_embed_gpu_gate`, `dasllama_common.das`); on true it stashes the token
  ids, skips the CPU embed loop, and the resident driver gathers the rows on device through
  the ids-form prefill. That prefill installs SEPARATELY from the resident driver bundle
  (`install_rdec_prefill_ids`), so a tier without it never arms the gate, and a late decline
  backfills the stash on the CPU in `forward_prefill_body`. The arms mirror `embed_row`'s
  ladder: a tied q8 table gathers from the resident cls plane, a raw f32 table uploads whole
  to a device plane under a size cap the residency plan counts first, and the kq ladder keeps
  the CPU embed. What leaves the window wall is the CPU embed loop and the x upload - the ids ride a
  4-byte-per-row upload instead. Metal has no twin: its whole-forward driver embeds host-side.
- **The deltanet-resident seats are Vulkan-only.** The whole-model driver's recurrent arm
  installs SEPARATELY from the resident bundle (`install_moe_gpu_resident_dn`: a recurrent
  layer's plane set, its beta/alpha rows, the per-token owner bind and the prefill's slot
  handoff), so a tier without the seats declines a recurrent layer by name
  (`resident_layer_decline`) and the per-op rails serve it. Metal has no seat to install: its
  whole-forward driver carries the recurrent branch inside its layer encoder.
- **The device argmax pick is Vulkan-only** (`ARCHITECTURE_GPU_VULKAN_RESIDENCY.md#logits-transfer-queue`): a
  bare-argmax stream's token id lands in place of its logits row, its speculative round's rows too; Metal lands every row's logits.
