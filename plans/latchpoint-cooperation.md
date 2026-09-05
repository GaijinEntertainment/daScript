# Interaction feedback and Patch cooperation

## Changes

- Successful transfers and physical interactions show a short, immediate notification with
  an audio cue. Brack's actuator gift explicitly says it was added to cargo and drives a
  two-second handover gesture. The notification does not wait for TTS.
- Gate motors have 2.3x channel gain and a larger attenuation reference distance (6 rather
  than 3 units), plus a latch-release cue. This improves audibility without changing the
  speech/music volume controls.
- Landmark text uses narrow holographic brackets and an anchor stem with distance fading,
  rather than a filled rectangle. The compass is removed; object names remain visible.
- Brack inspects with his gripper, Patch shifts a foreleg during idle inspection, and Vesper
  raises a small illuminated record slate. Conversation suppresses their idle work cycles.

## Generator interaction

Installing the coil now records `coil_installed`; it does not immediately restore power.
The player must hold F near the generator to crank its jammed starter. Solo completion takes
nine seconds. Repaired, following Patch approaches the service position when brought nearby,
faces the machine, and holds the clutch during a three-second assisted start. Its tool arm,
inspection beam, crank motion and starter sound accompany the attempt.

Releasing F, leaving the interaction area, entering text/menu input, or lacking a coil stops
the attempt and resets progress. Startup is deterministic, consumes no additional item and
does not require Patch: the solo route remains valid. Archive requirements remain power AND
repaired Patch, not assistance. Explicit requests to help start the generator map to Patch's
existing validated follow action. Dialogue knowledge distinguishes coil installation from
startup and does not claim that an LLM response itself starts the machine.

Old powered saves are migrated to `coil_installed=true` on load; already-completed power is
not revoked. In-progress cranking is transient and resets on reload. Existing saves can
continue; New Game is the clean way to experience the complete revised sequence.

## Verification

60 daslang tests and 59 Python reference tests pass. Native tests cover manual/assisted
startup, interruptions, absent coils, out-of-range players, and Patch's service approach
and facing. The Python reference driver retains the old immediate-start gameplay and is
not a parity oracle for this revised generator sequence.

A real server-backed Brack request transferred one actuator, updated cargo, and displayed
the notification before voice preparation completed. The live test leaves that actuator
in the current save. Holographic labels and the transfer notification were visually inspected.
No complete manual in-game navigation playthrough is claimed; Patch still uses local steering
and can require the player to guide it around obstacles.

Ambient LLM chatter, additional environmental sound layers and temporal antialiasing remain
subsequent passes. Inference stream configuration is unchanged.
