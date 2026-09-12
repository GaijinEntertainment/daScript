# dasLLAMA Architecture - the Vulkan resident driver's MoE block

Companion to `ARCHITECTURE_GPU_VULKAN.md`; section numbers are `ARCHITECTURE.md`'s. This
document carries sections 2.2af, 2.2ag and 2.2ak - the resident driver's routed block in its two eras:
the MoE block of the prefill window, and the whole-model driver's MoE token command, and the
gemma-4 form both eras take. The window
chain the prefill block runs inside (sec.2.2j), the recurrent block beside it (sec.2.2ad), the Q8
requant byte store (sec.2.2p), the decode GEMV family's grid codebook buffer (sec.2.2ab) and the
tile probe's shared descriptor set layout (sec.2.2ac) are `ARCHITECTURE_GPU_VULKAN.md`'s. The
token command's attention and recurrent heads this block's tail follows, and the per-op tier's
decode era - the decode span whose kernels the routed block runs - are
`ARCHITECTURE_GPU_VULKAN_DECODE.md`'s sections 2.2r-2.2v. The cooperative-matrix tiles the
expert GEMMs run on and the per-op tier's MoE expert chain are
`ARCHITECTURE_GPU_VULKAN_GEMM.md`'s sections 2.2k-2.2m, 2.2q and 2.2ae, and the lane split of
the decode GEMV family the token command's expert GEMVs take is its section 2.2ah. What a model has to fit
on the card before any of this runs - the residency plan that sizes the expert planes, and the
marks swap - is `ARCHITECTURE_GPU_VULKAN_RESIDENCY.md`'s sections 2.2n-2.2o. The GPU backend
role table these sections build on stays in `ARCHITECTURE_GPU.md` sec.1.5.

### 2.2af The MoE block of the prefill window {#vk-prefill-moe-block}

**An MoE layer's window block replaces the dense FFN tail with a routed block on the device;
the attention head and the residual steps are shared.** The whole-model driver admits a MoE
whose expert stacks fit the arena beside its attention quads (`ARCHITECTURE_GPU_VULKAN_RESIDENCY.md`
sec.2.2n), and the window then never leaves the device between layers: the CPU's routing,
bucketing and combine of the per-op tier (`ARCHITECTURE_GPU_VULKAN_GEMM.md` sec.2.2q) become
five device stages over the window's FFN-normed rows. Every window millisecond and per-layer
microsecond below is the `DASLLAMA_GPU_PROF=1` window profile (`vk_rdpf`) on the RTX 5060 Ti,
except where a probe arm is named.

- **The router GEMM** rides the cm2 tile where the split-k scratch exists (an f16 feed on a
  cm2 device) and the model width is a 512-multiple: the FFN-normed rows are converted to f16
  into the x feed plane, then `F16GemmCm2` - an f16 x f16 GEMM on the workgroup-scope 128 x 128
  tile with an f32 accumulator, its weights an f16 copy of the router rows minted at prepare
  beside the decode GEMV's f32 plane - runs eight k chunks into the split-k scratch planes and
  the reduce sums them into the logits (`pf_f16g_enc`). The grid of such a GEMM alone (four
  position tiles by two row tiles on the 35B) leaves the device under-filled, which is what the
  chunks are for. The same class computes the deltanet beta and alpha rows
  (`ARCHITECTURE_GPU_VULKAN.md` sec.2.2ad). Off that route `RouterGemm` serves, the span's router
  GEMV batched: a 64 x 32 tile of positions by router rows per workgroup, each invocation a 4 x 2
  block whose two rows sit 16 apart, K in 64-wide steps through a float4 stage in shared memory
  at a row stride of 17 float4, the next step's rows fetched into registers while the current
  step computes, and each output's four products per float4 added in k order (the scalar loop's
  sums to the bit). The stride and the row split are the bank rule: sixteen lanes reading
  sixteen rows land on all eight 16-byte bank groups, so a warp's two weight loads take two
  wavefronts each and its four activation loads (two distinct rows) one; the scalar tile is
  bound by that shared-memory traffic at about six FMAs per staged float4. The 64-wide K step is
  why the MoE seats ask for a 64-multiple row width. The router plane holds every MoE layer's
  f32 rows, and a gated shared expert's gate vector rides as one more row past the experts, so
  one dispatch writes the logits row `[ne | gate]` per position.
- **The per-row select** (`TopKRows`, one workgroup per position) is the decode top-k's core
  over each row: the softmax, k picks largest-first with ties to the lower index, the
  renormalized or scaled weights - the host `moe_select_core`'s arithmetic - written
  position-major as the picked expert and its weight per slot.
