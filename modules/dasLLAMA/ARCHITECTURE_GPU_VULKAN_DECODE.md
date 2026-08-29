# dasLLAMA Architecture - the Vulkan per-op tier's decode era

Companion to `ARCHITECTURE_GPU_VULKAN.md`; section numbers are `ARCHITECTURE.md`'s. This
document carries sections 2.2r-2.2t: the decode attention block over per-layer K/V mirrors,
the streamed expert layer's GPU/CPU split, and the whole-token decode span. The prefill window
chain, the cm2 tiles and the MoE expert chain these build on are `ARCHITECTURE_GPU_VULKAN.md`
sections 2.2j-2.2q.

### 2.2r The per-op tier's decode attention block {#decode-attention-block}

A MoE model rides the per-op tier, whose decode ran attention on the CPU: q/k/v/o are GEMVs
over weights the tier already holds resident for prefill, and reading them from host DRAM
was the single largest cost of a decode token. The block moves one layer's whole attention
onto the device as ONE submit: the q/k/v class GEMVs over the resident quad (k and v ride
the q stack's activation image), the qk-norm+rope kernel (or the plain rope) storing the
token's K/V row into the layer's f16 mirror, the decode attention over that mirror, the
attention row requantized in the `wo` plane's form, and the `wo` GEMV. The `wo` row and the
RAW k and v projection rows come back - f32, straight off the k/v GEMV output, because the
qk-norm and rope ran only on the mirror copy - so the host re-derives its own cache row and
keeps every position for the CPU arms: the CPU fallback, and a later window's CPU prefill.
The PREFILL chain's readback is the other way round; its k rows come home roped.

**The block serves one shape, and `attn_dec_shape_ok` is the whole gate.** The layer's q, k, v
and o planes are resident, the k source is the layer itself (no shared KV), the session's cache
is flat (not paged) and neither codec is tq4, the rope covers the whole head, and the model
carries no qkv or output bias, no v-norm, no attention sinks, no logit softcap, no q gate and no
sliding window - the chain's kernels implement none of them. q, k and v also share one quant
class, because k and v read the q stack's activation image.

**The mirror is per layer, keyed by the q plane offset, and capped** (`DAT_MIRROR_ROWS` rows;
VRAM is layers x rows x kvd x 4 bytes). The loader reports that need
(`set_moe_gpu_dat_need`) and the tier carves it from the weight budget BEFORE placement, the
way the stream slots are carved: the mirrors are allocated after placement, and un-carved they
came out of the desktop reserve and paged the resident expert stacks (the FFN chain's submit
went from 147 us to 741 us). The prefill chain fills it from its own f16 K/V
shadows at the end of each layer's window (`ARCHITECTURE_GPU_VULKAN.md` sec.2.2q's fa arm converts the attended prefix;
the fill is a device copy of it), and the block appends one row per served token.

**Ownership is a generation plus a per-layer count.** Every prefill claims ONCE, at the end of
`forward_prefill_body` - the token entry and the embedding entries alike - and the tier mints a
generation only when a window's fa arm filled a mirror since the last mint (`record_at_cmd`
counts the fills); a mirror no window filled empties at the mint, because it holds another
session's rows. A prefill that filled nothing mints nothing, and the session keeps whatever
generation it had - a stale one, or none. The session keeps the minted generation in
`Session.dat_gen`; a layer serves a token only when the session's generation is current and the
layer's count is exactly `pos`. Anything else names its remedy: a stale generation claims fresh
at the first layer that asks (every layer's count to zero - a session starting with decode, a
CPU-prefilled session, or a session taking the mirrors over), a count off `pos` HYDRATES the
layer from the host cache's rows [0, pos) (f16 rows copied, f32 rows converted, any other codec
decoded to f32 on the host first) - a CPU prefill ahead of the block, or a rewind. A count that
can never match (past the cap) declines to the CPU chain. The block therefore never depends on
which arm produced earlier positions; it depends on the host cache being whole, which the
readback keeps true. The measured figures in this section come from `benchmarks/decode_prof.das
--depth 0` under `DASLLAMA_GPU_PROF=1` and `benchmarks/lcpp_bench.das -t 16` (pp512 / tg128), on
the reference box (zen2 16 lanes, 5060 Ti, driver 610.74).

**A resident layer's FFN carries a TAIL when the next layer's attention rides the block**
(`DASLLAMA_GPU_DEC_TAIL`, on): the down rows never come home - the chain continues on the device
with the routed combine onto the residual row, the next layer's attention rms, and the requant
into that layer's q-stack acts image - and is submitted WITHOUT a fence. The next attention
submit's fence covers both (fence signals respect submission order on one queue, and the tail's
closing barrier reaches that submit's GEMVs the same way), so the pair costs ONE host wait, and
the residual row comes back with that attention's readback. The host arms the tail per layer at
attention time with a forward look (`attn_dec_gpu_next_serves`): only when the next layer would
serve with no remedy, so a hydrate or a claim is never pre-empted; a streamed layer's FFN (CPU)
and the last layer keep the plain form, whose host reduce the combine kernel mirrors in order.

### 2.2s A streamed expert layer's prefill is split between the GPU and the CPU {#streamed-layer-split}

