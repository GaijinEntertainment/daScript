# dasLLAMA Architecture - the Vulkan tier's NextN draft head

Companion to `ARCHITECTURE_GPU_VULKAN_RESIDENCY.md`; a section is cited by its anchor. This
document carries the NextN draft head the resident driver homes beside the trunk - the draft
command that steps it, the prompt warm the window chain gives its slab, and the rollback a
rejected device verify takes from the copies the verify command made. The residency plan,
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
the CPU's. Under `DASLLAMA_GPU_PROF=1` the draft command stamps its dispatches into a list of its
own (`g_rdq_draft`): the upload, the cat rows' norms and requant (`draft_cat`), the eh_proj GEMV
(`draft_eh`), the head's attention norm and feed requant (`draft_attnorm`), the head layer's
roles under the trunk's names (`q`, `kv`, `qknrope`, `attn`, `wo`, `ar1`, `rq_f`, the FFN's), the
head norm and classifier feed (`draft_norm`), the classifier (`draft_cls`) and the picks
(`draft_pick`); its samples go to the draft's own ledger (`g_rdq_d`), printed every 32 drafts as
`vk_rdec gpu avg/draft over N: ...` (the idle at its end is the gap since the verify sampled
before it) and `vk_rdec host wall/draft over N: ...` - the memcpys piece holds the hydrate's
upload where one ran, the wait the transfer-queue landing, and the head row's readback its own
piece.

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

### A rejected device verify rolls the region back on the device {#resident-verify-rollback}

**A verify the resident seat served rejects with no host snapshot and no replayed step: the
recurrent state the verify advanced past the committed rows comes back from a copy the verify
command itself took.** The CPU round's reject on a recurrent model restores the snapshot it took
before the verify (`mtp_state_snapshot`: every recurrent slot flushed home, the host state
copied) and re-forwards the committed token. Where the driver homes the head on a hybrid, the
verify command copies, after row `i`'s fused step in each recurrent layer and before row
`i + 1`'s, that layer's slot state and both ring images into row `i` of a rollback scratch
(`rd_encode_roll_copy`: two device-to-device copies under the hazard rail - a transfer read of
what the step wrote, the next row's step ordered behind it - for rows `0 .. n - 2`, a reject past
the last row being an accept), so scratch row `a` holds the state the region would hold had the
round stepped rows `0 .. a` alone: the same dispatches on the same inputs, so the copy is that
state bit for bit and nothing re-derives it. The scratch is one buffer for every region (one
session verifies at a time; `RDec.roll_region` names whose rows it holds), `verify_rows - 1`
rows deep - the round's depth at one region - each row every recurrent layer's `nvh x ds x ds`
floats of state and `2 x cd x (dconv - 1)` floats of ring at the layer's `roll_off`, sized at the
head's arm once every recurrent layer is set (`rd_roll_alloc`; the plan counts the same bytes,
`rdec_rollback_bytes` - 21,528,576 a row on Qwen3.5-0.8B, `PERF_LEDGER.md`'s entry). Because the
copies bake the region's slot offsets, a hybrid records its verify commands per region
(`rd_v_idx`) where an attention model's stay region-free. The reject is then one small command:
the rollback seat (`vk_rdec_rollback`, through the tier's `install_rdec_rollback` and the
engine's `register_mtp_rollback_override("vulkan", ...)`) copies scratch row `a` back over the
live slots in a one-shot submission after the verify's landing - the verify completed at its
wait, so no rail runs between the two, and the next command orders behind the submission as
every state upload does - and flips the region's ring parity back to row `a`'s (the verify
flipped it once a row); the resident seat (`vulkan_resident_rollback`) cuts the region's rows and
the session's `dn_pos` to `pos + a + 1`, and its head rows to `pos + a` - row `pos + a` holds the
rejected draft's input. The slots stay the session's, valid
and dirty, so no state crosses the bus; the CPU restore (`mtp_state_restore`) is what releases
them, and it never runs here. The round (`mtp_reject`) takes verify row `a`'s landed logits and
post-norm hidden as `s.logits` and the carry at `pos + a + 1` - the seat landed every row's
(`mtp_logits_b`, `mtp_hrows`) - and `n_past` moves by `a + 1`. The trunk's K/V rows and the
head's rows above the cut are dead by the watermark on the device and in the host copy alike:
the verify read head rows `pos - 1 .. pos + n - 2` back to the host, rows `pos + a` and up hold
the rejected drafts' inputs, and the next draft at row `pos + a` rewrites its row on whichever
side drafts, so nothing reads a row above the next draft's. The snapshot's place in the round
follows the seat's decision: a declined seat leaves the session untouched, so the CPU verify
snapshots right before its own prefill (`mtp_verify_two`), and a served verify snapshots only
where the owner registered no rollback seat; a rollback seat that declines after its verify seat
served is an engine bug the round panics on by name, since no snapshot holds the state. The CPU
depth-1 round asks two rows, so one scratch row serves it; a round of depth k would reject to any
`a < k` through the same copies. `test_gpu_resident_hybrid_mtp_verify_reject` holds the
rolled-back slots and the next step bit for bit to a one-row session's, `test_mtp.das`'s
forced-reject round holds the stream to plain decode on both rails, and the profiler's
`mtp.rollback` section prices the reject where `mtp.snapshot` and `mtp.replay` did.
