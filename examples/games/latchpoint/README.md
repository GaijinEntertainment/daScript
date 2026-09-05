# Latchpoint local sandbox

A small industrial yard with Brack, Patch and Vesper: exploration, salvage, repair and
free-form robot conversations. The game runtime is daslang, including authoritative state,
saves, dialogue, audio and input. It calls a separate local dasllama-server through dasOPENAI;
the model never owns game state. This is a local prototype, not a browser release.

The older Python experiments are preserved on `archive/latchpoint-python-experiments`.
The [archive guide](../../../plans/latchpoint-python-experiments.md) describes their contents,
limitations and proposed daslang port. Historical `plans/latchpoint*.md` reports describe
individual development passes; their Python commands belong to the archived checkout.

Design: [Latchpoint](../../../plans/latchpoint.md).
Known issues: [playtest follow-ups](../../../plans/latchpoint-playtest-followups.md).
Mac bring-up: [M5 validation](../../../plans/latchpoint-m5.md).

## Build and launch

Build this checkout's runtime and shared modules together, with dasLLVM, dasHV, dasAudio,
dasMinfft, dasGlfw and dasStbImage available. Do not borrow modules built against another
checkout's headers. From a configured Windows Release build:

```powershell
cmake --build build --config Release --target daslang daslang-live dasModuleHV dasModuleAudio dasModuleMinfft dasModuleGlfw dasModuleStbImage -j 16 -- /nodeReuse:false
```

Copy `server.example.toml` to ignored `server.local.toml` and set your local model paths.
The example paths describe a development box, not portable defaults. The game requests
the alias `qwen3-30b`; playtests used Qwen3-30B-A3B-Instruct-2507 Q4_K_M.
For speech, configure Kitten Nano with `tts_g2p.bin` and `tts_postag.bin` beside its GGUF;
`characters.json` selects its resident voices and speaking rates. Configure Parakeet ASR
for voice input, not an audio tower in Qwen. Put global `tts`/`asr` settings above `[[models]]`.

```powershell
powershell -NoProfile -File examples/games/latchpoint/start-server.ps1
powershell -NoProfile -File examples/games/latchpoint/start-game.ps1
```

On macOS/Linux, build the checkout's runtime/modules, then:

```sh
bash examples/games/latchpoint/start-server.sh
bash examples/games/latchpoint/start-game.sh
```

Wait for `http://127.0.0.1:18082/v1/stats` to report the configured model, `tts.ready`
and `asr_ready` before testing all voice features. Cold compilation/model preparation can
take minutes. Server logs are under `logs/latchpoint-server/`; game logs are under
`logs/latchpoint-scene/`. The server launcher uses the repository's Python watchdog.

On macOS the launcher creates an ad-hoc signed development app at `bin/Latchpoint.app`
and opens it through Launch Services. Allow microphone access when prompted after pressing V.
A binary launched directly under SSH can receive silence because macOS attributes capture to
SSH and refuses a prompt. The launcher does not modify the privacy database.

The example config uses eight CPU lanes, two generation streams and GPU inference off.
The Windows launcher also pins CPU inference through environment settings. The Unix launcher
honors its config; M5 was tested with both CPU and Metal. A worker-lane limit is not a total
OS-thread cap or a guarantee of reserving particular CPU cores. The server can listen beyond
loopback: this is local development, not an authenticated public deployment.

Closing the game leaves inference running. Stop the server and watchdog with:

```powershell
Invoke-RestMethod -Method Post http://127.0.0.1:18082/shutdown
```

## Controls and objectives

- WASD or arrows: move; Left/Right strafe. Either Shift: hurry. Left or right click-drag: look.
- E near a robot: type. Enter or numpad Enter: send. Escape: cancel typing.
- Hold V near a robot: record up to 20 seconds. Release to transcribe, edit, then Enter to send.
  Escape discards a recording. Capture never starts automatically.
- F near a component: salvage/install it. Code checks parts, location and prerequisites.
- After installing the coil, hold F at the generator: nine seconds manually, three with
  repaired/following Patch at the clutch. Releasing F resets progress; assistance is optional.
- F at the service plate inside the opened archive: inspect the record and complete the
  short discovery. Free exploration continues without a forced ending screen.
