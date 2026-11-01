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

- `ARCHITECTURE_ENGINE.md` - the engine core, arch-registration and support charters, and the gates that hold the
  test folder's documents to its files (`ARCHITECTURE_ENGINE.md#test-folder-gates`).
- `ARCHITECTURE_ENGINE_MECHANISMS.md` - the RoPE scaling the loader folds into the `Model` once
  (`ARCHITECTURE_ENGINE_MECHANISMS.md#rope-scaling-bake`), and the decode pick tape a parity cell
  pins one arm's routing on another with (`ARCHITECTURE_ENGINE_MECHANISMS.md#moe-pick-tape`).
- `ARCHITECTURE_ENGINE_SERVING.md` - the serving charter, the prefix cache's partial page
  (`ARCHITECTURE_ENGINE_SERVING.md#prefix-tail-page`), its checkpoints on a recurrent model, the
  reasoning budget that closes a thinking span for the model, and the host pool behind
  device-home streams (`ARCHITECTURE_ENGINE_SERVING.md#device-home-pool`).
- `ARCHITECTURE_ENGINE_FORMATS.md` - the format, load-rail and CPU-kernel-tier charters, plus
  the shapes they key off - the bigram merge heap both tokenizer backends run
  (`ARCHITECTURE_ENGINE_FORMATS.md#bpe-merge-heap`), the one RoPE fill over a position source
  (`ARCHITECTURE_ENGINE_FORMATS.md#rope-one-fill`), and the Model's per-format plane table, its
  only `dim` field (`ARCHITECTURE_ENGINE_FORMATS.md#model-plane-table-dim`).
- `ARCHITECTURE_ENGINE_FORMATS_NV4.md` - the nv4 member: its planes as NVFP4's disk bytes in the k4
  tiling (`#nv4-planes`) and the per-tensor scale beside them (`#nv4-site-scale`).
- `ARCHITECTURE_GPU.md` - the GPU backend role table (`ARCHITECTURE_GPU.md#gpu-backends`).
- `ARCHITECTURE_GPU_ASYMMETRIES.md` - the closed list of the allowed asymmetries between the
  GPU backends - Metal, Vulkan and CUDA (`ARCHITECTURE_GPU_ASYMMETRIES.md#gpu-asymmetries`).
- `ARCHITECTURE_GPU_ASYMMETRIES_CUDA.md` - the entries of that list that are the CUDA home's alone: the
  int8 MMQ prefill, the FP4 expert rung, the nv4 nibble decode, the MMQ pin
  (`ARCHITECTURE_GPU_ASYMMETRIES_CUDA.md#cuda-asymmetries`).
- `ARCHITECTURE_GPU_SHARED_KERNELS.md` - the kernels more than one GPU home stamps: the shared templates' mechanism
  (`ARCHITECTURE_GPU_SHARED_KERNELS.md#gpu-shared-kernels`), the kernel pairs, their stamps and the pairs that stay
  per home (`ARCHITECTURE_GPU_SHARED_KERNELS.md#gpu-kernel-pairs`), the routed block's routing kernels both discrete
  homes stamp (`ARCHITECTURE_GPU_SHARED_KERNELS.md#moe-routing-kernels`) and the deltanet kernels they stamp with
  their smalls layout and capacities (`ARCHITECTURE_GPU_SHARED_KERNELS.md#dn-discrete-kernels`).
- `ARCHITECTURE_GPU_SHARED_READINGS.md` - what the shared row templates' stamps read on each home against the per-home
  bodies before them, and the instruments the readings come from (`ARCHITECTURE_GPU_SHARED_READINGS.md#gpu-shared-readings`).
- `ARCHITECTURE_MEDIA.md` - the encoder-tower, audio, ASR and vision charters.
- `ARCHITECTURE_TTS.md` - the text-to-speech charters - the front end, the block home, the
  shared assembly, the families and the facade.
- `ARCHITECTURE_POCKET.md` - the Pocket TTS family, a continuous-audio language model with its
  own assembly.
