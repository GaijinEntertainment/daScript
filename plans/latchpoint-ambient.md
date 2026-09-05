# Ambient robot activity and chatter

## Activity sequences

- Brack lowers his drill onto a scrap plate, spins up, grinds briefly and retracts while
  spinning down. Flutes visibly rotate. The motor, grinding loop and 48 instanced spark
  particles are driven by the same phase functions. Sparks originate at the plate contact,
  only after full extension and spin-up. Rotation integrates the speed envelope rather than
  multiplying the world clock by a changing speed.
- Patch switches the inspection lamp on, sweeps it, pauses and switches it off. Eyes remain
  illuminated. Listening, speaking, following and generator assistance keep the work lamp
  available instead of letting the ambient cycle turn it off.
- Vesper's existing record-slate activity supplies a natural occasion for a remark.
- Player conversation, microphone capture and menus suppress background work. The drill
  stops/silences on interruption; the current implementation retracts immediately rather
  than blending a full interrupted animation transition.

## Chatter pipeline

The first version produces one short observation per eligible event, not a multi-robot
conversation. Eligible events are completed drill tests, inspection sweeps, record checks
and distant lightning. Requests have at least 65 seconds between them, start only while
idle, and require the speaker to be within 13 units. There is a startup quiet period.

dasOPENAI runs the background request through its own native stream/status job, independent
of the player's job. The candidate sees a compact character description, the observed event,
current power/repair facts and the previous line. No gameplay tools are supplied. Truncated,
oversized, exact-repeat or tool-bearing responses are rejected before synthesis.

Before synthesis and again before queuing playback, the game checks player-interaction epoch,
world snapshot, distance, age and the chatter toggle. Stale lines are dropped, never converted
into actions. The candidate expires after 40 seconds; generation/TTS requests have 20/15-second
timeouts. This is state/transport validation, not a semantic guarantee against invented flavor.

Ambient speech uses the existing positional voice path, a separate caption, and the robot's
speaking-light envelope. It does not replace the player's last reply or selected conversation
resident. When playback starts, the observation is retained in that speaker's own history.
E/V interrupt it for player interaction. F12 disables future chatter, clears queued ambient
audio and interrupts ambient playback; the preference persists. Gameplay and music continue.

## Concurrency

The local and example server configs now use two generation streams with the same eight CPU
lanes. A graceful `/restart` applied the local setting. Live stats reported `max_streams=2`
and `peak_active=2` during a deliberately overlapping ambient/player request test. The ambient
result was discarded after the player's interaction epoch changed; the player's reply and
TTS completed normally.

Priority here means admission policy, independent client slots and discarding background
results. It is not server-side weighted scheduling or preemptive cancellation. In-flight
background generation can still compete for CPU, and already-started TTS can occupy a worker
briefly. No claim is made that foreground latency is unaffected.

## Verification

- 71 daslang tests pass, including contact/spin gating, lamp/eye separation, stale-state,
  interruption, expiry, distance, disabled chatter, rejected tool calls and caption isolation.
- 59 Python reference tests pass; games review passes.
- The live drill/plate/spark view and Vesper's ambient caption were visually inspected.
- Natural activity-driven remarks generated and played. A controlled records observation
  produced a short Vesper remark and positional playback with the correct speaker caption.
- HDR readback in the inspected drill view reported zero invalid values and GL errors.

Sound design still needs the user's listening pass. Two-robot exchanges and more sophisticated
activity blending remain follow-ups; this pass does not retune the foreground dialogue model.
