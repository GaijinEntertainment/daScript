# dasLLAMA Architecture - the Vulkan tier's NextN draft head

Companion to `ARCHITECTURE_GPU_VULKAN_RESIDENCY.md`; a section is cited by its anchor. This
document carries the NextN draft head the resident driver homes beside the trunk - the draft
command that steps it, the prompt warm the window chain gives its slab, the rollback a
rejected device verify takes from the copies the verify command made, the verify window a
declined verify seat runs the CPU verify in, and the speculative knob's per-step gate on the
carry. A head that rides the hyper-connection chain - its routed experts on the host beside the
trunk's - is `ARCHITECTURE_GPU_VULKAN_HC.md#hc-draft-head`, its verify `#hc-verify-rows`. The residency plan,
the marks swap and the logits landing the head rides are in
`ARCHITECTURE_GPU_VULKAN_RESIDENCY.md`; the same-slab verify that re-warms the head's rows a
round at a time is in `ARCHITECTURE_GPU_VULKAN_NROW.md#nrow-verify-command`; the Metal round
these levers take their shape from is `ARCHITECTURE_GPU_MTP.md`. The GPU backend role table
these sections build on stays in `ARCHITECTURE_GPU.md#gpu-backends`.

### The NextN draft head is one more layer the driver homes {#resident-draft-head}

