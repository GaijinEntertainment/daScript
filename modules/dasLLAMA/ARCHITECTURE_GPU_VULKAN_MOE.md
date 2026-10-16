# dasLLAMA Architecture - the Vulkan resident driver's MoE block

Companion to `ARCHITECTURE_GPU_VULKAN.md`; a section is cited by its anchor. This
document carries the resident driver's routed block in its two eras:
the MoE block of the prefill window with its Q8-fed chain, the whole-model driver's MoE token
command, the select that reaches 512 experts two logits a thread, and the gemma-4 form both eras
take. The window
chain the prefill block runs inside (`ARCHITECTURE_GPU_VULKAN.md#vk-prefill-window-chain`), the recurrent block beside it (`ARCHITECTURE_GPU_VULKAN_WINDOW.md#vk-prefill-dn-block`), the Q8
requant byte store (`ARCHITECTURE_GPU_VULKAN.md#q8-requant-byte-store`), the decode GEMV family's grid codebook buffer (`ARCHITECTURE_GPU_VULKAN.md#kq-gemv-grid-buffer`) and the
tile probe's shared descriptor set layout (`ARCHITECTURE_GPU_VULKAN.md#khrx-shared-set-layout`) are `ARCHITECTURE_GPU_VULKAN.md`'s. The
token command's attention and recurrent heads this block's tail follows, and the per-op tier's
decode era - the decode span whose kernels the routed block runs - are in
`ARCHITECTURE_GPU_VULKAN_DECODE.md`. The cooperative-matrix tiles the expert GEMMs run on and
the per-op tier's MoE expert chain are in `ARCHITECTURE_GPU_VULKAN_GEMM.md`, and the lane split
of the decode GEMV family the token command's expert GEMVs take is
`ARCHITECTURE_GPU_VULKAN_GEMM.md#kq-gemv-lanes`. What a model has to fit on the card before any
of this runs is in `ARCHITECTURE_GPU_VULKAN_RESIDENCY.md`. The GPU backend
role table these sections build on stays in `ARCHITECTURE_GPU.md#gpu-backends`.

### The MoE block of the prefill window {#vk-prefill-moe-block}

**An MoE layer's window block replaces the dense FFN tail with a routed block on the device;
the attention head and the residual steps are shared.** The whole-model driver admits a MoE
whose expert stacks fit the arena beside its attention quads (`ARCHITECTURE_GPU_VULKAN_RESIDENCY.md#resident-plan`), and the window then never leaves the device between layers: the CPU's routing,
bucketing and combine of the per-op tier (`ARCHITECTURE_GPU_VULKAN_GEMM.md#cm2-expert-chain`) become
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
  chunks are for. The same class computes the E-series' per-layer-embedding projection, which
  off cm2 takes `F16GemmKhr` instead - the whole-K twin on subgroup-scope 16x16x16 fragments, a
  64 x 64 output tile a workgroup with the x and w^T fragments loaded straight from the planes
  (`ple_proj_shape_ok`: a KHR device at subgroup 32, `dim` and the projected width 16-multiples); its
  store stages each fragment in workgroup memory and lands only the elements inside `rows` and `d`,
  so a width off the 16-lattice (the tower's rel-plane GEMM at 2 npos - 1 columns) never spills a
  fragment's tail into the next row. Off that route `RouterGemm` serves, the span's router
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
- **The per-row select** (`TopKRows`, one workgroup per position) is the decode top-k's records
  without the GEMV meta (`TopKRecords` with `META` off) over each row: the softmax, k picks largest-first with ties to the lower index, the
  renormalized or scaled weights - the host `moe_select_core`'s arithmetic - written
  position-major as the picked expert and its weight per slot.
- **The bucket schedule** (`MoeSched`, one workgroup) writes what the host fill writes for the
  per-op chain: the experts' slot counts are an atomic tally over the window (a thread an expert,
  256 of them or the 512-thread stamp `MoeSched2` past 256 experts, `moe_sched_enc`, stride the
  picks, one workgroup atomic per slot), exclusive scans place each bucket's rows,
  its region indices (experts with rows, in expert order) and its tile workgroups, and the
  three expert planes' schedules land as 4-word records plus per-workgroup maps. A
  bucket is cut into at most two pieces by the tile ladder: a bucket within the s column (32
  rows) is one s piece; a bigger bucket takes whole m columns (128 rows) with the last one
  partial, unless the remainder past the whole columns fits the s column, which then takes it
  (`sched_ladder_m_rows`). The s pieces run the s stamp and the m pieces the e stamp - the m
  tile's column at the format's own k step, keyed `CM2_TC_E` in the class ladders
  (`ARCHITECTURE_GPU_VULKAN_GEMM.md#cm2-tile-pick-and-default`) - and every piece dispatches one workgroup per
  column tile per 128-row weight tile of the plane. In mm mode both piece kinds run the KHR
  128 x 128 tile (`ARCHITECTURE_GPU_VULKAN_GEMM.md#khr-mm-kq-tile`) over the same records and maps -
  a 32-row s piece is an edge tile there, and the workgroup counts agree because a column is one
  tile either way - so a KHR-mode card serves the resident MoE block (`vk_rdec_moe_ok`). The
  per-32 expert rails ride the same pieces through their own `<Fmt>Cm2T` stamps; the mx4 one
  needs no codebook - the e2m1 nibble decodes as the DOUBLED magnitude {0, 1, 2, 3, 4, 6, 8, 12},
  built per byte by shift and mask, against the HALVED E8M0 scale (`e8m0_half`; the bit patterns
  below exponent 2 are the denormals), the CPU `dot_mx4q8_scalar`'s own arithmetic, so the tile,
  the decode GEMV and the CPU dot agree by construction, and its scale plane is one byte a block,
  read four blocks a word. The s pieces' records sit at `[0, ne)`,
  the m pieces' at `[ne, 2 ne)`,
  and each dispatch's map at its own offset past the records (`PF_MOE_MAP_OFF`, 4096 words for up
  to 512 experts twice), the two tile counts scanned as one packed word (its halves stay under
  65536 for any admitted shape: 512 tokens by 64 slots over 512 experts). Every dispatch is sized
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
  tiles load a partial column unclamped, `ARCHITECTURE_GPU_VULKAN_GEMM.md#cm2-tile-pick-and-default`) and hold a
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