- `ARCHITECTURE_TRANSLATE.md` - the Bergamot translation family (`ARCHITECTURE_TRANSLATE.md#nmt-charter`).

Every file under `dasllama/` appears in a companion. Two carry generated CONTENT rather
than being generated themselves: `dasllama_env.das` holds the `[EnvConfig]` knob declarations
(`ENVIRONMENT.md` is generated FROM it by `harness/gen_env_doc.das`), and `dasllama_unicode.das`
is hand-written around transcoded Unicode data tables (the RANGES/WS tables - regenerate by
re-transcoding `$LCPP/src/unicode-data.cpp`).

## Mechanisms

Each companion's sections are anchored by topic; a citation spells `<doc>.md#<anchor>`.

- `ARCHITECTURE_IMAGE_VULKAN.md` - the vulkan lane's mint: the GPU walk inside the streaming save, the job-span view, the
  trimmed lane, the walk's decline
- `ARCHITECTURE_IMAGE_LEDGER.md` - the image rail's ledgered cases: the assistant drafter's non-image lane and the suite
  cells that mint a model image by design
- `ARCHITECTURE_IMAGE_GPU_PLANES.md` - the planes a mint bakes for a GPU halfword route: the dev-W f16 panel plane and
  the tower twin-W plane
- `ARCHITECTURE_IMAGE.md` - the prepared-image rail, the lattice-only
  per-format plane table both halves of the rail skip around, the
  layout stamp `REVIEW.das` keeps over the byte-placing code, the meta-field tripwire every
  struct serializer opens with, the planar-vs-blob flavor rebake, and the per-format slots the
  interleave identity prints.
- `ARCHITECTURE_GPU_RACE_SHAPES.md` - the tensor-GEMM and fused-attention shapes that measured
  out, the retained reference arms beside them, and the sanctioned float-A stamps.
- `ARCHITECTURE_GPU_TOWER.md` - the Metal tower attention routes, the Metal tower driver's
  encode chains, and the StyleTTS2 synthesis chain.
- `ARCHITECTURE_GPU_TOWER_FC.md` - the Metal FastConformer chain's second form, the one GigaAM serves
  through: the rotary position form and the LayerNorm conv-module norm.
- `ARCHITECTURE_GPU_TOWER_POCKET.md` - the Metal tower driver's Pocket TTS seats: the codec, the
  frame loop and the text prompt.
- `ARCHITECTURE_GPU_TOWER_VULKAN.md` - the Vulkan tower's row classes and attention routes, the
  Vulkan tower driver's encode chains, and the Vulkan ASR-decoder driver.
- `ARCHITECTURE_GPU_TOWER_VULKAN_TTS.md` - the StyleTTS2 synthesis seats and the Pocket codec,
  frames and prompt seats on the Vulkan tower: the shared weight slab, the f32-exact front end, the source
  and the generator, the seats' scratch and declines.
- `ARCHITECTURE_GPU_TOWER_VULKAN_FC.md` - the Vulkan FastConformer chain's rotary form (GigaAM): the rope
  tables, the padded attention route, the LayerNorm conv stamp, the conv front.
- `ARCHITECTURE_GPU_TOWER_VULKAN_VISION.md` - the Vulkan tower's vision chains: the stems, blocks, taps
  and tails of gemma4uv, gemma4v, gemma3v, qwen3v and qwen25v on both weight lanes, and the lane policy's
  Vulkan default.
- `ARCHITECTURE_GPU_QUANT_PLANES.md` - the Metal kq split scale plane, the iquant GEMV grid read
  with its f4-slab twin, and the Metal GEMV site abstraction.
- `ARCHITECTURE_GPU_BATCH.md` - the per-layer FFN widths on the Metal drivers and the Vulkan
  whole-model driver, and the Metal batch driver's row shapes - the E-series rows, a MoE's shared
  expert, a deltanet hybrid's recurrent rows, and the split single-pass attention the single row
  and the verify rows take too (`ARCHITECTURE_GPU_BATCH.md#metal-batch-row-shapes`).