**A NextN-headed model's draft head rides the arena as layer `n_layers`, and a recorded draft
command steps it.** Where the tier installed the draft seat (`install_rdec_draft`) and the carry
landing, and the head has the trunk's attention shape, a dense FFN and planes the resident GEMVs
serve (`resident_head_decline` logs any other head at load, whose draft stays the CPU's; on the
hyper-connection chain a routed head with its experts on the host is admitted too,
`ARCHITECTURE_GPU_VULKAN_HC.md#hc-draft-head`), the plan counts eight planes - eh_proj [2dim -> dim]
in its own format, the q/k/v/o quad, the FFN triple (a routed head's dense triple is its shared
expert's) - the chain's planes for a head on it, and one more K/V slot a region past the trunk's; a
headless model plans what it always did. The
norms plane takes the head's q/k rows at index `n_layers` of the q/k block and five rows past it -
the attention, FFN, embed, carry and head norms, the final norm's row where the file ships no head
norm (`rdec_norms_len`) - since layer `n_layers`' own rows would index the final norm's. The layer
sits outside `RDec.layers`, so no trunk walk sees it. Where the head rides, the per-row quant
planes (`xq_dev`, `xs_dev`) hold twice `dim` a row: eh_proj reads the `[enorm ; hnorm]` row through
them (`vk_rdec_prepare`'s `wide`, `vk_rdec_set_head`'s `xq_bytes`). The draft command, one per region, norms the
uploaded embed row and carry `h` into one `[enorm ; hnorm]` row, runs eh_proj into x, the layer
through the token command's attention and FFN encoders on its slot at row `pos - 1` (the head's
own `TokMeta` block and rope rows), the head norm into the carry's row, the classifier and the
pick; the pick and that row land (`mtp_h`, `mtp_h_pos1` 0), and the seat answers the pick as the
draft's token (`MtpDraftOverrideFn`), so the round's greedy walk (`mtp_draft`, `pick_only`)
compares ids the host never re-derives and the logits stay on the device - the picks-only
transfer twin (`ARCHITECTURE_GPU_VULKAN_RESIDENCY.md#logits-transfer-queue`); `forward_mtp`'s
callers and a sampled walk land the logits too, and the landing's finite check reads them there.
The head's rows the device writes stay on the device: the region counts the rows its slot holds
(`RdecRegion.head_cnt`: a claim zeroes it, every prefill call lowers it to `start_pos - 1` and the
chain's prompt warm raises it to the prompt's end, a draft raises it past its row - the host's rows
past the held ones go up with the draft - and a verify past its rows, the draft before it holding
the rows below the re-warm's (the seat panics by name where it does not), a host write lowers it to
the written row) beside the rows the host cache holds as the session's (`RdecRegion.head_host`:
the prompt warm's readback raises it with `head_cnt`, a device write lowers it to the written row,
a host write raises it past the rows written), so every row written so far is current on the side
with the higher count (`rdec_head_device_wrote`, `rdec_head_host_writes`;
`test_gpu_resident_hybrid_mtp_draft_upload` holds a draft's upload counted and hydrated back).
The device's rows come down on a pass
(`rdec_head_hydrate`: the rows below the row the CPU reads next that the device holds and the host
does not, through the head warm seat's `read`) - a draft or a verify the seat declines, the
continuation's seam, which the driver's seam seat (`register_mtp_seam_override("vulkan", ...)`)
hydrates for and leaves to the CPU seam, a steal and a drop (`rdec_hydrate_host`) - so no served
round reads a head row back, and the CPU round's reject path and a draft the seat declines read
what the device holds. The "vulkan" owner (`register_mtp_seat_owner`: the driver armed on a NextN
model) registers the seam, draft and verify seats (`register_mtp_seam_override`,
`register_mtp_draft_override`, `register_mtp_verify_override`); the round stays the CPU's. Under `DASLLAMA_GPU_PROF=1` the draft
command stamps its dispatches into a list of its own (`g_rdq_draft`): the upload, the cat rows'
norms and requant (`draft_cat`), the eh_proj GEMV (`draft_eh`), the head's attention norm and
feed requant (`draft_attnorm`), the head layer's roles under the trunk's names (`q`, `kv`,
`qknrope`, `attn`, `wo`, `ar1`, `rq_f`, the FFN's), the head norm and classifier feed
(`draft_norm`), the classifier (`draft_cls`) and the picks (`draft_pick`); its samples go to the
draft's own ledger (`g_rdq_draft_ledger`), printed every 32 drafts as `vk_rdec gpu avg/draft over N: ...`
(the idle at its end is the gap since the verify sampled before it) and `vk_rdec host wall/draft
over N: ...` - the memcpys piece holds the hydrate's upload where one ran, the wait the
transfer-queue landing, the landing copy the pick's read (and the logits' copy where they land).

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
holds them: the round drafts off stale history until its verifies' re-warm rewrites the rows and
acceptance dips, and the first draft uploads the host rows below its row as the host holds them.
Whether the warm ran or not, the override's readback (`rdec_prefill_head_rows`) hydrates the
device-only rows below `start_pos` first (`rdec_head_hydrate`, before the sync), so a
continuation whose seam did not run - a rewind, a foreign handoff - raises the host's count only
over rows the host holds current, never over a stale copy of a row the device wrote. The warm's cost is eh_proj and the
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
them, and it never runs here. The round (`mtp_reject`) takes verify row `a`'s landed post-norm
hidden as the carry at `pos + a + 1` and its landed logits as `s.logits` - or, under the caller's
pick ask (`Session.pick_asked`), where the seat landed the rows' picks and no logits, publishes
row `a`'s pick for the caller's `sample_` (`land_pick`) - the seat landed every row's (`mtp_picks`,
`mtp_hrows`, and `mtp_logits_b` where the logits land) - and `n_past` moves by `a + 1`. The
trunk's K/V rows and the head's rows above the cut are dead by the watermark on the device and in
the host copy alike (`rdec_head_rows_cut`): the verify wrote head rows `pos - 1 .. pos + n - 2` on
the device, rows `pos + a` and up hold the rejected drafts' inputs, and the next draft at row
`pos + a` rewrites its row on whichever side drafts, so nothing reads a row above the next
draft's. On a model with no recurrent layer the round skips the rollback seat (`mtp_reject`:
`rolled` holds from `recr_mask == 0`) - no slot to copy back, so nothing cuts the region's rows
or its head rows, and the same watermark rule covers them: the next draft at row `pos + a`
rewrites its row, and the rows above it are dead. The snapshot's place in the round
follows the seat's decision: a declined seat leaves the session untouched, so the CPU verify
snapshots right before its own prefill (`mtp_verify_two`), and a served verify snapshots only
where the owner registered no rollback seat; a rollback seat that declines after its verify seat
served is an engine bug the round panics on by name, since no snapshot holds the state. Every
decline of the verify seat sits above `rdec_dn_own_all`, the call that moves the session's
recurrent state to the device: a decline below it would hand the CPU verify a session whose
state the device holds. `REVIEW.das`'s `check_verify_decline_before_state_move` walks
`vulkan_resident_verify_gated` and `vulkan_resident_verify_go` and fails a `return false` after
that call; it licenses no names. The CPU
depth-1 round asks two rows, so one scratch row serves it; a round of depth k would reject to any
`a < k` through the same copies. `test_gpu_resident_hybrid_mtp_verify_reject` holds the
rolled-back slots and the next step bit for bit to a one-row session's, `test_mtp.das`'s
forced-reject round holds the stream to plain decode on both rails, and the profiler's
`mtp.rollback` section prices the reject where `mtp.snapshot` and `mtp.replay` did.

