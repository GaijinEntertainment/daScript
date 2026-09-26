# src/misc Architecture

## 1. File charters

- `job_que.cpp` - compute lanes, OS placement, dispatch and spin/park scheduling.
- `job_que_spin.h` - spin deadline construction, renewal and expiration; its clock type is
  supplied by the runtime or a deterministic test.
- `sysos.cpp` - the per-platform core-count probes `job_que.cpp` calls.
- `network.cpp` - the single-client TCP `Server` the DAP debugger and `daslib/network` sit on,
  the `Client` end beside it, `probe_local_port`, and the two helpers every socket error passes
  through.
- `dep_recorder.cpp` - what a run read, for a build system deciding whether a check has to run
  at all; armed by `DAS_DEPFILE` / `-MD`, silent otherwise.
- `alloc_tracker.cpp` - the RelWithDebInfo C++ heap leak tracker: the live-allocation map, the
  exit-time report, and the per-frame symbolizer. `alloc_tracker_overrides.cpp` beside it carries
  the global `operator new`/`delete` that feed it, compiled into every binary and shared module.

The knobs are bound to daslang in `src/builtin/module_builtin_jobque.cpp`; each knob's caller
contract is stated on its declaration in `include/daScript/misc/job_que.h`.

## 2. The default worker count is a three-source ladder

`jobque_thread_count` reads three sources in one order: the `DAS_JOBQUE_THREADS` environment
variable, then the app-declared request (`JobQue::set_default_threads`), then the platform
default. All three spell TOTAL compute lanes - N-1 workers plus the computing main thread - so
they are directly comparable. The request differs from `set_default_threads_cap` in direction: a
cap only lowers the platform default, a request can RAISE past it, which is what a prefill-heavy
workload needs to reach the slow tier. The cap still bounds an accepted request, so an explicit
"at most N" - a benchmark's `-t` - beats a standing request from a library `[init]`.

## 3. A second core tier is judged by KIND, not by size

`apple_slow_tier_is_compute` reads `hw.perflevel1.name` and matches it against an ALLOWLIST of
names known to be full compute tiers - "Performance" and "Super". Apple names the perflevels, and
the name is what says whether the tier is worth computing on: an M5 Max reports
"Super"/"Performance", both full compute tiers, so a batch pool gains from spanning them, while
earlier M-series report "Performance"/"Efficiency" and an Efficiency core straggles every
barrier-synchronized `parallel_for` it joins - measured ~1.6x slower on an 8P+2E prefill than the
same prefill on P cores alone. Counting the tiers cannot tell those two boxes apart; only the name
can.

The list is an allowlist rather than a denylist on "Efficiency" because the two failure directions
are not symmetric. An unrecognized future tier stays un-extended until someone vouches for it by
adding one string: the pool is then smaller than the box could support, which shows up as a plainly
visible idle-core deficit and is cheap to fix. A denylist instead extends the pool onto whatever
Apple ships next - a localized, renamed, or genuinely slow tier - and a straggler on a
barrier-synchronized pool is a silent regression that reads as ordinary slowness.

## 4. On darwin, thread placement is a QoS class

macOS publishes no thread-to-CPU pin, so `SetCurrentThreadAffinityCpu` sets a QoS class there
and ignores its cpu argument: the hard mode takes the top class, the hint mode the one below. On
a heterogeneous Mac, `jobque_apply_affinity_slot` gives the top class to the fast-tier slots only
- slot 0 is the dispatch caller, slots 1..perf-1 the first workers; under the default worker-limit
order those are exactly the active set, while `DAS_JOBQUE_LIMIT_ORDER=spread` ranks the active set
by the golden-stride walk instead, so most of its live workers then sit on demoted slots - the
spread A/B trades the tier placement away by design - and the next class down goes to the rest, so
the scheduler seats the surplus lanes on the slower tier. One class across every lane instead lets the scheduler dice the threads over the few
fast cores, which measures as a per-run token-generation placement lottery.

## 5. A socket error has one source per platform, and one reader