The router reads the f32 normed rows, so an MoE layer takes the split add+rms arm at the FFN
site (the fused twins never store `xb`), and the last-layer FFN slice of
`ARCHITECTURE_GPU_VULKAN.md#vk-prefill-window-chain` does not apply to the routed block. The arm's requant of
the normed rows feeds the dense triple alone - the gather takes the f32 rows - so a layer with
no shared expert skips it. On a coopmat2 device in cm2 mode, or in mm mode with the KHR tile, the
expert pieces run on the f16-fed tiles (`rdec_moe_ok` admits every expert format the f16 feed
admits); on any other device - an sdot4, f16 or int8 card, or `DASLLAMA_COOPMAT=sdot4` - they run on
the Q8-fed chain (`#vk-moe-q8-chain`). Q5_1 rides the schedule's s and e stamps (`Q51Cm2T`, the
per-32 plane verbatim, `BLKW` 32) and the token command's `Q51Gemv` over Q8_0 activations
(`vk_fmt_b32`, one lane a block), so a 704-wide down stack rides its own format where the K-quant
rails' 256-multiple rule would demote it to q8; a dense q51 plane takes the same GEMV and the s tile
(`ARCHITECTURE_GPU_VULKAN.md#vk-b32-window-gemm`). The per-32 IQ4_NL plane (`iq4nl32`) rides the
same admission on its own stamps (`Iq4nl32Cm2T`, the codebook staged as the kq iq4 tiles stage it;
`Iq4nl32Gemv` through `iq4_word`), the 640-wide down stacks of Qwen3.8-Flash-Next.

### The routed block's Q8-fed chain {#vk-moe-q8-chain}

