# dasLLAMA Architecture - the whole-model driver's hyper-connection chain

Companion to `ARCHITECTURE_GPU_VULKAN.md`; a section is cited by its anchor. This document carries the
whole-model driver's form for a hyper-connection model (qwen4exp, Qwen3.8-Flash-Next): the wide residual
and its mixer seams on the device, the n-gram side input, the split token command whose routed
experts run on the host, the hot expert pool beside it, and the NextN head riding the chain with its
verify's rows. The heads the chain runs between its seams are
`ARCHITECTURE_GPU_VULKAN_DECODE.md#hybrid-token-command`'s; the residency plan that admits the model is
`ARCHITECTURE_GPU_VULKAN_RESIDENCY.md#resident-plan`.

### The hyper-connection chain and the split token command {#hc-token-command}

**A hyper-connection model's residual is `hc` parallel streams, and every block reads one mixed row and
scatters one output row.** The driver holds the wide residual on the device (`RDec.hcres_dev`, [hc x dim],
started as hc copies of the embedded row) and runs each block seam as a mixer: the grouped rms of the
streams under the site's gamma rows (`HcNorm`, a workgroup a stream), the scatter logits as a router-form
GEMV over the site's f32 inject rows (`RouterGemv` with `ne = hc`, `dim = hc x dim`), the normed streams'
Q8_0 image, the q8 down GEMV into the low-rank row, `silu(lo / hc)` requantized (`HcLoRq`), the q8 up GEMV
back to the streams' width, and the gated stream mean into the block input row `xb` (`HcMix`). The block
output - the wo row after a head, the FFN's row after the FFN - scatters back into every stream under
`2 sigmoid(inject_c / hc)` (`HcCombine`); the head mixer is the same chain with no inject rows, into the
classifier's feed. The block norms a plain model reads off the norms plane do not exist on this
model - its mixers carry them - so those rows stay zero and no head reads them. The mixer planes are the
site's q8 arena planes (`RDec.hc_sites`, `2 x layers` sites and the head's) beside two f32 device
buffers, the norm rows and the inject rows.

