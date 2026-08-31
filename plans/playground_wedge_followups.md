# playground wedge handling - follow-ups and standing caveats

The wedge arc (deferred `Module::Shutdown` under a live browser loop + the bounded
main-thread join in `waitForJob`) fixed the nightly's interpreter wedges. What it
deliberately did NOT settle is ledgered here.

## Blast radius of the bounded join (`waitForJob`, module_builtin_jobque.cpp)

Active only under `__EMSCRIPTEN__ && __EMSCRIPTEN_PTHREADS__` AND on the browser main
thread: the threaded playground interpreter and the daspkg `release wasm` builds (those
always link `-pthread`). Native, single-threaded wasm, worker-thread joins,
`waitForJobWithTimeout`, and every Channel/LockBox/Stream wait are untouched.

- **False-positive class: a single legitimately long job.** Progress is the remaining-count
  moving, so ONE job that runs longer than the 10s window - or jobs starved behind another
  dispatch's long jobs on the 4-worker wasm pool - trips "join deadlock avoided" on a join
  that would have finished. Interpreted wasm is 10-50x slower than native, so a >10s single
  job is reachable from user code. No shipped sample joins mid-run (the labs poll `isReady`
  and only join at shutdown behind the epoch bail). If a legitimate trip ever surfaces:
  soften to log-and-keep-waiting on the first window and throw on the second, or widen the
  window - the constant is local to `waitForJob`.
- **Aftermath of a trip is bounded by refcounts.** Late-completing jobs land on a still-valid
  status; `job_status_remove` refuses while refs are held. Worst case is a leaked JobStatus
  plus its leak-report line - no use-after-free.

## Blast radius of the deferred Module::Shutdown (utils/daslang/main.cpp)

Emscripten `daslang` binary only, browser-loop path only. Non-loop wasm programs and the
compiled wasm64 cards (own entry point) are unchanged.

- **Unverified seam: a second `callMain` in one wasm instance while the first run's loop is
  live.** The superseded-loop path leaves modules alive on purpose (the next program is
  about to compile), so run 2 calls `Module::Initialize` on already-initialized modules.
  Believed idempotent; not exercised - the playground runs one program per frame, and the
  frame refuses second runs. Verify before any embedder resurrects multi-run
  daslang-in-wasm.
- The handle-leak dump for browser-loop programs moved from end-of-main (reporting off) to
  loop end (reporting on) - leak lines now appear when the program actually finishes.

## The structural gap: user code can still freeze the page

`while true {}` in pasted code wedges the tab today: the interpreter runs on the run
frame's main thread, same-origin frames share the tab's thread, and a frozen thread runs
no parent-side watchdog - there is nothing to click. The bounded join covers the jobque
deadlock class only.

Structural fix: run daslang main on a pthread (`-sPROXY_TO_PTHREAD`, plus offscreen
canvas/framebuffer for the GL path). The page thread stays live, the parent gets a real
kill/restart switch (destroying the frame already terminates its workers), and every
blocking join becomes legal on what is then a real thread. Own arc: GLFW event proxying,
AudioWorklet interplay, and the pthread-pool budget all need the build-and-browser loop.
The debug rig from the wedge arc applies (threaded `web/build_mt` staged into
`site/playground`, COOP/COEP server over `site/`, Chromium trace + name-section
symbolication - mind the bare `-s` in CMakeCommon's Release flags stripping wasm names).
