# dasLLAMA Architecture - the attention kernels both discrete homes stamp

Companion to `ARCHITECTURE_GPU.md`; a section is cited by its anchor. This document carries the attention bodies
`dasllama_gpu_kernels_discrete.das` holds for the Vulkan and CUDA homes: the decode attention's key split, the prefill's
flash-attention tile, the K/V mirror's block codecs, q8_0 and tq4, with tq4's attention in the rotated basis, and the
batched rope store's strided k and v reads. The
Vulkan window chain the flash tile serves is `ARCHITECTURE_GPU_VULKAN.md#vk-prefill-window-chain`; the attention-side planes
the Vulkan driver uploads (the q/k/v bias, the sink logits, the output bias) are
`ARCHITECTURE_GPU_VULKAN_ATTN.md#vk-attn-planes`.

### The token command's attention splits a head's keys across workgroups {#vk-decode-attn-split}

**The decode attention dispatches a workgroup per (kv head, slab of its q heads, key split), and
the group's last piece combines.** A workgroup reads its kv head's K and V rows once and scores
them against the slab's query heads of the GQA group that share them - `DA_G` (four), or two on a
group of one or two heads (`da_slab_is_g2`: the `g2` stamps of `DaAttnT`, whose template constant
`G` sizes every score and V loop, so a group of one or two heads - gemma-2, gemma-4's sliding
layers, any MHA carrier - scores no dead heads; `da_slab_heads` is the one expression the kernel's
`G` and the host's workgroup count derive from); a group wider than the slab takes several slabs,
a narrower one leaves dead heads whose q rows are zero and whose reductions and stores are skipped
(`da_attn_row_wgs` counts a row's workgroups, the same count on either slab). The token command
picks the slab a layer (`da_slab_of` names it a `DaSlab` - four, two, or the sink stamp - and
`da_attn_stamp_enc` dispatches it); the per-op seam (`dasllama_vulkan_seams.das`) and the
attention-tower chain dispatch the four-head slab whatever the group. The pass is a chain
of latencies, not a stream of bytes: a workgroup a head walking two keys a step behind a subgroup
reduction each read Llama-3.2-1B's sixteen layers at 28 us a four-row step and 8.5 a one-row step on
the RTX PRO 4500, the same whatever the split, so the pass (`DaAttnT`) spends its threads on
independent work. The device figures in this section are read by `harness/vk_attn_probe.das` under
`-jit` in cm2 mode (device timestamps a layer) and by the `DASLLAMA_GPU_PROF=1` token profile of
`benchmarks/lcpp_bench.das`; `PERF_LEDGER.md`'s 2026-09-19 section is the record.

**A pass is one chain: the scores, the softmax, the V accumulate, the fold and the slab finish.** A
256-key chunk's scores take a thread a key: the K row's words load eight at a time (`KV16`, one
round trip a 128 dims), the slab's dots - one a head - read the q rows from shared memory, and no subgroup
reduction sits between the keys; the chunk's first V words load in the same round trip, since they
wait on nothing the scores compute. The softmax's max and sum a head are one subgroup reduction
each, the subgroups' values folded by every thread from shared memory (`sgp`, `ml`). The V
accumulate keeps a thread a (key group, eight dims) with a head's eight sums in registers, eight
rows' words in flight. The finish folds the key groups by subgroup shuffles, then across the
subgroups through the q slab (`qrow`, spent by then), a round of heads at a time. The driver serves
a 32-multiple head up to 256 and the 512 head; the resident gate declines 288..480, where the wide
arm addresses a second element at dim + 256. The shuffle fold across key groups runs where the
row's word count (`hs / 8`) is a power of two - 32, 64, 128, 256, 512; at 96, 160, 192 and 224 the
pass keeps every key group's partial in the slab, the finish sums them, and the threads past the
last whole key group hold no keys.

**The split ladder follows the attended span, and the last piece to land finishes.** The pass
cuts the attended span into `nsplit` 32-aligned pieces, each running the online softmax over its
piece into an unnormalized piece a head (max, denominator, accumulators; an empty piece weighs
nothing). The piece count is the span's: one piece under
`RD_UNSPLIT_POS` (512), `RD_SPLIT_PIECES` (four) to `RD_WIDE_POS` (3072), `RD_SPLIT_WIDE_PIECES`
(eight) past it for the one-row command alone, each capped by the partials plane's `attn_nsplit`
(`da_nsplit`: enough (head, split) workgroups to cover the SM count twice, at most
`DA_NSPLIT_MAX`); a sliding-window layer attends at most its window, so its count is its
window's span's under the command's; and a layer's count is capped so its (kv head, slab)
workgroups over the command's rows fit one wave of the SMs (`rd_layer_pieces`: the SM count over
the row's workgroups times the rows the command steps, never the planes' height, one at least) - the pass is a chain of
latencies a workgroup, and the ruler reads a second wave as a second chain: at 640 positions
and four rows gpt-oss's shape (64 heads, eight a kv head: sixteen workgroups a row) reads 16.4
us a layer unsplit and 34.9 at four pieces, Llama-1B's (eight a row) 14.5 and 22.6, the 30B's
(eight a row, head 128) 22.6 at one and 20.6 at two, where E4B's (two a row) wants its four
pieces - 22.7 against 34.4 unsplit - and every shape climbs past about thirty-two
workgroups a layer. The cap reads each command's own rows, so the one-row command fills the wave alone and the N-row
command fills it with its rows; the flat rows of a `--npl 4` bench run held through the cap (gpt-oss 209.1 -> 209.2, Llama-1B 459.2 -> 465.0, E4B 112.5 unchanged; `PERF_LEDGER.md`'s
2026-09-21 section). Where the cap reads one, the unsplit and wide twins record the same chain as
the split command (`unsplit_on` and `wide_on` read `attn_nsplit` alone), a duplicate the form switch
pays in recording, never in a step. The ruler (`harness/vk_attn_probe.das`
under `-jit` on the pod, `PERF_LEDGER.md`'s 2026-09-20 section) read the rule at the Qwen2.5-0.5B
geometry on the RTX PRO 4500, us a layer: one piece 10.4 at 384 positions and 14.5 at 640 against
four pieces' 16.5 at both, four pieces 16.5 at 1024 against one piece's 18.6 and 18.6 at 1536
against 26.7 (the edge sits between 640 and 1024; the qwen3 0.6B geometry's four pieces win from
640 at one row), four pieces 20.6 at 2048 and 27 at 4096 against sixteen's 33 and 34; past 4096
eight pieces pay for one row alone - 33 against four's 41 at 8192, 47 against 72 at 16384 - and
lose at four rows (51 against 44, 81 against 74), so the N-row command stays at four pieces at
every span past 512. The old rule, covering the card twice whatever the span, recorded sixteen
pieces for a one-row step and eleven for a four-row step and paid double. The one-row and N-row
forms sum a layer's keys in one order - a batched row the row alone bit for bit - while the N-row
command's rows times the row's workgroups times the span's pieces fit one wave; past that the
N-row command takes fewer pieces and the regions rig holds its rows to the split bar instead
(`vk_rdec_rows_split_alike`). Every row of the batch sits in one band: a group's furthest row
picks the group's form, and a row batched across a band edge takes its group's. The
pieces land in the partials plane; past them sit the arrival counters (`partu`, a word a (row, kv
head, slab) at `cntoff`), each atomically bumped after a device-scope release. The piece that reads
its group's count last aligns each head's pieces by their maxes, normalizes, gates and stores the
row, and rearms the counter to zero (unsplit, the pass stores it), so no combine dispatch follows
and the split costs no second launch. The store quantizes the row for the `wo` plane (`rqk`: Q8_0
blocks by the 32-lane group's amax, Q8_K superblocks by the workgroup's on a head of 256 or 512), so
no requant dispatch follows either - but under a tq4 mirror, where the pass neither gates nor
requants (`ARCHITECTURE_GPU_SHARED_ATTN.md#vk-kv-tq4-basis`). A split device records the token command as a ladder of
forms over the same sets - the split chain and an unsplit twin at the epoch's record, the wide
twin on the first step at `RD_WIDE_POS` (a device whose `attn_nsplit` stays within the split
form's pieces has no wide twin) - and the profiler keeps each form's stamp names and restarts
its averages when a run crosses a band edge. Under `RD_UNSPLIT_POS` a head's whole row is at
most two of the pass's 256-key chunks, so the unsplit twin serves there.

**The flash tiles stay unsplit.** The flash tile's cm2 arm (`FaT` at `KHR = false`) runs one
workgroup a (head, 64-row q tile) unsplit, its KHR arm one a (head, 16-row q tile): a key split there
costs more in partial stores and a combine than the shorter key loop returns on every gemma shape
measured (`followup_vulkan.md` item 50). A model that softcaps its attention logits (gemma-2) takes
the tile's `CAP` leaves at head size 256: every scaled score through `cap * tanh(s / cap)` before the
mask, the 8-row tile serving any other capped shape. Every kernel that stages a head does it in a
256-thread workgroup, 32 dims to a lane and a second element a thread past 256;
`resident_layer_decline` names a layer outside the served head sizes.

**A sink model (gpt-oss) runs the pass's `SINK` twins.** The head's sink logit is a phantom key with
no value row, read off the sink plane (`ARCHITECTURE_GPU_VULKAN_ATTN.md#vk-attn-planes`) bound past the pass's eight bindings: the unsplit
store folds it into the row's max and denominator once, a split piece carries none, and the last
piece's combine (the pass's own `SINK` stamp) seeds its aligned max and denominator with it.

### The flash-attention tile accumulates O in f16 under a biased row max {#flash-attn-tile}

**The flash-attention tile (`FaT`, one template with a `KHR` axis, its cm2 arm here) accumulates O in f16 under a biased row max, masks only its edge steps, and lands its
output f16 when the `wo` feed is f16.** The running row max carries 3 ln 2, so every P = e^(S - M) sits at an eighth or under and
the f16 O accumulator cannot overflow; L carries the same bias, the final divide cancels it, and S, L and M stay f32. The mask pass
runs only on the steps that cross the causal diagonal or the tile's last window start; the KV loop carries `[dont_unroll]` (RTX PRO
4500, a 512-row window: E4B's attention 6868 -> 4014 us, gemma-3-1b's 2671 -> 1844). The `OUT16` instance writes the `wo` feed
plane directly, so no attn-to-f16 convert encodes; the f32 instance serves the quant route. **Without cm2 - a KHR-only card, or a
mode forced off cm2 - the tile's KHR arm (`FaT` at `KHR = true`, its twelve `fa_khr_*` stamps) serves the same head sizes but 96, the window, the softcap and the gated head of 256 (each thread's o row times the sigmoid of its gate columns at the store) from the same push constants on
subgroup-scope 16x16x16 fragments:** sixteen query rows a workgroup, 64 keys a step, each subgroup owning one 16-key column of
S = Q K^T (K^T a column-major load straight from the f16 shadow) and, after the softmax runs per element through shared memory, a
quarter of the head's output columns of P V (f32 fragments a step - an f16 accumulator rounds toward zero on RDNA3's matrix cores, `ARCHITECTURE_GPU_SHARED_GEMM.md#khr-mm-kq-tile` - rounded once to f16 for the step's stash and summed into each thread's f32 o row; the walk takes the head in
64-wide halves, which is why a head of 96 has a cm2 stamp alone and stays on the chunked pair off cm2). A key chunk that starts past
the window's end is skipped; the last one may load up to fifteen rows past it, so every KV mirror carries `RDEC_MIR_SLACK` elements past its
planes and the driver's prepare zero-fills both mirrors whole: the fragment reads the unwritten rows past the window's end, inside a plane and in the slack, and a masked probability times zero stays zero where a garbage NaN would not. The block-codec kernels lean on the same slack: a block's quants load as whole words, so a region's last block reads one word past its bytes. The tile stages nothing from the shadows: its q tile and the step's P V plane are sized for the 512-wide head on every stamp, 32 KB of the 49152 B of workgroup memory the tier requires of a device, so K^T and V fragments load straight from the f16 rows - an f16 mirror's, or a block codec's shadow. It runs its row max under the cm2 tile's 3 ln 2 bias (`FA_MAX_BIAS`). `fa_khr_serves` is the one pick the pre-flight and the window chain share: the tile wants those f16 rows and a 32-lane subgroup (its softmax runs eight lanes a row), serves a gated model - a q plane of `[q | gate]` - at the 256 head alone (its gated stamps; a gated head of another size keeps the chunked attention pair, and under a tq4 mirror the un-rotation gates and the tile runs ungated, `ARCHITECTURE_GPU_SHARED_ATTN.md#vk-kv-tq4-basis`), and serves the softcap at an ungated 256 head alone, its cap stamps - the cm2 tile's rule but the head of 96.

