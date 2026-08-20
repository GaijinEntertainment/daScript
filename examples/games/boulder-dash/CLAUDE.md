# Boulder Dash — daslang-live Example

Tribute clone of classic Boulder Dash cave physics. Not original First Star cave data.

## Running

```bash
# Live-reload mode
bin/Release/daslang-live.exe examples/games/boulder-dash/main.das

# Standalone
bin/Release/daslang.exe examples/games/boulder-dash/main.das

# Headless smoke
bin/Release/daslang.exe examples/games/boulder-dash/main.das -- --max-frames 3

# Physics / generator tests (no GLFW)
bin/Release/daslang.exe dastest/dastest.das -- --test examples/games/boulder-dash/test_cave.das
bin/Release/daslang.exe dastest/dastest.das -- --test examples/games/boulder-dash/test_generate.das
```

## Controls

- **Arrow keys** — one tap is one cell. Motion starts on press and lands on the next cave tick. A new direction after motion has started waits for the following cell. Hold ~0.3s to keep walking.
- **Z** or **Ctrl** — grab: dig/collect the adjacent tile without stepping (also queued)
- **Space** — start / resume after game over / skip cave-cleared pause
- **Escape** — pause / unpause

## Gameplay

- 40×22 cave, 8 Hz scan-line physics, movers interpolated between ticks
- Dirt, boulders, diamonds, brick, steel, fireflies, butterflies, amoeba, magic wall, expanding wall, slime
- Collect the quota to open the exit; timer kills; 3 lives
- Caves are generated from `(seed, cave_index)` — Liepa-style random fill plus rectangular patches

## Architecture

- `cave.das` — tiles, scan tick, ASCII I/O (no GLFW)
- `cave_gen.das` — seeded generator
- `main.das` — GLFW/OpenGL, audio, HUD, live commands
- `test_*.das` — headless dastest

ASCII legend lives next to `TILE_CH` in `cave.das`.

## Live commands

`cmd_game_status`, `cmd_reset_game`, `cmd_set_seed`, `cmd_next_cave`, `cmd_prev_cave`, `cmd_god_mode`, `cmd_set_time`, `cmd_set_diamonds`, `cmd_dump_cave`, `cmd_load_ascii`, `cmd_tick`, `cmd_slow_motion`, plus built-in `screenshot` / `help`.
