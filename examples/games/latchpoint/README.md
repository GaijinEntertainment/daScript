# Latchpoint local sandbox

The native daslang/OpenGL scene is now playable: a small industrial yard with Brack,
Patch, and Vesper, free-form typed conversations, salvage, repair, and gated areas.
The game runtime is daslang, including authoritative state, saves, dialogue, audio and input.
It uses dasOPENAI to call the separate local dasllama-server; the model never owns game state.
Python drivers remain developer-only reference tests, not game launch dependencies.

Design: [Latchpoint](../../../plans/latchpoint.md).
Measured results and limitations: [evaluation](../../../plans/latchpoint-evaluation.md).
Latest graphics changes and checks: [twilight pass](../../../plans/latchpoint-twilight.md).
Dialogue status: [dialogue/playtest pass](../../../plans/latchpoint-pass3.md).
Native runtime, audio and input: [native pass](../../../plans/latchpoint-native.md).
Latest interaction changes: [feedback and Patch cooperation](../../../plans/latchpoint-cooperation.md).
Ambient activity and chatter: [ambient pass](../../../plans/latchpoint-ambient.md).
Latest finishing changes: [input, voice and archive](../../../plans/latchpoint-finishing.md).

## Play the native scene

Start the inference server below, then from this checkout's root:

```powershell
cmake --build build --config Release --target daslang-live -j 24 -- /nodeReuse:false
powershell -NoProfile -File examples/games/latchpoint/start-game.ps1
```

The launcher opens the game. Ports are 18082 for inference and 19091 for live development;
there is no game bridge on 18083. Active logs go
to `logs/latchpoint-scene/`. The launcher disables the front-end module cache to avoid an
observed late-bound OpenGL failure after cached compilation; normal live reload still works.

- WASD or arrow keys: move (Left/Right strafe); either Shift: hurry; right mouse drag: look.
- E near a resident: type a conversation; Enter: send; Escape: cancel typing.
- Numpad Enter also confirms/sends. P toggles subtle robot voice coloration for the next line.
- Hold V near a resident: record up to 20 seconds; release to transcribe locally, edit, and
  press Enter to send. Escape discards a live recording. No microphone capture starts automatically.
- F near a component: salvage/install it. Parts and prerequisites are checked by game code.
- After installing the coil, hold F at the generator: nine seconds manually, three with
  repaired/following Patch at the clutch. Releasing F resets progress; assistance is optional.
- Once the archive opens, press F at the service plate inside to inspect it and complete
  the short discovery. Free exploration continues; there is no forced ending screen.
- R: read saved world state again when no request is pending.
- Escape outside text entry: New Game / Continue menu. Click a button or use Up/Down and Enter.
- Outside text entry: L disables/enables storm flashes; F6 toggles screen-space reflections;
  F7 toggles dust and light scattering. The menu shows their state; settings persist.
- F8 toggles music, F9 world effects, F10 speech, F11 local conversation logging.
- F12 toggles ambient robot chatter. It is occasional, positional and yields to E/V player
  interaction. Remarks follow activities/weather, not a constant stream of random dialogue.
- E interrupts speech to start a reply; Escape interrupts speech or discards the pending
  inference result and opens the menu. Already-validated actions are not undone.

New Game clears progress and all resident conversations. Continue restores saved progress,
conversations, and the avatar/Patch positions saved on entering the menu or closing the game.
The position checkpoint is `saves/scene-pose.json`. Holographic landmark labels identify
WORKSHOP 01, GENERATOR 02, ARCHIVE 03, and SALVAGE 04; the coil is on the workshop bench.
Consumed salvage no longer offers an interaction prompt.

The world keeps moving while a resident responds. Patch can follow once repaired and
asked, and can be asked to stop following. Workshop/archive barriers and generator status follow validated receipts. This is
an optional short objective chain, not a forced quest menu. dasAudio plays positional speech,
robot cues, motors, wind and delayed thunder; daStrudel supplies a lightweight synthesized
score. Gate geometry travels before collision allows passage. Microphone input is deliberate
push-to-talk; VAD endpointing and echo cancellation are not implemented yet.

Idle Brack tests his drill on a scrap plate with contact-only grinding/sparks; Patch switches
and sweeps an inspection lamp; Vesper consults records. Optional LLM observations use a separate
background client job with stale-state checks and no gameplay tools. The server configuration
uses two generation streams, still sharing eight CPU lanes. See the ambient report for limits.

The game saves to `saves/scene.json`; it is separate from the terminal trial save. Do not run
the old Python bridge against this save. Closing the game leaves the inference server running.
dasOPENAI requests run on a background thread with a 90-second chat timeout and 25-second
ASR/TTS timeout; a native byte stream
returns results to the main thread. In-flight requests survive ordinary live reloads. An
interrupted request drains in the background before another inference request can start.
Patch uses local collision-aware steering, not a navigation mesh, and can
get stuck behind obstacles. Collision/movement are ground-plane only; no jump/combat yet.

### Gameplay facts and free-form conversation

