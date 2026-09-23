# dasLLAMA Architecture - GPU backends

Companion to `ARCHITECTURE.md`; section numbers are that document's. This document carries
section 1.5: the GPU backend role table with its closed asymmetry lists.
`ARCHITECTURE_GPU_RACE_SHAPES.md` beside it carries section 2.2b - the tensor-GEMM and
fused-attention shapes that measured out. The Metal tower's attention routes and its encode
chains are `ARCHITECTURE_GPU_TOWER.md` sections 2.2w-2.2x.

### 1.5 GPU backends {#gpu-backends}

A GPU backend is a FAMILY of role files - matching things in matching files across backends, so
that a question answered for one backend has an obvious address in the other. The roles:

| role | holds | must not hold |
|---|---|---|
| the kernel home<br>`dasllama_metal_kernels`, `dasllama_vulkan_classes` | kernel source, the kernel-side quant-decode helpers and the per-word codebook accessors (`[grid_words]` bakes each from `dasllama_kqformat`'s one literal at compile time - the bytes live there, the home carries the baked copy), the derived-access/PSO census; on Vulkan the one device buffer kernel data fills (`kq_grid_dev`, the grid codebooks) and the host-side ensure/set/enc pick ladders and grid rules over its own class stamps (`gemv_*`, `q8_gemv_gu_n_*`, `q8_batch_cls_*`, `kq_batch_cls_*`, `fa_stamp_*`, `f16_gemm_*`, `da_slab_*`, and the tile trios `khr_cls_*` / `cm2e_cls_*` / `cm2_cls_*` that `kq_tile_stamp` stamps over `KqFmt`) | device state other than `kq_grid_dev`, engine types |
| `dasllama_<gpu>_common`<br>`dasllama_metal_common`, `dasllama_vulkan_common` | device state, buffer/command plumbing, hazard + capture rail, profiler, host-side quant-decode helpers (Metal's `iq4_lut`), the family's registrant of a tier seat that names a size the device state keeps (Metal's `dn_mirror_room`) | driver policy |
| `dasllama_<gpu>_decode`<br>`dasllama_metal_decode`, `dasllama_vulkan_decode` | the resident token-step driver + decode-time arms | kernel bodies |
| `dasllama_<gpu>_prefill`<br>`dasllama_metal_prefill`, `dasllama_vulkan_prefill` | the batched prefill driver + batch arms | kernel bodies |
| `dasllama_<gpu>_shapes`<br>`dasllama_metal_shapes` | PORTABLE servability gates - no GPU C++ require, so any box can bake | device calls |
| the tower driver<br>`dasllama_metal_tower` | one-shot embedder/encoder encodes (gemma4uv chain, the gemma4v ViT, gemma3v SigLIP and qwen3v block loops - qwen3v adds the vision NEOX rope, the fused-qkv weight-offset GEMMs, and the inline deepstack tap + tail merger chains - the whisper-class block loop, the qwen25v window ViT, the gemma4a Conformer and canary FastConformer chains with their mel/conv fronts, the conv frontends + the qwen3a padded-weight slab and GPU front/mel) - no session, no KV, no mirror; registers the gemma4uv, gemma4v, gemma3v, qwen3v, qwen25v, encoder_blocks, tower-conv, qwen3a-front, qwen3a-mel, gemma4a, gemma4a-chunk and canary hooks | decoder state |
| the ASR-decoder driver<br>`dasllama_metal_asr_dec` | the whisper decoder on Metal: the 34B weight blob, the f16 resident cross/self K/V, window-granular cross-KV + decode-step serves; registers the whisper cross-KV and decode hooks (family registries in `dasllama_whisper`) | kernel bodies, LLM session state |
| the assistant-drafter driver<br>`dasllama_metal_mtp_gemma` | the gemma-4 assistant drafter on Metal: the sidecar blob upload, the Q-only layer chain reading the TARGET mirror at the two capture layers with the decode's own attention kernels, the speculative round over the batch driver's same-slab verify; registers the `metal` round override and delegates head-less-drafter-less models to `metal_mtp_spec_round` | kernel bodies, mirror ownership |
| the kernel-access lens<br>`dasllama_metal_lens` (Metal), `dasllama_vulkan_dispatch` (Vulkan - the `[vk_dispatch]` macro derives access per class) | the kernel-access macro and its dispatch-support macros (`compile_stamp`, `release_handles`) | anything else |

- **Vulkan additionally has an ENTRY, `dasllama_math_vulkan.das`** - capability probe/arm, `.dlim`
  identity source, cross-arm routers, the `[init]` installs. It re-exports the family `public`,
  and its NAME is the transformer umbrella's `?vulkan` require contract (deliberately LAST in the
  umbrella: the vulkan drivers are the hot-edit modules, and require order is the jit obj-cache
  layout): **never rename it.**
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
- **Backend-only capabilities live in their matching ROLE file, not in new grab-bags** - vulkan's
  weight arena, streamed mirrors, heat cache, host-import, coopmat; metal's blob transform and MTP.
- **The tower driver owns NO PSOs.** Its kernels (LN, f32 mul_mm, the two gelu flavors,
  posadd, the gemma4v clamp / rope2d / GEGLU-quick, the head restride - gemma3v's and, offset-bound, the qwen3v/prefill slicers) live in the kernel home, so `metal_decode_init` compiles and `metal_kernels_release`
  releases them like every other registry PSO; the borrowed prefill builders (`pf_enc_bf16_mm`,
  `pf_enc_hmm`, `pf_enc_rms`, `enc_cvt_half`, `enc_add_bias_rows`, `enc_rope` - the qwen3v
  vision NEOX apply - the attention trio `enc_qk_mm`/`enc_rowstat`/`enc_av_mm`, the tower
  specials `enc_tower_flash`/`enc_tower_kv_hc`/`enc_tower_win_attn`/`enc_tower_row_gather`,
  and the `enc_g4a_*`/`enc_cn_*`/`enc_q3a_*` chain families; this list is the closed borrowed
  set `REVIEW_TOWER.md`'s rules key on) come up through
  `metal_prefill_pso_init`, prefill's public bring-up seat, and `plane_buffer` in common is
  public for the same wrap-a-plane reason. The tower-only kernel classes COMPILE in the
  prefill file beside the builders that bind their PSOs - the same convergence debt sec.1.5's
  kernel-home entry ledgers, not a new placement. The tower's own objects (the ones buffer,
  its scratch pool, the qwen3a padded-weight slab) release through `metal_tower_shutdown`,
  and the slab additionally drops with the weights epoch through the tower's reload prep.
- **The tower driver is a Metal-only role** - Vulkan has no tower twin; audio/vision encodes
  on the Vulkan tier stay CPU (the gemma4v ViT, gemma3v SigLIP and qwen3v block loops
  included: on Vulkan and on plain CPU boxes those towers serve their q8 lanes). Likewise the non-causal media span: Metal serves it through
  `AttnArgs.uend` - including the FUSED image turn (head + media rows + tail as ONE eval, the
  per-query mask through `AttnArgs.ulo`); the Vulkan resident prefill declines span evals
  (`followup_general.md` #23's remaining half) and registers the split-span capability
  (`register_prefill_override_split_span`), so `eval_embd_span_` keeps the three-eval splice
  while vulkan is the active override. The qwen mrope quantum rides the same shape through a
  second capability seat (`register_prefill_override_mrope_tables`): Metal's `enc_rope` reads
  the per-token table rows `prefill_rope_tables` builds from the grid map, so it serves mrope
  unchanged and registers the seat; an override without it (vulkan builds angles from a scalar
  position) declines the quantum to the CPU loop by name. The deepstack quantum is the third
  seat (`register_prefill_override_ds_adds`): Metal uploads the caller's wide quantum WHOLE
  (the `Session.wide_src` borrow), slices x and the slice-major ds planes on-device through
  the offset-bound head restride, and encodes one `enc_add` at the slice offset after each
  tapped layer's residual - no new kernel (the CPU-side split, `ds_split_quantum`, survives
  as the CPU-loop fallback and the warm/MTP edge); an override without the seat declines
  deepstack quanta by name, so Metal serves them and Vulkan does not. The E-series PLE pre-step
  is two gate seats, one per direction: `register_ple_gpu_gate` for a prefill override that builds
  the side input on device off the stashed token ids (Metal and Vulkan register it),
  `register_ple_gpu_decode_gate` for a decode override that gathers the token's row on device
  (Vulkan alone); the hub skips the CPU pre-step only for the direction whose gate answers yes,
  so the Metal decode keeps reading the CPU-built side input.
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
  head of 64 idles the lanes past it (zero query, no store) - serving both on the f16/f32 mirrors
  with no per-head stamp; the block codecs keep the chunked per-(row, head) pair at head 64.
- **Family-shared kernel classes live in `dasllama_metal_kernels`.** The `[metal_dispatch]` lens
  generates `enc_*` builders and MSL globals into the module the class COMPILES in, so co-location
  follows the class, never "the builder needs the driver module". Prefill's prefill-only classes are convergence debt, not precedent.
  A per-format family stamp is `[metal_dispatch(stamp = "<family>:<fmt>:<form>")]` (`kq_mm`, `moe_mm`,
  `moe_mm_split`): the lens derives every string the long form spells, an explicit argument wins, and the
  threadgroup-memory global is always `<Class>_<kernel method>_msl_tgmem`. Hosts compile through
  `compile_stamp(<stem>_msl, ok)` - one spelling, so a source never pairs with another kernel's entry.
  A stamp's `params=` are its family's shared builder signature - the forms are taken by address
  into one table - so the lens's unread-param refusal (a `params=` name no `grid=`, `tg=`,
  `requires=` or `@span` reads) exempts stamp-derived params; a long-form class is refused.
- **Ledgered kernel-binding asymmetries** - a REVIEW rule firing on one of these is expected, and
  this entry is the sanction: the moe mul_mm TENSOR twins (`MetalMoeMulMmQ8T` / `MetalMoeMulMmMx4T`)
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
  their N leaves read it, every other stamp (k2, k3, k4, k5, k6, q40, iq4xs, iq4nl) binds it unread.
- **`dasllama_gpu_tier.das`** - the device-cooperation SPI: hook types, install/unset slots, route/mark/want/status
  state, engine-facing forwarders. Vulkan implements it (per-op offload plus resident plumbing, and the decode-era
  seats it alone fills: the cm2 expert chain `set_moe_gpu_ffn_xf_hooks` / `_async_hooks`, the decode attention block
  `set_moe_gpu_attn_dec_hooks`, the decode FFN tail `set_moe_gpu_ffn_tail_hooks`, the deltanet decode step's state
  seams `set_moe_gpu_dn_state_hooks` (flush, invalidate, release), the whole-token span `set_moe_gpu_span_dec_hook` -
  the span rides common's decode override registry as `vulkan_moe_span`, selected by the MoE placement and declining
  per token - the resident driver's q/k/v projection-bias seat `install_moe_gpu_resident_bias`, its attention-sink seat
  `install_moe_gpu_resident_sinks` (the per-head sink plane, `ARCHITECTURE_GPU_VULKAN_ATTN.md` sec.2.2am), its MoE seats
  `install_moe_gpu_resident_moe` (the tile admission per expert triple, the routing geometry with the router plane, an
  MoE layer, and the routed block on a layer another seat built), its mirror-region seat `install_moe_gpu_resident_regions`
  (`rdec_select_region` names the region every mirror address and the next token command resolve against; a tier
  without it serves one region), its N-row batch seat `install_moe_gpu_resident_batch` (the N-row token command beside
  the resident driver: `rows` answers how many rows it steps at once on the armed model, 0 = none; `rdec_token_n` steps
  them), all behind the route lever `set_gpu_resident_route` / `gpu_want_resident`, the
  OS video-memory seat `install_moe_gpu_os_memory` the residency plan sizes against, the weight-bytes seat
  `install_rdec_note_weight_bytes` the decode warm-up guard reads, and the per-layer-embedding seats `install_rdec_ple`
  - the branch's width, its per-layer gate and proj planes, the pre-step's projection and the token table on the
  device). The installs are one-way (a test that arms the tier never restores them): no uninstall exists and none is
  needed, a seat serves whatever model loads next. Metal deliberately does not - UMA makes residency moot there, and
  Metal integrates as a whole-forward driver through common's override registries (the ASR-decoder driver is the one
  exception: whisper is not a `Model`, so its hooks are family registries in `dasllama_whisper`, same decline contract).
- **`dasllama_gpu_resident.das`** - the WHOLE-MODEL residency rail: bake the device layout offline into the flavor
  image, upload a model's stacks to the tier, and drive decode/prefill entirely on device. It is device-AGNOSTIC - it
  holds no device call and requires no GPU module, reaching the hardware only through the `dasllama_gpu_tier` SPI and
  entering the engine only through common's override registries. `"vulkan"` is the tier string it registers under, not
  a dependency, which is why it compiles on every box. It requires common back for `Model`/`Session`, so like the
  Metal drivers it is required from the transformer umbrella, never from common.
- **A dry bake runs the whole resident arm with no device.** `vulkan_bake_role` puts the tier in
  bake mode, and each `rdec_*` device seam answers for itself so the arm walk reaches the end: a
  seam that only records a layout (`vk_rdec_set_emb`) answers true, a seam that would allocate
  device memory (`vk_rdec_upload_emb_f32`) answers false without touching a device. The split is
  what keeps a baked `.dlim` layout equal to the one a real arm produces.
- **`dasllama_kernel_access.das`** - the shared body-walk read/write classifier both GPU lenses run
  on, plus the dispatch-lens micro-grammar (the grid/tg/params spec tokenizers and the shared
  AST-emission core: `is_digit_tok`, `role_ok`, `derived_role`, `mk_uint_cast`, `mk_call1`,
  `mk_grid_dim`, `param_type`). One owner by design: a private copy per lens drifts (metal folding
  only the literal "1" where vulkan folds any integer). Backend-specific lowering stays in that backend's lens.
- **The authoritative site of each constant kind.** Tile constant in a kernel body: the literal in the generated `*_msl` global or the
  SPIR-V dump (`DASLLAMA_VK_SPV_DUMP=<dir>` writes every class kernel's words). Grid constant: the class's `[metal_dispatch]` / `[vk_dispatch]`
  `grid=` spec (`"n/c"` is a CEIL-divide); a `grid = "wgs"` class carries no number there - its grid is the kernel body's workgroup-index
  decode with the host helper that computes `wgs`. Threadgroup constant: Metal's `tg=` spec or Vulkan's `[spirv_kernel(local_size_x=)]`. Uniform: the single writer that fills its buffer.

**PSO lifecycle - the family shares ONE device and queue** (`metal_common_init`; the second-device
question was surveyed and closed against; the tune-time race arms' transient queue is
`ARCHITECTURE_MEASUREMENT_KERNEL_RACE.md` sec.2.21's). The decode PSO set lives as `g_pso_*` in
`dasllama_metal_common`, is compiled by `metal_decode_init` in `dasllama_metal_kernels` and
released by `metal_kernels_release` there - the kernels module owns its set's lifecycle even
though the vars live with the device state. Prefill's `g_pf_pso_*` set is prefill-private end to
end: `metal_prefill_init` compiles, `metal_prefill_shutdown` releases.

**Race and tune code for a kernel family lives beside the family.** The shared scaffolding
(`race_buf`, `race_envelope_ok`, `race_pair_ms`, `MetalTensorRaceResult`) is `<gpu>_common`'s;
each module races its OWN families (`metal_tensor_race_decode` in kernels, `metal_tensor_race`
in prefill) and the tuner calls those public entries.

**Decline REASONS are enum values in the shapes module** (`MetalDecodeDecline`, `MetalPrefillDecline`);
decline COUNTING lives in `<gpu>_common` beside `require_or_panic`, for both paths.

Sections 2.28-2.29, 2.33-2.37a and 2.39 - the Metal speculative round, the depth a round drafts, and
the verify, drafter and batch-driver mechanics - are `ARCHITECTURE_GPU_MTP.md`; sections 2.30-2.32,
2.38-2.38a and 2.39a - the decode driver's kernel forms, pre-encoded steps and layer encoder - are `ARCHITECTURE_GPU_MTP_DECODE.md`.

**The allowed asymmetries between the backends - this list is closed; a new one lands with its entry here:**

- **The `dasllama_gpu_tier` cooperation SPI is Vulkan-only**: every hook seat the tier
  exposes (`install_moe_gpu_tier` and the `set_moe_gpu_*_hooks` setters) is registered by the
  Vulkan family alone, and the role row above enumerates the seats; a new seat lands in that
  row, not as a new entry here. The one seat outside that rule is the entry below.
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
  collecting the set over a CPU-only window, `ARCHITECTURE_RUNTIME.md` sec.2.12) - a
  driver-cost shield, not a placement mechanism; memory is still memory.
- **The weights-epoch drop is Metal-only.** `bump_weights_epoch`'s listener seat
  (`register_weights_epoch_listener`) has one subscriber: `_common`'s `metal_weights_drop`, which runs the
  registered reload preps (`register_reload_prep`; the decode driver registers `discard_pre_encoded_steps`),
  quiesces, and releases the address-keyed region caches. Vulkan's reload story is the unmap notify
  (`set_moe_gpu_unmap_notify`) - a different seam for a different ownership model.
- **The batched pre-encoded step is Metal-only**: the batch driver encodes the next step under the
  current one's GPU run (`ARCHITECTURE_GPU_MTP_DECODE.md` sec.2.38a, `DASLLAMA_METAL_BATCH_PRE`);
  Vulkan's N-row token command records once and resubmits, so it has no encode to move.
- **The speculative round is Metal-only.** `register_mtp_round_override("metal", ...)` has one
  registrant, `gemma_mtp_spec_round` (falling through to `metal_mtp_spec_round` with no drafter);
  the same-slab verify and the NextN draft forward exist only in the Metal decode driver, and
  Vulkan serves the CPU round (`ARCHITECTURE_GPU_MTP.md`).
- **The joint speculative tick is Metal-only.** `register_mtp_spec_batch_override("metal", ...)`
  has one registrant, `metal_mtp_spec_eval_batch`: the scheduler's tick hands every speculative
  stream to it and one same-slab verify carries all their rows (`ARCHITECTURE_GPU_MTP.md`
  sec.2.37a); on Vulkan and the CPU the tick steps each stream through its own round.
- **Lens depth**: both lenses generate `enc_*` builders from kernel classes - Metal via
  `[metal_dispatch]`, Vulkan via `[vk_dispatch]` (per-class set layouts + push constants, and
  NonWritable derived per binding from the access classification - `ARCHITECTURE_GPU_VULKAN.md`
  sec.2.2aj carries the rule, its refusal and its reading; Metal lowers a read role to `device const`
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
  adopt: `ARCHITECTURE_GPU_VULKAN_RESIDENCY.md` sec.2.2n) has no Metal twin, and
  `gpu_device_sessions` answers 0 there: Metal's whole-forward driver reads the host cache in
  unified memory, so it has no mirror to split, and the batched row homes its streams on the host
  (`ARCHITECTURE_MEASUREMENT.md` sec.2.5).
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
- **The device argmax pick is Vulkan-only** (`ARCHITECTURE_GPU_VULKAN_RESIDENCY.md` sec.2.2an): a
  bare-argmax stream's token id lands in place of its logits row; Metal lands every row's logits.

Vulkan is the deliberately-designed model of this shape; Metal converges as it is touched.

The Vulkan resident driver's sections live in its companions, each head saying what it holds: 2.2j,
2.2p, 2.2ab, 2.2ac, 2.2ad, 2.2ai and 2.2aj in `ARCHITECTURE_GPU_VULKAN.md`; 2.2al and 2.2am in
`ARCHITECTURE_GPU_VULKAN_ATTN.md`; 2.2k-2.2m, 2.2q, 2.2ae and 2.2ah in
`ARCHITECTURE_GPU_VULKAN_GEMM.md`; 2.2n-2.2o and 2.2an in `ARCHITECTURE_GPU_VULKAN_RESIDENCY.md`; 2.2ao-2.2ap in
`ARCHITECTURE_GPU_VULKAN_NROW.md`; 2.2r-2.2v in
`ARCHITECTURE_GPU_VULKAN_DECODE.md`; 2.2af, 2.2ag and 2.2ak in `ARCHITECTURE_GPU_VULKAN_MOE.md`.

Section 2.2b, the tensor-GEMM and fused-attention shapes that measured out, is
`ARCHITECTURE_GPU_RACE_SHAPES.md`; sections 2.2w-2.2x, the tower attention routes and the tower
driver's encode chains, are `ARCHITECTURE_GPU_TOWER.md`; sections 2.2y-2.2z, the Metal quant plane
reads, are `ARCHITECTURE_GPU_QUANT_PLANES.md`.