### A declined verify seat runs the CPU verify inside a window the resident prefill declines {#resident-verify-window}

**A speculative round whose verify seat declines runs the CPU verify's two-row `forward_prefill`
inside a verify window the session carries, and the resident prefill declines that window by name
as the first rung of its ladder.** The CPU verify (`mtp_verify_two`) names the session in
`mtp_verify_window_active` around its two-row prefill; the resident prefill override reads it
before it binds a region, takes or claims a mirror or touches a recurrent slot, and passes the call
as `RdecPass.verify_window` through the hydrate every pass takes (`rdec_pass_hydrated`,
`ARCHITECTURE_GPU_VULKAN_RESIDENCY.md#resident-pass-hydrate`): the region's device-only trunk K/V
rows and head rows come down to the host cache, and the CPU rails take the two rows - the head
over the host rows the seat's decline hydrated, the trunk's rows the region holds staying on the
device, the round reading only rows the side that ran them wrote. A hybrid's recurrent state
needs no hydrate of the window's own: the CPU verify's snapshot (`mtp_state_snapshot`,
`dn_flush_all`) and the CPU chain's per-layer `dn_flush_layer` send each device slot home before
the CPU reads it, and the CPU chain runs only at the session's recurrent position
(`dn_forward_only_guard`) - a round off it, which the verify seat passes as `rewind`, the CPU
rails refuse by name, since the recurrent state is forward-only on every rail
(`test_gpu_resident_hybrid_mtp_rewind_refused` holds the pass and the refusal by name).
The window is the session's (`Session.uid`), so a panic inside it - that refusal among them -
strands the window on the session that died and every other session's prefill serves as before.
Without the window a model with no recurrent layer - a routed-head carrier in GLM-4.5-Air's
shape, whose head the driver declines - would have its two rows served by the window chain,
which leaves the host `x_b` rows the CPU verify's classifier reads unwritten; the fail-closed
guard for that case stays (`mtp_verify_served_panic`: a prefill driver that landed the logits
inside the window), and the decline is what keeps it unreachable. The resident verify seat and
this window are the two ways a round's verify rows run, and a round never mixes them.

### The speculative knob gates the carry per step, never the plan {#resident-spec-knob-gate}

**The carry landing, the stored classifier feed and the hidden-row copy consult the speculative
knob (`get_mtp_spec()`) on every step, so a NextN model with speculation off pays no carry and
keeps the last layer's fusions.** The token command's last layer stores its classifier feed
(`storex` on the residual step's requant stamp, `rd_next_feed`) only while the knob is on, and
that store is what turns the fused residual-plus-requant forms off at that site (`rd_rows_fuse`,
and `comb_rq` on a MoE last layer); with the knob off the last layer takes the fused forms a
headless model takes, no `xb_dev -> hid_host` copy runs and no carry is stashed. What the knob
never moves is the plan: the head planes, the head's K/V slot, the verify rows, the rollback
scratch and the landing's own planes (`hid_host`, the transfer-shared `xb_dev`) are provisioned
at the arm whether the knob is on or off - `RDec.carry` is set on every NextN model whose tier
installed the landing (`rdec_carry_planned`), `RDec.carry_live` is the step's reading of the knob
(`rd_carry_sync` at every served entry re-records the commands whose last site stores or lands
the row when it flips), the landing (`vk_rdec_land_carry`) asserts the plan and the per-step
stash (`rdec_carry_on`) reads the knob - so a knob flip (`set_mtp_spec`, the server's `--mtp`,
the scheduler's per-tick arm) never re-plans the residency and a knob turned on after the load
lands the carry from its first step: a NextN model with speculation off carries the head's bytes
and runs the step a headless model runs. The draft's hidden is the head's own row and lands
whatever the knob says - the draft command copies it itself wherever the transfer command would
not (no transfer family, or the knob off) - and a verify with the knob off has no stored row to
land, so the verify seat declines it to the CPU verify.
