# Sequence Board Game — Live Coding Project

## Golden Rule

**Live coding robustness is MORE important than the game itself.** If live-reload breaks at any point, stop everything and fix it before continuing. The game is a vehicle for exercising and stress-testing daslang's live coding infrastructure.

## Game: Sequence

A board/card game where players place chips on a 10x10 board of playing cards by playing matching cards from their hand. First to complete sequences (5 chips in a row) wins.

### Rules Summary

- **Board**: 10x10 grid, each non-Jack card appears twice, 4 FREE corners (wild for everyone)
- **Players**: 1 human + 1/2/3 bots (2, 3, or 4 player modes). No networking.
- **Hand sizes**: 7 cards (2p), 6 cards (3p), 5 cards (4p)
- **Turn**: play a card from hand, place chip on matching board space, draw a card
- **Dead cards**: both board spaces for a card are occupied — discard and redraw
- **Black Jacks** (spade, club) = remove an opponent's chip (not from a completed sequence)
- **Red Jacks** (heart, diamond) = wild, place chip on any open space
- **Win**: 2 sequences (2 players), 1 sequence (3-4 players)
- **No team mode**

### Card Naming (dasCards convention)

Format: `{suit}_{rank}` — e.g. `heart_queen`, `spade_1` (ace), `club_10`
Suits: `club`, `diamond`, `heart`, `spade`
Ranks: `1` (ace), `2`-`10`, `jack`, `queen`, `king`

## File Structure

```
main.das            -- rendering, window, input, animations, sound
                       owns all state, dispatches bot moves via function pointers
gameplay.das        -- module: board model, rules, state machine, win detection
                       PURE LOGIC — no OpenGL, no GLFW, no rendering
bot_random.das      -- random legal move bot
bot_greedy.das      -- position-evaluation bot
bot_heuristic.das   -- configurable weight parameters bot
bot_mcts.das        -- Monte Carlo tree search bot
elo.das             -- module: ELO rating + headless tournament runner
test_gameplay.das   -- tests for gameplay logic (grows each phase)
test_bots.das       -- tests for bot correctness
test_elo.das        -- tests for ELO calculations
```

### Module Boundaries

- `gameplay.das` exports types (`GameState`, `Move`, `ChipColor`, etc.) and pure functions (`make_game`, `legal_moves`, `apply_move`, `check_win`). Zero rendering dependencies.
- `main.das` requires `gameplay`, owns `@live` state variables, handles all rendering, input, and bot dispatch.
- Each `bot_*.das` exports a single function: `def bot_XXX_move(game : GameState; player_idx : int) : Move`
- `elo.das` requires `gameplay`, runs headless bot-vs-bot tournaments.

## Board Layout

The board uses the **original Sequence board card order** (see reference image in PLAN.md context). The layout is a hardcoded 10x10 array in `gameplay.das`. FREE corners at positions (0,0), (0,9), (9,0), (9,9). No jacks appear on the board.

Cards are displayed upright (no rotation) in a 10x10 grid, centered in the window with padding. The board is height-limited in a landscape window, leaving wide margins on both sides for player hands.

## Player Hand Display

Each player's hand is displayed in a screen corner:
- **P1 (human)**: bottom-left — 4 rows sorted by suit (clubs, diamonds, hearts, spades), cards sorted by rank within each row
- **P2**: top-left
- **P3**: top-right
- **P4**: bottom-right

In **cheat mode**, all players show the 4-row suit display. In **normal mode**, bot hands show N face-down cards stacked diagonally at ~20° (just shows card count).

## Live Coding Patterns

- `@live` vars for all tunable parameters (colors, sizes, delays, volumes)
- `require live/live_vars` is REQUIRED for `@live` persistence — without it, vars reset every reload
- `is_reload()` guard on expensive init (GPU resources, deck loading)
- `[live_command]` for debug/test endpoints
- `[before_reload]`/`[after_reload]` only if `@live` isn't sufficient
- Cheat mode: toggle to see all players' hands
- GPU resources (deck, renderer) must be recreated every init — they don't survive reload

## Testing

Every phase adds tests. Run with:
```
bin/Release/daslang.exe dastest/dastest.das -- --test examples/daslive/sequence/test_gameplay.das
```

## Current Status

- **Phase 0**: COMPLETE — CLAUDE.md + PLAN.md written
- **Phase 1**: COMPLETE — Board layout + static rendering
  - `gameplay.das`: module with `BOARD_LAYOUT` (10x10), `ChipColor` enum, jack helpers, `is_valid_card_name`
  - `main.das`: 10x10 upright card grid, auto-scaled with padding, FREE corners as gold-tinted card backs
  - `test_gameplay.das`: 13 tests (all pass) — dimensions, corners, 96 non-free cells, each card x2, no jacks, valid names, bounds
  - `@live` vars: `board_pad`, `bg_color`
  - `require live/live_vars` added for `@live` persistence
  - Note: `require opengl/opengl_boost` must be explicit in `main.das` (not transitively visible from `cards/opengl_cards`)
  - Note: MCP `compile_check` and `run_test` don't process `.das_module` files — use `daslang.exe`/`daslang-live.exe` for full compilation, MCP for gameplay-only tests
  - Note: GPU resources (deck, renderer) don't survive live reload — always recreate in init()
  - Fixed: `live_host_clear_live_vars()` added to dasLiveHost for full reload @live reset
- **Phase 2**: COMPLETE — Chips + hover + click
  - `gameplay.das`: `board_chips` array, `get_chip`, `place_chip`, `clear_all_chips`, `next_chip_color`, `chip_color_to_float4`
  - `main.das`: `screen_to_cell` hit-testing, `handle_mouse` hover+click, chip tinting via `draw_card` tint, `hover_tint` @live var
  - `main.das`: `[live_command]` endpoints: `cmd_place_chip(row, col, color)`, `cmd_clear_board`
  - `test_gameplay.das`: 21 tests (13 Phase 1 + 8 Phase 2) — chip placement, bounds, color cycle, alpha values
  - Note: chips rendered as card tint overlay (no separate geometry), click cycles through blue→green→red→yellow→none
  - Note: `require daslib/json_boost` needed for `from_JV` in live commands, `require glfw/glfw_boost` for mouse input
- **Phase 3**: NOT STARTED — Game state + turn structure
