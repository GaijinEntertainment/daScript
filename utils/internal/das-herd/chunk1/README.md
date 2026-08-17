# Chunk 1: fresh process-per-session overhead

This probe measures the deliberately simplest isolated topology before dasHerd
commits to it. For every command, the benchmark launches a fresh
`daslang.exe` worker. That worker compiles the session-host script, acknowledges
readiness through a flushed stdout pipe, and then owns the same ConPTY,
renderless terminal emulator, and raw log used by Chunk 0.

The three contemporaneous lanes are:

1. direct process capture plus raw log;
2. in-process ConPTY, terminal emulation, and raw log;
3. a fresh daScript worker process containing lane 2, with a minimal pipe
   acknowledgement standing in for the attach protocol.

Run from the repository root:

```text
bin\Release\daslang.exe utils\dasHerd\chunk1\benchmark_session_host.das -- --warmup=5 --rounds=30
```

Add `--discard` for disposable smoke runs. The worker's ready timestamp
includes OS process creation, daScript startup, compilation, module
initialization, and delivery of the first IPC byte. Hosted exit time is ready
time plus the worker's independently measured launch-to-target-exit time. The
250 ms conservative ConHost drain is included only in worker total, not hosted
exit.

## First measurement

Measured 2026-07-20 on the same machine and Release build as Chunk 0, using
five unrecorded warmups and 30 recorded samples per command.

| command | direct exit p50 / p95 | in-process terminal p50 / p95 | fresh host p50 / p95 | fresh-host delta |
| --- | ---: | ---: | ---: | ---: |
| `git --version` | 42.1 / 54.7 ms | 56.0 / 69.0 ms | 389.9 / 437.2 ms | +333.8 ms |
| colored `git status --short --branch` | 74.0 / 86.5 ms | 89.5 / 99.5 ms | 432.6 / 525.8 ms | +343.0 ms |
| intentional missing ref | 45.7 / 57.6 ms | 57.8 / 70.2 ms | 402.4 / 514.7 ms | +344.6 ms |

The worker-ready median was 339.8-340.4 ms. Its p95 was 376.7 ms for the first
command and rose to 435.8-450.8 ms during the longer repeated run. The terminal
operation inside the worker remained close to the in-process result: 55.3,
88.8, and 59.6 ms respectively. The additional latency is therefore fresh
daScript process startup and script compilation, not the PTY, emulator, log,
or pipe protocol.

One separately sampled status worker peaked at approximately 70.8 MiB working
set and 52.5 MiB private memory. This is an interpreted compiler process, so it
is an upper bound rather than a prediction for a small AOT/native host.

## Decision supported by this probe

Do not launch and compile an interpreted daScript worker for every short
command. That implementation adds roughly a third of a second per session and
substantial transient memory.

This result does not reject per-session process isolation. The next fair
topology measurement is a small AOT/native session-host bootstrap running the
same daScript session core. A persistent per-worktree host is the fallback if
an isolated compiled host still costs too much; pooling is not justified yet.
