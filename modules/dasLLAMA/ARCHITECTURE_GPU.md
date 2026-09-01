# dasLLAMA Architecture - GPU backends and refuted kernel shapes

Companion to `ARCHITECTURE.md`; section numbers are that document's.

### 1.5 GPU backends {#gpu-backends}

A GPU backend is a FAMILY of role files - matching things in matching files across backends, so
that a question answered for one backend has an obvious address in the other. The roles:

| role | holds | must not hold |
|---|---|---|
| the kernel home<br>`dasllama_metal_kernels`, `dasllama_vulkan_classes` | kernel source, the derived-access/PSO census | device state, engine types |
| `dasllama_<gpu>_common`<br>`dasllama_metal_common`, `dasllama_vulkan_common` | device state, buffer/command plumbing, hazard + capture rail, profiler, shared quant-decode helpers (`iq4_lut`) | driver policy |
| `dasllama_<gpu>_decode`<br>`dasllama_metal_decode`, `dasllama_vulkan_decode` | the resident token-step driver + decode-time arms | kernel bodies |
| `dasllama_<gpu>_prefill`<br>`dasllama_metal_prefill`, `dasllama_vulkan_prefill` | the batched prefill driver + batch arms | kernel bodies |
| `dasllama_<gpu>_shapes`<br>`dasllama_metal_shapes` | PORTABLE servability gates - no GPU C++ require, so any box can bake | device calls |
| the tower driver<br>`dasllama_metal_tower` | one-shot embedder/encoder encodes (gemma4uv chain, the gemma4v ViT, gemma3v SigLIP and qwen3v block loops - qwen3v adds the vision NEOX rope, the fused-qkv weight-offset GEMMs, and the inline deepstack tap + tail merger chains - the whisper-class block loop, the qwen25v window ViT, the gemma4a Conformer and canary FastConformer chains with their mel/conv fronts, the conv frontends + the qwen3a padded-weight slab and GPU front/mel) - no session, no KV, no mirror; registers the gemma4uv, gemma4v, gemma3v, qwen3v, qwen25v, encoder_blocks, tower-conv, qwen3a-front, qwen3a-mel, gemma4a, gemma4a-chunk and canary hooks | decoder state |
| the ASR-decoder driver<br>`dasllama_metal_asr_dec` | the whisper decoder on Metal: the 34B weight blob, the f16 resident cross/self K/V, window-granular cross-KV + decode-step serves; registers the whisper cross-KV and decode hooks (family registries in `dasllama_whisper`) | kernel bodies, LLM session state |
| the kernel-access lens<br>`dasllama_metal_lens` (Metal), `dasllama_vulkan_dispatch` (Vulkan - the `[vk_dispatch]` macro derives access per class) | the kernel-access macro | anything else |

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
  metal_common -> dasllama_common -> metal_gemm would cycle.
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
  deepstack quanta by name, so Metal serves them and Vulkan does not.
- **Per-layer FFN widths (MatFormer E-series, at most two - `ffn_second_hidden`) serve on Metal
  only**: the decode and prefill drivers bind the width per layer (dense trunks, no MTP; batch
  keeps the layer-0 hoist behind its uniformity decline). The Vulkan tier has no PLE arm, so
  E-series never reaches its width question.
- **Family-shared kernel classes live in `dasllama_metal_kernels`.** The `[metal_dispatch]` lens
  generates `enc_*` builders and MSL globals into the module the class COMPILES in, so co-location
  follows the class - "the builder needs the driver module" is never a placement reason. Prefill's
  prefill-only classes are convergence debt, not precedent.
- **Ledgered kernel-binding asymmetries** - a REVIEW rule firing on one of these is expected, and
  this entry is the sanction: the moe mul_mm TENSOR twins (`MetalMoeMulMmQ8T` /
  `MetalMoeMulMmMx4T`) keep the pre-family compact kargs slots while their base classes bind the
  family numbers, so no shared bind path may span the two layouts; and the in-engine moe mul_mm
  A/B race harnesses (`dasllama_metal_prefill.das`) encode through `kn_moe_mm_family_tail`
  rather than a per-class `enc_*` builder.
- **`dasllama_gpu_tier.das`** - the device-cooperation SPI: hook types, install/unset slots,
  route/mark/want/status state, engine-facing forwarders. Vulkan implements it (per-op offload plus
  resident plumbing, and the decode-era seats it alone fills: the cm2 expert chain
  `set_moe_gpu_ffn_xf_hooks` / `_async_hooks`, the decode attention block
  `set_moe_gpu_attn_dec_hooks`, the decode FFN tail `set_moe_gpu_ffn_tail_hooks`, the
  whole-token span `set_moe_gpu_span_dec_hook` - the span rides common's decode override
  registry as `vulkan_moe_span`, selected by the MoE placement and declining per token). The
  installs are one-way: a test that arms the tier installs the seats and never restores them,
  because no uninstall exists and none is needed - a seat serves whatever model loads next; Metal
  deliberately does not, because UMA makes residency moot there and Metal
  integrates as a whole-forward driver through common's override registries (the ASR-decoder
  driver is the one exception: whisper is not a `Model`, so its hooks are family registries in
  `dasllama_whisper`, same decline contract).
