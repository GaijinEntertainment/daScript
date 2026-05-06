# Pac-Man — daslang-live Example

## Running

```bash
# Live-reload mode
bin/Release/daslang-live.exe examples/games/pacman/main.das

# Standalone mode
bin/Release/daslang.exe examples/games/pacman/main.das
```

## Controls

- **Arrow keys** — move Pac-Man (direction is queued, applied on next available cell alignment)
- **Space** — start game / restart after game over / advance to next level after winning

## Gameplay

Classic Pac-Man rules with several additions:

- Eat all pellets and power pills to clear the level and advance
- 4 ghosts (Blinky, Pinky, Inky, Clyde) patrol the maze with simple targeting AI
- Eat a power pill → ghosts enter flee mode (blue); eat them for 200/400/800/1600 combo points
- Flee blink warning: ghosts flash white during the last 2 seconds of power mode
- **Bonus fruit** spawns below the ghost house at 70 and 130 pellets eaten (cherry=100pts, strawberry=300pts)
- **Difficulty scales per level**: Pac and ghost speeds increase, power duration shrinks each level
- **Scatter/chase cycle**: ghosts alternate 7 s scatter (corners) → 20 s chase, reversing on switch
- **Ghost release stagger**: ghosts leave the house based on a dot counter (0/30/60/90 pellets)
- 3 lives; losing all → Game Over; clearing the maze → next level
- **READY! intro** (2 s) before play begins after each life or level start
- **Slow motion on death**: game slows to 20% speed for the death animation
- **Screen shake**: subtle camera jolt on power-pill pickup, fruit collect, and ghost eat

## Scoring

| Event | Points |
|---|---|
| Pellet | 10 |
| Power pill | 50 |
| Ghost (combo ×1–4) | 200 / 400 / 800 / 1600 |
| Cherry | 100 |
| Strawberry | 300 |

Score popups appear in world space for ghost and fruit pickups.

## Architecture

- **Single file** `main.das` (~1260 lines)
- **Maze**: 21×23 cell grid — `0`=open, `1`=wall, `2`=pellet, `3`=power pill, `4`=ghost house interior, `5`=ghost house door; tunnel wrap handled via column boundary logic (no special cell type)
- **Pellets/power pills**: flat `array<bool>` indexed by `row * MAZE_COLS + col`
- **Ghosts**: DECS entities (`[decs_template] struct Ghost`), drawn via `[decs(stage=draw)]` + `decs_stage("draw")`
- **Pac-Man**: `float2` logical position + smoothed `pac_render_pos` for sub-cell interpolation
- **Bonus fruit**: plain `Fruit` struct (not DECS); 9 s timer; two spawns per level
- **Score popups**: `array<ScorePopup>` — world-space floating text, 1.5 s life
- **Music**: daStrudel patterns, four tracks (menu / play / flee / game-over)
- **SFX**: procedurally generated PCM (`gen_sine_sweep` / `gen_noise_burst`)
- **Rendering**: tilted arcade-cam perspective, Phong shading; spheres for Pac + ghosts, cubes for walls
- **Screen shake**: `shake_timer` / `shake_mag` vars; sinusoidal camera offset decaying over time
- **Slow motion**: `raw_dt` vs `dt`; game logic uses `dt = raw_dt * 0.2` while `pac_dead`; timers (die/ready/end-screen) use `raw_dt`

## Live-reload

All `@live` variables (score, lives, pac_pos, pac_dir, power_timer, level, shake_timer, …) persist across hot-reloads.
Ghost entities are preserved by `decs_live`. Audio handles preserved by `audio_live`.
