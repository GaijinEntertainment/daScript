# dasLLAMA Architecture - the row kernels the homes share

Companion to `ARCHITECTURE_GPU.md`; a section is cited by its anchor. This document carries the row kernels of the token
step and of the routed block that more than one home stamps (`dasllama_gpu_kernels_discrete.das`,
`dasllama_gpu_kernels_common.das`): the Q8 requant byte store, the residual step's two forms, how the row kernels address a
batched step's rows, the token-table gather, the routed combine and its gemma-4 form, the select's depth, the routed
weights' scales and the f16 activation's row base. The Vulkan commands that dispatch them are `ARCHITECTURE_GPU_VULKAN_NROW.md#nrow-token-command` and
`ARCHITECTURE_GPU_VULKAN_MOE.md#resident-moe-token`.

### The Q8 requant writers store one quant per byte {#q8-requant-byte-store}

Every requant writer on the class rail - the prefill and decode-tail kernels that write Q8_0 or
Q8_K quants - declares its output plane `array<int8>` and stores one quant per element, over
SPIR-V's 8-bit storage path; the fused decode step `DnStepFused` keeps its packed-word head
requant, the one writer that packs instead. Packing four quants into a `uint`
instead costs a shift-and-or chain per word, and in a Q8_K writer - where four co-active lanes
each hold one byte of the word - two subgroup shuffles per element on top. The stored bytes are
the same under either form: the amax fold, the scale and the rounding decide them, and all
three sit above the store. The path needs the device's 8/16-bit storage feature set, which the
family's device creator enables.

### The residual step's two forms spell the sandwich add as one fma {#residual-step-fma}