The scene uses a native port of the earlier gameplay-intent approach: a model classifies a
request, code validates any action, practical replies use current facts, and ordinary chat
uses a speech-only model request. Classification and conversational hallucination weaknesses
remain. The experimental Python verifier is not connected to the game.

Experimental `VerifiedTrial` uses separate Qwen requests to extract player-authorized actions,
write natural dialogue using current facts and actual action receipts, and check the reply
before speech. All actions still pass the game's deterministic prerequisites. The read-only
verifier cannot authorize actions from a robot's own promises. One recheck of the original
player request can recover a missed action; speech gets at most two drafts.

Extracted false state/action claims block speech; softer semantic findings are logged rather
than repeatedly rewriting conversation. Claim extraction and request interpretation remain
fallible, so this is a playable-test safeguard, not a guarantee against hallucinations.
All three requests use the same local model with separate contexts, not another loaded model.
CPU latency includes these extra requests, and cold prompts can take noticeably longer.
The older `--grounded` terminal path remains available for comparison; use `--verified`
with `verified_power_route.json` or `verified_repair_route.json` to test the experimental path.
The latest full-route attempts exposed false-positive reply rejection and malformed plans;
the verifier is not yet a recommended replacement for the default bridge.

### Local playtest traces

The native runtime records submitted conversations, intents, action receipts, replies,
world snapshots and event times under `logs/latchpoint-playtests/native-runtime-v1.jsonl`.
Nothing is uploaded automatically. Tell testers that their typed text is recorded and avoid
personal information. New Game clears game memories but retains these diagnostic traces.
Review/redact a trace before sharing it, or delete the selected trace to remove it.
F11 opts out of future traces; existing logs remain until explicitly removed. Temporary
microphone WAVs live under `logs/latchpoint-input/` and are deleted when transcription returns.
The audio goes only to the configured loopback server, not to a cloud service.

### Live development

Use the `live_*` MCP tools with port `19091`. `cmd_scene_state` exposes the avatar,
Patch position, input, pending job, reply, and world flags. `cmd_pose` accepts `x,y,z,yaw,tilt`;
`cmd_talk` accepts resident index `npc` (0/1/2) and `text`; `cmd_interact` performs only the
interaction available at the current position. Built-in `key_press`, `key_release`,
`key_type`, and `screenshot` exercise the actual input and framebuffer paths.
The pose/talk commands are developer inspection controls, not player permissions.
Reload after editing dependencies; the host watches the entry point automatically.
`cmd_render_settings` accepts `material` (0 lit, 1 roughness, 2 metalness, 3 normals),
`bloom`, `ao`, and `time` (-1 for realtime). `cmd_hdr_check` reads back HDR pixels and GL
errors; its readback stalls are diagnostic, not rendering benchmark measurements.

Rendering reuses River Run's shadow/post-processing modules, with a twilight sky, twelve
local lights, a shadowed swinging lamp, static yard reflection probe, bounded SSR, dust and
light scattering. Procedural roughness/metalness/normal detail and floor parallax complement
beveled geometry. The scene overrides the sky, normal transform and AO blur. No duplicated
River Run source or bitmap texture assets. See the twilight report for approximation limits.
Keep this checkout layout for now;
standalone/browser packaging is a later step.

## Start the local server

Build this checkout's runtime and shared modules together. On this Windows box:

```powershell
cmake -S . -B build -G "Visual Studio 18 2026" -A x64 -DDAS_LLVM_DISABLED=OFF -DLLVM_DIR=D:/Work/libclang/lib/cmake/llvm -DOPENSSL_ROOT_DIR=C:/Users/Boris/AppData/Local/daslang/openssl -DDAS_FLEX_BISON_DISABLED=ON -DDAS_TESTS_DISABLED=ON -DDAS_TUTORIAL_DISABLED=ON -DDAS_AOT_EXAMPLES_DISABLED=ON
cmake --build build --config Release --target daslang dasModuleHV dasModuleAudio dasModuleMinfft -j 24 -- /nodeReuse:false
```

Copy `server.example.toml` to `server.local.toml` and set the GGUF paths. The checked-in
example paths describe the development box; they are not portable defaults. Optionally
add `tts = ".../kitten-nano.gguf"` ABOVE the first `[[models]]` section, with matching
`tts_g2p.bin` and `tts_postag.bin` beside the model. The speech family determines valid
voice names; `characters.json` uses Kitten Nano voices. Later voice input will use
separate Parakeet ASR, not Qwen audio input; typing remains available.
For the audio round-trip probe, add `asr = "D:/Work/whisper.cpp/models/ggml-parakeet-tdt-0.6b-v3-f32.bin"`
above the first `[[models]]` section too, adjusting the path to your installation.

```powershell
powershell -NoProfile -File examples/games/latchpoint/start-server.ps1
```

The launcher uses the repository watchdog, eight compute lanes, one generation stream,
CPU model slots, and GPU inference off. It does not change another deployment's config.
Its port is 18082; logs and the working directory are `logs/latchpoint-server/` at the
repository root. Cold compilation/model preparation can take minutes. Wait for
`http://127.0.0.1:18082/v1/stats` to report both models and `tts.ready` if speech is enabled.
The worker-lane cap is not a process affinity mask or a total operating-system thread cap.
The server can listen beyond loopback; this is a local development configuration, not
an authenticated public deployment.

