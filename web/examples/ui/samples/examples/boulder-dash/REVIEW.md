# Boulder Dash Sample Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture doc:
`ARCHITECTURE.md`.

**Everything that reads a tick's outgoing state sits between `cave_tick` and the next tick**
 - scoring from `collected`, `play_tick_sfx`, and `cave_consume_input` run immediately after
the tick, in that order. The per-tick event fields are cleared at the top of every tick, and
consuming input before the tick applies the input one tick late.

**`skip_draw` is sized and filled only in `mark_movers`, and `mark_movers` runs before
`draw_static_tiles` on every frame** - `draw_static_tiles` indexes the mask without a
length check.

**A `Tile` member added or reordered updates the `tile_color` and `mat_for` arms in
`main.das` in the same change** - a tile without those arms renders floor-colored, i.e.
invisibly.

**`tile_color` and `mat_for` classify every tile identically** - a tile matched by one
family test and not the other draws with one family's color and another's material.

**Weakening or deleting a check in `test_cave_rules.das` or `test_sfx_gen.das` is a
defect** - those two suites are the only enforcement of the couplings this checklist
names, and relaxing one silently un-enforces a rule above.

**When one tick moves two objects through a shared cell, the move whose SOURCE is the
shared cell runs first** - `move_tile` clears source before writing destination, so the
reversed order erases the object written there (`try_push`: boulder first, then player).

**A new `Cave` field that records something that happened during a tick gains an
assignment in `reset_tick_events` and an arm in `play_tick_sfx` in the same change** - 
missing the first latches the event across ticks; missing the second makes it silent.

**A diff that adds, removes, or reorders a random draw on `generate_cave`'s path - 
`paint_interior` and everything it calls, plus `finish_cave` - says in the PR that it
reshuffles every cave of every seed.** Generation is a pure function of
`(world_seed, cave_index)`, and that purity is the retry-after-death mechanism
(ARCHITECTURE.md sec. Generation).

**Placement - one file, one line: a diff keeps each file inside its line, and a new file
adds its line here, with its tests, in the same change.**

- `cave.das` - the pure simulation: grid, tiles, tick, input latch. Zero rendering, zero IO.
- `cave_gen.das` - procedural builder and its sanity/repair passes; finishes every
  returned cave with `finish_cave`.
- `sfx_gen.das` - pure PCM synthesis; owns `AUDIO_RATE`/`AUDIO_CHANNELS`. Zero playback.
- `live_stub.das` - playground shim for the `live/*` host. Zero game logic.
- `main.das` - orchestration: rendering, input polling, audio playback, music, live commands.
- `test_cave_rules.das` - the cave-simulation dastest suite.
- `test_sfx_gen.das` - the sound-synthesis dastest suite.
- `ARCHITECTURE.md` - design rationale for this folder.
- `REVIEW.md` - this file.
- `LAWS.md` - append-only intent provenance (CLAUDE.md sec. "Boris's rulings get a `LAWS.md`
  sidecar", repo root). Never groomed.
