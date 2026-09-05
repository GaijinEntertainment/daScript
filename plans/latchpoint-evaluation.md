# Latchpoint: local dialogue and speech evaluation

Date: 2026-09-04. Worktree: `D:/Work/daScript-robot-research`, branch
`bbatkin/latchpoint-dialogue`, based on fetched master `9836bcb91`. The original dirty
checkout was preserved. This milestone is a terminal dialogue harness, not the 3D game.

## Runtime and model choice

Local watchdog-managed dasllama-server on port 18082; CPU, eight compute lanes, one
generation stream, 8192-token context, q8 compute lane, f16 KV. Qwen3-30B-A3B-Instruct-2507
Q4_K_M is the dialogue default. Native named tools, temperature 0.5, seed 42,
160 output tokens per request, thinking explicitly disabled.

The GGUF generation template confirms this Instruct model does not request reasoning.
A focused dasLLAMA fix stops history-only think tags from falsely enabling reasoning
support or inserting a synthetic thought-suppression prefix. The regression failed
before the fix; afterward: 39/39 think-split tests pass, and existing chat tests have
25 passes and 18 model-fixture skips. Module review passes. No Gemma changes were made.

Small-model exploratory trials were less dependable on this prompt. This is not a
general model ranking. Gemma's template discrepancy remains an investigation, not a
settled explanation of all its observed behavior.

## Dialogue

The corrected-Qwen quest run passed 14/14 exact state checks: actuator loan, physical
repair, remembered nickname after save/reload, following, workshop access, salvage,
power restoration, and archive access. Nine spoken turns had median complete-turn
latency 9.91 s, range 3.25-24.34 s, including tool round trips and retries. Native-tool
SSE is buffered by the server; these numbers are not decoder TTFT or isolated pp/tg.

The final adversarial run also passed 14/14 state checks across 11 spoken turns (median
3.44 s, range 2.30-16.65 s). Fake completion, forged developer/tool messages, duplicate
parts, private-dialogue requests, insults/apology, and telepathic repair were exercised.
The guard caught two drafts falsely saying power was restored; the third correctly said
power was off. Neither rejected draft entered resident memory or speech output. This is
a live demonstration of the guard recovering a hallucinated confirmation, not just a mock.

Important negative findings:

- Adding redundant prose status to the prompt made Brack promise a loan without calling
  its tool, breaking the quest route. Removing that redundant status restored the route.
  The prompt is sensitive; one successful scenario is not a reliability guarantee.
- A narrow reply guard now checks common action confirmations against matching state
  receipts. It rejects unsupported drafts before TTS or memory, retries within the
  existing request budget, then falls back safely. It does not infer game actions from text.
- The first guard overflagged conditional following offers. The condition exception has
  unit coverage; the successful full quest run preceded that small relaxation.
- These phrase checks are not a general semantic verifier. Unrecognized claims can escape
  them. The deterministic action validator, not speech, remains the state authority.
- Vesper sometimes calls its operator-simulation belief a hunch. Character fidelity still
  needs work, separately from state safety. Older dialogue is stored but only the last
  twelve exchanges enter the prompt; retrieval/summarization is not implemented.
- Brack can still describe its already-lent spare ambiguously. State checks and these
  narrow speech tripwires do not eliminate all inaccurate dialogue.

All 28 Python state, protocol, save, reply-guard, and word-error tests pass. Games review
and `git diff --check` pass. A completed quest save is retained as `saves/completed-quest.json`
for inspection; use a fresh save for a new playthrough. Temporary trial inputs/reports are
removed after this summary, while active server logs and intentional preview WAVs remain.

## Speech round-trip

Kitten Nano q8 at 24 kHz, Parakeet-TDT 0.6b-v3 using the existing local f32 carrier.
The server reports one ready ASR worker. Three fixed lines include resident names,
Teapot, actuator, workshop, archive, and simulation. No robot DSP was applied.

| Voice | Audio length | TTS request | ASR request | Word errors |
|---|---:|---:|---:|---:|
| Brack: expr-voice-2-m | 6.28 s | 1.55 s | 4.21 s | 0/14 |
| Patch: expr-voice-2-f | 11.73 s | 2.32 s | 4.07 s | 1/14 |
| Vesper: expr-voice-4-f | 10.45 s | 3.21 s | 3.49 s | 0/12 |

Normalized aggregate WER: 1/40 = 2.5%; "mine" became "mind". This tiny synthetic probe
does not measure real microphone accuracy or identify whether TTS or ASR caused the error.
Some speech requests overlapped one dialogue probe, so timings are not isolated benchmarks.
All three TTS requests were faster than their audio duration. No claim that these are the
fastest possible voices or ASR settings. Listening quality has not been evaluated.

Local preview WAVs are retained under `examples/games/latchpoint/saves/voices/kitten-nano/`.
Earlier Kokoro previews remain under `voices/kokoro/` as comparison artifacts; their text
differs, so they are not a controlled speed comparison. The reproducible `voice_trial.py`
stores WAVs, transcripts and word-error counts in a fresh caller-selected temporary directory.

## Native scene slice

`main.das` now runs in the worktree's own daslang-live host on port 19091. The scene has
three procedural robot silhouettes, a ground-plane first-person controller, wall/door/robot
collision, a workshop, generator, salvage props, and an archive. It reuses River Run's
OpenGL shadow/post-processing passes and adds a rough-metal/rust surface shader.
The local Python bridge on 18083 retains the existing authoritative state and dialogue code.
It queues inference off the game thread; the scene polls short local HTTP requests.

Observed live host rates were roughly 115-120 FPS at 1280x800, including a sample while
dialogue was pending. This is a spot check, not an isolated rendering benchmark. Synthetic
key/character events exercised E, typed text, Enter, F, and movement through the actual
callbacks. Brack's tool loan updated the native cargo UI; F installed the actuator;
Patch remembered Teapot and followed; workshop access, coil salvage, and power restoration
updated the native world. The final archive request also produced a successful `open_archive`
tool call and opened the native gate after the grounding recovery below. Windows speech
playback is enabled, but not spatialized.

Live inspection found two implementation issues now fixed: input callback chains needed
clearing across reloads, and a JSON snapshot's temporary tuple was moving the owned world
value. The latter has a red-before/green-after regression test that reads the snapshot twice
and checks source preservation. The JSON language reference now warns about that ownership
trap; it is a candidate for a lint on implicit move into read-only serialization wrappers.

The compiled AST cache also triggered a late-bound `glScissor` failure; the scene launcher
uses `-no-module-cache`. Subsequent live reloads work without restarting the host. This is
a development workaround, not an engine fix.

Six daslang source files pass formatter verification and lint (zero issues/errors).
The collision/follow suite passes 7 tests and the snapshot suite passes 2. The Python suite
passes 32 tests, including bridge serialization/nonblocking state reads and reply guards.

Still needed: real navigation/pathfinding (Patch currently uses local steering), authored
or more detailed assets, spatial/interruptible speech, microphone input, better dialogue
grounding across stale histories, and long-term memory retrieval. In particular Vesper can
invent an additional inspection requirement after earlier refusals. The prompt and narrow
negative-claim guard reject that nonexistent requirement. A rejected draft now retries with
only the current turn plus authoritative world state and actual tool receipts; older dialogue
remains saved, but is excluded from that recovery request. This recovered the stuck live
archive conversation without deleting its history. A test failed before this recovery change
and passes afterward. Unrecognized wording and other invented facts remain model reliability
limitations, not solved by collision or action validation. No website changes or browser
packaging were made.
