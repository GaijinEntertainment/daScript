# dasLLAMA Architecture - the Vulkan tier's N-row token command

Companion to `ARCHITECTURE_GPU_VULKAN_RESIDENCY.md`; a section is cited by its anchor. This
document carries the N-row token command a batched step's rows go through, its same-slab form the
speculative verify steps one stream's rows through, and the residual step's two forms it holds bit
for bit. The residency plan, the marks swap and the logits landing
that the command runs under are in `ARCHITECTURE_GPU_VULKAN_RESIDENCY.md`; the decode
attention's forms are `ARCHITECTURE_GPU_VULKAN_ATTN.md#vk-decode-attn-split`; the per-op tier's
decode era, whose routed block the command's rows form mirrors, is `ARCHITECTURE_GPU_VULKAN_DECODE.md`.

### The N-row token command: a batched step's rows through one weight pass {#nrow-token-command}

**A batched decode step runs its rows through ONE recorded command, so a layer's weights stream
once for the step instead of once a row.** The driver sizes every per-token plane to `RDec.nb`
rows - one a region, or the speculative verify's rows where a NextN model asks more
(`ARCHITECTURE_GPU_VULKAN_NROW.md#nrow-verify-command`), `RD_NB_MAX` at most, eight, the N-column GEMV leaves' width - and
`vk_rdec_token_n_rows` answers how many rows the armed model steps at once: `nb` over dense,
MoE, per-layer-embedding, shared-KV and recurrent layers and a gated q, and none where a layer or
the tail has no N-row form - a dense plane in a per-32 expert format (q51, iq4nl32, mx4: the formats
with no N-column leaf, every kq leaf having one), a routed block beside a per-layer-embedding
branch, more routed slots than the block's slot planes hold, or a routed layer whose experts serve on
the host on a plain MoE (the one-row split command serves it, `ARCHITECTURE_GPU_VULKAN_HC.md#hc-token-command`) -
and logs the reason once per armed model. The classifier epilogue (the final softcap and the suppressed ids, `ClsEpilogue`)
runs once over the rows' logits planes, `ClsEpiArgs.rows` planes `vocab` apart, the id a row's
own; the one-row command and the prefill's tail pass one row. The pins matter on the one-row
path alone: the batch driver's host tail pins the suppressed ids again on every row after the
override (`dasllama_batch.das`), where the one-row decode returns before its host tail when the
device produced the logits. A q/k norm has one: the rows take whichever form the one-row
command takes - the fused norm + rope + store (`QknRopeKvT`, a head-row a workgroup with the row
in the workgroup id, each row at its own token meta) where the one-row command fuses, the split
pair (`qk_rms_cls`, the per-head rms over every row's projection row before the rope, the
projection row's width as both strides) where it does not - so the rows' q and k are the one-row
command's bit for bit. The fused kernel and the split pair round apart, because the compiler
contracts each kernel on its own, so one command runs one form for every row. Every set over a
per-row plane binds the plane's whole `nb` extent; the rule guards two shapes - an ungated q row
sitting inside the projection row, and a q binding sized to one row, which leaves every row but
the first reading past its binding. Every GEMV goes out as an N-column dispatch
(`GemvArgs.ncols` activation rows one weight pass apart by `ystride`); the q8 leaf takes its
two-output-rows-a-subgroup twin past `g_q8_n2_min_n` on an even row count, off by default
because the pod's down GEMV read 750 us a step under the pair against 587 a row a subgroup.
Where the one-row command fuses the dense FFN's gate and up GEMVs with the activation and its
requant (`RLayer.gu_on`), the rows' do too (`Q8GemvGuNT`, stamped at two, four and eight
columns like the plain q8 leaf: a workgroup owns 32 output rows for every column, each column's
half-block one 16-byte load beside the weight word, a column past the live ones re-dotting the
last live column's row with its block never stored, and a subgroup a column quantizes the
column's block, so the columns' rows quantize in parallel - the eight-column guarded unroll it
replaced read a quarter slower than the split gate and up GEMVs at four rows on the 12B,
`PERF_LEDGER.md`'s gemma section of 2026-09-20); the residual epilogues stay separate dispatches
over the rows, because an epilogue run by the last workgroup would serialize the rows' steps
where the separate dispatch runs them in parallel workgroups. The rows' fused add-rms-and-requant
stamp (`rq_b`) writes Q8_0 blocks alone, so the command's prologue fuses only where layer 0's head
takes a Q8_0 feed and reads no float row; every other head takes the split add-rms and requant
pair, as the site before a routed block does. The row-parallel kernels take the
rows' planes whole; the rope, the mirror store and the attention run at each row's own position,
cached count and mirror region, which ride the shared `TokMeta` block a row (`mirbase` an
offset in mirror units; `DaAttnArgs.rowwg` and `qrow` carry the row stride into the attention, `rowwg`
0 naming a one-row dispatch). The attention's key split is the span's, the ladder the one-row
command takes below its wide form (`rd_split_pieces` and the layer's window cap,
`ARCHITECTURE_GPU_VULKAN_ATTN.md#vk-decode-attn-split`), so the rows sum as each row does alone while every
row sits in one band - the batch's furthest row picks the form, the unsplit twin under
`RD_UNSPLIT_POS` and the split form at every span past it (the ruler reads eight pieces slower
than four at four rows, so the rows take no wide twin). A command is recorded once per row
count and form - the split form and the unsplit twin on first use of the row count, the twin's
availability decided at prepare with its buffers, never by a one-row record - and keeps its own
stamp names; the one-row command's list is borrowed for the record and put back. The command's
N-column leaves and its fused gate-up form are built on the first batched step; a stamp that
declines on the device logs once, and the command answers 0 rows from then on, so the
row-at-a-time loop serves.

**A recurrent layer's head steps the rows in their regions' state slots.** The qkv, z, beta/alpha
and out GEMVs go out in their N-column forms over the rows' planes - a projection row (qkv | z) a
row, the beta and alpha rows a row apart at the arm's stride (the q8 arm's GEMVs land them `nvh`
apart, the f32 arm's router form `2 x nvh`, `DnStepArgs.beta_row_stride`), an o row a row - and the fused
step (`dn_step_cls`) runs a workgroup per (row, head), the row's `TokMeta` naming the state and
ring slot (`dnslot`) and the ring parity it reads, so two rows in two regions advance two states
in one dispatch and the driver flips each row's region parity after the submit
(`ARCHITECTURE_GPU_VULKAN_DECODE.md#hybrid-token-command`); the batch driver owns each row's state in its
region before the command, as the one-row path does before a token. A gated q rides the same
kernels as one row: the q GEMV's y stride, the fused norm+rope's and the attention's `qrow` are the
q plane's `2 x qd` row where the gate is on, the projection row's width where it is not.

**A MoE layer's routed block steps the rows as regions of the one-row leaves.** The router GEMV
takes the rows as columns of one dispatch (`RouterArgs.ncols`: a workgroup an expert row, its
weights read once and dotted against every column, the logits a row at `obase + c * ne`); the per-row top-k
stamp (`TopKN`, a row a workgroup over the record base `TopKRecords`; the one-row command
dispatches the same stamp at one workgroup) writes row p's k slots at `p * k` - the gate and up region records reading row p's
feed blocks (`TopkArgs.xnb1`), the down records each slot's hidden row; and the gate, up and
down GEMVs are the one-row leaves over `nrows * k` regions (`GemvArgs.nreg`), the act over every
slot, the combine per row (`ClsArComb`'s position is its workgroup). The rows take the split
gate and up forms - each fused stamp's twin byte for byte, by the stamps' own cells - and the
unfolded down, whose fold under the routing weights is the combine's; the site before the block
takes the split add-rms and requant pair, since the router reads the normed rows as floats. The
routed planes hold `nb` rows (`moe_dlog_dev`, `moe_xq_dev` and `moe_xs_dev`, `egate_dev`,
`eup_dev`, `edown_dev`), every set binding them whole (the one-row command reads row 0), and the rail declines a
model whose `nb * k` slots pass `MAX_ROUTED_SLOTS`, the slot planes' extent. The rows share no
expert weights - every slot reads its expert whole, as the reference's decode does below its
grouped-GEMM threshold (`mul_mat_vec_max_cols` on its Vulkan backend, `MMVQ_MAX_BATCH_SIZE` on
CUDA) - so the batched step's gain on a MoE carrier is the attention, the router, the shared
expert and the submit, not the experts' bytes. The one-column leaves walk their regions
interleaved a row at a time (`r = rg % nreg`, the y row at `r * d + row`), so an expert two
slots share leaves DRAM once and the second slot reads it from cache - the reference's MoE
GEMV puts every token's dot of one row index in one block for the same reason - where a
region-major walk put the two reads a whole expert stack apart. The rows' combine quantizes the next head's feed
under a decision both commands share (`RLayer.comb_rq`, made once with the combine sets by
whichever command records first, `rd_ensure_comb_sets`): the two commands record in any order,
and a set either record binds always exists. One recorder encodes both commands
(`rd_encode_token` over `nrows`): the one-row arms at one row, the N-column leaves, the rows
stamp and the bisect knob past it, the sets one and the same. The router and the fused per-layer-embedding
kernels hold at most eight columns in their register and workgroup arrays (`RD_NB_MAX`), clamped
in the kernel; the router's partial plane holds sixteen subgroups' worth, twice the tier's floor.

**An E-series row carries its own per-layer-embedding side input, and a shared-KV layer projects
q alone.** The command takes the rows' side inputs position-major (`RdecTokenNFn`'s `ple` rows:
the table rows the batch driver gathers from each row's token where the device finishes the
pre-step, else the pre-step's finished rows as the workspace holds them), uploads them on the
cos plane's hazard bit, and where the device finishes the pre-step runs the projection as one
N-column dispatch (`RouterArgs.ncols`, the rows the columns, into `tok_plep`'s rows) and the per-slice
finish over every row's slices. A layer's branch takes the one-row branch's forms over the rows:
the gate as an N-column GEMV into `pleg_dev`'s rows, then where the one-row branch fuses
(`RLayer.ple_fused`) the FFN step's rows stamp quantizing the rows as they are and the fused act +
requant + proj over the columns (`Q8GemvPleAct` with `PleActArgs.ncols`: every column's x built
once in workgroup memory, one proj row a subgroup dotted against all of them from one pass over
its weights), else the rows' own Q8_0 quants of the residual, the act over every row's slice of
this layer (`ActArgs.ustride` the side row's width, `ulen` the slice) and the proj as an N-column
GEMV into `ffnout`'s rows; the two residual steps are the one-row command's over `nrows`
workgroups. The side planes (`ple_host`,
`ple_dev`, `pleg_dev`, `tok_plep`) hold `nb` rows, the one-row sets binding the first. A
shared-KV layer's head projects q as the N-column GEMV alone: the rope and store pass no k
pairs and no k-head groups, and the attention reads the donor layer's rows from the mirror.

**The rows' logits come home a row a job-queue lane, straight off the cached mapping.**
`rdec_nrow_steps` counts the steps the command served, so a cell holds that its batched steps
went through the command and not the row-at-a-time loop the two silent declines (no region, two
rows one region) fall to. `rd_land_logits_n` hands each row's copy to a lane where a queue serves (`maybe_parallel_for`,
the lanes idle while the device owns the step) and copies in order without one: a lane copies
about 14 GB/s, and the earlier form - the whole plane into a scratch row on one lane, then a row
a copy out of it - passed four rows of a 152k vocab twice over one lane (322 us a step on the
pod: the step's host stamps under `DASLLAMA_GPU_PROF=1`, `PERF_LEDGER.md`'s 2026-09-20 section).

**The engine reaches the command through the driver seam, and falls back a row at a time.**
`install_moe_gpu_resident_batch` installs the pair (`rdec_token_n`, `rdec_token_n_rows`) beside
the resident driver, so a tier with no batch arm answers 0 rows. `rdec_batch_rows_at_once` gathers
the step's residuals, rope rows, positions, counts and regions, submits once, reads each
host-cached row's K/V back and lands its logits; it returns false when a row finds no region or
two rows share one, and the caller's row-at-a-time loop serves that step. `DASLLAMA_VK_NROW_BISECT`
(`ENVIRONMENT.md`) drops a class of dispatch from the recorded command so a profile prices it; the
logits are garbage under any bit.

### The same-slab verify: one stream's rows in one region {#nrow-verify-command}

**A speculative round's verify steps one stream's k+1 rows through the N-row command, every row in
that stream's region.** Row i is the round's token or draft i at position `pos + i`: its `TokMeta`
names the region's mirror base and state slot, the position and a cached count of `pos + i + 1`.
The command stores every row's K/V before any row attends, and the count masks each row to the keys
at or below its own position, so row i reads the rows the same command stored below it and never
one above. The per-row planes hold the rows: where the driver homes a NextN head it sizes them at
the larger of the region count and the verify's rows - the round's depth plus one, read at load
(`get_mtp_depth`) - `RD_NB_MAX` capping both, so a verify of more rows passes to the CPU (`verify_rows`). The
command records once per row count and attention form (`RDec.v_cmd`, `v_cmd_unsplit`) - and, on
a hybrid, per region, since the rollback copies it carries bake the region's slot
(`ARCHITECTURE_GPU_VULKAN_MTP.md#resident-verify-rollback`) - the rows' form the furthest row's,
over the N-row command's sets; `vk_rdec_verify_rows` answers the depth plus one the driver was
prepared for (`RDec.verify_rows`), 0 where the head, the N-row command or an N-column leaf of the
head's planes is off the device, each reason logged once.

**The recurrent heads step the rows one at a time.** The N-row command's fused step runs every row
at once, each against its own slot; the rows of one slot would all read the pre-step state. So the
verify's GEMVs still take the rows as the columns of one dispatch, and the fused step (`dn_step_cls`)
runs a dispatch a row with the row in the push (`DnStepArgs.row0`): the hazard rail orders each
dispatch after the one before through the state's write, and row i's `TokMeta` carries the ring
parity the row before it left (the region's word xor i), so it reads the image row i - 1 wrote. The
driver flips the region's word once a row after the submit. A row's step is the one-row dispatch's
arithmetic on the state the row before it left (`test_vkd_dn_step_rows_sameslot`), so a verify row
reads the one-row command's logits bit for bit wherever the N-row command does.

**The draft head re-warms in the same command, in Metal's shape**
(`ARCHITECTURE_GPU_MTP.md#mtp-verify-draft-warm`). After the picks, head row i takes
`[enorm(embed tok_i) ; hnorm(h_i)]` - `h_0` the pre-draft carry the host uploads, `h_i` the trunk's
post-norm row `i - 1`, copied out of `xb` into the cat plane on the device - through eh_proj with the
rows as columns, the head's attention norm and requant, and its k and v projections, norm, rope and
store: its K/V rows `pos - 1` .. `pos + n - 2` and nothing past them, since no verify reads the
head's attention output. The store-only pass (`rd_encode_attn_head`'s `kv_only`) runs no q GEMV
where q has its own plane (a merged q/k/v plane keeps its one dispatch) and dispatches the fused
norm + rope + store, or the split norm and rope pair, over the k-head groups alone (`nh` 0, the
k pairs from pair 0), so the rows it stores are the draft command's k-head workgroups' bit for bit
on the same input - `test_gpu_resident_hybrid_mtp_verify` holds the re-warm's row `pos - 1` to the
draft's. The head's rows sit a position below the trunk's, so the head reads a `TokMeta` block and
rope rows of its own (`RDec.head_tok`, `head_cos_dev`), which its draft command reads too; its
attention norm lands in the cat plane, so `xb` keeps the trunk rows the landing copies. The
head's K/V rows stay on the device after the command, as a draft's row does, and come down on a
pass (`ARCHITECTURE_GPU_VULKAN_MTP.md#resident-draft-head`).

**The seat leaves the session as the CPU verify does.** Every row's pick and post-norm hidden leave
on the transfer queue (`ARCHITECTURE_GPU_VULKAN_RESIDENCY.md#logits-transfer-queue`), and the rows'
logits with them unless the round asked for the picks alone - the caller's pick ask
(`Session.pick_asked`, `RdecVerifyFn`'s `pick_only`): the picks-only twin then carries the landing,
the logits plane never leaves the device, and a guard step (`rd_guard_rows`: any of the rows on the
one-row command's cadence) lands the logits anyway so the over-commit check reads row 0; the tier's
`landed_logits` witness (`install_rdec_verify`'s third seat, `rdec_verify_landed_logits`) answers whether the
last served verify landed them, so the finite check runs on every landed row and never on a plane
that stayed on the device. The seat
(`vulkan_resident_verify`, `register_mtp_verify_override`) lands the picks in `mtp_picks`, the hidden
rows in `mtp_hrows`, the logits - where they land - in `mtp_logits_b`, `mtp_logits` (row 0) and
`logits` (the last row), `mtp_h` the last row's at `pos + n`, the region's rows at `pos + n`, the
recurrent state n rows on, and reads the pre-draft carry the round saved in `mtp_xb_save` before
its draft. The round's greedy walk reads row 0's pick against the draft wherever the seat served,
and under the pick ask publishes the committed row's pick (`land_pick`: row 1's on an accept, row
0's on a reject) for the caller's `sample_`, as the plain step's landing does; a caller that never
asks reads `s.logits` as before, and the sampled walk keeps the logits (a pick ask beside a sampled
walk is an engine bug the round panics on by name). Every check that can decline runs before the
session's state comes up, so a decline leaves the session to the CPU verify untouched, the head's
device rows brought down first. A reject takes the rollback seat, not a snapshot: the command
copied each recurrent layer's slot after every row but its last, and the seat puts row `a`'s copy
back and cuts the region's rows to `pos + a + 1`, the rows past them dead
(`ARCHITECTURE_GPU_VULKAN_MTP.md#resident-verify-rollback`). The trunk's K/V rows stay on the
device, as a one-row step's do.

**The verify command stamps its dispatches under its own ledger.** Under `DASLLAMA_GPU_PROF=1`
the command records a timestamp a dispatch role as the token command does, into a stamp list of
its own per row count and attention form (`g_rdq_verify`, `g_rdq_verify_unsplit`; the split
form's list is borrowed for the record and put back): the trunk's roles under the one-row
command's names - a GEMV over the rows as columns bills to the role it bills at one row - with
the sequential recurrent rows a stamp a row's step (`dn_seq`) and a stamp a rollback copy
(`roll`, the two summing over the rows and layers), the classifier over n columns (`cls`, the
epilogue and the picks as the token command names them), and the head re-warm under `warm_*`:
the cat rows' copies, norms and requant (`warm_cat`), the eh_proj GEMV (`warm_eh`), the head's
attention norm and feed requant (`warm_attnorm`), and its k and v projections, norm and rope
store under the trunk's role names prefixed `warm_` (`warm_kv`, `warm_qknrope`, or `warm_qkn` and
`warm_rope` on the split pair; a merged q/k/v plane stamps `warm_qkv`). The samples go to the
verify's own ledger (`g_rdq_verify_ledger`), so the round's alternating draft and verify commands never
restart the token ledger's averages, and every 32 verifies the driver prints `vk_rdec gpu
avg/verify over N: ...` in the token line's shape (`vk_rdec dn avg/verify` beside it on a hybrid,
the idle since the command sampled before - the draft's - at its end) and `vk_rdec host
wall/verify over N of n rows: ...` - the memcpys and meta, the submit, the wait (the
transfer-queue landing of the picks and hidden rows, and of the n logits rows where they land, is
inside it) and the landing copy. The round's profiler sections (`mtp.verify`, `mtp.rollback`)
price the seat's whole wall from the engine's side, the draft's `mtp.draft`.

### The residual step's two forms spell the sandwich add as one fma {#residual-step-fma}

The residual step has two forms on the decode rail: the row kernel (`ArBase.accum_row`, a
row a workgroup, the N-row command's every site) and the q8 GEMV's epilogue
(`Q8GemvAr.epilogue`, the one-row command's post-attention and post-FFN sites), and the
regions cells hold the two commands bit for bit. The forms are ONE text - `ResidualT.accum_row`
and `ResidualT.quant32`, the template shell both classes stamp, its reads and writes behind
accessor overrides (the row kernel's stash slab against the GEMV's re-read of its row) - so the
reduce, the four-column round and the requant cannot drift apart; a driver still decides per kernel whether a multiply
feeding an add contracts into one fma: contracting one kernel's sandwich column (`x + wn * (a *
ainv)`, a gemma's post norm over the add partner) and not the other's rounds the two one ulp
apart on a few percent of the row, and the batched rows drift from the session alone. Both forms
spell that add as `mad` - the GLSL `Fma` instruction, fused by definition - so the driver has no
contraction to choose; the plain column carries no multiply before its add. The row kernel walks
its row four columns a round - `accum_row` issues a round's four loads before its adds,
`ArBase.store_out` its four loads before its stores - because a one-workgroup row kernel's time
is its load rounds.
`test_vkd_q8_gemv_ar_row_twin` holds the epilogue to the row kernel bit for bit on both columns,
fed the GEMV's own y row.
