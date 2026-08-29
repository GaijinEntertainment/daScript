# The wasm browser lifecycle has no GC boundary

Every `options gc` program driven by the browser three-call lifecycle grows its heap
without bound until the wasm 2 GiB ceiling, then traps. Nothing collects.

## The defect

`maybe_collect_gc` (`modules/dasLiveHost/live/live_gc.das:19`) states the contract in its own
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
  - `src/builtin/module_builtin_runtime.cpp:1947` `main_loop_arg` - `eval_main_loop`'s
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

## The fix (shipped)

`Context::collectHeapIfMostlyFree(LineInfo * at = nullptr)` - one method, called by both
drivers after `update()` returns and before the loop-control decision. It is the C++ twin of
`maybe_collect_gc`, threshold for threshold: collect both heaps when the string heap is more
than a third unused, else when the heap is more than two thirds unused; otherwise nothing.
It returns false and touches nothing on a context without `options gc` +
`options persistent_heap`, so the boundary is inert for every program that did not opt in
(`heap_collect` would throw there; this must not).

The four questions, as settled:

1. **Where** - in the ticks themselves. The tick is what replaced the `main` loop, so it
   carries the `main` loop's duty. Both ticks call it only on the keep-going path; a frame
   that ends the loop goes straight to `shutdown()`.
2. **What** - `maybe_collect_gc`'s thresholds exactly, so desktop and web collect on the
   same policy and a program cannot behave differently by platform.
3. **Opt-out** - the `persistent`/`gcEnabled` flags on the context, read directly; no
   function presence is consulted.
4. **Cost** - a collection is cheap by construction when the thresholds admit it (a mostly-free
   heap has little live data to walk); the physarum soak below shows no frame-rate change.

Covered by `tests-cpp/small/test_lifecycle_gc.cpp`: 200 frames of a 64 KB junk array grow the
heap past 6 MB, one call reclaims it more than fourfold, a second pass holds, and a context
without `options gc` declines without throwing. The tick sites are `__EMSCRIPTEN__`-only and
have no native test; the browser measurements below are their proof.

## The desktop control (run 2026-08-29)

The same source on desktop, where the program's own `main` loop calls `maybe_collect_gc()`
every frame, holds flat: RSS 419 MB across a three-minute run, `STAT RN` at 131.8% CPU
throughout, so the process was working rather than stalled. That rules out the one hypothesis
that would have made the missing boundary irrelevant - if the growth were LIVE data, collection
would reclaim nothing and desktop would climb too. It does not. The heap is collectable garbage,
and the only difference is who calls the collector.

## Verification (run 2026-08-29, local Chromium, both drivers)

A minimal fixture (`options gc` + `persistent_heap`, `update()` allocating one 64 KB array and
a short string per frame, nothing retained) through each driver, before and after:

| driver | before | after |
|---|---|---|
| interpreter (`web_loop_tick`, `daslang_static`) | 64 KB/frame, 3.8 MB -> 2002 MB by frame 32,040, page at 4.2 GB | flat at 68 KB through frame 18,900 |
| native control (`main` loop + `maybe_collect_gc`) | - | flat at 68 KB through frame 1,800 |

Physarum itself on the compiled card (`jit_web_lifecycle_tick`), after: main-context heap flat
at 14.2 MB across 25,200 sim frames, with transient bumps to 16.4 MB that collect within one
sample; string heap flat at 52 bytes. Total page memory by
`performance.measureUserAgentSpecificMemory()` held at 480-481 MB over the soak, where the
pre-fix rate of ~1.2 MB/s would have added over 100 MB in the first 88 seconds alone. Main
context was the whole leak; the strudel layer and the job-clone contexts contributed nothing
measurable.

Still owed: the nightly playground sweep's Physarum Lab cell going green and staying green,
which is the only instrument that runs unattended.

## Resolved on the way

Whether the leak rate differed between the per-frame `new_thread` dispatch and the persistent
job queue was never settled - the control run was instrumented with `-sPTHREADS_DEBUG=1` and
drowned in its own logging. It no longer matters: the cause was the missing boundary, and with
it in place the card holds flat under the job queue. The job-clone contexts were also cleared
by reading: the non-pooled path frees each clone with the job, the pooled path calls
`restartHeaps()` on reuse. Strudel's own desktop leak report (`potential memory leak
detected`, ~99 KB) is real but tiny and separate; it did not show in the browser totals.
