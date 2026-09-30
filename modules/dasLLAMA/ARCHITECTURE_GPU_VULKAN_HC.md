# dasLLAMA Architecture - the whole-model driver's hyper-connection chain

Companion to `ARCHITECTURE_GPU_VULKAN.md`; a section is cited by its anchor. This document carries the
whole-model driver's form for a hyper-connection model (qwen4exp, Qwen3.8-Flash-Next): the wide residual
and its mixer seams on the device, the n-gram side input, and the split token command whose routed
experts run on the host. The heads the chain runs between its seams are
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
(`RDec.cmd_seg`, `rd_record_hc_segments`): a segment ends after a routed layer's router by landing the
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
256-expert reach of the device select kernels does not gate this form; one mirror region serves it, and
its N-row command and NextN verify are not written.

### The hot expert pool {#hc-hot-pool}

**A routed layer keeps its hot experts in device slots, and a step's picks split: the device runs the
hits while the host sums the misses.** The carrier has a working set - a layer's picks fall on a few
of its 512 experts far more often than on the rest - and the device decodes an expert's planes six to
ten times faster than the host does (`iq3s4` at the gate shape: 7 us a cold plane on the reference
card, the host's chain 61 us an expert), so the slots the card has room for pay. A pool is `slots`
slots a routed layer (`RdHotLayer`: a gate, an up and a down plane pair, each `slots` experts in the
plane's device layout, a slot's stride the expert's weight blocks), and the same count on every layer.
The count is the resident plan's (`ResidentPlan.hot_slots`, `resident_hot_slots`), sized with the
mirror because the two share the card's room. Unasked, the pool takes 64 slots where they fit beside
the mirror at the context asked; where they do not, it takes up to 32 out of the mirror's context,
which keeps a floor of 32768 positions - a slot the device serves is worth more than context past the
floor, and a plan that fills the card to its cap beside a desktop pages (the carrier's decode reads
2.9 tok/s so, 30 with the pool and a mirror of 88 thousand positions). `DASLLAMA_GPU_HEAT` asks a
count, served from the same floor's room, and 0 asks none. The pick shortens the mirror's context to
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

**The n-gram side input takes the panel form.** The window's side rows (`pf_hcemb`, [np x ind], the
host's `ngram_gather` per position) requantize once, the key and value GEMMs run over the rows, `PleGate`
(a workgroup a stream row, `rows > 0`) gates every row and writes its normed gated row into the panel
(`pf_hpanel`) instead of the ring, `PleConv` reads a tap inside the window off the panel and a tap before
it off the ring (`pos0` names the window's first position), and the window's last `min(rows, nrows)` panel
rows copy into the ring's slots (`pos % nrows`, the region the tok meta's `dnslot` names) for the rows after
the window - the decode steps then advance the ring on the device (`Session.ple_ring_device`). A prompt
served this way passes nothing to the CPU (`RdecPass.hc_prefill` is a model the seats do not hold), and
the deltanet state and the ring stay on the device as any other hybrid's do.
