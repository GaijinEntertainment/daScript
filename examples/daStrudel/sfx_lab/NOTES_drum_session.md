# SFX Lab — drum session notes (Claude, 2026-06-18)

First-pass estimates for all 11 remaining GM drums. Each `sounds/<name>.sfx` was hand-seeded
(informed by drum-tuning knowledge + the snare's spectral recon) then tuned by the **same
annealed-ES fitter as the live app** (verbatim copy in `batch_drums.das`, fixed seed → results
match clicking "optimize all" in-app; confirmed: batch sd 87% == live sd 0.8747).

## Fit results (overall %, guess → tuned)

| drum | guess→tuned | layers | character note |
|---|---|---|---|
| sd (snare)   | 73 → **87** | modal body + noise | good; mix a touch bright/short |
| hh (closed)  | 44 → **83** | noise + metal | good |
| oh (open)    | 77 → **86** | noise + metal | good |
| cp (clap)    | 78 → **88** | noise (repeat) | good; could use a flam tap |
| tom_low      | 79 → **86** | modal + sine sweep | ⚠ pitch drifted 100→207 Hz (too high for a low tom) |
| tom_high     | 80 → **85** | modal + sine sweep | ok |
| ride         | 79 → **84** | bell + wash | ok; hardest basis (sustained inharmonic) |
| crash        | 70 → **86** | wash + metal | good |
| rimshot      | 48 → **88** | tok + click | ⚠⚠ pitch drifted 420→**85 Hz** — scores 88% but is NOT a rimshot |
| cowbell      | 66 → **80** | 2-mode modal | ⚠⚠ spread collapsed 1.48→1.00 (two-tone identity GONE) + dur 0.35→1.0s |
| tambourine   | 72 → **88** | noise + jingle | ⚠ jingle spread collapsed →1.00; noise carries it |

No muted layers (the batch guards each layer's solo-peak).

---

## THE headline finding: the fit metric optimizes spectral distance, not perceptual identity

The fitter (level-invariant dB log-spectrogram distance + envelope) happily **moves the
fundamental by octaves** or **merges modal partials into unison** if it shaves spectral error —
but those moves destroy exactly what makes a drum recognizable. So **high fit % ≠ sounds right.**

Concrete evidence this session:
- **rimshot**: 420 Hz "tok" → **85 Hz** boom. 88% fit, wrong instrument.
- **cowbell**: spread 1.48 (540+800 Hz two-tone) → **1.00 (single 924 Hz tone)**, duration → 1.0 s. The cowbell's whole identity is the two detuned partials; the metric threw it away.
- **tom_low**: 100 → 207 Hz (up an octave; "low" tom isn't low anymore).
- **tambourine**: jingle modal spread → 1.00 (collapsed).

These are kept AS-IS in the saved files — they're the honest fitter output, and they're the
best demonstration of why we need a better criterion. (Don't trust the % alone when auditioning
rimshot/cowbell.)

### Tools missing to figure out WHY the drums aren't drumming (diagnosis)
1. **A perceptual/identity term in the fit.** Candidates, in priority order:
   - **Fundamental-octave penalty** — punish when the mix's strongest low partial is an octave (or fifth) off the ref's. Stops the rimshot/tom_low pitch drift.
   - **Spectral-peak alignment** — match the ref's discrete peaks (we already extract `ref_peaks`), not just the smoothed envelope. Stops the cowbell unison-collapse.
   - **Attack-weighted distance** — weight the first 20–50 ms heavily; the transient is most of a drum's identity. (Right now onset and tail count equally.)
2. **Per-knob before→after delta + sensitivity** — the optimizer is a black box returning a number. A view of which params it moved (and each param's d(fit)) would have shown "it's abusing pitch" immediately.
3. **Per-layer fit contribution** — "fit with this layer soloed / muted" so you can see whether the body or the noise is carrying (or fighting) the match.
4. **Peak-alignment readout** — `ref_peaks` vs `mix_peaks` as a list with semitone error ("ref 323 / mix 237 ✗ −5 st"). Reading raw Hz arrays is slow.
5. **Attack/transient inspector** — onset-zoom waveform + attack-time/peak for ref vs mix.
6. **Residual (ref − mix) spectrum view** — shows what's still unmatched → "add a layer here", and tells you when a single voice basis simply can't reach the ref (inharmonic-metal + sweep needs ≥2 layers).

---

## Tools missing to run this SMOOTHLY (workflow ergonomics)
1. **A drum-kit / batch view** — I had to write `batch_drums.das` headless because there's no "tune all" or "open next". A kit panel (rows = .sfx targets, per-row fit %, buttons: optimize / next / prev) would make 11 drums fluid instead of 11 manual open→optimize→save→listen cycles.
2. **A/B audition on one hotkey (ref ⇄ mix), loudness-matched** — fastest way to judge. The fit is level-invariant but the ear isn't, so the A/B must gain-match or it misleads. (drum_compare had an S-toggle; the lab has separate play buttons.)
3. **Quick per-layer solo/mute from the keyboard** while tuning.
4. **Save-as-next helper** — auto-suggest `sounds/<refname>.sfx` from the loaded reference; save in one click.
5. **Bounce-to-wav** of the current sound (a button + live command over the module's `render_sound_file`) — to listen outside the app or drop into a track.
6. **A "new layer from template" + silent-layer validator.** Authoring `.sfx` by hand is landmined (see gotchas) — a validator that flags a layer rendering near-silent would catch it instantly.

---

## Authoring `.sfx` gotchas hit this session (load-bearing)
- **`sscan_json` zero-inits array elements; it does NOT apply struct field defaults.** So an
  omitted `"enabled"` on a layer becomes `false` → the layer is muted → silent render. Every
  hand-authored layer needs explicit `"enabled":true`.
- **sfxr `lpf_freq` defaults to 1.0 (= filter fully open).** Omit it and zero-init makes it `0.0`,
  which engages a *fully-closed* lowpass → **silent layer**. Every hand-authored sfxr layer needs
  `"lpf_freq":1.0` (and `sound_vol > 0`).
- Top-level `Sound` fields and the `post` struct DO keep their defaults (the doc is
  pre-constructed before `sscan_json`), so omitting `"post"` entirely = all effects off (fine).
  Only **array elements** (layers) zero-init.
- **The optimizer can drive a layer silent** (lpf_freq → 0, sound_vol → 0) since those are in its
  knob set — a latent footgun for the live "optimize" too. The batch flags it; the app doesn't.

## Files
- `sounds/{sd,hh,oh,cp,tom_low,tom_high,ride,crash,rimshot,cowbell,tambourine}.sfx` — the estimates.
- `batch_drums.das` — headless "tune every drum" (reusable; faithful copy of the app optimizer).
- `drum_refs/*.wav` — the Stolen-soundfont references (gitignored).
