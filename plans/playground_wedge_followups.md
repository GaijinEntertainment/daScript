# playground wedge handling - follow-ups and standing caveats

The wedge arc (deferred `Module::Shutdown` under a live browser loop + the refcount-gated
main-thread join in `waitForJob`) fixed the nightly's interpreter wedges. What it
deliberately did NOT settle is ledgered here.

## The bounded join (`waitForJob`, module_builtin_jobque.cpp)

Active only under `__EMSCRIPTEN__ && __EMSCRIPTEN_PTHREADS__` AND on the browser main
thread: the threaded playground interpreter and the daspkg `release wasm` builds. Native,
single-threaded wasm, worker-thread joins, `waitForJobWithTimeout`, and every
Channel/LockBox/Stream wait are untouched.

- **The throw fires only when nothing can ever notify**: a stall window with
  `refCount() <= 1` — every dispatched job/thread holds a ref via the capture macros, so
  the only throwable state is appended-but-never-dispatched (the wedge class). A long or
  starved job holds a ref and the join waits forever, as on native; after the first stall
  window it logs one stderr line (mirrored to the devtools console by the run frame).
  This gate is what makes the throw safe: with no holders there is no `with_*` scope guard
  left to terminate through and no later writer into the joined status's stack frame.
- **Remaining stall subclass, by design**: a join held by genuinely stuck work (a job
  parked on a channel the joiner was supposed to fill) still freezes the tab — visible in
  the console via the stall line, not recoverable in-page. The structural fix is the
  `PROXY_TO_PTHREAD` arc below.
- **Channel/Stream blocking pops on the main thread are a sibling wedge class** —
  `for_each_clone` over a channel nothing fills parks the tab with no bound at all. Not
  covered by this arc.

## The deferred Module::Shutdown and loop teardown (utils/daslang/main.cpp)

Emscripten `daslang` binary only, browser-loop path only.

- **Teardown order at the loop's natural end**: script `shutdown()` (exceptions now
  printed, not swallowed) → bounded global-que drain (3s) → Context delete → deferred
  `Module::Shutdown`. On a drain timeout the loop's Context AND the modules are
  deliberately leaked with a log line — freeing memory under running jobs is heap
  corruption, and `~Module_JobQue`/`~JobQue` join workers unbounded. One program per frame
  makes the leak inert in the playground; a long-lived multi-run embedder would accumulate.
- **Unverified seam: a second `callMain` in one wasm instance while the first run's loop is
  live.** The superseded path leaves modules alive on purpose, so run 2 reaches
  `Module::Initialize` on already-initialized modules and `g_envTotal` drifts up by one
  (suppressed leak dumps, atexit audit trip on exit). No current embedding can do it — the
  run frame refuses second runs, `_interp.html` and the node test call `callMain` once.
- **Thread affinity**: the deferred `Module::Shutdown` runs on the thread that services the
  emscripten main loop, which today is the thread that ran `Module::Initialize`. The
  `PROXY_TO_PTHREAD` arc moves daslang main onto a pthread — the tick's shutdown must move
  with it or `daScriptEnvironment`'s thread-local bound env is null there.

## The structural gap: user code can still freeze the page

`while true {}` in pasted code wedges the tab: the interpreter runs on the run frame's main
thread, same-origin frames share the tab's thread, and a frozen thread runs no parent-side
watchdog. The refcount gate narrows the join case to stuck-work-only; compute loops and
blocking pops remain.

Structural fix: run daslang main on a pthread (`-sPROXY_TO_PTHREAD`, plus offscreen
canvas/framebuffer for the GL path). The page thread stays live, the parent gets a real
kill/restart switch (destroying the frame already terminates its workers), and every
blocking join becomes legal on what is then a real thread. Own arc: GLFW event proxying,
AudioWorklet interplay, and the pthread-pool budget all need the build-and-browser loop.
The debug rig from the wedge arc applies (threaded `web/build_mt` staged into
`site/playground`, COOP/COEP server over `site/`, Chromium trace + name-section
symbolication - mind the bare `-s` in CMakeCommon's Release flags stripping wasm names).