- **`dasllama_gpu_resident.das`** - the WHOLE-MODEL residency rail: bake the device layout offline
  into the flavor image, upload a model's stacks to the tier, and drive decode/prefill entirely on
  device. It is device-AGNOSTIC - it holds no device call and requires no GPU module, reaching the
  hardware only through the `dasllama_gpu_tier` SPI and entering the engine only through common's
  override registries. `"vulkan"` is the tier string it registers under, not a dependency, which is
  why it compiles on every box. It requires common back for `Model`/`Session`, so like the Metal
  drivers it is required from the transformer umbrella, never from common.
- **A dry bake runs the whole resident arm with no device.** `vulkan_bake_role` puts the tier in
  bake mode, and each `rdec_*` device seam answers for itself so the arm walk reaches the end: a
  seam that only records a layout (`vk_rdec_set_emb`) answers true, a seam that would allocate
  device memory (`vk_rdec_upload_emb_f32`) answers false without touching a device. The split is
  what keeps a baked `.dlim` layout equal to the one a real arm produces.
- **`dasllama_kernel_access.das`** - the shared body-walk read/write classifier both GPU lenses run
  on, plus the dispatch-lens micro-grammar (the grid/tg/params spec tokenizers and the shared
  AST-emission core: `is_digit_tok`, `role_ok`, `derived_role`, `mk_uint_cast`, `mk_call1`,
  `mk_grid_dim`, `param_type`). One owner by design - the two lenses' grammars drifted apart
  when each carried a private copy (metal folded only the literal "1" where vulkan folded any
  integer). Backend-specific lowering stays in that backend's lens.

**PSO lifecycle - the family shares ONE device and queue** (`metal_common_init`; the second-device
question was surveyed and closed against). The decode PSO set lives as `g_pso_*` in
`dasllama_metal_common`, is compiled by `metal_decode_init` in `dasllama_metal_kernels` and
released by `metal_kernels_release` there - the kernels module owns its set's lifecycle even
though the vars live with the device state. Prefill's `g_pf_pso_*` set is prefill-private end to
end: `metal_prefill_init` compiles, `metal_prefill_shutdown` releases.

**Race and tune code for a kernel family lives beside the family.** The shared scaffolding
(`race_buf`, `race_envelope_ok`, `race_pair_ms`, `MetalTensorRaceResult`) is `<gpu>_common`'s;
each module races its OWN families (`metal_tensor_race_decode` in kernels, `metal_tensor_race`
in prefill) and the tuner calls those public entries.

**Decline REASONS are enum values in the shapes module** (`MetalDecodeDecline`,
`MetalPrefillDecline`); decline COUNTING lives in `<gpu>_common` beside `require_or_panic`, for
both paths.

**The allowed asymmetries between the backends - this list is closed; a new one lands with its
entry here:**

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
  (`register_weights_epoch_listener`) has one subscriber: `_common`'s `metal_weights_drop`,
  which runs the registered reload preps (`register_reload_prep`; the decode driver registers
  `discard_pre`), quiesces, and releases the address-keyed region caches. Vulkan's reload
  story is the unmap notify (`set_moe_gpu_unmap_notify`) - a different seam for a different
  ownership model.
- **Lens depth**: both lenses generate `enc_*` builders from kernel classes - Metal via
  `[metal_dispatch]`, Vulkan via `[vk_dispatch]` (per-class set layouts + push constants; the
  class-kernel arc retired the hand-built 6-slot set ladders outright) - and both speak the
  multi-kernel form (`kernel=` names the method, one macro instance per kernel, declared roles
  must cover every kernel).
- **`family=` is Vulkan-only.** A vulkan family shares the per-class surface - the `VkdClass`
  global, the `set_*` builder, the pipe slots - across classes with one binding layout. Metal's
  `enc_*` builder is the entire generated surface, so there is nothing for a family to share;
  cross-class PSO/source sharing on Metal is a PSO-lifecycle question, not a lens one.
- **`@default` is Metal-only.** A `[metal_dispatch]` field may name a fallback global
  (`@default = g_one`) that the generated builder binds when the caller passes null;
  `[vk_dispatch]` has no counterpart - vulkan callers pass a real buffer at every slot. If
  vulkan grows an optional-bind shape, it lands as this same annotation, not a new spelling.