The residual step has two forms on the decode rail: the row kernel (`ArBase`, a stamp of
`GkAddRmsRows`, a row a workgroup, the N-row command's every site) and the q8 GEMV's epilogue
(`Q8GemvAr.epilogue`, the one-row command's post-attention and post-FFN sites), and the
regions cells hold the two commands bit for bit. The forms are ONE text - `GkResidualRow.accum_row_into`
and `GkResidualRow.quant32`, the shell both classes stamp (`dasllama_gpu_kernels_common.das`, the
Metal residual stamps' shell too), its reads and writes behind accessor overrides (the row kernel's
stash slab against the GEMV's re-read of its row; that re-read crosses the workgroup's waves
through buffer memory, so the GEMV's residual binding is `@coherent` like its y - a wave's
clamped four-column loads otherwise hold the last block's line from before another wave wrote
it, and the barrier pair does not refresh a non-coherent line on RADV) - so the
reduce, the four-column round and the requant cannot drift apart; a driver still decides per kernel whether a multiply
feeding an add contracts into one fma: contracting one kernel's sandwich column (`x + wn * (a *
ainv)`, a gemma's post norm over the add partner) and not the other's rounds the two one ulp
apart on a few percent of the row, and the batched rows drift from the session alone. Both forms
spell that add as `mad` - the GLSL `Fma` instruction, fused by definition - so the driver has no
contraction to choose; the plain column carries no multiply before its add. The row kernel walks
its row four columns a round - `accum_row_into` issues a round's four loads before its adds,
`GkQ8BlockStore.store_x4` its four loads before its stores - because a one-workgroup row kernel's time
is its load rounds.
`test_vkd_q8_gemv_ar_row_twin` holds the epilogue to the row kernel bit for bit on both columns,
fed the GEMV's own y row.

### The row kernels address a batched step's rows through the token block {#nrow-kernel-rows}

The row-parallel kernels take the rows' planes whole; the rope, the mirror store and the attention run at each row's own
position, cached count and mirror region, which ride the shared `TokMeta` block a row (`mirbase` an offset in mirror units;
`DaAttnArgs.rowwg` and `qrow` carry the row stride into the attention, `rowwg` 0 naming a one-row dispatch).

### A token table's gather is its format's {#token-table-gather}

Which gather a table takes is its disk type's:

| Table on disk | Gather | Its plane |
|---|---|---|
| Q8_0 | `EmbGather` (four elements a thread off the per-32 plane) | the tied classifier's at its block, or an untied table's linear q8 copy |
| a superblock format (the 14 `kq_sb` tags) | its `KqEmb<Fmt>` leaf | the tied classifier's K-quant plane, or an untied table's own (`resident_emb_plane`) |
| F32, F16, BF16 | `EmbGatherF32` | the table's exact f32 rows (F16/BF16 widen without loss, `Model.emb_f32_exact`) uploaded whole under `RDEC_EMB_F32_CAP` |

A `KqEmb<Fmt>` leaf is the format's own decode GEMV leaf stamped under `KqGemvLeafT`'s `EMB`: a
thread a 32-block, element k the int8 lane `blk_decode` leaves and the term pair `blk_fold` reads (the
per-format arithmetic is the GEMV's own, never re-spelled), the grid formats staging their codebook as
the GEMV does. A per-32 table (q51, iq4nl32, q4032, q5032) is the `Blk32GemvRunT` family's `EMB` leaf
(`Q51Emb`, `Iq4nl32Emb`, `Q4032Emb`, `Q5032Emb`), its block decode the GEMV's own; mx4 never tags a table.

### The routed combine rides the residual step {#moe-combine-row}

**The combine rides the residual step.** The add+rms that follows the routed block (`ClsArComb`,
its f16 twin `ClsArCombF16B` where the next layer's head takes the f16 feed) adds the shared
expert's down rows at the sigmoid of the gate logit - the dense tail ran the shared triple
first, as the layer's dense triple at the shared width - and the k weighted expert rows
through the slot map, straight into the residual, then norms the row for the next layer. A
layer without a shared expert takes the same step with the add partner off. The step sums
the FFN row first - the gated shared row, then the slots in order - and adds it to the
residual, the order the separate combine dispatch and the plain add it replaced took, so the
resident-vs-CPU bars of the MoE files keep their calibration: the fold's natural order - the
residual first - moves the rounding enough to flip a router near-tie downstream, and one step
of the 35B two-window cell reads 1.50 logits off the CPU chain against a 1.39 bar where the
chain's order reads 0.39. The row pass reads the first eight slots' metas - row base, routing
weight, bias row - once per thread into a per-thread array, then walks FOUR columns a round,
the four columns' loads written ahead of their adds; slots past eight walk the memory form (a one-workgroup row kernel
is its dependent-load rounds, and a column a round over 2880 columns is twelve of them - the
token command's one-row form, `ARCHITECTURE_GPU_VULKAN_MOE.md#resident-moe-token`); a slot-major pass through the row stash instead read 2.4 ms more on the 30B window, the
shared-memory read-modify-write per slot costing what the register sum does not. The token
command's tail folds the same way; the residual step read those rows anyway, and the fold took
one dispatch per layer out of both chains. There is no Q8 requant leaf: a third form would
bind a ninth buffer, past the eight the hazard rail carries, so where the next layer's head
takes the Q8 feed the f32 leaf writes the normed row and a separate requant follows it.

### The gemma-4 combine is one template's residual step {#moe-g4-combine}

The combine (`ClsArCombG4`, the f16 normed-row twin `ClsArCombG4F16B`) is the residual step of the CPU chain's
`gemma4_moe_ffn` on the template both homes stamp (`GkMoeCombG4`, args `GkMoeCombArgs`): the k weighted routed rows summed
in slot order (`GkMoeCombine.slot_sum_into`, the routed combine's own sum) and normed under the routed post norm
(`rpost_off`), the dense row in `ffnout` normed under the dense post norm (`dpost_off`), the two added and normed under the
layer's post-FFN norm (`spost_off`), the residual `(x + that) * out_scale`, then the next layer's norm (`nwoff`) - four
workgroup reductions over one row stash, the dense row re-read from `ffnout` rather than staged twice.

### The select reaches 512 experts two logits a thread {#vk-topk-depth}

**A select workgroup holds `DEPTH` logits a thread, so 256 threads reach 512 experts.**
`GkTopKRow.select_row` (at 256 lanes) reads expert `tid + i x 256` for `i < DEPTH`: the row max over every held
logit, the exp floored at -80 and the thread's sum before the workgroup sum, then each pick takes a
thread's largest held weight (the lower expert on a tie) into the same subgroup and workgroup
reduction the one-deep form runs, and the picked expert's holder knocks it out. At `DEPTH` 1 the
arithmetic is the one-deep form's, so a model at 256 experts or fewer reads the same picks.
`topk_depth` picks the stamp by the expert count (`TopKN2` / `TopKRows2` past 256, in their
one-deep classes' set families), `RDEC_EXPERTS_MAX` (512) is the reach the plan admits, and the
window's bucket schedule takes its 512-thread stamp at the same count. No 512-expert carrier fits
the 32 GB card whole; the kernels hold to the host select (`test_vkd_topk_n`, `test_vkd_moe_routing`).

### The routed weights' scales {#moe-wscale}

**A model's `expert_weights_scale` (0 and 1 mean none) and, on gemma-4, the layer's per-expert down scale multiply every
picked weight inside the select, in the CPU `moe_select_core`'s order: the renorm, the global scale, then the down scale.**
`GkTopKRow.scale_weight` carries both on both homes: the global scale off `GkTopKArgs.wscale`, the down scale under the
`DSCALE` template arm off the row `dsoff` names (`GK_NO_DSCALE` none). `enc_moe_select_rows` is the Metal decode's,
the batch and verify rows' and the prefill window's one select; a down-scale row selects the `DSCALE` stamps
(`MetalMoeSelectDs`, `MetalMoeSelect512Ds`), because the arm costs the plain stamps 0.27 us a dispatch on the M5 even
with no row named (2.88 against 3.16 us at 256 experts, 3.75 against 4.03 at 512), while the down-scale stamp replaces the
select and its separate scale pass (3.30 against 3.89 us at 128 experts). The Vulkan top-k (`TopKRecords`) is one stamp
with the arm on: its row is named or `GK_NO_DSCALE`.

### The f16 activation honors a row base {#act-row-base}

`ActF16T` reads its elements from `ActArgs.elem0` for `nelem` elements, so a driver can run it
over a slice of the window: the Vulkan home's last-layer slice passes the slice's first element
(`ARCHITECTURE_GPU_VULKAN.md#vk-prefill-window-chain`), a driver that serves the whole window
passes `0` (the CUDA home). The kernel reads nothing of that policy; its 16-byte fast path needs
`elem0 | ubase` on the four-element grid, and the scalar path serves the rest.
