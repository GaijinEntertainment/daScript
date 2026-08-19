# E-series decoder bring-up — everything about gemma-4 E2B works in every mode

## Direction

The gemma-4 E-series decoders (E2B/E4B, `has_ple`) have two pre-existing holes, both
logistics rather than kernels (discussion 2026-08-19). Close them, then prove the whole
model — decode, prefill, chat, the vision and audio splices — across the serving modes
(fp32 / q8 / q4) on CPU and on the Metal tier.

## The two holes

1. **CPU: the PLE rail is q8-only.** The per-layer token table (`per_layer_token_embd`,
   gather-only, 2.3 GB Q8 / 9.4 GB expanded on E2B) is reserved on the quant plane and read
   by `ple_gather_row`, which routes on the table's DISK format (q8/k4/k5/k6/q40 arms all
   exist). But the loader routes it through the generic big-weight helper, so the SERVING
   mode drags it into the wrong plane: fp32 expands it into `wblob` (9.4 GB) while the
   reader indexes the empty `qblob` — an unguarded OOB on the first forward; q4 got a named
   guard instead of a rail. Fix: pin the table's load to its disk-format plane in every
   serving mode. Gather-time dequant is bit-identical to an eager expand, so the fp32 rail
   stays exact. The q4 guard (loader ~1742) comes out — RULED: q4 unlocks.

2. **Metal: one uniformity check blocks E2B.** `feed_forward_length` is per-layer on E2B
   (35 layers, widths {6144, 12288} — probe-verified from the gguf). Both drivers hoist
   `hidden = layer_hidden(0)` once and decline `layers` when any layer differs. Everything
   else E2B needs is in both implemented-needs sets already (ple, v_norm, attn_scale,
   sliding, pre/post norms, geglu, out_scale) and `kv_share_ok` passes its 20 shared-KV
   layers. Fix: per-layer hidden through the dispatch loops (per-layer uniform + grid),
   scratch sized to the max width, declines relaxed to per-layer alignment (%64 — both
   widths pass). No new kernels.

## Out of scope (ledgered)

- Batched PLE on the GPU (the multi-stream Metal arm) — `followup_general.md` #40,
  long-term differentiation (llama.cpp has no batched-PLE path either).
- The whole-model resident recorder rejects shared-KV/sliding models by design — E-series
  rides the rails tier (same as E4B today).
- E4B vision (mmproj not on this box); Vulkan.

## Slices

- **A. The CPU PLE rail**: table pinned to its disk-format plane under fp32/q4; the q4
  guard deleted (it had no fixture); `ple_gather_row` untouched; a go-live tripwire
  (`ple_check_table`) at both carrier seams — gguf load and image parse — so a stale
  pre-rail image fails AT LOAD, named, instead of OOB mid-forward. Tests
  (`tests/test_ple_modes.das`): an fp32 E2B forward (greedy reply coherent; PLE gather rows
  bit-match a direct dequant probe; plane-length asserts prove no table expansion — exact
  where RSS is noisy), the kq-table pin under fp32 (Q4_K_M), a q4 forward smoke, negative
  controls (mutate the table routing → red).
- **B. The Metal hetero-hidden fix**: prefill + single-stream decode take per-layer hidden;
  support-matrix cells move E2B from `layers` to served (and pin the reasons that remain);
  `harness/parity.das --ngl` GPU-vs-CPU on E2B Q8_0 AND E2B Q4_K_M (the kq arm), per the
  REVIEW_GPU driver rule.
- **C. The matrix sweep**: one pass over the model's surface — decode/prefill/chat ×
  (fp32, q8, q4) on CPU; +AMX row; Metal q8 decode+prefill; the vision and audio splices on
  each tier they serve. Record-grade numbers for changed cells only (the released exe).
  Anything red stops the arc (no known-interim write-offs).

## Predictions (registered before any code)

- P1 (fp32 rail): after slice A, fp32 E2B runs end to end first try; the load's RSS is
  ≈ the fp32 weight expand WITHOUT the 9.4 GB table; gather rows bit-match the dequant
  probe (exactness is by construction — a mismatch means a routing bug, not noise).
- P2 (q4): the gate/proj requant path works on the first E-series run with zero code
  changes beyond the guard removal — the reading-based claim survives its first execution.
- P3 (Metal): after slice B the first red, if any, is in scratch sizing or a reused
  uniform (the `hidden` hoist has ~5 consumers per driver), NOT in a kernel; parity lands
  inside the established GPU envelope on both q8 and kq arms.
- P4 (Metal perf): E2B Metal decode lands 1.4–2.2× the CPU q8 decode (memory-bound:
  ~380 vs ~200 GB/s effective on the M1 Max), i.e. ≈ 60–90 tok/s vs the CPU's 42.
