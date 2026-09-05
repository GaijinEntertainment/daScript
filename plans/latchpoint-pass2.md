# Latchpoint: second playable pass

## Rendering

- Sky rays now interpolate before normalization. Frozen-cloud screenshots at yaw 0 and
  0.2 radians were reprojected into the same world directions: 16,200 sky pixels had mean
  RGB error 1.26/255 and 95th-percentile error 2.87/255 (post-process grain remains active).
  Cloud drift is faster and visible while standing still.
- Nonuniformly scaled meshes use inverse-transpose normals. Surface detail uses object-space
  procedural roughness/metalness maps, with footprint-filtered fine/brushed variation instead
  of unfiltered world-space pixel noise. Material debug views expose both maps and normals.
  These are shader-generated maps, not external bitmap textures.
- The existing 2048 shadow map now uses 5x5 tent-weighted PCF, slope-dependent bias, and
  full depth/frustum bounds checks. AO blur rejects unrelated depths and normals at edges.
  A separate screen-space shadow pass was not added.
- The actuator is anchored to its crate top. Rotated crates are spaced apart so their
  coplanar top faces cannot overlap. The generator's collision-only box is no longer drawn
  as a wall hiding the actual machine; the machine has a supporting plinth.
- A seven-yaw sweep read back the HDR target: zero invalid/out-of-half-float-range pixels,
  zero GL errors, peak RGB between 1.14 and 22.11. This does not prove every sporadic glitch
  is gone. The readback is a diagnostic stall, not an FPS benchmark. Live status continued
  to show approximately 120 FPS outside diagnostic readbacks.

## Controls, saves, and wayfinding

New Game / Continue works with mouse clicks or Up/Down + Enter. Escape opens the menu and
selects Continue by default; Escape first cancels an active text entry. New Game clears
world progress and resident histories through the serialized bridge worker, then resets
native positions. A failed save restores the previous in-memory world. Continue restores
progress and the position checkpoint saved on menu entry or normal shutdown.

WASD and arrows now have identical movement semantics, with Left/Right strafing. Duplicate
bindings do not double speed; diagonals are normalized. Both Shift keys hurry. Right mouse
drag looks around. Actual synthetic key/character/mouse events exercise these callbacks.

The HUD labels WORKSHOP 01, GENERATOR 02, ARCHIVE 03, and SALVAGE 04, and shows a compass.
The coil gets a nearby pickup label. Collected actuator/coil props and interaction prompts
use the same collected-state condition. The live pickup test showed an actuator in cargo
and no remaining salvage interaction; Continue preserved both cargo and position.

## Gameplay dialogue

The native scene now uses `GroundedTrial` rather than treating free-form speech as action
confirmation. Qwen classifies intent, with recent dialogue resolving references and short
answers. Game code validates and executes the requested action and authors the factual
reply from the receipt/current state. Locations and interlock explanations use the real
scene landmarks. A repaired Patch is reported as repaired even while generator power is off.
Patch can follow or stop following; requests to lead to the coil get an honest capability
limit and real directions, not a nonexistent navigation promise.

Creative conversation still uses Qwen, but that path cannot execute unplanned tools. This
trades some improvisation in practical lines for reliable facts. Classification remains
fallible; unfamiliar or ambiguous requests can need clarification. The original native-tool
dialogue experiment remains available in the terminal driver without `--grounded`.

`player_regression.json` replays the user's actuator, workshop, coil, guidance, partial
archive-condition, and eventual-unlock wording. The latest run passed 14/14 state checks;
the location and guidance transcripts named the actual workshop bench and admitted the
absence of a leading controller. Typical gameplay turns were around 1-2 seconds on the
eight-lane CPU setup; classification uses temperature 0, while creative chat uses 0.5.

No second verifier model was added. A useful next experiment is a pre-speech verifier for
misrouted conversational drafts: check the player request, draft, and tool receipts, then
request a bounded retry. It should not silently perform arbitrary late actions after the
player changes context. Every correction still goes through the same action validator.

## Speech pacing

Same 18-word sentence, Kitten Nano q8, 24 kHz:

| Resident | Original WPM | Selected speed | New WPM |
|---|---:|---:|---:|
| Brack | 139 | 1.12 | 160 |
| Patch | 93 | 1.60 | 147 |
| Vesper | 128 | 1.20 | 157 |

Parakeet transcribed all three selected clips exactly after punctuation/case normalization
(0/54 word errors). This is a tiny synthetic intelligibility check, not a subjective voice
quality judgment or microphone benchmark. Spatial audio, interruption behavior, and ASR
input are still separate work.

## Checks and handoff

42 Python tests pass. Four daslang suites report 23 passing tests total (controls, save
snapshot, shading math, world/collision). Nine daslang files pass lint and formatter checks.
The games review gate and `git diff --check` pass. The original checkout's unrelated edits
remain untouched. The pre-pass playthrough is backed up under `saves/scene-before-pass2-*`.
