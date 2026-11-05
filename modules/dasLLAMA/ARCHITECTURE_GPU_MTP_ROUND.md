# dasLLAMA Architecture - the DFlash2 round's host seams

Companion to `ARCHITECTURE_GPU_MTP.md`; a section is cited by its anchor. This document carries how the Metal DFlash2 round
keeps the host off the device's critical path: the verify encoded while the pass runs, and the n-gram model's commit stage
folded into the buffers it follows. The round itself, its two command buffers and the device accept are
`ARCHITECTURE_GPU_MTP.md#mtp-round-one-join`; the replay's own form is `ARCHITECTURE_GPU_MTP_DECODE.md#mtp-replay-form`.

### The verify is encoded while the pass runs {#verify-ahead}

**A round whose verify reads no drafted id on the host before its join holds its pass under every acceptance, because the
picks the host would read are needed only by the walk after the verify.** The pass with its selector is committed with no
wait and the verify's command buffer, embedding its rows off the selector's id list, is encoded and committed behind it on
the same queue (`metal_verify_rows_on_device`), so the device runs pass then verify with no gap and the host's seat prep
and encode overlap the pass. A sampled round qualifies wherever its selection ask lists no draft (`sel_ask_reads_drafts`:
a penalty window over the drafts is the one ask that does); a drawing pass (`MetalDflashWalkSoft`) writes the session's
LCG state into its word buffer before the commit and lands its candidates, q and the stepped state after the verify, in
the round's land (`DflashDeviceSeams.land`), where the host records q - nothing between the pass and the walk reads the
rng, the q record or the picks, so the stream is the waited round's draw for draw. The greedy round of a model whose verify
takes device rows (`metal_verify_takes_device_rows`) always held; `DASLLAMA_METAL_VERIFY_AHEAD` = 1 adds the sampled round,
0 holds the greedy round alone.

**A model whose n-gram side input reads the host's ids - its rows' hash gather - encodes the verify ahead and commits it
late** (`DASLLAMA_METAL_VERIFY_AHEAD` = 2, `metal_verify_takes_device_rows_late`, the hyper-connection trunk with it): the
seat encodes the whole verify while the pass runs, then waits the pass and lands its picks (`df_land_early`, the pass kept
held so the round's land still lands what it owes), gathers the side rows against the window and pokes them into the
verify's side-input buffer, and only then commits. The poke is a shared buffer's contents written before its command
buffer is committed, so the device reads the rows the ids name; the seam left is the commit-to-start of one buffer instead
of the encode of ~1600 dispatches.

### The n-gram model's commit stage rides the buffers it follows {#commit-one}

**An n-gram model's conv-ring copies are encoded into the verify's and the replay's command buffers, because each was a
buffer of its own the host waited.** The verify's land saves the ring and writes the verify's rows into it, and a rejected
round restores the save and writes rows 0..a; the save reads the ring after the verify's conv reads and before its rows
land, which the verify's own tail orders (`encode_verify_step`'s `ple_tail`: the save, then the rows, after the
classifier), and the restore and the committed rows follow the recurrent replay in its encoder (`encode_dn_replay`), whose
buffer then commits unwaited as every other model's replay does - the parked step keeps the save and the conv-input rows
alive until the unpark retires the replay. `DASLLAMA_METAL_COMMIT_ONE`; off, the copies run in buffers of their own, each
waited, and the n-gram model's replay is waited.