Winsock reports a failed socket call through `WSAGetLastError()` and leaves `errno` untouched;
POSIX reports it in `errno`. `network.cpp` reads the error only through `last_socket_error()`,
which returns whichever the platform set, and asks "retry later?" only through
`socket_would_block()`, which knows that the would-block code is `WSAEWOULDBLOCK` on Windows and
`EAGAIN`, `EWOULDBLOCK`, or `EINTR` elsewhere. `Server::send_msg` loops on would-block and closes
the client on any other error; `Client::send_msg` waits for the socket to drain instead, and a
peer that reads nothing for ten seconds is a closed connection, since the client runs on the
caller's only thread and a spin there stalls everything else; `tick` treats any other `recv`
error as a disconnect, and the client's `tick` stops once a callback closed the socket. A site that
read `errno` after a Winsock call would see 0 and treat a dead peer as "no error", so a send to
a disconnected client would retry forever while holding the debug-agent context lock, and the
tick that notices the closed socket could never run. `REVIEW.das` beside this file fails a
`network.cpp` that reads `errno` outside `last_socket_error()`, or names a would-block code
outside `socket_would_block()`. A peer that closed never signals the process: the accepted
socket carries `SO_NOSIGPIPE` on Apple and every send passes `MSG_NOSIGNAL` where the platform
defines it, so a write after the client went away is the `EPIPE` that `send_msg`'s error path
closes on, not a `SIGPIPE` that ends the debuggee - a disconnect request resumes every context
and their exit events race the client's close. Two more Winsock differences the client end
carries: a failed non-blocking connect is reported in `select`'s EXCEPT set, never the write
set, so `wait_writable_until` watches both and `SO_ERROR` names the outcome either way (POSIX
`poll` reports it as writable); and a specific-address `bind` succeeds beside a wildcard
listener on the same port, so `probe_local_port` binds the wildcard address whatever host it
was asked about - the question it answers is whether a listener, which binds the wildcard,
could take the port, and a wildcard bind is refused by any holder on every platform.

## 6. The leak dump runs last, so a static dtor's free is not a leak

`alloc_tracker.cpp` reports from an `atexit` handler, and heap that some other static destructor
is about to free is indistinguishable from a leak while that destructor has not run. The handler
therefore has to be registered FIRST, because `atexit` runs LIFO: `#pragma init_seg(lib)` puts the
registrar's constructor ahead of every user-level static on MSVC, and
`__attribute__((init_priority(101)))` does the same everywhere else. Losing that ordering does not
lose a leak, it invents one - every process-lifetime cache in the runtime (the dasbind late-bind
map, the dynamic-module registries, the JIT parallel-emit job vector) is freed by a static dtor or
by `Module::Shutdown`, so a dump that runs before them reports the whole set. A toolchain with
neither mechanism keeps the old ordering and over-reports; nothing else breaks.

What makes running last SAFE is that the tracker owns no destructible state: `getMap` and
`getMutex` placement-new into static storage and are never destructed, so a `track_free_hook`
arriving during static teardown - after the dump, at any point - still has a live map to tombstone
into.

## 7. A frame prints at the best tier that resolved, never as nothing

A frame falls back rather than vanishing: the symbol name plus its offset when the platform
resolver found one, else the module plus the frame's offset from its load base, else `?`. That
middle tier is what carries a POSIX build - `dladdr` reads `.dynsym` only, so every static and
hidden-visibility function in the runtime resolves to no symbol, and a report that printed `?` for
those would hide most of its own stacks behind manual base-address arithmetic. With the module and
offset in hand, `addr2line -f -C -e <module> <offset>` is the whole recovery. The MSVC arm carries
one tier the POSIX arm does not: it distrusts a symbol whose offset exceeds
`kMaxTrustedSymbolOffset`, because `SymFromAddr` answers with a distant neighbour where `dladdr`
answers with nothing.

## 8. The spin window reads the clock once per stride, for both of the things it needs it for {#spin-window-clock-stride}

A worker in the spin-before-park window (`JobQue::job`, opt-in via `setWorkerSpin`) wants the
clock twice: to extend the window when it served a team chunk, and to end the window when it
did not. Both happen at one place and one cadence - `JOBQUE_SPIN_DEADLINE_STRIDE` iterations -
off a single `JobQueSpinClock::now()`; the team-chunk arm only raises a flag the strided read then
acts on. Keeping the extend out of the loop body is the load-bearing half: in team mode the
worker does its WORK inside this loop, so a clock read on the served-a-chunk path fires per
chunk and lands between units of real work rather than in idle spin.

