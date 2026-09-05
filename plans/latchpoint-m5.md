# M5 bring-up

The branch was pushed and checked out in the clean `/Users/borisbatkin/Work/daScript` tree.
The full configured Ninja Release build completed on M5. Models stay outside Git: the existing
Qwen3-30B-A3B-Instruct-2507 Q4_K_M and Parakeet v3 model were reused; Kitten Nano and its two
front-end packs were copied into the M5 model store.

The initial server config remains CPU-only, eight total lanes and two generation streams.
M5 reports six **Super** and twelve **Performance** cores. The engine already distinguishes
that topology from an efficiency-core tier and phase-shapes its dispatch. An eight-lane cap
does not guarantee reservation of a particular number of Super cores for the game.

## Portability corrections

- Left or right mouse-button dragging controls the camera; either works with a touchpad.
- HUD layout uses logical window coordinates; scene rendering retains framebuffer pixels.
  Scissor rectangles scale between the two, fixing undersized Retina text/panels.
- The atmosphere fullscreen draw completed without error, but its framebuffer blit reported
  GL_INVALID_ENUM on M5 and failed to copy scattering/SSR back into the scene. The pass now
  uses the existing texture-copy path. Lamp rays returned visibly and GL errors cleared.
  The underlying wide blit binding/driver issue has not been isolated to a minimal repro.
- Generic/font GL cleanup no longer mutates vertex-array state with VAO zero bound. A hidden
  core-profile test validates the forbidden-operation control and clean cleanup/font drawing.
- macOS launches `Latchpoint.app` through Launch Services, with a microphone usage description,
  rather than running the game as an SSH child. The first SSH-launched capture contained only
  zero samples, and TCC explicitly denied the SSH-attributed microphone request without a prompt.
  The user must approve the app's microphone request; no permission database edits are made.
- Silent capture and empty ASR replies now produce actionable messages instead of announcing
  a nonexistent transcript. Upload cleanup eligibility is determined in the caller context.
- Brack's drill and scrap plate sit ahead of his tracks, with a jointed arm and vertical
  cutting stroke. Cycle tests check track clearance and cutting-edge/contact alignment.

## Validation and remaining comparisons

All 81 game cases passed on M5; new checks cover either-button dragging, Retina scissor
mapping, core-profile cleanup and silent/empty recognition. All 59 Python reference tests and the
games review gate also passed. Live rendering, chat and TTS were exercised. After the application
relaunch the user confirmed voice works and the game feels substantially more responsive.
A separate Kitten Nano -> WAV -> Parakeet check returned the exact 12-word reference sentence.

Observed rendering is at a 2560x1600 framebuffer for a 1280x800 logical window. No frame-time
benchmark or GPU-inference comparison is claimed by this bring-up.

Next measured comparisons: CPU total/phase limits under the same game scene, then Metal GPU
inference with the same prompts and active rendering. Record response latency plus frame-time
distribution/spikes, including ambient+player overlap. Do not infer hard core affinity from a
thread-count setting, or infer smoothness from an average FPS alone.