- `ARCHITECTURE_GPU_PREFILL.md` - the Metal prefill driver's attention slab, pad rows, chunked
  submission, the f16 twin dual-store, the last-layer FFN tail, the hyper-connection window, and an
  embedding's hidden-only window (`ARCHITECTURE_GPU_PREFILL.md#prefill-hidden-only`).
- `ARCHITECTURE_GPU_PREFILL_GEMM.md` - the Metal prefill driver's GEMM form ladder, dev-W knee map,
  GEMV tail peel, the short window's split-K (`ARCHITECTURE_GPU_PREFILL_GEMM.md#prefill-split-k`), and
  the dense-KQ tensor mul_mm scaffold.
- `ARCHITECTURE_GPU_PREFILL_MOE.md` - the Metal prefill driver's routed block - the MoE bucket
  rail, its tensor-twin scaffold and the split-format expert twins, the K-quant twins' crowns and
  the knee that picks them, and the short window's gathered expert route; the gemma-4 MoE tail as one step
  (`ARCHITECTURE_GPU_PREFILL_MOE.md#metal-g4-moe-step`).

- `ARCHITECTURE_GPU_PREFILL_WINDOW.md` - the Metal prefill driver's device attention pair (the
  K/V twins, the device-direct QK and AV with the tail stamp of a head off the 64 lattice, the row
  softmax written in place), the mirror-fed window and mirror adoption
  (`ARCHITECTURE_GPU_PREFILL_WINDOW.md#prefill-kv-mirror`), the segmented window over several
  sessions' chunks (`ARCHITECTURE_GPU_PREFILL_WINDOW.md#prefill-segments`), the recurrent scan over
  half rows, and how a kernel's hot loads are addressed.
- `ARCHITECTURE_GPU_SHARED_ATTN.md` - the attention bodies both discrete homes (Vulkan and CUDA) stamp: the token
  command's attention key split, the prefill's flash-attention tile, the K/V mirror's block codecs (q8_0, tq4) with
  tq4's rotated basis, and the batched rope store's strided k and v reads.
- `ARCHITECTURE_GPU_SHARED_GEMM.md` - the GEMM tiles and GEMVs both discrete homes stamp: the cm2 decode callbacks'
  16-bit lanes, the mx4 decode, the device scale row, the cm2 tile body's k step, fast path and loop bounds, the KHR
  arm's hand-staged kq tile, the decode GEMV family's lane split by row length and region walk, and the routed gate-up
  fusion.
- `ARCHITECTURE_GPU_SHARED_ROWS.md` - the row kernels of the token step and the routed block more than one home
  stamps: the Q8 requant byte store, the residual step's two forms, the batched step's rows through the token block,
  the token-table gather, the routed combine and its gemma-4 form, the select's depth, the routed weights' scales and
  the f16 activation's row base.
- `ARCHITECTURE_GPU_VULKAN.md` - the Vulkan resident driver's prefill chain - the
  prefill window chain, a per-32 plane's window GEMM, the decode GEMV family's grid codebook buffer,
  the tile probe's shared descriptor set layout, the
  device-init roster of the Vulkan capabilities the tier keys its routes on, and the
  `[vk_dispatch]` lens's readonly derivation.
- `ARCHITECTURE_GPU_VULKAN_ATTN.md` - the attention-side planes the resident driver uploads beside its norms - the
  q/k/v projection bias, gpt-oss's sink logits with the flash tiles' sink stamps, and its output bias.
- `ARCHITECTURE_GPU_VULKAN_WINDOW.md` - what the prefill window does a session at a time: the
  recurrent block over its region's state and ring, the segmented window holding several sessions'
  chunks, the short window's peel through the token commands, an embedding's hidden rows
  (`ARCHITECTURE_GPU_VULKAN_WINDOW.md#vk-prefill-hidden-rows`) and the one scratch region its texts share
  (`ARCHITECTURE_GPU_VULKAN_WINDOW.md#vk-prefill-scratch`).
