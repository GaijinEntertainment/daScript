# examples/games - architecture

The desktop games in this folder are the single source for the playground samples: the
staged copy under `web/examples/ui/samples/examples/<game>/` is the desktop file set,
verbatim, and `REVIEW.das` beside this document carries the byte-identity check that
keeps it that way.

## 1. One source, two runtimes

A game is a daslang-live app - it requires the `live/*` client modules and the native
`live_host` module for its window, clock, and reload machinery. The wasm playground
runtime links the same native `live_host` and resolves the same `live/` tree, so the
desktop require block compiles there unmodified, with the reload machinery dormant (no
live host attached, `is_live_mode()` false). The main loop is the `eval_main_loop`
builtin - a blocking loop natively, `emscripten_set_main_loop` on the web - and the loop
body owns the GC boundary: `live_end_frame` only swaps buffers.

## 2. Music has two arms

The strudel player runs either on a worker thread (`strudel_init`/`strudel_play`) or on
the main thread (`strudel_create_channel` + per-frame `strudel_tick`). A game picks the
arm once, at audio init: the worker arm needs a real thread and a threaded audio
backend - any native build qualifies, and so does the standalone wasm card (pthread pool
plus a yielding main loop); the wasm playground interpreter does not, and spawning there
parks the whole tab. In the main-thread arm `strudel_command` is a silent no-op (no
command stream exists), so each game routes music commands through its `music_dispatch`,
which calls the handler directly.

## 3. The deployed runtime is the compatibility floor

The identity between the two trees relies on the deployed `daslang_static` linking
`live_host` and embedding the `live/` tree. A runtime regression there breaks every
staged game at once; the nightly dasweb-verify boot is the watchdog.

## 4. The touch pad

On a touch device each game declares its on-screen controls once a frame through
`live/glfw_touch_pad` (`modules/dasGlfw/ARCHITECTURE.md` sec. 2.3): the pad holds the game's own
keys in the virtual key table `glfwGetKey` reads, so the input code stays the keyboard code, and
the game only adds a mapping - Arkanoid puts the paddle under the finger, Pac-Man reads a swipe,
Boulder Dash draws a d-pad and a grab button, River Run a d-pad that also throttles (diagonals
hold both axes) and a fire button.
A corner button pauses and any tap starts or restarts; the prompts read `TAP` where the keyboard
build reads `PRESS SPACE`. The two games that play sideways ask for landscape and draw a rotate
hint in a portrait window. On a desktop `--touch-pad` after the daslang `--` turns the pad on.
In Arkanoid a finger landing launches a held ball and then steers; the F-key look and CRT toggles
get no pad control and stay keyboard-only.

## 5. A narrow window opens the field of view

Arkanoid and Pac-Man frame their playfield for a design aspect - 1024x768 and 900x760 - and hold
that horizontal coverage in a narrower window by opening the vertical field of view in proportion,
instead of cropping the sides. A phone in portrait sees the whole field, taller, not a slice of it.
