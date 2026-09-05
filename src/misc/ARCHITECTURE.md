# src/misc Architecture

## 1. File charters

- `job_que.cpp` - how many compute lanes a `JobQue` starts with, and where the OS puts them.
- `sysos.cpp` - the per-platform core-count probes `job_que.cpp` calls.
- `network.cpp` - the single-client TCP `Server` the DAP debugger and `daslib/network` sit on,
  and the two helpers every socket error passes through.

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
`EAGAIN`, `EWOULDBLOCK`, or `EINTR` elsewhere. `send_msg` loops on would-block and closes the
client on any other error; `tick` treats any other `recv` error as a disconnect. A site that
read `errno` after a Winsock call would see 0 and treat a dead peer as "no error", so a send to
a disconnected client would retry forever while holding the debug-agent context lock, and the
tick that notices the closed socket could never run. `REVIEW.das` beside this file fails a
`network.cpp` that reads `errno` outside `last_socket_error()`, or names a would-block code
outside `socket_would_block()`.