- **The workgroup-footprint gate is Vulkan-only.** `[vk_dispatch]` sums a class's `@workgroup`
  members and its generated `ensure_*` declines by name (`vkd_wg_fits`) before the pipeline
  build, because MoltenVK's over-cap failure is an opaque `INITIALIZATION_FAILED` - and the
  resident driver declines residency with it (`vk_rdec_prepare`). `[metal_dispatch]` has no
  footprint gate: Metal's own pipeline compile fails loudly with the footprint in the error.
- **Vulkan has no shapes module yet** - `resident_upload` declines ad hoc by feature name; the
  gap is `followup_vulkan.md` item 1, not a precedent to copy.
- **The device-side token-embedding gather is Vulkan-only.** The engine asks one probe before
  it embeds (`register_embed_gpu_gate`, `dasllama_common.das`); on true it stashes the token
  ids, skips the CPU embed loop, and the resident driver gathers the rows on device through
  the ids-form prefill. That prefill installs SEPARATELY from the resident driver bundle
  (`install_rdec_prefill_ids`), so a tier without it never arms the gate, and a late decline
  backfills the stash on the CPU in `forward_prefill_body`. The arms mirror `embed_row`'s
  ladder: a tied q8 table gathers from the resident cls plane, a raw f32 table uploads whole
  to a device plane under a size cap the residency plan counts first, and the kq ladder keeps
  the CPU embed. What leaves the window wall is the CPU embed loop and the x upload - the ids
  ride a 4-byte-per-row upload instead. Metal has no twin: its whole-forward driver embeds
  host-side.

Vulkan is the deliberately-designed model of this shape; Metal converges as it is touched.

### 2.2b Tensor-GEMM shapes that measured out (M5, interleaved-race evidence)

The forms below were built, raced against the crowned tg-staged q8u GEMM at real model
shapes, and LOST; the probe that holds the GEMM numbers is
`benchmarks/matmul/bench_metal_nax_probe.das`, and the fused-attention numbers live in
`benchmarks/attn/bench_metal_pf_fused_attn.das`. Re-attempting one without new structure is
re-buying a measured loss.

The probe also RETAINS the decided-and-shipped arms (the half-A stream, the dev-W all-device
form, the tall M-tile twin, the bk staging depths, the no-zero-init form) as hand-written MSL
reference implementations beside the refuted ones - they are the arc's bisect ledger, and the
sync duty is `REVIEW_GPU.md`'s. The attention lab keeps the same class of retained references:
its PORT/PORTH/PORTC arms and `benchmarks/attn/lcpp_flash_dk72.metal` are the oracle-exact
external reference the shipped `MetalTowerFlash` was decided against, and the DIAL arm races
the production dialect against them - the bisect seat when the flash regresses.


- **Per-simdgroup register-fragment matmul2d (16x32x16, device -> `vec<T,8>` fragments):**
  1.7-2.0x slower for weight GEMMs, vectorized loads and deep n-blocking included. The
  fragment architecture pays for attention's streaming operand reuse (one resident Q against
  a K/V walk), not for a GEMM's operand traffic - cooperative tg staging wins there.
- **Mixed-integer matmul2d operands (float x int8, and the i8 x i8 per-block-fold form):**
  3.5x slower - the mixed-int combinations exist in MPP's type lists but lower off the NAX
  fast path. W8A8 claims from other stacks do not transfer through MPP.

**Sanctioned float-A stamps** - the kernel classes stamped `[metal_kernel(float_a_ok=true)]`:
every tensor template's `XT = float` stamp - the live fallback wherever the half panel is absent
(below the convert row floor, panel does not fit, half-X pinned off) - the
batch-decode/classifier `MetalQ8GemmTensorT` family, whose half-X extension is an open ledger
item, and the double-buffered `*Db` staging templates (`MetalQ8MulMmDbT`, `MetalKqMulMmK45DbT`,
`MetalKqMulMmK6DbT`), which pin `XT = float16` today - there the flag is scaffolding a future
float stamp would need, not a live float operand.
- **Fused single-kernel attention (scores in threadgroup, online softmax):** loses 10-80% to
  the pipelined three-pass at real shapes (`benchmarks/attn/bench_metal_pf_fused_attn.das`) -
  Metal's cross-kernel pipelining plus full-width softmax beat tg-scope fusion.

The positive laws these races established - half operands, stage-only-to-transform,
consecutive staging runs, relaxed_precision always - are `REVIEW_GPU.md` rules and the
`modules/dasMetal/REVIEW.das` descriptor gate; this section keeps only the refuted shapes
and why they lose.

Sections 2.2j-2.2p, the Vulkan resident driver, are `ARCHITECTURE_GPU_VULKAN.md`.

### 2.2w The tower attention routes {#tower-attn-routes}