### The K/V mirror's block codecs: q8_0 and tq4 {#vk-kv-block-codecs}

**The mirror holds the CPU cache's own bytes, in one of four codecs.** A session is served only on
the armed mirror's codec, and a pass to the CPU rails copies its rows down byte for byte, so a
device row is the cache's row: f16 or f32 elements, q8_0's 34-byte blocks of 32 elements (an f16
scale, 32 int8 quants) or tq4's 18-byte blocks (an f16 scale, 32 four-bit quants;
`dasllama_kv_codec.das` is the truth for both). `resident_mirror_dtype` reads the codec asked -
`set_gpu_kv_dtype` (`reset_gpu_kv_dtype` lifts it), the server's `--kv-dtype`, `DASLLAMA_GPU_KV`,
f16 with none - and `resident_mirror_dtype_of` arms the codec the model serves (`kv_codec_served`):
tq4 steps down to q8_0 on a head that is not a power of two - the NextN head's among them - or on
an attention with a Q/K/V bias, whose K bias outliers would set every 4-bit block's scale; either
block codec steps down to f16 on a head or a K/V row that is not whole 32-element blocks
(`kv_codec_geometry_why`). A session's creation applies the same rule, so no session of such a
model carries a codec it cannot serve. The load announces every ask that stepped down: an f32 ask
(the mirror arms f32 under `DASLLAMA_VK_KV32=1` alone), a `DASLLAMA_GPU_KV` value that names no
codec, a model's heads or bias. A q8_0 mirror is
17/32 of the f16 mirror's bytes and tq4 9/32, which is what the codecs are for: the mirror's bytes
are what a card's context and its resident experts compete for. A block-codec mirror is addressed
in blocks. `RLayer.mir_base`, a token's `mirbase` and the region stride count units - an element
under f16 and f32, a block under the block codecs (`kv_row_units`, `kv_unit_bytes` the unit's
bytes, over `RDec.kv_dt`) - so every base the float kernels compute carries over, and only the
kernels that touch mirror bytes know a unit's size. Those kernels address a byte in 32 bits
(`RDEC_CODEC_BYTE_SPAN`), so a block-codec slab holds 4 GiB a side at most whatever range the device
binds, and the plan caps the context there (`resident_binding_ctx`, `BLOCK_CODEC_SIDE_BYTES`).

