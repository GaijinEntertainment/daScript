# Fit-metric design (research distillation + what's implemented, 2026-06-18)

## The bug (confirmed by research)
The old metric per-spectrogram-normalized to 0 dB + hard −80 dB floor, then averaged |dB diff|. A
sustained broadband **noise wash** cheaply half-fills the floored grid → low average distance; and a
128-pt RMS envelope over a fixed 2 s window (~16 ms/pt, peak-normalized) is dominated by the silent
tail, so a 150 ms transient and a 2 s sustain read 97% identical. **Weights can't fix this** — you need
terms that explicitly measure *where energy sits in time and how sparse it is*.

## Implemented (sfx_analysis.das) — composite fit, percussive default
`fit(ref, mix, rate, w = FitWeights())` → `FitScore{spectral, shape, attack, overall}`:
- **spectral** (`spectral_dist`, weight 0.50): peak-normalize each signal by ONE global scalar (NOT
  per-cell/per-spectrogram), time-resolved log-magnitude spectrogram, clamp cells to [−80,0] dB, mean
  |diff|. No per-spectrogram renorm → spurious energy where the ref is quiet is penalized. This alone
  closes most of the hole.
- **shape** (weight 0.35) — the anti-wash wall (Timbre Toolbox / MPEG-7): from the raw RMS envelope,
  **temporal centroid** (energy center-of-mass, s), **effective duration** (>40% max, s), **crest
  factor** (peak/RMS). log-ratio distances, `squash(d,0.7)=1−e^(−d/0.7)`, averaged. A wash has a LATE
  centroid + LONG duration + LOW crest and cannot fake all three.
- **attack** (weight 0.15): log-attack-time (2%→80% of peak envelope), `squash(d,0.5)`.
- `overall = Σ wᵢ·scoreᵢ`. Squash = smooth/bounded → gradient everywhere (good for the hill-climber).

**Proven:** synthetic wash vs transient → overall 0.22 (shape 0.05); identical → 1.0 (test_analysis.das
regression). Real snare estimate: **0.875 → 0.50** with the honest breakdown timbre 78 / shape 25 /
attack 13 — i.e. "timbre's close, time-structure is wrong," which is exactly the snare's problem.

## Nature-based presets (Boris: multiple metrics → top menu) — ONE engine, weight profiles
`FitWeights` profile per sound nature; default = percussive. Planned profiles:
- **Percussive** (drums): spectral .50 / shape .35 / attack .15 (current default)
- **Sustained/pad**: spectral .75 / shape .20 / attack .05
- **Tonal/melodic**: spectral .65 / shape .20 / attack .15 (+ a pitch term when added)
Auto-classify via **spectral flatness** (see seeding research) → could auto-pick the preset.
**TODO:** add the preset selector to the top menu + thread the chosen `FitWeights` through the
optimizer's `eval_fit` (currently uses the percussive default).

## Deferred / could strengthen (from research, not yet implemented)
- True multi-resolution STFT (6 FFT sizes 2048…64, spectral-convergence + log-mag L1) — current
  spectral term is single-resolution time-resolved; the small windows would give the attack more
  spectral footprint.
- Attack-windowed spectral (0–50 ms, onset-aligned), log-mel(64) L1, MFCC anchor, spectral-flatness
  trajectory, decay-slope term. The current 3 shape terms already forbid the wash; these add polish.
- A pitch/harmonicity term for the Tonal preset.

Sources: Parallel WaveGAN (MR-STFT), DDSP losses.py, auraloss, Masuda&Saito ISMIR'21 (spectral-alone
ineffective), Schwär&Müller 2023 (MSS gradient pitfalls), Peeters Timbre Toolbox JASA'11 (temporal
centroid / effective duration / LAT / crest), MPEG-7 audio descriptors.
