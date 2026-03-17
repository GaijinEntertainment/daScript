# Arcanoid — daslang-live Example

## Running

```bash
# Live-reload mode (recommended — hot-reload on save, REST API on port 9090)
bin/Release/daslang-live.exe examples/daslive/arcanoid/main.das

# Standalone mode (no live reload, no REST API)
bin/Release/daslang.exe examples/daslive/arcanoid/main.das
```

Both modes use the same `main.das`. Under `daslang.exe`, the script's `main()` function runs the init/update/shutdown loop directly. Under `daslang-live.exe`, the host manages the lifecycle and enables hot-reload + REST API.

## REST API (live mode only)

Server starts automatically on **port 9090** when running under `daslang-live.exe`.

### Built-in Endpoints

```bash
curl http://localhost:9090/status                    # fps, uptime, paused, dt
curl http://localhost:9090/error                     # last compilation error (plain text)
curl -X POST http://localhost:9090/reload            # trigger reload
curl -X POST http://localhost:9090/pause             # pause
curl -X POST http://localhost:9090/unpause           # unpause
```

### Commands

All commands via `POST /command` with JSON body `{"name": "...", "args": {...}}`.

```bash
# List all available commands
curl -X POST http://localhost:9090/command -d '{"name":"help"}'
```

#### Built-in Commands (from modules)

| Command | Description | Args |
|---|---|---|
| `help` | List all commands | — |
| `screenshot` | Save framebuffer to PNG | `file` (default: "screenshot.png") |
| `cmd_watch_status` | Show watched files with stat info | — |
| `cmd_watch_touch` | Touch watched file to trigger reload | — |
| `cmd_watch_signal` | Signal files changed (bypass watcher) | — |

#### Game Commands (from `[live_command]` in main.das)

| Command | Description | Args |
|---|---|---|
| `cmd_game_status` | Score, lives, ball/brick counts, game state | — |
| `cmd_spawn_ball` | Spawn a ball | `x` (float), `vx` (float) |
| `cmd_spawn_bricks` | Spawn a new set of bricks | — |
| `cmd_spawn_bonus` | Spawn a bonus at paddle | `type` (string) |
| `cmd_powerup` | Activate a powerup directly | `type` (string) |
| `cmd_reset_game` | Reset game to initial state | — |
| `cmd_set_lives` | Set lives count | `lives` (int) |
| `cmd_god_mode` | Toggle god mode (ball never falls) | — |
| `cmd_slow_motion` | Set game speed multiplier | `speed` (float, 0.1=slow, 1=normal, 2=fast) |
| `cmd_decs_dump` | Dump DECS archetypes/entities | — |

**Powerup/bonus types:** `triple_ball`, `wide_paddle`, `narrow_paddle`, `sticky_paddle`, `fireball`, `extra_life`, `speed_up`, `speed_down`

### Example curl Commands

```bash
# Check game state
curl -X POST http://localhost:9090/command -d '{"name":"cmd_game_status"}'

# Spawn a ball at center with slight rightward velocity
curl -X POST http://localhost:9090/command -d '{"name":"cmd_spawn_ball","args":{"x":0,"vx":0.3}}'

# Activate wide paddle
curl -X POST http://localhost:9090/command -d '{"name":"cmd_powerup","args":{"type":"wide_paddle"}}'

# Toggle god mode
curl -X POST http://localhost:9090/command -d '{"name":"cmd_god_mode"}'

# Slow motion
curl -X POST http://localhost:9090/command -d '{"name":"cmd_slow_motion","args":{"speed":0.3}}'

# Take a screenshot
curl -X POST http://localhost:9090/command -d '{"name":"screenshot","args":{"file":"arcanoid.png"}}'

# Reset the game
curl -X POST http://localhost:9090/command -d '{"name":"cmd_reset_game"}'
```

## Architecture

- **ECS**: Uses DECS (daslang ECS) for balls, bricks, bonuses, particles, trails
- **Rendering**: OpenGL 3.3 with vertex/fragment shaders, planar shadows, HUD text
- **Audio**: Procedurally generated sound effects via `audio_boost`
- **State preservation**: Full persistence across live reloads — game keeps running seamlessly
  - `decs_live` serializes/restores all DECS entities (bricks, balls, bonuses, trails)
  - `[before_reload]`/`[after_reload]` save/restore globals: paddle_x, score, lives, game_state, powerup timers, sticky state, god_mode, game_speed, audio handles
  - `init()` guards `decs::restart()` + `spawn_bricks()` with `is_reload()` so entities aren't wiped on reload

## Keyboard Controls

- **Arrow keys**: Move paddle left/right
- **Space**: Start game / launch ball / release sticky ball / restart after game over
- **Escape**: Pause / unpause

## Game States

`GameState.menu` → `GameState.playing` → `GameState.game_over_state` or `GameState.win_state`

Pause available during `playing` via Escape.

## File Structure

Single file: `main.das` (~1250 lines). Contains everything: shaders, audio generation, game logic, rendering, HUD, live commands, state preservation.
