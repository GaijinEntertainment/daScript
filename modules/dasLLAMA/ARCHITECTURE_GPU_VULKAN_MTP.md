# dasLLAMA Architecture - the Vulkan tier's NextN draft head

Companion to `ARCHITECTURE_GPU_VULKAN_RESIDENCY.md`; a section is cited by its anchor. This
document carries the NextN draft head the resident driver homes beside the trunk - the draft
command that steps it and the prompt warm the window chain gives its slab. The residency plan,
the marks swap and the logits landing the head rides are in
`ARCHITECTURE_GPU_VULKAN_RESIDENCY.md`; the same-slab verify that re-warms the head's rows a
round at a time is in `ARCHITECTURE_GPU_VULKAN_NROW.md#nrow-verify-command`; the Metal round
these levers take their shape from is `ARCHITECTURE_GPU_MTP.md`. The GPU backend role table
these sections build on stays in `ARCHITECTURE_GPU.md#gpu-backends`.

### The NextN draft head is one more layer the driver homes {#resident-draft-head}

**A NextN-headed model's draft head rides the arena as layer `n_layers`, and a recorded draft
command steps it.** Where the tier installed the draft seat (`install_rdec_draft`) and the carry
landing, and the head has the trunk's attention shape, a dense FFN and planes the resident GEMVs
serve (`resident_head_decline` logs any other head at load, whose draft stays the CPU's), the plan
counts eight planes - eh_proj [2dim -> dim] in its own format, the q/k/v/o quad, the FFN triple -
and one more K/V slot a region past the trunk's; a headless model plans what it always did. The
norms plane takes the head's q/k rows at index `n_layers` of the q/k block and five rows past it -
the attention, FFN, embed, carry and head norms, the final norm's row where the file ships no head
norm (`rdec_norms_len`) - since layer `n_layers`' own rows would index the final norm's. The layer
sits outside `RDec.layers`, so no trunk walk sees it. The draft command, one per region, norms the
uploaded embed row and carry `h` into one `[enorm ; hnorm]` row, runs eh_proj into x, the layer
through the token command's attention and FFN encoders on its slot at row `pos - 1` (the head's
own `TokMeta` block and rope rows), the head norm into the carry's row, the classifier and the
pick; the logits, the pick and that row land (`mtp_h`, `mtp_h_pos1` 0) and the row's K/V comes
back to the host. The region tracks the rows its slot holds as the host does
(`RdecRegion.head_cnt`: a claim zeroes it, every prefill call lowers it to `start_pos - 1` and the
chain's prompt warm raises it to the prompt's end, a declined draft lowers it to its row, a verify
raises it past its rows where the held rows reach them), and a draft uploads the host rows past it
first. The host's rows are the round's copy: the CPU seam and a CPU draft write them, and every
row the device writes - a draft's, the verify's re-warm rows, the prompt warm's - comes back to
them, so the CPU round's reject path and a draft the seat declines read what the device holds. The
"vulkan" owner (`register_mtp_seat_owner`: the driver armed on a NextN model) registers the draft
and verify seats (`register_mtp_draft_override`, `register_mtp_verify_override`); the round stays
the CPU's.

### The window chain warms the head's slab over the prompt {#resident-head-prompt-warm}

**A resident-served prompt warms the head's K/V rows on the device, one more layer after the
trunk's last, and the CPU warm stands down behind it.** The CPU warm (`mtp_prefill_warm`) writes
head row `i` of a prompt's rows `0 .. npos - 2` from `[enorm(embed(tok_{i+1})) ; hnorm(h_i)]` -
`h_i` the trunk's post-final-norm hidden of row `i` - at head position `i` over the head rows
below it, reading the host residual rows the CPU trunk left in `x_b`. A prefill driver that
landed the logits itself (`prefill_override_logits_done`) never wrote those rows - the device
embed gather never touches `x_b`, and behind the CPU embed they hold embeddings - so the warm
runs only where no such driver served the prompt, and the window chain warms the slab itself
where the round is on (`get_mtp_spec`), the head rides the driver and the embed is unscaled (the
head reads the raw embed row the chain's residual holds; `rdec_head_warm_want` asks the driver
per call through the tier's `install_rdec_head_warm` seat). Per window the chain keeps the
window's embed rows before layer 0 overwrites the residual, runs the last layer's FFN and final
norm over every row instead of the last thirty-two, and after the classifier and the carry copy
encodes head rows `j` at positions `pos0 + w0 - carry + j`, `carry` one from the second window
on: the trunk's post-norm rows copied a row down with the previous window's last row at row 0,
the embed and carry norms over the shifted rows, the two halves interleaved into the
`[enorm ; hnorm]` image by two multi-region copies, the eh_proj GEMM into the residual plane, the
head's attention norm through the prologue set, its feed, its k and v GEMMs over its own planes,
their norms at the head's q/k slot and the rope store into the head's mirror slot at the rows'
own rope rows (`pf_head_warm`) - the prefill tiles and encoders the trunk's layers take, the
head's rows carrying no profiler stamp. The chain writes `npos - 1` head rows a prompt
(`vk_rdec_head_warm_rows`); the override reads them back into the host cache, uploads the host
rows below `start_pos` the device never saw - the seam row the CPU seam wrote, and any stale
stretch under it - and counts the region's head rows to `start_pos + npos - 1`
(`rdec_prefill_head_rows`), so the next draft hydrates nothing and the host copy the CPU round
reads stays the device's. A chain that cannot warm - the head off the device, a scaled embed, a
feed plane the chain does not carry - says so once and leaves the head's prompt rows as the host
holds them: the round drafts off stale history until its verifies' re-warm rewrites the rows,
acceptance dips, and nothing reads a host row nobody wrote. The warm's cost is eh_proj and the
head's k and v projections over the prompt plus the last layer's FFN over the whole window
(`PERF_LEDGER.md`'s entry, which the pod measures).
