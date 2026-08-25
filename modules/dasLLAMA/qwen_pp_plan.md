# qwen img:pp remainder - the two levers (ACTIVE)

Successor of the qwen3vl arc's slice M/J (plan archived: `history/dasLLAMA/qwen3vl_plan.md`)
and the reduction-split decode GEMV arc (merged). Scope = the two ruled levers:

1. **4B lever** - the image-turn residual (~25 ms at the bench shape; text at parity).
   Candidate mechanism: the wide quantum's staging/eval path (upload, restride, tracked-pool
   dependency pass) - attribution OPEN on current master, must re-pin first.
2. **8B lever** - the small-M prefill GEMM: text pp at p512 is parity, p341 is -6.2%.
   The mulmm at dim-4096, M~352-pad shapes; tile/tune territory, image-blind.

30B (-4%) rides whatever falls out. Board context (rig, Metal, image cells): 4B -7%,
8B -7%, 30B -4% vs the ref pairs at equal turn shape (321 rows both engines).

## Protocol

Released rig exe (`performance/_rig/dasllama-bench.app/.../dasllama-bench.exe`), quiet box,
r=3 t=8, `--ngl 99`; models + ref binary paths per `METHODOLOGY.md`. Image cells:
`--image` + `--image-mmproj` + `--image-think` on the coco fixture. Text-M control: `-p 341`
(same ceil-32 pad = 352 as the image turn's npos 321). Every in-process/stage figure below
("the probe") is `harness/image_turn_probe.das` under `-jit` (best-of-3 walls per arm;
knockout arms = `DASLLAMA_METAL_PREFILL_SKIP`; model = `DASLLAMA_PROBE_MODEL`, default the
8B) - direction-grade by contract, never board rows. Figures quoted as before/after across
commits are two probe runs at the two tips, not an in-process A/B; the in-process
tail-on/off compare lives in the parity arm (token-exact - bit-exactness is impossible by
design across the f16-tile and f32-dot GEMM forms).

## Predictions (BEFORE the phase-0 runs)

- **P1** 8B text p341 vs ref: das -6+/-2% (the 8B image gap IS the text gap at that M).
  **SCORED: WRONG - artifact.** First-process read 524.7 (-5.8%) reproduced the prediction,
  but the warm re-run prices 568.3 +/- 0.3 = **+2.1% AHEAD** (ref 556.8); the deficit was the
  cold-first-process page tax (the ledgered trap, struck again - and it back-explains the
  slice-M "-6.2% p341" control). There is NO 8B small-M text GEMM gap; the 8B lever as
  planned (phase 2, dim-4096 mulmm) is DEAD.
- **P2** 4B text p341 vs ref: parity +/-3%. **SCORED: CORRECT** - 1037.9 +/- 0.8 vs ref 1010.6
  = +2.7% (first-process run, no cold tax visible; 4B pages were still warm).
- **P3** 4B img:pp vs its own text control at equal pad: das image ~20-30 ms slower
  (the image-specific residual survives on master). **SCORED: PARTIAL** - bench context:
  4B +12.7 ms, 8B +16.1 ms over own text at equal pad; but in ONE process (the probe) the
  image-shaped quantum costs only ~+6 ms - the rest is the post-encode context residue.
- **P4** the GEMM-family Delta carries the image tax. **SCORED: WRONG** - the probe knockouts
  put the whole in-process +6 ms in the ATTENTION family (skip-attn deletes it; skip-gemm
  leaves it); ds-wide staging is FREE (wide == narrow to 0.1 ms - the slice-J borrow verdict
  reconfirmed).
- **P5** 8B small-M GEMM rate deficit. **SCORED: WRONG twice** - the p341 text "gap" was the
  cold-page artifact, and the true structure is the ceil-32 M-tile STAIRCASE: p320 = 551 ms
  (10 tile-rows), p321 = 599 ms (11 tile-rows), p352 = 600 ms (flat). One tile-row = ~48 ms
  on 8B; the image turn (npos=321) bills a whole tile-row for ONE real row.

## The re-derived gap (warm, current master, vs fresh ref mints)

| term | 8B | 4B |
|---|---|---|
| dead last-M-tile (31 pad rows) | ~42 ms | ~23 ms |
| span-attention tax (probe, in-process) | ~6 ms | (unprobed, expect ~4) |
| post-encode context residue | ~10 ms | ~7 ms |
| das image wall vs ref image wall | 615.5 vs 568.6 (-7.6%) | 340.7 vs ~318 (-7%) |

Ref image mints (fresh, warm): the 8B ref image turn's prefill 568.6 ms / 321 rows
(564.6 tok/s), encode 500 ms (das encode 255.8 - 2x ahead); ref decode 32.2 tok/s (das
38.7 - +20%).

## The lever (proposed, not yet ruled)

**GEMV-tail dispatch** - zero new kernels: inside `enc_gemm_mm`, when `npos % 32` lands in
[1, 8], dispatch `floor(npos/32)` mul_mm tile-rows + the remainder rows through the existing
fixed-B `mv_b2/b4` GEMV family at x/y row offsets (built for offsets - the fused-row
segment sites). Cost model: the tail tile-row is compute-bound (~48 ms on 8B), the GEMV
tail streams the weights once (~12 ms) -> net ~36 ms (8B) / ~23 ms (4B). Projection with all
three terms closed: 8B ~563 vs 568.6 (das leads), 4B ~315 vs ~318 (das leads). Also buys
every text pp at npos%32 in [1,8]. Follow-ups: the kq mul_mm twins (kq mv_b exists), the
MoE prefill (30B -4%), the span-attn +6 ms, the ~10 ms post-encode residue.

## Ladder

- [x] Phase 0: text-M controls + fresh image cells + fresh ref mints (above).
- [x] Attribution: probe (3 workloads x 5 skip arms, one process) + the M staircase.
- [x] RULING: the GEMV-tail dispatch lever (approved).
- [x] Implemented: enc_gemm_mm peels npos % 32 in [1,8] onto the mv family (r == 1 rides the
      reduction-split GEMV - the mv forms idle half their lanes on a lone row, measured: the
      mv-only tail bought 24 ms, the r==1 reroute 8.6 more). Per-form reduction alignment
      gates the peel - the b4 stripe reads whole 128-quant rounds, so kdim % 128 != 0 keeps
      the padded tile (dims like 576/896/1152 misread otherwise - device-probed), and r >= 2
      always takes the b4 form (b2's stripe needs % 256). DASLLAMA_METAL_MM_TAIL knob +
      setter + engage counters; DASLLAMA_VERSION 9; the mv_b2 store now gates on nrows -
      defensive: at nrows == 1 it phantom-wrote its second row, which the batch rail's two
      sizing paths make either an unread pad row or an off-the-buffer write.
      Probe (8B, one process, npos=321): tokens 599.2 -> 566.8 ms, wide-span 605.3 -> 572.6.
- [x] Gates: mm-tail parity arm (residues 257/321%32==1 and 261/%32==5, token-exact, tail-off
      A/B, engage counters; y-offset poison control REDS it), kernels suite 7/7.
- [x] base + batch gates green; rig re-released; the board re-measured record-grade
      (released exe, r=3, quiet box, fresh upstream pairs): 8B img:pp 522 -> 552 tok/s
      (-7.5% -> -2.2% vs 564.6), 4B img:pp 942 -> 986 (-2.7% vs 1013.2), 8B text p321
      534.5 -> 566.7 (+7.8% ahead of 525.9), p341/p512 unchanged, 30B Omni 783 (rides).
      PERF_LEDGER entry in.
- [x] Followup 1 (span-attn tax) split and fixed: the AV kernels dropped the causal walk
      bound wholesale when the span was armed (`jlimit = np32`); now tile-conditional
      (`max(uend, causal reach)` only where the tile holds span rows - a pure shrink over
      softmax-zeroed columns, bit-neutral). Probe: tax1 3.3 -> 0.58 ms (the constant
      overhead deleted - small-span turns win up to ~3 ms); tax300 unchanged within noise -
      at the real 300-row span the extra walk was nearly free, the remaining ~6 ms is
      INTRINSIC non-causal pairs + softmax length. The image cell does not move from this.
      GATE LESSON: the span-fused f16 fused-vs-splice witness (bit-identity bar 1e-4) reds
      under the GEMV tail - a splice chunk with npos % 32 in the tail window serves its
      remainder rows on f32-dot GEMVs while the fused eval stays f16-tile; the witness now
      pins the tail OFF for both legs (it isolates the MASK; the tail has its own arm). The
      tail commit's gate scope missed this - bit-identity witnesses belong in the
      change-affects set of ANY GEMM-numerics change.
- [ ] Followups: post-encode residue (~10 ms: ~6.7 GPU cache aftermath + ~3.8 host slack;
      lever = encode-ahead overlap, shared with the decode submission-overhead arc), kq
      mul_mm + MoE prefill tails (the mv_kq twins exist), r in [9,15] via a third mv pass.
      Review-round harvest: (a) the batch-DECODE mv rail carries the same unguarded stripe
      widths with no alignment clause (pre-existing - B in [2,8] on a dim % 128 != 0 model
      misreads; own arc, failing test first); (b) u_dn_qd2 is built from qd, not qd_l - a
      latent bn != d exception if a hetero q-gated arch ever lands (unreachable today);
      (c) the b2/b4 y binding carries no @span, so every peeled site pays a conservative
      whole-buffer barrier - correctness-neutral, confounds per-r timing comparisons.
