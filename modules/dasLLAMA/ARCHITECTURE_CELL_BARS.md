# dasLLAMA Architecture - the kernel cells' re-derived bars

Companion to `ARCHITECTURE.md`; this document ledgers each kernel cell bar that was re-derived from a measured cause rather
than kept at its first value, with the cause, so a later reader can tell a derived bar from a widened one.

### Kernel cell bars re-derived from a measured cause {#rederived-cell-bars}

Each entry is a cell, its bar, and the cause the bar is derived from. A case body that the three GPU homes share takes
its bar from the home: the rig's `home_bar` reads the home's own of a `HomeBars` (`tests/_compares.das`). A bar sits
beside the reading it rests on - each compare's logged `relative reading`, from `tests/run.das` under
`DAS_LOG_LEVEL=info`: Metal by `--suite kernels --arm metal` on the M5 Max; Vulkan and CUDA by `--area tts` on the
RTX PRO 4500 fleet pod, one home a run.

- **The StyleTTS2 harmonic source's own noise draw** (`NOISE_BARS`, `tests/_discrete_tts_source_cells.das`): the device
  draw against the CPU hash `tts_hash_normal`. Where the cosine lands near zero the relative bar has nothing to scale by,
  and the difference there is the argument's ulp times the amplitude - about 2.8e-6 on these inputs - so every home's bar
  carries an absolute floor of 2e-5, a few times above it. Metal and CUDA compute the log and cosine in exact f32
  against the CPU and hold a relative 1e-5, read at 3.5e-7 and 2.8e-7 (the CUDA home compiles its source stamps without
  NVRTC's fast math, `fastmath = false`). Vulkan holds 5e-5: its driver's log and cosine are approximate forms, read
  at 2.8e-5.
- **The StyleTTS2 harmonic source's mixed signal** (`HAR_BARS`, the same file): 2e-6 relative on every home, read at
  1.8e-7 on Metal, 2.1e-7 on Vulkan and 1.8e-7 on CUDA, whose source stamps compile without NVRTC's fast math, so its
  sin and tanh are the full-precision forms.
- **The StyleTTS2 inverse STFT** (`WAVE_BARS`, the same file): an envelope bar on every home - 1e-4 of each sample's sum
  of term magnitudes, with an absolute floor of 1e-5 - against the double-precision reference. A sample where terms in
  the hundreds cancel carries their f32 rounding, which a bar on the sample's own value would read as an error. The
  control is a log-magnitude moved by 2.0 in one frame, which every sample that frame overlaps must miss.