- `ARCHITECTURE_GPU_VULKAN_GEMM.md` - how the Vulkan driver runs its GEMMs on the cooperative-matrix
  tiles - the tile pick and the coopmat mode ladder, the short window's split-K
  (`ARCHITECTURE_GPU_VULKAN_GEMM.md#vk-short-split`), the class-pipeline build seat, and the MoE expert
  chain on those tiles.
- `ARCHITECTURE_GPU_VULKAN_KHR.md` - the window GEMMs over a file's f32 rows that ride the KHR arm's
  staged tile on every coopmat card, and why a K-quant window plane stays on the cm2 tiles.
- `ARCHITECTURE_GPU_VULKAN_ROOM.md` - the room the residency plan may fill on a card shared with a
  desktop: the OS's answer and the headroom where none comes, the over-commit guards, and the two
  allocation shapes the Windows memory manager pages out (the priority tag, the mapped ReBAR type).
- `ARCHITECTURE_GPU_VULKAN_RESIDENCY.md` - what a model has to fit on the card before the driver
  runs - the residency plan, the checkpoints a device-home recurrent stream keeps on the device,
  the token rows' gather off the table's own plane, the GPU-slot marks swap that lets one slot
  serve many models, the hydrate a call passing to the CPU rails takes first, and the one Vulkan
  device every context of a process shares (`ARCHITECTURE_GPU_VULKAN_RESIDENCY.md#vk-shared-device`).
- `ARCHITECTURE_GPU_VULKAN_LANDING.md` - the token command's logits landing on the transfer queue, and
  the selection landing a sampled stream's step takes in place of its rows.
- `ARCHITECTURE_GPU_VULKAN_MTP.md` - the NextN draft head the resident driver homes beside the
  trunk: the draft command that steps it, the prompt warm the window chain gives its slab, the
  rollback a rejected device verify takes from the verify command's own copies, the verify
  window a declined verify seat runs the CPU verify in, and the speculative knob's per-step
  gate on the carry; the assistant drafter beside a head-less trunk
  (`ARCHITECTURE_GPU_VULKAN_MTP.md#resident-assistant-drafter`).

- `ARCHITECTURE_GPU_VULKAN_NROW.md` - the N-row token command a batched step's rows go through,
  the N-column leaf every format has, the column tiles a dispatch past eight columns runs in,
  its same-slab form the speculative verify steps one stream's rows through, and the residual
  step's two forms it holds bit for bit.
- `ARCHITECTURE_GPU_VULKAN_HC.md` - the whole-model driver's hyper-connection chain (qwen4exp):
  the wide residual's mixer seams on the device, the n-gram side input, the split token
  command whose routed experts run on the host between its segments, the streamed prompt window
  the host and the device meet in, and the NextN head and its verify rows on the chain.
- `ARCHITECTURE_GPU_VULKAN_POOL.md` - the whole-model driver's hot expert pool: the slots a routed
  layer's hottest experts hold beside the mirror, the decode step's DMA fill of its misses, a
  prompt window's pool prefix, and the decode meet's window over the pool.
- `ARCHITECTURE_GPU_VULKAN_DECODE.md` - the per-op tier's decode era - the decode attention block
  over per-layer K/V mirrors, the whole-token decode span, the deltanet decode step's per-session
  resident state, and the whole-model driver's hybrid token command and prefill (recurrent
  layers, gated attention, partial rotary, the prefill-to-decode state handoff).
- `ARCHITECTURE_GPU_VULKAN_STREAM.md` - the streamed expert layers: the prefill GPU/CPU split of
  a layer whose stacks live on the host, the per-expert chain its GPU head runs - an expert a
  transfer submit and a compute submit waiting its own copy - the pinned stacks in the device
  layout the host reads through row-major kernels, the streamed slot as a ring of experts, and
  the expert cache whose misses read off the lane on demand.
- `ARCHITECTURE_GPU_VULKAN_PEROP.md` - the per-op tier's dense FFN rail: a dense layer's FFN triple on the dense rail
  and the decode row it runs on the device; the attention-quad chain's gemma decline
  (`ARCHITECTURE_GPU_VULKAN_PEROP.md#perop-attn-gemma`).

