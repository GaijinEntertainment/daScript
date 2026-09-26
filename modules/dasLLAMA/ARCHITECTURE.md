# dasLLAMA Architecture

The three documents divide as follows, and the division is load-bearing - the same fact written
in two of them will drift:

| doc | audience | carries |
|---|---|---|
| `README.md` | users | what each file *is*, one line, plus how to run things |
| `ARCHITECTURE.md` | me | what *belongs* in each file and why the system is shaped this way |
| `REVIEW.md` | `/code-review`, and us while writing | criteria checkable against a diff |

A `*_plan.md` in this folder is an ACTIVE arc's working plan - a shipped arc's plan leaves the
tree.

The companions carry this document's sections. The routing block under each heading below is
the index: it names every companion that section runs to, and the sections that companion holds.

---

## File charters {#file-charters}

When placement charters disagree on one function, the charter line in this section's companion
decides. Each companion's charters sit in its own `File charters` section, anchored by topic.

A companion's sections are listed in three places: its line in this document's routing block
(under `File charters` or `Mechanisms`), the companion's own opening, and the routing sentence of
any sibling companion that sends a reader to it.

- `ARCHITECTURE_ENGINE.md` - the engine core, arch-registration, support and serving charters.
- `ARCHITECTURE_ENGINE_FORMATS.md` - the format, load-rail and CPU-kernel-tier charters, plus
  the shapes they key off - the bigram merge heap both tokenizer backends run
  (`ARCHITECTURE_ENGINE_FORMATS.md#bpe-merge-heap`), the one RoPE fill over a position source
  (`ARCHITECTURE_ENGINE_FORMATS.md#rope-one-fill`), and the Model's per-format plane table, its
  only `dim` field (`ARCHITECTURE_ENGINE_FORMATS.md#model-plane-table-dim`).
- `ARCHITECTURE_GPU.md` - the GPU backend role table and the backend asymmetries
  (`ARCHITECTURE_GPU.md#gpu-backends`).
- `ARCHITECTURE_MEDIA.md` - the encoder-tower, audio, ASR and vision charters.
- `ARCHITECTURE_TTS.md` - the text-to-speech charters - the front end, the block home, the
  shared assembly, the families and the facade.
- `ARCHITECTURE_POCKET.md` - the Pocket TTS family, a continuous-audio language model with its
  own assembly.

Every file under `dasllama/` appears in a companion. Two carry generated CONTENT rather
than being generated themselves: `dasllama_env.das` holds the `[EnvConfig]` knob declarations
(`ENVIRONMENT.md` is generated FROM it by `harness/gen_env_doc.das`), and `dasllama_unicode.das`
is hand-written around transcoded Unicode data tables (the RANGES/WS tables - regenerate by
re-transcoding `$LCPP/src/unicode-data.cpp`).

## Mechanisms

Each companion's sections are anchored by topic; a citation spells `<doc>.md#<anchor>`.

- `ARCHITECTURE_IMAGE.md` - the prepared-image rail, the baked dev-W f16 plane, the lattice-only
  per-format plane table both halves of the rail skip around, the baked tower twin-W plane, the
  layout stamp `REVIEW.das` keeps over the byte-placing code, the meta-field tripwire every
  struct serializer opens with, the planar-vs-blob flavor rebake, and the per-format slots the
  interleave identity prints.
- `ARCHITECTURE_GPU_RACE_SHAPES.md` - the tensor-GEMM and fused-attention shapes that measured
  out, the retained reference arms beside them, and the sanctioned float-A stamps.
- `ARCHITECTURE_GPU_TOWER.md` - the Metal tower attention routes, the Metal tower driver's
  encode chains, the StyleTTS2 synthesis chain, and the Pocket TTS codec and frames seats.
- `ARCHITECTURE_GPU_TOWER_VULKAN.md` - the Vulkan tower's row classes and attention routes, the
  Vulkan tower driver's encode chains, and the Vulkan ASR-decoder driver.
- `ARCHITECTURE_GPU_QUANT_PLANES.md` - the Metal kq split scale plane, the iquant GEMV grid read
  with its f4-slab twin, and the Metal GEMV site abstraction.
- `ARCHITECTURE_GPU_PREFILL.md` - the Metal prefill driver's GEMM form ladder, dev-W knee map,
  attention slab, chunked submission, the f16 twin dual-store, the last-layer FFN tail, and the
  dense-KQ tensor mul_mm scaffold.
- `ARCHITECTURE_GPU_PREFILL_MOE.md` - the Metal prefill driver's routed block - the MoE bucket
  rail, its tensor-twin scaffold, and the split-format expert twins.
