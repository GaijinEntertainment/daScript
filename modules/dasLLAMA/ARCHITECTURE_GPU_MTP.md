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

### 2.31 The K-quant small-batch form is a per-box crown {#kq-rows-crown}

**A verify row on a K-quant plane costs what the box says, not what the kernel comment says.**
At two to eight rows `enc_kq_site_b` dispatches either the small-batch twin (one weight pass per
row group) or B single-row GEMV passes. The K-quant GEMV is ALU-bound, so sharing the weight pass
pays only where ALU is plentiful: on the M5 Max the two-row twin costs 0.52-0.89 of two passes,
on the M4 Pro 1.35-1.5 for k4 while k6's twin still wins. The mint races the production wrappers
per format (`race_kq_rows`, twin against two passes) and crowns `kq_rows_<fmt>` where the passes
win; `metal_decode_init` reads the crowns once into `g_kq_rows_crowned` and the dispatcher takes
the passes exactly there. An unraced box keeps the twin. The M4 Pro's Qwen3.8-27B round paid
2.06x a step for its two verify rows under the twin - the reason its depth-1 speculation lost.

The k4 twin itself has two forms, and the form is the box's too. The ext twin gives a thread one
weight row and two x columns; the two-row register tile (`MetalKqMvB2K4R2`) gives it two weight
rows and both columns, so every x float4 load feeds two rows - fewer loads per FMA, twice the
live registers. The tile wins where ALU is short (M4 Pro: 0.74-0.96 of two passes against the ext
twin's 1.36-1.49) and loses where it is not (M5 Max: 0.63-1.07 against 0.52-0.89). The mint races
the two production twins first (`race_kq_k4_form`) and crowns `kq_mvb2_k4_r2` where the tile
wins; `enc_kq_mvb` takes the tile at two rows exactly there, and the k4 rows race that follows
meets the box's twin form. An unraced box keeps the ext twin.