Where no f16-fed tile serves the expert pieces (`pf_moe_tiles` false, `RDec.pf_moe_q8`), the window's
routed block keeps the router, the select, the bucket schedule and the combine, and runs the expert
GEMMs over Q8 rows: the gather copies each slot row's normed row into its bucket row as floats
(`MoeGatherF32`), one requant takes the gathered rows to the gate plane's feed (Q8_K for a superblock
format, Q8_0 otherwise), the gate and up GEMMs run the sdot4 batch tiles - `Q8Batch` on a q8 plane,
the kq batch leaves on a superblock plane, `Blk32BatchT` on a per-32 plane (`pf_q8_tile_ok`) - the
act + requant (`Q8ActRq` / `Q8KActRq`, the biased `Q8ActRqB` on gpt-oss's stacks) takes the hidden
rows to the down plane's feed, and the down tile writes the routed rows the combine reads. The
sdot4 tile is 32 rows by 32 columns, so the schedule runs s-only (`SchedArgs.s_only`): every bucket a
run of 32-row tiles on the s column, no m piece, the column tiles 32 wide (`pf_moe_bounds`).

### The whole-model driver's MoE token command {#resident-moe-token}

**An MoE layer rides the same recorded token command as a dense layer; its FFN tail is a routed
block over arena expert planes.** After the layer's attention head and the FFN norm, a layer
with a shared expert runs the dense tail over the shared triple (gate, up, the act, down into
`ffnout`), then the routed block: the router GEMV (`RouterGemv`) over the f32 normed row reads
the driver's router plane - every MoE layer's rows and, when the shared expert is gated, its gate
row last - into one logits row; the top-k (`TopKN` at one workgroup, the span's kernel over the decode's core)
writes the k routing weights and the three expert GEMVs' slot regions, each a `(block, feed
block)` pair whose block is the expert plane's slab-local base plus the pick's stride; gate and
up run the class GEMV over k regions, the act writes k hidden rows (Q4_K, IQ4_XS or iq3s4 gate and up stacks in one
slab feeding a per-32 down take the leaf template's fused arm for the three - `KqGemvLeafT.gu_shell`, stamped `KqGemvK4Gu`,
`KqGemvIq4xsGu`, `KqGemvIq3s4Gu`: both dots, the act and its requant a 32-row block a workgroup, one dispatch where three
ran; `RLayer.egu`), down runs k regions into the
routed rows; and the residual step that follows folds the combine in (`ClsArComb`, the prefill's
`ARCHITECTURE_GPU_VULKAN_MOE.md#vk-prefill-moe-block` kernel at one row): the shared expert's row in `ffnout` at the sigmoid of its gate
logit, the k weighted routed rows through the top-k's slot map, then the next layer's norm - a
layer without a shared expert takes the same step with the add partner off. A one-row dispatch is
latency, not bandwidth: the step's row pass is the `ARCHITECTURE_GPU_VULKAN_MOE.md#vk-prefill-moe-block` form - the first eight slots' metas
read once a thread, four columns a round with the loads written ahead of the adds, the sums still in slot
order - so an element waits on one load round per four columns rather than one per column; on
the `DASLLAMA_GPU_PROF=1` token profile (`vk_rdec moe avg/token`, `benchmarks/lcpp_bench.das`
under `-jit` with `DASLLAMA_ALLOW_UNTUNED=1`, cm2 mode) gpt-oss-20b's 24 layers at four slots
read 272 us a token in the step on the RTX PRO 4500 where the column-a-round form read 719, and
the fused norm+requant before the routed block 247 where it read 344. **A model without expert
biases still binds a down-bias plane: one ZERO ROW of the model dim.** The combine reads it at
row base 0 for every slot, so its column loop adds the bias unconditionally and carries no
branch a load waits behind; the prepare allocates and uploads that row whatever the model
carries. **gpt-oss's softmax-weight gate rides the renormalized-softmax arm.** The gate picks its
k on the raw logits (under the router bias) and softmaxes over the picks alone, which is the
softmax over every expert renormalized over the picks - the weights the `norm_topk` arm already
computes - so the host passes `norm_topk` for it and the top-k kernels carry no second gate.
Native-MXFP4 routed stacks take `KqGemvK4Gu`'s fusion (`Mx4GemvGu` on `Mx4Gemv`'s row dot;
`RLayer.egu_mx4`): a workgroup owns 32 hidden rows of one slot, a subgroup a row over both
stacks - the up stack's regions at their own binding, inside the gate stack's slab - and the
first subgroup adds the slot's expert bias rows, activates and quantizes the block with the
requant kernel's own amax and quant, so the bytes are the split path's. The slot regions are
device buffers the top-k fills each token; the dense triple's host-filled regions stay what they are.

**A recurrent MoE layer takes the routed block after its deltanet head** (the hybrid MoE,
`ARCHITECTURE_GPU_VULKAN_DECODE.md#hybrid-token-command`'s head with this section's tail): the deltanet
registration builds the layer, its shared expert rides as the dense triple, and the routed block
registers on the built layer through its own seat (`vk_rdec_set_moe_experts`, the seat the
attention form calls after its quad); the window chain's recurrent block precedes the same
routed block (`ARCHITECTURE_GPU_VULKAN_MOE.md#vk-prefill-moe-block`, the tail every head shares).

**The routed block is the decode span's FFN half transplanted, not a second copy of the span.**
The kernels are the span's (`ARCHITECTURE_GPU_VULKAN_DECODE.md#whole-token-decode-span`); what differs is the
home: the arena's expert planes and the resident driver's activation row instead of the per-op
tier's stacks and the span's own row. A model the plan admits therefore takes neither the span
nor the per-op rails - the resident prefill (`ARCHITECTURE_GPU_VULKAN_MOE.md#vk-prefill-moe-block`) fills the one mirror the token command
reads.

