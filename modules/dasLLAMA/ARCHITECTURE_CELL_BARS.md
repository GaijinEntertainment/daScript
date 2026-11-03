# dasLLAMA Architecture - the kernel cells' re-derived bars

Companion to `ARCHITECTURE.md`; this document ledgers each kernel cell bar that was re-derived from a measured cause rather
than kept at its first value, with the cause, so a later reader can tell a derived bar from a widened one.

### Kernel cell bars re-derived from a measured cause {#rederived-cell-bars}

Each entry is a cell, its bar, and the cause the bar is derived from.

- **The StyleTTS2 harmonic source's own noise draw** (`st2_source_gate`, `tests/test_metal_prefill_kernels.das`): the
  device draw against the CPU hash `tts_hash_normal` at a relative bar of 1e-5 with an absolute floor of 2e-5. The kernel
  computes the draw's log and cosine under Metal's default fast math, the CPU in exact f32. Where the cosine lands near
  zero the relative bar has nothing to scale by, and the difference there is the argument's ulp times the amplitude - about
  2.8e-6 on these inputs. The floor sits a few times above it, so the draw still fails on any value off by a step of its
  own.
- **The StyleTTS2 inverse STFT** (`st2_istft_gate`, the same file): an envelope bar - 1e-4 of each sample's sum of term
  magnitudes, with an absolute floor of 1e-5 - against the double-precision reference. A sample where terms in the hundreds
  cancel carries their f32 rounding, which a bar on the sample's own value would read as an error. The control is a
  log-magnitude moved by 2.0 in one frame, which every sample that frame overlaps must miss.
