# LAWS - append-only intent provenance for this folder's rule documents

- **2026-08-25** (`REVIEW.md`): Boris approved landing the M5 arc's measured emitter laws as
  rules and gates - every matmul2d descriptor sets relaxed_precision=true (RP=false keeps
  the op off the tensor-unit fast path, measured 2-3x), enforced by a REVIEW.das cell over
  the emitter's descriptor sites.