The codec's kernels are shared templates in the discrete layer (`KvQ8StoreT`, `KvTq4StoreT`, `KvTq4UnrotT`, `KvShadowT`), each
home stamping its leaves under the same dispatch stems; the CUDA home's dense slice serves both block codecs on them, its
tq4 stamps under the emitter's `precise` option (`modules/dasCUDA/ARCHITECTURE.md`, the emitted kernel), the twin of the
Vulkan leaves' `precise`.

**The stores stay the float stamps; a store pass quantizes.** Under a block codec the rope stamps
(the bias, the q/k norm, the rotation, the V copy) write K and V rows as floats into staged planes:
`kst_dev` / `vst_dev` in the token command, a row a batch row, addressed through the static token
block `stage_tok` whose row r sits at position r, and `pf_kst` / `pf_vst` in the window chain. The
codec's store kernel (`KvQ8StoreT`, `KvTq4StoreT`; the `_b` stamps take the window's rows by
position; `rd_enc_kv_store` encodes either) then quantizes the staged rows into the mirror at each
row's region and position, byte for byte the CPU's `quantize_q8kv_row` and `quantize_tq4kv_row`.
One byte pair differs by design: a block scale past the half range stores as +/-65504 where the
CPU's convert stores an infinity, since no f16 store into a K/V buffer leaves the finite range.
Every store form the float mirrors have - the plain rope, the fused q/k norm, the partial rope, a
shared-KV layer - so serves a block codec with no stamp of its own, and no existing stamp's SPIR-V
moves. The staged planes ride the
window chain's V-staging hazard bit (`VHZ_VST`), idle in the token command.