The experts' feed is the layer's quantized row when their form is the dense triple's, else a
second requant of the normed row in their own form (a K-quant expert stack beside a shared
expert the loader could only transcode to q8; a shared expert kept in the file's K-quant form
shares the experts' feed). The fused add+rms+requant twins are off on a MoE: the router reads the
normed row those twins never store. The MoE seats install separately
(`install_moe_gpu_resident_moe`), so a tier without them declines a MoE by name, and the plan
declines a router the top-k kernels do not serve - a gate off the two softmax forms, an expert
selection bias, more than 512 experts or more picks a row than the 128 routed slots - by name too; the router bias, the
biased stacks and the mx4 stacks are gpt-oss's served form (`ARCHITECTURE_GPU_VULKAN_MOE.md#resident-moe-token`).

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

### The gemma-4 form of the routed block {#vk-gemma4-moe-block}

**A gemma-4 MoE layer (`RdecLayerGeom.moe_g4`, the config's dense shared expert) is the routed
block of `ARCHITECTURE_GPU_VULKAN_MOE.md#vk-prefill-moe-block` and `ARCHITECTURE_GPU_VULKAN_MOE.md#resident-moe-token` with three of its rows re-sourced and its combine re-normed; no
kernel of the block changes but the combine.** The dense triple is the layer's own FFN planes
(`ffn_gate` / `ffn_up` / `ffn_down` at the FFN width, placed on the dense rail like a dense
layer's) and runs as the `ARCHITECTURE_GPU_VULKAN_MOE.md#resident-moe-token` dense tail: x under the FFN norm, requantized, gate, up, act,
down into `ffnout`. The routed branch does not read that normed row: its feed is x under the
layer's own routed pre norm, so the feed is always the experts' own image (`xe_own`) - the norm
step reuses the layer's add+rms set with the add off and the pre-norm row named in the push
(`RD_NORM_PRE_FFN2`), then the experts' requant (the token command) or the f16 gather (the window
chain) reads it - the token command takes the norm and the requant as one `ArRq` stamp with the add
off (`RLayer.pre2_fused`). The router reads a third row: a weightless rms of x under the router's
learned input scale over sqrt(dim), which the norms plane carries as one weight row
(`RD_NORM_ROUTER`: scale[i] / sqrt(dim)); the window chain produces it with the add+rms kernel, the
token command's router GEMV takes that rms itself, every expert workgroup over the same x
(`RouterArgs.norm_on`, `rtn_fused`), so no norm dispatch precedes it.
The top-k folds the checkpoint's per-expert down scale into each routing weight after the
renormalization (`GkTopKArgs.dsoff`, in the decode's and the window's top-k alike, names the layer's row in the
`[n_moe x ne]` scale plane `rdec_prepare_moe` uploads; `GK_NO_DSCALE` on every other model, the
binding filled and never read), where the CPU chain scales the picked weights before the expert
GEMVs - the select's `DSCALE` arm both homes stamp (`ARCHITECTURE_GPU_PREFILL_MOE.md#moe-wscale`). The combine
(`ClsArCombG4`, the f16 normed-row twin `ClsArCombG4F16B`) is the residual step of the CPU chain's `gemma4_moe_ffn` on
the template both homes stamp (`GkMoeCombG4`, args `GkMoeCombArgs`): the k weighted routed rows summed in slot order
(`GkMoeCombine.slot_sum_into`, the routed combine's own sum) and normed under the routed post norm (`rpost_off`), the
dense row in `ffnout` normed under the dense post norm (`dpost_off`), the two added and normed under the layer's post-FFN
norm (`spost_off`), the residual `(x + that) * out_scale`, then the next layer's norm (`nwoff`) - four workgroup
reductions over one row stash, the dense row re-read from `ffnout` rather than staged twice. Where the down experts are a
per-32 format (Q5_1, IQ4_NL off the lattice, Q4_0, Q5_0, MXFP4) the token command's down GEMV folds the k rows itself
(`Blk32GemvT.sum_shell`, stamped `Q51GemvSum` and its four twins: a subgroup a row of the model dim, the k regions' dots
summed under the routing weights), so the combine reads that row
(`GkMoeCombArgs.presummed`) where it read k through the slot map; and where the next head's feed is Q8_0
the token command's combine quantizes it itself (`ClsArCombG4Rq`, `RLayer.comb_rq`), so the requant
dispatch that opened the next layer (or the classifier) goes. The norms plane grows by the
four rows (`RDEC_NORM_ROWS` 10 with the output-bias row of `ARCHITECTURE_GPU_VULKAN_ATTN.md#vk-attn-planes`; zero on every other model). The plan's unserved list names the
dense shared expert only where no backend installed the MoE seats; the plan sizes an MoE layer
at its expert triple plus the FFN triple, and the layer decline reads the FFN triple's formats on
the dense rail.
