# dasLLAMA Architecture - the Vulkan tier's N-row token command

Companion to `ARCHITECTURE_GPU_VULKAN_RESIDENCY.md`; section numbers are `ARCHITECTURE.md`'s. This
document carries sections 2.2ao and 2.2ap: the N-row token command a batched step's rows go
through, and the residual step's two forms it holds bit for bit. The residency plan, the marks
swap and the logits landing that the command runs under are `ARCHITECTURE_GPU_VULKAN_RESIDENCY.md`
sections 2.2n-2.2o and 2.2an; the decode attention's forms are `ARCHITECTURE_GPU_VULKAN_ATTN.md`
sec.2.2al; the per-op tier's decode era, whose routed block the command's rows form mirrors, is
`ARCHITECTURE_GPU_VULKAN_DECODE.md` sections 2.2r-2.2v.

### 2.2ao The N-row token command: a batched step's rows through one weight pass {#nrow-token-command}

**A batched decode step runs its rows through ONE recorded command, so a layer's weights stream
once for the step instead of once a row.** The driver sizes every per-token plane to `RDec.nb`
rows - `min(regions, RD_NB_MAX)`, eight at most, the N-column GEMV leaves' width - and
`vk_rdec_token_n_rows` answers how many rows the armed model steps at once: `nb` over dense,
MoE, per-layer-embedding, shared-KV and recurrent layers and a gated q, and none where a layer or
the tail has no N-row form - a dense plane in a per-32 expert format (q51, mx4: the two formats
with no N-column leaf, every kq leaf having one), a routed block beside a per-layer-embedding
branch, or more routed slots than the block's slot planes hold - and logs the reason once per
armed model. The classifier epilogue (the final softcap and the suppressed ids, `ClsEpilogue`)
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
element offset; `DaAttnArgs.rowwg` and `qrow` carry the row stride into the attention, `rowwg`
0 naming a one-row dispatch). The attention's key split is the span's, the ladder the one-row
command takes below its wide form (`rd_split_pieces` and the layer's window cap,
`ARCHITECTURE_GPU_VULKAN_ATTN.md` sec.2.2al), so the rows sum as each row does alone while every
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
(`ARCHITECTURE_GPU_VULKAN_DECODE.md` sec.2.2v); the batch driver owns each row's state in its
region before the command, as the one-row path does before a token. A gated q rides the same
kernels as one row: the q GEMV's y stride, the fused norm+rope's and the attention's `qrow` are the
q plane's `2 x qd` row where the gate is on, the projection row's width where it is not.

**A MoE layer's routed block steps the rows as regions of the one-row leaves.** The router GEMV
takes the rows as columns of one dispatch (`RouterArgs.ncols`: a workgroup an expert row, its
weights read once and dotted against every column, the logits a row at `obase + c * ne`); the per-row top-k
stamp (`TopKN`, a row a workgroup over the record base `TopKRecords` the one-row stamp `TopK`
splices too) writes row p's k slots at `p * k` - the gate and up region records reading row p's
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

### 2.2ap The residual step's two forms spell the sandwich add as one fma {#residual-step-fma}

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