- **The bucket schedule** (`MoeSched`, one workgroup) writes what the host fill writes for the
  per-op chain: the experts' slot counts are an atomic tally over the window (the 256 threads
  stride the picks, one workgroup atomic per slot), exclusive scans place each bucket's rows,
  its region indices (experts with rows, in expert order) and its tile workgroups, and the
  three expert planes' schedules land as 4-word records plus per-workgroup maps. A
  bucket is cut into at most two pieces by the tile ladder: a bucket within the s column (32
  rows) is one s piece; a bigger bucket takes whole m columns (128 rows) with the last one
  partial, unless the remainder past the whole columns fits the s column, which then takes it
  (`sched_ladder_m_rows`). The s pieces run the s stamp and the m pieces the e stamp - the m
  tile's column at the format's own k step, keyed `CM2_TC_E` in the class ladders
  (`ARCHITECTURE_GPU_VULKAN_GEMM.md` sec.2.2l) - and every piece dispatches one workgroup per
  column tile per 128-row weight tile of the plane. The s pieces' records sit at `[0, ne)`,
  the m pieces' at `[ne, 2 ne)`,
  and each dispatch's map at its own offset past the records (`PF_MOE_MAP_OFF`, 2048 words for up
  to 256 experts twice), the two tile counts scanned as one packed word (its halves stay under
  65536 for any admitted shape: 512 tokens by 64 slots over 256 experts). Every dispatch is sized
  for its worst case - one s tile per expert; every expert's whole columns plus a partial one -
  and the map's tail past the real workgroup count carries the sentinel (`SCHED_NONE`): a tile
  workgroup that reads it sees a zero-row region and returns before its first barrier. A real
  window's router is skewed - on the Qwen3-30B-A3B at 512 tokens, 69 of 128 experts route, nine
  hold over 128 rows (the largest 467) - so the ladder runs about 90 column tiles where the s
  column alone ran 175 (`DASLLAMA_GPU_PROF=1` prints the last MoE layer's buckets and both
  counts): the expert planes at 563 / 652 us against 815 / 896 on the s column alone
  (`harness/vk_gemm_probe.das -- moesk:iq2xxs`, that window's profile). The slot-to-bucket-row
  map hands every slot the next row of its expert's bucket through an atomic cursor, so the
  rows within a bucket land in an order the schedule does not fix - which nothing downstream
  reads: the tiles compute rows apart and the combine reads each slot's row through the map, so
  the sums are the CPU walk's to the bit (the kernel cell checks the map as a permutation of
  each bucket's rows). The per-expert slot walks this replaced - thread e scanning every slot
  twice, staged through workgroup memory in 256-slot chunks - cost 163 us per layer on the 30B.
- **The gather, the expert tiles and the act** are the per-op chain's: the f16 gather scatters
  each position's row into its bucket rows, gate and up run the cm2 tiles over the gathered
  image - each plane's m dispatch then its s dispatch, the two writing disjoint rows of one
  plane under separate hazard bits (`VHZ_GATE_M`, `VHZ_UP_M`, `VHZ_MDN_M`) so they co-run and the
  reader's barrier covers both - the act writes the f16 hidden rows, and down runs the tiles
  again. The window planes carry 128 rows of read slack past the last bucket row (the s and m
  tiles load a partial column unclamped, `ARCHITECTURE_GPU_VULKAN_GEMM.md` sec.2.2l) and hold a
  whole window of `PF_WINDOW x k` bucket rows, so the block never chunks.
- **The combine rides the residual step.** The add+rms that follows the block (`ClsArComb`,
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
  chain's order reads 0.39. The slot loop loads eight rows together, then four, then one at a
  time - the token command's one-row form is latency, and the groups are its shape (sec.2.2ag);
  a slot-major pass through the row stash instead read 2.4 ms more on the 30B window, the
  shared-memory read-modify-write per slot costing what the register sum does not. The token
  command's tail folds the same way; the residual step read those rows anyway, and the fold took
  one dispatch per layer out of both chains. There is no Q8 requant leaf: a third form would
  bind a ninth buffer, past the eight the hazard rail carries, so where the next layer's head
  takes the Q8 feed the f32 leaf writes the normed row and a separate requant follows it.

The router reads the f32 normed rows, so an MoE layer takes the split add+rms arm at the FFN
site (the fused twins never store `xb`), and the last-layer FFN slice of
`ARCHITECTURE_GPU_VULKAN.md` sec.2.2j does not apply to the routed block. The arm's requant of
the normed rows feeds the dense triple alone - the gather takes the f32 rows - so a layer with
no shared expert skips it. The tile family is the f16-fed cm2 tiles, so the plan admits a MoE
only in cm2 mode on a coopmat2 device with every expert format the f16 feed admits
(`rdec_moe_ok`).

### 2.2ag The whole-model driver's MoE token command {#resident-moe-token}

**An MoE layer rides the same recorded token command as a dense layer; its FFN tail is a routed
block over arena expert planes.** After the layer's attention head and the FFN norm, a layer
with a shared expert runs the dense tail over the shared triple (gate, up, the act, down into
`ffnout`), then the routed block: the router GEMV (`RouterGemv`) over the f32 normed row reads
the driver's router plane - every MoE layer's rows and, when the shared expert is gated, its gate
row last - into one logits row; the top-k (`TopK`, the span's kernel over the decode's core)
writes the k routing weights and the three expert GEMVs' slot regions, each a `(block, feed
block)` pair whose block is the expert plane's slab-local base plus the pick's stride; gate and
up run the class GEMV over k regions, the act writes k hidden rows, down runs k regions into the
routed rows; and the residual step that follows folds the combine in (`ClsArComb`, the prefill's
sec.2.2af kernel at one row): the shared expert's row in `ffnout` at the sigmoid of its gate
logit, the k weighted routed rows through the top-k's slot map, then the next layer's norm - a
layer without a shared expert takes the same step with the add partner off. A one-row dispatch is
latency: the step loads eight slots' rows together, then four, then one at a time (the sums
still in slot order), so an element waits on one load round per group rather than per slot - on
the `DASLLAMA_GPU_PROF=1` token profile (`vk_rdec moe avg/token`, RTX 5060 Ti) the Qwen1.5-MoE
twin's 24 layers at four slots read about 290 us per token in the step where a
plain slot loop read 360 and the add plus the separate combine 199 and 233; on the 30B's 48
layers at eight slots 440 where a four-slot group alone read 490 (the compiler's own unroll of
the plain loop served eight slots but left four to a scalar tail). The slot regions are device
buffers the top-k fills each token; the dense triple's host-filled regions stay what they are.

**A recurrent MoE layer takes the routed block after its deltanet head** (the hybrid MoE,
`ARCHITECTURE_GPU_VULKAN_DECODE.md` sec.2.2v's head with this section's tail): the deltanet
registration builds the layer, its shared expert rides as the dense triple, and the routed block
registers on the built layer through its own seat (`vk_rdec_set_moe_experts`, the seat the
attention form calls after its quad); the window chain's recurrent block precedes the same
routed block (sec.2.2af, the tail every head shares).

**The routed block is the decode span's FFN half transplanted, not a second copy of the span.**
The kernels are the span's (`ARCHITECTURE_GPU_VULKAN_DECODE.md` sec.2.2t); what differs is the
home: the arena's expert planes and the resident driver's activation row instead of the per-op
tier's stacks and the span's own row. A model the plan admits therefore takes neither the span
nor the per-op rails - the resident prefill (sec.2.2af) fills the one mirror the token command
reads.

The experts' feed is the layer's quantized row when their form is the dense triple's, else a
second requant of the normed row in their own form (a K-quant expert stack beside a shared
expert the loader could only transcode to q8; a shared expert kept in the file's K-quant form
shares the experts' feed). The fused add+rms+requant twins are off on a MoE: the router reads the
normed row those twins never store. The MoE seats install separately
(`install_moe_gpu_resident_moe`), so a tier without them declines a MoE by name, and the plan
declines a router the top-k kernels do not serve - a non-softmax gate, a router or selection
bias, biased or mx4 expert stacks, more than 256 experts or 64 routed slots - by name too.

### 2.2ak The gemma-4 form of the routed block {#vk-gemma4-moe-block}

**A gemma-4 MoE layer (`RdecLayerGeom.moe_g4`, the config's dense shared expert) is the routed
block of sec.2.2af and sec.2.2ag with three of its rows re-sourced and its combine re-normed; no
kernel of the block changes but the combine.** The dense triple is the layer's own FFN planes
(`ffn_gate` / `ffn_up` / `ffn_down` at the FFN width, placed on the dense rail like a dense
layer's) and runs as the sec.2.2ag dense tail: x under the FFN norm, requantized, gate, up, act,
down into `ffnout`. The routed branch does not read that normed row: its feed is x under the
layer's own routed pre norm, so the feed is always the experts' own image (`xe_own`) - the norm
step reuses the layer's add+rms set with the add off and the pre-norm row named in the push
(`RD_NORM_PRE_FFN2`), then the experts' requant (the token command) or the f16 gather (the window
chain) reads it. The router reads a third row: a weightless rms of x under the router's learned
input scale over sqrt(dim), which the norms plane carries as one weight row
(`RD_NORM_ROUTER`: scale[i] / sqrt(dim)), so the same add+rms kernel with the add off produces it.
The top-k folds the checkpoint's per-expert down scale into each routing weight after the
renormalization (`TopkArgs.dsoff` / `TopkRowsArgs.dsoff` name the layer's row in the
`[n_moe x ne]` scale plane `rdec_prepare_moe` uploads; `NO_DSCALE` on every other model, the
binding filled and never read), where the CPU chain scales the picked weights before the expert
GEMVs. The combine (`ClsArCombG4`, the f16 normed-row twin `ClsArCombG4F16B`) is the residual step
of the CPU chain's `gemma4_moe_ffn`: the k weighted routed rows summed and normed under the
routed post norm (`ArArgs.p2off`), the dense row in `ffnout` normed under the dense post norm
(`pwoff`), the two added and normed under the layer's post-FFN norm (`p3off`), the residual
`(x + that) * out_scale`, then the next layer's norm - four workgroup reductions over one row
stash, the dense row re-read from `ffnout` rather than staged twice. The norms plane grows by the
four rows (`RDEC_NORM_ROWS` 9; zero on every other model). The plan's unserved list names the
dense shared expert only where no backend installed the MoE seats; the plan sizes an MoE layer
at its expert triple plus the FFN triple, and the layer decline reads the FFN triple's formats on
the dense rail.
