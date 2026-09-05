# Latchpoint playtest follow-ups

## Reported after the cooperation pass

- Brack actuator handover: visual/cargo receipt works, but the player reports no audible cue.
- Installing an actuator into Patch: player reports no audible cue.
- Salvaging the coil: player reports no audible cue.

These remain open even though a generic feedback cue is wired in code. Verify actual playback,
gain, masking, location/attenuation and reload state before claiming resolution. Distinguish
handover, installation and salvage with suitable sounds; do not equate a scheduled command
with an audible result.

- After a successful repair, asking Patch how he is can still produce a limping/damaged reply.
  This is stale conversational state in speech, not evidence that the authoritative repair
  failed. Add the exact sequence (repair -> social check-in) to real-model regressions. Current
  world facts must win over the damaged-leg persona and old exchanges. Still open.

## Server stability

The separately filed GC/frame-attribution failure remains open; restarting recovered service
but did not fix its cause. See the native-pass recovery notes and the issue link recorded below.

[GitHub issue #3940](https://github.com/GaijinEntertainment/daScript/issues/3940)

No gameplay, audio, or inference settings were changed while recording these observations.

## Live-reload failure observed during finishing pass

Adding the archive state field preceded an `__after_reload_live_vars` deserialization failure
(implausibly large byte-array lengths), followed by `UNHANDLED_CPP_EXCEPTION` in
`live_host_clear_store` (`dasLiveHost.cpp:94`, `run_lifecycle` at `main.cpp:672`). The live host
exited. A fresh launch loaded the saved game successfully. Root cause is not established;
the log is retained at `logs/latchpoint-finish/reload-crash.log`. This is separate from #3940.