**The n-gram side input lands before one layer's attention mixer.** The host gathers the token's hash
rows (`ngram_gather`, the arch's pre-step over the table in host memory) and hands the driver the row;
the command requantizes it, runs the key and value q8 GEMVs, gates each stream's value by the sigmoid of
the signed root of the normed key . normed stream dot (`PleGate`, a workgroup a stream, which also writes
the grouped-normed gated row into the conv ring's slot of this position), and lands the dilated depthwise
causal conv's silu plus the gated value in the streams (`PleConv`). The ring is a device plane a region
(`[regions x rows x hc x dim]`, zero at rest: a tap before the sequence reads zero), its slot
`pos % rows` off the row's `TokMeta`. The deltanet heads take the sigmoid out-gate through the step and
scan kernels' `ZSIG` stamps (`dn_step_sig_cls`, `dn_scan_p3_sig_cls`), picked by `RdecDnGeom.zsig`.

**The routed experts serve on the host, and the token command splits at every routed block.** The
model's expert stacks (55 GiB on the carrier) never fit the card beside its other planes, and their
formats sit outside the device tile family, so the plan leaves them out (`resident_host_experts`), the
layers register with no expert planes (`RLayer.experts_host`) and the routed block on the device is the
router GEMV alone. The command is recorded once per region as `nseg` = routed layers + 1 segments
(`RDec.cmd_seg`, `rd_record_segments`): a segment ends after a routed layer's router by landing the
FFN-mixed row and the router logits in host memory (`hx_host`, `hlog_host`); the host selects the top-k
and runs the plain q8 routed chain (`moe_routed_sum`, the CPU forward's own expert code over the
session's scratch) and writes the weighted sum into `hacc_host`; the next segment opens by copying it into
the routed rows' row 0 and scattering it with the shared expert's row at its gate (`HcCombine`'s
`moe_on`). The step (`vk_rdec_token_split`) submits and waits each segment in turn, the host's sum
between them, and the last segment lands the logits as the token command does. A segment boundary is a
full queue wait, so the segments share no hazard tracker; inside a segment the streams' scratch planes
ride other roles' hazard bits (the wide residual x's, the normed streams the wo row's, the low-rank row
the gate plane's, its quants the hidden's, the up projection the up plane's, the scatter logits the
routing smalls'), which only adds barriers. The host's select reaches any expert count, so the
256-expert reach of the device select kernels does not gate this form; one mirror region serves it, its
N-row command is not written, and its NextN verify is the split command's rows form (`#hc-verify-rows`).

**A plain MoE the card does not hold whole at a context asked whole takes the same split, layer by
layer.** Where the plan puts some routed layers' experts on the host (`ResidentPlan.host_layers`: the first
that many in layer order, `#hc-hot-pool`), those layers register with no expert blocks
and the one-row recorder (`rd_encode_token` with a segment base) cuts at each: the router alone, the
FFN-normed row and the logits landed, the command ended; the next segment opens on the host's sum
(`rd_encode_routed_sum`, the pool's hits added) and takes the plain residual step over it - the
combine-folded residual with one slot at the identity map under a weight of one (`RDec.ones_dev`), the
shared expert's row at its gate, the bias rows summed on the host. The layers whose experts the card
holds run the routed block as before, in the same command. The step is `vk_rdec_token_split` for every
model with host layers (`rd_record_segments` records the hyper-connection chain or the plain cut;
`vk_rdec_token` serves none), the host callback is the hyper-connection chain's (`rdec_host_experts_step`,
the hot pool's split inside it), the segments' commands are built at the first token
(`rd_host_segments_setup`). The split form of the attention is the one recorded (no unsplit or wide twin),
the N-row command and the rows verify decline a host layer (the one-row split command serves), and the
prefill window cuts the same way (`#hc-window-chain`). Regions are per segment, so a server's streams each
keep their mirror region through the split. The plain form takes a routed model that is not a
hyper-connection model (that model's own chain serves it) and has no gemma-4 parallel dense expert
(`moe_dense_shexp`, whose combine has no host form), on a tier with the split seats installed and at most
`RDEC_HOT_PICKS` picks a row (`resident_host_layers_ok`).

### The hot expert pool {#hc-hot-pool}

**A routed layer keeps its hot experts in device slots, and a step's picks split: the device runs the
hits while the host sums the misses.** The carrier has a working set - a layer's picks fall on a few
of its 512 experts far more often than on the rest - and the device decodes an expert's planes six to
ten times faster than the host does (`iq3s4` at the gate shape: 7 us a cold plane on the reference
card, the host's chain 61 us an expert), so the slots the card has room for pay. A pool is `slots`
slots a routed layer (`RdHotLayer`: a gate, an up and a down plane pair, each `slots` experts in the
plane's device layout, a slot's stride the expert's weight blocks), and the same count on every layer
with host experts - every routed layer of a hyper-connection model, the first `host_layers` of a plain
MoE the card does not hold whole at a context asked whole; a layer whose experts the card holds needs
none. The plan picks those layers (`resident_plan_host_layers`) only under a strict pin (`set_gpu_ctx_strict`,
the server's `--ctx`), where the mirror at the asked context leaves no room for every expert stack: the
fewest routed layers' expert stacks - the first in layer order - go to the host for the rest to fit
(`resident_host_layers_fit`), the pool over those layers takes what is left. Unasked, a plain MoE the card
does not hold whole stays with the per-op rails, which decode faster at every context (Qwen3-30B-A3B
Q4_K_M on the reference card, `debug-jit`: tg128 64.0 against this form's 41.8, tg64 at 8192 positions 64.5
against 47.8) and lose only the long prompt (pp8192 93.4 against this form's flat ~540; `PERF_LEDGER.md`'s
host-layers entry carries the rows).
The count is the resident plan's (`ResidentPlan.hot_slots`, `resident_hot_slots`), sized with the
mirror because the two share the card's room. Unasked, the pool takes 64 slots where they fit beside
the mirror at the context asked; where they do not, it takes up to 32 out of the mirror's context,
which keeps a floor of 32768 positions - a slot the device serves is worth more than context past the
floor, and a plan that fills the card to its cap beside a desktop pages (the carrier's decode reads
2.9 tok/s so, 30 with the pool and a mirror of 88 thousand positions). `DASLLAMA_GPU_HEAT` asks a
count, served from the same floor's room, and 0 asks none. A context asked whole (`set_gpu_ctx_strict`,
the server's `--ctx`) is the floor itself: the pool counts its slots out of what the asked mirror leaves
(`resident_hot_floor_ctx`, scaled by eight sevenths for the re-plan's seven eighths), down to none, and
the driver declines only where the dense planes and the asked mirror alone pass the card. The pick shortens the mirror's context to
what the pool leaves and holds the pool at the count the first plan settled on, so the slack of the
shortened context stays slack. Under four slots no pool arms. The chain a window's hits ride
(`RDEC_HOT_CHAIN_BYTES`, 320 MB of scratch) is taken off the room before the slots are counted and
charged to the plan only with a pool (`resident_hot_fit`): a plan with no pool - none asked, or none
that fits - keeps the mirror's context and the room a model needs to fit.

**The host owns the policy.** It holds, a layer, the slot of every expert and the expert of every slot
(`HotLayer`) and a pick count an expert that halves every 256 routed rows. After a window's host step
on a layer the rows' picks are counted and the free slots fill, hottest expert first
(`rdec_hot_window`); a full pool swaps at most eight experts a window and one a decode step, and only
an expert whose count passes twice the coldest slot's plus one - a swap costs a gather and an upload
of 2.6 MiB, more than the host's sum of the same expert. An upload is the loader's gather of the
expert's rows (`layout_gather_dispatch`, the bytes the arena would hold) written into the slot
(`vk_rdec_hot_upload`); it runs between a segment's wait and the layer's hit chain, so no command reads
a slot while it is written.

**A decode step's hits run in a command of their own.** After a segment lands the FFN-mixed row and
the router logits, the host selects, counts, admits, and splits the picks (`rdec_hot_step`): the hits'
slots and weights go to the device (`vk_rdec_hot_submit` writes the region records of the three GEMVs
and the weights into host-visible buffers the sets bind, zero weights past the hit count), the misses
pack to the front of the session's picks and the host sums them (`moe_routed_sum_k`). The hit chain is
the routed block's own - the row's feed requant in the gate plane's form, the gate and up GEMVs over
`nhit` regions, the act with the down feed's requant, the down GEMV into the routed rows - recorded
once a layer a hit count (`RdHotLayer.cmds`, k + 1 forms, on first use) and submitted with no fence, so
it runs while the host sums. The next segment opens by copying the host's sum into the routed sum's row
(`hacc_dev`) and adding the hit rows under their weights (`MoeCombine` with its slot window at 1 .. k
over slot rows 1 .. k, so the sum accumulates onto the row instead of starting it; a zero weight skips
its row, which is stale). The hit chain's rows are a compute write no fence covered, so that segment's
hazard tracker starts with them pending and its first access takes a barrier
(`rd_encode_routed_sum`). A step with no hit submits nothing and its zero weights leave the combine
adding nothing.

**A window's hits run through the tier's expert chain, beside the host.** The pool's planes are
stacks of the tier (`make_stack_shell` at `HOT_BASE` plus the layer's stride, a plane a stack, a slot
an expert's elements), so the batch arm's f16 chain (`vk_moe_ffn_batch_xf_begin`,
`ARCHITECTURE_GPU_VULKAN_GEMM.md#cm2-expert-chain`) finds them by offset like any resident stack. At
a routed layer's host step the host selects the window's rows, counts and fills or swaps the pool
first - so the window that warmed a slot reads it - and splits the picks (`rdec_hot_regions`): the
experts in the pool are the device's, their buckets its regions over the pool's planes (biggest
first), their slot rows' weights in the device's plane and zero in the host's; the chain takes the
window's rows, gathers the bucket rows as f16, runs the gate, up and down tiles over the regions,
combines under the device's weights and lands the rows on the host, all in one submit the host does
not wait for. The host sums the rest (`moe_routed_rows_sum` under `split`: the pool's experts take
no chain, a zero weight's parked row is unread), joins the chain and adds its rows
(`rdec_hot_rows_join`). A window under 32 rows, a window past the chain's one chunk, and a layer
whose formats the chain's f16 feed does not admit stay the host's whole. A per-32 expert plane
(q51, mx4, iq4nl32) takes the chain's s column alone (`batch_tile_edges`): its one cm2 stamp is the
32-row tile.

**The pool's policy is picked offline, on a trace of the routed picks.** Under
`DASLLAMA_MOE_TRACE=<file>` every host expert step records its picks (`rdec_trace_picks`): a record
is a layer, its row count, then k expert ids a row and k weights a row; a record of layer -1 with no
rows marks a prompt that starts at position 0. The file (`rdec_trace_write`) opens with the magic
`MOET` and three int32s - the routed layers, the expert count and k, set where the driver arms - and
holds the records to its end: the first flush writes the header and every later flush appends, the
last at the model drop. `harness/hot_pool_sim.das` replays a trace through a policy at a slot count;
the live run's per-layer line under `DASLLAMA_GPU_PROF` (`rdec_hot_say`: a layer's hit share and
swaps since the pool armed, where the misses sit) is its cross-check.

### The hyper-connection window chain {#hc-window-chain}

**A prompt runs through the window chain in the same seams, over the window's rows at once, with the
host's routed sums between the segments.** The mixer kernels take a `rows` count (the decode form is
rows 1), so one kernel serves both chains: `HcInit` opens the wide rows (`pf_hcres`, [np x hc x dim])
from the embedded rows; a site's mixer (`pf_hc_mix`) norms every stream row (`HcNorm`, `row0` picks the
window's last row alone for the head mixer), runs the inject logits as the router GEMM (`RouterGemm`, the
router GEMV's column reach is eight) into `pf_hcinj` [np x hc], requantizes the normed panel into its own
Q8_0 image (`pf_hcxq`, wider than the shared xq plane), runs the down and up q8 GEMMs through
`pf_gemm_enc` under the site's own meta slots (`pf_hc_meta0`, two a site, then the head's, then the side
input's key and value) and lands the block rows in `pf_xb`; the attention block (`pf_attn_block`) and the
deltanet block (`rd_pf_recurrent`, the `dn_scan_p3_sig_cls` stamp under `dn_zsig`) read them from there
as the plain chain does and land in `pf_xb2`; `HcCombine` over the rows scatters them, each row under its
own inject logits (`lstride`, `row0`: the last layer's slice scatters the rows the classifier reads). The
FFN site's mixer feeds the shared expert (`pf_ffn_block`) and the router (`RouterGemm` into `pf_mlog`);
the segment then ends by landing the FFN-mixed rows and the logits (`pf_hx_host`, `pf_hlog_host`), the
host runs `moe_routed_sum_rows` over every row (the grouped CPU expert chain, the same row set on both
arms of a parity tape - the last layer's slice too), and the next segment (`pf_seg`, routed layers plus
one a window) copies the sums into `pf_hacc_dev` and scatters them with the shared rows at their gates.
The head mixer's single row feeds the classifier through row 0 of `pf_xb`.

**A plain MoE with host layers cuts its window the same way, inside `pf_run`.** At a host layer the window
runs the router alone (`pf_moe_router`), lands the window's FFN-normed rows and logits (`pf_hx_host`,
`pf_hlog_host`), ends and waits the command, asks the host's rows through the tier's seat
(`rdec_host_rows`; the seat holds the chain's `rdec_host_experts_rows` from the arm, `set_rdec_host_rows`, and a
no-op from the model drop, `clear_rdec_host_rows` - the hyper-connection window chain takes its host callback
per call instead), then re-opens
the same command on a copy of the sums into `pf_hacc_dev` and the plain combine over them (one slot a row
at the identity map, `pf_ident_dev` / `pf_ones_dev`). The overlap ring is off for such a model - a cut is
a fence mid-window - and the device layers keep the MoE block.

**The n-gram side input takes the panel form.** The window's side rows (`pf_hcemb`, [np x ind], the
host's `ngram_gather` per position) requantize once, the key and value GEMMs run over the rows, `PleGate`
(a workgroup a stream row, `rows > 0`) gates every row and writes its normed gated row into the panel
(`pf_hpanel`) instead of the ring, `PleConv` reads a tap inside the window off the panel and a tap before
it off the ring (`pos0` names the window's first position), and the window's last `min(rows, nrows)` panel
rows copy into the ring's slots (`pos % nrows`, the region the tok meta's `dnslot` names) for the rows after
the window - the decode steps then advance the ring on the device (`Session.ple_ring_device`). A prompt
served this way passes nothing to the CPU (`RdecPass.hc_prefill` is a model the seats do not hold), and
the deltanet state and the ring stay on the device as any other hybrid's do.

### The NextN head on the chain {#hc-draft-head}

**The draft head rides the chain as one more routed layer: its block's two mixer seams sit past the
trunk's in the site list, its routed experts sum on the host like the trunk's, and it owns a slot set of
the hot expert pool.** The loader lays the head's seams at sites `2 x n_layers` and `2 x n_layers + 1`;
the trunk's head mixer follows them (`RDec.hc_final_site`), then the head's own head mixer
(`mtp_hc_head_*`, `hc_mtp_site`) and the carry's per-stream norm gammas (`mtp_hnorm`, `hc_hnorm_row`).
The resident side admits a routed head where the tier serves host experts (`resident_head_routed`), counts
it in every routed walk (`resident_routed_layers`: the plan, the router plane's last slot, the biases, the
pool's `HotLayer`s), and reads its dense triple off the shared expert's planes (`resident_head_fmts`,
`resident_head_ffn_plane`). The carry is the trunk's WIDE residual before the head mixer (`hc x dim`
floats a row, `Session.mtp_h` at `mtp_h_dim`): every hc command copies it aside (`hccarry_out_dev`)
before the head mixer runs, and the landing reads that plane (`rd_carry_src`, `rd_carry_width`) as the
plain driver reads the post-norm row. The draft (`rd_record_hc_draft`) is two segments: the first opens on
the embed row and the wide carry (`head_cat_host`), norms the embed row under `mtp_enorm`, grouped-norms
the carry's streams under the hnorm gammas (`s_hc_norm_carry`), lays the pairs `[enorm ; hnorm_c]` out as
hc cat rows, runs eh_proj with the rows as columns into the head's own wide residual (`head_s_eh_hc`; the
N-column leaf ensured for hc columns), then the head's block as a trunk layer's - mixer, attention, scatter,
mixer, the shared expert, the router - and lands the FFN-mixed row and the router logits; the host sums
the head's picks (`rdec_host_experts_step` at layer `n_layers`, the pool's slots where they hit); the
second segment takes the sum, scatters it, copies the head's wide residual aside as the carry, runs the
head's head mixer, the classifier and the pick. The seat passes the host step as `RdecDraftFn`'s
`experts`. The window chain warms the head's slab over the prompt in the hc form (`pf_hc_head_warm`
after the window's head mixer and tail): head row j pairs the window's embed row j + 1 - carry with the
trunk's wide residual at the row before it (the previous window's last row for row 0, kept in
`head_pf_hsrc_w`), norms the pair per stream, lays the hc pairs out stream-major as cat rows, runs eh_proj
with the rows as columns (`head_pf_wsm`), lays the head's wide rows row-major over the window's wide panel
(dead past the head mixer), runs the head's attention mixer over the head rows and its k and v projections,
q/k norm and rope store (`pf_head_kv_store`, the store the plain warm shares). The parity cell is
`tests/test_gpu_resident_hc.das`'s draft cell: the device draft against the CPU `forward_mtp` on the
same token, wide carry and row, the head's select pinned by the pick tape.

### The verify's rows on the chain {#hc-verify-rows}

**The speculative verify is the split command over `n` rows: the seams' rows forms, every row's routed
sum on the host between the segments, the deltanet steps a row at a time with the roll copies between,
the n-gram side input in its panel form committed to the ring after the rows, and the head warmed over the
rows in the last segment.** `rd_record_hc_segments_n` records the segments at the driver's verify row
count (`RDec.hc_v_seg`, a region's `nseg` command buffers): the wide rows open as hc copies of every
embedded row, the mixers take `nrows` (`HcNorm` over `hc x nrows` workgroups, the inject `RouterGemv` and
the q8 down and up GEMVs with the rows as columns, `HcMix` and `HcCombine` over the rows), the attention
and deltanet blocks take the N-row command's rows forms (`seq`: the fused step a row with
`rd_encode_roll_copy` between, so a reject rolls back on the device as the plain hybrid's does), the
router lands every row's logits, and the host's rows step (`rdec_host_experts_rows`) selects and sums
each row. The pool serves the rows through the decode form's hit chain (`rdec_hot_rows_step`,
`vk_rdec_hot_submit_rows`): one chain over `nrows x k` regions, each row's hits first and its misses
padded with slot 0 at weight 0 (the combine skips a zero weight), each region reading its row's
quantized feed (`RdHotLayer.xnb1`), and the next segment's combine runs a row a workgroup over row r's
window of k slot rows (`r0 = 1`, `r1 = nrows x k + 1`) onto its accumulator row; `s.moe_exp_gpu` marks the
pooled experts and `s.moe_w_cpu` zeroes their weights, so `moe_routed_rows_sum` sums the misses alone.
The side input's rows form takes the panel (`PleGate` and `PleConv` at `rows > 0` with `pos0 =
PLE_POS_TOK`, the window's first position read off the token record so the recorded command serves every
position) and `PleCommit` lands the rows' normed gated rows in the ring's slots after the conv read them;
at depth 1 a rejected row's slot is rewritten by the next token before any conv reads it. After the rows'
classifier the head warms over them (`rd_encode_hc_verify_warm`): row i pairs its embed row (`hemb_dev`)
with the carry of the row before it - the parked carry for row 0, the rows' wide residual for the rest
(`hcsrc_dev`) - normed per stream, the pairs as hc cat rows a head row, eh_proj with `hc x nrows` columns
into the head's wide residual over the rows' wide panel (`hcres_dev`, dead past the head mixer once the
rows' carry is copied aside), the head's attention mixer and its attention at the head's slot storing the
rows' K/V. The seat (`vulkan_resident_verify_go`)
widens its rows to the carry's width, gathers the rows' side rows through the arch's pre-step (which
advances the n-gram window; `mtp_ple_window_snapshot` - the CPU round's own - and the rollback's re-advance
over the accepted rows put it back on a reject) and hands the rows step as `RdecVerifyFn`'s `experts`. The
rail (`rd_ensure_hc_v_rail`) serves depth 1 alone - two rows, the count the driver was prepared for; any
other declines with its reason and the CPU verify serves - and builds the N-row command's stamps first (its
GEMV leaves, the rows requant, the per-row top-k) and the hc leaves past them. The parity cell is `tests/test_gpu_resident_hc.das`'s verify
cell: one round on the device against the split command's own one-row steps on the same picks
(`moe_pick_tape_lane`, a lane replay of a rows-form tape) at the split bar, and against the CPU's one-row
steps at the wide bar - the CPU compare is the parity evidence, the device-vs-device compare a rounding
control. Measured on the zen2 (`PERF_LEDGER.md`), the round does not pay while the pool's
hits sit near 40% on real text - the verify rows' host sums cost more than a plain token - so a server slot
leaves the round off on this form (`gpu_resident_experts_host`).