**The GPU takes the head of the routed-expert list, the CPU the tail, concurrently.** A layer
whose expert stacks live on the host (the streamed layers of a model past the weight budget) used
to stream every expert's planes in whole and idle the CPU; the plain streamed rail still does that
when `DASLLAMA_GPU_MOE_SPLIT` is off. On, the router's per-expert bucket counts are walked in
LPT order (largest bucket first) and the GPU claims experts while its predicted time - the
claimed experts' plane bytes over the bus rate plus its bucket rows times the cm2 chain's row
cost - stays under the CPU's predicted time for the rows left (its kq groupn row cost). The
three constants (`SPLIT_BUS_BPS`, `SPLIT_GPU_ROW_S`, `SPLIT_CPU_ROW_S` in `dasllama_blocks.das`)
sit at 13.5 GB/s, 1.2 us and 10 us - the reference box's measurements (zen2 16 lanes, 5060 Ti on
PCIe gen4 x8; `benchmarks/lcpp_bench.das -t 16` pp512 under `DASLLAMA_GPU_PROF=1`: the bus moves a
streamed expert's bytes at ~11 GB/s, the cm2 chain costs ~1.6 us per bucket row, the CPU kq
groupn chain ~10 us per bucket row), tuned to where the split balanced best. A head that would cover every expert falls back to
the plain rail - streaming the whole layer beats splitting it against an idle CPU.

**The GPU head streams only its experts' regions** (`copy_stream_group_partial` copies the
claimed experts' plane ranges into the stream slot at their expert offsets), records the cm2
chain asynchronously (`vk_moe_ffn_batch_xf_begin`) and joins after the CPU tail
(`..._join`); the CPU requantizes, gathers, and runs the tail's gate/up/down for its experts
alone, and the two partial outputs reduce into one combine. Where the split CAN engage - the knob
on, a cm2 device - the tier keeps ONE stream slot instead of two (`stream_slot_count`): the
partial copy is on demand per layer and never prefetches the next group, so the second slot
would only hold VRAM the resident stacks want. A device the split cannot serve keeps both slots
and the whole-group rail's overlap - compute on one while the transfer queue fills the other.

### 2.2t The whole-token decode span {#whole-token-decode-span}

**Every resident expert layer's decode runs as ONE recorded chain and ONE submit per token**
(`DASLLAMA_GPU_DEC_SPAN`, on; the `vulkan_moe_span` decode override, selected when the MoE
placement leaves expert layers or the classifier on the device, deselected at the model drop). The per-layer form costs a submit per attention and one per FFN, and each submit
carries ~75 us of queue and fence latency the GPU spends idle (`DASLLAMA_GPU_PROF=1`'s
`vk_dec prof` ledger over 96 tokens) - on a 48-layer model with 35
resident layers that is the largest single term of the token. The span keeps the host out of the
resident suffix [l0, n_layers): the host feeds layer l0's attention row in its plane's acts
form, the device runs per layer the attention chain (sec.2.2r above), the residual add fused with the
FFN rms (`cls_ar`, `add_on`), the gate/up feed requants, the router GEMV over an f32 plane
(`router_gemv_cls`: one workgroup per expert row, f32 in and f32 out - the host router's own
arithmetic rather than a quant chain, so the device's picks track the CPU's up to summation
order) and
the softmax + top-k (`topk_cls`), the FFN chain, the routed combine
onto the residual row, and the next layer's attention rms + requant into that layer's feed
image; the residual row and every layer's raw k/v rows come home in one staging buffer after
the one fence. The streamed layers below l0 keep the per-layer path (their FFN is the CPU's),
so the override runs them on the arch's blocks first.

**The host's span arguments are a cache keyed on the carrier.** The per-layer offsets and the
router and norm row pointers are filled once per (carrier, suffix) and reused per token; the key
carries the carrier itself, not only the first offset, so a swap to a same-layout carrier refills
them instead of reading rows through the previous carrier's pointers.

**The top-k kernel writes the FFN chain's schedule.** The decode FFN GEMVs read per-row metas
(weight block, feed row) from the stacks' host-visible meta buffers; the host wrote them per
token, the span's top-k writes them on the device: slot j's weight block is the layer's expert-0
block plus e_j times the blocks per expert, its feed row is 0 for the gate/up stacks (one feed
row serves every slot) and j for the down stack (the act+requant output has one hidden row per
slot - the row stride is the down feed's blocks per row). The select mirrors the host's
`moe_select_core` softmax arm: max, exp floored at -80, sum, k picks of the largest with the
lowest index on ties and the winner knocked out, the picks renormalized (the sum clamped at
f16-min) and scaled. Only that arm rides - sigmoid gates, router biases, shared experts,
post-norms and biased experts decline to the per-layer path (`span_model_ok`).

**The span declines whole, per token, never per layer.** `span_first_layer` walks the layers
downward through the block's serve gate (so a hydrate a layer needs runs first) and stops at
the first layer that cannot ride; the override then requires at least two layers and NO
resident layer under the suffix - a resident layer there would arm its FFN tail against the
suffix's first feed (the layer under is a streamed one, or there is none). A model whose gate
is not the softmax-with-renorm the top-k kernel mirrors, whose expert count or routed slots
exceed the kernel's one-workgroup reach, whose router planes exceed one SSBO range, or whose
arch binds a non-standard attention or FFN block declines whole (`span_model_ok`). The gemv sets carry no hazard bits on their feed and meta bindings, so the
span's recorder declares those edges by hand (`vhz_dep` on the span's own region bits) after
each requant and after the top-k. The loader carves the router planes with the mirrors
(`set_moe_gpu_dat_need`).