- Escape outside text entry: New Game / Continue. Click, or use Up/Down and Enter.
- R: reread saved world state when no request is pending.
- L: toggle storm flashes. F6: reflections. F7: dust and light scattering.
- F8/F9/F10: toggle music/world effects/speech. P: robot voice coloration for the next line.
- F11: toggle future local conversation logging. F12: ambient robot chatter.

New Game clears progress and resident conversations. Continue restores progress, conversations
and the avatar/Patch position checkpoint in `saves/scene-pose.json`. World state is in
`saves/scene.json`. Keep archived trial drivers away from these saves.
Holographic landmarks identify WORKSHOP 01, GENERATOR 02, ARCHIVE 03 and SALVAGE 04.
The coil is on the workshop bench; consumed salvage no longer offers an interaction prompt.

The world moves while a robot responds. Repaired Patch can follow and help start the generator.
Gate geometry travels before collision permits passage. Movement is ground-plane only, with
no jumping or combat. Patch uses local collision-aware steering, not a navigation mesh, and
can get stuck behind obstacles.

## Conversations, sound and privacy

A model classifies the player request, code validates actions, practical replies use current
facts, and ordinary chat uses a speech-only model request. Classification and conversational
hallucinations remain prototype limitations. The archived multi-request verifier is not
connected to the game. Resident histories persist separately; prompts use recent exchanges,
not unlimited long-term retrieval.

dasAudio supplies positional voices, robot cues, motors, wind and delayed thunder. daStrudel
supplies the synthesized score. Brack drills a scrap plate with contact-only grinding/sparks;
Patch sweeps an inspection lamp; Vesper consults records. Optional background observations
use a separate client job, stale-state checks and no gameplay tools, and yield to E/V input.

E interrupts speech to start a reply. Escape interrupts speech or discards a pending inference
result and opens the menu; validated actions are not undone. dasOPENAI runs off the main thread
with a 90-second player-chat timeout and 25-second ASR/TTS timeout. Interrupted requests drain
before another player request starts. Push-to-talk has no VAD endpointing or echo cancellation.

Submitted conversations, intents, receipts, replies and snapshots are logged locally under
`logs/latchpoint-playtests/native-runtime-v1.jsonl`. Tell testers their text is recorded
and avoid personal information. Nothing is uploaded automatically. F11 stops future traces;
New Game clears game memories but retains logs. Review/redact traces before sharing, or delete
the selected log to remove it. Temporary WAVs are under `logs/latchpoint-input/` and are
deleted when transcription returns. Audio goes to the configured loopback server.

## Rendering and live development

Rendering uses OpenGL with River Run shadow/post-processing modules, a twilight sky, twelve
local lights, a shadowed swinging lamp, a static yard reflection probe, bounded screen-space
reflections, dust and scattering. Procedural roughness/metalness/normal detail and floor
parallax complement beveled geometry. No bitmap texture assets or duplicated River Run sources
are required. Keep this checkout layout; standalone/browser packaging is future work.

The launcher disables the front-end module cache following an observed late-bound OpenGL
failure after cached compilation. Normal live reload still works. Use `live_*` MCP tools on
port `19091`; the host auto-reloads watched source changes, including compile-error recovery.
`cmd_scene_state`, `cmd_pose`, `cmd_talk` and `cmd_interact` are developer inspection controls.
Built-in key/mouse commands and screenshots exercise actual input and framebuffer paths.

`cmd_render_settings` accepts `material` (0 lit, 1 roughness, 2 metalness, 3 normals), `bloom`,
`ao` and `time` (-1 realtime). `trace_errors=true` enables per-pass GL error capture in
`cmd_hdr_check`. Readback and tracing are diagnostics, not frame-time benchmarks;
disable them for performance comparisons.

## Tests

Run a native test file from the repository root, for example:

```powershell
bin/Release/daslang.exe dastest/dastest.das -- --test examples/games/latchpoint/test_world.das --failures-only
```

The `test_*.das` files cover world state, conversations, input, audio processing, geometry and
render-state contracts. `test_core_gl.das` needs a working OpenGL context. Real-model dialogue,
microphone permissions, voice appeal and perceived rendering smoothness also need playtesting.
