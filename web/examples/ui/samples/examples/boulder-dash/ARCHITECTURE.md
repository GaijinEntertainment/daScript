# Boulder Dash — architecture

A web-playground Boulder Dash: `cave.das` is the pure simulation, `cave_gen.das` the
procedural builder, `sfx_gen.das` the pure sound synthesis, `live_stub.das` the playground
shim standing in for the `live/*` host, and `main.das` the orchestration — rendering,
input, audio, music, and the `[live_command]` surface (dead code under the stub). The two
test suites run with `bin/daslang dastest/dastest.das -- --test <file>` from the repo root.

## The tick and its scan order

`cave_tick` scans cells row-major ascending (y outer, x inner). The direction is a rule of
physics, not a habit: gravity is +y, so top-down guarantees a falling object is processed
before the cell it falls into, and `move_tile` marks only destinations LATER in scan order
as `scanned`, so one object is never stepped twice in a tick. Scanning bottom-up would make
a vertical stack of rocks fall as one rigid column instead of separating. After the cell
scan, three whole-grid passes run in order — amoeba, magic, exit — so they see the tick's
settled result, and the diamond collected during the scan opens the exit the same tick.

`move_tile` clears the source cell before writing the destination. When one tick performs
two moves sharing a cell, the move whose SOURCE is the shared cell must run first —
`try_push` moves the boulder, then the player; reversed, the player would be erased.

## The virtual steel boundary

The grid accessors supply every edge test: out-of-bounds reads as `Tile.steel` (objects
settle on the bottom row, insects turn at the edge), out-of-bounds writes are dropped
(`explode_at`'s 3x3 and `fill_rect` need no clipping), out-of-bounds `scanned` reads as
already-handled. `cave_gen` paints a real steel border on top; the virtual wall is the
safety net for ASCII-loaded caves.

## Two clocks

`time_left` burns real wall-clock dt (`cave_apply_time`, per frame). `magic_timer` burns
`1.0 / CAVE_HZ` per tick — it assumes ticks arrive at `CAVE_HZ`. The debug knob
`game_speed` (`cmd_slow_motion`) scales only the tick rate, so away from 1.0 the magic
wall's seconds diverge from wall-clock seconds while the cave timer stays real. Sanctioned:
it is a debug knob, and the divergence is the accepted cost of not scaling the frame clock.

## Input and the predictive slide

Every moving object's animation LAGS the simulation (the grid holds the result;
`draw_movers` shows the trip) — except the player, whose step animation LEADS it. On a
non-tick frame `maybe_start_player_slide` asks `cave_peek_step` where the player would
land, animates toward it over exactly the time remaining until the tick, and raises
`cave_lock_input` so a new direction is parked in `next_dir` rather than invalidating the
committed destination. `cave_peek_step` models only the plain-step branch — its enterable
set must equal `try_step`'s step branch, and a wrong prediction shows as the player
snapping back. The asymmetric contract: a PROMISED step must always land (over-prediction
is the snap-back defect); an unpromised step is tolerated only for tiles the tick itself
transforms — the terminal explosion stages clear mid-tick, so the player can enter them
unpredicted, which renders as a pop instead of a slide. Tap semantics: a tap queues exactly one step; auto-repeat arms only after
the tap's step is consumed AND the key has been held `REPEAT_DELAY` (0.32 s — long enough
that a deliberate tap never repeats, short enough that running does not feel gated).

`sim_playing` is a fixed-step accumulator that never runs two ticks in one frame: surplus
beyond one step is dropped, and dt is clamped to 1/30 s upstream. A stall loses cave time
rather than fast-forwarding the cave through the player; below 30 fps the game clock runs
slow by design.

## Generation

Caves are generated-and-tested, not constructed valid: up to 16 candidates per
`(world_seed, cave_index)` (retry perturbs the seed, not the index), each accepted by
`cave_is_sane` — exactly one player, an exit, a diamond, and `player_survives_idle` for 24
ticks — else the hand-built `fallback_cave`. `harden_spawn` makes the spawn survivable:
dirt-plugs the shafts above the player (turning EMPTY to dirt matters — it blocks later
falls, not just current rocks), clears the 8 neighbors, deletes insects within Chebyshev
distance 8. Generation is a pure function of `(world_seed, cave_index)`; that purity IS the
retry-after-death mechanism — nothing snapshots a cave. Consequently any change to the
paint pipeline's RNG draw sequence reshuffles every cave of every seed.

The ASCII round-trip (`cave_to_ascii`/`cave_from_ascii`) carries the grid only. It doubles
as the deep-copy path for the idle probe (fixed seed — the probe's slime/amoeba rolls are
not the played cave's), and `cmd_load_ascii` runs `finish_cave` on the loaded grid so the
quota and time budget exist and the exit can open.

## Sound

All effects are synthesized at startup; no assets. `gen_sine_sweep` integrates its
frequency ramp (`phase = fs*t + (fe-fs)*t^2/(2d)`), so the named endpoints are the heard
frequencies; a negative end frequency is legal and means the sweep runs through zero and
reflects upward — several bank entries use it deliberately. Every effect ends in a linear
fade to silence; there is no attack ramp, which gives short effects their click transient.
The bank encodes a language: downward sweeps are mass and loss, upward sweeps are value and
gain, duration tracks event weight (per-tick chatter 0.035–0.09 s, once-per-life events
0.25–0.7 s), and the two fanfares sit a fourth apart. `gen_noise_burst` uses a fixed seed —
every explosion is byte-identical. `AUDIO_RATE` sizes the buffers AND is the rate passed to
playback; both live in `sfx_gen` so they cannot diverge.

Music runs on the threaded strudel player and only where the audio backend is threaded;
the game sends colon-separated string commands, one track per named layer.

## Rendering

One shader pair draws everything; per-tile character comes from six material uniforms
(`mat_for`) plus a base color (`tile_color`) — the two switch on the same tile families and
must classify identically. Blending is enabled explicitly each frame (slime is the one
translucent draw). `mark_movers` builds the `skip_draw` mask and is its only resize site,
so it must run before `draw_static_tiles` every frame. The tick-interpolation factor in
`update()` is the exact inverse of `sim_playing`'s step; the two expressions must stay
identical or movers drift.
