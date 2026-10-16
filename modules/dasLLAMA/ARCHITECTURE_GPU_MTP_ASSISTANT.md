# dasLLAMA Architecture - the gemma-4 assistant drafter

Companion to `ARCHITECTURE_GPU_MTP.md`; a section is cited by its anchor. This document carries the gemma-4 assistant
drafter on both GPU homes: one drafter step, the sidecar and the attach, and the speculative rounds of a scheduler tick.
The Metal round's queue order and the accept walk it shares are `ARCHITECTURE_GPU_MTP.md#mtp-round-one-join`, the batch
driver's same-slab rows `ARCHITECTURE_GPU_MTP.md#batch-same-slab`; the Vulkan driver's draft command and verify are
`ARCHITECTURE_GPU_VULKAN_MTP.md#resident-assistant-drafter`. The engine charter of the backend-free file
(`dasllama_mtp_gemma.das`) is in `ARCHITECTURE_ENGINE.md`.

### One assistant-drafter step {#assistant-drafter-step}

**The drafter is a Q-only head on the target's cache at ONE frozen anchor.** A step takes
`(tok, h)`: it concatenates `embed(tok) * sqrt(tdim)` with the target hidden `h` into a `2 x tdim`
row, projects it down with `pre_projection`, runs the drafter's layers, applies `output_norm`, and
reads the normed hidden twice - through the token table for logits (the draft is their argmax) and
through `post_projection` for the `tdim`-wide hidden the NEXT step consumes in `h`'s slot.

A layer is rms -> Q -> per-head q_norm -> rope at the anchor -> attention -> wo -> post-norm ->
residual, then rms -> gate/up -> geglu -> down -> post-norm -> (residual + branch) * layer_scale.
The drafter owns no K/V projection: layer `l` attends the TARGET's layer `kv_layer[l]` rows
`[0, anchor)`, GQA-broadcast over that layer's kv heads at scale 1.0, and a sliding layer masks
`anchor - key >= window` (low bound `anchor - window + 1`; the GPU attention passes, whose count is
the anchor, take `window - 1` keys). The anchor is the seed's own position - the target's `n_past` -
so the seed row itself is unfed and every draft of a round ropes at the same position. Each attention
class ropes on its own terms: the full class over `hs` at the target's `rope_freq_base` with the
p-RoPE frequency table, the sliding class over its own head size at `rope_freq_base_swa` with none.
The seed's row comes off the trunk's own token table in whatever format it holds (the device's
table gather), so a K-quant table drafts as a Q8_0 one does. The attention reads every mirror codec
the decode does: a tq4 mirror's query rotates into the stored rows' basis before the pass and the
pass's output rotates back after it.

### The sidecar and the attach {#assistant-drafter-attach}

**The sidecar beside a trunk is found by name, loads Q8_0 alone, and attaches on the GPU home that serves the trunk.**
The resolver (`gemma_drafter_sidecar`, registered backend-free so every build finds it) takes the trunk's file stem before
its trailing quant token (`-Q<digit>` or `-IQ<digit>`) and looks for `mtp-<stem>-Q8_0.gguf`, then the same with an unsloth
`-UD` tag dropped from the stem - the upstream head is named after the plain trunk - then the exact-name `mtp-<file>`
sibling. The head's quant is chosen apart from the trunk's, and the loader takes Q8_0 tensors alone: a sidecar in any
other type refuses naming the tensor and its type, and the trunk serves plain. Every attach asks the trunk check
(`gemma_drafter_target_why`): a per-layer-embedding trunk (the E-series) refuses first - its verify rows need the side
input the draft ids index, and no sidecar exists for one - then the hidden width against the drafter's `tdim`, then each
drafter layer's capture layer (the trunk's last layer of its attention class) against the drafter's head size, kv heads
and class. Each GPU home registers its seams (`MtpAssistantSeams`, `register_mtp_assistant`) and refuses a trunk it does
not serve - Metal a trunk that is not the metal-blob flavor or whose table the device does not gather, Vulkan one the
resident driver does not serve whole - before it reads the sidecar; the facade attaches on the home that takes the trunk
and logs every home's refusal where none does. One drafter attaches at a time, and the attach selects the assistant
round kind, so `get_mtp_depth` serves `runtime.mtp_depth_assistant`.

### The assistant rounds of a tick {#assistant-drafter-rounds}

**A tick's speculating streams draft at the round's depth on the device and verify together in one pass.** The
scheduler's tick hands every speculating stream to the attached home's rounds (`mtp_spec_eval_batch` ->
`MtpAssistantSeams.rounds`) at `get_mtp_depth`, under each session's cap (`Session.mtp_depth_cap`: depth 1 for a stream
whose reasoning budget can still force a close, since a draft past the close would be evaled and not emitted), and emits
each stream's accepted drafts in order before it samples the next token. The round is one protocol on both homes
(`mtp_assistant_rounds`, `dasllama_mtp_gemma.das`), each home supplying its seams as one `MtpAssistantHooks`: the bind, the
draft chains, the verify, a verify row's pick, the device's drafts read back, the commit; each home keeps its tallies as one
`MtpAssistantStats`. A stream the home does not bind - no carried hidden at its position, a round past its context, on
Metal a mirror short of the round's rows, on Vulkan no region left - and every warm stream of a failed verify steps plain
after the verify, so a plain step's binding takes no region the round's streams held. The warm streams each
draft their chain at their own anchor from their own token and carried hidden; on Metal every chain encodes into one
command buffer, its inputs written into a slot a stream, the token rows a stream's group in one token list. One verify
then carries every warm stream's [token, drafts]: on Metal the batch driver's same-slab step in groups
(`verify_batch_groups`, a group a stream's consecutive rows over its own mirror), on Vulkan the N-row token command
(`ARCHITECTURE_GPU_VULKAN_MTP.md#resident-assistant-drafter`). Each stream's accept walk and host commit are the shared
pair (`mtp_accept_walk`, `mtp_round_commit_host`); its carry becomes the verify's hidden row at the accepted count.
