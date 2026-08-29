# The wasm browser lifecycle has no GC boundary

Every `options gc` program driven by the browser three-call lifecycle grows its heap
without bound until the wasm 2 GiB ceiling, then traps. Nothing collects.

## The defect

`maybe_collect_gc` (`modules/dasLiveHost/live/live_gc.das:15`) states the contract in its own
doc-comment: the canonical shape - `init`/`update`/`shutdown` plus a standalone `main` loop -
calls it once per iteration, and under the daslang-live host it self-disables because the host
collects each frame. So there are exactly two sanctioned GC drivers: the program's own `main`
loop, or the live host.

The browser lifecycle is a third driver, and it was never given one.

- `skills/daspkg.md:351` - on wasm the browser lifecycle auto-drives `init`/`update`/`shutdown`
  per `requestAnimationFrame` and **bypasses `main`**. Intended: a game keeps its plain desktop
  `main`, with no per-platform edits.
- There are THREE browser-lifecycle drivers, and two of them collect nothing:
  - `utils/daslang/main.cpp:338` `web_loop_tick` - the interpreter-in-wasm path, which is what
    the **playground** runs. Evaluates `update()`, reads its return value for loop control,
    reports exceptions. No collection.
  - `src/builtin/module_jit.cpp:1845` `jit_web_lifecycle_tick` - the standalone cross-compiled
    exe path, which is what an **examples card** runs (`--jit-target=wasm64`). Same shape, same
    omission. This is the driver the measurement below actually exercised.
  - `src/builtin/module_builtin_runtime.cpp:1948` `main_loop_arg` - `eval_main_loop`'s
    emscripten arm, used by the ported games. This one is FINE, and it shows why: it invokes a
    daslang block, and those blocks call `maybe_collect_gc()` themselves
    (`examples/games/pacman/main.das:1312`). The GC call survived there because it stayed in
    das; it was lost in exactly the two drivers that replaced `main` from the C++ side.

  The fix therefore has two sites, not one, and they must be verified separately - a card and
  the playground are different code paths, so confirming one says nothing about the other.
- `is_live_mode()` is false in a card, so `maybe_collect_gc` would not self-disable - it is
  simply never reached, because the `main` loop that calls it never runs.
- `options gc` + `options persistent_heap = true` is required for `heap_collect` to work at
  all, so these programs are explicitly collect-on-demand, and nothing demands.

## Evidence (physarum wasm64 card - the jit_web_lifecycle_tick driver, local, 2026-08-29)

Heap growth is monotonic and geometric (emscripten grows ~1.2x per step):

    72.5 -> 87 -> 104.5 -> 125.4 -> 150.5 -> 180.7 -> 216.9 -> 374.9 -> 449.8 MB

Roughly 1.2 MB/s, reaching the 2 GiB ceiling in about half an hour, then:

    Cannot enlarge memory, requested 2147766880 bytes, but the limit is 2147483648 bytes!
    out of heap memory: requested 524288 bytes

repeating for thousands of messages until the module traps. After the trap the page answers
nothing - even `Module.wasmMemory` throws `RuntimeError: unreachable` - the canvas is frozen
mid-frame and the HUD holds its last values. That is what the sample verifier reports as a
wedge, and it is the standing nightly red for Physarum Lab.

Growing the heap is synchronous on the main thread, so a single grow near the ceiling is a long
stall. A user report of two ~945 ms main-thread freezes in `HandlePostMessage` -> `RunMicrotasks`
matches two late grows better than any dispatch-side explanation.

## Scope

Not lab-specific and not dispatch-specific. Every `options gc` program the examples pipeline
drives through `web_loop_tick` is affected, which is every graphics card. Samples that finish
quickly never reach the ceiling, so this reads as "long-running cards wedge" rather than as a
systematic defect - which is why it survived.

## The fix

One collection boundary in the browser tick, matching what the desktop loop body does. Open
questions to settle before writing it:

1. **Where.** In `web_loop_tick` after the `update()` eval, or inside the harness. The tick is
   the honest home: it is the thing that replaced the `main` loop, so it should carry the
   `main` loop's duty.
2. **What.** `maybe_collect_gc`'s thresholds (string heap over 1/3 unused, heap over 2/3
   unused) exist so a per-frame call stays cheap - it collects only a mostly-free heap. The
   C++ side should apply the same policy rather than collecting unconditionally, or call the
   das function when the program exposes it.
3. **Programs without `options gc`.** `heap_collect` throws without `options gc` +
   `options persistent_heap`, so the boundary must be a no-op for programs that do not opt in.
   Check the flags on the context, not the presence of a function.
4. **Cost.** Verify the collection does not itself introduce a frame stall on a large live
   heap; the thresholds are meant to prevent that, but they have never run per-frame on a card.

## The desktop control (run 2026-08-29)

The same source on desktop, where the program's own `main` loop calls `maybe_collect_gc()`
every frame, holds flat: RSS 419 MB across a three-minute run, `STAT RN` at 131.8% CPU
throughout, so the process was working rather than stalled. That rules out the one hypothesis
that would have made the missing boundary irrelevant - if the growth were LIVE data, collection
would reclaim nothing and desktop would climb too. It does not. The heap is collectable garbage,
and the only difference is who calls the collector.

## Verification

- A card that reproduces the growth today (physarum is the fastest) shows a flat or sawtooth
  heap instead of a monotonic ramp, over a soak long enough to have OOM'd before - 45 minutes
  at the measured rate gives margin.
- The playground arm measured separately, since it is the other driver. Physarum in the
  interpreted playground is the same program through `web_loop_tick`; it should show the same
  ramp before the fix and the same flattening after.
- The nightly playground sweep's Physarum Lab cell goes green and stays green.
- A no-`options gc` card still runs, proving the no-op path.
- Desktop is unaffected: its `main` loop keeps calling `maybe_collect_gc` itself.

## Not yet known

Whether the leak rate differs between the per-frame `new_thread` dispatch and the persistent
job queue. A control run was attempted and was not usable: it was instrumented with
`-sPTHREADS_DEBUG=1`, which emits about four lines per thread, and at ~464 threads/s the arm
under test produced 250,000 console messages in minutes. Whatever the answer, it is a rate
question - the missing boundary is the cause either way. The strudel layer also reports a leak
of its own on desktop (`potential memory leak detected`), which is a separate thread to pull
and may be a second, smaller source.