- `ARCHITECTURE_GPU_VULKAN.md` - the Vulkan resident driver's prefill chain and byte stores - the
  prefill window chain, the Q8 requant byte store, the decode GEMV family's grid codebook buffer,
  the tile probe's shared descriptor set layout, the recurrent block of the prefill window, the
  device-init roster of the Vulkan capabilities the tier keys its routes on, and the
  `[vk_dispatch]` lens's readonly derivation.
- `ARCHITECTURE_GPU_VULKAN_ATTN.md` - the token command's attention key split, and the
  attention-side planes the resident driver uploads beside its norms - the q/k/v projection
  bias, gpt-oss's sink logits with the flash tiles' sink stamps, and its output bias.
- `ARCHITECTURE_GPU_VULKAN_GEMM.md` - the cooperative-matrix tiles the Vulkan tier's GEMMs run
  on - the cm2 decode lanes, the tile pick and the coopmat mode ladder, the class-pipeline build
  seat, the MoE expert chain on those tiles, the KHR arm's hand-staged kq tile - and the decode
  GEMV family's lane split by row length.
- `ARCHITECTURE_GPU_VULKAN_RESIDENCY.md` - what a model has to fit on the card before the driver
  runs - the residency plan, and the GPU-slot marks swap that lets one slot serve many models.
- `ARCHITECTURE_GPU_VULKAN_NROW.md` - the N-row token command a batched step's rows go through,
  and the residual step's two forms it holds bit for bit.
- `ARCHITECTURE_GPU_VULKAN_DECODE.md` - the per-op tier's decode era - the decode attention block
  over per-layer K/V mirrors, the streamed expert layer's GPU/CPU split, the whole-token decode
  span, the deltanet decode step's per-session resident state, and the whole-model driver's
  hybrid token command and prefill (recurrent layers, gated attention, partial rotary, the
  prefill-to-decode state handoff).
- `ARCHITECTURE_GPU_VULKAN_MOE.md` - the resident driver's routed block in its two eras - the MoE
  block of the prefill window, and the whole-model driver's MoE token command - and the gemma-4
  form both eras take.
- `ARCHITECTURE_GPU_MTP.md` - the Metal speculative round over the batch driver's same-slab
  verify, and the box knob that sets the depth a round drafts.
- `ARCHITECTURE_GPU_MTP_DECODE.md` - the kernel argument-alignment contract enforced at every
  dispatch, the K-quant small-batch crown and the row-buffer pad a multi-row verify dispatches
  under, the single-row driver's greedy chain, and the decode layer encoder.
- `ARCHITECTURE_RUNTIME.md` - kernel shape, caches, lint policy, knobs, coverage, the GPU ramp,
  the hybrid worker pool, the MoE region split, the job queue the engine dispatches on, and the
  mode that dispatches to none.
- `ARCHITECTURE_MEDIA.md` - the padded tower GEMM widths, the family GPU hooks, the tower weight
  lane, and the plain-Model ASR decoders.
- `ARCHITECTURE_MEASUREMENT.md` - the benchmark rig, the tune gate, the sanctioned
  instrumentation rails, the ASR board's GPU row pairs, the `[tuned]` perm precedence, the mint
  wall in the sidecar's provenance, the fat exe's first-start race, and the speculative round's
  ruler record.
- `ARCHITECTURE_MEASUREMENT_VK_GEMM_PROBE.md` - the Vulkan GEMM probe's arms, the shapes they run
  at, and the alternates they are read against.
- `ARCHITECTURE_MEASUREMENT_KERNEL_RACE.md` - the instruments that time a kernel away from the
  served graph - kernel-race fidelity, the gemv's own tune seat, and the CPU kernel bench's
  fixture conditions.
- `ARCHITECTURE_CPU_KERNELS.md` - the sub-block-packed k3/k6 planes, the grid formats' panel and
  row-group decodes, the VBMI symbol lattice, the tier that selects on the target rather than the
  host, the score dot classic prefill shares with decode, the module-scope visibility a lifted
  worker lambda needs of its kernel, the two interleaves a grp<mr> repack is made of, and the JIT
  partition a hot leaf instantiates in.
- `ARCHITECTURE_TTS.md` - the TTS block home's two layouts, tap stacking, the padded concat
  width, the served carrier on the image rail, the sine source's operation order, the heteronym
  context, the rig's scoring, and the two-tier phoneme pack.
- `ARCHITECTURE_TTS_MEMORY.md` - the per-synthesis carrier, the generator's six buffers, the
  chunk-sized peak with its cap and the idle release, and the harmonic source streaming in frame
  windows.
- `ARCHITECTURE_POCKET.md` - the Pocket codec's windowed stream, a voice as the backbone's
  key-value memory, the one-step head's baked timestep constants, the released chunker, and the
  published file's served quants.

## Inherited invariants {#inherited-invariants}

`ARCHITECTURE_INVARIANTS.md` carries this section.