- `ARCHITECTURE_GPU_VULKAN_MOE.md` - the resident driver's routed block in its two eras - the MoE
  block of the prefill window with its Q8-fed chain, and the whole-model driver's MoE token
  command - the select that reaches 512 experts two logits a thread, and the gemma-4 form both
  eras take.
- `ARCHITECTURE_GPU_MTP.md` - the Metal speculative round over the batch driver's same-slab
  verify, and the box knob that sets the depth a round drafts.
- `ARCHITECTURE_GPU_MTP_ACCEPT.md` - what a sampled stream's round does with its drafts on both homes:
  the three acceptances, the sampler's seams, the drafter distribution a rejection round records,
  and the chain cut.
- `ARCHITECTURE_GPU_KV_RING.md` - both homes' sliding-layer K/V ring: a sliding layer's mirror rows as a ring of its
  window and one command's rows (`ARCHITECTURE_GPU_KV_RING.md#kv-sliding-ring`).
- `ARCHITECTURE_GPU_MTP_ASSISTANT.md` - the gemma-4 assistant drafter on both GPU homes: one
  drafter step, the sidecar and the attach, and the rounds of a scheduler tick.
- `ARCHITECTURE_GPU_MTP_DECODE.md` - the kernel argument-alignment contract enforced at every
  dispatch, the K-quant small-batch crown and the row-buffer pad a multi-row verify dispatches
  under, and the decode layer encoder.
- `ARCHITECTURE_GPU_MTP_DECODE_PICK.md` - the single-row driver's greedy chain and the Metal
  selection landing.
- `ARCHITECTURE_RUNTIME.md` - kernel shape, caches, lint policy, knobs, coverage, the GPU ramp.
- `ARCHITECTURE_RUNTIME_JOBQUE.md` - the hybrid worker pool, the MoE region split, the job queue
  the engine dispatches on, and the mode that dispatches to none.
- `ARCHITECTURE_MEDIA.md` - the padded tower GEMM widths, the family GPU hooks, the tower weight
  lane, and the plain-Model ASR decoders.
- `ARCHITECTURE_MEDIA_ASR.md` - the ASR families' mechanisms - the facade's carrier sniff order, the
  oracle a family is specified by, the family's tokens, language and long-form policy, the GigaAM
  Conformer chain's log-mel (`ARCHITECTURE_MEDIA_ASR.md#gigaam-log-mel`), rotary input and planes, and
  the shared transducer's decode step (`ARCHITECTURE_MEDIA_ASR.md#rnnt-decode-step`) and its device loop
  (`ARCHITECTURE_MEDIA_ASR.md#rnnt-device-decode`).
- `ARCHITECTURE_MEASUREMENT.md` - the benchmark rig, the tune gate, the sanctioned
  instrumentation rails, the `[tuned]` perm precedence, the mint wall in the sidecar's provenance,
  the audio outlier rule, the parity fixture's prefix bar, the committed records gate, and the
  speculative round's ruler record.
- `ARCHITECTURE_MEASUREMENT_REFERENCE_ROWS.md` - the llama.cpp rows a GPU home's records read
  against: the Vulkan board's (`ARCHITECTURE_MEASUREMENT_REFERENCE_ROWS.md#vulkan-reference-rows`)
  and the CUDA build's with its kernel-by-kernel trace
  (`ARCHITECTURE_MEASUREMENT_REFERENCE_ROWS.md#cuda-reference-rows`).
- `ARCHITECTURE_MEASUREMENT_AUDIO.md` - the audio board's instruments: the ASR board's GPU row pairs
  (`ARCHITECTURE_MEASUREMENT_AUDIO.md#asr-gpu-pairs`), the MLX ASR reference, the TTS reference
  instrument (`ARCHITECTURE_MEASUREMENT_AUDIO.md#the-tts-reference-instrument`), and its CUDA
  provider and g2p-venv runs.