**The decode attention reads the blocks natively and scores them with the packed integer dot.**
`DaAttnT`'s `KVQ8` and `KVTQ4` constants replace the pass's row loads: a thread loads its block's
scale once and the quants as words - a block starts 0 or 2 bytes off a word, and the load shifts by
that skew (`kv_blk_scale`, `kv_skew_word`). A block-codec stamp (`KVBLK`) first quantizes the slab's
query rows to q8_0 blocks in the workgroup, a thread a block (`qq`, the words; `qsc`, the scales),
the CPU chain's own query quantization, and a K word then scores against a query word in one
`sdot4` (`VK_KHR_shader_integer_dot_product`, the feature the Q8 batch tile already needs), the
block's integer sum scaled by both scales at its end; a tq4 word's nibbles score as their two
byte-expanded halves (the even elements' word and the odd's) against a query packed the same way,
the nibble's -8 offset paid once a block as eight times the block's query sum (`qsum`). The two
chains quantize the query the same way (`quantize_q8_0_into`'s amax over 127 and its rounding) but
from two nearby f32 queries, so their roundings part by a quant step on a share of the elements:
the q8_0 cells' bars carry that noise over the f16 reading (`tests/test_gpu_resident_qwen2.das`,
`tests/test_gpu_resident_regions_q8kv.das`), which the packed dot buys back at depth - a two-stream
8k decode on Qwen3-0.6B reads about 5 % faster under q8_0 (`PERF_LEDGER.md`). The V accumulate stays a float multiply of the probability
and the dequantized quant: it carries no integer form. `da_attn_stamp_ensure` / `_set` / `_enc`
pick the stamp by the mirror's codec, the sinks and the slab.

