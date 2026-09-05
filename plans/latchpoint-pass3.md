# Latchpoint: natural dialogue and playtest pass

## Scope

This pass targets a completable small sandbox, not perfect conversational interpretation.
`VerifiedTrial` is available experimentally instead of the earlier topic router and canned
gameplay replies, but is NOT the default: real-model route checks exposed regressions that
mocked unit tests do not catch. Graphics, new companion mechanics, and microphone input are deferred.

## Dialogue and authority

- A request-only planner extracts actions and explicit task commitments. A compound offer
  to restore power plus a workshop request records the agreement before attempting access.
- Game code remains authoritative for permissions, prerequisites, inventory and doors.
- Natural speech gets current facts, shared world knowledge, separate resident history,
  and actual action receipts. It cannot execute its own draft tool calls.
- A separate, read-only model request extracts reply claims. Code checks extracted action
  and quoted state claims against receipts and world state before releasing speech.
- Missing-action recovery rechecks the original player request, never authorizes an action
  because the robot promised it. Two speech drafts, one missing-action recheck, three action
  attempts maximum. Malformed plans get one protocol-only retry before failing closed.
- Softer relevance/style findings are advisory and retained in traces, not correction loops.
  Claim extraction can miss things: this is not a universal hallucination guarantee.
- Brack's spare is a free consumable gift despite the legacy `lend_actuator` action name.
  No invented earning, payment, return, or repair-first prerequisite is intended.
- The archive contains maintenance records, service histories and an older identification
  plate. Its unlock still requires both power and repaired Patch, with no extra escort.

## Native fixes

- E's activation character is consumed without swallowing the first genuinely typed letter.
  Live key-down/character/key-up checks confirmed empty initial text, then a typed `e`.
- The SALVAGE 04 label, actuator mesh and pickup prompt share the availability predicate.
  Live state confirmed the label is disabled in the already-consumed save.
- Patch turns toward the avatar while following; its complete model rotates about its root,
  gait follows actual movement, and heading persists with the pose checkpoint.
  Following is still local steering, not a route-leading ability.

## Playtest traces

Local JSONL traces live in `logs/latchpoint-playtests/`. They contain typed dialogue, drafts,
plans, receipts, review findings, before/after world state, timings and a source fingerprint.
The menu discloses local logging. Nothing uploads automatically; inform testers and redact
before sharing. New Game does not erase diagnostic traces. The bridge supports
`--no-playtest-log` (and optional `--speech`) for an opt-out launch.

## Deterministic verification

- 59 Python tests pass, including authorization, bounded recovery and trace isolation.
- 29 daslang tests pass across six test files; the scene compiles.
- Eleven daslang files lint clean; games review reports OK.

## Remaining rough edges

The final power-route attempt correctly recorded the compound agreement and opened the
workshop, then correctly denied archive access with unrepaired Patch. It subsequently
aborted on an unauthorized task commitment in the actuator request. A bounded protocol
retry was added. The repair-route attempt then exposed false `lend_actuator` claims extracted
from correct Patch repair instructions, causing two fallback replies, followed by an invalid
action plan even after retry. Neither full real-model route passed. These are blocking
regressions for promoting the experimental path, not merely imperfect style.

The live bridge was briefly switched for startup verification, then returned to GroundedTrial
without altering player progress. Native fixes and local trace recording remain active.
The earlier gameplay-routing weaknesses therefore remain in the default dialogue path.

CPU inference now includes multiple sequential requests. Longer cold prompts and retries
can be conspicuous; responsiveness needs fresh player feedback, not only throughput numbers.
A rejected double draft falls back to a safe retry message. A protocol failure can still
surface as an interrupted conversation. Old histories can influence wording; start a new
game when comparing behavior across builds.

Reported wall/generator overlaps, generator-facing questions and intermittent black squares
remain for the graphics discussion. No new graphics fix is claimed here.
