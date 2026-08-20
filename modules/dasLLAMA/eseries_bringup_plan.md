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
- **B. The Metal hetero-hidden fix — DONE**: prefill + single-stream decode bind per-layer
  hidden (two uniform slots, `ffn_second_hidden` ≤ 2 widths, scratch to the max; batch/verify
  keep the layer-0 hoist behind their uniformity decline). Support-matrix rows: E2B Q8_0 + the
  Q4_K_M kq carrier (small tier, always on) + the E4B control (PARITY_FULL). `parity.das --ngl`
  E2B Q8_0 = 40/40 token-exact vs CPU; the kq leg coherent (freeform near-tie divergence — the
  forced-feed cells are that carrier's parity instrument, per the tests doctrine).
  Landed along the way: `ple_gather_row` is now metal-blob-aware (the blob transform compacts
  k4/k5 scale strips to 16B and splits k6 — the planar strides read garbage; embed_row's cls_kq
  arm was already blob-aware, the PLE gather predated the pattern), and the row gained a
  single-stream forced-step logits cell — the B=2 verify-rail cont cell is blind to the
  single-stream decode loop, and a short counting continuation survives a width mis-bind
  token-for-token. Negative controls: planar-stride poison → index panic; decode width poison →
  forced-step cell 39/34 vs bar 8; prefill width poison → cont cell 50/51 vs bar 8.
- **C. The matrix sweep — DONE**: CPU fp32/q8/q4 chat turns + gather bit-match (slice A's
  gates); Metal q8 decode+prefill (slice B's rows + parity); vision splice green
  (`test_vision_chat` E2B caption through the gemma4v tower); audio splice green (the gemma4a
  oracle cells in the model-free pass). Board: E2B is NOT an official model (E4B is the
  E-series board row, uniform widths, unchanged) — no board cell changed, nothing minted.
  P4 settled by hand on the released exe (`dasllama-bench -m gemma-4-E2B-it-Q8_0.gguf
  -p 512 -n 128 -r 5 -t 8 --ngl 99`, fresh paranoid tune sha=99524cbf2): tg128
  95.6 ± 1.0 tok/s, pp512 1948 ± 5 tok/s — ~2.3× the CPU's ~42, just above the predicted
  1.4–2.2× band. Landed along the way: the image-rail PLE tripwire moved to the after-borrows
  seam in `load_model_image` (`image_post_load` runs while every plane is still empty — the
  first released-exe run false-panicked on its own fresh mint), with the `gemma4e` image-suite
  arm as its gate. Open question for Boris: should E2B become an official board model?

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
