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
  - Note: chips rendered with custom GL shader (GL_TRIANGLE_FAN fill + GL_LINE_LOOP outline), 15% card tint, 90% opaque chips
  - Note: `require daslib/json_boost` needed for `from_JV` in live commands, `require glfw/glfw_boost` for mouse input
- **Phase 3**: COMPLETE — Game state + turn structure
  - `gameplay.das`: `GameState` struct (hands, draw pile, discard, current player, phase, board_chips, sequences), `Move` struct, `GamePhase` enum
  - `gameplay.das`: All functions take `GameState` as first parameter (no module global) — enables bot state cloning
  - `gameplay.das`: `make_game(game, num_players, seed)`, `build_double_deck()`, `shuffle_deck()`, `deal_hands()`
  - `gameplay.das`: `legal_moves(game, player_idx)`, `apply_move(game, player_idx, move)`, `is_dead_card(game, card)`, `discard_dead_cards(game, player_idx)`
  - `gameplay.das`: Jack logic — black jacks remove opponent chips (not from completed sequences), red jacks wild placement
  - `gameplay.das`: `set_hand()`, `sort_hand()`, `card_suit()`, `card_rank()`, `suit_order()`, `rank_order()`
  - `main.das`: `draw_hand_by_suit()` — 4 rows (one per suit), sorted by rank, with click-to-select for human
  - `main.das`: `draw_hand_stack()` — diagonal stack of face-down cards for bot hands (non-cheat mode)
  - `main.das`: Hand layout — P1=bottom-left, P2=top-left, P3=top-right, P4=bottom-right, using board scale
  - `main.das`: Card selection + play-by-click on board cells, turn advancement, right-click deselect
  - `main.das`: Green/red hover tint for legal/illegal moves when card selected, 50% darkening of non-selected cards
  - `main.das`: `[live_command]` endpoints: `cmd_new_game`, `cmd_game_status`, `cmd_toggle_cheat`, `cmd_set_hand`, `cmd_debug_hands`
  - `main.das`: `@live` vars restored with `require live/live_vars` — per-variable keys with init-expression hash detection
  - `test_gameplay.das`: 44 tests (21 Phase 1+2 + 23 Phase 3) — deck building, hand sizes, draw pile, game state, card validity, deterministic shuffle, suit/rank parsing, hand sorting, legal moves, apply_move, wrong player rejection, dead card detection, player chip colors, full game simulation, is_legal_move validation
  - Note: `cmd_set_hand` auto-sets phase to `playing` if game not started, for standalone testing
  - Note: Fixed array dimension ordering — `string[MAX_PLAYERS][MAX_HAND_SIZE]` (first index selects player)
  - Note: `live_vars.das` redesigned — each `@live` var gets own key with init-expression hash; changing default in code takes effect on normal reload
- **Phase 4**: COMPLETE — Win detection + sequence highlighting
  - `gameplay.das`: `Sequence` struct (5 cells + color), stored in `GameState.sequences`
  - `gameplay.das`: `find_sequences(game)` — scans all 4 directions (horizontal, vertical, diagonal, anti-diagonal), FREE corners wild for all players, sequences can share at most one cell
  - `gameplay.das`: `check_win(game)` — returns winner player index or -1; 2 sequences for 2p, 1 for 3-4p
  - `gameplay.das`: `count_sequences(game, color)`, `sequences_to_win(num_players)`, `cell_matches_color()`, `is_in_sequence()`
  - `gameplay.das`: Black jack protection — `is_legal_move`, `legal_moves`, `has_legal_moves`, and `apply_move` all reject removing chips from completed sequences
  - `gameplay.das`: `apply_move` calls `find_sequences` + `check_win` after each chip placement; sets `game_over` phase and `winner` on win
  - `main.das`: Sequence chips pulse (brightness + white outline + slight size increase) using `sin(uptime * speed)`
  - `main.das`: Game over overlay — large pulsing winner-colored chip in center; hands remain visible
  - `main.das`: `[live_command]` `cmd_check_sequences` — manually finds sequences and reports them with winner detection
  - `main.das`: `@live` vars: `seq_pulse_speed`, `seq_pulse_min` for tuning pulse animation
  - `test_gameplay.das`: 55 tests (44 Phase 1-3 + 11 Phase 4) — horizontal/vertical/diagonal/anti-diagonal detection, FREE corner inclusion, 4-not-enough, overlapping sequences share one cell, win conditions for 2p and 3p, black jack can't remove from sequence, apply_move triggers game over
- **Phase 5**: COMPLETE — Random bot
  - `bot_random.das`: module with `bot_random_move(game, player_idx, seed)` — picks uniformly random legal move using simple LCG
  - `main.das`: Bot auto-play — player 0 is human, players 1+ are bots. After human move, bot plays after `bot_delay` timer
  - `main.das`: `@live bot_delay = 0.5` — tunable seconds between bot moves
  - `main.das`: `bot_timer`, `bot_rng_seed` state for bot turn scheduling
  - `main.das`: `discard_dead_cards` called before each bot move
  - `main.das`: `[live_command]` `cmd_bot_move` — manually trigger one bot move for current or specified player
  - `main.das`: `[live_command]` `cmd_force_move`, `cmd_set_phase`, `cmd_set_player`, `cmd_reset_board` — debug commands
  - `test_bots.das`: 14 tests — legal moves, different seeds, jack handling (red/black), no-moves case, 100-move stress test, full game simulation
- **Phase 6**: COMPLETE — Animations
  - `main.das`: Animation queue system — `Anim` struct with type, position, progress, duration, deferred move
  - `main.das`: `AnimType.chip_place` — bounce overshoot (0→1.2→1.0 scale), `AnimType.chip_remove` — flash+shrink (1.0→1.3→0)
  - `main.das`: `queue_animated_move()` — queues animation and defers `apply_move` until animation completes
  - `main.das`: Both human and bot moves go through animation queue; input blocked during animation
  - `main.das`: `@live anim_chip_duration`, `@live anim_remove_duration` for tuning
  - `main.das`: `@live game` — GameState now persists across live reloads (required archive fix for multi-dim fixed arrays)
  - `daslib/archive.das`: Added `serialize` overloads for 2D through 6D fixed arrays (`auto(TT)[][]`, etc.)
  - `tests/archive/test_archive.das`: 7 new tests — 2D int, 2D string, 3D int, struct with 2D array
  - `test_anim.das`: 17 tests — chip_place scale curve (start/overshoot/settle/monotonic/clamp), chip_remove scale (start/flash/shrink/non-negative), chip_remove alpha (opaque/transparent/monotonic/clamp)
