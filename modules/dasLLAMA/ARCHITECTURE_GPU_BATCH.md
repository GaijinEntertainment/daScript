# dasLLAMA Architecture - the Metal batch driver's row shapes

Companion to `ARCHITECTURE_GPU.md`; a section is cited by its anchor. This document carries the
per-layer FFN widths on the Metal drivers and the Vulkan whole-model driver, and the shapes the
Metal batch driver serves - the E-series rows, a MoE's shared expert, a deltanet hybrid's
recurrent rows, and the split single-pass attention the single row and the verify rows take too.
The GPU backend role table these sections build on stays in `ARCHITECTURE_GPU.md#gpu-backends`.

### Per-layer FFN widths and the batch's row shapes {#metal-batch-row-shapes}

**Per-layer FFN widths (MatFormer E-series, at most two - `ffn_second_hidden`) serve on Metal
and on the Vulkan whole-model driver**: the Metal decode, batch and prefill drivers bind the
width per layer (dense trunks, no MTP; the batch sizes its panels to the wider width and carries
the second width's row-total twin), and the Vulkan driver's per-layer geometry (`RLayer.hid`)
carries it beside the PLE branch. The Metal batch serves the rest of the E-series shape the same
way the single row does: the PLE branch as a rows form over a layer-major side plane (the CPU
pre-step's position-major rows transposed at the poke), and a shared-KV layer as Q-only rows -
the rope-store grid stops at the Q pairs, so nothing is written into the source slab the layer's
attention reads through the aliased row prefix. A MoE's shared expert rides the batch the same
way it rides the verify rows: the gate dot as a one-row router GEMV, the gate|up pair and the
down as rows forms by each plane's tag over the expert panel once the routed W2 consumed it. A
deltanet hybrid's recurrent layer runs its projections as rows GEMVs, each plane by its tag, then the conv, history,
norm, scan and gate once each over every row, a row a session against that session's own `DnMirror`
(`recurrent_batch`, `ARCHITECTURE_GPU_MTP.md#dn-group-stages`), and the mirrors advance when the step lands
(`BatchLanding.dn_uids`); a step whose rows have no resident, stashed or CPU-synced state declines `dn_state`. The
mirror cache (`g_dn_mirrors`, an LRU over the pool's slots, `ARCHITECTURE_GPU_MTP.md#dn-mirror-pool`) rests at four
sessions and grows to whatever a scheduler names through the tier's `gpu_dn_room_` seam at its creation
(`create_scheduler` -> the facade's `gpu_dn_room`), and the batch build names its own row count too: a cache narrower
than the batch would evict a row of the step it prepares. Every plane a rows GEMV lands in is sized to the rows form's four-row tile
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
