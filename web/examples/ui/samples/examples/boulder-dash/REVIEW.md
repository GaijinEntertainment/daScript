# Boulder Dash Sample Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture doc:
`ARCHITECTURE.md`.

**Never read the state a tick produced anywhere but between that `cave_tick` call and the
next tick - scoring from `collected`, `play_tick_sfx`, and `cave_consume_input` run
immediately after the tick, in that order.** The per-tick event fields are cleared at the top
of every tick, and consuming input before the tick applies the input one tick late.

**Never size or fill `skip_draw` anywhere but in `mark_movers`, and never call
`draw_static_tiles` in a frame that has not already called `mark_movers`** -
`draw_static_tiles` indexes the mask without a length check.

**A diff that adds or reorders a `Tile` member also updates the `tile_color` and `mat_for`
arms in `main.das`, in the same change** - a tile with no arm renders floor-colored, so it
is invisible.

**A diff that changes a tile family test in `tile_color` or `mat_for` makes the matching
change to the other, in the same change** - a tile matched by one family test and not the
other draws with one family's color and another's material.

**Weakening or deleting a check in `test_cave_rules.das` or `test_sfx_gen.das` is a
defect** - those two suites are the only enforcement of the couplings this checklist names.

**A diff that adds or reorders two moves that share a cell in one tick puts the move whose
SOURCE is the shared cell first** - `move_tile` clears the source cell before writing the
destination, so the reversed order erases the object written there (`try_push`: boulder
first, then player).

**A diff that adds a `Cave` field recording something that happened during a tick also adds
an assignment in `reset_tick_events` and an arm in `play_tick_sfx`, in the same change** -
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
