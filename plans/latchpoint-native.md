# Latchpoint native runtime, audio and feedback pass

## Runtime boundary

The game no longer launches or calls `scene_bridge.py`. `lp_runtime.das` owns world changes,
resident histories and saves. `lp_dialogue.das` runs the dialogue state machine; `lp_inference.das`
uses dasOPENAI's compatible transport/audio APIs on background threads. The separate
dasllama-server still owns the large models. No server embedding or new inference backend.

Native byte streams carry serialized results, not pointers into the worker's heap. A done
status fences resource removal. The stream/status handles and dialogue stage survive ordinary
live reload. Cancelling a conversation discards its eventual result; it does not abort server
generation, and another inference request waits for the old one to drain. Physical exploration
remains available. Chat requests time out at 90 seconds; ASR/TTS requests at 25 seconds.
Sending again while a cancelled request drains preserves the typed text for retry.

The existing version-1 save format remains readable. Native resident memory retains role/text
exchanges; historical Python tool-message metadata is not retained in its reduced message
schema. Public world events remain. Each save keeps one `.previous` copy; the migration also
preserves `scene-before-native.json`. Unsupported/partial save headers fail without destroying
the current state. Saves are backed up but not atomic replacements; a write failure is reported,
and action state is rolled back in memory. A prior backup may be needed after a disk failure.

Practical dialogue remains an intent classifier plus authoritative facts; general chat is
speech-only. This is not a new hallucination solution. One live repair question still selected
the wrong gameplay topic during bring-up. Deterministic action guards are ported and tested;
the experimental Python verifier is not enabled. Python trial suites remain external references.

## Sound and motion

- A three-layer synthesized daStrudel score: low drone, sparse metallic FM notes and a quiet
  mechanical pulse. Generator power introduces a fourth harmonic layer. No SoundFont download.
- dasAudio positional speech with eight HRTF slots, distance attenuation and a small speaker
  coloration filter. Music ducks under speech. Actual playback status/PCM energy drives the
  speaking phase and light envelope; text is not used to guess speech duration.
- Generated motor, servo, click, wind and thunder sounds; gate motors follow travel, footsteps
  follow avatar distance, generator hum follows power, and thunder is delayed from storm events.
- Listening is bright/steady, thinking dim/slow-pulsed, speaking energy-driven; head/inspection
  motion moves light and voice attachments together. Vesper turns gently, Brack's head moves,
  and Patch's head/antenna sit within its existing root-follow animation.
- Gates lift over roughly 2/2.5 seconds. Collision remains until the opening reaches head
  clearance. Guide rails support the raised panels.
- Egg-shaped backdrop rocks are replaced with squat angular forms positioned beyond the
  playable yard, eliminating the intrusive non-colliding forms rather than adding blockers.

## Speech input and controls

Hold V near a robot, release to transcribe, then edit and press Enter. Capture is capped at
20 seconds; Escape discards the take. The microphone is never opened automatically. The
robot remains bright during capture and ASR. The native client sends the WAV to the loopback
server using dasOPENAI transcription, and removes the temporary WAV afterward.

This first input path is push-to-talk, not Silero-VAD endpointing or echo cancellation.
Microphone hardware and speaker bleed still need the user's live test. Keyboard input remains.
The soundtrack is ducked during capture; robot speech is stopped before capture starts.

F8 music, F9 world effects, F10 voice, F11 future conversation traces. Visual/audio/privacy
preferences share the existing settings file. Key repeats no longer retrigger toggles.
E interrupts speech to type; Escape interrupts and opens the menu. The HUD distinguishes
recording, ASR, thinking, voice preparation and speaking.

## Checks

- 54 daslang cases and 59 existing Python reference tests pass; games review passes.

- Native tests cover both original objective routes, actor permissions, item consumption,
  duplicate transfer rejection, save ownership/round-trip, separate memories, corrupt-save
  rejection, gate collision timing, sound sample bounds, light phases and attachment identity.
- Real requests reached the server through dasOPENAI with the Python bridge stopped.
  A live conversation completed across a reload while inference was pending.
- A daslang-only Kitten Nano -> WAV -> Parakeet test returned the exact 12-word sentence:
  "My leg needs an actuator. The copper coil belongs in the generator."
  This tests transport/intelligibility, not subjective voice or soundtrack quality.
- No real microphone was activated by automated testing. No cloud endpoint was used.

Patch's cooperative generator interaction, VAD/hands-free input, richer sound design and
dialogue tuning remain separate passes. The original objective chain is intentionally unchanged.

## Server recovery observation

Filed as [#3940](https://github.com/GaijinEntertainment/daScript/issues/3940).

The server log recorded a GC exception at `utils/dasllama-server/main.das:777`:
heap collection could not attribute locals of a compiled/position-less frame. The process
remained alive but stopped responding to its health endpoint, so the watchdog reported
unhealthy without restarting it. Terminating that identified server child allowed the
watchdog to relaunch it; a subsequent real conversation and TTS completed. The game and
save did not need a reset. This recovers service but does not fix the underlying GC defect.