The stride is what that call costs. Native is 1 - the clock is a vDSO read, cheaper than the
`jobque_spin_pause` burst it guards - which leaves native behaviour exactly as it was, one read
per iteration. Native `JobQueSpinClock` remains `std::chrono::steady_clock`. Under emscripten,
`JobQueSpinClock` reads `emscripten_get_now()` directly as a double in milliseconds and keeps
its deadlines in that representation inside WASM. Browser builds avoid the pointer/integer
WASI `clock_time_get` JavaScript bridge, which boxes BigInts and, with debug checks, builds
temporary strings. Standalone Emscripten retains its own platform implementation of the clock.
The direct clock still crosses into JS for the browser performance clock, so the stride is 16.
`JobQueSpinClock` advertises `is_steady = false` because Emscripten can use
`Date.now` outside performance-clock contexts.
Fractional milliseconds are retained even at epoch-sized origins; spin deadlines do not need
integer nanoseconds. The WASM import/behavior regression is `web/test/jobque_spin_clock.cjs`.

A ZERO window strides 1 whatever the target and performs no clock reads, including at
construction. Team mode enters this loop on its own, so a worker with `spinUs == 0`
has to be free to park on the first test. `JobQueSpinDeadline` answers that test directly
without constructing or comparing timestamps. Striding that
case would hold it for a whole stride; extending a zero-length window on a served chunk would hold
it for one more iteration. Either is a spin the caller explicitly asked not to have, which is what
`jobque_spin_us = 0` in a box profile says. What a stride buys is only the resolution of the
window's trailing edge, which stays far finer than the window itself, and a worker that finds
work leaves the loop on the work, never on the clock.

`jobque_spin_pause` takes an emscripten arm for the same reason. Every other target has a
spin-wait hint that costs a few tens of cycles; wasm has none, and the `this_thread::yield()`
the generic arm would use reaches `sched_yield`, which crosses into JS and reads the clock - a
loop iteration then costs more than the chunk it is waiting for. A counted volatile loop gives
the same backoff with nothing but wasm in it. What makes either of these worth doing is that
this loop is where a team worker does its WORK: the pause sits between one `runTeamChunks` call
and the next, so its price is latency in front of the next chunk, not idle spin. The window is a
time budget, so a cheaper iteration does not shorten the window - it buys chunks taken sooner.

## 9. A recorded dependency is a source the compile read {#dependency-recording}

`dep_recorder.cpp` answers the question a build system asks before it runs a check: is there any
point? A C compile answers it by having the compiler report what it opened (`cc -MD -MF x.d`),
and ninja stats that list instead of compiling again. daslang reports the same way, from the
same position - it is the process that opens the files, so it is the only thing that knows which
modules a compile needed.

What it records is exactly that: the files served by the compile's `FileAccess` - the root, every
module it requires, transitively - plus the compile-time inputs a macro pinned through
`add_module_cache_dependency`. It is the require graph, written as one Make rule.

What it does NOT record is the point. A file a PROGRAM reads while it runs is not what its
compile required; a directory listing is not either; and a build artifact is the output of some
other edge, so naming one makes ninja build that edge to satisfy the check - while an artifact
the build rewrites, like the jitted object cache, leaves the check dirty for good. Deciding what
to rebuild is ninja's job, and it does it from this list; the list's only duty is to be the
compile's own inputs and nothing else.

Two rules survive from the shape of the thing. A path that no longer exists when the rule is
written is dropped, because ninja treats a missing dependency as dirty and the check would then
re-run forever. And the file holds exactly one rule - several naming one target is ninja's
deprecated `depfilemulti`, and an isolated sweep whose workers each wrote their own would
produce one per worker - so a process merges into the rule already there under a lock file. The
in-memory set shards by path hash: one lock over it serializes a parallel sweep.

The recorder is armed by `DAS_DEPFILE` or by `-MD -MF`, and unarmed it costs two relaxed atomic
loads per file. The environment form is the one a build edge uses, and the reason is reach: a
dastest worker subprocess, a `daslang -compile-only` a gate spawns per file, and a daslang-built
`.exe` all inherit a variable and none of them would see a flag on the parent's command line.
That is also why the recorder lives here, in the runtime library, rather than in the daslang CLI.