Stop this instance gracefully:

```powershell
Invoke-RestMethod -Method Post http://127.0.0.1:18082/shutdown
```

## Talk and experiment

From the repository root, Python 3.10+ with no third-party dependencies:

```powershell
python -B examples/games/latchpoint/dialogue_trial.py --grounded
```

The default model is `qwen3-30b`, using native named tools and thinking disabled.
`--grounded` selects the scene's intent/receipt route. Omitting it keeps the original
native-tool dialogue experiment for comparisons; its phrase guard is not the scene's
primary gameplay authority anymore.
The default save is `saves/session.json` beside the driver. Restarting loads it.
`/npc brack`, `/npc patch`, and `/npc vesper` select who you are addressing. Speak freely;
`/world` displays the exact game state and `/quit` exits. The terminal currently prints
each completed reply; it captures SSE timing but is not the future streaming game UI.

Physical interactions are explicit developer stand-ins:

- `/salvage actuator`: collect the single replacement at the mine entrance.
- `/install actuator`: consume an actuator and repair Patch.
- `/salvage coil`: collect the single coil, requiring workshop access.
- `/install coil`: consume the coil and restore power.

Everything else goes through conversation. The model can request a loan, task agreement,
access, or Patch's following intent. It cannot manufacture items or complete a physical
repair by saying so. Following is a saved intention; this trial has no moving geometry.
Before releasing speech, a narrow phrase-based guard checks several action claims against
their matching authoritative state flags. Unsupported claims trigger bounded retries and
then a safe fallback. Rejected drafts are recorded in trial reports, not NPC memory or TTS.
This catches common false confirmations, including after failed tools, but is not a complete
semantic verifier: unfamiliar wording can escape it and conditional wording can be overflagged.
Known false interlock denials are checked too. A rejected draft retries using just the current
turn and engine telemetry, avoiding stale-state claims in earlier dialogue. Earlier exchanges
remain saved and available on ordinary turns; this recovery request intentionally omits them.

Resident histories are separate and persist in full. The active prompt includes the
last twelve whole exchanges and the latest sixteen public world events. Older dialogue
is retained on disk but is not retrieved yet: do not call this unlimited long-term memory.
Use one driver per save file. A fresh `--save` path starts a new sandbox.

## Repeatable trials

Use a dedicated temporary directory for inputs/results and a fresh save for each run:

```powershell
$trialDir = Join-Path $env:TEMP ("latchpoint-" + [guid]::NewGuid())
New-Item -ItemType Directory $trialDir | Out-Null
python -B examples/games/latchpoint/dialogue_trial.py --grounded --scenario examples/games/latchpoint/player_regression.json --save "$trialDir/quest-save.json" --output "$trialDir/quest-report.json"
python -B examples/games/latchpoint/dialogue_trial.py --scenario examples/games/latchpoint/mischief.json --save "$trialDir/mischief-save.json" --output "$trialDir/mischief-report.json"
```

Optional `--speech-dir <temporary-directory>` synthesizes each reply to a WAV with its
resident's voice and records audio duration and synthesis time. It does not play audio.
Reports include actual replies, tool results, per-request timing, complete turn latency,
and exact-state checks. Nonzero exit means a state check failed. State checks cannot
judge humor, invented speech, or personality: read the transcripts too.

The server buffers native tool envelopes during SSE. Consequently first content can
arrive only when generation finishes on a request carrying tools; reported first-content
time must not be mistaken for raw decoder TTFT. The trial disables thinking explicitly.
`--thinking` enables the reasoning-capable path for comparison. `--json-protocol` selects
the experimental JSON-envelope alternative; it is not constrained decoding and malformed
responses fail without executing an action. Native named tools are the demonstrated path.

Run the deterministic state/save checks without a server:

```powershell
Set-Location examples/games/latchpoint
python -B -m unittest -v test_dialogue_trial.py
```

With both `tts.ready` and `asr_ready` set in server stats, run the speech round-trip:

```powershell
python -B examples/games/latchpoint/voice_trial.py --output-dir "$trialDir/voices"
```

This requires a fresh directory and records three WAVs, recognized text, normalized word
error rate, and request timings. Listen to the clips as well: ASR agreement does not measure
voice appeal, and a transcription error does not identify which speech component caused it.
Run all driver tests with `python -B -m unittest discover -s examples/games/latchpoint -p 'test_*.py'`.

Run the scene's deterministic collision/follow tests:

```powershell
bin/Release/daslang.exe dastest/dastest.das -- --test examples/games/latchpoint/test_world.das --failures-only
```

No website files, browser model download, or microphone capture are part of this milestone.

Kitten Nano speaking rates are configured per resident in `characters.json` via `speech_speed`:
Brack 1.12, Patch 1.6, Vesper 1.2. These tune utterance duration, not inference thread count.
On one common 18-word sentence the selected rates were about 160/147/157 words per minute;
all three selected clips were transcribed exactly by Parakeet after punctuation normalization.