A tower head width is padded to `hs_pad = max(64, ceil32(hs))` - 72 and 80 both land on 96 -
because the QK stamps walk multiples of 32 while the guarded AV forms carry no multiple-of-64
requirement. A route that uses the padded width pays a restride to and from the towers' compact
rows. Three routes serve tower attention:

- **The flash route** takes a head size of exactly 72 on a canvas whose row count divides 64.
  It runs one pass - no score slab, no rowstat, no head restrides - reading K and V as
  per-head-contiguous f16 panels (the panel and tail-slack contract sits on the kernel class)
  while Q and the output ride the compact f32 rows. `DASLLAMA_METAL_TOWER_FLASH=0` pins the
  slab trio.
- **The per-window route** serves the qwen2.5 ViT's block-diagonal layers: one threadgroup per
  (window, head), one thread per query row, windows capped at 64 rows. It reads q, k and v
  compact and straight from device in f32 - a window's K and V sit in L1, and f16 staging noise
  would compound over the tower's 28 layers. Layers that attend in full restride onto the
  padded slab and take the shared trio.
- **The slab trio** - the prefill driver's QK, rowstat and AV builders - serves everything else
  and sizes its score slab at `heads x mp x nk64` halfs. Off the flash route that slab is the
  encode's only attention allocation; on it the driver takes a stub.

### 2.2x The tower driver's encode chains {#tower-encode-chains}

Each family the tower driver serves gets one chain that encodes the whole encode into ONE
command buffer and reads back once: the block loop, and - for gemma4a and qwen3a - the mel and
conv front ahead of it. A chain is dispatch for dispatch the same graph as its family's CPU
encoder loop, in the same order and at the same operand shapes, so the CPU loop is the chain's
specification and the CPU-vs-GPU transcript cells are its parity instrument. Every chain is
best-effort: it answers false (or -1) on any shape, knob, quant-mode or device decline, and the
CPU chain serves that encode. Engage is read from counter deltas (`metal_tower_stats`,
`metal_tower_f16_encodes`), never from "the model ran".

### 2.2y The Metal kq split scale plane {#metal-kq-split-scale-plane}

Every superblock format but k4, k5, q40 and iq4nl stores its Metal-blob scale row SPLIT into two
regions of one buffer: the 16-byte sub-scale strips of every superblock first, then the packed
per-superblock d tail. A kernel binds that one buffer twice - the strips at `soff = sb0 * 16` and
the tail at `doff = nsb * 16 + sb0 * 2` - so the two reads stride independently and the strip
read stays 16-byte aligned. k2 is the one shape variation: its tail is 4 bytes per superblock
(`nsb * 16 + sb0 * 4`), because it carries d and dmin. `kq_scales_of` builds the pair;
`metal_blob_scale_plane` mints it at bake time, folding each format's 20-byte decoded row into
`[16B strips][2B d]` (k3's row is 18 bytes and is already in that shape). The 2-byte tail is why
a region's bind offset must be a multiple of 512 elements - the `(off/256)*2` d-plane bind is
4-byte aligned only then - which is what `metal_blob_off_ok` and `moe_site_ok` check. iq4nl is
the exception: it reuses q40's 16-byte plane of eight f16 d per superblock, binds once, and
ignores `doff`. The Vulkan tier does not use this form - it binds the decoded 20-byte row as five
uints per superblock.

### 2.2z The iquant GEMV grid read and its f4-slab twin {#metal-iquant-gemv-grid}

Every iquant Metal GEMV - iq3s, iq3xxs, iq2s, iq2xs, iq2xxs - reads its codebook grid DIRECT
off the module's hoisted constant tables and float-expands each word in place. That is the base
form and the one that ships everywhere.

Three of them carry a second form, the f4-slab twin (`MetalKqGemvIq3sF4`,
`MetalKqGemvIq3xxsF4`, `MetalKqGemvIq2xxsF4`): the whole grid staged into threadgroup memory
once per threadgroup, pre-expanded to `float4` magnitudes, signs applied by select, so the
inner loop does no byte extraction. The twin is a PER-BOX CROWN, never a heuristic and never a
default - it wins on M5-class GPUs and REGRESSES on M1/M4-class, which have less threadgroup
bandwidth to trade for the byte work. Its PSO compiles only when the box's tune manifest
carries the crown (`metal_tensor_crowned("kq_gemv_<fmt>_f4")`), and `enc_kq_gemv` picks the
twin exactly when that PSO is non-null, so a box with no crown never compiles it.

The `kq_gemv_iq3s_f4` and `kq_gemv_iq3xxs_f4` crowns are raced (`race_gemv_f4_twin`).
`kq_gemv_iq2xxs_f4` cannot be settled by an isolated race at all and is minted from a serving
A/B instead - `ARCHITECTURE_MEASUREMENT.md` sec.2.21.
