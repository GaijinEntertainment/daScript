# Port convergence: one source for desktop games and playground samples

Goal (ruled 2026-08-28): the game sources under `examples/games/<g>/` and the playground
bundles under `web/examples/ui/samples/examples/<g>/` become byte-identical, with the
`examples/games/REVIEW.das` identity gate keeping them that way (ratified as the endgame -
the copies stay in git; deploy-time staging was considered and rejected, see phase 3).

## Why this is nearly free (probed, deployed runtime)

- The deployed wasm32 `daslang_static` links the native `live_host` module and resolves
  the whole `live/` tree. Desktop `rr_globals.das` and `rr_postfx.das` - full `live/*`
  require block, `live_host` and all - compile and run VERBATIM in the playground
  (proven in local staging against the deployed runtime, in-game). No stubs, no
  guarded requires needed. `live_stub.das` is obsolete.
- `rr_live.das` requires only game modules; `live/live_commands` resolves in the wasm
  runtime, so `[live_command]` functions compile there as dead registry entries.
  (`live_commands`' own `require live_host` is vestigial - compiles without it.)
- Reload machinery is dormant in the playground: `is_live_mode()` false, no host.

Two real deltas remain, both fixable in shared source:

1. **Main loop.** The ports use the `eval_main_loop` builtin (blocking loop natively,
   `emscripten_set_main_loop`/rAF on the web); the desktop mains use a blocking
   `while (!exit_requested())`. Unify on `eval_main_loop` everywhere. Reconcile the
   `maybe_collect_gc()` difference (the port dropped it because `live_end_frame`
   already collects).
2. **Music.** Desktop `rr_audio.das` verbatim FREEZES the playground tab: the game music
   path uses the threaded strudel arm (`strudel_start` -> `new_thread`), which locks the
   wasm main thread. The playground's own `audio_strudel.das` sample proves the
   single-threaded arm works there (`strudel_create_channel` + per-frame `strudel_tick`
   on the main thread). RULED: converge on a single-threaded fallback - threaded
   `strudel_start` where available, channel+tick otherwise - so the playground gets
   music instead of a gate. This replaces both the port's `is_standalone_exe()` hack
   and the desktop-only `!audio_is_single_threaded()` gate; the "threaded available"
   predicate needs a probe (candidates: jobque worker count, audio backend query, or a
   small native capability query).

## Phases

1. **river_run pilot** (after PR #3891 lands - it touched the port copies):
   `eval_main_loop` in both mains; the music fallback arm in `rr_audio.das`; desktop
   require blocks verbatim; delete `live_stub.das` from the bundle; update the bundle
   file list (`data.json`, `_interp.html`) to the desktop file set (+`rr_live.das`).
   Validate: native run, playground drive (staging recipe below), Playwright suite.
2. **arcanoid, pacman, boulder-dash**: same treatment. Their `main.das` deltas are the
   same classes (loop, music gate, nolint-comment drift) plus boulder-dash's port-only
   files (`sfx_gen.das`, its dastest suites) - decide per file: fold desktop-side or
   charter as sanctioned port-only.
3. **Ban the fork re-forming** (amended from "kill the copy"): the copies stay in git
   but `examples/games/REVIEW.das` gains `check_port_identity` - every `.das` under a
   samples dir with a desktop twin must be byte-identical to it, in both directions.
   True stage-from-examples (deleting the git copies, staging at deploy time) was
   REJECTED (ruled 2026-08-29): it rewires pages.yml, which runs only on master pushes -
   a mistake there breaks the live site with no PR-time signal - and the copy step the
   gate polices is trivial.

## Traps and notes for the doer

- Guarded requires are NOT needed here, but for the record (ast_parse.cpp:254): a
  plain-name guard (`require ?live_host X`) tests for a REGISTERED C++ module; a
  path guard (contains `/`) tests that the guard's own file resolves. The deployed
  runtime's lexer fuses `?.` into the safe-navigation token - spell `? ./x` if ever
  needed.
- Native compile checks of the samples tree during transition: once the bundle files
  are the desktop files, compiling them natively is fine; while `live_stub.das` and a
  desktop require block coexist in one dir, both arms would load natively - sequence
  each game's bundle swap atomically.
- Local playground staging that works (the local wasm32 build is still broken for GL):
  fetch deployed `daslang.io/playground/daslang_static.{js,wasm}` over `site/playground/`,
  `rsync web/examples/ui/samples/ site/playground/samples/`, serve `site/` on 8765,
  drive `?example=<g>` headless (run button `#run`, output `#output`, canvas in the
  `run-frame.html` iframe).
- The deployed runtime is the compatibility floor: convergence relies on it linking
  `live_host` + embedding `live/`; a runtime regression there breaks every converged
  bundle at once. Phase 3's verifier (dasweb-verify nightly boot) is the watchdog.