**The prefill's attention reads an f16 shadow.** The flash tiles and the chunked pair load K and V
through tensor layouts in f16 stamps, which a block's interleaved scale does not fit. Before a
layer's attention a shadow pass (`KvShadowT`, a stamp a codec) dequantizes the rows that layer
attends - from a sliding layer's first attended tile on - into one f16 plane pair every layer
shares (`ksh_dev` / `vsh_dev`, the layer's rows from base 0), each half clamped to the finite
range, and the f16 stamps read that. The
window's own rows are quantized first, so the prefill attends the rows a later decode step reads.
The pair is scratch the plan charges (`resident_shadow_bytes`): one layer's rows at the region's
context, both sides, as halves, with the window chain's staged float rows (`pf_kst` / `pf_vst`)
beside it; each plane binds as one range, which caps a region's context too (`resident_shadow_ctx`).
The prepare zero-fills both shadow planes whole, their slack included
(`RDEC_MIR_SLACK`), so a flash fragment that loads rows past the attended span reads finite
halves: a masked probability times a finite value stays zero, where a NaN would not. The pair
rides the split-k scratch hazard bit (`VHZ_SK`), since no split-k reduce runs between a layer's
shadow pass and the attention that reads it. Its cost is a dequant of the attended span a layer a window:
Llama-3.2-1B Q4_K_M reads pp8192 15860 under q8_0 against 16854 under f16 (debug-jit, `PERF_LEDGER.md`).

**The NextN head's rows are mirror rows like a layer's.** The draft command runs the head through
the token command's attention head (`rd_encode_attn_head`), so its row stages, quantizes and
attends as a trunk row does, addressed through the head's own token block; the window chain's
prompt warm (`pf_head_kv_store`) stages the head's K and V rows and runs the window store, with
no q head to rotate under tq4, since the warm attends nothing.

### tq4 attends in the rotated basis {#vk-kv-tq4-basis}

**A tq4 row is the head's row under a sign flip and an orthonormal Walsh-Hadamard transform, and a
dot product survives the rotation.** `KvTq4StoreT` runs a workgroup a (row, head): it rotates the
staged K and V heads and quantizes them, and it rotates the row's q heads in place - a shared-KV
layer stores nothing and still rotates q. The scores are then dots of rotated queries against
rotated keys, the same dots. The V accumulate sums rotated rows, so the pass's output is the
attention row in the rotated basis, and `KvTq4Unrot` takes each head back: the inverse transform,
then the signs (the CPU's `fwht_unsign_row`), a workgroup a (row, head). The signs are the CPU's (`tq4_fill_signs`: one row of
`KV_TQ4_SIGNS` floats, a head reading its prefix). The butterfly passes (`KvTq4WhtBase`, the base
shell both kernels derive) run barrier-ordered over a
workgroup's shared slab under the `precise` kernel option, so each sum rounds as the CPU's does and
the stored bytes are the CPU's.

**What acts on the attention row acts after the un-rotation.** A gated-q model's gate (`q_gated`)
and the requant folded into the pass's store (`rqk`) both read the row in the model's basis. Under
tq4 the pass runs ungated (`rd_attn_gated`) with `DA_RQ_NONE`, the un-rotation applies the gate -
it reads the gate off the q plane's head stride - and the separate requant follows. The window chain does the same around its tiles: the store rotates the window's q
rows, the shadow holds rotated rows, the tiles run their ungated stamps into the f32 plane, the
un-rotation gates, and the f16 feed converts after it.

**A four-bit quantizer amplifies rounding.** A quant on a rounding edge flips on a float's last
bit, and a tq4 step is an eighth of its block's range where q8_0's is a 127th. Two chains that sum
in different orders part by whole logits on a small model: on Qwen2.5-0.5B the device and the CPU
chain read 2-6 apart at a 10-15 max logit, and the CPU chain itself 4-8 from its own f16 rows. So
no logit bar holds a tq4 mirror to the CPU chain. The kernel cells hold the store to the CPU's
bytes and the attention to the dequantized rows (`test_vulkan_kv_codec_kernels.das`), and the
model cells hold comparisons: the device reads no further from the f16 chain than the CPU's tq4
rows do, and layer 0's stored bytes are the CPU cache's but for a quant on a rounding edge
(`test_gpu_resident_qwen2.das`).

### The batched rope store reads k and v through a stride and a base {#rope-kv-merged-reads}

`RopeKvStoreBT` addresses a window row's k head at `row * kstride + h * hs` and its v rows at
`voff` inside the same buffer, so one body serves a split k/v projection (`kstride = kvd`,
`voff = npos * kvd`) and a merged one (`kstride = 2 * kvd`, `voff = kvd`). Each home decides
when it merges - the Vulkan home under its arena rule
(`ARCHITECTURE_GPU_VULKAN.md#vk-prefill-window-chain`), the CUDA home on the planes' formats
alone - and the kernel reads only the two fields.
