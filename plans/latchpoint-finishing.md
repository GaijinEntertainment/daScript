# Input, voice and archive finishing pass

- Nearby robot hint now exposes both paths: E to type and hold V to speak. Main Enter and
  numpad Enter share confirmation handling for the menu and chat.
- Drill motor gain is raised 25%; the grinding/contact layer 45%.
- Robot voice coloration is subtle, with different wet amounts for Brack/Patch/Vesper,
  a small low-frequency adjustment, short feed-forward resonance and gentle saturation.
  Stereo channels are processed independently. P toggles processing for the next utterance;
  the setting persists. Dry playback is the unprocessed model output.
- Two irregular, faceted outcrop meshes replace beveled-box rocks. Broken height bands,
  varied silhouettes and rubble clusters add structure; stone-only strata/dust coloring
  separates geology from the existing industrial materials. Bounds stay outside the yard.
- F inspects a service plate inside the unlocked archive. Its serial matches the player's
  avatar; the inspection predates the shift and an operator signature is already present.
  Discovery is saved once, with a completion notification. Reinspection remains available,
  and the sandbox never exits or forces a win screen.

## Verification

77 daslang cases and 59 Python reference tests pass. Tests cover keypad confirmation,
archive gating/reinspection/persistence, bounded outcrop geometry, voice bounds and dry bypass.
The native Continue menu was exercised with numpad Enter; chat submission also accepted it.

The existing repaired-Patch save was driven through validated workshop access, coil pickup,
installation, nine-second manual startup, archive access and record inspection. Developer
camera placement was used between interactions; this was not a navigation/collision playthrough.
The resulting test save is complete; use New Game for a fresh full run.

All three voices produced identical dry and processed ASR transcripts for the same 12-word
test sentence. The comparison WAVs are retained under `logs/latchpoint-finish/voices/`:
0 = Brack, 1 = Patch, 2 = Vesper; each has `-dry.wav` and `-processed.wav`.
The exact outputs are in `logs/latchpoint-finish/voice-check.log`. This is an intelligibility
spot check, not a substitute for the user's listening evaluation.

The outcrop view and archive payoff were visually inspected. Adding saved/live state fields
exposed a live-host reload recovery crash; its evidence is retained in `reload-crash.log`
in the same log directory and recorded in the playtest follow-up ledger. A fresh host loaded
the saved game successfully. No live-host engine fix or new public issue is claimed here.
