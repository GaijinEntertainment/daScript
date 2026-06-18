# Analysis → synth-param seeding (research distillation, 2026-06-18)

Goal: from a recorded one-shot, extract params to SEED the layers CLOSE before the local fitter
refines. (The "analysis" step we kept deferring — Boris's "fitter can't get you there from way
off" is the trigger.) One STFT up front feeds almost everything. Maps onto our ModalVoice
(pitch / spread / n_modes / ring / brightness / damp_skew / transient / transient_rate) + SfxrSound
(wave / base_freq / env A-S-D / punch / lpf / hpf).

## Pipeline (in order)
0. **Preprocess once:** trim leading silence (first sample > −60 dB of peak = onset). STFT: Blackman
   window, N=4096–8192, hop ≤256, zero-pad 2–4× (sharper parabolic interp). Keep magnitude in dB.
   Compute short-time RMS envelope + energy-decay curve EDC (backward-integrated, far less noisy).
1. **Nature gate — pitched vs noise (THIS is the auto-classifier):** spectral flatness
   `SFM = exp(mean(ln P)) / mean(P)` on an early-decay frame. SFM≲0.1–0.2 = pitched (tom/cowbell/
   rimshot) → weight modal; SFM≳0.4–0.5 = noise (snare/hat/cymbal) → weight sfxr noise. (white noise
   plateaus ~0.5–0.6, not 1.0.) Spectral crest `max|X|/Σ|X|` is the complement. **→ can auto-pick the
   metric preset AND the layer mix.**
2. **Multiple impacts (clap/flam/roll):** spectral-flux onset envelope `flux[m]=Σ max(0,|X[m,k]|−|X[m−1,k]|)`
   + 3-condition peak-pick (local-max AND above local-mean+δ AND min-spacing). >1 onset → seed repeat:
   count, median inter-onset-interval = echo time, successive-peak amp ratio = feedback/decay. Then
   analyze only the first impact for the rest.
3. **Pitch = strongest LOW partial** on a frame ~10–30ms AFTER the onset peak (not the transient frame).
   Peak-detect (bin > −40…−50 dB rel max, local max) + **parabolic refine in dB**:
   `p=0.5(α−γ)/(α−2β+γ)`, `f=(k+p)·Fs/N`. Pick strongest peak in the drum's expected low band (cheat-
   sheet). **Octave guard:** score candidates {f, f/2, 2f} by harmonic-comb / two-way-mismatch, take best.
   → modal pitch + sfxr base_freq.
4. **Partials → spread / n_modes / brightness:** top M (4–8) refined peaks, sorted. Geometric spread
   via log-linear fit: regress `ln(f_k)` on `(k−1)`, `spread=exp(slope)` (≈ geomean of f_{k+1}/f_k).
   Harmonic vs inharmonic: ratios near integers = harmonic; near membrane set
   {1, 1.59, 2.14, 2.30, 2.65, 2.92, 3.16, ...} = inharmonic membrane. brightness = mean(amp of partials
   above f1)/amp(f1), or spectral centroid / f1.
5. **Decay → ring / damp_skew:** fit a line to the EDC in dB (start −5 dB below peak, fit to −25/−35 dB,
   extrapolate): `T60 = 60/|slope dB·s⁻¹|`. Per-band T60 (e.g. <200/200–800/800–3k/3k–12k Hz); HF decays
   faster → `damp_skew = T60_high/T60_low` (or fit `T60(f)=T60_ref·(f/f_ref)^−s`, s=skew). Robust form:
   nonlinear LS `E(t)=A·e^(−αt)+n0` to not let the noise floor flatten the slope.
6. **Attack → transient amount/rate, punch:** log-attack-time (MPEG-7 / Peeters weakest-effort): t_start
   = envelope first > ~2% max, t_stop = first reach max (or ~80–90%). attack_time=t_stop−t_start (short→
   fast/sharp→high transient_rate, short sfxr attack). transient AMOUNT = crest factor (peak/RMS) of the
   first ~20ms. punch = early-overshoot ratio (peak energy first ~10ms vs steady). temporal centroid
   `Σt·E/ΣE` = front-loaded check.
7. **Tonal/noise split → modal vs noise gain:** (A cheap) noise_fraction≈SFM. (B best, = our 2-layer
   model) SMS sines+residual: resynth detected sines, subtract → residual; modal_gain=RMS(sines),
   noise_gain=RMS(residual); residual spectral envelope → sfxr lpf (centroid) / hpf (low rolloff).
   (C easy) median-filter HPSS: H=median over time, P=median over freq, Wiener masks (exp 2); P=noise,
   H=tonal.
8. **Clamp every value** against the cheat-sheet, then hand to the local optimizer.

## Per-drum cheat-sheet (priors / sanity clamps)
| drum | fundamental | harm/inharm | tonal:noise | decay |
|---|---|---|---|---|
| kick | 40–100 Hz (+2–5k click) | near-harmonic low | tonal+short click | short-med, LF longest |
| snare | body 150–250 Hz | inharm + wire noise | **noise-dominated** | med body + dense HF buzz |
| toms | 80–140 Hz (up to 250) | **inharmonic membrane** | strongly pitched | med-long ring |
| hh closed | none, 3–12 kHz | inharm/noise | noise | very short |
| hh open | none, 1–15 kHz | inharm/noise | noise | long sizzle |
| ride | ping 300–600 Hz + bell | bell pitched, wash inharm | tonal ping + noisy wash | long |
| crash | none, 300 Hz–20 kHz | inharmonic chaotic | noise-like (high SFM) | long, HF-first decay |
| clap | none, 1–3 kHz | noise | noise + **3–5 impacts** | short cluster + tail |
| cowbell | 500–800 Hz + strong 2nd | **inharm but pitched** (2 modes) | tonal | med metallic ring |
| rimshot | click + body 150–800 Hz | inharm pitched click | tonal click + noise | very short sharp |
| tambourine | none, 5–15 kHz | inharm/noise | noise + **jingle impacts** | short jingle cluster |

Ideal circular-membrane ratios (inharmonic target): 1, 1.594, 2.136, 2.296, 2.653, 2.918, 3.156, 3.501.

Sources: SMS-tools (MTG), Smith CCRMA (peak interp / F0 from peaks), Maher&Beauchamp TWM 1994,
Peeters CUIDADO 2004 + Timbre Toolbox 2011 (LAT), Fitzgerald HPSS DAFx 2010, Karjalainen modal-decay
JAES 2002, JASA fast-inharmonicity 2007, MPEG-7 audio descriptors, librosa onset docs.