- `ARCHITECTURE_MEASUREMENT_RELEASE.md` - where a between-release kernel change's board rows land
  (`ARCHITECTURE_MEASUREMENT_RELEASE.md#board-rows-release-boundary`).
- `ARCHITECTURE_MEASUREMENT_FAT_START.md` - a fat exe's first-start race and its per-user
  sidecar (`ARCHITECTURE_MEASUREMENT_FAT_START.md#fat-first-start`), and the per-box runtime
  file a run with no sidecar of its own races into
  (`ARCHITECTURE_MEASUREMENT_FAT_START.md#box-runtime-file`).
- `ARCHITECTURE_MEASUREMENT_SERVED.md` - the client clock around a live chat server's turns and
  what its figures are (`ARCHITECTURE_MEASUREMENT_SERVED.md#served-turn-instrument`).
- `ARCHITECTURE_MEASUREMENT_FORCED_FEED.md` - the reference build's forced feed on the E2B Q4_K_M:
  its driver, build, command line and readings (`ARCHITECTURE_MEASUREMENT_FORCED_FEED.md#reference-forced-feed`).
- `ARCHITECTURE_MEASUREMENT_VK_GEMM_PROBE.md` - the Vulkan GEMM probe's arms, the shapes they run
  at, and the alternates they are read against.
- `ARCHITECTURE_MEASUREMENT_KERNEL_RACE.md` - the instruments that time a kernel away from the
  served graph - kernel-race fidelity, the gemv's own tune seat, the row-major families' race on
  the streamed shape alone, and the CPU kernel bench's fixture conditions - and the timing-race
  vocabulary the race checklist's terms come from
  (`ARCHITECTURE_MEASUREMENT_KERNEL_RACE.md#timing-race-vocabulary`).
- `ARCHITECTURE_CPU_KERNELS.md` - the sub-block-packed k3/k6 planes, the grid formats' panel and
  row-group decodes, the VBMI symbol lattice, the tier that selects on the target rather than the
  host, the score dot classic prefill shares with decode, the module-scope visibility a lifted
  worker lambda needs of its kernel, the two interleaves a grp<mr> repack is made of, the bf16
  tile that keeps its sum in the matrix unit, the JIT partition a hot leaf instantiates in, and the
  batch tile's lattice (`ARCHITECTURE_CPU_KERNELS.md#f32-batch-tile-lattice`) and the bf16 weight
  plane's matrix-unit family (`ARCHITECTURE_CPU_KERNELS.md#bf16-weight-tile-family`); the q8 chain's
  activation forms against the reference's
  (`ARCHITECTURE_CPU_KERNELS.md#q8-activation-forms`).

- `ARCHITECTURE_TTS.md` - the TTS block home's two layouts, tap stacking, the padded concat
  width, the served carrier on the image rail, the sine source's operation order, the heteronym
  context, the rig's scoring, and the two-tier phoneme pack.
- `ARCHITECTURE_TTS_MEMORY.md` - the per-synthesis carrier, the generator's six buffers, the
  chunk-sized peak with its cap and the idle release, and the harmonic source streaming in frame
  windows.
- `ARCHITECTURE_DOCS.md` - the HOW_TO document type: its steps, traps and tense, and the gate
  that holds them (`ARCHITECTURE_DOCS.md#howto-documents`).
- `ARCHITECTURE_POCKET.md` - the Pocket codec's windowed stream, a voice as the backbone's
  key-value memory, the one-step head's baked timestep constants, the released chunker, and the
  published file's served quants.
- `ARCHITECTURE_TRANSLATE.md` - the Marian source's two served lanes (`ARCHITECTURE_TRANSLATE.md#nmt-lanes`), the
  oracle the family is held to (`ARCHITECTURE_TRANSLATE.md#nmt-oracle`), the device homes' shared admission
  (`ARCHITECTURE_TRANSLATE.md#nmt-device-homes`), and the decode step's narrowed pool
  (`ARCHITECTURE_TRANSLATE.md#nmt-step-lanes`).

## Inherited invariants {#inherited-invariants}

`ARCHITECTURE_INVARIANTS.md` carries this section.
