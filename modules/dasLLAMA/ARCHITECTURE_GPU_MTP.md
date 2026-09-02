# dasLLAMA Architecture - the Metal speculative round

Companion to `ARCHITECTURE_GPU.md`; section numbers are `ARCHITECTURE.md`'s. This document
carries sections 2.28-2.30: the speculative round on Metal, the box knob that sets the depth a
round drafts, and the argument-alignment contract a kernel declares on its `[metal_dispatch]` -
the contract the batch driver's fixed-B mul_mv forms carry. The GPU backend role table these
sections build on, the assistant-drafter driver's role row included, stays in
`ARCHITECTURE_GPU.md` sec.1.5.

### 2.28 The speculative round on Metal {#mtp-round-one-join}

**The speculative round on Metal is one queue, one join.** A round drafts k tokens and verifies them
as k+1 same-slab rows of the batch driver. The gemma assistant drafter encodes its k steps into ONE
command buffer committed without a wait: draft i embeds `bvtok[i]` (the seed at 0), its argmax lands
in `bvtok[i+1]` on the device, and its post_projection is copied on the device into the next step's
input; the batch driver then takes that token buffer (`set_batch_same_slab_tokens`) and dequantizes
the rows' embeddings on its own encoder instead of the workspace's CPU rows, lands every row's
argmax (`enc_argmax_rows`) with the logits, and the host reads tokens and winners only after the
verify's join - queue order completes the chain first. The drafter's h input is the target's
post-output_norm hidden, carried in `s.mtp_h` by the decode on head-less models
(`set_metal_mtp_carry_hidden`); the pre-norm residual is a lever that measured worse. The two-row
verify's GPU cost is ~1.2x a one-row step on this model (the rows' weight stream), the physics the
round cannot recover; everything the host did between the drafts and the verify was.

### 2.29 The depth a round drafts {#mtp-depth-knob}

**The depth a round drafts is a box knob per round kind, not a controller.** `get_mtp_depth()`
serves an explicit setting (`set_mtp_depth`, the `--mtp-depth` flag) when one was made, else the
box profile's `runtime.mtp_depth_assistant` or `runtime.mtp_depth_nextn` by the `MtpRoundKind` the
assistant drafter's attach and detach select. The per-position acceptance curve has the same shape
on every task (p2/p1 about 0.7), so nothing a controller could observe changes the depth ranking;
what does is the box's k-row verify cost - the M5's two-row gemma verify is ~1.2x a step and depth 2
ties depth 1, the M4 Pro's is 1.5x and depth 2 loses - so the tuner mints the assistant knob as a
serving race of depth 1 against depth 2 on the SpecBench chat corpus with a gemma-4 vehicle and its
`mtp-` head (depth 2 must beat by 2% - its downside is asymmetric and a tie is not worth the longer
round; a synthetic 32-token prompt is not the site shape, the drafter accepts less of incoherent
text; no vehicle means depth 1). The NextN knob defaults to 1, the depth every
NextN carrier measured best.

### 2.30 The kernel argument-alignment contract {#metal-dispatch-requires}

**A kernel's argument-alignment contract is declared on its `[metal_dispatch]` and enforced at
every dispatch.** `requires = "lhs % N, ..."` (lhs a `params=` name or a kargs field) makes the
generated builder check each item before it binds and call `metal_requires_failed` on a miss -
a panic naming the builder and the broken contract, or the test hook installed with
`set_metal_requires_hook`. The check is one integer modulo per dispatch, so the kernel keeps its
tail-guard-free main loop and the DRIVER does the shape routing: the fixed-B mul_mv forms stripe
K in 256 (B2) / 128 (B4) element chunks, and the batched decode driver gates each mv site on its
own K (`mv_kdim`, `mv_wo`, `mv_w2`), falling to the tail-exact GEMV form where the alignment
fails (gemma-4-26B-A4B's dense hidden 2112 on the w2 site). Every `[metal_dispatch]` GEMM form
carries its grid divisors the same way - the production mul_mm `mp % 32, d % 64`, the 32- and
64-wide GEMM-B forms and their tensor twins `ka.ndim % 32|64` with the q8 block `ka.kdim % 32`,
the split-K pair `d % 32` - so a grid that would have truncated silently now names the site.
A contract on a value that reaches the builder only as a bound uniform BUFFER (the mul_mm's K)
stays with the caller: the dispatch never pays a readback.
